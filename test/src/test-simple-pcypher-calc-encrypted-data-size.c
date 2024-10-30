
#include <stddef.h>
#include <simple-pcypher/simple-pcypher.h>
#include "test.h"
#include "test-simple-pcypher-calc-encrypted-data-size.h"

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
}

void test_simple_pcypher_calc_encrypted_data_size (){
  testcase();
  testcase2();
  testcase3();
}
