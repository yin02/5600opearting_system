#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check platform: use Winsock for Windows and standard socket API for other OS
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#define CLOSESOCKET closesocket    // For Windows, use closesocket to close sockets / 在Windows上使用closesocket关闭socket
#pragma comment(lib, "ws2_32.lib") // Link Winsock library for Windows / 在Windows上链接Winsock库
#else
#include <unistd.h>
#include <arpa/inet.h>    // POSIX socket API for other platforms / 在其他平台使用POSIX的socket API
#define CLOSESOCKET close // For Unix-like systems, use close to close sockets / 在类Unix系统中使用close关闭socket
#endif

#define BUFFER_SIZE 4096 // Define buffer size as 4KB / 定义缓冲区大小为4KB
#define PORT 2024        // Define server port / 定义服务器端口

// Send WRITE command and write file content to the server
// 发送WRITE命令，并将文件内容写入服务器
void send_write_command(int socket_desc, const char *local_file, const char *remote_file)
{
  FILE *file = fopen(local_file, "r"); // Open the local file for reading / 打开本地文件进行读取
  if (!file)
  {
    perror("File open error"); // If file opening fails, print error / 如果文件打开失败，输出错误信息
    return;
  }

  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "WRITE %s ", remote_file); // Format WRITE command / 格式化WRITE命令

  // Read the file content into message buffer / 将文件内容读取到消息缓冲区
  char file_data[BUFFER_SIZE];
  fread(file_data, 1, sizeof(file_data) - 1, file); // Read file data / 读取文件数据
  fclose(file);

  strcat(message, file_data); // Append file content to the command / 将文件内容附加到命令后面

  // Send the command and file content / 发送命令及文件内容
  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed"); // If send fails, print error / 如果发送失败，输出错误信息
    return;
  }

  printf("WRITE command sent: %s\n", message); // Print sent message / 输出发送的命令信息

  // Receive server's response / 接收服务器的响应
  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';                 // Null-terminate the received data / 添加字符串结束符
  printf("Server response: %s\n", server_reply); // Print server's reply / 输出服务器的回复
}

// Send GET command and receive file
// 发送GET命令并接收文件
void send_get_command(int socket_desc, const char *remote_file, const char *local_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "GET %s", remote_file); // Format GET command / 格式化GET命令

  // Send GET command / 发送GET命令
  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("GET command sent: %s\n", message); // Print sent message / 输出发送的命令信息

  // Open the local file to save received data / 打开本地文件以保存接收到的数据
  FILE *file = fopen(local_file, "w");
  if (!file)
  {
    perror("File open error");
    return;
  }

  char server_reply[BUFFER_SIZE];
  int received;
  // Receive and write file content to the local file / 接收并将文件内容写入本地文件
  while ((received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0)) > 0)
  {
    fwrite(server_reply, 1, received, file); // Write received data to the file / 将接收到的数据写入文件
  }

  fclose(file);
  printf("File retrieved and saved to %s\n", local_file); // Finished receiving file / 文件接收完成并保存
}

// Send RM command to remove remote file
// 发送RM命令删除远程文件
void send_rm_command(int socket_desc, const char *remote_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "RM %s", remote_file); // Format RM command / 格式化RM命令

  // Send remove command / 发送删除命令
  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("RM command sent: %s\n", message); // Print sent message / 输出发送的命令信息

  // Receive server's response / 接收服务器的响应
  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server response: %s\n", server_reply); // Print server's reply / 输出服务器的回复
}

// Send REVERT command to restore remote file
// 发送REVERT命令恢复远程文件
void send_revert_command(int socket_desc, const char *remote_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "REVERT %s", remote_file); // Format REVERT command / 格式化REVERT命令

  // Send revert command / 发送恢复命令
  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("REVERT command sent: %s\n", message); // Print sent message / 输出发送的命令信息

  // Receive server's response / 接收服务器的响应
  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server response: %s\n", server_reply); // Print server's reply / 输出服务器的回复
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
  // Initialize Winsock for Windows / 在Windows中初始化Winsock
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
    return 1; // Exit the program if Winsock initialization fails / 如果Winsock初始化失败，则退出程序
  }
#endif

  // argc 在这里的作用：
  // 参数验证：

  // 程序检查 argc 是否 小于 3，用来验证用户是否传递了足够的参数以执行命令（如 WRITE、GET、RM 或 REVERT）。
  // 程序至少需要 3 个参数：
  // 程序名称（即 argv[0]）。
  // 命令（如 WRITE、GET、RM 或 REVERT）。
  // 必需的文件（例如本地文件和远程文件）。

  if (argc < 3)
  {
    printf("Usage:\n");
    printf("  %s WRITE <local_file> <remote_file>\n", argv[0]);
    printf("  %s GET <remote_file> <local_file>\n", argv[0]);
    printf("  %s RM <remote_file>\n", argv[0]);
    printf("  %s REVERT <remote_file>\n", argv[0]);
    return 1; // If invalid arguments, print usage information and exit / 如果命令行参数无效，则输出使用信息并退出
  }

  // Create socket / 创建socket
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0)
  {
    perror("Socket creation failed"); // Print error if socket creation fails / 如果创建socket失败，输出错误信息
    return 1;                         // Exit the program if socket creation fails / 如果创建socket失败，退出程序
  }

  // Set up server address / 设置服务器地址
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);                   // Set the port number (converted to network byte order) / 设置端口号（转换为网络字节顺序）
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server address (localhost in this case) / 服务器地址（此处为localhost）

  // Connect to the server / 连接到服务器
  if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Connect failed"); // Print error if connection fails / 如果连接失败，输出错误信息
    CLOSESOCKET(socket_desc); // Close socket / 关闭socket
#ifdef _WIN32
    WSACleanup(); // Clean up Winsock if using Windows / 如果使用Windows，清理Winsock
#endif
    return 1; // Exit the program if connection fails / 如果连接失败，退出程序
  }

  // Execute corresponding function based on the command / 根据命令执行相应的函数
  if (strcmp(argv[1], "WRITE") == 0 && argc == 4)
  {
    send_write_command(socket_desc, argv[2], argv[3]);
  }
  else if (strcmp(argv[1], "GET") == 0 && argc == 4)
  {
    send_get_command(socket_desc, argv[2], argv[3]);
  }
  else if (strcmp(argv[1], "RM") == 0 && argc == 3)
  {
    send_rm_command(socket_desc, argv[2]);
  }
  else if (strcmp(argv[1], "REVERT") == 0 && argc == 3)
  {
    send_revert_command(socket_desc, argv[2]);
  }
  else
  {
    printf("Invalid command or arguments.\n"); // If invalid command, print error message / 如果命令无效，输出错误信息
  }

  // Close socket connection / 关闭socket连接
  CLOSESOCKET(socket_desc);
#ifdef _WIN32
  WSACleanup(); // Clean up Winsock if using Windows / 如果使用Windows，清理Winsock
#endif
  return 0; // Exit the program successfully / 程序成功退出
}
