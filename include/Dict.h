#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include "HType/Open.h"
#include "OAHT.h"
#include "CdsErrors.h"

typedef HT Dict;

/**
 * Creates an empty dictionary with the specified initial `size`.
 * 
 * This operation must be called for a dictionary before the dictionary
 * can be used with any other operation. Returns a pointer to the newly
 * allocated dictionary, or `NULL` if the allocation fails. The caller
 * takes ownership of the returned dictionary and must call `Dict_destroy`
 * on it followed by `free` to release both the dictionary structure
 * and its contents.
 * 
 * @param size Initial dictionary size. 
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
 * 
 * @param dict Pointer to the dictionary pointer to destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, or an error on failure.
 */
int Dict_destroy(Dict** dict);

/**
 * Inserts `data` into the dictionary specified by `dict` under
 * the specified `key`. The memory referenced by `key` and
 * `data` should remain valid as long as the element remains
 * in the dictionary.
 * 
 * It is the responsibility of the caller to
 * manage the storage associated with `key` and `data`.
 * 
 * @param dict Dictionary where the element will be inserted.
 * @param key  Key under which to store the data.
 * @param data Value to associate with the key.
 * 
 * @return `0` if inserting the element is successful, `1` if an element
 * with the specified `key` already exists, or `-1` otherwise.
 */
int Dict_insert(Dict* dict, const void* key, void* data);

/**
 * Removes the element with the specified `key` from the dictionary
 * specified by `dict`. Returns a pointer to the data that was
 * stored under the key, or `NULL` if no element with the specified
 * `key` is found.
 * 
 * It is the responsibility of the caller to
 * manage the storage associated with `key` and `data`.
 * 
 * @param dict Dictionary from which to remove the element.
 * @param key  Key of the element to remove.
 * @param data Output parameter: receives pointer to removed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, `CONTAINER_ERROR_NOT_FOUND`
 * if key not found, or other error codes for invalid parameters.
 */
int Dict_remove(Dict* dict, const char* key, void** data);

/**
 * Looks up the data stored under the specified `key` in the dictionary specified by `dict`.
 * Returns a pointer to the data, or `NULL` if no element with the specified `key` is found.
 * The returned pointer remains valid as long as the element remains in the dictionary,
 * and the caller should not modify or free the data through this pointer.
 * 
 * @param dict   Dictionary in which to perform the lookup.
 * @param key    Key to search for.
 * @param result Receives pointer to found data,
 * or `NULL` if key not found
 * 
 *  @return `CONTAINER_SUCCESS` if key found,
 * `CONTAINER_ERROR_NOT_FOUND` if key not found,
 * or other error codes for invalid parameters.
 */
int Dict_lookup(const Dict* dict, const char* key, void** result);

/**
 * Returns the number of elements currently stored in the dictionary.
 * 
 * @param dict Dictionary to query for current element count
 * .
 * @return Number of elements on success, error code otherwise.
 */
ssize_t Dict_size(const Dict* dict);

#endif /* _DICTIONARY_H */