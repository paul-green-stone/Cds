#include <string.h>
#include <stdio.h>

#include "../include/BST.h"

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

/**
 * 
 */
static void destroy_right(BST* tree, BTreeNode* node);

/**
 * 
 */
static void rotate_left(BTreeNode** node) {

    BTreeNode* left, *grandchild;
    /* ======== */

    left = BTNl(*node);

    if (((AVLNode*) BTN_data(left))->factor == AVL_LEFT_HEAVY) {

        /* Perform an `LL` rotation */
        BTNl(*node) = BTNr(left);
        BTNr(left) = *node;
        ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
        ((AVLNode*) BTN_data(left))->factor = AVL_BALANCED;
        *node = left;
    }
    else {
        
        /* Perform an `LR` rotation */
        grandchild = BTNr(left);
        BTNr(left) = BTNl(grandchild);
        BTNl(grandchild) = left;
        BTNl(*node) = BTNr(grandchild);
        BTNr(grandchild) = *node;

        switch (((AVLNode*) BTN_data(grandchild))->factor) {

            case AVL_LEFT_HEAVY: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_RIGHT_HEAVY;
                ((AVLNode*) BTN_data(left))->factor = AVL_BALANCED;

                break ;
            }

            case AVL_BALANCED: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                ((AVLNode*) BTN_data(left))->factor = AVL_BALANCED;

                break ;
            }

            case AVL_RIGHT_HEAVY: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                ((AVLNode*) BTN_data(left))->factor = AVL_LEFT_HEAVY;

                break ;
            }
        }

        ((AVLNode*) BTN_data(grandchild))->factor = AVL_BALANCED;
        *node = grandchild;
    }
}

/**
 * 
 */
static void rotate_right(BTreeNode** node) {

    BTreeNode* right, *grandchild;
    /* ======== */

    right =  BTNr(*node);

    if (((AVLNode*) BTN_data(right))->factor == AVL_RIGHT_HEAVY) {

        /* Perform an RR rotation */
        BTNr(*node) = BTNl(right);
        BTNl(right) = *node;
        ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
        ((AVLNode*) BTN_data(right))->factor = AVL_BALANCED;
        *node = right;
    }
    else {

        /* Perform an RL rotation */
        grandchild = BTNl(right);
        BTNl(right) = BTNr(grandchild);
        BTNr(grandchild) = right;
        BTNr(*node) = BTNl(grandchild);
        BTNl(grandchild) = *node;

        switch (((AVLNode*) BTN_data(grandchild))->factor) {

            case AVL_LEFT_HEAVY: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                ((AVLNode*) BTN_data(right))->factor = AVL_RIGHT_HEAVY;

                break ;
            }

            case AVL_BALANCED: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                ((AVLNode*) BTN_data(right))->factor = AVL_BALANCED;

                break ;
            }

            case AVL_RIGHT_HEAVY: {

                ((AVLNode*) BTN_data(*node))->factor = AVL_LEFT_HEAVY;
                ((AVLNode*) BTN_data(right))->factor = AVL_BALANCED;

                break ;
            }
        }

        ((AVLNode*) BTN_data(grandchild))->factor = AVL_BALANCED;
        *node = grandchild;
    }
}

/**
 *
 */
static void destroy_left(BTree* tree, BTreeNode* node) {

    BTreeNode** position;
    /* ======== */

    if (tree->size == 0) { return ; }

    /* Determine where to destroy nodes */
    if (node == NULL) { position = &tree->root; }
    else { position = &node->left; }

    if (*position != NULL) {

        destroy_left(tree, *position);
        destroy_right(tree, *position);

        if (tree->destroy != NULL) { tree->destroy(((AVLNode*) (*position)->data)->data); }

        free((*position)->data);
        free(*position);
        *position = NULL;

        tree->size--;
    }
}

/**
 * 
 */
static void destroy_right(BTree* tree, BTreeNode* node) {

    BTreeNode** position;
    /* ======== */

    if (tree->size == 0) { return ; }

    /* Determine where to destroy nodes */
    if (node == NULL) { position = &tree->root; }
    else { position = &node->right; }

    if (*position != NULL) {

        destroy_left(tree, *position);
        destroy_right(tree, *position);

        if (tree->destroy != NULL) { tree->destroy(((AVLNode*) (*position)->data)->data); }

        free((*position)->data);
        free(*position);
        *position = NULL;

        tree->size--;
    }
}

/**
 * 
 */
static int insert(BST* tree, BTreeNode** node, const void* data, int* balanced) {

    AVLNode* avl_data;
    int cmpval, retval;
    /* ======== */

    if (BTN_eob(*node)) {

        if ((avl_data = calloc(1, sizeof(AVLNode))) == NULL) { return -1; }

        avl_data->factor = AVL_BALANCED;
        avl_data->is_hidden = 0;
        avl_data->data = (void*) data;

        /* ======== */
        return BTree_insert_l(tree, *node, avl_data);
    }
    /* Handle insertion into a tree that is not empty */
    else {

        cmpval = tree->compare(data, ((AVLNode*) BTN_data(*node))->data);

        if (cmpval < 0) {

            /* Move to the left */
            if (BTN_eob(BTNl(*node))) {
                
                if ((avl_data = calloc(1, sizeof(AVLNode))) == NULL) { return -1; }

                avl_data->factor = AVL_BALANCED;
                avl_data->is_hidden = 0;
                avl_data->data = (void*) data;

                if (BTree_insert_l(tree, *node, avl_data) != 0) { return -1; }

                *balanced = 0;
            }
            else {

                if ((retval = insert(tree, &BTNl(*node), data, balanced)) != 0) { return retval; }
            }

            /* Ensure that the tree remains balanced */
            if (!(*balanced)) {

                switch (((AVLNode*) BTN_data(*node))->factor) {
                    
                    case AVL_LEFT_HEAVY: {

                        rotate_left(node);
                        *balanced = 1;
                        
                        break ;
                    }

                    case AVL_BALANCED: {

                        ((AVLNode*) BTN_data(*node))->factor = AVL_LEFT_HEAVY;

                        break ;
                    }

                    case AVL_RIGHT_HEAVY: {

                        ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                        *balanced = 1;

                        break ;
                    }
                }
            }
        }
        else if (cmpval > 0) {

            /* Move to the right */
            if (BTN_eob(BTNr(*node))) {

                if ((avl_data = calloc(1, sizeof(AVLNode))) == NULL) { return -1; }

                avl_data->factor = AVL_BALANCED;
                avl_data->is_hidden = 0;
                avl_data->data = (void*) data;

                if (BTree_insert_r(tree, *node, avl_data) != 0) { return -1; }

                *balanced = 0;
            }
            else {

                if ((retval = insert(tree, &BTNr(*node), data, balanced)) != 0) { return retval; }
            }

            if (!(*balanced)) {

                switch (((AVLNode*) BTN_data(*node))->factor) {

                    case AVL_LEFT_HEAVY: {

                        ((AVLNode*) BTN_data(*node))->factor = AVL_BALANCED;
                        *balanced = 1;

                        break ;
                    }

                    case AVL_BALANCED: {

                        ((AVLNode*) BTN_data(*node))->factor = AVL_RIGHT_HEAVY;

                        break ;
                    }

                    case AVL_RIGHT_HEAVY: {

                        rotate_right(node);
                        *balanced = 1;
                    }
                }
            }
        }
        else {

            /* Handle finding a copy of the data */
            if (!((AVLNode*) BTN_data(*node))->is_hidden) {

                /* Do nothing since the data is in the tree and not hidden */\
                return -1;
            }
            else {

                /* Insert the new data and mark it as not hidden */
                if (tree->destroy != NULL) {

                    /* Destroy the hidden data since it is being replaced */
                    tree->destroy(((AVLNode*) BTN_data(*node))->data);
                }

                ((AVLNode*) BTN_data(*node))->data = (void*) data;
                ((AVLNode*) BTN_data(*node))->is_hidden = 0;

                *balanced = 1;
            }
        }
    }

    /* ======== */
    return 0;
}

/**
 * 
 */
static int hide(BST* tree, BTreeNode* node, const void* data) {

    int cmpval, retval;
    /* ======== */

    if (BTN_eob(node)) {
        return -1;
    }

    cmpval = tree->compare(data, ((AVLNode*) BTN_data(node))->data);

    if (cmpval < 0) {
        /* Move to the left */
        retval = hide(tree, BTNl(node), data);
    }
    else if (cmpval > 0) {
        /* Move to the right */
        retval = hide(tree, BTNr(node), data);
    }
    else {

        ((AVLNode*) BTN_data(node))->is_hidden = 1;
        retval = 0;
    }

    /* ======== */
    return retval;
}

/**
 * 
 */
static void* lookup(BST* tree, BTreeNode* node, const void* data) {

    int cmpval;
    void* retval;
    /* ======== */

    if (BTN_eob(node)) {
        return NULL;
    }

    cmpval = tree->compare(data, ((AVLNode*) BTN_data(node))->data);

    if (cmpval < 0) {
        retval = lookup(tree, BTNl(node), data);
    }
    else if (cmpval > 0) {
        retval = lookup(tree, BTNr(node), data);
    }
    else {

        if (!((AVLNode*) BTN_data(node))->is_hidden) {
            return ((AVLNode*) BTN_data(node))->data;
        }
        else {
            return NULL;
        }
    }

    /* ======== */
    return retval;
}

/* ================================================================ */
/* ======================== IMPLEMENTATION ======================== */
/* ================================================================ */

void BST_init(BST* tree, int (*compare)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    BTree_init(tree, destroy);
    tree->compare = compare;
}

void BST_destroy(BST* tree) {

    destroy_left(tree, NULL);
    memset(tree, 0, sizeof(BST));
}

int BST_insert(BST* tree, const void* data) {

    int balanced = 0;
    /* ======== */
    return insert(tree, &tree->root, data, &balanced);
}

int BST_remove(BST* tree, const void* data) {
    return hide(tree, tree->root, data);
}

void* BST_lookup(BST* tree, const void* data) {
    return lookup(tree, tree->root, data);
}