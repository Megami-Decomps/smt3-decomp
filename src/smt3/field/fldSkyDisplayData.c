#include "common.h"

/* Field initialization clears five words, then publishes entries 0 and 1. */
u32 fldSkyDisplayPointers[5] __attribute__((section(".data"))) = { 0 };
