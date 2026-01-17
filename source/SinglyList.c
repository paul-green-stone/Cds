#include "../include/SinglyList.h"

/**
 * 
 */
#define SLIST_CORRUPTED(list) \
    (((list)->size == 0 && ((list)->head || (list)->tail)) || \
      ((list)->size > 0 && (!(list)->head || !(list)->tail)) )

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

static char* descriptions[] = {
    "operation was successful",
    "the provided list pointer is NULL",
    "the provided node pointer is NULL",
    "the provided data pointer is NULL",
    "the specified node does not belong to the list",
    "no comparison function is provided for search operations",
    "memory allocation failed",
    "the list structure is corrupted",
    "the list is empty, no data available to remove",
    "the requested data could not be found in the list"
};


/* Forward declare */
struct sList;

typedef struct s_node {

    void* data;
    /* Prevents removing foreign nodes and allows O(1) membership validation */
    sList* sentinel;

    struct s_node* next;
} sNode;

static sNode* _create_node(void* data) {

    sNode* node = NULL;
    /* ======== */

    if ((node = calloc(1, sizeof(sNode))) == NULL) {
        return NULL;
    }

    node->data = data;

    /* ======== */
    return node;
}

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

sListResult sList_init(sList* list, void (*destroy)(void* data), int (*match)(const void* key1, const void* key2)) {

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    list->size = 0;
    list->head = list->tail = NULL;
    list->destroy = destroy;
    list->match = match;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

/* ================================================================ */

sListResult sList_destroy(sList* list) {

    void* data = NULL;
    /* ======== */

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        return (list->last_error = SLIST_ERR_CORRUPTED);
    }

    while (list->size > 0) {

        /* The function call decrements the size of the list */
        data = sList_remove_first(list);

        if ((list->destroy != NULL) && (data != NULL)) { list->destroy(data); }
    }

    list->head = list->tail = NULL;
    list->destroy = NULL;
    list->match = NULL;
    /* Don’t rely on side effects */
    list->size = 0;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

/* ================================================================ */

sListResult sList_insert_last(sList* list, void* data) {

    sNode* node = NULL;
    /* ======== */

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        return (list->last_error = SLIST_ERR_CORRUPTED);
    }

    if (data == NULL) {
        return (list->last_error = SLIST_ERR_NULL_DATA);
    }

    if ((node = _create_node(data)) == NULL) {
        return (list->last_error = SLIST_ERR_NO_MEMORY);
    }

    if (list->size == 0) {
        list->head = list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }

    node->sentinel = list;
    list->size++;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

/* ================================================================ */

sListResult sList_insert_first(sList* list, void* data) {

    sNode* node = NULL;
    /* ======== */

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        return (list->last_error = SLIST_ERR_CORRUPTED);
    }

    if (data == NULL) {
        return (list->last_error = SLIST_ERR_NULL_DATA);
    }

    if ((node = _create_node(data)) == NULL) {
        return (list->last_error = SLIST_ERR_NO_MEMORY);
    }

    if (list->size == 0) {
        list->head = list->tail = node;
    }
    else {
        node->next = list->head;
        list->head = node;
    }

    node->sentinel = list;
    list->size++;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

/* ================================================================ */

void* sList_remove_last(sList* list) {

    sNode* previous = NULL;
    sNode* current = NULL;
    void* data = NULL;
    /* ======== */

    if (list == NULL) {
        return NULL;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        list->last_error = SLIST_ERR_CORRUPTED;
        /* ======== */
        return NULL;
    }

    /* Do not allow removal from an empty list */
    if (list->size > 0) {

        data = list->tail->data;
        current = list->tail;

        if (list->size == 1) {
            list->head = list->tail = NULL;
        }
        else {
            for (previous = list->head, current = previous->next; current != list->tail; previous = current, current = current->next) ;

            list->tail = previous;
            list->tail->next = NULL;
        }

        current->sentinel = NULL;
        free(current);
        list->size--;
        list->last_error = SLIST_OK;
    }
    else {
        list->last_error = SLIST_ERR_EMPTY;
    }

    /* ======== */
    return data;
}

/* ================================================================ */

void* sList_remove_first(sList* list) {

    sNode* node = NULL;
    void* data = NULL;
    /* ======== */

    if (list == NULL) {
        return NULL;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        list->last_error = SLIST_ERR_CORRUPTED;
        /* ======== */
        return NULL;
    }

    /* Do not allow removal from an empty list */
    if (list->size > 0) {

        data = list->head->data;
        node = list->head;

        if (list->size == 1) {
            list->head = list->tail = NULL;
        }
        else {
            list->head = list->head->next;
        }

        list->last_error = SLIST_OK;
        node->sentinel = NULL;
        free(node);
        list->size--;
    }
    else {
        list->last_error = SLIST_ERR_EMPTY;
    }

    /* ======== */
    return data;
}

/* ================================================================ */

const sNode* sList_find(const sList* list, const void* data, int (*match)(const void* key1, const void* key2)) {

    sList* mutable = list;
    /* ======== */

    if (list == NULL) {
        return NULL;
    }

    int (*_match)(const void* key1, const void* key2) = (match) ? match : list->match;

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        mutable->last_error = SLIST_ERR_CORRUPTED;
        /* ======== */
        return NULL;
    }

    if ((list->size == 0)) {
        mutable->last_error = SLIST_ERR_EMPTY;
        /* ======== */
        return NULL;
    }

    if (!_match) {
        mutable->last_error = SLIST_ERR_NO_MATCH_FUNCTION;
        /* ======== */
        return NULL;
    }

    if (data == NULL) {
        mutable->last_error = SLIST_ERR_NULL_DATA;
        /* ======== */
        return NULL;
    }

    for (sNode* t = list->head; t != NULL; t = t->next) {

        if (_match(t->data, data) == 1) {
            mutable->last_error = SLIST_OK;
            /* ======== */
            return t;
        }
    }

    mutable->last_error = SLIST_ERR_MISSING;

    /* ======== */
    return NULL;
}

/* ================================================================ */

void* sList_remove(sList* list, sNode* node) {

    sNode* previous = NULL;
    sNode* current = NULL;

    void* data = NULL;
    /* ======== */

    if (list == NULL) {
        return NULL;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        list->last_error = SLIST_ERR_CORRUPTED;
        /* ======== */
        return NULL;
    }

    if (list->size == 0) {
        list->last_error = SLIST_ERR_EMPTY;
        /* ======== */
        return NULL;
    }

    if (node == NULL) {
        list->last_error = SLIST_ERR_NULL_NODE;
        /* ======== */
        return NULL;
    }

    if (node->sentinel != list) {
        list->last_error = SLIST_ERR_FOREIGN_NODE;
        /* ======== */
        return NULL;
    }

    if (node == list->head) { return sList_remove_first(list); }

    if (node == list->tail) { return sList_remove_last(list); }

    for (previous = list->head, current = previous->next; current != node; previous = current, current = current->next) ;

    data = node->data;
    previous->next = current->next;
    current->next = NULL;

    node->sentinel = NULL;
    free(node);
    list->size--;
    list->last_error = SLIST_OK;

    /* ======== */
    return data;
}

/* ================================================================ */

sListResult sList_insert_after(sList* list, sNode* node, void* data) {

    sNode* t = NULL;
    sNode* n = NULL;
    /* ======== */

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        return (list->last_error = SLIST_ERR_CORRUPTED);
    }

    if ((list->size == 0)) {
        return (list->last_error = SLIST_ERR_EMPTY);
    }

    if (data == NULL) {
        return (list->last_error = SLIST_ERR_NULL_DATA);
    }

    if (node == NULL) {
        return (list->last_error = SLIST_ERR_NULL_NODE);
    }

    if (node->sentinel != list) {
        return (list->last_error = SLIST_ERR_FOREIGN_NODE);
    }

    if (node == list->tail) { return sList_insert_last(list, data); }

    if ((n = _create_node(data)) == NULL) {
        return (list->last_error = SLIST_ERR_NO_MEMORY);
    }

    for (t = list->head; t != node; t = t->next) ;

    n->sentinel = list;
    n->next = t->next;
    node->next = n;

    list->size++;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

/* ================================================================ */

sListResult sList_insert_before(sList* list, sNode* node, void* data) {

    sNode* t = NULL;
    sNode* n = NULL;
    /* ======== */

    if (list == NULL) {
        return SLIST_ERR_NULL_LIST;
    }

    /* Detect corruption */ 
    if (SLIST_CORRUPTED(list)) {
        return (list->last_error = SLIST_ERR_CORRUPTED);
    }

    if ((list->size == 0)) {
        return (list->last_error = SLIST_ERR_EMPTY);
    }

    if (data == NULL) {
        return (list->last_error = SLIST_ERR_NULL_DATA);
    }

    if (node == NULL) {
        return (list->last_error = SLIST_ERR_NULL_NODE);
    }

    if (node->sentinel != list) {
        return (list->last_error = SLIST_ERR_FOREIGN_NODE);
    }

    if (node == list->head) { return sList_insert_first(list, data); }

    if ((n = _create_node(data)) == NULL) {
        return (list->last_error = SLIST_ERR_NO_MEMORY);;
    }

    for (t = list->head; t->next != node; t = t->next) ;

    n->sentinel = list;
    n->next = node;
    t->next = n;

    list->size++;

    /* ======== */
    return (list->last_error = SLIST_OK);
}

const char* sList_get_last_error(const sList* list) {
    return list ? descriptions[list->last_error] : NULL;
}

/* ================================================================ */
/* ============================= NODE ============================= */
/* ================================================================ */

void* sNode_data(const sNode* node) {
    return node ? node->data : NULL;
}

const sNode* sNode_next(const sNode* node) {
    return node ? node->next : NULL;
}

/* ================================================================ */
