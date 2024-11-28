# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# Output binaries
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Platform-specific configurations
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
    EXE_EXT = .exe
else
    LIBS =
    EXE_EXT =
endif

# Targets
all: $(SERVER)$(EXE_EXT) $(CLIENT)$(EXE_EXT)

$(SERVER)$(EXE_EXT): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER)$(EXE_EXT) $(LIBS)

$(CLIENT)$(EXE_EXT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT)$(EXE_EXT) $(LIBS)

clean:
	rm -f $(SERVER)$(EXE_EXT) $(CLIENT)$(EXE_EXT)
