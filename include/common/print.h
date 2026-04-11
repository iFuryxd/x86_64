#ifndef PRINT_H
#define PRINT_H

#include <kernel/vga.h>
#include <stdint.h>

#define AS_KERNEL  (uint8_t*)"\nKERNEL: "
#define AS_ERR     (uint8_t*)"\nERROR: "
#define AS_PMM     (uint8_t*)"\nPMM: "
#define AS_MBI     (uint8_t*)"\nMBI: "
#define AS_VMM     (uint8_t*)"\nVMM: "
#define AS_VLD     (uint8_t*)"\nVALIDATOR: "
#define AS_USR     (uint8_t*)"\nUSER: "
#define AS_NONE    (uint8_t*)"\n"


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


void print(uint8_t* caller, const char *msg, enum color color);


#endif
