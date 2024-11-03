In Part 3, we add page replacement algorithms to handle cases when the cache is full. This part extends the cache design from Part 2 by implementing two replacement strategies: **Random Replacement** and **Least Recently Used (LRU) Replacement**. These strategies manage which messages are evicted from the cache when new messages need to be added, simulating how a real system would handle limited main memory.

Here’s how to design and explain Part 3.

---

### 1. **Objective for Part 3**

- **Implement Replacement Algorithms**: Add functionality to the cache system to replace messages when the cache reaches its capacity.
- **Random and LRU Replacement**:
  - **Random Replacement**: Selects a random message in the cache for replacement.
  - **LRU Replacement**: Selects the least recently used message for replacement, based on access timestamps.

### 2. **Adding Replacement Strategies**

We modify the `Cache` structure and `add_to_cache()` function to incorporate these two replacement strategies.

#### a) Update `Cache` Structure

Add an enum to specify which replacement strategy to use. This will allow flexibility to switch between strategies.

In **`cache.h`**:

```c
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
```

- **Explanation to Instructor**:
  - **Replacement Strategy Enum**: Allows for easy selection of either `RANDOM_REPLACEMENT` or `LRU_REPLACEMENT`, enabling comparison of different approaches to cache replacement.
  - **Modular Design**: This structure lets us easily add or modify replacement strategies in the future.

#### b) Modify `init_cache()` to Set the Replacement Strategy

In **`cache.c`**:

```c
void init_cache(Cache* cache, ReplacementStrategy strategy) {
    cache->current_size = 0;
    cache->strategy = strategy;
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->messages[i] = NULL;
        cache->last_accessed[i] = 0;
    }
}
```

- **Explanation to Instructor**: The `init_cache()` function now takes an additional argument, `strategy`, allowing you to specify the replacement strategy when initializing the cache. This choice will affect how messages are replaced when the cache is full.

#### c) Implement `add_to_cache()` with Replacement Strategies

The `add_to_cache()` function now includes logic for both replacement strategies. If the cache is full, it will select a message to replace based on the chosen strategy.

```c
void add_to_cache(Cache* cache, Message* msg) {
    int replace_index = 0;

    if (cache->current_size < CACHE_SIZE) {
        // Add to first empty slot
        cache->messages[cache->current_size] = msg;
        cache->last_accessed[cache->current_size] = time(NULL);
        cache->current_size++;
    } else {
        // Cache is full, replacement is needed
        if (cache->strategy == RANDOM_REPLACEMENT) {
            // Randomly select a message to replace
            replace_index = rand() % CACHE_SIZE;
        } else if (cache->strategy == LRU_REPLACEMENT) {
            // Find the least recently used message
            replace_index = 0;
            for (int i = 1; i < CACHE_SIZE; i++) {
                if (cache->last_accessed[i] < cache->last_accessed[replace_index]) {
                    replace_index = i;
                }
            }
        }

        // Free the existing message at replace_index
        free(cache->messages[replace_index]);
        // Replace with the new message
        cache->messages[replace_index] = msg;
        cache->last_accessed[replace_index] = time(NULL);
    }
}
```

- **Explanation to Instructor**:
  - **Random Replacement**: Randomly selects an index in the cache array to replace. This approach is simple but may not be efficient, as it can replace frequently accessed messages.
  - **LRU Replacement**: Iterates through `last_accessed` to find the index with the oldest timestamp. This approach is more intelligent, as it keeps frequently accessed messages in the cache.
  - **Freeing Old Messages**: Before replacing a message, the old message’s memory is freed to prevent memory leaks, ensuring efficient memory management.

### 3. **Modify `main.c` to Test Replacement Strategies**

Update `main.c` to initialize the cache with both replacement strategies (Random and LRU) and test them separately. This lets you compare their performance under similar conditions.

In **`main.c`**:

```c
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "cache.h"

int main() {
    // Testing with LRU Replacement Strategy
    printf("Testing with LRU Replacement Strategy:\n");
    Cache lru_cache;
    init_cache(&lru_cache, LRU_REPLACEMENT);

    for (int i = 1; i <= 20; i++) {
        char content[50];
        snprintf(content, sizeof(content), "Message content %d", i);
        Message* msg = create_msg(i, "Alice", "Bob", content);
        if (msg == NULL || store_msg(msg) != 0) {
            fprintf(stderr, "Error creating/storing message\n");
            return 1;
        }

        Message* cached_msg = get_from_cache(&lru_cache, i);
        if (cached_msg != NULL) {
            printf("Cache hit: Retrieved message ID %d from cache.\n", i);
        } else {
            printf("Cache miss: Loading message ID %d from disk and adding to cache.\n", i);
            Message* retrieved_msg = retrieve_msg(i);
            add_to_cache(&lru_cache, retrieved_msg);
        }
    }

    for (int i = 0; i < lru_cache.current_size; i++) free(lru_cache.messages[i]);

    // Testing with Random Replacement Strategy
    printf("\nTesting with Random Replacement Strategy:\n");
    Cache random_cache;
    init_cache(&random_cache, RANDOM_REPLACEMENT);

    for (int i = 1; i <= 20; i++) {
        char content[50];
        snprintf(content, sizeof(content), "Message content %d", i);
        Message* msg = create_msg(i, "Alice", "Bob", content);
        if (msg == NULL || store_msg(msg) != 0) {
            fprintf(stderr, "Error creating/storing message\n");
            return 1;
        }

        Message* cached_msg = get_from_cache(&random_cache, i);
        if (cached_msg != NULL) {
            printf("Cache hit: Retrieved message ID %d from cache.\n", i);
        } else {
            printf("Cache miss: Loading message ID %d from disk and adding to cache.\n", i);
            Message* retrieved_msg = retrieve_msg(i);
            add_to_cache(&random_cache, retrieved_msg);
        }
    }

    for (int i = 0; i < random_cache.current_size; i++) free(random_cache.messages[i]);

    return 0;
}
```

### Explanation to Instructor

- **Purpose of Each Replacement Strategy**:
  - **Random Replacement** is a simple approach that doesn’t consider access frequency, making it less efficient but easy to implement.
  - **LRU Replacement** is designed to keep frequently accessed messages in the cache, reflecting a more realistic memory management strategy where recently used data is prioritized.
- **Testing Both Strategies**: By testing both strategies, we can compare their effectiveness in minimizing disk accesses, demonstrating the impact of replacement strategies on performance.
- **Efficient Memory Management**: Messages are dynamically allocated, and we free them when they’re replaced to prevent memory leaks, which is essential in a real-world memory management system.

### 4. **Connection to Hierarchical Memory**

- **Hierarchical Memory Simulation**: Part 3 simulates main memory limitations by enforcing a fixed cache size and using replacement strategies to manage memory more effectively.
- **Impact of Replacement Strategies**:
  - **Random Replacement** models a basic eviction policy that’s easy to implement but may not be efficient in a memory hierarchy.
  - **LRU Replacement** aligns with real-world virtual memory systems, where frequently accessed data is retained in the main memory cache.
- **Preparing for Evaluation**: Part 3 sets up the foundation for evaluating cache performance in Part 4, where we’ll measure cache hits, misses, and the hit ratio to determine each strategy's effectiveness.

---

### Summary to Present to Instructor

- **Objective**: Add replacement strategies to manage cache contents when full.
- **Design**:
  - **Random Replacement**: Simple and randomly evicts messages, less efficient.
  - **LRU Replacement**: Keeps frequently accessed messages, mirroring real-world memory management.
- **Memory Hierarchy Connection**: Both strategies allow the cache to function within memory limits, simulating main memory’s role in a memory hierarchy.
- **Benefits**: Efficient memory management and realistic data retention. This design prepares for performance evaluation by contrasting the effectiveness of each strategy in reducing disk access.

This explanation covers the design, implementation, and relevance