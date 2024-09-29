#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 10 * 1024 * 1024   // 10 MB,10 megabyte of storage from the heap
#define STATIC_SIZE 5 * 1024 * 1024  // 5 MB,, 5 MB of storage from the static segment
#define STACK_SIZE 1 * 1024 * 1024   // 1 MB,1MB from the stack

// static allocation in the data segment
static char static_segment[STATIC_SIZE];

int main() {
    // allocating memory on the heap
    char *heap_memory = (char *)malloc(HEAP_SIZE);

    if (heap_memory == NULL) {
        fprintf(stderr, "Heap memory allocation failed\n");
        return 1;
    }

    // allocating memory on the stack (done implicitly)
    char stack_memory[STACK_SIZE];

    // memory is returned to the OS when the process terminates
    printf("Memory allocated successfully\n");

    // freeing heap memory
    free(heap_memory);

    return 0;
}
