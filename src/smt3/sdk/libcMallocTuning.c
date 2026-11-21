#include "common.h"

/* The allocator reads these as full-width values from its .data state. */
u64 __malloc_trim_threshold __attribute__((section(".data"))) = 0x20000;
u64 __malloc_top_pad __attribute__((section(".data"))) = 0;
