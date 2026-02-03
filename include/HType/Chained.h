/**
 * A chained hash table consists of an array of buckets.
 * Each bucket is a linked list containing the elements
 * that hash to a certain position in the table. The structure
 * `hash_table` is the chained hash table data structure.
 * This structure consists of six members: `buckets` is
 * the number of buckets allocated in the table;
 * `hash`, `match`, and `destroy` are members used to
 * encapsulate the functions passed to `HT_init`;
 * `size` is the number of elements currently in the table;
 * and `table` is the array of buckets.
 */

#ifndef CHAINED_HASHTABLE_H
#define CHAINED_HASHTABLE_H

#include "../SinglyList.h"

struct hash_table {

    size_t (*hash)(const void* key);
    int (*match)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    void* _info;
};

#endif /* CHAINED_HASHTABLE_H */