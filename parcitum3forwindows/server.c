#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <direct.h>                    // For _mkdir
#define mkdir(path, mode) _mkdir(path) // Ignore mode for Windows
typedef int socklen_t;
#define CLOSESOCKET closesocket
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#define CLOSESOCKET close
#endif

#define BUFFER_SIZE 4096
#define PORT 2024
#define ROOT_DIR "server_root/"
#define FILES_DIR "server_root/files/"
#define VERSIONS_DIR "server_root/versions/"
#define LOG_FILE "server_root/version.log"

FILE *log_file;

// Utility function to log operations
void log_version_change(const char *operation, const char *file_path)
{
  FILE *log = fopen(LOG_FILE, "a");
  if (!log)
    return;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  fprintf(log, "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d] Operation: %s %s\n",
          t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
          t->tm_hour, t->tm_min, t->tm_sec,
          operation, file_path);
  fclose(log);
}

// Backup a file before modifying or deleting
void backup_file(const char *file_path)
{
  char backup_path[512];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  snprintf(backup_path, sizeof(backup_path), "%s%s.%.4d%.2d%.2d%.2d%.2d%.2d",
           VERSIONS_DIR, strrchr(file_path, '/') + 1,
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec);

  FILE *original = fopen(file_path, "r");
  if (!original)
    return;

  FILE *backup = fopen(backup_path, "w");
  if (!backup)
  {
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
}

// Handle WRITE command
void handle_write_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  char file_data[BUFFER_SIZE];

  const char *file_name_start = strstr(message, "WRITE ");
  if (!file_name_start)
  {
    send(client_sock, "ERROR: Invalid WRITE command\n", 29, 0);
    return;
  }

  file_name_start += strlen("WRITE ");
  char *file_content_start = strchr(file_name_start, ' ');
  if (!file_content_start)
  {
    send(client_sock, "ERROR: Missing file content\n", 29, 0);
    return;
  }

  size_t file_name_length = file_content_start - file_name_start;
  strncpy(remote_file_name, file_name_start, file_name_length);
  remote_file_name[file_name_length] = '\0';
  strncpy(file_data, file_content_start + 1, sizeof(file_data) - 1);
  file_data[sizeof(file_data) - 1] = '\0';

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  backup_file(file_path);

  FILE *file = fopen(file_path, "w");
  if (!file)
  {
    send(client_sock, "ERROR: Could not save file\n", 27, 0);
    return;
  }

  fprintf(file, "%s", file_data);
  fclose(file);

  send(client_sock, "File written successfully\n", 26, 0);
  log_version_change("WRITE", file_path);
}

// Handle GET command
void handle_get_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "GET %255s", remote_file_name) != 1)
  {
    send(client_sock, "ERROR: Invalid GET command\n", 27, 0);
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  FILE *file = fopen(file_path, "r");
  if (!file)
  {
    send(client_sock, "ERROR: File not found\n", 22, 0);
    return;
  }

  char buffer[BUFFER_SIZE];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
  {
    send(client_sock, buffer, bytes_read, 0);
  }

  fclose(file);
}

// Handle RM command
void handle_delete_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "RM %255s", remote_file_name) != 1)
  {
    send(client_sock, "ERROR: Invalid RM command\n", 26, 0);
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  backup_file(file_path);

  if (remove(file_path) == 0)
  {
    send(client_sock, "File deleted successfully\n", 26, 0);
    log_version_change("RM", file_path);
  }
  else
  {
    send(client_sock, "ERROR: Failed to delete file\n", 29, 0);
  }
}

// Handle REVERT command
void handle_revert_request(int client_sock, const char *message)
{
  char remote_file_name[256], version[64];
  if (sscanf(message, "REVERT %255s %63s", remote_file_name, version) != 2)
  {
    send(client_sock, "ERROR: Invalid REVERT command\n", 30, 0);
    return;
  }

  char file_path[512], backup_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);
  snprintf(backup_path, sizeof(backup_path), "%s%s.%s", VERSIONS_DIR, remote_file_name, version);

  FILE *backup = fopen(backup_path, "r");
  if (!backup)
  {
    send(client_sock, "ERROR: Version not found\n", 25, 0);
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

// Handle client connection
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
      handle_write_request(client_sock, buffer);
    }
    else if (strncmp(buffer, "GET", 3) == 0)
    {
      handle_get_request(client_sock, buffer);
    }
    else if (strncmp(buffer, "RM", 2) == 0)
    {
      handle_delete_request(client_sock, buffer);
    }
    else if (strncmp(buffer, "REVERT", 6) == 0)
    {
      handle_revert_request(client_sock, buffer);
    }
    else
    {
      send(client_sock, "ERROR: Unsupported command\n", 28, 0);
    }
  }

  CLOSESOCKET(client_sock);
  return NULL;
}

int main()
{
#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
    return -1;
  }
#endif

  mkdir(ROOT_DIR, 0755);
  mkdir(FILES_DIR, 0755);
  mkdir(VERSIONS_DIR, 0755);

  log_file = fopen(LOG_FILE, "a");
  if (!log_file)
  {
    perror("Failed to open server log file");
    return -1;
  }

  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0)
  {
    perror("Socket creation failed");
    fclose(log_file);
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Bind failed");
    CLOSESOCKET(server_sock);
    fclose(log_file);
    return -1;
  }

  if (listen(server_sock, 10) < 0)
  {
    perror("Listen failed");
    CLOSESOCKET(server_sock);
    fclose(log_file);
    return -1;
  }

  printf("Server listening on port %d...\n", PORT);

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0)
    {
      perror("Accept failed");
      continue;
    }

    pthread_t client_thread;
    int *new_sock = malloc(sizeof(int));
    *new_sock = client_sock;

    if (pthread_create(&client_thread, NULL, handle_client, new_sock) != 0)
    {
      perror("Thread creation failed");
      free(new_sock);
    }

    pthread_detach(client_thread);
  }

  CLOSESOCKET(server_sock);
  fclose(log_file);

#ifdef _WIN32
  WSACleanup();
#endif

  return 0;
}
