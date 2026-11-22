#ifndef SDK_KERNEL_STARTUP_ENTRIES_H
#define SDK_KERNEL_STARTUP_ENTRIES_H

#include "common.h"

typedef struct {
    u32 index;
    union {
        u32 *(*find)(u32 *, const u32 *, u32);
        s32 (*wordCopy)(u32 *, const u32 *, u32);
        s32 (*byteCopy)(u8 *, const u8 *, u32);
        void *resident;
    } entry;
} kernelStartupEntryPair_t;

extern kernelStartupEntryPair_t systemCallEntryPairs[2];
extern kernelStartupEntryPair_t execEntryPairs[3];
extern kernelStartupEntryPair_t setArgEntryPairs[1];

#endif
