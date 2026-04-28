#ifndef PMM_H
#define PMM_H

#include <common/types.h>

typedef struct {
    uint64_t total_frames;
    uint64_t available_frames;
    uint64_t used_frames;
    uint64_t total_mib;
    uint64_t available_mib;
    uint64_t used_mib;
    uint64_t reserved_frames;
    uint64_t reserved_mib;
} pmm_info;

extern uint64_t frame_count;

void pmm_init();
paddr_t pmm_alloc_frame();
void pmm_free_frame(paddr_t phys);
pmm_info *pmm_fetch_info();

#endif