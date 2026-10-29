#include "common.h"
#include "event.h"
#include "kernel.h"
#include "ps2asm.h"


f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *pControl);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);

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

void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);

void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

u32 mdlGetColor(mdlObj_t *aHandle);

void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);

void evtUnitLightOn(evtUnit_t *h);

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

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

struct dds3Basic_t *dds3GetActiveScene(void);

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

void sdfChipFreeDirect(void *aPtr);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

s32 evtDeleteUnitHandle(evtUnit_t *h);

void evtSetWaitMotion(evtUnit_t *h, s32 motion);

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} sdfMotionState_t;

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *path);

void dds3SetPosPoseSet(dds3Basic_t *pBasic, u128 *pos);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, u128 *rot);

void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

void Mult(void);


struct dds3Basic_t *dds3GetControlScene(void);

extern void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);


void ToMatrix(void);

s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio);

void dds3GetPosPoseSetMCR(dds3Basic_t *object, u128 *vector);

extern void evtGetUnitDirVector(evtUnit_t *h);


/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b) {
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SETVECTOR_Y_V0(0.0f);
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnit_t *h) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(&h->effObj->poseset->rot);
    ToMatrix();
    SDF_VU0_STORE_ROW2(v);
    v[1] = 0.0f;
    v[3] = 1.0f;
    SDF_VU0_LOADVECTOR(v);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnit_t *h) {
    f32 value;

    switch (h->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtVu0SubVectorXZAxis(h->targetVector, (f32 *)&h->effObj->poseset->pos);
        SDF_VU0_GETLENGTH(value);
        if (value <= 20.0f) {
            return 1;
        }
        break;
    case 3:
        value = dds3PathBasicControlGetRatio((void *)h->pathHandle);
        if (h->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
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

s32 evtGetNextMovePos(evtUnit_t *h) {
    f32 t;

    if (h->motionState != EVT_UNIT_MOTION_STATE_SOURCE) {
        return 0;
    }
    switch (h->transitionSourceKind) {
    case EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR:
    case EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE:
        break;
    case EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE:
        t = dds3PathBasicControlGetRatio((void *)h->pathHandle);
        if (h->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (t == 0.0f) {
                return 0;
            }
        } else {
            if (t == 1.0f) {
                return 0;
            }
        }
        t += h->pathSpeed;
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (1.0f < t) {
            t = 1.0f;
        }
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)h->pathHandle, t);
        dds3PathBasicControlGetPos((dds3PathBasicControl_t *)h->pathHandle);
        SDF_VU0_STOREVECTOR(h->targetVector);
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
void GetBaseLightRGB(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((dds3Basic_t *)h->currentTransitionValue)->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = h->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                SDF_VU0_LOADVECTOR(info);
    } else {
                SDF_VU0_LOADVECTOR(dds3LightParam.sources[0]);
    }
}

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void GetBaseAmbientRGB(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = ((dds3Basic_t *)h->currentTransitionValue)->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR) {
        scale = 0.0078125f;
        color = h->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                SDF_VU0_LOADVECTOR(info->secondColor);
    } else {
                SDF_VU0_LOADVECTOR(dds3LightParam.ambient);
    }
}

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void GetBaseLightDir(evtUnit_t *h) {
    s32 ownVector = 0;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((evtTargetInfo_t *)((dds3Basic_t *)h->currentTransitionValue)->workdata)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & 0x400) {
                SDF_VU0_LOADVECTOR(h->vec10);
    } else if (ownVector) {
                SDF_VU0_LOADVECTOR(h->vec10);
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

void sdf3DMakeLightParallelPacket(void *aPacket, evtLightSource_t **aLights, sdfVector4_t *aAmbient);

/* vu0 routine: unpack the unit's colours into a parallel light packet. */
void MakeLightPacket(evtUnit_t *h) {
    evtLightSource_t source;
    sdfVector4_t ambient;
    evtLightSource_t *lights[3] = { &source, NULL, NULL };
    evtColorWork_t work[2];
    u32 flags;
    f32 *channel;
    s32 i;

    flags = h->flags;
    if (flags & 0x20000) {
        return;
    }
    h->value = 0;
    if ((flags & 0x40000) == 0) {
        if ((flags & 0x700) == 0) {
            return;
        }
    }
    work[0].word = h->color;
    EE_MMI_RGBA_UNPACK(&work[0], 0.0078125f);
    SDF_VU0_STOREVECTOR_MEMOPS(source.color);
    SDF_VU0_LOADVECTOR(h->vec10);
    SDF_VU0_STOREVECTOR(&source.direction);
    work[1].word = h->color50;
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
    sdf3DMakeLightParallelPacket(h->endpointWork, lights, &ambient);
    h->value = (u32)h->endpointWork;
}

void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame) {
    h->flags |= EVT_UNIT_FLAG_TARGET_TRANSITION;
    h->previousTransitionValue = h->currentTransitionValue;
    h->currentTransitionValue = (s32)light;
    if (frame == 0) {
        h->previousTransitionValue = 0;
        h->transitionElapsed = 0;
        h->transitionDuration = 0;
        h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
    } else {
        h->flags |= EVT_UNIT_FLAG_TARGET_BLEND_IN;
        h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_OUT;
        h->transitionDuration = frame;
        h->transitionElapsed = 0;
    }
}

void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame) {
    if (h->currentTransitionValue != 0) {
        if (frame == 0) {
            h->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
            h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
            h->currentTransitionValue = 0;
        } else {
            h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_IN;
            h->transitionDuration = frame;
            h->flags |= EVT_UNIT_FLAG_TARGET_BLEND_OUT;
            h->transitionElapsed = 0;
        }
    }
}

void func_001E35B0(evtUnit_t *unit, u32 value)
{
    unit->value = value;
    unit->flags = unit->flags | EVT_UNIT_FLAG_VALUE_CHANGED;
}

void evtUnlinkExternalLightPacket(evtUnit_t *h) {
    h->flags = h->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    MakeLightPacket(h);
}

/* vu0 routine: pack the base light and ambient vectors into the unit's colours. */
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb) {
    evtColorWork_t work[2];
    union {
        f32 value;
        u32 bits;
    } scale;
    u32 light;
    u32 ambient;

    h->colorDuration = frame;
    h->color08 = lrgb;
    h->color58 = argb;
    scale.value = 128.0f;
    GetBaseLightRGB(h);
    MMI_RGBA_PACK_UNIT(light, scale.bits);
    /* Retail retains this colour-slot store in the ambient call delay slot,
     * as does DDS evtInitializeUnitColorTransition. */
    work[0].word = light;
    h->firstCurrent = light;
    GetBaseAmbientRGB(h);
    MMI_RGBA_PACK_UNIT(ambient, scale.bits);
    h->color54 = ambient;
    h->flags = (h->flags | 0x800) & ~0x1000;
}

/* vu0 routine: normalise the target direction and retain the current source. */
void evtSetUnitLightDir(evtUnit_t *h, s32 frame) {
    h->directionDuration = frame;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(h->vec30);
    GetBaseLightDir(h);
    SDF_VU0_STOREVECTOR_NOMEM(h->vec20);
    h->flags = (h->flags | 0x2400) & ~0x4000;
}

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb) {
    h->rgbDuration = frame;
    h->rgbElapsed = 0;
    if (frame == 0) {
        mdlSetColor((mdlObj_t *)h->owner,
            (mdlGetColor((mdlObj_t *)h->owner) & 0xFF000000) | (rgb & 0xFFFFFF));
        h->color60 = (h->color60 & 0xFF000000) | (rgb & 0xFFFFFF);
        h->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
    } else {
        u32 currentRgb = mdlGetColor((mdlObj_t *)h->owner) & 0xFFFFFF;

        h->color60 = (h->color60 & 0xFF000000) | currentRgb;
        h->color64 = (h->color64 & 0xFF000000) | (rgb & 0xFFFFFF);
        h->flags |= EVT_UNIT_FLAG_RGB_TRANSITION;
    }
}

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a) {
    h->alphaDuration = frame;
    h->alphaElapsed = 0;
    if (frame == 0) {
        mdlSetColor((mdlObj_t *)h->owner,
            (mdlGetColor((mdlObj_t *)h->owner) & 0xFFFFFF) | (a & 0xFF000000));
        h->color60 = (h->color60 & 0xFFFFFF) | (a & 0xFF000000);
        h->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
    } else {
        u32 currentAlpha = mdlGetColor((mdlObj_t *)h->owner) & 0xFF000000;

        h->color60 = (h->color60 & 0xFFFFFF) | currentAlpha;
        h->color64 = (h->color64 & 0xFFFFFF) | (a & 0xFF000000);
        h->flags |= EVT_UNIT_FLAG_ALPHA_TRANSITION;
    }
}

u8 evtUnitLightAnimeStat(evtUnit_t *h)
{
    return (h->flags & 0x7800) != 0;
}

void evtUnitLightOn(evtUnit_t *h)
{
    h->flags = h->flags | EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
}

void evtUnitLightOff(evtUnit_t *h, s32 frame) {
    if (frame == 0) {
        h->flags &= ~0x700;
        h->flags &= ~0x5000;
        h->flags &= ~0x2800;
    } else {
        evtSetUnitLightAndAmbient(h, frame, 0, 0);
        h->flags = (h->flags & ~0x800) | 0x1000;
                SDF_VU0_LOADIDENTITYVECTOR();
        evtSetUnitLightDir(h, frame);
        h->flags = (h->flags & ~0x2000) | 0x4000;
    }
}

evtUnit_t *evtGetUnitHandle(s32 serial) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), serial, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((dds3UnitObjectBasicWorkData_t *)obj->workdata)->hUnit;
    }
    return NULL;
}

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit) {
    if (unit == NULL) {
        return NULL;
    }
    return ((dds3UnitObjectBasicWorkData_t *)unit->workdata)->hUnit;
}

void *sdfChipAllocDirect(u32 aSize);

extern sdfVector4_t D_0053A850;

evtUnit_t *evtCreateUnitHandle(dds3Basic_t *owner, mdlObj_t *model) {
    evtUnit_t *work;
    sdfVector4_t axis;

    axis = D_0053A850;
    work = sdfChipAllocDirect(0x148);
    memset(work, 0, 0x148);
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->waitBlendEnd = 10;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
    work->transitionMotionScale = 1.0f;
    work->effObj = owner;
    work->owner = model;
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

s32 evtDeleteUnitHandle(evtUnit_t *h) {
    void *endpointWork;

    if (h == NULL) {
        return 1;
    }
    endpointWork = h->endpointWork;
    h->owner->inner->light = 0;
    if (endpointWork != NULL) {
        sdfChipFreeDirect(endpointWork);
        h->endpointWork = NULL;
    }
    if (h->pathHandle != 0) {
        dds3DeletePathControl((dds3PathBasicControl_t *)h->pathHandle);
        h->pathHandle = 0;
    }
    sdfChipFreeDirect(h);
    return 1;
}

s32 evtGetUnitStat(evtUnit_t *h) {
    return h->motionState;
}

/* Callers pass the promoted full-width value (one reads a byte table field);
 * only the low halfword is stored. */
void evtSetWaitMotion(evtUnit_t *h, s32 motion) {
    h->waitMotion = motion;
}

void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp) {
    h->transitionMotionScale = sp;
}

void evtSetWaitHokanFrame(evtUnit_t *h, s32 before, s32 smooth) {
    h->waitBlendStart = before;
    h->waitBlendEnd = smooth;
}

s32 evtCheckUnitWait(evtUnit_t *h) {
    s32 state = evtGetUnitStat(h);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return 1;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && h->motionTicks > 0 &&
        h->owner->first->stat == SDF_MOTION_STATE_TERMINAL) {
        return 1;
    }
    return 0;
}

void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion) {
    h->firstSlot = group;
    h->secondSlot = motion;
}

void func_001E3C30(evtUnit_t *h, s32 slot, s32 firstValue, s32 secondValue, s32 thirdValue,
                   s32 mode);

void evtRunUnitKutiMotion(evtUnit_t *h)
{
    func_001E3C30(h, h->firstSlot, h->secondSlot, 0, 0, 2);
}

void func_001E3C30(evtUnit_t *h, s32 slot, s32 firstValue, s32 secondValue, s32 thirdValue,
                   s32 mode)
{
    h->slotFlags[slot] = 3;
    h->slotA[slot] = firstValue;
    h->slotB[slot] = secondValue;
    h->slotC[slot] = thirdValue;

    switch (mode) {
    case 0:
        h->slotFlags[slot] |= 4;
        break;
    case 1:
        h->slotFlags[slot] |= 8;
        break;
    case 2:
        h->slotFlags[slot] |= 0x10;
        break;
    }
}


void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode) {
    h->flags &= ~0x1;
    h->flags &= ~0x20;
    h->flags &= ~0x40;
    h->flags &= ~0x400000;
    h->flags &= ~0x800000;
    h->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    (&h->motionSetup)->motionId = motion;
    (&h->motionSetup)->startFrame = before;
    (&h->motionSetup)->endFrame = smooth;
    h->flags |= 0x80;
    h->motionTicks = 0;
    h->motionCallback = 0;
    switch (mode) {
    case 0:
        h->flags |= 0x20;
        break;
    case 1:
        h->flags |= 0x1;
        break;
    case 2:
        break;
    case 3:
        h->flags |= 0x40;
        break;
    }
}

extern f32 __ieee754_acosf(f32 x);

extern void FromRotation(f32 aAngle);

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern sdfVector4_t D_0053A860;

/* vu0 routine: turn the unit toward dir, at most a tenth of a radian a frame. */
s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio) {
    sdfVector4_t axis;
    sdfVector4_t rotation;
    sdfVector4_t flat;
    f32 cross;
    f32 angle;

    axis = D_0053A860;
    evtGetUnitDirVector(h);
    SDF_VU0_COPYVECTOR_A_TO_C();
    evtVu0SubVectorXZAxis(pos, (f32 *)&h->effObj->poseset->pos);
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
    angle = angle * radio;
    if ((h->flags & 0x200000) == 0) {
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
    dds3AddRotPoseSet(h->effObj, &rotation);
    return 0;
}

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtUnitRotationQuat(evtUnit_t *h, f32 *rot, f32 radio) {
    f32 v[4];

    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_GET_ROW2();
    VU0_SET_VF10_Y_CLEAR_W(0.0f);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
    SDF_VU0_LOADVECTOR_B(&h->effObj->poseset->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(v);
    return evtUnitRotationTarget(h, v, radio);
}

s32 evtUnitMoveType0(evtUnit_t *h) {
    f32 v[4];
    f32 scale;
    dds3Basic_t *obj;

    evtUnitRotationTarget(h, h->targetVector, h->directionOffset * 0.01f);
    if (h->directionOffset != 0) {
        evtGetUnitDirVector(h);
        obj = h->effObj;
    } else {
        SDF_VU0_LOADVECTOR(h->targetVector);
        obj = h->effObj;
        SDF_VU0_LOADVECTOR_B(&obj->poseset->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    }
    scale = h->motionParameter * 0.1f;
    SDF_VU0_SCALEVECTOR_V0(scale);
    SDF_VU0_STOREVECTOR(v);
    dds3GetPosPoseSetMCR(obj, (u128 *)v);
    return 1;
}

s32 evtUnitMoveTypePathTrace(evtUnit_t *h) {
    f32 v[4];

    dds3PathBasicControlIsFinished((dds3PathBasicControl_t *)h->pathHandle);
    dds3PathBasicControlGetPos((dds3PathBasicControl_t *)h->pathHandle);
    SDF_VU0_STOREVECTOR(v);
    dds3SetPosPoseSet(h->effObj, (u128 *)v);
    if (h->flags & 0x10) {
        dds3PathBasicControlGetRot((dds3PathBasicControl_t *)h->pathHandle);
        SDF_VU0_COPYVECTOR_A_TO_B();
        FromEuler(0.0f, 3.14159265f, 0.0f);
        Mult();
        SDF_VU0_STOREVECTOR(v);
        dds3SetRotPoseSet(h->effObj, (u128 *)v);
    }
    return 1;
}

void func_001E4108(evtUnit_t *work, void *src) {
    SDF_VU0_COPYVECTOR(work->targetVector, src);
}

void evtSetUnitRot1(evtUnit_t *h, s128 *rot, s32 para1) {
    if (para1 > 0 && para1 <= 100) {
        h->linkedUnit = NULL;
        h->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        SDF_VU0_COPYVECTOR(h->targetVector, rot);
        h->motionParameter = para1;
        h->directionOffset = 0;
        h->motionCallback = 0;
        h->motionTicks = 0;
    }
}

void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), posID, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitRot1(h, (s128 *)&source->rotation, para1);
        h->linkedUnit = worldUnit;
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

void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys) {
    dds3Basic_t *pathSource;
    dds3PathBasicControl_t *path;

    pathSource = dds3SearchSceneBasic(dds3GetControlScene(), pathID, 0x10);
    if (pathSource == NULL) {
        return;
    }
    if (h->pathHandle != 0) {
        dds3DeletePathControl((dds3PathBasicControl_t *)h->pathHandle);
    }
    path = dds3AllocPathBasicWorkData(pathSource);
    h->pathHandle = (s32)path;
    h->pathSpeed = 40.0f / GetSplineLength(path);
    if (movedir == 0) {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 0.0f);
        dds3PathBasicControlAlterMode(path, 0);
    } else {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 1.0f);
        dds3PathBasicControlAlterMode(path, 1);
        h->pathSpeed = -h->pathSpeed;
    }
    switch (movesys) {
    case 0:
        h->motionSubmode = 0;
        h->flags &= ~2;
        break;
    case 1:
        h->motionSubmode = 3;
        h->flags |= 2;
        break;
    }
    switch (movedir) {
    case 0:
        h->flags &= ~EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    case 1:
        h->flags |= EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    }
    switch (rotsys) {
    case 0:
        h->flags &= ~8;
        h->flags &= ~0x10;
        break;
    case 1:
        h->flags |= 8;
        h->flags &= ~0x10;
        break;
    case 2:
        h->flags &= ~8;
        h->flags |= 0x10;
        break;
    }
    h->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    h->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE;
    h->linkedUnit = pathSource;
    dds3PathBasicControlGetPos(path);
    SDF_VU0_STOREVECTOR(h->targetVector);
    h->motionParameter = para1;
    h->directionOffset = para2;
    h->motionCallback = 0;
    h->motionTicks = 0;
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

extern void UnitAnime(evtUnit_t *unit);
extern void mdlSmoothLoopMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);
extern void mdlSmoothSingleMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);
extern s32 mdlChkMotionEndEx(mdlObj_t *model, s32 group);
extern s32 printf(const char *format, ...);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0053A850);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0053A860);

s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *unused, dds3Basic_t *object) {
    s32 moving = 0;
    s32 wobble = 0;
    dds3UnitObjectBasicWorkData_t *data = object->workdata;
    evtUnit_t *unit = data->hUnit;
    mdlObj_t *model;
    s32 i;
    u8 enabled;
    s32 (*callback)(evtUnit_t *, s32);

    if (unit == NULL) {
        return 0;
    }
    model = unit->owner;
    UnitAnime(unit);
    switch (unit->motionState) {
    case EVT_UNIT_MOTION_STATE_IDLE:
    case EVT_UNIT_MOTION_STATE_VECTOR:
        if (unit->flags & 0x80) {
            if (unit->flags & 1) {
                mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            }
            unit->flags &= ~0x80;
        } else if (unit->flags & 0x20) {
            /* Keep selected-ID and current-ID restarts distinct; native retains the
             * current index load even after merging the two paths. */
            s32 currentMotion = model->current.h.motionIndex;

            if (unit->waitMotion != currentMotion) {
                if (model->first->stat == 5) {
                    mdlSmoothLoopMotion(model, 0, unit->waitMotion,
                                        (f32)unit->waitBlendStart,
                                        (f32)unit->waitBlendEnd);
                    model->first->speed = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            } else {
                if (model->first->stat == 5) {
                    mdlSmoothLoopMotion(model, 0, currentMotion,
                                        (f32)unit->waitBlendStart,
                                        (f32)unit->waitBlendEnd);
                    model->first->speed = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            }
        }
        if (unit->flags & 0x40) {
            wobble = 1;
        } else {
            wobble = 0;
        }
        if (unit->motionState == EVT_UNIT_MOTION_STATE_VECTOR) {
            if (evtUnitRotationQuat(unit, unit->targetVector, (f32)unit->motionParameter / 100.0f) !=
                0) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_SOURCE:
        if (evtCheckMoveEnd(unit) != 1 ||
            evtGetNextMovePos(unit) != 0) {
            if (unit->motionSetup.motionId != model->current.h.motionIndex) {
                if (unit->flags & 1) {
                    mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
                } else {
                    mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
                }
            }
            switch (unit->motionSubmode) {
            case 0:
            case 1:
                moving = evtUnitMoveType0(unit);
                break;
            case 2:
                break;
            case 3:
                moving = evtUnitMoveTypePathTrace(unit);
                break;
            default:
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
                break;
            }
            callback = unit->motionCallback;
            if (callback != NULL && callback(unit, 0) == 1) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    case EVT_UNIT_MOTION_STATE_MOTION:
        if (unit->motionTicks == 0) {
            if (unit->flags & 1) {
                mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            }
            unit->flags &= ~0x80;
        } else if (model->first->stat == 5) {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        } else if ((unit->flags & 0x800000) && (f32)unit->motionEndFrame <= model->first->frame) {
            s32 currentMotion;

            unit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
            currentMotion = model->current.h.motionIndex;
            unit->flags &= ~0x800000;
            if (unit->waitMotion != currentMotion) {
                mdlSmoothLoopMotion(model, 0, unit->waitMotion,
                                    (f32)unit->waitBlendStart,
                                    (f32)unit->waitBlendEnd);
                model->first->speed = unit->transitionMotionScale;
                unit->motionTicks = 0;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_VALUE:
        callback = unit->motionCallback;
        if (callback != NULL) {
            if (callback(unit, 0) == 1) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    default:
        return 0;
    }
    if (unit->flags & 0x400000) {
        wobble = 1;
    }
    if (wobble != 0) {
        unit->wobblePhase += 0.104719755f;
        if (6.2831853f <= unit->wobblePhase) {
            unit->wobblePhase -= 6.2831853f;
        }
        unit->effObj->poseset->flag = (unit->effObj->poseset->flag | 1) & ~2;
    }
    for (i = 1; i < 12; i++) {
        enabled = unit->slotFlags[i] & 1;
        if (enabled != 0) {
            if (unit->slotFlags[i] & 2) {
                unit->slotFlags[i] &= ~2;
                printf("set exmot %d_%d\n", i, unit->slotA[i]);
                if (unit->slotFlags[i] & 8) {
                    mdlSmoothLoopMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                } else {
                    mdlSmoothSingleMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                }
            }
            if (mdlChkMotionEndEx(model, i) != 0 &&
                (unit->slotFlags[i] & 4)) {
                mdlSmoothLoopMotion(model, i, unit->tableValues[i], 0.0f, 0.0f);
            }
        }
    }
    MakeLightPacket(unit);
    if (unit->motionTicks < 0x7FFF) {
        unit->motionTicks++;
    }
    if (moving != 0) {
        if (unit->movingTicks < 0x7FFF) {
            unit->movingTicks++;
        }
        unit->idleTicks = 0;
    } else {
        if (unit->idleTicks < 0x7FFF) {
            unit->idleTicks++;
        }
        if (unit->idleTicks >= 6) {
            unit->movingTicks = 0;
        }
    }
    return moving;
}
