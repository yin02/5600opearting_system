#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 线性同余生成器（LCG）的参数设置
// LCG 是一种简单且广泛使用的伪随机数生成算法，使用下列公式生成随机数：
// X_{n+1} = (a * X_n + c) % m
#define MULTIPLIER 1103515245  // 乘数 (a)
#define INCREMENT 12345        // 增量 (c)
#define MODULUS 2147483648     // 模数 (m)，即 2^31，用于控制随机数的范围

unsigned int seed = 123456789; // 初始种子 (X_0)，可通过函数修改

// 函数: genRand
// ----------------
//  使用线性同余生成器 (LCG) 生成随机数。
// 
//  参数:
//    min: 随机数生成的最小值
//    max: 随机数生成的最大值
// 
//  返回:
//    在[min, max] 范围内的随机整数。
// 
//  说明:
//    该函数通过 LCG 算法生成伪随机数，并将其范围限制在指定的 [min, max] 之间。
int genRand(int min, int max) {
    // 使用 LCG 更新种子并生成一个新的伪随机数
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;

    // 将生成的随机数调整到 [min, max] 范围内，并返回
    return min + (seed % (max - min + 1));
}

int main(int argc, char *argv[]) {
    // 检查命令行参数是否正确。如果参数数量小于 3 或大于 4，则打印使用提示并退出。
    if (argc < 3 || argc > 4) {
        printf("Usage: %s <number_of_randoms> <file_name> [-a]\n", argv[0]);
        return 1;
    }

    // 获取要生成的随机数的数量
    int numRandoms = atoi(argv[1]);  // 将第一个参数（随机数的数量）转换为整数
    char *fileName = argv[2];        // 第二个参数是文件名

    // 如果命令行包含第三个参数 "-a"，则进入追加模式；否则进入覆盖模式
    int appendMode = (argc == 4 && strcmp(argv[3], "-a") == 0);

    // 打开文件
    FILE *file;
    if (appendMode) {
        file = fopen(fileName, "a"); // 以追加模式打开文件（如果文件存在，追加内容）
    } else {
        file = fopen(fileName, "w"); // 以覆盖模式打开文件（如果文件存在，覆盖内容）
    }

    // 检查文件是否成功打开。如果打开失败，打印错误消息并退出。
    if (file == NULL) {
        printf("Error: Could not open file %s\n", fileName);
        return 1;
    }

    // 生成随机数并将其写入文件
    for (int i = 0; i < numRandoms; i++) {
        int randNum = genRand(1, 100);  // 生成 [1, 100] 范围内的随机数
        fprintf(file, "%d\n", randNum); // 将随机数写入文件
    }

    // 关闭文件
    fclose(file);

    // 打印生成的随机数数量以及输出的文件名
    printf("Generated %d random numbers and wrote to %s\n", numRandoms, fileName);
    return 0;
}
