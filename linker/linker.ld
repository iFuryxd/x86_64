OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

SECTIONS {
  . = 1M;
  __kernel_start = .;
  
  .multiboot : ALIGN(8) {
    KEEP(*(.multiboot))
  }
  .text : ALIGN(4096)
    {
    *(.text*)
    }
  .rodata : ALIGN(4096)
    {
    *(.rodata*)
    }
  .data : ALIGN(4096)
    {
    *(.data*)
    }
  .bss : ALIGN(4096)
    {
    *(COMMON)
    *(.bss*)
    }
    . = ALIGN(4096);
  __kernel_end = .;
}
