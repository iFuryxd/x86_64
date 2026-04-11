#ifndef UTIL_H
#define UTIL_H

#include <common/types.h>
#include <common/print.h>

static void halt(void) {
    print(AS_KERNEL,"HALTING", l_red);
    __asm__ volatile("cli; hlt");
}

static void delay(volatile uint64_t amount) {
    while(amount--)
    __asm__ volatile("nop");
}

#endif
