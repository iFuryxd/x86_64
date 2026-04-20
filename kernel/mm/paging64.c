#include <kernel/mm/paging64.h>
#include <kernel/util.h>
#include <common/print.h>
#include <common/memutil.h>
#include <stdint.h>

#define ENTRY_COUNT 512
#define HUGE (1ULL << 7)

#define WRITABLE (1ULL << 1)
#define PRESENT (1ULL << 0)

#define __ALIGN__ __attribute__((aligned(4096)))
#define MASK_4KiB 0x000FFFFFFFFFF000ULL
#define MASK_2MiB 0x000FFFFFFFE00000ULL

#define MiB 0x0000000000100000

#define PAGE_SIZE_4k 4096ULL
#define PAGE_SIZE_2M (2ULL * 1024 * 1024)

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

static void clear_tables(void) {
        memset(pml4, 0, sizeof(pml4));
        memset(pdp, 0, sizeof(pdp));
        memset(pd, 0, sizeof(pd));
}

void idmap_2MiB(uint64_t phys, uint64_t virt, uint64_t flags) {
    __CHECK_EQUALITY(phys, virt)
    __CHECK_PHYS(phys)
    __CHECK_VIRT(virt)

    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdp_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    
    __CHECK_INDEX(pml4_index, pdp_index)

    pd[pd_index] = (phys & MASK_2MiB) | flags | PRESENT | HUGE;
}


uint32_t p64_init(void) {
        print(AS_PAG64, "CLEARING TABLES", l_green);
        clear_tables();
        
        print(AS_PAG64, "LINKING PML4[0]->PDP[0]->PD", l_green);
        pml4[0] = ((uint64_t)pdp & MASK_4KiB) | PRESENT | WRITABLE;
        pdp[0] = ((uint64_t)pd & MASK_4KiB) | PRESENT | WRITABLE;
        print(AS_PAG64, "IDENTITY MAPPING AS WRITABLE"
        "\n{ phys:0x00000000 virt:0x00000000 }\n{ phys:0x00200000 virt:0x00200000 }", l_green);
        idmap_2MiB(0x00000000, 0x00000000, WRITABLE);
        idmap_2MiB(0x00200000, 0x00200000, WRITABLE);
        print(AS_PAG64, "RETURNING TO CALLER", l_green);
        return (uint32_t)pml4;
}