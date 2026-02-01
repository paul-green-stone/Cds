#include <stdlib.h>
#include <string.h>

#include "../include/CdsErrors.h"
#include "../include/OAHT.h"

#define _hterror (((struct information*) (container)->_info)->last_error_code)
#define _htpositions (((struct information*) (container)->_info)->positions)
#define _htable (((struct information*) (container)->_info)->table)
#define _htsize (((struct information*) (container)->_info)->size)
#define _htvacated (((struct information*) (container)->_info)->vacated)

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

static char vacated;

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
    "Output pointer is null",
    "Container has not been initialized",
    "Data already exists in container"
};

/**
 * `positions` is the number of positions allocated
 * in the hash table;
 * 
 * `vacated` is a pointer that will be initialized to
 * a special storage location to indicate that a
 * particular position in the table has had an element
 * removed from it;
 * 
 * `size` is the number of elements currently in the table;
 * 
 * `table` is the array in which the elements are stored.
 */

struct information {

    void** table;
    void* vacated;

    size_t positions;
    size_t size;

    int last_error_code;
};

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

void foo(const HT* container) {

    for (size_t i = 0; i < _htpositions; i++) {

        printf("[%ld]: ", i);
        if (_htable[i]) printf("%d", *((int*) _htable[i]));
        printf("\n");
    }
}

int HT_init(HT* container, size_t positions, size_t (*h1)(const void* key), size_t (*h2)(const void* key), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    struct information* info = NULL;
    /* ======== */

    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    if (container->_info != NULL) {

        _hterror = CONTAINER_ERROR_ALREADY_INIT;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_INIT;
    }

    if ((info = calloc(1, sizeof(struct information))) == NULL) {
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    if ((info->table = calloc(positions, sizeof(void*))) == NULL) {

        free(info);
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    info->positions = positions;
    info->last_error_code = CONTAINER_SUCCESS;
    info->size = 0;
    info->vacated = NULL;

    container->_info = info;
    container->h1 = h1;
    container->h2 = h2;
    container->match = match;
    container->destroy = destroy;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int HT_destroy(HT* container) {

   if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    for (size_t i = 0; i < _htpositions; i++) {

        if (container->destroy != NULL) {
            container->destroy(_htable[i]); 
        }
    }

    free(_htable);
    free(container->_info);
    /* Mark it uninitialized */
    container->_info = NULL;
    
    memset(container, 0, sizeof(HT));

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int HT_insert(HT* container, const void* data) {

    int hash_code;
    void* _data = NULL;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ============ The container has unoccupied positions ============ */
    if (_htsize == _htpositions) {

        _hterror = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    /* ========== The container aready has the specified data ========== */
    if (HT_lookup(container, data, &_data) == CONTAINER_SUCCESS) {

        _hterror = CONTAINER_ERROR_ALREADY_EXISTS;
        /* ======== */
        return 1;
    }

    /* ==================== Computing the hash code ==================== */
    for (size_t i = 0; i < _htpositions; i++) {

        hash_code = (container->h1(data) + (i * container->h2(data))) % _htpositions;

        if ((_htable[hash_code] == NULL) || (_htable[hash_code] == _htvacated)) {

            _htable[hash_code] = (void*) data;
            _htsize++;
            /* ======== */
            break ;
        }
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

int HT_remove(HT* container, const void* src, void** dst) {

    int exit_code = CONTAINER_ERROR_NOT_FOUND;
    int hash_code;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ==================== Computing the hash code ==================== */
    for (size_t i = 0; i < _htpositions; i++) {

        hash_code = (container->h1(src) + (i * container->h2(src))) % _htpositions;

        if (_htable[hash_code] == NULL) {

            _hterror = CONTAINER_ERROR_NOT_FOUND;
            /* ======== */
            return CONTAINER_ERROR_NOT_FOUND;
        }
        else if (container->match(_htable[hash_code], src) == 1) {

            *dst = _htable[hash_code];
            _htable[hash_code] = _htvacated;
            _htsize--;
            
            exit_code = CONTAINER_SUCCESS;
            /* ======== */
            break ;
        }
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

int HT_lookup(const HT* container, const void* src, void** dst) {

    int exit_code = CONTAINER_ERROR_NOT_FOUND;
    int hash_code;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    if (src == NULL) {

        _hterror = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    /* ================== The container is not empty ================== */
    if (_htsize == 0) {

        _hterror = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    for (size_t i = 0; i < _htpositions; i++) {

        hash_code = (container->h1(src) + (i * container->h2(src))) % _htpositions;

        if (_htable[hash_code] == NULL) {

            _hterror = (exit_code = CONTAINER_ERROR_NOT_FOUND);
            /* ======== */
            break ;
        }
        else if (container->match(_htable[hash_code], src) == 1) {

            *dst = _htable[hash_code];
            _hterror = (exit_code = CONTAINER_SUCCESS);
            /* ======== */
            break ;
        }
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

ssize_t HT_size(const HT* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ======== */
    return _htsize;
}

/* ================================================================ */

const char* HT_error(const HT* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return NULL;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return NULL;
    }

    /* ======== */
    return descriptions[_hterror];
}