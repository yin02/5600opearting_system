

## Project Structure

- **`main.c`**: The entry point of the program, which sets up the cache, stores messages, and evaluates cache performance.
- **`message.h`**: Header file defining the `Message` struct and message-related function declarations.
- **`message.c`**: Implements functions for creating, storing, and retrieving messages from disk.
- **`cache.h`**: Header file defining the `Cache` struct, replacement strategies, and cache function declarations.
- **`cache.c`**: Implements cache initialization, message retrieval, caching, and page replacement algorithms.
- **`Makefile`**: A build file to compile the program and manage dependencies.

---

## Tasks

### Part 1: Memory Hierarchy Simulation (30 Points)

- **Task**: Create a cache mechanism for a message-oriented data store. Messages are retrieved rapidly, but due to volume, not all fit in main memory and are stored in secondary memory (file system).
- **Steps**:
  - Define a `Message` struct with a unique identifier, timestamp, sender, receiver, content, and a delivered flag.
  - Implement `create_msg()` to create a new message and return a dynamically allocated `Message`.
  - Implement `store_msg()` to store a message on disk.
  - Implement `retrieve_msg()` to retrieve a message by ID.

### Part 2: Caching (30 Points)

- **Task**: Add a cache to store messages in memory to reduce disk accesses.
- **Requirements**:
  - Store each message in a paged structure in memory, with a fixed size (e.g., 1024 bytes).
  - If a message isn’t found in cache, load it from disk and add it to the cache.
  - Document your design choices, cache structure, and lookup mechanism.

### Part 3: Page Replacement (20 Points)

- **Task**: Implement two page replacement algorithms for the cache:
  - **Random Replacement**: Replaces a random page in the cache.
  - **LRU (Least Recently Used)**: Replaces the least recently used page.

### Part 4: Evaluation (20 Points)

- **Task**: Evaluate the cache performance by calculating:
  - Number of cache hits per 1000 random message accesses.
  - Number of cache misses per 1000 random message accesses.
  - Cache hit ratio per 1000 random message accesses.

---

## Getting Started

### Compilation
To compile the program, use the following command:

```bash
make
```

```bash 
./message_program

```
