#include "common.h"

/* Native heap-lock owner and recursion depth. */
s32 mallocLockOwner __attribute__((section(".data"))) = -1;
s32 mallocLockCount __attribute__((section(".data"))) = 0;
