#ifndef PIPE_H
#define PIPE_H

#include "common.h"
#include "libsdf.h"

typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    u8 id;
    u8 action;
    u8 enabled;
    u8 mdl;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct tagPipePosData {
    f32 position[3];
    u32 opaque0C;
    f32 direction[3];
    u32 opaque1C;
} tagPipePosData;

typedef struct tagPipePosTable {
    sdfMemHandle_t *aHandle;
    s32 tSize;
    tagPipePosData *points;
    u32 opaque0C;
} tagPipePosTable;

typedef struct tagPipeLinkData {
    u16 id;
    s16 partsID;
    struct tagPipeLinkData *next;
    struct tagPipeLinkData *prev;
    u8 unk0C[4];
} tagPipeLinkData;


typedef struct tagPipeLinkInfo {
    sdfMemHandle_t *aHandle;
    tagPipeLinkData *head;
    s32 maxNum;
    s32 nextId;
} tagPipeLinkInfo;

typedef struct tagPipePartsType {
    f32 radius;
    f32 length;
    u32 frame;
} tagPipePartsType;


typedef struct tagPipePartsInfo {
    u16 partsID;
    u8 inWay;
    u8 goWay;
    tagPipePartsType *data;
} tagPipePartsInfo;

typedef struct {
    s32 count;
    void *data;
} pipeLinkSource_t;

#endif
