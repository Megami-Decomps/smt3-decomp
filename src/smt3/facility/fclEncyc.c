#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    /* 0x0000 */ u16 unk_00;
    /* 0x0002 */ u16 index;
    /* 0x0004 */ u8 shift;
    /* 0x0005 */ u8 unk_05[3];
} cmpCursorPos_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x17C];
    /* 0x017C */ cmpCursorPos_t cursorPos;
} lstListWindow_t;

/* The save-work fields this unit reaches that the shared dds3GlobalWork_t view
 * leaves as padding or does not cover at all. */
typedef struct
{
    /* 0x00000 */ u8 unk_00000[0x1914];
    /* 0x01914 */ u8 id[0x10];
    /* 0x01924 */ s32 idCount;
    /* 0x01928 */ u8 unk_01928[0x10558];
    /* 0x11E80 */ s32 recordCount;
} dds3GlobalWorkView_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_t;

extern fclEncyc_instance_t *g_pinst;

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
    /* 0x14 */ s32 activeTime;
    /* 0x18 */ s32 deactiveTime;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
} fclRbnWork_t;

extern s32 GetValidUnitIndex(s32 index);

extern void ReleaseDebug_fclEncyc(void);

extern void sdfMemFreeDirect(void *ptr);

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

extern void qsort(void *base, s32 count, s32 size, s32 (*cmp)(const void *a, const void *b));

typedef struct
{
    /* 0x00 */ u8 unk_00[0x54];
    /* 0x54 */ s32 unk_54;
} motionWork_t;

extern fclRbnWork_t *g_pdebug;

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

extern s32 printf(const char *fmt, ...);

typedef struct
{
    /* 0x00 */ u8 unk_00[0xC];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 unk_1E[2];
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
} fclReadRibbon_t;

extern s32 dds3KillProcess(s32 process, s32 shutdowntimer);

typedef struct
{
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} cmpVec4_t;

extern cmpVec4_t cmpStatusModelPos;

extern cmpVec4_t cmpStatusModelBasePos;

extern void itfMesMngRelease(s32 msg);

extern void cmpResetDrawEnv(void);

extern void FillIndex(s16 *dst, s32 count);

extern void fclEncycSetExplainData(void *data);

extern void frDestroyFontRequestAfter(void *frq);

extern void fclRbnRelease(fclRbnWork_t *work);

extern void fclEncycStartProcess(void);

extern void fclEncycSetTexHandle(s32 *src);

/* Called above its definition, where ee-gcc assumed int. */
void *dds3GetActiveScene(void);

extern void *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(void *scene, s32 flag);

extern void itfMesMngRequestMessageRelease(s32 msg, s32 flag);

void fclUpdatePanel(fclPanel_t *panel);

s32 GetPriceForSummon(s32 arg0, s32 arg1, s32 arg2, s32 level, s32 str, s32 mag, s32 vit, s32 agi)
{
    s32 total = level + str + mag + vit + agi;

    return total * total / 20 * 100;
}

s32 fclEncycWork(void)
{
    return g_pinst != NULL;
}

void fclEncycInitRecord(s32 mode)
{
    s32 *count = &((dds3GlobalWorkView_t *)DDS3_GBWK)->recordCount;

    if (mode == 0) {
        *count = 0;
    }
    g_pinst = NULL;
    g_pdebug = NULL;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", fclEncycStartProcess);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D768);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D778);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D798);

INCLUDE_ASM(const s32, "facility/fclEncyc", fclEncycKillProcess);

void fclEncycSetTexHandle(s32 *src)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        g_pinst->texHandle[i] = src[i];
    }
}

void fclEncycSetExplainData(void *data)
{
    g_pinst->explainData = data;
}

/* One encyclopaedia record. Member names from the IDB, which the two local
 * views this replaces already agreed with where they overlapped: one was a
 * 0xB0 blob reading only id, the other a 0x52 prefix reading the param rows. */
typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 reserved0;
    /* 0x04 */ u16 maxhp;
    /* 0x06 */ u16 maxmp;
    /* 0x08 */ u32 exp;
    /* 0x0C */ u16 level;
    /* 0x0E */ s8 param[6];
    /* 0x14 */ s8 skillparam[6];
    /* 0x1A */ u16 skillcnt;
    /* 0x1C */ u16 skill[24];
    /* 0x4C */ s8 mitamaparam[6];
    /* 0x52 */ s8 levelupparam[6];
    /* 0x58 */ u16 keisyoskill[24];
    /* 0x88 */ u8 keiattr[24];
    /* 0xA0 */ u16 hensinmae;
    /* 0xA2 */ u16 reserved1;
    /* 0xA4 */ u32 flag;
    /* 0xA8 */ u32 reserved2;
    /* 0xAC */ u32 hensinmaeexp;
} fclencyceelem_t;

extern s32 fclEncycSearch(s32 devilId);

extern s32 fclEncycGetRatio(void);

extern void SetElem(fclencyceelem_t *dst, fclencyceelem_t *src);

extern void SetElem2(fclencyceelem_t *dst, fclencyceelem_t *src);

extern s32 datDebugPrintf(const char *fmt, ...);

extern s32 evtBitOn(s32 bit);

extern s32 evtBitOff(s32 bit);

s32 fclEncycRegist2(fclencyceelem_t *entry, s32 store)
{
    s32 *num = &cmbGlobalWork->encycNum;
    fclencyceelem_t *table = (fclencyceelem_t *)((u8 *)cmbGlobalWork + 0x11E90);
    s32 index = fclEncycSearch(*(u16 *)((u8 *)entry + 0x0));
    s32 known = 1;
    s32 count;

    if (index < 0) {
        count = *num;
        if (count >= 0xB8) {
            printf("****** FATAL!! The encyclopedia record is bound to burst!!!\n");
            datDebugPrintf("****** FATAL!! The encyclopedia record is bound to burst!!!\n");
            return -1;
        }
        known = 0;
        index = count;
        *num = index + 1;
    } else {
        if (store == 0) {
            return -1;
        }
        known = 1;
    }
    SetElem2(&table[index], entry);
    if (fclEncycGetRatio() >= 0x64) {
        evtBitOn(0x913);
    } else {
        evtBitOff(0x913);
    }
    return known;
}

s32 fclEncycRegist(fclencyceelem_t *entry, s32 store)
{
    s32 *num = &cmbGlobalWork->encycNum;
    fclencyceelem_t *table = (fclencyceelem_t *)((u8 *)cmbGlobalWork + 0x11E90);
    s32 index = fclEncycSearch(*(u16 *)((u8 *)entry + 0x4));
    s32 known = 1;
    s32 count;

    if (index < 0) {
        count = *num;
        if (count >= 0xB8) {
            printf("****** FATAL!! The encyclopedia record is bound to burst!!!\n");
            datDebugPrintf("****** FATAL!! The encyclopedia record is bound to burst!!!\n");
            return -1;
        }
        known = 0;
        index = count;
        *num = index + 1;
    } else {
        if (store == 0) {
            return -1;
        }
        known = 1;
    }
    SetElem(&table[index], entry);
    if (fclEncycGetRatio() >= 0x64) {
        evtBitOn(0x913);
    } else {
        evtBitOff(0x913);
    }
    return known;
}

s32 fclEncycSearch(s32 devilId)
{
    fclencyceelem_t *entry = (fclencyceelem_t *)((u8 *)cmbGlobalWork + 0x11E90);
    s32 count = cmbGlobalWork->encycNum;
    s32 i;

    for (i = 0; i < count; i++) {
        if (devilId == entry->id) {
            return i;
        }
        entry++;
    }
    return -1;
}

s32 fclEncycGetNum(void)
{
    return cmbGlobalWork->encycNum;
}

s32 fclEncycGetRatio(void)
{
    s32 num = fclEncycGetNum();
    s32 ratio = num * 100 / 0xB8;

    if (num > 0) {
        if (ratio <= 0) {
            ratio = 1;
        }
    }
    return ratio;
}

s32 fclEncycCheckRegistable(void)
{
    return GetValidUnitIndex(0);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", fclEncycDebugRegist);

INCLUDE_ASM(const s32, "facility/fclEncyc", fclEncycDebugProc);

void fclEncycDebugShutdown(void)
{
    ReleaseDebug_fclEncyc();
}

void InitializeInstance(fclEncyc_instance_t *work)
{
    work->unk_6D = 1;
    work->modelId = -1;
    work->flags = 0;
    work->unk_58 = 0;
    work->unk_5A = 0;
    work->listCursor[0] = 0;
    work->listCursor[1] = 0;
    work->listCursor[2] = 0;
    work->listCursor[3] = 0;
    work->unk_6C = 0;
    work->unk_70 = 0;
    work->explainData = NULL;
    work->unk_364 = 0;
    work->unk_36C = 0;
    SetScreenOffset(work, 0);
}

extern void *sdfMemAllocDirect(s32 size);

extern s32 sdfMemLock(void *ptr);

extern void *sdfMemAllocDirect(s32 size);

extern s32 sdfMemLock(void *ptr);

void allocPhaseBuffer(fclEncyc_instance_t *work, s32 size)
{
    void *buf = sdfMemAllocDirect(size);
    s32 lock = sdfMemLock(buf);

    work->allocation = buf;
    work->unk_36C = lock;
}

void ReleaseWork(fclEncyc_instance_t *work)
{
    if (work->unk_36C != 0) {
        sdfMemFreeDirect(work->allocation);
        work->unk_36C = 0;
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CALC_ENCYC_Process);

extern void SetCameraLightParam(fclEncyc_instance_t *work);

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

s32 DRAW_ENCYC_Process(void)
{
    fclEncyc_instance_t *work = g_pinst;
    void (*phase)(fclEncyc_instance_t *work);

    if (work == NULL) {
        return 0;
    }
    if ((work->flags & 0x20) == 0) {
        return 0;
    }
    SetCameraLightParam(work);
    phase = DrawPhaseTable[(s16)work->unk_58];
    if (phase != NULL) {
        phase(work);
    }
    return 0;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", UPDT_ENCYC_Process);

INCLUDE_ASM(const s32, "facility/fclEncyc", InitPhaseRegistTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseRegistTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawPhaseRegistTop);

extern void ReleaseFontList(void **list, s32 count);

void DestPhaseRegistTop(fclEncyc_instance_t *work)
{
    s32 base = work->unk_36C;

    if (*(void **)(base + 0x11DC) != NULL)
    {
        frDestroyFontRequestAfter(*(void **)(base + 0x11DC));
        *(void **)(base + 0x11DC) = NULL;
    }
    ReleaseFontList((void **)(base + 0x11E0), 7);
    if (*(s32 *)(base + 0x11FC) != 0)
    {
        dds3KillProcess(*(s32 *)(base + 0x11FC), 0);
    }
    ReleaseWork(work);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", InitPhaseRegistConfirm);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseRegistConfirm);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawPhaseRegistConfirm);

extern void fclRbnRelease(fclRbnWork_t *work);

void DestPhaseRegistConfirm(fclEncyc_instance_t *work)
{
    void *scene;

    fclRbnRelease(*(fclRbnWork_t **)(work->unk_36C + 0x200));
    cmpResetDrawEnv();
    scene = dds3GetActiveScene();
    if (scene != NULL)
    {
        dds3VisibleSceneBasic(scene, 1);
    }
    ReleaseWork(work);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseRegistAll);

INCLUDE_ASM(const s32, "facility/fclEncyc", InitPhaseReadTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseReadTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawPhaseReadTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", DestPhaseReadTop);

INCLUDE_ASM(const s32, "facility/fclEncyc", InitPhaseReadStatus);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseReadStatus);

void DrawPhaseReadStatus(fclEncyc_instance_t *work)
{
    s32 base = work->unk_36C;

    DrawBackBlackBoard(*(s32 *)(base + 0x58), *(s16 *)(base + 0x5C));
    if (cmpGetEncycWork() != 0) {
        work->flags |= 0x400;
    }
    cmpDrawStatusComEx(base + 0x90, *(u8 *)(base + 0x1E), -1, 0, 0, 0, 0xA09DC380, 1, 0, 0);
    DrawExplainBack(*(s16 *)(base + 0x5E));
    fclRbnDraw(*(s32 *)(base + 0x80), *(s32 *)(base + 0xC), *(s32 *)(base + 0x10), 0xFFFFF0,
               *(s16 *)(base + 0x1C), 5, 0x3A0, 0x47);
    if (*(s16 *)(base + 0x1C) < 0x80) {
        DrawReadStatusMoney(work);
    }
    DrawReadMainHelp(work);
}

void DestPhaseReadStatus(fclEncyc_instance_t *work)
{
    s32 base = work->unk_36C;
    void *scene;

    ReleaseFontExplain(work, 1);
    itfMesMngRelease(*(s32 *)(base + 0x8));
    if (*(void **)(base + 0x84) != NULL) {
        frDestroyFontRequestAfter(*(void **)(base + 0x84));
        *(void **)(base + 0x84) = NULL;
    }
    fclRbnRelease(*(fclRbnWork_t **)(base + 0x80));
    cmpResetDrawEnv();
    scene = dds3GetActiveScene();
    if (scene != NULL) {
        dds3VisibleSceneBasic(scene, 1);
    }
    ReleaseWork(work);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseReadSummon);

void DrawPhaseReadSummon(fclEncyc_instance_t *work)
{
    s32 base = work->unk_36C;

    DrawBackBlackBoard(*(s32 *)(base + 0x58), *(s16 *)(base + 0x5C));
    if (cmpGetEncycWork() != 0) {
        work->flags |= 0x400;
    }
    cmpDrawStatusComEx(base + 0x90, *(u8 *)(base + 0x1E), -1, 0, 0, 0, 0xA09DC380, 1, 0, 0);
    DrawExplainBack(*(s16 *)(base + 0x5E));
    fclRbnDraw(*(s32 *)(base + 0x80), *(s32 *)(base + 0xC), *(s32 *)(base + 0x10), 0xFFFFF0,
               *(s16 *)(base + 0x1C), 5, 0x3A0, 0x47);
    if (*(s16 *)(base + 0x1C) < 0x80) {
        DrawReadStatusMoney(work);
    }
    DrawReadMainHelp(work);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseReadExplain);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawPhaseReadExplain);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcPhaseReadView);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawPhaseReadView);

INCLUDE_ASM(const s32, "facility/fclEncyc", AcceptKeyRegistConfirm);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcConfirmStatus);

INCLUDE_ASM(const s32, "facility/fclEncyc", GetValidUnitIndex);

INCLUDE_ASM(const s32, "facility/fclEncyc", RegistAll);

INCLUDE_ASM(const s32, "facility/fclEncyc", InitListWindow);

void PushListwinCursor(fclEncyc_instance_t *pinst, lstListWindow_t *plistwin, s32 stack)
{
    cmpCursorPos_t *cur = &plistwin->cursorPos;

    pinst->listCursor[stack * 2] = cur->index;
    pinst->listCursor[stack * 2 + 1] = cur->shift;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", PopListwinCursor);

void AdjustStackCursor(s32 count, s16 *cursor, s16 *scroll)
{
    s32 pos = *cursor;
    s32 top = *scroll;

    if (count <= 0) {
        return;
    }
    if (pos + 7 < count) {
        return;
    }
    if (pos > 0) {
        pos--;
    } else if (pos + top >= count) {
        top = count - 1;
    }
    *cursor = pos;
    *scroll = top;
}

void ResetReadMainRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_0C = 0;
    rbn->unk_10 = 0x58;
    rbn->unk_14 = 0x480;
    rbn->unk_18 = 0xA8;
    rbn->unk_1C = 0x80;
    rbn->unk_20 = 0;
    rbn->unk_24 = 0x100;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", AcceptKeyChangeSort);

INCLUDE_ASM(const s32, "facility/fclEncyc", ApplyChangeListIndex);

INCLUDE_ASM(const s32, "facility/fclEncyc", AcceptKeyListWindow);

extern s32 cmpRaceOrder(const void *a, const void *b);

void CreateSortIndexByRace(s16 *index)
{
    s32 count = fclEncycGetNum();

    FillIndex(index, count);
    qsort(index, count, 2, cmpRaceOrder);
}

extern s32 cmpLevelOrder(const void *a, const void *b);

void CreateSortIndexByLevel(s16 *index)
{
    s32 count = fclEncycGetNum();

    FillIndex(index, count);
    qsort(index, count, 2, cmpLevelOrder);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", cmpRaceOrder);

INCLUDE_ASM(const s32, "facility/fclEncyc", cmpLevelOrder);

void FillIndex(s16 *dst, s32 count)
{
    s16 i;

    for (i = 0; i < count; i++) {
        *dst++ = i;
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", SetElem);

INCLUDE_ASM(const s32, "facility/fclEncyc", SetElem2);

extern s32 fclEncycSearch(s32 devilId);

extern void SetRecordToUnitCore(void *unit, void *record);

void SetRecordToUnit(datUnitWork_t *unit, s32 devilId)
{
    u8 *base = (u8 *)DDS3_GBWK + 0x11E80;
    s32 index = fclEncycSearch(devilId);

    SetRecordToUnitCore(unit, base + index * 0xB0 + 0x10);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", SetRecordToUnitCore);

INCLUDE_ASM(const s32, "facility/fclEncyc", CompareUnitToRecord);

extern s32 datAddDevil(u16 id, s32 mode);

void ExecSummon(s32 index)
{
    u16 *record = (u16 *)((u8 *)DDS3_GBWK + index * 0xB0 + 0x11E90);
    s32 slot = datAddDevil(*record, 0);

    SetRecordToUnitCore(&cmbGlobalWork->party[slot], record);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", drawRegistList);

INCLUDE_ASM(const s32, "facility/fclEncyc", CreateFontListRead);

void ReleaseFontList(void **list, s32 count)
{
    if (count > 0) {
        void **p = list;
        s32 i = count;

        do {
            if (*p != NULL) {
                frDestroyFontRequestAfter(*p);
                *p = NULL;
            }
            p++;
            i--;
        } while (i != 0);
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CreateFontExplain);

extern void itfMesMngRequestMessageRelease(s32 msg, s32 flag);

void ReleaseFontExplain(fclEncyc_instance_t *work, s32 release)
{
    s32 base = work->unk_36C;

    itfMesMngRequestMessageRelease(*(s32 *)(base + 8), 0);
    if (*(void **)(base + 0x88) != NULL)
    {
        frDestroyFontRequestAfter(*(void **)(base + 0x88));
        *(void **)(base + 0x88) = NULL;
    }
    if (release != 0)
    {
        itfMesMngRelease(*(s32 *)(base + 8));
    }
}

void FadeOutRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_1C -= 0x10;
    if (rbn->unk_1C < 0) {
        rbn->unk_1C = 0;
    }
    rbn->unk_20 += 0x20;
    rbn->unk_0C -= rbn->unk_20;
    rbn->unk_14 -= rbn->unk_24;
    rbn->unk_24 -= 0x20;
}

void FadeInRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_1C += 0x10;
    if (rbn->unk_1C >= 0x81) {
        rbn->unk_1C = 0x80;
    }
    rbn->unk_24 += 0x20;
    rbn->unk_0C += rbn->unk_20;
    rbn->unk_20 -= 0x20;
    rbn->unk_14 += rbn->unk_24;
}

s32 GetDevilParam(fclencyceelem_t *elem, s32 type)
{
    s32 sum = elem->param[type] + elem->skillparam[type] + elem->mitamaparam[type];

    if (sum <= 0) {
        sum = 1;
    } else if (sum >= 0x29) {
        sum = 0x28;
    }
    return sum;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", PrepSummon);

INCLUDE_ASM(const s32, "facility/fclEncyc", drawSummonCount);

INCLUDE_ASM(const s32, "facility/fclEncyc", drawEncycRatio);

s32 CalcFadeAlpha(s16 *alpha, s32 up, s32 limit, s32 step)
{
    s16 v = *alpha;
    s32 more = 1;

    if (up != 0) {
        v = v + step;
        if (v > limit) {
            v = limit;
            more = 0;
        }
    } else {
        v = v - step;
        if (v < 0) {
            v = 0;
            more = 0;
        }
    }
    *alpha = v;
    return more;
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcListElemColor);

/* Called above its definition, where ee-gcc assumed int. */
u16 cmpGetStatusModelId(void);

void RequestModelLoad(fclEncyc_instance_t *work, s32 id)
{
    if (cmpGetEncycWork() != 0) {
        if (id == cmpGetStatusModelId()) {
            return;
        }
    }
    *(u16 *)((u8 *)work + 0x6E) = id;
    work->flags |= 4;
}

void SetScreenOffset(fclEncyc_instance_t *work, s32 flag)
{
    if (flag != 0)
    {
        work->screenOfsX = 2188.0f;
        work->screenOfsY = 2048.0f;
    }
    else
    {
        work->screenOfsX = 2048.0f;
        work->screenOfsY = 2048.0f;
    }
}

void MoveScreenOffset(fclEncyc_instance_t *work)
{
    if ((work->flags & 8) != 0) {
        work->screenOfsX -= 16.0f;
        if (work->screenOfsX < 2048.0f) {
            work->screenOfsX = 2048.0f;
        }
    } else {
        work->screenOfsX += 16.0f;
        if (2188.0f < work->screenOfsX) {
            work->screenOfsX = 2188.0f;
        }
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", SetCameraLightParam);

INCLUDE_ASM(const s32, "facility/fclEncyc", ControlModel);

INCLUDE_ASM(const s32, "facility/fclEncyc", GetViewModeKey);

INCLUDE_ASM(const s32, "facility/fclEncyc", GetAnalogPad);

INCLUDE_ASM(const s32, "facility/fclEncyc", InitCameraForViewMode);

INCLUDE_ASM(const s32, "facility/fclEncyc", SetBoundControl);

void initViewCamera(cmpVec4_t *v)
{
    SDF_VU0_COPYVECTOR(&v[0], &cmpStatusModelBasePos);
    SDF_VU0_COPYVECTOR(&v[1], &cmpStatusModelPos);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", InitModelPosRot);

void RequestChangeMotion(motionWork_t *work, s32 loop)
{
    s32 flags = work->unk_54 & ~0xC0;

    work->unk_54 = flags | 0x40;
    if (loop != 0) {
        work->unk_54 = flags | 0xC0;
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", ChangeModelMotion);

INCLUDE_ASM(const s32, "facility/fclEncyc", CirculateDevilObject);

INCLUDE_ASM(const s32, "facility/fclEncyc", ChangeDevilObjectRegistConfirm);

extern void RequestModelLoad(fclEncyc_instance_t *work, s32 id);

void ChangeDevilObjectReadMain(s32 index, fclEncyc_instance_t *work)
{
    s32 base = work->unk_36C;
    u16 *record = (u16 *)((u8 *)DDS3_GBWK + index * 0xB0 + 0x11E90);

    *(u16 *)base = index;
    RequestModelLoad(work, *record);
    SetRecordToUnitCore((void *)(base + 0x90), record);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcMoveRegistKeyHelp);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcMoveReadMainHelp);

INCLUDE_ASM(const s32, "facility/fclEncyc", CalcMoveViewModeRibbon);

extern s32 fclDrawPanel(fclPanel_t *panel, s32 id);

extern void lstDrawListWindow(void *list, s32 mode, s32 id);

void DrawListWindowBack(void *list, fclPanel_t *panel)
{
    fclDrawPanel(panel, 0x47);
    fclUpdatePanel(panel);
    lstDrawListWindow(list, 0, 0x47);
}

INCLUDE_ASM(const s32, "facility/fclEncyc", drawRegistSprites);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawRegistListwinElem);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawReadListwinCaption);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawReadListwinElem);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawReadStatusMoney);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawExplainBack);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawBackBlackBoard);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawRegistAllHelp);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawRegistKeyHelp);

INCLUDE_ASM(const s32, "facility/fclEncyc", DrawReadMainHelp);

void InitializeDebug_fclEncyc(void)
{
    void *buf = sdfMemAllocDirect(0xC);
    fclRbnWork_t *work = (fclRbnWork_t *)sdfMemLock(buf);

    work->allocation = buf;
    work->unk_04 = 0;
    work->unk_06 = 0;
    work->unk_08 = 0;
    work->unk_0A = 0;
    g_pdebug = work;
}

void ReleaseDebug_fclEncyc(void)
{
    if (g_pdebug != NULL) {
        sdfMemFreeDirect(g_pdebug->allocation);
        g_pdebug = NULL;
    }
}

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugProcMenu_fclEncyc);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D828);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D838);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D848);

INCLUDE_RODATA(const s32, "facility/fclEncyc", D_0053D858);

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugProcRegister);

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugProcViewkey);

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugProcDispBound);

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugRemoveRecord);

INCLUDE_ASM(const s32, "facility/fclEncyc", DebugUpdateListwin);
INCLUDE_SDATA(const s32, "facility/fclEncyc", g_pinst);

INCLUDE_SDATA(const s32, "facility/fclEncyc", g_pdebug);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCB0);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCB8);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCC0);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCC8);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCD0);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCD8);

INCLUDE_SDATA(const s32, "facility/fclEncyc", D_0054FCE0);

