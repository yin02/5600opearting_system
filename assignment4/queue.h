#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the process structure (process_t)
typedef struct process {
    int id;              // Process identifier
    char *name;          // Process name
    long cumulative_time;// Cumulative run-time
    int priority;        // Priority level
} process_t;

// Define the node structure for the doubly-linked list
typedef struct node {
    void *data;           // Pointer to the data (generic)
    struct node *prev;    // Previous node
    struct node *next;    // Next node
} node_t;

// Define the queue structure (queue_t)
typedef struct queue {
    node_t *head;         // Pointer to the first element
    node_t *tail;         // Pointer to the last element
    int size;             // Size of the queue
} queue_t;

// Function prototypes
queue_t* create_queue();
void push_queue(queue_t* queue, void* element);
void* pop_queue(queue_t* queue);
process_t* remove_process(queue_t* queue);
int get_queue_size(queue_t* queue);
void destroy_queue(queue_t* queue);

#endif
