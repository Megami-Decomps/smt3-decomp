#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

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
} fclEncyc_instance_tag;

extern s32 titleAllocWork(s32 arg);

extern s8 dds3PadButtonState[];

extern char D_0053DD98[];

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 titleFreeWork(s32 arg);

typedef struct
{
    /* 0x00 */ void *unk_00;
    /* 0x04 */ void *unk_04;
    /* 0x08 */ void *unk_08;
    /* 0x0C */ u8 unk_0C[0x34];
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ u8 unk_4C[8];
    /* 0x54 */ s32 cursor;
    /* 0x58 */ void *unk_58;
    /* 0x5C */ u8 unk_5C[4];
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 tover[5];
    /* 0x78 */ u8 unk_78[0x10];
    /* 0x88 */ s8 unk_88;
    /* 0x89 */ u8 unk_89[3];
    /* 0x8C */ s32 unk_8C;
    /* 0x90 */ s32 unk_90;
    /* 0x94 */ s32 unk_94[5];
    /* 0xA8 */ s32 unk_A8;
} mselGlobalWork_t;

extern mselGlobalWork_t *gGlobal;

extern void frDestroyFontRequestAfter(void *pFRQ);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

INCLUDE_ASM(const s32, "kernel/dds3TitleMain", dds3Titlemain);

s32 titleStart(s32 mode)
{
    titleAllocWork(mode);
    return -1;
}

s32 titleExit(void)
{
    titleFreeWork(0);
    dds3KillProcessName(D_0053DD98, 1);
    return 0;
}

s32 msel_2286A0(void)
{
    titleAllocWork(0);
    return 0;
}

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

extern void frDrawFontEx(void *pFRQ, s32 Mode);

void mselDrawMenuText(s32 x, s32 y, s32 text, s32 arg)
{
    void *frq = frwGetSmallFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, s32 pStr, s32 pFRQ);

void mselDrawSaveNum(s32 x, s32 y, s32 text, s32 arg)
{
    void *frq = frwGetSmallNumFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

void mselUpdateCursor(void)
{
    if ((dds3PadButtonState[0x26] & 2) != 0) {
        gGlobal->cursor--;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if ((dds3PadButtonState[0x27] & 2) != 0) {
        gGlobal->cursor++;
        p2sdPlaySE(0, 0x7F, 0x3F);
    }
    if (gGlobal->cursor < 0) {
        gGlobal->cursor = 1;
    }
    if (gGlobal->cursor >= 2) {
        gGlobal->cursor = 0;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3TitleMain", mselDrawTitleAnim);

INCLUDE_ASM(const s32, "kernel/dds3TitleMain", mselDrawBackground);

INCLUDE_ASM(const s32, "kernel/dds3TitleMain", mselDrawTitleAnim2);

/* NON_MATCHING: 13 of 27 words differ (first at +0x14). */
#ifdef NON_MATCHING
void mselToggleFlag(u8 *toggle, s32 *value, s8 dir)
{
    s32 v;
    s32 level;

    *toggle = (*toggle == 0);
    if (dir == 0) {
        level = *value;
        v = level;
        level = (v & 0xFF) + 0x10;
    } else {
        level = *value;
        v = level;
        level = (v & 0xFF) - 0x10;
    }
    if (level > 0) {
        if (level > 0x80) {
            level = 0x80;
        }
    } else {
        level = 0;
    }
    *value = (v & ~0xFF) | level;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3TitleMain", mselToggleFlag);
#endif

INCLUDE_ASM(const s32, "kernel/dds3TitleMain", mselMakeSaveText);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE50);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE70);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DE88);

INCLUDE_RODATA(const s32, "kernel/dds3TitleMain", D_0053DEA8);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD38);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD40);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", gGlobal);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", fldWmapEnabled);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD4C);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD50);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD58);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD60);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD68);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD70);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD78);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD80);

INCLUDE_SDATA(const s32, "kernel/dds3TitleMain", D_0054FD88);

