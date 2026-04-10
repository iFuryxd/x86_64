#ifndef UTIL_H
#define UTIL_H

#include <common/types.h>
#include <kernel/vga.h>

static void halt(void) {
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("\nKERNEL_EVENT: HALTING");
    __asm__ volatile("cli; hlt");
}

static void delay(volatile uint64_t amount) {
    while(amount--)
    __asm__ volatile("nop");
}

#endif