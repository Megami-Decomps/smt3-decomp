#include "common.h"
#include "basic.h"

#include "ps2asm.h"

extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern s32 dds3InitPoseSet(dds3Basic_t *basic);
extern void dds3FreePoseSet(dds3Basic_t *basic);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);
extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);
extern void dds3SetPosPoseSet(dds3Basic_t *basic, const sdfVector4_t *position);
extern void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);
extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene);
extern void dds3SetScreenFovy(f32 fovy);
extern void ToMatrix(void);
extern void LookAtMatrix(const sdfVector4_t *eye, const sdfVector4_t *target,
                          const sdfVector4_t *up);
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewTarget;
extern sdfVector4_t aViewDown;
extern sdfVector4_t D_00530D38;
extern sdfVector4_t D_00530D48;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void dds3ObjectBaseCreateMover(dds3Basic_t *basic);
void MakeCamera(dds3Basic_t *basic);

s32 func_00112370(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;

    dds3InitPoseSet(basic);
    work = sdfChipAllocDirect(sizeof(dds3CameraObjectBasicWorkData_t));
    basic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(basic);
    work->pObjectBaseControlWorkData = control;
    dds3ObjectBaseOnFlag(basic, 0x62);
    work->flag = 0;
    return 1;
}

void func_001123D0(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work;

    dds3FreePoseSet(basic);
    work = basic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

/* Publish only the selected scene camera; consume its pending FOV update. */
s32 func_00112410(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;
    dds3CameraObjectBasicWorkData_t *work;

    pose = basic->poseset;
    work = basic->workdata;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        MakeCamera(basic);
        dds3SetRadiusPoseSet(pose);
    }
    if (dds3GetActiveCameraSceneBasic(dds3GetControlScene()) == basic) {
        SDF_VU0_COPYVECTOR(&aViewEye, &pose->pos);
        SDF_VU0_COPYVECTOR(&aViewTarget, &work->targetPos);
        SDF_VU0_COPYVECTOR(&aViewDown, &work->upVector);
        if (work->flag & 1) {
            dds3SetScreenFovy(work->fovy);
            work->flag &= ~1;
        }
    }
    return 1;
}

s32 func_001124F8(void)
{
    return 1;
}

/* Rotate the local vectors and cache their world-space look-at matrix. */
void MakeCamera(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;
    dds3PoseSet_t *pose = basic->poseset;

    SDF_VU0_LOADVECTOR(&pose->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&work->vertical);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&work->upVector);
    if (work->mode == 1) {
        SDF_VU0_LOADVECTOR(&work->eyeVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&pose->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&work->targetPos);
    }
    LookAtMatrix(&work->targetPos, &pose->pos, &work->upVector);
    SDF_VU0_STOREMATRIX(&work->cameraTM);
}

dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    return work->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AllocCameraObjectBasic(u32 serialID)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_CAMERAOBJECT);

    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *position,
                                           const sdfVector4_t *rotation)
{
    sdfVector4_t initialUp;
    sdfVector4_t initialEye;
    dds3Basic_t *basic;
    dds3CameraObjectBasicWorkData_t *work;

    initialUp = D_00530D38;
    initialEye = D_00530D48;
    basic = dds3AllocCameraObjectBasic(serialID);
    work = basic->workdata;
    work->mode = 1;
    work->fovy = 0.6283185f;
    dds3SetRotPoseSet(basic, rotation);
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, &initialUp);
    SDF_VU0_COPYVECTOR(&work->eyeVector, &initialEye);
    MakeCamera(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 serialID, const sdfVector4_t *position,
                                             const sdfVector4_t *target, const sdfVector4_t *up)
{
    dds3Basic_t *basic = dds3AllocCameraObjectBasic(serialID);
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = 0.6283185f;
    work->mode = 0;
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, up);
    SDF_VU0_COPYVECTOR(&work->targetPos, target);
    MakeCamera(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasic(u32 serialID, f32 fovy, const sdfVector4_t *position,
                                     const sdfVector4_t *eye, const sdfVector4_t *up, s32 mode)
{
    dds3Basic_t *basic = dds3AllocCameraObjectBasic(serialID);
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = fovy;
    work->mode = mode;
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, up);
    SDF_VU0_COPYVECTOR(&work->eyeVector, eye);
    if (mode == 0) {
        SDF_VU0_COPYVECTOR(&work->targetPos, eye);
    }
    MakeCamera(basic);
    return basic;
}

void dds3DeleteCameraObjectBasic(dds3Basic_t *basic)
{
    dds3DeleteSceneBasicData(basic);
}

/* vu0 routine: return the camera matrix in vf28-vf31. */
void func_00112888(dds3Basic_t *basic)
{
    SDF_VU0_LOADMATRIX(&((dds3CameraObjectBasicWorkData_t *)basic->workdata)->cameraTM);
}

/* The recovered name is misleading: retail stores a target, not a matrix. */
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, const sdfVector4_t *target)
{
    SDF_VU0_COPYVECTOR(&((dds3CameraObjectBasicWorkData_t *)basic->workdata)->targetPos, target);
}

/* vu0 routine: return the cached world-space target in vf10. */
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *basic)
{
    sdfVector4_t *target = &((dds3CameraObjectBasicWorkData_t *)basic->workdata)->targetPos;

    SDF_VU0_LOADVECTOR_BARRIER(target);
}

void dds3CameraObjectBasicSetFovy(dds3Basic_t *basic, f32 fovy)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = fovy;
    work->flag |= 1;
}

void func_001128F0(dds3Basic_t *basic, f32 fovy)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = fovy;
}

f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    return work->fovy;
}

/* vu0 routine: refresh the rotated vectors and copy target/eye to the caller. */
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *basic, sdfVector4_t *target, sdfVector4_t *eye)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;
    dds3PoseSet_t *pose = basic->poseset;

    SDF_VU0_LOADVECTOR(&pose->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&work->vertical);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&work->upVector);
    if (work->mode == 1) {
        SDF_VU0_LOADVECTOR(&work->eyeVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&pose->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&work->targetPos);
    }
    target->x = work->targetPos.x;
    target->y = work->targetPos.y;
    target->z = work->targetPos.z;
    target->w = work->targetPos.w;
    eye->x = pose->pos.x;
    eye->y = pose->pos.y;
    eye->z = pose->pos.z;
    eye->w = pose->pos.w;
}

u32 dds3CameraObjectBasicGetMode(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    return work->mode;
}

void func_00112A18(dds3Basic_t *basic, u32 mode)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    if (work->mode != mode) {
        work->mode = mode;
    }
}

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00530D38);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00530D48);

