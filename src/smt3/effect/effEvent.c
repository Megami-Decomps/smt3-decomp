#include "common.h"
#include "libsdf.h"
#include "effBattle.h"
#include "ps2asm.h"
typedef struct effEveBEDParams_t {
    f32 position[4];
    f32 orientation[4];
    f32 range;
    f32 height;
    f32 scale;
    u32 color;
} effEveBEDParams_t;

/* The constructor reserves 0x38 bytes; only its init prefix is serialized. */
typedef struct effEveBEDCut_t {
    effEveBEDParams_t init;
    void *actor;
    effBTLCut_t *effect;
} effEveBEDCut_t;

typedef struct effSimpleBED_t {
    effBTL_t *handle;
    effEveBEDCut_t *owner;
    effEveBEDParams_t init;
    u8 active;
    u8 unk39[3];
} effSimpleBED_t;

/* Serialized records permit unaligned sources and destinations. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) effEveBEDRecord_t;

extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void effEveBEDCutDraw(effEveBEDCut_t *work);
#define EFF_EVENT_NEUTRAL_COLOR 0x80808080


#define EFF_EVENT_BLUR_SOURCE_BYTES 0x28
#define EFF_EVENT_BLUR_RESOURCE_INDEX 2
#define EFF_EVENT_STAGGERED_RESOURCE_INDEX 3
#define EFF_EVENT_SQUARE_RESOURCE_INDEX 0
#define EFF_EVENT_STAGGERED_SLOT_COUNT 4
#define EFF_EVENT_VECTOR_COMPONENTS 4
#define EFF_EVENT_HALF_TURN 3.14159265f
#define EFF_EVENT_JITTER_RANGE 0.3f
#define EFF_EVENT_JITTER_BASE 0.7f
#define EFF_EVENT_RANDOM_MIDPOINT 0.5f
#define EFF_CHANNEL_ALPHA_SCALE 127.0f
#define EFF_CHANNEL_ALPHA_SHIFT 24
#define EFF_CHANNEL_RGB_COLOR 0x00808080
#define EFF_CURVE_SCALAR_SOURCE_OFFSET 16
#define EFF_CURVE_ROW_FLOATS 4
#define EFF_CURVE_SOURCE_ROW_COUNT 4


/* The stored screen record keeps eight bytes beyond the copied blur quad. */
typedef struct effScreenDrawParams_t {
    effBlurQuad_t source;
    u8 pad28[8];
} effScreenDrawParams_t;


/* A setup record occupies 0x40 bytes in the paired static data blocks.
 * The final word and the callback-adjacent words remain semantically unknown. */
typedef struct effEventSetupRecord_t {
    u8 unknown00[0x1C];
    void *source;
    void *destination;
    u32 copyBytes;
    u8 unknown28[4];
    s32 (*callback)(void *);
    u8 unknown30[8];
    s32 *callbackResult;
    u32 unknown3C;
} effEventSetupRecord_t;

extern s8 Eve_BlurShow;
extern effScreenDrawParams_t Eve_Blur;
extern s8 Eve_PartialMultiBlurShow;
extern effBlurTemplate_t *hEve_PartialMultiBlur;
extern s8 Eve_DistortionBlurShow;
extern effBlurScatterWork_t *hEve_DistortionBlur;
extern s8 Eve_RippleBlurShow;
extern effBlurScaleWork_t *hEve_RippleBlur;
extern s8 Eve_FilterShow;
extern effSolidRectParams_t Eve_Filter;
extern s8 Eve_PartialMultiFilterShow;
extern effResourceRectWork_t *hEve_PartialMultiFilter;
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const effBlurTemplateBody_t *body);
extern u8 D_00320228[];
extern u8 D_003201F8[];
/* Factories accept the serialized parameter prefix used by effect callbacks. */
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(effBlurScatterParams_t *params);
extern u8 D_00320270[];
effResourceRectWork_t *dds3EffectPartialMultiFilterCreate(effResourceRectParams_t *params);
extern u8 D_00320298[];
effBlurScaleWork_t *dds3EffectRippleBlurCreate(effBlurScaleParams_t *params);
extern s8 D_0054F2C5;
/* callbackResult must be valid whenever callback is present. */
extern effEventSetupRecord_t D_003203E0;
extern void *memcpy(void *, const void *, u32);
/* Native callers pass the record address as a word; return it unchanged. */
u32 dblurUpdateHandle(u32 value);
extern void effDebugAllocDrawTag(void);
extern void effDebugDrawMenu(void *);
extern void effDebugLinkDrawTag(void);
extern s8 D_005298CB[];
extern effScreenDrawParams_t D_003202C8;
extern s8 mblurDone;
extern effEventSetupRecord_t mblurDebugMenu;
extern effBlurTemplateBody_t D_00320420;
extern s8 dblurDone;
extern effEventSetupRecord_t dblurDebugMenu;
extern effBlurScatterParams_t D_00320598;
extern s8 evtViewerDirty;
extern effEventSetupRecord_t filterDebugMenu;
extern effSolidRectParams_t D_00320770;
extern s8 mfilterDone;
extern effEventSetupRecord_t mfilterDebugMenu;
extern effResourceRectParams_t D_00320870;
extern s8 rblurDone;
extern effEventSetupRecord_t rblurDebugMenu;
extern effBlurScaleParams_t D_003209B0;
extern sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

typedef struct dds3Billboard_t dds3Billboard_t;
typedef struct dds3EffectPCPData_t dds3EffectPCPData_t;


/* Shared billboard-emitter parameters copied into each live set. Creation and
 * parameter copies clamp startDelaySpread, fadeIn, and fadeOut to at least
 * one; motionDelaySpread is consumed unchanged. */
typedef struct effEventBillParams_t {
    f32 position[4];
    u32 particleCount;
    s32 duration;
    s32 startDelaySpread;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Forwarded to the billboard mode setter in direct draw. */
    u8 pad2A[2];
    s32 fadeIn;
    s32 fadeOut;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent; /* Speeds multiply by 1 + this / 100 per step. */
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge; /* Batched path fades primary texture RGB from black. */
    s32 colorTransitionFrames;
} effEventBillParams_t;

typedef struct effEventBillParticle_t {
    f32 position[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z direction; Y accumulates vertical offset. */
    u8 pad1C[4];
    s32 age;
    s32 motionDelay;
    f32 baseScale;
    f32 scalePhaseX;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 scalePhaseY;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 rotation;
    f32 angularSpeed;
    f32 swayAmplitude;
    f32 swayPhase;
    f32 verticalSpeed;
    f32 lateralSpeed;
    u8 pad5C[4];
} effEventBillParticle_t;

/* Particle storage is followed by this owner in the same allocation. */
typedef struct effEventBillSet_t {
    effEventBillParams_t head;
    effEventBillParticle_t *particles;
    u8 flag;
    u8 pad81[3];
    sdfMemHandle_t *allocationHandle;
} effEventBillSet_t;


/* Shared channel parameter block copied into each allocated channel work. */
typedef struct effChanHead_t {
    f32 controlPoints[4][4];
    u8 enabled;
    u8 pad41[3];
    u32 count;
    s32 steps;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    f32 jitter[4];
    u8 pad68[0x100];
} effChanHead_t;

typedef struct effChanRecord_t {
    s32 delay;
    struct dds3EffectPart_t *param; /* Retained compact effect owner, cloned per record. */
} effChanRecord_t;

/* Allocated channel owner shared by the event factory and channel consumers. */
typedef struct effChanWork_t {
    effChanHead_t head;
    effChanRecord_t *records;
    struct effArrHdr_t *slots; /* Trailing allocation header, not the slot base. */
    sdfMemHandle_t *buffer;
} effChanWork_t;

/* effCopyVertRows writes XYZ at a four-float stride, leaves +0x40..+0x57
 * untouched, then copies four scalars at +0x58. */
typedef struct effFloatRows_t {
    f32 primary[16];
    u8 opaque40[0x18];
    f32 secondary[4];
} effFloatRows_t;

/* Allocated interpolation cursor; effCreateChannel requests exactly 0x18 bytes. */
typedef struct effChan_t {
    sdfMemHandle_t *allocation;
    u32 recordCount;
    effFloatRows_t *rows;
    u32 cursorIndex;
    f32 cursorPosition;
    f32 cursorStep;
} effChan_t;

/* Emitter view: the first eight bytes are not used by this consumer. */
typedef struct effEmit_t {
    u8 opaque00[8];
    effChanWork_t *primitive;
} effEmit_t;

/* Parameter template owner; the four-byte prefix is not read by channel work. */
typedef struct effChanSourceOwner_t {
    u8 opaque00[4];
    dds3EffectPart_t *param;
} effChanSourceOwner_t;

/* Source header followed by the owner used to duplicate each record parameter. */
typedef struct effChanSource_t {
    effChanHead_t head;
    effChanSourceOwner_t *owner;
} effChanSource_t;


extern f32 D_00320B88[];
extern f32 aViewEye[4];
extern u8 aViewDown[];
/* Read vector inputs; VU0 forms first minus second and stores second in vf31. */
void LookAtMatrix(const void *directionPoint, const void *basisOrigin, const void *up);
/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void ReverseMatrix(void);
/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */
void ToMatrix(void);
extern f32 D_00320BA0[];
/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixY_B(f32 angle);
/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. */
void MultMatrixNoArg(void);
u32 sdfMemLock(sdfMemHandle_t *allocation);
extern s32 EffEveCPSmokeCount;
extern dds3Billboard_t *dds3BillboardFileLoad(s32 kind, const char *path);
extern u32 hEffEveCPSmokeBill01Tex;
extern u32 hEffEveCPSmokeBill02Tex;
void dds3BillboardDelete(struct dds3Billboard_t *obj);
void sdfMemFreeDirect(sdfMemHandle_t *allocation);
extern effRandState_t dds3EffectBase[];
/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *state);
/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *state);
extern f32 aViewTarget[4];
void dds3BillboardSetMode(struct dds3Billboard_t *billboard, s32 mode);
f32 sdfSin(f32 angle);
/* Blend packed RGBA8888 colors; t >= 1 returns colorB unchanged. */
u32 effColorBlend(u32 colorA, u32 colorB, f32 t);
f32 sdfCos(f32 angle);
/* Copy one 128-bit vector from the read-only source to destination storage. */
void dds3BillboardSetPosition(void *dst, const void *src);
void dds3BillboardSetScale(struct dds3Billboard_t *billboard, f32 x, f32 y);
void dds3BillboardSetRGBA(struct dds3Billboard_t *billboard, u32 parameter);
/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct dds3Billboard_t *billboard, f32 angle);
extern void dds3BillboardDraw(dds3Billboard_t *obj);
extern effEventBillParams_t EVE_ParSmokeParam;
dds3EffectPart_t *dds3EffectPartCreate(u16 kind, void *source);
dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *source);
/* Return a borrowed block at the selected record's signed relative offset. */
void *dds3EffectPCPDataGetData(const dds3EffectPCPData_t *table, s32 recordIndex);
u32 dds3EffectPCPDataGetType(const dds3EffectPCPData_t *table, s32 recordIndex);
void dds3Bezier4PDelete(struct effArrHdr_t *work);
void dds3EffectPartDelete(dds3EffectPart_t *work);
effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *table, s32 slotIndex);
void dds3EffectPartSetColor(dds3EffectPart_t *work, u32 value);
s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);
/* Optional source and matrix operations retain their generic pointer domains. */
void dds3EffectPartSetPosition(dds3EffectPart_t *work, void *source);
void dds3EffectPartDraw(dds3EffectPart_t *work);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern effArrHdr_t *dds3Bezier4PCreate(s32 count);


void Eve_BlurON(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void) {
    Eve_BlurShow = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void Eve_BlurSetParam(void *parameters) {
    memcpy(&Eve_Blur, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
effScreenDrawParams_t *Eve_BlurGetParam(void) {
    return &Eve_Blur;
}

void Eve_PartialMultiBlurON(void) {
    Eve_PartialMultiBlurShow = 1;
}

void Eve_PartialMultiBlurOFF(void) {
    Eve_PartialMultiBlurShow = 0;
}

/* Copy the pixel rectangle only; retain its selected source resource. */
void Eve_PartialMultiBlurSetParam(effBlurTemplateBody_t *parameters) {
    hEve_PartialMultiBlur->body = *parameters;
}

/* Return the current textured-blur work without retaining it. */
effBlurTemplate_t *Eve_PartialMultiBlurGetParams(void) {
    return hEve_PartialMultiBlur;
}

void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiBlur->texture = texture;
}

/* Select the same resource-table entry used by the filter-blur initializer. */
void Eve_PartialMultiBlurSetDefTex(void) {
    hEve_PartialMultiBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_DistortionBlurON(void) {
    Eve_DistortionBlurShow = 1;
}

void Eve_DistortionBlurOFF(void) {
    Eve_DistortionBlurShow = 0;
}

/* Update scatter parameters without replacing the owned allocation or slots. */
void Eve_DistortionBlurSetParam(effBlurScatterParams_t *parameters) {
    hEve_DistortionBlur->params = *parameters;
}

/* Return the current filter-blur work without retaining it. */
effBlurScatterWork_t *Eve_DistortionBlurGetParams(void) {
    return hEve_DistortionBlur;
}

void Eve_DistortionBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_DistortionBlur->texture = sourceHandle;
}

/* Install the filter-blur resource-table texture without changing its slots. */
void Eve_DistortionBlurSetDefTex(void) {
    hEve_DistortionBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_RippleBlurON(void) {
    Eve_RippleBlurShow = 1;
}

void Eve_RippleBlurOFF(void) {
    Eve_RippleBlurShow = 0;
}

/* Update scale parameters without replacing the owned allocation or slots. */
void Eve_RippleBlurSetParam(effBlurScaleParams_t *parameters) {
    hEve_RippleBlur->params = *parameters;
}

/* Return the current staggered-blur work without retaining it. */
effBlurScaleWork_t *Eve_RippleBlurGetParams(void) {
    return hEve_RippleBlur;
}

void Eve_RippleBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_RippleBlur->texture = sourceHandle;
}

/* Install the staggered-blur resource-table texture without changing its slots. */
void Eve_RippleBlurSetDefTex(void) {
    hEve_RippleBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void Eve_FilterON(void) {
    Eve_FilterShow = 1;
}

void Eve_FilterOFF(void) {
    Eve_FilterShow = 0;
}

/* Replace the complete solid-rectangle parameter block. */
void Eve_FilterSetParam(effSolidRectParams_t *parameters) {
    Eve_Filter = *parameters;
}

/* Expose the solid-rectangle parameter block by address. */
effSolidRectParams_t *Eve_FilterGetParams(void) {
    return &Eve_Filter;
}

void Eve_PartialMultiFilterON(void) {
    Eve_PartialMultiFilterShow = 1;
}

void Eve_PartialMultiFilterOFF(void) {
    Eve_PartialMultiFilterShow = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(effResourceRectParams_t *parameters) {
    hEve_PartialMultiFilter->params = *parameters;
}

/* Return the current textured-square work without retaining it. */
effResourceRectWork_t *Eve_PartialMultiFilterGetParams(void) {
    return hEve_PartialMultiFilter;
}

void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiFilter->texture = texture;
}

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->texture = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((effBlurTemplateBody_t *)D_00320228);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((effBlurScatterParams_t *)D_003201F8);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((effResourceRectParams_t *)D_00320270);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((effBlurScaleParams_t *)D_00320298);
    Eve_RippleBlurGetParams()->params.count = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

INCLUDE_ASM(const s32, "effect/effEvent", Eve_EffectDraw);

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
s32 func_001AD6E8(void) {
    u8 ready = D_0054F2C5;

    if (D_0054F2C5 == 0) {
        effEventSetupRecord_t *state = &D_003203E0;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        D_0054F2C5 = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&D_003203E0);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&D_003203E0);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        D_0054F2C5 = 0;
    }
    return D_0054F2C5;
}

/* Expose this screen-draw setup block; no parameter copy is made. */
effScreenDrawParams_t *evtGetUnityCalcArea(void) {
    return &D_003202C8;
}

/* Replace the screen-draw source prefix while preserving the trailing bytes. */
void evtSetUnityCalcArea(void *parameters) {
    memcpy(&D_003202C8, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Prepare the blur-template callback from its explicit parameter block,
 * then process the channel; its optional copy still uses source/destination. */
s32 mblurProcess(void) {
    u8 ready = mblurDone;

    if (mblurDone == 0) {
        effEventSetupRecord_t *state = &mblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00320420);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        mblurDone = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&mblurDebugMenu);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&mblurDebugMenu);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        mblurDone = 0;
    }
    return mblurDone;
}

/* Expose the blur-template setup parameters by address. */
effBlurTemplateBody_t *mblurGetParam(void) {
    return &D_00320420;
}

/* Replace the setup template; this does not modify the active blur work. */
void mblurSetParam(effBlurTemplateBody_t *parameters) {
    D_00320420 = *parameters;
}

/* Prepare scatter setup from its explicit parameters, then process the channel. */
s32 dblurProcess(void) {
    u8 ready = dblurDone;

    if (dblurDone == 0) {
        effEventSetupRecord_t *state = &dblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00320598);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        dblurDone = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&dblurDebugMenu);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&dblurDebugMenu);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        dblurDone = 0;
    }
    return dblurDone;
}

/* Expose the scatter-blur setup parameters by address. */
effBlurScatterParams_t *dblurGetParam(void) {
    return &D_00320598;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void dblurSetParam(effBlurScatterParams_t *parameters) {
    D_00320598 = *parameters;
}

/* Prepare solid-rectangle setup from the channel source, then process it. */
s32 evtViewerCheckDirty(void) {
    u8 ready = evtViewerDirty;

    if (evtViewerDirty == 0) {
        effEventSetupRecord_t *state = &filterDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(state->source);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        evtViewerDirty = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&filterDebugMenu);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&filterDebugMenu);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        evtViewerDirty = 0;
    }
    return evtViewerDirty;
}

/* Expose the solid-rectangle setup parameters by address. */
effSolidRectParams_t *dblurGetParam2(void) {
    return &D_00320770;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void dblurSetParam2(effSolidRectParams_t *parameters) {
    D_00320770 = *parameters;
}

/* Prepare resource-template setup from its explicit body, then process the channel. */
s32 mfilterRun(void) {
    u8 ready = mfilterDone;

    if (mfilterDone == 0) {
        effEventSetupRecord_t *state = &mfilterDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_00320870);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        mfilterDone = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&mfilterDebugMenu);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&mfilterDebugMenu);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        mfilterDone = 0;
    }
    return mfilterDone;
}

/* Expose the resource-template setup body by address. */
effResourceRectParams_t *mfilterGetParam(void) {
    return &D_00320870;
}

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(effResourceRectParams_t *parameters) {
    D_00320870 = *parameters;
}

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
s32 rblurProcess(void) {
    u8 ready = rblurDone;

    if (rblurDone == 0) {
        effEventSetupRecord_t *state = &rblurDebugMenu;

        if (state->callback != NULL) {
            *state->callbackResult = state->callback(&D_003209B0);
        }
        if (state->destination != NULL) {
            if (state->source != NULL) {
                memcpy(state->destination, state->source, state->copyBytes);
            }
        }
        ready = 1;
        rblurDone = ready;
    }
    if (ready != 0) {
        dblurUpdateHandle((u32)&rblurDebugMenu);
        effDebugAllocDrawTag();
        effDebugDrawMenu(&rblurDebugMenu);
        effDebugLinkDrawTag();
    }
    if (D_005298CB[0] < 0) {
        rblurDone = 0;
    }
    return rblurDone;
}

/* Expose the scale-blur setup parameters by address. */
effBlurScaleParams_t *rblurGetParam(void) {
    return &D_003209B0;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void rblurSetParam(effBlurScaleParams_t *parameters) {
    D_003209B0 = *parameters;
}

effBTL_t *effEveBEDCreate(effBTL_t *source) {
    return effBTLCreate(source);
}

void effEveBEDDelete(effBTL_t *handle) {
    effBTLDelete(handle);
}

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCut_t *effEveBEDCutCreate(effBTL_t *owner, u16 kind, const void *params) {
    const effEveBEDParams_t *init = (const effEveBEDParams_t *)params;
    effEveBEDCut_t *work = sdfChipAllocDirect(sizeof(effEveBEDCut_t));

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effBTLCutCreate(owner, kind, 0, 0);
    effBTLCutSetScale(work->effect, init->scale);
    return work;
}

/* Release the attached effect before freeing the event work. */
void effEveBEDCutDelete(effEveBEDCut_t *work) {
    effBTLCutDelete(work->effect);
    sdfChipFreeDirect(work);
}

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEveBEDCutSetParams(effEveBEDCut_t *destination, const void *source) {
    *(effEveBEDRecord_t *)destination = *(const effEveBEDRecord_t *)source;
}

/* This copies a packed 0x30-byte record, not a complete 0x60-byte billboard state. */
void effEveBEDCutGetParams(const effEveBEDRecord_t *source, effEveBEDRecord_t *destination) {
    *destination = *source;
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEveBEDCutSetScale(effEveBEDCut_t *work, f32 scale) {
    effBTLCutSetScale(work->effect, scale);
}

void effEveBEDCutSetUnitHandle(effEveBEDCut_t *work, void *actor) {
    work->actor = actor;
}

INCLUDE_ASM(const s32, "effect/effEvent", effEveBEDCutDraw);

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(effBTL_t *handle, f32 scale) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->init.orientation[3] = 1.0f;
    work->init.range = 50.0f;
    work->init.height = 180.0f;
    work->init.position[1] = -90.0f;
    work->init.scale = scale;
    work->init.color = EFF_EVENT_NEUTRAL_COLOR;
    work->init.orientation[0] = 0;
    work->init.orientation[1] = 0;
    work->init.orientation[2] = 0;
    work->init.position[0] = 0;
    work->init.position[2] = 0;
    work->init.position[3] = 0;
    work->handle = effEveBEDCreate(handle);
    work->owner = effEveBEDCutCreate(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effSimpleBEDDelete(effSimpleBED_t *work) {
    effEveBEDCutDelete(work->owner);
    if (work->active != 0) {
        effEveBEDDelete(work->handle);
    }
    sdfChipFreeDirect(work);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *src) {
    effEveBEDParams_t *block = &src->init;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->owner = effEveBEDCutCreate(src->handle, 0, block);
    work->handle = src->handle;
    memcpy(&work->init, block, sizeof(work->init));
    work->active = 0;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effSimpleBEDDraw(effSimpleBED_t *work) {
    effEveBEDCutDraw(work->owner);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *work, f32 *position) {
    work->init.position[0] = position[0];
    work->init.position[1] = position[1] - work->init.height * 0.5f;
    work->init.position[2] = position[2];
    work->init.position[3] = 0;
    effEveBEDCutSetParams(work->owner, &work->init);
}

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *work, u32 color) {
    work->init.color = color;
    effEveBEDCutSetParams(work->owner, &work->init);
}

void effEveBEDPartGetPositionPtr(effEveBEDParams_t *source, effBTLAimParams_t *params);

INCLUDE_ASM(const s32, "effect/effEvent", effEveBEDPartGetPositionPtr);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005343D0);

effEventBillSet_t *effEVECombParSmokeCreate(effEventBillParams_t *src) {
    u32 particleCount = src->particleCount;
    u32 particleBytes = particleCount * sizeof(effEventBillParticle_t);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(particleBytes + sizeof(effEventBillSet_t));
    effEventBillParticle_t *particle = (effEventBillParticle_t *)sdfMemLock(allocationHandle);
    effEventBillSet_t *work = (effEventBillSet_t *)((u8 *)particle + particleBytes);
    u32 particleIndex;

    work->head = *src;
    work->allocationHandle = allocationHandle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.fadeIn <= 0) {
        work->head.fadeIn = 1;
    }
    if (work->head.fadeOut <= 0) {
        work->head.fadeOut = 1;
    }
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = 1;
    }
    EffEveCPSmokeCount = EffEveCPSmokeCount + 1;
    if (EffEveCPSmokeCount == 1) {
        hEffEveCPSmokeBill01Tex = (u32)dds3BillboardFileLoad(0, "/efftool/bill/dbball01.tmx");
        hEffEveCPSmokeBill02Tex = (u32)dds3BillboardFileLoad(0, "/efftool/bill/dbball02.tmx");
    }
    particleCount = work->head.particleCount;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        particle->age = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(effEventBillSet_t *work) {
    EffEveCPSmokeCount = EffEveCPSmokeCount - 1;
    if (EffEveCPSmokeCount == 0) {
        dds3BillboardDelete((dds3Billboard_t *)hEffEveCPSmokeBill01Tex);
        dds3BillboardDelete((dds3Billboard_t *)hEffEveCPSmokeBill02Tex);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEVECombParSmokePartInit(effEventBillSet_t *work, s32 index) {
    effEventBillParticle_t *particle = &work->particles[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    particle->age = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % startDelaySpread);
    particle->motionDelay = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % motionDelaySpread);
    scaleFactor = dds3GetRandFloatA(dds3EffectBase) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    particle->baseScale = work->head.baseScale * scaleFactor;
    particle->scalePhaseX = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseY = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = work->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scalePhaseStepY = work->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = work->head.scaleAmplitudeX * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = work->head.scaleAmplitudeY * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->rotation = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (dds3GetRandA((effRandState_t *)dds3EffectBase) & 1) {
        particle->angularSpeed = work->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->angularSpeed = -(work->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = work->head.spawnRadius;
    direction[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->position[0] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[0];
    particle->position[1] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[1];
    particle->position[2] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[2];
    particle->motion[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(particle->motion);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->motion);
    particle->swayAmplitude = work->head.swayAmplitude * (dds3GetRandFloatA(dds3EffectBase) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    particle->swayPhase = 0;
    particle->verticalSpeed = work->head.verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    particle->lateralSpeed = work->head.lateralSpeed * (dds3GetRandFloatA(dds3EffectBase) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

void eff_1AEC58(effEventBillSet_t *work);

INCLUDE_ASM(const s32, "effect/effEvent", eff_1AEC58);

INCLUDE_ASM(const s32, "effect/effEvent", effEVECombParSmokeDraw);

void effEVECombParSmokeSetPosition(void *dst, void *src);

INCLUDE_ASM(const s32, "effect/effEvent", effEVECombParSmokeSetPosition);

/* Set the native byte flag without normalizing it to a boolean. */
void effEVECombParSmokeSetZtest(effEventBillSet_t *work, u8 flag) {
    work->flag = flag;
}

/* Copy the serialized emitter parameters without changing their values. */
void effEVECombParSmokeGetParams(const effEventBillParams_t *source, effEventBillParams_t *destination) {
    *destination = *source;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effEVECombParSmokeSetParams(effEventBillParams_t *destination, const effEventBillParams_t *source) {
    *destination = *source;
    if (destination->fadeIn <= 0) {
        destination->fadeIn = 1;
    }
    if (destination->fadeOut <= 0) {
        destination->fadeOut = 1;
    }
    if (destination->startDelaySpread <= 0) {
        destination->startDelaySpread = 1;
    }
}

/* Create from the installed default emitter block; the returned pointer is unused. */
void effEVECombParSmokeCreate2(void) {
    effEVECombParSmokeCreate(&EVE_ParSmokeParam);
}

effChanWork_t *effAPPoint4Create(const effChanHead_t *source, u16 kind, void *params) {
    const effChanHead_t *head = source;
    u32 recordCount = head->count;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(recordCount * sizeof(effChanRecord_t) + sizeof(effChanWork_t));
    effChanWork_t *work = (effChanWork_t *)sdfMemLock(allocation);
    effChanRecord_t *record = (effChanRecord_t *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = *head;
    work->buffer = allocation;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        delayModulus = work->head.spread;
        parameterTemplate = dds3EffectPartCreate(kind, params);
        record->param = parameterTemplate;
        record->delay = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->param = dds3EffectPartCopy(parameterTemplate);
            record->delay = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            record++;
        }
    }
    return work;
}

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
void effAPPoint4CreatePCF(void *data) {
    void *block0 = dds3EffectPCPDataGetData(data, 0);
    void *block1 = dds3EffectPCPDataGetData(data, 1);

    effAPPoint4Create(block0, dds3EffectPCPDataGetType(data, 1), block1);
}

/* Clone channel work, allocate slots and duplicate a parameter template per record.
 * The record count is captured before allocation; a nonpositive delay modulus becomes one. */
effChanWork_t *effAPPoint4Copy(effChanSource_t *source) {
    u32 recordCount = source->head.count;
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordCount * sizeof(effChanRecord_t) + sizeof(effChanWork_t));
    effChanWork_t *work = (effChanWork_t *)sdfMemLock(allocationHandle);
    effChanRecord_t *recordCursor = (effChanRecord_t *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = source->head;
    work->buffer = allocationHandle;
    work->records = recordCursor;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->head.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->param = dds3EffectPartCopy(parameterTemplate);
            recordCursor->delay = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effAPPoint4Delete(effChanWork_t *work) {
    effChanRecord_t *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    dds3Bezier4PDelete(work->slots);
    recordCount = work->head.count;
    recordCursor = work->records;
    if (recordCount != 0) {
        do {
            dds3EffectPartDelete(recordCursor->param);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfMemFreeDirect(work->buffer);
}

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effAPPoint4PartInit(effChanWork_t *work, u32 recordIndex) {
    f32 jitterScale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 jitteredPoint[4];
    effChanRecord_t *record = work->records + recordIndex;
    effCubicBezierSlot_t *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    cursorStep = 1.0f / (f32)work->head.steps;
    bezierSlot = dds3Bezier4PGetPart(work->slots, recordIndex);
    bezierSlot->t = 0;
    bezierSlot->step = cursorStep;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale[0] = work->head.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[0][0] = jitteredPoint[0];
    bezierSlot->controlPoints[0][1] = jitteredPoint[1];
    bezierSlot->controlPoints[0][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale[0] = work->head.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[1][0] = jitteredPoint[0];
    bezierSlot->controlPoints[1][1] = jitteredPoint[1];
    bezierSlot->controlPoints[1][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale[0] = work->head.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[2][0] = jitteredPoint[0];
    bezierSlot->controlPoints[2][1] = jitteredPoint[1];
    bezierSlot->controlPoints[2][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale[0] = work->head.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint[0];
    bezierSlot->controlPoints[3][1] = jitteredPoint[1];
    bezierSlot->controlPoints[3][2] = jitteredPoint[2];
    dds3EffectPartSetColor(record->param, 0);
}

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void effAPPoint4Draw(effChanWork_t *work) {
    effChanRecord_t *recordCursor = work->records;
    effArrHdr_t *bezierSlots = work->slots;
    u32 recordCount = work->head.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = work->head.enabled;
    s32 activeSteps = work->head.steps;
    s32 delayModulus = work->head.spread;
    s32 fadeInSteps = work->head.fadeIn;
    s32 fadeOutSteps = work->head.fadeOut;
    f32 position[3];

    (void)dds3Bezier4PGetPart(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->delay;

        if (recordTime == 0) {
            effAPPoint4PartInit(work, recordIndex);
        }
        if (recordTime > 0 && recordTime <= activeSteps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            dds3Bezier4PPartRun(bezierSlots, recordIndex, position);
            fadePhase = fadeInSteps > recordTime;
            if (fadePhase) {
                opacity = (f32)recordTime / (f32)fadeInSteps;
            } else {
                fadePhase = activeSteps - recordTime;
                if (fadePhase <= fadeOutSteps) {
                    opacity = (f32)fadePhase / (f32)fadeOutSteps;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * EFF_CHANNEL_ALPHA_SCALE);
            color = (alpha << EFF_CHANNEL_ALPHA_SHIFT) | EFF_CHANNEL_RGB_COLOR;
            dds3EffectPartSetColor(recordCursor->param, color);
            dds3EffectPartSetPosition(recordCursor->param, position);
            dds3EffectPartDraw(recordCursor->param);
        }
        if (recordTime < activeSteps) {
            recordCursor->delay++;
        } else if (cycleEnabled != 0) {
            recordCursor->delay = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        } else {
            recordCursor->delay++;
        }
        recordCursor++;
        recordIndex++;
    } while (recordIndex < recordCount);
}

/* Copy XYZ from four float4 rows without touching destination W, then copy four trailing scalars. */
void func_001B0730(effChan_t *channel, f32 *source) {
    f32 *sourceRow;
    u32 rowIndex;
    f32 *scalarCursor;
    f32 *destinationRow;

    sourceRow = source;
    rowIndex = 0;
    source += EFF_CURVE_SCALAR_SOURCE_OFFSET;
    scalarCursor = channel->rows->secondary;
    destinationRow = channel->rows->primary;
    do {
        rowIndex++;
        destinationRow[0] = sourceRow[0];
        destinationRow[1] = sourceRow[1];
        destinationRow[2] = sourceRow[2];
        sourceRow += EFF_CURVE_ROW_FLOATS;
        destinationRow += EFF_CURVE_ROW_FLOATS;
        *scalarCursor = *source++;
        scalarCursor++;
    } while (rowIndex < EFF_CURVE_SOURCE_ROW_COUNT);
}

/* Jitter each slot and choose its initial unsigned-modulo step.
 * Stored record time is one ahead of the Bezier cursor parameter's sampled step. */
void effEveAPP4Randomize(effEmit_t *emitter) {
    effChanWork_t *channelWork = emitter->primitive;
    u32 stepModulus = channelWork->head.steps;
    u32 recordCount = channelWork->head.count;
    effChanRecord_t *recordCursor = channelWork->records;
    u32 recordIndex = 0;
    effCubicBezierSlot_t *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effAPPoint4PartInit(channelWork, recordIndex);
        recordCursor->delay = dds3GetRandA((effRandState_t *)dds3EffectBase) % stepModulus;
        bezierSlot = dds3Bezier4PGetPart(channelWork->slots, recordIndex);
        recordIndex++;
        randomStep = recordCursor->delay;
        cursorStep = bezierSlot->step;
        recordCursor->delay = randomStep + 1;
        recordCursor++;
        bezierSlot->t = cursorStep * (f32)randomStep;
    } while (recordIndex < recordCount);
}

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534420);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534430);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534440);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534450);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534460);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534470);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534480);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534490);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344A0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344B0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344C0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344D0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344E0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_005344F0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534500);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534510);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534520);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534530);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534540);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534550);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534560);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534570);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534588);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00534598);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_BlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_DistortionBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_FilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiFilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_RippleBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F280);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F288);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F290);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F298);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F2F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F300);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F308);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F310);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F318);

INCLUDE_SDATA(const s32, "effect/effEvent", mfilterDone);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F31D);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F320);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F328);

INCLUDE_SDATA(const s32, "effect/effEvent", EffEveCPSmokeCount);

INCLUDE_SDATA(const s32, "effect/effEvent", hEffEveCPSmokeBill01Tex);

INCLUDE_SDATA(const s32, "effect/effEvent", hEffEveCPSmokeBill02Tex);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F33C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F340);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F348);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F350);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F358);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F360);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0054F368);

