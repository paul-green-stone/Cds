#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include "HType/Open.h"
#include "OAHT.h"

typedef HT Dict;

/**
 * Returns the number of elements currently stored in the dictionary specified by `dict`.
 */
#define asize(dict) ((dict)->size)

/**
 * Returns the total number of positions (capacity) available in the dictionary specified by `dict`.
 * This represents the maximum number of elements that can be stored in the dictionary.
 */
#define lsize(dict) ((dict)->positions)

/**
 * Creates an empty dictionary with the specified initial `size`.
 * This operation must be called for a dictionary before the dictionary
 * can be used with any other operation. Returns a pointer to the newly
 * allocated dictionary, or `NULL` if the allocation fails. The caller
 * takes ownership of the returned dictionary and must call `Dict_destroy`
 * on it followed by `free` to release both the dictionary structure
 * and its contents.
 * 
 * @return A pointer to the newly allocated dictionary, or `NULL` on failure.
 */
Dict* Dict_create(int size);

/**
 * Destroys the dictionary specified by `dict`. This operation frees all
 * dynamically allocated resources associated with the dictionary structure
 * and its elements, but does not free the data stored in the elements.
 * Users must manually delete all data they have previously added before
 * calling this function. After the call, `dict` is set to `NULL`.
 */
void Dict_destroy(Dict** dict);

/**
 * Inserts `data` into the dictionary specified by `dict` under the specified `key`.
 * The memory referenced by `key` and `data` should remain valid as long as the
 * element remains in the dictionary. It is the responsibility of the caller to
 * manage the storage associated with `key` and `data`.
 * 
 * @return `0` if inserting the element is successful, `1` if an element
 * with the specified `key` already exists, or `-1` otherwise.
 */
int Dict_insert(Dict* dict, const void* key, void* data);

/**
 * Removes the element with the specified `key` from the dictionary specified by `dict`.
 * Returns a pointer to the data that was stored under the key, or `NULL` if no
 * element with the specified `key` is found. The caller takes ownership of the
 * returned data pointer and is responsible for freeing its associated memory
 * when no longer needed.
 * 
 * @return A pointer to the removed data, or `NULL` if the key was not found.
 */
void* Dict_remove(Dict* dict, const char* key);

/**
 * Looks up the data stored under the specified `key` in the dictionary specified by `dict`.
 * Returns a pointer to the data, or `NULL` if no element with the specified `key` is found.
 * The returned pointer remains valid as long as the element remains in the dictionary,
 * and the caller should not modify or free the data through this pointer.
 * 
 * @return A pointer to the data stored under the key, or `NULL` if the key was not found.
 */
void* Dict_lookup(const Dict* dict, const char* key);

/**
 * provides a snapshot of all keys in the dictionary at the time of the call.
 * Returns an array of pointers to all keys currently stored in the dictionary specified by `dict`.
 * The array is dynamically allocated and null-terminated. The caller takes ownership of the
 * returned array and must call `free` on it when it is no longer needed. The key pointers
 * remain valid until `Dict_destroy` is called or the keys are removed.
 * 
 * @return A null-terminated array of key pointers, or `NULL` if the dictionary is empty or allocation fails.
 */
const char** Dict_keys(const Dict* dict);

/**
 * Returns a pointer to the data stored in the element at the specified `pos_idx`
 * position in the dictionary specified by `d`. The order of elements is
 * implementation-defined and may change as elements are inserted or removed.
 * The returned pointer remains valid as long as the element remains in the dictionary, and the caller should
 * not modify or free the data through this pointer.
 * 
 * @return A pointer to the data at the specified position, or `NULL` if the index is invalid.
 */
void* Dict_getElm(const Dict* d, int pos_idx);

#endif /* _DICTIONARY_H */