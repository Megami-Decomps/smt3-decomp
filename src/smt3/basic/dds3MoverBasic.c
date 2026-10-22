#include "common.h"
#include "basic.h"
#include "ps2asm.h"

/* The matter object caches its path position at +0x20, independently of pose. */
typedef struct
{
    u8 unk00[0x20];
    sdfVector4_t position;
} dds3MatterPathPosition_t;

extern void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);
extern void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);
extern f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl);
extern void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output);
extern s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work);
extern void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource);
extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *pos);
extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);
extern void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);
extern void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);
extern void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);
extern void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

typedef struct dds3MoverBasicWorkData_t
{
    dds3Basic_t *pObjectBasic;
    dds3PathBasicControl_t *pPathControl;
    dds3BasicControlProc_t pFunc;
    dds3Basic_t *pPathBasic;
} dds3MoverBasicWorkData_t;

extern s32 dds3MoverCount;
extern char D_002FA670[10][16]; /* "MOVER00".."MOVER09" */

extern void *memset(void *a, s32 b, u32 c);
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern u32 dds3AllocObjectId(void);
extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);
extern dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *pathBasic);
extern void dds3DeletePathControl(dds3PathBasicControl_t *control);

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);

s32 MoverBasicConstructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = sdfChipAllocDirect(sizeof(dds3MoverBasicWorkData_t));
    pBasic->workdata = work;
    memset(work, 0, sizeof(dds3MoverBasicWorkData_t));
    return 1;
}

void MoverBasicDestructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = pBasic->workdata;
    dds3MoverBasicFreePath(pBasic);
    dds3ObjectBaseSetControlBasicType(work->pObjectBasic, NULL, 1);
    sdfChipFreeDirect(work);
}

/* Paths supply absolute channels; a primary callback supplies relative pose. */
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic)
{
    sdfVector4_t pathVector;
    fldResourceLightPath_t light;
    dds3PoseSet_t relativePose;
    dds3MoverBasicWorkData_t *work = pBasic->workdata;
    dds3Basic_t *object = work->pObjectBasic;
    dds3PoseSet_t *pose = object->poseset;
    dds3BasicControlProc_t callback;
    dds3CameraObjectBasicWorkData_t *camera;
    f32 fovy;

    if (work->pPathControl != NULL) {
        if (work->pPathControl->flags & DDS3_PATHBASIC_FLAG_POS) {
            dds3PathBasicControlGetPos(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetPosPoseSet(object, &pathVector);
            if (object->type >> 24 == BASIC_TYPE_MATTEROBJECT) {
                SDF_VU0_COPYVECTOR(&((dds3MatterPathPosition_t *)object->workdata)->position, &pathVector);
            }
        }
        if (work->pPathControl->flags & DDS3_PATHBASIC_FLAG_ROT) {
            dds3PathBasicControlGetRot(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetRotPoseSet(object, &pathVector);
        }
        if (work->pPathControl->flags & DDS3_PATHBASIC_FLAG_FOVY) {
            if (object->type >> 24 == BASIC_TYPE_CAMERAOBJECT) {
                fovy = dds3PathBasicControlGetFovy(work->pPathControl);
                camera = object->workdata;
                camera->fovy = fovy;
                camera->flag |= 1;
            }
        }
        if (work->pPathControl->flags & DDS3_PATHBASIC_FLAG_LIGHT) {
            if (object->type >> 24 == BASIC_TYPE_LIGHTOBJECT) {
                dds3PathBasicControlNextStep(work->pPathControl, &light);
                dds3SetLightObjectPathResoruce(object, &light);
            }
        }
        dds3PathBasicControlIsFinished(work->pPathControl);
    } else {
        callback = work->pFunc;
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

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic)
{
    dds3Basic_t *basic;
    dds3MoverBasicWorkData_t *work;

    basic = dds3AddSceneBasicData(BASIC_TYPE_MOVER);
    work = basic->workdata;
    basic->serialID = dds3AllocObjectId();
    work->pObjectBasic = pObjectBasic;
    basic->name = D_002FA670[dds3MoverCount];
    dds3MoverCount++;
    dds3MoverCount %= 10;
    return basic;
}

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func)
{
    dds3MoverBasicWorkData_t *work = mover->workdata;

    work->pFunc = func;
}

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    work->pPathBasic = pPathBasic;
}

void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    if (work->pPathControl != NULL) {
        dds3DeletePathControl(work->pPathControl);
    }
    work->pPathControl = dds3AllocPathBasicWorkData(work->pPathBasic);
}

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    if (work->pPathControl != NULL) {
        dds3DeletePathControl(work->pPathControl);
        work->pPathControl = NULL;
    }
}

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    return work->pPathControl;
}

s32 dds3MoverBasicUpdate(dds3Basic_t *pBasic)
{
    return MoverBasicMainprocuder(pBasic);
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

