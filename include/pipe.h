#ifndef PIPE_H
#define PIPE_H

#include "common.h"

typedef struct tagPipePartsType {
    f32 radius;
    f32 length;
    u32 frame;
} tagPipePartsType;

typedef struct {
    s32 count;
    void *data;
} pipeLinkSource_t;

#endif
