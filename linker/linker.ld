ENTRY(_start)

SECTIONS {
  . = 1M; /* Kernel entry point 1MB */
  
  .multiboot ALIGN(8) : {
    KEEP(*(.multiboot))
    }
  .text ALIGN(4K) :
    {
    *(.text*)
    }
  .rodata ALIGN(4K) :
    {
    *(.rodata*)
    }
  .data ALIGN(4K) :
    {
    *(.data*)
    }
  .bss ALIGN(4K) :
    {
    *(COMMON)
    *(.bss*)
    }
}
