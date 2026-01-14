#ifndef _HEAP_H
#define _HEAP_H

typedef struct heap {

    int (*compare)(const void* key1, const void* key2);
    void (*destroy)(void* data);

    int size;

    void** tree;
} Heap;

/**
 * Macro that evaluates to the number of nodes in the heap specified by `heap`.
 * 
 * @return Number of nodes in the heap.
 */
#define Heap_size(heap)

/**
 * Initializes the heap specified by `heap`. This operation must be
 * called for a heap before the heap can be used with any other operation.
 * The `compare` argument is a function used by various heap operations to compare
 * nodes when fixing the heap. This function should return `1` if `key1` > `key2`, `0` if
 * `key1` = `key2`, and `–1` if `key1` < `key2` for a top-heavy heap.
 * For a bottom-heavy heap, compare should reverse the cases that return `1` and `–1`.
 * The destroy argument provides a way to free dynamically allocated data when `Heap_destroy` is
 * called. For example, if the heap contains data dynamically allocated using `malloc`,
 * `destroy` should be set to `free` to free the data as the heap is destroyed. For
 * structured data containing several dynamically allocated members, `destroy`
 * should be set to a user-defined function that calls `free` for each dynamically
 * allocated member as well as for the structure itself. For a heap containing data that
 * should not be freed, destroy should be set to `NULL`.
 * 
 * @return None.
 */
void Heap_init(Heap* heap, int (*compare)(const void* key1, const void* key2), void (*destroy)(void* data));

/**
 * Destroys the heap specified by `heap`. No other operations are
 * permitted after calling `Heap_destroy` unless `Heap_init` is called again.
 * The `Heap_destroy` operation removes all nodes from a heap and calls the
 * function passed as `destroy` to `Heap_init` once for each node as it is removed,
 * provided `destroy` was not set to `NULL`.
 * 
 * @return None.
 */
void Heap_destroy(Heap* heap);

/**
 * Inserts a node into the heap specified by `heap`. The new node
 * contains a pointer to `data`, so the memory referenced by `data`
 * should remain valid as long as the node remains in the heap.
 * It is the responsibility of the caller to manage the storage associated with `data`.
 * 
 * @return `0` if inserting the node is successful, or `–1` otherwise.
 */
int Heap_insert(Heap* heap, const void* data);

/**
 * Extracts the node at the top of the heap specified by `heap`.
 * The caller takes ownership of the returned data pointer and is
 * responsible for freeing its associated memory when no longer needed.
 * 
 * @return A pointer to the data that was stored in the extracted element, or `NULL` if the heap is empty.
 */
void* Heap_extract(Heap* heap);

#endif /* _HEAP_H */