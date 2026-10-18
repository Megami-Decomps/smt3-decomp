#include "common.h"
#include "basic.h"
#include "ps2asm.h"

/* The matter object caches its path position at +0x20, independently of pose. */
typedef struct
{
    u8 unk00[0x20];
    sdfVector4_t position;
} dds3MatterPathPosition_t;

extern void dds3PathBasicControlGetPos(dds3PathBasicControl_t *work);
extern void dds3PathBasicControlGetRot(dds3PathBasicControl_t *work);
extern f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *work);
extern void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output);
extern s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work);
extern void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *res);
extern void dds3SetPosPoseSet(dds3Basic_t *basic, const sdfVector4_t *position);
extern void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);
extern void dds3BackupPoseSet(dds3PoseSet_t *pose);
extern void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);
extern void dds3AddRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);
extern void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

typedef struct dds3MoverBasicWork_t
{
    dds3Basic_t *object;
    dds3PathBasicControl_t *pathcontrol;
    dds3BasicControlProc_t primarycontrol;
    dds3Basic_t *pathbasic;
} dds3MoverBasicWork_t;

extern s32 dds3MoverCount;
extern char D_002FA670[10][16]; /* "MOVER00".."MOVER09" */

extern void *memset(void *s, s32 c, u32 n);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern u32 dds3AllocObjectId(void);
extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *object, dds3Basic_t *item, s32 itemType);
extern dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *pathBasic);
extern void dds3DeletePathControl(dds3PathBasicControl_t *control);

void dds3MoverBasicFreePath(dds3Basic_t *basic);
s32 MoverBasicMainprocuder(dds3Basic_t *basic);

s32 MoverBasicConstructor(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work;

    work = sdfChipAllocDirect(sizeof(dds3MoverBasicWork_t));
    basic->workdata = work;
    memset(work, 0, sizeof(dds3MoverBasicWork_t));
    return 1;
}

void MoverBasicDestructor(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work;

    work = basic->workdata;
    dds3MoverBasicFreePath(basic);
    dds3ObjectBaseSetControlBasicType(work->object, NULL, 1);
    sdfChipFreeDirect(work);
}

/* Paths supply absolute channels; a primary callback supplies relative pose. */
s32 MoverBasicMainprocuder(dds3Basic_t *basic)
{
    sdfVector4_t pathVector;
    fldResourceLightPath_t light;
    dds3PoseSet_t relativePose;
    dds3MoverBasicWork_t *work = basic->workdata;
    dds3Basic_t *object = work->object;
    dds3PoseSet_t *pose = object->poseset;
    dds3BasicControlProc_t callback;
    dds3CameraObjectBasicWorkData_t *camera;
    f32 fovy;

    if (work->pathcontrol != NULL) {
        if (work->pathcontrol->flags & DDS3_PATHBASIC_FLAG_POS) {
            dds3PathBasicControlGetPos(work->pathcontrol);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetPosPoseSet(object, &pathVector);
            if (object->type >> 24 == BASIC_TYPE_MATTEROBJECT) {
                SDF_VU0_COPYVECTOR(&((dds3MatterPathPosition_t *)object->workdata)->position, &pathVector);
            }
        }
        if (work->pathcontrol->flags & DDS3_PATHBASIC_FLAG_ROT) {
            dds3PathBasicControlGetRot(work->pathcontrol);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetRotPoseSet(object, &pathVector);
        }
        if (work->pathcontrol->flags & DDS3_PATHBASIC_FLAG_FOVY) {
            if (object->type >> 24 == BASIC_TYPE_CAMERAOBJECT) {
                fovy = dds3PathBasicControlGetFovy(work->pathcontrol);
                camera = object->workdata;
                camera->fovy = fovy;
                camera->flag |= 1;
            }
        }
        if (work->pathcontrol->flags & DDS3_PATHBASIC_FLAG_LIGHT) {
            if (object->type >> 24 == BASIC_TYPE_LIGHTOBJECT) {
                dds3PathBasicControlNextStep(work->pathcontrol, &light);
                dds3SetLightObjectPathResoruce(object, &light);
            }
        }
        dds3PathBasicControlIsFinished(work->pathcontrol);
    } else {
        callback = work->primarycontrol;
        pose->frameFall = 0;
        if (callback != NULL) {
            dds3BackupPoseSet(&relativePose);
            if (callback(&relativePose, object) == 1) {
                dds3GetSclPoseSetMCR(object, &relativePose.scl);
                dds3AddRotPoseSet(object, &relativePose.rot);
                dds3GetPosPoseSetMCR(object, &relativePose.pos);
            }
        }
    }
    return 1;
}

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *object)
{
    dds3Basic_t *basic;
    dds3MoverBasicWork_t *work;

    basic = dds3AddSceneBasicData(BASIC_TYPE_MOVER);
    work = basic->workdata;
    basic->serialID = dds3AllocObjectId();
    work->object = object;
    basic->name = D_002FA670[dds3MoverCount];
    dds3MoverCount++;
    dds3MoverCount %= 10;
    return basic;
}

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    work->primarycontrol = func;
}

void dds3MoverBasicSetPath(dds3Basic_t *basic, dds3Basic_t *pathBasic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    work->pathbasic = pathBasic;
}

void dds3MoverBasicMakePath(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    if (work->pathcontrol != NULL) {
        dds3DeletePathControl(work->pathcontrol);
    }
    work->pathcontrol = dds3AllocPathBasicWorkData(work->pathbasic);
}

void dds3MoverBasicFreePath(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    if (work->pathcontrol != NULL) {
        dds3DeletePathControl(work->pathcontrol);
        work->pathcontrol = NULL;
    }
}

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    return work->pathcontrol;
}

s32 dds3MoverBasicUpdate(dds3Basic_t *basic)
{
    return MoverBasicMainprocuder(basic);
}

u32 BasicTypeToItemType(u32 type)
{
    u32 itemType = 0;

    switch (type) {
    case BASIC_TYPE_MOVER:
        itemType = 1;
        break;
    case BASIC_TYPE_STATICMODEL:
    case BASIC_TYPE_EFFECT:
    case BASIC_TYPE_EVENT:
        itemType = 3;
        break;
    case BASIC_TYPE_MODELRANGE:
        itemType = 4;
        break;
    case BASIC_TYPE_AREA:
        itemType = 5;
        break;
    }
    return itemType;
}

INCLUDE_SDATA(const s32, "basic/dds3MoverBasic", dds3MoverCount);

INCLUDE_SDATA(const s32, "basic/dds3MoverBasic", D_0054E9CC);

