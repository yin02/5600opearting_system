# Makefile for CS5600 - Week X (Windows version)

# Compiler and flags
CC = gcc
CFLAGS = -pthread

# Targets
all: multithread producer_consumer

# Part 1 - multithread program
multithread: multithread.c
	$(CC) $(CFLAGS) -o multithread.exe multithread.c

# Part 3 - Producer Consumer problem
producer_consumer: pqueuepmain.c
	$(CC) $(CFLAGS) -o producer_consumer.exe pqueuepmain.c

# Clean up executables
clean:
	del multithread.exe producer_consumer.exe
