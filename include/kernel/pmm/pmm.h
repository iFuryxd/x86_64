#ifndef PMM_H
#define PMM_H

#include <common/types.h>
#include <kernel/pmm/multiboot_parser.h>
#define PAGE_SIZE 4096
#define PMM_BITMAP_MAX_BYTES 8192

void pmm_init(void);
uint32_t pmm_alloc_frame(void);
void pmm_free_frame(uint32_t phys_addr);
void dump_pmm_info(void);

#endif
