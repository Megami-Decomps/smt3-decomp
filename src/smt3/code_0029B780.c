#include "common.h"
#include "ps2asm.h"

typedef struct tagpipeBasic {
    void *base;
    void *mdl;
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
    void *handle;
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
extern void *sdfDmacHandlerTable;
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
extern s32 func_002A87F0(s32 arg0);
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
extern void SleepThread(void);
extern void sdfIpuRemoveHandler(void *handler);
extern s32 dds3KillProcess(void *id, s32 shutdowntimer);
extern void func_002A8758(s32 arg0);
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

INCLUDE_ASM(const s32, "code_0029B780", dds3ConfigProcessStart);

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

INCLUDE_ASM(const s32, "code_0029B780", DEBUG_PRINT);

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

INCLUDE_ASM(const s32, "code_0029B780", texLoad);

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

INCLUDE_ASM(const s32, "code_0029B780", AddCamera);

INCLUDE_ASM(const s32, "code_0029B780", CalcCamera);

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

INCLUDE_ASM(const s32, "code_0029B780", pipeMessDraw);

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

INCLUDE_ASM(const s32, "code_0029B780", mymdlSetMotion);

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

INCLUDE_ASM(const s32, "code_0029B780", pipeCameraLerp);

INCLUDE_ASM(const s32, "code_0029B780", vec_2A6B80);

INCLUDE_ASM(const s32, "code_0029B780", vecRotate2D);

INCLUDE_ASM(const s32, "code_0029B780", vec_2A6CA8);

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

INCLUDE_ASM(const s32, "code_0029B780", vecNormalize);

f32 vecLength(f32 *v)
{
    f32 len;

    EE_SQRT_F32(len, v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return len;
}

INCLUDE_ASM(const s32, "code_0029B780", vecDot);

f32 vecAngleBetween(f32 *a, f32 *b)
{
    return __ieee754_acosf(vecDot(a, b));
}

INCLUDE_ASM(const s32, "code_0029B780", vecTranslate);

INCLUDE_ASM(const s32, "code_0029B780", mtxRotateX);

INCLUDE_ASM(const s32, "code_0029B780", mtxRotateY);

INCLUDE_ASM(const s32, "code_0029B780", mtxRotateZ);

INCLUDE_ASM(const s32, "code_0029B780", mtxStore);

INCLUDE_ASM(const s32, "code_0029B780", mtx_2A7B10);

INCLUDE_ASM(const s32, "code_0029B780", mtx_2A7BB8);

f32 func_002A7DF8(f32 value, f32 count)
{
    f32 result = 1.0f;
    s32 i;

    for (i = 1; i <= count; i++) {
        result *= value;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_0029B780", vecDistance);

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

INCLUDE_ASM(const s32, "code_0029B780", CreateThreadSimple);

INCLUDE_ASM(const s32, "code_0029B780", sdfCreateThreadWithStack);

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

INCLUDE_ASM(const s32, "code_0029B780", FatalError);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548FD8);

INCLUDE_RODATA(const s32, "code_0029B780", D_00548FF8);

INCLUDE_ASM(const s32, "code_0029B780", DmacAddHandler);

INCLUDE_ASM(const s32, "code_0029B780", sdfIpuRemoveHandler);

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

INCLUDE_ASM(const s32, "code_0029B780", sdfDelayThread);

s32 sdfGetRunTimeCountBase(void)
{
    return *(volatile u32 *)0x10000000;
}

s32 sdfGetRunTimeCount(s32 aBaseTime)
{
    return (*(volatile u32 *)0x10000000 - aBaseTime) & 0xFFFF;
}

INCLUDE_ASM(const s32, "code_0029B780", sdfSystemThreadProc);

INCLUDE_ASM(const s32, "code_0029B780", dds3InitSubsystems);

INCLUDE_ASM(const s32, "code_0029B780", sdfStartThreadAndWait);

void sdfWaitVblank(s32 count)
{
    s32 rest = count - CancelWakeupThread(GetThreadId());

    do {
        SleepThread();
        rest--;
    } while (rest > 0);
}

INCLUDE_ASM(const s32, "code_0029B780", func_002A8758);

INCLUDE_ASM(const s32, "code_0029B780", func_002A87F0);

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

INCLUDE_ASM(const s32, "code_0029B780", VblankThreadProc);

INCLUDE_ASM(const s32, "code_0029B780", sdfInitThreadSystem);

void sdfSetMainThreadPriority(void)
{
    s32 thid = GetThreadId();

    D_00552054 = thid;
    ChangeThreadPriority(thid, 0x7C);
    SleepThread();
}
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

