#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>

#define BUFFER_SIZE 4096
#define PORT 2024
#define ROOT_DIR "server_root/"
#define FILES_DIR "server_root/files/"
#define METADATA_DIR "server_root/metadata/"
#define LOG_FILE ROOT_DIR "server.log"

FILE *log_file;

// Log function
void log_message(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  fprintf(log_file, "\n");
  fflush(log_file);
  va_end(args);
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
    log_message("Invalid WRITE command received: %s", message);
    return;
  }

  file_name_start += strlen("WRITE ");
  char *file_content_start = strchr(file_name_start, ' ');
  if (!file_content_start)
  {
    send(client_sock, "ERROR: Missing file content\n", 29, 0);
    log_message("Missing file content in WRITE command: %s", message);
    return;
  }

  size_t file_name_length = file_content_start - file_name_start;
  strncpy(remote_file_name, file_name_start, file_name_length);
  remote_file_name[file_name_length] = '\0';
  strncpy(file_data, file_content_start + 1, sizeof(file_data) - 1);
  file_data[sizeof(file_data) - 1] = '\0';

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  FILE *file = fopen(file_path, "w");
  if (!file)
  {
    perror("File open error");
    send(client_sock, "ERROR: Could not save file\n", 27, 0);
    log_message("Failed to open file for writing: %s", file_path);
    return;
  }

  fprintf(file, "%s", file_data);
  fclose(file);

  send(client_sock, "File written successfully\n", 26, 0);
  log_message("File written successfully: %s", file_path);
}

// Handle GET command
void handle_get_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "GET %255s", remote_file_name) != 1)
  {
    send(client_sock, "ERROR: Invalid GET command\n", 27, 0);
    log_message("Invalid GET command received: %s", message);
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  FILE *file = fopen(file_path, "r");
  if (!file)
  {
    send(client_sock, "ERROR: File not found\n", 22, 0);
    log_message("File not found: %s", file_path);
    return;
  }

  char file_data[BUFFER_SIZE];
  size_t bytes_read;
  while ((bytes_read = fread(file_data, 1, sizeof(file_data), file)) > 0)
  {
    send(client_sock, file_data, bytes_read, 0);
  }

  fclose(file);
  log_message("File sent successfully: %s", file_path);
}

// Handle RM command
void handle_delete_request(int client_sock, const char *message)
{
  char remote_file_name[256];
  if (sscanf(message, "RM %255s", remote_file_name) != 1)
  {
    send(client_sock, "ERROR: Invalid RM command\n", 26, 0);
    log_message("Invalid RM command received: %s", message);
    return;
  }

  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

  if (remove(file_path) == 0)
  {
    send(client_sock, "File deleted successfully\n", 26, 0);
    log_message("File deleted successfully: %s", file_path);
  }
  else
  {
    send(client_sock, "ERROR: Failed to delete file\n", 29, 0);
    log_message("Failed to delete file: %s", file_path);
  }
}

// Handle LS command
void handle_ls_request(int client_sock)
{
  char command[256], buffer[BUFFER_SIZE];
  snprintf(command, sizeof(command), "ls %s > ls_output.txt", FILES_DIR);
  system(command);

  FILE *file = fopen("ls_output.txt", "r");
  if (!file)
  {
    send(client_sock, "ERROR: Could not list files\n", 28, 0);
    log_message("Failed to list files in directory: %s", FILES_DIR);
    return;
  }

  while (fgets(buffer, sizeof(buffer), file))
  {
    send(client_sock, buffer, strlen(buffer), 0);
  }

  fclose(file);
  remove("ls_output.txt");
  log_message("Directory listing sent successfully.");
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
    printf("Received: %s\n", buffer);
    log_message("Received: %s", buffer);

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
    else if (strncmp(buffer, "LS", 2) == 0)
    {
      handle_ls_request(client_sock);
    }
    else
    {
      send(client_sock, "ERROR: Unsupported command\n", 28, 0);
      log_message("Unsupported command received: %s", buffer);
    }
  }

  close(client_sock);
  return NULL;
}

int main()
{
  mkdir(ROOT_DIR, 0755);
  mkdir(FILES_DIR, 0755);
  mkdir(METADATA_DIR, 0755);

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
    log_message("Socket creation failed");
    fclose(log_file);
    return -1;
  }

  struct sockaddr_in server_addr, client_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Bind failed");
    log_message("Bind failed");
    close(server_sock);
    fclose(log_file);
    return -1;
  }

  if (listen(server_sock, 10) < 0)
  {
    perror("Listen failed");
    log_message("Listen failed");
    close(server_sock);
    fclose(log_file);
    return -1;
  }

  printf("Server listening on port %d...\n", PORT);
  log_message("Server listening on port %d...", PORT);

  while (1)
  {
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0)
    {
      perror("Accept failed");
      log_message("Accept failed");
      continue;
    }

    pthread_t client_thread;
    int *new_sock = malloc(sizeof(int));
    *new_sock = client_sock;

    if (pthread_create(&client_thread, NULL, handle_client, new_sock) != 0)
    {
      perror("Thread creation failed");
      log_message("Thread creation failed");
      free(new_sock);
    }

    pthread_detach(client_thread);
  }

  close(server_sock);
  fclose(log_file);
  return 0;
}
