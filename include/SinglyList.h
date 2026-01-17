#ifndef SINGLY_LINKED_LISTS_H
#define SINGLY_LINKED_LISTS_H

/**
 * @note Many operations perform internal consistency checks.
 * If a corruption is detected, the operation fails and
 * `last_error` is set to `SLIST_ERR_CORRUPTED`.
 */

#include <stdlib.h>

typedef enum {
    SLIST_OK = 0,

    SLIST_ERR_NULL_LIST,
    SLIST_ERR_NULL_NODE,
    SLIST_ERR_NULL_DATA,

    SLIST_ERR_FOREIGN_NODE,

    SLIST_ERR_NO_MATCH_FUNCTION,
    SLIST_ERR_NO_MEMORY,

    SLIST_ERR_CORRUPTED,
    SLIST_ERR_EMPTY,
    SLIST_ERR_MISSING
} sListResult;

typedef struct s_node sNode;

typedef struct {

    void (*destroy)(void* data);
    int (*match)(const void* key1, const void* key2);

    sNode* head;
    sNode* tail;

    size_t size;
    sListResult last_error;
} sList;

/* ================================================================ */
/* ============================= LIST ============================= */
/* ================================================================ */

/**
 * Initializes the linked list specified by `list`. This operation must
 * be called for a linked list before the list can be used with any other operation.
 * 
 * The `destroy` argument provides a way to free dynamically allocated data when
 * `sList_destroy` is called. For example, if the list contains data dynamically allocated
 * using `malloc`, destroy should be set to `free` to free the data as the linked list is
 * destroyed. For structured data containing several dynamically allocated members,
 * `destroy` should be set to a user-defined function that calls free for each dynamically
 * allocated member as well as for the structure itself. For a linked list containing
 * data that should not be freed, destroy should be set to `NULL`.
 * 
 * The `match` argument specifies a user-defined comparison function used to identify
 * matching elements during search operations (e.g., `sList_find`). The function must
 * return a non-zero value if `key1` (element data) matches `key2` (search key), and zero
 * otherwise. If `match` is set to `NULL`, search operations that rely on element matching
 * will fail.
 * 
 * @param list     Pointer to the list to initialize (must not be `NULL`).
 * @param destroy  Optional destructor called on each element during destroy.
 * @param match    Optional comparison function used by search operations.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_init(sList* list, void (*destroy)(void* data), int (*match)(const void* key1, const void* key2));

/**
 * Destroys the linked list specified by `list`. No other operations
 * are permitted after calling `sList_destroy` unless `sList_init` is called again
 * The `sList_destroy` operation removes all elements from a linked list and calls the function
 * passed as `destroy` to `sList_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @param list Pointer to the list (must not be `NULL`).
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_destroy(sList* list);

/**
 * Inserts an element at the tail of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the list. It is the responsibility
 * of the caller to manage the storage associated with `data`.
 * 
 * @param list Pointer to the list.
 * @param data Pointer to user-managed data.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @note `data` must not be NULL.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_insert_last(sList* list, void* data);

/**
 * Inserts an element at the head of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the list. It is the responsibility
 * of the caller to manage the storage associated with `data`.
 * 
 * @param list Pointer to the list.
 * @param data Pointer to user-managed data.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @note `data` must not be NULL.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_insert_first(sList* list, void* data);

/**
 * Removes the element at the tail of the list specified by `list`.
 * Returns a pointer to the data that was stored in the removed element,
 * or `NULL` if the list is empty. The caller takes ownership of the returned
 * data pointer and is responsible for freeing its associated memory when no longer needed.
 * 
 * @param list Pointer to the list.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return Pointer to removed data, or `NULL` on failure.
 */
void* sList_remove_last(sList* list);

/**
 * Removes the element at the head of the list specified by `list`.
 * Returns a pointer to the data that was stored in the removed element,
 * or `NULL` if the list is empty. The caller takes ownership of the returned
 * data pointer and is responsible for freeing its associated memory when no longer needed.
 * 
 * @param list Pointer to the list.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return Pointer to removed data, or `NULL` on failure.
 */
void* sList_remove_first(sList* list);

/**
 * Searches for the first occurrence of data in the list specified by `list`
 * using the user-defined `match` function. The match function should return non-zero
 * for a successful match between `key1` (node data) and `key2` (search data).
 * 
 * If `match` is non-`NULL`, it is used for this search operation only.
 * Otherwise, the `match` function configured during `sList_init` is used.
 * If both are `NULL`, then fail.
 * 
 * @param list  Pointer to the list.
 * @param data  Search key.
 * @param match Optional match override.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @note `data` must not be NULL.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return Pointer to the matching node, or `NULL` otherwise.
 */
const sNode* sList_find(const sList* list, const void* data, int (*match)(const void* key1, const void* key2));

/**
 * Removes the specified node from the `list`.
 * The node must be a valid node previously obtained from the list (e.g., via `sList_find`).
 * The caller takes ownership of the returned data pointer and is responsible
 * for freeing its associated memory when no longer needed.
 * 
 * @param list Pointer to the list.
 * @param node Node to remove.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return Pointer to removed data, or `NULL` on failure.
 */
void* sList_remove(sList* list, sNode* node);

/**
 * Inserts a new node containing data just after the specified node in the linked list specified by `list`.
 * The new node contains a pointer to `data`, so the memory referenced by `data` should remain valid as
 * long as the node remains in the list. 
 * It is the responsibility of the caller to manage the storage associated with data.
 * 
 * @param list Pointer to the list.
 * @param node Existing node (must belong to list).
 * @param data Pointer to user-managed data.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @note `data` must not be NULL.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_insert_after(sList* list, sNode* node, void* data);

/**
 * Inserts a new node containing data just before the specified node in the list specified by `list`.
 * The new node contains a pointer to `data`, so the memory referenced by `data` should remain valid as
 * long as the node remains in the list.
 * It is the responsibility of the caller to manage the storage associated with data.
 * 
 * @param list Pointer to the list.
 * @param node Existing node (must belong to list).
 * @param data Pointer to user-managed data.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @note `data` must not be NULL.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
sListResult sList_insert_before(sList* list, sNode* node, void* data);

/**
 * Retrieves a human-readable error message corresponding to the last error encountered
 * by the singly linked list. The error message is based on the last operation performed
 * on the list and its result.
 * 
 * @param list A pointer to the singly linked list from which the last error message will be retrieved.
 * If the list is `NULL`, the function will return `NULL`.
 * 
 * @return A pointer to a string containing the description of the last error encountered.
 */
const char* sList_get_last_error(const sList* list);

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
 * This function provides read-only access to the link between nodes and
 * can be used to iterate through the list starting from the head.
 *
 * If `node` is `NULL` or if the specified node is the last element in the list,
 * the function returns `NULL`.
 *
 * @note The returned node, if any, is owned by the list. The caller must not
 *       attempt to free or modify the node structure itself.
 *
 * @param node A pointer to a valid node obtained from the list.
 *
 * @return A pointer to the next node in the list, or `NULL` if there is no next node.
 */
const sNode* sNode_next(const sNode* node);

#endif /* SINGLY_LINKED_LISTS_H */