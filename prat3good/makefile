CC = gcc
CFLAGS = -Wall -Wextra -pthread

# Detect the OS
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
    RM = del
    EXE_SUFFIX = .exe
else
    LIBS =
    RM = rm -f
    EXE_SUFFIX =
endif

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server$(EXE_SUFFIX) server.c $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o client$(EXE_SUFFIX) client.c $(LIBS)

clean:
	$(RM) server$(EXE_SUFFIX) client$(EXE_SUFFIX)
