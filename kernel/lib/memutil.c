#include <common/memutil.h>

void *memset(void *passed_dest, int passed_val, size_t n) {
  uint8_t *dest = (uint8_t *)passed_dest;
  uint8_t val = (uint8_t)passed_val;

  for (size_t i = 0; i < n; i++) {
    dest[i] = val;
  }

  return dest;
}

void *memcpy(void *passed_dest, const void *passed_src, size_t n) {
  uint8_t *dest = (uint8_t *)passed_dest;
  const uint8_t *src = (uint8_t *)passed_src;
  for (size_t i = 0; i < n; i++) {
    dest[i] = src[i];
  }

  return dest;
}

void *memmove(void *passed_dest, const void *passed_src, size_t n) {
  uint8_t *dest = (uint8_t *)passed_dest;
  const uint8_t *src = (uint8_t *)passed_src;

  if (dest < src) {
    for (size_t i = 0; i < n; i++) {
      dest[i] = src[i];
    }
  } else if (dest > src) {
    for (size_t i = n; i > 0; i--) {
      dest[i - 1] = src[i - 1];
    }
  }

  return dest;
}
