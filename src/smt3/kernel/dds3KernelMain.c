#include "common.h"
#include "kernel.h"
#include "sdfFrame.h"

extern u32 dds3FrameCount;
extern u32 dds3FrameID;
extern u32 dds3RunCount;
extern s8 dds3SoftResetFlag;
extern u8 dds3SoftResetDisableFlag;
extern u32 dds3PauseSign;
extern u32 dds3PauseTime;
extern u32 dds3SlowSign;
extern u32 dds3SlowTime;
extern u32 dds3SlowRunCount;
extern u32 dds3SlowEndCount;
extern u16 dds3VblanksPerFrame;
extern s16 dds3FrameRunTime;
extern s16 dds3LastDrawFrameCount;
extern s8 dds3FileCmdLoPrio;
extern u16 sdfReqDrawDelaySleepCount;
extern u16 sdfDrawFrameCounter;
extern char dds3HardResetMessage[]; /* "hard reset !!\n" */
extern char dds3ResetExecPath[]; /* "cdrom0:\\SLPS_999.99;1" */
extern char dds3FirstProcessName[]; /* "1st" */
extern char dds3DrawFirstProcessName[]; /* "draw1st" */
extern char dds3DrawProcessName[]; /* "RequestDraw" */
extern char dds3BasicMainProcessName[]; /* "basic_main" */
extern char dds3BasicDrawProcessName[]; /* "basic_draw" */
extern char dds3FontSysProcessName[]; /* "font_sys" */
extern char dds3DebugDevDumpProcessName[]; /* "DbgDevDump" */
extern void dds3InitDrawEnv(void);
extern void dds3PadInitialize(void);
extern dds3ProcessID_t *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB,
    s32 v2, s32 v3, dds3ProcessFunc_t drawExit,
    dds3ProcessFunc_t deleteExit, s32 p);
extern s32 dds3FirstProcess(dds3ProcessID_t *id);
extern s32 dds3DrawFirstProcess(dds3ProcessID_t *id);
extern s32 dds3DrawProcess(dds3ProcessID_t *id);
extern s32 dds3BasicMainProcess(dds3ProcessID_t *id);
extern s32 dds3BasicDrawProcess(dds3ProcessID_t *id);
extern s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id);
extern s32 frFlipPacketBuf(dds3ProcessID_t *id);
extern void frDestroyFontTable(dds3ProcessID_t *id);
extern void frInitFontTable(s32 GsFontNums, s32 TextNums);
extern void fileSysInit(void);
extern void itfMesMngInitializeManager(void);
extern void sndInit(void);
extern void sdfDevConsInit(void);
extern void mdlInitManager(void);
extern void dds3EffectManagerInit(void);
extern void fldFirstInit(void);
extern void nbPnlLoad(void);
extern void dds3LoadBootFile(void);
extern void effBTLSystemInit(void);
extern void dds3StartMovieManager(void);
extern void evtPicturePriProcessInit(void);
extern void dds3GlobalWorkInit(void);
extern void dds3AddWorld(s32 bufcnt);
extern void dds3InitRandCB(void);
extern void dds3InitAdmini(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);
extern void dds3InitDebugSystem(void);
extern void cmpTexLoad(void);
extern void dds3FadeClear(void);
extern void evtInitMoonProcess(void);
extern void encInitProcess(void);
extern u32 sdfGetRunTimeCountBase(void);
extern u32 sdfGetRunTimeCount(u32 aBaseTime);
extern void sdfWaitVblank(s32 count);
extern void sdfGrReqDrawDelay(s32 count);
extern void sdfPadWork(void);
extern void sdfFileCmdThreadSetHiPrio(void);
extern void sdfFileCmdThreadSetLoPrio(void);
extern void dds3RunProcessList(void);
extern s32 dds3ProcessMain(void);
extern void LoadExecPS2(const char *path, s32 argc, char **argv);
#include "sdk_libc.h"

void dds3FirstInit(void)
{
    dds3InitDrawEnv();
    dds3PadInitialize();
    dds3StartProcess(dds3FirstProcessName, 0, 0, 0, dds3FirstProcess, 0, 0);
    dds3StartProcess(dds3DrawFirstProcessName, 0x2710, 0, 0, dds3DrawFirstProcess, 0, 0);
    dds3StartProcess(dds3DrawProcessName, 0x4E20, 0, 0, dds3DrawProcess, 0, 0);
    dds3StartProcess(dds3BasicMainProcessName, 0x3FC, 1, 1, dds3BasicMainProcess, 0, 0);
    dds3StartProcess(dds3BasicDrawProcessName, 0x2B09, 1, 1, dds3BasicDrawProcess, 0, 0);
    frInitFontTable(0x100, 0x200);
    dds3StartProcess(dds3FontSysProcessName, 0x4E1F, 0, 0, frFlipPacketBuf,
        (dds3ProcessFunc_t)frDestroyFontTable, 0);
    fileSysInit();
    itfMesMngInitializeManager();
    sndInit();
    sdfDevConsInit();
    mdlInitManager();
    dds3EffectManagerInit();
    fldFirstInit();
    nbPnlLoad();
    dds3LoadBootFile();
    effBTLSystemInit();
    dds3StartMovieManager();
    evtPicturePriProcessInit();
    dds3GlobalWorkInit();
    dds3SoftResetFlag = 0;
    dds3FrameCount = 0;
    dds3AddWorld(0x2710);
    dds3InitRandCB();
    dds3InitAdmini();
    dds3AdminiCallSequence(0, 0, 0, 0);
    dds3InitDebugSystem();
    cmpTexLoad();
    dds3StartProcess(dds3DebugDevDumpProcessName, 0x5214, 0, 0, dds3DebugDevDumpProcess, 0, 0);
    dds3FadeClear();
    evtInitMoonProcess();
    encInitProcess();
    dds3AdminiCallSequence(1, 0, 0, 0);
}


void KernelDebug(void) {
}

void dds3SoftReset(void)
{
    dds3SoftResetFlag = 1;
}

u32 dds3GetKernelCount(void)
{
    return dds3FrameCount;
}

u32 dds3GetFrameID(void)
{
    return dds3FrameID;
}

void dds3PauseOn(void)
{
    dds3PauseTime = 0;
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3PauseOff(void)
{
    dds3PauseSign = 0;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3Stop(u32 frame)
{
    dds3PauseTime = SDF_FRAMES(frame);
    dds3PauseSign = 1;
    dds3SlowSign = 0;
}

void dds3SlowOn(u32 aSlowTime, u32 slowcnt)
{
    u32 runCount = dds3RunCount;

    dds3SlowSign = 1;
    dds3SlowTime = aSlowTime;
    dds3SlowRunCount = runCount;
    dds3SlowEndCount = slowcnt;
    dds3PauseSign = 0;
    dds3PauseTime = 0;
}

void dds3SlowOff(void)
{
    dds3PauseSign = 0;
    dds3SlowTime = 4;
    dds3PauseTime = 0;
    dds3SlowSign = 0;
}

void dds3ScreenCaptureOn(void) {
}

void dds3ScreenCaptureOff(void) {
}

void dds3SoftResetOn(void)
{
    dds3SoftResetDisableFlag = 0;
}

void dds3SoftResetOff(void)
{
    dds3SoftResetDisableFlag = 1;
}

void dds3KernelMain(void)
{
    u32 base;
    s32 time;
    s32 sign;

    dds3FirstInit();
    dds3FrameID = 0;
    goto top;
frame:
    dds3FrameCount++;
    dds3FrameRunTime = sdfGetRunTimeCount(base) - sdfReqDrawDelaySleepCount;
    dds3LastDrawFrameCount = sdfDrawFrameCounter;
    dds3FrameID ^= 1;
    goto runcount;
pause:
    time = dds3PauseTime;
    sign = dds3PauseSign;
    if (time > 0) {
        time--;
        if (time == 0) {
            sign = 0;
        }
        dds3PauseTime = time;
        dds3PauseSign = sign;
    }
runcount:
    dds3RunCount++;
    if (dds3FileCmdLoPrio == 0) {
        sdfFileCmdThreadSetHiPrio();
    }
top:
    if (dds3SoftResetFlag != 0) {
        goto done;
    }
    sdfWaitVblank(dds3VblanksPerFrame);
    sdfGrReqDrawDelay(dds3VblanksPerFrame - 1);
    sdfPadWork();
    if (dds3FileCmdLoPrio != 0) {
        sdfFileCmdThreadSetLoPrio();
    }
    if (dds3PauseSign != 0) {
        goto pause;
    }
    base = sdfGetRunTimeCountBase();
    dds3RunProcessList();
    if (dds3ProcessMain() == 0) {
        goto done;
    }
    goto frame;
done:
    printf(dds3HardResetMessage);
    LoadExecPS2(dds3ResetExecPath, 0, 0);
}

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3BasicMainProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3BasicDrawProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3FontSysProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3DebugDevDumpProcessName);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3HardResetMessage);

INCLUDE_RODATA(const s32, "kernel/dds3KernelMain", dds3ResetExecPath);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3RunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetFlag);

SDATA_PAD(1);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SoftResetDisableFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3PauseTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowRunCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3SlowEndCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FrameRunTime);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3LastDrawFrameCount);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3VblanksPerFrame);

SDATA_PAD(2);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3KernelDebugSign);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FileCmdLoPrio);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3FirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawFirstProcessName);

INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", dds3DrawProcessFlag);

SDATA_PAD(7);

