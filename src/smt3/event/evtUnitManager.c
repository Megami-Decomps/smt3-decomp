#include "common.h"
#include "event.h"
#include "kernel.h"
#include "ps2asm.h"


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
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SETVECTOR_Y_V0(0.0f);
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnit_t *unit) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(&unit->effObj->poseset->rot);
    ToMatrix();
    SDF_VU0_STORE_ROW2(v);
    v[1] = 0.0f;
    v[3] = 1.0f;
    SDF_VU0_LOADVECTOR(v);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnit_t *unit) {
    f32 value;

    switch (unit->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtVu0SubVectorXZAxis(unit->targetVector, (f32 *)&unit->effObj->poseset->pos);
        SDF_VU0_GETLENGTH(value);
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
        SDF_VU0_STOREVECTOR(unit->targetVector);
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
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, t);
        dds3PathBasicControlGetPos(path);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(segmentLength);
        length += segmentLength;
        SDF_VU0_COPYVECTOR_C_TO_A();
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
                SDF_VU0_LOADVECTOR(info);
    } else {
                SDF_VU0_LOADVECTOR(dds3LightParam.sources[0]);
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
                SDF_VU0_LOADVECTOR(info->secondColor);
    } else {
                SDF_VU0_LOADVECTOR(dds3LightParam.ambient);
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
                SDF_VU0_LOADVECTOR(unit->vec10);
    } else if (ownVector) {
                SDF_VU0_LOADVECTOR(unit->vec10);
    } else {
                SDF_VU0_LOADVECTOR(dds3LightParam.sources[0] + 4);
    }
}

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

/* The light source block sdf3DMakeLightParallelPacket reads: colour then
 * direction, each a quadword. */
typedef struct {
    sdfVector4_t color;
    sdfVector4_t direction;
    u8 pad20[0x20];
} evtLightSource_t;

void sdf3DMakeLightParallelPacket(void *packet, evtLightSource_t **lights, sdfVector4_t *ambient);

/* vu0 routine: unpack the unit's colours into a parallel light packet. */
void MakeLightPacket(evtUnit_t *unit) {
    evtLightSource_t source;
    sdfVector4_t ambient;
    evtLightSource_t *lights[3] = { &source, NULL, NULL };
    evtColorWork_t work[2];
    u32 flags;
    f32 *channel;
    s32 i;

    flags = unit->flags;
    if (flags & 0x20000) {
        return;
    }
    unit->value = 0;
    if ((flags & 0x40000) == 0) {
        if ((flags & 0x700) == 0) {
            return;
        }
    }
    work[0].word = unit->color;
    EE_MMI_RGBA_UNPACK(&work[0], 0.0078125f);
    SDF_VU0_STOREVECTOR_MEMOPS(source.color);
    SDF_VU0_LOADVECTOR(unit->vec10);
    SDF_VU0_STOREVECTOR(&source.direction);
    work[1].word = unit->color50;
    EE_MMI_RGBA_UNPACK(&work[1], 0.0078125f);
    SDF_VU0_STOREVECTOR(&ambient);
    ambient.w = 1.0f;
    channel = &ambient.x;
    for (i = 2; i >= 0; i--) {
        if (1.0f < *channel) {
            *channel = 1.0f;
        }
        channel++;
    }
    sdf3DMakeLightParallelPacket(unit->endpointWork, lights, &ambient);
    unit->value = (u32)unit->endpointWork;
}

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

/* vu0 routine: pack the base light and ambient vectors into the unit's colours. */
/* NON_MATCHING: 3 of 37 words differ (first at +0x10). */
#ifdef NON_MATCHING
void evtSetUnitLightAndAmbient(evtUnit_t *unit, s32 duration, u32 firstColor, u32 secondColor) {
    evtColorWork_t work[2];
    u32 scale;
    s32 light;
    s32 ambient;

    scale = 0x43000000;
    unit->color08 = firstColor;
    unit->colorDuration = duration;
    unit->color58 = secondColor;
    GetBaseLightRGB(unit);
    MMI_RGBA_PACK_UNIT(light, scale);
    unit->firstCurrent = light;
    work[0].word = light;
    GetBaseAmbientRGB(unit);
    MMI_RGBA_PACK_UNIT(ambient, scale);
    unit->color54 = ambient;
    unit->flags = (unit->flags | 0x800) & ~0x1000;
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient);
#endif

/* vu0 routine: normalise the target direction and retain the current source. */
void evtSetUnitLightDir(evtUnit_t *unit, s32 duration) {
    unit->directionDuration = duration;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(unit->vec30);
    GetBaseLightDir(unit);
    SDF_VU0_STOREVECTOR_NOMEM(unit->vec20);
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
                SDF_VU0_LOADIDENTITYVECTOR();
        evtSetUnitLightDir(unit, duration);
        unit->flags = (unit->flags & ~0x2000) | 0x4000;
    }
}

evtUnit_t *evtGetUnitHandle(s32 worldObjectKey) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), worldObjectKey, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((dds3UnitObjectBasicWorkData_t *)obj->workdata)->unit;
    }
    return NULL;
}

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return ((dds3UnitObjectBasicWorkData_t *)obj->workdata)->unit;
}

void *sdfChipAllocDirect(u32 size);

extern sdfVector4_t D_0053A850;

evtUnit_t *evtCreateUnitHandle(dds3Basic_t *effObj, mdlObj_t *owner) {
    evtUnit_t *work;
    sdfVector4_t axis;

    axis = D_0053A850;
    work = sdfChipAllocDirect(0x148);
    memset(work, 0, 0x148);
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->waitBlendEnd = 10;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
    work->transitionMotionScale = 1.0f;
    work->effObj = effObj;
    work->owner = owner;
    work->motionSetup.endFrame = 10;
    work->waitBlendStart = 0;
    work->motionSetup.startFrame = 0;
    SDF_VU0_LOADVECTOR_MEMOPS(axis);
    SDF_VU0_STOREVECTOR(work->vec10);
    SDF_VU0_STOREVECTOR(work->vec40);
    work->color0C = 0xB2B2B2;
    work->color = 0xB2B2B2;
    work->color5C = 0x80303030;
    work->color50 = 0x80303030;
    work->endpointWork = sdfChipAllocDirect(0xE0);
    memset(work->endpointWork, 0, 0xE0);
    work->value = 0;
    return work;
}

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

extern f32 __ieee754_acosf(f32 x);

extern void FromRotation(f32 angle);

void dds3AddRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);

extern sdfVector4_t D_0053A860;

/* vu0 routine: turn the unit toward dir, at most a tenth of a radian a frame. */
s32 evtUnitRotationTarget(evtUnit_t *unit, f32 *dir, f32 scale) {
    sdfVector4_t axis;
    sdfVector4_t rotation;
    sdfVector4_t flat;
    f32 cross;
    f32 angle;

    axis = D_0053A860;
    evtGetUnitDirVector(unit);
    SDF_VU0_COPYVECTOR_A_TO_C();
    evtVu0SubVectorXZAxis(dir, (f32 *)&unit->effObj->poseset->pos);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&flat);
    SDF_VU0_LOADVECTOR_B_MEMOPS(axis);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(cross);
    SDF_VU0_LOADVECTOR(&flat);
    SDF_VU0_INNERPRODUCT(angle);
    if (0.9998f <= angle) {
        return 1;
    }
    angle = __ieee754_acosf(angle);
    if (0.0f < cross) {
        angle = -angle;
    }
    angle = angle * scale;
    if ((unit->flags & 0x200000) == 0) {
        if (angle < -0.15707963f) {
            angle = -0.15707963f;
        }
        if (0.15707963f < angle) {
            angle = 0.15707963f;
        }
    }
    SDF_VU0_LOADVECTOR_MEMOPS(axis);
    FromRotation(angle);
    SDF_VU0_STOREVECTOR_NOMEM(&rotation);
    dds3AddRotPoseSet(unit->effObj, &rotation);
    return 0;
}

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtUnitRotationQuat(evtUnit_t *unit, f32 *quat, f32 angle) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(quat);
    ToMatrix();
    SDF_VU0_GET_ROW2();
    VU0_SET_VF10_Y_CLEAR_W(0.0f);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
    SDF_VU0_LOADVECTOR_B(&unit->effObj->poseset->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(v);
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
        SDF_VU0_LOADVECTOR(unit->targetVector);
        obj = unit->effObj;
        SDF_VU0_LOADVECTOR_B(&obj->poseset->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    }
    scale = unit->motionParameter * 0.1f;
    SDF_VU0_SCALEVECTOR_V0(scale);
    SDF_VU0_STOREVECTOR(v);
    dds3GetPosPoseSetMCR(obj, (u128 *)v);
    return 1;
}

s32 evtUnitMoveTypePathTrace(evtUnit_t *unit) {
    f32 v[4];

    dds3PathBasicControlIsFinished((dds3PathBasicControl_t *)unit->pathHandle);
    dds3PathBasicControlGetPos((dds3PathBasicControl_t *)unit->pathHandle);
    SDF_VU0_STOREVECTOR(v);
    dds3SetPosPoseSet(unit->effObj, (u128 *)v);
    if (unit->flags & 0x10) {
        dds3PathBasicControlGetRot((dds3PathBasicControl_t *)unit->pathHandle);
        SDF_VU0_COPYVECTOR_A_TO_B();
        FromEuler(0.0f, 3.14159265f, 0.0f);
        Mult();
        SDF_VU0_STOREVECTOR(v);
        dds3SetRotPoseSet(unit->effObj, (u128 *)v);
    }
    return 1;
}

void func_001E4108(evtUnit_t *work, void *src) {
    SDF_VU0_COPYVECTOR(work->targetVector, src);
}

void evtSetUnitRot1(evtUnit_t *work, s128 *vector, s32 frames) {
    if (frames > 0 && frames <= 100) {
        work->linkedUnit = NULL;
        work->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        SDF_VU0_COPYVECTOR(work->targetVector, vector);
        work->motionParameter = frames;
        work->directionOffset = 0;
        work->motionCallback = 0;
        work->motionTicks = 0;
    }
}

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

void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast) {
    work->motionSubmode = motionSubmode;
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR;
    work->linkedUnit = NULL;
    SDF_VU0_COPYVECTOR(work->targetVector, vector);
    work->motionParameter = frames;
    work->directionOffset = directionOffset;
    work->motionCallback = callback;
    work->motionTicks = 0;
}

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
    SDF_VU0_STOREVECTOR(work->targetVector);
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
