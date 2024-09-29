#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1024  // Maximum size of a line to read

// Define the dynamic block structure
typedef struct {
    int *data;         // Pointer to hold dynamic integer array
    size_t size;       // Size of the array
} dyn_block;

// Function to allocate a dynamic block
dyn_block *alloc_dyn_block(size_t size) {
    dyn_block *block = (dyn_block *)malloc(sizeof(dyn_block));
    if (block == NULL) {
        return NULL;  // Return NULL if allocation fails
    }
    block->data = (int *)malloc(size * sizeof(int));  // Allocate array
    if (block->data == NULL) {
        free(block);  // Free the block if array allocation fails
        return NULL;
    }
    block->size = size;  // Set the size
    return block;
}

// Function to store an integer array in a dynamic block
void store_mem_blk(dyn_block *block, int *array, size_t size) {
    // Resize if needed
    block->data = realloc(block->data, size * sizeof(int));
    block->size = size;  // Update size
    // Store integers in the dynamic block
    for (size_t i = 0; i < size; i++) {
        block->data[i] = array[i];  // Copy data into the dynamic block
    }
}

// Function to read integers from a file and load them into dynamic blocks
void read_blocks(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");  // Print error if file cannot be opened
        return;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file)) {
        // Count the number of integers in the line
        int count = 0;
        char *token = strtok(line, " ");
        int *array = (int *)malloc(sizeof(int) * LINE_SIZE);  // Temporary array for integers

        while (token != NULL) {
            array[count++] = atoi(token);  // Convert string to integer
            token = strtok(NULL, " ");
        }

        // Allocate memory for the dynamic block
        dyn_block *block = alloc_dyn_block(count);
        if (block == NULL) {
            fprintf(stderr, "Memory allocation for block failed\n");
            free(array);  // Free temporary array if allocation fails
            continue;  // Skip to the next line
        }

        // Store integers in the dynamic block
        store_mem_blk(block, array, count);

        // Verify that data was loaded correctly (printing it)
        printf("Block contains %d integers: ", count);
        for (int i = 0; i < block->size; i++) {
            printf("%d ", block->data[i]);
        }
        printf("\n");

        // Free memory for the dynamic block
        free(block->data);
        free(block);
        free(array);  // Free the temporary array
    }

    fclose(file);  // Close the file
}

int main() {
    // Read integers from the file
    read_blocks("blocks.data");
    return 0;
}
