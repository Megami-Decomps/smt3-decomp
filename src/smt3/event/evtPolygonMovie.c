#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "effect.h"
#include "ps2asm.h"


void *dds3GetProcessData(dds3ProcessID_t *task);

/* SMT3 allocates and clears 0xD4 bytes; the process flag helpers use its head. */
typedef struct evtPolygonMovieWork_t {
    u32 movieFlags;
    u8 unk04[0xD0];
} evtPolygonMovieWork_t;

extern void *sdfChipAllocDirect(u32 size);
extern void *memset(void *destination, s32 value, u32 size);

u32 evtHokanColor(s32 enable, f32 t, u32 a, u32 b);

typedef struct evtBlendVecColor_t {
    f32 vec[4];
    u32 color;
} evtBlendVecColor_t;

extern f32 evtHokanFogParam[4];

extern u32 D_0032C830[4];


extern effBlurQuad_t D_0032C840;


extern effBlurTemplateBody_t D_0032C870;


extern effSolidRectParams_t D_0032C8A0;


extern effResourceRectParams_t D_0032C8C0;


extern effBlurScatterParams_t D_0032C8F0;


extern effBlurScaleParams_t D_0032C920;

typedef struct polyMovieClip_t {
    u8 opaque_00[8];
    f32 duration;
    f32 position;
} polyMovieClip_t;

typedef struct polyMovieState_t {
    u8 opaque_00[4];
    polyMovieClip_t *clip;
} polyMovieState_t;

typedef struct polyMovieObject_t {
    u8 opaque_00[0x18];
    polyMovieState_t *state;
} polyMovieObject_t;

s32 evtStopMover(polyMovieObject_t *movie, s32 undo, s32 start, s32 end);

typedef struct evtScaledValue_t {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} evtScaledValue_t;

void dds3PathBasicControlSetRatio(evtScaledValue_t *value, f32 multiplier);

void dds3PathBasicPauseOn(evtScaledValue_t *value);

void dds3PathBasicPauseOff(evtScaledValue_t *value);

extern f32 D_0032C7F0[4];

extern f32 D_0032C800[4];

extern f32 D_0032C810[4];

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

typedef struct pmdEntry_t {
    u32 type;
    u32 elementSize;
    u32 value;
    u32 offset;
} pmdEntry_t;

typedef struct pmdHeader_t {
    u8 pad[0x10];
    s32 count;
    s32 kind;
    u8 pad2[8];
    pmdEntry_t entries[1];
} pmdHeader_t;

sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out);

extern void *memcpy(void *dst, const void *src, u32 size);

static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, unit);
}

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void SetBlendVecW(f32 w)
{
    SDF_VU0_SETVECTOR_W(w);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", UnitFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", CameraFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", QuakeFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", FadeFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SeFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", BgmFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", BlurFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", AddEffect);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", AddEffectTable);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", AddEffectTableMG12);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", DelEffectTable_evtPolygonMovie);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", EffectFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SearchBasicFromNameIndex_evtPolygonMovie);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", MG12FrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", BEDFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", LightFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SkyFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", BokasiFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", GetKeyFrameRatio);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SLightFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SFogFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", Blur2FrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", MBlurFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", DBlurFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", RBlurFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", MFilterFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", FilterFrameFunc);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", FrameRun);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtPolygonMovieProcess2);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", CheckLoad_evtPolygonMovie);

s32 evtCheckPolygonMovieRun(dds3ProcessID_t *task)
{
    evtPolygonMovieWork_t *state;
    s32 set;

    state = (evtPolygonMovieWork_t *)dds3GetProcessData(task);
    set = 0;
    if (state->movieFlags & 8) {
        set = 1;
    }
    return set;
}

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BAF8);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BB20);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtPolygonMovieProcess1);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtPolygonMovieShutdown);

f32 evtHokanFloat(s32 enable, f32 t, f32 a, f32 b)
{
    if (enable) {
        return a * (1.0f - t) + b * t;
    }
    return a;
}

s32 evtHokanInt(s32 enable, f32 t, s32 a, s32 b)
{
    s32 result = a;

    if (enable) {
        result = (s32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return result;
}

u32 evtHokanUInt(s32 enable, f32 t, u32 a, u32 b)
{
    if (enable) {
        return (u32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return a;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanColor);

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtHokanSceneLight(s32 enable, f32 t, f32 *a, f32 *b, f32 *out)
{
    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(out);
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a + 4);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b + 4);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(out + 4);
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a + 8);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b + 8);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(out + 8);
}

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 enable, f32 t, evtBlendVecColor_t *a, evtBlendVecColor_t *b, evtBlendVecColor_t *out)
{

    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        SDF_VU0_SETVECTOR_X(a->vec[0]);
        SDF_VU0_SETVECTOR_Y(a->vec[1]);
        SDF_VU0_SETVECTOR_Z(a->vec[2]);
        SetBlendVecW(a->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - t);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (b != NULL) {
        SDF_VU0_SETVECTOR_X(b->vec[0]);
        SDF_VU0_SETVECTOR_Y(b->vec[1]);
        SDF_VU0_SETVECTOR_Z(b->vec[2]);
        SetBlendVecW(b->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(out->vec[0]);
    SDF_VU0_GETVECTOR_Y(out->vec[1]);
    SDF_VU0_GETVECTOR_Z(out->vec[2]);
    SDF_VU0_GETVECTOR_W(out->vec[3]);
    if (a != NULL) {
        UnpackRGBAWordFloat(a->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - t);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (b != NULL) {
        UnpackRGBAWordFloat(b->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(t);
    SDF_VU0_ADDVECTOR();
    out->color = PackRGBAWord(128.0f);
}

void evtHokanBlur2(s32 enable, f32 t, effBlurQuad_t *a, effBlurQuad_t *b, effBlurQuad_t *out)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C840;
    }
    if (b == NULL) {
        b = &D_0032C840;
    }
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->angle = evtHokanFloat(enable, t, a->angle, b->angle);
    out->displacement = evtHokanFloat(enable, t, a->displacement, b->displacement);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        out->position[positionOrCornerIndex] = evtHokanInt(enable, t, a->position[positionOrCornerIndex], b->position[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->corners[positionOrCornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->corners[positionOrCornerIndex][coordinateIndex], b->corners[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanMultBlur(s32 enable, f32 t, effBlurTemplateBody_t *a, effBlurTemplateBody_t *b, effBlurTemplateBody_t *out)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C870;
    }
    if (b == NULL) {
        b = &D_0032C870;
    }
    out->extent = evtHokanUInt(enable, t, a->extent, b->extent);
    out->source.color = evtHokanColor(enable, t, a->source.color, b->source.color);
    out->source.blendControl = a->source.blendControl;
    out->source.angle = evtHokanFloat(enable, t, a->source.angle, b->source.angle);
    out->source.displacement = evtHokanFloat(enable, t, a->source.displacement, b->source.displacement);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        out->source.position[positionOrCornerIndex] = evtHokanInt(enable, t, a->source.position[positionOrCornerIndex], b->source.position[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->source.corners[positionOrCornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->source.corners[positionOrCornerIndex][coordinateIndex], b->source.corners[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanFilter(s32 enable, f32 t, effSolidRectParams_t *a, effSolidRectParams_t *b, effSolidRectParams_t *out);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanFilter);

void evtHokanMultFilter(s32 enable, f32 t, effResourceRectParams_t *a, effResourceRectParams_t *b, effResourceRectParams_t *out)
{
    s32 centerCoordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C8C0;
    }
    if (b == NULL) {
        b = &D_0032C8C0;
    }
    out->extent = evtHokanUInt(enable, t, a->extent, b->extent);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        out->center[centerCoordinateIndex] = evtHokanInt(
            enable, t, a->center[centerCoordinateIndex], b->center[centerCoordinateIndex]);
    }
    out->draw.rgba = evtHokanColor(enable, t, a->draw.rgba, b->draw.rgba);
    out->draw.blendControl = a->draw.blendControl;
}

void evtHokanDistBlur(s32 enable, f32 t, effBlurScatterParams_t *a, effBlurScatterParams_t *b, effBlurScatterParams_t *out)
{
    s32 positionAxisIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C8F0;
    }
    if (b == NULL) {
        b = &D_0032C8F0;
    }
    out->count = evtHokanUInt(enable, t, a->count, b->count);
    out->delaySpread = evtHokanUInt(enable, t, a->delaySpread, b->delaySpread);
    out->angleStep = evtHokanFloat(enable, t, a->angleStep, b->angleStep);
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtHokanFloat(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtHokanFloat(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->positionSpread = evtHokanUInt(enable, t, a->positionSpread, b->positionSpread);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        out->position[positionAxisIndex] = evtHokanInt(enable, t, a->position[positionAxisIndex], b->position[positionAxisIndex]);
    }
    out->size = evtHokanUInt(enable, t, a->size, b->size);
}

void evtHokanRipBlur(s32 enable, f32 t, effBlurScaleParams_t *a, effBlurScaleParams_t *b, effBlurScaleParams_t *out)
{
    s32 positionAxisIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C920;
    }
    if (b == NULL) {
        b = &D_0032C920;
    }
    out->count = evtHokanUInt(enable, t, a->count, b->count);
    out->phaseStep = evtHokanFloat(enable, t, a->phaseStep, b->phaseStep);
    out->spacing = evtHokanFloat(enable, t, a->spacing, b->spacing);
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtHokanFloat(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtHokanFloat(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->angleStep = evtHokanFloat(enable, t, a->angleStep, b->angleStep);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        out->position[positionAxisIndex] = evtHokanInt(enable, t, a->position[positionAxisIndex], b->position[positionAxisIndex]);
    }
    out->size = evtHokanUInt(enable, t, a->size, b->size);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtRunUnitMode);

void evtSetPolygonMovieFlag(dds3ProcessID_t *process, s32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    work->movieFlags |= mask;
}

void evtResetPolygonMovieFlag(dds3ProcessID_t *process, s32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    work->movieFlags &= ~mask;
}

u8 evtCheckPolygonMovieFlag(dds3ProcessID_t *process, u32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    return (work->movieFlags & mask) != 0;
}

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtStopMover(polyMovieObject_t *movie, s32 undo, s32 startFrame, s32 endFrame)
{
    polyMovieClip_t *clip;
    s32 elapsedFrames;
    s32 duration;

    elapsedFrames = endFrame - startFrame;
    if (elapsedFrames < 0) {
        elapsedFrames = 0;
    }
    clip = movie->state->clip;
    if (clip != NULL) {
        duration = (s32)clip->duration;
        if (duration <= elapsedFrames) {
            elapsedFrames = duration;
        }
        dds3PathBasicControlSetRatio((evtScaledValue_t *)clip, (f32)elapsedFrames / (f32)duration);
        if (undo == 0) {
            dds3PathBasicPauseOn((evtScaledValue_t *)clip);
        } else {
            dds3PathBasicPauseOff((evtScaledValue_t *)clip);
        }
    }
}

evtPolygonMovieWork_t *InitPolygonMovieWorkData(void)
{
    evtPolygonMovieWork_t *work;

    work = sdfChipAllocDirect(sizeof(evtPolygonMovieWork_t));
    if (work == NULL) {
        return work;
    }
    memset(work, 0, sizeof(evtPolygonMovieWork_t));
    return work;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData1);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtFreePolygonMovieWorkData);

/* Allocate a resource block holding a fresh "PMD2" header; returns the handle. */
sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out)
{
    s32 header[16] = {0, 0, 0x32444D50, 0, 1, 4, 0, 0, 0, 0x10, 1, 0x30, 0, 999, 1000, 0};
    s32 headerBytes;
    sdfMemHandle_t *handle;
    void *block;

    headerBytes = 0x40;
    handle = sdfMemAllocDirect(headerBytes);
    block = (void *)sdfMemLock(handle);
    memcpy(block, header, headerBytes);
    *out = block;
    return handle;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", SetPMFileName);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData2);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtStartPolygonMovie2);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", evtCameraFrameState);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", nowfovy);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", D_0054F8E0);

