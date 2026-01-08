#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

#include <stdlib.h>

/* ================================================================ */
/* =========== Define a structure for binary tree nodes =========== */
/* ================================================================ */

typedef struct binary_tree_node {

    void* data;

    struct binary_tree_node* left;
    struct binary_tree_node* right;
} BTreeNode;

/* ================================================================ */
/* ============= Define a structure for binary trees ============== */
/* ================================================================ */

typedef struct binary_tree {

    int (*compare)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    struct binary_tree_node* root;

    int size;
} BTree;

/**
 * Macro that evaluates to the number of nodes in the binary tree specified by `tree`.
 * 
 * @return Number of nodes in the tree.
 */
#define BTree_size(tree) (tree)->size

/**
 * Macro that evaluates to the node at the root of the binary tree specified by `tree`.
 * 
 * @return Node at the root of a tree.
 */
#define BTree_root(tree) (tree)->root

/**
 * Macro that determines whether the `node` specified as node marks the end of a branch in a binary tree.
 * 
 * @return `1` if the node marks the end of a branch, or `0` otherwise.
 */
#define BTN_eob(node) (node) == NULL

/**
 * Macro that determines whether the node specified as `node` is a leaf node in a binary tree.
 * 
 * @return `1` if the node is a leaf node, or `0` otherwise.
 */
#define BTN_is_leaf(node) ((node)->left == NULL && (node)->right == NULL)

/**
 * Macro that evaluates to the data stored in the node of a binary tree specified by `node`.
 * 
 * @return Data stored in the node.
 */
#define BTN_data(node) (node)->data

/**
 * Macro that evaluates to the node of a binary tree that is the left child of the node
 * specified by `node`.
 * 
 * @return Left child of the specified node.
 */
#define BTNl(node) (node)->left

/**
 * Macro that evaluates to the node of a binary tree that is the right
 * child of the node specified by `node`.
 * 
 * @return Right child of the specified node.
 */
#define BTNr(node) (node)->right

/**
 * Initializes the binary tree specified by `tree`. This operation must
 * be called for a binary tree before the tree can be used with any other operation.
 * he destroy argument provides a way to free dynamically allocated data when
 * `BTree_destroy` is called. For example, if the tree contains data dynamically
 * allocated using `malloc`, destroy should be set to `free` to free the data as the binary
 * tree is destroyed. For structured data containing several dynamically allocated
 * members, `destroy` should be set to a user-defined function that calls `free` for each
 * dynamically allocated member as well as for the structure itself. For a binary tree
 * containing data that should not be freed, `destroy` should be set to `NULL`.
 * 
 * @return None.
 */
int BTree_init(BTree* tree, void (*destroy)(void* data));

/**
 * Destroys the binary tree specified by `tree`. No other operations
 * are permitted after calling `BTree_destroy` unless `BTree_init` is called again.
 * The `BTree_destroy` operation removes all nodes from a binary tree and calls the
 * function passed as `destroy` to `BTree_init` once for each node as it is removed,
 * provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void BTree_destroy(BTree* tree);

/**
 * Inserts a node as the left child of `node` in the binary tree specified
 * by `tree`. If `node` already has a left child, `BTree_insert_l` returns `–1`.
 * If `node` is `NULL`, the new node is inserted as the root node. The tree must
 * be empty to insert a node as the root node; otherwise, `BTree_insert_l` returns `–1`.
 * When successful, the new node contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the node remains in the binary
 * tree. It is the responsibility of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the node is successful, or `–1` otherwise.
 */
int BTree_insert_l(BTree* tree, BTreeNode* node, const void* data);

/**
 * Inserts a node as the right child of `node` in the binary tree specified
 * by `tree`. If `node` already has a right child, `BTree_insert_r` returns `–1`.
 * If `node` is `NULL`, the new node is inserted as the root node. The tree must
 * be empty to insert a node as the root node; otherwise, `BTree_insert_r` returns `–1`.
 * When successful, the new node contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the node remains in the binary
 * tree. It is the responsibility of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the node is successful, or `–1` otherwise.
 */
int BTree_insert_r(BTree* tree, BTreeNode* node, const void* data);

/**
 * Removes the subtree rooted at the left child of `node` from the
 * binary tree specified by `tree`. If `node` is `NULL`, all nodes
 * in the tree are removed. The function passed as `destroy` to `BTree_init`
 * is called once for each node as it is removed, provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void BTree_remove_l(BTree* tree, BTreeNode* node);

/**
 * Removes the subtree rooted at the right child of `node` from the
 * binary tree specified by `tree`. If `node` is `NULL`, all nodes
 * in the tree are removed. The function passed as `destroy` to `BTree_init`
 * is called once for each node as it is removed, provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void BTree_remove_r(BTree* tree, BTreeNode* node);

/**
 * Merges the two binary trees specified by `left` and `right` into the
 * single binary tree `merge`. After merging is complete, merge contains `data` in its
 * root node, and `left` and `right` are the left and right subtrees of
 * its root. Once the trees have been merged, `left` and `right` are as
 * if `BTree_destroy` had been called on them.
 * 
 * @return `0` if merging the trees is successful, or `–1` otherwise.
 */
int BTree_merge(BTree* merge, BTree* left, BTree* right, const void* data);

/**
 * Prints all elements in the binary tree specified by `tree` using level order
 * traversal (breadth-first). Each level of the tree is printed on its own line.
 * The user-defined `print` function is called for each node's data.
 * The traversal visits nodes from left to right on each level.
 * 
 * @return None.
 */

void BTree_print(const BTree* tree, void (*print)(const void* data));

/**
 * Searches for a node in the binary tree specified by `tree` containing data that matches
 * the search criteria. Uses the user-defined `match` function to compare node data against
 * the search `data`. Returns the first matching node found using a level-order traversal.
 * Returns `NULL` if no matching node is found.
 * 
 * @return A pointer to the matching node, or `NULL` if no match is found.
 */
BTreeNode* BTree_lookup(const BTree* tree, int (*match)(const void* key1, const void* key2), const void* data);

#endif /* _BINARY_TREE_H */