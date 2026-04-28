#ifndef CPUID_H
#define CPUID_H

#include <common/types.h>

typedef struct {
kbool_t cpuid_exists;
uint64_t max_bleaf;
uint64_t max_eleaf;
kbool_t has_msr;
kbool_t has_pae;
kbool_t has_long_mode;
} cpuid_info;


cpuid_info cpuid_probe(void);

#endif
