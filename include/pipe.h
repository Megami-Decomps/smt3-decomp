#ifndef PIPE_H
#define PIPE_H

#include "common.h"

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
