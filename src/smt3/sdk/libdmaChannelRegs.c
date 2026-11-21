#include "libdma.h"

/* CHCR bases of the ten EE DMA channels, in SDK channel-number order. */
sceDmaChan *sceDmaChanRegs[10] = {
    (sceDmaChan *)0x10008000,
    (sceDmaChan *)0x10009000,
    (sceDmaChan *)0x1000A000,
    (sceDmaChan *)0x1000B000,
    (sceDmaChan *)0x1000B400,
    (sceDmaChan *)0x1000C000,
    (sceDmaChan *)0x1000C400,
    (sceDmaChan *)0x1000C800,
    (sceDmaChan *)0x1000D000,
    (sceDmaChan *)0x1000D400,
};
