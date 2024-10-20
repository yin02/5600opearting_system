#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ARRAY_SIZE 10
int global_array[ARRAY_SIZE];  // Global shared array

pthread_mutex_t lock;

void *writer(void *data) {
    pthread_mutex_lock(&lock);  // Lock to prevent race conditions
    for (int i = 0; i < ARRAY_SIZE; i++) {
        global_array[i] = i * 10;  // Populate the array with some data
        printf("Thread Writer: Writing %d to global_array[%d]\n", global_array[i], i);
    }
    pthread_mutex_unlock(&lock);  // Unlock after writing
    return NULL;
}

void *reader(void *data) {
    pthread_mutex_lock(&lock);  // Lock to prevent race conditions
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Thread Reader: Reading global_array[%d] = %d\n", i, global_array[i]);
    }
    pthread_mutex_unlock(&lock);  // Unlock after reading
    return NULL;
}

int main(void) {
    pthread_t writer_thread, reader_thread;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    // Create the writer and reader threads
    pthread_create(&writer_thread, NULL, writer, NULL);
    pthread_create(&reader_thread, NULL, reader, NULL);

    // Wait for both threads to finish
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    printf("Exiting from main program\n");
    return 0;
}


// Consistency Issue: we  notice inconsistent behavior, where the reader thread tries to read from the array before the writer has finished writing. This happens because both threads access shared data without any synchronization mechanism in place.

// Expectation of Inconsistency: Without proper synchronization (like locks or mutexes), it is expected to have inconsistent data access when multiple threads interact with shared resources.

// Solution: To avoid these issues, we can use synchronization techniques like mutexes (as shown above), semaphores, or condition variables to ensure that threads do not interfere with each other's operations.