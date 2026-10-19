#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

extern void *dds3GetProcessData(dds3ProcessID_t *process);

extern void nbKillMainProcess(void);

extern void nbCameraPressProcessInit(void);

void SetDummy(void) {
}

nbMainProcessData_t *nbGetMainProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessPriority(0x3FA));
}

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbGetUnitWorkPointer);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbGetPartyFromFormindex);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbGetUnitWorkFromFormindex);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbGetSelectBit);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetFormationSideFlag);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbDebugSelect);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbDebugCamera);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbDebugMainProcess);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", DebugDisp2);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", CheckActionProcessEnd);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", CheckSenseiOrBackAttack);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", CheckActionRun);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", SetBattleFlag);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbPushAction);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", CheckErrorActionStack);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbPopAction);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbCheckActionStack);

void nbClearActionStack(void)
{
    nbGetMainProcessData()->actionstackp = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetMaePhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbStartBattlePhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetEventPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetAtoPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetPressMaePhase);

void nbSetPressNiidaPhase(void)
{
    nbCameraPressProcessInit();
}

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetActionMaePhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetActionPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetChkEndPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetChkTurnPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetResultPhase);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543448);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543458);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetEndPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetDebugPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetNextPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbSetPhase);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbMainProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", SetJyocyuData);

void FreeJyocyuData(nbMainProcessData_t *work)
{
    effBTLDelete(work->residentEffect);
    work->residentEffect = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", ClearLocalBit);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbMainShutdown);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", SetDebugFlag);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbInitMainProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", SetFlag);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543510);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543520);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543530);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543540);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbInitMainProcess);

INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbKillMainProcess);
