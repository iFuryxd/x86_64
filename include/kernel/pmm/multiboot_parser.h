#ifndef MULTIBOOT_PARSER_H
#define MULTIBOOT_PARSER_H

#include <common/types.h>

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_MMAP 6
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_TAG_ALIGN 8

#define PMM_MIN_ADDR 0x100000ULL
#define MAX_MEMORY_REGIONS 32

typedef struct {
  uint32_t size;
  uint32_t reserved;
} multiboot_info_t;

typedef struct {
  uint32_t type;
  uint32_t size;
} multiboot_tag_t;

typedef struct {
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
} multiboot_tag_mmap_t;

typedef struct {
  uint64_t addr;
  uint64_t len;
  uint32_t type;
  uint32_t zero;
} multiboot_mmap_entry_t;

typedef struct {
  uint64_t base;
  uint64_t len;
  uint32_t type;
} memory_region_t;

extern memory_region_t memory_regions[MAX_MEMORY_REGIONS];
extern uint32_t memory_region_count;

extern uint32_t multiboot_info_base;
extern uint32_t multiboot_info_size;

void parse_mmap(uint32_t multiboot_info);
void dump_memory_regions(void);

#endif
