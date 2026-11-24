#include "sdfCoreRenderParameters.h"

sdfCoreFaceDivPacket_t corePktFaceDivParm __attribute__((section(".bss")));
u128 corePktFogParm __attribute__((section(".bss")));
u128 corePktWorldLightMtxCache[3] __attribute__((section(".bss")));

SIZE_CHECK(corePktFaceDivParamSize, sdfCorePacketFaceDivParm_t, 0x30);
SIZE_CHECK(corePktFaceDivPacketSize, sdfCoreFaceDivPacket_t, 0x30);
SIZE_CHECK(corePktWorldLightCacheSize, u128[3], 0x30);
