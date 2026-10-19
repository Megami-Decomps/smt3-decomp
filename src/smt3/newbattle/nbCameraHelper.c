#include "common.h"
#include "libsdf.h"

/* Owned file list: names stay inline in each 0x3C-byte node. */
typedef struct dds3EffectFileNode_t
{
    s32 type;
    char name[0x30];
    struct dds3EffectFileNode_t *previous;
    struct dds3EffectFileNode_t *next;
} dds3EffectFileNode_t;

typedef struct
{
    s32 resourceCount;
    char *directoryPath;
    dds3EffectFileNode_t *head;
} dds3EffectFileList_t;

/* Complete 0x44-byte browser owner, including its retained preview texture. */
typedef struct
{
    s32 x;
    s32 y;
    s32 result;
    s32 resourceCount;
    u32 unk10;
    s32 selectedIndex;
    s32 visibleIndex;
    u32 previewActive;
    u32 repeatDelay;
    u32 drawSurface;
    u32 borderColor;
    u32 fillColor;
    dds3EffectFileNode_t *firstVisibleEntry;
    dds3EffectFileNode_t *selectedEntry;
    dds3EffectFileNode_t *cachedEntry;
    sdfTexHandle_t *textureHandle;
    dds3EffectFileList_t *resourceList;
} dds3EffectFileWindow_t;

extern s8 dds3PadBitTable[];
extern s8 D_005298CB[];
extern char D_0054F268[];
extern sdfGrOt_t sdfGrOtTable[];
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern s32 sprintf(char *destination, const char *format, ...);
extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);
extern sdfMemHandle_t *sdfDevLoadFileEx(char *name, void *topPtr, s32 *sizePtr);
extern sdfTexHandle_t *sdfTexSet(void *data);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *dds3PrimFrameRect(s32 x, s32 y, s32 z, s32 width, s32 height, u32 fill, u32 border);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 flags, const char *format, ...);
extern void dds3AdminiCallSequence(s32 sequence, s32 data, s32 bytes, s32 flags);
extern u8 sdfHddValid;
extern u32 strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern s32 dds3EffectDirOpen(void *name);
extern void dds3EffectDirClose(s32 directory);
extern void *sdf2DAllocSprite(s32 loopCount);
extern void *sdf2DGetPrimPtr(void *packet);
extern void sdf2DLinkTexPacket(sdfDrawTag_t *tag, sdfTexHandle_t *texture, s32 gsContext);

/* One textureless sprite as sdf2D lays it out: the colour, then two corners
 * of 0x20 bytes each. */
typedef struct
{
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} dds3EffectSpriteCorner_t;

typedef struct
{
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    dds3EffectSpriteCorner_t corner[2];
} dds3EffectSpriteVertex_t;
void dds3EffectListSetTexture(dds3EffectFileWindow_t *work, char *resource);
void dds3EffectFileListWindowDrawSprite(dds3EffectFileWindow_t *work);

/* SDK directory entry filled by the directory-read provider. */
typedef struct
{
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} dds3EffectDirEnt_t;

/* One node per file whose extension matches a requested type bit. The 0x10
 * types use a two-character extension, so the dot sits one byte later. */
/* NON_MATCHING: 6 of 270 words differ (first at +0xE4). */
#ifdef NON_MATCHING
dds3EffectFileList_t *dds3EffectFileListCreate(char *path, s32 typeMask)
{
    dds3EffectDirEnt_t entry;
    dds3EffectFileList_t *root;
    dds3EffectFileNode_t *previous;
    dds3EffectFileNode_t *node;
    s32 directory;
    s32 length;
    s32 type;
    char dot;
    char period;

    directory = dds3EffectDirOpen(path);
    root = (dds3EffectFileList_t *)sdfChipAllocDirect(0xC);
    if (sdfHddValid != 0) {
        root->directoryPath = (char *)sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    } else {
        root->directoryPath = (char *)sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    }
    previous = NULL;
    root->head = NULL;
    root->resourceCount = 0;
    while ((length = dds3EffectDirRead(directory, &entry)) > 0) {
        if ((entry.flags & 0xF000) == 0x1000) {
            continue;
        }
        if (length < 5) {
            continue;
        }
        if (length >= 0x2F) {
            continue;
        }
        if (typeMask & 0x10) {
            period = '.';
            dot = entry.name[length - 3];
        } else {
            period = '.';
            dot = entry.name[length - 4];
        }
        if (dot != period) {
            continue;
        }
        type = 0;
        if ((typeMask & 1) &&
            ((entry.name[length - 3] == 't' && entry.name[length - 2] == 'm' && entry.name[length - 1] == 'x') ||
             (entry.name[length - 3] == 'T' && entry.name[length - 2] == 'M' && entry.name[length - 1] == 'X'))) {
            type = 1;
        } else if ((typeMask & 2) &&
            ((entry.name[length - 3] == 'p' && entry.name[length - 2] == '2' && entry.name[length - 1] == 'a') ||
             (entry.name[length - 3] == 'P' && entry.name[length - 2] == '2' && entry.name[length - 1] == 'A'))) {
            type = 2;
        } else if ((typeMask & 4) &&
            ((entry.name[length - 3] == 'd' && entry.name[length - 2] == '3' && entry.name[length - 1] == 'p') ||
             (entry.name[length - 3] == 'D' && entry.name[length - 2] == '3' && entry.name[length - 1] == 'P'))) {
            type = 4;
        } else if ((typeMask & 8) &&
            ((entry.name[length - 3] == 'b' && entry.name[length - 2] == 'e' && entry.name[length - 1] == 'd') ||
             (entry.name[length - 3] == 'B' && entry.name[length - 2] == 'E' && entry.name[length - 1] == 'D'))) {
            type = 8;
        } else if ((typeMask & 0x10) &&
            ((entry.name[length - 2] == 'p' && entry.name[length - 1] == 'b') ||
             (entry.name[length - 2] == 'P' && entry.name[length - 1] == 'B'))) {
            type = 0x10;
        } else if ((typeMask & 0x20) &&
            ((entry.name[length - 3] == 'p' && entry.name[length - 2] == 'c' && entry.name[length - 1] == 'f') ||
             (entry.name[length - 3] == 'P' && entry.name[length - 2] == 'C' && entry.name[length - 1] == 'F'))) {
            type = 0x20;
        }
        if (type == 0) {
            continue;
        }
        node = (dds3EffectFileNode_t *)sdfChipAllocDirect(0x3C);
        node->type = type;
        strcpy(node->name, entry.name);
        if (previous != NULL) {
            previous->next = node;
        } else {
            root->head = node;
        }
        node->next = NULL;
        node->previous = previous;
        root->resourceCount++;
        previous = node;
    }
    dds3EffectDirClose(directory);
    return root;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListCreate);
#endif

void dds3EffectFileListDelete(dds3EffectFileList_t *root) {
    dds3EffectFileNode_t *node = root->head;

    if (node != NULL) {
        do {
            dds3EffectFileNode_t *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
    }
    sdfChipFreeDirect(root->directoryPath);
    sdfChipFreeDirect(root);
}

dds3EffectFileWindow_t *dds3EffectFileListWindowCreate(dds3EffectFileList_t *list) {
    dds3EffectFileWindow_t *resource = sdfChipAllocDirect(sizeof(*resource));

    resource->x = 0;
    resource->y = 0xC8;
    resource->result = 0;
    resource->resourceCount = list->resourceCount;
    resource->unk10 = 0;
    resource->selectedIndex = 0;
    resource->visibleIndex = 0;
    resource->previewActive = 0;
    resource->repeatDelay = 0;
    resource->drawSurface = 0x47;
    resource->borderColor = 0x40806020;
    resource->fillColor = 0x30000000;
    resource->firstVisibleEntry = list->head;
    resource->selectedEntry = list->head;
    resource->cachedEntry = 0;
    resource->textureHandle = 0;
    resource->resourceList = list;
    return resource;
}

s32 dds3EffectFileListWindowDraw(dds3EffectFileWindow_t *descriptor) {
    dds3EffectFileNode_t *visibleEntry;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;
    s32 drawnRows;
    s32 contentRow;

    if (descriptor->repeatDelay != 0) {
        descriptor->repeatDelay--;
    } else if (descriptor->firstVisibleEntry == NULL) {
        if (D_005298CB[0] < 0) {
            descriptor->result = 2;
        }
    } else if (descriptor->result == 0) {
        dds3EffectFileNode_t *navigationEntry;
        dds3EffectFileNode_t *selectedEntry;
        s32 navigationStep;
        if ((dds3PadBitTable[6] & 2) != 0) {
            if (descriptor->selectedEntry->previous != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->previous;
                descriptor->selectedIndex--;
                if (descriptor->visibleIndex != 0) {
                    descriptor->visibleIndex--;
                }
                if (descriptor->selectedEntry == descriptor->firstVisibleEntry) {
                    if (descriptor->selectedEntry->previous != NULL) {
                        descriptor->firstVisibleEntry = descriptor->selectedEntry->previous;
                        descriptor->visibleIndex = 1;
                    }
                }
            }
        } else if ((dds3PadBitTable[7] & 2) != 0) {
            if (descriptor->selectedEntry->next != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->next;
                descriptor->selectedIndex++;
                descriptor->visibleIndex++;
                if (descriptor->visibleIndex == 0xE) {
                    if (descriptor->firstVisibleEntry->next != NULL) {
                        descriptor->firstVisibleEntry = descriptor->firstVisibleEntry->next;
                        descriptor->visibleIndex = 0xD;
                    }
                }
            }
        } else if ((dds3PadBitTable[9] & 2) != 0) {
            selectedEntry = descriptor->selectedEntry;
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = selectedEntry->previous;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex--;
                    if (descriptor->visibleIndex != 0) {
                        descriptor->visibleIndex--;
                    }
                    selectedEntry = descriptor->selectedEntry;
                    if (selectedEntry == descriptor->firstVisibleEntry) {
                        dds3EffectFileNode_t *previous = selectedEntry->previous;
                        if (previous != NULL) {
                            descriptor->firstVisibleEntry = previous;
                            descriptor->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((dds3PadBitTable[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = descriptor->selectedEntry->next;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex++;
                    descriptor->visibleIndex++;
                    if (descriptor->visibleIndex == 0xE) {
                        dds3EffectFileNode_t *next = descriptor->firstVisibleEntry->next;
                        if (next != NULL) {
                            descriptor->firstVisibleEntry = next;
                            descriptor->visibleIndex = 0xD;
                        }
                    }
                }
            }
        } else if (dds3PadBitTable[3] < 0) {
            descriptor->result = 2;
        } else if (dds3PadBitTable[1] < 0) {
            descriptor->result = 1;
        } else {
            dds3EffectFileNode_t *cachedEntry = descriptor->cachedEntry;
            selectedEntry = descriptor->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category = selectedEntry->type;
                switch (category) {
                case 1: {
                    char resourceName[0x70];
                    sprintf(resourceName, D_0054F268,
                        descriptor->resourceList->directoryPath, selectedEntry->name);
                    dds3EffectListSetTexture(descriptor, resourceName);
                    descriptor->previewActive = category;
                    selectedEntry = descriptor->selectedEntry;
                    break;
                }
                case 2:
                    descriptor->previewActive = 0;
                    break;
                case 4:
                    descriptor->previewActive = 0;
                    break;
                default:
                    descriptor->previewActive = 0;
                    break;
                }
                descriptor->cachedEntry = selectedEntry;
            }
            if (descriptor->previewActive != 0) {
                dds3EffectFileListWindowDrawSprite(descriptor);
            }
        }
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
        0x8000 - (descriptor->x << 4), 0x8000 - (descriptor->y << 3),
        0xFEFFFF, 0xC00, 0x620, descriptor->fillColor, descriptor->borderColor));
    if (descriptor->previewActive != 0) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
            0x8000 - (descriptor->x << 4),
            0x8000 - ((descriptor->y - 0xC4) << 3),
            0xFEFFFF, 0x800, 0x400, 0, 0x40806020));
    }
    contentRow = descriptor->y - 2;
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
        0x8000 - ((descriptor->x - 2) << 4),
        0x8000 - (contentRow << 3), 0xFF0000, 5,
        descriptor->resourceList->directoryPath));
    contentRow -= 0xC;
    visibleEntry = descriptor->firstVisibleEntry;
    drawnRows = 0;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == descriptor->selectedEntry ? 4 : 0;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x8000 - ((descriptor->x - 2) << 4),
            0x8000 - (contentRow << 3), 0xFF0000,
            selectionFlags, visibleEntry->name));
        drawnRows++;
        contentRow -= 0xC;
        if (drawnRows >= 0xF) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }
    surface = &sdfGrOtTable[descriptor->drawSurface];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, packetList);
    return descriptor->result;
}

void dds3EffectFileListWindowDelete(dds3EffectFileWindow_t *work) {
    sdfTexHandle_t *texture = work->textureHandle;

    if (texture != NULL) {
        sdfTexFreeDelayed(texture);
        work->textureHandle = NULL;
    }
    sdfChipFreeDirect(work);
}

void dds3EffectFileListWindowSetPos(dds3EffectFileWindow_t *message, s32 x, s32 y) {
    message->x = x;
    message->y = y;
}

u32 dds3_1AB818(dds3EffectFileWindow_t *work) {
    return work->selectedIndex;
}

u32 dds3EffectFileListWindowGetState(dds3EffectFileWindow_t *work) {
    return work->result;
}

u32 dds3EffectFileListWindowGetFilename(dds3EffectFileWindow_t *message, char *destination) {
    sprintf(destination, D_0054F268, message->resourceList->directoryPath, message->selectedEntry->name);
    return message->selectedEntry->type;
}

void dds3EffectFileListWindowSetOt(dds3EffectFileWindow_t *work, u32 value) {
    work->repeatDelay = value;
}

void dds3EffectFileListWindowSetPadWait(dds3EffectFileWindow_t *work, u32 value) {
    work->drawSurface = value;
}

void dds3EffectFileListWindowSetWindowColor(dds3EffectFileWindow_t *message, u32 borderColor, u32 fillColor) {
    message->borderColor = borderColor;
    message->fillColor = fillColor;
}

void dds3EffectListSetTexture(dds3EffectFileWindow_t *work, char *textureResource) {
    sdfTexHandle_t *texture;
    sdfMemHandle_t *loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != NULL) {
        sdfTexFreeDelayed(work->textureHandle);
        work->textureHandle = NULL;
    }
    loadedResource = sdfDevLoadFileEx(textureResource, resourceWords, 0);
    texture = sdfTexSet((void *)(resourceWords[0]));
    work->textureHandle = texture;
    sdfMemFreeDirect(loadedResource);
}

/* NON_MATCHING: 2 of 70 words differ (first at +0x7C). */
#ifdef NON_MATCHING
void dds3EffectFileListWindowDrawSprite(dds3EffectFileWindow_t *work)
{
    sdfDrawTag_t *list;
    void *sprite;
    dds3EffectSpriteVertex_t *vertex;
    s32 left;
    s32 top;

    if (work->textureHandle == NULL) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sprite = sdf2DAllocSprite(1);
    vertex = (dds3EffectSpriteVertex_t *)sdf2DGetPrimPtr(sprite);
    vertex->a = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->r = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[1].u = work->textureHandle->width << 4;
    vertex->corner[1].v = work->textureHandle->height << 4;
    left = 0x8000 - (work->x << 4);
    vertex->corner[0].x = left;
    left += 0x800;
    top = 0x8000 - ((work->y - 0xC4) << 3);
    vertex->corner[0].y = top;
    vertex->corner[0].mask = 0xFF0000;
    vertex->corner[0].flag = 0;
    vertex->corner[1].x = left;
    vertex->corner[1].y = top + 0x400;
    vertex->corner[1].mask = 0xFF0000;
    vertex->corner[1].flag = 0;
    sdf2DLinkTexPacket(list, work->textureHandle, 0);
    sdfDrawTagLink(list, (u32)sprite);
    {
        sdfGrOt_t *ot = &sdfGrOtTable[71];

        ot->regFunc(ot, list);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListWindowDrawSprite);
#endif

void InitParticleEditorProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void InitSpriteViewerProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

INCLUDE_SDATA(const s32, "newbattle/nbCameraHelper", D_0054F268);

