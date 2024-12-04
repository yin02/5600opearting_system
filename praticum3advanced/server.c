#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <direct.h>                    // For _mkdir / 用于 _mkdir 创建目录
#define mkdir(path, mode) _mkdir(path) // Ignore mode for Windows / 在Windows中忽略mode参数
typedef int socklen_t;                 // Defining socklen_t for Windows / 为Windows定义socklen_t
#define CLOSESOCKET closesocket        // Define CLOSESOCKET for closing the socket / 定义关闭socket的函数
#pragma comment(lib, "ws2_32.lib")     // Link to Winsock library / 链接到Winsock库
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#define CLOSESOCKET close // Define CLOSESOCKET for Unix-like systems / 为类Unix系统定义关闭socket的函数
#endif

#define BUFFER_SIZE 4096                   // Define buffer size for file transfers / 定义文件传输缓冲区的大小
#define PORT 2024                          // Define the server port / 定义服务器的端口号
#define ROOT_DIR "server_root/"            // Root directory for server files / 服务器文件的根目录
#define FILES_DIR "server_root/files/"     // Directory for storing files / 存储文件的目录
#define BACKUPS_DIR "server_root/backups/" // Directory for storing backups / 存储备份的目录
#define LOG_FILE "server_root/version.log" // Log file for version changes / 用于记录版本变化的日志文件

// Log changes / 记录文件操作日志
void log_version_change(const char *operation, const char *file_path)
{
  FILE *log = fopen(LOG_FILE, "a"); // Open the log file in append mode / 以追加模式打开日志文件
  if (!log)
  {
    perror("Failed to open log file"); // If opening the log file fails / 如果打开日志文件失败
    return;
  }

  time_t now = time(NULL);                                 // Get the current time / 获取当前时间
  struct tm *t = localtime(&now);                          // Convert to local time / 转换为本地时间
  fprintf(log, "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d] %s: %s\n", // Write formatted log entry with timestamp / 使用时间戳格式化日志条目并写入
          t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
          t->tm_hour, t->tm_min, t->tm_sec,
          operation, file_path);

  fclose(log); // Close the log file / 关闭日志文件
}

// Create backup / 创建文件备份
void create_backup_copy(const char *file_path)
{
  char backup_path[512];
  const char *base_name = strrchr(file_path, '/');     // Find the base file name / 获取文件的基本名称
  base_name = (base_name ? base_name + 1 : file_path); // Adjust if path exists, else use full file name / 如果路径存在，则调整，否则使用完整的文件名

  snprintf(backup_path, sizeof(backup_path), "%s%s", BACKUPS_DIR, base_name); // Format the backup path / 格式化备份路径

  FILE *original = fopen(file_path, "r"); // Open the original file for reading / 打开原始文件进行读取
  if (!original)
  {
    printf("No original file to backup: %s\n", file_path); // Notify if original file doesn't exist / 如果原始文件不存在，则通知
    return;
  }

  FILE *backup = fopen(backup_path, "w"); // Create the backup file / 创建备份文件
  if (!backup)
  {
    perror("Failed to create backup file"); // If backup file creation fails / 如果创建备份文件失败
    fclose(original);
    return;
  }

  char buffer[BUFFER_SIZE];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), original)) > 0) // Read from the original file and write to backup / 从原始文件读取并写入备份
  {
    fwrite(buffer, 1, bytes, backup);
  }

  fclose(original); // Close both the original and backup files / 关闭原始文件和备份文件
  fclose(backup);

  printf("Backup copy created: %s\n", backup_path); // Inform the user that the backup was successful / 通知用户备份已成功创建
}

// Handle WRITE command / 处理WRITE命令
void handle_write_request(int client_sock, const char *message)
{
  char remote_file_name[256], file_data[BUFFER_SIZE];

  const char *file_name_start = strstr(message, "WRITE "); // Find the start of the WRITE command / 查找WRITE命令的起始位置
  if (!file_name_start)
  {
    send(client_sock, "ERROR: Invalid WRITE command\n", 29, 0); // Send error message if command is invalid / 如果命令无效，则发送错误信息
    return;
  }

  file_name_start += strlen("WRITE ");
  char *file_content_start = strchr(file_name_start, ' '); // Find the space between the file name and content / 查找文件名和内容之间的空格
  if (!file_content_start)
  {
    send(client_sock, "ERROR: Missing file content\n", 29, 0); // Send error if file content is missing / 如果文件内容缺失，则发送错误信息
    return;
  }

  size_t file_name_length = file_content_start - file_name_start; // Extract the file name length / 提取文件名长度
  strncpy(remote_file_name, file_name_start, file_name_length);
  remote_file_name[file_name_length] = '\0';
  strncpy(file_data, file_content_start + 1, sizeof(file_data) - 1); // Extract file content / 提取文件内容
  file_data[sizeof(file_data) - 1] = '\0';

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name); // Format the file path / 格式化文件路径

  create_backup_copy(file_path); // Create a backup before writing / 写入之前先创建备份

  FILE *file = fopen(file_path, "w"); // Open the file for writing / 打开文件进行写入
  if (!file)
  {
    send(client_sock, "ERROR: Could not save file\n", 27, 0); // Send error if file couldn't be saved / 如果文件无法保存，则发送错误信息
    return;
  }

  fprintf(file, "%s", file_data); // Write the file content / 写入文件内容
  fclose(file);

  send(client_sock, "File written successfully\n", 26, 0); // Send success message to the client / 向客户端发送成功消息
  log_version_change("WRITE", file_path);                  // Log the write operation / 记录写入操作
}

// Handle GET command / 处理GET命令
void handle_get_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "GET %255s", remote_file_name) != 1) // Parse the file name from the GET command / 从GET命令中解析文件名
  {
    send(client_sock, "ERROR: Invalid GET command\n", 27, 0); // Send error message if GET command is invalid / 如果GET命令无效，则发送错误信息
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name); // Format the file path / 格式化文件路径

  FILE *file = fopen(file_path, "r"); // Open the file for reading / 打开文件进行读取
  if (!file)
  {
    send(client_sock, "ERROR: File not found\n", 22, 0); // Send error if file not found / 如果文件未找到，则发送错误信息
    return;
  }

  char buffer[BUFFER_SIZE];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) // Read file content and send it to the client / 读取文件内容并发送给客户端
  {
    send(client_sock, buffer, bytes_read, 0);
  }

  fclose(file);
}

// Handle RM command / 处理RM命令
void handle_delete_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "RM %255s", remote_file_name) != 1) // Parse the file name from RM command / 从RM命令中解析文件名
  {
    send(client_sock, "ERROR: Invalid RM command\n", 26, 0); // Send error if RM command is invalid / 如果RM命令无效，则发送错误信息
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name); // Format the file path / 格式化文件路径

  create_backup_copy(file_path); // Create backup before deletion / 删除之前先创建备份

  if (remove(file_path) == 0) // Delete the file / 删除文件
  {
    send(client_sock, "File deleted successfully\n", 26, 0); // Notify client of successful deletion / 通知客户端文件已删除
    log_version_change("RM", file_path);                     // Log the deletion / 记录删除操作
  }
  else
  {
    send(client_sock, "ERROR: Failed to delete file\n", 29, 0); // Send error if deletion failed / 如果删除失败，则发送错误信息
  }
}

// Handle REVERT command / 处理REVERT命令
void handle_revert_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "REVERT %255s", remote_file_name) != 1) // Parse the file name from REVERT command / 从REVERT命令中解析文件名
  {
    send(client_sock, "ERROR: Invalid REVERT command\n", 30, 0); // Send error if REVERT command is invalid / 如果REVERT命令无效，则发送错误信息
    return;
  }

  char file_path[512], backup_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);
  snprintf(backup_path, sizeof(backup_path), "%s%s", BACKUPS_DIR, remote_file_name); // Format the file and backup paths / 格式化文件路径和备份路径

  FILE *backup = fopen(backup_path, "r"); // Open backup file for reading / 打开备份文件进行读取
  if (!backup)
  {
    send(client_sock, "ERROR: Backup not found\n", 25, 0); // Send error if backup doesn't exist / 如果备份文件不存在，则发送错误信息
    return;
  }

  FILE *file = fopen(file_path, "w"); // Open the file for writing / 打开文件进行写入
  if (!file)
  {
    fclose(backup);
    send(client_sock, "ERROR: Failed to restore file\n", 30, 0); // Send error if file can't be restored / 如果无法恢复文件，则发送错误信息
    return;
  }

  char buffer[BUFFER_SIZE];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), backup)) > 0) // Read from backup and write to the original file / 从备份读取并写入到原始文件
  {
    fwrite(buffer, 1, bytes, file);
  }

  fclose(backup); // Close both backup and original files / 关闭备份文件和原始文件
  fclose(file);

  send(client_sock, "File reverted successfully\n", 28, 0); // Notify client that the file was reverted successfully / 通知客户端文件已恢复
  log_version_change("REVERT", file_path);                  // Log the revert operation / 记录恢复操作
}

// Handle client connection / 处理客户端连接
void *handle_client(void *arg)
{
  int client_sock = *(int *)arg;
  free(arg);

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));
  if (recv(client_sock, buffer, sizeof(buffer), 0) > 0)
  {
    if (strncmp(buffer, "WRITE", 5) == 0)
    {
      handle_write_request(client_sock, buffer); // Call handle for WRITE command / 调用WRITE命令处理函数
    }
    else if (strncmp(buffer, "GET", 3) == 0)
    {
      handle_get_request(client_sock, buffer); // Call handle for GET command / 调用GET命令处理函数
    }
    else if (strncmp(buffer, "RM", 2) == 0)
    {
      handle_delete_request(client_sock, buffer); // Call handle for RM command / 调用RM命令处理函数
    }
    else if (strncmp(buffer, "REVERT", 6) == 0)
    {
      handle_revert_request(client_sock, buffer); // Call handle for REVERT command / 调用REVERT命令处理函数
    }
    else
    {
      send(client_sock, "ERROR: Unsupported command\n", 28, 0); // Unsupported command / 不支持的命令
    }
  }

  CLOSESOCKET(client_sock); // Close client socket / 关闭客户端的socket连接
  return NULL;
}

int main()
{
#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // Initialize Winsock on Windows / 在Windows上初始化Winsock
  {
    return -1; // Return error if initialization fails / 如果初始化失败，返回错误
  }
#endif

  mkdir(ROOT_DIR, 0755);    // Create the root directory / 创建根目录
  mkdir(FILES_DIR, 0755);   // Create the files directory / 创建文件目录
  mkdir(BACKUPS_DIR, 0755); // Create the backups directory / 创建备份目录

  int server_sock = socket(AF_INET, SOCK_STREAM, 0); // Create the server socket / 创建服务器socket
  if (server_sock < 0)
  {
    return -1; // If socket creation fails, return error / 如果socket创建失败，返回错误
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT); // Set server port / 设置服务器端口
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) // Bind the socket to the address / 将socket绑定到地址
  {
    return -1; // Return error if bind fails / 如果绑定失败，返回错误
  }

  listen(server_sock, 10); // Listen for incoming connections / 监听传入的连接

  printf("Server listening on port %d...\n", PORT); // Print server listening message / 打印服务器监听的消息

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len); // Accept incoming client connection / 接受传入的客户端连接
    if (client_sock < 0)
    {
      continue; // If accepting client fails, continue / 如果接受客户端连接失败，则继续
    }

    pthread_t client_thread;
    int *new_sock = malloc(sizeof(int));
    *new_sock = client_sock;

    pthread_create(&client_thread, NULL, handle_client, new_sock); // Create a thread for the client / 为客户端创建线程
    pthread_detach(client_thread);                                 // Detach the thread to handle client asynchronously / 分离线程以异步处理客户端
  }

  CLOSESOCKET(server_sock); // Close the server socket / 关闭服务器socket
#ifdef _WIN32
  WSACleanup(); // Clean up Winsock for Windows / 清理Windows的Winsock资源
#endif
  return 0;
}
