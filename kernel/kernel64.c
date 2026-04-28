#include <kernel/arch/x86_64/cpuid.h>
#include <kernel/arch/x86_64/serial.h>
#include <kernel/mm/pmm.h>
#include <common/print.h>
#include <kernel/arch/x86_64/vga.h>
#include <kernel/util.h>
#include <common/types.h>
#include <kernel/mm/parse_mbi.h>

#include <common/serial_debug.h>


#define MAGIC 0x36D76289

void kernel64_main(uint64_t magic, uint64_t multiboot_info) {
    vga_clear();
    print(AS_KERNEL, "ENTERED 64-bit KERNEL", l_green);
    if (magic != MAGIC) {
        err error = {
            .subsystem = "KERNEL",
            .code = MAGIC,
            .details = "fatal error. passed invalid uint64_t magic value to:\n\tvoid kernel64_main(uint64_t magic, uint64_t multiboot_info). \nmagic != MAGIC(0x36D76289)",
        };
        panic(&error);
    }
    cpuid_probe();
    print(AS_KERNEL, "PARSING MBI", l_green);
    parse_mbi(multiboot_info);

    serial_write("memory regions: ");
    for (uint64_t index = 0; index < memory_region_count; index++) {
        serial_debug("\nbase = ", memory_regions[index].base);
        serial_debug("\nlen = ", memory_regions[index].len);
        serial_debug("\ntype = ", memory_regions[index].type);
    }

    
    print(AS_KERNEL, "RUNNING INITIALIZER FOR PMM", l_green);
    pmm_init();
    print(AS_KERNEL, "FETCHING INFORMATION", l_green);
    pmm_info *retrieved_info = pmm_fetch_info();
    for (;;) {
        halt();
    }
}
