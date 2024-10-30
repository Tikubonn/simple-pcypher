
#include <stddef.h>
#include <simple-pcypher/simple-pcypher.h>
#include "test.h"
#include "test-simple-pcypher-calc-key.h"

static void testcase (){
  simple_pcypher_key key;
  TEST(simple_pcypher_calc_key(0, 101, &key) == 0, "");
  TEST(key == 2, "");
}

static void testcase2 (){
  simple_pcypher_key key;
  TEST(simple_pcypher_calc_key(101, 101, &key) == 0, "");
  TEST(key == 103, "");
}

static void testcase3 (){
  simple_pcypher_key key;
  TEST(simple_pcypher_calc_key(0, SIZE_MAX, &key) != 0, "");
}

void test_simple_pcypher_calc_key (){
  testcase();
  testcase2();
  testcase3();
}
