The implementation of **Option 4a: Multiple Clients** in your server code is primarily demonstrated in how the server handles incoming client connections by spawning a new thread for each client. Here's how you can explain this to your professor:

---

### **Demonstration of Option 4a: Multiple Clients**

1. **Where It Is Implemented in Code**
   - The key implementation is in the `main()` function where the server accepts client connections and spawns a thread to handle each connection. The relevant snippet is:

```c
while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Accept failed");
        continue;
    }

    pthread_t client_thread;
    int *new_sock = malloc(sizeof(int)); // Allocate memory for the socket
    *new_sock = client_sock;

    if (pthread_create(&client_thread, NULL, handle_client, new_sock) != 0) {
        perror("Thread creation failed");
        free(new_sock);
        continue;
    }

    pthread_detach(client_thread); // Ensure resources are cleaned up
}
```

   - **Explanation for Professor**:
     - For every accepted client connection, a new thread is created using `pthread_create`.
     - The `handle_client` function is executed in the context of each thread, allowing concurrent handling of multiple clients.
     - The `pthread_detach` ensures the resources of the thread are automatically cleaned up after it completes execution, preventing memory leaks.

2. **Concurrency for File Operations**
   - Each client thread handles a single client’s request. The `handle_client()` function processes commands such as `WRITE`, `GET`, `RM`, and `REVERT`.

   - Example from `handle_client()`:
     ```c
     if (strncmp(buffer, "WRITE", 5) == 0) {
         handle_write_request(client_sock, buffer);
     } else if (strncmp(buffer, "GET", 3) == 0) {
         handle_get_request(client_sock, buffer);
     }
     ```
     - Each thread independently invokes the appropriate handler (`handle_write_request`, etc.) for the client's command.

3. **Suggestion for Further Enhancement**
   - **Why Mutex Locks May Be Necessary**:
     - If two clients write to or delete the same file simultaneously, there is a risk of data corruption.
   - **Example of Adding Mutex Locks**:
     - Introduce a `pthread_mutex_t` to lock critical regions:
       ```c
       pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

       void handle_write_request(int client_sock, const char *message) {
           pthread_mutex_lock(&file_mutex);
           // Perform file operations
           pthread_mutex_unlock(&file_mutex);
       }
       ```
     - This ensures only one thread can modify a file at a time, preserving data integrity.

---

### **What to Tell Your Professor**
- "The server spawns a new thread for every client connection using `pthread_create`. This implementation allows multiple clients to interact with the server concurrently.
- For critical file operations, we are considering using `pthread_mutex` to prevent race conditions if multiple clients access the same file simultaneously."

Let me know if you'd like a more detailed script for presenting this explanation!





### **Option 4a: 多客户端支持的实现**

#### **在代码中的体现**
1. **多客户端支持的核心代码位置**  
   - 实现多客户端支持的核心代码在 `main()` 函数中，服务器通过 `accept` 接受客户端连接后，为每个客户端创建一个新的线程。这部分代码如下：

```c
while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Accept failed");
        continue;
    }

    pthread_t client_thread;
    int *new_sock = malloc(sizeof(int)); // 为客户端 socket 分配内存
    *new_sock = client_sock;

    if (pthread_create(&client_thread, NULL, handle_client, new_sock) != 0) {
        perror("Thread creation failed");
        free(new_sock);
        continue;
    }

    pthread_detach(client_thread); // 确保线程完成后资源自动释放
}
```

   - **如何向教授解释：**
     - 每当服务器接收到客户端连接时，都会通过 `pthread_create` 创建一个新线程。
     - 每个线程独立执行 `handle_client` 函数来处理该客户端的请求，实现了并发处理。
     - 使用 `pthread_detach` 确保线程完成后释放资源，避免内存泄漏。

2. **文件操作的并发支持**  
   - 每个客户端线程只处理自己的请求，在 `handle_client()` 函数中按需求调用对应的命令处理函数。例如：
     ```c
     if (strncmp(buffer, "WRITE", 5) == 0) {
         handle_write_request(client_sock, buffer);
     } else if (strncmp(buffer, "GET", 3) == 0) {
         handle_get_request(client_sock, buffer);
     }
     ```
     - 通过这种方式，每个线程独立运行，互不干扰。

#### **进一步的优化建议**
1. **为何需要引入互斥锁：**
   - 如果两个客户端同时操作同一个文件（例如同时写入或删除），可能会引发数据竞争和文件损坏问题。
   - 解决方法是在关键代码区加入互斥锁（`pthread_mutex`），以确保同一时间只有一个线程可以操作该文件。

2. **如何实现互斥锁：**
   - 在文件操作的代码中加入锁机制：
     ```c
     pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

     void handle_write_request(int client_sock, const char *message) {
         pthread_mutex_lock(&file_mutex);
         // 执行文件操作
         pthread_mutex_unlock(&file_mutex);
     }
     ```
     - 在文件操作（例如 `WRITE` 或 `DELETE`）前加锁，操作完成后解锁，确保数据的完整性。

---

#### **如何向教授表达**
- "我们的服务器使用 `pthread_create` 为每个客户端连接创建一个独立的线程，允许多个客户端同时与服务器交互，体现了多客户端支持的能力。"
- "为进一步保证文件操作的安全性，我们计划使用 `pthread_mutex` 来为关键的文件操作区域添加锁，从而避免多个线程同时访问同一文件可能导致的数据竞争问题。"

如果需要详细的陈述内容，或如何在教授面前展示，随时告诉我！


### **测试多客户端功能的方法**

为了测试服务器是否正确支持多客户端并发，你可以通过以下步骤验证：

---

#### **1. 准备工具**
- **编译并启动服务器**  
  确保你的 `server` 已经编译，并可以正常运行：
  ```bash
  ./server
  ```

- **编译并准备客户端**  
  确保客户端可以通过不同的命令操作文件系统：
  ```bash
  ./client WRITE localfile1.txt remotefile1.txt
  ./client WRITE localfile2.txt remotefile2.txt
  ```

---

#### **2. 测试方法**

##### **方法 1: 手动并发测试**
1. **创建两个本地文件**
   创建两个不同的本地文件，用于测试并发写入：
   ```bash
   echo "Content for Client 1" > localfile1.txt
   echo "Content for Client 2" > localfile2.txt
   ```

2. **同时启动两个客户端**
   在两个终端分别运行以下命令，模拟两个客户端同时请求：
   ```bash
   ./client WRITE localfile1.txt remotefile1.txt
   ```
   ```bash
   ./client WRITE localfile2.txt remotefile2.txt
   ```

3. **验证文件写入**
   检查服务器目录中是否正确写入了两个文件：
   ```bash
   ls server_root/files/
   cat server_root/files/remotefile1.txt
   cat server_root/files/remotefile2.txt
   ```

##### **方法 2: 脚本并发测试**
1. **创建一个测试脚本**
   创建一个脚本，使用 `&` 来并发运行多个客户端：
   ```bash
   # test_concurrent.sh
   ./client WRITE localfile1.txt remotefile1.txt &
   ./client WRITE localfile2.txt remotefile2.txt &
   wait
   ```

2. **运行脚本**
   ```bash
   chmod +x test_concurrent.sh
   ./test_concurrent.sh
   ```

3. **检查结果**
   同样验证 `server_root/files/` 中的文件内容，确保两个客户端的写入都成功且没有互相覆盖。

---

#### **3. 引入竞争条件测试**

在没有锁的情况下，可能会出现数据竞争。为了观察这种情况，可以尝试以下步骤：

1. **重复写入同一个文件**
   同时让两个客户端写入相同的远程文件：
   ```bash
   ./client WRITE localfile1.txt sharedfile.txt &
   ./client WRITE localfile2.txt sharedfile.txt &
   wait
   ```

2. **观察结果**
   检查 `sharedfile.txt` 的内容是否被两个客户端的写入互相覆盖。如果文件内容不完整或被覆盖，说明需要添加互斥锁。

---

#### **4. 验证互斥锁效果**
1. **在服务器代码中添加互斥锁**  
   在 `handle_write_request` 和其他文件操作中加入 `pthread_mutex`。

2. **重新运行竞争测试**
   再次运行重复写入测试，确保 `sharedfile.txt` 的内容不会出现竞争问题。

---

#### **5. 使用压力测试工具**
可以借助工具进行更大规模的并发测试，例如：
- **`ab` (Apache Benchmark)**:
  - 示例命令：模拟 50 个客户端同时请求：
    ```bash
    ab -n 50 -c 10 -p payload.txt http://127.0.0.1:2024/
    ```
  - 将 `payload.txt` 设置为你的命令（如 `WRITE` 或 `GET`）。
  
- **`wrk`**:
  ```bash
  wrk -t4 -c50 -d10s http://127.0.0.1:2024/
  ```

通过上述方法，你可以手动或自动测试你的多客户端支持是否健壮，并验证是否正确处理并发请求。测试结果是多客户端正确响应且文件系统完整。



### **测试多客户端功能的方法**

为了测试服务器是否正确支持多客户端并发，你可以通过以下步骤验证：

---

#### **1. 准备工具**
- **编译并启动服务器**  
  确保你的 `server` 已经编译，并可以正常运行：
  ```bash
  ./server
  ```

- **编译并准备客户端**  
  确保客户端可以通过不同的命令操作文件系统：
  ```bash
  ./client WRITE localfile1.txt remotefile1.txt
  ./client WRITE localfile2.txt remotefile2.txt
  ```

---

#### **2. 测试方法**

##### **方法 1: 手动并发测试**
1. **创建两个本地文件**
   创建两个不同的本地文件，用于测试并发写入：
   ```bash
   echo "Content for Client 1" > localfile1.txt
   echo "Content for Client 2" > localfile2.txt
   ```

2. **同时启动两个客户端**
   在两个终端分别运行以下命令，模拟两个客户端同时请求：
   ```bash
   ./client WRITE localfile1.txt remotefile1.txt
   ```
   ```bash
   ./client WRITE localfile2.txt remotefile2.txt
   ```

3. **验证文件写入**
   检查服务器目录中是否正确写入了两个文件：
   ```bash
   ls server_root/files/
   cat server_root/files/remotefile1.txt
   cat server_root/files/remotefile2.txt
   ```

##### **方法 2: 脚本并发测试**
1. **创建一个测试脚本**
   创建一个脚本，使用 `&` 来并发运行多个客户端：
   ```bash
   # test_concurrent.sh
   ./client WRITE localfile1.txt remotefile1.txt &
   ./client WRITE localfile2.txt remotefile2.txt &
   wait
   ```

2. **运行脚本**
   ```bash
   chmod +x test_concurrent.sh
   ./test_concurrent.sh
   ```

3. **检查结果**
   同样验证 `server_root/files/` 中的文件内容，确保两个客户端的写入都成功且没有互相覆盖。

---

#### **3. 引入竞争条件测试**

在没有锁的情况下，可能会出现数据竞争。为了观察这种情况，可以尝试以下步骤：

1. **重复写入同一个文件**
   同时让两个客户端写入相同的远程文件：
   ```bash
   ./client WRITE localfile1.txt sharedfile.txt &
   ./client WRITE localfile2.txt sharedfile.txt &
   wait
   ```

2. **观察结果**
   检查 `sharedfile.txt` 的内容是否被两个客户端的写入互相覆盖。如果文件内容不完整或被覆盖，说明需要添加互斥锁。

---

#### **4. 验证互斥锁效果**
1. **在服务器代码中添加互斥锁**  
   在 `handle_write_request` 和其他文件操作中加入 `pthread_mutex`。

2. **重新运行竞争测试**
   再次运行重复写入测试，确保 `sharedfile.txt` 的内容不会出现竞争问题。

---

#### **5. 使用压力测试工具**
可以借助工具进行更大规模的并发测试，例如：
- **`ab` (Apache Benchmark)**:
  - 示例命令：模拟 50 个客户端同时请求：
    ```bash
    ab -n 50 -c 10 -p payload.txt http://127.0.0.1:2024/
    ```
  - 将 `payload.txt` 设置为你的命令（如 `WRITE` 或 `GET`）。
  
- **`wrk`**:
  ```bash
  wrk -t4 -c50 -d10s http://127.0.0.1:2024/
  ```

通过上述方法，你可以手动或自动测试你的多客户端支持是否健壮，并验证是否正确处理并发请求。测试结果是多客户端正确响应且文件系统完整。


### 你实现的功能总结

---

#### **1. 日志功能 (Log)**

**实现内容：**
- 每次操作（如 `WRITE`、`GET`、`RM`、`REVERT`）都会记录日志。
- 日志文件保存在服务器目录下的 `server_root/version.log` 文件中。
- 记录的日志包括以下信息：
  - 时间戳
  - 操作类型 (`WRITE`, `GET`, `RM`, `REVERT`)
  - 操作的文件路径

**代码参考：**
```c
void log_version_change(const char *operation, const char *file_path)
{
    FILE *log = fopen(LOG_FILE, "a");
    if (!log)
    {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log, "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d] %s: %s\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            operation, file_path);

    fclose(log);
}
```

**日志示例：**
```
[2024-11-27 20:30:15] WRITE: server_root/files/remotefile.txt
[2024-11-27 20:32:10] RM: server_root/files/remotefile.txt
[2024-11-27 20:34:45] REVERT: server_root/files/remotefile.txt
```

**优点：**
- **审计追踪**：可追踪所有文件操作。
- **调试**：便于排查错误。

**如何展示给教授：**
- 告诉教授日志的存储路径和格式。
- 提供样例日志，说明可以帮助调试和记录文件系统历史。

---

#### **2. 版本控制与文件恢复 (Version Control & REVERT)**

**实现内容：**
- 每次文件修改或删除前，会自动创建备份文件。
- 备份文件保存在 `server_root/backups/` 目录下。
- 使用 `REVERT` 命令，可以从备份中恢复文件。

**备份机制：**
- 备份文件的命名规则为：
  ```
  server_root/backups/<文件名>
  ```
- 使用以下函数实现备份：
```c
void create_backup_copy(const char *file_path)
{
    char backup_path[512];
    const char *base_name = strrchr(file_path, '/');
    base_name = (base_name ? base_name + 1 : file_path);

    snprintf(backup_path, sizeof(backup_path), "%s%s", BACKUPS_DIR, base_name);

    FILE *original = fopen(file_path, "r");
    if (!original)
    {
        printf("No original file to backup: %s\n", file_path);
        return;
    }

    FILE *backup = fopen(backup_path, "w");
    if (!backup)
    {
        perror("Failed to create backup file");
        fclose(original);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), original)) > 0)
    {
        fwrite(buffer, 1, bytes, backup);
    }

    fclose(original);
    fclose(backup);

    printf("Backup copy created: %s\n", backup_path);
}
```

**恢复机制：**
- 使用 `REVERT` 命令，可以从 `server_root/backups/` 目录中恢复文件：
```c
void handle_revert_request(int client_sock, const char *message)
{
    char remote_file_name[256];
    if (sscanf(message, "REVERT %255s", remote_file_name) != 1)
    {
        send(client_sock, "ERROR: Invalid REVERT command\n", 30, 0);
        return;
    }

    char file_path[512], backup_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);
    snprintf(backup_path, sizeof(backup_path), "%s%s", BACKUPS_DIR, remote_file_name);

    FILE *backup = fopen(backup_path, "r");
    if (!backup)
    {
        send(client_sock, "ERROR: Backup not found\n", 25, 0);
        return;
    }

    FILE *file = fopen(file_path, "w");
    if (!file)
    {
        fclose(backup);
        send(client_sock, "ERROR: Failed to restore file\n", 30, 0);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), backup)) > 0)
    {
        fwrite(buffer, 1, bytes, file);
    }

    fclose(backup);
    fclose(file);

    send(client_sock, "File reverted successfully\n", 28, 0);
    log_version_change("REVERT", file_path);
}
```

**如何展示给教授：**
1. **演示备份**：
   - 修改文件前展示备份目录 `server_root/backups/` 是空的。
   - 修改或删除文件后展示备份文件已生成。

2. **演示恢复**：
   - 删除文件后，使用 `REVERT` 恢复文件。
   - 展示恢复后的文件内容与原始文件一致。

---

#### **功能优点总结**

- **日志**：记录所有操作，便于调试和审计。
- **版本控制与恢复**：
  - 避免因误操作导致的数据丢失。
  - 提供历史记录，可以随时回滚到安全版本。
  
通过这两个功能，你不仅满足了基本需求，还为系统增加了重要的容错和可维护性能力，展示了扩展性的优秀设计理念。