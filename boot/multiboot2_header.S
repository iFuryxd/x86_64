.intel_syntax noprefix

.section .multiboot, "a"
.align 8

.global multiboot_header

multiboot_header:
    .long 0xE85250D6
    .long 0
    .long header_end - multiboot_header
    .long -(0xE85250D6 + (header_end - multiboot_header))

    /* Request the Multiboot2 memory map tag. */
    .short 1
    .short 0
    .long 16
    .long 6
    .long 0

    .short 0
    .short 0
    .long 8

header_end:

.section .note.GNU-stack,"",@progbits
