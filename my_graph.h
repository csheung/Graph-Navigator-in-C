// =================== Support Code =================
// Graph.
//
// - Implement each of the functions to create a working graph.
// - Do not change any of the function declarations
//   - (i.e. graph_t* create_graph() should not have additional arguments)
// - You should not have any 'printf' statements in your graph functions.
//   - (You may consider using these printf statements to debug, but they 
//      should be removed from your final version)
// ==================================================
#ifndef MYGRAPH_H
#define MYGRAPH_H

#include "my_dll.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

// Create a graph data structure
typedef struct graph {
    int numNodes;
    int numEdges;
    dll_t* nodes;   // a list of nodes storing all of our nodes

    // bookkeeping for Dikjstra's Algorithm
    // -1 indicates that there is no shortest-path data in the graph
    // it should be reset whenever a node or edge is added or removed 
    // from the graph
    int source;
} graph_t;

typedef struct graph_node {
    // per-node bookkeping for Dikjstra's Algorithm
    int data;
    int cost;
    int from;
    int visited;
    // per-node neighbor information which now stores edges instead of nodes
    dll_t* inNeighbors;
    dll_t* outNeighbors;
} graph_node_t;

typedef struct graph_edge {
    int from;
    int to;
    int cost;
} graph_edge_t;

// Creates a graph
// Returns a pointer to a newly created graph.
// The graph should be initialized with data on the heap.
// The graph fields should also be initialized to default values.
// Returns NULL if we cannot allocate memory.
graph_t* create_graph(){
    // Modify the body of this function as needed.
    graph_t* myGraph= malloc(sizeof(graph_t));
    if (myGraph == NULL) {
	return NULL;
    }
    myGraph->nodes = create_dll();
    myGraph->numEdges = 0;
    myGraph->numNodes = 0;
    myGraph->source = -1;
    return myGraph;
}

// Returns the node pointer if the node exists.
// Returns NULL if the node doesn't exist or the graph is NULL
graph_node_t* find_node( graph_t * g, int value){
    if (g == NULL) {
	return NULL;
    }
    
    node_t* temp = g->nodes->head;
    if (temp == NULL) {
	return NULL;
    }

    while (temp != NULL) {
	graph_node_t* cur = temp->data;
	if (cur->data == value) {
	    return cur;
	}
	temp = temp->next;
    }
    // return NULL when not found
    return NULL;
}

// helper function
// find node_t* pointer from the dll of graph_t
node_t* find_dll_node(graph_t* g, int target) {
    if (g == NULL) {
	return NULL;
    }

    node_t* dummy = g->nodes->head;
    while (dummy != NULL) {
    	graph_node_t* temp = dummy->data;
	if (temp->data == target) {
   	    return dummy;
	}
	dummy = dummy->next;
    }
    return NULL;
}

// Creates a graph node
// Note: This relies on your dll implementation.
graph_node_t * create_graph_node(int value){
    graph_node_t * graph_node = malloc(sizeof(graph_node_t));
    if ( graph_node == NULL ) return NULL;
    
    graph_node->data = value;
    graph_node->cost = INFINITY;
    graph_node->from = INT_MIN;
    graph_node->visited = 0;
    graph_node->inNeighbors = create_dll();
    graph_node->outNeighbors = create_dll();
    
    return graph_node;
}

// helper function refresh the 'visited' variable to 0
// // refresh the 'visited' identifier to 0 after using
int refresh_visited_nodes(graph_t* g) {
    if (g == NULL) {
	return -1;
    }

    node_t* dummy = g->nodes->head;
    while (dummy != NULL) {
	graph_node_t* temp = dummy->data;
	temp->visited = 0;
	dummy = dummy->next;
    }
    return 1;
}

// Create a graph_edge
// Returns a pointer to a newly created graph edge
graph_edge_t* create_graph_edge(int from, int to, int cost) {
    graph_edge_t* myEdge = malloc(sizeof(graph_edge_t));
    if (myEdge == NULL) {
	return NULL;
    }
    myEdge->from = from;
    myEdge->to = to;
    myEdge->cost = cost;
    return myEdge;
} 

// helper function to debug
// print specified graph dll out
int graph_print(graph_t* g) {
	if (g == NULL) {
		return -1;
	}

	node_t* dummy = g->nodes->head;
	while (dummy != NULL) {
	    graph_node_t* gn = dummy->data;
	    printf("%d ", gn->data);
	    dummy = dummy->next;
	}
	return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node already exists )
// Returns -1 if the graph is NULL.
int graph_add_node(graph_t* g, int value){
    if ( g == NULL ) return -1;
    
    if (find_node(g, value) != NULL) return -1;
    
    graph_node_t * newNode = create_graph_node(value);
    if ( newNode == NULL ) return -1;
    
    assert(g->nodes);
    dll_push_back(g->nodes, newNode);
    g->numNodes++;
    
    return 1;
}

// remove the edge from all inNeighbors
int graph_remove_from_inNeighbors(graph_t* g, graph_node_t* gn) {
    if (g == NULL || gn == NULL) {
	return -1;
    }

    // go inside the list of inNeighbors for outer loop 
    node_t* neigh = gn->inNeighbors->head;
    while (neigh != NULL) {
	graph_edge_t* edge = neigh->data;
	graph_node_t* gnn = find_node(g, edge->from);
	if (gnn == NULL) {
	    break;
	}
	node_t* node = gnn->outNeighbors->head;


	int i;
	for (i = 0; i < dll_size(gnn->outNeighbors); i++) {
	    graph_edge_t* ee = node->data;
	    if (ee->to == gn->data) {
		free(ee);
		dll_remove(gnn->outNeighbors, i);
		g->numEdges--;
		break;
	    }
	    node = node->next;
	}
	neigh = neigh->next;
    }
    return 1;
}

// remove the edge from all outNeighbors
int graph_remove_from_outNeighbors(graph_t* g, graph_node_t* gn) {
    if (g == NULL || gn == NULL) {
	return -1;
    }

    // go inside the list of inNeighbors for outer loop 
    node_t* neigh = gn->outNeighbors->head;
    while (neigh != NULL) {
	graph_edge_t* edge = neigh->data;
	graph_node_t* gnn = find_node(g, edge->to);
	if (gnn == NULL) {
	    break;
	}
	node_t* node = gnn->inNeighbors->head;
	
	int i;
	for (i = 0; i < dll_size(gnn->inNeighbors); i++) {
	    graph_edge_t* ee = node->data;
	    if (ee->from == gn->data) {
		free(ee);
		dll_remove(gnn->inNeighbors, i);
		g->numEdges--;
		break;
	    }
	    node = node->next;
	}
	neigh = neigh->next;
    }
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node doesn't exist )
// Returns -1 if the graph is NULL.
int graph_remove_node(graph_t* g, int value){
    // The function removes the node from the graph along with any edges associated with it.
    // That is, this node would have to be removed from all the in and out neighbor's lists that contain it.
    if (g == NULL) {
	return -1;
    }
    // locate node address first before memory free
    node_t* dummy = find_dll_node(g, value);
    // locate the graph node of given value
    graph_node_t* gn = find_node(g, value);
    if (dummy == NULL || gn == NULL) {
	return 0;
    }

    assert(gn);

    graph_remove_from_inNeighbors(g, gn);
    graph_remove_from_outNeighbors(g, gn);

    // free memories of graph node
    free_dll(gn->inNeighbors);
    free_dll(gn->outNeighbors);
    free(gn);

    // remove node from g->nodes after freeing its internal graph node data
    int i;
    node_t* temp = g->nodes->head;
    for (i = 0; i < dll_size(g->nodes); i++) {
	if (temp == dummy) {
	    dll_remove(g->nodes, i);
	    break;
	}
	temp = temp->next;
    }
    // subtract one from number of nodes in graph
    g->numNodes--;

    return 1;
}

/*
// DLL find graph node
// Check if graph node item exists in a DLL (e.g. DLL of In/OutNeighbors)
// Returns -1 if the dll is NULL
// Returns 1 if true (The node exists in the DLL)
// Returns 0 if false (The node does not exist)
int dll_find_item(dll_t* l, int item) {
    	if (l == NULL) {
		return -1;
	}

	// traverse dll to check if item is inside
	node_t* dummy = l->head;
	while (dummy != NULL) {
	    graph_node_t* temp = dummy->data;
	    if (temp->data == item) {
		return 1;
	    }
	    dummy = dummy->next;
	}
	return 0;
}
*/

// DLL finds graph edge
// Check if graph edge already exists
// returns NULL if DLL is NULL
// returns NULL if edge cannot be found
// returns graph edge struct if edge exists
graph_edge_t* find_edge(dll_t* l, int source, int dest) {
    if (l == NULL) {
	return NULL;
    }

    node_t* n = l->head;
    while (n != NULL) {
        graph_edge_t* e = n->data;
        if (e != NULL) {
	    if (e->from == source && e->to == dest) {
	        return e;
	    }
	}
	n = n->next;
    }
    return NULL;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge already exists )
// Returns -1 if the graph is NULL.
int graph_add_edge(graph_t * g, int source, int destination, int cost){
    // The function adds an edge from source to destination but not the other way.
    // Make sure you are not adding the same edge multiple times.
    // Make sure you modify the in and out neighbors appropriatelly.
    // Destination will be an out neighbor of source.
    // Source will be an in neighbor of destination.
    if (g == NULL) {
	return -1;
    }

    // locate the addresses of graph nodes
    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, destination);
    if (source_gn == NULL || dest_gn == NULL) {
	return 0;
    }

    if (find_edge(source_gn->outNeighbors, source, destination) != NULL || find_edge(dest_gn->inNeighbors, source, destination) != NULL) {
	return 0;
    }

    // create new edge
    graph_edge_t* newEdge = create_graph_edge(source, destination, cost);
    if (newEdge == NULL) {
	return 0;
    }

    // push into source_gn's outNeighbors and dest_gn's inNeighbors
    dll_push_back(source_gn->outNeighbors, newEdge);
    dll_push_back(dest_gn->inNeighbors, newEdge);

    // add one to number of edges in graph
    g->numEdges++;

    return 1;
}

// DLL removes graph edge
// returns -1 if DLL is NULL
// returns 0 if edge cannot be found
// returns 1 if edge is removed
int remove_edge_from_neighbors(dll_t* l, dll_t* ll, int source, int dest) {
    if (l == NULL) {
	return -1;
    }

    node_t* n = l->head;
    int i;
    for (i = 0; i < dll_size(l); i++) {
        graph_edge_t* e = n->data;
        if (e != NULL) {
	    if (e->from == source && e->to == dest) {
		dll_remove(l, i);
		return 1;
	    }
	}
	n = n->next;
    }

    n = ll->head;
    for (i = 0; i < dll_size(ll); i++) {
        graph_edge_t* ee = n->data;
        if (ee != NULL) {
	    if (ee->from == source && ee->to == dest) {
		dll_remove(l, i);
		return 1;
	    }
	}
	n = n->next;
    }

    return 0;
}


// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge doesn't exists )
// Returns -1 if the graph is NULL.
int graph_remove_edge(graph_t * g, int source, int destination){
    // TODO: Implement me!
    //The function removes an edge from source to destination but not the other way.
    //Make sure you remove destination from the out neighbors of source.
    //Make sure you remove source from the in neighbors of destination.
    if (g == NULL) {
	return -1;
    }

    // locate graph nodes in the graph
    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, destination);
    if (source_gn == NULL || dest_gn == NULL) {
	return 0;
    }

    // ensure the edge exists
    if (find_edge(source_gn->outNeighbors, source, destination) == NULL || find_edge(dest_gn->inNeighbors, source, destination) == NULL) {
	return 0;
    }

    // locate and save the address of target edge first
    graph_edge_t* edge = find_edge(source_gn->outNeighbors, source, destination);
    // remove memories from the neighbor lists of graph nodes
    remove_edge_from_neighbors(source_gn->outNeighbors, dest_gn->inNeighbors, source, destination);

    // free memory of target edge
    free(edge);

    // subtract one from number of edges
    g->numEdges--;
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist )
// Returns -1 if the graph is NULL.
int contains_edge(graph_t * g, int source, int destination){
    if (g == NULL) {
	return -1;
    }

    // locate the addresses of graph nodes
    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, destination);
    if (source_gn == NULL || dest_gn == NULL) {
	return 0;
    }

    // check if the edge exists
    // apply self-defined 'find_edge' function
    if (find_edge(source_gn->outNeighbors, source, destination) == NULL || find_edge(dest_gn->inNeighbors, source, destination) == NULL) {
	return 0;
    }
    return 1;
}

// Returns a pointer to the edge
// Returns NULL on failure (or if the source or destination nodes don't exist)
// Exits if the graph is NULL (only use this function if the graph `contains_edge` is true)
graph_edge_t* graph_get_edge(graph_t * g, int source, int destination) {
    if (g == NULL || contains_edge(g, source, destination) != 1) {
	return NULL;
    }

    // locate the addresses of graph nodes
    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, destination);
    if (source_gn == NULL || dest_gn == NULL) return NULL;

    // traverse to compare given value and data of each edge
    node_t* n = source_gn->outNeighbors->head;
    while (n != NULL) {
	graph_edge_t* edge = n->data;
	if (edge->from == source && edge->to == destination) {
	    return edge;
	}
	n = n->next;
    }
    return NULL;
}

// Returns dll_t* of all the in neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getInNeighbors( graph_t * g, int value ){
    if (g == NULL || find_dll_node(g, value) == NULL) {
	return NULL;
    }

    graph_node_t* dummy = find_node(g, value);
    if (dummy == NULL) {
	return NULL;
    }

    return dummy->inNeighbors;
}

// Returns the number of in neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumInNeighbors( graph_t * g, int value){
    if (g == NULL || find_dll_node(g,value) == NULL || find_node(g,value) == NULL) {
	return -1;
    }

    dll_t* dll_inNeighbors = getInNeighbors(g, value);
    if (dll_inNeighbors == NULL) {
	return 0;
    }

    return dll_size(dll_inNeighbors);
}

// Returns dll_t* of all the out neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getOutNeighbors( graph_t * g, int value ){
    if (g == NULL || find_dll_node(g,value) == NULL) {
	return NULL;
    }

    graph_node_t* dummy = find_node(g,value);
    if (dummy == NULL) {
	return NULL;
    }

    return dummy->outNeighbors; 
}

// Returns the number of out neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumOutNeighbors( graph_t * g, int value){
     if (g == NULL || find_dll_node(g,value) == NULL || find_node(g,value) == NULL) {
	return -1;
    }

    dll_t* dll_outNeighbors = getOutNeighbors(g, value);
    if (dll_outNeighbors == NULL) {
	return 0;
    }

    return dll_size(dll_outNeighbors); 
}

// returns 1 if we can reach the destination from source
// returns 0 if it is not reachable
// returns -1 if the graph is NULL (using BFS)
int graph_is_reachable(graph_t * g, int source, int dest){
    if (g == NULL) {
	return -1;
    }

    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, dest);
    if (source_gn == NULL || dest_gn == NULL) {
	return -1;
    }

    // access the head node of outNeighbors of source graph node
    node_t* dummy = source_gn->outNeighbors->head;
    if (dummy == NULL) {
	return 0;
    }

    // create a DLL to record the traversed node 
    // to help explore their outNeighbors afterward (BFS)
    dll_t* nodeList = create_dll();
    while (dummy != NULL) {
	graph_edge_t* edge = dummy->data;
	graph_node_t* current = find_node(g, edge->to);

	// continue to next item if the node has already been visited
	if (current->visited == 1) {
	    dummy = dummy->next;
	    continue;
	}

	// push to the back of our record DLL after knowing it is not visited
	dll_push_back(nodeList, current);

	// free and refresh before returning after locating destination
	if (current == dest_gn) {
	    free_dll(nodeList);
	    refresh_visited_nodes(g);
	    return 1;
	}
	// mark current node as 'visited' and move on to the next
	current->visited = 1;
	dummy = dummy->next;

	// go into outneighbors' outneighbors to keep searching
	// pop out from the nodeList to find the next graph node recorded
	while (dummy == NULL) {
	    node_t* tempNode = nodeList->head;
	    if (tempNode == NULL) {
		free_dll(nodeList);
		refresh_visited_nodes(g);
		return 0;
	    }
	    graph_node_t* tempGN = tempNode->data;
	    // reassign dummy to start another round of iteration
	    dummy = tempGN->outNeighbors->head;
	    dll_pop_front(nodeList);
	}
    }
    free_dll(nodeList);
    refresh_visited_nodes(g);
    return 0;
}

// DFS helper function to explore each potential path from each node in the graph
// returns 1 if cycle is found
// returns 0 if no cycles could be found
// returns -1 if the graph is NULL
int dfs_find_cycle(graph_t* g, graph_node_t* gn) {
    if (gn == NULL) {
	return -1;
    }

    // cycle is found when reaching visited node
    if (gn->visited == 1) {
	return 1;
    }

    node_t* n = gn->outNeighbors->head;
    while (n != NULL) {
	graph_edge_t* edge = n->data;
	graph_node_t* gnn = find_node(g, edge->to);

	// check for self-cycle
	// and dfs to go deep by exploring lists of outNeighbors
	if (gn == gnn || dfs_find_cycle(g, gnn) == 1) {
	    refresh_visited_nodes(g);
	    return 1;
	}
	// mark node as visited after dfs
	gnn->visited = 1;
	n = n->next;
    }
    refresh_visited_nodes(g);
    return 0;
}

// returns 1 if there is a cycle in the graph
// returns 0 if no cycles exist in the graph
// returns -1 if the graph is NULL 
int graph_has_cycle(graph_t * g){
    if (g == NULL) {
	return -1;
    }

    node_t* n = g->nodes->head;
    while (n != NULL) {
	graph_node_t* gn = n->data;
	if (dfs_find_cycle(g, gn) == 1) {
	    return 1;
	}
	n = n->next;
    }
    return 0;
}

// DFS helper function to find path
// returns 1 if any path 
// returns 0 if no path exists
// returns -1 if the graph is NULL
int dfs_find_path(graph_t* g, dll_t* path, graph_node_t* gn, graph_node_t* dest) {
    if (g == NULL) {
	return -1;
    }

    // end this path search when entering a cycle
    if (gn->visited == 1) {
	return 0;
    }

    if (gn == dest) {
	// push and record current graph node into the path dll
	// refresh all nodes 'visited' variable to 0
	dll_push_back(path, gn);
	refresh_visited_nodes(g);
	return 1;
    }

    // mark current graph node as 'visited' to continue
    gn->visited = 1;

    // access the outNeighbors of current graph node
    node_t* n = gn->outNeighbors->head;
    if (n == NULL) {
	return 0;
    }

    int i;
    for (i = 0; i < dll_size(gn->outNeighbors); i++) {
	graph_edge_t* edge = n->data;
	graph_node_t* gnn = find_node(g, edge->to);

	// go deep into each neighbor graph node to see if path exists
	if (dfs_find_path(g, path, gnn, dest) == 1) {
	    dll_push_front(path, gn);
	    return 1;
	}
	n = n->next;
    }
    // node unfound then refresh all 'visited' node and return 0
    refresh_visited_nodes(g);
    return 0;
}

// prints any path from source to destination if there 
// exists a path from the source to the destination.
// Returns 1 if there is a path from source to destination
// Returns 0 if there is not a path from a source to destination
// Returns -1 if the graph is NULL
int graph_print_path(graph_t * g, int source, int dest){
    if (g == NULL) {
	return -1;
    }

    if (graph_is_reachable(g, source, dest) != 1) {
	return 0;
    }

    // locate the graph_node_t of the source and destination
    graph_node_t* source_gn = find_node(g, source);
    graph_node_t* dest_gn = find_node(g, dest);
    if (source_gn == NULL || dest_gn == NULL) {
	return -1;
    }

    // malloc a path dll to record the node during search
    dll_t* path = create_dll();
    // dfs helper function to explore every potential path from lists of outNeighbors
    dfs_find_path(g, path, source_gn, dest_gn);

    // print data following the order of path DLL
    node_t* dummy = path->head;
    printf("The Path is:\n");
    while (dummy != NULL) {
	graph_node_t* temp = dummy->data;
	printf("%d ", temp->data);
	dummy = dummy->next;
    }
    printf("\n");
    // free memories of path DLL and node_t inside 
    free_dll(path);
    return 1;
}

// Returns the number of nodes in the graph
// Returns -1 if the graph is NULL.
int graph_num_nodes(graph_t* g){
    if (g == NULL) {
	return -1;
    }
    return g->numNodes;
}

// Returns the number of edges in the graph,
// Returns -1 on if the graph is NULL
int graph_num_edges(graph_t* g){
    if (g == NULL) {
	return -1;
    }
    return g->numEdges;
}

// Free graph
// Removes a graph and ALL of its elements from memory.
// This should be called before the program terminates.
// Make sure you free all the dll's too.
void free_graph(graph_t* g){
    // TODO: Implement me!
    if (g == NULL) {
	return;
    }

    node_t* dummy = g->nodes->head;
    graph_node_t* temp;
    while (dummy != NULL) {
	temp = dummy->data;

	// free edges only using outNeighbors side
	node_t* n = temp->outNeighbors->head;
	while (n != NULL) {
	    graph_edge_t* e = n->data;
	    n = n->next;
	    free(e);
	}
	
	// free its lists of inNeighbors and outNeighbors
	free_dll(temp->inNeighbors);
	free_dll(temp->outNeighbors);

	// free the graph node itself
	free(temp);

	// free the next node until NULL
	dummy = dummy->next;
    }
    // free the dll for all nodes and the graph struct itself
    free_dll(g->nodes);
    free(g);
}

#endif
