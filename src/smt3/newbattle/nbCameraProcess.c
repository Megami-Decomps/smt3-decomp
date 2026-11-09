#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"
#include "basic.h"
#include "dds3obj.h"
#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

/* The tail halves its step every retail tick. Applying the decayed step
 * (1 - r) / r times as large each host tick sums to the retail total. */
#define CAMERA_STEP(aStep, aTail)                                                                 \
    ((aTail) ? (aStep) * ((1.0f - SDF_FRAME_DECAY(0.5f)) / SDF_FRAME_DECAY(0.5f))                 \
             : SDF_FRAME_STEP(aStep))

/* 16-byte rows indexed by camera state; +0xC selects the per-state reset. */
typedef struct nbCameraStateEntry_t {
    char *name;
    void (*init)(void);
    void (*calc)(void);
    s8 reset;
    u8 unk0D[3];
} nbCameraStateEntry_t;

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

extern void SetRotateMatrixY(f32 aAngle);

extern void effBTLModelAllHide(void);

extern void func_001B2158(u32 mask);

extern f32 func_001B2270(sdfVector4_t *pCenter, u32 mask);

/* 128-byte party records; this unit only reads the formation index. */


typedef struct nbCameraUnitWork_t {
    u8 unk00[0x28];
    nbFormation_t *pForm;
} nbCameraUnitWork_t;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void nbCameraInit(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);



extern s32 nbCameraProcess_nbCameraProcess(dds3ProcessID_t *id);

extern void nbCameraShutdown(dds3ProcessID_t *id);

extern s32 nbCameraDraw(dds3ProcessID_t *id);

extern void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

extern nbCamera_t pNbCamera;

extern nbCameraStateEntry_t nbCameraStateTable[];

extern void effBTLSkillSetFormationInfo(s32 InvokeID, u32 TargetBit);

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void effBTLModelTargetGroupHide(void);

extern void nbCameraShot_All_Set(nbCamera_t *pNbCamera);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

void nbCameraSetState(u16 state);

void nbCameraClearState(nbCamera_t *pCamera)
{
    pCamera->anim.rotate_f = 0;
    pCamera->anim.rotate_s = 0;
    pCamera->anim.distance_f = 0;
    pCamera->anim.distance_s = 0.0f;
}

void nbCameraAnim_RotateSet(nbCamera_t *pNbCamera, s32 frame, s32 rotate, f32 start_f)
{
    pNbCamera->anim.rotate_f = SDF_FRAMES(frame);
    pNbCamera->anim.rotate_s = start_f / frame;
    SetRotateMatrixY(pNbCamera->anim.rotate_s * rotate);
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
}

void func_001B1518(nbCamera_t *pCamera, s32 frames, s32 count, f32 angle)
{
    f32 step = -angle / (f32)frames;

    pCamera->anim.distance_f = SDF_FRAMES(frames);
    pCamera->anim.distance_s = step;
    pCamera->distance += step * (f32)count;
}

void nbCameraState_InvokeTargetSelect_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->ActionProcessID[pData->NowActionProcess];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 4) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_InvokeTarget_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->ActionProcessID[pData->NowActionProcess];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 8) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_EnemyParty_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->ActionProcessID[pData->NowActionProcess];
    nbFormation_t *pForm = pData->form;
    nbCameraUnitWork_t *pWork;
    u32 bits;
    u32 i;

    if (pProcess == NULL) {
        return;
    }
    pWork = dds3GetProcessData(pProcess);
    bits = 0;
    for (i = 0; i < 32; i++) {
        if ((pForm->stat & 0x200) != 0) {
            bits |= 1 << i;
        }
        pForm++;
    }
    effBTLSkillSetFormationInfo(pWork->pForm->formindex, bits);
}

void nbCameraState_DebugShotAll_Init(void)
{
    nbCameraShot_All_Set(&pNbCamera);
}

void nbCameraSetState(u16 state)
{
    pNbCamera.state_change = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.state = 0;
    }
}

void func_001B1770(u16 state)
{
    pNbCamera.state_change = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.state = 0;
    }
}

void nbCameraSetBougai(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(13);
}

void nbCameraSetTaizi(nbFormation_t *pInvokeForm, nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(pInvokeForm->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(14);
}

void nbCameraSetTarget(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(15);
}

void nbCameraSetHojo(nbFormation_t *pHojo, nbFormation_t *pTarget1, nbFormation_t *pTarget2)
{
    effBTLSkillSetFormationInfo(pHojo->formindex, (1 << pTarget1->formindex) | (1 << pTarget2->formindex));
    nbCameraSetState(16);
}

void nbCameraEventSetTarget(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex, 1 << pTargetForm->formindex);
    nbCameraSetState(24);
}

void nbCameraScriptSet1Shot(nbFormation_t *pTargetForm)
{
    effBTLSkillSetFormationInfo(effBTLSkillGetInvokeFormation()->formindex,
                                1 << pTargetForm->formindex);
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllHide();
    effBTLModelTargetGroupHide();
    nbCameraSetState(0x1A);
}

extern f32 nbCameraGetBossDistance(void);
extern f32 effBTLModelGetHeight(nbFormation_t *pFormation);
extern void effBTLModelTargetPartyShow(void);
extern f32 sdfAtan(f32 value);

void nbCameraScriptSetBougai(nbFormation_t *pTargetForm)
{
    nbMainProcessData_t *pData;
    nbFormation_t *pForm;
    nbFormation_t *pInvokeForm;
    sdfVector4_t position;
    sdfVector4_t target;
    nbCamera_t *pCamera;
    u32 targetParty;
    u32 partyMask = 0x200;
    u32 partyCount = 0;
    u32 i = 0;
    f32 radiusSum;
    f32 maxHeight;
    f32 height;
    f32 radius;
    f32 angle;
    f32 fovy;
    f32 cameraDistance;

    pInvokeForm = effBTLSkillGetInvokeFormation();
    targetParty = 1 << pTargetForm->formindex;
    maxHeight = 0.0f;
    effBTLSkillSetFormationInfo(pInvokeForm->formindex, targetParty);
    radiusSum = 0.0f;
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    effBTLModelAllHide();
    effBTLModelTargetPartyShow();

    pCamera = &pNbCamera;
    pData = nbGetMainProcessData();
    pForm = pData->form;
    if ((pTargetForm->stat & 0x200) == 0) {
        partyMask = 0x100;
    }
    do {
        if ((pForm->stat & partyMask) != 0 && (pForm->stat & 1) != 0) {
            radius = pForm->r * pForm->scl;
            radius += radius;
            radiusSum += radius;
            height = effBTLModelGetHeight(pForm);
            if (maxHeight < height) {
                maxHeight = height;
            }
            partyCount++;
        }
        i++;
        pForm++;
    } while (i < 32);
    if (partyCount == 0) {
        return;
    }

    fovy = 0.69813163f;
    if (partyCount >= 2) {
        fovy = 0.9308421016f;
    }
    angle = sdfAtan(fovy * 0.5f);
    if (radiusSum < maxHeight) {
        radiusSum = maxHeight * 0.5f;
    } else {
        radiusSum *= 0.5f;
    }
    if (radiusSum < 400.0f) {
        radiusSum = 400.0f;
    }
    radiusSum /= angle;

    target.x = 0.0f;
    target.y = -(maxHeight * 0.5f);
    cameraDistance = nbCameraGetBossDistance() * 0.5f;
    position.x = target.x;
    position.y = -160.0f;
    target.z = cameraDistance;
    position.z = cameraDistance - radiusSum;

    SDF_VU0_LOADVECTOR(&target);
    SDF_VU0_STOREVECTOR(pCamera);
    SDF_VU0_LOADVECTOR_B(&position);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(pCamera->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    nbCameraSetState(0x1B);
}

extern sdfVector4_t nbCameraForward;
extern void ToMatrix(void);

void nbCameraScriptSet(sdfVector4_t *position, sdfVector4_t *rotation)
{
    effBTLModelAllPopPosition();
    effBTLModelAllPopRotate();
    SDF_VU0_LOADVECTOR(rotation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&nbCameraForward);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera.direction);
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR();
    /* No C access observes the target vector here: the following scalar
     * stores and state setter touch disjoint fields. Keep the SDK address-only
     * store; invalidating unrelated memory changes this unit's code generation. */
    SDF_VU0_STOREVECTOR_NOMEM(&pNbCamera.target);
    pNbCamera.distance = 1.0f;
    pNbCamera.fovy = 0.6981316209f;
    nbCameraSetState(0x1C);
}

void nbCameraInit(void)
{
    nbCameraShot_All_Set(&pNbCamera);
    pNbCamera.state_change = 0;
    pNbCamera.state_old2 = 0;
    pNbCamera.state_old = 0;
    pNbCamera.state = 0;
}

s32 nbCameraCheckNiidaCamera(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();

    return dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) == pData->niidacamera;
}

#ifdef NON_MATCHING
extern s32 nbGetActionSeq(dds3ProcessID_t *id);
extern void effBTLModelAllShow(void);
#endif

void nbCameraCheckStateChange(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    dds3ProcessID_t *process = data->ActionProcessID[data->NowActionProcess];
    s32 actionSeq;

    if (process == NULL) {
        return;
    }
    actionSeq = nbGetActionSeq(process);
    if (data->orderindex != -1) {
        nbFormation_t *form = &data->form[data->party[data->order[data->orderindex]].formindex];

        if ((form->stat & 0x100) != 0) {
            if (actionSeq == 1) {
                nbCameraSetState(2);
            } else if (actionSeq == 4) {
                pNbCamera.state = 2;
                nbCameraSetState(3);
            } else if (actionSeq == 5) {
                nbCameraSetState(4);
            }
        } else if (actionSeq == 1) {
            effBTLModelAllShow();
        }
    }
    if (actionSeq == 10) {
        nbCameraSetState(17);
        return;
    }
}

#include "sdk_libc.h"

extern void nbRestartMotion(void);
extern void nbSetEnemyFormation(nbMainProcessData_t *data);
extern u32 effBTLDebug_CameraAllEditCheck(void);
extern void nbCameraCommandSetFixPosition(void);
extern void nbCameraShot_CommandFixPos_Set(nbCamera_t *camera);
extern void effBTLModelShowRun(void);
extern void nbCameraPressProcessRun(void);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *basic, f32 fovy);

s32 nbCameraProcess_nbCameraProcess(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    dds3Basic_t *camera;
    dds3CameraObjectBasicWorkData_t *work;
    sdfVector4_t eye;
    volatile sdfVector4_t scale;
    f32 distance;
    s32 rotateTail;
    s32 distanceTail;

    nbCameraCheckStateChange();
    if (pNbCamera.state != pNbCamera.state_change) {
        if ((pNbCamera.state_change == 2 || pNbCamera.state_change == 4) && (data->stat & 0x40)) {
            pNbCamera.state_change = 0;
        }
        if (pNbCamera.state_change != 6 && pNbCamera.state_change != 8 &&
            pNbCamera.state_change != 9 && pNbCamera.state_change != 0 &&
            pNbCamera.state_change != 10) {
            printf("nbCamera:nbRestartMotion\n");
            nbRestartMotion();
        }
        pNbCamera.state_old2 = pNbCamera.state_old;
        pNbCamera.state_old = pNbCamera.state;
        pNbCamera.state = pNbCamera.state_change;
        pNbCamera.phase = 0;
        pNbCamera.frame = 0;
        pNbCamera.type = 0;
        pNbCamera.angle = 0.0f;
        if ((data->stat & 0x2000) && pNbCamera.state != 10 && pNbCamera.state != 0 &&
            pNbCamera.state != 17) {
            nbSetEnemyFormation(data);
            data->stat &= ~0x2000;
        }
        nbCameraClearState(&pNbCamera);
        if (nbCameraStateTable[pNbCamera.state].init != NULL) {
            nbCameraStateTable[pNbCamera.state].init();
        }
    }
    if (nbCameraStateTable[pNbCamera.state].calc != NULL && SDF_FRAME_DUE(dds3GetKernelCount())) {
        nbCameraStateTable[pNbCamera.state].calc();
    }
    if (effBTLDebug_CameraAllEditCheck() != 0) {
        nbCameraCommandSetFixPosition();
        nbCameraShot_CommandFixPos_Set(&pNbCamera);
    }
    effBTLModelShowRun();
    nbCameraPressProcessRun();
    if (nbCameraCheckNiidaCamera() == 0) {
        return 0;
    }
    camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    work = camera->workdata;
    rotateTail = pNbCamera.anim.rotate_f <= 0;
    if (!rotateTail) {
        pNbCamera.anim.rotate_f--;
    } else {
        pNbCamera.anim.rotate_s *= SDF_FRAME_DECAY(0.5f);
    }
    if (pNbCamera.anim.rotate_s != 0.0f) {
        SetRotateMatrixY(CAMERA_STEP(pNbCamera.anim.rotate_s, rotateTail));
        SDF_VU0_LOADVECTOR(&pNbCamera.direction);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&pNbCamera.direction);
    }
    distanceTail = pNbCamera.anim.distance_f <= 0;
    if (!distanceTail) {
        pNbCamera.anim.distance_f--;
    } else {
        pNbCamera.anim.distance_s *= SDF_FRAME_DECAY(0.5f);
    }
    if (pNbCamera.anim.distance_s != 0.0f) {
        pNbCamera.distance += CAMERA_STEP(pNbCamera.anim.distance_s, distanceTail);
    }
    distance = -pNbCamera.distance;
    scale.z = distance;
    scale.x = distance;
    scale.y = distance;
    SDF_VU0_LOADVECTOR(&pNbCamera.direction);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_STOREVECTOR(&work->eyeVector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&pNbCamera.target);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_B(&work->targetPos);
    SDF_VU0_STOREVECTOR(&eye);
    SDF_FRAME_INTERPOLATE_VEC(dds3GetKernelCount(), &work->eyeVector.x, &eye.x, &eye.x, 3);
    SDF_FRAME_INTERPOLATE_VEC(dds3GetKernelCount(), &work->targetPos.x, &work->targetPos.x, &work->targetPos.x, 3);
    dds3SetPosPoseSet(camera, &eye);
    dds3CameraObjectBasicSetFovy(camera, SDF_FRAME_INTERPOLATE(dds3GetKernelCount(), &pNbCamera.fovy, pNbCamera.fovy));
    work->flag |= 1;
    pNbCamera.frame += SDF_FRAME_DUE(dds3GetKernelCount());
    return 0;
}

s32 nbCameraDraw(dds3ProcessID_t *id)
{
    return 0;
}

void nbCameraShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->camera = NULL;
}

void nbInitCameraProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *process = dds3StartProcess("nb_camera", 0x3FB, 2, 1,
                                                (dds3ProcessFunc_t)nbCameraProcess_nbCameraProcess,
                                                (dds3ProcessFunc_t)nbCameraShutdown, NULL);

    dds3LinkProcess(data->MainProcessID, process);
    data->camera = process;
    dds3LinkProcess(process, dds3StartProcess("nb_camera_draw", 0x2B0E, 1, 1,
                                              (dds3ProcessFunc_t)nbCameraDraw, NULL, NULL));
    nbCameraInit();
}

void func_001B2158(u32 mask)
{
    sdfVector4_t sum;
    sdfVector4_t scale;
    nbFormation_t *pForm;
    u32 count;
    u32 stat;
    s32 i;
    f32 zero;

    count = 0;
    pForm = nbGetMainProcessData()->form;
    zero = 0.0f;
    sum.z = zero;
    sum.y = zero;
    sum.x = zero;
    for (i = 0; i < 32; i++) {
        stat = pForm->stat;
        if ((stat & mask) != 0 && (stat & 1) != 0) {
            effBTLModelGetSphereCenter(pForm);
            SDF_VU0_LOADVECTOR_B_MEMOPS(sum);
            SDF_VU0_ADDVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_MEMOPS(sum);
            count++;
        }
        pForm++;
    }
    if (count != 0) {
        scale.x = 1.0f / count;
    } else {
        scale.x = 1.0f;
    }
    scale.y = scale.x;
    scale.z = scale.x;
    SDF_VU0_LOADVECTOR_MEMOPS(sum);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
}

f32 func_001B2270(sdfVector4_t *pCenter, u32 mask)
{
    nbFormation_t *pForm = nbGetMainProcessData()->form;
    f32 length;
    f32 max = 0.0f;
    s32 i;

    for (i = 0x1F; i >= 0; i--) {
        if ((pForm->stat & mask) != 0 && (pForm->stat & 1) != 0) {
            effBTLModelGetSphereCenter(pForm);
            SDF_VU0_LOADVECTOR_B(pCenter);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(length);
            if (length + pForm->r * pForm->scl > max) {
                max = length + pForm->r * pForm->scl;
            }
        }
        pForm++;
    }
    return max;
}

void nb_1B2340(u32 mask)
{
    sdfVector4_t center;

    func_001B2158(mask);
    SDF_VU0_STOREVECTOR_MEMOPS(center);
    func_001B2270(&center, mask);
    SDF_VU0_LOADVECTOR_MEMOPS(center);
}


void nbCameraState_Start_Init(void);
void nbCameraState_Start_Calc(void);
void nbCameraState_CommandRoot_Init(void);
void nbCameraState_CommandRoot_Calc(void);
void nbCameraState_CommandTargetSel_Init(void);
void nbCameraState_CommandTargetSel_Calc(void);
void nbCameraState_SkillAttack_Init(void);
void nbCameraState_SkillAttack_Calc(void);
void nbCameraState_SkillStart_Init(void);
void nbCameraState_SkillCut1_Init(void);
void nbCameraState_SkillCut1_Calc(void);
void nbCameraState_SkillCut2_Init(void);
void nbCameraState_SkillCut2_Calc(void);
void func_001B8018(void);
void func_001B8020(void);
void nbCameraState_NegoBougai_Init(void);
void nbCameraState_NegoBougai_Calc(void);
void nbCameraState_NegoTaizi_Init(void);
void nbCameraState_NegoTaizi_Calc(void);
void nbCameraState_NegoTarget_Init(void);
void nbCameraState_NegoTarget_Calc(void);
void nbCameraState_NegoHojo_Init(void);
void nbCameraState_NegoHojo_Calc(void);
void nbCameraState_PlayerSide_Init(void);
void nbCameraState_EnemySide_Init(void);
void nb_1B3470(void);
void nb_1B3490(void);
void nbCameraState_NegoTarget_Calc2(void);
void nbCameraState_EnemyBad_Init(void);
void nbCameraState_EnemyBad_Calc(void);
void nbCameraState_NegoBougai_Init2(void);
extern char nbCameraStateNameWait[];
extern char nbCameraStateNameStart[];
extern char nbCameraStateNameCommandRoot[];
extern char nbCameraStateNameCommandBad[];
extern char nbCameraStateNameCommandTargetSelect[];
extern char nbCameraStateNameSkillAttack[];
extern char nbCameraStateNameSkillStart[];
extern char nbCameraStateNameSkillCut1[];
extern char nbCameraStateNameSkillCut2[];
extern char nbCameraStateNameSkillCut3[];
extern char nbCameraStateNameSkillEnd[];
extern char nbCameraStateNamePressTurn[];
extern char nbCameraStateNameSummon[];
extern char nbCameraStateNameNegoBougai[];
extern char nbCameraStateNameNegoTaizi[];
extern char nbCameraStateNameNegoTarget[];
extern char nbCameraStateNameNegoHojo[];
extern char nbCameraStateNameNegoEnd[];
extern char D_005344A0[];
extern char nbCameraStateNameResult[];
extern char nbCameraStateNamePlayerSide[];
extern char nbCameraStateNameEnemySide[];
extern char nbCameraStateNameEnd[];
extern char nbCameraStateNameEvent[];
extern char D_00534470[];
extern char nbCameraStateNameEnemyBad[];
extern char nbCameraStateNameScript1Shot[];
extern char nbCameraStateNameScriptBougai[];
extern char nbCameraStateNameScriptFix[];
extern char D_00534420[];

nbCameraStateEntry_t nbCameraStateTable[30] = {
    { nbCameraStateNameWait, NULL, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameStart, nbCameraState_Start_Init, nbCameraState_Start_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameCommandRoot, nbCameraState_CommandRoot_Init, nbCameraState_CommandRoot_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameCommandBad, nbCameraState_CommandRoot_Init, nbCameraState_CommandRoot_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameCommandTargetSelect, nbCameraState_CommandTargetSel_Init, nbCameraState_CommandTargetSel_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillAttack, nbCameraState_SkillAttack_Init, nbCameraState_SkillAttack_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillStart, nbCameraState_SkillStart_Init, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillCut1, nbCameraState_SkillCut1_Init, nbCameraState_SkillCut1_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillCut2, nbCameraState_SkillCut2_Init, nbCameraState_SkillCut2_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillCut3, NULL, nbCameraState_SkillCut2_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameSkillEnd, func_001B8018, func_001B8020, 0, { 0, 0, 0 } },
    { nbCameraStateNamePressTurn, NULL, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameSummon, NULL, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameNegoBougai, nbCameraState_NegoBougai_Init, nbCameraState_NegoBougai_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameNegoTaizi, nbCameraState_NegoTaizi_Init, nbCameraState_NegoTaizi_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameNegoTarget, nbCameraState_NegoTarget_Init, nbCameraState_NegoTarget_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameNegoHojo, nbCameraState_NegoHojo_Init, nbCameraState_NegoHojo_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameNegoEnd, NULL, NULL, 0, { 0, 0, 0 } },
    { D_005344A0, NULL, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameResult, NULL, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNamePlayerSide, nbCameraState_PlayerSide_Init, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameEnemySide, nbCameraState_EnemySide_Init, NULL, 0, { 0, 0, 0 } },
    { nbCameraStateNameEnd, nb_1B3470, nb_1B3490, 0, { 0, 0, 0 } },
    { nbCameraStateNameEvent, NULL, NULL, 0, { 0, 0, 0 } },
    { D_00534470, nbCameraState_NegoTarget_Init, nbCameraState_NegoTarget_Calc2, 0, { 0, 0, 0 } },
    { nbCameraStateNameEnemyBad, nbCameraState_EnemyBad_Init, nbCameraState_EnemyBad_Calc, 1, { 0, 0, 0 } },
    { nbCameraStateNameScript1Shot, nbCameraState_NegoTarget_Init, nbCameraState_NegoTarget_Calc2, 0, { 0, 0, 0 } },
    { nbCameraStateNameScriptBougai, nbCameraState_NegoBougai_Init2, nbCameraState_NegoBougai_Calc, 0, { 0, 0, 0 } },
    { nbCameraStateNameScriptFix, NULL, NULL, 0, { 0, 0, 0 } },
    { D_00534420, nbCameraState_DebugShotAll_Init, NULL, 0, { 0, 0, 0 } },
};
