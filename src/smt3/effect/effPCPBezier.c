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
typedef struct effBezierTrackPoly_t {
    u32 color;                              /* 0x00 */
    s32 surfaceIndex;                       /* 0x04 */
    s32 pointCapacity;                      /* 0x08: history point capacity */
    s32 activePointCount;                   /* 0x0C */
    s32 nextPointWriteIndex;                /* 0x10: first point of the next three-point sample */
    s32 subdivisions;                       /* 0x14: retained input value */
    u128 *points;                           /* 0x18 */
    u32 *colors;                            /* 0x1C */
    sdfMat_t *resourceHandle;               /* 0x20 */
    sdfMemHandle_t *allocation;                /* 0x24 */
    u128 *endPoints;                        /* 0x28 */
    u32 *endColors;                         /* 0x2C */
} effBezierTrackPoly_t;

/* Copied 0x50-byte input for a group of thunder-fragment ribbons. */
typedef struct effBezierParam_t {
    f32 origin[4];                          /* 0x00 */
    u32 mode;                               /* 0x10: retains native selector values */
    f32 spreadX;                            /* 0x14 */
    f32 spreadZ;                            /* 0x18 */
    f32 height;                             /* 0x1C */
    u32 groupCount;                         /* 0x20 */
    s32 curveFrames;                        /* 0x24 */
    s32 startDelayRange;                    /* 0x28 */
    u32 unk2C;                              /* 0x2C */
    s32 fadeFrames;                         /* 0x30 */
    f32 width;                              /* 0x34 */
    s32 historyLength;                      /* 0x38 */
    s32 subdivisions;                       /* 0x3C */
    u32 palette[4];                         /* 0x40: inner-start, outer-start, inner-end, outer-end */
} effBezierParam_t;

/* One 0x80-byte moving ribbon slot plus its event work. */
typedef struct effBezierSlot_t {
    f32 position[4];                        /* 0x00: current sampled point */
    s32 age;                                /* 0x10: negative delay, then elapsed frames */
    f32 fadeScale;                          /* 0x14 */
    effBezierTrackPoly_t *fragment;/* 0x18 */
    effBezierCurve_t curve;           /* 0x1C */
    struct effBezierCut_t *eventNode;          /* 0x7C */
} effBezierSlot_t;

/* Allocation header followed by groupCount inline 0x80-byte slots. */
typedef struct effBezierWork_t {
    effBezierParam_t params;         /* 0x00 */
    effBezierSlot_t *slots;           /* 0x50 */
    u32 tintColor;                           /* 0x54 */
    struct effBezierBED_t *soundMixer;           /* 0x58 */
    u8 ownsMixerVoices;                      /* 0x5C */
    u8 pad5D[3];                             /* 0x5D */
    sdfMemHandle_t *allocation;                 /* 0x60 */
} effBezierWork_t;


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


extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *allocation);
extern void sdfMemFreeDirect(sdfMemHandle_t *allocation);
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatSetConstant(sdfMat_t *material, f32 constant);
extern void sdfMatFreeDelayed(sdfMat_t *material);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void *memset(void *dst, s32 value, u32 size);
extern void *dds3EffectPCPDataGetData(void *table, s32 index);
extern effBezierBED_t *effEveBEDCreate(effBezierBED_t *params);
extern void effEveBEDDelete(effBezierBED_t *bed);
extern effBezierCut_t *effEveBEDCutCreate(effBezierBED_t *bed, s32 mode, effBezierPlacement_t *place);
extern void effEveBEDCutDelete(effBezierCut_t *cut);
extern u32 effColorBlend(u32 firstColor, u32 secondColor, f32 blend);
extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *tag, void *allocator);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *material, void *allocator);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *effPkt3DPrimMakePoly(effBezierDrawParam_t *params);

extern effBezierDrawParam_t dds3BezierVertexCount, dds3BezierTrackPolyVertex;
extern const f32 D_0031F0B0[8][4];
extern const u32 D_0031F020[28], D_0031F090[8];
extern sdfGrOt_t *dds3BezierTrackPolyParam[];

effBezierTrackPoly_t *dds3BezierTrackPolySetVertex(s32 historyLength, s32 subdivisions);
void dds3BezierTrackPolyCreate(effBezierTrackPoly_t *history, u32 *gradientColors);
void dds3BezierTrackPolyDelete(effBezierTrackPoly_t *work);

effBezierWork_t *effPCPBezierBillCreate(const effBezierParam_t *src, effBezierBED_t *eventParams)
{
    u32 count = src->groupCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effBezierSlot_t) + sizeof(effBezierWork_t));
    effBezierWork_t *work = (effBezierWork_t *)sdfMemLock(allocation);
    effBezierSlot_t *slot = (effBezierSlot_t *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->params = *src;
    work->allocation = allocation;
    work->ownsMixerVoices = 1;
    work->slots = slot;
    work->tintColor = 0x80808080;
    work->soundMixer = effEveBEDCreate(eventParams);
    historyLength = work->params.historyLength;
    if (historyLength == 0) {
        work->params.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->params.subdivisions;
    if (subdivisions <= 0) {
        work->params.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->params.startDelayRange <= 0) {
        work->params.startDelayRange = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
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
        slot->fragment = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->fragment, palette);
        slot->age = 0;
        slot->fadeScale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->eventNode = effEveBEDCutCreate(work->soundMixer, 2, &place);
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

effBezierWork_t *effPCPBezierBillCopy(effBezierWork_t *src) {
    u32 count = src->params.groupCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effBezierSlot_t) + sizeof(effBezierWork_t));
    effBezierWork_t *work = (effBezierWork_t *)sdfMemLock(allocation);
    effBezierSlot_t *slot = (effBezierSlot_t *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->params = src->params;
    work->ownsMixerVoices = 0;
    work->allocation = allocation;
    work->soundMixer = src->soundMixer;
    work->slots = slot;
    work->tintColor = 0x80808080;
    historyLength = work->params.historyLength;
    if (historyLength == 0) {
        work->params.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->params.subdivisions;
    if (subdivisions <= 0) {
        work->params.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->params.startDelayRange <= 0) {
        work->params.startDelayRange = 1;
    }
    palette[0] = work->params.palette[0];
    palette[1] = work->params.palette[1];
    palette[2] = work->params.palette[2];
    palette[3] = work->params.palette[3];
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
        slot->fragment = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->fragment, palette);
        slot->age = 0;
        slot->fadeScale = 1.0f;
        slot->curve.pointIndex = 0;
        slot->curve.t = 0;
        slot->curve.parameterStep = 0;
        slot->eventNode = effEveBEDCutCreate(work->soundMixer, 2, &place);
    }
    return work;
}

void effPCPBezierBillDelete(effBezierWork_t *group) {
    u32 i = 0;
    u32 count = group->params.groupCount;
    effBezierSlot_t *slot = group->slots;

    if (count != 0) {
        do {
            i++;
            dds3BezierTrackPolyDelete(slot->fragment);
            effEveBEDCutDelete(slot->eventNode);
            slot++;
        } while (i < count);
    }
    if (group->ownsMixerVoices != 0) {
        effEveBEDDelete(group->soundMixer);
    }
    sdfMemFreeDirect(group->allocation);
}

extern effRandState_t dds3EffectBase[];
extern sdfVector4_t aViewEye, aViewTarget;
extern u32 effBTLSkillCheckRun(void);
extern s32 effBTLSkillGetTargetFormation(s32 index);
extern s32 effBTLSkillGetInvokeFormation(void);
extern void effBTLModelGetSphereCenter(s32 formation);
extern f32 dds3GetRandFloatA(effRandState_t *state);
extern u32 dds3GetRandA(effRandState_t *state);
extern void SetRotateMatrixAxis(f32 angle, const f32 *axis);
extern s32 effPCPBezierEval(effBezierCurve_t *curve, f32 *out);
extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);
extern f32 sdfAtan2(f32 x, f32 z);
extern void FromEuler(f32 x, f32 y, f32 z);
extern void effEveBEDCutSetParams(effBezierCut_t *cut, const effBezierPlacement_t *place);
extern void effEveBEDCutDraw(effBezierCut_t *cut);

void dds3BezierTrackPolyReset(effBezierTrackPoly_t *history);
void dds3BezierTrackPolySetVC(effBezierTrackPoly_t *history, u128 *source);
void dds3BezierTrackPolyDraw(effBezierTrackPoly_t *history);

/* vu0 routine: respawns each ribbon's bezier controls and walks its history. */
/* NON_MATCHING: 15 of 1497 words differ (first at +0x1188). */
#ifdef NON_MATCHING
void effPCPBezierBillDraw(effBezierWork_t *work)
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
    effBezierSlot_t *slot;
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

    count = work->params.groupCount;
    fadeStep = 1.0f / (f32)work->params.fadeFrames;
    segments = work->params.subdivisions - 1;
    frames = work->params.curveFrames;
    delayRange = work->params.startDelayRange;
    mode = work->params.mode;
    tint = work->tintColor;
    slot = work->slots;
    SDF_VU0_COPYVECTOR(origin, work->params.origin);
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
    half[0] = work->params.width * 0.5f;
    half[1] = work->params.width * 0.5f;
    half[2] = work->params.width * 0.5f;
    i = 0;
    if (count != 0) {
        do {
        if (slot->curve.parameterStep == 0.0f) {
            bias = 0.5f;
            drop = -work->params.height * dds3GetRandFloatA(dds3EffectBase);
            drift = work->params.spreadZ * ((dds3GetRandFloatA(dds3EffectBase) - bias) * 2.0f);
            spawn[0] = origin[0] + work->params.spreadX * ((dds3GetRandFloatA(dds3EffectBase) - bias) * 2.0f);
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
            slot->position[0] = point[0];
            slot->position[1] = point[1];
            slot->position[2] = point[2];
            slot->age = -(s32)(dds3GetRandA(dds3EffectBase) % delayRange);
            dds3BezierTrackPolyReset(slot->fragment);
        } else if (slot->age++ >= 0) {
            if (slot->curve.t >= 1.0f && slot->curve.pointIndex >= 3) {
                if (fadeStep < slot->fadeScale) {
                    fade = slot->fadeScale - fadeStep;
                    slot->fadeScale = fade;
                    slot->fragment->color = ((u32)(fade * 128.0f) << 24) | 0x808080;
                    slot->fragment->color = effMagaCylinderCalcColor(tint, slot->fragment->color);
                } else {
                    dds3BezierTrackPolyReset(slot->fragment);
                }
            } else {
                j = 0;
                prev[0] = slot->position[0];
                prev[1] = slot->position[1];
                prev[2] = slot->position[2];
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
                        dds3BezierTrackPolySetVC(slot->fragment, (u128 *)tri);
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
                dds3BezierTrackPolySetVC(slot->fragment, (u128 *)tri);
                endPoints = slot->fragment->endPoints;
                slot->position[0] = point[0];
                slot->position[1] = point[1];
                slot->position[2] = point[2];
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
                place.payload[0] = slot->position[0];
                place.payload[1] = slot->position[1];
                place.payload[2] = slot->position[2];
                place.payload[3] = 0.0f;
                place.color = tint;
                facing[0] = center[0] - place.payload[0];
                facing[2] = center[2] - place.payload[2];
                FromEuler(0.0f, sdfAtan2(facing[0], facing[2]), 0.0f);
                SDF_VU0_STOREVECTOR(&place.payload[4]);
                effEveBEDCutSetParams(slot->eventNode, &place);
                effEveBEDCutDraw(slot->eventNode);
            }
            dds3BezierTrackPolyDraw(slot->fragment);
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

void effPCPBezierBillSetColor(effBezierWork_t *work, u32 color)
{
    work->tintColor = color;
}

effBezierTrackPoly_t *dds3BezierTrackPolySetVertex(s32 historyLength, s32 subdivisions) {
    s32 count = historyLength * subdivisions * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(bufferBytes + sizeof(effBezierTrackPoly_t));
    u8 *storage = (u8 *)sdfMemLock(allocation);
    effBezierTrackPoly_t *history = (effBezierTrackPoly_t *)(storage + bufferBytes);
    sdfMat_t *asset;

    history->points = (u128 *)storage;
    storage += count * sizeof(u128);
    history->endPoints = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->colors = (u32 *)storage;
    storage += colorBytes;
    history->endColors = (u32 *)storage;
    history->surfaceIndex = 2;
    history->pointCapacity = count;
    history->nextPointWriteIndex = 3;
    history->subdivisions = subdivisions;
    history->color = 0x80808080;
    history->allocation = allocation;
    history->activePointCount = 0;
    memcpy(history->endPoints, D_0031F0B0, sizeof(D_0031F0B0));
    asset = sdfMatNew();
    history->resourceHandle = asset;
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

void dds3BezierTrackPolyDelete(effBezierTrackPoly_t *work) {
    sdfMatFreeDelayed(work->resourceHandle);
    sdfMemFreeDirect(work->allocation);
}

void dds3BezierTrackPolyReset(effBezierTrackPoly_t *history) {
    history->nextPointWriteIndex = 3;
    history->color = 0x80808080;
    history->activePointCount = 0;
}

void dds3BezierTrackPolyCreate(effBezierTrackPoly_t *history, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = history->pointCapacity / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = history->colors;
    u32 innerStart = gradientColors[0] & 0xFFFFFF;
    u32 innerEnd = gradientColors[2] & 0xFFFFFF;
    u32 outerStart = gradientColors[1];
    u32 outerEnd = gradientColors[3];
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

void dds3BezierTrackPolySetVC(effBezierTrackPoly_t *history, u128 *source) {
    s32 position = history->nextPointWriteIndex;
    u128 *points = history->points;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], source);
    SDF_VU0_COPYVECTOR(&points[position + 1], source + 1);
    SDF_VU0_COPYVECTOR(&points[position + 2], source + 2);
    position += 3;
    count = history->pointCapacity;
    history->nextPointWriteIndex = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], source);
        SDF_VU0_COPYVECTOR(&points[1], source + 1);
        SDF_VU0_COPYVECTOR(&points[2], source + 2);
        history->nextPointWriteIndex = 3;
    }
    if (history->activePointCount < count - 3) {
        history->activePointCount += 3;
    }
}

void dds3BezierTrackPolyDraw(effBezierTrackPoly_t *history) {
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

    if (history->activePointCount == 0) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, history->resourceHandle, 0);
    recent = history->activePointCount;
    start[0] = history->nextPointWriteIndex - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + history->pointCapacity;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    dds3BezierVertexCount.colors = history->colors;
    dds3BezierVertexCount.color = history->color;
    for (i = 0; i < 2; i++) {
        draw = &dds3BezierVertexCount;
        remaining = length[i];
        draw->primitiveCount = 16;
        draw->points = history->points + start[i];
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
    colors = history->endColors;
    edgeColor = dds3BezierVertexCount.colors[-3];
    centerColor = dds3BezierVertexCount.colors[-2];
    colors[0] = centerColor;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = edgeColor;
    dds3BezierTrackPolyVertex.colors = colors;
    dds3BezierTrackPolyVertex.points = history->endPoints;
    dds3BezierTrackPolyVertex.color = history->color;
    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&dds3BezierTrackPolyVertex));
    surface = dds3BezierTrackPolyParam[history->surfaceIndex];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

