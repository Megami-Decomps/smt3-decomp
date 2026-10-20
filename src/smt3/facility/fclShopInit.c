#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void dds3KillProcessName(char *namestr, s32 flag);

void fclFreeStage(void);

extern s32 evtBitCheck(s32 bit);

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

extern void p2sdStopBGM(s32 id);

extern cmbWork_t *D_0054FB20;

extern void sdfMemFreeDirect(void *ptr);

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

extern s8 shpProcessStat;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x2200];
    /* 0x2200 */ u16 price[0x22];
    /* 0x2244 */ u16 sellPrice[0x22];
} shopProcWork_t;

extern char D_0053CF70[];

extern shopProcWork_t *shopProcWork;

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 handle);

extern char D_0054FB48[];

extern char D_0053D028[];

extern char D_0053D038[];

extern void fclScriptForcedEnd(u8 *script);

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

typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ void (*calcSizes)();
} fclRagTex_t;

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

extern void cmpInitCursorRange(void *cursor, u8 index, u8 rows, s32 flag);

extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 e);

/* Called above its definition, where ee-gcc assumed int. */
void *sdfDevLoadFileEx(char *name, void **data, s32 *size);

void fclShopEventEnd(void)
{
    dds3KillProcessName(D_0053CF70, 1);
    fclFreeStage();
}

s32 func_00206BA8(void)
{
    if (D_0054FB20 == NULL) {
        return 0;
    }
    return D_0054FB20->unk_1C;
}

void fclShopInitItemList(void *list)
{
    lstInitListWindow(0xC0, 0x388, 0xFFFFF, 0x12B, 0xB8, 0x19191E59, 0, list);
    cmpInitCursorLayout((u8 *)list + 0x15C, 0x21, 0x10, 0xFF, 0x16, 0x1A);
    cmpInitCursorRange((u8 *)list + 0x15C, 6, 6, 0);
}

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CF70);

INCLUDE_ASM(const s32, "facility/fclShopInit", fclShopTexLoad);

void fclShopTexFree(shpUpdateWork_t *work)
{
    s32 *tex = work->texHandle;
    s8 i;

    for (i = 0; (u8)i < 2; i++) {
        sdfTexFree(*tex++);
    }
    printf("SHOP : Tex free.\n");
}

void fclShopInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x241C);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

extern char D_0053CFD0[]; /* "/facility/scr/shop/mes_f.bf" */

extern char D_0053CFF0[]; /* "/facility/scr/shop/mes_g.bf" */

void fclShopGetTexPath(void *work, s32 mode)
{
    char *path;

    if (mode != 1) {
        path = D_0053CFF0;
        if (mode != 5) {
            path = "/facility/scr/shop/message.bf";
        }
    } else if (evtBitCheck(0x931) == 0) {
        path = D_0053CFD0;
    } else {
        path = "/facility/scr/shop/message.bf";
    }
    *(s32 *)((u8 *)work + 0x2388) = (s32)sdfDevLoadFileEx(path, (u8 *)work + 0x238C, 0);
}

INCLUDE_ASM(const s32, "facility/fclShopInit", fclAllocShopWork);

void shop_update_Shutdown(void)
{
    if (D_0054FB20 == NULL) {
        return;
    }
    fclShopTexFree((shpUpdateWork_t *)D_0054FB20);
    fclScriptForcedEnd((u8 *)((shpUpdateWork_t *)D_0054FB20)->scriptArg);
    sdfMemFreeDirect(((shpUpdateWork_t *)D_0054FB20)->unk_2388);
    fclShopEventEnd();
    sdfMemFreeDirect(((shpUpdateWork_t *)D_0054FB20)->allocation);
    D_0054FB20 = NULL;
    shpProcessStat = 2;
    evtDisableMoonDraw();
    p2sdStopBGM(0x310000);
}

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CFD0);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CFF0);

INCLUDE_ASM(const s32, "facility/fclShopInit", fclShopProcessStart);

void fclShopProcessEnd(void)
{
    dds3KillProcessName(D_0054FB48, 1);
    dds3KillProcessName(D_0053D028, 1);
    dds3KillProcessName(D_0053D038, 1);
}

s32 shpCheckProcessStat(void)
{
    s8 stat = shpProcessStat;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            shpProcessStat = 0;
        }
    }
    return 0;
}

void func_00207178(void)
{
    if (D_0054FB20 != NULL) {
        D_0054FB20->flags |= 0x10;
    }
}

s32 func_00207198(s32 value, s8 align)
{
    s8 rem = value % align;

    if (rem != 0) {
        if (align < 0) {
            value -= rem;
        } else {
            value += align - rem;
        }
    } else {
        value += align;
    }
    return value;
}

u16 shpGetItemPrice(s32 index, s8 sell)
{
    u16 price;

    if (sell == 0) {
        price = shopProcWork->price[index];
    } else {
        price = shopProcWork->sellPrice[index];
    }
    return price;
}

typedef struct
{
    /* 0x0 */ u16 unk_00;
    /* 0x2 */ s16 flag;
    /* 0x4 */ u8 state[2];
} shpItemEntry_t;

extern shpItemEntry_t D_0032F650[];

s32 shpItemIsUnlocked(id)
s32 id;
{
    s32 on;

    if (D_0032F650[id].flag >= 0) {
        on = evtBitCheck(D_0032F650[id].flag) != 0;
    } else {
        on = 0;
    }
    return D_0032F650[id].state[on];
}

extern u16 D_0032F682[];

s32 shpCountUnlockedItems(void)
{
    s32 count = 0;
    u16 *row = &D_0032F682[(u8)shpItemIsUnlocked() * 65];
    s32 i;

    for (i = 0; i < 32; i++)
    {
        if (*row != 0)
        {
            count++;
        }
        row += 2;
    }
    return count;
}

INCLUDE_ASM(const s32, "facility/fclShopInit", shpFindItemSlot);

INCLUDE_ASM(const s32, "facility/fclShopInit", shpGetSellPrice);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053D028);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053D038);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB20);

INCLUDE_SDATA(const s32, "facility/fclShopInit", shpProcessStat);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB25);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB28);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB30);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB38);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB40);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB48);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB50);

INCLUDE_SDATA(const s32, "facility/fclShopInit", D_0054FB58);

