#ifndef FR_FONT_DUMP_STORAGE_H
#define FR_FONT_DUMP_STORAGE_H

#include "sdf_resource.h"

typedef union frFontDmaTag_t frFontDmaTag_t;

typedef struct frFontGsWrite_t frFontGsWrite_t;

typedef struct frFontGsPacket_t frFontGsPacket_t;

/* The SDK DMA tag is accessed as a quadword and as its four command words. */
union frFontDmaTag_t {
    u128 value;
    u32 words[4];
};

struct frFontGsWrite_t {
    u64 value;
    u64 reg;
};

struct frFontGsPacket_t {
    frFontDmaTag_t dma;
    u64 gifTag;
    u64 gifRegisters;
    frFontGsWrite_t writes[6];
};

typedef struct
{
    u32 header[4];
    u64 words[32];
} frFontDumpSlot_t;
SIZE_CHECK(frFontDumpSlotSizeCheck, frFontDumpSlot_t, 0x110);

extern sdfDrawTag_t frDumpDrawEnvTag[2];
extern frFontGsPacket_t frDumpDrawEnvPacket[2];
extern sdfDrawTag_t frDumpTextureTag[2];
extern frFontDumpSlot_t frDumpFontTexturePacket[2];
#endif
