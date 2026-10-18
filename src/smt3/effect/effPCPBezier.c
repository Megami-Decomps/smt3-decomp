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

INCLUDE_ASM(const s32, "effect/effPCPBezier", effPCPBezierBillDraw);

void effPCPBezierBillSetPosition(void *dst, void *src) {
    SDF_VU0__COPYVECTOR(dst, src);
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

    SDF_VU0__COPYVECTOR(&points[position], source);
    SDF_VU0__COPYVECTOR(&points[position + 1], source + 1);
    SDF_VU0__COPYVECTOR(&points[position + 2], source + 2);
    position += 3;
    count = history->pointCapacity;
    history->nextPointWriteIndex = position;
    if (position == count) {
        SDF_VU0__COPYVECTOR(&points[0], source);
        SDF_VU0__COPYVECTOR(&points[1], source + 1);
        SDF_VU0__COPYVECTOR(&points[2], source + 2);
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

