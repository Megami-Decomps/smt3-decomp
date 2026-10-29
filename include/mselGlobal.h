#ifndef MSEL_GLOBAL_H
#define MSEL_GLOBAL_H

#include "sdf_resource.h"

typedef struct
{
    /* 0x00 */ void *unk_00;
    /* 0x04 */ void *unk_04;
    /* 0x08 */ sdfMemHandle_t *unk_08;
    /* 0x0C */ u8 unk_0C[0x34];
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ s32 cursor;
    /* 0x58 */ void *unk_58;
    /* 0x5C */ void *unk_5C;
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 tover[5];
    /* 0x78 */ u8 Zalpha_cnt;
    /* 0x79 */ u8 reserved79[3];
    /* 0x7C */ u32 Z_RGBA;
    /* 0x80 */ u32 ZNalpha_cnt;
    /* 0x84 */ u32 ZN_RGBA;
    /* 0x88 */ s8 unk_88;
    /* 0x89 */ u8 unk_89[3];
    /* 0x8C */ s32 unk_8C;
    /* 0x90 */ s32 unk_90;
    /* 0x94 */ s32 unk_94[5];
    /* 0xA8 */ s32 unk_A8;
    /* 0xAC */ u32 cur_alpha;
    /* 0xB0 */ u32 cur_alphacnt;
} mselGlobalWork_t;

#endif
