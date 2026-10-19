#include "common.h"
#include "basic.h"

typedef struct {
    u8 unk00[0x64];
    u32 flag;
} dds3LinkedLightWork_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *basic);

extern void dds3SetPosPoseSet(dds3Basic_t *basic, void *position);

extern void dds3SetRotPoseSet(dds3Basic_t *basic, void *rotation);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern dds3Basic_t *dds3AreaBasicGetFastArea(void);

extern void dds3ObjectBaseSetControlBasicType(dds3Basic_t *basic, dds3Basic_t *area, s32 type);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *basic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void evtDeleteUnitHandle(struct evtUnit_t *unit);

extern void dds3ObjectBaseFreeModelHandle(dds3Basic_t *basic);

extern void sdfChipFreeDirect(void *block);

extern void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *basic, s32 duration);

extern void dds3LinkLightObjectToUnitObject(dds3Basic_t *light, dds3Basic_t *basic, s32 duration);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *scene, s32 type);

extern s32 dds3GetBasicArrayCount(dds3BasicArrayHandle_t *list);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *list);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *list);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *list);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *list);

extern s32 dds3ComparePosPoseSet2(dds3PoseSet_t *first, dds3PoseSet_t *second);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *basic);

dds3ObjectBaseControl_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *basic) {
    return ((dds3UnitObjectWork_t *)basic->workdata)->control;
}

dds3Basic_t *dds3AddUnitObjectBasic(s32 id, void *position, void *rotation) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_UNITOBJECT);
    dds3UnitObjectWork_t *work;

    if (basic != NULL) {
        basic->serialID = id;
        dds3ObjectBaseCreateMover(basic);
        dds3SetPosPoseSet(basic, position);
        dds3SetRotPoseSet(basic, rotation);
        dds3SetRadiusPoseSet(basic->poseset);
        work = basic->workdata;
        work->status = -1;
        work->unk04 = 0;
        dds3ObjectBaseSetControlBasicType(basic, dds3AreaBasicGetFastArea(), 5);
        dds3ObjectBaseRegistArea(basic);
        return basic;
    }
    return NULL;
}

void dds3DeleteUnitObjectBasic(dds3Basic_t *basic) {
    dds3DeleteSceneBasicData(basic);
}

s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *basic) {
    return ((dds3UnitObjectWork_t *)basic->workdata)->status;
}

struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic) {
    return ((dds3UnitObjectWork_t *)basic->workdata)->unit;
}

void dds3UnitObjectBasicSetOt(dds3Basic_t *basic, s32 ot) {
    ((dds3UnitObjectWork_t *)basic->workdata)->ot = ot;
}

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicConstructor);

void dds3UnitObjectBasicDestructor(dds3Basic_t *basic) {
    dds3UnitObjectWork_t *work;

    dds3UnitObjectBasicEL_LinkFree(basic);
    dds3FreePoseSet(basic);
    work = basic->workdata;
    if (work->status != -1) {
        work->status = -1;
    }
    if (work->unit != NULL) {
        evtDeleteUnitHandle(work->unit);
        work->unit = NULL;
    }
    dds3ObjectBaseFreeModelHandle(basic);
    dds3ObjectBaseFreeControlWorkData(work->control);
    sdfChipFreeDirect(basic->workdata);
}

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicMainProcedure);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicDrawProcedure);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *basic) {
    dds3UnitObjectWork_t *work;

    work = basic->workdata;
    if (work->lightID != -1) {
        dds3UnlinkLightObjectFromUnitObject(basic, 10);
        work->lightID = -1;
    }
}

/* Detach a light when leaving its radius, then select the first eligible light. */
void dds3UnitObjectBasicEL_Link(dds3Basic_t *basic) {
    dds3UnitObjectWork_t *work = basic->workdata;
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
                dds3ComparePosPoseSet2(basic->poseset, light->poseset) == 0 &&
                work->lightID == light->serialID) {
                dds3UnlinkLightObjectFromUnitObject(basic, 10);
                work->lightID = -1;
            }
        } while (dds3SetNextBasicArray(list) != 0);
    }
    dds3FreeBasicArrayHandle(list);

    if (work->lightID == -1) {
        list = dds3SceneBasicGetList(scene, BASIC_TYPE_LIGHTOBJECT);
        if (dds3SetFirstBasicArray(list) == 0) {
            goto freeList;
        }
        do {
            light = dds3GetBasicArray(list);
            lightWork = light->workdata;
            if (!(lightWork->flag & 4) &&
                dds3ComparePosPoseSet2(basic->poseset, light->poseset) != 0) {
                goto linkLight;
            }
        } while (dds3SetNextBasicArray(list) != 0);

freeList:
        dds3FreeBasicArrayHandle(list);
        return;

linkLight:
        dds3LinkLightObjectToUnitObject(light, basic, 10);
        work->lightID = light->serialID;
        dds3FreeBasicArrayHandle(list);
    }
}

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_00530D60);

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_00530D70);

