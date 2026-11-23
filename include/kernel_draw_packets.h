#ifndef KERNEL_DRAW_PACKETS_H
#define KERNEL_DRAW_PACKETS_H

#include "draw.h"

/* Two-context Z-clear packets are 0xB0 bytes, two buffers per state. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t testGif;
    sdfGrTagStdReg_t test;
    sdfGifTag_t clearGif;
    sdfGrTagStdReg_t colorA;
    sdfGrTagStdReg_t positionA;
    sdfGrTagStdReg_t colorB;
    sdfGrTagStdReg_t positionB;
    sdfGifTag_t blendGif;
    sdfGrTagStdReg_t blend[2];
} dds3ZClearPacket_t;

/* The fade sprite packet: a four-register state block then a textureless
 * sprite, 0xB0 bytes, one per frame buffer. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t stateGif;
    sdfGrTagStdReg_t state[4];
    sdfGifTag_t spriteGif;
    sdfGrTagStdReg_t sprite[4];
} dds3FadePacket_t;

SIZE_CHECK(dds3ZClearPacketSizeCheck, dds3ZClearPacket_t, 0xB0);
SIZE_CHECK(dds3FadePacketSizeCheck, dds3FadePacket_t, 0xB0);

extern dds3ZClearPacket_t dds3ZClearDrawTag[3][2];
extern dds3FadePacket_t dds3FadePacket[2];

#endif
