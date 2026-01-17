# README

## Cds

### Queue Data Structure (C)

A queue is a data structure that follows the First-In-First-Out (FIFO) principle. This implementation is based on a singly linked list, allowing efficient queue operations. The queue supports basic operations like initialization, enqueue, dequeue, peeking, and destruction.

## Table of Contents

1. [Overview](#Overview-eyes)
2. [Features](#Features-rocket)
3. [Installation](#Installation-hammer_and_wrench)
4. [Usage](#Usage-gear)
5. [Example](#Example-computer)
6. [Contribute](#Contribute)

## Overview :eyes:

This queue implementation is based on a singly linked list. It allows for efficient enqueue and dequeue operations while maintaining the FIFO order of elements. The queue supports a variety of operations, including initialization, element insertion, removal, and retrieval of the front element.

## Features :rocket:

* **FIFO (First In, First Out)**: The first element added is the first to be removed.
* **Dynamic Size**: The queue grows and shrinks as elements are added or removed.
* **Memory Management**: The queue automatically handles memory management for its elements.

## Installation :hammer_and_wrench:

To use the data structure, simply download or clone this repository, then include the header file in your project:

```bash
git clone https://github.com/paul-green-stone/Cds.git
```

Include the header in your C source files:

```C
#include "Cds/include/Queue.h"
```

Link your program with the source files:

```bash
gcc -o output main.c source/SinglyList.c source/Queue.c
```

## Usage :gear:

### Initializing a Queue

To create a new queue, initialize it using the `Queue_init` function. You can provide a custom function to handle data destruction:

```C
Queue queue;
Queue_init(&queue, free);
```

### Enqueuing Elements

To add an integer into the queue, use `Queue_enqueue` function:

```C
for (size_t i = 0; i < 10; i++) {
    
    int* data = malloc(sizeof(int));
    Queue_enqueue(&queue, data);
}
```

### Dequeuing Elements

To remove an element from the queue, you can use the `Queue_dequeue` function:

```C
while (queue.size) {

    int* data = Queue_dequeue(&queue);
    free(data);
}
```
Dequeuing data from a queue does not reclaim its memory, so it is necessary to free it when you are done working with data.
Usually, you use the same function as you specified in `Queue_init` to destroy data, so you can use `queue.destroy(data)`.

### Peeking Data

To peek at the data on top of the queue, use `Queue_peek`. The function returns the top element without removing it or changing the queue size:

```C
const int* top = Queue_peek(&queue);
printf("%d\n", *top);
```

### Destroying a Queue

```C
Queue_destroy(&queue);
```

You destroy a queue by calling `Queue_destroy`. All data in the queue is passed to its `destroy` callback to reclaim memory. Note that the same `destroy` function from `Queue_init` is called on every element. If your queue contains heterogeneous data, this may cause segfaults—pop elements manually and use appropriate destroy functions instead.

### Examining Errors

If any function above fails, call `Queue_get_last_error` to examine why:

```C
/* Assuming the queue is not NULL */
QueueResult res = Queue_enqueue(&queue, data);
if (res != SLIST_OK) {
    printf("Error: %s\n", Queue_get_last_error(&queue));
}
```

## Example :computer:

Data you store in a queue can be as complex as you need it to be. Refer to the [example](../linked-lists/Singly-Linked.md#example-computer) section on how to write more advanced data for a container.

## Contribute

For contribution guidelines, check the [Contribute section](../../README.md#contribute).