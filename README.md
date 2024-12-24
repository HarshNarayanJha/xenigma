# Xenigma

#### Video Demo: https://youtu.be/Nf8Cx3qXS-Y

#### Description: A lightweight C program that provides file encryption using XOR cipher security. Simple yet effective for basic encryption needs.

## Features

- File encryption using XOR cipher
- Support for both text and binary files
- Customizable key storage
- Simple command-line interface
- Fast and efficient processing

## Working

This CLI project written in C is a simple encryption program that uses the XOR encryption approach. It is a very simple encryption algorithm, but not very secure.
It works on the principle of the XOR (^) operator. It is a simple bitwise operator that outputs 1 if both of the inputs are different.

| A   | B   | Y   |
| --- | --- | --- |
| 0   | 0   | 0   |
| 0   | 1   | 1   |
| 1   | 0   | 1   |
| 1   | 1   | 0   |

When applied on bigger numbers, we can notice a very crucial yet intriguing property about it, if with XOR a number with some key, and then XOR the result with the same key, we get the original number as output!

(NOTE that ^ refers to the XOR operator, and not exponentiation in this document)

35 ^ 6 = 37
37 ^ 6 = 35

This happens because of the binary representation of the numbers

35 = 00100011
06 = 00000110
--------------- ^
37 = 00100101

The 6 acts as a mask and flips the 2nd and 3rd bits (from right).

37 = 00100101
06 = 00000110
--------------- ^
35 = 00100011

And then the same mask again flips the 2nd and 3rd bits (from right), reverting it to the original number.

Now let's imagine if we apply this to a file, reading all of its contents, byte by byte, and using a 1 byte number as the key, we can do xor on each byte of the file, and write the resulting buffer to a new file.
This file will be encrypted and by no means it's contents can be read without having the key first. The same 1 byte key will be used to decrypt the file back using the exact same operation.

Though in my program, the 1 byte key (8-bits) as very less span, and can be easily brute-forced on the encrypted file until we get some meaningful data back.
But in real world scenarios, keys are long, effectively 128-bits or more, and they are very hard to brute force, taking unimaginable years in practice for even the fastest computers out there.

The program source can be found in the `src/main.c` file, which is written in the C language, and compiled with the Makefile at `Makefile`, on my system using the `gcc` compiler, version `14.2.1 20240910 (GCC)` on `Arch Linux`.
The program starts with the required includes for stdlib headers, and then declares some constants for the default filenames and extensions, if not supplies by the user, and a default read buffer size.
We have 3 methods, `help()`, `encrypt()` and `decrypt()` for displaying help, encrypting, and decrypting respectively.

The `main` method start with reading and parsing command line arguments and setting various flags read by the user.
Such as `-f` for input file name (required), `-o` for output filename (defaults to `.xor` extension appended to input), and `-k` for key filename (default to `xor.key`) to store the key used for encryption.
Then either a `-e` or `-d` for encrypting or decrypting function. And optional `-b` flag can be used to instruct the program to treat encryption input and decrypted output files as binary (default is ASCII for encryption input and decryption output).

One should keep the **key file safe and secure, and it should not be shared with anyone**. It can be and is used to decrypt your sensitive encrypted files.

Afterwards, the respective function kicks off.

1. Encrypt

The encrypt function starts with generating a random 1 byte key (from 0-255) using `rand() % 256`. Since computers use pseudorandom generators, they can be predictive.
To mitigate this, we might first randomize the randomizer using some random data, such as the current timestamp (which will be unique at any point in time) using `srand(time(NULL));`.
After calculating the file names, we print the informative message `printf("Will read from %s and write encrypted content to %s. Writing key to %s.\n", ifile, ofile, kfile);` and start the process.

The program open all files, writes the key and closes it. Then it starts reading from the input file into a buffer, xor's each byte with the KEY and writes to the output file using `buffer[i] ^= KEY;`

Afterwards it closes all the files, and frees up all the allocated memory.

2. Decrypt

The decrypt function follows a very similar procedure as the encrypt function, starts by calculating the filenames.
Then we print the informative message `printf("Will read from %s and write decrypted content to %s using the key from %s.\n", ifile, ofile, kfile);` and start the process.
The program open all files, reads the key and closes it. Then it starts reading from the input file into a buffer, xor's each byte with the KEY and writes to the output file using `buffer[i] ^= KEY;`

Afterwards it closes all the files, and frees up all the allocated memory.

## Installation

1. Clone the repository:

```bash
git clone https://github.com/yourusername/xenigma.git
cd xenigma
```

2. Build the project:

```bash
make
```

The executable will be created in the `bin` directory.

## Usage

### Encryption

```bash
xenigma -f file.txt -o encrypted.xor -k my_secret.key -e
```

- `-f`: Input file (required)
- `-o`: Output file (optional, default: `<input_file>.xor`)
- `-k`: Key file location (optional, default: `xor.key`)
- `-e`: Encrypt mode flag

### Decryption

```bash
xenigma -f encrypted.xor -o decrypted.txt -k my_secret.key -d
```

- `-f`: Input file (required)
- `-o`: Output file (optional, default: `out.txt` or `file.out` for binary)
- `-k`: Key file location (optional, default: `xor.key`)
- `-d`: Decrypt mode flag

### Binary Files

Use the `-b` flag when dealing with binary files:

```bash
xenigma -f image.png -e -b
xenigma -f image.png.xor -d -b
```

## Command Line Options

| Option      | Description                              |
| ----------- | ---------------------------------------- |
| `-f <file>` | Input file to encrypt/decrypt (required) |
| `-o <file>` | Output file location (optional)          |
| `-k <file>` | Key file location (optional)             |
| `-e`        | Encrypt mode                             |
| `-d`        | Decrypt mode                             |
| `-b`        | Binary file mode                         |
| `-h`        | Show help message                        |

## Default Behaviour

- Encryption output: `<input_file>.xor`
- Decryption output: `out.txt` (text) or `file.out` (binary)
- Key file: `xor.key`

## Important Notes

1. The program will overwrite output files without warning
2. Keep your key file safe — it's required for decryption
3. Use the same key file that was used for encryption when decrypting
4. Binary mode (-b) must be used consistently for encryption and decryption.

## Examples

1. Encrypt a text file:

```bash
xenigma -f secret.txt -e
```

2. Decrypt using custom filenames:

```bash
xenigma -f encrypted.xor -o decrypted.txt -k my_key.key -d
```

3. Handle binary files:

```bash
xenigma -f image.jpg -e -b
xenigma -f image.jpg.xor -d -b
```

## Building from Source

```bash
# Clone the repository
git clone https://github.com/HarshNarayanJha/xenigma.git

# Navigate to project directory
cd xenigma

# Build the project
make

# Clean build files
make clean
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Licence

This project is licensed under the MIT Licence — see the [LICENCE](LICENSE) file for details.

## Security Notice

XOR encryption is a simple cipher and should not be used for serious security needs. This tool is for educational purposes and basic encryption tasks only.
