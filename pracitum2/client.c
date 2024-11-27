#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    typedef int socklen_t;
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
#endif

#define BUFFER_SIZE 4096

void send_delete_command(SOCKET socket_desc, const char* remote_file_name) {
    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "DELETE %s\n", remote_file_name);

    // Send DELETE command to server
    if (send(socket_desc, message, strlen(message), 0) < 0) {
        printf("Error: Failed to send DELETE command\n");
        return;
    }

    printf("DELETE command sent successfully\n");

    // Receive server response
    char server_reply[BUFFER_SIZE];
    memset(server_reply, 0, sizeof(server_reply));
    if (recv(socket_desc, server_reply, sizeof(server_reply) - 1, 0) < 0) {
        printf("Error: Failed to receive server response\n");
        return;
    }

    printf("Server reply: %s\n", server_reply);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("%s WRITE <local_file_path> <remote_file_name>\n", argv[0]);
        printf("%s GET <remote_file_name> <local_file_name>\n", argv[0]);
        printf("%s DELETE <remote_file_name>\n", argv[0]);
        return -1;
    }

    const char* command = argv[1];
    const char* remote_file_name = argc >= 3 ? argv[2] : NULL;

    // Initialize Winsock for Windows
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return -1;
    }
#endif

    // Create socket
    SOCKET socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == INVALID_SOCKET) {
        printf("Could not create socket. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    // Connect to server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connect failed. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    printf("Connected to server\n");

    // Handle DELETE command
    if (strcmp(command, "DELETE") == 0) {
        if (argc != 3) {
            printf("Usage: %s DELETE <remote_file_name>\n", argv[0]);
            return -1;
        }
        send_delete_command(socket_desc, remote_file_name);
    } else {
        printf("Unsupported command: %s\n", command);
    }

    // Close socket
    CLOSESOCKET(socket_desc);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
