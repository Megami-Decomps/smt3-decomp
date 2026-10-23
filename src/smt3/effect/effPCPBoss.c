#include "common.h"
#include "ps2asm.h"

typedef struct effPCPBossBED_t
{
    u8 unk0000[0xC3C];
    s32 active;
} effPCPBossBED_t;

typedef union effPCPBossVector_t
{
    u128 q;
    f32 f[4];
} effPCPBossVector_t;

/* The placement block every effEveBEDCutCreate call in this unit fills on the
 * stack. */
typedef struct effPCPBossPlace_t
{
    f32 payload[11];
    u32 color;
} effPCPBossPlace_t;

/* One crack segment: the extents effPCPCrackSizeInit randomizes plus the
 * negative age that delays its start. */
/* The fragment parameter head effPCP_ThunderPolyListCreate copies out of a
 * work; only the fragment count is set by hand here. */
typedef struct effPCPBossThunderHead_t
{
    u8 unk00[0x24];
    s32 fragmentCount;
    u8 unk28[0x2C];
} effPCPBossThunderHead_t;

typedef struct effPCPBossThunderParam_t
{
    f32 points[10][4];
    s32 count;
    effPCPBossThunderHead_t head;
} effPCPBossThunderParam_t;

typedef struct effPCPCrackPartFlash_t
{
    f32 length;
    f32 width_in;
    f32 width_out;
    s32 frame;
    u8 type;
    u8 unk11[3];
} effPCPCrackPartFlash_t;

typedef struct effPCPCrackPart_t
{
    void *hFlash;
    f32 dir[3];
    u8 unk10[4];
    f32 rotate;
    s32 rotationStartAge;
    f32 rotate_add;
    effPCPCrackPartFlash_t *pSize;
} effPCPCrackPart_t;

typedef struct effPCPCrack_t
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
    effPCPCrackPart_t *pPart;
    void *groupsHandle;
    u32 MarkCount;
    u16 cellCount;
    u8 unk9A[2];
    s32 frame;
    u32 abgr;
    void *system;
    s32 part;
} effPCPCrack_t;

/* The parameter block effPCPCrackCreate copies into the work head; the two
 * rotation fields sit where the work keeps its group pointer. */
/* The leading block effPCPCrackCreate copies into the work head in one go. */
typedef struct effCrackHead_t
{
    s32 word[35];
} effCrackHead_t;

typedef struct effPCPCrackParam_t
{
    effCrackHead_t head;
    s32 rotationStartAge;
    f32 angularSpeed;
} effPCPCrackParam_t;

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

typedef struct effPCPBossBael_t
{
    f32 target[4];
    effBaelHead_t param;
    s32 frame;
    u32 color;
    s32 hPB;
    s32 hPCF;
} effPCPBossBael_t;

typedef struct effPCPBossONGYParam_t
{
    s32 pFormation;
} effPCPBossONGYParam_t;

typedef struct effPCPBossONGY_t
{
    s32 param;
    void *pMat;
    s32 radian;
    s32 frame;
} effPCPBossONGY_t;

/* The argument block effPkt3DPrimMakePoly reads. */
typedef struct effPCPBossWork_t
{
    u16 primCount;
    u16 vertexCount;
    u16 unk04;
    u8 unk06[2];
    u32 color;
    const void *unk0C;
    effPCPBossVector_t *positions;
    u8 unk14[4];
    f32 *uvs;
    u8 unk1C[4];
    u32 *colors;
    u8 unk24[8];
} effPCPBossWork_t;

typedef struct effPCPBossBUNSINParam_t
{
    s32 pStartForm;
    s32 pEndForm;
} effPCPBossBUNSINParam_t;

/* The marked node effPCPBossBUNSINCreate reads its two endpoints from. */
typedef struct effPCPBossBUNSINNode_t
{
    u8 unk00[0x20];
    f32 pos[3];
} effPCPBossBUNSINNode_t;

typedef struct effPCPBossBUNSIN_t
{
    f32 pos_s[4];
    f32 pos_e[4];
    effPCPBossBUNSINParam_t param;
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
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossDengeki_t;

/* The model node a formation's helper unit owns, down to the root the mark
 * lookups take. */
typedef struct effPCPBossOwner_t
{
    u8 unk00[0x18];
    void *inner;
} effPCPBossOwner_t;

typedef struct effPCPBossHUnit_t
{
    u8 unk00[0x8C];
    effPCPBossOwner_t *owner;
} effPCPBossHUnit_t;

/* One battle formation slot: the main process data holds 32 of them. */
typedef struct effPCPBossUnit_t
{
    u8 unk00[0x78];
    s32 flags;
    u8 unk7C[0x10];
    effPCPBossHUnit_t *hunit;
    u8 unk90[0x60];
} effPCPBossUnit_t;

typedef struct effPCPBossThunderbolt_t
{
    u8 position[0x10];
    u8 param[0x54];
    void *pTPolyList;
    s32 frame;
    u32 color;
} effPCPBossThunderbolt_t;

typedef struct effPCPBossKAGUInvoke_t
{
    u8 param[0x3C0];
    s32 point;
    void *polyLists[26];
    s32 frame;
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
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossKAGUAttack_t;

typedef struct effPCPBossKAGUThunderbolt_t
{
    u8 position[0x10];
    u8 unk010[0xE0];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    void *pTPolyList;
    effPCPCrack_t *pCrack;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
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
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 unkB4;
    u32 color;
} effPCPBossKAGU2Attack_t;

typedef struct effPCPBossKAGU2MUHENK_t
{
    s32 param;
    u32 color;
} effPCPBossKAGU2MUHENK_t;

typedef struct effPCPBossMETATOBeam_t
{
    u8 position[0x10];
    u8 unk10[0x54];
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    void *polyLists[2];
    s32 frame;
    u32 abgr;
} effPCPBossMETATOBeam_t;

typedef struct effPCPScreenCrackModel_t
{
    s32 param;
    s32 hModel;
    s32 frame;
    u32 abgr;
} effPCPScreenCrackModel_t;

typedef struct effPCPMuzzleFlashParam_t
{
    s32 scale_h;
    s32 scale_w;
} effPCPMuzzleFlashParam_t;

typedef struct effPCPMuzzleFlash_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCPMuzzleFlashParam_t param;
    void *hBillboard;
    s32 frame;
    f32 scale;
} effPCPMuzzleFlash_tag;

/* The block effPCPDANTE_Thunder_Copy hands straight back to Create. */
typedef struct effPCPDANTE_Thunder_Param_t
{
    u8 head[0x54];
    s32 mark;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
} effPCPDANTE_Thunder_Param_t;

typedef struct effPCPDANTE_Thunder_tag
{
    u8 position[0x10];
    u8 matrix[0x40];
    effPCPDANTE_Thunder_Param_t params;
    f32 scale;
    u32 color;
    void *poly;
    s32 frame;
    u8 drawRequest;
    u8 unkC5[3];
    s32 process;
} effPCPDANTE_Thunder_tag;

typedef struct effPCPDANTE_UV_tag
{
    s32 u;
    s32 v;
    u32 w;
    u32 h;
} effPCPDANTE_UV_tag;

/* The 16-byte block effPCPDANTE_Damage_Create copies into the work head; the
 * last word is the per-target delay step. */
typedef struct effPCPDANTE_Damage_Param_t
{
    u8 unk00[0xC];
    s32 wait;
} effPCPDANTE_Damage_Param_t;

typedef struct effPCPDANTE_Damage_tag
{
    u8 position[0x10];
    u8 unk010[0x20];
    s32 unk030;
    u8 unk034;
    u8 unk035[3];
    effPCPBossBED_t *bed;
    void *cuts[0x20];
    u8 unk0BC[0x80];
    u32 color;
} effPCPDANTE_Damage_tag;

typedef struct effPCP_PBCamera_Param_t
{
    s32 life;
} effPCP_PBCamera_Param_t;

typedef struct effPCP_PBCamera_tag
{
    u8 matrix[0x40];
    u8 position[0x10];
    s32 param;
    s32 part;
    u8 unk58;
    u8 unk59[3];
    s32 unk5C;
} effPCP_PBCamera_tag;

typedef struct effPCPDANTE_ShowTime_Param_t
{
    s32 reserve;
} effPCPDANTE_ShowTime_Param_t;

typedef struct effPCPDANTE_ShowTime_tag
{
    u8 position[0x10];
    s32 param;
    u8 panels[3][0x38];
    void *textures[2];
    s32 frame;
    u8 frameParts[0xC];
    u8 texLoaded;
} effPCPDANTE_ShowTime_tag;

typedef struct effPCP_BED_Param_t
{
    s32 life;
    s32 fade_in;
    s32 fade_out;
} effPCP_BED_Param_t;

typedef struct effPCP_BED_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCP_BED_Param_t param;
    u32 abgr;
    s32 frame;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
} effPCP_BED_tag;

typedef struct effPCP_HBED2_Param_t
{
    s32 word[7];
} effPCP_HBED2_Param_t;

typedef struct effPCP_HBED2_tag
{
    effPCP_HBED2_Param_t param;
    void *pHelperBED;
} effPCP_HBED2_tag;

extern u8 dds3EffectBase[];

extern void *memcpy(void *a, const void *b, u32 c);
extern void *sdfChipAllocDirect(u32 aSize);
extern void *sdfChipAllocZeroDirect(u32 aSize);
extern void *memset(void *a, s32 b, u32 c);
extern void *sdfMatNew(void);
extern void sdfMatSetConstant(void *aMat, f32 aRatio);
extern void *dds3BillboardCreate(s32 count, void *params);
extern void *dds3BillboardCopy(void *hBillboard);
extern effPCPBossWork_t effPCPBossWork;
extern u8 D_0031FF20[];
extern void *sdfTexSet(const void *aTex);
extern void effEveBEDCutDelete(void *hEveBEDCut);
extern void effEveBEDDelete(effPCPBossBED_t *hEveBED);
extern effPCPBossBED_t *effEveBEDCreate(const void *pData);
extern void *effEveBEDCutCreate(effPCPBossBED_t *hEveBED, s32 CutNum, const effPCPBossPlace_t *pParams);
extern void *effPCPHelperBEDCreate(void *pParam, void *pPBData, void *pBEDData);
extern void effEveBEDCutSetParams(void *hEveBEDCut, const effPCPBossPlace_t *pParams);
extern void effEveBEDCutDraw(void *hEveBEDCut);
extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
extern void *dds3GetProcessData(void);
extern u32 effBTLSkillCheckRun(void);
extern u32 effBTLSkillGetTargetBit(void);
extern u32 dds3GetRandIntA(void *aLimit, u32 name);
extern void *effBTLSkillGetInvokeFormation(void);
extern void effBTLModelGetMarkMatrix2(void *pFormation, s32 mark);
extern void effBTLModelGetMarkPosition2(void *pFormation, s32 mark);
extern void effDebugPointDraw(const void *pPos);
extern void effDebugLineDraw(const void *pPos0, const void *pPos1);
extern s32 sdfModelGetMarkPosMatrix(void *aRoot, s32 aId);
extern void effPCPCrackInit(effPCPCrack_t *pPCPCrack);
extern void dds3PadVibrationSet(s32 port, u8 power, s32 frames);
extern effPCPBossUnit_t **nbGetMainProcessData(void);
extern s32 effBTLModelGetID(effPCPBossUnit_t *pFormation);
extern u128 *effPCP_ThunderPolyListGetParam(void *pTPolyList);
extern void effPCP_ThunderPolyListDraw(void *hData);
extern void effPCP_ThunderPolyListSetColor(void *hData, u32 abgr);
extern void nbPanelFlashInit(void *pFlash, s32 scale_x, s32 scale_y, s32 abgr, f32 abgr_c, f32 addX, s32 addY);
extern void nbPanelFlashDraw(void *pFlash, u32 abgrBase);
extern void effBTLModelGetSphereCenter(void *pFormation);
extern void *effBTLSkillGetTargetFormation(s32 number);
extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
extern void ReverseMatrix(void);
extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(effPCPBossWork_t *aParm);
extern void sdfDrawTagLink(void *aTag, u32 aPacket);
extern u8 D_002F5C20[], D_002F5C30[];
extern u8 effPCPBossTexRect[];
extern u8 D_0031FF80[];
extern void dds3EffectPartSetRotateMatrix(s32 hEffPart, const void *pMatrix);
extern void dds3EffectPartSetPosition(s32 hEffPart, const void *pPos);
extern void dds3EffectPartSetColor(s32 hEffPart, u32 abgr);
extern void dds3EffectPartDraw(s32 hEffPart);
s32 effPCPDANTE_Thunder_ProcessDraw(void);
void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_tag *hData, const void *pPos);
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_tag *hData, const void *pRot);
extern void *effPCPHelperBEDCopy(void *helper);
extern void sdfChipFreeDirect(void *aPtr);
extern void *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(void *aHandle);
extern void sdfMemFreeDirect(void *aHandle);
extern u32 dds3GetRandA(u8 *name);
extern f32 dds3GetRandFloatA(u8 *name);
extern s32 dds3EffectPartCreate(s32 type, void *pData);
extern s32 dds3EffectPartCopy(s32 hEffPartSrc);
extern void dds3EffectPartDelete(s32 hEffPart);
extern effCrackModel_t *dds3EffectPartGetHandle(s32 hEffPart);
extern void mdlSingleMotion(effCrackModel_t *aHandle, s32 aGroup, s32 aNumber);
extern s32 sdfModelGetMarkPosCount(void *aRoot);
extern void *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, s32 type);
extern void dds3TrackPolySetMode(void *hTrackPoly, s32 mode);
extern void dds3TrackPolyDelete(void *hTrackPoly);
extern void *dds3FlashPolyListCreate(s32 face);
extern void dds3FlashPolyListDelete(void *hPoly);
extern f32 *dds3FlashPolyListGetVertex(void *pool, s32 index);
extern effCrackColorSlot_t *dds3FlashPolyListGetVC(void *pool, s32 index);
extern void dds3FlashPolyListSetBlend(void *hPoly, s32 blend);
extern effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB);
extern effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);


extern void *effPCPBossTexHandle;
extern s32 D_0054F1F4;
extern s32 effPCPBossONGY_RFrame;
extern s32 dds3RenderDrawEnv;

extern void dds3AllocRenderTexture(s32 sx, s32 sy, f32 aspect);
extern void dds3RenderTextureON(void);
extern void dds3RenderTextureOFF(void);
extern void dds3FreeRenderTexture(void);
extern void *mdlLoad(u32 aMajor, u32 aMinor);
extern void mdlFree(void *aHandle);
extern void mdlSmoothLoopMotion(void *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                                f32 aSmoothLeng);
extern s32 dds3GetKernelCount(void);
extern s32 printf(const char *format, ...);

extern void sdfMatFreeDelayed(void *aMat);
extern void effPCP_ThunderPolyListDelete(void *hData);
extern void *effPCP_ThunderPolyListCreate(const effPCPBossThunderParam_t *param);
extern void *dds3EffectPCPDataGetData(void *hData, u32 number, ...);
extern effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(effPCPBossThunderbolt_t *pParam);

extern f32 effPCPBossKAGUDrawParam[4];
extern f32 effPCPBossKAGU2DrawParam[4];

extern void dds3BillboardDelete(void *hData);
extern void dds3BillboardSetRGBA(void *hBillboard, u32 abgr);
extern effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(effPCPBossKAGUInvoke_t *pParam);
extern effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);
extern void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *pPCPBossKAGUThunderbolt, const f32 *pConvPos);
extern void effPCPCrackDraw(effPCPCrack_t *hData);

extern void dds3KillProcess(s32 id, s32 shutdowntimer);
extern void effPCP_ThunderPolyDelete(void *hData);
extern void sdfTexFreeDelayed(void *aHandle);
extern void mdlSetRotate(void *aHandle);
extern void effPCP_BED_MatrixToQuaternion(const void *pMatrix);
extern void *effPCPHelperBEDGetModel(void *pHelperBED);
extern void effPCPHelperBEDDelete(void *hData);
extern void effPCPHelperBEDDraw(void *hData);
extern void effPCPHelperBEDSetPosition(void *hData, const void *pPos);
extern void effPCPHelperBEDSetColor(void *hData, u32 abgr);
extern void effPCPHelperBEDSetScale(void *hData, f32 scale);
extern effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Create(const effPCPDANTE_Thunder_Param_t *pParam);
extern effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Create(void *pParam, void *pBED);
extern u8 D_00533E10[];
extern u8 D_00533DD0[];
extern u8 D_00533DE0[];
extern void Mult(void);
extern void ToMatrix(void);
extern void MultMatrixNoArg(void);
extern s32 dds3BillboardGetAnimFrame(void *hBillboard);
extern void dds3BillboardSetAnimKey2(void *hBillboard, s32 frame);
extern f32 dds3BillboardGetDirectionRotate(void *pPos, const void *pVec);
extern f32 dds3BillboardGetDirectionScale(void *pPos, const void *pVec);
extern void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);
extern void dds3BillboardSetRotate(void *hBillboard, f32 rot);
extern void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
extern void dds3BillboardDraw(void *hData);
extern s32 sdfModelGetMarkPos(void *aRoot, s32 aId);
extern void SetRotateMatrixY_B(f32 aAngle);
extern void SetRotateMatrixAxis(f32 aAngle, const void *aAxis);
extern void SetRotateMatrixZ(f32 aAngle);
extern void *effPCP_ThunderPolyCreate(const void *params);
extern s32 dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                            void *drawExit, void *deleteExit, void *p);
extern void dds3SetProcessData(s32 id, void *data);
extern const char D_00533DF0[];
extern u128 *effPCP_ThunderPolyGetParam(void *pThunderPoly);
extern void effPCP_ThunderPolySetColor(void *hData, u32 abgr);
extern void effPCP_ThunderPolyDraw(void *hData);
extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, u32 uw, u32 vh, u32 rgba,
                        void *th);

extern s32 D_0054F200[2], D_0054F208[2];
extern u8 D_0054F210[2], D_0054F218[2];
extern effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF);
extern effPCPBossDengeki_t *effPCPBossDengekiCreate(effPCPBossDengeki_t *pParam, void *pBEDData);
extern effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreate(effPCPBossKAGUAttack_t *pParam,
                                                          void *pBEDData);
extern effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreate(
    effPCPBossKAGUThunderbolt_t *pParam, void *pBP, void *pBED);
extern effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreate(effPCPBossKAGU2Attack_t *pParam,
                                                            void *pBEDData);
extern effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB);
extern effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *pParam, void *pPB);
extern effPCPMuzzleFlash_tag *effPCPMuzzleFlashCreate(effPCPMuzzleFlashParam_t *pParam,
                                                    void *pP2A);
extern effPCP_PBCamera_tag *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *pParam, void *pPB);
extern effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *pParam,
                                                           void *pTMX0, void *pTMX1);
extern effPCP_BED_tag *effPCP_BED_Create(effPCP_BED_Param_t *pParam, void *pBED);
extern effPCP_HBED2_tag *effPCP_HBED2_Create(effPCP_HBED2_Param_t *pParam, void *pPBData, void *pBEDData);
void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize);

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
    s32 value = param->pFormation;
    void *matrix;

    work->frame = 0;
    work->param = value;
    work->radian = 0;
    matrix = sdfMatNew();
    work->pMat = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    return work;
}

void effPCPBossONGYDelete(effPCPBossONGY_t *pPCPBossONGY)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(pPCPBossONGY->pMat);
    sdfChipFreeDirect(pPCPBossONGY);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDraw);

s32 effPCPBossONGYGetLength(void)
{
    return 0x16;
}

effPCPBossONGY_t *effPCPBossONGY2Create(effPCPBossONGYParam_t *param, void *unused)
{
    effPCPBossONGY_t *work = sdfChipAllocDirect(0x10);
    s32 value = param->pFormation;
    void *matrix;

    work->frame = 0;
    work->param = value;
    work->radian = 0;
    matrix = sdfMatNew();
    work->pMat = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    return work;
}

void effPCPBossONGY2Delete(effPCPBossONGY_t *pPCPBossONGY2)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(pPCPBossONGY2->pMat);
    sdfChipFreeDirect(pPCPBossONGY2);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Draw);

s32 effPCPBossONGY2GetLength(void)
{
    return 0x1B;
}

effPCPBossBUNSIN_t *effPCPBossBUNSINCreate(effPCPBossBUNSINParam_t *pParam)
{
    effPCPBossBUNSIN_t *work = sdfChipAllocDirect(0x34);
    effPCPBossBUNSINNode_t *node;
    void *matrix;

    work->param = *pParam;
    work->unk2C = 0;
    node = (effPCPBossBUNSINNode_t *)work->param.pStartForm;
    work->unk30 = 0;
    work->pos_s[0] = node->pos[0];
    work->pos_s[1] = node->pos[1];
    work->pos_s[2] = node->pos[2];
    work->pos_s[3] = 0.0f;
    node = (effPCPBossBUNSINNode_t *)work->param.pEndForm;
    work->pos_e[0] = node->pos[0];
    work->pos_e[1] = node->pos[1];
    work->pos_e[2] = node->pos[2];
    work->pos_e[3] = 0.0f;
    matrix = sdfMatNew();
    work->matrix = matrix;
    sdfMatSetConstant(matrix, 1.0f);
    effPCPBossONGYTexRenderStart();
    memset(&effPCPBossWork, 0, sizeof(effPCPBossWork_t));
    effPCPBossWork.unk04 = 0x4000;
    effPCPBossWork.unk0C = D_0031FF20;
    effPCPBossWork.color = 0x80808080;
    return work;
}

void effPCPBossBUNSINDelete(effPCPBossBUNSIN_t *pPCPBossBUNSIN)
{
    effPCPBossONGYTexRenderEnd();
    sdfMatFreeDelayed(pPCPBossBUNSIN->matrix);
    sdfChipFreeDirect(pPCPBossBUNSIN);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly);

/* vu0 routine: the BUNSIN billboard quad. */
/* NON_MATCHING: 9 of 117 words differ (first at +0x90). */
#ifdef NON_MATCHING
void effPCPBossBUNSINDrawPoly2(void *tag, void *asset, const u128 *center, u32 color, f32 width,
                               f32 height)
{
    effPCPBossVector_t points[4];
    f32 uv[8];

    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR(center);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(tag, 0);
    LookAtMatrix(D_002F5C20, D_002F5C30, effPCPBossTexRect);
    ReverseMatrix();
    width = width * 0.5f;
    height = height * 0.5f;
    effPCPBossWork.color = color;
    effPCPBossWork.vertexCount = 4;
    effPCPBossWork.uvs = uv;
    effPCPBossWork.primCount = 2;
    uv[7] = 1.0f;
    effPCPBossWork.positions = points;
    points[0].f[1] = -height;
    points[0].f[0] = -width;
    effPCPBossWork.colors = NULL;
    uv[2] = 1.0f;
    uv[5] = 1.0f;
    uv[6] = 1.0f;
    uv[0] = 0.0f;
    uv[1] = 0.0f;
    uv[3] = 0.0f;
    uv[4] = 0.0f;
    points[0].f[2] = 0.0f;
    points[0].f[3] = 0.0f;
    SDF_VU0_LOADVECTOR(&points[0]);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&points[0]);
    points[1].f[0] = width;
    points[1].f[1] = -height;
    points[1].f[2] = 0.0f;
    points[1].f[3] = 0.0f;
    SDF_VU0_LOADVECTOR(&points[1]);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&points[1]);
    points[2].f[0] = -width;
    points[2].f[1] = height;
    points[2].f[2] = 0.0f;
    points[2].f[3] = 0.0f;
    SDF_VU0_LOADVECTOR(&points[2]);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&points[2]);
    points[3].f[0] = width;
    points[3].f[1] = height;
    points[3].f[2] = 0.0f;
    points[3].f[3] = 0.0f;
    SDF_VU0_LOADVECTOR(&points[3]);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&points[3]);
    sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly2);
#endif

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
    sub.pStartForm = work->head.unk00;
    sub.pEndForm = work->head.unk04;
    work->parts[0] = effPCPBossBUNSINCreate(&sub);
    sub.pStartForm = work->head.unk00;
    sub.pEndForm = work->head.unk08;
    work->parts[1] = effPCPBossBUNSINCreate(&sub);
    sub.pStartForm = work->head.unk04;
    sub.pEndForm = work->head.unk0C;
    work->parts[2] = effPCPBossBUNSINCreate(&sub);
    return work;
}

void effPCPBossBUNSIN2Delete(effPCPBossBUNSIN2_t *pPCPBossBUNSIN2)
{
    effPCPBossBUNSINDelete(pPCPBossBUNSIN2->parts[2]);
    effPCPBossBUNSINDelete(pPCPBossBUNSIN2->parts[1]);
    effPCPBossBUNSINDelete(pPCPBossBUNSIN2->parts[0]);
    sdfChipFreeDirect(pPCPBossBUNSIN2);
}

void effPCPBossBUNSIN2Draw(effPCPBossBUNSIN2_t *pPCPBossBUNSIN2)
{
    s32 length = effPCPBossBUNSINGetLength();

    switch (pPCPBossBUNSIN2->state) {
    case 0:
        effPCPBossBUNSINDraw(pPCPBossBUNSIN2->parts[0]);
        if (pPCPBossBUNSIN2->frame >= length - 14) {
            pPCPBossBUNSIN2->state = 1;
        }
        break;
    case 1:
        if (pPCPBossBUNSIN2->frame < length) {
            effPCPBossBUNSINDraw(pPCPBossBUNSIN2->parts[0]);
        }
        effPCPBossBUNSINDraw(pPCPBossBUNSIN2->parts[1]);
        effPCPBossBUNSINDraw(pPCPBossBUNSIN2->parts[2]);
        break;
    }
    pPCPBossBUNSIN2->frame++;
}

s32 effPCPBossBUNSIN2GetLength(void)
{
    return effPCPBossBUNSINGetLength() * 2;
}

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize)
{
    f32 blend = pPCPCrack->offsetRandomness;
    f32 scale = pPCPCrack->scale;
    f32 t;

    pSize->length = pPCPCrack->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = pPCPCrack->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    pSize->width_in = pPCPCrack->baseExtent * t * scale;
    pSize->width_out = pPCPCrack->tipExtent * t * scale;
    pSize->type = dds3GetRandA(dds3EffectBase) & 1;
    pSize->frame = -(dds3GetRandA(dds3EffectBase) % pPCPCrack->delaySpread);
}

void effPCPCrackInit(effPCPCrack_t *pPCPCrack)
{
    effCrackModel_t *model = dds3EffectPartGetHandle(pPCPCrack->part);
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if (pPCPCrack->delaySpread <= 0) {
        pPCPCrack->delaySpread = 1;
    }
    model->first->frameStep = pPCPCrack->frameStep;
    mdlSingleMotion(model, 0, 0);
    pPCPCrack->cellCount = model->first->frameCount;
    pPCPCrack->MarkCount = sdfModelGetMarkPosCount(model->inner);
    pPCPCrack->system = dds3TrackPolyCreate(pPCPCrack->MarkCount, pPCPCrack->cellCount, 1, 1);
    dds3TrackPolySetMode(pPCPCrack->system, pPCPCrack->drawBucket);
    pPCPCrack->groupsHandle = NULL;
    if (pPCPCrack->hasCells) {
        pPCPCrack->groupsHandle = sdfMemAllocDirect(pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t)
            + pPCPCrack->cellCount * pPCPCrack->MarkCount * sizeof(effPCPCrackPartFlash_t));
        nextCells = (effPCPCrackPartFlash_t *)sdfMemLock(pPCPCrack->groupsHandle);
        pPCPCrack->pPart = (effPCPCrackPart_t *)nextCells;
        nextCells = (effPCPCrackPartFlash_t *)((u8 *)nextCells + pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t));
        for (groupIndex = 0, group = pPCPCrack->pPart; groupIndex < pPCPCrack->MarkCount; groupIndex++, group++) {
            effPCPCrackPartFlash_t *cell;

            group->hFlash = dds3FlashPolyListCreate(pPCPCrack->cellCount);
            if (pPCPCrack->directionMode == 0) {
                group->dir[0] = 0.0f;
                group->dir[1] = -1.0f;
                group->dir[2] = 0.0f;
            } else {
                group->dir[0] = 0.0f;
                group->dir[1] = 1.0f;
                group->dir[2] = 0.0f;
            }
            group->rotate_add = (0.1f * (3.14159265f / 180.0f));
            group->rotate = 0.0f;
            for (cellIndex = 0; cellIndex < pPCPCrack->cellCount; cellIndex++) {
                f32 *vertices = dds3FlashPolyListGetVertex(group->hFlash, cellIndex);
                effCrackColorSlot_t *colors = dds3FlashPolyListGetVC(group->hFlash, cellIndex);

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
            dds3FlashPolyListSetBlend(group->hFlash, pPCPCrack->blend);
            group->pSize = nextCells;
            nextCells += pPCPCrack->cellCount;
            for (cellIndex = 0, cell = group->pSize; cellIndex < pPCPCrack->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(pPCPCrack, cell);
            }
        }
    }
}

effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = pParam->head;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->part = dds3EffectPartCreate(3, pPB);
    effPCPCrackInit(work);
    return work;
}

void effPCPCrackCreatePCF(void *parameterTable) {
    effPCPCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(parameterTable, 0);
    effectParams = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPCrackCreate(bossParams, effectParams);
}

/* NON_MATCHING: 10 of 73 words differ (first at +0xD0). */
#ifdef NON_MATCHING
effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src)
{
    effPCPCrack_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = *(effCrackHead_t *)src;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackCopy);
#endif

void effPCPCrackDelete(effPCPCrack_t *hData)
{
    u32 groupIndex;

    if (hData->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < hData->MarkCount; groupIndex++) {
            dds3FlashPolyListDelete(hData->pPart[groupIndex].hFlash);
        }
        sdfMemFreeDirect(hData->groupsHandle);
    }
    dds3TrackPolyDelete(hData->system);
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackDraw);

void effPCPCrackSetPosition(effPCPCrack_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPCrackSetColor(effPCPCrack_t *hData, u32 abgr)
{
    hData->abgr = abgr;
}

effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, pParam, 0x10);
    work->color = 0x80808080;
    work->frame = 0;
    work->hPB = dds3EffectPartCreate(3, pPB);
    work->hPCF = dds3EffectPartCreate(6, pPCF);
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
effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossBael_t *work = sdfChipAllocDirect(0x30);

    memcpy(&work->param, &hData->param, 0x10);
    work->frame = 0;
    work->color = 0x80808080;
    work->hPB = dds3EffectPartCopy(hData->hPB);
    work->hPCF = dds3EffectPartCopy(hData->hPCF);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBaelCopy);
#endif

void effPCPBossBaelDelete(effPCPBossBael_t *hData)
{
    dds3EffectPartDelete(hData->hPCF);
    dds3EffectPartDelete(hData->hPB);
    sdfChipFreeDirect(hData);
}

void effPCPBossBaelDraw(effPCPBossBael_t *hData)
{
    u128 matrix[4];

    if (effBTLSkillCheckRun() != 0) {
        if (hData->frame == 0) {
            effBTLModelGetSphereCenter(effBTLSkillGetTargetFormation(0));
            SDF_VU0_STOREVECTOR(hData);
        }
        LookAtMatrix(hData, &hData->param, D_0031FF80);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
        SDF_VU0_STOREMATRIX_MEMOPS(matrix[0], matrix[1], matrix[2], matrix[3]);
    } else {
        hData->target[0] = 0.0f;
        hData->target[1] = 0.0f;
        hData->target[2] = 400.0f;
        hData->target[3] = 0.0f;
        SDF_VU0_STOREIDENTITYVECTOR(hData);
        SDF_VU0_STOREIDENTITY(matrix);
    }
    dds3EffectPartSetRotateMatrix(hData->hPB, matrix);
    dds3EffectPartSetRotateMatrix(hData->hPCF, matrix);
    dds3EffectPartSetPosition(hData->hPB, &hData->param);
    dds3EffectPartSetPosition(hData->hPCF, &hData->param);
    dds3EffectPartSetColor(hData->hPB, hData->color);
    dds3EffectPartSetColor(hData->hPCF, hData->color);
    dds3EffectPartDraw(hData->hPB);
    dds3EffectPartDraw(hData->hPCF);
    hData->frame++;
}

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(&hData->param, pPos);
}

void effPCPBossBaelSetColor(effPCPBossBael_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossDengeki_t *effPCPBossDengekiCreate(effPCPBossDengeki_t *pParam, void *pBEDData)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossPlace_t place;
    effPCPBossDengeki_t *work = sdfChipAllocDirect(sizeof(effPCPBossDengeki_t));
    s32 i;

    memcpy(work, pParam, 0x58);
    work->frame = 0;
    work->color = 0x80808080;
    if (pBEDData != NULL) {
        effPCPBossBED_t *bed;

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
        bed = effEveBEDCreate(pBEDData);
        work->hBED = bed;
        work->cut = effEveBEDCutCreate(bed, 2, &place);
    }
    memcpy(&thunder.head, work, 0x54);
    thunder.count = 4;
    for (i = 0; i < 4; i++) {
        work->polyLists[i] = effPCP_ThunderPolyListCreate(&thunder);
    }
    return work;
}

void effPCPBossDengekiCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossDengekiCreate(firstBlock, secondBlock);
}

effPCPBossDengeki_t *effPCPBossDengekiCopy(effPCPBossDengeki_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossDengeki_t *work = effPCPBossDengekiCreate(hData, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = hData->hBED;

    work->hBED = bed;
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

void effPCPBossDengekiDelete(effPCPBossDengeki_t *hData)
{
    void **list;
    s32 i;

    effEveBEDCutDelete(hData->cut);
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    list = hData->polyLists;
    for (i = 0; i < 4; i++) {
        effPCP_ThunderPolyListDelete(*list++);
    }
    sdfChipFreeDirect(hData);
}

void effPCPBossDengekiDraw(effPCPBossDengeki_t *hData)
{
    effPCPBossPlace_t place;
    effPCPBossVector_t p14;
    effPCPBossVector_t p17;
    effPCPBossVector_t p18;
    effPCPBossVector_t p15;
    effPCPBossVector_t p16;
    effPCPBossVector_t segA0;
    effPCPBossVector_t segB0;
    effPCPBossVector_t segA1;
    effPCPBossVector_t segB1;
    effPCPBossVector_t scale;
    u128 *param;
    effPCPBossUnit_t *model;
    effPCPBossUnit_t *found;
    s32 id;
    s32 i;
    f32 r;

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
    SDF_VU0_STOREVECTOR(&p14);
    p14.f[1] = p14.f[1] + 0.0f;
    effBTLModelGetMarkPosition2(model, 0x15);
    SDF_VU0_STOREVECTOR(&p15);
    effBTLModelGetMarkPosition2(model, 0x16);
    SDF_VU0_STOREVECTOR(&p16);
    effBTLModelGetMarkPosition2(model, 0x17);
    SDF_VU0_STOREVECTOR(&p17);
    effBTLModelGetMarkPosition2(model, 0x18);
    SDF_VU0_STOREVECTOR(&p18);
    scale.f[3] = 100.0f;
    scale.f[2] = 100.0f;
    scale.f[1] = 100.0f;
    scale.f[0] = 100.0f;
    SDF_VU0_LOADVECTOR((u8 *)model + 0x30);
    ToMatrix();
    SDF_VU0_COPYMATRIX_A_TO_C();
    SDF_VU0_LOADIDENTITYVECTOR();
    SDF_VU0_SETVECTOR_Z(1.0f);
    r = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    r = r + r;
    SetRotateMatrixY_B(r * 0.26179934f);
    MultMatrixNoArg();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&p15);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.33f);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR_NOMEM(&segA0);
    SDF_VU0_LOADVECTOR(&p15);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.66f);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR_NOMEM(&segA1);
    SDF_VU0_COPYMATRIX_C_TO_A();
    SDF_VU0_LOADIDENTITYVECTOR();
    SDF_VU0_SETVECTOR_Z(1.0f);
    r = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    r = r + r;
    SetRotateMatrixY_B(r * 0.26179934f);
    MultMatrixNoArg();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&p17);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.33f);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR_NOMEM(&segB0);
    SDF_VU0_LOADVECTOR(&p17);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.66f);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR_NOMEM(&segB1);
    param = effPCP_ThunderPolyListGetParam(hData->polyLists[0]);
    SDF_VU0_COPYVECTOR(param, &p15);
    SDF_VU0_COPYVECTOR(&param[1], &segA0);
    SDF_VU0_COPYVECTOR(&param[2], &segA1);
    SDF_VU0_COPYVECTOR(&param[3], &p14);
    effPCP_ThunderPolyListSetColor(hData->polyLists[0], hData->color);
    effPCP_ThunderPolyListDraw(hData->polyLists[0]);
    param = effPCP_ThunderPolyListGetParam(hData->polyLists[1]);
    SDF_VU0_COPYVECTOR(param, &p17);
    SDF_VU0_COPYVECTOR(&param[1], &segB0);
    SDF_VU0_COPYVECTOR(&param[2], &segB1);
    SDF_VU0_COPYVECTOR(&param[3], &p14);
    effPCP_ThunderPolyListDraw(hData->polyLists[1]);
    effPCP_ThunderPolyListSetColor(hData->polyLists[1], hData->color);
    SDF_VU0_LOADVECTOR(&p17);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&p18);
    VU0_LERP_VF10_VF11(0.15f);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.25f);
    SDF_VU0_STOREVECTOR_NOMEM(&segB0);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.7f);
    SDF_VU0_STOREVECTOR_NOMEM(&segB1);
    segB1.f[1] = segB1.f[1] - 100.0f;
    SDF_VU0_LOADVECTOR(&p15);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&p16);
    VU0_LERP_VF10_VF11(0.15f);
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.25f);
    SDF_VU0_STOREVECTOR_NOMEM(&segA0);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&p14);
    VU0_LERP_VF10_VF11(0.7f);
    SDF_VU0_STOREVECTOR_NOMEM(&segA1);
    segA1.f[1] = segA1.f[1] - 100.0f;
    param = effPCP_ThunderPolyListGetParam(hData->polyLists[2]);
    SDF_VU0_COPYVECTOR(param, &p15);
    SDF_VU0_COPYVECTOR(&param[1], &segA0);
    SDF_VU0_COPYVECTOR(&param[2], &segA1);
    SDF_VU0_COPYVECTOR(&param[3], &p14);
    effPCP_ThunderPolyListDraw(hData->polyLists[2]);
    effPCP_ThunderPolyListSetColor(hData->polyLists[2], hData->color);
    param = effPCP_ThunderPolyListGetParam(hData->polyLists[3]);
    SDF_VU0_COPYVECTOR(param, &p17);
    SDF_VU0_COPYVECTOR(&param[1], &segB0);
    SDF_VU0_COPYVECTOR(&param[2], &segB1);
    SDF_VU0_COPYVECTOR(&param[3], &p14);
    effPCP_ThunderPolyListDraw(hData->polyLists[3]);
    effPCP_ThunderPolyListSetColor(hData->polyLists[3], hData->color);
    place.payload[4] = 0.0f;
    place.payload[5] = 0.0f;
    place.payload[6] = 0.0f;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = hData->scale;
    place.payload[0] = p14.f[0];
    place.payload[1] = p14.f[1];
    place.payload[2] = p14.f[2];
    place.payload[3] = 0.0f;
    place.color = hData->color;
    effEveBEDCutSetParams(hData->cut, &place);
    effEveBEDCutDraw(hData->cut);
    hData->frame++;
}

void effPCPBossDengekiSetColor(effPCPBossDengeki_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossThunderbolt_t *effPCPBossThunderboltCreate(effPCPBossThunderbolt_t *pParam)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossThunderbolt_t *work = sdfChipAllocDirect(sizeof(effPCPBossThunderbolt_t));

    memcpy(work, pParam, 0x64);
    work->color = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, work->param, 0x54);
    thunder.count = 4;
    work->pTPolyList = effPCP_ThunderPolyListCreate(&thunder);
    return work;
}

effPCPBossThunderbolt_t *effPCPBossThunderboltCreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossThunderboltCreate(dds3EffectPCPDataGetData(hData, 0, tbldata));
}

effPCPBossThunderbolt_t *effPCPBossThunderboltCopy(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossThunderboltCreate(hData);
}

void effPCPBossThunderboltDelete(effPCPBossThunderbolt_t *hData)
{
    effPCP_ThunderPolyListDelete(hData->pTPolyList);
    sdfChipFreeDirect(hData);
}

void effPCPBossThunderboltDraw(effPCPBossThunderbolt_t *hData)
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
    param = effPCP_ThunderPolyListGetParam(hData->pTPolyList);
    SDF_VU0_LOADVECTOR(&pos);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_STOREVECTOR(param);
    SDF_VU0_LOADVECTOR_B(hData);
    PS2_FLOAT_BITS(bits, 0.33f);
    SDF_VU0_INTERPOLATE(bits);
    SDF_VU0_STOREVECTOR(&param[1]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    PS2_FLOAT_BITS(bits, 0.66f);
    SDF_VU0_INTERPOLATE(bits);
    SDF_VU0_STOREVECTOR(&param[2]);
    SDF_VU0_STOREVECTOR_B(&param[3]);
    effPCP_ThunderPolyListDraw(hData->pTPolyList);
    effPCP_ThunderPolyListSetColor(hData->pTPolyList, hData->color);
    hData->frame++;
}

void effPCPBossThunderboltSetPosition(effPCPBossThunderbolt_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPBossThunderboltSetColor(effPCPBossThunderbolt_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreate(effPCPBossKAGUInvoke_t *pParam)
{
    effPCPBossThunderParam_t thunder;
    void *handle = sdfMemAllocDirect(sizeof(effPCPBossKAGUInvoke_t));
    effPCPBossKAGUInvoke_t *work = sdfMemLock(handle);
    void **list;
    s32 count;
    s32 i;

    work->handle = handle;
    memcpy(work, pParam, 0x3C4);
    work->color = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, &work->param[0x360], 0x54);
    thunder.head.fragmentCount = 1;
    thunder.count = 9;
    count = work->point * 6;
    if (count > 0) {
        i = count;
        list = work->polyLists;
        do {
            *list++ = effPCP_ThunderPolyListCreate(&thunder);
            i--;
        } while (i != 0);
    }
    return work;
}

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossKAGUInvokeCreate(dds3EffectPCPDataGetData(hData, 0, tbldata));
}

effPCPBossKAGUInvoke_t *effPCPBossKAGUInvokeCopy(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossKAGUInvokeCreate(hData);
}

void effPCPBossKAGUInvokeDelete(effPCPBossKAGUInvoke_t *hData)
{
    void **list;
    s32 count;
    s32 i;

    count = hData->point * 6;
    if (count > 0) {
        i = count;
        list = hData->polyLists;
        do {
            effPCP_ThunderPolyListDelete(*list++);
            i--;
        } while (i != 0);
    }
    sdfMemFreeDirect(hData->handle);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDraw);

void effPCPBossKAGUInvokeSetColor(effPCPBossKAGUInvoke_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCreate(effPCPBossKAGUAttack_t *pParam, void *pBEDData)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossPlace_t place;
    effPCPBossKAGUAttack_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGUAttack_t));
    u32 count;
    u32 i;

    memcpy(work, pParam, 0x6C);
    work->frame = 0;
    work->color = 0x80808080;
    if (pBEDData != NULL) {
        effPCPBossBED_t *bed;

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
        bed = effEveBEDCreate(pBEDData);
        work->hBED = bed;
        work->cut = effEveBEDCutCreate(bed, 2, &place);
    }
    memcpy(&thunder.head, work, 0x54);
    thunder.head.fragmentCount = 1;
    thunder.count = 5;
    count = work->groupCount * 8;
    for (i = 0; i < count; i++) {
        work->polyLists[i] = effPCP_ThunderPolyListCreate(&thunder);
    }
    return work;
}

void effPCPBossKAGUAttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGUAttackCreate(firstBlock, secondBlock);
}

effPCPBossKAGUAttack_t *effPCPBossKAGUAttackCopy(effPCPBossKAGUAttack_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossKAGUAttack_t *work = effPCPBossKAGUAttackCreate(hData, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = hData->hBED;

    work->hBED = bed;
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

void effPCPBossKAGUAttackDelete(effPCPBossKAGUAttack_t *hData)
{
    u32 count;
    u32 i;

    effEveBEDCutDelete(hData->cut);
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    count = hData->groupCount * 8;
    for (i = 0; i < count; i++) {
        effPCP_ThunderPolyListDelete(hData->polyLists[i]);
    }
    sdfChipFreeDirect(hData);
}

/* vu0 routine: the five split points of the KAGU arc. */
void effPCPBossKAGUAttackGetSplitPoint(u128 *pPosBuff, const u128 *pPoint1, const u128 *pPoint2,
                                       const u128 *pCenter)
{
    effPCPBossVector_t startDir;
    effPCPBossVector_t endDir;
    effPCPBossVector_t radius;
    effPCPBossVector_t radiusStep;
    f32 startLength;
    f32 endLength;
    f32 delta;
    f32 blend;
    f32 step;
    u32 i;

    SDF_VU0_LOADVECTOR_B(pCenter);
    SDF_VU0_LOADVECTOR(pPoint1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(startLength);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&startDir);
    SDF_VU0_LOADVECTOR(pPoint2);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(endLength);
    delta = endLength - startLength;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&endDir);
    SDF_VU0_COPYVECTOR_B_TO_C();
    blend = 0.0f;
    step = 0.25f;
    radius.f[0] = startLength;
    radius.f[1] = startLength;
    radius.f[2] = startLength;
    radius.f[3] = 0.0f;
    radiusStep.f[0] = delta * step;
    radiusStep.f[1] = radiusStep.f[0];
    radiusStep.f[2] = radiusStep.f[0];
    radiusStep.f[3] = 0.0f;
    for (i = 0; i < 5; i++) {
        SDF_VU0_LOADVECTOR(&startDir);
        SDF_VU0_LOADVECTOR_B(&endDir);
        VU0_LERP_VF10_VF11(blend);
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(&radius);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pPosBuff);
        SDF_VU0_LOADVECTOR(&radiusStep);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B(&radius);
        blend += step;
        pPosBuff++;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDraw);

void effPCPBossKAGUAttackSetColor(effPCPBossKAGUAttack_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCreate(effPCPBossKAGUThunderbolt_t *pParam,
                                                              void *pBP, void *pBED)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossPlace_t place;
    effPCPBossKAGUThunderbolt_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGUThunderbolt_t));

    memcpy(work, pParam, 0x100);
    work->color = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, work->unk010, 0x54);
    thunder.count = 4;
    work->pTPolyList = effPCP_ThunderPolyListCreate(&thunder);
    if (pBP != NULL) {
        work->pCrack = effPCPCrackCreate((effPCPCrackParam_t *)&work->unk010[0x54], pBP);
    }
    if (pBED != NULL) {
        effPCPBossBED_t *bed;

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
        bed = effEveBEDCreate(pBED);
        work->hBED = bed;
        work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
    }
    return work;
}

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
effPCPBossKAGUThunderbolt_t *effPCPBossKAGUThunderboltCopy(effPCPBossKAGUThunderbolt_t *hData,
                                                           s32 bPos, s32 tbldata)
{
    effPCPBossKAGUThunderbolt_t *work = effPCPBossKAGUThunderboltCreate(hData, NULL, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed;
    effPCPCrack_t *crack;

    crack = effPCPCrackCopy(hData->pCrack);
    bed = hData->hBED;
    work->hBED = bed;
    work->pCrack = crack;
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
    work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUThunderboltCopy);
#endif

void effPCPBossKAGUThunderboltDelete(effPCPBossKAGUThunderbolt_t *hData)
{
    effEveBEDCutDelete(hData->hBEDCut);
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    effPCP_ThunderPolyListDelete(hData->pTPolyList);
    effPCPCrackDelete(hData->pCrack);
    sdfChipFreeDirect(hData);
}

void effPCPBossKAGUThunderboltDrawSub(effPCPBossKAGUThunderbolt_t *pPCPBossKAGUThunderbolt, const f32 *pConvPos)
{
    effPCPBossPlace_t place;
    effPCPBossVector_t tip;
    effPCPBossVector_t mark;
    effCrackModel_t *handle;
    effPCPBossUnit_t *model;
    effPCPBossUnit_t *found;
    u128 *list;
    s32 limit;
    s32 frame;
    s32 fadeIn;
    s32 fadeOut;
    s32 rest;
    s32 id;
    s32 i;
    f32 blend;
    u32 color;
    u32 bits;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = *nbGetMainProcessData();
        for (i = 0; i < 0x20; i++) {
            if ((model->flags & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x125 || id == 0x108) {
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
    limit = pPCPBossKAGUThunderbolt->limit;
    frame = pPCPBossKAGUThunderbolt->frame;
    fadeIn = pPCPBossKAGUThunderbolt->fadeIn;
    fadeOut = pPCPBossKAGUThunderbolt->fadeOut;
    effPCPCrackSetPosition(pPCPBossKAGUThunderbolt->pCrack, pPCPBossKAGUThunderbolt);
    effPCPCrackSetColor(pPCPBossKAGUThunderbolt->pCrack, pPCPBossKAGUThunderbolt->color);
    effPCPCrackDraw(pPCPBossKAGUThunderbolt->pCrack);
    handle = dds3EffectPartGetHandle(pPCPBossKAGUThunderbolt->pCrack->part);
    sdfModelGetMarkPos(handle->inner, 0);
    SDF_VU0_STOREVECTOR(&mark);
    if (frame < limit) {
        rest = limit - frame;
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else if (!(fadeOut < rest) && fadeOut != 0) {
            blend = (f32)rest / (f32)fadeOut;
        } else {
            blend = 1.0f;
        }
        color = effColorBlend(pPCPBossKAGUThunderbolt->color & 0xFFFFFF, pPCPBossKAGUThunderbolt->color, blend);
        place.payload[4] = 0.0f;
        place.payload[5] = 0.0f;
        place.payload[6] = 0.0f;
        place.payload[7] = 1.0f;
        place.payload[8] = 100.0f;
        place.payload[9] = 100.0f;
        place.payload[10] = pPCPBossKAGUThunderbolt->scale;
        place.payload[0] = mark.f[0];
        place.payload[1] = mark.f[1];
        place.payload[2] = mark.f[2];
        place.color = color;
        effEveBEDCutSetParams(pPCPBossKAGUThunderbolt->hBEDCut, &place);
        effEveBEDCutDraw(pPCPBossKAGUThunderbolt->hBEDCut);
        SDF_VU0_LOADVECTOR((u8 *)model + 0x20);
        SDF_VU0_LOADVECTOR_B(pConvPos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(&tip);
        list = effPCP_ThunderPolyListGetParam(pPCPBossKAGUThunderbolt->pTPolyList);
        SDF_VU0_LOADVECTOR(&tip);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_STOREVECTOR(list);
        SDF_VU0_LOADVECTOR_B(&mark);
        PS2_FLOAT_BITS(bits, 0.33f);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(&list[1]);
        SDF_VU0_COPYVECTOR_C_TO_A();
        PS2_FLOAT_BITS(bits, 0.66f);
        SDF_VU0_INTERPOLATE(bits);
        SDF_VU0_STOREVECTOR(&list[2]);
        SDF_VU0_STOREVECTOR_B(&list[3]);
        effPCP_ThunderPolyListSetColor(pPCPBossKAGUThunderbolt->pTPolyList, color);
        effPCP_ThunderPolyListDraw(pPCPBossKAGUThunderbolt->pTPolyList);
    }
    pPCPBossKAGUThunderbolt->frame++;
}

void effPCPBossKAGUThunderboltDraw(effPCPBossKAGUThunderbolt_t *hData)
{
    effPCPBossKAGUThunderboltDrawSub(hData, effPCPBossKAGUDrawParam);
}

void effPCPBossKAGUThunderboltSetPosition(effPCPBossKAGUThunderbolt_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPBossKAGUThunderboltSetColor(effPCPBossKAGUThunderbolt_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCreate(effPCPBossKAGU2Attack_t *pParam, void *pBEDData)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossPlace_t place;
    effPCPBossKAGU2Attack_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGU2Attack_t));
    u32 count;
    u32 i;

    memcpy(work, pParam, 0x6C);
    work->unkB4 = 0;
    work->color = 0x80808080;
    if (pBEDData != NULL) {
        effPCPBossBED_t *bed;

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
        bed = effEveBEDCreate(pBEDData);
        work->hBED = bed;
        work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
    }
    memcpy(&thunder.head, work, 0x54);
    thunder.head.fragmentCount = 1;
    thunder.count = 5;
    count = work->groupCount * 4;
    for (i = 0; i < count; i++) {
        work->polyLists[i] = effPCP_ThunderPolyListCreate(&thunder);
    }
    return work;
}

void effPCPBossKAGU2AttackCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPBossKAGU2AttackCreate(firstBlock, secondBlock);
}

effPCPBossKAGU2Attack_t *effPCPBossKAGU2AttackCopy(effPCPBossKAGU2Attack_t *hData, s32 bPos,
                                                   s32 tbldata)
{
    effPCPBossKAGU2Attack_t *work = effPCPBossKAGU2AttackCreate(hData, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = hData->hBED;

    work->hBED = bed;
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
    work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCPBossKAGU2AttackDelete(effPCPBossKAGU2Attack_t *hData)
{
    u32 count;
    u32 i;

    effEveBEDCutDelete(hData->hBEDCut);
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    count = hData->groupCount * 4;
    for (i = 0; i < count; i++) {
        effPCP_ThunderPolyListDelete(hData->polyLists[i]);
    }
    sdfChipFreeDirect(hData);
}

/* vu0 routine: the five split points of the KAGU2 arc. */
void effPCPBossKAGU2AttackGetSplitPoint(u128 *pPosBuff, const u128 *pPoint1, const effPCPBossVector_t *pPoint2,
                                        const u128 *pCenter, f32 rand, f32 rotate)
{
    effPCPBossVector_t startDir;
    effPCPBossVector_t endDir;
    effPCPBossVector_t radius;
    effPCPBossVector_t radiusStep;
    effPCPBossVector_t axis;
    u128 *point;
    f32 endLength;
    f32 baseRadius;
    f32 delta;
    f32 blend;
    f32 step;
    f32 r;
    u32 narrowBits;
    u32 i;

    SDF_VU0_LOADVECTOR_B(pCenter);
    SDF_VU0_LOADVECTOR(pPoint1);
    SDF_VU0_SUBVECTOR();
    baseRadius = 800.0f;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&startDir);
    SDF_VU0_LOADVECTOR(pPoint2);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(endLength);
    delta = endLength - baseRadius;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&endDir);
    SDF_VU0_COPYVECTOR_B_TO_C();
    blend = 0.0f;
    step = 0.25f;
    point = pPosBuff;
    radius.f[0] = baseRadius;
    radius.f[1] = baseRadius;
    radius.f[2] = baseRadius;
    radius.f[3] = 0.0f;
    radiusStep.f[0] = delta * step;
    radiusStep.f[1] = radiusStep.f[0];
    radiusStep.f[2] = radiusStep.f[0];
    radiusStep.f[3] = 0.0f;
    SDF_VU0_LOADVECTOR_B(pPoint2);
    SDF_VU0_LOADVECTOR(pPoint1);
    SDF_VU0_SETVECTOR_X(pPoint2->f[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&axis);
    r = dds3GetRandFloatA(dds3EffectBase);
    rotate += (1.0f - (r * rand + (1.0f - rand))) * 0.61086511611938477f;
    SetRotateMatrixAxis(rotate, &axis);
    i = 0;
    narrowBits = 0x3F400000;
    while (i < 5) {
        SDF_VU0_LOADVECTOR(&startDir);
        SDF_VU0_LOADVECTOR_B(&endDir);
        VU0_LERP_VF10_VF11(blend);
        SDF_VU0_NORMALIZE();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_LOADVECTOR_B(&radius);
        if (i == 2 && rotate != 0.0f) {
            SDF_VU0_SCALEVECTOR_B_BITS(narrowBits);
        }
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(point);
        SDF_VU0_LOADVECTOR_B(&radius);
        SDF_VU0_LOADVECTOR(&radiusStep);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B(&radius);
        blend += step;
        point++;
        i++;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDraw);

void effPCPBossKAGU2AttackSetColor(effPCPBossKAGU2Attack_t *hData, u32 abgr)
{
    hData->color = abgr;
}

void effPCPBossKAGU2ThunderboltDraw(effPCPBossKAGUThunderbolt_t *hData)
{
    effPCPBossKAGUThunderboltDrawSub(hData, effPCPBossKAGU2DrawParam);
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreate(void *pParam)
{
    effPCPBossKAGU2MUHENK_t *work = sdfChipAllocDirect(sizeof(effPCPBossKAGU2MUHENK_t));

    work->param = 0;
    work->color = 0x80808080;
    return work;
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossKAGU2MUHENKCreate(dds3EffectPCPDataGetData(hData, 0, tbldata));
}

effPCPBossKAGU2MUHENK_t *effPCPBossKAGU2MUHENKCopy(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossKAGU2MUHENKCreate(hData);
}

void effPCPBossKAGU2MUHENKDelete(effPCPBossKAGU2MUHENK_t *pPCPBossKAGU2MUHENK)
{
    sdfChipFreeDirect(pPCPBossKAGU2MUHENK);
}

/* vu0 routine: the seven MUHENK mark gizmos. */
void effPCPBossKAGU2MUHENKDraw(effPCPBossKAGU2MUHENK_t *pPCPBossKAGU2MUHENK)
{
    effPCPBossVector_t pos;
    effPCPBossVector_t lineEnd;
    effPCPBossVector_t down;
    effPCPBossUnit_t *model;
    s32 id;
    u32 i;
    effPCPBossUnit_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = *nbGetMainProcessData();
        for (i = 0; i < 0x20; i++) {
            if ((model->flags & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x125 || id == 0x108) {
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
    for (i = 0; i < 7; i++) {
        effBTLModelGetMarkPosition2(model, i + 0x14);
        SDF_VU0_STOREVECTOR_NOMEM(&pos);
        effDebugPointDraw(&pos);
        sdfModelGetMarkPosMatrix(model->hunit->owner->inner, i + 0x14);
        down.f[0] = 0.0f;
        down.f[1] = -1.0f;
        down.f[2] = 0.0f;
        SDF_VU0_LOADVECTOR(&down);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&down);
        lineEnd.f[0] = pos.f[0] + down.f[0] * 200.0f;
        lineEnd.f[1] = pos.f[1] + down.f[1] * 200.0f;
        lineEnd.f[2] = pos.f[2] + down.f[2] * 200.0f;
        effDebugLineDraw(&pos, &lineEnd);
    }
    pPCPBossKAGU2MUHENK->param++;
}

void effPCPBossKAGU2MUHENKSetColor(effPCPBossKAGU2MUHENK_t *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(pParam, pPB);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = pParam->rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = pParam->angularSpeed;
    }
    return work;
}

void effPCPCrack2CreatePCF(void *data) {
    effPCPCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(data, 0);
    effectParams = dds3EffectPCPDataGetData(data, 1);
    effPCPCrack2Create(bossParams, effectParams);
}

effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = src->pPart[groupIndex].rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = src->pPart[groupIndex].rotate_add;
    }
    return work;
}

void effPCPCrack2Delete(effPCPCrack_t *hData)
{
    effPCPCrackDelete(hData);
}

void effPCPCrack2Draw(effPCPCrack_t *hData)
{
    effPCPCrackDraw(hData);
}

void effPCPCrack2SetPosition(effPCPCrack_t *hData, const void *pPos)
{
    effPCPCrackSetPosition(hData, pPos);
}

void effPCPCrack2SetColor(effPCPCrack_t *hData, u32 abgr)
{
    effPCPCrackSetColor(hData, abgr);
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam)
{
    effPCPBossThunderParam_t thunder;
    effPCPBossMETATOBeam_t *work = sdfChipAllocDirect(sizeof(effPCPBossMETATOBeam_t));

    memcpy(work, pParam, 0x70);
    work->abgr = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.head, work->unk10, 0x54);
    thunder.count = 4;
    work->polyLists[0] = effPCP_ThunderPolyListCreate(&thunder);
    work->polyLists[1] = effPCP_ThunderPolyListCreate(&thunder);
    return work;
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(dds3EffectPCPDataGetData(hData, 0, tbldata));
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(hData);
}

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData)
{
    effPCP_ThunderPolyListDelete(hData->polyLists[1]);
    effPCP_ThunderPolyListDelete(hData->polyLists[0]);
    sdfChipFreeDirect(hData);
}

/* NON_MATCHING: 7 of 151 words differ (first at +0x170). */
#ifdef NON_MATCHING
void effPCPBossMETATOBeamDraw(effPCPBossMETATOBeam_t *hData)
{
    effPCPBossVector_t pos;
    effPCPBossUnit_t *model;
    u128 *param;
    s32 limit = hData->limit;
    s32 frame = hData->frame;
    s32 fadeIn = hData->fadeIn;
    s32 fadeOut = hData->fadeOut;
    s32 rest;
    s32 id;
    s32 i;
    u32 j;
    f32 blend;
    u32 color;
    u32 blendInner;
    u32 blendOuter;
    effPCPBossUnit_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = *nbGetMainProcessData();
        for (i = 0; i < 0x20; i++) {
            if ((model->flags & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x156 || id == 0xC1) {
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
    if (frame < limit) {
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else {
            rest = limit - frame;
            if (!(fadeOut < rest) && fadeOut != 0) {
                blend = (f32)rest / (f32)fadeOut;
            } else {
                blend = 1.0f;
            }
        }
        blendInner = 0x3EA8F5C2;
        blendOuter = 0x3F28F5C2;
        color = effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, blend);
        for (j = 0; j < 2; j++) {
            effBTLModelGetMarkPosition2(model, 0x14 + j);
            SDF_VU0_STOREVECTOR(&pos);
            param = effPCP_ThunderPolyListGetParam(hData->polyLists[j]);
            SDF_VU0_LOADVECTOR(&pos);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_STOREVECTOR(param);
            SDF_VU0_LOADVECTOR_B(hData);
            SDF_VU0_INTERPOLATE(blendInner);
            SDF_VU0_STOREVECTOR(&param[1]);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_INTERPOLATE(blendOuter);
            SDF_VU0_STOREVECTOR(&param[2]);
            SDF_VU0_STOREVECTOR_B(&param[3]);
            effPCP_ThunderPolyListSetColor(hData->polyLists[j], color);
            effPCP_ThunderPolyListDraw(hData->polyLists[j]);
        }
    }
    hData->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossMETATOBeamDraw);
#endif

void effPCPBossMETATOBeamSetPosition(effPCPBossMETATOBeam_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPBossMETATOBeamSetColor(effPCPBossMETATOBeam_t *hData, u32 abgr)
{
    hData->abgr = abgr;
}

void effPCPScreenCrackModelInit(effPCPScreenCrackModel_t *pPCPScrCrackModel) {
}

effPCPScreenCrackModel_t *effPCPScreenCrackModelCreate(effPCPScreenCrackModel_t *pParam, void *pPB)
{
    effPCPScreenCrackModel_t *work = sdfChipAllocDirect(0x10);

    work->param = pParam->param;
    work->abgr = 0x80808080;
    work->frame = 0;
    if (pPB != NULL) {
        work->hModel = dds3EffectPartCreate(3, pPB);
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

effPCPScreenCrackModel_t *effPCPScreenCrackModelCopy(effPCPScreenCrackModel_t *hData, s32 bPos,
                                                     s32 tbldata)
{
    effPCPScreenCrackModel_t *work;

    work = effPCPScreenCrackModelCreate(work, NULL);
    work->hModel = dds3EffectPartCopy(hData->hModel);
    return work;
}

void effPCPScreenCrackModelDelete(effPCPScreenCrackModel_t *hData)
{
    dds3EffectPartDelete(hData->hModel);
    sdfChipFreeDirect(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPScreenCrackModelDraw);

void effPCPScreenCrackModelSetColor(effPCPScreenCrackModel_t *hData, u32 abgr)
{
    hData->abgr = abgr;
}

effPCPMuzzleFlash_tag *effPCPMuzzleFlashCreate(effPCPMuzzleFlashParam_t *pParam, void *pP2A)
{
    effPCPMuzzleFlash_tag *work = sdfChipAllocDirect(0x64);

    work->param = *pParam;
    work->scale = 1.0f;
    work->frame = 0;
    SDF_VU0_STOREIDENTITY(work);
    work->hBillboard = dds3BillboardCreate(1, pP2A);
    return work;
}

void effPCPMuzzleFlashCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPMuzzleFlashCreate(firstBlock, secondBlock);
}

effPCPMuzzleFlash_tag *effPCPMuzzleFlashCopy(effPCPMuzzleFlash_tag *hData, s32 bPos, s32 tbldata)
{
    effPCPMuzzleFlash_tag *work = sdfChipAllocDirect(0x64);

    work->param = hData->param;
    work->scale = 1.0f;
    work->frame = 0;
    SDF_VU0_STOREIDENTITY(work);
    work->hBillboard = dds3BillboardCopy(hData->hBillboard);
    return work;
}

void effPCPMuzzleFlashDelete(effPCPMuzzleFlash_tag *hData)
{
    dds3BillboardDelete(hData->hBillboard);
    sdfChipFreeDirect(hData);
}

void effPCPMuzzleFlashDraw(effPCPMuzzleFlash_tag *hData)
{
    u128 direction;
    s32 animFrame = dds3BillboardGetAnimFrame(hData->hBillboard);
    s32 frame = hData->frame;
    void *model;
    void *position;
    f32 rot;
    f32 sc;
    f32 scale;

    if (frame < animFrame) {
        dds3BillboardSetAnimKey2(hData->hBillboard, frame);
        if (effBTLSkillCheckRun() != 0) {
            model = effBTLSkillGetInvokeFormation();
            SDF_VU0_LOADVECTOR((u8 *)model + 0x30);
            SDF_VU0_LOADVECTOR_B(D_00533DE0);
            Mult();
            ToMatrix();
            ReverseMatrix();
            SDF_VU0_COPYMATRIX_A_TO_B();
            SDF_VU0_LOADMATRIX(hData);
            MultMatrixNoArg();
        } else {
            SDF_VU0_LOADMATRIX(hData);
        }
        SDF_VU0_LOADVECTOR(D_00533DD0);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&direction);
        position = hData->pos;
        rot = dds3BillboardGetDirectionRotate(position, &direction);
        sc = dds3BillboardGetDirectionScale(position, &direction);
        if (sc < 0.5f) {
            sc = 0.5f;
        }
        scale = hData->scale;
        sc = sc * scale;
        dds3BillboardSetScale(hData->hBillboard, sc * *(f32 *)&hData->param.scale_w,
                              *(f32 *)&hData->param.scale_h * scale);
        dds3BillboardSetRotate(hData->hBillboard, rot);
        dds3BillboardSetPosition(hData->hBillboard, position);
        dds3BillboardDraw(hData->hBillboard);
        hData->frame++;
    }
}

void effPCPMuzzleFlashSetColor(effPCPMuzzleFlash_tag *hData, u32 abgr)
{
    dds3BillboardSetRGBA(hData->hBillboard, abgr);
}

void effPCPMuzzleFlashSetPosition(effPCPMuzzleFlash_tag *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData->pos, pPos);
}

void effPCPMuzzleFlashSetScale(effPCPMuzzleFlash_tag *hData, f32 scale)
{
    hData->scale = 1.0f;
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPMuzzleFlashSetRotateMatrix(effPCPMuzzleFlash_tag *hData, const void *pRot)
{
    SDF_VU0_LOADMATRIX(pRot);
    SDF_VU0_STOREMATRIX(hData);
}

/* vu0 routine: the DANTE lightning bolt's work and its draw process. */
effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Create(const effPCPDANTE_Thunder_Param_t *pParam)
{
    effPCPDANTE_Thunder_tag *work = sdfChipAllocDirect(sizeof(effPCPDANTE_Thunder_tag));
    u128 *param;
    s32 process;

    memcpy(&work->params, pParam, sizeof(work->params));
    work->color = 0x80808080;
    SetRotateMatrixZ(-1.5707963f);
    SDF_VU0_STOREMATRIX(work->matrix);
    SDF_VU0_STOREIDENTITYVECTOR(work);
    work->poly = effPCP_ThunderPolyCreate(&work->params);
    param = effPCP_ThunderPolyGetParam(work->poly);
    SDF_VU0_LOADVECTOR(param);
    param++;
    SDF_VU0_LOADVECTOR_B(param);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(work->scale);
    work->drawRequest = 0;
    work->frame = 0;
    process = dds3StartProcess(D_00533DF0, 0x2B0A, 0, 0, effPCPDANTE_Thunder_ProcessDraw, 0, 0);
    work->process = process;
    dds3SetProcessData(process, work);
    return work;
}

s32 effPCPDANTE_Thunder_ProcessDraw(void)
{
    effPCPDANTE_Thunder_tag *work = dds3GetProcessData();
    u128 matrix[4];
    u128 position;
    void *model;

    if (work->drawRequest != 0) {
        if (effBTLSkillCheckRun() != 0) {
            model = effBTLSkillGetInvokeFormation();
            effBTLModelGetMarkMatrix2(model, work->params.mark);
            SDF_VU0_STOREMATRIX_MEMOPS(matrix[0], matrix[1], matrix[2], matrix[3]);
            effPCPDANTE_Thunder_SetRotateMatrix(work, matrix);
            effBTLModelGetMarkPosition2(model, work->params.mark);
            SDF_VU0_STOREVECTOR_NOMEM(&position);
            effPCPDANTE_Thunder_SetPosition(work, &position);
        }
        effPCPDANTE_Thunder_Draw(work);
        work->drawRequest = 0;
    }
    return 0;
}

effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_CreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPDANTE_Thunder_Create(dds3EffectPCPDataGetData(hData, 0, tbldata));
}

void effPCPDANTE_Thunder_Delete(effPCPDANTE_Thunder_tag *hData)
{
    dds3KillProcess(hData->process, 0);
    effPCP_ThunderPolyDelete(hData->poly);
    sdfChipFreeDirect(hData);
}

effPCPDANTE_Thunder_tag *effPCPDANTE_Thunder_Copy(effPCPDANTE_Thunder_tag *hData, s32 bPos, s32 tbldata)
{
    return effPCPDANTE_Thunder_Create(&hData->params);
}

void effPCPDANTE_Thunder_DrawSub(effPCPDANTE_Thunder_tag *hData)
{
    hData->drawRequest = 1;
}

void effPCPDANTE_Thunder_Draw(effPCPDANTE_Thunder_tag *hData)
{
    s32 limit = hData->params.limit;
    s32 frame = hData->frame;
    s32 fadeIn;
    s32 fadeOut;
    u128 *param;
    s32 rest;
    f32 blend;
    u32 color;

    if (frame < limit) {
        fadeIn = hData->params.fadeIn;
        fadeOut = hData->params.fadeOut;
        param = effPCP_ThunderPolyGetParam(hData->poly);
        SDF_VU0_LOADMATRIX(hData->matrix);
        SDF_VU0_LOADVECTOR_B(D_00533E10);
        SDF_VU0_APPLYMATRIX_B();
        SDF_VU0_SCALEVECTOR_B(hData->scale);
        SDF_VU0_LOADVECTOR(hData);
        SDF_VU0_STOREVECTOR(param);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&param[1]);
        rest = limit - frame;
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else if (!(fadeOut < rest) && fadeOut != 0) {
            blend = (f32)rest / (f32)fadeOut;
        } else {
            blend = 1.0f;
        }
        color = effColorBlend(hData->color & 0xFFFFFF, hData->color, blend);
        effPCP_ThunderPolySetColor(hData->poly, color);
        effPCP_ThunderPolyDraw(hData->poly);
        hData->frame++;
    }
}

void effPCPDANTE_Thunder_SetColor(effPCPDANTE_Thunder_tag *hData, u32 abgr)
{
    hData->color = abgr;
}

void effPCPDANTE_Thunder_SetPosition(effPCPDANTE_Thunder_tag *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPDANTE_Thunder_SetRotateMatrix(effPCPDANTE_Thunder_tag *hData, const void *pRot)
{
    SDF_VU0_LOADMATRIX(pRot);
    SDF_VU0_STOREMATRIX(hData->matrix);
}

/* NON_MATCHING: 19 of 90 words differ (first at +0x4). */
#ifdef NON_MATCHING
effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Create(void *pParam, void *pBED)
{
    const effPCPDANTE_Damage_Param_t *param = pParam;
    effPCPDANTE_Damage_tag *work = sdfChipAllocZeroDirect(sizeof(effPCPDANTE_Damage_tag));
    effPCPBossUnit_t *unit;
    u32 bits;
    s32 delay;
    u8 *slot;
    u32 count;
    u32 i;
    u32 swaps;

    memcpy(work, pParam, 0x10);
    work->unk030 = 0;
    work->color = 0x80808080;
    if (pBED != NULL) {
        work->unk034 = 1;
        work->bed = effEveBEDCreate(pBED);
    }
    delay = 1;
    unit = *nbGetMainProcessData();
    bits = effBTLSkillGetTargetBit();
    count = 0;
    slot = work->unk010;
    for (i = 0; i < 32; i++, unit++) {
        if (unit->flags & 1) {
            if (bits & (1 << i)) {
                *slot++ = delay;
                count++;
                delay += param->wait;
            }
        }
    }
    swaps = count * 2;
    for (i = 0; i < swaps; i++) {
        u32 a = dds3GetRandIntA(NULL, count);
        u32 b = dds3GetRandIntA(NULL, count);
        u8 held = work->unk010[a];

        work->unk010[a] = work->unk010[b];
        work->unk010[b] = held;
    }
    return work;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Create);
#endif

void effPCPDANTE_Damage_CreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    effPCPDANTE_Damage_Create(firstBlock, secondBlock);
}

effPCPDANTE_Damage_tag *effPCPDANTE_Damage_Copy(effPCPDANTE_Damage_tag *hData, s32 bPos, s32 tbldata)
{
    effPCPDANTE_Damage_tag *work = effPCPDANTE_Damage_Create(hData, NULL);

    work->unk034 = 0;
    work->bed = hData->bed;
    return work;
}

void effPCPDANTE_Damage_Delete(effPCPDANTE_Damage_tag *hData)
{
    void **cut;
    u32 i;

    cut = hData->cuts;
    for (i = 0; i < 0x20; i++) {
        if (*cut != NULL) {
            effEveBEDCutDelete(*cut);
        }
        cut++;
    }
    if (hData->unk034 != 0) {
        effEveBEDDelete(hData->bed);
    }
    sdfChipFreeDirect(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_Damage_Run);

void effPCPDANTE_Damage_SetColor(effPCPDANTE_Damage_tag *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCP_PBCamera_tag *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *pParam, void *pPB)
{
    effPCP_PBCamera_tag *work = sdfChipAllocDirect(0x60);
    s32 value = pParam->life;

    work->unk5C = 0;
    work->param = value;
    work->unk58 = 0;
    if (pPB != NULL) {
        work->part = dds3EffectPartCreate(3, pPB);
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

effPCP_PBCamera_tag *effPCP_PBCamera_Copy(effPCP_PBCamera_tag *hData, s32 bPos, s32 tbldata)
{
    effPCP_PBCamera_tag *work = effPCP_PBCamera_Create((effPCP_PBCamera_Param_t *)&hData->param, NULL);

    work->part = dds3EffectPartCopy(hData->part);
    return work;
}

void effPCP_PBCamera_Delete(effPCP_PBCamera_tag *hData)
{
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_PBCamera_Run);

void effPCP_PBCamera_SetPosition(effPCP_PBCamera_tag *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData->position, pPos);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_PBCamera_SetRotateMatrix(effPCP_PBCamera_tag *hData, const void *pMatrix)
{
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STOREMATRIX(hData);
}

effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Create(effPCPDANTE_ShowTime_Param_t *pParam,
                                                    void *pTMX0, void *pTMX1)
{
    effPCPDANTE_ShowTime_tag *work = sdfChipAllocZeroDirect(0xD8);

    work->param = pParam->reserve;
    work->frame = 0;
    if (pTMX0 != NULL && pTMX1 != NULL) {
        work->textures[0] = sdfTexSet(pTMX0);
        work->textures[1] = sdfTexSet(pTMX1);
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

effPCPDANTE_ShowTime_tag *effPCPDANTE_ShowTime_Copy(effPCPDANTE_ShowTime_tag *hData, s32 bPos, s32 tbldata)
{
    effPCPDANTE_ShowTime_tag *work =
        effPCPDANTE_ShowTime_Create((effPCPDANTE_ShowTime_Param_t *)&hData->param, NULL, NULL);

    work->textures[0] = hData->textures[0];
    work->textures[1] = hData->textures[1];
    work->texLoaded = 0;
    return work;
}

void effPCPDANTE_ShowTime_Delete(effPCPDANTE_ShowTime_tag *hData)
{
    if (hData->texLoaded != 0) {
        sdfTexFreeDelayed(hData->textures[1]);
        sdfTexFreeDelayed(hData->textures[0]);
    }
    sdfChipFreeDirect(hData);
}

void effPCPDANTE_Sprite_Draw(s32 x, s32 y, f32 scale, u32 color, effPCPDANTE_UV_tag *uv, void *tex)
{
    s32 sw;
    s32 sh;
    u32 w;
    u32 h;

    if (scale == 1.0f) {
        sw = uv->w;
        sh = uv->h;
    } else {
        w = uv->w;
        sw = (f32)w * scale;
        h = uv->h;
        sh = (f32)h * scale;
        x -= (sw - w) / 2;
        y -= (sh - h) / 2;
    }
    nbPnlSprite(x, y, sw, sh, uv->u, uv->v, uv->w, uv->h, color, tex);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Bingo_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_AreYouReady_Draw);

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_ShowTime_Draw);

/* NON_MATCHING: 4 of 75 words differ (first at +0x34). */
#ifdef NON_MATCHING
void effPCPDANTE_ShowTime_Draw(effPCPDANTE_ShowTime_tag *hData)
{
    s32 frame = hData->frame;

    if (frame == 0) {
        nbPanelFlashInit(hData->panels[0], 0x43BBFF, 0x12A2F5, 0xA0, 0.45f, 0.35f, 0x5C);
        nbPanelFlashInit(hData->panels[1], 0x3664FF, 0x3CF0, -0x8C, 0.55f, 0.45f, -0x3C);
        nbPanelFlashInit(hData->panels[2], 0x31109F, 0x8D64C8, 0, 1.5f, 2.0f, 0x14);
    }
    if (frame >= 0) {
        nbPanelFlashDraw(hData->panels[0], 0x80808080);
        effPCPDANTE_ShowTime_Bingo_Draw(hData);
    }
    if (frame >= 0x16) {
        nbPanelFlashDraw(hData->panels[1], 0x80808080);
        effPCPDANTE_ShowTime_AreYouReady_Draw(hData);
    }
    if (frame >= 0x5C) {
        nbPanelFlashDraw(hData->panels[2], 0x80808080);
        effPCPDANTE_ShowTime_ShowTime_Draw(hData);
    }
    hData->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Draw);
#endif

void effPCPDANTE_ShowTime_SetPosition(effPCPDANTE_ShowTime_tag *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData, pPos);
}

/* NON_MATCHING: 30 of 63 words differ (first at +0x80). */
#ifdef NON_MATCHING
effPCP_BED_tag *effPCP_BED_Create(effPCP_BED_Param_t *pParam, void *pBED)
{
    effPCP_BED_tag *work = sdfChipAllocDirect(0x6C);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed;

    work->param = *pParam;
    work->frame = 0;
    work->abgr = 0x80808080;
    SDF_VU0_STOREIDENTITYVECTOR(work->pos);
    SDF_VU0_STOREIDENTITY(work);
    if (pBED != NULL) {
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
        place.color = work->abgr;
        bed = effEveBEDCreate(pBED);
        work->hBED = bed;
        work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
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

effPCP_BED_tag *effPCP_BED_Copy(effPCP_BED_tag *hData, s32 bPos, s32 tbldata)
{
    effPCP_BED_tag *work = effPCP_BED_Create(&hData->param, NULL);
    effPCPBossPlace_t place;
    effPCPBossBED_t *bed = hData->hBED;

    work->hBED = bed;
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
    work->hBEDCut = effEveBEDCutCreate(bed, 2, &place);
    return work;
}

void effPCP_BED_Delete(effPCP_BED_tag *hData)
{
    effEveBEDCutDelete(hData->hBEDCut);
    if (hData->hBED->active == 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfChipFreeDirect(hData);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCP_BED_MatrixToQuaternion);

void effPCP_BED_Draw(effPCP_BED_tag *hData)
{
    effPCPBossPlace_t place;
    s32 frame = hData->frame;
    s32 limit = hData->param.life;
    s32 fadeIn = hData->param.fade_in;
    s32 fadeOut = hData->param.fade_out;
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
    color = effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, blend);
    SDF_VU0_COPYVECTOR(place.payload, hData->pos);
    effPCP_BED_MatrixToQuaternion(hData);
    SDF_VU0_STOREVECTOR(&place.payload[4]);
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = color;
    effEveBEDCutSetParams(hData->hBEDCut, &place);
    effEveBEDCutDraw(hData->hBEDCut);
    hData->frame++;
}

void effPCP_BED_SetPosition(effPCP_BED_tag *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(hData->pos, pPos);
}

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCP_BED_SetRotateMatrix(effPCP_BED_tag *hData, const void *pMatrix)
{
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STOREMATRIX(hData);
}

void effPCP_BED_SetColor(effPCP_BED_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

effPCP_HBED2_tag *effPCP_HBED2_Create(effPCP_HBED2_Param_t *pParam, void *pPBData, void *pBEDData)
{
    effPCP_HBED2_tag *work = sdfChipAllocDirect(0x20);

    work->param = *pParam;
    if (pPBData != NULL && pBEDData != NULL) {
        work->pHelperBED = effPCPHelperBEDCreate(work, pPBData, pBEDData);
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

effPCP_HBED2_tag *effPCP_HBED2_Copy(effPCP_HBED2_tag *hData, s32 bPos, s32 tbldata)
{
    effPCP_HBED2_tag *work = effPCP_HBED2_Create((effPCP_HBED2_Param_t *)hData, NULL, NULL);

    work->pHelperBED = effPCPHelperBEDCopy(hData->pHelperBED);
    return work;
}

void effPCP_HBED2_Delete(effPCP_HBED2_tag *hData)
{
    effPCPHelperBEDDelete(hData->pHelperBED);
    sdfChipFreeDirect(hData);
}

void effPCP_HBED2_Draw(effPCP_HBED2_tag *hData)
{
    effPCPHelperBEDDraw(hData->pHelperBED);
}

void effPCP_HBED2_SetPosition(effPCP_HBED2_tag *hData, const void *pPos)
{
    effPCPHelperBEDSetPosition(hData->pHelperBED, pPos);
}

void effPCP_HBED2_SetRotateMatrix(effPCP_HBED2_tag *hData, const void *pMatrix)
{
    void *model = effPCPHelperBEDGetModel(hData->pHelperBED);

    effPCP_BED_MatrixToQuaternion(pMatrix);
    mdlSetRotate(model);
}

void effPCP_HBED2_SetColor(effPCP_HBED2_tag *hData, u32 abgr)
{
    effPCPHelperBEDSetColor(hData->pHelperBED, abgr);
}

void effPCP_HBED2_SetScale(effPCP_HBED2_tag *hData, f32 scale)
{
    effPCPHelperBEDSetScale(hData->pHelperBED, scale);
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

void effPadVibeFadeOUT(u32 mode, u8 power, s32 frame)
{
    if (mode < 2) {
        D_0054F200[mode] = frame;
        D_0054F210[mode] = power;
        D_0054F218[mode] = 0;
        D_0054F208[mode] = frame;
    }
}

void effPadVibeFadeIN(u32 mode, u8 power, s32 frame)
{
    if (mode < 2) {
        D_0054F200[mode] = frame;
        D_0054F210[mode] = power;
        D_0054F218[mode] = 1;
        D_0054F208[mode] = frame;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", dds3EffectBlurDrawQuad);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DA0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DB0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DC0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DD0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DE0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533DF0);

INCLUDE_RODATA(const s32, "effect/effPCPBoss", D_00533E10);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossTexHandle);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1F4);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", effPCPBossONGY_RFrame);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F1FC);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F200);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F208);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F210);

INCLUDE_SDATA(const s32, "effect/effPCPBoss", D_0054F218);

