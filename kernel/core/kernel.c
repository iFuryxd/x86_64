#include <common/types.h>
#include <kernel/vga.h>
void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  print("KERNEL_EVENT: STARTING KERNEL");
  if (magic != 0x36D76289) {
    print("KERNEL_EVENT: BAD MAGIC");
    print("\n");
    print("magic=");
    print_hex(magic);
    print("\n");
    print("KERNEL_EVENT: PANIC");
    while (1) {
    }
  }
  print("\n");
  print("KERNEL_EVENT: MAGIC CHECK PASSED");
  print("\n");
  print("magic=");
  print_hex(magic);
  print("\n");
  print("multiboot=");
  print_hex(multiboot_info);
  print("\n");
  for (;;) {
    __asm__ volatile("cli; hlt");
  }
}
