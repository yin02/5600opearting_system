#ifndef POLYBIUS_H // 如果还没有定义 POLYBIUS_H，则继续定义。
#define POLYBIUS_H // 定义 POLYBIUS_H 以防止多次包含头文件，防止重复定义

/*
 * 函数: pbEncode
 * ----------------
 *  该函数将一个普通文本字符串 (plaintext) 编码为密文 (encoded)。
 *
 *  参数:
 *    plaintext: 一个要加密的字符串，明文形式。可以包含大写或小写字母。
 *               该函数将忽略非字母字符（例如标点符号、数字等）。
 *               字符 'J' 会被视为 'I' 来处理，因为标准 Polybius 方阵
 *               5x5 不能容纳26个字母，所以 'I' 和 'J' 合并。
 *    encoded:   一个用于存储加密后密文的字符串。它将存储由行和列数字表示的密文。
 *               例如，字母 'A' 可能会被编码为 '11'，表示第一行和第一列。
 *
 *  返回:
 *    无返回值。加密后的密文将存储在 `encoded` 参数中。
 */
void pbEncode(const char *plaintext, char *encoded);

/*
 * 函数: pbDecode
 * ----------------
 *  该函数将一个密文字符串 (ciphertext) 解码为明文 (decoded)。
 *
 *  参数:
 *    ciphertext: 一个由数字组成的密文字符串。每个字母对应两个数字，
 *                第一个数字表示字母在 Polybius 方阵中的行号，
 *                第二个数字表示列号。例如 '11' 表示字母 'A'。
 *    decoded:    一个用于存储解密后明文的字符串。结果将是大写字母，所有字母都转换为大写。
 *                在解密过程中，'I' 和 'J' 被视为同一个字符。
 *
 *  返回:
 *    无返回值。解密后的明文将存储在 `decoded` 参数中。
 */
void pbDecode(const char *ciphertext, char *decoded);

#endif // 结束 POLYBIUS_H 宏定义
