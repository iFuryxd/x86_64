#include <kernel/arch/x86_64/gdt.h>
#include <common/print.h>
#include <kernel/util.h>

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
