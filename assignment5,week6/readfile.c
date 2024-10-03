#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define STRING_LENGTH 10

// Function to read strings from a file and enqueue them into the queue
void readFileAndEnqueue(const char *filename, Queue *queue) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char buffer[STRING_LENGTH + 1];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        enqueue(queue, buffer);
    }

    fclose(file);
}
