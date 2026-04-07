#include <common/memutil.h>
#include <kernel/pmm/pmm.h>
#include <kernel/vga.h>

static uint64_t max_addr = 0;
static uint64_t frame_count = 0;
static uint64_t bitmap_size_bytes = 0;
static uint64_t kernel_reserved_base = 0;
static uint64_t kernel_reserved_size = 0;
static uint64_t used_frame_count = 0;

static uint8_t bitmap_storage[PMM_BITMAP_MAX_BYTES];
static uint8_t *bitmap = bitmap_storage;

extern uint8_t __kernel_start[];
extern uint8_t __kernel_end[];

static uint64_t align_up_u64(uint64_t value, uint64_t align) {
  return (value + align - 1) & ~(align - 1);
}

static uint32_t read_u32(const void *ptr) {
  uint32_t value = 0;
  memcpy(&value, ptr, sizeof(value));
  return value;
}

static uint64_t read_u64(const void *ptr) {
  uint32_t low = read_u32(ptr);
  uint32_t high = read_u32((const uint8_t *)ptr + 4);
  return ((uint64_t)high << 32) | low;
}

static uint64_t region_end_u64(uint64_t base, uint64_t len) {
  const uint64_t max_u64 = ~0ULL;

  if (len == 0) {
    return base;
  }

  if (base > max_u64 - len) {
    return max_u64;
  }

  return base + len;
}

__attribute__((noinline)) static uint64_t find_max_addr(void) {
  uint64_t max = 0;
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
  vga_write("\nmemory_region_count=");
  vga_write_dec(memory_region_count);
  for (uint32_t i = 0; i < memory_region_count; i++) {
    vga_write("\nITERATION ");
    vga_write_dec(i);
    uint64_t end = 0;
    const memory_region_t *region = &memory_regions[i];

    if (region->type != MULTIBOOT_MEMORY_AVAILABLE) {
      continue;
    }

    if (region->len == 0) {
      continue;
    }

    end = region_end_u64(region->base, region->len);
    if (end > max) {
      max = end;
    }
    vga_write("\nend=");
    vga_write_hex64(end);
  }
  return max;
}

static void pmm_set_frame(uint64_t frame) {
  uint8_t mask = 0;

  if (frame >= frame_count) {
    return;
  }

  mask = (uint8_t)(1u << (frame % 8));
  if ((bitmap[frame / 8] & mask) == 0) {
    bitmap[frame / 8] |= mask;
    used_frame_count++;
  }
}

static void pmm_clear_frame(uint64_t frame) {
  uint8_t mask = 0;

  if (frame >= frame_count) {
    return;
  }

  mask = (uint8_t)(1u << (frame % 8));
  if ((bitmap[frame / 8] & mask) != 0) {
    bitmap[frame / 8] &= (uint8_t)~mask;
    used_frame_count--;
  }
}

static kbool_t pmm_test_frame(uint64_t frame) {
  if (frame >= frame_count) {
    return true;
  }
  return (bitmap[frame / 8] & (1 << (frame % 8))) != 0;
}

static void pmm_mark_region_free(uint64_t base, uint64_t len) {
  uint64_t region_base = base;
  uint64_t region_end = 0;
  uint64_t start = 0;
  uint64_t end = 0;

  if (len == 0) {
    return;
  }

  region_end = region_end_u64(base, len);
  if (region_end <= PMM_MIN_ADDR) {
    return;
  }

  if (region_base < PMM_MIN_ADDR) {
    region_base = PMM_MIN_ADDR;
  }

  start = align_up_u64(region_base, PAGE_SIZE) / PAGE_SIZE;
  end = region_end / PAGE_SIZE;
  if (start >= frame_count) {
    return;
  }
  if (end > frame_count) {
    end = frame_count;
  }
  for (uint64_t frame = start; frame < end; frame++) {
    pmm_clear_frame(frame);
  }
}

static void pmm_reserve_region(uint64_t base, uint64_t len) {
  uint64_t region_end = 0;
  uint64_t start = base / PAGE_SIZE;
  uint64_t end = 0;

  if (len == 0) {
    return;
  }

  region_end = region_end_u64(base, len);
  end = align_up_u64(region_end, PAGE_SIZE) / PAGE_SIZE;
  if (start >= frame_count) {
    return;
  }
  if (end > frame_count) {
    end = frame_count;
  }
  for (uint64_t frame = start; frame < end; frame++) {
    pmm_set_frame(frame);
  }
}

void pmm_init(void) {
  kernel_reserved_base = (uint32_t)__kernel_start;
  kernel_reserved_size = (uint32_t)(__kernel_end - __kernel_start);
  vga_write("PMM0\n");

  max_addr = find_max_addr();
  frame_count = (max_addr + PAGE_SIZE - 1) / PAGE_SIZE;
  bitmap_size_bytes = (frame_count + 7) / 8;
  used_frame_count = frame_count;
  vga_write("PMM1\n");
  if (bitmap_size_bytes > PMM_BITMAP_MAX_BYTES) {
    vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    vga_write("KERNEL_EVENT: KERNEL_PANIC\nbitmap_size_bytes bigger than "
              "PMM_BITMAP_MAX_BYTES(4096).\n HALTING");
    while (1) {
      __asm__ volatile("hlt");
    }
  }
  memset(bitmap, 0xFF, bitmap_size_bytes);
  vga_write("PMM2\n");
  for (uint32_t i = 0; i < memory_region_count; i++) {
    if (memory_regions[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
      pmm_mark_region_free(memory_regions[i].base, memory_regions[i].len);
    }
  }
  vga_write("PMM3\n");

  pmm_reserve_region(kernel_reserved_base, kernel_reserved_size);
  vga_write("PMM4\n");
  pmm_reserve_region(multiboot_info_base, multiboot_info_size);
  vga_write("PMM5\n");
}

uint32_t pmm_alloc_frame(void) {
  uint64_t start_frame = PMM_MIN_ADDR / PAGE_SIZE;
  for (uint64_t frame = start_frame; frame < frame_count; frame++) {
    if (!pmm_test_frame(frame)) {
      pmm_set_frame(frame);
      return (uint32_t)(frame * PAGE_SIZE);
    }
  }
  return 0;
}

void pmm_free_frame(uint32_t phys_addr) {
  if (phys_addr < PMM_MIN_ADDR) {
    return;
  }

  if (phys_addr % PAGE_SIZE != 0) {
    return;
  }

  uint64_t frame = phys_addr / PAGE_SIZE;
  if (frame >= frame_count) {
    return;
  }

  if ((uint64_t)phys_addr >= kernel_reserved_base &&
      (uint64_t)phys_addr < kernel_reserved_base + kernel_reserved_size) {
    return;
  }

  if ((uint64_t)phys_addr >= multiboot_info_base &&
      (uint64_t)phys_addr <
          (uint64_t)multiboot_info_base + multiboot_info_size) {
    return;
  }

  pmm_clear_frame(frame);
}

void dump_pmm_info(void) {
#ifdef KERNEL_DEBUG
  vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
  vga_write("\nDEBUG_PRINT: VALUES FOR PMM");
  vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
  vga_write("\nmax_addr=");
  vga_write_hex64(max_addr);
  vga_write("\nframe_count=");
  vga_write_dec64(frame_count);
  vga_write("\nused_frame_count=");
  vga_write_dec64(used_frame_count);
  vga_write("\nfree_frame_count=");
  vga_write_dec64(frame_count - used_frame_count);
  vga_write("\nbitmap_size_bytes=");
  vga_write_dec64(bitmap_size_bytes);
  vga_write("\nkernel_base=");
  vga_write_hex64(kernel_reserved_base);
  vga_write("\nkernel_size=");
  vga_write_dec64(kernel_reserved_size);
  vga_write("\nmultiboot_info_base=");
  vga_write_hex(multiboot_info_base);
  vga_write("\nmultiboot_info_size=");
  vga_write_dec(multiboot_info_size);
  vga_write("\n");
#endif
}
