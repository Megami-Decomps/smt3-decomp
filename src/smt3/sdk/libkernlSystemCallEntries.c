#include "sdk_kernel_startup_entries.h"

extern u32 *kFindAddress(u32 *, const u32 *, u32);
extern s32 kCopy(u32 *, const u32 *, u32);

/* Published during the kernel startup table scan. */
u32 *systemCallTable __attribute__((section(".data"))) = NULL;

kernelStartupEntryPair_t systemCallEntryPairs[2] = {
    {0x83, {.find = kFindAddress}},
    {0x5A, {.wordCopy = kCopy}},
};
