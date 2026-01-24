#ifndef SINGLY_LINKED_LISTS_H
#define SINGLY_LINKED_LISTS_H

#include "CdsErrors.h"
#include <stdlib.h>
#include <sys/types.h>

typedef struct s_node sNode;

typedef struct {

    void (*destroy)(void* data);
    int (*match)(const void* key1, const void* key2);

    void* _info;
} sList;

/* ================================================================ */
/* ============================= LIST ============================= */
/* ================================================================ */

/**
 * Initializes the linked list specified by `list`. This operation must
 * be called for a linked list before the list can be used with any
 * other operation.
 * 
 * The `destroy` argument provides a way to free dynamically allocated
 * data when `sList_destroy` is called. For example, if the list
 * contains data dynamically allocated using `malloc`, destroy should
 * be set to `free` to free the data as the linked list is destroyed.
 * For structured data containing several dynamically allocated members,
 * `destroy` should be set to a user-defined function that calls free
 * for each dynamically allocated member as well as for the structure
 * itself. For a linked list containing data that should not be freed,
 * `destroy` should be set to `NULL`.
 * 
 * The `match` argument specifies a user-defined comparison function
 * used to identify matching elements during search operations.
 * The function must return `1` if `key1` (element data) is equal to
 * `key2` (search key), and `0` otherwise.
 * 
 * @param list     Pointer to the list to initialize.
 * @param destroy  Optional destructor called on each element during destroy.
 * @param match    Optional comparison function used by search operations.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_init(sList* list, void (*destroy)(void* data), int (*match)(const void* key1, const void* key2));

/**
 * Destroys the linked list specified by `list`. No other operations
 * are permitted after calling `sList_destroy` unless `sList_init`
 * is called again. The `sList_destroy` operation removes all
 * elements from a linked list and calls the function passed as
 * `destroy` to `sList_init` once for each element as it is removed,
 * provided destroy was not set to `NULL`.
 * 
 * @param list Pointer to the list to destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_destroy(sList* list);

/**
 * Inserts an element at the tail of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element
 * remains in the list.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param list Pointer to the list.
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_insert_last(sList* list, void* data);

/**
 * Inserts an element at the head of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element
 * remains in the list.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param list Pointer to the list.
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_insert_first(sList* list, void* data);

/**
 * Removes the element at the tail of the list specified by `list`.
 * Upon return, `data` points to the data stored in the element that
 * was removed.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param list Pointer to the list.
 * @param data  Pointer to a location where a pointer to the
 *              removed element's data will be stored.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_remove_last(sList* list, void** data);

/**
 * Removes the element at the head of the list specified by `list`.
 * Upon return, `data` points to the data stored in the element that
 * was removed.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param list  Pointer to the list.
 * @param data  Pointer to a location where a pointer to the
 *              removed element's data will be stored.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_remove_first(sList* list, void** data);

/**
 * Searches for the first occurrence of data in the list specified
 * by `list` using the user-defined `match` function. 
 * Upon return, `node` points to the node just found. The function
 * must return `1` if `key1` (element data) is equal to `key2`
 * (search key), and `0` otherwise.
 * 
 * If `match` is non-`NULL`, it is used for this search operation
 * only. Otherwise, the `match` function configured during
 * `sList_init` is used. If both are `NULL`, then fail.
 * 
 * @param list  Pointer to the list.
 * @param data  Search key.
 * @param node  Pointer to a location where a pointer to the found
 *              node will be stored upon success
 * @param match Optional match override.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_find(const sList* list, const void* data, sNode** node, int (*match)(const void* key1, const void* key2));

/**
 * Removes the specified node from the `list`.
 * The node must be a valid node previously obtained from the list
 * (e.g., via `sList_find`).
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param list  Pointer to the list.
 * @param node  Node to remove.
 * @param data  Pointer to a location where a pointer to the removed
 *              node's data will be stored upon success.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_remove(sList* list, sNode* node, void** data);

/**
 * Inserts a new node containing data just after the specified
 * node in the linked list specified by `list`. The new node
 * contains a pointer to `data`, so the memory referenced by
 * `data` should remain valid as long as the node remains in
 * the list.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with data.
 * 
 * @param list Pointer to the list.
 * @param node Existing node (must belong to list).
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_insert_after(sList* list, sNode* node, void* data);

/**
 * Inserts a new node containing data just before the specified
 * node in the list specified by `list`. The new node contains
 * a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the node remains in the list.
 * 
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with data.
 * 
 * @param list Pointer to the list.
 * @param node Existing node (must belong to list).
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int sList_insert_before(sList* list, sNode* node, void* data);

/**
 * Retrieves a human-readable error message corresponding to the
 * last error encountered by the singly linked list. The error
 * message is based on the last operation performedon the list
 * and its result.
 * 
 * @param list Pointer to the list.
 * 
 * @return A null-terminated string containing the description of
 * the last error encountered.
 */
const char* sList_error(const sList* list);

/**
 * Returns a pointer to the head node of the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Pointer to the head node, or `NULL` if the list is empty or missing.
 */
sNode* sList_head(const sList* list);

/**
 * Returns a pointer to the tail node of the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Pointer to the tail node, or `NULL` if the list is empty or missing.
 */
sNode* sList_tail(const sList* tail);

/**
 * Returns the number of elements in the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Number of elements on success, negative value if `list` is `NULL`.
 */
ssize_t sList_size(const sList* list);

/* ================================================================ */
/* ============================= NODE ============================= */
/* ================================================================ */

/** 
 * Returns the data pointer stored in the specified node.
 * The returned pointer is borrowed; ownership remains with the caller
 * or the list, depending on how the data was originally managed.
 * 
 * @param node Pointer to a node.
 *
 * @return A pointer to the node's data.
 */
void* sNode_data(const sNode* node);

/**
 * Returns the next node in the linked list.
 *
 * @note    The returned node, if any, is owned by the list.
 *          The caller must not attempt to free or modify the
 *          node structure itself.
 *
 * @param node A pointer to a valid node obtained from the list.
 *
 * @return A pointer to the next node in the list, or `NULL`
 * if there is no next node or `node` is missing.
 */
sNode* sNode_next(const sNode* node);

#endif /* SINGLY_LINKED_LISTS_H */