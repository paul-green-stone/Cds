#include "../include/Queue.h"

int Queue_init(Queue *queue, void (*destroy)(void *data)) {
    return sList_init(queue, destroy, NULL);
}

int Queue_destroy(Queue* queue) {
    return sList_destroy(queue);
}

int Queue_enqueue(Queue* queue, void* data) {
    return sList_insert_last(queue, data);
}

void* Queue_dequeue(Queue* queue) {

    void* data = NULL;
    /* ======== */
    sList_remove_first(queue, &data);

    /* ======== */
    return data;
}

const void* Queue_peek(const Queue* queue) {

    const void* data = NULL;
    /* ======== */

    data = sNode_data(sList_head(queue));

    /* ======== */
    return data;
}
