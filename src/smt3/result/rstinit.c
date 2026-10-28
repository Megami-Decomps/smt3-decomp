#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern void *dds3SearchProcessName(const char *namestr);

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
} fclEncyc_instance_tag;

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 reserved01[0x17];
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ s8 unk_19;
    /* 0x001A */ u8 unk_1A[0x16];
    /* 0x0030 */ datUnitWork_t *levelUpUnit[16];
    /* 0x0070 */ s8 levelUpCount;
    /* 0x0071 */ s8 levelUpStep;
    /* 0x0072 */ u8 unk_72[0x0B];
    /* 0x007D */ u8 reserved7D;
    /* 0x007E */ u8 reserved7E[6];
    /* 0x0084 */ u16 evoID;
    /* 0x0086 */ u8 reserved86[0xE6];
    /* 0x016C */ datUnitWork_t *unit;
    /* 0x0170 */ u8 unk_170[0x2175];
    /* 0x22E5 */ s8 unk_22E5;
    /* 0x22E6 */ s8 unk_22E6;
    /* 0x22E7 */ u8 unk_22E7;
    /* 0x22E8 */ f32 unk_22E8;
    /* 0x22EC */ f32 unk_22EC;
    /* 0x22F0 */ u8 unk_22F0[0x18];
    /* 0x2308 */ s32 unk_2308;
} evtStartWork_t;

extern evtStartWork_t *EventStart;

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

extern evtStartWork_t *GBWK_1;

extern s32 printf(const char *fmt, ...);

extern char D_0053DA28[];

extern s8 gProcessStat_4;

extern char D_0054FCF0[];

extern char D_0053DA90[];

extern char D_0053DAA0[];

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);
extern void fclClearFlag(s8 index);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern void rstGetEvoModelParam(u16 id, f32 *out);
extern datUnitVisual_t datModelParamTable[];
extern void evtStartEvent(s32 no);

void rstInitEvoAct(void)
{
    f32 modelParam[3];

    fclClearFlag(0);
    fclClearFlag(1);
    fclSetWorkValue(2, 1);
    fclSetWorkValue(3, 1);
    fclSetWorkValue(4, GBWK_1->unit->id);
    fclSetWorkValue(5, GBWK_1->evoID);
    rstGetEvoModelParam(GBWK_1->evoID, modelParam);
    modelParam[1] *= datModelParamTable[GBWK_1->evoID].formscale;
    fclSetWorkValue(6, (s32)modelParam[1]);
    evtStartEvent(0x329);
    GBWK_1->reserved7D = 0;
}

s32 rstChkEvoAct(void)
{
    char name[10] = "e809_main";

    return dds3SearchProcessName(name) != 0;
}

void rstKillEvoAct(void)
{
    fclFreeStage();
    printf(D_0053DA28);
}

void rstSetTestParam(void)
{
    cmbGlobalWork->party[0].unit.exp += 100;
    cmbGlobalWork->party[4].unit.exp += cmbGlobalWork->party[4].unit.level * 100;
}

s32 rstChkLevelUpTarget(datUnitWork_t *pStock)
{
    if (pStock->flag & 1) {
        if (!(pStock->badstatus & 0x800)) {
            if (cmbGetLevelUpCount(pStock, 0) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern s32 rstChkLevelUpTarget(datUnitWork_t *pStock);

s32 rstChkResultEntry(void)
{
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        if (rstChkLevelUpTarget(&cmbGlobalWork->party[i].unit) != 0)
        {
            return 1;
        }
    }
    return 0;
}

extern s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp);

u8 rstCalcLevelFromExp(u8 Index, u32 Exp)
{
    datUnitWork_t *unit = &cmbGlobalWork->party[Index].unit;

    return (u8)unit->level + cmbGetLevelUpCount(unit, Exp - unit->exp);
}

s32 rstCalcExpFromLevel(u8 Index, s32 Level)
{
    return datCopyDevilWorkEntry(&cmbGlobalWork->party[Index].unit, Level);
}

void rstChkResult2Process(u8 *work)
{
    memset(&work[0x129F], 0, 6);
}

INCLUDE_ASM(const s32, "result/rstinit", rstCreateTargetList);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA28);

INCLUDE_ASM(const s32, "result/rstinit", rstInit);

/* Resource prefix: allocations and texture are installed by rstInit. */
typedef struct {
    u8 reserved00[0x14];
    sdfMemHandle_t *allocation;
    u8 reserved18[0x22E0];
    sdfMemHandle_t *scriptAllocation;
    void *script;
    sdfMemHandle_t *messageAllocation;
    u8 reserved2304[8];
    sdfTexHandle_t *texture;
} rstResourceWork_t;

extern void rstClearResultWork(void);
extern void sdfTexFree(sdfTexHandle_t *texture);
extern s32 fclDestroyMessage(void);
extern void sdfMemFreeDirect(sdfMemHandle_t *allocation);
extern void cmpModelFreeResource(void);
extern void cmpResetDrawEnv(void);
extern void rstSetPanelEffFlag(void);
extern void rstSetStatusModelReady(s32 ready);
extern s32 p2sdStopBGM(s32 id);

/* KillProcess ignores this shutdown callback's return value. */
void rstDestroyResult2(dds3ProcessID_t *id)
{
    if (GBWK_1 != NULL) {
        rstClearResultWork();
        sdfTexFree(((rstResourceWork_t *)GBWK_1)->texture);
        fclDestroyMessage();
        sdfMemFreeDirect(((rstResourceWork_t *)GBWK_1)->messageAllocation);
        cmpModelFreeResource();
        cmpResetDrawEnv();
        sdfMemFreeDirect(((rstResourceWork_t *)GBWK_1)->scriptAllocation);
        rstSetPanelEffFlag();
        rstSetStatusModelReady(0);
        sdfMemFreeDirect(((rstResourceWork_t *)GBWK_1)->allocation);
        GBWK_1 = NULL;
        gProcessStat_4 = 2;
        p2sdStopBGM(0x2B0000);
    }
}

extern evtStartWork_t *rstInit(s32 mode);
extern s32 rstCalc(dds3ProcessID_t *id);
extern s32 rstDraw(dds3ProcessID_t *id);
extern s32 rstUpdate(dds3ProcessID_t *id);
extern void rstDestroyResult2(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *work);

void rstResult2ProcessStart(void)
{
    if (GBWK_1 != NULL) {
        return;
    }
    GBWK_1 = rstInit(0);
    dds3StartProcess(D_0054FCF0, 0x405, 1, 1, rstCalc, NULL, GBWK_1);
    dds3StartProcess(D_0053DA90, 0x2B15, 1, 1, rstDraw, NULL, GBWK_1);
    dds3StartProcess(D_0053DAA0, 0x5211, 1, 1, rstUpdate,
                     (dds3ProcessFunc_t)rstDestroyResult2, GBWK_1);
    gProcessStat_4 = 1;
}

void rstResult2ProcessEnd(void)
{
    dds3KillProcessName(D_0054FCF0, 1);
    dds3KillProcessName(D_0053DA90, 1);
    dds3KillProcessName(D_0053DAA0, 1);
}

s32 checkResultStat(void)
{
    if (gProcessStat_4 == 1) {
        return 1;
    }
    if (gProcessStat_4 < 2) {
        return 0;
    }
    if (gProcessStat_4 == 2) {
        gProcessStat_4 = 0;
    }
    return 0;
}

void rstSetLevelUpFlag(void)
{
    if (GBWK_1 != NULL) {
        GBWK_1->flags |= 0x10;
    }
}

void rstAddExp(s32 amount, datUnitWork_t *work)
{
    if (work->level < 0xFF) {
        work->level += amount;
    }
}

void rstAdvanceLevelUpStep(void)
{
    evtStartWork_t *work = EventStart;
    s8 step;

    if (work->levelUpCount == 0) {
        return;
    }
    step = work->levelUpStep;
    if (step >= work->levelUpCount) {
        return;
    }
    work->levelUpStep++;
    rstPrepareStatusModel(6, work->levelUpUnit[step]->id, 0);
}

s32 func_0021FE18(void)
{
    return EventStart->unk_22E5 >= 0;
}

void func_0021FE30(s8 a, s8 b, f32 x, f32 y)
{
    EventStart->unk_22E5 = a;
    EventStart->unk_22E8 = x;
    EventStart->unk_22EC = y;
    EventStart->unk_22E6 = b;
}

INCLUDE_ASM(const s32, "result/rstinit", rstDrawEvoModel);

void rstSetEvoState(void)
{
    EventStart->unk_22E6 = 2;
}

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA90);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DAA0);

INCLUDE_SDATA(const s32, "result/rstinit", GBWK_1);

INCLUDE_SDATA(const s32, "result/rstinit", gProcessStat_4);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCED);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF0);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF8);

