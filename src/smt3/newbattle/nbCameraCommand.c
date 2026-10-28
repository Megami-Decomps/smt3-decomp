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

extern void effBTLModelGroupGetCenter(u32 group);

extern f32 nbCameraGroupGetMaxLength(sdfVector4_t *pPos, u32 group);

extern void nbCameraShot_CommandFixPos_Set2(nbCamera_t *pNbCamera);

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

extern f32 effBTLModelGroupGetSphere(u32 group);

extern void effBTLModelGroupGetDirection(u32 group, f32 *pDirection);

extern f32 sdfAtan(f32 aX);

extern void SetRotateMatrixX(f32 aAngle);

extern u32 effBTLSkillGetInvokeParty(void);

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern s32 effBTLDebug_CameraAllTableCheck(void);

extern u8 nbCameraCRSetPlayerFormRequestFlag;

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void effBTLModelAllShow(void);

/* 12-byte rows; nbCameraGetModelAllTable indexes them by unit work slot. */
typedef struct nbCamera_AllTable_t {
    f32 distance;
    f32 up;
    f32 radius;
} nbCamera_AllTable_t;

extern nbCamera_AllTable_t nbCameraGetModelAllTable_Dummy[];

extern nbCamera_AllTable_t D_00325950;

extern nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation);

nbCamera_AllTable_t *nbCameraGetModelAllTable(nbFormation_t *pFormation)
{
    datUnitWork_t *pWork;

    if (pFormation == NULL) {
        return &D_00325950;
    }
    if ((pFormation->stat & 1) == 0) {
        return &D_00325950;
    }
    pWork = nbGetUnitWorkFromFormindex(pFormation->formindex);
    if (effBTLDebug_CameraAllTableCheck() == 0) {
        return &D_00325950;
    }
    if ((pWork->flag & 4) != 0) {
        return nbCameraGetModelAllTable_Dummy;
    }
    return &nbCameraGetModelAllTable_Dummy[pWork->id];
}

f32 nbCameraGetModelRadius(nbFormation_t *pFormation)
{
    f32 radius = nbCameraGetModelAllTable(pFormation)->radius;

    if (radius == 0.0f) {
        return pFormation->r * pFormation->scl;
    }
    return radius;
}

f32 nbCameraGroupGetMaxLength(sdfVector4_t *pPos, u32 group)
{
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    f32 max = 0.0f;
    f32 length;
    s32 i;

    for (i = 0x1F; i >= 0; i--) {
        if ((pForm->stat & group) != 0 && (pForm->stat & 3) == 3) {
            effBTLModelGetSphereCenter(pForm);
            SDF_VU0_LOADVECTOR_B(pPos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(length);
            length += nbCameraGetModelRadius(pForm);
            if (length > max) {
                max = length;
            }
        }
        pForm++;
    }
    return max;
}

f32 nbCameraGroupGetSphere(nbFormation_t *group)
{
    sdfVector4_t center;

    f32 length;

    effBTLModelGroupGetCenter(group);
    SDF_VU0_STOREVECTOR_MEMOPS(center);
    length = nbCameraGroupGetMaxLength(&center, group);
    SDF_VU0_LOADVECTOR_MEMOPS(center);
    return length;
}

void nbCameraCommandSetFixPosition(void)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
}

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraShot_CommandFixPos_Set2);

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraShot_CommandFixPos_Set);

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraShot_AllParty_Set);

void nbCameraShot_FriendParty_Set(nbCamera_t *pNbCamera)
{
    u32 party;
    f32 radius;

    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllShow();
    party = effBTLSkillGetInvokeParty();
    radius = effBTLModelGroupGetSphere(party);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->target.x = 0.0f;
    if (radius < 250.0f) {
        radius = 250.0f;
    }
    pNbCamera->fovy = 0.69813163f;
    pNbCamera->distance = radius / sdfAtan(0.49567342f);
    effBTLModelGroupGetDirection(party, &pNbCamera->direction);
    SetRotateMatrixX(-0.26179934f);
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraShot_Summon_Set);

void nbCameraCRSetPlayerFormRequest(void)
{
    nbCameraCRSetPlayerFormRequestFlag = TRUE;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraCommand_GroundCollision);

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraState_CommandRoot_Init);

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraState_CommandRoot_Calc);

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraState_CommandTargetSel_Init);

void nbCameraState_CommandTargetSel_Calc(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraState_Command_KORONZCheck);
