#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

s32 fclGetFlag(s8 index);

extern s32 shopDrawPanelMode0(s8 mode);

extern s32 shopUpdateItemList(s32 mode);

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

extern char D_0053D028[];

extern char D_0053D038[];

extern void frDestroyFontRequestAfter(void *pFRQ);

extern s32 sprintf(char *out, const char *fmt, ...);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shpCalcPayOfMoney);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shpChkChoiceMax);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shop_Process);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void trmDrawListText(fclPanelPos_t *panel, s32 msg, s32 *tex, char *str,
                            s32 arg);

extern u8 *shopDrawWork;

extern char shopSpriteTable[];

extern s32 D_0032FA90[];

extern s32 D_0032FA94[];

extern void shopDrawItemCount(fclPanelPos_t *pos, s32 color, s32 id);

extern void shopDrawBuyRow(s32 row, u8 *work, s8 mode, s32 id);

extern void shopDrawSellRow(s32 row, u8 *work, s8 mode, s32 id);

extern void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 id);

extern s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim);

extern s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim);

extern void fclDrawSpriteFilter(s32 x, s32 y, s32 d, u8 filter, s32 *color, s32 id, s32 idx,
                                s32 *texs, fclSpriteTbl_t *table, s32 OtNo);

extern void *frwGetNumFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, char *pStr, s32 pFRQ);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

extern fclSpriteTbl_t fclSpriteTable[];

extern s32 D_00345D88[];

extern char D_0054FB60[];

void shopDrawPanelRow(fclPanelPos_t *pos, s32 color, s32 num, s32 ot)
{
    s32 colors[4];
    char buf[16];
    void *frq;
    s32 target;
    s32 alpha;

    target = fclGetPanelAnimTarget((fclPanelAnim_t *)(shopDrawWork + 0x241C));
    alpha = (0x200 - fclGetPanelAnimPhase((fclPanelAnim_t *)(shopDrawWork + 0x241C))) / 4;
    color = (color & ~0xFF) | (alpha & 0xFF);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteFilter(pos->unk_00 + target + 0x10D0, pos->unk_04 + 0x168, pos->unk_08 << 4, 1,
                        colors, 0, 1, (s32 *)(shopDrawWork + 0x2394),
                        (fclSpriteTbl_t *)shopSpriteTable, ot);
    fclDrawSpriteFilter(pos->unk_00 + target + 0x10D0, pos->unk_04 + 0x168, pos->unk_08 << 4, 1,
                        colors, 0, 0xA, (s32 *)(shopDrawWork + 0x2394),
                        (fclSpriteTbl_t *)shopSpriteTable, ot);
    fclDrawSpriteFilter(pos->unk_00 + target + 0x1E40, pos->unk_04 + 0x1A0, pos->unk_08 << 4, 1,
                        colors, 0xA, 1, D_00345D88, fclSpriteTable, ot);
    color = (color & 0xFF) | 0xA09DC300;
    sprintf(buf, D_0054FB60, num);
    frq = frwGetNumFontFRQ(pos->unk_00 + target + 0x1840, pos->unk_04 + 0x1A0, pos->unk_08, color,
                           buf, 0);
    func_001C7E38(frq, 1, ot);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawPrice);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawRowBox);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawRowText);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawBuyRow);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawSellRow);

extern s32 cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 a,
                                  s32 mode, s32 b, s32 id);

s32 shopUpdateItemList(s32 active)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (active != 0) {
        mode = 0;
        if (fclGetFlag(0) == 0) {
            mode = 2;
            if (*(s8 *)(shopDrawWork + 3) == 3) {
                mode = 1;
            }
        }
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&pos);
    shopDrawItemCount(&pos, 0x80808080, 0x47);
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&pos);
    cmpUpdateListSelection(&pos, shopDrawWork + 0x2c, 0, mode, 0x100, 0x47);
}

s32 shopDrawPanelMode0(s8 mode)
{
    fclPanelPos_t pos;

    fclDrawPanel((fclPanel_t *)(shopDrawWork + 0x23DC), 0x47);
    fclUpdatePanel((fclPanel_t *)(shopDrawWork + 0x23DC));
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    trmDrawListText(&pos, D_0032FA90[0], (s32 *)(shopDrawWork + 0x2394),
                    shopSpriteTable, 0x47);
    fclInitPanelPos(0, 0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawItemCount(&pos, 0x80808080, 0x47);
    shopDrawBuyRow(0, shopDrawWork + 0x1068, mode, 0x47);
    fclDrawPhaseHeader(shopDrawWork + 0x1068, 0x80808080, 0, 0x47);
}

s32 shopDrawPanelMode1(s8 mode)
{
    fclPanelPos_t pos;

    fclDrawPanel((fclPanel_t *)(shopDrawWork + 0x23DC), 0x47);
    fclUpdatePanel((fclPanel_t *)(shopDrawWork + 0x23DC));
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    trmDrawListText(&pos, D_0032FA94[0], (s32 *)(shopDrawWork + 0x2394),
                    shopSpriteTable, 0x47);
    fclInitPanelPos(0, 0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawItemCount(&pos, 0x80808080, 0x47);
    shopDrawSellRow(0, shopDrawWork + 0x1068, mode, 0x47);
    fclDrawPhaseHeader(shopDrawWork + 0x1068, 0x80808080, 0, 0x47);
}

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawChoiceCount);

INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawPricePanel);

s32 shopDrawPhase7(void)
{
    return shopDrawPanelMode0(1);
}

s32 shopUpdateItemListDefault(void)
{
    return shopUpdateItemList(0);
}

INCLUDE_ASM(const s32, "facility/fclShopCalc", shop_draw_Process);
INCLUDE_SDATA(const s32, "facility/fclShopCalc", D_0054FB60);

INCLUDE_SDATA(const s32, "facility/fclShopCalc", D_0054FB68);

