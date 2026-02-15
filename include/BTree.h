#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

#include <stdlib.h>

/* ================================================================ */
/* ========================= STRUCTURES =========================== */
/* ================================================================ */

/* =========== Define a structure for binary tree nodes =========== */
typedef struct binary_tree_node {

    void* data;

    struct binary_tree_node* left;
    struct binary_tree_node* right;
} BTreeNode;

/* ============= Define a structure for binary trees ============== */
typedef struct binary_tree {

    int (*compare)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    void* _info;
} BTree;

/* ================================================================ */
/* =========================== MACROS ============================= */
/* ================================================================ */

/**
 * Determines whether the specified `node` marks the end of a branch
 * in a binary tree.
 * 
 * @param node Pointer to a binary tree node.
 * 
 * @return `1` if `node` is `NULL`, `0` otherwise.
 */
#define BTN_eob(node) ((node) == NULL)

/**
 * Determines whether the specified `node` is a leaf node
 * in a binary tree.
 * 
 * @param node Pointer to a binary tree node.
 * 
 * @return `1` if `node` is a leaf, `0` otherwise.
 */
#define BTN_is_leaf(node) ((node)->left == NULL && (node)->right == NULL)

/**
 * Evaluates to the data pointer stored in the specified `node`.
 * 
 * @param node Pointer to a binary tree node.
 * 
 * @return Pointer to the data stored in `node`.
 */
#define BTN_data(node) (node)->data

/**
 * Evaluates to the left child node of the specified `node`.
 * 
 * @param node Pointer to a binary tree node.
 * 
 * @return Pointer to the left child node, or `NULL` if none.
 */
#define BTNl(node) (node)->left

/**
 * Evaluates to the right child node of the specified `node`.
 * 
 * @param node Pointer to a binary tree node.
 * 
 * @return Pointer to the right child node, or `NULL` if none.
 */
#define BTNr(node) (node)->right

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

/**
 * Initializes the binary tree specified by `tree`. This operation must
 * be called for a binary tree before the tree can be used with
 * any other operation.
 * 
 * The destroy argument provides a way to free dynamically allocated
 * data when `BTree_destroy` is called. For example, if the tree
 * contains data dynamically allocated using `malloc`, `destroy`
 * should be set to `free` to free the data as the binary tree
 * is destroyed.For structured data containing several dynamically
 * allocated members, `destroy` should be set to a user-defined
 * function that calls `free` for each dynamically allocated member
 * as well as for the structure itself. For a binary tree
 * containing data that should not be freed, `destroy`
 * should be set to `NULL`.
 * 
 * @param tree      Pointer to the binary tree to initialize.
 * @param destroy   Optional destructor called on each element during destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_init(BTree* tree, void (*destroy)(void* data));

/**
 * Destroys the binary tree specified by `tree`. No other operations
 * are permitted after calling `BTree_destroy` unless `BTree_init`
 * is called again. The `BTree_destroy` operation removes all nodes
 * from a binary tree and calls the function passed as `destroy`
 * to `BTree_init` once for each node as it is removed,
 * provided `destroy` was not set to `NULL`.
 * 
 * @param tree Pointer to the binary tree to destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_destroy(BTree* tree);

/**
 * Inserts a node as the left child of `node` in the binary tree
 * specified by `tree`. If `node` already has a left child,
 * `BTree_insert_l` returns `–1`. If `node` is `NULL`, the
 * new node is inserted as the root node. The tree must be empty to
 * insert a node as the root node; otherwise, `BTree_insert_l`
 * returns `–1`.
 * 
 * When successful, the new node contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the node remains in the binary
 * tree.
 * 
 * It is the responsibility of the caller to manage the storage associated with `data`.
 * 
 * @param tree Pointer to the binary tree to initialize.
 * @param node Parent node for the new left child.
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_insert_l(BTree* tree, BTreeNode* node, const void* data);

/**
 * Inserts a node as the right child of `node` in the binary tree
 * specified by `tree`. If `node` already has a right child,
 * `BTree_insert_l` returns `–1`. If `node` is `NULL`, the
 * new node is inserted as the root node. The tree must be empty to
 * insert a node as the root node; otherwise, `BTree_insert_l`
 * returns `–1`.
 * 
 * When successful, the new node contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the node remains in the binary
 * tree.
 * 
 * It is the responsibility of the caller to manage the storage associated with `data`.
 * 
 * @param tree Pointer to the binary tree to initialize.
 * @param node Parent node for the new right child.
 * @param data Pointer to user-managed data.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_insert_r(BTree* tree, BTreeNode* node, const void* data);

/**
 * Removes the subtree rooted at the left child of `node` from the
 * binary tree specified by `tree`. If `node` is `NULL`, all nodes
 * in the tree are removed.
 * 
 * The function passed as `destroy` to `BTree_init` is called once
 * for each node as it is removed, provided `destroy` was not set to `NULL`.
 * 
 * @param tree The binary tree from which to remove nodes.
 * @param node The node whose left subtree should be removed.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_remove_l(BTree* tree, BTreeNode* node);

/**
 * Removes the subtree rooted at the right child of `node` from the
 * binary tree specified by `tree`. If `node` is `NULL`, all nodes
 * in the tree are removed.
 * 
 * The function passed as `destroy` to `BTree_init` is called once
 * for each node as it is removed, provided `destroy` was not set to `NULL`.
 * 
 * @param tree The binary tree from which to remove nodes.
 * @param node The node whose right subtree should be removed.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_remove_r(BTree* tree, BTreeNode* node);

/**
 * Merges the two binary trees specified by `left` and `right` into the
 * single binary tree `merge`. After merging is complete, merge contains
 * `data` in its root node, and `left` and `right` are the left and right
 * subtrees of its root. 
 * 
 * Once the trees have been merged, `left` and `right` are as if
 * `BTree_destroy` had been called on them.
 * 
 * @param merge The target empty tree to receive the merged structure.
 * @param left  The source tree for the left subtree; destroyed after use.
 * @param right The source tree for the right subtree; destroyed after use.
 * @param data The data value stored in the new root node of `merge`.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_merge(BTree* merge, BTree* left, BTree* right, const void* data);

/**
 * Prints all elements in the binary tree specified by `tree`
 * using level order traversal (breadth-first). Each level of
 * the tree is printed on its own line.
 * 
 * The user-defined `print` function is called for each node's data.
 * The traversal visits nodes from left to right on each level.
 * 
 * @param tree
 * @param print
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int BTree_print(const BTree* tree, void (*print)(const void* data));

/**
 * Searches for a node in the binary tree specified by `tree`
 * containing data that matches the search criteria.
 * 
 * Uses the user-defined `match` function to compare node data
 * against the search `data`.
 * 
 * @param tree
 * @param match
 * @param data
 * 
 * @return A pointer to the matching node, or `NULL` if no match is found.
 */
int BTree_lookup(const BTree* tree, int (*match)(const void* key1, const void* key2), const void* data, BTreeNode** node);

/**
 * Returns the number of nodes in the binary tree specified by `tree`
 * 
 * @param tree Pointer to the binary tree structure.
 * 
 * @return Number of nodes; a negative code otherwise.
 */
ssize_t BTree_size(const BTree* tree);

/**
 * Returns a pointer to the root node of the binary tree
 * specified by `tree`.
 * 
 * @param tree  Pointer to the binary tree structure.
 * @param error Pointer to an integer for error status.
 * 
 * @return Number of nodes; a negative code otherwise.
 */
BTreeNode* BTree_root(const BTree* tree, int* error);

#endif /* _BINARY_TREE_H */
