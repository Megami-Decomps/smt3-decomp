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


/* Shared pipe records: node stride is 0x20; PosTable.points is at +8. */
typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    s8 id;
    u8 action;
    u8 enabled;
    u8 mdl;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipePathNode {
    f32 position[3];
    u32 opaque0C;
    f32 direction[3];
    u32 opaque1C;
} tagPipePathNode;

typedef struct tagPipePosTable {
    sdfMemHandle_t *aHandle;
    s32 tSize;
    tagPipePathNode *points;
    u32 opaque0C;
} tagPipePosTable;

extern s32 offsetCount;
extern tagPipePosTable D_00528F58;

extern tagPipeObject *objMyData(void);
extern void getPosTranslate(tagPipeObject *object, f32 *position);
extern void vecSet(f32 *vector, f32 x, f32 y, f32 z);
extern void vecScale(f32 *vector, f32 scale);
extern void pipeCameraLerp(f32 *vector, f32 angle);

void PipeCameraAction(void)
{
    f32 difference[4];
    f32 forward[4];
    f32 up[4];
    f32 sum[4];
    f32 position[4];
    f32 vertical[4];
    tagPipeObject *object;
    s32 pathIndex;
    f32 ease;
    f32 scale;
    s32 index;

    pathIndex = offsetCount;
    object = objMyData();
    ease = 0.0f;
    if (object == NULL) {
        return;
    }

    getPosTranslate(object, position);

    /* The six XYZ vectors occupy native 0x10-byte stack slots. */
    difference[0] = position[0] - D_00528F58.points[pathIndex].position[0];
    difference[1] = position[1] - D_00528F58.points[pathIndex].position[1];
    difference[2] = position[2] - D_00528F58.points[pathIndex].position[2];
    vecSet(forward,
           D_00528F58.points[pathIndex + 10].position[0] -
               D_00528F58.points[pathIndex].position[0],
           D_00528F58.points[pathIndex + 10].position[1] -
               D_00528F58.points[pathIndex].position[1],
           D_00528F58.points[pathIndex + 10].position[2] -
               D_00528F58.points[pathIndex].position[2]);
    vecSet(up,
           D_00528F58.points[pathIndex].direction[0],
           D_00528F58.points[pathIndex].direction[1],
           D_00528F58.points[pathIndex].direction[2]);

    pipeCamShakeVec[0] = position[0];
    pipeCamShakeVec[1] = position[1];
    pipeCamShakeVec[2] = position[2];
    vecNormalize(forward);
    vecScale(forward, -600.0f);

    if (pathIndex < 480) {
        vecNormalize(up);
        vecScale(up, 400.0f);
        pipeCamShake[0] = D_00528F58.points[480].position[0] +
                          forward[0] + up[0];
        pipeCamShake[1] = D_00528F58.points[480].position[1] +
                          forward[1] + up[1];
        pipeCamShake[2] = D_00528F58.points[480].position[2] +
                          forward[2] + up[2];
    } else if (pathIndex < 680) {
        scale = (f32)(pathIndex - 480);
        ease = scale / 200.0f;
        ease = 1.0f - (1.0f - ease) * (1.0f - ease);
        vecNormalize(up);
        vecScale(up, 400.0f - ease * 500.0f);
        sum[0] = forward[0] + up[0];
        sum[1] = forward[1] + up[1];
        sum[2] = forward[2] + up[2];
        index = (s32)(ease * scale + 480.0f);
        pipeCamShake[0] = D_00528F58.points[index].position[0] + sum[0];
        pipeCamShake[1] = D_00528F58.points[index].position[1] + sum[1];
        pipeCamShake[2] = D_00528F58.points[index].position[2] + sum[2];
    } else if (pathIndex < 800) {
        f32 radius;
        f32 target;
        f32 y;
        f32 limit;

        ease = (f32)(pathIndex - 680) / 120.0f;
        radius = fsqrtf(250000.0f - object->pos[0] * object->pos[0]);
        target = -100.0f - ease * -50.0f;
        y = object->pos[1] + target;
        limit = radius + 50.0f;
        if (y < 0.0f) {
            if (limit < -y) {
                goto constrainOffset;
            }
        } else if (limit < y) {
            goto constrainOffset;
        }
        goto offsetReady;
constrainOffset:
        /* Reuse the vertical distance for the boundary correction. Qualified
         * 2.96 allocation gives its 10-reference/live-13 pseudo f0 before the
         * limit's 3/9 pseudo f1; splitting the correction changes four words. */
        y = radius + 50.0f;
        if (object->pos[1] < 0.0f) {
            y = y + object->pos[1];
        } else {
            y = y - object->pos[1];
        }
        target = -y;
offsetReady:
        vecNormalize(up);
        vecScale(up, target);
        pipeCamShake[0] = position[0] + forward[0] + up[0];
        pipeCamShake[1] = position[1] + forward[1] + up[1];
        pipeCamShake[2] = position[2] + forward[2] + up[2];
    }

    if (pathIndex < 680) {
        vertical[0] = 0.0f;
        vertical[1] = 1.0f;
        vertical[2] = 0.0f;
        pipeCameraLerp(vertical, ease * -1.5707963f);
        pipeCameraPos[0] = vertical[0];
        pipeCameraPos[1] = vertical[1];
        pipeCameraPos[2] = vertical[2];
    }
}

void PipeCameraAction2(void)
{
    f32 difference[4];
    f32 forward[4];
    f32 up[4];
    f32 sum[4];
    f32 position[4];
    tagPipeObject *object;
    f32 blend;
    s32 offset;

    object = objMyData();
    blend = 0.0f;
    if (object == NULL) {
        return;
    }
    getPosTranslate(object, position);
    difference[0] = position[0] - D_00528F58.points[offsetCount].position[0];
    difference[1] = position[1] - D_00528F58.points[offsetCount].position[1];
    difference[2] = position[2] - D_00528F58.points[offsetCount].position[2];
    vecSet(forward,
           D_00528F58.points[offsetCount + 10].position[0] - D_00528F58.points[offsetCount].position[0],
           D_00528F58.points[offsetCount + 10].position[1] - D_00528F58.points[offsetCount].position[1],
           D_00528F58.points[offsetCount + 10].position[2] - D_00528F58.points[offsetCount].position[2]);
    vecSet(up, D_00528F58.points[offsetCount].direction[0],
           D_00528F58.points[offsetCount].direction[1], D_00528F58.points[offsetCount].direction[2]);
    vecNormalize(forward);
    vecScale(forward, -600.0f);

    offset = offsetCount;
    if (offset < 340) {
        pipeCamShakeVec[0] = D_00528F58.points[offset].position[0];
        pipeCamShakeVec[1] = D_00528F58.points[offset].position[1] - 100.0f;
        pipeCamShakeVec[2] = D_00528F58.points[offset].position[2];
        vecNormalize(up);
        vecScale(up, 600.0f);
        pipeCamShake[0] = D_00528F58.points[offsetCount].position[0] + up[0];
        pipeCamShake[1] = D_00528F58.points[offsetCount].position[1] + up[1];
        pipeCamShake[2] = D_00528F58.points[offsetCount].position[2] + up[2];
        pipeCameraPos[0] = blend;
        pipeCameraPos[1] = 1.0f;
        pipeCameraPos[2] = blend;
    } else if (offset < 800) {
        blend = (f32)(offset - 340) / 460.0f;
        pipeCamShakeVec[0] = D_00528F58.points[offset].position[0] + difference[0] * blend;
        pipeCamShakeVec[1] = (D_00528F58.points[offset].position[1] - 100.0f) +
                              (difference[1] + 100.0f) * blend;
        pipeCamShakeVec[2] = D_00528F58.points[offset].position[2] + difference[2] * blend;
        vecNormalize(up);
        vecScale(up, 600.0f - blend * 650.0f);
        sum[0] = (D_00528F58.points[offsetCount].position[0] + up[0]) + forward[0] * blend;
        sum[1] = (D_00528F58.points[offsetCount].position[1] + up[1]) + forward[1] * blend;
        sum[2] = (D_00528F58.points[offsetCount].position[2] + up[2]) + forward[2] * blend;
        pipeCamShake[0] = sum[0];
        pipeCamShake[1] = sum[1];
        pipeCamShake[2] = sum[2];
        pipeCameraPos[0] = D_00528F58.points[offsetCount].direction[0] * blend;
        pipeCameraPos[1] = (D_00528F58.points[offsetCount].direction[1] + -1.0f) * blend + 1.0f;
        pipeCameraPos[2] = D_00528F58.points[offsetCount].direction[2] * blend;
    }
}


void PipeCameraAction3(void)
{
    f32 difference[4];
    f32 forward[4];
    f32 up[4];
    /* Retained from PipeCameraAction's six-vector layout. This variant no
     * longer writes sum, but 2.96 reserves its 16-byte slot. Removing the
     * declaration shifts position, vertical and the saved registers by 16. */
    f32 sum[4];
    f32 position[4];
    f32 vertical[4];
    tagPipeObject *object;
    s32 pathIndex;
    f32 ease;
    f32 scale;
    s32 index;

    pathIndex = offsetCount;
    object = objMyData();
    ease = 0.0f;
    if (object == NULL) {
        return;
    }

    getPosTranslate(object, position);

    /* The six XYZ vectors occupy native 0x10-byte stack slots. */
    difference[0] = position[0] - D_00528F58.points[pathIndex].position[0];
    difference[1] = position[1] - D_00528F58.points[pathIndex].position[1];
    difference[2] = position[2] - D_00528F58.points[pathIndex].position[2];
    vecSet(forward,
           D_00528F58.points[pathIndex + 10].position[0] -
               D_00528F58.points[pathIndex].position[0],
           D_00528F58.points[pathIndex + 10].position[1] -
               D_00528F58.points[pathIndex].position[1],
           D_00528F58.points[pathIndex + 10].position[2] -
               D_00528F58.points[pathIndex].position[2]);
    vecSet(up,
           D_00528F58.points[pathIndex].direction[0],
           D_00528F58.points[pathIndex].direction[1],
           D_00528F58.points[pathIndex].direction[2]);

    pipeCamShakeVec[0] = position[0];
    pipeCamShakeVec[1] = position[1];
    pipeCamShakeVec[2] = position[2];
    vecNormalize(forward);
    vecScale(forward, -600.0f);

    if (pathIndex < 480) {
        vecNormalize(up);
        vecScale(up, -400.0f);
        pipeCamShake[0] = D_00528F58.points[480].position[0] +
                          forward[0] + up[0];
        pipeCamShake[1] = D_00528F58.points[480].position[1] +
                          forward[1] + up[1];
        pipeCamShake[2] = D_00528F58.points[480].position[2] +
                          forward[2] + up[2];
    } else if (pathIndex < 680) {
        scale = (f32)(pathIndex - 480);
        ease = scale / 200.0f;
        ease = 1.0f - (1.0f - ease) * (1.0f - ease);
        vecNormalize(up);
        vecScale(up, -400.0f - ease * -300.0f);
        index = (s32)(ease * scale + 480.0f);
        pipeCamShake[0] = D_00528F58.points[index].position[0] + forward[0] + up[0];
        pipeCamShake[1] = D_00528F58.points[index].position[1] + forward[1] + up[1];
        pipeCamShake[2] = D_00528F58.points[index].position[2] + forward[2] + up[2];
    } else if (pathIndex < 800) {
        f32 radius;
        f32 target;
        f32 y;
        f32 limit;

        ease = (f32)(pathIndex - 680) / 120.0f;
        radius = fsqrtf(250000.0f - object->pos[0] * object->pos[0]);
        target = -100.0f - ease * -50.0f;
        y = object->pos[1] + target;
        limit = radius + 50.0f;
        if (y < 0.0f) {
            if (limit < -y) {
                goto constrainOffset;
            }
        } else if (limit < y) {
            goto constrainOffset;
        }
        goto offsetReady;
constrainOffset:
        /* As in PipeCameraAction, reuse the distance for the correction.
         * Qualified 2.96 allocation gives its 10-reference/live-13 pseudo f0
         * before the comparison limit's 3-reference/live-9 pseudo f1. */
        y = radius + 50.0f;
        if (object->pos[1] < 0.0f) {
            y = y + object->pos[1];
        } else {
            y = y - object->pos[1];
        }
        target = -y;
offsetReady:
        vecNormalize(up);
        vecScale(up, target);
        pipeCamShake[0] = position[0] + forward[0] + up[0];
        pipeCamShake[1] = position[1] + forward[1] + up[1];
        pipeCamShake[2] = position[2] + forward[2] + up[2];
    }

    if (pathIndex < 680) {
        vertical[0] = 0.0f;
        vertical[1] = -1.0f;
        vertical[2] = 0.0f;
        pipeCameraLerp(vertical, ease * 1.5707963f);
        pipeCameraPos[0] = vertical[0];
        pipeCameraPos[1] = vertical[1];
        pipeCameraPos[2] = vertical[2];
    }
}


typedef struct tagPipeQuakeWeights { s8 values[6]; } tagPipeQuakeWeights;
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

