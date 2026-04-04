#include <kernel/vga.h>

typedef struct {
  uint8_t character;
  uint8_t attribute;
} __attribute__((packed)) vga_entry_t;

static volatile vga_entry_t* vga = (vga_entry_t*)0xB8000;
static uint8_t color = 0x0F;

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static void vga_scroll(void) {
for (uint32_t row = 1; row < VGA_HEIGHT; row++){
  for (uint32_t col = 0; col < VGA_WIDTH; col++){
    vga[(row - 1) * VGA_WIDTH + col] = vga[row * VGA_WIDTH + col];
  }
}
for (uint32_t col = 0; col < VGA_WIDTH; col++){
  uint32_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
  vga[index].character = ' ';
  vga[index].attribute = color;
}
cursor_y = VGA_HEIGHT - 1;
cursor_x = 0;
}

static void vga_pchar(char c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else {
    uint32_t index = cursor_y * VGA_WIDTH + cursor_x;
    
    vga[index].character = c;
    vga[index].attribute = color;

    cursor_x++;
  }

  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }

  if (cursor_y >= VGA_HEIGHT) {
    vga_scroll();
  }
}

void vga_write(const char *str) {
  if (str == NULL) return;
  while (*str) {
    vga_pchar(*str++);
  }
}

void vga_write_hex(uint32_t val) {
    const char *hex = "0123456789ABCDEF";

    vga_write("0x");

    for (int i = 0; i < 8; i++) {
        uint32_t shift = (7 - i) * 4;
        uint32_t nibble = (val >> shift) & 0xF;
        vga_pchar(hex[nibble]);
    }
}

void vga_set_color(uint8_t new_color) {
  color = new_color;
}
uint8_t vga_make_color(uint8_t fg, uint8_t bg) {
return (bg << 4) | fg;
}
void vga_clear(void) {
  for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga[i].character = ' ';
    vga[i].attribute = color;
  }
  cursor_x = 0;
  cursor_y = 0;
}
