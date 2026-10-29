#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

struct pipeTextureFile_t;

extern void (*D_00551828)(void);

extern void *memset(void *a, s32 b, u32 c);

typedef struct tagPipeTexture {
    sdfTexHandle_t *th;
    sdfMemHandle_t *mh;
    struct pipeTextureFile_t *mp;
    u32 unk0C;
} tagPipeTexture;

typedef struct tagMakaObject_t {
    s32 maka;
    s8 count;
    s8 resv[3];
    s32 pos[2];
    void *makaObject;
} tagMakaObject_t;

extern void eglLinkSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
                          s32 uw, s32 vh, u32 rgba0, u32 rgba1,
                          u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

extern void makaInit(void);

extern tagPipeTexture *MakaTex;

extern s32 ZmakaCount;

extern void makaDrawNormal(tagMakaObject_t *object);

extern void makaDrawSingle(tagMakaObject_t *object);

extern void makaDrawMulti(tagMakaObject_t *object);

extern void *makaWork;


void makaInit(void)
{
    void *work = makaWork;

    memset(work, 0, 0xC8);
    work = NULL;
}

void makaAdd(f32 x, f32 y, s32 amount)
{
    s32 posX = (s32)x;
    s32 posY = (s32)y;
    tagMakaObject_t *object = makaWork;
    s32 remaining = 15;

    while (--remaining != -1) {
        if (object->maka == 0) {
            if (amount & 0x80000000) {
                object->pos[0] = 256;
                object->pos[1] = 188;
                object->count = 16;
            } else {
                object->pos[0] = posX;
                object->pos[1] = posY;
                object->count = 15;
            }
            object->maka = amount;
            break;
        }
        object++;
    }
}

s32 makaGetBarWidth(s32 maka)
{
    u32 rest = maka & 0x7FFFFFFF;
    s32 digits = 0;

    while (rest != 0) {
        rest /= 10;
        digits++;
    }
    return digits;
}

s32 makaGetBarCell(s32 maka, s32 digit)
{
    s16 scale = 1;
    s32 i;

    if (digit == 0) {
        return -1;
    }
    for (i = digit - 1; i != 0; i--) {
        scale = scale * 10;
    }
    return (maka / scale) % 10;
}

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawNormal);

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawSingle);

extern void eglLinkAlpha(s32 alpha);
extern void makaDrawBar(s32 x, s32 y, u16 value, u8 alpha, f32 sx, f32 sy);
extern void makaDrawMultiGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha);
extern void makaDrawGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha);

/* NON_MATCHING: 291 of 293 instruction words match at the native 1172-byte size. */
#ifdef NON_MATCHING
void makaDrawMulti(tagMakaObject_t *object)
{
    f32 easeTable[7] = {0.4f, 0.8f, 0.83f, 0.86f, 0.89f, 0.95f, 1.0f};
    s8 phase = (s8)(16 - (s32)object->count);
    s32 value = object->maka;
    f32 easing;
    f32 alpha = 0.0f;

    eglLinkAlpha(0);

    if (phase < 8) {
        easing = easeTable[phase - 1];
    } else if (phase < 12) {
        easing = (11 - phase) * 0.25f;
    } else {
        easing = 0.0f;
    }

    if (phase < 2) {
        alpha = 1.0f;
    } else if (phase < 13) {
        alpha = (12 - phase) / 11.0f;
    } else {
        alpha = 0.0f;
    }

    if (phase < 8) {
        makaDrawBar(object->pos[0],
                    (s32)(object->pos[1] * (1.0f - easing)
                         + (object->pos[1] - 40) * easing), (u16)value,
                    (u32)(alpha * 128.0f), 1.7f, 1.7f);
    } else {
        makaDrawBar(object->pos[0],
                    (s32)((object->pos[1] - 40) * easing
                         + (object->pos[1] + 3) * (1.0f - easing)), (u16)value,
                    (u32)(alpha * 128.0f), 1.7f, 1.7f);
    }

    eglLinkAlpha(1);
    if (phase < 7) {
        alpha = 0.0f;
    } else if (phase < 8) {
        alpha = 1.0f;
    } else {
        alpha = (16 - phase) / 9.0f;
    }
    makaDrawBar(object->pos[0], object->pos[1] - 40, (u16)value,
                (u8)(u32)(alpha * 25.0f), 1.8f, 1.8f);

    if (phase < 7) {
        easing = 0.0f;
    } else if (phase < 12) {
        easing = (11 - phase) * 0.25f;
    } else {
        easing = 0.0f;
    }
    if (phase < 4) {
        alpha = 0.0f;
    } else if (phase < 7) {
        alpha = (phase - 3) / 3.0f;
    } else if (phase < 14) {
        alpha = (13 - phase) / 7.0f;
    } else {
        alpha = 0.0f;
    }
    makaDrawMultiGauge(256, 156,
                       1.0f - easing + (easing + easing),
                       1.0f - easing + easing * 1.3f, alpha * 64.0f);

    if (phase < 5) {
        easing = 0.0f;
    } else if (phase < 12) {
        easing = (phase - 4) / 7.0f;
    } else {
        easing = 1.0f;
    }
    if (phase < 4) {
        alpha = 0.0f;
    } else if (phase < 8) {
        alpha = (phase - 3) * 0.25f;
    } else if (phase < 12) {
        alpha = (11 - phase) * 0.25f;
    } else {
        alpha = 0.0f;
    }
    makaDrawGauge(0x100, 0x94,
                  1.0f - easing + easing * 2.1f,
                  1.0f - easing + easing * 3.0f,
                  alpha * 89.0f);
}

#else
INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawMulti);
#endif

void makaDraw(tagMakaObject_t *makaPtr)
{
    if (makaPtr->maka < 0) {
        if (ZmakaCount == 1) {
            makaDrawSingle(makaPtr);
        } else {
            makaDrawMulti(makaPtr);
        }
    } else {
        makaDrawNormal(makaPtr);
        makaPtr = NULL;
    }
}

void makaDrawGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha)
{
    u32 color = ((s32)alpha << 24) | 0x808080;

    eglLinkSprite((s32)(x - sx * 52.0f), (s32)(y - sy * 30.0f), (s32)(sx * 104.0f),
                  (s32)(sy * 61.0f), 1, 1, 0x68, 0x3D, color, color, color, color,
                  MakaTex[2].th);
}

void makaDrawMultiGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha)
{
    u32 color = ((s32)alpha << 24) | 0x808080;

    eglLinkSprite((s32)(x - sx * 81.0f), (s32)(y - sy * 13.0f), (s32)(sx * 163.0f),
                  (s32)(sy * 26.0f), 1, 1, 0xA3, 0x1A, color, color, color, color,
                  MakaTex[3].th);
}

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawNormalGauge);

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawBar);

INCLUDE_RODATA(const s32, "pipe/pipeMakaAnim", D_00548FD8);

INCLUDE_RODATA(const s32, "pipe/pipeMakaAnim", D_00548FF8);

