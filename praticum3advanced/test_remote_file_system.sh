#!/bin/bash

# Configuration
SERVER_EXEC="./server"
CLIENT_EXEC="./client"
TEST_FILES=("file1.txt" "file2.txt" "file3.txt")
LOCAL_FILES=("localfile1.txt" "localfile2.txt" "localfile3.txt")
DOWNLOADED_FILES=("downloadedfile1.txt" "downloadedfile2.txt" "downloadedfile3.txt")
BACKUP_DIR="server_root/backups/"
LOG_FILE="server_root/version.log"

# Helper functions
start_server() {
    echo "Starting the server..."
    $SERVER_EXEC &
    SERVER_PID=$!
    sleep 2
}

stop_server() {
    echo "Stopping the server..."
    kill $SERVER_PID
    sleep 1
}

clean_up_temp_files() {
    echo "Cleaning up temporary test files..."
    rm -f "${LOCAL_FILES[@]}" "${DOWNLOADED_FILES[@]}"
}

# Test WRITE command
test_write() {
    echo "Testing WRITE command..."
    for i in "${!TEST_FILES[@]}"; do
        echo "This is test content for ${LOCAL_FILES[i]}." > "${LOCAL_FILES[i]}"
        response=$($CLIENT_EXEC WRITE "${LOCAL_FILES[i]}" "${TEST_FILES[i]}")
        if [[ $response == *"File written successfully"* ]]; then
            echo "WRITE passed for ${TEST_FILES[i]}."
        else
            echo "WRITE failed for ${TEST_FILES[i]}."
            echo "Server response: $response"
            exit 1
        fi
    done
}

# Test GET command
test_get() {
    echo "Testing GET command..."
    for i in "${!TEST_FILES[@]}"; do
        response=$($CLIENT_EXEC GET "${TEST_FILES[i]}" "${DOWNLOADED_FILES[i]}")
        if [[ -f "${DOWNLOADED_FILES[i]}" && "$(cat "${DOWNLOADED_FILES[i]}")" == "This is test content for ${LOCAL_FILES[i]}." ]]; then
            echo "GET passed for ${TEST_FILES[i]}."
        else
            echo "GET failed for ${TEST_FILES[i]}."
            echo "Server response: $response"
            exit 1
        fi
    done
}

# Test RM command
test_rm() {
    echo "Testing RM command..."
    for file in "${TEST_FILES[@]}"; do
        response=$($CLIENT_EXEC RM "$file")
        if [[ ! -f "server_root/files/$file" ]]; then
            echo "RM passed for $file."
        else
            echo "RM failed for $file."
            echo "Server response: $response"
            exit 1
        fi
    done
}

# Test REVERT command
test_revert() {
    echo "Testing REVERT command..."
    for file in "${TEST_FILES[@]}"; do
        if [[ ! -f "${BACKUP_DIR}${file}" ]]; then
            echo "No backups available for REVERT test of $file."
            continue
        fi

        response=$($CLIENT_EXEC REVERT "$file")
        if [[ -f "server_root/files/$file" && "$(cat "server_root/files/$file")" == "This is test content for ${LOCAL_FILES[i]}." ]]; then
            echo "REVERT passed for $file."
        else
            echo "REVERT failed for $file."
            echo "Server response: $response"
            exit 1
        fi
    done
}

# Main script execution
echo "Preparing test environment..."
mkdir -p server_root/files/ server_root/backups/

start_server

test_write
test_get
test_rm
test_revert

stop_server

clean_up_temp_files

echo "All tests completed successfully. Logs and backups retained for review."
