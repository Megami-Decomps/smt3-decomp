#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

typedef struct nbCameraAnim_t {
    s32 rotate_f;
    f32 rotate_s;
    s32 distance_f;
    f32 distance_s;
} nbCameraAnim_t;

typedef struct nbCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 state_change;
    u32 state;
    u32 state_old;
    u32 state_old2;
    u32 phase;
    s32 frame;
    u32 type;
    f32 angle;
    nbCameraAnim_t anim;
} nbCamera_t;

extern void nbCameraState_Start_Set(nbCamera_t *pNbCamera);

extern void nbCameraState_Start_BA_Set(nbCamera_t *pNbCamera);

/* 128-byte party records; this unit only reads the formation index. */


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

extern void nbCameraState_Start_BA_Animation(nbCamera_t *pNbCamera);

extern void nbCameraState_Start_Animation(nbCamera_t *pNbCamera);

/* Same 0x38-byte Bezier segment consumed by nbCameraLerpVec in effMath.c. */
typedef struct dds3Bezier4PPart_t {
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} dds3Bezier4PPart_t;

extern dds3Bezier4PPart_t nbCameraBAState;
extern dds3Bezier4PPart_t nbCameraStartBAState;
extern s32 nbCameraLerpVec(dds3Bezier4PPart_t *slot, f32 *out);
extern f32 D_00551DFC;
extern f32 D_00551E00;
extern s32 D_0054F380;
extern void effBTLModelAllShow(void);

extern u8 D_0056B270[];
extern void *memcpy(void *dst, const void *src, u32 size);

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_Set);

extern dds3Bezier4PPart_t nbCameraStartTable[2];
extern void effBTLModelAllShow(void);

void nbCameraState_Start_Animation(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;
    f32 battlePos[4];
    f32 startPos[4];
    s32 pending;
    u32 i;

    switch (nbCameraStartState) {
    case 0:
        nbCameraStartTable[0].step = 0.0027777777f;
        nbCameraStartTable[1].step = 0.0027777777f;
        D_00551E00 = 0.03333333135f;
        D_00551DFC = 0.0030555553f;
        nbCameraStartState = 1;
        nbCameraStartTable[1].t = 0.0f;
        nbCameraStartTable[0].t = 0.0f;
        break;
    case 1:
        data = nbGetMainProcessData();
        form = data->form + 4;
        i = 0;
        pending = 0;
        for (; i < 10; i++, form++) {
            if ((form->stat & 0x201) == 0x201 && form->hunit == NULL) {
                pending = 1;
                break;
            }
        }
        if (pending == 0) {
            nbCameraStartState = 2;
            D_0054F370 = 1;
            D_0054F378 = 1;
        }
        break;
    case 2:
        nbCameraStartTable[0].step += D_00551DFC;
        nbCameraStartTable[1].step += D_00551DFC;
        if (nbCameraStartTable[0].step >= D_00551E00) {
            nbCameraStartTable[0].step = D_00551E00;
            nbCameraStartTable[1].step = D_00551E00;
            nbCameraStartState = 3;
        }
        break;
    case 3:
        if (nbCameraStartTable[1].t > 0.75f) {
            nbCameraStartState = 4;
        }
        break;
    case 4:
        nbCameraStartTable[1].step *= 0.9649999738f;
        if (nbCameraStartTable[1].step < 0.0009999999311f) {
            nbCameraStartTable[1].step = 0.0f;
        }
        nbCameraStartTable[0].step = nbCameraStartTable[1].step;
        if (nbCameraStartTable[1].t >= 1.0f || nbCameraStartTable[1].step == 0.0f) {
            nbCameraStartState = 5;
        }
        break;
    case 5:
        return;
    }

    nbCameraLerpVec(&nbCameraStartTable[1], startPos);
    nbCameraLerpVec(&nbCameraStartTable[0], battlePos);
    SDF_VU0_LOADVECTOR(battlePos);
    SDF_VU0_STOREVECTOR(&pNbCamera->target);
    SDF_VU0_LOADVECTOR_B(startPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);

    D_00551E08++;
    if (D_0054F370 != 0) {
        D_0054F374++;
    }
    if (D_0054F378 != 0) {
        D_0054F37C++;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCameraStartEnd", nbCameraState_Start_BA_Set);

void nbCameraState_Start_BA_Animation(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;
    f32 battlePos[4];
    f32 startPos[4];
    s32 pending;
    u32 i;

    data = nbGetMainProcessData();
    form = data->form + 4;
    pending = 0;
    for (i = 0; i < 10; i++, form++) {
        if ((form->stat & 0x201) == 0x201 && form->hunit == NULL) {
            pending = 1;
            break;
        }
    }

    if (pending == 0) {
        nbCameraStartBAState.step += D_00551DFC;
        nbCameraBAState.step += D_00551DFC;
        if (nbCameraStartBAState.step >= D_00551E00) {
            nbCameraStartBAState.step = D_00551E00;
            nbCameraBAState.step = D_00551E00;
        }
    }

    nbCameraLerpVec(&nbCameraStartBAState, startPos);
    nbCameraLerpVec(&nbCameraBAState, battlePos);
    if (nbCameraStartBAState.t > 0.75f) {
        nbCameraStartBAState.step *= 0.9649999738f;
        if (nbCameraStartBAState.step < 0.0009999999311f) {
            nbCameraStartBAState.step = 0.0f;
        }
        nbCameraBAState.step *= 0.9649999738f;
        if (nbCameraBAState.step < 0.0009999999311f) {
            nbCameraBAState.step = 0.0f;
        }
    }

    SDF_VU0_LOADVECTOR_B(startPos);
    SDF_VU0_LOADVECTOR(battlePos);
    SDF_VU0_STOREVECTOR(&pNbCamera->target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);

    if (nbCameraStartBAState.t >= 1.0f || nbCameraStartBAState.step == 0.0f) {
        nbCameraStartState = 5;
        data->stat |= 0x2000;
    }
    effBTLModelAllShow();
    D_0054F380++;
}

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
        printf("nbCameraStart:Read Wait = %d ****\n", D_0054F374);
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

void func_001B34B0(const void *src)
{
    memcpy(D_0056B270, src, 0x58);
}

INCLUDE_RODATA(const s32, "newbattle/nbCameraStartEnd", D_00534630);

