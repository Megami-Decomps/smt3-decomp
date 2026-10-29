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

void dds3SetPosPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *pos);

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

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, f32 *tgtpos,
                                                      f32 *campos);

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

f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs) {
    s32 i, j, k;
    f32 xi, yi, zi, xj, yj, zj, xk, yk, zk;
    f32 dx, dz, lenA, lenB, travelA, travelB;
    f32 zA, zB, yA, yB, width, deltaZ, correction, height;
    if (xs[0] == xs[1]) { i = 2; j = 0; k = 1; }
    else if (xs[0] == xs[2]) { i = 1; j = 0; k = 2; }
    else { i = 0; j = 1; k = 2; }
    xi = xs[i]; yi = ys[i]; zi = zs[i];
    xj = xs[j]; yj = ys[j]; zj = zs[j];
    xk = xs[k]; yk = ys[k]; zk = zs[k];
    dx = xj - xi; dz = zj - zi;
    lenA = fsqrtf(dx * dx + dz * dz);
    dx = xk - xi; dz = zk - zi;
    lenB = fsqrtf(dx * dx + dz * dz);
    travelA = lenA * (x - xi) / (xj - xi);
    travelB = lenB * (x - xi) / (xk - xi);
    zA = (zj - zi) * travelA / lenA + zi;
    zB = (zk - zi) * travelB / lenB + zi;
    yA = (yj - yi) * travelA / lenA + yi;
    yB = (yk - yi) * travelB / lenB + yi;
    width = zB - zA;
    deltaZ = z - zA;
    if (width < 0.001f && -0.001f < width) {
        /* A collapsed span contributes no height displacement. */
        correction = 0.0f;
        height = yA + correction;
    } else {
        correction = (yB - yA) * deltaZ / width;
        height = correction + yA;
    }
    return height;
}

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

extern f32 fldCamPitch;

typedef struct {
    f32 distance;
    f32 eyeHeight;
    f32 targetHeight;
    f32 fovy;
} fldCamParamRow_t;

extern fldCamParamRow_t fldCamParamTable[];

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);

extern f32 fldCamDirLock;

extern f32 fldCamDist;

extern f32 fldCamHeight;

extern f32 fldCamParamTable2;

extern f32 D_0054ECA0;

extern f32 D_0054ECDC;

extern s32 fldHitChkMatter;

extern s32 fldHitChkSlope;

extern s32 fldHitChkCamera(const sdfVector4_t *eyepos, const sdfVector4_t *disppos,
                           sdfVector4_t *hitpos);

extern void fldCamSetLeng(void);

/* NON_MATCHING: 208 of 219 words differ, at 876 bytes against 884. */
#ifdef NON_MATCHING
void resetCamNormal(void) {
    sdfVector4_t eye;
    sdfVector4_t eyeBase;
    sdfVector4_t tgt;
    sdfVector4_t ta;
    sdfVector4_t tb;
    sdfVector4_t hitpos;
    sdfVector4_t pos;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    fldAreaWork_t *gb;
    dds3Basic_t *node;
    f32 angle;
    f32 ofs;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetRotPoseSet(unit->handle);
    angle = EulerAngleAxisY() * 180.0f / 3.14f;
    fldGlobalBase.camAngle = angle;
    fldGlobalBase.camAngle2 = angle;
    dds3GetPosPoseSet(unit->handle);
    SDF_VU0_STOREVECTOR(&pos);
    gb = &fldGlobalBase;
    gb->prevX = pos.x;
    gb->prevY = pos.y;
    gb->prevZ = pos.z;
    gb->x = pos.x;
    gb->y = pos.y;
    gb->z = pos.z;
    fldCamPitch = -gb->camAngle;
    D_0054ECDC = fldCamPitch;
    eye.x = gb->x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    eye.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].eyeHeight;
    eye.z = gb->z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    tgt.x = gb->x + sdfSin(fldCamPitch * 3.14f / 180.0f) *
        fldCamParamTable[gb->cameraParamIndex].distance;
    tgt.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].targetHeight;
    tgt.z = gb->z + sdfCos(fldCamPitch * 3.14f / 180.0f) *
        fldCamParamTable[gb->cameraParamIndex].distance;
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &tgt);
    fldCamDirLock = 0.0f;
    fldHitChkMatter = -1;
    fldHitChkSlope = -1;
    eyeBase.x = gb->x;
    eyeBase.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].eyeHeight;
    eyeBase.z = gb->z;
    ofs = fldCamDirLock;
    tgt.y = tgt.y + ofs;
    fldCamDist = ofs;
    SDF_VU0_COPYVECTOR(&ta, &eyeBase);
    SDF_VU0_COPYVECTOR(&tb, &tgt);
    fldHitChkCamera(&tb, &ta, &hitpos);
    dx = hitpos.x - eyeBase.x;
    dy = hitpos.y - eyeBase.y;
    dz = hitpos.z - eyeBase.z;
    len = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamHeight = len * (D_0054ECA0 / fldCamParamTable2);
    tgt.x = eyeBase.x - (eyeBase.x - hitpos.x) / len * fldCamHeight;
    tgt.y = eyeBase.y - (eyeBase.y - hitpos.y) / len * fldCamHeight - ofs;
    tgt.z = eyeBase.z - (eyeBase.z - hitpos.z) / len * fldCamHeight;
    dds3SetPosPoseSet(control->camera, &tgt);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &tgt);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamSetLeng();
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", resetCamNormal);
#endif

/* NON_MATCHING: 207 of 215 words differ, at 860 bytes against 864. */
#ifdef NON_MATCHING
void resetCamNormal2(void) {
    sdfVector4_t eye;
    sdfVector4_t eyeBase;
    sdfVector4_t tgt;
    sdfVector4_t ta;
    sdfVector4_t tb;
    sdfVector4_t hitpos;
    sdfVector4_t pos;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    fldAreaWork_t *gb;
    dds3Basic_t *node;
    f32 ofs;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetRotPoseSet(unit->handle);
    fldGlobalBase.camAngle = EulerAngleAxisY() * 180.0f / 3.14f;
    dds3GetPosPoseSet(unit->handle);
    SDF_VU0_STOREVECTOR(&pos);
    gb = &fldGlobalBase;
    gb->prevX = gb->x;
    gb->prevY = gb->y;
    gb->prevZ = gb->z;
    gb->x = pos.x;
    gb->y = pos.y;
    gb->z = pos.z;
    eye.x = gb->x -
        sdfSin((gb->camAngle + 180.0f) * 3.14f / 180.0f);
    eye.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].eyeHeight;
    eye.z = gb->z +
        sdfCos((gb->camAngle + 180.0f) * 3.14f / 180.0f);
    tgt.x = gb->x + sdfSin(fldCamPitch * 3.14f / 180.0f) *
        fldCamParamTable[gb->cameraParamIndex].distance;
    tgt.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].targetHeight;
    tgt.z = gb->z + sdfCos(fldCamPitch * 3.14f / 180.0f) *
        fldCamParamTable[gb->cameraParamIndex].distance;
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &tgt);
    eyeBase.x = gb->x;
    eyeBase.y = gb->y +
        fldCamParamTable[gb->cameraParamIndex].eyeHeight;
    eyeBase.z = gb->z;
    ofs = fldCamDist;
    fldCamDist = fldCamDirLock;
    tgt.y = tgt.y + (ofs + (fldCamDirLock - ofs) * 0.0625f);
    SDF_VU0_COPYVECTOR(&ta, &eyeBase);
    SDF_VU0_COPYVECTOR(&tb, &tgt);
    fldHitChkCamera(&tb, &ta, &hitpos);
    dx = hitpos.x - eyeBase.x;
    dy = hitpos.y - eyeBase.y;
    dz = hitpos.z - eyeBase.z;
    len = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamHeight = len * (D_0054ECA0 / fldCamParamTable2);
    tgt.x = eyeBase.x - (eyeBase.x - hitpos.x) / len * fldCamHeight;
    tgt.y = eyeBase.y - (eyeBase.y - hitpos.y) / len * fldCamHeight;
    tgt.z = eyeBase.z - (eyeBase.z - hitpos.z) / len * fldCamHeight;
    dds3SetPosPoseSet(control->camera, &tgt);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &tgt);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamSetLeng();
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", resetCamNormal2);
#endif

void fldCamGetPhokanParam(void) {
    f32 angle;

    fldCamPhokanEyeX[0] =
        fldGlobalBase.x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight;
    fldCamPhokanEyeX[2] =
        fldGlobalBase.z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[3] = 1.0f;
    angle = fldCamPitch * 3.14f / 180.0f;
    fldCamPhokanTgtX[0] = fldGlobalBase.x +
        sdfSin(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    fldCamPhokanTgtX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex].targetHeight;
    fldCamPhokanTgtX[2] = fldGlobalBase.z +
        sdfCos(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    fldCamPhokanTgtX[3] = 1.0f;
}

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
    dds3SetPosPoseSet(control->camera, (const sdfVector4_t *)&farPoint.q);
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

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, const sdfVector4_t *target);

extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

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
    dds3CameraObjectBasicGetCameraTM(control->camera, (const sdfVector4_t *)&eye);
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
    dds3SetPosPoseSet(control->camera, (const sdfVector4_t *)&target.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
}

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);

extern f32 fldCamPitch;

void resetCamTgtPlayer(void) {
    sdfVector4_t eye, target;
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
    eye.x = D_002FC9C0[0] + (fldGlobalBase.x - D_002FC9C0[0]);
    eye.y = D_002FC9C0[1] +
               (fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight - D_002FC9C0[1]);
    eye.z = D_002FC9C0[2] + (fldGlobalBase.z - D_002FC9C0[2]);
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    dds3GetTMPoseSet(unit->handle);
    target.w = 1.0f;
    target.x = fldGlobalBase.campos.x;
    target.y = fldGlobalBase.campos.y;
    target.z = fldGlobalBase.campos.z;
    dds3SetPosPoseSet(control->camera, &target);
    target.x = fldCamTarget[0] + (target.x - fldCamTarget[0]);
    target.y = fldCamTarget[1] + (target.y - fldCamTarget[1]);
    target.z = fldCamTarget[2] + (target.z - fldCamTarget[2]);
    dds3SetPosPoseSet(control->camera, &target);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    dx = eye.x - target.x;
    dy = eye.y - target.y;
    dz = eye.z - target.z;
    fldCamLength = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamPitch = GetSphericalPosMove(target.x, target.z, eye.x, eye.z) + 180.0f;
}

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
    dds3CameraObjectBasicGetCameraTM(control->camera, (const sdfVector4_t *)&eye);
    target.f[1] = fldCamTarget[1] + (fldGlobalBase.y - 200.0f - fldCamTarget[1]) * 0.0625f;
    target.f[0] = fldCamTarget[0] + (fldGlobalBase.x - fldCamTarget[0]) * 0.0625f;
    target.f[2] = fldCamTarget[2] + (fldGlobalBase.z - fldCamTarget[2]) * 0.0625f;
    dds3SetPosPoseSet(control->camera, (const sdfVector4_t *)&target.q);
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
    dds3SetScreenFovy(fldCamParamTable[state->cameraParamIndex].fovy * 3.14f / 180.0f);
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

