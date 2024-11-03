#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>
#include <time.h>

// Define the Message struct
typedef struct {
    int id;
    time_t time_sent;       // Timestamp for when the message was sent
    char sender[50];        // Sender's name
    char receiver[50];      // Receiver's name
    char content[256];      // Message content
    bool delivered;         // Delivery status
} Message;

// Function declarations
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);
int store_msg(const Message* msg);
Message* retrieve_msg(int id);

#endif // MESSAGE_H
