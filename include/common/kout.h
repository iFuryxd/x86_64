#ifndef KOUT_H
#define KOUT_H

#include <kernel/vga.h>
#include <stdint.h>

#define KERNEL  (uint8_t*)"KERNEL: "
#define ERR     (uint8_t*)"ERROR: "
#define PMM     (uint8_t*)"PMM: "
#define VMM     (uint8_t*)"VMM: "
#define VLD     (uint8_t*)"VALIDATOR: "
#define NONE    (uint8_t*)""
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

typedef struct {
uint8_t* writeas;
}__writeas;

void print(__writeas as, uint8_t* msg, enum color color);
void println(__writeas, uint8_t* msg, enum color color);

#endif