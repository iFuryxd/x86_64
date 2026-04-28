#ifndef SERIAL_DEBUG_H
#define SERIAL_DEBUG_H

#include <kernel/arch/x86_64/serial.h>
#include <common/types.h>

void serial_debug(const char* msg, const uint64_t value);
#endif