#include "common.h"
#include "basic.h"

struct dds3ObjectBaseControlWorkData_t;

typedef struct dds3CameraObjectBasicWorkData_t
{
    sdfMatrix4_t cameraTM;
    sdfVector4_t eyeVector;
    sdfVector4_t vertical;
    sdfVector4_t targetPos;
    sdfVector4_t upVector;
    struct dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u32 mode;
    u32 flag;
    f32 fovy;
} dds3CameraObjectBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(u32 type);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void dds3ObjectBaseCreateMover(dds3Basic_t *basic);
void MakeCamera(dds3Basic_t *basic);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112370);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001123D0);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112410);

s32 func_001124F8(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", MakeCamera);

struct dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic)
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

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3AddCameraObjectBasicTypeFree);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3AddCameraObjectBasicTypeLookAt);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3AddCameraObjectBasic);

void dds3DeleteCameraObjectBasic(dds3Basic_t *basic)
{
    dds3DeleteSceneBasicData(basic);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112888);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3CameraObjectBasicGetCameraTM);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3CameraObjectBasicGetTargetPos);

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

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", dds3CameraObjectBasicGetTgtCam);

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

