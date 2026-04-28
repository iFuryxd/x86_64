#include <kernel/handle/error/pmmerr.h>
#include <common/print.h>
#include <kernel/mm/parse_mbi.h>
#include <kernel/mm/pmm.h>

#define U8 (uint8_t *)
#define CU8 (const char*)

#define FSIZE 4096
#define BITMAP_SIZE_BYTE 262144

#define AVAILABLE 0
#define RESERVED 1

#define MASK(bindex) (1u << (bindex))
#define BYTE_INDEX(frame) ((frame) / 8)
#define BIT_INDEX(frame) ((frame) % 8)

#define __aldown(addr) ((addr) & ~(FSIZE - 1))
#define __alup(addr) (((addr) + FSIZE - 1) & ~(FSIZE - 1))

typedef struct {
  paddr_t phys;
  err status;
} perr;

extern uint8_t __kernel_start;
extern uint8_t __kernel_end;

uint8_t *details(uint64_t errCode) {
    switch (errCode) {
    case PMM_ERR_NO_FREE_FRAME: return U8 "no frame available";
    case PMM_ERR_FREE_PHYS_EQUAL_TO_0: return U8 "attempt to free physical address equal to 0 not allowed";
    default: return U8 "can not provide details: reason -> unknown error";
    }
}

perr is_reserved(paddr_t phys) {
  uint64_t __mbi_start = __aldown(multiboot_info_base), __mbi_end = __alup(multiboot_info_base + multiboot_info_size);
  uint64_t __kstart = (uint64_t)&__kernel_start, __kend = (uint64_t)&__kernel_end;
  if ((phys >= __mbi_start && phys <= __mbi_end) || (phys >= __kstart && phys <= __kend)) {
    return (perr) {.phys = phys, .status.subsystem = "PMM", .status.code = PMM_ERR_FREE_UNAUTHORIZED_PHYS};
  }
  else {
  return (perr) {.phys = phys, .status.code = PMM_OK};
  }
}

perr is_page_aligned(paddr_t phys) {
  if (phys % FSIZE != 0) {
    return (perr) {.phys = phys, .status.subsystem = "PMM", .status.code = PMM_ERR_FREE_NOT_PAGE_ALIGNED};
  } 
  return (perr) {.phys = phys, .status.subsystem = "PMM", .status.code = PMM_OK};
}

perr is_outside_ram(paddr_t phys) {
  if (phys > frame_count * FSIZE) {
    return (perr) {.phys = phys, .status.subsystem = "PMM", .status.code = PMM_ERR_FREE_OUTSIDE_RAM_BOUNDS};
  }
  return (perr) {.phys = phys, .status.subsystem = "PMM", .status.code = PMM_OK};
}

kbool_t can_be_freed(paddr_t phys) {
  if (is_reserved(phys).status.code != PMM_OK) {
    handle(is_reserved(phys).status);
    return false;
  }
  if (is_page_aligned(phys).status.code != PMM_OK) {
    handle(is_page_aligned(phys).status);
    return false;
  }
  if (is_outside_ram(phys).status.code != PMM_OK) {
    handle(is_outside_ram(phys).status);
    return false;
  }
  return true;
}

void handle(err status) {
    status.details = CU8 details(status.code);
    print(status.subsystem, status.details, l_red);
    return;    
}

