#ifndef CACHE_H
#define CACHE_H

#include "message.h"
#include <time.h>
#include <stdlib.h>

#define CACHE_SIZE 16

// Enum for cache replacement strategies
typedef enum {
    RANDOM_REPLACEMENT,
    LRU_REPLACEMENT
} ReplacementStrategy;

typedef struct {
    Message* messages[CACHE_SIZE];
    time_t last_accessed[CACHE_SIZE];
    int current_size;
    ReplacementStrategy strategy;
} Cache;

// Cache-related function declarations
void init_cache(Cache* cache, ReplacementStrategy strategy);
Message* get_from_cache(Cache* cache, int id);
void add_to_cache(Cache* cache, Message* msg);
void evaluate_cache(Cache* cache, int num_accesses);

#endif // CACHE_H
