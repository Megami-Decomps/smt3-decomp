#include "font.h"

typedef struct FontGsControl_t FontGsControl_t;

struct FontGsControl_t {
    sdfMemHandle_t *mh;
    sdfMemHandle_t *ud_mh;
    s32 unit_width; /* 0x08 */
    s32 unit_height; /* 0x0C */
    s32 unit_size; /* 0x10 */
    s32 unit_num_max; /* 0x14 */
    u32 unit_nums; /* 0x18 */
    FontMem_t *root; /* 0x1C */
};

extern FontGsControl_t gFGC;
extern s8 D_0054F3FE;
extern u32 D_0054F414;
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern u16 D_00325B10[];
extern u16 D_00325E50[];
extern s8 D_00326061[];
extern u16 frMessageCode;
extern s32 frDefaultFontHandle;
extern s32 strlen(const char *s);
extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);
s32 fmDestroyFontMemControl(void);

FontGsControl_t *fmGetInternalFGC(void) {
    return &gFGC;
}

s32 fmGetRestBufNum(void) {
    return gFGC.unit_nums;
}

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes) {
    u8 *records = NULL;
    FontMem_t *node;
    s32 count;
    s32 i;
    s32 row;
    s32 column;

    if (gFGC.root != NULL && D_0054F3FE != 0) {
        return 0;
    }
    count = columns * rows;
    gFGC.unit_width = cellWidth;
    gFGC.unit_height = cellHeight;
    gFGC.unit_nums = count;
    gFGC.unit_num_max = count;
    gFGC.unit_size = cellWidth * cellHeight / 2;
    gFGC.mh = sdfMemAllocDirect(count * sizeof(FontMem_t));
    gFGC.root = (FontMem_t *)sdfMemLock(gFGC.mh);
    if (recordBytes != 0) {
        gFGC.ud_mh = sdfMemAllocDirect(recordBytes * gFGC.unit_nums);
        records = (u8 *)sdfMemLock(gFGC.ud_mh);
    }
    for (i = 0; i < gFGC.unit_nums - 1; i++) {
        gFGC.root[i].next = &gFGC.root[i + 1];
    }
    gFGC.root[i].next = gFGC.root;
    for (i = gFGC.unit_nums - 1; i > 0; i--) {
        gFGC.root[i].prev = &gFGC.root[i - 1];
    }
    gFGC.root[0].prev = &gFGC.root[gFGC.unit_nums - 1];
    i = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            node = &gFGC.root[i];
            if (node == NULL) {
                fmDestroyFontMemControl();
                return 0;
            }
            node->id = i;
            node->uv.u0 = (column * cellWidth) << 4;
            node->uv.u1 = ((column * cellWidth) << 4) + (cellWidth << 4);
            if (records != NULL) {
                node->user_data = (GsFont_t *)(records + i * recordBytes);
            } else {
                node->user_data = NULL;
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
    if (gFGC.root == 0) {
        return 0;
    }
    sdfMemFreeDirect(gFGC.mh);
    sdfMemFreeDirect(gFGC.ud_mh);
    gFGC.root = 0;
    return 1;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FontMem_t *fmRentFontMem(void) {
    FontMem_t *resourceNode = gFGC.root->next;

    if (resourceNode->id == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    gFGC.unit_nums -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}

/* Insert after the list's fixed anchor and update the entry count. */
void fmRetFontMem(FontMem_t *pFontArea) {
    FontMem_t *anchor = gFGC.root;
    FontMem_t *nextNode = anchor->next;

    pFontArea->prev = anchor;
    pFontArea->next = nextNode;
    gFGC.unit_nums += 1;
    anchor->next = pFontArea;
    nextNode->prev = pFontArea;
}

s32 frFindTagCode(u16 code) {
    s32 i = 0;

    do {
        if (D_00325B10[i] == code) {
            return i;
        }
        i++;
    } while (i < 0x1A0);
    return -1;
}

s32 frFindTagCode2(u16 code) {
    s32 i = 0;

    do {
        if (D_00325E50[i] == code) {
            return i;
        }
        i++;
    } while (i < 0x108);
    return -1;
}

void frGetMessageCode(u16 code) {
    if (code == 0x85EC) {
        return;
    }
    if (code < 0x2F) {
        return;
    }
    if ((u16)(code - 0x3A) < 7) {
        return;
    }
    frMessageCode = code;
}

void cmpSetSkillListRows(s32 rows) {
    if (rows <= 0) {
        rows = 20;
    }
    frDefaultFontHandle = rows;
    D_00326061[0] = rows - 5;
}

s32 cmp_1C6180(const char *text, s32 width) {
    s32 length = strlen(text);

    if (length < 0x10) {
        return width;
    }
    return width - length + 0x10;
}

s32 func_001C61B8(frFontHandle_t *chain, s32 limit) {
    frTextHandle_t *glyph = chain->firstChild;
    frTextHandle_t *first = glyph;
    s32 total = 0;
    s32 count = 0;
    s32 step;

    if (glyph != NULL) {
        do {
            total += glyph->advance;
            count++;
            glyph = glyph->next;
            total += chain->glyphCodeOrContext.byteRoles.spacing;
        } while (glyph != NULL);
    }
    if (!(limit < total)) {
        return total;
    }
    step = (total - limit + count - 1) / count;
    total = 0;
    glyph = first;
    if (glyph != NULL) {
        do {
            glyph->advance -= step;
            glyph->cellDimensions.cellWidth -= step;
            total += glyph->advance;
            glyph = glyph->next;
            total += chain->glyphCodeOrContext.byteRoles.spacing;
        } while (glyph != NULL);
    }
    chain->advance = total;
    return total;
}

void fr_1C6270(frFontHandle_t *chain) {
    frTextHandle_t *glyph = chain->firstChild;
    s32 shiftIndex;

    if (glyph != NULL) {
        do {
            if (glyph->glyphCodeOrContext.glyphCode < 0x20) {
                shiftIndex = 0;
            } else if (glyph->glyphCodeOrContext.glyphCode < 0x80) {
                shiftIndex = glyph->glyphCodeOrContext.glyphCode - 0x20;
            } else {
                shiftIndex = ((glyph->glyphCodeOrContext.glyphCode - 0x8080) & 0xFF00) / 2 +
                             ((glyph->glyphCodeOrContext.glyphCode - 0x8080) & 0x7F);
            }
            frGetXShiftLev(glyph, shiftIndex, 0);
            glyph = glyph->next;
        } while (glyph != NULL);
    }
}

void func_001C62F8(u32 messageCode) {
    D_0054F414 = messageCode;
}

INCLUDE_SDATA(const s32, "interface/fmGslCont", D_0054F3F8);

