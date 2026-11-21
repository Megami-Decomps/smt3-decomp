#ifndef EFF_THUNDER_POLY_PRESET_DATA_H
#define EFF_THUNDER_POLY_PRESET_DATA_H

#include "common.h"

/* Native parameter prefixes copied into the two effect work objects. */
typedef struct effPCP_ThunderPolyParam_t effPCP_ThunderPolyParam_t;
struct effPCP_ThunderPolyParam_t {
    f32 start[4];
    f32 end[4];
    u16 blend;         /* 0x20 */
    u8 pad22[2];
    u32 number;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 split;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 color_center;     /* 0x40: packed center color used by symmetric cell ramps */
    f32 width_border;          /* 0x44: width of the continued strip */
    u32 color_border;    /* 0x48: packed middle color used by symmetric cell ramps */
    f32 width_edge;          /* 0x4C: added outer margin */
    u32 color_edge;      /* 0x50: packed edge color used by symmetric cell ramps */
};

extern effPCP_ThunderPolyParam_t D_0031D310;
extern effPCP_ThunderPolyParam_t D_0031D370;
extern effPCP_ThunderPolyParam_t D_0031D3D0;
extern effPCP_ThunderPolyParam_t D_0031D430;
extern effPCP_ThunderPolyParam_t D_0031D490;
extern effPCP_ThunderPolyParam_t D_0031D4F0;
extern effPCP_ThunderPolyParam_t D_0031D550;
extern effPCP_ThunderPolyParam_t D_0031D5B0;
extern effPCP_ThunderPolyParam_t D_0031D610;
extern effPCP_ThunderPolyParam_t D_0031D670;
extern effPCP_ThunderPolyParam_t D_0031D6D0;
extern effPCP_ThunderPolyParam_t D_0031D730;
extern effPCP_ThunderPolyParam_t D_0031D790;
extern effPCP_ThunderPolyParam_t D_0031D7F0;
extern effPCP_ThunderPolyParam_t D_0031D850;
extern effPCP_ThunderPolyParam_t D_0031D8B0;
extern effPCP_ThunderPolyParam_t D_0031D910;
extern effPCP_ThunderPolyParam_t D_0031D970;
extern effPCP_ThunderPolyParam_t D_0031D9D0;
extern effPCP_ThunderPolyParam_t D_0031DA30;
extern effPCP_ThunderPolyParam_t D_0031DA90;
extern effPCP_ThunderPolyParam_t D_0031DAF0;

typedef struct effPCP_ThunderRadiatePolyParam_t effPCP_ThunderRadiatePolyParam_t;
struct effPCP_ThunderRadiatePolyParam_t {
    f32 origin[4];
    u16 blend;
    u8 pad12[2];
    f32 length;
    u32 number;
    u8 opaque1C[8];
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 split;
    u8 pad2E[2];
    f32 coreWidth;
    u32 color_center;
    f32 bandWidth;
    u32 color_border;
    f32 edgeWidth;
    u32 color_edge;
};

extern effPCP_ThunderRadiatePolyParam_t D_0031DB50;

#endif
