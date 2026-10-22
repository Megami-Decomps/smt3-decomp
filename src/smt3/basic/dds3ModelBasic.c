#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

typedef struct {
    /* 0x00 */ dds3Basic_t *pTextureBasic;
    /* 0x04 */ sdfItemArray_t *pModelDef;
    /* 0x08 */ sdfMotionPlayBook_t *pMotDef;
    /* 0x0C */ void *pTexList;
    /* 0x10 */ u8 unk10[8];
} dds3ModelBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfItemArray_t *aModelData);

extern sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook);

extern void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType);

dds3Basic_t *dds3AddModelBasic(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, dds3Basic_t *pTextureBasic, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    work->pMotDef = pMotDef;
    basic->serialID = serialID;
    basic->name = pName;
    work->pTextureBasic = pTextureBasic;
    work->pModelDef = pModelDef;
    work->pTexList = NULL;
    return basic;
}

dds3Basic_t *dds3AddModelBasic2(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, void *pTexList, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    basic->serialID = serialID;
    work->pTexList = pTexList;
    work->pModelDef = pModelDef;
    work->pMotDef = pMotDef;
    basic->name = pName;
    return basic;
}

sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic) {
    dds3ModelBasicWorkData_t *work = basic->workdata;
    sdfModelRootNode_t *model;

    if (work->pTexList != NULL) {
        model = sdfModelMake(work->pTexList, work->pModelDef);
    } else {
        model = sdfModelMake(work->pTextureBasic->workdata, work->pModelDef);
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
    if (work->pMotDef != NULL) {
        motion = sdfMotionCtrlrNew(model, work->pMotDef);
        sdfMotionChange(motion, 0, 1);
    }
    return motion;
}
