# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# OS detection
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
    RM = del /Q
    EXE_SUFFIX = .exe
else
    LIBS =
    RM = rm -f
    EXE_SUFFIX =
endif

# Targets
all: server client

server: server.c
	$(CC) $(CFLAGS) -o server$(EXE_SUFFIX) server.c $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o client$(EXE_SUFFIX) client.c $(LIBS)

clean:
	$(RM) server$(EXE_SUFFIX) client$(EXE_SUFFIX)
