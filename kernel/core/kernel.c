#include "../../include/common/types.h"

void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  volatile char *vga = (volatile char *)0xB8000;
  vga[0] = '0';
  vga[1] = 0x0F;
  vga[2] = 'K';
  vga[3] = 0x0F;

  if (magic != 0x36D76289) {
    vga[0] = 'P';
    vga[1] = 0x0F;
    vga[2] = 'A';
    vga[3] = 0x0F;
    vga[4] = 'N';
    vga[5] = 0x0F;
    vga[6] = 'I';
    vga[7] = 0x0F;
    vga[8] = 'C';
    vga[9] = 0x0F;
    while (1) {
    }
  }

  while (1) {
    __asm__ volatile("hlt");
  }
}
