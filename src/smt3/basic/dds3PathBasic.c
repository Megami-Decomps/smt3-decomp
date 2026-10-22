#include "common.h"
#include "basic.h"
#include "ps2asm.h"

#define DDS3_PATH_CURVE_POSITION 0
#define DDS3_PATH_CURVE_ROTATION 2
#define DDS3_PATH_CURVE_SCALAR 4
#define DDS3_PATH_CURVE_WORLD_TRANSFORM 5

#define DDS3_PATH_POSITION_CHANNEL 0x1
#define DDS3_PATH_ROTATION_CHANNEL 0x2
#define DDS3_PATH_SCALAR_CHANNEL 0x4
#define DDS3_PATH_WORLD_TRANSFORM_CHANNEL 0x10

extern void *memset(void *a, s32 b, u32 c);
extern void *sdfChipAllocDirect(u32 aSize);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern s32 dds3AllocSplineHandle(f32 (*points)[3], u32 count, s32 mode);
extern void dds3PathEvalSpline(u32 *segment, f32 *weight, dds3PathKeyframes_t *keys, f32 time);
extern void Lerp(f32 aRatio);
extern void dds3FreeSplineHandle(s32 spline);
extern void sdfChipFreeDirect(void *aPtr);


void dds3PathEvalSpline(u32 *segment, f32 *weight, dds3PathKeyframes_t *keys, f32 frame) {
    u32 count = keys->count;
    u32 i;
    u32 *frames = keys->frames;

    for (i = 0; i < count - 1; i++) {
        if ((f32)frames[i] <= frame && frame < (f32)frames[i + 1]) {
            *weight = 1.0f / (f32)(frames[i + 1] - frames[i]);
            *segment = i;
            *weight *= frame - (f32)frames[i];
            return;
        }
    }
    *segment = count - 2;
    *weight = 1.0f;
}

dds3Basic_t *dds3AddPathBasic(s32 serialID, void *pData, const char *pName)
{
    dds3Basic_t *object = dds3AddSceneBasicData(0x10);

    object->serialID = serialID;
    object->name = pName;
    object->workdata = pData;
    return object;
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3AllocPathBasicWorkData);

void dds3DeletePathControl(dds3PathBasicControl_t *work)
{
    dds3FreeSplineHandle(work->spline);
    sdfChipFreeDirect(work);
}

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 (*points)[3];

    if (pControl->flags & DDS3_PATH_POSITION_CHANNEL) {
        keys = pControl->positionKeys;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->time);
        points = keys->data.position;
        SDF_VU0_SETVECTOR_X(points[segment + 1][0]);
        SDF_VU0_SETVECTOR_Y(points[segment + 1][1]);
        SDF_VU0_SETVECTOR_Z(points[segment + 1][2]);
        SDF_VU0_SCALEVECTOR_V0_BARRIER(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(points[segment][0]);
        SDF_VU0_SETVECTOR_Y(points[segment][1]);
        SDF_VU0_SETVECTOR_Z(points[segment][2]);
        SDF_VU0_SCALEVECTOR_V0_BARRIER(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 (*quaternions)[4];
    f32 (*first)[4];
    f32 (*second)[4];

    if (pControl->flags & DDS3_PATH_ROTATION_CHANNEL) {
        keys = pControl->rotationKeys;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->time);
        quaternions = keys->data.rotation;
        first = &quaternions[segment];
        SDF_VU0_LOADVECTOR(first);
        second = &quaternions[segment] + 1;
        SDF_VU0_LOADVECTOR_B(second);
        Lerp(weight);
    } else {
        SDF_VU0_LOADIDENTITYVECTOR();
    }
}

/* vu0 routine: interpolate the light channel's radii and colors. */
void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    fldResourceLightPath_t *entries;

    if (work->flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
        keys = work->transformKeys;
        dds3PathEvalSpline(&segment, &weight, keys, work->time);
        entries = (fldResourceLightPath_t *)keys->data.transform;
        SDF_VU0_SETVECTOR_X(entries[segment + 1].InsideR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].OutsideR);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].Soften);
        SDF_VU0_SETVECTOR_W(entries[segment + 1].Bias);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].InsideR);
        SDF_VU0_SETVECTOR_Y(entries[segment].OutsideR);
        SDF_VU0_SETVECTOR_Z(entries[segment].Soften);
        SDF_VU0_SETVECTOR_W(entries[segment].Bias);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->InsideR);
        SDF_VU0_GETVECTOR_Y(output->OutsideR);
        SDF_VU0_GETVECTOR_Z(output->Soften);
        SDF_VU0_GETVECTOR_W(output->Bias);
        SDF_VU0_SETVECTOR_X(entries[segment + 1].DiffuseR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].DiffuseG);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].DiffuseB);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].DiffuseR);
        SDF_VU0_SETVECTOR_Y(entries[segment].DiffuseG);
        SDF_VU0_SETVECTOR_Z(entries[segment].DiffuseB);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->DiffuseR);
        SDF_VU0_GETVECTOR_Y(output->DiffuseG);
        SDF_VU0_GETVECTOR_Z(output->DiffuseB);
        SDF_VU0_SETVECTOR_X(entries[segment + 1].AmbientR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].AmbientG);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].AmbientB);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].AmbientR);
        SDF_VU0_SETVECTOR_Y(entries[segment].AmbientG);
        SDF_VU0_SETVECTOR_Z(entries[segment].AmbientB);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->AmbientR);
        SDF_VU0_GETVECTOR_Y(output->AmbientG);
        SDF_VU0_GETVECTOR_Z(output->AmbientB);
    } else {
        memset(output, 0, sizeof(*output));
    }
}

f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 *values;

    if (pControl->flags & DDS3_PATH_SCALAR_CHANNEL) {
        keys = pControl->scalarKeys;
        dds3PathEvalSpline(&segment, &weight, keys, pControl->time);
        values = keys->data.scalar;
        return values[segment] * (1.0f - weight) + values[segment + 1] * weight;
    }
    return 0.0f;
}

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work)
{
    s32 result = 1;

    if (work->flags & DDS3_PATHBASIC_FLAG_STOP) {
        return 1;
    }
    if (work->direction == DDS3_PATHBASIC_CONTROL_FORWARD) {
        if (work->duration > work->time) {
            work->time = work->time + 1.0f;
        } else if (work->flags & DDS3_PATHBASIC_FLAG_LOOP) {
            work->time = 0.0f;
        } else {
            result = 0;
        }
    } else if (work->direction == DDS3_PATHBASIC_CONTROL_BACK) {
        if (work->time > 0.0f) {
            work->time = work->time - 1.0f;
        } else if (work->flags & DDS3_PATHBASIC_FLAG_LOOP) {
            work->time = work->duration;
        } else {
            result = 0;
        }
    }
    return result;
}

void dds3PathBasicControlAlterMode(dds3PathBasicControl_t *pControl, s32 mode)
{
    pControl->direction = mode;
}

s32 dds3PathBasicControlGetMode(dds3PathBasicControl_t *work)
{
    return work->direction;
}

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio)
{
    pControl->time = ratio * pControl->duration;
}

f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *pControl)
{
    return pControl->time / pControl->duration;
}

void dds3PathBasicPauseOn(dds3PathBasicControl_t *work)
{
    work->flags |= DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl)
{
    pControl->flags &= ~DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicLoopOn(dds3PathBasicControl_t *pControl)
{
    pControl->flags |= DDS3_PATHBASIC_FLAG_LOOP;
}

void dds3PathBasicLoopOff(dds3PathBasicControl_t *pControl)
{
    pControl->flags &= ~DDS3_PATHBASIC_FLAG_LOOP;
}
