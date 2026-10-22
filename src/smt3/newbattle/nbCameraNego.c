#include "common.h"
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

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern s32 effBTLModelGetMarkPosition(nbFormation_t *pFormation, s32 mark);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern s32 effBTLModelGetID(nbFormation_t *pFormation);

extern u128 D_00324740;

extern nbCamera_t pNbCamera;

extern u32 nbCamera_NegoBougai_Wait;

extern f32 nbCamera_NegoHojo_Rate;

extern f32 nbCamera_NegoHojo_Distance;

extern void func_0016E1F8(void);

extern void func_0016E120(void);

extern u128 nbCameraNegoBougaiVec;

extern f32 nbCamera_NegoBougai_Rate;

extern f32 nbCamera_NegoBougai_Distance;

extern u32 nbCamera_NegoBougai_Frame;

void nbCameraState_NegoBougai_Init(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    SDF_VU0_COPYVECTOR(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->distance;
    nbCamera_NegoBougai_Wait = 15;
    nbCamera_NegoBougai_Frame = 0;
}

void nbCameraState_NegoBougai_Init2(void)
{
    nbCamera_t *pCamera = &pNbCamera;

    SDF_VU0_COPYVECTOR(&nbCameraNegoBougaiVec, pCamera);
    nbCamera_NegoBougai_Rate = 1.0f;
    nbCamera_NegoBougai_Distance = pCamera->distance;
    nbCamera_NegoBougai_Wait = 8;
    nbCamera_NegoBougai_Frame = 0;
}

void nbCameraState_NegoBougai_SetWaitFrame(u32 frame)
{
    nbCamera_NegoBougai_Wait = frame;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraNego", nbCameraState_NegoBougai_Calc);

void nbCameraState_NegoTaizi_Init(void)
{
    func_0016E1F8();
    func_0016E120();
}

INCLUDE_ASM(const s32, "newbattle/nbCameraNego", nbCameraState_NegoTaizi_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraNego", nbCameraState_NegoTarget_Init);

void nbCameraState_NegoTarget_Calc(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pForm = effBTLSkillGetTargetFormation(0);

    if ((pForm->stat & 3) == 3 && effBTLModelGetID(pForm) == 0x3E) {
        effBTLModelGetSphereCenter(pForm);
        SDF_VU0_LOADVECTOR_B(&D_00324740);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(pCamera);
    }
}

void nbCameraState_NegoTarget_Calc2(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pForm = effBTLSkillGetTargetFormation(0);

    if ((pForm->stat & 3) == 3) {
        if (effBTLModelGetMarkPosition(pForm, 1) == 0) {
            effBTLModelGetSphereCenter(pForm);
        }
        SDF_VU0_STOREVECTOR(pCamera);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCameraNego", nbCameraState_NegoHojo_Init);

void nbCameraState_NegoHojo_Calc(void)
{
    nbCamera_NegoHojo_Rate = nbCamera_NegoHojo_Rate * 0.925f;
    pNbCamera.distance = nbCamera_NegoHojo_Distance * (1.0f - nbCamera_NegoHojo_Rate);
}
