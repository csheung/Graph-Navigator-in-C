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
#ifndef MYSHORTEST_H
#define MYSHORTEST_H

#include <stdlib.h>
#include <assert.h>
#include <math.h>

// Our library that we have written.
// Also, by a really smart engineer!
#include "my_graph.h"
#include "my_pq.h"
// Note that we are locating this file
// within the same directory, so we use quotations
// and provide the path to this file which is within
// our current directory.

// helper function to ensure positive weight edges
// returns -1 if graph node is NULL
// returns 0 if negative weight edges exist
// returns 1 if all edges from the graph node are positively weighted
int check_edges_weight(graph_node_t* gn) {
    if (gn == NULL) {
	return -1;
    }

    node_t* n = gn->outNeighbors->head;
    while (n != NULL) {
	graph_edge_t* e = n->data;
	if (e->cost < 0) {
	    return 0;
	}
	n = n->next;
    }
    return 1;
}

// returns 1 if Dijkstra's algorithm completes successfully
// returns 0 if Dikjstra's algorithm does not complete successfully
// NOTE: Recall from our discussion, Dikjstra's algorithm does not
//       handle negative weight edges
// returns -1 if the graph is NULL
int calculate_shortest_path(graph_t * g, int source) {
	// TODO: Implement me!
	if (g == NULL) {
	    return -1;
	}

	if (g->source != -1) return 0;

	// check if there are any negative weighted nodes or edges
	node_t* n = g->nodes->head;
	// create a priority queue to contain the graph nodes
	queue_t* q = create_queue(g->numNodes);
	if (n == NULL || q == NULL) {
	    return 0;
	}
	while (n != NULL) {
	    graph_node_t* gn = n->data;
	    if (gn->cost < 0 || check_edges_weight(gn) != 1) {
		return 0;
	    }
	    // assign default cost and from
	    gn->cost = INFINITY;
	    // gn->from = UNDEFINED;
	    // push current graph node into queue
	    queue_enqueue(q, gn);
	    n = n->next;
	}

	// identify the source graph node
	graph_node_t* source_gn = find_node(g, source);
	if (source_gn == NULL) {
	    return 0;
	}
	// assign the source the graph variable
	g->source = source;
	// assign 0 to 'cost' of source graph node
	source_gn->cost = 0;

	// start handling graph node by popping out from priority queue
	while (!queue_empty(q)) {
	    graph_node_t* current = queue_dequeue(q);
	    if (current == NULL || current->cost < 0) {
		g->source = -1;
		return 0;
	    }

	    node_t* node = current->outNeighbors->head;
	    while (node != NULL) {
		// access data of graph edge and confirm if it exists
		graph_edge_t* edge = node->data;
		// find the graph node where the edge goes
		graph_node_t* target = find_node(g, edge->to);
		// calculate alternative cost then compare with target cost
		int alt = current->cost + edge->cost;
		if (alt < target->cost) {
		    target->cost = alt;
		    target->from = current->data;
		}
		node = node->next;
	    }
	}
	// free priority queue
	free_queue(q);
	return 1;
}

// DFS function to trace the path
// using the 'from' variable assigned to each node by 'calculate_shortest_path'
// returns -1 if graph is NULL
// returns 0 if the tracing is unsuccessful
// returns 1 if successfully traced back from destination to source
int dfs_trace_path(graph_t* g, dll_t* path, int source, graph_node_t* station_gn) {
	if (g == NULL) {
	    return -1;
	}

	if (path == NULL || station_gn == NULL || station_gn->cost < 0) {
	    return 0;
	}

	if (station_gn->cost == 0 && station_gn->data == source) {
	    dll_push_front(path, station_gn);
	    return 1;
	}

	// push current graph node to path front
	dll_push_front(path, station_gn);
	// access variable to find where the graph node is from
	int from = station_gn->from;
	graph_node_t* from_gn = find_node(g, from);
	if (from_gn != NULL) {
	    return dfs_trace_path(g, path, source, from_gn);
	}
	g->source = -1;
	return 0;
}

// prints the shortest path from source to destination if there
// exists a path from the source to the destination. Consider
// how you can solve this recursively if you want to avoid using
// another data structure.
//
// Returns 1 if there is a path from source to destination
// Returns 0 if there is not a path from source to destination
// Returns -1 if the graph is NULL
// Returns -2 if called before `calculate_shortest_path` is called
// Note: consider the bookkeeping fields in `graph_t` struct to
//       determine whether this is true or not
int print_shortest_path(graph_t * g, int source, int dest) {
	// TODO: Implement me!
	if (g == NULL) {
	    return -1;
	}

	// check if the path exists from source to dest
	if (graph_is_reachable(g, source, dest) != 1) {
	    return 0;
	}

	// check if 'g->source' is -1 before calculation
	if (g->source != -1) {
	    return -2;
	}

	// call function of calculation to perform Dijkstra's algorithms
	calculate_shortest_path(g, source);
	// make sure the g->source been changed upon successful path calculation
	if (g->source == -1) {
	    return -2;
	}

	// access the graph node of destination
	graph_node_t* dest_gn = find_node(g, dest);
	if (dest_gn == NULL) return 0;

	// malloc a path to save node stations
	dll_t* path = create_dll();
	if (path == NULL) return 0;

	// DFS function to trace the path using each 'from' variable of graph nodes
	dfs_trace_path(g, path, source, dest_gn);

	// print the path from DLL
	node_t* n = path->head;
	printf("The Shortest Path is:\n");
	while (n != NULL) {
	    graph_node_t* gn = n->data;
	    printf("%d ", gn->data);
	    n = n->next;
	}
	printf("\n");

	// assign g->source back to -1 and free DLL memories
	g->source = -1;
	free_dll(path);
	return 1;
}


#endif
