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

extern f32 fldCamPitch;

typedef struct {
    f32 distance;
    f32 eyeHeight;
    f32 targetHeight;
    f32 fovy;
} fldCamParamRow_t;

extern fldCamParamRow_t fldCamParamTable[];
extern f32 fldCamDirLock;
extern s32 fldHitChkMatter;
extern s32 fldHitChkSlope;

extern collision_s *fldMatterTable;

#ifdef NON_MATCHING
void fldCamCalcSaka(void)
{
    s32 index = fldHitChkSlope;
    collision_s *collision;
    sdfVector3_t offset;
    f32 xs[3], ys[3], zs[3];
    f32 x, z;
    f32 pi;

    fldGlobalBase.float130 = 0.0f;
    fldCamDirLock = 0.0f;
    if (index == -1) {
        index = fldHitChkMatter;
        if (index < 0) return;
    }
    if (fldMatterTable == NULL) return;
    collision = &fldMatterTable[index];
    if (collision->matter != NULL) {
        offset.x = collision->matter->poseset->pos.x;
        offset.y = collision->matter->poseset->pos.y;
        offset.z = collision->matter->poseset->pos.z;
    } else {
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.0f;
    }
    pi = 3.14f;
    xs[0] = collision->max[0][0] + offset.x;
    ys[0] = collision->max[0][1] + offset.y;
    zs[0] = collision->max[0][2] + offset.z;
    xs[1] = collision->max[1][0] + offset.x;
    ys[1] = collision->max[1][1] + offset.y;
    zs[1] = collision->max[1][2] + offset.z;
    xs[2] = collision->max[2][0] + offset.x;
    ys[2] = collision->max[2][1] + offset.y;
    zs[2] = collision->max[2][2] + offset.z;
    x = fldGlobalBase.x - sdfSin(fldCamPitch * pi / 180.0f) *
        fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    z = fldGlobalBase.z - sdfCos(fldCamPitch * pi / 180.0f) *
        fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    fldCamDirLock = fldGlobalBase.y - fldCamCalcSlopeOffset(x, z, xs, ys, zs);
    x = fldGlobalBase.x - sdfSin(-fldGlobalBase.camAngle * pi / 180.0f) *
        fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    z = fldGlobalBase.z - sdfCos(-fldGlobalBase.camAngle * pi / 180.0f) *
        fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    fldGlobalBase.float130 = fldGlobalBase.y - fldCamCalcSlopeOffset(x, z, xs, ys, zs);
}
#else
INCLUDE_ASM(const s32, "field/fldCamera", fldCamCalcSaka);
#endif

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);


extern f32 fldCamDist;

extern f32 fldCamHeight;

extern f32 fldCamParamTable2;

extern f32 D_0054ECA0;

extern f32 D_0054ECDC;


extern s32 fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

extern void fldCamSetLeng(void);

/* NON_MATCHING: 208 of 219 words differ, at 876 bytes against 884. */
#ifdef NON_MATCHING
void resetCamNormal(void) {
    sdfVector4_t eye;
    sdfVector4_t eyeBase;
    sdfVector4_t tgt;
    f32 ta[4] __attribute__((aligned(16)));
    f32 tb[4] __attribute__((aligned(16)));
    f32 hitpos[4] __attribute__((aligned(16)));
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
    fldHitChkCamera(tb, ta, hitpos);
    dx = hitpos[0] - eyeBase.x;
    dy = hitpos[1] - eyeBase.y;
    dz = hitpos[2] - eyeBase.z;
    len = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamHeight = len * (D_0054ECA0 / fldCamParamTable2);
    tgt.x = eyeBase.x - (eyeBase.x - hitpos[0]) / len * fldCamHeight;
    tgt.y = eyeBase.y - (eyeBase.y - hitpos[1]) / len * fldCamHeight - ofs;
    tgt.z = eyeBase.z - (eyeBase.z - hitpos[2]) / len * fldCamHeight;
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
    f32 ta[4] __attribute__((aligned(16)));
    f32 tb[4] __attribute__((aligned(16)));
    f32 hitpos[4] __attribute__((aligned(16)));
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
    fldHitChkCamera(tb, ta, hitpos);
    dx = hitpos[0] - eyeBase.x;
    dy = hitpos[1] - eyeBase.y;
    dz = hitpos[2] - eyeBase.z;
    len = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamHeight = len * (D_0054ECA0 / fldCamParamTable2);
    tgt.x = eyeBase.x - (eyeBase.x - hitpos[0]) / len * fldCamHeight;
    tgt.y = eyeBase.y - (eyeBase.y - hitpos[1]) / len * fldCamHeight;
    tgt.z = eyeBase.z - (eyeBase.z - hitpos[2]) / len * fldCamHeight;
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

extern f32 R360_LR(f32 a, f32 b);
extern s32 fldEncountTestFlag(u32 flag);
extern s8 dds3PadButtonState[];
extern s32 D_0054ECB0;
extern s32 l_UseCollision;
extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, const sdfVector4_t *target);
extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

/* DDS player-follow camera reconstruction adapted to SMT3 native geometry,
 * smoothing and collision behavior; see docs/normal-camera-matching.md. */
void calcCamNormal(void) {
    sdfVector4_t nearPoint;
    sdfVector4_t referencePoint;
    sdfVector4_t farPoint;
    f32 referenceCopy[4] __attribute__((aligned(16)));
    f32 farCopy[4] __attribute__((aligned(16)));
    sdfVector4_t scaledPoint;
    f32 collisionPoint[4] __attribute__((aligned(16)));
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 heightOffset;
    f32 collisionLength;
    f32 ratio;
    f32 fx, fy, fz;
    f32 dx, dy, dz;
    f32 unitX, unitY, unitZ;
    f32 divisorX, divisorY, divisorZ;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetPosPoseSet(unit->handle);
    nearPoint.w = 1.0f;
    nearPoint.x = fldGlobalBase.x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    nearPoint.y = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight;
    nearPoint.z = fldGlobalBase.z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    R360_LR(fldCamPitch, D_0054ECDC);
    if (fldEncountTestFlag(0x40)) {
        if (dds3PadButtonState[0x23]) {
            D_0054ECB0 = 1;
        } else {
            D_0054ECB0 = 0;
        }
    }
    if (D_0054ECB0) {
        divisorX = 3.0f;
        divisorY = 4.0f;
    } else {
        divisorX = 4.0f;
        divisorY = 8.0f;
    }
    nearPoint.x = D_002FC9C0[0] + (nearPoint.x - D_002FC9C0[0]) / divisorX;
    nearPoint.y = D_002FC9C0[1] + (nearPoint.y - D_002FC9C0[1]) / divisorY;
    nearPoint.z = D_002FC9C0[2] + (nearPoint.z - D_002FC9C0[2]) / divisorX;
    dds3CameraObjectBasicGetCameraTM(control->camera, &nearPoint);
    referencePoint.x = fldGlobalBase.x;
    referencePoint.y = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight;
    referencePoint.z = fldGlobalBase.z;
    dds3GetTMPoseSet(unit->handle);
    angle = fldCamPitch * 3.14f / 180.0f;
    farPoint.w = 1.0f;
    fx = fldGlobalBase.x + sdfSin(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    farPoint.x = fx;
    fy = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].targetHeight;
    farPoint.y = fy;
    fz = fldGlobalBase.z + sdfCos(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    farPoint.z = fz;
    if (l_UseCollision) {
        heightOffset = fldCamDist + (fldCamDirLock - fldCamDist) * 0.0625f;
        fldCamDist = fldCamDirLock;
        fy = farPoint.y + heightOffset;
        farPoint.y = fy;
    } else {
        fldCamDist = fldCamDirLock = 0.0f;
    }
    dds3SetPosPoseSet(control->camera, &farPoint);
    SDF_VU0_COPYVECTOR(&referenceCopy, &referencePoint);
    SDF_VU0_COPYVECTOR(&farCopy, &farPoint);
    if (fldCamHeight > D_0054ECA0) {
        fldCamHeight -= 1000.0f;
        if (fldCamHeight < D_0054ECA0) {
            fldCamHeight = D_0054ECA0;
        }
    } else if (fldCamHeight < D_0054ECA0) {
        fldCamHeight += 1000.0f;
        if (D_0054ECA0 < fldCamHeight) {
            fldCamHeight = D_0054ECA0;
        }
    }
    /* Retail retains this scaled XYZ scratch at sp+0x50, separate from the
     * collision output at +0x60. Reload the published far point here; these
     * same values also feed the no-collision smoothing path below. */
    fx = farPoint.x;
    scaledPoint.x = nearPoint.x - fx * fldCamParamTable2;
    fy = farPoint.y;
    scaledPoint.y = nearPoint.y - fy * fldCamParamTable2;
    fz = farPoint.z;
    scaledPoint.z = nearPoint.z - fz * fldCamParamTable2;
    if (l_UseCollision) {
        fldHitChkCamera(farCopy, referenceCopy,
                        collisionPoint);
        ratio = D_0054ECA0 / fldCamParamTable2;
        dx = collisionPoint[0] - referencePoint.x;
        dy = collisionPoint[1] - referencePoint.y;
        dz = collisionPoint[2] - referencePoint.z;
        collisionLength = fsqrtf(dx * dx + dy * dy + dz * dz);
        unitX = (referencePoint.x - collisionPoint[0]) / collisionLength;
        unitY = (referencePoint.y - collisionPoint[1]) / collisionLength;
        unitZ = (referencePoint.z - collisionPoint[2]) / collisionLength;
        if (collisionLength * ratio < fldCamHeight) {
            fldCamHeight = collisionLength * ratio;
        }
        fx = referencePoint.x - unitX * fldCamHeight;
        fy = referencePoint.y - unitY * fldCamHeight;
        fz = referencePoint.z - unitZ * fldCamHeight;
        farPoint.x = fx;
        farPoint.y = fy;
        farPoint.z = fz;
    }
    if (D_0054ECB0) {
        divisorX = 3.0f;
        divisorY = 4.0f;
        divisorZ = 3.0f;
    } else {
        divisorX = 8.0f;
        divisorY = 8.0f;
        divisorZ = 8.0f;
    }
    fx = fldCamTarget[0] + (fx - fldCamTarget[0]) / divisorX;
    farPoint.x = fx;
    fy = fldCamTarget[1] + (fy - fldCamTarget[1]) / divisorY;
    farPoint.y = fy;
    fz = fldCamTarget[2] + (fz - fldCamTarget[2]) / divisorZ;
    farPoint.z = fz;
    dds3SetPosPoseSet(control->camera, &farPoint);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamSetLeng();
}


extern f32 D_0054EC88;

/* Native direction-locked sibling of calcCamNormal; see
 * docs/dir-lock-camera-matching.md for its retained reference-vector stores. */
void calcCamDirLock(void) {
    sdfVector4_t nearPoint;
    sdfVector4_t referencePoint;
    sdfVector4_t farPoint;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 heightOffset;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetPosPoseSet(unit->handle);
    nearPoint.w = 1.0f;
    nearPoint.x = fldGlobalBase.x;
    nearPoint.y = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight;
    nearPoint.z = fldGlobalBase.z;
    nearPoint.x = D_002FC9C0[0] + (nearPoint.x - D_002FC9C0[0]) * 0.5f;
    nearPoint.y = D_002FC9C0[1] + (nearPoint.y - D_002FC9C0[1]) * 0.5f;
    nearPoint.z = D_002FC9C0[2] + (nearPoint.z - D_002FC9C0[2]) * 0.5f;
    dds3CameraObjectBasicGetCameraTM(control->camera, &nearPoint);
    /* Retail keeps the reference XYZ setup at +0x10 although this mode has
     * no collision query. calcCamNormal uses this same setup as a live base. */
    referencePoint.x = fldGlobalBase.x;
    referencePoint.y = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].eyeHeight;
    referencePoint.z = fldGlobalBase.z;
    dds3GetTMPoseSet(unit->handle);
    fldCamPitch = D_0054EC88;
    angle = fldCamPitch * 3.14f / 180.0f;
    farPoint.w = 1.0f;
    farPoint.x = fldGlobalBase.x + sdfSin(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    farPoint.y = fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex].targetHeight;
    farPoint.z = fldGlobalBase.z + sdfCos(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex].distance;
    if (l_UseCollision) {
        heightOffset = fldCamDist + (fldCamDirLock - fldCamDist) * 0.0625f;
        fldCamDist = fldCamDirLock;
        farPoint.y += heightOffset;
    } else {
        fldCamDist = fldCamDirLock = 0.0f;
    }
    dds3SetPosPoseSet(control->camera, &farPoint);
    farPoint.x = fldCamTarget[0] + (farPoint.x - fldCamTarget[0]) * 0.5f;
    farPoint.y = fldCamTarget[1] + (farPoint.y - fldCamTarget[1]) * 0.5f;
    farPoint.z = fldCamTarget[2] + (farPoint.z - fldCamTarget[2]) * 0.5f;
    dds3SetPosPoseSet(control->camera, &farPoint);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamSetLeng();
}


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

/* Existing camera-vector storage and construction/smoothing phases; see
 * docs/camera-dispatch-matching.md for the Chase/Main family qualification. */
void fldCamUpdateChase(void) {
    sdfVector4_t eye, target;
    fldFollowCameraControl_t *control;
    dds3Basic_t *node;

    control = fldGetFollowCameraControl();
    memset(&eye, 0, sizeof(eye));
    eye.w = 1.0f;
    memset(&target, 0, sizeof(target));
    target.w = 1.0f;
    D_0054ECB4 = (f32)(dds3PadStickState[0][0][0] - 0x80) * 0.7f + fldCamPitch;
    D_0054ECB8 = -((f32)(dds3PadStickState[0][0][1] - 0x80) * 2.5f);
    eye.x = fldGlobalBase.x - sdfSin(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.y = fldGlobalBase.y - 200.0f + D_0054ECB8;
    eye.z = fldGlobalBase.z - sdfCos(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.x = D_002FC9C0[0] + (eye.x - D_002FC9C0[0]) * 0.0625f;
    eye.y = D_002FC9C0[1] + (eye.y - D_002FC9C0[1]) * 0.0625f;
    eye.z = D_002FC9C0[2] + (eye.z - D_002FC9C0[2]) * 0.0625f;
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    target.x = fldGlobalBase.x;
    target.y = fldGlobalBase.y - 200.0f;
    target.z = fldGlobalBase.z;
    target.x = fldCamTarget[0] + (target.x - fldCamTarget[0]) * 0.0625f;
    target.y = fldCamTarget[1] + (target.y - fldCamTarget[1]) * 0.0625f;
    target.z = fldCamTarget[2] + (target.z - fldCamTarget[2]) * 0.0625f;
    dds3SetPosPoseSet(control->camera, &target);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
}

dds3Basic_t *fldCamNowEveCamera(void) {
    dds3Basic_t *worldObject = dds3GetActiveScene();
    dds3Basic_t *currentEntry = dds3GetActiveCameraSceneBasic(worldObject);

    if (fldGetUnitFollowCamera() == currentEntry) {
        currentEntry = NULL;
    }
    return currentEntry;
}

/* First-person sibling of the normal follow camera. See
 * docs/first-person-camera-matching.md for native geometry and storage. */
void calcCamSyukan(void) {
    sdfVector4_t nearPoint;
    sdfVector4_t referencePoint;
    sdfVector4_t farPoint;
    f32 referenceCopy[4] __attribute__((aligned(16)));
    f32 farCopy[4] __attribute__((aligned(16)));
    sdfVector4_t scaledPoint;
    f32 collisionPoint[4] __attribute__((aligned(16)));
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 heightOffset;
    f32 supportHeight;
    f32 collisionLength;
    f32 ratio;
    f32 fx, fy, fz;
    f32 dx, dy, dz;
    f32 unitX, unitY, unitZ;
    f32 divisorX, divisorY, divisorZ;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    if (fldGlobalBase.wordF8) {
        divisorX = 1.0f;
        divisorY = 1.0f;
        divisorZ = 1.0f;
    } else {
        divisorX = 2.0f;
        divisorY = 4.0f;
        divisorZ = 2.0f;
    }
    fldGlobalBase.wordF8 = 0;
    dds3GetPosPoseSet(unit->handle);
    nearPoint.w = 1.0f;
    nearPoint.x = fldGlobalBase.x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 30.0f;
    nearPoint.y = fldGlobalBase.y - 140.0f;
    nearPoint.z = fldGlobalBase.z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 30.0f;
    nearPoint.x = D_002FC9C0[0] + (nearPoint.x - D_002FC9C0[0]) / divisorX;
    nearPoint.y = D_002FC9C0[1] + (nearPoint.y - D_002FC9C0[1]) / divisorY;
    nearPoint.z = D_002FC9C0[2] + (nearPoint.z - D_002FC9C0[2]) / divisorZ;
    dds3CameraObjectBasicGetCameraTM(control->camera, &nearPoint);
    referencePoint.x = fldGlobalBase.x;
    referencePoint.y = fldGlobalBase.y - 140.0f;
    referencePoint.z = fldGlobalBase.z;
    dds3GetTMPoseSet(unit->handle);
    angle = fldCamPitch * 3.14f / 180.0f;
    farPoint.w = 1.0f;
    fx = fldGlobalBase.x + sdfSin(angle) * 100.0f;
    farPoint.x = fx;
    fy = fldGlobalBase.y - 140.0f;
    farPoint.y = fy;
    fz = fldGlobalBase.z + sdfCos(angle) * 100.0f;
    farPoint.z = fz;
    if (l_UseCollision) {
        supportHeight = fldCamDirLock / 5.0f;
        heightOffset = fldCamDist + (supportHeight - fldCamDist) * 0.0625f;
        fldCamDist = supportHeight;
        fy = farPoint.y + heightOffset;
        farPoint.y = fy;
    } else {
        fldCamDist = fldCamDirLock = 0.0f;
    }
    dds3SetPosPoseSet(control->camera, &farPoint);
    SDF_VU0_COPYVECTOR(&referenceCopy, &referencePoint);
    SDF_VU0_COPYVECTOR(&farCopy, &farPoint);
    if (fldCamHeight > D_0054ECA0) {
        fldCamHeight -= 1000.0f;
        if (fldCamHeight < D_0054ECA0) {
            fldCamHeight = D_0054ECA0;
        }
    } else if (fldCamHeight < D_0054ECA0) {
        fldCamHeight += 1000.0f;
        if (D_0054ECA0 < fldCamHeight) {
            fldCamHeight = D_0054ECA0;
        }
    }
    /* Retail retains this scaled XYZ scratch at sp+0x50, separate from the
     * collision output at +0x60. Reload the published far point here; these
     * same values also feed the no-collision smoothing path below. */
    fx = farPoint.x;
    scaledPoint.x = nearPoint.x - fx * fldCamParamTable2;
    fy = farPoint.y;
    scaledPoint.y = nearPoint.y - fy * fldCamParamTable2;
    fz = farPoint.z;
    scaledPoint.z = nearPoint.z - fz * fldCamParamTable2;
    if (l_UseCollision) {
        fldHitChkCamera(farCopy, referenceCopy,
                        collisionPoint);
        ratio = D_0054ECA0 / fldCamParamTable2;
        dx = collisionPoint[0] - referencePoint.x;
        dy = collisionPoint[1] - referencePoint.y;
        dz = collisionPoint[2] - referencePoint.z;
        collisionLength = fsqrtf(dx * dx + dy * dy + dz * dz);
        unitX = (referencePoint.x - collisionPoint[0]) / collisionLength;
        unitY = (referencePoint.y - collisionPoint[1]) / collisionLength;
        unitZ = (referencePoint.z - collisionPoint[2]) / collisionLength;
        if (collisionLength * ratio < fldCamHeight) {
            fldCamHeight = collisionLength * ratio;
        }
        fx = referencePoint.x - unitX * fldCamHeight;
        fy = referencePoint.y - unitY * fldCamHeight;
        fz = referencePoint.z - unitZ * fldCamHeight;
        farPoint.x = fx;
        farPoint.y = fy;
        farPoint.z = fz;
    }
    fx = fldCamTarget[0] + (fx - fldCamTarget[0]) / divisorX;
    farPoint.x = fx;
    fy = fldCamTarget[1] + (fy - fldCamTarget[1]) / divisorY;
    farPoint.y = fy;
    fz = fldCamTarget[2] + (fz - fldCamTarget[2]) / divisorZ;
    farPoint.z = fz;
    dds3SetPosPoseSet(control->camera, &farPoint);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamLength = 0.0f;
}


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

extern void fldPlayerAlpOn(void);
extern void fldPlayerSvsOn(void);
extern void fldPlayerAlpOff(void);
extern void fldPlayerSdwOff(void);
extern void fldPlayerSvsOff(void);
extern void fldPlayerOff(void);
extern void fldCamCalcSaka(void);
extern void fldCamUpdateChase(void);
extern void fldEventUpdateEx_Unity_2(void);
extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);
extern char D_00531BF0[];
extern s32 D_0054EC94;
extern f32 D_0054EC98;
extern f32 D_0054EC9C;

/* Six-mode camera dispatcher; native returns NULL on every path. */
INCLUDE_RODATA(const s32, "field/fldCamera", D_00531BF0);

void *fldCamMain(void) {
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    s32 left = 0;
    s32 right = 0;
    s32 up = 0;
    s32 down = 0;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    if (control->camera == NULL || unit->handle == NULL) {
        return NULL;
    }
    fldPlayerAlpOn();
    if (evtBitCheck(0xAF0)) {
        if (!fldCamNowEveCamera()) {
            if (fldGlobalBase.area >= 11) {
                if (fldGlobalBase.mode == 0) {
                    fldPlayerSvsOn();
                    fldPlayerAlpOff();
                    fldPlayerSdwOff();
                }
            }
        }
    }
    if (fldCamNowEveCamera()) {
        fldGlobalBase.wordF8 = 1;
        fldCamElvTask();
        fldPlayerSvsOff();
        return NULL;
    }
    if (fldGlobalBase.area == 18 || fldGlobalBase.area == 28 || fldGlobalBase.area == 30) {
        fldGlobalBase.cameraParamIndex = 1;
    }
    if (fldGlobalBase.area >= 11 && fldEncountTestFlag(0x40)) {
        if (dds3PadStickState[0][0][0] < 0x40) {
            left = 1;
        }
        if (dds3PadStickState[0][0][0] > 0xC0) {
            right = 1;
        }
        if (dds3PadStickState[0][0][1] < 0x40) {
            down = 1;
        }
        if (dds3PadStickState[0][0][1] > 0xC0) {
            up = 1;
        }
    }
    if (fldGlobalBase.area < 10) {
        fldCamUpdateDefault();
        return NULL;
    }
    fldCamCalcSaka();
    if (fldNpcCamTarget) {
        fldGlobalBase.wordF8 = 1;
        if (fldNpcCamTarget == 2) {
            dds3CameraObjectBasicSetFovy(control->camera, 50.0f * 3.14f / 180.0f);
        } else {
            dds3CameraObjectBasicSetFovy(control->camera, 40.0f * 3.14f / 180.0f);
        }
    } else if (fldGlobalBase.mode == 0 && evtBitCheck(0xAF0)) {
        if (scrSearchScriptName(D_00531BF0)) {
            dds3CameraObjectBasicSetFovy(control->camera,
                fldCamParamTable[fldGlobalBase.cameraParamIndex].fovy * 3.14f / 180.0f);
        } else {
            dds3CameraObjectBasicSetFovy(control->camera, 60.0f * 3.14f / 180.0f);
        }
    } else {
        dds3CameraObjectBasicSetFovy(control->camera,
            fldCamParamTable[fldGlobalBase.cameraParamIndex].fovy * 3.14f / 180.0f);
    }
    if (fldGlobalBase.mode) {
        fldCamMiwatasi = 0;
        D_0054EC94 = 0;
        D_0054EC98 = 0.0f;
        D_0054EC9C = 0.0f;
    }
    switch (fldGlobalBase.mode) {
    case 0:
        if (fldNpcCamTarget) {
            fldPlayerOff();
        }
        if (left || right || down || up) {
            fldCamUpdateChase();
            break;
        }
        if (fldNpcCamTarget) {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn == 0) {
                dds3FadeIn(1, 0, 0, 6);
            }
            calcCamNormalNPC();
        } else {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn) {
                dds3FadeIn(2, 0, 0, 6);
            }
            if (evtBitCheck(0xAF0)) {
                calcCamSyukan();
            } else {
                calcCamNormal();
            }
            fldCamResetOn = 0;
        }
        break;
    case 3:
        if (fldNpcCamTarget) {
            fldPlayerOff();
        }
        if (fldNpcCamTarget) {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn == 0) {
                dds3FadeIn(3, 0, 0, 6);
            }
            calcCamNormalNPC();
        } else {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn) {
                dds3FadeIn(4, 0, 0, 6);
            }
            calcCamDirLock();
            fldCamResetOn = 0;
        }
        break;
    case 1:
        if (fldNpcCamTarget) {
            fldPlayerOff();
        }
        if (fldNpcCamTarget) {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn == 0) {
                dds3FadeIn(5, 0, 0, 6);
            }
            calcCamNormalNPC();
        } else {
            if (fldGlobalBase.wordCC == 0 && fldCamResetOn) {
                dds3FadeIn(6, 0, 0, 6);
            }
            fldEventUpdateEx_Unity_2();
            fldCamResetOn = 0;
        }
        break;
    case 4:
        fldCamUpdateFollow();
        break;
    case 5:
        fldPlayerOff();
        calcCamNormalNPC();
        break;
    }
    return NULL;
}


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

