#include "common.h"
#include "model.h"
#include "motion.h"
#include "ps2asm.h"
#include "draw3d.h"

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
    mdlObj_t *object;
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

typedef struct
{
    mdlObj_t *object;
    s32 mark0;
    s32 mark1;
    u32 startFrame;
    s32 unk10;
    s32 historyLength;
    f32 sampleStep;
    u32 blend;
    u32 color;
    u32 unk24;
} effSwordTrackParam_t;

/* Sword trails have their history pointer at +0x30 instead of +0x38. */
typedef struct
{
    effSwordTrackParam_t params;
    u32 updateCount;
    u32 otIndex;
    Dds3HermitianTrackPoly_t *trackPoly;
} effSwordTrack_t;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void sdfChipFreeDirect(void *ptr);
extern void *sdfChipAllocDirect(u32 size);
extern sdfGrOt_t sdfGrOtTable[];
extern u32 dds3GetFrameID(void);
extern void sdfGrSetFrameMaskRGB(const u8 *source, void *packet, s32 gsContext);
extern void sdfGrSetFrameMaskReset(const u8 *source, void *packet, s32 gsContext);
extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress);

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct
{
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} effDrawBank_t;

extern effDrawBank_t dds3DrawEnv[];
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
void dds3HermitianTrackPolySetVertex(Dds3HermitianTrackPoly_t *data, const u128 *src);
void dds3HermitianTrackPolyReset(Dds3HermitianTrackPoly_t *data);
void dds3HermitianTrackPolyDraw(Dds3HermitianTrackPoly_t *data);
void Eff_ModelTrackPolyGetMarkPosition(EffModelTrackPoly_t *work);
void effSwordTrackDraw(effSwordTrack_t *work);
void effSwordTrackRecord(effSwordTrack_t *work);
void effSwordTrackPolygonDraw(effSwordTrack_t *work);
void effSwordTrackPolygon2DDraw(effSwordTrack_t *work);

extern s32 sdfModelGetMarkPos(void *model, s32 mark);
s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *object);
effSwordTrack_t *effSwordTrackCreate(effSwordTrackParam_t *params);
void effSwordTrackDelete(effSwordTrack_t *work);
void effSwordTrackReset(effSwordTrack_t *work);
/* Endpoint pairs are sampled into the history ring below. */

EffModelTrackPoly_t *Eff_ModelTrackPolyCreate(effModelTrackPolyParam_t *params)
{
    EffModelTrackPoly_t *work = sdfChipAllocDirect(sizeof(EffModelTrackPoly_t));
    effSwordTrackParam_t sword;

    work->params = *params;
    if (Eff_ModelTrackPoly_DANTE_Check(work->params.object)) {
        sword.object = params->object;
        sword.mark0 = 0x65;
        sword.mark1 = 0x64;
        sword.startFrame = 8;
        sword.unk10 = 0xC8;
        switch (mdlGetMotionNum(params->object, 0)) {
        case 10:
            sword.historyLength = 6;
            sword.color = 0x80C8C8C8;
            break;
        case 9:
            sword.historyLength = 5;
            sword.color = 0xC0C0C0C0;
            break;
        case 0x11:
            sword.historyLength = 5;
            sword.color = 0xB8A8A8A8;
            break;
        case 0x12:
            sword.historyLength = 7;
            sword.color = 0xC8C8C8C8;
            break;
        default:
            sword.historyLength = 6;
            sword.color = 0xB8B8B8B8;
            break;
        }
        sword.sampleStep = 0.025f;
        sword.blend = 1;
        work->trackPoly = (Dds3HermitianTrackPoly_t *)effSwordTrackCreate(&sword);
    } else {
        work->params.unk1C = 6;
        work->updateCount = 0;
        work->trackPoly = dds3HermitianTrackPolyCreate(work->params.historyLength, 6);
        dds3HermitianTrackPolySetVC(work->trackPoly, params->gradientColors);
        dds3HermitianTrackPolySetBlend(work->trackPoly, params->blend);
    }
    return work;
}

void Eff_ModelTrackPolyDelete(EffModelTrackPoly_t *work)
{
    if (Eff_ModelTrackPoly_DANTE_Check(work->params.object)) {
        effSwordTrackDelete((effSwordTrack_t *)work->trackPoly);
        work->trackPoly = NULL;
    }
    if (work->trackPoly != NULL) {
        pMeshDelete(work->trackPoly);
    }
    sdfChipFreeDirect(work);
}

void Eff_ModelTrackPolyReset(EffModelTrackPoly_t *work)
{
    if (Eff_ModelTrackPoly_DANTE_Check(work->params.object)) {
        effSwordTrackReset((effSwordTrack_t *)work->trackPoly);
        return;
    }
    work->updateCount = 0;
    dds3HermitianTrackPolyReset(work->trackPoly);
}

void Eff_ModelTrackPolyGetMarkPosition(EffModelTrackPoly_t *work)
{
    QuadF32 pair[2];
    mdlObj_t *object = work->params.object;

    sdfModelGetMarkPos(object->inner, work->params.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(pair[0]);
    sdfModelGetMarkPos(object->inner, work->params.mark1);
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

void Eff_ModelTrackPolyDraw(EffModelTrackPoly_t *work)
{
    effSwordTrack_t *sword;
    s32 minor;
    u32 saved;
    f32 frame;
    Dds3HermitianTrackPoly_t *poly;

    if (Eff_ModelTrackPoly_DANTE_Check(work->params.object)) {
        minor = mdlGetMinorNum(work->params.object);
        sword = (effSwordTrack_t *)work->trackPoly;
        switch (mdlGetMotionNum(work->params.object, 0)) {
        case 10:
            if (sword->updateCount < 0x1A) {
                sword->updateCount++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 9:
            if (sword->updateCount < 0x15) {
                sword->updateCount++;
            } else if (minor >= 0x153 && minor < 0x156) {
                saved = sword->params.color;
                if (sword->updateCount >= 0x31) {
                    sword->params.color = 0;
                }
                effSwordTrackDraw(sword);
                sword->params.color = saved;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x10:
            if (sword->updateCount < 0xB) {
                sword->updateCount++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x11:
            if (minor >= 0x153 && minor < 0x156) {
                if (sword->updateCount < 0x1C) {
                    sword->updateCount++;
                } else {
                    effSwordTrackDraw(sword);
                }
            } else if (sword->updateCount < 0x16) {
                sword->updateCount++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x12:
            if ((minor >= 0x153 && minor < 0x156) ? (sword->updateCount < 0x11) : (sword->updateCount < 0xB)) {
                sword->updateCount++;
            } else {
                saved = sword->params.color;
                if (sword->updateCount - 0x48 < 0x35) {
                    sword->params.color = 0;
                }
                effSwordTrackDraw(sword);
                sword->params.color = saved;
            }
            break;
        default:
            if (sword->updateCount >= 0xB) {
                effSwordTrackDraw(sword);
            } else {
                sword->updateCount++;
            }
            break;
        }
    } else {
        frame = work->params.object->first->frame;
        if (work->params.startFrame <= frame) {
            if (frame <= work->params.endFrame) {
                if (work->updateCount % work->params.sampleInterval == 0) {
                    Eff_ModelTrackPolyGetMarkPosition(work);
                }
                dds3HermitianTrackPolyDraw(work->trackPoly);
            } else {
                poly = work->trackPoly;
                if ((poly->color & 0xFF000000) != 0) {
                    poly->color += 0xE0000000;
                }
                dds3HermitianTrackPolyDraw(poly);
            }
            work->updateCount++;
        } else {
            Eff_ModelTrackPolyReset(work);
            work = NULL;
        }
    }
}

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
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(h[2]);
    SDF_VU0_LOADVECTOR_B(tan[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
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

s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *object)
{
    s32 minor;

    if (mdlGetMajorNum(object) != 1) {
        return 0;
    }
    minor = mdlGetMinorNum(object);
    if (minor != 0xC0) {
        if (minor < 0xC0) {
            return 0;
        }
        if (minor >= 0x156) {
            return 0;
        }
        if (minor < 0x153) {
            return 0;
        }
    }
    return 1;
}

effSwordTrack_t *effSwordTrackCreate(effSwordTrackParam_t *params)
{
    effSwordTrack_t *work = sdfChipAllocDirect(sizeof(effSwordTrack_t));
    u32 colors[4];

    work->params = *params;
    work->updateCount = 0;
    work->trackPoly = dds3HermitianTrackPolyCreate(params->historyLength,
                                                   (s32)(1.0f / params->sampleStep));
    colors[0] = params->color;
    colors[1] = params->color;
    colors[2] = params->color;
    colors[3] = params->color;
    dds3HermitianTrackPolySetVC(work->trackPoly, colors);
    dds3HermitianTrackPolySetBlend(work->trackPoly, params->blend);
    return work;
}

void effSwordTrackDelete(effSwordTrack_t *work)
{
    pMeshDelete(work->trackPoly);
    sdfChipFreeDirect(work);
}

void effSwordTrackRecord(effSwordTrack_t *work)
{
    mdlObj_t *object = work->params.object;
    QuadF32 points[2];

    sdfModelGetMarkPos(object->inner, work->params.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(points[0]);
    sdfModelGetMarkPos(object->inner, work->params.mark1);
    SDF_VU0_STOREVECTOR(&points[1]);
    dds3HermitianTrackPolySetVertex(work->trackPoly, (u128 *)points);
}

void effSwordTrackPolygonDraw(effSwordTrack_t *work)
{
    Dds3HermitianTrackPoly_t *data = work->trackPoly;
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 runStart[4];
    s32 runPointCount[2];
    sdf3DPrimPolyParm_t *draw;
    s32 runIndex;
    s32 pointsRemaining;
    s32 activePointCount;
    sdfGrOt_t *ot;

    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, data->material, 0);
    activePointCount = data->count;
    runStart[0] = data->head - activePointCount;
    if (runStart[0] < 2) {
        runStart[0] -= 2;
        runPointCount[0] = -runStart[0];
        runStart[0] += data->capacity;
        runPointCount[1] = activePointCount - runPointCount[0] + 2;
        runStart[1] = 0;
    } else {
        runPointCount[0] = activePointCount;
        runPointCount[1] = 0;
    }
    D_0056AF00.color = 0x20808080;
    D_0056AF00.vertexColor = NULL;
    for (runIndex = 0; runIndex < 2; runIndex++) {
        draw = &D_0056AF00;
        draw->vertex = (sdfVector4_t *)&data->vertices[runStart[runIndex]];
        draw->faceCount = 0x10;
        draw->pointCount = 0x12;
        pointsRemaining = runPointCount[runIndex];
        while (pointsRemaining >= 0x12) {
            pointsRemaining -= 0x10;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&D_0056AF00));
            D_0056AF00.vertex += 0x10;
        }
        if (pointsRemaining >= 4) {
            draw->pointCount = pointsRemaining;
            draw->faceCount = pointsRemaining - 2;
            sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(draw));
        }
    }
    ot = &sdfGrOtTable[work->otIndex];
    ot->regFunc(ot, list);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygon2DDraw);

void effSwordTrackDraw(effSwordTrack_t *work)
{
    sdfDrawTag_t *list;
    void *mask;
    u64 *packet;
    f32 frame;

    frame = work->params.object->first->frame;
    if (frame < (f32)work->params.startFrame) {
        return;
    }
    effSwordTrackRecord(work);
    work->otIndex = 0x2E;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    mask = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(list, (u32)mask);
    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
    packet[2] = ((u64)0x10000000 << 32) | 0x8002;
    packet[3] = 0xE;
    packet[4] = 0x717FB;
    packet[5] = 0x47;
    packet[6] = 0x44;
    packet[7] = 0x42;
    sdfDrawTagLink(list, (u32)packet);
    {
        sdfGrOt_t *ot = &sdfGrOtTable[work->otIndex];

        ot->regFunc(ot, list);
    }
    effSwordTrackPolygonDraw(work);
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    mask = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(list, (u32)mask);
    {
        sdfGrOt_t *ot = &sdfGrOtTable[work->otIndex];

        ot->regFunc(ot, list);
    }
    work->otIndex = 0x2E;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
    packet[2] = ((u64)0x10000000 << 32) | 0x8002;
    packet[3] = 0xE;
    packet[4] = 0x717FB;
    packet[5] = 0x47;
    packet[6] = 0x44;
    packet[7] = 0x42;
    sdfDrawTagLink(list, (u32)packet);
    {
        sdfGrOt_t *ot = &sdfGrOtTable[work->otIndex];

        ot->regFunc(ot, list);
    }
    effSwordTrackPolygon2DDraw(work);
    work->updateCount++;
}

void effSwordTrackReset(effSwordTrack_t *work)
{
    work->updateCount = 0;
    dds3HermitianTrackPolyReset(work->trackPoly);
}
