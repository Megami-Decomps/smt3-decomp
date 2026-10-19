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

extern void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pCamera, void *pWork);

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

/* The per-shot parameters the nbCameraShot_*_Set2 family reads; 0x30 on the
 * stack at every call site. */
typedef struct nbCameraSkillInfo_t {
    u32 shot1;
    u32 shot23;
    f32 angleH;
    f32 angleW;
    f32 heightMin;
    f32 widthMin;
    u32 ptr1;
    u32 ptr23;
    u32 anim;
    u8 pad24[0xC];
} nbCameraSkillInfo_t;

extern char D_00534608[];

extern char D_00534630[];

extern void nbCameraShot_InvokeParty_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);

extern u128 D_0056B330;

extern u128 nbCameraEnemyBadFlag;

extern f32 nbCameraEnemyBadStep;

extern f32 nbCameraEnemyBadTarget;

extern void nbCameraState_SkillAttack_Init(void);

extern void nbCameraShot_Invoke_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);

extern void nbCameraShot_TargetParty_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);

extern void nbCameraShot_PullOne_Init(nbCamera_t *pCamera);

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern void nbCameraShot_GroundCollision(nbCamera_t *pCamera);

extern nbCamera_t pNbCamera;

extern void effBTLSkillSetFormationInfo(u32 invokeId, u32 targetBits);

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void effBTLModelAllShow(void);

extern void nbCameraShot_All_Set(nbCamera_t *pCamera);

extern void nbCameraShot_All_Set2(nbCamera_t *pCamera, void *pWork);

extern u128 nbCameraAnalyzeVec;

extern f32 nbCamera_Analyze_Distance;

extern f32 nbCamera_Analyze_Rate;

extern f32 nbCamera_Analyze_Rate_Add;

extern u32 nbCamera_Analyze_Frame;

extern u32 nbCamera_Analyze_Wait;

extern void nbCameraShot_InvokeTargetReverse_Set2(nbCamera_t *pCamera, void *pWork);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCamera_SkillInfo_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_GroundCollision);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_TargetParty_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetParty_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_Invoke_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_Target_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_One_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_One_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_PullOne_Init);

void nbCameraShot_PullOne_Calc(nbCamera_t *pCamera, f32 rate)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();

    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pCamera);
    pCamera->target.y -= pForm->h * pForm->scl * 0.5f * rate;
    nbCameraShot_GroundCollision(pCamera);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTarget_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetReverse_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetNear_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetNearReverse_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetPartyTop_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetPartyTop2_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeParty_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_All_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_All_Set3);

void nbCameraState_Analyze_Set(nbCamera_t *pCamera)
{
    SDF_VU0_COPYVECTOR(&nbCameraAnalyzeVec, pCamera);
    nbCamera_Analyze_Distance = pCamera->distance;
    nbCamera_Analyze_Rate = 1.0f;
    nbCamera_Analyze_Rate_Add = -0.01f;
    nbCamera_Analyze_Frame = 1;
    nbCamera_Analyze_Wait = 5;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_Analyze_Calc);

void nbCameraShot_All_Set(nbCamera_t *pCamera)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
    pCamera->target.x = -195.3f;
    pCamera->target.y = -321.2f;
    pCamera->target.z = -884.7f;
    pCamera->direction.x = 0.2207f;
    pCamera->direction.y = 0.2273f;
    pCamera->direction.z = 0.9485f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", bCameraShot_AttackInvokeTargetPartyNear_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_AttackTuibi_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_AttackTuibiReverse_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillAttack_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillAttack_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillStart_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_Cut1Enemy_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_Cut1Enemy_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillCut1_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillCut1_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillCut2_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillCut2_Calc);

void func_001B8018(void) {
}

void func_001B8020(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_PlayerSide_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_EnemySide_Init);

void nbCameraEnemyPartySet(f32 pitch, f32 yaw)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    u32 i = 0;
    u32 invoke = 0;
    u32 bits = 0;

    for (; i < 32; i++) {
        if ((pForm->stat & 0x203) == 0x203) {
            invoke = i;
            bits |= 1 << invoke;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(invoke, bits);
    work.angleH = pitch;
    work.angleW = yaw;
    work.heightMin = 0.0f;
    work.widthMin = 700.0f;
    nbCameraShot_TargetParty_Set2(&pNbCamera, &work);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_EnemyBad_Init);

void nbCameraState_EnemyBad_Calc(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t focus;
    sdfVector4_t scale;
    f32 distance = -pCamera->distance;

    nbCameraEnemyBadStep *= 0.925f;
    scale.x = distance;
    scale.y = distance;
    scale.z = distance;
    SDF_VU0_LOADVECTOR(&pCamera->direction);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pCamera);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&focus);
    SDF_VU0_LOADVECTOR(&D_0056B330);
    SDF_VU0_LOADVECTOR_B(&nbCameraEnemyBadFlag);
    VU0_LERP_VF10_VF11(nbCameraEnemyBadStep);
    SDF_VU0_STOREVECTOR(pCamera);
    SDF_VU0_LOADVECTOR_B(&focus);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->distance = nbCameraEnemyBadTarget * (nbCameraEnemyBadStep * 0.3f + 0.7f);
}
