#include <string.h>
#include <stdio.h>

#include "../include/BTree.h"
#include "../include/Queue.h"

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

BTreeNode* BTN_match(BTreeNode* node, int (*match)(const void* data1, const void* data2), const void* data) {

    if (node == NULL) {
        return NULL;
    }
}

int BTree_init(BTree* tree, void (*destroy)(void* data)) {

    tree->size = 0;
    tree->compare = NULL;
    tree->destroy = NULL;
    tree->root = NULL;
}

void BTree_destroy(BTree* tree) {

    BTree_remove_l(tree, NULL);
    memset(tree, 0, sizeof(BTree));    
}

int BTree_insert_l(BTree* tree, BTreeNode* node, const void* data) {

    BTreeNode* new_node, **position;
    /* ======== */

    if (node == NULL) {

        if (tree->size > 0) {
            return -1;
        }
        
        position = &tree->root;
    }
    else {

        if (BTNl(node)) {
            return -1;
        }

        position = &node->left;
    }

    if ((new_node = calloc(1, sizeof(BTreeNode))) == NULL) {
        return -1;
    }

    new_node->data = (void*) data;
    new_node->left = new_node->right = NULL;
    *position = new_node;

    tree->size++;

    /* ======== */
    return 0;
}

int BTree_insert_r(BTree* tree, BTreeNode* node, const void* data) {

    BTreeNode* new_node, **position;
    /* ======== */

    if (node == NULL) {

        if (tree->size > 0) {
            return -1;
        }
        
        position = &tree->root;
    }
    else {

        if (BTNr(node)) {
            return -1;
        }

        position = &node->right;
    }

    if ((new_node = calloc(1, sizeof(BTreeNode))) == NULL) {
        return -1;
    }

    new_node->data = (void*) data;
    new_node->left = new_node->right = NULL;
    *position = new_node;

    tree->size++;

    /* ======== */
    return 0;
}

void BTree_remove_l(BTree* tree, BTreeNode* node) {

    BTreeNode** position;

    if (tree->size == 0) {
        return ;
    }

    if (node == NULL) {
        position = &tree->root;
    }
    else {
        position = &node->left;
    }

    if (*position != NULL) {

        BTree_remove_l(tree, *position);
        BTree_remove_r(tree, *position);

        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        tree->size--;
    }
}

void BTree_remove_r(BTree* tree, BTreeNode* node) {

    BTreeNode** position;

    if (tree->size == 0) {
        return ;
    }

    if (node == NULL) {
        position = &tree->root;
    }
    else {
        position = &node->right;
    }

    if (*position != NULL) {

        BTree_remove_l(tree, *position);
        BTree_remove_r(tree, *position);

        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        tree->size--;
    }
}

int BTree_merge(BTree* merge, BTree* left, BTree* right, const void* data) {

    BTree_init(merge, left->destroy);

    if (BTree_insert_l(merge, NULL, data) != 0) {

        BTree_destroy(merge);
        /* ======== */
        return -1;
    }

    merge->root->left = left->root;
    merge->root->right = right->root;

    merge->size = merge->size += left->size + right->size;

    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;

    /* ======== */
    return 0;
}

void BTree_print(const BTree* tree, void (*print)(const void* data)) {

    Queue q;
    BTreeNode* node = tree->root;
    int level_size = 1;
    int elm_added = 0;
    /* ======== */

    Queue_init(&q, NULL);

    if ((print == NULL)) { return ; }

    if (tree->size == 0) { return ; }

    Queue_enqueue(&q, node);

    while (q.size > 0) {
        
        for (int i = 0; i < level_size; i++) {

            node = Queue_dequeue(&q);
            print(node->data);

            if (node->left) {
                Queue_enqueue(&q, node->left);
                elm_added++;
            }
            if (node->right) {
                Queue_enqueue(&q, node->right);
                elm_added++;
            }

            if (i + 1 != level_size) { printf(", "); }
        }

        level_size = elm_added;
        elm_added = 0;
        printf("\n");
    }

    Queue_destroy(&q);
}

BTreeNode* BTree_lookup(const BTree* tree, int (*match)(const void* key1, const void* key2), const void* data) {

    Queue q;
    BTreeNode* node = NULL;
    /* ======== */

    if (match == NULL) { return NULL; }

    if (data == NULL) { return NULL; }

    if (tree->size == 0) { return NULL; }

    Queue_init(&q, NULL);
    Queue_enqueue(&q, tree->root);

    while (q.size > 0) {

        node = Queue_dequeue(&q);

        if (match(node->data, data) == 1) { return node; }

        if (node->left) { Queue_enqueue(&q, node->left); }
        if (node->right) { Queue_enqueue(&q, node->right); }
    }

    /* ======== */
    return NULL;
}
