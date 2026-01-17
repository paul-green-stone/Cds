#include "../include/Queue.h"

QueueResult Queue_init(Queue *queue, void (*destroy)(void *data)) {
    return sList_init(queue, destroy, NULL);
}

QueueResult Queue_destroy(Queue* queue) {
    return sList_destroy(queue);
}

QueueResult Queue_enqueue(Queue* queue, const void* data) {
    return sList_insert_last(queue, data);
}

void* Queue_dequeue(Queue* queue) {
    return sList_remove_first(queue);
}

const void* Queue_peek(Queue* queue) {

    if (queue == NULL) {
        return NULL;
    }
    
    if (queue->size > 0) {
        queue->last_error = SLIST_ERR_EMPTY;
        /* ======== */
        return sNode_data(queue->head);
    }

    /* ======== */
    return NULL;
}
