
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <simple-pcypher/simple-pcypher.h>
#include "buffer.h"

typedef enum cypher_mode {
  CYPHER_ENCRYPTION,
  CYPHER_DECRYPTION,
} cypher_mode;

static inline int parse_uint_as_hexadecimal (const char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 16;
      value += str[index] - '0';
    }
    else 
    if ('a' <= str[index] && str[index] <= 'f'){
      value *= 16;
      value += str[index] - 'a' + 16;
    }
    else 
    if ('A' <= str[index] && str[index] <= 'F'){
      value *= 16;
      value += str[index] - 'A' + 16;
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing hexadecimal.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static inline int parse_uint_as_digits (const char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 10;
      value += str[index] - '0';
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing digits.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static inline int parse_uint (const char *str, uintmax_t *valuep){
  if (str[0] == '0'){
    if (str[1] == 'x'){
      return parse_uint_as_hexadecimal(str +2, valuep);
    }
    else {
      return parse_uint_as_digits(str, valuep);
    }
  }
  else {
    return parse_uint_as_digits(str, valuep);
  }
}

static inline int parse_args (int argc, char **argv, cypher_mode *modep, FILE **inputp, FILE **outputp, size_t *positionp, size_t *sizep, bool *givensizep, simple_pcypher_key *keybasep, bool *showhelpp, bool *showversionp){
  cypher_mode mode = CYPHER_ENCRYPTION;
  bool givenmode = false;
  char *inputfile = NULL;
  char *outputfile = NULL;
  uintmax_t position = 0;
  bool givenposition = false;
  uintmax_t size;
  bool givensize = false;
  uintmax_t keybase;
  bool givenkeybase = false;
  bool showhelp = false; 
  bool showversion = false;
  size_t index = 1;
  while (index < argc){
    if (strcmp(argv[index], "--help") == 0){
      showhelp = true;
      break;
    }
    else 
    if (strcmp(argv[index], "--version") == 0){
      showversion = true;
      break;
    }
    else 
    if (strcmp(argv[index], "-e") == 0){
      if (!givenmode){
        mode = CYPHER_ENCRYPTION;
        givenmode = true;
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a parameter -e or -d.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-d") == 0){
      if (!givenmode){
        mode = CYPHER_DECRYPTION;
        givenmode = true;
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a parameter -d or -d.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-p") == 0){
      if (index +1 < argc){
        if (!givenposition){
          if (parse_uint(argv[index +1], &position)){
            return 1;
          }
          givenposition = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -p %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -p.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-s") == 0){
      if (index +1 < argc){
        if (!givensize){
          if (parse_uint(argv[index +1], &size)){
            return 1;
          }
          givensize = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -s %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -s.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-k") == 0){
      if (index +1 < argc){
        if (!givenkeybase){
          if (parse_uint(argv[index +1], &keybase)){
            return 1;
          }
          givenkeybase = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -k %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -k.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-o") == 0){
      if (index +1 < argc){
        if (outputfile == NULL){
          outputfile = argv[index +1];
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a output file \"%s\".\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -o.\n");
        return 1;
      }
    }
    else {
      if (inputfile == NULL){
        inputfile = argv[index];
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a input file \"%s\".\n", argv[index]);
        return 1;
      }
    }
  }
  if (showhelp){
    *modep = CYPHER_ENCRYPTION;
    *inputp = stdin;
    *outputp = stdout;
    *givensizep = false;
    *showhelpp = true;
    *showversionp = false;
    return 0;
  }
  else 
  if (showversion){
    *modep = CYPHER_ENCRYPTION;
    *inputp = stdin;
    *outputp = stdout;
    *givensizep = false;
    *showhelpp = false;
    *showversionp = true;
    return 0;
  }
  else {
    if (givenkeybase){
      FILE *input;
      if (inputfile == NULL){
        input = stdin;
      }
      else {
        input = fopen(inputfile, "rb");
        if (input == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", inputfile, errno);
          return 1;
        }
      }
      FILE *output;
      if (outputfile == NULL){
        output = stdout;
      }
      else {
        output = fopen(outputfile, "wb");
        if (output == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", outputfile, errno);
          if (fclose(input) == EOF){
            fprintf(stderr, "Could not close file \"%s\" (errno = %d).\n", inputfile, errno);
          }
          return 1;
        }
      }
      *modep = mode;
      *inputp = input;
      *outputp = output;
      *positionp = position;
      *sizep = size;
      *givensizep = givensize;
      *keybasep = keybase;
      *showhelpp = false;
      *showversionp = false;
      return 0;
    }
    else {
      fprintf(stderr, "Require a -k parameter.\n");
      return 1;
    }
  }
}

const char HELP_MESSAGE[] = 
"Usage: simple-pcypher [OPTION]... -k KEY [FILE]\n"
"Encrypt or decrypt FILE with simple-pcypher.\n"
"\n"
"OPTION:\n"
"  -o file: An output file. default is stdout.\n"
"  -p position: An position to read start. It use to decryption mode.\n"
"  -s size: An read size. It use to decryption mode.\n"
"  --help: Show help then exit.\n"
"  --version: Show version then exit.\n"
"KEY: An unsigned integer used to common key. You can use digits or hexadecimal format like as 123 and 0x123abc.\n"
"FILE: An input file. default is stdin.\n"
;

const char VERSION_MESSAGE[] = 
"simple-pcypher 0.1.0\n"
;

int main (int argc, char **argv){
  cypher_mode mode;
  FILE *input;
  FILE *output;
  size_t position;
  size_t size;
  bool givensize;
  simple_pcypher_key keybase;
  bool showhelp;
  bool showversion;
  if (parse_args(argc, argv, &mode, &input, &output, &position, &size, &givensize, &keybase, &showhelp, &showversion)){
    return 1;
  }
  if (showhelp){
    size_t writtensize = fwrite(HELP_MESSAGE, 1, sizeof(HELP_MESSAGE), stdout);
    if (writtensize < sizeof(HELP_MESSAGE)){
      if (ferror(stdout)){
        fprintf(stderr, "Caused some error at writing into a file.\n");
        return 1;
      }
    }
    return 0;
  }
  else 
  if (showversion){
    size_t writtensize = fwrite(VERSION_MESSAGE, 1, sizeof(VERSION_MESSAGE), stdout);
    if (writtensize < sizeof(VERSION_MESSAGE)){
      if (ferror(stdout)){
        fprintf(stderr, "Caused some error at writing into a file.\n");
        return 1;
      }
    }
    return 0;
  }
  else {
    buffer buffer;
    buffer_setup(&buffer);
    while (true){
      uint8_t readdata[4096];
      size_t readsize = fread(readdata, 1, sizeof(readdata), input);
      if (readsize < sizeof(readdata)){
        if (ferror(input)){
          fprintf(stderr, "Caused some error at reading a file.\n");
          return 1;
        }
        if (readsize == 0){
          break;
        }
      }
      if (buffer_write(readdata, readsize, &buffer)){
        return 1;
      }
    }
    uint8_t *bufferdata;
    size_t buffersize;
    buffer_data(&buffer, &bufferdata, &buffersize);
    switch (mode){
      case CYPHER_ENCRYPTION: {
        size_t encrypteddatasize;
        if (simple_pcypher_calc_encrypted_data_size(buffersize, &encrypteddatasize)){
          fprintf(stderr, "Could not find a prime number used to encrypted data size by %zu.\n", buffersize);
          return 1;
        }
        simple_pcypher_key key;
        if (simple_pcypher_calc_key(keybase, encrypteddatasize, &key)){
          fprintf(stderr, "Could not find a prime number used to cypher key by %zu and %zu.\n", keybase, encrypteddatasize);
          return 1;
        }
        uint8_t *encrypteddata = malloc(encrypteddatasize);
        if (encrypteddata == NULL){
          fprintf(stderr, "Could not allocate memory (size = %zu).\n", encrypteddatasize);
          return 1;
        }
        if (simple_pcypher_encrypt(bufferdata, buffersize, key, encrypteddata, encrypteddatasize)){
          return 1;
        }
        size_t writtensize = fwrite(encrypteddata, 1, encrypteddatasize, output);
        if (writtensize < encrypteddatasize){
          fprintf(stderr, "Caused some error at writing into a file.\n");
          return 1;
        }
        return 0;
      }
      case CYPHER_DECRYPTION: {
        size_t finalizedsize = givensize? size: buffersize;
        simple_pcypher_key key;
        if (simple_pcypher_calc_key(keybase, finalizedsize, &key)){
          fprintf(stderr, "Could not find a prime number used to cypher key by %zu and %zu.\n", keybase, finalizedsize);
          return 1;
        }
        uint8_t *decrypteddata = malloc(finalizedsize);
        if (decrypteddata == NULL){
          fprintf(stderr, "Could not allocate memory (size = %zu).\n", finalizedsize);
          return 1;
        }
        if (simple_pcypher_decrypt(position, finalizedsize, bufferdata, buffersize, key, decrypteddata)){
          return 1;
        }
        size_t writtensize = fwrite(decrypteddata, 1, finalizedsize, output);
        if (writtensize < finalizedsize){
          fprintf(stderr, "Caused some error at writing into a file.\n");
          return 1;
        }
        return 0;
      }
      default:
        fprintf(stderr, "Unknown cypher mode was given (%d).\n", mode);
        return 1;
    }
  }
}
