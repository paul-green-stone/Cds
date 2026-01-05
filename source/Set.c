#include "../include/Set.h"

void Set_init(Set* set, int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    sList_init(set, destroy); 
    set->match = match;
}

void Set_destroy(Set* set) {
    sList_destroy(set);
}

int Set_insert(Set* set, const void* data) {

    /* Do not allow the insertion of duplicate */
    if (Set_is_member(set, data)) {
        return 1;
    }

    /* ======== */
    return sList_insert_last(set, data);
}

void* Set_remove(Set* set, const void* data) {

    Node* node = sList_find(set, data, set->match);
    void* _data = NULL;

    if (node) {
        _data = sList_remove(set, node);
    }

    /* ======== */
    return _data;
}

int Set_union(Set* setu, const Set* set1, const Set* set2) {

    Set_init(setu, set1->match, NULL);

    for (Node* node = set1->head; node != NULL; node = node->next) {

        if (sList_insert_last(setu, node->data) == -1) {

            Set_destroy(setu);
            /* ======== */
            return 1;
        }
    }

    for (Node* node = set2->head; node != NULL; node = node->next) {

        if (Set_is_member(set1, node->data)) {
            continue ;
        }
        else {

            if (sList_insert_last(setu, node->data) == -1) {
                
                Set_destroy(setu);
                /* ======== */
                return 1;
            }
        }
    }

    /* ======== */
    return 0;
}

int Set_intersection(Set* seti, const Set* set1, const Set* set2) {

    Set_init(seti, set1->match, NULL);

    for (Node* node = set1->head; node != NULL; node = node->next) {

        if (Set_is_member(set2, node->data)) {

            if (sList_insert_last(seti, node->data) == -1) {

                Set_destroy(seti);
                /* ========= */
                return -1;
            }
        }
    }

    /* ======== */
    return 0;
}

int Set_difference(Set* setd, const Set* set1, const Set* set2) {

    Set_init(setd, set1->match, NULL);

    for (Node* node = set1->head; node != NULL; node = node->next) {

        if (!Set_is_member(set2, node->data)) {

            if (sList_insert_last(setd, node->data) == -1) {

                Set_destroy(setd);
                /* ========= */
                return -1;
            }
        }
    }

    /* ======== */
    return 0;    
}

int Set_is_member(const Set* set, const void* data) {
    return sList_find(set, data, set->match) != NULL ? 1 : 0;
}

int Set_is_subset(const Set* set1, const Set* set2) {

    if (set1->size > set2->size) {
        return 0;
    }

    for (Node* node = set1->head; node != NULL; node = node->next) {

        if (!Set_is_member(set2, node->data)) {
            return 0;
        }
    }
    
    /* ========= */
    return 1;
}

int Set_is_equal(const Set* set1, const Set* set2) {

    if (set1->size != set2->size) {
        return 0;
    }

    return Set_is_subset(set1, set2);
}