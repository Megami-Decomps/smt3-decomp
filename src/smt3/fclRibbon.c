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
    /* 0x00 */ void *allocation;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ s32 activeTime;
    /* 0x18 */ s32 deactiveTime;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
} fclRbnWork_t;

extern void sdfMemFreeDirect(void *ptr);

extern void MoveActive(fclRbnWork_t *work);

extern void MoveDeactive(fclRbnWork_t *work);

extern void fclRbnRelease(fclRbnWork_t *work);

extern void fclRbnSwitchActive(fclRbnWork_t *rbn, s32 index);

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

extern void fclRbnRelease(fclRbnWork_t *work);

INCLUDE_ASM(const s32, "fclRibbon", fclRbnInit);

void fclRbnRelease(fclRbnWork_t *work)
{
    sdfMemFreeDirect(work->allocation);
}

void fclRbnCalc(fclRbnWork_t *work)
{
    if (work->flags & 1) {
        MoveActive(work);
    }
    if (work->flags & 2) {
        MoveDeactive(work);
    }
    work->phase = (work->phase - 2) & 0x7F;
}

INCLUDE_ASM(const s32, "fclRibbon", fclRbnDraw);

void fclRbnSwitchActive(fclRbnWork_t *work, s32 index)
{
    if (index >= 0 && index < work->count && index != work->unk_10)
    {
        work->unk_12 = work->unk_10;
        work->flags |= 3;
        work->unk_10 = index;
        work->activeTime = 0x40;
        work->deactiveTime = 0x80;
        work->phase = 0;
    }
}

INCLUDE_ASM(const s32, "fclRibbon", fclRbnSetPara);

INCLUDE_ASM(const s32, "fclRibbon", dumpCursorInfo);

void MoveActive(fclRbnWork_t *work)
{
    work->activeTime -= 8;
    if (work->activeTime <= 0) {
        work->activeTime = 0;
        work->flags &= ~1;
    }
}

void MoveDeactive(fclRbnWork_t *work)
{
    work->deactiveTime -= 0x10;
    if (work->deactiveTime <= 0) {
        work->deactiveTime = 0;
        work->flags &= ~2;
    }
}

INCLUDE_ASM(const s32, "fclRibbon", DrawBackFrame);

INCLUDE_ASM(const s32, "fclRibbon", DrawBack);

INCLUDE_ASM(const s32, "fclRibbon", DrawHexagon);

INCLUDE_ASM(const s32, "fclRibbon", DrawHexagonElem);

INCLUDE_ASM(const s32, "fclRibbon", DrawElem);

INCLUDE_RODATA(const s32, "fclRibbon", D_0053D9D8);

INCLUDE_RODATA(const s32, "fclRibbon", D_0053D9E8);

INCLUDE_RODATA(const s32, "fclRibbon", D_0053D9F8);

INCLUDE_RODATA(const s32, "fclRibbon", D_0053DA08);

