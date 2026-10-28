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
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
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
    /* 0x08 */ sdfMemHandle_t *unk_08;
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

extern mselGlobalWork_t *gGlobal;

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);
extern s32 sprintf(char *buffer, const char *format, ...);
extern char D_0054FD50[];
extern char D_0054FD58[];
extern char *datExplainTable[];
extern char *text2[];


extern void frDestroyFontRequestAfter(void *pFRQ);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern void eglSetOt(u32 ot);

extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void eglLinkAlpha(s32 mode);

extern void eglLinkSprite(s32 x, s32 y, s32 w, s32 h,
                          s32 u, s32 v, s32 uw, s32 vh,
                          u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

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

extern f32 sdfSin(f32 angle);
extern char *D_00342FF8[];
extern char *D_00343008[];

void mselDrawTitleAnim(void)
{
    u32 color;

    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 512, 448, 0, 0, 512, 448,
                  0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->unk_40);
    gGlobal->cur_alphacnt = (gGlobal->cur_alphacnt + 10) % 360;
    gGlobal->cur_alpha = (u32)((sdfSin((f32)((gGlobal->cur_alphacnt + 90) % 360) /
                                     180.0f * 0x1.921f9ep+1f) + 1.0f) *
                              0.5f * 88.0f + 104.0f);
    color = 0x80808000 | (u8)gGlobal->cur_alpha;
    switch (gGlobal->cursor) {
    case 0:
        eglLinkSprite(38, 162, 132, 30, 249, 455, 132, 30,
                      color, color, color, color, gGlobal->unk_40);
        eglLinkSprite(38, 198, 99, 30, 146, 455, 99, 30,
                      0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      gGlobal->unk_40);
        break;
    case 1:
        eglLinkSprite(38, 162, 132, 30, 10, 455, 132, 30,
                      0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      gGlobal->unk_40);
        eglLinkSprite(38, 198, 99, 30, 385, 455, 99, 30,
                      color, color, color, color, gGlobal->unk_40);
        break;
    }
    switch (gGlobal->cursor) {
    case 0:
        mselDrawMenuText(239, 82, 0x64618780, (s32)D_00342FF8[0]);
        mselDrawMenuText(239, 94, 0x64618780, (s32)D_00342FF8[1]);
        mselDrawMenuText(239, 106, 0x64618780, (s32)D_00342FF8[2]);
        return;
    case 1:
        mselDrawMenuText(239, 82, 0x64618780, (s32)D_00343008[0]);
        mselDrawMenuText(239, 94, 0x64618780, (s32)D_00343008[1]);
        mselDrawMenuText(239, 106, 0x64618780, (s32)D_00343008[2]);
        break;
    }
}

void mselDrawBackground(void)
{
    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                  0x80101010, 0x80101010, 0x80101010, 0x80101010,
                  gGlobal->unk_48);
}

extern f32 sdfSin(f32 angle);
extern void mselToggleFlag(u8 *toggle, s32 *value, s8 dir);
extern void mselMakeSaveText(s32 *devilIds, u32 color, u32 nameColor);

void mselDrawTitleAnim2(s32 animate)
{
    eglSetOt(0x47);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                  0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  gGlobal->unk_48);
    mselToggleFlag(&gGlobal->Zalpha_cnt, (s32 *)&gGlobal->Z_RGBA, 0);
    mselToggleFlag((u8 *)&gGlobal->unk_88, &gGlobal->unk_8C, 1);
    if (animate == 0) {
        mselMakeSaveText(gGlobal->tover, gGlobal->Z_RGBA, gGlobal->Z_RGBA);
    } else {
        gGlobal->ZNalpha_cnt = (gGlobal->ZNalpha_cnt + 10) % 360;
        gGlobal->ZNalpha_cnt = gGlobal->ZNalpha_cnt <= 0 ? 0 :
            (gGlobal->ZNalpha_cnt <= 180 ? gGlobal->ZNalpha_cnt : 180);
        gGlobal->ZN_RGBA = (u32)((sdfSin((f32)((gGlobal->ZNalpha_cnt + 90) % 360) /
                                       180.0f * 0x1.921f9ep+1f) + 1.0f) *
                                0.5f * 127.0f + 128.0f);
        mselMakeSaveText(gGlobal->tover, gGlobal->Z_RGBA,
                         (gGlobal->Z_RGBA & 0xFFFFFF00) | gGlobal->ZN_RGBA);
    }
    mselMakeSaveText(gGlobal->unk_94, gGlobal->unk_8C, gGlobal->unk_8C);
}

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

/* Layout recovered from facility/fclEncyc.c; native indexes these rows with
 * a 0xB0 stride from offset 0x10 of the handle's data. */
typedef struct fclencyceelem_t {
    u16 id;
    u16 reserved0;
    u16 maxhp;
    u16 maxmp;
    u32 exp;
    u16 level;
    s8 param[6];
    s8 skillparam[6];
    u16 skillcnt;
    u16 skill[24];
    s8 mitamaparam[6];
    s8 levelupparam[6];
    u16 keisyoskill[24];
    u8 keiattr[24];
    u16 hensinmae;
    u16 reserved1;
    u32 flag;
    u32 reserved2;
    u32 hensinmaeexp;
} fclencyceelem_t;

typedef struct mselSaveData_t {
    u8 header[0x10];
    fclencyceelem_t records[1];
} mselSaveData_t;

typedef struct datDevilFormat_t {
    u32 flag;
    u8 race, level;
    u16 hp, maxhp, mp, maxmp;
    s16 aisyoid;
    s8 param[6];
    u16 skill[8];
    s16 keisyotype;
    u16 keisyoform, dropmakka, dropexp;
    u8 dropitem[2], droppoint[2];
    u16 specialbit;
    u8 specialitem, specialpoint, hougyokupoint, masekipoint;
    s8 attackattr;
    u8 attackcnt, attackinterval, reserve1;
} datDevilFormat_t;
extern datDevilFormat_t datDevilTable[];

void mselMakeSaveText(s32 *devilIds, u32 color, u32 nameColor)
{
    s32 index;
    s32 devilId;
    s32 level;
    s32 y;
    char text[0x20];
    mselSaveData_t *saveData;

    saveData = (mselSaveData_t *)sdfMemGetPtr(gGlobal->unk_08);
    for (index = 0; index < 5; index++) {
        devilId = devilIds[index];
        y = 0x32 + index * 0x10;
        if (devilId != -1) {
            fclencyceelem_t *record = &saveData->records[devilId];
            mselDrawMenuText(0x64, y, color, (s32)datExplainTable[datDevilTable[record->id].race]);
            mselDrawMenuText(0xA4, y, nameColor, (s32)text2[record->id]);
            sprintf(text, D_0054FD50);
            mselDrawMenuText(0x140, y, color, (s32)text);
            level = record->level;
            if (level >= 0x64) {
                level = 0x63;
            }
            sprintf(text, D_0054FD58, level);
            mselDrawSaveNum(0x15A, y + 2, color, (s32)text);
        }
    }
}

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

