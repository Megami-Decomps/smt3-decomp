#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "font.h"
#include "kernel.h"

s32 fclGetFlag(s8 index);

extern s32 shopDrawPanelMode0(s8 mode);

extern s32 shopUpdateItemList(s32 mode);



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

extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);

extern s32 sprintf(char *out, const char *fmt, ...);

typedef struct {
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ s8 state;
    /* 0x0002 */ u8 reserved0002[2];
    /* 0x0004 */ s8 step;
    /* 0x0005 */ u8 reserved0005[0x13];
    /* 0x0018 */ s8 started;
    /* 0x0019 */ u8 reserved0019[3];
    /* 0x001C */ s32 paymentTotal;
    /* 0x0020 */ u8 reserved0020[4];
    /* 0x0024 */ s8 quantity;
    /* 0x0025 */ u8 reserved0025[0x11C1];
    /* 0x11E6 */ s16 itemTop;
    /* 0x11E8 */ u8 itemOffset;
    /* 0x11E9 */ u8 reserved11E9[0x11A3];
    /* 0x238C */ s32 script;
    /* 0x2390 */ s32 scriptProc;
    /* 0x2394 */ u8 reserved2394[0x60];
    /* 0x23F4 */ s32 unk_23F4;
} shpPaymentWork_t;

typedef struct {
    u8 reserved0000[0xA48];
    s32 money;
    u8 reserved0A4C[0xEDC];
    u8 items[0xA0];
} shpPaymentGlobal_t;
extern shpPaymentWork_t *shopProcWork;
extern shpPaymentGlobal_t *shopMoneyGlobal asm("DDS3_GBWK");
extern s32 shpGetSellPrice(s32 index, s8 kind);
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

extern s32 fclChkDbgProcess(void);
extern s32 shopStartEvent(s8 mode);
extern s32 dds3GetProcessStat(void *id);
extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);
extern void fclClearFlag(s8 index);
extern void *dds3GetProcessData(dds3ProcessID_t *id);

#ifdef NON_MATCHING
/* NON_MATCHING: 37 of 64 words match at the exact 256-byte size. */
extern void cmpUpdateTerminate(void *work);

s32 shop_Process(dds3ProcessID_t *id)
{
    shopProcWork = dds3GetProcessData(id);
    if (fclChkDbgProcess() != 0) {
        return 0;
    }
    if (shopProcWork->started == 0) {
        shopStartEvent(1);
        shopProcWork->started = 1;
    }
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    switch (shopProcWork->state) {
    case 0:
        shopProcWork->unk_23F4 = 0;
        if (shopProcWork->step == 1) {
            if (dds3GetProcessStat((void *)shopProcWork->scriptProc) != 2) {
                shopProcWork->scriptProc =
                    fclScriptStart(shopProcWork->script, 0, NULL);
            }
        }
        break;
    case 8:
        if (shopProcWork->step == 1) {
            fclClearFlag(2);
        }
        break;
    }
    shopProcWork->step = 0;
    if ((shopProcWork->flags & 0x10) == 0) {
        return 0;
    }
    cmpUpdateTerminate(shopProcWork);
    return -1;
}
#else
INCLUDE_ASM(const s32, "facility/fclShopCalc", shop_Process);
#endif


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
    u8 flags;
    s8 phase;
    u8 reserved02;
    s8 previousPhase;
    u8 reserved04[0x18];
    s32 paymentRemaining;
    u8 reserved20[4];
    s8 choiceCount;
    u8 reserved0025[0x1083];
    s32 panelOriginX;
    s32 panelOriginY;
    u8 reserved10B0[0x114];
    shopChoiceCursor_t choiceCursor;
    u8 reserved11FC[0x1198];
    s32 texHandle[2];
    fclPanel_t pricePanel;
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

extern frFontHandle_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color, const char *text, frFontHandle_t *previous);

extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);

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

extern char D_0054FB68[];

#ifdef NON_MATCHING
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 z, u32 color, const char *text, frFontHandle_t *chain);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page, s32 spriteId, s32 *textures, fclSpriteTbl_t *table, s32 ot);
#endif

void shopDrawPrice(fclPanelPos_t *pos, s32 count, s8 drawMode, s32 ot)
{
    char text[16];
    frFontHandle_t *request;

    if (count == 0) {
        return;
    }

    sprintf(text, D_0054FB68, count);
    if (drawMode == 0) {
        request = frwGetSmallNumFontFRQ(pos->unk_00 + 0x10A0, pos->unk_04 + 0xA0,
                                        pos->unk_08, 0x89FEFF80, text, NULL);
    } else {
        request = frwGetNumFontFRQ(pos->unk_00 + 0x1070, pos->unk_04 + 0x70,
                                   pos->unk_08, 0x89FEFF80, text, NULL);

        if (drawMode == 1 || drawMode == 3) {
            fclDrawSpriteRes(pos->unk_00 + 0x1140, pos->unk_04 + 0x10,
                             pos->unk_08 << 4, NULL, 0, 0xB,
                             ((shopDrawWork_t *)shopDrawWork)->texHandle,
                             shopSpriteTable, ot);
        }
        if (drawMode == 2 || drawMode == 3) {
            fclDrawSpriteRes(pos->unk_00 + 0x1140, pos->unk_04 + 0x140,
                             pos->unk_08 << 4, NULL, 0, 0xC,
                             ((shopDrawWork_t *)shopDrawWork)->texHandle,
                             shopSpriteTable, ot);
        }
        if (drawMode != 4) {
            fclDrawSpriteRes(pos->unk_00 + 0x1140, pos->unk_04 + 0x130,
                             pos->unk_08 << 4, NULL, 0, 0xD,
                             ((shopDrawWork_t *)shopDrawWork)->texHandle,
                             shopSpriteTable, ot);
        }
    }

    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
    fclDrawSpriteRes(pos->unk_00 + 0xF90, pos->unk_04 + 0xB0,
                     pos->unk_08 << 4, NULL, 0, 4,
                     ((shopDrawWork_t *)shopDrawWork)->texHandle,
                     shopSpriteTable, ot);
}

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


typedef struct
{
    s32 rowHeight;
    u8 reserved04[0x1E];
    s16 selectedTop;
    u8 selectedOffset;
    u8 reserved25;
    u8 rowCount;
} shopDrawCursor_t;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    s16 width;
    s16 height;
    s32 color;
} shopDrawListPanel_t;

typedef struct
{
    u8 reserved00[0x40];
    shopDrawListPanel_t panel;
    u8 reserved54[0x108];
    shopDrawCursor_t cursor;
} shopDrawListWork_t;

extern void lstDrawListWindow(void *work, s8 noCursor, s32 ot);
extern void lstDrawListWindowCursor(void *work, s32 ot);
extern u32 cmpDrawListItemBox(void *cursor, s32 row, s32 slot, s32 mode, s32 height);
extern s32 cmpItemHasFlag(u16 item, s8 which);
extern s32 fclDrawPanel(fclPanel_t *panel, s32 ot);
extern void fclDrawPriceText(fclPanelPos_t *panel, u16 item, s32 ot);


extern s32 cmbChkGetHearts(s32 rawId);

#ifdef NON_MATCHING
void shopDrawBuyRow(s32 row, u8 *work, s8 mode, s32 ot)
{
    shopDrawListWork_t *list = (shopDrawListWork_t *)work;
    shopDrawCursor_t *cursor = &list->cursor;
    fclPanelPos_t pos;
    s32 x = list->panel.x;
    s32 y = list->panel.y;
    s32 z = list->panel.z;
    s32 i;

    lstDrawListWindow(work, mode, ot);
    for (i = 0; i < cursor->rowCount; i++) {
        u16 item = shpGetItemPrice(cursor->selectedTop + i, 0);
        s32 price;
        s32 disabled;
        s32 slot;
        u32 color;

        if (item == 0) {
            break;
        }
        price = shpGetSellPrice(cursor->selectedTop + i, 0);
        disabled = DDS3_GBWK->maka < shpGetSellPrice(cursor->selectedTop + i, 0);
        slot = datGetItemSlotIndex(item);
        if (slot != 0 && cmbChkGetHearts(slot) != 0) {
            disabled = 1;
        }
        if (mode != 0) {
            disabled |= 2;
        }
        color = cmpDrawListItemBox(cursor, i, i, disabled, 0x100);
        shopDrawRowText(x, y, z, color, item, price, ot);
        y += cursor->rowHeight * 8;
    }
    shopDrawRowBox((fclPanelPos_t *)&list->panel, 0, ot);
    fclDrawPanel(&((shopDrawWork_t *)shopDrawWork)->pricePanel, 0x47);
    fclInitPanelPos(0x1470, 0x3B0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    fclDrawPriceText(&pos, shpGetItemPrice(cursor->selectedTop + cursor->selectedOffset, 0), 0x47);
    fclDrawPhaseHeader((u8 *)&((shopDrawWork_t *)shopDrawWork)->pricePanel, 0x14132D33, 1, 0x47);
    if (mode == 0) {
        lstDrawListWindowCursor(work, ot);
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclShopCalc", shopDrawBuyRow);
#endif


void shopDrawSellRow(s32 row, u8 *work, s8 mode, s32 id)
{
    shopDrawListWork_t *list = (shopDrawListWork_t *)work;
    shopDrawCursor_t *cursor = &list->cursor;
    s32 x = list->panel.x;
    s32 y = list->panel.y;
    s32 z = list->panel.z;
    s32 i;

    lstDrawListWindow(work, mode, id);
    for (i = 0; i < cursor->rowCount; i++) {
        u16 item = shpGetItemPrice(cursor->selectedTop + i, 1);
        u32 price = shpGetSellPrice(cursor->selectedTop + i, 1);
        s32 boxMode = cmpItemHasFlag(item, 1) == 0;
        s32 color;

        if (mode != 0) boxMode |= 2;
        color = cmpDrawListItemBox(cursor, i, i, boxMode, 0x100);
        shopDrawRowText(x, y, z, color, item, price, id);
        y += cursor->rowHeight * 8;
    }

    shopDrawRowBox((fclPanelPos_t *)&list->panel, 1, id);
    fclDrawPanel((fclPanel_t *)(shopDrawWork + 0x239C), 0x47);
    {
        fclPanelPos_t panel;
        u16 item;

        fclInitPanelPos(0x1470, 0x3B0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&panel);
        item = shpGetItemPrice(cursor->selectedTop + cursor->selectedOffset, 1);
        fclDrawPriceText(&panel, item, 0x47);
    }
    fclDrawPhaseHeader(shopDrawWork + 0x239C, 0x1121D33, 1, 0x47);
    if (mode == 0) lstDrawListWindowCursor(work, id);
}


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

#ifdef NON_MATCHING
extern void fclUpdatePanel(fclPanel_t *pPanel);
#endif

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

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 fclChkDbgProcess(void);
extern void cmpUpdateTerminate(void *work);

s32 shop_draw_Process(dds3ProcessID_t *id)
{
    fclPanelPos_t pos;
    shopDrawWork = dds3GetProcessData(id);
    if (fclChkDbgProcess() != 0) return 0;
    if (fclGetFlag(1) == 0) return 0;
    if ((((shopDrawWork_t *)shopDrawWork)->flags & 0x20) != 0) {
        cmpUpdateTerminate(shopDrawWork);
        return -1;
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    shopDrawPanelRow(&pos, 0x80808080, ((shopDrawWork_t *)shopDrawWork)->paymentRemaining, 0x47);
    if ((((shopDrawWork_t *)shopDrawWork)->flags & 2) != 0) {
        switch (((shopDrawWork_t *)shopDrawWork)->phase) {
        case 0: shopUpdateItemList(1); break;
        case 1:
            if (fclGetFlag(0) != 0) shopDrawPanelMode0(0);
            else shopDrawPanelMode0(2);
            break;
        case 2:
            if (fclGetFlag(0) != 0) shopDrawPanelMode1(0);
            else shopDrawPanelMode1(2);
            break;
        case 3:
        case 4: shopUpdateItemList(0); break;
        case 5:
            if (fclGetFlag(0) == 0) shopDrawPanelMode0(2);
            else shopDrawChoiceCount(0);
            break;
        case 6:
            if (fclGetFlag(0) == 0) shopDrawPanelMode1(2);
            else shopDrawChoiceCount(1);
            break;
        case 7: shopDrawPhase7(); break;
        case 8:
        case 9:
            if (((shopDrawWork_t *)shopDrawWork)->previousPhase == 5) shopDrawPricePanel(0);
            else shopDrawPricePanel(1);
            break;
        case 10:
        case 11: shopUpdateItemListDefault(); break;
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "facility/fclShopCalc", D_0054FB60);

INCLUDE_SDATA(const s32, "facility/fclShopCalc", D_0054FB68);

