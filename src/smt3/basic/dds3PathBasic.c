#include "common.h"
#include "basic.h"


#define DDS3_PATH_CURVE_POSITION 0
#define DDS3_PATH_CURVE_ROTATION 2
#define DDS3_PATH_CURVE_SCALAR 4
#define DDS3_PATH_CURVE_WORLD_TRANSFORM 5

#define DDS3_PATH_POSITION_CHANNEL 0x1
#define DDS3_PATH_ROTATION_CHANNEL 0x2
#define DDS3_PATH_SCALAR_CHANNEL 0x4
#define DDS3_PATH_WORLD_TRANSFORM_CHANNEL 0x10

extern void *memset(void *dst, s32 value, u32 size);
extern void *sdfChipAllocDirect(u32 size);
extern dds3Basic_t *dds3AddSceneBasicData(s32 kind);
extern s32 dds3AllocSplineHandle(f32 (*points)[3], u32 count, s32 mode);
extern void dds3PathEvalSpline(u32 *segment, f32 *weight, dds3PathKeyframes_t *keys, f32 time);
extern void Lerp(f32 weight);
extern void dds3FreeSplineHandle(s32 spline);
extern void sdfChipFreeDirect(void *ptr);

/* sceVu0 element access: the value goes through $2 into vf2.x, then one field
 * of vf10 at a time. gcc 2.96 emits none of qmtc2/qmfc2/vaddx/prot3w itself. */
#define VU0_SET_VF10_X(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.x vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")

#define VU0_SET_VF10_Y(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.y vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")

#define VU0_SET_VF10_Z(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vaddx.z vf10, vf0, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2")

#define VU0_MUL_VF10_SCALAR(value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "mfc1 $2, %0\n\t" \
    "qmtc2.ni $2, $vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t" \
    ".set reorder" \
    : : "f" (value) : "$2", "memory")

#define VU0_COPY_VF10_TO_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder")

#define VU0_ZERO_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder")

#define VU0_LOAD_VF10(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (src))

#define VU0_LOAD_VF11(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (src))

#define VU0_ADD_VF10_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder")

#define VU0_CLEAR_VF10_W() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w vf10, vf10, vf0x\n\t.set reorder")

#define VU0_SET_VF10_W(value) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.w vf10, vf0, vf2x\n\t.set reorder" : : "f" (value) : "$2")
#define VU0_LERP_SCALE(value) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" : : "f" (value))
#define VU0_GET_VF10_X(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define VU0_GET_VF10_Y(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tprot3w $2, $2\n\t" \
    "mtc1 $2, %0\n\t.set reorder" : "=f" (out) : : "$2")
#define VU0_GET_VF10_Z(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tpexew $2, $2\n\t" \
    "mtc1 $2, %0\n\t.set reorder" : "=f" (out) : : "$2")
#define VU0_GET_VF10_W(out) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.x vf2, vf0, vf10w\n\tqmfc2.ni $2, vf2\n\t" \
    "mtc1 $2, %0\n\t.set reorder" : "=f" (out) : : "$2")

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

dds3Basic_t *dds3AddPathBasic(s32 key, void *work, const char *name)
{
    dds3Basic_t *object = dds3AddSceneBasicData(0x10);

    object->serialID = key;
    object->name = name;
    object->workdata = work;
    return object;
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3AllocPathBasicWorkData);

void dds3DeletePathControl(dds3PathBasicControl_t *work)
{
    dds3FreeSplineHandle(work->spline);
    sdfChipFreeDirect(work);
}

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *work)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 (*points)[3];

    if (work->flags & DDS3_PATH_POSITION_CHANNEL) {
        keys = work->positionKeys;
        dds3PathEvalSpline(&segment, &weight, keys, work->time);
        points = keys->data.position;
        VU0_SET_VF10_X(points[segment + 1][0]);
        VU0_SET_VF10_Y(points[segment + 1][1]);
        VU0_SET_VF10_Z(points[segment + 1][2]);
        VU0_MUL_VF10_SCALAR(weight);
        VU0_COPY_VF10_TO_VF11();
        VU0_SET_VF10_X(points[segment][0]);
        VU0_SET_VF10_Y(points[segment][1]);
        VU0_SET_VF10_Z(points[segment][2]);
        VU0_MUL_VF10_SCALAR(1.0f - weight);
        VU0_ADD_VF10_VF11();
        VU0_CLEAR_VF10_W();
    } else {
        VU0_ZERO_VF10();
    }
}

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *work)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 (*quaternions)[4];
    f32 (*first)[4];
    f32 (*second)[4];

    if (work->flags & DDS3_PATH_ROTATION_CHANNEL) {
        keys = work->rotationKeys;
        dds3PathEvalSpline(&segment, &weight, keys, work->time);
        quaternions = keys->data.rotation;
        first = &quaternions[segment];
        VU0_LOAD_VF10(first);
        second = &quaternions[segment] + 1;
        VU0_LOAD_VF11(second);
        Lerp(weight);
    } else {
        VU0_ZERO_VF10();
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
        VU0_SET_VF10_X(entries[segment + 1].InsideR);
        VU0_SET_VF10_Y(entries[segment + 1].OutsideR);
        VU0_SET_VF10_Z(entries[segment + 1].Soften);
        VU0_SET_VF10_W(entries[segment + 1].Bias);
        VU0_LERP_SCALE(weight);
        VU0_COPY_VF10_TO_VF11();
        VU0_SET_VF10_X(entries[segment].InsideR);
        VU0_SET_VF10_Y(entries[segment].OutsideR);
        VU0_SET_VF10_Z(entries[segment].Soften);
        VU0_SET_VF10_W(entries[segment].Bias);
        VU0_LERP_SCALE(1.0f - weight);
        VU0_ADD_VF10_VF11();
        VU0_GET_VF10_X(output->InsideR);
        VU0_GET_VF10_Y(output->OutsideR);
        VU0_GET_VF10_Z(output->Soften);
        VU0_GET_VF10_W(output->Bias);
        VU0_SET_VF10_X(entries[segment + 1].DiffuseR);
        VU0_SET_VF10_Y(entries[segment + 1].DiffuseG);
        VU0_SET_VF10_Z(entries[segment + 1].DiffuseB);
        VU0_LERP_SCALE(weight);
        VU0_COPY_VF10_TO_VF11();
        VU0_SET_VF10_X(entries[segment].DiffuseR);
        VU0_SET_VF10_Y(entries[segment].DiffuseG);
        VU0_SET_VF10_Z(entries[segment].DiffuseB);
        VU0_LERP_SCALE(1.0f - weight);
        VU0_ADD_VF10_VF11();
        VU0_GET_VF10_X(output->DiffuseR);
        VU0_GET_VF10_Y(output->DiffuseG);
        VU0_GET_VF10_Z(output->DiffuseB);
        VU0_SET_VF10_X(entries[segment + 1].AmbientR);
        VU0_SET_VF10_Y(entries[segment + 1].AmbientG);
        VU0_SET_VF10_Z(entries[segment + 1].AmbientB);
        VU0_LERP_SCALE(weight);
        VU0_COPY_VF10_TO_VF11();
        VU0_SET_VF10_X(entries[segment].AmbientR);
        VU0_SET_VF10_Y(entries[segment].AmbientG);
        VU0_SET_VF10_Z(entries[segment].AmbientB);
        VU0_LERP_SCALE(1.0f - weight);
        VU0_ADD_VF10_VF11();
        VU0_GET_VF10_X(output->AmbientR);
        VU0_GET_VF10_Y(output->AmbientG);
        VU0_GET_VF10_Z(output->AmbientB);
    } else {
        memset(output, 0, sizeof(*output));
    }
}

f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *work)
{
    u32 segment;
    f32 weight;
    dds3PathKeyframes_t *keys;
    f32 *values;

    if (work->flags & DDS3_PATH_SCALAR_CHANNEL) {
        keys = work->scalarKeys;
        dds3PathEvalSpline(&segment, &weight, keys, work->time);
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

void dds3PathBasicControlAlterMode(dds3PathBasicControl_t *work, s32 direction)
{
    work->direction = direction;
}

s32 dds3PathBasicControlGetMode(dds3PathBasicControl_t *work)
{
    return work->direction;
}

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *work, f32 ratio)
{
    work->time = ratio * work->duration;
}

f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *work)
{
    return work->time / work->duration;
}

void dds3PathBasicPauseOn(dds3PathBasicControl_t *work)
{
    work->flags |= DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicPauseOff(dds3PathBasicControl_t *work)
{
    work->flags &= ~DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicLoopOn(dds3PathBasicControl_t *work)
{
    work->flags |= DDS3_PATHBASIC_FLAG_LOOP;
}

void dds3PathBasicLoopOff(dds3PathBasicControl_t *work)
{
    work->flags &= ~DDS3_PATHBASIC_FLAG_LOOP;
}
