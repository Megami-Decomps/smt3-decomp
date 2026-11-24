#ifndef SDF_CORE_RENDER_PARAMETERS_H
#define SDF_CORE_RENDER_PARAMETERS_H

#include "common.h"

/* VU0 input basis and emitted base/delta color vectors. */
typedef struct sdfCorePacketFaceDivParm_t {
    u8 opaque00[4]; /* 0x00 */
    f32 scaledY; /* 0x04 */
    u8 opaque08[4]; /* 0x08 */
    f32 scale; /* 0x0C */
    f32 colorBase[4]; /* 0x10 */
    f32 colorDelta[4]; /* 0x20 */
} sdfCorePacketFaceDivParm_t;

/* The same three quadwords are filled by face setup and copied to VU memory. */
typedef union {
    sdfCorePacketFaceDivParm_t fields;
    u128 quadwords[3];
} sdfCoreFaceDivPacket_t;

extern sdfCoreFaceDivPacket_t corePktFaceDivParm;
extern u128 corePktFogParm;
extern u128 corePktWorldLightMtxCache[3];

#endif
