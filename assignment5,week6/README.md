


# Random String Encryption with Polybius Cipher

## Project Overview

This project demonstrates how to generate random strings, store them in a queue, and encrypt them using a Polybius cipher. The encryption is processed in batches of 100 strings, with multiple processes handling the encryption. The project uses inter-process communication (IPC) and queue data structures to achieve efficient encryption.

## Files

- **genrand.c**: Generates 10,000 random strings and writes them to a file (`random_strings.txt`).
- **queue.c / queue.h**: Implements a queue data structure to manage the strings.
- **readfile.c / readfile.h**: Reads strings from `random_strings.txt` and stores them in a queue.
- **encrypt_process.c**: Processes strings in batches, spawns child processes to encrypt using the Polybius cipher.
- **polybius_cipher.c**: Implements the Polybius cipher used to encrypt the strings.
- **Makefile**: Contains the build rules to compile the project.
- **README.md**: This document, explaining how to run and test the program.

## Compilation Instructions

To compile the program, run the following command in the root directory of the project:

```bash
make
```

This will generate the following executables:
- `genrand.exe`: Generates random strings and saves them to `random_strings.txt`.
- `polybius_cipher.exe`: The Polybius cipher program.
- `encrypt_process.exe`: Reads strings from `random_strings.txt` and encrypts them in batches of 100.

## How to Run

### Step 1: Generate Random Strings
Run the following command to generate 10,000 random strings:

```bash
genrand.exe
```

This will create a file `random_strings.txt` containing 10,000 random strings.

### Step 2: Encrypt the Strings
Run the following command to encrypt the strings:

```bash
encrypt_process.exe
```

This will read strings from `random_strings.txt`, process them in batches of 100, and use multiple processes to encrypt each batch using `polybius_cipher.exe`. Encrypted strings will be stored in files named `encrypted_batch_*.txt`.

## Edge Cases Handled

- **Empty `random_strings.txt`**: The program handles an empty input file without crashing.
- **Missing `random_strings.txt`**: If the file does not exist, the program outputs an appropriate error message.
- **Missing `polybius_cipher.exe`**: If the cipher executable is missing, the program will detect this and exit with an error message.

## Testing the Program

### Basic Test
1. Run `genrand.exe` to generate random strings.
2. Run `encrypt_process.exe` to encrypt those strings.
3. Verify the output in the generated `encrypted_batch_*.txt` files.

### Edge Case Testing
1. **Empty File Test**: Clear the contents of `random_strings.txt` and run `encrypt_process.exe`. The program should handle this gracefully.
2. **File Not Found**: Rename or delete `random_strings.txt` and run `encrypt_process.exe`. It should output a meaningful error message.
3. **Cipher Program Missing**: Rename or delete `polybius_cipher.exe` and run `encrypt_process.exe`. It should handle the missing executable with an error message.

