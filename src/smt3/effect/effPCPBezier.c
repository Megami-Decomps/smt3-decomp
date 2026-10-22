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
    s32 subdivisions;                       /* 0x14: retained input value */
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
    effBezierCurve_t curve;           /* 0x1C */
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
extern void *dds3EffectPCPDataGetData(void *table, s32 index);
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
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}

void effPCPBezierBillCreatePCF(void *paramTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(paramTable, 0);
    secondBlock = dds3EffectPCPDataGetData(paramTable, 1);
    effPCPBezierBillCreate(firstBlock, secondBlock);
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
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
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

/* vu0 routine: respawns each ribbon's bezier controls and walks its history. */
/* NON_MATCHING: 15 of 1497 words differ (first at +0x1188). */
#ifdef NON_MATCHING
void effPCPBezierBillDraw(effPCPBezierBill_tag *hData)
{
    effBezierPlacement_t place;
    f32 center[4];
    f32 axis[4];
    f32 spin[4];
    f32 spawn[4];
    f32 point[4];
    f32 origin[4];
    f32 prev[4];
    f32 viewDir[4];
    f32 half[4];
    f32 facing[4];
    f32 tri[3][4];
    f32 side[4];
    f32 sideBack[4];
    f32 tangent[4];
    s32 delayRange;
    u32 i;
    u32 count;
    u32 segments;
    u32 mode;
    u32 tint;
    effPCPBezierBillPart_tag *slot;
    u128 *endPoints;
    f32 fadeStep;
    f32 frames;
    f32 drop;
    f32 drift;
    f32 radius;
    f32 scale;
    f32 bend;
    f32 reach;
    f32 bias;
    f32 fade;
    u32 blend;
    u32 j;

    count = hData->param.number;
    fadeStep = 1.0f / (f32)hData->param.fadeFrames;
    segments = hData->param.subdivisions - 1;
    frames = hData->param.curveFrames;
    delayRange = hData->param.startDelayRange;
    mode = hData->param.mode;
    tint = hData->abgr;
    slot = hData->pPart;
    SDF_VU0_COPYVECTOR(origin, hData->param.origin);
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    if (effBTLSkillCheckRun() != 0) {
        s32 formation;

        if (mode == 3) {
            formation = effBTLSkillGetTargetFormation(0);
        } else {
            formation = effBTLSkillGetInvokeFormation();
        }
        effBTLModelGetSphereCenter(formation);
        SDF_VU0_STOREVECTOR(center);
    } else {
        center[0] = 0.0f;
        center[1] = -80.0f;
        center[2] = -400.0f;
    }
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDir);
    half[0] = hData->param.width * 0.5f;
    half[1] = hData->param.width * 0.5f;
    half[2] = hData->param.width * 0.5f;
    i = 0;
    if (count != 0) {
        do {
        if (slot->curve.parameterStep == 0.0f) {
            bias = 0.5f;
            drop = -hData->param.height * dds3GetRandFloatA(dds3EffectBase);
            drift = hData->param.spreadZ * ((dds3GetRandFloatA(dds3EffectBase) - bias) * 2.0f);
            spawn[0] = origin[0] + hData->param.spreadX * ((dds3GetRandFloatA(dds3EffectBase) - bias) * 2.0f);
            spawn[1] = origin[1] - drop;
            spawn[2] = origin[2] + drift;
            SDF_VU0_LOADVECTOR(spawn);
            SDF_VU0_LOADVECTOR_B(center);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(radius);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(axis);
            switch (mode) {
            case 0:
                spin[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
                spin[1] = -dds3GetRandFloatA(dds3EffectBase);
                spin[2] = 0.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                SetRotateMatrixAxis((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.5235987f, axis);
                scale = 50.0f;
                slot->curve.controlPoints[0].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[0].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[0].z = spin[2] * scale + center[2];
                if (-50.0f < slot->curve.controlPoints[0].y) {
                    slot->curve.controlPoints[0].y = -50.0f;
                }
                scale = 200.0f;
                bend = 150.0f;
                slot->curve.controlPoints[1].x = spin[0] * scale - axis[0] * bend + center[0];
                slot->curve.controlPoints[1].y = spin[1] * scale - axis[1] * bend + center[1];
                slot->curve.controlPoints[1].z = spin[2] * scale - axis[2] * bend + center[2];
                if (-30.0f < slot->curve.controlPoints[1].y) {
                    slot->curve.controlPoints[1].y = -30.0f;
                }
                scale = 300.0f;
                slot->curve.controlPoints[2].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[2].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[2].z = spin[2] * scale + center[2];
                if (-20.0f < slot->curve.controlPoints[2].y) {
                    slot->curve.controlPoints[2].y = -20.0f;
                }
                reach = radius * 0.25f;
                scale = 200.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                slot->curve.controlPoints[3].x = scale * spin[0] + reach * axis[0] + center[0];
                slot->curve.controlPoints[3].y = scale * spin[1] + reach * axis[1] + center[1];
                slot->curve.controlPoints[3].z = scale * spin[2] + reach * axis[2] + center[2];
                if (-10.0f < slot->curve.controlPoints[3].y) {
                    slot->curve.controlPoints[3].y = -10.0f;
                }
                reach = radius * 0.5f;
                scale = 150.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                slot->curve.controlPoints[4].x = scale * spin[0] + reach * axis[0] + center[0];
                slot->curve.controlPoints[4].y = scale * spin[1] + reach * axis[1] + center[1];
                slot->curve.controlPoints[4].z = scale * spin[2] + reach * axis[2] + center[2];
                if (-10.0f < slot->curve.controlPoints[4].y) {
                    slot->curve.controlPoints[4].y = -10.0f;
                }
                reach = radius * 0.75f;
                scale = 65.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                slot->curve.controlPoints[5].x = scale * spin[0] + reach * axis[0] + center[0];
                slot->curve.controlPoints[5].y = scale * spin[1] + reach * axis[1] + center[1];
                slot->curve.controlPoints[5].z = scale * spin[2] + reach * axis[2] + center[2];
                if (-10.0f < slot->curve.controlPoints[5].y) {
                    slot->curve.controlPoints[5].y = -10.0f;
                }
                reach = radius;
                slot->curve.controlPoints[6].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[6].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[6].z = reach * axis[2] + center[2];
                if (-10.0f < slot->curve.controlPoints[6].y) {
                    slot->curve.controlPoints[6].y = -10.0f;
                }
                break;
            case 1:
                spin[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.5f;
                spin[1] = -dds3GetRandFloatA(dds3EffectBase);
                spin[2] = 0.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                scale = 25.0f;
                slot->curve.controlPoints[0].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[0].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[0].z = spin[2] * scale + center[2];
                scale = 100.0f;
                bend = 150.0f;
                slot->curve.controlPoints[1].x = spin[0] * scale - axis[0] * bend + center[0];
                slot->curve.controlPoints[1].y = spin[1] * scale - axis[1] * bend + center[1];
                slot->curve.controlPoints[1].z = spin[2] * scale - axis[2] * bend + center[2];
                scale = 400.0f;
                slot->curve.controlPoints[2].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[2].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[2].z = spin[2] * scale + center[2];
                reach = radius * 0.25f;
                scale = 450.0f;
                slot->curve.controlPoints[3].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[3].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[3].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.5f;
                scale = 500.0f;
                slot->curve.controlPoints[4].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[4].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[4].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.75f;
                scale = 300.0f;
                slot->curve.controlPoints[5].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[5].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[5].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius;
                slot->curve.controlPoints[6].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[6].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[6].z = reach * axis[2] + center[2];
                break;
            case 2:
                spin[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
                spin[1] = -dds3GetRandFloatA(dds3EffectBase);
                spin[2] = 0.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                scale = 20.0f;
                slot->curve.controlPoints[0].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[0].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[0].z = spin[2] * scale + center[2];
                reach = radius * 0.15f;
                scale = 50.0f;
                slot->curve.controlPoints[1].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[1].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[1].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.3f;
                scale = 80.0f;
                slot->curve.controlPoints[2].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[2].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[2].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.45f;
                scale = 60.0f;
                slot->curve.controlPoints[3].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[3].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[3].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.6f;
                scale = 40.0f;
                slot->curve.controlPoints[4].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[4].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[4].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.75f;
                scale = 20.0f;
                slot->curve.controlPoints[5].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[5].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[5].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius;
                slot->curve.controlPoints[6].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[6].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[6].z = reach * axis[2] + center[2];
                break;
            case 3:
                spin[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
                spin[1] = -(dds3GetRandFloatA(dds3EffectBase) * 0.3f + 0.7f);
                spin[2] = 0.0f;
                SDF_VU0_LOADVECTOR(spin);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR_NOMEM(spin);
                scale = 25.0f;
                slot->curve.controlPoints[0].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[0].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[0].z = spin[2] * scale + center[2];
                scale = 100.0f;
                bend = 150.0f;
                slot->curve.controlPoints[1].x = spin[0] * scale - axis[0] * bend + center[0];
                slot->curve.controlPoints[1].y = spin[1] * scale - axis[1] * bend + center[1];
                slot->curve.controlPoints[1].z = spin[2] * scale - axis[2] * bend + center[2];
                scale = 400.0f;
                slot->curve.controlPoints[2].x = spin[0] * scale + center[0];
                slot->curve.controlPoints[2].y = spin[1] * scale + center[1];
                slot->curve.controlPoints[2].z = spin[2] * scale + center[2];
                reach = radius * 0.25f;
                scale = 450.0f;
                slot->curve.controlPoints[3].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[3].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[3].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.5f;
                scale = 500.0f;
                slot->curve.controlPoints[4].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[4].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[4].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius * 0.75f;
                scale = 300.0f;
                slot->curve.controlPoints[5].x = spin[0] * scale + reach * axis[0] + center[0];
                slot->curve.controlPoints[5].y = spin[1] * scale + reach * axis[1] + center[1];
                slot->curve.controlPoints[5].z = spin[2] * scale + reach * axis[2] + center[2];
                reach = radius;
                slot->curve.controlPoints[6].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[6].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[6].z = reach * axis[2] + center[2];
                break;
            case 4:
                reach = radius * 0.14f;
                slot->curve.controlPoints[0].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[0].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[0].z = reach * axis[2] + center[2];
                reach = radius * 0.28f;
                slot->curve.controlPoints[1].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[1].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[1].z = reach * axis[2] + center[2];
                reach = radius * 0.42f;
                slot->curve.controlPoints[2].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[2].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[2].z = reach * axis[2] + center[2];
                reach = radius * 0.56f;
                slot->curve.controlPoints[3].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[3].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[3].z = reach * axis[2] + center[2];
                reach = radius * 0.7f;
                slot->curve.controlPoints[4].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[4].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[4].z = reach * axis[2] + center[2];
                reach = radius * 0.84f;
                slot->curve.controlPoints[5].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[5].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[5].z = reach * axis[2] + center[2];
                reach = radius;
                slot->curve.controlPoints[6].x = reach * axis[0] + center[0];
                slot->curve.controlPoints[6].y = reach * axis[1] + center[1];
                slot->curve.controlPoints[6].z = reach * axis[2] + center[2];
                break;
            }
            slot->curve.pointIndex = 0;
            slot->curve.t = 0.0f;
            slot->curve.parameterStep = 3.0f / (frames * (f32)(segments + 1));
            effPCPBezierEval(&slot->curve, point);
            slot->pos[0] = point[0];
            slot->pos[1] = point[1];
            slot->pos[2] = point[2];
            slot->frame = -(s32)(dds3GetRandA(dds3EffectBase) % delayRange);
            dds3BezierTrackPolyReset(slot->hBztp);
        } else if (slot->frame++ >= 0) {
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                if (fadeStep < slot->alpha) {
                    fade = slot->alpha - fadeStep;
                    slot->alpha = fade;
                    slot->hBztp->abgr = ((u32)(fade * 128.0f) << 24) | 0x808080;
                    slot->hBztp->abgr = effMagaCylinderCalcColor(tint, slot->hBztp->abgr);
                } else {
                    dds3BezierTrackPolyReset(slot->hBztp);
                }
            } else {
                j = 0;
                prev[0] = slot->pos[0];
                prev[1] = slot->pos[1];
                prev[2] = slot->pos[2];
                if (segments != 0) {
                    do {
                        effPCPBezierEval(&slot->curve, point);
                        SDF_VU0_LOADVECTOR(point);
                        SDF_VU0_COPYVECTOR_A_TO_C();
                        SDF_VU0_LOADVECTOR_B(prev);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_LOADVECTOR_B(viewDir);
                        SDF_VU0_OUTERPRODUCT();
                        SDF_VU0_NORMALIZE();
                        SDF_VU0_LOADVECTOR_B(half);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_SWAPVECTOR_A_C();
                        SDF_VU0_STOREVECTOR_NOMEM(tri[1]);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_ADDVECTOR_A_C();
                        SDF_VU0_STOREVECTOR_NOMEM(tri[0]);
                        SDF_VU0_COPYVECTOR_C_TO_A();
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_STOREVECTOR_B_NOMEM(tri[2]);
                        SDF_VU0_COPYVECTOR(prev, point);
                        dds3BezierTrackPolySetVC(slot->hBztp, (u128 *)tri);
                        j++;
                    } while (j < segments);
                }
                effPCPBezierEval(&slot->curve, point);
                SDF_VU0_LOADVECTOR(point);
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR_B(prev);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(tangent);
                SDF_VU0_LOADVECTOR_B(viewDir);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(side);
                SDF_VU0_LOADVECTOR_B(half);
                SDF_VU0_MULVECTOR();
                SDF_VU0_SWAPVECTOR_A_C();
                SDF_VU0_STOREVECTOR(tri[1]);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(tri[0]);
                SDF_VU0_COPYVECTOR_C_TO_A();
                SDF_VU0_SUBVECTOR_B_A();
                SDF_VU0_STOREVECTOR_B_NOMEM(tri[2]);
                dds3BezierTrackPolySetVC(slot->hBztp, (u128 *)tri);
                endPoints = slot->hBztp->pHeadVertex;
                slot->pos[0] = point[0];
                slot->pos[1] = point[1];
                slot->pos[2] = point[2];
                SDF_VU0_COPYVECTOR(&endPoints[0], tri[1]);
                SDF_VU0_COPYVECTOR(&endPoints[1], tri[0]);
                SDF_VU0_COPYVECTOR(&endPoints[7], tri[2]);
                sideBack[0] = -side[0];
                sideBack[1] = -side[1];
                sideBack[2] = -side[2];
                sideBack[3] = -side[3];
                SDF_VU0_LOADVECTOR_C(tri[1]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                PS2_FLOAT_BITS(blend, 0.333333333f);
                SDF_VU0_INTERPOLATE(blend);
                SDF_VU0_NORMALIZE();
                SDF_VU0_LOADVECTOR_B(half);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(&endPoints[2]);
                SDF_VU0_LOADVECTOR(side);
                SDF_VU0_LOADVECTOR_B(tangent);
                PS2_FLOAT_BITS(blend, 0.666666666f);
                SDF_VU0_INTERPOLATE(blend);
                SDF_VU0_NORMALIZE();
                SDF_VU0_LOADVECTOR_B(half);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(&endPoints[3]);
                SDF_VU0_LOADVECTOR(tangent);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(&endPoints[4]);
                SDF_VU0_LOADVECTOR(sideBack);
                SDF_VU0_LOADVECTOR_B(tangent);
                PS2_FLOAT_BITS(blend, 0.666666666f);
                SDF_VU0_INTERPOLATE(blend);
                SDF_VU0_NORMALIZE();
                SDF_VU0_LOADVECTOR_B(half);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(&endPoints[5]);
                SDF_VU0_LOADVECTOR(sideBack);
                SDF_VU0_LOADVECTOR_B(tangent);
                PS2_FLOAT_BITS(blend, 0.333333333f);
                SDF_VU0_INTERPOLATE(blend);
                SDF_VU0_NORMALIZE();
                SDF_VU0_LOADVECTOR_B(half);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(&endPoints[6]);
            }
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                place.payload[0] = slot->pos[0];
                place.payload[1] = slot->pos[1];
                place.payload[2] = slot->pos[2];
                place.payload[3] = 0.0f;
                place.color = tint;
                facing[0] = center[0] - place.payload[0];
                facing[2] = center[2] - place.payload[2];
                FromEuler(0.0f, sdfAtan2(facing[0], facing[2]), 0.0f);
                SDF_VU0_STOREVECTOR(&place.payload[4]);
                effEveBEDCutSetParams(slot->hBEDCut, &place);
                effEveBEDCutDraw(slot->hBEDCut);
            }
            dds3BezierTrackPolyDraw(slot->hBztp);
        }
            i++;
            slot++;
        } while (i < count);
    }
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBezier", effPCPBezierBillDraw);
#endif


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
    history->subdivisions = pVertex;
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

