#ifndef FLDTAKARAWORK_TAG_H
#define FLDTAKARAWORK_TAG_H

#include "common.h"

struct dds3Basic_t;

typedef struct fldTakaraWork_tag
{
    struct dds3Basic_t *object;
    s32 id;
    s32 activationState;
    s32 activationRequested;
    f32 x;
    f32 y;
    f32 z;
    f32 angle;
    s32 mode;
    void *effect;
    s32 tableIndex;
    u8 pad2C[4];
} fldTakaraSlot_t;

#endif
