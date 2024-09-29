#include <stdio.h>
#include <string.h>
#include "polybius.h"  // 引入包含编码和解码函数声明的头文件

/*
 * 函数: main
 * ------------
 *  根据命令行参数执行 Polybius 方阵加密或解密操作。
 *  支持命令行选项 -e（加密）和 -d（解密）。
 * 
 *  参数:
 *    argc: 命令行参数的数量
 *    argv: 存储命令行参数的字符串数组
 * 
 *  返回:
 *    返回 0 表示成功执行，1 表示命令行参数不正确。
 */
int main(int argc, char *argv[]) {
    // 检查命令行参数的数量是否正确。至少需要 3 个参数：程序名、-e 或 -d、输入文本
    if (argc < 3) {
        printf("Usage: %s -e/-d <text>\n", argv[0]); // 提示使用方法
        return 1; // 参数不正确时返回 1
    }

    // 创建一个字符缓冲区，用于存储加密或解密后的结果
    char output[256];  // 假设编码或解码后的文本长度不超过 256 个字符

    // 如果命令行参数是 "-e"，则执行编码操作
    if (strcmp(argv[1], "-e") == 0) {
        pbEncode(argv[2], output);  // 调用编码函数，将输入文本转换为密文
        printf("Encoded: %s\n", output);  // 打印编码后的密文
    }
    // 如果命令行参数是 "-d"，则执行解码操作
    else if (strcmp(argv[1], "-d") == 0) {
        pbDecode(argv[2], output);  // 调用解码函数，将密文转换为明文
        printf("Decoded: %s\n", output);  // 打印解码后的明文
    }
    // 如果命令行参数既不是 "-e" 也不是 "-d"，提示用户输入有效的选项
    else {
        printf("Invalid option. Use -e to encode or -d to decode.\n");
    }

    return 0; // 程序成功结束，返回 0
}
