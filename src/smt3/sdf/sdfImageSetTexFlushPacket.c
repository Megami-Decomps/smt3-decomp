#include "draw.h"

/* DMA END and VIF DIRECT each carry two quadwords. The packed GIFtag
 * terminates after one A+D write of zero to the GS TEXFLUSH register.
 * The DMA NEXT link points here, so hardware requires 16-byte alignment. */
sdfPrimTexflushPacket_t sdfImageSetTexFlushPacket __attribute__((aligned(16))) = {
    {
        { { 2, 0, SDF_DMA_TAG_ID_END << 4, NULL, 0, 0x50000002 } },
        { 0x1000000000008001ULL, 0xE },
    },
    { { 0, SDF_GS_REG_TEXFLUSH } },
};
