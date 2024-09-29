CC=gcc
CFLAGS=-Wall

all: allocmem loadmem

allocmem: allocmem.c
	$(CC) $(CFLAGS) -o allocmem allocmem.c

loadmem: loadmem.c
	$(CC) $(CFLAGS) -o loadmem loadmem.c

clean:
	rm -f allocmem loadmem  # Clean up compiled files
