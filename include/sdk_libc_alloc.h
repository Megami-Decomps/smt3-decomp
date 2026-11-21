#ifndef SDK_LIBC_ALLOC_H
#define SDK_LIBC_ALLOC_H

#include "common.h"

typedef struct mallocChunkPrefix_t {
    u32 previousSize;
    u32 size;
    struct mallocChunkPrefix_t *forward;
    struct mallocChunkPrefix_t *backward;
} mallocChunkPrefix_t;

typedef struct mallocBinLinks_t {
    mallocChunkPrefix_t *forward;
    mallocChunkPrefix_t *backward;
} mallocBinLinks_t;

typedef struct mallocArenaPrefix_t {
    u32 opaque00;
    u32 bitmap;
    mallocChunkPrefix_t *top;
    mallocChunkPrefix_t *topBackward; /* bin 0's backward link; its forward link is top */
    mallocBinLinks_t bins[127];
} mallocArenaPrefix_t;

extern mallocArenaPrefix_t __malloc_av_;

#endif
