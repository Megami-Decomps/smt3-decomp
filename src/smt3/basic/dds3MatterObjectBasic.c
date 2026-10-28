#include "common.h"
#include "basic.h"
#include "model.h"
#include "ps2asm.h"

typedef struct {
    /* 0x00 */ dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    /* 0x04 */ u32 hojiID;
    /* 0x08 */ u32 alpmode;
    /* 0x0C */ s32 OtLinkIdx;
    /* 0x10 */ f32 base_pos[4];
    /* 0x20 */ f32 move_pos[4];
} dds3MatterObjectBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, void *pos);

extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, void *rot);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *memset(void *a, s32 b, u32 c);

extern void dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void rsetOtOfs(s32 otofs);

extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                     s32 ztst);

extern void dds3ObjectBaseModelLinkOt(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic);

extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);

extern sdfModelOt_t D_002F69B8[];

extern sdfModelOt_t dds3MatterDrawParam;

extern dds3Basic_t *D_0054EAE8;

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern s32 fldDoor_Step(s32 matter_resid, f32 *x, f32 *y, f32 *z);

extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *pBasic) {
    const dds3MatterObjectBasicWorkData_t *work = pBasic->workdata;
    return work->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot) {
    f32 zero[4];
    dds3Basic_t *basic;

    memset(zero, 0, sizeof(zero));
    zero[3] = 1.0f;
    basic = dds3AddSceneBasicData(BASIC_TYPE_MATTEROBJECT);
    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    dds3SetRotPoseSet(basic, rot);
    dds3SetPosPoseSet(basic, zero);
    dds3SetRadiusPoseSet(basic->poseset);
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[0] = pos[0];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[1] = pos[1];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[2] = pos[2];
    ((dds3MatterObjectBasicWorkData_t *)basic->workdata)->base_pos[3] = pos[3];
    return basic;
}

void dds3DeleteMatterObjectBasic(dds3Basic_t *aPtr) {
    dds3DeleteSceneBasicData(aPtr);
}

void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->OtLinkIdx = idx;
}

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, u32 hojiID) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->hojiID = hojiID;
}

u32 dds3MatterObjectBasicGetHojiID(u32 resID) {
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3MatterObjectBasicWorkData_t *work;

    scene = dds3GetControlScene();
    basic = dds3SearchSceneBasic(scene, resID, BASIC_TYPE_MATTEROBJECT);
    work = basic->workdata;
    return work->hojiID;
}

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode) {
    ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode = alpmode;
}

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr) {
    return ((dds3MatterObjectBasicWorkData_t *)aPtr->workdata)->alpmode;
}

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;
    void *allocation;

    dds3InitPoseSet(aPtr);
    allocation = sdfChipAllocDirect(sizeof(dds3MatterObjectBasicWorkData_t));
    aPtr->workdata = allocation;
    memset(allocation, 0, sizeof(dds3MatterObjectBasicWorkData_t));
    work = aPtr->workdata;
    work->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    work->hojiID = 0;
    work->alpmode = 0;
    work->OtLinkIdx = -1;
    work->base_pos[0] = 0.0f;
    work->base_pos[1] = 0.0f;
    work->base_pos[2] = 0.0f;
    work->base_pos[3] = 1.0f;
    work->move_pos[0] = 0.0f;
    work->move_pos[1] = 0.0f;
    work->move_pos[2] = 0.0f;
    work->move_pos[3] = 1.0f;
    dds3ObjectBaseOnFlag(aPtr, 0xE2);
    return 1;
}

void dds3MatterObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;

    dds3FreePoseSet(aPtr);
    work = aPtr->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

s32 dds3MatterObjectBasicMainProcedure(dds3Basic_t *aPtr) {
    f32 x;
    f32 y;
    f32 z;
    dds3MatterObjectBasicWorkData_t *work = aPtr->workdata;
    sdfModelRootNode_t *model;
    s32 alpha;
    dds3PoseSet_t *pose;

    switch (work->alpmode) {
    case 0:
        aPtr->color = 0x80808080;
        break;
    case 1:
        aPtr->color = 0x80808080;
        break;
    case 2:
        aPtr->color = 0x00808080;
        break;
    case 3:
        alpha = *((u8 *)&aPtr->color + 3);
        if (alpha < 0x80) {
            alpha += 8;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    case 4:
        alpha = *((u8 *)&aPtr->color + 3);
        if (alpha > 0x40) {
            alpha -= 8;
        }
        if (alpha < 0x40) {
            alpha = 0x40;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    case 5:
        alpha = *((u8 *)&aPtr->color + 3);
        if (alpha < 0x80) {
            alpha += 4;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    case 6:
        alpha = *((u8 *)&aPtr->color + 3);
        alpha = (alpha != 0) ? alpha - 4 : 0;
        if (alpha < 0) {
            alpha = 0;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    case 7:
        alpha = *((u8 *)&aPtr->color + 3);
        if (alpha < 0x80) {
            alpha += 2;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    case 8:
        alpha = *((u8 *)&aPtr->color + 3);
        alpha = (alpha != 0) ? alpha - 2 : 0;
        if (alpha <= 0) {
            alpha = 0;
        }
        aPtr->color = (alpha << 24) | 0x808080;
        break;
    }
    model = dds3ObjectBaseGetModelHandle(aPtr);
    if (model != NULL) {
        dds3OffPoseSetFlag(aPtr->poseset, 1);
        dds3GetRotPoseSet(aPtr);
        SDF_VU0_STOREVECTOR(&model->rotate);
        dds3GetTMPoseSet(aPtr);
        dds3GetPosPoseSet(aPtr);
        if (fldDoor_Step(aPtr->serialID, &x, &y, &z) != 0) {
            pose = aPtr->poseset;
            pose->pos.x = work->base_pos[0] + work->move_pos[0] + x;
            pose->pos.y = work->base_pos[1] + work->move_pos[1] + y;
            pose->pos.z = work->base_pos[2] + work->move_pos[2] + z;
        } else {
            pose = aPtr->poseset;
            pose->pos.x = work->base_pos[0] + work->move_pos[0];
            pose->pos.y = work->base_pos[1] + work->move_pos[1];
            pose->pos.z = work->base_pos[2] + work->move_pos[2];
        }
        SDF_VU0_SET_ROW3();
        SDF_VU0_STOREMATRIX(&model->tm);
        dds3SetRadiusPoseSet(pose);
        sdfModelEvalTm(model);
    }
    return 1;
}

s32 dds3MatterObjectBasicDrawProcedure(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work = aPtr->workdata;
    u32 color;

    if (work->OtLinkIdx == -1) {
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        dds3ObjectBaseModelLinkOt(&D_002F69B8[0], aPtr);
        rsetOtOfs(0x27);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    } else {
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x60, 1, 0, 0, 1, 2);
        if (work->alpmode < 5) {
            if (work->alpmode >= 3) {
            if (aPtr->color != 0x80808080) {
                if (dds3ObjectBaseCheckFlag(D_0054EAE8, 1) != 0) {
                    color = aPtr->color;
                    aPtr->color = 0x80808080;
                    dds3ObjectBaseModelLinkOt(&D_002F69B8[work->OtLinkIdx], aPtr);
                    aPtr->color = color;
                } else {
                    dds3ObjectBaseModelLinkOt(&dds3MatterDrawParam, aPtr);
                }
            } else {
                dds3ObjectBaseModelLinkOt(&D_002F69B8[work->OtLinkIdx], aPtr);
            }
            }
        }
        dds3ObjectBaseModelLinkOt(&D_002F69B8[work->OtLinkIdx], aPtr);
        rsetOtOfs(0x22);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    }
    return 1;
}
