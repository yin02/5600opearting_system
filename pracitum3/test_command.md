Here’s how to test the **server-client interaction** and verify all functionalities:

---

### **1. Compile the Server and Client**

Compile the `server.c` and `client.c` files:

```bash
gcc -Wall -Wextra -pthread -o server server.c
gcc -Wall -Wextra -o client client.c
```

---

### **2. Start the Server**

Run the server in one terminal:
```bash
./server
```

Expected output on the console:
```
Server listening on port 2024...
```

---

### **3. Client Interactions**

In a separate terminal, use the client to send commands to the server.

#### **WRITE Command**

1. **Create a Local File**:
   ```bash
   echo "This is a test file." > localfile.txt
   ```

2. **Send the File to the Server**:
   ```bash
   ./client WRITE localfile.txt remotefile.txt
   ```

3. **Server Console Output**:
   ```
   Received: WRITE remotefile.txt
   ```

4. **Verify on the Server**:
   ```bash
   ls server_root/files/
   cat server_root/files/remotefile.txt
   ```
   Expected output:
   ```
   remotefile.txt
   This is a test file.
   ```

5. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: WRITE remotefile.txt
   File written successfully: server_root/files/remotefile.txt
   ```

---

#### **GET Command**

1. **Download the File from the Server**:
   ```bash
   ./client GET remotefile.txt downloadedfile.txt
   ```

2. **Verify on the Client**:
   ```bash
   cat downloadedfile.txt
   ```
   Expected output:
   ```
   This is a test file.
   ```

3. **Server Console Output**:
   ```
   Received: GET remotefile.txt
   ```

4. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: GET remotefile.txt
   File sent successfully: server_root/files/remotefile.txt
   ```

---

#### **RM Command**

1. **Delete the File on the Server**:
   ```bash
   ./client RM remotefile.txt
   ```

2. **Verify the File Is Deleted**:
   ```bash
   ls server_root/files/
   ```
   Expected output:
   ```
   (No files found)
   ```

3. **Server Console Output**:
   ```
   Received: RM remotefile.txt
   ```

4. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: RM remotefile.txt
   File deleted successfully: server_root/files/remotefile.txt
   ```

---

#### **LS Command**

1. **List Files in the Server Directory**:
   ```bash
   ./client LS
   ```

2. **Verify Output on the Client**:
   Expected output:
   ```
   (No files found)
   ```

3. **Server Console Output**:
   ```
   Received: LS
   ```

4. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: LS
   Directory listing sent successfully.
   ```

---

### **4. Error Handling Tests**

#### **Invalid Command**

1. **Send an Invalid Command**:
   ```bash
   ./client INVALID remotefile.txt
   ```

2. **Client Output**:
   ```
   ERROR: Unsupported command
   ```

3. **Server Console Output**:
   ```
   Received: INVALID remotefile.txt
   ```

4. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: INVALID remotefile.txt
   Unsupported command received: INVALID remotefile.txt
   ```

---

#### **File Not Found**

1. **Attempt to GET a Non-existent File**:
   ```bash
   ./client GET nonexistent.txt downloadedfile.txt
   ```

2. **Client Output**:
   ```
   ERROR: File not found
   ```

3. **Server Console Output**:
   ```
   Received: GET nonexistent.txt
   ```

4. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: GET nonexistent.txt
   File not found: server_root/files/nonexistent.txt
   ```

5. **Attempt to RM a Non-existent File**:
   ```bash
   ./client RM nonexistent.txt
   ```

6. **Client Output**:
   ```
   ERROR: Failed to delete file
   ```

7. **Check the Log File**:
   ```bash
   cat server_root/server.log
   ```
   Expected log:
   ```
   Received: RM nonexistent.txt
   Failed to delete file: server_root/files/nonexistent.txt
   ```

---

### **5. Simultaneous Clients**

Open multiple terminals and run concurrent commands to test threading and concurrency.

Example:

- **Terminal 1**:
  ```bash
  ./client WRITE localfile1.txt remotefile1.txt
  ```

- **Terminal 2**:
  ```bash
  ./client GET remotefile1.txt downloadedfile1.txt
  ```

- **Terminal 3**:
  ```bash
  ./client LS
  ```

Verify that:
- Commands execute without blocking or errors.
- Log file records all operations correctly.

---

### **Final Log Check**

After all tests, verify the `server_root/server.log` contains a complete record of all operations.

```bash
cat server_root/server.log
```

---

This comprehensive testing ensures the server and client handle all operations and edge cases correctly. Let me know if you encounter any issues!