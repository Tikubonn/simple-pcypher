
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <simple-pcypher/simple-pcypher.h>

#define PLAINTEXT "This is plaintext!"

int main (){

  size_t encrypteddatasize;
  simple_pcypher_calc_encrypted_data_size(sizeof(PLAINTEXT), &encrypteddatasize);

  simple_pcypher_key key;
  simple_pcypher_calc_key(0x123, encrypteddatasize, &key);

  uint8_t encrypteddata[encrypteddatasize];
  simple_pcypher_encrypt(PLAINTEXT, sizeof(PLAINTEXT), key, encrypteddata, encrypteddatasize);
  printf("encrypted = ");
  fwrite(encrypteddata, encrypteddatasize, 1, stdout);
  putc('\n', stdout);

  uint8_t decrypteddata[sizeof(PLAINTEXT)];
  simple_pcypher_decrypt(0, sizeof(PLAINTEXT), encrypteddata, encrypteddatasize, key, decrypteddata);
  printf("decrypted = %s\n", decrypteddata);

  return 0;

}
