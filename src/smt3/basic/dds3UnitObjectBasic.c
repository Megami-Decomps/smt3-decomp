#include "common.h"
#include "basic.h"
#include "model.h"
#include "event.h"
#include "ps2asm.h"

typedef struct {
    u8 unk00[0x64];
    u32 flag;
} dds3LinkedLightWork_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, void *pos);

extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, void *rot);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern dds3Basic_t *dds3AreaBasicGetFastArea(void);

extern void dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *memset(void *a, s32 b, u32 c);

extern void dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);

extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);

extern void dds3ObjectBaseModelLinkOt(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic);

extern void fldSdwDraw(dds3Basic_t *basic);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);

extern void dds3DrawUnitObjectShadow(s32 fade, f32 x, f32 y, f32 z, f32 radius);

extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern sdfModelOt_t D_002F69B8[];

extern void *memcpy(void *dst, const void *src, u32 n);

extern void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

extern s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *pPoseSet, dds3Basic_t *pBasic);

extern void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);

extern void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern s32 mdlGetMajorNum(void *aHandle);

extern s32 mdlGetMinorNum(void *aHandle);

extern void mdlSetScale(void *aHandle);

extern void mdlSetRotate(void *aHandle);

extern void mdlSetPosition(void *aHandle);

extern f32 sdfSin(f32 aRadius);

extern void Normalize(void);

extern void FromRotation(f32 aAngle);

extern void Mult(void);

extern const sdfVector4_t D_00530D60;

extern const sdfVector4_t D_00530D70;

extern void evtDeleteUnitHandle(struct evtUnit_t *h);

extern void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic);

extern void sdfChipFreeDirect(void *aPtr);

extern void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame);

extern void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

extern s32 dds3GetBasicArrayCount(dds3BasicArrayHandle_t *hndl);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3ComparePosPoseSet2(dds3PoseSet_t *first, dds3PoseSet_t *second);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic);

void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic);

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic) {
    return ((dds3UnitObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_UNITOBJECT);
    dds3UnitObjectBasicWorkData_t *work;

    if (basic != NULL) {
        basic->serialID = serialID;
        dds3ObjectBaseCreateMover(basic);
        dds3SetPosPoseSet(basic, aPos);
        dds3SetRotPoseSet(basic, aRot);
        dds3SetRadiusPoseSet(basic->poseset);
        work = basic->workdata;
        work->status = -1;
        work->pWorkData = 0;
        dds3ObjectBaseSetControlBasicType(basic, dds3AreaBasicGetFastArea(), 5);
        dds3ObjectBaseRegistArea(basic);
        return basic;
    }
    return NULL;
}

void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr) {
    dds3DeleteSceneBasicData(aPtr);
}

s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *aPtr) {
    return ((dds3UnitObjectBasicWorkData_t *)aPtr->workdata)->status;
}

struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b) {
    return ((dds3UnitObjectBasicWorkData_t *)b->workdata)->hUnit;
}

void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx) {
    ((dds3UnitObjectBasicWorkData_t *)aPtr->workdata)->ot = idx;
}

s32 dds3UnitObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3UnitObjectBasicWorkData_t *work;
    void *allocation;

    dds3InitPoseSet(aPtr);
    allocation = sdfChipAllocDirect(sizeof(dds3UnitObjectBasicWorkData_t));
    aPtr->workdata = allocation;
    memset(allocation, 0, sizeof(dds3UnitObjectBasicWorkData_t));
    work = aPtr->workdata;
    work->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    work->hUnit = NULL;
    work->EL_serialID = -1;
    work->ot = -1;
    dds3ObjectBaseOnFlag(aPtr, 0x42);
    return 1;
}

void dds3UnitObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3UnitObjectBasicWorkData_t *work;

    dds3UnitObjectBasicEL_LinkFree(aPtr);
    dds3FreePoseSet(aPtr);
    work = aPtr->workdata;
    if (work->status != -1) {
        work->status = -1;
    }
    if (work->hUnit != NULL) {
        evtDeleteUnitHandle(work->hUnit);
        work->hUnit = NULL;
    }
    dds3ObjectBaseFreeModelHandle(aPtr);
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(aPtr->workdata);
}

s32 dds3UnitObjectBasicMainProcedure(dds3Basic_t *aPtr) {
    sdfVector4_t axis;
    dds3PoseSet_t backup;
    dds3UnitObjectBasicWorkData_t *work;
    void *model;
    s32 wobble = 0;
    s32 rigid = 0;
    s32 minor;

    memcpy(&axis, &D_00530D70, sizeof(axis));
    work = aPtr->workdata;
    if (work->hUnit != NULL) {
        dds3BackupPoseSet(&backup);
        if (evtUnitManagerPrimaryFunc(&backup, aPtr) != 0) {
            dds3GetSclPoseSetMCR(aPtr, &backup.scl);
            dds3AddRotPoseSet(aPtr, &backup.rot);
            dds3GetPosPoseSetMCR(aPtr, &backup.pos);
        }
    }
    model = work->pObjectBaseControlWorkData->pModel;
    if (model != NULL && dds3CheckPoseSetFlag(aPtr->poseset, 1) == 1) {
        dds3OffPoseSetFlag(aPtr->poseset, 1);
        if (work->hUnit != NULL) {
            if (work->hUnit->motionState == 0 || work->hUnit->motionState == 3) {
                u32 f = work->hUnit->flags & 0x40;
                wobble = f != 0;
            }
            if (work->hUnit->flags & 0x400000) {
                wobble = 1;
            }
        }
        if (mdlGetMajorNum(model) == 1) {
            minor = mdlGetMinorNum(model);
            if (minor == 0xC0 ||
                (minor >= 0xC0 ? (minor < 0x156 ? minor >= 0x153 : 0) : 0)) {
                rigid = 1;
            }
        }
        SDF_VU0_LOADVECTOR(&aPtr->poseset->scl);
        if (!rigid && work->hUnit != NULL && wobble) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_X(sdfSin(work->hUnit->wobblePhase) * 0.01f);
            SDF_VU0_SETVECTOR_Y(sdfSin(work->hUnit->wobblePhase) * 0.004f);
            SDF_VU0_MULVECTOR();
            SDF_VU0_ADDVECTOR();
        }
        mdlSetScale(model);
        SDF_VU0_LOADVECTOR(&aPtr->poseset->rot);
        Normalize();
        if (!rigid && work->hUnit != NULL && wobble) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR_MEMOPS(axis);
            FromRotation(sdfSin(work->hUnit->wobblePhase) * 0.006f);
            Mult();
        }
        if (*(u32 *)work->unk18 & 1) {
            SDF_VU0_LOADVECTOR_B(&D_00530D60);
            Mult();
        }
        mdlSetRotate(model);
        if (dds3CheckPoseSetFlag(aPtr->poseset, 8) != 0) {
            SDF_VU0_LOADVECTOR(&aPtr->poseset->pos2);
            SDF_VU0_VECTOR_W_SET_ONE();
        } else {
            SDF_VU0_LOADVECTOR(&aPtr->poseset->pos);
            SDF_VU0_VECTOR_W_SET_ONE();
        }
        mdlSetPosition(model);
        dds3SetRadiusPoseSet(aPtr->poseset);
    }
    if (dds3ObjectBaseCheckFlag(aPtr, 0x100) == 0) {
        dds3UnitObjectBasicEL_Link(aPtr);
    }
    return 1;
}

s32 dds3UnitObjectBasicDrawProcedure(dds3Basic_t *aPtr) {
    sdfVector4_t pos = {0.0f, 0.0f, 0.0f, 1.0f};
    dds3UnitObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    struct evtUnit_t *hUnit;
    s32 fade;

    if (dds3ObjectBaseCheckFlag(aPtr, 1) != 0) {
        return 1;
    }
    hUnit = dds3GetUnitHandleObjectBasic(aPtr);
    if (dds3ObjectBaseCheckFlag(aPtr, 0x200) != 0 && hUnit != NULL &&
        !(hUnit->owner->flags & 1)) {
        fldSdwDraw(aPtr);
    }
    work = aPtr->workdata;
    if (work->ot == -1) {
        dds3ObjectBaseModelLinkOt(&D_002F69B8[0], aPtr);
    } else {
        dds3ObjectBaseModelLinkOt(&D_002F69B8[work->ot], aPtr);
    }
    if (dds3ObjectBaseCheckFlag(aPtr, 0x400) == 0) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(aPtr, 0x1000) != 0) {
        return 1;
    }
    if (hUnit == NULL) {
        return 1;
    }
    control = dds3UnitObjectBasicGetControlWorkData(aPtr);
    fade = *((u8 *)&((mdlObj_t *)control->pModel)->inner->color + 3);
    if (sdfModelGetMarkPos(hUnit->owner->inner, 0) != 0) {
        SDF_VU0_STOREVECTOR_MEMOPS(pos);
        dds3DrawUnitObjectShadow(fade, pos.x, aPtr->poseset->pos.y, pos.z, aPtr->poseset->r);
    } else {
        if (dds3CheckPoseSetFlag(aPtr->poseset, 8) != 0) {
            pos.x = aPtr->poseset->pos2.x;
            pos.y = aPtr->poseset->pos2.y;
            pos.z = aPtr->poseset->pos2.z;
        } else {
            pos.x = aPtr->poseset->pos.x;
            pos.y = aPtr->poseset->pos.y;
            pos.z = aPtr->poseset->pos.z;
        }
        dds3DrawUnitObjectShadow(fade, pos.x, pos.y, pos.z, aPtr->poseset->r);
    }
    return 1;
}

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic) {
    dds3UnitObjectBasicWorkData_t *work;

    work = unitBasic->workdata;
    if (work->EL_serialID != -1) {
        dds3UnlinkLightObjectFromUnitObject(unitBasic, 10);
        work->EL_serialID = -1;
    }
}

/* Detach a light when leaving its radius, then select the first eligible light. */
void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic) {
    dds3UnitObjectBasicWorkData_t *work = unitBasic->workdata;
    dds3Basic_t *scene = dds3GetControlScene();
    dds3BasicArrayHandle_t *list;
    dds3Basic_t *light;
    dds3LinkedLightWork_t *lightWork;

    if (scene == NULL) {
        return;
    }
    list = dds3SceneBasicGetList(scene, BASIC_TYPE_LIGHTOBJECT);
    if (list == NULL) {
        return;
    }
    if (dds3GetBasicArrayCount(list) == 0) {
        dds3FreeBasicArrayHandle(list);
        return;
    }
    if (dds3SetFirstBasicArray(list) != 0) {
        do {
            light = dds3GetBasicArray(list);
            lightWork = light->workdata;
            if (!(lightWork->flag & 4) &&
                dds3ComparePosPoseSet2(unitBasic->poseset, light->poseset) == 0 &&
                work->EL_serialID == light->serialID) {
                dds3UnlinkLightObjectFromUnitObject(unitBasic, 10);
                work->EL_serialID = -1;
            }
        } while (dds3SetNextBasicArray(list) != 0);
    }
    dds3FreeBasicArrayHandle(list);

    if (work->EL_serialID == -1) {
        list = dds3SceneBasicGetList(scene, BASIC_TYPE_LIGHTOBJECT);
        if (dds3SetFirstBasicArray(list) == 0) {
            goto freeList;
        }
        do {
            light = dds3GetBasicArray(list);
            lightWork = light->workdata;
            if (!(lightWork->flag & 4) &&
                dds3ComparePosPoseSet2(unitBasic->poseset, light->poseset) != 0) {
                goto linkLight;
            }
        } while (dds3SetNextBasicArray(list) != 0);

freeList:
        dds3FreeBasicArrayHandle(list);
        return;

linkLight:
        dds3LinkLightObjectToUnitObject(light, unitBasic, 10);
        work->EL_serialID = light->serialID;
        dds3FreeBasicArrayHandle(list);
    }
}

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_00530D60);

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_00530D70);

