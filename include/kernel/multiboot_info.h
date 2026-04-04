#ifndef MULTIBOOT_INFO_H
#define MULTIBOOT_INFO_H

#include <common/types.h>

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

void parse_mmap(uint32_t multiboot_info, uint8_t WRITE_TO_VGA);

#endif
