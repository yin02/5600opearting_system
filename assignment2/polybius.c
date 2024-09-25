#include <stdio.h>
#include <ctype.h>
#include <string.h>

// 定义 Polybius 方阵，使用 5x5 的字母矩阵
// 方阵中的字母 I 和 J 合并在一起，因为英文字母有 26 个，而 5x5 方阵只能容纳 25 个字母
char polybiusSquare[5][5] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'}, // I 和 J 合并为一个格子
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
};

/*
 * 函数: pbEncode
 * ----------------
 *  该函数使用 Polybius 密码将明文字符串 (plaintext) 编码为密文 (encoded)。
 *
 *  参数:
 *    plaintext:  一个要加密的字符串，可以包含大小写字母。I 和 J 被视为同一个字母。
 *                字母将被转化为大写，非字母字符将被忽略。
 *    encoded:    存储加密后密文的字符串缓冲区。密文由行和列数字组成，
 *                每个字母对应 Polybius 方阵中的行和列。
 *
 *  返回:
 *    无返回值。加密后的密文存储在 `encoded` 缓冲区中。
 */
void pbEncode(const char *plaintext, char *encoded) {
    int k = 0; // k 用于跟踪加密字符串的位置
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char letter = toupper(plaintext[i]);  // 将字母转为大写
        if (letter == 'J') letter = 'I';      // 将 'J' 转化为 'I'，因为 Polybius 方阵中没有 'J'

        // 查找字母在 Polybius 方阵中的位置
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
                if (polybiusSquare[row][col] == letter) {
                    encoded[k++] = '1' + row; // 存储对应的行号（转换为字符）
                    encoded[k++] = '1' + col; // 存储对应的列号（转换为字符）
                    break; // 找到对应字母后跳出循环
                }
            }
        }
    }
    encoded[k] = '\0'; // 用 '\0' 结束字符串，表示字符串结尾
}

/*
 * 函数: pbDecode
 * ----------------
 *  该函数使用 Polybius 密码将密文字符串 (ciphertext) 解码为明文 (decoded)。
 *
 *  参数:
 *    ciphertext:  一个由数字对组成的字符串，每对数字表示 Polybius 方阵中的字母位置。
 *                 每两个数字对应方阵中的行和列，例如 '11' 表示字母 'A'。
 *    decoded:     存储解密后明文的字符串缓冲区。结果将是大写的字母。
 *
 *  返回:
 *    无返回值。解密后的明文存储在 `decoded` 缓冲区中。
 */
void pbDecode(const char *ciphertext, char *decoded) {
    int k = 0; // k 用于跟踪解密字符串的位置
    for (int i = 0; ciphertext[i] != '\0'; i += 2) { // 每次读取两位数字
        int row = ciphertext[i] - '1';     // 计算出行号
        int col = ciphertext[i + 1] - '1'; // 计算出列号
        decoded[k++] = polybiusSquare[row][col]; // 将对应的字母存入解密后的字符串
    }
    decoded[k] = '\0'; // 用 '\0' 结束字符串，表示字符串结尾
}
