#ifndef PRINT_H
#define PRINT_H

#include <kernel/arch/x86_64/vga.h>
#include <stdint.h>

#define AS_KERNEL  "\nKERNEL: "
#define AS_ERR     "\nERROR: "
#define AS_PMM     "\nPMM: "
#define AS_MBI     "\nMBI: "
#define AS_VMM     "\nVMM: "
#define AS_VLD     "\nVALIDATOR: "
#define AS_CPUID   "\nCPUID: "
#define AS_USR     "\nUSER: "
#define AS_NONE    "\n"


enum color{
black,
green,
cyan,
red,
magenta,
brown,
l_gray,
d_gray,
l_blue,
l_green,
l_cyan,
l_red,
l_magenta,
l_brown,
white,
};


void print(const char* caller, const char *msg, enum color color);


#endif
