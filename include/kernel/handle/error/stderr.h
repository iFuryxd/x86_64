#ifndef STDERR_H
#define STDERR_H

#include <common/types.h>

typedef struct {
    const char *subsystem;
    uint32_t code;
    const char *details;
} err;

#endif