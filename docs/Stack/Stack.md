# README

## Cds

### Stack Data Structure (C)

A stack is a data structure that follows the Last-In-First-Out (LIFO) principle. This implementation is based on a singly linked list, allowing
efficient stack operations. The stack supports basic operations like initialization, pushing, popping, peeking, and destruction.

## Table of Contents

1. [Overview](#Overview)
2. [Features](#Features)
3. [Installation](#Installation)
4. [Usage](#Usage)
5. [Example](#Example)
6. [Contribute](#Contribute)

## Overview :eyes:

This stack implementation is based on a singly linked list, allowing for efficient push and pop operations. The stack supports a variety of operations, including initialization, element insertion, removal, and retrieval of the top element.

## Features :rocket:

* **LIFO (Last In, First Out)**: The most recently added element is the first one to be removed.
* **Dynamic Size**: The stack grows and shrinks as elements are added and removed.
* **Memory Management**: The stack automatically handles memory management for its elements.

## Installation :hammer_and_wrench:

To use the data structure, simply download or clone this repository, then include the header file in your project:

```bash
git clone https://github.com/paul-green-stone/Cds.git
```

Include the header in your C source files:

```C
#include "Cds/include/Stack.h"
```

Link your program with the source files:

```bash
gcc -o output main.c source/SinglyList.c source/Stack.c
```

## Usage :gear:

### Initializing a Stack

To create a new stack, initialize it using the `Stack_init` function. You can provide a custom function to handle data destruction:

```C
Stack stack;
Stack_init(&stack, free);
```

### Pushing Elements

To add an integer into the stack, use `Stack_push` function:

```C
for (size_t i = 0; i < 10; i++) {
    
    int* data = malloc(sizeof(int));
    Stack_push(&stack, data);
}
```

### Popping Elements

To pop an element from the stack, you can use the `Stack_pop` function:

```C
while (stack.size) {

    int* data = Stack_pop(&stack);
    free(data);
}
```
Popping data from a stack does not reclaim its memory, so it is necessary to free it when you are done working with data.
Usually, you use the same function as you specified in `Stack_init` to destroy data, so you can use `stack.destroy(data)`.

### Peeking Data

To peek at the data on top of the stack, use `Stack_peek`. The function returns the top element without removing it or changing the stack size:

```C
const int* top = Stack_peek(&stack);
printf("%d\n", *top);
```

### Destroying a Stack

```C
Stack_destroy(&stack);
```

You destroy a stack by calling `Stack_destroy`. All data in the stack is passed to its `destroy` callback to reclaim memory. Note that the same `destroy` function from `Stack_init` is called on every element. If your stack contains heterogeneous data, this may cause segfaults—pop elements manually and use appropriate destroy functions instead.

### Examining Errors

If any function above fails, call `Stack_get_last_error` to examine why:

```C
/* Assuming the stack is not NULL */
StackResult res = Stack_push(&stack, data);
if (res != SLIST_OK) {
    printf("Error: %s\n", Stack_get_last_error(&stack));
}
```

## Example :computer:

Data you store in a stack can be as complex as you need it to be. Refer to the [example](../linked-lists/Singly-Linked.md#example-computer) section on how to write more advanced data for a container.

## Contribute

For contribution guidelines, check the [Contribute section](../../README.md#contribute).