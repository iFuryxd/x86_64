#include <kernel/mm/pmm.h>
#include <kernel/handle/error/stderr.h>
#include <kernel/handle/error/pmmerr.h>
#include <common/print.h>
#include <kernel/mm/parse_mbi.h>
#include <common/memutil.h>
#include <kernel/util.h>
#include <common/serial_debug.h>

#define FSIZE 4096
#define BITMAP_SIZE_BYTE 262144
#define MiB 0x0000000000100000

#define AVAILABLE 0
#define RESERVED 1

#define MASK(bindex) ((uint8_t)(1u << (bindex)))
#define BYTE_INDEX(frame) ((frame) / 8)
#define BIT_INDEX(frame) ((frame) % 8)

#define __aldown(addr) ((addr) & ~(FSIZE - 1))
#define __alup(addr) (((addr) + FSIZE - 1) & ~(FSIZE - 1))

extern uint8_t __kernel_start;
extern uint8_t __kernel_end;

typedef struct {
  paddr_t phys;
  err status;
} perr;

static pmm_info info = {
  .total_frames = 0,
  .available_frames = 0,
  .used_frames = 0,
  .total_mib = 0,
  .available_mib = 0,
  .used_mib = 0,
  .reserved_frames = 0,
  .reserved_mib = 0,
  };

uint64_t frame_count = 0;
uint64_t used_framec = 0;
uint64_t reserved_frames = 0;

uint64_t total_mib = 0;
uint64_t used_mib = 0;
uint64_t reserved_mib = 0;

static uint8_t bitmap[BITMAP_SIZE_BYTE] __aligned4k;

static uint64_t last_findex = 0;

void update_info();

void initbmap(void) {
    memset(bitmap, 0xFF, sizeof(bitmap));
}

paddr_t get_max_addr() {
  paddr_t max = 0;
  for (uint64_t index = 0; index < memory_region_count; index++) {
    paddr_t addr = 0; 
    if (memory_regions[index].type == 1) {
    addr = __alup(memory_regions[index].base + memory_regions[index].len);
    }
    if (addr > max) max = addr; 
  }
  return max;
}

kbool_t testFrame(paddr_t frame) {
  return (bitmap[BYTE_INDEX(frame)] & MASK(BIT_INDEX(frame))) != 0;
}

void setFrame(paddr_t frame) {
  if (!testFrame(frame)) {
    bitmap[BYTE_INDEX(frame)] |= MASK(BIT_INDEX(frame));
    used_framec++;
  }
  update_info();
}

void clearFrame(paddr_t frame) {
  if (testFrame(frame)) {
    bitmap[BYTE_INDEX(frame)] &= (uint8_t)~MASK(BIT_INDEX(frame));
    used_framec--;
  }
  update_info();
}

perr retPhys(){
  paddr_t index;
  for (index = last_findex; index < frame_count; index++) {
    if (!testFrame(index)) {
      last_findex = index;
      return (perr){ .phys = index * FSIZE, .status.subsystem = "PMM", .status.code = PMM_OK};
    }
  }
  return (perr){ .phys = 0, .status.subsystem = "PMM", .status.code = PMM_ERR_NO_FREE_FRAME};
}

paddr_t pmm_alloc_frame() {
  perr response = retPhys();
  if (response.status.code != PMM_OK) {
    handle(response.status);
    return 0;
  }
  setFrame(response.phys / FSIZE);
  return response.phys;
}

void pmm_free_frame(paddr_t phys) {
    if (!can_be_freed(phys)) return;
    clearFrame(phys / FSIZE);
}

void reserve_region(paddr_t start, paddr_t end) {
  paddr_t __start = __aldown(start);
  paddr_t __end = __alup(end);
  
  for (paddr_t findex = __start / FSIZE; findex < __end / FSIZE; findex++) {
    if (!testFrame(findex)) {
      setFrame(findex);
      reserved_frames++;
    }
  }
  update_info();
}

void mark_memreg_free(void) {
  for (uint64_t reg_index = 0; reg_index < memory_region_count; reg_index++) {
    if (memory_regions[reg_index].type == 1) {
      paddr_t start = __aldown(memory_regions[reg_index].base) / FSIZE;
      paddr_t end = __alup(memory_regions[reg_index].base + memory_regions[reg_index].len)  / FSIZE;
        for (paddr_t findex = start; findex < end; findex++){
          clearFrame(findex);
        }
        serial_debug("\nstart = ", start);
        serial_debug("\nend = ", end);
    }
  }
  update_info();
}

void set_fcount() {
  frame_count = get_max_addr() / FSIZE;
  used_framec = frame_count;
}

void update_info() {
  info.total_frames = frame_count;
  info.used_frames = used_framec;
  info.available_frames = frame_count - used_framec;
  info.total_mib = (frame_count * FSIZE) / MiB;
  info.available_mib = (info.available_frames * FSIZE) / MiB;
  info.used_mib = (used_framec * FSIZE) / MiB;
  info.reserved_frames = reserved_frames;
  info.reserved_mib = (reserved_frames * FSIZE) / MiB;
}

void pmm_init() {
  print(AS_PMM, "SETING FRAME COUNT", l_green);
  set_fcount();
  print(AS_PMM, "INITIALIZING BITMAP", l_green);
  initbmap();
  print(AS_PMM, "MARKING AVAILABLE MEMORY REGIONS AS FREE", l_green);
  mark_memreg_free();
  printdec("used after free: ", used_framec);
  print(AS_PMM, "RESERVING RANGE 0 - 4096", l_green);
  reserve_region(0, FSIZE);
  print(AS_PMM, "RESERVING KERNEL MEMORY RANGE", l_green);
  printhex("__kernel_start = ", (paddr_t)&__kernel_start);
  printhex("__kernel_end = ", (paddr_t)&__kernel_end);
  reserve_region((paddr_t)&__kernel_start, (paddr_t)&__kernel_end);
  printdec("frame_count: ", frame_count);
  printdec("used_framec: ", used_framec);
  printdec("reserved_frames: ", reserved_frames);
  print(AS_PMM, "RESERVING MULTIBOOT INFORMATION MEMORY RANGE", l_green);
  printhex("__multiboot_info_start = ", __aldown(multiboot_info_base));
  printhex("__multiboot_info_end = ", __aldown(multiboot_info_base) + __alup(multiboot_info_size));
  reserve_region(__aldown(multiboot_info_base), __aldown(multiboot_info_base) + __alup(multiboot_info_size));
  update_info();
  printdec("frame_count: ", frame_count);
  printdec("used_framec: ", used_framec);
  printdec("reserved_frames: ", reserved_frames);
}

pmm_info *pmm_fetch_info() {
  print(AS_PMM, "UPDATING INFORMATION", l_green);
  update_info();
    printdec("PMM->total frames: ", info.total_frames);
    printdec("PMM->used frames: ", info.used_frames);
    printdec("PMM->available frames: ", info.available_frames);
    printdec("PMM->reserved frames: ", info.reserved_frames);
    printdec("PMM->total mib: ", info.total_mib);
    printdec("PMM->used mib: ", info.used_mib);
    printdec("PMM->available mib: ", info.available_mib);
  return &info;
}
