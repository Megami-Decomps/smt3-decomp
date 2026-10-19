#include "common.h"
#include "basic.h"

typedef struct {
    /* 0x00 */ dds3ObjectBaseControlWorkData_t *control;
    /* 0x04 */ u32 hojiID;
    /* 0x08 */ u32 alphaMode;
    /* 0x0C */ s32 ot;
    /* 0x10 */ f32 offset[4];
    /* 0x20 */ f32 position[4];
} dds3MatterObjectBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *basic);

extern void dds3SetPosPoseSet(dds3Basic_t *basic, void *position);

extern void dds3SetRotPoseSet(dds3Basic_t *basic, void *rotation);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void sdfChipFreeDirect(void *block);

extern void *sdfChipAllocDirect(u32 size);

extern void *memset(void *destination, s32 value, u32 size);

extern void dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 id, s32 type);

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *basic) {
    const dds3MatterObjectBasicWorkData_t *work = basic->workdata;
    return work->control;
}

dds3Basic_t *dds3AddMatterObjectBasic(s32 id, f32 *offset, f32 *rotation) {
    f32 zero[4];
    dds3Basic_t *basic;

    memset(zero, 0, sizeof(zero));
    zero[3] = 1.0f;
    basic = dds3AddSceneBasicData(BASIC_TYPE_MATTEROBJECT);
    basic->serialID = id;
    dds3ObjectBaseCreateMover(basic);
    dds3SetRotPoseSet(basic, rotation);
    dds3SetPosPoseSet(basic, zero);
    dds3SetRadiusPoseSet(basic->poseset);
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->offset[0] = offset[0];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->offset[1] = offset[1];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->offset[2] = offset[2];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->offset[3] = offset[3];
    return basic;
}

void dds3DeleteMatterObjectBasic(dds3Basic_t *basic) {
    dds3DeleteSceneBasicData(basic);
}

void dds3MatterObjectBasicSetOt(dds3Basic_t *basic, s32 ot) {
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->ot = ot;
}

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *basic, u32 hojiID) {
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->hojiID = hojiID;
}

u32 dds3MatterObjectBasicGetHojiID(u32 id) {
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3MatterObjectBasicWorkData_t *work;

    scene = dds3GetControlScene();
    basic = dds3SearchSceneBasic(scene, id, BASIC_TYPE_MATTEROBJECT);
    work = basic->workdata;
    return work->hojiID;
}

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *basic, u32 alphaMode) {
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->alphaMode = alphaMode;
}

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *basic) {
    return ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->alphaMode;
}

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *basic) {
    dds3MatterObjectBasicWorkData_t *work;
    void *allocation;

    dds3InitPoseSet(basic);
    allocation = sdfChipAllocDirect(sizeof(dds3MatterObjectBasicWorkData_t));
    basic->workdata = allocation;
    memset(allocation, 0, sizeof(dds3MatterObjectBasicWorkData_t));
    work = basic->workdata;
    work->control = dds3ObjectBaseInitControlWorkData(basic);
    work->hojiID = 0;
    work->alphaMode = 0;
    work->ot = -1;
    work->offset[0] = 0.0f;
    work->offset[1] = 0.0f;
    work->offset[2] = 0.0f;
    work->offset[3] = 1.0f;
    work->position[0] = 0.0f;
    work->position[1] = 0.0f;
    work->position[2] = 0.0f;
    work->position[3] = 1.0f;
    dds3ObjectBaseOnFlag(basic, 0xE2);
    return 1;
}

void dds3MatterObjectBasicDestructor(dds3Basic_t *basic) {
    dds3MatterObjectBasicWorkData_t *work;

    dds3FreePoseSet(basic);
    work = basic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->control);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "basic/dds3MatterObjectBasic", dds3MatterObjectBasicMainProcedure);

INCLUDE_ASM(const s32, "basic/dds3MatterObjectBasic", dds3MatterObjectBasicDrawProcedure);
