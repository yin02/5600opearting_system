#include "queue.h"

// Create an empty queue
queue_t* create_queue() {
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->head = queue->tail = NULL;
    queue->size = 0;
    return queue;
}

// Push an element into the queue (enqueue)
void push_queue(queue_t* queue, void* element) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = element;
    new_node->next = NULL;
    new_node->prev = queue->tail;

    if (queue->tail) {
        queue->tail->next = new_node;
    } else {
        queue->head = new_node;
    }
    queue->tail = new_node;
    queue->size++;
}

// Pop an element from the front of the queue (dequeue)
void* pop_queue(queue_t* queue) {
    if (queue->size == 0) {
        return NULL;
    }
    node_t* temp = queue->head;
    void* data = temp->data;
    queue->head = temp->next;
    if (queue->head) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

// Remove the process with the highest priority
process_t* remove_process(queue_t* queue) {
    if (queue->size == 0) {
        return NULL;
    }

    node_t* current = queue->head;
    node_t* highest_priority_node = current;
    process_t* highest_priority_process = (process_t*)current->data;

    // Traverse the queue to find the process with the highest priority
    while (current != NULL) {
        process_t* current_process = (process_t*)current->data;
        if (current_process->priority > highest_priority_process->priority) {
            highest_priority_process = current_process;
            highest_priority_node = current;
        }
        current = current->next;
    }

    // Remove the node with the highest priority
    if (highest_priority_node->prev) {
        highest_priority_node->prev->next = highest_priority_node->next;
    } else {
        queue->head = highest_priority_node->next;
    }
    if (highest_priority_node->next) {
        highest_priority_node->next->prev = highest_priority_node->prev;
    } else {
        queue->tail = highest_priority_node->prev;
    }

    free(highest_priority_node);
    queue->size--;
    return highest_priority_process;
}

// Get the current size of the queue
int get_queue_size(queue_t* queue) {
    return queue->size;
}

// Free all memory associated with the queue
void destroy_queue(queue_t* queue) {
    while (queue->size > 0) {
        pop_queue(queue);
    }
    free(queue);
}
