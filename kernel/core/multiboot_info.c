#include <kernel/multiboot_info.h>
#include <kernel/vga.h>

void parse_mmap(uint32_t multiboot_info) {
  multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;
  multiboot_tag_t *tag = (multiboot_tag_t *)(mbi + 1);

  while (tag->type != 0 && tag->type != 6){
    tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7));
  }

  if (tag->type == 6) {
    multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
    multiboot_mmap_entry_t *entry =
        (multiboot_mmap_entry_t *)((uint8_t *)mmap + sizeof(multiboot_tag_mmap_t));

    while ((uint8_t *)entry < (uint8_t *)mmap + mmap->size) {
      print("\nregion: ");
      print_hex((uint32_t)entry->addr);
      print("\nsize=");
      print_hex((uint32_t)entry->len);
      print("\ntype=");
      print_hex(entry->type);
      print("\n");
      entry = (multiboot_mmap_entry_t *)((uint8_t *)entry + mmap->entry_size);
    }
  } else {
    print("KERNEL_EVENT: tag->type != 6\n HALTING...");
  }
}
