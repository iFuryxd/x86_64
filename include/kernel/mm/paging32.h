#ifndef VMM_H
#define VMM_H

#include <common/types.h>

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024
#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2

#define PAGE_FRAME_MASK 0xFFFFF000
void pager_init(void);
void map_page(uint32_t virt, uint32_t phys, uint32_t flags);
void unmap_page(uint32_t virt);

#endif