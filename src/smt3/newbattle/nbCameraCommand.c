#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"
#include "fpu.h"

extern u64 fptodp(f32);
extern s32 printf(const char *, ...);
extern char D_00534BB8[]; /* Padded ground-collision diagnostic. */

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

extern f32 effBTLModelGetHeight(nbFormation_t *formation);
extern void effBTLGetGroupPos(nbFormation_t *formation, sdfVector4_t *position);
extern void effBTLModelSetPosition(nbFormation_t *formation, sdfVector4_t *position);
extern void nbCameraShot_GroundCollision(nbCamera_t *camera);
extern sdfVector4_t D_00325960;

extern s32 nbCameraBoss_Command_Init(nbCamera_t *camera);
extern void nbCameraShot_CommandFixPos_Calc2(nbCamera_t *camera, nbFormation_t *formation);

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

void nbCameraShot_CommandFixPos_Set2(nbCamera_t *pNbCamera)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    nbFormation_t *formation;

    if (data != NULL && data->orderindex != -1) {
        formation = &data->form[data->party[data->order[data->orderindex]].formindex];
        if ((formation->stat & 0x200) == 0) {
            if (nbCameraBoss_Command_Init(pNbCamera) == 0) {
                nbCameraShot_CommandFixPos_Calc2(pNbCamera, formation);
            }
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCameraCommand", nbCameraShot_CommandFixPos_Calc2);

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

void nbCameraShot_AllParty_Set(nbCamera_t *pNbCamera)
{
    sdfVector4_t enemy;
    sdfVector4_t player;
    sdfVector4_t position;
    nbMainProcessData_t *data;
    nbFormation_t *formation;
    u32 i = 0;
    s32 first = 1;
    f32 enemyMax, enemyMin;
    f32 playerMax, playerMin, playerRadius;
    f32 height, radius, length, blend, projection;

    effBTLModelAllShow();
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    nbCameraState_InvokeTargetSelect_SearchSet();
    data = nbGetMainProcessData();
    enemyMax = enemyMin = 0.0f;
    formation = &data->form[4];
    for (; i < 10; i++, formation++) {
        if ((formation->stat & 3) == 3) {
            height = effBTLModelGetHeight(formation);
            if (first) {
                enemyMax = height;
                first = 0;
                enemyMin = enemyMax;
            } else {
                if (height > enemyMax) enemyMax = height;
                if (height < enemyMin) enemyMin = height;
            }
        }
    }
    /* This repeats the enemy extrema scan. Retail retains the minimum-height
     * comparison/update too, although only maximum height and radius are used. */
    playerRadius = 0.0f;
    playerMax = 0.0f;
    playerMin = 0.0f;
    first = 1;
    formation = data->form;
    for (i = 0; i < 4; i++, formation++) {
        if ((formation->stat & 3) == 3) {
            radius = formation->r * formation->scl;
            height = effBTLModelGetHeight(formation);
            if (first) {
                playerMax = height;
                playerRadius = radius;
                playerMin = playerMax;
                first = 0;
            } else {
                if (radius > playerRadius) playerRadius = radius;
                if (height > playerMax) playerMax = height;
                if (height < playerMin) playerMin = height;
            }
        }
    }
    if (playerMax >= 300.0f && enemyMax < 450.0f) {
        formation = data->form;
        for (i = 0; i < 4; i++, formation++) {
            if ((formation->stat & 3) == 3) {
                effBTLGetGroupPos(formation, &position);
                position.z -= 250.0f;
                effBTLModelSetPosition(formation, &position);
            }
        }
    }
    effBTLModelGroupGetSphere(0x100);
    SDF_VU0_STOREVECTOR(&player);
    player.y = 0.0f;
    player.z -= playerRadius;
    effBTLModelGroupGetSphere(0x200);
    SDF_VU0_STOREVECTOR(&enemy);
    enemy.x = player.x;
    enemy.y = -enemyMin * 0.5f;
    if (enemyMax < 450.0f) {
        blend = 0x1.53b644p-1f;
        if (enemyMin > 150.0f) blend = 0x1.4cccccp-1f;
    } else {
        blend = 0x1.6e147ap-1f;
        if (!(enemyMax > 800.0f)) blend = 0x1.4cccccp-1f;
    }
    SDF_VU0_LOADVECTOR(&enemy);
    SDF_VU0_LOADVECTOR_B(&player);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    length *= 0.5f;
    length -= playerRadius * 0.5f;
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&D_00325960);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_COPYVECTOR_C_TO_B();
    VU0_LERP_VF10_VF11(blend);
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
    SDF_VU0_LOADVECTOR(&enemy);
    SDF_VU0_LOADVECTOR_B(&player);
    VU0_LERP_VF10_VF11(0.5f);
    SDF_VU0_STOREVECTOR(pNbCamera);
    pNbCamera->fovy = 0x1.657182p-1f;
    projection = sdfAtan(0x1.1504c4p-1f);
    pNbCamera->distance = length / projection;
    /* Qualified allocation trace: the two length phases share pseudo96 ($f20).
     * Its division result is separate (uid522); separate length/radius locals
     * instead coalesce that result with the first length, changing two words. */
    length = effBTLModelGroupGetSphere(0x100);
    if (length < 480.0f) length = 480.0f;
    pNbCamera->distance += length / projection;
    nbCameraShot_GroundCollision(pNbCamera);
}

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

