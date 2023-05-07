// =================== Support Code =================
// Priority Queue
//
// - Implementation of the functions to create a working circular queue.
// - This similar to the queue that we built in HW 2 except that elements
//   are dequeue in order of "priority". We will be implementing it so that
//   elements with the lowest cost are the ones that are dequeued first.
// - Do not change any of the function declarations
//   - (i.e. queue_t* create_queue(unsigned int _capacity) should not have additional arguments)
// - You should not have any 'printf' statements in your queue functions. 
//   - (You may consider using these printf statements to debug, 
//     but they should be removed from your final version)
// ==================================================
#ifndef MYPQ_H
#define MYPQ_H

#include <math.h>
#include "my_graph.h"

// The main data structure for the queue
struct queue {
	unsigned int back;	    // The next free position in the queue
				    		// (i.e. the end or tail of the line)
	unsigned int front;	    // Current 'head' of the queue
				    		// (i.e. the front or head of the line)
	unsigned int size;	    // How many total elements we currently have enqueued.
	unsigned int capacity;      // Maximum number of items the queue can hold
	graph_node_t** data; 	    // The data our queue holds	
};
// Creates a global definition of 'queue_t' so we 
// do not have to retype 'struct queue' everywhere.
typedef struct queue queue_t;

// Create a queue
// Returns a pointer to a newly created queue.
// The queue should be initialized with data on
// the heap.
queue_t* create_queue(unsigned int _capacity){
	queue_t* myQueue = (queue_t*)malloc(sizeof(queue_t));
	if (myQueue == NULL) {
	    return NULL;
	}
	// assign values for struct member variables
	myQueue->back = myQueue->front = myQueue->size = 0;
	myQueue->capacity = _capacity;
	myQueue->data = (graph_node_t**)malloc(_capacity * sizeof(graph_node_t*));
	if (myQueue->data == NULL) {
	    return NULL;
	}
	return myQueue;
}

// Queue Empty
// Check if the queue is empty
// Returns 1 if true (The queue is completely empty)
// Returns 0 if false (the queue has at least one element enqueued)
int queue_empty(queue_t* q){
	// queue is empty when size equals 0
	if (q->size <= 0) {
		return 1;
	}
	return 0;
}

// Queue Full
// Check if the queue is Full
// Returns 1 if true (The queue is completely full)
// Returns 0 if false (the queue has more space available to enqueue items)
int queue_full(queue_t* q){
	// queue is full when reaching the capacity
	if (q->size >= q->capacity) {
		return 1;
	}
	return 0;
}

// Enqueue a new node
// i.e. push a new item into our data structure
// Returns a -1 if the operation fails (otherwise returns 0 on success).
// (i.e. if the queue is full that is an error).
int queue_enqueue(queue_t* q, graph_node_t* item){
	if ( queue_full(q) ) {
		return -1;
	}
	// assign item to the back position
	q->data[q->back] = item;
	// reset back to the next position
	q->back = (q->back + 1) % q->capacity;
	// add to size
	q->size++;

	return 0;
}

// Dequeue the node that has the minimum cost
// Returns the item at the front of the queue and
// removes an item from the queue.
// Removing from an empty queue should crash the program, call exit(1)
graph_node_t* queue_dequeue(queue_t* q){
	if ( queue_empty(q) ) {
		exit(1);
	}

	// initiate a variable to hold minimum cost for comparison
	int min = q->data[q->front]->cost;
	int ptr = q->front;
	int i = q->front + 1; // begins with 2nd place if it exists
	int j;
	while (i < (q->back + 10)) {
	    // use i to traverse and j for accessing array data
	    if (i >= 10) {
		j = i % q->capacity;
	    } else {
		j = i;
	    }
	    if (q->data[j]->cost < min) {
		min = q->data[j]->cost;
		ptr = j;
	    }
	    i++;
	}

	// assign the front data to variable
	graph_node_t* item = q->data[ptr];
	// use ptr to shift all data one position forward
	while (ptr > q->front) {
	    q->data[ptr] = q->data[ptr-1];
	    ptr--;
	}
	// change front position to the next
	q->front = (q->front + 1) % q->capacity;
	// subtract size
	q->size--;

	return item;
}

// Queue Size
// Queries the current size of a queue
// A queue that has not been previously created will crash the program.
// (i.e. A NULL queue cannot return the size, call exit(1))
unsigned int queue_size(queue_t* q){
	if (q == NULL) {
	    exit(1);
	}
	return (q->size);
}

// Free queue
// Removes a queue and all of its elements from memory.
// This should be called before the proram terminates.
void free_queue(queue_t* q){
	if (q == NULL) return;

	// free array data of queue from the heap
	free(q->data);
	// free queue data from the heap
	free(q);
}


#endif
