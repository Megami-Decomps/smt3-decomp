#include "common.h"
#include "model.h"
#include "motion.h"
#include "ps2asm.h"
#include "draw3d.h"

typedef struct
{
    u32 blend;
    u32 abgr;
    s32 point;
    s32 count;
    s32 current;
    s32 split;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3HermitianTrackPoly_tag;

typedef struct
{
    mdlObj_t *hModel;
    s32 mark0;
    s32 mark1;
    f32 frame_s;
    f32 frame_e;
    u32 record;
    s32 face;
    u32 split;
    u32 blend;
    u32 gradientColors[4];
} Eff_ModelTrackPolyParam_t;

typedef struct
{
    Eff_ModelTrackPolyParam_t params;
    u32 updateCount;
    dds3HermitianTrackPoly_tag *trackPoly;
} effModelTrackPoly_t;

typedef struct
{
    effModelTrackPoly_t **hPolyList;
    u32 number;
    sdfMemHandle_t *hMem;
} effMTPolyListHandle_t;

typedef struct
{
    mdlObj_t *hModel;
    s32 mark0;
    s32 mark1;
    u32 startFrame;
    s32 point;
    s32 historyLength;
    f32 step;
    u32 blend;
    u32 abgr;
    u32 sub_alpha_in;
} effSwordTrackParams_t;

/* Sword trails have their history pointer at +0x30 instead of +0x38. */
typedef struct
{
    effSwordTrackParams_t param_dante;
    u32 frame;
    u32 otIndex;
    dds3HermitianTrackPoly_tag *hPoly;
} Eff_ModelTrackPoly_tag;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern void sdfChipFreeDirect(void *aPtr);
extern void *sdfChipAllocDirect(u32 aSize);
extern sdfGrOt_t sdfGrOtTable[];
extern u32 dds3GetFrameID(void);
extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);
extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);
extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, u32 aPacket);

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct
{
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} effDrawBank_t;

extern effDrawBank_t dds3DrawEnv[];
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatFreeDelayed(sdfMat_t *aMat);
extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 aAllocator);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);
extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
extern sdf3DPrimPolyParm_t D_0056AF00;
extern u32 D_0031FFE0[];
extern sdfGrOt_t *D_00320020[];
extern sdfGrOt_t D_002F64B8;
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex);
void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *hPoly);
void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *hPoly);
void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly);
void effSwordTrackDraw(Eff_ModelTrackPoly_tag *hSwordTrack);
void effSwordTrackRecord(Eff_ModelTrackPoly_tag *hSwordTrack);
void effSwordTrackPolygonDraw(Eff_ModelTrackPoly_tag *hSwordTrack);
void effSwordTrackPolygon2DDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

extern s32 sdfModelGetMarkPos(void *aRoot, s32 aId);
s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *hModel);
Eff_ModelTrackPoly_tag *effSwordTrackCreate(effSwordTrackParams_t *pParams);
void effSwordTrackDelete(Eff_ModelTrackPoly_tag *hSwordTrack);
void effSwordTrackReset(Eff_ModelTrackPoly_tag *hSwordTrack);
/* Endpoint pairs are sampled into the history ring below. */

/* Called above its definition, where ee-gcc assumed int. */
dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split);

effModelTrackPoly_t *Eff_ModelTrackPolyCreate(Eff_ModelTrackPolyParam_t *pParam)
{
    effModelTrackPoly_t *work = sdfChipAllocDirect(sizeof(effModelTrackPoly_t));
    effSwordTrackParams_t sword;

    work->params = *pParam;
    if (Eff_ModelTrackPoly_DANTE_Check(work->params.hModel)) {
        sword.hModel = pParam->hModel;
        sword.mark0 = 0x65;
        sword.mark1 = 0x64;
        sword.startFrame = 8;
        sword.point = 0xC8;
        switch (mdlGetMotionNum(pParam->hModel, 0)) {
        case 10:
            sword.historyLength = 6;
            sword.abgr = 0x80C8C8C8;
            break;
        case 9:
            sword.historyLength = 5;
            sword.abgr = 0xC0C0C0C0;
            break;
        case 0x11:
            sword.historyLength = 5;
            sword.abgr = 0xB8A8A8A8;
            break;
        case 0x12:
            sword.historyLength = 7;
            sword.abgr = 0xC8C8C8C8;
            break;
        default:
            sword.historyLength = 6;
            sword.abgr = 0xB8B8B8B8;
            break;
        }
        sword.step = 0.025f;
        sword.blend = 1;
        work->trackPoly = (dds3HermitianTrackPoly_tag *)effSwordTrackCreate(&sword);
    } else {
        work->params.split = 6;
        work->updateCount = 0;
        work->trackPoly = dds3HermitianTrackPolyCreate(work->params.face, 6);
        dds3HermitianTrackPolySetVC(work->trackPoly, pParam->gradientColors);
        dds3HermitianTrackPolySetBlend(work->trackPoly, pParam->blend);
    }
    return work;
}

void Eff_ModelTrackPolyDelete(effModelTrackPoly_t *work)
{
    if (Eff_ModelTrackPoly_DANTE_Check(work->params.hModel)) {
        effSwordTrackDelete((Eff_ModelTrackPoly_tag *)work->trackPoly);
        work->trackPoly = NULL;
    }
    if (work->trackPoly != NULL) {
        pMeshDelete(work->trackPoly);
    }
    sdfChipFreeDirect(work);
}

void Eff_ModelTrackPolyReset(effModelTrackPoly_t *hMTPoly)
{
    if (Eff_ModelTrackPoly_DANTE_Check(hMTPoly->params.hModel)) {
        effSwordTrackReset((Eff_ModelTrackPoly_tag *)hMTPoly->trackPoly);
        return;
    }
    hMTPoly->updateCount = 0;
    dds3HermitianTrackPolyReset(hMTPoly->trackPoly);
}

void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly)
{
    QuadF32 pair[2];
    mdlObj_t *object = hMTPoly->params.hModel;

    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(pair[0]);
    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark1);
    SDF_VU0_STOREVECTOR(&pair[1]);
    dds3HermitianTrackPolySetVertex(hMTPoly->trackPoly, (const u128 *)pair);
}

void Eff_ModelTrackPolyVertexSet(effModelTrackPoly_t *hMTPoly, const u128 *pVertex) {
    dds3HermitianTrackPolySetVertex(hMTPoly->trackPoly, pVertex);
}

void Eff_ModelTrackPolyColorSet(effModelTrackPoly_t *hMTPoly, u32 abgr) {
    hMTPoly->trackPoly->abgr = abgr;
}

void Eff_ModelTrackPolyDrawOnly(effModelTrackPoly_t *hMTPoly) {
    dds3HermitianTrackPolyDraw(hMTPoly->trackPoly);
}

void Eff_ModelTrackPolyDraw(effModelTrackPoly_t *hMTPoly)
{
    Eff_ModelTrackPoly_tag *sword;
    s32 minor;
    u32 saved;
    f32 frame;
    dds3HermitianTrackPoly_tag *poly;

    if (Eff_ModelTrackPoly_DANTE_Check(hMTPoly->params.hModel)) {
        minor = mdlGetMinorNum(hMTPoly->params.hModel);
        sword = (Eff_ModelTrackPoly_tag *)hMTPoly->trackPoly;
        switch (mdlGetMotionNum(hMTPoly->params.hModel, 0)) {
        case 10:
            if (sword->frame < 0x1A) {
                sword->frame++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 9:
            if (sword->frame < 0x15) {
                sword->frame++;
            } else if (minor >= 0x153 && minor < 0x156) {
                saved = sword->param_dante.abgr;
                if (sword->frame >= 0x31) {
                    sword->param_dante.abgr = 0;
                }
                effSwordTrackDraw(sword);
                sword->param_dante.abgr = saved;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x10:
            if (sword->frame < 0xB) {
                sword->frame++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x11:
            if (minor >= 0x153 && minor < 0x156) {
                if (sword->frame < 0x1C) {
                    sword->frame++;
                } else {
                    effSwordTrackDraw(sword);
                }
            } else if (sword->frame < 0x16) {
                sword->frame++;
            } else {
                effSwordTrackDraw(sword);
            }
            break;
        case 0x12:
            if ((minor >= 0x153 && minor < 0x156) ? (sword->frame < 0x11) : (sword->frame < 0xB)) {
                sword->frame++;
            } else {
                saved = sword->param_dante.abgr;
                if (sword->frame - 0x48 < 0x35) {
                    sword->param_dante.abgr = 0;
                }
                effSwordTrackDraw(sword);
                sword->param_dante.abgr = saved;
            }
            break;
        default:
            if (sword->frame >= 0xB) {
                effSwordTrackDraw(sword);
            } else {
                sword->frame++;
            }
            break;
        }
    } else {
        frame = hMTPoly->params.hModel->first->frame;
        if (hMTPoly->params.frame_s <= frame) {
            if (frame <= hMTPoly->params.frame_e) {
                if (hMTPoly->updateCount % hMTPoly->params.record == 0) {
                    Eff_ModelTrackPolyGetMarkPosition(hMTPoly);
                }
                dds3HermitianTrackPolyDraw(hMTPoly->trackPoly);
            } else {
                poly = hMTPoly->trackPoly;
                if ((poly->abgr & 0xFF000000) != 0) {
                    poly->abgr += 0xE0000000;
                }
                dds3HermitianTrackPolyDraw(poly);
            }
            hMTPoly->updateCount++;
        } else {
            Eff_ModelTrackPolyReset(hMTPoly);
            hMTPoly = NULL;
        }
    }
}

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(number * sizeof(effModelTrackPoly_t *) + sizeof(effMTPolyListHandle_t));
    effMTPolyListHandle_t *list = (effMTPolyListHandle_t *)sdfMemLock(handle);
    u32 workIndex;

    list->hMem = handle;
    list->hPolyList = (effModelTrackPoly_t **)(list + 1);
    list->number = number;
    for (workIndex = 0; workIndex < number; workIndex++) {
        list->hPolyList[workIndex] = Eff_ModelTrackPolyCreate(pParam);
    }
    return list;
}

void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list) {
    u32 count = list->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDelete(list->hPolyList[workIndex]);
        workIndex++;
    }
    sdfMemFreeDirect(list->hMem);
}

void Eff_ModelTrackPolyListVertexSet(effMTPolyListHandle_t *hMTPolyList, s32 index, const u128 *pVertex) {
    Eff_ModelTrackPolyVertexSet(hMTPolyList->hPolyList[index], pVertex);
}

void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index) {
    Eff_ModelTrackPolyReset(hMTPolyList->hPolyList[index]);
}

void Eff_ModelTrackPolyListColorSet(effMTPolyListHandle_t *hMTPolyList, s32 index, u32 abgr) {
    Eff_ModelTrackPolyColorSet(hMTPolyList->hPolyList[index], abgr);
}

void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList) {
    u32 count = hMTPolyList->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDrawOnly(hMTPolyList->hPolyList[workIndex]);
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

dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split) {
    s32 count = face * split * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *handle = sdfMemAllocDirect(bytes + sizeof(dds3HermitianTrackPoly_tag));
    u8 *cursor = (u8 *)sdfMemLock(handle);
    dds3HermitianTrackPoly_tag *data = (dds3HermitianTrackPoly_tag *)(cursor + bytes);

    data->pVertex = (u128 *)cursor;
    cursor += count * sizeof(u128);
    data->blend = 2;
    data->abgr = 0x80808080;
    data->point = count;
    data->count = 0;
    data->current = 2;
    data->split = split;
    data->hMem = handle;
    data->pVC = (u32 *)cursor;
    data->pMat = sdfMatNew();
    sdfMatSetConstant((sdfMat_t *)data->pMat, 1.0f);
    memset(&D_0056AF00, 0, sizeof(D_0056AF00));
    D_0056AF00.attr = 0x4000;
    D_0056AF00.faceIndex = D_0031FFE0;
    return data;
}

void pMeshDelete(dds3HermitianTrackPoly_tag *hPoly) {
    sdfMatFreeDelayed((sdfMat_t *)hPoly->pMat);
    sdfMemFreeDirect(hPoly->hMem);
}

void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *hPoly) {
    hPoly->current = 2;
    hPoly->abgr = 0x80808080;
    hPoly->count = 0;
}

void dds3HermitianTrackPolySetBlend(dds3HermitianTrackPoly_tag *hPoly, u32 blend) {
    hPoly->blend = blend;
}

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *pABGR) {
    f32 t = 0.0f;
    u32 count = hPoly->point >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = hPoly->pVC;

    if (count != 0) {
        do {
            colors[0] = effColorBlend(pABGR[2], pABGR[0], t);
            colors[1] = effColorBlend(pABGR[3], pABGR[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}

void dds3HermitianTrackPolyGetVertex(dds3HermitianTrackPoly_tag *hPoly, void *pVertex0, void *pVertex1, s32 frame)
{
    s32 index = hPoly->current - 2 * (hPoly->split * (frame - 1) + frame);
    u8 *vertex;

    if (index < 2) {
        index += hPoly->point - 2;
    }
    vertex = (u8 *)hPoly->pVertex + index * 16;
    SDF_VU0_COPYVECTOR(pVertex0, vertex);
    SDF_VU0_COPYVECTOR(pVertex1, vertex + 16);
}

void dds3HermitianTrackPolySetCurrent(dds3HermitianTrackPoly_tag *hPoly, s32 frame) {
    s32 pos = hPoly->current + ((hPoly->split - 1) * (frame - 1) + frame) * -2;

    if (pos < 2) {
        pos += hPoly->point - 2;
    }
    hPoly->current = pos;
}

void dds3HermitianTrackPolySetVertexSub(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    s32 position = hPoly->current;
    u128 *points = hPoly->pVertex;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], pVertex);
    SDF_VU0_COPYVECTOR(&points[position + 1], pVertex + 1);
    position += 2;
    count = hPoly->point;
    hPoly->current = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], pVertex);
        SDF_VU0_COPYVECTOR(&points[1], pVertex + 1);
        hPoly->current = 2;
    }
    if (hPoly->count < count - 2) {
        hPoly->count += 2;
    }
}

/* Resample the newest history span: cubic interpolation for established
 * tracks, linear extension while the history is still short. */
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    f32 firstHistory[4][4];
    f32 secondHistory[4][4];
    f32 firstBasePoint[4];
    f32 secondBasePoint[4];
    f32 firstPointDelta[4];
    f32 secondPointDelta[4];
    f32 firstSample[4];
    f32 secondSample[4];
    s32 activePointCount = hPoly->count;
    s32 sampleCount = hPoly->split;
    f32 sampleWeightStep;
    f32 sampleWeight;
    s32 sampleIndex;
    u128 *secondSampleOutput;

    if (activePointCount == 0) {
        dds3HermitianTrackPolySetVertexSub(hPoly, pVertex);
        return;
    }

    if (activePointCount >= sampleCount * 6) {
        s32 savedRingPosition = hPoly->current;
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 3);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstHistory[2], (u128 *)secondHistory[2], 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(secondHistory[3], pVertex + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        dds3HermitianTrackPolySetCurrent(hPoly, 2);
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
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
        hPoly->count = activePointCount;
        hPoly->current = savedRingPosition;
    }

    if (activePointCount >= sampleCount * 4) {
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstHistory[0], (u128 *)secondHistory[0], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstHistory[1], (u128 *)secondHistory[1], 1);
        SDF_VU0_COPYVECTOR(firstHistory[2], pVertex);
        SDF_VU0_COPYVECTOR(secondHistory[2], pVertex + 1);
        SDF_VU0_COPYVECTOR(firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(secondHistory[3], pVertex + 1);
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
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
                sampleIndex++;
            } while (sampleIndex < sampleCount);
        }
    } else {
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)firstBasePoint, (u128 *)secondBasePoint, 1);
        SDF_VU0_LOADVECTOR(pVertex);
        SDF_VU0_LOADVECTOR_B(firstBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(firstPointDelta);
        SDF_VU0_LOADVECTOR(pVertex + 1);
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
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
    }
}

/* Submit up to two wrapped ring runs in 18-point strips, overlapping by two. */
void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *hPoly) {
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
    sdf3DPrimLinkMat(list, hPoly->pMat, 0);
    activePointCount = hPoly->count;
    runStart[0] = hPoly->current - activePointCount;
    if (runStart[0] < 2) {
        wrappedStartOffset = runStart[0] - 2;
        runStart[1] = 0;
        runStart[0] = wrappedStartOffset + hPoly->point;
        runPointCount[0] = -wrappedStartOffset;
        runPointCount[1] = activePointCount - runPointCount[0] + 2;
    } else {
        runPointCount[0] = activePointCount;
        runPointCount[1] = 0;
    }
    D_0056AF00.vertexColor = hPoly->pVC;
    for (runIndex = 0; runIndex < 2; runIndex++) {
        draw = &D_0056AF00;
        draw->vertex = (sdfVector4_t *)&hPoly->pVertex[runStart[runIndex]];
        draw->color = hPoly->abgr;
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
    if (hPoly->blend < 4) {
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))D_00320020[hPoly->blend]->regFunc)(D_00320020[hPoly->blend], list);
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

s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *hModel)
{
    s32 minor;

    if (mdlGetMajorNum(hModel) != 1) {
        return 0;
    }
    minor = mdlGetMinorNum(hModel);
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

Eff_ModelTrackPoly_tag *effSwordTrackCreate(effSwordTrackParams_t *pParams)
{
    Eff_ModelTrackPoly_tag *work = sdfChipAllocDirect(sizeof(Eff_ModelTrackPoly_tag));
    u32 colors[4];

    work->param_dante = *pParams;
    work->frame = 0;
    work->hPoly = dds3HermitianTrackPolyCreate(pParams->historyLength,
                                                   (s32)(1.0f / pParams->step));
    colors[0] = pParams->abgr;
    colors[1] = pParams->abgr;
    colors[2] = pParams->abgr;
    colors[3] = pParams->abgr;
    dds3HermitianTrackPolySetVC(work->hPoly, colors);
    dds3HermitianTrackPolySetBlend(work->hPoly, pParams->blend);
    return work;
}

void effSwordTrackDelete(Eff_ModelTrackPoly_tag *hSwordTrack)
{
    pMeshDelete(hSwordTrack->hPoly);
    sdfChipFreeDirect(hSwordTrack);
}

void effSwordTrackRecord(Eff_ModelTrackPoly_tag *hSwordTrack)
{
    mdlObj_t *object = hSwordTrack->param_dante.hModel;
    QuadF32 points[2];

    sdfModelGetMarkPos(object->inner, hSwordTrack->param_dante.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(points[0]);
    sdfModelGetMarkPos(object->inner, hSwordTrack->param_dante.mark1);
    SDF_VU0_STOREVECTOR(&points[1]);
    dds3HermitianTrackPolySetVertex(hSwordTrack->hPoly, (u128 *)points);
}

void effSwordTrackPolygonDraw(Eff_ModelTrackPoly_tag *hSwordTrack)
{
    dds3HermitianTrackPoly_tag *data = hSwordTrack->hPoly;
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
    sdf3DPrimLinkMat(list, data->pMat, 0);
    activePointCount = data->count;
    runStart[0] = data->current - activePointCount;
    if (runStart[0] < 2) {
        runStart[0] -= 2;
        runPointCount[0] = -runStart[0];
        runStart[0] += data->point;
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
        draw->vertex = (sdfVector4_t *)&data->pVertex[runStart[runIndex]];
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
    ot = &sdfGrOtTable[hSwordTrack->otIndex];
    ot->regFunc(ot, list);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygon2DDraw);

void effSwordTrackDraw(Eff_ModelTrackPoly_tag *hSwordTrack)
{
    sdfDrawTag_t *list;
    void *mask;
    u64 *packet;
    f32 frame;

    frame = hSwordTrack->param_dante.hModel->first->frame;
    if (frame < (f32)hSwordTrack->param_dante.startFrame) {
        return;
    }
    effSwordTrackRecord(hSwordTrack);
    hSwordTrack->otIndex = 0x2E;
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
        sdfGrOt_t *ot = &sdfGrOtTable[hSwordTrack->otIndex];

        ot->regFunc(ot, list);
    }
    effSwordTrackPolygonDraw(hSwordTrack);
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    mask = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(list, (u32)mask);
    {
        sdfGrOt_t *ot = &sdfGrOtTable[hSwordTrack->otIndex];

        ot->regFunc(ot, list);
    }
    hSwordTrack->otIndex = 0x2E;
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
        sdfGrOt_t *ot = &sdfGrOtTable[hSwordTrack->otIndex];

        ot->regFunc(ot, list);
    }
    effSwordTrackPolygon2DDraw(hSwordTrack);
    hSwordTrack->frame++;
}

void effSwordTrackReset(Eff_ModelTrackPoly_tag *hSwordTrack)
{
    hSwordTrack->frame = 0;
    dds3HermitianTrackPolyReset(hSwordTrack->hPoly);
}
