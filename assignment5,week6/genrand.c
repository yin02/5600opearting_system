#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_STRINGS 10000
#define STRING_LENGTH 10

// Function to generate random strings
void generateRandomString(char *str, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length] = '\0';
}

// Function to generate strings and write them to a file
void generateAndWriteStrings(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char randomString[STRING_LENGTH + 1];
    for (int i = 0; i < TOTAL_STRINGS; i++) {
        generateRandomString(randomString, STRING_LENGTH);
        fprintf(file, "%s\n", randomString);
    }

    fclose(file);
    printf("Generated and wrote %d random strings to %s\n", TOTAL_STRINGS, filename);
}

int main() {
    srand(time(NULL)); // Seed random number generator
    generateAndWriteStrings("random_strings.txt");
    return 0;
}
