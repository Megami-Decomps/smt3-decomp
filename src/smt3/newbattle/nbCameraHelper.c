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
} nbCameraHelper_effFileListHandle_t;

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
    nbCameraHelper_effFileListHandle_t *resourceList;
} nbCameraHelper_effFLWinHandle_t;

extern s8 dds3PadBitTable[];
extern s8 D_005298CB[];
extern char D_0054F268[];
extern sdfGrOt_t sdfGrOtTable[];
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
#include "sdk_libc.h"

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);
extern sdfTexHandle_t *sdfTexSet(void *aTex);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern void *sdfGrAllocPacketBuff(u32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);
extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);
extern void dds3AdminiCallSequence(s32 sequence, s32 data, s32 datasize, s32 restore);
extern u8 sdfHddValid;
extern u32 strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern s32 dds3EffectDirOpen(void *name);
extern void dds3EffectDirClose(s32 directory);
extern void *sdf2DAllocSprite(s32 aNumOfSpr);
extern void *sdf2DGetPrimPtr(void *aPacketPtr);
/* The provider returns the linked packet address, even when ignored here. */
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

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
void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *resource);
void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work);

/* SDK directory entry filled by the directory-read provider. */
typedef struct
{
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} dds3EffectDirEnt_t;

typedef enum EffResourceKind {
    EFF_RESOURCE_KIND_NONE = 0, EFF_RESOURCE_KIND_TMX = 1, EFF_RESOURCE_KIND_P2A = 2,
    EFF_RESOURCE_KIND_D3P = 4, EFF_RESOURCE_KIND_BED = 8, EFF_RESOURCE_KIND_PB = 0x10,
    EFF_RESOURCE_KIND_PCF = 0x20
} EffResourceKind;

extern s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry);

#define EFF_BUILTIN_NAME_COUNT 0x2F
#define EFF_DIRECTORY_FLAG_CLEAR 0x1000
#define EFF_DIRECTORY_PATH_BYTES 0x70
extern char *dds3EffectFileNameTable[];
extern char D_0054F260[];
extern u32 D_00551DE4;
extern s32 sceDopen(void *path);
extern s32 sceDclose(s32 directory);
extern s32 sceDread(s32 directory, dds3EffectDirEnt_t *entry);

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfHddValid != 0) {
        sprintf(path, D_0054F260, name);
        return sceDopen(path);
    } else {
        D_00551DE4 = 0;
        return 0;
    }
}

/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory) {
    if (sdfHddValid != 0) {
        sceDclose(directory);
    }
}

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry) {
    if (sdfHddValid != 0) {
        return sceDread(directory, entry);
    }
    if ((u32)D_00551DE4 >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, dds3EffectFileNameTable[D_00551DE4]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    D_00551DE4++;
    return strlen(entry->name);
}

/* One node per file whose extension matches a requested type bit. The 0x10
 * types use a two-character extension, so the dot sits one byte later. */
nbCameraHelper_effFileListHandle_t *dds3EffectFileListCreate(char *path, s32 typeMask) {
    dds3EffectDirEnt_t entry;
    s32 directory;
    nbCameraHelper_effFileListHandle_t *root;
    dds3EffectFileNode_t *node;
    dds3EffectFileNode_t *previous;
    s32 length;
    EffResourceKind type;
    directory = dds3EffectDirOpen(path);
    root = sdfChipAllocDirect(sizeof(*root));
    if (sdfHddValid != 0) {
        root->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    } else {
        root->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    }
    previous = NULL;
    root->head = NULL;
    root->resourceCount = 0;
    while ((length = dds3EffectDirRead(directory, &entry)) > 0) {
        if ((entry.flags & 0xF000) == 0x1000) continue;
        if (length < 5) continue;
        if (length >= 0x2F) continue;
        if ((typeMask & 0x10 ? entry.name[length - 3] : entry.name[length - 4]) != '.') continue;
        type = EFF_RESOURCE_KIND_NONE;
        if ((typeMask & 1) &&
            ((entry.name[length-3]=='t' && entry.name[length-2]=='m' && entry.name[length-1]=='x') ||
             (entry.name[length-3]=='T' && entry.name[length-2]=='M' && entry.name[length-1]=='X'))) type = EFF_RESOURCE_KIND_TMX;
        else if ((typeMask & 2) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='2' && entry.name[length-1]=='a') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='2' && entry.name[length-1]=='A'))) type = EFF_RESOURCE_KIND_P2A;
        else if ((typeMask & 4) &&
            ((entry.name[length-3]=='d' && entry.name[length-2]=='3' && entry.name[length-1]=='p') ||
             (entry.name[length-3]=='D' && entry.name[length-2]=='3' && entry.name[length-1]=='P'))) type = EFF_RESOURCE_KIND_D3P;
        else if ((typeMask & 8) &&
            ((entry.name[length-3]=='b' && entry.name[length-2]=='e' && entry.name[length-1]=='d') ||
             (entry.name[length-3]=='B' && entry.name[length-2]=='E' && entry.name[length-1]=='D'))) type = EFF_RESOURCE_KIND_BED;
        else if ((typeMask & 0x10) &&
            ((entry.name[length-2]=='p' && entry.name[length-1]=='b') ||
             (entry.name[length-2]=='P' && entry.name[length-1]=='B'))) type = EFF_RESOURCE_KIND_PB;
        else if ((typeMask & 0x20) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='c' && entry.name[length-1]=='f') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='C' && entry.name[length-1]=='F'))) type = EFF_RESOURCE_KIND_PCF;
        if (type == EFF_RESOURCE_KIND_NONE) continue;
        node = sdfChipAllocDirect(sizeof(*node));
        node->type = type;
        strcpy(node->name, entry.name);
        if (previous != NULL) previous->next = node;
        else root->head = node;
        node->next = NULL;
        node->previous = previous;
        root->resourceCount++;
        previous = node;
    }
    dds3EffectDirClose(directory);
    return root;
}

void dds3EffectFileListDelete(nbCameraHelper_effFileListHandle_t *a) {
    dds3EffectFileNode_t *node = a->head;

    if (node != NULL) {
        do {
            dds3EffectFileNode_t *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
    }
    sdfChipFreeDirect(a->directoryPath);
    sdfChipFreeDirect(a);
}

nbCameraHelper_effFLWinHandle_t *dds3EffectFileListWindowCreate(nbCameraHelper_effFileListHandle_t *a) {
    nbCameraHelper_effFLWinHandle_t *resource = sdfChipAllocDirect(sizeof(*resource));

    resource->x = 0;
    resource->y = 0xC8;
    resource->result = 0;
    resource->resourceCount = a->resourceCount;
    resource->unk10 = 0;
    resource->selectedIndex = 0;
    resource->visibleIndex = 0;
    resource->previewActive = 0;
    resource->repeatDelay = 0;
    resource->drawSurface = 0x47;
    resource->borderColor = 0x40806020;
    resource->fillColor = 0x30000000;
    resource->firstVisibleEntry = a->head;
    resource->selectedEntry = a->head;
    resource->cachedEntry = 0;
    resource->textureHandle = 0;
    resource->resourceList = a;
    return resource;
}

s32 dds3EffectFileListWindowDraw(nbCameraHelper_effFLWinHandle_t *a) {
    dds3EffectFileNode_t *visibleEntry;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;
    s32 drawnRows;
    s32 contentRow;

    if (a->repeatDelay != 0) {
        a->repeatDelay--;
    } else if (a->firstVisibleEntry == NULL) {
        if (D_005298CB[0] < 0) {
            a->result = 2;
        }
    } else if (a->result == 0) {
        dds3EffectFileNode_t *navigationEntry;
        dds3EffectFileNode_t *selectedEntry;
        s32 navigationStep;
        if ((dds3PadBitTable[6] & 2) != 0) {
            if (a->selectedEntry->previous != NULL) {
                a->selectedEntry = a->selectedEntry->previous;
                a->selectedIndex--;
                if (a->visibleIndex != 0) {
                    a->visibleIndex--;
                }
                if (a->selectedEntry == a->firstVisibleEntry) {
                    if (a->selectedEntry->previous != NULL) {
                        a->firstVisibleEntry = a->selectedEntry->previous;
                        a->visibleIndex = 1;
                    }
                }
            }
        } else if ((dds3PadBitTable[7] & 2) != 0) {
            if (a->selectedEntry->next != NULL) {
                a->selectedEntry = a->selectedEntry->next;
                a->selectedIndex++;
                a->visibleIndex++;
                if (a->visibleIndex == 0xE) {
                    if (a->firstVisibleEntry->next != NULL) {
                        a->firstVisibleEntry = a->firstVisibleEntry->next;
                        a->visibleIndex = 0xD;
                    }
                }
            }
        } else if ((dds3PadBitTable[9] & 2) != 0) {
            selectedEntry = a->selectedEntry;
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = selectedEntry->previous;
                if (navigationEntry != NULL) {
                    a->selectedEntry = navigationEntry;
                    a->selectedIndex--;
                    if (a->visibleIndex != 0) {
                        a->visibleIndex--;
                    }
                    selectedEntry = a->selectedEntry;
                    if (selectedEntry == a->firstVisibleEntry) {
                        dds3EffectFileNode_t *previous = selectedEntry->previous;
                        if (previous != NULL) {
                            a->firstVisibleEntry = previous;
                            a->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((dds3PadBitTable[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = a->selectedEntry->next;
                if (navigationEntry != NULL) {
                    a->selectedEntry = navigationEntry;
                    a->selectedIndex++;
                    a->visibleIndex++;
                    if (a->visibleIndex == 0xE) {
                        dds3EffectFileNode_t *next = a->firstVisibleEntry->next;
                        if (next != NULL) {
                            a->firstVisibleEntry = next;
                            a->visibleIndex = 0xD;
                        }
                    }
                }
            }
        } else if (dds3PadBitTable[3] < 0) {
            a->result = 2;
        } else if (dds3PadBitTable[1] < 0) {
            a->result = 1;
        } else {
            dds3EffectFileNode_t *cachedEntry = a->cachedEntry;
            selectedEntry = a->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category = selectedEntry->type;
                switch (category) {
                case 1: {
                    char resourceName[0x70];
                    sprintf(resourceName, D_0054F268,
                        a->resourceList->directoryPath, selectedEntry->name);
                    dds3EffectListSetTexture(a, resourceName);
                    a->previewActive = category;
                    selectedEntry = a->selectedEntry;
                    break;
                }
                case 2:
                    a->previewActive = 0;
                    break;
                case 4:
                    a->previewActive = 0;
                    break;
                default:
                    a->previewActive = 0;
                    break;
                }
                a->cachedEntry = selectedEntry;
            }
            if (a->previewActive != 0) {
                dds3EffectFileListWindowDrawSprite(a);
            }
        }
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
        0x8000 - (a->x << 4), 0x8000 - (a->y << 3),
        0xFEFFFF, 0xC00, 0x620, a->fillColor, a->borderColor));
    if (a->previewActive != 0) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
            0x8000 - (a->x << 4),
            0x8000 - ((a->y - 0xC4) << 3),
            0xFEFFFF, 0x800, 0x400, 0, 0x40806020));
    }
    contentRow = a->y - 2;
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
        0x8000 - ((a->x - 2) << 4),
        0x8000 - (contentRow << 3), 0xFF0000, 5,
        a->resourceList->directoryPath));
    contentRow -= 0xC;
    visibleEntry = a->firstVisibleEntry;
    drawnRows = 0;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == a->selectedEntry ? 4 : 0;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x8000 - ((a->x - 2) << 4),
            0x8000 - (contentRow << 3), 0xFF0000,
            selectionFlags, visibleEntry->name));
        drawnRows++;
        contentRow -= 0xC;
        if (drawnRows >= 0xF) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }
    surface = &sdfGrOtTable[a->drawSurface];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, packetList);
    return a->result;
}

void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a) {
    sdfTexHandle_t *texture = a->textureHandle;

    if (texture != NULL) {
        sdfTexFreeDelayed(texture);
        a->textureHandle = NULL;
    }
    sdfChipFreeDirect(a);
}

void dds3EffectFileListWindowSetPos(nbCameraHelper_effFLWinHandle_t *a, s32 b, s32 c) {
    a->x = b;
    a->y = c;
}

u32 dds3_1AB818(nbCameraHelper_effFLWinHandle_t *work) {
    return work->selectedIndex;
}

u32 dds3EffectFileListWindowGetState(nbCameraHelper_effFLWinHandle_t *a) {
    return a->result;
}

u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b) {
    sprintf(b, D_0054F268, a->resourceList->directoryPath, a->selectedEntry->name);
    return a->selectedEntry->type;
}

void dds3EffectFileListWindowSetOt(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 ot) {
    hEffFLWin->repeatDelay = ot;
}

void dds3EffectFileListWindowSetPadWait(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 wait) {
    hEffFLWin->drawSurface = wait;
}

void dds3EffectFileListWindowSetWindowColor(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 frame_c, u32 bg_c) {
    hEffFLWin->borderColor = frame_c;
    hEffFLWin->fillColor = bg_c;
}

void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *textureResource) {
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

void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work)
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
    vertex->b = 0x80;
    vertex->g = 0x80;
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

void InitParticleEditorProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void InitSpriteViewerProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

INCLUDE_SDATA(const s32, "newbattle/nbCameraHelper", D_0054F260);

INCLUDE_SDATA(const s32, "newbattle/nbCameraHelper", D_0054F268);

