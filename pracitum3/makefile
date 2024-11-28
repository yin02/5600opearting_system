CC = gcc
CFLAGS = -Wall -Wextra -pthread
SERVER_SRC = server.c
CLIENT_SRC = client.c
SERVER_BIN = server
CLIENT_BIN = client

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
