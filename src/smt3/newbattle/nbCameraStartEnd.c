#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

typedef struct nbCameraAnim_t {
    s32 rotateFrames;
    f32 rotateStep;
    s32 distanceFrames;
    f32 distanceStep;
} nbCameraAnim_t;

typedef struct nbCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 stateChange;
    u32 state;
    u32 stateOld;
    u32 stateOld2;
    u32 phase;
    s32 frame;
    u32 type;
    f32 angle;
    nbCameraAnim_t anim;
} nbCamera_t;

extern void nbCameraState_Start_Set(nbCamera_t *pCamera);

extern void nbCameraState_Start_BA_Set(nbCamera_t *pCamera);

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

extern s32 nbCameraStartState;

extern u32 D_0054F37C;


extern char D_00534630[];

extern s32 printf(const char *format, ...);

extern u8 D_0054F370;

extern u32 D_0054F374;

extern u8 D_0054F378;

extern u32 D_0054F37C;

extern nbCamera_t pNbCamera;

extern s32 D_00551E08;

extern void nbCameraState_Start_BA_Animation(nbCamera_t *pCamera);

extern void nbCameraState_Start_Animation(nbCamera_t *pCamera);

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_Animation);

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_BA_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_BA_Animation);

void nbCameraState_Start_Init(void)
{
    if ((nbGetMainProcessData()->stat & 0x10) != 0) {
        nbCameraState_Start_BA_Set(&pNbCamera);
    } else {
        nbCameraState_Start_Set(&pNbCamera);
    }
    D_0054F370 = 0;
    D_0054F374 = 0;
    D_0054F378 = 0;
    D_0054F37C = 0;
}

void nbCameraState_Start_Calc(void)
{
    if ((nbGetMainProcessData()->stat & 0x10) != 0) {
        nbCameraState_Start_BA_Animation(&pNbCamera);
        return;
    }
    nbCameraState_Start_Animation(&pNbCamera);
}

s32 nb_1B33F0(void)
{
    return D_00551E08;
}

s32 nbCameraStart(void)
{
    if (D_0054F370 != 0) {
        D_0054F370 = 0;
        printf("nbCameraStart:Read Wait = %d ****
", D_0054F374);
    }
    if (D_0054F378 != 0) {
        if (nbCameraStartState == 5) {
            D_0054F378 = 0;
            printf(D_00534630, D_0054F37C);
        }
    }
    return nbCameraStartState == 5;
}

void func_001B3460(nbCamera_t *pCamera) {
}

void func_001B3468(nbCamera_t *pCamera) {
}

void nb_1B3470(void)
{
    func_001B3460(&pNbCamera);
}

void nb_1B3490(void)
{
    func_001B3468(&pNbCamera);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", func_001B34B0);

INCLUDE_RODATA(const s32, "newbattle/nbCameraStartEnd", D_00534630);

