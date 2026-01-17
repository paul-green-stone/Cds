#include "../include/Stack.h"

StackResult Stack_init(Stack* stack, void (*destroy)(void* data)) {
    return sList_init(stack, destroy, NULL);
}

/* ================================================================ */

StackResult Stack_destroy(Stack* stack) {
    return sList_destroy(stack);
}

/* ================================================================ */

StackResult Stack_push(Stack* stack, void* data) {
    return sList_insert_last(stack, data);
}

/* ================================================================ */

void* Stack_pop(Stack* stack) {
    return sList_remove_last(stack);
}

/* ================================================================ */

const void* Stack_peek(Stack* stack) {

    if (stack == NULL) {
        return NULL;
    }
    
    if (stack->size > 0) {
        stack->last_error = SLIST_ERR_EMPTY;
        /* ======== */
        return sNode_data(stack->tail);
    }

    /* ======== */
    return NULL;
}
