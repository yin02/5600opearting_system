#!/bin/bash

# 执行 arger 程序，并传递所有脚本接收到的参数
./arger "$@"

# 检查上一个命令（arger）的退出状态码
if [ $? -ne 0 ]; then
    # 如果退出状态码不等于 0（即程序执行失败），则输出错误信息
    echo "An error occurred while running arger."
    
    # 退出脚本并返回状态码 1，表示执行失败
    exit 1
else
    # 如果退出状态码等于 0（即程序成功执行），则输出成功信息
    echo "arger ran successfully."
fi
