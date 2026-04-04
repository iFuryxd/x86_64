#include <common/types.h>
#include <kernel/vga.h>
#include <kernel/multiboot_info.h>
void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("KERNEL_EVENT: STARTING KERNEL\n");
  if (magic != 0x36D76289) {
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL_EVENT: BAD MAGIC");
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_write("magic=");
    vga_write_hex(magic);
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL_EVENT: PANIC");
    while (1) {
    }
  }
  vga_write("KERNEL_EVENT: MAGIC CHECK PASSED\n");
  vga_write("magic=");
  vga_write_hex(magic);
  vga_write("\nmultiboot_info=");
  vga_write_hex(multiboot_info);
  vga_write("\nKERNEL_EVENT: PARSING MULTIBOOT_INFO\n");
  parse_mmap(multiboot_info, 0);
  while(1) {
    __asm__ volatile("cli; hlt");
  }
}
