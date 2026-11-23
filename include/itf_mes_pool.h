#ifndef ITF_MES_POOL_H
#define ITF_MES_POOL_H

#include "itfMesManager.h"

typedef struct itfMesLink_t itfMesLink_t;
typedef struct itfMesLinkControl_t itfMesLinkControl_t;
typedef struct itfMesManager_t itfMesManager_t;

/* Nodes are the native 128-entry message pool at manager + 0x20.
 * Each state pointer at node + 0xC is NULL while its window is free. */
struct itfMesLink_t {
    struct itfMesLink_t *previous;
    struct itfMesLink_t *next;
    s32 index;
    instanceMes_tag *state;
    sdfMemHandle_t *resourceHandle;
};

struct itfMesLinkControl_t {
    itfMesLink_t *activeHead;
    itfMesLink_t *activeTail;
    itfMesLink_t *firstFree;
    itfMesLink_t *lastFree;
};

struct itfMesManager_t {
    u32 activeWindowCount;
    sdfTexHandle_t *windowTexture;
    u32 ctr;
    u16 flags;
    u16 unkE;
    itfMesLinkControl_t pool;
    itfMesLink_t nodes[0x80];
};

SIZE_CHECK(itfMesPoolLinkSize, itfMesLink_t, 0x14);
SIZE_CHECK(itfMesPoolControlSize, itfMesLinkControl_t, 0x10);
SIZE_CHECK(itfMesPoolRootSize, itfMesManager_t, 0xA20);

extern itfMesManager_t itfMesMngManager;

/* Pass the complete node array storage, not the first node's byte view. */
void itfMiscLinkInitialize(itfMesLinkControl_t *pool, void *storage, s32 count, s32 stride);
itfMesLink_t *itfMiscLinkTake(itfMesLinkControl_t *pool);
void itfMiscLinkGive(itfMesLink_t *node, itfMesLinkControl_t *pool);

#endif
