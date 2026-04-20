#include <common/print.h>
#include <kernel/arch/x86_64/vga.h>
#include <kernel/util.h>
#include <common/types.h>

void kernel64_main(uint32_t magic, uint32_t multiboot_info) {
    vga_clear();
    print(AS_KERNEL, "ENTERED 64-bit KERNEL", l_green);
    printhex("multiboot magic=", magic);
    printhex("multiboot info=", multiboot_info);
    for (;;) {
        halt();
    }
}
