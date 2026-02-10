#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "../include/HType/Open.h"
#include "../include/OAHT.h"
#include "../include/Dict.h"

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

typedef struct {

    const char* key;
    void* data;
} Dict_ent;

size_t h1_fnv1a(const void* data) {

    uint32_t hash_code = 0x811c9dc5u;   // FNV offset basis
    uint32_t prime = 0x01000193u;       // FNV prime

    const char* key = ((Dict_ent*) data)->key;
    /* ======== */
    
    while (*key) {

        hash_code ^= (uint32_t)(unsigned char)*key++;
        hash_code *= prime;
    }

    /* ======== */
    return hash_code;
}

size_t h2_djb2(const void* data) {

    uint32_t hash_code = 5381;
    int c;

    const char* key = ((Dict_ent*) data)->key;
    /* ======== */
    
    while ((c = *key++)) {
        hash_code = ((hash_code << 5) + hash_code) + c;
    }
    
    /* ======== */
    return hash_code;
}

int key_match(const void* key1, const void* key2) {

    int t;
    /* ======== */
    return (t = strcmp(((Dict_ent*) key1)->key, ((Dict_ent*) key2)->key)) == 0 ? 1 : t;
}

/* ================================================================ */
/* ======================== IMPLEMENTATION ======================== */
/* ================================================================ */

Dict* Dict_create(int logical_size) {

    HT* dict;
    /* ======== */

    if ((dict = calloc(1, sizeof(HT))) == NULL) {
        return NULL;
    }

    if (HT_init(dict, logical_size, h1_fnv1a, h2_djb2, key_match, free) == -1) {

        free(dict);
        dict = NULL;
    }

    /* ======== */
    return dict;
}

/* ================================================================ */

int Dict_destroy(Dict** container) {

    /* =============== Make sure the container is valid =============== */
    if (*container == NULL || container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    HT_destroy(*container);
    free(*container);
    *container = NULL;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int Dict_insert(Dict* container, const void* key, void* _data) {

    Dict_ent* ent = NULL;
    void* data = NULL;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= Make sure key and data valid ================= */
    if ((key == NULL) || (_data == NULL)) {
        return CONTAINER_ERROR_NULL_DATA;
    }

    if ((ent = calloc(1, sizeof(Dict_ent))) == NULL) {
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    ent->key = key;
    ent->data = _data;

    HT_lookup(container, (void*) ent, &data);

    if (data != NULL) {

        free(ent);
        /* ======== */
        return CONTAINER_ERROR_ALREADY_EXISTS;
    }

    /* ======== */
    return HT_insert(container, ent);
}

/* ================================================================ */

int Dict_remove(Dict* container, const char* key, void** _data) {

    Dict_ent ent = {key, NULL};
    Dict_ent* ret_ent = NULL;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= Make sure key and data valid ================= */
    if ((key == NULL) || (_data == NULL)) {
        return CONTAINER_ERROR_NULL_DATA;
    }

    HT_remove(container, &ent, (void**) &ret_ent);
    
    if (ret_ent == NULL) {
        return CONTAINER_ERROR_NOT_FOUND;
    }

    *_data = ret_ent->data;

    container->destroy(ret_ent);

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int Dict_lookup(const Dict* container, const char* key, void** result) {

    Dict_ent ent = {key, NULL};
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ================= Make sure key and data valid ================= */
    if ((key == NULL) || (result == NULL)) {
        return CONTAINER_ERROR_NULL_DATA;
    }

    HT_lookup(container, &ent, result);
    *result = ((Dict_ent*) *result)->data;

    if (*result == NULL) {
        return CONTAINER_ERROR_NOT_FOUND;
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

ssize_t Dict_size(const Dict* dict) {
    return HT_size(dict);
}
