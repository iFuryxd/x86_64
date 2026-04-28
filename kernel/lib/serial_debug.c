#include "kernel/arch/x86_64/serial.h"
#include <common/serial_debug.h>

void serial_debug(const char *msg, const uint64_t value) {
    serial_write(msg);
    serial_write_dec64(value);
}