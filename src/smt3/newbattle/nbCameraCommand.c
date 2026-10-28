#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"
#include "fpu.h"

extern u64 sdfFloatToULong(f32);
extern s32 printf(const char *, ...);
extern char D_00534BB8[]; /* Padded ground-collision diagnostic. */
EQUATE_FPTODP();

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

extern void effBTLModelGroupGetDirection(u32 group, sdfVector4_t *pDirection);

extern f32 sdfAtan(f32 aX);
extern u32 effBTLModelGetInvokeGroupNumber(void);
extern f32 nbCameraGetSummonNear(void);
extern f32 nbCameraGetSummonFar(void);

extern void SetRotateMatrixX(f32 aAngle);

extern u32 effBTLSkillGetInvokeParty(void);

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern s32 effBTLDebug_CameraAllTableCheck(void);

extern s8 nbCameraCRSetPlayerFormRequestFlag;

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void effBTLModelAllShow(void);

extern s32 effBTLModelGetGroupNumber(u32 group);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern s32 nbGetActionSeq(dds3ProcessID_t *id);

extern void dds3CrossfadeStart(s32 frames);

extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

extern void nbCameraState_InvokeTargetSelect_SearchSet(void);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern u32 effBTLSkillGetInvokeParty(void);

extern u32 effBTLSkillGetTargetParty(void);

extern u32 effBTLSkillGetTargetNumber(void);

extern void nbCameraShot_AllParty_Set(nbCamera_t *pNbCamera);

extern void nbCameraShot_Summon_Set(nbCamera_t *pNbCamera);

extern void nbCameraCommand_GroundCollision(nbCamera_t *pNbCamera);

extern void nbCameraState_Command_KORONZCheck(void);

extern s32 effBTLModelGetID(nbFormation_t *formation);
extern void effBTLModelShow(nbFormation_t *formation);
extern void effBTLModelCheckBoundsHide(nbFormation_t *formation, sdfVector4_t *target,
    sdfVector4_t *eye, sdfVector4_t *up, f32 radius, f32 height);
extern sdfVector4_t aViewDown;

extern nbCamera_t pNbCamera;

extern s32 nbCameraState_CommandRoot_OrderIndex;

void nbCameraState_CommandRoot_Init(void);

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

f32 nbCameraGroupGetSphere(u32 group)
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

void nbCameraShot_CommandFixPos_Set(nbCamera_t *pNbCamera)
{
    s32 playerGroupCount = effBTLModelGetGroupNumber(0x100);
    s32 enemyGroupCount = effBTLModelGetGroupNumber(0x200);

    /* Retail tests these counts and uses two distinct epilogues to tail-call Set2. */
    if (playerGroupCount == 1 && enemyGroupCount == playerGroupCount) {
        nbCameraShot_CommandFixPos_Set2(pNbCamera);
    } else {
        nbCameraShot_CommandFixPos_Set2(pNbCamera);
    }
}

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

void nbCameraShot_Summon_Set(nbCamera_t *pNbCamera)
{
    u32 party;
    f32 radius;
    f32 nearDistance;
    f32 offset;

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
    if (effBTLModelGetInvokeGroupNumber() < 4) {
        nearDistance = nbCameraGetSummonNear();
        offset = nearDistance + nbCameraGetSummonFar() * 0.5f;
        pNbCamera->target.x += offset;
        radius += offset;
    }
    pNbCamera->fovy = 0.69813163f;
    pNbCamera->distance = radius / sdfAtan(0.49567342f);
    effBTLModelGroupGetDirection(party, &pNbCamera->direction);
    SetRotateMatrixX(-0.26179934f);
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
}

void nbCameraCRSetPlayerFormRequest(void)
{
    nbCameraCRSetPlayerFormRequestFlag = TRUE;
}

void nbCameraCommand_GroundCollision(nbCamera_t *pNbCamera)
{
    sdfVector4_t point;
    /* Only bare-address VU0 loads observe this scratch vector. */
    volatile sdfVector4_t scale;
    f32 height;
    f32 distance;

    scale.z = -pNbCamera->distance;
    scale.x = -pNbCamera->distance;
    scale.y = -pNbCamera->distance;
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pNbCamera);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&point);
    if (point.y > -20.0f) {
        printf(D_00534BB8, point.y);
        height = point.y - -20.0f;
        distance = ffabsf(height / pNbCamera->direction.y);
        pNbCamera->distance -= fsqrtf(height * height + distance * distance);
    }
}

void nbCameraState_CommandRoot_Init(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    if (pNbCamera.state_old == 1) {
        dds3CrossfadeStart(4);
    } else if (pNbCamera.state_old == 10) {
        dds3CrossfadeStart(4);
    } else if (pNbCamera.state_old == 17) {
        dds3CrossfadeStart(8);
    }
    if (nbCameraCRSetPlayerFormRequestFlag != 0) {
        nbSetPlayerFormation(data, 0);
        nbCameraCRSetPlayerFormRequestFlag = 0;
    }
    nbCameraCommandSetFixPosition();
    nbCameraShot_CommandFixPos_Set(&pNbCamera);
    nbCameraCommand_GroundCollision(&pNbCamera);
    nbCameraState_Command_KORONZCheck();
    nbCameraState_CommandRoot_OrderIndex = data->orderindex;
}

void nbCameraState_CommandRoot_Calc(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    dds3ProcessID_t *id = data->ActionProcessID[data->NowActionProcess];

    if (id == NULL) return;
    if (nbGetActionSeq(id) != 1) return;
    if (nbCameraState_CommandRoot_OrderIndex == data->orderindex) return;
    nbCameraState_CommandRoot_Init();
    dds3CrossfadeStart(4);
}

void nbCameraState_CommandTargetSel_Init(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    u32 invokeParty;
    u32 targetParty;
    u32 targetNumber;
    u32 flags = 0;
    u32 i;

    nbCameraState_InvokeTargetSelect_SearchSet();
    invokeParty = effBTLSkillGetInvokeParty();
    targetParty = effBTLSkillGetTargetParty();
    targetNumber = effBTLSkillGetTargetNumber();
    for (i = 0; i < targetNumber; i++) {
        flags |= effBTLSkillGetTargetFormation(i)->stat & 0x300;
    }
    if (data->TarSelType == 1) {
        nbCameraShot_Summon_Set(&pNbCamera);
    } else {
        if (flags == 0x300) {
            nbCameraShot_AllParty_Set(&pNbCamera);
        } else if (invokeParty == targetParty) {
            nbCameraShot_FriendParty_Set(&pNbCamera);
        } else {
            nbCameraCommandSetFixPosition();
            nbCameraShot_CommandFixPos_Set(&pNbCamera);
            nbCameraCommand_GroundCollision(&pNbCamera);
            nbCameraState_Command_KORONZCheck();
        }
    }
    nbCameraCommand_GroundCollision(&pNbCamera);
}

void nbCameraState_CommandTargetSel_Calc(void) {
}

void nbCameraState_Command_KORONZCheck(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    nbFormation_t *active;
    nbFormation_t *formation;
    /* Qualified ee-gcc allocation traces: uid73 loads the selected index into
     * pseudo87; uid116 resets it for the scan and uid242 increments it.
     * Sharing that live index gives $17; separate indices give $2/$18.
     * Both allocation traces passed hooked/unhooked/plain output parity. */
    u32 i;
    sdfVector4_t eye;
    sdfVector4_t target;
    /* VU0 alone observes the component stores through the bare address. */
    volatile sdfVector4_t scale;

    if (data == NULL || data->orderindex == -1) {
        return;
    }
    i = data->party[data->order[data->orderindex]].formindex;
    if (i >= 16) {
        return;
    }
    active = &data->form[i];
    if (active->stat & 0x200) {
        return;
    }
    formation = data->form;
    for (i = 0; i < 32; i++, formation++) {
        if ((formation->stat & 0x103) == 0x103 && formation != active) {
            if (effBTLModelGetID(formation) == 0x82) {
                effBTLModelShow(formation);
                scale.z = -pNbCamera.distance;
                scale.x = -pNbCamera.distance;
                scale.y = -pNbCamera.distance;
                SDF_VU0_LOADVECTOR(&pNbCamera.direction);
                SDF_VU0_LOADVECTOR_B(&scale);
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(&pNbCamera);
                SDF_VU0_ADDVECTOR();
                /* The following eye store supplies the memory boundary for both outputs. */
                SDF_VU0_STOREVECTOR_B_NOMEM(&target);
                SDF_VU0_STOREVECTOR(&eye);
                effBTLModelCheckBoundsHide(formation, &target, &eye, &aViewDown,
                    115.0f, -(formation->center.y - formation->h * formation->scl * 0.5f));
            }
        }
    }
}

INCLUDE_RODATA(const s32, "newbattle/nbCameraCommand", D_00534BB8);
