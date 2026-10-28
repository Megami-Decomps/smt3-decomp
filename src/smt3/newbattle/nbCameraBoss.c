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

extern u8 dds3EffectBase[];

extern u32 dds3GetRandA(u8 *name);

extern void effBTLModelAllHide(void);

extern s32 effBTLModelGetGroupNumber(u32 group);

extern void nbCameraShot_InvokeTarget_Set2(nbCamera_t *pNbCamera, void *pNBSInfo);

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

extern s32 nbCameraBoss_BSKAGU_Command_Calc(nbCamera_t *pNbCamera, nbFormation_t *pCmdForm);

extern s32 nbCameraBoss_BSLUSI_Command_Calc(nbCamera_t *pNbCamera, nbFormation_t *pCmdForm);

extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);

extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);

extern s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pNbCamera);

extern u32 effBTLSkillGetTargetNumber(void);

extern void effBTLGetGroupPos(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos);

extern f32 D_0054F3A0[];

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

extern void effBTLModelInvokeShow(void);

extern f32 effBTLModelGroupGetSphere(u32 group);

extern void effBTLModelGroupGetDirection(u32 group, f32 *pDirection);

extern f32 sdfAtan(f32 aX);

extern s32 nbCameraBossCutFlag;

extern s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pNbCamera);

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

extern void nbCameraShot_Invoke_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);

extern s32 nbCameraHero_Cut2_Run(nbCamera_t *pNbCamera, void *pNBSInfo);

extern sdfVector4_t nbCameraBSARICut1Vec;

extern f32 BSARI_Cut1_Rate;

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern s32 effBTLModelGetID(nbFormation_t *pFormation);

extern void nbCameraShot_GroundCollision(nbCamera_t *pNbCamera);

extern datUnitVisual_t datModelParamTable[];

extern nbCamera_t pNbCamera;

extern u32 nbCameraBoss_BSBERU_E915_Frame;

extern f32 nbCameraBoss_BSBERU_E915_Rate;

extern u32 Foru_E913_Frame;

extern void effBTLModelAllShow(void);

extern u32 nbCameraBoss_BSARI_MASEHA_Frame;

extern s32 nbCameraBoss_BSARI_MASEHA_Type;

extern void nbResetAllFormPosRot(void);

extern void effBTLModelInvokeGroupHide(void);

extern void nbCameraShot_All_Set2(nbCamera_t *pNbCamera, void *pNBSInfo);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern f32 nbCameraBoss_BSALBI_E912_Rate;

extern f32 nbCameraBoss_BSALBI_E912_Distance;

extern s32 nbCameraBoss_BSARI_SIMANO_Type;

extern f32 nbCameraBoss_BSARI_SIMANO_Rate;

extern u8 nbCameraBoss_Pish_MUHENK_Type;

extern u32 nbCameraBoss_Pish_MUHENK_Frame;

extern void nbCameraShot_InvokeTargetReverse_Set2(nbCamera_t *pNbCamera, void *pNBSInfo);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

void nbCameraSetState(u16 state);

void nbCameraState_InvokeTarget_SearchSet(void);

typedef struct effBezier4PPart_t {
    sdfVector3_t point[4];
    f32 weight;
    f32 step;
} effBezier4PPart_t;

extern effBezier4PPart_t nbCamera_DOUTYO_B4P;

extern void nbCameraLerpVec(effBezier4PPart_t *pPart, sdfVector4_t *pOut);

extern u32 nbCameraHero_Cut_Frame;

extern void nbCameraShot_PullOne_Init(nbCamera_t *pNbCamera);

extern void nbCameraShot_PullOne_Calc(nbCamera_t *pNbCamera, f32 rate);

extern void SetRotateMatrixY(f32 aAngle);

extern u32 effBTLSkillGetID(void);

extern void effBTLSkillSetFormationInfo(s32 InvokeID, u32 TargetBit);

extern void effBTLModelTargetGroupHide(void);

extern void nbCameraShot_TargetParty_Set2(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo);

extern effBezier4PPart_t D_0056B350[2];

extern f32 nbCameraBoss_Sp_MURET2_Accele;

extern u32 effBTLSkillGetState(void);

extern u32 effBTLModelGetBadStatus(nbFormation_t *pFormation);

s32 nbCameraDANTE_DCOMBO_Set(s32 frame);

s32 nbCameraDANTE_DCOMBO_KAGU_Set(s32 frame);

extern u32 effBTLSkillGetTargetParty(void);

extern s32 nbCameraBoss_CheckRotate(nbFormation_t *pFormation);

extern sdfVector4_t D_00325A20;

extern void effBTLModelTargetPartyShow(void);

extern void effBTLModelGroupGetCenter(u32 group);

extern s32 nbCameraBoss_BSLUSI_Type;


extern f32 effBTLPartGetScalePtr(nbFormation_t *pFormation);

s32 nbCameraBoss_BSARI_MASEHA_Run(nbCamera_t *pNbCamera);

s32 nbCameraBoss_BSARI_SIMANO_Run(nbCamera_t *pNbCamera);

s32 nbCameraBoss_Pish_MUHENK_Run(nbCamera_t *pNbCamera);







INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_InvokeTarget_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_TargetInvoke_Set);

s32 nbCameraBoss_Foru_SIMON_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    nbCameraSkillInfo_t work;
    sdfVector4_t center;
    u32 state;
    u32 type;

    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    state = effBTLSkillGetState();
    if (effBTLModelGetBadStatus(pInvoke) & 0x40) {
        return 0;
    }
    if (state & 0xC) {
        return 0;
    }
    if (dds3GetRandA(dds3EffectBase) & 1) {
        work.angleH = -0.26179934f;
        if (dds3GetRandA(dds3EffectBase) & 1) {
            work.angleW = 0.5235987f;
        } else {
            work.angleW = -0.5235987f;
        }
        work.HeightMin = 0.0f;
        work.WidthMin = 500.0f;
        nbCameraShot_TargetParty_Set2(pNbCamera, &work);
    } else {
        effBTLModelAllShow();
        type = dds3GetRandA(dds3EffectBase) & 3;
        switch (type) {
        case 0:
            pNbCamera->target.x = 459.5f;
            pNbCamera->target.y = -122.1f;
            pNbCamera->target.z = 752.6f;
            pNbCamera->direction.x = -0.4505f;
            pNbCamera->direction.y = 0.0663f;
            pNbCamera->direction.z = -0.8903f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->target.x = -238.1f;
            pNbCamera->target.y = -97.6f;
            pNbCamera->target.z = 757.9f;
            pNbCamera->direction.x = 0.1716f;
            pNbCamera->direction.y = 0.0122f;
            pNbCamera->direction.z = -0.9851f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 2:
            pNbCamera->target.x = -2.9f;
            pNbCamera->target.y = -232.4f;
            pNbCamera->target.z = -1378.6f;
            pNbCamera->direction.x = 0.0052f;
            pNbCamera->direction.y = 0.0645f;
            pNbCamera->direction.z = 0.9979f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 3:
            pNbCamera->target.x = 764.5f;
            pNbCamera->target.y = -706.7f;
            pNbCamera->target.z = 639.2f;
            pNbCamera->direction.x = -0.6054f;
            pNbCamera->direction.y = 0.3837f;
            pNbCamera->direction.z = -0.6973f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    if (effBTLSkillGetTargetNumber() == 0) {
        return 1;
    }
    effBTLModelGroupGetCenter(pTarget->stat & 0x300);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    effBTLModelLook(pInvoke, &center);
    return 1;
}

void nbCameraBoss_Foru_E913_Set(void)
{
    Foru_E913_Frame = 0;
    pNbCamera.frame = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Foru_E913_Run);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_MURET2_Set);

s32 nbCameraBoss_Sp_MURET2_Run(nbCamera_t *pNbCamera)
{
    sdfVector4_t targetPos;
    sdfVector4_t eyePos;
    f32 limit = 0.014285714f;
    f32 length;

    if (effBTLSkillGetState() & 0xC) {
        return 0;
    }
    D_0056B350[0].step = D_0056B350[0].step + nbCameraBoss_Sp_MURET2_Accele;
    D_0056B350[1].step = D_0056B350[1].step + nbCameraBoss_Sp_MURET2_Accele;
    if (D_0056B350[0].step > limit) {
        D_0056B350[0].step = limit;
    }
    D_0056B350[1].step = D_0056B350[0].step;
    if (D_0056B350[1].weight > 0.75f) {
        D_0056B350[1].step = D_0056B350[0].step * 0.965f;
        if (D_0056B350[1].step < 0.001f) {
            D_0056B350[1].step = 0.0f;
        }
        D_0056B350[0].step = D_0056B350[1].step;
    }
    nbCameraLerpVec(&D_0056B350[1], &eyePos);
    nbCameraLerpVec(&D_0056B350[0], &targetPos);
    SDF_VU0_LOADVECTOR(&targetPos);
    SDF_VU0_STOREVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&eyePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pNbCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    return 1;
}

s32 nbCameraBoss_Sp_DOUTYO_TP_Set(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *p;
    nbCameraSkillInfo_t work;
    f32 angle;
    u32 mask;
    u32 i;

    mask = 0;
    nbCameraSetState(0x17);
    pData = nbGetMainProcessData();
    p = pData->form;
    for (i = 0; i < 0x20; i++) {
        if ((p->stat & 0x201) == 0x201) {
            mask |= 1 << p->formindex;
        }
        p++;
    }
    angle = 0.61086512f;
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, mask);
    effBTLModelTargetGroupHide();
    work.angleH = angle;
    if (dds3GetRandA(dds3EffectBase) & 1) {
        work.angleW = -0.61086512f;
    } else {
        work.angleW = angle;
    }
    nbCameraShot_TargetParty_Set2(pNbCamera, &work);
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_TPFix_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sp_DOUTYO_Set);

s32 nbCameraBoss_Sp_DOUTYO_Run(nbCamera_t *pNbCamera)
{
    effBezier4PPart_t *pPart = &nbCamera_DOUTYO_B4P;
    sdfVector4_t pos;
    f32 step;
    f32 length;

    nbCameraLerpVec(pPart, &pos);
    if (pPart->weight > 0.75f) {
        step = pPart->step * 0.965f;
        pPart->step = step;
        if (step < 0.001f) {
            pPart->step = 0.0f;
        }
    }
    SDF_VU0_LOADVECTOR(&pNbCamera->target);
    SDF_VU0_LOADVECTOR_B(&pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pNbCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    return 1;
}

s32 nbCameraBoss_Sp_JAREIH_Cut1_Set(nbCamera_t *pNbCamera)
{
    nbResetAllFormPosRot();
    return 0;
}

s32 nbCameraBoss_Sp_JAREIH_Set(nbCamera_t *pNbCamera)
{
    return 1;
}

s32 nbCameraBoss_Sp_JAREIH_Run(nbCamera_t *pNbCamera)
{
    u8 work[0x30];
    sdfVector4_t center;
    f32 length;

    if (pNbCamera->frame >= 50) {
        nbCameraShot_All_Set2(pNbCamera, work);
    } else {
        nbFormation_t *pForm = effBTLSkillGetInvokeFormation();

        center.x = -385.7f;
        center.y = -10.0f;
        center.z = -488.4f;
        effBTLModelGetSphereCenter(pForm);
        SDF_VU0_STOREVECTOR(pNbCamera);
        pNbCamera->target.y *= 0.5f;
        SDF_VU0_LOADVECTOR(pNbCamera);
        SDF_VU0_LOADVECTOR_B(&center);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(length);
        pNbCamera->distance = length;
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera->direction);
        pNbCamera->fovy = 0.69813163f;
        SetRotateMatrixY(pNbCamera->frame * -0.008726646f);
        SDF_VU0_LOADVECTOR(&pNbCamera->direction);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
    return 1;
}

s32 nbCameraBoss_Sp_PSYSTL_Set(nbCamera_t *pNbCamera)
{
    u8 work[0x30];

    effBTLModelAllShow();
    nbCameraShot_InvokeTargetReverse_Set2(pNbCamera, work);
    return 1;
}

s32 nbCameraBoss_Sp_PSYSTL_Run(nbCamera_t *pNbCamera)
{
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSMIZ_Command_Set);

s32 nbCameraBoss_BSONGY_Command_Set(nbCamera_t *pNbCamera)
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
    SDF_VU0_COPYVECTOR(&pNbCamera->target, &target);
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
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    pNbCamera->distance = 1700.0f;
    effBTLModelLook(pForm, &pNbCamera->target);
    return 1;
}

s32 nbCameraBoss_Ongy_BUNSIN_Set(nbCamera_t *pNbCamera)
{
    nbCameraSetState(0);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Ongy_BUNSIN_Run);

s32 nbCameraBoss_BSARUS_Command_Set(nbCamera_t *pNbCamera)
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
    SDF_VU0_COPYVECTOR(&pNbCamera->target, &target);
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&target);
    SDF_VU0_LOADVECTOR_B(&center);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    pNbCamera->distance = length + 500.0f;
    effBTLModelLook(pForm, &pNbCamera->target);
    return 1;
}


s32 nbCameraBoss_BSSAMA_Command_Set(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pBase;
    nbFormation_t *pForm;
    sdfVector4_t pos;
    sdfVector4_t base;
    sdfVector4_t center;
    sdfVector4_t dir;

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
    SDF_VU0_STOREVECTOR_NOMEM(&base);
    base.y = base.y + pBase->h * pBase->scl * 0.35f;
    SDF_VU0_COPYVECTOR(pNbCamera, &base);
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&base);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    pos.x = center.x + dir.x * 550.0f;
    pos.y = center.y + dir.y * 550.0f;
    pos.z = center.z + dir.z * 550.0f;
    if (pos.y > -10.0f) {
        pos.y = -10.0f;
    }
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    effBTLModelLook(pForm, &pNbCamera->target);
    return 1;
}


s32 nbCameraBoss_Samael_AKUI_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllShow();
    nbResetAllFormPosRot();
    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        pNbCamera->target.x = 799.62f;
        pNbCamera->target.y = -165.55f;
        pNbCamera->target.z = 1412.67f;
        pNbCamera->direction.x = -0.43f;
        pNbCamera->direction.y = -0.08f;
        pNbCamera->direction.z = -0.9f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        pNbCamera->target.x = -818.13f;
        pNbCamera->target.y = -95.8f;
        pNbCamera->target.z = 1316.26f;
        pNbCamera->direction.x = 0.47f;
        pNbCamera->direction.y = -0.12f;
        pNbCamera->direction.z = -0.87f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_Samael_AKUI_Run(nbCamera_t *pNbCamera)
{
    return 1;
}


/* NON_MATCHING: 4 of 77 words; one global load is scheduled three loads early. */
#ifdef NON_MATCHING
s32 nbCameraBoss_Sukady_DAIJIS_Set(nbCamera_t *pNbCamera)
{
    u32 type;

    effBTLModelAllShow();
    type = dds3GetRandIntA(0, 3);
    switch (type) {
    case 0:
        pNbCamera->direction.x = D_0054F3A0[0];
        pNbCamera->direction.y = 0.29f;
        pNbCamera->direction.z = 0.96f;
        pNbCamera->target.x = 11.27f;
        pNbCamera->target.y = -596.07f;
        pNbCamera->target.z = -1543.5999f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.55f;
        pNbCamera->direction.y = 0.19f;
        pNbCamera->direction.z = 0.81f;
        pNbCamera->target.x = -944.1399f;
        pNbCamera->target.y = -454.91f;
        pNbCamera->target.z = -1265.4899f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.38f;
        pNbCamera->direction.y = 0.48f;
        pNbCamera->direction.z = -0.79f;
        pNbCamera->target.x = -584.87f;
        pNbCamera->target.y = -902.14f;
        pNbCamera->target.z = 1002.33f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Sukady_DAIJIS_Set);
#endif


s32 nbCameraBoss_Baal_BAELNO_Set(nbCamera_t *pNbCamera)
{
    u8 work[0x30];

    if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
        nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
    } else {
        nbCameraShot_InvokeTargetReverse_Set2(pNbCamera, work);
    }
    return 1;
}

s32 nbCameraBoss_Baal_BAELNO_Run(nbCamera_t *pNbCamera)
{
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Set(nbCamera_t *pNbCamera)
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
    work.HeightMin = 0.0f;
    work.WidthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pNbCamera, &work);
    pNbCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Cut1_Run(nbCamera_t *pNbCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pNbCamera->frame < datModelParamTable[id].part[2].actframe) {
        pNbCamera->distance += 12.5f;
    }
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Set(nbCamera_t *pNbCamera)
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
    work.HeightMin = 0.0f;
    work.WidthMin = 0.0f;
    nbCameraShot_Invoke_Set2(pNbCamera, &work);
    pNbCamera->target.y -= pForm->h * pForm->scl * 0.25f;
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Cut1_Run(nbCamera_t *pNbCamera)
{
    s32 id = effBTLModelGetID(effBTLSkillGetInvokeFormation());

    if (id == 0x200) {
        return 0;
    }
    if (pNbCamera->frame < datModelParamTable[id].part[3].actframe) {
        pNbCamera->distance -= 15.0f;
    }
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    sdfVector4_t target;
    sdfVector4_t mark;
    sdfVector4_t center;
    sdfVector4_t dir;
    f32 distance;

    effBTLModelTargetPartyShow();
    effBTLModelInvokeGroupHide();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    effBTLModelGetMarkPosition2(pInvoke, 0x14);
    SDF_VU0_STOREVECTOR_NOMEM(&mark);
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_LOADVECTOR_B(&mark);
    VU0_LERP_VF10_VF11(0.25f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    SDF_VU0_STOREVECTOR_NOMEM(&mark);
    distance = effBTLPartGetScalePtr(pTarget) * 150.0f + 600.0f;
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    mark.y = 0.0f;
    center.y = 0.0f;
    center.x = center.x + pTarget->r * pTarget->scl;
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&mark);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    target.x = center.x + dir.x * distance;
    target.y = -25.0f;
    target.z = center.z + dir.z * distance;
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    effBTLModelLook(pTarget, &pNbCamera->target);
    return 1;
}

s32 nbCameraBoss_Bsnet_Attack_Run(nbCamera_t *pNbCamera)
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

s32 nbCameraBoss_BSALBI_InvokeTargetParty_Set(nbCamera_t *pNbCamera)
{
    s32 type;

    effBTLModelAllShow();
    type = dds3GetRandIntA(0, 2);
    switch (type) {
    case 0:
        effBTLModelInvokeGroupHide();
        pNbCamera->direction.x = -0.31f;
        pNbCamera->direction.y = 0.36f;
        pNbCamera->direction.z = -0.88f;
        pNbCamera->target.x = 510.01996f;
        pNbCamera->target.y = -852.57f;
        pNbCamera->target.z = 1895.83f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.03f;
        pNbCamera->direction.y = 0.17f;
        pNbCamera->direction.z = 0.98f;
        pNbCamera->target.x = -62.22f;
        pNbCamera->target.y = -493.04f;
        pNbCamera->target.z = -1677.5599f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
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

void nbCameraBoss_Bsnet_E907_2_Set(void)
{
    u8 work[0x30];
    nbCamera_t camera;
    nbCamera_t *pCamera = &pNbCamera;

    SDF_VU0_COPYVECTOR(&Bsnet_E907_2_EndTarget, &pCamera->target);
    SDF_VU0_COPYVECTOR(&Bsnet_E907_2_EndDirection, &pCamera->direction);
    Bsnet_E907_2_StartDistance = pCamera->distance;
    nbCameraShot_All_Set2(&camera, work);
    SDF_VU0_COPYVECTOR(&D_0056B420, &camera.target);
    SDF_VU0_COPYVECTOR(&D_0056B430, &camera.direction);
    Bsnet_E907_2_EndDistance = camera.distance * 1.05f;
    Bsnet_E907_2_Rate = 1.0f;
}

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

s32 nbCameraBoss_BSARI_Command_Set(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pBoss;
    nbFormation_t *pForm;
    sdfVector4_t target;
    sdfVector4_t boss;
    sdfVector4_t center;
    sdfVector4_t dir;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->orderindex == -1) {
        return 0;
    }
    pBoss = &pData->form[4];
    pForm = &pData->form[pData->party[pData->order[pData->orderindex]].formindex];
    effBTLModelGetSphereCenter(pBoss);
    SDF_VU0_STOREVECTOR_NOMEM(&boss);
    boss.y = boss.y + pBoss->h * pBoss->scl * 0.35f;
    SDF_VU0_COPYVECTOR(pNbCamera, &boss);
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    center.y = -effBTLModelGetHeight(pForm);
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&boss);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    target.x = center.x + dir.x * 950.0f;
    target.y = center.y + dir.y * 950.0f;
    target.z = center.z + dir.z * 950.0f;
    if (target.y > -10.0f) {
        target.y = -10.0f;
    }
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    effBTLModelLook(pForm, &pNbCamera->target);
    return 1;
}

s32 nbCameraBoss_BSARI_Cut1_Set(nbCamera_t *pNbCamera)
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

s32 nbCameraBoss_BSARI_Cut1_Run(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.9f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetSphereCenter(pForm);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->target.y -= pForm->h * pForm->scl * 0.15f;
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pNbCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut1_Set(nbCamera_t *pNbCamera)
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

s32 nbCameraBoss_BSARI2_Cut1_Run(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm = effBTLSkillGetInvokeFormation();
    f32 rate = BSARI_Cut1_Rate * 0.985f;
    f32 blend;
    f32 length;

    BSARI_Cut1_Rate = rate;
    blend = rate * 0.5f + 0.75f;
    effBTLModelGetMarkPosition2(pForm, 1);
    SDF_VU0_STOREVECTOR(pNbCamera);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&nbCameraBSARICut1Vec);
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI_MASEHA_Set(nbCamera_t *pNbCamera)
{
    nbCameraBoss_BSARI_MASEHA_Frame = 0;
    nbCameraBoss_BSARI_MASEHA_Type = dds3GetRandIntA(0, 3);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSARI_MASEHA_Run);

s32 nbCameraBoss_BSARI_SIMANO_Set(nbCamera_t *pNbCamera)
{
    nbCameraBoss_BSARI_SIMANO_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_BSARI_SIMANO_Rate = 1.0f;
    return 1;
}

s32 nbCameraBoss_BSARI_SIMANO_Run(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm;
    sdfVector4_t target;
    sdfVector4_t dir;
    f32 back;
    u32 type;

    pForm = effBTLSkillGetInvokeFormation();
    effBTLModelAllShow();
    type = nbCameraBoss_BSARI_SIMANO_Type;
    switch (type) {
    case 0:
        pNbCamera->direction.x = 0.0f;
        pNbCamera->direction.y = 0.29f;
        pNbCamera->direction.z = 0.96f;
        pNbCamera->target.x = -27.54f;
        pNbCamera->target.y = -835.08991f;
        pNbCamera->target.z = -1958.6698f;
        break;
    case 1:
        pNbCamera->direction.x = -0.24f;
        pNbCamera->direction.y = 0.07f;
        pNbCamera->direction.z = 0.97f;
        pNbCamera->target.x = 591.60993f;
        pNbCamera->target.y = -413.11997f;
        pNbCamera->target.z = -1890.9499f;
        break;
    case 2:
        pNbCamera->direction.x = 0.49f;
        pNbCamera->direction.y = 0.07f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = -991.69f;
        pNbCamera->target.y = -402.41f;
        pNbCamera->target.z = -1581.38f;
        break;
    }
    sdfAtan(0.34906582f);
    back = 0.0f;
    SDF_VU0_LOADVECTOR(&pForm->center);
    SDF_VU0_LOADVECTOR_B(pNbCamera);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    target.x = pForm->center.x - dir.x * back;
    target.y = pForm->center.y - dir.y * back;
    target.z = pForm->center.z - dir.z * back;
    nbCameraBoss_BSARI_SIMANO_Rate = nbCameraBoss_BSARI_SIMANO_Rate * 0.95f;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&dir);
    VU0_LERP_VF10_VF11(nbCameraBoss_BSARI_SIMANO_Rate);
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&target);
    VU0_LERP_VF10_VF11(nbCameraBoss_BSARI_SIMANO_Rate);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_Bsnet_AISHOH_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pNbCamera->direction.x = 0.77f;
        pNbCamera->direction.y = -0.29f;
        pNbCamera->direction.z = 0.57f;
        pNbCamera->target.x = -1638.33f;
        pNbCamera->target.y = -66.21999f;
        pNbCamera->target.z = -854.07f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.65f;
        pNbCamera->direction.y = -0.37f;
        pNbCamera->direction.z = 0.66f;
        pNbCamera->target.x = -1513.32f;
        pNbCamera->target.y = 9.739999f;
        pNbCamera->target.z = -1146.56f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.74f;
        pNbCamera->direction.y = -0.31f;
        pNbCamera->direction.z = 0.59f;
        pNbCamera->target.x = -2002.5799f;
        pNbCamera->target.y = -34.919995f;
        pNbCamera->target.z = -1281.7199f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
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

s32 nbCameraBoss_Kagu_FLEACH_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllShow();
    if (nbCameraBossCutFlag == 1) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.4f;
            pNbCamera->direction.y = 0.22f;
            pNbCamera->direction.z = 0.89f;
            pNbCamera->target.x = 949.28992f;
            pNbCamera->target.y = -1372.0599f;
            pNbCamera->target.z = -2017.89f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.01f;
            pNbCamera->direction.y = 0.26f;
            pNbCamera->direction.z = 0.97f;
            pNbCamera->target.x = -98.7f;
            pNbCamera->target.y = -1369.1199f;
            pNbCamera->target.z = -2463.0596f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.34f;
            pNbCamera->direction.y = 0.07f;
            pNbCamera->direction.z = 0.94f;
            pNbCamera->target.x = -1205.4999f;
            pNbCamera->target.y = -669.11994f;
            pNbCamera->target.z = -1848.38f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.01f;
            pNbCamera->direction.y = 0.26f;
            pNbCamera->direction.z = 0.97f;
            pNbCamera->target.x = -98.7f;
            pNbCamera->target.y = -1369.1199f;
            pNbCamera->target.z = -2463.0596f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Kagu_MJINKO_Set);

s32 nbCameraBoss_Pish_PRESS_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pNbCamera->direction.x = -0.44f;
        pNbCamera->direction.y = 0.24f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = 1621.9799f;
        pNbCamera->target.y = -1533.9999f;
        pNbCamera->target.z = -1239.0198f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = -0.7584f;
        pNbCamera->direction.y = -0.108f;
        pNbCamera->direction.z = 0.6428f;
        pNbCamera->target.x = 1769.9f;
        pNbCamera->target.y = -311.0f;
        pNbCamera->target.z = -288.7f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.29f;
        pNbCamera->direction.y = -0.1f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = -739.69f;
        pNbCamera->target.y = -207.52f;
        pNbCamera->target.z = -1314.05f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_Pish_MUHENK_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllShow();
    nbCameraBoss_Pish_MUHENK_Type = dds3GetRandIntA(0, 3);
    nbCameraBoss_Pish_MUHENK_Frame = 0;
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Pish_MUHENK_Run);

s32 nbCameraBoss_BSKAGU_Command_Calc(nbCamera_t *pNbCamera, nbFormation_t *pCmdForm)
{
    nbMainProcessData_t *pData;
    sdfVector4_t target;
    sdfVector4_t boss;
    sdfVector4_t center;
    sdfVector4_t dir;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    effBTLModelGetSphereCenter(&pData->form[4]);
    SDF_VU0_STOREVECTOR_NOMEM(&boss);
    SDF_VU0_COPYVECTOR(pNbCamera, &boss);
    effBTLModelGetSphereCenter(pCmdForm);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    center.y = -effBTLModelGetHeight(pCmdForm);
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&boss);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    target.x = center.x + dir.x * 950.0f;
    target.y = center.y + dir.y * 950.0f;
    target.z = center.z + dir.z * 950.0f;
    if (target.y > -10.0f) {
        target.y = -10.0f;
    }
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.69813163f;
    effBTLModelLook(pCmdForm, &pNbCamera->target);
    return 1;
}

s32 nbCameraBoss_BSKAGU_Command_Set(nbCamera_t *pNbCamera)
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
    ret = nbCameraBoss_BSKAGU_Command_Calc(pNbCamera,
        &pData->form[pData->party[pData->order[pData->orderindex]].formindex]);
    pData = NULL;
    return ret;
}

s32 nbCameraBoss_Bskagu_Cut1_Set(nbCamera_t *pNbCamera)
{
    u32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 4);
    switch (type) {
    case 0:
        pNbCamera->direction.x = 0.34f;
        pNbCamera->direction.y = -0.07f;
        pNbCamera->direction.z = 0.94f;
        pNbCamera->target.x = -827.22992f;
        pNbCamera->target.y = -244.35f;
        pNbCamera->target.z = -861.4f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 1:
        pNbCamera->direction.x = -0.28f;
        pNbCamera->direction.y = -0.07f;
        pNbCamera->direction.z = 0.96f;
        pNbCamera->target.x = 747.71f;
        pNbCamera->target.y = -268.25995f;
        pNbCamera->target.z = -742.54f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 2:
        pNbCamera->direction.x = -0.26f;
        pNbCamera->direction.y = 0.42f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = 899.59f;
        pNbCamera->target.y = -1686.4f;
        pNbCamera->target.z = -842.64991f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 3:
        pNbCamera->direction.x = 0.36f;
        pNbCamera->direction.y = 0.04f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = -850.21991f;
        pNbCamera->target.y = -754.96991f;
        pNbCamera->target.z = -1404.91f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    }
    return 1;
}

s32 nbCameraBoss_Bskagu_Cut1_Run(nbCamera_t *pNbCamera)
{
    return 1;
}

s32 nbCameraBoss_Bspish_Cut1_Set(nbCamera_t *pNbCamera)
{
    nbCameraSkillInfo_t work;
    u32 type;

    effBTLModelAllHide();
    effBTLModelInvokeShow();
    type = dds3GetRandIntA(0, 5);
    switch (type) {
    case 0:
        pNbCamera->direction.x = 0.3f;
        pNbCamera->direction.y = -0.15f;
        pNbCamera->direction.z = 0.94f;
        pNbCamera->target.x = -1034.8f;
        pNbCamera->target.y = -314.58997f;
        pNbCamera->target.z = -1062.9799f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 1:
        pNbCamera->direction.x = -0.26f;
        pNbCamera->direction.y = -0.23f;
        pNbCamera->direction.z = 0.94f;
        pNbCamera->target.x = 716.63f;
        pNbCamera->target.y = -257.57f;
        pNbCamera->target.z = -935.71f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 1;
        break;
    case 2:
        pNbCamera->direction.x = 0.31f;
        pNbCamera->direction.y = 0.33f;
        pNbCamera->direction.z = 0.89f;
        pNbCamera->target.x = -1155.65f;
        pNbCamera->target.y = -1912.3699f;
        pNbCamera->target.z = -1384.39f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        nbCameraBossCutFlag = 0;
        break;
    case 3:
        pNbCamera->direction.x = -0.35f;
        pNbCamera->direction.y = 0.3f;
        pNbCamera->direction.z = 0.89f;
        pNbCamera->target.x = 1208.52f;
        pNbCamera->target.y = -1856.16f;
        pNbCamera->target.z = -1390.7f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
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
        work.HeightMin = 0.0f;
        work.WidthMin = 0.0f;
        nbCameraShot_Invoke_Set2(pNbCamera, &work);
        nbCameraShot_GroundCollision(pNbCamera);
        break;
    }
    return 1;
}

s32 nbCameraBoss_Bspish_Cut1_Run(nbCamera_t *pNbCamera)
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

s32 nbCameraBoss_Meta_METAT1_0_Set(nbCamera_t *pNbCamera)
{
    pNbCamera->direction.x = 0.47f;
    pNbCamera->direction.y = -0.28f;
    pNbCamera->direction.z = 0.84f;
    pNbCamera->target.x = -229.18f;
    pNbCamera->target.y = -433.18f;
    pNbCamera->target.z = 261.52997f;
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_Meta_METAT1_1_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    if (pNbCamera->frame != 11) {
        return 1;
    }
    n = dds3GetRandIntA(0, 3);
    switch (n) {
    case 0:
        pNbCamera->direction.x = 0.44f;
        pNbCamera->direction.y = 0.08f;
        pNbCamera->direction.z = 0.89f;
        pNbCamera->target.x = -733.24f;
        pNbCamera->target.y = -284.88996f;
        pNbCamera->target.z = -1329.35f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.39f;
        pNbCamera->direction.y = 0.32f;
        pNbCamera->direction.z = -0.87f;
        pNbCamera->target.x = -602.86994f;
        pNbCamera->target.y = -541.77f;
        pNbCamera->target.z = 1101.0899f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        nbCameraBoss_BSARI1_EP_IT_Set(pNbCamera);
        break;
    }
    return 1;
}

void nbCameraBoss_BSBERU_E915_Init(void)
{
    nbCameraBoss_BSBERU_E915_Frame = 0;
    nbCameraBoss_BSBERU_E915_Rate = 1.0f;
}

void nbCameraBoss_BSBERU_E915_0_Set(void)
{
    sdfVector4_t from, to;
    /* Both VU interpolations consume the same binary32 factor. */
    union { f32 value; u32 bits; } blend;
    nbCamera_t *camera;

    effBTLModelAllShow();
    camera = &pNbCamera;
    blend.value = (s32)nbCameraBoss_BSBERU_E915_Frame / 200.0f;
    from.x = -1028.459839f;
    from.y = -172.94f;
    from.z = -927.7399292f;
    to.x = 621.57f;
    to.y = -810.81f;
    to.z = -849.51f;
    SDF_VU0_LOADVECTOR_MEMOPS(from);
    SDF_VU0_LOADVECTOR_B(&to);
    SDF_VU0_INTERPOLATE(blend.bits);
    SDF_VU0_STOREVECTOR(camera);
    from.x = 0.67f;
    from.y = -0.14f;
    from.z = 0.73f;
    to.x = -0.36f;
    to.y = 0.44f;
    to.z = 0.82f;
    SDF_VU0_LOADVECTOR_MEMOPS(from);
    SDF_VU0_LOADVECTOR_B(&to);
    SDF_VU0_INTERPOLATE(blend.bits);
    SDF_VU0_STOREVECTOR(&camera->direction);
    camera->distance = 1.0f;
    camera->fovy = 0.69813163f;
    nbCameraBoss_BSBERU_E915_Frame++;
}

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

s32 nbCameraBoss_BSLUSI_Command_Set(nbCamera_t *pNbCamera)
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
    ret = nbCameraBoss_BSLUSI_Command_Calc(pNbCamera,
        &pData->form[pData->party[pData->order[pData->orderindex]].formindex]);
    pData = NULL;
    return ret;
}

s32 nbCameraBoss_BSLUSI_Command_Calc(nbCamera_t *pNbCamera, nbFormation_t *pCmdForm)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pBoss;
    sdfVector4_t target;
    sdfVector4_t boss;
    sdfVector4_t center;
    sdfVector4_t dir;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    pBoss = &pData->form[4];
    effBTLModelGetSphereCenter(pBoss);
    SDF_VU0_STOREVECTOR_NOMEM(&boss);
    boss.y = boss.y + 260.0f;
    boss.z = boss.z - 600.0f / pBoss->scl;
    SDF_VU0_COPYVECTOR(pNbCamera, &boss);
    effBTLModelGetSphereCenter(pCmdForm);
    SDF_VU0_STOREVECTOR_NOMEM(&center);
    center.y = -effBTLModelGetHeight(pCmdForm);
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(&boss);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    target.x = center.x + dir.x * 550.0f;
    target.y = center.y + dir.y * 550.0f;
    target.z = center.z + dir.z * 550.0f;
    if (target.y > 0.0f) {
        target.y = D_0054F3A0[12];
    }
    SDF_VU0_LOADVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&target);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->fovy = 0.87266452f;
    effBTLModelLook(pCmdForm, &pNbCamera->target);
    return 1;
}

/* NON_MATCHING: 21 of 125 words, exact size; the rate bits land in $3, not $4. */
#ifdef NON_MATCHING
s32 nbCameraBoss_Lusi_Cut1_Set(nbCamera_t *pNbCamera)
{
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate;
    u32 bits;
    u32 type;

    if (pNbCamera->frame == 0) {
        effBTLModelAllShow();
    }
    rate = pNbCamera->frame / 103.0f;
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    PS2_FLOAT_BITS(bits, rate);
    type = nbCameraBoss_BSLUSI_Type;
    switch (type) {
    case 0:
        from.x = -921.4f;
        from.y = 254.55999f;
        from.z = -1444.55f;
        to.x = -6.46f;
        to.y = 381.44f;
        to.z = -1858.0999f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(pNbCamera);
        from.x = 0.33f;
        from.y = -0.57f;
        from.z = 0.75f;
        to.x = -0.01f;
        to.y = -0.57f;
        to.z = 0.82f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
        break;
    case 1:
        from.x = 1064.8399f;
        from.y = 167.46f;
        from.z = -1605.9f;
        to.x = -6.46f;
        to.y = 381.44f;
        to.z = -1858.0999f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(pNbCamera);
        from.x = -0.42f;
        from.y = -0.46f;
        from.z = 0.78f;
        to.x = -0.01f;
        to.y = -0.57f;
        to.z = 0.82f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
        break;
    }
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Lusi_Cut1_Set);
#endif

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Lusi_TWORLD_Set);

s32 nbCameraBoss_Lusi_OHNOOH_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pNbCamera->direction.x = 0.32f;
        pNbCamera->direction.y = -0.18f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = -753.05f;
        pNbCamera->target.y = -83.349991f;
        pNbCamera->target.z = -1876.56f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.7853981f;
        break;
    case 1:
        pNbCamera->direction.x = -0.01f;
        pNbCamera->direction.y = -0.14f;
        pNbCamera->direction.z = 0.99f;
        pNbCamera->target.x = 3.8399997f;
        pNbCamera->target.y = -158.56f;
        pNbCamera->target.z = -2395.3096f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = -0.42f;
        pNbCamera->direction.y = -0.1f;
        pNbCamera->direction.z = 0.9f;
        pNbCamera->target.x = 929.23993f;
        pNbCamera->target.y = -230.86f;
        pNbCamera->target.z = -1871.33f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.7853981f;
        break;
    case 3:
        pNbCamera->direction.x = -0.22f;
        pNbCamera->direction.y = 0.52f;
        pNbCamera->direction.z = 0.82f;
        pNbCamera->target.x = 600.78f;
        pNbCamera->target.y = -1638.08f;
        pNbCamera->target.z = -1939.0699f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.7853981f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_Lusi_ASIKIK_Set(nbCamera_t *pNbCamera)
{
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate;

    if (pNbCamera->frame == 0) {
        effBTLModelAllHide();
        effBTLModelInvokeShow();
    }
    rate = pNbCamera->frame / 60.0f;
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    from.x = 1.35f;
    from.y = -2119.0f;
    from.z = -1037.5799f;
    to.x = 1.35f;
    to.y = -263.94f;
    to.z = -948.35993f;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(rate);
    SDF_VU0_STOREVECTOR(pNbCamera);
    from.x = 0.0f;
    from.y = 0.25f;
    from.z = 0.97f;
    to.x = D_0054F3A0[13];
    to.y = -0.43f;
    to.z = 0.9f;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(rate);
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraShot_BossTargetInvoke_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    sdfVector4_t invokeCenter;
    sdfVector4_t targetCenter;
    f32 radius;

    effBTLModelAllShow();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR(&invokeCenter);
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR(&targetCenter);
    targetCenter.y = -effBTLModelGetHeight(pTarget);
    radius = pTarget->r * pTarget->scl;
    if (invokeCenter.x < targetCenter.x) {
        targetCenter.x = targetCenter.x - radius;
    } else {
        targetCenter.x = targetCenter.x + radius;
    }
    SDF_VU0_LOADVECTOR(&invokeCenter);
    SDF_VU0_STOREVECTOR(pNbCamera);
    SDF_VU0_LOADVECTOR_B(&targetCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pNbCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    pNbCamera->distance = pNbCamera->distance + radius / sdfAtan(0.34906582f) + 650.0f;
    pNbCamera->fovy = 0.69813163f;
    if (nbCameraBoss_CheckRotate(pTarget)) {
        effBTLModelGetSphereCenter(pInvoke);
        SDF_VU0_STOREVECTOR(&invokeCenter);
        effBTLModelLook(pTarget, &invokeCenter);
    }
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraShot_BossEX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    sdfVector4_t invokeCenter;
    sdfVector4_t groupCenter;
    u32 party;
    f32 length;
    f32 half;
    f32 angle;
    f32 radius;
    f32 dist;

    effBTLModelAllShow();
    party = effBTLSkillGetTargetParty();
    effBTLModelGroupGetSphere(party);
    SDF_VU0_STOREVECTOR(&groupCenter);
    groupCenter.y = 0.0f;
    pInvoke = effBTLSkillGetInvokeFormation();
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR(&invokeCenter);
    invokeCenter.y = -effBTLModelGetHeight(pInvoke);
    SDF_VU0_LOADVECTOR(&invokeCenter);
    SDF_VU0_LOADVECTOR_B(&groupCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    half = length * 0.5f;
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&D_00325A20);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_COPYVECTOR_C_TO_B();
    VU0_LERP_VF10_VF11(0.35f);
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR(&invokeCenter);
    SDF_VU0_LOADVECTOR_B(&groupCenter);
    VU0_LERP_VF10_VF11(0.7f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->fovy = 0.69813163f;
    angle = sdfAtan(0.49567342f);
    dist = half / angle;
    pNbCamera->distance = dist;
    dist = effBTLModelGroupGetSphere(party) / angle;
    radius = pNbCamera->distance;
    if (dist < 1100.0f) {
        pNbCamera->distance = radius + 1100.0f;
    } else {
        pNbCamera->distance = radius + dist;
    }
    nbCameraShot_GroundCollision(pNbCamera);
    return 1;
}

s32 nbCameraBoss_Cut1_Init(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm;
    u32 id;
    u32 skill;

    pForm = effBTLSkillGetInvokeFormation();
    id = effBTLModelGetID(pForm);
    skill = effBTLSkillGetID();
    switch (id) {
    case 0x101:
    case 0x111:
    case 0x113:
    case 0x126:
    case 0x127:
    case 0x128:
        if (skill == 0x99) {
            nbCameraBoss_Sp_JAREIH_Cut1_Set(pNbCamera);
            break;
        }
        return 0;
    case 0x103:
    case 0x124:
        if (skill == 0xA9 || skill == 0xAA) {
            nbCameraBoss_Bsnet_Attack_Cut1_Set(pNbCamera);
        } else {
            nbCameraBoss_Bsnet_Cut1_Set(pNbCamera);
        }
        break;
    case 0x123:
        if (skill == 0xFD) {
            nbCameraBoss_BSARI_MASEHA_Set(pNbCamera);
        } else if (skill == 0xA8) {
            nbCameraBoss_BSARI_SIMANO_Set(pNbCamera);
        } else {
            nbCameraBoss_BSARI_Cut1_Set(pNbCamera);
        }
        break;
    case 0x102:
        if (skill == 0xFD) {
            nbCameraBoss_BSARI_MASEHA_Set(pNbCamera);
        } else if (skill == 0xA8) {
            nbCameraBoss_BSARI_SIMANO_Set(pNbCamera);
        } else {
            nbCameraBoss_BSARI2_Cut1_Set(pNbCamera);
        }
        break;
    case 0x125:
        nbCameraBoss_Bskagu_Cut1_Set(pNbCamera);
        break;
    case 0x108:
        if (skill == 0xDD) {
            nbCameraBoss_Pish_MUHENK_Set(pNbCamera);
        } else {
            nbCameraBoss_Bspish_Cut1_Set(pNbCamera);
        }
        break;
    case 0x158:
        if (skill == 0x111 || skill == 0x10E) {
            nbCameraBoss_BSLUSI_Type = dds3GetRandIntA(0, 2);
            return 1;
        }
    default:
        return 0;
    }
}

/* NON_MATCHING: 33 of 117 words, 2 short; three arms call callees still in
 * assembly, so ee-gcc leaves a plain branch where retail annuls. */
#ifdef NON_MATCHING
s32 nbCameraBoss_Cut1_Run(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm;
    u32 id;
    u32 skill;
    pForm = effBTLSkillGetInvokeFormation();
    id = effBTLModelGetID(pForm);
    skill = effBTLSkillGetID();
    switch (id) {
    case 0x103:
    case 0x124:
        if (skill == 0xA9 || skill == 0xAA) {
            nbCameraBoss_Bsnet_Attack_Cut1_Run(pNbCamera);
        } else {
            nbCameraBoss_Bsnet_Cut1_Run(pNbCamera);
        }
        break;
    case 0x123:
        if (skill == 0xFD) {
            nbCameraBoss_BSARI_MASEHA_Run(pNbCamera);
        } else if (skill == 0xA8) {
            nbCameraBoss_BSARI_SIMANO_Run(pNbCamera);
        } else {
            nbCameraBoss_BSARI_Cut1_Run(pNbCamera);
        }
        break;
    case 0x102:
        if (skill == 0xFD) {
            nbCameraBoss_BSARI_MASEHA_Run(pNbCamera);
        } else if (skill == 0xA8) {
            nbCameraBoss_BSARI_SIMANO_Run(pNbCamera);
        } else {
            nbCameraBoss_BSARI2_Cut1_Run(pNbCamera);
        }
        break;
    case 0x125:
        nbCameraBoss_Bskagu_Cut1_Run(pNbCamera);
        break;
    case 0x108:
        if (skill == 0xDD) {
            nbCameraBoss_Pish_MUHENK_Run(pNbCamera);
        } else {
            nbCameraBoss_Bspish_Cut1_Run(pNbCamera);
        }
        break;
    case 0x158:
        if (skill == 0x10F) {
            nbCameraBoss_Lusi_ASIKIK_Set(pNbCamera);
            break;
        }
        if (skill == 0x111 || skill == 0x10E) {
            nbCameraBoss_Lusi_Cut1_Set(pNbCamera);
            break;
        }
    default:
        return 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut1_Run);
#endif

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Init);

/* NON_MATCHING: 92 of 138 words, 2 short; the 0x11F arm's first `return 1`
 * becomes a local block instead of a delay-slot constant. */
#ifdef NON_MATCHING
s32 nbCameraBoss_Cut2_Run(nbCamera_t *pNbCamera)
{
    nbFormation_t *pForm;
    u32 id;
    u32 skill;

    pForm = effBTLSkillGetInvokeFormation();
    id = effBTLModelGetID(pForm);
    skill = effBTLSkillGetID();
    switch (id) {
    case 0x101:
    case 0x111:
    case 0x113:
    case 0x126:
    case 0x127:
    case 0x128:
        if (skill == 0xFC) {
            nbCameraBoss_Sp_MURET2_Run(pNbCamera);
            break;
        }
        if (skill == 0x99) {
            nbCameraBoss_Sp_JAREIH_Run(pNbCamera);
            break;
        }
        return 0;
    case 0x10D:
        if (skill == 0xE9) {
            nbCameraBoss_Ongy_BUNSIN_Run(pNbCamera);
            break;
        }
        return 0;
    case 0x11F:
        if (skill == 0xF2) {
            nbCameraBoss_Samael_AKUI_Run(pNbCamera);
            break;
        }
        if (skill != 0 && skill != 0xA5) {
            return 0;
        }
        return 1;
    case 0x120:
        if (skill == 0xC9) {
            nbCameraBoss_Baal_BAELNO_Run(pNbCamera);
            break;
        }
        return 0;
    case 0x146:
    case 0x147:
    case 0x148:
        if (skill != 0) {
            return 0;
        }
        return 1;
    case 0x103:
    case 0x124:
        if (skill == 0xA9 || skill == 0xAA) {
            nbCameraBoss_Bsnet_Attack_Run(pNbCamera);
            break;
        }
        return 1;
    case 0x102:
    case 0x123:
        if (skill == 0xFD) {
            nbCameraBoss_BSARI_MASEHA_Run(pNbCamera);
            break;
        }
        if (skill == 0xA8) {
            nbCameraBoss_BSARI_SIMANO_Run(pNbCamera);
            break;
        }
        return 1;
    case 0x125:
        if (skill == 0xF1) {
            nbCameraBoss_Kagu_MJINKO_Set(pNbCamera);
            break;
        }
        return 0;
    case 0x108:
        if (skill == 0xDD) {
            nbCameraBoss_Pish_MUHENK_Run(pNbCamera);
            break;
        }
        return 0;
    case 0x156:
        if (skill == 0xEB) {
            nbCameraBoss_Meta_METAT1_1_Set(pNbCamera);
            break;
        }
        return 0;
    case 0x157:
        if (skill == 0x103) {
            nbCameraBoss_Beru_HAENO1_Set(pNbCamera);
            break;
        }
        return 0;
    case 0x158:
        if (skill == 0x110) {
            nbCameraBoss_Lusi_TWORLD_Set(pNbCamera);
            break;
        }
    default:
        return 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Run);
#endif

s32 nbCameraBoss_CheckRotate(nbFormation_t *pFormation)
{
    switch (effBTLModelGetID(pFormation)) {
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

s32 nbCameraBoss_CheckFix(void)
{
    nbMainProcessData_t *pData;
    nbFormation_t *p;
    s32 i;

    pData = nbGetMainProcessData();
    p = &pData->form[4];
    for (i = 0; i < 10; i++) {
        if ((p->stat & 3) == 3) {
            switch ((u32)effBTLModelGetID(p)) {
            case 0x102:
            case 0x103:
            case 0x108:
            case 0x116:
            case 0x123:
            case 0x124:
            case 0x125:
            case 0x156:
            case 0x157:
            case 0x158:
                return 0;
            }
        }
        p++;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Command_Init);

s32 nbCameraBoss_BSARI1_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pNbCamera->direction.x = -0.26f;
        pNbCamera->direction.y = 0.33f;
        pNbCamera->direction.z = 0.91f;
        pNbCamera->target.x = 746.94f;
        pNbCamera->target.y = -1248.2499f;
        pNbCamera->target.z = -1527.32f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        pNbCamera->direction.x = 0.62f;
        pNbCamera->direction.y = 0.3f;
        pNbCamera->direction.z = 0.73f;
        pNbCamera->target.x = -1308.1398f;
        pNbCamera->target.y = -960.32f;
        pNbCamera->target.z = -1071.01f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI1_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pNbCamera->direction.x = -0.31f;
        pNbCamera->direction.y = -0.1f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = 740.91993f;
        pNbCamera->target.y = -251.30999f;
        pNbCamera->target.z = -1268.8299f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        pNbCamera->direction.x = 0.45f;
        pNbCamera->direction.y = 0.01f;
        pNbCamera->direction.z = 0.89f;
        pNbCamera->target.x = -865.67f;
        pNbCamera->target.y = -294.74f;
        pNbCamera->target.z = -1501.93f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI1_Cut23_EX03_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pNbCamera->direction.x = -0.26f;
        pNbCamera->direction.y = 0.33f;
        pNbCamera->direction.z = 0.91f;
        pNbCamera->target.x = 746.94f;
        pNbCamera->target.y = -1248.2499f;
        pNbCamera->target.z = -1527.32f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.62f;
            pNbCamera->direction.y = 0.3f;
            pNbCamera->direction.z = 0.73f;
            pNbCamera->target.x = -1308.1398f;
            pNbCamera->target.y = -960.32f;
            pNbCamera->target.z = -1071.01f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.58f;
            pNbCamera->direction.y = -0.1f;
            pNbCamera->direction.z = 0.73f;
            pNbCamera->target.x = -956.97f;
            pNbCamera->target.y = -209.06998f;
            pNbCamera->target.z = -799.32f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
            pNbCamera->direction.x = -0.44f;
            pNbCamera->direction.y = -0.18f;
            pNbCamera->direction.z = 0.88f;
            pNbCamera->target.x = 1006.2f;
            pNbCamera->target.y = -104.62999f;
            pNbCamera->target.z = -914.78f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
        } else {
            pNbCamera->direction.x = -0.52f;
            pNbCamera->direction.y = 0.15f;
            pNbCamera->direction.z = 0.84f;
            pNbCamera->target.x = 1143.1f;
            pNbCamera->target.y = -780.76f;
            pNbCamera->target.z = -962.94f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
        }
    } else {
        if ((dds3GetRandA(dds3EffectBase) & 1) != 0) {
            pNbCamera->direction.x = 0.42f;
            pNbCamera->direction.y = -0.16f;
            pNbCamera->direction.z = 0.89f;
            pNbCamera->target.x = -661.40992f;
            pNbCamera->target.y = -45.249997f;
            pNbCamera->target.z = -782.04f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
        } else {
            pNbCamera->direction.x = 0.68f;
            pNbCamera->direction.y = 0.11f;
            pNbCamera->direction.z = 0.72f;
            pNbCamera->target.x = -1340.8499f;
            pNbCamera->target.y = -533.69f;
            pNbCamera->target.z = -711.26f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pNbCamera->direction.x = -0.32f;
        pNbCamera->direction.y = 0.08f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = 774.63f;
        pNbCamera->target.y = -461.97f;
        pNbCamera->target.z = -1267.7199f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        pNbCamera->direction.x = 0.56f;
        pNbCamera->direction.y = 0.06f;
        pNbCamera->direction.z = 0.82f;
        pNbCamera->target.x = -1213.39f;
        pNbCamera->target.y = -383.00995f;
        pNbCamera->target.z = -1264.4799f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_Cut23_EX03_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pNbCamera->direction.x = 0.49f;
    pNbCamera->direction.y = 0.36f;
    pNbCamera->direction.z = 0.8f;
    pNbCamera->target.x = -997.73993f;
    pNbCamera->target.y = -1100.6099f;
    pNbCamera->target.z = -1215.93f;
    pNbCamera->distance = 1.0f;
    pNbCamera->fovy = 0.69813163f;
    return 1;
}

s32 nbCameraBoss_BSARI1_EP_IT_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pNbCamera->direction.x = -0.18f;
        pNbCamera->direction.y = 0.04f;
        pNbCamera->direction.z = 0.98f;
        pNbCamera->target.x = 333.63f;
        pNbCamera->target.y = -282.11f;
        pNbCamera->target.z = -1884.7499f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.57f;
        pNbCamera->direction.y = -0.01f;
        pNbCamera->direction.z = 0.82f;
        pNbCamera->target.x = -1262.9799f;
        pNbCamera->target.y = -197.49999f;
        pNbCamera->target.z = -1548.4999f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.36f;
        pNbCamera->direction.y = -0.12f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = -658.44f;
        pNbCamera->target.y = -47.229996f;
        pNbCamera->target.z = -1726.4599f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = -0.01f;
        pNbCamera->direction.y = 0.06f;
        pNbCamera->direction.z = 1.0f;
        pNbCamera->target.x = -6.07f;
        pNbCamera->target.y = -366.72f;
        pNbCamera->target.z = -2008.05f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSARI2_EP_IT_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pNbCamera->direction.x = -0.18f;
        pNbCamera->direction.y = 0.04f;
        pNbCamera->direction.z = 0.98f;
        pNbCamera->target.x = 333.63f;
        pNbCamera->target.y = -282.11f;
        pNbCamera->target.z = -1884.7499f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.57f;
        pNbCamera->direction.y = -0.01f;
        pNbCamera->direction.z = 0.82f;
        pNbCamera->target.x = -1262.9799f;
        pNbCamera->target.y = -197.49999f;
        pNbCamera->target.z = -1548.4999f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.36f;
        pNbCamera->direction.y = -0.12f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = -658.44f;
        pNbCamera->target.y = -47.229996f;
        pNbCamera->target.z = -1726.4599f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = -0.01f;
        pNbCamera->direction.y = 0.06f;
        pNbCamera->direction.z = 1.0f;
        pNbCamera->target.x = -6.07f;
        pNbCamera->target.y = -366.72f;
        pNbCamera->target.z = -2008.05f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSNET1_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pNbCamera->direction.x = -0.28f;
            pNbCamera->direction.y = 0.3f;
            pNbCamera->direction.z = 0.91f;
            pNbCamera->target.x = 1068.39f;
            pNbCamera->target.y = -1047.2299f;
            pNbCamera->target.z = -1474.66f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pNbCamera->direction.x = 0.26f;
            pNbCamera->direction.y = 0.14f;
            pNbCamera->direction.z = 0.96f;
            pNbCamera->target.x = -515.2f;
            pNbCamera->target.y = -573.76f;
            pNbCamera->target.z = -1415.0599f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSNET_EP_IT_Set(nbCamera_t *pNbCamera)
{
    effBTLModelAllShow();
    switch (dds3GetRandIntA(0, 5)) {
    case 0:
        pNbCamera->direction.x = -0.09f;
        pNbCamera->direction.y = -0.02f;
        pNbCamera->direction.z = 1.0f;
        pNbCamera->target.x = 308.80997f;
        pNbCamera->target.y = -254.05f;
        pNbCamera->target.z = -2068.33f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
    case 2:
        pNbCamera->direction.x = 0.41f;
        pNbCamera->direction.y = 0.0f;
        pNbCamera->direction.z = 0.91f;
        pNbCamera->target.x = -793.4f;
        pNbCamera->target.y = -293.23f;
        pNbCamera->target.z = -1708.04f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = -0.22f;
        pNbCamera->direction.y = 0.32f;
        pNbCamera->direction.z = 0.92f;
        pNbCamera->target.x = 426.57f;
        pNbCamera->target.y = -775.52f;
        pNbCamera->target.z = -1754.41f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 4:
        pNbCamera->direction.x = 0.19f;
        pNbCamera->direction.y = 0.39f;
        pNbCamera->direction.z = 0.9f;
        pNbCamera->target.x = -330.54f;
        pNbCamera->target.y = -925.6f;
        pNbCamera->target.z = -1622.9899f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}


/* NON_MATCHING: 19 of 79 words; the bodies are exact, the selector movz is not. */
#ifdef NON_MATCHING
s32 nbCameraBoss_BSKAGU_EP_IT_Set(nbCamera_t *pNbCamera)
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
        pNbCamera->direction.x = -0.31f;
        pNbCamera->direction.y = 0.01f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = 617.1609f;
        pNbCamera->target.y = -279.74997f;
        pNbCamera->target.z = -1764.7099f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = 0.28f;
        pNbCamera->direction.y = 0.01f;
        pNbCamera->direction.z = 0.96f;
        pNbCamera->target.x = -448.85996f;
        pNbCamera->target.y = -282.52997f;
        pNbCamera->target.z = -1788.65f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.47f;
        pNbCamera->direction.y = -0.11f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = -870.40992f;
        pNbCamera->target.y = -34.279995f;
        pNbCamera->target.z = -1640.9098f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = -0.22f;
        pNbCamera->direction.y = 0.22f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = 475.55f;
        pNbCamera->target.y = -659.79993f;
        pNbCamera->target.z = -1966.0599f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_BSKAGU_EP_IT_Set);
#endif


s32 nbCameraBoss_BSBERU_EP_IT_Set(nbCamera_t *pNbCamera)
{
    u32 n;

    effBTLModelAllShow();
    n = dds3GetRandIntA(0, 4);
    switch (n) {
    case 0:
        pNbCamera->direction.x = -0.4f;
        pNbCamera->direction.y = -0.07f;
        pNbCamera->direction.z = 0.92f;
        pNbCamera->target.x = 550.54993f;
        pNbCamera->target.y = -129.88999f;
        pNbCamera->target.z = -1359.81f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = -0.33f;
        pNbCamera->direction.y = 0.22f;
        pNbCamera->direction.z = 0.92f;
        pNbCamera->target.x = 534.35993f;
        pNbCamera->target.y = -549.46f;
        pNbCamera->target.z = -1439.95f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.41f;
        pNbCamera->direction.y = -0.07f;
        pNbCamera->direction.z = 0.91f;
        pNbCamera->target.x = -700.19f;
        pNbCamera->target.y = -184.47999f;
        pNbCamera->target.z = -1440.54f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = 0.46f;
        pNbCamera->direction.y = 0.22f;
        pNbCamera->direction.z = 0.86f;
        pNbCamera->target.x = -796.84f;
        pNbCamera->target.y = -586.89f;
        pNbCamera->target.z = -1466.5999f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_EP_IT_Set(nbCamera_t *pNbCamera)
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
        pNbCamera->direction.x = -0.35f;
        pNbCamera->direction.y = -0.17f;
        pNbCamera->direction.z = 0.92f;
        pNbCamera->target.x = 832.66993f;
        pNbCamera->target.y = -56.02f;
        pNbCamera->target.z = -1851.29f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.7853981f;
        break;
    case 1:
        pNbCamera->direction.x = 0.42f;
        pNbCamera->direction.y = -0.19f;
        pNbCamera->direction.z = 0.89f;
        pNbCamera->target.x = -1114.79f;
        pNbCamera->target.y = -41.68f;
        pNbCamera->target.z = -1686.27f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.7853981f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSKAGU_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        pNbCamera->direction.x = -0.61f;
        pNbCamera->direction.y = 0.29f;
        pNbCamera->direction.z = 0.74f;
        pNbCamera->target.x = 1385.7698f;
        pNbCamera->target.y = -1184.8399f;
        pNbCamera->target.z = -892.5199f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        pNbCamera->direction.x = 0.64f;
        pNbCamera->direction.y = -0.03f;
        pNbCamera->direction.z = 0.77f;
        pNbCamera->target.x = -1443.05f;
        pNbCamera->target.y = -321.31f;
        pNbCamera->target.z = -830.79f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    }
    return 1;
}

s32 nbCameraBoss_BSKAGU_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pNbCamera->direction.x = -0.64f;
            pNbCamera->direction.y = 0.3f;
            pNbCamera->direction.z = 0.71f;
            pNbCamera->target.x = 1255.7399f;
            pNbCamera->target.y = -813.03992f;
            pNbCamera->target.z = -819.09f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.52f;
            pNbCamera->direction.y = 0.03f;
            pNbCamera->direction.z = 0.86f;
            pNbCamera->target.x = 997.13f;
            pNbCamera->target.y = -372.64997f;
            pNbCamera->target.z = -1053.4299f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 2);
        switch (n) {
        case 0:
            pNbCamera->direction.x = 0.58f;
            pNbCamera->direction.y = 0.07f;
            pNbCamera->direction.z = 0.81f;
            pNbCamera->target.x = -1318.2299f;
            pNbCamera->target.y = -430.66f;
            pNbCamera->target.z = -1078.51f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_EP_IT_Set(nbCamera_t *pNbCamera)
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
        pNbCamera->direction.x = 0.48f;
        pNbCamera->direction.y = -0.15f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = -986.15992f;
        pNbCamera->target.y = -42.259995f;
        pNbCamera->target.z = -1594.4098f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 1:
        pNbCamera->direction.x = -0.31f;
        pNbCamera->direction.y = 0.01f;
        pNbCamera->direction.z = 0.95f;
        pNbCamera->target.x = 617.1609f;
        pNbCamera->target.y = -279.74997f;
        pNbCamera->target.z = -1764.7099f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 2:
        pNbCamera->direction.x = 0.57f;
        pNbCamera->direction.y = -0.02f;
        pNbCamera->direction.z = 0.82f;
        pNbCamera->target.x = -1407.16f;
        pNbCamera->target.y = -256.49997f;
        pNbCamera->target.z = -1795.4799f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 3:
        pNbCamera->direction.x = -0.36f;
        pNbCamera->direction.y = -0.07f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = 706.98993f;
        pNbCamera->target.y = -152.13f;
        pNbCamera->target.z = -1804.0999f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 4:
        pNbCamera->direction.x = 0.28f;
        pNbCamera->direction.y = 0.01f;
        pNbCamera->direction.z = 0.96f;
        pNbCamera->target.x = -448.85996f;
        pNbCamera->target.y = -282.52997f;
        pNbCamera->target.z = -1788.65f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    case 5:
        pNbCamera->direction.x = 0.47f;
        pNbCamera->direction.y = -0.11f;
        pNbCamera->direction.z = 0.87f;
        pNbCamera->target.x = -870.40992f;
        pNbCamera->target.y = -34.279995f;
        pNbCamera->target.z = -1640.9098f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
        break;
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pNbCamera->direction.x = -0.38f;
        pNbCamera->direction.y = -0.01f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = 950.09f;
        pNbCamera->target.y = -379.85996f;
        pNbCamera->target.z = -662.6399f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.54f;
            pNbCamera->direction.y = -0.14f;
            pNbCamera->direction.z = 0.83f;
            pNbCamera->target.x = -1457.63f;
            pNbCamera->target.y = -83.44f;
            pNbCamera->target.z = -507.48f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.42f;
            pNbCamera->direction.y = 0.42f;
            pNbCamera->direction.z = 0.8f;
            pNbCamera->target.x = -1429.4899f;
            pNbCamera->target.y = -1608.3599f;
            pNbCamera->target.z = -973.89991f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u8 work[0x30];
    u32 n;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pNbCamera->direction.x = -0.17f;
            pNbCamera->direction.y = 0.09f;
            pNbCamera->direction.z = 0.98f;
            pNbCamera->target.x = 519.72992f;
            pNbCamera->target.y = -555.24994f;
            pNbCamera->target.z = -1470.1099f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.45f;
            pNbCamera->direction.y = 0.14f;
            pNbCamera->direction.z = 0.88f;
            pNbCamera->target.x = 1523.54f;
            pNbCamera->target.y = -841.15f;
            pNbCamera->target.z = -1597.79f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    } else {
        n = dds3GetRandIntA(0, 3);
        switch (n) {
        case 0:
            pNbCamera->direction.x = 0.3f;
            pNbCamera->direction.y = -0.01f;
            pNbCamera->direction.z = 0.95f;
            pNbCamera->target.x = -823.9599f;
            pNbCamera->target.y = -244.6f;
            pNbCamera->target.z = -1453.4499f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.58f;
            pNbCamera->direction.y = 0.07f;
            pNbCamera->direction.z = 0.81f;
            pNbCamera->target.x = -1318.2299f;
            pNbCamera->target.y = -430.66f;
            pNbCamera->target.z = -1078.51f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 2:
            nbCameraShot_InvokeTarget_Set2(pNbCamera, work);
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSPISH_Cut23_EX03_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x > pInvoke->orgpos.x) {
        pNbCamera->direction.x = -0.38f;
        pNbCamera->direction.y = -0.01f;
        pNbCamera->direction.z = 0.93f;
        pNbCamera->target.x = 950.09f;
        pNbCamera->target.y = -379.85996f;
        pNbCamera->target.z = -662.6399f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.69813163f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.54f;
            pNbCamera->direction.y = -0.14f;
            pNbCamera->direction.z = 0.83f;
            pNbCamera->target.x = -1457.63f;
            pNbCamera->target.y = -83.44f;
            pNbCamera->target.z = -507.48f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.42f;
            pNbCamera->direction.y = 0.42f;
            pNbCamera->direction.z = 0.8f;
            pNbCamera->target.x = -1429.4899f;
            pNbCamera->target.y = -1608.3599f;
            pNbCamera->target.z = -973.89991f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSMETA_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.3f;
            pNbCamera->direction.y = 0.28f;
            pNbCamera->direction.z = 0.91f;
            pNbCamera->target.x = 780.83991f;
            pNbCamera->target.y = -876.85993f;
            pNbCamera->target.z = -1185.59f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.49f;
            pNbCamera->direction.y = -0.26f;
            pNbCamera->direction.z = 0.83f;
            pNbCamera->target.x = 607.6999f;
            pNbCamera->target.y = -37.06f;
            pNbCamera->target.z = -203.45f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.62f;
            pNbCamera->direction.y = 0.37f;
            pNbCamera->direction.z = 0.7f;
            pNbCamera->target.x = -1387.0398f;
            pNbCamera->target.y = -1122.9098f;
            pNbCamera->target.z = -824.89f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.72f;
            pNbCamera->direction.y = -0.11f;
            pNbCamera->direction.z = 0.69f;
            pNbCamera->target.x = -1111.4899f;
            pNbCamera->target.y = -88.11f;
            pNbCamera->target.z = -332.72f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSMETA_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x < pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.4f;
            pNbCamera->direction.y = -0.09f;
            pNbCamera->direction.z = 0.91f;
            pNbCamera->target.x = 488.05997f;
            pNbCamera->target.y = -118.58f;
            pNbCamera->target.z = -800.80994f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.09f;
            pNbCamera->direction.y = -0.18f;
            pNbCamera->direction.z = 0.98f;
            pNbCamera->target.x = 184.78998f;
            pNbCamera->target.y = -75.7099915f;
            pNbCamera->target.z = -724.27f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.64f;
            pNbCamera->direction.y = 0.11f;
            pNbCamera->direction.z = 0.76f;
            pNbCamera->target.x = -1315.6f;
            pNbCamera->target.y = -465.79f;
            pNbCamera->target.z = -1038.3699f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.36f;
            pNbCamera->direction.y = -0.03f;
            pNbCamera->direction.z = 0.93f;
            pNbCamera->target.x = -521.849915f;
            pNbCamera->target.y = -154.85f;
            pNbCamera->target.z = -971.6399f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSBERU_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.55f;
            pNbCamera->direction.y = -0.13f;
            pNbCamera->direction.z = 0.83f;
            pNbCamera->target.x = -482.02997f;
            pNbCamera->target.y = -100.239991f;
            pNbCamera->target.z = -585.49994f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.5f;
            pNbCamera->direction.y = 0.45f;
            pNbCamera->direction.z = 0.74f;
            pNbCamera->target.x = -672.03f;
            pNbCamera->target.y = -827.14f;
            pNbCamera->target.z = -935.79993f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.04f;
            pNbCamera->direction.y = 0.08f;
            pNbCamera->direction.z = 1.0f;
            pNbCamera->target.x = -69.98f;
            pNbCamera->target.y = -307.73996f;
            pNbCamera->target.z = -768.24f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.31f;
            pNbCamera->direction.y = 0.4f;
            pNbCamera->direction.z = 0.87f;
            pNbCamera->target.x = 358.62995f;
            pNbCamera->target.y = -751.66993f;
            pNbCamera->target.z = -924.78992f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSBERU_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.47f;
            pNbCamera->direction.y = -0.24f;
            pNbCamera->direction.z = 0.85f;
            pNbCamera->target.x = -412.12f;
            pNbCamera->target.y = -76.77f;
            pNbCamera->target.z = -604.7f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = 0.55f;
            pNbCamera->direction.y = 0.28f;
            pNbCamera->direction.z = 0.79f;
            pNbCamera->target.x = -730.98993f;
            pNbCamera->target.y = -657.599915f;
            pNbCamera->target.z = -986.97992f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.38f;
            pNbCamera->direction.y = -0.14f;
            pNbCamera->direction.z = 0.92f;
            pNbCamera->target.x = 393.21f;
            pNbCamera->target.y = -115.28f;
            pNbCamera->target.z = -756.429932f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        case 1:
            pNbCamera->direction.x = -0.58f;
            pNbCamera->direction.y = 0.28f;
            pNbCamera->direction.z = 0.76f;
            pNbCamera->target.x = 756.9f;
            pNbCamera->target.y = -554.929932f;
            pNbCamera->target.z = -776.30994f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.69813163f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_EX01_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pTarget->orgpos.x <= pInvoke->orgpos.x) {
        pNbCamera->direction.x = 0.29f;
        pNbCamera->direction.y = -0.27f;
        pNbCamera->direction.z = 0.92f;
        pNbCamera->target.x = -513.85993f;
        pNbCamera->target.y = -36.889996f;
        pNbCamera->target.z = -954.55994f;
        pNbCamera->distance = 1.0f;
        pNbCamera->fovy = 0.87266452f;
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.58f;
            pNbCamera->direction.y = 0.01f;
            pNbCamera->direction.z = 0.81f;
            pNbCamera->target.x = 1516.4499f;
            pNbCamera->target.y = -646.57f;
            pNbCamera->target.z = -1426.9f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        case 1:
            pNbCamera->direction.x = -0.39f;
            pNbCamera->direction.y = -0.39f;
            pNbCamera->direction.z = 0.84f;
            pNbCamera->target.x = 479.86997f;
            pNbCamera->target.y = -41.26f;
            pNbCamera->target.z = -491.59995f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_EX02_Set(nbCamera_t *pNbCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    effBTLModelAllShow();
    effBTLModelInvokeGroupHide();
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    if (pInvoke->orgpos.x >= pTarget->orgpos.x) {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = 0.39f;
            pNbCamera->direction.y = 0.34f;
            pNbCamera->direction.z = 0.86f;
            pNbCamera->target.x = -1069.34f;
            pNbCamera->target.y = -1393.5999f;
            pNbCamera->target.z = -2041.3499f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        case 1:
            pNbCamera->direction.x = 0.29f;
            pNbCamera->direction.y = -0.27f;
            pNbCamera->direction.z = 0.92f;
            pNbCamera->target.x = -513.85993f;
            pNbCamera->target.y = -36.889996f;
            pNbCamera->target.z = -954.55994f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        }
    } else {
        switch (dds3GetRandIntA(0, 2)) {
        case 0:
            pNbCamera->direction.x = -0.58f;
            pNbCamera->direction.y = 0.01f;
            pNbCamera->direction.z = 0.81f;
            pNbCamera->target.x = 1516.4499f;
            pNbCamera->target.y = -646.57f;
            pNbCamera->target.z = -1426.9f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        case 1:
            pNbCamera->direction.x = -0.27f;
            pNbCamera->direction.y = -0.26f;
            pNbCamera->direction.z = 0.93f;
            pNbCamera->target.x = 298.16f;
            pNbCamera->target.y = -131.8f;
            pNbCamera->target.z = -999.899902344f;
            pNbCamera->distance = 1.0f;
            pNbCamera->fovy = 0.87266452f;
            break;
        }
    }
    return 1;
}

s32 nbCameraBoss_BSLUSI_Cut23_AllParty_Set(nbCamera_t *pNbCamera)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraBoss_Cut2_Ptr_Init);

s32 nbCameraBoss_Cut2_Ptr_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    return 0;
}

s32 nbCameraHero_Cut1_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    nbMainProcessData_t *pData;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->stat & 8) {
        return 0;
    }
    if (effBTLModelGetID(effBTLSkillGetInvokeFormation()) != 0) {
        return 0;
    }
    nbCameraHero_Cut_Frame = 0;
    if (nbCameraHero_Cut2_Run(pNbCamera, pNBSInfo) == 0) {
        if (pNBSInfo->ptr_1 != 6) {
            return 0;
        }
        nbCameraShot_PullOne_Init(pNbCamera);
    }
    return 1;
}

s32 nbCameraHero_Cut1_Run(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    nbMainProcessData_t *pData;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->stat & 8) {
        return 0;
    }
    if (effBTLModelGetID(effBTLSkillGetInvokeFormation()) != 0) {
        return 0;
    }
    if (nbCameraHero_Cut2_Run(pNbCamera, pNBSInfo) != 0) {
        return 1;
    }
    if (pNBSInfo->ptr_1 != 6) {
        return 0;
    }
    nbCameraShot_PullOne_Calc(pNbCamera, 0.45f);
    if (pNbCamera->frame < datModelParamTable[0].part[11].actframe) {
        SetRotateMatrixY(0.017453291f);
        SDF_VU0_LOADVECTOR(&pNbCamera->direction);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    }
    return 1;
}

s32 nbCameraHero_Cut2_Init(nbCamera_t *pNbCamera, nbCameraSkillInfo_t *pNBSInfo)
{
    nbMainProcessData_t *pData;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return 0;
    }
    if (pData->stat & 8) {
        return 0;
    }
    if (effBTLModelGetID(effBTLSkillGetInvokeFormation()) != 0) {
        return 0;
    }
    switch (effBTLSkillGetID()) {
    case 0x83:
    case 0x85:
    case 0x88:
    case 0x8F:
    case 0x90:
    case 0x93:
    case 0xA0:
    case 0xA1:
    case 0xA3:
        return 1;
    case 0xF9:
        return 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraHero_Cut2_Run);

INCLUDE_RODATA(const s32, "newbattle/nbCameraBoss", D_005357E0);

INCLUDE_ASM(const s32, "newbattle/nbCameraBoss", nbCameraDANTE_Init);

s32 nbCameraDANTE_Run(nbCamera_t *pCamera)
{
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;
    u32 state;

    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget = effBTLSkillGetTargetFormation(0);
    switch ((u32)effBTLModelGetID(pInvoke)) {
    case 0xC0:
    case 0x153:
    case 0x154:
    case 0x155:
        break;
    default:
        return 0;
    }
    state = effBTLSkillGetState();
    if (effBTLModelGetBadStatus(pInvoke) & 0x40) {
        return 0;
    }
    if (state & 0xC) {
        return 0;
    }
    switch (effBTLSkillGetID()) {
    case 0x107:
        switch ((u32)effBTLModelGetID(pTarget)) {
        case 0x158:
            nbCameraDANTE_DCOMBO_Set(pCamera->frame);
            break;
        case 0x108:
        case 0x125:
            nbCameraDANTE_DCOMBO_KAGU_Set(pCamera->frame);
            break;
        }
        return 1;
    case 0x106:
    case 0x108:
    case 0x109:
    case 0x10A:
    case 0x10C:
        return 0;
    case 0x10B:
    case 0x10D:
        break;
    default:
        return 0;
    }
    return 1;
}

s32 nbCameraDANTE_DCOMBO_Set(s32 frame)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate;

    rate = frame / 180.0f;
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    from.x = -955.19f;
    from.y = -42.79f;
    from.z = -948.07f;
    to.x = 580.33991f;
    to.y = -57.76f;
    to.z = -834.14f;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(rate);
    SDF_VU0_STOREVECTOR(pCamera);
    from.x = 0.66f;
    from.y = -0.32f;
    from.z = 0.68f;
    to.x = -0.36f;
    to.y = -0.19f;
    to.z = 0.91f;
    SDF_VU0_LOADVECTOR(&from);
    SDF_VU0_LOADVECTOR_B(&to);
    VU0_LERP_VF10_VF11(rate);
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->distance = 1.0f;
    pCamera->fovy = 0.7853981f;
    return 1;
}

s32 nbCameraDANTE_KAGIRI_Set(s32 frame)
{
    nbCamera_t *pCamera;
    nbFormation_t *pInvoke;
    nbFormation_t *pTarget;

    if (frame != 0) {
        return 1;
    }
    {
        pCamera = &pNbCamera;
        pInvoke = effBTLSkillGetInvokeFormation();
        pTarget = effBTLSkillGetTargetFormation(0);
        if (pInvoke->orgpos.x < pTarget->orgpos.x) {
            pCamera->direction.x = -0.28f;
            pCamera->direction.y = -0.05f;
            pCamera->direction.z = 0.96f;
            pCamera->target.x = 765.98993f;
            pCamera->target.y = -455.17997f;
            pCamera->target.z = -1640.78f;
        } else {
            pCamera->direction.x = 0.44f;
            pCamera->direction.y = -0.24f;
            pCamera->direction.z = 0.87f;
            pCamera->target.x = -709.38f;
            pCamera->target.y = -40.54f;
            pCamera->target.z = -1283.63f;
        }
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.7853981f;
    }
    return 1;
}

s32 nbCameraDANTE_DCOMBO_KAGU_Set(s32 frame)
{
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t from;
    sdfVector4_t to;
    f32 rate;

    if (frame < 70) {
        rate = frame / 101.0f;
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        from.x = 764.57f;
        from.y = -204.63f;
        from.z = -578.03f;
        to.x = -807.44f;
        to.y = -260.23f;
        to.z = -645.38f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        VU0_LERP_VF10_VF11(rate);
        SDF_VU0_STOREVECTOR(pCamera);
        from.x = -0.44f;
        from.y = -0.1f;
        from.z = 0.89f;
        to.x = 0.43f;
        to.y = 0.01f;
        to.z = 0.9f;
        SDF_VU0_LOADVECTOR(&from);
        SDF_VU0_LOADVECTOR_B(&to);
        VU0_LERP_VF10_VF11(rate);
        SDF_VU0_STOREVECTOR(&pCamera->direction);
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    } else if (frame == 70) {
        pCamera->direction.x = -0.36f;
        pCamera->direction.y = -0.19f;
        pCamera->direction.z = 0.91f;
        pCamera->target.x = 524.35993f;
        pCamera->target.y = -51.69f;
        pCamera->target.z = -408.83f;
        pCamera->distance = 1.0f;
        pCamera->fovy = 0.69813163f;
    }
    return 1;
}
