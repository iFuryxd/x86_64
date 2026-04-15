#ifndef GDT_H
#define GDT_H

#include <common/types.h>

//Structure for a GDT entry, since all of them have the same structure I can use something like this.
//Looks bigger than some GDT entry models because codex said I need to have more fields since the CPU expects this kind of format
typedef struct __attribute__((packed)) {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} gdt_entry;


//Actual GDT structure with necessary entries based on standart 32-bit GDT models
typedef struct __attribute__((packed)) {
    gdt_entry null_d;
    gdt_entry kernel_code;
    gdt_entry kernel_data;
    gdt_entry user_code;
    gdt_entry user_data;
    gdt_entry tss;
} gdt_32;


//Minimal GDT for long mode jump, will change latter to include user_code64/data and tss
typedef struct __attribute__((packed)) {
    gdt_entry null_d;
    gdt_entry kernel_code64;
    gdt_entry kernel_data;
}gdt_64;

//Setup each entry based on the following parameters: base, limit, access, flags
gdt_entry gdt_make_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif