.intel_syntax noprefix

.section .text
.global _start
.extern kernel_main

_start:
    cli

    mov esp, stack_top

    push ebx   /* multiboot info */
    push eax   /* magic */

    call kernel_main

.hang:
    hlt
    jmp .hang

.section .bss
.align 16

stack_bottom:
    .skip 16384

stack_top:
