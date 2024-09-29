#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1024

// function to read integers from a file and load them into dynamic blocks
void read_blocks(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file)) {
        // counting number of integers in the line
        int count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            count++;
            token = strtok(NULL, " ");
        }

        // dynamically allocate memory for the integers
        dyn_block *block = alloc_dyn_block(count);
        
        // storing integers in the dynamic block
        token = strtok(line, " ");
        for (int i = 0; i < count; i++) {
            block->data[i] = atoi(token);
            token = strtok(NULL, " ");
        }

        // verify that data was loaded correctly (printing it)
        printf("Block contains %d integers\n", count);
        for (int i = 0; i < count; i++) {
            printf("%d ", block->data[i]);
        }
        printf("\n");

        // free memory for the block
        free(block->data);
        free(block);
    }

    fclose(file);
}

int main() {
    // reading integers from the file
    read_blocks("blocks.data");
    return 0;
}
