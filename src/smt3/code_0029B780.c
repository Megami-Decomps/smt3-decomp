#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"


/* Native 0x30-byte EE thread descriptor passed to CreateThread. */
typedef struct {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 unk18[0x18];
} sdfKernelThreadParam_t;

typedef struct sdfHandlerNode_t {
    struct sdfHandlerNode_t *next;
    s32 handlerKind;
    s32 channel;
    s32 handlerId;
} sdfHandlerNode_t;

struct pipeTextureFile_t;

extern char D_00548EB0[]; /* Padded "Pipe_Cam". */
extern char D_00549018[];
extern f32 pipeCamShakeVec[], pipeCamShake[], pipeCameraPos[];
extern dds3Basic_t *pipe_Camera;
extern s32 _gp, sdfThreadSema, sdfVblankFlag, D_00551824;
extern u8 sdfPadManagerMode, D_005756C0[], D_005776C0[];
extern void (*D_00551828)(void);
extern sdfThCB_t *sdfVblankThreadId;
extern sdfThCB_t D_00552040;

extern u32 dds3AllocObjectId(void);
extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 id, f32 *position, f32 *eye, f32 *up);
extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);
extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, f32 *eye);
extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 radius);
extern dds3Basic_t *dds3GetControlScene(void);
extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);
extern void dds3SetScreenFovy(f32 fovy);
extern s32 fclChkMessage(void);
extern void fclUpdateMessage(void);
extern void fclEndMessage(void);
extern sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *model, s32 index);
extern u32 dds3GetRandA(void *seed);
extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);
extern void *dds3AllocConfigWork(s32 mode);
extern s32 dds3CalcConfig(void *process);
extern s32 dds3DrawConfig(void *process);
extern s32 dds3UpdateConfig(void *process);
extern void dds3DestroyConfig(void *process);
extern void *dds3StartProcess(const char *name, s32 priority, s32 startDelay, s32 endDelay,
                             s32 (*update)(void *), void (*destroy)(void *), void *work);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *data, s32 mode);
extern sdfTexHandle_t *sdfTexSet(struct pipeTextureFile_t *data);
extern s32 CreateThread(sdfKernelThreadParam_t *parameter);
extern s32 AddIntcHandler2(s32 channel, s32 handler, s32 argument, s32 next);
extern s32 AddDmacHandler2(s32 channel, s32 handler, s32 argument, s32 next);
extern s32 AddSbusIntcHandler(s32 channel, s32 handler);
extern void DisableIntc(s32 channel);
extern void RemoveIntcHandler(s32 channel, s32 handlerId);
extern void RemoveDmacHandler(s32 channel, s32 handlerId);
extern void RemoveSbusIntcHandler(s32 channel);
extern s32 SetAlarm(u16 delay, void (*callback)(s32, s32, s32), void *argument);
extern s32 WaitSema(s32 semaphore);
extern s32 SignalSema(s32 semaphore);
extern s32 StartThread(s32 threadId, s32 argument);
extern void ExitDeleteThread(void);
extern void TerminateThread(s32 threadId);
extern void libc_2DBCB0(s32 threadId);
extern void EnableIntc(s32 channel);
extern void WakeupThread(s32 threadId);
extern void InitThread(void);
extern void sdfPadManager(void);
extern void sdfPadWork(void);
extern void sdfFileCmdThreadCheckPrio(void);
extern void sdfChipInit(u32 bytes);
extern void sdfMemInit(u32 bytes);
extern void sdfGsMemInit(void);
extern void sdfImageSetInit(void);
extern void sdfTexInit(void);
extern void sdfPadInitManager(void);
extern void sdfGrInit(u32 packetBufferBytes);
extern void sdf3DInit(s32 count);
extern void sdfCoreDrawInit(s32 bytes);
extern void sdfRandomizeByTime(void *seed);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);
extern void *memset(void *destination, s32 value, u32 bytes);
extern void *memcpy(void *destination, const void *source, u32 bytes);
extern void *sdfMemGetPtr(void *handle);
extern void sdfMemFreeDirect(void *handle);

void FatalError(const char *format, ...) __attribute__((noreturn));
s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
void *sdfAllocAuto(s32 bytes);
void sdfFreeAuto(void *allocation);
void sdfAlarmWakeup(s32 unused0, s32 unused1, s32 threadId);
void sdfWaitVblank(s32 count);
void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void), void *stack,
                           s64 stackBytes, s32 priority, s32 argument);
s32 sdfInitThreadSystem(void);
s32 VblankIntcHandler(s32 cause);
f32 vecLength(f32 *vector);
f32 vecNormalize(f32 *vector);
f32 func_002A7DF8(f32 base, f32 exponent);

#define T0_COUNT ((volatile u32 *)0x10000000)
#define T0_MODE ((volatile u32 *)0x10000010)
#define HW_WRITE(reg, value) (*(reg) = (value))


typedef struct tagpipeBasic {
    void *base;
    mdlObj_t *mdl;
    u8 unk08[4];
    s32 unk0C;
    f32 pos[3];
    u8 unk1C[4];
    f32 rot[3];
    u8 unk2C[4];
    f32 scl[3];
} tagpipeBasic;

typedef struct tagPipeLinkData {
    u8 unk00[4];
    struct tagPipeLinkData *next;
    struct tagPipeLinkData *prev;
} tagPipeLinkData;

typedef struct tagPipeObject {
    u8 unk00[0x18];
    s8 unk18;
    u8 action;
    u8 unk1A[2];
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipePosTable tagPipePosTable;

typedef struct tagPipeTexture {
    sdfTexHandle_t *handle;
    sdfMemHandle_t *allocation;
    struct pipeTextureFile_t *resourceData;
    u32 unk0C;
} tagPipeTexture;

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern char D_00548858[];
extern char D_005488B8[];
extern char D_005488D0[];
extern char D_005488E0[];
extern char D_005488F0[];

extern void *hWorkArea;
extern sdfHandlerNode_t *sdfDmacHandlerTable;
extern void *D_005516C4;
extern s32 D_00552054;
extern char D_00551700[];
extern char D_00551808[];
/* [1] is dds3ConfigBackup; both are written through one base register. */
extern s32 dds3ConfigValue[2];

extern s32 playerDAMAGE;
extern s32 myMaka;
extern s32 pipeMyMakaCount;
extern s8 rBoxNum;
extern s8 dds3ConfigState;
extern u8 sdfCdClock;
extern s32 sdfVblankSema;
extern s8 slLoadKillFlag;
extern s32 pipeBulrLimitValue;
extern void *pipeMessData;
extern void *makaWork;
extern u8 *GBWK;
extern u8 *DDS3_GBWK;

extern s32 CreateSema(sdfSemaParam_t *param);
extern s32 iWakeupThread(s32 thid);
extern s8 ObjectSound(void);
extern s8 fclStartMessage(s32 msgno);
extern void fclDestroyMessage(void);
extern s32 fclGetPanelAnimPhase(void *p);
extern void sdfMemFreeDelayedDirect(void *handle);
extern void sdfTexFree(void *handle);
extern void mdlSetColor(void *handle, u32 color);
extern void dds3ObjectBaseOnFlag(void *basic, u32 flag);
extern void *dds3SearchProcessName(const char *name);
extern s32 dds3GetConfigFlag(s32 index, s32 *table);
extern f32 vecDot(f32 *a, f32 *b);
extern f32 __ieee754_acosf(f32 x);
extern sdfThCB_t *func_002A87F0(s32 threadId);
extern void *fclLoadTexture(char *filename);
extern void fclSetPanelAnimState(u8 *panel, s8 state);
extern s32 fclRunPanelAnim(u8 *panel);
extern void dds3SetPosPoseSet(void *basic, f32 *pos);
extern void dds3SetRotPoseSet(void *basic, f32 *rot);
extern void dds3SetSclPoseSet(void *basic, f32 *scl);
extern void objMdlDel(tagPipeObject *objPtr);
extern void objInit(void);
extern void objMyLoad(tagPipeObject *objPtr);
extern s32 objAdd(tagPipeObject *objPtr);
extern s32 GetThreadId(void);
extern void ChangeThreadPriority(s32 thid, s32 prio);
extern s32 SleepThread(void);
extern void sdfIpuRemoveHandler(sdfHandlerNode_t *handler);
extern s32 dds3KillProcess(void *id, s32 shutdowntimer);
extern void func_002A8758(sdfThCB_t *node);
extern void p2sdPlaySE(s32 midino, s32 vol, s32 pan);
extern s32 cmpUpdateTerminate(u8 *p);
extern u8 dds3GetConfigFadeAlpha(void);
extern s32 dds3GetConfigAnimPhase(void);
extern s32 printf(const char *fmt, ...);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);
extern s32 p2sdStopBGM(s32 midino);
extern tagPipeObject *objMyData(void);
extern void objDel(tagPipeObject *objPtr);
extern void objDelAll(void);
extern void objVisibleAll(s8 value);
extern void fclSetMessageVar_20743088(s8 index, char *str);
extern s32 pipeMainSection(void);
extern void pipeDrawSection(void);
extern s32 pipeSoundSection(void);
extern void CalcCamera(void);
extern void *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(void *handle);
extern void *sdfChipAllocDirect(s32 size);
extern s32 CancelWakeupThread(s32 thid);
extern void sdfTexFreeDelayed(void *handle);

void slKillLoadProcess(void)
{
    if (D_005516C4 != NULL) {
        dds3KillProcess(D_005516C4, 1);
        D_005516C4 = NULL;
        slLoadKillFlag = 0;
    }
}

s32 dds3_29B7B0(void)
{
    return slLoadKillFlag;
}

void func_0029B7B8(void) {
}

INCLUDE_ASM(const s32, "code_0029B780", dds3ConfigChanged);

void dds3ConfigSaveBackup(void)
{
    printf(D_00548858);
    dds3ConfigValue[0] = dds3ConfigValue[1] = *(s32 *)(DDS3_GBWK + 0x11AB8);
}

INCLUDE_ASM(const s32, "code_0029B780", dds3ConfigRestoreBackup);

INCLUDE_ASM(const s32, "code_0029B780", dds3InitConfigItems);

s32 dds3UpdateConfigCursor(void)
{
    fclSetPanelAnimState(GBWK + 0x1058, 2);
    return fclRunPanelAnim(GBWK + 0x1058);
}

s32 dds3GetConfigAnimPhase(void)
{
    return fclGetPanelAnimPhase(GBWK + 0x1058);
}

u8 dds3GetConfigFadeAlpha(void)
{
    return (0x200 - dds3GetConfigAnimPhase()) / 4;
}

void dds3ScrollConfigList(void)
{
    u8 *gbwk;
    s32 next;

    if (dds3GetConfigAnimPhase() > 0) {
        return;
    }
    gbwk = GBWK;
    next = *(s32 *)(gbwk + 0x10A0) + 0xC;
    *(s32 *)(gbwk + 0x10A0) = next;
    if (next > 0x200) {
        *(s32 *)(gbwk + 0x10A0) = 0;
    }
}

INCLUDE_ASM(const s32, "code_0029B780", dds3ToggleConfigFlag);

INCLUDE_ASM(const s32, "code_0029B780", dds3GetConfigFlag);

s32 dds3GetGlobalConfigFlag(s32 index)
{
    return dds3GetConfigFlag(index, (s32 *)(DDS3_GBWK + 0x11AB8));
}

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigLabel);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigCursor);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigItem);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigSlider);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigBar);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigValue);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigText);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigBackground);

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfigPanel);

void *cfgTexLoad(void)
{
    return fclLoadTexture(D_005488B8);
}

void dds3FreeConfigTexture(void *handle)
{
    if (handle != NULL) {
        sdfTexFree(handle);
    }
}

INCLUDE_ASM(const s32, "code_0029B780", dds3AllocConfigWork);

INCLUDE_ASM(const s32, "code_0029B780", dds3DestroyConfig);

void dds3ConfigProcessStart(s32 mode) {
    if (GBWK == 0) {
        GBWK = dds3AllocConfigWork(mode);
        dds3StartProcess(D_00551700, 0x3F2, 1, 1, dds3CalcConfig, NULL, GBWK);
        dds3StartProcess(D_005488D0, 0x2B07, 1, 1, dds3DrawConfig, NULL, GBWK);
        dds3StartProcess(D_005488E0, 0x520B, 1, 1, dds3UpdateConfig, dds3DestroyConfig, GBWK);
        dds3ConfigState = 1;
    }
}

void dds3ConfigProcessEnd(void)
{
    dds3KillProcessName(D_00551700, 1);
    dds3KillProcessName(D_005488D0, 1);
    dds3KillProcessName(D_005488E0, 1);
}

s32 checkConfigState(void)
{
    if (dds3ConfigState == 1) {
        return 1;
    }
    if (dds3ConfigState >= 2) {
        if (dds3ConfigState == 2) {
            dds3ConfigState = 0;
        }
    }
    return 0;
}

s32 dds3CalcConfig(void *PID)
{
    u8 *gbwk = GBWK;

    if ((gbwk[0] & 0x10) != 0) {
        cmpUpdateTerminate(gbwk);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0029B780", dds3DrawConfig);

INCLUDE_ASM(const s32, "code_0029B780", dds3UpdateConfig);

INCLUDE_ASM(const s32, "code_0029B780", MainPipe);

INCLUDE_ASM(const s32, "code_0029B780", InitPipe);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548888);

INCLUDE_RODATA(const s32, "code_0029B780", D_005488B8);

INCLUDE_RODATA(const s32, "code_0029B780", D_005488D0);

INCLUDE_RODATA(const s32, "code_0029B780", D_005488E0);

INCLUDE_RODATA(const s32, "code_0029B780", D_005488F0);

INCLUDE_ASM(const s32, "code_0029B780", EndPipe);

s32 ChkPipe(void)
{
    return dds3SearchProcessName(D_005488F0) != NULL;
}

void DEBUG_PRINT(const char *fmt, ...) {
}

s32 pipe_Section(void)
{
    s32 ret = pipeMainSection();

    pipeDrawSection();
    pipeSoundSection();
    CalcCamera();
    return ret;
}

INCLUDE_RODATA(const s32, "code_0029B780", D_00548910);

INCLUDE_ASM(const s32, "code_0029B780", pipeInitSection);

INCLUDE_ASM(const s32, "code_0029B780", pipeEndSection);

INCLUDE_ASM(const s32, "code_0029B780", pipeDataRelease);

INCLUDE_ASM(const s32, "code_0029B780", pipeLoadSection);

INCLUDE_ASM(const s32, "code_0029B780", pipeMainSection);

INCLUDE_ASM(const s32, "code_0029B780", pipeDrawSection);

s32 pipeSoundSection(void)
{
    return ObjectSound();
}

INCLUDE_ASM(const s32, "code_0029B780", pipeOffset);

INCLUDE_ASM(const s32, "code_0029B780", pipeStart);

void pipeEnd(void)
{
    tagPipeObject *objPtr;

    p2sdStopBGM(0x670001);
    objPtr = objMyData();
    objDel(objPtr);
    objDelAll();
    objVisibleAll(0);
    objPtr = NULL;
}

INCLUDE_ASM(const s32, "code_0029B780", PipeMain);

INCLUDE_ASM(const s32, "code_0029B780", pipeLoadObjects);

INCLUDE_ASM(const s32, "code_0029B780", MyTask);

INCLUDE_ASM(const s32, "code_0029B780", MyAction);

INCLUDE_ASM(const s32, "code_0029B780", MyHitAction);

INCLUDE_ASM(const s32, "code_0029B780", EmTask);

INCLUDE_ASM(const s32, "code_0029B780", ActionRot);

INCLUDE_ASM(const s32, "code_0029B780", ActionHm);

INCLUDE_ASM(const s32, "code_0029B780", ActionSlide);

INCLUDE_ASM(const s32, "code_0029B780", ActionCrush);

INCLUDE_ASM(const s32, "code_0029B780", ActionBound);

INCLUDE_ASM(const s32, "code_0029B780", ActionAbsorp);

INCLUDE_ASM(const s32, "code_0029B780", ActionDeath);

INCLUDE_ASM(const s32, "code_0029B780", ActionZMotion);

INCLUDE_ASM(const s32, "code_0029B780", ActionKey);

INCLUDE_ASM(const s32, "code_0029B780", HITCHECK_CUR);

INCLUDE_ASM(const s32, "code_0029B780", ObjectDraw);

void MyDraw(void) {
}

INCLUDE_ASM(const s32, "code_0029B780", ObjectSound);

INCLUDE_ASM(const s32, "code_0029B780", MySound);

void EmSound(tagPipeObject *objPtr)
{
    if ((objPtr->flags & 0x1000) != 0) {
        p2sdPlaySE(0x670008, 0x7F, 0x3F);
        objPtr = NULL;
    }
}

s32 GetMaka(void)
{
    return myMaka;
}

s32 GetRBoxNum(void)
{
    return rBoxNum;
}

s32 GetDamage(void)
{
    return playerDAMAGE;
}

INCLUDE_ASM(const s32, "code_0029B780", PlayerStatusSetUp);

s32 pipeSelectSection(void)
{
    return 0;
}

void DataRelease(void) {
}

void workRelease(void)
{
    if (hWorkArea != NULL) {
        sdfMemFreeDelayedDirect(hWorkArea);
        hWorkArea = NULL;
    }
}

INCLUDE_ASM(const s32, "code_0029B780", mymdlStartUp);

INCLUDE_ASM(const s32, "code_0029B780", Initmymdl);

INCLUDE_ASM(const s32, "code_0029B780", getPosTranslate);

INCLUDE_ASM(const s32, "code_0029B780", objMyLoad);

INCLUDE_ASM(const s32, "code_0029B780", objLoad);

INCLUDE_ASM(const s32, "code_0029B780", objMake);

INCLUDE_ASM(const s32, "code_0029B780", objOption);

INCLUDE_ASM(const s32, "code_0029B780", objOptionInc);

void objSetAction(tagPipeObject *objPtr, u8 action, s16 count)
{
    objPtr->action = action;
    objPtr->count = count;
}

void objInitMyAdd(void)
{
    tagPipeObject obj;

    objInit();
    objMyLoad(&obj);
    objAdd(&obj);
}

INCLUDE_ASM(const s32, "code_0029B780", objCopy);

INCLUDE_ASM(const s32, "code_0029B780", objCopy2);

INCLUDE_ASM(const s32, "code_0029B780", objAddDMaka);

INCLUDE_ASM(const s32, "code_0029B780", objAddDRBOX);

INCLUDE_ASM(const s32, "code_0029B780", objInit);

INCLUDE_ASM(const s32, "code_0029B780", objAdd);

void objDel(tagPipeObject *objPtr)
{
    objMdlDel(objPtr);
    objPtr->unk18 = -1;
}

INCLUDE_ASM(const s32, "code_0029B780", objDelAll);

INCLUDE_ASM(const s32, "code_0029B780", objInfo);

INCLUDE_ASM(const s32, "code_0029B780", objMyData);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548B50);

INCLUDE_ASM(const s32, "code_0029B780", objMdlAdd);

INCLUDE_ASM(const s32, "code_0029B780", objMdlDel);

INCLUDE_ASM(const s32, "code_0029B780", objMdlPosSet);

INCLUDE_ASM(const s32, "code_0029B780", objMdlSclSet);

INCLUDE_ASM(const s32, "code_0029B780", objMdlColorSet);

INCLUDE_ASM(const s32, "code_0029B780", objMdlAlphaSet);

INCLUDE_ASM(const s32, "code_0029B780", objMdlMotion);

void pipeMyMakaAdd(s32 addMaka)
{
    myMaka += addMaka;
    pipeMyMakaCount += addMaka;
}

void pipeMyMakaSub(s32 subMaka)
{
    myMaka -= subMaka;
}

void pipeMyMakaSubRate(f32 subMakaRate)
{
    myMaka = myMaka - myMaka * subMakaRate;
}

INCLUDE_ASM(const s32, "code_0029B780", objMyStatusChange);

INCLUDE_ASM(const s32, "code_0029B780", objVisibleAll);

INCLUDE_ASM(const s32, "code_0029B780", mdlTask);

INCLUDE_ASM(const s32, "code_0029B780", partsInit);

INCLUDE_ASM(const s32, "code_0029B780", partsAdd);

INCLUDE_ASM(const s32, "code_0029B780", partsDel);

INCLUDE_ASM(const s32, "code_0029B780", PipemdlRot);

INCLUDE_ASM(const s32, "code_0029B780", PipeTexLoad);

INCLUDE_ASM(const s32, "code_0029B780", PipeTexRelease);

s32 texLoad(const char *name, tagPipeTexture *record) {
    void *allocation = sdfDevLoadFileEx(name, (u32 *)&record->resourceData, 0);
    record->allocation = allocation;
    record->handle = sdfTexSet(record->resourceData);
    if (record->allocation != NULL) {
        sdfMemFreeDelayedDirect(record->allocation);
        record->allocation = NULL;
        record->resourceData = NULL;
    }
    return 1;
}

s32 texRelease(tagPipeTexture *texPtr)
{
    if (texPtr->handle != NULL) {
        sdfTexFreeDelayed(texPtr->handle);
        texPtr->handle = NULL;
    }
    return 1;
}

s32 pipeFileSave(s8 filename, tagPipePosTable *posPtr, s8 index)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_0029B780", pipeFileLoad);

INCLUDE_ASM(const s32, "code_0029B780", getPartsInfo);

INCLUDE_ASM(const s32, "code_0029B780", getPartsType);

s32 pipe_2A2F38(void)
{
    return 0;
}

s32 pipe_2A2F40(void)
{
    return 1;
}

void func_002A2F48(void) {
}

void func_002A2F50(void) {
}

INCLUDE_ASM(const s32, "code_0029B780", getViewPortMatrix);

INCLUDE_ASM(const s32, "code_0029B780", PipeCamera);

INCLUDE_ASM(const s32, "code_0029B780", PipeCameraAction);

INCLUDE_ASM(const s32, "code_0029B780", PipeCameraAction2);

INCLUDE_ASM(const s32, "code_0029B780", PipeCameraAction3);

INCLUDE_ASM(const s32, "code_0029B780", CamActionQuake);

void AddCamera(void) {
    dds3Basic_t *scene;
    dds3Basic_t *camera;
    f32 *eye = pipeCamShakeVec;

    camera = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), pipeCamShake, eye, pipeCameraPos);
    pipe_Camera = camera;
    dds3SetUnitObjectId(camera, D_00548EB0);
    dds3CameraObjectBasicGetCameraTM(pipe_Camera, eye);
    dds3GetRadiusPoseSet(pipe_Camera, 2.0f);
    scene = dds3GetControlScene();
    dds3SetActiveCameraSceneBasic(scene, pipe_Camera);
    dds3SetScreenFovy(0.87964585f);
}

void CalcCamera(void) {
    dds3CameraObjectBasicWorkData_t *cameraData;
    dds3Basic_t *camera;

    dds3CameraObjectBasicGetCameraTM(pipe_Camera, pipeCamShakeVec);
    dds3SetPosPoseSet(pipe_Camera, pipeCamShake);
    camera = pipe_Camera;
    ((void (*)(dds3Basic_t *))camera->basicfunction->mainprocuder)(camera);
    cameraData = pipe_Camera->workdata;
    PS2_COPY_QUAD(&cameraData->vertical, pipeCameraPos);
    PS2_COPY_QUAD(&cameraData->upVector, pipeCameraPos);
}

INCLUDE_RODATA(const s32, "code_0029B780", D_00548EB0);

INCLUDE_ASM(const s32, "code_0029B780", pipeCheckLevel);

INCLUDE_ASM(const s32, "code_0029B780", pipeResultSection);

INCLUDE_ASM(const s32, "code_0029B780", pipeResultDraw);

INCLUDE_ASM(const s32, "code_0029B780", pipeMessStartIdx);

INCLUDE_ASM(const s32, "code_0029B780", pipeMessInit);

void pipeFreeMessage(void)
{
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(pipeMessData);
}

void pipeSetMessageNum(s32 num)
{
    char buf[32];

    sprintf(buf, D_00551808, num);
    fclSetMessageVar_20743088(0, buf);
    fclStartMessage(0);
}

INCLUDE_ASM(const s32, "code_0029B780", pipeStartRandomMessage);

s32 pipeStartMessage(void)
{
    return fclStartMessage(2);
}

s32 pipeMessDraw(void) {
    s32 finished = 0;
    if (fclChkMessage()) {
        fclUpdateMessage();
    } else {
        finished = 1;
        fclEndMessage();
    }
    return finished;
}

INCLUDE_ASM(const s32, "code_0029B780", pipeGetRandomMessage);

INCLUDE_ASM(const s32, "code_0029B780", InitModel);

INCLUDE_ASM(const s32, "code_0029B780", mymdlAddObject);

void mymdlDelObject(tagpipeBasic *pWork)
{
    pWork->unk0C = 0;
    dds3ObjectBaseOnFlag(pWork->base, 1);
    pWork = NULL;
}

INCLUDE_ASM(const s32, "code_0029B780", mymdlInitObject);

void mymdlPosSet(tagpipeBasic *pWork, f32 x, f32 y, f32 z)
{
    pWork->pos[0] = x;
    pWork->pos[1] = y;
    pWork->pos[2] = z;
    dds3SetPosPoseSet(pWork->base, pWork->pos);
    pWork = NULL;
}

INCLUDE_ASM(const s32, "code_0029B780", SetMdlPosition);

void mymdlRotSet(tagpipeBasic *pWork, f32 x, f32 y, f32 z)
{
    pWork->rot[0] = x;
    pWork->rot[1] = y;
    pWork->rot[2] = z;
    dds3SetRotPoseSet(pWork->base, pWork->rot);
    pWork = NULL;
}

void mymdlSclSet(tagpipeBasic *pWork, f32 scl)
{
    pWork->scl[0] = scl;
    pWork->scl[1] = scl;
    pWork->scl[2] = scl;
    dds3SetSclPoseSet(pWork->base, pWork->scl);
    pWork = NULL;
}

void mymdlColorSet(tagpipeBasic *pWork, u32 color)
{
    mdlSetColor(pWork->mdl, color);
    pWork = NULL;
}

INCLUDE_ASM(const s32, "code_0029B780", mymdlAlphaSet);

INCLUDE_ASM(const s32, "code_0029B780", mymdlInit);

INCLUDE_ASM(const s32, "code_0029B780", mymdlVisible);

INCLUDE_ASM(const s32, "code_0029B780", mymdlVisibleAll);

void mymdlSetMotion(tagpipeBasic *node, f32 frame) {
    void *motion = mdlGetMotionCtrlr(node->mdl, 0);
    if (frame < 0.0f) {
        u32 random = dds3GetRandA(NULL);
        sdfMotionSeek(motion, (f32)(random % node->mdl->first->frameLength));
        return;
    }
    sdfMotionSeek(motion, frame);
    /* DDS2 mnuSampleModelMotionAtFrame and SMT3 002A4BB0 retain jal here;
     * resetting the consumed frame follows the documented tail-call exception. */
    frame = 0.0f;
}

void pipeBulrSetLimit(s32 frame)
{
    pipeBulrLimitValue = frame;
}

INCLUDE_ASM(const s32, "code_0029B780", pipeBulrLimit);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffCreateLoadFile);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffLoad);

INCLUDE_ASM(const s32, "code_0029B780", pipeFreeEffects);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffRelease);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffDraw);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffCal);

INCLUDE_ASM(const s32, "code_0029B780", pipeGetObjData);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffSet);

INCLUDE_ASM(const s32, "code_0029B780", pipeLoadEffect);

INCLUDE_ASM(const s32, "code_0029B780", pipeEffDraw2Doffset);

void makaInit(void)
{
    void *work = makaWork;

    memset(work, 0, 0xC8);
    work = NULL;
}

INCLUDE_ASM(const s32, "code_0029B780", makaAdd);

s32 makaGetBarWidth(s32 maka)
{
    u32 rest = maka & 0x7FFFFFFF;
    s32 digits = 0;

    while (rest != 0) {
        rest /= 10;
        digits++;
    }
    return digits;
}

INCLUDE_ASM(const s32, "code_0029B780", makaGetBarCell);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawNormal);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawSingle);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawMulti);

INCLUDE_ASM(const s32, "code_0029B780", makaDraw);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawGauge);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawMultiGauge);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawNormalGauge);

INCLUDE_ASM(const s32, "code_0029B780", makaDrawBar);

INCLUDE_ASM(const s32, "code_0029B780", LoadLinkData);

s32 pipeLinkGetNum(u8 *lPtr, s32 size)
{
    s32 num = 1;

    while (num < size && *lPtr != 0) {
        lPtr++;
        num++;
    }
    return num;
}

INCLUDE_ASM(const s32, "code_0029B780", pipeCreateLink);

INCLUDE_ASM(const s32, "code_0029B780", pipeInitLink);

INCLUDE_ASM(const s32, "code_0029B780", pipeMakeNodeLink);

s32 pipeConnectLink(tagPipeLinkData *pPrev, tagPipeLinkData *pNext)
{
    if (pPrev == NULL || pNext == NULL) {
        return 0;
    }
    pPrev->next = pNext;
    pNext->prev = pPrev;
    return 1;
}

s32 pipeDellLink(tagPipeLinkData *pData)
{
    tagPipeLinkData *prev;
    tagPipeLinkData *next;

    if (pData == NULL) {
        return 0;
    }
    prev = pData->prev;
    next = pData->next;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0029B780", pipeFreeLink);

void pipeCameraLerp(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[1] = vector[1] * cosine + vector[2] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[2] = vector[1] * -sine + vector[2] * cosine;
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

void vec_2A6B80(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[0] = vector[0] * cosine - vector[2] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[2] = vector[0] * sine + vector[2] * cosine;
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

void vecRotate2D(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[0] = vector[0] * cosine + vector[1] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[1] = vector[0] * -sine + vector[1] * cosine;
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

void vec_2A6CA8(float *vector, float *axis, float angle) {
    sdfVector4_t normalizedAxis;
    sdfVector4_t scratch;
    sdfVector4_t inputVector;
    float rotationMatrix[9];

    memset(&scratch, 0, sizeof(scratch));
    scratch.x = axis[0];
    scratch.y = axis[1];
    scratch.z = axis[2];
    normalizedAxis = scratch;
    memset(&inputVector, 0, sizeof(inputVector));
    inputVector.x = vector[0];
    inputVector.y = vector[1];
    inputVector.z = vector[2];
    scratch = inputVector;
    vecNormalize(&normalizedAxis.x);
    rotationMatrix[0] = normalizedAxis.x * normalizedAxis.x * (1.0f - sdfCos(angle)) + sdfCos(angle);
    rotationMatrix[1] = normalizedAxis.x * normalizedAxis.y * (1.0f - sdfCos(angle)) - normalizedAxis.z * sdfSin(angle);
    rotationMatrix[2] = normalizedAxis.x * normalizedAxis.z * (1.0f - sdfCos(angle)) + normalizedAxis.y * sdfSin(angle);
    rotationMatrix[3] = normalizedAxis.y * normalizedAxis.x * (1.0f - sdfCos(angle)) + normalizedAxis.z * sdfSin(angle);
    rotationMatrix[4] = normalizedAxis.y * normalizedAxis.y * (1.0f - sdfCos(angle)) + sdfCos(angle);
    rotationMatrix[5] = normalizedAxis.y * normalizedAxis.z * (1.0f - sdfCos(angle)) - normalizedAxis.x * sdfSin(angle);
    rotationMatrix[6] = normalizedAxis.z * normalizedAxis.x * (1.0f - sdfCos(angle)) - normalizedAxis.y * sdfSin(angle);
    rotationMatrix[7] = normalizedAxis.z * normalizedAxis.y * (1.0f - sdfCos(angle)) + normalizedAxis.x * sdfSin(angle);
    rotationMatrix[8] = normalizedAxis.z * normalizedAxis.z * (1.0f - sdfCos(angle)) + sdfCos(angle);
    vector[0] = scratch.x * rotationMatrix[0] + scratch.y * rotationMatrix[3] + scratch.z * rotationMatrix[6];
    vector[1] = scratch.x * rotationMatrix[1] + scratch.y * rotationMatrix[4] + scratch.z * rotationMatrix[7];
    vector[2] = scratch.x * rotationMatrix[2] + scratch.y * rotationMatrix[5] + scratch.z * rotationMatrix[8];
}

void vec_2A6FF0(f32 *a, f32 *b)
{
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
}

void vec_2A7028(f32 *a, f32 *b)
{
    a[0] -= b[0];
    a[1] -= b[1];
    a[2] -= b[2];
}

void vec_2A7060(f32 *v, f32 x, f32 y, f32 z)
{
    v[0] += x;
    v[1] += y;
    v[2] += z;
}

void vecSet(f32 *v, f32 x, f32 y, f32 z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void vecScale(f32 *v, f32 scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

float vecNormalize(float *vector) {
    float length = vecLength(vector);

    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

f32 vecLength(f32 *v)
{
    f32 len;

    EE_SQRT_F32(len, v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return len;
}

float vecDot(float *left, float *right) {
    sdfVector4_t normalizedLeft;
    sdfVector4_t normalizedRight;

    normalizedLeft = *(sdfVector4_t *)left;
    normalizedRight = *(sdfVector4_t *)right;
    vecNormalize(&normalizedLeft.x);
    vecNormalize(&normalizedRight.x);
    return normalizedLeft.x * normalizedRight.x + normalizedLeft.y * normalizedRight.y + normalizedLeft.z * normalizedRight.z;
}

f32 vecAngleBetween(f32 *a, f32 *b)
{
    return __ieee754_acosf(vecDot(a, b));
}

void vecTranslate(float *output, float *left, float *right) {
    sdfVector4_t normalizedLeft;
    sdfVector4_t normalizedRight;

    normalizedLeft = *(sdfVector4_t *)left;
    normalizedRight = *(sdfVector4_t *)right;
    vecNormalize(&normalizedLeft.x);
    vecNormalize(&normalizedRight.x);
    output[0] = normalizedLeft.y * normalizedRight.z - normalizedLeft.z * normalizedRight.y;
    output[1] = normalizedLeft.z * normalizedRight.x - normalizedLeft.x * normalizedRight.z;
    output[2] = normalizedLeft.x * normalizedRight.y - normalizedLeft.y * normalizedRight.x;
}

void mtxRotateX(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0];
    rotated.m[1] = matrix->m[1];
    rotated.m[2] = matrix->m[2];
    rotated.m[3] = matrix->m[3];
    rotated.m[4] = matrix->m[4] * sdfCos(angle) + matrix->m[8] * sdfSin(angle);
    rotated.m[5] = matrix->m[5] * sdfCos(angle) + matrix->m[9] * sdfSin(angle);
    rotated.m[6] = matrix->m[6] * sdfCos(angle) + matrix->m[10] * sdfSin(angle);
    rotated.m[7] = matrix->m[7] * sdfCos(angle) + matrix->m[11] * sdfSin(angle);
    rotated.m[8] = matrix->m[4] * -sdfSin(angle) + matrix->m[8] * sdfCos(angle);
    rotated.m[9] = matrix->m[5] * -sdfSin(angle) + matrix->m[9] * sdfCos(angle);
    rotated.m[10] = matrix->m[6] * -sdfSin(angle) + matrix->m[10] * sdfCos(angle);
    rotated.m[11] = matrix->m[7] * -sdfSin(angle) + matrix->m[11] * sdfCos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxRotateY(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0] * sdfCos(angle) + matrix->m[8] * -sdfSin(angle);
    rotated.m[1] = matrix->m[1] * sdfCos(angle) + matrix->m[9] * -sdfSin(angle);
    rotated.m[2] = matrix->m[2] * sdfCos(angle) + matrix->m[10] * -sdfSin(angle);
    rotated.m[3] = matrix->m[3] * sdfCos(angle) + matrix->m[11] * -sdfSin(angle);
    rotated.m[4] = matrix->m[4];
    rotated.m[5] = matrix->m[5];
    rotated.m[6] = matrix->m[6];
    rotated.m[7] = matrix->m[7];
    rotated.m[8] = matrix->m[0] * sdfSin(angle) + matrix->m[8] * sdfCos(angle);
    rotated.m[9] = matrix->m[1] * sdfSin(angle) + matrix->m[9] * sdfCos(angle);
    rotated.m[10] = matrix->m[2] * sdfSin(angle) + matrix->m[10] * sdfCos(angle);
    rotated.m[11] = matrix->m[3] * sdfSin(angle) + matrix->m[11] * sdfCos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxRotateZ(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0] * sdfCos(angle) + matrix->m[4] * sdfSin(angle);
    rotated.m[1] = matrix->m[1] * sdfCos(angle) + matrix->m[5] * sdfSin(angle);
    rotated.m[2] = matrix->m[2] * sdfCos(angle) + matrix->m[6] * sdfSin(angle);
    rotated.m[3] = matrix->m[3] * sdfCos(angle) + matrix->m[7] * sdfSin(angle);
    rotated.m[4] = matrix->m[0] * -sdfSin(angle) + matrix->m[4] * sdfCos(angle);
    rotated.m[5] = matrix->m[1] * -sdfSin(angle) + matrix->m[5] * sdfCos(angle);
    rotated.m[6] = matrix->m[2] * -sdfSin(angle) + matrix->m[6] * sdfCos(angle);
    rotated.m[7] = matrix->m[3] * -sdfSin(angle) + matrix->m[7] * sdfCos(angle);
    rotated.m[8] = matrix->m[8];
    rotated.m[9] = matrix->m[9];
    rotated.m[10] = matrix->m[10];
    rotated.m[11] = matrix->m[11];
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxStore(sdfMatrix4_t *destination, sdfMatrix4_t *source) {
    sdfMatrix4_t sourceCopy = *source;

    destination->m[0] = sourceCopy.m[0];
    destination->m[1] = sourceCopy.m[4];
    destination->m[2] = sourceCopy.m[8];
    destination->m[3] = sourceCopy.m[12];
    destination->m[4] = sourceCopy.m[1];
    destination->m[5] = sourceCopy.m[5];
    destination->m[6] = sourceCopy.m[9];
    destination->m[7] = sourceCopy.m[13];
    destination->m[8] = sourceCopy.m[2];
    destination->m[9] = sourceCopy.m[6];
    destination->m[10] = sourceCopy.m[10];
    destination->m[11] = sourceCopy.m[14];
    destination->m[12] = sourceCopy.m[3];
    destination->m[13] = sourceCopy.m[7];
    destination->m[14] = sourceCopy.m[11];
    destination->m[15] = sourceCopy.m[15];
}

f32 *mtx_2A7B10(f32 *direction, f32 *matrix) {
    f32 transformed[4];
    f32 x = direction[0];
    f32 y = direction[1];
    f32 z = direction[2];

    transformed[0] = x * matrix[0] + y * matrix[4] + z * matrix[8];
    transformed[1] = x * matrix[1] + y * matrix[5] + z * matrix[9];
    transformed[2] = x * matrix[2] + y * matrix[6] + z * matrix[10];
    memcpy(direction, transformed, 16);
    return direction;
}

void *mtx_2A7BB8(sdfVector4_t *controlPoints, s32 degree, s32 sampleCount) {
    void *outputBlock;
    void *coefficientBlock;
    f32 *coefficients;
    sdfVector4_t *output;
    sdfVector4_t *sample;
    s32 i;
    s32 j;

    coefficientBlock = sdfMemAllocDirect(degree + 1);
    coefficients = (f32 *)sdfMemGetPtr(coefficientBlock);
    outputBlock = sdfMemAllocDirect(sampleCount + 2);
    output = (sdfVector4_t *)sdfMemGetPtr(outputBlock);

    for (i = 0; i <= degree; i++) {
        coefficients[i] = 1.0f;
        for (j = 1; j <= i; j++) {
            coefficients[i] = (coefficients[i] * (f32)(degree - j + 1)) / (f32)j;
        }
    }

    i = 0;
    output[0].x = controlPoints[0].x;
    output[0].y = controlPoints[0].y;
    output[0].z = controlPoints[0].z;

    if (sampleCount >= 0) {
        sample = output;
        do {
            f32 t = (f32)i / (f32)sampleCount;
            f32 x = 0.0f;
            f32 y = 0.0f;
            f32 z = 0.0f;

            j = 0;
            if (degree >= 0) {
                sdfVector4_t *point = controlPoints;
                f32 *coefficient = coefficients;
                do {
                    f32 powers = func_002A7DF8(t, (f32)j);
                    f32 weight;

                    powers *= func_002A7DF8(1.0f - t, (f32)(degree - j));
                    weight = *coefficient * powers;
                    x += point->x * weight;
                    y += point->y * weight;
                    z += point->z * weight;
                    point++;
                    coefficient++;
                    j++;
                } while (j <= degree);
            }

            sample->x = x;
            sample->y = y;
            sample->z = z;
            sample++;
            i++;
        } while (i <= sampleCount);
    }

    sdfMemFreeDirect(coefficientBlock);
    return outputBlock;
}

f32 func_002A7DF8(f32 value, f32 count)
{
    f32 result = 1.0f;
    s32 i;

    for (i = 1; i <= count; i++) {
        result *= value;
    }
    return result;
}

void vecDistance(f32 quaternion[4], f32 matrix[4][4]) {
    f32 trace;
    f32 scale;
    s32 i;
    u8 j;
    u8 k;
    f32 xx = matrix[0][0];
    f32 yy = matrix[1][1];
    f32 zz = matrix[2][2];

    trace = xx + yy + zz + 1.0f;
    if (trace >= 1.0f) {
        scale = fsqrtf(trace) * 2.0f;
        quaternion[3] = scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = xx > yy ? 0 : 1;
        if (zz > matrix[i][i]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        scale = fsqrtf(matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f) * 2.0f;
        if (scale != 0.0f) {
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[i][j] + matrix[j][i]) / scale;
            quaternion[k] = (matrix[i][k] + matrix[k][i]) / scale;
            quaternion[3] = (matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
}

void sdfDummyFunc(void) {
}

s32 sdfCreateSema(s32 initCount, s32 maxCount, u32 option)
{
    sdfSemaParam_t param;

    param.initCount = initCount;
    param.maxCount = maxCount;
    param.option = option;
    return CreateSema(&param);
}

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority) {
    sdfKernelThreadParam_t param;

    memset(&param, 0, sizeof(param));
    param.entry = entryAddress;
    param.stack = workspace;
    param.stackSize = stackBytes;
    param.priority = priority;
    param.gp = &_gp;
    return CreateThread(&param);
}

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocAuto(stackBytes);
    return CreateThreadSimple((void *)entryAddress, workspace, stackBytes, priority);
}

void *sdfAllocAuto(s32 size)
{
    void *p;

    if (size > 0x400) {
        p = sdfMemLock(sdfMemAllocDirect(size));
        size = 0;
    } else {
        p = sdfChipAllocDirect(size);
        size = 0;
    }
    return p;
}

INCLUDE_ASM(const s32, "code_0029B780", sdfFreeAuto);

INCLUDE_ASM(const s32, "code_0029B780", sdfFreeAutoDelayed);

INCLUDE_ASM(const s32, "code_0029B780", sdf_2A8258);

void FatalError(const char *format, ...) {
    for (;;) {
    }
}

sdfHandlerNode_t *DmacAddHandler(s32 kind, s32 channel, s32 handler, s32 arg, s32 next) {
    sdfHandlerNode_t *node;
    s32 id;

    switch (kind) {
    case 0:
        id = AddIntcHandler2(channel, handler, arg, next);
        break;
    case 1:
        id = AddDmacHandler2(channel, handler, arg, next);
        break;
    case 2:
        id = AddSbusIntcHandler(channel, handler);
        break;
    default:
        FatalError(D_00549018, kind);
        break;
    }
    node = sdfAllocAuto(sizeof(*node));
    node->handlerKind = kind;
    node->channel = channel;
    node->handlerId = id;
    node->next = sdfDmacHandlerTable;
    sdfDmacHandlerTable = node;
    return node;
}

void sdfIpuRemoveHandler(sdfHandlerNode_t *target) {
    sdfHandlerNode_t **link = &sdfDmacHandlerTable;
    sdfHandlerNode_t *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->handlerKind) {
            case 0:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case 1:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case 2:
                RemoveSbusIntcHandler(node->channel);
                break;
            }
            sdfFreeAuto(target);
            return;
        }
        /* Non-head handles are followed through their own link. */
        link = &target->next;
        node = *link;
    }
}

void sdf_2A84D8(void)
{
    void *handler;

    while ((handler = sdfDmacHandlerTable) != NULL) {
        sdfIpuRemoveHandler(handler);
    }
}

void sdfAlarmWakeup(s32 arg0, s32 arg1, s32 thid)
{
    iWakeupThread(thid);
}

void sdfDelayThread(u32 delay) {
    s32 threadId = GetThreadId();
    CancelWakeupThread(threadId);
    SetAlarm(delay & 0xFFFF, sdfAlarmWakeup, (void *)threadId);
    SleepThread();
}

s32 sdfGetRunTimeCountBase(void)
{
    return *(volatile u32 *)0x10000000;
}

s32 sdfGetRunTimeCount(s32 aBaseTime)
{
    return (*(volatile u32 *)0x10000000 - aBaseTime) & 0xFFFF;
}

void sdfSystemThreadProc(void) {
    for (;;) {
        sdfWaitVblank(1);
        sdfPadManager();
        if (!sdfPadManagerMode) {
            sdfPadWork();
        }
        sdfFileCmdThreadCheckPrio();
        if (D_00551828 != NULL) {
            D_00551828();
        }
    }
}

void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes) {
    HW_WRITE(T0_MODE, 0x83);
    HW_WRITE(T0_COUNT, 0);
    sdfChipInit(initialHeapSize);
    sdfMemInit(heapSize);
    sdfGsMemInit();
    sdfImageSetInit();
    sdfTexInit();
    sdfPadInitManager();
    sdfInitThreadSystem();
    sdfGrInit(packetBufferBytes);
    sdf3DInit(0x40);
    sdfCoreDrawInit(D_00551824);
    sdfRandomizeByTime(NULL);
    sdfStartThreadAndWait(&D_00552040, sdfSystemThreadProc, D_005756C0, 0x2000, 0x4C, 0);
}

void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void), void *stack, s64 stackSize, s32 priority, s32 threadArgument) {
    node->threadId = CreateThreadSimple((void *)entry, stack, (s32)stackSize, priority);
    WaitSema(sdfThreadSema);
    node->next = sdfVblankThreadId;
    sdfVblankThreadId = node;
    SignalSema(sdfThreadSema);
    StartThread(node->threadId, threadArgument);
}

void sdfWaitVblank(s32 count)
{
    s32 rest = count - CancelWakeupThread(GetThreadId());

    do {
        SleepThread();
        rest--;
    } while (rest > 0);
}

void func_002A8758(sdfThCB_t *node) {
    sdfThCB_t **link;
    sdfThCB_t *current;
    s32 threadId;

    WaitSema(sdfThreadSema);
    link = &sdfVblankThreadId;
    current = *link;
    while (current != node) {
        link = &current->next;
        current = current->next;
    }
    *link = node->next;
    SignalSema(sdfThreadSema);
    threadId = node->threadId;
    if (threadId == GetThreadId()) {
        ExitDeleteThread();
        return;
    }
    TerminateThread(threadId);
    libc_2DBCB0(threadId);
}

sdfThCB_t *func_002A87F0(s32 threadId) {
    sdfThCB_t *node;

    if (threadId < 0) {
        threadId = GetThreadId();
    }
    WaitSema(sdfThreadSema);
    node = sdfVblankThreadId;
    while (node != NULL) {
        if (node->threadId == threadId) {
            break;
        }
        node = node->next;
    }
    SignalSema(sdfThreadSema);
    return node;
}

void sdf_2A8858(void)
{
    func_002A8758(func_002A87F0(-1));
}

s32 VblankIntcHandler(s32 cause)
{
    u8 clock;

    if (cause == 2) {
        iWakeupThread(sdfVblankSema);
        clock = sdfCdClock;
        if (clock != 0) {
            sdfCdClock = clock - 1;
        }
    }
    return 0;
}

void VblankThreadProc(void) {
    sdfThCB_t *threadEntry;

    DmacAddHandler(0, 2, (s32)VblankIntcHandler, -1, 0);
    EnableIntc(2);
    for (;;) {
        SleepThread();
        sdfVblankFlag++;
        WaitSema(sdfThreadSema);
        for (threadEntry = sdfVblankThreadId; threadEntry != NULL; threadEntry = threadEntry->next) {
            WakeupThread(threadEntry->threadId);
        }
        SignalSema(sdfThreadSema);
    }
}

s32 sdfInitThreadSystem(void) {
    s32 threadId;

    InitThread();
    sdfVblankFlag = 0;
    sdfVblankThreadId = NULL;
    sdfThreadSema = sdfCreateSema(1, 1, 0);
    threadId = CreateThreadSimple((void *)VblankThreadProc, D_005776C0, 0x800, 0x3E);
    sdfVblankSema = threadId;
    return StartThread(threadId, 0);
}

void sdfSetMainThreadPriority(void)
{
    s32 thid = GetThreadId();

    D_00552054 = thid;
    ChangeThreadPriority(thid, 0x7C);
    SleepThread();
}
INCLUDE_RODATA(const s32, "code_0029B780", D_00548FD8);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548FF8);

INCLUDE_RODATA(const s32, "code_0029B780", D_00549018);

INCLUDE_SDATA(const s32, "code_0029B780", slLoadKillFlag);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C1);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C4);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516C8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516D0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516D8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516E0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516E8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005516F0);

INCLUDE_SDATA(const s32, "code_0029B780", dds3ConfigValue);

INCLUDE_SDATA(const s32, "code_0029B780", dds3ConfigBackup);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551700);

INCLUDE_SDATA(const s32, "code_0029B780", mapIndex);

INCLUDE_SDATA(const s32, "code_0029B780", mapPrev);

INCLUDE_SDATA(const s32, "code_0029B780", debugMode);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551711);

INCLUDE_SDATA(const s32, "code_0029B780", SectionIndex);

INCLUDE_SDATA(const s32, "code_0029B780", pipeInitDone);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551719);

INCLUDE_SDATA(const s32, "code_0029B780", pipeLoadState);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551724);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551728);

INCLUDE_SDATA(const s32, "code_0029B780", pbHitCheckCur);

INCLUDE_SDATA(const s32, "code_0029B780", offsetCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551738);

INCLUDE_SDATA(const s32, "code_0029B780", D_0055173C);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551740);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551744);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551748);

INCLUDE_SDATA(const s32, "code_0029B780", pipeIndex);

INCLUDE_SDATA(const s32, "code_0029B780", attackCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551751);

INCLUDE_SDATA(const s32, "code_0029B780", myMaka);

INCLUDE_SDATA(const s32, "code_0029B780", ZmakaCount);

INCLUDE_SDATA(const s32, "code_0029B780", playerDAMAGE);

INCLUDE_SDATA(const s32, "code_0029B780", rBoxNum);

INCLUDE_SDATA(const s32, "code_0029B780", offsetStep);

INCLUDE_SDATA(const s32, "code_0029B780", offsetStepCount);

INCLUDE_SDATA(const s32, "code_0029B780", CameraAction);

INCLUDE_SDATA(const s32, "code_0029B780", camMode);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551765);

INCLUDE_SDATA(const s32, "code_0029B780", myStatus);

INCLUDE_SDATA(const s32, "code_0029B780", pipeObjList);

INCLUDE_SDATA(const s32, "code_0029B780", pipeObjTail);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551770);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMyMakaCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551778);

INCLUDE_SDATA(const s32, "code_0029B780", hWorkArea);

INCLUDE_SDATA(const s32, "code_0029B780", WorkAreaPtr);

INCLUDE_SDATA(const s32, "code_0029B780", PartsWork);

INCLUDE_SDATA(const s32, "code_0029B780", PipeCur);

INCLUDE_SDATA(const s32, "code_0029B780", PipeCur2);

INCLUDE_SDATA(const s32, "code_0029B780", PipeLine);

INCLUDE_SDATA(const s32, "code_0029B780", ObjWork);

INCLUDE_SDATA(const s32, "code_0029B780", basicMy);

INCLUDE_SDATA(const s32, "code_0029B780", MyLine);

INCLUDE_SDATA(const s32, "code_0029B780", PipeEMS);

INCLUDE_SDATA(const s32, "code_0029B780", PipeEML);

INCLUDE_SDATA(const s32, "code_0029B780", PipeRBOX);

INCLUDE_SDATA(const s32, "code_0029B780", PipeMaka1);

INCLUDE_SDATA(const s32, "code_0029B780", PipeZMaka);

INCLUDE_SDATA(const s32, "code_0029B780", PipeDMaka);

INCLUDE_SDATA(const s32, "code_0029B780", PipeDRBOX);

INCLUDE_SDATA(const s32, "code_0029B780", MakaTex);

INCLUDE_SDATA(const s32, "code_0029B780", makaWork);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_CELL01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_BREAK01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_BREAK02_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_DAMAGE01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_DAMAGE03_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_ATTACK01_D3P);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517DC);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517E0);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517E8);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517F0);

INCLUDE_SDATA(const s32, "code_0029B780", pipe_Camera);

INCLUDE_SDATA(const s32, "code_0029B780", D_005517FC);

INCLUDE_SDATA(const s32, "code_0029B780", ResultIndex);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551801);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551808);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMessData);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551810);

INCLUDE_SDATA(const s32, "code_0029B780", pipeMessCount);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551815);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551818);

INCLUDE_SDATA(const s32, "code_0029B780", pipeBulrLimitValue);

INCLUDE_SDATA(const s32, "code_0029B780", sdfDmacHandlerTable);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551824);

INCLUDE_SDATA(const s32, "code_0029B780", D_00551828);

INCLUDE_SDATA(const s32, "code_0029B780", sdfVblankFlag);

