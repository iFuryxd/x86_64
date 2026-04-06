#include <kernel/pmm/multiboot_parser.h>
#include <kernel/vga.h>

uint32_t multiboot_info_base = 0;
uint32_t multiboot_info_size = 0;

memory_region_t memory_regions[MAX_MEMORY_REGIONS];
uint32_t memory_region_count = 0;

static uint32_t align_up_u32(uint32_t value, uint32_t align) {
  return (value + align - 1) & ~(align - 1);
}

void parse_mmap(uint32_t multiboot_info) {
  multiboot_info_base = multiboot_info;
  memory_region_count = 0;
  multiboot_info_size = 0;

  if (multiboot_info == 0) {
    return;
  }

  {
    multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;
    uint8_t *cursor = NULL;
    uint8_t *mbi_end = NULL;

    if (mbi->size < sizeof(multiboot_info_t)) {
      return;
    }

    multiboot_info_size = mbi->size;
    cursor = (uint8_t *)(mbi + 1);
    mbi_end = (uint8_t *)mbi + mbi->size;

    while (cursor + sizeof(multiboot_tag_t) <= mbi_end) {
      multiboot_tag_t *tag = (multiboot_tag_t *)cursor;
      uint32_t aligned_tag_size = 0;

      if (tag->type == MULTIBOOT_TAG_TYPE_END) {
        break;
      }

      if (tag->size < sizeof(multiboot_tag_t)) {
        return;
      }

      if (cursor + tag->size > mbi_end) {
        return;
      }

      aligned_tag_size = align_up_u32(tag->size, MULTIBOOT_TAG_ALIGN);
      if (aligned_tag_size < tag->size) {
        return;
      }

      if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
        multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
        uint8_t *entry_ptr = NULL;
        uint8_t *mmap_end = cursor + tag->size;

        if (mmap->size < sizeof(multiboot_tag_mmap_t)) {
          return;
        }

        if (mmap->entry_size < sizeof(multiboot_mmap_entry_t)) {
          return;
        }

        entry_ptr = (uint8_t *)mmap + sizeof(multiboot_tag_mmap_t);
        while (entry_ptr + mmap->entry_size <= mmap_end) {
          multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)entry_ptr;

          if (entry->len != 0 && memory_region_count < MAX_MEMORY_REGIONS) {
            memory_regions[memory_region_count].base = entry->addr;
            memory_regions[memory_region_count].len = entry->len;
            memory_regions[memory_region_count].type = entry->type;
            memory_region_count++;
          }

          entry_ptr += mmap->entry_size;
        }

        break;
      }

      cursor += aligned_tag_size;
    }
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
    vga_write("\nregion type=");
    vga_write_dec(memory_regions[i].type);
  }
#endif
}
