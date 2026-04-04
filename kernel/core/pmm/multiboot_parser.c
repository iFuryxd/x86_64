#include <kernel/pmm/multiboot_parser.h>
#include <kernel/vga.h>

memory_region_t memory_regions[MAX_MEMORY_REGIONS];
uint32_t memory_region_count = 0;
void parse_mmap(uint32_t multiboot_info) {
  multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;
  multiboot_tag_t *tag = (multiboot_tag_t *)(mbi + 1);

  while (tag->type != MULTIBOOT_TAG_TYPE_END){
    if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
    multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
    multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)((uint8_t *)mmap + sizeof(multiboot_tag_mmap_t));

    while ((uint8_t *)entry < (uint8_t *)mmap + mmap->size) {
      if (entry->type == MULTIBOOT_MEMORY_AVAILABLE){
        if (memory_region_count < MAX_MEMORY_REGIONS){
          uint64_t region_start = entry->addr;
          uint64_t region_end = entry->addr + entry->len;

          if (region_end > PMM_MIN_ADDR) {
            if (region_start < PMM_MIN_ADDR){
              region_start = PMM_MIN_ADDR;
            }
          memory_regions[memory_region_count].base = region_start;
          memory_regions[memory_region_count].len = region_end - region_start;
          memory_region_count++;
          }
        }
      }
      entry = (multiboot_mmap_entry_t *)((uint8_t *)entry + mmap->entry_size);
    }
  }
  tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7));
}
}

void dump_memory_regions(void) {
#ifdef KERNEL_DEBUG
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
  vga_write("DEBUG_PRINT: VALUES FOR MEMORY_REGIONS");
  for (uint32_t i = 0; i < memory_region_count; i++) {
    vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    vga_write("\nregion index=");
    vga_write_dec((uint32_t)i);
    vga_write("\nregion base=");
    vga_write_hex64(memory_regions[i].base);
    vga_write("\nregion length=");
    vga_write_hex64(memory_regions[i].len);
  }
#endif
}
