#include "common.h"
#include "basic.h"
#include "dds3obj.h"

/* The event constructor allocates a complete 16-byte payload. */
typedef struct
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u8 unk04[12];
} dds3EventObjectBasicWorkData_t;

extern s32 dds3InitPoseSet(dds3Basic_t *basic);
extern void dds3FreePoseSet(dds3Basic_t *basic);
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);



u32 EventObjectBasicConstructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;

    dds3InitPoseSet(pBasic);
    work = sdfChipAllocDirect(sizeof(dds3EventObjectBasicWorkData_t));
    pBasic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(pBasic);
    work->pObjectBaseControlWorkData = control;
    return 1;
}

void EventObjectBasicDestructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;

    dds3FreePoseSet(pBasic);
    work = pBasic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

s32 EventObjectBasicMainProcuder(dds3Basic_t *pBasic)
{
    dds3PoseSet_t *pose;

    pose = pBasic->poseset;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        dds3SetRadiusPoseSet(pose);
    }
    return 1;
}

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos,
                                   const sdfVector4_t *rot)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENTOBJECT);

    basic->serialID = serialID;
    dds3SetPosPoseSet(basic, pos);
    dds3SetRotPoseSet(basic, rot);
    dds3SetRadiusPoseSet(basic->poseset);
    return basic;
}

dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic)
{
    const dds3EventObjectBasicWorkData_t *work = pBasic->workdata;

    return work->pObjectBaseControlWorkData;
}
