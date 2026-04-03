#include <common/types.h>
#include <kernel/vga.h>
#include <kernel/multiboot_info.h>
void kernel_main(uint32_t magic, uint32_t multiboot_info) {
  print("KERNEL_EVENT: STARTING KERNEL\n");
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
  print("KERNEL_EVENT: MAGIC CHECK PASSED\n");
  print("magic=");
  print_hex(magic);
  print("\nmultiboot_info=");
  print_hex(multiboot_info);
  print("\nKERNEL_EVENT: PARSING MULTIBOOT_INFO\n");
  parse_mmap(multiboot_info);
  while(1) {
    __asm__ volatile("cli; hlt");
  }
}
