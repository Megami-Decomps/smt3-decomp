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
    s32 unk24;
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

typedef struct nbCamera_t {
    u128 vec00;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u32 state;
    u32 stateFrame;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    u32 unk3C;
    u8 unk40[8];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    f32 unk54;
} nbCamera_t;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct nbCameraHelper_t {
    u8 unk00[0x38];
    effPartHandle_t *hPart;
} nbCameraHelper_t;

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
typedef struct nbCameraSkillInfo_t nbCameraSkillInfo_t;

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

INCLUDE_ASM(const s32, "code_001B0858", dds3_1B09C0);

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
    pCamera->unk48 = 0;
    pCamera->unk4C = 0;
    pCamera->unk50 = 0;
    pCamera->unk54 = 0.0f;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraAnim_RotateSet);

void func_001B1518(nbCamera_t *pCamera, s32 frames, s32 count, f32 angle)
{
    f32 step = -angle / (f32)frames;

    pCamera->unk50 = frames;
    pCamera->unk54 = step;
    pCamera->unk20 += step * (f32)count;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_InvokeTargetSelect_SearchSet);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_InvokeTarget_SearchSet);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_EnemyParty_SearchSet);

void nbCameraState_DebugShotAll_Init(void)
{
    nbCameraShot_All_Set(&pNbCamera);
}


void nbCameraSetState(u16 state)
{
    pNbCamera.state = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.stateFrame = 0;
    }
}

void func_001B1770(u16 state)
{
    pNbCamera.state = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.stateFrame = 0;
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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraScriptSet1Shot);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraScriptSetBougai);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraScriptSet);

void nbCameraInit(void)
{
    nbCameraShot_All_Set(&pNbCamera);
    pNbCamera.state = 0;
    pNbCamera.unk34 = 0;
    pNbCamera.unk30 = 0;
    pNbCamera.stateFrame = 0;
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

INCLUDE_ASM(const s32, "code_001B0858", nbInitCameraProcess);

INCLUDE_ASM(const s32, "code_001B0858", func_001B2158);

INCLUDE_ASM(const s32, "code_001B0858", func_001B2270);

INCLUDE_ASM(const s32, "code_001B0858", nb_1B2340);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_Animation);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_BA_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_BA_Animation);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Start_Init);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraStart);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_PullOne_Calc);

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
    SDF_VU0_COPYVECTOR(&nbCameraAnalyzeVec, pCamera);
    nbCamera_Analyze_Distance = pCamera->unk20;
    nbCamera_Analyze_Rate = 1.0f;
    nbCamera_Analyze_Rate_Add = -0.01f;
    nbCamera_Analyze_Frame = 1;
    nbCamera_Analyze_Wait = 5;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_Analyze_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_All_Set);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraEnemyPartySet);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_EnemyBad_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_EnemyBad_Calc);

void nbCameraState_NegoBougai_Init(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    SDF_VU0_COPYVECTOR(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->unk20;
    nbCamera_NegoBougai_Wait = 15;
    nbCamera_NegoBougai_Frame = 0;
}

void nbCameraState_NegoBougai_Init2(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    SDF_VU0_COPYVECTOR(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->unk20;
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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoTarget_Calc);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoTarget_Calc2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraState_NegoHojo_Init);

void nbCameraState_NegoHojo_Calc(void)
{
    nbCamera_NegoHojo_Rate = nbCamera_NegoHojo_Rate * 0.925f;
    pNbCamera.unk20 = nbCamera_NegoHojo_Distance * (1.0f - nbCamera_NegoHojo_Rate);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraGetModelAllTable);

f32 nbCameraGetModelRadius(nbFormation_t *pFormation)
{
    f32 radius = nbCameraGetModelAllTable(pFormation)->radius;

    if (radius == 0.0f) {
        return pFormation->r * pFormation->scl;
    }
    return radius;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraGroupGetMaxLength);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraGroupGetSphere);


void nbCameraCommandSetFixPosition(void)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_CommandFixPos_Set2);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_CommandFixPos_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_AllParty_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraShot_FriendParty_Set);

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
    pNbCamera.unk3C = 0;
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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Samael_AKUI_Set);


s32 nbCameraBoss_Samael_AKUI_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Sukady_DAIJIS_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Baal_BAELNO_Set);


s32 nbCameraBoss_Baal_BAELNO_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Attack_Cut1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Attack_Cut1_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Cut1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Cut1_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_Attack_Set);


s32 nbCameraBoss_Bsnet_Attack_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSALBI_E912_Set);

void nbCameraBoss_BSALBI_E912_Run(void)
{
    f32 rate = nbCameraBoss_BSALBI_E912_Rate;
    f32 distance = nbCameraBoss_BSALBI_E912_Distance;

    nbCameraBoss_BSALBI_E912_Rate = rate * 0.95f;
    pNbCamera.unk20 = distance * 0.65f + distance * 0.35f * rate;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSALBI_InvokeTargetParty_Set);

void nbCameraBoss_Bsnet_E907_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

void nbCameraBoss_Bsnet_E907_Run(void)
{
    pNbCamera.unk20 -= 60.0f;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_E907_2_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Bsnet_E907_2_Run);

void nbCameraBoss_Bsnet_E905_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_Command_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_Cut1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI_Cut1_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut1_Run);

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
    pNbCamera.unk3C = 0;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_E909_Run);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_Attack_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Kagu_FLEACH_Set);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSMETA_E914_0_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSMETA_E914_1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Meta_METAT1_0_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_Meta_METAT1_1_Set);


void nbCameraBoss_BSBERU_E915_Init(void)
{
    nbCameraBoss_BSBERU_E915_Frame = 0;
    nbCameraBoss_BSBERU_E915_Rate = 1.0f;
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_0_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_1_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_E915_2_Set);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_Cut23_EX03_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut23_EX01_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_Cut23_EX03_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI1_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSARI2_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSNET1_Cut23_EX02_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSNET_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSBERU_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSLUSI_EP_IT_Set);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraBoss_BSKAGU_Cut23_EX01_Set);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperCreate);

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

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperModelSetSub);


void nbCameraHelperModelSet(mdlObj_t *hModel)
{
    nbCameraHelperModelSetSub(hModel, 0, 1);
}

INCLUDE_RODATA(const s32, "code_001B0858", D_00535850);

INCLUDE_RODATA(const s32, "code_001B0858", D_00535878);

INCLUDE_RODATA(const s32, "code_001B0858", D_00535898);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperViewer_PB_Init);

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperViewer_PB_Draw);

void nbCameraHelperViewer_PB_End(void *pData)
{
    dds3EffectFileListWindowDelete(nbCameraHelperViewerPBWork);
    dds3EffectFileListDelete(nbCameraHelperViewerPBHandle);
}

INCLUDE_ASM(const s32, "code_001B0858", nbCameraHelperViewer_Init);

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

