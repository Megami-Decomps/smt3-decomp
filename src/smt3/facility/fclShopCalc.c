#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "font.h"

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

typedef struct {
    u8 reserved00[0x1C];
    s32 paymentTotal;
    u8 reserved20[4];
    s8 quantity;
    u8 reserved25[0x11C1];
    s16 itemTop;
    u8 itemOffset;
} shpPaymentWork_t;

typedef struct {
    u8 reserved0000[0xA48];
    s32 money;
    u8 reserved0A4C[0xEDC];
    u8 items[0xA0];
} shpPaymentGlobal_t;
extern shpPaymentWork_t *shopProcWork;
extern shpPaymentGlobal_t *shopMoneyGlobal asm("DDS3_GBWK");
extern u32 shpGetSellPrice(s32 index, s8 kind);
extern u16 shpGetItemPrice(s32 index, s8 kind);

s8 shpCalcPayOfMoney(s32 index, s8 kind)
{
    s32 selected;
    s32 price;
    s32 maxQuantity = -1;
    if (index < 0) {
        selected = shopProcWork->itemTop + shopProcWork->itemOffset;
    } else {
        selected = index;
    }
    price = shpGetSellPrice(selected, kind);
    if (index < 0) shopProcWork->paymentTotal = shopProcWork->quantity * price;
    if (kind == 0 && price > 0) {
        u16 item;
        s32 owned;
        maxQuantity = shopMoneyGlobal->money / price;
        if (maxQuantity >= 100) maxQuantity = 99;
        item = shpGetItemPrice(selected, 0);
        owned = shopMoneyGlobal->items[item];
        if (owned + maxQuantity >= 100) maxQuantity -= owned + maxQuantity - 99;
    }
    return maxQuantity;
}

extern s32 datGetItemSlotIndex(u16 item);

s8 shpChkChoiceMax(s32 index, s8 kind, s8 preserveTotal)
{
    s32 savedTotal = shopProcWork->paymentTotal;
    s32 maxQuantity;
    if (kind == 0) {
        u16 item = shpGetItemPrice(shopProcWork->itemTop + shopProcWork->itemOffset, 0);
        if (datGetItemSlotIndex(item) != 0) {
            maxQuantity = shpCalcPayOfMoney(index, 0);
            if (maxQuantity >= 2) maxQuantity = 1;
        } else {
            maxQuantity = shpCalcPayOfMoney(index, 0);
        }
    } else {
        u16 item;
        shpCalcPayOfMoney(index, 1);
        item = shpGetItemPrice(shopProcWork->itemTop + shopProcWork->itemOffset, 1);
        maxQuantity = shopMoneyGlobal->items[item];
    }
    if (preserveTotal != 0) shopProcWork->paymentTotal = savedTotal;
    return maxQuantity;
}

INCLUDE_ASM(const s32, "facility/fclShopCalc", shop_Process);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern s32 trmDrawListText(fclPanelPos_t *panel, char *text, s32 *tex,
                           fclSpriteTbl_t *table, s32 ot);

typedef struct {
    s32 stride;
    u8 reserved04[0x20];
    u8 selectionOffset;
    u8 reserved25[0x13];
} shopChoiceCursor_t;
typedef struct shopDrawWork_t {
    u8 reserved0000[0x24];
    s8 choiceCount;
    u8 reserved0025[0x1083];
    s32 panelOriginX;
    s32 panelOriginY;
    u8 reserved10B0[0x114];
    shopChoiceCursor_t choiceCursor;
    u8 reserved11FC[0x1198];
    s32 texHandle[2];
} shopDrawWork_t;

extern u8 *shopDrawWork;

extern fclSpriteTbl_t shopSpriteTable[];

extern char *D_0032FA90[];

extern char *D_0032FA94[];

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
                        colors, 0, 1, ((shopDrawWork_t *)shopDrawWork)->texHandle,
                        (fclSpriteTbl_t *)shopSpriteTable, ot);
    fclDrawSpriteFilter(pos->unk_00 + target + 0x10D0, pos->unk_04 + 0x168, pos->unk_08 << 4, 1,
                        colors, 0, 0xA, ((shopDrawWork_t *)shopDrawWork)->texHandle,
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

extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page,
                             s32 spriteId, s32 *textures, fclSpriteTbl_t *table, s32 ot);

void shopDrawRowBox(fclPanelPos_t *panel, s32 mode, s32 ot)
{
    cmpDrawBoxF(0x2E0, 0x308, panel->unk_08, 0x74, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 5,
                     ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    cmpDrawBoxF(0xA60, 0x308, panel->unk_08, 0x58, 9, 0x19191E59, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 6,
                     ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    cmpDrawBoxF(0x13E0, 0x308, panel->unk_08, 0xBD, 9, 0x141E80, ot);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 9,
                     ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    cmpDrawBoxF(0x1020, 0x308, panel->unk_08, 0x2E, 9, 0x19191E59, ot);
    if (mode == 0) {
        fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 7,
                         ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    } else {
        fclDrawSpriteRes(0, 0, panel->unk_08 << 4, NULL, 0, 8,
                         ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    }
}

extern char *pStr[];
extern char D_0054FB60[];
extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 z, u32 color, const char *text, frFontHandle_t *chain);
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 z, u32 color, const char *text, frFontHandle_t *chain);
extern s32 fclDrawListSprite(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 blend, s32 ot);

void shopDrawRowText(s32 x, s32 y, s32 z, u32 color, u16 item, s32 count, s32 ot)
{
    char text[16];
    frFontHandle_t *request;

    request = frwGetSmallFontFRQ2(x + 0x240, y + 0x68, z, color, pStr[item], NULL);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
    sprintf(text, D_0054FB60, count);
    request = frwGetSmallNumFontFRQ(x + 0x9C0, y + 0x80, z, color, text, NULL);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
    fclDrawSpriteRes(x + 0xE50, y + 0x98, z << 4, NULL, 0, 2,
                     ((shopDrawWork_t *)shopDrawWork)->texHandle, shopSpriteTable, ot);
    fclDrawListSprite(x + 0x1F0, y + 0x128, z, 0x104, 2, 0x22242F80, 0, ot);
}


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
    trmDrawListText(&pos, D_0032FA90[0], ((shopDrawWork_t *)shopDrawWork)->texHandle,
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
    trmDrawListText(&pos, D_0032FA94[0], ((shopDrawWork_t *)shopDrawWork)->texHandle,
                    shopSpriteTable, 0x47);
    fclInitPanelPos(0, 0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawItemCount(&pos, 0x80808080, 0x47);
    shopDrawSellRow(0, shopDrawWork + 0x1068, mode, 0x47);
    fclDrawPhaseHeader(shopDrawWork + 0x1068, 0x80808080, 0, 0x47);
}

extern void shopDrawPrice(fclPanelPos_t *pos, s32 count, s8 drawMode, s32 ot);

void shopDrawChoiceCount(s8 mode)
{
    fclPanelPos_t pos;
    s8 drawMode;
    s8 maxChoice;
    s8 choiceCount;
    shopChoiceCursor_t *cursor;
    if (mode == 0) shopDrawPanelMode0(1);
    else shopDrawPanelMode1(1);
    choiceCount = ((shopDrawWork_t *)shopDrawWork)->choiceCount;
    drawMode = choiceCount == 1;
    if (choiceCount == 99) drawMode = 2;
    maxChoice = shpChkChoiceMax(-1, mode, 1);
    if (((shopDrawWork_t *)shopDrawWork)->choiceCount >= maxChoice) {
        if (drawMode == 1) drawMode = 4;
        else drawMode = 2;
    }
    if (drawMode == 0) drawMode = 3;
    cursor = &((shopDrawWork_t *)shopDrawWork)->choiceCursor;
    fclInitPanelPos(((shopDrawWork_t *)shopDrawWork)->panelOriginX,
        ((shopDrawWork_t *)shopDrawWork)->panelOriginY + cursor->selectionOffset * cursor->stride * 8,
        0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawPrice(&pos, ((shopDrawWork_t *)shopDrawWork)->choiceCount, drawMode, 0x47);
}


void shopDrawPricePanel(s8 mode)
{
    fclPanelPos_t pos;
    shopChoiceCursor_t *cursor;
    if (mode == 0) shopDrawPanelMode0(2);
    else shopDrawPanelMode1(2);
    cursor = &((shopDrawWork_t *)shopDrawWork)->choiceCursor;
    fclInitPanelPos(((shopDrawWork_t *)shopDrawWork)->panelOriginX,
        ((shopDrawWork_t *)shopDrawWork)->panelOriginY + cursor->selectionOffset * cursor->stride * 8,
        0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawPrice(&pos, ((shopDrawWork_t *)shopDrawWork)->choiceCount, 4, 0x47);
}


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

