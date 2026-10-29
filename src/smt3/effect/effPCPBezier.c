#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "effect.h"

typedef struct effBezierBED_t effBezierBED_t;
typedef struct effBezierCut_t effBezierCut_t;


typedef struct effBezierPlacement_t {
    f32 payload[11];
    u32 color;
} effBezierPlacement_t;

/* One retained fragment-history header follows its point/color buffers. */
typedef struct dds3BezierTrackPoly_tag {
    u32 abgr;                              /* 0x00 */
    s32 blend;                       /* 0x04 */
    s32 point;                      /* 0x08: history point capacity */
    s32 count;                   /* 0x0C */
    s32 current;                /* 0x10: first point of the next three-point sample */
    s32 split;                       /* 0x14: retained input value */
    u128 *pVertex;                           /* 0x18 */
    u32 *pVC;                            /* 0x1C */
    sdfMat_t *pMat;               /* 0x20 */
    sdfMemHandle_t *hMem;                /* 0x24 */
    u128 *pHeadVertex;                        /* 0x28 */
    u32 *pHeadVC;                         /* 0x2C */
} dds3BezierTrackPoly_tag;

/* Copied 0x50-byte input for a group of thunder-fragment ribbons. */
typedef struct effPCPBezierBillParam_t {
    f32 origin[4];                          /* 0x00 */
    u32 mode;                               /* 0x10: retains native selector values */
    f32 spreadX;                            /* 0x14 */
    f32 spreadZ;                            /* 0x18 */
    f32 height;                             /* 0x1C */
    u32 number;                         /* 0x20 */
    s32 curveFrames;                        /* 0x24 */
    s32 startDelayRange;                    /* 0x28 */
    u32 fade_in;                              /* 0x2C */
    s32 fadeFrames;                         /* 0x30 */
    f32 width;                              /* 0x34 */
    s32 historyLength;                      /* 0x38 */
    s32 subdivisions;                       /* 0x3C */
    u32 palette[4];                         /* 0x40: inner-start, outer-start, inner-end, outer-end */
} effPCPBezierBillParam_t;

/* One 0x80-byte moving ribbon slot plus its event work. */
typedef struct effPCPBezierBillPart_tag {
    f32 pos[4];                        /* 0x00: current sampled point */
    s32 frame;                                /* 0x10: negative delay, then elapsed frames */
    f32 alpha;                          /* 0x14 */
    dds3BezierTrackPoly_tag *hBztp;/* 0x18 */
    effBezierCurve_t Bezier7P;           /* 0x1C */
    struct effBezierCut_t *hBEDCut;          /* 0x7C */
} effPCPBezierBillPart_tag;

/* Allocation header followed by groupCount inline 0x80-byte slots. */
typedef struct effPCPBezierBill_tag {
    effPCPBezierBillParam_t param;         /* 0x00 */
    effPCPBezierBillPart_tag *pPart;           /* 0x50 */
    u32 abgr;                           /* 0x54 */
    struct effBezierBED_t *hBED;           /* 0x58 */
    u8 original;                      /* 0x5C */
    u8 pad5D[3];                             /* 0x5D */
    sdfMemHandle_t *hMem;                 /* 0x60 */
} effPCPBezierBill_tag;


/* Vertex-draw descriptor used by the thunder history and end-cap packets.
 * Both title providers clear the first 0x2C bytes and keep these globals in
 * 0x30-byte slots; the final four bytes remain opaque. */
typedef struct effBezierDrawParam_t {
    s16 primitiveCount; /* 0x00 */
    s16 pointCount;     /* 0x02 */
    u16 flags;          /* 0x04 */
    u8 opaque_06[2];
    u32 color;          /* 0x08 */
    const u32 *primitiveIndices; /* 0x0C */
    u128 *points;       /* 0x10 */
    u8 opaque_14[0xC];
    u32 *colors;        /* 0x20 */
    u8 opaque_24[0xC];
} effBezierDrawParam_t;


extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
extern void sdfMatFreeDelayed(sdfMat_t *aMat);
extern void *memcpy(void *a, const void *b, u32 c);
extern void *memset(void *a, s32 b, u32 c);
extern effBezierBED_t *effEveBEDCreate(effBezierBED_t *pData);
extern void effEveBEDDelete(effBezierBED_t *hEveBED);
extern effBezierCut_t *effEveBEDCutCreate(effBezierBED_t *hEveBED, s32 CutNum, effBezierPlacement_t *pParams);
extern void effEveBEDCutDelete(effBezierCut_t *hEveBEDCut);
extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, void *aAllocator);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, void *aAllocator);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern void *effPkt3DPrimMakePoly(effBezierDrawParam_t *aParm);

extern effBezierDrawParam_t dds3BezierVertexCount, dds3BezierTrackPolyVertex;
extern const f32 D_0031F0B0[8][4];
extern const u32 D_0031F020[28], D_0031F090[8];
extern sdfGrOt_t *dds3BezierTrackPolyParam[];

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex);
void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split);
void dds3BezierTrackPolyDelete(dds3BezierTrackPoly_tag *hPoly);

effPCPBezierBill_tag *effPCPBezierBillCreate(const effPCPBezierBillParam_t *pParam, effBezierBED_t *pBEDData)
{
    u32 count = pParam->number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effPCPBezierBillPart_tag) + sizeof(effPCPBezierBill_tag));
    effPCPBezierBill_tag *work = (effPCPBezierBill_tag *)sdfMemLock(allocation);
    effPCPBezierBillPart_tag *slot = (effPCPBezierBillPart_tag *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->param = *pParam;
    work->hMem = allocation;
    work->original = 1;
    work->pPart = slot;
    work->abgr = 0x80808080;
    work->hBED = effEveBEDCreate(pBEDData);
    historyLength = work->param.historyLength;
    if (historyLength == 0) {
        work->param.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->param.subdivisions;
    if (subdivisions <= 0) {
        work->param.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->param.startDelayRange <= 0) {
        work->param.startDelayRange = 1;
    }
    palette[0] = work->param.palette[0];
    palette[1] = work->param.palette[1];
    palette[2] = work->param.palette[2];
    palette[3] = work->param.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->hBztp = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->hBztp, palette);
        slot->frame = 0;
        slot->alpha = 1.0f;
        slot->Bezier7P.pointIndex = 0;
        slot->Bezier7P.t = 0;
        slot->Bezier7P.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}

effPCPBezierBill_tag *effPCPBezierBillCreatePCF(void *paramTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(paramTable, 0);
    secondBlock = dds3EffectPCPDataGetData(paramTable, 1);
    return effPCPBezierBillCreate(firstBlock, secondBlock);
}

effPCPBezierBill_tag *effPCPBezierBillCopy(effPCPBezierBill_tag *src) {
    u32 count = src->param.number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effPCPBezierBillPart_tag) + sizeof(effPCPBezierBill_tag));
    effPCPBezierBill_tag *work = (effPCPBezierBill_tag *)sdfMemLock(allocation);
    effPCPBezierBillPart_tag *slot = (effPCPBezierBillPart_tag *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->param = src->param;
    work->original = 0;
    work->hMem = allocation;
    work->hBED = src->hBED;
    work->pPart = slot;
    work->abgr = 0x80808080;
    historyLength = work->param.historyLength;
    if (historyLength == 0) {
        work->param.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->param.subdivisions;
    if (subdivisions <= 0) {
        work->param.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->param.startDelayRange <= 0) {
        work->param.startDelayRange = 1;
    }
    palette[0] = work->param.palette[0];
    palette[1] = work->param.palette[1];
    palette[2] = work->param.palette[2];
    palette[3] = work->param.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->hBztp = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->hBztp, palette);
        slot->frame = 0;
        slot->alpha = 1.0f;
        slot->Bezier7P.pointIndex = 0;
        slot->Bezier7P.t = 0;
        slot->Bezier7P.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}

void effPCPBezierBillDelete(effPCPBezierBill_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.number;
    effPCPBezierBillPart_tag *slot = hData->pPart;

    if (count != 0) {
        do {
            i++;
            dds3BezierTrackPolyDelete(slot->hBztp);
            effEveBEDCutDelete(slot->hBEDCut);
            slot++;
        } while (i < count);
    }
    if (hData->original != 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfMemFreeDirect(hData->hMem);
}

extern effRandState_t dds3EffectBase[];
extern sdfVector4_t aViewEye, aViewTarget;
extern u32 effBTLSkillCheckRun(void);
extern s32 effBTLSkillGetTargetFormation(s32 number);
extern s32 effBTLSkillGetInvokeFormation(void);
extern void effBTLModelGetSphereCenter(s32 pFormation);
extern f32 dds3GetRandFloatA(effRandState_t *name);
extern u32 dds3GetRandA(effRandState_t *name);
extern void SetRotateMatrixAxis(f32 aAngle, const f32 *aAxis);
extern s32 effPCPBezierEval(effBezierCurve_t *curve, f32 *out);
extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);
extern f32 sdfAtan2(f32 aY, f32 aX);
extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);
extern void effEveBEDCutSetParams(effBezierCut_t *hEveBEDCut, const effBezierPlacement_t *pParams);
extern void effEveBEDCutDraw(effBezierCut_t *hEveBEDCut);

void dds3BezierTrackPolyReset(dds3BezierTrackPoly_tag *hPoly);
void dds3BezierTrackPolySetVC(dds3BezierTrackPoly_tag *hPoly, u128 *pABGR);
void dds3BezierTrackPolyDraw(dds3BezierTrackPoly_tag *hPoly);

/* The ribbon and end-cap vectors are full 16-byte-aligned VU objects.
 * This donor-backed SDK store/readback pipeline is audited specifically here;
 * see docs/bezier-matching.md before changing its memory-access boundaries. */
typedef f32 effBezierVector_t[4] __attribute__((aligned(16)));

void effPCPBezierBillDraw(effPCPBezierBill_tag *group)
{
    effBezierPlacement_t place __attribute__((aligned(16)));
    effBezierVector_t start;
    effBezierVector_t direction;
    effBezierVector_t offset;
    effBezierVector_t end;
    effBezierVector_t point;
    effBezierVector_t origin;
    effBezierVector_t previous;
    effBezierVector_t cameraDirection;
    effBezierVector_t width;
    effBezierVector_t delta;
    effBezierVector_t ribbon[3];
    effBezierVector_t side;
    effBezierVector_t oppositeSide;
    effBezierVector_t tangent;
    f32 distance, yOffset, zOffset, xOffset;
    f32 along;
    f32 spread;
    f32 pullback;
    u32 delayRange;
    u32 i, j;
    u32 count;
    f32 curveFrames;
    u32 subdivisions;
    f32 fadeStep;
    u32 mode;
    u32 color;
    effPCPBezierBillPart_tag *slot;
    f32 (*cap)[4];

    count = group->param.number;
    curveFrames = group->param.curveFrames;
    subdivisions = group->param.subdivisions - 1;
    fadeStep = 1.0f / group->param.fadeFrames;
    delayRange = group->param.startDelayRange;
    mode = group->param.mode;
    color = group->abgr;
    slot = group->pPart;
    SDF_VU0_COPYVECTOR(origin, group->param.origin);
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    if (effBTLSkillCheckRun()) {
        s32 formation;
        if (mode == 3) formation = effBTLSkillGetTargetFormation(0);
        else formation = effBTLSkillGetInvokeFormation();
        effBTLModelGetSphereCenter(formation);
        SDF_VU0_STOREVECTOR_NOMEM(start);
    } else {
        start[0] = 0.0f;
        start[1] = -80.0f;
        start[2] = -400.0f;
    }
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    width[0] = group->param.width * 0.5f;
    width[1] = group->param.width * 0.5f;
    width[2] = group->param.width * 0.5f;
    for (i = 0; i < count; i++, slot++) {
        if (slot->Bezier7P.parameterStep == 0.0f) {
            yOffset = -group->param.height * dds3GetRandFloatA(dds3EffectBase);
            zOffset = group->param.spreadZ * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
            xOffset = group->param.spreadX * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
            end[0] = origin[0] + xOffset;
            end[1] = origin[1] - yOffset;
            end[2] = origin[2] + zOffset;
            SDF_VU0_LOADVECTOR(end);
            SDF_VU0_LOADVECTOR_B(start);
            SDF_VU0_SUBVECTOR_EXTENDED();
            SDF_VU0_GETLENGTH(distance);
            SDF_VU0_NORMALIZE_EXTENDED();
            SDF_VU0_STOREVECTOR_NOMEM(direction);
            switch (mode) {
            case 0:
                offset[0] = ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
                offset[1] = -dds3GetRandFloatA(dds3EffectBase);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * 0.5235987f, direction);
                spread = 50.0f;
                slot->Bezier7P.controlPoints[0].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[0].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[0].z = offset[2] * spread + start[2];
                if (slot->Bezier7P.controlPoints[0].y > -50.0f) slot->Bezier7P.controlPoints[0].y = -50.0f;
                spread = 200.0f;
                pullback = 150.0f;
                slot->Bezier7P.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->Bezier7P.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->Bezier7P.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                if (slot->Bezier7P.controlPoints[1].y > -30.0f) slot->Bezier7P.controlPoints[1].y = -30.0f;
                spread = 300.0f;
                slot->Bezier7P.controlPoints[2].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[2].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[2].z = offset[2] * spread + start[2];
                if (slot->Bezier7P.controlPoints[2].y > -20.0f) slot->Bezier7P.controlPoints[2].y = -20.0f;
                along = distance * 0.25f;
                spread = 200.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_APPLYMATRIX_XYZ_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                slot->Bezier7P.controlPoints[3].x = spread * offset[0] + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[3].y = spread * offset[1] + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[3].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->Bezier7P.controlPoints[3].y > -10.0f) slot->Bezier7P.controlPoints[3].y = -10.0f;
                along = distance * 0.5f;
                spread = 150.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_APPLYMATRIX_XYZ_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                slot->Bezier7P.controlPoints[4].x = spread * offset[0] + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[4].y = spread * offset[1] + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[4].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->Bezier7P.controlPoints[4].y > -10.0f) slot->Bezier7P.controlPoints[4].y = -10.0f;
                along = distance * 0.75f;
                spread = 65.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_APPLYMATRIX_XYZ_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                slot->Bezier7P.controlPoints[5].x = spread * offset[0] + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[5].y = spread * offset[1] + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[5].z = spread * offset[2] + along * direction[2] + start[2];
                if (slot->Bezier7P.controlPoints[5].y > -10.0f) slot->Bezier7P.controlPoints[5].y = -10.0f;
                along = distance;
                slot->Bezier7P.controlPoints[6].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[6].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[6].z = along * direction[2] + start[2];
                if (slot->Bezier7P.controlPoints[6].y > -10.0f) slot->Bezier7P.controlPoints[6].y = -10.0f;
                break;
            case 1:
                offset[0] = ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * 0.5f;
                offset[1] = -dds3GetRandFloatA(dds3EffectBase);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                spread = 25.0f;
                slot->Bezier7P.controlPoints[0].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[0].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[0].z = offset[2] * spread + start[2];
                spread = 100.0f;
                pullback = 150.0f;
                slot->Bezier7P.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->Bezier7P.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->Bezier7P.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                spread = 400.0f;
                slot->Bezier7P.controlPoints[2].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[2].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[2].z = offset[2] * spread + start[2];
                along = distance * 0.25f;
                spread = 450.0f;
                slot->Bezier7P.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.5f;
                spread = 500.0f;
                slot->Bezier7P.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 300.0f;
                slot->Bezier7P.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->Bezier7P.controlPoints[6].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[6].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 2:
                offset[0] = ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
                offset[1] = -dds3GetRandFloatA(dds3EffectBase);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                spread = 20.0f;
                slot->Bezier7P.controlPoints[0].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[0].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[0].z = offset[2] * spread + start[2];
                along = distance * 0.15f;
                spread = 50.0f;
                slot->Bezier7P.controlPoints[1].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[1].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[1].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.3f;
                spread = 80.0f;
                slot->Bezier7P.controlPoints[2].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[2].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[2].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.45f;
                spread = 60.0f;
                slot->Bezier7P.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.6f;
                spread = 40.0f;
                slot->Bezier7P.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 20.0f;
                slot->Bezier7P.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->Bezier7P.controlPoints[6].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[6].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 3:
                offset[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
                offset[1] = -(dds3GetRandFloatA(dds3EffectBase) * 0.3f + 0.7f);
                offset[2] = 0.0f;
                SDF_VU0_LOADVECTOR(offset);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(offset);
                spread = 25.0f;
                slot->Bezier7P.controlPoints[0].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[0].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[0].z = offset[2] * spread + start[2];
                spread = 100.0f;
                pullback = 150.0f;
                slot->Bezier7P.controlPoints[1].x = (offset[0] * spread - direction[0] * pullback) + start[0];
                slot->Bezier7P.controlPoints[1].y = (offset[1] * spread - direction[1] * pullback) + start[1];
                slot->Bezier7P.controlPoints[1].z = (offset[2] * spread - direction[2] * pullback) + start[2];
                spread = 400.0f;
                slot->Bezier7P.controlPoints[2].x = offset[0] * spread + start[0];
                slot->Bezier7P.controlPoints[2].y = offset[1] * spread + start[1];
                slot->Bezier7P.controlPoints[2].z = offset[2] * spread + start[2];
                along = distance * 0.25f;
                spread = 450.0f;
                slot->Bezier7P.controlPoints[3].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[3].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[3].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.5f;
                spread = 500.0f;
                slot->Bezier7P.controlPoints[4].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[4].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[4].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance * 0.75f;
                spread = 300.0f;
                slot->Bezier7P.controlPoints[5].x = offset[0] * spread + along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[5].y = offset[1] * spread + along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[5].z = offset[2] * spread + along * direction[2] + start[2];
                along = distance;
                slot->Bezier7P.controlPoints[6].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[6].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[6].z = along * direction[2] + start[2];
                break;
            case 4:
                along = distance * 0.14f;
                slot->Bezier7P.controlPoints[0].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[0].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[0].z = along * direction[2] + start[2];
                along = distance * 0.28f;
                slot->Bezier7P.controlPoints[1].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[1].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[1].z = along * direction[2] + start[2];
                along = distance * 0.42f;
                slot->Bezier7P.controlPoints[2].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[2].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[2].z = along * direction[2] + start[2];
                along = distance * 0.56f;
                slot->Bezier7P.controlPoints[3].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[3].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[3].z = along * direction[2] + start[2];
                along = distance * 0.7f;
                slot->Bezier7P.controlPoints[4].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[4].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[4].z = along * direction[2] + start[2];
                along = distance * 0.84f;
                slot->Bezier7P.controlPoints[5].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[5].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[5].z = along * direction[2] + start[2];
                along = distance;
                slot->Bezier7P.controlPoints[6].x = along * direction[0] + start[0];
                slot->Bezier7P.controlPoints[6].y = along * direction[1] + start[1];
                slot->Bezier7P.controlPoints[6].z = along * direction[2] + start[2];
                break;
            }
            slot->Bezier7P.pointIndex = 0;
            slot->Bezier7P.t = 0.0f;
            slot->Bezier7P.parameterStep = 3.0f / (curveFrames * (subdivisions + 1));
            effPCPBezierEval(&slot->Bezier7P, point);
            slot->pos[0] = point[0];
            slot->pos[1] = point[1];
            slot->pos[2] = point[2];
            slot->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayRange);
            dds3BezierTrackPolyReset(slot->hBztp);
        } else if (slot->frame++ >= 0) {
            if (slot->Bezier7P.t >= 1.0f && slot->Bezier7P.pointIndex >= 3) {
                if (slot->alpha > fadeStep) {
                    slot->alpha -= fadeStep;
                    slot->hBztp->abgr = ((u32)(slot->alpha * 128.0f) << 24) | 0x808080;
                    slot->hBztp->abgr = effMagaCylinderCalcColor(color, slot->hBztp->abgr);
                } else {
                    dds3BezierTrackPolyReset(slot->hBztp);
                }
            } else {
                previous[0] = slot->pos[0];
                previous[1] = slot->pos[1];
                previous[2] = slot->pos[2];
                for (j = 0; j < subdivisions; j++) {
                    effPCPBezierEval(&slot->Bezier7P, point);
                    SDF_VU0_LOADVECTOR(point);
                    SDF_VU0_COPYVECTOR_A_TO_C_EXTENDED();
                    SDF_VU0_LOADVECTOR_B(previous);
                    SDF_VU0_SUBVECTOR_EXTENDED();
                    SDF_VU0_LOADVECTOR_B(cameraDirection);
                    SDF_VU0_OUTERPRODUCT_EXTENDED();
                    SDF_VU0_NORMALIZE_EXTENDED();
                    SDF_VU0_LOADVECTOR_B(width);
                    SDF_VU0_MULVECTOR_EXTENDED();
                    SDF_VU0_COPYVECTOR_A_TO_TEMP_EXTENDED();
                    SDF_VU0_COPYVECTOR_C_TO_A_EXTENDED();
                    SDF_VU0_COPYVECTOR_TEMP_TO_C_EXTENDED();
                    SDF_VU0_STOREVECTOR_NOMEM(ribbon[1]);
                    SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
                    SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                    SDF_VU0_STOREVECTOR_NOMEM(ribbon[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A_EXTENDED();
                    SDF_VU0_SUBVECTOR_B_A_EXTENDED();
                    SDF_VU0_STOREVECTOR_B_NOMEM(ribbon[2]);
                    SDF_VU0_COPYVECTOR(previous, point);
                    dds3BezierTrackPolySetVC(slot->hBztp, (u128 *)ribbon);
                }
                effPCPBezierEval(&slot->Bezier7P, point);
                SDF_VU0_LOADVECTOR(point);
                SDF_VU0_COPYVECTOR_A_TO_C_EXTENDED();
                SDF_VU0_LOADVECTOR_B(previous);
                SDF_VU0_SUBVECTOR_EXTENDED();
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(tangent);
                SDF_VU0_LOADVECTOR_B(cameraDirection);
                SDF_VU0_OUTERPRODUCT_EXTENDED();
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(side);
                SDF_VU0_LOADVECTOR_B(width);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_COPYVECTOR_A_TO_TEMP_EXTENDED();
                SDF_VU0_COPYVECTOR_C_TO_A_EXTENDED();
                SDF_VU0_COPYVECTOR_TEMP_TO_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(ribbon[1]);
                SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(ribbon[0]);
                SDF_VU0_COPYVECTOR_C_TO_A_EXTENDED();
                SDF_VU0_SUBVECTOR_B_A_EXTENDED();
                SDF_VU0_STOREVECTOR_B_NOMEM(ribbon[2]);
                dds3BezierTrackPolySetVC(slot->hBztp, (u128 *)ribbon);
                cap = (f32 (*)[4])slot->hBztp->pHeadVertex;
                slot->pos[0] = point[0];
                slot->pos[1] = point[1];
                slot->pos[2] = point[2];
                SDF_VU0_COPYVECTOR(cap[0], ribbon[1]);
                SDF_VU0_COPYVECTOR(cap[1], ribbon[0]);
                SDF_VU0_COPYVECTOR(cap[7], ribbon[2]);
                oppositeSide[0] = -side[0];
                oppositeSide[1] = -side[1];
                oppositeSide[2] = -side[2];
                oppositeSide[3] = -side[3];
                SDF_VU0_LOADVECTOR_C(ribbon[1]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                SDF_VU0_INTERPOLATE(0.3333333333f);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(cap[2]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                SDF_VU0_INTERPOLATE(0.6666666667f);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(cap[3]);
                SDF_VU0_LOADVECTOR(tangent);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(cap[4]);
                SDF_VU0_LOADVECTOR(oppositeSide);
                SDF_VU0_LOADVECTOR_B(tangent);
                SDF_VU0_INTERPOLATE(0.6666666667f);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(cap[5]);
                SDF_VU0_LOADVECTOR(oppositeSide);
                SDF_VU0_LOADVECTOR_B(tangent);
                SDF_VU0_INTERPOLATE(0.3333333333f);
                SDF_VU0_NORMALIZE_EXTENDED();
                SDF_VU0_LOADVECTOR_B(width);
                SDF_VU0_MULVECTOR_EXTENDED();
                SDF_VU0_ADDVECTOR_A_C_EXTENDED();
                SDF_VU0_STOREVECTOR_NOMEM(cap[6]);
            }
            if (slot->Bezier7P.t >= 1.0f && slot->Bezier7P.pointIndex >= 3) {
                place.payload[0] = slot->pos[0];
                place.payload[1] = slot->pos[1];
                place.payload[2] = slot->pos[2];
                place.payload[3] = 0.0f;
                place.color = color;
                delta[0] = start[0] - place.payload[0];
                delta[2] = start[2] - place.payload[2];
                FromEuler(0.0f, sdfAtan2(delta[0], delta[2]), 0.0f);
                SDF_VU0_STOREVECTOR_NOMEM(&place.payload[4]);
                effEveBEDCutSetParams(slot->hBEDCut, &place);
                effEveBEDCutDraw(slot->hBEDCut);
            }
            dds3BezierTrackPolyDraw(slot->hBztp);
        }
    }
}


void effPCPBezierBillSetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPBezierBillSetColor(effPCPBezierBill_tag *work, u32 color)
{
    work->abgr = color;
}

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex) {
    s32 count = hPoly * pVertex * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(bufferBytes + sizeof(dds3BezierTrackPoly_tag));
    u8 *storage = (u8 *)sdfMemLock(allocation);
    dds3BezierTrackPoly_tag *history = (dds3BezierTrackPoly_tag *)(storage + bufferBytes);
    sdfMat_t *asset;

    history->pVertex = (u128 *)storage;
    storage += count * sizeof(u128);
    history->pHeadVertex = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->pVC = (u32 *)storage;
    storage += colorBytes;
    history->pHeadVC = (u32 *)storage;
    history->blend = 2;
    history->point = count;
    history->current = 3;
    history->split = pVertex;
    history->abgr = 0x80808080;
    history->hMem = allocation;
    history->count = 0;
    memcpy(history->pHeadVertex, D_0031F0B0, sizeof(D_0031F0B0));
    asset = sdfMatNew();
    history->pMat = asset;
    sdfMatSetConstant(asset, 1.0f);
    memset(&dds3BezierVertexCount, 0, 0x2C);
    dds3BezierVertexCount.flags = 0x4000;
    dds3BezierVertexCount.primitiveIndices = D_0031F020;
    memset(&dds3BezierTrackPolyVertex, 0, 0x2C);
    dds3BezierTrackPolyVertex.flags = 0x4000;
    dds3BezierTrackPolyVertex.primitiveIndices = D_0031F090;
    dds3BezierTrackPolyVertex.primitiveCount = 6;
    dds3BezierTrackPolyVertex.pointCount = 8;
    return history;
}

void dds3BezierTrackPolyDelete(dds3BezierTrackPoly_tag *hPoly) {
    sdfMatFreeDelayed(hPoly->pMat);
    sdfMemFreeDirect(hPoly->hMem);
}

void dds3BezierTrackPolyReset(dds3BezierTrackPoly_tag *hPoly) {
    hPoly->current = 3;
    hPoly->abgr = 0x80808080;
    hPoly->count = 0;
}

void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split) {
    f32 t = 0.0f;
    u32 count = face->point / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = face->pVC;
    u32 innerStart = split[0] & 0xFFFFFF;
    u32 innerEnd = split[2] & 0xFFFFFF;
    u32 outerStart = split[1];
    u32 outerEnd = split[3];
    u32 i = 0;

    if (count != 0) {
        do {
            u32 alphaMask = (u32)(alpha * 2147483648.0f) & 0xFF000000;

            colors[0] = effColorBlend(outerEnd, outerStart, t);
            colors[1] = effColorBlend(innerEnd, innerStart, t) | alphaMask;
            colors[2] = colors[0];
            t += step;
            if (i < alphaCount) {
                alpha += alphaStep;
            }
            i++;
            colors += 3;
        } while (i < count);
    }
}

void dds3BezierTrackPolySetVC(dds3BezierTrackPoly_tag *hPoly, u128 *pABGR) {
    s32 position = hPoly->current;
    u128 *points = hPoly->pVertex;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], pABGR);
    SDF_VU0_COPYVECTOR(&points[position + 1], pABGR + 1);
    SDF_VU0_COPYVECTOR(&points[position + 2], pABGR + 2);
    position += 3;
    count = hPoly->point;
    hPoly->current = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], pABGR);
        SDF_VU0_COPYVECTOR(&points[1], pABGR + 1);
        SDF_VU0_COPYVECTOR(&points[2], pABGR + 2);
        hPoly->current = 3;
    }
    if (hPoly->count < count - 3) {
        hPoly->count += 3;
    }
}

void dds3BezierTrackPolyDraw(dds3BezierTrackPoly_tag *hPoly) {
    s32 start[4];
    s32 length[4];
    sdfDrawTag_t *list;
    s32 recent;
    s32 wrapped;
    s32 i;
    s32 remaining;
    effBezierDrawParam_t *draw;
    u32 *colors;
    u32 edgeColor;
    u32 centerColor;
    sdfGrOt_t *surface;

    if (hPoly->count == 0) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, hPoly->pMat, 0);
    recent = hPoly->count;
    start[0] = hPoly->current - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + hPoly->point;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    dds3BezierVertexCount.colors = hPoly->pVC;
    dds3BezierVertexCount.color = hPoly->abgr;
    for (i = 0; i < 2; i++) {
        draw = &dds3BezierVertexCount;
        remaining = length[i];
        draw->primitiveCount = 16;
        draw->points = hPoly->pVertex + start[i];
        draw->pointCount = 15;
        while (remaining >= 15) {
            remaining -= 12;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&dds3BezierVertexCount));
            dds3BezierVertexCount.points += 12;
            dds3BezierVertexCount.colors += 12;
        }
        if (remaining >= 6) {
            draw->primitiveCount = (remaining / 3) * 4 - 4;
            draw->pointCount = remaining;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(draw));
            draw->colors += remaining;
        }
    }
    colors = hPoly->pHeadVC;
    edgeColor = dds3BezierVertexCount.colors[-3];
    centerColor = dds3BezierVertexCount.colors[-2];
    colors[0] = centerColor;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = edgeColor;
    dds3BezierTrackPolyVertex.colors = colors;
    dds3BezierTrackPolyVertex.points = hPoly->pHeadVertex;
    dds3BezierTrackPolyVertex.color = hPoly->abgr;
    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&dds3BezierTrackPolyVertex));
    surface = dds3BezierTrackPolyParam[hPoly->blend];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

