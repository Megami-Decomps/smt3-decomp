#ifndef SND_MANAGER_H
#define SND_MANAGER_H

#include "libsdf.h"

typedef struct
{
    /* 0x00 */ sdfMemHandle_t *allocation;
    /* 0x04 */ sdfTexHandle_t *texHandle[5];
    /* 0x18 */ s32 mode;
    /* 0x1C */ s32 cursor;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 unk_3C;
} dds3TitleWork_t;

#endif
