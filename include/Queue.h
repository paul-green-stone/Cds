#ifndef _QUEUE_H
#define _QUEUE_H

#include "SinglyList.h"

typedef sList Queue;

/**
 * Initializes the queue specified by `queue`. his operation must be
 * called for a queue before the queue can be used with any other operation.
 * The `destroy` argument provides a way to free dynamically allocated data when
 * `Queue_destroy` is called. or a queue containing data that should not be freed,
 * `destroy` should be set to `NULL`.
 * 
 * @return None.
 */
void Queue_init(Queue *queue, void (*destroy)(void *data));

/**
 * Destroys the queue specified by `queue`. No other operations are
 * permitted after calling `Queus_destroy` unless `Queue_init` is called again.
 * The `Queus_destroy` operation removes all elements from a queue and calls the function passed
 * as `destroy` to `Queue_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @return None.
 */
void Queue_destroy(Queue* queue);

/**
 * Enqueues an element at the tail of the queue specified by `queue`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the queue. It is the
 * responsibility of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if enqueuing the element is successful, or `–1` otherwise.
 */
int Queue_enqueue(Queue* queue, const void* data);

/**
 * Dequeues an element from the head of the queue specified by `queue`.
 * It is the responsibility of the caller to manage the storage associated
 * with the data.
 * 
 * @return A pointer to the data that was stored in the removed element, or `NULL` if the queue is empty.
 */
void* Queue_dequeue(Queue* queue);

/**
 * This operation does not remove the first element from the queue.
 * The returned pointer remains valid as long as the element remains in the queue,
 * and the caller should not modify or free the data through this pointer.
 * 
 * @return A pointer to the data stored in the first element of the queue specified by `queue`, or `NULL` if the queue is empty.
 */
const void* Queue_peek(const Queue* queue);

#endif /* _QUEUE_H */