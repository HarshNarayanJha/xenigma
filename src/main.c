#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const char *ENCRYPT_AUTO_EXTENSION = ".xor";
const char *KEY_FILE_NAME = "xor.key";

const char *DECRYPT_FILE_NAME = "out.txt";
const char *DECRYPT_FILE_NAME_BINARY = "file.out";
const int BUFFER_SIZE = 4096;

void help(char *const basename);
bool encrypt(char *ifile, char *ofile, char *kfile, bool is_binary);
bool decrypt(char *ifile, char *ofile, char *kfile, bool is_binary);

int main(int argc, char *const argv[]) {
  int opt;

  char *ifile = NULL;
  char *ofile = NULL;
  char *kfile = NULL;

  bool is_encrypt = false;
  bool is_decrypt = false;

  bool is_binary = false;

  while ((opt = getopt(argc, argv, "f:o:k:hedb")) != -1) {
    switch (opt) {
    case 'h':
      help(argv[0]);
      return 0;
    case 'f':
      ifile = malloc(strlen(optarg) + 1);
      strcpy(ifile, optarg);
      break;
    case 'o':
      ofile = malloc(strlen(optarg) + 1);
      strcpy(ofile, optarg);
      break;
    case 'k':
      kfile = malloc(strlen(optarg) + 1);
      strcpy(kfile, optarg);
      break;
    case 'e':
      is_encrypt = true;
      break;
    case 'd':
      is_decrypt = true;
      break;
    case 'b':
      is_binary = true;
      break;
    case '?':
      return 0;
    }
  }

  if (is_encrypt == false && is_decrypt == false) {
    printf("Either of -e or -d must be passed.\n");
    return 1;
  }

  if (is_encrypt == true && is_decrypt == true) {
    printf("You passed both -e and -d. Invalid call.\n");
    return 1;
  }

  if (is_encrypt == true) {
    if (encrypt(ifile, ofile, kfile, is_binary)) {
      printf("File successfully encrypted. Keep the key safe!\n");
    }
    return 0;
  }

  if (is_decrypt == true) {
    if (decrypt(ifile, ofile, kfile, is_binary)) {
      printf("File successfully decrypted. Keep the file safe!\n");
    }
    return 0;
  }

  free(ifile);
  free(ofile);
  free(kfile);
}

bool encrypt(char *ifile, char *ofile, char *kfile, bool is_binary) {
  // randomize randomizer
  srand(time(NULL));

  // 1 byte random key
  unsigned char KEY = rand() % 256;

  if (ifile == NULL) {
    printf("Didn't pass the input file\n");
    return false;
  }

  if (ofile == NULL) {
    // calculate output file name based on input file
    ofile = malloc(strlen(ifile) + strlen(ENCRYPT_AUTO_EXTENSION) + 1);
    strcpy(ofile, ifile);
    strcat(ofile, ENCRYPT_AUTO_EXTENSION);
  }

  if (kfile == NULL) {
    // calculate key file name
    kfile = malloc(strlen(KEY_FILE_NAME) + 1);
    strcpy(kfile, KEY_FILE_NAME);
  }

  printf("Will read from %s and write encrypted content to %s. Writing key to %s.\n", ifile, ofile, kfile);

  FILE *input;
  FILE *output;
  FILE *keyf;

  if (is_binary) {
    printf("Reading input in binary mode.\n");
    input = fopen(ifile, "rb");
  } else {
    input = fopen(ifile, "r");
  }

  if (input == NULL) {
    printf("%s couldn't be opened for reading. You sure it exists?\n", ifile);
    return false;
  }

  output = fopen(ofile, "wb");
  if (output == NULL) {
    printf("%s couldn't be opened for writing.\n", ofile);
    return false;
  }

  keyf = fopen(kfile, "wb");
  if (keyf == NULL) {
    printf("%s couldn't be opened for writing.\n", kfile);
    return false;
  }

  // write to xor.key and close it
  fwrite(&KEY, sizeof(char), 1, keyf);
  fclose(keyf);

  char buffer[BUFFER_SIZE];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < bytes_read; i++) {
      buffer[i] ^= KEY;
    }

    fwrite(buffer, sizeof(char), bytes_read, output);
  }

  // close file streams
  fclose(output);
  fclose(input);

  return true;
}

bool decrypt(char *ifile, char *ofile, char *kfile, bool is_binary) {
  if (ifile == NULL) {
    printf("Didn't pass the input file to decrypt\n");
    return false;
  }

  if (ofile == NULL) {
    // calculate output file name based on input file
    if (is_binary) {
      ofile = malloc(strlen(DECRYPT_FILE_NAME_BINARY) + 1);
      strcpy(ofile, DECRYPT_FILE_NAME_BINARY);
    } else {
      ofile = malloc(strlen(DECRYPT_FILE_NAME) + 1);
      strcpy(ofile, DECRYPT_FILE_NAME);
    }
  }

  if (kfile == NULL) {
    // calculate key file name
    kfile = malloc(strlen(KEY_FILE_NAME) + 1);
    strcpy(kfile, KEY_FILE_NAME);
  }

  printf("Will read from %s and write decrypted content to %s using the key from %s.\n", ifile, ofile, kfile);

  FILE *input;
  FILE *output;
  FILE *keyf;

  input = fopen(ifile, "rb");
  if (input == NULL) {
    printf("%s couldn't be opened for reading. You sure it exists?\n", ifile);
    return false;
  }

  if (is_binary) {
    printf("Writing output in binary mode.\n");
    output = fopen(ofile, "wb");
  } else {
    output = fopen(ofile, "w");
  }

  if (output == NULL) {
    printf("%s couldn't be opened for writing.\n", ofile);
    return false;
  }

  keyf = fopen(kfile, "rb");
  if (keyf == NULL) {
    printf("%s couldn't be opened for reading. You sure it exists?\n", kfile);
    return false;
  }

  // read the key
  unsigned char KEY;
  size_t key_read = fread(&KEY, sizeof(char), 1, keyf);
  fclose(keyf);

  if (key_read != 1) {
    printf("Couldn't read key from %s. Error", kfile);
    return false;
  }

  printf("Using key %b\n", KEY);

  // read, decrypt, and write!
  char buffer[BUFFER_SIZE];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < bytes_read; i++) {
      buffer[i] ^= KEY;
    }

    fwrite(buffer, sizeof(char), bytes_read, output);
  }

  // close file streams
  fclose(output);
  fclose(input);

  return true;
}

void help(char *const basename) {
  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                         XENIGMA                            ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  printf("A utility to encrypt your files with XOR encryption security.\n\n");

  printf("USAGE:\n");
  printf("  %s -f <input_file> [-o <output_file>] [-k <key_file>] -e|-d [-b]\n\n", basename);

  printf("OPTIONS:\n");
  printf("  -f <input_file>    Input file to encrypt/decrypt (required)\n");
  printf("  -o <output_file>   Output file location (optional)\n");
  printf("                     Defaults:\n");
  printf("                     • Encrypt: <input_file>.xor\n");
  printf("                     • Decrypt: out.txt or file.out (binary)\n");
  printf("  -k <key_file>      Key file location (default: xor.key)\n");
  printf("  -e                 Encrypt the input file\n");
  printf("  -d                 Decrypt the input file\n");
  printf("  -b                 Binary mode (use for non-text files)\n");
  printf("  -h                 Show this help message\n\n");

  printf("EXAMPLES:\n");
  printf("  Encrypt:   %s -f secret.txt -e\n", basename);
  printf("  Decrypt:   %s -f secret.txt.xor -d\n", basename);
  printf("  Binary:    %s -f image.jpg -e -b\n\n", basename);

  printf("WARNING:\n");
  printf("  • Output files will be overwritten without confirmation\n");
  printf("  • Keep your key file safe - it's required for decryption\n");
  printf("  • Binary mode (-b) must be used consistently\n\n");
}
