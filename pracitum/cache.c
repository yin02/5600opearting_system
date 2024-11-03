#include "cache.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Initialize cache with a given replacement strategy
void init_cache(Cache* cache, ReplacementStrategy strategy) {
    cache->current_size = 0;
    cache->strategy = strategy;
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->messages[i] = NULL;
        cache->last_accessed[i] = 0;
    }
}

// Retrieve a message from cache by ID
Message* get_from_cache(Cache* cache, int id) {
    for (int i = 0; i < cache->current_size; i++) {
        if (cache->messages[i] && cache->messages[i]->id == id) {
            cache->last_accessed[i] = time(NULL);
            return cache->messages[i];
        }
    }
    return NULL;
}

// Add a message to the cache with replacement strategies
void add_to_cache(Cache* cache, Message* msg) {
    int replace_index = 0;

    if (cache->current_size < CACHE_SIZE) {
        cache->messages[cache->current_size] = msg;
        cache->last_accessed[cache->current_size] = time(NULL);
        cache->current_size++;
    } else {
        if (cache->strategy == RANDOM_REPLACEMENT) {
            replace_index = rand() % CACHE_SIZE;
        } else if (cache->strategy == LRU_REPLACEMENT) {
            for (int i = 1; i < CACHE_SIZE; i++) {
                if (cache->last_accessed[i] < cache->last_accessed[replace_index]) {
                    replace_index = i;
                }
            }
        }
        free(cache->messages[replace_index]);
        cache->messages[replace_index] = msg;
        cache->last_accessed[replace_index] = time(NULL);
    }
}

// Evaluate cache performance
void evaluate_cache(Cache* cache, int num_accesses) {
    int hits = 0, misses = 0;
    srand(time(NULL));

    for (int i = 0; i < num_accesses; i++) {
        int random_id = (rand() % 20) + 1;
        Message* msg = get_from_cache(cache, random_id);
        if (msg != NULL) {
            hits++;
        } else {
            msg = retrieve_msg(random_id);
            if (msg != NULL) {
                add_to_cache(cache, msg);
                misses++;
            }
        }
    }

    double hit_ratio = (double)hits / num_accesses;
    printf("Cache Evaluation:\n");
    printf("Total Accesses: %d\n", num_accesses);
    printf("Cache Hits: %d\n", hits);
    printf("Cache Misses: %d\n", misses);
    printf("Cache Hit Ratio: %.2f%%\n", hit_ratio * 100);
}
