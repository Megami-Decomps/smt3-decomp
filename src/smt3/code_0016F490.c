#include "common.h"
#include "motion.h"

/* lqc2 into vf10, which the mdlSet* entry points read as their argument. */
#define VU0_LOAD_VF10_PTR(src) __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" ::"r"(src))
#define VU0_LOAD_VF10_MEM(src) __asm__ volatile(".set noreorder\n\tlqc2 $vf10, %0\n\t.set reorder" ::"m"(*(src)))

extern u128 D_0031DF60;
extern u128 D_0031DF70;
extern u128 D_0031DF80;

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

typedef struct
{
    u32 kateg;
    u32 param;
    void *pHandle;
} dds3EffectPCP_t;

typedef struct
{
    u32 size;
    u32 offset;
    u32 type;
    u32 reserve;
} dds3EffectPCPData_Part_t;

typedef struct
{
    u32 kateg;
    u32 param;
    u32 part;
    u32 reserve;
    dds3EffectPCPData_Part_t pcpData[1];
} dds3EffectPCPData_t;

typedef struct
{
    void *(*create)(void *);
    void (*draw)(void *);
    void (*destroy)(void *);
    void *(*copy)(void *);
    void (*setPosition)(void *, void *);
    void (*setScale)(void *, f32);
    void (*setRotateMatrix)(void *, void *);
    void (*setColor)(void *, u32);
    void (*setDirection)(void *, void *);
    void (*setArea)(void *, f32);
} dds3EffectFunc_t;

typedef struct dds3ModelHandle_t
{
    u32 stat;
    struct dds3ModelHandle_t *next;
    struct dds3ModelHandle_t *prev;
    void *resrc;
    s16 motionMajorNum;
    s16 motionMinorNum;
    void *effects;
    sdfModelRootNode_t *modelRoot;
    sdfMotionCtrlr_t *motionCtrlr;
} dds3ModelHandle_t;

void mdlSetPosition(dds3ModelHandle_t *aHandle);
void mdlSetRotate(dds3ModelHandle_t *aHandle);
void mdlSetScale(dds3ModelHandle_t *aHandle);
void mdlSetColor(dds3ModelHandle_t *aHandle, u32 aColor);
void mdlLoopMotion(dds3ModelHandle_t *aHandle, s32 aGroup, s32 aNumber);

extern dds3EffectFunc_t dds3EffectFuncTable[7];
extern dds3EffectFunc_t EffectPCPFuncTable[];
extern void **D_0031DDD0[];

void *sdfChipAllocDirect(s32 aSize);
void sdfChipFreeDirect(void *aPtr);
void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);
void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);
s32 mdlGetMajorNum(dds3ModelHandle_t *aHandle);
s32 mdlGetMinorNum(dds3ModelHandle_t *aHandle);
dds3ModelHandle_t *mdlLoad(u32 aMajor, u32 aMinor);
void dds3EffectPartModelInit(dds3ModelHandle_t *hModel);
effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);
u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *hPCPData);
u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *hPCPData);
u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *hPCPData, u32 number);
void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightInit);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightON);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightOFF);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightCompulsionOFF);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightModelSet);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightGroupSet);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightSet);

INCLUDE_ASM(const s32, "code_0016F490", effBTLModelColorSet);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightStart);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightEnd);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightMain);

INCLUDE_ASM(const s32, "code_0016F490", effBTLModelColorMain);

INCLUDE_ASM(const s32, "code_0016F490", effBTLPointLightRun);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_SetUnitColorRGB);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_GetScalePtr);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_Part_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_Part_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_Part_HandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystem_Part_HandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Shock_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Freeze_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Sleep_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Panic_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Bind_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Close_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Poison_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Charm_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Paralyze_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_Delete2);

void effBTLSys_Fly_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Fly_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Delete2);

void effBTLSys_Stone_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Curse_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_SetColor);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_CurseCAMP_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_Delete2);

void effBTLSys_Stone_Dead_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Stone_Dead_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_Delete2);

void effBTLSys_Hansya_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Hansya_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_Delete2);

void effBTLSys_Kyusyuu_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Kyusyuu_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_Delete2);

void effBTLSys_Return_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Return_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_Delete2);

void effBTLSys_Attack_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Attack_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_Delete2);

void effBTLSys_AttackC_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_AttackC_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_Draw);

void effBTLSys_Encount_SetColor(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Encount_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kaihuku_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Kyusyu_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_Delete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_Delete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_GetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_DataLoad);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_DataFree);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_PartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSys_Nego_Stone_PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemDelete2);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemDraw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemGetLength);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemGetID);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemGetRGBA);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCut1DataFree);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533318);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533338);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533350);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533368);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533380);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533398);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333B0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333D0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005333E8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533400);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533418);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533438);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533450);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533468);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533488);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334D0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005334F0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533510);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533520);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533538);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533558);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533578);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533590);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335E0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005335F0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533608);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533620);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533638);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533650);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533668);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533680);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533690);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336C8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005336E8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533708);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533720);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533738);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533750);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533768);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533780);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533798);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337B0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337C8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337E0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005337F8);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533810);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533828);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533840);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533858);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533878);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533890);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338A8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338C0);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338D8);

INCLUDE_RODATA(const s32, "code_0016F490", D_005338F8);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCut1Create);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCut1Draw);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCut1Delete);

void effBTLSystemCut1PartHandleCreate(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemCut1PartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effBTLEncountStart);

INCLUDE_ASM(const s32, "code_0016F490", effBTLEncountEnd);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemInit);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemEnd);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemPartHandleCreate);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSystemPartHandleDelete);

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadCreate);

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadDraw);

INCLUDE_ASM(const s32, "code_0016F490", effCAMPBadDelete);

void func_00174858(void) {
}

void func_00174860(void) {
}

void func_00174868(void) {
}

void func_00174870(void) {
}

void func_00174878(void) {
}

INCLUDE_ASM(const s32, "code_0016F490", effBTLSetFieldColor);

INCLUDE_ASM(const s32, "code_0016F490", eff_174888);

INCLUDE_ASM(const s32, "code_0016F490", effBTLFieldColorIsOn);

INCLUDE_ASM(const s32, "code_0016F490", eff_174898);

INCLUDE_ASM(const s32, "code_0016F490", effMakeBedFileName);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSkillSetActive);

INCLUDE_ASM(const s32, "code_0016F490", effBTLSkillClearActive);

INCLUDE_ASM(const s32, "code_0016F490", effBTLCutReset);

INCLUDE_ASM(const s32, "code_0016F490", effBTLCutSetMax);

INCLUDE_ASM(const s32, "code_0016F490", effBTLCutDebugPrint);

INCLUDE_ASM(const s32, "code_0016F490", func_00174AB8);

INCLUDE_ASM(const s32, "code_0016F490", func_00174BB0);

INCLUDE_ASM(const s32, "code_0016F490", func_00174C50);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A40);

INCLUDE_ASM(const s32, "code_0016F490", effMakeBedPath);

INCLUDE_ASM(const s32, "code_0016F490", eff_174D38);

INCLUDE_ASM(const s32, "code_0016F490", eff_174D98);

INCLUDE_ASM(const s32, "code_0016F490", func_00174DC0);

INCLUDE_ASM(const s32, "code_0016F490", eff_174E88);

INCLUDE_ASM(const s32, "code_0016F490", func_00175030);

INCLUDE_ASM(const s32, "code_0016F490", eff_175070);

INCLUDE_ASM(const s32, "code_0016F490", func_001750D0);

INCLUDE_ASM(const s32, "code_0016F490", effBTLDebug_CameraAllEditCheck);

INCLUDE_ASM(const s32, "code_0016F490", effBTLDebug_CameraAllTableCheck);

INCLUDE_ASM(const s32, "code_0016F490", eff_175328);

INCLUDE_ASM(const s32, "code_0016F490", nbSetHelperViewer);

INCLUDE_ASM(const s32, "code_0016F490", nb_175338);

INCLUDE_ASM(const s32, "code_0016F490", nb_175440);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A60);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A70);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A80);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533A90);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AA0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AB0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AC0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AD0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AE0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533AF0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B00);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B10);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B20);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B30);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B40);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B50);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B60);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B70);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B80);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533B90);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BA0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BB0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BC0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BD0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BE0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533BF0);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C00);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C10);

INCLUDE_ASM(const s32, "code_0016F490", effBTLDebugPrintDistance);

INCLUDE_ASM(const s32, "code_0016F490", eff_175768);

INCLUDE_ASM(const s32, "code_0016F490", eff_1757A8);

INCLUDE_ASM(const s32, "code_0016F490", eff_1757D8);

INCLUDE_ASM(const s32, "code_0016F490", dds3_175878);

void InitModelCollisionEditorProcess(void)
{
    dds3AdminiCallSequence(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartGetHandle);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartGetType);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = type;
    hEffPart->pHandle = dds3EffectFuncTable[type].create(pData);
    return hEffPart;
}

void dds3EffectPartDelete(effPartHandle_t *hEffPart)
{
    dds3EffectFuncTable[hEffPart->type].destroy(hEffPart->pHandle);
    sdfChipFreeDirect(hEffPart);
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartDraw);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = hEffPartSrc->type;
    hEffPart->pHandle = dds3EffectFuncTable[hEffPartSrc->type].copy(hEffPartSrc->pHandle);
    return hEffPart;
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetPosition);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetScale);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetRotateMatrix);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetColor);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetDirection);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartSetArea);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartTMXBillboardCreate);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartP2ABillboardCreate);

void dds3EffectPartBillboardScale(void *hBillboard, f32 scale)
{
    dds3BillboardSetScale(hBillboard, scale, scale);
}

void dds3EffectPartModelInit(dds3ModelHandle_t *hModel)
{
    VU0_LOAD_VF10_PTR(&D_0031DF60);
    mdlSetPosition(hModel);
    VU0_LOAD_VF10_PTR(&D_0031DF70);
    mdlSetRotate(hModel);
    VU0_LOAD_VF10_PTR(&D_0031DF80);
    mdlSetScale(hModel);
    mdlSetColor(hModel, 0x80808080);
    if (hModel->motionCtrlr != NULL) {
        mdlLoopMotion(hModel, 0, 0);
        hModel->motionCtrlr->speed = 1.0f;
    }
    hModel->stat &= ~1;
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelCreate);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelDraw);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelDelete);

dds3ModelHandle_t *dds3EffectPartModelCopy(void *hData)
{
    s32 major;
    s32 minor;
    dds3ModelHandle_t *hModel;

    major = mdlGetMajorNum(hData);
    minor = mdlGetMinorNum(hData);
    hModel = mdlLoad(major, minor);
    dds3EffectPartModelInit(hModel);
    return hModel;
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelSetPosition);

void dds3EffectPartModelSetScale(void *hData, f32 scale)
{
    f32 v[3];

    v[0] = v[1] = v[2] = scale;
    VU0_LOAD_VF10_MEM(v);
    mdlSetScale(hData);
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelSetRotateMatrix);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPartModelSetColor);

dds3EffectPCP_t *dds3EffectPCPCreate(dds3EffectPCPData_t *hPCPData)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = dds3EffectPCPDataGetKateg(hPCPData);
    hPCP->param = dds3EffectPCPDataGetParam(hPCPData);
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(D_0031DDD0[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(hPCPData);
    }
    return hPCP;
}

void dds3EffectPCPDelete(dds3EffectPCP_t *hPCP)
{
    EffectPCPFuncTable[hPCP->kateg].destroy(hPCP->pHandle);
    sdfChipFreeDirect(hPCP);
}

void dds3EffectPCPDraw(dds3EffectPCP_t *hPCP)
{
    EffectPCPFuncTable[hPCP->kateg].draw(hPCP->pHandle);
}

dds3EffectPCP_t *dds3EffectPCPCopy(dds3EffectPCP_t *hPCPSrc)
{
    dds3EffectPCP_t *hPCP;

    hPCP = sdfChipAllocDirect(sizeof(dds3EffectPCP_t));
    hPCP->kateg = hPCPSrc->kateg;
    hPCP->param = hPCPSrc->param;
    if (EffectPCPFuncTable[hPCP->kateg].copy == NULL) {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].create(D_0031DDD0[hPCP->kateg][hPCP->param]);
    } else {
        hPCP->pHandle = EffectPCPFuncTable[hPCP->kateg].copy(hPCPSrc->pHandle);
    }
    return hPCP;
}

void dds3EffectPCPSetPosition(dds3EffectPCP_t *hPCP, void *pPos)
{
    if (EffectPCPFuncTable[hPCP->kateg].setPosition != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setPosition(hPCP->pHandle, pPos);
    }
}

void dds3EffectPCPSetScale(dds3EffectPCP_t *hPCP, f32 scale)
{
    if (EffectPCPFuncTable[hPCP->kateg].setScale != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setScale(hPCP->pHandle, scale);
    }
}

void dds3EffectPCPSetRotateMatrix(dds3EffectPCP_t *hPCP, void *pMatrix)
{
    if (EffectPCPFuncTable[hPCP->kateg].setRotateMatrix != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setRotateMatrix(hPCP->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetDirection(dds3EffectPCP_t *hPCP, void *pMatrix)
{
    if (EffectPCPFuncTable[hPCP->kateg].setDirection != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setDirection(hPCP->pHandle, pMatrix);
    }
}

void dds3EffectPCPSetColor(dds3EffectPCP_t *hPCP, u32 abgr)
{
    if (EffectPCPFuncTable[hPCP->kateg].setColor != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setColor(hPCP->pHandle, abgr);
    }
}

void dds3EffectPCPSetArea(dds3EffectPCP_t *hPCP, f32 area)
{
    if (EffectPCPFuncTable[hPCP->kateg].setArea != NULL) {
        EffectPCPFuncTable[hPCP->kateg].setArea(hPCP->pHandle, area);
    }
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPCPDataGetKateg);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPCPDataGetParam);

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPCPDataGetSize);

INCLUDE_ASM(const s32, "code_0016F490", dds3EffectPCPDataGetType);

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number)
{
    u16 type;
    void *pData;

    type = (u16)dds3EffectPCPDataGetType(hPCPData, number);
    pData = dds3EffectPCPDataGetData(hPCPData, number);
    return dds3EffectPartCreate(type, pData);
}

INCLUDE_SDATA(const s32, "code_0016F490", effBTLPointLightProcess);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0E4);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLFieldColorOn);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F1);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F4);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F0F8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F100);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLCutCount);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F10C);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F110);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F118);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F120);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F128);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F130);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F134);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F135);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F138);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F140);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F148);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F150);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F158);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F160);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F168);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F170);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F178);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F180);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F188);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18A);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18B);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F18C);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F190);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F198);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1A0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1A8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1B0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1B8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C0);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLDebugCameraAllEdit);

INCLUDE_SDATA(const s32, "code_0016F490", effBTLDebugCameraAllTable);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C3);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1C8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1D0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1D8);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1E0);

INCLUDE_SDATA(const s32, "code_0016F490", D_0054F1E4);

INCLUDE_RODATA(const s32, "code_0016F490", D_00533C40);

