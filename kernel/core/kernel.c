#include <common/types.h>
#include <kernel/pmm/parse_mbi.h>
#include <kernel/pmm/pmm.h>
#include <kernel/vmm/vmm.h>
#include <kernel/vga.h>
#include <kernel/util.h>


#define MAGIC 0x36D76289


void kernel_32main(uint32_t magic, uint32_t multiboot_info) {
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("KERNEL: STARTING KERNEL");
  if (magic != MAGIC) {
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL: PANIC -> BAD MAGIC");
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    vga_write("magic=");
    vga_write_hex(magic);
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL: HALTING");
    halt();
  }
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nKERNEL: PARSING MBI");
  parse_mbi(multiboot_info);

  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nKERNEL: INITIALIZING PMM");
  pmm_init();

  vga_write("\nKERNEL: INITIALIZING VMM");
  vmm_init();

  halt();
}
