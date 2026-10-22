#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

/* 16-byte rows indexed by camera state; +0xC selects the per-state reset. */
typedef struct nbCameraStateEntry_t {
    u8 unk00[0xC];
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

extern void SetRotateMatrixY(f32 aAngle);

extern void effBTLModelAllHide(void);

extern void func_001B2158(u32 mask);

extern f32 func_001B2270(sdfVector4_t *pCenter, u32 mask);

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

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

extern void effBTLSkillSetFormationInfo(u32 InvokeID, u32 TargetBit);

extern void effBTLModelAllPopPosition(void);

extern void effBTLModelAllPopRotate(void);

extern void func_0016E1F8(void);

extern void nbCameraShot_All_Set(nbCamera_t *pNbCamera);

typedef struct dds3Scene_t dds3Scene_t;

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

extern dds3Scene_t *dds3GetActiveScene(void);

extern s32 dds3GetActiveCameraSceneBasic(dds3Scene_t *pBasic);

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
    pNbCamera->anim.rotate_f = frame;
    pNbCamera->anim.rotate_s = start_f / frame;
    SetRotateMatrixY(pNbCamera->anim.rotate_s * rotate);
    SDF_VU0_LOADVECTOR(&pNbCamera->direction);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&pNbCamera->direction);
}

void func_001B1518(nbCamera_t *pCamera, s32 frames, s32 count, f32 angle)
{
    f32 step = -angle / (f32)frames;

    pCamera->anim.distance_f = frames;
    pCamera->anim.distance_s = step;
    pCamera->distance += step * (f32)count;
}

void nbCameraState_InvokeTargetSelect_SearchSet(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
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
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
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
    dds3ProcessID_t *pProcess = pData->unkD4[pData->unkE4];
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
    pNbCamera.stateChange = state;
    if (nbCameraStateTable[state].reset != 0) {
        pNbCamera.state = 0;
    }
}

void func_001B1770(u16 state)
{
    pNbCamera.stateChange = state;
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
    func_0016E1F8();
    nbCameraSetState(0x1A);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraProcess", nbCameraScriptSetBougai);

INCLUDE_ASM(const s32, "newbattle/nbCameraProcess", nbCameraScriptSet);

void nbCameraInit(void)
{
    nbCameraShot_All_Set(&pNbCamera);
    pNbCamera.stateChange = 0;
    pNbCamera.stateOld2 = 0;
    pNbCamera.stateOld = 0;
    pNbCamera.state = 0;
}

s32 nbCameraCheckNiidaCamera(void)
{
    nbMainProcessData_t *pData = nbGetMainProcessData();

    return dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) == pData->unkB4;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraProcess", nbCameraCheckStateChange);

INCLUDE_ASM(const s32, "newbattle/nbCameraProcess", nbCameraProcess_nbCameraProcess);

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

    dds3LinkProcess(data->unkBC, process);
    data->camera = process;
    dds3LinkProcess(process, dds3StartProcess("nb_camera_draw", 0x2B0E, 1, 1,
                                              (dds3ProcessFunc_t)nbCameraDraw, NULL, NULL));
    nbCameraInit();
}

INCLUDE_ASM(const s32, "newbattle/nbCameraProcess", func_001B2158);

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

