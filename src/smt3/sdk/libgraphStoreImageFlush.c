#include "common.h"

/* VIF MSKPATH3 (unmask PATH3) followed by three NOPs, stored as one FIFO quadword. */
const u128 sceGsStoreImageFlushPacket __attribute__((section(".data"))) = 0x06000000;
