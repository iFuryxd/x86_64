#include <common/types.h>
#include <kernel/pmm/multiboot_parser.h>
#include <kernel/pmm/pmm.h>
#include <kernel/vga.h>

#define MAGIC 0x36D76289

static void halt(void) {
  while (true) {
    __asm__ volatile("cli; hlt");
  }
}

void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("KERNEL_EVENT: STARTING KERNEL\n");
  if (magic != MAGIC) {
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL_PANIC: BAD MAGIC");
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    vga_write("magic=");
    vga_write_hex(magic);
    vga_write("\n");
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL_PANIC: HALTING");
    halt();
  }
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
  vga_write("DEBUG_PRINT: MAGIC VALUE CHECK PASSED\n");
  vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
  vga_write("magic=");
  vga_write_hex(magic);
  vga_write("\nmultiboot_info=");
  vga_write_hex(multiboot_info);
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nKERNEL_EVENT: PARSING MULTIBOOT_INFO\n");
  parse_mmap(multiboot_info);
#ifdef KERNEL_DEBUG
  dump_memory_regions();
#endif

  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
  vga_write("\nKERNEL_EVENT: INITIALIZING PMM\n");
  pmm_init();

#ifdef KERNEL_DEBUG
  // dump_pmm_info();
#endif

  halt();
}
