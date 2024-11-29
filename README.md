# Xenigma 🔐

A lightweight C program that provides file encryption using XOR cipher security. Simple yet effective for basic encryption needs.

## Features

- File encryption using XOR cipher
- Support for both text and binary files
- Customizable key storage
- Simple command-line interface
- Fast and efficient processing

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

## Default Behavior

- Encryption output: `<input_file>.xor`
- Decryption output: `out.txt` (text) or `file.out` (binary)
- Key file: `xor.key`

## Important Notes

1. The program will overwrite output files without warning
2. Keep your key file safe - it's required for decryption
3. Use the same key file that was used for encryption when decrypting
4. Binary mode (-b) must be used consistently for encryption and decryption

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

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Security Notice

XOR encryption is a simple cipher and should not be used for serious security needs. This tool is for educational purposes and basic encryption tasks only.
