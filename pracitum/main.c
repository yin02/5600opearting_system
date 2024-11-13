#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "cache.h"

int main() {
    // 测试 LRU 替换策略
    printf("Testing with LRU Replacement Strategy:\n");
    
    // 定义一个缓存对象，并初始化为 LRU 替换策略
    Cache lru_cache;
    init_cache(&lru_cache, LRU_REPLACEMENT);

    // 创建并添加20条消息到缓存中
    for (int i = 1; i <= 20; i++) {
        char content[50];  // 用于存储消息内容的临时字符串
        snprintf(content, sizeof(content), "Message content %d", i);  // 创建消息内容
        Message* msg = create_msg(i, "Alice", "Bob", content);  // 创建一条消息
        if (msg == NULL || store_msg(msg) != 0) {  // 检查消息是否创建并成功存储到磁盘
            fprintf(stderr, "Error creating/storing message\n");
            return 1;  // 如果创建或存储失败，返回错误
        }
        add_to_cache(&lru_cache, msg);  // 将消息添加到缓存中
    }

    // 使用 evaluate_cache 函数评估 LRU 缓存的性能，模拟1000次随机访问
    evaluate_cache(&lru_cache, 1000);

    // 释放 LRU 缓存中所有消息的内存
    for (int i = 0; i < lru_cache.current_size; i++) free(lru_cache.messages[i]);

    // 测试随机替换策略
    printf("\nTesting with Random Replacement Strategy:\n");
    
    // 定义一个缓存对象，并初始化为随机替换策略
    Cache random_cache;
    init_cache(&random_cache, RANDOM_REPLACEMENT);

    // 创建并添加20条消息到缓存中
    for (int i = 1; i <= 20; i++) {
        char content[50];  // 用于存储消息内容的临时字符串
        snprintf(content, sizeof(content), "Message content %d", i);  // 创建消息内容
        Message* msg = create_msg(i, "Alice", "Bob", content);  // 创建一条消息
        if (msg == NULL || store_msg(msg) != 0) {  // 检查消息是否创建并成功存储到磁盘
            fprintf(stderr, "Error creating/storing message\n");
            return 1;  // 如果创建或存储失败，返回错误
        }
        add_to_cache(&random_cache, msg);  // 将消息添加到缓存中
    }

    // 使用 evaluate_cache 函数评估随机替换策略的缓存性能，模拟1000次随机访问
    evaluate_cache(&random_cache, 1000);

    // 释放随机替换策略缓存中所有消息的内存
    for (int i = 0; i < random_cache.current_size; i++) free(random_cache.messages[i]);

    return 0;  // 程序成功结束
}


// This code simulates checking the cache first (like accessing main memory). If the message isn’t found in the cache (a cache miss), it loads from disk and adds it to the cache.
// Benefits of Caching: By adding frequently accessed messages to the cache, we reduce the number of times we need to access the slower disk storage.
// Hierarchical Memory Concept: Caching aligns with the memory hierarchy concept where frequently accessed data is stored closer to the CPU. This cache is an intermediate layer between fast registers (hypothetically the CPU) and slower disk storage.
// Cache Efficiency: The cache reduces retrieval times by storing a fixed number of messages. Access times for cached messages are much faster than loading from disk.
// Fixed Size: By fixing the cache size, you simulate the limited capacity of main memory, which mirrors real-world constraints and lays the groundwork for page replacement strategies in Part 3.
// Cache Hits and Misses: This structure naturally allows you to track cache hits and misses, which will be useful for evaluating cache performance in Part 4.