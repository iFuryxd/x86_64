#include <common/memutil.h>
#include <kernel/vga.h>
void *memset(void *passed_dest, int passed_val, size_t n) {
  uint8_t *dest = (uint8_t *)passed_dest;
  uint8_t val = (uint8_t)passed_val;

  for (size_t i = 0; i < n; i++) {
    dest[i] = val;
  }

  return dest;
}

void *memcpy(void *passed_dest, const void *passed_src, size_t n) {
#ifdef KERNEL_DEBUG
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nCASTING passed_dest to dest. void* -> uint8_t *");
#endif
  uint8_t *dest = (uint8_t *)passed_dest;
#ifdef KERNEL_DEBUG
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nCASTING passed_src to src. void* -> uint8_t *");
#endif
  const uint8_t *src = (uint8_t *)passed_src;
#ifdef KERNEL_DEBUG
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nENTERING LOOP");
#endif
  for (size_t i = 0; i < n; i++) {
#ifdef KERNEL_DEBUG
    vga_write("\nITERATION ");
    vga_write_dec(i);
#endif
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
