#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include "BTree.h"

typedef BTree BST;

#define AVL_LEFT_HEAVY  -1
#define AVL_BALANCED     0
#define AVL_RIGHT_HEAVY  1


/**
 * Define a structure for nodes in AVL trees. 
 */
typedef struct avl_node {
    
    void* data;

    int is_hidden;
    int factor;
} AVLNode;

/**
 * Initializes the binary search tree specified by `tree`. This operation
 * must be called for a binary search tree before the tree can be used with any
 * other operation. The function pointer `compare` specifies a user-defined function
 * to compare elements. This function should return `1` if `key1` > `key2`, `0` if `key1`
 * = `key2`, and `–1` if `key1` < `key2`. The `destroy` argument provides a way to free
 * dynamically allocated data when `BST_destroy` is called. For a binary search tree#
 * containing data that should not be freed, destroy should be set to `NULL`.
 * 
 * @return None.
 */
void BST_init(BST* tree, int (*compare)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the binary search tree specified by  `tree`. No other operations
 * are permitted after calling `BST_destroy` unless `BST_init` is called again.
 * The `BST_destroy` operation removes all nodes from a binary search tree and
 * calls the function passed as `destroy` to `BST_init` once for each node as it is
 * removed, provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void BST_destroy(BST* tree);

/**
 * Inserts a node into the binary search tree specified by `tree`. The
 * new node contains a pointer to `data`, so the memory referenced by `data` should
 * remain valid as long as the node remains in the binary search tree. It is the
 * responsibility of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the node is successful, `1` if the node is already in the
 * tree, or `–1` otherwise.
 */
int BST_insert(BST* tree, const void* data);

/**
 * Removes the node matching data from the binary search tree specified by `tree`.
 * In actuality, this operation only performs a lazy removal, in
 * which the node is simply marked as hidden. Thus, no pointer is returned to the
 * data matching `data`. The data in the tree must remain valid even after it has been
 * removed.
 * 
 * @return `0` if removing the node is successful, or `–1` otherwise.
 */
int BST_remove(BST* tree, const void* data);

/**
 * Determines whether a node matches data in the binary search tree specified as `tree`.
 * 
 * @return A pointer to the data stored in the matching node, or `NULL` otherwise.
 */
void* BST_lookup(BST* tree, const void* data);

#endif /* _BINARY_SEARCH_TREE_H */