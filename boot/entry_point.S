.intel_syntax noprefix

.set CODE64_SEL, 0x08
.set DATA64_SEL, 0x10
.set IA32_EFER, 0xC0000080

.global _start
.extern kernel64_main

.section .text
.code32
_start:
    cli

    mov dword ptr [boot_magic], eax
    mov dword ptr [multiboot_info_ptr], ebx

    lea esp, [boot_stack_top]

    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    mov eax, offset pdp_table
    or eax, 0x3
    mov dword ptr [pml4_table], eax
    mov dword ptr [pml4_table + 4], 0

    mov eax, offset pd_table
    or eax, 0x3
    mov dword ptr [pdp_table], eax
    mov dword ptr [pdp_table + 4], 0

    mov dword ptr [pd_table], 0x00000083
    mov dword ptr [pd_table + 4], 0
    mov dword ptr [pd_table + 8], 0x00200083
    mov dword ptr [pd_table + 12], 0

    lgdt [gdt64_descriptor]

    mov eax, offset pml4_table
    mov cr3, eax

    mov ecx, IA32_EFER
    rdmsr
    or eax, (1 << 8)
    wrmsr

    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    push CODE64_SEL
    push offset long_mode_start
    retf

.code64
long_mode_start:
    mov ax, DATA64_SEL
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, offset boot_stack_top
    mov edi, dword ptr [rip + boot_magic]
    mov esi, dword ptr [rip + multiboot_info_ptr]
    call kernel64_main

.hang:
    cli
    hlt
    jmp .hang

.section .rodata
.align 8
gdt64:
    .quad 0x0000000000000000
    .quad 0x00A09A0000000000
    .quad 0x00C0920000000000
gdt64_end:

gdt64_descriptor:
    .word gdt64_end - gdt64 - 1
    .long gdt64

.section .bss
.align 4096
pml4_table:
    .skip 4096

.align 4096
pdp_table:
    .skip 4096

.align 4096
pd_table:
    .skip 4096

.align 16
boot_magic:
    .skip 4

multiboot_info_ptr:
    .skip 4

boot_stack_bottom:
    .skip 16384

boot_stack_top:

.section .note.GNU-stack,"",@progbits
