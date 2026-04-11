#include <kernel/vga.h>
#include <kernel/io.h>
#include <common/memutil.h>

typedef struct {
  uint8_t character;
  uint8_t attribute;
} __attribute__((packed)) vga_entry_t;

static volatile vga_entry_t* vga = (vga_entry_t*)0xB8000;
static uint8_t color = VGA_ENTRY_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static void vga_scroll(void) {

memmove((void *)vga, (const void *)(vga + VGA_WIDTH), (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(vga_entry_t));

for (uint32_t col = 0; col < VGA_WIDTH; col++){
  uint32_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
  vga[index].character = ' ';
  vga[index].attribute = color;
}

cursor_y = VGA_HEIGHT - 1;
cursor_x = 0;
}

static void vga_update_cursor(void) {
  uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void vga_pchar(uint8_t c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c == '\r') {
    cursor_x = 0;
  } else if(c == '\t') {
    cursor_x += 4 - (cursor_x % 4);
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
  vga_update_cursor();
}

void vga_write(const uint8_t* str) {
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

void vga_write_hex64(uint64_t val) {
    const char *hex = "0123456789ABCDEF";

    vga_write("0x");

    for (int i = 0; i < 16; i++) {
        uint64_t shift = (uint64_t)(15 - i) * 4;
        uint64_t nibble = (val >> shift) & 0xFULL;
        vga_pchar(hex[nibble]);
    }
}

void vga_write_dec(uint32_t val) {
  char buffer[10];
  int i = 0;

  if (val == 0){
    vga_pchar('0');
    return;
  }

  while (val > 0) {
    buffer[i++] = '0' + (val % 10);
    val /= 10;
  }

  while (i > 0) {
    vga_pchar(buffer[--i]);
  }
}

void vga_write_dec64(uint64_t val) {
  char buffer[20];
  int i = 0;

  if (val == 0) {
    vga_pchar('0');
    return;
  }

  while (val > 0) {
    buffer[i++] = '0' + (val % 10);
    val /= 10;
  }

  while (i > 0) {
    vga_pchar(buffer[--i]);
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
  vga_update_cursor();
}
