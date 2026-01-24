#include "../include/SinglyList.h"
#include "../include/CdsErrors.h"

#include <sys/types.h>

#define CACHE_SIZE 16

/**
 * Macros for accessing internal members of the singly
 * linked list. These are used within the source file to
 * extract information like size, head, and tail of the list.
 */

/**
 * Get a container size.
 */
#define _lsize(container) (((struct information*) (container)->_info)->size)

/**
 * Get a container head node.
 */
#define _lhead(container) (((struct information*) (container)->_info)->head)

/**
 * Get a container tail node.
 */
#define _ltail(container) (((struct information*) (container)->_info)->tail)

/**
 * Get an error code of the last container operation.
 */
#define _lerror(container) (((struct information*) (container)->_info)->last_error_code)

/**
 * Get the cache structure from container metadata.
 */
#define _lcache(container) (((struct information*) (container)->_info)->cache)

/* ================================================================ */
/* ============================= NODE ============================= */
/* ================================================================ */

/**
 * 
 */
typedef struct s_node {

    void* data;
    /* Prevents removing foreign nodes and allows O(1) membership validation */
    sList* sentinel;

    struct s_node* next;
} sNode;

static const char* descriptions[] = {
    "Success",
    "Container pointer is null",
    "Failed to allocate memory",
    "Data pointer is null",
    "Container is empty",
    "Node does not belong to this list",
    "No callback function available",
    "Data not found",
    "Container already initialized",
    "Output pointer is null"
};

/* ================================================================ */
/* ============================ CACHE ============================= */
/* ================================================================ */

typedef struct cache {

    sNode* _cache[CACHE_SIZE];
    
    size_t free_slots;
    size_t free_slot_idx;
} Cache;

/* ================================================================ */

/**
 * Clear all cache slots, reset free_slots and `free_slot_idx` for reuse.
 */
#define _free_cache                                 \
    do {                                            \
        for (size_t i = 0; i < CACHE_SIZE; i++) {   \
            _lcache(container)._cache[i] = NULL;    \
        }                                           \
        _lcache(container).free_slots = CACHE_SIZE; \
        _lcache(container).free_slot_idx = 0;       \
    } while (0)

/**
 * Find first free (`NULL`) slot in cache. Sets `result` = index or `-1`
 * if cache full. Uses lexical 'container' from calling scope. 
 * Early exit on first free slot.
 */
#define _find_free_slot(result)                                                     \
    do {                                                                            \
        result = -1;                                                                \
        for (size_t i = 0; i < CACHE_SIZE; i++) {                                   \
            if (_lcache(container)._cache[i] == NULL) { result = i; break ; }       \
        }                                                                           \
    } while (0)

/**
 * Find cached predecessor node whose next points to target 'node'.
 * Sets `result` = predecessor or `NULL` if not cached. Skips empty 
 * slots. Enables O(1) unlink.
 */
#define _find_cache_node(node, result)                              \
    do {                                                            \
        result = NULL;                                              \
        for (size_t i = 0; i < CACHE_SIZE; i++) {                   \
            if (_lcache(container)._cache[i] == NULL) continue ;    \
            else if (_lcache(container)._cache[i]->next == (node)) {\
                (result) = _lcache(container)._cache[i];            \
                break ;                                             \
            }                                                       \
        }                                                           \
    } while (0)                                                     \

/**
 * Remove specific node from cache by pointer match. Sets matching
 * slot to `NULL`. Early exit on first match. Used after node
 * removal to prevent stale pointers.
 */
#define _clear_node(node)                                   \
    do  {                                                   \
        for (size_t i = 0; i < CACHE_SIZE; i++) {           \
            if (_lcache(container)._cache[i] == (node)) {   \
                _lcache(container)._cache[i] = NULL;        \
                break ;                                     \
            }                                               \
        }                                                   \
    } while (0)                                             \

/**
 * Cache node using FIFO eviction. Skips if node already cached
 * Advances `free_slot_idx` circularly: slot[N] = slot[0] on wrap.
 */
#define _save_cache(node)                                   \
    do {                                                    \
        int found = 0;                                      \
        for (ssize_t i = 0; i < CACHE_SIZE; i++) {          \
            if (_lcache(container)._cache[i] == node) {     \
                found = 1;                                  \
            }                                               \
        }                                                   \
        if (!found) {                                       \
            _lcache(container)._cache[_lcache(container).free_slot_idx++ % CACHE_SIZE] = (node);    \
        }                                                   \
    } while (0)                                             \

/**
 * Find first cached node matching 'data' via `_match` callback from
 * calling scope. Sets `node` = matching cached node or leaves
 * uninitialized if no cache hit. O(1) average case for repeated
 * find→remove patterns.
 */
#define _find_cache_data(data, node)                                \
    do {                                                            \
        for (size_t i = 0; i < CACHE_SIZE; i++) {                   \
            if (_lcache(container)._cache[i] == NULL) continue ;    \
            void* n_data = sNode_data(_lcache(container)._cache[i]);\
            if (_match(data, n_data) == 1) {                        \
                (node) = _lcache(container)._cache[i]; break ;      \
            }                                                       \
        }                                                           \
    } while (0)                                                     \

/**
 * Internal singly linked container metadata.
 * Stores head, tail pointers, element count, and the last error state.
 * Intended for internal use only to prevent accidental modification
 * of container internals by external code.
 */
struct information {

    Cache cache;

    sNode* head;
    sNode* tail;

    size_t size;

    int last_error_code;
};

/**
 * Allocate and initialize a node.
 */
static sNode* _create_node(void* data) {

    sNode* node = NULL;
    /* ======== */

    if ((node = calloc(1, sizeof(sNode))) == NULL) { return NULL; }

    node->data = data;

    /* ======== */
    return node;
}

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

int sList_init(sList* container, void (*destroy)(void* data), int (*match)(const void* key1, const void* key2)) {

    struct information* info = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (container->_info != NULL) {

        _lerror(container) = CONTAINER_ERROR_ALREADY_INIT;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_INIT;
    }

    /* Initialize the container */
    if ((info = calloc(1, sizeof(struct information))) == NULL) {
        
        _lerror(container) = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    container->_info = info;
    _lsize(container) = 0;
    _lhead(container) = NULL;
    _ltail(container) = NULL;
    _lerror(container) = 0;
    _lcache(container).free_slots = CACHE_SIZE;

    container->destroy = destroy;
    container->match = match;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_destroy(sList* container) {

    void* data = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    while (_lsize(container)) {

        /* The function call decrements the size of the container */
        sList_remove_first(container, &data);

        /* Call a user-defined function to free dynamically allocated data */
        if (container->destroy != NULL) { container->destroy(data); }
    }

    _lsize(container) = 0;
    _lhead(container) = NULL;
    _ltail (container) = NULL;
    _lerror(container) = 0;
    _free_cache;

    /* Destroy the internal container holding list state information */
    free(container->_info);

    container->destroy = NULL;
    container->match = NULL;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_insert_last(sList* container, void* data) {

    sNode* node = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (data == NULL) {
        
        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    /* Allocate storage for the element */
    if ((node = _create_node(data)) == NULL) {

        _lerror(container) = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    if (_lsize(container) == 0) {
        _lhead(container) = _ltail(container) = node;
    }
    else {

        _ltail(container)->next = node;
        _ltail(container) = node;
    }

    node->sentinel = container;
    _lsize(container)++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_insert_first(sList* container, void* data) {

    sNode* node = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (data == NULL) {
        
        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    if ((node = _create_node(data)) == NULL) {

        _lerror(container) = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    if (_lsize(container) == 0) {
        _lhead(container) = _ltail(container) = node;
    }
    else {
        node->next = _lhead((container));
        _lhead(container) = node;
    }

    node->sentinel = container;
    _lsize(container)++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_remove_last(sList* container, void** data) {

    sNode* previous = NULL;
    sNode* current = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (data == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    /* Do not allow removal from an empty container */
    if (_lsize(container) > 0) {

        *data = _ltail(container)->data;
        current = _ltail(container);

        if (_lsize(container) == 1) {
            _lhead(container) = _ltail(container) = NULL;
        }
        else {
            for (previous = _lhead(container), current = previous->next; current != _ltail(container); previous = current, current = current->next) ;

            _ltail(container) = previous;
            _ltail(container)->next = NULL;
        }

        current->sentinel = NULL;
        free(current);

        _lsize(container)--;
        _lerror(container) = 0;
    }
    else {
        
        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_remove_first(sList* container, void** data) {

    sNode* node = NULL;
    int exit_code = CONTAINER_SUCCESS;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (data == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    /* Do not allow removal from an empty container */
    if (_lsize(container) > 0) {

        node = _lhead(container);
        *data = node->data;

        if (_lsize(container) == 1) {
            _lhead(container) = _ltail(container) = NULL;
        }
        else {
            _lhead(container) = _lhead(container)->next;
        }

        _lerror(container) = 0;
        /* Adjust the size of the container to account for the removed element */
        _lsize(container)--;

        node->next = NULL;
        node->sentinel = NULL;
        /* Free the storage allocated by the node */
        free(node);
    }
    else {
        
        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        exit_code = CONTAINER_ERROR_EMPTY;
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

int sList_find(const sList* container, const void* data, sNode** dest, int (*match)(const void* key1, const void* key2)) {

    sNode* prev = NULL;
    sNode* cached_node = NULL;
    int exit_code = CONTAINER_SUCCESS;
    *dest = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    int (*_match)(const void* key1, const void* key2) = (match) ? match : container->match;

    if (_lsize(container) == 0) {

        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    if (!_match) {

        _lerror(container) = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }

    if (data == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    if (dest == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_OUTPUT;
        /* ======== */
        return CONTAINER_ERROR_NULL_OUTPUT;
    }

    /* ================================================================ */

    /* Looking in the cache first */
    _find_cache_data(data, cached_node);
    if (cached_node != NULL) {
        *dest = cached_node;
    }
    /* ================================================================ */
    else {
        for (sNode* node = _lhead(container), *prevn = NULL; node != NULL; prevn = node, node = node->next) {

            if (_match(node->data, data) == 1) {

                /* Store the node and the one that precedes it into the cache */
                _save_cache(node);
                _save_cache(prev);

                *dest = node;
                /* ======== */
                break ;
            }

            prev = node;
        }
    }

    if (*dest == NULL) {

        _lerror(container) = CONTAINER_ERROR_NOT_FOUND;
        exit_code = CONTAINER_ERROR_NOT_FOUND;
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

int sList_remove(sList* container, sNode* node, void** data) {

    sNode* previous = NULL;
    sNode* current = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (node == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    if (data == NULL) {

        _lerror(container) = CONTAINER_ERROR_NULL_OUTPUT;
        /* ======== */
        return CONTAINER_ERROR_NULL_OUTPUT;
    }

    if (_lsize(container) == 0) {

        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    if (node->sentinel != container) {

        _lerror(container) = CONTAINER_ERROR_INVALID_NODE;
        /* ======== */
        return CONTAINER_ERROR_INVALID_NODE;
    }

    if (node == _lhead(container)) { return sList_remove_first(container, data); }

    if (node == _ltail(container)) { return sList_remove_last(container, data); }

    /* ================================================================ */

    /* Looking in the cache first */
    _find_cache_node(node, previous);
    if (previous != NULL) {
        current = node;
        _clear_node(node);
    }
    /* ================================================================ */
    else {
        for (previous = _lhead(container), current = previous->next; current != node; previous = current, current = current->next) ;
    }

    previous->next = current->next;

    *data = node->data;
    node->next = NULL;
    node->sentinel = NULL;
    free(node);

    _lsize(container)--;
    _lerror(container) = 0;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_insert_after(sList* container, sNode* node, void* data) {

    sNode* _node = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (_lsize(container) == 0) {

        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    if ((data == NULL) || (node == NULL)) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    if (node->sentinel != container) {

        _lerror(container) = CONTAINER_ERROR_INVALID_NODE;
        /* ======== */
        return CONTAINER_ERROR_INVALID_NODE;
    }

    if (node == _ltail(container)) { return sList_insert_last(container, data); }

    if ((_node = _create_node(data)) == NULL) {

        _lerror(container) = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    _node->next = node->next;
    _node->sentinel = container;
    node->next = _node;

    _lsize(container)++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int sList_insert_before(sList* container, sNode* node, void* data) {

    sNode* previous = NULL, *current = NULL;
    sNode* _node = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (_lsize(container) == 0) {

        _lerror(container) = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    if ((data == NULL) || (node == NULL)) {

        _lerror(container) = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    if (node->sentinel != container) {

        _lerror(container) = CONTAINER_ERROR_INVALID_NODE;
        /* ======== */
        return CONTAINER_ERROR_INVALID_NODE;
    }

    if (node == _lhead(container)) { return sList_insert_first(container, data); }

    /* Allocate memory for a new node */
    if ((_node = _create_node(data)) == NULL) {

        _lerror(container) = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    /* ================================================================ */

    /* Looking in the cache first */
    _find_cache_node(node, previous);
    if (previous != NULL) {
        current = node;
    }
    /* ================================================================ */
    else {
        for (previous = _lhead(container), current = previous->next; current != node; previous = current, current = current->next) ;
    }

    previous->next = _node;
    _node->sentinel = container;
    _node->next = node;

    _lsize(container)++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

const char* sList_error(const sList* container) {
    return container ? descriptions[-_lerror(container)] : NULL;
}

sNode* sList_head(const sList* container) {
    return (container != NULL ? _lhead(container) : NULL);
}

sNode* sList_tail(const sList* container) {
    return (container != NULL ? _ltail(container) : NULL);
}

ssize_t sList_size(const sList* container) {
    return (container != NULL ? _lsize(container) : -1);
}

/* ================================================================ */
/* ============================= NODE ============================= */
/* ================================================================ */

void* sNode_data(const sNode* node) {
    return (node ? node->data : NULL);
}

sNode* sNode_next(const sNode* node) {
    return (node ? node->next : NULL);
}

/* ================================================================ */
