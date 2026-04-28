#include <kernel/arch/x86_64/serial.h>
#include <kernel/arch/x86_64/io.h>


#define COM1 0x3F8

static int serial_ready(void) {
    return inb(COM1 + 5) & 0x20;
}

void serial_init() {
    outb(COM1 + 1, 0x00); // disable interrupts
    outb(COM1 + 3, 0x80); // enable DLAB
    outb(COM1 + 0, 0x03); // divisor low  = 3
    outb(COM1 + 1, 0x00); // divisor high = 0
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // enable FIFO
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void serial_write_char(char c) {
    while(!serial_ready()) { }
    outb(COM1, c);
}

void serial_write(const char *str) {
    while (*str) {
        if (*str == '\n')
            serial_write_char('\r');
        serial_write_char(*str++);
    }
}

void serial_write_hex64(uint64_t val) {
    const char *hex = "0123456789ABCDEF";

    serial_write("0x");

    for (int i = 0; i < 16; i++) {
        uint64_t shift = (uint64_t)(15 - i) * 4;
        uint64_t nibble = (val >> shift) & 0xFULL;
        serial_write_char(hex[nibble]);
    }
}

void serial_write_dec64(uint64_t val) {
  char buffer[20];
  int i = 0;

  if (val == 0) {
    serial_write_char('0');
    return;
  }

  while (val > 0) {
    buffer[i++] = '0' + (val % 10);
    val /= 10;
  }

  while (i > 0) {
    serial_write_char(buffer[--i]);
  }
}