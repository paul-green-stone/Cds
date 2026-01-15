#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

#include "Heap.h"

typedef Heap PQ;

/**
 * Macro that evaluates to the highest priority element in the priority queue specified by `queue`.
 * 
 * @return Highest priority element in the priority queue, or `NULL if the priority queue is empty.
 */
#define pqpeek(queue) ((queue)->tree == NULL || (queue)->size == 0 ? NULL : (queue)->tree[0])

/**
 * Macro that evaluates to the number of elements in the priority queue specified by `queue`.
 * 
 * @return Number of elements in the priority queue.
 */
#define pqsize(queue) (queue)->size

/**
 * Initializes the priority queue specified by `queue`. This operation
 * must be called for a priority queue before it can be used with any other operation.
 * The `compare` argument is a function used by various priority queue operations
 * in maintaining the priority queue’s heap property. This function should
 * return `1` if `key1` > `key2`, `0` if `key1` = `key2`, and `–1` if `key1` < `key2` for a priority
 * queue in which large keys have a higher priority. For a priority queue in which
 * smaller keys have a higher priority, `compare` should reverse the cases that return 1
 * and –1. The destroy argument provides a way to free dynamically allocated data
 * when `PQ_destroy` is called. For example, if the priority queue contains data
 * dynamically allocated using `malloc`, destroy should be set to `free` to free the data
 * as the priority queue is destroyed. For structured data containing several dynamically
 * allocated members, `destroy` should be set to a user-defined function that
 * calls `free` for each dynamically allocated member as well as for the structure itself.
 * For a priority queue containing data that should not be freed, `destroy` should be
 * set to `NULL`.
 * 
 * @return None.
 */
#define pqinit Heap_init

/**
 * Destroys the priority queue specified by `queue. No other operations
 * are permitted after calling `PQ_destroy` unless `PQ_init` is called again.
 * The `PQ_destroy` operation extracts all elements from a priority queue and calls
 * the function passed as `destroy` to `PQ_init` once for each element as it is
 * extracted, provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
#define pqdest Heap_destroy

/**
 * Inserts an element into the priority queue specified by `queue`.
 * The new element contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the element remains in the priority queue.
 * It is the responsibility of the caller to manage the storage associated with data.
 * 
 * @return `0` if inserting the element is successful, or `–1` otherwise.
 */
#define pqinst Heap_insert

/**
 * Extracts the element at the top of the priority queue specified by `pqueue`.
 * It is the responsibility of the caller to manage the storage associated with the data
 * 
 * @return A pointer to the data that was stored in the extracted element, or `NULL` if the queue is empty.
 */
#define pqextr Heap_extract

#endif /* _PRIORITY_QUEUE_H */