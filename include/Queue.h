#ifndef QUEUE_H
#define QUEUE_H

#include "SinglyList.h"

typedef sList Queue;

/**
 * Initializes the queue specified by `queue`. This operation must
 * be called for a queue before the queue can be used with any other
 * operation. The `destroy` argument provides a way to free
 * dynamically allocated data when `Queue_destroy` is called.
 * For a queue containing data that should not be freed, `destroy`
 * should be set to `NULL`.
 * 
 * @param queue     Pointer to the queue to initialize.
 * @param destroy   Optional destructor called on each element during destroy.
 *  
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Queue_init(Queue *queue, void (*destroy)(void *data));

/**
 * Destroys the queue specified by `queue`. No other operations are
 * permitted after calling `Queue_destroy` unless `Queue_init` is
 * called again. The `Queue_destroy` operation removes all elements
 * from a queue and calls the function passed as `destroy` to
 * `Queue_init` once for each element as it is removed, provided
 * `destroy` was not set to `NULL`.
 * 
 * @param queue Pointer to the queue to destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Queue_destroy(Queue* queue);

/**
 * Enqueues an element at the tail of the queue specified by `queue`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the element
 * remains in the queue. 
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param queue     The queue to enqueue the element onto.
 * @param data      A pointer to the data to store in the queue.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Queue_enqueue(Queue* queue, void* data);

/**
 * Dequeues an element from the head of the queue specified by `queue`.
 * 
 * It is the responsibility of the caller to manage the storage associated
 * with the data.
 * 
 * @param queue The queue to pop the element from.
 * 
 * @return A pointer to the data that was stored in the queue, `NULL` otherwise.
 */
void* Queue_dequeue(Queue* queue);

/**
 * This operation does not remove the first element from the queue.
 * The returned pointer remains valid as long as the element remains
 * in the queue, and the caller should not modify or free the data
 * through this pointer.
 * 
 * @param queue The queue to peek at.
 * 
 * @return A pointer to the data stored in the front element of the queue, `NULL` otherswise.
 */
const void* Queue_peek(const Queue* queue);

#endif /* QUEUE_H */