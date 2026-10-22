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

extern sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfItemArray_t *aModelData);

extern sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook);

extern void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType);

dds3Basic_t *dds3AddModelBasic(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, dds3Basic_t *pTextureBasic, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    work->motionTable = pMotDef;
    basic->serialID = serialID;
    basic->name = pName;
    work->fallbackModel = pTextureBasic;
    work->itemList = pModelDef;
    work->modelResource = NULL;
    return basic;
}

dds3Basic_t *dds3AddModelBasic2(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, void *pTexList, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    basic->serialID = serialID;
    work->modelResource = pTexList;
    work->itemList = pModelDef;
    work->motionTable = pMotDef;
    basic->name = pName;
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
