#include "common.h"

/* libpad starts uninitialized, with diagnostic printing enabled. */
s32 padInitialized __attribute__((section(".data"))) = 0;
s32 padDebugPrint __attribute__((section(".data"))) = 1;
