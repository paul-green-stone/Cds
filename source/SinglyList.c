#include "../include/SinglyList.h"

#include <stdio.h>

/* ================================================================ */
/* ============================ STATIC ============================ */
/* ================================================================ */

static Node* _create_node(const void* data) {

    Node* node = NULL;
    /* ======== */

    if ((node = calloc(1, sizeof(Node))) == NULL) {
        return NULL;
    }

    node->data = (void*) data;

    /* ======== */
    return node;
}

/* ================================================================ */
/* ========================== INTERFACE =========================== */
/* ================================================================ */

void sList_init(sList* list, void (*destroy)(void* data)) {

    list->size = 0;
    list->head = list->tail = NULL;
    list->destroy = destroy;
}

/* ================================================================ */

void sList_destroy(sList* list) {

    void* data = NULL;
    /* ======== */

    while (list->size > 0) {

        data = sList_remove_first(list);

        if (list->destroy != NULL) { list->destroy(data); }
    }

    list->head = list->tail = NULL;
    list->destroy = NULL;
}

/* ================================================================ */

int sList_insert_last(sList* list, const void* data) {

    Node* node = NULL;
    /* ======== */

    if ((node = _create_node(data)) == NULL) {
        return -1;
    }

    if (list->size == 0) {
        list->head = list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }

    node->sentinel = list;
    list->size++;

    /* ======== */
    return 0;
}

/* ================================================================ */

int sList_insert_first(sList* list, const void* data) {

    Node* node = NULL;
    /* ======== */

    if ((node = _create_node((void*) data)) == NULL) {
        return -1;
    }

    if (list->size == 0) {
        list->head = list->tail = node;
    }
    else {
        node->next = list->head;
        list->head = node;
    }

    node->sentinel = list;
    list->size++;

    /* ======== */
    return 0;
}

/* ================================================================ */

void* sList_remove_last(sList* list) {

    Node* previous = NULL;
    Node* current = NULL;
    void* data = NULL;
    /* ======== */

    if (list->size > 0) {

        data = list->tail->data;
        current = list->tail;

        if (list->size == 1) {
            list->head = list->tail = NULL;
        }
        else {
            for (previous = list->head, current = previous->next; current != list->tail; previous = current, current = current->next) ;

            list->tail = previous;
            list->tail->next = NULL;
        }

        free(current);
        list->size--;
    }

    /* ======== */
    return data;
}

/* ================================================================ */

void* sList_remove_first(sList* list) {

    Node* node = NULL;
    void* data = NULL;
    /* ======== */

    /* === Do not allow removal from an empty list === */
    if (list->size > 0) {

        data = list->head->data;
        node = list->head;

        if (list->size == 1) {
            list->head = list->tail = NULL;
        }
        else {
            list->head = list->head->next;
        }

        free(node);
        list->size--;
    }

    /* ======== */
    return data;
}

/* ================================================================ */

Node* sList_find(const sList* list, const void* data, int (*match)(const void* key1, const void* key2)) {

    if ((list->size == 0) || (match == NULL) || (data == NULL)) {
        return NULL;
    }

    for (Node* t = list->head; t != NULL; t = t->next) {
        
        if (match(data, t->data) == 1) {
            return t;
        }
    }

    /* ======== */
    return NULL;
}

/* ================================================================ */

void* sList_remove(sList* list, Node* node) {

    Node* previous = NULL;
    Node* current = NULL;

    void* data = NULL;
    /* ======== */

    if (list->size == 0) {
        return NULL;
    }

    if (node->sentinel != list) {
        return NULL;
    }

    if (node == list->head) { return sList_remove_first(list); }

    if (node == list->tail) { return sList_remove_last(list); }

    for (previous = list->head, current = previous->next; current != node; previous = current, current = current->next) ;

    previous->next = current->next;
    current->next = NULL;

    free(node);
    list->size--;

    /* ======== */
    return 0;
}

/* ================================================================ */

int sList_insert_after(sList* list, Node* node, const void* data) {

    Node* t = NULL;
    Node* n = NULL;

    /* ======== */

    if ((list->size == 0) || (node == NULL) || (data == NULL) || (node->sentinel != list)) {
        return -1;
    }

    if (node == list->tail) { return sList_insert_last(list, data); }

    if ((n = _create_node(data)) == NULL) {
        return -1;
    }

    for (t = list->head; t != node; t = t->next) ;

    n->sentinel = list;
    n->next = t->next;
    node->next = n;

    list->size++;

    /* ======== */
    return 0;
}

/* ================================================================ */

int sList_insert_before(sList* list, Node* node, const void* data) {

    Node* t = NULL;
    Node* n = NULL;

    /* ======== */

    if ((list->size == 0) || (node == NULL) || (data == NULL) || (node->sentinel != list)) {
        return -1;
    }

    if (node == list->head) { return sList_insert_first(list, data); }

    if ((n = _create_node(data)) == NULL) {
        return -1;
    }

    for (t = list->head; t->next != node; t = t->next) ;

    n->sentinel = list;
    n->next = node;
    t->next = n;

    list->size++;

    /* ======== */
    return 0;
}

/* ================================================================ */
