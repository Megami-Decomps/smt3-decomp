#ifndef EVT_WORLD_H
#define EVT_WORLD_H

#include "basic.h"

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

#endif /* EVT_WORLD_H */
