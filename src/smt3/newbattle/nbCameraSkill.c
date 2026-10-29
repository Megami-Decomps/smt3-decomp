#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "fpu.h"
#include "nb.h"
#include "model.h"
#include "motion.h"
#include "event.h"
#include "newdata.h"

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
    f32 half;
    nbCameraAnim_t anim;
} nbCamera_t;


/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

/* The per-shot parameters the nbCameraShot_*_Set2 family reads; 0x30 on the
 * stack at every call site. */
typedef struct nbCameraSkillInfo_t {
    u32 shot_1;
    u32 shot_23;
    f32 angleH;
    f32 angleW;
    f32 HeightMin;
    f32 WidthMin;
    u32 ptr_1;
    u32 ptr23;
    u32 anim;
    u8 pad24[0xC];
} nbCameraSkillInfo_t;


extern char D_00534630[];

extern void nbCameraShot_InvokeParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);

extern sdfVector4_t D_0056B330;

extern sdfVector4_t nbCameraEnemyBadFlag;

extern f32 nbCameraEnemyBadFlagLanes[] asm("nbCameraEnemyBadFlag");

extern f32 D_0056B330Lanes[] asm("D_0056B330");

extern f32 nbCameraEnemyBadStep;

extern f32 nbCameraEnemyBadTarget;

extern void nbCameraState_SkillAttack_Init(void);

extern void nbCameraShot_Invoke_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);

extern void nbCameraShot_TargetParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);

extern void nbCameraShot_PullOne_Init(nbCamera_t *pNbCamera);

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern void nbCameraShot_GroundCollision(nbCamera_t *pNbCamera);

extern nbCamera_t pNbCamera;

extern void effBTLSkillSetFormationInfo(u32 InvokeID, u32 TargetBit);

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void effBTLModelAllShow(void);

extern void nbCameraShot_All_Set(nbCamera_t *pNbCamera);

extern void nbCameraShot_All_Set2(nbCamera_t *pNbCamera, void *pNBSInfo);

extern u128 nbCameraAnalyzeVec;

extern f32 nbCamera_Analyze_Distance;

extern f32 nbCamera_Analyze_Rate;

extern f32 nbCamera_Analyze_Rate_Add;

extern u32 nbCamera_Analyze_Frame;

extern u32 nbCamera_Analyze_Wait;

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

typedef struct nbCameraSkillPtr_t {
    u32 unk00[7];
} nbCameraSkillPtr_t;

extern u32 dds3DrawFlag;
extern f32 nbCameraModelMotionCr;
extern u32 nbCameraSkillAttackStep;
extern u32 effBTLSkillGetTargetNumber(void);
extern u32 effBTLSkillGetInvokeParty(void);
extern u32 effBTLSkillGetTargetParty(void);
extern void effBTLModelGroupPopPosition(u32 group);
extern void effBTLModelAllPopPosition(void);
extern s32 nbCameraBoss_Cut1_Init(nbCamera_t *pNbCamera);
extern s32 nbCameraBoss_Cut2_Run(nbCamera_t *pNbCamera);
extern s32 nbCameraBoss_Cut2_Ptr_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern s32 nbCameraDANTE_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern s32 nbCameraHero_Cut1_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern s32 nbCameraHero_Cut2_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_One_Init(nbCamera_t *pNbCamera, nbFormation_t *pFormation);
extern void nbCameraShot_Summon_Set(nbCamera_t *pNbCamera);
extern void nbCameraShot_AttackTuibi_Set(nbCamera_t *pNbCamera);
extern void nbCameraShot_AttackTuibiReverse_Set(nbCamera_t *pNbCamera);
extern void nbCameraShot_InvokeTargetParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraState_Analyze_Calc(nbCamera_t *pNbCamera);
extern void nbCameraState_SkillAttack_Init(void);

extern nbCameraSkillPtr_t D_00320EB0[];
extern nbCameraSkillInfo_t nbCameraSkillNBSInfo;
extern void nbCamera_SkillInfo_Set(nbCameraSkillInfo_t *pInfo, nbCameraSkillPtr_t *pPtr);
extern u32 effBTLSkillGetID(void);
extern void nbCameraSetState(u16 state);
extern u8 dds3EffectBase[];
extern u32 dds3GetRandA(u8 *name);
extern void effBTLModelAllHide(void);
extern void effBTLModelTargetPartyShow(void);
extern void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);
extern s32 effBTLModelGetMarkPositionEvalTm(nbFormation_t *pFormation, s32 mark);
extern s32 effBTLModelCheckBossID(nbFormation_t *pFormation);
extern void mdlGetPosition(mdlObj_t *aHandle);
extern void effBTLModelInvokeGroupHide(void);
extern f32 sdfAtan(f32 aX);
extern f32 EulerAngleAxisY(void);
extern void SetRotateMatrixXYZ(sdfVector4_t *aAngles);
extern s32 nbCameraBoss_Cut1_Run(nbCamera_t *pNbCamera);
extern s32 nbCameraHero_Cut1_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraState_Cut1Enemy_Calc(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void effBTLModelInvokeShow(void);
extern void effBTLModelLook2(nbFormation_t *pFormation, sdfVector4_t *pTargetPos, f32 angle);
extern void SetRotateMatrixY(f32 aAngle);

typedef struct nbCamera_AllTable_t {
    f32 distance;
    f32 up;
    f32 radius;
} nbCamera_AllTable_t;

extern nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation);
extern void effBTLModelAllShow(void);
extern void effBTLModelTargetGroupHide(void);
extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);
extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);
extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);
extern s32 nbCameraBoss_CheckRotate(nbFormation_t *pFormation);
extern void SetRotateMatrixX(f32 aAngle);
extern void SetRotateMatrixY_B(f32 aAngle);
extern void MultMatrixRevNoArg(void);
extern f32 effBTLModelGetRadius(nbFormation_t *pFormation);
extern void effBTLModelGetDirection(nbFormation_t *pFormation, sdfVector4_t *pDirection);
extern void nbCameraState_SkillAttack_Calc(void);
extern void nbCameraShot_One_Calc(nbCamera_t *pNbCamera, nbFormation_t *pFormation);
extern u128 nbCameraCut1EnemyVec;
extern f32 nbCamera_Cut1Enemy_Distance;
extern f32 nbCamera_Cut1Enemy_Rate;
extern s32 nbCamera_Cut1Enemy_Frame;

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCamera_SkillInfo_Set);

/* vu0 routine: project the camera ray to the floor and rebuild its direction. */
void nbCameraShot_GroundCollision(nbCamera_t *pNbCamera)
{
    sdfVector4_t point;
    sdfVector4_t horizontal;
    volatile sdfVector4_t scale;
    f32 height;
    f32 distance;
    f32 scale2;

    distance = pNbCamera->distance;
    scale.z = -distance;
    scale.x = -distance;
    scale.y = -distance;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pNbCamera);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&point);
    if (point.y > -20.0f) {
        SDF_VU0_LOADVECTOR(pNbCamera);
        SDF_VU0_SETVECTOR_Y(0.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&point);
        SDF_VU0_SETVECTOR_Y(0.0f);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&horizontal);
        height = pNbCamera->target.y - -20.0f;
        scale2 = fsqrtf(height * height + distance * distance);
        point.x = -horizontal.x * scale2;
        point.y = height;
        point.z = -horizontal.z * scale2;
        SDF_VU0_LOADVECTOR(&point);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
}

extern f32 effBTLModelGroupGetSphere(u32 group);
extern void effBTLModelGroupGetDirection(u32 group, sdfVector4_t *pDirection);

void nbCameraShot_TargetParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t direction;
    sdfVector4_t angles;
    sdfVector4_t pos;
    sdfVector4_t neg;
    u32 group;
    f32 radius;
    f32 fov;
    f32 half;
    f32 scale;
    f32 length;

    effBTLModelAllHide();
    effBTLModelTargetPartyShow();
    group = effBTLSkillGetTargetParty();
    pNbCamera->fovy = 0.69813163f;
    radius = effBTLModelGroupGetSphere(group);
    SDF_VU0_STOREVECTOR(pNbCamera);
    fov = pNbCamera->fovy;
    if (effBTLSkillGetTargetNumber() >= 2) {
        fov *= 1.35f;
    } else {
        fov *= 1.2f;
    }
    half = 0.5f;
    scale = sdfAtan(fov * half);
    if (pNBSInfo->HeightMin != 0.0f) {
        pNbCamera->target.y = -pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->HeightMin) {
        radius = pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->WidthMin) {
        radius = pNBSInfo->WidthMin * half;
    }
    radius /= scale;
    effBTLModelGroupGetDirection(group, &direction);
    if (direction.z > 0.0f) {
        angles.x = -pNBSInfo->angleH;
        angles.y = -pNBSInfo->angleW;
    } else {
        angles.x = pNBSInfo->angleH;
        angles.y = pNBSInfo->angleW;
    }
    angles.z = 0.0f;
    SetRotateMatrixXYZ(&angles);
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&direction);
    pNbCamera->distance = radius;
    pNbCamera->direction.x = -direction.x;
    pNbCamera->direction.y = -direction.y;
    pNbCamera->direction.z = -direction.z;
    scale = -radius;
    neg.x = scale;
    neg.y = scale;
    neg.z = scale;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&neg);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pNbCamera);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    if (pos.y > 0.0f) {
        radius *= 1.15f;
        fov = pNbCamera->target.y;
        fov *= fov;
        fov += radius * radius;
        fov = fsqrtf(fov);
        effBTLModelGroupGetDirection(group, &direction);
        pos.x = direction.x * fov;
        pos.y = direction.y * fov;
        pos.z = direction.z * fov;
        SetRotateMatrixY(pNBSInfo->angleW);
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&pos);
        pos.x += pNbCamera->target.x;
        pos.z += pNbCamera->target.z;
        SDF_VU0_LOADVECTOR(pNbCamera);
        SDF_VU0_LOADVECTOR_B(&pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_GETLENGTH(length);
        pNbCamera->distance = length * 0.85f;
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_InvokeTargetParty_Set2);

void nbCameraShot_Invoke_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t direction;
    sdfVector4_t angles;
    nbFormation_t *pForm;
    f32 radius;
    f32 scale;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    pForm = effBTLSkillGetInvokeFormation();
    pNbCamera->fovy = 0.69813163f;
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pNbCamera);
    radius = effBTLModelGetRadius(pForm);
    scale = sdfAtan(pNbCamera->fovy * 1.35f * 0.5f);
    if (pNBSInfo->HeightMin != 0.0f) {
        pNbCamera->target.y = -pNBSInfo->HeightMin * 0.5f;
    }
    if (radius + radius < pNBSInfo->HeightMin) {
        radius = pNBSInfo->HeightMin * 0.5f;
    }
    if (radius + radius < pNBSInfo->WidthMin) {
        radius = pNBSInfo->WidthMin * 0.5f;
    }
    radius /= scale;
    effBTLModelGetDirection(pForm, &direction);
    if (direction.z > 0.0f) {
        angles.x = -pNBSInfo->angleH;
        angles.y = -pNBSInfo->angleW;
    } else {
        angles.x = pNBSInfo->angleH;
        angles.y = pNBSInfo->angleW;
    }
    angles.z = 0.0f;
    SetRotateMatrixXYZ(&angles);
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&direction);
    pNbCamera->distance = radius;
    pNbCamera->direction.x = -direction.x;
    pNbCamera->direction.y = -direction.y;
    pNbCamera->direction.z = -direction.z;
}

void nbCameraShot_Target_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t direction;
    sdfVector4_t angles;
    sdfVector4_t pos;
    sdfVector4_t neg;
    nbFormation_t *pForm;
    f32 radius;
    f32 scale;
    f32 half;
    f32 length;

    effBTLModelAllHide();
    effBTLModelTargetPartyShow();
    pForm = effBTLSkillGetTargetFormation(0);
    pNbCamera->fovy = 0.69813163f;
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pNbCamera);
    half = 0.5f;
    radius = effBTLModelGetRadius(pForm);
    scale = sdfAtan(pNbCamera->fovy * 1.35f * half);
    if (pNBSInfo->HeightMin != 0.0f) {
        pNbCamera->target.y = -pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->HeightMin) {
        radius = pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->WidthMin) {
        radius = pNBSInfo->WidthMin * half;
    }
    half = radius / scale;
    effBTLModelGetDirection(pForm, &direction);
    if (direction.z > 0.0f) {
        angles.x = -pNBSInfo->angleH;
        angles.y = -pNBSInfo->angleW;
    } else {
        angles.x = pNBSInfo->angleH;
        angles.y = pNBSInfo->angleW;
    }
    angles.z = 0.0f;
    SetRotateMatrixXYZ(&angles);
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&direction);
    pNbCamera->distance = half;
    pNbCamera->direction.x = -direction.x;
    pNbCamera->direction.y = -direction.y;
    pNbCamera->direction.z = -direction.z;
    scale = -half;
    neg.x = scale;
    neg.y = scale;
    neg.z = scale;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&neg);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pNbCamera);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    if (pos.y > 0.0f) {
        half *= 1.15f;
        radius = pNbCamera->target.y;
        radius *= radius;
        radius += half * half;
        radius = fsqrtf(radius);
        effBTLModelGetDirection(pForm, &direction);
        pos.x = direction.x * radius;
        pos.y = direction.y * radius;
        pos.z = direction.z * radius;
        SetRotateMatrixY(pNBSInfo->angleW);
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&pos);
        pos.x += pNbCamera->target.x;
        pos.z += pNbCamera->target.z;
        SDF_VU0_LOADVECTOR(pNbCamera);
        SDF_VU0_LOADVECTOR_B(&pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_GETLENGTH(length);
        pNbCamera->distance = length * 0.85f;
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
}

extern f32 sdfAtan2(f32 aY, f32 aX);
extern void nbCameraClearState(nbCamera_t *pCamera);
extern void nbCameraAnim_RotateSet(nbCamera_t *pNbCamera, s32 frame, s32 rotate, f32 start_f);

void nbCameraShot_One_Init(nbCamera_t *pCamera, nbFormation_t *pForm)
{
    sdfVector4_t targetPos;
    sdfVector4_t formPos;
    sdfVector4_t delta;
    sdfVector4_t angles;
    nbCamera_t *pBoss;
    nbFormation_t *pTarget;
    u32 invokeParty;
    u32 targetParty;
    f32 scale;

    effBTLModelAllHide();
    effBTLModelInvokeGroupHide();
    invokeParty = effBTLSkillGetInvokeParty();
    targetParty = effBTLSkillGetTargetParty();
    pCamera->fovy = 0.69813163f;
    scale = sdfAtan(0.34906582f);
    angles.x = 0.0f;
    angles.y = 0.0f;
    angles.z = 0.0f;
    pCamera->distance = pForm->oneshotr * pForm->scl / scale;
    if (effBTLSkillGetTargetNumber() == 1 && invokeParty != targetParty) {
        pTarget = effBTLSkillGetTargetFormation(0);
        effBTLModelGetSphereCenter(pTarget);
        SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
        effBTLModelGetSphereCenter(pForm);
        SDF_VU0_STOREVECTOR_NOMEM(&formPos);
        delta.x = targetPos.x - formPos.x;
        delta.z = targetPos.z - formPos.z;
        angles.y = sdfAtan2(delta.x, delta.z);
    } else {
        SDF_VU0_LOADVECTOR(&pForm->rot);
        angles.y = EulerAngleAxisY();
    }
    switch (dds3GetRandA(dds3EffectBase) & 3) {
    case 0:
        angles.x = 0.26179934f;
        break;
    case 1:
        angles.x = -0.26179934f;
        break;
    case 2:
        angles.x = 0.08726646f;
        break;
    case 3:
        angles.x = -0.08726646f;
        break;
    }
    pBoss = &pNbCamera;
    nbCameraClearState(pBoss);
    switch (dds3GetRandA(dds3EffectBase) & 3) {
    case 0:
        nbCameraAnim_RotateSet(pBoss, 60, -20, 1.0471974f);
        break;
    case 1:
        nbCameraAnim_RotateSet(pBoss, 60, -20, -1.0471974f);
        break;
    case 2:
        nbCameraAnim_RotateSet(pBoss, 60, 0, -0.26179934f);
        angles.y += 0.34906582f;
        break;
    case 3:
        nbCameraAnim_RotateSet(pBoss, 60, 0, 0.26179934f);
        angles.y -= 0.34906582f;
        break;
    }
    SetRotateMatrixXYZ(&angles);
    pCamera->direction.x = 0.0f;
    pCamera->direction.y = 0.0f;
    pCamera->direction.z = -1.0f;
    pCamera->direction.w = 0.0f;
    SDF_VU0_LOADVECTOR(&pCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_NOMEM(&pCamera->direction);
}

void nbCameraShot_One_Calc(nbCamera_t *pNbCamera, nbFormation_t *pFormation)
{
    if (effBTLModelGetMarkPositionEvalTm(pFormation, 1)) {
        SDF_VU0_COPYVECTOR_A_TO_B();
        mdlGetPosition(pFormation->hunit->owner);
        SDF_VU0_SWAPVECTOR_A_B();
        SDF_VU0_SUBVECTOR();
        SDF_VU0_LOADVECTOR_B(&pFormation->pos);
        SDF_VU0_ADDVECTOR();
    } else {
        effBTLModelGetSphereCenter(pFormation);
    }
    SDF_VU0_STOREVECTOR(pNbCamera);
}

/* NON_MATCHING: 2 of 58 words differ (first at +0x74). */
#ifdef NON_MATCHING
void nbCameraShot_PullOne_Init(nbCamera_t *pNbCamera)
{
    sdfVector4_t angles;
    nbFormation_t *pForm;
    f32 fovy;
    f32 width;

    effBTLModelAllHide();
    effBTLModelInvokeGroupHide();
    pForm = effBTLSkillGetInvokeFormation();
    pNbCamera->fovy = 0.69813163f;
    fovy = sdfAtan(0.78539807f);
    if (pForm->r > pForm->h) {
        width = pForm->r * pForm->scl;
    } else {
        width = pForm->h * pForm->scl;
    }
    pNbCamera->distance = width / fovy;
    angles.z = 0.0f;
    angles.x = 0.0f;
    SDF_VU0_LOADVECTOR(&pForm->rot);
    angles.y = -EulerAngleAxisY();
    SetRotateMatrixXYZ(&angles);
    pNbCamera->direction.x = 0.0f;
    pNbCamera->direction.z = -1.0f;
    pNbCamera->direction.y = 0.0f;
    pNbCamera->direction.w = 0.0f;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_PullOne_Init);
#endif

void nbCameraShot_PullOne_Calc(nbCamera_t *pNbCamera, f32 rate)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();

    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->target.y -= pForm->h * pForm->scl * 0.5f * rate;
    nbCameraShot_GroundCollision(pNbCamera);
}

void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t invokePos;
    sdfVector4_t targetPos;
    sdfVector4_t mid;
    sdfVector4_t look;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    f32 width;
    f32 radius;
    f32 length;
    f32 angle;
    f32 half;
    f32 clamp;

    effBTLModelTargetPartyShow();
    effBTLModelInvokeGroupHide();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    width = nbCameraGetModelAllTable(pInvoke)->radius;
    if (width == 0.0f) {
        width = pInvoke->r;
    }
    clamp = pInvoke->scl;
    radius = width * clamp;
    clamp = 100.0f;
    if (clamp < radius) {
        clamp = clamp / radius;
        radius *= clamp;
    }
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    invokePos.y -= pInvoke->h * pInvoke->scl * 0.25f;
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    SDF_VU0_STOREVECTOR_NOMEM(&look);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR_NOMEM(&mid);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    if (targetPos.x > invokePos.x) {
        angle = -sdfAtan(radius * 1.7f / length);
    } else {
        angle = sdfAtan(radius * 1.7f / length);
    }
    SetRotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&mid);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_STOREVECTOR_NOMEM(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
    length += radius * 2.5f / sdfAtan(0.4712388f);
    pNbCamera->distance = length;
    pNbCamera->fovy = 0.69813163f;
    if (nbCameraBoss_CheckRotate(pInvoke) != 0) {
        effBTLModelLook(pInvoke, &look);
    }
    if (nbCameraBoss_CheckRotate(pTarget) != 0) {
        effBTLModelLook2(pTarget, &invokePos, 0.5235987f);
    }
}

void nbCameraShot_InvokeTargetReverse_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t targetPos;
    sdfVector4_t invokePos;
    sdfVector4_t mid;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    f32 width;
    f32 radius;
    f32 length;
    f32 angle;
    f32 half;
    f32 fovy;
    f32 clamp;

    effBTLModelAllShow();
    effBTLModelTargetGroupHide();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    width = nbCameraGetModelAllTable(pTarget)->radius;
    if (width == 0.0f) {
        width = pTarget->r;
    }
    clamp = pTarget->scl;
    radius = width * clamp;
    clamp = 100.0f;
    if (clamp < radius) {
        clamp = clamp / radius;
        radius *= clamp;
    }
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    targetPos.y -= pTarget->h * pTarget->scl * 0.25f;
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR_NOMEM(&mid);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    if (invokePos.x > targetPos.x) {
        angle = -sdfAtan(radius * 1.7f / length);
    } else {
        angle = sdfAtan(radius * 1.7f / length);
    }
    SetRotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&mid);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_STOREVECTOR_NOMEM(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
    fovy = 0.69813163f;
    length += radius * 2.5f / sdfAtan(0.34906582f);
    pNbCamera->fovy = fovy;
    pNbCamera->distance = length;
    if (nbCameraBoss_CheckRotate(pTarget) != 0) {
        effBTLModelLook2(pTarget, &invokePos, 0.5235987f);
    }
    if (nbCameraBoss_CheckRotate(pInvoke) != 0) {
        effBTLModelLook(pInvoke, &targetPos);
    }
}

void nbCameraShot_InvokeTargetNear_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t invokePos;
    sdfVector4_t targetPos;
    sdfVector4_t direction;
    sdfVector4_t look;
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    f32 length;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    length += pInvoke->r * pInvoke->scl + pTarget->r * pTarget->scl;
    length *= 0.5f;
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    VU0_LERP_VF10_VF11(0.4f);
    SDF_VU0_STOREVECTOR_NOMEM(pNbCamera);
    targetPos.y = invokePos.y;
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&direction);
    if (direction.z > 0.0f) {
        SetRotateMatrixX(-0.5235987f);
    } else {
        SetRotateMatrixX(0.5235987f);
    }
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        SetRotateMatrixY_B(-0.69813163f);
    } else {
        SetRotateMatrixY_B(0.69813163f);
    }
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
    pNbCamera->distance = length / sdfAtan(0.418878943f);
    pNbCamera->fovy = 0.69813163f;
    if (nbCameraBoss_CheckRotate(pTarget) != 0) {
        effBTLModelGetSphereCenter(pInvoke);
        SDF_VU0_STOREVECTOR_NOMEM(&look);
        effBTLModelLook(pTarget, &look);
    }
}

void nbCameraShot_InvokeTargetNearReverse_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t targetPos;
    sdfVector4_t invokePos;
    sdfVector4_t direction;
    sdfVector4_t look;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    f32 height;
    f32 length;

    effBTLModelAllShow();
    effBTLModelTargetGroupHide();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    length += pTarget->r * pTarget->scl + pInvoke->r * pInvoke->scl;
    length *= 0.5f;
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    VU0_LERP_VF10_VF11(0.65f);
    SDF_VU0_STOREVECTOR_NOMEM(pNbCamera);
    height = effBTLModelGetHeight(pInvoke);
    height -= effBTLModelGetHeight(pTarget);
    if (height < 250.0f) {
        invokePos.y = targetPos.y;
    }
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&direction);
    if (direction.z > 0.0f) {
        SetRotateMatrixX(-0.5235987f);
    } else {
        SetRotateMatrixX(0.5235987f);
    }
    if (pInvoke->orgpos.x > pTarget->orgpos.x) {
        SetRotateMatrixY_B(-0.69813163f);
    } else {
        SetRotateMatrixY_B(0.69813163f);
    }
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
    pNbCamera->distance = length / sdfAtan(0.418878943f);
    pNbCamera->fovy = 0.69813163f;
    if (nbCameraBoss_CheckRotate(pTarget) != 0) {
        effBTLModelGetSphereCenter(pInvoke);
        SDF_VU0_STOREVECTOR_NOMEM(&look);
        effBTLModelLook(pTarget, &look);
    }
}

extern void effBTLGetGroupPos(nbFormation_t *pFormation, f32 *pPos);

void nbCameraShot_InvokeTargetPartyTop_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t p;
    sdfVector4_t targetPos;
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u32 targetParty;
    u32 i;
    u32 stat;
    u32 count;
    f32 radius;
    f32 height;
    f32 maxHeight;

    pInvoke = effBTLSkillGetInvokeFormation();
    targetParty = effBTLSkillGetTargetParty();
    radius = effBTLModelGroupGetSphere(targetParty);
    radius = radius + radius;
    SDF_VU0_STOREVECTOR(&p);
    if (radius > 1400.0f) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }
    if (effBTLModelGetHeight(pInvoke) <= 150.0f) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }

    maxHeight = 0.0f;
    pTarget = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++, pTarget++) {
        stat = pTarget->stat;
        if ((stat & targetParty) != 0) {
            if ((stat & 1) != 0) {
                if ((stat & 2) != 0) {
                    height = effBTLModelGetHeight(pTarget);
                    if (height > maxHeight)
                        maxHeight = height;
                }
            }
        }
    }
    if (maxHeight > 500.0f) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }
    if (dds3GetRandA(dds3EffectBase) % 5 == 1) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    if (pInvoke->stat & 0x100) {
        if (pInvoke->orgpos.x > 0.0f) {
            pNbCamera->target.x = -411.9f;
            pNbCamera->target.y = -594.4f;
            pNbCamera->target.z = -832.9f;
            pNbCamera->direction.x = 0.2716f;
            pNbCamera->direction.y = 0.4355f;
            pNbCamera->direction.z = 0.8583f;
        } else {
            pNbCamera->target.x = 493.3f;
            pNbCamera->target.y = -577.5f;
            pNbCamera->target.z = -703.7f;
            pNbCamera->direction.x = -0.3424f;
            pNbCamera->direction.y = 0.416f;
            pNbCamera->direction.z = 0.8425f;
        }
    } else if (pInvoke->orgpos.x > 0.0f) {
        pNbCamera->target.x = -387.7f;
        pNbCamera->target.y = -765.3f;
        pNbCamera->target.z = 660.5f;
        pNbCamera->direction.x = 0.247f;
        pNbCamera->direction.y = 0.5918f;
        pNbCamera->direction.z = -0.7673f;
    } else {
        pNbCamera->target.x = 435.9f;
        pNbCamera->target.y = -773.1f;
        pNbCamera->target.z = 628.2f;
        pNbCamera->direction.x = -0.3459f;
        pNbCamera->direction.y = 0.5932f;
        pNbCamera->direction.z = -0.727f;
    }
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.6981316209f;
    if (nbCameraBoss_CheckRotate(pInvoke))
        effBTLModelLook(pInvoke, &p);

    effBTLGetGroupPos(pInvoke, (f32 *)&targetPos);
    count = effBTLSkillGetTargetNumber();
    for (i = 0; i < count; i++) {
        pTarget = effBTLSkillGetTargetFormation(i);
        if (nbCameraBoss_CheckRotate(pTarget) && (pTarget->stat & 3) == 3 &&
            pTarget != pInvoke)
            effBTLModelLook2(pTarget, &targetPos, 0.523598671f);
    }
    return;
}

void nbCameraShot_InvokeTargetPartyTop2_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t p;
    sdfVector4_t targetPos;
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u32 targetParty;
    u32 i;
    u32 stat;
    u32 count;
    f32 radius;
    f32 height;
    f32 maxHeight;

    pInvoke = effBTLSkillGetInvokeFormation();
    targetParty = effBTLSkillGetTargetParty();
    radius = effBTLModelGroupGetSphere(targetParty);
    radius = radius + radius;
    SDF_VU0_STOREVECTOR(&p);
    if (radius > 1400.0f) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }

    maxHeight = 0.0f;
    pTarget = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++, pTarget++) {
        stat = pTarget->stat;
        if ((stat & targetParty) != 0) {
            if ((stat & 1) != 0) {
                if ((stat & 2) != 0) {
                    height = effBTLModelGetHeight(pTarget);
                    if (height > maxHeight)
                        maxHeight = height;
                }
            }
        }
    }
    if (maxHeight > 500.0f) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }
    if ((pInvoke->stat & 0x100) != 0 && dds3GetRandA(dds3EffectBase) % 5 == 1) {
        nbCameraShot_InvokeTargetParty_Set2(pNbCamera, pNBSInfo);
        return;
    }

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    if (pInvoke->stat & 0x100) {
        if (pInvoke->orgpos.x > 0.0f) {
            pNbCamera->target.x = 625.5f;
            pNbCamera->target.y = -831.9f;
            pNbCamera->target.z = -1403.1f;
            pNbCamera->direction.x = -0.2574f;
            pNbCamera->direction.y = 0.4964f;
            pNbCamera->direction.z = 0.8291f;
        } else {
            pNbCamera->target.x = -964.8f;
            pNbCamera->target.y = -578.3f;
            pNbCamera->target.z = -1059.2f;
            pNbCamera->direction.x = 0.5428f;
            pNbCamera->direction.y = 0.377f;
            pNbCamera->direction.z = 0.7505f;
        }
    } else if (pInvoke->orgpos.x > 0.0f) {
        pNbCamera->target.x = 176.2f;
        pNbCamera->target.y = -834.1f;
        pNbCamera->target.z = -1360.7f;
        pNbCamera->direction.x = -0.1092f;
        pNbCamera->direction.y = 0.4678f;
        pNbCamera->direction.z = 0.877f;
    } else {
        pNbCamera->target.x = -235.0f;
        pNbCamera->target.y = -875.9f;
        pNbCamera->target.z = -1381.6f;
        pNbCamera->direction.x = 0.1615f;
        pNbCamera->direction.y = 0.551f;
        pNbCamera->direction.z = 0.8187f;
    }
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.6981316209f;
    if (nbCameraBoss_CheckRotate(pInvoke))
        effBTLModelLook(pInvoke, &p);

    effBTLGetGroupPos(pInvoke, (f32 *)&targetPos);
    count = effBTLSkillGetTargetNumber();
    for (i = 0; i < count; i++) {
        pTarget = effBTLSkillGetTargetFormation(i);
        if (nbCameraBoss_CheckRotate(pTarget) && (pTarget->stat & 3) == 3 && pTarget != pInvoke &&
            nbCameraBoss_CheckRotate(pTarget))
            effBTLModelLook2(pTarget, &targetPos, 0.523598671f);
    }
}

extern void effBTLModelInvokePartyShow(void);

void nbCameraShot_InvokeParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t pos;
    sdfVector4_t direction;
    sdfVector4_t angles;
    u32 group;
    f32 radius;
    f32 half;
    f32 scale;
    f32 zero;
    f32 length;

    effBTLModelAllHide();
    effBTLModelInvokePartyShow();
    group = effBTLSkillGetInvokeParty();
    pNbCamera->fovy = 0.69813163f;
    radius = effBTLModelGroupGetSphere(group);
    SDF_VU0_STOREVECTOR(pNbCamera);
    half = 0.5f;
    zero = 0.0f;
    scale = sdfAtan(pNbCamera->fovy * 1.35f * half);
    if (pNBSInfo->HeightMin != zero) {
        pNbCamera->target.y = -pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->HeightMin) {
        radius = pNBSInfo->HeightMin * half;
    }
    if (radius + radius < pNBSInfo->WidthMin) {
        radius = pNBSInfo->WidthMin * half;
    }
    half = radius / scale;
    if (pNBSInfo->angleH > zero) {
        half *= 1.15f;
        radius = pNbCamera->target.y;
        radius *= radius;
        radius += half * half;
        radius = fsqrtf(radius);
        effBTLModelGroupGetDirection(group, &direction);
        pos.x = direction.x * radius;
        pos.y = direction.y * radius;
        pos.z = direction.z * radius;
        SetRotateMatrixY(pNBSInfo->angleW);
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&pos);
        pos.x += pNbCamera->target.x;
        pos.z += pNbCamera->target.z;
        SDF_VU0_LOADVECTOR(pNbCamera);
        SDF_VU0_LOADVECTOR_B(&pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_GETLENGTH(length);
        pNbCamera->distance = length * 0.85f;
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    } else {
        effBTLModelGroupGetDirection(group, &direction);
        if (direction.z > zero) {
            angles.x = -0.056723193f;
        } else {
            angles.x = 0.056723193f;
        }
        angles.y = pNBSInfo->angleW;
        angles.z = 0.0f;
        SetRotateMatrixXYZ(&angles);
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&direction);
        pNbCamera->distance = half;
        pNbCamera->direction.x = -direction.x;
        pNbCamera->direction.y = -direction.y;
        pNbCamera->direction.z = -direction.z;
    }
}

extern f32 nbGetEnemyInterval(void);
extern f32 nbCameraGetBossDistance(void);
extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);

/* NON_MATCHING: 109 of 125 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void nbCameraShot_All_Set2(nbCamera_t *pNbCamera, void *pNBSInfo)
{
    sdfVector4_t pos;
    nbFormation_t *pForm;
    f32 maxPlayerHeight;
    f32 maxEnemyHeight;
    f32 maxPlayerRadius;
    f32 total;
    f32 value;
    f32 interval;
    f32 height;
    f32 width;
    f32 radius;
    u32 i = 0;
    u32 enemies = 0;

    effBTLModelAllShow();
    pForm = nbGetMainProcessData()->form;
    maxPlayerHeight = 0.0f;
    maxEnemyHeight = maxPlayerHeight;
    maxPlayerRadius = maxPlayerHeight;
    total = maxPlayerHeight;
    for (; i < 32; i++) {
        if ((pForm->stat & 0x201) == 0x201) {
            width = pForm->r * pForm->scl;
            total += width + width;
            height = effBTLModelGetHeight(pForm);
            if (maxEnemyHeight < height) {
                maxEnemyHeight = height;
            }
            enemies++;
        } else if ((pForm->stat & 0x101) == 0x101) {
            height = effBTLModelGetHeight(pForm);
            if (maxPlayerHeight < height) {
                maxPlayerHeight = height;
            }
            width = pForm->r * pForm->scl;
            if (maxPlayerRadius < width) {
                maxPlayerRadius = width;
            }
        }
        pForm++;
    }
    interval = nbGetEnemyInterval();
    if (enemies >= 2) {
        total += interval * (enemies - 1);
    }
    if (total < maxEnemyHeight) {
        total = maxEnemyHeight;
    }
    pNbCamera->target.x = 0.0f;
    pNbCamera->target.y = -(maxEnemyHeight * 0.25f);
    value = 0.5f;
    pNbCamera->target.z = nbCameraGetBossDistance() * value;
    maxEnemyHeight = sdfAtan(0.5235987f);
    pNbCamera->fovy = 0.69813163f;
    pos.x = 0.0f;
    pos.y = -maxPlayerHeight;
    pos.z = -nbCameraGetBossDistance() * value;
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(value);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    radius = effBTLModelGroupGetSphere(0x100);
    if (radius < 250.0f) {
        radius = 250.0f;
    }
    pNbCamera->distance = value + radius / maxEnemyHeight;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_All_Set2);
#endif


INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraShot_All_Set3);

void nbCameraState_Analyze_Set(nbCamera_t *pNbCamera)
{
    SDF_VU0_COPYVECTOR(&nbCameraAnalyzeVec, pNbCamera);
    nbCamera_Analyze_Distance = pNbCamera->distance;
    nbCamera_Analyze_Rate = 1.0f;
    nbCamera_Analyze_Rate_Add = -0.01f;
    nbCamera_Analyze_Frame = 1;
    nbCamera_Analyze_Wait = 5;
}

#ifdef NON_MATCHING
extern s32 effBTLModelGetID(nbFormation_t *pFormation);
extern void effBTLModelInvokePartyHide(void);

void nbCameraState_Analyze_Calc(nbCamera_t *pNbCamera)
{
    nbFormation_t *target = effBTLSkillGetTargetFormation(0);
    sdfVector4_t scale;
    sdfVector4_t focus;
    s32 id = effBTLModelGetID(target);
    f32 rate;
    f32 distance;
    f32 length;
    f32 base;
    f32 want;

    switch (id) {
    case 0x102: case 0x103: case 0x108:
    case 0x123: case 0x124: case 0x125:
        effBTLModelInvokePartyHide();
        return;
    }
    rate = nbCamera_Analyze_Rate + nbCamera_Analyze_Rate_Add;
    if (rate > 0.215f) {
        nbCamera_Analyze_Rate_Add *= 1.1f;
    } else {
        nbCamera_Analyze_Rate_Add *= 0.725f;
    }
    nbCamera_Analyze_Rate = rate < 0.0f ? 0.0f : rate;
    distance = -pNbCamera->distance;
    scale.x = distance;
    scale.y = distance;
    scale.z = distance;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&pNbCamera->target);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&focus);
    effBTLModelGetMarkPosition2(target, 1);
    SDF_VU0_LOADVECTOR_B(&nbCameraAnalyzeVec);
    VU0_LERP_VF10_VF11(nbCamera_Analyze_Rate);
    SDF_VU0_STOREVECTOR(&pNbCamera->target);
    SDF_VU0_LOADVECTOR_B(&focus);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_GETLENGTH(length);
    pNbCamera->distance = length;
    if (0.001f < nbCamera_Analyze_Rate) {
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
    pNbCamera->fovy = 0.69813163f;
    length = sdfAtan(0.34906582f);
    if (id == 0x82 || id == 0x11F) {
        base = target->oneshotr * target->scl * 1.25f / length;
    } else {
        base = target->oneshotr * target->scl / length;
    }
    want = base + (nbCamera_Analyze_Distance - base) * nbCamera_Analyze_Rate;
    pNbCamera->distance = want;
    if (want < base) {
        pNbCamera->distance = base;
    }
    nbCamera_Analyze_Frame++;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_Analyze_Calc);
#endif

void nbCameraShot_All_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
    pNbCamera->target.x = -195.3f;
    pNbCamera->target.y = -321.2f;
    pNbCamera->target.z = -884.7f;
    pNbCamera->direction.x = 0.2207f;
    pNbCamera->direction.y = 0.2273f;
    pNbCamera->direction.z = 0.9485f;
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
}

/* NON_MATCHING: 188 of 194 words match at native 776-byte size. */
#ifdef NON_MATCHING
void bCameraShot_AttackInvokeTargetPartyNear_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t basePos;
    sdfVector4_t cameraPos;
    sdfVector4_t center;
    nbFormation_t *invoke;
    nbFormation_t *target;
    nbFormation_t *form;
    nbFormation_t *lowest;
    u32 invokeParty;
    u32 i;
    s32 first;
    f32 groupRadius;
    f32 lowestHeight;
    f32 radius;
    f32 span;
    f32 length;
    f32 radiusAngle;
    f32 boundAngle;
    f32 distance;

    effBTLModelAllShow();
    effBTLModelAllPopRotate();
    invoke = effBTLSkillGetInvokeFormation();
    target = effBTLSkillGetTargetFormation(0);
    invokeParty = effBTLSkillGetInvokeParty();
    groupRadius = effBTLModelGroupGetSphere(invokeParty);
    SDF_VU0_STOREVECTOR_NOMEM(&cameraPos);

    first = 1;
    form = nbGetMainProcessData()->form;
    lowest = NULL;
    lowestHeight = 0.0f;
    for (i = 0; i < 32; i++, form++) {
        u32 flags = form->stat;
        f32 height;

        if ((flags & invokeParty) == 0) {
            continue;
        }
        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 2) == 0) {
            continue;
        }
        height = effBTLModelGetHeight(form);
        if (first != 0) {
            lowestHeight = height;
            lowest = form;
            first = 0;
        } else if (height < lowestHeight) {
            lowestHeight = height;
            lowest = form;
        }
    }

    if (lowest != NULL) {
        cameraPos.y = -(lowestHeight * 0.5f);
        effBTLModelGetSphereCenter(lowest);
        SDF_VU0_STOREVECTOR_NOMEM(&center);
        cameraPos.y = (cameraPos.y + center.y) * 0.5f;
    }

    effBTLModelGetSphereCenter(target);
    SDF_VU0_STOREVECTOR_NOMEM(&basePos);
    basePos.y = -effBTLModelGetHeight(target);
    radius = target->r * target->scl;

    SDF_VU0_LOADVECTOR(&cameraPos);
    SDF_VU0_LOADVECTOR_B(&basePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    radiusAngle = sdfAtan(0.34906582f);
    if (150.0f < radius) {
        distance = radius / radiusAngle;
    } else if (90.0f < radius) {
        distance = (radius * 1.5f) / radiusAngle;
    } else {
        distance = (radius + radius) / radiusAngle;
    }

    /* Keep the combined extent live across the next atan call. */
    span = length + distance;
    boundAngle = sdfAtan(0.4712388f);
    if (span < groupRadius / boundAngle) {
        span *= boundAngle;
        effBTLModelGetSphereCenter(invoke);
        SDF_VU0_STOREVECTOR_NOMEM(&center);
        if (cameraPos.x > center.x) {
            cameraPos.x = cameraPos.x - (groupRadius - span);
        } else {
            cameraPos.x = cameraPos.x + (groupRadius - span);
        }
    }

    SDF_VU0_LOADVECTOR(&cameraPos);
    SDF_VU0_STOREVECTOR_NOMEM(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&basePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
    pNbCamera->fovy = 0.6981316209f;
    pNbCamera->distance += distance * 1.05f;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", bCameraShot_AttackInvokeTargetPartyNear_Set);
#endif

extern f32 sdfSin(f32 aRadius);
extern f32 fabsf(f32 x);
extern void effBTLModelTargetShow(void);

void nbCameraShot_AttackTuibi_Set(nbCamera_t *pNbCamera)
{
    sdfVector4_t targetPos;
    sdfVector4_t invokePos;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    f32 angle;
    f32 half;
    f32 length;
    f32 dist;
    f32 height;
    u32 invokeParty;

    invokeParty = effBTLSkillGetInvokeParty();
    if (invokeParty != effBTLSkillGetTargetParty()) {
        effBTLModelAllShow();
        effBTLModelInvokeGroupHide();
    } else {
        effBTLModelAllHide();
        effBTLModelInvokeShow();
        effBTLModelTargetShow();
    }
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    pNbCamera->fovy = 0.69813163f;
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    invokePos.y -= pInvoke->r * pInvoke->scl * 0.5f;
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    if (pNbCamera->frame == 0) {
        if (invokePos.x > targetPos.x) {
            pNbCamera->angle = 0.5235987f;
        } else {
            pNbCamera->angle = -0.5235987f;
        }
    }
    angle = pNbCamera->angle;
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    length += pInvoke->r * pInvoke->scl;
    dist = length + pTarget->r * pTarget->scl;
    SetRotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    dist *= sdfSin(fabsf(angle));
    half = 0.5f;
    dist *= 1.15f;
    height = effBTLModelGetHeight(pInvoke) - pInvoke->r * pInvoke->scl * half;
    if (dist < height) {
        dist = height;
    }
    height = effBTLModelGetHeight(pTarget) - pTarget->r * pInvoke->scl * half;
    if (dist < height) {
        dist = height;
    }
    pNbCamera->distance = dist * half / sdfAtan(pNbCamera->fovy * half);
}

void nbCameraShot_AttackTuibiReverse_Set(nbCamera_t *pNbCamera)
{
    sdfVector4_t invokePos;
    sdfVector4_t targetPos;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    f32 angle;
    f32 half;
    f32 length;
    f32 dist;
    f32 height;

    effBTLModelAllShow();
    effBTLModelTargetGroupHide();
    effBTLSkillGetInvokeParty();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    if (pNbCamera->frame == 0) {
        if (pTarget->pos.x > pInvoke->pos.x) {
            pNbCamera->angle = 0.5235987f;
        } else {
            pNbCamera->angle = -0.5235987f;
        }
    }
    pNbCamera->fovy = 0.69813163f;
    angle = pNbCamera->angle;
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&targetPos);
    targetPos.y -= pTarget->r * pTarget->scl * 0.5f;
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR_NOMEM(&invokePos);
    invokePos.y -= pInvoke->r * pInvoke->scl * 0.5f;
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_LOADVECTOR_B(&invokePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    length += pTarget->r * pTarget->scl;
    dist = length + pInvoke->r * pInvoke->scl;
    SetRotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&invokePos);
    SDF_VU0_LOADVECTOR_B(&targetPos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    dist *= sdfSin(fabsf(angle));
    half = 0.5f;
    dist *= 1.15f;
    height = effBTLModelGetHeight(pTarget) - pTarget->r * pTarget->scl * half;
    if (dist < height) {
        dist = height;
    }
    height = effBTLModelGetHeight(pInvoke) - pInvoke->r * pTarget->scl * half;
    if (dist < height) {
        dist = height;
    }
    pNbCamera->distance = dist * half / sdfAtan(pNbCamera->fovy * half);
}

extern s32 nbCameraBoss_Cut2_Init(nbCamera_t *pNbCamera);
extern void nbCameraState_InvokeTarget_SearchSet(void);
extern s32 fptodp(f32 value);
extern s32 printf(const char *format, ...);
extern f32 D_003246D0[];
extern f32 D_003246F0[];
extern f32 D_00324710[];
extern f32 D_00324720[];

/* NON_MATCHING: 20 of 137 words differ (first at +0x168). */
#ifdef NON_MATCHING
void nbCameraState_SkillAttack_Init(void)
{
    sdfVector4_t look;
    u32 ptr[8];
    nbCameraSkillInfo_t work;
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u32 invokeParty;
    u32 targetParty;
    f32 width;

    if (pCamera->frame == 0) {
        dds3DrawFlag |= 0x2000000;
    }
    if (effBTLModelCheckBossID(effBTLSkillGetInvokeFormation()) != 0) {
        if (nbCameraBoss_Cut2_Init(pCamera) != 0) {
            return;
        }
    }
    effBTLModelAllPopPosition();
    if (effBTLSkillGetTargetNumber() == 1) {
        pInvoke = effBTLSkillGetInvokeFormation();
        pTarget = effBTLSkillGetTargetFormation(0);
        invokeParty = effBTLSkillGetInvokeParty();
        targetParty = effBTLSkillGetTargetParty();
        if ((pInvoke->stat & 0x200) != 0 && invokeParty != targetParty) {
            ptr[2] = dds3GetRandA(dds3EffectBase) % 18;
            ptr[3] = dds3GetRandA(dds3EffectBase) % 14;
            work.angleH = D_003246D0[ptr[2]];
            work.angleW = D_003246F0[ptr[2]];
            work.HeightMin = D_00324710[0];
            work.WidthMin = D_00324720[0];
            ptr[4] = 0;
            ptr[5] = 0;
            nbCameraState_InvokeTarget_SearchSet();
            width = effBTLModelGroupGetSphere(0x200);
            width += width;
            if (width < 1500.0f) {
                bCameraShot_AttackInvokeTargetPartyNear_Set(pCamera, &work);
                nbCameraSkillAttackStep = 0xB;
            } else {
                printf("nbCameraState_SkillAttack : %f < NBCAMERA_ATTACK_MOVE_WIDTH_MAX\n", fptodp(width));
                nbCameraShot_InvokeTargetReverse_Set2(pCamera, &work);
                nbCameraSkillAttackStep = 0xB;
            }
        } else {
            nbCameraSkillAttackStep = 0;
        }
        if (invokeParty != targetParty) {
            if (nbCameraBoss_CheckRotate(pTarget) != 0) {
                effBTLModelGetSphereCenter(pInvoke);
                SDF_VU0_STOREVECTOR(&look);
                effBTLModelLook(pTarget, &look);
            }
        }
    } else {
        effBTLModelAllPopPosition();
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillAttack_Init);
#endif

void nbCameraState_SkillAttack_Calc(void)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    nbCamera_t *pCamera;
    u32 invokeParty;
    u32 targetParty;

    if (effBTLModelCheckBossID(pForm) != 0) {
        if (nbCameraBoss_Cut2_Run(&pNbCamera) != 0) {
            return;
        }
    }
    pCamera = &pNbCamera;
    if (pCamera->frame == 0) {
        nbCameraModelMotionCr = pForm->cr * pForm->scl;
    }
    if (effBTLSkillGetTargetNumber() == 1) {
        pForm = effBTLSkillGetInvokeFormation();
        invokeParty = effBTLSkillGetInvokeParty();
        targetParty = effBTLSkillGetTargetParty();
        if ((pForm->stat & 0x200) != 0 && invokeParty != targetParty) {
            if (nbCameraSkillAttackStep == 0) {
                if (nbCameraModelMotionCr < 450.0f) {
                    nbCameraShot_AttackTuibiReverse_Set(pCamera);
                }
            } else {
                nbCameraSkillAttackStep--;
                if (nbCameraSkillAttackStep == 0) {
                    effBTLModelGroupPopPosition(targetParty);
                    pCamera->frame = 0;
                    if (nbCameraModelMotionCr < 450.0f) {
                        nbCameraShot_AttackTuibiReverse_Set(pCamera);
                    } else {
                        nbCameraShot_InvokeTargetReverse_Set2(pCamera, &work);
                    }
                }
            }
        } else {
            nbCameraShot_AttackTuibi_Set(&pNbCamera);
        }
    } else {
        nbCameraShot_InvokeTargetParty_Set2(pCamera, &work);
    }
    nbCameraShot_GroundCollision(&pNbCamera);
}

/* NON_MATCHING: 1 of 30 words differ (first at +0x48). */
#ifdef NON_MATCHING
void nbCameraState_SkillStart_Init(void)
{
    datNormalSkillVisual_t *pVisual = datSkillTable.visual;

    nbCamera_SkillInfo_Set(&nbCameraSkillNBSInfo, &D_00320EB0[effBTLSkillGetID()]);
    pVisual += effBTLSkillGetID();
    if (pVisual->hatudo == 0) {
        nbCameraSetState(7);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraSkill", nbCameraState_SkillStart_Init);
#endif

void nbCameraState_Cut1Enemy_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    nbCameraSkillInfo_t work;
    f32 angle;
    f32 half;

    work.HeightMin = 0.0f;
    work.WidthMin = 600.0f;
    angle = 1.0f;
    if ((dds3GetRandA(dds3EffectBase) & 1) == 0) {
        angle = -1.0f;
    }
    work.angleH = angle;
    work.angleW = 0.0f;
    nbCameraShot_InvokeParty_Set2(pNbCamera, &work);
    SDF_VU0_COPYVECTOR(&nbCameraCut1EnemyVec, pNbCamera);
    nbCamera_Cut1Enemy_Distance = pNbCamera->distance;
    effBTLModelGetMarkPosition2(effBTLSkillGetInvokeFormation(), 1);
    SDF_VU0_LOADVECTOR_B(&nbCameraCut1EnemyVec);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    nbCamera_Cut1Enemy_Rate = 1.0f;
    nbCamera_Cut1Enemy_Frame = 0;
}

void nbCameraState_Cut1Enemy_Calc(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    sdfVector4_t scale;
    sdfVector4_t focus;
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 distance;
    f32 length;
    f32 base;
    f32 want;

    if (nbCamera_Cut1Enemy_Frame >= 0) {
        nbCamera_Cut1Enemy_Rate *= 0.88f;
        distance = -pNbCamera->distance;
        scale.x = distance;
        scale.y = distance;
        scale.z = distance;
        SDF_VU0_LOADVECTOR(&pNbCamera->direction);
        SDF_VU0_LOADVECTOR_B(&scale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(pNbCamera);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&focus);
        effBTLModelGetMarkPosition2(pForm, 1);
        SDF_VU0_LOADVECTOR_B(&nbCameraCut1EnemyVec);
        VU0_LERP_VF10_VF11(nbCamera_Cut1Enemy_Rate);
        SDF_VU0_STOREVECTOR(pNbCamera);
        SDF_VU0_LOADVECTOR_B(&focus);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_GETLENGTH(length);
        pNbCamera->distance = length;
        if (0.001f < nbCamera_Cut1Enemy_Rate) {
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&pNbCamera->direction);
        }
        pNbCamera->fovy = 0.69813163f;
        base = pForm->oneshotr * pForm->scl * 1.25f / sdfAtan(0.34906582f);
        want = base + (nbCamera_Cut1Enemy_Distance - base) * nbCamera_Cut1Enemy_Rate;
        pNbCamera->distance = want;
        if (want < base) {
            pNbCamera->distance = base;
        }
    }
    nbCamera_Cut1Enemy_Frame++;
}

void nbCameraState_SkillCut1_Init(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pForm;

    if (pCamera->frame == 0) {
        dds3DrawFlag |= 0x2000000;
    }
    effBTLModelAllPopRotate();
    effBTLModelAllPopPosition();
    pForm = effBTLSkillGetInvokeFormation();
    if (effBTLModelCheckBossID(pForm) != 0) {
        if (nbCameraBoss_Cut1_Init(&pNbCamera) != 0) {
            return;
        }
    }
    if (nbCameraHero_Cut1_Init(&pNbCamera, &nbCameraSkillNBSInfo) != 0) {
        return;
    }
    switch (nbCameraSkillNBSInfo.shot_1) {
    case 0:
        if ((pForm->stat & 0x100) != 0) {
            nbCameraShot_One_Init(&pNbCamera, pForm);
        } else {
            nbCameraState_Cut1Enemy_Init(&pNbCamera, &nbCameraSkillNBSInfo);
        }
        break;
    case 2:
        nbCameraState_SkillAttack_Init();
        break;
    case 3:
        nbCameraShot_InvokeTargetReverse_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
        break;
    case 4:
        nbCameraShot_TargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
        break;
    case 8:
        nbCameraShot_Summon_Set(&pNbCamera);
        break;
    case 10:
        nbCameraState_SkillAttack_Init();
        break;
    }
}

void nbCameraState_SkillCut1_Calc(void)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();

    if (effBTLModelCheckBossID(pForm) != 0) {
        if (nbCameraBoss_Cut1_Run(&pNbCamera) != 0) {
            return;
        }
    }
    if (nbCameraHero_Cut1_Run(&pNbCamera, &nbCameraSkillNBSInfo) != 0) {
        return;
    }
    if (nbCameraSkillNBSInfo.shot_1 != 2) {
        if (nbCameraSkillNBSInfo.shot_1 < 3) {
            if (nbCameraSkillNBSInfo.shot_1 == 0) {
                if ((pForm->stat & 0x100) != 0) {
                    nbCameraShot_One_Calc(&pNbCamera, effBTLSkillGetInvokeFormation());
                } else {
                    nbCameraState_Cut1Enemy_Calc(&pNbCamera, &nbCameraSkillNBSInfo);
                }
            }
        }
    } else {
        nbCameraState_SkillAttack_Calc();
    }
    nbCameraShot_GroundCollision(&pNbCamera);
}

extern char D_00534A20[];
extern s32 nbCameraHero_Cut2_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern s32 nbCameraDANTE_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern s32 nbCameraBoss_Cut2_Ptr_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_AllParty_Set(nbCamera_t *pNbCamera);
extern void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_InvokeTargetPartyTop_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_InvokeTargetPartyTop2_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_InvokeTargetNear_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern void nbCameraShot_InvokeTargetNearReverse_Set(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);
extern u32 effBTLModelGetGroupNumber(u32 group);

INCLUDE_RODATA(const s32, "newbattle/nbCameraSkill", D_00534A20);

void nbCameraState_SkillCut2_Init(void)
{
    u32 skillID = effBTLSkillGetID();

    nbCamera_SkillInfo_Set(&nbCameraSkillNBSInfo, &D_00320EB0[skillID]);
    if (effBTLSkillGetTargetNumber() == 0) {
        if (nbCameraSkillNBSInfo.shot_23 == 19) {
            nbCameraShot_Invoke_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
        } else {
            printf(D_00534A20);
            nbCameraClearState(&pNbCamera);
            nbCameraShot_All_Set(&pNbCamera);
        }
        return;
    }

    {
        nbFormation_t *invoke = effBTLSkillGetInvokeFormation();

        nbCameraClearState(&pNbCamera);
        if (effBTLModelCheckBossID(invoke) != 0) {
            if (nbCameraBoss_Cut2_Init(&pNbCamera) != 0) {
                return;
            }
        }
        if (nbCameraHero_Cut2_Init(&pNbCamera, &nbCameraSkillNBSInfo) != 0) {
            return;
        }
        if (nbCameraDANTE_Init(&pNbCamera, &nbCameraSkillNBSInfo) != 0) {
            return;
        }
        if (nbCameraBoss_Cut2_Ptr_Init(&pNbCamera, &nbCameraSkillNBSInfo) != 0) {
            return;
        }

        nbCameraModelMotionCr = invoke->cr * invoke->scl;
    }

    {
        u32 invokeParty = effBTLSkillGetInvokeParty();
        u32 targetParty = effBTLSkillGetTargetParty();

        if (invokeParty == targetParty) {
            switch (nbCameraSkillNBSInfo.shot_23) {
            case 13:
                nbCameraShot_Target_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 17:
                nbCameraShot_All_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 18:
                nbCameraShot_AllParty_Set(&pNbCamera);
                break;
            case 19:
                nbCameraShot_Invoke_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 20:
                nbCameraShot_TargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            default:
                if (effBTLSkillGetTargetNumber() < 2) {
                    u32 groupCount = effBTLModelGetGroupNumber(targetParty);
                    if (targetParty == 0x200 && groupCount >= 2) {
                        nbCameraShot_TargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                    } else {
                        nbCameraShot_Target_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                    }
                } else {
                    nbCameraShot_TargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                }
                break;
            }
        } else if (effBTLSkillGetTargetNumber() >= 2) {
            switch (nbCameraSkillNBSInfo.shot_23) {
            case 1:
                nbCameraShot_InvokeTargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 2:
                nbCameraShot_TargetParty_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 5:
                nbCameraShot_InvokeTargetPartyTop_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 6:
                nbCameraShot_InvokeTargetPartyTop2_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 18:
                nbCameraShot_AllParty_Set(&pNbCamera);
                break;
            case 19:
                nbCameraShot_Invoke_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            default:
                break;
            }
        } else {
            switch (nbCameraSkillNBSInfo.shot_23) {
            case 1:
                nbCameraShot_InvokeTarget_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 5:
                nbCameraShot_InvokeTargetPartyTop_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 6:
                nbCameraShot_InvokeTargetPartyTop2_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 7:
                nbCameraShot_InvokeTargetReverse_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 14: {
                nbCamera_t *camera = &pNbCamera;
                nbCameraShot_InvokeTarget_Set2(camera, &nbCameraSkillNBSInfo);
                nbCameraState_Analyze_Set(camera);
                break;
            }
            case 2:
            case 13:
                nbCameraShot_Target_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            case 15:
                if (nbCameraModelMotionCr < 450.0f) {
                    nbCameraShot_InvokeTargetNear_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                } else {
                    nbCameraShot_AttackTuibi_Set(&pNbCamera);
                }
                break;
            case 16:
                effBTLModelGroupPopPosition(targetParty);
                if (nbCameraModelMotionCr < 450.0f) {
                    nbCameraShot_InvokeTargetNearReverse_Set(&pNbCamera, &nbCameraSkillNBSInfo);
                } else {
                    nbCameraShot_InvokeTargetReverse_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                }
                break;
            case 18:
                nbCameraShot_AllParty_Set(&pNbCamera);
                break;
            case 19:
                nbCameraShot_Invoke_Set2(&pNbCamera, &nbCameraSkillNBSInfo);
                break;
            default:
                break;
            }
        }
    }
}

void nbCameraState_SkillCut2_Calc(void)
{
    nbCamera_t *pCamera;

    if (effBTLModelCheckBossID(effBTLSkillGetInvokeFormation()) != 0) {
        if (nbCameraBoss_Cut2_Run(&pNbCamera) != 0) {
            return;
        }
    }
    pCamera = &pNbCamera;
    if (nbCameraHero_Cut2_Run(pCamera, &nbCameraSkillNBSInfo) != 0) {
        return;
    }
    if (nbCameraDANTE_Run(pCamera, &nbCameraSkillNBSInfo) != 0) {
        return;
    }
    if (nbCameraBoss_Cut2_Ptr_Run(pCamera, &nbCameraSkillNBSInfo) != 0) {
        return;
    }
    switch (nbCameraSkillNBSInfo.shot_23) {
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        nbCameraState_SkillAttack_Calc();
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    case 14:
        nbCameraState_Analyze_Calc(&pNbCamera);
        break;
    }
    nbCameraShot_GroundCollision(&pNbCamera);
}

void func_001B8018(void) {
}

void func_001B8020(void) {
}

void nbCameraState_PlayerSide_Init(void)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    u32 i = 0;
    u32 invoke = 0;
    u32 bits = 0;
    f32 angle;
    f32 half;

    for (; i < 32; i++) {
        if ((pForm->stat & 0x103) == 0x103) {
            invoke = i;
            bits |= 1 << invoke;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(invoke, bits);
    effBTLModelAllHide();
    angle = 0.26179934f;
    effBTLModelTargetPartyShow();
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    work.HeightMin = 0.0f;
    work.WidthMin = 700.0f;
    work.angleH = angle;
    switch (dds3GetRandA(dds3EffectBase) & 3) {
    case 0:
    case 1:
        work.angleW = 0.0f;
        break;
    case 2:
        work.angleW = -0.26179934f;
        break;
    case 3:
        work.angleW = angle;
        break;
    }
    nbCameraShot_TargetParty_Set2(&pNbCamera, &work);
}

void nbCameraState_EnemySide_Init(void)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    u32 i = 0;
    u32 invoke = 0;
    u32 bits = 0;
    f32 angle;
    f32 half;

    for (; i < 32; i++) {
        if ((pForm->stat & 0x203) == 0x203) {
            invoke = i;
            bits |= 1 << invoke;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(invoke, bits);
    effBTLModelAllHide();
    angle = 0.26179934f;
    effBTLModelTargetPartyShow();
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    work.HeightMin = 0.0f;
    work.WidthMin = 700.0f;
    work.angleH = angle;
    switch (dds3GetRandA(dds3EffectBase) & 3) {
    case 0:
    case 1:
        work.angleW = 0.0f;
        break;
    case 2:
        work.angleW = -0.26179934f;
        break;
    case 3:
        work.angleW = angle;
        break;
    }
    nbCameraShot_TargetParty_Set2(&pNbCamera, &work);
}

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
    work.HeightMin = 0.0f;
    work.WidthMin = 700.0f;
    nbCameraShot_TargetParty_Set2(&pNbCamera, &work);
}

void nbCameraState_EnemyBad_Init(void)
{
    nbCameraSkillInfo_t work;
    nbMainProcessData_t *pData;
    nbFormation_t *pForm;
    nbCamera_t *pCamera = &pNbCamera;
    u32 invoke = 0;
    u32 bits;
    u32 i;

    pData = nbGetMainProcessData();
    bits = 0;
    pForm = pData->form;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 0x203) == 0x203) {
            invoke = i;
            bits |= 1 << invoke;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(invoke, bits);
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllHide();
    effBTLModelTargetPartyShow();
    work.HeightMin = 0.0f;
    work.WidthMin = 700.0f;
    work.angleH = 0.17453291f;
    work.angleW = 0.0f;
    nbCameraShot_TargetParty_Set2(pCamera, &work);
    SDF_VU0_COPYVECTOR(&nbCameraEnemyBadFlag, pCamera);
    effBTLModelGetSphereCenter(&pData->form[pData->party[pData->order[pData->orderindex]].formindex]);
    SDF_VU0_STOREVECTOR(&D_0056B330);
    D_0056B330Lanes[1] = nbCameraEnemyBadFlagLanes[1];
    nbCameraEnemyBadTarget = pCamera->distance;
    nbCameraEnemyBadStep = 1.0f;
}

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
