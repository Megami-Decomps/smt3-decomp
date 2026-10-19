#include "common.h"
#include "basic.h"

#include "libsdf.h"
#include "ps2asm.h"

#include "billboard.h"

typedef struct EffUnitObject {
    f32 localMatrix[4][4];
    f32 transform[4][4];
    struct billObj_t *billboard;
    sdfMat_t *resource;
} EffUnitObject;

typedef struct BillKindFuncs {
    struct billObj_t *(*func)();
    void (*callback)();
    void (*destroy)();
} BillKindFuncs;

extern BillKindFuncs dds3BillboardFuncTable[];
extern struct billObj_t *hGeneralBillboard[];

s32 printf(const char *fmt, ...);

struct billObj_t *dds3Billboard_NormalCreate(void *data);
u32 dds3Billboard_P2AColorMul(u32 color, u32 scale);


typedef struct BillTextureQuad {
    u16 components[8];
} BillTextureQuad;

typedef struct billChildPayload_t {
    sdfTexHandle_t *texHandle;  
    u16 variant;  
    u8 pad06[2];
    s32 refCount;       
    BillTextureQuad uv;  
    f32 x;              
    f32 y;              
    f32 halfWidth;   
    f32 halfHeight;  
    sdfDrawTag_t *pendingLists[5];  
    u16 packetListIndex;  
    u8 pad42[2];
    struct sdfMemHandle_t *allocation;  
    sdfBillBoardBatch_t *work;  
    struct billChildPayload_t *next;  
} BillChildPayload;

typedef struct BillAnimationEntry {
    s32 offset;
    s32 colorOffset;
    u32 entryModeFlags;  
    u32 frameCount;
    u32 flags;
} BillAnimationEntry;

typedef struct billData_t {
    struct sdfMemHandle_t *allocation;  
    u8 *base;
    BillAnimationEntry *entries;
    s32 entryCount;
    s32 childCount;
    s32 listRefCount;
    struct billChildPayload_t **children;
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

typedef struct billOut_t {
    s32 unk0;
    s32 frameIndex;
    s32 framesRemaining;
    BillAnimationEntry *entry;
    BillRecord *record;
} BillOut;


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

#define BILL_VARIANT_MASK 0xFFFF

void dds3BillboardTagLink(billObj_t *obj, BillChildPayload *child);
BillData *dds3BillAnimLoadP2A(void *resourceData);
void *sdfChipAllocDirect(s32 size);
void dds3BillboardSetScale(struct billObj_t *billboard, f32 x, f32 y);

typedef struct BillLinkedValue {
    s32 opaque00;
    s32 value;
} BillLinkedValue;


#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4


/* Initialize the paired texture/state list, then append one quad to each
 * child stream. The second child owns the geometry and common batch count. */
void dds3MultiBillboardTagLink(billObj_t *obj, billRenderPair_t *node);

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
billObj_t *dds3BillboardCopy(const billObj_t *source);


billObj_t *dds3BillboardCreate(s32 index, u32 data);

void dds3BillboardDelete(billObj_t *obj);

/* Drop one reference; the last one releases every entry and the block itself. */
void dds3BillAnimFreeP2A(BillData *block);

/* Select an animation and initialize the plural records' signed start delays. */
void dds3Billboard_P2ASetNumber(billObj_t *obj, s32 animationEntryIndex);

/* Advance an entry's frame timer and apply its current record to the shared
 * child payload. Plural descriptors are handled by the dispatcher instead. */
BillChildPayload *dds3Billboard_P2AAnimDraw(billObj_t *obj, BillOut *out);


extern void *memcpy(void *dest, const void *src, u32 n);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);


void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Build a quad packet from paired colors and per-corner UV sets. */
u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32));

sdfMat_t *sdfMatNew(void);

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *asset);

void sdfChipFreeDirect(void *memory);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

void sdfMatSetConstant(struct sdfMat_t *asset, f32 value);

void sdfMatSetAmbient(struct sdfMat_t *asset, u32 value);

void sdfMatSetDiffuse(struct sdfMat_t *asset, u32 value);

void sdfMatSetSpecular(struct sdfMat_t *asset, u32 value);

/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const void *header);

extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

void BillboardFreeTex(BillChildPayload *child);
BillChildPayload *BillboardLoadTex(void *resource);
void dds3MultiBillboardPktLink(billRenderPair_t *pair);
void dds3Billboard_NormalDelete(billObj_t *obj);
billObj_t *dds3Billboard_P2ACreate(void *resourceData);
billObj_t *dds3Billboard_P2ACopy(const billObj_t *obj);
void dds3Billboard_P2ADelete(billObj_t *obj);
void dds3Billboard_P2ADraw(billObj_t *obj);
void dds3BillboardDraw(billObj_t *obj);
u32 dds3BillboardGeneralNumber(void);
void dds3BillboardSetPosition(void *dst, const void *src);
void dds3BillboardSetRotate(billObj_t *effect, float angle);
void dds3BillboardSetRGBA(billObj_t *effect, u32 value);
void func_0015D050(billObj_t *effect, const BillTextureQuad *textureQuad);
void dds3BillboardSetCondition(billObj_t *effect, s32 variant);
u16 dds3BillboardGetType(billObj_t *effect);
u16 dds3BillboardGetDrawType(billObj_t *effect);
s32 dds3BillboardGetCurrentAnimNumber(billObj_t *effect);
s32 dds3BillboardGetAnimNumber(billObj_t *effect);
s32 dds3BillboardGetAnimFrame(const billObj_t *effect);
s32 dds3BillboardGetAnimType(billObj_t *effect);
f32 dds3BillboardGetDirectionScale(void *self, f32 *axis);
EffUnitObject *dds3P2APolygonCreate(s32 entryIndex);
EffUnitObject *dds3P2APolygonCopy(const EffUnitObject *source);
void dds3P2APolygonDelete(EffUnitObject *instance);
void dds3P2APolygonSetPosition(EffUnitObject *instance, s128 *position);
void dds3P2APolygonSetRotateMatrix(EffUnitObject *instance, void *matrix);
void dds3P2APolygonSetRGBA(EffUnitObject *instance, u32 value);
void dds3P2APolygonSetDirection(void *dst, void *src);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerInit);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerExit);

INCLUDE_ASM(const s32, "effect/billManager", BillboardLoadTex);

INCLUDE_ASM(const s32, "effect/billManager", BillboardFreeTex);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardLinkOt);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardPktLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardLinkOt);

void dds3Billboard_BasicInit(billObj_t *billboard) {
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

void dds3Billboard_NormalDelete(billObj_t *obj) {
    BillboardFreeTex(obj->u.child);
    sdfChipFreeDirect(obj);
}

void dds3Billboard_NormalDraw(billObj_t *obj) {
    dds3BillboardTagLink(obj, obj->u.child);
}

billObj_t *dds3Billboard_P2ACreate(void *resourceData) {
    BillData *data;
    billObj_t *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = dds3BillAnimLoadP2A(resourceData);
    }
    n = data->entryCount;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (BillOut *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    newobj->pair.cameraFacingMode = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

billObj_t *dds3Billboard_P2ACopy(const billObj_t *obj) {
    BillData *data;
    s32 n;
    billObj_t *newobj;

    data = obj->u.animationData;
    n = data->entryCount;
    data->listRefCount = data->listRefCount + 1;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (BillOut *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

void dds3Billboard_P2ADelete(billObj_t *obj) {
    dds3BillAnimFreeP2A(obj->u.animationData);
    sdfChipFreeDirect(obj);
}

BillChildPayload *dds3Billboard_P2AAnimDraw(billObj_t *obj, BillOut *out) {
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

void dds3Billboard_P2ASetNumber(billObj_t *obj, s32 animationEntryIndex) {
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

billObj_t *dds3BillboardCreate(s32 index, u32 data) {
    billObj_t *newobj;

    newobj = dds3BillboardFuncTable[index].func(data);
    dds3Billboard_BasicInit(newobj);
    newobj->kind = index;
    newobj->callback = dds3BillboardFuncTable[index].callback;
    return newobj;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardFileLoad);

billObj_t *dds3BillboardCopy(const billObj_t *source) {
    billObj_t *copy;
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

void dds3BillboardDelete(billObj_t *obj) {
    dds3BillboardFuncTable[obj->kind].destroy();
}

void dds3BillboardDraw(billObj_t *obj) {
    obj->callback();
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGeneral);

u32 dds3BillboardGeneralNumber(void) {
    return 8;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index) {
    return hGeneralBillboard[index]->u.child->texHandle;
}

void dds3BillboardSetPosition(void *dst, const void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* Set the radian rotation applied to the billboard quad corners. */
void dds3BillboardSetRotate(billObj_t *effect, float angle) {
    effect->rotationAngle = angle;
}

/* Set the two child scale components stored at +0x10 and +0x14. */
void dds3BillboardSetScale(billObj_t *effect, float x, float y) {
    effect->childScaleX = x;
    effect->childScaleY = y;
}

void dds3BillboardSetRGBA(billObj_t *effect, u32 value) {
    effect->childParam = value;
}

void func_0015D050(billObj_t *effect, const BillTextureQuad *textureQuad) {
    if (effect->kind == 0) {
        memcpy(&effect->u.child->uv, textureQuad, sizeof(*textureQuad));
    }
}

void dds3BillboardSetMode(billObj_t *effect, s32 mode) {
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

/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billObj_t *effect, s32 variant) {
    s32 remainingChildren;
    BillChildPayload **childEntries;
    BillChildPayload **childCursor;

    variant = (s16)variant;
    switch (effect->kind) {
    case 0:
        effect->u.child->variant = (u16)variant;
        break;
    case 1:
        remainingChildren = effect->u.animationData->childCount;
        childEntries = effect->u.animationData->children;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->variant = (u16)variant;
                childCursor++;
                remainingChildren--;
            } while (remainingChildren != 0);
        }
        break;
    }
}

s32 dds3BillboardGetTex(billObj_t *effect) {
    if (effect->kind == 0) {
        return (s32)effect->u.child->texHandle;
    }
    return 0;
}

u16 dds3BillboardGetType(billObj_t *effect) {
    return effect->kind;
}

void dds3BillboardSetDrawType(billObj_t *effect, s32 variantValue) {
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

u16 dds3BillboardGetDrawType(billObj_t *effect) {
    switch (effect->kind) {
    case 0:
        return effect->u.child->variant;
    case 1:
        return effect->pair.cameraFacingMode;
    default:
        return 0;
    }
}

void dds3BillboardSetAnimNumber(billObj_t *effect, u32 entryIndex) {
    if (effect->kind == 1 && effect->animationEntryIndex != entryIndex) {
        dds3Billboard_P2ASetNumber(effect, entryIndex);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 dds3BillboardGetCurrentAnimNumber(billObj_t *effect) {
    if (effect->kind == 1) {
        return effect->animationEntryIndex;
    }
    return 0;
}

s32 dds3BillboardGetAnimNumber(billObj_t *effect) {
    if (effect->kind == 1) {
        return ((BillLinkedValue *)effect->u.animationData->base)->value;
    }
    return 0;
}

void dds3BillboardSetAnimKey(billObj_t *effect, u32 startFrame) {
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

void dds3BillboardSetAnimKey2(billObj_t *effect, u32 startFrame) {
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

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billObj_t *effect) {
    if (effect->kind == 1) {
        const BillOut *entry = effect->resolvedEntries;
        const BillAnimationEntry *frame = entry->entry;
        return frame->frameCount;
    }
    return 0;
}

u16 dds3BillboardGetAnimState(const billObj_t *effect) {
    if (effect->kind == 1) {
        return effect->animationActive;
    }
    return 0;
}

s32 dds3BillboardGetAnimType(billObj_t *effect) {
    if (effect->kind == 1) {
        return effect->modeFlags;
    }
    return 0;
}

void dds3BillboardSetAnimColorAdvMode(billObj_t *billboard) {
    if (billboard->kind == 1) {
        billboard->modeFlags |= 0x1000000;
    }
}

void dds3BillboardSetXYOffset(billObj_t *billboard, float width, float height) {
    if (billboard->kind == 0) {
        BillChildPayload *child = billboard->u.child;
        child->halfWidth = width * 0.5f;
        child->halfHeight = height * 0.5f;
    }
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDirectionRotate);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDirectionScale);

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
EffUnitObject *dds3P2APolygonCreate(s32 entryIndex) {
    EffUnitObject *instance = sdfChipAllocDirect(sizeof(EffUnitObject));

    instance->billboard = dds3BillboardCreate(1, entryIndex);
    instance->resource = sdfMatNew();
    sdfMatSetConstant(instance->resource, 1.0f);
    SDF_VU0_STOREIDENTITY(instance->transform);
    return instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
EffUnitObject *dds3P2APolygonCopy(const EffUnitObject *source) {
    EffUnitObject *instance = sdfChipAllocDirect(sizeof(EffUnitObject));

    instance->billboard = dds3BillboardCopy(
        source->billboard);
    instance->resource = sdfMatNew();
    sdfMatSetConstant(instance->resource, 1.0f);
    sdfMatSetDiffuse(instance->resource, 0x80808080);
    sdfMatSetAmbient(instance->resource, 0x80808080);
    sdfMatSetSpecular(instance->resource, 0x80808080);
    SDF_VU0_STOREIDENTITY(instance->transform);
    SDF_VU0_STOREIDENTITY(instance->localMatrix);
    return instance;
}

void dds3P2APolygonDelete(EffUnitObject *instance) {
    sdfMatFreeDelayed((sdfMat_t *)instance->resource);
    dds3BillboardDelete(instance->billboard);
    sdfChipFreeDirect(instance);
}

void dds3P2APolygonSetPosition(EffUnitObject *instance, s128 *position) {
    dds3BillboardSetPosition((s128 *)instance->billboard, position);
}

void dds3P2APolygonSetScale(s32 instance, float scale) {
    dds3BillboardSetScale(((EffUnitObject *)instance)->billboard, scale, scale);
}

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(EffUnitObject *instance, void *matrix) {
    SDF_VU0_LOADMATRIX(matrix);
    SDF_VU0_STOREMATRIX(instance->transform);
}

void dds3P2APolygonSetRGBA(EffUnitObject *instance, u32 value) {
    dds3BillboardSetRGBA(instance->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

void dds3P2APolygonGetResource(EffUnitObject *instance, s32 *modeValues) {
    billObj_t *billboard = instance->billboard;

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

