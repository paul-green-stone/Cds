#include <string.h>
#include <stdio.h>

#include "../include/CHT.h"

#define _hterror (((struct information*) (container)->_info)->last_error_code)
#define _htbuckets (((struct information*) (container)->_info)->buckets)
#define _htable (((struct information*) (container)->_info)->table)
#define _htsize (((struct information*) (container)->_info)->size)

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

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

struct information {

    sList* table;

    ssize_t buckets;
    ssize_t size;

    int last_error_code;
};

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

void foo(HT* container, void (*print)(void* d)) {

    for (size_t i = 0; i < _htbuckets; i++) {
        printf("[%ld]: ", i);
        for (sNode* node = sList_head(&_htable[i]); node != NULL; node = sNode_next(node)) {
            print(sNode_data(node));

            if (sNode_next(node) != NULL) printf(", ");
        }
        printf("\n");
    }
}

/* ================================================================ */

int HT_init(HT* container, int buckets, int (*hash)(const void* data), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    struct information* info = NULL;
    /* ======== */

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    if (container->_info != NULL) {

        _hterror = CONTAINER_ERROR_ALREADY_INIT;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_INIT;
    }

    if ((info = calloc(1, sizeof(struct information))) == NULL) {
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    if ((info->table = calloc(buckets, sizeof(sList))) == NULL) {

        free(info);
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < buckets; i++) {
        sList_init(&info->table[i], destroy, match);
    }

    info->buckets = buckets;

    container->_info = info;
    container->hash = hash;
    container->match = match;
    container->destroy = destroy;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int HT_destroy(HT* container) {

    if (container == NULL) { return CONTAINER_ERR_NULL_PTR; }

    for (size_t i = 0; i < _htbuckets; i++) {
        sList_destroy(&_htable[i]);
    }

    free(_htable);
    free(container->_info);

    memset(container, 0, sizeof(HT));

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int HT_insert(HT* container, void* data) {

    size_t hash_code;
    int ret_code = CONTAINER_SUCCESS;
    /* ======== */

    if (container->hash == NULL) {
        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }
    
    /* Do nothing if the data is already in the table */
    if (HT_lookup(container, data) != NULL) {
        _hterror = CONTAINER_ERROR_ALREADY_EXISTS;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_EXISTS;
    }

    hash_code = container->hash(data) % _htbuckets;

    if ((ret_code = sList_insert_first(&_htable[hash_code], data)) == CONTAINER_SUCCESS) {
        _htsize++;
    }

    /* ======== */
    return ret_code;
}

/* ================================================================ */

void* HT_remove(HT* container, const void* data) {

    size_t hash_code;

    void* _data = NULL;
    sNode* node;
    /* ======== */

    if (container->hash == NULL) {

        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return NULL;
    }

    hash_code = container->hash(data) % _htbuckets;

    if (sList_find(&_htable[hash_code], data, &node, container->match) != CONTAINER_SUCCESS) {

        _hterror = CONTAINER_ERROR_NOT_FOUND;
        /* ======== */
        return NULL;
    }

    sList_remove(&_htable[hash_code], node, &_data);
    _htsize--;

    /* ======== */
    return _data;
}

/* ================================================================ */

void* HT_lookup(const HT* container, const void* data) {

    size_t hash_code;
    sNode* node = NULL;
    /* ======== */

    if (container->hash == NULL) {
        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return NULL;
    }

    hash_code = container->hash(data) % _htbuckets;

    /* ======== */
    return (sList_find(&_htable[hash_code], data, &node, container->match)) == CONTAINER_SUCCESS ? sNode_data(node) : NULL;
}

/* ================================================================ */

const char* HT_error(const HT* container) {
    return (container ? descriptions[-_hterror] : NULL);
}
