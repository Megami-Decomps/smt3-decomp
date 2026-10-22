#include "common.h"
#include "event.h"
#include "kernel.h"
#include "draw3d.h"
#include "libsdf.h"
#include "ps2asm.h"
void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

u32 dds3AllocObjectId(void);

void dds3ObjectBaseCreateMover(void *pObjectBasic);

void dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                     s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, f32 *pos, f32 *rot);

struct dds3Basic_t *dds3GetControlScene(void);

extern void *memset(void *a, s32 b, u32 c);


void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);


void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

void *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32, s32);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

extern void dds3ObjectBaseOnFlag(void *object, s32 flags);

extern void dds3ObjectBaseRegistArea(void *pObjectBasic);

void evtSetUnitPos(u32 unitID, u32 posID);

void Mult(void);


void dds3SetPosPoseSet(dds3Basic_t *pBasic, u128 *pos);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, u128 *rot);

void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern void FromRotation(f32 aAngle);

void dds3DeleteScene(struct dds3Basic_t *scene);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

enum {
    EVT_WORLD_SLOT_MOVIE = 3,
    EVT_WORLD_SLOT_UNIT = 5
};

struct BfFlw0Header;

typedef struct evtWorldSlot_t {
    s32 count;
    dds3Basic_t *head;
    dds3Basic_t *tail;
} evtWorldSlot_t;

typedef struct evtWorldTable_t {
    s32 areaRoomKey;
    struct sdfMemHandle_t *resource;
    evtWorldSlot_t *slots;
    dds3Basic_t *cameraObject;
    dds3Basic_t *playerObject;
    u32 indexedHandle;
    struct sdfMemHandle_t *sceneResourceHandle;
    struct BfFlw0Header *sceneScript;
    s32 drawEnabled;
    u8 pad24[0x1C];
} evtWorldTable_t;

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

void dds3SceneBasicMakeScene(dds3Basic_t *object, u32 resourceId);

void dds3SetActiveScene(struct dds3Basic_t *scene);

void dds3SetControlScene(struct dds3Basic_t *scene);

void dds3SetMapNo(dds3Basic_t *scene, u32 mapno);

s32 evtLoadArea(s32 fld, s32 area);

extern dds3Basic_t *dds3AddScene(void);

extern void evtSetTestCamera(void);

extern void dds3SetScreenFovy(f32 fovy);

extern void dds3SetLight(void *light, s32 no);

extern void dds3SetFog(void *fog);

extern void dds3SetAmbient(u128 *ambient);

/* Three parallel lights, 0x40 each, which is what the IDB types here and what
 * the +0x40 steps below were spelling as byte arithmetic. */
extern sdf3DLight_t D_0032C5F0[3];

extern sdf3DFogParam_t D_0032C6B0;

extern u128 D_0032C6D0;

extern void sprintf(char *, const char *, ...);


extern void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area,
                                                       u32 pF1, u32 pF2,
                                                       const sdfTextureOffsetListHeader_t *pTB, u32 pBF);

extern s32 scrStartScriptByName(s32 priority, const char *path, s32 option);

extern char D_0056CE50[];

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

s32 dds3GetProcessCount(u32 type);

dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n);

enum evtProcessState {
    EVT_PROCESS_STATE_DELAYED_START = 1,
    EVT_PROCESS_STATE_DESTROY_PENDING = 3
};

extern const char D_0053A828[0x28];

s32 printf(const char *format, ...);

void dds3PathBasicPauseOn(dds3PathBasicControl_t *value);

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(void *pObjectBasic);

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);


void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);


extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *pControl);

void *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *pMoverBasic);


extern void dds3PathBasicLoopOn(struct dds3PathBasicControl_t *pControl);

extern void dds3PathBasicLoopOff(struct dds3PathBasicControl_t *pControl);

typedef struct effNode_t effNode_t;

void dds3EffectSetLoop(effNode_t *hEffect, u8 loop);

typedef struct evtNodeInner_t {
    u8 pad00[0x08];
    s32 kind;
    void *node;
} evtNodeInner_t;


void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *pObjectBasic, u32 mode);

struct dds3Basic_t *dds3GetActiveScene(void);

void evtSetTestCamera(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    f32 rotation[4];
    dds3Basic_t *object;

    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    object = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), position, rotation);
    dds3GetRadiusPoseSet((dds3Basic_t *)object, 10.0f);
    dds3ObjectBaseCreateMover(object);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), (dds3Basic_t *)object);
    dds3ObjectBaseSetPadControl(object, 0);
}

s32 evtSetRegistUnit(s32 major, s32 minor) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, position, rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3ObjectBaseMakeUnit(object, major, minor);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetPadControl(object, 0);
    return counter;
}

s32 evtLoadUnit(s32 major, s32 minor) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, position, rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3ObjectBaseMakeUnit(object, major, minor);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}

void evtSetUnitPos(u32 unitID, u32 posID) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *target;
    dds3Basic_t *source;
    evtWorldSourceTransformPrefix_t *sourceTransform;

    target = dds3SearchSceneBasic(dds3GetControlScene(), unitID, 5);
    if (target != NULL) {
        source = dds3SearchSceneBasic(dds3GetControlScene(), posID, 0x11);
        if (source != NULL) {
            sourceTransform = source->workdata;
            dds3SetPosPoseSet(target, (u128 *)&sourceTransform->position);
                        SDF_VU0_LOADVECTOR(quaternion);
            FromRotation(3.14159265f);
                        SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
                        SDF_VU0_STOREVECTOR(quaternion);
            dds3SetRotPoseSet(target, (u128 *)quaternion);
            dds3GetPosPoseSet(target);
            SDF_VU0_STOREVECTOR(&target->poseset->pos2);
        }
    }
}

void evtFreeStage(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}

/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void) {
    dds3Basic_t *secondaryObject = (dds3Basic_t *)dds3GetControlScene();
    evtWorldTable_t *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((evtWorldTable_t *)secondaryObject->workdata);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

/* Sized explicitly: retail pads this to 16 bytes and an inline literal would
 * emit 13 plus whatever gcc aligns to, which shortens .rodata and shifts the
 * link. The same padding is why the strings around it stay placeholders. */
const char D_0053A7F8[0x10] = "%sf%03d_%03d";


/* Build the field world object and load its stage resource. */
s32 evtLoadArea(s32 fld, s32 area)
{
    char directory[0x40];
    char resourcePath[0x50];
    dds3Basic_t *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (fld << 16) + area);
    fldMakeFilePath(directory, fld, area);
    sprintf(resourcePath, D_0053A7F8, directory, fld, area);
    dds3SceneBasicMakeScene(worldObject, (u32)resourcePath);
    evtSetTestCamera();
    if (fld < 10) {
        dds3SetScreenFovy(0.9424777031f);
    } else {
        dds3SetScreenFovy(0.75398216f);
    }
    dds3SetLight(&D_0032C5F0[0], 0);
    dds3SetLight(&D_0032C5F0[1], 1);
    dds3SetLight(&D_0032C5F0[2], 2);
    dds3SetFog(&D_0032C6B0);
    dds3SetAmbient(&D_0032C6D0);
    return 1;
}

/* Build a field world object from the supplied resource parameters. */
s32 evtLoadArea_FromMemory(s32 fld, s32 area, s32 pF1, s32 pF2,
                                     const sdfTextureOffsetListHeader_t *pTB, s32 pBF)
{
    dds3Basic_t *worldObject;
    char directory[0x40];

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (fld << 16) + area);
    fldMakeFilePath(directory, fld, area);
    dds3SceneBasicMakeScene_FromMemory(worldObject, fld, area, pF1, pF2, pTB, pBF);
    evtSetTestCamera();
    dds3SetScreenFovy(0.75398216f);
    dds3SetLight(&D_0032C5F0[0], 0);
    dds3SetLight(&D_0032C5F0[1], 1);
    dds3SetLight(&D_0032C5F0[2], 2);
    dds3SetFog(&D_0032C6B0);
    dds3SetAmbient(&D_0032C6D0);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

void evtStartEvent(s32 no) {
    sprintf(D_0056CE50, "/event/e%03d/e%03d/scr/e%03d.bf", no - no % 10, no, no);
    scrStartScriptByName(0x3EB, D_0056CE50, 0);
}
const char D_0053A828[0x28] = "kill field script -> [%s]\n";


void evtKillFieldScript(void)
{
    s32 taskState;
    s32 taskIndex;
    dds3ProcessID_t *task;

    for (taskState = EVT_PROCESS_STATE_DELAYED_START; taskState < EVT_PROCESS_STATE_DESTROY_PENDING; taskState++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < dds3GetProcessCount(taskState); taskIndex++) {
            task = dds3SearchProcessNumber(taskState, taskIndex);
            if (task->priority == 0x3EA) {
                printf(D_0053A828, task->name);
                dds3KillProcess(task, 0);
                /* Destruction changes the queue, so restart the current scan. */
                goto restartStateScan;
            }
        }
    }
}

void evtPathStop(dds3Basic_t *basic)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicPauseOn(slotData);
    }
}

void evtPathStart(dds3Basic_t *basic)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicPauseOff(slotData);
    }
}

/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *basic, f32 rate) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        if (rate < 0.0f) {
            rate = 0.0f;
        }
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        dds3PathBasicControlSetRatio(slotData, rate);
    }
}

void evtPathDir(dds3Basic_t *basic, u32 dir) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(slotData, dir);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj) {
    dds3ObjectBaseControlWorkData_t *ref;
    dds3Basic_t *slotNode;

    if (path == NULL) {
        return 0;
    }
    ref = dds3GetBaseControlWorkData(obj);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->control[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3MoverBasicSetPath(slotNode, path);
    dds3MoverBasicMakePath(slotNode);
    return 1;
}

s32 evtCheckPathStat(dds3Basic_t *obj) {
    f32 target = 1.0f;
    dds3Basic_t *slot;
    dds3PathBasicControl_t *handle;
    s32 kind;

    if (obj == NULL) {
        return -1;
    }
    slot = dds3ObjectBaseGetControlBasic(obj, 1);
    if (slot == NULL) {
        return -1;
    }
    handle = dds3MoverBasicGetPathControl(slot);
    if (handle == NULL) {
        return -1;
    }
    kind = dds3PathBasicControlGetMode(handle);
    if (kind != 0) {
        if (kind != 1) {
            return -1;
        }
        target = 0.0f;
    }
    if (dds3PathBasicControlGetRatio(handle) == target) {
        return 0;
    }
    return 1;
}

void evtPathLoop(dds3Basic_t *basic, s32 sw) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(basic);

    if (data == NULL) {
        return;
    }
    if (sw != 0) {
        dds3PathBasicLoopOn(data);
    } else {
        dds3PathBasicLoopOff(data);
    }
}

dds3Basic_t *evtAddCamera(f32 *eye, f32 *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), eye, rot);

    if (obj == 0) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}

void evtEffectLoopControl(dds3Basic_t *eff, s32 sw) {
    evtNodeInner_t *inner = eff->workdata;
    void *node;

    switch (inner->kind) {
    case 1:
    case 7:
    case 8:
        break;
    default:
        return;
    }
    node = inner->node;
    if (sw == 0) {
        dds3EffectSetLoop(node, sw);
    }
}

s32 evtStageAnimeControl(s32 type, s32 mode) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    s32 combinedMode;

    if (world == NULL) {
        return 0;
    }
    combinedMode = type * 3 + mode;
    for (node = ((evtWorldTable_t *)world->workdata)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, combinedMode);
    }
    return 1;
}
