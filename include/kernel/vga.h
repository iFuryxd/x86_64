#ifndef VGA_H
#define VGA_H

#include <common/types.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void print(const char *str);
void print_hex(uint32_t val);
void clear(void);
#endif
