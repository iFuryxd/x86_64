#include <common/types.h>
#include <kernel/vga.h>
void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  print("KERNEL_EVENT: BOOTING COMPLETE");
  print("\n");

  if (magic != 0x36D76289) {
    print("KERNEL_EVENT: BAD MAGIC");
    print("\n");
    print("magic=");
    print_hex(magic);
    print("\n");
    print("KERNEL_EVENT: PANIC");
    while (1) {
    };
  }
  print("KERNEL_EVENT: MAGIC CHECK PASSED");
  print("\n");
  print("magic=");
  print_hex(magic);
  print("\n");
  print("ultiboot_info=");
  print_hex(multiboot_info);

  while (1) {
    __asm__ volatile("hlt");
  }
}
