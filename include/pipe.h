#ifndef PIPE_H
#define PIPE_H

#include "common.h"
#include "libsdf.h"

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
