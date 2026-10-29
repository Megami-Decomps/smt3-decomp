#include "common.h"
#include "sdk_libgcc.h"
#include "basic.h"
#include "draw3d.h"

#include "libsdf.h"
#include "fileManager.h"
#include "ps2asm.h"

#include "billboard.h"
#include "draw.h"

typedef struct dds3P2APolygon_tag {
    f32 dir[4][4];
    f32 rot[4][4];
    struct billHandle_t *hBillboard;
    sdfMat_t *pMat;
} dds3P2APolygon_tag;

typedef struct dds3EffectFunc3_t {
    struct billHandle_t *(*create)();
    void (*draw)();
    void (*destroy)();
} dds3EffectFunc3_t;

extern dds3EffectFunc3_t dds3BillboardFuncTable[];
extern struct billHandle_t *hGeneralBillboard[];

#include "sdk_libc.h"

typedef struct billGeneralEntry_t {
    s16 kind;
    u8 pad02[6];
} billGeneralEntry_t;

typedef struct billGeneralRow_t {
    u8 pad00[4];
    s16 kind;
    u8 pad06[2];
} billGeneralRow_t;


extern billGeneralEntry_t BillboardTable[];
extern billGeneralRow_t D_00316FF0[];
extern struct dds3BillResource_tag *dds3BillboardTag;

extern void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);
extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);
extern void *sdfMemGetPtr(struct sdfMemHandle_t *aHandle);
extern fileRequest_t *pacLoad(const char *filename);
extern void pacSync(fileRequest_t *pacFileHandle);
extern void pacFileHandleFree_(void *pacHandle);

billHandle_t *dds3BillboardGeneral(s32 number);

extern sdfGrOt_t *D_00317050[];

typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

extern effBlurDrawBank_t dds3DrawEnv[];
extern billRenderPair_t *D_00551D78;
extern sdfGrOt_t sdfGrOtTable[];
extern f32 dds3BillboardTagParam[4];
extern f32 D_00317030[4];

extern sdf3DScreenParam_t dds3ScreenParam;
extern f32 D_002FA240[16];
extern void MultMatrixRev(f32 *aMatrix);
extern f32 atan2f(f32 y, f32 x);

extern sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle);
extern s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);
extern void sdf3DMakeDirectRefPacket(void *aPacket, u32 aData, s32 aLeng);
extern void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, u32 aPacket);
extern void sdfBillBoardMakeTMPacket(void *aPacket, const f32 *aTM);
extern f32 sdfCos(f32 aRadius);
extern f32 sdfSin(f32 aRadius);
extern f64 fabs(f64 value);
PS2_SYMBOL_AT(sdfGrOt_t, dds3MultiBillboardOt, sdfGrOtTable, 0x580);

extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern u32 dds3GetFrameID(void);
extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);
extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, u32 aPacket);

extern u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor,
                              const void *aUVList, const void *aXYOffsetList,
                              s32 aNum, void *(*aAllocator)(s32));

struct billHandle_t *dds3Billboard_NormalCreate(void *data);
u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1);


typedef struct billColorWork_t {
    u32 word;
    u8 pad04[12];
} billColorWork_t;


typedef struct P2A_ANIMATION_tag {
    s32 spranm;
    s32 alpha;
    u32 attr;  
    u32 times;
    u32 mode;
} P2A_ANIMATION_tag;

typedef struct billData_t {
    struct sdfMemHandle_t *hMem;  
    u8 *pHeader;
    P2A_ANIMATION_tag *pAnimTable;
    s32 AnimNum;
    s32 TexNum;
    s32 count;
    struct dds3BillResource_tag **pResList;
} dds3BillAnimResource_tag;

typedef struct P2A_TEXANIM_tag {
    s16 sx;
    s16 sy;
    s16 cx;
    s16 cy;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    s16 texid;
    s16 attr;  
    f32 angle;  
} P2A_TEXANIM_tag;

typedef struct billOut_t {
    s32 hResource;
    s32 key;
    s32 wait;
    P2A_ANIMATION_tag *pAnim;
    P2A_TEXANIM_tag *pTexAnim;
} dds3BillAnim_tag;


enum {
    BILL_ANIMATION_FLAG_FRAME_COLORS = 0x00000001,
    BILL_ANIMATION_FLAG_PACKET_LIST_2 = 0x00000002,
    BILL_ANIMATION_FLAG_PACKET_LIST_3 = 0x00000004,
    BILL_ANIMATION_FLAG_PACKET_LIST_MASK = 0x00000006,
    BILL_ANIMATION_FLAG_STOP_AT_END = 0x00000010,
    BILL_ANIMATION_FLAG_PLURAL_ENTRIES = 0x10000000
};

/* One row of the plural-entry table a P2A animation points at: the sprite
 * animation to run, its start delay, and the centre the quad is drawn at.
 * The remaster models the table as raw data, so this name is the tree's own. */
typedef struct billPluralEntry_t {
    s16 entryIndex;
    s16 delay;
    s16 x;
    s16 y;
    u8 unk08[0x10];
} billPluralEntry_t;

#define BILL_VARIANT_MASK 0xFFFF

void dds3BillboardTagLink(billHandle_t *obj, dds3BillResource_tag *child);
dds3BillAnimResource_tag *dds3BillAnimLoadP2A(void *resourceData);
void *sdfChipAllocDirect(s32 aSize);
void dds3BillboardSetScale(struct billHandle_t *hBillboard, f32 x, f32 y);

typedef struct P2A_HEAD_t {
    s32 tmxofs;
    s32 AnimNum;
} P2A_HEAD_t;


#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4


/* Initialize the paired texture/state list, then append one quad to each
 * child stream. The second child owns the geometry and common batch count. */
void dds3MultiBillboardTagLink(billHandle_t *obj, billRenderPair_t *node);

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard);


billHandle_t *dds3BillboardCreate(s32 index, u32 data);

void dds3BillboardDelete(billHandle_t *hData);

/* Drop one reference; the last one releases every entry and the block itself. */
void dds3BillAnimFreeP2A(dds3BillAnimResource_tag *hBillAnimRes);

/* Select an animation and initialize the plural records' signed start delays. */
void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number);

/* Advance an entry's frame timer and apply its current record to the shared
 * child payload. Plural descriptors are handled by the dispatcher instead. */
dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim);


extern void *memcpy(void *a, const void *b, u32 c);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);


void sdfDrawTagLink(struct sdfDrawTag_t *aTag, u32 aPacket);

/* Build a quad packet from paired colors and per-corner UV sets. */
u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

sdfMat_t *sdfMatNew(void);

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *aMat);

void sdfChipFreeDirect(void *aPtr);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);

void sdfMatSetAmbient(struct sdfMat_t *aMat, u32 aColor);

void sdfMatSetDiffuse(struct sdfMat_t *aMat, u32 aColor);

void sdfMatSetSpecular(struct sdfMat_t *aMat, u32 aColor);

/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const void *aTex);

extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

void BillboardFreeTex(dds3BillResource_tag *hResource);
dds3BillResource_tag *BillboardLoadTex(void *pTex);
void dds3MultiBillboardPktLink(billRenderPair_t *hResource);
void dds3Billboard_NormalDelete(billHandle_t *hData);
billHandle_t *dds3Billboard_P2ACreate(void *resourceData);
billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A);
void dds3Billboard_P2ADelete(billHandle_t *hData);
void dds3Billboard_P2ADraw(billHandle_t *hData);
void dds3BillboardDraw(billHandle_t *hData);
u32 dds3BillboardGeneralNumber(void);
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetRotate(billHandle_t *hBillboard, float rot);
void dds3BillboardSetRGBA(billHandle_t *hBillboard, u32 abgr);
void func_0015D050(billHandle_t *effect, const billUvQuad_t *textureQuad);
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition);
u16 dds3BillboardGetType(billHandle_t *hBillboard);
u16 dds3BillboardGetDrawType(billHandle_t *hBillboard);
s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *hBillboard);
s32 dds3BillboardGetAnimNumber(billHandle_t *hBillboard);
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard);
s32 dds3BillboardGetAnimType(billHandle_t *hBillboard);
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec);
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex);
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source);
void dds3P2APolygonDelete(dds3P2APolygon_tag *hData);
void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos);
void dds3P2APolygonSetRotateMatrix(dds3P2APolygon_tag *hData, void *pMatrix);
void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr);
void dds3P2APolygonSetDirection(void *hData, void *pMatrix);

/* NON_MATCHING: 2 of 46 words differ (first at +0x40). */
#ifdef NON_MATCHING
void dds3BillboardManagerInit(void) {
    fileRequest_t *request;
    sdfPacDecodeResrc_t *work;
    billGeneralRow_t *row;
    billHandle_t **slot;

    dds3BillboardTag = NULL;
    request = pacLoad("/bill/billtmx.LB");
    pacSync(request);
    work = request->pac.resrcHead;
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
    printf("billManager:[%s]\n", "/bill/billtmx.LB");
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

dds3BillResource_tag *BillboardLoadTex(void *pTex) {
    sdfMemHandle_t *allocation;
    sdfBillBoardBatch_t *batch;
    dds3BillResource_tag *child;
    sdfTexHandle_t *tex;
    s32 uvWidth;
    s32 uvHeight;
    s32 i;

    allocation = sdfMemAllocDirect(0x44C);
    batch = (sdfBillBoardBatch_t *)sdfMemLock(allocation);
    child = (dds3BillResource_tag *)((u8 *)batch + 0x400);
    child->count = 0;
    child->hMem = allocation;
    child->batch = batch;
    batch->count = 0;
    child->next = child;
    child->type = 0;
    for (i = 3; i >= 0; i--) {
        child->pBillTag[i] = NULL;
    }
    child->current_mode = 0;
    tex = sdfTexSet(pTex);
    child->count = child->count + 1;
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
    child->hTex = tex;
    child->width = (uvWidth >> 5) * 0.0625f;
    child->height = (uvHeight >> 5) * 0.0625f;
    child->cx = 0.0f;
    child->cy = 0.0f;
    return child;
}

void BillboardFreeTex(dds3BillResource_tag *hResource) {
    s32 refCount = hResource->count - 1;

    hResource->count = refCount;
    if (refCount == 0) {
        printf("BillManager Free Tex\n");
        sdfTexFreeDelayed(hResource->hTex);
        sdfMemFreeDirect(hResource->hMem);
    }
}

/* Adapted from Megami-Decomps/dds-decomp, effect/billManager.c at f79f86dc.
 * Each row is eight actual X/Y offsets, consumed as a 32-byte quad stream. */
void dds3BillboardTagLink(billHandle_t *obj, dds3BillResource_tag *child) {
    f32 matrix[16] __attribute__((aligned(16)));
    f32 direction[4];
    f32 dot;
    sdfBillBoardBatch_t *work;
    s32 selected;
    void *packet;
    u32 *vertexPacket;
    u8 *geometry;
    s32 index;
    f32 x, y, halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    selected = child->current_mode;
    if (selected != obj->mode) {
        work = child->batch;
        if (work->count > 0) {
            vertexPacket = sdfBillBoardMakeN((const u128 *)work->centre, work->color,
                work->uv, work->offset, work->count, 0);
            sdfDrawTagLink(child->pBillTag[selected], (u32)vertexPacket);
            work->count = 0;
        }
        selected = obj->mode;
        child->current_mode = selected;
    }
    if (child->pBillTag[selected] == NULL) {
        child->pBillTag[selected] = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(child->pBillTag[selected]);
        packet = sdfGrAllocPacketBuff(0x20);
        sdf3DMakeDirectRefPacket(packet,
            (u32)sdfTexGetPacket(child->hTex),
            sdfTexGetPacketLength(child->hTex));
        sdfDrawTagLinkRef(child->pBillTag[selected], packet);
        if ((u16)(child->type & 1) != 0) {
            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(D_00317030);
            SDF_VU0_INNERPRODUCT(dot);
            dds3BillboardTagParam[1] = 1.0 - fabs(dot);
            SDF_VU0_STOREVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR_BARRIER(dds3BillboardTagParam);
            SDF_VU0_SCALE_XYZ();
            SDF_VU0_STOREMATRIX(matrix);
        } else {
            SDF_VU0_STOREIDENTITY(matrix);
        }
        geometry = (u8 *)sdfGrAllocPacketBuff(0x38);
        sdfBillBoardMakeTMPacket(geometry, matrix);
        sdfDrawTagLink(child->pBillTag[selected], (u32)geometry);
        if (child->next == child) {
            child->next = dds3BillboardTag;
            dds3BillboardTag = child;
        }
    }
    work = child->batch;
    index = work->count;
    SDF_VU0_COPYVECTOR(&work->centre[index], obj->pos);
    work->color[index] = obj->abgr;
    memcpy(&work->uv[index], &child->uv, sizeof(child->uv));
    x = child->cx * obj->childScaleX;
    y = child->cy * obj->childScaleY;
    halfWidth = child->width * obj->childScaleX;
    halfHeight = child->height * obj->childScaleY;
    if (obj->rot == 0.0f) {
        work->offset[index][0] = x - halfWidth;
        work->offset[index][1] = y - halfHeight;
        work->offset[index][2] = x + halfWidth;
        work->offset[index][3] = y - halfHeight;
        work->offset[index][4] = x + halfWidth;
        work->offset[index][5] = y + halfHeight;
        work->offset[index][6] = x - halfWidth;
        work->offset[index][7] = y + halfHeight;
    } else {
        cosine = sdfCos(obj->rot);
        sine = sdfSin(obj->rot);
        cornerX = x - halfWidth;
        cornerY = y - halfHeight;
        work->offset[index][0] = cornerX * cosine - cornerY * sine;
        work->offset[index][1] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y - halfHeight;
        work->offset[index][2] = cornerX * cosine - cornerY * sine;
        work->offset[index][3] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y + halfHeight;
        work->offset[index][4] = cornerX * cosine - cornerY * sine;
        work->offset[index][5] = cornerX * sine + cornerY * cosine;
        cornerX = x - halfWidth;
        cornerY = y + halfHeight;
        work->offset[index][6] = cornerX * cosine - cornerY * sine;
        work->offset[index][7] = cornerX * sine + cornerY * cosine;
    }
    work->count++;
    if (work->count == 15) {
        vertexPacket = sdfBillBoardMakeN((const u128 *)work->centre, work->color,
            work->uv, work->offset, 15, 0);
        sdfDrawTagLink(child->pBillTag[selected], (u32)vertexPacket);
        work->count = 0;
    }
}

void dds3BillboardLinkOt(void) {
    dds3BillResource_tag *res = dds3BillboardTag;

    if (res != NULL) {
        do {
            sdfBillBoardBatch_t *batch = res->batch;
            s32 count = batch->count;
            dds3BillResource_tag *next;
            s32 i;

            if (count > 0) {
                u32 *packet = sdfBillBoardMakeN(batch->centre, batch->color, batch->uv,
                                                batch->offset, count, NULL);
                sdfDrawTagLink(res->pBillTag[res->current_mode], (u32)packet);
                batch->count = 0;
            }
            for (i = 0; i < 4; i++) {
                sdfDrawTag_t *tag = res->pBillTag[i];
                if (tag != NULL) {
                    D_00317050[i]->regFunc(D_00317050[i], tag);
                    res->pBillTag[i] = NULL;
                }
            }
            next = res->next;
            res->next = res;
            res = next;
        } while (res != NULL);
    }
    dds3BillboardTag = NULL;
}

#ifdef NON_MATCHING
extern f32 D_00317060[4];
extern f32 dds3MultiBillboardTagParam[4];
extern void sdfGrSetFrameMaskRGB(const u8 *env, void *packet, s32 context);
extern sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *handle);
extern s32 sdfTexGetPacketCtx2Length(sdfTexHandle_t *handle);

void dds3MultiBillboardTagLink(billHandle_t *obj, billRenderPair_t *node)
{
    f32 matrix[16] __attribute__((aligned(16)));
    f32 direction[4];
    f32 dot;
    sdfPrimTransPacket_t *blend;
    sdfBillBoardBatch_t *batch;
    dds3BillResource_tag *child;
    void *packet;
    sdfTexPacket_t *texture;
    s32 length, index;
    f32 x, y, halfWidth, halfHeight, cosine, sine, cornerX, cornerY;

    if (node->packetList == NULL) {
        node->packetList = sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(node->packetList);
        packet = sdfGrAllocPacketBuff(0x40);
        sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameMaskSource, packet, 0);
        sdfDrawTagLinkRefGif(node->packetList, (u32)packet);
        blend = sdfGrAllocPacketBuff(0x40);
        blend->elem.test.reg = 0x47;
        blend->elem.alpha.data = 0x44;
        blend->elem.alpha.reg = 0x42;
        blend->header.dmaWords.tag = 3;
        blend->header.dmaWords.vif = 0x5000000310000000ULL;
        blend->header.giftag.parm = 0x1000000000008002ULL;
        blend->header.giftag.regs = 0xE;
        blend->elem.test.data = 0x51001;
        sdfDrawTagLink(node->packetList, (u32)blend);
        blend = sdfGrAllocPacketBuff(0x40);
        blend->header.dmaWords.tag = 3;
        blend->header.dmaWords.vif = 0x5000000310000000ULL;
        blend->header.giftag.parm = 0x1000000000008002ULL;
        blend->header.giftag.regs = 0xE;
        blend->elem.test.data = 0x51001;
        blend->elem.test.reg = 0x48;
        switch (node->kind) {
        case 1: blend->elem.alpha.data = 0x54; break;
        case 2: blend->elem.alpha.data = 0x58; break;
        case 3: blend->elem.alpha.data = 0x52; break;
        }
        blend->elem.alpha.reg = 0x43;
        sdfDrawTagLink(node->packetList, (u32)blend);
        packet = sdfGrAllocPacketBuff(0x20);
        texture = sdfTexGetPacket(node->children[0]->hTex);
        length = sdfTexGetPacketLength(node->children[0]->hTex);
        sdf3DMakeDirectRefPacket(packet, (u32)texture, length);
        sdfDrawTagLinkRef(node->packetList, (u32)packet);
        packet = sdfGrAllocPacketBuff(0x20);
        texture = sdfTexGetPacketCtx2(node->children[1]->hTex);
        length = sdfTexGetPacketCtx2Length(node->children[1]->hTex);
        sdf3DMakeDirectRefPacket(packet, (u32)texture, length);
        sdfDrawTagLinkRef(node->packetList, (u32)packet);
        packet = sdfGrAllocPacketBuff(0x38);
        if (node->cameraFacingMode == 1) {
            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(D_00317060);
            SDF_VU0_INNERPRODUCT(dot);
            dds3MultiBillboardTagParam[1] = 1.0 - fabs(dot);
            SDF_VU0_STOREVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR_BARRIER(dds3MultiBillboardTagParam);
            SDF_VU0_SCALE_XYZ();
            SDF_VU0_STOREMATRIX(matrix);
        } else {
            SDF_VU0_STOREIDENTITY(matrix);
        }
        sdfBillBoardMakeTMPacket(packet, matrix);
        sdfDrawTagLink(node->packetList, (u32)packet);
        node->next = D_00551D78;
        D_00551D78 = node;
    }
    child = node->children[0];
    index = node->children[1]->batch->count;
    batch = child->batch;
    memcpy(&batch->uv[index], &child->uv, sizeof(child->uv));
    batch->color[index] = node->colors[0];
    child = node->children[1];
    batch = child->batch;
    memcpy(&batch->uv[index], &child->uv, sizeof(child->uv));
    batch->color[index] = node->colors[1];
    x = child->cx * obj->childScaleX;
    y = child->cy * obj->childScaleY;
    halfWidth = child->width * obj->childScaleX;
    halfHeight = child->height * obj->childScaleY;
    if (obj->rot == 0.0f) {
        batch->offset[index][0] = x - halfWidth;
        batch->offset[index][1] = y - halfHeight;
        batch->offset[index][2] = x + halfWidth;
        batch->offset[index][3] = y - halfHeight;
        batch->offset[index][4] = x + halfWidth;
        batch->offset[index][5] = y + halfHeight;
        batch->offset[index][6] = x - halfWidth;
        batch->offset[index][7] = y + halfHeight;
    } else {
        cosine = sdfCos(obj->rot);
        sine = sdfSin(obj->rot);
        cornerX = x - halfWidth;
        cornerY = y - halfHeight;
        batch->offset[index][0] = cornerX * cosine - cornerY * sine;
        batch->offset[index][1] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        batch->offset[index][2] = cornerX * cosine - cornerY * sine;
        batch->offset[index][3] = cornerX * sine + cornerY * cosine;
        cornerY = y + halfHeight;
        batch->offset[index][4] = cornerX * cosine - cornerY * sine;
        batch->offset[index][5] = cornerX * sine + cornerY * cosine;
        cornerX = x - halfWidth;
        batch->offset[index][6] = cornerX * cosine - cornerY * sine;
        batch->offset[index][7] = cornerX * sine + cornerY * cosine;
    }
    SDF_VU0_COPYVECTOR(&batch->centre[index], obj->pos);
    batch->count = (s32)((u32)batch->count + 1);
    if (batch->count == 7) dds3MultiBillboardPktLink(node);
}
#else
INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);
#endif

void dds3MultiBillboardPktLink(billRenderPair_t *hResource) {
    u32 colors[8][2];
    u16 uv[7][16];
    sdfBillBoardBatch_t *batch[2];
    s32 count;
    s32 i;
    u32 *packet;

    batch[0] = hResource->children[0]->batch;
    batch[1] = hResource->children[1]->batch;
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
    sdfDrawTagLink(hResource->packetList, (u32)packet);
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
            if (pair->children[1]->batch->count != 0) {
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

void dds3Billboard_BasicInit(billHandle_t *pBasic) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    pBasic->mode = 0;
    pBasic->pos[3] = zero;
    pBasic->pos[2] = zero;
    pBasic->pos[1] = zero;
    pBasic->pos[0] = zero;
    pBasic->unk18 = one;
    pBasic->childScaleY = one;
    pBasic->unk1C = zero;
    pBasic->childScaleX = one;
    pBasic->abgr = 0x80808080;
    pBasic->rot = zero;
}

billHandle_t *dds3Billboard_NormalCreate(void *data) {
    billHandle_t *obj = sdfChipAllocDirect(0x34);

    if (data != NULL) {
        obj->u.child = BillboardLoadTex(data);
    }
    return obj;
}

void dds3Billboard_NormalDelete(billHandle_t *hData) {
    BillboardFreeTex(hData->u.child);
    sdfChipFreeDirect(hData);
}

void dds3Billboard_NormalDraw(billHandle_t *hData) {
    dds3BillboardTagLink(hData, hData->u.child);
}

billHandle_t *dds3Billboard_P2ACreate(void *resourceData) {
    dds3BillAnimResource_tag *data;
    billHandle_t *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = dds3BillAnimLoadP2A(resourceData);
    }
    n = data->AnimNum;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (dds3BillAnim_tag *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    newobj->pair.cameraFacingMode = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A) {
    dds3BillAnimResource_tag *data;
    s32 n;
    billHandle_t *newobj;

    data = pCopyP2A->u.animationData;
    n = data->AnimNum;
    data->count = data->count + 1;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (dds3BillAnim_tag *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

void dds3Billboard_P2ADelete(billHandle_t *hData) {
    dds3BillAnimFreeP2A(hData->u.animationData);
    sdfChipFreeDirect(hData);
}

dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim) {
    P2A_ANIMATION_tag *entry = pBillAnim->pAnim;
    dds3BillAnimResource_tag *data = pP2A->u.animationData;
    P2A_TEXANIM_tag *record;
    dds3BillResource_tag *child;
    s32 index;
    u32 color;

    if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        return NULL;
    }
    if (pBillAnim->wait <= 0) {
        pBillAnim->key++;
        if ((u32)pBillAnim->key >= entry->times) {
            if (entry->mode & BILL_ANIMATION_FLAG_STOP_AT_END) {
                pP2A->animationActive = 0;
                pBillAnim->key = entry->times - 1;
            } else {
                pBillAnim->key = 0;
            }
        }
        pBillAnim->wait = pBillAnim->pTexAnim[pBillAnim->key].attr;
    } else {
        pBillAnim->wait--;
    }
    index = pBillAnim->key;
    record = pBillAnim->pTexAnim + index;
    if (entry->mode & BILL_ANIMATION_FLAG_FRAME_COLORS) {
        color = ((u32 *)(data->pHeader + entry->alpha))[index];
    } else {
        color = 0x80808080;
    }
    pP2A->abgr = color;
    child = data->pResList[record->texid];
    if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
        pP2A->mode = 2;
    } else if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
        pP2A->mode = 3;
    } else {
        pP2A->mode = 1;
    }
    pP2A->rot = record->angle;
    child->uv.components[0] = record->u1;
    child->uv.components[1] = record->v1;
    child->uv.components[2] = record->u2;
    child->uv.components[3] = record->v1;
    child->uv.components[4] = record->u2;
    child->uv.components[5] = record->v2;
    child->uv.components[6] = record->u1;
    child->uv.components[7] = record->v2;
    {
        f32 halfHeight = record->sy * 0.5f;
        f32 halfWidth = record->sx * 0.5f;
        f32 y = halfHeight - record->cy;
        f32 x = halfWidth - record->cx;

        child->height = halfHeight;
        child->width = halfWidth;
        child->cy = y;
        child->cx = x;
    }
    return child;
}

/* vu0 routine: unpack two colours, multiply the lanes and pack. */
u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1) {
    billColorWork_t work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = color0;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = color1;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

void dds3Billboard_P2ADraw(billHandle_t *hData) {
    u32 modeFlags;
    u32 savedColor;
    f32 savedRotation;
    dds3BillResource_tag *child;

    if (hData->animationActive == 0) {
        return;
    }
    modeFlags = hData->modeFlags;
    if (modeFlags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        dds3BillAnimResource_tag *data = hData->u.animationData;
        P2A_ANIMATION_tag *entry = data->pAnimTable + hData->animationEntryIndex;
        billPluralEntry_t *records;
        u32 i;

        savedColor = hData->abgr;
        savedRotation = hData->rot;
        records = (billPluralEntry_t *)(data->pHeader + entry->spranm);
        for (i = 0; i < hData->entryCount; i++) {
            dds3BillAnim_tag *out = (dds3BillAnim_tag *)((u8 *)(i * sizeof(dds3BillAnim_tag)) + (u32)hData->resolvedEntries);
            s32 frameIndex = out->key;

            if (frameIndex >= 0) {
                f32 savedX;
                f32 savedY;

                child = dds3Billboard_P2AAnimDraw(hData, out);
                savedX = child->cx;
                savedY = child->cy;
                child->cx = records[i].x;
                child->cy = -records[i].y;
                hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
                hData->rot = hData->rot + savedRotation;
                dds3BillboardTagLink(hData, child);
                child->cx = savedX;
                child->cy = savedY;
            } else {
                out->key = frameIndex + 1;
            }
        }
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else if (modeFlags & 0x40) {
        savedColor = hData->abgr;
        savedRotation = hData->rot;
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries + 1);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else if (modeFlags & 0x80) {
        u32 flags;

        savedColor = hData->abgr;
        savedRotation = hData->rot;
        hData->pair.children[0] = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->pair.colors[0] = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->pair.children[1] = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries + 1);
        hData->pair.colors[1] = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        flags = hData->resolvedEntries[1].pAnim->mode;
        if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
            hData->pair.kind = 2;
        } else if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
            hData->pair.kind = 3;
        } else {
            hData->pair.kind = 1;
        }
        hData->rot = hData->rot + savedRotation;
        dds3MultiBillboardTagLink(hData, &hData->pair);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else {
        savedColor = hData->abgr;
        savedRotation = hData->rot;
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    }
}

void dds3Billboard_P2ASetAnim(dds3BillAnimResource_tag *hBillAnimRes, s32 number, dds3BillAnim_tag *pBillAnim) {
    u8 *base;
    P2A_ANIMATION_tag *entry;
    s32 offset;
    s16 value;

    base = hBillAnimRes->pHeader;
    entry = hBillAnimRes->pAnimTable + number;
    offset = entry->spranm;
    pBillAnim->pAnim = entry;
    base = base + offset;
    pBillAnim->key = 0;
    value = ((P2A_TEXANIM_tag *)base)->attr;
    pBillAnim->pTexAnim = (P2A_TEXANIM_tag *)base;
    pBillAnim->wait = value;
}

void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number) {
    dds3BillAnimResource_tag *data = pP2A->u.animationData;
    P2A_ANIMATION_tag *entry = data->pAnimTable + number;

    if (entry->times == 0) {
        pP2A->animationActive = 0;
        return;
    }
    if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        billPluralEntry_t *records;
        u32 i = 0;

        printf("billAnim..PLURAL SET\n");
        pP2A->modeFlags = 0x10000000;
        pP2A->animationEntryIndex = number;
        pP2A->entryCount = entry->times;
        records = (billPluralEntry_t *)(data->pHeader + entry->spranm);
        for (; i < entry->times; i++) {
            dds3Billboard_P2ASetAnim(data, records[i].entryIndex, pP2A->resolvedEntries + i);
            (pP2A->resolvedEntries)[i].key = -records[i].delay;
        }
    } else if (entry->attr & 0xC0) {
        printf("billAnim..(A)MTEX SET\n");
        pP2A->modeFlags = entry->attr;
        pP2A->animationEntryIndex = number;
        pP2A->entryCount = 2;
        dds3Billboard_P2ASetAnim(data, number, pP2A->resolvedEntries);
        dds3Billboard_P2ASetAnim(data, number + 1, pP2A->resolvedEntries + 1);
    } else {
        pP2A->modeFlags = 0;
        pP2A->entryCount = 1;
        pP2A->animationEntryIndex = number;
        dds3Billboard_P2ASetAnim(data, number, pP2A->resolvedEntries);
    }
    if (entry->attr & 0x100) {
        printf("billAnim..P2A POLYGON\n");
    }
    pP2A->animationActive = 1;
}

dds3BillAnimResource_tag *dds3BillAnimLoadP2A(void *resourceData) {
    sdfMemHandle_t *allocation;
    dds3BillAnimResource_tag *block;
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
    allocation = sdfMemAllocDirect(headerSize + childCount * sizeof(dds3BillResource_tag) + 0x1C);
    block = (dds3BillAnimResource_tag *)sdfMemLock(allocation);
    base = (u8 *)block + 0x1C;
    block->hMem = allocation;
    memcpy(base, resourceData, *(s32 *)resourceData);
    block->pHeader = base;
    block->TexNum = childCount;
    block->pAnimTable = (P2A_ANIMATION_tag *)(base + 8);
    block->pResList = (dds3BillResource_tag **)(base + *(s32 *)base + 8);
    for (i = 0; i < childCount; i++) {
        block->pResList[i] = BillboardLoadTex((u8 *)resourceData + *texOffsets++);
    }
    block->count = 1;
    block->AnimNum = 1;
    entryCount = ((s32 *)block->pHeader)[1];
    for (i = 0; i < entryCount; i++) {
        P2A_ANIMATION_tag *entry = &block->pAnimTable[i];

        if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
            u32 frameCount = entry->times;

            block->AnimNum = frameCount;
            printf("billAnim no[%d][%d]...PLURAL\n", i, frameCount);
        } else if (entry->attr & 0x40) {
            block->AnimNum = 2;
            printf("billAnim no[%d][%d]...MTEX\n", i, 2);
        } else if (entry->attr & 0x80) {
            block->AnimNum = 2;
            printf("billAnim no[%d][%d]...AMTEX\n", i, 2);
        }
    }
    return block;
}

void dds3BillAnimFreeP2A(dds3BillAnimResource_tag *hBillAnimRes) {
    s32 listRefCount = hBillAnimRes->count - 1;

    hBillAnimRes->count = listRefCount;
    if (listRefCount == 0) {
        s32 i = 0;

        printf("BillManager P2A Free\n");
        for (; i < hBillAnimRes->TexNum; i++) {
            BillboardFreeTex(hBillAnimRes->pResList[i]);
        }
        sdfMemFreeDirect(hBillAnimRes->hMem);
    }
}

void dds3BillboardCopyO(billHandle_t *obj, s32 *out) {
    dds3BillResource_tag *child;

    if (obj->type == 1) {
        child = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
        out[0] = (s32)child->hTex;
        out[1] = *(s32 *)&child->uv;
    }
}

billHandle_t *dds3BillboardCreate(s32 index, u32 data) {
    billHandle_t *newobj;

    newobj = dds3BillboardFuncTable[index].create(data);
    dds3Billboard_BasicInit(newobj);
    newobj->type = index;
    newobj->callback = dds3BillboardFuncTable[index].draw;
    return newobj;
}

billHandle_t *dds3BillboardFileLoad(s32 index, const char *name) {
    sdfMemHandle_t *allocation;
    u32 data;
    billHandle_t *obj;

    printf("BillManager Load...[%s]\n", name);
    allocation = sdfDevLoadFileEx(name, &data, 0);
    obj = dds3BillboardCreate(index, data);
    sdfMemFreeDirect(allocation);
    return obj;
}

billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard) {
    billHandle_t *copy;
    dds3BillResource_tag *data;
    u16 sourceKind;
    void (*sourceCallback)();

    if (hBillboard->type == 1) {
        copy = dds3Billboard_P2ACopy(hBillboard);
        dds3Billboard_BasicInit(copy);
        copy->type = hBillboard->type;
        copy->callback = hBillboard->callback;
    } else {
        copy = dds3Billboard_NormalCreate(NULL);
        dds3Billboard_BasicInit(copy);
        sourceKind = hBillboard->type;
        data = hBillboard->u.child;
        sourceCallback = hBillboard->callback;
        copy->type = sourceKind;
        copy->callback = sourceCallback;
        data->count = data->count + 1;
        copy->u.child = data;
    }
    return copy;
}

void dds3BillboardDelete(billHandle_t *hData) {
    dds3BillboardFuncTable[hData->type].destroy();
}

void dds3BillboardDraw(billHandle_t *hData) {
    hData->callback();
}

billHandle_t *dds3BillboardGeneral(s32 number) {
    billHandle_t *obj;
    dds3BillResource_tag *child;
    s32 refCount;

    obj = dds3BillboardCreate(BillboardTable[number].kind, 0);
    child = hGeneralBillboard[number]->u.child;
    refCount = child->count;
    obj->u.child = child;
    child->count = refCount + 1;
    return obj;
}

u32 dds3BillboardGeneralNumber(void) {
    return 8;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number) {
    return hGeneralBillboard[number]->u.child->hTex;
}

void dds3BillboardSetPosition(void *hBillboard, const void *pPos) {
    SDF_VU0_COPYVECTOR(hBillboard, pPos);
}

/* Set the radian rotation applied to the billboard quad corners. */
void dds3BillboardSetRotate(billHandle_t *hBillboard, float rot) {
    hBillboard->rot = rot;
}

/* Set the two child scale components stored at +0x10 and +0x14. */
void dds3BillboardSetScale(billHandle_t *hBillboard, float x, float y) {
    hBillboard->childScaleX = x;
    hBillboard->childScaleY = y;
}

void dds3BillboardSetRGBA(billHandle_t *hBillboard, u32 abgr) {
    hBillboard->abgr = abgr;
}

void func_0015D050(billHandle_t *effect, const billUvQuad_t *textureQuad) {
    if (effect->type == 0) {
        memcpy(&effect->u.child->uv, textureQuad, sizeof(*textureQuad));
    }
}

void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode) {
    mode = (s16)mode;
    switch (hBillboard->type) {
    case 0:
    case 3:
        hBillboard->mode = mode;
        break;
    case 1: {
        s32 entryCount = hBillboard->entryCount;
        if (entryCount > 0) {
            s32 remaining = entryCount;
            dds3BillAnim_tag *entries = hBillboard->resolvedEntries;
            s32 index = 0;
            do {
                P2A_ANIMATION_tag *entry = entries[index].pAnim;
                u32 flags = entry->mode & ~BILL_ANIMATION_FLAG_PACKET_LIST_MASK;
                entry->mode = flags;
                if (mode == 2) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_2;
                } else if (mode == 3) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_3;
                }
                index++;
            } while (--remaining != 0);
        }
        break;
    }
    }
}

/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition) {
    s32 remainingChildren;
    dds3BillResource_tag **childEntries;
    dds3BillResource_tag **childCursor;

    condition = (s16)condition;
    switch (hBillboard->type) {
    case 0:
        hBillboard->u.child->type = (u16)condition;
        break;
    case 1:
        remainingChildren = hBillboard->u.animationData->TexNum;
        childEntries = hBillboard->u.animationData->pResList;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->type = (u16)condition;
                childCursor++;
                remainingChildren--;
            } while (remainingChildren != 0);
        }
        break;
    }
}

s32 dds3BillboardGetTex(billHandle_t *hBillboard) {
    if (hBillboard->type == 0) {
        return (s32)hBillboard->u.child->hTex;
    }
    return 0;
}

u16 dds3BillboardGetType(billHandle_t *hBillboard) {
    return hBillboard->type;
}

void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type) {
    s32 maskedValue = type & BILL_VARIANT_MASK;

    switch (hBillboard->type) {
    case 0:
        hBillboard->u.child->type = (u16)maskedValue;
        break;
    case 1:
        hBillboard->pair.cameraFacingMode = maskedValue;
        break;
    }
}

u16 dds3BillboardGetDrawType(billHandle_t *hBillboard) {
    switch (hBillboard->type) {
    case 0:
        return hBillboard->u.child->type;
    case 1:
        return hBillboard->pair.cameraFacingMode;
    default:
        return 0;
    }
}

void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number) {
    if (hBillboard->type == 1 && hBillboard->animationEntryIndex != number) {
        dds3Billboard_P2ASetNumber(hBillboard, number);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->animationEntryIndex;
    }
    return 0;
}

s32 dds3BillboardGetAnimNumber(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return ((P2A_HEAD_t *)hBillboard->u.animationData->pHeader)->AnimNum;
    }
    return 0;
}

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame) {
    if (hBillboard->type == 1) {
        s32 entryCount = hBillboard->entryCount;

        if (entryCount > 0) {
            dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
            s32 remaining = entryCount;

            do {
                P2A_ANIMATION_tag *frameData = entry->pAnim;
                u32 wrappedFrame = frame % frameData->times;
                remaining -= 1;
                entry->wait = 0;
                entry->key = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

void dds3BillboardSetAnimKey2(billHandle_t *hBillboard, u32 frame) {
    if (hBillboard->type == 1) {
        s32 entryCount = hBillboard->entryCount;

        if (entryCount > 0) {
            dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
            s32 remaining = entryCount;

            do {
                P2A_ANIMATION_tag *frameData = entry->pAnim;
                u32 wrappedFrame = frame % frameData->times;
                remaining -= 1;
                entry->wait = 1;
                entry->key = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        const dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
        const P2A_ANIMATION_tag *frame = entry->pAnim;
        return frame->times;
    }
    return 0;
}

u16 dds3BillboardGetAnimState(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->animationActive;
    }
    return 0;
}

s32 dds3BillboardGetAnimType(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->modeFlags;
    }
    return 0;
}

void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        hBillboard->modeFlags |= 0x1000000;
    }
}

void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y) {
    if (hBillboard->type == 0) {
        dds3BillResource_tag *child = hBillboard->u.child;
        child->width = x * 0.5f;
        child->height = y * 0.5f;
    }
}

/* vu0 routine: screen-space angle of a world axis at a world position. */
f32 dds3BillboardGetDirectionRotate(void *pPos, f32 *pVec) {
    f32 delta[4];
    f32 origin[4];
    f32 tip[4];

    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_LOADVECTOR(pPos);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(origin);
    SDF_VU0_LOADVECTOR(pVec);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(100.0f);
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(tip);
    delta[0] = origin[0] - tip[0];
    delta[1] = origin[1] - tip[1];
    delta[2] = 0.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(delta[0]);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_MEMOPS(delta[0]);
    return atan2f(delta[1], delta[0]);
}

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec) {
    f32 dot;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(pVec);
    SDF_VU0_INNERPRODUCT(dot);
    dot = sdfFabs(dot);
    return 1.0f - dot;
}

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCreate(1, entryIndex);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    SDF_VU0_STOREIDENTITY(instance->rot);
    return instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCopy(
        source->hBillboard);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    sdfMatSetDiffuse(instance->pMat, 0x80808080);
    sdfMatSetAmbient(instance->pMat, 0x80808080);
    sdfMatSetSpecular(instance->pMat, 0x80808080);
    SDF_VU0_STOREIDENTITY(instance->rot);
    SDF_VU0_STOREIDENTITY(instance->dir);
    return instance;
}

void dds3P2APolygonDelete(dds3P2APolygon_tag *hData) {
    sdfMatFreeDelayed((sdfMat_t *)hData->pMat);
    dds3BillboardDelete(hData->hBillboard);
    sdfChipFreeDirect(hData);
}

void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos) {
    dds3BillboardSetPosition((s128 *)hData->hBillboard, pPos);
}

void dds3P2APolygonSetScale(s32 hData, float scale) {
    dds3BillboardSetScale(((dds3P2APolygon_tag *)hData)->hBillboard, scale, scale);
}

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(dds3P2APolygon_tag *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STOREMATRIX(hData->rot);
}

void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr) {
    dds3BillboardSetRGBA(hData->hBillboard, abgr);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *hData, void *pMatrix) {
    VU0_COPY_MATRIX(hData, pMatrix);
}

typedef struct {
    s32 mode;
    dds3BillResource_tag *primary;
    dds3BillResource_tag *secondary;
} dds3P2APolygonResource_t;

void dds3P2APolygonGetResource(dds3P2APolygon_tag *hP2APoly, dds3P2APolygonResource_t *pP2APolyRes) {
    billHandle_t *billboard = hP2APoly->hBillboard;

    if (billboard->type == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            pP2APolyRes->mode = 2;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else if (modeFlags & 0x80) {
            pP2APolyRes->mode = 3;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else {
            pP2APolyRes->mode = 0;
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
        }
    }
}


extern u32 D_003170B0[];
extern u32 D_003170C0[];
extern sdfGrOt_t *D_003170D0[];
extern void MultMatrixNoArg(void);
extern void sdf3DPrimLinkTm(sdfDrawTag_t *, s32 (*)(s32));
extern void sdf3DPrimLinkMat(sdfDrawTag_t *, sdfMat_t *, s32 (*)(s32));
extern void sdfMatSetTex(sdfMat_t *, sdfTexHandle_t *);
extern void sdfMatSetMultiTex(sdfMat_t *, sdfTexHandle_t *);
extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void dds3P2APolygonDraw(dds3P2APolygon_tag *instance) {
    sdf3DPrimPolyParm_t params;
    dds3P2APolygonResource_t resources;
    /* VU0 scratch is reloaded after its translation row is replaced. */
    volatile f32 matrix[4][4] __attribute__((aligned(16)));
    f32 positions[4][4];
    f32 uv[4][2];
    f32 secondaryUv[4][2];
    sdfDrawTag_t *list;
    dds3BillResource_tag *child;
    sdfTexHandle_t *texture;
    sdfGrOt_t *surface;
    f32 x, y;
    f32 scaleX, scaleY;
    f32 cornerX, cornerY;
    f32 cosine, sine;
    f32 width, height;

    if (!dds3BillboardGetAnimState(instance->hBillboard)) {
        return;
    }
    dds3P2APolygonGetResource(instance, &resources);
    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    SDF_VU0_LOADMATRIX(instance->rot);
    SDF_VU0_LOADMATRIX_B(instance->dir);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX_NOMEM(matrix);
    matrix[3][0] = instance->hBillboard->pos[0];
    matrix[3][1] = instance->hBillboard->pos[1];
    matrix[3][2] = instance->hBillboard->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    memset(&params, 0, sizeof(params));
    params.attr = 0x4000;
    params.faceIndex = D_003170B0;
    params.vertex = (sdfVector4_t *)positions;
    params.vertexColor = D_003170C0;
    params.uv = (sdfVector2_t *)uv;
    params.color = instance->hBillboard->abgr;
    params.faceCount = 2;
    params.pointCount = 4;
    child = resources.primary;
    scaleX = instance->hBillboard->childScaleX;
    scaleY = instance->hBillboard->childScaleY;
    x = child->cx * scaleX;
    y = child->cy * scaleY;
    width = child->width * scaleX;
    height = child->height * scaleY;
    cosine = sdfCos(instance->hBillboard->rot);
    sine = sdfSin(instance->hBillboard->rot);
    cornerX = x - width;
    cornerY = y - height;
    positions[0][0] = cornerX * cosine - cornerY * sine;
    positions[0][1] = cornerX * sine + cornerY * cosine;
    positions[0][2] = 0.0f;
    cornerX = x + width;
    cornerY = y - height;
    positions[1][0] = cornerX * cosine - cornerY * sine;
    positions[1][1] = cornerX * sine + cornerY * cosine;
    positions[1][2] = 0.0f;
    cornerX = x + width;
    cornerY = y + height;
    positions[3][0] = cornerX * cosine - cornerY * sine;
    positions[3][1] = cornerX * sine + cornerY * cosine;
    positions[3][2] = 0.0f;
    cornerX = x - width;
    cornerY = y + height;
    positions[2][0] = cornerX * cosine - cornerY * sine;
    positions[2][1] = cornerX * sine + cornerY * cosine;
    positions[2][2] = 0.0f;
    width = 1.0f / (f32)(child->hTex->width * 16);
    height = 1.0f / (f32)(child->hTex->height * 16);
    texture = child->hTex;
    uv[0][0] = child->uv.components[0] * width;
    uv[0][1] = child->uv.components[1] * height;
    uv[1][0] = child->uv.components[2] * width;
    uv[1][1] = child->uv.components[3] * height;
    uv[2][0] = child->uv.components[6] * width;
    uv[2][1] = child->uv.components[7] * height;
    uv[3][0] = child->uv.components[4] * width;
    uv[3][1] = child->uv.components[5] * height;
    sdfMatSetTex(instance->pMat, texture);
    switch ((u32)resources.mode) {
    case 2:
    case 3:
        child = resources.secondary;
        scaleX = instance->hBillboard->childScaleX;
        scaleY = instance->hBillboard->childScaleY;
        x = child->cx * scaleX;
        y = child->cy * scaleY;
        width = child->width * scaleX;
        height = child->height * scaleY;
        params.attr |= 0x1000;
        params.multiUv = (sdfVector2_t *)secondaryUv;
        cosine = sdfCos(instance->hBillboard->rot);
        sine = sdfSin(instance->hBillboard->rot);
        cornerX = x - width;
        cornerY = y - height;
        positions[0][0] = cornerX * cosine - cornerY * sine;
        positions[0][1] = cornerX * sine + cornerY * cosine;
        positions[0][2] = 0.0f;
        cornerX = x + width;
        cornerY = y - height;
        positions[1][0] = cornerX * cosine - cornerY * sine;
        positions[1][1] = cornerX * sine + cornerY * cosine;
        positions[1][2] = 0.0f;
        cornerX = x + width;
        cornerY = y + height;
        positions[3][0] = cornerX * cosine - cornerY * sine;
        positions[3][1] = cornerX * sine + cornerY * cosine;
        positions[3][2] = 0.0f;
        cornerX = x - width;
        cornerY = y + height;
        positions[2][0] = cornerX * cosine - cornerY * sine;
        positions[2][1] = cornerX * sine + cornerY * cosine;
        positions[2][2] = 0.0f;
        texture = child->hTex;
        width = (f32)(texture->width * 16);
        height = (f32)(texture->height * 16);
        secondaryUv[0][0] = child->uv.components[0] / width;
        secondaryUv[0][1] = child->uv.components[1] / height;
        secondaryUv[1][0] = child->uv.components[2] / width;
        secondaryUv[1][1] = child->uv.components[3] / height;
        secondaryUv[2][0] = child->uv.components[6] / width;
        secondaryUv[2][1] = child->uv.components[7] / height;
        secondaryUv[3][0] = child->uv.components[4] / width;
        secondaryUv[3][1] = child->uv.components[5] / height;
        sdfMatSetMultiTex(instance->pMat, texture);
        break;
    }
    sdf3DPrimLinkMat(list, instance->pMat, 0);
    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&params));
    surface = D_003170D0[instance->hBillboard->mode];
    surface->regFunc(surface, list);
}

