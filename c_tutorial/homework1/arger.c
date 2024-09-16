/*
* arger.c / Practice Assignment
*
* yinsun / CS5600 / Northeastern University
* Fall 2024 / Sep 8, 2024
*argc 是表示命令行参数数量的变量
*argv[0] 是程序的名字，后面的 argv[1]、argv[2] 等是实际传入的参数。
*大于三个是因为程序至少需要两个输入参数才能正确执行操作。
*
*
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
void to_upper(char *str);
void to_lower(char *str);
void to_capitalize(char *str);

int main(int argc, char *argv[]) {
    if (argc < 3) {
// *argc 是表示命令行参数数量的变量
// *argv[0] 是程序的名字，后面的 argv[1]、argv[2] 等是实际传入的参数。
// *大于三个是因为程序至少需要两个输入参数才能正确执行操作。
        printf("Error: Not enough arguments provided.\n");
        return -1;
    }

    // Check the option provided/str comparison,0 represent the true
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[1], "-u") == 0) {
            to_upper(argv[i]);
        } else if (strcmp(argv[1], "-l") == 0) {
            to_lower(argv[i]);
        } else if (strcmp(argv[1], "-cap") == 0) {
            to_capitalize(argv[i]);
        } else {
            printf("Error: Unknown option '%s'.\n", argv[1]);
            return -1;
        }

        // Print the result with a space between words (except after the last word)
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }

    printf("\n");
    return 0;
}
// char *str 是一个指向字符串（字符数组）的指针，指向字符串的第一个字符。
// 在 while (*str) 循环中，每次迭代中 *str 代表当前指针指向的字符。
// 当你执行 *str = toupper(*str) 时，你修改了字符串中当前指向位置的字符
// Convert string to uppercasestr++ 让指针移动到下一个字符的位置，这样你可以遍历整个字符串，
// 直到遇到空字符 \0，也就是循环结束的条件。
void to_upper(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

// Convert string to lowercase
void to_lower(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// Capitalize first letter of each word
void to_capitalize(char *str) {
    int capitalize = 1;
    while (*str) {
        if (isspace(*str)) {
            capitalize = 1;
        } else if (capitalize && isalpha(*str)) {
            *str = toupper(*str);
            capitalize = 0;
        } else {
            *str = tolower(*str);
        }
        str++;
    }
}
// 使用指针遍历字符串比使用数组索引（例如 str[i]）更高效，
// 因为指针可以直接访问内存地址，不需要通过下标来计算地址偏移。
// 指针可以灵活地处理内存中的数据，
// 通过递增指针的值来遍历字符，而不需要显式维护索引变量。