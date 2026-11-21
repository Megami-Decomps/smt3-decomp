#ifndef SDF_SCREEN_STATE_H
#define SDF_SCREEN_STATE_H

#include "sdf_types.h"

/* The camera, as sdf3DMakeScreenParam derives it and sdf3DSetCamera copies it
 * into the global sdf3DScreenParam. The first twelve fields are the inputs;
 * TM, scale and trans are what it computes, and cameraPos comes from
 * sdf3DMakeViewportPacket inverting the camera matrix. */
typedef struct sdf3DScreenParam_t
{
    u32 flags; /* sdf_3d_screen_flag */
    f32 aspect;
    f32 projDist; /* unused when FOVY is set */
    f32 fovy;
    f32 ox;
    f32 oy;
    f32 width;
    f32 height;
    f32 zMin;
    f32 zMax;
    f32 nearClip;
    f32 farClip;
    sdfMatrix4_t TM;
    sdfVector4_t scale;
    sdfVector4_t trans;
    sdfVector4_t cameraPos;
} sdf3DScreenParam_t;

extern sdf3DScreenParam_t sdf3DScreenParam;
extern sdfMatrix4_t sdf3DScreenTm;
extern sdfMatrix4_t sdfCameraTm;
extern f32 sdf3DEyeOffset[4];

#endif
