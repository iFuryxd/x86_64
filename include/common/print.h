#ifndef PRINT_H
#define PRINT_H

#include <kernel/arch/x86_64/vga.h>
#include <common/types.h>

#define AS_KERNEL  "\nKERNEL: "
#define AS_ERR     "\nERROR: "
#define AS_WARN    "\nWARNING: "
#define AS_PMM     "\nPMM: "
#define AS_MBI     "\nMBI: "
#define AS_PAG32   "\nPAGER 32: "
#define AS_PAG64   "\nPAGER 64: "
#define AS_VLD     "\nVALIDATOR: "
#define AS_CPUID   "\nCPUID: "
#define AS_ELMD    "\nELMD: "
#define AS_USR     "\nUSER: "
#define AS_TEST    "\nTEST: "
#define AS_NONE    "\n"

enum color{
black,
green,
cyan,
red,
magenta,
brown,
yellow,
l_gray,
d_gray,
l_blue,
l_green,
l_cyan,
l_red,
l_magenta,
l_yellow,
white,
};


void print(const char* caller, const char *msg, enum color color);
void error(const char* msg);
void warn(const char* msg);
void printhex(const char* msg, const uint64_t value);
void printdec(const char* msg, const uint64_t value);

#endif
