#include "common.h"
#include "ps2asm.h"

typedef struct effPCPBossBED_t
{
    u8 unk0000[0xC3C];
    s32 active;
} effPCPBossBED_t;

/* The placement block every effEveBEDCutCreate call in this unit fills on the
 * stack. */
typedef struct effPCPBossPlace_t
{
    f32 payload[11];
    u32 color;
} effPCPBossPlace_t;

/* One crack segment: the extents effPCPCrackSizeInit randomizes plus the
 * negative age that delays its start. */
typedef struct effCrackCell_t
{
    f32 offsetDistance;
    f32 baseExtent;
    f32 tipExtent;
    s32 age;
    u8 flip;
    u8 unk11[3];
} effCrackCell_t;

typedef struct effCrackGroup_t
{
    void *drawPool;
    f32 direction[3];
    u8 unk10[4];
    f32 rotationAngle;
    s32 rotationStartAge;
    f32 angularSpeed;
    effCrackCell_t *cells;
} effCrackGroup_t;

typedef struct effCrackWork_t
{
    u8 position[0x10];
    u8 unk10[4];
    f32 frameStep;
    f32 scale;
    u8 unk1C[0x1C];
    u16 drawBucket;
    u8 unk3A[2];
    u8 hasCells;
    u8 unk3D[7];
    s32 delaySpread;
    u8 unk48[0x10];
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    u8 unk6C[0x18];
    s32 blend;
    u8 directionMode;
    u8 unk89[3];
    effCrackGroup_t *groups;
    void *groupsHandle;
    u32 groupCount;
    u16 cellCount;
    u8 unk9A[2];
    s32 unk9C;
    u32 color;
    void *system;
    s32 part;
} effCrackWork_t;

/* The parameter block effPCPCrackCreate copies into the work head; the two
 * rotation fields sit where the work keeps its group pointer. */
/* The leading block effPCPCrackCreate copies into the work head in one go. */
typedef struct effCrackHead_t
{
    s32 word[35];
} effCrackHead_t;

typedef struct effCrackParam_t
{
    effCrackHead_t head;
    s32 rotationStartAge;
    f32 angularSpeed;
} effCrackParam_t;

typedef struct effCrackMotion_t
{
    u8 unk00[0x20];
    f32 frameStep;
    u8 unk24[0xA];
    u16 frameCount;
} effCrackMotion_t;

typedef struct effCrackModel_t
{
    u8 unk00[0x18];
    void *inner;
    effCrackMotion_t *first;
} effCrackModel_t;

/* Five vertex colour words per cell. */
typedef struct effCrackColorSlot_t
{
    u32 color[5];
} effCrackColorSlot_t;

typedef struct effBaelHead_t
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} effBaelHead_t;

typedef struct effBaelWork_t
{
    f32 target[4];
    effBaelHead_t parameterVector;
    s32 frame;
    u32 color;
    s32 modelResource;
    s32 auxResource;
} effBaelWork_t;

typedef struct effPCPBossONGYParam_t
{
    s32 unk00;
} effPCPBossONGYParam_t;

typedef struct effPCPBossONGY_t
{
    s32 param;
    void *matrix;
    s32 unk08;
    s32 unk0C;
} effPCPBossONGY_t;

typedef struct effPCPBossWork_t
{
    u8 unk00[4];
    u16 unk04;
    u8 unk06[2];
    u32 unk08;
    const void *unk0C;
    u8 unk10[0x1C];
} effPCPBossWork_t;

typedef struct effPCPBossBUNSINParam_t
{
    s32 unk00;
    s32 unk04;
} effPCPBossBUNSINParam_t;

/* The marked node effPCPBossBUNSINCreate reads its two endpoints from. */
typedef struct effPCPBossBUNSINNode_t
{
    u8 unk00[0x20];
    f32 pos[3];
} effPCPBossBUNSINNode_t;

typedef struct effPCPBossBUNSIN_t
{
    f32 posA[4];
    f32 posB[4];
    effPCPBossBUNSINParam_t marks;
    void *matrix;
    s32 unk2C;
    s32 unk30;
} effPCPBossBUNSIN_t;

typedef struct effPCPBossBUNSIN2Head_t
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} effPCPBossBUNSIN2Head_t;

typedef struct effPCPBossBUNSIN2_t
{
    effPCPBossBUNSIN2Head_t head;
    s32 frame;
    s32 state;
    effPCPBossBUNSIN_t *parts[3];
} effPCPBossBUNSIN2_t;

typedef struct effPCPBossDengeki_t
{
    u8 unk00[0x54];
    f32 scale;
    void *polyLists[4];
    effPCPBossBED_t *bed;
    void *cut;
    u8 unk70[4];
    u32 color;
} effPCPBossDengeki_t;

/* One battle formation slot: the main process data holds 32 of them. */
typedef struct effPCPBossUnit_t
{
    u8 unk00[0x78];
    s32 flags;
    u8 unk7C[0x74];
} effPCPBossUnit_t;

typedef struct effPCPBossThunderbolt_t
{
    u8 position[0x10];
    u8 unk10[0x54];
    void *polyList;
    s32 frame;
    u32 color;
} effPCPBossThunderbolt_t;

typedef struct effPCPBossKAGUInvoke_t
{
    u8 unk000[0x3C0];
    s32 groupCount;
    void *polyLists[27];
    u32 color;
    void *handle;
} effPCPBossKAGUInvoke_t;

typedef struct effPCPBossKAGUAttack_t
{
    u8 unk00[0x54];
    f32 scale;
    u8 unk58[0xC];
    s32 groupCount;
    u8 unk68[4];
    void *polyLists[32];
    effPCPBossBED_t *bed;
    void *cut;
    u8 unkF4[4];
    u32 color;
} effPCPBossKAGUAttack_t;

typedef struct effPCPBossKAGUThunderbolt_t
{
    u8 position[0x10];
    u8 unk010[0xE0];
    f32 scale;
    u8 unk0F4[0xC];
    void *polyList;
    effCrackWork_t *crack;
    effPCPBossBED_t *bed;
    void *cut;
    u8 unk110[4];
    u32 color;
} effPCPBossKAGUThunderbolt_t;

typedef struct effPCPBossKAGU2Attack_t
{
    u8 unk00[0x54];
    f32 scale;
    u8 unk58[0xC];
    s32 groupCount;
    u8 unk68[4];
    void *polyLists[16];
    effPCPBossBED_t *bed;
    void *cut;
    u8 unkB4[4];
    u32 color;
} effPCPBossKAGU2Attack_t;

typedef struct effPCPBossKAGU2MUHENK_t
{
    s32 unk00;
    u32 color;
} effPCPBossKAGU2MUHENK_t;

typedef struct effPCPBossMETATOBeam_t
{
    u8 position[0x10];
    u8 unk10[0x60];
    void *polyLists[2];
    u8 unk78[4];
    u32 color;
} effPCPBossMETATOBeam_t;

typedef struct effPCPScreenCrackModel_t
{
    s32 param;
    s32 part;
    s32 unk08;
    u32 color;
} effPCPScreenCrackModel_t;

typedef struct effPCPMuzzleFlashHead_t
{
    s32 unk00;
    s32 unk04;
} effPCPMuzzleFlashHead_t;

typedef struct effPCPMuzzleFlash_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    effPCPMuzzleFlashHead_t head;
    void *billboard;
    s32 unk5C;
    f32 scale;
} effPCPMuzzleFlash_t;

typedef struct effPCPDANTE_Thunder_t
{
    u8 position[0x10];
    u8 matrix[0x40];
    u8 params[0x54];
    s32 mark;
    u8 unkA8[0x10];
    u32 color;
    void *poly;
    u8 unkC0[4];
    u8 drawRequest;
    u8 unkC5[3];
    s32 process;
} effPCPDANTE_Thunder_t;

typedef struct effPCPDANTE_Damage_t
{
    u8 position[0x10];
    u8 unk010[0x24];
    u8 unk034;
    u8 unk035[3];
    effPCPBossBED_t *bed;
    void *cuts[0x20];
    u8 unk0BC[0x80];
    u32 color;
} effPCPDANTE_Damage_t;

typedef struct effPCP_PBCamera_Param_t
{
    s32 unk00;
} effPCP_PBCamera_Param_t;

typedef struct effPCP_PBCamera_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    s32 param;
    s32 part;
    u8 unk58;
    u8 unk59[3];
    s32 unk5C;
} effPCP_PBCamera_t;

typedef struct effPCPDANTE_ShowTime_Param_t
{
    s32 unk00;
} effPCPDANTE_ShowTime_Param_t;

typedef struct effPCPDANTE_ShowTime_t
{
    u8 position[0x10];
    s32 param;
    u8 panels[3][0x38];
    void *textures[2];
    s32 unk0C4;
    u8 unk0C8[0xC];
    u8 texLoaded;
} effPCPDANTE_ShowTime_t;

typedef struct effPCP_BED_Param_t
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
} effPCP_BED_Param_t;

typedef struct effPCP_BED_t
{
    u8 matrix[0x40];
    u8 position[0x10];
    effPCP_BED_Param_t param;
    u32 color;
    s32 unk60;
    effPCPBossBED_t *bed;
    void *cut;
} effPCP_BED_t;

typedef struct effPCP_HBED2_Param_t
{
    s32 word[7];
} effPCP_HBED2_Param_t;

typedef struct effPCP_HBED2_t
{
    effPCP_HBED2_Param_t param;
    void *helper;
} effPCP_HBED2_t;

extern u8 dds3EffectBase[];

extern void *memcpy(void *dst, const void *src, u32 size);
extern void *sdfChipAllocDirect(u32 size);
extern void *sdfChipAllocZeroDirect(u32 size);
extern void *memset(void *dst, s32 value, u32 size);
extern void *sdfMatNew(void);
extern void sdfMatSetConstant(void *asset, f32 value);
extern void *dds3BillboardCreate(s32 count, void *params);
extern void *dds3BillboardCopy(void *billboard);
extern effPCPBossWork_t effPCPBossWork;
extern u8 D_0031FF20[];
extern void *sdfTexSet(const void *header);
extern void effEveBEDCutDelete(void *cut);
extern void effEveBEDDelete(effPCPBossBED_t *bed);
extern effPCPBossBED_t *effEveBEDCreate(const void *params);
extern void *effEveBEDCutCreate(effPCPBossBED_t *bed, s32 mode, const effPCPBossPlace_t *place);
extern void *effPCPHelperBEDCreate(void *param, void *pb, void *bed);
extern void effEveBEDCutSetParams(void *cut, const effPCPBossPlace_t *place);
extern void effEveBEDCutDraw(void *cut);
extern u32 effColorBlend(u32 firstColor, u32 secondColor, f32 blend);
extern void *dds3GetProcessData(void);
extern u32 effBTLSkillCheckRun(void);
extern void *effBTLSkillGetInvokeFormation(void);
extern void effBTLModelGetMarkMatrix2(void *model, s32 mark);
extern void effBTLModelGetMarkPosition2(void *model, s32 mark);
extern void effPCPCrackInit(effCrackWork_t *work);
extern void dds3PadVibrationSet(s32 port, u8 power, s32 frames);
extern effPCPBossUnit_t **nbGetMainProcessData(void);
extern s32 effBTLModelGetID(effPCPBossUnit_t *unit);
extern u128 *effPCP_ThunderPolyListGetParam(void *list);
extern void effPCP_ThunderPolyListDraw(void *list);
extern void effPCP_ThunderPolyListSetColor(void *list, u32 color);
extern void nbPanelFlashInit(void *panel, s32 a, s32 b, s32 c, f32 x, f32 y, s32 d);
extern void nbPanelFlashDraw(void *panel, u32 color);
extern void effBTLModelGetSphereCenter(void *model);
extern void *effBTLSkillGetTargetFormation(s32 index);
extern void LookAtMatrix(const void *target, const void *eye, const void *up);
extern void ReverseMatrix(void);
extern u8 D_0031FF80[];
extern void dds3EffectPartSetRotateMatrix(s32 part, const void *matrix);
extern void dds3EffectPartSetPosition(s32 part, const void *position);
extern void dds3EffectPartSetColor(s32 part, u32 color);
extern void dds3EffectPartDraw(s32 part);
void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_t *work, const void *source);
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_t *work, const void *source);
extern void *effPCPHelperBEDCopy(void *helper);
extern void sdfChipFreeDirect(void *ptr);
extern void *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(void *handle);
extern void sdfMemFreeDirect(void *handle);
extern u32 dds3GetRandA(u8 *state);
extern f32 dds3GetRandFloatA(u8 *state);
extern s32 dds3EffectPartCreate(s32 kind, void *param);
extern s32 dds3EffectPartCopy(s32 part);
extern void dds3EffectPartDelete(s32 part);
extern effCrackModel_t *dds3EffectPartGetHandle(s32 part);
extern void mdlSingleMotion(effCrackModel_t *model, s32 motion, s32 arg);
extern s32 sdfModelGetMarkPosCount(void *inner);
extern void *dds3TrackPolyCreate(s32 groups, s32 cells, s32 arg, s32 topology);
extern void dds3TrackPolySetMode(void *system, s32 mode);
extern void dds3TrackPolyDelete(void *system);
extern void *dds3FlashPolyListCreate(s32 count);
extern void dds3FlashPolyListDelete(void *pool);
extern f32 *dds3FlashPolyListGetVertex(void *pool, s32 index);
extern effCrackColorSlot_t *dds3FlashPolyListGetVC(void *pool, s32 index);
extern void dds3FlashPolyListSetBlend(void *pool, s32 blend);
extern effCrackWork_t *effPCPCrackCreate(effCrackParam_t *param, void *effectParam);
extern effCrackWork_t *effPCPCrackCopy(effCrackWork_t *src);


extern void *effPCPBossTexHandle;
extern s32 D_0054F1F4;
extern s32 effPCPBossONGY_RFrame;
extern s32 dds3RenderDrawEnv;

extern void dds3AllocRenderTexture(s32 width, s32 height, f32 arg2);
extern void dds3RenderTextureON(void);
extern void dds3RenderTextureOFF(void);
extern void dds3FreeRenderTexture(void);
extern void *mdlLoad(u32 group, u32 id);
extern void mdlFree(void *ctx);
extern void mdlSmoothLoopMotion(void *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                                f32 blendDurationFrames);
extern s32 dds3GetKernelCount(void);
extern s32 printf(const char *format, ...);

extern void sdfMatFreeDelayed(void *asset);
extern void effPCP_ThunderPolyListDelete(void *list);
extern void *dds3EffectPCPDataGetData(void *data, u32 number, ...);
extern effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(void *params);

extern f32 effPCPBossKAGUDrawParam[4];
extern f32 effPCPBossKAGU2DrawParam[4];

extern void dds3BillboardDelete(void *billboard);
extern void dds3BillboardSetRGBA(void *billboard, u32 color);
extern effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(void *params);
extern effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *params);
extern void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *work, const f32 *param);
extern void effPCPCrackDraw(effCrackWork_t *work);

extern void dds3KillProcess(s32 process, s32 flags);
extern void effPCP_ThunderPolyDelete(void *poly);
extern void sdfTexFreeDelayed(void *texture);
extern void mdlSetRotate(void *model);
extern void effPCP_BED_MatrixToQuaternion(const void *matrix);
extern void *effPCPHelperBEDGetModel(void *helper);
extern void effPCPHelperBEDDelete(void *helper);
extern void effPCPHelperBEDDraw(void *helper);
extern void effPCPHelperBEDSetPosition(void *helper, const void *source);
extern void effPCPHelperBEDSetColor(void *helper, u32 color);
extern void effPCPHelperBEDSetScale(void *helper, f32 scale);
extern effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_Create(void *params);
extern effPCPDANTE_Damage_t *effPCPDANTE_Damage_Create(void *params, void *bedParams);

extern s32 D_0054F200[2], D_0054F208[2];
extern u8 D_0054F210[2], D_0054F218[2];
extern effBaelWork_t *effPCPBossBaelCreate(void *p0, void *p1, void *p2);
extern effPCPBossDengeki_t *effPCPBossDengekiCreate(effPCPBossDengeki_t *param, void *bed);
extern effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreate(effPCPBossKAGUAttack_t *param,
                                                          void *bed);
extern effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreate(
    effPCPBossKAGUThunderbolt_t *param, void *pb, void *bed);
extern effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreate(effPCPBossKAGU2Attack_t *param,
                                                            void *bed);
extern effCrackWork_t *effPCPCrack2Create(effCrackParam_t *p0, void *p1);
extern effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *param, void *pb);
extern effPCPMuzzleFlash_t *effPCPMuzzleFlashCreate(effPCPMuzzleFlashHead_t *param,
                                                    void *billParams);
extern effPCP_PBCamera_t *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *param, void *pb);
extern effPCPDANTE_ShowTime_t *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *param,
                                                           void *tex0, void *tex1);
extern effPCP_BED_t *effPCP_BED_Create(effPCP_BED_Param_t *param, void *bedParams);
extern effPCP_HBED2_t *effPCP_HBED2_Create(effPCP_HBED2_Param_t *param, void *pb, void *bed);
void effPCPCrackSizeInit(effCrackWork_t *work, effCrackCell_t *cell);

void effPCPBossONGYTexRenderStart(void)
{
    if (D_0054F1F4 == 0) {
        dds3RenderDrawEnv = 0;
        dds3AllocRenderTexture(0x100, 0x100, 0.8f);
        dds3RenderTextureON();
        effPCPBossTexHandle = mdlLoad(1, 0x10D);
        mdlSmoothLoopMotion(effPCPBossTexHandle, 0, 0, 0.0f, 4.0f);
        effPCPBossONGY_RFrame = dds3GetKernelCount() - 1;
        printf("effPCPBossONGY:Model Load\n");
    }
    D_0054F1F4++;
}

void effPCPBossONGYTexRenderEnd(void)
{
    if (--D_0054F1F4 == 0) {
        dds3RenderTextureOFF();
        dds3FreeRenderTexture();
        mdlFree(effPCPBossTexHandle);
        effPCPBossTexHandle = NULL;
        printf("effPCPBossONGY:Model Free\n");
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYModelTexRender);

s32 effPCPBossONGYModelTexRenderCheck(void)
{
    return effPCPBossONGY_RFrame != dds3GetKernelCount();
}

effPCPBossONGY_t *effPCPBossONGYCreate(effPCPBossONGYParam_t *param, void *unused)
{
    effPCPBossONGY_t *work = sdfChipAllocDirect(0x10);
    s32 value = param->unk00;
    void *matrix;

    work->unk0C = 0;
    work->param = value;
    work->unk08 = 0;
    matrix = sdfMatNew();
    work->matrix = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    return work;
}

void effPCPBossONGYDelete(effPCPBossONGY_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDraw);

s32 effPCPBossONGYGetLength(void)
{
    return 0x16;
}

effPCPBossONGY_t *effPCPBossONGY2Create(effPCPBossONGYParam_t *param, void *unused)
{
    effPCPBossONGY_t *work = sdfChipAllocDirect(0x10);
    s32 value = param->unk00;
    void *matrix;

    work->unk0C = 0;
    work->param = value;
    work->unk08 = 0;
    matrix = sdfMatNew();
    work->matrix = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    return work;
}

void effPCPBossONGY2Delete(effPCPBossONGY_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Draw);

s32 effPCPBossONGY2GetLength(void)
{
    return 0x1B;
}

effPCPBossBUNSIN_t *effPCPBossBUNSINCreate(effPCPBossBUNSINParam_t *param)
{
    effPCPBossBUNSIN_t *work = sdfChipAllocDirect(0x34);
    effPCPBossBUNSINNode_t *node;
    void *matrix;

    work->marks = *param;
    work->unk2C = 0;
    node = (effPCPBossBUNSINNode_t *)work->marks.unk00;
    work->unk30 = 0;
    work->posA[0] = node->pos[0];
    work->posA[1] = node->pos[1];
    work->posA[2] = node->pos[2];
    work->posA[3] = 0.0f;
    node = (effPCPBossBUNSINNode_t *)work->marks.unk04;
    work->posB[0] = node->pos[0];
    work->posB[1] = node->pos[1];
    work->posB[2] = node->pos[2];
    work->posB[3] = 0.0f;
    matrix = sdfMatNew();
    work->matrix = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    effPCPBossWork.unk08 = 0x80808080;
    return work;
}

void effPCPBossBUNSINDelete(effPCPBossBUNSIN_t *work)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(work->matrix);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly2);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDraw);

s32 effPCPBossBUNSINGetLength(void)
{
    return 0x18;
}

effPCPBossBUNSIN2_t *effPCPBossBUNSIN2Create(effPCPBossBUNSIN2_t *param, void *unused)
{
    effPCPBossBUNSIN2_t *work = sdfChipAllocDirect(0x24);
    effPCPBossBUNSINParam_t sub;

    work->head = param->head;
    work->frame = 0;
    work->state = 0;
    sub.unk00 = work->head.unk00;
    sub.unk04 = work->head.unk04;
    work->parts[0] = effPCPBossBUNSINCreate(&sub);
    sub.unk00 = work->head.unk00;
    sub.unk04 = work->head.unk08;
    work->parts[1] = effPCPBossBUNSINCreate(&sub);
    sub.unk00 = work->head.unk04;
    sub.unk04 = work->head.unk0C;
    work->parts[2] = effPCPBossBUNSINCreate(&sub);
    return work;
}

void effPCPBossBUNSIN2Delete(effPCPBossBUNSIN2_t *work)
{
    effPCPBossBUNSINDelete(work->parts[2]);
    effPCPBossBUNSINDelete(work->parts[1]);
    effPCPBossBUNSINDelete(work->parts[0]);
    sdfChipFreeDirect(work);
}

void effPCPBossBUNSIN2Draw(effPCPBossBUNSIN2_t *work)
{
    s32 length = effPCPBossBUNSINGetLength();

    switch (work->state) {
    case 0:
        effPCPBossBUNSINDraw(work->parts[0]);
        if (work->frame >= length - 14) {
            work->state = 1;
        }
        break;
    case 1:
        if (work->frame < length) {
            effPCPBossBUNSINDraw(work->parts[0]);
        }
        effPCPBossBUNSINDraw(work->parts[1]);
        effPCPBossBUNSINDraw(work->parts[2]);
        break;
    }
    work->frame++;
}

s32 effPCPBossBUNSIN2GetLength(void)
{
    return effPCPBossBUNSINGetLength() * 2;
}

void effPCPCrackSizeInit(effCrackWork_t *work, effCrackCell_t *cell)
{
    f32 blend = work->offsetRandomness;
    f32 scale = work->scale;
    f32 t;

    cell->offsetDistance = work->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = work->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    cell->baseExtent = work->baseExtent * t * scale;
    cell->tipExtent = work->tipExtent * t * scale;
    cell->flip = dds3GetRandA(dds3EffectBase) & 1;
    cell->age = -(dds3GetRandA(dds3EffectBase) % work->delaySpread);
}

void effPCPCrackInit(effCrackWork_t *work)
{
    effCrackModel_t *model = dds3EffectPartGetHandle(work->part);
    effCrackGroup_t *group;
    effCrackCell_t *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if (work->delaySpread <= 0) {
        work->delaySpread = 1;
    }
    model->first->frameStep = work->frameStep;
    mdlSingleMotion(model, 0, 0);
    work->cellCount = model->first->frameCount;
    work->groupCount = sdfModelGetMarkPosCount(model->inner);
    work->system = dds3TrackPolyCreate(work->groupCount, work->cellCount, 1, 1);
    dds3TrackPolySetMode(work->system, work->drawBucket);
    work->groupsHandle = NULL;
    if (work->hasCells) {
        work->groupsHandle = sdfMemAllocDirect(work->groupCount * sizeof(effCrackGroup_t)
            + work->cellCount * work->groupCount * sizeof(effCrackCell_t));
        nextCells = (effCrackCell_t *)sdfMemLock(work->groupsHandle);
        work->groups = (effCrackGroup_t *)nextCells;
        nextCells = (effCrackCell_t *)((u8 *)nextCells + work->groupCount * sizeof(effCrackGroup_t));
        for (groupIndex = 0, group = work->groups; groupIndex < work->groupCount; groupIndex++, group++) {
            effCrackCell_t *cell;

            group->drawPool = dds3FlashPolyListCreate(work->cellCount);
            if (work->directionMode == 0) {
                group->direction[0] = 0.0f;
                group->direction[1] = -1.0f;
                group->direction[2] = 0.0f;
            } else {
                group->direction[0] = 0.0f;
                group->direction[1] = 1.0f;
                group->direction[2] = 0.0f;
            }
            group->angularSpeed = (0.1f * (3.14159265f / 180.0f));
            group->rotationAngle = 0.0f;
            for (cellIndex = 0; cellIndex < work->cellCount; cellIndex++) {
                f32 *vertices = dds3FlashPolyListGetVertex(group->drawPool, cellIndex);
                effCrackColorSlot_t *colors = dds3FlashPolyListGetVC(group->drawPool, cellIndex);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_STOREVECTOR(vertices + 4);
                SDF_VU0_STOREVECTOR(vertices + 8);
                SDF_VU0_STOREVECTOR(vertices + 12);
                SDF_VU0_STOREVECTOR(vertices + 16);
            }
            dds3FlashPolyListSetBlend(group->drawPool, work->blend);
            group->cells = nextCells;
            nextCells += work->cellCount;
            for (cellIndex = 0, cell = group->cells; cellIndex < work->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(work, cell);
            }
        }
    }
}

effCrackWork_t *effPCPCrackCreate(effCrackParam_t *param, void *effectParam)
{
    effCrackWork_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = param->head;
    work->color = 0x80808080;
    work->unk9C = 0;
    work->part = dds3EffectPartCreate(3, effectParam);
    effPCPCrackInit(work);
    return work;
}

void effPCPCrackCreatePCF(void *parameterTable) {
    effCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(parameterTable, 0);
    effectParams = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPCrackCreate(bossParams, effectParams);
}

/* NON_MATCHING: 10 of 73 words differ (first at +0xD0). */
#ifdef NON_MATCHING
effCrackWork_t *effPCPCrackCopy(effCrackWork_t *src)
{
    effCrackWork_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = *(effCrackHead_t *)src;
    work->color = 0x80808080;
    work->unk9C = 0;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackCopy);
#endif

void effPCPCrackDelete(effCrackWork_t *work)
{
    u32 groupIndex;

    if (work->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
            dds3FlashPolyListDelete(work->groups[groupIndex].drawPool);
        }
        sdfMemFreeDirect(work->groupsHandle);
    }
    dds3TrackPolyDelete(work->system);
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackDraw);

void effPCPCrackSetPosition(effCrackWork_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

void effPCPCrackSetColor(effCrackWork_t *work, u32 color)
{
    work->color = color;
}

effBaelWork_t *effPCPBossBaelCreate(void *vector, void *paramA, void *paramB)
{
    effBaelWork_t *work;

    work = sdfChipAllocDirect(sizeof(effBaelWork_t));
    memcpy(&work->parameterVector, vector, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->modelResource = dds3EffectPartCreate(3, paramA);
    work->auxResource = dds3EffectPartCreate(6, paramB);
    return work;
}

void effPCPBossBaelCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPBossBaelCreate(firstBlock, secondBlock, thirdBlock);
}

/* NON_MATCHING: 11 of 32 words differ (first at +0x24). */
#ifdef NON_MATCHING
effBaelWork_t *effPCPBossBaelCopy(effBaelWork_t *src, s32 index, s32 type)
{
    effBaelWork_t *work = sdfChipAllocDirect(0x30);

    memcpy(&work->parameterVector, &src->parameterVector, 0x10);
    work->frame = 0;
    work->color = 0x80808080;
    work->modelResource = dds3EffectPartCopy(src->modelResource);
    work->auxResource = dds3EffectPartCopy(src->auxResource);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelCopy);
#endif

void effPCPBossBaelDelete(effBaelWork_t *work)
{
    dds3EffectPartDelete(work->auxResource);
    dds3EffectPartDelete(work->modelResource);
    sdfChipFreeDirect(work);
}

void effPCPBossBaelDraw(effBaelWork_t *work)
{
    u128 matrix[4];

    if (effBTLSkillCheckRun() != 0) {
        if (work->frame == 0) {
            effBTLModelGetSphereCenter(effBTLSkillGetTargetFormation(0));
            SDF_VU0_STOREVECTOR(work);
        }
        LookAtMatrix(work, &work->parameterVector, D_0031FF80);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
        SDF_VU0_STOREMATRIX_MEMOPS(matrix[0], matrix[1], matrix[2], matrix[3]);
    } else {
        work->target[0] = 0.0f;
        work->target[1] = 0.0f;
        work->target[2] = 400.0f;
        work->target[3] = 0.0f;
        SDF_VU0_STOREIDENTITYVECTOR(work);
        SDF_VU0_STOREIDENTITY(matrix);
    }
    dds3EffectPartSetRotateMatrix(work->modelResource, matrix);
    dds3EffectPartSetRotateMatrix(work->auxResource, matrix);
    dds3EffectPartSetPosition(work->modelResource, &work->parameterVector);
    dds3EffectPartSetPosition(work->auxResource, &work->parameterVector);
    dds3EffectPartSetColor(work->modelResource, work->color);
    dds3EffectPartSetColor(work->auxResource, work->color);
    dds3EffectPartDraw(work->modelResource);
    dds3EffectPartDraw(work->auxResource);
    work->frame++;
}

void effPCPBossBaelSetPosition(effBaelWork_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(&work->parameterVector, source);
}

void effPCPBossBaelSetColor(effBaelWork_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiCreate);

void effPCPBossDengekiCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossDengekiCreate(firstBlock, secondBlock);
}

effPCPBossDengeki_t *effPCPBossDengekiCopy(effPCPBossDengeki_t *src, s32 index, s32 type)
{
    effPCPBossDengeki_t *work = effPCPBossDengekiCreate(src, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = src->bed;

    work->bed = bed;
    place.payload[0] = 0.0f;
    place.payload[1] = 0.0f;
    place.payload[2] = 0.0f;
    place.payload[3] = 0.0f;
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = work->scale;
    place.color = 0x80808080;
    work->cut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCPBossDengekiDelete(effPCPBossDengeki_t *work)
{
    void **list;
    s32 i;

    effEveBEDCutDelete(work->cut);
    if (work->bed->active == 0) {
        effEveBEDDelete(work->bed);
    }
    list = work->polyLists;
    for (i = 0; i < 4; i++) {
        effPCP_ThunderPolyListDelete(*list++);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossDengekiDraw);

void effPCPBossDengekiSetColor(effPCPBossDengeki_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossThunderboltCreate);

effPCPBossThunderbolt_t *effPCPBossThunderboltCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossThunderboltCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossThunderbolt_t *effPCPBossThunderboltCopy(void *params, s32 index, s32 type)
{
    return effPCPBossThunderboltCreate(params);
}

void effPCPBossThunderboltDelete(effPCPBossThunderbolt_t *work)
{
    effPCP_ThunderPolyListDelete(work->polyList);
    sdfChipFreeDirect(work);
}

void effPCPBossThunderboltDraw(effPCPBossThunderbolt_t *work)
{
    u128 pos;
    effPCPBossUnit_t *model;
    u128 *param;
    u32 bits;
    s32 id;
    s32 i;
    effPCPBossUnit_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = *nbGetMainProcessData();
        for (i = 0; i < 0x20; i++) {
            if ((model->flags & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x124 || id == 0x103) {
                    found = model;
                    break;
                }
            }
            model++;
        }
        if (found == NULL) {
            return;
        }
    }
    effBTLModelGetMarkPosition2(model, 0x14);
    SDF_VU0_STOREVECTOR(&pos);
    param = effPCP_ThunderPolyListGetParam(work->polyList);
    SDF_VU0_LOADVECTOR(&pos);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_STOREVECTOR(param);
    SDF_VU0_LOADVECTOR_B(work);
    PS2_FLOAT_BITS(bits, 0.33f);
    SDF_VU0_INTERPOLATE(bits);
    SDF_VU0_STOREVECTOR(&param[1]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    PS2_FLOAT_BITS(bits, 0.66f);
    SDF_VU0_INTERPOLATE(bits);
    SDF_VU0_STOREVECTOR(&param[2]);
    SDF_VU0_STOREVECTOR_B(&param[3]);
    effPCP_ThunderPolyListDraw(work->polyList);
    effPCP_ThunderPolyListSetColor(work->polyList, work->color);
    work->frame++;
}

void effPCPBossThunderboltSetPosition(effPCPBossThunderbolt_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

void effPCPBossThunderboltSetColor(effPCPBossThunderbolt_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeCreate);

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossKAGUInvokeCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCopy(void *params, s32 index, s32 type)
{
    return effPCPBossKAGUInvokeCreate(params);
}

void effPCPBossKAGUInvokeDelete(effPCPBossKAGUInvoke_t *work)
{
    void **list;
    s32 count;
    s32 i;

    count = work->groupCount * 6;
    if (count > 0) {
        i = count;
        list = work->polyLists;
        do {
            effPCP_ThunderPolyListDelete(*list++);
            i--;
        } while (i != 0);
    }
    sdfMemFreeDirect(work->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDraw);

void effPCPBossKAGUInvokeSetColor(effPCPBossKAGUInvoke_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackCreate);

void effPCPBossKAGUAttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGUAttackCreate(firstBlock, secondBlock);
}

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCopy(effPCPBossKAGUAttack_t *src, s32 index, s32 type)
{
    effPCPBossKAGUAttack_t *work = effPCPBossKAGUAttackCreate(src, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = src->bed;

    work->bed = bed;
    place.payload[0] = 0.0f;
    place.payload[1] = 0.0f;
    place.payload[2] = 0.0f;
    place.payload[3] = 0.0f;
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = work->scale;
    place.color = 0x80808080;
    work->cut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCPBossKAGUAttackDelete(effPCPBossKAGUAttack_t *work)
{
    u32 count;
    u32 i;

    effEveBEDCutDelete(work->cut);
    if (work->bed->active == 0) {
        effEveBEDDelete(work->bed);
    }
    count = work->groupCount * 8;
    for (i = 0; i < count; i++) {
        effPCP_ThunderPolyListDelete(work->polyLists[i]);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDraw);

void effPCPBossKAGUAttackSetColor(effPCPBossKAGUAttack_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCreate);

void effPCPBossKAGUThunderboltCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPBossKAGUThunderboltCreate(firstBlock, secondBlock, thirdBlock);
}

/* NON_MATCHING: 23 of 44 words differ (first at +0x8). */
#ifdef NON_MATCHING
effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCopy(effPCPBossKAGUThunderbolt_t *src,
                                                           s32 index, s32 type)
{
    effPCPBossKAGUThunderbolt_t *work = effPCPBossKAGUThunderboltCreate(src, NULL, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed;
    effCrackWork_t *crack;

    crack = effPCPCrackCopy(src->crack);
    bed = src->bed;
    work->bed = bed;
    work->crack = crack;
    place.payload[0] = 0.0f;
    place.payload[1] = 0.0f;
    place.payload[2] = 0.0f;
    place.payload[3] = 0.0f;
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = work->scale;
    place.color = 0x80808080;
    work->cut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCopy);
#endif

void effPCPBossKAGUThunderboltDelete(effPCPBossKAGUThunderbolt_t *work)
{
    effEveBEDCutDelete(work->cut);
    if (work->bed->active == 0) {
        effEveBEDDelete(work->bed);
    }
    effPCP_ThunderPolyListDelete(work->polyList);
    effPCPCrackDelete(work->crack);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltDrawSub);

void effPCPBossKAGUThunderboltDraw(effPCPBossKAGUThunderbolt_t *work)
{
    effPCPBossKAGUThunderboltDrawSub(work, effPCPBossKAGUDrawParam);
}

void effPCPBossKAGUThunderboltSetPosition(effPCPBossKAGUThunderbolt_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

void effPCPBossKAGUThunderboltSetColor(effPCPBossKAGUThunderbolt_t *work, u32 color)
{
    work->color = color;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackCreate);

void effPCPBossKAGU2AttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGU2AttackCreate(firstBlock, secondBlock);
}

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCopy(effPCPBossKAGU2Attack_t *src, s32 index,
                                                   s32 type)
{
    effPCPBossKAGU2Attack_t *work = effPCPBossKAGU2AttackCreate(src, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = src->bed;

    work->bed = bed;
    place.payload[0] = 0.0f;
    place.payload[1] = 0.0f;
    place.payload[2] = 0.0f;
    place.payload[3] = 0.0f;
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = work->scale;
    place.color = 0x80808080;
    work->cut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCPBossKAGU2AttackDelete(effPCPBossKAGU2Attack_t *work)
{
    u32 count;
    u32 i;

    effEveBEDCutDelete(work->cut);
    if (work->bed->active == 0) {
        effEveBEDDelete(work->bed);
    }
    count = work->groupCount * 4;
    for (i = 0; i < count; i++) {
        effPCP_ThunderPolyListDelete(work->polyLists[i]);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackGetSplitPoint);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDraw);

void effPCPBossKAGU2AttackSetColor(effPCPBossKAGU2Attack_t *work, u32 color)
{
    work->color = color;
}

void effPCPBossKAGU2ThunderboltDraw(effPCPBossKAGUThunderbolt_t *work)
{
    effPCPBossKAGUThunderboltDrawSub(work, effPCPBossKAGU2DrawParam);
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreate(void *params)
{
    effPCPBossKAGU2MUHENK_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGU2MUHENK_t));

    work->unk00 = 0;
    work->color = 0x80808080;
    return work;
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossKAGU2MUHENKCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCopy(void *params, s32 index, s32 type)
{
    return effPCPBossKAGU2MUHENKCreate(params);
}

void effPCPBossKAGU2MUHENKDelete(effPCPBossKAGU2MUHENK_t *work)
{
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2MUHENKDraw);

void effPCPBossKAGU2MUHENKSetColor(effPCPBossKAGU2MUHENK_t *work, u32 color)
{
    work->color = color;
}

effCrackWork_t *effPCPCrack2Create(effCrackParam_t *param, void *effectParam)
{
    effCrackWork_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(param, effectParam);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = param->rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = param->angularSpeed;
    }
    return work;
}

void effPCPCrack2CreatePCF(void *data) {
    effCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(data, 0);
    effectParams = dds3EffectPCPDataGetData(data, 1);
    effPCPCrack2Create(bossParams, effectParams);
}

effCrackWork_t *effPCPCrack2Copy(effCrackWork_t *src)
{
    effCrackWork_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->groupCount; groupIndex++) {
        work->groups[groupIndex].rotationStartAge = src->groups[groupIndex].rotationStartAge;
        work->groups[groupIndex].rotationAngle = 0.0f;
        work->groups[groupIndex].angularSpeed = src->groups[groupIndex].angularSpeed;
    }
    return work;
}

void effPCPCrack2Delete(effCrackWork_t *work)
{
    effPCPCrackDelete(work);
}

void effPCPCrack2Draw(effCrackWork_t *work)
{
    effPCPCrackDraw(work);
}

void effPCPCrack2SetPosition(effCrackWork_t *work, const void *source)
{
    effPCPCrackSetPosition(work, source);
}

void effPCPCrack2SetColor(effCrackWork_t *work, u32 color)
{
    effPCPCrackSetColor(work, color);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamCreate);

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *data, s32 index, s32 type)
{
    return effPCPBossMETATOBeamCreate(dds3EffectPCPDataGetData(data, 0, type));
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *params, s32 index, s32 type)
{
    return effPCPBossMETATOBeamCreate(params);
}

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *work)
{
    effPCP_ThunderPolyListDelete(work->polyLists[1]);
    effPCP_ThunderPolyListDelete(work->polyLists[0]);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamDraw);

void effPCPBossMETATOBeamSetPosition(effPCPBossMETATOBeam_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

void effPCPBossMETATOBeamSetColor(effPCPBossMETATOBeam_t *work, u32 color)
{
    work->color = color;
}

void effPCPScreenCrackModelInit(effPCPScreenCrackModel_t *work) {
}

effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *param, void *pb)
{
    effPCPScreenCrackModel_t *work = sdfChipAllocDirect(0x10);

    work->param = param->param;
    work->color = 0x80808080;
    work->unk08 = 0;
    if (pb != NULL) {
        work->part = dds3EffectPartCreate(3, pb);
    }
    effPCPScreenCrackModelInit(work);
    return work;
}

void effPCPScreenCrackModelCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPScreenCrackModelCreate(firstBlock, secondBlock);
}

effPCPScreenCrackModel_t *effPCPScreenCrackModelCopy(effPCPScreenCrackModel_t *src, s32 index,
                                                     s32 type)
{
    effPCPScreenCrackModel_t *work;

    work = effPCPScreenCrackModelCreate(work, NULL);
    work->part = dds3EffectPartCopy(src->part);
    return work;
}

void effPCPScreenCrackModelDelete(effPCPScreenCrackModel_t *work)
{
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelDraw);

void effPCPScreenCrackModelSetColor(effPCPScreenCrackModel_t *work, u32 color)
{
    work->color = color;
}

effPCPMuzzleFlash_t *effPCPMuzzleFlashCreate(effPCPMuzzleFlashHead_t *param, void *billParams)
{
    effPCPMuzzleFlash_t *work = sdfChipAllocDirect(0x64);

    work->head = *param;
    work->scale = 1.0f;
    work->unk5C = 0;
    SDF_VU0_STOREIDENTITY(work);
    work->billboard = dds3BillboardCreate(1, billParams);
    return work;
}

void effPCPMuzzleFlashCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPMuzzleFlashCreate(firstBlock, secondBlock);
}

effPCPMuzzleFlash_t *effPCPMuzzleFlashCopy(effPCPMuzzleFlash_t *src, s32 index, s32 type)
{
    effPCPMuzzleFlash_t *work = sdfChipAllocDirect(0x64);

    work->head = src->head;
    work->scale = 1.0f;
    work->unk5C = 0;
    SDF_VU0_STOREIDENTITY(work);
    work->billboard = dds3BillboardCopy(src->billboard);
    return work;
}

void effPCPMuzzleFlashDelete(effPCPMuzzleFlash_t *work)
{
    dds3BillboardDelete(work->billboard);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPMuzzleFlashDraw);

void effPCPMuzzleFlashSetColor(effPCPMuzzleFlash_t *work, u32 color)
{
    dds3BillboardSetRGBA(work->billboard, color);
}

void effPCPMuzzleFlashSetPosition(effPCPMuzzleFlash_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work->position, source);
}

void effPCPMuzzleFlashSetScale(effPCPMuzzleFlash_t *work, f32 scale)
{
    work->scale = 1.0f;
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPMuzzleFlashSetRotateMatrix(effPCPMuzzleFlash_t *work, const void *source)
{
    SDF_VU0_LOADMATRIX(source);
    SDF_VU0_STOREMATRIX(work);
}

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DA0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DB0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DC0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DD0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DE0);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Create);

/* NON_MATCHING: 15 of 43 words differ (first at +0x20). */
#ifdef NON_MATCHING
s32 effPCPDANTE_Thunder_ProcessDraw(void)
{
    effPCPDANTE_Thunder_t *work = dds3GetProcessData();
    u128 matrix[4];
    u128 position;
    void *model;

    if (work->drawRequest != 0) {
        if (effBTLSkillCheckRun() != 0) {
            model = effBTLSkillGetInvokeFormation();
            effBTLModelGetMarkMatrix2(model, work->mark);
            SDF_VU0_STOREMATRIX_MEMOPS(matrix[0], matrix[1], matrix[2], matrix[3]);
            effPCPDANTE_Thunder_SetRotateMatrix(work, matrix);
            effBTLModelGetMarkPosition2(model, work->mark);
            SDF_VU0_STOREVECTOR(&position);
            effPCPDANTE_Thunder_SetPosition(work, &position);
        }
        effPCPDANTE_Thunder_Draw(work);
        work->drawRequest = 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_ProcessDraw);
#endif

effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_CreatePCF(void *data, s32 index, s32 type)
{
    return effPCPDANTE_Thunder_Create(dds3EffectPCPDataGetData(data, 0, type));
}

void effPCPDANTE_Thunder_Delete(effPCPDANTE_Thunder_t *work)
{
    dds3KillProcess(work->process, 0);
    effPCP_ThunderPolyDelete(work->poly);
    sdfChipFreeDirect(work);
}

effPCPDANTE_Thunder_t *effPCPDANTE_Thunder_Copy(effPCPDANTE_Thunder_t *src, s32 index, s32 type)
{
    return effPCPDANTE_Thunder_Create(src->params);
}

void effPCPDANTE_Thunder_DrawSub(effPCPDANTE_Thunder_t *work)
{
    work->drawRequest = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Thunder_Draw);

void effPCPDANTE_Thunder_SetColor(effPCPDANTE_Thunder_t *work, u32 color)
{
    work->color = color;
}

void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_t *work, const void *source)
{
    SDF_VU0_LOADMATRIX(source);
    SDF_VU0_STOREMATRIX(work->matrix);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Create);

void effPCPDANTE_Damage_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPDANTE_Damage_Create(firstBlock, secondBlock);
}

effPCPDANTE_Damage_t *effPCPDANTE_Damage_Copy(effPCPDANTE_Damage_t *src, s32 index, s32 type)
{
    effPCPDANTE_Damage_t *work = effPCPDANTE_Damage_Create(src, NULL);

    work->unk034 = 0;
    work->bed = src->bed;
    return work;
}

void effPCPDANTE_Damage_Delete(effPCPDANTE_Damage_t *work)
{
    void **cut;
    u32 i;

    cut = work->cuts;
    for (i = 0; i < 0x20; i++) {
        if (*cut != NULL) {
            effEveBEDCutDelete(*cut);
        }
        cut++;
    }
    if (work->unk034 != 0) {
        effEveBEDDelete(work->bed);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Run);

void effPCPDANTE_Damage_SetColor(effPCPDANTE_Damage_t *work, u32 color)
{
    work->color = color;
}

effPCP_PBCamera_t *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *param, void *pb)
{
    effPCP_PBCamera_t *work = sdfChipAllocDirect(0x60);
    s32 value = param->unk00;

    work->unk5C = 0;
    work->param = value;
    work->unk58 = 0;
    if (pb != NULL) {
        work->part = dds3EffectPartCreate(3, pb);
    }
    return work;
}

void effPCP_PBCamera_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCP_PBCamera_Create(firstBlock, secondBlock);
}

effPCP_PBCamera_t *effPCP_PBCamera_Copy(effPCP_PBCamera_t *src, s32 index, s32 type)
{
    effPCP_PBCamera_t *work = effPCP_PBCamera_Create((effPCP_PBCamera_Param_t *)&src->param, NULL);

    work->part = dds3EffectPartCopy(src->part);
    return work;
}

void effPCP_PBCamera_Delete(effPCP_PBCamera_t *work)
{
    dds3EffectPartDelete(work->part);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Run);

void effPCP_PBCamera_SetPosition(effPCP_PBCamera_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work->position, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_PBCamera_SetRotateMatrix(effPCP_PBCamera_t *work, const void *source)
{
    SDF_VU0_LOADMATRIX(source);
    SDF_VU0_STOREMATRIX(work);
}

effPCPDANTE_ShowTime_t *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *param,
                                                    void *tex0, void *tex1)
{
    effPCPDANTE_ShowTime_t *work = sdfChipAllocZeroDirect(0xD8);

    work->param = param->unk00;
    work->unk0C4 = 0;
    if (tex0 != NULL && tex1 != NULL) {
        work->textures[0] = sdfTexSet(tex0);
        work->textures[1] = sdfTexSet(tex1);
        work->texLoaded = 1;
    }
    return work;
}

void effPCPDANTE_ShowTime_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCPDANTE_ShowTime_Create(firstBlock, secondBlock, thirdBlock);
}

effPCPDANTE_ShowTime_t *effPCPDANTE_ShowTime_Copy(effPCPDANTE_ShowTime_t *src, s32 index, s32 type)
{
    effPCPDANTE_ShowTime_t *work =
        effPCPDANTE_ShowTime_Create((effPCPDANTE_ShowTime_Param_t *)&src->param, NULL, NULL);

    work->textures[0] = src->textures[0];
    work->textures[1] = src->textures[1];
    work->texLoaded = 0;
    return work;
}

void effPCPDANTE_ShowTime_Delete(effPCPDANTE_ShowTime_t *work)
{
    if (work->texLoaded != 0) {
        sdfTexFreeDelayed(work->textures[1]);
        sdfTexFreeDelayed(work->textures[0]);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Sprite_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Bingo_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_AreYouReady_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_ShowTime_Draw);

/* NON_MATCHING: 4 of 75 words differ (first at +0x34). */
#ifdef NON_MATCHING
void effPCPDANTE_ShowTime_Draw(effPCPDANTE_ShowTime_t *work)
{
    s32 frame = work->unk0C4;

    if (frame == 0) {
        nbPanelFlashInit(work->panels[0], 0x43BBFF, 0x12A2F5, 0xA0, 0.45f, 0.35f, 0x5C);
        nbPanelFlashInit(work->panels[1], 0x3664FF, 0x3CF0, -0x8C, 0.55f, 0.45f, -0x3C);
        nbPanelFlashInit(work->panels[2], 0x31109F, 0x8D64C8, 0, 1.5f, 2.0f, 0x14);
    }
    if (frame >= 0) {
        nbPanelFlashDraw(work->panels[0], 0x80808080);
        effPCPDANTE_ShowTime_Bingo_Draw(work);
    }
    if (frame >= 0x16) {
        nbPanelFlashDraw(work->panels[1], 0x80808080);
        effPCPDANTE_ShowTime_AreYouReady_Draw(work);
    }
    if (frame >= 0x5C) {
        nbPanelFlashDraw(work->panels[2], 0x80808080);
        effPCPDANTE_ShowTime_ShowTime_Draw(work);
    }
    work->unk0C4++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Draw);
#endif

void effPCPDANTE_ShowTime_SetPosition(effPCPDANTE_ShowTime_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work, source);
}

/* NON_MATCHING: 41 of 62 words differ (first at +0x24). */
#ifdef NON_MATCHING
effPCP_BED_t *effPCP_BED_Create(effPCP_BED_Param_t *param, void *bedParams)
{
    effPCP_BED_t *work = sdfChipAllocDirect(0x6C);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed;

    work->param = *param;
    work->unk60 = 0;
    work->color = 0x80808080;
    SDF_VU0_STOREIDENTITYVECTOR(work->position);
    SDF_VU0_STOREIDENTITY(work);
    if (bedParams != NULL) {
        place.payload[0] = 0.0f;
        place.payload[1] = 0.0f;
        place.payload[2] = 0.0f;
        place.payload[3] = 0.0f;
        place.payload[4] = 0.0f;
        place.payload[5] = 0.0f;
        place.payload[6] = 0.0f;
        place.payload[7] = 1.0f;
        place.payload[8] = 100.0f;
        place.payload[9] = 100.0f;
        place.payload[10] = 1.0f;
        place.color = 0x80808080;
        bed = effEveBEDCreate(bedParams);
        work->bed = bed;
        work->cut = effEveBEDCutCreate(bed, 2, &place);
    }
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_Create);
#endif

void effPCP_BED_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCP_BED_Create(firstBlock, secondBlock);
}

effPCP_BED_t *effPCP_BED_Copy(effPCP_BED_t *src, s32 index, s32 type)
{
    effPCP_BED_t *work = effPCP_BED_Create(&src->param, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = src->bed;

    work->bed = bed;
    place.payload[0] = 0.0f;
    place.payload[1] = 0.0f;
    place.payload[2] = 0.0f;
    place.payload[3] = 0.0f;
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    work->cut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCP_BED_Delete(effPCP_BED_t *work)
{
    effEveBEDCutDelete(work->cut);
    if (work->bed->active == 0) {
        effEveBEDDelete(work->bed);
    }
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_MatrixToQuaternion);

void effPCP_BED_Draw(effPCP_BED_t *work)
{
    effPCPBossPlace_t place;
    s32 frame = work->unk60;
    s32 limit = work->param.unk00;
    s32 fadeIn = work->param.unk04;
    s32 fadeOut = work->param.unk08;
    s32 rest;
    f32 blend;
    u32 color;

    if (limit < frame) {
        return;
    }
    if (frame < fadeIn && fadeIn != 0) {
        blend = (f32)frame / (f32)fadeIn;
    } else {
        rest = limit - frame;
        if (rest <= fadeOut && fadeOut != 0) {
            blend = (f32)rest / (f32)fadeOut;
        } else {
            blend = 1.0f;
        }
    }
    color = effColorBlend(work->color & 0xFFFFFF, work->color, blend);
    SDF_VU0_COPYVECTOR(place.payload, work->position);
    effPCP_BED_MatrixToQuaternion(work);
    SDF_VU0_STOREVECTOR(&place.payload[4]);
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = color;
    effEveBEDCutSetParams(work->cut, &place);
    effEveBEDCutDraw(work->cut);
    work->unk60++;
}

void effPCP_BED_SetPosition(effPCP_BED_t *work, const void *source)
{
    SDF_VU0_COPYVECTOR(work->position, source);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_BED_SetRotateMatrix(effPCP_BED_t *work, const void *source)
{
    SDF_VU0_LOADMATRIX(source);
    SDF_VU0_STOREMATRIX(work);
}

void effPCP_BED_SetColor(effPCP_BED_t *work, u32 color)
{
    work->color = color;
}

effPCP_HBED2_t *effPCP_HBED2_Create(effPCP_HBED2_Param_t *param, void *pb, void *bed)
{
    effPCP_HBED2_t *work = sdfChipAllocDirect(0x20);

    work->param = *param;
    if (pb != NULL && bed != NULL) {
        work->helper = effPCPHelperBEDCreate(work, pb, bed);
    }
    return work;
}

void effPCP_HBED2_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    effPCP_HBED2_Create(firstBlock, secondBlock, thirdBlock);
}

effPCP_HBED2_t *effPCP_HBED2_Copy(effPCP_HBED2_t *src, s32 index, s32 type)
{
    effPCP_HBED2_t *work = effPCP_HBED2_Create((effPCP_HBED2_Param_t *)src, NULL, NULL);

    work->helper = effPCPHelperBEDCopy(src->helper);
    return work;
}

void effPCP_HBED2_Delete(effPCP_HBED2_t *work)
{
    effPCPHelperBEDDelete(work->helper);
    sdfChipFreeDirect(work);
}

void effPCP_HBED2_Draw(effPCP_HBED2_t *work)
{
    effPCPHelperBEDDraw(work->helper);
}

void effPCP_HBED2_SetPosition(effPCP_HBED2_t *work, const void *source)
{
    effPCPHelperBEDSetPosition(work->helper, source);
}

void effPCP_HBED2_SetRotateMatrix(effPCP_HBED2_t *work, const void *source)
{
    void *model = effPCPHelperBEDGetModel(work->helper);

    effPCP_BED_MatrixToQuaternion(source);
    mdlSetRotate(model);
}

void effPCP_HBED2_SetColor(effPCP_HBED2_t *work, u32 color)
{
    effPCPHelperBEDSetColor(work->helper, color);
}

void effPCP_HBED2_SetScale(effPCP_HBED2_t *work, f32 scale)
{
    effPCPHelperBEDSetScale(work->helper, scale);
}

void effPadVibeFadeUpdate(void)
{
    f32 full = 1.0f;
    f32 ratio;
    s32 port;

    for (port = 0; port < 2; port++) {
        if (D_0054F200[port] != 0) {
            if (D_0054F208[port] > 0) {
                ratio = (f32)D_0054F208[port] / (f32)D_0054F200[port];
                if (D_0054F218[port] != 0) {
                    ratio = full - ratio;
                }
                dds3PadVibrationSet(port, (u32)(D_0054F210[port] * ratio), 0x64);
                D_0054F208[port]--;
            } else {
                dds3PadVibrationSet(port, 0, 0);
                D_0054F200[port] = 0;
            }
        }
    }
}

void effPadVibeFadeOUT(u32 port, u8 intensity, s32 frames)
{
    if (port < 2) {
        D_0054F200[port] = frames;
        D_0054F210[port] = intensity;
        D_0054F218[port] = 0;
        D_0054F208[port] = frames;
    }
}

void effPadVibeFadeIN(u32 port, u8 intensity, s32 frames)
{
    if (port < 2) {
        D_0054F200[port] = frames;
        D_0054F210[port] = intensity;
        D_0054F218[port] = 1;
        D_0054F208[port] = frames;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", dds3EffectBlurDrawQuad);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533E10);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossTexHandle);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1F4);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossONGY_RFrame);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1FC);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F200);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F208);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F210);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F218);

