# xenigma

It's Enigma with XOR security - a C program that encrypts your files using XOR encryption.

### Usage

For encryption:

```shell
xenigma -f file.txt -o file.out -k save_key_to.key
```

-o and -k are optional parameters. -f is required.
Default output filename: `<input_file_name>.xor`
Default key filename: `xor.key`

For decryption:

```shell
xenigma -f in.file -o file.txt -k read_key_from.key
```

-o and -k are optional parameters. -f is required.
Default output filename: `out.txt` (or `file.out` for binary files)
Default key filename: `xor.key`

Use -b flag to mark the original file (and thus the decrypted file) as binary.

Note: Input file can be any file type, not just text files.

### Help

```shell
C utility to encrypt your files with XOR encryption security.
Usage: xenigma -f <input_file> [-o <output_file>] [-k <key_file>] -e|-d [-b] [-h]
Options:
  -f <input_file>    Input file to encrypt/decrypt (required)
  -o <output_file>   Output file (autodetermined - input_file.xor for encrypt, file.txt/file.out for decrypt). WARNING: File will be overwritten
  -k <key_file>      Key file location to save/read from (default: xor.key)
  -e                 Encrypt the input file
  -d                 Decrypt the input file
  -b                 Specifies that the original file is in binary, not plaintext
  -h                 Show this help message
```
