#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern s32 fclSetWorkValue(s8 index, s32 value);

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
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
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
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
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
} cmpCursorWork_t;

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

extern void fclRunPanelAnim(void *work);

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
    /* 0x0001C */ cmpCursorWork_t combineCursor;
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
    /* 0x17640 */ cmpCursorWork_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern s32 datAddItem(s32 id, s32 num);

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ u8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[8];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x14];
    /* 0x002C */ cmpCursorWork_t listCursor;
    /* 0x0064 */ u8 unk_0064[0x2324];
    /* 0x2388 */ void *unk_2388;
    /* 0x238C */ s32 script;
    /* 0x2390 */ s32 scriptArg;
    /* 0x2394 */ s32 texHandle[2];
    /* 0x239C */ u8 unk_239C[0x80];
    /* 0x241C */ fclPanelAnim_t anim;
} shpUpdateWork_t;

extern shpUpdateWork_t *shopUpdateWork;

extern void cmpMoveCursorListEnd(cmpCursorWork_t *cursor);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

u8 shpAddServiceTicket(s32 Nums)
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

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqBuy);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqSell);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqBuyNums);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqSellNums);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqConfirm);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqPay);

void shpServiceTicketItem(void)
{
    shopUpdateWork->unk_0005 = 1;
    shopUpdateWork->unk_0008 =
        fclScriptStart(shopUpdateWork->script, 0xC, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = 0xB;
}

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpServiceTicket);

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

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateServiceTicketItem);

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shpUpdateSeqRoot);

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

INCLUDE_ASM(const s32, "facility/fclShopUpdate", shop_update_Process);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D0C0);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D0E0);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D100);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D118);

INCLUDE_RODATA(const s32, "facility/fclShopUpdate", D_0053D130);

