#ifndef EFF_PCP_BOSS_DRAW_DATA_H
#define EFF_PCP_BOSS_DRAW_DATA_H

#include "common.h"

typedef struct effPCPDANTE_UV_tag
{
    s32 u;
    s32 v;
    u32 w;
    u32 h;
} effPCPDANTE_UV_tag;

extern f32 D_0031FF70[4];
extern f32 effPCPBossLookDown[4];
extern f32 effPCPBossKAGUDrawParam[4];
extern f32 effPCPBossKAGU2DrawParam[4];
extern effPCPDANTE_UV_tag effPCPDANTE_Bingo_UV;
extern effPCPDANTE_UV_tag effPCPDANTE_AreYouReady_UV;
extern effPCPDANTE_UV_tag effPCPDANTE_ShowTime_UV;

#endif
