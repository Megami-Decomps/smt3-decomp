#include "common.h"
#include "libsdf.h"
#include "effBattle.h"
#include "event.h"
#include "model.h"
#include "ps2asm.h"
#include "effDebug.h"

typedef struct effEveBEDParams_t {
    f32 center[4];
    f32 rot[4];
    f32 r;
    f32 h;
    f32 scale;
    u32 abgr;
} effEveBEDParams_t;

/* The constructor reserves 0x38 bytes; only its init prefix is serialized. */
typedef struct effEveBEDCut_t {
    effEveBEDParams_t init;
    evtUnit_t *actor;
    effBTLCut_t *effect;
} effEveBEDCut_t;

typedef struct effSimpleBED_t {
    effBTL_t *hBED;
    effEveBEDCut_t *hCut;
    effEveBEDParams_t pParams;
    u8 original;
    u8 unk39[3];
} effSimpleBED_t;

/* Serialized records permit unaligned sources and destinations. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) effEveBEDRecord_t;

extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut);
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




extern s8 Eve_BlurShow;
extern effScreenDrawParams_t Eve_Blur;
extern s8 Eve_PartialMultiBlurShow;
extern effBlurTemplate_t *hEve_PartialMultiBlur;
extern s8 Eve_DistortionBlurShow;
extern effBlurScatterWork_t *hEve_DistortionBlur;
extern s8 Eve_RippleBlurShow;
extern effBlurScaleWork_t *hEve_RippleBlur;
extern s8 Eve_FilterShow;
extern dds3Effect_Filter_t Eve_Filter;
extern s8 Eve_PartialMultiFilterShow;
extern dds3Effect_PartialMultiFilter_t *hEve_PartialMultiFilter;
extern void dds3EffectBlur(const dds3Effect_Blur_t *source);
extern void dds3EffectPartialMultiBlurDraw(effBlurTemplate_t *work);
extern void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);
extern void dds3EffectRippleBlurDraw(effBlurScaleWork_t *work);
extern void dds3EffectFilter(dds3Effect_Filter_t *params);
extern void dds3EffectPartialMultiFilterDraw(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);
extern u8 D_00320228[];
extern u8 D_003201F8[];
/* Factories accept the serialized parameter prefix used by effect callbacks. */
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);
extern u8 D_00320270[];
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);
extern u8 D_00320298[];
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);
extern s8 D_0054F2C5;
/* The result slot must be valid whenever the create callback is present. */
extern effDebugMenu_t D_003203E0;
extern void *memcpy(void *, const void *, u32);
extern void effDebugAllocDrawTag(void);
extern void effDebugLinkDrawTag(void);
extern s8 D_005298CB[];
extern effScreenDrawParams_t D_003202C8;
extern s8 mblurDone;
extern effDebugMenu_t mblurDebugMenu;
extern dds3Effect_PartialMultiBlurParam_t D_00320420;
extern s8 dblurDone;
extern effDebugMenu_t dblurDebugMenu;
extern dds3Effect_DistortionBlurParam_t D_00320598;
extern s8 evtViewerDirty;
extern effDebugMenu_t filterDebugMenu;
extern dds3Effect_Filter_t D_00320770;
extern s8 mfilterDone;
extern effDebugMenu_t mfilterDebugMenu;
extern dds3Effect_PartialMultiFilterParam_t D_00320870;
extern s8 rblurDone;
extern effDebugMenu_t rblurDebugMenu;
extern dds3Effect_RippleBlurParam_t D_003209B0;
extern sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

typedef struct dds3Billboard_t dds3Billboard_t;


/* Shared billboard-emitter parameters copied into each live set. Creation and
 * parameter copies clamp startDelaySpread, fadeIn, and fadeOut to at least
 * one; motionDelaySpread is consumed unchanged. */
typedef struct effEVECombParSmokeParam_t {
    f32 position[4];
    u32 number;
    s32 duration;
    s32 wait;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Forwarded to the billboard mode setter in direct draw. */
    u8 pad2A[2];
    s32 fade_in;
    s32 fade_out;
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
} effEVECombParSmokeParam_t;

typedef struct effEVECombParSmokePart_t {
    f32 pos[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z direction; Y accumulates vertical offset. */
    u8 pad1C[4];
    s32 frame;
    s32 wait;
    f32 scale;
    f32 s_w_r;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 s_h_r;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 roll;
    f32 roll_s;
    f32 amplitude;
    f32 angle;
    f32 up_s;
    f32 shift_s;
    u8 pad5C[4];
} effEVECombParSmokePart_t;

/* Particle storage is followed by this owner in the same allocation. */
typedef struct effEVECombParSmoke_t {
    effEVECombParSmokeParam_t head;
    effEVECombParSmokePart_t *pPart;
    u8 Ztest;
    u8 pad81[3];
    sdfMemHandle_t *hMem;
} effEVECombParSmoke_t;


/* Shared channel parameter block copied into each allocated channel work. */
typedef struct effAPPoint4Param_t {
    f32 controlPoints[4][4];
    u8 enabled;
    u8 pad41[3];
    u32 count;
    s32 steps;
    s32 spread;
    s32 fade_in;
    s32 fade_out;
    f32 jitter[4];
    u8 pad68[0x100];
} effAPPoint4Param_t;

typedef struct effAPPoint4Part_tag {
    s32 frame;
    struct dds3EffectPart_t *hPart; /* Retained compact effect owner, cloned per record. */
} effAPPoint4Part_tag;

/* Allocated channel owner shared by the event factory and channel consumers. */
typedef struct effAPPoint4_tag {
    effAPPoint4Param_t param;
    effAPPoint4Part_tag *pPart;
    struct effArrHdr_t *pBezier4P; /* Trailing allocation header, not the slot base. */
    sdfMemHandle_t *hMem;
} effAPPoint4_tag;

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
    effAPPoint4_tag *primitive;
} effEmit_t;

/* Parameter template owner; the four-byte prefix is not read by channel work. */
typedef struct effChanSourceOwner_t {
    u8 opaque00[4];
    dds3EffectPart_t *param;
} effChanSourceOwner_t;

/* Source header followed by the owner used to duplicate each record parameter. */
typedef struct effChanSource_t {
    effAPPoint4Param_t head;
    effChanSourceOwner_t *owner;
} effChanSource_t;


extern f32 D_00320B88[];
extern f32 aViewEye[4];
extern u8 aViewDown[];
/* Read vector inputs; VU0 forms first minus second and stores second in vf31. */
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void ReverseMatrix(void);
/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */
void ToMatrix(void);
extern f32 D_00320BA0[];
/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixY_B(f32 aAngle);
/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. */
void MultMatrixNoArg(void);
u32 sdfMemLock(sdfMemHandle_t *aHandle);
extern s32 EffEveCPSmokeCount;
extern dds3Billboard_t *dds3BillboardFileLoad(s32 kind, const char *path);
extern u32 hEffEveCPSmokeBill01Tex;
extern u32 hEffEveCPSmokeBill02Tex;
void dds3BillboardDelete(struct dds3Billboard_t *hData);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern effRandState_t dds3EffectBase[];
/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);
/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);
extern f32 aViewTarget[4];
void dds3BillboardSetMode(struct dds3Billboard_t *hBillboard, s32 mode);
f32 sdfSin(f32 aRadius);
/* Blend packed RGBA8888 colors; t >= 1 returns colorB unchanged. */
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
f32 sdfCos(f32 aRadius);
/* Copy one 128-bit vector from the read-only source to destination storage. */
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetScale(struct dds3Billboard_t *hBillboard, f32 x, f32 y);
void dds3BillboardSetRGBA(struct dds3Billboard_t *hBillboard, u32 abgr);
/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct dds3Billboard_t *hBillboard, f32 rot);
extern void dds3BillboardDraw(dds3Billboard_t *hData);
extern effEVECombParSmokeParam_t EVE_ParSmokeParam;
dds3EffectPart_t *dds3EffectPartCreate(u16 type, void *pData);
dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *hEffPartSrc);
/* Return a borrowed block at the selected record's signed relative offset. */
void dds3Bezier4PDelete(struct effArrHdr_t *pEffBezier4P);
void dds3EffectPartDelete(dds3EffectPart_t *hEffPart);
effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *pEffBezier4P, s32 index);
void dds3EffectPartSetColor(dds3EffectPart_t *hEffPart, u32 abgr);
s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);
/* Optional source and matrix operations retain their generic pointer domains. */
void dds3EffectPartSetRotateMatrix(dds3EffectPart_t *hEffPart, sdfMatrix4_t *pMatrix);
extern void Mult(void);
extern void ToMatrix(void);
extern void MultMatrixNoArg(void);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);
extern sdfVector4_t D_005343D0;
void effEveBEDPartGetPositionPtr(effEveBEDParams_t *pParams, effBTLAimParams_t *pPosPtr);
void dds3EffectPartSetPosition(dds3EffectPart_t *hEffPart, void *pPos);
void dds3EffectPartDraw(dds3EffectPart_t *hEffPart);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);
extern effArrHdr_t *dds3Bezier4PCreate(s32 number);


void Eve_BlurON(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void) {
    Eve_BlurShow = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void Eve_BlurSetParam(void *pParam) {
    memcpy(&Eve_Blur, pParam, EFF_EVENT_BLUR_SOURCE_BYTES);
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
void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *pParam) {
    hEve_PartialMultiBlur->body = *pParam;
}

/* Return the current textured-blur work without retaining it. */
effBlurTemplate_t *Eve_PartialMultiBlurGetParams(void) {
    return hEve_PartialMultiBlur;
}

void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *hTex) {
    hEve_PartialMultiBlur->texture = hTex;
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
void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *pParam) {
    hEve_DistortionBlur->params = *pParam;
}

/* Return the current filter-blur work without retaining it. */
effBlurScatterWork_t *Eve_DistortionBlurGetParams(void) {
    return hEve_DistortionBlur;
}

void Eve_DistortionBlurSetTex(sdfTexHandle_t *hTex) {
    hEve_DistortionBlur->texture = hTex;
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
void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *pParam) {
    hEve_RippleBlur->params = *pParam;
}

/* Return the current staggered-blur work without retaining it. */
effBlurScaleWork_t *Eve_RippleBlurGetParams(void) {
    return hEve_RippleBlur;
}

void Eve_RippleBlurSetTex(sdfTexHandle_t *hTex) {
    hEve_RippleBlur->texture = hTex;
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
void Eve_FilterSetParam(dds3Effect_Filter_t *pParam) {
    Eve_Filter = *pParam;
}

/* Expose the solid-rectangle parameter block by address. */
dds3Effect_Filter_t *Eve_FilterGetParams(void) {
    return &Eve_Filter;
}

void Eve_PartialMultiFilterON(void) {
    Eve_PartialMultiFilterShow = 1;
}

void Eve_PartialMultiFilterOFF(void) {
    Eve_PartialMultiFilterShow = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam) {
    hEve_PartialMultiFilter->param = *pParam;
}

/* Return the current textured-square work without retaining it. */
dds3Effect_PartialMultiFilter_t *Eve_PartialMultiFilterGetParams(void) {
    return hEve_PartialMultiFilter;
}

void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *hTex) {
    hEve_PartialMultiFilter->hAlphaTex = hTex;
}

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->hAlphaTex = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)D_00320228);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)D_003201F8);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)D_00320270);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)D_00320298);
    Eve_RippleBlurGetParams()->params.number = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

/* Draw every screen effect whose show flag is set; the last one tail calls. */
void Eve_EffectDraw(void) {
    if (Eve_BlurShow != 0) {
        dds3EffectBlur(&Eve_Blur.source);
    }
    if (Eve_PartialMultiBlurShow != 0) {
        dds3EffectPartialMultiBlurDraw(hEve_PartialMultiBlur);
    }
    if (Eve_DistortionBlurShow != 0) {
        dds3EffectDistortionBlurDraw(hEve_DistortionBlur);
    }
    if (Eve_RippleBlurShow != 0) {
        dds3EffectRippleBlurDraw(hEve_RippleBlur);
    }
    if (Eve_FilterShow != 0) {
        dds3EffectFilter(&Eve_Filter);
    }
    if (Eve_PartialMultiFilterShow != 0) {
        dds3EffectPartialMultiFilterDraw(hEve_PartialMultiFilter);
    }
}

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
s32 func_001AD6E8(void) {
    u8 ready = D_0054F2C5;

    if (D_0054F2C5 == 0) {
        effDebugMenu_t *state = &D_003203E0;

        if (state->create != NULL) {
            *state->param = state->create(state->source);
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
        dblurUpdateHandle(&D_003203E0);
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
        effDebugMenu_t *state = &mblurDebugMenu;

        if (state->create != NULL) {
            *state->param = state->create(&D_00320420);
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
        dblurUpdateHandle(&mblurDebugMenu);
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
dds3Effect_PartialMultiBlurParam_t *mblurGetParam(void) {
    return &D_00320420;
}

/* Replace the setup template; this does not modify the active blur work. */
void mblurSetParam(dds3Effect_PartialMultiBlurParam_t *parameters) {
    D_00320420 = *parameters;
}

/* Prepare scatter setup from its explicit parameters, then process the channel. */
s32 dblurProcess(void) {
    u8 ready = dblurDone;

    if (dblurDone == 0) {
        effDebugMenu_t *state = &dblurDebugMenu;

        if (state->create != NULL) {
            *state->param = state->create(&D_00320598);
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
        dblurUpdateHandle(&dblurDebugMenu);
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
dds3Effect_DistortionBlurParam_t *dblurGetParam(void) {
    return &D_00320598;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void dblurSetParam(dds3Effect_DistortionBlurParam_t *parameters) {
    D_00320598 = *parameters;
}

/* Prepare solid-rectangle setup from the channel source, then process it. */
s32 evtViewerCheckDirty(void) {
    u8 ready = evtViewerDirty;

    if (evtViewerDirty == 0) {
        effDebugMenu_t *state = &filterDebugMenu;

        if (state->create != NULL) {
            *state->param = state->create(state->source);
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
        dblurUpdateHandle(&filterDebugMenu);
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
dds3Effect_Filter_t *dblurGetParam2(void) {
    return &D_00320770;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void dblurSetParam2(dds3Effect_Filter_t *parameters) {
    D_00320770 = *parameters;
}

/* Prepare resource-template setup from its explicit body, then process the channel. */
s32 mfilterRun(void) {
    u8 ready = mfilterDone;

    if (mfilterDone == 0) {
        effDebugMenu_t *state = &mfilterDebugMenu;

        if (state->create != NULL) {
            *state->param = state->create(&D_00320870);
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
        dblurUpdateHandle(&mfilterDebugMenu);
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
dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void) {
    return &D_00320870;
}

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    D_00320870 = *parameters;
}

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
s32 rblurProcess(void) {
    u8 ready = rblurDone;

    if (rblurDone == 0) {
        effDebugMenu_t *state = &rblurDebugMenu;

        if (state->create != NULL) {
            *state->param = state->create(&D_003209B0);
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
        dblurUpdateHandle(&rblurDebugMenu);
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
dds3Effect_RippleBlurParam_t *rblurGetParam(void) {
    return &D_003209B0;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void rblurSetParam(dds3Effect_RippleBlurParam_t *parameters) {
    D_003209B0 = *parameters;
}

effBTL_t *effEveBEDCreate(effBTL_t *pData) {
    return effBTLCreate(pData);
}

void effEveBEDDelete(effBTL_t *hEveBED) {
    effBTLDelete(hEveBED);
}

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCut_t *effEveBEDCutCreate(effBTL_t *hEveBED, u16 CutNum, const void *pParams) {
    const effEveBEDParams_t *init = (const effEveBEDParams_t *)pParams;
    effEveBEDCut_t *work = sdfChipAllocDirect(sizeof(effEveBEDCut_t));

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effBTLCutCreate(hEveBED, CutNum, 0, 0);
    effBTLCutSetScale(work->effect, init->scale);
    return work;
}

/* Release the attached effect before freeing the event work. */
void effEveBEDCutDelete(effEveBEDCut_t *hEveBEDCut) {
    effBTLCutDelete(hEveBEDCut->effect);
    sdfChipFreeDirect(hEveBEDCut);
}

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams) {
    *(effEveBEDRecord_t *)hEveBEDCut = *(const effEveBEDRecord_t *)pParams;
}

/* This copies a packed 0x30-byte record, not a complete 0x60-byte billboard state. */
void effEveBEDCutGetParams(const effEveBEDRecord_t *hEveBEDCut, effEveBEDRecord_t *pParams) {
    *pParams = *hEveBEDCut;
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEveBEDCutSetScale(effEveBEDCut_t *hEveBEDCut, f32 scale) {
    effBTLCutSetScale(hEveBEDCut->effect, scale);
}

void effEveBEDCutSetUnitHandle(effEveBEDCut_t *hEveBEDCut, void *hUnit) {
    hEveBEDCut->actor = hUnit;
}

/* NON_MATCHING: 2 of 119 words differ (first at +0xA8). */
#ifdef NON_MATCHING
void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut) {
    sdfVector4_t position;
    sdfMatrix4_t base;
    sdfMatrix4_t marked;
    effBTLCut_t *cut = hEveBEDCut->effect;
    u32 frame = cut->currentFrame;
    effBTL_SkillCutTable_t *list = cut->list;
    effBTL_SkillPartsList_t *entry;
    dds3EffectPart_t **works;
    sdfModelRootNode_t *root;
    sdfMatrix4_t *mp;
    u32 color;
    s32 remaining;
    u16 count;
    u8 kind;

    SDF_VU0_LOADVECTOR(hEveBEDCut->init.rot);
    SDF_VU0_LOADVECTOR_B(&D_005343D0);
    Mult();
    ToMatrix();
    SDF_VU0_STOREMATRIX(&base);
    entry = list->entries;
    count = list->count;
    color = hEveBEDCut->init.abgr;
    if (count != 0) {
        remaining = count;
        works = cut->parameterWorks;
        do {
            if (entry->timing < frame) {
                kind = entry->pos_ptr.scaleKind;
                if (works[0] != NULL) {
                    if (kind != 5 || hEveBEDCut->actor == NULL) {
                        effEveBEDPartGetPositionPtr(&hEveBEDCut->init, &entry->pos_ptr);
                        SDF_VU0_STOREVECTOR(&position);
                        dds3EffectPartSetPosition(works[0], &position);
                        dds3EffectPartSetRotateMatrix(works[0], &base);
                    } else {
                        root = hEveBEDCut->actor->owner->inner;
                        sdfModelGetMarkPos(root, entry->pos_ptr.aimMode);
                        SDF_VU0_STOREVECTOR(&position);
                        dds3EffectPartSetPosition(works[0], &position);
                        sdfModelGetMarkPosMatrix(root, entry->pos_ptr.aimMode);
                        SDF_VU0_LOADMATRIX_B(&base);
                        MultMatrixNoArg();
                        mp = &marked;
                        SDF_VU0_STOREMATRIX_RWADDR(mp);
                        dds3EffectPartSetRotateMatrix(works[0], &marked);
                    }
                    dds3EffectPartSetColor(works[0], color);
                    dds3EffectPartDraw(works[0]);
                }
            }
            remaining--;
            works++;
            entry++;
        } while (remaining != 0);
    }
    if (cut->triggerFrame == frame && cut->triggerFrameCallback != NULL) {
        cut->triggerFrameCallback(cut);
    }
    cut->currentFrame++;
}
#else
INCLUDE_ASM(const s32, "effect/effEvent", effEveBEDCutDraw);
#endif

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(effBTL_t *pData, f32 scale) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->pParams.rot[3] = 1.0f;
    work->pParams.r = 50.0f;
    work->pParams.h = 180.0f;
    work->pParams.center[1] = -90.0f;
    work->pParams.scale = scale;
    work->pParams.abgr = EFF_EVENT_NEUTRAL_COLOR;
    work->pParams.rot[0] = 0;
    work->pParams.rot[1] = 0;
    work->pParams.rot[2] = 0;
    work->pParams.center[0] = 0;
    work->pParams.center[2] = 0;
    work->pParams.center[3] = 0;
    work->hBED = effEveBEDCreate(pData);
    work->hCut = effEveBEDCutCreate(work->hBED, 0, &work->pParams);
    work->original = 1;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effSimpleBEDDelete(effSimpleBED_t *hSimpleBED) {
    effEveBEDCutDelete(hSimpleBED->hCut);
    if (hSimpleBED->original != 0) {
        effEveBEDDelete(hSimpleBED->hBED);
    }
    sdfChipFreeDirect(hSimpleBED);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *hSimpleBEDSrc) {
    effEveBEDParams_t *block = &hSimpleBEDSrc->pParams;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->hCut = effEveBEDCutCreate(hSimpleBEDSrc->hBED, 0, block);
    work->hBED = hSimpleBEDSrc->hBED;
    memcpy(&work->pParams, block, sizeof(work->pParams));
    work->original = 0;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effSimpleBEDDraw(effSimpleBED_t *hSimpleBED) {
    effEveBEDCutDraw(hSimpleBED->hCut);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *hSimpleBED, f32 *pPos) {
    hSimpleBED->pParams.center[0] = pPos[0];
    hSimpleBED->pParams.center[1] = pPos[1] - hSimpleBED->pParams.h * 0.5f;
    hSimpleBED->pParams.center[2] = pPos[2];
    hSimpleBED->pParams.center[3] = 0;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *hSimpleBED, u32 abgr) {
    hSimpleBED->pParams.abgr = abgr;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}

/* vu0 routine: place one part relative to the record's position and facing. */
void effEveBEDPartGetPositionPtr(effEveBEDParams_t *pParams, effBTLAimParams_t *pPosPtr) {
    sdfVector4_t out;
    sdfVector4_t offset;
    sdfVector4_t position;
    s32 range = pPosPtr->rangeOverride;
    u8 dir = pPosPtr->directionMode;
    u8 mode = pPosPtr->aimMode;
    f32 radius;
    f32 half;
    f32 y;
    f32 zero;

    if (range == 0) {
        radius = pParams->r;
    } else {
        radius = (f32)range;
    }
    half = pParams->h * 0.5f;
    SDF_VU0_COPYVECTOR(&position, pParams);
    if (mode == 5) {
        if (dir == 8 || dir == 0xA) {
            y = -1.0f;
            if (range != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = position.y - D_00320B88[mode] * half;
        if (dir == 8 || dir == 0xA) {
            if (range != 0) {
                y -= radius;
            }
        }
    }
    if (dir == 9 || mode == 4) {
        if (half < radius) {
            offset.z = -radius;
        } else {
            offset.z = -half;
        }
        zero = 0.0f;
        offset.y = zero;
        offset.x = zero;
        position.y = y;
        LookAtMatrix(&position, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&position);
        SDF_VU0_ADDVECTOR();
        return;
    }
    if (dir == 8 || dir == 0xA) {
        out.x = position.x;
        out.y = y;
        out.z = position.z;
    } else {
        offset.x = 0.0f;
        offset.z = 1.0f;
        offset.y = 0.0f;
        SDF_VU0_LOADVECTOR(pParams->rot);
        ToMatrix();
        SetRotateMatrixY_B(D_00320BA0[dir]);
        MultMatrixNoArg();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&offset);
        out.x = position.x + radius * offset.x;
        out.y = y + radius * offset.y;
        out.z = position.z + radius * offset.z;
    }
    SDF_VU0_LOADVECTOR(&out);
}

INCLUDE_RODATA(const s32, "effect/effEvent", D_005343D0);

effEVECombParSmoke_t *effEVECombParSmokeCreate(effEVECombParSmokeParam_t *pParam) {
    u32 particleCount = pParam->number;
    u32 particleBytes = particleCount * sizeof(effEVECombParSmokePart_t);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(particleBytes + sizeof(effEVECombParSmoke_t));
    effEVECombParSmokePart_t *particle = (effEVECombParSmokePart_t *)sdfMemLock(allocationHandle);
    effEVECombParSmoke_t *work = (effEVECombParSmoke_t *)((u8 *)particle + particleBytes);
    u32 particleIndex;

    work->head = *pParam;
    work->hMem = allocationHandle;
    work->pPart = particle;
    work->Ztest = 0;
    if (work->head.fade_in <= 0) {
        work->head.fade_in = 1;
    }
    if (work->head.fade_out <= 0) {
        work->head.fade_out = 1;
    }
    if (work->head.wait <= 0) {
        work->head.wait = 1;
    }
    EffEveCPSmokeCount = EffEveCPSmokeCount + 1;
    if (EffEveCPSmokeCount == 1) {
        hEffEveCPSmokeBill01Tex = (u32)dds3BillboardFileLoad(0, "/efftool/bill/dbball01.tmx");
        hEffEveCPSmokeBill02Tex = (u32)dds3BillboardFileLoad(0, "/efftool/bill/dbball02.tmx");
    }
    particleCount = work->head.number;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        particle->frame = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(effEVECombParSmoke_t *pCombParSmoke) {
    EffEveCPSmokeCount = EffEveCPSmokeCount - 1;
    if (EffEveCPSmokeCount == 0) {
        dds3BillboardDelete((dds3Billboard_t *)hEffEveCPSmokeBill01Tex);
        dds3BillboardDelete((dds3Billboard_t *)hEffEveCPSmokeBill02Tex);
    }
    sdfMemFreeDirect(pCombParSmoke->hMem);
}

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEVECombParSmokePartInit(effEVECombParSmoke_t *pCombParSmoke, s32 index) {
    effEVECombParSmokePart_t *particle = &pCombParSmoke->pPart[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = pCombParSmoke->head.wait;
    s32 motionDelaySpread = pCombParSmoke->head.motionDelaySpread;

    particle->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % startDelaySpread);
    particle->wait = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % motionDelaySpread);
    scaleFactor = dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.scaleRandomness + (1.0f - pCombParSmoke->head.scaleRandomness);
    particle->scale = pCombParSmoke->head.baseScale * scaleFactor;
    particle->s_w_r = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->s_h_r = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = pCombParSmoke->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scalePhaseStepY = pCombParSmoke->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = pCombParSmoke->head.scaleAmplitudeX * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = pCombParSmoke->head.scaleAmplitudeY * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->roll = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (dds3GetRandA((effRandState_t *)dds3EffectBase) & 1) {
        particle->roll_s = pCombParSmoke->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->roll_s = -(pCombParSmoke->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = pCombParSmoke->head.spawnRadius;
    direction[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->pos[0] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[0];
    particle->pos[1] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[1];
    particle->pos[2] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[2];
    particle->motion[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(particle->motion);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->motion);
    particle->amplitude = pCombParSmoke->head.swayAmplitude * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.swayRandomness + (1.0f - pCombParSmoke->head.swayRandomness));
    particle->angle = 0;
    particle->up_s = pCombParSmoke->head.verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.verticalSpeedRandomness + (1.0f - pCombParSmoke->head.verticalSpeedRandomness));
    particle->shift_s = pCombParSmoke->head.lateralSpeed * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.lateralSpeedRandomness + (1.0f - pCombParSmoke->head.lateralSpeedRandomness));
}

void eff_1AEC58(effEVECombParSmoke_t *work);

/* vu0 routine: advance and draw every live particle of one smoke set. */
void eff_1AEC58(effEVECombParSmoke_t *work) {
    f32 offset[EFF_EVENT_VECTOR_COMPONENTS];
    f32 origin[EFF_EVENT_VECTOR_COMPONENTS];
    f32 side[EFF_EVENT_VECTOR_COMPONENTS];
    effEVECombParSmokePart_t *particle;
    u8 repeat;
    u32 count;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    f32 growth;
    f32 swayPhaseStep;
    f32 swayAmplitudeStep;
    f32 drift;
    f32 wave;
    f32 ratio;
    f32 scaleX;
    f32 scaleY;
    u32 rgba;
    s32 age;
    s32 remaining;
    u32 i;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(side);
    growth = work->head.accelerationPercent / 100.0f + 1.0f;
    particle = work->pPart;
    count = work->head.number;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fade_in;
    fadeOut = work->head.fade_out;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    SDF_VU0_COPYVECTOR(origin, work);
    i = 0;
    dds3BillboardSetMode((dds3Billboard_t *)hEffEveCPSmokeBill01Tex, work->head.drawMode);
    dds3BillboardSetMode((dds3Billboard_t *)hEffEveCPSmokeBill02Tex, 3);
    for (; i < count; i++, particle++) {
        age = particle->frame;
        if (age == 0) {
            effEVECombParSmokePartInit(work, i);
        }
        if (age > 0 && age <= duration) {
            if (particle->wait >= 0) {
                drift = particle->shift_s;
                particle->pos[0] += side[0] * drift;
                particle->pos[1] += side[1] * drift;
                particle->pos[2] += side[2] * drift;
                wave = particle->amplitude * sdfSin(particle->angle);
                particle->motion[1] += particle->up_s;
                offset[0] = particle->pos[0] + particle->motion[0] * wave;
                offset[1] = particle->pos[1] + particle->motion[1];
                offset[2] = particle->pos[2] + particle->motion[2] * wave;
                particle->up_s *= growth;
                particle->shift_s *= growth;
                particle->amplitude += swayAmplitudeStep;
                particle->angle += swayPhaseStep;
            } else {
                particle->wait++;
                offset[0] = particle->pos[0];
                offset[1] = particle->pos[1];
                offset[2] = particle->pos[2];
            }
            if (age < fadeIn) {
                ratio = (f32)age / (f32)fadeIn;
            } else {
                remaining = duration - age;
                if (remaining <= fadeOut) {
                    ratio = (f32)remaining / (f32)fadeOut;
                } else {
                    ratio = 1.0f;
                }
            }
            rgba = effColorBlend(color & 0xFFFFFF, color, ratio);
            scaleY = particle->scale;
            scaleX = scaleY + particle->scaleAmplitudeX * sdfCos(particle->s_w_r);
            scaleY = scaleY + particle->scaleAmplitudeY * sdfCos(particle->s_h_r);
            SDF_VU0_LOADVECTOR_MEMOPS(offset[0]);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_MEMOPS(offset[0]);
            dds3BillboardSetPosition((dds3Billboard_t *)hEffEveCPSmokeBill01Tex, offset);
            dds3BillboardSetScale((dds3Billboard_t *)hEffEveCPSmokeBill01Tex, scaleX, scaleY);
            dds3BillboardSetRGBA((dds3Billboard_t *)hEffEveCPSmokeBill01Tex, rgba);
            dds3BillboardSetRotate((dds3Billboard_t *)hEffEveCPSmokeBill01Tex, particle->roll);
            dds3BillboardDraw((dds3Billboard_t *)hEffEveCPSmokeBill01Tex);
            dds3BillboardSetPosition((dds3Billboard_t *)hEffEveCPSmokeBill02Tex, offset);
            dds3BillboardSetScale((dds3Billboard_t *)hEffEveCPSmokeBill02Tex, scaleX, scaleY);
            dds3BillboardSetRGBA((dds3Billboard_t *)hEffEveCPSmokeBill02Tex, rgba);
            dds3BillboardSetRotate((dds3Billboard_t *)hEffEveCPSmokeBill02Tex, particle->roll);
            dds3BillboardDraw((dds3Billboard_t *)hEffEveCPSmokeBill02Tex);
            particle->s_w_r += particle->scalePhaseStepX;
            particle->s_h_r += particle->scalePhaseStepY;
            particle->roll += particle->roll_s;
        }
        if (duration == age && repeat != 0) {
            particle->frame = 0;
        } else {
            particle->frame++;
        }
    }
}

extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, u32 aPacket);
extern s32 dds3BillboardGetTex(void *hBillboard);
extern sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle);
extern s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);
extern void sdf3DMakeDirectRefPacket(void *aPacket, u32 aData, s32 aLeng);
extern void sdfBillBoardMakeTMPacket(void *aPacket, const f32 *aTM);
extern u32 *sdfBillBoardMakeN(const sdfVector4_t *aCenter, const void *aColor,
                              const void *aUVList, const void *aXYOffsetList, s32 aNum,
                              void *(*aAllocator)(s32));
/* The 15-quad batch sdfBillBoardMakeN takes as four parallel arrays, with the
 * texture coordinates and the corner offsets as flat rows. */
typedef struct effEveBillBatch_t {
    sdfVector4_t centre[15];
    u32 color[15];
    s16 uv[15][8];
    f32 corner[15][8];
    s32 count;
} effEveBillBatch_t;

extern sdfGrOt_t sdfGrOtTable[85];

PS2_SYMBOL_AT(sdfGrOt_t, effEveCPSmokeOt1, sdfGrOtTable, 0x600);

/* vu0 routine: advance every live particle of one smoke set and batch two
 * textured quads per particle into the two ordering tables. VIF consumes XYZ;
 * native leaves position.w untouched before its full-quadword add/copies. */
void effEVECombParSmokeDraw(effEVECombParSmoke_t *work) {
    effEveBillBatch_t primary, secondary;
    f32 matrix[4][4];
    f32 position[4], origin[4], lateral[4];
    sdfDrawTag_t *primaryList, *secondaryList;
    sdfGrOt_t *surface;
    sdfTexHandle_t *primaryTexture, *secondaryTexture;
    effEVECombParSmokePart_t *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut, colorStart, colorFrames;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;
    struct DmaPacketHeader *texturePacket;
    const sdfTexPacket_t *textureBuffer;
    s32 textureBytes;
    u8 *geometryPacket;
    sdfDrawTag_t *stateList;
    u64 *statePacket;
    primary.count = 0;
    secondary.count = 0;
    primaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(primaryList);
    secondaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(secondaryList);
    primaryTexture = (sdfTexHandle_t *)dds3BillboardGetTex((void *)hEffEveCPSmokeBill01Tex);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(primaryTexture);
    textureBytes = sdfTexGetPacketLength(primaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfDrawTagLinkRef(primaryList, (u32)texturePacket);
    secondaryTexture = (sdfTexHandle_t *)dds3BillboardGetTex((void *)hEffEveCPSmokeBill02Tex);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(secondaryTexture);
    textureBytes = sdfTexGetPacketLength(secondaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfDrawTagLinkRef(secondaryList, (u32)texturePacket);
    SDF_VU0_STOREIDENTITY(matrix);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(primaryList, (u32)geometryPacket);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(secondaryList, (u32)geometryPacket);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lateral);
    particle = work->pPart;
    particleCount = work->head.number;
    acceleration = work->head.accelerationPercent / 100.0f + 1.0f;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fade_in;
    fadeOut = work->head.fade_out;
    colorStart = work->head.colorStartAge;
    colorFrames = work->head.colorTransitionFrames;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    SDF_VU0_COPYVECTOR(origin, work->head.position);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->frame;
        if (age == 0) {
            effEVECombParSmokePartInit(work, index);
        }
        if (age > 0 && age <= duration) {
            f32 swaySine, sway, alpha, transition;
            f32 modulationX, scaleX, scaleY, phaseCosine;
            f32 halfWidth, halfHeight;
            u32 primaryColor, secondaryColor;
            s32 slot;
            s16 u, v;
            if (particle->wait >= 0) {
                f32 lateralSpeed = particle->shift_s;
                particle->pos[0] += lateral[0] * lateralSpeed;
                particle->pos[1] += lateral[1] * lateralSpeed;
                particle->pos[2] += lateral[2] * lateralSpeed;
                swaySine = sdfSin(particle->angle);
                sway = particle->amplitude * swaySine;
                particle->motion[1] += particle->up_s;
                position[0] = particle->pos[0] + particle->motion[0] * sway;
                position[1] = particle->pos[1] + particle->motion[1];
                position[2] = particle->pos[2] + particle->motion[2] * sway;
                particle->up_s *= acceleration;
                particle->shift_s *= acceleration;
                particle->amplitude += swayAmplitudeStep;
                particle->angle += swayPhaseStep;
            } else {
                particle->wait++;
                position[0] = particle->pos[0];
                position[1] = particle->pos[1];
                position[2] = particle->pos[2];
            }
            if (age < fadeIn) {
                alpha = (f32)age / (f32)fadeIn;
            } else if (duration - age <= fadeOut) {
                alpha = (f32)(duration - age) / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
            if (age < colorStart) {
                primaryColor = effColorBlend(0, color & 0xFF000000, alpha);
            } else if (age < colorStart + colorFrames) {
                if (colorFrames > 0) {
                    transition = (f32)(age - colorStart) / (f32)colorFrames;
                    if (1.0f < transition) transition = 1.0f;
                } else {
                    transition = 1.0f;
                }
                primaryColor = effColorBlend(color & 0xFF000000, color, transition);
                primaryColor = effColorBlend(primaryColor & 0xFFFFFF, primaryColor, alpha);
            } else {
                primaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            }
            secondaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            scaleY = particle->scale;
            phaseCosine = sdfCos(particle->s_w_r);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfCos(particle->s_h_r);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = primary.count;
            SDF_VU0_COPYVECTOR(&primary.centre[slot], position);
            u = (s16)((u16)primaryTexture->width << 4);
            v = (s16)((u16)primaryTexture->height << 4);
            primary.color[slot] = primaryColor;
            primary.uv[slot][0] = 0;
            primary.uv[slot][1] = 0;
            primary.uv[slot][2] = u;
            primary.uv[slot][3] = 0;
            primary.uv[slot][4] = u;
            primary.uv[slot][5] = v;
            primary.uv[slot][6] = 0;
            primary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfCos(particle->roll);
            sine = sdfSin(particle->roll);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            primary.corner[slot][0] = cornerX * cosine - cornerY * sine;
            primary.corner[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            primary.corner[slot][2] = cornerX * cosine - cornerY * sine;
            primary.corner[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            primary.corner[slot][4] = cornerX * cosine - cornerY * sine;
            primary.corner[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            primary.corner[slot][6] = cornerX * cosine - cornerY * sine;
            primary.corner[slot][7] = cornerX * sine + cornerY * cosine;
            primary.count++;
            }
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = secondary.count;
            SDF_VU0_COPYVECTOR(&secondary.centre[slot], position);
            u = (s16)((u16)secondaryTexture->width << 4);
            v = (s16)((u16)secondaryTexture->height << 4);
            secondary.color[slot] = secondaryColor;
            secondary.uv[slot][0] = 0;
            secondary.uv[slot][1] = 0;
            secondary.uv[slot][2] = u;
            secondary.uv[slot][3] = 0;
            secondary.uv[slot][4] = u;
            secondary.uv[slot][5] = v;
            secondary.uv[slot][6] = 0;
            secondary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfCos(particle->roll);
            sine = sdfSin(particle->roll);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            secondary.corner[slot][0] = cornerX * cosine - cornerY * sine;
            secondary.corner[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            secondary.corner[slot][2] = cornerX * cosine - cornerY * sine;
            secondary.corner[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            secondary.corner[slot][4] = cornerX * cosine - cornerY * sine;
            secondary.corner[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            secondary.corner[slot][6] = cornerX * cosine - cornerY * sine;
            secondary.corner[slot][7] = cornerX * sine + cornerY * cosine;
            secondary.count++;
            }
            if (primary.count == 15) {
                sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.centre,
                    primary.color, primary.uv, primary.corner[0], 15, 0));
                primary.count = 0;
            }
            if (secondary.count == 15) {
                sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.centre,
                    secondary.color, secondary.uv, secondary.corner[0], 15, 0));
                secondary.count = 0;
            }
            particle->s_w_r += particle->scalePhaseStepX;
            particle->s_h_r += particle->scalePhaseStepY;
            particle->roll += particle->roll_s;
        }
        if (duration == age && repeat) particle->frame = 0;
        else particle->frame++;
    }
    if (primary.count > 0) {
        sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.centre,
            primary.color, primary.uv, primary.corner[0], primary.count, 0));
    }
    if (secondary.count > 0) {
        sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.centre,
            secondary.color, secondary.uv, secondary.corner[0], secondary.count, 0));
    }
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    statePacket[0] = 3;
    statePacket[1] = 0x5000000310000000ULL;
    statePacket[2] = 0x1000000000008002ULL;
    statePacket[3] = 0xE;
    if (work->Ztest) statePacket[4] = 0x51801;
    else statePacket[4] = 0x31801;
    statePacket[5] = 0x47;
    statePacket[6] = 0x44;
    statePacket[7] = 0x42;
    sdfDrawTagLink(stateList, (u32)statePacket);
    effEveCPSmokeOt1.regFunc(&effEveCPSmokeOt1, stateList);
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    statePacket[0] = 3;
    statePacket[1] = 0x5000000310000000ULL;
    statePacket[2] = 0x1000000000008002ULL;
    statePacket[3] = 0xE;
    if (work->Ztest) statePacket[4] = 0x51801;
    else statePacket[4] = 0x31801;
    statePacket[5] = 0x47;
    statePacket[6] = 0x42;
    statePacket[7] = 0x42;
    sdfDrawTagLink(stateList, (u32)statePacket);
    surface = &sdfGrOtTable[49];
    surface->regFunc(surface, stateList);
    (surface - 1)->regFunc(surface - 1, primaryList);
    surface->regFunc(surface, secondaryList);
}


/* vu0 routine: copy one quadword of position between unaligned records. */
void effEVECombParSmokeSetPosition(void *pCombParSmoke, void *pPos) {
    SDF_VU0_COPYVECTOR(pCombParSmoke, pPos);
}

/* Set the native byte flag without normalizing it to a boolean. */
void effEVECombParSmokeSetZtest(effEVECombParSmoke_t *pCombParSmoke, u8 test) {
    pCombParSmoke->Ztest = test;
}

/* Copy the serialized emitter parameters without changing their values. */
void effEVECombParSmokeGetParams(const effEVECombParSmokeParam_t *pCombParSmoke, effEVECombParSmokeParam_t *pParam) {
    *pParam = *pCombParSmoke;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effEVECombParSmokeSetParams(effEVECombParSmokeParam_t *pCombParSmoke, const effEVECombParSmokeParam_t *pParam) {
    *pCombParSmoke = *pParam;
    if (pCombParSmoke->fade_in <= 0) {
        pCombParSmoke->fade_in = 1;
    }
    if (pCombParSmoke->fade_out <= 0) {
        pCombParSmoke->fade_out = 1;
    }
    if (pCombParSmoke->wait <= 0) {
        pCombParSmoke->wait = 1;
    }
}

/* Create from the installed default emitter block; the returned pointer is unused. */
void effEVECombParSmokeCreate2(void) {
    effEVECombParSmokeCreate(&EVE_ParSmokeParam);
}

effAPPoint4_tag *effAPPoint4Create(const effAPPoint4Param_t *pParams, u16 type, void *pData) {
    const effAPPoint4Param_t *head = pParams;
    u32 recordCount = head->count;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(recordCount * sizeof(effAPPoint4Part_tag) + sizeof(effAPPoint4_tag));
    effAPPoint4_tag *work = (effAPPoint4_tag *)sdfMemLock(allocation);
    effAPPoint4Part_tag *record = (effAPPoint4Part_tag *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->param = *head;
    work->hMem = allocation;
    work->pPart = record;
    if (work->param.spread <= 0) {
        work->param.spread = 1;
    }
    work->pBezier4P = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        delayModulus = work->param.spread;
        parameterTemplate = dds3EffectPartCreate(type, pData);
        record->hPart = parameterTemplate;
        record->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->hPart = dds3EffectPartCopy(parameterTemplate);
            record->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
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
effAPPoint4_tag *effAPPoint4Copy(effChanSource_t *source) {
    u32 recordCount = source->head.count;
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordCount * sizeof(effAPPoint4Part_tag) + sizeof(effAPPoint4_tag));
    effAPPoint4_tag *work = (effAPPoint4_tag *)sdfMemLock(allocationHandle);
    effAPPoint4Part_tag *recordCursor = (effAPPoint4Part_tag *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->param = source->head;
    work->hMem = allocationHandle;
    work->pPart = recordCursor;
    if (work->param.spread <= 0) {
        work->param.spread = 1;
    }
    work->pBezier4P = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->param.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->hPart = dds3EffectPartCopy(parameterTemplate);
            recordCursor->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effAPPoint4Delete(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    dds3Bezier4PDelete(hData->pBezier4P);
    recordCount = hData->param.count;
    recordCursor = hData->pPart;
    if (recordCount != 0) {
        do {
            dds3EffectPartDelete(recordCursor->hPart);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfMemFreeDirect(hData->hMem);
}

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effAPPoint4PartInit(effAPPoint4_tag *pAPPoint4, u32 index) {
    f32 jitterScale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 jitteredPoint[4];
    effAPPoint4Part_tag *record = pAPPoint4->pPart + index;
    effCubicBezierSlot_t *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    cursorStep = 1.0f / (f32)pAPPoint4->param.steps;
    bezierSlot = dds3Bezier4PGetPart(pAPPoint4->pBezier4P, index);
    bezierSlot->t = 0;
    bezierSlot->step = cursorStep;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale[0] = pAPPoint4->param.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[1]);
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
    jitterScale[0] = pAPPoint4->param.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[2]);
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
    jitterScale[0] = pAPPoint4->param.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
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
    jitterScale[0] = pAPPoint4->param.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint[0];
    bezierSlot->controlPoints[3][1] = jitteredPoint[1];
    bezierSlot->controlPoints[3][2] = jitteredPoint[2];
    dds3EffectPartSetColor(record->hPart, 0);
}

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void effAPPoint4Draw(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor = hData->pPart;
    effArrHdr_t *bezierSlots = hData->pBezier4P;
    u32 recordCount = hData->param.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = hData->param.enabled;
    s32 activeSteps = hData->param.steps;
    s32 delayModulus = hData->param.spread;
    s32 fadeInSteps = hData->param.fade_in;
    s32 fadeOutSteps = hData->param.fade_out;
    f32 position[3];

    (void)dds3Bezier4PGetPart(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->frame;

        if (recordTime == 0) {
            effAPPoint4PartInit(hData, recordIndex);
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
            dds3EffectPartSetColor(recordCursor->hPart, color);
            dds3EffectPartSetPosition(recordCursor->hPart, position);
            dds3EffectPartDraw(recordCursor->hPart);
        }
        if (recordTime < activeSteps) {
            recordCursor->frame++;
        } else if (cycleEnabled != 0) {
            recordCursor->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        } else {
            recordCursor->frame++;
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
void effEveAPP4Randomize(effEmit_t *hEffect) {
    effAPPoint4_tag *channelWork = hEffect->primitive;
    u32 stepModulus = channelWork->param.steps;
    u32 recordCount = channelWork->param.count;
    effAPPoint4Part_tag *recordCursor = channelWork->pPart;
    u32 recordIndex = 0;
    effCubicBezierSlot_t *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effAPPoint4PartInit(channelWork, recordIndex);
        recordCursor->frame = dds3GetRandA((effRandState_t *)dds3EffectBase) % stepModulus;
        bezierSlot = dds3Bezier4PGetPart(channelWork->pBezier4P, recordIndex);
        recordIndex++;
        randomStep = recordCursor->frame;
        cursorStep = bezierSlot->step;
        recordCursor->frame = randomStep + 1;
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

