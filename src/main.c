#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const char *AUTO_EXTENSION = ".xor";
const char *KEY_FILE_NAME = "xor.key";
const int BUFFER_SIZE = 100;

void help(char *const basename);
bool encrypt(char *ifile, char *ofile, char *kfile);
bool decrypt(char *ifile, char *ofile, char *kfile);

int main(int argc, char *const argv[]) {
  int opt;

  char *ifile = NULL;
  char *ofile = NULL;
  char *kfile = NULL;

  bool is_encrypt = false;
  bool is_decrypt = false;

  while ((opt = getopt(argc, argv, "f:okhed")) != -1) {
    switch (opt) {
    case 'h':
      help(argv[0]);
      return 0;
    case 'f':
      ifile = optarg;
      break;
    case 'o':
      ofile = optarg;
      break;
    case 'k':
      kfile = optarg;
      break;
    case 'e':
      is_encrypt = true;
      break;
    case 'd':
      is_decrypt = true;
      break;
    case '?':
      printf("Unknown option %s\n", optarg);
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
    if (encrypt(ifile, ofile, kfile)) {
      printf("File successfully encrypted. Keep the key safe!\n");
    }
    return 0;
  }

  if (is_decrypt == true) {
    if (decrypt(ifile, ofile, kfile)) {
      printf("File successfully decrypted. Keep the file safe!\n");
    }
    return 0;
  }
}

bool encrypt(char *ifile, char *ofile, char *kfile) {
  // randomize randomizer
  srand(time(NULL));

  // 8 bytes random key
  int KEY = rand() % 256;

  if (ifile == NULL) {
    printf("Didn't pass the input file\n");
    return false;
  }

  if (ofile == NULL) {
    // calculate output file name based on input file
    ofile = (char *)malloc(sizeof(char) * (strlen(ifile) + strlen(AUTO_EXTENSION)));
    strcat(ofile, ifile);
    strcat(ofile, AUTO_EXTENSION);
  }

  if (kfile == NULL) {
    // calculate key file name
    kfile = (char *)malloc(sizeof(char) * (strlen(KEY_FILE_NAME)));
    strcat(kfile, KEY_FILE_NAME);
  }

  printf("Will read from %s and write encrypted content to %s. Writing key to "
         "%s.\n",
         ifile, ofile, kfile);

  FILE *input;
  FILE *output;
  FILE *keyf;

  input = fopen(ifile, "r");
  if (input == NULL) {
    printf("%s couldn't be opened for reading. You sure it exists?\n", ifile);
    return false;
  }

  output = fopen(ofile, "wb");
  if (output == NULL) {
    printf("%s couldn't be opened for writing.\n", ofile);
    return false;
  }

  keyf = fopen(kfile, "w");
  if (keyf == NULL) {
    printf("%s couldn't be opened for writing.\n", kfile);
    return false;
  }

  // write to xor.key and close it
  fwrite(&KEY, sizeof(char), 8, keyf);
  fclose(keyf);

  char buffer[BUFFER_SIZE];
  size_t read;

  bool wrote_successfully = true;

  while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, input))) {
    if (ferror(input)) {
      printf("Error reading %s", ifile);
      wrote_successfully = false;
      break;
    }

    for (int i = 0; i < BUFFER_SIZE; i++) {
      if (buffer[i] == '\0')
        break;

      char input_file_c = buffer[i];
      char output_file_c = input_file_c ^ KEY;
      buffer[i] = output_file_c;
    }

    // now encrypt the buffer and write to output file.
    fwrite(buffer, sizeof(char), BUFFER_SIZE, output);
  }

  // close file streams
  fclose(output);
  fclose(input);

  return wrote_successfully;
}

bool decrypt(char *ifile, char *ofile, char *kfile) {}

void help(char *const basename) {
  printf("C utility to encrypt your files with XOR encryption security.\n");
  printf("Usage: %s -f <input_file> [-o <output_file>] [-k <key_file>] [-e|-d] [-h]\n", basename);
  printf("Options:\n");
  printf("  -f <input_file>    Input file to encrypt/decrypt (required)\n");
  printf("  -o <output_file>   Output file (autodetermined - input_file.xor for encrypt, file.out for decrypt)\n");
  printf("  -k <key_file>      Key file location to save/read from (default: xor.key)\n");
  printf("  -e                 Encrypt the input file\n");
  printf("  -d                 Decrypt the input file\n");
  printf("  -h                 Show this help message\n");
}
