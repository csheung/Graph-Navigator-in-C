// Compile this assignment with: gcc -Wall main.c -o main
// Use this file to create several grpahs and show that your implementation
// of Dijkstra's shortest path algorithm works!
//
// Include parts of the C Standard Library
// These have been written by some other really
// smart engineers.
#include <stdio.h>  // For IO operations
#include <stdlib.h> // for malloc/free

// Our library that we have written.
// Also, by a really smart engineer!
#include "my_graph.h"
#include "my_pq.h"
#include "my_shortest.h"
// Note that we are locating this file
// within the same directory, so we use quotations
// and provide the path to this file which is within
// our current directory.

int unitTest1() {

    graph_t * graph = create_graph();

    graph_add_node(graph, 1);
    graph_add_node(graph, 1);
    graph_add_node(graph, 2);

    printf("total nodes: 2==%d\n", graph_num_nodes(graph));
    printf("total edges: 0==%d\n", graph->numEdges);

    if (graph_num_nodes(graph) == 2 && graph->numEdges == 0) {
	free_graph(graph);
	return 1;
    }
    free_graph(graph);
    return 0;
}

int unitTest2(){

    graph_t * graph = create_graph();

    graph_add_node(graph, 1);
    graph_add_node(graph, 2);

    if (graph_add_node(graph, 1) == -1 && graph_add_node(graph, 2) == -1) {
	free_graph(graph);
        return 1;
    }
    free_graph(graph);
    return 0;
}

// test functions of Lab09
// check reachability, cycle-detecting and path-printing
int unitTest3(){

    graph_t* graph = create_graph();

    int i;
    for (i = 1; i <= 10; i++) {
	graph_add_node(graph, i);
    }
    graph_add_edge(graph, 1, 7, 1);
    graph_add_edge(graph, 3, 7, 2);
    graph_add_edge(graph, 5, 7, 3);
    graph_add_edge(graph, 7, 7, 4);
    graph_add_edge(graph, 7, 9, 5);

    int test0 = graph_is_reachable(graph, 7, 7);
    int test1 = graph_print_path(graph, 1, 7);
    int test2 = graph_print_path(graph, 1, 2);
    int test3 = graph_print_path(graph, 1, 9);
    int test4 = graph_print_path(graph, 3, 5);
    int test5 = graph_print_path(graph, 7, 7);
    int test6 = graph_has_cycle(graph);
    int test7 = contains_edge(graph, 1, 7);
    int test8 = contains_edge(graph, 7, 7);
    int test9 = contains_edge(graph, 1, 9);
    printf("%d %d %d %d %d %d %d %d %d %d\n", test0, test1, test2, test3, test4, test5, test6, test7, test8, test9);

    free_graph(graph);

    if (test0 == 1 && test1 == 1 && test2 == 0 && test3 == 1 && test4 == 0 && test5 == 1 && test6 == 1 && test7 == 1 && test8 == 1 && test9 == 0) {
        return 1;
    }
    return 0;
}

// test graph_get_edge function
int unitTest4(){

    graph_t* graph = create_graph();

    int i;
    for (i = 1; i <= 10; i++) {
	graph_add_node(graph, i);
    }
    graph_add_edge(graph, 1, 7, 1);
    graph_add_edge(graph, 3, 7, 2);
    graph_add_edge(graph, 5, 7, 3);
    graph_add_edge(graph, 7, 7, 4);
    graph_add_edge(graph, 7, 9, 5);

    graph_edge_t* e = graph_get_edge(graph, 3, 7);
    int test0 = e->from;
    int test1 = e->to;
    int test2 = e->cost;
    graph_edge_t* ee = graph_get_edge(graph, 3, 9);

    free_graph(graph);

    if (e != NULL && test0 == 3 && test1 == 7 && test2 == 2 && ee == NULL) {
	return 1;
    }
    return 0;
}

int unitTest5(){

    graph_t* graph = create_graph();

    int i;
    for (i = 1; i <= 10; i++) {
	graph_add_node(graph, i);
    }
    graph_add_edge(graph, 1, 7, 1);
    graph_add_edge(graph, 3, 7, 2);
    graph_add_edge(graph, 5, 7, 3);
    graph_add_edge(graph, 7, 7, 4);
    graph_add_edge(graph, 7, 9, 5);

    int n0 = graph->numEdges;
    int test0 = contains_edge(graph, 7, 7);
    int test1 = contains_edge(graph, 7, 9);

    graph_remove_edge(graph, 7, 7);
    int test2 = contains_edge(graph, 7, 7);

    int n1 = graph->numNodes;

    graph_remove_node(graph, 7);
    int n2 = graph->numNodes;
    int n3 = graph->numEdges;

    printf("%d %d %d %d %d %d %d\n", n0, n1, n2, n3, test0, test1, test2);

    free_graph(graph);

    if (n0 == 5 && n1 == 10 && n2 == 9 && n3 == 0 && test0 == 1 && test1 == 1 && test2 == 0) {
	return 1;
    }
    return 0;
}

int unitTest6(){

    graph_t* graph = create_graph();

    int i;
    for (i = 10; i >= 1; i--) {
	graph_add_node(graph, i);
	graph_node_t* temp = find_node(graph, i);
	temp->cost = i;
    }

    int j;
    for (j = 1; j <= 10; j++) {
	graph_add_edge(graph, 1, i, i);
    }
 
    queue_t* q = create_queue(dll_size(graph->nodes));

    node_t* n = graph->nodes->head;
    while (n != NULL) {
	graph_node_t* gn = n->data;
	queue_enqueue(q, gn);
	n = n->next;
    }

    graph_node_t* gn1 = queue_dequeue(q);
    int test1 = gn1->data;
    graph_node_t* gn2 = queue_dequeue(q);
    int test2 = gn2->data;
    graph_node_t* gn3 = queue_dequeue(q);
    int test3 = gn3->data;
    int test4 = queue_size(q);
    printf("%d %d %d %d\n", test1, test2, test3, test4);

    free_queue(q);
    free_graph(graph);

    if (test1 == 1 && test2 == 2 && test3 == 3 && test4 == 7) {
	return 1;
    }
    return 0;
}

// test 'my_shortest_path.h'
int unitTest7(){

    graph_t* graph = create_graph();

    int i;
    for (i = 10; i >= 1; i--) {
	graph_add_node(graph, i);
    }

    int j;
    for (j = 1; j <= 10; j++) {
	graph_add_edge(graph, 2, j, 10);
    }
    graph_add_edge(graph, 1, 2, 3);
    graph_add_edge(graph, 1, 3, 2);
    graph_add_edge(graph, 3, 5, 2);
    graph_add_edge(graph, 5, 7, 3);
    graph_add_edge(graph, 3, 7, 8);

    int a = graph->source;
    int b = print_shortest_path(graph, 1, 7);
    int c = graph->source;

    int k;
    for (k = 1; k <= 10; k++) {
	graph_node_t* gn = find_node(graph, k);
	printf("\nFrom 1 to %d:\n", k);
	print_shortest_path(graph, 1, k);
	printf("lowest cost is: %d\n", gn->cost);
    }
 
    free_graph(graph);

    if (a == -1 && b == 1 && c == -1) {
	return 1;
    }
    return 0;
}
 
 
int (*unitTests[])(int) = {
    unitTest1,
    unitTest2,
    unitTest3,
    unitTest4,
    unitTest5,
    unitTest6,
    unitTest7,
    NULL
};

// ====================================================
// ================== Program Entry ===================
// ====================================================
int main(int argc, const char * argv[]) {

    unsigned int testsPassed = 0;
    // List of Unit Tests to test your data structure
    int counter = 0;
    while(unitTests[counter] != NULL) {
		printf("========unitTest %d========\n",counter);
		if(1==unitTests[counter](1)){
			printf("passed test\n");
			testsPassed++;	
		} else {
			printf("failed test\n");
		}
		counter++;
    }
    return 0;
}

