#include "cache.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 初始化缓存，设置替换策略
void init_cache(Cache* cache, ReplacementStrategy strategy) {
    cache->current_size = 0;  // 初始化当前缓存大小为0
    cache->strategy = strategy;  // 设置缓存的替换策略（如随机替换或LRU替换）
    
    // 初始化缓存中的每个消息为NULL，并将每个消息的最后访问时间设置为0
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->messages[i] = NULL;  // 初始化缓存中的每个消息为NULL
        cache->last_accessed[i] = 0;  // 初始化每个消息的最后访问时间为0
    }
}

// 从缓存中根据ID获取消息
Message* get_from_cache(Cache* cache, int id) {
    // 遍历缓存中的所有消息，查找是否存在该ID的消息
    for (int i = 0; i < cache->current_size; i++) {
        // 如果找到了对应ID的消息
        if (cache->messages[i] && cache->messages[i]->id == id) {
            // 更新该消息的最后访问时间
            cache->last_accessed[i] = time(NULL);
            return cache->messages[i];  // 返回找到的消息
        }
    }
    return NULL;  // 如果没有找到该消息，则返回NULL
}

// 将消息添加到缓存中，使用不同的替换策略
void add_to_cache(Cache* cache, Message* msg) {
    int replace_index = 0;  // 替换消息的索引

    // 如果缓存中还有空间，直接将新消息添加到缓存
    if (cache->current_size < CACHE_SIZE) {
        cache->messages[cache->current_size] = msg;  // 将新消息添加到缓存中
        cache->last_accessed[cache->current_size] = time(NULL);  // 更新消息的最后访问时间
        cache->current_size++;  // 增加缓存的当前大小
    } else {
        // 如果缓存已满，根据替换策略选择一个要替换的消息
        if (cache->strategy == RANDOM_REPLACEMENT) {
            // 随机替换策略：随机选择一个消息进行替换
            replace_index = rand() % CACHE_SIZE;
        } else if (cache->strategy == LRU_REPLACEMENT) {
            // LRU（最近最少使用）替换策略：选择最后访问时间最久的消息进行替换
            for (int i = 1; i < CACHE_SIZE; i++) {
                if (cache->last_accessed[i] < cache->last_accessed[replace_index]) {
                    replace_index = i;  // 更新替换索引
                }
            }
        }

        // 释放被替换消息的内存
        free(cache->messages[replace_index]);

        // 将新消息添加到缓存
        cache->messages[replace_index] = msg;
        cache->last_accessed[replace_index] = time(NULL);  // 更新消息的最后访问时间
    }
}

// 评估缓存的性能，模拟一定数量的访问
void evaluate_cache(Cache* cache, int num_accesses) {
    int hits = 0, misses = 0;  // 缓存命中和缓存未命中的计数器
    srand(time(NULL));  // 设置随机数种子

    // 模拟 num_accesses 次随机访问
    for (int i = 0; i < num_accesses; i++) {
        int random_id = (rand() % 20) + 1;  // 生成一个随机的消息ID（假设消息ID范围是1到20）
        
        // 从缓存中尝试获取消息
        Message* msg = get_from_cache(cache, random_id);
        if (msg != NULL) {
            // 如果缓存命中
            hits++;
        } else {
            // 如果缓存未命中，从存储中检索消息
            msg = retrieve_msg(random_id);
            if (msg != NULL) {
                // 如果消息成功从存储中检索到，添加到缓存中
                add_to_cache(cache, msg);
                misses++;  // 增加缓存未命中的计数
            }
        }
    }

    // 计算缓存命中率
    double hit_ratio = (double)hits / num_accesses;
    printf("Cache Evaluation:\n");
    printf("Total Accesses: %d\n", num_accesses);  // 输出总访问次数
    printf("Cache Hits: %d\n", hits);  // 输出缓存命中次数
    printf("Cache Misses: %d\n", misses);  // 输出缓存未命中次数
    printf("Cache Hit Ratio: %.2f%%\n", hit_ratio * 100);  // 输出缓存命中率
}
