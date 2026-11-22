#include "sdk_kernel_startup_entries.h"

extern s32 kCopy_0(u32 *, const u32 *, u32);
extern s32 kCopy_1(u8 *, const u8 *, u32);

kernelStartupEntryPair_t execEntryPairs[3] = {
    {0x5A, {.wordCopy = kCopy_0}},
    {0x5B, {.resident = (void *)0x80074000}},
    {0xFFFFC402, {.resident = NULL}},
};

kernelStartupEntryPair_t setArgEntryPairs[1] __attribute__((section(".data"))) = {
    {0x5A, {.byteCopy = kCopy_1}},
};
