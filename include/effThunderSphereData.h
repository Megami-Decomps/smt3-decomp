#ifndef EFF_THUNDER_SPHERE_DATA_H
#define EFF_THUNDER_SPHERE_DATA_H

#include "common.h"

typedef struct effPCPThunderSpherePolyParam_t effPCPThunderSpherePolyParam_t;

/* The sphere-poly parameters, 0x4C bytes the constructors copy whole. The tail
 * is the thunder family's six fields: a width and a colour for the centre, the
 * border and the edge. The decomp read it twice, once for the widths and once
 * for the colours, which is why the two views agreed on everything before. */
struct effPCPThunderSpherePolyParam_t {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 width;
    f32 height;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[2];
    f32 width_center; /* 0x34: nothing reads it */
    u32 color_center;
    f32 width_border;
    u32 color_border;
    f32 width_edge;
    u32 color_edge;
};

extern effPCPThunderSpherePolyParam_t D_0031D040;
extern effPCPThunderSpherePolyParam_t D_0031D090;
extern effPCPThunderSpherePolyParam_t D_0031D0E0;
extern effPCPThunderSpherePolyParam_t D_0031D130;
extern effPCPThunderSpherePolyParam_t D_0031D180;
extern effPCPThunderSpherePolyParam_t D_0031D1D0;
extern effPCPThunderSpherePolyParam_t D_0031D220;
extern effPCPThunderSpherePolyParam_t D_0031D270;
extern effPCPThunderSpherePolyParam_t D_0031D2C0;

#endif
