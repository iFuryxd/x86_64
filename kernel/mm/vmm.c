#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>
#include <common/memutil.h>
#include <kernel/util.h>
#include <common/print.h>

static uint32_t page_dir[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32_t first_page_table[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));


static void vmm_enable(uint32_t page_dir_phys) {
__asm__ volatile("mov %0, %%cr3" : : "r"(page_dir_phys));
__asm__ volatile(
    "mov %%cr0, %%eax\n"
    "or $0x80000000, %%eax\n"
    "mov %%eax, %%cr0\n"
    :
    :
    : "eax");
}

void vmm_map_page(uint32_t virt, uint32_t phys, uint32_t flags) {
uint32_t dir_index = virt >> 22;
uint32_t table_index = (virt >> 12) & 0x3FF;
uint32_t *page_table = NULL;
if ((page_dir[dir_index] & PAGE_PRESENT) == 0) {
    uint32_t new_table_phys = pmm_alloc_frame();
    if (new_table_phys == 0) {
        print(AS_ERR, "NO FREE FRAME FOR PAGE TABLE", l_red);
        halt();
    }
    if (new_table_phys >= 0x400000) {
        print(AS_ERR, "NEW PAGE TABLE NOT IDENTIFY_MAPPED", l_red);
        halt();
    }
        page_table = (uint32_t *)new_table_phys;
        memset(page_table, 0, PAGE_SIZE);
        page_dir[dir_index] = (new_table_phys & PAGE_FRAME_MASK) | PAGE_PRESENT | PAGE_WRITABLE;
    } else {
        page_table = (uint32_t *)(page_dir[dir_index] & PAGE_FRAME_MASK);
    }
    page_table[table_index] = (phys & PAGE_FRAME_MASK) | (flags & 0xFFF) | PAGE_PRESENT;

    __asm__ volatile("invlpg (%0)" : : "r"(virt) : "memory");
}



void vmm_init(void) {
    memset(page_dir, 0, sizeof(page_dir));
    memset(first_page_table, 0, sizeof(first_page_table));

    page_dir[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    for (uint32_t addr = 0; addr < 0x400000; addr += PAGE_SIZE) {
        vmm_map_page(addr, addr, PAGE_WRITABLE);
    }
    vmm_enable((uint32_t)page_dir);
    print(AS_VMM, "PAGING ENABLED", l_green);
}