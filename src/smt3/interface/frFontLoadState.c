#include "common.h"

/* Writable load-state flags for the nine native font slots. */
u32 frFontLoaded[9] __attribute__((section(".data"))) = { 0 };
