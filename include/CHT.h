#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "HType/Chained.h"

typedef struct hash_table HT;

/**
 * Returns the number of elements currently stored in the hash table.
 * 
 * @param ht Pointer to the hash table whose size will be returned.
 * 
 * @return  Number of elements in the hash table on success, or a negative
 *          error code.
 */
ssize_t HT_size(const HT* ht);

/**
 * Returns a human-readable description of the last error that occurred
 * in the hash table operations.
 * 
 * @param ht    Pointer to the hash table whose last error description
 *              will be returned.
 * 
 * @return Constant string describing the last error code stored in the hash table,
 *         or `NULL` if the container is invalid or uninitialized.
 */
const char* HT_error(const HT* ht);

/**
 * Initializes the hash table specified by `ht`.
 * This operation must be called for a hash table before the
 * hash table can be used with any other operation. The number
 * of slots allocated in the hash table is specified by `slots`.
 * 
 * The function pointer `hash` specifies a user-defined hash
 * function for hashing keys.
 * 
 * The function pointer `match` specifies a user-defined
 * function to determine whether two keys match.
 * It should return `1` if `key1` is equal to `key2`, and
 * `0` otherwise.
 * 
 * The `destroy` argument provides a way to free
 * dynamically allocated data when `HT_destroy` is called.
 * For example, if the hash table contains data dynamically
 * allocated using `malloc`, destroy should be set to `free` to
 * free the data as the hash table is destroyed.
 * For structured data containing several dynamically allocated
 * members, `destroy` should be set to a user-defined function
 * that calls `free` for each dynamically allocated member as
 * well as for the structure itself. For a hash table
 * containing data that should not be `freed`, `destroy` should
 * be set to `NULL`.
 * 
 * @param ht        Pointer to the hash table structure to be initialized.
 * @param slots     Number of slots (buckets) to allocate in the hash table.
 * @param hash      Function pointer to user-defined hash function that takes a key
 *                  and returns an integer hash value.
 * @param match     Optional destructor called on each element during destroy.
 * @param destroy   Optional comparison function used by search operations.
 * 
 * @return `CONTAINER_SUCCESS` if initializing the hash table is successful, error code otherwise.
 */
int HT_init(HT* ht, int slots, size_t (*hash)(const void* key), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the hash table specified by `ht`.
 * No other operations are permitted after calling
 * `HT_destroy` unless `HT_init` is called again.
 * The `HT_destroy` operation removes all elements from
 * a hash table and calls the function passed as `destroy`
 * to `HT_init` once for each element as it is removed,
 * provided destroy was not set to `NULL`.
 * 
 * @param ht    Pointer to the hash table structure to be destroyed.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int HT_destroy(HT* ht);

/**
 * Inserts an element into the hash table specified by `ht`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element
 * remains in the hash table.
 * 
 * It is the responsibility of the
 * caller to manage the storage associated with `data`.
 * 
 * @param ht    Pointer to the hash table into which the element will be inserted.
 * @param data  Pointer to the data to be inserted into the hash table.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int HT_insert(HT* ht, void* data);

/**
 * Removes the element matching `data` from the hash table specified by `ht`.
 * It is the responsibility of the caller to manage the storage associated with the data.
 * 
 * @return A pointer to the data that was stored in the removed element, or `NULL`
 * if no element with the specified `key` is found.
 */
int HT_remove(HT* ht, const void* src, void** dst);

/**
 * Finds the element matching `data` in the hash table specified by `ht`.
 * Returns a pointer to the data stored in the matching element, or `NULL` if no
 * element with the specified `key` is found. The returned pointer remains valid
 * as long as the element remains in the hash table, and the caller should not
 * modify or free the data through this pointer.
 * 
 * @return A pointer to the found data, or `NULL` if the key was not found.
 */
int HT_lookup(const HT* ht, const void* src, void** dst);

#endif /* _HASHTABLE_H */