#include "common.h"
#include "event.h"
#include "kernel.h"



dds3ProcessID_t *dds3SearchProcessName(const char *name);

dds3ProcessID_t *evtSearchEventPack(u32 taskId);

void evtMakeEventPackName(s32 taskId, char *buffer);

typedef u8 sdfTextureClampMode_t;

void *dds3GetProcessData(dds3ProcessID_t *task);

typedef struct evtPackEntry_t {
    s32 kind;
    s32 dataSize;
    s32 cueCount;
    u32 dataOffset;
    s32 secondaryResourceId;
    s32 resourceId;
    u8 pad18[4];
    s32 motionId;
} evtPackEntry_t;

typedef struct evtPackHeader_t {
    u8 pad00[0x10];
    s32 entryCount;
    u8 pad14[0xC];
    evtPackEntry_t entries[0];
} evtPackHeader_t;

typedef struct evtPackLoadState_t {
    s32 eventId;
    s32 loaded;
    void *pendingRequest;
    struct sdfMemHandle_t *resourceAllocation;
    u8 *payload;
    evtPackHeader_t *header;
    evtPackEntry_t *entries;
    u8 *entryPoint;
    u8 pad20[4];
    s32 sceneAllocation1;
    u8 pad28[8];
    s32 sceneAllocation2;
    u8 pad34[4];
    struct sdfTexHandle_t *effect72;
    s32 effect71;
    s32 effect76;
    s32 effect75;
} evtPackLoadState_t;

s32 evtGetEventPackStat(u32 taskId);

evtPackLoadState_t *evtGetPackProcessData(u32 taskId);

void *evtGetResorceData(u32 taskId, s32 key);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);


struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *header);

extern void Eve_DistortionBlurSetTex(sdfTexHandle_t *);

extern s32 evtMotionSeWork;

s32 printf(const char *format, ...);

extern void mdlPackDecode(s32, s32, s32, void *, s32);

extern s32 evtSetRegistUnit(s32 slotKind, s32 resource);


extern s32 evtLoadArea_FromMemory(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                            const sdfTextureOffsetListHeader_t *textureOffsets, s32 flags);

extern void fldFileAddrResetMode(u32 relocateOnRelease);

extern u32 dds3DrawFlag;

enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

typedef struct evtMotionSeTaskParams_t {
    s32 modelKey;
    s32 eventTaskId;
    s32 resourceId;
} evtMotionSeTaskParams_t;

typedef struct evtMotionSeCue_t {
    s32 frame;
    s32 fade;
    u8 unk08[8];
} evtMotionSeCue_t;

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

struct dds3Basic_t *dds3GetActiveScene(void);

extern sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *ctx, s32 searchId);

extern s32 evtPlaySe(s32 id, s32 fade);

void sdfChipFreeDirect(void *memory);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

struct FileRequest;

struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

s32 fileHandleFree(struct FileRequest *request);

s32 pacFileCheckEnd(const struct FileRequest *request);

enum {
    EVT_PACK_ENTRY_POINT_KIND = 0,
    EVT_PACK_LOAD_COMPLETE = 2
};

extern char D_0054FAC8[];

s32 evtCheckScriptDone(dds3ProcessID_t *task);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void evtWaitEventPackLoad(dds3ProcessID_t *task);

extern void fileSyncAll(void);

extern void Eve_DistortionBlurSetDefTex(void);

extern void Eve_PartialMultiBlurSetDefTex(void);

extern void Eve_RippleBlurSetDefTex(void);

extern void Eve_PartialMultiFilterSetDefTex(void);

dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessFunc_t destroy, void *userValue);

dds3ProcessID_t *evtLoadEventPack(s32 taskId);

void *sdfChipAllocDirect(s32 size);

extern void *memset(void *dst, s32 c, u32 n);

void evtKillEventPack(s32 taskId);

s32 dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);

extern void sprintf(char *, const char *, ...);

struct FileRequest *fileLoad(const char *requestName);

extern char D_0054FAC0[];

extern char D_0056D148[];





dds3ProcessID_t *evtSearchEventPack(u32 taskId) {
    char taskName[32];

    evtMakeEventPackName(taskId, taskName);
    return dds3SearchProcessName((const char *)taskName);
}

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackStat(u32 taskId) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(taskId);
    if (task != 0) {
        evtPackLoadState_t *state = (evtPackLoadState_t *)dds3GetProcessData(task);
        return state->loaded;
    }
    return -1;
}

evtPackLoadState_t *evtGetPackProcessData(u32 taskId) {
    dds3ProcessID_t *task;

    task = evtSearchEventPack(taskId);
    if (task != 0) {
        return (evtPackLoadState_t *)dds3GetProcessData(task);
    }
    return NULL;
}

void *evtGetResorceData(u32 taskId, s32 key) {
    s32 i;
    evtPackLoadState_t *data;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(taskId);
    if (task != 0) {
        data = (evtPackLoadState_t *)dds3GetProcessData(task);
        if (data->loaded != 2) {
            return 0;
        }
        for (i = 0; i < data->header->entryCount; i++) {
            if (data->entries[i].secondaryResourceId == key) {
                return data->payload + data->entries[i].dataOffset;
            }
        }
        return 0;
    }
    return NULL;
}

void func_001FA310(s32 taskId, s32 key) {
    evtPackLoadState_t *data = evtGetPackProcessData(taskId);
    void *address = evtGetResorceData(taskId, key);
    sdfTexHandle_t *texture;
    if (address != 0) {
        sdfTexHandle_t *old = data->effect72;
        if (old != 0) {
            sdfTexFreeDelayed(old);
            data->effect72 = 0;
        }
        texture = sdfTexSet((TMX_HEADER_t *)(address));
        Eve_DistortionBlurSetTex(texture);
        data->effect72 = texture;
    }
}

INCLUDE_ASM(const s32, "event/evtEventPack", evt_1FA398);

/* Decode one retained unit payload and return its new viewer-object key. */
s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId) {
    evtPackLoadState_t *packState;
    evtPackEntry_t *entry;
    s32 packEntryIndex;
    s32 entrySize;
    u8 *modelPayload;
    s32 viewerObjectKey;

    packState = evtGetPackProcessData(eventId);
    packEntryIndex = 0;
    if (packState->header->entryCount > 0) {
        entry = packState->entries;
        do {
            if (entry->secondaryResourceId == resourceId) {
                switch (entry->kind) {
                case 5:
                    entrySize = entry->dataSize;
                    modelPayload = packState->payload + entry->dataOffset;
                    printf("unit decode rid:%d, size:%d\n", resourceId, entrySize);
                    switch (eventId) {
                    case 0x2A4:
                    case 0x2AC:
                    case 0x2AE:
                    case 0x2B0:
                        mdlPackDecode(3, evtMotionSeWork, 0x101, modelPayload, entrySize);
                        break;
                    default:
                        mdlPackDecode(3, evtMotionSeWork, 0x103, modelPayload, entrySize);
                        break;
                    }
                    viewerObjectKey = evtSetRegistUnit(3, evtMotionSeWork);
                    evtMotionSeWork++;
                    if (evtMotionSeWork >= 0x7D0) {
                        evtMotionSeWork = 0x3E8;
                    }
                    printf("BE regist unit %d < %d >\n", evtMotionSeWork, entrySize);
                    return viewerObjectKey;
                }
            }
            entry++;
        } while (++packEntryIndex < packState->header->entryCount);
    }
    return -1;
}

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtRegistBEField(s32 eventId, s32 resourceId) {
    evtPackLoadState_t *packState;
    evtPackEntry_t *entry;
    s32 entryCount;
    s32 remainingEntryCount;
    s32 *areaRoomIds;
    s32 *resource0Data;
    s32 *resource1Data;
    const sdfTextureOffsetListHeader_t *textureOffsets;

    packState = evtGetPackProcessData(eventId);
    areaRoomIds = NULL;
    resource1Data = NULL;
    resource0Data = NULL;
    textureOffsets = NULL;
    entryCount = packState->header->entryCount;
    if (entryCount > 0) {
        entry = packState->entries;
        remainingEntryCount = entryCount;
        do {
            if (entry->resourceId == resourceId) {
                switch (entry->kind) {
                case 2:
                    resource0Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 3:
                    resource1Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 4:
                    textureOffsets = (const sdfTextureOffsetListHeader_t *)(packState->payload + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == resourceId && entry->kind == 1) {
                areaRoomIds = (s32 *)(packState->payload + entry->dataOffset);
            }
            entry++;
        } while (--remainingEntryCount != 0);
    }

    if (areaRoomIds != NULL && resource0Data != NULL && resource1Data != NULL && textureOffsets != NULL) {
        evtLoadArea_FromMemory(areaRoomIds[0], areaRoomIds[1], (s32)resource0Data, (s32)resource1Data, textureOffsets, 0);
        fldFileAddrResetMode(1);
        dds3DrawFlag |= 0x02000000;
        return 1;
    }
    return 0;
}

s32 evtMotionSeProcess1(dds3ProcessID_t *task) {
    evtMotionSeTaskParams_t *motionSeParams;
    dds3Basic_t *motionNode;
    evtPackLoadState_t *packState;
    evtUnit_t *unit;
    u16 motionId;
    s32 motionFrame;
    s32 entryIndex;
    s32 cueIndex;
    evtMotionSeCue_t *cues;

    motionSeParams = (evtMotionSeTaskParams_t *)dds3GetProcessData(task);
    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    motionNode = dds3SearchSceneBasic(dds3GetActiveScene(), motionSeParams->modelKey, EFF_WORLD_KIND_FOLLOW_MODEL);
    if (motionNode == NULL) {
        return -1;
    }
    packState = evtGetPackProcessData(motionSeParams->eventTaskId);
    if (packState == NULL) {
        return -1;
    }
    if (motionSeParams->eventTaskId < 600) {
        return 0;
    }
    unit = ((dds3UnitObjectWork_t *)motionNode->workdata)->unit;
    if (unit == NULL) {
        return -1;
    }
    motionId = mdlGetMotionCtrlr(unit->owner, 0)->cutNumber;
    motionFrame = unit->owner->first->frame;
    for (entryIndex = 0; entryIndex < packState->header->entryCount; entryIndex++) {
        if (packState->entries[entryIndex].kind == 6 &&
            packState->entries[entryIndex].resourceId == motionSeParams->resourceId &&
            packState->entries[entryIndex].motionId == motionId) {
            cues = (evtMotionSeCue_t *)(packState->payload + packState->entries[entryIndex].dataOffset);
            for (cueIndex = 0; cueIndex < packState->entries[entryIndex].cueCount; cueIndex++) {
                if (cues[cueIndex].frame == motionFrame) {
                    evtPlaySe(motionSeParams->eventTaskId, cues[cueIndex].fade);
                    printf("EVE_SE motno = %d, frame = %d\n", motionId, motionFrame);
                }
            }
        }
    }
    return 0;
}

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeScriptProcessData(dds3ProcessID_t *task)
{
    sdfChipFreeDirect((void *)(u32)dds3GetProcessData(task));
}

const char D_0053C668[0x10] __attribute__((aligned(8))) = "mse_%d_%d";

/* Allocate a three-word task parameter block and format its "mse_..." name. */
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId) {
    char taskName[0x20];
    evtMotionSeTaskParams_t *params;

    sprintf(taskName, D_0053C668, eventTaskId, resourceId);
    params = sdfChipAllocDirect(0xC);
    memset(params, 0, 0xC);
    params->modelKey = modelKey;
    params->eventTaskId = eventTaskId;
    params->resourceId = resourceId;
    return dds3StartProcess(taskName, 0x3EC, 0, 0, evtMotionSeProcess1, (dds3ProcessFunc_t)evtFreeScriptProcessData, params);
}

const char D_0053C678[0x28] __attribute__((aligned(8))) = "/event/e%03d/e%03d/scr/e%03d.be";

/* Resolve the event's script path ("/event/eNNN/eNNN/scr/eNNN.be", grouped by tens) and start loading it. */
void evtLoadEventScript(evtPackLoadState_t *state) {
    s32 eventId;
    s32 eventDirectoryId;
    struct FileRequest *request;

    printf(D_0054FAC0);
    eventId = state->eventId;
    eventDirectoryId = eventId - eventId % 10;
    sprintf(D_0056D148, D_0053C678, eventDirectoryId, eventId, eventId);
    request = fileLoad(D_0056D148);
    state->pendingRequest = request;
    state->loaded = 1;
}

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCheckEventScriptLoaded(evtPackLoadState_t *state) {
    evtPackHeader_t *header;
    s32 entryPointIndex;

    if (state->pendingRequest != NULL) {
        if (pacFileCheckEnd(state->pendingRequest) != 0) {
            state->resourceAllocation = fileGetMemHandle((struct FileRequest *)state->pendingRequest);
            fileHandleFree(state->pendingRequest);
            state->pendingRequest = NULL;
            header = (evtPackHeader_t *)sdfMemLock(state->resourceAllocation);
            state->payload = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryPointIndex = 0; entryPointIndex < state->header->entryCount; entryPointIndex++) {
                if (state->entries[entryPointIndex].kind == EVT_PACK_ENTRY_POINT_KIND) {
                    state->entryPoint = state->payload + state->entries[entryPointIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        printf(D_0054FAC8);
        state->loaded = EVT_PACK_LOAD_COMPLETE;
    }
}

s32 evtCheckScriptDone(dds3ProcessID_t *task) {
    evtPackLoadState_t *packState = (evtPackLoadState_t *)dds3GetProcessData(task);

    switch (packState->loaded) {
    default:
        if (packState->loaded < 2) {
            if (packState->loaded == 0) {
                evtLoadEventScript(packState);
            }
        }
        break;
    case 1:
        evtCheckEventScriptLoaded(packState);
        break;
    }
    return 0;
}

/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtWaitEventPackLoad(dds3ProcessID_t *task) {
    void *stateHandle = dds3GetProcessData(task);
    evtPackLoadState_t *packState = (evtPackLoadState_t *)stateHandle;

    fileSyncAll();
    if (stateHandle != 0) {
        if (packState->effect72 != 0) {
            Eve_DistortionBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect72);
        }
        if (packState->effect71 != 0) {
            Eve_PartialMultiBlurSetDefTex();
            sdfTexFreeDelayed((struct sdfTexHandle_t *)packState->effect71);
        }
        if (packState->effect76 != 0) {
            Eve_RippleBlurSetDefTex();
            sdfTexFreeDelayed((struct sdfTexHandle_t *)packState->effect76);
        }
        if (packState->effect75 != 0) {
            Eve_PartialMultiFilterSetDefTex();
            sdfTexFreeDelayed((struct sdfTexHandle_t *)packState->effect75);
        }
        if (packState->pendingRequest != NULL) {
            fileHandleFree(packState->pendingRequest);
        }
        if (packState->resourceAllocation != NULL) {
            sdfMemFreeDelayedDirect(packState->resourceAllocation);
        }
        if (packState->sceneAllocation1 != 0) {
            sdfMemFreeDirect((struct sdfMemHandle_t *)(u32)(packState->sceneAllocation1));
        }
        if (packState->sceneAllocation2 != 0) {
            sdfMemFreeDirect((struct sdfMemHandle_t *)(u32)(packState->sceneAllocation2));
        }
    }
    sdfChipFreeDirect((void *)stateHandle);
}

/* Schedule the event-pack process only if it does not already exist. */
dds3ProcessID_t *evtLoadEventPack(s32 taskId) {
    char taskName[0x20];
    evtPackLoadState_t *taskData;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(taskId);
    if (task == 0) {
        evtMakeEventPackName(taskId, taskName);
        taskData = sdfChipAllocDirect(sizeof(evtPackLoadState_t));
        memset(taskData, 0, sizeof(evtPackLoadState_t));
        taskData->eventId = taskId;
        taskData->loaded = 0;
        task = dds3StartProcess(taskName, DDS3_PRIORITY_EVENT_PACK, 1, 1, evtCheckScriptDone, (dds3ProcessFunc_t)evtWaitEventPackLoad, taskData);
    }
    return task;
}

void evtKillEventPack(s32 taskId) {
    dds3ProcessID_t *taskHandle;

    taskHandle = evtSearchEventPack(taskId);
    if (taskHandle != 0) {
        dds3KillProcess(taskHandle, 0);
        return;
    }
}

/* Drain every event-pack process at the priority used during creation. */
void evt_1FAD40(void) {
    dds3ProcessID_t *task;

    while (task = dds3SearchProcessPriority(DDS3_PRIORITY_EVENT_PACK), task != NULL) {
        dds3KillProcess(task, 0);
    }
}

INCLUDE_ASM(const s32, "event/evtEventPack", cmbEnableMenuEntry2);

INCLUDE_ASM(const s32, "event/evtEventPack", cmbEnableMenuEntry6);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C6A0);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C6C0);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C6E8);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C710);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C738);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C760);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C788);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C7B0);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C7C8);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C7D8);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C7E8);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C800);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C818);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C828);

INCLUDE_RODATA(const s32, "event/evtEventPack", D_0053C840);

INCLUDE_SDATA(const s32, "event/evtEventPack", evtMotionSeWork);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_0054FABC);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_0054FAC0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_0054FAC8);

INCLUDE_SDATA(const s32, "event/evtEventPack", CMB_GBWK);

