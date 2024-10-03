CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -mconsole

all: genrand polybius_cipher encrypt_process

genrand: genrand.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o genrand genrand.o

polybius_cipher: polybius_cipher.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o polybius_cipher polybius_cipher.o

encrypt_process: encrypt_process.o queue.o readfile.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o encrypt_process encrypt_process.o queue.o readfile.o

genrand.o: genrand.c
	$(CC) $(CFLAGS) -c genrand.c

polybius_cipher.o: polybius_cipher.c
	$(CC) $(CFLAGS) -c polybius_cipher.c

encrypt_process.o: encrypt_process.c queue.h readfile.h
	$(CC) $(CFLAGS) -c encrypt_process.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

readfile.o: readfile.c readfile.h
	$(CC) $(CFLAGS) -c readfile.c

clean:
	rm -f *.o genrand polybius_cipher encrypt_process
