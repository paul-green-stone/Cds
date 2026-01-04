#ifndef _STACK_H
#define _STACK_H

#include "SinglyList.h"

typedef sList Stack;

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
 * @return None.
 */
void Stack_init(Stack* stack, void (*destroy)(void* data));

/**
 * Destroys the stack specified by `stack`. No other operations are
 * permitted after calling `Stack_destroy` unless `Stack_init` is called again.
 * The `Stack_destroy` operation removes all elements from a stack and calls the function passed
 * as `destroy` to `Stack_init` once for each element as it is removed, provided
 * destroy was not set to `NULL`.
 * 
 * @return None.
 */
void Stack_destroy(Stack* stack);

/**
 * Pushes an element onto the stack specified by `stack`. The new
 * element contains a pointer to `data`, so the memory referenced by `data` should
 * remain valid as long as the element remains in the stack. It is the responsibility of
 * the caller to manage the storage associated with data.
 * 
 * @return `0` if pushing the element is successful, or `–1` otherwise.
 */
int Stack_push(Stack* stack, const void* data);

/**
 * Pops an element off the stack specified by `stack`. It is the
 * responsibility of the caller to manage the storage associated with the data.
 * The caller takes ownership of the returned data pointer and must free it using the
 * `destroy` function that was specified during stack initialization,
 * or manage it appropriately based on how the data was originally allocated.
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
 * @return A pointer to the data stored in the top element of the stack specified by stack, or `NULL` if the stack is empty.

 */
const void* Stack_peek(const Stack* stack); 

#endif /* _STACK_H */

