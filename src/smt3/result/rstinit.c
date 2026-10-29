#include "common.h"
#include "result2.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern void *dds3SearchProcessName(const char *namestr);

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);



extern rstData_t *EventStart;





extern rstData_t *GBWK_1;

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
extern s32 cmpGetEncycWork(void);
extern void mdlSmoothSingleMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);
extern void mdlSmoothLoopMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);

void rstInitEvoAct(void)
{
    f32 modelParam[3];

    fclClearFlag(0);
    fclClearFlag(1);
    fclSetWorkValue(2, 1);
    fclSetWorkValue(3, 1);
    fclSetWorkValue(4, GBWK_1->unit->id);
    fclSetWorkValue(5, GBWK_1->evoWork.id);
    rstGetEvoModelParam(GBWK_1->evoWork.id, modelParam);
    modelParam[1] *= datModelParamTable[GBWK_1->evoWork.id].formscale;
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
    DDS3_GBWK->party[0].unit.exp += 100;
    DDS3_GBWK->party[4].unit.exp += DDS3_GBWK->party[4].unit.level * 100;
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
        if (rstChkLevelUpTarget(&DDS3_GBWK->party[i].unit) != 0)
        {
            return 1;
        }
    }
    return 0;
}

extern s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp);

u8 rstCalcLevelFromExp(u8 Index, u32 Exp)
{
    datUnitWork_t *unit = &DDS3_GBWK->party[Index].unit;

    return (u8)unit->level + cmbGetLevelUpCount(unit, Exp - unit->exp);
}

s32 rstCalcExpFromLevel(u8 Index, s32 Level)
{
    return datCopyDevilWorkEntry(&DDS3_GBWK->party[Index].unit, Level);
}

void rstChkResult2Process(u8 *work)
{
    memset(&work[0x129F], 0, 6);
}

extern void rstPrepareStatusModel(s32 group, s32 id, s32 blocking);

void rstCreateTargetList(rstData_t *work)
{
    s32 useFirstUnit = 1;
    s32 i;

    if (rstChkLevelUpTarget(&DDS3_GBWK->party[0].unit) != 0) {
        rstPrepareStatusModel(6, 0, 1);
        useFirstUnit = 0;
    }

    work->levelUpCount = 0;
    work->levelUpStep = 0;
    i = 1;
    while (i < DDS3_GBWK->stockCount) {
        u8 stockIndex = (u8)DDS3_GBWK->stock[i];
        datUnitWork_t *unit = &DDS3_GBWK->party[stockIndex].unit;
        i++;

        if (rstChkLevelUpTarget(unit) != 0) {
            work->levelUpUnit[work->levelUpCount++] = unit;
        }
    }

    if (useFirstUnit != 0 && work->levelUpCount != 0) {
        datUnitWork_t *unit = work->levelUpUnit[work->levelUpStep++];
        rstPrepareStatusModel(6, unit->id, 1);
    }
}

/* Allocations, cursors and the texture are installed by rstInit. */


extern char D_0053DA40[]; /* "/result2/scr/message.bf" */
extern char D_0053DA58[]; /* "/result2/mes_data.bmd" */
extern char D_0053DA70[]; /* "/result2/tmx/lvup_02.tmx" */

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void cmpInitSeqInfo(void *work, void *parent);
extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);
extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, s32 layout);
extern void cmpModelInit(void);
extern void rstClearHeartsMasterPhase(void);
extern void *sdfDevLoadFileEx(char *name, void **data, s32 *size);
extern void fclInitMessage(void *message);
extern void fclSetSelMessagePos(s32 pos);
extern void cmpStartProcessPanelEff(s32 mode);
extern void rstSetStatusModelReady(s32 ready);
extern sdfTexHandle_t *fclLoadTexture(char *pFileName);
extern void rstSetResultWork(sdfTexHandle_t *texture);
extern void dds3FadeIn(s32 a, s32 b, s32 c, s32 frames);
extern s32 p2sdPlayBGM(s32 id);

rstData_t *rstInit(s32 parent)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(0x2318);
    rstData_t *work = sdfMemLock(allocation);

    memset(work, 0, 0x2318);
    work->allocation = allocation;
    cmpInitSeqInfo(work, (void *)parent);
    work->unk_0001 = 0;
    work->unk_22E5 = -1;
    work->unk_22E4 = -0x80;
    work->unk_0004 = 1;
    work->unk_0072 = 1;
    work->unk_0003 = 0;
    work->unk_001B = 0;
    cmpInitCursorLayout(&work->paramCursor, 0, 0, 0, 0, 0);
    cmpInitCursorRange(&work->paramCursor, 5, 5, 0);
    cmpInitCursorRange(&work->cursorInfo, 8, 8, 0);
    cmpModelInit();
    rstChkResult2Process((u8 *)work);
    rstClearHeartsMasterPhase();
    work->scriptAllocation = sdfDevLoadFileEx(D_0053DA40, &work->script, 0);
    work->messageAllocation = sdfDevLoadFileEx(D_0053DA58, &work->message, 0);
    fclInitMessage(work->message);
    fclSetSelMessagePos(0);
    cmpStartProcessPanelEff(0);
    rstSetStatusModelReady(1);
    work->texture = fclLoadTexture(D_0053DA70);
    rstSetResultWork(work->texture);
    rstCreateTargetList(work);
    work->unk_2310 = 0x2B3;
    dds3FadeIn(0, 0, 0, 0xF);
    p2sdPlayBGM(0x2B0000);
    return work;
}


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
        sdfTexFree(GBWK_1->texture);
        fclDestroyMessage();
        sdfMemFreeDirect(GBWK_1->messageAllocation);
        cmpModelFreeResource();
        cmpResetDrawEnv();
        sdfMemFreeDirect(GBWK_1->scriptAllocation);
        rstSetPanelEffFlag();
        rstSetStatusModelReady(0);
        sdfMemFreeDirect(GBWK_1->allocation);
        GBWK_1 = NULL;
        gProcessStat_4 = 2;
        p2sdStopBGM(0x2B0000);
    }
}

extern rstData_t *rstInit(s32 mode);
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
    rstData_t *work = EventStart;
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

void rstDrawEvoModel(void)
{
    mdlObj_t *model;
    s8 mode;

    /* The native query precedes the state guard; fetch the model again below. */
    cmpGetEncycWork();
    if (EventStart->unk_22E5 < 0 || EventStart->unit->id == 0) {
        return;
    }
    model = (mdlObj_t *)cmpGetEncycWork();
    if (model == NULL) {
        return;
    }
    mode = EventStart->unk_22E6;
    if (mode == 0) {
        mdlSmoothSingleMotion(model, 0, EventStart->unk_22E5,
                              EventStart->unk_22E8, EventStart->unk_22E8);
        func_0021FE30(0xD, 1, EventStart->unk_22EC, 0.0f);
        return;
    }
    if (model->first->stat == 5 || mode == 2) {
        mdlSmoothLoopMotion(model, 0, EventStart->unk_22E5,
                            EventStart->unk_22E8, EventStart->unk_22E8);
        func_0021FE30(-1, 0, 0.0f, 0.0f);
        return;
    }
}

void rstSetEvoState(void)
{
    EventStart->unk_22E6 = 2;
}

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA28);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA40);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA58);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA70);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA90);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DAA0);

INCLUDE_SDATA(const s32, "result/rstinit", GBWK_1);

INCLUDE_SDATA(const s32, "result/rstinit", gProcessStat_4);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCED);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF0);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF8);

