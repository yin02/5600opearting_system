CC = gcc
CFLAGS = -Wall -Wextra -g
ifdef OS
    LDFLAGS = -lws2_32
else
    LDFLAGS = -lpthread
endif

all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) client.c -o client $(LDFLAGS)

clean:
	rm -f server client
