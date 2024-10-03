#include <stdio.h>
#include <ctype.h>
#include <string.h>

char polybiusSquare[5][5] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
};

// Function to encode using Polybius cipher
void pbEncode(const char *plaintext, char *encoded) {
    int k = 0;
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char letter = toupper(plaintext[i]);
        if (letter == 'J') letter = 'I';

        if (isalpha(letter)) {
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    if (polybiusSquare[row][col] == letter) {
                        encoded[k++] = '1' + row;
                        encoded[k++] = '1' + col;
                        break;
                    }
                }
            }
        }
    }
    encoded[k] = '\0';
}

int main() {
    char buffer[512];
    char encoded[1024];

    // Read multiple lines from stdin
    while (fgets(buffer, sizeof(buffer), stdin)) {
        pbEncode(buffer, encoded);
        printf("%s\n", encoded);
        fflush(stdout);  // Ensure the output is flushed after each line
    }

    return 0;
}
