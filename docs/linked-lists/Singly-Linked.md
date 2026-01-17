# README

## Cds

### Singly-Linked List Data Structure (C)

A simple, efficient, and flexible implementation of singly linked lists in C, with advanced error checking and memory management capabilities. This implementation provides basic operations for list manipulation, such as inserting, removing, and searching for nodes.

## Table of Contents

1. [Overview](#Overview-eyes)
2. [Features](#Features-rocket)
3. [Installation](#Installation-hammer_and_wrench)
4. [Usage](#Usage-gear)
5. [API Documentation](#API-Documentation-book)
6. [Error Handling](#Error-Handling-x)
7. [Example](#Example-computer)
8. [Contribute](#Contribute)

## Overview :eyes:

This code provides a simple implementation of a **singly linked list** in C. The linked list supports common operations like insertion at the head and tail, deletion from the head and tail, searching for nodes, and inserting/removing nodes after/before specific nodes. The list also includes robust error checking, including detection of corrupted lists and memory management issues.

## Features :rocket:

- **Simple Interface**: Easy-to-use functions to manipulate singly linked lists.
- **Error Handling**: Clear error codes for common issues (e.g., null pointers, corrupted lists).
- **Memory Management**: Ability to manage dynamic memory safely with optional destruction of node data.
- **Flexible Matching**: Custom comparison function support for search operations.
- **Corruption Detection**: Automatically detects if the list structure is corrupted and prevents further manipulation (*partially implemented*).

## Installation :hammer_and_wrench:

To use the data structure, simply download or clone this repository, then include the header file in your project:

```bash
git clone https://github.com/paul-green-stone/Cds.git
```

Include the header in your C source files:

```C
#include "Cds/include/SinglyList.h"
```

Link your program with the source files:

```bash
gcc -o output main.c source/SinglyList.c
```

## Usage :gear:

### Initializing a List

To create a new list, initialize it using the `sList_init` function. You can provide custom functions to handle data destruction and matching:

```C
sList list;
/* There is no default match function for the given list  yet */
sList_init(&list, NULL, NULL);
```

### Writing the `compare` function for the list

Let's assume we store integers in the list, then the function for comparing them would be something like this:

```C
int intcmp(const void* key1, const void* key2) {
    return *((int*) a) - *((int*) b) == 0 ? 1 : *((int*) a) - *((int*) b);
}
```

<div style="background-color: #e3f2fd; border-left: 4px solid #2196F3; padding: 1em; margin: 1em 0;"> 💡 The <b>compare</b> function receives the data stored directly in each list node as <b>key1</b>. The list implementation automatically extracts and passes this data to your function during operations like searching — no manual data access needed. </div>


Then the initialization would look like this:

```C
sList_init(&list, NULL, intcmp);
```

### Basic Usage

#### Insert Data

* **Insert at the tail**:

```C
/* Storing 90 at the end of the list */
sList_insert_last(&myList, &(int){90});
```

* **Insert at the head**:

```C
/* Storing 33 at the head of the list */
sList_insert_first(&myList, &(int){33});
```

#### Remove Data

* **Remove from the tail**:

```C
/* Assuming working with the same list, the return data would be a pointer to 90 */
void* data = sList_remove_last(&myList);
```

* **Remove from the head**:

```C
/* Assuming working with the same list, the return data would be a pointer to 33 */
void* data = sList_remove_first(&myList);
```

#### Find Data

```C
const sNode* node = sList_find(&list, &(int){90}, NULL);
```

And then you can extract data with the function `sNode_data`:

```C
if (node) { printf("Found: %d\n", *((int*) sNode_data(node))); }
```

### API Documentation :books:

```c
sListResult sList_init(sList* list, void (*destroy)(void* data), int (*match)(const void* key1, const void* key2));
```

Initializes a new list. The `destroy` function is called when the list is destroyed, and `match` is used to compare data during search operations.

```C
sListResult sList_destroy(sList* list);
```

Destroys the list and frees memory associated with its nodes, calling the `destroy` function on each node's data if set.

```C
sListResult sList_insert_last(sList* list, void* data);
```

Inserts a new node with the provided `data` at the end of the `list`.

```C
sListResult sList_insert_first(sList* list, void* data);
```

Inserts a new node with the provided `data` at the beginning of the `list`.

```C
void* sList_remove_last(sList* list);
```

Removes and returns the last node's data in the `list`.

```C
void* sList_remove_first(sList* list);
```

Removes and returns the first node's data in the `list`.

```C
const sNode* sList_find(const sList* list, const void* data, int (*match)(const void* key1, const void* key2));
```

Searches the list for a node that matches the given data using the provided `match` function.

```C
void* sList_remove(sList* list, sNode* node);
```

Removes the specified `node` from the `list`.

```C
sListResult sList_insert_after(sList* list, sNode* node, void* data);
```

Inserts a new node after the specified `node`.

```C
sListResult sList_insert_before(sList* list, sNode* node, void* data);
```

Inserts a new node before the specified `node`.

### Error Handling :x:

The library uses an error handling system based on the `sListResult` enum. Common error codes include:

- `SLIST_OK`: Operation successful.
- `SLIST_ERR_NULL_LIST`: The provided list is NULL.
- `SLIST_ERR_NULL_NODE`: The provided node is NULL.
- `SLIST_ERR_NULL_DATA`: The provided data is NULL.
- `SLIST_ERR_NO_MEMORY`: Memory allocation failed.
- `SLIST_ERR_CORRUPTED`: The list structure is corrupted.
- `SLIST_ERR_EMPTY`: The list is empty.
- `SLIST_ERR_FOREIGN_NODE`: The node does not belong to the list.
- `SLIST_ERR_MISSING`: The list does not have a node with the given data.

### Example :computer:

And now let's write something more practical for everyday use! First of all we define a book:

```C
typedef struct book {

    const char* title;
    const char* author;
    const char* isbn10;
} Book;
```

and write a function that allocates memory for a book and initializes it:

```C
Book* create_book(const char* title, const char* author, const char* isbn10) {

    Book* book = malloc(sizeof(Book));

    book->title = title;
    book->author = author;
    book->isbn10 = isbn10;

    return book;
};
```

Before we're ready to supply our list with books, we need to tell the list how to compare them in order to find the one we're interested in:

```C
int cmp_by_ISBN10(const void* book, const void* isbn) {

    const Book* _book = (const Book*) book;
    const char* _isbn = isbn;

    int result = strcmp(_book->isbn10, _isbn);
    /* ======== */
    return result == 0 ? 1 : result;
}
```

The function above is straightforward - it simply compares books' ISBNs to find the one we want. Now we are ready to add some books into the list!

```C
sList books;
sList_init(&books, free, cmp_book);

/* Adding some books into the list */
sList_insert_first(&books, create_book(
        "Mastering Algorithms with C Useful Techniques from Sorting to Encryption",
        "Kyle Loudon",
        "1565924533"
    ));

    sList_insert_first(&books, create_book(
        "Game Engine Architecture",
        "Jason Gregory",
        "1138035459"
    ));
```

we can print the book's information by creating a simple function, such as this:

```C
void print_book(const Book* book) {
    printf("Author: %s\n", book->author);
    printf("Title: %s\n", book->title);
    printf("ISBN10: %s\n", book->isbn10);
}
```

and show the content of the list in the following manner:

```C
for (sNode* i = books.head; i != NULL; i = (sNode*) sNode_next(i)) {
    
    /* The finction extract the node's data */
    print_book(sNode_data(i));

    if (sNode_next(i) != NULL) printf("\n");
}
```

In order to find a book by its ISBN10, we can call `sList_find`, which returns a node that contains the data we need:

```C
/* Calling `sNode_data` extracts the data from the returned node */
Book* b = sNode_data(sList_find(&list, "1138035459", NULL));

if (b) {
    printf("\nThe title of the book we found: %s\n", b->title);
}
```

If you don't want to find a book by its ISBN10 — for example — you can supply another comparison function to `sList_find`, which will use it for that search only:

```C
int cmp_by_author(const void* book, const void* author) {

    const Book* _book = book;
    const char* _author = author;
    
    int result = strcmp(_book->author, _author);
    /* ======== */
    return result == 0 ? 1 : result;
}

Book* b = sNode_data(sList_find(&list, "Jason Gregory", cmp_by_author));

if (b) {
    printf("\nThe title of the book we found: %s\n", b->title);
}
```

:bulb: With a few tweaks, you can modify comparison functions as needed. For example, in the call above, we use the author's name to find a book. The book's author field must precisely match the search term. In the comparison function, you can preprocess the search parameter — stripping whitespace or capitalizing letters — as needed. It's up to your requirements.

### Contribute

For contribution guidelines, check the [Contribute section](../../README.md#contribute).
