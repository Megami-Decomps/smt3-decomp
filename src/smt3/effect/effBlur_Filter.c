#include "common.h"
#include "libsdf.h"
#include "effect.h"

extern sdfGrOt_t sdfGrOtTable[];


extern u64 *effPkt2DAllocF4(u32 flags);

extern u8 dds3EffectBase[];

void dds3EffectBlur(const effBlurQuad_t *source);

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *state);

extern void *memcpy(void *dest, const void *src, u32 n);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void *sdfChipAllocDirect(s32 size);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfChipFreeDirect(void *memory);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlur_AlphaMultiParmSet);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlur);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDrawFrameMask);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDrawAlphaMulti);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDrawFrameMaskReset);

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectBlurDraw);

void dds3EffectBlurSetRect(effBlurTemplateBody_t *work) {
    s32 x = work->source.position[0] + 0x100;
    s32 y = work->source.position[1] + 0xE0;
    s32 extent = work->extent;

    work->source.corners[0][0] = x - extent;
    work->source.corners[0][1] = y - extent;
    work->source.corners[1][0] = x + extent;
    work->source.corners[1][1] = y + extent;
    dds3EffectBlur(&work->source);
}

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const effBlurTemplateBody_t *src) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, src, sizeof(*src));
    return dst;
}

void dds3EffectPartialMultiBlurDelete(effBlurTemplate_t *owner) {
    sdfChipFreeDirect(owner);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectPartialMultiBlurDraw);

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurScatterWork_t *dst, effBlurScatterParams_t *src) {
    dst->params = *src;
}

/* Supply a source handle for the first blur variant. */
void dds3EffectPartialMultiBlurSetTex(effBlurScatterWork_t *work, struct sdfTexHandle_t *sourceHandle) {
    work->texture = sourceHandle;
}

/* Acquire the first variant's source handle from the effect manager. */
void dds3EffectPartialMultiBlurSetTexDef(effBlurScatterWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *work, effBlurScatterSlot_t *slot) {
    effBlurQuad_t *quad = &slot->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    slot->delay = dds3GetRandA((u8 *)dds3EffectBase) % (work->params.delaySpread + 1);
    slot->angle = -3.14159265f;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    spread = work->params.positionSpread;
    halfSize = work->params.size;
    quad->position[0] = work->params.position[0] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->position[1] = work->params.position[1] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    centerX = quad->position[0] + 256;
    quad->corners[0][0] = centerX - halfSize;
    quad->corners[1][0] = centerX + halfSize;
    centerY = quad->position[1] + 224;
    quad->corners[0][1] = centerY - halfSize;
    quad->corners[1][1] = centerY + halfSize;
}

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(effBlurScatterParams_t *params)
{
    struct sdfMemHandle_t *allocation;
    effBlurScatterWork_t *work;
    effBlurScatterSlot_t *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(effBlurScatterWork_t) + 100 * sizeof(effBlurScatterSlot_t));
    work = (effBlurScatterWork_t *)sdfMemLock(allocation);
    work->params = *params;
    work->allocation = allocation;
    work->slots = (effBlurScatterSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(2);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 100; slotIndex++, slot++) {
        dds3EffectDistBlurPartInit(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

/* Release the first variant's owned effect resource. */
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *work) {
    sdfMemFreeDirect(work->allocation);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectDistortionBlurDraw);

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *dst, effBlurScaleParams_t *src) {
    u32 count = dst->params.count;

    dst->params = *src;
    dst->params.count = count;
}

/* The scale variant has its own source-handle setter. */
void func_001A2668(effBlurScaleWork_t *work, struct sdfTexHandle_t *sourceHandle) {
    work->texture = sourceHandle;
}

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void dds3EffectDistortionBlurSetTex(effBlurScaleWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectRipBlurPartAreaSet);

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *work, effBlurScaleSlot_t *slot) {
    effBlurQuad_t *quad = &slot->quad;

    slot->phase = 0.0f;
    slot->angle = 0.0f;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->position[0] = work->params.position[0];
    quad->position[1] = work->params.position[1];
}

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *work) {
    effBlurScaleSlot_t *slot = work->slots;
    s32 count = work->params.count;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.spacing * (f32)i);
    }
}

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(effBlurScaleParams_t *src) {
    s32 count = src->count;
    void *allocation = sdfMemAllocDirect(count * 0x30 + 0x38);
    effBlurScaleWork_t *work = (effBlurScaleWork_t *)sdfMemLock(allocation);
    effBlurScaleSlot_t *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, src, 0x2C);
    work->allocation = allocation;
    work->slots = (effBlurScaleSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(3);
    slot = work->slots;
    while (i < count) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.spacing * (f32)i);
        i++;
        slot++;
    }
    return work;
}

/* Release the scale variant's allocation, including its inline slot array. */
void dds3EffectRippleBlurDelete(effBlurScaleWork_t *work) {
    sdfMemFreeDirect(work->allocation);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectRippleBlurDraw);

/* Queue blend state and a uniformly colored GS quad from two source corners. */
void dds3EffectFilter(effSolidRectParams_t *source) {
    sdfDrawTag_t *list;
    sdfGrOt_t *ot;
    u64 *blendPacket;
    u64 *drawPacket;
    u64 color;
    u32 topLeft;
    u32 bottomLeft;
    u32 topRight;
    u32 bottomRight;
    s32 x[4];
    s32 y[4];

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = source->blendControl;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);

    drawPacket = effPkt2DAllocF4(0);
    color = source->color | 0x3F80000000000000ULL;
    x[0] = (source->corners[0][0] << 4) + 0x7000;
    x[1] = (source->corners[1][0] << 4) + 0x7000;
    y[0] = (source->corners[0][1] << 3) + 0x7900;
    y[1] = (source->corners[1][1] << 3) + 0x7900;
    topLeft = (u16)x[0] | ((u32)(u16)y[0] << 16);
    bottomLeft = (u16)x[0] | ((u32)(u16)y[1] << 16);
    topRight = (u16)x[1] | ((u32)(u16)y[0] << 16);
    bottomRight = (u16)x[1] | ((u32)(u16)y[1] << 16);
    drawPacket[5] = color;
    drawPacket[7] = color;
    drawPacket[9] = color;
    drawPacket[11] = color;
    drawPacket[6] = 0xFF000000000000ULL | topLeft;
    drawPacket[8] = 0xFF000000000000ULL | bottomLeft;
    drawPacket[10] = 0xFF000000000000ULL | topRight;
    drawPacket[12] = 0xFF000000000000ULL | bottomRight;
    sdfDrawTagLink(list, (u32)drawPacket);
    ot = &sdfGrOtTable[57];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))ot->regFunc)(ot, list);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectFilterDrawTexQuad);

/* Clone rectangle parameters and select a fresh source handle. */
effResourceRectWork_t *dds3EffectPartialMultiFilterCreate(effResourceRectParams_t *src) {
    effResourceRectWork_t *dst = sdfChipAllocDirect(sizeof(effResourceRectWork_t));

    dst->texture = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->params, src, sizeof(*src));
    return dst;
}

void dds3EffectPartialMultiFilterDelete(effResourceRectWork_t *work) {
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectPartialMultiFilterDraw);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00533E30);

INCLUDE_RODATA(const s32, "effect/effBlur_Filter", D_00533E38);

