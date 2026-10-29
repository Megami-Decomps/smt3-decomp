#ifndef KERNEL_DRAW_H
#define KERNEL_DRAW_H

#include "draw.h"
#include "draw3d.h"

/* Native offsets: parallel lights 0x00, ambient 0x10, fog 0x20. */
typedef struct
{
    sdf3DLight_t *parallel[4];
    sdfVector4_t ambient;
    sdf3DFogParam_t fog;
} dds3LightState_t;

/* sdfGrSetStdDrawEnv and sdfGrLinkStdDrawEnv are handed the DMA tag that
 * prefixes the draw env, so the env's own packet header sits 0x10 in. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGrDrawEnv_t env;
    u8 unk180[0xA0];
} dds3StdDrawEnv_t;

/* One frame buffer's draw bank, stride 0x1BE0. Three draw tags: the VU1 init
 * chain at 0x0, the scene chain at 0x12D0, and the second-screen chain at
 * 0x1A10. */
typedef struct
{
    sdfDrawTag_t initTag;
    sdf3DInitPacket_t initPacket;
    sdfModelDrawEnv_t modelEnv[11];
    sdfDrawTag_t drawTag;
    sdfPreDrawTag_t preDrawTag;
    dds3StdDrawEnv_t stdEnv;
    sdf3DViewportPacket_t viewport;
    sdf3DLightParallelPacket_t light;
    sdfDrawTag_t renderTag;
    sdfGrDrawEnv_t renderEnv;
    sdf3DViewportPacket_t renderViewport;
    sdf3DLightParallelPacket_t renderLight;
    sdfDrawTag_t subTag;
    sdf3DViewportPacket_t subViewport;
    sdf3DLightParallelPacket_t subLight;
} dds3DrawBank_t;

/* Native frame renderer ordering table: map size 0xAA0, stride 0x20. */
extern sdfGrOt_t sdfGrOtTable[85];

#endif
