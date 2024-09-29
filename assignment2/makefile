CC = gcc
CFLAGS = -Wall

all: genrand cipher

genrand: genrand.c
	$(CC) $(CFLAGS) genrand.c -o genrand

cipher: cipher.c polybius.c
	$(CC) $(CFLAGS) cipher.c polybius.c -o cipher

clean:
	rm -f genrand cipher

