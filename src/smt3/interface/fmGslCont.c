#include "font.h"

typedef struct fmFontControl_t fmFontControl_t;

struct fmFontControl_t {
    sdfMemHandle_t *nodeAllocation;
    sdfMemHandle_t *recordStorageAllocation;
    s32 cellWidth; /* 0x08 */
    s32 cellHeight; /* 0x0C */
    s32 pixelByteCount; /* 0x10 */
    s32 capacity; /* 0x14 */
    u32 count; /* 0x18 */
    fmFontNode_t *head; /* 0x1C */
};

extern fmFontControl_t gFGC;
extern s8 D_0054F3FE;
extern u32 D_0054F414;
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
s32 fmDestroyFontMemControl(void);

fmFontControl_t *fmGetInternalFGC(void) {
    return &gFGC;
}

s32 fmGetRestBufNum(void) {
    return gFGC.count;
}

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes) {
    u8 *records = NULL;
    fmFontNode_t *node;
    s32 count;
    s32 i;
    s32 row;
    s32 column;

    if (gFGC.head != NULL && D_0054F3FE != 0) {
        return 0;
    }
    count = columns * rows;
    gFGC.cellWidth = cellWidth;
    gFGC.cellHeight = cellHeight;
    gFGC.count = count;
    gFGC.capacity = count;
    gFGC.pixelByteCount = cellWidth * cellHeight / 2;
    gFGC.nodeAllocation = sdfMemAllocDirect(count * sizeof(fmFontNode_t));
    gFGC.head = (fmFontNode_t *)sdfMemLock(gFGC.nodeAllocation);
    if (recordBytes != 0) {
        gFGC.recordStorageAllocation = sdfMemAllocDirect(recordBytes * gFGC.count);
        records = (u8 *)sdfMemLock(gFGC.recordStorageAllocation);
    }
    for (i = 0; i < gFGC.count - 1; i++) {
        gFGC.head[i].next = &gFGC.head[i + 1];
    }
    gFGC.head[i].next = gFGC.head;
    for (i = gFGC.count - 1; i > 0; i--) {
        gFGC.head[i].prev = &gFGC.head[i - 1];
    }
    gFGC.head[0].prev = &gFGC.head[gFGC.count - 1];
    i = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            node = &gFGC.head[i];
            if (node == NULL) {
                fmDestroyFontMemControl();
                return 0;
            }
            node->nodeIndex = i;
            node->uv.u0 = (column * cellWidth) << 4;
            node->uv.u1 = ((column * cellWidth) << 4) + (cellWidth << 4);
            if (records != NULL) {
                node->item = (frFontRecord_t *)(records + i * recordBytes);
            } else {
                node->item = NULL;
            }
            node->uv.v0 = (row * cellHeight) << 4;
            node->uv.v1 = ((row * cellHeight) << 4) + (cellHeight << 4);
            i++;
        }
    }
    D_0054F3FE = 1;
    return 1;
}

/* Release the group's handles once and clear its active-node flag. */
s32 fmDestroyFontMemControl(void) {
    if (gFGC.head == 0) {
        return 0;
    }
    sdfMemFreeDirect(gFGC.nodeAllocation);
    sdfMemFreeDirect(gFGC.recordStorageAllocation);
    gFGC.head = 0;
    return 1;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
fmFontNode_t *fmRentFontMem(void) {
    fmFontNode_t *resourceNode = gFGC.head->next;

    if (resourceNode->nodeIndex == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    gFGC.count -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}

/* Insert after the list's fixed anchor and update the entry count. */
void fmRetFontMem(fmFontNode_t *node) {
    fmFontNode_t *anchor = gFGC.head;
    fmFontNode_t *nextNode = anchor->next;

    node->prev = anchor;
    node->next = nextNode;
    gFGC.count += 1;
    anchor->next = node;
    nextNode->prev = node;
}

INCLUDE_ASM(const s32, "interface/fmGslCont", frFindTagCode);

INCLUDE_ASM(const s32, "interface/fmGslCont", frFindTagCode2);

INCLUDE_ASM(const s32, "interface/fmGslCont", frGetMessageCode);

INCLUDE_ASM(const s32, "interface/fmGslCont", cmpSetSkillListRows);

INCLUDE_ASM(const s32, "interface/fmGslCont", cmp_1C6180);

INCLUDE_ASM(const s32, "interface/fmGslCont", func_001C61B8);

INCLUDE_ASM(const s32, "interface/fmGslCont", fr_1C6270);

void func_001C62F8(u32 messageCode) {
    D_0054F414 = messageCode;
}

INCLUDE_SDATA(const s32, "interface/fmGslCont", D_0054F3F8);

