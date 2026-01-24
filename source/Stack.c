#include "../include/Stack.h"

int Stack_init(Stack* stack, void (*destroy)(void* data)) {
    return sList_init(stack, destroy, NULL);
}

/* ================================================================ */

int Stack_destroy(Stack* stack) {
    return sList_destroy(stack);
}

/* ================================================================ */

int Stack_push(Stack* stack, void* data) {
    return sList_insert_last(stack, data);
}

/* ================================================================ */

void* Stack_pop(Stack* stack) {

    void* data = NULL;
    /* ======== */

    sList_remove_last(stack, &data);

    /* ======== */
    return data;
}

/* ================================================================ */

const void* Stack_peek(const Stack* stack) {

    const void* data = NULL;
    /* ======== */

    data = sNode_data(sList_tail(stack));

    /* ======== */
    return data;
}
