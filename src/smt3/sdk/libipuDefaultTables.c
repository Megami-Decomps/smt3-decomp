#include "libipuDefaultTables.h"

/* Four intra-quantizer chunks, followed by one flat non-intra chunk. */
ipuFifoWord_t ipuDefaultQuantizerMatrices[5] = {
    { { 0x1616131013101008ULL, 0x1B1A181A16161616ULL } },
    { { 0x1B1B1A1A1A1A1B1BULL, 0x1D2222221D1D1D1BULL } },
    { { 0x20201D1D1B1B1D1DULL, 0x2223232526252222ULL } },
    { { 0x3030282828262623ULL, 0x5345453A38382E2EULL } },
    { { 0x1010101010101010ULL, 0x1010101010101010ULL } },
};

/* Sixteen 16-bit VQ palette entries, transferred as two FIFO words. */
ipuFifoWord_t ipuDefaultVqClut[2] = {
    { { 0x03E0084204210000ULL, 0x1CE718C614A51084ULL } },
    { { 0x7C00294A2529001FULL, 0x39CE7FFF35AD318CULL } },
};
