#!/bin/bash

# Configuration
SERVER_EXEC="./server"
CLIENT_EXEC="./client"
TEST_FILE="remotefile.txt"
LOCAL_FILE="localfile.txt"
DOWNLOADED_FILE="downloadedfile.txt"
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
    rm -f $LOCAL_FILE $DOWNLOADED_FILE
}

# Test WRITE command
test_write() {
    echo "Testing WRITE command..."
    echo "This is test content for WRITE." > $LOCAL_FILE
    response=$($CLIENT_EXEC WRITE $LOCAL_FILE $TEST_FILE)
    if [[ $response == *"File written successfully"* ]]; then
        echo "WRITE passed."
    else
        echo "WRITE failed."
        echo "Server response: $response"
        exit 1
    fi
}

# Test GET command
test_get() {
    echo "Testing GET command..."
    response=$($CLIENT_EXEC GET $TEST_FILE $DOWNLOADED_FILE)
    if [[ -f $DOWNLOADED_FILE && "$(cat $DOWNLOADED_FILE)" == "This is test content for WRITE." ]]; then
        echo "GET passed."
    else
        echo "GET failed."
        echo "Server response: $response"
        exit 1
    fi
}

# Test RM command
test_rm() {
    echo "Testing RM command..."
    response=$($CLIENT_EXEC RM $TEST_FILE)
    if [[ ! -f server_root/files/$TEST_FILE ]]; then
        echo "RM passed."
    else
        echo "RM failed."
        echo "Server response: $response"
        exit 1
    fi
}

# Test REVERT command
test_revert() {
    echo "Testing REVERT command..."
    if [[ ! -f ${BACKUP_DIR}${TEST_FILE} ]]; then
        echo "No backups available for REVERT test."
        exit 1
    fi

    response=$($CLIENT_EXEC REVERT $TEST_FILE)
    if [[ -f server_root/files/$TEST_FILE && "$(cat server_root/files/$TEST_FILE)" == "This is test content for WRITE." ]]; then
        echo "REVERT passed."
    else
        echo "REVERT failed."
        echo "Server response: $response"
        exit 1
    fi
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
