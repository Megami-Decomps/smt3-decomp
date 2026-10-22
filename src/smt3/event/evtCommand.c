#include "common.h"
#include "event.h"
#include "kernel.h"
#include "scr.h"
#include "field.h"

struct effPCPAgiraoFirePartList_t;
extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, const struct effPCPAgiraoFirePartList_t *effdata);
extern dds3ProcessID_t *evtStartPolygonMovie2(u32 priority, s32 eventno, s32 cutno);


void scrSetIntReturnValue(s32 i);

extern s32 scrGetIntPara(s32 n);

extern s32 evtLoadUnit(s32 major, s32 minor);

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

dds3ProcessID_t *evtSearchEventPack(u32 eventno);

dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);

scrProcessWork_s *scrGetScriptHandle(void);

extern void datDebugPrintf(const char *fmt, ...);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

void evtSetUnitPos(u32 unitID, u32 posID);

void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

void Mult(void);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, u128 *rot);

f32 scrGetFloatPara(s32 n);

struct dds3Basic_t *dds3GetActiveScene(void);

extern void *memset(void *a, s32 b, u32 c);

extern u32 D_0054F754;

extern void evtRunUnitKutiMotion(u32 h);

void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

extern evtUnit_t *evtGetUnitHandle(s32 serial);

extern s32 scrGetMessageHandle(void);

extern void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);

s32 mdlChkMotionEndEx(const mdlObj_t *aHandle, s32 aGroup);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

void evtUnitLightOn(evtUnit_t *h);

void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

extern const char D_0053A948[];

void *evtGetResorceData(u32 eventno, s32 rid);

s32 printf(const char *format, ...);

extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(struct effPCPAgiraoFirePartList_t *effdata, void *pos, void *rot);

extern const char D_0053A990[];

extern const char D_0053A970[];

extern void * dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);

typedef struct effEventWork_t effEventWork_t;

void effEveBEDCutSetScale(effEventWork_t *hEveBEDCut, f32 scale);

extern void dds3StartBEDEffectObjectBasic(void *basic, u32 cutno);


extern f32 evtCalcEffBedScale(s32);

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

typedef struct evtModelHeader_t {
    u8 pad00[4];
    u32 flags;
    u8 pad08[8];
    f32 positionX;
    f32 positionY;
    f32 positionZ;
} evtModelHeader_t;


void dds3AdminiCallSequence(s32 sequence, s32 data, s32 datasize, s32 restore);

enum {
    EVT_SCALAR_REQUEST_SIZE = 4,
    EVT_PAIRED_REQUEST_SIZE = 8,
    EVT_FIELD_SEQUENCE_REQUEST_SIZE = 0xA0,
    EVT_WORLD_OBJECT_KIND_FIRST = 4,
    EVT_WORLD_OBJECT_KIND_LIMIT = 10,
    EVT_CAMP_TASK_READY_VALUE = 2
};

void scrAllReleaseScript(void);

void CallBattleSub(s32 encno, s32 eventno);

void CallBattleSub2(s32 encno);

char *scrGetStrPara(s32 n);

void fldBgmFadeOut(void);

void fldSetSceneParam2(s32 pParam, s32 loadField, s32 loadArea, char *pointResName);

extern s32 fldGb[];

void fldSetSceneParam3(s32 pParam, s32 loadField, s32 loadArea, char *pointResName);

void dds3SetPosPoseSet(dds3Basic_t *pBasic, u128 *pos);

struct dds3Basic_t *fldGetUnitFollowCamera(void);

void *evtSearchBasic(s32 type, s32 serial);


struct dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID);

typedef s32 (*dds3MoverUpdate_t)(struct dds3PoseSet_t *, struct dds3Basic_t *);

typedef struct dds3SlotResource_t {
    struct dds3Basic_t *target;
    dds3PathBasicControl_t *path;
    dds3MoverUpdate_t update;
    struct dds3Basic_t *sourceObject;
} dds3SlotResource_t;

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *pMoverBasic);


void dds3ObjectBaseCreateMover(void *pObjectBasic);

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);


s32 fldCommand_GetHojiID(char *pName);

s32 fldGbSetGimic(s32 fld, s32 area, s32 hojiID, s32 toplast);

extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);


extern char D_0053AD70[], D_0053AD80[], D_0053AD90[];

void evtEnableMoonCalc(void);

void evtDisableMoonCalc(void);

s32 evtGetAgeOfMoon(void);

s32 evtGetAgeOfMoon16(void);

void evtSetAgeOfMoon(s32 age);

dds3ProcessID_t *evtLoadEventPack(s32 eventno);

s32 evtGetEventPackStat(u32 eventno);

extern char D_0053ADE0[];

void sdfPrintf(const char *str, ...);

extern char D_0053ADD0[];

extern char D_0053ADF0[];

extern char D_0053AEB8[];

void evtPathRate(dds3Basic_t *basic, f32 rate);

void evtPathDir(dds3Basic_t *basic, u32 dir);

dds3ProcessID_t *evtStartPictureProcess(s32 priority, struct sdfTexHandle_t *texpath);

extern char D_0053AF38[];

typedef u8 sdfTextureClampMode_t;

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

struct sdfMemHandle_t *sdfDevLoadFileEx(
    const char *name, u32 *outAddress, u32 *outSize);


struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

extern sdfTexHandle_t *DeadMoonTmx;

void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);


#include "basic.h"
#include "ps2asm.h"
extern void dds3EffectObjectPresetMG12(dds3Basic_t *eff);

extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                                      dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target,
                                                 s32 index);

extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);

extern void *dds3GetControlScene(void);

extern s32 dds3GetMapNo(void *scene);

extern void dds3SetActiveCameraSceneBasic(void *pSceneBasic, dds3Basic_t *pCameraBasic);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, f32 *pos, f32 *rot);

extern s32 dds3AllocObjectId(void);

extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);

extern u32 dds3DrawFlag;

extern void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);

extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

extern s32 evtCheckPathStat(dds3Basic_t *obj);

extern s32 evtRegistBEField(s32 eventno, s32 rid);

extern s32 evtLoadArea(s32 fld, s32 area);

extern s32 evtStageAnimeControl(s32 type, s32 mode);

extern void evtFreeAllUnit(void);

extern void evtFreeStage(void);

extern void fldSetNextField(s32 command, s32 para);

extern s32 sdfDelayedResrcCheck(void);

extern char D_0053A9D8[];


extern char D_0053AA38[];

extern char D_0053AA88[];

extern void dds3ObjectBaseOnFlag(void *basic, s32 flag);

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3SetSclPoseSet(void *pBasic, u128 *scl);

extern s32 evtCheckUnitWait(evtUnit_t *h);

extern void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern void evtUnitLightOff(evtUnit_t *h, s32 frame);

extern u8 evtUnitLightAnimeStat(evtUnit_t *h);

extern char D_0053A8C8[];

extern char D_0053A908[];

extern void func_001E3C30(void *unit, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5);

extern void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);

extern void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);

extern void evtPathStop(dds3Basic_t *basic);

extern void evtPathStart(dds3Basic_t *basic);

extern void evtPathLoop(dds3Basic_t *basic, s32 sw);

extern void evtLoadEventBgm(s32 event);

extern s32 evtCheckEventBgmLoaded(s32 event);

extern s32 scrGetCommandTimer(void);

extern void evtSetSkyAlpha(s32 frame, s32 a);

extern s32 dds3ExistProcess(void *id);

extern void *DDS3_GBWK;

extern f32 sdfSin(f32 aRadius);

extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 rgba, sdfTexHandle_t *th);

/* Rows of 0x310 bytes; only the three size fields the bed scale reads are
 * named here. */
typedef struct {
    u8 unk00[0x10];
    f32 formsize;                   /* 0x10 */
    f32 formheight;                 /* 0x14 */
    f32 formscale;                  /* 0x18 */
    u8 unk1C[0x310 - 0x1C];
} datModelParam_t;

extern datModelParam_t datModelParamTable[];

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

#define EVT_RGBA_SCALE_128 0x43000000

extern void evtPictureSwap(void *id1, void *id2, s32 pat, s32 prm);

extern void evtPictureSetSize(void *id, s32 sw, s32 sh, s32 tw, s32 th);

extern void evtPictureSetPos(void *id, s32 x0, s32 y0, s32 x1, s32 y1, s32 time);

extern void evtPictureSetAlpha(void *id, s32 alp0, s32 alp1, s32 time);

extern void evtPictureSetZoom(void *id, s32 zoom0, s32 zoom1, s32 time);

extern s32 evtPictureStat(void *id);

extern void fldProc_SisetuReturnPoint(s32 fld, s32 area, char *pName);

extern void fldBgmStop(void);

extern void fldPauseBgm(void);

extern void p2sdPlaySE(s32 no, s32 volume, s32 pan);

extern void encStart(void);

extern s32 evtNextEventNo;

extern void evtSetPolygonMovieFlag(void *id, s32 flag);

extern void evtResetPolygonMovieFlag(void *id, s32 flag);

extern s32 evtCheckPolygonMovieFlag(void *id, s32 flag);

extern void sdfTexFreeDelayed(void *aHandle);

extern void evtSetUnitMove2(evtUnit_t *h, s32 movetype, s32 posID, s32 motion,
                            s32 para1, s32 para2, void *func, void *data);

extern char D_0053AE10[];

extern char D_0053AE28[];

extern char D_0053AE70[];

extern char D_0053AE88[];

extern char D_0053AE98[];

extern char D_0053AEA8[];


extern void evtPictureDrawOn(void *id);

extern void evtPictureDrawOff(void *id);

extern void evtKillPictureProcess(void *id);

extern char D_0053AF00[];

extern char D_0053AF18[];

extern char D_0053AFE8[];

extern char D_0053B010[];

extern s32 *dds3GetActiveUnitSceneBasic(void *pSceneBasic);

extern void fldSetFilterAnime(s32 on);

extern void fldSkySetAlpha(s32 alp);

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern char D_0053ADE0[];

extern dds3Basic_t *dds3SceneBasicSearchName(void *scene, const char *name);

extern dds3Basic_t *dds3AddMG12EffectObjectBasic(s32 type, const char *file);

extern void *evtStartPictureProcess2(s32 priority, const char *texdata);

extern char D_0053AFA0[];

extern char D_0053AC00[];

extern char D_0053AC20[];

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} evtVec_t;

extern dds3Basic_t *dds3AddD3PEffectObjectBasic(const char *file, evtVec_t *pos, evtVec_t *rot);

extern dds3Basic_t *dds3AddBEDEffectObjectBasic(const char *file, evtVec_t *pos, evtVec_t *rot);

extern char D_0053A9A8[];

extern void evtKillEventPack(s32 eventno);

extern char D_0053AD70[];

extern char D_0053AD80[];

extern char D_0053ADB0[];

extern char D_0053AE00[];

void *evtSearchBasic(s32 type, s32 serial)
{
    void *scene;

    scene = dds3GetActiveScene();
    return dds3SearchSceneBasic(scene, serial, type);
}

u32 evtCommand_MODEL_PLAYER(void)
{
    void *scene;
    s32 *basic;
    s32 key;

    scene = dds3GetActiveScene();
    basic = dds3GetActiveUnitSceneBasic(scene);
    key = -1;
    if (basic != NULL) {
        key = basic[1];
    }
    scrSetIntReturnValue(key);
    return 1;
}

u32 evtCommand_MODEL_DSP(void) {
    s32 slotKind;
    s32 resource;
    s32 viewerObjectKey;

    slotKind = scrGetIntPara(0);
    resource = scrGetIntPara(1);
    viewerObjectKey = evtLoadUnit(slotKind, resource);
    scrSetIntReturnValue(viewerObjectKey);
    return 1;
}

u32 evtCommand_MODEL_BE(void) {
    s32 eventId;
    s32 resourceId;
    s32 modelKey;
    dds3ProcessID_t *motionSeTask;

    if (scrGetScriptHandle() == 0) {
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    modelKey = evtInitMotionSeProcess(eventId, resourceId);
    if (modelKey < 0) {
        datDebugPrintf("MODEL_BE not fount RID = %d!\n", scrGetIntPara(1));
        return 1;
    }
    eventId = scrGetIntPara(0);
    resourceId = scrGetIntPara(1);
    motionSeTask = evtLoadModelSe(modelKey, eventId, resourceId);
    if (motionSeTask != 0) {
        dds3LinkProcess(evtSearchEventPack(scrGetIntPara(0)), motionSeTask);
    }
    /* The original success path ends in this void call, without a return. */
    scrSetIntReturnValue(modelKey);
}

u32 evtCommand_MODEL_POS(void) {
    u32 targetKey;
    u32 sourceKey;

    targetKey = scrGetIntPara(0);
    sourceKey = scrGetIntPara(1);
    evtSetUnitPos(targetKey, sourceKey);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_ON(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOnFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_ON2(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOnFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_OFF(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_POS_XYZ(void)
{
    f32 pos[4];
    void *scene;
    s32 key;
    void *unit;

    memset(pos, 0, 0x10);
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    pos[0] = scrGetFloatPara(1);
    pos[1] = scrGetFloatPara(2);
    pos[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(unit, (u128 *)pos);
    return 1;
}

u32 evtCommand_MODEL_ROT_XYZ(void) {
    f32 v[4];
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return 1;
    }
    toRad = 0.017453293f;
    x = scrGetFloatPara(1) * toRad;
    y = scrGetFloatPara(2) * toRad;
    FromEuler(x, y, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
    Mult();
    SDF_VU0_STOREVECTOR(v);
    dds3SetRotPoseSet(unit, (u128 *)v);
    return 1;
}

u32 evtCommand_MODEL_SCL_XYZ(void)
{
    f32 scl[4];
    void *scene;
    s32 key;
    void *unit;

    memset(scl, 0, 0x10);
    scl[3] = 1.0f;
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    scl[0] = scrGetFloatPara(1);
    scl[1] = scrGetFloatPara(2);
    scl[2] = scrGetFloatPara(3);
    dds3SetSclPoseSet(unit, (u128 *)scl);
    return 1;
}

u32 evt_1E50C0(void)
{
    void *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), scrGetIntPara(2), scrGetIntPara(3), 0);
    printf(D_0053A8C8);
    return 1;
}

u32 evt_1E5158(void)
{
    void *unit;
    s32 motion;
    s32 start;
    s32 end;
    s32 mode;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    motion = scrGetIntPara(1);
    start = scrGetIntPara(2);
    end = scrGetIntPara(3);
    mode = scrGetIntPara(4);
    evtSetUnitMotion(unit, motion, start, end, mode);
    return 1;
}

u32 evt_1E51F8(void)
{
    void *unit;
    s32 p1;
    s32 p2;
    s32 p3;
    s32 p4;
    s32 p5;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    p1 = scrGetIntPara(1);
    p2 = scrGetIntPara(2);
    p3 = scrGetIntPara(3);
    p4 = scrGetIntPara(4);
    p5 = scrGetIntPara(5);
    func_001E3C30(unit, p1, p2, p3, p4, p5);
    return 1;
}

void evtRunKutiMotion(void) {
    evtRunUnitKutiMotion(D_0054F754);
}

u32 evtCommand_KUTISET(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        D_0054F754 = (u32)unit;
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_KUTI(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 firstSlot = scrGetIntPara(1);
        s32 secondSlot = scrGetIntPara(2);
        evtSetUnitKutiMotion(unit, firstSlot, secondSlot);
        evtRunUnitKutiMotion((u32)unit);
        D_0054F754 = (u32)unit;
    }
    {
        s32 window = scrGetMessageHandle();
        if (window < 0) {
            return 1;
        }
        itfMesMngSetKutiCallback(window, evtRunKutiMotion);
    }
    return 1;
}

u32 evtCommand_MODEL_WAIT(void)
{
    s32 key;
    void *unit;
    u32 ret = 1;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return ret;
    }
    return evtCheckUnitWait(unit) != 0;
}

u32 evtCommand_MODEL_EXWAIT(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;
    s32 motionNodeIndex;
    u32 result = 1;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return result;
    }
    motionNodeIndex = scrGetIntPara(1);
    if (((unit->slotFlags[motionNodeIndex] & 1) & 0xFF) == 0) {
        return result;
    }
    return mdlChkMotionEndEx(unit->owner, scrGetIntPara(1)) != 0;
}

u32 evt_1E54B8(void) {
    void *ctx;
    s32 worldObjectKey;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetActiveScene();
    worldObjectKey = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(ctx, worldObjectKey, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3DeleteSceneBasicData(unit);
    return 1;
}

u32 evtCommand_MODEL_MOVE(void)
{
    evtUnit_t *unit;
    s32 posId;
    s32 moveType;
    s32 para1;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    posId = scrGetIntPara(2);
    moveType = scrGetIntPara(1);
    para1 = scrGetIntPara(3);
    evtSetUnitMove2(unit, posId, moveType, -1, para1, scrGetIntPara(4), NULL, NULL);
    return 1;
}

u32 evtCommand_MODEL_MOVE2(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    {
        s32 pathObjectKey = scrGetIntPara(1);
        s32 frames = scrGetIntPara(5);
        s32 directionOffset = scrGetIntPara(6);
        s32 pathFollowMode = scrGetIntPara(2);
        s32 dirFlag = scrGetIntPara(4);
        s32 sideMode = scrGetIntPara(3);
        evtSetUnitMove3(unit, pathObjectKey, frames, directionOffset, pathFollowMode, dirFlag, sideMode);
    }
    return 1;
}

u32 evtCommand_MODEL_WAIT_EXMOT(void) {
    evtUnit_t *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 tableValueIndex = scrGetIntPara(1);
        s32 value = scrGetIntPara(2);
        unit->tableValues[tableValueIndex] = value;
    }
    return 1;
}

/* Store the script-supplied value for the selected event unit. */
u32 evtCommand_MODEL_WAIT_MOT(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->waitMotion = scrGetIntPara(1);
    return 1;
}

/* Store the two script-supplied world-unit state values. */
u32 evtCommand_MODEL_WAIT_HOKAN(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->waitBlendStart = scrGetIntPara(1);
    unit->waitBlendEnd = scrGetIntPara(2);
    return 1;
}

u32 evtCommand_MODEL_MOVE_MOT(void)
{
    s32 key;
    void *unit;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), 5, 7, 1);
    printf(D_0053A908);
    return 1;
}

u32 evtCommand_MODEL_ROT(void) {
    s32 unitObjectKey;
    evtUnit_t *unit;
    s32 worldObjectKey;
    s32 frames;

    unitObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(unitObjectKey);
    if (unit == NULL) {
        return 1;
    }
    unit->motionTicks = 0;
    worldObjectKey = scrGetIntPara(1);
    frames = scrGetIntPara(2);
    evtSetUnitRot2(unit, worldObjectKey, frames);
    return 1;
}

u32 evtCommand_MODEL_ON(void)
{
    u32 **unit;

    unit = (u32 **)evtGetUnitHandle(scrGetIntPara(0));
    if (unit != NULL) {
        unit[35][0] &= ~1;
    }
    return 1;
}

u32 evtCommand_MODEL_OFF(void)
{
    u32 **unit;

    unit = (u32 **)evtGetUnitHandle(scrGetIntPara(0));
    if (unit != NULL) {
        unit[35][0] |= 1;
    }
    return 1;
}

u32 evtCommand_MODEL_LIGHT_ON(void) {
    s32 worldObjectKey;
    evtUnit_t *unit;

    worldObjectKey = scrGetIntPara(0);
    unit = evtGetUnitHandle(worldObjectKey);
    evtUnitLightOn(unit);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_OFF(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnitLightOff(unit, time);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_WAIT(void)
{
    s32 key;
    void *unit;
    s32 stat;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    stat = evtUnitLightAnimeStat(unit);
    return stat == 0;
}

u32 evtCommand_MODEL_LIGHT_RGB(void);

u32 evtCommand_MODEL_LIGHT_RGB(void)
{
    evtColorWork_t work[2];
    evtUnit_t *unit;
    u32 light;
    u32 ambient;
    u32 scale;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    scale = EVT_RGBA_SCALE_128;
    MMI_RGBA_PACK_UNIT(light, scale);
    work[0].word = light;
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(5));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(6));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(7));
    SDF_VU0_VECTOR_W_SET_ONE();
    MMI_RGBA_PACK_UNIT(ambient, scale);
    work[1].word = ambient;
    evtSetUnitLightAndAmbient(unit, scrGetIntPara(1), light, ambient);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_DIR(void);

u32 evtCommand_MODEL_LIGHT_DIR(void)
{
    f32 dir[4];
    evtUnit_t *unit;
    f32 mag;

    memset(dir, 0, sizeof(dir));
    unit = evtGetUnitHandle(scrGetIntPara(0));
    dir[0] = scrGetFloatPara(2);
    dir[1] = scrGetFloatPara(3);
    dir[2] = scrGetFloatPara(4);
    mag = sdfFabs(dir[0]);
    if (mag <= 0.01f) {
        mag = sdfFabs(dir[1]);
        if (mag <= 0.01f) {
            mag = sdfFabs(dir[2]);
            if (mag <= 0.01f) {
                datDebugPrintf(D_0053A948);
                return 1;
            }
        }
    }
    SDF_VU0_LOADVECTOR(dir);
    evtSetUnitLightDir(unit, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_RGB(void);

/* NON_MATCHING: 2 of 47 words differ (first at +0x94). */
#ifdef NON_MATCHING
u32 evtCommand_MODEL_RGB(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    evtSetUnitColorRGB(unit, scrGetIntPara(1), color);
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_RGB);
#endif

u32 evtCommand_MODEL_A(void);

u32 evtCommand_MODEL_A(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_LOADIDENTITYVECTOR();
    SDF_VU0_SETVECTOR_W(scrGetFloatPara(2));
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    evtSetUnitColorA(unit, scrGetIntPara(1), color);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_LINK(void)
{
    s32 key;
    void *unit;
    void *target;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    target = evtSearchBasic(9, scrGetIntPara(2));
    if (target == NULL) {
        return 1;
    }
    evtLinkLightObjectBasic(unit, target, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_LIGHT_UNLINK(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnlinkLightObjectBasic(unit, time);
    return 1;
}

u32 evtCommand_EFFECT_BE(void) {
    u8 firstVector[16];
    u8 secondVector[16];
    s32 taskId;
    s32 key;
    struct effPCPAgiraoFirePartList_t *descriptor;
    dds3Basic_t *object;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    descriptor = evtGetResorceData(taskId, key);
    if (descriptor == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf(D_0053A970, key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddD3PEffectObjectBasic2(descriptor, firstVector, secondVector);
    if (object == NULL) {
        printf(D_0053A990, 1);
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFECT_LOAD(void)
{
    evtVec_t pos;
    evtVec_t scale;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    basic = dds3AddD3PEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(D_0053A9A8);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

u32 evtCommand_EFFBED_LOAD(void)
{
    evtVec_t pos;
    evtVec_t scale;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;
    basic = dds3AddBEDEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(D_0053A9A8);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A8C8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A908);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A948);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A970);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A990);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A9A8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A9D8);

u32 evtCommand_EFFBED_BE(void) {
    f32 position[4];
    f32 scale[4];
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFBED_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = (dds3Basic_t *)dds3AddBEDEffectObjectBasic2(resource, position, scale);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFBED_BE_EVO(void) {
    return 1;
}

u32 evtCommand_EFFBED_START(void)
{
    s32 key;
    dds3Basic_t *basic;
    u32 entry;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        entry = scrGetIntPara(1);
        if (entry >= 3U) {
            datDebugPrintf("error EFFBED_START cut no over = %d\n", entry);
            return 1;
        } else {
            dds3StartBEDEffectObjectBasic(basic, entry);
        }
    }
    return 1;
}

f32 evtCalcEffBedScale(s32 uid)
{
    f32 formsize;
    f32 formheight;
    f32 scale;

    formsize = datModelParamTable[uid].formsize;
    formheight = datModelParamTable[uid].formheight;
    scale = (formsize + formheight * 0.5f) * 0.5f * datModelParamTable[uid].formscale * 0.014285714f;
    if (2.0f < scale) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

u32 evtCommand_EFFBED_START_EVO(void) {
    void *unit;

    unit = evtSearchBasic(7, scrGetIntPara(0));
    if (unit != NULL) {
        s32 entryId = scrGetIntPara(1);
        s32 modelRecordIndex;
        if ((u32)entryId >= 3) {
            datDebugPrintf("error EFFBED_START cut no over = %d\n", entryId);
            return 1;
        }
        dds3StartBEDEffectObjectBasic(unit, entryId);
        modelRecordIndex = scrGetIntPara(2);
        if (modelRecordIndex >= 0) {
            f32 value = evtCalcEffBedScale(modelRecordIndex);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                effEveBEDCutSetScale(target, value);
            }
        }
    }
    return 1;
}

u32 evtCommand_EFFMG1_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(1, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AA38);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AA38);

u32 evtCommand_EFFMG1_BE(void) {
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFMG1_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(1, (struct effPCPAgiraoFirePartList_t *)resource);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFMG2_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(2, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AA88);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AA88);

u32 evtCommand_EFFMG2_BE(void) {
    s32 taskId;
    s32 key;
    void *resource;
    dds3Basic_t *object;

    taskId = scrGetIntPara(0);
    key = scrGetIntPara(1);
    resource = evtGetResorceData(taskId, key);
    if (resource == NULL) {
        key = scrGetIntPara(1);
        datDebugPrintf("EFFMG2_BE not fount RID = %d\n", key);
        scrSetIntReturnValue(0);
        return 1;
    }
    object = dds3AddMG12EffectObjectBasic2(2, (struct effPCPAgiraoFirePartList_t *)resource);
    if (object == NULL) {
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(object, 1);
        scrSetIntReturnValue(object->serialID);
    }
    return 1;
}

u32 evtCommand_EFFMG2_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_CLS(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3DeleteSceneBasicData(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_POS_XYZ(void) {
    dds3Basic_t *obj;
    evtModelHeader_t *header;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    header = (evtModelHeader_t *)obj->workdata;
    if (header->flags & 4) {
        header->positionX = scrGetFloatPara(1);
        header->positionY = scrGetFloatPara(2);
        header->positionZ = scrGetFloatPara(3);
    } else {
        obj->poseset->pos.x = scrGetFloatPara(1);
        obj->poseset->pos.y = scrGetFloatPara(2);
        obj->poseset->pos.z = scrGetFloatPara(3);
        obj->poseset->flag = (obj->poseset->flag | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    }
    return 1;
}

u32 evtCommand_EFFECT_ROT_XYZ(void) {
    f32 v[4];
    dds3Basic_t *obj;
    f32 toRad;
    f32 x;
    f32 y;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    obj = evtSearchBasic(7, scrGetIntPara(0));
    if (!(((evtModelHeader_t *)obj->workdata)->flags & 4)) {
        toRad = 0.017453293f;
        x = scrGetFloatPara(1) * toRad;
        y = scrGetFloatPara(2) * toRad;
        FromEuler(x, y, 0.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * toRad);
        Mult();
        SDF_VU0_STOREVECTOR(v);
        dds3SetRotPoseSet(obj, (u128 *)v);
    }
    return 1;
}

u32 evtCommand_EFFECT_POS(void) {
    dds3Basic_t *obj;
    dds3Basic_t *source;
    evtWorldSourceTransformPrefix_t *vec;
    dds3PoseSet_t *params;

    obj = evtSearchBasic(7, scrGetIntPara(0));
    if (obj == NULL) {
        return 1;
    }
    source = evtSearchBasic(0x11, scrGetIntPara(1));
    if (source == NULL) {
        return 1;
    }
    vec = source->workdata;
    if (!(((evtModelHeader_t *)obj->workdata)->flags & 4)) {
        params = obj->poseset;
        params->pos.x = vec->position.x;
        params->pos.y = vec->position.y;
        params->pos.z = vec->position.z;
        params->rot.x = vec->rotation[0];
        params->rot.y = vec->rotation[1];
        params->rot.z = vec->rotation[2];
        params->rot.w = vec->rotation[3];
    }
    obj->poseset->flag = (obj->poseset->flag | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) & ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    return 1;
}

s32 evtCommand_EFFMG1_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG1_POS mg1 ID error!\n");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    if (point0 == NULL || point1 == NULL) {
        datDebugPrintf("EFFMG1_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, NULL, NULL) == 0) {
        datDebugPrintf("EFFMG1_POS set error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFMG2_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;
    dds3Basic_t *point2;
    dds3Basic_t *point3;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG2_POS mg2 ID error!\n");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    point2 = evtSearchBasic(7, scrGetIntPara(3));
    point3 = evtSearchBasic(7, scrGetIntPara(4));
    if (point0 == NULL || point1 == NULL || point2 == NULL || point3 == NULL) {
        datDebugPrintf("EFFMG2_POS point ID not found!\n");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, point2, point3) == 0) {
        datDebugPrintf("EFFMG2_POS set error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFECT_ON(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 1);
        dds3EffectObjectBasicStatOn(basic, 1);
    }
    return 1;
}

s32 evtCommand_EFFECT_LINK(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObject(effect, owner);
    return 1;
}

s32 evtCommand_EFFECT_LINK2(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObjectIndex(effect, owner, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_EFFECT_UNLINK(void)
{
    dds3Basic_t *basic;

    basic = evtSearchBasic(7, scrGetIntPara(0));
    if (basic == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(basic);
    return 1;
}

s32 evtCommand_EFFECT_OFF(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 0);
    }
    return 1;
}

s32 evtCommand_EFFECT_PATH_MOVE(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *path;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFECT_PATH_MOVE not found eff!\n");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        datDebugPrintf("EFFECT_PATH_MOVE not found path!\n");
        return 1;
    }
    evtPathLinkAndMake(path, effect);
    return 1;
}

s32 evtCommand_EFFECT_PATH_WAIT(void)
{
    s32 key = scrGetIntPara(0);
    dds3Basic_t *basic = evtSearchBasic(7, key);
    s32 stat;

    if (basic == NULL) {
        datDebugPrintf("EFFECT_PATH_WAIT not found eff!\n");
        return 1;
    }
    stat = evtCheckPathStat(basic);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

u32 evtCommand_SEARCH_ID(void)
{
    void *scene;
    dds3Basic_t *basic;

    scene = dds3GetControlScene();
    basic = dds3SceneBasicSearchName(scene, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AC00, scrGetStrPara(0));
        datDebugPrintf(D_0053AC20, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

s32 evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

s32 evtCommand_CALL_STAFF(void)
{
    dds3AdminiCallSequence(0x10, 0, 0, 0);
    return 1;
}

s32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(5, 0, 0, 0);
    return 1;
}

u32 evtCommand_CALL_COMBINE(void)
{
    fldProc_SisetuReturnPoint(fldGlobalBase.area, fldGlobalBase.floor + 1, scrGetStrPara(0));
    dds3AdminiCallSequence(0x1b, 0, 0, 0);
    scrSetIntReturnValue(0);
    return 1;
}

u32 evtCommand_CALL_RECOVER(void)
{
    fldProc_SisetuReturnPoint(fldGlobalBase.area, fldGlobalBase.floor + 1, scrGetStrPara(0));
    dds3AdminiCallSequence(0x1e, 0, 0, 0);
    scrSetIntReturnValue(0);
    return 1;
}

u32 evtCommand_CALL_SHOP(void)
{
    fldProc_SisetuReturnPoint(fldGlobalBase.area, fldGlobalBase.floor + 1, scrGetStrPara(0));
    dds3AdminiCallSequence(0x1c, 0, 0, 0);
    scrSetIntReturnValue(0);
    return 1;
}

u32 evtCommand_CALL_RAG(void)
{
    fldProc_SisetuReturnPoint(fldGlobalBase.area, fldGlobalBase.floor + 1, scrGetStrPara(0));
    dds3AdminiCallSequence(0x1d, 0, 0, 0);
    scrSetIntReturnValue(0);
    return 1;
}

/* Forward two VM integers as the admin request's two-word payload. */
s32 evtCommand_CALL_SAVE(void)
{
    s32 payload[2];

    payload[0] = scrGetIntPara(0);
    payload[1] = scrGetIntPara(1);
    dds3AdminiCallSequence(0x1F, (s32)payload, EVT_PAIRED_REQUEST_SIZE, 0);
    scrSetIntReturnValue(0);
    return 1;
}

u32 evtCommand_CALL_PUZZLEBOY(void)
{
    s32 request[1];
    s32 stage;

    fldBgmFadeOut();
    stage = scrGetIntPara(0);
    request[0] = stage;
    fldSetNextField(fldGb[0], stage + 1000);
    dds3AdminiCallSequence(0x26, (s32)request, 4, 0);
    scrAllReleaseScript();
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AC00);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AC20);

s32 evtCommand_CALL_NEXT(void)
{
    s32 para0 = scrGetIntPara(0);
    s32 para1 = scrGetIntPara(1);
    s32 para2 = scrGetIntPara(2);
    s32 command;

    printf("CALL_NEXT(%d,%d,%d)\n", para0, para1, para2);
    command = scrGetIntPara(1);
    fldSetNextField(command, scrGetIntPara(2));
    return 1;
}

/* The event ID becomes a four-byte dispatch payload. */
s32 evtCommand_CALL_EVENT(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    printf("call_event:%d\n", eventId);
    dds3AdminiCallSequence(7, (s32)&eventId, EVT_SCALAR_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

u32 evtCommand_EXIT_EVENT(void) {
    dds3AdminiExitSequence();
    return 1;
}

void CallBattleSub(s32 encno, s32 eventno)
{
    s32 request[2];

    fldBgmStop();
    p2sdPlaySE(0xF, 0x7F, 0x3F);
    encStart();
    request[0] = 0;
    request[1] = encno;
    evtNextEventNo = eventno;
    dds3AdminiCallSequence(0x13, (s32)request, 8, eventno > 0);
    scrAllReleaseScript();
}

s32 evtCommand_CALL_BATTLE(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub(eventId, 0);
    return 1;
}

void CallBattleSub2(s32 encno)
{
    s32 request[2];

    fldPauseBgm();
    p2sdPlaySE(0xF, 0x7F, 0x3F);
    encStart();
    request[0] = 0;
    request[1] = encno;
    evtNextEventNo = 0;
    dds3AdminiCallSequence(0x13, (s32)request, 8, 1);
    scrAllReleaseScript();
}

s32 evtCommand_CALL_BATTLE2(void)
{
    s32 eventId;

    eventId = scrGetIntPara(0);
    CallBattleSub2(eventId);
    return 1;
}

s32 evtCommand_RESTORE_BATTLE(void)
{
    s32 eventId;
    s32 requestMode;

    eventId = scrGetIntPara(0);
    requestMode = scrGetIntPara(1);
    CallBattleSub(eventId, requestMode);
    return 1;
}

/* A field-sequence request is a fixed 0xa0-byte VM message. */
s32 evtCommand_CALL_FIELD(void)
{
    s32 stage;
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    fldBgmFadeOut();
    stage = scrGetIntPara(0);
    kind = scrGetIntPara(1);
    sequenceName = scrGetStrPara(2);
    fldSetSceneParam2((s32)requestBuffer, stage, kind, sequenceName);
    dds3AdminiCallSequence(6, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

/* Use the current group's stage with the VM's kind and sequence-name inputs. */
s32 evtCommand_CALL_FIELD2(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam2((s32)requestBuffer, fldGb[0], kind, sequenceName);
    dds3AdminiCallSequence(6, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

/* Build the current group's field request using the alternate initializer. */
s32 evtCommand_CALL_FIELD3(void)
{
    s32 kind;
    char *sequenceName;
    u8 requestBuffer[EVT_FIELD_SEQUENCE_REQUEST_SIZE];

    kind = scrGetIntPara(0);
    sequenceName = scrGetStrPara(1);
    fldSetSceneParam3((s32)requestBuffer, fldGb[0], kind, sequenceName);
    dds3AdminiCallSequence(6, (s32)requestBuffer, EVT_FIELD_SEQUENCE_REQUEST_SIZE, 0);
    scrAllReleaseScript();
    return 1;
}

s32 evtCommand_FIELD_BE(void)
{
    s32 event;
    s32 resource;

    sdfPrintf("FIELD_BE start\n");
    event = scrGetIntPara(0);
    resource = scrGetIntPara(1);
    evtRegistBEField(event, resource);
    sdfPrintf("FIELD_BE end\n");
    return 1;
}

s32 evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    s32 mapno;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    mapno = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != mapno) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}

s32 evtCommand_FIELD_ANIME_START(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(0, value);
    return 1;
}

s32 evtCommand_FIELD_ANIME_STOP(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(1, value);
    return 1;
}

s32 evtCommand_FIELD_CLS(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown !!\n");
    datDebugPrintf("field shutdown.\n");
    return 1;
}

s32 evtCommand_FIELD_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown2 !!\n");
    datDebugPrintf("field shutdown2.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!\n");
    datDebugPrintf("unit all clear.\n");
    return 1;
}

s32 evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!\n");
    datDebugPrintf("unit all clear2.\n");
    return 1;
}

u32 evtCommand_CAM_SEL(void)
{
    dds3Basic_t *camera;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        fldCamSyukanSuriawase();
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    return 1;
}

u32 evtCommand_CAM_SEL_FOVY(void)
{
    dds3Basic_t *camera;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
        *(u32 *)((u8 *)camera->workdata + 0x88) |= 1;
    }
    if (camera == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    return 1;
}

s32 evtCommand_CAM_PATH_MOVE(void)
{
    dds3Basic_t *camera;
    dds3Basic_t *path;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    if (evtPathLinkAndMake(path, camera) == 0) {
        printf("CAM_PATH_MOVE error!\n");
        return 1;
    }
    return 1;
}

s32 evtCommand_CAM_PATH_WAIT(void)
{
    dds3Basic_t *camera;
    s32 stat;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        return 1;
    }
    stat = evtCheckPathStat(camera);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

s32 evtCommand_CAM_CREATE(void)
{
    f32 pos[4];
    f32 rot[4];
    s32 id;

    memset(pos, 0, 0x10);
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    id = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(id, pos, rot), 1.0f);
    scrSetIntReturnValue(id);
    return 1;
}

s32 evtCommand_CAM_CLS(void)
{
    s32 key;
    dds3Basic_t *camera;

    key = scrGetIntPara(0);
    camera = evtSearchBasic(4, key);
    if (camera == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(camera);
    return 1;
}

s32 evtCommand_CAM_POS_XYZ(void) {
    void *effectUnit;
    f32 position[4];

    memset(position, 0, sizeof(position));
    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    position[0] = scrGetFloatPara(1);
    position[1] = scrGetFloatPara(2);
    position[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(effectUnit, (u128 *)position);
    return 1;
}

/* Convert the three degree inputs before composing and storing a quaternion. */
s32 evtCommand_CAM_ROT_XYZ(void) {
    void *effectUnit;
    f32 quaternion[4];
    f32 degreesToRadians;
    f32 pitch;

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    degreesToRadians = 0.017453293f;
    pitch = scrGetFloatPara(1) * degreesToRadians;
    FromEuler(pitch, scrGetFloatPara(2) * degreesToRadians, 0.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    FromEuler(0.0f, 0.0f, scrGetFloatPara(3) * degreesToRadians);
    Mult();
    SDF_VU0_STOREVECTOR(quaternion);
    dds3SetRotPoseSet(effectUnit, (u128 *)quaternion);
    return 1;
}

s32 evtCommand_CAM_ROT_Q(void) {
    void *effectUnit;
    f32 quaternion[4];

    if (scrGetIntPara(0) < 0) {
        effectUnit = fldGetUnitFollowCamera();
    } else {
        effectUnit = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (effectUnit == NULL) {
        return 1;
    }
    quaternion[0] = scrGetFloatPara(1);
    quaternion[1] = scrGetFloatPara(2);
    quaternion[2] = scrGetFloatPara(3);
    quaternion[3] = scrGetFloatPara(4);
    dds3SetRotPoseSet(effectUnit, (u128 *)quaternion);
    return 1;
}

s32 evtCommand_OBJ_PATH_MOVE(void) {
    void *unit;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *data;
    dds3Basic_t *slotNode;
    dds3PathBasicControl_t *path;
    dds3SlotResource_t *resource;
    s32 roomNumber;

    unit = evtSearchBasic(6, scrGetIntPara(0));
    if (unit == NULL) {
        unit = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (unit == NULL) {
            printf(D_0053AD70, scrGetIntPara(0));
            printf(D_0053AD80, scrGetIntPara(1));
            printf(D_0053AD90);
            return 1;
        }
    }
    target = evtSearchBasic(0x10, scrGetIntPara(1));
    if (target == NULL)
        return 1;
    data = dds3MatterObjectBasicGetControlWorkData(unit);
    slotNode = data->control[1];
    if (slotNode == NULL) {
        dds3ObjectBaseCreateMover(unit);
        slotNode = data->control[1];
    }
    resource = slotNode->workdata;
    if (resource->sourceObject == NULL) {
        dds3MoverBasicSetPath(slotNode, target);
        dds3MoverBasicMakePath(slotNode);
    }
    path = dds3MoverBasicGetPathControl(slotNode);
    if (path == 0)
        return 1;
    switch (scrGetIntPara(2)) {
    case 0:
        dds3PathBasicControlAlterMode(path, 0);
        if (target->name != 0) {
            roomNumber = fldCommand_GetHojiID((char *)target->name);
            if (roomNumber > 0)
                fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        }
        break;
    case 1:
        dds3PathBasicControlAlterMode(path, 1);
        if (target->name != 0) {
            roomNumber = fldCommand_GetHojiID((char *)target->name);
            if (roomNumber > 0)
                fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
        break;
    }
    return 1;
}

s32 evtCommand_OBJ_PATH_WAIT(void)
{
    void *obj;
    s32 stat;

    obj = evtSearchBasic(6, scrGetIntPara(0));
    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            printf(D_0053AD70, scrGetIntPara(0));
            printf(D_0053AD80, scrGetIntPara(1));
            printf(D_0053ADB0);
            return 1;
        }
    }
    stat = evtCheckPathStat(obj);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

s32 evtCommand_MOON_START(void)
{
    evtEnableMoonCalc();
    return 1;
}

s32 evtCommand_MOON_STOP(void)
{
    evtDisableMoonCalc();
    return 1;
}

s32 evtCommand_GET_MOON(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon());
    return 1;
}

s32 evtCommand_GET_MOON16(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16());
    return 1;
}

s32 evtCommand_CHG_MOON(void)
{
    evtSetAgeOfMoon(scrGetIntPara(0));
    return 1;
}

/* Create a missing camp task, then wait for its ready value.
 * No VM context completes immediately; a pending task returns zero. */
s32 evtCommand_LOAD_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    char *message;

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) == 0) {
        message = D_0053ADD0;
        datDebugPrintf(message, taskId);
        sdfPrintf(message, taskId);
        dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(taskId));
        return 0;
    }
    if (evtGetEventPackStat(taskId) == EVT_CAMP_TASK_READY_VALUE) {
        message = D_0053ADE0;
        datDebugPrintf(message, taskId);
        sdfPrintf(message, taskId);
        return 1;
    }
    return 0;
}

/* Start the camp task only if no task with this ID exists yet.
 * A missing VM context completes without creating a task. */
s32 evtCommand_READ_BE(void) {
    s32 taskId = scrGetIntPara(0);
    scrProcessWork_s *commandWork = scrGetScriptHandle();

    if (commandWork == NULL) {
        return 1;
    }
    if (evtSearchEventPack(taskId) != 0) {
        return 1;
    }
    datDebugPrintf(D_0053ADF0, taskId);
    dds3LinkProcess(commandWork->ProcessID, evtLoadEventPack(taskId));
    return 1;
}

u32 evtCommand_CHK_BE(void)
{
    s32 no;
    s32 ret;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() != NULL && evtSearchEventPack(no) != 0
            && evtGetEventPackStat(no) == 2) {
        datDebugPrintf(D_0053ADE0, no);
        ret = 1;
    } else {
        ret = 0;
    }
    scrSetIntReturnValue(ret);
    return 1;
}

u32 evtCommand_DEL_BE(void)
{
    s32 no;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() == NULL) {
        return 1;
    }
    if (evtSearchEventPack(no) == 0) {
        return 1;
    }
    datDebugPrintf(D_0053AE00, no);
    evtKillEventPack(no);
    return 1;
}

u32 evtCommand_PMV_LOAD(void)
{
    scrProcessWork_s *work;
    dds3ProcessID_t *movieTask;

    printf(D_0053AE10, scrGetIntPara(0), scrGetIntPara(1));
    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->ProcessID == NULL) {
        printf(D_0053AE28);
        return 1;
    }
    movieTask = evtStartPolygonMovie2(work->ProcessID->priority, scrGetIntPara(0), scrGetIntPara(1));
    datDebugPrintf(D_0053AE70, scrGetIntPara(0), scrGetIntPara(1));
    dds3LinkProcess(work->ProcessID, movieTask);
    evtSetPolygonMovieFlag(movieTask, 1);
    scrSetIntReturnValue((s32)movieTask);
    printf(D_0053AE88);
    return 1;
}

u32 evtCommand_PMV_RUN2(void)
{
    void *id;

    printf(D_0053AE98);
    id = (void *)scrGetIntPara(0);
    evtResetPolygonMovieFlag(id, 1);
    scrSetIntReturnValue((s32)id);
    printf(D_0053AEA8);
    return 1;
}

u32 evtCommand_PMV_LOAD_END_WAIT(void)
{
    return evtCheckPolygonMovieFlag((void *)scrGetIntPara(0), 8) != 0;
}

/* Create and return an event/scene movie task without setting bit 0.
 * The owning scheduler task supplies the viewer task's priority. */
s32 evtCommand_PMV_RUN(void) {
    scrProcessWork_s *commandWork = scrGetScriptHandle();
    s32 eventId;
    s32 sceneId;
    dds3ProcessID_t *movieTask;

    if (commandWork == NULL) {
        return 1;
    }
    if (commandWork->ProcessID == 0) {
        printf(D_0053AEB8);
        return 1;
    }
    eventId = scrGetIntPara(0);
    sceneId = scrGetIntPara(1);
    movieTask = evtStartPolygonMovie2(commandWork->ProcessID->priority, eventId, sceneId);
    dds3LinkProcess(commandWork->ProcessID, movieTask);
    scrSetIntReturnValue((s32)(u32)movieTask);
    return 1;
}

s32 evtCommand_PATH_STOP(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathStop(obj);
    return 1;
}

s32 evtCommand_PATH_START(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathStart(obj);
    return 1;
}

/* Apply the float state, then update a room flag when its parsed number is positive.
 * Values above 0.5 set that flag; the VM float parameter is deliberately re-read. */
s32 evtCommand_PATH_RATE(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        worldNode = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    evtPathRate(worldNode, scrGetFloatPara(1));
    roomName = (char *)worldNode->name;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetFloatPara(1) > 0.5f) {
            fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        } else {
            fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

/* Apply the integer state and update a positive parsed room number's flag.
 * Unlike the float command, an integer value of zero sets the room flag. */
s32 evtCommand_PATH_DIR(void) {
    dds3Basic_t *worldNode;
    s32 roomNumber;
    char *roomName;
    s32 objectKind = EVT_WORLD_OBJECT_KIND_FIRST;

    do {
        worldNode = evtSearchBasic(objectKind, scrGetIntPara(0));
        objectKind++;
    } while (objectKind < EVT_WORLD_OBJECT_KIND_LIMIT && worldNode == NULL);
    if (worldNode == NULL) {
        worldNode = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (worldNode == NULL) {
            return 1;
        }
    }
    evtPathDir(worldNode, scrGetIntPara(1));
    roomName = (char *)worldNode->name;
    if (roomName == 0) {
        return 1;
    }
    roomNumber = fldCommand_GetHojiID(roomName);
    if (roomNumber > 0) {
        if (scrGetIntPara(1) == 0) {
            fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 1);
        } else {
            fldGbSetGimic(fldGlobalBase.area, fldGlobalBase.floor + 1, roomNumber, 0);
        }
    }
    return 1;
}

s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 1);
    return 1;
}

s32 evtCommand_PATH_LOOP_OFF(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 0);
    return 1;
}

s32 evtCommand_LIGHT_PATH_MOVE(void)
{
    void *light;

    light = evtSearchBasic(9, scrGetIntPara(0));
    if (evtPathLinkAndMake(evtSearchBasic(0x10, scrGetIntPara(1)), light) != 0) {
        return 1;
    }
    printf(D_0053AF00);
    datDebugPrintf(D_0053AF18);
    return 1;
}

s32 evtCommand_LIGHT_PATH_WAIT(void)
{
    s32 stat;

    stat = evtCheckPathStat(evtSearchBasic(9, scrGetIntPara(0)));
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AD70);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AD80);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AD90);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADB0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADD0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADE0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADF0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE00);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE10);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE28);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE70);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE88);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE98);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AEA8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AEB8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF00);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF18);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF38);

u32 evtCommand_TEX_BE(void) {
    scrProcessWork_s *work;
    s32 id;
    struct sdfTexHandle_t *entry;
    dds3ProcessID_t *task;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->ProcessID == NULL) {
        printf(D_0053AF38);
        return 1;
    }
    id = scrGetIntPara(0);
    entry = evtGetResorceData(id, scrGetIntPara(1));
    if (entry == 0) {
        datDebugPrintf("TEX_BE not fount RID = %d\n", scrGetIntPara(1));
        return 1;
    }
    task = evtStartPictureProcess(0x2AFE, entry);
    dds3LinkProcess(work->ProcessID, task);
    scrSetIntReturnValue((s32)task);
    return 1;
}

u32 evtCommand_TEX_LOAD(void)
{
    void *pw;
    void *process;

    pw = scrGetScriptHandle();
    if (pw == NULL) {
        return 1;
    }
    if (*(s32 *)((u8 *)pw + 0xE4) == 0) {
        printf(D_0053AFA0);
        return 1;
    }
    process = evtStartPictureProcess2(0x2AFE, scrGetStrPara(0));
    dds3LinkProcess(*(void **)((u8 *)pw + 0xE4), process);
    scrSetIntReturnValue((s32)process);
    return 1;
}

u32 evtCommand_TEX_ON(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOn(id);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOff(id);
    }
    return 1;
}

u32 evtCommand_TEX_CLS(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtKillPictureProcess(id);
    }
    return 1;
}

u32 evtCommand_TEX_SWAP(void)
{
    void *id1;
    void *id2;
    s32 pat;
    s32 prm;

    id1 = (void *)scrGetIntPara(0);
    id2 = (void *)scrGetIntPara(1);
    pat = scrGetIntPara(2);
    prm = scrGetIntPara(3);
    if (dds3ExistProcess(id1) != 0) {
        if (dds3ExistProcess(id2) != 0) {
            evtPictureSwap(id1, id2, pat, prm);
        }
    }
    return 1;
}

u32 evtCommand_TEX_OT(void) {
    u64 id;
    u64 fade;

    id = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPictureSetOt(id, fade);
    return 1;
}

u32 evtCommand_TEX_PRI(void) {
    u64 task;
    u64 value;

    task = scrGetIntPara(0);
    value = scrGetIntPara(1);
    evtPictureSetPri(task, value);
    return 1;
}

u32 evtCommand_TEX_SIZE(void)
{
    void *id;
    s32 sw;
    s32 sh;
    s32 tw;

    id = (void *)scrGetIntPara(0);
    sw = scrGetIntPara(1);
    sh = scrGetIntPara(2);
    tw = scrGetIntPara(3);
    evtPictureSetSize(id, sw, sh, tw, scrGetIntPara(4));
    return 1;
}

u32 evtCommand_TEX_POS(void)
{
    void *id;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    id = (void *)scrGetIntPara(0);
    x0 = scrGetIntPara(1);
    y0 = scrGetIntPara(2);
    x1 = scrGetIntPara(3);
    y1 = scrGetIntPara(4);
    evtPictureSetPos(id, x0, y0, x1, y1, scrGetIntPara(5));
    return 1;
}

u32 evtCommand_TEX_ALP(void)
{
    void *id;
    s32 alp0;
    s32 alp1;

    id = (void *)scrGetIntPara(0);
    alp0 = scrGetIntPara(1);
    alp1 = scrGetIntPara(2);
    evtPictureSetAlpha(id, alp0, alp1, scrGetIntPara(3));
    return 1;
}

u32 evtCommand_TEX_ZOOM(void)
{
    void *id;
    s32 zoom0;
    s32 zoom1;

    id = (void *)scrGetIntPara(0);
    zoom0 = scrGetIntPara(1);
    zoom1 = scrGetIntPara(2);
    evtPictureSetZoom(id, zoom0, zoom1, scrGetIntPara(3));
    return 1;
}

u32 evtCommand_TEX_STAT(void)
{
    s32 stat;
    s32 mode;
    s32 ret;

    stat = evtPictureStat((void *)scrGetIntPara(0));
    mode = scrGetIntPara(1);
    ret = 0;
    if (mode == 0) {
        ret = stat != 0;
    }
    if (mode == 1) {
        if ((stat & 1) != 0) {
            ret = 1;
        }
    }
    if (mode == 2) {
        if ((stat & 2) != 0) {
            ret = 1;
        }
    }
    if (mode == 3) {
        if ((stat & 4) != 0) {
            ret = 1;
        }
    }
    scrSetIntReturnValue(ret);
    return 1;
}

u32 evtCommand_SET_SKY(void)
{
    s32 r;
    s32 g;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    r = scrGetIntPara(0);
    g = scrGetIntPara(1);
    fldSkySetFilter(r, g, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_SET_SKY_A(void)
{
    s32 alpha;

    alpha = scrGetIntPara(1);
    if (alpha < -255) {
        datDebugPrintf(D_0053AFE8);
        alpha = -255;
    }
    if (alpha > 255) {
        datDebugPrintf(D_0053B010);
        alpha = 255;
    }
    evtSetSkyAlpha(scrGetIntPara(0), alpha);
    return 1;
}

u32 evtCommand_BOKASI_ON(void) {
    evtSetBokasi(1);
    return 1;
}

u32 evtCommand_BOKASI_OFF(void) {
    evtSetBokasi(0);
    return 1;
}

u32 evtCommand_LOAD_SOUND_E(void)
{
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else if (evtCheckEventBgmLoaded(scrGetIntPara(0)) != 0) {
        return 1;
    }
    return 0;
}

u32 evtCommand_BGM_PLAY_E(void) {
    u64 eventId;
    u64 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlayEventBgm(eventId, fade);
    return 1;
}

u32 evtCommand_BGM_TRANS_E(void) {
    u64 eventId;
    u64 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtTransEventBgm(eventId, fade);
    return 1;
}

u32 evtCommand_FADEOUT_E(void) {
    u64 eventId;
    u64 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtStopEventBgm(eventId, fade);
    return 1;
}

u32 evtCommand_BGM_VOL_DOWN(void) {
    p2sdBgmVolDown();
    nbBgmStop();
    return 1;
}

u32 evtCommand_BGM_VOL_UP(void) {
    p2sdBgmVolUp();
    nbBgmStop();
    return 1;
}

u32 evtCommand_SE_PLAY_E(void) {
    u64 id;
    u64 fade;

    id = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlaySe(id, fade);
    return 1;
}

u32 evt_1E9168(void) {
    u64 eventId;
    u64 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    func_001FA158(eventId, fade);
    return 1;
}

u32 evt_1E91A8(void) {
    u64 task;
    u64 value;

    task = scrGetIntPara(0);
    value = scrGetIntPara(1);
    datAddKotowariPoint(task, value);
    return 1;
}

u32 evt_1E91E8(void)
{
    u8 *gb;

    gb = DDS3_GBWK;
    scrSetIntReturnValue(((u8 *)scrGetIntPara(0) + (u32)gb)[0x1CEC]);
    return 1;
}

s32 evt_1E9220(void)
{
    scrSetIntReturnValue(datGetKotowariLank(0));
    return 1;
}

s32 evt_1E9248(void)
{
    scrSetIntReturnValue(datGetKotowariLank(1));
    return 1;
}

s32 evt_1E9270(void)
{
    scrSetIntReturnValue(datGetKotowariLank(2));
    return 1;
}

u32 evt_1E9298(void) {
    u64 task;
    u64 value;

    task = scrGetIntPara(0);
    value = scrGetIntPara(1);
    func_001FA310(task, value);
    return 1;
}

u32 evt_1E92D8(void) {
    Eve_DistortionBlurSetDefTex();
    return 1;
}

u32 evt_1E92F8(void)
{
    if (scrGetIntPara(0) <= 0) {
        dds3DrawFlag |= 0x2000000;
        return 1;
    }
    if (scrGetCommandTimer() < scrGetIntPara(0)) {
        dds3DrawFlag |= 0x2000000;
        return 0;
    }
    return 1;
}

void evtDrawDeadMoonSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                           s32 rgb, sdfTexHandle_t *th, s32 ox, s32 oy,
                           f32 cnt, f32 alpha0, f32 alpha1, f32 scale0, f32 scale1)
{
    f32 ratio;
    f32 width;
    f32 height;
    s32 scale;
    s32 alpha;
    f32 drawHeight;
    f32 drawWidth;

    ratio = sdfSin(cnt) * 0.5f + 0.5f;
    width = w;
    height = h;
    alpha = (s32)(alpha0 * ratio + alpha1 * (1.0f - ratio));
    alpha = (alpha << 7) / 100;
    scale = (s32)(scale0 * ratio + scale1 * (1.0f - ratio));
    drawWidth = width * scale / 100.0f;
    drawHeight = height * scale / 100.0f;
    DrwSprite2DF((width - drawWidth) * 0.5f + ox + x,
                 (height - drawHeight) * 0.5f + oy + y,
                 drawWidth, drawHeight, u, v, uw, vh, rgb | (alpha << 24), th);
}

/* Acquire the background texture, then release the temporary file allocation. */
INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AFA0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AFE8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053B010);

void nbLoadDeadMoonTex(void) {
    u32 resource;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx("/fld/f/bin/moon_07.tmx", &resource, 0);

    DeadMoonTmx = sdfTexSet((TMX_HEADER_t *)(resource));
    sdfMemFreeDirect(fileAllocation);
}

void nbFreeDeadMoonTex(void)
{
    if (DeadMoonTmx != NULL) {
        sdfTexFreeDelayed(DeadMoonTmx);
    }
    DeadMoonTmx = NULL;
}

INCLUDE_SDATA(const s32, "event/evtCommand", D_0054F754);

INCLUDE_SDATA(const s32, "event/evtCommand", DeadMoonTmx);

