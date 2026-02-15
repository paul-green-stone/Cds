#include <string.h>
#include <stdio.h>

#include "../include/BTree.h"
#include "../include/Queue.h"

#define _terror (((struct information*) (container)->_info)->last_error_code)
#define _tsize  (((struct information*) (container)->_info)->size)
#define _troot  (((struct information*) (container)->_info)->root)

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

static struct information {

    struct binary_tree_node* root;

    size_t size;
    int last_error_code;
};

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

int BTree_init(BTree* container, void (*destroy)(void* data)) {

    struct information* info = NULL;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ========== The container has been initialized already ========== */
    if (container->_info != NULL) {

        _terror = CONTAINER_ERROR_ALREADY_INIT;
        /* ======== */
        return CONTAINER_ERROR_ALREADY_INIT;
    }

    /* ================== Memory allocation failure =================== */
    if ((info = calloc(1, sizeof(struct information))) == NULL) {
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    info->size = 0;
    info->root = NULL;
    info->last_error_code = CONTAINER_SUCCESS;

    container->_info = info;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_destroy(BTree* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    BTree_remove_l(container, NULL);
    free(container->_info);
    memset(container, 0, sizeof(BTree));
    
    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_insert_l(BTree* container, BTreeNode* node, const void* data) {

    BTreeNode* new_node, **position;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ============== Determine where to insert the node ============== */
    if (node == NULL) {

        /* ====== Allow insertion at the root only in an empty tree ======= */
        if (_tsize > 0) {
            return -1;
        }
        
        position = &_troot;
    }
    else {

        /* ===== Normally allow insertion only at the end of a branch ===== */
        if (BTNl(node)) {
            return -1;
        }

        position = &node->left;
    }

    /* ================== Memory allocation failure =================== */
    if ((new_node = calloc(1, sizeof(BTreeNode))) == NULL) {
        
        _terror = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    new_node->data = (void*) data;
    new_node->left = new_node->right = NULL;
    *position = new_node;

    _tsize++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_insert_r(BTree* container, BTreeNode* node, const void* data) {

    BTreeNode* new_node, **position;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ============== Determine where to insert the node ============== */
    if (node == NULL) {

        /* ====== Allow insertion at the root only in an empty tree ======= */
        if (_tsize > 0) {
            return -1;
        }
        
        position = &_troot;
    }
    else {

        /* ===== Normally allow insertion only at the end of a branch ===== */
        if (BTNr(node)) {
            return -1;
        }

        position = &node->right;
    }

    /* ================== Memory allocation failure =================== */
    if ((new_node = calloc(1, sizeof(BTreeNode))) == NULL) {
        
        _terror = CONTAINER_ERROR_OUT_OF_MEMORY;
        /* ======== */
        return CONTAINER_ERROR_OUT_OF_MEMORY;
    }

    new_node->data = (void*) data;
    new_node->left = new_node->right = NULL;
    *position = new_node;

    _tsize++;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_remove_l(BTree* container, BTreeNode* node) {

    BTreeNode** position;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* =========== Do not allow removal from an empty tree ============ */
    if (_tsize == 0) {

        _terror = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    /* ================ Determine where to remove nodes =============== */
    if (node == NULL) {
        position = &_troot;
    }
    else {
        position = &node->left;
    }

    /* ======================= Remove the nodes ======================= */
    if (*position != NULL) {

        BTree_remove_l(container, *position);
        BTree_remove_r(container, *position);

        if (container->destroy != NULL) {
            /* Call a user-defined function to free dynamically allocated data */
            container->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        _tsize--;
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_remove_r(BTree* container, BTreeNode* node) {

    BTreeNode** position;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* =========== Do not allow removal from an empty tree ============ */
    if (_tsize == 0) {

        _terror = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    /* ================ Determine where to remove nodes =============== */
    if (node == NULL) {
        position = &_troot;
    }
    else {
        position = &node->right;
    }

    /* ======================= Remove the nodes ======================= */
    if (*position != NULL) {

        BTree_remove_l(container, *position);
        BTree_remove_r(container, *position);

        if (container->destroy != NULL) {
            /* Call a user-defined function to free dynamically allocated data */
            container->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        _tsize--;
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_merge(BTree* container, BTree* left, BTree* right, const void* data) {

    int error_code;
    /* ======== */

    /* ================== Initialize the merged tree ================== */
    BTree_init(container, left->destroy);

    /* ================ Make sure containers is valid ================= */
    if ((left == NULL) || (right == NULL)) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if ((left->_info == NULL) || (right->_info == NULL)) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ===== Insert the data for the root node of the merged tree ===== */
    if ((error_code = BTree_insert_l(container, NULL, data)) != CONTAINER_SUCCESS) {

        BTree_destroy(container);
        /* ======== */
        return error_code;
    }

    /* ===== Merge the two binary trees into a single binary tree ===== */
    _troot->left = ((struct information*) left->_info)->root;
    _troot->right = ((struct information*) left->_info)->root;

    /* ============ Adjust the size of the new binary tree ============ */
    _tsize = BTree_size(left) + BTree_size(right);

    /* ==== Do not let the original trees access the merged nodes ===== */
    ((struct information*) left->_info)->root = NULL;
    ((struct information*) left->_info)->size = 0;
    ((struct information*) right->_info)->root = NULL;
    ((struct information*) right->_info)->size = 0;

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int BTree_print(const BTree* container, void (*print)(const void* data)) {

    Queue q = {0};
    BTreeNode* node = _troot;
    int level_size = 1;
    int elm_added = 0;
    int error_code;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ========================= No callback ========================== */
    if ((print == NULL)) {

        _terror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }

    /* ====================== The tree is empty ======================= */
    if (_tsize == 0) {

        _terror = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    /* ===================== Initializing a queue ===================== */
    if ((error_code = Queue_init(&q, NULL)) != CONTAINER_SUCCESS) {

        _terror = error_code;
        /* ======== */
        return error_code;
    }

    if ((error_code = Queue_enqueue(&q, node)) != CONTAINER_SUCCESS) {
        
        _terror = error_code;
        Queue_destroy(&q);
        /* ======== */
        return error_code;
    }

    while (Queue_size(&q) > 0) {
        
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

            if (i + 1 != level_size) {
                printf(", ");
            }
        }

        level_size = elm_added;
        elm_added = 0;
        printf("\n");
    }

    Queue_destroy(&q);

    /* ======== */
    return CONTAINER_SUCCESS;
}

int BTree_lookup(const BTree* container, int (*match)(const void* key1, const void* key2), const void* data, BTreeNode** result) { 

    Queue q = {0};
    BTreeNode* node = NULL;
    int error_code;
    /* ======== */

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ========================= No callback ========================== */
    if ((match == NULL)) {

        _terror = CONTAINER_ERROR_NO_CALLBACK;
        /* ======== */
        return CONTAINER_ERROR_NO_CALLBACK;
    }

    /* ================ There is no data to search for ================ */
    if (data == NULL) {

        _terror = CONTAINER_ERROR_NULL_DATA;
        /* ======== */
        return CONTAINER_ERROR_NULL_DATA;
    }

    /* ================== Can't save data into NULL =================== */
    if (result == NULL) {

        _terror = CONTAINER_ERROR_NULL_OUTPUT;
        /* ======== */
        return CONTAINER_ERROR_NULL_OUTPUT;
    }
    
    /* ========================== Empty tree ========================== */
    if (_tsize == 0) {

        _terror = CONTAINER_ERROR_EMPTY;
        /* ======== */
        return CONTAINER_ERROR_EMPTY;
    }

    /* ===================== Initializing a queue ===================== */
    if ((error_code = Queue_init(&q, NULL)) != CONTAINER_SUCCESS) {

        _terror = error_code;
        /* ======== */
        return error_code;
    }

     if ((error_code = Queue_enqueue(&q, _troot)) != CONTAINER_SUCCESS) {
        
        _terror = error_code;
        Queue_destroy(&q);
        /* ======== */
        return error_code;
    }

    while (Queue_size(&q) > 0) {

        node = Queue_dequeue(&q);

        if (match(node->data, data) == 1) {
            
            *result = node;
            Queue_destroy(&q);
            /* ======== */
            return CONTAINER_SUCCESS;
        }

        if (node->left) { Queue_enqueue(&q, node->left); }
        if (node->right) { Queue_enqueue(&q, node->right); }
    }

    Queue_destroy(&q);

    /* ======== */
    return CONTAINER_ERROR_NOT_FOUND;
}

/* ================================================================ */

ssize_t BTree_size(const BTree* container) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {
        return CONTAINER_ERR_NULL_PTR;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {
        return CONTAINER_ERROR_UNINIT;
    }

    /* ======== */
    return _tsize;
}

/* ================================================================ */

BTreeNode* BTree_root(const BTree* container, int* error_code) {

    /* =============== Make sure the container is valid =============== */
    if (container == NULL) {

        *error_code = CONTAINER_ERR_NULL_PTR;
        /* ======== */
        return NULL;
    }

    /* ============ The container has not been initialized ============ */
    if (container->_info == NULL) {

        *error_code = CONTAINER_ERROR_UNINIT;
        /* ======== */
        return NULL;
    }

    /* ======== */
    return _troot;
}
