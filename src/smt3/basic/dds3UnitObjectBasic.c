#include "common.h"
#include "basic.h"
#include "event.h"
#include "model.h"
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

extern void dds3InitPoseSet(dds3Basic_t *basic);
extern void *sdfChipAllocDirect(u32 size);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

s32 dds3UnitObjectBasicConstructor(dds3Basic_t *basic)
{
    dds3UnitObjectBasicWorkData_t *work;

    dds3InitPoseSet(basic);
    basic->workdata = sdfChipAllocDirect(sizeof(*work));
    memset(basic->workdata, 0, sizeof(*work));
    work = basic->workdata;
    work->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(basic);
    work->hUnit = NULL;
    work->EL_serialID = -1;
    work->ot = -1;
    dds3ObjectBaseOnFlag(basic, 0x42);
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

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicMainProcedure);

extern sdfModelOt_t D_002F69B8[];
extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);
extern void dds3ObjectBaseModelLinkOt(sdfModelOt_t *ot, dds3Basic_t *basic);
extern void fldSdwDraw(dds3Basic_t *basic);
extern void dds3DrawUnitObjectShadow(s32 alpha, f32 x, f32 y, f32 z, f32 radius);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);
extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pose, u32 flag);

s32 dds3UnitObjectBasicDrawProcedure(dds3Basic_t *basic) {
    sdfVector4_t worldPosition;
    dds3UnitObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    sdfModelRootNode_t *modelRoot;
    struct evtUnit_t *unit;
    s32 alpha;

    memset(&worldPosition, 0, sizeof(worldPosition));
    worldPosition.w = 1.0f;
    if (dds3ObjectBaseCheckFlag(basic, 1)) {
        return 1;
    }

    unit = dds3GetUnitHandleObjectBasic(basic);
    if (dds3ObjectBaseCheckFlag(basic, 0x200) && unit != NULL &&
        (unit->owner->flags & 1) == 0) {
        fldSdwDraw(basic);
    }

    work = basic->workdata;
    if (work->ot == -1) {
        dds3ObjectBaseModelLinkOt(&D_002F69B8[0], basic);
    } else {
        dds3ObjectBaseModelLinkOt(&D_002F69B8[work->ot], basic);
    }

    if (!dds3ObjectBaseCheckFlag(basic, 0x400) ||
        dds3ObjectBaseCheckFlag(basic, 0x1000) || unit == NULL) {
        return 1;
    }

    control = dds3UnitObjectBasicGetControlWorkData(basic);
    modelRoot = ((mdlObj_t *)control->pModel)->inner;
    alpha = ((const u8 *)&modelRoot->color)[3];
    if (sdfModelGetMarkPos(unit->owner->inner, 0) != 0) {
        SDF_VU0_STOREVECTOR(&worldPosition);
        dds3DrawUnitObjectShadow(alpha, worldPosition.x, basic->poseset->pos.y,
                                 worldPosition.z, basic->poseset->r);
    } else {
        if (dds3CheckPoseSetFlag(basic->poseset, 8)) {
            worldPosition.x = basic->poseset->pos2.x;
            worldPosition.y = basic->poseset->pos2.y;
            worldPosition.z = basic->poseset->pos2.z;
        } else {
            worldPosition.x = basic->poseset->pos.x;
            worldPosition.y = basic->poseset->pos.y;
            worldPosition.z = basic->poseset->pos.z;
        }
        dds3DrawUnitObjectShadow(alpha, worldPosition.x, worldPosition.y,
                                 worldPosition.z, basic->poseset->r);
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

