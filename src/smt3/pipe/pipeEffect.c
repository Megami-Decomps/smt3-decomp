#include "fpu.h"
#include "common.h"
#include "sdf_resource.h"
#include "pipe.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);



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


extern void dds3EffectDelete(void *pData);
extern void *dds3EffectLoad(void *pD3P);
void pipeEffCreateLoadFile(PIPE_EFF_t *effect, const char *path, u8 maxSize);

extern s32 offsetCount;

extern PIPE_EFF_t *pipe_CELL01_D3P, *pipe_BREAK01_D3P, *pipe_BREAK02_D3P;

extern PIPE_EFF_t *pipe_DAMAGE01_D3P, *pipe_DAMAGE03_D3P, *pipe_ATTACK01_D3P;

extern void pipeGetObjData(PIPE_EFF_t *effect, u8 type);

extern void pipeEffDraw2Doffset(PIPE_EFF_t *pEffect, u8 maxSize);

extern void pipeFreeEffects(PIPE_EFF_t *effect, u8 maxSize);

extern void pipeEffRelease(void);

extern void pipeBulrLimit(void);

extern s32 pipeBulrLimitValue;

extern void sdfMemFreeDelayedDirect(void *aHandle);

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


void pipeEffCreateLoadFile(PIPE_EFF_t *effect, const char *path, u8 maxSize)
{
    PIPE_EFF_t *entry;
    u8 i = maxSize - 1;

    effect->mhFile = NULL;
    effect->d3p = NULL;
    effect->x = effect->y = effect->z = 0.0f;
    effect->size = 0;
    effect->ptr = NULL;
    effect->mhFile = sdfDevLoadFileEx(path, (u32 *)&effect->ptr, (u32 *)&effect->size);

    effect++;
    entry = effect;
    if (--i != 0xFF) {
        do {
            entry->mhFile = NULL;
            entry->size = effect->size;
            entry->d3p = NULL;
            entry->x = entry->y = entry->z = 0.0f;
            entry->ptr = NULL;
            entry->pos2[0] = 0.0f;
            entry->pos2[1] = 0.0f;
            entry->offset = 0;
            entry++;
        } while (--i != 0xFF);
    }
}

void pipeEffLoad(void)
{
    s32 maxSize = 10;

    pipeEffCreateLoadFile(pipe_CELL01_D3P, "/pipe/CEL01.D3P", maxSize);
    pipeEffCreateLoadFile(pipe_BREAK01_D3P, "/pipe/BRAKE01.D3P", maxSize);
    pipeEffCreateLoadFile(pipe_BREAK02_D3P, "/pipe/BRAKE02.D3P", maxSize);
    pipeEffCreateLoadFile(pipe_DAMAGE01_D3P, "/pipe/DAMAGE01.D3P", maxSize);
    pipeEffCreateLoadFile(pipe_DAMAGE03_D3P, "/pipe/DAMAGE03.D3P", maxSize);
    maxSize = 5;
    pipeEffCreateLoadFile(pipe_ATTACK01_D3P, "/pipe/FPS3.D3P", maxSize);
    /* Retail keeps a jal and epilogue after the final call. The same dead
     * reset is present in pipeEffDraw and pipeEffCal; both sibling bodies
     * match exactly with their native jal-plus-return sequences. */
    maxSize = 0;
}

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

void pipeGetObjData(PIPE_EFF_t *effect, u8 type)
{
    PIPE_EFF_t *pEff = effect;
    u8 i;

    if (type == 2) {
        s32 offset = offsetCount;

        for (i = 9; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset = offset;
            }
            pEff++;
        }
    } else if (type == 4) {
        for (i = 9; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset += 6;
            }
            pEff++;
        }
    } else if (type == 6) {
        tagPipeObject *objPtr = objMyData();
        s32 offset;

        i = 4;
        offset = offsetCount + 4;
        for (; i != 0xFF; i--) {
            if (pEff->d3p != NULL) {
                pEff->offset = offset;
                pEff->pos2[0] = objPtr->pos[0] - 16.0f;
                pEff->pos2[1] = objPtr->pos[1] - 48.0f;
            }
            pEff++;
        }
    }
}


void pipeLoadEffect(s32 effectID, PIPE_EFF_t *effects, u8 maxSize, f32 x, f32 y);

void pipeEffSet(tagPipeObject *object, s32 type)
{
    switch ((s32)(u8)type) {
    case 2:
        pipeLoadEffect(object->offset, pipe_DAMAGE01_D3P, 10, object->pos[0], object->pos[1]);
        pipeLoadEffect(object->offset, pipe_DAMAGE03_D3P, 10, object->pos[0], object->pos[1]);
        break;
    case 4:
        pipeLoadEffect(object->offset + 5, pipe_BREAK01_D3P, 10, object->pos[0], object->pos[1]);
        pipeLoadEffect(object->offset + 5, pipe_BREAK02_D3P, 10, object->pos[0], object->pos[1]);
        pipeLoadEffect(object->offset + 5, pipe_CELL01_D3P, 10, object->pos[0], object->pos[1]);
        break;
    case 6: {
        f32 x = object->pos[0] - 16.0f;
        f32 y = object->pos[1] - 48.0f;
        pipeLoadEffect(object->offset, pipe_ATTACK01_D3P, 5, x, y);
        /* Like pipeEffLoad's maxSize reset, consume the local selector after
         * the final load. Retail keeps jal and the common epilogue here. */
        type = 0;
        break;
    }
    default:
        break;
    }
}

void pipeLoadEffect(s32 effectID, PIPE_EFF_t *effects, u8 maxSize, f32 x, f32 y)
{
    PIPE_EFF_t *first = effects;

    maxSize--;
    while (--maxSize != 0xFF) {
        if (effects->d3p == NULL) {
            void *handle = dds3EffectLoad(first->ptr);
            effects->pos2[0] = x;
            effects->d3p = handle;
            effects->pos2[1] = y;
            effects->offset = effectID;
            effects->count = 14;
            break;
        }
        effects++;
    }
}

extern void *memset(void *dest, s32 value, u32 size);
extern s32 dds3EffectGetState(void *effect);
extern void dds3EffectSetPosition(void *effect, const void *position);
extern void dds3EffectDraw(void *effect);
extern void getPosTranslate(tagPipeObject *object, f32 *position);

void pipeEffDraw2Doffset(PIPE_EFF_t *pEffect, u8 maxSize)
{
    PIPE_EFF_t *effect;
    f32 position[4];
    tagPipeObject object;
    u8 index;

    memset(position, 0, 0x10);
    effect = pEffect;
    position[3] = 1.0f;
    index = maxSize - 2;
    if (index != 0xFF) {
        do {
            if (effect->d3p != NULL) {
                if (dds3EffectGetState(effect->d3p) == 0) {
                    dds3EffectDelete(effect->d3p);
                    effect->d3p = NULL;
                } else {
                    f32 x = effect->pos2[0];
                    f32 y = effect->pos2[1];
                    object.pos[0] = x;
                    object.pos[1] = y;
                    object.offset = effect->offset;
                    getPosTranslate(&object, position);
                    dds3EffectSetPosition(effect->d3p, position);
                    dds3EffectDraw(effect->d3p);
                    if (maxSize == 5 && effect->count != 0) {
                        effect->count--;
                        if (effect->count == 0) {
                            dds3EffectDelete(effect->d3p);
                            effect->d3p = NULL;
                        }
                    }
                }
            }
            index--;
            effect++;
        } while (index != 0xFF);
    }
}

INCLUDE_SDATA(const s32, "pipe/pipeEffect", pipeBulrLimitValue);

