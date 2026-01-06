/**
 * An open-addressed hash table fundamentally consists of a single array. The 
 * structure `hash_table` is the open-addressed hash table data structure. This
 * structure consists of eight members: `positions` is the number of positions allocated
 * in the hash table; `vacated` is a pointer that will be initialized to a special
 * storage location to indicate that a particular position in the table has had an
 * element removed from it; `h1`, `h2`, `match`, and `destroy` are members used to
 * encapsulate the functions passed to `HT_init`; `size` is the number of elements
 * currently in the table; and `table` is the array in which the elements are stored.
 */

#ifndef _OPEN_ADDRESSED_HASH_TABLE
#define _OPEN_ADDRESSED_HASH_TABLE

struct hash_table {

    void** table;
    void* vacated;

    /* METHODS */
    int (*h1)(const void* key);
    int (*h2)(const void* key);
    int (*match)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    int size;
    int positions;
};

#endif /* _OPEN_ADDRESSED_HASH_TABLE */