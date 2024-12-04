#!/bin/bash

# Configuration
# 配置部分
SERVER_EXEC="./server"  # Path to the server executable / 服务器可执行文件路径
CLIENT_EXEC="./client"  # Path to the client executable / 客户端可执行文件路径
TEST_FILES=("file1.txt" "file2.txt" "file3.txt")  # Test files to upload to the server / 上传到服务器的测试文件
LOCAL_FILES=("localfile1.txt" "localfile2.txt" "localfile3.txt")  # Local files to write content for the test / 本地文件，用于测试写入内容
DOWNLOADED_FILES=("downloadedfile1.txt" "downloadedfile2.txt" "downloadedfile3.txt")  # Files to download / 要下载的文件
BACKUP_DIR="server_root/backups/"  # Backup directory on the server / 服务器上的备份目录
LOG_FILE="server_root/version.log"  # Log file for version control / 用于版本控制的日志文件

# Helper functions
# 辅助函数

# Start the server
# 启动服务器
start_server() {
    echo "Starting the server..."  # Inform the user the server is starting / 通知用户服务器正在启动
    $SERVER_EXEC &  # Run the server in the background / 在后台运行服务器
    SERVER_PID=$!  # Save the server process ID / 保存服务器的进程ID
    sleep 2  # Wait for the server to initialize / 等待服务器初始化
}

# Stop the server
# 停止服务器
stop_server() {
    echo "Stopping the server..."  # Inform the user the server is stopping / 通知用户服务器正在停止
    kill $SERVER_PID  # Kill the server process by its PID / 使用PID停止服务器进程
    sleep 1  # Wait for the server to shut down properly / 等待服务器关闭
}

# Clean up temporary test files
# 清理临时测试文件
clean_up_temp_files() {
    echo "Cleaning up temporary test files..."  # Inform the user that temporary files are being cleaned up / 通知用户正在清理临时文件
    rm -f "${LOCAL_FILES[@]}" "${DOWNLOADED_FILES[@]}"  # Remove the temporary files / 删除临时文件
}

# Check if a file exists, exit if not
# 检查文件是否存在，如果不存在则退出
check_file_existence() {
    if [[ ! -f $1 ]]; then
        echo "ERROR: $1 does not exist."  # If file does not exist, print error / 如果文件不存在，则打印错误
        exit 1
    fi
}

# Test WRITE command
# 测试WRITE命令
test_write() {
    echo "Testing WRITE command..."  # Inform the user that the WRITE command is being tested / 通知用户正在测试WRITE命令
    for i in "${!TEST_FILES[@]}"; do  # Iterate through each file in the TEST_FILES array / 遍历每个TEST_FILES数组中的文件
        echo "This is test content for ${LOCAL_FILES[i]}." > "${LOCAL_FILES[i]}"  # Write test content to the local file / 将测试内容写入本地文件
        response=$($CLIENT_EXEC WRITE "${LOCAL_FILES[i]}" "${TEST_FILES[i]}")  # Call the client to send the WRITE command / 调用客户端发送WRITE命令
        if [[ $response == *"File written successfully"* ]]; then  # Check if the server responded with a success message / 检查服务器是否响应成功信息
            echo "WRITE passed for ${TEST_FILES[i]}."  # Inform that the WRITE command was successful / 通知WRITE命令成功
        else
            echo "WRITE failed for ${TEST_FILES[i]}."  # Inform that the WRITE command failed / 通知WRITE命令失败
            echo "Server response: $response"  # Print the server's response / 打印服务器的响应
            exit 1  # Exit the script if the WRITE command fails / 如果WRITE命令失败，退出脚本
        fi
    done
}

# Test GET command
# 测试GET命令
test_get() {
    echo "Testing GET command..."  # Inform the user that the GET command is being tested / 通知用户正在测试GET命令
    for i in "${!TEST_FILES[@]}"; do  # Iterate through each file in TEST_FILES / 遍历TEST_FILES中的每个文件
        response=$($CLIENT_EXEC GET "${TEST_FILES[i]}" "${DOWNLOADED_FILES[i]}")  # Call the client to send the GET command / 调用客户端发送GET命令
        # Check if the downloaded file exists and the content is correct / 检查下载的文件是否存在且内容正确
        if [[ -f "${DOWNLOADED_FILES[i]}" && "$(cat "${DOWNLOADED_FILES[i]}")" == "This is test content for ${LOCAL_FILES[i]}." ]]; then
            echo "GET passed for ${TEST_FILES[i]}."  # Inform that the GET command was successful / 通知GET命令成功
        else
            echo "GET failed for ${TEST_FILES[i]}."  # Inform that the GET command failed / 通知GET命令失败
            echo "Server response: $response"  # Print the server's response / 打印服务器的响应
            exit 1  # Exit the script if the GET command fails / 如果GET命令失败，退出脚本
        fi
    done
}

# Test RM command
# 测试RM命令
test_rm() {
    echo "Testing RM command..."  # Inform the user that the RM command is being tested / 通知用户正在测试RM命令
    for file in "${TEST_FILES[@]}"; do  # Iterate through each file in TEST_FILES / 遍历TEST_FILES中的每个文件
        response=$($CLIENT_EXEC RM "$file")  # Call the client to send the RM command / 调用客户端发送RM命令
        if [[ ! -f "server_root/files/$file" ]]; then  # Check if the file was successfully deleted / 检查文件是否成功删除
            echo "RM passed for $file."  # Inform that the RM command was successful / 通知RM命令成功
        else
            echo "RM failed for $file."  # Inform that the RM command failed / 通知RM命令失败
            echo "Server response: $response"  # Print the server's response / 打印服务器的响应
            exit 1  # Exit the script if the RM command fails / 如果RM命令失败，退出脚本
        fi
    done
}

# Test REVERT command
# 测试REVERT命令
test_revert() {
    echo "Testing REVERT command..."  # Inform the user that the REVERT command is being tested / 通知用户正在测试REVERT命令
    for i in "${!TEST_FILES[@]}"; do  # Iterate through each file in TEST_FILES / 遍历TEST_FILES中的每个文件
        if [[ ! -f "${BACKUP_DIR}${TEST_FILES[i]}" ]]; then  # Check if a backup exists for the file / 检查文件是否存在备份
            echo "No backups available for REVERT test of ${TEST_FILES[i]}."  # Inform that no backup is available for this file / 如果没有备份，则通知
            continue
        fi

        response=$($CLIENT_EXEC REVERT "${TEST_FILES[i]}")  # Call the client to send the REVERT command / 调用客户端发送REVERT命令
        # Check if the file exists after the revert and print its content
        # 检查文件是否存在且打印其内容
        if [[ -f "server_root/files/${TEST_FILES[i]}" ]]; then
            echo "Reverted file content for ${TEST_FILES[i]}: $(cat "server_root/files/${TEST_FILES[i]}")"
        else
            echo "ERROR: ${TEST_FILES[i]} does not exist after revert."  # If the file doesn't exist after revert, print error / 如果恢复后文件不存在，打印错误
            echo "Server response: $response"  # Print the server's response / 打印服务器的响应
            exit 1  # Exit if the file doesn't exist / 如果文件不存在，退出脚本
        fi

        # Verify that the file content matches the original content after revert / 验证恢复后的文件内容是否与原始内容匹配
        if [[ "$(cat "server_root/files/${TEST_FILES[i]}")" == "This is test content for ${LOCAL_FILES[i]}." ]]; then
            echo "REVERT passed for ${TEST_FILES[i]}."  # Inform that the REVERT command was successful / 通知REVERT命令成功
        else
            echo "REVERT failed for ${TEST_FILES[i]}."  # Inform that the REVERT command failed / 通知REVERT命令失败
            echo "Server response: $response"  # Print the server's response / 打印服务器的响应
            exit 1  # Exit the script if the REVERT command fails / 如果REVERT命令失败，退出脚本
        fi
    done
}

# Main script execution
# 脚本主要执行部分
echo "Preparing test environment..."  # Inform the user that the test environment is being prepared / 通知用户正在准备测试环境
mkdir -p server_root/files/ server_root/backups/  # Create required directories for testing / 创建所需的测试目录

start_server  # Start the server / 启动服务器

test_write  # Test WRITE command / 测试WRITE命令
test_get  # Test GET command / 测试GET命令
test_rm  # Test RM command / 测试RM命令
test_revert  # Test REVERT command / 测试REVERT命令

stop_server  # Stop the server / 停止服务器

clean_up_temp_files  # Clean up temporary test files / 清理临时测试文件

echo "All tests completed successfully. Logs and backups retained for review."  # Inform that all tests passed / 通知所有测试成功完成
