#include "common.h"
#include "basic.h"


typedef struct EffUnitObject {
    f32 localMatrix[4][4];
    f32 transform[4][4];
    struct BillObj *billboard;
    void *resource;
} EffUnitObject;

typedef struct BillKindFuncs {
    struct BillObj *(*func)();
    void (*callback)();
    void (*destroy)();
} BillKindFuncs;

extern BillKindFuncs dds3BillboardFuncTable[];
extern struct BillObj *hGeneralBillboard[];

void *sdfMatNew(void);
s32 printf(const char *fmt, ...);

struct BillObj *dds3Billboard_NormalCreate(void *data);
u32 dds3Billboard_P2AColorMul(u32 color, u32 scale);
void dds3MultiBillboardTagLink(struct BillObj *obj, void *pair);


struct SdfMemBlock;
typedef struct SdfMemBlock SdfMemBlock;
struct BillPacketWork;
typedef struct BillPacketWork BillPacketWork;
struct SdfListHead;
typedef struct SdfListHead SdfListHead;
struct SdfTexBuf;
typedef struct SdfTexBuf SdfTexBuf;
struct SdfTexRef;
typedef struct SdfTexRef SdfTexRef;
struct SdfTexResource;
typedef struct SdfTexResource SdfTexResource;

typedef struct BattleEffect BattleEffect;

typedef struct BillTextureQuad {
    u16 components[8];
} BillTextureQuad;

typedef u8 SdfTextureClampMode;

typedef struct SdfTex {
    struct SdfTex *next;
    struct SdfTex *prev;
    SdfTexRef *reference;
    s16 width;
    s16 height;
    SdfTexResource *primaryResource;
    SdfTexResource *secondaryResource;
    u8 paletteCount;
    u8 clutFormat;
    u8 pixelFormat;
    u8 maxMipLevel;
    u16 lodParameters;  
    u8 unk1E;
    SdfTextureClampMode clampMode;  
    s32 resourceKey;
    s32 battleTextureSlot;
    SdfTexBuf *primaryBuffer;
    SdfTexBuf *secondaryBuffer;
    u8 *paletteData;  
    s32 paletteDataSize;  
    s32 unk38;
    u8 *intensityMap;  
} SdfTex;

typedef struct BillChildPayload {
    SdfTex *texture;  
    u16 variant;  
    u8 pad06[2];
    s32 refCount;       
    BillTextureQuad uv;  
    f32 x;              
    f32 y;              
    f32 halfWidth;   
    f32 halfHeight;  
    SdfListHead *pendingLists[5];  
    u16 packetListIndex;  
    u8 pad42[2];
    struct SdfMemBlock *allocation;  
    BillPacketWork *work;  
    struct BillChildPayload *next;  
} BillChildPayload;

typedef struct BillAnimationEntry {
    s32 offset;
    s32 colorOffset;
    u32 entryModeFlags;  
    u32 frameCount;
    u32 flags;
} BillAnimationEntry;

typedef struct BillData {
    struct SdfMemBlock *allocation;  
    u8 *base;
    BillAnimationEntry *entries;
    s32 entryCount;
    s32 childCount;
    s32 listRefCount;
    struct BillChildPayload **children;
} BillData;

typedef struct BillRecord {
    s16 width;
    s16 height;
    s16 x;
    s16 y;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    s16 childIndex;
    s16 frameDelay;  
    f32 rotationAngle;  
} BillRecord;

typedef struct BillOut {
    s32 unk0;
    s32 frameIndex;
    s32 framesRemaining;
    BillAnimationEntry *entry;
    BillRecord *record;
} BillOut;

typedef struct BillRenderPair {
    struct BillChildPayload *children[2];
    u16 cameraFacingMode;  
    u16 kind;
    u32 colors[2];
    SdfListHead *packetList;  
    struct BillRenderPair *next;  
} BillRenderPair;

typedef struct BillObj {
    f32 position[4];  
    f32 childScaleX;   
    f32 childScaleY;   
    f32 unk18;
    f32 unk1C;
    f32 rotationAngle;   
    u32 childParam;    
    void (*callback)();  
    u16 kind;          
    u16 requestedPacketListIndex;  
    union {
        void *payload;  
        struct BillChildPayload *child;  
        struct BillData *animationData;  
    } u;
    BillRenderPair pair;  
    u16 animationActive;  
    u8 pad52[2];
    u32 modeFlags;  
    u32 animationEntryIndex;  
    s32 entryCount;
    struct BillOut *resolvedEntries;  
} BillObj;

enum {
    BILL_ANIMATION_FLAG_FRAME_COLORS = 0x00000001,
    BILL_ANIMATION_FLAG_PACKET_LIST_2 = 0x00000002,
    BILL_ANIMATION_FLAG_PACKET_LIST_3 = 0x00000004,
    BILL_ANIMATION_FLAG_PACKET_LIST_MASK = 0x00000006,
    BILL_ANIMATION_FLAG_STOP_AT_END = 0x00000010,
    BILL_ANIMATION_FLAG_PLURAL_ENTRIES = 0x10000000
};

typedef struct BillPluralRecord {
    s16 entryIndex;  
    s16 delay;       
    s16 x;           
    s16 y;           
    u8 unk08[0x10];
} BillPluralRecord;

typedef void (*BattleEffectCallback)(BattleEffect *effect);

#define BILL_VARIANT_MASK 0xFFFF

void dds3BillboardTagLink(BillObj *obj, BillChildPayload *child);
BillData *dds3BillAnimLoadP2A(void *resourceData);
void *sdfChipAllocDirect(s32 size);
void dds3BillboardSetScale(struct BillObj *billboard, f32 x, f32 y);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerInit);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerExit);

INCLUDE_ASM(const s32, "effect/billManager", BillboardLoadTex);

INCLUDE_ASM(const s32, "effect/billManager", BillboardFreeTex);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardLinkOt);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardPktLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardLinkOt);

void dds3Billboard_BasicInit(BillObj *billboard) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    billboard->requestedPacketListIndex = 0;
    billboard->position[3] = zero;
    billboard->position[2] = zero;
    billboard->position[1] = zero;
    billboard->position[0] = zero;
    billboard->unk18 = one;
    billboard->childScaleY = one;
    billboard->unk1C = zero;
    billboard->childScaleX = one;
    billboard->childParam = 0x80808080;
    billboard->rotationAngle = zero;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_NormalCreate);

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_NormalDelete);

void dds3Billboard_NormalDraw(BillObj *obj) {
    dds3BillboardTagLink(obj, obj->u.child);
}

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2ACreate);

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2ACopy);

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2ADelete);

BillChildPayload *dds3Billboard_P2AAnimDraw(BillObj *obj, BillOut *out) {
    BillAnimationEntry *entry = out->entry;
    BillData *data = obj->u.animationData;
    BillRecord *record;
    BillChildPayload *child;
    s32 index;
    u32 color;

    if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        return NULL;
    }
    if (out->framesRemaining <= 0) {
        out->frameIndex++;
        if ((u32)out->frameIndex >= entry->frameCount) {
            if (entry->flags & BILL_ANIMATION_FLAG_STOP_AT_END) {
                obj->animationActive = 0;
                out->frameIndex = entry->frameCount - 1;
            } else {
                out->frameIndex = 0;
            }
        }
        out->framesRemaining = out->record[out->frameIndex].frameDelay;
    } else {
        out->framesRemaining--;
    }
    index = out->frameIndex;
    record = out->record + index;
    if (entry->flags & BILL_ANIMATION_FLAG_FRAME_COLORS) {
        color = ((u32 *)(data->base + entry->colorOffset))[index];
    } else {
        color = 0x80808080;
    }
    obj->childParam = color;
    child = data->children[record->childIndex];
    if (entry->flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
        obj->requestedPacketListIndex = 2;
    } else if (entry->flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
        obj->requestedPacketListIndex = 3;
    } else {
        obj->requestedPacketListIndex = 1;
    }
    obj->rotationAngle = record->rotationAngle;
    child->uv.components[0] = record->u0;
    child->uv.components[1] = record->v0;
    child->uv.components[2] = record->u1;
    child->uv.components[3] = record->v0;
    child->uv.components[4] = record->u1;
    child->uv.components[5] = record->v1;
    child->uv.components[6] = record->u0;
    child->uv.components[7] = record->v1;
    {
        f32 halfHeight = record->height * 0.5f;
        f32 halfWidth = record->width * 0.5f;
        f32 y = halfHeight - record->y;
        f32 x = halfWidth - record->x;

        child->halfHeight = halfHeight;
        child->halfWidth = halfWidth;
        child->y = y;
        child->x = x;
    }
    return child;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2AColorMul);

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2ADraw);

void dds3Billboard_P2ASetAnim(BillData *animationData, s32 entryIndex, BillOut *resolvedEntry) {
    u8 *base;
    BillAnimationEntry *entry;
    s32 offset;
    s16 value;

    base = animationData->base;
    entry = animationData->entries + entryIndex;
    offset = entry->offset;
    resolvedEntry->entry = entry;
    base = base + offset;
    resolvedEntry->frameIndex = 0;
    value = ((BillRecord *)base)->frameDelay;
    resolvedEntry->record = (BillRecord *)base;
    resolvedEntry->framesRemaining = value;
}

void dds3Billboard_P2ASetNumber(BillObj *obj, s32 animationEntryIndex) {
    BillData *data = obj->u.animationData;
    BillAnimationEntry *entry = data->entries + animationEntryIndex;

    if (entry->frameCount == 0) {
        obj->animationActive = 0;
        return;
    }
    if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        BillPluralRecord *records;
        u32 i = 0;

        printf("billAnim..PLURAL SET\n");
        obj->modeFlags = 0x10000000;
        obj->animationEntryIndex = animationEntryIndex;
        obj->entryCount = entry->frameCount;
        records = (BillPluralRecord *)(data->base + entry->offset);
        for (; i < entry->frameCount; i++) {
            dds3Billboard_P2ASetAnim(data, records[i].entryIndex, obj->resolvedEntries + i);
            (obj->resolvedEntries)[i].frameIndex = -records[i].delay;
        }
    } else if (entry->entryModeFlags & 0xC0) {
        printf("billAnim..(A)MTEX SET\n");
        obj->modeFlags = entry->entryModeFlags;
        obj->animationEntryIndex = animationEntryIndex;
        obj->entryCount = 2;
        dds3Billboard_P2ASetAnim(data, animationEntryIndex, obj->resolvedEntries);
        dds3Billboard_P2ASetAnim(data, animationEntryIndex + 1, obj->resolvedEntries + 1);
    } else {
        obj->modeFlags = 0;
        obj->entryCount = 1;
        obj->animationEntryIndex = animationEntryIndex;
        dds3Billboard_P2ASetAnim(data, animationEntryIndex, obj->resolvedEntries);
    }
    if (entry->entryModeFlags & 0x100) {
        printf("billAnim..P2A POLYGON\n");
    }
    obj->animationActive = 1;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillAnimLoadP2A);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillAnimFreeP2A);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardCopyO);

BillObj *dds3BillboardCreate(s32 index, u32 data) {
    BillObj *newobj;

    newobj = dds3BillboardFuncTable[index].func(data);
    dds3Billboard_BasicInit(newobj);
    newobj->kind = index;
    newobj->callback = dds3BillboardFuncTable[index].callback;
    return newobj;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardFileLoad);

BillObj *dds3BillboardCopy(const BillObj *source) {
    BillObj *copy;
    BillChildPayload *data;
    u16 sourceKind;
    void (*sourceCallback)();

    if (source->kind == 1) {
        copy = dds3Billboard_P2ACopy(source);
        dds3Billboard_BasicInit(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
    } else {
        copy = dds3Billboard_NormalCreate(NULL);
        dds3Billboard_BasicInit(copy);
        sourceKind = source->kind;
        data = source->u.child;
        sourceCallback = source->callback;
        copy->kind = sourceKind;
        copy->callback = sourceCallback;
        data->refCount = data->refCount + 1;
        copy->u.child = data;
    }
    return copy;
}

void dds3BillboardDelete(BillObj *obj) {
    dds3BillboardFuncTable[obj->kind].destroy();
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardDraw);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGeneral);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGeneralNumber);

SdfTex *dds3BillboardGetGeneralTex(s32 index) {
    return hGeneralBillboard[index]->u.child->texture;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetPosition);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetRotate);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetScale);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetRGBA);

INCLUDE_ASM(const s32, "effect/billManager", func_0015D050);

void dds3BillboardSetMode(BillObj *effect, s32 mode) {
    mode = (s16)mode;
    switch (effect->kind) {
    case 0:
    case 3:
        effect->requestedPacketListIndex = mode;
        break;
    case 1: {
        s32 entryCount = effect->entryCount;
        if (entryCount > 0) {
            s32 remaining = entryCount;
            BillOut *entries = effect->resolvedEntries;
            s32 index = 0;
            do {
                BillAnimationEntry *entry = entries[index].entry;
                u32 flags = entry->flags & ~BILL_ANIMATION_FLAG_PACKET_LIST_MASK;
                entry->flags = flags;
                if (mode == 2) {
                    entry->flags = flags | BILL_ANIMATION_FLAG_PACKET_LIST_2;
                } else if (mode == 3) {
                    entry->flags = flags | BILL_ANIMATION_FLAG_PACKET_LIST_3;
                }
                index++;
            } while (--remaining != 0);
        }
        break;
    }
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetCondition);

s32 dds3BillboardGetTex(BillObj *effect) {
    if (effect->kind == 0) {
        return (s32)effect->u.child->texture;
    }
    return 0;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetType);

void dds3BillboardSetDrawType(BillObj *effect, s32 variantValue) {
    s32 maskedValue = variantValue & BILL_VARIANT_MASK;

    switch (effect->kind) {
    case 0:
        effect->u.child->variant = (u16)maskedValue;
        break;
    case 1:
        effect->pair.cameraFacingMode = maskedValue;
        break;
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDrawType);

void dds3BillboardSetAnimNumber(BillObj *effect, u32 entryIndex) {
    if (effect->kind == 1 && effect->animationEntryIndex != entryIndex) {
        dds3Billboard_P2ASetNumber(effect, entryIndex);
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetCurrentAnimNumber);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetAnimNumber);

void dds3BillboardSetAnimKey(BillObj *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            BillOut *entry = effect->resolvedEntries;
            s32 remaining = entryCount;

            do {
                BillAnimationEntry *frameData = entry->entry;
                u32 wrappedFrame = startFrame % frameData->frameCount;
                remaining -= 1;
                entry->framesRemaining = 0;
                entry->frameIndex = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

void dds3BillboardSetAnimKey2(BillObj *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            BillOut *entry = effect->resolvedEntries;
            s32 remaining = entryCount;

            do {
                BillAnimationEntry *frameData = entry->entry;
                u32 wrappedFrame = startFrame % frameData->frameCount;
                remaining -= 1;
                entry->framesRemaining = 1;
                entry->frameIndex = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetAnimFrame);

u16 dds3BillboardGetAnimState(const BillObj *effect) {
    if (effect->kind == 1) {
        return effect->animationActive;
    }
    return 0;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetAnimType);

void dds3BillboardSetAnimColorAdvMode(BillObj *billboard) {
    if (billboard->kind == 1) {
        billboard->modeFlags |= 0x1000000;
    }
}

void dds3BillboardSetXYOffset(BillObj *billboard, float width, float height) {
    if (billboard->kind == 0) {
        BillChildPayload *child = billboard->u.child;
        child->halfWidth = width * 0.5f;
        child->halfHeight = height * 0.5f;
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDirectionRotate);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDirectionScale);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonCreate);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonCopy);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonDelete);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonSetPosition);

void dds3P2APolygonSetScale(s32 instance, float scale) {
    dds3BillboardSetScale(((EffUnitObject *)instance)->billboard, scale, scale);
}

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonSetRotateMatrix);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonSetRGBA);

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonSetDirection);

void dds3P2APolygonGetResource(EffUnitObject *instance, s32 *modeValues) {
    BillObj *billboard = instance->billboard;

    if (billboard->kind == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            modeValues[0] = 2;
            modeValues[2] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else if (modeFlags & 0x80) {
            modeValues[0] = 3;
            modeValues[2] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else {
            modeValues[0] = 0;
            modeValues[1] = (s32)dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
        }
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3P2APolygonDraw);

