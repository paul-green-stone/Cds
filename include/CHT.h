#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "HType/Chained.h"

typedef struct hash_table HT;

/**
 * Initializes the hash table specified by `ht`. This operation
 * must be called for a hash table before the hash table can be used
 * with any other operation. The number of slots allocated in the hash table is
 * specified by `slots`. The function pointer `hash` specifies a user-defined hash
 * function for hashing keys. The function pointer `match` specifies a user-defined
 * function to determine whether two keys match. It should return `1` if `key1` is 
 * equal to `key2`, and `0` otherwise. The `destroy` argument provides a way to free
 * dynamically allocated data when `HT_destroy` is called. For example, if the hash table
 * contains data dynamically allocated using `malloc`, destroy should be set to `free` to
 * free the data as the hash table is destroyed. For structured data containing several
 * dynamically allocated members, `destroy` should be set to a user-defined function
 * that calls `free` for each dynamically allocated member as well as for the structure
 * itself. For a hash table containing data that should not be `freed`, destroy should
 * be set to `NULL`.
 * 
 * @return `0` if initializing the hash table is successful, or `–1` otherwise.
 */
int HT_init(HT* ht, int slots, int (*hash)(const void* key), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the hash table specified by `ht`. No other operations are
 * permitted after calling `HT_destroy` unless `HT_init` is called again.
 * The `HT_destroy` operation removes all elements from a hash table and calls the
 * function passed as `destroy` to `HT_init` once for each element as it is removed,
 * provided destroy was not set to `NULL`.
 * 
 * @return None.
 */
void HT_destroy(HT* ht);

/**
 * Inserts an element into the hash table specified by `ht`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element
 * remains in the hash table. It is the responsibility of the
 * caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the element is successful, `1` if the element
 * is already in the hash table, or `–1` otherwise
 */
int HT_insert(HT* ht, const void* data);

/**
 * Removes the element matching `data` from the hash table specified by `ht`.
 * It is the responsibility of the caller to manage the storage associated with the data.
 * 
 * @return A pointer to the data that was stored in the removed element, or `NULL`
 * if no element with the specified `key` is found.
 */
void* HT_remove(HT* ht, const char* data);

/**
 * Finds the element matching `data` in the hash table specified by `ht`.
 * Returns a pointer to the data stored in the matching element, or `NULL` if no
 * element with the specified `key` is found. The returned pointer remains valid
 * as long as the element remains in the hash table, and the caller should not
 * modify or free the data through this pointer.
 * 
 * @return A pointer to the found data, or `NULL` if the key was not found.
 */
void* HT_lookup(const HT* ht, const char* data);

#endif /* _HASHTABLE_H */