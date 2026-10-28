#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"


extern f32 pipeCamShakeVec[], pipeCamShake[], pipeCameraPos[];

extern dds3Basic_t *pipe_Camera;

extern void (*D_00551828)(void);

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 id, f32 *position, f32 *eye, f32 *up);

extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, f32 *eye);

extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 radius);

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern s32 pipeCheckLevel(s32 level, s32 type);

extern void dds3SetPosPoseSet(void *pBasic, f32 *pos);

extern void CalcCamera(void);

INCLUDE_ASM(const s32, "pipe/pipeCamera", getViewPortMatrix);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCamera);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction2);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction3);

INCLUDE_ASM(const s32, "pipe/pipeCamera", CamActionQuake);

void AddCamera(void) {
    dds3Basic_t *scene;
    dds3Basic_t *camera;
    f32 *eye = pipeCamShakeVec;

    camera = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), pipeCamShake, eye, pipeCameraPos);
    pipe_Camera = camera;
    dds3SetUnitObjectId(camera, "Pipe_Cam");
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
    SDF_VU0_COPYVECTOR(&cameraData->vertical, pipeCameraPos);
    SDF_VU0_COPYVECTOR(&cameraData->upVector, pipeCameraPos);
}

s32 pipeCheckLevel(s32 level, s32 type)
{
    s32 ret = 0;

    switch (type) {
    case 40:
        ret = 0;
        break;
    case 41:
        ret = level == 1;
        break;
    case 42:
        ret = level < 4;
        break;
    case 43:
        ret = level < 6;
        break;
    case 44:
        ret = level < 8;
        break;
    case 45:
        ret = 1;
        break;
    }
    return ret;
}
INCLUDE_SDATA(const s32, "pipe/pipeCamera", D_005517F0);

INCLUDE_SDATA(const s32, "pipe/pipeCamera", pipe_Camera);

INCLUDE_SDATA(const s32, "pipe/pipeCamera", D_005517FC);

