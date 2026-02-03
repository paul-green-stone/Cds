#include <string.h>
#include <stdio.h>

#include "../include/CHT.h"

#define _htable (((struct information*) (container)->_info)->table)
#define _htbuckets (((struct information*) (container)->_info)->buckets)
#define _htsize (((struct information*) (container)->_info)->size)
#define _hterror (((struct information*) (container)->_info)->last_error_code)

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

int HT_init(HT* container, int buckets, size_t (*hash)(const void* data), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    struct information* info = NULL;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============== The container has been initialized ============== */
    if (container->_info != NULL) {

        _hterror = CONTAINER_ERROR_ALREADY_INIT;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_INIT;
    }

    /* ======================= Memory allocation ======================= */
    if ((info = calloc(1, sizeof(struct information))) == NULL) {
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    if ((info->table = calloc(buckets, sizeof(sList))) == NULL) {

        free(info);
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    /* ====================== Initializing lists ====================== */
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

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================ The container is uninitialized ================ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

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

    /* =============== Make sure the method is available =============== */
    if (container->hash == NULL) {

        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }
    
    /* ======== Do nothing if the data is already in the table ======== */
    if (HT_lookup(container, data, &_data) == CONTAINER_SUCCESS) {
        
        _hterror = CONTAINER_ERROR_ALREADY_EXISTS;
        /* ======== */
        return 1;
    }

    hash_code = container->hash(data) % _htbuckets;

    if ((ret_code = sList_insert_first(&_htable[hash_code], data)) == CONTAINER_SUCCESS) {
        _htsize++;
    }

    /* ======== */
    return ret_code;
}

/* ================================================================ */

int HT_remove(HT* container, const void* src, void** dst) {

    sNode* node = NULL;
    
    size_t hash_code;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ============== Make sure the methods are available ============== */
    if ((container->hash == NULL) || (container->match == NULL)) {

        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }

    hash_code = container->hash(src) % _htbuckets;

    if (sList_find(&_htable[hash_code], src, &node, container->match) != CONTAINER_SUCCESS) {

        _hterror = CONTAINER_ERROR_NOT_FOUND;
        /* ======== */
        return CONTAINER_ERROR_NOT_FOUND;
    }

    sList_remove(&_htable[hash_code], node, dst);
    _htsize--;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int HT_lookup(const HT* container, const void* src, void** dst) {

    size_t hash_code;
    sNode* node = NULL;
    int exit_code = CONTAINER_SUCCESS;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= The container is initialized ================= */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* =============== Make sure the method is available =============== */
    if ((container->hash == NULL) || (container->match == NULL)) {

        _hterror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }

    hash_code = container->hash(src) % _htbuckets;
    printf("Hash code = %d\n", hash_code);
    if (sList_find(&_htable[hash_code], src, &node, container->match) == CONTAINER_SUCCESS) {

        printf("Hey!\n");
        *dst = sNode_data(node);
        _hterror = (exit_code = CONTAINER_SUCCESS);
    }
    else {
        _hterror = (exit_code = CONTAINER_ERROR_NOT_FOUND);
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

const char* HT_error(const HT* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return NULL;
    }

    /* ================ The container is uninitialized ================ */
    if (container->_info == NULL) {
        return NULL;
    }

    /* ======== */
    return (container ? descriptions[-_hterror] : NULL);
}

/* ================================================================ */

ssize_t HT_size(const HT* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================ The container is uninitialized ================ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ======== */
    return _htsize;
}
