#ifndef STACK_H
#define STACK_H

#include "SinglyList.h"

typedef sList Stack;

/**
 * Retrieves a human-readable error message corresponding to the
 * last error encountered by the stack. The error message is
 * based on the last operation performed on the stack and its
 * result.
 * 
 * @param stack Pointer to the stack.
 * 
 * @return A null-terminated string containing the description of
 * the last error encountered.
 */
#define Stack_error sList_error

/**
 * Initializes the stack specified by `stack`. This operation must be
 * called for a stack before the stack can be used with any other
 * operation.
 * 
 * The `destroy` argument provides a way to free dynamically allocated data
 * when `Stack_destroy` is called. For example, if the stack contains data
 * dynamically allocated using `malloc`, `destroy` should be set to `free`
 * to free the data as the stack is destroyed. For structured data
 * containing several dynamically allocated members, `destroy` should be
 * set to a user-defined function that calls `free` for each dynamically
 * allocated member as well as for the structure itself. For a stack
 * containing data that should not be freed, `destroy` should be set to `NULL`.
 * 
 * @param stack     Pointer to the stack to initialize.
 * @param destroy   Optional destructor called on each element during destroy.
 *  
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Stack_init(Stack* stack, void (*destroy)(void* data));

/**
 * Destroys the stack specified by `stack`. No other operations are
 * permitted after calling `Stack_destroy` unless `Stack_init` is called
 * again. The `Stack_destroy` operation removes all elements from a stack
 * and calls the function passed as `destroy` to `Stack_init` once for
 * each element as it is removed, provided destroy was not set to `NULL`.
 * 
 * @param stack Pointer to the list to destroy.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Stack_destroy(Stack* stack);

/**
 * Pushes an element onto the stack specified by `stack`.
 * The new element contains a pointer to `data`, so the memory
 * referenced by `data` should remain valid as long as the
 * element remains in the stack.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param stack The stack to push the element onto.
 * @param data A pointer to the data to store in the stack.
 * 
 * @return `CONTAINER_SUCCESS` on success, error code otherwise.
 */
int Stack_push(Stack* stack, void* data);

/**
 * Pops an element off the stack specified by `stack`.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with the data.
 * 
 * @param stack The stack to pop the element from.
 * 
 * @return A pointer to the data that was stored in the stack, `NULL` otherwise.
 */
void* Stack_pop(Stack* stack);

/**
 * Returns a pointer to the data stored in the top element
 * of the stack specified by `stack`. This operation
 * does not remove the top element from the stack. The
 * returned pointer remains valid as long as the element
 * remains in the stack, and the caller should not modify
 * or free the data through this pointer.
 * 
 * @param stack The stack to peek at.
 * 
 * @return A pointer to the data stored in the top element of the stack, `NULL` otherwise.
 */
const void* Stack_peek(const Stack* stack);

#endif /* STACK_H */

