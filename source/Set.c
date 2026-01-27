#include "../include/Set.h"

int Set_init(Set* set, int (*match)(const void* key1, const void* key2), void (*destroy)(void* data)) {
    return sList_init(set, destroy, match);
}

/* ================================================================ */

int Set_destroy(Set* set) {
    return sList_destroy(set);
}

/* ================================================================ */

int Set_insert(Set* set, void* data) {

    /* Do not allow the insertion of duplicate */
    if (Set_is_member(set, data)) {
        return 1;
    }

    /* ======== */
    return sList_insert_last(set, data);
}
/* ================================================================ */

int Set_remove(Set* set, const void* data) {

    sNode* node = NULL;
    void* _data = NULL;

    int exit_code = CONTAINER_SUCCESS;
    /* ======== */
    
    exit_code = sList_find(set, data, &node, NULL);

    if (node != NULL) {
        exit_code = sList_remove(set, node, &_data);

        if (set->destroy != NULL) {
            set->destroy(_data);
        }
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

int Set_union(Set* setu, const Set* set1, const Set* set2) {

    int exit_code = CONTAINER_SUCCESS;
    /* ======== */

    if ((exit_code = Set_init(setu, set1->match, NULL)) != CONTAINER_SUCCESS) {
        return exit_code;
    }

    for (sNode* node = sList_head(set1); node != NULL; node = sNode_next(node)) {

        if ((exit_code = sList_insert_last(setu, sNode_data(node))) != CONTAINER_SUCCESS) {

            Set_destroy(setu);
            /* ======== */
            return exit_code;
        }
    }

    for (sNode* node = sList_head(set2); node != NULL; node = sNode_next(node)) {

        if (Set_is_member(set1, sNode_data(node))) {
            continue ;
        }
        else {

            if ((exit_code = sList_insert_last(setu, sNode_data(node))) != CONTAINER_SUCCESS) {
                
                Set_destroy(setu);
                /* ======== */
                return exit_code;
            }
        }
    }

    /* ======== */
    return CONTAINER_SUCCESS;
}

/* ================================================================ */

int Set_intersection(Set* seti, const Set* set1, const Set* set2) {

    int exit_code = CONTAINER_SUCCESS;
    /* ======== */

    Set_init(seti, set1->match, NULL);

    for (sNode* node = sList_head(set1); node != NULL; node = sNode_next(node)) {

        if (Set_is_member(set2, sNode_data(node))) {

            if ((exit_code = sList_insert_last(seti, sNode_data(node))) != CONTAINER_SUCCESS) {

                Set_destroy(seti);
                /* ========= */
                return exit_code;
            }
        }
    }

    /* ======== */
    return exit_code;
}

/* ================================================================ */

int Set_difference(Set* setd, const Set* set1, const Set* set2) {

    int exit_code = CONTAINER_SUCCESS;
    /* ======== */

    Set_init(setd, set1->match, NULL);

    for (sNode* node = sList_head(set1); node != NULL; node = sNode_next(node)) {

        if (!Set_is_member(set2, sNode_data(node))) {

            if ((exit_code = sList_insert_last(setd, sNode_data(node))) != CONTAINER_SUCCESS) {

                Set_destroy(setd);
                /* ========= */
                return exit_code;
            }
        }
    }

    /* ======== */
    return exit_code;    
}

/* ================================================================ */

int Set_is_member(const Set* set, const void* data) {

    sNode* node =  NULL;
    /* ======== */
    return sList_find(set, data, &node, NULL) == CONTAINER_SUCCESS ? 1 : 0;
}

/* ================================================================ */

int Set_is_subset(const Set* set1, const Set* set2) {

    /* An empty set is a subset of any other */
    if ((set1 == NULL || set2 == NULL) || ((set1 == NULL) && (set2 == NULL))) {
        return 1;
    }

    if (sList_size(set1) > sList_size(set2)) {
        return 0;
    }

    for (sNode* node = sList_head(set1); node != NULL; node = sNode_next(node)) {

        if (!Set_is_member(set2, sNode_data(node))) {
            return 0;
        }
    }
    
    /* ========= */
    return 1;
}

/* ================================================================ */

int Set_is_equal(const Set* set1, const Set* set2) {

    /* Two empty sets are equal */
    if ((set1 == NULL) && (set2 == NULL)) {
        return 1;
    }

    if (((set1 == NULL) && (set2 != NULL)) || ((set1 != NULL) && (set2 == NULL))) {
        return 0;
    }

    if (sList_size(set1) > sList_size(set2)) {
        return 0;
    }

    /* ======== */
    return Set_is_subset(set1, set2);
}