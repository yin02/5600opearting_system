The error `Bind failed: Address already in use` occurs because the server process is already running or the port (2024) is still occupied by a previous instance. Follow these steps to resolve the issue:

---

### **Steps to Resolve**

#### **1. Check if the Server Process is Already Running**
Run the following command to find the process using port 2024:
```bash
lsof -i :2024
```

If the server is running, you will see output like this:
```plaintext
COMMAND   PID    USER   FD   TYPE  DEVICE SIZE/OFF NODE NAME
server   12345   user   3u   IPv4  123456      0t0  TCP *:2024 (LISTEN)
```

#### **2. Kill the Running Server Process**
Terminate the process using the PID (e.g., `12345`):
```bash
kill -9 12345
lsof -ti :2024 | xargs kill -9

```

