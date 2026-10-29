#ifndef MVI_MANAGER_H
#define MVI_MANAGER_H

#include "common.h"

typedef struct
{
    /* 0x00 */ u8 unk_00[0x3A];
    /* 0x3A */ u8 viewerStep;
    /* 0x3B */ u8 unk_3B[0x29];
    /* 0x64 */ s32 frameCount;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 frameIndex;
} movieDecodeWork_t;

extern movieDecodeWork_t movieDecodeWork asm("D_00343E40");

#endif
