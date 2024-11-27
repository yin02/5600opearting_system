#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <direct.h>  // For _mkdir
    #define mkdir(path, mode) _mkdir(path)  // Ignore mode for Windows
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
#define PORT 2000
#define ROOT_DIR "server_root/"
#define FILES_DIR "server_root/files/"
#define METADATA_DIR "server_root/metadata/"

// Function to initialize sockets (for Windows)
void init_socket() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#endif
}

// Function to clean up sockets (for Windows)
void cleanup_socket() {
#ifdef _WIN32
    WSACleanup();
#endif
}

// Handle WRITE request
void handle_write_request(SOCKET client_sock, const char* message) {
    char remote_file_name[256];
    char file_data[BUFFER_SIZE];

    if (sscanf(message, "WRITE %s\n%[^\t\n]", remote_file_name, file_data) != 2) {
        const char* error_message = "ERROR: Invalid WRITE command\n";
        send(client_sock, error_message, strlen(error_message), 0);
        return;
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

    FILE* file = fopen(file_path, "w");
    if (!file) {
        const char* error_message = "ERROR: Could not save file\n";
        send(client_sock, error_message, strlen(error_message), 0);
        return;
    }

    fprintf(file, "%s", file_data);
    fclose(file);

    const char* success_message = "File saved successfully\n";
    send(client_sock, success_message, strlen(success_message), 0);
}

// Handle GET request
void handle_get_request(SOCKET client_sock, const char* message) {
    char remote_file_name[256];
    if (sscanf(message, "GET %s", remote_file_name) != 1) {
        const char* error_message = "ERROR: Invalid GET command\n";
        send(client_sock, error_message, strlen(error_message), 0);
        return;
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

    FILE* file = fopen(file_path, "r");
    if (!file) {
        const char* error_message = "ERROR: File not found\n";
        send(client_sock, error_message, strlen(error_message), 0);
        return;
    }

    char file_data[BUFFER_SIZE];
    memset(file_data, 0, sizeof(file_data));
    fread(file_data, sizeof(char), sizeof(file_data) - 1, file);
    fclose(file);

    send(client_sock, file_data, strlen(file_data), 0);
}

// Handle DELETE request
void handle_delete_request(SOCKET client_sock, const char* message) {
    char remote_file_name[256];
    if (sscanf(message, "DELETE %s", remote_file_name) != 1) {
        const char* error_message = "ERROR: Invalid DELETE command\n";
        send(client_sock, error_message, strlen(error_message), 0);
        return;
    }

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", FILES_DIR, remote_file_name);

    if (remove(file_path) == 0) {
        const char* success_message = "File deleted successfully\n";
        send(client_sock, success_message, strlen(success_message), 0);
    } else {
        const char* error_message = "ERROR: Could not delete file\n";
        send(client_sock, error_message, strlen(error_message), 0);
    }
}

// Handle incoming client connection
void* handle_client(void* client_socket) {
    SOCKET client_sock = *(SOCKET*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_sock, buffer, sizeof(buffer) - 1, 0) <= 0) {
        printf("Failed to receive message\n");
        CLOSESOCKET(client_sock);
        return NULL;
    }

    printf("Received: %s\n", buffer);

    if (strncmp(buffer, "WRITE", 5) == 0) {
        handle_write_request(client_sock, buffer);
    } else if (strncmp(buffer, "GET", 3) == 0) {
        handle_get_request(client_sock, buffer);
    } else if (strncmp(buffer, "DELETE", 6) == 0) {
        handle_delete_request(client_sock, buffer);
    } else {
        const char* error_message = "ERROR: Unsupported command\n";
        send(client_sock, error_message, strlen(error_message), 0);
    }

    CLOSESOCKET(client_sock);
    return NULL;
}

// Main function
int main() {
    init_socket();

    mkdir(ROOT_DIR, 0755);
    mkdir(FILES_DIR, 0755);
    mkdir(METADATA_DIR, 0755);

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("Could not create socket. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    listen(server_sock, 10);
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        SOCKET client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        SOCKET* new_sock = malloc(sizeof(SOCKET));
        *new_sock = client_sock;

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, new_sock);
    }

    CLOSESOCKET(server_sock);
    cleanup_socket();
    return 0;
}
