#include "common.h"
#include "event.h"
#include "kernel.h"
#include "ps2asm.h"
void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

u32 dds3AllocObjectId(void);

void dds3ObjectBaseCreateMover(void *object);

void dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                     s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *world, dds3Basic_t *camera);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 counter, f32 *position, f32 *rotation);

struct dds3Basic_t *dds3GetControlScene(void);

extern void *memset(void *dst, s32 value, u32 size);


void mdlLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);


void *dds3ObjectBaseGetModelHandle(dds3Basic_t *object);

void *dds3ObjectBaseSetControlBasic(dds3Basic_t *object, dds3Basic_t *slotData);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *object, s32, s32);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 counter, void *position, void *rotation);

extern void dds3ObjectBaseOnFlag(void *object, s32 flags);

extern void dds3ObjectBaseRegistArea(void *object);

void evtSetUnitPos(u32 targetKey, u32 sourceKey);

void Mult(void);


void dds3SetPosPoseSet(dds3Basic_t *object, u128 *vector);

void dds3SetRotPoseSet(dds3Basic_t *object, u128 *vector);

void dds3GetPosPoseSet(dds3Basic_t *object);

extern void FromRotation(f32 angle);

void dds3DeleteScene(struct dds3Basic_t *worldNode);

void dds3DeleteSceneBasicData(dds3Basic_t *node);

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

void fldMakeFilePath(char *buffer, s32 area, s32 unused);

void dds3SceneBasicMakeScene(dds3Basic_t *object, u32 resourceId);

void dds3SetActiveScene(struct dds3Basic_t *primaryObject);

void dds3SetControlScene(struct dds3Basic_t *secondaryObject);

void dds3SetMapNo(dds3Basic_t *worldNode, u32 areaRoomKey);

s32 evtLoadArea(s32 area, s32 room);

extern dds3Basic_t *dds3AddScene(void);

extern void evtSetTestCamera(void);

extern void dds3SetScreenFovy(f32 fieldOfView);

extern void dds3SetLight(void *lightSource, s32 index);

extern void dds3SetFog(void *source);

extern void dds3SetAmbient(u128 *source);

extern const char D_0053A7F8[0x10];

extern char D_0032C5F0[];

extern char D_0032C6B0[];

extern char D_0032C6D0[];

extern void sprintf(char *, const char *, ...);


extern void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *object, u32 area, u32 room,
                                                       u32 resource0Data, u32 resource1Data,
                                                       const sdfTextureOffsetListHeader_t *textureOffsets, u32 flags);

extern s32 scrStartScriptByName(s32 priority, const char *path, s32 option);

extern char D_0056CE50[];

s32 dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

s32 dds3GetProcessCount(u32 state);

dds3ProcessID_t *dds3SearchProcessNumber(u32 taskState, s32 taskIndex);

enum evtProcessState {
    EVT_PROCESS_STATE_DELAYED_START = 1,
    EVT_PROCESS_STATE_DESTROY_PENDING = 3
};

extern const char D_0053A828[0x28];

s32 printf(const char *format, ...);

void dds3PathBasicPauseOn(dds3PathBasicControl_t *value);

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(void *object);

void dds3PathBasicPauseOff(dds3PathBasicControl_t *value);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *value, f32 multiplier);


void dds3MoverBasicSetPath(dds3Basic_t *object, dds3Basic_t *sourceObject);


extern void dds3MoverBasicMakePath(dds3Basic_t *object);

f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *value);

void *dds3ObjectBaseGetControlBasic(dds3Basic_t *object, s32 slotIndex);

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *object);


extern void dds3PathBasicLoopOn(struct dds3PathBasicControl_t *data);

extern void dds3PathBasicLoopOff(struct dds3PathBasicControl_t *data);

typedef struct effNode_t effNode_t;

void dds3EffectSetLoop(effNode_t *node, u8 flag);

typedef struct evtNodeInner_t {
    u8 pad00[0x08];
    s32 kind;
    void *node;
} evtNodeInner_t;


void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *object, u32 requestedMode);

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

s32 evtSetRegistUnit(s32 slotKind, s32 resource) {
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
    dds3ObjectBaseMakeUnit(object, slotKind, resource);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetPadControl(object, 0);
    return counter;
}

s32 evtLoadUnit(s32 slotKind, s32 resource) {
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
    dds3ObjectBaseMakeUnit(object, slotKind, resource);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *target;
    dds3Basic_t *source;
    evtWorldSourceTransformPrefix_t *sourceTransform;

    target = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (target != NULL) {
        source = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
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

const char D_0053A7F8[0x10] = "%sf%03d_%03d";


/* Build the field world object and load its stage resource. */
s32 evtLoadArea(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    dds3Basic_t *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (area << 16) + room);
    fldMakeFilePath(directory, area, room);
    sprintf(resourcePath, D_0053A7F8, directory, area, room);
    dds3SceneBasicMakeScene(worldObject, (u32)resourcePath);
    evtSetTestCamera();
    if (area < 10) {
        dds3SetScreenFovy(0.9424777031f);
    } else {
        dds3SetScreenFovy(0.75398216f);
    }
    dds3SetLight(D_0032C5F0, 0);
    dds3SetLight(D_0032C5F0 + 0x40, 1);
    dds3SetLight(D_0032C5F0 + 0x80, 2);
    dds3SetFog(D_0032C6B0);
    dds3SetAmbient((u128 *)D_0032C6D0);
    return 1;
}

/* Build a field world object from the supplied resource parameters. */
s32 evtLoadArea_FromMemory(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                     const sdfTextureOffsetListHeader_t *textureOffsets, s32 flags)
{
    dds3Basic_t *worldObject;
    char directory[0x40];

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (area << 16) + room);
    fldMakeFilePath(directory, area, room);
    dds3SceneBasicMakeScene_FromMemory(worldObject, area, room, resource0Data, resource1Data, textureOffsets, flags);
    evtSetTestCamera();
    dds3SetScreenFovy(0.75398216f);
    dds3SetLight(D_0032C5F0, 0);
    dds3SetLight(D_0032C5F0 + 0x40, 1);
    dds3SetLight(D_0032C5F0 + 0x80, 2);
    dds3SetFog(D_0032C6B0);
    dds3SetAmbient((u128 *)D_0032C6D0);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

void evtStartEvent(s32 eventId) {
    sprintf(D_0056CE50, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
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

void evtPathStop(dds3Basic_t *object)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOn(slotData);
    }
}

void evtPathStart(dds3Basic_t *object)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(object);
    if (slotData != 0) {
        dds3PathBasicPauseOff(slotData);
    }
}

/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *worldNode, f32 multiplier) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(worldNode);
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        dds3PathBasicControlSetRatio(slotData, multiplier);
    }
}

void evtPathDir(dds3Basic_t *worldNode, u32 direction) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(worldNode);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(slotData, direction);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(dds3Basic_t *object, dds3Basic_t *owner) {
    dds3ObjectBaseControl_t *ref;
    dds3Basic_t *slotNode;

    if (object == NULL) {
        return 0;
    }
    ref = dds3GetBaseControlWorkData(owner);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->control[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3MoverBasicSetPath(slotNode, object);
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

void evtPathLoop(dds3Basic_t *obj, s32 flag) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(obj);

    if (data == NULL) {
        return;
    }
    if (flag != 0) {
        dds3PathBasicLoopOn(data);
    } else {
        dds3PathBasicLoopOff(data);
    }
}

dds3Basic_t *evtAddCamera(f32 *pos, f32 *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), pos, rot);

    if (obj == 0) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}

void evtEffectLoopControl(dds3Basic_t *owner, s32 nodeFlag) {
    evtNodeInner_t *inner = owner->workdata;
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
    if (nodeFlag == 0) {
        dds3EffectSetLoop(node, nodeFlag);
    }
}

s32 evtStageAnimeControl(s32 modeSelector, s32 modeValue) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    s32 combinedMode;

    if (world == NULL) {
        return 0;
    }
    combinedMode = modeSelector * 3 + modeValue;
    for (node = ((evtWorldTable_t *)world->workdata)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, combinedMode);
    }
    return 1;
}
