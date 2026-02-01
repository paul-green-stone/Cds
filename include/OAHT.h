#include "HType/Open.h"

#include <stddef.h>
#include <sys/types.h>

typedef struct hash_table HT;

/**
 * Initializes the hash table specified by `ht`.
 * This operation must be called for an hash table
 * before the hash table can be used with any other
 * operation. The number of positions to be allocated
 * in the hash table is specified by `positions`.
 * 
 * The function pointers `h1` and `h2` specify user-defined
 * auxiliary hash functions for double hashing.
 * 
 * The function pointer `match` specifies a user-defined
 * function to determine if two keys match.
 * 
 * The `destroy` argument provides a way to free dynamically
 * allocated data when `HT_destroy` is called.
 * 
 * For an hash table containing data that should not be freed,
 * `destroy` should be set to `NULL`.
 * 
 * @param ht        Pointer to the hash table structure to initialize.
 * @param positions Number of hash table positions to allocate.
 * @param h1        Primary hash function for double hashing, returns initial
 *                  index.
 * @param h2        Secondary hash function for double hashing, used to compute 
 *                  probe offsets
 * @param match     Comparison function returning `1` if two keys match,
 *                  `0` otherwise.
 * @param destroy   Cleanup function to free data when table is destroyed,
 *                  or `NULL`.
 * 
 * @return `CONTAINER_SUCCESS` if initializing the hash table is successful,
 *          error code otherwise.
 */
int HT_init(HT* ht, size_t positions, size_t (*h1)(const void* key), size_t (*h2)(const void* key), int (*match)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the hash table specified by `ht`.
 * No other operations are permitted after calling
 * `HT_destroy` unless `HT_init` is called again.
 * 
 * The `HT_destroy` operation removes all elements
 * from a hash table and calls the function passed
 * as `destroy` to `HT_init` once for each element
 * as it is removed, provided `destroy` was not set to `NULL`.
 * 
 * @param ht Pointer to the hash table structure to destroy.
 * 
 * @return `CONTAINER_SUCCESS` if initializing the hash table is successful,
 *          error code otherwise.
 */
int HT_destroy(HT* ht);

/**
 * Inserts an element into the hash table specified
 * by `ht`. The new element contains a pointer to `data`,
 * so the memory referenced by `data` should remain valid as
 * long as the element remains in the hash table.
 * 
 * It is * the responsibility of the caller to manage the
 * storage associated with data.
 * 
 * @param ht    Pointer to the initialized hash table where the element will be inserted.
 * @param data  Pointer to the data to insert into the hash table.
 * 
 * @return `CONTAINER_SUCCESS` if inserting the element is successful, `1` if the element is already
 * in the hash table, or a negative error code otherwise.
 */
int HT_insert(HT* ht, const void* data);

/**
 * Removes the element matching `data` from the hash table
 * specified by `ht`.
 * 
 * 
 * It is the responsibility of the caller to manage the storage associated with the data.
 * 
 * @return A pointer to the data that was stored in the removed element, or `NULL`
 * if no element with the specified `key` is found.
 */
int HT_remove(HT* ht, const void* src, void** dst);

/**
 * Finds the element matching `data` in the hash table
 * specified by `ht`.
 * 
 * @param ht    Pointer to the initialized hash table to search.
 * @param src   Pointer to the key data to search for in the hash table.
 * @param dst   Pointer to a pointer where the matching data will be stored.
 * 
 * @return A pointer to the found data, or `NULL` if the key was not found.
 */
int HT_lookup(const HT* ht, const void* src, void** dst);

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
