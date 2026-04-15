#include <kernel/util.h>

void halt(void) {
    print(AS_KERNEL,"HALTING", l_red);
    __asm__ volatile("cli; hlt");
}

void delay(volatile uint64_t amount) {
    while(amount--)
    __asm__ volatile("nop");
}