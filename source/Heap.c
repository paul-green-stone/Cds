#include <stdlib.h>
#include <string.h>

#include "../include/Heap.h"

#define heap_parent(npos) ((int) (((npos) - 1) / 2))
#define heap_left(npos) (((npos) * 2) + 1)
#define heap_right(npos) (((npos) * 2) + 2)

void Heap_init(Heap* heap, int (*compare)(const void* key1, const void* key2), void (*destroy)(void* data)) {

    heap->size = 0;
    heap->compare = compare;
    heap->destroy = destroy;
    heap->tree = NULL;
}

void Heap_destroy(Heap* heap) {

    if (heap->destroy != NULL) {

        for (size_t i = 0; i < heap->size; i++) {
            heap->destroy(heap->tree[i]);
        }
    }

    free(heap->tree);
    memset(heap, 0, sizeof(Heap));
}

int Heap_insert(Heap* heap, const void* data) {

    void* temp;
    int ipos, ppos;
    /* ======== */

    if ((temp = (void**) realloc(heap->tree, (heap->size + 1) * sizeof(void*))) == NULL) {
        return -1;
    }
    else {
        heap->tree = temp;
    }

    heap->tree[heap->size] = (void*) data;

    /* Heapify the tree by pushing the contents of the new node upward */
    ipos = heap->size;
    ppos = heap_parent(ipos);

    while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0) {

        /* Swap the contents of the current node and its parent */
        temp = heap->tree[ppos];
        heap->tree[ppos] = heap->tree[ipos];
        heap->tree[ipos] = temp;

        /* Move up one level in the tree to continue heapifying */
        ipos = ppos;
        ppos = heap_parent(ipos);
    }

    heap->size++;

    /* ======== */
    return 0;
}

void* Heap_extract(Heap* heap) {

    void* save, *temp, *data;
    int ipos, lpos, rpos, mpos;
    /* ======== */

    if (heap->size == 0) { return NULL; }

    /* Extract the node at the top of the heap */
    data = heap->tree[0];

    save = heap->tree[heap->size - 1];

    if (heap->size - 1 > 0) {
        if ((temp = (void**) realloc(heap->tree, (heap->size - 1) * sizeof(void*))) == NULL) { return NULL; }
        else { heap->tree = temp; }

        heap->size--;
    }
    else {
        
        free(heap->tree);
        heap->tree = NULL;
        heap->size = 0;
        /* ======== */
        return data;
    }

    /* Copy the last node to the top */
    heap->tree[0] = save;

    /* Heapify the tree by pushing the contents of the new top downward */
    ipos = 0;
    lpos = heap_left(ipos);
    rpos = heap_right(ipos);

    while (1) {

        lpos = heap_left(ipos);
        rpos = heap_right(ipos);

        if (lpos < heap->size && heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0) {
            mpos = lpos;
        }
        else {
            mpos = ipos;
        }

        if (rpos < heap->size && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0) {
            mpos = rpos;
        }

        if (mpos == ipos) { break ; }
        else {

            temp = heap->tree[mpos];
            heap->tree[mpos] = heap->tree[ipos];
            heap->tree[ipos] = temp;

            ipos = mpos;
        }
    }

    /* ======== */
    return data;
}