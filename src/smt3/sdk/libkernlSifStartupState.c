#include "common.h"

/* Native SIF startup words, reset before the corresponding setup. */
s32 D_0052C2B8 __attribute__((section(".data"))) = 0;
s32 sifRpcInitialized __attribute__((section(".data"))) = 0;
