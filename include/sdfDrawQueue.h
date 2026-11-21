#ifndef SDF_DRAW_QUEUE_H
#define SDF_DRAW_QUEUE_H

#include "common.h"

struct sdfDrawTag_t;
struct sdfPreDrawHook_t;

typedef struct sdfDrawReqRec_t {
    struct sdfDrawTag_t *drawTag;
    struct sdfPreDrawHook_t *preDraw;
    u8 drawCount;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} sdfDrawReqRec_t;

extern sdfDrawReqRec_t sdfGrDrawState[];

#endif
