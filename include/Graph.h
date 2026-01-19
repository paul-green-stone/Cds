#ifndef GRAPH_H
#define GRAPH_H

#include <sys/types.h>

#include "SinglyList.h"
#include "Set.h"

/* Define a structure for adjacency lists */
typedef struct vertex {

    void* data;
    /* A set of adjacent vertices */
    Set vertices;
} Vertex;

/* Define a structure for graphs */
typedef struct graph {

    /* A list of all the vertices in the graph */
    sList vertices;
    
    void* _info;
    int (*match)(const void* key1, const void* key2);
    void (*destroy)(void* data);
} Graph;

/* Define colors for vertices in graphs */
typedef enum {whie, gray, black} VertexColor;

/**
 * Macro that evaluates to the list of adjacency-list structures in `graph`.
 * 
 * Each element in the list is a `Vertex` structure. The actual list of
 * adjacency-list structures in the graph is returned, so it must not be
 * manipulated by the caller.
 * 
 * @param graph
 * 
 * @return 
 */
#define adjlists(graph) ((graph)->adjlists)

/**
 * Initializes the graph specified by `graph`. This operation must
 * be called for a graph before the graph can be used with any
 * other operation.
 * 
 * The `match` argument is a function used by various graph operations
 * to determine if two vertices match. It should return `1` if `key1`
 * is equal to `key2`, and `0` otherwise.
 * 
 * The `destroy` argument provides a way to free dynamically allocated
 * data when `Graph_destroy` is called. For example, if the graph contains
 * data dynamically allocated using `malloc`, `destroy` should be set to
 * `free` to free the data as the graph is destroyed. For structured data
 * containing several dynamically allocated members, `destroy` should be
 * set to a user-defined function that calls `free` for each dynamically
 * allocated member as well as for the structure itself. For a graph
 * containing data that should not be freed, destroy should be set to `NULL`.
 * 
 * @param graph     Pointer to the graph to initialize. Must not be `NULL`.
 * @param match     Function used to compare two vertex keys.
 * @param destroy   Function used to free vertex data, or `NULL` if no
 *                  deallocation is required.
 * 
 * @return Returns `0` on successful initialization. Returns `-1` if
 * `graph` is `NULL`.
 */ 
int Graph_init(Graph* graph, int (*match)(const void* key1, const void* key2), void (*destoy)(void* data));

/**
 * Destroys the graph specified by `graph`. No other operations are
 * permitted after calling `Graph_destroy` unless `Graph_init`
 * is called again.
 * 
 * The `Graph_destroy` operation removes all vertices and edges from
 * a graph and calls the function passed as `destroy` to `Graph_init`
 * once for each vertex or edge as it is removed, provided destroy
 * was not set to `NULL`.
 * 
 * @param graph Pointer to the graph to destroy.
 * 
 * @return Returns `0` on successful initialization. Returns `-1` if
 * `graph` is `NULL`.
 */
int Graph_destroy(Graph* graph);

/**
 * Inserts a vertex into the graph specified by `graph`. The new
 * vertex contains a pointer to `data`, so the memory referenced
 * by `data` should remain valid as long as the vertex remains in
 * the graph.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data`.
 * 
 * @param graph Pointer to the graph into which the vertex is inserted.
 *              The graph must have been previously initialized.
 * @param data  Pointer to the data to associate with the new vertex.
 *              This pointer is stored directly in the graph and is
 *              not copied
 * 
 * @return 
 */
int Graph_add_V(Graph* graph, const void* data);

/**
 * Inserts an edge from the vertex specified by `data1` to the
 * vertex specified by `data2` in the graph specified by `graph`.
 * Both vertices must have been inserted previously using `Graph_add_V`.
 * 
 * The new edge is represented with a pointer to `data2 in the adjacency
 * list of the vertex specified by `data1`, so the memory referenced by 
 * `data2` should remain valid as long as the edge remains in the graph.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with `data2`.
 * 
 * To enter an edge `(data1, data2)` in an undirected graph, call this
 * operation twice: once to insert an edge from `data1` to `data2`,
 * and again to insert the implied edge from `data2` to `data1`.
 * 
 * @param graph Pointer to the graph in which the edge is inserted.
 *              The graph must have been previously initialized.
 * @param data1 Pointer to the data identifying the source vertex.
 *              This pointer must match the data used when the vertex
 *              was inserted.
 * @param data2 Pointer to the data identifying the destination vertex.
 *              This pointer is stored directly in the adjacency list.
 */
int Graph_add_E(Graph* graph, const void* data1, const void* data2);

/**
 * Removes the vertex matching `data` from the graph specified by `graph`.
 * All edges incident to and from the vertex must have been removed
 * previously using `Graph_del_E`.
 * 
 * Upon return, data points to the data stored in the vertex that was removed.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with the data.
 * 
 * @param graph
 * @param data
 * 
 * @return 
 */
int Graph_del_V(Graph* graph, void** data);

/**
 * Removes the edge from `data1` to `data2` in the graph specified by `graph`.
 * 
 * Upon return, `data2` points to the data stored in the adjacency list of the
 * vertex specified by `data1`.
 * 
 * It is the responsibility of the caller to manage the storage
 * associated with the data.
 * 
 * @param graph
 * @param data1
 * @param data2
 *
 * @return
 */
int Graph_del_E(Graph* graph, const void* data1, const void** data2);

/**
 * Retrieves vertices that are adjacent to the vertex specified by `data`
 * in `graph`. The adjacent vertices are returned in the form of a
 * `Vertex` structure, a structure containing the vertex matching
 * `data` and a set of vertices adjacent to it.
 * 
 * A pointer to the actual adjacency list in the graph is returned,
 * so it must not be manipulated by the caller.
 * 
 * @param graph
 * @param data1
 * @param list
 * 
 * @return
 */
int Graph_get_adjl(const Graph* graph, const void* data1, Vertex** list);

/**
 * Determines whether the vertex specified by `data2` is adjacent to
 * the vertex specified by `data1` in `graph`.
 * 
 * @param graph
 * @param data1
 * @param data2
 * 
 * @return
 */
int Graph_is_adjacent(const Graph* graph, const void* data1, const void* data2);

/**
 * 
 */
ssize_t Graph_vertices(const Graph* graph);

/**
 * 
 */
ssize_t Graph_edges(const Graph* graph);

#endif /* GRAPH_H */