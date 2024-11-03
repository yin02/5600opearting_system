# Virtual Memory and Cache System Simulation


## Overview

This project simulates a hierarchical memory structure with a focus on virtual memory concepts. It involves creating a message-oriented data store, implementing a caching mechanism, and using page replacement algorithms to manage limited cache space. We evaluated two page replacement strategies—Random Replacement and Least Recently Used (LRU) Replacement—to measure their effectiveness in retaining frequently accessed data.

---

## Part 1: Message Structure and Disk Storage

### Objective
The goal of Part 1 was to create a `Message` data structure and implement disk storage for messages. This simulates secondary storage in a memory hierarchy, where messages that don’t fit in main memory are stored on disk.

### Design and Implementation

- **`Message` Structure**: Defined to include fields for `id`, `time_sent`, `sender`, `receiver`, `content`, and a `delivered` flag. This structure mimics a data packet that would be stored in a memory hierarchy.
- **Functions**:
  - **`create_msg()`**: Creates a dynamically allocated `Message` object with given values.
  - **`store_msg()`**: Saves a message to disk with the filename based on its ID.
  - **`retrieve_msg()`**: Reads a message from disk by ID, populating a `Message` structure.
  
### Connection to Memory Hierarchy
In a hierarchical memory system, data that isn’t immediately needed in main memory (cache) is stored on disk. Part 1 simulates this by storing messages to disk when they cannot all fit in cache memory.

---

## Part 2: Caching Mechanism

### Objective
Part 2 introduces a cache layer to store a subset of messages in memory, allowing faster access to frequently accessed messages. This layer simulates main memory in a hierarchical memory structure.

### Design and Implementation

- **`Cache` Structure**:
  - Holds an array of pointers to `Message` objects, representing a fixed-size cache of `CACHE_SIZE`.
  - Maintains an array of access timestamps to track the last access time for each message.
  - Uses an enum `ReplacementStrategy` to specify either `RANDOM_REPLACEMENT` or `LRU_REPLACEMENT`.
- **Functions**:
  - **`init_cache()`**: Initializes the cache with a specified replacement strategy.
  - **`get_from_cache()`**: Checks if a message is already in cache.
  - **`add_to_cache()`**: Adds a message to the cache if space is available. If the cache is full, a replacement strategy is applied in Part 3.

### Connection to Memory Hierarchy
The cache represents the main memory, storing frequently accessed data closer to the CPU. By caching messages, we reduce the number of accesses to disk (secondary storage), optimizing retrieval speed.

---

## Part 3: Page Replacement Strategies

### Objective
Implement two page replacement strategies—Random Replacement and LRU Replacement—to manage the cache when it reaches its capacity. This part simulates how limited main memory space requires efficient data replacement strategies to retain frequently accessed data.

### Design and Implementation

- **Replacement Strategies**:
  - **Random Replacement**: Replaces a randomly chosen message in the cache. This simple strategy does not account for message access frequency.
  - **LRU Replacement**: Replaces the least recently used message in the cache, identified by the oldest access timestamp. This strategy prioritizes retaining frequently accessed messages.
  
- **Implementation in `add_to_cache()`**:
  - When the cache is full, the `add_to_cache()` function identifies a replacement index based on the specified strategy. The message at that index is evicted, and the new message is added to the cache.

### Connection to Memory Hierarchy
Replacement strategies reflect real-world memory management techniques. In a hierarchical memory system, LRU is often preferred because it aligns with the principle of keeping frequently accessed data in faster memory levels, thereby reducing the need for slower storage accesses.

---

## Part 4: Cache Performance Evaluation

### Objective
Evaluate the performance of the caching system using each replacement strategy by measuring cache hits, misses, and hit ratio. This helps to assess the effectiveness of each strategy in retaining frequently accessed messages in memory.

### Design and Implementation

- **Evaluation Function `evaluate_cache()`**:
  - Simulates a series of random message accesses (1000 accesses) to measure the number of cache hits, misses, and the resulting hit ratio.
  - Randomly selects message IDs between 1 and 20 for access, simulating realistic cache usage patterns.
  - Tracks and prints the total cache hits, misses, and hit ratio for each strategy.

### Results

Below is a sample output from running `evaluate_cache()` with both LRU and Random Replacement:

```plaintext
Testing with LRU Replacement Strategy:
Cache Evaluation:
Total Accesses: 1000
Cache Hits: 680
Cache Misses: 320
Cache Hit Ratio: 68.00%

Testing with Random Replacement Strategy:
Cache Evaluation:
Total Accesses: 1000
Cache Hits: 520
Cache Misses: 480
Cache Hit Ratio: 52.00%
