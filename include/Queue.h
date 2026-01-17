#ifndef QUEUE_H
#define QUEUE_H

#include "SinglyList.h"

typedef sList Queue;
typedef sListResult QueueResult;

/**
 * Initializes the queue specified by `queue`. This operation must be
 * called for a queue before the queue can be used with any other operation.
 * The `destroy` argument provides a way to free dynamically allocated data when
 * `Queue_destroy` is called. For a queue containing data that should not be freed,
 * `destroy` should be set to `NULL`.
 * 
 * @param queue The queue to initialize.
 * @param destroy The function used to free the data in each queue element when the queue is destroyed.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 *  
 * @return `SLIST_OK` on success, error code otherwise.
 */
QueueResult Queue_init(Queue *queue, void (*destroy)(void *data));

/**
 * Destroys the queue specified by `queue`. No other operations are
 * permitted after calling `Queue_destroy` unless `Queue_init` is called again.
 * The `Queue_destroy` operation removes all elements from a queue and calls the function passed
 * as `destroy` to `Queue_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @param queue The queue to destroy.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
QueueResult Queue_destroy(Queue* queue);

/**
 * Enqueues an element at the tail of the queue specified by `queue`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the queue. It is the
 * responsibility of the caller to manage the storage associated with `data`.
 * 
 * @param queue The queue to enqueue the element onto.
 * @param data A pointer to the data to store in the queue.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
QueueResult Queue_enqueue(Queue* queue, const void* data);

/**
 * Dequeues an element from the head of the queue specified by `queue`.
 * It is the responsibility of the caller to manage the storage associated
 * with the data.
 * 
 * @param queue The queue to pop the element from.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return A pointer to the data that was stored in the queue, or `NULL` if the queue is empty.
 */
void* Queue_dequeue(Queue* queue);

/**
 * This operation does not remove the first element from the queue.
 * The returned pointer remains valid as long as the element remains in the queue,
 * and the caller should not modify or free the data through this pointer.
 * 
 * @param queue The queue to peek at.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return A pointer to the data stored in the front element of the queue, or `NULL` if the queue is empty.
 */
const void* Queue_peek(Queue* queue);

#endif /* QUEUE_H */