
#include <stddef.h>
#include <stdint.h>

#ifndef MAX
#define MAX(a, b) ((a)<(b)?(b):(a))
#endif

typedef struct buffer {
  uint8_t *data;
  size_t seek;
  size_t size;
} buffer;

static inline void buffer_setup (buffer *buffer){
  buffer->data = NULL;
  buffer->seek = 0;
  buffer->size = 0;
}

static inline int _buffer_stretch (size_t datasize, buffer *buffer){
  size_t extendedsize = buffer->size;
  while (extendedsize < buffer->seek + datasize){
    extendedsize = MAX(1024, extendedsize * 2);
  }
  if (buffer->size != extendedsize){
    void *extendeddata = realloc(buffer->data, extendedsize);
    if (extendeddata == NULL){
      return 1;
    }
    buffer->data = extendeddata;
    buffer->size = extendedsize;
    return 0;
  }
  else {
    return 0;
  }
}

static inline int buffer_write (const void *data, size_t datasize, buffer *buffer){
  if (_buffer_stretch(datasize, buffer)){
    return 1;
  }
  for (size_t index = 0; index < datasize; index++){
    buffer->data[buffer->seek + index] = ((uint8_t*)data)[index];
  }
  buffer->seek += datasize;
  return 0;
}

static inline void buffer_clear (buffer *buffer){
  free(buffer->data);
  buffer->data = NULL;
  buffer->seek = 0;
  buffer->size = 0;
}

static inline void buffer_data (buffer *buffer, uint8_t **datap, size_t *datasizep){
  *datap = buffer->data;
  *datasizep = buffer->seek;
}
