#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 15 // Change buffer size to 15

int buffer[BUF_SIZE];
int num = 0; // Current number of items in the buffer
int produced_count = 0; // Total number of items produced
int consumed_count = 0; // Total number of items consumed

pthread_mutex_t mut;
pthread_cond_t cond;

// Producer function
void *producer(void *param) {
    int thread_id = *((int *)param);

    while (1) {
        pthread_mutex_lock(&mut);

        // Exit condition: Stop after producing 30 items
        if (produced_count >= 30) {
            pthread_mutex_unlock(&mut);
            break;
        }

        while (num == BUF_SIZE) {
            // Buffer is full, wait for space
            pthread_cond_wait(&cond, &mut);
        }

        // Produce item (use produced_count as the item)
        buffer[num] = produced_count;
        num++;
        produced_count++;

        printf("Producer %d: Produced item %d (Buffer count: %d)\n", thread_id, produced_count, num);

        // Signal consumers that the buffer is not empty
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mut);

        // Simulate production time
        usleep(rand() % 100000);
    }

    printf("Producer %d exiting\n", thread_id);
    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *param) {
    int thread_id = *((int *)param);
    int item;

    while (1) {
        pthread_mutex_lock(&mut);

        // Exit condition: Stop after consuming 30 items
        if (consumed_count >= 30) {
            pthread_mutex_unlock(&mut);
            break;
        }

        while (num == 0) {
            // Buffer is empty, wait for items
            pthread_cond_wait(&cond, &mut);
        }

        // Consume item
        item = buffer[num - 1];
        num--;
        consumed_count++;

        printf("Consumer %d: Consumed item %d (Buffer count: %d)\n", thread_id, item, num);

        // Signal producers that the buffer is not full
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mut);

        // Simulate consumption time
        usleep(rand() % 150000);
    }

    printf("Consumer %d exiting\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t producers[2], consumers[2];
    int producer_ids[2] = {1, 2};
    int consumer_ids[2] = {1, 2};

    // Initialize mutex and condition variable
    if (pthread_mutex_init(&mut, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);
    }

    // Create producer threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
            perror("pthread_create producer");
            exit(1);
        }
    }

    // Create consumer threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]) != 0) {
            perror("pthread_create consumer");
            exit(1);
        }
    }

    // Join producer threads
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < 2; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    printf("All items produced and consumed. Exiting program.\n");

    return 0;
}
