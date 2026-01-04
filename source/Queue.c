#include "../include/Queue.h"

void Queue_init(Queue *queue, void (*destroy)(void *data)) {
    sList_init(queue, destroy);
}

void Queue_destroy(Queue* queue) {
    sList_destroy(queue);
}

int Queue_enqueue(Queue* queue, const void* data) {
    return sList_insert_last(queue, data);
}

void* Queue_dequeue(Queue* queue) {
    return sList_remove_first(queue);
}

const void* Queue_peek(const Queue* queue) {

    if (queue->size > 0) {
        return queue->head->data;
    }

    /* ======== */
    return NULL;
}
