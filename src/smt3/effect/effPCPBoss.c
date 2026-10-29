#include "common.h"
#include "ps2asm.h"
#include "fpu.h"
#include "model.h"
#include "motion.h"
#include "draw3d.h"
#include "effect.h"
#include "event.h"
#include "nb.h"

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

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

/* Parameter fields below are read directly by effPCPCrackDraw. The complete
 * 0x8C head is copied by Create/Copy, followed by the 0x20 runtime suffix. */
typedef struct effPCPCrack_t
{
    f32 position[4];
    u8 drawTrail;
    f32 frameStep;
    f32 scale;
    f32 trailWidth;
    s32 trailColorStartFrame;
    s32 trailColorTransitionFrames;
    u32 trailStartCenterColor;
    u32 trailStartEdgeColor;
    u32 trailEndCenterColor;
    u32 trailEndEdgeColor;
    u16 drawBucket;
    u8 unk3A[2];
    u8 hasCells;
    u8 cellCountFromFrame;
    s32 cellStartFrame;
    s32 delaySpread;
    s32 cellDuration;
    s32 cellFadeIn;
    s32 cellFadeOut;
    s32 offsetRampFrames;
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    s32 cellColorStartAge;
    s32 cellColorTransitionFrames;
    u32 cellStartCenterColor;
    u32 cellStartEdgeColor;
    u32 cellEndCenterColor;
    u32 cellEndEdgeColor;
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
    parSystem_t *system;
    effPartHandle_t *part;
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
    u32 abgr;
    effPartHandle_t *hPB;
    effPartHandle_t *hPCF;
} effPCPBossBael_t;

typedef struct effPCPBossONGYParam_t
{
    struct nbFormation_t *pFormation;
} effPCPBossONGYParam_t;

typedef struct effPCPBossONGY_t
{
    struct nbFormation_t *param;
    sdfMat_t *pMat;
    f32 radian;
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
    union {
        s32 unk2C;
        f32 radian;
    };
    s32 frame;
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
    union
    {
        u8 param[0x3C0];
        struct
        {
            f32 directions[6][9][4];
            effPCPBossThunderHead_t head;
            s32 limit;
            s32 fadeIn;
            s32 fadeOut;
        } draw;
    };
    s32 pTPList;
    void *point[26];
    s32 frame;
    u32 color;
    void *handle;
} effPCPBossKAGUInvoke_t;

typedef struct effPCPBossKAGUAttack_t
{
    u8 unk00[0x54];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
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
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
    f32 randomness;
    void *polyLists[16];
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
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
    effPartHandle_t *hModel;
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
    s32 total;
    s32 fadeIn;
    s32 fadeOut;
    s32 wait;
} effPCPDANTE_Damage_Param_t;

typedef struct effPCPDANTE_Damage_tag
{
    effPCPDANTE_Damage_Param_t param;
    u8 delay[0x20];
    s32 unk030;
    u8 unk034;
    u8 unk035[3];
    effPCPBossBED_t *bed;
    void *cuts[0x20];
    s32 counters[0x20];
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
    s32 life;
    effPartHandle_t *part;
    u8 cameraOverride;
    u8 unk59[3];
    s32 frame;
} effPCP_PBCamera_tag;

typedef struct effPCPDANTE_ShowTime_Param_t
{
    s32 reserve;
} effPCPDANTE_ShowTime_Param_t;

typedef struct effPCPDANTE_ShowTime_tag
{
    u8 position[0x10];
    s32 param;
    u8 flash[3][0x38];
    void *hTex[2];
    s32 frame;
    s32 frameParts[3]; /* Bingo, AreYouReady, ShowTime at +0xC8/+0xCC/+0xD0. */
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
extern s32 effBTLModelGetID(nbFormation_t *pFormation);
extern u128 *effPCP_ThunderPolyListGetParam(void *pTPolyList);
extern void effPCP_ThunderPolyListDraw(void *hData);
extern void effPCP_ThunderPolyListSetColor(void *hData, u32 abgr);
extern void nbPanelFlashInit(void *pFlash, u32 color0, u32 color1, s32 x, f32 scaleX, f32 scaleY, s32 y);
extern void nbPanelFlashDraw(void *pFlash, u32 abgrBase);
extern void effBTLModelGetSphereCenter(void *pFormation);
extern void *effBTLSkillGetTargetFormation(s32 number);
extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
extern void ReverseMatrix(void);
extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(effPCPBossWork_t *aParm);
extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);
extern u8 D_002F5C20[], D_002F5C30[];
extern u8 effPCPBossTexRect[];
extern u8 D_0031FF80[];
extern void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, const void *pMatrix);
extern void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, const void *pPos);
extern void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);
extern void dds3EffectPartDraw(effPartHandle_t *hEffPart);
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
extern effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);
extern effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);
extern void dds3EffectPartDelete(effPartHandle_t *hEffPart);
extern void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);
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
extern void mdlSetRotate(mdlObj_t *aHandle);
extern void FromEuler(f32 x, f32 y, f32 z);
extern f32 func_002CF748(f32 y, f32 x);
extern f32 tanf(f32 x);
extern f32 __ieee754_acosf(f32 x);
extern void mdlSetPosition(mdlObj_t *aHandle);
extern void mdlLink(mdlObj_t *aHandle, void *aModelOt);
extern void sdfDrawTagInit(sdfDrawTag_t *aTag);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern sdfModelOt_t D_002F69B8[];
extern f32 aViewEye[4], aViewTarget[4];
extern sdf3DScreenParam_t dds3ScreenParam;
extern void effPCP_BED_MatrixToQuaternion(f32 matrix[4][4]);
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

#ifdef NON_MATCHING
extern f32 D_002F5B8C;
extern f32 aViewDown[4];
extern sdfModelOt_t D_002F6A58[];
extern sdf3DLightParallelPacket_t D_0056ADF0;
extern f32 sdfAtan(f32 value);
extern u32 mdlGetColor(mdlObj_t *model);
extern void mdlSetColor(mdlObj_t *model, u32 color);
extern void sdf3DMakeLightParallelPacket(sdf3DLightParallelPacket_t *packet,
                                        sdfLightSources_t lights, f32 *ambient);

void effPCPBossONGYModelTexRender(const void *target, nbFormation_t *unused, f32 height)
{
    f32 light[2][4] __attribute__((aligned(16)));
    sdfVector4_t ambient;
    sdfVector4_t distance;
    volatile struct { sdfLightSources_t list; } initial, lights;
    nbFormation_t *forms, *found;
    evtUnit_t *unit;
    mdlObj_t *model;
    u32 color;
    s32 count;
    u32 i;
    f32 scale;

    initial.list[0] = light;
    initial.list[1] = NULL;
    initial.list[2] = NULL;
    lights = initial;
    count = dds3GetKernelCount();
    if (effPCPBossONGY_RFrame == count) {
        return;
    }
    effPCPBossONGY_RFrame = count;
    SDF_VU0_COPYVECTOR(D_002F5C20, target);
    scale = height / sdfAtan(D_002F5B8C * 0.5f);
    distance.x = scale;
    distance.y = scale;
    distance.z = scale;
    distance.w = 0.0f;
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(D_002F5C20);
    SDF_VU0_COPYVECTOR_B_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&distance);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(D_002F5C30);
    SDF_VU0_COPYVECTOR(effPCPBossTexRect, aViewDown);
    forms = nbGetMainProcessData()->form;
    found = NULL;
    for (i = 4; i < 14; i++) {
        if ((forms[i].stat & 3) == 3 && forms[i].hunit != NULL) {
            found = &forms[i];
            break;
        }
    }
    if (found != NULL) {
        unit = found->hunit;
        color = unit->color;
        MMI_RGBA_UNPACK(&color, 0x3C000000);
        SDF_VU0_STOREVECTOR(light[0]);
        color = found->hunit->color50;
        MMI_RGBA_UNPACK(&color, 0x3C000000);
        SDF_VU0_STOREVECTOR(&ambient);
        SDF_VU0_COPYVECTOR(light[1], found->hunit->vec10);
        sdf3DMakeLightParallelPacket(&D_0056ADF0, (f32 (**)[4])lights.list, (f32 *)&ambient);
        model = effPCPBossTexHandle;
        model->inner->light = &D_0056ADF0;
        mdlSetColor(effPCPBossTexHandle, (mdlGetColor(found->hunit->owner) & 0xFFFFFF) | 0x80000000);
    } else {
        color = 0x80808080;
        MMI_RGBA_UNPACK(&color, 0x3C000000);
        SDF_VU0_STOREVECTOR(light[0]);
        SDF_VU0_STOREVECTOR(&ambient);
        sdf3DMakeLightParallelPacket(&D_0056ADF0, (f32 (**)[4])lights.list, (f32 *)&ambient);
        model = effPCPBossTexHandle;
        model->inner->light = &D_0056ADF0;
        mdlSetColor(model, 0x80808080);
    }
    mdlLink(effPCPBossTexHandle, D_002F6A58);
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYModelTexRender);
#endif

s32 effPCPBossONGYModelTexRenderCheck(void)
{
    return effPCPBossONGY_RFrame != dds3GetKernelCount();
}

effPCPBossONGY_t *effPCPBossONGYCreate(effPCPBossONGYParam_t *param, void *unused)
{
    effPCPBossONGY_t *work = sdfChipAllocDirect(0x10);
    nbFormation_t *value = param->pFormation;
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

/* NON_MATCHING: 306 of 358 words match at the exact 1432-byte size. */
#ifdef NON_MATCHING
extern u32 dds3CheckRenderTexture(void);
extern void *dds3GetRenderTexture(void);
extern void sdfMatSetTex(sdfMat_t *mat, sdfTexHandle_t *tex);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *mat, s32 (*allocator)(s32));
extern s32 mdlGetMotionNum(mdlObj_t *model, s32 group);
extern void sdfMotionSeek(sdfMotionCtrlr_t *controller, f32 frame);
extern f32 sdfSin(f32 angle);
extern void Mult(void);
extern void mdlSetScale(mdlObj_t *model);
extern void effPCPBossONGYModelTexRender(const void *target, nbFormation_t *form, f32 height);
extern effPCPBossVector_t D_00533DA0;
extern sdfGrOt_t sdfGrOtTable[];

void effPCPBossONGYDraw(effPCPBossONGY_t *work)
{
    effPCPBossVector_t origin;
    effPCPBossVector_t scale;
    effPCPBossVector_t target;
    effPCPBossVector_t points[32];
    f32 uv[64];
    u32 colors[32];
    nbFormation_t *form;
    sdfDrawTag_t *tag;
    effPCPBossVector_t *point;
    f32 *st;
    u32 *color;
    u32 count;
    u32 i;
    u32 phase;
    f32 rate;
    f32 angle;
    f32 angleStep;
    f32 sway;
    f32 y;
    f32 v;
    f32 nextV;
    f32 x;
    f32 width;
    f32 uvStep;
    f32 yStep;
    f32 height;
    f32 renderHeight;
    f32 offset;
    f32 scaleValue;
    mdlObj_t *model;
    sdfGrOt_t *ot;

    if (!dds3CheckRenderTexture()) {
        return;
    }
    if (!dds3GetRenderTexture()) {
        return;
    }
    model = effPCPBossTexHandle;
    if (!model) {
        return;
    }
    form = work->param;
    height = 340.0f;
    renderHeight = height;
    offset = 0.0f;
    width = 271.999969482421875f;
    SDF_VU0_LOADVECTOR(&form->pos);
    SDF_VU0_STOREVECTOR_MEMOPS(origin);
    SDF_VU0_STOREVECTOR(&target);
    mdlSetPosition(model);
    SDF_VU0_LOADVECTOR(&form->rot);
    SDF_VU0_LOADVECTOR_B(&D_00533DA0);
    Mult();
    mdlSetRotate(effPCPBossTexHandle);
    scaleValue = form->scl;
    scale.f[3] = 0.0f;
    scale.f[2] = scaleValue;
    scale.f[0] = scaleValue;
    scale.f[1] = scaleValue;
    SDF_VU0_LOADVECTOR(&scale);
    offset = height + offset;
    mdlSetScale(effPCPBossTexHandle);
    mdlSmoothLoopMotion(effPCPBossTexHandle, 0,
                       mdlGetMotionNum(form->hunit->owner, 0), 0.0f, 4.0f);
    sdfMotionSeek(((mdlObj_t *)effPCPBossTexHandle)->first,
                  (form->hunit->owner)->first->frame);
    target.f[1] -= offset;
    effPCPBossONGYModelTexRender(&target, form, renderHeight);
    if (work->frame >= 12) {
        return;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    origin.f[1] -= height;
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR_MEMOPS(origin);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(tag, 0);
    sdfMatSetTex(work->pMat, dds3GetRenderTexture());
    sdf3DPrimLinkMat(tag, work->pMat, 0);
    LookAtMatrix(D_002F5C20, D_002F5C30, effPCPBossTexRect);
    ReverseMatrix();
    rate = (f32)work->frame / 12.0f;
    effPCPBossWork.color = ((u32)((1.0f - rate) * 128.0f) << 24) | 0x808080;
    effPCPBossWork.vertexCount = 32;
    effPCPBossWork.positions = points;
    effPCPBossWork.uvs = uv;
    effPCPBossWork.primCount = 16;
    effPCPBossWork.colors = colors;
    count = 0;
    phase = (u32)(rate * 15.0f);
    angle = work->radian;
    work->radian = angle + 0.043633227f;
    angleStep = ((f32)phase * 3.141592502593994140625f) / 300.0f;
    sway = rate * 67.99999237060546875f;
    uvStep = 0.0033333333f;
    y = -height;
    v = 0.0f;
    yStep = 2.2666666507720947265625f;
    point = points;
    st = uv;
    color = colors;
    for (i = 0; i < 300; i++) {
        nextV = v + uvStep;
        st[1] = v;
        st[0] = 0.0f;
        st += 2;
        st[0] = 1.0f;
        st[1] = v;
        st += 2;
        st[1] = nextV;
        st[0] = 0.0f;
        st += 2;
        st[0] = 1.0f;
        st[1] = nextV;
        st += 2;
        x = sdfSin(angle) * sway;
        point->f[1] = y;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        point->f[0] = x - width;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[1] = y;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        point->f[0] = width + x;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        point->f[0] = x - width;
        point->f[1] = y + yStep;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        point->f[0] = width + x;
        point->f[1] = y + yStep;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        count += 4;
        color[0] = 0x80808080;
        color[1] = 0x80808080;
        color[2] = 0x80808080;
        color[3] = 0x80808080;
        color += 4;
        y += yStep;
        angle += angleStep;
        if (count >= 32) {
            sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
            color = colors;
            point = points;
            st = uv;
            count = 0;
        }
        v = nextV;
    }
    if (count >= 4) {
        effPCPBossWork.primCount = count >> 1;
        effPCPBossWork.vertexCount = count;
        sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
    }
    ot = &sdfGrOtTable[43];
    ot->regFunc(ot, tag);
    work->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGYDraw);
#endif

s32 effPCPBossONGYGetLength(void)
{
    return 0x16;
}

effPCPBossONGY_t *effPCPBossONGY2Create(effPCPBossONGYParam_t *param, void *unused)
{
    effPCPBossONGY_t *work = sdfChipAllocDirect(0x10);
    nbFormation_t *value = param->pFormation;
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

#ifdef NON_MATCHING
void effPCPBossONGY2Draw(effPCPBossONGY_t *work)
{
    effPCPBossVector_t origin, scale, target, points[32];
    f32 uv[64];
    u32 colors[32];
    volatile u32 colorScratch, convertedColor;
    nbFormation_t *form;
    sdfDrawTag_t *tag;
    effPCPBossVector_t *point;
    f32 *st;
    u32 *color;
    sdfGrOt_t *ot;
    mdlObj_t *model;
    u32 count, i;
    f32 width, height, offset, scaleValue;
    f32 rate, filledHeight, y, v, nextV, angle, initialSway, sway, fade;

    if (!dds3CheckRenderTexture()) {

        return;

    }
    if (!dds3GetRenderTexture()) {
        return;
    }
    model = effPCPBossTexHandle;
    if (!model) {
        return;
    }
    form = work->param;
    width = 271.999969482421875f;
    height = 340.0f;
    offset = 0.0f;
    SDF_VU0_LOADVECTOR(&form->pos);
    SDF_VU0_STOREVECTOR_MEMOPS(origin);
    SDF_VU0_STOREVECTOR(&target);
    mdlSetPosition(model);
    SDF_VU0_LOADVECTOR(&form->rot);
    SDF_VU0_LOADVECTOR_B(&D_00533DA0);
    Mult();
    mdlSetRotate(effPCPBossTexHandle);
    scaleValue = form->scl;
    scale.f[3] = 0.0f;
    scale.f[2] = scaleValue;
    scale.f[0] = scaleValue;
    scale.f[1] = scaleValue;
    SDF_VU0_LOADVECTOR(&scale);
    offset = height + offset;
    mdlSetScale(effPCPBossTexHandle);
    mdlSmoothLoopMotion(effPCPBossTexHandle, 0,
                        mdlGetMotionNum(form->hunit->owner, 0), 0.0f, 4.0f);
    sdfMotionSeek(((mdlObj_t *)effPCPBossTexHandle)->first, form->hunit->owner->first->frame);
    target.f[1] -= offset;
    effPCPBossONGYModelTexRender(&target, form, height);
    if (work->frame >= 17) {
        return;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    origin.f[1] -= height;
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR_MEMOPS(origin);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(tag, 0);
    sdfMatSetTex(work->pMat, dds3GetRenderTexture());
    sdf3DPrimLinkMat(tag, work->pMat, 0);
    LookAtMatrix(D_002F5C20, D_002F5C30, effPCPBossTexRect);
    ReverseMatrix();
    effPCPBossWork.color = 0x80808080;
    effPCPBossWork.positions = points;
    effPCPBossWork.colors = colors;
    effPCPBossWork.uvs = uv;
    rate = (1.0f - (f32)work->frame / 17.0f) * 0.800000011920929f;
    filledHeight = rate * 680.0f;
    if (rate > 0.0f) {
        uv[0] = 0.0f;
        uv[1] = 0.0f;
        uv[2] = 1.0f;
        uv[3] = 0.0f;
        uv[4] = 0.0f;
        uv[5] = rate;
        uv[6] = 1.0f;
        uv[7] = rate;
        point = points;
        point->f[0] = -width;
        point->f[1] = -height;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[0] = width;
        point->f[1] = -height;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[0] = -width;
        point->f[1] = filledHeight - height;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        point++;
        point->f[0] = width;
        point->f[1] = filledHeight - height;
        point->f[2] = 0.0f;
        point->f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(point);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(point);
        effPCPBossWork.vertexCount = 4;
        colors[3] = 0x80808080;
        colors[0] = 0x80808080;
        colors[1] = 0x80808080;
        colors[2] = 0x80808080;
        effPCPBossWork.primCount = 2;
        sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
    }
    ot = &sdfGrOtTable[43];
    if (rate < 1.0f) {
        angle = work->radian;
        effPCPBossWork.vertexCount = 32;
        effPCPBossWork.primCount = 16;
        y = filledHeight - height;
        work->radian = angle + 0.15707962214946747f;
        v = rate;
        initialSway = sdfSin(angle) * 27.199995040893555f;
        fade = 1.0f;
        count = 0;
        point = points;
        st = uv;
        color = colors;
        for (i = 0; i < 100; i++) {
            nextV = v + 0.001999999862164259f;
            st[1] = v;
            st[0] = 0.0f;
            st[2] = 1.0f;
            st[3] = v;
            st[5] = nextV;
            st[4] = 0.0f;
            st[6] = 1.0f;
            st[7] = nextV;
            st += 8;
            sway = initialSway - sdfSin(angle) * 27.199995040893555f;
            point->f[1] = y;
            point->f[2] = 0.0f;
            point->f[3] = 0.0f;
            point->f[0] = sway - width;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(point);
            point++;
            point->f[1] = y;
            point->f[2] = 0.0f;
            point->f[3] = 0.0f;
            point->f[0] = width + sway;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(point);
            point++;
            point->f[2] = 0.0f;
            point->f[3] = 0.0f;
            point->f[0] = sway - width;
            point->f[1] = y + 1.3599997758865356f;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(point);
            point++;
            point->f[2] = 0.0f;
            point->f[3] = 0.0f;
            point->f[0] = width + sway;
            point->f[1] = y + 1.3599997758865356f;
            SDF_VU0_LOADVECTOR(point);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(point);
            point++;
            colorScratch = 0x80808080;
            MMI_RGBA_UNPACK(&colorScratch, 0x3C000000);
            SDF_VU0_SCALEVECTOR(fade);
            MMI_RGBA_PACK_128(convertedColor);
            count += 4;
            color[0] = 0x80808080;
            color[1] = 0x80808080;
            color[2] = 0x80808080;
            color[3] = 0x80808080;
            color += 4;
            y += 1.3599997758865356f;
            v = nextV;
            fade += -0.009999999776482582f;
            angle += 0.18849553167819977f;
            if (count >= 32) {
                sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
                count = 0;
                point = points;
                st = uv;
                color = colors;
            }
        }
        if (count >= 4) {
            effPCPBossWork.primCount = count >> 1;
            effPCPBossWork.vertexCount = count;
            sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
        }
    }
    ot->regFunc(ot, tag);
    work->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossONGY2Draw);
#endif

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
    work->frame = 0;
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

#ifdef NON_MATCHING
void effPCPBossBUNSINDrawPoly(sdfDrawTag_t *tag, effPCPBossBUNSIN_t *work,
                              const u128 *center, u32 color, f32 sway, f32 angleRange,
                              f32 width, f32 height)
{
    effPCPBossVector_t points[32];
    f32 uv[64];
    effPCPBossVector_t *point;
    f32 *st;
    f32 angle, angleStep, y, yStep, v, nextV, x, halfWidth;
    u32 i;
    u16 count;

    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR(center);
    VU0_ADD_VF31_VF10_XYZ();
    sdf3DPrimLinkTm(tag, 0);
    LookAtMatrix(D_002F5C20, D_002F5C30, effPCPBossTexRect);
    ReverseMatrix();
    yStep = height / 100.0f;
    angle = work->radian;
    halfWidth = width * 0.5f;
    effPCPBossWork.vertexCount = 32;
    effPCPBossWork.primCount = 16;
    effPCPBossWork.positions = points;
    angleStep = (angleRange * 3.141592502593994140625f) / 100.0f;
    effPCPBossWork.colors = NULL;
    y = -(height * 0.5f);
    work->radian = angle + 0.052359871566295624f;
    effPCPBossWork.uvs = uv;
    effPCPBossWork.color = color;
    point = points;
    st = uv;
    v = 0.0f;
    count = 0;
    for (i = 0; i < 100; i++) {
        nextV = v + 0.01f;
        st[0] = 0.0f;
        st[1] = v;
        st[2] = 1.0f;
        st[3] = v;
        st[4] = 0.0f;
        st[5] = nextV;
        st[6] = 1.0f;
        st[7] = nextV;
        st += 8;
        x = sdfSin(angle) * sway;
        point[0].f[0] = x - halfWidth;
        point[0].f[1] = y;
        point[0].f[2] = 0.0f;
        point[0].f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(&point[0]);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&point[0]);
        point[1].f[0] = halfWidth + x;
        point[1].f[1] = y;
        point[1].f[2] = 0.0f;
        point[1].f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(&point[1]);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&point[1]);
        point[2].f[0] = x - halfWidth;
        point[2].f[1] = y + yStep;
        point[2].f[2] = 0.0f;
        point[2].f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(&point[2]);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&point[2]);
        point[3].f[0] = halfWidth + x;
        point[3].f[1] = y + yStep;
        point[3].f[2] = 0.0f;
        point[3].f[3] = 0.0f;
        SDF_VU0_LOADVECTOR(&point[3]);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&point[3]);
        point += 4;
        count += 4;
        y += yStep;
        angle += angleStep;
        if (count >= 32) {
            st = uv;
            sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
            count = 0;
            point = points;
        }
        v = nextV;
    }
    if (count >= 4) {
        effPCPBossWork.primCount = count >> 1;
        effPCPBossWork.vertexCount = count;
        sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork));
    }
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossBUNSINDrawPoly);
#endif

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

#ifdef NON_MATCHING
extern void effPCPBossBUNSINDraw(void *pPCPBossBUNSIN);
#endif

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

effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    memcpy(work, src, sizeof(effCrackHead_t));
    work->frame = 0;
    work->abgr = 0x80808080;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}

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

extern f32 D_0031FF70[4];
extern void mdlSetScale(mdlObj_t *model);
extern void dds3TrackPolyDraw(parSystem_t *system);
extern void dds3FlashPolyListDraw(void *pool);
extern void dds3FlashPolyListSetABGR(void *pool, u32 color);
extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);

/* Same native algorithm as dds-decomp/redthing1's effPCPCrackDraw: transform
 * each trail sample, then emit and fade its delayed flash cells. */
void effPCPCrackDraw(effPCPCrack_t *work) {
    f32 perpendicular[4];
    f32 direction[4];
    f32 axis[4];
    f32 scaleVector[4];
    f32 widthVector[4];
    f32 displacement[4];
    f32 baseVector[4];
    f32 tipVector[4];
    mdlObj_t *model;
    struct sdfModelRootNode_t *map;
    s32 i;
    s32 colorStart;
    s32 mode;
    u32 tint;
    u32 center;
    u128 *historyStart;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    s32 colorFrames;
    u8 cellCountFromFrame;
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *cell;
    parCell_t *historyCell;
    u128 *positions; /* Position workspace reused for trail history and cell vertices. */
    u32 *colorWords;
    s32 frame;
    s32 groupCount;
    s32 colorCellCount;
    s32 j, count;
    s32 historyCount;
    s32 age, currentAge;
    u32 edge, alphaColor;
    f32 blend, trailLength, cellLength, taperStep;
    f32 taperRange;
    f32 baseExtent, tipExtent;

    frame = work->frame;
    mode = work->directionMode;
    model = dds3EffectPartGetHandle(work->part);
    map = model->inner;
    SDF_VU0_LOADVECTOR(work->position);
    mdlSetPosition(model);
    scaleVector[0] = work->scale;
    scaleVector[1] = work->scale;
    scaleVector[2] = work->scale;
    scaleVector[3] = 0.0f;
    SDF_VU0_LOADVECTOR(scaleVector);
    mdlSetScale(model);
    mdlLink(model, D_002F69B8);
    if (work->drawTrail) {
        colorStart = work->trailColorStartFrame;
        if (frame > colorStart) {
            if (work->trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->trailColorTransitionFrames;
                center = effColorBlend(work->trailStartCenterColor, work->trailEndCenterColor, blend);
                edge = effColorBlend(work->trailStartEdgeColor, work->trailEndEdgeColor, blend);
            } else {
                center = work->trailEndCenterColor;
                edge = work->trailEndEdgeColor;
            }
        } else {
            center = work->trailStartCenterColor;
            edge = work->trailStartEdgeColor;
        }
        groupCount = work->MarkCount;
        for (i = 0; i < groupCount; i++) {
            colorWords = work->system->cells[i].colors;
            colorCellCount = work->cellCount;
            if (colorCellCount != 0) {
                j = colorCellCount;
                do {
                    colorWords[0] = edge;
                    colorWords[1] = center;
                    colorWords[2] = edge;
                    colorWords += 3;
                    --j;
                } while (j != 0);
            }
            groupCount = work->MarkCount;
        }
    }
    groupCount = work->MarkCount;
    widthVector[0] = (work->trailWidth * 0.5f) * scaleVector[0];
    widthVector[1] = (work->trailWidth * 0.5f) * scaleVector[0];
    widthVector[2] = (work->trailWidth * 0.5f) * scaleVector[0];
    widthVector[3] = 0.0f;
    perpendicular[0] = perpendicular[1] = perpendicular[2] = perpendicular[3] = widthVector[3];
    tint = work->abgr;
    i = 0;
    if (groupCount > 0) {
        taperRange = -1.0f;
        do {
            historyCell = &work->system->cells[i];
            positions = historyCell->history;
            historyCell->color = tint;
            if (mode == 2) {
                direction[0] = 0.0f;
                direction[1] = 0.0f;
                direction[2] = 1.0f;
                sdfModelGetMarkPosMatrix(map, i);
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                work->pPart[i].dir[0] = direction[0];
                work->pPart[i].dir[1] = direction[1];
                work->pPart[i].dir[2] = direction[2];
            }
            sdfModelGetMarkPos(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->vertexCount;
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount);
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 1);
                SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 2);
                historyCell->vertexCount += 3;
                taperStep = taperRange / frame;
                blend = 1.0f;
                SDF_VU0_LOADVECTOR_C(D_0031FF70);
                if (frame > 0) {
                    const f32 *trailWidth = widthVector;
                    j = frame;
                    do {
                        SDF_VU0_LOADVECTOR(positions + 4);
                        SDF_VU0_LOADVECTOR_B(positions + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(trailLength);
                        if (trailLength > 1.0f) {
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(perpendicular);
                        } else {
                            SDF_VU0_LOADVECTOR(perpendicular);
                        }
                        SDF_VU0_LOADVECTOR_B(trailWidth);
                        SDF_VU0_SCALEVECTOR_B_BITS(blend);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_LOADVECTOR(positions + 1);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(positions);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(positions + 2);
                        --j;
                        blend += taperStep;
                        positions += 3;
                    } while (j != 0);
                }
            }
            i++;
        } while (i < groupCount);
    }
    if (work->drawTrail) {
        dds3TrackPolyDraw(work->system);
    }
    if (work->groupsHandle != NULL) {
        duration = work->cellDuration;
        count = frame - work->cellStartFrame;
        fadeIn = work->cellFadeIn;
        fadeOut = work->cellFadeOut;
        colorStart = work->cellColorStartAge;
        colorFrames = work->cellColorTransitionFrames;
        cellCountFromFrame = work->cellCountFromFrame;
        if (count > 0) {
            ramp = work->offsetRampFrames;
            if (cellCountFromFrame == 1) count = frame;
            if (count > work->cellCount) count = work->cellCount;
            group = work->pPart;
            for (i = 0; i < groupCount; i++, group++) {
                historyCell = &work->system->cells[i];
                historyStart = historyCell->history;
                cell = group->pSize;
                dds3FlashPolyListSetABGR(group->hFlash, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = (u128 *)dds3FlashPolyListGetVertex(group->hFlash, j);
                    colorWords = dds3FlashPolyListGetVC(group->hFlash, j)->color;
                    age = cell->frame;
                    if (age > colorStart) {
                        if (work->cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effColorBlend(work->cellStartCenterColor, work->cellEndCenterColor, blend);
                            edge = effColorBlend(work->cellStartEdgeColor, work->cellEndEdgeColor, blend);
                        } else {
                            center = work->cellEndCenterColor;
                            edge = work->cellEndEdgeColor;
                        }
                    } else {
                        center = work->cellStartCenterColor;
                        edge = work->cellStartEdgeColor;
                    }
                    center &= 0xFFFFFF;
                    edge &= 0xFFFFFF;
                    currentAge = cell->frame;
                    if (currentAge >= 0) {
                        if (currentAge >= ramp) direction[1] = cell->length;
                        else direction[1] = (cell->length * currentAge) / ramp;
                        displacement[0] = group->dir[0] * direction[1];
                        displacement[1] = group->dir[1] * direction[1];
                        displacement[2] = group->dir[2] * direction[1];
                        displacement[3] = 0.0f;
                        if (mode == 2 && currentAge >= group->rotationStartAge) {
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(historyStart + 1);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR_NOMEM(axis);
                            SetRotateMatrixAxis(group->rotate, axis);
                            SDF_VU0_LOADVECTOR(displacement);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(displacement);
                            group->rotate += group->rotate_add;
                        }
                        baseExtent = cell->width_in;
                        baseVector[0] = baseExtent;
                        baseVector[1] = baseExtent;
                        baseVector[2] = baseExtent;
                        baseVector[3] = 0.0f;
                        tipExtent = cell->width_out;
                        tipVector[0] = tipExtent;
                        tipVector[1] = tipExtent;
                        tipVector[2] = tipExtent;
                        tipVector[3] = 0.0f;
                        SDF_VU0_LOADVECTOR(historyStart + 4);
                        SDF_VU0_LOADVECTOR_B(historyStart + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(cellLength);
                        if (cellLength > 1.0f) {
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_COPYVECTOR_A_TO_B();
                            SDF_VU0_LOADVECTOR(tipVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(tipVector);
                            SDF_VU0_LOADVECTOR(baseVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(baseVector);
                            SDF_VU0_LOADVECTOR_C(displacement);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(baseVector);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 4);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(tipVector);
                            SDF_VU0_ADDVECTOR_A_C();
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 1);
                        } else {
                            SDF_VU0_LOADIDENTITYVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_STOREVECTOR(positions + 1);
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_STOREVECTOR(positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMagaCylinderCalcColor(edge, alphaColor);
                        colorWords[1] = effMagaCylinderCalcColor(edge, alphaColor);
                        if (cell->type) {
                            colorWords[2] = effMagaCylinderCalcColor(0x80000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMagaCylinderCalcColor(0xFF000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->frame++;
                }
                dds3FlashPolyListDraw(group->hFlash);
            }
        }
    }
    work->frame++;
}

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
    work->abgr = 0x80808080;
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

effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, &hData->param, 0x10);
    work->frame = 0;
    work->abgr = 0x80808080;
    work->hPB = dds3EffectPartCopy(hData->hPB);
    work->hPCF = dds3EffectPartCopy(hData->hPCF);
    return work;
}
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
    dds3EffectPartSetColor(hData->hPB, hData->abgr);
    dds3EffectPartSetColor(hData->hPCF, hData->abgr);
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
    hData->abgr = abgr;
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
    nbFormation_t *model;
    nbFormation_t *found;
    s32 id;
    s32 i;
    f32 r;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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
    nbFormation_t *model;
    u128 *param;
    u32 bits;
    s32 id;
    s32 i;
    nbFormation_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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
    count = work->pTPList * 6;
    if (count > 0) {
        i = count;
        list = work->point;
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

    count = hData->pTPList * 6;
    if (count > 0) {
        i = count;
        list = hData->point;
        do {
            effPCP_ThunderPolyListDelete(*list++);
            i--;
        } while (i != 0);
    }
    sdfMemFreeDirect(hData->handle);
}

/* NON_MATCHING: 3 of 184 words differ (first at +0x1A4). */
#ifdef NON_MATCHING
void effPCPBossKAGUInvokeDraw(effPCPBossKAGUInvoke_t *hData)
{
    nbFormation_t *model;
    nbFormation_t *found;
    effPCPBossVector_t marks[8];
    effPCPBossVector_t center;
    effPCPBossVector_t *centerPtr;
    u128 *param;
    s32 frame;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    s32 rest;
    s32 i;
    s32 id;
    u32 count;
    u32 index;
    u32 listIndex;
    u32 slot;
    u32 vertex;
    void **list;
    f32 blend;
    f32 distance;
    f32 scale;
    u32 color;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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

    frame = hData->frame;
    limit = hData->draw.limit;
    fadeIn = hData->draw.fadeIn;
    fadeOut = hData->draw.fadeOut;
    if (frame < limit) {
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else {
            rest = limit - frame;
            if (fadeOut >= rest && fadeOut != 0) {
                blend = (f32)rest / (f32)fadeOut;
            } else {
                blend = 1.0f;
            }
        }
        color = effColorBlend(hData->color & 0xFFFFFF, hData->color, blend);

        for (index = 0; index < 8; index++) {
            effBTLModelGetMarkPosition2(model, 0x14 + index);
            SDF_VU0_STOREVECTOR(&marks[index]);
        }
        effBTLModelGetMarkPosition2(model, 0);
        SDF_VU0_STOREVECTOR(&center);
        centerPtr = &center;

        SDF_VU0_LOADVECTOR_B(centerPtr);
        SDF_VU0_LOADVECTOR(&marks[0]);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(distance);
        count = (u32)hData->pTPList;
        slot = 0;
        distance -= 200.0f;

        for (index = 0; index < count; index++) {
            for (listIndex = 0; listIndex < 6; listIndex++) {
                list = &hData->point[slot];
                param = effPCP_ThunderPolyListGetParam(*list);
                SDF_VU0_LOADVECTOR_B(centerPtr);
                for (vertex = 0; vertex < 9; vertex++) {
                    SDF_VU0_LOADVECTOR(&hData->draw.directions[listIndex][vertex][0]);
                    SDF_VU0_VECTOR_W_SET_ZERO();
                    scale = distance + hData->draw.directions[listIndex][vertex][3];
                    SDF_VU0_SCALEVECTOR_BITS(scale);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&param[vertex]);
                }
                effPCP_ThunderPolyListSetColor(*list, color);
                effPCP_ThunderPolyListDraw(*list);
                slot++;
            }
        }
    }
    hData->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUInvokeDraw);
#endif

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

#ifdef NON_MATCHING
void effPCPBossKAGUAttackDraw(effPCPBossKAGUAttack_t *work)
{
    nbFormation_t *model;
    effPCPBossVector_t marks[8];
    effPCPBossVector_t center;
    effPCPBossVector_t end;
    effPCPBossPlace_t place;
    s32 frame, limit, fadeIn, fadeOut, rest, id;
    u32 i, j, groupCount, color;
    f32 blend;
    void **list;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 32; i++, model++) {
            if ((model->stat & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x125 || id == 0x108) {
                    break;
                }
            }
        }
        if (i == 32) {
            return;
        }
    }
    frame = work->frame;
    limit = work->limit;
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    if (frame < limit) {
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else {
            rest = limit - frame;
            if (fadeOut >= rest && fadeOut != 0) {
                blend = (f32)rest / (f32)fadeOut;
            } else {
                blend = 1.0f;
            }
        }
        color = effColorBlend(work->color & 0xFFFFFF, work->color, blend);
        center.f[0] = 0.0f;
        center.f[1] = 0.0f;
        center.f[2] = 0.0f;
        for (i = 0; i < 8; i++) {
            effBTLModelGetMarkPosition2(model, i + 0x14);
            SDF_VU0_STOREVECTOR(&marks[i]);
            center.f[0] += marks[i].f[0];
            center.f[1] += marks[i].f[1];
            center.f[2] += marks[i].f[2];
        }
        center.f[3] = 0.0f;
        center.f[2] *= 0.125f;
        center.f[0] *= 0.125f;
        center.f[1] *= 0.125f;
        SDF_VU0_LOADVECTOR(&model->pos);
        SDF_VU0_LOADVECTOR_B(effPCPBossKAGUDrawParam);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&end);
        groupCount = work->groupCount;
        list = work->polyLists;
        for (i = 0; i < groupCount; i++) {
            for (j = 0; j < 8; j++, list++) {
                if (j != 6) {
                    effPCPBossKAGUAttackGetSplitPoint(effPCP_ThunderPolyListGetParam(*list),
                                                      &marks[j].q, &end.q, &center.q);
                    effPCP_ThunderPolyListSetColor(*list, color);
                    effPCP_ThunderPolyListDraw(*list);
                }
            }
        }
        place.payload[0] = end.f[0];
        place.payload[1] = end.f[1];
        place.payload[2] = end.f[2];
        place.payload[3] = 0.0f;
        place.payload[4] = 0.0f;
        place.payload[5] = 0.0f;
        place.payload[6] = 0.0f;
        place.payload[7] = 1.0f;
        place.payload[8] = 100.0f;
        place.payload[9] = 100.0f;
        place.payload[10] = work->scale;
        place.color = color;
        effEveBEDCutSetParams(work->cut, &place);
        effEveBEDCutDraw(work->cut);
    }
    work->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGUAttackDraw);
#endif

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
    nbFormation_t *model;
    nbFormation_t *found;
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
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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

#ifdef NON_MATCHING
void effPCPBossKAGU2AttackDraw(effPCPBossKAGU2Attack_t *work)
{
    nbFormation_t *model;
    effPCPBossVector_t marks[6], center, end;
    effPCPBossPlace_t place;
    s32 frame, limit, fadeIn, fadeOut, rest, id;
    u32 i, count, color;
    f32 blend, randomness;
    void **list;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 32; i++, model++) {
            if ((model->stat & 3) == 3) {
                id = effBTLModelGetID(model);
                if (id == 0x125 || id == 0x108) {
                    break;
                }
            }
        }
        if (i == 32) {
            return;
        }
    }
    frame = work->frame;
    limit = work->duration;
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    if (frame < limit) {
        if (frame < fadeIn && fadeIn != 0) {
            blend = (f32)frame / (f32)fadeIn;
        } else {
            rest = limit - frame;
            if (fadeOut >= rest && fadeOut != 0) {
                blend = (f32)rest / (f32)fadeOut;
            } else {
                blend = 1.0f;
            }
        }
        color = effColorBlend(work->color & 0xFFFFFF, work->color, blend);
        center.f[0] = 0.0f;
        center.f[1] = 0.0f;
        center.f[2] = 0.0f;
        for (i = 0; i < 6; i++) {
            effBTLModelGetMarkPosition2(model, i + 0x1E);
            SDF_VU0_STOREVECTOR(&marks[i]);
            center.f[0] += marks[i].f[0];
            center.f[1] += marks[i].f[1];
            center.f[2] += marks[i].f[2];
        }
        center.f[3] = 0.0f;
        center.f[2] /= 6.0f;
        center.f[0] /= 6.0f;
        center.f[1] /= 6.0f;
        SDF_VU0_LOADVECTOR(&model->pos);
        SDF_VU0_LOADVECTOR_B(effPCPBossKAGU2DrawParam);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&end);
        count = work->groupCount;
        randomness = work->randomness;
        list = work->polyLists;
        for (i = 0; i < count; i++) {
            effPCPBossKAGU2AttackGetSplitPoint(effPCP_ThunderPolyListGetParam(*list),
                                              &marks[1].q, &end, &center.q, randomness, 0.0f);
            effPCP_ThunderPolyListSetColor(*list, color);
            effPCP_ThunderPolyListDraw(*list++);
            effPCPBossKAGU2AttackGetSplitPoint(effPCP_ThunderPolyListGetParam(*list),
                                              &marks[2].q, &end, &center.q, randomness, 0.61086511611938477f);
            effPCP_ThunderPolyListSetColor(*list, color);
            effPCP_ThunderPolyListDraw(*list++);
            effPCPBossKAGU2AttackGetSplitPoint(effPCP_ThunderPolyListGetParam(*list),
                                              &marks[4].q, &end, &center.q, randomness, -0.61086511611938477f);
            effPCP_ThunderPolyListSetColor(*list, color);
            effPCP_ThunderPolyListDraw(*list++);
            effPCPBossKAGU2AttackGetSplitPoint(effPCP_ThunderPolyListGetParam(*list),
                                              &marks[5].q, &end, &center.q, randomness, 0.0f);
            effPCP_ThunderPolyListSetColor(*list, color);
            effPCP_ThunderPolyListDraw(*list++);
        }
        place.payload[0] = end.f[0];
        place.payload[1] = end.f[1];
        place.payload[2] = end.f[2];
        place.payload[3] = 0.0f;
        place.payload[4] = 0.0f;
        place.payload[5] = 0.0f;
        place.payload[6] = 0.0f;
        place.payload[7] = 1.0f;
        place.payload[8] = 100.0f;
        place.payload[9] = 100.0f;
        place.payload[10] = work->scale;
        place.color = color;
        effEveBEDCutSetParams(work->hBEDCut, &place);
        effEveBEDCutDraw(work->hBEDCut);
    }
    work->frame++;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPBossKAGU2AttackDraw);
#endif

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
    nbFormation_t *model;
    s32 id;
    u32 i;
    nbFormation_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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
    nbFormation_t *model;
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
    nbFormation_t *found;

    if (effBTLSkillCheckRun() != 0) {
        model = effBTLSkillGetInvokeFormation();
    } else {
        found = NULL;
        model = nbGetMainProcessData()->form;
        for (i = 0; i < 0x20; i++) {
            if ((model->stat & 3) == 3) {
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

void effPCPScreenCrackModelDraw(effPCPScreenCrackModel_t *hData)
{
    f32 view[4] __attribute__((aligned(16)));
    f32 angle;
    f32 halfFovy;
    f32 distance;
    f32 pi;
    mdlObj_t *model = dds3EffectPartGetHandle(hData->hModel);
    sdfDrawTag_t *tag;
    u64 *packet;
    sdfGrOt_t *ot;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(view);
    pi = 3.141592503f;
    angle = pi - __ieee754_acosf(view[1]);
    FromEuler(angle, func_002CF748(view[0], view[2]) + pi, 0.0f);
    mdlSetRotate(model);

    halfFovy = tanf(dds3ScreenParam.fovy * 0.5f);
    distance = 384.0f / (halfFovy + halfFovy);
    SDF_VU0_LOADVECTOR_B(view);
    /* Transfer the binary32 scalar through a GPR to the second VU vector. */
    SDF_VU0_SCALEVECTOR_B_BITS(-distance);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_ADDVECTOR();
    mdlSetPosition(model);

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    packet = (u64 *)sdfGrAllocPacketBuff(0x30);
    packet[4] = 0x31801;
    packet[0] = 2;
    packet[1] = 0x5000000210000000ULL;
    packet[2] = 0x1000000000008001ULL;
    packet[3] = 0xE;
    packet[5] = 0x47;
    sdfDrawTagLink(tag, (u32)packet);
    ot = D_002F69B8[7].ot[1];
    ot->regFunc(ot, tag);
    mdlLink(model, &D_002F69B8[7]);

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    packet = (u64 *)sdfGrAllocPacketBuff(0x30);
    packet[4] = 0x717FB;
    packet[0] = 2;
    packet[1] = 0x5000000210000000ULL;
    packet[2] = 0x1000000000008001ULL;
    packet[3] = 0xE;
    packet[5] = 0x47;
    sdfDrawTagLink(tag, (u32)packet);
    ot = D_002F69B8[7].ot[1];
    ot->regFunc(ot, tag);
    hData->frame++;
}


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

#ifdef NON_MATCHING
extern void effPCPDANTE_Thunder_Draw(effPCPDANTE_Thunder_tag *hData);
#endif

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
    nbFormation_t *unit;
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
    unit = nbGetMainProcessData()->form;
    bits = effBTLSkillGetTargetBit();
    count = 0;
    slot = work->delay;
    for (i = 0; i < 32; i++, unit++) {
        if (unit->stat & 1) {
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
        u8 held = work->delay[a];

        work->delay[a] = work->delay[b];
        work->delay[b] = held;
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

extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate);
extern f32 effBTLPartGetScalePtr(nbFormation_t *form);

void effPCPDANTE_Damage_Run(effPCPDANTE_Damage_tag *work)
{
    effPCPBossPlace_t place;
    /* Scan state precedes timing snapshots for the native spill order;
       see docs/levers/scalar-spills.md. */
    u32 i;
    u32 targetBits;
    u32 slot = 0;
    s32 total = work->param.total;
    s32 fadeIn = work->param.fadeIn;
    s32 fadeOut = work->param.fadeOut;
    nbFormation_t *unit = nbGetMainProcessData()->form;
    targetBits = effBTLSkillGetTargetBit();

    for (i = 0; i < 0x20; i++, unit++) {
        if ((unit->stat & 1) != 0 && (targetBits & ((u32)1 << i)) != 0) {
            f32 scale;

            place.payload[0] = 0.0f;
            place.payload[1] = 0.0f;
            place.payload[2] = 0.0f;
            place.payload[3] = 0.0f;
            effBTLModelGetSphereCenter(unit);
            SDF_VU0_STOREVECTOR(place.payload);
            SDF_VU0_COPYVECTOR(&place.payload[4], &unit->rot);
            place.payload[8] = unit->r * unit->scl;
            place.payload[9] = unit->h * unit->scl;
            scale = effBTLPartGetScalePtr(unit);
            place.payload[10] = scale;

            if (work->delay[slot] != 0) {
                work->delay[slot] = work->delay[slot] - 1;
                if (work->delay[slot] == 0) {
                    nbSetUnitMotion(unit, 1, 0, 1.0f);
                    place.color = 0x80808080;
                    work->cuts[slot] = effEveBEDCutCreate(work->bed, 2, &place);
                    work->counters[slot] = 0;
                }
            }
            if (work->cuts[slot] != NULL) {
                s32 frame = work->counters[slot];
                f32 alpha;

                if (frame < total) {
                    if (frame < fadeIn && fadeIn != 0) {
                        alpha = (f32)frame / (f32)fadeIn;
                    } else {
                        s32 remaining = total - frame;

                        if (remaining <= fadeOut && fadeOut != 0) {
                            alpha = (f32)remaining / (f32)fadeOut;
                        } else {
                            alpha = 1.0f;
                        }
                    }
                    place.color = effColorBlend(work->color & 0xFFFFFF, work->color, alpha);
                    effEveBEDCutDraw(work->cuts[slot]);
                    work->counters[slot] = work->counters[slot] + 1;
                } else {
                    effEveBEDCutDelete(work->cuts[slot]);
                    work->cuts[slot] = NULL;
                }
            }
            slot++;
        }
    }
}

void effPCPDANTE_Damage_SetColor(effPCPDANTE_Damage_tag *hData, u32 abgr)
{
    hData->color = abgr;
}

effPCP_PBCamera_tag *effPCP_PBCamera_Create(effPCP_PBCamera_Param_t *pParam, void *pPB)
{
    effPCP_PBCamera_tag *work = sdfChipAllocDirect(0x60);
    s32 value = pParam->life;

    work->frame = 0;
    work->life = value;
    work->cameraOverride = 0;
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
    effPCP_PBCamera_tag *work = effPCP_PBCamera_Create((effPCP_PBCamera_Param_t *)&hData->life, NULL);

    work->part = dds3EffectPartCopy(hData->part);
    return work;
}

void effPCP_PBCamera_Delete(effPCP_PBCamera_tag *hData)
{
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}

extern s32 effBTLSkillGetID(void);
extern void mdlGetPosition(mdlObj_t *model);
extern void effPCP_PBCamera_SetPosition(effPCP_PBCamera_tag *work, const void *position);
extern s32 nbCameraDANTE_KAGIRI_Set(s32 frame);
extern void nbCameraSetState(u16 state);
extern void nbCameraHelperModelSetSub(mdlObj_t *model, u32 from, u32 to);

/* nbCameraProcess camera prefix; D_00320E64 is its fovy at +0x24. */
typedef struct effPCPBossCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 state_change;
    u32 state;
} effPCPBossCamera_t;
extern effPCPBossCamera_t pNbCamera;

void effPCP_PBCamera_Run(effPCP_PBCamera_tag *work)
{
    sdfVector4_t position;
    u32 skill;
    nbFormation_t *invoke;
    nbFormation_t *target;
    u32 from;
    u32 to;
    mdlObj_t *model;

    if (work->life > work->frame) {
        from = 0;
        to = 1;
        target = NULL;
        if (effBTLSkillCheckRun()) {
            skill = effBTLSkillGetID();
            invoke = effBTLSkillGetInvokeFormation();
            target = effBTLSkillGetTargetFormation(0);
            if ((invoke->stat & 3) == 3) {
                mdlGetPosition(invoke->hunit->owner);
                SDF_VU0_STOREVECTOR(&position);
                effPCP_PBCamera_SetPosition(work, &position);
            }
        } else {
            skill = 0;
        }
        if (work->frame > 0) {
            if (pNbCamera.state == 10 || pNbCamera.state_change == 10) {
                return;
            }
        }
        dds3EffectPartSetRotateMatrix(work->part, work);
        dds3EffectPartSetPosition(work->part, work->position);
        dds3EffectPartDraw(work->part);
        if (target != NULL) {
            switch (skill) {
            case 0x108:
                if (effBTLModelGetID(target) == 0x158) {
                    from = 2;
                    to = 3;
                }
                break;
            case 0x10D:
                switch ((u32)effBTLModelGetID(target)) {
                case 0x100:
                case 0x102:
                case 0x108:
                case 0x109:
                case 0x123:
                case 0x125:
                case 0x158:
                    from = 2;
                    to = 3;
                    break;
                }
                break;
            case 0x10B:
                switch ((u32)effBTLModelGetID(target)) {
                case 0x102:
                case 0x123:
                case 0x158:
                    nbCameraDANTE_KAGIRI_Set(work->frame);
                    work->cameraOverride = 1;
                    break;
                }
                break;
            case 0x107:
                switch ((u32)effBTLModelGetID(target)) {
                case 0x108:
                case 0x125:
                case 0x158:
                    work->cameraOverride = 1;
                    work->frame++;
                    break;
                }
                break;
            }
        }
        if (work->cameraOverride == 0) {
            model = dds3EffectPartGetHandle(work->part);
            if (work->frame == 0) {
                nbCameraSetState(0x17);
            }
            nbCameraHelperModelSetSub(model, from, to);
            switch (skill) {
            case 0x107:
            case 0x108:
                pNbCamera.fovy = 0x1.becde20000000p-1f;
                break;
            case 0x10B:
            case 0x10D:
                pNbCamera.fovy = 0x1.eb7c120000000p-1f;
                break;
            }
        }
        work->frame++;
    }
}


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
        work->hTex[0] = sdfTexSet(pTMX0);
        work->hTex[1] = sdfTexSet(pTMX1);
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

    work->hTex[0] = hData->hTex[0];
    work->hTex[1] = hData->hTex[1];
    work->texLoaded = 0;
    return work;
}

void effPCPDANTE_ShowTime_Delete(effPCPDANTE_ShowTime_tag *hData)
{
    if (hData->texLoaded != 0) {
        sdfTexFreeDelayed(hData->hTex[1]);
        sdfTexFreeDelayed(hData->hTex[0]);
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

extern effPCPDANTE_UV_tag effPCPDANTE_Bingo_UV;
extern void nbPnl_rsetAlpha(s32 mode);

#ifdef NON_MATCHING
void effPCPDANTE_ShowTime_Bingo_Draw(effPCPDANTE_ShowTime_tag *work)
{
    u32 frame = work->frameParts[0];
    u32 age;
    u32 color;
    s32 x;
    f32 rate;
    f32 fade;

    if (frame < 21) {
        rate = (f32)(s32)frame / 10.0f;
        fade = rate > 1.0f ? 2.0f - rate : rate;
        if (frame < 11) {
            fade *= fade;
            nbPnl_rsetAlpha(0);
        } else {
            rate = 1.0f;
            nbPnl_rsetAlpha(1);
        }
        x = (s32)(rate * 370.0f);
        color = ((u32)(fade * 128.0f) << 24) | 0x808080;
        effPCPDANTE_Sprite_Draw(x, 244, 1.0f, color, &effPCPDANTE_Bingo_UV, work->hTex[0]);
        if (frame == 10) {
            nbPnl_rsetAlpha(1);
            effPCPDANTE_Sprite_Draw(x, 244, 1.0f, 0x20808080, &effPCPDANTE_Bingo_UV, work->hTex[0]);
        }
    }
    age = frame - 10;
    if (age < 17) {
        nbPnl_rsetAlpha(0);
        effPCPDANTE_Sprite_Draw(370, 244, 1.0f, 0x80808080, &effPCPDANTE_Bingo_UV, work->hTex[0]);
    }
    if (frame - 27 < 21) {
        u32 exitAge = frame - 27;

        nbPnl_rsetAlpha(0);
        color = ((u32)((1.0f - (f32)(s32)exitAge / 20.0f) * 128.0f) << 24) | 0x808080;
        x = (s32)((f32)(s32)(exitAge * exitAge) * 1.5f) + 370;
        effPCPDANTE_Sprite_Draw(x, 244, 1.0f, color, &effPCPDANTE_Bingo_UV, work->hTex[0]);
    }
    if (age < 17) {
        nbPnl_rsetAlpha(1);
        rate = (f32)(s32)age * 0.0625f;
        color = ((u32)((1.0f - rate) * 64.0f) << 24) | 0x808080;
        effPCPDANTE_Sprite_Draw(370, 244, rate * rate + 1.0f, color,
                               &effPCPDANTE_Bingo_UV, work->hTex[0]);
        effPCPDANTE_Sprite_Draw(370, 244, rate * 0.5f * rate + 1.0f, color,
                               &effPCPDANTE_Bingo_UV, work->hTex[0]);
    }
    work->frameParts[0] = (u32)work->frameParts[0] + 1;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_Bingo_Draw);
#endif

extern effPCPDANTE_UV_tag effPCPDANTE_AreYouReady_UV;

void effPCPDANTE_ShowTime_AreYouReady_Draw(effPCPDANTE_ShowTime_tag *hData)
{
    s32 frame = hData->frameParts[1];
    u32 i;
    s32 age;
    s32 y;
    u32 alpha;
    u32 color;
    f32 rate;
    f32 fade;

    for (i = 0; i < 6; i++) {
        age = frame + i - 6;
        alpha = 48 + (u32)((f32)i / 6.0f * 80.0f);
        if (age >= 0 && age <= 16) {
            rate = (f32)age / 8.0f;
            /* Retail keeps the triangular fade at 0x1A03E0..3F4 even though
             * the draw guard below discards its descending half. Bingo uses
             * that half live (0x19FFE0..FF4), the same copied-family shape. */
            if (rate > 1.0f) {
                fade = 2.0f - rate;
            } else {
                fade = rate;
            }
            if (age <= 8) {
                fade *= fade;
                nbPnl_rsetAlpha(0);
                y = 184 - (s32)(rate * 92.0f * rate);
                color = ((u32)((f32)alpha * fade) << 24) | 0x808080;
                effPCPDANTE_Sprite_Draw(32, y, 1.0f, color,
                                       &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
                if (age == 8) {
                    nbPnl_rsetAlpha(1);
                    effPCPDANTE_Sprite_Draw(32, y, 1.0f, 0x20808080,
                                           &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
                }
            }
        }
    }
    /* The hold uses absolute frames; the glow below uses relative age. Stock
     * RTL keeps both predicates through gcse (247/486); sharing the relative
     * age instead caches one predicate across the exit loop. */
    if (frame >= 10 && frame <= 26) {
        nbPnl_rsetAlpha(0);
        effPCPDANTE_Sprite_Draw(32, 92, 1.0f, 0x80808080,
                               &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
    }
    for (i = 0; i < 4; i++) {
        s32 trailAge = frame + i - 4;
        alpha = 16 + (u32)((f32)i / 4.0f * 112.0f);
        if (trailAge >= 26 && trailAge <= 38) {
            nbPnl_rsetAlpha(0);
            trailAge -= 26;
            rate = 1.0f - (f32)trailAge / 12.0f;
            color = ((u32)((f32)alpha * rate) << 24) | 0x808080;
            y = 92 - (s32)((f32)(trailAge * trailAge) * 3.0f);
            effPCPDANTE_Sprite_Draw(32, y, 1.0f, color,
                                   &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
        }
    }
    age = frame - 10;
    if (age >= 0 && age <= 16) {
        nbPnl_rsetAlpha(1);
        rate = (f32)age / 16.0f;
        color = ((u32)((1.0f - rate) * 80.0f) << 24) | 0x808080;
        effPCPDANTE_Sprite_Draw(32, 92, rate * 0.75f * rate + 1.0f, color,
                               &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
        effPCPDANTE_Sprite_Draw(32, 92, rate * 0.325f * rate + 1.0f, color,
                               &effPCPDANTE_AreYouReady_UV, hData->hTex[0]);
    }
    hData->frameParts[1]++;
}

extern effPCPDANTE_UV_tag effPCPDANTE_ShowTime_UV;
#ifdef NON_MATCHING
void effPCPDANTE_ShowTime_ShowTime_Draw(effPCPDANTE_ShowTime_tag *work)
{
    u32 frame = work->frameParts[2];
    u32 age = frame - 23;
    void *texture = work->hTex[1];
    u32 color;
    f32 rate;

    if (age < 13) {
        nbPnl_rsetAlpha(0);
        color = ((u32)((1.0f - (f32)(s32)age / 12.0f) * 128.0f) << 24) | 0x808080;
        effPCPDANTE_Sprite_Draw(162, 172, 1.0f, color, &effPCPDANTE_ShowTime_UV, texture);
    }
    if (frame - 10 < 13) {
        nbPnl_rsetAlpha(0);
        effPCPDANTE_Sprite_Draw(162, 172, 1.0f, 0x80808080, &effPCPDANTE_ShowTime_UV, texture);
    }
    if (frame < 13) {
        nbPnl_rsetAlpha(1);
        rate = 1.0f - (f32)(s32)frame / 12.0f;
        effPCPDANTE_Sprite_Draw(162, 172, 2.0f * rate * rate + 1.0f, 0x20808080,
                               &effPCPDANTE_ShowTime_UV, texture);
        effPCPDANTE_Sprite_Draw(162, 172, rate * 1.5f * rate + 1.0f, 0x20808080,
                               &effPCPDANTE_ShowTime_UV, texture);
    }
    if (frame < 7) {
        nbPnl_rsetAlpha(1);
        rate = (f32)(s32)frame / 6.0f;
        effPCPDANTE_Sprite_Draw(162, 172, rate * rate + 1.0f, 0x10808080,
                               &effPCPDANTE_ShowTime_UV, texture);
        effPCPDANTE_Sprite_Draw(162, 172, rate * 0.75f * rate + 1.0f, 0x10808080,
                               &effPCPDANTE_ShowTime_UV, texture);
    }
    age = frame - 8;
    if (age < 15) {
        nbPnl_rsetAlpha(1);
        rate = (f32)(s32)age / 14.0f;
        color = ((u32)((1.0f - rate) * 84.0f) << 24) | 0x808080;
        effPCPDANTE_Sprite_Draw(162, 172, rate * 0.75f + 1.0f, color,
                               &effPCPDANTE_ShowTime_UV, texture);
        effPCPDANTE_Sprite_Draw(162, 172, rate * 0.325f + 1.0f, color,
                               &effPCPDANTE_ShowTime_UV, texture);
    }
    work->frameParts[2] = (u32)work->frameParts[2] + 1;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPDANTE_ShowTime_ShowTime_Draw);
#endif

/* NON_MATCHING: 4 of 75 words differ (first at +0x34). */
#ifdef NON_MATCHING

void effPCPDANTE_ShowTime_Draw(effPCPDANTE_ShowTime_tag *hData)
{
    s32 frame = hData->frame;

    if (frame == 0) {
        nbPanelFlashInit(hData->flash[0], 0x43BBFF, 0x12A2F5, 0xA0, 0.45f, 0.35f, 0x5C);
        nbPanelFlashInit(hData->flash[1], 0x3664FF, 0x3CF0, -0x8C, 0.55f, 0.45f, -0x3C);
        nbPanelFlashInit(hData->flash[2], 0x31109F, 0x8D64C8, 0, 1.5f, 2.0f, 0x14);
    }
    if (frame >= 0) {
        nbPanelFlashDraw(hData->flash[0], 0x80808080);
        effPCPDANTE_ShowTime_Bingo_Draw(hData);
    }
    if (frame >= 0x16) {
        nbPanelFlashDraw(hData->flash[1], 0x80808080);
        effPCPDANTE_ShowTime_AreYouReady_Draw(hData);
    }
    if (frame >= 0x5C) {
        nbPanelFlashDraw(hData->flash[2], 0x80808080);
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

void effPCP_BED_MatrixToQuaternion(f32 matrix[4][4])
{
    f32 quaternion[4] __attribute__((aligned(16)));
    f32 trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    f32 scale;
    s32 i;
    s32 j;
    s32 k;

    if (trace >= 1.0f) {
        scale = 2.0f * fsqrtf(trace);
        quaternion[3] = scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = matrix[0][0] > matrix[1][1] ? 0 : 1;
        if (matrix[i][i] < matrix[2][2]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        trace = matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f;
        if (trace != 0.0f) {
            scale = 2.0f * fsqrtf(trace);
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[j][i] + matrix[i][j]) / scale;
            quaternion[k] = (matrix[k][i] + matrix[i][k]) / scale;
            quaternion[3] = (matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
    quaternion[0] = -quaternion[0];
    quaternion[1] = -quaternion[1];
    SDF_VU0_LOADVECTOR_MEMOPS(*(const u128 *)quaternion);
}

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
    effPCP_BED_MatrixToQuaternion((f32 (*)[4])hData->matrix);
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

    effPCP_BED_MatrixToQuaternion((f32 (*)[4])pMatrix);
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





extern f32 sdfCos(f32 angle);

#ifdef NON_MATCHING
void dds3EffectBlurDrawQuad(const dds3Effect_Blur_t *source, effBlurPrimTriStrip_t *quad)
{
    const u8 *color = (const u8 *)&source->abgr;
    s32 x0, y0, x1, y1;
    f32 s0, t0, s1, t1;
    f32 centerX, centerY, scale, cosine, sine;
    f32 u0, v0, u1, v1, u2, v2, u3, v3;
    f32 yScale = 0.002232142857f;

    quad->r = color[0];
    quad->g = color[1];
    quad->b = color[2];
    quad->a = color[3];
    x0 = ((u32)source->xy[0][0] << 4) + 0x7000;
    quad->vtx[0].x = x0;
    y0 = ((u32)source->xy[0][1] << 3) + 0x7900;
    quad->vtx[1].x = x0;
    quad->vtx[0].y = y0;
    y1 = ((u32)source->xy[1][1] << 3) + 0x7900;
    quad->vtx[1].y = y1;
    x1 = ((u32)source->xy[1][0] << 4) + 0x7000;
    quad->vtx[2].y = y0;
    quad->vtx[3].y = y1;
    quad->vtx[3].x = x1;
    quad->vtx[2].x = x1;
    s0 = (f32)source->xy[0][0] * 0.001953125f;
    t0 = (f32)source->xy[0][1] * 0.001953125f;
    t1 = (f32)source->xy[1][1] * 0.001953125f;
    s1 = (f32)source->xy[1][0] * 0.001953125f;
    quad->vtx[0].z = 0;
    quad->vtx[1].s = s0;
    quad->vtx[2].t = t0;
    quad->vtx[1].z = 0;
    quad->vtx[2].z = 0;
    quad->vtx[3].z = 0;
    quad->vtx[0].s = s0;
    quad->vtx[0].t = t0;
    centerX = ((f32)source->center[0] + 256.0f) * 0.001953125f;
    centerY = ((f32)source->center[1] + 224.0f) * yScale;
    quad->vtx[3].t = t1;
    quad->vtx[3].s = s1;
    quad->vtx[1].t = t1;
    quad->vtx[2].s = s1;
    quad->vtx[0].adc = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].adc = 0;
    scale = source->scale;
    cosine = sdfCos(source->rotate);
    sine = sdfSin(source->rotate);
    u0 = (quad->vtx[0].s - centerX) * scale;
    v0 = (quad->vtx[0].t - centerY) * scale;
    u1 = (quad->vtx[1].s - centerX) * scale;
    v1 = (quad->vtx[1].t - centerY) * scale;
    u2 = (quad->vtx[2].s - centerX) * scale;
    v2 = (quad->vtx[2].t - centerY) * scale;
    u3 = (quad->vtx[3].s - centerX) * scale;
    v3 = (quad->vtx[3].t - centerY) * scale;
    quad->vtx[0].s = (u0 * cosine - v0 * sine) + centerX;
    quad->vtx[0].t = (u0 * sine + v0 * cosine) + centerY;
    quad->vtx[1].s = (u1 * cosine - v1 * sine) + centerX;
    quad->vtx[1].t = (u1 * sine + v1 * cosine) + centerY;
    quad->vtx[2].s = (u2 * cosine - v2 * sine) + centerX;
    quad->vtx[2].t = (u2 * sine + v2 * cosine) + centerY;
    quad->vtx[3].t = (u3 * sine + v3 * cosine) + centerY;
    quad->vtx[3].s = (u3 * cosine - v3 * sine) + centerX;
}
#else
INCLUDE_ASM(const s32, "effect/effPCPBoss", dds3EffectBlurDrawQuad);
#endif

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

