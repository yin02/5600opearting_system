#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "message.h"

// Create a new message
Message* create_msg(int id, const char* sender, const char* receiver, const char* content) {
    Message* msg = (Message*)malloc(sizeof(Message)); // 为消息分配内存Allocate memory for the message
    // 主存 是计算机的内存，例如 RAM（随机存取存储器）
    // 检查内存分配是否成功
    if (msg == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
      // 设置消息ID
    msg->id = id;
      // 获取当前时间，并设置消息发送时间
    msg->time_sent = time(NULL);
       // 使用strncpy防止超出目标缓冲区的大小
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    // 复制接收者信息到消息结构体的接收者字段
    strncpy(msg->receiver, receiver, sizeof(msg->receiver) - 1);
    // 复制消息内容到消息结构体的内容字段
    strncpy(msg->content, content, sizeof(msg->content) - 1);
    // 设置消息的已发送状态为false，表示消息未投递
    msg->delivered = false;
    return msg;
}

// Store message to disk，Simulating Secondary Storage: 
// The store_msg() function saves each message as a separate file on disk. 
// This resembles storing messages in secondary storage when main memory (cache) reaches capacity.
int store_msg(const Message* msg) {
    // If the provided message pointer is NULL, return an error code (-1)
    if (msg == NULL) return -1;
     // Create a filename based on the message ID (e.g., message_1.txt)
    char filename[100];
    snprintf(filename, sizeof(filename), "message_%d.txt", msg->id);
      // Open the file for writing ('w' mode). If the file cannot be opened, return an error code (-1)
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
//  simulating retrieval from secondary storage back to main memory.
// Retrieve message from disk
Message* retrieve_msg(int id) {
    // Create a filename based on the message ID (e.g., message_1.txt)
    //  filename is dynamically generated using snprintf,
    //  incorporating the message id to uniquely identify the file containing the message (e.g., message_1.txt).
    char filename[100];
    snprintf(filename, sizeof(filename), "message_%d.txt", id);
 // Open the file for reading ('r' mode). If the file cannot be opened, return NULL
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;
    // Allocate memory for the Message object
    Message* msg = (Message*)malloc(sizeof(Message));
    if (msg == NULL) { // If memory allocation fails, close the file and return NULL
        fclose(file);
        return NULL;
    }
// 使用 fscanf 从文件中逐行读取消息的各个字段，并存储到 Message 结构体的相应字段中：
    fscanf(file, "ID: %d\n", &msg->id);
    fscanf(file, "Time Sent: %lld\n", (long long *)&msg->time_sent);  // Cast to long long pointer
    fscanf(file, "Sender: %49s\n", msg->sender);
    fscanf(file, "Receiver: %49s\n", msg->receiver);
    fscanf(file, "Content: %255s\n", msg->content);
    fscanf(file, "Delivered: %d\n", (int *)&msg->delivered);  // Cast bool to int for compatibility
    fclose(file);
    return msg;
}
