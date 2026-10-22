#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);

extern u32 dds3GetRandA(void *name);

extern void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

extern void *memset(void *a, s32 b, u32 c);

typedef struct tagpipeBasic {
    void *base;
    mdlObj_t *mdl;
    u8 unk08[4];
    s32 enabled;
    f32 pos[4];
    f32 rot[4];
    f32 scl[4];
} tagpipeBasic;


extern s32 mymdlAddObject(tagpipeBasic *pWork, u16 mdlSize);

extern void mymdlDelObject(tagpipeBasic *pWork);

extern void mymdlVisible(tagpipeBasic *pWork, s8 value);

extern void mymdlVisibleAll(tagpipeBasic *pWork, s16 mdlSize, s8 value);

extern void mymdlInitObject(tagpipeBasic *pWork, u16 mdlSize);

extern void dds3DeleteUnitObjectBasic(void *basic);

extern void DEBUG_PRINT(const char *str, ...);

extern u32 mdlGetColor(void *aHandle);

extern void mdlSetColor(void *aHandle, u32 aColor);

extern void dds3ObjectBaseOnFlag(void *basic, u32 flag);

extern void dds3SetPosPoseSet(void *pBasic, f32 *pos);

extern void dds3SetRotPoseSet(void *pBasic, f32 *rot);

extern void dds3SetSclPoseSet(void *pBasic, f32 *scl);


INCLUDE_ASM(const s32, "pipe/pipeModel", InitModel);

s32 mymdlAddObject(tagpipeBasic *pWork, u16 mdlSize)
{
    s16 i;

    for (i = 0; i < mdlSize; i++) {
        if (pWork[i].enabled == 0) {
            dds3ObjectBaseOffFlag(pWork[i].base, 1);
            pWork[i].enabled = 1;
            return i;
        }
    }
    DEBUG_PRINT("i %d NO OBJECT\n", i);
    return -1;
}

void mymdlDelObject(tagpipeBasic *pWork)
{
    pWork->enabled = 0;
    dds3ObjectBaseOnFlag(pWork->base, 1);
    pWork = NULL;
}

void mymdlInitObject(tagpipeBasic *pWork, u16 mdlSize)
{
    s16 i;

    for (i = 0; i < mdlSize; i++) {
        mymdlDelObject(pWork);
        pWork++;
    }
}

void mymdlPosSet(tagpipeBasic *pWork, f32 x, f32 y, f32 z)
{
    pWork->pos[0] = x;
    pWork->pos[1] = y;
    pWork->pos[2] = z;
    dds3SetPosPoseSet(pWork->base, pWork->pos);
    pWork = NULL;
}

void SetMdlPosition(tagpipeBasic *pWork, s16 mdlSize)
{
    s16 i;

    for (i = 0; i < mdlSize; i++) {
        if (pWork[i].base != NULL) {
            dds3DeleteUnitObjectBasic(pWork[i].base);
            pWork[i].base = NULL;
        }
    }
}

void mymdlRotSet(tagpipeBasic *pWork, f32 x, f32 y, f32 z)
{
    pWork->rot[0] = x;
    pWork->rot[1] = y;
    pWork->rot[2] = z;
    dds3SetRotPoseSet(pWork->base, pWork->rot);
    pWork = NULL;
}

void mymdlSclSet(tagpipeBasic *pWork, f32 scl)
{
    pWork->scl[0] = scl;
    pWork->scl[1] = scl;
    pWork->scl[2] = scl;
    dds3SetSclPoseSet(pWork->base, pWork->scl);
    pWork = NULL;
}

void mymdlColorSet(tagpipeBasic *pWork, u32 color)
{
    mdlSetColor(pWork->mdl, color);
    pWork = NULL;
}

void mymdlAlphaSet(tagpipeBasic *pWork, u8 alpha)
{
    mdlSetColor(pWork->mdl, (mdlGetColor(pWork->mdl) & 0xFFFFFF) | (alpha << 24));
    pWork = NULL;
}

void mymdlInit(tagpipeBasic *pWork, u16 sclx, f32 scly, f32 sclz, f32 sclw, f32 mdlSize)
{
    u16 i;

    for (i = 0; i < sclx; i++) {
        memset(pWork, 0, 0x40);
        pWork->pos[3] = 1.0f;
        pWork->scl[0] = scly;
        pWork->scl[1] = sclz;
        pWork->scl[2] = sclw;
        pWork->scl[3] = mdlSize;
        pWork++;
    }
}

void mymdlVisible(tagpipeBasic *pWork, s8 value)
{
    if (value == 1) {
        dds3ObjectBaseOffFlag(pWork->base, 1);
    } else {
        dds3ObjectBaseOnFlag(pWork->base, 1);
        pWork = NULL;
    }
}

void mymdlVisibleAll(tagpipeBasic *pWork, s16 mdlSize, s8 value)
{
    s16 i;

    for (i = 0; i < mdlSize; i++) {
        if (pWork[i].enabled == 1) {
            mymdlVisible(&pWork[i], value);
        }
    }
}

void mymdlSetMotion(tagpipeBasic *node, f32 frame) {
    void *motion = mdlGetMotionCtrlr(node->mdl, 0);
    if (frame < 0.0f) {
        u32 random = dds3GetRandA(NULL);
        sdfMotionSeek(motion, (f32)(random % node->mdl->first->frameLength));
        return;
    }
    sdfMotionSeek(motion, frame);
    /* DDS2 mnuSampleModelMotionAtFrame and SMT3 002A4BB0 retain jal here;
     * resetting the consumed frame follows the documented tail-call exception. */
    frame = 0.0f;
}

