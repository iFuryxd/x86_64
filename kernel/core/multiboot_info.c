#include <kernel/multiboot_info.h>
#include <kernel/vga.h>

void parse_mmap(uint32_t multiboot_info, uint8_t WRITE_TO_VGA) {
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
      if (WRITE_TO_VGA) {
      vga_write("\nregion: ");
      vga_write_hex((uint32_t)entry->addr);
      vga_write("\nsize=");
      vga_write_hex((uint32_t)entry->len);
      vga_write("\ntype=");
      vga_write_hex(entry->type);
      vga_write("\n");
      }
      entry = (multiboot_mmap_entry_t *)((uint8_t *)entry + mmap->entry_size);
    }
  } else {
    vga_write("KERNEL_EVENT: tag->type != 6\n HALTING...");
  }
}
