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

extern u8 dds3EffectBase[];

extern u32 dds3GetRandA(u8 *state);

extern void effBTLModelAllHide(void);

extern s32 effBTLModelGetGroupNumber(u32 mask);

extern void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pCamera, void *pWork);

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

extern s32 nbCameraBoss_BSKAGU_Command_Calc(nbCamera_t *pCamera, nbFormation_t *pForm);

extern s32 nbCameraBoss_BSLUSI_Command_Calc(nbCamera_t *pCamera, nbFormation_t *pForm);

extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);

extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pCamera);

extern u32 effBTLSkillGetTargetNumber(void);

extern void effBTLGetGroupPos(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern f32 D_0054F3A0[];

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

extern void effBTLModelInvokeShow(void);

extern f32 effBTLModelGroupGetSphere(u32 mask);

extern void effBTLModelGroupGetDirection(u32 mask, f32 *pDirection);

extern f32 sdfAtan(f32 x);

extern s32 nbCameraBossCutFlag;

extern s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pCamera);

extern u32 effBTLSkillGetTargetNumber(void);

extern void effBTLGetGroupPos(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern f32 D_0054F3A0[];

extern void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);

extern u128 Bsnet_E907_2_EndTarget;

extern u128 Bsnet_E907_2_EndDirection;

extern f32 Bsnet_E907_2_StartDistance;

extern f32 Bsnet_E907_2_EndDistance;

extern f32 Bsnet_E907_2_Rate;

extern u128 D_0056B420;

extern u128 D_0056B430;

extern void nbCameraState_SkillAttack_Init(void);

extern void nbCameraShot_Invoke_Set2(nbCamera_t *pCamera, nbCameraSkillInfo_t *pWork);

extern s32 nbCameraHero_Cut2_Run(nbCamera_t *pCamera, void *pWork);

extern sdfVector4_t nbCameraBSARICut1Vec;

extern f32 BSARI_Cut1_Rate;

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 index);

extern s32 effBTLModelGetID(nbFormation_t *pFormation);

extern void nbCameraShot_GroundCollision(nbCamera_t *pCamera);

extern datUnitVisual_t datModelParamTable[];

extern nbCamera_t pNbCamera;

extern u32 nbCameraBoss_BSBERU_E915_Frame;

extern f32 nbCameraBoss_BSBERU_E915_Rate;

extern u32 Foru_E913_Frame;

extern void effBTLModelAllShow(void);

extern u32 nbCameraBoss_BSARI_MASEHA_Frame;

extern s32 nbCameraBoss_BSARI_MASEHA_Type;

extern void nbResetAllFormPosRot(void);

extern void func_0016E120(void);

extern void nbCameraShot_All_Set2(nbCamera_t *pCamera, void *pWork);

extern s32 dds3GetRandIntA(s32 low, s32 high);

extern f32 nbCameraBoss_BSALBI_E912_Rate;

extern f32 nbCameraBoss_BSALBI_E912_Distance;

extern s32 nbCameraBoss_BSARI_SIMANO_Type;

extern f32 nbCameraBoss_BSARI_SIMANO_Rate;

extern u8 nbCameraBoss_Pish_MUHENK_Type;

extern u32 nbCameraBoss_Pish_MUHENK_Frame;

extern void nbCameraShot_InvokeTargetReverse_Set2(nbCamera_t *pCamera, void *pWork);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

void nbCameraSetState(u16 state);

void nbCameraState_InvokeTarget_SearchSet(void);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_InvokeTarget_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_TargetInvoke_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Foru_SIMON_Set);

void nbCameraBoss_Foru_E913_Set(void)
{
    Foru_E913_Frame = 0;
    pNbCamera.frame = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Foru_E913_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_MURET2_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_MURET2_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_TP_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_TPFix_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_Run);

s32 nbCameraBoss_Sp_JAREIH_Cut1_Set(nbCamera_t *pCamera)
{
    nbResetAllFormPosRot();
    return 0;
}

s32 nbCameraBoss_Sp_JAREIH_Set(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_JAREIH_Run);

s32 nbCameraBoss_Sp_PSYSTL_Set(nbCamera_t *pCamera)
{
    u8 work[0x30];

    effBTLModelAllShow();
    nbCameraShot_InvokeTargetReverse_Set2(pCamera, work);
    return 1;
}

s32 nbCameraBoss_Sp_PSYSTL_Run(nbCamera_t *pCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSMIZ_Command_Set);

s32 nbCameraBoss_BSONGY_Command_Set(nbCamera_t *pCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pBase;
    nbFormation_t *pForm;
    nbFormation_t *p;
    sdfVector4_t pos;
    sdfVector4_t target;
    sdfVector4_t center;
    u32 i;
    f32 depth;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if ((u32)effBTLModelGetGroupNumber(0x200) < 2) {
        if (pData->turn == 0) {
            return 0;
        }
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    pBase = pData->form;
    pForm = &pBase[pData->party[pData->order[pData->orderindex]].formindex];
    target.x = 34.067421f;
    target.y = -180.0f;
    target.z = 235.19156f;
    target.w = 0.0f;
    SDF_VU0_COPYVECTOR(&pCamera->target, &target);
    depth = 700.0f;
    p = pBase;
    for (i = 0; i < 4; i++) {
        if ((p->stat & 3) == 3) {
            effBTLGetGroupPos(p, &pos);
            pos.z -= depth;
            effBTLModelSetPosition(p, &pos);
        }
        p++;
    }
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&target);
    SDF_VU0_LOADVECTOR_B(&center);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    pCamera->distance = 1700.0f;
    effBTLModelLook(pForm, &pCamera->target);
    return 1;
}

s32 nbCameraBoss_Ongy_BUNSIN_Set(nbCamera_t *pCamera)
{
    nbCameraSetState(0);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Ongy_BUNSIN_Run);

s32 nbCameraBoss_BSARUS_Command_Set(nbCamera_t *pCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pForm;
    sdfVector4_t target;
    sdfVector4_t center;
    f32 length;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    pForm = &pData->form[pData->party[pData->order[pData->orderindex]].formindex];
    effBTLModelGroupGetSphere(0x200);
    SDF_VU0_STOREVECTOR(&target);
    target.y = 10.0f;
    SDF_VU0_COPYVECTOR(&pCamera->target, &target);
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&target);
    SDF_VU0_LOADVECTOR_B(&center);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    pCamera->distance = length + 500.0f;
    effBTLModelLook(pForm, &pCamera->target);
    return 1;
}


/* NON_MATCHING: 25 of 115 words; ee-gcc CSEs an address retail recomputes. */
#ifdef NON_MATCHING
s32 nbCameraBoss_BSSAMA_Command_Set(nbCamera_t *pCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pBase;
    nbFormation_t *pForm;
    sdfVector4_t pos;
    sdfVector4_t target;
    sdfVector4_t center;
    sdfVector4_t dir;
    f32 length;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    pBase = &pData->form[4];
    pForm = &pData->form[pData->party[pData->order[pData->orderindex]].formindex];
    effBTLModelGetSphereCenter(pBase);
    SDF_VU0_STOREVECTOR(&target);
    target.y += pBase->h * pBase->scl * 0.35f;
    SDF_VU0_COPYVECTOR(&pCamera->target, &target);
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    pos.x = center.x + dir.x * 550.0f;
    pos.y = center.y + dir.y * 550.0f;
    pos.z = center.z + dir.z * 550.0f;
    if (pos.y > -10.0f) {
        pos.y = -10.0f;
    }
    SDF_VU0_LOADVECTOR(&pCamera->target);
    SDF_VU0_LOADVECTOR_B(&pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    effBTLModelLook(pForm, &pCamera->target);
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSSAMA_Command_Set);
#endif


s32 nbCameraBoss_Samael_AKUI_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    nbResetAllFormPosRot();
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        pCamera->target.x = 799.62f;
        pCamera->target.y = -165.55f;
        pCamera->target.z = 1412.67f;
        pCamera->direction.x = -0.43f;
        pCamera->direction.y = -0.08f;
        pCamera->direction.z = -0.9f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->target.x = -818.13f;
        pCamera->target.y = -95.8f;
        pCamera->target.z = 1316.26f;
        pCamera->direction.x = 0.47f;
        pCamera->direction.y = -0.12f;
        pCamera->direction.z = -0.87f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_Samael_AKUI_Run(nbCamera_t *pCamera)
{
    return 1;
}


/* NON_MATCHING: 4 of 77 words; one global load is scheduled three loads early. */
#ifdef NON_MATCHING
s32 nbCameraBoss_Sukady_DAIJIS_Set(nbCamera_t *pCamera)
{
    u32 type;

    effBTLModelAllShow();
    type = dds3GetRandIntA(0, 3);
    switch (type) {
    case 0:
        pCamera->direction.x = D_0054F3A0[0];
        pCamera->direction.y = 0.29f;
        pCamera->direction.z = 0.96f;
        pCamera->target.x = 11.27f;
        pCamera->target.y = -596.07f;
        pCamera->target.z = -1543.5999f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.55f;
        pCamera->direction.y = 0.19f;
        pCamera->direction.z = 0.81f;
        pCamera->target.x = -944.1399f;
        pCamera->target.y = -454.91f;
        pCamera->target.z = -1265.4899f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.38f;
        pCamera->direction.y = 0.48f;
        pCamera->direction.z = -0.79f;
        pCamera->target.x = -584.87f;
        pCamera->target.y = -902.14f;
        pCamera->target.z = 1002.33f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sukady_DAIJIS_Set);
#endif


s32 nbCameraBoss_Baal_BAELNO_Set(nbCamera_t *pCamera)
{
    u8 work[0x30];

    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        nbCameraShot_InvokeTarget_Set2(pCamera, work);
    } else {
        nbCameraShot_InvokeTargetReverse_Set2(pCamera, work);
    }
    return 1;
}

s32 nbCameraBoss_Baal_BAELNO_Run(nbCamera_t *pCamera)
{
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Set(nbCamera_t *pCamera)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm;

    nbCameraState_SkillAttack_Init();
    pForm = effBTLSkillGetInvokeFormation();
    nbCameraState_InvokeTarget_SearchSet();
    work.angleH = 0.26179934f;
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        work.angleW = 0.5235987f;
    } else {
        work.angleW = -0.5235987f;
    }
    work.heightMin = 0.0f;
    work.widthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pCamera, &work);
    pCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Run(nbCamera_t *pCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pCamera->frame < datModelParamTable[id].part[2].unk18) {
        pCamera->distance += 12.5f;
    }
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Set(nbCamera_t *pCamera)
{
    nbCameraSkillInfo_t work;
    nbFormation_t *pForm;

    nbCameraState_SkillAttack_Init();
    pForm = effBTLSkillGetInvokeFormation();
    nbCameraState_InvokeTarget_SearchSet();
    work.angleH = 0.26179934f;
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        work.angleW = 0.5235987f;
    } else {
        work.angleW = -0.5235987f;
    }
    work.heightMin = 0.0f;
    work.widthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pCamera, &work);
    pCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Run(nbCamera_t *pCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pCamera->frame < datModelParamTable[id].part[3].unk18) {
        pCamera->distance -= 15.0f;
    }
    nbCameraShot_GroundCollision(pCamera);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Bsnet_Attack_Set);

s32 nbCameraBoss_Bsnet_Attack_Run(nbCamera_t *pCamera)
{
    return 1;
}

void nbCameraBoss_BSALBI_E912_Set(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 radius;

    nbCameraSetState(0);
    radius = effBTLModelGroupGetSphere(0x200);
    SDF_VU0_STOREVECTOR(pCamera);
    effBTLModelGroupGetDirection(0x200, &pCamera->direction);
    pCamera->direction.z = -pCamera->direction.z;
    radius /= sdfAtan(0.34906582f);
    nbCameraBoss_BSALBI_E912_Rate = 1.0f;
    pCamera->fovy = 0.69813163f;
    pCamera->distance = radius;
    nbCameraBoss_BSALBI_E912_Distance = radius;
}

void nbCameraBoss_BSALBI_E912_Run(void)
{
    f32 rate = nbCameraBoss_BSALBI_E912_Rate;
    f32 distance = nbCameraBoss_BSALBI_E912_Distance;

    nbCameraBoss_BSALBI_E912_Rate = rate * 0.95f;
    pNbCamera.distance = distance * 0.65f + distance * 0.35f * rate;
}

s32 nbCameraBoss_BSALBI_InvokeTargetParty_Set(nbCamera_t *pCamera)
{
    s32 type;

    effBTLModelAllShow();
    type = dds3GetRandIntA(0, 2);
    switch (type) {
    case 0:
        func_0016E120();
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = 0.36f;
        pCamera->direction.z = -0.88f;
        pCamera->target.x = 510.01996f;
        pCamera->target.y = -852.57f;
        pCamera->target.z = 1895.83f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.03f;
        pCamera->direction.y = 0.17f;
        pCamera->direction.z = 0.98f;
        pCamera->target.x = -62.22f;
        pCamera->target.y = -493.04f;
        pCamera->target.z = -1677.5599f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

void nbCameraBoss_Bsnet_E907_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

void nbCameraBoss_Bsnet_E907_Run(void)
{
    pNbCamera.distance -= 60.0f;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Bsnet_E907_2_Set);

void nbCameraBoss_Bsnet_E907_2_Run(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 rate = Bsnet_E907_2_Rate;
    f32 step = 1.0f - rate;

    Bsnet_E907_2_Rate = rate * 0.925f;
    if (step > 0.0f) {
        if (step > 1.0f) {
            step = 1.0f;
        }
        SDF_VU0_LOADVECTOR(&Bsnet_E907_2_EndTarget);
        SDF_VU0_LOADVECTOR_B(&D_0056B420);
        VU0_LERP_VF10_VF11(step);
        SDF_VU0_STOREVECTOR(pCamera);
        SDF_VU0_LOADVECTOR(&Bsnet_E907_2_EndDirection);
        SDF_VU0_LOADVECTOR_B(&D_0056B430);
        VU0_LERP_VF10_VF11(step);
        SDF_VU0_STOREVECTOR(&pCamera->direction);
        pCamera->distance = Bsnet_E907_2_StartDistance +
                         (Bsnet_E907_2_EndDistance - Bsnet_E907_2_StartDistance) * step;
    }
}

void nbCameraBoss_Bsnet_E905_Set(void)
{
    u8 work[0x30];

    nbCameraShot_All_Set2(&pNbCamera, work);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSARI_Command_Set);

s32 nbCameraBoss_BSARI_Cut1_Set(nbCamera_t *pCamera)
{
    u32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 4);
    switch (type) {
    case 0:
        nbCameraBSARICut1Vec.x = 772.02f;
        nbCameraBSARICut1Vec.y = -310.88f;
        nbCameraBSARICut1Vec.z = -535.27f;
        break;
    case 1:
        nbCameraBSARICut1Vec.x = -762.72f;
        nbCameraBSARICut1Vec.y = -289.89f;
        nbCameraBSARICut1Vec.z = -902.4f;
        break;
    case 2:
        nbCameraBSARICut1Vec.x = 830.26f;
        nbCameraBSARICut1Vec.y = -1282.93f;
        nbCameraBSARICut1Vec.z = -627.39f;
        break;
    case 3:
        nbCameraBSARICut1Vec.x = -848.57f;
        nbCameraBSARICut1Vec.y = -1141.78f;
        nbCameraBSARICut1Vec.z = -801.32f;
        break;
    }
    BSARI_Cut1_Rate = 1.0f;
    return 1;
}

s32 nbCameraBoss_BSARI_Cut1_Run(nbCamera_t *pCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.9f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pCamera);
    pCamera->target.y -= pForm->h * pForm->scl * 0.15f;
    SDF_VU0_LOADVECTOR(pCamera);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut1_Set(nbCamera_t *pCamera)
{
    s32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 2);
    switch (type) {
    case 0:
        nbCameraBSARICut1Vec.x = 772.02f;
        nbCameraBSARICut1Vec.y = -310.88f;
        nbCameraBSARICut1Vec.z = -535.27f;
        break;
    case 1:
        nbCameraBSARICut1Vec.x = -804.65f;
        nbCameraBSARICut1Vec.y = -314.06f;
        nbCameraBSARICut1Vec.z = -799.75f;
        break;
    }
    BSARI_Cut1_Rate = 1.0f;
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut1_Run(nbCamera_t *pCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.985f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetMarkPosition2(pForm, 1);
    SDF_VU0_STOREVECTOR(pCamera);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI_MASEHA_Set(nbCamera_t *pCamera)
{
    nbCameraBoss_BSARI_MASEHA_Frame = 0;
    nbCameraBoss_BSARI_MASEHA_Type = dds3GetRandIntA(0, 3);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSARI_MASEHA_Run);

s32 nbCameraBoss_BSARI_SIMANO_Set(nbCamera_t *pCamera)
{
    nbCameraBoss_BSARI_SIMANO_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_BSARI_SIMANO_Rate = 1.0f;
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSARI_SIMANO_Run);

s32 nbCameraBoss_Bsnet_AISHOH_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pCamera->direction.x = 0.77f;
        pCamera->direction.y = -0.29f;
        pCamera->direction.z = 0.57f;
        pCamera->target.x = -1638.33f;
        pCamera->target.y = -66.21999f;
        pCamera->target.z = -854.07f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.65f;
        pCamera->direction.y = -0.37f;
        pCamera->direction.z = 0.66f;
        pCamera->target.x = -1513.32f;
        pCamera->target.y = 9.739999f;
        pCamera->target.z = -1146.56f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.74f;
        pCamera->direction.y = -0.31f;
        pCamera->direction.z = 0.59f;
        pCamera->target.x = -2002.5799f;
        pCamera->target.y = -34.919995f;
        pCamera->target.z = -1281.7199f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

void nbCameraBoss_Kagu_E909_Set(void)
{
    pNbCamera.frame = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Kagu_E909_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Kagu_Attack_Set);

s32 nbCameraBoss_Kagu_FLEACH_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    if (nbCameraBossCutFlag == 1) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.4f;
            pCamera->direction.y = 0.22f;
            pCamera->direction.z = 0.89f;
            pCamera->target.x = 949.28992f;
            pCamera->target.y = -1372.0599f;
            pCamera->target.z = -2017.89f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.01f;
            pCamera->direction.y = 0.26f;
            pCamera->direction.z = 0.97f;
            pCamera->target.x = -98.7f;
            pCamera->target.y = -1369.1199f;
            pCamera->target.z = -2463.0596f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.34f;
            pCamera->direction.y = 0.07f;
            pCamera->direction.z = 0.94f;
            pCamera->target.x = -1205.4999f;
            pCamera->target.y = -669.11994f;
            pCamera->target.z = -1848.38f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.01f;
            pCamera->direction.y = 0.26f;
            pCamera->direction.z = 0.97f;
            pCamera->target.x = -98.7f;
            pCamera->target.y = -1369.1199f;
            pCamera->target.z = -2463.0596f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Kagu_MJINKO_Set);

s32 nbCameraBoss_Pish_PRESS_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pCamera->direction.x = -0.44f;
        pCamera->direction.y = 0.24f;
        pCamera->direction.z = 0.87f;
        pCamera->target.x = 1621.9799f;
        pCamera->target.y = -1533.9999f;
        pCamera->target.z = -1239.0198f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = -0.7584f;
        pCamera->direction.y = -0.108f;
        pCamera->direction.z = 0.6428f;
        pCamera->target.x = 1769.9f;
        pCamera->target.y = -311.0f;
        pCamera->target.z = -288.7f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.29f;
        pCamera->direction.y = -0.1f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = -739.69f;
        pCamera->target.y = -207.52f;
        pCamera->target.z = -1314.05f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_Pish_MUHENK_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    nbCameraBoss_Pish_MUHENK_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_Pish_MUHENK_Frame = 0;
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Pish_MUHENK_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSKAGU_Command_Calc);

s32 nbCameraBoss_BSKAGU_Command_Set(nbCamera_t *pCamera)
{
    nbMainProcessData_t *pData;
    s32 ret;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    ret = nbCameraBoss_BSKAGU_Command_Calc(pCamera,
        &pData->form[pData->party[pData->order[pData->orderindex]].formindex]);
    pData = NULL;
    return ret;
}

s32 nbCameraBoss_Bskagu_Cut1_Set(nbCamera_t *pCamera)
{
    u32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 4);
    switch (type) {
    case 0:
        pCamera->direction.x = 0.34f;
        pCamera->direction.y = -0.07f;
        pCamera->direction.z = 0.94f;
        pCamera->target.x = -827.22992f;
        pCamera->target.y = -244.35f;
        pCamera->target.z = -861.4f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 1:
        pCamera->direction.x = -0.28f;
        pCamera->direction.y = -0.07f;
        pCamera->direction.z = 0.96f;
        pCamera->target.x = 747.71f;
        pCamera->target.y = -268.25995f;
        pCamera->target.z = -742.54f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 2:
        pCamera->direction.x = -0.26f;
        pCamera->direction.y = 0.42f;
        pCamera->direction.z = 0.87f;
        pCamera->target.x = 899.59f;
        pCamera->target.y = -1686.4f;
        pCamera->target.z = -842.64991f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 3:
        pCamera->direction.x = 0.36f;
        pCamera->direction.y = 0.04f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = -850.21991f;
        pCamera->target.y = -754.96991f;
        pCamera->target.z = -1404.91f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    }
    return 1;
}

s32 nbCameraBoss_Bskagu_Cut1_Run(nbCamera_t *pCamera)
{
    return 1;
}

s32 nbCameraBoss_Bspish_Cut1_Set(nbCamera_t *pCamera)
{
    nbCameraSkillInfo_t work;
    u32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 5);
    switch (type) {
    case 0:
        pCamera->direction.x = 0.3f;
        pCamera->direction.y = -0.15f;
        pCamera->direction.z = 0.94f;
        pCamera->target.x = -1034.8f;
        pCamera->target.y = -314.58997f;
        pCamera->target.z = -1062.9799f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 1:
        pCamera->direction.x = -0.26f;
        pCamera->direction.y = -0.23f;
        pCamera->direction.z = 0.94f;
        pCamera->target.x = 716.63f;
        pCamera->target.y = -257.57f;
        pCamera->target.z = -935.71f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 2:
        pCamera->direction.x = 0.31f;
        pCamera->direction.y = 0.33f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = -1155.65f;
        pCamera->target.y = -1912.3699f;
        pCamera->target.z = -1384.39f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 3:
        pCamera->direction.x = -0.35f;
        pCamera->direction.y = 0.3f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = 1208.52f;
        pCamera->target.y = -1856.16f;
        pCamera->target.z = -1390.7f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 4:
        nbCameraState_SkillAttack_Init();
        effBTLSkillGetInvokeFormation();
        nbCameraState_InvokeTarget_SearchSet();
        work.angleH = 0.26179934f;
        if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
            work.angleW = 0.5235987f;
            nbCameraBossCutFlag = 0;
        } else {
            work.angleW = -0.5235987f;
            nbCameraBossCutFlag = 1;
        }
        work.heightMin = 0.0f;
        work.widthMin = 0.0f;
        nbCameraShot_Invoke_Set2(pCamera, &work);
        nbCameraShot_GroundCollision(pCamera);
        break;
    }
    return 1;
}

s32 nbCameraBoss_Bspish_Cut1_Run(nbCamera_t *pCamera)
{
    return 1;
}

void nbCameraBoss_BSMETA_E914_0_Set(void)
{
    effBTLModelAllShow();
    pNbCamera.direction.x = 0.56f;
    pNbCamera.direction.y = -0.19f;
    pNbCamera.direction.z = 0.81f;
    pNbCamera.target.x = -946.41993f;
    pNbCamera.target.y = -103.86f;
    pNbCamera.target.z = -904.08991f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

void nbCameraBoss_BSMETA_E914_1_Set(void)
{
    effBTLModelAllShow();
    pNbCamera.direction.x = -0.2f;
    pNbCamera.direction.y = 0.29f;
    pNbCamera.direction.z = 0.94f;
    pNbCamera.target.x = 441.29f;
    pNbCamera.target.y = -762.66993f;
    pNbCamera.target.z = -1527.5899f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

s32 nbCameraBoss_Meta_METAT1_0_Set(nbCamera_t *pCamera)
{
    pCamera->direction.x = 0.47f;
    pCamera->direction.y = -0.28f;
    pCamera->direction.z = 0.84f;
    pCamera->target.x = -229.18f;
    pCamera->target.y = -433.18f;
    pCamera->target.z = 261.52997f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_Meta_METAT1_1_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    if (pCamera->frame != 11) {
        return 1;
    }
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pCamera->direction.x = 0.44f;
        pCamera->direction.y = 0.08f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = -733.24f;
        pCamera->target.y = -284.88996f;
        pCamera->target.z = -1329.35f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.39f;
        pCamera->direction.y = 0.32f;
        pCamera->direction.z = -0.87f;
        pCamera->target.x = -602.86994f;
        pCamera->target.y = -541.77f;
        pCamera->target.z = 1101.0899f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        nbCameraBoss_BSARI1_EP_IT_Set(pCamera);
        break;
    }
    return 1;
}

void nbCameraBoss_BSBERU_E915_Init(void)
{
    nbCameraBoss_BSBERU_E915_Frame = 0;
    nbCameraBoss_BSBERU_E915_Rate = 1.0f;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSBERU_E915_0_Set);

void nbCameraBoss_BSBERU_E915_1_Set(void)
{
    pNbCamera.direction.x = -0.27f;
    pNbCamera.direction.y = 0.37f;
    pNbCamera.direction.z = 0.89f;
    pNbCamera.target.x = 361.64997f;
    pNbCamera.target.y = -627.90992f;
    pNbCamera.target.z = -651.73993f;
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.69813163f;
}

void nbCameraBoss_BSBERU_E915_2_Set(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate = nbCameraBoss_BSBERU_E915_Rate;
    f32 blend;

    nbCameraBoss_BSBERU_E915_Rate = rate * 0.99f;
    blend = (s32)nbCameraBoss_BSBERU_E915_Frame / 30.0f;
    from.x = 361.64997f;
    from.y = -627.90992f;
    from.z = -651.73993f;
    to.x = 511.64f;
    to.y = -606.7f;
    to.z = -1581.8299f;
    blend *= rate;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_STOREVECTOR(pCamera);
    from.x = -0.27f;
    from.y = 0.37f;
    from.z = 0.89f;
    to.x = -0.23f;
    to.y = 0.13f;
    to.z = 0.96f;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    nbCameraBoss_BSBERU_E915_Frame++;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Beru_HAENO1_Set);

s32 nbCameraBoss_BSLUSI_Command_Set(nbCamera_t *pCamera)
{
    nbMainProcessData_t *pData;
    s32 ret;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    ret = nbCameraBoss_BSLUSI_Command_Calc(pCamera,
        &pData->form[pData->party[pData->order[pData->orderindex]].formindex]);
    pData = NULL;
    return ret;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSLUSI_Command_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Lusi_Cut1_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Lusi_TWORLD_Set);

s32 nbCameraBoss_Lusi_OHNOOH_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pCamera->direction.x = 0.32f;
        pCamera->direction.y = -0.18f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = -753.05f;
        pCamera->target.y = -83.349991f;
        pCamera->target.z = -1876.56f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
        break;
    case 1:
        pCamera->direction.x = -0.01f;
        pCamera->direction.y = -0.14f;
        pCamera->direction.z = 0.99f;
        pCamera->target.x = 3.8399997f;
        pCamera->target.y = -158.56f;
        pCamera->target.z = -2395.3096f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = -0.42f;
        pCamera->direction.y = -0.1f;
        pCamera->direction.z = 0.9f;
        pCamera->target.x = 929.23993f;
        pCamera->target.y = -230.86f;
        pCamera->target.z = -1871.33f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
        break;
    case 3:
        pCamera->direction.x = -0.22f;
        pCamera->direction.y = 0.52f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = 600.78f;
        pCamera->target.y = -1638.08f;
        pCamera->target.z = -1939.0699f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Lusi_ASIKIK_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraShot_BossTargetInvoke_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraShot_BossEX02_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut1_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut1_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Run);

s32 nbCameraBoss_CheckRotate(nbFormation_t *pForm)
{
    switch (effBTLModelGetID(pForm)) {
    case 0x102:
    case 0x103:
    case 0x108:
    case 0x123:
    case 0x124:
    case 0x125:
    case 0x146:
    case 0x147:
    case 0x148:
    case 0x156:
    case 0x157:
    case 0x158:
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_CheckFix);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Command_Init);

s32 nbCameraBoss_BSARI1_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.26f;
        pCamera->direction.y = 0.33f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = 746.94f;
        pCamera->target.y = -1248.2499f;
        pCamera->target.z = -1527.32f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.62f;
        pCamera->direction.y = 0.3f;
        pCamera->direction.z = 0.73f;
        pCamera->target.x = -1308.1398f;
        pCamera->target.y = -960.32f;
        pCamera->target.z = -1071.01f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI1_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = -0.1f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 740.91993f;
        pCamera->target.y = -251.30999f;
        pCamera->target.z = -1268.8299f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.45f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = -865.67f;
        pCamera->target.y = -294.74f;
        pCamera->target.z = -1501.93f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI1_Cut23_EX03_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pCamera->direction.x = -0.26f;
        pCamera->direction.y = 0.33f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = 746.94f;
        pCamera->target.y = -1248.2499f;
        pCamera->target.z = -1527.32f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.62f;
            pCamera->direction.y = 0.3f;
            pCamera->direction.z = 0.73f;
            pCamera->target.x = -1308.1398f;
            pCamera->target.y = -960.32f;
            pCamera->target.z = -1071.01f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.58f;
            pCamera->direction.y = -0.1f;
            pCamera->direction.z = 0.73f;
            pCamera->target.x = -956.97f;
            pCamera->target.y = -209.06998f;
            pCamera->target.z = -799.32f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
            pCamera->direction.x = -0.44f;
            pCamera->direction.y = -0.18f;
            pCamera->direction.z = 0.88f;
            pCamera->target.x = 1006.2f;
            pCamera->target.y = -104.62999f;
            pCamera->target.z = -914.78f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
        } else {
            pCamera->direction.x = -0.52f;
            pCamera->direction.y = 0.15f;
            pCamera->direction.z = 0.84f;
            pCamera->target.x = 1143.1f;
            pCamera->target.y = -780.76f;
            pCamera->target.z = -962.94f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
        }
    } else {
        if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
            pCamera->direction.x = 0.42f;
            pCamera->direction.y = -0.16f;
            pCamera->direction.z = 0.89f;
            pCamera->target.x = -661.40992f;
            pCamera->target.y = -45.249997f;
            pCamera->target.z = -782.04f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
        } else {
            pCamera->direction.x = 0.68f;
            pCamera->direction.y = 0.11f;
            pCamera->direction.z = 0.72f;
            pCamera->target.x = -1340.8499f;
            pCamera->target.y = -533.69f;
            pCamera->target.z = -711.26f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.32f;
        pCamera->direction.y = 0.08f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 774.63f;
        pCamera->target.y = -461.97f;
        pCamera->target.z = -1267.7199f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.56f;
        pCamera->direction.y = 0.06f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = -1213.39f;
        pCamera->target.y = -383.00995f;
        pCamera->target.z = -1264.4799f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX03_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    func_0016E120();
    pCamera->direction.x = 0.49f;
    pCamera->direction.y = 0.36f;
    pCamera->direction.z = 0.8f;
    pCamera->target.x = -997.73993f;
    pCamera->target.y = -1100.6099f;
    pCamera->target.z = -1215.93f;
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pCamera->direction.x = -0.18f;
        pCamera->direction.y = 0.04f;
        pCamera->direction.z = 0.98f;
        pCamera->target.x = 333.63f;
        pCamera->target.y = -282.11f;
        pCamera->target.z = -1884.7499f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.57f;
        pCamera->direction.y = -0.01f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = -1262.9799f;
        pCamera->target.y = -197.49999f;
        pCamera->target.z = -1548.4999f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.36f;
        pCamera->direction.y = -0.12f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = -658.44f;
        pCamera->target.y = -47.229996f;
        pCamera->target.z = -1726.4599f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = -0.01f;
        pCamera->direction.y = 0.06f;
        pCamera->direction.z = 1.0f;
        pCamera->target.x = -6.07f;
        pCamera->target.y = -366.72f;
        pCamera->target.z = -2008.05f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_EP_IT_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pCamera->direction.x = -0.18f;
        pCamera->direction.y = 0.04f;
        pCamera->direction.z = 0.98f;
        pCamera->target.x = 333.63f;
        pCamera->target.y = -282.11f;
        pCamera->target.z = -1884.7499f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.57f;
        pCamera->direction.y = -0.01f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = -1262.9799f;
        pCamera->target.y = -197.49999f;
        pCamera->target.z = -1548.4999f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.36f;
        pCamera->direction.y = -0.12f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = -658.44f;
        pCamera->target.y = -47.229996f;
        pCamera->target.z = -1726.4599f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = -0.01f;
        pCamera->direction.y = 0.06f;
        pCamera->direction.z = 1.0f;
        pCamera->target.x = -6.07f;
        pCamera->target.y = -366.72f;
        pCamera->target.z = -2008.05f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSNET1_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pCamera->direction.x = -0.28f;
            pCamera->direction.y = 0.3f;
            pCamera->direction.z = 0.91f;
            pCamera->target.x = 1068.39f;
            pCamera->target.y = -1047.2299f;
            pCamera->target.z = -1474.66f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pCamera->direction.x = 0.26f;
            pCamera->direction.y = 0.14f;
            pCamera->direction.z = 0.96f;
            pCamera->target.x = -515.2f;
            pCamera->target.y = -573.76f;
            pCamera->target.z = -1415.0599f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSNET_EP_IT_Set(nbCamera_t *pCamera)
{
    effBTLModelAllShow();
    switch (dds3GetRandIntA(0, 5)) {
    case 0:
        pCamera->direction.x = -0.09f;
        pCamera->direction.y = -0.02f;
        pCamera->direction.z = 1.0f;
        pCamera->target.x = 308.80997f;
        pCamera->target.y = -254.05f;
        pCamera->target.z = -2068.33f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
    case 2:
        pCamera->direction.x = 0.41f;
        pCamera->direction.y = 0.0f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = -793.4f;
        pCamera->target.y = -293.23f;
        pCamera->target.z = -1708.04f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = -0.22f;
        pCamera->direction.y = 0.32f;
        pCamera->direction.z = 0.92f;
        pCamera->target.x = 426.57f;
        pCamera->target.y = -775.52f;
        pCamera->target.z = -1754.41f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 4:
        pCamera->direction.x = 0.19f;
        pCamera->direction.y = 0.39f;
        pCamera->direction.z = 0.9f;
        pCamera->target.x = -330.54f;
        pCamera->target.y = -925.6f;
        pCamera->target.z = -1622.9899f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}


/* NON_MATCHING: 19 of 79 words; the bodies are exact, the selector movz is not. */
#ifdef NON_MATCHING
s32 nbCameraBoss_BSKAGU_EP_IT_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    if (nbCameraBossCutFlag == 1) {
        n = ((dds3GetRandA(dds3EffectBase) & 1) != 0) ? 0 : 3;
    } else {
        n = ((dds3GetRandA(dds3EffectBase) & 1) != 0) ? 1 : 2;
    }
    switch (n) {
    case 0:
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 617.1609f;
        pCamera->target.y = -279.74997f;
        pCamera->target.z = -1764.7099f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = 0.28f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.96f;
        pCamera->target.x = -448.85996f;
        pCamera->target.y = -282.52997f;
        pCamera->target.z = -1788.65f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.47f;
        pCamera->direction.y = -0.11f;
        pCamera->direction.z = 0.87f;
        pCamera->target.x = -870.40992f;
        pCamera->target.y = -34.279995f;
        pCamera->target.z = -1640.9098f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = -0.22f;
        pCamera->direction.y = 0.22f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 475.55f;
        pCamera->target.y = -659.79993f;
        pCamera->target.z = -1966.0599f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSKAGU_EP_IT_Set);
#endif


s32 nbCameraBoss_BSBERU_EP_IT_Set(nbCamera_t *pCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pCamera->direction.x = -0.4f;
        pCamera->direction.y = -0.07f;
        pCamera->direction.z = 0.92f;
        pCamera->target.x = 550.54993f;
        pCamera->target.y = -129.88999f;
        pCamera->target.z = -1359.81f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = -0.33f;
        pCamera->direction.y = 0.22f;
        pCamera->direction.z = 0.92f;
        pCamera->target.x = 534.35993f;
        pCamera->target.y = -549.46f;
        pCamera->target.z = -1439.95f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.41f;
        pCamera->direction.y = -0.07f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = -700.19f;
        pCamera->target.y = -184.47999f;
        pCamera->target.z = -1440.54f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = 0.46f;
        pCamera->direction.y = 0.22f;
        pCamera->direction.z = 0.86f;
        pCamera->target.x = -796.84f;
        pCamera->target.y = -586.89f;
        pCamera->target.z = -1466.5999f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_EP_IT_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u32 n;

    effBTLModelAllShow();
    if (effBTLSkillGetTargetNumber() >= 2) {
        n = dds3GetRandIntA(0, 2);
    } else {
        pInvoke = effBTLSkillGetInvokeFormation();
        pTarget = effBTLSkillGetTargetFormation(0);
        n = (pTarget->orgpos.x <= pInvoke->orgpos.x) ? 1 : 0;
    }
    switch (n) {
    case 0:
        pCamera->direction.x = -0.35f;
        pCamera->direction.y = -0.17f;
        pCamera->direction.z = 0.92f;
        pCamera->target.x = 832.66993f;
        pCamera->target.y = -56.02f;
        pCamera->target.z = -1851.29f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
        break;
    case 1:
        pCamera->direction.x = 0.42f;
        pCamera->direction.y = -0.19f;
        pCamera->direction.z = 0.89f;
        pCamera->target.x = -1114.79f;
        pCamera->target.y = -41.68f;
        pCamera->target.z = -1686.27f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSKAGU_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pCamera->direction.x = -0.61f;
        pCamera->direction.y = 0.29f;
        pCamera->direction.z = 0.74f;
        pCamera->target.x = 1385.7698f;
        pCamera->target.y = -1184.8399f;
        pCamera->target.z = -892.5199f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        pCamera->direction.x = 0.64f;
        pCamera->direction.y = -0.03f;
        pCamera->direction.z = 0.77f;
        pCamera->target.x = -1443.05f;
        pCamera->target.y = -321.31f;
        pCamera->target.z = -830.79f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSKAGU_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pCamera->direction.x = -0.64f;
            pCamera->direction.y = 0.3f;
            pCamera->direction.z = 0.71f;
            pCamera->target.x = 1255.7399f;
            pCamera->target.y = -813.03992f;
            pCamera->target.z = -819.09f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.52f;
            pCamera->direction.y = 0.03f;
            pCamera->direction.z = 0.86f;
            pCamera->target.x = 997.13f;
            pCamera->target.y = -372.64997f;
            pCamera->target.z = -1053.4299f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pCamera->direction.x = 0.58f;
            pCamera->direction.y = 0.07f;
            pCamera->direction.z = 0.81f;
            pCamera->target.x = -1318.2299f;
            pCamera->target.y = -430.66f;
            pCamera->target.z = -1078.51f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_EP_IT_Set(nbCamera_t *pCamera)
{
    u32 n = 0;
    u32 r;

    effBTLModelAllShow();
    if (nbCameraBossCutFlag == 1) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            n = 1;
            break;
        case 1:
            n = 3;
            break;
        }
    } else {
        r = dds3GetRandIntA(0, 3);
        switch (r) {
        case 0:
            n = 0;
            break;
        case 1:
            n = 2;
            break;
        case 2:
            n = 4;
            break;
        case 3:
            n = 5;
            break;
        }
    }
    switch (n) {
    case 0:
        pCamera->direction.x = 0.48f;
        pCamera->direction.y = -0.15f;
        pCamera->direction.z = 0.87f;
        pCamera->target.x = -986.15992f;
        pCamera->target.y = -42.259995f;
        pCamera->target.z = -1594.4098f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 1:
        pCamera->direction.x = -0.31f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.95f;
        pCamera->target.x = 617.1609f;
        pCamera->target.y = -279.74997f;
        pCamera->target.z = -1764.7099f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 2:
        pCamera->direction.x = 0.57f;
        pCamera->direction.y = -0.02f;
        pCamera->direction.z = 0.82f;
        pCamera->target.x = -1407.16f;
        pCamera->target.y = -256.49997f;
        pCamera->target.z = -1795.4799f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 3:
        pCamera->direction.x = -0.36f;
        pCamera->direction.y = -0.07f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = 706.98993f;
        pCamera->target.y = -152.13f;
        pCamera->target.z = -1804.0999f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 4:
        pCamera->direction.x = 0.28f;
        pCamera->direction.y = 0.01f;
        pCamera->direction.z = 0.96f;
        pCamera->target.x = -448.85996f;
        pCamera->target.y = -282.52997f;
        pCamera->target.z = -1788.65f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    case 5:
        pCamera->direction.x = 0.47f;
        pCamera->direction.y = -0.11f;
        pCamera->direction.z = 0.87f;
        pCamera->target.x = -870.40992f;
        pCamera->target.y = -34.279995f;
        pCamera->target.z = -1640.9098f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pCamera->direction.x = -0.38f;
        pCamera->direction.y = -0.01f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = 950.09f;
        pCamera->target.y = -379.85996f;
        pCamera->target.z = -662.6399f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.54f;
            pCamera->direction.y = -0.14f;
            pCamera->direction.z = 0.83f;
            pCamera->target.x = -1457.63f;
            pCamera->target.y = -83.44f;
            pCamera->target.z = -507.48f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.42f;
            pCamera->direction.y = 0.42f;
            pCamera->direction.z = 0.8f;
            pCamera->target.x = -1429.4899f;
            pCamera->target.y = -1608.3599f;
            pCamera->target.z = -973.89991f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pCamera->direction.x = -0.17f;
            pCamera->direction.y = 0.09f;
            pCamera->direction.z = 0.98f;
            pCamera->target.x = 519.72992f;
            pCamera->target.y = -555.24994f;
            pCamera->target.z = -1470.1099f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.45f;
            pCamera->direction.y = 0.14f;
            pCamera->direction.z = 0.88f;
            pCamera->target.x = 1523.54f;
            pCamera->target.y = -841.15f;
            pCamera->target.z = -1597.79f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pCamera->direction.x = 0.3f;
            pCamera->direction.y = -0.01f;
            pCamera->direction.z = 0.95f;
            pCamera->target.x = -823.9599f;
            pCamera->target.y = -244.6f;
            pCamera->target.z = -1453.4499f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.58f;
            pCamera->direction.y = 0.07f;
            pCamera->direction.z = 0.81f;
            pCamera->target.x = -1318.2299f;
            pCamera->target.y = -430.66f;
            pCamera->target.z = -1078.51f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX03_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pCamera->direction.x = -0.38f;
        pCamera->direction.y = -0.01f;
        pCamera->direction.z = 0.93f;
        pCamera->target.x = 950.09f;
        pCamera->target.y = -379.85996f;
        pCamera->target.z = -662.6399f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.54f;
            pCamera->direction.y = -0.14f;
            pCamera->direction.z = 0.83f;
            pCamera->target.x = -1457.63f;
            pCamera->target.y = -83.44f;
            pCamera->target.z = -507.48f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.42f;
            pCamera->direction.y = 0.42f;
            pCamera->direction.z = 0.8f;
            pCamera->target.x = -1429.4899f;
            pCamera->target.y = -1608.3599f;
            pCamera->target.z = -973.89991f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSMETA_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.3f;
            pCamera->direction.y = 0.28f;
            pCamera->direction.z = 0.91f;
            pCamera->target.x = 780.83991f;
            pCamera->target.y = -876.85993f;
            pCamera->target.z = -1185.59f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.49f;
            pCamera->direction.y = -0.26f;
            pCamera->direction.z = 0.83f;
            pCamera->target.x = 607.6999f;
            pCamera->target.y = -37.06f;
            pCamera->target.z = -203.45f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.62f;
            pCamera->direction.y = 0.37f;
            pCamera->direction.z = 0.7f;
            pCamera->target.x = -1387.0398f;
            pCamera->target.y = -1122.9098f;
            pCamera->target.z = -824.89f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.72f;
            pCamera->direction.y = -0.11f;
            pCamera->direction.z = 0.69f;
            pCamera->target.x = -1111.4899f;
            pCamera->target.y = -88.11f;
            pCamera->target.z = -332.72f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSMETA_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.4f;
            pCamera->direction.y = -0.09f;
            pCamera->direction.z = 0.91f;
            pCamera->target.x = 488.05997f;
            pCamera->target.y = -118.58f;
            pCamera->target.z = -800.80994f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.09f;
            pCamera->direction.y = -0.18f;
            pCamera->direction.z = 0.98f;
            pCamera->target.x = 184.78998f;
            pCamera->target.y = -75.7099915f;
            pCamera->target.z = -724.27f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.64f;
            pCamera->direction.y = 0.11f;
            pCamera->direction.z = 0.76f;
            pCamera->target.x = -1315.6f;
            pCamera->target.y = -465.79f;
            pCamera->target.z = -1038.3699f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.36f;
            pCamera->direction.y = -0.03f;
            pCamera->direction.z = 0.93f;
            pCamera->target.x = -521.849915f;
            pCamera->target.y = -154.85f;
            pCamera->target.z = -971.6399f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSBERU_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.55f;
            pCamera->direction.y = -0.13f;
            pCamera->direction.z = 0.83f;
            pCamera->target.x = -482.02997f;
            pCamera->target.y = -100.239991f;
            pCamera->target.z = -585.49994f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.5f;
            pCamera->direction.y = 0.45f;
            pCamera->direction.z = 0.74f;
            pCamera->target.x = -672.03f;
            pCamera->target.y = -827.14f;
            pCamera->target.z = -935.79993f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.04f;
            pCamera->direction.y = 0.08f;
            pCamera->direction.z = 1.0f;
            pCamera->target.x = -69.98f;
            pCamera->target.y = -307.73996f;
            pCamera->target.z = -768.24f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.31f;
            pCamera->direction.y = 0.4f;
            pCamera->direction.z = 0.87f;
            pCamera->target.x = 358.62995f;
            pCamera->target.y = -751.66993f;
            pCamera->target.z = -924.78992f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSBERU_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.47f;
            pCamera->direction.y = -0.24f;
            pCamera->direction.z = 0.85f;
            pCamera->target.x = -412.12f;
            pCamera->target.y = -76.77f;
            pCamera->target.z = -604.7f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = 0.55f;
            pCamera->direction.y = 0.28f;
            pCamera->direction.z = 0.79f;
            pCamera->target.x = -730.98993f;
            pCamera->target.y = -657.599915f;
            pCamera->target.z = -986.97992f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.38f;
            pCamera->direction.y = -0.14f;
            pCamera->direction.z = 0.92f;
            pCamera->target.x = 393.21f;
            pCamera->target.y = -115.28f;
            pCamera->target.z = -756.429932f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        case 1:
            pCamera->direction.x = -0.58f;
            pCamera->direction.y = 0.28f;
            pCamera->direction.z = 0.76f;
            pCamera->target.x = 756.9f;
            pCamera->target.y = -554.929932f;
            pCamera->target.z = -776.30994f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_EX01_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x <= pInvoke->orgpos.x) {
        pCamera->direction.x = 0.29f;
        pCamera->direction.y = -0.27f;
        pCamera->direction.z = 0.92f;
        pCamera->target.x = -513.85993f;
        pCamera->target.y = -36.889996f;
        pCamera->target.z = -954.55994f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.87266452f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.58f;
            pCamera->direction.y = 0.01f;
            pCamera->direction.z = 0.81f;
            pCamera->target.x = 1516.4499f;
            pCamera->target.y = -646.57f;
            pCamera->target.z = -1426.9f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        case 1:
            pCamera->direction.x = -0.39f;
            pCamera->direction.y = -0.39f;
            pCamera->direction.z = 0.84f;
            pCamera->target.x = 479.86997f;
            pCamera->target.y = -41.26f;
            pCamera->target.z = -491.59995f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_EX02_Set(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    func_0016E120();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = 0.39f;
            pCamera->direction.y = 0.34f;
            pCamera->direction.z = 0.86f;
            pCamera->target.x = -1069.34f;
            pCamera->target.y = -1393.5999f;
            pCamera->target.z = -2041.3499f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        case 1:
            pCamera->direction.x = 0.29f;
            pCamera->direction.y = -0.27f;
            pCamera->direction.z = 0.92f;
            pCamera->target.x = -513.85993f;
            pCamera->target.y = -36.889996f;
            pCamera->target.z = -954.55994f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pCamera->direction.x = -0.58f;
            pCamera->direction.y = 0.01f;
            pCamera->direction.z = 0.81f;
            pCamera->target.x = 1516.4499f;
            pCamera->target.y = -646.57f;
            pCamera->target.z = -1426.9f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        case 1:
            pCamera->direction.x = -0.27f;
            pCamera->direction.y = -0.26f;
            pCamera->direction.z = 0.93f;
            pCamera->target.x = 298.16f;
            pCamera->target.y = -131.8f;
            pCamera->target.z = -999.899902344f;
            pCamera->distance = 1.0f;
            pCamera->fovy = 0.87266452f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_AllParty_Set(nbCamera_t *pCamera)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Ptr_Init);

s32 nbCameraBoss_Cut2_Ptr_Run(nbCamera_t *pCamera, nbCameraSkillInfo_t *pSkillInfo)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraHero_Cut1_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraHero_Cut1_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraHero_Cut2_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraHero_Cut2_Run);

INCLUDE_RODATA(const s32, "newbattle/nbCameraBoss", D_005357E0);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_DCOMBO_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_KAGIRI_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_DCOMBO_KAGU_Set);
