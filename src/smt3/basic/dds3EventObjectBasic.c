#include "common.h"
#include "basic.h"

/* The event constructor allocates a complete 16-byte payload. */
typedef struct
{
    dds3ObjectBaseControl_t *control;
    u8 unk04[12];
} dds3EventObjectWork_t;

extern s32 dds3InitPoseSet(dds3Basic_t *basic);
extern void dds3FreePoseSet(dds3Basic_t *basic);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern void dds3SetPosPoseSet(dds3Basic_t *basic, const sdfVector4_t *position);
extern void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);

u32 EventObjectBasicConstructor(dds3Basic_t *basic)
{
    dds3EventObjectWork_t *work;
    dds3ObjectBaseControl_t *control;

    dds3InitPoseSet(basic);
    work = sdfChipAllocDirect(sizeof(dds3EventObjectWork_t));
    basic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(basic);
    work->control = control;
    return 1;
}

void EventObjectBasicDestructor(dds3Basic_t *basic)
{
    dds3EventObjectWork_t *work;

    dds3FreePoseSet(basic);
    work = basic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->control);
    sdfChipFreeDirect(work);
}

s32 EventObjectBasicMainProcuder(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;

    pose = basic->poseset;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        dds3SetRadiusPoseSet(pose);
    }
    return 1;
}

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *position,
                                   const sdfVector4_t *rotation)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENTOBJECT);

    basic->serialID = serialID;
    dds3SetPosPoseSet(basic, position);
    dds3SetRotPoseSet(basic, rotation);
    dds3SetRadiusPoseSet(basic->poseset);
    return basic;
}

dds3ObjectBaseControl_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *basic)
{
    const dds3EventObjectWork_t *work = basic->workdata;

    return work->control;
}
