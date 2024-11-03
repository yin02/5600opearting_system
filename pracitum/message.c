#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "message.h"

// Create a new message
Message* create_msg(int id, const char* sender, const char* receiver, const char* content) {
    Message* msg = (Message*)malloc(sizeof(Message));
    if (msg == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    msg->id = id;
    msg->time_sent = time(NULL);
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    strncpy(msg->receiver, receiver, sizeof(msg->receiver) - 1);
    strncpy(msg->content, content, sizeof(msg->content) - 1);
    msg->delivered = false;
    return msg;
}

// Store message to disk
int store_msg(const Message* msg) {
    if (msg == NULL) return -1;
    char filename[100];
    snprintf(filename, sizeof(filename), "message_%d.txt", msg->id);
    FILE* file = fopen(filename, "w");
    if (file == NULL) return -1;

    fprintf(file, "ID: %d\n", msg->id);
    fprintf(file, "Time Sent: %lld\n", (long long)msg->time_sent);  // Cast to long long for compatibility
    fprintf(file, "Sender: %s\n", msg->sender);
    fprintf(file, "Receiver: %s\n", msg->receiver);
    fprintf(file, "Content: %s\n", msg->content);
    fprintf(file, "Delivered: %d\n", (int)msg->delivered);  // Cast bool to int for compatibility
    fclose(file);
    return 0;
}

// Retrieve message from disk
Message* retrieve_msg(int id) {
    char filename[100];
    snprintf(filename, sizeof(filename), "message_%d.txt", id);
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Message* msg = (Message*)malloc(sizeof(Message));
    if (msg == NULL) {
        fclose(file);
        return NULL;
    }

    fscanf(file, "ID: %d\n", &msg->id);
    fscanf(file, "Time Sent: %lld\n", (long long *)&msg->time_sent);  // Cast to long long pointer
    fscanf(file, "Sender: %49s\n", msg->sender);
    fscanf(file, "Receiver: %49s\n", msg->receiver);
    fscanf(file, "Content: %255s\n", msg->content);
    fscanf(file, "Delivered: %d\n", (int *)&msg->delivered);  // Cast bool to int for compatibility
    fclose(file);
    return msg;
}
