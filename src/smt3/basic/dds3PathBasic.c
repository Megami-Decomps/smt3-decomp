#include "common.h"

/* The scene object dds3AddSceneBasicData returns; only what this unit sets. */
typedef struct dds3SceneBasic_t
{
    u8 unk00[4];
    s32 key;
    const char *name;
    u8 unk0C[0xC];
    void *data;
} dds3SceneBasic_t;

/* One sample of the world transform channel: quaternion, position, scale. */
typedef struct dds3PathTransform_t
{
    f32 rotation[4];
    f32 position[3];
    f32 scale[3];
} dds3PathTransform_t;

/* One channel: count samples of data, with a frame number for each. */
typedef struct dds3PathKeyframes_t
{
    u32 count;
    union
    {
        f32 *scalar;
        f32 (*position)[3];
        f32 (*rotation)[4];
        dds3PathTransform_t *transform;
    } data;
    u32 *frames;
} dds3PathKeyframes_t;

typedef struct dds3PathCurveEntry_t
{
    u32 kind;
    dds3PathKeyframes_t *keys;
} dds3PathCurveEntry_t;

typedef struct dds3PathCurveTable_t
{
    u32 count;
    dds3PathCurveEntry_t entries[1];
} dds3PathCurveTable_t;

typedef struct dds3PathBasicControl_t
{
    s32 direction;
    u32 flags;
    f32 duration;
    f32 time;
    s32 spline;
    dds3PathKeyframes_t *positionKeys;
    dds3PathKeyframes_t *rotationKeys;
    dds3PathKeyframes_t *scalarKeys;
    dds3PathKeyframes_t *transformKeys;
} dds3PathBasicControl_t;

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
extern dds3SceneBasic_t *dds3AddSceneBasicData(s32 kind);
extern s32 dds3AllocSplineHandle(f32 (*points)[3], u32 count, s32 mode);
extern void dds3PathEvalSpline(u32 *segment, f32 *weight, dds3PathKeyframes_t *keys, f32 time);
extern void Lerp(f32 weight);

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

dds3SceneBasic_t *dds3AddPathBasic(s32 key, void *work, const char *name)
{
    dds3SceneBasic_t *object = dds3AddSceneBasicData(0x10);

    object->key = key;
    object->name = name;
    object->data = work;
    return object;
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3AllocPathBasicWorkData);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3DeletePathControl);

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

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicControlNextStep);

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

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicControlIsFinished);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicControlAlterMode);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicControlGetMode);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *work, f32 ratio)
{
    work->time = ratio * work->duration;
}

f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *work)
{
    return work->time / work->duration;
}

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicPauseOn);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicPauseOff);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicLoopOn);

INCLUDE_ASM(const s32, "basic/dds3PathBasic", dds3PathBasicLoopOff);
