#include <string.h>
#include <stdio.h>

#include "../include/CHT.h"

int HT_init(HT* ht, int slots, int (*hash)(const void* data), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    if ((ht->table = (sList*) malloc(slots * sizeof(sList))) == NULL) {
        return -1;
    }

    ht->buckets = slots;

    for (size_t i = 0; i < ht->buckets; i++) {
        sList_init(&ht->table[i], destroy);
    }

    ht->hash = hash;
    ht->match = match;
    ht->destroy = destroy;

    ht->size = 0;

    /* ======== */
    return 0;
}

void HT_destroy(HT* ht) {

    for (size_t i = 0; i < ht->buckets; i++) {
        sList_destroy(&ht->table[i]);
    }

    free(ht->table);

    memset(ht, 0, sizeof(HT));
}

int HT_insert(HT* ht, const void* data) {

    int hash_code;
    int ret_code;
    /* ======== */

    if (ht->hash == NULL) {
        return -1;
    }
    
    /* Do nothing if the data is already in the table */
    if (HT_lookup(ht, data) != NULL) {
        return 1;
    }

    hash_code = ht->hash(data) % ht->buckets;

    if ((ret_code = sList_insert_first(&ht->table[hash_code], data)) == 0) {
        ht->size++;
    }

    /* ======== */
    return ret_code;
}

void* HT_remove(HT* ht, const char* data) {

    int hash_code;

    void* _data = NULL;
    Node* n;
    /* ======== */

    if (ht->hash == NULL) {
        return NULL;
    }

    hash_code = ht->hash(data) % ht->buckets;

    if ((n = sList_find(&ht->table[hash_code], data, ht->match)) ==  NULL) {
        return NULL;
    }

    _data = sList_remove(&ht->table[hash_code], n);
    ht->size--;

    /* ======== */
    return _data;
}

void* HT_lookup(const HT* ht, const char* data) {

    int hash_code;
    Node* n = NULL;
    /* ======== */

    if (ht->hash == NULL) {
        return NULL;
    }

    hash_code = ht->hash(data) % ht->buckets;

    /* ======== */
    return (n = sList_find(&ht->table[hash_code], data, ht->match)) == NULL ? NULL : n->data;
}