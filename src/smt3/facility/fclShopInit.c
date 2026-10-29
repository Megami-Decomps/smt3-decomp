#include "common.h"
#include "camp.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern s32 evtBitCheck(s32 no);

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

extern void p2sdStopBGM(s32 id);

extern cmbWork_t *D_0054FB20;

extern void sdfMemFreeDirect(void *aHandle);



extern s8 shpProcessStat;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x19];
    /* 0x0019 */ s8 itemType;
    /* 0x001A */ u8 unk_1A[0x21E6];
    /* 0x2200 */ u16 price[0x22];
    /* 0x2244 */ u16 sellPrice[0x22];
} shopProcWork_t;

extern char D_0053CF70[];

extern shopProcWork_t *shopProcWork;

#include "sdk_libc.h"

extern void sdfTexFree(s32 aHandle);

extern char D_0054FB48[];

extern char D_0053D028[];

extern char D_0053D038[];

extern void fclScriptForcedEnd(u8 *PID);

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
    /* 0x0018 */ u8 pad_0018;
    /* 0x0019 */ s8 itemType;
    /* 0x001A */ u8 pad_001A[0x12];
    /* 0x002C */ cmpCursorInfo_t listCursor;
    /* 0x0064 */ u8 pad_0064[0x219C];
    /* 0x2200 */ u16 itemSlots[32];
    /* 0x2240 */ s8 itemCount;
    /* 0x2241 */ s8 hasMissingHeart;
    /* 0x2242 */ u8 pad_2242[0x146];
    /* 0x2388 */ void *unk_2388;
    /* 0x238C */ s32 script;
    /* 0x2390 */ s32 scriptArg;
    /* 0x2394 */ s32 texHandle[2];
    /* 0x239C */ u8 unk_239C[0x80];
    /* 0x241C */ fclPanelAnimState_t anim;
} shpUpdateWork_t;


extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, s32 layout);

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

/* Called above its definition, where ee-gcc assumed int. */

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

extern char D_0053CF80[];

extern char *D_0032F638[];
extern s32 fclLoadTexture(char *pFileName);
extern void fclSetPanelTex(void *panel, s32 a2, s32 a3, s32 a4, s32 a5, s32 tex);

void fclShopTexLoad(void *workp)
{
    shpUpdateWork_t *work = workp;
    s8 i;

    for (i = 0; (u8)i < 2; i++) {
        work->texHandle[i] = fclLoadTexture(D_0032F638[i]);
    }
    fclSetPanelTex((u8 *)work + 0x23DC, 0, 0, 0x100, 0x80, work->texHandle[1]);
    printf(D_0053CF80);
}

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CF70);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CF80);

void fclShopTexFree(shpUpdateWork_t *pData)
{
    s32 *tex = pData->texHandle;
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
    *(s32 *)((u8 *)work + 0x2388) = (s32)sdfDevLoadFileEx(path, (u32 *)((u8 *)work + 0x238C), 0);
}

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void cmpInitSeqInfo(void *work, void *parent);
extern void fclInitCursorWindow(void *cursor, void *data, u8 count);
extern void fclSetCursorList(void *cursor, s32 mode);
extern void fclInitPanel(void *panel, s32 x, s32 y, s32 z, s32 width,
                         s32 height, s32 color, s8 type, s8 speed);
extern void evtDisableMoonCalc(void);
extern void p2sdPlayBGM(s32 id);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern char D_0032F640[];

cmbWork_t *fclAllocShopWork(s8 type, void *parent)
{
    sdfMemHandle_t *allocation;
    shpUpdateWork_t *work;

    allocation = sdfMemAllocDirect(0x2464);
    work = sdfMemLock(allocation);
    memset(work, 0, 0x2464);
    work->allocation = allocation;
    cmpInitSeqInfo(work, parent);
    work->unk_0001 = 0;
    work->itemType = type;
    work->unk_0004 = 1;
    work->unk_0003 = 0;
    work->scriptArg = 0;
    fclInitCursorWindow((u8 *)work + 0x28, D_0032F640, 4);
    fclSetCursorList((u8 *)work + 0x28, 0x19);
    fclShopInitItemList((u8 *)work + 0x1068);
    fclInitPanel((u8 *)work + 0x239C, 0x13E0, 0x388, 0xFFFFF, 0xBD, 0x62,
                 0x161E2766, 2, 0x10);
    fclInitPanel((u8 *)work + 0x23DC, 0, 0x298, 0xFFFFF, 0x200, 0xE4,
                 0, 0, 0x1D);
    fclShopGetTexPath(work, type);
    evtDisableMoonCalc();
    fclShopTexLoad(work);
    fclShopInitRagTex((u8 *)work);
    p2sdPlayBGM(0x310000);
    fclSetWorkValue(2, 0);
    fclSetWorkValue(3, type);
    return (cmbWork_t *)work;
}

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

extern char D_0053D010[];
extern cmbWork_t *fclAllocShopWork(s8 type, void *parent);
extern s8 FclDebug(s8 mode, void *work);
extern s32 shop_Process(dds3ProcessID_t *id);
extern s32 shop_draw_Process(dds3ProcessID_t *id);
extern s32 shop_update_Process(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority,
    s32 startTimer, s32 shutdownTimer, dds3ProcessFunc_t process,
    dds3ProcessFunc_t shutdown, void *work);

void fclShopProcessStart(s8 *argument)
{
    s8 type = 0;
    if (argument != NULL) type = *argument;
    printf(D_0053D010, type);
    if (D_0054FB20 == NULL) {
        D_0054FB20 = fclAllocShopWork(type, 0);
        FclDebug(3, D_0054FB20);
        dds3StartProcess(D_0054FB48, 0x402, 1, 1, shop_Process, NULL, D_0054FB20);
        dds3StartProcess(D_0053D028, 0x2B12, 1, 1, shop_draw_Process, NULL, D_0054FB20);
        dds3StartProcess(D_0053D038, 0x520E, 1, 1, shop_update_Process,
                        (dds3ProcessFunc_t)shop_update_Shutdown, D_0054FB20);
        shpProcessStat = 1;
    }
}

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

s32 shpCountUnlockedItems(s32 itemType)
{
    s32 count = 0;
    u16 *row = &D_0032F682[(u8)shpItemIsUnlocked(itemType) * 65];
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

extern s32 datGetItemSlotIndex(u16 id);
extern s32 cmbChkGetHearts(s32 id);
extern s32 fclSetWorkValue(s8 index, s32 value);

void shpFindItemSlot(shpUpdateWork_t *work)
{
    u16 *row = &D_0032F682[(u8)shpItemIsUnlocked(work->itemType) * 65];
    s32 i;

    work->itemCount = 0;
    work->hasMissingHeart = 0;
    for (i = 0; i < shpCountUnlockedItems(work->itemType); i++) {
        u16 item = *row;
        s32 slot;

        row += 2;
        slot = datGetItemSlotIndex(item);
        if (slot != 0) {
            if (cmbChkGetHearts((s8)slot) != 0) {
                continue;
            }
            work->hasMissingHeart = 1;
        }
        work->itemSlots[work->itemCount++] = item;
    }
    fclSetWorkValue(4, work->hasMissingHeart);
}

typedef struct
{
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u32 price;
} shpItemTableEntry_t;

extern shpItemTableEntry_t datItemTable[];

typedef struct
{
    /* 0x00 */ u16 base;
    /* 0x02 */ struct
    {
        u16 id;
        u16 price;
    } items[32];
} shpPriceRow_t;

extern shpPriceRow_t D_0032F680[];

s32 shpGetSellPrice(s32 index, s8 sell)
{
    u16 item = shpGetItemPrice(index, sell);
    s32 ret;

    if (sell == 0) {
        s32 row = (u8)shpItemIsUnlocked(shopProcWork->itemType);
        u16 fallback = D_0032F680[row].base;
        u32 price = D_0032F680[row].items[index].price;
        u32 base = datItemTable[item].price;
        s32 div = 100;

        if (price == 0) {
            price = fallback;
        }
        ret = base * price / div;
        if (evtBitCheck(0xAF2) == 0) {
            ret *= 3;
        }
    } else {
        ret = datItemTable[item].price >> 1;
    }
    return ret;
}

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CFD0);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053CFF0);

INCLUDE_RODATA(const s32, "facility/fclShopInit", D_0053D010);

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

