/**
 * In general, the set operations presented here are somewhat costly, primarily
 * because many of them search for members of one set in another by traversing
 * each member. However, we can improve the running times of these operations by
 * using a more efficient searching technique, such as hashing. Nevertheless, 
 * the implementation provided here is a general-purpose
 * approach whose performance is adequate for small to medium-sized sets of data.
 */

#ifndef _SET_H
#define _SET_H

#include "SinglyList.h"

typedef sList Set;

/**
 * Initializes the set specified by `set`. This operation must be called
 * for a set before the set can be used with any other operation.
 * The `match` argument is a function used by various set operations to determine if two members
 * match. It should return `1` if `key1` is equal to `key2`, and `0` otherwise.
 * The `destroy` argument provides a way to free dynamically allocated data when `Set_destroy` is
 * called. For example, if the set contains data dynamically allocated using `malloc`,
 * `destroy` should be set to `free` to free the data as the set is destroyed.
 * For structured data containing several dynamically allocated members, `destroy` should be
 * set to a user-defined function that calls `free` for each dynamically allocated member
 * as well as for the structure itself. For a set containing data that should not be
 * freed, destroy should be set to `NULL`.
 * 
 * @return None.
 */
void Set_init(Set* set, int (*match)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the set specified by `set`. No other operations are permitted
 * after calling `Set_destroy` unless `Set_init` is called again.
 * The `Set_destroy` operation removes all members from a set and calls the function
 * passed as `destroy` to `Set_init` once for each member as it is removed,
 * provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void Set_destroy(Set* set);

/**
 * Inserts a member into the set specified by `set`. The new member
 * contains a pointer to `data`, so the memory referenced by `data` should remain
 * valid as long as the member remains in the set. It is the responsibility of the caller
 * to manage the storage associated with data.
 * 
 * @return `0` if inserting the member is successful, `1` if the member is already in the set, or `–1` otherwise.
 */
int Set_insert(Set* set, const void* data);

/**
 * Removes the member containing `data` from the set specified by `set`.
 * Returns a pointer to the data that was stored in the removed member, or
 * `NULL` if no matching member is found in the set. The caller takes ownership
 * of the returned data pointer and is responsible for freeing its associated
 * memory when no longer needed.
 * 
 * @return A pointer to the removed data, or `NULL` if the member was not found.
 */
void* Set_remove(Set* set, const void* data);

/**
 * Builds a set that is the union of `set1` and `set2`. Upon return,
 * `setu` contains the union. Because `setu` points to data in `set1` and `set2`, the data
 * in `set1` and `set2` must remain valid until `setu` is destroyed with `Set_destroy`.
 * 
 * @return `0` if computing the union is successful, or `–1` otherwise.
 */
int Set_union(Set* setu, const Set* set1, const Set* set2);

/**
 * Builds a set that is the intersection of `set1` and set2. Upon
 * return, `seti` contains the intersection. Because `seti` points to data in `set1`, the
 * data in `set1` must remain valid until `seti` is destroyed with `Set_destroy`.
 * 
 * @return `0` if computing the union is successful, or `–1` otherwise.
 */
int Set_intersection(Set* seti, const Set* set1, const Set* set2);

/**
 * Builds a set that is the difference of `set1` and `set2`. Upon return,
 * `setd` contains the difference. Because `setd` points to data in `set1`, the data in
 * `set1` must remain valid until `setd` is destroyed with `Set_destroy`.
 * 
 * @return `0` if computing the union is successful, or `–1` otherwise.
 */
int Set_difference(Set* setd, const Set* set1, const Set* set2);

/**
 * Determines whether the data specified by data matches that of a
 * member in the set specified by `set`.
 * 
 * @return `1` if the member is found, or `0` otherwise.
 */
int Set_is_member(const Set* set, const void* data);

/**
 * Determines whether the set specified by `set1` is a subset of the
 * set specified by `set2`.
 * 
 * @return `1` if the set is a subset, or `0` otherwise.
 */
int Set_is_subset(const Set* set1, const Set* set2);

/**
 * Determines whether the set specified by `set1` is equal to the set specified by `set2`.
 * 
 * @return `1` if the sets are equal, or `0` otherwise.
 */
int Set_is_equal(const Set* set1, const Set* set2);

#endif /* _SET_H */