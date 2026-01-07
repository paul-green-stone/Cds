#include <stdlib.h>
#include <string.h>

#include "../include/OAHT.h"

static char vacated;

int HT_init(HT* ht, int positions, int (*h1)(const void* key), int (*h2)(const void* key), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    if ((ht->table = calloc(positions, sizeof(void*))) == NULL) {
        return -1;
    }

    ht->positions = positions;
    ht->vacated = &vacated;

    ht->h1 = h1;
    ht->h2 = h2;
    ht->match = match;
    ht->destroy = destroy;
    ht->size = 0;

    /* ======== */
    return 0;
}

void HT_destroy(HT* ht) {

    if (ht->destroy != NULL)  {

        for (size_t i = 0; i < ht->positions; i++) {

            if ((ht->table[i] != NULL) && (ht->table[i] != ht->vacated)) {
                ht->destroy(ht->table[i]);
            }
        }
    }

    free(ht->table);
    memset(ht, 0, sizeof(HT));
}

int HT_insert(HT* ht, const void* data) {

    int hash_code;
    /* ======== */

    if (ht->size == ht->positions) {
        return -1;
    }

    if (HT_lookup(ht, data) != NULL) {
        return 1;
    }

    for (size_t i = 0; i < ht->positions; i++) {

        hash_code = (ht->h1(data) + (i * ht->h2(data))) % ht->positions;

        if ((ht->table[hash_code] == NULL) || (ht->table[hash_code] == ht->vacated)) {

            ht->table[hash_code] = (void*) data;
            ht->size++;
            /* ======== */
            return 0;
        }
    }

    /* ======== */
    return -1;
}

void* HT_remove(HT* ht, const void* data) {

    int hash_code;
    void* _data = NULL;
    /* ======== */

    for (size_t i = 0; i < ht->positions; i++) {

        hash_code = (ht->h1(data) + (i * ht->h2(data))) % ht->positions;

        if (ht->table[hash_code] == NULL) {
            return NULL;
        }
        else if (ht->match(ht->table[hash_code], data)) {

            _data = ht->table[hash_code];
            ht->table[hash_code] = ht->vacated;
            ht->size--;
            /* ======== */
            break ;
        }
    }

    /* ======== */
    return _data;
}

void* HT_lookup(const HT* ht, const void* data) {

    int hash_code;
    void* _data = NULL;
    /* ======== */

    for (size_t i = 0; i < ht->positions; i++) {

        hash_code = (ht->h1(data) + (i * ht->h2(data))) % ht->positions;

        if (ht->table[hash_code] == NULL) {
            return NULL;
        }
        else if (ht->match(ht->table[hash_code], data)) {

            _data = ht->table[hash_code];
            break ;
        }
    }

    /* ======== */
    return _data;
}