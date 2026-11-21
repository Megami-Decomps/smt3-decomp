#ifndef SDK_KERNEL_TLB_ENTRY_H
#define SDK_KERNEL_TLB_ENTRY_H

#include "common.h"

typedef struct {
    u32 pageMask;
    u32 entryHi;
    u32 entryLo0;
    u32 entryLo1;
} kernelTlbEntry_t;

#endif
