/**
 * An open-addressed hash table fundamentally consists of a
 * single array. The structure `hash_table` is the
 * open-addressed hash table data structure. This
 * structure consists of four methods: 
 *  
 * `h1`, `h2`, `match`, and `destroy` are methods used to
 * encapsulate the functions passed to `HT_init`;
 */

#include <stddef.h>

#ifndef _OPEN_ADDRESSED_HASH_TABLE
#define _OPEN_ADDRESSED_HASH_TABLE

struct hash_table {

    /* METHODS */
    size_t (*h1)(const void* key);
    size_t (*h2)(const void* key);
    int (*match)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    void* _info;
};

#endif /* _OPEN_ADDRESSED_HASH_TABLE */