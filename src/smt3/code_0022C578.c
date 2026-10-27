#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    u16 skill[8];
    s8 count;
} cmpSkillList_t;

extern char D_0054FB00[];

extern s8 gProcessStat_0;

extern cmpSkillList_t *cmpSkillCursor;

extern void *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                              void *drawExit, void *deleteExit, void *p);

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

extern s32 datGetMaxHp(datUnitWork_t *work);

extern s32 datGetMaxMp(datUnitWork_t *work);

extern s32 dds3GetProcessStat(void *id);

extern s8 trmMode;

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s32 fldWmapEnabled;

extern u8 cmpLoadState;

extern u32 cmbBirthPanel;

extern u16 rstHeartsMasterPhase;

extern u8 rstSkillActFlag;

extern s32 D_0054FEBC;

extern s8 cmpStatusModelFlag;

extern s8 cmpStatusModelLoadState;

extern s32 cmpEncycWork;

extern u16 cmpStatusModelId;

extern u32 fclDbgPID;

extern void *D_00345D9C[];

extern s32 campStatusActive;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_tag;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ s32 texHandle[5];
    /* 0x18 */ u8 unk_18[4];
    /* 0x1C */ s32 cursor;
    /* 0x20 */ u8 unk_20[0x10];
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} dds3TitleWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ u8 unk_0019[0x17];
    /* 0x0030 */ s8 unk_30;
    /* 0x0031 */ s8 unk_31;
    /* 0x0032 */ u8 unk_0032[0x112E];
    /* 0x1160 */ s32 unk_1160;
    /* 0x1164 */ s32 unk_1164;
    /* 0x1168 */ s32 unk_1168;
    /* 0x116C */ s32 unk_116C;
    /* 0x1170 */ s32 unk_1170;
    /* 0x1174 */ s32 unk_1174;
} campInfoWork_t;

extern fclEncyc_instance_tag *g_pinst;

extern dds3TitleWork_t *titleWork;

extern campInfoWork_t *campInfoWork;

extern s8 cmpParamGaugeState[2];

extern u8 cmpStatusModelReady;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s32 stat;
    /* 0x10 */ s16 act;
    /* 0x12 */ u16 deact;
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 ctr;
    /* 0x20 */ s32 num;
} fclRibbon_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x13];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} cmpInfoWindow_t;

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

extern rcvWork_t *shutdownfunc;

extern s8 gProcessStat_3;

typedef struct
{
    s8 state;
    u8 param;
    u8 unk_02[2];
    s32 value;
} cmpCursorSlot_t;

extern cmpInfoWindow_t *cmpInfoWindow;

extern cmbWork_t *D_0054FB20;

extern s32 mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

extern void sdfMemFreeDirect(void *aHandle);

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

extern s32 gMsgID;

extern s32 gSelMsgNo;

typedef struct
{
    /* 0x00 */ s8 id;
    /* 0x01 */ s8 type;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ s32 color;
} cmpHeartsIcon_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0xF];
    /* 0x14 */ void *allocation;
    /* 0x18 */ u16 slot[10];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 unk_31;
    /* 0x32 */ u8 unk_32[7];
    /* 0x39 */ s8 slotCursor;
} campStatusPanel_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_01[3];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_05[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 state;
    /* 0x0019 */ u8 unk_19[2];
    /* 0x001B */ u8 unk_1B;
    /* 0x001C */ u8 unk_1C[0x18];
    /* 0x0034 */ s32 unk_34;
    /* 0x0038 */ s32 unk_38;
    /* 0x003C */ u8 unk_3C[0x2A8];
} cmpPanelEff_t;

extern cmpHeartsIcon_t D_00345E58[];

extern campStatusPanel_t *campStatusPanel;

extern s32 D_00574A48[6];

extern void campUpdateEffectStep(void);

extern s32 func_00232198(void);

extern cmpPanelEff_t *cmpPanelEffList;

extern s32 cmpDrawSpriteFlag(u8 id, s32 flag, s32 arg);

extern s32 cmpDrawSprite(s32 id, s32 flag, s32 arg);

extern s32 cmpDrawSprite2(s32 id, s32 flag, s32 arg);

extern s32 cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);

extern s32 cmpDrawBoxG(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *colors, s32 g, s32 h);

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct
{
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 Flag;
    /* 0x06 */ u16 res;
} fclSpriteEntry_t;

typedef struct
{
    /* 0x00 */ s8 tex;
    /* 0x02 */ fclSpriteRect_t rect;
} fclSpriteRes_t;

typedef struct
{
    /* 0x00 */ fclSpriteRes_t *res;
    /* 0x04 */ fclSpriteEntry_t *entries;
} fclSpriteTbl_t;

extern s32 sdfDelayedResrcCheck(void);

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 PartyCnt;
    /* 0x03 */ u8 unk_03[2];
    /* 0x05 */ s8 StockIdx[0x10];
    /* 0x15 */ s8 ListIdx[0x10];
    /* 0x25 */ u8 unk_25[0x31];
} cmpLocalStock_t;

/* The stock work the cursor helpers take: two 0x56-byte rows at the top and a
 * pair of 0x103C-byte records at 0xC0, both selected by the same index. */
typedef struct
{
    /* 0x0000 */ u8 unk_0000[0xE];
    /* 0x000E */ s16 unk_0E;
    /* 0x0010 */ u8 unk_10;
    /* 0x0011 */ u8 unk_0011[0x102B];
} cmpStockInfo_t;

typedef struct
{
    /* 0x0000 */ cmpLocalStock_t stock[2];
    /* 0x00AC */ u8 unk_00AC[0x14];
    /* 0x00C0 */ cmpStockInfo_t info[2];
} cmpStockWork_t;

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    /* 0x00 */ cmpCursorInfo_t cursor;
    /* 0x38 */ s8 unk_38;
} cmpPadStopWork_t;

typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ u8 unk_0038[0x1004];
} cmpListPage_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x1040];
    /* 0x1058 */ cmpListPage_t page[3];
    /* 0x410C */ u8 unk_410C[0x22];
    /* 0x412E */ s16 itemCursor;
    /* 0x4130 */ u8 itemScroll;
    /* 0x4131 */ u8 unk_4131[0x2053];
    /* 0x6184 */ cmpLocalStock_t stock[2];
    /* 0x6230 */ u8 unk_6230[0x2078];
    /* 0x82A8 */ u8 drawPhase;
    /* 0x82A9 */ u8 unk_82A9[3];
    /* 0x82AC */ cmpLocalStock_t drawStock;
    /* 0x8302 */ u8 unk_8302;
    /* 0x8303 */ s8 unk_8303;
    /* 0x8304 */ u16 itemId[1];
    /* 0x8306 */ u8 unk_8306[0x142];
    /* 0x8448 */ u8 crossPanel[0x20];
    /* 0x8468 */ s32 unk_8468;
    /* 0x846C */ u8 unk_846C[0x34];
    /* 0x84A0 */ s32 effectStep;
    /* 0x84A4 */ u8 unk_84A4[0xC];
    /* 0x84B0 */ s32 stockAlpha[16];
    /* 0x84F0 */ s8 stockStep[16];
    /* 0x8500 */ s32 stockDelay[16];
} campWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 flags2;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 unk_04[4];
} datItemEntry_t;

/* datSkillTable viewed from +0x20, which is where retail folds the base for
 * cmpChkSkillBad: it forms datSkillTable + 0x20 and indexes from there, so
 * &entry->badtype off the newdata.h declaration emits a different address.
 * kind and id are the IDB's badtype (+0x24) and basstatus (+0x26). */
typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u16 id;
    /* 0x08 */ u8 unk_08[0x14];
} datSkillBadWindow_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x20];
    /* 0x20 */ datSkillBadWindow_t bad;
} datSkillBadEntry_t;

extern datSkillBadEntry_t datSkillBadEntries[] asm("datSkillTable");

extern campWork_t *campWork;

extern datItemEntry_t datItemTable[];

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

extern void cmpBuildStockNameList(cmpLocalStock_t *stock);

extern void cmpInitStockCursorPos(void);

extern s32 cmpStartSkillTokusyuMsg(u16 skill);

extern void cmpModelInitCore2(void);

extern campStatusPanel_t *cmpStatusPhase;

extern s8 cmpStatusWindowHidden;

extern s8 cmpModelInitFlag;

extern u16 D_0054FEEE;

extern s32 cmpModelHandle;

extern fclRibbon_t *g_pdebug;

extern s32 cmpDrawBoxLine(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 *g);

extern s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot);

extern f32 dds3ScreenParam[];

extern s32 D_0054FEE0;

extern s8 D_00349380[];

extern char *D_00349388[];

extern s8 dds3PadButtonState[];

extern void *dds3GetProcessData(s32 id);

extern s8 shpProcessStat;

extern s8 gProcessStat_1;

extern s8 gProcessStat_2;

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ u8 unk_03[2];
    /* 0x05 */ s8 unk_05;
    /* 0x06 */ u8 unk_06[6];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ void *allocation;
    /* 0x18 */ void *owner;
} cmpSeqInfo_t;

extern void cmpUpdateCursorScroll(cmpCursorInfo_t *cursor, s8 mode);

extern void cmpSetInfoWindowHidden(void);

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} dds3PartyUnit_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000[0xA54];
    /* 0x00A54 */ dds3PartyUnit_t party[16];
    /* 0x01914 */ s8 stock[0x10];
    /* 0x01924 */ s32 stockCount;
    /* 0x01928 */ u8 unk_01928[0x3D];
    /* 0x01965 */ u8 unk_1965;
    /* 0x01966 */ u8 unk_01966[0x394];
    /* 0x01CFA */ u8 equipHearts;
    /* 0x01CFB */ u8 unk_01CFB[0x40];
    /* 0x01D3B */ u8 devilSlot[0x100];
    /* 0x01E3B */ u8 unk_01E3B[0x10045];
    /* 0x11E80 */ s32 encycNum;
} cmbGlobalWork_t;

/* newdata.h declares the same symbol dds3GlobalWork_t *DDS3_GBWK, and this
 * unit reads it through a different member set at the same offsets. Both are
 * partial views; the alias holds them apart until one covers the object. */
extern cmbGlobalWork_t *cmbGlobalWork asm("DDS3_GBWK");

extern u8 *GBWK_0;

extern u8 *GBWK_1;

extern u8 *GBWK_3;

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 aHandle);

typedef struct
{
    /* 0x00 */ s8 index;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 id;
} cmbStockSlot_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000;
    /* 0x00001 */ s8 unk_00001;
    /* 0x00002 */ u8 unk_00002;
    /* 0x00003 */ u8 unk_00003;
    /* 0x00004 */ s8 unk_00004;
    /* 0x00005 */ s8 unk_00005;
    /* 0x00006 */ u8 unk_00006[2];
    /* 0x00008 */ s32 unk_00008;
    /* 0x0000C */ u8 unk_0000C[0x10];
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x123EE];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B[5];
    /* 0x16510 */ s32 script;
    /* 0x16514 */ s32 scriptArg;
    /* 0x16518 */ u8 unk_16518[4];
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ u32 *unk_1654C[5];
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x1064];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s8 mode, s8 flag);

extern s32 cmpStatSetTimer(s32 kind, s8 index);

extern sdfGrOt_t sdfGrOtTable[85];

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1300];
    /* 0x1300 */ u8 frameTex[0x8E0];
} cmpDrawBank_t;

extern cmpDrawBank_t dds3DrawEnv[];

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(void *aTag, u32 aPacket);

extern void cmpReCalcStockCursorPos(s8 index);

typedef struct
{
    /* 0x00 */ u8 motionId;
    /* 0x01 */ u8 unk_01[7];
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 evo[3];
    /* 0x18 */ u8 unk_18[4];
    /* 0x1C */ f32 pos[3];
    /* 0x28 */ u8 unk_28[4];
} cmpModelParam_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x22];
    /* 0x22 */ s16 cursor;
    /* 0x24 */ u8 scroll;
    /* 0x25 */ u8 unk_25[7];
    /* 0x2C */ u8 *itemFlags;
} rcvRootWork_t;

extern cmpModelParam_t cmpModelParamTable[];

extern s32 dds3FadeCheck(void);

extern s8 gProcessStat_4;

extern s8 campExitState;

extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

extern void campDrawText(s32 id, s32 flag);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void mdlSetRotate(void *aHandle);

typedef struct
{
    /* 0x00 */ void *unk_00;
    /* 0x04 */ void *unk_04;
    /* 0x08 */ void *unk_08;
    /* 0x0C */ u8 unk_0C[0x34];
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ u8 unk_4C[8];
    /* 0x54 */ s32 cursor;
    /* 0x58 */ void *unk_58;
    /* 0x5C */ u8 unk_5C[4];
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 tover[5];
    /* 0x78 */ u8 unk_78[0x10];
    /* 0x88 */ s8 unk_88;
    /* 0x89 */ u8 unk_89[3];
    /* 0x8C */ s32 unk_8C;
    /* 0x90 */ s32 unk_90;
    /* 0x94 */ s32 unk_94[5];
    /* 0xA8 */ s32 unk_A8;
} mselGlobalWork_t;

extern mselGlobalWork_t *gGlobal;

extern s32 PlayingNo;

extern s32 fclChkMessage(void);

extern void cmpReturnParamDH(void);

extern s32 cmpUnloadStatusModel(void);

typedef s32 (*cmpSeqFunc_t)(void);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern char D_0054FB48[];

extern char D_0054FB98[];

extern char D_0054FBD8[];

extern char D_0054FCF0[];

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ u8 unk_0A;
    /* 0x0B */ u8 unk_0B;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
} cmpDHWindow_t;

typedef struct
{
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} cmpVec4_t;

extern cmpVec4_t cmpStatusModelPos;

extern cmpVec4_t cmpStatusModelBasePos;

extern s32 cmpDrawTriG(s32 a, s32 b, s32 c, s32 *colors);

extern s32 fclBlendColor(s32 color, s32 mask, s32 alpha);

extern void cmpUpdatePanelEffSlots(void);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ s32 unk_0004;
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ s32 unk_000C;
    /* 0x0010 */ u8 unk_0010[0xEC];
    /* 0x00FC */ s32 unk_00FC;
    /* 0x0100 */ s32 unk_0100;
    /* 0x0104 */ s32 unk_0104;
    /* 0x0108 */ s32 unk_0108;
    /* 0x010C */ s32 unk_010C;
    /* 0x0110 */ s32 unk_0110;
    /* 0x0114 */ s32 unk_0114;
    /* 0x0118 */ s32 unk_0118;
} stfUiWork_t;

extern stfUiWork_t *oStaff_UI;

extern s8 gMsgFlag;

extern void cmpRemoveStockDevilEntry(void);

extern void cmpInitDrawList(void);

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
} cmpMsgEntry_t;

extern cmpMsgEntry_t D_00345DD0[];

extern cmpMsgEntry_t D_00345DA0[];

extern campStatusPanel_t *campDhUpdateWork;

extern s8 cmpGetHeartsIconId(s8 index);

extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, s32 layout);

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern void cmpModelFreeResource(void);

extern void cmpResetDrawEnv(void);

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern void cmpUpdateTerminate(cmpInfoWindow_t *window);

extern void frDestroyFontRequestAfter(void *pFRQ);

extern void func_0023B500(s32 value);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpLoadStatusModel(void);

extern s32 cmpPlayCursorSE(s32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

extern s32 cmpUpdateStatusModelLoad(u16 id);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

extern s32 D_00345D88[];

extern char D_0054FB60[];

extern s32 cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 a,
                                  s32 mode, s32 b, s32 id);

extern s32 cmpModelSetup(u16 a, s32 b);

extern s32 cmpChkPadPressUpDown(void);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char D_0054FBF0[];

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char D_0054FC20[];

extern void *frwGetSmallFontFRQ2(s32 X, s32 Y, s32 Pri, s32 Color, char *pStr, s32 pFRQ);

extern void func_001C7E38(void *frq, s32 mode, s32 arg);

extern void fmProcTrigSequence(s32 mode);

extern void cmpDrawScrollBar(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h,
                             cmpCursorInfo_t *cursor, s32 j, s32 k);

void fclDrawSpriteSize(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color, s32 id, s32 idx, s32 *texList, fclSpriteTbl_t *table, s32 OtNo);

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor(cmpCursorInfo_t *cur, s8 dir);

/* Called above its definition, where ee-gcc assumed int. */
s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

s32 fclDestroyMessage(void);

void fclLoadFileAsync(const char *name, void *buf);

void fclSetSelMessagePos(s8 SelPos);

s32 fclLoadTexture(char *pFileName);

extern void *sdfMemAllocDirect(s32 aSize);

extern s32 sdfMemLock(void *aHandle);

extern void *sdfMemAllocDirect(s32 aSize);

extern s32 sdfMemLock(void *aHandle);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

/* Called above its definition, where ee-gcc assumed int. */
u16 cmpGetStatusModelId(void);

extern s32 cmpModelSetup(u16 a, s32 b);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern char D_0054FD18[]; /* "/sound/" */

extern s8 D_0054FD10;

extern s32 D_0054FD20;

extern s32 D_0054FD24;

void rstSetResultWork(void *work)
{
    D_00345D9C[0] = work;
}

void rstClearResultWork(void)
{
    D_00345D9C[0] = NULL;
}

extern char D_0053F478[];

void cmpTexLoad(void)
{
    D_00345D88[0] = fclLoadTexture(D_0053F478);
    D_00345D88[1] = fclLoadTexture("/camp/tmx/camp_02.tmx");
    D_00345D88[2] = fclLoadTexture("/camp/tmx/camp_03.tmx");
    D_00345D88[3] = fclLoadTexture("/camp/tmx/camp_04.tmx");
    D_00345D88[4] = fclLoadTexture("/camp/tmx/camp_bg.tmx");
    D_00345D88[5] = 0;
}

extern s32 D_00345D88[5];

void camp_22C610(void)
{
    s32 *handle = D_00345D88;
    s32 i;

    for (i = 4; i >= 0; i--) {
        sdfTexFree(*handle++);
    }
}

void func_0022C658(void) {
}

INCLUDE_ASM(const s32, "code_0022C578", campDebugRegistAll);

void dds3ClearStockTable(void)
{
    datUnitWork_t *stock = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    u8 *gbwk;
    s32 i;

    for (i = 0xF; i >= 0; i--, stock++) {
        stock->flag = 0;
    }
    /* The u8 * base is load-bearing: it gets one $gp load for both stores,
     * where DDS3_GBWK-> per statement reloads it. +0x1924 is stocklist, whose
     * first four bytes this clears as a word; the IDB has only unk2644 for
     * +0xA54. */
    gbwk = (u8 *)DDS3_GBWK;
    *(s32 *)(gbwk + 0x1924) = 1;
    *(u32 *)(gbwk + 0xA54) |= 7;
}

void cmpInitLocalStock(void)
{
    campWork->stock[0].count = 3;
    cmpBuildStockNameList(&campWork->stock[0]);
    cmpInitStockCursorPos();
}

void cmpInitDrawList(void)
{
    campWork->drawStock.count = -1;
    cmpBuildStockNameList(&campWork->drawStock);
}

void cmpInitStockTable(void)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        campWork->stockAlpha[i] = 0;
        campWork->stockStep[i] = 0x10;
        campWork->stockDelay[i] = 0;
    }
    for (i = 0; i < 12; i++)
    {
        campWork->stockAlpha[i + 4] = 0;
        campWork->stockStep[i + 4] = 0x20;
        campWork->stockDelay[i + 4] = i;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpinit);

extern void rstSetPanelEffFlag(void);

void camp_update_Shutdown(void)
{
    if (campWork != NULL) {
        fclDestroyMessage();
        campHideStatusWindow();
        cmpSetStatusWindowHidden();
        campHideStatusWindow();
        rstSetPanelEffFlag();
        sdfMemFreeDirect(campWork->allocation);
        campWork = NULL;
        campExitState = 2;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpProcessStart);

extern char D_0054FE38[];

extern char D_0053F4F0[];

extern char D_0053F500[];

void campKillProcesses(void)
{
    dds3KillProcessName(D_0054FE38, 0);
    dds3KillProcessName(D_0053F4F0, 0);
    dds3KillProcessName(D_0053F500, 0);
}

s32 campCheckExitState(void)
{
    if (campExitState == 1) {
        return 1;
    }
    if (campExitState < 2) {
        return 0;
    }
    if (campExitState == 2) {
        campExitState = 0;
    }
    return 0;
}

void cmpSetLoadState(u8 state)
{
    cmpLoadState = state;
}

u8 cmpGetLoadState(void)
{
    return cmpLoadState;
}

/* NON_MATCHING: 9 of 35 words differ, exact size. */
#ifdef NON_MATCHING
s32 cmpSetStockRemoveEff(u8 *work)
{
    s32 index = cmpGetStockInfoIndex((cmpLocalStock_t *)work, 0, 1);
    s32 room = cmpStockHasRoom((cmpStockWork_t *)work, 0);
    s32 slot;

    work += 0xAC;
    slot = cmpFindFreeStockSlot(index, room);
    if (room == 0) {
        return cmpGetCursorSlotValue(0, (cmpCursorSlot_t *)(work + slot * 16 + 0x1078)) != 0x100;
    }
    return cmpGetCursorSlotValue(0, (cmpCursorSlot_t *)(work + slot * 16 + 0x3C)) != 0x100;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpSetStockRemoveEff);
#endif

extern s32 cmpFindFreeStockSlot(s32 index, s32 room);

/* NON_MATCHING: 12 of 32 words differ (first at +0x50). */
#ifdef NON_MATCHING
/* Called above its definition, where ee-gcc assumed int. */
s32 cmpStockHasRoom(cmpStockWork_t *work, s8 index);

void cmpSetIniStockRecoverEff(u8 *work)
{
    s32 index = cmpGetStockInfoIndex((cmpLocalStock_t *)work, 0, 1);
    s32 room = cmpStockHasRoom((cmpStockWork_t *)work, 0);
    s32 slot;
    u8 *target;

    work += 0xAC;
    slot = cmpFindFreeStockSlot(index, room);
    target = work + slot * 16 + 0x1078;
    if (room != 0) {
        target = work + slot * 16 + 0x3C;
    }
    cmpSetCursorSlot(0, 4, 0x10, (cmpCursorSlot_t *)target);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpSetIniStockRecoverEff);
#endif

INCLUDE_ASM(const s32, "code_0022C578", cmpSetStockRecoverEff);

INCLUDE_ASM(const s32, "code_0022C578", cmpDestroyItem);

datUnitWork_t *cmpGetSelectSrcStock(void)
{
    s32 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);

    return (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index;
}

s8 cmpGetSelectedStockIndex(void)
{
    return cmpGetStockInfoIndex(&campWork->stock[0], 1, 0);
}

datUnitWork_t *cmpGetSelectDstStock(void)
{
    s32 index = cmpGetSelectedStockIndex();

    return &cmbGlobalWork->party[index].unit;
}

u16 cmpGetSelectItemID(void)
{
    return campWork->itemId[campWork->itemCursor + campWork->itemScroll];
}

INCLUDE_ASM(const s32, "code_0022C578", cmpSetItSkMessageVar);

extern s32 cmpGetSkillTargetArea(u16 skillId);

s32 cmpGetItSkErrSelFlag(u16 skillId)
{
    s32 flags = 0;
    s32 area = cmpGetSkillTargetArea(skillId);
    datUnitWork_t *unit;
    s32 i;

    switch (area)
    {
    case 1:
        flags = 1 << cmpGetSelectedStockIndex();
        break;
    case 2:
        unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
        for (i = 0; i < 16; i++)
        {
            if ((unit->flag & 2) != 0)
            {
                flags |= 1 << i;
            }
            unit++;
        }
        break;
    }
    return flags;
}

s32 cmpFindSkillMsgIndex(s32 which, u16 id)
{
    cmpMsgEntry_t *entry = D_00345DA0;
    u32 i;

    for (i = 0; i < 7; i++, entry++) {
        if (entry->id == id) {
            if (which == 0) {
                return entry->unk_02;
            }
            return entry->unk_04;
        }
    }
    return -1;
}

s32 cmpFindItemMsgIndex(s32 which, u16 id)
{
    cmpMsgEntry_t *entry = D_00345DD0;
    u32 i;

    for (i = 0; i < 7; i++, entry++) {
        if (entry->id == id) {
            if (which == 0) {
                return entry->unk_02;
            }
            return entry->unk_04;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpCreateLocalItem);

INCLUDE_ASM(const s32, "code_0022C578", cmpStartSkillTokusyuMsg);

void cmpStartItemTokusyuMsg(u16 item)
{
    cmpStartSkillTokusyuMsg(datItemTable[item].skill);
}

// 19 of 40 words match at the exact size; retail calls the tokusyu
// message before deciding the mode, this decides it first.
#ifdef NON_MATCHING
s32 cmpStartSkillMsg(u16 skill)
{
    s32 guide = cmpClearButtonGuide(skill);
    s32 mode;
    s32 index;

    if (guide >= 5)
    {
        goto zero;
    }
    if (guide > 0)
    {
        mode = 1;
        goto have_mode;
    }
zero:
    mode = 0;
have_mode:
    if (cmpStartSkillTokusyuMsg(skill) != 0)
    {
        return 2;
    }
    index = cmpFindSkillMsgIndex(mode, skill);
    if (index <= 0)
    {
        return mode;
    }
    if (mode == 0)
    {
        fclStartMessage(index + 0xC);
    }
    else
    {
        fclStartMessage(index + 0x20);
    }
    return mode;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpStartSkillMsg);
#endif

/* NON_MATCHING: 22 of 40 words differ at the exact size: msg lands in $2
 * instead of $8 and the whole register numbering shifts with it. */
#ifdef NON_MATCHING
extern s32 cmpFindKugatatiMsgEntry(datUnitWork_t *unit);
extern s32 cmbChkDevilEvo(datUnitWork_t *unit);

s32 cmpGetKugatatiMsg(void)
{
    datUnitWork_t *stock = cmpGetSelectDstStock();
    s32 entry = cmpFindKugatatiMsgEntry(stock);
    s32 evo = cmbChkDevilEvo(stock);
    s32 diff;
    s32 lvl;
    s32 msg;

    if (entry < 0)
    {
        return 27;
    }
    diff = entry - stock->level;
    lvl = ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->level;
    msg = 0;
    if (diff < 2)
    {
        msg = 26;
    }
    if (diff == 2)
    {
        msg = 28;
    }
    if (diff == 3)
    {
        msg = 29;
    }
    if (diff >= 4)
    {
        msg = 30;
    }
    if (lvl + 6 >= evo)
    {
        return msg;
    }
    return 31;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpGetKugatatiMsg);
#endif

INCLUDE_ASM(const s32, "code_0022C578", cmpStartItemMsg);

INCLUDE_ASM(const s32, "code_0022C578", cmpUseItem);

s32 cmpGetSelectSkillID(void)
{
    datUnitWork_t *unit =
        &cmbGlobalWork->party[cmpGetStockInfoIndex(campWork->stock, 0, 0)].unit;

    return unit->skill[campWork->itemCursor + campWork->itemScroll];
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUseSkill);

/* NON_MATCHING: 1 of 38 words differs, exact size: xori against retail's sltiu. */
#ifdef NON_MATCHING
extern s32 cmpGetSkillEntry(u16 id, datUnitWork_t *unit);

s32 cmpChkDoNotUseSkill(u16 skill)
{
    s32 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);
    u8 use;

    if (cmpGetSkillEntry(skill, (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index) == 0) {
        return -1;
    }
    use = datSkillTable.normal[skill].use & 1;
    if (use == 0) {
        return 1;
    }
    return skill >= 0x120;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpChkDoNotUseSkill);
#endif

/* Called above its definition, where ee-gcc assumed int. */
s8 cmpStartStockMsg(s8 flag);

s32 cmpItemIsStockFull(u16 id)
{
    if (id == 0x3A) {
        switch (cmpStartStockMsg(1)) {
        case 0:
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 13 of 28 words differ, 112 bytes against retail's 140. */
#ifdef NON_MATCHING
s32 cmpItemIsUsable(u16 item)
{
    u8 flags = datItemTable[item].flags;
    u8 usable = flags & 1;

    if (usable == 0) {
        return 1;
    }
    switch (campWork->page[0].cursor.unk_22 + (u8)campWork->page[0].cursor.unk_24) {
    case 0:
        if (cmpItemIsStockFull(item) != 0) {
            return 1;
        }
        return 0;
    case 1:
        if ((flags & 2) != 0) {
            return 0;
        }
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpItemIsUsable);
#endif

void cmpApplyStockRecover(void)
{
    s32 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);

    cmpSetStockFlag((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index);
    cmpRemoveStockSlot(cmpGetStockInfoIndex(&campWork->stock[0], 0, 1));
    cmpCompactStockList(index);
    datClearCommMemory((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index);
    datAllClearCommMemory();
}

extern void cmpClearStockFlag(datUnitWork_t *unit);

extern void datClearCommMemory(datUnitWork_t *work);

extern void datAllClearCommMemory(void);

void cmpApplyStockRemove(void)
{
    s8 index = cmpGetStockInfoIndex(campWork->stock, 0, 0);

    if (index < 0) {
        return;
    }
    cmpClearStockFlag(&cmbGlobalWork->party[index].unit);
    datClearCommMemory(&cmbGlobalWork->party[index].unit);
    datAllClearCommMemory();
}

INCLUDE_ASM(const s32, "code_0022C578", cmpRemoveStockDevilEntry);

void cmp_22E258(void *work)
{
    memset(work, 0, 0x2128);
}

s32 cmpInitSkillCursor(datUnitWork_t *unit)
{
    s32 n = 0;
    u16 *skill;

    if (unit->skillcnt != 0)
    {
        skill = &unit->skill[0];
        do
        {
            if (cmpChkDoNotUseSkill(*skill++) == 0)
            {
                return 1;
            }
            n++;
            cmpMoveCursor((cmpCursorInfo_t *)campWork->unk_410C, 1);
        } while (n < unit->skillcnt);
    }
    cmpMoveCursor((cmpCursorInfo_t *)campWork->unk_410C, 0);
    cmpMoveCursor((cmpCursorInfo_t *)campWork->unk_410C, 1);
    return 0;
}

void cmpInitStockCursorPos(void)
{
    s32 rows;

    rows = campWork->stock[0].PartyCnt + campWork->stock[0].unk_03[0];
    cmpInitCursorRange(campWork->unk_6230, (u8)rows, (u8)rows, 0);
    rows = campWork->stock[1].PartyCnt + campWork->stock[1].unk_03[0];
    cmpInitCursorRange((u8 *)campWork + 0x726C, (u8)rows, (u8)rows, 0);
}

/* NON_MATCHING: 6 of 33 words differ (first at +0x28). */
#ifdef NON_MATCHING
void cmpReCalcStockCursorPos(s8 index)
{
    if (index == 0 || index == 2) {
        cmpChangeCursorListNums(
            (campWork->stock[0].PartyCnt + campWork->stock[0].unk_03[0]) & 0xFF,
            &campWork->unk_6230[0]);
    }
    if (index == 1 || index == 2) {
        cmpChangeCursorListNums(
            (campWork->stock[1].PartyCnt + campWork->stock[1].unk_03[0]) & 0xFF,
            &campWork->unk_6230[0x103C]);
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpReCalcStockCursorPos);
#endif

void cmpInitRootCursorPos(void)
{
    cmpResetCursorInfo(&campWork->page[0].cursor);
    cmpResetCursorInfo(&campWork->page[1].cursor);
    cmpResetCursorInfo(&campWork->page[2].cursor);
}

void cmpInitSelectCursor(u8 index)
{
    cmpInitCursorRange(campWork->unk_410C, index, 8, 0);
    cmpInitCursorLayout(campWork->unk_410C, 0, 0, 0, 0, 0x17);
}

s8 camp_H_cmpLocalStock_t__ctor(void)
{
    cmpLocalStock_t stock;

    stock.count = 9;
    cmpBuildStockNameList(&stock);
    return stock.PartyCnt;
}

s8 cmpStartStockMsg(s8 flag)
{
    cmpLocalStock_t stock;

    stock.count = 0xA;
    if (flag != 0)
    {
        stock.count = 0xCB;
    }
    cmpBuildStockNameList(&stock);
    if (flag != 0)
    {
        return (u8)stock.PartyCnt + stock.unk_03[0];
    }
    return (s8)stock.unk_03[0];
}

void cmpCalcRoot(void)
{
    if (campWork->unk_0004 != 0) {
        cmpInitStockCursorPos();
        cmpInitRootCursorPos();
        cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, cmpUpdateListSelection,
                             (u8 *)campWork + 0x1C, 0, 0);
        cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
    }
}

void cmpSetSkillTSL(u16 skill)
{
    campWork->stock[1].count = 0x8F;
    cmpBuildStockNameList(&campWork->stock[1]);
    cmpReCalcStockCursorPos(1);
}

void cmpSetItemTSL(u16 itemId)
{
    u16 skill = datItemTable[itemId].skill;

    if (itemId == 0x3A) {
        campWork->stock[1].count = 0x8B;
        cmpBuildStockNameList(&campWork->stock[1]);
        cmpReCalcStockCursorPos(1);
        return;
    }
    cmpSetSkillTSL(skill);
}

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F4F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F500);

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcSkill);

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcDestroyItem);

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcItem);

INCLUDE_ASM(const s32, "code_0022C578", cmpSetPartyDSL);

s32 cmpCalcStockControl(void)
{
    switch (campWork->page[1].cursor.unk_22 + (u8)campWork->page[1].cursor.unk_24)
    {
    case 0:
        cmpApplyStockRecover();
        if (cmpStartStockMsg(0) == 0)
        {
            return 0;
        }
        campWork->stock[0].count = 0xA;
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(2);
        cmpInitDrawList();
        break;
    case 1:
        cmpApplyStockRemove();
        if (camp_H_cmpLocalStock_t__ctor() == 0)
        {
            return 0;
        }
        break;
    case 2:
        break;
    }
    cmpMoveStockCursor(0, 0, (u8 *)&campWork->stock[0]);
    return 1;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcParty);

extern void dds3ConfigProcessStart(s32 Mode);

void cmpCalcConfig(void)
{
    if (campWork->unk_0004 == 0) {
        return;
    }
    cmpSetStockVisible(0);
    cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, NULL, NULL, 0, 0);
    campWork->unk_8468 = 0x200;
    dds3ConfigProcessStart(0);
}

void cmpCalcLoad(void)
{
    if (campWork->unk_0004 == 0) {
        return;
    }
    cmpSetStockVisible(0);
    cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, NULL, NULL, 0, 0);
    campWork->unk_8468 = 0x200;
    fmProcTrigSequence(1);
}

extern s32 fclChkMessage(void);

extern void cmpCreateLocalItem(s32 mode);

extern void cmpChangeCursorListNums(s32 count, u8 *cursor);

/* NON_MATCHING: 1 of 27 words differ (first at +0x18). */
#ifdef NON_MATCHING
void cmpUpdateList(void)
{
    if (campWork->unk_8303 == 0) {
        return;
    }
    if (fclChkMessage() != 0) {
        return;
    }
    cmpCreateLocalItem(0);
    cmpChangeCursorListNums(campWork->unk_8302, campWork->unk_410C);
    campWork->unk_8303 = 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateList);
#endif

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcSequence);

INCLUDE_ASM(const s32, "code_0022C578", nmeMakeGradColors);

INCLUDE_ASM(const s32, "code_0022C578", nmeDrawQuadPanel);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F590);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F5A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F5C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F5D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F5F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F610);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F628);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F640);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F650);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F660);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F678);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F690);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F6A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F6B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F6D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053F6E8);

INCLUDE_ASM(const s32, "code_0022C578", nmeDrawGradQuad);

INCLUDE_ASM(const s32, "code_0022C578", nmeDrawBlendQuad);

s32 nmeDrawBox(s32 color, s32 unused, s32 flag)
{
    return cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, color, flag);
}

void nmeDrawSizedSprite(u8 id, u16 idx, s32 *texs, fclSpriteTbl_t *tbl, s32 color, s32 size,
                        s32 ot)
{
    s32 colors[4];
    s32 half = size / 2;

    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteSize(-(half * 16), -(half * 8), 0, size + 0x200, size + 0x1C0, colors, id, idx,
                      texs, tbl, ot);
}

INCLUDE_ASM(const s32, "code_0022C578", nmeLerp);

s32 nmeLerp2(s32 scale, s32 value, s32 range)
{
    s32 half = range / 2;
    s32 quarter = half / 2;
    s32 rem = value % half;

    if (rem >= quarter) {
        rem = half - rem;
    }
    return scale * rem / quarter;
}

INCLUDE_ASM(const s32, "code_0022C578", nmeDrawFrame);

INCLUDE_ASM(const s32, "code_0022C578", nmeDrawBlendText);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawHelp);

extern s32 cmpMenuTextTable[];

extern s32 cmpStartProcessInfo(s32 text, s32 flag);

void cmp_2304C0(s8 index)
{
    s32 *entry = &cmpMenuTextTable[index];

    if (cmpStartProcessInfo(*entry, 0) == 0) {
        func_0023B500(*entry);
    }
}

void cmpDisableInfoWindow(void)
{
    cmpSetInfoWindowHidden();
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawText);

INCLUDE_ASM(const s32, "code_0022C578", func_002306A0);

extern s32 cmpGetSkillTargetArea(u16 skillId);

void campSkillTargetIsSelf(u16 skillId, s8 value)
{
    if (cmpGetSkillTargetArea(skillId) == 1)
    {
        campWork->drawPhase = value;
    }
    else
    {
        campWork->drawPhase = 4;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockRow);

void campDrawSetPhase(void)
{
    campDrawText(1, 0);
    campWork->drawPhase = 2;
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockCursor);

INCLUDE_ASM(const s32, "code_0022C578", campDrawNumber);

INCLUDE_ASM(const s32, "code_0022C578", campDrawFade);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FB28);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FB50);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FB68);

INCLUDE_ASM(const s32, "code_0022C578", camp_draw_Process);

extern u32 dds3DebugSign;

extern void func_00242A70(s32 key);

s32 campDebugCheckPad(s8 key)
{
    if (dds3DebugSign == 0) {
        return 0;
    }
    if (campStatusIsActive() != 0) {
        return 1;
    }
    if (dds3PadButtonState[0x2D] < 0) {
        func_00242A70(key);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateRoot);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateItem);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateParty);

INCLUDE_ASM(const s32, "code_0022C578", campUpdateStockSelect);

INCLUDE_ASM(const s32, "code_0022C578", campUpdateStockCursor);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateSkillSelect);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateProcessStatus);

void cmpRemoveStockDevil(void)
{
    cmpRemoveStockDevilEntry();
    campWork->stock[0].count = 0x8B;
    cmpBuildStockNameList(&campWork->stock[0]);
    cmpInitDrawList();
    cmpReCalcStockCursorPos(2);
}

INCLUDE_ASM(const s32, "code_0022C578", campWaitSelMessage);

void campCloseInfoWindow(void)
{
    if (campInfoWorkExists() == 0) {
        campWork->unk_0001 = 0;
        campWork->unk_0004 = 2;
        cmpDisableInfoWindow();
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateDestroyItemConfirm);

extern s32 checkConfigState(void);

void campShowStock(void)
{
    if (checkConfigState() == 0)
    {
        cmpSetStockVisible(1);
        cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, cmpUpdateListSelection,
                             (u8 *)campWork + 0x1C, 0, 0);
        campWork->unk_0001 = 0;
        campWork->unk_0004 = 1;
    }
}

void cmpUpdateLoad(void)
{
    if (fmProcessChk() != 0)
    {
        return;
    }
    switch (slGetExitMode())
    {
    case 2:
        dds3FadeIn(0, 0, 0, 0xF);
        cmpSetLoadState(1);
        campWork->unk_0000 |= 0x10;
        cmpDisableInfoWindow();
        return;
    case 0:
        dds3FadeIn(0, 0, 0, 0);
        cmpSetStockVisible(1);
        cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, cmpUpdateListSelection,
                             (u8 *)campWork + 0x1C, 0, 0);
        campWork->unk_0001 = 0;
        campWork->unk_0004 = 1;
        break;
    }
}

void campUpdateEffectStep(void)
{
    campWork_t *work = campWork;

    work->effectStep++;
    if (work->effectStep >= 0x900) {
        work->effectStep = 0;
    }
}

s32 func_00232198(void)
{
    s32 i;

    for (i = 0; i < 0x10; i++) {
        if (campWork->stockDelay[i] > 0) {
            campWork->stockDelay[i]--;
        } else {
            if (campWork->stockAlpha[i] < 0x100) {
                campWork->stockAlpha[i] += campWork->stockStep[i];
                if (campWork->stockAlpha[i] < 0x100) {
                    continue;
                }
            }
            campWork->stockAlpha[i] = 0x100;
        }
    }
}

s32 campUpdateEffects(void)
{
    campUpdateEffectStep();
    return func_00232198();
}

INCLUDE_ASM(const s32, "code_0022C578", camp_update_Process);

s32 cmpStockEffIsActive(u8 *work)
{
    s8 state = work[0x2124];

    switch (state) {
    case 4:
        return 4;
    case 6:
        return 0;
    }
    return 1;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x14];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s8 unk_18;
    /* 0x19 */ u8 unk_19[3];
    /* 0x1C */ cmpLocalStock_t *stock;
} cmpStockSeqInfo_t;

void *cmpAllocStockProcess(cmpLocalStock_t *pNameList, s32 pParent)
{
    void *chunk = sdfMemAllocDirect(0x20);
    cmpStockSeqInfo_t *info = sdfMemLock(chunk);

    info->allocation = chunk;
    cmpInitSeqInfo(info, pParent);
    info->unk_18 = 0;
    info->stock = pNameList;
    pNameList->count = 0xF;
    cmpBuildStockNameList(pNameList);
    return info;
}

void camp_update_stock_Shutdown(void)
{
    if (campStatusPanel != NULL) {
        sdfMemFreeDirect(campStatusPanel->allocation);
        campStatusPanel = NULL;
    }
}

extern s32 camp_calc_stock_Process(s32 id);
extern s32 camp_draw_stock_Process(s32 id);
extern s32 camp_update_stock_Process(s32 id);

void *cmpStartProcessStock(cmpLocalStock_t *stock, s32 value)
{
    void *proc;

    if (campStatusPanel != NULL) {
        return NULL;
    }
    campStatusPanel = cmpAllocStockProcess(stock, value);
    proc = dds3StartProcess("camp_calc_stock", 0x3F2, 1, 1, camp_calc_stock_Process, NULL,
                            campStatusPanel);
    dds3StartProcess("camp_draw_stock", 0x2B07, 1, 1, camp_draw_stock_Process, NULL,
                     campStatusPanel);
    dds3StartProcess("camp_update_stock", 0x520B, 1, 1, camp_update_stock_Process,
                     camp_update_stock_Shutdown, campStatusPanel);
    return proc;
}

void campHideStatusWindow(void)
{
    if (campStatusPanel != NULL) {
        campStatusPanel->flags |= 0x10;
    }
}

void cmpSetStockVisible(s8 visible)
{
    if (campStatusPanel == NULL) {
        return;
    }
    if (visible != 0) {
        campStatusPanel->flags |= 2;
    } else {
        campStatusPanel->flags &= ~2;
    }
}

/* NON_MATCHING: 1 of 10 words match, 40 bytes against retail's 48; retail
 * zero-extends the returned byte with sll 24 / srl 24. */
#ifdef NON_MATCHING
u8 cmpClearStatusFlags(void)
{
    campStatusPanel_t *panel = campStatusPanel;

    if (panel != NULL) {
        panel->flags &= 2;
        return panel->flags;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpClearStatusFlags);
#endif

extern cmpInfoWindow_t *D_00551F9C;

s32 camp_calc_stock_Process(s32 id)
{
    cmpInfoWindow_t *window = dds3GetProcessData(id);

    D_00551F9C = window;
    if ((window->flags & 0x10) != 0) {
        cmpUpdateTerminate(window);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockPanel);

extern cmpInfoWindow_t *D_00551FA0;

extern void campDrawStockPanel(s32 panel, s32 mode);

s32 camp_draw_stock_Process(s32 id)
{
    cmpInfoWindow_t *window = dds3GetProcessData(id);

    D_00551FA0 = window;
    if ((window->flags & 0x20) != 0) {
        cmpUpdateTerminate(window);
        return -1;
    }
    if ((window->flags & 2) != 0) {
        campDrawStockPanel(window->unk_1C, 0);
    }
    return 0;
}

/* NON_MATCHING: 2 of 35 words differ, exact size; both are one register name. */
#ifdef NON_MATCHING
extern cmpStockSeqInfo_t *cmpStockUpdateState;

s32 camp_update_stock_Process(s32 id)
{
    cmpGetPressedButton(0x30);
    cmpStockUpdateState = dds3GetProcessData(id);
    if ((cmpStockUpdateState->unk_00[0] & 0x40) != 0) {
        return -1;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)cmpStockUpdateState->stock + 0xAC), 0);
    }
    cmpUpdateCursorSlots((cmpCursorInfo_t *)((u8 *)cmpStockUpdateState->stock + 0xAC));
    cmpUpdateCursorSlots((cmpCursorInfo_t *)((u8 *)cmpStockUpdateState->stock + 0x10E8));
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", camp_update_stock_Process);
#endif

void cmpSetDHEntry(void) {
}

extern s32 cmpHeartsIconIsVisible(s8 index);

s32 datCountRegisteredHearts(void)
{
    s32 i;

    for (i = 1; i < 0x19; i++) {
        if (cmpHeartsIconIsVisible(i) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 cmpHeartsIconIsVisible(s8 id)
{
    s32 i;

    for (i = 0; i < ((u8 *)cmbGlobalWork)[0x1D7B]; i++) {
        if (cmbGlobalWork->unk_01CFB[i] == id) {
            return 1;
        }
    }
    return 0;
}

void cmpInitHeartsIconTable(campInfoWork_t *work)
{
    u32 i;

    for (i = 0; i < 0x1B; i++) {
        cmpGetHeartsIconId((s8)i);
        if (cmpGetHeartsIconId((s8)i) == DDS3_GBWK->heartsequip) {
            work->unk_31 = i;
            return;
        }
    }
    work->unk_31 = 0;
}

s8 cmpGetHeartsIconId(s8 index)
{
    return D_00345E58[index].id;
}

s8 cmpGetHeartsIconType(s8 index)
{
    return D_00345E58[index].type;
}

s32 cmpGetHeartsIconColor(s8 index)
{
    return D_00345E58[index].color;
}

s32 campDhCheckReady(void)
{
    if (campInfoWork->unk_116C == 0) {
        return 1;
    }
    if (campInfoWork->unk_1170 == 0) {
        return 1;
    }
    return campInfoWork->unk_1174 == 0;
}

s32 campDhIconIsReady(void)
{
    if (campInfoWork->unk_1160 == 0) {
        return 0;
    }
    if (campInfoWork->unk_1164 == 0) {
        return 0;
    }
    return campInfoWork->unk_1168 != 0;
}

/* NON_MATCHING: 136 emitted bytes against retail's 140; one register copy short. */
#ifdef NON_MATCHING
extern char D_0053FCE8[];

void campDhCheckTextures(void)
{
    s32 ofs;
    s32 i;

    for (i = 0, ofs = 0x1160; i < 3; i++, ofs += 4) {
        void **tex = (void **)((u8 *)campInfoWork + ofs + 0xC);

        if (*tex != NULL && *(s32 *)((u8 *)campInfoWork + ofs) == 0) {
            *(s32 *)((u8 *)campInfoWork + ofs) = sdfTexSet(*tex);
            printf(D_0053FCE8, i);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDhCheckTextures);
#endif

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x116C];
    /* 0x116C */ s32 texHandle[3];
} cmpDhWork_t;

s32 cmpLoadDHTextures(cmpDhWork_t *work)
{
    fclLoadFileAsync("/camp/tmx/camp_dh1.tmx", &work->texHandle[0]);
    fclLoadFileAsync("/camp/tmx/camp_dh2.tmx", &work->texHandle[1]);
    fclLoadFileAsync("/camp/tmx/camp_dh3.tmx", &work->texHandle[2]);
    return printf("Read hearts tex.\n");
}

extern char D_0053FD60[];

void campDhFreeTexture(campInfoWork_t *work)
{
    if (work->unk_1160 != 0) {
        sdfTexFree(work->unk_1160);
        work->unk_1160 = 0;
    }
    if (work->unk_1164 != 0) {
        sdfTexFree(work->unk_1164);
        work->unk_1164 = 0;
    }
    if (work->unk_1168 != 0) {
        sdfTexFree(work->unk_1168);
        work->unk_1168 = 0;
    }
    printf(D_0053FD60);
}

extern void campDhFreeTexture(campInfoWork_t *work);

extern void sdfMemFreePtrDirect(void *aPtr);

extern char D_0053FD78[];

void campDhFreeResources(campInfoWork_t *work)
{
    s32 *tex;
    s32 i;

    campDhFreeTexture(work);
    tex = &work->unk_116C;
    for (i = 0; i < 3; i++)
    {
        sdfMemFreePtrDirect((void *)tex[i]);
        tex[i] = 0;
    }
    printf(D_0053FD78);
}

s32 func_00232C08(void)
{
    return campInfoWork->unk_01 == 0;
}

extern u32 pstr[];

extern u32 pstr[];

s32 cmpSetHeartsSequenceRemove(void)
{
    if (cmbGlobalWork->equipHearts == 0)
    {
        return 0;
    }
    if ((cmbGlobalWork->party[0].unit.badstatus & 0x8000) == 0)
    {
        campInfoWork->unk_18 = 0;
        fclSetMessageVar_20743088(0xC, pstr[cmbGlobalWork->equipHearts]);
        fclSetMessageInvalid(0);
        fclStartMessage(4);
        campInfoWork->unk_01 = 3;
        campInfoWork->unk_04 = 1;
        return 1;
    }
    fclSetMessageInvalid(1);
    fclStartMessage(1);
    campInfoWork->unk_01 = 4;
    campInfoWork->unk_04 = 1;
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpInitHeartsIcon);

void cmpInitDHWindow(u8 *base)
{
    cmpDHWindow_t *dh = (cmpDHWindow_t *)(base + 0x1F78);

    dh->unk_08 = 0x20;
    dh->unk_10 = 0x10;
    dh->unk_04 = 0;
    dh->unk_09 = 0;
    dh->unk_0A = 0;
    dh->unk_0C = 0;
    dh->unk_00 = 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpinitDH);

extern void campDhFreeResources(campInfoWork_t *work);

void camp_update_dh_Shutdown(void)
{
    if (campInfoWork != NULL) {
        fclSetSelMessagePos(campInfoWork->unk_30);
        cmpSetStockVisible(1);
        cmpDisableInfoWindow();
        campDhFreeResources(campInfoWork);
        sdfMemFreeDirect(campInfoWork->allocation);
        campInfoWork = NULL;
    }
}

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FD60);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FD78);

INCLUDE_ASM(const s32, "code_0022C578", cmpStartProcessDH);

s32 campInfoWorkExists(void)
{
    return campInfoWork != NULL;
}

void campHideInfoWindow(void)
{
    if (campInfoWork != NULL) {
        campInfoWork->unk_00 |= 0x10;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpSetStatusStartEff);

INCLUDE_ASM(const s32, "code_0022C578", camp_calc_dh_Process);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawBox);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawBoxSprite);

void campDhDrawGaugeBars(void)
{
    s32 corner[4];

    corner[0] = 0;
    corner[1] = 0x101E80;
    corner[2] = 0;
    corner[3] = 0x101E80;
    cmpDrawBoxGDefault(0x5C0, 0x948, 0, 0xAC, 0x74, corner, 0x47);
    corner[0] = 0x101E80;
    corner[1] = 0x101E80;
    corner[2] = 0x101E80;
    corner[3] = 0x101E80;
    cmpDrawBoxGDefault(0x1080, 0x948, 0, 0xF8, 0x74, corner, 0x47);
}

void fclCalcArrowCursorOffset(s8 index, s32 *x, s32 *y)
{
    s8 col;

    if (index != 0x18) {
        col = index % 8;
        *x = col * 55;
        *y = (index / 8) * 58;
    } else {
        *x = 0xC0;
        *y = 0xAE;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", fclGetArrowCursorTarget);

INCLUDE_ASM(const s32, "code_0022C578", fclDrawArrowCursorIcon);

INCLUDE_ASM(const s32, "code_0022C578", fclDrawArrowCursorBox);

INCLUDE_ASM(const s32, "code_0022C578", fclGetArrowCursorEntry);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawBackground);

INCLUDE_ASM(const s32, "code_0022C578", fclDrawArrowCursorLabel);

INCLUDE_ASM(const s32, "code_0022C578", fclDrawArrowCursor);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawPanel);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawQuadGauge);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawHelpTextAlt);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawHelpText);

INCLUDE_ASM(const s32, "code_0022C578", campDhDrawHeartsGauge);

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x1B];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20[0xC];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 iconIndex;
} campDhWork_t;

extern campDhWork_t *campDhDrawWork;

extern void campDhDrawHeartsGauge(void *gauge, s8 arg);

void campDhDrawHeartsIcon(s32 a, s32 b, s32 c, s8 d, s32 e)
{
    campDhWork_t *work = campDhDrawWork;

    if ((work->flags & 1) == 0) {
        if (*((u8 *)DDS3_GBWK + 0x1CFA) == 0) {
            return;
        }
    }
    campDhDrawHeartsGauge((u8 *)work + 0x1F78, d);
}

/* NON_MATCHING: 2 of 28 words differ (first at +0x38). */
#ifdef NON_MATCHING
void campDhDrawIcon(void)
{
    campDhDrawBoxSprite(campDhDrawWork->unk_1C);
    if (campDhIconIsReady() != 0) {
        campDhDrawPanel(0, 0, 0, 0x100);
        campDhDrawHeartsIcon(0, 0, 0, cmpGetHeartsIconId(campDhDrawWork->iconIndex), 0x100);
    }
    campDhDrawBox(campDhDrawWork->fadeAlpha);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDhDrawIcon);
#endif

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

void campDhDrawHearts(void)
{
    u8 hearts[6];
    s32 i;

    for (i = 0; i < 6; i++) {
        hearts[i] = cmbGetHeartsParam(DDS3_GBWK->heartsequip, (s8)i);
    }
    cmpDrawStatusComEx((s32)((u8 *)DDS3_GBWK + 0xA54), 0x80, -1, 0, (s32)hearts, 0, 0xA09DC380,
                       4, 4, 0);
}

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FDC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FDD8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FDE8);

INCLUDE_ASM(const s32, "code_0022C578", camp_draw_dh_Process);

INCLUDE_ASM(const s32, "code_0022C578", campDhApplyEntry);

s32 campDhFindEntry(void)
{
    s8 id = cmpGetHeartsIconId(campDhUpdateWork->unk_31);
    s8 i;

    if (cmpHeartsIconIsVisible(id) == 0)
    {
        return 1;
    }
    if (cmbGlobalWork->equipHearts != 0)
    {
        if (id == cmbGlobalWork->equipHearts)
        {
            return 2;
        }
        for (i = 0; i < ((u8 *)cmbGlobalWork)[0x1D7B]; i++)
        {
            if (id == cmbGlobalWork->unk_01CFB[i])
            {
                if ((cmbGlobalWork->party[0].unit.badstatus & 0x8000) != 0)
                {
                    return 4;
                }
            }
        }
    }
    else if ((cmbGlobalWork->party[0].unit.badstatus & 0x8000) == 0)
    {
        return 3;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpReturnParamDH);

extern void datEquipHearts(s32 HeartsID);

void campDhEquipHearts(void)
{
    u8 *gbwk;

    datEquipHearts(cmpGetHeartsIconId(campDhUpdateWork->unk_31));
    gbwk = (u8 *)DDS3_GBWK;
    if (*(u16 *)(gbwk + 0xA5A) == 0) {
        *(u16 *)(gbwk + 0xA5A) = 1;
    }
    if (*(u16 *)(gbwk + 0xA5E) == 0) {
        *(u16 *)(gbwk + 0xA5E) = 1;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", campDhUpdateCursor);

s32 cmpUnloadStatusModel(void)
{
    cmpModelFreeResource();
    while (sdfDelayedResrcCheck() != 0) {
    }
    cmpResetDrawEnv();
    campDhUpdateWork->unk_01[0] = 0;
    campDhUpdateWork->unk_04 = 1;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateHeartsAttachStatus);

/* campDhUpdateWork, as this unit's animation and icon code reads it. The two
 * were separate structs whose 0x28-byte prefixes agreed, so they are one here.
 * A third view, campStatusPanel_t *campDhUpdateWork, is declared above: the
 * alias is what lets both spellings of the one object coexist, and unifying
 * them is type recovery on the real allocation, not a rename. */
typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1C];
    /* 0x001C */ s32 unk_1C;
    /* 0x0020 */ s32 unk_20;
    /* 0x0024 */ s32 unk_24;
    /* 0x0028 */ s32 unk_28;
    /* 0x002C */ u8 unk_002C[0x114C];
    /* 0x1178 */ s32 iconTimer[64];
} campDhTimerWork_t;

extern campDhTimerWork_t *campDhTimerWork asm("campDhUpdateWork");

void cmpStepHeartsAttachCursor(s32 step)
{
    s32 period = 20;
    s32 phase;
    s32 parts;

    if (step == 0)
    {
        campDhTimerWork->unk_28 = 0;
    }
    else
    {
        if (campDhTimerWork->unk_28 >= 0x13)
        {
            return;
        }
        campDhTimerWork->unk_28++;
    }
    phase = campDhTimerWork->unk_28 % period;
    if (phase == 0)
    {
        dds3PadVibrationSet(1, 0x80, 0x14);
    }
    parts = 3;
    if (phase == period / parts)
    {
        dds3PadVibrationSet(1, 0x40, 0x14);
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateHeartsAttachConfirm);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateHeartsRemoveConfirm);

s32 campDhWaitMessageEnd(void)
{
    s32 busy = fclChkMessage();

    if (busy != 0) {
        return busy;
    }
    cmpReturnParamDH();
    return cmpUnloadStatusModel();
}


void campDhAdvanceAnim(void)
{
    campDhTimerWork->unk_1C++;
    if (campDhTimerWork->unk_1C >= 0x400) {
        campDhTimerWork->unk_1C = 0;
    }
    campDhTimerWork->unk_20 += 8;
    if (campDhTimerWork->unk_20 >= 0x200) {
        campDhTimerWork->unk_20 = 0;
    }
    if (campDhTimerWork->unk_24 < 0x200) {
        campDhTimerWork->unk_24 += 0x20;
    } else {
        campDhTimerWork->unk_24 += 0x10;
    }
    if (campDhTimerWork->unk_24 >= 0x400) {
        campDhTimerWork->unk_24 = 0;
    }
}

void campDhFadeAlpha(void)
{
    campStatusPanel_t *work = campDhUpdateWork;
    s32 alpha;

    if ((work->flags & 1) == 0) {
        alpha = work->fadeAlpha + 0x20;
        work->fadeAlpha = alpha;
        if (alpha >= 0x101) {
            work->fadeAlpha = 0x100;
        }
    } else {
        alpha = work->fadeAlpha - 0x20;
        if (alpha < 0) {
            alpha = 0;
        }
        work->fadeAlpha = alpha;
    }
}

void campDhUpdateIcons(void)
{
    s32 i;

    for (i = 0; i < 64; i++) {
        if (campDhTimerWork->iconTimer[i] > 0) {
            campDhTimerWork->iconTimer[i] -= 0x10;
        }
        if (campDhTimerWork->iconTimer[i] < 0) {
            campDhTimerWork->iconTimer[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "code_0022C578", campDhRollRatio);

typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02[0xE];
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ u8 unk_1C[4];
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ u8 unk_24[4];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
} campDhEntry_t;

extern s32 campDhRollRatio(void);

INCLUDE_ASM(const s32, "code_0022C578", campDhGetEntryRatio);

/* NON_MATCHING: 4 of 38 words differ, exact size; all four are a register pair. */
#ifdef NON_MATCHING
void campDhScanEntries(void)
{
    s32 ofs;
    s32 i;

    for (ofs = 0, i = 0x18; i >= 0; i--, ofs += 0x34) {
        campDhEntry_t *entry = (campDhEntry_t *)((u8 *)campDhUpdateWork + ofs + 0x1278);

        if (entry->unk_01 < 0) {
            continue;
        }
        campDhGetEntryRatio(entry);
        if (entry->unk_10 > 0) {
            entry->unk_10 -= 1;
            continue;
        }
        if ((entry->unk_2C = entry->unk_2C + entry->unk_30) >= 0x201) {
            entry->unk_2C = 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDhScanEntries);
#endif

INCLUDE_ASM(const s32, "code_0022C578", campDhFindVisibleIcon);

INCLUDE_ASM(const s32, "code_0022C578", camp_update_dh_Process);

extern void cmpSetSkillListRows(s32 rows);

void campDrawSkillPanel(void)
{
    cmpSetSkillListRows(0x14);
    campDrawText(0, 0);
    cmpSetSkillListRows(-1);
    campWork->drawPhase = 0;
}

void cmbSetBirthPanel(u32 panel)
{
    cmbBirthPanel = panel;
}

void rstSetHeartsMasterPhase(void)
{
    rstHeartsMasterPhase = 0x200;
}

void rstClearHeartsMasterPhase(void)
{
    rstHeartsMasterPhase = 0;
}

u8 rst_235DA0(void)
{
    return rstHeartsMasterPhase;
}

void rstSetSkillActFlag(u8 flag)
{
    rstSkillActFlag = flag;
}

void rstClearFlag(s8 index)
{
    cmpParamGaugeState[index] = 0;
}

s8 func_00235DC8(s8 index)
{
    return cmpParamGaugeState[index];
}

extern s32 cmpStatusAnimCount[];

extern s32 cmpStatTimer;

s32 cmpStatSetTimer(s32 kind, s8 index)
{
    cmpStatusAnimCount[index] = kind;
    cmpParamGaugeState[index] = 1;
    cmpStatTimer = 0;
    memset(D_00574A48, 0, 0x14);
    return cmpStatusAnimCount[index];
}

void func_00235E40(s8 index)
{
    D_00574A48[index] = 1;
}

s32 func_00235E60(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        if (D_00574A48[i] != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpGetGaugeBlinkAlpha);

s32 cmpGetPanelAlphaOrDefault(u8 *alpha, s32 color)
{
    s32 value = 0x80;

    if (alpha != NULL) {
        value = *alpha;
    }
    return (color & ~0xFF) | value;
}

INCLUDE_ASM(const s32, "code_0022C578", rstDrawNumber);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusIconRow);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusIcons);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusName);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusGauge);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusRow);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawParamGauge);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawParamPanel);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusValue);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusBars);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillRowText);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillIcons);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkill);

extern void cmpDrawStatusComEx(s32 a, u8 alpha, s8 b, s8 c, s32 d, s8 e, s32 color, s8 f,
                               s8 g, s8 h);

void cmpDrawStatusCom(s32 a, s8 b, s8 c, s8 d)
{
    cmpDrawStatusComEx(a, 0x80, -1, 0, 0, 0, 0xA09DC380, b, c, d);
}

extern void cmpDrawStatusComEx2(s32 a, s32 b, s32 c, s32 alpha, s32 d, s32 e, s32 f, s32 g,
                                s32 color, s32 h, s32 i, s32 j, s32 k);

void cmpDrawStatusComEx(s32 a, u8 alpha, s8 b, s8 c, s32 d, s8 e, s32 color, s8 f, s8 g, s8 h)
{
    cmpDrawStatusComEx2(a, 0, 0, alpha, b, c, d, e, color, f, g, h, 1);
}

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);

void cmpCalcTargetDevil(datUnitWork_t *src, datUnitWork_t *dst)
{
    datUnitWork_t tmp;

    cmbCopyCustomDevilToStock(dst, src);
    if ((src->flag & 0x200) != 0) {
        cmbCopyDefaultDevilToStock(src->id, &tmp);
        dst->level = tmp.level;
    }
}

extern s32 cmpStatusAnimCount[];

void cmpAdvanceStatusAnim(void)
{
    if (cmpStatusAnimCount[0] < 0x30) {
        cmpStatusAnimCount[0]++;
    } else {
        cmpStatusAnimCount[0] = 0;
    }
    if (cmpStatusAnimCount[1] < 0x3C) {
        cmpStatusAnimCount[1]++;
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusComEx2);

extern char *pStr[];

extern void *frwGetSmallFontFRQ2(s32 X, s32 Y, s32 Pri, s32 Color, char *pStr, s32 pFRQ);

void cmpDrawItemListHeader(s32 x, s32 y, s32 c, u16 index)
{
    void *frq = frwGetSmallFontFRQ2(x, y, 0xFFFFFF, c, pStr[index], 0);

    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawItemListScrollBar);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawItemListRow);

INCLUDE_ASM(const s32, "code_0022C578", cmpCalcItemEntry);

void campDrawInfoPanel(void)
{
    campDrawText(4, 0);
    campWork->drawPhase = 0;
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawSeq3);

extern char *text1[];

void cmpDrawSkillListHeader(s32 x, s32 y, s32 c, u16 index)
{
    void *frq;

    cmpSetSkillListRows(0x12);
    frq = frwGetSmallFontFRQ2(x, y, 0xFFFFFF, c, text1[index], 0);
    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

u8 cmpGetSkillIcon(u16 skill)
{
    return datSkillTable.normal[skill].costtype;
}

u16 cmpGetSkillCost(u16 skill, u16 *work)
{
    u16 cost = datSkillTable.normal[skill].cost;
    u16 base = datSkillTable.normal[skill].costbase;

    if (cmpGetSkillIcon(skill) == 1) {
        cost = base + work[4] * cost / 100;
    }
    return cost;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillListRow);

void cmpDrawSkillListScrollBar(s32 a, cmpCursorInfo_t *cursor)
{
    if (cursor->unk_20 != 0) {
        cmpDrawScrollBar(0, 0x280, 0x10, 0xA4, 0x16, 0x80808080, a, cursor->unk_24, cursor,
                         0, 0x47);
    }
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillList);

void cmpExpandColor(u64 *dst, u32 color)
{
    u64 wide = color;

    dst[0] = ((color >> 24) & 0xFF) | ((u64)((color >> 16) & 0xFF) << 32);
    dst[1] = ((wide & 0xFF00) >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSetup3DView);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSpriteFlag);

s32 func_00239C08(u8 id, s32 arg)
{
    return cmpDrawSpriteFlag(id, 0, arg);
}

s32 cmpDrawSprite(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x47;
    } else {
        prim[1] = 0x48;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    return ((s32 (*)(sdfGrOt_t *, void *))otp->regFunc)(otp, tag);
}

s32 fclDrawPanelSprite(s32 id, s32 arg)
{
    return cmpDrawSprite(id, 0, arg);
}

s32 cmpDrawSprite2(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x42;
    } else {
        prim[1] = 0x43;
    }
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    return ((s32 (*)(sdfGrOt_t *, void *))otp->regFunc)(otp, tag);
}

s32 fclDrawPanelSprite2(s32 id, s32 arg)
{
    return cmpDrawSprite2(id, 0, arg);
}

void cmpDrawSpriteIndexed(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x49;
    prim[0] = id;
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawSpriteIndexed2(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x14;
    prim[0] = id;
    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, packet);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void func_00239F88(s32 *work, s32 value)
{
    work[0] = value;
    work[1] = value;
    work[2] = value;
    work[3] = value;
}

s32 cmp_239FA0(s32 a, s32 b, s32 c, s32 color)
{
    s32 colors[3];

    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    return cmpDrawTriG(a, b, c, colors);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawTriG);

s32 cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h)
{
    s32 colors[4];

    colors[3] = color;
    colors[1] = color;
    colors[2] = color;
    colors[0] = color;
    return cmpDrawBoxG(a, b, c, d, e, colors, g, h);
}

s32 cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    return cmpDrawBoxSolid(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawBoxG);

s32 cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *f, s32 g)
{
    return cmpDrawBoxG(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "code_0022C578", cmp_23A360);

void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    s32 pair[2];

    pair[1] = g;
    pair[0] = g;
    cmpDrawBoxLine(a, b, c, d, e, f, pair);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawBoxLine);

INCLUDE_ASM(const s32, "code_0022C578", cmpDraw3DPoly);

void cmpDisableRGB(s32 ot)
{
    sdfGrOt_t *otp;
    void *tag;
    void *mask;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    mask = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(tag, (u32)mask);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpAllocRefGifTag(s32 ot)
{
    sdfGrOt_t *otp;
    void *tag;
    void *mask;

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    mask = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(tag, (u32)mask);
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmp_23A8B0(s32 arg)
{
    cmpDisableRGB(arg);
    fclDrawPanelSprite(0x1001, arg);
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0, arg);
    fclDrawPanelSprite(0x50000, arg);
    func_00239C08(1, arg);
}

/* NON_MATCHING: 4 of 16 words differ (first at +0x28). */
#ifdef NON_MATCHING
s32 cmp_23A918(s32 arg)
{
    func_00239C08(0, arg);
    cmpAllocRefGifTag(arg);
    return fclDrawPanelSprite(0x5C000, arg);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmp_23A918);
#endif

void func_0023A958(f32 *pos)
{
    aViewEye[0] = pos[0];
    aViewEye[1] = pos[1];
    aViewEye[2] = pos[2];
    aViewEye[3] = 0.0f;
}

void func_0023A980(f32 *pos)
{
    aViewTarget[0] = pos[0];
    aViewTarget[1] = pos[1];
    aViewTarget[2] = pos[2];
    aViewTarget[3] = 0.0f;
}

extern void mdlSetPosition(void *aHandle);

void func_0023A9A8(f32 *pos, void *model)
{
    SDF_VU0_SETVECTOR_X(pos[0]);
    SDF_VU0_SETVECTOR_Y(pos[1]);
    SDF_VU0_SETVECTOR_Z(pos[2]);
    mdlSetPosition(model);
}

void func_0023A9F0(f32 *rot, void *model)
{
    FromEuler(rot[0], rot[1], rot[2]);
    mdlSetRotate(model);
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x10];
} cmpModelLink_t;

extern cmpModelLink_t D_002F69B8[];

extern void mdlLink(void *aHandle, cmpModelLink_t *aModelOt);

void cmp_23AA28(f32 *a, f32 *rot, void *model, s32 index)
{
    func_0023A9A8(a, model);
    func_0023A9F0(rot, model);
    mdlLink(model, &D_002F69B8[index]);
}

void func_0023AA88(s32 value)
{
    D_0054FEBC = value;
}

s32 func_0023AA90(void)
{
    if (dds3FadeCheck() != 0) {
        return 0;
    }
    return D_0054FEBC;
}

cmpSeqFunc_t cmp_23AAC0(s32 value)
{
    func_0023AA88(value);
    dds3FadeIn(0, 0, 0, 0xF);
    return func_0023AA90;
}

cmpSeqFunc_t cmp_23AAF8(s32 value)
{
    func_0023AA88(value);
    dds3FadeOut(0, 0, 0, 0xF);
    return func_0023AA90;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawDebugPrint);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawFrameRect);

s8 cmpGetStatusModelFlag(void)
{
    return cmpStatusModelFlag;
}

void cmpSetStatusModelFlag(s8 flag)
{
    cmpStatusModelFlag = flag;
}

s8 cmpGetStatusModelLoadState(void)
{
    return cmpStatusModelLoadState;
}

void cmpSetModelReloadFlag(void)
{
    cmpModelInitCore2();
    cmpStatusModelLoadState = 1;
}

void cmpClearStatusModelLoadState(void)
{
    cmpStatusModelLoadState = 0;
}

s32 func_0023AD30(void)
{
    campStatusPanel_t *panel = cmpStatusPhase;

    if (panel == NULL) {
        return -1;
    }
    return cmbGlobalWork->party[panel->slot[panel->slotCursor]].unit.id;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpInitStatus);

void camp_update_status_Shutdown(void)
{
    cmpModelFreeResource();
    cmpResetDrawEnv();
    if (cmpStatusPhase != NULL) {
        cmpSetStockVisible(1);
        sdfMemFreeDirect(cmpStatusPhase->allocation);
        cmpStatusPhase = NULL;
    }
}

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FEB8);

INCLUDE_ASM(const s32, "code_0022C578", cmpStartProcessStatus);

void cmpSetStatusWindowHidden(void)
{
    if (cmpStatusPhase != NULL) {
        cmpStatusPhase->flags |= 0x10;
        cmpStatusWindowHidden = 1;
    }
}

extern void campFreeStatusModel(void);

s32 camp_calc_status_Process(s32 id)
{
    cmpStatusPhase = dds3GetProcessData(id);
    if (cmpStatusWindowHidden >= 2) {
        if (cmpStatusWindowHidden == 3) {
            cmpStatusWindowHidden = 0;
        } else {
            cmpStatusWindowHidden++;
        }
    }
    campFreeStatusModel();
    if ((cmpStatusPhase->flags & 0x10) == 0) {
        return 0;
    }
    if (cmpLoadStatusModel() != 0) {
        return 0;
    }
    cmpStatusWindowHidden = 0;
    cmpUpdateTerminate((cmpInfoWindow_t *)cmpStatusPhase);
    return -1;
}

s32 camp_draw_status_Process(s32 id)
{
    datUnitWork_t *unit =
        (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + cmpStatusPhase->slot[cmpStatusPhase->slotCursor];

    cmpStatusPhase = dds3GetProcessData(id);
    if (cmpStatusWindowHidden != 0)
    {
        return 0;
    }
    if ((cmpStatusPhase->flags & 0x20) != 0)
    {
        cmpUpdateTerminate((cmpInfoWindow_t *)cmpStatusPhase);
        return -1;
    }
    cmpDrawStatusCom((s32)unit, 0, 0, 0);
    return 0;
}

void campFreeStatusModel(void)
{
    s32 flag = cmpGetStatusModelFlag();

    if (flag != 0 && flag != 3) {
        return;
    }
    if (cmpGetStatusModelLoadState() == 0) {
        return;
    }
    cmpModelFreeResource();
    cmpSetStatusModelFlag(1);
    cmpClearStatusModelLoadState();
    flag = 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateStatusModelLoad);

INCLUDE_ASM(const s32, "code_0022C578", camp_update_status_Process);

void *cmpAllocInfoProcess(void *param, s32 pParent)
{
    void *chunk = sdfMemAllocDirect(0x1C);
    cmpSeqInfo_t *info = sdfMemLock(chunk);

    info->allocation = chunk;
    cmpInitSeqInfo(info, pParent);
    info->owner = param;
    return info;
}

void camp_update_info_Shutdown(void)
{
    if (cmpInfoWindow != NULL) {
        sdfMemFreeDirect(cmpInfoWindow->allocation);
        cmpInfoWindow = NULL;
    }
}

extern s32 camp_calc_info_Process(s32 id);
extern s32 camp_draw_info_Process(s32 id);
extern s32 camp_update_info_Process(s32 id);

s32 cmpStartProcessInfo(s32 text, s32 flag)
{
    s32 proc;

    if (cmpInfoWindow != NULL) {
        return 0;
    }
    cmpInfoWindow = cmpAllocInfoProcess((void *)text, flag);
    proc = (s32)dds3StartProcess("camp_calc_info", 0x3F2, 1, 1, camp_calc_info_Process, NULL,
                                 cmpInfoWindow);
    dds3StartProcess("camp_draw_info", 0x2B07, 1, 1, camp_draw_info_Process, NULL, cmpInfoWindow);
    dds3StartProcess("camp_update_info", 0x520B, 1, 1, camp_update_info_Process,
                     camp_update_info_Shutdown, cmpInfoWindow);
    return proc;
}

void cmpSetInfoWindowHidden(void)
{
    if (cmpInfoWindow != NULL) {
        cmpInfoWindow->flags |= 0x10;
    }
}

void func_0023B500(s32 value)
{
    if (cmpInfoWindow != NULL) {
        cmpInfoWindow->unk_18 = value;
    }
}

s32 camp_calc_info_Process(s32 id)
{
    cmpInfoWindow_t *window = dds3GetProcessData(id);

    cmpInfoWindow = window;
    if ((window->flags & 0x10) != 0) {
        cmpUpdateTerminate(window);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", camp_draw_info_Process);

s32 camp_update_info_Process(s32 id)
{
    cmpInfoWindow = dds3GetProcessData(id);
    if ((cmpInfoWindow->flags & 0x40) != 0) {
        return -1;
    }
    return 0;
}

extern void cmpSetAfterCrossFunc(void *panel, s32 b, s32 c, void *func, void *data, s32 f,
                                 s32 g);

extern void campInitPanelPos(void *panel, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

void camp_H_cmpGlobalWork_t__ctor(void)
{
    cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, NULL, NULL, 0, 0);
    campInitPanelPos((u8 *)campWork + 0x8448, 0, 0, 0, 0, 0, 0);
}

void cmpSetCrossGeneralVal(u8 *work, s8 value)
{
    work[0x50] = value;
}

extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanelPos_t *pPanelPos);

void campInitPanelPos(void *panel, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    u8 *work = panel;

    fclInitPanelPos(b, c, 0x10, 0, 0, 0, (fclPanelPos_t *)(work + 4));
    *(s32 *)work = d;
    *(s32 *)(work + 0x20) = g;
    *(s32 *)(work + 0x18) = e;
    *(s32 *)(work + 0x1C) = f;
    work[0x24] = 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpSetAfterCrossFunc);

INCLUDE_ASM(const s32, "code_0022C578", campRunPanelCallback);

void fclInitCursorWindow(void *work, s32 flag, u8 count)
{
    cmpCursorInfo_t *cursor = (cmpCursorInfo_t *)((u8 *)work + 4);

    cmpInitCursorRange(cursor, count, count, flag);
    cmpInitCursorLayout(cursor, 0, 0, 0, 0, 0x17);
}

void fclSetCursorList(void **work, void *list)
{
    work[1] = list;
}

s32 cmpCursorBlinkColor(s32 timer)
{
    if (timer < 0x101) {
        return ((timer << 7) >> 8) | 0x80808000;
    }
    return (((0x200 - timer) << 7) >> 8) | 0x80808000;
}

s32 cmpCursorFadeChannel(s32 color, s32 timer, s32 rate)
{
    s32 level = color & 0xFF;

    if (timer >= 0x101) {
        timer = 0x200 - timer;
    }
    level -= ((rate * timer) << 3) >> 8;
    if (level < 0) {
        level = 0;
    }
    return (color & ~0xFF) | level;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListItemBox);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListItemText);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListFrame);

s32 cmpGetBlinkOffset(s32 kind, cmpPanelEff_t *eff)
{
    s32 phase = eff->unk_34;

    if (kind != 0x100) {
        return 0;
    }
    if (phase > 0x100) {
        phase = 0x200 - phase;
    }
    return phase;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListRowBox);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListRows);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListColumn);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawScrollBar);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawScrollThumb);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateListSelection);

/* NON_MATCHING: 2 of 42 words differ, exact size: the argument-register copy and
 * the `slt` are swapped. */
// 40 of 42 words match at the exact size; one adjacent pair is scheduled
// in the opposite order.
#ifdef NON_MATCHING
void cmp_23C918(fclPanelPos_t *pos, s32 full, s32 total, s32 color1, s32 color2, s32 flag)
{
    if (total < full)
    {
        full = total;
    }
    cmpDrawBoxF(pos->unk_00, pos->unk_04, pos->unk_08, (u16)pos->unk_0C, (u16)pos->unk_0E, color2,
                flag);
    cmpDrawBoxF(pos->unk_00, pos->unk_04, pos->unk_08, (u16)pos->unk_0C * full / total,
                (u16)pos->unk_0E, color1, flag);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmp_23C918);
#endif

INCLUDE_ASM(const s32, "code_0022C578", camp_23C9C0);

INCLUDE_ASM(const s32, "code_0022C578", camp_23CAF8);

INCLUDE_ASM(const s32, "code_0022C578", campDrawInfoText);

INCLUDE_ASM(const s32, "code_0022C578", camp_23CC70);

INCLUDE_ASM(const s32, "code_0022C578", camp_23CDB8);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStatusRow1);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusBadge);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStatusRow2);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStatusRow3);

s32 campGetCursorColorSel(s32 color, cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);

    return fclBlendColor(color, -1, alpha);
}

s32 campGetCursorColorAlt(cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);
    s32 scale = (s32)fsqrtf(512.0f);

    return scale * scale * alpha / 256;
}

s32 campGetCursorColorAlt2(s32 color, cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);

    return fclBlendColor(color & ~0xFF, color, alpha);
}

s32 campGetStockAlpha(s8 index, s8 row)
{
    return campWork->stockAlpha[index + row * 4];
}

s32 campCalcStockScale(s8 index, s8 row)
{
    s32 v = 0x100 - campGetStockAlpha(index, row);
    s32 n = (s32)(fsqrtf(4096.0f) * (f32)v * 0.00390625f);

    return n * n;
}

s32 campDrawStockCell(s32 color, s8 index, s8 row)
{
    s32 alpha = (color & 0xFF) * campGetStockAlpha(index, row) / 256;

    return (color & ~0xFF) | (alpha & 0xFF);
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockStatus);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockLabel);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockLabel2);

s32 cmpStockHasRoom(cmpStockWork_t *work, s8 index)
{
    s32 ofs = index * 0x103C + 0xC0;
    cmpStockInfo_t *info = (cmpStockInfo_t *)((u8 *)work + ofs);
    s32 limit = work->stock[index].PartyCnt;

    return info->unk_0E + info->unk_10 < limit;
}

s32 campDrawStockIcon(s8 a, s8 b, s8 *rows, s8 *cols, s8 force)
{
    if (force != 0) {
        return 1;
    }
    if (rows[0] == a && cols[0] == b) {
        return 1;
    }
    if (rows[1] == a && cols[1] == b) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockRow1);

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockRow2);

INCLUDE_ASM(const s32, "code_0022C578", cmpFindFreeStockSlot);

void cmpScaleColorByPhase(s8 phase, u32 *colors, s32 scale)
{
    if ((phase == 1) || (phase == 3))
    {
        colors[2] = (colors[2] & 0xFFFFFF00) | (((colors[2] & 0xFF) * scale) >> 8);
        colors[3] = (colors[3] & 0xFFFFFF00) | (((colors[3] & 0xFF) * scale) >> 8);
    }
    if ((phase == 2) || (phase == 3))
    {
        colors[0] = (colors[0] & 0xFFFFFF00) | (((colors[0] & 0xFF) * scale) >> 8);
        colors[1] = (colors[1] & 0xFFFFFF00) | (((colors[1] & 0xFF) * scale) >> 8);
    }
}

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockHighlight);

INCLUDE_ASM(const s32, "code_0022C578", campCheckStockFree);

/* NON_MATCHING: 14 of 39 words differ at the exact size: retail keeps a second
 * register holding work + ofs that the C coalesces away. */
#ifdef NON_MATCHING
s32 campCheckStockRoom(s8 index, s8 listIndex, s8 *pRoom, u8 *pFlag, cmpStockWork_t *work, s8 skip)
{
    s32 ofs = index * 0x103C + 0xD0;
    u8 *p = (u8 *)work + ofs;
    s32 ret = *(s32 *)(p + 0xC);
    s8 room;

    *pFlag = p[0x14];
    if (skip == 0)
    {
        room = cmpStockHasRoom(work, index);
        *pRoom = room;
        cmpFindFreeStockSlot(listIndex, room);
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campCheckStockRoom);
#endif

INCLUDE_ASM(const s32, "code_0022C578", campDrawStockSlots);

INCLUDE_ASM(const s32, "code_0022C578", cmpRandomizePanelEff);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffBar);

typedef struct
{
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 start;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 target;
    /* 0x14 */ u8 unk_14[0xC];
    /* 0x20 */ s32 total;
    /* 0x24 */ s32 left;
    /* 0x28 */ s32 unk_28;
} cmpPanelEffAnim_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x24];
    /* 0x24 */ cmpPanelEffAnim_t anim[16];
} cmpPanelEffState_t;

void cmpUpdatePanelEffEntry(cmpPanelEffState_t *state)
{
    cmpPanelEffAnim_t *anim = state->anim;
    s32 i;

    for (i = 0; i < 16; i++) {
        s32 total = anim->total;
        s32 left = anim->left;
        s32 start = anim->start;

        anim->pos = start + (anim->target - start) * (((total - left) << 8) / total) / 256;
        if (left > 0) {
            anim->left = left - 1;
        }
        anim++;
    }
}

s32 func_0023ECF0(s32 index)
{
    if (cmpPanelEffList == NULL) {
        return 0;
    }
    return cmpPanelEffList[index].unk_34;
}

typedef struct
{
    /* 0x0000 */ u8 unk_0000[8];
    /* 0x0008 */ u8 state;
    /* 0x0009 */ u8 unk_0009[2];
    /* 0x000B */ u8 kind;
    /* 0x000C */ u8 unk_000C[0x2D8];
} cmpPanelEffView_t;

s32 func_0023ED18(s8 kind, s32 value)
{
    cmpPanelEff_t *list = cmpPanelEffList;
    s32 *slot = (s32 *)((u8 *)list + 0x38);
    cmpPanelEffView_t *eff;
    s32 i;

    if (list == NULL) {
        return -1;
    }
    eff = (cmpPanelEffView_t *)((u8 *)list + 0x10);
    for (i = 0; i < 0x20; i++) {
        if (eff[i].state != 0 && eff[i].kind == kind && *slot == value) {
            return i;
        }
        slot = (s32 *)((u8 *)slot + 0x2E4);
    }
    return -1;
}

s32 cmpSetPanelEffParam(s32 index, s32 value)
{
    if (cmpPanelEffList == NULL) {
        return 0;
    }
    cmpPanelEffList[index].unk_38 = value;
    return 1;
}

s32 func_0023EDB0(s32 index)
{
    if (index < 0) {
        return 0;
    }
    if (index < 0x21) {
        return 0;
    }
    cmpPanelEffList[index].state = 0;
    return 0;
}

s8 rstGetPanelEffState(s32 index)
{
    if (index < 0) {
        return 0;
    }
    if (index < 0x21) {
        return 0;
    }
    return cmpPanelEffList[index].state;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpAddPanelEffEntry);

INCLUDE_ASM(const s32, "code_0022C578", cmpAddQuePanelEff);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffBox);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffRing);

INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffQuad);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FF60);

INCLUDE_ASM(const s32, "code_0022C578", cmpFindPanelEffSlot);

INCLUDE_ASM(const s32, "code_0022C578", cmpUpdatePanelEffSlots);

s32 camp_update_paneleff_Process(void)
{
    if (cmpPanelEffList->flags & 0x40) {
        return -1;
    }
    cmpUpdatePanelEffSlots();
    return 0;
}

void cmpClearPanelEffSlots(cmpPanelEff_t *list)
{
    s32 i;

    for (i = 0x1F; i >= 0; i--) {
        list[i].state = 0;
    }
}

/* NON_MATCHING: 3 of 38 words differ, exact size; all three are one register name. */
#ifdef NON_MATCHING
cmpPanelEff_t *cmpAllocPanelEffWork(value)
s32 value;
{
    cmpPanelEff_t *work;
    void *chunk;

    if (cmpPanelEffList == NULL) {
        chunk = sdfMemAllocDirect(0x5C98);
        work = (cmpPanelEff_t *)sdfMemLock(chunk);
        memset(work, 0, 0x5C98);
        work->allocation = chunk;
        cmpClearPanelEffSlots(work);
        cmpInitSeqInfo(work, value);
        work->flags &= ~1;
        work->unk_04 = 0;
    } else {
        work = cmpPanelEffList;
    }
    return work;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpAllocPanelEffWork);
#endif

void camp_update_paneleff_Shutdown(void)
{
    if (cmpPanelEffList != NULL) {
        sdfMemFreeDirect(cmpPanelEffList->allocation);
        cmpPanelEffList = NULL;
    }
}

extern cmpPanelEff_t *cmpAllocPanelEffWork();

void *cmpStartProcessPanelEff(void)
{
    cmpPanelEff_t *work = cmpAllocPanelEffWork();

    cmpPanelEffList = work;
    return dds3StartProcess("camp_update_paneleff", 0x520B, 1, 1, camp_update_paneleff_Process,
                            camp_update_paneleff_Shutdown, work);
}

void rstSetPanelEffFlag(void)
{
    if (cmpPanelEffList != NULL) {
        cmpPanelEffList->flags |= 0x40;
    }
}

void rstSetStatusModelReady(s32 ready)
{
    if (ready == 0) {
        cmpStatusModelReady = 0;
    } else {
        cmpStatusModelReady = 1;
    }
}

s32 cmpGetEncycWork(void)
{
    return cmpEncycWork;
}

u16 cmpGetStatusModelId(void)
{
    return cmpStatusModelId;
}

INCLUDE_ASM(const s32, "code_0022C578", cmp_23F8F0);

void func_0023F9F8(u16 id, f32 delta)
{
    if (delta < 0.0f && cmpModelParamTable[id].scale - delta < 0.0f)
    {
        return;
    }
    cmpModelParamTable[id].scale += delta;
    cmpGetStatusModelMotion(cmpEncycWork);
}

void func_0023FA98(u16 id, s32 x, s32 y, s32 z)
{
    cmpModelParamTable[id].evo[0] += x;
    cmpModelParamTable[id].evo[1] += y;
    cmpModelParamTable[id].evo[2] += z;
}

void func_0023FB10(u16 id, s32 x, s32 y, s32 z)
{
    cmpModelParamTable[id].pos[0] += x;
    cmpModelParamTable[id].pos[1] += y;
    cmpModelParamTable[id].pos[2] += z;
}

u8 cmpGetModelMotionId(u16 id)
{
    return cmpModelParamTable[id].motionId;
}

f32 cmpGetModelScale(u16 id)
{
    return cmpModelParamTable[id].scale;
}

void rstGetEvoModelParam(u16 id, f32 *out)
{
    out[0] = cmpModelParamTable[id].evo[0];
    out[1] = cmpModelParamTable[id].evo[1];
    out[2] = cmpModelParamTable[id].evo[2];
}

void cmpGetModelPosRot(u16 id, f32 *out)
{
    out[0] = cmpModelParamTable[id].pos[0];
    out[1] = cmpModelParamTable[id].pos[1];
    out[2] = cmpModelParamTable[id].pos[2];
}

INCLUDE_ASM(const s32, "code_0022C578", cmpModelInitCore2);

void cmpModelInit(void)
{
    cmpModelInitFlag = 0;
    D_0054FEEE = 6;
    cmpStatusModelId = 0;
    cmpModelHandle = 0;
    cmpModelInitCore2();
}

void cmpResetDrawEnv(void)
{
    dds3ScreenParam[4] = 2048.0f;
    dds3ScreenParam[5] = 2048.0f;
}

extern void mdlFree(s32 aHandle);

extern void cmpFreeBadEffect(void);

extern char D_00540028[];

void cmpModelFreeResource(void)
{
    if (cmpEncycWork != 0)
    {
        printf(D_00540028, D_0054FEEE, cmpStatusModelId);
        mdlFree(cmpEncycWork);
        cmpEncycWork = 0;
        cmpFreeBadEffect();
    }
}

s32 cmpPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    D_0054FEE0 = mdlPrepare(group, id, blocking);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpCreateBadEffect);

extern void *hCAMPBadEffect;

extern u8 D_00349300[];

extern char D_00540040[];

extern void *effCAMPBadCreate(u8 *work, s32 kind);

s32 cmpBadEffSetup(u16 id, u32 flag)
{
    s32 kind;

    if (cmpEncycWork == 0)
    {
        return 0;
    }
    switch (flag)
    {
    case 0x100:
        kind = 9;
        break;
    case 0x40:
        kind = 7;
        break;
    case 0x20:
        kind = 6;
        break;
    case 0x8000:
        kind = 0xD;
        break;
    default:
        return 0;
    }
    cmpCreateBadEffect(id, cmpEncycWork, D_00349300);
    hCAMPBadEffect = effCAMPBadCreate(D_00349300, kind);
    printf(D_00540040);
    return 1;
}

extern void *hCAMPBadEffect;

extern void effCAMPBadDelete(void *hCAMPBadEffect);

extern char D_00540058[];

void cmpFreeBadEffect(void)
{
    if (hCAMPBadEffect != NULL)
    {
        effCAMPBadDelete(hCAMPBadEffect);
        hCAMPBadEffect = NULL;
        printf(D_00540058);
    }
}

extern char D_00540070[];

void rstPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    cmpPrepareStatusModel(group, id, blocking);
    printf(D_00540070, group, id);
}

/* NON_MATCHING: 4 of 46 words differ, exact size: the two constant-1 pseudos
 * swap $2 and $3. */
// 42 of 46 words match at the exact size; two constants land in swapped
// registers and the branch between them moves with them.
#ifdef NON_MATCHING
s32 cmpModelSetup(u16 id, s32 blocking)
{
    s32 group;
    s32 model;

    if (cmpModelInitFlag != 0)
    {
        return 0;
    }
    if (id == 0)
    {
        group = 6;
        model = 0;
    }
    else
    {
        group = 6;
        model = id;
    }
    if ((group == D_0054FEEE) && (model == cmpStatusModelId) && (cmpEncycWork != 0))
    {
        return 2;
    }
    cmpModelFreeResource();
    D_0054FEEE = group;
    cmpStatusModelId = model;
    cmpModelHandle = blocking;
    cmpPrepareStatusModel(D_0054FEEE, cmpStatusModelId, cmpModelHandle);
    cmpModelInitFlag = 1;
    if (blocking == 1)
    {
        cmpLoadStatusModel();
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpModelSetup);
#endif

extern char D_00540090[];

extern s32 mdlLoad(s32 group, s32 id);

extern void cmpGetStatusModelMotion();

s32 cmpLoadStatusModel(void)
{
    if (cmpModelInitFlag != 0)
    {
        if (cmpPrepareStatusModel(D_0054FEEE, cmpStatusModelId, cmpModelHandle) != 0)
        {
            if (cmpPrepareStatusModel(D_0054FEEE, cmpStatusModelId, cmpModelHandle) != -1)
            {
                cmpModelFreeResource();
                cmpEncycWork = mdlLoad(D_0054FEEE, cmpStatusModelId);
                cmpGetStatusModelMotion(cmpEncycWork);
                printf(D_00540090, D_0054FEEE, cmpStatusModelId);
                cmpModelInitFlag = 0;
                return 2;
            }
        }
        return 1;
    }
    return 0;
}

#include "motion.h"

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s8 aNumber);

void cmpGetStatusModelMotion(mdlObj_t *model)
{
    s8 motion = (s8)cmpModelParamTable[cmpStatusModelId].motionId;

    if (cmpStatusModelId == 0 &&
        (((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->badstatus & 0x8000) != 0) {
        motion = 2;
    }
    mdlLoopMotion(model, 0, motion);
    model->first->speed = cmpModelParamTable[cmpStatusModelId].scale;
}

extern datUnitVisual_t datModelParamTable[];

extern void mdlSetScale(void *aHandle);

f32 cmpGetStatusModelScale(void *model, s32 flag)
{
    sdfVector4_t scale;
    f32 value = 1.0f;

    if (flag != 0) {
        value = datModelParamTable[cmpStatusModelId].formscale;
    }
    scale.x = value;
    scale.w = 1.0f;
    scale.y = value;
    scale.z = value;
    SDF_VU0_LOADVECTOR_MEMOPS(scale);
    mdlSetScale(model);
    return value;
}

void cmpResetStatusModelPos(void)
{
    cmpStatusModelBasePos.z = -400.0f;
    cmpStatusModelBasePos.y = 0.0f;
    cmpStatusModelPos.x = 0.0f;
    cmpStatusModelPos.y = 0.0f;
    cmpStatusModelPos.z = 0.0f;
    cmpStatusModelBasePos.x = 0.0f;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpSetStatusModelPose);

void cmpSetModelRotate(void *model)
{
    FromEuler(cmpModelParamTable[cmpStatusModelId].pos[0] * 3.1415926f / 180.0f,
              cmpModelParamTable[cmpStatusModelId].pos[1] * 3.1415926f / 180.0f,
              cmpModelParamTable[cmpStatusModelId].pos[2] * 3.1415926f / 180.0f);
    mdlSetRotate(model);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpSetStatusModelParam);

extern void cmpSetStatusModelPose(s32 model);

extern void cmpSetModelRotate(void *model);

extern void cmpSetStatusModelParam(void);

extern void effCAMPBadDraw(void *hCAMPBadEffect);

s32 cmpDrawStatusModel(s32 scene)
{
    s32 err = cmpLoadStatusModel();

    if (err != 0) {
        return err;
    }
    if (cmpEncycWork != 0) {
        cmpSetStatusModelPose(cmpEncycWork);
        cmpSetModelRotate(cmpEncycWork);
        cmpSetStatusModelParam();
        mdlLink(cmpEncycWork, scene);
        if (hCAMPBadEffect != NULL) {
            effCAMPBadDraw(hCAMPBadEffect);
        }
    }
    return err;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpRebuildStockList);

void cmpRemoveStockSlot(s8 index)
{
    s32 i;

    cmbGlobalWork->stock[index] = 0;
    for (i = index + 1; i < cmbGlobalWork->stockCount; i++)
    {
        cmbGlobalWork->stock[i - 1] = cmbGlobalWork->stock[i];
    }
    cmbGlobalWork->stockCount--;
}

void cmpCompactStockList(s8 devilId)
{
    cmbGlobalWork->stock[cmbGlobalWork->stockCount++] = devilId;
}

void cmpClearStockFlag(datUnitWork_t *work)
{
    work->flag &= ~2;
}

void cmpSetStockFlag(datUnitWork_t *work)
{
    work->flag |= 2;
}

void cmpSetMaxHPMP(datUnitWork_t *unit)
{
    s32 hpRate = unit->hp * 1024 / unit->maxhp;
    s32 mpRate = unit->mp * 1024 / unit->maxmp;

    unit->maxhp = datGetMaxHp(unit);
    unit->maxmp = datGetMaxMp(unit);
    unit->hp = unit->maxhp * hpRate / 1024;
    unit->mp = unit->maxmp * mpRate / 1024;
}

s8 cmpGetParamGaugeColor(s8 index)
{
    return D_00349380[index];
}

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FFE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_0053FFF8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540000);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540008);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540018);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540020);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540028);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540030);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540040);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540054);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540058);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540070);

INCLUDE_RODATA(const s32, "code_0022C578", D_0054007C);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540090);

INCLUDE_RODATA(const s32, "code_0022C578", D_005400B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005400C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005400D0);

INCLUDE_ASM(const s32, "code_0022C578", cmpExecRecover);

s32 cmp_240C10(void)
{
    return 1;
}

u16 cmpChkSkillBad(u16 skill)
{
    datSkillBadWindow_t *bad = &datSkillBadEntries[skill].bad;

    if (bad->kind != 2) {
        return 0;
    }
    return bad->id;
}

s32 cmpGetSkillEntry(u16 id, datUnitWork_t *unit)
{
    datNormalSkill_t *skill = &datSkillTable.normal[id];
    u16 cost = skill->cost;

    switch (skill->costtype) {
    case 1:
        if (unit->hp < cost) {
            return 0;
        }
        break;
    case 2:
        if (unit->mp < cost) {
            return 0;
        }
        break;
    }
    return 1;
}

s32 cmpAddCost(u16 skill, datUnitWork_t *unit)
{
    datNormalSkill_t *entry = &datSkillTable.normal[skill];
    u16 cost = entry->cost;

    switch (entry->costtype)
    {
    case 1:
        if (unit->hp < cost)
        {
            return 0;
        }
        datAddHp(unit, -cost);
        break;
    case 2:
        if (unit->mp < cost)
        {
            return 0;
        }
        datAddMp(unit, -cost);
        break;
    }
    return 1;
}

s32 cmpGetSkillTargetArea(u16 skillId)
{
    s32 area;

    if (skillId == 0) {
        return 1;
    }
    area = datSkillTable.normal[skillId].targettype;
    switch (area) {
    case 0:
        return 1;
    case 1:
    case 2:
        return 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUseItemKou);

s32 cmpUseItemTokusyu(u16 item, s32 target, s32 user)
{
    if (cmpUseItemKou(item, user) >= 0)
    {
        p2sdPlaySE(0x10, 0x7F, 0x3F);
        return 3;
    }
    if (item == 0x3A)
    {
        p2sdPlaySE(0x11, 0x7F, 0x3F);
        return 5;
    }
    /* The nesting is what keeps ee-gcc from turning this into a sibling call. */
    do
    {
        return cmpUseSkillTokusyu(datItemTable[item].skill, target, user);
    } while (0);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpUseSkillTokusyu);

INCLUDE_ASM(const s32, "code_0022C578", cmpRecover);

char *cmpGetParamName(s8 index)
{
    return D_00349388[index];
}

INCLUDE_ASM(const s32, "code_0022C578", cmpGetPressedButton);

INCLUDE_ASM(const s32, "code_0022C578", cmpPlayCursorSE);

s32 cmpChkPadPressUpDown(void)
{
    if (dds3PadButtonState[0x26] != 0) {
        return 1;
    }
    if (dds3PadButtonState[0x27] != 0) {
        return 2;
    }
    return 0;
}

s32 cmpGetPadUpDown(void)
{
    if (dds3PadButtonState[0x24] != 0) {
        return 1;
    }
    if (dds3PadButtonState[0x25] != 0) {
        return 2;
    }
    return 0;
}

s32 func_002415D8(void)
{
    if (dds3PadButtonState[0x28] != 0) {
        return 1;
    }
    if (dds3PadButtonState[0x2A] != 0) {
        return 3;
    }
    return 0;
}

s32 cmpAllocEffectSlot(s32 color, s8 a, s8 b, cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);

    if (cmpGetCursorSlotState(0, slot) == 3)
    {
        return 0x89FEFF80;
    }
    if (alpha > 0)
    {
        if (alpha < 0x100)
        {
            /* The nesting is what keeps ee-gcc from turning this into a sibling call. */
            do
            {
                return fclBlendColor(color, 0x5B50E080, alpha);
            } while (0);
        }
        return color;
    }
    if (a != 0)
    {
        if (b == 0)
        {
            return 0x89FEFF80;
        }
    }
    return color;
}

/* NON_MATCHING: 18 of 21 words differ (first at +0x0). */
#ifdef NON_MATCHING
void cmpClearCursorSlots(cmpCursorInfo_t *cur)
{
    cmpCursorSlot_t (*slot)[2] = (cmpCursorSlot_t (*)[2])((u8 *)cur + 0x3C);
    s32 i;
    s32 j;

    for (j = 0; j < 0x100; j++) {
        for (i = 0; i < 2; i++) {
            slot[j][i].state = 0;
            slot[j][i].param = 0;
            slot[j][i].value = 0x100;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpClearCursorSlots);
#endif

/* NON_MATCHING: 8 of 15 words differ (first at +0xC). */
#ifdef NON_MATCHING
void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot)
{
    cmpCursorSlot_t *p = &slot[index];
    s32 *v = &slot[index].value;

    if (p->state != 3) {
        *v = 0;
        p->param = param;
        p->state = state;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpSetCursorSlot);
#endif

s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot)
{
    slot += index;
    return slot->value;
}

s8 cmpGetCursorSlotState(s8 index, cmpCursorSlot_t *slot)
{
    slot += index;
    return slot->state;
}

/* NON_MATCHING: 30 of 31 words differ (first at +0x4). */
#ifdef NON_MATCHING
void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor)
{
    cmpCursorSlot_t (*slot)[2] = (cmpCursorSlot_t (*)[2])((u8 *)cursor + 0x3C);
    s32 i;
    s32 j;

    for (j = 0; j < 0x100; j++) {
        for (i = 0; i < 2; i++) {
            if (slot[j][i].state != 0) {
                if (slot[j][i].value < 0x100) {
                    slot[j][i].value += slot[j][i].param;
                } else {
                    slot[j][i].state = 0;
                }
                if (slot[j][i].value >= 0x100) {
                    slot[j][i].value = 0x100;
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateCursorSlots);
#endif

/* NON_MATCHING: 7 of 37 words differ, exact size. */
#ifdef NON_MATCHING
void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode)
{
    u8 *work = cur;

    memset(work + 8, 0, 0x18);
    *(s32 *)(work + 0x18) = a;
    *(s32 *)(work + 0x1C) = b;
    *(s32 *)(work + 8) = a;
    *(s32 *)work = mode;
    *(s32 *)(work + 0xC) = b;
    *(s32 *)(work + 0x10) = c;
    *(s32 *)(work + 0x14) = d;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpInitCursorLayout);
#endif

void cmpInitCursorRange(void *cursor, u8 index, u8 rows, s32 flag)
{
    cmpPadStopWork_t *work = cursor;
    s32 last;

    memset(&work->cursor.unk_20, 0, 0x101C);
    last = (index < rows) ? index : rows;
    work->cursor.unk_20 = index;
    work->cursor.unk_26 = last;
    work->cursor.unk_22 = 0;
    work->cursor.unk_24 = 0;
    work->cursor.unk_25 = 0;
    work->cursor.unk_28 = flag;
    work->cursor.unk_2C = 0;
    work->cursor.unk_30 = 0;
    work->cursor.unk_34 = 0;
    work->unk_38 = 0;
    work->cursor.StopFlag = 0;
    cmpClearCursorSlots(&work->cursor);
}

void cmpUpdateCursorScroll(cmpCursorInfo_t *cursor, s8 mode)
{
    u8 *flags;
    s32 idx;
    s32 step;
    s32 count;
    s32 i;
    u8 row;

    flags = (u8 *)cursor->unk_2C;
    if (flags == NULL)
    {
        return;
    }
    step = (mode >= 0) ? 1 : -1;
    row = cursor->unk_24;
    idx = cursor->unk_22 + row;
    if (!(flags[idx] & 2))
    {
        return;
    }
    count = cursor->unk_20;
    cursor->unk_24 = row + mode;
    i = 1;
    while (i < count)
    {
        idx += step;
        if (!(flags[idx] & 2))
        {
            return;
        }
        cursor->unk_24 = (u8)cursor->unk_24 + mode;
        i++;
    }
}

void cmpSetCursorFlag(cmpCursorInfo_t *cur, s32 flag)
{
    cur->unk_2C = flag;
    cmpUpdateCursorScroll(cur, 1);
}

/* NON_MATCHING: 9 of 28 words differ (first at +0x8). */
#ifdef NON_MATCHING
void cmp_241A10(cmpCursorInfo_t *cursor)
{
    u8 *flags = (u8 *)cursor->unk_2C;
    s32 count;
    s32 i;

    if (flags == NULL) {
        return;
    }
    count = cursor->unk_20;
    i = count - 1;
    cursor->unk_21 = 0;
    if (count > 0) {
        do {
            if ((flags[i] & 2) != 0) {
                cursor->unk_21++;
            }
        } while (i-- > 0);
    }
    cmpUpdateCursorScroll(cursor, 1);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmp_241A10);
#endif

/* NON_MATCHING: 3 of 26 words differ (first at +0x20). */
#ifdef NON_MATCHING
s32 cmp_241A88(cmpCursorInfo_t *cursor, s32 index)
{
    u8 *flags = (u8 *)cursor->unk_2C;
    u8 value;

    if (flags == NULL) {
        return 0;
    }
    if (index >= cursor->unk_20) {
        return 0;
    }
    value = flags[index];
    if ((value & 2) != 0) {
        return 0;
    }
    flags[index] = value | 2;
    cursor->unk_21++;
    cmpUpdateCursorScroll(cursor, 1);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmp_241A88);
#endif

void cmpResetCursorPos(s16 value, cmpCursorInfo_t *cursor)
{
    cursor->StopFlag = 0;
    cursor->unk_24 = 0;
    cursor->unk_25 = 0;
    cursor->unk_22 = value;
}

void cmpSetCursorRow(s8 row, cmpCursorInfo_t *cursor)
{
    cursor->unk_24 = row;
}

void cmpResetCursorInfo(cmpCursorInfo_t *cursor)
{
    cursor->unk_08 = cursor->unk_18;
    cursor->unk_0C = cursor->unk_1C;
    cmpResetCursorPos(0, cursor);
    cmpUpdateCursorScroll(cursor, 1);
    cmpClearCursorSlots(cursor);
}

s32 rcvRootHasItems(rcvRootWork_t *work)
{
    u8 *flags = work->itemFlags;

    if (flags == NULL) {
        return 1;
    }
    return (flags[work->cursor + work->scroll] & 1) ^ 1;
}

s32 cmpCursorAtFirstEnabled(cmpCursorInfo_t *cursor)
{
    s32 count = 0;
    s32 total = cursor->unk_22 + (u8)cursor->unk_24;
    u8 *flags = (u8 *)cursor->unk_2C;
    s32 i;

    if (flags != NULL) {
        for (i = total - 1; i >= 0; i--) {
            if ((flags[i] & 2) != 0) {
                count++;
            }
        }
        if (count < total) {
            goto no;
        }
        return 1;
    }
    if (total != 0) {
        goto no;
    }
    return 1;
no:
    return 0;
}

s32 cmpCursorAtLastEnabled(cmpCursorInfo_t *cursor)
{
    u8 *flags;
    s32 idx;
    s32 count;
    s32 n;
    s32 i;

    flags = (u8 *)cursor->unk_2C;
    idx = cursor->unk_22 + (u8)cursor->unk_24;
    n = 0;
    if (flags != NULL)
    {
        count = cursor->unk_20;
        for (i = idx + 1; i < count; i++)
        {
            if (flags[i] & 2)
            {
                n++;
            }
        }
        if (idx >= count - n - 1)
        {
            return 1;
        }
    }
    else if (idx == cursor->unk_20 - 1)
    {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0022C578", cmpFindNextCursorItem);

void cmpAdvanceCursorPhase(cmpCursorInfo_t *cursor)
{
    cursor->unk_34 += 6;
    if (cursor->unk_34 > 0x200) {
        cursor->unk_34 -= 0x200;
    }
}

s32 cmpMoveCursor(cmpCursorInfo_t *cur, s8 dir)
{
    if (dir != 0)
    {
        cur->unk_34 = 0;
    }
    else
    {
        cmpAdvanceCursorPhase(cur);
    }
    cmpFindNextCursorItem(cur, dir, 1);
    cur = NULL;
}

s32 cmpGetCursorMoveMode(cmpCursorInfo_t *cursor)
{
    if (cursor->unk_20 < 2) {
        return 1;
    }
    if (cursor->StopFlag != 0) {
        return 2;
    }
    return 0;
}

s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir)
{
    s32 mode = cmpGetCursorMoveMode(cursor);

    if (dir != 0)
    {
        if ((mode == 0) || (mode == 2))
        {
            cursor->unk_34 = 0;
        }
    }
    else
    {
        cmpAdvanceCursorPhase(cursor);
    }
    if (mode == 1)
    {
        cursor->StopFlag = 2;
        return 0;
    }
    if ((dir == 0) && (mode == 2))
    {
        if (dds3PadButtonState[0x26] != 0)
        {
            return 0;
        }
        if (dds3PadButtonState[0x27] != 0)
        {
            return 0;
        }
    }
    /* The nesting is what keeps ee-gcc from turning this into a sibling call. */
    do
    {
        return cmpFindNextCursorItem(cursor, dir, 1);
    } while (0);
}

INCLUDE_ASM(const s32, "code_0022C578", cmpChangeCursorListNums);

void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor)
{
    s32 end = cursor->unk_20 + 0xFF;

    cursor->unk_25 = end - cursor->unk_21;
    cursor->unk_24 = end;
}

extern f32 D_0054E474;

/* NON_MATCHING: 15 of 32 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop)
{
    s8 delay = work->unk_38;

    if (delay < 0) {
        s32 next = work->cursor.unk_30;

        if (next >= 0) {
            f32 scaled = next;

            next = (s32)(scaled / D_0054E474);
            work->cursor.unk_30 = next;
        }
        if (next < 0) {
            work->cursor.unk_30 = 0;
        }
        if (work->cursor.unk_30 == 0) {
            work->unk_38 = 0;
        }
    } else {
        work->unk_38 = -delay;
    }
    if (work->cursor.StopFlag == 2) {
        return bits & ~stop;
    }
    return bits;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDisablePadBitStop);
#endif

s8 cmpGetCursorRow(cmpCursorInfo_t *cursor)
{
    return cursor->StopFlag;
}

extern s32 cmpMoveCursor2(cmpCursorInfo_t *cursor, s8 dir);

void cmpMoveStockCursor(s8 index, s8 dir, u8 *base)
{
    cmpMoveCursor2((cmpCursorInfo_t *)(base + index * 0x103C + 0xAC), dir);
}

void func_00242320(s8 value, u8 *work)
{
    work[0x2124] = value;
}

/* NON_MATCHING: 16 of 38 words differ, exact size. */
#ifdef NON_MATCHING
s32 cmpCursorSlotIsSelectable(datUnitWork_t *unit, u16 mask, s8 flag)
{
    s32 flags = unit->flag;
    s32 test;

    if ((flags & 1) != 0) {
        if (flag != 0 && (flags & 2) != 0 && (mask & 0x20) == 0) {
            goto no;
        }
        if ((flags & 0x10) != 0 && (mask & 0x40) == 0) {
            goto no;
        }
        if ((unit->badstatus & 0x800) != 0 && (mask & 0x80) == 0) {
            goto no;
        }
        if ((flags & 4) != 0) {
            test = mask & 4;
        } else {
            test = mask & 8;
        }
        if (test == 0) {
            goto no;
        }
        return 1;
    }
    if ((mask & 0x10) != 0) {
        return 1;
    }
no:
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpCursorSlotIsSelectable);
#endif

/* NON_MATCHING: 6 of 32 words differ (first at +0x5C). */
#ifdef NON_MATCHING
s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s8 index, s8 flag)
{
    s32 ofs = index * 0x103C + 0xC0;
    cmpStockInfo_t *info = (cmpStockInfo_t *)((u8 *)stock + ofs);
    s8 slot = *(u8 *)&info->unk_0E + info->unk_10;

    if (flag == 0) {
        return ((s8 *)stock)[slot + index * 0x56 + 5];
    } else {
        return ((s8 *)stock)[slot + index * 0x56 + 0x15];
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpGetStockInfoIndex);
#endif

INCLUDE_ASM(const s32, "code_0022C578", cmpBuildStockNameList);

s8 cmpGetStockCount(void)
{
    return cmbGlobalWork->stockCount;
}

void cmpSortStockList(void)
{
    s32 i;
    s32 k;
    s8 j;
    s8 cur;

    for (i = 0; i < cmbGlobalWork->stockCount; i++)
    {
        j = 0;
        cur = cmbGlobalWork->stock[i];
        for (k = 0; k < cmbGlobalWork->stockCount; k++)
        {
            if ((i != k) && (cur == (u8)cmbGlobalWork->stock[k]))
            {
                cmbGlobalWork->stock[k] = 0;
                cmbGlobalWork->stockCount--;
            }
            else
            {
                cmbGlobalWork->stock[j++] = cmbGlobalWork->stock[k];
            }
        }
    }
}

s8 func_002426F0(void)
{
    if (cmbGlobalWork->stockCount < 0x10) {
        return cmbGlobalWork->stockCount;
    }
    return -1;
}

s8 func_00242710(u8 id)
{
    s32 count = cmbGlobalWork->stockCount;
    s8 n = 0;

    if (count > 0) {
        u8 *p = (u8 *)cmbGlobalWork->stock;
        s32 i = count;

        do {
            if (id == *p) {
                n++;
            }
            p++;
            i--;
        } while (i != 0);
    }
    return n;
}

void cmpDebugDumpCount(void)
{
    s32 i;

    cmpSortStockList();
    cmpGetStockCount();
    printf("cnt = %d\n", *(s32 *)DDS3_GBWK->stocklist);
    for (i = 0; i < 0x10; i++) {
        printf("%02d : %d\n", i, (s8)DDS3_GBWK->unk1914[i]);
    }
}

void cmpInitSeqInfo(cmpSeqInfo_t *pSeq, s32 pParent)
{
    pSeq->unk_00 = 3;
    pSeq->unk_02 = -1;
    pSeq->unk_05 = 0;
    pSeq->unk_0C = 0;
    pSeq->unk_10 = pParent;
}

s32 cmpChkSeqInfo(cmbGbWork_t *work)
{
    if (work->unk_00005 != 0) {
        if (dds3GetProcessStat((void *)work->unk_00008) == 3) {
            work->unk_00005 = 0;
            return 2;
        }
        if (dds3GetProcessStat((void *)work->unk_00008) == 0) {
            work->unk_00005 = 0;
        }
        return 1;
    }
    return 0;
}

s32 cmpUpdateSeqInfo(cmbGbWork_t *work, s8 value)
{
    s32 stat = cmpChkSeqInfo(work);

    if ((s8)work->unk_00002 >= 0)
    {
        if (stat == 1)
        {
            return 0;
        }
        if (work->unk_00004 > 0)
        {
            work->unk_00003 = (u8)work->unk_00001;
        }
        work->unk_00001 = (u8)work->unk_00002;
        work->unk_00002 = -1;
        return 1;
    }
    if (value >= 0)
    {
        if (work->unk_00004 > 0)
        {
            work->unk_00003 = value;
        }
    }
    if (stat != 1)
    {
        work->unk_00005 = 0;
    }
    return 0;
}

void cmpUpdateTerminate(cmpInfoWindow_t *window)
{
    window->flags |= (*(vu8 *)&window->flags & 0x70) << 1;
}

void cmp_242918(s32 id, u8 bits)
{
    u8 *flags = dds3GetProcessData(id);

    if (flags != NULL) {
        *flags |= bits;
    }
}

void cmp_242958(s32 id, u8 bits)
{
    u8 *flags = dds3GetProcessData(id);

    if (flags != NULL) {
        *flags &= ~bits;
    }
}

u8 cmp_242998(s32 id)
{
    u8 *data = dds3GetProcessData(id);

    if (data == NULL) {
        return 0;
    }
    return data[0];
}

typedef struct
{
    /* 0x00 */ s8 skillattr;
    /* 0x01 */ s8 type;
    /* 0x02 */ s16 index;
    /* 0x04 */ u16 keisyoform;
    /* 0x06 */ s8 capacity;
    /* 0x07 */ u8 flag;
} cmbSkillAttr_t;

extern cmbSkillAttr_t attr[];

INCLUDE_ASM(const s32, "code_0022C578", func_002429C0);

/* NON_MATCHING: 13 of 17 words differ (first at +0xC). */
#ifdef NON_MATCHING
s32 cmpItemHasFlag(u16 item, s8 which)
{
    if (which == 0) {
        if ((datItemTable[item].flags & 1) != 0) {
            return 1;
        }
        return 0;
    }
    return (datItemTable[item].flags2 & 2) == 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpItemHasFlag);
#endif

void func_00242A70(s32 key) {
}

s32 campStatusIsActive(void)
{
    return campStatusActive != 0;
}

INCLUDE_RODATA(const s32, "code_0022C578", D_00540128);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540138);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540148);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540158);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540168);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540178);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540188);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540198);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401CC);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005401F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540208);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540218);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540228);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540238);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540248);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540258);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540268);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540278);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540288);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540298);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005402F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540308);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540318);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540328);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540338);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540348);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540358);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540368);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540378);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540388);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540398);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005403F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540408);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540418);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540428);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540438);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540448);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540458);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540468);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540478);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540488);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540498);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005404F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540508);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540518);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540528);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540538);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540548);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540558);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540568);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540578);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540588);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540598);

INCLUDE_RODATA(const s32, "code_0022C578", D_005405A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005405B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005405C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005405D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005405F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540610);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540628);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540640);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540650);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540668);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540690);

INCLUDE_RODATA(const s32, "code_0022C578", D_005406A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005406D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005406F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540708);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540718);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540738);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540758);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540770);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540798);

INCLUDE_RODATA(const s32, "code_0022C578", D_005407A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005407C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005407D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005407E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005407F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540828);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540850);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540888);

INCLUDE_RODATA(const s32, "code_0022C578", D_005408B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005408E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540908);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540930);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540958);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540978);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540998);

INCLUDE_RODATA(const s32, "code_0022C578", D_005409B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005409E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540A10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540A38);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540A50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540A60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540A88);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540AA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540AC8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540AE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540B08);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540B20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540B38);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540B60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540B78);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540BA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540BC8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540BE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C08);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C38);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540C98);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540CB8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540CD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540CF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540D18);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540D30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540D58);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540D70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540D98);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540DC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540DE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540E10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540E28);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540E58);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540E78);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540E98);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540EC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540EE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540F08);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540F30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540F50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540F70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540F88);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540FA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540FC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540FD8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00540FE8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541008);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541038);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541060);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541088);

INCLUDE_RODATA(const s32, "code_0022C578", D_005410A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005410B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005410D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005410E8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541110);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541130);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541148);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541170);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541188);

INCLUDE_RODATA(const s32, "code_0022C578", D_005411A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005411C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005411D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005411F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541218);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541230);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541248);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541268);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541280);

INCLUDE_RODATA(const s32, "code_0022C578", D_005412A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005412C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005412D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005412F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541318);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541330);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541358);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541380);

INCLUDE_RODATA(const s32, "code_0022C578", D_005413A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005413C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005413E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541408);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541430);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541448);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541460);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541478);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541498);

INCLUDE_RODATA(const s32, "code_0022C578", D_005414B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005414D8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005414F8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541518);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541540);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541558);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541568);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541580);

INCLUDE_RODATA(const s32, "code_0022C578", D_005415A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005415B8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005415D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005415F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541608);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541620);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541638);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541648);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541660);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541678);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541698);

INCLUDE_RODATA(const s32, "code_0022C578", D_005416B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005416D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005416F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541710);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541738);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541760);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541780);

INCLUDE_RODATA(const s32, "code_0022C578", D_005417A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005417C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005417E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541808);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541828);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541850);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541878);

INCLUDE_RODATA(const s32, "code_0022C578", D_005418A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005418C8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005418E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541908);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541928);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541950);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541970);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541990);

INCLUDE_RODATA(const s32, "code_0022C578", D_005419A8);

INCLUDE_RODATA(const s32, "code_0022C578", D_005419D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005419F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541A08);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541A30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541A58);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541A78);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541A98);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541AB8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541AE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541B08);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541B30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541B58);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541B78);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541B90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541BB8);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541BE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541BF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541C90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541CF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541D90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541DF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541E90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541EA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541EB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541EC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541ED0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541EE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541EF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541F90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00541FF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542000);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542010);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542020);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542030);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542040);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542050);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542060);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542070);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542080);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542090);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005420F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542100);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542110);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542120);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542130);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542140);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542150);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542160);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542170);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542180);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542190);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005421F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542200);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542210);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542220);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542230);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542240);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542250);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542260);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542270);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542280);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542290);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005422F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542300);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542310);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542320);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542330);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542340);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542350);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542360);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542370);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542380);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542390);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005423F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542400);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542410);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542420);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542430);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542440);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542450);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542460);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542470);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542480);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542490);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005424F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542500);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542510);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542520);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542530);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542540);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542550);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542560);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542570);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542580);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542590);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005425F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542600);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542610);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542620);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542630);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542640);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542650);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542660);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542670);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542680);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542690);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005426F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542700);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542710);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542720);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542730);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542740);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542750);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542760);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542770);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542780);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542790);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005427F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542800);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542810);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542820);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542830);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542840);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542850);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542860);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542870);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542880);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542890);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005428F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542900);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542910);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542920);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542930);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542940);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542950);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542960);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542970);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542980);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542990);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429A0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429B0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429C0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429D0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429E0);

INCLUDE_RODATA(const s32, "code_0022C578", D_005429F0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542A90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542AF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542B90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542BF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542C90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542CF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542D90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542DF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542E90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542EA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542EB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542EC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542ED0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542EE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542EF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F00);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F10);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F20);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F30);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F40);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F50);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F60);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F70);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F80);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542F90);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FA0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FB0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FC0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FD0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FE0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00542FF0);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543000);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543018);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543028);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543040);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543050);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543060);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543070);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543080);

INCLUDE_RODATA(const s32, "code_0022C578", D_00543098);

INCLUDE_RODATA(const s32, "code_0022C578", D_005430A8);

INCLUDE_SDATA(const s32, "code_0022C578", campWork);

INCLUDE_SDATA(const s32, "code_0022C578", campExitState);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDD5);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FDF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE00);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE08);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE10);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE18);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE20);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE28);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE30);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE38);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE40);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE48);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE50);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE58);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE60);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE68);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE70);

INCLUDE_SDATA(const s32, "code_0022C578", campStatusPanel);

INCLUDE_SDATA(const s32, "code_0022C578", campInfoWork);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE7C);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE80);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatTimer);

INCLUDE_SDATA(const s32, "code_0022C578", rstHeartsMasterPhase);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE8E);

INCLUDE_SDATA(const s32, "code_0022C578", cmbBirthPanel);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE94);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FE98);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEBC);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatusPhase);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatusWindowHidden);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatusModelLoadState);

INCLUDE_SDATA(const s32, "code_0022C578", cmpInfoWindow);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FECC);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FED0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FED8);

INCLUDE_SDATA(const s32, "code_0022C578", cmpPanelEffList);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEE0);

INCLUDE_SDATA(const s32, "code_0022C578", cmpEncycWork);

INCLUDE_SDATA(const s32, "code_0022C578", hCAMPBadEffect);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatusModelReady);

INCLUDE_SDATA(const s32, "code_0022C578", cmpModelInitFlag);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEEE);

INCLUDE_SDATA(const s32, "code_0022C578", cmpStatusModelId);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEF2);

INCLUDE_SDATA(const s32, "code_0022C578", cmpModelHandle);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FEF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF00);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF08);

INCLUDE_SDATA(const s32, "code_0022C578", campStatusActive);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF14);

INCLUDE_SDATA(const s32, "code_0022C578", cmbRaceTable);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF1C);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF20);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF28);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF30);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF38);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF40);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF48);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF50);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF58);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF60);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF68);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF70);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF78);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF80);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF88);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF90);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FF98);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFC0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFC8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFD0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_0054FFF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550000);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550004);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550008);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550010);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550018);

INCLUDE_SDATA(const s32, "code_0022C578", D_0055001C);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550020);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550028);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550030);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550038);

INCLUDE_SDATA(const s32, "code_0022C578", D_0055003C);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550040);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550048);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550050);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550054);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550058);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550060);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550064);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550068);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550070);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550078);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550080);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550088);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550090);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550098);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005500F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550100);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550108);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550110);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550118);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550120);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550128);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550130);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550134);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550138);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550140);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550148);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550150);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550158);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550160);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550168);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550170);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550178);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550180);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550188);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550190);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550198);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005501F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550200);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550208);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550210);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550218);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550220);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550228);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550230);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550238);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550240);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550248);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550250);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550258);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550260);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550268);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550270);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550278);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550280);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550288);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550290);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550298);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005502F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550300);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550308);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550310);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550318);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550320);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550328);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550330);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550338);

INCLUDE_SDATA(const s32, "code_0022C578", D_0055033C);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550340);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550348);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550350);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550358);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550360);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550368);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550370);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550378);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550380);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550388);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550390);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550398);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005503F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550400);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550408);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550410);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550418);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550420);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550428);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550430);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550438);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550440);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550448);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550450);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550458);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550460);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550468);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550470);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550478);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550480);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550488);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550490);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550498);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005504F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550500);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550508);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550510);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550518);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550520);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550528);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550530);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550538);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550540);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550548);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550550);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550558);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550560);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550568);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550570);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550578);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550580);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550588);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550590);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550598);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005505F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550600);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550608);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550610);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550618);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550620);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550628);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550630);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550638);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550640);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550648);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550650);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550658);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550660);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550668);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550670);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550678);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550680);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550688);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550690);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550698);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005506F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550700);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550708);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550710);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550718);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550720);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550728);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550730);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550738);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550740);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550748);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550750);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550758);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550760);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550768);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550770);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550778);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550780);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550788);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550790);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550798);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005507F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550800);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550808);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550810);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550818);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550820);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550828);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550830);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550838);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550840);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550848);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550850);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550858);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550860);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550868);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550870);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550878);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550880);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550888);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550890);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550898);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005508F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550900);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550908);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550910);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550918);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550920);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550928);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550930);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550938);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550940);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550948);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550950);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550958);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550960);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550968);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550970);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550978);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550980);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550988);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550990);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550998);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509A0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509A8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509B0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509B8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509C0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509C8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509D0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509D8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509E0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509E8);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509F0);

INCLUDE_SDATA(const s32, "code_0022C578", D_005509F8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A00);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A08);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A10);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A18);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A20);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A28);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A30);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A38);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A40);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A48);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A50);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A58);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A60);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A68);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A70);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A78);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A80);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A88);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A90);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550A98);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AC0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AC8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AD0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550AF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B00);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B08);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B10);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B18);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B20);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B28);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B30);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B38);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B40);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B48);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B50);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B58);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B60);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B68);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B70);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B78);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B80);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B88);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B90);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550B98);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BC0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BC8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BD0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550BF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C00);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C08);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C10);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C18);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C20);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C28);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C30);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C38);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C40);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C48);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C50);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C58);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C60);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C68);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C70);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C78);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C80);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C88);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C90);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550C98);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CC0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CC8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CD0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550CF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D00);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D08);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D10);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D18);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D20);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D28);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D30);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D38);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D40);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D48);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D50);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D58);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D60);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D68);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D70);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D78);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D80);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D88);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D90);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550D98);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DA8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DB0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DB8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DC0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DC8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DD0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DD8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DE0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DE8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DF0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550DF8);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E00);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E08);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E10);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E18);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E20);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E28);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E30);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E38);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E40);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E48);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E50);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E58);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E60);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E68);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E70);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E78);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E80);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E88);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E90);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550E98);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550EA0);

INCLUDE_SDATA(const s32, "code_0022C578", D_00550EA8);

