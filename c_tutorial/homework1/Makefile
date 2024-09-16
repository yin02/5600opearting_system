# Compiler settings - Can be customized.
CC = gcc
CFLAGS = -Wall -g

# Executable names
EXE1 = learnc
EXE2 = arger

# Source files
SRC1 = learnc.c mpg2km.c
SRC2 = arger.c

# Object files
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

all: $(EXE1) $(EXE2)

# Target for EXE1
$(EXE1): $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $^

# Target for EXE2
$(EXE2): $(OBJ2)
	$(CC) $(CFLAGS) -o $@ $^

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# To remove generated files
clean:
	rm -f $(EXE1) $(EXE2) *.o

# Dependencies
learnc.o: mpg2km.h
mpg2km.o: mpg2km.h
arger.o:
