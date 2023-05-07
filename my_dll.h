// =================== Support Code =================
// Doubly Linked List ( DLL ).
//
//
//
// - Implement each of the functions to create a working DLL.
// - Do not change any of the function declarations
//   - (i.e. dll_t* create_dll() should not have additional arguments)
// - You should not have any 'printf' statements in your DLL functions. 
//   - (You may consider using these printf statements to debug,
//     but they should be removed from your final version)
//   - (You may write helper functions to help you debug your code such as print_list etc)
// ==================================================
#ifndef MYDLL_H
#define MYDLL_H

// Create a node data structure to store data within
// our DLL. In our case, we will stores 'integers'
typedef struct node {
	void* data;
	struct node* next;
  	struct node* previous;
} node_t;

// Create a DLL data structure
// Our DLL holds a pointer to the first node in our DLL called head,
// and a pointer to the last node in our DLL called tail.
typedef struct DLL {
	int count;		// count keeps track of how many items are in the DLL.
	node_t* head;		// head points to the first node in our DLL.
	node_t* tail;          // tail points to the last node in our DLL.
} dll_t;

// Creates a DLL
// Returns a pointer to a newly created DLL.
// The DLL should be initialized with data on the heap.
// (Think about what the means in terms of memory allocation)
// The DLLs fields should also be initialized to default values.
// Returns NULL if we could not allocate memory.
dll_t* create_dll(){
	// allocate memory for creating a new DLL
	dll_t* myDLL = (dll_t*)malloc(sizeof(dll_t));
	if (myDLL == NULL) {
	    return NULL;
	}
	// set default values for member variables of the new DLL
	myDLL->count = 0;
	myDLL->head = NULL; 
	myDLL->tail = NULL;	

	return myDLL;
}

// DLL Empty
// Check if the DLL is empty
// Returns -1 if the dll is NULL.
// Returns 1 if true (The DLL is completely empty)
// Returns 0 if false (the DLL has at least one element enqueued)
int dll_empty(dll_t* l){

	if (l == NULL) {
		return -1;
	}
	
	if (l->count <= 0) {
		return 1;
	} else {
		return 0;
	}
}

// DLL Exist
// Check if node item exists
// Added for HW8 Graph assignment
// Returns -1 if the dll is NULL
// Returns 1 if true (The node exists in the DLL)
// Returns 0 if false (The node does not exist)
int dll_exist(dll_t* l, void* item) {

    	if (l == NULL) {
		return -1;
	}

	node_t* dummy = l->head;
	while (dummy != NULL) {
	    if (dummy == item) {
		return 1;
	    }
	    dummy = dummy->next;
	}
	return 0;
}


// push a new item to the front of the DLL (before the first node in the list).
// Returns -1 if DLL is NULL.
// Returns 1 on success
// Returns 0 on failure ( i.e. we couldn't allocate memory for the new node)
// (i.e. the memory allocation for a new node failed).
int dll_push_front(dll_t* l, void* item){

	if (l == NULL) {
		return -1;
	}

	// allocate memory for creating a new node
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	// confirm the node has been successfully created
	if (newNode == NULL) {
		return 0;
	}
	
	// assign item as new node's data and NULL as its previous pointer
	newNode->data = item;
	newNode->previous = NULL;
	newNode->next = l->head;

	// arrange respective actions for empty and non-empty list
	if (dll_empty(l)) {
		l->tail = newNode;
	} else {
		l->head->previous = newNode;
	}
	// make head be the new node and add to count
	l->head = newNode;
	l->count++;
	
	return 1;
}

// push a new item to the end of the DLL (after the last node in the list).
// Returns -1 if DLL is NULL.
// Returns 1 on success
// Returns 0 on failure ( i.e. we couldn't allocate memory for the new node)
// (i.e. the memory allocation for a new node failed).
int dll_push_back(dll_t* l, void* item){

	if (l == NULL) {
		return -1;
	}

	// allocate memory for creating a new node
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	// confirm the node has been successfully created
	if (newNode == NULL) {
		return 0;
	}

	// assign item as new node's data and NULL as its previous pointer 
	newNode->data = item;
	newNode->previous = l->tail;
	newNode->next = NULL;

	// arrange respective actions for empty and non-empty list
	if (dll_empty(l)) {
		l->head = newNode;
	} else {
		l->tail->next = newNode;
	}
	// make tail be the new node and add to count
	l->tail = newNode;
	l->count++;

	return 1;
}

// Returns the first item in the DLL and also removes it from the list.
// Returns -1 if the DLL is NULL. 
// Returns 0 on failure, i.e. there is noting to pop from the list.
// Assume no negative numbers in the list or the number zero.
void* dll_pop_front(dll_t* t){

	if (t == NULL) {
		return NULL;
	} else if (dll_empty(t)) {
		return NULL;
	}

	// create a temporary node to save original head pointer
	node_t* popItem = t->head;
	void* num = popItem->data; // save the data of temporary node

	t->head = popItem->next; // assign next node to be new head
	// let tail be NULL if new head is NULL (only one element originally)
	if (t->head == NULL) { 
		t->tail = NULL;
	// assign the previous of new head to NULL if new head pointer is not NULL 
	} else {
		t->head->previous = NULL;
	}
	// subtract count for calling pop method each time
	t->count--;
	
	// free the memory allocated for that temporary node before
	free(popItem);
	// return popped-out data
	return num;
}

// Returns the last item in the DLL, and also removes it from the list.
// Returns a -1 if the DLL is NULL. 
// Returns 0 on failure, i.e. there is noting to pop from the list.
// Assume no negative numbers in the list or the number zero.
void* dll_pop_back(dll_t* t){

	if (t == NULL) {
		return NULL;
	} else if (dll_empty(t)) {
		return NULL;
	}

	// create a temporary node to save original tail pointer
	node_t* popItem = t->tail;
	void* num = popItem->data; // save the data of temporary node

	t->tail = popItem->previous; // assign previous node to be new tail
	// let head be NULL if new tail is NULL (only one element originally)
	if (t->tail == NULL) {
		t->head = NULL;
	// assign the next of new tail to NULL if new tail pointer is not NULL
	} else {
		t->tail->next = NULL;
	}
	// subtract count for calling pop method
	t->count--;

	// free the memory allocated for that temporary node before
	free(popItem);
	// return popped-out data
	return num;	
}


// Inserts a new node before the node at the specified position.
// Returns -1 if the list is NULL
// Returns 1 on success
// Retruns 0 on failure:
//  * we couldn't allocate memory for the new node
//  * we tried to insert at a negative location.
//  * we tried to insert past the size of the list
//   (inserting at the size should be equivalent as calling push_back).
int dll_insert(dll_t* l, int pos, void* item){
	
	if (l == NULL) {
		return -1;
	} else if (pos < 0 || pos > l->count) {
		return 0;
	}

	// push a new node with item to the front if pos is 0
	if (pos == 0) {
		return dll_push_front(l, item);
	// push it to the back if pos equals size of DLL
	} else if (pos == l->count) {
		return dll_push_back(l, item);
	}

	// allocate memory for creating a new node
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	// make sure the new node has been successfully created
	if (newNode == NULL) {
		return 0;
	}
	// assign item to data variable of new node
	newNode->data = item;	
	// create a temporary node variable to save address of original head
	node_t* tempPrev = l->head;
	// initiate for loop
	// traverse to corresponding position based on 'pos' argument
	// move to next node each loop while adding up the loop variable
	int i;
	for (i = 1; i < pos; i++) {
		tempPrev = tempPrev->next;
	}
	// create another temporary node to save address of next node 
	node_t* tempNext = tempPrev->next;
	// set new node as into the pointer of its previous and next node
	tempPrev->next = newNode;
	tempNext->previous = newNode;
	// set previous and next pointers for new node
	newNode->previous = tempPrev;
	newNode->next = tempNext;
	// add to count/size of DLL
	l->count++;

	return 1;
}

// Returns the item at position pos starting at 0 ( 0 being the first item )
// Returns -1 if the list is NULL
//  (does not remove the item)
// Returns 0 on failure:
//  * we tried to get at a negative location.
//  * we tried to get past the size of the list
// Assume no negative numbers in the list or the number zero.
void* dll_get(dll_t* l, int pos){

	if (l == NULL) {
		return NULL;
	} else if (dll_empty(l) || pos < 0 || pos >= l->count) {
		return NULL;
	}
	// create a temporary node variable for dummy of the head
	node_t* temp = l->head;
	// initiate for loop
	// traverse to corresponding position
	// move to next node each loop
	int i;
	for (i = 0; i < pos; i++) {
		temp = temp->next;
	}
	// return data variable of position
	return temp->data;
}

// Removes the item at position pos starting at 0 ( 0 being the first item )
// Returns -1 if the list is NULL
// Returns 0 on failure:
//  * we tried to remove at a negative location.
//  * we tried to remove get past the size of the list
// Assume no negative numbers in the list or the number zero.
void* dll_remove(dll_t* l, int pos){

	if (l == NULL) {
		return NULL;
	} else if (dll_empty(l) || pos < 0 || pos >= l->count) {
		return NULL;
	}

	// pop a new node returning item if pos is 0
	if (pos == 0) {
		return dll_pop_front(l);
	// pop a new node returning item if pos equals size
	} else if (pos == (l->count-1)) {
		return dll_pop_back(l);
	}

	// create a dummy (temporary node) for the head node
	node_t* temp = l->head;
	// initiate for loop
	// assign to its own next node each loop
	// traverse to corresponding position
	int i;
	for (i = 0; i < pos; i++) {
		temp = temp->next;
	}
	// change the pointer of the previous and next nodes
	temp->previous->next = temp->next;
	temp->next->previous = temp->previous;
	// subtract one from 'count' after removing node from DLL
	l->count--;

	// set variable to save data of the node
	void* num = temp->data;
	// free memory of the node
	free(temp);
	// return data
	return num;
}

// remove specified node from the DLL
// built for HW8 Graph assignment
void* dll_remove_node(dll_t* l, void* item){

	if (l == NULL || dll_empty(l) || item == NULL) {
		return NULL;
	}

	// type casting
	node_t* n = (node_t*) item;

	// check if node exists
	if (!(dll_exist(l, n))) {
		printf("node is unfound in the neighbors lists\n");
		return NULL;
	}
	
	// pop a new node returning item if pos is 0
	if (l->head == n) {
		return dll_pop_front(l);
	// pop a new node returning item if pos equals size
	} else if (l->tail == n) {
		return dll_pop_back(l);
	}

	// create a dummy (temporary node) for the head node
	node_t* temp = l->head;

	// initiate while loop
	// assign to its own next node each loop
	// traverse to corresponding position
	while (temp != n) {
		temp = temp->next;
	}

	// change the pointer of the previous and next nodes
	temp->previous->next = temp->next;
	temp->next->previous = temp->previous;
	// subtract one from 'count' after removing node from DLL
	l->count--;

	// set variable to save data of the node
	void* num = temp->data;
	// free memory of the node
	free(temp);
	// return data
	return num;
}

/*
// helper function to debug
void dll_print(dll_t* l) {
	if (l == NULL) {
		return;
	}

	node_t* dummy = l->head;
	while (dummy != NULL) {
	    printf("%d ", (int) dummy->data);
	    dummy = dummy->next;
	}
	return;
}
*/

// DLL Size
// Returns -1 if the DLL is NULL.
// Queries the current size of a DLL
int dll_size(dll_t* t){
	if (t == NULL) {
		return -1;
	}
	// return 'count' variable (size)
	return t->count;
}

// Free DLL
// Removes a DLL and all of its elements from memory.
// This should be called before the proram terminates.
void free_dll(dll_t* t){
	// create a temporary node for the head node
	node_t* temp;
	// for loop
	// free memory of each node while traversing DLL
	int i;
	for (i = 0; i < t->count; i++) {
		temp = t->head;
		t->head = temp->next;
		free(temp);
	}
	// free DLL
	free(t);
}



#endif
