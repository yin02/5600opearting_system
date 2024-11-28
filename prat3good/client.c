#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define PORT 2024

void send_write_command(int socket_desc, const char *local_file, const char *remote_file)
{
  FILE *file = fopen(local_file, "r");
  if (!file)
  {
    perror("File open error");
    return;
  }

  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "WRITE %s ", remote_file);

  // Read the file data into the message buffer
  char file_data[BUFFER_SIZE];
  fread(file_data, 1, sizeof(file_data) - 1, file);
  fclose(file);

  strcat(message, file_data);

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("WRITE command sent: %s\n", message);

  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server response: %s\n", server_reply);
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

  printf("GET command sent: %s\n", message);

  FILE *file = fopen(local_file, "w");
  if (!file)
  {
    perror("File open error");
    return;
  }

  char server_reply[BUFFER_SIZE];
  int received;
  while ((received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0)) > 0)
  {
    fwrite(server_reply, 1, received, file);
  }

  fclose(file);
  printf("File retrieved and saved to %s\n", local_file);
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

  printf("RM command sent: %s\n", message);

  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server response: %s\n", server_reply);
}

void send_revert_command(int socket_desc, const char *remote_file)
{
  char message[BUFFER_SIZE];
  snprintf(message, sizeof(message), "REVERT %s", remote_file);

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    perror("Send failed");
    return;
  }

  printf("REVERT command sent: %s\n", message);

  char server_reply[BUFFER_SIZE];
  int received = recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0);
  if (received < 0)
  {
    perror("Receive failed");
    return;
  }

  server_reply[received] = '\0';
  printf("Server response: %s\n", server_reply);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage:\n");
    printf("  %s WRITE <local_file> <remote_file>\n", argv[0]);
    printf("  %s GET <remote_file> <local_file>\n", argv[0]);
    printf("  %s RM <remote_file>\n", argv[0]);
    printf("  %s REVERT <remote_file>\n", argv[0]);
    return 1;
  }

  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0)
  {
    perror("Socket creation failed");
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Connect failed");
    return 1;
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
  else if (strcmp(argv[1], "REVERT") == 0 && argc == 3)
  {
    send_revert_command(socket_desc, argv[2]);
  }
  else
  {
    printf("Invalid command or arguments.\n");
  }

  close(socket_desc);
  return 0;
}
