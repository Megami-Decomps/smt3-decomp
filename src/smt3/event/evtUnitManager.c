#include "common.h"
#include "event.h"
#include "kernel.h"


#define VU0_LOAD_VF(vf, src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (src))

#define VU0_STORE_VF(vf, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

#define VU0_STORE_VF_UNCLOBBERED(vf, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))

/* A C float broadcast through vf2.x, then the one operation that uses it. */
#define VU0_SCALE_VF10(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" \
    : : "f" (f))

#define VU0_SET_VF10_Y(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" \
    "vaddx.y vf10, vf0, vf2x\n\t.set reorder" \
    : : "f" (f))

#define VU0_SET_AXIS_CLEAR_W(f, axis) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvaddx." #axis " vf10, vf0, vf2x\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t.set reorder" \
    : : "f" (f))

#define VU0_MOVE_VF(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw " #dst ", " #src "\n\t.set reorder")

#define VU0_SUB(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw " #dst ", " #a ", " #b "\n\t.set reorder")

#define VU0_ADD(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw " #dst ", " #a ", " #b "\n\t.set reorder")

#define VU0_CLEAR_W(vf) __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w " #vf ", " #vf ", vf0x\n\t.set reorder")

#define VU0_LENGTH_VF10(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tvsqrt Q, vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "mtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")

#define VU0_NORMALIZE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder")

#define EE_MMI_RGBA_UNPACK(src, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "lw $2, 0(%0)\n" \
    "pextlb $2, $0, $2\n" \
    "pextlh $2, $0, $2\n" \
    "qmtc2.ni $2, vf10\n" \
    "vitof0.xyzw vf10, vf10\n" \
    "qmtc2.ni %1, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    ".set reorder" \
    : : "r"(src), "r"(unit) : "$2", "memory")


f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *value);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *value, f32 multiplier);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *path);

f32 GetSplineLength(dds3PathBasicControl_t *path);

typedef struct evtTargetInfo_t {
    f32 firstColor[4];
    f32 direction[4];
    u8 pad20[0x20];
    f32 secondColor[4];
    f32 nearDistance, farDistance;
    f32 auxFirst, auxSecond;
    u32 unk60;
    u32 flags;
} evtTargetInfo_t;

enum EvtTargetInfoFlags {
    EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR = 0x8
};

/* The fallback ambient vector follows the four source pointers at +0x10. */
typedef struct {
    f32 *sources[4];
    f32 ambient[4];
} evtLightParams_t;

extern evtLightParams_t dds3LightParam;

void evtLinkLightObjectBasic(evtUnit_t *unit, dds3Basic_t *target, s32 duration);

void evtSetUnitLightAndAmbient(evtUnit_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnit_t *unit, s32 duration);

void evtSetUnitColorRGB(evtUnit_t *unit, s32 duration, u32 color);

u32 mdlGetColor(mdlObj_t *ctx);

void mdlSetColor(mdlObj_t *ctx, u32 value);

void evtSetUnitColorA(evtUnit_t *unit, s32 duration, u32 color);

void evtUnitLightOn(evtUnit_t *unit);

enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

struct dds3Basic_t *dds3GetActiveScene(void);

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

void sdfChipFreeDirect(void *memory);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

s32 evtDeleteUnitHandle(evtUnit_t *work);

void evtSetWaitMotion(evtUnit_t *unit, s32 value);

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} sdfMotionState_t;

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *path);

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *path);

void dds3SetPosPoseSet(dds3Basic_t *object, u128 *vector);

void dds3SetRotPoseSet(dds3Basic_t *object, u128 *vector);

void FromEuler(f32 x, f32 y, f32 z);

void Mult(void);


struct dds3Basic_t *dds3GetControlScene(void);

extern void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);


void ToMatrix(void);

s32 evtUnitRotationTarget(evtUnit_t *unit, f32 *dir, f32 scale);

void dds3GetPosPoseSetMCR(dds3Basic_t *object, u128 *vector);

extern void evtGetUnitDirVector(evtUnit_t *unit);





/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b) {
    VU0_LOAD_VF(vf10, a);
    VU0_LOAD_VF(vf11, b);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SET_VF10_Y(0.0f);
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnit_t *unit) {
    f32 v[4];

    VU0_LOAD_VF(vf10, &unit->effObj->poseset->rot);
    ToMatrix();
    VU0_STORE_VF(vf30, v);
    v[1] = 0.0f;
    v[3] = 1.0f;
    VU0_LOAD_VF(vf10, v);
    VU0_NORMALIZE_VF10();
    VU0_SCALE_VF10(-1.0f);
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnit_t *unit) {
    f32 value;

    switch (unit->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtVu0SubVectorXZAxis(unit->targetVector, (f32 *)&unit->effObj->poseset->pos);
        VU0_LENGTH_VF10(value);
        if (value <= 20.0f) {
            return 1;
        }
        break;
    case 3:
        value = dds3PathBasicControlGetRatio((void *)unit->pathHandle);
        if (unit->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (value == 0.0f) {
                return 1;
            }
        } else if (value == 1.0f) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 evtGetNextMovePos(evtUnit_t *unit) {
    f32 t;

    if (unit->motionState != EVT_UNIT_MOTION_STATE_SOURCE) {
        return 0;
    }
    switch (unit->transitionSourceKind) {
    case EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR:
    case EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE:
        break;
    case EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE:
        t = dds3PathBasicControlGetRatio((void *)unit->pathHandle);
        if (unit->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (t == 0.0f) {
                return 0;
            }
        } else {
            if (t == 1.0f) {
                return 0;
            }
        }
        t += unit->pathSpeed;
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (1.0f < t) {
            t = 1.0f;
        }
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)unit->pathHandle, t);
        dds3PathBasicControlGetPos((dds3PathBasicControl_t *)unit->pathHandle);
        VU0_STORE_VF($vf10, unit->targetVector);
        return 1;
    }
    return 0;
}

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 GetSplineLength(dds3PathBasicControl_t *path) {
    f32 savedScaleFactor;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segmentLength;

    savedScaleFactor = dds3PathBasicControlGetRatio((dds3PathBasicControl_t *)path);
    dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 0.0f);
    dds3PathBasicControlGetPos(path);
    do {
        VU0_MOVE_VF(vf11, vf10);
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, t);
        dds3PathBasicControlGetPos(path);
        VU0_MOVE_VF(vf12, vf10);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(segmentLength);
        length += segmentLength;
        VU0_MOVE_VF(vf10, vf12);
        t += step;
    } while (t <= 1.0f);
    dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, savedScaleFactor);
    return length;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", UnitAnime);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void GetBaseLightRGB(evtUnit_t *unit) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((dds3Basic_t *)unit->currentTransitionValue)->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = unit->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, info);
    } else {
                VU0_LOAD_VF(vf10, dds3LightParam.sources[0]);
    }
}

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void GetBaseAmbientRGB(evtUnit_t *unit) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((dds3Basic_t *)unit->currentTransitionValue)->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR) {
        scale = 0.0078125f;
        color = unit->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, info->secondColor);
    } else {
                VU0_LOAD_VF(vf10, dds3LightParam.ambient);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void GetBaseLightDir(evtUnit_t *unit) {
    s32 ownVector = 0;

    if (unit->currentTransitionValue != 0 && (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((evtTargetInfo_t *)((dds3Basic_t *)unit->currentTransitionValue)->workdata)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (unit->flags & 0x400) {
                VU0_LOAD_VF(vf10, unit->vec10);
    } else if (ownVector) {
                VU0_LOAD_VF(vf10, unit->vec10);
    } else {
                VU0_LOAD_VF(vf10, dds3LightParam.sources[0] + 4);
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", MakeLightPacket);

void evtLinkLightObjectBasic(evtUnit_t *unit, dds3Basic_t *target, s32 duration) {
    unit->flags |= EVT_UNIT_FLAG_TARGET_TRANSITION;
    unit->previousTransitionValue = unit->currentTransitionValue;
    unit->currentTransitionValue = (s32)target;
    if (duration == 0) {
        unit->previousTransitionValue = 0;
        unit->transitionElapsed = 0;
        unit->transitionDuration = 0;
        unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
    } else {
        unit->flags |= EVT_UNIT_FLAG_TARGET_BLEND_IN;
        unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_OUT;
        unit->transitionDuration = duration;
        unit->transitionElapsed = 0;
    }
}

void evtUnlinkLightObjectBasic(evtUnit_t *unit, s32 duration) {
    if (unit->currentTransitionValue != 0) {
        if (duration == 0) {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
            unit->currentTransitionValue = 0;
        } else {
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_IN;
            unit->transitionDuration = duration;
            unit->flags |= EVT_UNIT_FLAG_TARGET_BLEND_OUT;
            unit->transitionElapsed = 0;
        }
    }
}

void func_001E35B0(evtUnit_t *unit, u32 value)
{
    unit->value = value;
    unit->flags = unit->flags | EVT_UNIT_FLAG_VALUE_CHANGED;
}

void evtUnlinkExternalLightPacket(evtUnit_t *unit) {
    unit->flags = unit->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    MakeLightPacket(unit);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient);

/* vu0 routine: normalise the target direction and retain the current source. */
void evtSetUnitLightDir(evtUnit_t *unit, s32 duration) {
    unit->directionDuration = duration;
    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec30);
    GetBaseLightDir(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->vec20);
    unit->flags = (unit->flags | 0x2400) & ~0x4000;
}

void evtSetUnitColorRGB(evtUnit_t *unit, s32 duration, u32 color) {
    unit->rgbDuration = duration;
    unit->rgbElapsed = 0;
    if (duration == 0) {
        mdlSetColor((mdlObj_t *)unit->owner,
            (mdlGetColor((mdlObj_t *)unit->owner) & 0xFF000000) | (color & 0xFFFFFF));
        unit->color60 = (unit->color60 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
    } else {
        u32 currentRgb = mdlGetColor((mdlObj_t *)unit->owner) & 0xFFFFFF;

        unit->color60 = (unit->color60 & 0xFF000000) | currentRgb;
        unit->color64 = (unit->color64 & 0xFF000000) | (color & 0xFFFFFF);
        unit->flags |= EVT_UNIT_FLAG_RGB_TRANSITION;
    }
}

void evtSetUnitColorA(evtUnit_t *unit, s32 duration, u32 color) {
    unit->alphaDuration = duration;
    unit->alphaElapsed = 0;
    if (duration == 0) {
        mdlSetColor((mdlObj_t *)unit->owner,
            (mdlGetColor((mdlObj_t *)unit->owner) & 0xFFFFFF) | (color & 0xFF000000));
        unit->color60 = (unit->color60 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
    } else {
        u32 currentAlpha = mdlGetColor((mdlObj_t *)unit->owner) & 0xFF000000;

        unit->color60 = (unit->color60 & 0xFFFFFF) | currentAlpha;
        unit->color64 = (unit->color64 & 0xFFFFFF) | (color & 0xFF000000);
        unit->flags |= EVT_UNIT_FLAG_ALPHA_TRANSITION;
    }
}

u8 evtUnitLightAnimeStat(evtUnit_t *unit)
{
    return (unit->flags & 0x7800) != 0;
}

void evtUnitLightOn(evtUnit_t *unit)
{
    unit->flags = unit->flags | EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
}

void evtUnitLightOff(evtUnit_t *unit, s32 duration) {
    if (duration == 0) {
        unit->flags &= ~0x700;
        unit->flags &= ~0x5000;
        unit->flags &= ~0x2800;
    } else {
        evtSetUnitLightAndAmbient(unit, duration, 0, 0);
        unit->flags = (unit->flags & ~0x800) | 0x1000;
                VU0_MOVE_VF(vf10, vf0);
        evtSetUnitLightDir(unit, duration);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

evtUnit_t *evtGetUnitHandle(s32 worldObjectKey) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), worldObjectKey, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((dds3UnitObjectWork_t *)obj->workdata)->unit;
    }
    return NULL;
}

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return ((dds3UnitObjectWork_t *)obj->workdata)->unit;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCreateUnitHandle);

s32 evtDeleteUnitHandle(evtUnit_t *transitionWork) {
    void *endpointWork;

    if (transitionWork == NULL) {
        return 1;
    }
    endpointWork = transitionWork->endpointWork;
    transitionWork->owner->inner->light = 0;
    if (endpointWork != NULL) {
        sdfChipFreeDirect(endpointWork);
        transitionWork->endpointWork = NULL;
    }
    if (transitionWork->pathHandle != 0) {
        dds3DeletePathControl((dds3PathBasicControl_t *)transitionWork->pathHandle);
        transitionWork->pathHandle = 0;
    }
    sdfChipFreeDirect(transitionWork);
    return 1;
}

s32 evtGetUnitStat(evtUnit_t *unit) {
    return unit->motionState;
}

/* Callers pass the promoted full-width value (one reads a byte table field);
 * only the low halfword is stored. */
void evtSetWaitMotion(evtUnit_t *unit, s32 storedParameter) {
    unit->waitMotion = storedParameter;
}

void evtSetWaitMotionSpeed(evtUnit_t *unit, f32 transitionMotionScale) {
    unit->transitionMotionScale = transitionMotionScale;
}

void evtSetWaitHokanFrame(evtUnit_t *unit, s32 firstParameter, s32 secondParameter) {
    unit->waitBlendStart = firstParameter;
    unit->waitBlendEnd = secondParameter;
}

s32 evtCheckUnitWait(evtUnit_t *unit) {
    s32 state = evtGetUnitStat(unit);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return 1;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && unit->motionTicks > 0 &&
        unit->owner->first->stat == SDF_MOTION_STATE_TERMINAL) {
        return 1;
    }
    return 0;
}

void evtSetUnitKutiMotion(evtUnit_t *unit, s32 firstSlot, s32 secondSlot) {
    unit->firstSlot = firstSlot;
    unit->secondSlot = secondSlot;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtRunUnitKutiMotion);

void evtSetUnitMotion(evtUnit_t *unit, s32 motionId, s32 startFrame, s32 endFrame, s32 mode) {
    unit->flags &= ~0x1;
    unit->flags &= ~0x20;
    unit->flags &= ~0x40;
    unit->flags &= ~0x400000;
    unit->flags &= ~0x800000;
    unit->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    (&unit->motionSetup)->motionId = motionId;
    (&unit->motionSetup)->startFrame = startFrame;
    (&unit->motionSetup)->endFrame = endFrame;
    unit->flags |= 0x80;
    unit->motionTicks = 0;
    unit->motionCallback = 0;
    switch (mode) {
    case 0:
        unit->flags |= 0x20;
        break;
    case 1:
        unit->flags |= 0x1;
        break;
    case 2:
        break;
    case 3:
        unit->flags |= 0x40;
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitRotationTarget);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtUnitRotationQuat(evtUnit_t *unit, f32 *quat, f32 angle) {
    f32 v[4];

    VU0_LOAD_VF(vf10, quat);
    ToMatrix();
    VU0_MOVE_VF(vf10, vf30);
    VU0_SET_AXIS_CLEAR_W(0.0f, y);
    VU0_NORMALIZE_VF10();
    VU0_SCALE_VF10(-1.0f);
    VU0_LOAD_VF(vf11, &unit->effObj->poseset->pos);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, v);
    return evtUnitRotationTarget(unit, v, angle);
}

s32 evtUnitMoveType0(evtUnit_t *unit) {
    f32 v[4];
    f32 scale;
    dds3Basic_t *obj;

    evtUnitRotationTarget(unit, unit->targetVector, unit->directionOffset * 0.01f);
    if (unit->directionOffset != 0) {
        evtGetUnitDirVector(unit);
        obj = unit->effObj;
    } else {
        VU0_LOAD_VF(vf10, unit->targetVector);
        obj = unit->effObj;
        VU0_LOAD_VF(vf11, &obj->poseset->pos);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
    }
    scale = unit->motionParameter * 0.1f;
    VU0_SCALE_VF10(scale);
    VU0_STORE_VF(vf10, v);
    dds3GetPosPoseSetMCR(obj, (u128 *)v);
    return 1;
}

s32 evtUnitMoveTypePathTrace(evtUnit_t *unit) {
    f32 v[4];

    dds3PathBasicControlIsFinished((dds3PathBasicControl_t *)unit->pathHandle);
    dds3PathBasicControlGetPos((dds3PathBasicControl_t *)unit->pathHandle);
    VU0_STORE_VF($vf10, v);
    dds3SetPosPoseSet(unit->effObj, (u128 *)v);
    if (unit->flags & 0x10) {
        dds3PathBasicControlGetRot((dds3PathBasicControl_t *)unit->pathHandle);
        VU0_MOVE_VF(vf11, vf10);
        FromEuler(0.0f, 3.14159265f, 0.0f);
        Mult();
        VU0_STORE_VF($vf10, v);
        dds3SetRotPoseSet(unit->effObj, (u128 *)v);
    }
    return 1;
}

void func_001E4108(evtUnit_t *work, void *src);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001E4108);

void evtSetUnitRot1(evtUnit_t *work, s128 *vector, s32 frames);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitRot1);

void evtSetUnitRot2(evtUnit_t *work, s32 worldObjectKey, s32 frames) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitRot1(work, (s128 *)&source->rotation, frames);
        work->linkedUnit = worldUnit;
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove1);

void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitMove1(work, motionSubmode, (s128 *)&source->position, unused, frames, directionOffset, callback, unusedLast);
        work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
        work->linkedUnit = worldUnit;
    }
}

void evtSetUnitMove3(evtUnit_t *work, s32 pathObjectKey, s32 frames, s32 directionOffset, s32 pathFollowMode, s32 dirFlag, s32 sideMode) {
    dds3Basic_t *pathSource;
    dds3PathBasicControl_t *path;

    pathSource = dds3SearchSceneBasic(dds3GetControlScene(), pathObjectKey, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (work->pathHandle != 0) {
        dds3DeletePathControl((dds3PathBasicControl_t *)work->pathHandle);
    }
    path = dds3AllocPathBasicWorkData(pathSource);
    work->pathHandle = (s32)path;
    work->pathSpeed = 40.0f / GetSplineLength(path);
    if (dirFlag == 0) {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 0.0f);
        dds3PathBasicControlAlterMode(path, 0);
    } else {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 1.0f);
        dds3PathBasicControlAlterMode(path, 1);
        work->pathSpeed = -work->pathSpeed;
    }
    switch (pathFollowMode) {
    case 0:
        work->motionSubmode = 0;
        work->flags &= ~2;
        break;
    case 1:
        work->motionSubmode = 3;
        work->flags |= 2;
        break;
    }
    switch (dirFlag) {
    case 0:
        work->flags &= ~EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    case 1:
        work->flags |= EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    }
    switch (sideMode) {
    case 0:
        work->flags &= ~8;
        work->flags &= ~0x10;
        break;
    case 1:
        work->flags |= 8;
        work->flags &= ~0x10;
        break;
    case 2:
        work->flags &= ~8;
        work->flags |= 0x10;
        break;
    }
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE;
    work->linkedUnit = pathSource;
    dds3PathBasicControlGetPos(path);
    VU0_STORE_VF($vf10, work->targetVector);
    work->motionParameter = frames;
    work->directionOffset = directionOffset;
    work->motionCallback = 0;
    work->motionTicks = 0;
}

s32 func_001E4510(evtUnit_t *work, s32 (*callback)(evtUnit_t *, s32)) {
    s32 ret = 0;

    if (callback != 0) {
        work->motionCallback = callback;
        work->motionTicks = 0;
        work->motionState = EVT_UNIT_MOTION_STATE_VALUE;
        ret = 1;
    }
    return ret;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0053A850);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0053A860);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitManagerPrimaryFunc);
