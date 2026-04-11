#include <common/types.h>
#include <kernel/pmm/parse_mbi.h>
#include <kernel/pmm/pmm.h>
#include <kernel/vmm/vmm.h>
#include <kernel/vga.h>
#include <kernel/util.h>
#include <common/print.h>

#define MAGIC 0x36D76289


void kernel_32main(uint32_t magic, uint32_t multiboot_info) {
  print(AS_KERNEL, "STARTING KERNEL", l_green);
  if (magic != MAGIC) {
    print(AS_KERNEL, "BAD MAGIC", l_red);
    print(AS_NONE, "magic=", white);
    vga_write_hex(magic);
    print(AS_KERNEL, "HALTING", l_red);
    halt();
  }
  print(AS_KERNEL, "PARSING MBI", l_green);
  parse_mbi(multiboot_info);
  print(AS_KERNEL, "INITIALIZING PMM", l_green);
  pmm_init();

  print(AS_KERNEL, "INITIALIZING VMM", l_green);
  vmm_init();

  halt();
}
