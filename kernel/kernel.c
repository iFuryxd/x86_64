#include <common/types.h>
#include <kernel/mm/parse_mbi.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/paging32.h>
#include <kernel/util.h>
#include <common/print.h>
#include <kernel/arch/x86_64/cpuid.h>

#define MAGIC 0x36D76289


void kernel_32main(uint32_t magic, uint32_t multiboot_info) {
  print("KERNEL: ", "STARTING KERNEL", l_green);
  if (magic != MAGIC) {
    print(AS_KERNEL, "BAD MAGIC", l_red);
    print(AS_NONE, "magic=", white);
    vga_write_hex(magic);
    halt();
  }
  print(AS_KERNEL, "PARSING MBI", l_green);
  parse_mbi(multiboot_info);
  print(AS_KERNEL, "INITIALIZING PMM", l_green);
  pmm_init();

  print(AS_KERNEL, "INITIALIZING PAGER", l_green);
  pager_init();
  print(AS_KERNEL, "RUNNING CPUID PROBE", l_green);
  cpuid_info info = cpuid_probe();
  if (!info.has_long_mode || !info.has_msr || !info.has_pae) {
    error("CPUID INFORMATION HAS NO MSR/PAE/LONG_MODE");
    halt();
  }
  halt();
}
