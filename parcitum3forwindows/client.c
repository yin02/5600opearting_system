#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define SERVER_PORT 2024

void send_write_command(int socket_desc, const char *local_file, const char *remote_file)
{
  FILE *file = fopen(local_file, "r");
  if (!file)
  {
    perror("Failed to open local file");
    return;
  }

  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "WRITE %s ", remote_file);

  char file_data[BUFFER_SIZE];
  size_t bytes_read = fread(file_data, 1, sizeof(file_data) - 1, file);
  file_data[bytes_read] = '\0';

  fclose(file);

  strncat(message, file_data, sizeof(message) - strlen(message) - 1);

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("WRITE command sent: %s\n", remote_file);

  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server reply: %s\n", server_reply);
}

void send_get_command(int socket_desc, const char *remote_file, const char *local_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "GET %s", remote_file);

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  FILE *file = fopen(local_file, "w");
  if (!file)
  {
    perror("Failed to open local file for writing");
    return;
  }

  char buffer[BUFFER_SIZE];
  int received;
  while ((received = recv(socket_desc, buffer, sizeof(buffer) - 1, 0)) > 0)
  {
    fwrite(buffer, 1, received, file);
  }

  fclose(file);
  printf("File received and saved as: %s\n", local_file);
}

void send_rm_command(int socket_desc, const char *remote_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "RM %s", remote_file);

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server reply: %s\n", server_reply);
}

void send_ls_command(int socket_desc)
{
  char message[] = "LS";

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  char buffer[BUFFER_SIZE];
  int received;
  printf("Directory listing:\n");
  while ((received = recv(socket_desc, buffer, sizeof(buffer) - 1, 0)) > 0)
  {
    buffer[received] = '\0';
    printf("%s", buffer);
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage:\n");
    printf("  WRITE <local_file> <remote_file>\n");
    printf("  GET <remote_file> <local_file>\n");
    printf("  RM <remote_file>\n");
    printf("  LS\n");
    return -1;
  }

  int socket_desc;
  struct sockaddr_in server_addr;
  char server_ip[] = "127.0.0.1";

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0)
  {
    perror("Unable to create socket");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(server_ip);

  if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Unable to connect to server");
    close(socket_desc);
    return -1;
  }

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
  else if (strcmp(argv[1], "LS") == 0 && argc == 2)
  {
    send_ls_command(socket_desc);
  }
  else
  {
    printf("Invalid command or arguments\n");
  }

  close(socket_desc);
  return 0;
}
