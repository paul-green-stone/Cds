#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int h1_fnv1a(const void* data) {

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

int h2_djb2(const void* data) {

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

void Dict_destroy(Dict** dict) {

    HT_destroy(*dict);
    free(*dict);
    *dict = NULL;
}

int Dict_insert(Dict* dict, const void* key, void* data) {

    Dict_ent* ent = calloc(1, sizeof(Dict_ent));

    if (ent == NULL) {
        return -1;
    }

    ent->key = key;
    ent->data = data;

    if (HT_lookup(dict, (void*) ent) != NULL) {

        free(ent);
        /* ======== */
        return 1;
    }

    /* ======== */
    return HT_insert(dict, ent);
}

void* Dict_remove(Dict* dict, const char* key) {

    Dict_ent ent = {key, NULL};
    void* data = NULL;
    Dict_ent* ret_ent = NULL;
    /* ======== */

    ret_ent = HT_remove(dict, &ent);
    data = ret_ent->data;

    dict->destroy(ret_ent);

    /* ======== */
    return data;
}

void* Dict_lookup(const Dict* dict, const char* key) {

    Dict_ent ent = {key, NULL};
    /* ======== */
    return HT_lookup(dict, &ent);
}

void* Dict_getElm(const Dict* d, int pos_idx) {
    return d->table[pos_idx] == d->vacated ? NULL : d->table[pos_idx] != NULL ? ((Dict_ent*) d->table[pos_idx])->data : NULL;
}

const char** Dict_keys(const Dict* d) {

    const char** keys = NULL;
    /* ======== */

    keys = calloc(d->size, sizeof(const char*));

    for (size_t t_idx = 0, k_idx = 0; k_idx < d->size; t_idx++) {

        const char* _ = d->table[t_idx] == d->vacated ? NULL : d->table[t_idx] != NULL ? ((Dict_ent*) d->table[t_idx])->key : NULL;

        if (_) {
            keys[k_idx++] = _;
        }
    }

    /* ======== */
    return keys;
}
