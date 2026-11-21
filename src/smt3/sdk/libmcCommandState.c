#include "common.h"

/* Track the outstanding memory-card command and its semaphore. */
s32 mcLastCmd __attribute__((section(".data"))) = 0;
s32 mcSema __attribute__((section(".data"))) = -1;
