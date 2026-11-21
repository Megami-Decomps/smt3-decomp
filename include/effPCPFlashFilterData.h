#ifndef EFF_PCP_FLASH_FILTER_DATA_H
#define EFF_PCP_FLASH_FILTER_DATA_H

#include "effect.h"

/* Inputs copied and consumed by the flash sphere and multi-filter constructors. */
typedef struct effPCPFlashPolySphereParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 interval;
    u32 abgr_in;
    u32 abgr_out;
    f32 width_in;
    f32 width_out;
    f32 radius;
    f32 rotate;
    u32 blend;
} effPCPFlashPolySphereParam_t;

typedef struct effPCPMultiFilterExpParam_t {
    u8 type;
    u8 pad01[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 area_s;
    s32 area_e;
    dds3Effect_PartialMultiFilterParam_t res;
} effPCPMultiFilterExpParam_t;

extern effPCPFlashPolySphereParam_t D_0031DC20;
extern effPCPMultiFilterExpParam_t D_0031DC60;
extern effPCPMultiFilterExpParam_t D_0031DCA0;

#endif
