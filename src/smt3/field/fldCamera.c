#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldFollowCameraControl_t fldFollowCameraControl_t;

/* fldGetFollowCameraControl owns the follow-camera basic at +0x78. */
struct fldFollowCameraControl_t {
    u8 unk00[0x78];
    dds3Basic_t *camera;
};

extern void *memset(void *, int, unsigned int);

f32 sdfSin(f32 aRadius);

f32 sdfCos(f32 aRadius);

fldFollowCameraControl_t *fldGetFollowCameraControl(void);

void dds3SetPosPoseSet(dds3Basic_t *pBasic, u128 *pos);

extern f32 fldCamPhokanSelX[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

extern f32 fldCamPhokanEyeX[];

extern f32 fldCamPhokanTgtX[];

extern f32 D_002FC9C0[];

extern f32 fldCamTarget[];

dds3Basic_t *fldGetUnitFollowCamera(void);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

dds3Basic_t *dds3GetActiveScene(void);

extern f32 fldCamElvState;

extern f32 D_0055DB20[];

extern f32 D_0055DB10[];

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *, u32);

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern s32 strcmp(const char *left, const char *right);

s32 evtBitCheck(s32 no);

extern s32 fldCamResetOn;

extern s32 fldCamMiwatasi;

extern s32 fldElvCameraScene;

extern void fldCamSetChaseCore(f32 height);

/* fldUnitControl_t as fldGetUnitControl returns it. */
typedef struct {
    dds3Basic_t *handle;
    dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

extern fldUnitControl_t *fldGetUnitControl(void);

extern f32 fldCamLength;

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    s32 angle = (s32)(360.0f - fldCalcAngleBetween(fromX, fromZ, toX, toZ) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "field/fldCamera", fldCamCalcSlopeOffset);

void fldCamSetLeng(void) {
    f32 dx = D_002FC9C0[0] - fldCamTarget[0];
    f32 dy = D_002FC9C0[1] - fldCamTarget[1];
    f32 dz = D_002FC9C0[2] - fldCamTarget[2];

    fldCamLength = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

void fldCamResetNpc(void)
{
    fldCamResetOn = 0;
}

INCLUDE_ASM(const s32, "field/fldCamera", fldCamCalcSaka);

INCLUDE_ASM(const s32, "field/fldCamera", resetCamNormal);

INCLUDE_ASM(const s32, "field/fldCamera", resetCamNormal2);

extern f32 fldCamPitch;

extern f32 fldCamParamTable[][4];

/* NON_MATCHING: 3 of 95 words differ (first at +0x90). */
#ifdef NON_MATCHING
void fldCamGetPhokanParam(void) {
    f32 angle;

    fldCamPhokanEyeX[0] =
        fldGlobalBase.x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex][1];
    fldCamPhokanEyeX[2] =
        fldGlobalBase.z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[3] = 1.0f;
    angle = fldCamPitch * 3.14f / 180.0f;
    fldCamPhokanTgtX[0] = fldGlobalBase.x +
        sdfSin(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex][0];
    fldCamPhokanTgtX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex][2];
    fldCamPhokanTgtX[2] = fldGlobalBase.z +
        sdfCos(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex][0];
    fldCamPhokanTgtX[3] = 1.0f;
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", fldCamGetPhokanParam);
#endif

void fldCamUpdateFollow(void) {
    union {
        u128 q;
        f32 f[4];
    } nearPoint, farPoint;
    fldFollowCameraControl_t *control = fldGetFollowCameraControl();
    f32 ratio = (f32)fldCamPhokanState / (f32)fldCamPHokanMode;
    f32 remaining = 1.0f - ratio;
    dds3Basic_t *node;

    nearPoint.f[0] = fldCamPhokanEyeX[0] * ratio + fldCamPhokanSelX[0] * remaining;
    nearPoint.f[1] = fldCamPhokanEyeX[1] * ratio + fldCamPhokanSelX[1] * remaining;
    nearPoint.f[2] = fldCamPhokanEyeX[2] * ratio + fldCamPhokanSelX[2] * remaining;
    farPoint.f[0] = fldCamPhokanTgtX[0] * ratio + fldCamPhokanTgtX[0] * remaining;
    farPoint.f[1] = fldCamPhokanTgtX[1] * ratio + fldCamPhokanTgtX[1] * remaining;
    farPoint.f[2] = fldCamPhokanTgtX[2] * ratio + fldCamPhokanTgtX[2] * remaining;
    SDF_VU0_COPYVECTOR(D_002FC9C0, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    dds3SetPosPoseSet(control->camera, &farPoint.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamPhokanState++;
    if (fldCamPHokanMode < fldCamPhokanState) {
        fldGlobalBase.mode = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldCamera", calcCamNormal);

INCLUDE_ASM(const s32, "field/fldCamera", calcCamDirLock);

extern s32 fldNpcCamTarget;

extern f32 fldCamNpcTarget;

extern f32 fldCamNpcTarget2;

extern f32 D_0054EF6C;

extern f32 D_0054EF70;

extern f32 D_0054EF74;

extern f32 D_0054EF78;

extern f32 D_0054EF80;

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, void *out);

extern void dds3GetTMPoseSet(dds3Basic_t *object);

void calcCamNormalNPC(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 dist;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetPosPoseSet(unit->handle);
    eye.f[3] = 1.0f;
    fldGlobalBase.menuOpenFlag = 4;
    fldGlobalBase.encountState = 5;
    if (fldNpcCamTarget == 2) {
        fldCamResetOn = 2;
        eye.f[0] = D_0054EF6C;
        eye.f[2] = D_0054EF74;
        eye.f[1] = D_0054EF70 + fldCamNpcTarget + 60.0f;
    } else {
        fldCamResetOn = 1;
        eye.f[0] = D_0054EF6C;
        eye.f[2] = D_0054EF74;
        eye.f[1] = D_0054EF70 + fldCamNpcTarget;
    }
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    dds3GetTMPoseSet(unit->handle);
    target.f[3] = 1.0f;
    if (fldNpcCamTarget == 2) {
        angle = 3.14f;
        angle = -D_0054EF78 * angle;
        angle = angle / 180.0f;
        dist = 200.0f;
        target.f[0] = eye.f[0] + sdfSin(angle) * dist;
        target.f[1] = D_0054EF70 + fldCamNpcTarget;
        target.f[2] = eye.f[2] + sdfCos(angle) * dist;
    } else {
        angle = 3.14f;
        angle = -D_0054EF78 * angle;
        angle = angle / 180.0f;
        dist = 200.0f;
        target.f[0] = eye.f[0] + sdfSin(angle) * (fldCamNpcTarget2 + dist);
        target.f[1] = D_0054EF70 + D_0054EF80;
        target.f[2] = eye.f[2] + sdfCos(angle) * (fldCamNpcTarget2 + dist);
    }
    dds3SetPosPoseSet(control->camera, &target.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
}

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);

extern f32 fldCamPitch;

extern f32 fldCamParamTable[][4];

/* NON_MATCHING: 36 of 126 words differ (first at +0x5C). */
#ifdef NON_MATCHING
void resetCamTgtPlayer(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 dx;
    f32 dy;
    f32 dz;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetRotPoseSet(unit->handle);
    angle = EulerAngleAxisY() * 180.0f / 3.14f;
    fldGlobalBase.camAngle = angle;
    fldGlobalBase.camAngle2 = angle;
    eye.f[0] = D_002FC9C0[0] + (fldGlobalBase.x - D_002FC9C0[0]);
    eye.f[1] = D_002FC9C0[1] +
               (fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex][1] - D_002FC9C0[1]);
    eye.f[2] = D_002FC9C0[2] + (fldGlobalBase.z - D_002FC9C0[2]);
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    dds3GetTMPoseSet(unit->handle);
    target.f[3] = 1.0f;
    target.f[0] = fldGlobalBase.campos.x;
    target.f[1] = fldGlobalBase.campos.y;
    target.f[2] = fldGlobalBase.campos.z;
    dds3SetPosPoseSet(control->camera, &target.q);
    target.f[0] = fldCamTarget[0] + (target.f[0] - fldCamTarget[0]);
    target.f[1] = fldCamTarget[1] + (target.f[1] - fldCamTarget[1]);
    target.f[2] = fldCamTarget[2] + (target.f[2] - fldCamTarget[2]);
    dds3SetPosPoseSet(control->camera, &target.q);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    dx = eye.f[0] - target.f[0];
    dy = eye.f[1] - target.f[1];
    dz = eye.f[2] - target.f[2];
    fldCamLength = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamPitch = GetSphericalPosMove(target.f[0], target.f[2], eye.f[0], eye.f[2]) + 180.0f;
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", resetCamTgtPlayer);
#endif

INCLUDE_ASM(const s32, "field/fldCamera", fldEventUpdateEx_Unity_2);

INCLUDE_ASM(const s32, "field/fldCamera", fldCamSetChaseCore);

void fldCamSetChase(void)
{
    fldCamSetChaseCore(0.0f);
}

void fldCamUpdateDefault(void)
{
    fldCamSetChaseCore(8.0f);
}

extern u8 dds3PadStickState[2][2][2];

extern f32 fldCamPitch;

extern f32 D_0054ECB4;

extern f32 D_0054ECB8;

/* NON_MATCHING: 28 of 148 words differ (first at +0x16C). */
#ifdef NON_MATCHING
void fldCamUpdateChase(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    dds3Basic_t *node;

    control = fldGetFollowCameraControl();
    memset(&eye, 0, sizeof(eye));
    eye.f[3] = 1.0f;
    memset(&target, 0, sizeof(target));
    target.f[3] = 1.0f;
    D_0054ECB4 = (f32)(dds3PadStickState[0][0][0] - 0x80) * 0.7f + fldCamPitch;
    D_0054ECB8 = -((f32)(dds3PadStickState[0][0][1] - 0x80) * 2.5f);
    eye.f[0] = fldGlobalBase.x - sdfSin(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.f[1] = fldGlobalBase.y - 200.0f + D_0054ECB8;
    eye.f[2] = fldGlobalBase.z - sdfCos(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.f[0] = D_002FC9C0[0] + (eye.f[0] - D_002FC9C0[0]) * 0.0625f;
    eye.f[1] = D_002FC9C0[1] + (eye.f[1] - D_002FC9C0[1]) * 0.0625f;
    eye.f[2] = D_002FC9C0[2] + (eye.f[2] - D_002FC9C0[2]) * 0.0625f;
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    target.f[1] = fldCamTarget[1] + (fldGlobalBase.y - 200.0f - fldCamTarget[1]) * 0.0625f;
    target.f[0] = fldCamTarget[0] + (fldGlobalBase.x - fldCamTarget[0]) * 0.0625f;
    target.f[2] = fldCamTarget[2] + (fldGlobalBase.z - fldCamTarget[2]) * 0.0625f;
    dds3SetPosPoseSet(control->camera, &target.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", fldCamUpdateChase);
#endif

dds3Basic_t *fldCamNowEveCamera(void) {
    dds3Basic_t *worldObject = dds3GetActiveScene();
    dds3Basic_t *currentEntry = dds3GetActiveCameraSceneBasic(worldObject);

    if (fldGetUnitFollowCamera() == currentEntry) {
        currentEntry = NULL;
    }
    return currentEntry;
}

INCLUDE_ASM(const s32, "field/fldCamera", calcCamSyukan);

void fldSetElvCameraScene(u32 mode) {
    fldElvCameraScene = mode;
    dds3CameraObjectBasicGetTgtCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), D_0055DB20,
                                              D_0055DB10);
    fldCamElvState = 0;
}

void fldCamElvTask(void) {
    f32 direction = 0.0f;
    f32 phase = fldCamElvState;
    dds3Basic_t *camera;

    if (fldElvCameraScene != 0) {
        if (fldElvCameraScene == 1) {
            direction = 1.0f;
        }
        if (fldElvCameraScene == 2) {
            direction = 1.0f;
        }
        if (fldElvCameraScene == -1) {
            direction = -1.0f;
        }
        if (fldElvCameraScene == -2) {
            direction = -1.0f;
        }
        camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        if (fldElvCameraScene == 1 || fldElvCameraScene == -1) {
            if (phase < 3.14f) {
                phase += 0.2f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.5f;
            } else {
                phase += 0.02f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.0f;
            }
        }
        if (fldElvCameraScene == 2 || fldElvCameraScene == -2) {
            phase += 11.0f;
            if (phase > 0.0f) {
                phase -= 1.0f;
            } else if (phase > -3.14f) {
                phase -= 0.2f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.0f;
            } else {
                phase -= 0.01f;
            }
            phase -= 11.0f;
        }
        fldCamElvState = phase;
        dds3OnPoseSetFlag(camera->poseset, POSESET_FLAG_UPDATEPOSE);
    }
}

void fldClearElvCameraScene(void)
{
    fldElvCameraScene = 0;
}

INCLUDE_RODATA(const s32, "field/fldCamera", D_00531BF0);

INCLUDE_ASM(const s32, "field/fldCamera", fldCamMain);

extern void *dds3SearchProcessName(const char *namestr);

extern void dds3SetScreenFovy(f32 fovy);

extern f32 fldCamParamTable[][4];

extern char D_00531BF0[];

void fldCamSyukanSuriawase(void) {
    fldAreaWork_t *state;

    if (dds3SearchProcessName("fldProcSequence") == NULL) {
        return;
    }
    state = &fldGlobalBase;
    if (state->mode != 0) {
        return;
    }
    if (evtBitCheck(0xAF0) == 0) {
        return;
    }
    if (scrSearchScriptName(D_00531BF0) != NULL) {
        return;
    }
    if (dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) != fldGetUnitFollowCamera()) {
        return;
    }
    dds3SetScreenFovy(fldCamParamTable[state->cameraParamIndex][3] * 3.14f / 180.0f);
}

s32 fldCamMiwatasiChk(void)
{
    return fldCamMiwatasi;
}

s32 fldCamNpcCamChk(char *pName) {
    if (pName == NULL) {
        return 0;
    }
    if (fldGlobalBase.mode == 5 && strcmp(fldGlobalBase.requestedSceneName, pName) == 0) {
        return 1;
    }
    return 0;
}
INCLUDE_SDATA(const s32, "field/fldCamera", D_0054EC88);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamDist);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamMiwatasi);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054EC94);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054EC98);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054EC9C);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECA0);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamHeight);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamParamTable2);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamResetOn);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECB0);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECB4);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECB8);

INCLUDE_SDATA(const s32, "field/fldCamera", fldElvCameraScene);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamElvState);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECC4);

INCLUDE_SDATA(const s32, "field/fldCamera", fldHitChkMatter);

INCLUDE_SDATA(const s32, "field/fldCamera", fldHitChkSlope);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamPhokanState);

INCLUDE_SDATA(const s32, "field/fldCamera", fldCamPHokanMode);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECD8);

INCLUDE_SDATA(const s32, "field/fldCamera", D_0054ECDC);

