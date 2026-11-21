#include "sdk_kernel_tlb_entry.h"

SIZE_CHECK(kernelTlbTableSize, kernelTlbTable_t, 0x1C);

/* Boot mapping counts and storage used by InitTLB32MB. */
kernelTlbTable_t tlbTable = {
    ARRAY_COUNT(tlbKernelEntries),
    ARRAY_COUNT(tlbDefaultEntries),
    ARRAY_COUNT(tlbExtendedEntries),
    0,
    tlbKernelEntries,
    tlbDefaultEntries,
    tlbExtendedEntries,
};
