#include "common.h"
#include "kernel.h"
#include "scr.h"
#include "event.h"

extern u32 abcd;
extern u32 dds3KernelDebugSign;
extern s32 ChkPipe(void);
extern void EndPipe(void);
extern void FinishMovieViewer(void);
extern void InitMovieViewer(void);
extern s32 campCheckExitState(void);
extern void campDebugRegistAll(void);
extern void campKillProcesses(void);
extern s32 checkConfigState(void);
extern s32 checkResultStat(void);
extern void cmpProcessStart(void);
extern void dds3ConfigProcessEnd(void);
extern void dds3ConfigProcessStart(s32 Mode);
extern void evtChkEndProcess(void);
extern void evtExitSky(void);
extern void evtFreeStage(void);
extern void evtInitEventTest(void);
extern void evtInitEventViewer(void);
extern void fclCombineProcessEnd(void);
extern void fclCombineProcessStart(void);
extern void fclRagProcessEnd(void);
extern void fclRecoverProcessEnd(void);
extern void fclRecoverProcessStart(s32 Place);
extern void fclShopProcessEnd(void);
extern void fclShopProcessStart(void *pPlace);
extern void fclTerminalProcessEnd(void);
extern void fldProc_CallSisetuReturn(void);
extern void fmProcTrigSequence(s32 mode);
extern s32 fmProcessChk(void);
extern void frTestFontInit(void);
extern void func_0022C658(void);
extern void itfDebugMesMngInitialize(void);
extern s32 mselExit(u32 mode);
extern void mselInit(void);
extern void nbExitBattleTest(void);
extern s32 nmeCheckProcessStat(void);
extern void nmeNameProcessEnd(void);
extern s32 ragCheckProcessStat(void);
extern s32 rcvCheckProcessStat(void);
extern void rstResult2ProcessEnd(void);
extern void rstResult2ProcessStart(void);
extern void rstSetTestParam(void);
extern s32 shpCheckProcessStat(void);
extern void stfExit(void);
extern void stfInit(void);
extern void titleExit(void);
extern void titleStart(u32 mode);
extern s32 trmCheckProcessStat(void);

#include "sdk_libc.h"

extern const char D_0052E230[];
extern void fldProcTrigSequence(void *srcParam, u32 mode);
extern void nmeStartProcesses(s32 arg0, s32 arg1, s32 arg2);
extern void fclRagProcessStart(s32 Place);
extern void pbStartMapEditor(s32 mode, s32 stage);
extern void evtKillEventViewer(void);

extern s32 cmbCheckProcessStat(void);
extern void InitPipe(s32 data, s32 prevData);
extern void dds3AdminiExitSequence(void);
extern void nbInitBattleTest(void);
extern void nbCallNewBattle(s32 encpack, void *data);
extern s32 evtLoadArea(s32 fld, s32 area);
extern s32 evtLoadUnit(s32 major, s32 minor);
extern evtUnit_t *evtGetUnitHandle(s32 serial);
extern void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);
extern void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                                f32 aSmoothLeng);
extern s32 evtNextEventNo;
extern s32 fldGb __attribute__((section(".data")));
extern void *sdfChipAllocDirect(u32 aSize);
extern s32 itfMesMngInitialize(void *sub);


extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;
extern s32 scrHandleCount;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);
extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);
extern dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);
extern s32 dds3StartMenuProcess(dds3ProcessID_t *id);
extern void func_00157050(void);
extern void func_00157058(void);
extern s32 fmShutDown2(void);
extern void fldMainLoadStart(void);
extern void fldAllShutDown(void);
extern s32 fldProcShutDown2(void);
extern void fldProcNextJump(void);
extern dds3ProcessID_t *evtInitSky(void);
extern void fclTerminalProcessStart(s32 TerminalType, s32 TerminalNo);
extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                                scrProcTable_t *proctable,
                                                scrLabelTable_t *labeltable, scrCode_u *code,
                                                u8 *mesdata, char *strdata, s32 procno);

dds3ProcessID_t *CallDebug(void)
{
    return dds3StartProcess("StartSel", 0x2AF9, 0, 0, dds3StartMenuProcess, NULL, NULL);
}

s32 ExitDebug(void)
{
    dds3KillProcessName("StartSel", 1);
    return 1;
}

s32 CheckDebug(void)
{
    return 0;
}

void CallBootMc(void)
{
    fmProcTrigSequence(3);
}

s32 ExitBootMc(void)
{
    s32 done;
    s32 ret;

    func_00157050();
    func_00157058();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckBootMc(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallTitle(u32 flg, u32 *data)
{
    if (data == 0) {
        titleStart(0);
        return;
    }
    titleStart(*data);
}

s32 ExitTitle(void)
{
    titleExit();
    return 0;
}

s32 CheckTitle(void)
{
    return 0;
}

void CallLoad(void)
{
    fmProcTrigSequence(1);
}

s32 ExitLoad(void)
{
    func_00157050();
    func_00157058();
    if (fmShutDown2() != 0) {
        fldMainLoadStart();
        return 0;
    }
    return -1;
}

s32 CheckLoad_dds3SequenceList(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallLoad2(void) {
}

s32 ExitLoad2(void)
{
    fldMainLoadStart();
    return 0;
}

s32 CheckLoad2(void)
{
    return 1;
}

void CallStaff(void)
{
    stfExit();
}

s32 ExitStaff(void)
{
    stfInit();
    return 0;
}

s32 CheckStaff(void)
{
    return 0;
}

void CallEndSave(void)
{
    fmProcTrigSequence(2);
}

s32 ExitEndSave(void)
{
    s32 done;
    s32 ret;

    func_00157050();
    func_00157058();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckEndSave(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallField(u32 flg, void *data)
{
    fldProcTrigSequence(data, flg);
}

s32 ExitField(void)
{
    s32 ret;

    fldAllShutDown();
    ret = -1;
    if (fldProcShutDown2() != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckField(void)
{
    return 0;
}

void CallNewBattle(void) {
}

s32 ExitNewBattle(void)
{
    return 0;
}

s32 CheckNewBattle(void)
{
    return 0;
}

void CallEvent(u32 flg, s32 *data)
{
    s32 eventNo = 0;

    switch (flg) {
    case 0:
        printf("start event!\n");
        eventNo = *data;
        break;
    case 1:
        printf(D_0052E230);
        eventNo = evtNextEventNo;
        break;
    }
    if (eventNo > 0) {
        evtInitSky();
        evtStartEvent(eventNo);
    }
}

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_0052E230);

s32 ExitEvent(void)
{
    printf("ExitEvent\n");
    evtExitSky();
    fldProcNextJump();
    return 0;
}

s32 CheckEvent(void)
{
    return 0;
}

void CallCamp(void)
{
    cmpProcessStart();
}

s32 ExitCamp(void)
{
    campKillProcesses();
    return 0;
}

s32 CheckCamp(void)
{
    s32 campState;

    campState = campCheckExitState();
    return campState == 0;
}

void CallCombine(void) {
}

s32 ExitCombine(void)
{
    return 0;
}

s32 CheckCombine(void)
{
    return 1;
}

void CallShop(void) {
}

s32 ExitShop(void)
{
    return 0;
}

s32 CheckShop(void)
{
    return 1;
}

void CallRag(void) {
}

s32 ExitRag(void)
{
    return 0;
}

s32 CheckRag(void)
{
    return 1;
}

void CallRecover(void) {
}

s32 ExitRecover(void)
{
    return 0;
}

s32 CheckRecover(void)
{
    return 1;
}

void CallTerminal(void) {
}

s32 ExitTerminal(void)
{
    return 0;
}

s32 CheckTerminal(void)
{
    return 1;
}

void CallResult2(void)
{
    rstResult2ProcessStart();
}

s32 ExitResult2(void)
{
    fldProcNextJump();
    rstResult2ProcessEnd();
    return 0;
}

s32 CheckResult2(void)
{
    s32 taskResult;

    taskResult = checkResultStat();
    return taskResult == 0;
}

void CallConfig(void)
{
    dds3ConfigProcessStart(1);
}

s32 ExitConfig(void)
{
    dds3ConfigProcessEnd();
    return 0;
}

s32 CheckConfig(void)
{
    s32 taskResult;

    taskResult = checkConfigState();
    return taskResult == 0;
}

void CallNewBattleTest(u32 flg, s32 *data)
{
    if (flg == 0) {
        if (data == NULL) {
            nbInitBattleTest();
            return;
        }
        nbCallNewBattle(data[0], (void *)data[1]);
        return;
    }
    dds3AdminiExitSequence();
    fldProcNextJump();
}

s32 ExitNewBattleTest(void)
{
    nbExitBattleTest();
    return 0;
}

s32 CheckNewBattleTest(void)
{
    if (dds3SearchProcessName("battle test") != NULL) {
        return 0;
    }
    if (dds3SearchProcessPriority(0x3FA) == NULL) {
        fldProcNextJump();
        return 1;
    }
    return 0;
}

void CallBattleFieldTest(void) {
}

s32 ExitBattleFieldTest(void)
{
    return 0;
}

s32 CheckBattleFieldTest(void)
{
    return 0;
}

void CallKernelTest(void)
{
    evtUnit_t *unit;

    evtLoadArea(0x6C, 1);
    unit = evtGetUnitHandle(evtLoadUnit(3, 3));
    evtSetUnitMotion(unit, 1, 0, 0x5A, 0);
    mdlSmoothLoopMotion(unit->owner, 1, 0, 0.0f, 0.0f);
}

s32 ExitKernelTest(void)
{
    return 0;
}

s32 CheckKernelTest(void)
{
    return 0;
}

void CallEventTest(void)
{
    abcd = 0;
    evtInitEventTest();
    evtInitSky();
}

s32 ExitEventTest(void)
{
    evtChkEndProcess();
    evtExitSky();
    return 1;
}

s32 CheckEventTest(void)
{
    return 0;
}

void CallEventViewer(void)
{
    dds3KernelDebugSign = 0;
    evtFreeStage();
    evtInitEventViewer();
}

s32 ExitEventViewer(void)
{
    evtKillEventViewer();
    dds3KernelDebugSign = 1;
    return 0;
}

s32 CheckEventViewer(void)
{
    return 0;
}

void CallInterfaceTest(void) {
}

s32 ExitInterfaceTest(void)
{
    return 0;
}

s32 CheckInterfaceTest(void)
{
    return 0;
}

void CallMessageScriptTest(void)
{
    itfDebugMesMngInitialize();
}

s32 ExitMessageScriptTest(void)
{
    return 0;
}

s32 CheckMessageScriptTest(void)
{
    return 0;
}

void CallCampTest(void)
{
    campDebugRegistAll();
    func_0022C658();
    cmpProcessStart();
}

s32 ExitCampTest(void)
{
    campKillProcesses();
    return 0;
}

s32 CheckCampTest(void)
{
    s32 sceneState;

    sceneState = campCheckExitState();
    return sceneState == 0;
}

void CallCombineTest(void)
{
    fclCombineProcessStart();
}

s32 ExitCombineTest(void)
{
    fclCombineProcessEnd();
    return 0;
}

s32 CheckCombineTest(void)
{
    if (!cmbCheckProcessStat()) {
        fldProc_CallSisetuReturn();
        return 1;
    }
    return 0;
}

void CallShopTest(s32 flg, void *data)
{
    fclShopProcessStart(data);
}

s32 ExitShopTest(void)
{
    fclShopProcessEnd();
    fldProc_CallSisetuReturn();
    return 0;
}

s32 CheckShopTest(void)
{
    s32 taskExists;

    taskExists = shpCheckProcessStat();
    return taskExists == 0;
}

void CallRagTest(void)
{
    evtInitSky();
    fclRagProcessStart(0);
}

s32 ExitRagTest(void)
{
    evtExitSky();
    fclRagProcessEnd();
    fldProc_CallSisetuReturn();
    return 0;
}

s32 CheckRagTest(void)
{
    s32 taskState;

    taskState = ragCheckProcessStat();
    return taskState == 0;
}

void CallRecoverTest(void)
{
    fclRecoverProcessStart(0);
}

s32 ExitRecoverTest(void)
{
    fclRecoverProcessEnd();
    fldProc_CallSisetuReturn();
    return 0;
}

s32 CheckRecoverTest(void)
{
    s32 taskState;

    taskState = rcvCheckProcessStat();
    return taskState == 0;
}

void CallTerminalTest(dds3ProcessID_t *flg, s32 *data)
{
    evtInitSky();
    if (data != NULL) {
        fclTerminalProcessStart(data[0], data[1]);
        return;
    }
    fclTerminalProcessStart(0, 1);
}

s32 ExitTerminalTest(void)
{
    evtExitSky();
    fclTerminalProcessEnd();
    return 0;
}

s32 CheckTerminalTest(void)
{
    s32 sceneState;

    sceneState = trmCheckProcessStat();
    return sceneState == 0;
}

void CallResult2Test(void)
{
    rstSetTestParam();
    rstResult2ProcessStart();
}

s32 ExitResult2Test(void)
{
    rstResult2ProcessEnd();
    return 0;
}

s32 CheckResult2Test(void)
{
    s32 taskResult;

    taskResult = checkResultStat();
    return taskResult == 0;
}

void CallNameTest(void)
{
    nmeStartProcesses(0, 0, 1);
}

s32 ExitNameTest(void)
{
    nmeNameProcessEnd();
    return 0;
}

s32 CheckNameTest(void)
{
    s32 taskResult;

    taskResult = nmeCheckProcessStat();
    return taskResult == 0;
}

void CallModelViewerTest(void) {
}

s32 ExitModelViewerTest(void)
{
    return 0;
}

s32 CheckModelViewerTest(void)
{
    return 0;
}

void CallSoundTest(void) {
}

s32 ExitSoundTest(void)
{
    return 0;
}

s32 CheckSoundTest(void)
{
    return 0;
}

void CallBillboardTest(void) {
}

s32 ExitBillboardTest(void)
{
    return 0;
}

s32 CheckBillboardTest(void)
{
    return 0;
}

void CallFontTest(void)
{
    frTestFontInit();
}

s32 ExitFontTest(void)
{
    return 0;
}

s32 CheckFontTest(void)
{
    return dds3SearchProcessName("test_font") == NULL;
}

void CallMapEditor(void)
{
    pbStartMapEditor(0, 0);
}

s32 ExitMapEditor(void)
{
    return 0;
}

s32 CheckMapEditor(void)
{
    return 0;
}

void CallPuzzleBoy(u32 flg, s32 *data)
{
    s32 stage = -1;

    if (data != NULL) {
        stage = *data;
    }
    pbStartMapEditor(1, stage);
}

s32 ExitPuzzleBoy(void)
{
    return 0;
}

s32 CheckPuzzleBoy(void)
{
    return 0;
}

void CallMovieViewerTest(void)
{
    InitMovieViewer();
}

s32 ExitMovieViewerTest(void)
{
    FinishMovieViewer();
    return 0;
}

s32 CheckMovieViewerTest(void)
{
    return 0;
}

void CallPipe(u32 flg, s32 *data)
{
    if (data == NULL) {
        InitPipe(0, 0);
        return;
    }
    InitPipe(data[0], data[1]);
    fldGb = 0x3E7;
}

s32 ExitPipe(void)
{
    EndPipe();
    fldProcNextJump();
    return 0;
}

s32 CheckPipe(void)
{
    s32 sceneState;

    sceneState = ChkPipe();
    return sceneState == 0;
}

void CallModeSelect(u32 flg, u32 *data)
{
    if (data == 0) {
        mselExit(0);
        return;
    }
    mselExit(*data);
}

s32 ExitModeSelect(void)
{
    mselInit();
    return 0;
}

s32 CheckModeSelect(void)
{
    return 0;
}

void CallTgs(void) {
}

s32 ExitTgs(void)
{
    return 0;
}

s32 CheckTgs(void)
{
    return 0;
}

s32 dat_1097D8(void)
{
    return 0;
}

void func_001097E0(void) {
}

void datDebugPrintf(const char *format, ...)
{
}

s32 scr_109830(void)
{
    return 0;
}

void func_00109838(void) {
}

void func_00109840(void) {
}

void scrLinkHandle(scrProcessWork_s *work)
{
    scrProcessWork_s *last = scrHandleLast;

    if (last == NULL) {
        scrHandleFirst = work;
        scrHandleLast = work;
        work->prev = NULL;
        work->next = NULL;
    } else {
        work->prev = last;
        last->next = work;
        work->next = NULL;
        scrHandleLast = work;
    }
    scrHandleCount++;
}

void scrUnlinkHandle(scrProcessWork_s *work)
{
    if (scrHandleFirst == work) {
        scrHandleFirst = work->next;
    } else {
        work->prev->next = work->next;
    }
    if (scrHandleLast == work) {
        scrHandleLast = work->prev;
    } else {
        work->next->prev = work->prev;
    }
    work->prev = NULL;
    work->next = NULL;
    scrHandleCount--;
}

scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                         scrProcTable_t *proctable, scrLabelTable_t *labeltable,
                                         scrCode_u *code, u8 *mesdata, char *strdata, s32 procno)
{
    scrProcessWork_s *work;
    s32 i;

    if (header == NULL || typetable == NULL || proctable == NULL || code == NULL || procno < 0 ||
        procno >= typetable->ItemCount) {
        return NULL;
    }
    work = sdfChipAllocDirect(sizeof(scrProcessWork_s));
    if (work == NULL) {
        return NULL;
    }
    for (i = 0; (((char *)work->Name)[i] = proctable[procno].Name[i]) != 0; i++) {
    }
    work->CP = proctable[procno].CodeAddress;
    work->SP = 0;
    {
        scrStackData_u *stackValue = work->StackData;
        u8 *stackType = work->StackType;

        for (i = 27; i >= 0; i--) {
            *stackType++ = 0;
            stackValue++->i = 0;
        }
    }
    work->Header = header;
    work->TypeTable = typetable;
    work->ProcTable = proctable;
    work->LabelTable = labeltable;
    work->Code = code;
    work->MessageData = mesdata;
    work->StringData = (u8 *)strdata;
    work->StartProc = procno;
    work->MesHandle = -1;
    work->ScriptTimer = 0;
    work->CommandTimer = 0;
    work->MemHandle = NULL;
    work->LocalInt = NULL;
    work->LocalFloat = NULL;
    work->ProcessID = NULL;
    work->prev = NULL;
    work->next = NULL;
    if (header->LocalIntCount > 0) {
        work->LocalInt = sdfChipAllocDirect(header->LocalIntCount * 4);
        for (i = 0; i < header->LocalIntCount; i++) {
            work->LocalInt[i] = 0;
        }
    } else {
        work->LocalInt = NULL;
    }
    if (header->LocalFloatCount > 0) {
        work->LocalFloat = sdfChipAllocDirect(header->LocalFloatCount * 4);
        for (i = 0; i < header->LocalFloatCount; i++) {
            work->LocalFloat[i] = 0.0f;
        }
    } else {
        work->LocalFloat = NULL;
    }
    if (mesdata != NULL) {
        work->MesHandle = itfMesMngInitialize(mesdata);
    }
    scrLinkHandle(work);
    printf("procedure start <%s>\n", proctable[procno].Name);
    datDebugPrintf("start <%s>\n", proctable[procno].Name);
    return work;
}

scrProcessWork_s *fldFlowParse(scrHeader_t *header, s32 procno)
{
    scrTypeTable_t *typetable;
    scrProcTable_t *proctable = NULL;
    scrLabelTable_t *labeltable = NULL;
    scrCode_u *code = NULL;
    u8 *mesdata = NULL;
    char *strdata = NULL;
    s32 i;

    typetable = (scrTypeTable_t *)&header[1];
    if (*(s32 *)header->MagicCode != 0x30574C46) {
        return NULL;
    }
    for (i = 0; i < header->TypeTableCount; i++) {
        switch (typetable[i].Type) {
        case SCR_DATATYPE_PROCTABLE:
            proctable = (scrProcTable_t *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_LABELTABLE:
            labeltable = (scrLabelTable_t *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_SCRDATA:
            code = (scrCode_u *)((u8 *)header + typetable[i].ItemAddress);
            break;
        case SCR_DATATYPE_MESSAGE:
            if (typetable[i].ItemCount != 0) {
                mesdata = (u8 *)header + typetable[i].ItemAddress;
            }
            break;
        case SCR_DATATYPE_STRINGS:
            strdata = (char *)header + typetable[i].ItemAddress;
            break;
        default:
            return NULL;
        }
    }
    return scrPrintProcedureStart(header, typetable, proctable, labeltable, code, mesdata, strdata,
                                  procno);
}

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", abcd);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleCount);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleFirst);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", scrHandleLast);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_0054E9A0);

