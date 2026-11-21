#ifndef SDK_KERNEL_TLB_ENTRY_H
#define SDK_KERNEL_TLB_ENTRY_H

#include "common.h"

typedef struct {
    u32 pageMask;
    u32 entryHi;
    u32 entryLo0;
    u32 entryLo1;
} kernelTlbEntry_t;

/* InitTLB32MB loads the kernel and default entries, stores how many it wrote in
 * loadedCount, then loads the extended ones. */
typedef struct {
    s32 kernelCount;
    s32 defaultCount;
    s32 extendedCount;
    s32 loadedCount;
    kernelTlbEntry_t *kernelEntries;
    kernelTlbEntry_t *defaultEntries;
    kernelTlbEntry_t *extendedEntries;
} kernelTlbTable_t;

extern kernelTlbEntry_t tlbKernelEntries[13];
extern kernelTlbEntry_t tlbDefaultEntries[18];
extern kernelTlbEntry_t tlbExtendedEntries[8];
extern kernelTlbTable_t tlbTable;

#endif
