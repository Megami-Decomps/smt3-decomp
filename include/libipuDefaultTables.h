#ifndef LIBIPU_DEFAULT_TABLES_H
#define LIBIPU_DEFAULT_TABLES_H

#include "common.h"

/* Default words are initialized as low/high little-endian halves and
 * consumed as a 128-bit value by the IPU FIFO's quadword transfers. */
typedef union ipuFifoWord_t {
    u64 halves[2];
    u128 fifo;
} ipuFifoWord_t;

SIZE_CHECK(ipuFifoWordSize, ipuFifoWord_t, 16);
extern ipuFifoWord_t ipuDefaultQuantizerMatrices[5];
extern ipuFifoWord_t ipuDefaultVqClut[2];

#endif
