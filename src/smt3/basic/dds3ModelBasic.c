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
} dds3ModelBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern sdfModelRootNode_t *sdfModelMake(void *resource, sdfItemArray_t *items);

extern sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *model, sdfMotionPlayBook_t *motion);

extern void sdfMotionChange(sdfMotionCtrlr_t *motion, s32 cut, s32 mode);

dds3Basic_t *dds3AddModelBasic(s32 id, sdfItemArray_t *items, sdfMotionPlayBook_t *motionTable, dds3Basic_t *fallbackModel, char *name) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    work->motionTable = motionTable;
    basic->serialID = id;
    basic->name = name;
    work->fallbackModel = fallbackModel;
    work->itemList = items;
    work->modelResource = NULL;
    return basic;
}

dds3Basic_t *dds3AddModelBasic2(s32 id, sdfItemArray_t *items, sdfMotionPlayBook_t *motionTable, void *modelResource, char *name) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    basic->serialID = id;
    work->modelResource = modelResource;
    work->itemList = items;
    work->motionTable = motionTable;
    basic->name = name;
    return basic;
}

sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic) {
    dds3ModelBasicWorkData_t *work = basic->workdata;
    sdfModelRootNode_t *model;

    if (work->modelResource != NULL) {
        model = sdfModelMake(work->modelResource, work->itemList);
    } else {
        model = sdfModelMake(work->fallbackModel->workdata, work->itemList);
    }
    return model;
}

sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model) {
    sdfMotionCtrlr_t *motion = NULL;
    dds3ModelBasicWorkData_t *work;

    if (model == NULL) {
        return NULL;
    }
    work = basic->workdata;
    if (work->motionTable != NULL) {
        motion = sdfMotionCtrlrNew(model, work->motionTable);
        sdfMotionChange(motion, 0, 1);
    }
    return motion;
}
