#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *AUTO_EXTENSION = ".xor";
const int BUFFER_SIZE = 100;

void help(char *const basename);

int main(int argc, char *const argv[]) {
  int opt;

  char *ifile = NULL;
  char *ofile = NULL;

  while ((opt = getopt(argc, argv, "f:oh")) != -1) {
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
    }
  }

  if (ifile == NULL) {
    printf("Didn't pass the input file\n");
    return 1;
  }

  if (ofile == NULL) {
    // calculate output file name based on input file
    ofile =
        (char *)malloc(sizeof(char) * (strlen(ifile) + strlen(AUTO_EXTENSION)));
    strcat(ofile, ifile);
    strcat(ofile, AUTO_EXTENSION);
  }

  printf("Will read from %s and write encrypted content to %s\n", ifile, ofile);

  FILE *input;
  FILE *output;

  input = fopen(ifile, "r");
  if (input == NULL) {
    printf("%s couldn't be opened for reading. You sure it exists?\n", ifile);
    return 1;
  }

  output = fopen(ofile, "wb");
  if (output == NULL) {
    printf("%s couldn't be opened for writing. You sure it exists?\n", ofile);
    return 1;
  }

  char buffer[BUFFER_SIZE];
  size_t read;

  while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, input))) {
    if (ferror(input)) {
      printf("Error reading %s", ifile);
      break;
    }

    // now encrypt the buffer and write to output file.
  }

  // close file streams
  fclose(output);
  fclose(input);
}

void help(char *const basename) {
  printf("C utility to encrypt your files with XOR encryption security.\n");
  printf("Usage: %s -f <input_file> [-o <output_file>] [-h]\n", basename);
  printf("Options:\n");
  printf("  -f <input_file>    Input file to encrypt (required)\n");
  printf(
      "  -o <output_file>   Output file for encrypted data (autodetermined)\n");
  printf("  -h                 Show this help message\n");
}
