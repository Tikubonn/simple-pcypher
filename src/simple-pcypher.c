
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <threads.h>
#include "simple-pcypher.h"

thread_local _simple_pcypher_errno simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;

char __stdcall *simple_xcypher_errno_message (_simple_xcypher_errno errno){
  switch (errno){
    case SIMPLE_PCYPHER_ERRNO_NONE:
      return "None.";
    case SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_BEST_PRIME_NUMBER:
      return "Could not find best prime number.";
    case SIMPLE_PCYPHER_ERRNO_KEY_AND_BLOCK_ARE_OVERSIZE: 
      return "Cypher key and block are oversize. Those's product must be lesser than SIZE_MAX.";
    case SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE: 
      return "Index out of range.";
    default:
      return "Unknown errno given.";
  }
}

static inline bool is_prime_number (size_t number){
  switch (number){
    case 0:
    case 1:
      return false;
    case 2:
      return true;
    default:
      if (number % 2 == 0){
        return false;
      }
      else {
        size_t max = ceil(sqrt(number));
        for (size_t n = 3; n <= max; n += 2){
          if (number % n == 0){
            return false;
          }
        }
        return true;
      }
  }
}

int __stdcall simple_pcypher_calc_encrypted_data_size (size_t datasize, size_t *encrypteddatasizep){
  for (size_t n = datasize; n < SIZE_MAX; n++){
    if (is_prime_number(n)){
      *encrypteddatasizep = n;
      simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;
      return 0;
    }
  }
  simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_PRIME_NUMBER;
  return 1;
}

int __stdcall simple_pcypher_calc_key (simple_pcypher_key keybase, size_t encrypteddatasize, simple_pcypher_key *keyp){
  for (simple_pcypher_key n = keybase; n < SIMPLE_PCYPHER_KEY_MAX && n <= SIZE_MAX / encrypteddatasize; n++){
    if (n != encrypteddatasize && is_prime_number(n)){
      *keyp = n;
      simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;
      return 0;
    }
  }
  for (simple_pcypher_key n = SIZE_MAX / encrypteddatasize; 0 < n; n--){
    if (n != encrypteddatasize && is_prime_number(n)){
      *keyp = n;
      simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;
      return 0;
    }
  }
  simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_COULD_NOT_FIND_PRIME_NUMBER;
  return 1;
}

static inline uint64_t xorlshift (uint64_t number){
  number ^= number << 13;
  number ^= number >> 7;
  number ^= number << 17;
  return number;
}

static inline void calc_index_and_mask (size_t index, size_t size, simple_pcypher_key key, size_t *indexp, uint8_t *maskp){
  *indexp = (index + key) % size * key % size;
  *maskp = xorlshift((index + key) % size * key);
}

int __stdcall simple_pcypher_encrypt (const void *data, size_t datasize, simple_pcypher_key key, void *encrypteddata, size_t encrypteddatasize){
  if (key <= SIZE_MAX / encrypteddatasize){
    for (size_t index = 0; index < datasize; index++){
      size_t index2;
      uint8_t mask;
      calc_index_and_mask(index, encrypteddatasize, key, &index2, &mask);
      ((uint8_t*)encrypteddata)[index2] = ((uint8_t*)data)[index] ^ mask;
    }
    for (size_t index = datasize; index < encrypteddatasize; index++){
      size_t index2;
      uint8_t mask;
      calc_index_and_mask(index, encrypteddatasize, key, &index2, &mask);
      ((uint8_t*)encrypteddata)[index2] = ((uint8_t*)data)[index % datasize] ^ mask;
    }
    simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;
    return 0;
  }
  else {
    simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_KEY_AND_BLOCK_ARE_OVERFLOW;
    return 1;
  }
}

int __stdcall simple_pcypher_decrypt (size_t position, size_t size, const void *data, size_t datasize, simple_pcypher_key key, void *decrypteddata){
  if (key <= SIZE_MAX / datasize){
    if (position + size <= datasize){
      for (size_t index = 0; index < size; index++){
        size_t index2;
        uint8_t mask;
        calc_index_and_mask(position + index, datasize, key, &index2, &mask);
        ((uint8_t*)decrypteddata)[index] = ((uint8_t*)data)[index2] ^ mask;
      }
      simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_NONE;
      return 0;
    }
    else {
      simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_INDEX_OUT_OF_RANGE;
      return 1;
    }
  }
  else {
    simple_pcypher_errno = SIMPLE_PCYPHER_ERRNO_KEY_AND_BLOCK_ARE_OVERFLOW;
    return 1;
  }
}
