#include <kernel/vga.h>

static uint16_t *vga_buffer = (uint16_t *)0xB8000;
static uint8_t color = 0x0F;

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static void putchar(char c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
    return;
  } else {
    uint32_t index = cursor_y * VGA_WIDTH + cursor_x;
    vga_buffer[index] = (color << 8) | (uint8_t)c;

    cursor_x++;
  }

  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }

  if (cursor_y >= VGA_HEIGHT) {
    cursor_y = VGA_HEIGHT - 1;
  }
}

void print(const char *str) {
  while (*str) {
    putchar(*str++);
  }
}

void print_hex(uint32_t val) {
  const char *hex = "0123456789ABCDEF";

  print("0x");
  for (int i = 7; i >= 0; i--) {
    uint32_t shift = i * 4;
    uint32_t nibble = (val >> shift) & 0xF;
    putchar(hex[nibble]);
  }
}

void clear(void) {
  for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_buffer[i] = (color << 8) | ' ';
  }
  cursor_x = 0;
  cursor_y = 0;
}
