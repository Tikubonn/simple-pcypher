
#include <stddef.h>
#include <simple-pcypher/simple-pcypher.h>
#include "test.h"

static void testcase (){
  size_t encrypteddatasize;
  TEST(simple_pcypher_calc_encrypted_data_size(0, &encrypteddatasize) == 0, "");
  TEST(encrypteddatasize == 2, "");
}

static void testcase2 (){
  size_t encrypteddatasize;
  TEST(simple_pcypher_calc_encrypted_data_size(100, &encrypteddatasize) == 0, "");
  TEST(encrypteddatasize == 101, "");
}

static void testcase3 (){
  size_t encrypteddatasize;
  TEST(simple_pcypher_calc_encrypted_data_size(SIZE_MAX, &encrypteddatasize) != 0, "");
  TEST(simple_pcypher_errno == SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_BEST_PRIME_NUMBER);
}

void test_simple_pcypher_calc_encrypted_data_size (){
  testcase();
  testcase2();
  testcase3();
}
