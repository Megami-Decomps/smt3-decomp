#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x7C];
    /* 0x007C */ s8 unk_7C;
    /* 0x007D */ s8 unk_7D;
    /* 0x007E */ u8 unk_007E[0xEE];
    /* 0x016C */ void *unk_16C;
    /* 0x0170 */ u8 unk_170[0x112E];
    /* 0x129E */ s8 unk_129E;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[0x103F];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x2F];
    /* 0x2314 */ s32 fadeColor;
} rstDrawWork_t;

extern rstDrawWork_t *rstDrawWork;

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

void rstDrawEvoBgFade(void)
{
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, rstDrawWork->fadeColor, 7);
}

INCLUDE_ASM(const s32, "rstdraw", rstDraw);
