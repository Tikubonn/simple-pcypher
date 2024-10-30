
#include "test-simple-pcypher-calc-encrypted-data-size.h"
#include "test-simple-pcypher-calc-key.h"
#include "test-simple-pcypher-encrypt-decrypt.h"

int main (){
  test_simple_pcypher_calc_encrypted_data_size();
  test_simple_pcypher_calc_key();
  test_simple_pcypher_encrypt_decrypt();
  return 0;
}
