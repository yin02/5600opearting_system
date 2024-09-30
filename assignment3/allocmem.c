#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE (10 * 1024 * 1024)   // 10 MB
#define STATIC_SIZE (5 * 1024 * 1024)  // 5 MB
#define STACK_SIZE (1 * 1024 * 1024)   // 1 MB

// Static allocation in the data segment
static char static_segment[STATIC_SIZE];

int main() {
    // Allocate memory on the heap
    char *heap_memory = (char *)malloc(HEAP_SIZE);
    if (heap_memory == NULL) {
        fprintf(stderr, "Heap memory allocation failed\n");
        return 1;  // Exit if allocation fails
    }

    // Allocate memory on the stack (done implicitly)
    char stack_memory[STACK_SIZE];

    // Use the stack_memory variable to avoid unused variable warning
    stack_memory[0] = 0;  // Example usage of stack memory

    printf("Memory allocated successfully:\n");
    printf("Heap Memory: %p\n", (void*)heap_memory);
    printf("Static Memory: %p\n", (void*)static_segment);
    printf("Stack Memory: %p\n", (void*)stack_memory);

    // Free the allocated heap memory
    free(heap_memory);

    return 0;
}
