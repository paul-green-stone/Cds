#include "../include/Stack.h"

void Stack_init(Stack* stack, void (*destroy)(void* data)) {
    sList_init(stack, destroy);
}

/* ================================================================ */

void Stack_destroy(Stack* stack) {
    sList_destroy(stack);
}

/* ================================================================ */

int Stack_push(Stack* stack, const void* data) {
    return sList_insert_last(stack, data);
}

/* ================================================================ */

void* Stack_pop(Stack* stack) {
    return sList_remove_last(stack);
}

/* ================================================================ */

const void* Stack_peek(const Stack* stack) {
    
    if (stack->size > 0) {
        return stack->tail->data;
    }

    /* ======== */
    return NULL;
}
