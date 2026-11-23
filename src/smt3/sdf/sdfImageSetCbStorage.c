#include "sdfImageSetCbStorage.h"

typedef char sdfImageSetCbSize[sizeof(sdfImageSetCb_t) == 0x14 ? 1 : -1];

/* The upload queue's single control block; sdfImageSetInit initializes it. */
sdfImageSetCb_t sdfImageSetCb __attribute__((section(".bss")));
