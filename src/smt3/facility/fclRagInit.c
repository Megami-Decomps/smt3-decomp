#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "fclRag.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern u32 dds3GetRandA(void *state);

extern s32 evtStartEvent(s32 no);

extern s32 fclSetWorkValue(s8 index, s32 value);

void fclClearFlag(s8 index);

extern void p2sdStopBGM(s32 id);

extern void sdfMemFreeDirect(void *aHandle);

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0xF];
    /* 0x14 */ void *allocation;
    /* 0x18 */ u16 slot[10];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 unk_31;
    /* 0x32 */ u8 unk_32[7];
    /* 0x39 */ s8 slotCursor;
} campStatusPanel_t;

extern void qsort(void *base, s32 count, s32 size, s32 (*cmp)(const void *a, const void *b));

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

extern s8 gProcessStat_1;

extern char D_0053D140[];

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

extern u8 *GBWK_0;

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 aHandle);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern char D_0054FB98[];

extern char D_0053D1F0[];

extern char D_0053D200[];

extern void fclScriptForcedEnd(u8 *PID);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

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

extern u8 ragHeartsTable[];

extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, s32 layout);

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

/* Called above its definition, where ee-gcc assumed int. */
void *dds3GetActiveScene(void);

void fclRagEventStart(s8 Signal)
{
    fclSetWorkValue(1, Signal);
    fclClearFlag(1);
    evtStartEvent(0x323);
    p2sdPlaySE(0x390001, 0x7F, 0x3F);
}

void fclRagEventEnd(void)
{
    dds3KillProcessName(D_0053D140, 1);
    fclFreeStage();
}

s32 func_00209D88(u16 id)
{
    u16 *entry = (u16 *)(ragHeartsTable + 0x160);
    u32 i;

    for (i = 0; i < 0x15; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0x2C);
    }
    return 0;
}

s32 func_00209DD0(u16 id)
{
    u16 *entry = (u16 *)(ragHeartsTable + 0x500);
    u32 i;

    for (i = 0; i < 0x15; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0x2C);
    }
    return 0;
}

s32 func_00209E18(u16 id)
{
    u16 *entry = (u16 *)ragHeartsTable;
    u32 i;

    for (i = 0; i < 0x19; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0xE);
    }
    return 0;
}

extern u8 D_0032FC98[];

extern u8 D_0032FCE0[];

/* NON_MATCHING: 22 of 51 words differ (first at +0x8). */
#ifdef NON_MATCHING
u8 ragGetListItem2(u8 index, s8 kind)
{
    u32 limit = D_0032FC98[index * 35 + 6] + 1;
    s32 pick = (u32)dds3GetRandA(0) % limit;
    u8 *weight = &D_0032FC98[index * 35];
    u8 *item = &D_0032FC98[kind * 7] + index * 35 + 7;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (*weight >= pick) {
            return *item;
        }
        weight++;
        item++;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "facility/fclRagInit", ragGetListItem2);
#endif

/* NON_MATCHING: 22 of 51 words differ (first at +0x8). */
#ifdef NON_MATCHING
u8 ragGetListItem3(u8 index, s8 kind)
{
    u32 limit = D_0032FCE0[index * 35 + 6] + 1;
    s32 pick = (u32)dds3GetRandA(0) % limit;
    u8 *weight = &D_0032FCE0[index * 35];
    u8 *item = &D_0032FCE0[kind * 7] + index * 35 + 7;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (*weight >= pick) {
            return *item;
        }
        weight++;
        item++;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "facility/fclRagInit", ragGetListItem3);
#endif

void ragInitHeartsList(s32 count, lstWork_t *work)
{
    lstInitListWindow(0x250, 0x280, 0xFFFFF, 0xBF, 0xDD, 0x19191E59, 0, work);
    cmpInitCursorLayout(&work->listCursor, 0x23, 0x16, 0x8F, 0x16, 0x1A);
    cmpInitCursorRange(&work->listCursor, count, 7, 0);
}

void ragInitList2(s32 count, lstWork_t *work)
{
    lstInitListWindow(0x250, 0x280, 0xFFFFF, 0xBF, 0xDD, 0x19191E59, 0, work);
    cmpInitCursorLayout(&work->listCursor, 0x23, 0x16, 0x8F, 0x16, 0x1A);
    cmpInitCursorRange(&work->listCursor, count, 7, 0);
}

void ragInitList3(s32 count, lstWork_t *work)
{
    lstInitListWindow(0x250, 0x280, 0xFFFFF, 0xBF, 0xDD, 0x19191E59, 0, work);
    cmpInitCursorLayout(&work->listCursor, 0x23, 0x16, 0x8F, 0x16, 0x1A);
    cmpInitCursorRange(&work->listCursor, count, 7, 0);
}

typedef struct {
    u8 reserved00[0x1A];
    s8 heartItems[32];
    s8 heartCount;
    s8 list2[8];
    s8 list2Count;
    s8 list3[8];
    s8 list3Count;
    s8 hasUnregistered;
    u8 reserved4E[0xF2];
    cmpCursorInfo_t cursor;
    u8 reserved178[0x1004];
    lstWork_t heartWindow;
    u8 reserved1310[0x1004];
    lstWork_t window2;
    u8 reserved24A8[0x1004];
    lstWork_t window3;
} ragListWork_t;

typedef struct {
    u16 item;
    u8 reserved02[12];
} ragHeartEntry_t;

extern ragHeartEntry_t ragHeartEntries[] asm("ragHeartsTable");
extern u8 D_0032FC58[][32];
extern s32 datGetItemSlotIndex(u16 item);
extern s32 cmbChkGetHearts(s32 id);
extern s32 cmpItemOrder(u8 *a, u8 *b);

#ifdef NON_MATCHING
s32 ragBuildHeartsList(u8 type, ragListWork_t *work)
{
    s8 count = 0;
    s32 i;

    work->hasUnregistered = 0;
    for (i = 0; i < 32; i++) {
        u8 index = D_0032FC58[type][i];
        u16 item = ragHeartEntries[index].item;
        s32 slot = datGetItemSlotIndex(item);

        if (slot != 0) {
            if (cmbChkGetHearts((s8)slot) != 0) {
                continue;
            }
            work->hasUnregistered = 1;
        }
        if (item != 0) {
            work->heartItems[count++] = index;
        }
    }
    qsort(work->heartItems, count, 1, (s32 (*)(const void *, const void *))cmpItemOrder);
    fclSetWorkValue(4, work->hasUnregistered);
    return count;
}
#else
INCLUDE_ASM(const s32, "facility/fclRagInit", ragBuildHeartsList);
#endif

extern void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern s32 cmpItemOrder(u8 *a, u8 *b);

extern u8 D_0032FC48[];

extern u8 D_0032FC50[];

/* NON_MATCHING: 22 of 93 words differ, a real divide where retail folded the
 * VLA size into an andi, and the register cascade that follows. */
#ifdef NON_MATCHING
s8 ragBuildList2(u8 index, u8 *work)
{
    u8 *dst = work + 0x3B;
    s8 k = 0;
    s32 roll = dds3GetRandA(0);
    u32 n = 4;
    s8 pick[n];
    s32 extra = (u32)roll % sizeof(pick);
    u8 *src;
    s8 *p;
    u8 count;
    s32 i;

    p = &pick[n - 1];
    for (i = n - 1; i >= 0; i--) {
        *p-- = i;
    }
    shuffleIndices(pick, n, 0x10);
    src = D_0032FC48;
    for (i = n - 1; i >= 0; i--) {
        dst[k++] = *src;
        src += 2;
    }
    count = n;
    for (i = 0; i < extra; i++) {
        dst[k++] = ragGetListItem2(index, pick[i]);
    }
    count += extra;
    qsort(dst, count, 1, (s32 (*)(const void *, const void *))cmpItemOrder);
    return count;
}
#else
INCLUDE_ASM(const s32, "facility/fclRagInit", ragBuildList2);
#endif

/* NON_MATCHING: 22 of 93 words differ, the same residual as ragBuildList2. */
#ifdef NON_MATCHING
s8 ragBuildList3(u8 index, u8 *work)
{
    u8 *dst = work + 0x44;
    s8 k = 0;
    s32 roll = dds3GetRandA(0);
    u32 n = 4;
    s8 pick[n];
    s32 extra = (u32)roll % sizeof(pick);
    u8 *src;
    s8 *p;
    u8 count;
    s32 i;

    p = &pick[n - 1];
    for (i = n - 1; i >= 0; i--) {
        *p-- = i;
    }
    shuffleIndices(pick, n, 0x10);
    src = D_0032FC50;
    for (i = n - 1; i >= 0; i--) {
        dst[k++] = *src;
        src += 2;
    }
    count = n;
    for (i = 0; i < extra; i++) {
        dst[k++] = ragGetListItem3(index, pick[i]);
    }
    count += extra;
    qsort(dst, count, 1, (s32 (*)(const void *, const void *))cmpItemOrder);
    return count;
}
#else
INCLUDE_ASM(const s32, "facility/fclRagInit", ragBuildList3);
#endif

typedef struct {
    u8 reserved00[2];
    s16 eventBit;
    u8 hearts[2];
    u8 list2[2];
    u8 list3[2];
} ragListConfig_t;
extern ragListConfig_t gCommandFlag_0[];
extern u8 ragListFlags[5];
extern s32 ragBuildHeartsList(u8 type, ragListWork_t *work);
extern s8 ragBuildList2(u8 type, u8 *work);
extern s8 ragBuildList3(u8 type, u8 *work);
extern void cmpSetCursorFlag(cmpCursorInfo_t *cursor, s32 flags);

void ragCreateAllList(s32 onlyHearts, s32 type, ragListWork_t *work)
{
    s32 active;
    if (gCommandFlag_0[type].eventBit >= 0) {
        active = evtBitCheck(gCommandFlag_0[type].eventBit) != 0;
    } else {
        active = 0;
    }
    work->heartCount = ragBuildHeartsList(gCommandFlag_0[type].hearts[active], work);
    if (gCommandFlag_0[type].hearts[active] == 0) {
        ragListFlags[0] |= 2;
        work->cursor.unk_21++;
    }
    ragInitHeartsList(work->heartCount, &work->heartWindow);
    if (onlyHearts == 0) {
        work->list2Count = ragBuildList2(gCommandFlag_0[type].list2[active], (u8 *)work);
        if (gCommandFlag_0[type].list2[active] == 0) {
            ragListFlags[1] |= 2;
            work->cursor.unk_21++;
        }
        ragInitList2(work->list2Count, &work->window2);
        work->list3Count = ragBuildList3(gCommandFlag_0[type].list3[active], (u8 *)work);
        if (gCommandFlag_0[type].list3[active] == 0) {
            ragListFlags[2] |= 2;
            work->cursor.unk_21++;
        }
        cmpSetCursorFlag(&work->cursor, (s32)ragListFlags);
        ragInitList3(work->list3Count, &work->window3);
    }
}

typedef struct fclPanel
{
    s32 X;
    s32 Y;
    s32 Z;
    s16 W;
    s16 H;
    s32 Col;
    u8 Spd;
    u8 reserved15[3];
    s32 Timer;
    s32 Timer2;
    s32 Flag;
    u8 reserved24[0x10];
    s32 TexHandle;
    void (*draw)(struct fclPanel *panel, s32 id);
    void (*update)(struct fclPanel *panel);
} fclPanel_t;

typedef struct
{
    s32 tex;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    s32 X;
    s32 Y;
    s32 Z;
    u8 color[4];
    u8 Flag;
} fclSprite_t;

typedef struct
{
    u8 reserved0000[0x14];
    void *allocation;
    u8 reserved0018[0x462C];
    void *messageFile;
    u32 script;
    s32 scriptArg;
    s32 backgroundTexture;
    s32 texture[2];
    fclSprite_t messageSprite;
    fclPanel_t panel[3];
} ragLoadTextureWork_t;

extern char *D_0032FC28[];
extern char D_0053D150[];
extern char D_0053D170[];
extern s32 fclLoadTexture(char *filename);
extern void fclSetPanelTex(fclPanel_t *panel, s32 u0, s32 v0, s32 u1, s32 v1,
                           s32 texture);
extern void fclSetSprite(fclSprite_t *sprite, s32 width, s32 height, s32 z,
                         s32 u0, s32 v0, s32 u1, s32 v1, s32 color, s32 texture);

void fclRagLoadTextures(u8 *data)
{
    ragLoadTextureWork_t *work = (ragLoadTextureWork_t *)data;
    s8 i;

    for (i = 0; (u8)i < 2; i++) {
        work->texture[i] = fclLoadTexture(D_0032FC28[i]);
    }
    fclSetPanelTex(&work->panel[2], 0, 0, 0x100, 0x80, work->texture[1]);
    work->backgroundTexture = fclLoadTexture(D_0053D150);
    fclSetSprite(&work->messageSprite, 0x200, 0x1C0, 0, 5, 5, 0x1FB, 0xFB,
                 0x80808080, work->backgroundTexture);
    printf(D_0053D170);
}


extern char D_0053D188[];

void fclRagTexFree(u8 *pData)
{
    s32 *tex = (s32 *)(pData + 0x4654);
    s8 i;

    for (i = 0; (u8)i < 2; i++) {
        sdfTexFree(*tex++);
    }
    sdfTexFree(*(s32 *)(GBWK_0 + 0x4650));
    printf(D_0053D188);
}

void fclRagTexLoad(u8 *pData)
{
    fclRagTex_t *tex = (fclRagTex_t *)(pData + 0x4740);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

typedef struct ragPanel
{
    s32 X;
    s32 Y;
    s32 Z;
    s16 W;
    s16 H;
    s32 Col;
    u8 Spd;
    u8 reserved15[3];
    s32 Timer;
    s32 Timer2;
    s32 Flag;
    u8 reserved24[0x10];
    s32 TexHandle;
    void (*draw)(struct ragPanel *panel, s32 id);
    void (*update)(struct ragPanel *panel);
} ragPanel_t;

typedef struct ragSprite
{
    s32 tex;
    u16 width;
    u16 height;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    s32 X;
    s32 Y;
    s32 Z;
    u8 color[4];
    u8 flags;
} ragSprite_t;

typedef struct fclRagWork
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 sequenceFlag;
    /* 0x0002 */ u8 reserved02;
    /* 0x0003 */ u8 reserved03;
    /* 0x0004 */ u8 workStatus;
    /* 0x0005 */ u8 reserved05[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 reserved18;
    /* 0x0019 */ s8 ragType;
    /* 0x001A */ u8 reserved1A[0x462A];
    /* 0x4644 */ void *messageFile;
    /* 0x4648 */ u32 messageData;
    /* 0x464C */ u8 *forcedEndScript;
    /* 0x4650 */ s32 backgroundTexture;
    /* 0x4654 */ s32 texture[2];
    /* 0x465C */ ragSprite_t messageSprite;
    /* 0x4680 */ ragPanel_t panel[3];
    /* 0x4740 */ fclRagTex_t textureCalc[2];
} fclRagWork_t;

extern void fclInitCursorWindow(void *work, void *data, u8 count);
extern void fclSetCursorList(void *work, s32 mode);
extern char D_0054FB88[];
extern char D_0054FB90[];
extern char D_0053D1A0[];
extern char D_0053D1B0[];
extern char D_0053D1C0[];

void fclRagUpdateInit(s8 type, fclRagWork_t *owner)
{
    ragListWork_t *work = (ragListWork_t *)owner;
    u8 *items;
    s32 i;

    ragCreateAllList(0, type, work);
    fclInitCursorWindow((u8 *)work + 0x13C, ragHeartsTable + 0x8A0, 5);
    fclSetCursorList((u8 *)work + 0x13C, 0x19);
    memset(ragListFlags, 0, sizeof(ragListFlags));
    printf(D_0054FB88);
    printf(D_0053D1A0);
    items = (u8 *)work->heartItems;
    for (i = 31; i >= 0; i--) {
        printf(D_0054FB90, *items++);
    }
    printf(D_0054FB88);
    i = 0;
    printf(D_0053D1B0);
    if (work->list2Count > 0) {
        items = (u8 *)work->list2;
        do {
            printf(D_0054FB90, *items++);
            i++;
        } while (i < work->list2Count);
    }
    printf(D_0054FB88);
    i = 0;
    printf(D_0053D1C0);
    if (work->list3Count > 0) {
        items = (u8 *)work->list3;
        do {
            printf(D_0054FB90, *items++);
            i++;
        } while (i < work->list3Count);
    }
    printf(D_0054FB88);
    printf(D_0054FB88);
}

extern void *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(void *handle);
extern void *sdfDevLoadFileEx(const char *name, u32 *data, u32 *size);
extern void cmpInitSeqInfo(void *work, s32 seq);
extern void fclRagUpdateInit(s8 type, fclRagWork_t *work);
extern void fclInitPanel(ragPanel_t *panel, s32 x, s32 y, s32 z, s32 w,
                         s32 h, s32 color, s8 type, s8 speed);
extern void evtDisableMoonCalc(void);
extern void cmpModelInit(void);
extern void cmpResetDrawEnv(void);
extern void fclRagLoadTextures(u8 *work);
extern void fclRagTexLoad(u8 *work);
extern void p2sdPlayBGM(s32 id);
extern char D_0053D1D0[];

fclRagWork_t *fclAllocRagWork(s8 type, s32 seq)
{
    void *handle = sdfMemAllocDirect(0x4788);
    fclRagWork_t *work = sdfMemLock(handle);

    memset(work, 0, 0x4788);
    work->allocation = handle;
    cmpInitSeqInfo(work, seq);
    work->sequenceFlag = 0;
    work->ragType = type;
    work->workStatus = 1;
    work->reserved03 = 0;
    work->forcedEndScript = NULL;
    fclRagUpdateInit(type, work);
    fclInitPanel(&work->panel[0], 0xF60, 0x280, 0xFFFFF, 0xE5, 0x6B,
                 0x161E2766, 2, 0x10);
    fclInitPanel(&work->panel[1], 0xEA0, 0x6E0, 0xFFFFF, 0xF1, 0x51,
                 0x101E3159, 4, 0x10);
    fclInitPanel(&work->panel[2], 0, 0x190, 0xFFFFF, 0x200, 0x109,
                 0, 0, 0x1D);
    work->messageFile = sdfDevLoadFileEx(D_0053D1D0, &work->messageData, NULL);
    evtDisableMoonCalc();
    fclRagLoadTextures((u8 *)work);
    cmpModelInit();
    cmpResetDrawEnv();
    fclRagTexLoad((u8 *)work);
    p2sdPlayBGM(0x390000);
    fclSetWorkValue(2, 0);
    return work;
}

extern void fclRagTexFree(u8 *pData);

extern void evtDisableMoonDraw(void);

void fclDestroyRag(void)
{
    if (GBWK_0 == NULL) {
        return;
    }
    fclRagTexFree(GBWK_0);
    fclScriptForcedEnd(((fclRagWork_t *)GBWK_0)->forcedEndScript);
    sdfMemFreeDirect(((fclRagWork_t *)GBWK_0)->messageFile);
    fclRagEventEnd();
    sdfMemFreeDirect(((fclRagWork_t *)GBWK_0)->allocation);
    GBWK_0 = NULL;
    gProcessStat_1 = 2;
    evtDisableMoonDraw();
    p2sdStopBGM(0x390000);
}

extern fclRagWork_t *fclAllocRagWork(s8 type, s32 seq);
extern void FclDebug(s32 mode, void *work);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority,
                                          s32 startTimer, s32 shutdownTimer,
                                          dds3ProcessFunc_t process,
                                          dds3ProcessFunc_t shutdown,
                                          void *work);
extern s32 ragCalc(dds3ProcessID_t *id);
extern s32 ragDraw(dds3ProcessID_t *id);
extern s32 ragUpdate(dds3ProcessID_t *id);

void fclRagProcessStart(s8 Signal)
{
    if (GBWK_0 == NULL) {
        GBWK_0 = (u8 *)fclAllocRagWork(Signal, 0);
        FclDebug(2, GBWK_0);
        dds3StartProcess(D_0054FB98, 0x403, 1, 1, ragCalc, NULL, GBWK_0);
        dds3StartProcess(D_0053D1F0, 0x2B13, 1, 1, ragDraw, NULL, GBWK_0);
        dds3StartProcess(D_0053D200, 0x520F, 1, 1, ragUpdate,
                         (dds3ProcessFunc_t)fclDestroyRag, GBWK_0);
        gProcessStat_1 = 1;
    }
}

void fclRagProcessEnd(void)
{
    dds3KillProcessName(D_0054FB98, 1);
    dds3KillProcessName(D_0053D1F0, 1);
    dds3KillProcessName(D_0053D200, 1);
}

s32 ragCheckProcessStat(void)
{
    s8 stat = gProcessStat_1;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            gProcessStat_1 = 0;
        }
    }
    return 0;
}

void func_0020ADE0(void)
{
    if (GBWK_0 != NULL) {
        *GBWK_0 |= 0x10;
    }
}

u16 *ragGetConfirmEntry(s8 kind, u8 index)
{
    switch (kind) {
    case 0:
        return (u16 *)(ragHeartsTable + 0x2 + index * 0xE);
    case 1:
        return (u16 *)(ragHeartsTable + 0x180 + index * 0x2C);
    case 2:
        return (u16 *)(ragHeartsTable + 0x520 + index * 0x2C);
    default:
        return NULL;
    }
}

typedef struct
{
    /* 0x00 */ u16 item;
    /* 0x02 */ s8 count;
    /* 0x03 */ u8 unk_03;
} ragItemReq_t;

s32 ragFindItemSlot(ragItemReq_t *req)
{
    u8 *base = (u8 *)cmbGlobalWork + 8;
    s32 i;

    for (i = 0; i < 3; i++) {
        u32 item = req->item;

        if (item != 0 && ((u8 *)item + (u32)base)[0x1920] < req->count) {
            return 0;
        }
        req++;
    }
    return 1;
}

extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);
extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

void ragSetSeqPhase2(ragOffer_t *offer, datUnitWork_t *unit)
{
    s32 i;
    ragCustomUnit_t *custom;

    cmbCopyDefaultDevilToStock(offer->devilId, unit);
    custom = &offer->unit;
    if (offer->custom == 0) {
        return;
    }
    unit->level = custom->level;
    i = 0;
    do {
        unit->param[i] = custom->params[i];
        i++;
    } while (i < 6);
    unit->skillcnt = 0;
    for (i = 0; i < 8; i++) {
        if ((unit->skill[i] = custom->skills[i]) != 0) {
            unit->skillcnt++;
        }
    }
    unit->exp = datCopyDevilWorkEntry(unit, unit->level);
}

extern campStatusPanel_t *ragCalcWork;

extern void *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(void *basic, s32 visible);

void ragUpdateScene(void)
{
    void *scene;

    if (ragCalcWork->unk_04 != 0) {
        scene = dds3GetActiveScene();
        if (scene != NULL) {
            dds3VisibleSceneBasic(scene, 0);
        }
    }
}

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D140);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D150);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D170);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D188);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D1A0);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D1B0);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D1C0);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D1D0);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D1F0);

INCLUDE_RODATA(const s32, "facility/fclRagInit", D_0053D200);

INCLUDE_SDATA(const s32, "facility/fclRagInit", GBWK_0);

INCLUDE_SDATA(const s32, "facility/fclRagInit", gProcessStat_1);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB75);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB78);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB80);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB88);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB90);

INCLUDE_SDATA(const s32, "facility/fclRagInit", D_0054FB98);

