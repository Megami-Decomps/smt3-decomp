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
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);
extern void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);

extern void effBTLModelAllHide(void);

extern s32 effBTLModelGetMarkPosition(nbFormation_t *pFormation, s32 mark);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern s32 effBTLModelGetID(nbFormation_t *pFormation);

extern u128 D_00324740;

extern nbCamera_t pNbCamera;

extern s32 nbCamera_NegoBougai_Wait;

extern f32 nbCamera_NegoHojo_Rate;

extern f32 nbCamera_NegoHojo_Distance;

extern void effBTLModelTargetGroupHide(void);

extern void effBTLModelInvokeGroupHide(void);

extern sdfVector4_t nbCameraNegoBougaiVec;

extern f32 nbCamera_NegoBougai_Rate;

extern f32 nbCamera_NegoBougai_Distance;

extern void ToMatrix(void);

extern f32 sdfAtan(f32 value);

extern s32 nbCamera_NegoBougai_Frame;

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

void nbCameraState_NegoBougai_Calc(void)
{
    nbFormation_t *pFormation = effBTLSkillGetTargetFormation(0);
    nbCamera_t *pCamera = &pNbCamera;
    sdfVector4_t scale;
    sdfVector4_t focus;
    f32 length;
    f32 base;
    f32 want;

    if ((pFormation->stat & 3) == 3) {
        if (nbCamera_NegoBougai_Frame >= nbCamera_NegoBougai_Wait) {
            nbCamera_NegoBougai_Rate *= 0x1.ab851ep-1f;
            scale.x = -pCamera->distance;
            scale.y = -pCamera->distance;
            scale.z = -pCamera->distance;
            SDF_VU0_LOADVECTOR(&pCamera->direction);
            SDF_VU0_LOADVECTOR_B(&scale);
            SDF_VU0_MULVECTOR();
            SDF_VU0_LOADVECTOR_B(pCamera);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&focus);
            effBTLModelGetMarkPosition2(pFormation, 1);
            SDF_VU0_LOADVECTOR_B(&nbCameraNegoBougaiVec);
            VU0_LERP_VF10_VF11(nbCamera_NegoBougai_Rate);
            SDF_VU0_STOREVECTOR(pCamera);
            SDF_VU0_LOADVECTOR_B(&focus);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETLENGTH(length);
            pCamera->distance = length;
            if (0x1.0624dcp-10f < nbCamera_NegoBougai_Rate) {
                SDF_VU0_COPYVECTOR_C_TO_A();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(&pCamera->direction);
            }
            pCamera->fovy = 0x1.657182p-1f;
            base = pFormation->oneshotr * pFormation->scl * 1.25f / sdfAtan(0x1.657182p-2f);
            want = base + (nbCamera_NegoBougai_Distance - base) * nbCamera_NegoBougai_Rate;
            pCamera->distance = want;
            if (want < base) {
                pCamera->distance = base;
            }
        }
        nbCamera_NegoBougai_Frame++;
    }
}

void nbCameraState_NegoTaizi_Init(void)
{
    effBTLModelTargetGroupHide();
    effBTLModelInvokeGroupHide();
}

extern u32 effBTLSkillGetTargetParty(void);
extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern void SetRotateMatrixY(f32 angle);
extern f32 sdfSin(f32 angle);
extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);
extern void nbCameraShot_GroundCollision(nbCamera_t *pCamera);

void nbCameraState_NegoTaizi_Calc(void)
{
    nbCamera_t *pCamera = &pNbCamera;
    nbFormation_t *pTarget;
    nbFormation_t *pInvoke;
    sdfVector4_t targetCenter;
    sdfVector4_t invokeCenter;
    f32 angle;
    f32 length;
    f32 range;
    f32 height;

    effBTLSkillGetTargetParty();
    pTarget = effBTLSkillGetTargetFormation(0);
    pInvoke = effBTLSkillGetInvokeFormation();
    if ((pTarget->stat & 3) != 3 || (pInvoke->stat & 3) != 3) {
        return;
    }

    pCamera->fovy = 0x1.657182p-1f;
    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR(&invokeCenter);
    invokeCenter.y -= pInvoke->r * 0.5f;
    effBTLModelGetSphereCenter(pTarget);
    SDF_VU0_STOREVECTOR(&targetCenter);
    SDF_VU0_LOADVECTOR(&targetCenter);
    SDF_VU0_LOADVECTOR_B(&invokeCenter);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(&pCamera->target);

    if (pCamera->frame == 0) {
        if (invokeCenter.x > targetCenter.x) {
            pCamera->angle = 0x1.0c152p-1f;
        } else {
            pCamera->angle = -0x1.0c152p-1f;
        }
    }
    angle = pCamera->angle;

    SDF_VU0_LOADVECTOR(&invokeCenter);
    SDF_VU0_LOADVECTOR_B(&targetCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    range = length + pInvoke->r * pInvoke->scl + pTarget->r * pTarget->scl;
    SetRotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&targetCenter);
    SDF_VU0_LOADVECTOR_B(&invokeCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pCamera->direction);

    range *= sdfSin(sdfFabs(angle));
    height = effBTLModelGetHeight(pInvoke) - pInvoke->r * pInvoke->scl * 0.5f;
    if (range < height) {
        range = height;
    }
    height = effBTLModelGetHeight(pTarget) - pTarget->r * pInvoke->scl * 0.5f;
    if (range < height) {
        range = height;
    }
    pCamera->distance = (range * 0.5f) / sdfAtan(pCamera->fovy * 0.5f);
    nbCameraShot_GroundCollision(pCamera);
}

void nbCameraState_NegoTarget_Init(void)
{
    nbFormation_t *pForm;
    nbCamera_t *pCamera;

    effBTLModelAllHide();
    effBTLModelTargetGroupHide();
    pCamera = &pNbCamera;
    pForm = effBTLSkillGetTargetFormation(0);
    if ((pForm->stat & 3) == 3) {
        pCamera->direction.x = 0.0f;
        pCamera->direction.y = 0.0f;
        pCamera->direction.z = -1.0f;
        SDF_VU0_LOADVECTOR(&pForm->rot);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&pCamera->direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&pCamera->direction);
        pCamera->fovy = 0x1.657182p-1f;
        pCamera->distance = pForm->oneshotr * pForm->scl / sdfAtan(0x1.657182p-2f);
        if (effBTLModelGetMarkPosition(pForm, 1) == 0) {
            effBTLModelGetSphereCenter(pForm);
        }
        SDF_VU0_STOREVECTOR(pCamera);
    }
}

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

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);
extern void effBTLModelAllShow(void);
extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);
extern void effBTLGetGroupPos(nbFormation_t *pFormation, sdfVector4_t *pPos);
extern void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos);
extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);

typedef struct nbCamera_AllTable_t {
    f32 distance;
    f32 up;
    f32 radius;
} nbCamera_AllTable_t;

extern nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation);

void nbCameraState_NegoHojo_Init(void)
{
    nbFormation_t *pTarget[2];
    f32 len[2];
    sdfVector4_t pos;
    sdfVector4_t mid;
    sdfVector4_t center[2];
    sdfVector4_t invokeCenter;
    nbMainProcessData_t *pData;
    nbCamera_t *pCamera;
    nbFormation_t *pInvoke;
    nbCamera_AllTable_t *pTable;
    nbFormation_t *p;
    f32 maxlen;
    f32 want;
    f32 add;
    f32 delta;
    f32 atanv;
    f32 width;
    u32 i;

    pData = nbGetMainProcessData();
    if (pData == NULL) {
        return;
    }
    pCamera = &pNbCamera;
    pInvoke = effBTLSkillGetInvokeFormation();
    pTarget[0] = effBTLSkillGetTargetFormation(0);
    pTarget[1] = effBTLSkillGetTargetFormation(1);
    effBTLModelAllShow();
    pTable = nbCameraGetModelAllTable(pInvoke);

    effBTLModelGetSphereCenter(pTarget[0]);
    SDF_VU0_STOREVECTOR_NOMEM(&center[0]);
    effBTLModelGetSphereCenter(pTarget[1]);
    SDF_VU0_STOREVECTOR_NOMEM(&center[1]);
    SDF_VU0_LOADVECTOR_B(&center[0]);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(&mid);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&center[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len[0]);
    if (pTarget[0]->h * 0.5f < pTarget[0]->r) {
        len[0] = len[0] + pTarget[0]->r * pTarget[0]->scl;
    } else {
        len[0] = len[0] + pTarget[0]->h * pTarget[0]->scl * 0.5f;
    }
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&center[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len[1]);
    if (pTarget[1]->h * 0.5f < pTarget[1]->r) {
        len[1] = len[1] + pTarget[1]->r * pTarget[1]->scl;
    } else {
        len[1] = len[1] + pTarget[1]->h * pTarget[1]->scl * 0.5f;
    }
    maxlen = len[1] < len[0] ? len[0] : len[1];

    effBTLModelGetSphereCenter(pInvoke);
    SDF_VU0_STOREVECTOR(&invokeCenter);
    invokeCenter.y = -effBTLModelGetHeight(pInvoke);
    invokeCenter.y = invokeCenter.y + pTable->up * pInvoke->scl;

    SDF_VU0_LOADVECTOR(&mid);
    SDF_VU0_STOREVECTOR(pCamera);
    SDF_VU0_LOADVECTOR_B(&invokeCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_GETLENGTH(pCamera->distance);

    atanv = sdfAtan(0x1.657182p-2f);
    width = pInvoke->r * pInvoke->scl;
    if (pTable->distance == 0.0f) {
        if (150.0f < width) {
            add = width / atanv;
        } else if (90.0f < width) {
            add = width * 1.5f / atanv;
        } else {
            add = (width + width) / atanv;
        }
    } else {
        add = pTable->distance * pInvoke->scl;
    }
    pCamera->distance = pCamera->distance + add;

    want = maxlen / sdfAtan(0x1.fb91dp-2f);
    if (pCamera->distance < want) {
        delta = want - pCamera->distance;
        p = pData->form;
        for (i = 0; i < 4; i++) {
            if ((p->stat & 0x103) == 0x103) {
                effBTLGetGroupPos(p, &pos);
                pos.z -= delta;
                effBTLModelSetPosition(p, &pos);
            }
            p++;
        }
        pCamera->distance = want;
        invokeCenter.z -= delta;
    }

    SDF_VU0_LOADVECTOR(&mid);
    SDF_VU0_LOADVECTOR_B(&invokeCenter);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0x1.657182p-1f;
    effBTLModelLook(pInvoke, &mid);
    nbCamera_NegoHojo_Distance = pCamera->distance;
    nbCamera_NegoHojo_Rate = 0.5f;
}

void nbCameraState_NegoHojo_Calc(void)
{
    nbCamera_NegoHojo_Rate = nbCamera_NegoHojo_Rate * 0.925f;
    pNbCamera.distance = nbCamera_NegoHojo_Distance * (1.0f - nbCamera_NegoHojo_Rate);
}
