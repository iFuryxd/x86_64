#include <common/memutil.h>
#include <kernel/mm/parse_mbi.h>
#include <kernel/handle/error/validate_mbi.h>
#include <kernel/util.h>
#include <common/print.h>

uintptr_t multiboot_info_base = 0;
size_t multiboot_info_size = 0;

memory_region_t memory_regions[MAX_MEMORY_REGIONS];
uint64_t memory_region_count = 0;

static uint32_t align_up_u32(uint32_t value, uint32_t align) {
  return (value + align - 1) & ~(align - 1);
}

static uint32_t read_u32(const uint8_t *ptr) {
  uint32_t value = 0;
  memcpy(&value, ptr, sizeof(value));
  return value;
}

void parse_mbi(uintptr_t multiboot_info) {
  print(AS_MBI, "RUNNING VALIDATOR FOR MBI", l_green);
  err response = mbi_errcheck(multiboot_info);
  if (response.code != MBI_VALID) {
    panic(&response);
  }
  print("\nMBI STATUS: " , response.details, l_green);
  multiboot_info_base = (uintptr_t)multiboot_info;
  memory_region_count = 0;
  multiboot_info_size = 0;

    multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info;
    uint8_t *cursor = NULL;
    uint8_t *mbi_end = NULL;

    multiboot_info_size = (size_t)mbi->size;
    cursor = (uint8_t *)(mbi + 1);
    mbi_end = (uint8_t *)mbi + multiboot_info_size;

    while (cursor + sizeof(multiboot_tag_t) <= mbi_end) {
      multiboot_tag_t *tag = (multiboot_tag_t *)cursor;

      if (tag->type == MULTIBOOT_TAG_TYPE_END) {
        break;
      }

      if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
        multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
        uint8_t *entry_ptr = NULL;
        uint8_t *mmap_end = (uint8_t *)mmap + mmap->size;

        entry_ptr = (uint8_t *)mmap + sizeof(multiboot_tag_mmap_t);

        while (entry_ptr + mmap->entry_size <= mmap_end && memory_region_count < MAX_MEMORY_REGIONS) {
          uint32_t addr_low = read_u32(entry_ptr + 0);
          uint32_t addr_high = read_u32(entry_ptr + 4);
          uint32_t len_low = read_u32(entry_ptr + 8);
          uint32_t len_high = read_u32(entry_ptr + 12);
          uint32_t type = read_u32(entry_ptr + 16);

          if (len_low != 0 || len_high != 0) {
            memory_regions[memory_region_count].base = ((paddr_t)addr_high << 32) | addr_low;
            memory_regions[memory_region_count].len = ((uint64_t)len_high << 32) | len_low;
            memory_regions[memory_region_count].type = type;
            memory_region_count++;
          }

          entry_ptr += mmap->entry_size;
        }
      }

      cursor += align_up_u32(tag->size, MULTIBOOT_TAG_ALIGN);
    }
  }


