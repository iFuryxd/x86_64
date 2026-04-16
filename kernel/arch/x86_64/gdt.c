#include <kernel/arch/x86_64/gdt.h>
#include <common/print.h>
#include <kernel/util.h>

static gdt_32 gdt32;
static gdtr_32 gdtr32;

gdt_entry gdt_make_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt_entry entry = {
        0,
        0,
        0,
        0,
        0,
        0,
    };
    if (limit > 0xFFFFF) {
        error("GDT CANNOT ENCODE LIMITS LARGER THAN 0xFFFFF");
        halt();
    }
    entry.limit_low = limit & 0xFFFF;
    entry.base_low = base & 0xFFFF;
    entry.base_mid = (base >> 16) & 0xFF;
    entry.access = access;
    entry.limit_high_flags = ((limit >> 16) & 0x0F) | ((flags & 0x0F) << 4);
    entry.base_high = (base >> 24) & 0xFF;

    return entry;
}

void gdt_setup(void) {
    gdt32.null_d = gdt_make_entry(0, 0, 0, 0);
    gdt32.kernel_code = gdt_make_entry(0, 0xFFFF, 0x9A, 0xC);
    gdt32.kernel_data = gdt_make_entry(0, 0xFFFF, 0x92, 0xC);
    gdt32.user_code = gdt_make_entry(0, 0xFFFF, 0xFA, 0xC);
    gdt32.user_data = gdt_make_entry(0, 0xFFFF, 0xF2, 0xC);
    
    gdtr32.limit = sizeof(gdt32) - 1;
    gdtr32.base = (uint32_t)&gdt32;
}

void gdt_load(void) {
    __asm__ __volatile__("lgdt %0" : : "m" (gdtr32));
}

const gdtr_32 *gdt_get_gdtr(void) {
    return &gdtr32;
}