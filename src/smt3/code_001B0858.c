#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

/* Cubic spline fitted over a point list; dds3AllocSplineHandle takes 0x2C. */
typedef struct dds3Spline_t {
    void *handle;
    void *coefHandle;
    s32 count;
    s16 single;
    s16 pad0E;
    void *points;
    f32 *coefs;
    f32 *coefB;
    f32 *coefC;
    s32 unk20;
    f32 unk24;
    f32 unk28;
} dds3Spline_t;

/* Walks a point list at a fixed step; dds3_1B1238 takes 0x18. */
typedef struct dds3SplineWalk_t {
    void *handle;
    s32 count;
    f32 *points;
    s32 index;
    f32 t;
    f32 step;
} dds3SplineWalk_t;

/* 16-byte rows indexed by camera state; +0xC selects the per-state reset. */
typedef struct nbCameraStateEntry_t {
    u8 unk00[0xC];
    s8 reset;
    u8 unk0D[3];
} nbCameraStateEntry_t;

typedef struct nbCameraAnim_t {
    s32 rotateFrames;
    f32 rotateStep;
    s32 distanceFrames;
    f32 distanceStep;
} nbCameraAnim_t;

typedef struct nbCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 stateChange;
    u32 state;
    u32 stateOld;
    u32 stateOld2;
    u32 phase;
    s32 frame;
    u32 type;
    f32 angle;
    nbCameraAnim_t anim;
} nbCamera_t;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct nbCameraHelper_t {
    u8 unk00[0x30];
    u32 unk30;
    s32 unk34;
    effPartHandle_t *hPart;
} nbCameraHelper_t;

extern u8 dds3EffectBase[];
extern u32 dds3GetRandA(u8 *state);
extern void SetRotateMatrixY(f32 angle);
extern void effBTLModelAllHide(void);
extern void effBTLModelGroupGetCenter(u32 mask);
extern f32 nbCameraGroupGetMaxLength(sdfVector4_t *pCenter, u32 mask);
extern s32 effBTLModelGetGroupNumber(u32 mask);
extern void *sdfChipAllocDirect(s32 size);
extern effPartHandle_t *nbCameraHelperFileLoadPB(const char *name);
extern void func_001B2158(u32 mask);
extern f32 func_001B2270(sdfVector4_t *pCenter, u32 mask);
extern void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pCamera, void *pWork);
extern void nbCameraState_Start_Set(nbCamera_t *pCamera);
extern void nbCameraState_Start_BA_Set(nbCamera_t *pCamera);
extern void nbCameraShot_CommandFixPos_Set2(nbCamera_t *pCamera);
typedef struct nbCameraUnitWork_t {
    u8 unk00[0x28];
    nbFormation_t *pForm;
} nbCameraUnitWork_t;

/* The per-shot parameters the nbCameraShot_*_Set2 family reads; 0x30 on the
 * stack at every call site. */
typedef struct nbCameraSkillInfo_t {
    u32 shot1;
    u32 shot23;
    f32 angleH;
    f32 angleW;
    f32 heightMin;
    f32 widthMin;
    u32 ptr1;
    u32 ptr23;
    u32 anim;
    u8 pad24[0xC];
} nbCameraSkillInfo_t;

extern void *dds3GetProcessData(dds3ProcessID_t *process);
extern void effBTLModelInvokeShow(void);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, u32 mark);
extern void *dds3EffectFileListCreate(const char *path, s32 max);
extern void *dds3EffectFileListWindowCreate(void *hFileList);
extern void dds3EffectFileListWindowSetPos(void *pWork, s32 x, s32 y);
extern void dds3EffectFileListWindowSetOt(void *pWork, s32 ot);
extern void dds3EffectFileListWindowSetPadWait(void *pWork, s32 wait);
extern void dds3EffectFileListWindowSetWindowColor(void *pWork, u32 fill, u32 edge);
extern const char D_005358A8[];
extern s32 nbCameraStartState;
extern u32 D_0054F37C;
extern char D_00534608[];
extern char D_00534630[];
extern char D_00535850[];
extern char D_00535878[];
extern s32 printf(const char *format, ...);
extern f32 effBTLModelGroupGetSphere(u32 mask);
extern void effBTLModelGroupGetDirection(u32 mask, f32 *pDirection);
extern f32 sdfAtan(f32 x);
extern void SetRotateMatrixX(f32 angle);
extern u32 effBTLSkillGetInvokeParty(void);
extern s32 nbCameraBossCutFlag;
extern s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pCamera);
extern void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);
extern void nbCameraShot_InvokeParty_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);
extern u128 D_003259C0;
extern u128 D_003259D0;
extern u128 D_003259E0;
extern u128 D_003259F0;
extern u128 Bsnet_E907_2_EndTarget;
extern u128 Bsnet_E907_2_EndDirection;
extern f32 Bsnet_E907_2_StartDistance;
extern f32 Bsnet_E907_2_EndDistance;
extern f32 Bsnet_E907_2_Rate;
extern u128 D_0056B420;
extern u128 D_0056B430;
extern u128 D_0056B330;
extern u128 nbCameraEnemyBadFlag;
extern f32 nbCameraEnemyBadStep;
extern f32 nbCameraEnemyBadTarget;
extern u128 nbCameraCut1EnemyVec;
extern f32 nbCamera_Cut1Enemy_Distance;
extern u32 nbCamera_Cut1Enemy_Frame;
extern f32 nbCamera_Cut1Enemy_Rate;
extern void nbCameraState_SkillAttack_Init(nbCamera_t *pCamera);
extern void nbCameraShot_Invoke_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);
extern void nbCameraShot_TargetParty_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);
extern void nbCameraShot_PullOne_Init(nbCamera_t *pCamera);
extern s32 nbCameraHero_Cut2_Run(nbCamera_t *pCamera, void *pWork);
extern void nbCameraInit(void);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);
extern void ToMatrix(void);
extern sdfVector4_t nbCameraBSARICut1Vec;
extern f32 BSARI_Cut1_Rate;
extern u32 nbCameraHero_Cut_Frame;
extern char *nbCameraHelperViewerMode;
extern nbCameraHelper_t *D_0054F3DC;
extern sdfMemHandle_t *D_0054F3E0;
extern sdfMemHandle_t *D_0054F3E4;
extern void *nbCameraHelperViewerState;
extern void *nbCameraHelperViewerHandle;
extern u128 D_00320E30;
extern u128 D_00320E50;
extern char D_005345C8[];
extern char D_005345D8[];
extern s32 nbCameraProcess_nbCameraProcess(dds3ProcessID_t *id);
extern void nbCameraShutdown(dds3ProcessID_t *id);
extern s32 nbCameraDraw(dds3ProcessID_t *id);
extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);
extern s32 effBTLModelGetMarkPosition(nbFormation_t *pFormation, s32 mark);
extern s32 effBTLModelGetMarkPositionEvalTm(nbFormation_t *pFormation, s32 mark);
extern nbFormation_t *effBTLSkillGetTargetFormation(s32 index);
extern s32 effBTLModelGetID(nbFormation_t *pFormation);
extern void nbCameraShot_GroundCollision(nbCamera_t *pCamera);
extern datUnitVisual_t datModelParamTable[];
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern s32 effBTLDebug_CameraAllTableCheck(void);
extern u128 D_00324740;
extern u8 D_0054F370;
extern u32 D_0054F374;
extern u8 D_0054F378;
extern u32 D_0054F37C;

extern nbCamera_t pNbCamera;
extern nbCameraStateEntry_t nbCameraStateTable[];
extern s8 nbCameraPressRunFlag;
extern u32 nbCameraPressThrough;
extern u32 nbCameraPressFrame;
extern u8 nbCameraCRSetPlayerFormRequestFlag;
extern u32 nbCamera_NegoBougai_Wait;
extern u32 nbCameraBoss_BSBERU_E915_Frame;
extern f32 nbCameraBoss_BSBERU_E915_Rate;
extern u32 Foru_E913_Frame;
extern s32 D_00551E08;

extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void effBTLSkillSetFormationInfo(u32 invokeId, u32 targetBits);
extern void effBTLModelAllPopPosition(void);
extern void effBTLModelAllPopRotate(void);
extern void effBTLModelAllShow(void);
extern void nbCameraHelperModelSetSub(mdlObj_t *hModel, u32 target, u32 eye);

extern f32 nbCamera_NegoHojo_Rate;
extern f32 nbCamera_NegoHojo_Distance;

extern u32 nbCameraBoss_BSARI_MASEHA_Frame;
extern s32 nbCameraBoss_BSARI_MASEHA_Type;
extern void *nbCameraHelperViewerPBWork;
extern void *nbCameraHelperViewerPBHandle;

extern void nbResetAllFormPosRot(void);
extern void func_0016E1F8(void);
extern void func_0016E120(void);
extern void nbCameraShot_All_Set(nbCamera_t *pCamera);
extern void nbCameraShot_All_Set2(nbCamera_t *pCamera, void *pWork);
extern mdlObj_t *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);
extern void dds3EffectPartDelete(effPartHandle_t *hEffPart);
extern void sdfChipFreeDirect(void *pChip);
extern void dds3EffectFileListWindowDelete(void *pWork);
extern void dds3EffectFileListDelete(void *hFileList);
extern s32 dds3GetRandIntA(s32 low, s32 high);

typedef struct dds3Scene_t dds3Scene_t;

/* 12-byte rows; nbCameraGetModelAllTable indexes them by unit work slot. */
typedef struct nbCamera_AllTable_t {
    f32 unk00;
    f32 unk04;
    f32 radius;
} nbCamera_AllTable_t;

extern u128 nbCameraAnalyzeVec;
extern u128 nbCameraNegoBougaiVec;
extern f32 nbCamera_Analyze_Distance;
extern f32 nbCamera_Analyze_Rate;
extern f32 nbCamera_Analyze_Rate_Add;
extern u32 nbCamera_Analyze_Frame;
extern u32 nbCamera_Analyze_Wait;
extern f32 nbCamera_NegoBougai_Rate;
extern f32 nbCamera_NegoBougai_Distance;
extern u32 nbCamera_NegoBougai_Frame;

extern nbCamera_AllTable_t nbCameraGetModelAllTable_Dummy[];
extern nbCamera_AllTable_t D_00325950;
extern nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation);
extern void nbCameraState_Start_BA_Animation(nbCamera_t *pCamera);
extern void nbCameraState_Start_Animation(nbCamera_t *pCamera);

extern f32 nbCameraBoss_BSALBI_E912_Rate;
extern f32 nbCameraBoss_BSALBI_E912_Distance;
extern s32 nbCameraBoss_BSARI_SIMANO_Type;
extern f32 nbCameraBoss_BSARI_SIMANO_Rate;
extern u8 nbCameraBoss_Pish_MUHENK_Type;
extern u32 nbCameraBoss_Pish_MUHENK_Frame;

extern void nbCameraShot_InvokeTargetReverse_Set2(nbCamera_t *pCamera, void *pWork);
extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern dds3Scene_t *dds3GetActiveScene(void);
extern s32 dds3GetActiveCameraSceneBasic(dds3Scene_t *pScene);

void nbCameraSetState(u16 state);
void nbCameraPressStart(void);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
void dds3AllocSplinePoints(dds3Spline_t *spline, s32 flattenEqualComponents);
void dds3BuildSplinePoints(f32 *tangents, f32 *points, u32 count);
void dds3BuildSplineCoefs(dds3Spline_t *spline, f32 *tangents);
void dds3BuildSplineCoefsSkipDup(dds3Spline_t *spline, f32 *tangents);
void func_001B1370(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 index, f32 t);
void func_001B0A60(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t);


dds3Spline_t *dds3AllocSplineHandle(f32 *points, u32 count, s32 flattenEqualComponents) {
    sdfMemHandle_t *primaryResource = sdfMemAllocDirect(0x2C);
    f32 step = 0.05f;
    dds3Spline_t *primitive = (dds3Spline_t *)sdfMemLock(primaryResource);
    sdfMemHandle_t *coefficientResource;
    f32 (*coefficients)[3];

    primitive->handle = primaryResource;
    primitive->count = count;
    primitive->points = points;
    primitive->unk20 = 0;
    primitive->unk24 = 0;
    primitive->unk28 = step;
    if (count < 3) {
        primitive->coefs = NULL;
        primitive->single = 1;
    } else {
        coefficientResource = sdfMemAllocDirect(count * 36);
        coefficients = (f32 (*)[3])sdfMemLock(coefficientResource);
        primitive->handle = primaryResource;
        primitive->coefHandle = coefficientResource;
        primitive->single = 0;
        primitive->count = count;
        primitive->points = points;
        primitive->coefs = (f32 *)coefficients;
        primitive->coefB = (f32 *)(coefficients + count);
        primitive->coefC = (f32 *)(coefficients + count * 2);
        primitive->unk20 = 0;
        primitive->unk24 = 0;
        primitive->unk28 = step;
        dds3AllocSplinePoints(primitive, flattenEqualComponents);
    }
    return primitive;
}

void dds3FreeSplineHandle(dds3Spline_t *pSpline)
{
    if (pSpline == NULL) {
        return;
    }
    if (pSpline->coefs != NULL) {
        sdfMemFreeDirect(pSpline->coefHandle);
    }
    sdfMemFreeDirect(pSpline->handle);
}

s32 dds3_1B09C0(sdfVector4_t *pVertex, dds3Spline_t *pSpline)
{
    f32 t = pSpline->unk24;
    s32 index = pSpline->unk20;
    s32 running = 1;

    func_001B0A60(pVertex, pSpline, index, t);
    t += pSpline->unk28;
    if (t > 1.0f) {
        t -= 1.0f;
        index++;
    }
    if ((u32)index >= (u32)(pSpline->count - 1)) {
        t = 0.0f;
        index = 0;
        running = 0;
    }
    pSpline->unk20 = index;
    pSpline->unk24 = t;
    return running;
}

void func_001B0A60(sdfVector4_t *vertex, dds3Spline_t *primitive, s32 recordIndex, f32 t) {
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if ((u16)primitive->single == 0) {
        a = primitive->coefs + recordIndex * 3;
        b = primitive->coefB + recordIndex * 3;
        c = primitive->coefC + recordIndex * 3;
        d = (f32 *)primitive->points + recordIndex * 3;
        vertex->x = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex->y = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex->z = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex->w = 1.0f;
    } else {
        a = (f32 *)primitive->points + recordIndex * 3;
        b = a + 3;
        vertex->x = a[0] + (b[0] - a[0]) * t;
        vertex->y = a[1] + (b[1] - a[1]) * t;
        vertex->z = a[2] + (b[2] - a[2]) * t;
    }
}

/* vu0 routine: sample the curve into vf10. */
void func_001B0B90(dds3Spline_t *primitive, s32 recordIndex, f32 t)
{
    f32 vertex[4];
    f32 *a;
    f32 *b;
    f32 *c;
    f32 *d;

    if ((u16)primitive->single == 0) {
        a = primitive->coefs + recordIndex * 3;
        b = primitive->coefB + recordIndex * 3;
        c = primitive->coefC + recordIndex * 3;
        d = (f32 *)primitive->points + recordIndex * 3;
        vertex[0] = ((a[0] * t + b[0]) * t + c[0]) * t + d[0];
        vertex[1] = ((a[1] * t + b[1]) * t + c[1]) * t + d[1];
        vertex[2] = ((a[2] * t + b[2]) * t + c[2]) * t + d[2];
        vertex[3] = 1.0f;
    } else {
        a = (f32 *)primitive->points + recordIndex * 3;
        b = a + 3;
        vertex[0] = a[0] + (b[0] - a[0]) * t;
        vertex[1] = a[1] + (b[1] - a[1]) * t;
        vertex[2] = a[2] + (b[2] - a[2]) * t;
    }
    VU0_LOAD_VF10_FROM(*(u128 *)vertex);
}


void func_001B0CC0(dds3Spline_t *pSpline)
{
    pSpline->unk20 = 0;
    pSpline->unk24 = 0;
}


void func_001B0CD0(dds3Spline_t *pSpline, f32 value)
{
    pSpline->unk28 = value;
}

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(primitive->count * 12);
    f32 *tangentData = (f32 *)sdfMemLock(handle);

    dds3BuildSplinePoints(tangentData, primitive->points, primitive->count);
    if (flattenEqualComponents == 0) {
        dds3BuildSplineCoefs(primitive, tangentData);
    } else {
        dds3BuildSplineCoefsSkipDup(primitive, tangentData);
    }
    sdfMemFreeDirect(handle);
}

INCLUDE_ASM(const s32, "code_001B0858", dds3BuildSplinePoints);

void SolveTridiagonal(f32 *solution, f32 *rhs, s32 count) {
    s32 byteCount = count * sizeof(f32);
    void *handle = sdfMemAllocDirect(byteCount);
    f32 *upper;
    s32 i;
    f32 denominator;
    f32 *upperEnd;
    f32 *solutionEnd;
    f32 *rhsEnd;

    count -= 2;
    upper = (f32 *)sdfMemLock(handle);
    denominator = 2.0f;

    solution[0] = rhs[0] / denominator;
    for (i = 1; i <= count; i++) {
        upper[i] = 1.0f / denominator;
        denominator = 4.0f - upper[i];
        solution[i] = (rhs[i] - solution[i - 1]) / denominator;
    }
    upperEnd = (f32 *)((u8 *)upper + byteCount);
    solutionEnd = (f32 *)((u8 *)solution + byteCount);
    rhsEnd = (f32 *)((u8 *)rhs + byteCount);
    upperEnd[-1] = 1.0f / denominator;
    denominator = 2.0f - upperEnd[-1];
    solutionEnd[-1] = (rhsEnd[-1] - solutionEnd[-2]) / denominator;
    for (i = count; i >= 0; i--) {
        solution[i] -= upper[i + 1] * solution[i + 1];
    }
    sdfMemFreeDirect(handle);
}

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    for (segment = 0; segment < count - 1; segment++) {
        s32 component;
        for (component = 0; component < 3; component++) {
            f32 endValue = points[segment * 3 + component + 3];
            f32 startValue = points[segment * 3 + component];
            f32 startTangent = tangents[component * count + segment];
            f32 endTangent = tangents[component * count + segment + 1];
            cubic[segment * 3 + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * 3 + component] = startTangent;
        }
    }
}

void dds3BuildSplineCoefsSkipDup(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    s32 component;

    for (segment = 0; segment < count - 1; segment++) {
        for (component = 0; component < 3; component++) {
            f32 startValue = points[segment * 3 + component];
            f32 endValue = points[segment * 3 + component + 3];

            if (startValue == endValue) {
                cubic[segment * 3 + component] = 0.0f;
                quadratic[segment * 3 + component] = 0.0f;
                linear[segment * 3 + component] = 0.0f;
            } else {
                f32 startTangent = tangents[component * count + segment];
                f32 endTangent = tangents[component * count + segment + 1];

                cubic[segment * 3 + component] = 2.0f * (startValue - endValue) + (startTangent + endTangent);
                quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f - (2.0f * startTangent + endTangent);
                linear[segment * 3 + component] = startTangent;
            }
        }
    }
}

void *dds3_1B1238(void *rows, u32 count) {
    void *channel = NULL;
    sdfMemHandle_t *handle;
    dds3SplineWalk_t *channelObject;

    if (count < 4) {
        return channel;
    }
    handle = sdfMemAllocDirect(0x18);
    channel = (void *)sdfMemLock(handle);
    channelObject = channel;
    channelObject->handle = handle;
    channelObject->step = 0.05f;
    channelObject->count = count;
    channelObject->points = rows;
    channelObject->t = 0;
    channelObject->index = 0;
    return channel;
}


s32 dds3_1B12B0(dds3SplineWalk_t *pWalk)
{
    if (pWalk != NULL) {
        sdfMemFreeDirect(pWalk->handle);
    }
}

s32 dds3_1B12D0(void *vertex, dds3SplineWalk_t *channel) {
    s32 continuing = 1;
    f32 position = channel->t;
    u32 recordIndex = channel->index;

    func_001B1370(vertex, channel, recordIndex, position);
    position += channel->step;
    if (position > 1.0f) {
        position -= 1.0f;
        recordIndex += 3;
    }
    if (recordIndex >= channel->count - 1) {
        position = 0.0f;
        recordIndex = 0;
        continuing = 0;
    }
    channel->index = recordIndex;
    channel->t = position;
    return continuing;
}

void func_001B1370(sdfVector4_t *vertex, dds3SplineWalk_t *channel, s32 recordIndex, f32 t) {
    f32 weights[4];
    f32 oneMinusT = 1.0f - t;
    f32 *p0 = channel->points + recordIndex * 3;
    f32 *p1 = p0 + 3;
    f32 *p2 = p0 + 6;
    f32 *p3 = p0 + 9;

    weights[0] = oneMinusT * oneMinusT * oneMinusT;
    weights[1] = t * (oneMinusT * oneMinusT) * 3.0f;
    weights[2] = t * t * oneMinusT * 3.0f;
    weights[3] = t * t * t;
    vertex->x = p0[0] * weights[0] + p1[0] * weights[1] + p2[0] * weights[2] + p3[0] * weights[3];
    vertex->y = p0[1] * weights[0] + p1[1] * weights[1] + p2[1] * weights[2] + p3[1] * weights[3];
    vertex->z = p0[2] * weights[0] + p1[2] * weights[1] + p2[2] * weights[2] + p3[2] * weights[3];
    vertex->w = 1.0f;
}


void func_001B1478(dds3SplineWalk_t *pWalk)
{
    pWalk->index = 0;
    pWalk->t = 0.0f;
}


void func_001B1488(dds3SplineWalk_t *pWalk, f32 step)
{
    pWalk->step = step;
}


void nbCameraClearState(nbCamera_t *pCamera)
{
    pCamera->anim.rotateFrames = 0;
    pCamera->anim.rotateStep = 0;
    pCamera->anim.distanceFrames = 0;
    pCamera->anim.distanceStep = 0.0f;
}

void nbCameraAnim_RotateSet(nbCamera_t *pCamera, s32 frames, s32 total, f32 frame)
{
    pCamera->anim.rotateFrames = frames;
    pCamera->anim.rotateStep = frame / frames;
    SetRotateMatrixY(pCamera->anim.rotateStep * total);
    VU0_LOAD_VF10(&pCamera->direction);
    VU0_APPLY_MATRIX_VF10();
    VU0_STORE_VF10(&pCamera->direction);
}

void func_001B1518(nbCamera_t *pCamera, s32 frames, s32 count, f32 angle)
{
    f32 step = -angle / (f32)frames;

    pCamera->anim.distanceFrames = frames;
    pCamera->anim.distanceStep = step;
    pCamera->distance += step * (f32)count;
}

void nbCameraState_InvokeTargetSelect_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 4) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_InvokeTarget_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 8) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_EnemyParty_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 0x200) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_DebugShotAll_Init(void)
{
    nbCameraShot_All_Set(&pNbCamera);
}


void nbCameraSetState(u16 state)
{
    pNbCamera.stateChange = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.state = 0;
    }
}

void func_001B1770(u16 state)
{
    pNbCamera.stateChange = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.state = 0;
    }
}

void nbCameraSetBougai(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(13);
}


void nbCameraSetTaizi(nbFormation_t *pInvokeForm, nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(pInvokeForm->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(14);
}

void nbCameraSetTarget(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(15);
}

void nbCameraSetHojo(nbFormation_t *pHojo, nbFormation_t *pTarget1, nbFormation_t *pTarget2)
{
    effBTLSkillSetFormationInfo(pHojo->formindex, (1 << pTarget1->formindex) | (1 << pTarget2->formindex));
    nbCameraSetState(16);
}

void nbCameraEventSetTarget(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(24);
}

void nbCameraScriptSet1Shot(nbFormation_t *pFormation)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex,
                                1 << pFormation->formindex);
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllHide();
    func_0016E1F8();
    nbCameraSetState(0x1A);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraScriptSetBougai);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraScriptSet);

void nbCameraInit(void)
{
    nbCameraShot_All_Set(&pNbCamera);
    pNbCamera.stateChange = 0;
    pNbCamera.stateOld2 = 0;
    pNbCamera.stateOld = 0;
    pNbCamera.state = 0;
}

s32 nbCameraCheckNiidaCamera(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();

    return dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) == pData->unkB4;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraCheckStateChange);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraProcess_nbCameraProcess);


s32 nbCameraDraw(dds3ProcessID_t *id)
{
    return 0;
}


void nbCameraShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->camera = NULL;
}

void nbInitCameraProcess(nbMainProcessData_t *pData)
{
    dds3ProcessID_t *process = dds3StartProcess(D_005345C8, 0x3FB, 2, 1,
                                                (dds3ProcessFunc_t)nbCameraProcess_nbCameraProcess,
                                                (dds3ProcessFunc_t)nbCameraShutdown, NULL);

    dds3LinkProcess(pData->unkBC, process);
    pData->camera = process;
    dds3LinkProcess(process, dds3StartProcess(D_005345D8, 0x2B0E, 1, 1,
                                              (dds3ProcessFunc_t)nbCameraDraw, NULL, NULL));
    nbCameraInit();
}

INCLUDE_ASM(const s32, "code_001B0858", func_001B2158);

f32 func_001B2270(sdfVector4_t *pCenter, u32 mask)
{
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    f32 length;
    f32 max = 0.0f;
    s32 i;

    for (i = 0x1F; i >= 0; i--) {
        if ((pForm->stat & mask) != 0 && (pForm->stat & 1) != 0) {
            effBTLModelGetSphereCenter(pForm);
            VU0_LOAD_VF11(pCenter);
            VU0_SUB_VF10_VF11();
            VU0_LENGTH_VF10(length);
            if (length + pForm->r * pForm->scl > max) {
                max = length + pForm->r * pForm->scl;
            }
        }
        pForm++;
    }
    return max;
}

void nb_1B2340(u32 mask)
{
    sdfVector4_t center;

    func_001B2158(mask);
    VU0_STORE_VF10_TO(center);
    func_001B2270(&center, mask);
    VU0_LOAD_VF10_FROM(center);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_Animation);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_BA_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_BA_Animation);

void nbCameraState_Start_Init(void)
{
    if ((nbGetMainProcessData()->stat & 0x10) != 0) {
        nbCameraState_Start_BA_Set(&pNbCamera);
    } else {
        nbCameraState_Start_Set(&pNbCamera);
    }
    D_0054F370 = 0;
    D_0054F374 = 0;
    D_0054F378 = 0;
    D_0054F37C = 0;
}

void nbCameraState_Start_Calc(void)
{
    if ((nbGetMainProcessData()->stat & 0x10) != 0) {
        nbCameraState_Start_BA_Animation(&pNbCamera);
        return;
    }
    nbCameraState_Start_Animation(&pNbCamera);
}


s32 nb_1B33F0(void)
{
    return D_00551E08;
}

s32 nbCameraStart(void)
{
    if (D_0054F370 != 0) {
        D_0054F370 = 0;
        printf(D_00534608, D_0054F374);
    }
    if (D_0054F378 != 0) {
        if (nbCameraStartState == 5) {
            D_0054F378 = 0;
            printf(D_00534630, D_0054F37C);
        }
    }
    return nbCameraStartState == 5;
}

void func_001B3460(nbCamera_t *pCamera) {
}

void func_001B3468(nbCamera_t *pCamera) {
}


void nb_1B3470(void)
{
    func_001B3460(&pNbCamera);
}


void nb_1B3490(void)
{
    func_001B3468(&pNbCamera);
}

INCLUDE_ASM(const s32, "code_001B0858", func_001B34B0);

INCLUDE_RODATA(const s32, "code_001B0858", D_00534608);

INCLUDE_RODATA(const s32, "code_001B0858", D_00534630);

INCLUDE_ASM(const s32, "code_001B0858", nbCamera_SkillInfo_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_GroundCollision);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_TargetParty_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetParty_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_Invoke_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_Target_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_One_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_One_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_PullOne_Init);

void nbCameraShot_PullOne_Calc(nbCamera_t *pCamera, f32 rate)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();

    effBTLModelGetSphereCenter(pForm);
    VU0_STORE_VF10(pCamera);
    pCamera->target.y -= pForm->h * pForm->scl * 0.5f * rate;
    nbCameraShot_GroundCollision(pCamera);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTarget_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetReverse_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetNear_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetNearReverse_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetPartyTop_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeTargetPartyTop2_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_InvokeParty_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_All_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_All_Set3);

void nbCameraState_Analyze_Set(nbCamera_t *pCamera)
{
    PS2_COPY_QUAD(&nbCameraAnalyzeVec, pCamera);
    nbCamera_Analyze_Distance = pCamera->distance;
    nbCamera_Analyze_Rate = 1.0f;
    nbCamera_Analyze_Rate_Add = -0.01f;
    nbCamera_Analyze_Frame = 1;
    nbCamera_Analyze_Wait = 5;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Analyze_Calc);

void nbCameraShot_All_Set(nbCamera_t *pCamera)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
    pCamera->target.x = -195.3f;
    pCamera->target.y = -321.2f;
    pCamera->target.z = -884.7f;
    pCamera->direction.x = 0.2207f;
    pCamera->direction.y = 0.2273f;
    pCamera->direction.z = 0.9485f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
}

INCLUDE_ASM(const s32, "code_001B0858", bCameraShot_AttackInvokeTargetPartyNear_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_AttackTuibi_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_AttackTuibiReverse_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillAttack_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillAttack_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillStart_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Cut1Enemy_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Cut1Enemy_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillCut1_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillCut1_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillCut2_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_SkillCut2_Calc);

void func_001B8018(void) {
}

void func_001B8020(void) {
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_PlayerSide_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_EnemySide_Init);

void nbCameraEnemyPartySet(f32 pitch, f32 yaw)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    u32 i = 0;
    u32 invoke = 0;
    u32 bits = 0;

    for (; i < 32; i++) {
        if ((pForm->stat & 0x203) == 0x203) {
            invoke = i;
            bits |= 1 << invoke;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(invoke, bits);
    work.angleH = pitch;
    work.angleW = yaw;
    work.heightMin = 0.0f;
    work.widthMin = 700.0f;
    nbCameraShot_TargetParty_Set2(&pNbCamera, &work);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_EnemyBad_Init);

void nbCameraState_EnemyBad_Calc(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t focus;
    sdfVector4_t scale;
    f32 distance = -pCamera->distance;

    nbCameraEnemyBadStep *= 0.925f;
    scale.x = distance;
    scale.y = distance;
    scale.z = distance;
    VU0_LOAD_VF10(&pCamera->direction);
    VU0_LOAD_VF11(&scale);
    VU0_MUL_VF10_VF11();
    VU0_LOAD_VF11(pCamera);
    VU0_ADD_VF10_VF11();
    VU0_STORE_VF10(&focus);
    VU0_LOAD_VF10(&D_0056B330);
    VU0_LOAD_VF11(&nbCameraEnemyBadFlag);
    VU0_LERP_VF10_VF11(nbCameraEnemyBadStep);
    VU0_STORE_VF10(pCamera);
    VU0_LOAD_VF11(&focus);
    VU0_SUB_VF10_VF11();
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(&pCamera->direction);
    pCamera->distance = nbCameraEnemyBadTarget * (nbCameraEnemyBadStep * 0.3f + 0.7f);
}

void nbCameraState_NegoBougai_Init(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    PS2_COPY_QUAD(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->distance;
    nbCamera_NegoBougai_Wait = 15;
    nbCamera_NegoBougai_Frame = 0;
}

void nbCameraState_NegoBougai_Init2(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    PS2_COPY_QUAD(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->distance;
    nbCamera_NegoBougai_Wait = 8;
    nbCamera_NegoBougai_Frame = 0;
}


void nbCameraState_NegoBougai_SetWaitFrame(u32 frame)
{
    nbCamera_NegoBougai_Wait = frame;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoBougai_Calc);

void nbCameraState_NegoTaizi_Init(void)
{
    func_0016E1F8();
    func_0016E120();
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoTaizi_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoTarget_Init);

void nbCameraState_NegoTarget_Calc(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pForm = effBTLSkillGetTargetFormation(0);

    if ((pForm->stat & 3) == 3 && effBTLModelGetID(pForm) == 0x3E) {
        effBTLModelGetSphereCenter(pForm);
        VU0_LOAD_VF11(&D_00324740);
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(pCamera);
    }
}

void nbCameraState_NegoTarget_Calc2(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pForm = effBTLSkillGetTargetFormation(0);

    if ((pForm->stat & 3) == 3) {
        if (effBTLModelGetMarkPosition(pForm, 1) == 0) {
            effBTLModelGetSphereCenter(pForm);
        }
        VU0_STORE_VF10(pCamera);
    }
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoHojo_Init);

void nbCameraState_NegoHojo_Calc(void)
{
    nbCamera_NegoHojo_Rate = nbCamera_NegoHojo_Rate * 0.925f;
    pNbCamera.distance = nbCamera_NegoHojo_Distance * (1.0f - nbCamera_NegoHojo_Rate);
}

nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation)
{
    datUnitWork_t *pWork;

    if (pFormation == NULL) {
        return &D_00325950;
    }
    if ((pFormation->stat & 1) == 0) {
        return &D_00325950;
    }
    pWork = nbGetUnitWorkFromFormindex(pFormation->formindex);
    if (effBTLDebug_CameraAllTableCheck() == 0) {
        return &D_00325950;
    }
    if ((pWork->flag & 4) != 0) {
        return nbCameraGetModelAllTable_Dummy;
    }
    return &nbCameraGetModelAllTable_Dummy[pWork->id];
}

f32 nbCameraGetModelRadius(nbFormation_t *pFormation)
{
    f32 radius = nbCameraGetModelAllTable(pFormation)->radius;

    if (radius == 0.0f) {
        return pFormation->r * pFormation->scl;
    }
    return radius;
}

f32 nbCameraGroupGetMaxLength(sdfVector4_t *pCenter, u32 mask)
{
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    f32 max = 0.0f;
    f32 length;
    s32 i;

    for (i = 0x1F; i >= 0; i--) {
        if ((pForm->stat & mask) != 0 && (pForm->stat & 3) == 3) {
            effBTLModelGetSphereCenter(pForm);
            VU0_LOAD_VF11(pCenter);
            VU0_SUB_VF10_VF11();
            VU0_LENGTH_VF10(length);
            length += nbCameraGetModelRadius(pForm);
            if (length > max) {
                max = length;
            }
        }
        pForm++;
    }
    return max;
}

f32 nbCameraGroupGetSphere(nbFormation_t *pFormation)
{
    sdfVector4_t center;

    f32 length;

    effBTLModelGroupGetCenter(pFormation);
    VU0_STORE_VF10_TO(center);
    length = nbCameraGroupGetMaxLength(&center, pFormation);
    VU0_LOAD_VF10_FROM(center);
    return length;
}


void nbCameraCommandSetFixPosition(void)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_CommandFixPos_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_CommandFixPos_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_AllParty_Set);

void nbCameraShot_FriendParty_Set(nbCamera_t *pCamera)
{
    u32 party;
    f32 radius;

    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
    party = effBTLSkillGetInvokeParty();
    radius = effBTLModelGroupGetSphere(party);
    VU0_STORE_VF10(pCamera);
    pCamera->target.x = 0.0f;
    if (radius < 250.0f) {
        radius = 250.0f;
    }
    pCamera->fovy = 0.69813163f;
    pCamera->distance = radius / sdfAtan(0.49567342f);
    effBTLModelGroupGetDirection(party, &pCamera->direction);
    SetRotateMatrixX(-0.26179934f);
    VU0_LOAD_VF10(&pCamera->direction);
    VU0_APPLY_MATRIX_VF10();
    VU0_STORE_VF10(&pCamera->direction);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_Summon_Set);


void nbCameraCRSetPlayerFormRequest(void)
{
    nbCameraCRSetPlayerFormRequestFlag = TRUE;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraCommand_GroundCollision);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_CommandRoot_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_CommandRoot_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_CommandTargetSel_Init);

void nbCameraState_CommandTargetSel_Calc(void) {
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Command_KORONZCheck);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraPressProcessRun);


INCLUDE_ASM(const s32, "code_001B0858", nbCameraPressProcessInit);


void nbCameraPressStart(void)
{
    nbCameraPressFrame = 0;
    nbCameraPressRunFlag = TRUE;
}


s32 nbCameraPressProcessCheckRun(void)
{
    return nbCameraPressRunFlag;
}


s32 nbCameraPressProcessGetFrame(void)
{
    return nbCameraPressFrame;
}


void nbCameraPressProcessThrough(void)
{
    nbCameraPressThrough = TRUE;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_InvokeTarget_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_TargetInvoke_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Foru_SIMON_Set);


void nbCameraBoss_Foru_E913_Set(void)
{
    Foru_E913_Frame = 0;
    pNbCamera.frame = 0;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Foru_E913_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_MURET2_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_MURET2_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_DOUTYO_TP_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_DOUTYO_TPFix_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_DOUTYO_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_DOUTYO_Run);

s32 nbCameraBoss_Sp_JAREIH_Cut1_Set(nbCamera_t *pCamera)
{
    nbResetAllFormPosRot();
    return 0;
}


s32 nbCameraBoss_Sp_JAREIH_Set(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sp_JAREIH_Run);

s32 nbCameraBoss_Sp_PSYSTL_Set(nbCamera_t *pCamera)
{
    u8 work[0x30];

    effBTLModelAllShow();
    nbCameraShot_InvokeTargetReverse_Set2(pCamera, work);
    return 1;
}


s32 nbCameraBoss_Sp_PSYSTL_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSMIZ_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSONGY_Command_Set);

s32 nbCameraBoss_Ongy_BUNSIN_Set(nbCamera_t *pCamera)
{
    nbCameraSetState(0);
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Ongy_BUNSIN_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARUS_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSSAMA_Command_Set);

s32 nbCameraBoss_Samael_AKUI_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    nbResetAllFormPosRot();
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        pCamera->target.x = 799.62f;
        pCamera->target.y = -165.55f;
        pCamera->target.z = 1412.67f;
        pCamera->direction.x = -0.43f;
        pCamera->direction.y = -0.08f;
        pCamera->direction.z = -0.9f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->target.x = -818.13f;
        pCamera->target.y = -95.8f;
        pCamera->target.z = 1316.26f;
        pCamera->direction.x = 0.47f;
        pCamera->direction.y = -0.12f;
        pCamera->direction.z = -0.87f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}


s32 nbCameraBoss_Samael_AKUI_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sukady_DAIJIS_Set);

s32 nbCameraBoss_Baal_BAELNO_Set(nbCamera_t *pCamera)
{
    u8 work[0x30];

    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        nbCameraShot_InvokeTarget_Set2(pCamera, work);
    } else {
        nbCameraShot_InvokeTargetReverse_Set2(pCamera, work);
    }
    return 1;
}


s32 nbCameraBoss_Baal_BAELNO_Run(nbCamera_t *pCamera)
{
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Set(nbCamera_t *pCamera)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm;

    nbCameraState_SkillAttack_Init(pCamera);
    pForm = effBTLSkillGetInvokeFormation();
    nbCameraState_InvokeTarget_SearchSet();
    work.angleH = 0.26179934f;
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        work.angleW = 0.5235987f;
    } else {
        work.angleW = -0.5235987f;
    }
    work.heightMin = 0.0f;
    work.widthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pCamera, &work);
    pCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Run(nbCamera_t *pCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pCamera->frame < datModelParamTable[id].part[2].unk18) {
        pCamera->distance += 12.5f;
    }
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Set(nbCamera_t *pCamera)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm;

    nbCameraState_SkillAttack_Init(pCamera);
    pForm = effBTLSkillGetInvokeFormation();
    nbCameraState_InvokeTarget_SearchSet();
    work.angleH = 0.26179934f;
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        work.angleW = 0.5235987f;
    } else {
        work.angleW = -0.5235987f;
    }
    work.heightMin = 0.0f;
    work.widthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pCamera, &work);
    pCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Run(nbCamera_t *pCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pCamera->frame < datModelParamTable[id].part[3].unk18) {
        pCamera->distance -= 15.0f;
    }
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Attack_Set);


s32 nbCameraBoss_Bsnet_Attack_Run(nbCamera_t *pCamera)
{
    return 1;
}

void nbCameraBoss_BSALBI_E912_Set(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 radius;

    nbCameraSetState(0);
    radius = effBTLModelGroupGetSphere(0x200);
    VU0_STORE_VF10(pCamera);
    effBTLModelGroupGetDirection(0x200, &pCamera->direction);
    pCamera->direction.z = -pCamera->direction.z;
    radius /= sdfAtan(0.34906582f);
    nbCameraBoss_BSALBI_E912_Rate = 1.0f;
    pCamera->fovy = 0.69813163f;
    pCamera->distance = radius;
    nbCameraBoss_BSALBI_E912_Distance = radius;
}

void nbCameraBoss_BSALBI_E912_Run(void)
{
    f32 rate = nbCameraBoss_BSALBI_E912_Rate;
    f32 distance = nbCameraBoss_BSALBI_E912_Distance;

    nbCameraBoss_BSALBI_E912_Rate = rate * 0.95f;
    pNbCamera.distance = distance * 0.65f + distance * 0.35f * rate;
}

s32 nbCameraBoss_BSALBI_InvokeTargetParty_Set(nbCamera_t *pCamera)
{
    s32 type;

    effBTLModelAllShow();
    type = dds3GetRandIntA(0, 2);
    switch (type) {
    case 0:
        func_0016E120();
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = 0.36f;
        pCamera->direction.z = -0.88f;
        pCamera->target.x = 510.01996f;
        pCamera->target.y = -852.57f;
        pCamera->target.z = 1895.83f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.03f;
        pCamera->direction.y = 0.17f;
        pCamera->direction.z = 0.98f;
        pCamera->target.x = -62.22f;
        pCamera->target.y = -493.04f;
        pCamera->target.z = -1677.5599f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

void nbCameraBoss_Bsnet_E907_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

void nbCameraBoss_Bsnet_E907_Run(void)
{
    pNbCamera.distance -= 60.0f;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_E907_2_Set);

void nbCameraBoss_Bsnet_E907_2_Run(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 rate = Bsnet_E907_2_Rate;
    f32 step = 1.0f - rate;

    Bsnet_E907_2_Rate = rate * 0.925f;
    if (step > 0.0f) {
        if (step > 1.0f) {
            step = 1.0f;
        }
        VU0_LOAD_VF10(&Bsnet_E907_2_EndTarget);
        VU0_LOAD_VF11(&D_0056B420);
        VU0_LERP_VF10_VF11(step);
        VU0_STORE_VF10(pCamera);
        VU0_LOAD_VF10(&Bsnet_E907_2_EndDirection);
        VU0_LOAD_VF11(&D_0056B430);
        VU0_LERP_VF10_VF11(step);
        VU0_STORE_VF10(&pCamera->direction);
        pCamera->distance = Bsnet_E907_2_StartDistance +
                         (Bsnet_E907_2_EndDistance - Bsnet_E907_2_StartDistance) * step;
    }
}

void nbCameraBoss_Bsnet_E905_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_Cut1_Set);

s32 nbCameraBoss_BSARI_Cut1_Run(nbCamera_t *pCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.9f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetSphereCenter(pForm);
    VU0_STORE_VF10(pCamera);
    pCamera->target.y -= pForm->h * pForm->scl * 0.15f;
    VU0_LOAD_VF10(pCamera);
    VU0_COPY_VF10_TO_VF12();
    VU0_LOAD_VF11(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    VU0_COPY_VF10_TO_VF11();
    VU0_COPY_VF12_TO_VF10();
    VU0_SUB_VF10_VF11();
    VU0_LENGTH_VF10(length);
    pCamera->distance = length;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut1_Set(nbCamera_t *pCamera)
{
    s32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 2);
    switch (type) {
    case 0:
        nbCameraBSARICut1Vec.x = 772.02f;
        nbCameraBSARICut1Vec.y = -310.88f;
        nbCameraBSARICut1Vec.z = -535.27f;
        break;
    case 1:
        nbCameraBSARICut1Vec.x = -804.65f;
        nbCameraBSARICut1Vec.y = -314.06f;
        nbCameraBSARICut1Vec.z = -799.75f;
        break;
    }
    BSARI_Cut1_Rate = 1.0f;
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut1_Run(nbCamera_t *pCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.985f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetMarkPosition2(pForm, 1);
    VU0_STORE_VF10(pCamera);
    VU0_COPY_VF10_TO_VF12();
    VU0_LOAD_VF11(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    VU0_COPY_VF10_TO_VF11();
    VU0_COPY_VF12_TO_VF10();
    VU0_SUB_VF10_VF11();
    VU0_LENGTH_VF10(pCamera->distance);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI_MASEHA_Set(nbCamera_t *pCamera)
{
    nbCameraBoss_BSARI_MASEHA_Frame = 0;
    nbCameraBoss_BSARI_MASEHA_Type = dds3GetRandIntA(0, 3);
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_MASEHA_Run);

s32 nbCameraBoss_BSARI_SIMANO_Set(nbCamera_t *pCamera)
{
    nbCameraBoss_BSARI_SIMANO_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_BSARI_SIMANO_Rate = 1.0f;
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_SIMANO_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_AISHOH_Set);


void nbCameraBoss_Kagu_E909_Set(void)
{
    pNbCamera.frame = 0;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_E909_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_Attack_Set);

s32 nbCameraBoss_Kagu_FLEACH_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    if (nbCameraBossCutFlag == 1) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.4f;
            pCamera->direction.y = 0.22f;
            pCamera->direction.z = 0.89f;
            pCamera->target.x = 949.28992f;
            pCamera->target.y = -1372.0599f;
            pCamera->target.z = -2017.89f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.01f;
            pCamera->direction.y = 0.26f;
            pCamera->direction.z = 0.97f;
            pCamera->target.x = -98.7f;
            pCamera->target.y = -1369.1199f;
            pCamera->target.z = -2463.0596f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.34f;
            pCamera->direction.y = 0.07f;
            pCamera->direction.z = 0.94f;
            pCamera->target.x = -1205.4999f;
            pCamera->target.y = -669.11994f;
            pCamera->target.z = -1848.38f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.01f;
            pCamera->direction.y = 0.26f;
            pCamera->direction.z = 0.97f;
            pCamera->target.x = -98.7f;
            pCamera->target.y = -1369.1199f;
            pCamera->target.z = -2463.0596f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_MJINKO_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Pish_PRESS_Set);

s32 nbCameraBoss_Pish_MUHENK_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    nbCameraBoss_Pish_MUHENK_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_Pish_MUHENK_Frame = 0;
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Pish_MUHENK_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_Command_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bskagu_Cut1_Set);


s32 nbCameraBoss_Bskagu_Cut1_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bspish_Cut1_Set);


s32 nbCameraBoss_Bspish_Cut1_Run(nbCamera_t *pCamera)
{
    return 1;
}

void nbCameraBoss_BSMETA_E914_0_Set(void)
{
    effBTLModelAllShow();
    pNbCamera.direction.x = 0.56f;
    pNbCamera.direction.y = -0.19f;
    pNbCamera.direction.z = 0.81f;
    pNbCamera.target.x = -946.41993f;
    pNbCamera.target.y = -103.86f;
    pNbCamera.target.z = -904.08991f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

void nbCameraBoss_BSMETA_E914_1_Set(void)
{
    effBTLModelAllShow();
    pNbCamera.direction.x = -0.2f;
    pNbCamera.direction.y = 0.29f;
    pNbCamera.direction.z = 0.94f;
    pNbCamera.target.x = 441.29f;
    pNbCamera.target.y = -762.66993f;
    pNbCamera.target.z = -1527.5899f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

s32 nbCameraBoss_Meta_METAT1_0_Set(nbCamera_t *pCamera)
{
    pCamera->direction.x = 0.47f;
    pCamera->direction.y = -0.28f;
    pCamera->direction.z = 0.84f;
    pCamera->target.x = -229.18f;
    pCamera->target.y = -433.18f;
    pCamera->target.z = 261.52997f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Meta_METAT1_1_Set);


void nbCameraBoss_BSBERU_E915_Init(void)
{
    nbCameraBoss_BSBERU_E915_Frame = 0;
    nbCameraBoss_BSBERU_E915_Rate = 1.0f;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_0_Set);

void nbCameraBoss_BSBERU_E915_1_Set(void)
{
    pNbCamera.direction.x = -0.27f;
    pNbCamera.direction.y = 0.37f;
    pNbCamera.direction.z = 0.89f;
    pNbCamera.target.x = 361.64997f;
    pNbCamera.target.y = -627.90992f;
    pNbCamera.target.z = -651.73993f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

void nbCameraBoss_BSBERU_E915_2_Set(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate = nbCameraBoss_BSBERU_E915_Rate;
    f32 blend;

    nbCameraBoss_BSBERU_E915_Rate = rate * 0.99f;
    blend = (s32)nbCameraBoss_BSBERU_E915_Frame / 30.0f;
    from.x = 361.64997f;
    from.y = -627.90992f;
    from.z = -651.73993f;
    to.x = 511.64f;
    to.y = -606.7f;
    to.z = -1581.8299f;
    blend *= rate;
    VU0_LOAD_VF10(&from);
    VU0_LOAD_VF11(&to);
    VU0_LERP_VF10_VF11(blend);
    VU0_STORE_VF10(pCamera);
    from.x = -0.27f;
    from.y = 0.37f;
    from.z = 0.89f;
    to.x = -0.23f;
    to.y = 0.13f;
    to.z = 0.96f;
    VU0_LOAD_VF10(&from);
    VU0_LOAD_VF11(&to);
    VU0_LERP_VF10_VF11(blend);
    VU0_STORE_VF10(&pCamera->direction);
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    nbCameraBoss_BSBERU_E915_Frame++;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Beru_HAENO1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_Command_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Lusi_Cut1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Lusi_TWORLD_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Lusi_OHNOOH_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Lusi_ASIKIK_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_BossTargetInvoke_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_BossEX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Cut1_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Cut1_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Cut2_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Cut2_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_CheckRotate);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_CheckFix);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Command_Init);

s32 nbCameraBoss_BSARI1_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.26f;
        pCamera->direction.y = 0.33f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = 746.94f;
        pCamera->target.y = -1248.2499f;
        pCamera->target.z = -1527.32f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.62f;
        pCamera->direction.y = 0.3f;
        pCamera->direction.z = 0.73f;
        pCamera->target.x = -1308.1398f;
        pCamera->target.y = -960.32f;
        pCamera->target.z = -1071.01f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI1_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = -0.1f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 740.91993f;
        pCamera->target.y = -251.30999f;
        pCamera->target.z = -1268.8299f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.45f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = -865.67f;
        pCamera->target.y = -294.74f;
        pCamera->target.z = -1501.93f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_Cut23_EX03_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut23_EX01_Set);

s32 nbCameraBoss_BSARI2_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.32f;
        pCamera->direction.y = 0.08f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 774.63f;
        pCamera->target.y = -461.97f;
        pCamera->target.z = -1267.7199f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.56f;
        pCamera->direction.y = 0.06f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = -1213.39f;
        pCamera->target.y = -383.00995f;
        pCamera->target.z = -1264.4799f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX03_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    func_0016E120();
    pCamera->direction.x = 0.49f;
    pCamera->direction.y = 0.36f;
    pCamera->direction.z = 0.8f;
    pCamera->target.x = -997.73993f;
    pCamera->target.y = -1100.6099f;
    pCamera->target.z = -1215.93f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSNET1_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSNET_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_EP_IT_Set);

s32 nbCameraBoss_BSKAGU_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.61f;
        pCamera->direction.y = 0.29f;
        pCamera->direction.z = 0.74f;
        pCamera->target.x = 1385.7698f;
        pCamera->target.y = -1184.8399f;
        pCamera->target.z = -892.5199f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.64f;
        pCamera->direction.y = -0.03f;
        pCamera->direction.z = 0.77f;
        pCamera->target.x = -1443.05f;
        pCamera->target.y = -321.31f;
        pCamera->target.z = -830.79f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSPISH_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSPISH_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSPISH_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSPISH_Cut23_EX03_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSMETA_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSMETA_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_Cut23_EX02_Set);


s32 nbCameraBoss_BSLUSI_Cut23_AllParty_Set(nbCamera_t *pCamera)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Cut2_Ptr_Init);

s32 nbCameraBoss_Cut2_Ptr_Run(nbCamera_t *pCamera, nbCameraSkillInfo_t *pSkillInfo)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHero_Cut1_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHero_Cut1_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHero_Cut2_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHero_Cut2_Run);

INCLUDE_RODATA(const s32, "code_001B0858", D_005357E0);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraDANTE_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraDANTE_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraDANTE_DCOMBO_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraDANTE_KAGIRI_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraDANTE_DCOMBO_KAGU_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperFileLoadPB);

nbCameraHelper_t *nbCameraHelperCreate(const char *name)
{
    nbCameraHelper_t *pHelper = sdfChipAllocDirect(0x3C);

    pHelper->hPart = nbCameraHelperFileLoadPB(name);
    pHelper->unk30 = 0;
    pHelper->unk34 = nbCameraHelperLength(pHelper);
    return pHelper;
}

void nbCameraHelperDelete(nbCameraHelper_t *pHelper)
{
    dds3EffectPartDelete(pHelper->hPart);
    sdfChipFreeDirect(pHelper);
}

u16 nbCameraHelperLength(nbCameraHelper_t *pHelper)
{
    return dds3EffectPartGetHandle(pHelper->hPart)->first->frameLength;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperRun);

void nbCameraHelperModelSetSub(mdlObj_t *hModel, u32 target, u32 eye)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 length;

    if (sdfModelGetMarkPos(hModel->inner, target) == 0) {
        printf(D_00535850);
    }
    VU0_STORE_VF10(pCamera);
    if (sdfModelGetMarkPos(hModel->inner, eye) == 0) {
        printf(D_00535878);
    }
    VU0_COPY_VF10_TO_VF11();
    VU0_LOAD_VF10(pCamera);
    VU0_SUB_VF10_VF11();
    VU0_LENGTH_VF10(length);
    pCamera->distance = length;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(&pCamera->direction);
    pCamera->fovy = 0.7853981f;
}


void nbCameraHelperModelSet(mdlObj_t *hModel)
{
    nbCameraHelperModelSetSub(hModel, 0, 1);
}

s32 nbCameraHelperViewer_PB_Init(s32 unused)
{
    void *hFileList = dds3EffectFileListCreate(D_005358A8, 0x10);
    void *pWork;

    nbCameraHelperViewerPBHandle = hFileList;
    pWork = dds3EffectFileListWindowCreate(hFileList);
    nbCameraHelperViewerPBWork = pWork;
    dds3EffectFileListWindowSetPos(pWork, 0xF0, 0xD0);
    dds3EffectFileListWindowSetOt(nbCameraHelperViewerPBWork, 1);
    dds3EffectFileListWindowSetPadWait(nbCameraHelperViewerPBWork, 0x53);
    dds3EffectFileListWindowSetWindowColor(nbCameraHelperViewerPBWork, 0x80806020, 0x80000000);
    nbCameraHelperViewerMode = NULL;
    return unused;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperViewer_PB_Draw);

void nbCameraHelperViewer_PB_End(void *pData)
{
    dds3EffectFileListWindowDelete(nbCameraHelperViewerPBWork);
    dds3EffectFileListDelete(nbCameraHelperViewerPBHandle);
}

s32 nbCameraHelperViewer_Init(s32 unused)
{
    s32 size;

    if (nbCameraHelperViewerMode != NULL) {
        if (D_0054F3DC != NULL) {
            nbCameraHelperDelete(D_0054F3DC);
            if (D_0054F3E0 != NULL) {
                sdfMemFreeDirect(D_0054F3E0);
            }
            if (D_0054F3E4 != NULL) {
                sdfMemFreeDirect(D_0054F3E4);
            }
        }
        D_0054F3DC = nbCameraHelperCreate(nbCameraHelperViewerMode);
        nbCameraHelperViewerMode = NULL;
        size = nbCameraHelperLength(D_0054F3DC) * 16;
        D_0054F3E0 = sdfMemAllocDirect(size);
        nbCameraHelperViewerState = sdfMemLock(D_0054F3E0);
        D_0054F3E4 = sdfMemAllocDirect(size);
        nbCameraHelperViewerHandle = sdfMemLock(D_0054F3E4);
    }
    return unused;
}

INCLUDE_RODATA(const s32, "code_001B0858", D_00535850);

INCLUDE_RODATA(const s32, "code_001B0858", D_00535878);

INCLUDE_RODATA(const s32, "code_001B0858", D_00535898);

INCLUDE_RODATA(const s32, "code_001B0858", D_005358A8);

INCLUDE_RODATA(const s32, "code_001B0858", D_005358C0);

INCLUDE_RODATA(const s32, "code_001B0858", D_005358D0);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperViewer_Draw);

void nbCameraHelperViewer_End(void) {
}
INCLUDE_SDATA(const s32, "code_001B0858", D_0054F370);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F374);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F378);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F37C);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F380);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F384);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F388);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraCRSetPlayerFormRequestFlag);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F38D);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraState_CommandRoot_OrderIndex);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraPressRunFlag);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F395);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraPressThrough);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBossCutFlag);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3A0);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSARI_MASEHA_Frame);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSARI_MASEHA_Type);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSARI_SIMANO_Type);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSARI_SIMANO_Rate);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_Kagu_MJINKO_Type);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_Pish_MUHENK_Type);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3B9);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_Pish_MUHENK_Frame);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_Frame);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_Rate);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3C8);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraBoss_BSLUSI_Type);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3D0);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3D4);

INCLUDE_SDATA(const s32, "code_001B0858", nbCameraHelperViewerMode);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3DC);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3E0);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3E4);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3E8);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3E9);

INCLUDE_SDATA(const s32, "code_001B0858", D_0054F3F0);

