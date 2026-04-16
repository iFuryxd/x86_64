#ifndef PAGING64_H
#define PAGING64_H

#include <common/types.h>


void idmap_2MiB(uint64_t phys, uint64_t virt, uint64_t flags);

#endif