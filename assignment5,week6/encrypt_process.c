#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "queue.h"
#include "readfile.h"

#define STRING_LENGTH 10
#define BATCH_SIZE 100

void processBatch(Queue *queue) {
    char batchBuffer[BATCH_SIZE * (STRING_LENGTH + 2)] = {0};
    
    HANDLE hInputReadPipe, hInputWritePipe;
    HANDLE hOutputReadPipe, hOutputWritePipe;
    SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    while (!isQueueEmpty(queue)) {
        printf("Starting batch processing...\n");  // Debugging print

        // Collect 100 strings from the queue
        for (int i = 0; i < BATCH_SIZE && !isQueueEmpty(queue); i++) {
            char *str = dequeue(queue);
            strcat(batchBuffer, str);
            strcat(batchBuffer, "\n");
            free(str);
        }
        printf("Batch collected: \n%s\n", batchBuffer);  // Debugging print

        // Create input and output pipes
        if (!CreatePipe(&hInputReadPipe, &hInputWritePipe, &saAttr, 0)) {
            fprintf(stderr, "CreatePipe (input) failed\n");
            exit(1);
        }
        if (!CreatePipe(&hOutputReadPipe, &hOutputWritePipe, &saAttr, 0)) {
            fprintf(stderr, "CreatePipe (output) failed\n");
            exit(1);
        }

        // Set up the process info structure for the child process
        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
        ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        siStartInfo.hStdInput = hInputReadPipe;   // Child reads input from this pipe
        siStartInfo.hStdOutput = hOutputWritePipe; // Child writes output to this pipe
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        // Command to execute polybius_cipher.exe
        char cmd[256] = "polybius_cipher.exe";

        // Create the child process
        if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
            fprintf(stderr, "CreateProcess failed\n");
            exit(1);
        }
        printf("Child process created for encryption.\n");  // Debugging print

        // Write the batch buffer to the input pipe
        DWORD dwWritten;
        if (!WriteFile(hInputWritePipe, batchBuffer, strlen(batchBuffer), &dwWritten, NULL)) {
            fprintf(stderr, "WriteFile failed\n");
            exit(1);
        }
        printf("Batch written to pipe.\n");  // Debugging print

        // Close the write end of the input pipe to signal EOF to the child process
        CloseHandle(hInputWritePipe);

        // Read the encrypted data from the output pipe
        DWORD dwRead;
        char encryptedBuffer[4096];  // Make sure the buffer is large enough to hold the output
        if (ReadFile(hOutputReadPipe, encryptedBuffer, sizeof(encryptedBuffer) - 1, &dwRead, NULL)) {
            encryptedBuffer[dwRead] = '\0';  // Null-terminate the output
            printf("Encrypted batch: \n%s\n", encryptedBuffer);  // Output the result for testing
        } else {
            fprintf(stderr, "ReadFile failed\n");
            exit(1);
        }

        // Wait for the child process to finish
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
        printf("Child process finished.\n");  // Debugging print

        // Close handles
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(hInputReadPipe);
        CloseHandle(hOutputWritePipe);
        CloseHandle(hOutputReadPipe);

        // Clear the batch buffer for the next batch
        memset(batchBuffer, 0, sizeof(batchBuffer));
    }
    printf("All batches processed.\n");  // Debugging print
}

int main() {
    Queue *queue = createQueue();
    readFileAndEnqueue("random_strings.txt", queue);  // Read and enqueue strings

    // Process strings in batches of 100 and encrypt them using the Polybius cipher
    processBatch(queue);

    freeQueue(queue);  // Clean up the queue
    return 0;
}
