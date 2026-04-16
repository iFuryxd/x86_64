#include <kernel/mm/paging64.h>
#include <kernel/util.h>
#include <common/print.h>

#define ENTRY_COUNT 512
#define HUGE (1ULL << 7)

#define WRITABLE (1ULL << 1)
#define PRESENT (1ULL << 0)

#define __ALIGN__ __attribute__((aligned(4096)))
#define MASK_4KiB 0x000FFFFFFFFFF000ULL
#define MASK_2MiB 0x000FFFFFFFE00000ULL

#define MiB 0x0000000000100000

#define __CHECK_EQUALITY(phys, virt) if (phys != virt) { \
        warn("IDENTITY MAP REQUIRES PHYSICAL ADDRESS == VIRTUAL ADDRESS"); \
        return;\
        }
#define __CHECK_VIRT(addr) if ((addr & (2 * MiB - 1)) != 0) { \
        warn("VIRTUAL ADDRESS IS NOT 2 MiB ALIGNED"); \
        return;\
        }
#define __CHECK_PHYS(addr) if ((addr & (2 * MiB - 1)) != 0) { \
        warn("PHYSICALL ADDRESS IS NOT 2 MiB ALIGNED"); \
        return;\
        }
#define __CHECK_INDEX(L4, L3) if (L4 !=0 || L3 != 0) { \
        warn("OUT OF BOOTSTRAP RANGE"); \
        return;\
        }

static uint64_t pml4[ENTRY_COUNT] __ALIGN__;
static uint64_t pdp[ENTRY_COUNT] __ALIGN__;
static uint64_t pd[ENTRY_COUNT] __ALIGN__;

void idmap_2MiB(uint64_t phys, uint64_t virt, uint64_t flags) {
    __CHECK_EQUALITY(phys, virt)
    __CHECK_PHYS(phys)
    __CHECK_VIRT(virt)

    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdp_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    
    __CHECK_INDEX(pml4_index, pdp_index)

    pml4[pml4_index] = ((uint64_t)pdp & MASK_4KiB) | PRESENT | WRITABLE;
    pdp[pdp_index] = ((uint64_t)pd & MASK_4KiB) | PRESENT | WRITABLE;
    pd[pd_index] = (phys & MASK_2MiB) | flags | PRESENT | HUGE;
}