#ifndef SERIAL_H
#define SERIAL_H

#include <common/types.h>

void serial_init(void);
void serial_write_char(char c);
void serial_write(const char* str);

void serial_write_hex64(uint64_t val);
void serial_write_dec64(uint64_t val);

#endif