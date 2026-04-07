#include <common/memutil.h>
#include <kernel/pmm/multiboot_parser.h>
#include <kernel/vga.h>

uint32_t multiboot_info_base = 0;
uint32_t multiboot_info_size = 0;

memory_region_t memory_regions[MAX_MEMORY_REGIONS];
uint32_t memory_region_count = 0;

static uint32_t align_up_u32(uint32_t value, uint32_t align) {
  return (value + align - 1) & ~(align - 1);
}

static uint32_t read_u32(const uint8_t *ptr) {
  uint32_t value = 0;
  memcpy(&value, ptr, sizeof(value));
  return value;
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

    multiboot_info_size = mbi->size;
    if (multiboot_info_size < sizeof(multiboot_info_t)) {
      return;
    }

    cursor = (uint8_t *)(mbi + 1);
    mbi_end = (uint8_t *)mbi + multiboot_info_size;

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

      if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
        multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
        uint8_t *entry_ptr = NULL;
        uint8_t *mmap_end = (uint8_t *)mmap + mmap->size;

        if (mmap->size < sizeof(multiboot_tag_mmap_t)) {
          return;
        }

        if (mmap_end > cursor + tag->size) {
          return;
        }

        if (mmap->entry_size < sizeof(multiboot_mmap_entry_t)) {
          return;
        }

        entry_ptr = (uint8_t *)mmap + sizeof(multiboot_tag_mmap_t);

        while (entry_ptr + mmap->entry_size <= mmap_end &&
               memory_region_count < MAX_MEMORY_REGIONS) {
          uint32_t addr_low = read_u32(entry_ptr + 0);
          uint32_t addr_high = read_u32(entry_ptr + 4);
          uint32_t len_low = read_u32(entry_ptr + 8);
          uint32_t len_high = read_u32(entry_ptr + 12);
          uint32_t type = read_u32(entry_ptr + 16);

          if (len_low != 0 || len_high != 0) {
            memory_regions[memory_region_count].base =
                ((uint64_t)addr_high << 32) | addr_low;
            memory_regions[memory_region_count].len =
                ((uint64_t)len_high << 32) | len_low;
            memory_regions[memory_region_count].type = type;
            memory_region_count++;
          }

          entry_ptr += mmap->entry_size;
        }

        return;
      }

      aligned_tag_size = align_up_u32(tag->size, MULTIBOOT_TAG_ALIGN);
      if (aligned_tag_size < tag->size) {
        return;
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
