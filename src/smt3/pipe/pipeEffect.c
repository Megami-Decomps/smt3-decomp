#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

typedef struct tagPipeObject {
    s32 offset;
    f32 w;
    f32 pos[2];
    f32 vpos[2];
    s8 id;
    u8 action;
    u8 mdl;
    u8 unk1B;
    s16 count;
    u16 flags;
} tagPipeObject;

typedef struct pipeBlurParam_t {
    u32 color;
    s32 unk04;
    s32 unk08;
    f32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 width;
    s32 height;
    s32 unk28;
    s32 unk2C;
} pipeBlurParam_t;

extern void dds3EffectBlur(pipeBlurParam_t *param);

typedef struct PIPE_EFF_t {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *d3p;
    f32 x;
    f32 y;
    f32 z;
    f32 pos2[2];
    s32 offset;
    s32 count;
} PIPE_EFF_t;

extern char D_00548F48[];

extern void dds3EffectDelete(void *effect);

extern s32 offsetCount;

extern PIPE_EFF_t *pipe_CELL01_D3P, *pipe_BREAK01_D3P, *pipe_BREAK02_D3P;

extern PIPE_EFF_t *pipe_DAMAGE01_D3P, *pipe_DAMAGE03_D3P, *pipe_ATTACK01_D3P;

extern void pipeGetObjData(PIPE_EFF_t *effect, s32 maxSize);

extern void pipeEffDraw2Doffset(PIPE_EFF_t *effect, u8 maxSize);

extern void pipeFreeEffects(PIPE_EFF_t *effect, u8 maxSize);

extern void pipeEffRelease(void);

extern void pipeBulrLimit(void);

extern s32 pipeBulrLimitValue;

extern void sdfMemFreeDelayedDirect(void *handle);

extern tagPipeObject *objMyData(void);


void pipeBulrSetLimit(s32 frame)
{
    pipeBulrLimitValue = frame;
}

void pipeBulrLimit(void)
{
    pipeBlurParam_t blur;

    if (pipeBulrLimitValue != 0) {
        blur.color = 0x5C8A8A8A;
        blur.unk04 = 0x48;
        blur.unk0C = 1.0f;
        blur.width = 0x200;
        blur.height = 0x1C0;
        blur.unk08 = 0;
        blur.unk10 = 0;
        blur.unk14 = 0;
        blur.unk18 = 0;
        blur.unk1C = 0;
        dds3EffectBlur(&blur);
        pipeBulrLimitValue = pipeBulrLimitValue - 1;
    } else {
        blur.color = 0x2E8A8A8A;
        blur.unk04 = 0x48;
        blur.unk0C = 1.0f;
        blur.width = 0x200;
        blur.height = 0x1C0;
        blur.unk08 = 0;
        blur.unk10 = 0;
        blur.unk14 = 0;
        blur.unk18 = 0;
        blur.unk1C = 0;
        dds3EffectBlur(&blur);
    }
}

INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeEffCreateLoadFile);

INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeEffLoad);

void pipeFreeEffects(PIPE_EFF_t *effect, u8 maxSize)
{
    PIPE_EFF_t *pEff = effect;
    u8 i;

    for (i = maxSize - 2; i != 0xFF; i--) {
        if (pEff->d3p != NULL) {
            dds3EffectDelete(pEff->d3p);
            pEff->d3p = NULL;
        }
        pEff++;
    }
    if (effect->mhFile != NULL) {
        sdfMemFreeDelayedDirect(effect->mhFile);
        effect->mhFile = NULL;
    }
}

void pipeEffRelease(void)
{
    s32 maxSize = 10;

    pipeFreeEffects(pipe_CELL01_D3P, maxSize);
    pipeFreeEffects(pipe_BREAK01_D3P, 10);
    pipeFreeEffects(pipe_BREAK02_D3P, 10);
    pipeFreeEffects(pipe_DAMAGE01_D3P, 10);
    pipeFreeEffects(pipe_DAMAGE03_D3P, 10);
    pipeFreeEffects(pipe_ATTACK01_D3P, 5);
    maxSize = 0;
}

void pipeEffDraw(void)
{
    s32 maxSize = 10;

    pipeEffDraw2Doffset(pipe_CELL01_D3P, maxSize);
    pipeEffDraw2Doffset(pipe_BREAK01_D3P, 10);
    pipeEffDraw2Doffset(pipe_BREAK02_D3P, 10);
    pipeEffDraw2Doffset(pipe_DAMAGE01_D3P, 10);
    pipeEffDraw2Doffset(pipe_DAMAGE03_D3P, 10);
    pipeEffDraw2Doffset(pipe_ATTACK01_D3P, 5);
    maxSize = 0;
}

void pipeEffCal(void)
{
    s32 maxSize = 4;

    pipeGetObjData(pipe_CELL01_D3P, maxSize);
    pipeGetObjData(pipe_BREAK01_D3P, 4);
    pipeGetObjData(pipe_BREAK02_D3P, 4);
    pipeGetObjData(pipe_DAMAGE01_D3P, 2);
    pipeGetObjData(pipe_DAMAGE03_D3P, 2);
    pipeGetObjData(pipe_ATTACK01_D3P, 6);
    maxSize = 0;
}

/* NON_MATCHING: 9 of 71 words, exact size; the third arm materialises its
 * constants in a different order. */


#ifdef NON_MATCHING
void pipeGetObjData(PIPE_EFF_t *effect, s32 maxSize)
{
    PIPE_EFF_t *pEff = effect;
    u8 i;

    if ((u8)maxSize == 2) {
        s32 offset = offsetCount;

        for (i = 9; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset = offset;
            }
            pEff++;
        }
    } else if ((u8)maxSize == 4) {
        for (i = 9; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset += 6;
            }
            pEff++;
        }
    } else if ((u8)maxSize == 6) {
        tagPipeObject *objPtr = objMyData();
        s32 offset = offsetCount + 4;

        for (i = 4; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset = offset;
                pEff->pos2[0] = objPtr->pos[0] - 16.0f;
                pEff->pos2[1] = objPtr->pos[1] - 48.0f;
            }
            pEff++;
        }
    }
}
#else
INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeGetObjData);
#endif


INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeEffSet);

INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeLoadEffect);

INCLUDE_ASM(const s32, "pipe/pipeEffect", pipeEffDraw2Doffset);
