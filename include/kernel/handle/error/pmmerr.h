#ifndef PMMERR_H
#define PMMERR_H

#include <common/types.h>
#include <kernel/handle/error/stderr.h>

typedef enum {
PMM_OK = 0,
PMM_ERR_NO_FREE_FRAME,
PMM_ERR_FREE_PHYS_EQUAL_TO_0,
PMM_ERR_FREE_UNAUTHORIZED_PHYS,
PMM_ERR_FREE_NOT_PAGE_ALIGNED,
PMM_ERR_FREE_OUTSIDE_RAM_BOUNDS,
} pmmerr;

void handle(err status);
kbool_t can_be_freed(uint64_t phys);

#endif