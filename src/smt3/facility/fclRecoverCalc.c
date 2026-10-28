#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern s32 datGetMaxHp(datUnitWork_t *work);

extern s32 datGetMaxMp(datUnitWork_t *work);

s32 fclGetFlag(s8 index);

typedef struct
{
    /* 0x00 */ u8 animState;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08[0xC];
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelAnim_t;

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct
{
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 Flag;
    /* 0x06 */ u16 res;
} fclSpriteEntry_t;

typedef struct
{
    /* 0x00 */ s8 tex;
    /* 0x02 */ fclSpriteRect_t rect;
} fclSpriteRes_t;

typedef struct
{
    /* 0x00 */ fclSpriteRes_t *res;
    /* 0x04 */ fclSpriteEntry_t *entries;
} fclSpriteTbl_t;

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

extern void rcvReviveUnit(datUnitWork_t *work);

extern char D_0053D3F8[];

extern char D_0053D408[];

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

extern void frDestroyFontRequestAfter(void *pFRQ);

extern s32 sprintf(char *out, const char *fmt, ...);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern s32 trmDrawListText(fclPanelPos_t *panel, char *text, s32 *tex,
                            fclSpriteTbl_t *table, s32 ot);

extern void shopDrawItemCount(fclPanelPos_t *pos, s32 color, s32 id);

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

extern s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);

extern void fclDrawSpriteFilter(s32 x, s32 y, s32 d, u8 filter, s32 *color, s32 id, s32 idx,
                                s32 *texs, fclSpriteTbl_t *table, s32 OtNo);

extern void *frwGetNumFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, char *pStr, s32 pFRQ);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

extern fclSpriteTbl_t fclSpriteTable[];

extern s32 D_00345D88[];

extern s32 cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 a,
                                  s32 mode, s32 b, s32 id);

s32 rcvCalcPriceRecover(datUnitWork_t *work)
{
    u32 reviveHp = 0;
    s32 hpCost;
    u32 mpCost;
    if (work->badstatus & 0x800) {
        reviveHp = ((u32)datGetMaxHp(work) + 9) / 10;
    }
    hpCost = datGetMaxHp(work) - (work->hp + reviveHp);
    mpCost = datGetMaxMp(work) - work->mp;
    return (f32)hpCost + (f32)mpCost * 2.5f;
}

s32 rcvGetStatusPrice(datUnitWork_t *work)
{
    u16 status = work->badstatus;
    s32 price = 0;

    if (status & 0x20) {
        price = 100;
    }
    if (status & 0x40) {
        price = 100;
    }
    if (status & 0x100) {
        price = 100;
    }
    return price;
}

s32 rcvGetRevivePrice(datUnitWork_t *work)
{
    return (work->badstatus & 0x800) ? 400 : 0;
}

s32 rcvGetHealCost(void)
{
    return 400;
}

extern s32 rcvCalcPriceRecover(datUnitWork_t *pStock);

s32 rcvGetPriceForUnit(datUnitWork_t *work)
{
    s32 price = rcvCalcPriceRecover(work);

    price += rcvGetStatusPrice(work);
    price += rcvGetRevivePrice(work);
    return price;
}

s32 rcvGetRecoverPrice(datUnitWork_t *work, u8 type)
{
    switch (type) {
    case 7: return rcvGetPriceForUnit(work);
    case 1: return rcvCalcPriceRecover(work);
    case 2: return rcvGetStatusPrice(work);
    case 4: return rcvGetRevivePrice(work);
    default: return -1;
    }
}

s32 rcvAppAllRecover(datUnitWork_t *pStock)
{
    pStock->hp = datGetMaxHp(pStock);
    pStock->mp = datGetMaxMp(pStock);
    return 1;
}

void rcvClearStatusBits(datUnitWork_t *work)
{
    work->badstatus &= 0xFE9F;
}

void rcvReviveUnit(datUnitWork_t *unit)
{
    unit->badstatus &= 0xF7FF;
    unit->hp = (u32)(datGetMaxHp(unit) + 9) / 10;
}

void rcvClearDeadFlag(datUnitWork_t *work)
{
    work->badstatus &= 0x7FFF;
}

void sfply_AdjustPrepEnd(datUnitWork_t *unit)
{
    rcvReviveUnit(unit);
    rcvAppAllRecover(unit);
    rcvClearStatusBits(unit);
}

INCLUDE_ASM(const s32, "facility/fclRecoverCalc", rcvFullHeal);

INCLUDE_ASM(const s32, "facility/fclRecoverCalc", rcvCreateLocalStock);

typedef struct rcvCalcWork_t {
    u8 flags;
    s8 sequence;
    s8 partyCount;
    s8 reserveCount;
    s8 sequenceChanged;
    s8 stock[16];
    u8 unk0015[0xB];
    cmpCursorInfo_t cursor;
    u8 unk0058[0x1004];
    s32 recoverPrices[16];
    u8 unk109C[0x3430];
    s32 script;
    s32 scriptProcess;
    u8 unk44D4[0x24];
    s32 unk44F8;
} rcvCalcWork_t;

typedef struct cmpInfoWindow_t cmpInfoWindow_t;
extern rcvCalcWork_t *rcvCalcState;
extern void *dds3GetProcessData(s32 id);
extern s32 dds3GetProcessStat(s32 id);
extern void cmpClearCursorSlots(cmpCursorInfo_t *cursor);
extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern void fclClearFlag(s8 index);
extern void cmpUpdateTerminate(cmpInfoWindow_t *window);

s32 rcvCalc(s32 id)
{
    rcvCalcWork_t *work;
    rcvCalcState = dds3GetProcessData(id);
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    work = rcvCalcState;
    switch (work->sequence) {
    case 0:
        work->unk44F8 = 0;
        cmpClearCursorSlots(&work->cursor);
        if (rcvCalcState->sequenceChanged == 1) {
            if (dds3GetProcessStat(rcvCalcState->scriptProcess) != 2) {
                rcvCalcState->scriptProcess = fclScriptStart(rcvCalcState->script, 0, NULL);
            }
        }
        break;
    case 5:
        if (work->sequenceChanged == 1) {
            work->recoverPrices[0] = rcvGetHealCost();
            fclClearFlag(2);
        }
        break;
    }
    rcvCalcState->sequenceChanged = 0;
    if (rcvCalcState->flags & 0x10) {
        cmpUpdateTerminate((cmpInfoWindow_t *)rcvCalcState);
        return -1;
    }
    return 0;
}


typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1130];
    /* 0x1130 */ lstWork_t list;
    /* 0x12C4 */ u8 unk_12C4[0x3210];
    /* 0x44D4 */ s32 texHandle[3];
} rcvDrawWork_t;

extern rcvDrawWork_t *rcvDrawWork;

extern void rcvDrawPanelAt(lstWork_t *list, s32 flag, s32 arg);

extern char *D_003306F0[];

extern char *D_003306F4[];

extern char *D_003306F8[];

extern char *D_003306FC[];

extern fclSpriteTbl_t D_00330800[];

extern char D_0054FBF0[];

void rcvDrawPanelRow(fclPanelPos_t *pos, s32 color, s32 num, s32 ot)
{
    s32 colors[4];
    char buf[16];
    void *frq;
    s32 target;
    s32 alpha;

    target = fclGetPanelAnimTarget((fclPanelAnim_t *)((u8 *)rcvDrawWork + 0x4520));
    alpha = (0x200 - fclGetPanelAnimPhase((fclPanelAnim_t *)((u8 *)rcvDrawWork + 0x4520))) / 4;
    color = (color & ~0xFF) | (alpha & 0xFF);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteFilter(pos->unk_00 + target + 0x10D0, pos->unk_04 + 0x168, pos->unk_08 << 4, 1,
                        colors, 0, 1, rcvDrawWork->texHandle, D_00330800, ot);
    fclDrawSpriteFilter(pos->unk_00 + target + 0x10D0, pos->unk_04 + 0x168, pos->unk_08 << 4, 1,
                        colors, 0, 0xA, rcvDrawWork->texHandle, D_00330800, ot);
    fclDrawSpriteFilter(pos->unk_00 + target + 0x1E40, pos->unk_04 + 0x1A0, pos->unk_08 << 4, 1,
                        colors, 0xA, 1, D_00345D88, fclSpriteTable, ot);
    color = (color & 0xFF) | 0xA09DC300;
    sprintf(buf, D_0054FBF0, num);
    frq = frwGetNumFontFRQ(pos->unk_00 + target + 0x1840, pos->unk_04 + 0x1A0, pos->unk_08, color,
                           buf, 0);
    func_001C7E38(frq, 1, ot);
    frDestroyFontRequestAfter(frq);
}

extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page,
                            s32 spriteId, s32 *textures, fclSpriteTbl_t *table, s32 ot);

void rcvDrawRowBox(fclPanelPos_t *panel, s32 ot)
{
    cmpDrawBoxF(0x400, 0x308, panel->unk_08, 0x74, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 6,
                     rcvDrawWork->texHandle, D_00330800, ot);
    cmpDrawBoxF(0xBA0, 0x308, panel->unk_08, 0x52, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 7,
                     rcvDrawWork->texHandle, D_00330800, ot);
    cmpDrawBoxF(0x1180, 0x308, panel->unk_08, 0x52, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 8,
                     rcvDrawWork->texHandle, D_00330800, ot);
    cmpDrawBoxF(0x1740, 0x308, panel->unk_08, 0x64, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 9,
                     rcvDrawWork->texHandle, D_00330800, ot);
}


INCLUDE_ASM(const s32, "facility/fclRecoverCalc", rcvDrawRowText);

INCLUDE_ASM(const s32, "facility/fclRecoverCalc", rcvDrawPanelAt);

void rcvDrawSeq1(void)
{
    fclPanelPos_t panel;

    rcvDrawPanelAt(&rcvDrawWork->list, fclGetFlag(0) == 0, 0x47);
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
    trmDrawListText(&panel, D_003306F0[0], rcvDrawWork->texHandle, D_00330800, 0x47);
}

void rcvDrawSeq2(void)
{
    fclPanelPos_t panel;

    rcvDrawPanelAt(&rcvDrawWork->list, fclGetFlag(0) == 0, 0x47);
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
    trmDrawListText(&panel, D_003306F4[0], rcvDrawWork->texHandle, D_00330800, 0x47);
}

void rcvDrawSeq3(void)
{
    fclPanelPos_t panel;

    rcvDrawPanelAt(&rcvDrawWork->list, fclGetFlag(0) == 0, 0x47);
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
    trmDrawListText(&panel, D_003306F8[0], rcvDrawWork->texHandle, D_00330800, 0x47);
}

void rcvDrawSeq4(void)
{
    fclPanelPos_t panel;

    rcvDrawPanelAt(&rcvDrawWork->list, fclGetFlag(0) == 0, 0x47);
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
    trmDrawListText(&panel, D_003306FC[0], rcvDrawWork->texHandle, D_00330800, 0x47);
}

extern char *D_00330700[];

void rcvDrawItemList(void)
{
    fclPanelPos_t panel;

    fclGetFlag(0);
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&panel);
    shopDrawItemCount(&panel, 0x80808080, 0x47);
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
    trmDrawListText(&panel, D_00330700[0], rcvDrawWork->texHandle, D_00330800, 0x47);
}


void rcvUpdateItemList(s32 active)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (active != 0) {
        mode = 0;
        if (fclGetFlag(0) == 0) {
            mode = 2;
            if (*(s8 *)((u8 *)rcvDrawWork + 3) == 6) {
                mode = 1;
            }
        }
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&pos);
    shopDrawItemCount(&pos, 0x80808080, 0x47);
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&pos);
    cmpUpdateListSelection(&pos, (u8 *)rcvDrawWork + 0x20, 0, mode, 0x100, 0x47);
}

INCLUDE_RODATA(const s32, "facility/fclRecoverCalc", D_0053D460);

INCLUDE_SDATA(const s32, "facility/fclRecoverCalc", D_0054FBF0);

