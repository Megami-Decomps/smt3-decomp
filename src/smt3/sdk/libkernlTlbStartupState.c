#include "sdk_kernel_startup_entries.h"

extern s32 kCopy_3(u32 *, const u32 *, u32);

/* InitTLB publishes the argument-storage base after installing the handlers. */
void *execArgumentBlock __attribute__((section(".data"))) = NULL;

/* Initial entries used by the native registration sequence. The resident
 * destinations are EE entry addresses, not C call targets. */
kernelStartupEntryPair_t tlbEntryPairs[8] = {
    {0x5A, {.wordCopy = kCopy_3}},
    {0x5B, {.resident = (void *)0x80075000}},
    {0x54, {.resident = (void *)0x002E5100}}, /* Resume entry within _xlaunch. */
    {0x55, {.resident = NULL}},
    {0x56, {.resident = NULL}},
    {0x57, {.resident = NULL}},
    {0x58, {.resident = NULL}},
    {0x59, {.resident = NULL}},
};
