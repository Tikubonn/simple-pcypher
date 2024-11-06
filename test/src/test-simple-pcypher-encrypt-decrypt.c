
#include <stddef.h>
#include <stdint.h>
#include <simple-pcypher/simple-pcypher.h>
#include "test.h"
#include "test-simple-pcypher-encrypt-decrypt.h"

static void testcase (){
  uint8_t plaintext[256] = {0};
  size_t encrypteddatasize;
  TEST(simple_pcypher_calc_encrypted_data_size(sizeof(plaintext), &encrypteddatasize) == 0, "");
  TEST(encrypteddatasize == 257, "");
  uint8_t encrypteddata[encrypteddatasize];
  simple_pcypher_key key;
  TEST(simple_pcypher_calc_key(123, encrypteddatasize, &key) == 0, "");
  TEST(key == 127, "");
  TEST(simple_pcypher_encrypt(plaintext, sizeof(plaintext), key, encrypteddata, encrypteddatasize) == 0, "");
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(plaintext); index++){
    samecount += plaintext[index] == encrypteddata[index];
  }
  TEST(samecount < 257, "");

  //decrypt full.

  {
    uint8_t decrypteddata[sizeof(encrypteddata)];
    TEST(simple_pcypher_decrypt(0, sizeof(encrypteddata), encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < sizeof(plaintext); index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
    for (size_t index = sizeof(plaintext); index < sizeof(decrypteddata); index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt partially.

  {
    uint8_t decrypteddata[128];
    TEST(simple_pcypher_decrypt(0, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially2.

  {
    uint8_t decrypteddata[128];
    TEST(simple_pcypher_decrypt(128, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index +128], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially3.

  {
    uint8_t decrypteddata[1];
    TEST(simple_pcypher_decrypt(256, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 1; index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt (always error).

  {
    uint8_t decrypteddata[1024];
    TEST(simple_pcypher_decrypt(0, 1024, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
    TEST(simple_pcypher_errno == SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE);
  }

  //decrypt (always error2).

  {
    uint8_t decrypteddata[1];
    TEST(simple_pcypher_decrypt(1024, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
    TEST(simple_pcypher_errno == SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE);
  }
}

static void testcase2 (){
  uint8_t plaintext[256];
  for (size_t index = 0; index < sizeof(plaintext); index++){
    plaintext[index] = index & 0xff;
  }
  size_t encrypteddatasize;
  TEST(simple_pcypher_calc_encrypted_data_size(sizeof(plaintext), &encrypteddatasize) == 0, "");
  TEST(encrypteddatasize == 257, "");
  uint8_t encrypteddata[encrypteddatasize];
  simple_pcypher_key key;
  TEST(simple_pcypher_calc_key(123, encrypteddatasize, &key) == 0, "");
  TEST(key == 127, "");
  TEST(simple_pcypher_encrypt(plaintext, sizeof(plaintext), key, encrypteddata, encrypteddatasize) == 0, "");
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(plaintext); index++){
    samecount += plaintext[index] == encrypteddata[index];
  }
  TEST(samecount < 257, "");

  //decrypt full.

  {
    uint8_t decrypteddata[sizeof(encrypteddata)];
    TEST(simple_pcypher_decrypt(0, sizeof(encrypteddata), encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < sizeof(plaintext); index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
    for (size_t index = sizeof(plaintext); index < sizeof(decrypteddata); index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt partially.

  {
    uint8_t decrypteddata[128];
    TEST(simple_pcypher_decrypt(0, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially2.

  {
    uint8_t decrypteddata[128];
    TEST(simple_pcypher_decrypt(128, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index +128], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially3.

  {
    uint8_t decrypteddata[1];
    TEST(simple_pcypher_decrypt(256, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 1; index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt (always error).

  {
    uint8_t decrypteddata[1024];
    TEST(simple_pcypher_decrypt(0, 1024, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
    TEST(simple_pcypher_errno == SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE);
  }

  //decrypt (always error2).

  {
    uint8_t decrypteddata[1];
    TEST(simple_pcypher_decrypt(1024, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
    TEST(simple_pcypher_errno == SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE);
  }
}

void test_simple_pcypher_encrypt_decrypt (){
  testcase();
  testcase2();
}
