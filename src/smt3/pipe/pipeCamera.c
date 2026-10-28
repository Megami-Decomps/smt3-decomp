#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"


/* The eye snapshot is read by aligned LD pairs at the native global base. */
extern f32 pipeCamShakeVec[] __attribute__((aligned(8)));
extern f32 pipeCamShake[], pipeCameraPos[];

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

extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
extern f32 vecNormalize(f32 *);
extern void vecTranslate(f32 *, f32 *, f32 *);
extern f32 vecDot(f32 *, f32 *);

void getViewPortMatrix(sdfMatrix4_t *matrix)
{
    sdfVector4_t right;
    sdfVector4_t up;
    sdfVector4_t direction;
    sdfVector4_t eye;

    memcpy(&eye, pipeCamShakeVec, sizeof(eye));
    direction.x = pipeCamShakeVec[0] - pipeCamShake[0];
    direction.y = pipeCamShakeVec[1] - pipeCamShake[1];
    direction.z = pipeCamShakeVec[2] - pipeCamShake[2];
    vecNormalize(&direction.x);

    vecTranslate(&right.x, pipeCameraPos, &direction.x);
    vecNormalize(&right.x);
    vecTranslate(&up.x, &direction.x, &right.x);
    vecNormalize(&up.x);

    memset(matrix, 0, sizeof(*matrix));
    matrix->m[0] = right.x;
    matrix->m[4] = right.y;
    matrix->m[8] = right.z;
    matrix->m[1] = up.x;
    matrix->m[5] = up.y;
    matrix->m[9] = up.z;
    matrix->m[2] = direction.x;
    matrix->m[6] = direction.y;
    matrix->m[10] = direction.z;
    matrix->m[12] = -vecDot(&right.x, &eye.x);
    matrix->m[13] = -vecDot(&up.x, &eye.x);
    matrix->m[14] = -vecDot(&direction.x, &eye.x);
    matrix->m[15] = 1.0f;
}

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCamera);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction2);

INCLUDE_ASM(const s32, "pipe/pipeCamera", PipeCameraAction3);

typedef struct tagPipeQuakeWeights { s8 values[6]; } tagPipeQuakeWeights;
typedef struct tagPipePathNode { f32 position[3]; u32 opaque0C; f32 direction[3]; u32 opaque1C; } tagPipePathNode;
typedef struct tagPipePosTable { sdfMemHandle_t *aHandle; s32 tSize; tagPipePathNode *points; u32 opaque0C; } tagPipePosTable;
extern s8 CameraAction;
extern s32 offsetCount;
extern tagPipePosTable D_00528F58;
extern void vecSet(f32 *, f32, f32, f32);
extern f32 vecNormalize(f32 *);
extern void vecScale(f32 *, f32);

void CamActionQuake(void)
{
    tagPipeQuakeWeights weights = {{30, -30, 20, -20, 10, -10}};
    f32 vector[3];

    if (CameraAction != 0) {
        /* Preserve the index-first EE address sum; see
         * docs/levers/addressing-and-types.md. */
        tagPipePathNode *node = (tagPipePathNode *)(
            (u32)offsetCount * sizeof(tagPipePathNode) + (u32)D_00528F58.points);
        s32 action;

        vecSet(vector, node->direction[0], node->direction[1], node->direction[2]);
        vecNormalize(vector);
        action = CameraAction;
        vecScale(vector, (f32)weights.values[6 - action]);
        CameraAction--;
        pipeCamShakeVec[0] += vector[0];
        pipeCamShakeVec[1] += vector[1];
        pipeCamShakeVec[2] += vector[2];
        pipeCamShake[0] += vector[0];
        pipeCamShake[1] += vector[1];
        pipeCamShake[2] += vector[2];
    }
}


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
INCLUDE_SDATA(const s32, "pipe/pipeCamera", pipe_Camera);

INCLUDE_SDATA(const s32, "pipe/pipeCamera", D_005517FC);

