#ifndef FLDNPC_TAG_H
#define FLDNPC_TAG_H

#include "common.h"

struct dds3Basic_t;

typedef struct fldNpc_tag
{
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    sdfVector4_t rot;
    struct dds3Basic_t *object;
    f32 csize;
    u8 unk28[8];
    void *effect;
    f32 cntf;
    s32 cnti;
    f32 cntAnmf;
    s32 cntAnmi;
    s32 unk44;
    s32 room;
    s32 itemBoxIdx;
    s32 kind;
    s32 camera;
    s32 modelMajor;
    s32 modelIndex;
    char name[0x10];
} fldNpcSlot_t;

#endif
