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

typedef struct BillGeneralEntry {
    s16 kind;
    u8 pad02[6];
} BillGeneralEntry;

typedef struct BillGeneralRow {
    u8 pad00[4];
    s16 kind;
    u8 pad06[2];
} BillGeneralRow;

typedef struct BillPacRequest {
    u8 pad00[0x60];
    sdfPacWork_t *queueHead;
} BillPacRequest;

extern BillGeneralEntry BillboardTable[];
extern BillGeneralRow D_00316FF0[];
extern struct billChildPayload_t *dds3BillboardTag;
extern char D_00532C28[];
extern char D_00532C40[];
extern char D_00532D30[];
extern char D_00532CB8[];
extern char D_00532CD8[];
extern char D_00532CF8[];

extern void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);
extern void *sdfMemGetPtr(struct sdfMemHandle_t *handle);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *address, s32 flags);
extern BillPacRequest *pacLoad(const char *path);
extern void pacSync(BillPacRequest *request);
extern void pacFileHandleFree_(void *request);

billObj_t *dds3BillboardGeneral(s32 index);

typedef struct BillOtLink {
    u8 pad00[0x10];
    void (*link)(struct BillOtLink *param, sdfDrawTag_t *tag);
} BillOtLink;

extern BillOtLink *D_00317050[];

typedef struct BillDrawBank {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} BillDrawBank;

extern BillDrawBank dds3DrawEnv[];
extern billRenderPair_t *D_00551D78;
extern sdfGrOt_t sdfGrOtTable[];
extern f32 dds3BillboardTagParam[4];
extern f32 D_00317030[4];

typedef struct BillScreenParam {
    u8 pad00[0x30];
    f32 matrix[16];
    f32 scale[4];
    f32 offset[4];
    u8 pad90[0x10];
} BillScreenParam;

extern BillScreenParam dds3ScreenParam;
extern f32 D_002FA240[16];
extern void MultMatrixRev(f32 *matrix);
extern f32 func_002CF748(f32 y, f32 x);

extern sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *texture);
extern s32 sdfTexGetPacketLength(sdfTexHandle_t *texture);
extern void sdf3DMakeDirectRefPacket(void *packet, u32 sourceAddress, s32 payloadBytes);
extern void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);
extern void sdfBillBoardMakeTMPacket(void *packet, const f32 *matrix);
extern f32 sdfCos(f32 angle);
extern f32 sdfSin(f32 angle);
extern f64 sdfFloatToULong(f32 value) __attribute__((const));
extern s32 dpdiv(f64 left, f64 right) __attribute__((const));
extern f64 dpsub(f64 left, f64 right) __attribute__((const));
extern f32 sdfDoubleToULong(f64 value) __attribute__((const));
PS2_SYMBOL_AT(sdfGrOt_t, dds3MultiBillboardOt, sdfGrOtTable, 0x580);

extern void *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern u32 dds3GetFrameID(void);
extern void sdfGrSetFrameMaskReset(const u8 *source, void *packet, s32 gsContext);
extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress);

extern u32 *sdfBillBoardMakeN(const u128 *positions, const void *byteAttributes,
                              const void *halfAttributes, const void *wordAttributes,
                              s32 vertexCount, void *(*allocatePacket)(s32));

struct billObj_t *dds3Billboard_NormalCreate(void *data);
u32 dds3Billboard_P2AColorMul(u32 color, u32 scale);


typedef struct BillColorWork {
    u32 word;
    u8 pad04[12];
} BillColorWork;

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
    sdfDrawTag_t *pendingLists[4];  
    u16 packetListIndex;  
    u8 pad3E[2];
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

/* NON_MATCHING: 2 of 46 words differ (first at +0x40). */
#ifdef NON_MATCHING
void dds3BillboardManagerInit(void) {
    BillPacRequest *request;
    sdfPacWork_t *work;
    BillGeneralRow *row;
    billObj_t **slot;

    dds3BillboardTag = NULL;
    request = pacLoad(D_00532C28);
    pacSync(request);
    work = request->queueHead;
    if (work != NULL) {
        slot = hGeneralBillboard;
        row = D_00316FF0;
        do {
            void *data = sdfMemGetPtr(work->resrc);
            *slot = dds3BillboardCreate(row->kind, (u32)data);
            row++;
            sdfMemFreeDirect(work->resrc);
            slot++;
            work = work->next;
        } while (work != NULL);
    }
    pacFileHandleFree_(request);
    printf(D_00532C40, D_00532C28);
}
#else
INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerInit);
#endif

void dds3BillboardManagerExit(void) {
    u32 i;

    dds3BillboardTag = NULL;
    for (i = 0; i < 8; i++) {
        dds3BillboardDelete(hGeneralBillboard[i]);
    }
}

BillChildPayload *BillboardLoadTex(void *resource) {
    sdfMemHandle_t *allocation;
    sdfBillBoardBatch_t *batch;
    BillChildPayload *child;
    sdfTexHandle_t *tex;
    s32 uvWidth;
    s32 uvHeight;
    s32 i;

    allocation = sdfMemAllocDirect(0x44C);
    batch = (sdfBillBoardBatch_t *)sdfMemLock(allocation);
    child = (BillChildPayload *)((u8 *)batch + 0x400);
    child->refCount = 0;
    child->allocation = allocation;
    child->work = batch;
    batch->count = 0;
    child->next = child;
    child->variant = 0;
    for (i = 3; i >= 0; i--) {
        child->pendingLists[i] = NULL;
    }
    child->packetListIndex = 0;
    tex = sdfTexSet(resource);
    child->refCount = child->refCount + 1;
    uvHeight = (s16)((u16)tex->height << 4);
    uvWidth = (s16)((u16)tex->width << 4);
    child->uv.components[0] = 0;
    child->uv.components[1] = 0;
    child->uv.components[2] = uvWidth;
    child->uv.components[3] = 0;
    child->uv.components[4] = uvWidth;
    child->uv.components[5] = uvHeight;
    child->uv.components[6] = 0;
    child->uv.components[7] = uvHeight;
    child->texHandle = tex;
    child->halfWidth = (uvWidth >> 5) * 0.0625f;
    child->halfHeight = (uvHeight >> 5) * 0.0625f;
    child->x = 0.0f;
    child->y = 0.0f;
    return child;
}

void BillboardFreeTex(BillChildPayload *child) {
    s32 refCount = child->refCount - 1;

    child->refCount = refCount;
    if (refCount == 0) {
        printf("BillManager Free Tex\n");
        sdfTexFreeDelayed(child->texHandle);
        sdfMemFreeDirect(child->allocation);
    }
}

/* NON_MATCHING: 255 of 271 words differ (first at +0x28). */
#ifdef NON_MATCHING
void dds3BillboardTagLink(billObj_t *obj, BillChildPayload *child) {
    f32 matrix[4][4];
    f32 viewDir[4];
    sdfBillBoardBatch_t *batch;
    u16 listIndex;
    s32 count;
    f32 hwx;
    f32 hhy;
    f32 cx;
    f32 cy;
    VTX_t *corner;

    listIndex = child->packetListIndex;
    if (listIndex != obj->requestedPacketListIndex) {
        batch = child->work;
        count = batch->count;
        if (count > 0) {
            sdfDrawTagLink(child->pendingLists[listIndex],
                           (u32)sdfBillBoardMakeN(batch->centre, batch->color, batch->uv,
                                                  batch->offset, count, NULL));
            batch->count = 0;
        }
        listIndex = obj->requestedPacketListIndex;
        child->packetListIndex = listIndex;
    }
    if (child->pendingLists[listIndex] == NULL) {
        sdfDrawTag_t *tag;
        void *refPacket;
        sdfTexPacket_t *texPacket;
        void *packet;
        s32 texLength;

        tag = sdfGrAllocPacketBuff(0x20);
        child->pendingLists[listIndex] = tag;
        sdfDrawTagInit(tag);
        refPacket = sdfGrAllocPacketBuff(0x20);
        texPacket = sdfTexGetPacket(child->texHandle);
        texLength = sdfTexGetPacketLength(child->texHandle);
        sdf3DMakeDirectRefPacket(refPacket, (u32)texPacket, texLength);
        sdfDrawTagLinkRef(child->pendingLists[listIndex], (u32)refPacket);
        if (child->variant & 1) {
            f32 dot;
            f64 angle;

            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(D_00317030);
            SDF_VU0_INNERPRODUCT(dot);
            angle = sdfFloatToULong(dot);
            if (dpdiv(angle, 0.0) < 0) {
                angle = dpsub(0.0, angle);
            }
            dds3BillboardTagParam[1] = sdfDoubleToULong(dpsub(1.0, angle));
            SDF_VU0_STOREVECTOR(viewDir);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR(dds3BillboardTagParam);
            SDF_VU0_SCALE_XYZ();
            SDF_VU0_STOREMATRIX(matrix);
        } else {
            SDF_VU0_STOREIDENTITY(matrix);
        }
        packet = sdfGrAllocPacketBuff(0x38);
        sdfBillBoardMakeTMPacket(packet, &matrix[0][0]);
        sdfDrawTagLink(child->pendingLists[listIndex], (u32)packet);
        if (child->next == child) {
            child->next = dds3BillboardTag;
            dds3BillboardTag = child;
        }
    }
    batch = child->work;
    count = batch->count;
    SDF_VU0_COPYVECTOR(&batch->centre[count], obj->position);
    batch->color[count] = obj->childParam;
    *(BillTextureQuad *)batch->uv[count] = child->uv;
    corner = batch->offset[count];
    hwx = child->halfWidth * obj->childScaleX;
    hhy = child->halfHeight * obj->childScaleY;
    cx = child->x * obj->childScaleX;
    cy = child->y * obj->childScaleY;
    if (obj->rotationAngle == 0.0f) {
        corner[0].x = cx - hwx;
        corner[0].y = cy - hhy;
        corner[1].x = cx + hwx;
        corner[1].y = cy - hhy;
        corner[2].x = cx + hwx;
        corner[2].y = cy + hhy;
        corner[3].x = cx - hwx;
        corner[3].y = cy + hhy;
    } else {
        f32 cosine = sdfCos(obj->rotationAngle);
        f32 sine = sdfSin(obj->rotationAngle);

        corner[0].x = (cx - hwx) * cosine - (cy - hhy) * sine;
        corner[0].y = (cx - hwx) * sine + (cy - hhy) * cosine;
        corner[1].x = (cx + hwx) * cosine - (cy - hhy) * sine;
        corner[1].y = (cx + hwx) * sine + (cy - hhy) * cosine;
        corner[2].x = (cx + hwx) * cosine - (cy + hhy) * sine;
        corner[2].y = (cx + hwx) * sine + (cy + hhy) * cosine;
        corner[3].x = (cx - hwx) * cosine - (cy + hhy) * sine;
        corner[3].y = (cx - hwx) * sine + (cy + hhy) * cosine;
    }
    count = batch->count + 1;
    batch->count = count;
    if (count == 15) {
        sdfDrawTagLink(child->pendingLists[listIndex],
                       (u32)sdfBillBoardMakeN(batch->centre, batch->color, batch->uv,
                                              batch->offset, 15, NULL));
        batch->count = 0;
    }
}
#else
INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardTagLink);
#endif

void dds3BillboardLinkOt(void) {
    BillChildPayload *res = dds3BillboardTag;

    if (res != NULL) {
        do {
            sdfBillBoardBatch_t *batch = res->work;
            s32 count = batch->count;
            BillChildPayload *next;
            s32 i;

            if (count > 0) {
                u32 *packet = sdfBillBoardMakeN(batch->centre, batch->color, batch->uv,
                                                batch->offset, count, NULL);
                sdfDrawTagLink(res->pendingLists[res->packetListIndex], (u32)packet);
                batch->count = 0;
            }
            for (i = 0; i < 4; i++) {
                sdfDrawTag_t *tag = res->pendingLists[i];
                if (tag != NULL) {
                    D_00317050[i]->link(D_00317050[i], tag);
                    res->pendingLists[i] = NULL;
                }
            }
            next = res->next;
            res->next = res;
            res = next;
        } while (res != NULL);
    }
    dds3BillboardTag = NULL;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);

void dds3MultiBillboardPktLink(billRenderPair_t *pair) {
    u32 colors[8][2];
    u16 uv[7][16];
    sdfBillBoardBatch_t *batch[2];
    s32 count;
    s32 i;
    u32 *packet;

    batch[0] = pair->children[0]->work;
    batch[1] = pair->children[1]->work;
    count = batch[1]->count;
    for (i = 0; i < count; i++) {
        colors[i][0] = batch[0]->color[i];
        colors[i][1] = batch[1]->color[i];
        uv[i][0] = batch[0]->uv[i][0].u;
        uv[i][1] = batch[0]->uv[i][0].v;
        uv[i][4] = batch[0]->uv[i][1].u;
        uv[i][5] = batch[0]->uv[i][1].v;
        uv[i][8] = batch[0]->uv[i][2].u;
        uv[i][9] = batch[0]->uv[i][2].v;
        uv[i][12] = batch[0]->uv[i][3].u;
        uv[i][13] = batch[0]->uv[i][3].v;
        uv[i][2] = batch[1]->uv[i][0].u;
        uv[i][3] = batch[1]->uv[i][0].v;
        uv[i][6] = batch[1]->uv[i][1].u;
        uv[i][7] = batch[1]->uv[i][1].v;
        uv[i][10] = batch[1]->uv[i][2].u;
        uv[i][11] = batch[1]->uv[i][2].v;
        uv[i][14] = batch[1]->uv[i][3].u;
        uv[i][15] = batch[1]->uv[i][3].v;
    }
    packet = sdfMultiBillBoardMakeN((const u128 *)batch[1]->centre, colors, uv, batch[1]->offset, count, NULL);
    sdfDrawTagLink(pair->packetList, (u32)packet);
    batch[0]->count = 0;
    batch[1]->count = 0;
}

void dds3MultiBillboardLinkOt(void) {
    billRenderPair_t *pair = D_00551D78;
    sdfDrawTag_t *tag;
    u64 *packet;
    void *buffer;
    u32 frame;

    if (pair != NULL) {
        do {
            if (pair->children[1]->work->count != 0) {
                dds3MultiBillboardPktLink(pair);
            }
            dds3MultiBillboardOt.regFunc(&dds3MultiBillboardOt, pair->packetList);
            pair->packetList = NULL;
            pair = pair->next;
        } while (pair != NULL);
    }
    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    buffer = sdfGrAllocPacketBuff(0x40);
    frame = dds3GetFrameID();
    sdfGrSetFrameMaskReset(dds3DrawEnv[frame].frameMaskSource, buffer, 0);
    sdfDrawTagLinkRefGif(tag, (u32)buffer);
    packet = sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000;
    packet[2] = 0x1000000000008002;
    packet[3] = 0xE;
    packet[4] = 0x71801;
    packet[5] = 0x47;
    packet[6] = 0x48;
    packet[7] = 0x42;
    sdfDrawTagLink(tag, (u32)packet);
    dds3MultiBillboardOt.regFunc(&dds3MultiBillboardOt, tag);
    D_00551D78 = NULL;
}

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

billObj_t *dds3Billboard_NormalCreate(void *data) {
    billObj_t *obj = sdfChipAllocDirect(0x34);

    if (data != NULL) {
        obj->u.child = BillboardLoadTex(data);
    }
    return obj;
}

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

/* NON_MATCHING: parked so dds3Billboard_P2ADraw matches; it is byte-exact as C. */
#ifdef NON_MATCHING
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
#else
INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2AAnimDraw);
#endif

/* vu0 routine: unpack two colours, multiply the lanes and pack. */
u32 dds3Billboard_P2AColorMul(u32 color, u32 scale) {
    BillColorWork work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = color;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = scale;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

void dds3Billboard_P2ADraw(billObj_t *obj) {
    u32 modeFlags;
    u32 savedColor;
    f32 savedRotation;
    BillChildPayload *child;

    if (obj->animationActive == 0) {
        return;
    }
    modeFlags = obj->modeFlags;
    if (modeFlags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        BillData *data = obj->u.animationData;
        BillAnimationEntry *entry = data->entries + obj->animationEntryIndex;
        BillPluralRecord *records;
        u32 i;

        savedColor = obj->childParam;
        savedRotation = obj->rotationAngle;
        records = (BillPluralRecord *)(data->base + entry->offset);
        for (i = 0; i < obj->entryCount; i++) {
            BillOut *out = (BillOut *)((u8 *)(i * sizeof(BillOut)) + (u32)obj->resolvedEntries);
            s32 frameIndex = out->frameIndex;

            if (frameIndex >= 0) {
                f32 savedX;
                f32 savedY;

                child = dds3Billboard_P2AAnimDraw(obj, out);
                savedX = child->x;
                savedY = child->y;
                child->x = records[i].x;
                child->y = -records[i].y;
                obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
                obj->rotationAngle = obj->rotationAngle + savedRotation;
                dds3BillboardTagLink(obj, child);
                child->x = savedX;
                child->y = savedY;
            } else {
                out->frameIndex = frameIndex + 1;
            }
        }
        obj->rotationAngle = savedRotation;
        obj->childParam = savedColor;
    } else if (modeFlags & 0x40) {
        savedColor = obj->childParam;
        savedRotation = obj->rotationAngle;
        child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
        obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
        obj->rotationAngle = obj->rotationAngle + savedRotation;
        dds3BillboardTagLink(obj, child);
        child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries + 1);
        obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
        obj->rotationAngle = obj->rotationAngle + savedRotation;
        dds3BillboardTagLink(obj, child);
        obj->rotationAngle = savedRotation;
        obj->childParam = savedColor;
    } else if (modeFlags & 0x80) {
        u32 flags;

        savedColor = obj->childParam;
        savedRotation = obj->rotationAngle;
        obj->pair.children[0] = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
        obj->pair.colors[0] = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
        obj->pair.children[1] = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries + 1);
        obj->pair.colors[1] = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
        flags = obj->resolvedEntries[1].entry->flags;
        if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
            obj->pair.kind = 2;
        } else if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
            obj->pair.kind = 3;
        } else {
            obj->pair.kind = 1;
        }
        obj->rotationAngle = obj->rotationAngle + savedRotation;
        dds3MultiBillboardTagLink(obj, &obj->pair);
        obj->rotationAngle = savedRotation;
        obj->childParam = savedColor;
    } else {
        savedColor = obj->childParam;
        savedRotation = obj->rotationAngle;
        child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
        obj->childParam = dds3Billboard_P2AColorMul(savedColor, obj->childParam);
        obj->rotationAngle = obj->rotationAngle + savedRotation;
        dds3BillboardTagLink(obj, child);
        obj->rotationAngle = savedRotation;
        obj->childParam = savedColor;
    }
}

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

BillData *dds3BillAnimLoadP2A(void *resourceData) {
    sdfMemHandle_t *allocation;
    BillData *block;
    u8 *base;
    s32 *texOffsets;
    s32 childCount;
    s32 headerSize;
    s32 entryCount;
    s32 i;

    headerSize = *(s32 *)resourceData;
    texOffsets = (s32 *)((u8 *)resourceData + headerSize);
    childCount = *texOffsets;
    texOffsets++;
    allocation = sdfMemAllocDirect(headerSize + childCount * sizeof(BillChildPayload) + 0x1C);
    block = (BillData *)sdfMemLock(allocation);
    base = (u8 *)block + 0x1C;
    block->allocation = allocation;
    memcpy(base, resourceData, *(s32 *)resourceData);
    block->base = base;
    block->childCount = childCount;
    block->entries = (BillAnimationEntry *)(base + 8);
    block->children = (BillChildPayload **)(base + *(s32 *)base + 8);
    for (i = 0; i < childCount; i++) {
        block->children[i] = BillboardLoadTex((u8 *)resourceData + *texOffsets++);
    }
    block->listRefCount = 1;
    block->entryCount = 1;
    entryCount = ((s32 *)block->base)[1];
    for (i = 0; i < entryCount; i++) {
        BillAnimationEntry *entry = &block->entries[i];

        if (entry->flags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
            u32 frameCount = entry->frameCount;

            block->entryCount = frameCount;
            printf(D_00532CB8, i, frameCount);
        } else if (entry->entryModeFlags & 0x40) {
            block->entryCount = 2;
            printf(D_00532CD8, i, 2);
        } else if (entry->entryModeFlags & 0x80) {
            block->entryCount = 2;
            printf(D_00532CF8, i, 2);
        }
    }
    return block;
}

INCLUDE_RODATA(const s32, "effect/billManager", D_00532CB8);

INCLUDE_RODATA(const s32, "effect/billManager", D_00532CD8);

INCLUDE_RODATA(const s32, "effect/billManager", D_00532CF8);

void dds3BillAnimFreeP2A(BillData *block) {
    s32 listRefCount = block->listRefCount - 1;

    block->listRefCount = listRefCount;
    if (listRefCount == 0) {
        s32 i = 0;

        printf("BillManager P2A Free\n");
        for (; i < block->childCount; i++) {
            BillboardFreeTex(block->children[i]);
        }
        sdfMemFreeDirect(block->allocation);
    }
}

void dds3BillboardCopyO(billObj_t *obj, s32 *out) {
    BillChildPayload *child;

    if (obj->kind == 1) {
        child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
        out[0] = (s32)child->texHandle;
        out[1] = *(s32 *)&child->uv;
    }
}

billObj_t *dds3BillboardCreate(s32 index, u32 data) {
    billObj_t *newobj;

    newobj = dds3BillboardFuncTable[index].func(data);
    dds3Billboard_BasicInit(newobj);
    newobj->kind = index;
    newobj->callback = dds3BillboardFuncTable[index].callback;
    return newobj;
}

billObj_t *dds3BillboardFileLoad(s32 index, const char *name) {
    sdfMemHandle_t *allocation;
    u32 data;
    billObj_t *obj;

    printf(D_00532D30, name);
    allocation = sdfDevLoadFileEx(name, &data, 0);
    obj = dds3BillboardCreate(index, data);
    sdfMemFreeDirect(allocation);
    return obj;
}

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

billObj_t *dds3BillboardGeneral(s32 index) {
    billObj_t *obj;
    BillChildPayload *child;
    s32 refCount;

    obj = dds3BillboardCreate(BillboardTable[index].kind, 0);
    child = hGeneralBillboard[index]->u.child;
    refCount = child->refCount;
    obj->u.child = child;
    child->refCount = refCount + 1;
    return obj;
}

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

/* vu0 routine: screen-space angle of a world axis at a world position. */
f32 dds3BillboardGetDirectionRotate(void *position, f32 *axis) {
    f32 delta[4];
    f32 origin[4];
    f32 tip[4];

    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(dds3ScreenParam.matrix);
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam.offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(origin);
    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(100.0f);
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam.offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(tip);
    delta[0] = origin[0] - tip[0];
    delta[1] = origin[1] - tip[1];
    delta[2] = 0.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(delta[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_MEMOPS(delta[0]);
    return func_002CF748(delta[1], delta[0]);
}

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(void *self, f32 *axis) {
    f32 dot;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_INNERPRODUCT(dot);
    EE_ABS_F32(dot, dot);
    return 1.0f - dot;
}

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

INCLUDE_RODATA(const s32, "effect/billManager", D_00532D30);

