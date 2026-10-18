#include "common.h"
#include "ps2asm.h"
#include "draw3d.h"

typedef struct
{
    u8 pad00[0x18];
    void *model;
} EffTrackPolyModelObject_t;

typedef struct
{
    u32 blend;
    u32 color;
    s32 capacity;
    s32 count;
    s32 head;
    s32 division;
    u128 *vertices;
    u32 *colors;
    sdfMat_t *material;
    sdfMemHandle_t *allocation;
} Dds3HermitianTrackPoly_t;

typedef struct
{
    EffTrackPolyModelObject_t *object;
    s32 mark0;
    s32 mark1;
    f32 startFrame;
    f32 endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 blend;
    u32 gradientColors[4];
} effModelTrackPolyParam_t;

typedef struct
{
    effModelTrackPolyParam_t params;
    u32 updateCount;
    Dds3HermitianTrackPoly_t *trackPoly;
} EffModelTrackPoly_t;

typedef struct
{
    EffModelTrackPoly_t **items;
    u32 count;
    sdfMemHandle_t *handle;
} effModelTrackPolyList_t;

/* Sword trails have their history pointer at +0x30 instead of +0x38. */
typedef struct
{
    EffTrackPolyModelObject_t *object;
    s32 mark0;
    s32 mark1;
    u8 unk0C[0x1C];
    u32 updateCount;
    u32 unk2C;
    Dds3HermitianTrackPoly_t *trackPoly;
} effSwordTrack_t;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void sdfChipFreeDirect(void *ptr);
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatFreeDelayed(sdfMat_t *material);
extern void sdfMatSetConstant(sdfMat_t *material, f32 constant);
extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *tag, s32 mode);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *material, s32 mode);
extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *params);
extern u32 effColorBlend(u32 to, u32 from, f32 amount);
extern sdf3DPrimPolyParm_t D_0056AF00;
extern u32 D_0031FFE0[];
extern sdfGrOt_t *D_00320020[];
extern sdfGrOt_t D_002F64B8;
extern EffModelTrackPoly_t *Eff_ModelTrackPolyCreate(effModelTrackPolyParam_t *params);
extern void Eff_ModelTrackPolyReset(EffModelTrackPoly_t *work);
extern void Eff_ModelTrackPolyDelete(EffModelTrackPoly_t *work);
void dds3HermitianTrackPolySetVertex(Dds3HermitianTrackPoly_t *data, const u128 *src);
void dds3HermitianTrackPolyReset(Dds3HermitianTrackPoly_t *data);
void dds3HermitianTrackPolyDraw(Dds3HermitianTrackPoly_t *data);

extern s32 sdfModelGetMarkPos(void *model, s32 mark);
/* Endpoint pairs are sampled into the history ring below. */

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyCreate);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyDelete);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyReset);

void Eff_ModelTrackPolyGetMarkPosition(EffModelTrackPoly_t *work)
{
    QuadF32 pair[2];
    EffTrackPolyModelObject_t *object = work->params.object;

    sdfModelGetMarkPos(object->model, work->params.mark0);
    VU0_STORE_VF10_TO(pair[0]);
    sdfModelGetMarkPos(object->model, work->params.mark1);
    SDF_VU0_STOREVECTOR(&pair[1]);
    dds3HermitianTrackPolySetVertex(work->trackPoly, (const u128 *)pair);
}

void Eff_ModelTrackPolyVertexSet(EffModelTrackPoly_t *work, const u128 *endpoints) {
    dds3HermitianTrackPolySetVertex(work->trackPoly, endpoints);
}

void Eff_ModelTrackPolyColorSet(EffModelTrackPoly_t *work, u32 color) {
    work->trackPoly->color = color;
}

void Eff_ModelTrackPolyDrawOnly(EffModelTrackPoly_t *work) {
    dds3HermitianTrackPolyDraw(work->trackPoly);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyDraw);

effModelTrackPolyList_t *Eff_ModelTrackPolyListCreate(effModelTrackPolyParam_t *params, u32 count) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(count * sizeof(EffModelTrackPoly_t *) + sizeof(effModelTrackPolyList_t));
    effModelTrackPolyList_t *list = (effModelTrackPolyList_t *)sdfMemLock(handle);
    u32 workIndex;

    list->handle = handle;
    list->items = (EffModelTrackPoly_t **)(list + 1);
    list->count = count;
    for (workIndex = 0; workIndex < count; workIndex++) {
        list->items[workIndex] = Eff_ModelTrackPolyCreate(params);
    }
    return list;
}

void Eff_ModelTrackPolyListDelete(effModelTrackPolyList_t *list) {
    u32 count = list->count;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDelete(list->items[workIndex]);
        workIndex++;
    }
    sdfMemFreeDirect(list->handle);
}

void Eff_ModelTrackPolyListVertexSet(effModelTrackPolyList_t *list, s32 index, const u128 *endpoints) {
    Eff_ModelTrackPolyVertexSet(list->items[index], endpoints);
}

void Eff_ModelTrackPolyListReset(effModelTrackPolyList_t *list, s32 index) {
    Eff_ModelTrackPolyReset(list->items[index]);
}

void Eff_ModelTrackPolyListColorSet(effModelTrackPolyList_t *list, s32 index, u32 color) {
    Eff_ModelTrackPolyColorSet(list->items[index], color);
}

void Eff_ModelTrackPolyListDrawOnly(effModelTrackPolyList_t *list) {
    u32 count = list->count;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDrawOnly(list->items[workIndex]);
        workIndex++;
    }
}

/* vu0 routine: Catmull-Rom point between p[1] and p[2], left in vf10. */
void Eff_HermitianSpline(f32 (*p)[4], f32 t)
{
    f32 tan[2][4];
    f32 half[4];
    f32 h[4][4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    half[0] = 0.5f;
    half[1] = 0.5f;
    half[2] = 0.5f;
    SDF_VU0_LOADVECTOR(p[1]);
    SDF_VU0_LOADVECTOR_B(p[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[0]);
    SDF_VU0_LOADVECTOR(p[2]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(p[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(tan[1]);
    h[0][0] = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[0][1] = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[0][2] = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[1][0] = t3 - 2.0f * t2 + t;
    h[1][1] = t3 - 2.0f * t2 + t;
    h[1][2] = t3 - 2.0f * t2 + t;
    h[2][0] = t3 - t2;
    h[2][1] = t3 - t2;
    h[2][2] = t3 - t2;
    h[3][0] = -2.0f * t3 + 3.0f * t2;
    h[3][1] = -2.0f * t3 + 3.0f * t2;
    h[3][2] = -2.0f * t3 + 3.0f * t2;
    SDF_VU0_LOADVECTOR(h[0]);
    SDF_VU0_LOADVECTOR_B(p[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(h[1]);
    SDF_VU0_LOADVECTOR_B(tan[0]);
    SDF_VU0_MULVECTOR();
    VU0_ADD_VF12_VF10();
    SDF_VU0_LOADVECTOR(h[2]);
    SDF_VU0_LOADVECTOR_B(tan[1]);
    SDF_VU0_MULVECTOR();
    VU0_ADD_VF12_VF10();
    SDF_VU0_LOADVECTOR(h[3]);
    SDF_VU0_LOADVECTOR_B(p[2]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
}

Dds3HermitianTrackPoly_t *dds3HermitianTrackPolyCreate(s32 historyLength, s32 steps) {
    s32 count = historyLength * steps * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *handle = sdfMemAllocDirect(bytes + sizeof(Dds3HermitianTrackPoly_t));
    u8 *cursor = (u8 *)sdfMemLock(handle);
    Dds3HermitianTrackPoly_t *data = (Dds3HermitianTrackPoly_t *)(cursor + bytes);

    data->vertices = (u128 *)cursor;
    cursor += count * sizeof(u128);
    data->blend = 2;
    data->color = 0x80808080;
    data->capacity = count;
    data->count = 0;
    data->head = 2;
    data->division = steps;
    data->allocation = handle;
    data->colors = (u32 *)cursor;
    data->material = sdfMatNew();
    sdfMatSetConstant((sdfMat_t *)data->material, 1.0f);
    memset(&D_0056AF00, 0, sizeof(D_0056AF00));
    D_0056AF00.attr = 0x4000;
    D_0056AF00.faceIndex = D_0031FFE0;
    return data;
}

void pMeshDelete(Dds3HermitianTrackPoly_t *data) {
    sdfMatFreeDelayed((sdfMat_t *)data->material);
    sdfMemFreeDirect(data->allocation);
}

void dds3HermitianTrackPolyReset(Dds3HermitianTrackPoly_t *data) {
    data->head = 2;
    data->color = 0x80808080;
    data->count = 0;
}

void dds3HermitianTrackPolySetBlend(Dds3HermitianTrackPoly_t *data, u32 kind) {
    data->blend = kind;
}

void dds3HermitianTrackPolySetVC(Dds3HermitianTrackPoly_t *data, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = data->capacity >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = data->colors;

    if (count != 0) {
        do {
            colors[0] = effColorBlend(gradientColors[2], gradientColors[0], t);
            colors[1] = effColorBlend(gradientColors[3], gradientColors[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}

void dds3HermitianTrackPolyGetVertex(Dds3HermitianTrackPoly_t *poly, void *first, void *second, s32 back)
{
    s32 index = poly->head - 2 * (poly->division * (back - 1) + back);
    u8 *vertex;

    if (index < 2) {
        index += poly->capacity - 2;
    }
    vertex = (u8 *)poly->vertices + index * 16;
    SDF_VU0_COPYVECTOR(first, vertex);
    SDF_VU0_COPYVECTOR(second, vertex + 16);
}

void dds3HermitianTrackPolySetCurrent(Dds3HermitianTrackPoly_t *data, s32 amount) {
    s32 pos = data->head + ((data->division - 1) * (amount - 1) + amount) * -2;

    if (pos < 2) {
        pos += data->capacity - 2;
    }
    data->head = pos;
}

void dds3HermitianTrackPolySetVertexSub(Dds3HermitianTrackPoly_t *data, const u128 *src) {
    s32 position = data->head;
    u128 *points = data->vertices;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], src);
    SDF_VU0_COPYVECTOR(&points[position + 1], src + 1);
    position += 2;
    count = data->capacity;
    data->head = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], src);
        SDF_VU0_COPYVECTOR(&points[1], src + 1);
        data->head = 2;
    }
    if (data->count < count - 2) {
        data->count += 2;
    }
}

/* Resample the newest history span: cubic interpolation for established
 * tracks, linear extension while the history is still short. */
void dds3HermitianTrackPolySetVertex(Dds3HermitianTrackPoly_t *data, const u128 *src) {
    f32 firstHistory[4][4];
    f32 secondHistory[4][4];
    f32 firstBasePoint[4];
    f32 secondBasePoint[4];
    f32 firstPointDelta[4];
    f32 secondPointDelta[4];
    f32 firstSample[4];
    f32 secondSample[4];
    s32 activePointCount = data->count;
    s32 sampleCount = data->division;
    f32 sampleWeightStep;
    f32 sampleWeight;
    s32 sampleIndex;
    u128 *secondSampleOutput;

    if (activePointCount == 0) {
        dds3HermitianTrackPolySetVertexSub(data, src);
        return;
    }

    if (activePointCount >= sampleCount * 6) {
        s32 savedRingPosition = data->head;
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 3);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 2);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[2], (u128 *)secondHistory[2], 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], src);
        SDF_VU0_COPYVECTOR(secondHistory[3], src + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        dds3HermitianTrackPolySetCurrent(data, 2);
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)firstSample;
            secondSampleOutput = (u128 *)secondSample;
            firstSampleOutputBase = firstSampleOutput;
            sampleIndex = sampleCount;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                sampleIndex--;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
            } while (sampleIndex != 0);
        }
        data->count = activePointCount;
        data->head = savedRingPosition;
    }

    if (activePointCount >= sampleCount * 4) {
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 2);
        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 1);
        SDF_VU0_COPYVECTOR(firstHistory[2], src);
        SDF_VU0_COPYVECTOR(secondHistory[2], src + 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], src);
        SDF_VU0_COPYVECTOR(secondHistory[3], src + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        sampleIndex = 0;
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)firstSample;
            secondSampleOutput = (u128 *)secondSample;
            firstSampleOutputBase = firstSampleOutput;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
                sampleIndex++;
            } while (sampleIndex < sampleCount);
        }
    } else {
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(data, (u128 *)firstBasePoint, (u128 *)secondBasePoint, 1);
        SDF_VU0_LOADVECTOR(src);
        SDF_VU0_LOADVECTOR_B(firstBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(firstPointDelta);
        SDF_VU0_LOADVECTOR(src + 1);
        SDF_VU0_LOADVECTOR_B(secondBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(secondPointDelta);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        if (sampleCount > 0) {
            sampleIndex = sampleCount;
            firstSampleOutput = (u128 *)firstSample;
            do {
                sampleWeight += sampleWeightStep;
                sampleIndex--;
                firstSample[0] = firstBasePoint[0] + firstPointDelta[0] * sampleWeight;
                firstSample[1] = firstBasePoint[1] + firstPointDelta[1] * sampleWeight;
                firstSample[2] = firstBasePoint[2] + firstPointDelta[2] * sampleWeight;
                secondSample[0] = secondBasePoint[0] + secondPointDelta[0] * sampleWeight;
                secondSample[1] = secondBasePoint[1] + secondPointDelta[1] * sampleWeight;
                secondSample[2] = secondBasePoint[2] + secondPointDelta[2] * sampleWeight;
                dds3HermitianTrackPolySetVertexSub(data, firstSampleOutput);
            } while (sampleIndex != 0);
        }
    }
}

/* Submit up to two wrapped ring runs in 18-point strips, overlapping by two. */
void dds3HermitianTrackPolyDraw(Dds3HermitianTrackPoly_t *data) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 runStart[4];
    s32 runPointCount[2];
    sdf3DPrimPolyParm_t *draw;
    s32 runIndex;
    s32 pointsRemaining;
    s32 wrappedStartOffset;
    s32 activePointCount;
    sdfDrawTag_t *list2;
    u64 *packet;

    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, data->material, 0);
    activePointCount = data->count;
    runStart[0] = data->head - activePointCount;
    if (runStart[0] < 2) {
        wrappedStartOffset = runStart[0] - 2;
        runStart[1] = 0;
        runStart[0] = wrappedStartOffset + data->capacity;
        runPointCount[0] = -wrappedStartOffset;
        runPointCount[1] = activePointCount - runPointCount[0] + 2;
    } else {
        runPointCount[0] = activePointCount;
        runPointCount[1] = 0;
    }
    D_0056AF00.vertexColor = data->colors;
    for (runIndex = 0; runIndex < 2; runIndex++) {
        draw = &D_0056AF00;
        draw->vertex = (sdfVector4_t *)&data->vertices[runStart[runIndex]];
        draw->color = data->color;
        draw->faceCount = 0x10;
        draw->pointCount = 0x12;
        pointsRemaining = runPointCount[runIndex];
        while (pointsRemaining >= 0x12) {
            pointsRemaining -= 0x10;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&D_0056AF00));
            D_0056AF00.vertex += 0x10;
            D_0056AF00.vertexColor += 0x10;
        }
        if (pointsRemaining >= 4) {
            draw->pointCount = pointsRemaining;
            draw->faceCount = pointsRemaining - 2;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(draw));
            draw->vertexColor += pointsRemaining;
        }
    }
    if (data->blend < 4) {
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))D_00320020[data->blend]->regFunc)(D_00320020[data->blend], list);
    } else {
        list2 = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list2);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 6;
        packet[5] = 0x42;
        sdfDrawTagLink(list2, (u32)packet);
        D_002F64B8.regFunc(&D_002F64B8, list2);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_002F64B8.regFunc(&D_002F64B8, list);
    }
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPoly_DANTE_Check);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackCreate);

void effSwordTrackDelete(effSwordTrack_t *work)
{
    pMeshDelete(work->trackPoly);
    sdfChipFreeDirect(work);
}

void effSwordTrackRecord(effSwordTrack_t *work)
{
    EffTrackPolyModelObject_t *object = work->object;
    QuadF32 points[2];

    sdfModelGetMarkPos(object->model, work->mark0);
    VU0_STORE_VF10_TO(points[0]);
    sdfModelGetMarkPos(object->model, work->mark1);
    SDF_VU0_STOREVECTOR(&points[1]);
    dds3HermitianTrackPolySetVertex(work->trackPoly, (u128 *)points);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygonDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygon2DDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackDraw);

void effSwordTrackReset(effSwordTrack_t *work)
{
    work->updateCount = 0;
    dds3HermitianTrackPolyReset(work->trackPoly);
}
