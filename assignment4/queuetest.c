#include <stdio.h>
#include "queue.h"

// Helper function to print the queue of processes
void print_queue(queue_t* queue) {
    node_t* current = queue->head;
    printf("Queue state:\n");
    while (current != NULL) {
        process_t* process = (process_t*)current->data;
        printf("ID: %d, Name: %s, Cumulative Time: %ld, Priority: %d\n", 
               process->id, process->name, process->cumulative_time, process->priority);
        current = current->next;
    }
}

int main() {
    // Create a new queue
    queue_t* queue = create_queue();

    // Create some processes
    process_t p1 = {1, "Process1", 1000, 1};
    process_t p2 = {2, "Process2", 2000, 5};
    process_t p3 = {3, "Process3", 1500, 3};

    // Add processes to the queue
    push_queue(queue, &p1);
    push_queue(queue, &p2);
    push_queue(queue, &p3);

    // Print the queue
    printf("Initial queue:\n");
    print_queue(queue);

    // Remove the highest priority process
    process_t* highest_priority = remove_process(queue);
    printf("\nRemoved process with highest priority: %s\n", highest_priority->name);

    // Print the queue after removing the highest priority process
    print_queue(queue);

    // Pop a process from the front
    process_t* popped = (process_t*)pop_queue(queue);
    printf("\nPopped process: %s\n", popped->name);

    // Print the queue after popping
    print_queue(queue);

    // Clean up
    destroy_queue(queue);
    return 0;
}
