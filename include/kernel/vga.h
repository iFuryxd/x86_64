#ifndef VGA_H
#define VGA_H

#include <common/types.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *vga_buffer = (uint16_t *)0xB8000;
static uint8_t color = 0x0F;

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static void putchar(char c);
void print(const char *str);
void print_hex(uint32_t val);
void clear(void);
#endif
