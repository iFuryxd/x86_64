#include <kernel/pmm/pmm.h>
#include <kernel/vga.h>
#include <common/memutil.h>

static uint64_t max_addr = 0;
static uint64_t frame_count = 0;
static uint64_t bitmap_size_bytes = 0;

static uint8_t bitmap_storage[PMM_BITMAP_MAX_BYTES];
static uint8_t *bitmap = bitmap_storage;


static uint64_t find_max_addr(void) {
uint64_t max = 0;
    for (uint32_t i = 0; i < memory_region_count; i++) {
        uint64_t region_end = memory_regions[i].base + memory_regions[i].len;
        if (region_end > max) {
            max = region_end;
        }
    }
    return max;
}

static void pmm_set_frame(uint64_t frame) {
    bitmap[frame / 8] |= (1 << (frame % 8));
}

static void pmm_clear_frame(uint64_t frame) {
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static bool pmm_test_frame(uint64_t frame) {
    return (bitmap[frame / 8] & (1 << (frame % 8))) != 0;
}

static void pmm_mark_region_free(uint64_t base, uint64_t len) {
    uint64_t start = (base + PAGE_SIZE - 1) / PAGE_SIZE;
    uint64_t end = (base + len) / PAGE_SIZE;

    for (uint64_t frame = start; frame < end; frame++) {
        pmm_clear_frame(frame);
    }
} 

void pmm_init(void) {
    max_addr = find_max_addr();
    frame_count = (max_addr + PAGE_SIZE - 1) / PAGE_SIZE;
    bitmap_size_bytes = (frame_count + 7) / 8;
    if (bitmap_size_bytes > PMM_BITMAP_MAX_BYTES) {
        vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        vga_write("KERNEL_EVENT: KERNEL_PANIC\nbitmap_size_bytes bigger than PMM_BITMAP_MAX_BYTES(4096).\n HALTING");
        while(1){
        __asm__ volatile("hlt");
        }
    }
    memset(bitmap, 0xFF, bitmap_size_bytes);
    for (uint32_t i = 0; i < memory_region_count; i++) {
        pmm_mark_region_free(memory_regions[i].base, memory_regions[i].len);
    }
}

void dump_info(void) {
#ifdef KERNEL_DEBUG
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
    vga_write("\nDEBUG_PRINT: VALUES FOR PMM");
    vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    vga_write("\nmax_addr=");
    vga_write_dec((uint32_t)max_addr);
    vga_write("\nframe_count=");
    vga_write_dec((uint32_t)frame_count);
    vga_write("\nbitmap_size_bytes=");
    vga_write_dec((uint32_t)bitmap_size_bytes);
    vga_write("\n");
#endif
}
