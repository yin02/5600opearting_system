#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Create a new queue
Queue* createQueue() {
    Queue *q = (Queue*) malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Enqueue a string
void enqueue(Queue *q, char *str) {
    QueueNode *newNode = (QueueNode*) malloc(sizeof(QueueNode));
    newNode->data = strdup(str);
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

// Dequeue a string
char* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    char *data = temp->data;
    free(temp);
    return data;
}

// Free the queue
void freeQueue(Queue *q) {
    while (!isQueueEmpty(q)) {
        free(dequeue(q));
    }
    free(q);
}

// Check if the queue is empty
int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}
