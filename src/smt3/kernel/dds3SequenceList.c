#include "common.h"
#include "kernel.h"
#include "scr.h"

extern char D_0052E210[]; /* "StartSel" */
extern char D_0052E250[]; /* "battle test" */
extern char D_0052E260[]; /* "test_font" */

extern scrProcessWork_s *scrHandleFirst;
extern scrProcessWork_s *scrHandleLast;
extern s32 scrHandleCount;

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);
extern s32 dds3StartMenuProcess(dds3ProcessID_t *id);
extern void func_00157050(void);
extern void func_00157058(void);
extern s32 fmShutDown2(void);
extern void fldMainLoadStart(void);
extern void fldAllShutDown(void);
extern s32 fldProcShutDown2(void);
extern void fldProcNextJump(void);
extern void evtInitSky(void);
extern void fclTerminalProcessStart(s32 arg0, s32 arg1);
extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                                scrProcTable_t *proctable,
                                                scrLabelTable_t *labeltable, scrCode_u *code,
                                                u8 *mesdata, char *strdata, s32 procno);

dds3ProcessID_t *CallDebug(void)
{
    return dds3StartProcess(D_0052E210, 0x2AF9, 0, 0, dds3StartMenuProcess, NULL, NULL);
}

s32 ExitDebug(void)
{
    dds3KillProcessName(D_0052E210, 1);
    return 1;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckDebug);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallBootMc);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckBootMc);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallTitle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitTitle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckTitle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallLoad);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckLoad_dds3SequenceList);

void CallLoad2(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitLoad2);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckLoad2);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallStaff);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitStaff);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckStaff);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEndSave);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEndSave);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallField);

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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckField);

void CallNewBattle(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattle);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckNewBattle);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_0052E210);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEvent);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_0052E230);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitEvent);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEvent);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallCamp);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitCamp);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckCamp);

void CallCombine(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitCombine);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckCombine);

void CallShop(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitShop);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckShop);

void CallRag(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitRag);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckRag);

void CallRecover(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitRecover);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckRecover);

void CallTerminal(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitTerminal);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckTerminal);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallResult2);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitResult2);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckResult2);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallConfig);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitConfig);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckConfig);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallNewBattleTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNewBattleTest);

s32 CheckNewBattleTest(void)
{
    if (dds3SearchProcessName(D_0052E250) != NULL) {
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

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitBattleFieldTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckBattleFieldTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallKernelTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitKernelTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckKernelTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEventTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitEventTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEventTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallEventViewer);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitEventViewer);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckEventViewer);

void CallInterfaceTest(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitInterfaceTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckInterfaceTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallMessageScriptTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitMessageScriptTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckMessageScriptTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallCampTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitCampTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckCampTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallCombineTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitCombineTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckCombineTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallShopTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitShopTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckShopTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallRagTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitRagTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckRagTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallRecoverTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitRecoverTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckRecoverTest);

void CallTerminalTest(dds3ProcessID_t *id, s32 *args)
{
    evtInitSky();
    if (args != NULL) {
        fclTerminalProcessStart(args[0], args[1]);
        return;
    }
    fclTerminalProcessStart(0, 1);
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitTerminalTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckTerminalTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallResult2Test);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitResult2Test);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckResult2Test);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallNameTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitNameTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckNameTest);

void CallModelViewerTest(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitModelViewerTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckModelViewerTest);

void CallSoundTest(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitSoundTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckSoundTest);

void CallBillboardTest(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitBillboardTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckBillboardTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallFontTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitFontTest);

s32 CheckFontTest(void)
{
    return dds3SearchProcessName(D_0052E260) == NULL;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallMapEditor);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitMapEditor);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckMapEditor);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallPuzzleBoy);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitPuzzleBoy);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckPuzzleBoy);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallMovieViewerTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitMovieViewerTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckMovieViewerTest);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallPipe);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitPipe);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckPipe);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallModeSelect);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitModeSelect);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckModeSelect);

void CallTgs(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitTgs);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CheckTgs);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", dat_1097D8);

void func_001097E0(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", datDebugPrintf);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", scr_109830);

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

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_0052E250);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_0052E260);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", scrPrintProcedureStart);

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

