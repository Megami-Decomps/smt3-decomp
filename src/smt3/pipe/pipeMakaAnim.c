#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

struct pipeTextureFile_t;

extern void (*D_00551828)(void);

extern void *memset(void *destination, s32 value, u32 bytes);

typedef struct tagPipeTexture {
    sdfTexHandle_t *handle;
    sdfMemHandle_t *allocation;
    struct pipeTextureFile_t *resourceData;
    u32 unk0C;
} tagPipeTexture;

typedef struct tagMakaObject_t {
    s32 maka;
    s8 count;
    s8 resv[3];
    s32 pos[2];
    void *makaObject;
} tagMakaObject_t;

extern void eglLinkSprite(s32 x, s32 y, s32 width, s32 height, s32 u, s32 v,
                          s32 textureWidth, s32 textureHeight, u32 color0, u32 color1,
                          u32 color2, u32 color3, sdfTexHandle_t *texture);

extern void makaInit(void);

extern tagPipeTexture *MakaTex;

extern s32 ZmakaCount;

extern void makaDrawNormal(void);

extern void makaDrawSingle(void);

extern void makaDrawMulti(void);

extern void *makaWork;


void makaInit(void)
{
    void *work = makaWork;

    memset(work, 0, 0xC8);
    work = NULL;
}

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaAdd);

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

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawMulti);

void makaDraw(tagMakaObject_t *makaPtr)
{
    if (makaPtr->maka < 0) {
        if (ZmakaCount == 1) {
            makaDrawSingle();
        } else {
            makaDrawMulti();
        }
    } else {
        makaDrawNormal();
        makaPtr = NULL;
    }
}

void makaDrawGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha)
{
    u32 color = ((s32)alpha << 24) | 0x808080;

    eglLinkSprite((s32)(x - sx * 52.0f), (s32)(y - sy * 30.0f), (s32)(sx * 104.0f),
                  (s32)(sy * 61.0f), 1, 1, 0x68, 0x3D, color, color, color, color,
                  MakaTex[2].handle);
}

void makaDrawMultiGauge(s32 x, s32 y, f32 sx, f32 sy, f32 alpha)
{
    u32 color = ((s32)alpha << 24) | 0x808080;

    eglLinkSprite((s32)(x - sx * 81.0f), (s32)(y - sy * 13.0f), (s32)(sx * 163.0f),
                  (s32)(sy * 26.0f), 1, 1, 0xA3, 0x1A, color, color, color, color,
                  MakaTex[3].handle);
}

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawNormalGauge);

INCLUDE_ASM(const s32, "pipe/pipeMakaAnim", makaDrawBar);

INCLUDE_RODATA(const s32, "pipe/pipeMakaAnim", D_00548FD8);

INCLUDE_RODATA(const s32, "pipe/pipeMakaAnim", D_00548FF8);

