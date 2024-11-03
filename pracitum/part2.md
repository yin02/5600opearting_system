In Part 2, we add a caching mechanism to efficiently store and retrieve messages in memory. This caching system mimics the role of main memory (RAM) in a memory hierarchy by storing a subset of messages in memory, reducing the need to access the slower secondary storage (disk). Here’s a breakdown of the design, approach, and an explanation of how this aligns with the project’s hierarchical memory requirements.

---

### 1. **Objective for Part 2**

- **Add a Cache Layer**: The cache will store a subset of messages in memory, enabling faster access to frequently retrieved messages without needing to access the disk.
- **Fixed-Size Messages**: Each message in the cache should be of a fixed size, making it easier to manage memory and improve retrieval efficiency. A fixed size also emulates paging in virtual memory systems.
- **Caching Strategy**: The cache stores messages and retrieves them based on their unique identifiers. When a message isn’t in the cache, it is loaded from disk, added to the cache, and stored for future access.

---

### 2. **Design of the Cache Structure**

The cache is implemented using a `Cache` structure. This structure will manage the list of messages currently in memory, track the time each message was last accessed (for use in later replacement algorithms), and set a maximum capacity.

- **`Cache` Structure Design**:
  - **Array of Messages**: Store pointers to `Message` structures in an array. This array simulates the pages in a memory hierarchy.
  - **Access Timestamps**: Use an array of timestamps to keep track of when each message was last accessed. This timestamp array will be used for implementing Least Recently Used (LRU) replacement in Part 3.
  - **Current Size**: Track the current number of messages in the cache to manage when messages need to be replaced.

- **`Cache` Structure**:
  ```c
  #define CACHE_SIZE 16

  typedef struct {
      Message* messages[CACHE_SIZE];
      time_t last_accessed[CACHE_SIZE];
      int current_size;
  } Cache;
  ```
  - **`CACHE_SIZE`**: Defines the maximum number of messages that can be stored in the cache, simulating a fixed-size main memory.
  - **`last_accessed` Array**: Stores the last access time for each message, which will later support cache replacement strategies like LRU.

---

### 3. **Functions to Implement in Part 2**

#### a) `init_cache()`: Initialize the Cache

The `init_cache()` function initializes the cache structure, setting all message pointers to `NULL` and setting the current cache size to 0.

```c
void init_cache(Cache* cache) {
    cache->current_size = 0;
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->messages[i] = NULL;
        cache->last_accessed[i] = 0;
    }
}
```

- **Explanation to Instructor**: 
  - This function prepares the cache to store messages and initializes the `last_accessed` array to track usage patterns.

#### b) `get_from_cache()`: Retrieve Message from Cache

The `get_from_cache()` function checks if a message with a specified ID is currently in the cache. If found, it returns the message; otherwise, it returns `NULL`.

```c
Message* get_from_cache(Cache* cache, int id) {
    for (int i = 0; i < cache->current_size; i++) {
        if (cache->messages[i] && cache->messages[i]->id == id) {
            cache->last_accessed[i] = time(NULL);  // Update access time
            return cache->messages[i];
        }
    }
    return NULL;  // Cache miss
}
```

- **Explanation to Instructor**:
  - This function scans through the cache to locate a message by ID. If found, it updates the `last_accessed` timestamp, simulating a memory access in main memory.
  - If the message is not found in the cache, it indicates a **cache miss**.

#### c) `add_to_cache()`: Add Message to Cache

The `add_to_cache()` function adds a message to the cache. If the cache is full, it does not replace messages yet (replacement strategies will be added in Part 3).

```c
void add_to_cache(Cache* cache, Message* msg) {
    if (cache->current_size < CACHE_SIZE) {
        // Add to first empty slot
        cache->messages[cache->current_size] = msg;
        cache->last_accessed[cache->current_size] = time(NULL);
        cache->current_size++;
    } else {
        // Cache full, message replacement needed in Part 3
        printf("Cache is full, replacement strategy needed.\n");
    }
}
```

- **Explanation to Instructor**:
  - When the cache isn’t full, `add_to_cache()` simply adds the new message to the next available slot and sets its last access time.
  - If the cache is full, a replacement strategy will be implemented in Part 3 to handle this scenario.
  - **Connection to Memory Hierarchy**: By caching messages in memory, we reduce the number of disk accesses, aligning with the memory hierarchy concept where frequently accessed data is kept closer to the CPU.

#### d) Modify `main.c` to Use the Cache

In `main.c`, you can now test the cache by first checking if a message is in the cache (`get_from_cache`). If it’s a cache miss, retrieve the message from disk and add it to the cache.

```c
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "cache.h"

int main() {
    Cache cache;
    init_cache(&cache);

    for (int i = 1; i <= 20; i++) {
        char content[50];
        snprintf(content, sizeof(content), "Message content %d", i);
        Message* msg = create_msg(i, "Alice", "Bob", content);
        if (msg == NULL || store_msg(msg) != 0) {
            fprintf(stderr, "Error creating/storing message\n");
            return 1;
        }

        // Attempt to retrieve from cache first
        Message* cached_msg = get_from_cache(&cache, i);
        if (cached_msg != NULL) {
            printf("Cache hit: Retrieved message ID %d from cache.\n", i);
        } else {
            printf("Cache miss: Loading message ID %d from disk and adding to cache.\n", i);
            Message* retrieved_msg = retrieve_msg(i);
            add_to_cache(&cache, retrieved_msg);
        }
    }

    // Free allocated messages in cache
    for (int i = 0; i < cache.current_size; i++) {
        free(cache.messages[i]);
    }
    return 0;
}
```

- **Explanation to Instructor**:
  - This code simulates checking the cache first (like accessing main memory). If the message isn’t found in the cache (a cache miss), it loads from disk and adds it to the cache.
  - **Benefits of Caching**: By adding frequently accessed messages to the cache, we reduce the number of times we need to access the slower disk storage.

---

### 4. **Explaining Your Design and Approach**

When explaining your approach to your instructor, emphasize the following points:

- **Hierarchical Memory Concept**: Caching aligns with the memory hierarchy concept where frequently accessed data is stored closer to the CPU. This cache is an intermediate layer between fast registers (hypothetically the CPU) and slower disk storage.
- **Cache Efficiency**: The cache reduces retrieval times by storing a fixed number of messages. Access times for cached messages are much faster than loading from disk.
- **Fixed Size**: By fixing the cache size, you simulate the limited capacity of main memory, which mirrors real-world constraints and lays the groundwork for page replacement strategies in Part 3.
- **Cache Hits and Misses**: This structure naturally allows you to track cache hits and misses, which will be useful for evaluating cache performance in Part 4.

### 5. **How It Meets the Course Requirements**

- **Hierarchical Memory Structure**: Part 2 introduces a cache layer, representing the main memory in a hierarchical structure.
- **Performance Optimization**: This caching system optimizes retrieval by reducing disk access for frequently used data, which aligns with the project’s goal of understanding memory hierarchy and virtual memory concepts.
- **Preparation for Replacement Algorithms**: The setup in Part 2 prepares for Part 3, where you will need to replace messages when the cache reaches its maximum capacity, mimicking real-world memory management.

