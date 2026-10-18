#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

typedef struct {
    /* 0x00 */ dds3Basic_t *fallbackModel;
    /* 0x04 */ sdfItemArray_t *itemList;
    /* 0x08 */ sdfMotionPlayBook_t *motionTable;
    /* 0x0C */ void *modelResource;
    /* 0x10 */ u8 unk10[8];
} dds3ModelBasicWork_t;

typedef struct {
    u32 unk00;
    void *resourceData;
    u8 unk08[8];
    s32 controlValue;
} dds3ModelRangeWork_t;


extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *block);
extern void *memset(void *destination, s32 value, u32 size);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 id, s32 type);
extern sdfMemHandle_t *sdfMemGetHandleByPtr(void *resource);
extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *handle);

s32 dds3AllocModelBasicWorkData(dds3Basic_t *basic) {
    basic->workdata = sdfChipAllocDirect(sizeof(dds3ModelBasicWork_t));
    memset(basic->workdata, 0, sizeof(dds3ModelBasicWork_t));
    return 1;
}

void dds3ModelBasicFreeModel(dds3Basic_t *basic) {
    dds3ModelRangeWork_t *work;
    void *resource;

    work = basic->workdata;
    resource = work->resourceData;
    if (resource != NULL) {
        sdfMemFreeDelayedDirect(sdfMemGetHandleByPtr(resource));
    }
    sdfChipFreeDirect(work);
}

dds3Basic_t *dds3AddModelRangeBasicNew(s32 id, s32 unused1, s32 unused2, char *name) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_MODELRANGE);

    basic->name = name;
    ((dds3ModelRangeWork_t *)basic->workdata)->controlValue = 0;
    basic->serialID = id;
    return basic;
}

void dds3SetPathBasicSpline(dds3Basic_t *basic, s32 spline) {
    ((dds3PathBasicControl_t *)basic->workdata)->spline = spline;
}

s32 dds3SearchSceneBasicWorkData(u32 id) {
    dds3Basic_t *basic;
    dds3Basic_t *scene;

    scene = dds3GetControlScene();
    basic = dds3SearchSceneBasic(scene, id, BASIC_TYPE_MATTEROBJECT);
    return ((dds3ModelRangeWork_t *)basic->workdata)->controlValue;
}
