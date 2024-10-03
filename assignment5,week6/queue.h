#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    char *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

Queue* createQueue();
void enqueue(Queue *q, char *str);
char* dequeue(Queue *q);
void freeQueue(Queue *q);
int isQueueEmpty(Queue *q);

#endif
