#include "common.h"
#include "libsdf.h"
#include "effect.h"

extern sdfGrOt_t sdfGrOtTable[];


extern u64 *effPkt2DAllocF4(u32 prim);

extern u8 dds3EffectBase[];

void dds3EffectBlur(const dds3Effect_Blur_t *source);

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *name);

extern void *memcpy(void *a, const void *b, u32 c);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void *sdfChipAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, u32 aPacket);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

extern u32 dds3GetFrameID(void);

extern u32 dds3NullDrawProc(void);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, u32 aPacket);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, u32 aEnv, void *aPkt);

extern void sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern f32 sdfSin(f32 aRadius);

extern f64 fptodp(f32 value) __attribute__((const));

extern long dpcmp(f64 left, f64 right) __attribute__((const));

extern f64 dpsub(f64 left, f64 right) __attribute__((const));

extern f64 dpmul(f64 left, f64 right) __attribute__((const));

extern u32 dptoul(f64 value) __attribute__((const));


extern sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim);

extern void *effPkt2DGetPrimPtr(void *pPkt);

/* The frame buffers the blur samples from; only the mask source is named. */
typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

extern effBlurDrawBank_t dds3DrawEnv[];

/* Nine A+D registers per tri-strip: RGBAQ once, then ST and XYZ2 per vertex. */




void dds3EffectBlurDrawQuad(const dds3Effect_Blur_t *pBlur, effBlurPrimTriStrip_t *pPrim);

void dds3EffectBlurDrawFrameMask(sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *texture);

void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source);

void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list);

void dds3EffectBlurDraw(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture);

void dds3EffectFilterDrawTexQuad(effResourceRectDrawParams_t *source, sdfTexHandle_t *texture);

/* Build the alpha-multiply tri-strip: a flat grey quad over the blur rect. */
void dds3EffectBlur_AlphaMultiParmSet(const dds3Effect_Blur_t *pBlur, effBlurPrimTriStrip_t *pSpriteParm) {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    pSpriteParm->r = 128;
    pSpriteParm->g = 128;
    pSpriteParm->b = 128;
    pSpriteParm->a = pBlur->abgr >> 24;
    x0 = (pBlur->xy[0][0] << 4) + 0x7000;
    pSpriteParm->vtx[0].x = x0;
    y0 = (pBlur->xy[0][1] << 3) + 0x7900;
    pSpriteParm->vtx[0].y = y0;
    pSpriteParm->vtx[1].x = x0;
    y1 = (pBlur->xy[1][1] << 3) + 0x7900;
    pSpriteParm->vtx[1].y = y1;
    x1 = (pBlur->xy[1][0] << 4) + 0x7000;
    pSpriteParm->vtx[2].x = x1;
    pSpriteParm->vtx[2].y = y0;
    pSpriteParm->vtx[3].x = x1;
    pSpriteParm->vtx[3].y = y1;
    pSpriteParm->vtx[0].s = 0.0f;
    pSpriteParm->vtx[0].t = 0.0f;
    pSpriteParm->vtx[1].s = 0.0f;
    pSpriteParm->vtx[1].t = 1.0f;
    pSpriteParm->vtx[2].s = 1.0f;
    pSpriteParm->vtx[2].t = 0.0f;
    pSpriteParm->vtx[3].s = 1.0f;
    pSpriteParm->vtx[3].t = 1.0f;
    pSpriteParm->vtx[0].z = 0;
    pSpriteParm->vtx[0].adc = 0;
    pSpriteParm->vtx[1].z = 0;
    pSpriteParm->vtx[1].adc = 0;
    pSpriteParm->vtx[2].z = 0;
    pSpriteParm->vtx[2].adc = 0;
    pSpriteParm->vtx[3].z = 0;
    pSpriteParm->vtx[3].adc = 0;
}

/* Borrow the display frame as a texture, set the two blend states and the
   sampling window, then draw the source quad through it. */
void dds3EffectBlur(const dds3Effect_Blur_t *source) {
    sdfDrawTag_t *list;
    void *reference;
    u32 frame;
    sdfGrPacketHeader_t *prim;
    sdfGrOt_t *ot;

    if (dds3NullDrawProc() != 0) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    reference = (void *)sdfGrAllocPacketBuff(0x20);
    frame = dds3GetFrameID();
    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[frame].frameMaskSource, reference);

    {
        u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x40);

        packet[0] = 3;
        packet[1] = 0x5000000310000000ULL;
        packet[2] = 0x1000000000008002ULL;
        packet[3] = 0xE;
        packet[4] = 0x8000000080ULL;
        packet[5] = 0x3B;
        packet[6] = 0;
        packet[7] = 0x3F;
        sdfDrawTagLink(list, (u32)packet);
    }

    {
        u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x40);

        packet[0] = 3;
        packet[1] = 0x5000000310000000ULL;
        packet[2] = 0x1000000000008002ULL;
        packet[3] = 0xE;
        packet[4] = 0x31001;
        packet[5] = 0x47;
        packet[6] = source->blend;
        packet[7] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
    }

    {
        u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x30);

        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x37C00000009ULL;
        packet[5] = 8;
        sdfDrawTagLink(list, (u32)packet);
    }

    prim = effPkt2DAllocTriStrip(1, 0);
    dds3EffectBlurDrawQuad(source, effPkt2DGetPrimPtr(prim));
    sdfDrawTagLink(list, (u32)prim);
    ot = &sdfGrOtTable[58];
    ot->regFunc(ot, list);
}

/* Point the sampler at this frame's mask, queue the blend state the blur
   needs, then hand the display frame back as the second source. */
void dds3EffectBlurDrawFrameMask(sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *texture) {
    u64 *packet;
    void *mask;
    void *reference;
    u32 frame;

    mask = (void *)sdfGrAllocPacketBuff(0x40);
    frame = dds3GetFrameID();
    sdfGrSetFrameMaskRGB(dds3DrawEnv[frame].frameMaskSource, mask, 1);
    sdfDrawTagLinkRefGif(list, (u32)mask);

    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 0xE;
    packet[4] = 0x31001;
    packet[5] = 0x48;
    packet[6] = 0x44;
    packet[7] = 0x43;
    sdfDrawTagLink(list, (u32)packet);
    sdf2DLinkTexPacket(list, texture, 1);

    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 0xE;
    packet[4] = 0x31001;
    packet[5] = 0x47;
    packet[6] = blendControl | 0x10;
    packet[7] = 0x42;
    sdfDrawTagLink(list, (u32)packet);

    reference = (void *)sdfGrAllocPacketBuff(0x20);
    frame = dds3GetFrameID();
    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[frame].frameMaskSource, reference);
    {
        u64 *last = (u64 *)sdfGrAllocPacketBuff(0x40);

        last[0] = 3;
        last[1] = 0x5000000310000000ULL;
        last[2] = 0x1000000000008002ULL;
        last[3] = 0xE;
        last[4] = 0x8000000080ULL;
        last[5] = 0x3B;
        last[6] = 0;
        last[7] = 0x3F;
        sdfDrawTagLink(list, (u32)last);
    }
    {
        u64 *last = (u64 *)sdfGrAllocPacketBuff(0x30);

        last[0] = 2;
        last[1] = 0x5000000210000000ULL;
        last[2] = 0x1000000000008001ULL;
        last[3] = 0xE;
        last[4] = 0x37C00000009ULL;
        last[5] = 8;
        sdfDrawTagLink(list, (u32)last);
    }
}

/* Two tri-strips: the alpha-multiply pass, then the blurred source quad. */
void dds3EffectBlurDrawAlphaMulti(sdfDrawTag_t *list, const dds3Effect_Blur_t *source) {
    sdfGrPacketHeader_t *packet;

    packet = effPkt2DAllocTriStrip(1, 0x200);
    dds3EffectBlur_AlphaMultiParmSet(source, effPkt2DGetPrimPtr(packet));
    sdfDrawTagLink(list, (u32)packet);
    packet = effPkt2DAllocTriStrip(1, 0);
    dds3EffectBlurDrawQuad(source, effPkt2DGetPrimPtr(packet));
    sdfDrawTagLink(list, (u32)packet);
}

/* Restore the frame mask this frame's blur borrowed, then queue the list. */
void dds3EffectBlurDrawFrameMaskReset(sdfDrawTag_t *list) {
    void *packet;
    u32 frame;
    sdfGrOt_t *ot;

    packet = (void *)sdfGrAllocPacketBuff(0x40);
    frame = dds3GetFrameID();
    sdfGrSetFrameMaskReset(dds3DrawEnv[frame].frameMaskSource, packet, 1);
    sdfDrawTagLinkRefGif(list, (u32)packet);
    ot = &sdfGrOtTable[58];
    ot->regFunc(ot, list);
}

void dds3EffectBlurDraw(const dds3Effect_Blur_t *source, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;

    if (dds3NullDrawProc() != 0) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    dds3EffectBlurDrawFrameMask(list, source->blend, texture);
    dds3EffectBlurDrawAlphaMulti(list, source);
    dds3EffectBlurDrawFrameMaskReset(list);
}

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work) {
    s32 x = work->Blur.center[0] + 0x100;
    s32 y = work->Blur.center[1] + 0xE0;
    s32 extent = work->area;

    work->Blur.xy[0][0] = x - extent;
    work->Blur.xy[0][1] = y - extent;
    work->Blur.xy[1][0] = x + extent;
    work->Blur.xy[1][1] = y + extent;
    dds3EffectBlur(&work->Blur);
}

/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, pParam, sizeof(*pParam));
    return dst;
}

void dds3EffectPartialMultiBlurDelete(effBlurTemplate_t *hPartialMultiBlur) {
    sdfChipFreeDirect(hPartialMultiBlur);
}

void dds3EffectPartialMultiBlurDraw(effBlurTemplate_t *work) {
    s32 x;
    s32 y;
    s32 extent;

    if (dds3NullDrawProc() != 0) {
        return;
    }
    x = work->body.Blur.center[0] + 0x100;
    y = work->body.Blur.center[1] + 0xE0;
    extent = work->body.area;
    work->body.Blur.xy[0][0] = x - extent;
    work->body.Blur.xy[0][1] = y - extent;
    work->body.Blur.xy[1][0] = x + extent;
    work->body.Blur.xy[1][1] = y + extent;
    dds3EffectBlurDraw(&work->body.Blur, work->texture);
}

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *hPartialMultiBlur, const dds3Effect_PartialMultiBlurParam_t *pParam) {
    hPartialMultiBlur->body = *pParam;
}

/* Supply a source handle for the first blur variant. */
void dds3EffectPartialMultiBlurSetTex(effBlurScatterWork_t *hPartialMultiBlur, struct sdfTexHandle_t *hTex) {
    hPartialMultiBlur->texture = hTex;
}

/* Acquire the first variant's source handle from the effect manager. */
void dds3EffectPartialMultiBlurSetTexDef(effBlurScatterWork_t *hPartialMultiBlur) {
    hPartialMultiBlur->texture = dds3BillboardGetGeneralTex(2);
}

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *pParam, effBlurScatterSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    pPart->delay = dds3GetRandA((u8 *)dds3EffectBase) % (pParam->params.interval + 1);
    pPart->angle = -3.14159265f;
    quad->rotate = pParam->params.rotate;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    spread = pParam->params.area;
    halfSize = pParam->params.width;
    quad->center[0] = pParam->params.center[0] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->center[1] = pParam->params.center[1] +
        (s32)(spread * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f));
    centerX = quad->center[0] + 256;
    quad->xy[0][0] = centerX - halfSize;
    quad->xy[1][0] = centerX + halfSize;
    centerY = quad->center[1] + 224;
    quad->xy[0][1] = centerY - halfSize;
    quad->xy[1][1] = centerY + halfSize;
}

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam)
{
    struct sdfMemHandle_t *allocation;
    effBlurScatterWork_t *work;
    effBlurScatterSlot_t *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(effBlurScatterWork_t) + 100 * sizeof(effBlurScatterSlot_t));
    work = (effBlurScatterWork_t *)sdfMemLock(allocation);
    work->params = *pParam;
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
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *hDistortionBlur) {
    sdfMemFreeDirect(hDistortionBlur->allocation);
}

/* Each live slot waits out its delay, then fades as its angle leaves zero.
 * Adapted from the matched DDS/redthing1 effBlurStepScatterSlotsAndDraw.
 * Keep SMT3's two-argument renderer and canonical soft-double helpers:
 * dpcmp compares doubles; the pi and reciprocal literals are promoted floats. */
void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work)
{
    struct sdfDrawTag_t *list;
    effBlurScatterSlot_t *slot;
    s32 count;
    u32 alpha;
    f64 level;
    f64 angle;

    if (dds3NullDrawProc() == 0) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        dds3EffectBlurDrawFrameMask(list, work->params.blend, work->texture);
        slot = work->slots;
        if (work->params.number > 0) {
            count = work->params.number;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.14159265f) {
                        dds3EffectDistBlurPartInit(work, slot);
                    }
                    slot->quad.scale = work->params.distortion * sdfSin(slot->angle) + 1.0f;
                    level = fptodp((f32)(work->params.abgr >> 24));
                    angle = fptodp(slot->angle);
                    if (dpcmp(angle, 0.0) < 0) {
                        angle = dpsub(0.0, angle);
                    }
                    level = dpmul(level, dpsub(3.14159265f, angle));
                    level = dpmul(level, 1.0f / 3.14159265f);
                    alpha = dptoul(level);
                    slot->quad.abgr = (slot->quad.abgr & 0xFFFFFF) | (alpha << 24);
                    dds3EffectBlurDrawAlphaMulti(list, &slot->quad);
                    slot->angle += work->params.speed;
                } else {
                    slot->delay--;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        dds3EffectBlurDrawFrameMaskReset(list);
    }
}

/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *hDistortionBlur, dds3Effect_RippleBlurParam_t *pParam) {
    u32 count = hDistortionBlur->params.number;

    hDistortionBlur->params = *pParam;
    hDistortionBlur->params.number = count;
}

/* The scale variant has its own source-handle setter. */
void func_001A2668(effBlurScaleWork_t *work, struct sdfTexHandle_t *sourceHandle) {
    work->texture = sourceHandle;
}

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void dds3EffectDistortionBlurSetTex(effBlurScaleWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

/* Size the slot's quad from its current phase, about the work centre. */
void dds3EffectRipBlurPartAreaSet(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart) {
    s32 half = (s32)((f32)pParam->params.area * pPart->phase);
    s32 x = pParam->params.center[0] + 0x100;
    s32 y = pParam->params.center[1] + 0xE0;

    pPart->quad.xy[0][0] = x - half;
    pPart->quad.xy[0][1] = y - half;
    pPart->quad.xy[1][0] = x + half;
    pPart->quad.xy[1][1] = y + half;
}

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;

    pPart->phase = 0.0f;
    pPart->angle = 0.0f;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    quad->rotate = pParam->params.rotate;
    quad->center[0] = pParam->params.center[0];
    quad->center[1] = pParam->params.center[1];
}

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur) {
    effBlurScaleSlot_t *slot = hRippleBlur->slots;
    s32 count = hRippleBlur->params.number;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        dds3EffectRipBlurPartInit(hRippleBlur, slot);
        slot->phase = -(hRippleBlur->params.interval * (f32)i);
    }
}

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam) {
    s32 count = pParam->number;
    void *allocation = sdfMemAllocDirect(count * 0x30 + 0x38);
    effBlurScaleWork_t *work = (effBlurScaleWork_t *)sdfMemLock(allocation);
    effBlurScaleSlot_t *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, pParam, 0x2C);
    work->allocation = allocation;
    work->slots = (effBlurScaleSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(3);
    slot = work->slots;
    while (i < count) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.interval * (f32)i);
        i++;
        slot++;
    }
    return work;
}

/* Release the scale variant's allocation, including its inline slot array. */
void dds3EffectRippleBlurDelete(effBlurScaleWork_t *hRippleBlur) {
    sdfMemFreeDirect(hRippleBlur->allocation);
}

/* Phase-driven sibling of the distortion update; adapted from DDS/redthing1
 * effBlurStepScaleSlotsAndDraw. SMT3 preserves the slot's RGB and uses the
 * two-argument renderer. Extract then attenuate the same alpha quantity. */
void dds3EffectRippleBlurDraw(effBlurScaleWork_t *work)
{
    struct sdfDrawTag_t *list;
    effBlurScaleSlot_t *slot;
    s32 count;
    u32 alpha;

    if (dds3NullDrawProc() == 0) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        dds3EffectBlurDrawFrameMask(list, work->params.blend, work->texture);
        slot = work->slots;
        if (work->params.number > 0) {
            count = work->params.number;
            do {
                if (slot->phase >= 0.0f) {
                    slot->quad.scale = work->params.power * sdfSin(slot->angle) *
                                              (1.0f - slot->phase) + 1.0f;
                    alpha = work->params.abgr >> 24;
                    alpha = (u32)((f32)alpha * (1.0f - slot->phase));
                    slot->quad.abgr = (slot->quad.abgr & 0xFFFFFF) | (alpha << 24);
                    dds3EffectRipBlurPartAreaSet(work, slot);
                    dds3EffectBlurDrawAlphaMulti(list, &slot->quad);
                    slot->angle += work->params.frequency;
                    slot->phase += work->params.speed;
                    if (slot->phase > 1.0f) {
                        dds3EffectRipBlurPartInit(work, slot);
                    }
                } else {
                    slot->phase += work->params.speed;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        dds3EffectBlurDrawFrameMaskReset(list);
    }
}

/* Queue blend state and a uniformly colored GS quad from two source corners. */
void dds3EffectFilter(dds3Effect_Filter_t *source) {
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
    blendPacket[6] = source->blend;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);

    drawPacket = effPkt2DAllocF4(0);
    color = source->abgr | 0x3F80000000000000ULL;
    x[0] = (source->xy[0][0] << 4) + 0x7000;
    x[1] = (source->xy[1][0] << 4) + 0x7000;
    y[0] = (source->xy[0][1] << 3) + 0x7900;
    y[1] = (source->xy[1][1] << 3) + 0x7900;
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

/* One textured quad over the source rectangle, tinted by its own RGBA. */
/* NON_MATCHING: 22 of 110 words differ (first at +0xC8). */
#ifdef NON_MATCHING
void dds3EffectFilterDrawTexQuad(effResourceRectDrawParams_t *source, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    u64 *packet;
    effBlurPrimTriStrip_t *prim;
    sdfGrPacketHeader_t *primPacket;
    sdfGrOt_t *ot;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 0xE;
    packet[4] = 0x31001;
    packet[5] = 0x48;
    packet[6] = source->blendControl;
    packet[7] = 0x43;
    sdfDrawTagLink(list, (u32)packet);
    sdf2DLinkTexPacket(list, texture, 1);

    primPacket = effPkt2DAllocTriStrip(1, 0x200);
    prim = effPkt2DGetPrimPtr(primPacket);
    prim->vtx[0].s = 0.0f;
    prim->vtx[1].s = 0.0f;
    prim->vtx[2].s = 1.0f;
    prim->vtx[3].s = 1.0f;
    prim->vtx[0].t = 0.0f;
    prim->vtx[1].t = 1.0f;
    prim->vtx[2].t = 0.0f;
    prim->vtx[3].t = 1.0f;
    prim->vtx[0].s = 0.0f;
    prim->vtx[1].s = 0.0f;
    prim->vtx[2].s = 1.0f;
    prim->vtx[3].s = 1.0f;
    prim->vtx[0].t = 0.0f;
    prim->vtx[1].t = 1.0f;
    prim->vtx[2].t = 0.0f;
    prim->vtx[3].t = 1.0f;
    prim->r = source->color[0];
    prim->g = source->color[1];
    prim->b = source->color[2];
    prim->a = source->color[3];
    x0 = (source->bounds.left << 4) + 0x7000;
    prim->vtx[0].x = x0;
    y0 = (source->bounds.top << 3) + 0x7900;
    prim->vtx[0].y = y0;
    prim->vtx[1].x = x0;
    y1 = (source->bounds.bottom << 3) + 0x7900;
    prim->vtx[1].y = y1;
    x1 = (source->bounds.right << 4) + 0x7000;
    prim->vtx[2].x = x1;
    prim->vtx[2].y = y0;
    prim->vtx[3].x = x1;
    prim->vtx[3].y = y1;
    prim->vtx[0].z = 0;
    prim->vtx[0].adc = 0;
    prim->vtx[1].z = 0;
    prim->vtx[1].adc = 0;
    prim->vtx[2].z = 0;
    prim->vtx[2].adc = 0;
    prim->vtx[3].z = 0;
    prim->vtx[3].adc = 0;
    sdfDrawTagLink(list, (u32)primPacket);
    ot = &sdfGrOtTable[57];
    ot->regFunc(ot, list);
}
#else
INCLUDE_ASM(const s32, "effect/effBlur_Filter", dds3EffectFilterDrawTexQuad);
#endif

/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, pParam, sizeof(*pParam));
    return dst;
}

void dds3EffectPartialMultiFilterDelete(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter) {
    sdfChipFreeDirect(hPartialMultiFilter);
}

void dds3EffectPartialMultiFilterDraw(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter) {
    s32 half = (s32)((f32)hPartialMultiFilter->param.area * 1.4f);
    s32 x = hPartialMultiFilter->param.center[0] + 0x100;
    s32 y = hPartialMultiFilter->param.center[1] + 0xE0;

    hPartialMultiFilter->param.Filter.bounds.left = x - half;
    hPartialMultiFilter->param.Filter.bounds.top = y - half;
    hPartialMultiFilter->param.Filter.bounds.right = x + half;
    hPartialMultiFilter->param.Filter.bounds.bottom = y + half;
    dds3EffectFilterDrawTexQuad(&hPartialMultiFilter->param.Filter, hPartialMultiFilter->hAlphaTex);
}
