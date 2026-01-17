#ifndef STACK_H
#define STACK_H

#include "SinglyList.h"

typedef sList Stack;
typedef sListResult StackResult;

/**
 * Retrieves a human-readable error message corresponding to the last error encountered by
 * the stack. The error message is based on the last operation performed on the list and its result.
 * 
 * @param list A pointer to the stack from which the last error message will be retrieved.
 * If the list is `NULL`, the function will return `NULL`.
 * 
 * @return A pointer to a string containing the description of the last error encountered.
 */
#define Stack_get_last_error sList_get_last_error

/**
 * Initializes the stack specified by `stack`. This operation must be
 * called for a stack before the stack can be used with any other operation.
 * The `destroy` argument provides a way to free dynamically allocated data when
 * `stack_destroy` is called. For example, if the stack contains data dynamically allocated
 * using `malloc`, destroy should be set to `free` to free the data as the stack is
 * destroyed. For structured data containing several dynamically allocated members,
 * `destroy` should be set to a user-defined function that calls `free` for each
 * dynamically allocated member as well as for the structure itself. For a stack containin
 * data that should not be freed, destroy should be set to `NULL`.
 * 
 * @param stack The stack to initialize.
 * @param destroy The function used to free the data in each stack element when the stack is destroyed.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 *  
 * @return `SLIST_OK` on success, error code otherwise.
 */
StackResult Stack_init(Stack* stack, void (*destroy)(void* data));

/**
 * Destroys the stack specified by `stack`. No other operations are
 * permitted after calling `Stack_destroy` unless `Stack_init` is called again.
 * The `Stack_destroy` operation removes all elements from a stack and calls the function passed
 * as `destroy` to `Stack_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @param stack The stack to destroy.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
StackResult Stack_destroy(Stack* stack);

/**
 * Pushes an element onto the stack specified by `stack`. The new
 * element contains a pointer to `data`, so the memory referenced by `data` should
 * remain valid as long as the element remains in the stack. It is the responsibility of
 * the caller to manage the storage associated with data.
 * 
 * @param stack The stack to push the element onto.
 * @param data A pointer to the data to store in the stack.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return `SLIST_OK` on success, error code otherwise.
 */
StackResult Stack_push(Stack* stack, void* data);

/**
 * Pops an element off the stack specified by `stack`. It is the
 * responsibility of the caller to manage the storage associated with the data.
 * The caller takes ownership of the returned data pointer and must free it using the
 * `destroy` function that was specified during stack initialization,
 * or manage it appropriately based on how the data was originally allocated.
 * 
 * @param stack The stack to pop the element from.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return A pointer to the data that was stored in the stack, or `NULL` if the stack is empty.
 */
void* Stack_pop(Stack* stack);

/**
 * Returns a pointer to the data stored in the top element of the stack specified by `stack`.
 * This operation does not remove the top element from the stack. The returned pointer
 * remains valid as long as the element remains in the stack,
 * and the caller should not modify or free the data through this pointer.
 * 
 * @param stack The stack to peek at.
 * 
 * @note On failure, `last_error` is set to indicate the reason.
 * 
 * @return A pointer to the data stored in the top element of the stack, or `NULL` if the stack is empty.
 */
const void* Stack_peek(Stack* stack); 

#endif /* STACK_H */

