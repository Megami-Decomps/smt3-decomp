#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern s32 fclSetWorkValue(s8 index, s32 value);

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s32 stat;
    /* 0x10 */ s16 act;
    /* 0x12 */ u16 deact;
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 ctr;
    /* 0x20 */ s32 num;
} fclRibbon_t;

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

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} dds3PartyUnit_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000[0xA54];
    /* 0x00A54 */ dds3PartyUnit_t party[16];
    /* 0x01914 */ s8 stock[0x10];
    /* 0x01924 */ s32 stockCount;
    /* 0x01928 */ u8 unk_01928[0x3D];
    /* 0x01965 */ u8 unk_1965;
    /* 0x01966 */ u8 unk_01966[0x394];
    /* 0x01CFA */ u8 equipHearts;
    /* 0x01CFB */ u8 unk_01CFB[0x40];
    /* 0x01D3B */ u8 devilSlot[0x100];
    /* 0x01E3B */ u8 unk_01E3B[0x10045];
    /* 0x11E80 */ s32 encycNum;
} cmbGlobalWork_t;

/* newdata.h declares the same symbol dds3GlobalWork_t *DDS3_GBWK, and this
 * unit reads it through a different member set at the same offsets. Both are
 * partial views; the alias holds them apart until one covers the object. */
extern cmbGlobalWork_t *cmbGlobalWork asm("DDS3_GBWK");

extern s32 fclRunPanelAnim(void *work);

typedef struct
{
    /* 0x00 */ s8 index;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 id;
} cmbStockSlot_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000;
    /* 0x00001 */ s8 unk_00001;
    /* 0x00002 */ u8 unk_00002;
    /* 0x00003 */ u8 unk_00003;
    /* 0x00004 */ s8 unk_00004;
    /* 0x00005 */ s8 unk_00005;
    /* 0x00006 */ u8 unk_00006[2];
    /* 0x00008 */ s32 unk_00008;
    /* 0x0000C */ u8 unk_0000C[0x10];
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x123EE];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B[5];
    /* 0x16510 */ s32 script;
    /* 0x16514 */ s32 scriptArg;
    /* 0x16518 */ u8 unk_16518[4];
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ u32 *unk_1654C[5];
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x1064];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern void datAddItem(s32 id, s32 num);

typedef struct {
    u8 count;
    u8 reserved01;
    u16 item[0xA0];
} fclItemList_t;

typedef struct
{
    /* 0x0000 */ u8 reserved0000[0x15C];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} shpSellListWindow_t;

typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ s8 delay;
} cmpPadStopWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ s8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[8];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 reserved18;
    /* 0x0019 */ s8 itemType;
    /* 0x001A */ u8 reserved1A[2];
    /* 0x001C */ s32 paymentRemaining;
    /* 0x0020 */ s32 totalPay;
    /* 0x0024 */ s8 quantity;
    /* 0x0025 */ u8 reserved25[7];
    /* 0x002C */ cmpCursorInfo_t listCursor;
    /* 0x0064 */ u8 unk_0064[0x1004];
    /* 0x1068 */ shpSellListWindow_t sellList;
    /* 0x11FC */ u8 reserved11FC[0x1044];
    /* 0x2240 */ u8 itemCount;
    /* 0x2241 */ u8 reserved2241;
    /* 0x2242 */ fclItemList_t sellItems;
    /* 0x2384 */ s32 paymentStep;
    /* 0x2388 */ void *unk_2388;
    /* 0x238C */ s32 script;
    /* 0x2390 */ dds3ProcessID_t *scriptArg;
    /* 0x2394 */ s32 texHandle[2];
    /* 0x239C */ u8 unk_239C[0x80];
    /* 0x241C */ fclPanelAnim_t anim;
} shpUpdateWork_t;

extern shpUpdateWork_t *shopUpdateWork;

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

s32 shpAddServiceTicket(s32 Nums)
{
    datAddItem(0x3D, Nums);
    return cmbGlobalWork->unk_1965;
}

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpGetItemFromItemBox);

void shpExit(void)
{
    cmpMoveCursorListEnd(&shopUpdateWork->listCursor);
    shopUpdateWork->unk_0005 = 1;
    shopUpdateWork->unk_0008 =
        fclScriptStart(shopUpdateWork->script, 0xA, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = 4;
}

typedef struct {
    u8 reserved0000[0x1928];
    u8 items[0xA0];
} shopInventory_t;
extern shopInventory_t *shopInventory asm("DDS3_GBWK");
extern s32 datGetItemSlotIndex(u16 item);
extern s32 cmbChkGetHearts(s32 id);
extern s8 shpChkChoiceMax(s32 index, s8 kind, s8 preserveTotal);
extern s32 cmpGetPressedButton(s32 mask);
extern s32 fclGetFlag(s8 index);
extern u16 shpGetItemPrice(s32 index, s8 kind);
extern void lstMoveCursor2(shpSellListWindow_t *list, s8 direction);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

void shpUpdateSeqBuy(void)
{
    u16 button = (u16)cmpGetPressedButton(0x33);
    if (fclGetFlag(0) == 0) return;
    switch (button) {
    case 1: {
        u16 item = shpGetItemPrice(shopUpdateWork->sellList.listCursor.unk_22 +
            (u8)shopUpdateWork->sellList.listCursor.unk_24, 0);
        s8 maxChoice = shpChkChoiceMax(-1, 0, 0);
        if (maxChoice <= 0) {
            if (shopInventory->items[item] < 99) {
                button = 0x8000;
            } else {
                shopUpdateWork->unk_0005 = button;
                shopUpdateWork->unk_0008 = fclScriptStart(shopUpdateWork->script, 5, &shopUpdateWork->scriptArg);
                shopUpdateWork->unk_0001 = 7;
                shopUpdateWork->unk_0002 = button;
            }
        } else {
            s32 slot = datGetItemSlotIndex(item);
            if (slot != 0 && cmbChkGetHearts(slot) != 0) {
                button = 0x8000;
            } else {
                fclScriptStart(shopUpdateWork->script, 2, &shopUpdateWork->scriptArg);
                shopUpdateWork->unk_0001 = 5;
                shopUpdateWork->unk_0004 = 1;
            }
        }
        break;
    }
    case 2:
        fclScriptStart(shopUpdateWork->script, 8, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = 0;
        shopUpdateWork->unk_0004 = button;
        break;
    case 0x10: lstMoveCursor2(&shopUpdateWork->sellList, -1); break;
    case 0x20: lstMoveCursor2(&shopUpdateWork->sellList, 1); break;
    }
    lstMoveCursor2(&shopUpdateWork->sellList, 0);
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&shopUpdateWork->sellList.listCursor, button, 0x30));
}

extern s32 cmpGetPressedButton(s32 mask);
extern s32 fclGetFlag(s8 index);
extern u16 shpGetItemPrice(s32 index, s8 sell);
extern s8 shpCalcPayOfMoney(s32 index, s8 kind);
extern s32 cmpItemHasFlag(u16 item, s8 which);
extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern void lstMoveCursor2(shpSellListWindow_t *list, s8 direction);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

void shpUpdateSeqSell(void)
{
    u16 button = (u16)cmpGetPressedButton(0x33);
    cmpCursorInfo_t *cursor = &shopUpdateWork->sellList.listCursor;
    s32 selected = cursor->unk_22 + (u8)cursor->unk_24;
    u16 item = shpGetItemPrice(selected, 1);
    s32 sellEnabled = fclGetFlag(0);

    if (sellEnabled == 0) {
        return;
    }

    switch (button) {
    case 1:
        shpCalcPayOfMoney(-1, 1);
        if (cmpItemHasFlag(item, 1) == 0) {
            button = 0x8000;
        } else {
            fclScriptStart(shopUpdateWork->script, 4, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 6;
            shopUpdateWork->unk_0004 = button;
        }
        break;
    case 2:
        fclScriptStart(shopUpdateWork->script, 8, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = 0;
        shopUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        lstMoveCursor2(&shopUpdateWork->sellList, -1);
        break;
    case 0x20:
        lstMoveCursor2(&shopUpdateWork->sellList, 1);
        break;
    }

    lstMoveCursor2(&shopUpdateWork->sellList, 0);
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&shopUpdateWork->sellList.listCursor, button, 0x30));
}

extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern s8 shpCalcPayOfMoney(s32 index, s8 kind);
extern void func_002413B8(u32 button, s8 mode);

void shpUpdateSeqBuyNums(void)
{
    shpUpdateWork_t *work;
    s32 button = cmpGetPressedButton(0x333) & 0xFFFF;

    if (fclGetFlag(0) == 0) {
        return;
    }

    switch (button) {
    case 1:
        fclSetWorkValue(0, shopUpdateWork->paymentRemaining);
        fclSetWorkValue(7, 0);
        fclScriptStart(shopUpdateWork->script, 6, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = 8;
        shopUpdateWork->unk_0004 = button;
        break;
    case 2:
        fclScriptStart(shopUpdateWork->script, 1, &shopUpdateWork->scriptArg);
        shopUpdateWork->quantity = 1;
        shopUpdateWork->unk_0001 = 1;
        shopUpdateWork->unk_0004 = button;
        break;
    case 0x10:
        shopUpdateWork->quantity++;
        break;
    case 0x20:
        shopUpdateWork->quantity--;
        break;
    case 0x100:
        shopUpdateWork->quantity -= 10;
        break;
    case 0x200:
        shopUpdateWork->quantity += 10;
        break;
    }

    work = shopUpdateWork;
    {
        s8 maxQuantity = shpChkChoiceMax(-1, 0, 0);
        if (maxQuantity < work->quantity) {
            shopUpdateWork->quantity = shpChkChoiceMax(-1, 0, 0);
            button = 0;
        }
    }
    if (shopUpdateWork->quantity <= 0) {
        shopUpdateWork->quantity = 1;
        button = 0;
    }

    shpCalcPayOfMoney(-1, 0);
    func_002413B8(button, 3);
}

void shpUpdateSeqSellNums(void)
{
    shpUpdateWork_t *work;
    s32 button = cmpGetPressedButton(0x333) & 0xFFFF;
    if (fclGetFlag(0) == 0) return;
    switch (button) {
    case 1:
        fclSetWorkValue(0, shopUpdateWork->paymentRemaining);
        if (DDS3_GBWK->maka + shopUpdateWork->paymentRemaining <= 0x98967F)
            fclSetWorkValue(7, 0);
        else
            fclSetWorkValue(7, 1);
        fclScriptStart(shopUpdateWork->script, 6, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = 8;
        shopUpdateWork->unk_0004 = 1;
        break;
    case 2:
        fclScriptStart(shopUpdateWork->script, 3, &shopUpdateWork->scriptArg);
        shopUpdateWork->quantity = 1;
        shopUpdateWork->unk_0001 = 2;
        shopUpdateWork->unk_0004 = button;
        break;
    case 0x10: shopUpdateWork->quantity++; break;
    case 0x20: shopUpdateWork->quantity--; break;
    case 0x100: shopUpdateWork->quantity -= 10; break;
    case 0x200: shopUpdateWork->quantity += 10; break;
    }
    work = shopUpdateWork;
    if (shpChkChoiceMax(-1, 1, 0) < work->quantity) {
        shopUpdateWork->quantity = shpChkChoiceMax(-1, 1, 0);
        button = 0;
    }
    if (shopUpdateWork->quantity <= 0) {
        shopUpdateWork->quantity = 1;
        button = 0;
    }
    shpCalcPayOfMoney(-1, 1);
    func_002413B8(button, 3);
}

extern u32 dds3GetProcessStat(dds3ProcessID_t *id);

#ifdef NON_MATCHING
s32 shpUpdateSeqConfirm(s8 mode)
{
    s32 flag;

    if (dds3GetProcessStat(shopUpdateWork->scriptArg) == 2) {
        return 0;
    }
    flag = fclGetFlag(2);
    if (flag == 0) {
        s32 step = shopUpdateWork->paymentRemaining / 20;

        shopUpdateWork->unk_0001 = 9;
        shopUpdateWork->paymentStep = step != 0 ? step : 1;
        shopUpdateWork->unk_0004 = -1;
    } else if (flag == 1) {
        if (mode == 0) {
            fclScriptStart(shopUpdateWork->script, 2, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 5;
        } else {
            fclScriptStart(shopUpdateWork->script, 4, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 6;
        }
        shopUpdateWork->unk_0004 = 2;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqConfirm);
#endif

extern void fclCreateItemListEx(fclItemList_t *list, s8 kind);
extern void cmpChangeCursorListNums(s32 count, u8 *cursor);
extern void datAddMaka(s32 amount);
extern s32 fclAddItem(u16 item, s32 count);
extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

#ifdef NON_MATCHING
void shpUpdateSeqPay(void)
{
    s32 sell = shopUpdateWork->unk_0003 != 5;
    cmpCursorInfo_t *cursor = &shopUpdateWork->sellList.listCursor;
    u16 item = shpGetItemPrice(cursor->unk_22 + (u8)cursor->unk_24, sell);
    s32 sequence = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);

    if (sequence == 1) {
        return;
    }
    if (sequence == 2) {
        shopUpdateWork->quantity = 1;
        if (sell == 0) {
            fclScriptStart(shopUpdateWork->script, 1, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 1;
        } else {
            fclCreateItemListEx(&shopUpdateWork->sellItems, 1);
            cmpChangeCursorListNums(shopUpdateWork->sellItems.count,
                                    (u8 *)&shopUpdateWork->sellList.listCursor);
            if (shopUpdateWork->sellItems.count == 0) {
                fclScriptStart(shopUpdateWork->script, 8, &shopUpdateWork->scriptArg);
                shopUpdateWork->unk_0001 = 0;
            } else {
                fclScriptStart(shopUpdateWork->script, 3, &shopUpdateWork->scriptArg);
                shopUpdateWork->unk_0001 = 2;
            }
        }
        shopUpdateWork->unk_0004 = 1;
    } else if (shopUpdateWork->paymentRemaining > 0) {
        if (shopUpdateWork->paymentRemaining < shopUpdateWork->paymentStep) {
            datAddMaka(sell ? shopUpdateWork->paymentRemaining : -shopUpdateWork->paymentRemaining);
            if (sell == 0) {
                shopUpdateWork->totalPay += shopUpdateWork->paymentRemaining;
            }
            shopUpdateWork->paymentRemaining = 0;
        } else {
            datAddMaka(sell ? shopUpdateWork->paymentStep : -shopUpdateWork->paymentStep);
            if (sell == 0) {
                shopUpdateWork->totalPay += shopUpdateWork->paymentStep;
            }
            shopUpdateWork->paymentRemaining -= shopUpdateWork->paymentStep;
        }
        p2sdPlaySE(5, 0x7F, 0x3F);
    } else {
        fclAddItem(item, sell ? -shopUpdateWork->quantity : shopUpdateWork->quantity);
        shopUpdateWork->unk_0005 = 1;
        shopUpdateWork->unk_0008 = fclScriptStart(shopUpdateWork->script, 7,
                                                 &shopUpdateWork->scriptArg);
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqPay);
#endif

void shpServiceTicketItem(void)
{
    shopUpdateWork->unk_0005 = 1;
    shopUpdateWork->unk_0008 =
        fclScriptStart(shopUpdateWork->script, 0xC, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = 0xB;
}

extern s32 printf(const char *format, ...);
extern void fclClearFlag(s8 index);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern s32 evtBitCheck(s32 eventNo);
extern u16 shpGetItemFromItemBox(s32 slot);
extern char D_0053D080[]; /* "Total pay : %d\n" */

void shpServiceTicket(void)
{
    printf(D_0053D080, shopUpdateWork->totalPay);
    cmpMoveCursorListEnd(&shopUpdateWork->listCursor);
    fclClearFlag(4);
    fclClearFlag(5);

    fclSetWorkValue(4, shpGetItemFromItemBox(0));
    fclSetWorkValue(5, shpGetItemFromItemBox(1));
    fclSetWorkValue(6, shpGetItemFromItemBox(2));

    if (shopUpdateWork->itemType == 1 && evtBitCheck(0x931) == 0) {
        shpExit();
        return;
    }

    if (shopUpdateWork->totalPay >= 0x3E8) {
        shpAddServiceTicket(1);
        shopUpdateWork->unk_0005 = 1;
        shopUpdateWork->unk_0008 = fclScriptStart(shopUpdateWork->script, 0xB,
                                                  &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = 0xA;
    } else if (shpAddServiceTicket(0) >= 0xA) {
        shpServiceTicketItem();
    } else {
        shpExit();
    }
}

void shpUpdateServiceTicket(void)
{
    if (cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork) == 2)
    {
        s32 count = shpAddServiceTicket(0);

        if (count >= 0xA)
        {
            shpServiceTicketItem();
        }
        else
        {
            shpExit();
        }
    }
}

extern s8 fclGetWorkValue(s8 index);
extern s32 datAddHearts(s32 slot);

void shpUpdateServiceTicketItem(void)
{
    s32 info = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);
    s8 selection;
    u16 item;
    s32 slot;

    if (info == 2) {
        selection = 0;
        if (fclGetFlag(4) != 0) {
            selection = 1;
        }
        if (fclGetFlag(5) != 0) {
            selection = 2;
        }
        switch (selection) {
        case 0:
            item = fclGetWorkValue(4);
            break;
        case 1:
            item = fclGetWorkValue(5);
            break;
        default:
            item = fclGetWorkValue(6);
            break;
        }
        slot = datGetItemSlotIndex(item);
        if (slot != 0) {
            datAddHearts(slot);
        } else {
            datAddItem(item, 1);
        }
        shpAddServiceTicket(-10);
        shpExit();
    }
}

extern void fclCreateItemListEx(fclItemList_t *list, s8 kind);
extern void cmpChangeCursorListNums(s32 count, u8 *cursor);
extern void fclShopInitItemList(void *list);
extern void shpFindItemSlot(shpUpdateWork_t *work);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpChkPadPressUpDown(void);

void shpUpdateSeqRoot(void)
{
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    fclSetWorkValue(2, 0);
    if (fclGetFlag(0) == 0) return;
    switch (button) {
    case 1: {
        s32 itemIndex;
        fclShopInitItemList((u8 *)shopUpdateWork + 0x1068);
        shpFindItemSlot(shopUpdateWork);
        itemIndex = shopUpdateWork->listCursor.unk_22 +
                    (u8)shopUpdateWork->listCursor.unk_24 + 1;
        switch (itemIndex) {
        case 1:
            cmpChangeCursorListNums(shopUpdateWork->itemCount,
                                    (u8 *)&shopUpdateWork->sellList.listCursor);
            shopUpdateWork->quantity = 1;
            shpCalcPayOfMoney(-1, 0);
            fclScriptStart(shopUpdateWork->script, 1, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 1;
            shopUpdateWork->unk_0004 = button;
            break;
        case 2:
            fclCreateItemListEx(&shopUpdateWork->sellItems, 1);
            cmpChangeCursorListNums(shopUpdateWork->sellItems.count,
                                    (u8 *)&shopUpdateWork->sellList.listCursor);
            if (shopUpdateWork->sellItems.count == 0) return;
            shopUpdateWork->quantity = 1;
            shpCalcPayOfMoney(-1, 1);
            fclScriptStart(shopUpdateWork->script, 3, &shopUpdateWork->scriptArg);
            shopUpdateWork->unk_0001 = 2;
            shopUpdateWork->unk_0004 = button;
            break;
        case 3: {
            s32 result;
            shpUpdateWork_t *work;
            fclSetWorkValue(2, 1);
            shopUpdateWork->unk_0005 = button;
            result = fclScriptStart(shopUpdateWork->script, 9, &shopUpdateWork->scriptArg);
            work = shopUpdateWork;
            work->unk_0001 = 3;
            work->unk_0008 = result;
            shopUpdateWork->unk_0003 = 3;
            shopUpdateWork->unk_0004 = -1;
            shopUpdateWork->unk_0002 = 0;
            break;
        }
        case 4: shpServiceTicket(); break;
        }
        break;
    }
    case 2: shpServiceTicket(); break;
    case 0x10: cmpMoveCursor(&shopUpdateWork->listCursor, -1); break;
    case 0x20: cmpMoveCursor(&shopUpdateWork->listCursor, 1); break;
    }
    if (cmpChkPadPressUpDown() == 0)
        cmpMoveCursor(&shopUpdateWork->listCursor, 0);
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&shopUpdateWork->listCursor, button, 0x30));
}


void shopCheckSeqInfo(void)
{
    s32 info;

    fclSetWorkValue(2, 0);
    info = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);
    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    fclSetWorkValue(2, 2);
    fclScriptStart(shopUpdateWork->script, 8, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = (u8)shopUpdateWork->unk_0002;
    shopUpdateWork->unk_0004 = -1;
}

void shopStartScript(void)
{
    s32 info = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);

    if (info == 1) {
        return;
    }
    if (info == 2) {
        fclScriptStart(shopUpdateWork->script, 1, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = shopUpdateWork->unk_0002;
        shopUpdateWork->unk_0004 = 1;
    }
}

void shopUpdatePanelAnim(void)
{
    s8 seq = shopUpdateWork->unk_0001;

    switch (seq)
    {
    case 5:
    case 6:
    case 8:
    case 9:
        fclSetPanelAnimState(&shopUpdateWork->anim, 2);
        break;
    default:
        fclSetPanelAnimState(&shopUpdateWork->anim, 1);
        break;
    }
    fclRunPanelAnim(&shopUpdateWork->anim);
}

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 fclChkDbgProcess(void);
extern s32 fclGetFlag(s8 index);
extern void fclClearFlag(s8 index);
extern s32 cmpUpdateSeqInfo(void *work, s8 previous);
extern void shpUpdateSeqRoot(void);
extern void shpUpdateSeqBuy(void);
extern void shpUpdateSeqSell(void);
extern void shpUpdateSeqBuyNums(void);
extern void shpUpdateSeqSellNums(void);
extern s32 shpUpdateSeqConfirm(s8 mode);
extern void shpUpdateSeqPay(void);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D080);

s32 shop_update_Process(dds3ProcessID_t *id)
{
    shpUpdateWork_t *work;
    s8 previous;

    shopUpdateWork = dds3GetProcessData(id);
    previous = shopUpdateWork->unk_0001;
    if (fclChkDbgProcess() != 0) {
        return 0;
    }

    shopUpdatePanelAnim();
    if (fclGetFlag(1) == 0) {
        return 0;
    }

    work = shopUpdateWork;
    if ((work->unk_0000 & 0x40) != 0) {
        return -1;
    }
    if ((work->unk_0000 & 2) != 0 && (work->unk_0000 & 1) != 0) {
        switch (work->unk_0001) {
        case 0:
            shpUpdateSeqRoot();
            break;
        case 1:
            shpUpdateSeqBuy();
            break;
        case 2:
            shpUpdateSeqSell();
            break;
        case 3:
            shopCheckSeqInfo();
            break;
        case 4:
            if (cmpChkSeqInfo((cmbGbWork_t *)work) == 2) {
                fclClearFlag(1);
                fclSetWorkValue(1, 0);
                shopUpdateWork->unk_0000 |= 0x10;
            }
            break;
        case 5:
            shpUpdateSeqBuyNums();
            break;
        case 6:
            shpUpdateSeqSellNums();
            break;
        case 7:
            shopStartScript();
            break;
        case 8: {
            s32 result;

            if (work->unk_0003 == 5) {
                result = shpUpdateSeqConfirm(0);
            } else {
                result = shpUpdateSeqConfirm(1);
            }
            if (result == 0) {
                return 0;
            }
            break;
        }
        case 9:
            shpUpdateSeqPay();
            break;
        case 10:
            shpUpdateServiceTicket();
            break;
        case 11:
            shpUpdateServiceTicketItem();
            break;
        }
    }

    cmpUpdateSeqInfo(shopUpdateWork, previous);
    return 0;
}

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D0C0);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D0E0);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D100);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D118);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D130);

