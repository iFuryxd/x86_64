#ifndef VALIDATE_MBI_H
#define VALIDATE_MBI_H

#include <kernel/mm/parse_mbi.h>
#include <kernel/handle/error/stderr.h>
#include <common/types.h>

typedef enum {
  MBI_VALID = 0,
  MBI_ERR_NULL,
  MBI_ERR_HEADER_SIZE,
  MBI_ERR_TAG_BOUNDS,
  MBI_ERR_TAG_SIZE,
  MBI_ERR_NO_TAG_TYPE_MMAP,
  MBI_ERR_TAG_SUM_SIZE,
  MBI_ERR_MMAP_SIZE,
  MBI_ERR_MMAP_END,
  MBI_ERR_MMAP_ENTRY_SIZE,
  MBI_UNDEFINED,
} mbi_validation;

err mbi_errcheck(uintptr_t multiboot_info);

#endif