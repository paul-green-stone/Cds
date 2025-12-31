#ifndef _SINGLY_LINKED_LISTS_H
#define _SINGLY_LINKED_LISTS_H

#include <stdlib.h>

struct node {

    void* data;
    struct node* next;
    void* sentinel;
};

typedef struct node Node;

typedef struct {

    int size;

    Node* head;
    Node* tail;

    void (*destroy)(void* data);
} sList;

/* ================================================================ */

/**
 * Initializes the linked list specified by `list`. This operation must
 * be called for a linked list before the list can be used with any other operation.
 * The `destroy` argument provides a way to free dynamically allocated data when
 * `ist_destroy` is called. For example, if the list contains data dynamically allocated
 * using `malloc`, destroy should be set to `free` to free the data as the linked list is
 * destroyed. For structured data containing several dynamically allocated members,
 * `destroy` should be set to a user-defined function that calls free for each dynamically
 * allocated member as well as for the structure itself. For a linked list containing
 * data that should not be freed, destroy should be set to `NULL`.
 * 
 * @return None.
 */
void sList_init(sList* list, void (*destroy)(void* data));

/**
 * Destroys the linked list specified by `list`. No other operations
 * are permitted after calling `sList_destroy` unless `sList_init` is called again
 * The `sList_destroy` operation removes all elements from a linked list and calls the function
 * passed as `destroy` to `sList_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @return None.
 */
void sList_destroy(sList* list);

/**
 * Inserts an element at the tail of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the list. It is the responsibility
 * of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the element is successful, or `–1` otherwise.
 */
int sList_insert_last(sList* list, const void* data);

/**
 * Inserts an element at the head of the list specified by `list`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the list. It is the responsibility
 * of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the element is successful, or `–1` otherwise.
 */
int sList_insert_first(sList* list, const void* data);

/**
 * Removes the element at the tail of the list specified by `list`.
 * Returns a pointer to the data that was stored in the removed element,
 * or `NULL` if the list is empty. The caller takes ownership of the returned
 * data pointer and is responsible for freeing its associated memory when no longer needed.
 * 
 * @return A pointer to the data, or `NULL` if the list is empty.
 */
void* sList_remove_last(sList* list);

/**
 * Removes the element at the head of the list specified by `list`.
 * Returns a pointer to the data that was stored in the removed element,
 * or `NULL` if the list is empty. The caller takes ownership of the returned
 * data pointer and is responsible for freeing its associated memory when no longer needed.
 * 
 * @return A pointer to the data, or `NULL` if the list is empty.
 */
void* sList_remove_first(sList* list);

/**
 * Searches for the first occurrence of data in the list specified by `list`
 * using the user-defined `match` function. The match function should return non-zero
 * for a successful match between `key1` (node data) and `key2` (search data).
 * 
 * @return A pointer to the node containing the matching data, or `NULL` if no match is found.
 */
Node* sList_find(const sList* list, const void* data, int (*match)(const void* key1, const void* key2));

/**
 * Removes the specified node from the `list`.
 * The node must be a valid node previously obtained from the list (e.g., via `sList_find_node`).
 * The caller takes ownership of the returned data pointer and is responsible
 * for freeing its associated memory when no longer needed.
 * 
 * @return A pointer to the data that was stored in the removed node, or NULL if the node is invalid.
 */
void* sList_remove(sList* list, Node* node);

/**
 * Inserts a new node containing data just after the specified node in the linked list specified by `list`.
 * The new node contains a pointer to `data`, so the memory referenced by `data` should remain valid as
 * long as the node remains in the list. 
 * It is the responsibility of the caller to manage the storage associated with data.
 * 
 * @return `0` if successful, or `-1` if the operation fails.
 */
int sList_insert_after(sList* list, Node* node, const void* data);

/**
 * Inserts a new node containing data just before the specified node in the list specified by `list`.
 * The new node contains a pointer to `data`, so the memory referenced by `data` should remain valid as
 * long as the node remains in the list.
 * It is the responsibility of the caller to manage the storage associated with data.
 * 
 * @return `0` if successful, or `-1` if the operation fails.
 */
int sList_insert_before(sList* list, Node* node, const void* data);

#endif /* _SINGLY_LINKED_LISTS_H */