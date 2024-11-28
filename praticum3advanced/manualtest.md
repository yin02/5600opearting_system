### **Step-by-Step Manual Testing Guide**

Here’s how you can manually test your server and client to ensure all functionalities (WRITE, GET, RM, REVERT) work as expected.

---

### **1. Start the Server**

#### Command:
```bash
./server
```

#### Expected Output:
```plaintext
Server listening on port 2024...
```

If the port is in use, terminate existing processes with:
```bash
lsof -ti :2024 | xargs kill -9
```

Then, start the server again.

---

### **2. Test WRITE Command**

#### Purpose:
Send a file from the client to the server.

#### Steps:
1. Create a local file on the client side:
   ```bash
   echo "This is test content for WRITE." > localfile.txt
   ```

2. Run the client to send this file to the server:
   ```bash
   ./client WRITE localfile.txt remotefile.txt
   ```

#### Expected Server Logs:
```plaintext
Received WRITE command for file: remotefile.txt
File written successfully: server_root/files/remotefile.txt
```

#### Expected Client Output:
```plaintext
Server response: File written successfully
```

#### Verify:
1. Check if the file exists on the server:
   ```bash
   cat server_root/files/remotefile.txt
   ```
   Expected content:
   ```plaintext
   This is test content for WRITE.
   ```

---

### **3. Test GET Command**

#### Purpose:
Retrieve a file from the server to the client.

#### Steps:
1. Run the client to fetch the file from the server:
   ```bash
   ./client GET remotefile.txt downloadedfile.txt
   ```

#### Expected Server Logs:
```plaintext
Received GET command for file: remotefile.txt
Sending file data for: server_root/files/remotefile.txt
```

#### Expected Client Output:
```plaintext
File retrieved and saved to downloadedfile.txt
```

#### Verify:
1. Check the downloaded file on the client:
   ```bash
   cat downloadedfile.txt
   ```
   Expected content:
   ```plaintext
   This is test content for WRITE.
   ```

---

### **4. Test RM Command**

#### Purpose:
Delete a file on the server and create a backup.

#### Steps:
1. Run the client to delete the file on the server:
   ```bash
   ./client RM remotefile.txt
   ```

#### Expected Server Logs:
```plaintext
Received RM command for file: remotefile.txt
Backup copy created: server_root/backups/remotefile.txt
File deleted successfully: server_root/files/remotefile.txt
```

#### Expected Client Output:
```plaintext
Server response: File deleted successfully
```

#### Verify:
1. Check if the file has been removed from the server:
   ```bash
   ls server_root/files/remotefile.txt
   ```
   Expected output:
   ```plaintext
   ls: cannot access 'server_root/files/remotefile.txt': No such file or directory
   ```

2. Check the backup directory for the backup file:
   ```bash
   ls server_root/backups/
   ```
   Expected output:
   ```plaintext
   remotefile.txt
   ```

---

### **5. Test REVERT Command**

#### Purpose:
Restore a file from the backup on the server.

#### Steps:
1. Run the client to revert the file on the server:
   ```bash
   ./client REVERT remotefile.txt
   ```

#### Expected Server Logs:
```plaintext
Received REVERT command for file: remotefile.txt
File restored successfully: server_root/files/remotefile.txt
```

#### Expected Client Output:
```plaintext
Server response: File reverted successfully
```

#### Verify:
1. Check if the file is restored on the server:
   ```bash
   cat server_root/files/remotefile.txt
   ```
   Expected content:
   ```plaintext
   This is test content for WRITE.
   ```

---

### **6. View Server Logs**

To review all operations logged by the server:
```bash
cat server_root/version.log
```

Example log content:
```plaintext
[2024-11-27 20:15:30] WRITE: remotefile.txt
[2024-11-27 20:16:00] GET: remotefile.txt
[2024-11-27 20:16:30] RM: remotefile.txt
[2024-11-27 20:17:00] REVERT: remotefile.txt
```



## or auto test


``` bash
make
```

```bash
chmod +x test_remote_file_system.sh
./test_remote_file_system.sh
```



