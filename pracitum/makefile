# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Executable name
TARGET = message_program

# Source files
SRC = main.c message.c cache.c

# Header files
HEADERS = message.h cache.h

# Object files
OBJ = main.o message.o cache.o

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile main.o
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c

# Compile message.o
message.o: message.c message.h
	$(CC) $(CFLAGS) -c message.c

# Compile cache.o
cache.o: cache.c cache.h message.h
	$(CC) $(CFLAGS) -c cache.c

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJ)
