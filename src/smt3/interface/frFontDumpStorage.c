#include "frFontDumpStorage.h"

SIZE_CHECK(frFontDumpDrawTagSize, sdfDrawTag_t, 0x20);
SIZE_CHECK(frFontDumpDrawEnvSize, frFontGsPacket_t, 0x80);

/* Separate packet and draw-tag pairs for the two frame buffers. */
sdfDrawTag_t frDumpDrawEnvTag[2] __attribute__((section(".bss"), aligned(16)));
frFontGsPacket_t frDumpDrawEnvPacket[2] __attribute__((section(".bss"), aligned(16)));
sdfDrawTag_t frDumpTextureTag[2] __attribute__((section(".bss"), aligned(16)));
frFontDumpSlot_t frDumpFontTexturePacket[2] __attribute__((section(".bss"), aligned(16)));
