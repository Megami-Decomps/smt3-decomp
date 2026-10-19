#include "common.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"

typedef struct
{
    u16 devilId;
    u16 unk_02;
    s32 flag;
} cmbUnlockEntry_t;

typedef struct
{
    u16 skill[8];
    s8 count;
} cmpSkillList_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x38];
    /* 0x38 */ u16 skillCount;
    /* 0x3A */ u16 skill[12];
} cmbUnitSkills_t;

extern char D_0053C928[];
extern char D_0053C938[];
extern char D_0054FB00[];
extern cmbUnlockEntry_t D_0032DCC0[];
extern cmbUnlockEntry_t D_0032DBB8[];
extern s8 gProcessStat_0;
extern cmpSkillList_t *cmpSkillCursor;

extern s32 cmbDrawSequence(void);
extern void *dds3StartProcess(char *namestr, u32 priority, s32 starttimer, s32 shutdowntimer,
                              void *processfunc, void *shutdownfunc, void *data);
extern void dds3KillProcessName(char *namestr, s32 flag);

void fclUpdateInit(void);
void fclFreeStage(void);
s32 fclCombineCalc(void);
s32 fclCombineDraw(void);
s32 fclCombineUpdate(void);
void fclCombineDestroy(void);

typedef struct
{
    u8 race;
    s8 value;
} cmbRaceResult_t;

typedef struct
{
    u16 devilId;
    cmbRaceResult_t result[32];
} cmbRaceEntry_t;

typedef struct
{
    u16 devilA;
    u16 devilB;
    u16 result;
} cmbPairEntry_t;

typedef struct
{
    u8 flag;
    u8 unk_01[3];
    u8 race;
    u8 unk_05;
    u8 unk_06[54];
} datDevilInfo_t;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} cmbUnit_t;

extern cmbRaceEntry_t D_0032DA70[];
extern cmbPairEntry_t D_0032DB78[];
extern cmbPairEntry_t D_0032DB90[];
extern datDevilInfo_t datDevilTable[];

extern s32 dds3GetRandA(s32 seed);
extern s32 datGetMaxHp(datUnitWork_t *work);
extern s32 datGetMaxMp(datUnitWork_t *work);
extern s32 cmbLookupCombine(u16 devilA, u16 devilB, s8 flag);
extern s32 cmbCalcResultLevel(cmbUnit_t *unitA, cmbUnit_t *unitB, s32 mode, s8 flag);
extern s32 cmbGetRaceCombineResult(s32 race, u8 level);

extern s32 evtBitCheck(s32 bit);
extern s32 evtStartEvent(s32 event);
extern s32 evtGetAgeOfMoon(void);
extern void *dds3SearchProcessName(const char *name);
extern s32 dds3GetProcessStat(void *id);
extern s32 fclSetWorkValue(s8 index, s32 value);
s32 fclGetFlag(s8 index);
void fclClearFlag(s8 index);

extern s8 gCallMode;
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
extern u8 D_00343E74[];
extern void *D_00345D9C[];
extern s32 campStatusActive;

typedef struct
{
    /* 0x0000 */ u16 unk_00;
    /* 0x0002 */ u16 index;
    /* 0x0004 */ u8 shift;
    /* 0x0005 */ u8 unk_05[3];
} cmpCursorPos_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x17C];
    /* 0x017C */ cmpCursorPos_t cursorPos;
} lstListWindow_t;

/* The save-work fields this unit reaches that the shared dds3GlobalWork_t view
 * leaves as padding or does not cover at all. */
typedef struct
{
    /* 0x00000 */ u8 unk_00000[0x1914];
    /* 0x01914 */ u8 id[0x10];
    /* 0x01924 */ s32 idCount;
    /* 0x01928 */ u8 unk_01928[0x10558];
    /* 0x11E80 */ s32 recordCount;
} dds3GlobalWorkView_t;

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
} fclEncycWork_t;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ u8 unk_04[0x2C];
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} dds3TitleWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ u8 unk_0002[0x12];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x18];
    /* 0x0030 */ s8 unk_30;
    /* 0x0031 */ u8 unk_0031[0x112F];
    /* 0x1160 */ s32 unk_1160;
    /* 0x1164 */ s32 unk_1164;
    /* 0x1168 */ s32 unk_1168;
    /* 0x116C */ s32 unk_116C;
    /* 0x1170 */ s32 unk_1170;
    /* 0x1174 */ s32 unk_1174;
} campInfoWork_t;

extern fclEncycWork_t *g_pinst;
extern dds3TitleWork_t *titleWork;
extern campInfoWork_t *campInfoWork;
extern s8 D_00551F80[8];
extern s8 cmpParamGaugeState[2];
extern u8 cmpStatusModelReady;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ s32 activeTime;
    /* 0x18 */ s32 deactiveTime;
    /* 0x1C */ s32 phase;
    /* 0x20 */ s32 count;
} fclRbnWork_t;

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

typedef struct
{
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ f32 modelScale;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 smokeScale;
    /* 0x10 */ s32 color;
} cmbSmokeParam_t;

extern cmbSmokeParam_t D_0032DE20[];
extern rcvWork_t *D_0054FBB0;
extern char D_0053CF10[];
extern void sortSkillList(cmbUnit_t *unit);
extern void p2sdStopBGM(s32 id);
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

extern s32 mdlPrepare(s32 group, s32 id, s32 blocking);
extern s32 shopDrawPanelMode0(s32 mode);
extern s32 shopUpdateItemList(s32 mode);
extern void p2sdStopAllSe(void);
extern s32 GetValidUnitIndex(s32 index);
extern void ReleaseDebug_fclEncyc(void);
extern void sdfMemFreeDirect(void *ptr);

typedef struct
{
    /* 0x00 */ u8 animState;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08[0xC];
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelAnim_t;

typedef struct fclPanel
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ void (*draw)(struct fclPanel *panel);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

extern s32 gMsgID;
extern s32 gSelMsgNo;
extern s8 fclEndMessageEx(s8 flag);
extern void sdfChipFreeDirect(void *ptr);
extern s8 cmbAddLevelUpParamEx(void *work, s8 mode);
extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);
extern s32 titleAllocWork(s32 arg);
extern s32 mselShutDown(void);
extern s32 stfShutDown(void);
extern s32 dds3PadVibrationSet(s32 port, s32 power, s32 frames);
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
    /* 0x0001 */ u8 unk_01[0x13];
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
extern u8 D_00343E40[];
extern s32 sdfMovieIsFinished(void *movie);
extern void movieStop(void);
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
    /* 0x00 */ s32 tex;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ u8 color[4];
} fclSprite_t;

extern void qsort(void *base, s32 count, s32 size, s32 (*cmp)(const void *a, const void *b));
extern s32 func_00213148(const void *a, const void *b);
extern s32 func_00213190(const void *a, const void *b);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void fclFilterOFF(void);
extern void sdfFileCmdReadFile(const char *name, s32 offset, s32 size, void *done, void *buf);
extern void fclSetSprite(fclSprite_t *sprite, s32 x, s32 y, s32 d, s32 u0, s32 v0, s32 u1,
                         s32 v1, s32 color, s32 tex);
extern void fclDrawSpriteEx(s32 a, s32 b, s32 w, s32 h, fclSprite_t *sprite, s32 flags);
typedef struct
{
    /* 0x00 */ u16 skill;
    /* 0x02 */ u16 unk_02;
} datHeartsSkill_t;

typedef struct
{
    /* 0x00 */ u8 race;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s16 param;
    /* 0x04 */ u8 unk_04[0x15];
    /* 0x19 */ s8 unk_19;
    /* 0x1A */ u8 unk_1A[2];
    /* 0x1C */ datHeartsSkill_t heartsSkill[8];
    /* 0x3C */ u8 unk_3C[6];
} datRaceEntry_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x54];
    /* 0x54 */ s32 unk_54;
} motionWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0x13];
    /* 0x0018 */ u8 unk_18;
    /* 0x0019 */ u8 unk_19;
    /* 0x001A */ u8 unk_1A[8];
    /* 0x0022 */ u16 unk_22;
    /* 0x0024 */ u16 unk_24;
    /* 0x0026 */ u8 unk_26[0x52];
    /* 0x0078 */ s32 unk_78;
    /* 0x007C */ u8 unk_7C[0xF0];
    /* 0x016C */ datUnitWork_t *unk_16C;
    /* 0x0170 */ u8 unk_170[0x112E];
    /* 0x129E */ s8 unk_129E;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[0x103F];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x23];
    /* 0x2308 */ s32 unk_2308;
    /* 0x230C */ s32 unk_230C;
    /* 0x2310 */ u32 unk_2310;
} rstWork_t;

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ u8 unk_03[0x53];
} cmpLocalStock_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ s8 unk_04;
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
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorWork_t;

typedef struct
{
    /* 0x0000 */ cmpCursorWork_t cursor;
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
    /* 0x8306 */ u8 unk_8306[0x19A];
    /* 0x84A0 */ s32 effectStep;
    /* 0x84A4 */ u8 unk_84A4[0xC];
    /* 0x84B0 */ s32 stockAlpha[1];
} campWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 flags2;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 unk_04[4];
} datItemEntry_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u16 id;
    /* 0x08 */ u8 unk_08[0x14];
} datSkillBad_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[3];
    /* 0x03 */ u8 icon;
    /* 0x04 */ u8 unk_04[0x1C];
    /* 0x20 */ datSkillBad_t bad;
} datSkillEntry_t;

extern campWork_t *campWork;
extern datItemEntry_t datItemTable[];
extern datSkillEntry_t cmbSkillTable[] asm("datSkillTable");
extern f32 aViewEye[4];
extern f32 aViewTarget[4];
extern void rstSetMaxHpMp(s32 mode, s32 arg);
extern void func_00116BB8(void);
extern s32 dds3GlobalWorkInit(void);
extern void cmpBuildStockNameList(cmpLocalStock_t *stock);
extern void cmpInitStockCursorPos(void);
extern void cmpStartSkillTokusyuMsg(u16 skill);
extern void cmpModelInitCore2(void);
extern campStatusPanel_t *cmpStatusPhase;
extern s8 cmpStatusWindowHidden;
extern s8 cmpModelInitFlag;
extern u16 D_0054FEEE;
extern s32 cmpModelHandle;
extern fclRbnWork_t *g_pdebug;
extern s32 cmpDrawBoxLine(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 *g);
extern void itfMesMngSetVarImmediate(s32 msg, s8 index, s32 value);
extern void func_00224230(datUnitWork_t *unit);
extern s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot);
typedef struct
{
    /* 0x0000 */ u8 unk_00[0x30];
    /* 0x0030 */ datUnitWork_t *levelUpUnit[16];
    /* 0x0070 */ s8 levelUpCount;
    /* 0x0071 */ u8 levelUpStep;
    /* 0x0072 */ u8 unk_72[0xFA];
    /* 0x016C */ datUnitWork_t *unit;
    /* 0x0170 */ u8 unk_170[0x2175];
    /* 0x22E5 */ s8 unk_22E5;
    /* 0x22E6 */ s8 unk_22E6;
    /* 0x22E7 */ u8 unk_22E7;
    /* 0x22E8 */ f32 unk_22E8;
    /* 0x22EC */ f32 unk_22EC;
    /* 0x22F0 */ u8 unk_22F0[0x18];
    /* 0x2308 */ s32 unk_2308;
} evtStartWork_t;

extern evtStartWork_t *EventStart;
extern f32 dds3ScreenParam[];
extern s32 D_0054FEE0;
extern s8 D_00349380[];
extern char *D_00349388[];
extern s8 dds3PadButtonState[];
extern void *dds3GetProcessData(s32 id);
extern s8 shpProcessStat;
extern s8 gProcessStat_1;
extern s8 gProcessStat_2;
extern s32 itfMesMngGetSelectNo(s32 msg);
extern s32 itfMesMngGetSelPhase(s32 msg);
extern s32 fclWorkValue[16];

typedef struct
{
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ u8 unk_04[0x10];
} cmbSlotParam_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x2200];
    /* 0x2200 */ u16 price[0x22];
    /* 0x2244 */ u16 sellPrice[0x22];
} shopProcWork_t;

typedef struct
{
    /* 0x0000 */ s8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ s8 unk_02;
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x14];
    /* 0x0020 */ cmpCursorWork_t listCursor;
    /* 0x0058 */ u8 unk_0058[0x4474];
    /* 0x44CC */ s32 script;
    /* 0x44D0 */ s32 scriptArg;
} rcvUpdateWork_t;

extern cmbSlotParam_t D_0032DE28[];
extern char D_0053CF70[];
extern char D_0053D140[];
extern char D_0053D390[];
extern shopProcWork_t *shopProcWork;
extern rcvUpdateWork_t *rcvUpdateWork;
extern f32 D_0054E410;
extern u8 gCommandFlag[4];
extern void rcvReviveUnit(datUnitWork_t *work);

extern datRaceEntry_t D_00339E00[];
extern rstWork_t *startPos;
extern char D_0053F100[];
extern s32 scrGetIntPara(s32 index);
extern void scrSetIntReturnValue(s32 value);
extern s32 p2sdPlayBGM(s32 id);
extern s32 dds3PlayMovieByIndex(s32 index);

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

extern void cmpUpdateCursorScroll(cmpCursorWork_t *cursor, s32 mode);

typedef struct
{
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u32 regAddr;
    /* 0x08 */ u8 *buffer;
} movieViewerWork_t;

extern movieViewerWork_t movieViewerDebugFlag;
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

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x2216];
    /* 0x2216 */ s8 cursor;
    /* 0x2217 */ s8 unk_2217;
} trmCalcWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ u8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x134];
    /* 0x0140 */ cmpCursorWork_t listCursor;
    /* 0x0178 */ u8 unk_0178[0x44D0];
    /* 0x4648 */ s32 script;
    /* 0x464C */ s32 scriptArg;
    /* 0x4650 */ u8 unk_4650[0xF0];
    /* 0x4740 */ u8 seqMitama[4];
} ragUpdateWork_t;

extern cmbGlobalWork_t *cmbGlobalWork asm("DDS3_GBWK");
extern trmCalcWork_t *trmCalcWork;
extern ragUpdateWork_t *ragUpdateWork;
extern void fclRunPanelAnim(void *work);

extern u8 *GBWK_0;
extern u8 *GBWK_1;
extern u8 *GBWK_3;
extern s32 printf(const char *fmt, ...);
extern char D_0053DA28[];
extern char D_0053C878[];
extern u8 D_0032D1F0[];
extern s16 datGetDevilParam(u8 devilId);
extern void sdfTexFree(s32 handle);

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
    /* 0x0001C */ cmpCursorWork_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ s8 slotIndex[4];
    /* 0x04114 */ u8 unk_04114[4];
    /* 0x04118 */ s8 unk_4118;
    /* 0x04119 */ u8 unk_04119[3];
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
    /* 0x17640 */ cmpCursorWork_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRbnWork_t *ribbon;
} cmbGbWork_t;

extern cmbGbWork_t *CMB_GBWK;

extern u8 fclTerminalCommandCount[1];
extern s32 datAddItem(s32 itemId, s32 count);
extern s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s32 mode, s32 flag);

extern void MoveActive(fclRbnWork_t *work);
extern void MoveDeactive(fclRbnWork_t *work);
extern s32 cmpStatSetTimer(s32 kind, s8 index);
extern void rstSetDestroySkillMaxHpMp(void);

extern sdfGrOt_t sdfGrOtTable[85];

extern void release_movieobj2(void *movie, sdfGrOt_t *ot);
extern void cmpReCalcStockCursorPos(s32 index);

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

extern char D_0053DD98[];
extern char D_0053DF08[];

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);
extern s32 titleFreeWork(s32 arg);
extern void mselFreeResources(s32 arg);

typedef struct
{
    /* 0x00 */ u8 unk_00[0xC];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 unk_1E[2];
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
} fclReadRibbon_t;

extern s8 gProcessStat_4;


typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x7C];
    /* 0x007C */ s8 unk_7C;
    /* 0x007D */ s8 unk_7D;
    /* 0x007E */ u8 unk_007E[0xEE];
    /* 0x016C */ void *unk_16C;
    /* 0x0170 */ u8 unk_170[0x112E];
    /* 0x129E */ s8 unk_129E;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[0x103F];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x2F];
    /* 0x2314 */ s32 fadeColor;
} rstDrawWork_t;

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ char name[0x14];
} dds3MovieEntry_t;

extern rstDrawWork_t *rstDrawWork;
extern dds3MovieEntry_t D_00343BE8[];
extern s8 campExitState;

extern s32 moviePlayFile_27041744(s32 arg0, char *name);
extern void cmpResetCursorInfo(cmpCursorWork_t *cursor);
extern void campDrawText(s32 id, s32 flag);
extern void FromEuler(f32 x, f32 y, f32 z);
extern void mdlSetRotate(void *model);

typedef struct
{
    /* 0x00 */ void *unk_00;
    /* 0x04 */ void *unk_04;
    /* 0x08 */ void *unk_08;
    /* 0x0C */ u8 unk_0C[0x34];
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ u8 unk_4C[0xC];
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
} mselGlobalWork_t;

extern mselGlobalWork_t *gGlobal;
extern u16 dds3VblanksPerFrame;
extern s32 dds3KernelDebugSign;
extern s32 PlayingNo;

extern void sdfIpuRunDecode(s32 frames);
extern void titleFreeTextures(void);
extern void movieCloseStream(u8 *movie);
extern s32 dds3KillProcess(s32 process, s32 shutdowntimer);
extern s32 func_002043E0(u32 **work);
extern void mdlSmoothLoopMotion(void *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                                f32 blendDurationFrames);
extern s32 fclChkMessage(void);
extern void cmpReturnParamDH(void);
extern s32 cmpUnloadStatusModel(void);

typedef s32 (*cmpSeqFunc_t)(void);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);

extern char D_0054FB48[];
extern char D_0053D028[];
extern char D_0053D038[];
extern char D_0054FB98[];
extern char D_0053D1F0[];
extern char D_0053D200[];
extern char D_0054FBD8[];
extern char D_0053D3F8[];
extern char D_0053D408[];
extern char D_0053D640[];
extern char D_0053D650[];
extern char D_0053D660[];

extern void fclScriptForcedEnd(u8 *script);

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ s8 comment;
    /* 0x03 */ u8 unk_03;
} datCommentEntry_t;

extern datCommentEntry_t D_00339DF8[];
extern char D_0054FCF0[];
extern char D_0053DA90[];
extern char D_0053DAA0[];

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
    /* 0x0000 */ u8 unk_0000[0x20];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorWork_t listCursor;
} lstWork_t;

typedef struct
{
    /* 0x0 */ u16 skill;
    /* 0x2 */ u8 order;
    /* 0x3 */ u8 unk_3;
} skillSortEntry_t;

typedef struct
{
    /* 0x00 */ s32 handle;
    /* 0x04 */ s32 lock;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 id;
    /* 0x0E */ u8 unk_0E[6];
} cmbActionData_t;

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
extern void stfFreeTextures(void);
extern void itfMesMngRelease(s32 msg);
extern void cmpRemoveStockDevilEntry(void);
extern void cmpInitDrawList(void);

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
} cmpMsgEntry_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ s8 unk_0002;
    /* 0x0003 */ u8 unk_0003;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ s8 unk_0005;
    /* 0x0006 */ u8 unk_0006[2];
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ u8 unk_000C[0x20];
    /* 0x002C */ cmpCursorWork_t listCursor;
    /* 0x0064 */ u8 unk_0064[0x2328];
    /* 0x238C */ s32 script;
    /* 0x2390 */ s32 scriptArg;
    /* 0x2394 */ s32 texHandle[2];
    /* 0x239C */ u8 unk_239C[0x80];
    /* 0x241C */ fclPanelAnim_t anim;
} shpUpdateWork_t;

extern shpUpdateWork_t *shopUpdateWork;

extern cmpMsgEntry_t D_00345DD0[];
extern cmpMsgEntry_t D_00345DA0[];
typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ void (*calcSizes)();
} fclRagTex_t;

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());
extern void fclRagTexCalcSizes();
extern u8 ragHeartsTable[];
extern campStatusPanel_t *campDhUpdateWork;
extern s8 cmpGetHeartsIconId(s8 index);
extern void cmpInitCursorRange(void *cursor, s32 index, s32 rows, s32 flag);
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void cmpModelFreeResource(void);
extern void cmpResetDrawEnv(void);
extern void cmpMoveCursorListEnd(cmpCursorWork_t *cursor);
extern void FillIndex(s16 *dst, s32 count);
extern void cmpUpdateTerminate(cmpInfoWindow_t *window);
extern void fclEncycSetExplainData(void *data);
extern void frDestroyFontRequestAfter(void *frq);
extern void func_0023B500(s32 value);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineDisableCmdZensyo);

INCLUDE_ASM(const s32, "code_001FAE80", cmbEnableMenuEntry5);

s32 fclCombineChkEventStat(void)
{
    s32 stat;

    stat = dds3GetProcessStat(dds3SearchProcessName("e800_main"));
    if ((u32)stat < 4U) {
        if (stat != 0) {
            return 1;
        }
    }
    return 0;
}

void fclCombineEventStart(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    evtStartEvent(0x320);
}

void fclCombineInitWork(void)
{
    fclSetWorkValue(1, 100);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineLoadTextures);

void fclCombineTexFree(void)
{
    s8 i;

    for (i = 0; i < 7U; i++) {
        sdfTexFree(CMB_GBWK->texHandle[i]);
    }
    printf(D_0053C878);
}

void fclCombineSetTable(void)
{
    CMB_GBWK->table = D_0032D1F0;
}

s32 cmbChkGeneralStat(void)
{
    if ((cmbGlobalWork->party[0].unit.badstatus & 0x8000) != 0) {
        return 1;
    }
    if (evtGetAgeOfMoon() == 8) {
        return 0;
    }
    return 2;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineCheckMoonPhase);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineBGM);

void fclCombineInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x1A724);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x30, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053C878);

INCLUDE_ASM(const s32, "code_001FAE80", fclUpdateInit);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineDestroy);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineCalc);

s32 fclCombineDraw(void)
{
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    return cmbDrawSequence();
}

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineUpdate);

void fclCombineProcessStart(void)
{
    fclUpdateInit();
    dds3StartProcess(D_0054FB00, 0x400, 1, 1, fclCombineCalc, NULL, NULL);
    dds3StartProcess(D_0053C928, 0x2B10, 1, 1, fclCombineDraw, NULL, NULL);
    dds3StartProcess(D_0053C938, 0x520C, 1, 1, fclCombineUpdate, fclCombineDestroy, NULL);
    gProcessStat_0 = 1;
}

void fclCombineProcessEnd(void)
{
    dds3KillProcessName(D_0054FB00, 1);
    dds3KillProcessName(D_0053C928, 1);
    dds3KillProcessName(D_0053C938, 1);
}

s32 cmbCheckProcessStat(void)
{
    s8 stat = gProcessStat_0;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            gProcessStat_0 = 0;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetBirthPhase);

s32 cmbBirthDevilIsAvailable(void)
{
    u16 flag = cmbGlobalWork->party[0].unit.badstatus & 0x8000;

    return flag != 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbBuildResultRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCreateCombineResultTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCreateLocalStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcRoot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcDevSel1st);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSeqBuildResultTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetupBirthPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddBirthDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbStartResultScript);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcIntroduce);

s32 cmbSeqStartScript(void)
{
    if (fclEncycWork() != 0) {
        return 1;
    }
    if (CMB_GBWK->unk_00004 != 0) {
        fclScriptStart(CMB_GBWK->script, 3, &CMB_GBWK->scriptArg);
    }
    return 0;
}


INCLUDE_ASM(const s32, "code_001FAE80", cmbRunBirthDevilStep);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053C928);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053C938);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053C950);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053C988);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSeqSetPhase);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcSequence);

s32 cmbDevilIsUnlocked(u16 devilId)
{
    cmbUnlockEntry_t *entry = D_0032DCC0;
    u32 i;

    for (i = 0; i < 34; i++, entry++) {
        if ((devilId == entry->devilId) && (evtBitCheck(entry->flag) == 0)) {
            return 1;
        }
    }
    return 0;
}

void cmpSetSkillCursor(cmpSkillList_t *list)
{
    cmpSkillCursor = list;
}

s32 cmpFindSkillEntry(u16 skillId)
{
    cmpSkillList_t *list = cmpSkillCursor;
    s32 i;

    if (list == NULL) {
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        if (list->skill[i] == skillId) {
            return 1;
        }
    }
    return 0;
}

s8 cmbFindEvoIndex(u16 devilId)
{
    u32 i;

    for (i = 0; i < 4; i++) {
        if (D_0032DBB8[i].devilId == devilId) {
            return i;
        }
    }
    return -1;
}

s32 cmbApplyPercent(s32 base, s32 value, s32 max, u8 percent)
{
    s32 delta = base * percent / 100 - base;

    value = (delta == 0) ? value + 1 : value + delta;
    if (base * 2 < base + value) {
        value = base;
    }
    return (max < base + value) ? max - base : value;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetEvoTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetCombineRace);

s32 cmbIsSpecialPair(s32 devilA, s32 devilB, s32 race)
{
    if (race == 0xC1) {
        if (devilA == race) {
            return 1;
        }
        if (devilB == race) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindCombineResult);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindUnlockedResult);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindBirthEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindBirthResult);

s8 cmbFindRaceEntry(u16 devilId, u8 race)
{
    u32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        if (D_0032DA70[i].devilId == devilId) {
            for (j = 0; j < 32; j++) {
                if (D_0032DA70[i].result[j].race == race) {
                    return D_0032DA70[i].result[j].value;
                }
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindPairEntry);

s8 cmbCountSkills(cmbUnit_t *unit)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        if (unit->unit.keisyoskill[i] == 0) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbLookupCombine);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcResultLevel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetRaceCombineResult);

void cmbGetDevilIdAndRace(cmbUnit_t *unit, u16 *devilId, u8 *race)
{
    *devilId = unit->unit.id;
    *race = datDevilTable[unit->unit.id].race;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCheckUnlockedPair);

s32 cmbEntryIsValid(u16 devilId)
{
    u32 i;

    for (i = 0; i < 3; i++) {
        if (devilId == D_0032DB78[i].result) {
            return 1;
        }
    }
    return 0;
}

s32 cmbDevilIsSpecial(u16 devilId)
{
    return datDevilTable[devilId].flag & 2;
}

s32 cmbIsFullMoon(void)
{
    s32 moon = (s8)evtGetAgeOfMoon();

    if ((moon == 8) | (moon == 0)) {
        if ((dds3GetRandA(0) & 0xF) != 0) {
            return 0;
        }
    } else {
        if ((dds3GetRandA(0) & 0xFF) != 0) {
            return 0;
        }
    }
    return 1;
}

s32 cmbSkipRand(void)
{
    dds3GetRandA(0);
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindSpecialEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbBuildDevilEntry);

void cmbCalcUnitParam(cmbUnit_t *unit)
{
    s32 i;

    for (i = 0; i < 6; i++) {
        unit->unit.param[i] += unit->unit.param[i] / 5;
    }
    unit->unit.hp = datGetMaxHp(&unit->unit);
    unit->unit.maxhp = unit->unit.hp;
    unit->unit.mp = datGetMaxMp(&unit->unit);
    unit->unit.maxmp = unit->unit.mp;
}

u16 cmbCalcCombineResult(cmbUnit_t *unitA, cmbUnit_t *unitB, s8 flag, s8 levelFlag)
{
    s32 race;
    s32 level;
    s32 result;

    race = cmbLookupCombine(unitA->unit.id, unitB->unit.id, flag);
    level = cmbCalcResultLevel(unitA, unitB, 0, levelFlag);
    result = cmbGetRaceCombineResult(race, level);
    if (result == 0xCD) {
        result = (flag != 0) ? result : 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetCombineResultEx);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcIkenieExp);

s32 cmbDevilCanBirth(u16 devilId)
{
    datDevilInfo_t *entry = &datDevilTable[devilId];
    u32 flag;

    if (entry->race == 0) {
        return 1;
    }
    flag = *(u32 *)entry & 0x20;
    return flag > 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbMakeBirthDevil);

s8 cmbFindStockSlot(cmbUnit_t *list, u8 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (list[i].unit.flag == 0) {
            return i;
        }
    }
    return -1;
}

s8 ragUseItemOnUnit(u16 id, datUnitWork_t *unit, u8 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (unit->flag != 0 && unit->id == id) {
            return i;
        }
        unit++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbInheritSkills);

void rstClearSkillSlot(cmbUnit_t *unit, u8 slot)
{
    unit->unit.keisyoskill[slot] = 0;
    sortSkillList(unit);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbReplaceSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstAddSkillToSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkSkillOwner);

s8 rstFindSkillSlot(u16 skill, cmbUnit_t *unit)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        if (unit->unit.keisyoskill[i] == skill) {
            return i;
        }
    }
    return -1;
}

s8 cmbFindSkillSlot(cmbUnit_t *unit)
{
    s32 count = 0;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (unit->unit.keisyoskill[i] != 0) {
            count++;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyDefaultDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyCustomDevilToStock);

void cmbClearStockEntry(cmbUnit_t *unit)
{
    memset(unit, 0, 0xEC);
}

void cmbClearUnitSkills(cmbUnitSkills_t *unit)
{
    memset(unit->skill, 0, sizeof(unit->skill));
    unit->skillCount = 0;
}

void cmbClearSkillSlots(cmbUnit_t *unit)
{
    memset(unit->unit.keisyoskill, 0, sizeof(unit->unit.keisyoskill));
}

INCLUDE_ASM(const s32, "code_001FAE80", cmb_1FF0C8);

INCLUDE_ASM(const s32, "code_001FAE80", cmbApplyEvoUnlock);

void cmbFindStockEnd(s8 devilId)
{
    cmbGlobalWork->stock[cmbGlobalWork->stockCount++] = devilId;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbStoreBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddCustomDevilToStock);

extern s32 cmbCopyEvoDevil(datUnitWork_t *unit, u16 skill);

s32 cmbApplyEvoResult(datUnitWork_t *unit)
{
    u16 *skill = unit->keisyoskill;
    s32 total = 0;
    s32 i;

    for (i = 0; i < 24; i++)
    {
        if (*skill != 0)
        {
            total += cmbCopyEvoDevil(unit, *skill);
        }
        skill++;
    }
    return total;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbPickEvoResult);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcInheritCount);

s32 cmbGetSpecialItemId(void)
{
    return 0x8D;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetMoonBonus);

s32 cmbCheckDevilPair(datUnitWork_t *a, datUnitWork_t *b, datUnitWork_t *c)
{
    if (a->id == 0xC0) {
        return 1;
    }
    if (b->id == 0xC0) {
        return 1;
    }
    if (c == NULL) {
        goto no;
    }
    if (c->id != 0xC0) {
        goto no;
    }
    return 1;
no:
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawUnitLevel);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CB30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CB48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CB60);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockIcon);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockFrame);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockText);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockBg);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawListPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockList);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawDevilStats);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawDevilNameLevel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawResultIcon);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawResultHeader);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawPhaseDefault);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawInfoSprite);

extern u8 cmbSpriteTable[];
extern void fclDrawSpriteRes(s32 a, s32 b, s32 color, s32 d, s32 e, s32 f, s32 *handles,
                             u8 *table, s32 id);
void cmbDrawTopSprite(void)
{
    fclDrawSpriteRes(0, 0, 0xFFFFF0, 0, 0, 0x23, CMB_GBWK->texHandle, cmbSpriteTable, 0x47);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawTopText);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockScrollBar);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawResultFooter);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawResultPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockPanel1);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockPanel2);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStockPanel3);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawStatus);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawBirthDevilMaskBG);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", func_00201FB0);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawPhase3);

s32 cmbDrawPhasePanel(void)
{
    s32 phase = (s8)CMB_GBWK->unk_00002;

    switch (phase)
    {
    case 3:
        return cmbDrawPhase3(0);
    case 2:
        return func_00201FB0(1);
    default:
        cmbDrawPhaseDefault(1);
        phase = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawSequence);

void cmbExit(void)
{
    cmpMoveCursorListEnd(&CMB_GBWK->combineCursor);
    CMB_GBWK->unk_00005 = 1;
    CMB_GBWK->unk_00008 = fclScriptStart(CMB_GBWK->script, 0xB, &CMB_GBWK->scriptArg);
    CMB_GBWK->unk_00001 = 4;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateRoot);

s32 cmbStockIsLocked(s32 index)
{
    s32 flag = cmbGlobalWork->party[index].unit.flag & 0x800;

    return flag != 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChangeSequence);

INCLUDE_ASM(const s32, "code_001FAE80", cmbWaitCancelButton);

extern void fclRbnSwitchActive(fclRbnWork_t *rbn, s32 index);

void cmbSwitchRibbon(void)
{
    s32 index = CMB_GBWK->unk_1650A;

    if (CMB_GBWK->unk_4118 == -1) {
        s32 prev = index - 1;

        if (index == 3) {
            index = prev;
        }
    }
    fclRbnSwitchActive(CMB_GBWK->ribbon, index);
}

s32 cmbChkIkenieEnable(void)
{
    if (evtGetAgeOfMoon() != 8) {
        return 0;
    }
    if (CMB_GBWK->unk_4118 != -1) {
        return 0;
    }
    return evtBitCheck(0x25) != 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetAction);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateDecideDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateSacConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateStockCursor2);

void cmbSeqAfterBirth(void)
{
    s32 info = cmpChkSeqInfo(CMB_GBWK);

    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    if (CMB_GBWK->unk_4118 == -1) {
        CMB_GBWK->unk_00001 = 6;
    } else {
        CMB_GBWK->unk_00001 = 8;
    }
    CMB_GBWK->unk_00004 = 2;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateMenuCursor);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

void cmbSeqReturnToPrev(void)
{
    s32 info = cmpChkSeqInfo(CMB_GBWK);

    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    CMB_GBWK->unk_00001 = CMB_GBWK->unk_00002;
    CMB_GBWK->unk_00004 = -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeIntroduceModels);

extern void effEVECombParSmokeDelete(s32 handle);
void cmbFreeSmokeEffect(void)
{
    effEVECombParSmokeDelete(CMB_GBWK->smoke[0]);
    effEVECombParSmokeDelete(CMB_GBWK->smoke[1]);
    effEVECombParSmokeDelete(CMB_GBWK->smoke[2]);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateIntroduce);

extern void fclEncycStartProcess(void);

extern void fclEncycSetTexHandle(s32 *src);

void cmbStartEncycView(void)
{
    fclEncycStartProcess();
    fclEncycSetTexHandle(CMB_GBWK->texHandle);
    fclEncycSetExplainData(CMB_GBWK->explainData);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateResultCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbHandleDebugExit);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdatePanelAnim);

extern datUnitWork_t *cmbGetBirthDevilPanel(void);
extern s32 cmpUpdateStatusModelLoad(u16 id);
s32 cmbUpdateModelLoad(void)
{
    switch (CMB_GBWK->unk_00001) {
    case 7:
    case 8:
    case 10:
    case 11:
    case 15:
        cmpUpdateStatusModelLoad(cmbGetBirthDevilPanel()->id);
        break;
    }
}


INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateSequence);

void func_00203F18(u16 index, u8 type)
{
    D_0032DE20[index].type = type;
}

void func_00203F40(u16 index, f32 value)
{
    if (value < 0.0f) {
        value = 0.0f;
    }
    D_0032DE28[index].unk_00 = value;
}

void func_00203F78(u16 index, f32 scale)
{
    D_0032DE20[index].modelScale = scale;
}

void func_00203FA0(u16 index, f32 scale)
{
    D_0032DE20[index].smokeScale = scale;
}

void func_00203FC8(u16 index, s32 color)
{
    D_0032DE20[index].color = color;
}

u8 cmbGetSmokeType(u16 index)
{
    return D_0032DE20[index].type;
}

f32 func_00204018(u16 index)
{
    return D_0032DE20[index].unk_08;
}

f32 cmbGetModelScaleById(u16 index)
{
    return D_0032DE20[index].modelScale;
}

f32 cmbGetSmokeScale(u16 index)
{
    return D_0032DE20[index].smokeScale;
}

s32 cmbGetSmokeColor(u16 index)
{
    return D_0032DE20[index].color;
}

s32 cmbPrepareModel(s32 group, s32 id, s32 blocking)
{
    return mdlPrepare(group, id, blocking);
}

s32 cmbActionExists(void *work)
{
    s32 result = 0;

    if (cmbPrepareModel(6, *(u16 *)((u8 *)work + 0xC), 0) != 0) {
        result = cmbPrepareModel(6, *(u16 *)((u8 *)work + 0xC), 0) != -1;
    }
    return result;
}

extern void mdlGetFilebase(s32 kind, u16 id, char *out, s32 size);
extern s32 sprintf(char *out, const char *fmt, ...);
extern char D_0053CD68[];

s32 cmbMakeModelPath(u16 id, s32 *size)
{
    char path[0x80];
    char base[0x10];

    mdlGetFilebase(6, id, base, 0x10);
    sprintf(path, D_0053CD68, base);
    return (s32)sdfDevLoadFileEx(path, NULL, size);
}

void cmbInitActionData(s32 id, cmbActionData_t *data)
{
    memset(data, 0, sizeof(*data));
    data->id = id;
}

extern char D_0053CD80[];

void cmbLoadActionModel(s32 id, cmbActionData_t *data)
{
    s32 handle;

    cmbInitActionData(id, data);
    handle = cmbMakeModelPath(id, &data->unk_08);
    data->handle = handle;
    data->lock = sdfMemLock((void *)handle);
    printf(D_0053CD80, (u16)data->id);
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD68);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD80);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeActionModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetActionModelHandle);

extern void cmbGetActionModelHandle(void);
extern void mdlLoopMotion(void *model, s32 a, s32 b);
void *cmbUpdateIntroduceModel(void *work)
{
    void *model;

    cmbGetActionModelHandle();
    model = *(void **)((u8 *)work + 0x10);
    if (model != NULL) {
        mdlLoopMotion(model, 0, 0xD);
        model = *(void **)((u8 *)work + 0x10);
        *(s32 *)model &= ~1;
    }
    return model;
}

s32 func_002043E0(u32 **work)
{
    u32 *flags = work[4];

    if (flags == NULL) {
        return 0;
    }
    *flags |= 1;
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetPanelScale);

INCLUDE_ASM(const s32, "code_001FAE80", cmbRotateModelPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawModelPoly);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawModelAt);

extern void *dds3GetControlScene(void);
extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);
extern void *dds3PointBasicGetResource(void *point);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindCameraPoint);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawActionModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetModelMarkPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawFullScreenPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawModelPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawTargetModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetSceneModel);

extern void sdfModelOtInit(void *table, sdfGrOt_t *first, sdfGrOt_t *second, sdfGrOt_t *third,
                           sdfGrOt_t *fourth);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitModelOt);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CE38);

INCLUDE_ASM(const s32, "code_001FAE80", cmbLoadTargetModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindUnitScene);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateCameraPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateCameraZoom);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetBirthDevilPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateTimeTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbStartSmokeEffect);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetActionWorkValues);

INCLUDE_ASM(const s32, "code_001FAE80", cmbRunActionSe);

INCLUDE_ASM(const s32, "code_001FAE80", cmbActionBGM);

void func_00205AB8(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitAction);

INCLUDE_ASM(const s32, "code_001FAE80", cmb_205CE8);

void func_00205DB8(void)
{
    if (CMB_GBWK->unk_4118 < 0) {
        return;
    }
    func_002043E0(CMB_GBWK->unk_1654C);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmb_205DF0);

void cmb_205E68(void)
{
    mdlSmoothLoopMotion(CMB_GBWK->unk_1654C[4], 0, 1, 0.0f, 1.0f);
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00205EA0);

extern void cmbFreeActionModel(void *model);

void cmbFreeActionModels(void)
{
    cmbFreeActionModel((u8 *)CMB_GBWK + 0x16524);
    cmbFreeActionModel((u8 *)CMB_GBWK + 0x16538);
    if ((s8)CMB_GBWK->unk_4118 >= 0)
    {
        cmbFreeActionModel((u8 *)CMB_GBWK + 0x1654C);
    }
}

s32 cmbChkActScript(void)
{
    return dds3SearchProcessName(D_0053CF10) != NULL;
}

extern char D_0053CF20[];

s32 cmbChkPolygonMovie(void)
{
    void *proc = dds3SearchProcessName(D_0053CF20);

    if (proc == NULL)
    {
        return 0;
    }
    if ((*(s32 *)dds3GetProcessData((s32)proc) & 8) == 0)
    {
        return 1;
    }
    return dds3GetProcessStat(proc) != 3 ? 2 : 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateAction);

INCLUDE_ASM(const s32, "code_001FAE80", func_00206308);

INCLUDE_ASM(const s32, "code_001FAE80", func_002065C8);

INCLUDE_ASM(const s32, "code_001FAE80", func_00206888);

s32 func_00206910(datUnitWork_t *unit)
{
    s32 status;

    if ((unit->flag & 1) == 0) {
        return 0;
    }
    if ((unit->flag & 0x10) != 0) {
        return 0;
    }
    status = unit->badstatus & 0x800;
    return status == 0;
}

void func_00206940(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", shop_206948);

void func_00206A28(void) {
}

void func_00206A30(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", shop_206A38);

s32 shopStartEvent(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    return evtStartEvent(0x322);
}

void fclShopEventEnd(void)
{
    dds3KillProcessName(D_0053CF70, 1);
    fclFreeStage();
}

s32 func_00206BA8(void)
{
    if (D_0054FB20 == NULL) {
        return 0;
    }
    return D_0054FB20->unk_1C;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclShopInitItemList);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CF10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CF20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CF30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CF50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CF70);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopTexLoad);

s32 fclShopTexFree(shpUpdateWork_t *work)
{
    s32 *tex = work->texHandle;
    s8 i;

    for (i = 0; (u8)i < 2; i++) {
        sdfTexFree(*tex++);
    }
    return printf("SHOP : Tex free.\n");
}


void fclShopInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x241C);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclShopGetTexPath);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocShopWork);

INCLUDE_ASM(const s32, "code_001FAE80", shop_update_Shutdown);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopProcessStart);

void fclShopProcessEnd(void)
{
    dds3KillProcessName(D_0054FB48, 1);
    dds3KillProcessName(D_0053D028, 1);
    dds3KillProcessName(D_0053D038, 1);
}

s32 shpCheckProcessStat(void)
{
    s8 stat = shpProcessStat;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            shpProcessStat = 0;
        }
    }
    return 0;
}

void func_00207178(void)
{
    if (D_0054FB20 != NULL) {
        D_0054FB20->flags |= 0x10;
    }
}

s32 func_00207198(s32 value, s8 align)
{
    s8 rem = value % align;

    if (rem != 0) {
        if (align < 0) {
            value -= rem;
        } else {
            value += align - rem;
        }
    } else {
        value += align;
    }
    return value;
}

u16 shpGetItemPrice(s32 index, s8 sell)
{
    u16 price;

    if (sell == 0) {
        price = shopProcWork->price[index];
    } else {
        price = shopProcWork->sellPrice[index];
    }
    return price;
}

INCLUDE_ASM(const s32, "code_001FAE80", shpItemIsUnlocked);

extern s32 shpItemIsUnlocked(void);
extern u16 D_0032F682[];

s32 shpCountUnlockedItems(void)
{
    s32 count = 0;
    u16 *row = &D_0032F682[(u8)shpItemIsUnlocked() * 65];
    s32 i;

    for (i = 0; i < 32; i++)
    {
        if (*row != 0)
        {
            count++;
        }
        row += 2;
    }
    return count;
}

INCLUDE_ASM(const s32, "code_001FAE80", shpFindItemSlot);

INCLUDE_ASM(const s32, "code_001FAE80", shpGetSellPrice);

INCLUDE_ASM(const s32, "code_001FAE80", shpCalcPayOfMoney);

INCLUDE_ASM(const s32, "code_001FAE80", shpChkChoiceMax);

INCLUDE_ASM(const s32, "code_001FAE80", shop_Process);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawPanelRow);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawPrice);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawRowBox);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawRowText);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawBuyRow);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawSellRow);

INCLUDE_ASM(const s32, "code_001FAE80", shopUpdateItemList);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawPanelMode0);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawPanelMode1);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawChoiceCount);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawPricePanel);

s32 shopDrawPhase7(void)
{
    return shopDrawPanelMode0(1);
}

s32 shopUpdateItemListDefault(void)
{
    return shopUpdateItemList(0);
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D028);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D038);

INCLUDE_ASM(const s32, "code_001FAE80", shop_draw_Process);

u8 shpAddServiceTicket(s32 count)
{
    datAddItem(0x3D, count);
    return cmbGlobalWork->unk_1965;
}

INCLUDE_ASM(const s32, "code_001FAE80", shpGetItemFromItemBox);

void shpExit(void)
{
    cmpMoveCursorListEnd(&shopUpdateWork->listCursor);
    shopUpdateWork->unk_0005 = 1;
    shopUpdateWork->unk_0008 =
        fclScriptStart(shopUpdateWork->script, 0xA, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = 4;
}

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqBuy);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqSell);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqBuyNums);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqSellNums);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqPay);

void shpServiceTicketItem(void)
{
    shopUpdateWork->unk_0005 = 1;
    shopUpdateWork->unk_0008 =
        fclScriptStart(shopUpdateWork->script, 0xC, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = 0xB;
}

INCLUDE_ASM(const s32, "code_001FAE80", shpServiceTicket);

void shpUpdateServiceTicket(void)
{
    if (cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork) == 2)
    {
        s32 count = shpAddServiceTicket(0);

        if (count >= 0xA)
        {
            shpServiceTicketItem();
        }
        else
        {
            shpExit();
        }
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateServiceTicketItem);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqRoot);

void shopCheckSeqInfo(void)
{
    s32 info;

    fclSetWorkValue(2, 0);
    info = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);
    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    fclSetWorkValue(2, 2);
    fclScriptStart(shopUpdateWork->script, 8, &shopUpdateWork->scriptArg);
    shopUpdateWork->unk_0001 = (u8)shopUpdateWork->unk_0002;
    shopUpdateWork->unk_0004 = -1;
}

void shopStartScript(void)
{
    s32 info = cmpChkSeqInfo((cmbGbWork_t *)shopUpdateWork);

    if (info == 1) {
        return;
    }
    if (info == 2) {
        fclScriptStart(shopUpdateWork->script, 1, &shopUpdateWork->scriptArg);
        shopUpdateWork->unk_0001 = shopUpdateWork->unk_0002;
        shopUpdateWork->unk_0004 = 1;
    }
}


void shopUpdatePanelAnim(void)
{
    s8 seq = shopUpdateWork->unk_0001;

    switch (seq)
    {
    case 5:
    case 6:
    case 8:
    case 9:
        fclSetPanelAnimState(&shopUpdateWork->anim, 2);
        break;
    default:
        fclSetPanelAnimState(&shopUpdateWork->anim, 1);
        break;
    }
    fclRunPanelAnim(&shopUpdateWork->anim);
}

INCLUDE_ASM(const s32, "code_001FAE80", shop_update_Process);

void fclRagEventStart(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    evtStartEvent(0x323);
    p2sdPlaySE(0x390001, 0x7F, 0x3F);
}

void fclRagEventEnd(void)
{
    dds3KillProcessName(D_0053D140, 1);
    fclFreeStage();
}

s32 func_00209D88(u16 id)
{
    u16 *entry = (u16 *)(ragHeartsTable + 0x160);
    u32 i;

    for (i = 0; i < 0x15; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0x2C);
    }
    return 0;
}

s32 func_00209DD0(u16 id)
{
    u16 *entry = (u16 *)(ragHeartsTable + 0x500);
    u32 i;

    for (i = 0; i < 0x15; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0x2C);
    }
    return 0;
}

s32 func_00209E18(u16 id)
{
    u16 *entry = (u16 *)ragHeartsTable;
    u32 i;

    for (i = 0; i < 0x19; i++) {
        if (id == *entry) {
            return (u8)i;
        }
        entry = (u16 *)((u8 *)entry + 0xE);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", ragGetListItem2);

INCLUDE_ASM(const s32, "code_001FAE80", ragGetListItem3);

INCLUDE_ASM(const s32, "code_001FAE80", ragInitHeartsList);

INCLUDE_ASM(const s32, "code_001FAE80", ragInitList2);

INCLUDE_ASM(const s32, "code_001FAE80", ragInitList3);

INCLUDE_ASM(const s32, "code_001FAE80", ragBuildHeartsList);

INCLUDE_ASM(const s32, "code_001FAE80", ragBuildList2);

INCLUDE_ASM(const s32, "code_001FAE80", ragBuildList3);

INCLUDE_ASM(const s32, "code_001FAE80", ragCreateAllList);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D0C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D0E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D100);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D118);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D130);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D140);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagLoadTextures);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagTexFree);

void fclRagTexLoad(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x4740);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRagUpdateInit);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocRagWork);

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x462C];
    /* 0x4644 */ void *unk_4644;
    /* 0x4648 */ s32 unk_4648;
    /* 0x464C */ s32 unk_464C;
} fclRagWork_t;

extern void fclRagTexFree(u8 *work);
extern void evtDisableMoonDraw(void);

void fclDestroyRag(void)
{
    if (GBWK_0 == NULL) {
        return;
    }
    fclRagTexFree(GBWK_0);
    fclScriptForcedEnd(((fclRagWork_t *)GBWK_0)->unk_464C);
    sdfMemFreeDirect(((fclRagWork_t *)GBWK_0)->unk_4644);
    fclRagEventEnd();
    sdfMemFreeDirect(((fclRagWork_t *)GBWK_0)->allocation);
    GBWK_0 = NULL;
    gProcessStat_1 = 2;
    evtDisableMoonDraw();
    p2sdStopBGM(0x390000);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRagProcessStart);

void fclRagProcessEnd(void)
{
    dds3KillProcessName(D_0054FB98, 1);
    dds3KillProcessName(D_0053D1F0, 1);
    dds3KillProcessName(D_0053D200, 1);
}

s32 ragCheckProcessStat(void)
{
    s8 stat = gProcessStat_1;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            gProcessStat_1 = 0;
        }
    }
    return 0;
}

void func_0020ADE0(void)
{
    if (GBWK_0 != NULL) {
        *GBWK_0 |= 0x10;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", ragGetConfirmEntry);

typedef struct
{
    /* 0x00 */ u16 item;
    /* 0x02 */ s8 count;
    /* 0x03 */ u8 unk_03;
} ragItemReq_t;

s32 ragFindItemSlot(ragItemReq_t *req)
{
    u8 *base = (u8 *)cmbGlobalWork + 8;
    s32 i;

    for (i = 0; i < 3; i++) {
        u32 item = req->item;

        if (item != 0 && ((u8 *)item + (u32)base)[0x1920] < req->count) {
            return 0;
        }
        req++;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", ragSetSeqPhase2);

extern campStatusPanel_t *ragCalcWork;
extern void *dds3GetActiveScene(void);
extern void dds3VisibleSceneBasic(void *scene, s32 flag);
void ragUpdateScene(void)
{
    void *scene;

    if (ragCalcWork->unk_04 != 0) {
        scene = dds3GetActiveScene();
        if (scene != NULL) {
            dds3VisibleSceneBasic(scene, 0);
        }
    }
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D1F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D200);

INCLUDE_ASM(const s32, "code_001FAE80", ragCalc);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawFrameEdge);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawListFrame);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawHeartsFooter);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawListFooter);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawHeartsRows);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawListRows);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawHeartsList);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawList2);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawList3);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawSeqStatus);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawSeqConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", ragDrawSeqErr);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D240);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D250);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D260);

INCLUDE_ASM(const s32, "code_001FAE80", ragDraw);

extern s32 fclScriptStart(s32 script, s32 id, void *arg);

void ragExit(void)
{
    cmpMoveCursorListEnd(&ragUpdateWork->listCursor);
    ragUpdateWork->unk_0005 = 1;
    ragUpdateWork->unk_0008 = fclScriptStart(ragUpdateWork->script, 0xC,
                                             &ragUpdateWork->scriptArg);
    ragUpdateWork->unk_0001 = 5;
}

INCLUDE_ASM(const s32, "code_001FAE80", ragChkItemErr);

INCLUDE_ASM(const s32, "code_001FAE80", ragCheckItemUse);

INCLUDE_ASM(const s32, "code_001FAE80", ragChkDevilLevelErr);

INCLUDE_ASM(const s32, "code_001FAE80", ragSetSeqPhase);

INCLUDE_ASM(const s32, "code_001FAE80", ragGetHeartsFlag);

INCLUDE_ASM(const s32, "code_001FAE80", ragCheckHeartsChoice);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateCallSeq);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqItem);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeq2);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeq3);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateReturn);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqStatus);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqConfirm);

void ragUpdateSeqMitama(void)
{
    fclRunPanelAnim(ragUpdateWork->seqMitama);
}

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqTalk);

extern s32 ragUpdateCallSeq(s32 seq);
void ragUpdateSeqErr(void)
{
    s32 info = cmpChkSeqInfo((cmbGbWork_t *)ragUpdateWork);

    if (info == 1) {
        return;
    }
    if (info == 2) {
        ragUpdateCallSeq(ragUpdateWork->unk_0002);
        ragUpdateWork->unk_0004 = 1;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdate);

s32 fclStartRecoverEvent(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    return evtStartEvent(0x321);
}

void fclRecoverEventEnd(void)
{
    dds3KillProcessName(D_0053D390, 1);
    fclFreeStage();
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D2E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D310);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D338);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D360);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D370);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D380);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D390);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverLoadTextures);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x44D4];
    /* 0x44D4 */ s32 texHandle[3];
} fclRecoverWork_t;

void fclRecoverFreeTextures(fclRecoverWork_t *work)
{
    s32 *tex = work->texHandle;
    s8 i;

    for (i = 0; i < 3; i++) {
        sdfTexFree(*tex++);
    }
    printf("RECOVER : Tex free.\n");
}


extern void lstInitListWindow(s32 a, s32 b, s32 color, s32 d, s32 e, s32 f, s32 g,
                              void *list);

void fclRecoverInitItemList(void *list)
{
    lstInitListWindow(0x1E0, 0x388, 0xFFFFF, 0x1C4, 0xB8, 0x19191E59, 0, list);
    cmpInitCursorLayout((u8 *)list + 0x15C, 0x21, 8, 0x193, 0x1A, 0x23);
}

void fclRecoverInitTargetList(void *list)
{
    lstInitListWindow(0x270, 0x410, 0xFFFFF, 0xEC, 0xA8, 0x19191E59, 0, list);
    cmpInitCursorLayout((u8 *)list + 0x15C, 0x1E, 0xC, 0xC4, 0x1A, 0x1A);
}

void fclRecoverInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x4520);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverUpdateInit);

void fclDestroyRecover(void)
{
    if (D_0054FBB0 == NULL) {
        return;
    }
    fclRecoverFreeTextures((fclRecoverWork_t *)D_0054FBB0);
    fclScriptForcedEnd(D_0054FBB0->unk_44D0);
    sdfMemFreeDirect(D_0054FBB0->unk_44C8);
    fclRecoverEventEnd();
    sdfMemFreeDirect(D_0054FBB0->allocation);
    D_0054FBB0 = NULL;
    gProcessStat_2 = 2;
    evtDisableMoonDraw();
    p2sdStopBGM(0x300000);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverProcessStart);

void fclRecoverProcessEnd(void)
{
    dds3KillProcessName(D_0054FBD8, 1);
    dds3KillProcessName(D_0053D3F8, 1);
    dds3KillProcessName(D_0053D408, 1);
}

s32 rcvCheckProcessStat(void)
{
    s8 stat = gProcessStat_2;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            gProcessStat_2 = 0;
        }
    }
    return 0;
}

void func_0020DBC8(void)
{
    if (D_0054FBB0 != NULL) {
        D_0054FBB0->flags |= 0x10;
    }
}

s32 rcv_20DBE8(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", rcvCalcPriceRecover);

s32 rcvGetStatusPrice(datUnitWork_t *work)
{
    u16 status = work->badstatus;
    s32 price = 0;

    if (status & 0x20) {
        price = 100;
    }
    if (status & 0x40) {
        price = 100;
    }
    if (status & 0x100) {
        price = 100;
    }
    return price;
}

s32 rcvGetRevivePrice(datUnitWork_t *work)
{
    return (work->badstatus & 0x800) ? 400 : 0;
}

s32 rcvGetHealCost(void)
{
    return 400;
}

extern s32 rcvCalcPriceRecover(datUnitWork_t *work);

s32 rcvGetPriceForUnit(datUnitWork_t *work)
{
    s32 price = rcvCalcPriceRecover(work);

    price += rcvGetStatusPrice(work);
    price += rcvGetRevivePrice(work);
    return price;
}

INCLUDE_ASM(const s32, "code_001FAE80", rcvGetRecoverPrice);

s32 rcvAppAllRecover(datUnitWork_t *unit)
{
    unit->hp = datGetMaxHp(unit);
    unit->mp = datGetMaxMp(unit);
    return 1;
}

void rcvClearStatusBits(datUnitWork_t *work)
{
    work->badstatus &= 0xFE9F;
}

void rcvReviveUnit(datUnitWork_t *unit)
{
    unit->badstatus &= 0xF7FF;
    unit->hp = (u32)(datGetMaxHp(unit) + 9) / 10;
}

void rcvClearDeadFlag(datUnitWork_t *work)
{
    work->badstatus &= 0x7FFF;
}

void sfply_AdjustPrepEnd(datUnitWork_t *unit)
{
    rcvReviveUnit(unit);
    rcvAppAllRecover(unit);
    rcvClearStatusBits(unit);
}

INCLUDE_ASM(const s32, "code_001FAE80", rcvFullHeal);

INCLUDE_ASM(const s32, "code_001FAE80", rcvCreateLocalStock);

INCLUDE_ASM(const s32, "code_001FAE80", rcvCalc);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawPanelRow);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawRowBox);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D3F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D408);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D420);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D430);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D440);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawRowText);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawPanelAt);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawSeq1);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawSeq2);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawSeq3);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawSeq4);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDrawItemList);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateItemList);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D460);

INCLUDE_ASM(const s32, "code_001FAE80", rcvDraw);

void rcvExit(void)
{
    cmpMoveCursorListEnd(&rcvUpdateWork->listCursor);
    rcvUpdateWork->unk_0005 = 1;
    rcvUpdateWork->unk_0008 = fclScriptStart(rcvUpdateWork->script, 7,
                                             &rcvUpdateWork->scriptArg);
    rcvUpdateWork->unk_01 = 7;
}

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateData);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateCallSeq);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqAllRecover);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqRecover);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqStatusRecover);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqRevive);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqDispel);

s32 rcvIsHealPhase(void)
{
    if (rcvUpdateWork->unk_01 == 6) {
        if (rcvUpdateWork->unk_03 == 5) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdatePanelAnim);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdateSeqTalk);

INCLUDE_ASM(const s32, "code_001FAE80", rcvUpdate);

s8 trmGetCallMode(void)
{
    return gCallMode;
}

s8 trmGetMode(void)
{
    return trmMode;
}

void fclSetTerminalMode(s8 mode)
{
    trmMode = mode;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D4F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D518);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D528);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D538);

INCLUDE_ASM(const s32, "code_001FAE80", trmGetEventNo);

INCLUDE_ASM(const s32, "code_001FAE80", trmCreateEventProcessName);

INCLUDE_ASM(const s32, "code_001FAE80", trmCreateTerminalList);

void trmEndBGM(void)
{
    p2sdStopBGM(0x20001);
}

INCLUDE_ASM(const s32, "code_001FAE80", trmStartEvent);

INCLUDE_ASM(const s32, "code_001FAE80", trmEndEvent);

extern void trmCreateEventProcessName(s32 mode, char *name);

s32 trmChkEventWait(void)
{
    char name[0x20];

    if (trmGetMode() == 1)
    {
        return 1;
    }
    if (trmGetMode() == 5)
    {
        trmCreateEventProcessName(0, name);
        if (dds3SearchProcessName(name) != 0)
        {
            return 1;
        }
    }
    return 0;
}

void trmSetProcessTerm(void)
{
    if (gCallMode != 0) {
        fclScriptForcedEnd(*(u8 **)(GBWK_3 + 0x2220));
        fclSetTerminalMode(5);
        gProcessStat_3 = 2;
        gCallMode = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalScanCommands);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalInitList);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalInitMenu);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalDumpJump);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalResetCommands);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalLoadTextures);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalFreeTextures);

extern void fclTerminalScanCommands(void *work);
extern void fclTerminalResetCommands(s8 mode, u16 id, void *commands, void *work);
extern void fclTerminalInitList(void *list, s8 count, s32 handle);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalSetMode);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocTerminalWork);

INCLUDE_ASM(const s32, "code_001FAE80", fclDestroyTerminal);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalProcessStart);

void fclTerminalProcessEnd(void)
{
    dds3KillProcessName(D_0053D640, 1);
    dds3KillProcessName(D_0053D650, 1);
    dds3KillProcessName(D_0053D660, 1);
}

s32 trmCheckProcessStat(void)
{
    switch (gProcessStat_3) {
    case 0:
        break;
    case 1:
    case 2:
        return 1;
    }
    return 0;
}

void func_002109E0(void)
{
    if (GBWK_3 != NULL) {
        *GBWK_3 |= 0x10;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", trm_210A00);

s32 trm_210AF8(void)
{
    s32 index = scrGetIntPara(0);

    if ((u32)index < 4) {
        gCommandFlag[index] |= 1;
    }
    return 1;
}

s32 func_00210B38(void)
{
    u32 i;

    for (i = 0; i < 4; i++) {
        gCommandFlag[i] = 0;
    }
    return 1;
}

s32 func_00210B68(void)
{
    s32 count;

    fclTerminalCommandCount[0] &= 0xFD;
    count = GBWK_3[0x3D];
    GBWK_3[0x3D] = count - 1;
    return 1;
}

s32 func_00210B90(void)
{
    s32 count;

    fclTerminalCommandCount[0] |= 2;
    count = GBWK_3[0x3D];
    GBWK_3[0x3D] = count + 1;
    return 1;
}

void trmResetCursor(void)
{
    trmCalcWork->cursor = -1;
    trmCalcWork->unk_2217 = 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", trmCalc);

extern char D_00311450[][0x20];
extern char D_00311850[][0x20];
extern void *frwGetSmallFontFRQ2(s32 x, s32 y, s32 color, s32 d, char *text, s32 f);

void *trmGetListFont(s32 x, s32 y, s32 color, s32 d, s8 index, s8 which, s32 arg6)
{
    char *text;

    if (which == 0)
    {
        text = D_00311450[index];
    }
    else
    {
        text = D_00311850[index];
    }
    return frwGetSmallFontFRQ2(x, y, color, d, text, 0);
}

extern void *trmGetListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f, s32 g);
extern void func_001C7E38(void *frq, s32 mode, s32 arg);
void trmFreeListFont(s32 a, s32 b, s32 c, s32 d, s8 e, s8 f, s32 g)
{
    void *frq = trmGetListFont(a, b, c, d, e, f, g);

    func_001C7E38(frq, 1, g);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawListRow);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawList);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateListSelection);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawSeq0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D640);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D650);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D670);

INCLUDE_ASM(const s32, "code_001FAE80", trmDraw);

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0x38];
    /* 0x003D */ s8 unk_003D;
    /* 0x003E */ u8 unk_003E[0x21B3];
    /* 0x21F1 */ s8 unk_21F1;
    /* 0x21F2 */ u16 unk_21F2;
    /* 0x21F4 */ u8 unk_21F4[0x21];
    /* 0x2215 */ s8 unk_2215;
    /* 0x2216 */ u8 unk_2216[0xA];
    /* 0x2220 */ s32 unk_2220;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ u8 unk_2234[4];
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

s32 trmGetSaveResult(void)
{
    if (trmUpdateWork->unk_21F1 == 0) {
        s32 phase = trmUpdateWork->unk_21F2;

        if (phase < 0x14) {
            if (phase >= 0x12) {
                return 0x1CE006D;
            }
        }
        return 0x1CC006D;
    }
    return 0x1CD006D;
}

INCLUDE_ASM(const s32, "code_001FAE80", trmStartSaveAct);

s32 trmCheckSaveAct(void)
{
    s32 stat = dds3GetProcessStat((void *)trmUpdateWork->saveProcess);

    if ((u32)stat < 4U) {
        if (stat != 0) {
            return 1;
        }
    }
    fclFreeStage();
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", trmGetJumpTerminalNo);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSeqTrans);

void trmActInitMisc(void)
{
    p2sdStopAllSe();
}

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSeqConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateActLearge);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateActSmall);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateAct);

extern s32 fmProcessChk(void);
extern s32 trmGetSaveResult(void);
extern void trmStartSaveAct(s32 mode);

void trmUpdateSave(void)
{
    if (fmProcessChk() == 0)
    {
        p2sdStopBGM(trmGetSaveResult());
        trmStartSaveAct(1);
        trmUpdateWork->unk_0001 = 8;
        trmUpdateWork->unk_0004 = 1;
    }
}

extern void trmEndEvent(s32 mode);
extern void slSetTmnlID(s8 index, u16 id);
extern void fmProcTrigSequence(s32 mode);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSaveActIn);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSaveActOut);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdate);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2120B0);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawListSprite);

void fclDrawBG(fclSprite_t *sprite, s32 flags)
{
    if (sprite == NULL)
    {
        cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0x80, flags);
    }
    else
    {
        fclDrawSpriteEx(0, 0, sprite->unk_04, sprite->unk_06, sprite, flags);
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclSetupSprite);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteRect);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteAlpha);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteFilter);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteSize);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteFull);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteSizeFilter);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteSizeAlphaFilter);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteRes);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_212910);

extern void cmpDrawScrollBar(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h,
                             cmpCursorWork_t *cursor, s32 j, s32 k);

void fcl_2129A0(s32 a, s32 b, cmpCursorWork_t *cursor, s32 d)
{
    cmpDrawScrollBar(cursor->unk_08 * 16, cursor->unk_0C * 8, a, cursor->unk_10,
                     cursor->unk_14, 0x80808080, 0x100, 0, cursor, 0, d);
}

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2129F8);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawStrCenterTarget);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawCaption);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawListText);

INCLUDE_ASM(const s32, "code_001FAE80", trm_212CB8);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPriceText);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawItemCount);

s32 func_00213148(const void *a, const void *b)
{
    u32 keyA = *(u32 *)a;
    u32 keyB = *(u32 *)b;
    u16 subA;
    u16 subB;

    if (keyB < keyA) {
        return -1;
    }
    if (keyA < keyB) {
        return 1;
    }
    subA = *(u16 *)((u8 *)a + 4);
    subB = *(u16 *)((u8 *)b + 4);
    if (subB < subA) {
        return 1;
    }
    if (subA < subB) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00213190);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSkillOrder);

s32 cmbSetEvoResult(const void *a, const void *b)
{
    datDevilInfo_t *eb = *(datDevilInfo_t **)((u8 *)b + 4);
    datDevilInfo_t *ea = *(datDevilInfo_t **)((u8 *)a + 4);

    if (ea->race > eb->race) {
        return 1;
    }
    if (ea->race < eb->race) {
        return -1;
    }
    if (ea->unk_05 > eb->unk_05) {
        return -1;
    }
    return ea->unk_05 < eb->unk_05;
}

void buildSkillSortArray(cmbUnit_t *unit, skillSortEntry_t *out)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        out[i].skill = unit->unit.keisyoskill[i];
        out[i].order = unit->unit.keiattr[i];
    }
}

void applySkillSortArray(cmbUnit_t *unit, skillSortEntry_t *in)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        unit->unit.keisyoskill[i] = in[i].skill;
        unit->unit.keiattr[i] = in[i].order;
    }
}

void apply_213300(void *base, u8 count)
{
    qsort(base, count, 0xEC, func_00213148);
}

void sort_213328(u8 *base)
{
    qsort(&base[0x3A], 0x18, 2, func_00213190);
}

extern s32 cmpSkillOrder(const void *a, const void *b);
void sortSkillList(cmbUnit_t *unit)
{
    skillSortEntry_t order[24];

    buildSkillSortArray(unit, order);
    qsort(order, 0x18, 4, cmpSkillOrder);
    applySkillSortArray(unit, order);
}

typedef struct
{
    /* 0x00 */ u16 index;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ datDevilInfo_t *info;
} cmbRaceSortEntry_t;

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetRaceResult);

INCLUDE_ASM(const s32, "code_001FAE80", lstInitListWindow);

void func_002134E0(u8 *base, s32 mask, s32 index)
{
    base[index + 0x59] |= mask;
}

void func_002134F8(u8 *base, u8 mask, s32 index)
{
    base[index + 0x59] &= ~mask;
}

INCLUDE_ASM(const s32, "code_001FAE80", lstMoveCursor);

INCLUDE_ASM(const s32, "code_001FAE80", lstMoveCursor2);

void lstDrawUpDownCursor(lstWork_t *work)
{
    s32 flags = work->flags & 0xFFFF0000;

    if (work->listCursor.unk_22 != 0) {
        flags |= 1;
    }
    if ((work->listCursor.unk_22 + work->listCursor.unk_26) != work->listCursor.unk_20) {
        flags |= 2;
    }
    work->flags = flags;
}

INCLUDE_ASM(const s32, "code_001FAE80", lstDrawListWindow);

INCLUDE_ASM(const s32, "code_001FAE80", lstDrawListWindowCursor);

extern void lstMoveCursor(lstWork_t *work, s32 dir);

/* NON_MATCHING: 1 of 29 words differ (first at +0x30). */
#ifdef NON_MATCHING
s32 lstPageUp(lstWork_t *work)
{
    cmpCursorWork_t *cursor = &work->listCursor;
    s32 count = cursor->unk_26;
    s32 i;

    for (i = 0; i < count; i++) {
        if (cursor->unk_04 != 0) {
            break;
        }
        lstMoveCursor(work, -1);
    }
    return i;
}
#else
INCLUDE_ASM(const s32, "code_001FAE80", lstPageUp);
#endif

/* NON_MATCHING: 1 of 29 words differ (first at +0x30). */
#ifdef NON_MATCHING
s32 lstPageDown(lstWork_t *work)
{
    cmpCursorWork_t *cursor = &work->listCursor;
    s32 count = cursor->unk_26;
    s32 i;

    for (i = 0; i < count; i++) {
        if (cursor->unk_04 != 0) {
            break;
        }
        lstMoveCursor(work, 1);
    }
    return (s32)i;
}
#else
INCLUDE_ASM(const s32, "code_001FAE80", lstPageDown);
#endif

INCLUDE_ASM(const s32, "code_001FAE80", datGetItemSlotIndex);

INCLUDE_ASM(const s32, "code_001FAE80", fclAddItem);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

s32 fcl_213998(char *name, fclSceneObj_t *dst)
{
    fclSceneObj_t *obj = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, name);

    if (obj == NULL) {
        return 0;
    }
    *(s32 *)(dst->unk_18 + 0x80) = *(s32 *)(obj->unk_18 + 0x78);
    return 1;
}


INCLUDE_ASM(const s32, "code_001FAE80", fclLoadStage);

extern void dds3SetBlur(s32 a, s32 b, s32 c);
extern void dds3MultiBlurOff(s32 a);
extern void dds3DistBlurOff(s32 a);
extern void Eve_BlurOFF(void);
extern void Eve_PartialMultiBlurOFF(void);
extern void Eve_PartialMultiFilterOFF(void);
extern void Eve_DistortionBlurOFF(void);
extern void Eve_FilterOFF(void);
void fclFilterOFF(void)
{
    dds3SetBlur(0, 0, 0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    Eve_BlurOFF();
    Eve_PartialMultiBlurOFF();
    Eve_PartialMultiFilterOFF();
    Eve_DistortionBlurOFF();
    Eve_FilterOFF();
}

void fclFreeStage(void)
{
    evtFreeStage();
    while (sdfDelayedResrcCheck() != 0) {
    }
    fclFilterOFF();
}

void fclInitPanelPos(s32 a, s32 b, s32 c, s16 d, s16 e, s32 f, fclPanel_t *panel)
{
    panel->unk_00 = a;
    panel->unk_04 = b;
    panel->unk_08 = c;
    panel->unk_0C = d;
    panel->unk_0E = e;
    panel->unk_10 = f;
}

void fclScriptForcedEnd(u8 *script)
{
    if (script != NULL && dds3GetProcessStat(script) != 0) {
        dds3KillProcess(script, 0);
        script = NULL;
    }
}

extern s32 scrStartScript2(s32 id, s32 script, s32 arg);

s32 fclScriptStart(s32 script, s32 id, void *arg)
{
    s32 handle;

    if (arg != NULL)
    {
        fclScriptForcedEnd(*(u8 **)arg);
    }
    handle = scrStartScript2(0x7D0, script, id);
    fclClearFlag(0);
    if (arg != NULL)
    {
        *(s32 *)arg = handle;
    }
    return handle;
}

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void func_00213BB8(fclItemList_t *list)
{
    s32 item;

    list->count = 0;
    for (item = 1; item < 0xA0; item++) {
        if (((u8 *)item + (u32)cmbGlobalWork)[0x1928] != 0) {
            list->item[list->count++] = item;
        }
    }
}


INCLUDE_ASM(const s32, "code_001FAE80", fclCreateItemListEx);

s32 cmpItemOrder(u8 *a, u8 *b)
{
    u8 va = *a;
    u8 vb = *b;

    if (va > vb) {
        return 1;
    }
    if (va < vb) {
        return -1;
    }
    return 0;
}

s32 func_00213CD8(u8 *list, s32 count, u8 value)
{
    s32 kept = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        if (list[i] != value) {
            u8 entry = list[i];

            list[i] = 0;
            list[kept] = entry;
            kept++;
        }
    }
    return kept;
}

INCLUDE_ASM(const s32, "code_001FAE80", shuffleIndices);

extern char *text2[];
char *func_00213DC0(datUnitWork_t *unit)
{
    char *name = unit->namecode;

    if (name[0] != 0) {
        return name;
    }
    return text2[unit->id];
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00213DF0);

INCLUDE_ASM(const s32, "code_001FAE80", fclBlendColor);

extern s32 itfMesMngInitialize(void);
extern void itfMesMngChangeWindowType(s32 msg, s32 type, s32 flag);
s32 fclInitMessage(void)
{
    s32 ret;

    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize();
        itfMesMngChangeWindowType(gMsgID, 2, 0);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

extern void itfMesMngMarkBitStat(s32 msg, s32 bits);
extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);
extern void itfMesMngSetSelPhase(s32 msg, s32 phase);

s32 fclStartMessage(s32 id)
{
    if (gMsgID < 0)
    {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, 0x200000);
    itfMesMngRequestMessage(gMsgID, id, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = 1;
    return 1;
}

s32 fclStartSelMessage(s32 no)
{
    if (gMsgID < 0) {
        return 0;
    }
    gSelMsgNo = no;
    return 1;
}

void fclSetSelMessagePos(s8 pos)
{
    if (gMsgID >= 0) {
        gSelMsgPos = pos;
    }
}

s8 fclGetCurrentSelMessagePos(void)
{
    return gSelMsgPos;
}

s8 fclGetSelMessagePos(void)
{
    if (gMsgID < 0) {
        return -1;
    }
    return itfMesMngGetSelectNo(gMsgID);
}

extern void itfMesMngSetPhase(s32 msg, s32 phase);
extern void itfMesMngMsgWndCls(s32 msg);
extern void itfMesMngRequestMessageRelease(s32 msg, s32 flag);
extern void fclSetMessageInvalid(s8 flag);

s8 fclEndMessageEx(s8 flag)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngSetPhase(gMsgID, 0);
    if (flag != 0) {
        itfMesMngMsgWndCls(gMsgID);
    }
    itfMesMngRequestMessageRelease(gMsgID, 0);
    fclSetMessageInvalid(1);
    gMsgFlag = 0;
    return 1;
}

s8 fclEndMessage(void)
{
    return fclEndMessageEx(1);
}

s32 fclDestroyMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngRelease(gMsgID);
    gMsgID = -1;
    gMsgFlag = 0;
    gMsgInvFlag = 0;
    return 1;
}

s32 fclChkMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    if (gMsgInvFlag != 0 && gMsgFlag == 2) {
        return 0;
    }
    return (s8)(u8)gMsgFlag;
}

s32 fclChkSelMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    return itfMesMngGetSelPhase(gMsgID) >= 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclUpdateMessage);

void fclSetMessageVar_20743088(s8 index, s32 value)
{
    itfMesMngSetVarImmediate(gMsgID, index, value);
}

s8 fcl_214270(void)
{
    return gMsgInvFlag;
}

extern void itfMesMngClearBitStat(s32 msg, s32 bits);

void fclSetMessageInvalid(s8 flag)
{
    if (flag != 0) {
        itfMesMngClearBitStat(gMsgID, 0x800000);
        itfMesMngClearBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 0;
        itfMesMngSetPhase(gMsgID, 1);
        gMsgFlag = 1;
    } else {
        itfMesMngMarkBitStat(gMsgID, 0x800000);
        itfMesMngMarkBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 1;
    }
}

extern void itfMesMngSetMesPosition(s32 msg, s32 x, s32 y);
extern void itfMesMngSetOffsetY1(s32 msg, s32 y);

void fcl_2142F0(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

s32 fclChkMessageDraw(void *proc)
{
    if (dds3GetProcessStat(proc) == 1)
    {
        return 1;
    }
    if (dds3GetProcessStat(proc) == 2)
    {
        return 1;
    }
    return dds3GetProcessStat(proc) == 3;
}

void fclClearFlag(s8 index)
{
    D_00551F80[index] = 0;
}

s32 fclGetFlag(s8 index)
{
    s8 flag = D_00551F80[index];

    if (flag != 0) {
        return 1;
    }
    return 0;
}

s32 fclSetWorkValue(s8 index, s32 value)
{
    if (index >= 0x10) {
        return 0;
    }
    fclWorkValue[index] = value;
    return 1;
}

s8 fclGetWorkValue(s8 index)
{
    if (index >= 0x10) {
        index = 0xF;
    }
    return fclWorkValue[index];
}

s32 fcl_214428(void)
{
    D_00551F80[scrGetIntPara(0)] = 1;
    return 1;
}

s32 fcl_214458(void)
{
    s32 index = scrGetIntPara(0);

    if (index >= 0x10) {
        index = 0xF;
    }
    scrSetIntReturnValue(fclWorkValue[index]);
    return 1;
}

s32 fclLoadFileDone(s32 unused, s32 cmd, s32 value, s32 unused2, s32 *out)
{
    if (cmd == 8) {
        *out = value;
    }
    return 0;
}

void fclLoadFileAsync(const char *name, void *buf)
{
    sdfFileCmdReadFile(name, 0, -1, fclLoadFileDone, buf);
}

extern void *sdfDevLoadFileEx(char *name, void **data, s32 *size);
extern s32 sdfTexSet(void *data);
s32 fclLoadTexture(char *name)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(name, &data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}

void fclSetSpriteColor(u8 *sprite, u32 color)
{
    sprite[0x1C] = color >> 24;
    sprite[0x1D] = color >> 16;
    sprite[0x1E] = color >> 8;
    sprite[0x1F] = color;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclSetSpriteEx);

void fclSetSprite(fclSprite_t *sprite, s32 x, s32 y, s32 d, s32 u0, s32 v0, s32 u1, s32 v1,
                  s32 color, s32 tex)
{
    u0 <<= 4;
    v0 <<= 4;
    u1 <<= 4;
    v1 <<= 4;
    sprite->unk_04 = x;
    sprite->unk_0E = v1;
    sprite->tex = tex;
    sprite->unk_18 = d;
    sprite->unk_06 = y;
    sprite->unk_08 = u0;
    sprite->unk_0A = v0;
    sprite->unk_0C = u1;
    sprite->unk_14 = 0;
    sprite->unk_10 = 0;
    fclSetSpriteColor((u8 *)sprite, color);
}

void fclSetSpriteNormal(fclSprite_t *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    fclSetSprite(a, b, c, d, e, f, e + b, f + c, g, h);
}

void fclDrawSprite(s32 a, s32 b, fclSprite_t *sprite)
{
    fclDrawSpriteEx(a, b, sprite->unk_04, sprite->unk_06, sprite, 0x47);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteEx);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteCore);

INCLUDE_ASM(const s32, "code_001FAE80", fclCalcRotPos);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawSpriteAngleCore);

INCLUDE_ASM(const s32, "code_001FAE80", func_00214FD8);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagCalcTexSize);

extern s32 fclRagCalcTexSize(s32 size, s32 mode);

void fclRagTexCalcSizes(fclRagTex_t *tex)
{
    s32 mode = tex->unk_14;

    tex->unk_04 = fclRagCalcTexSize(tex->unk_0C, mode);
    tex->unk_08 = fclRagCalcTexSize(tex->unk_10, mode);
}

void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, void (*calc)())
{
    tex->unk_00 = 0;
    tex->unk_04 = a;
    tex->unk_08 = b;
    tex->unk_0C = c;
    tex->unk_10 = d;
    tex->unk_14 = e;
    tex->unk_18 = f;
    tex->calcSizes = calc;
    calc(tex);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRunPanelAnim);

void fclSetPanelAnimState(fclPanelAnim_t *anim, u8 state)
{
    anim->animState = state;
}

s32 fclGetPanelAnimTarget(fclPanelAnim_t *anim)
{
    return anim->unk_04;
}

s32 fclGetPanelAnimPhase(fclPanelAnim_t *anim)
{
    return anim->unk_14;
}

void func_00215188(fclPanelAnim_t *anim, s32 value)
{
    anim->unk_1C = value;
}

s32 fcl_215190(fclPanelAnim_t *anim)
{
    return anim->unk_1C;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPhaseValue);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelBorder);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPhaseHeader);

void fclDrawPanelTypeDef(fclPanel_t *panel, s32 flags)
{
    s32 color = panel->unk_10;

    fclDrawPanelSprite2(0x44, flags);
    cmpDrawBoxF(panel->unk_00, panel->unk_04, panel->unk_08, (u16)panel->unk_0C,
                (u16)panel->unk_0E, color, flags);
}

void fclUpdatePanelTypeDef(fclPanel_t *panel)
{
    panel->unk_18 += panel->unk_14;
    if (panel->unk_18 > 0x100) {
        panel->unk_18 = 0x100;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType0Grad);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType0);

void fclUpdatePanelType0(fclPanel_t *panel)
{
    panel->unk_18 += panel->unk_14;
    if (panel->unk_18 > 0x100) {
        panel->unk_18 = 0x100;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType1);

void fclUpdatePanelType1(fclPanel_t *panel)
{
    panel->unk_18 += panel->unk_14;
    if (panel->unk_18 > 0x200) {
        panel->unk_18 = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType2);

void fclUpdatePanelType2(fclPanel_t *panel)
{
    panel->unk_18 += panel->unk_14;
    if (panel->unk_18 > 0x100) {
        panel->unk_18 = 0x100;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelCorners);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType3BackGrad);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelEdges);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelType3);

void fclUpdatePanelType3(fclPanel_t *panel)
{
    panel->unk_18 += panel->unk_14;
    if (panel->unk_18 >= 0x201) {
        panel->unk_18 = 0;
    }
    panel->unk_1C += 0x20;
    if (panel->unk_1C >= 0x201) {
        panel->unk_1C = 0x200;
    }
}

void fclSetPanelTex(fclPanel_t *panel, s32 a, s32 b, s32 c, s32 d, s32 e)
{
    panel->unk_24 = a;
    panel->unk_28 = b;
    panel->unk_2C = c;
    panel->unk_30 = d;
    panel->unk_34 = e;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclInitPanel);

INCLUDE_ASM(const s32, "code_001FAE80", fclCreatePanel);

s32 fclDestroyPanel(void *panel)
{
    sdfChipFreeDirect(panel);
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanel);

void fclUpdatePanel(fclPanel_t *panel)
{
    panel->update(panel);
}

s32 FclDebug(void)
{
    return -1;
}

s32 fclChkDbgProcess(void)
{
    return fclDbgPID != 0;
}

s32 GetPriceForSummon(s32 arg0, s32 arg1, s32 arg2, s32 level, s32 str, s32 mag, s32 vit, s32 agi)
{
    s32 total = level + str + mag + vit + agi;

    return total * total / 20 * 100;
}

s32 fclEncycWork(void)
{
    return g_pinst != NULL;
}

void fclEncycInitRecord(s32 mode)
{
    s32 *count = &((dds3GlobalWorkView_t *)DDS3_GBWK)->recordCount;

    if (mode == 0) {
        *count = 0;
    }
    g_pinst = NULL;
    g_pdebug = NULL;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D6D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D6E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D708);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycStartProcess);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D768);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D778);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D798);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycKillProcess);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycSetTexHandle);

void fclEncycSetExplainData(void *data)
{
    g_pinst->explainData = data;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycRegist2);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycRegist);

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u8 unk_02[0xAE];
} fclEncycEntry_t;

s32 fclEncycSearch(s32 devilId)
{
    fclEncycEntry_t *entry = (fclEncycEntry_t *)((u8 *)cmbGlobalWork + 0x11E90);
    s32 count = cmbGlobalWork->encycNum;
    s32 i;

    for (i = 0; i < count; i++) {
        if (devilId == entry->devilId) {
            return i;
        }
        entry++;
    }
    return -1;
}


s32 fclEncycGetNum(void)
{
    return cmbGlobalWork->encycNum;
}

s32 fclEncycGetRatio(void)
{
    s32 num = fclEncycGetNum();
    s32 ratio = num * 100 / 0xB8;

    if (num > 0) {
        if (ratio <= 0) {
            ratio = 1;
        }
    }
    return ratio;
}


s32 fclEncycCheckRegistable(void)
{
    return GetValidUnitIndex(0);
}

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycDebugRegist);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycDebugProc);

void fclEncycDebugShutdown(void)
{
    ReleaseDebug_fclEncyc();
}

void InitializeInstance(fclEncycWork_t *work)
{
    work->unk_6D = 1;
    work->modelId = -1;
    work->flags = 0;
    work->unk_58 = 0;
    work->unk_5A = 0;
    work->listCursor[0] = 0;
    work->listCursor[1] = 0;
    work->listCursor[2] = 0;
    work->listCursor[3] = 0;
    work->unk_6C = 0;
    work->unk_70 = 0;
    work->explainData = NULL;
    work->unk_364 = 0;
    work->unk_36C = 0;
    SetScreenOffset(work, 0);
}


extern void *sdfMemAllocDirect(s32 size);
extern s32 sdfMemLock(void *ptr);

extern void *sdfMemAllocDirect(s32 size);
extern s32 sdfMemLock(void *ptr);

void allocPhaseBuffer(fclEncycWork_t *work, s32 size)
{
    void *buf = sdfMemAllocDirect(size);
    s32 lock = sdfMemLock(buf);

    work->allocation = buf;
    work->unk_36C = lock;
}

void ReleaseWork(fclEncycWork_t *work)
{
    if (work->unk_36C != 0) {
        sdfMemFreeDirect(work->allocation);
        work->unk_36C = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", CALC_ENCYC_Process);

extern void SetCameraLightParam(fclEncycWork_t *work);
extern void (*DrawPhaseTable[])(fclEncycWork_t *work);

s32 DRAW_ENCYC_Process(void)
{
    fclEncycWork_t *work = g_pinst;
    void (*phase)(fclEncycWork_t *work);

    if (work == NULL) {
        return 0;
    }
    if ((work->flags & 0x20) == 0) {
        return 0;
    }
    SetCameraLightParam(work);
    phase = DrawPhaseTable[(s16)work->unk_58];
    if (phase != NULL) {
        phase(work);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", UPDT_ENCYC_Process);

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseRegistTop);

extern void ReleaseFontList(void **list, s32 count);

void DestPhaseRegistTop(fclEncycWork_t *work)
{
    s32 base = work->unk_36C;

    if (*(void **)(base + 0x11DC) != NULL)
    {
        frDestroyFontRequestAfter(*(void **)(base + 0x11DC));
        *(void **)(base + 0x11DC) = NULL;
    }
    ReleaseFontList((void **)(base + 0x11E0), 7);
    if (*(s32 *)(base + 0x11FC) != 0)
    {
        dds3KillProcess(*(s32 *)(base + 0x11FC), 0);
    }
    ReleaseWork(work);
}

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseRegistConfirm);

extern void fclRbnRelease(fclRbnWork_t *work);

void DestPhaseRegistConfirm(fclEncycWork_t *work)
{
    void *scene;

    fclRbnRelease(*(fclRbnWork_t **)(work->unk_36C + 0x200));
    cmpResetDrawEnv();
    scene = dds3GetActiveScene();
    if (scene != NULL)
    {
        dds3VisibleSceneBasic(scene, 1);
    }
    ReleaseWork(work);
}

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseRegistAll);

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseReadTop);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseReadTop);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseReadTop);

INCLUDE_ASM(const s32, "code_001FAE80", DestPhaseReadTop);

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseReadStatus);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseReadStatus);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseReadStatus);

INCLUDE_ASM(const s32, "code_001FAE80", DestPhaseReadStatus);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseReadSummon);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseReadSummon);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseReadExplain);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseReadExplain);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseReadView);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseReadView);

INCLUDE_ASM(const s32, "code_001FAE80", AcceptKeyRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", CalcConfirmStatus);

INCLUDE_ASM(const s32, "code_001FAE80", GetValidUnitIndex);

INCLUDE_ASM(const s32, "code_001FAE80", RegistAll);

INCLUDE_ASM(const s32, "code_001FAE80", InitListWindow);

void PushListwinCursor(fclEncycWork_t *pinst, lstListWindow_t *plistwin, s32 stack)
{
    cmpCursorPos_t *cur = &plistwin->cursorPos;

    pinst->listCursor[stack * 2] = cur->index;
    pinst->listCursor[stack * 2 + 1] = cur->shift;
}

INCLUDE_ASM(const s32, "code_001FAE80", PopListwinCursor);

void AdjustStackCursor(s32 count, s16 *cursor, s16 *scroll)
{
    s32 pos = *cursor;
    s32 top = *scroll;

    if (count <= 0) {
        return;
    }
    if (pos + 7 < count) {
        return;
    }
    if (pos > 0) {
        pos--;
    } else if (pos + top >= count) {
        top = count - 1;
    }
    *cursor = pos;
    *scroll = top;
}

void ResetReadMainRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_0C = 0;
    rbn->unk_10 = 0x58;
    rbn->unk_14 = 0x480;
    rbn->unk_18 = 0xA8;
    rbn->unk_1C = 0x80;
    rbn->unk_20 = 0;
    rbn->unk_24 = 0x100;
}

INCLUDE_ASM(const s32, "code_001FAE80", AcceptKeyChangeSort);

INCLUDE_ASM(const s32, "code_001FAE80", ApplyChangeListIndex);

INCLUDE_ASM(const s32, "code_001FAE80", AcceptKeyListWindow);

extern s32 cmpRaceOrder(const void *a, const void *b);
void CreateSortIndexByRace(s16 *index)
{
    s32 count = fclEncycGetNum();

    FillIndex(index, count);
    qsort(index, count, 2, cmpRaceOrder);
}

extern s32 cmpLevelOrder(const void *a, const void *b);
void CreateSortIndexByLevel(s16 *index)
{
    s32 count = fclEncycGetNum();

    FillIndex(index, count);
    qsort(index, count, 2, cmpLevelOrder);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpRaceOrder);

INCLUDE_ASM(const s32, "code_001FAE80", cmpLevelOrder);

void FillIndex(s16 *dst, s32 count)
{
    s16 i;

    for (i = 0; i < count; i++) {
        *dst++ = i;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", SetElem);

INCLUDE_ASM(const s32, "code_001FAE80", SetElem2);

extern s32 fclEncycSearch(s32 devilId);
extern void SetRecordToUnitCore(void *unit, void *record);

void SetRecordToUnit(datUnitWork_t *unit, s32 devilId)
{
    u8 *base = (u8 *)DDS3_GBWK + 0x11E80;
    s32 index = fclEncycSearch(devilId);

    SetRecordToUnitCore(unit, base + index * 0xB0 + 0x10);
}

INCLUDE_ASM(const s32, "code_001FAE80", SetRecordToUnitCore);

INCLUDE_ASM(const s32, "code_001FAE80", CompareUnitToRecord);

INCLUDE_ASM(const s32, "code_001FAE80", ExecSummon);

INCLUDE_ASM(const s32, "code_001FAE80", drawRegistList);

INCLUDE_ASM(const s32, "code_001FAE80", CreateFontListRead);

INCLUDE_ASM(const s32, "code_001FAE80", ReleaseFontList);

INCLUDE_ASM(const s32, "code_001FAE80", CreateFontExplain);

extern void itfMesMngRequestMessageRelease(s32 msg, s32 flag);

void ReleaseFontExplain(fclEncycWork_t *work, s32 release)
{
    s32 base = work->unk_36C;

    itfMesMngRequestMessageRelease(*(s32 *)(base + 8), 0);
    if (*(void **)(base + 0x88) != NULL)
    {
        frDestroyFontRequestAfter(*(void **)(base + 0x88));
        *(void **)(base + 0x88) = NULL;
    }
    if (release != 0)
    {
        itfMesMngRelease(*(s32 *)(base + 8));
    }
}

void FadeOutRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_1C -= 0x10;
    if (rbn->unk_1C < 0) {
        rbn->unk_1C = 0;
    }
    rbn->unk_20 += 0x20;
    rbn->unk_0C -= rbn->unk_20;
    rbn->unk_14 -= rbn->unk_24;
    rbn->unk_24 -= 0x20;
}

void FadeInRibbon(fclReadRibbon_t *rbn)
{
    rbn->unk_1C += 0x10;
    if (rbn->unk_1C >= 0x81) {
        rbn->unk_1C = 0x80;
    }
    rbn->unk_24 += 0x20;
    rbn->unk_0C += rbn->unk_20;
    rbn->unk_20 -= 0x20;
    rbn->unk_14 += rbn->unk_24;
}

INCLUDE_ASM(const s32, "code_001FAE80", GetDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", PrepSummon);

INCLUDE_ASM(const s32, "code_001FAE80", drawSummonCount);

INCLUDE_ASM(const s32, "code_001FAE80", drawEncycRatio);

s32 CalcFadeAlpha(s16 *alpha, s32 up, s32 limit, s32 step)
{
    s16 v = *alpha;
    s32 more = 1;

    if (up != 0) {
        v = v + step;
        if (v > limit) {
            v = limit;
            more = 0;
        }
    } else {
        v = v - step;
        if (v < 0) {
            v = 0;
            more = 0;
        }
    }
    *alpha = v;
    return more;
}


INCLUDE_ASM(const s32, "code_001FAE80", CalcListElemColor);

void RequestModelLoad(fclEncycWork_t *work, s32 id)
{
    if (cmpGetEncycWork() != 0) {
        if (id == cmpGetStatusModelId()) {
            return;
        }
    }
    *(u16 *)((u8 *)work + 0x6E) = id;
    work->flags |= 4;
}


void SetScreenOffset(fclEncycWork_t *work, s32 flag)
{
    if (flag != 0)
    {
        work->screenOfsX = 2188.0f;
        work->screenOfsY = 2048.0f;
    }
    else
    {
        work->screenOfsX = 2048.0f;
        work->screenOfsY = 2048.0f;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", MoveScreenOffset);

INCLUDE_ASM(const s32, "code_001FAE80", SetCameraLightParam);

INCLUDE_ASM(const s32, "code_001FAE80", ControlModel);

INCLUDE_ASM(const s32, "code_001FAE80", GetViewModeKey);

INCLUDE_ASM(const s32, "code_001FAE80", GetAnalogPad);

INCLUDE_ASM(const s32, "code_001FAE80", InitCameraForViewMode);

INCLUDE_ASM(const s32, "code_001FAE80", SetBoundControl);

INCLUDE_ASM(const s32, "code_001FAE80", initViewCamera);

INCLUDE_ASM(const s32, "code_001FAE80", InitModelPosRot);

void RequestChangeMotion(motionWork_t *work, s32 loop)
{
    s32 flags = work->unk_54 & ~0xC0;

    work->unk_54 = flags | 0x40;
    if (loop != 0) {
        work->unk_54 = flags | 0xC0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", ChangeModelMotion);

INCLUDE_ASM(const s32, "code_001FAE80", CirculateDevilObject);

INCLUDE_ASM(const s32, "code_001FAE80", ChangeDevilObjectRegistConfirm);

extern void RequestModelLoad(fclEncycWork_t *work, s32 id);

void ChangeDevilObjectReadMain(s32 index, fclEncycWork_t *work)
{
    s32 base = work->unk_36C;
    u16 *record = (u16 *)((u8 *)DDS3_GBWK + index * 0xB0 + 0x11E90);

    *(u16 *)base = index;
    RequestModelLoad(work, *record);
    SetRecordToUnitCore((void *)(base + 0x90), record);
}

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveRegistKeyHelp);

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveReadMainHelp);

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveViewModeRibbon);

extern void fclDrawPanel(fclPanel_t *panel, s32 id);
extern void lstDrawListWindow(void *list, s32 mode, s32 id);
void DrawListWindowBack(void *list, fclPanel_t *panel)
{
    fclDrawPanel(panel, 0x47);
    fclUpdatePanel(panel);
    lstDrawListWindow(list, 0, 0x47);
}

INCLUDE_ASM(const s32, "code_001FAE80", drawRegistSprites);

INCLUDE_ASM(const s32, "code_001FAE80", DrawRegistListwinElem);

INCLUDE_ASM(const s32, "code_001FAE80", DrawReadListwinCaption);

INCLUDE_ASM(const s32, "code_001FAE80", DrawReadListwinElem);

INCLUDE_ASM(const s32, "code_001FAE80", DrawReadStatusMoney);

INCLUDE_ASM(const s32, "code_001FAE80", DrawExplainBack);

INCLUDE_ASM(const s32, "code_001FAE80", DrawBackBlackBoard);

INCLUDE_ASM(const s32, "code_001FAE80", DrawRegistAllHelp);

INCLUDE_ASM(const s32, "code_001FAE80", DrawRegistKeyHelp);

INCLUDE_ASM(const s32, "code_001FAE80", DrawReadMainHelp);

void InitializeDebug_fclEncyc(void)
{
    void *buf = sdfMemAllocDirect(0xC);
    fclRbnWork_t *work = (fclRbnWork_t *)sdfMemLock(buf);

    work->allocation = buf;
    work->unk_04 = 0;
    work->unk_06 = 0;
    work->unk_08 = 0;
    work->unk_0A = 0;
    g_pdebug = work;
}

void ReleaseDebug_fclEncyc(void)
{
    if (g_pdebug != NULL) {
        sdfMemFreeDirect(g_pdebug->allocation);
        g_pdebug = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", DebugProcMenu_fclEncyc);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D7E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D828);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D838);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D848);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D858);

INCLUDE_ASM(const s32, "code_001FAE80", DebugProcRegister);

INCLUDE_ASM(const s32, "code_001FAE80", DebugProcViewkey);

INCLUDE_ASM(const s32, "code_001FAE80", DebugProcDispBound);

INCLUDE_ASM(const s32, "code_001FAE80", DebugRemoveRecord);

INCLUDE_ASM(const s32, "code_001FAE80", DebugUpdateListwin);

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnInit);

void fclRbnRelease(fclRbnWork_t *work)
{
    sdfMemFreeDirect(work->allocation);
}

void fclRbnCalc(fclRbnWork_t *work)
{
    if (work->flags & 1) {
        MoveActive(work);
    }
    if (work->flags & 2) {
        MoveDeactive(work);
    }
    work->phase = (work->phase - 2) & 0x7F;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnDraw);

void fclRbnSwitchActive(fclRbnWork_t *work, s32 index)
{
    if (index >= 0 && index < work->count && index != work->unk_10)
    {
        work->unk_12 = work->unk_10;
        work->flags |= 3;
        work->unk_10 = index;
        work->activeTime = 0x40;
        work->deactiveTime = 0x80;
        work->phase = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnSetPara);

INCLUDE_ASM(const s32, "code_001FAE80", dumpCursorInfo);

void MoveActive(fclRbnWork_t *work)
{
    work->activeTime -= 8;
    if (work->activeTime <= 0) {
        work->activeTime = 0;
        work->flags &= ~1;
    }
}

void MoveDeactive(fclRbnWork_t *work)
{
    work->deactiveTime -= 0x10;
    if (work->deactiveTime <= 0) {
        work->deactiveTime = 0;
        work->flags &= ~2;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", DrawBackFrame);

INCLUDE_ASM(const s32, "code_001FAE80", DrawBack);

INCLUDE_ASM(const s32, "code_001FAE80", DrawHexagon);

INCLUDE_ASM(const s32, "code_001FAE80", DrawHexagonElem);

INCLUDE_ASM(const s32, "code_001FAE80", DrawElem);

INCLUDE_ASM(const s32, "code_001FAE80", rstInitEvoAct);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D9D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D9E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D9F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DA08);

s32 rstChkEvoAct(void)
{
    char name[10] = "e809_main";

    return dds3SearchProcessName(name) != 0;
}

void rstKillEvoAct(void)
{
    fclFreeStage();
    printf(D_0053DA28);
}

void rstSetTestParam(void)
{
    cmbGlobalWork->party[0].unit.exp += 100;
    cmbGlobalWork->party[4].unit.exp += cmbGlobalWork->party[4].unit.level * 100;
}

s32 rstChkLevelUpTarget(datUnitWork_t *unit)
{
    if (unit->flag & 1) {
        if (!(unit->badstatus & 0x800)) {
            if (cmbGetLevelUpCount(unit, 0) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern s32 rstChkLevelUpTarget(datUnitWork_t *unit);

s32 rstChkResultEntry(void)
{
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        if (rstChkLevelUpTarget(&cmbGlobalWork->party[i].unit) != 0)
        {
            return 1;
        }
    }
    return 0;
}

extern s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp);

u8 rstCalcLevelFromExp(u8 index, u32 exp)
{
    datUnitWork_t *unit = &cmbGlobalWork->party[index].unit;

    return (u8)unit->level + cmbGetLevelUpCount(unit, exp - unit->exp);
}

s32 rstCalcExpFromLevel(u8 id, s32 level)
{
    return datCopyDevilWorkEntry(&cmbGlobalWork->party[id].unit, level);
}

void rstChkResult2Process(u8 *work)
{
    memset(&work[0x129F], 0, 6);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstCreateTargetList);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DA28);

INCLUDE_ASM(const s32, "code_001FAE80", rstInit);

INCLUDE_ASM(const s32, "code_001FAE80", rstDestroyResult2);

INCLUDE_ASM(const s32, "code_001FAE80", rstResult2ProcessStart);

void rstResult2ProcessEnd(void)
{
    dds3KillProcessName(D_0054FCF0, 1);
    dds3KillProcessName(D_0053DA90, 1);
    dds3KillProcessName(D_0053DAA0, 1);
}

s32 checkResultStat(void)
{
    if (gProcessStat_4 == 1) {
        return 1;
    }
    if (gProcessStat_4 < 2) {
        return 0;
    }
    if (gProcessStat_4 == 2) {
        gProcessStat_4 = 0;
    }
    return 0;
}

void rstSetLevelUpFlag(void)
{
    if (GBWK_1 != NULL) {
        *GBWK_1 |= 0x10;
    }
}

void rstAddExp(s32 amount, datUnitWork_t *work)
{
    if (work->level < 0xFF) {
        work->level += amount;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", rstAdvanceLevelUpStep);

s32 func_0021FE18(void)
{
    return EventStart->unk_22E5 >= 0;
}

void func_0021FE30(s8 a, s8 b, f32 x, f32 y)
{
    EventStart->unk_22E5 = a;
    EventStart->unk_22E8 = x;
    EventStart->unk_22EC = y;
    EventStart->unk_22E6 = b;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawEvoModel);

void rstSetEvoState(void)
{
    EventStart->unk_22E6 = 2;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstSetMotionReqFromTbl);

extern s32 cmbChkSkillOwner(u16 skillId, datUnitWork_t *unit);
s32 rstChkAddSkill(u16 skillId)
{
    if (cmbChkSkillOwner(skillId, EventStart->unit) >= 0) {
        return 0;
    }
    return (EventStart->unit->skill[2] >= 8) ? 2 : 1;
}

s16 rstGetEquipHeartsAisyo(void)
{
    return datGetDevilParam(cmbGlobalWork->equipHearts);
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DA90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DAA0);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetDestroySkillMaxHpMp);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetMaxHpMp);

extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);

s32 rstChkParamLimitAll(datUnitWork_t *unit)
{
    s32 i;

    for (i = 0; i < 6; i++)
    {
        if (i == 1)
        {
            continue;
        }
        if (cmbGetParamBase(unit, i) < 0x28)
        {
            return 0;
        }
    }
    rstSetMaxHpMp(0, (s32)unit);
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstSetLevelUpCount);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetCurrentDevil);

INCLUDE_ASM(const s32, "code_001FAE80", rstInitWorkStock);

INCLUDE_ASM(const s32, "code_001FAE80", rstCreateBeforeSkillList);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkBeforeSkillOwner);

INCLUDE_ASM(const s32, "code_001FAE80", rstAutoAsignDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcEventInfo);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSeqPlayerLevelUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSeqDevilLevelUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstPickRandomComment);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetDefaultSkill);

s32 rstChkSkillPowerUp1(void)
{
    s32 ret;

    if (cmbGlobalWork->party[0].unit.level < 7)
    {
        return 0;
    }
    if (EventStart->unit->id == 0)
    {
        dds3GetRandA(0);
        ret = 1;
    }
    else
    {
        ret = (dds3GetRandA(0) & 3) == 0;
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstChkSkillPowerUp2);

INCLUDE_ASM(const s32, "code_001FAE80", rstRndGetPowerUpSkill);

s32 rstChkDevilSkillPowerUp(datUnitWork_t *work)
{
    s32 flag = work->flag & 0x40;

    if (flag != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstChkPartyDevilSkillPowerUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSkillPowerUpCore);

extern s32 rstChkSkillPowerUp1(void);
extern s32 rstCalcSkillPowerUpCore(void);

s32 rstCalcSkillPowerUp(void)
{
    s32 result = rstChkSkillPowerUp1();

    if (result != 0) {
        result = rstCalcSkillPowerUpCore();
    }
    return result;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstChkEvoEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcEvo);

extern s32 rstFindEvoEntryType7(datUnitWork_t *unit);
extern s32 cmbChkDevilEvo(datUnitWork_t *unit);

s32 rstCalcSeqEvoAt(void)
{
    s32 level;

    if (rstFindEvoEntryType7(EventStart->unit) >= 0)
    {
        level = cmbGlobalWork->party[0].unit.level + 6;
        if (level < cmbChkDevilEvo(EventStart->unit))
        {
            return -1;
        }
        return 0;
    }
    return -1;
}

s32 rstCalcSeqDestroySkill(void)
{
    cmpStatSetTimer(0, 2);
    rstSetDestroySkillMaxHpMp();
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstChkHeartsSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstRecalcHeartsSkill);

void rstLoadHeartsSkill(void) {
}

void rstStoreHeartsSkill(void) {
}

void rstInitSkillAct(s8 flag)
{
    cmpStatSetTimer(0, 1);
    rstSetSkillActFlag(flag);
    EventStart->unk_2308 = 0x1E;
    p2sdPlaySE(0x2B0003, 0x7F, 0x3F);
}

void func_002215B8(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", rstFindRegisteredDevil);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindDevilSlot);

s32 rstShakePad(void)
{
    return dds3PadVibrationSet(1, 0xFF, 0x1E);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstCalc);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetParamBase);

extern s32 datGetParam(datUnitWork_t *unit, s32 index);
s32 cmbGetParam(datUnitWork_t *unit, s32 index)
{
    s32 value;

    if (unit->id == 0) {
        value = unit->param[index];
    } else {
        value = datGetParam(unit, index);
    }
    return value;
}

s8 func_00221DB0(s8 index)
{
    return D_00339E00[index].unk_19;
}

s32 cmbChkGetHearts(s8 id)
{
    s32 i;

    for (i = 0; i < ((u8 *)cmbGlobalWork)[0x1D7B]; i++) {
        if (cmbGlobalWork->unk_01CFB[i] == id) {
            return 1;
        }
    }
    return 0;
}


s32 datFindCommentEntry(u16 devilId)
{
    datCommentEntry_t *entry = D_00339DF8;
    u32 i;

    for (i = 0; i < 1; i++, entry++) {
        if (entry->id == devilId) {
            return entry->comment;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", datGetPlayerLNDLevel);

s32 datGetDevilRace(s8 index)
{
    return D_00339E00[index].race & 7;
}

INCLUDE_ASM(const s32, "code_001FAE80", datGetPlayerLNDList);

s8 datGetDevilSlotIndex(u8 devilId)
{
    s8 count = 0;
    s32 i;

    if (devilId == 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (D_00339E00[devilId].heartsSkill[i].skill != 0) {
            count++;
        }
    }
    return count;
}


void func_00222120(u8 devilId)
{
    cmbGlobalWork->devilSlot[devilId] &= 0xF0;
}

s32 datDevilIsRegistered(u8 devilId)
{
    u8 flag = cmbGlobalWork->devilSlot[devilId] & 0x80;

    return flag != 0;
}

s8 datCountPlayerLND(void)
{
    s32 count = 0;
    s32 i = 0;

    if (*(u8 *)((s32)DDS3_GBWK + 0x1D7B) != 0)
    {
        do
        {
            if (datDevilIsRegistered(*(u8 *)(i + (s32)DDS3_GBWK + 0x1CFB)) != 0)
            {
                count++;
            }
            i++;
        } while (i < *(u8 *)((s32)DDS3_GBWK + 0x1D7B));
    }
    return count;
}

s32 datGetDevilSlotState(u8 devilId)
{
    return cmbGlobalWork->devilSlot[devilId] & 0xF;
}

void rstSetDevilRegistered(s8 devilId)
{
    cmbGlobalWork->devilSlot[devilId] |= 0x80;
}

u16 rstGetHeartsSkillAt(u8 race, s8 index)
{
    if (index >= 8) {
        return 0;
    }
    return D_00339E00[race].heartsSkill[index].skill;
}

extern s8 D_00339E1A[];

/* The row stride is not a multiple of the entry stride, so the index is in bytes. */
s8 func_00222250(u8 index, s8 slot)
{
    if (slot >= 8) {
        return -1;
    }
    return D_00339E1A[slot * 4 + index * 0x42];
}

s16 datGetDevilParam(u8 devilId)
{
    if (devilId == 0) {
        return 0;
    }
    return D_00339E00[devilId].param;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetHeartsParamByIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetHeartsParamEx);

extern s32 cmbGetHeartsParamEx(s8 devilId, s8 index, s32 registered);

s32 cmbGetHeartsParam(s8 devilId, s8 index)
{
    s32 param;

    if (datDevilIsRegistered(devilId) != 0)
    {
        param = cmbGetHeartsParamEx(devilId, index, 1);
    }
    else
    {
        param = cmbGetHeartsParamEx(devilId, index, 0);
    }
    return param;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddHeartsParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSubHeartsParam);

INCLUDE_ASM(const s32, "code_001FAE80", datAddHeartsSlot);

INCLUDE_ASM(const s32, "code_001FAE80", datAddHeartsEntry);

INCLUDE_ASM(const s32, "code_001FAE80", func_00222610);

INCLUDE_ASM(const s32, "code_001FAE80", rstCheckHeartsSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitStockUnit);

INCLUDE_ASM(const s32, "code_001FAE80", cmb_222828);

s8 cmbCheckEvoUnlocked(u16 devilId, datUnitWork_t *list, u8 count)
{
    s32 i = 0;

    if (count != 0) {
        do {
            if ((list->flag & 1) != 0) {
                if (list->id == devilId) {
                    return i;
                }
            }
            i++;
            list++;
        } while (i < count);
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCountEvoCandidates);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkEvoNeedFlag);

INCLUDE_ASM(const s32, "code_001FAE80", func_002229E8);

INCLUDE_ASM(const s32, "code_001FAE80", datFindKugatatiEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoStats);

s32 cmbCheckEvoPair(u16 maskA, u16 maskB)
{
    s32 i;

    for (i = 0; i < 0x10; i++) {
        if ((maskA >> i) & 1) {
            return ((u32)maskB >> i) & 1;
        }
    }
    return 0;
}

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u8 value;
    /* 0x03 */ u8 unk_03;
} cmbEvoEntry_t;

extern cmbEvoEntry_t D_00339658[];
s32 cmbFindEvoEntry(u16 devilId)
{
    cmbEvoEntry_t *entry = D_00339658;
    u32 i;

    for (i = 0; i < 0xE3; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->value;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoDevil);

extern u16 D_00339BA8[];
s32 cmbEvoEntryExists(u16 devilId)
{
    u16 *entry = D_00339BA8;
    u32 i;

    for (i = 0; i < 0x25; i++, entry++) {
        if (devilId == *entry) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCountEvoReady);

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 value;
} cmbPairResult_t;

extern cmbPairResult_t D_00339BF8[];
s32 func_00222DC0(u16 devilId)
{
    cmbPairResult_t *entry = D_00339BF8;
    u32 i;

    for (i = 0; i < 6; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->value;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddLevelUpParamEx);

typedef struct
{
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ s8 value;
    /* 0x03 */ u8 kind;
} cmpKugatatiEntry_t;

extern u8 datLevelUpTable[];

s8 rst_223080(datUnitWork_t *unit)
{
    return cmbAddLevelUpParamEx(unit, 0);
}

/* NON_MATCHING: 3 of 27 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 rstFindEvoEntryType7(datUnitWork_t *unit)
{
    cmpKugatatiEntry_t *entry = (cmpKugatatiEntry_t *)&datLevelUpTable[unit->id * 0x66];
    u8 level = unit->level;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (entry[1].kind == 7 && (u8)entry[1].value <= level) {
            return (s8)i;
        }
        entry++;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_001FAE80", rstFindEvoEntryType7);
#endif

/* NON_MATCHING: 3 of 27 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 rstFindEvoEntryType5(datUnitWork_t *unit)
{
    cmpKugatatiEntry_t *entry = (cmpKugatatiEntry_t *)&datLevelUpTable[unit->id * 0x66];
    u8 level = unit->level;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (entry[1].kind == 5 && (u8)entry[1].value < level) {
            return (s8)i;
        }
        entry++;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_001FAE80", rstFindEvoEntryType5);
#endif

/* NON_MATCHING: 4 of 29 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 cmbChkDevilEvo(datUnitWork_t *unit)
{
    u8 *p = &datLevelUpTable[unit->id * 0x66 + 8];
    s32 i;

    for (i = 0; i < 24; i++) {
        if (p[-1] == 5) {
            datDevilInfo_t *info = &datDevilTable[*(u16 *)p];

            return (s8)info->unk_05;
        }
        p += 4;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_001FAE80", cmbChkDevilEvo);
#endif

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindKugatatiMsgEntry);


INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcEvoEvent);

s32 rstCalcEvoLevelDiff(datUnitWork_t *a, datUnitWork_t *b, datUnitWork_t *c, s32 level)
{
    s32 limit = 0xFF;
    s32 diff;

    if (level >= 0) {
        limit = 0x63 - (b->level - level);
    }
    diff = (c->level - a->level) - 4;
    return (limit < diff) ? limit : diff;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoDevilToStock);

typedef struct
{
    /* 0x00 */ u16 skill;
    /* 0x02 */ u16 result;
} rstPowerUpEntry_t;

extern rstPowerUpEntry_t D_00339C20[];
s32 rstFindPowerUpSkill(u16 skillId)
{
    rstPowerUpEntry_t *entry = D_00339C20;
    u32 i;

    for (i = 0; i < 0x37; i++, entry++) {
        if (skillId == entry->skill) {
            return entry->result;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstRollPowerUpSkill);

s32 datCalcDevilExp(s32 level)
{
    s32 exp = (s32)((f32)(level * level * level) * 1.42f + -5.0f);

    if (exp < 0) {
        exp = 0;
    }
    return exp;
}

INCLUDE_ASM(const s32, "code_001FAE80", datCopyDevilWorkEntry);

s32 datAdvanceDevilLevel(datUnitWork_t *work)
{
    return datCopyDevilWorkEntry(work, work->level + 1);
}

typedef struct
{
    /* 0x00 */ u16 devilId;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ s8 unk_05;
} cmbMitamaEntry_t;

extern cmbMitamaEntry_t D_00339D10[];
s32 func_00223748(u16 devilId)
{
    cmbMitamaEntry_t *entry = D_00339D10;
    u32 i;

    for (i = 0; i < 0x26; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->unk_05;
        }
    }
    return 0;
}

s32 func_00223788(u16 devilId)
{
    cmbMitamaEntry_t *entry = D_00339D10;
    u32 i;

    for (i = 0; i < 0x26; i++, entry++) {
        if (entry->devilId == devilId) {
            return entry->unk_04;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstRollComment);

INCLUDE_ASM(const s32, "code_001FAE80", func_00223890);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetLevelUpCount);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawGetCursorRow);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusRow);


INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusRow2);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusPanel);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindUnitScene);

extern s32 rstFindUnitScene(s32 work);
s32 rstGetEncycPanel(void)
{
    s32 work;

    if (rstDrawWork->unk_7D == 0) {
        work = cmpGetEncycWork();
        if (work != 0) {
            return rstFindUnitScene(work);
        }
    }
}

s32 rstResetEncycPanel(void)
{
    s8 phase = rstDrawWork->unk_129E;

    if (phase < 2) {
        if (phase >= 0) {
            return rstGetEncycPanel();
        }
    }
    *(s32 *)(*(u8 **)((u8 *)cmpGetEncycWork() + 0x18) + 0x80) = 0;
}

s32 rstResetEncycPanel2(void)
{
    if (rstDrawWork->unk_7C < 5) {
        return rstGetEncycPanel();
    }
    *(s32 *)(*(u8 **)((u8 *)cmpGetEncycWork() + 0x18) + 0x80) = 0;
}

void rstDrawEvoBgFade(void)
{
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, rstDrawWork->fadeColor, 7);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstDraw);

void rstOverWriteSkill(u16 *slot, u16 skill)
{
    *slot = skill;
    rstSetMaxHpMp(1, startPos->unk_16C);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstAddSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstBuildDestroyList);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x22];
    /* 0x22 */ u8 unk_22;
    /* 0x23 */ u8 unk_23;
    /* 0x24 */ u8 unk_24;
} rstSkillSlot_t;

u16 rstSkillSlotIsFull(cmbUnitSkills_t *skills, rstSkillSlot_t *slot)
{
    s8 index = slot->unk_22 + slot->unk_24;

    if (index >= skills->skillCount) {
        if (startPos->unk_16C->id == 0) {
            return startPos->unk_24;
        }
        return startPos->unk_22;
    }
    return skills->skill[index];
}


s32 rstWaitConfirmButton(void)
{
    s32 timer;

    if (startPos->unk_2308 > 0) {
        if (dds3PadButtonState[0x21] < 0) {
            rstClearFlag(1);
            startPos->unk_2308 = 1;
        }
        timer = startPos->unk_2308;
        if (timer == 1) {
            startPos->unk_2308 = 0;
            return 2;
        }
        startPos->unk_2308 = timer - 1;
        return 1;
    }
    return 0;
}

void rstResetAsignParam(void)
{
    startPos->unk_19 = startPos->unk_18;
    rstChkResult2Process((u8 *)startPos);
}

void func_00224230(datUnitWork_t *unit)
{
    s32 i;

    for (i = 0; i < 6; i++) {
        unit->param[i] += startPos->unk_129F[i];
    }
}

void rstAddAsignParam(void)
{
    func_00224230(startPos->unk_16C);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqAsignPlayerParam);

void rstUpdateSeqPlayerLevelUpOnly(void)
{
    if (startPos->unk_129E == 0)
    {
        fclStartMessage(1);
        startPos->unk_129E = 1;
    }
    if (fclChkMessage() == 0)
    {
        startPos->unk_0001 = 1;
        startPos->unk_0004 = 1;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqGift);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDefaultSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqSkillPowerUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstStartIntroduce);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateEvoBgFade);

void rstStartEvoActSE(void)
{
    s32 timer = startPos->unk_78;
    s32 next;

    if (timer < 0) {
        return;
    }
    next = timer + 1;
    startPos->unk_78 = next;
    if (next != 0x28) {
        return;
    }
    p2sdPlaySE(0x2B0001, 0x7F, 0x3F);
    startPos->unk_78 = -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqEvoAct);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqEvo);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqEvoAt);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDestroySkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetHeartsSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDestroyConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqHeartsSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstStandbyHeartsAddPoint);

INCLUDE_ASM(const s32, "code_001FAE80", rstStandbyHeartsSkillPowerUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetHeartsRecover);

void rstSetHeartsRecoverPlayer(void)
{
    dds3PartyUnit_t *pu = &cmbGlobalWork->party[0];

    pu->unit.hp = pu->unit.maxhp;
    pu->unit.mp = pu->unit.maxmp;
    pu->unit.badstatus &= 0x8000;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstStandbyHeartsEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetHeartsEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqHeartsEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqHeartsMaster);

void rstUpdateBgTimer(void)
{
    startPos->unk_2310++;
    if (startPos->unk_2310 >= 0x900) {
        startPos->unk_2310 = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdate);

void func_002262C8(void) {
}

void func_002262D0(void) {
}

void func_002262D8(void) {
}

void func_002262E0(void) {
}

extern char D_0054FD18[]; /* "/sound/" */
extern char *strcat(char *dst, const char *src);
extern void *memcpy(void *dst, const void *src, u32 size);

char *sdfSndMakeFilename(char *dst, const char *name)
{
    memcpy(dst, D_0054FD18, 8);
    return strcat(dst, name);
}

extern s8 D_0054FD10;
extern s32 D_0054FD20;
extern s32 D_0054FD24;
extern s16 D_00551F94;
extern char D_00342FA0[];
extern char D_00342FC0[];
extern void *D_0033A540[];
extern void sdfSndInit(char *a, s32 b, char *c, s32 d);
extern void p2sdLoadSMGFile(void *file);

void sndInit(void)
{
    if (D_0054FD10 != 0) {
        p2sdStopAllSe();
        return;
    }
    D_0054FD10 = 1;
    sdfSndInit(D_00342FA0, 4, D_00342FC0, 4);
    p2sdLoadSMGFile(D_0033A540[0]);
    D_0054FD20 = 4;
    D_00551F94 = -1;
    D_0054FD24 = 0;
}

s32 snd_226388(void)
{
    return 0x7B0;
}

s32 snd_226390(void)
{
    p2sdPlayBGM(scrGetIntPara(0));
    return 1;
}

s32 dds3_2263B8(void)
{
    p2sdPlaySE(scrGetIntPara(0), 0x7F, 0x3F);
    return 1;
}

s32 dds3_2263E8(void)
{
    p2sdStopAllSe();
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", dds3TitleUpdateCursor);

INCLUDE_ASM(const s32, "code_001FAE80", dds3TitleDrawMenu);

INCLUDE_ASM(const s32, "code_001FAE80", dds3TitleDrawFade);

void dds3TitleClearFade(void)
{
    titleWork->unk_34 = 0;
    titleWork->unk_30 = 0;
}

void dds3TitleSetFade(void)
{
    titleWork->unk_34 = 0xC0;
    titleWork->unk_30 = 0x32;
}

INCLUDE_ASM(const s32, "code_001FAE80", dds3TitleUpdateFade);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DCD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DCE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DCF8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD18);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD58);

INCLUDE_ASM(const s32, "code_001FAE80", titleLoadTextures);

INCLUDE_ASM(const s32, "code_001FAE80", titleFreeTextures);

INCLUDE_ASM(const s32, "code_001FAE80", titleAllocWork);

s32 titleFreeWork(s32 arg)
{
    titleFreeTextures();
    sdfMemFreeDirect(titleWork->allocation);
    titleWork = NULL;
    movieStop();
    dds3KernelDebugSign = 1;
    return 1;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD98);

INCLUDE_ASM(const s32, "code_001FAE80", dds3Titlemain);

s32 titleStart(s32 arg)
{
    titleAllocWork(arg);
    return -1;
}

s32 titleExit(void)
{
    titleFreeWork(0);
    dds3KillProcessName(D_0053DD98, 1);
    return 0;
}

s32 msel_2286A0(void)
{
    titleAllocWork(0);
    return 0;
}

extern void *frwGetSmallFontFRQ(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);
extern void frDrawFontEx(void *frq, s32 mode);
void mselDrawMenuText(s32 x, s32 y, s32 text, s32 arg)
{
    void *frq = frwGetSmallFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

extern void *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);
void mselDrawSaveNum(s32 x, s32 y, s32 text, s32 arg)
{
    void *frq = frwGetSmallNumFontFRQ(x * 16, y * 16, 0, text, arg, 0);

    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "code_001FAE80", mselUpdateCursor);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawTitleAnim);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawBackground);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawTitleAnim2);

INCLUDE_ASM(const s32, "code_001FAE80", mselToggleFlag);

INCLUDE_ASM(const s32, "code_001FAE80", mselMakeSaveText);

INCLUDE_ASM(const s32, "code_001FAE80", mselDevilChoice);

void Msel_TOverData_Clear(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        gGlobal->tover[i] = -1;
    }
    gGlobal->unk_60 = 0;
}

void mselCountSaves(void)
{
    mselGlobalWork_t *work;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        gGlobal->unk_94[i] = gGlobal->tover[i];
    }
    work = gGlobal;
    work->unk_90 = work->unk_60;
    if (work->unk_60 != 0)
    {
        work->unk_88 = 0;
        work->unk_8C = 0x64618780;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", mselFindSaveSlot);

extern char D_0053DEC0[];
extern char D_0053DED8[];

void mselLoadBackground(void)
{
    gGlobal->unk_40 = fclLoadTexture(D_0053DEC0);
    gGlobal->unk_44 = 0;
    gGlobal->unk_48 = fclLoadTexture(D_0053DED8);
}

extern void sdfTexFreeDelayed(s32 handle);

void mselFreeTextures(void)
{
    if (gGlobal->unk_40 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_40);
        gGlobal->unk_40 = 0;
    }
    if (gGlobal->unk_44 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_44);
        gGlobal->unk_44 = 0;
    }
    if (gGlobal->unk_48 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_48);
        gGlobal->unk_48 = 0;
    }
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DEA8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DEC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DED8);

INCLUDE_ASM(const s32, "code_001FAE80", mselShutDown);

extern void mselFreeTextures(void);
extern void sdfMemFreeDelayedDirect(void *ptr);
void mselFreeResources(s32 arg)
{
    mselFreeTextures();
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(gGlobal->unk_04);
    sdfMemFreeDelayedDirect(gGlobal->unk_08);
    sdfMemFreeDelayedDirect(gGlobal->unk_58);
    sdfMemFreeDelayedDirect(gGlobal->unk_00);
    gGlobal = NULL;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DF08);

INCLUDE_ASM(const s32, "code_001FAE80", mselMain);

s32 mselExit(void)
{
    mselShutDown();
    return -1;
}

s32 mselInit(void)
{
    mselFreeResources(0);
    dds3KillProcessName(D_0053DF08, 1);
    return 0;
}

s32 fldWmapIsEnabled(void)
{
    return fldWmapEnabled;
}

INCLUDE_ASM(const s32, "code_001FAE80", mselStartDataSet);

extern s32 fldRunDanteGunFlag[];
extern void fldSetNextField(s32 a, s32 b, s32 c);
extern void dds3AdminiCallSequence(s32 a, void *b, s32 c, s32 d);

INCLUDE_ASM(const s32, "code_001FAE80", mselStartGame);

s32 dds3CreateGameData(void)
{
    evtFreeStage();
    func_00116BB8();
    return dds3GlobalWorkInit();
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DF90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DFA8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DFB8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DFC8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DFD8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DFF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E008);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E020);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E030);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E040);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E050);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E060);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E078);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E088);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E0F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E108);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E118);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E130);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E148);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E158);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E168);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E178);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E190);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E1A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E1C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E1D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E1E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E1F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E200);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E218);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E228);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E240);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E250);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E260);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E270);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E280);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E290);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E2A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E2C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E2D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E2E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E2F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E308);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E318);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E328);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E338);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E348);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E360);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E370);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E380);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E390);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E3A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E3B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E3C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E3D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E3F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E400);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E410);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E420);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E430);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E440);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E458);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E468);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E478);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E488);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E498);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E4A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E4B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E4C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E4D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E4E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E500);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E510);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E520);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E530);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E540);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E550);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E560);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E570);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E580);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E590);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E5F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E608);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E618);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E630);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E648);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E678);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E690);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E6A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E6B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E6C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E6E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E6F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E700);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E710);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E720);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E730);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E740);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E750);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E760);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E770);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E780);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E798);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E7B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E7C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E7D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E7E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E7F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E808);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E818);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E828);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E838);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E848);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E878);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E890);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E8A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E8B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E8D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E8E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E8F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E910);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E920);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E930);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E950);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E968);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E978);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E988);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E998);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E9A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E9B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E9D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053E9E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EA98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EAA8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EAB8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EAD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EAE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EAF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB18);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB68);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EB90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EBF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EC90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ECA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ECB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ECC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ECD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ECE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED18);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053ED90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EDA8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EDC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EDD8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EDF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE68);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EE98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EEB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EEC8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EEE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EEF8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EF88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053EFA0);

INCLUDE_ASM(const s32, "code_001FAE80", stfFreeFiles);

INCLUDE_ASM(const s32, "code_001FAE80", stfFreeFont);

INCLUDE_ASM(const s32, "code_001FAE80", stfDrawStaffText);

INCLUDE_ASM(const s32, "code_001FAE80", stfDrawText);

INCLUDE_ASM(const s32, "code_001FAE80", stfLoadUITexture);

void stfFreeTextures(void)
{
    if (oStaff_UI->unk_0004 != 0) {
        sdfTexFree(oStaff_UI->unk_0004);
    }
    if (oStaff_UI->unk_0008 != 0) {
        sdfTexFree(oStaff_UI->unk_0008);
    }
    if (oStaff_UI->unk_000C != 0) {
        sdfTexFree(oStaff_UI->unk_000C);
    }
    oStaff_UI->unk_0004 = 0;
    oStaff_UI->unk_0008 = 0;
    oStaff_UI->unk_000C = 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", stfGetUIField);

void stfResetUI(void)
{
    stfFreeTextures();
    oStaff_UI->unk_00FC = 0;
    oStaff_UI->unk_0100 = 0;
    oStaff_UI->unk_0104 = 0;
    oStaff_UI->unk_0114 = 0;
    oStaff_UI->unk_0108 = 0;
    oStaff_UI->unk_010C = 0;
    oStaff_UI->unk_0118 = 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", stfMain2);

INCLUDE_ASM(const s32, "code_001FAE80", GetoStaffUI);

INCLUDE_ASM(const s32, "code_001FAE80", ObjDispOff);

extern s32 p2sdGetPcmState(void);
extern void p2sdSyncSound(void);
extern void stfFreeFont(void);
void stfDestroy(void)
{
    dds3VblanksPerFrame = 2;
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    stfFreeTextures();
    stfFreeFont();
    sdfMemFreeDirect(oStaff_UI->allocation);
    oStaff_UI = NULL;
}

INCLUDE_ASM(const s32, "code_001FAE80", stfShutDown);

s32 stfExit(void)
{
    stfShutDown();
    return -1;
}

s32 stfInit(void)
{
    dds3KillProcessName(D_0053F100, 1);
    return 0;
}

s32 movieDrawProc(void)
{
    release_movieobj2(D_00343E40, &sdfGrOtTable[57]);
    return 0;
}

extern void movieOpenStream(dds3MovieEntry_t *table, char *name, s32 no);
extern char D_0053F3C8[];

s32 moviePlayFile_27041744(s32 no, char *name)
{
    if (PlayingNo == 0)
    {
        movieOpenStream(D_00343E40, name, no);
        PlayingNo =
            (s32)dds3StartProcess(D_0053F3C8, 0x2AFB, 1, 1, movieDrawProc, NULL, NULL);
    }
}

s32 dds3PlayMovieByIndex(s32 index)
{
    dds3MovieEntry_t *entry = &D_00343BE8[index];

    return moviePlayFile_27041744(entry->unk_00, entry->name);
}

void movieStop(void)
{
    if (PlayingNo != 0) {
        movieCloseStream(D_00343E40);
        dds3KillProcess(PlayingNo, 0);
        PlayingNo = 0;
    }
}

s32 dds3WaitMovieEnd(void)
{
    return sdfMovieIsFinished(D_00343E40);
}

u8 func_0022B7F8(void)
{
    return D_00343E74[0];
}

s32 movieMan_Process(void)
{
    sdfIpuRunDecode(60 / dds3VblanksPerFrame);
    return 0;
}

extern void sdfIpuInit(void);
extern char D_0053F3D8[];

void *dds3StartMovieManager(void)
{
    sdfIpuInit();
    return dds3StartProcess(D_0053F3D8, 0x385, 1, 0, movieMan_Process, NULL, NULL);
}

s32 dds3_22B880(void)
{
    dds3PlayMovieByIndex(scrGetIntPara(0));
    return 1;
}

s32 dds3_22B8A8(void)
{
    movieStop();
    return 1;
}

s32 movie_22B8C8(void)
{
    scrSetIntReturnValue(dds3WaitMovieEnd() == 0);
    return 1;
}

s32 movie_22B8F0(void)
{
    return func_0022B7F8() == 2;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ void *chip;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
} movieViewerChipList_t;

extern movieViewerChipList_t movieViewerChips asm("movieViewerWork");

void movieViewerFree(void)
{
    void *chip = movieViewerChips.chip;

    if (chip != NULL) {
        do {
            void *next = *(void **)chip;

            sdfChipFreeDirect(chip);
            chip = next;
        } while (chip != NULL);
        movieViewerChips.chip = NULL;
        movieViewerChips.unk_08 = 0;
        movieViewerChips.unk_0A = 0;
        movieViewerChips.unk_0C = 0;
    }
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F100);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F110);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F130);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F150);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F170);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F190);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F1A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F1C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F1E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F1F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F210);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F228);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F248);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F260);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F278);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F290);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F2B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F2C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F2E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F308);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F320);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F340);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F360);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F378);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F390);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F3B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F3C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F3D8);

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerScanDir);

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerStep);

INCLUDE_ASM(const s32, "code_001FAE80", movieDebugDrawInfo);

INCLUDE_ASM(const s32, "code_001FAE80", MovieViewerDrawFrameInfo);

INCLUDE_ASM(const s32, "code_001FAE80", MovieViewerProc);

extern void movieViewerScanDir(void);
extern s32 MovieViewerProc(void);
extern void *movieViewerWork[];
extern char D_0053F418[];

void InitMovieViewer(void)
{
    movieViewerScanDir();
    movieViewerWork[0] =
        dds3StartProcess(D_0053F418, 0x2B02, 1, 0, MovieViewerProc, NULL, NULL);
}

extern char D_0053F418[];
extern void *movieViewerWork[];
extern void movieViewerFree(void);

void FinishMovieViewer(void)
{
    void *proc = dds3SearchProcessName(D_0053F418);

    if (proc != NULL) {
        dds3KillProcess((s32)proc, 0);
        movieViewerWork[0] = NULL;
        movieStop();
    }
    movieViewerFree();
}

void movieViewerSetFlags(void)
{
    movieViewerDebugFlag.unk_02 = 1;
    movieViewerDebugFlag.unk_03 = 1;
}

void movieViewerInitRegs(void)
{
    movieViewerDebugFlag.regAddr = 0x10002010;
    movieViewerDebugFlag.buffer = D_00343E40;
    movieViewerSetFlags();
}

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerDrawInfo);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F418);

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerDebugDraw);

void rstSetResultWork(void *work)
{
    D_00345D9C[0] = work;
}

void rstClearResultWork(void)
{
    D_00345D9C[0] = NULL;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F438);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F448);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F458);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F468);

INCLUDE_ASM(const s32, "code_001FAE80", cmpTexLoad);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDebugRegistAll);

void dds3ClearStockTable(void)
{
    datUnitWork_t *stock = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    u8 *gbwk;
    s32 i;

    for (i = 0xF; i >= 0; i--, stock++) {
        stock->flag = 0;
    }
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

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitStockTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmpinit);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_Shutdown);

INCLUDE_ASM(const s32, "code_001FAE80", cmpProcessStart);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStockRemoveEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetIniStockRecoverEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStockRecoverEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDestroyItem);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetItSkMessageVar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetItSkErrSelFlag);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpCreateLocalItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartSkillTokusyuMsg);

void cmpStartItemTokusyuMsg(u16 item)
{
    cmpStartSkillTokusyuMsg(datItemTable[item].skill);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartSkillMsg);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetKugatatiMsg);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartItemMsg);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUseItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSelectSkillID);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUseSkill);

INCLUDE_ASM(const s32, "code_001FAE80", cmpChkDoNotUseSkill);

INCLUDE_ASM(const s32, "code_001FAE80", cmpItemIsStockFull);

INCLUDE_ASM(const s32, "code_001FAE80", cmpItemIsUsable);

INCLUDE_ASM(const s32, "code_001FAE80", cmpApplyStockRecover);

INCLUDE_ASM(const s32, "code_001FAE80", cmpApplyStockRemove);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRemoveStockDevilEntry);

void cmp_22E258(void *work)
{
    memset(work, 0, 0x2128);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitSkillCursor);

void cmpInitStockCursorPos(void)
{
    s32 rows;

    rows = campWork->stock[0].unk_02 + campWork->stock[0].unk_03[0];
    cmpInitCursorRange(campWork->unk_6230, (u8)rows, (u8)rows, 0);
    rows = campWork->stock[1].unk_02 + campWork->stock[1].unk_03[0];
    cmpInitCursorRange((u8 *)campWork + 0x726C, (u8)rows, (u8)rows, 0);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpReCalcStockCursorPos);

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
    return stock.unk_02;
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
        return (u8)stock.unk_02 + stock.unk_03[0];
    }
    return (s8)stock.unk_03[0];
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcRoot);

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

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F4F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F500);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcSkill);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcDestroyItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetPartyDSL);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcStockControl);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcParty);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcConfig);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcLoad);

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
INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateList);
#endif

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcSequence);

INCLUDE_ASM(const s32, "code_001FAE80", nmeMakeGradColors);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawQuadPanel);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F590);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F5A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F5C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F5D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F5F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F610);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F628);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F640);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F650);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F678);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F690);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F6A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F6B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F6D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053F6E8);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawGradQuad);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawBlendQuad);

s32 nmeDrawBox(s32 color, s32 unused, s32 flag)
{
    return cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, color, flag);
}

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawSizedSprite);

INCLUDE_ASM(const s32, "code_001FAE80", nmeLerp);

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

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawFrame);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawBlendText);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawHelp);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDrawText);

INCLUDE_ASM(const s32, "code_001FAE80", func_002306A0);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockRow);

void campDrawSetPhase(void)
{
    campDrawText(1, 0);
    campWork->drawPhase = 2;
}

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawNumber);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawFade);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB68);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_Process);

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


INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateRoot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateParty);

INCLUDE_ASM(const s32, "code_001FAE80", campUpdateStockSelect);

INCLUDE_ASM(const s32, "code_001FAE80", campUpdateStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateSkillSelect);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateProcessStatus);

void cmpRemoveStockDevil(void)
{
    cmpRemoveStockDevilEntry();
    campWork->stock[0].count = 0x8B;
    cmpBuildStockNameList(&campWork->stock[0]);
    cmpInitDrawList();
    cmpReCalcStockCursorPos(2);
}

INCLUDE_ASM(const s32, "code_001FAE80", campWaitSelMessage);

void campCloseInfoWindow(void)
{
    if (campInfoWorkExists() == 0) {
        campWork->unk_0001 = 0;
        campWork->unk_0004 = 2;
        cmpDisableInfoWindow();
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateDestroyItemConfirm);

extern s32 checkConfigState(void);
extern s32 cmpUpdateListSelection(void);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateLoad);

void campUpdateEffectStep(void)
{
    campWork_t *work = campWork;

    work->effectStep++;
    if (work->effectStep >= 0x900) {
        work->effectStep = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00232198);

s32 campUpdateEffects(void)
{
    campUpdateEffectStep();
    return func_00232198();
}

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_Process);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocStockProcess);

void camp_update_stock_Shutdown(void)
{
    if (campStatusPanel != NULL) {
        sdfMemFreeDirect(campStatusPanel->allocation);
        campStatusPanel = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartProcessStock);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpClearStatusFlags);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockPanel);

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

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_stock_Process);

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


INCLUDE_ASM(const s32, "code_001FAE80", cmpInitHeartsIconTable);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDhCheckTextures);

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


INCLUDE_ASM(const s32, "code_001FAE80", campDhFreeTexture);

extern void campDhFreeTexture(campInfoWork_t *work);
extern void sdfMemFreePtrDirect(void *ptr);
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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetHeartsSequenceRemove);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitHeartsIcon);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpinitDH);

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

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FD78);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartProcessDH);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStatusStartEff);

INCLUDE_ASM(const s32, "code_001FAE80", camp_calc_dh_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawBox);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawBoxSprite);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawGaugeBars);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclGetArrowCursorTarget);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawArrowCursorIcon);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawArrowCursorBox);

INCLUDE_ASM(const s32, "code_001FAE80", fclGetArrowCursorEntry);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawBackground);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawArrowCursorLabel);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawArrowCursor);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawPanel);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawQuadGauge);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHelpTextAlt);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHelpText);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHeartsGauge);

extern campStatusPanel_t *campDhDrawWork;
extern void campDhDrawHeartsGauge(void *gauge, s8 arg);
void campDhDrawHeartsIcon(s32 a, s32 b, s32 c, s8 d)
{
    campStatusPanel_t *work = campDhDrawWork;

    if ((work->flags & 1) == 0) {
        if (*((u8 *)DDS3_GBWK + 0x1CFA) == 0) {
            return;
        }
    }
    campDhDrawHeartsGauge((u8 *)work + 0x1F78, d);
}

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawIcon);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHearts);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDD8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDE8);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_dh_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDhApplyEntry);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFindEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmpReturnParamDH);

extern void datEquipHearts(s32 id);
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

INCLUDE_ASM(const s32, "code_001FAE80", campDhUpdateCursor);

s32 cmpUnloadStatusModel(void)
{
    cmpModelFreeResource();
    while (sdfDelayedResrcCheck() != 0) {
    }
    cmpResetDrawEnv();
    campDhUpdateWork->unk_01[0] = 0;
    campDhUpdateWork->unk_04 = 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsAttachStatus);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStepHeartsAttachCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsAttachConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsRemoveConfirm);

s32 campDhWaitMessageEnd(void)
{
    s32 busy = fclChkMessage();

    if (busy != 0) {
        return busy;
    }
    cmpReturnParamDH();
    return cmpUnloadStatusModel();
}

INCLUDE_ASM(const s32, "code_001FAE80", campDhAdvanceAnim);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDhUpdateIcons);

INCLUDE_ASM(const s32, "code_001FAE80", campDhRollRatio);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x14];
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ u8 unk_1C[4];
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ u8 unk_24[4];
    /* 0x28 */ s32 unk_28;
} campDhEntry_t;

extern s32 campDhRollRatio(void);
INCLUDE_ASM(const s32, "code_001FAE80", campDhGetEntryRatio);


INCLUDE_ASM(const s32, "code_001FAE80", campDhScanEntries);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFindVisibleIcon);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_dh_Process);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetGaugeBlinkAlpha);

s32 cmpGetPanelAlphaOrDefault(u8 *alpha, s32 color)
{
    s32 value = 0x80;

    if (alpha != NULL) {
        value = *alpha;
    }
    return (color & ~0xFF) | value;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawNumber);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusIconRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusIcons);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusName);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusGauge);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawParamGauge);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawParamPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusValue);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusBars);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillRowText);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillIcons);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkill);

extern void cmpDrawStatusComEx(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 color, s8 g,
                               s32 h, s32 i);
void cmpDrawStatusCom(s32 a, s8 b, s8 c, s8 d)
{
    cmpDrawStatusComEx(a, 0x80, -1, 0, 0, 0, 0xA09DC380, b, c, d);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusComEx);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusComEx2);

extern char *pStr[];
extern void *frwGetSmallFontFRQ2(s32 x, s32 y, s32 color, s32 d, char *text, s32 f);
void cmpDrawItemListHeader(s32 x, s32 y, s32 c, u16 index)
{
    void *frq = frwGetSmallFontFRQ2(x, y, 0xFFFFFF, c, pStr[index], 0);

    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawItemListScrollBar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawItemListRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcItemEntry);

void campDrawInfoPanel(void)
{
    campDrawText(4, 0);
    campWork->drawPhase = 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", campDrawSeq3);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillListHeader);

u8 cmpGetSkillIcon(u16 skill)
{
    return cmbSkillTable[skill].icon;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSkillCost);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillListRow);

void cmpDrawSkillListScrollBar(s32 a, cmpCursorWork_t *cursor)
{
    if (cursor->unk_20 != 0) {
        cmpDrawScrollBar(0, 0x280, 0x10, 0xA4, 0x16, 0x80808080, a, cursor->unk_24, cursor,
                         0, 0x47);
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillList);

void cmpExpandColor(u64 *dst, u32 color)
{
    u64 wide = color;

    dst[0] = ((color >> 24) & 0xFF) | ((u64)((color >> 16) & 0xFF) << 32);
    dst[1] = ((wide & 0xFF00) >> 8) | ((u64)(color & 0xFF) << 32);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSetup3DView);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSpriteFlag);

s32 func_00239C08(u8 id, s32 arg)
{
    return cmpDrawSpriteFlag(id, 0, arg);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSprite);

s32 fclDrawPanelSprite(s32 id, s32 arg)
{
    return cmpDrawSprite(id, 0, arg);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSprite2);

s32 fclDrawPanelSprite2(s32 id, s32 arg)
{
    return cmpDrawSprite2(id, 0, arg);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSpriteIndexed);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSpriteIndexed2);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawTriG);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawBoxG);

s32 cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *f, s32 g)
{
    return cmpDrawBoxG(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23A360);

void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    s32 pair[2];

    pair[1] = g;
    pair[0] = g;
    cmpDrawBoxLine(a, b, c, d, e, f, pair);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawBoxLine);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDraw3DPoly);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDisableRGB);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocRefGifTag);

void cmp_23A8B0(s32 arg)
{
    cmpDisableRGB();
    fclDrawPanelSprite(0x1001, arg);
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0, arg);
    fclDrawPanelSprite(0x50000, arg);
    func_00239C08(1, arg);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23A918);

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

extern void mdlSetPosition(void *model);

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
extern void mdlLink(void *model, cmpModelLink_t *link);
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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawDebugPrint);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawFrameRect);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitStatus);

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

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FEB8);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartProcessStatus);

void cmpSetStatusWindowHidden(void)
{
    if (cmpStatusPhase != NULL) {
        cmpStatusPhase->flags |= 0x10;
        cmpStatusWindowHidden = 1;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", camp_calc_status_Process);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_status_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campFreeStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateStatusModelLoad);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_status_Process);

void *cmpAllocInfoProcess(void *owner, s32 value)
{
    void *chunk = sdfMemAllocDirect(0x1C);
    cmpSeqInfo_t *info = sdfMemLock(chunk);

    info->allocation = chunk;
    cmpInitSeqInfo(info, value);
    info->owner = owner;
    return info;
}

void camp_update_info_Shutdown(void)
{
    if (cmpInfoWindow != NULL) {
        sdfMemFreeDirect(cmpInfoWindow->allocation);
        cmpInfoWindow = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartProcessInfo);

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

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_info_Process);

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

INCLUDE_ASM(const s32, "code_001FAE80", campInitPanelPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetAfterCrossFunc);

INCLUDE_ASM(const s32, "code_001FAE80", campRunPanelCallback);

void fclInitCursorWindow(void *work, s32 flag, u8 count)
{
    cmpCursorWork_t *cursor = (cmpCursorWork_t *)((u8 *)work + 4);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListItemBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListItemText);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListFrame);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListRowBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListRows);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListColumn);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawScrollBar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawScrollThumb);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateListSelection);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23C918);

INCLUDE_ASM(const s32, "code_001FAE80", camp_23C9C0);

INCLUDE_ASM(const s32, "code_001FAE80", camp_23CAF8);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawInfoText);

INCLUDE_ASM(const s32, "code_001FAE80", camp_23CC70);

INCLUDE_ASM(const s32, "code_001FAE80", camp_23CDB8);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStatusRow1);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusBadge);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStatusRow2);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStatusRow3);

s32 campGetCursorColorSel(s32 color, cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);

    return fclBlendColor(color, -1, alpha);
}

INCLUDE_ASM(const s32, "code_001FAE80", campGetCursorColorAlt);

s32 campGetCursorColorAlt2(s32 color, cmpCursorSlot_t *slot)
{
    s32 alpha = cmpGetCursorSlotValue(0, slot);

    return fclBlendColor(color & ~0xFF, color, alpha);
}

s32 campGetStockAlpha(s8 index, s8 row)
{
    return campWork->stockAlpha[index + row * 4];
}

INCLUDE_ASM(const s32, "code_001FAE80", campCalcStockScale);

s32 campDrawStockCell(s32 color, s8 index, s8 row)
{
    s32 alpha = (color & 0xFF) * campGetStockAlpha(index, row) / 256;

    return (color & ~0xFF) | (alpha & 0xFF);
}

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockStatus);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockLabel);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockLabel2);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStockHasRoom);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockRow1);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockRow2);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindFreeStockSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpScaleColorByPhase);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockHighlight);

INCLUDE_ASM(const s32, "code_001FAE80", campCheckStockFree);

INCLUDE_ASM(const s32, "code_001FAE80", campCheckStockRoom);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockSlots);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRandomizePanelEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffBar);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_0023ED18);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpAddPanelEffEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAddQuePanelEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffRing);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffQuad);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FF60);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindPanelEffSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdatePanelEffSlots);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocPanelEffWork);

void camp_update_paneleff_Shutdown(void)
{
    if (cmpPanelEffList != NULL) {
        sdfMemFreeDirect(cmpPanelEffList->allocation);
        cmpPanelEffList = NULL;
    }
}

extern cmpPanelEff_t *cmpAllocPanelEffWork(void);
extern char D_0053FFD0[];

void *cmpStartProcessPanelEff(void)
{
    cmpPanelEff_t *work = cmpAllocPanelEffWork();

    cmpPanelEffList = work;
    return dds3StartProcess(D_0053FFD0, 0x520B, 1, 1, camp_update_paneleff_Process,
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

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23F8F0);

INCLUDE_ASM(const s32, "code_001FAE80", func_0023F9F8);

INCLUDE_ASM(const s32, "code_001FAE80", func_0023FA98);

INCLUDE_ASM(const s32, "code_001FAE80", func_0023FB10);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpModelInitCore2);

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

extern void mdlFree(s32 handle);
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

void cmpPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    D_0054FEE0 = mdlPrepare(group, id, blocking);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCreateBadEffect);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FFD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FFE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FFF8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540000);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540008);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540018);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540020);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540028);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540030);

INCLUDE_ASM(const s32, "code_001FAE80", cmpBadEffSetup);

extern void *D_0054FEE8;
extern void effCAMPBadDelete(void *eff);
extern char D_00540058[];

void cmpFreeBadEffect(void)
{
    if (D_0054FEE8 != NULL)
    {
        effCAMPBadDelete(D_0054FEE8);
        D_0054FEE8 = NULL;
        printf(D_00540058);
    }
}

extern char D_00540070[];

void rstPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    cmpPrepareStatusModel(group, id, blocking);
    printf(D_00540070, group, id);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpModelSetup);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540054);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540058);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540070);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0054007C);

INCLUDE_ASM(const s32, "code_001FAE80", cmpLoadStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStatusModelMotion);

extern datUnitVisual_t datModelParamTable[];
extern void mdlSetScale(void *model);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStatusModelPose);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetModelRotate);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStatusModelParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRebuildStockList);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetMaxHPMP);

s8 cmpGetParamGaugeColor(s8 index)
{
    return D_00349380[index];
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_005400B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005400C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005400D0);

INCLUDE_ASM(const s32, "code_001FAE80", cmpExecRecover);

s32 cmp_240C10(void)
{
    return 1;
}

u16 cmpChkSkillBad(u16 skill)
{
    datSkillBad_t *bad = &cmbSkillTable[skill].bad;

    if (bad->kind != 2) {
        return 0;
    }
    return bad->id;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSkillEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAddCost);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSkillTargetArea);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUseItemKou);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUseItemTokusyu);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUseSkillTokusyu);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRecover);

char *cmpGetParamName(s8 index)
{
    return D_00349388[index];
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetPressedButton);

INCLUDE_ASM(const s32, "code_001FAE80", cmpPlayCursorSE);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocEffectSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpClearCursorSlots);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetCursorSlot);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateCursorSlots);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitCursorLayout);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitCursorRange);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateCursorScroll);

void cmpSetCursorFlag(cmpCursorWork_t *cursor, s32 flag)
{
    cursor->unk_2C = flag;
    cmpUpdateCursorScroll(cursor, 1);
}

/* NON_MATCHING: 9 of 28 words differ (first at +0x8). */
#ifdef NON_MATCHING
void cmp_241A10(cmpCursorWork_t *cursor)
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
INCLUDE_ASM(const s32, "code_001FAE80", cmp_241A10);
#endif

INCLUDE_ASM(const s32, "code_001FAE80", cmp_241A88);

void cmpResetCursorPos(s16 value, cmpCursorWork_t *cursor)
{
    cursor->unk_04 = 0;
    cursor->unk_24 = 0;
    cursor->unk_25 = 0;
    cursor->unk_22 = value;
}

void cmpSetCursorRow(s8 row, cmpCursorWork_t *cursor)
{
    cursor->unk_24 = row;
}

extern void cmpClearCursorSlots(cmpCursorWork_t *cursor);
void cmpResetCursorInfo(cmpCursorWork_t *cursor)
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

s32 cmpCursorAtFirstEnabled(cmpCursorWork_t *cursor)
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

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorAtLastEnabled);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindNextCursorItem);

void cmpAdvanceCursorPhase(cmpCursorWork_t *cursor)
{
    cursor->unk_34 += 6;
    if (cursor->unk_34 > 0x200) {
        cursor->unk_34 -= 0x200;
    }
}

s32 cmpMoveCursor(cmpCursorWork_t *cursor, s8 dir)
{
    if (dir != 0)
    {
        cursor->unk_34 = 0;
    }
    else
    {
        cmpAdvanceCursorPhase(cursor);
    }
    cmpFindNextCursorItem(cursor, dir, 1);
    cursor = NULL;
}

s32 cmpGetCursorMoveMode(cmpCursorWork_t *cursor)
{
    if (cursor->unk_20 < 2) {
        return 1;
    }
    if (cursor->unk_04 != 0) {
        return 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpMoveCursor2);

INCLUDE_ASM(const s32, "code_001FAE80", cmpChangeCursorListNums);

void cmpMoveCursorListEnd(cmpCursorWork_t *cursor)
{
    s32 end = cursor->unk_20 + 0xFF;

    cursor->unk_25 = end - cursor->unk_21;
    cursor->unk_24 = end;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDisablePadBitStop);

s8 cmpGetCursorRow(cmpCursorWork_t *cursor)
{
    return cursor->unk_04;
}

extern void cmpMoveCursor2(cmpCursorWork_t *cursor, s8 dir);

void cmpMoveStockCursor(s8 index, s8 dir, u8 *base)
{
    cmpMoveCursor2((cmpCursorWork_t *)(base + index * 0x103C + 0xAC), dir);
}

void func_00242320(s8 value, u8 *work)
{
    work[0x2124] = value;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorSlotIsSelectable);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStockInfoIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmpBuildStockNameList);

s8 cmpGetStockCount(void)
{
    return cmbGlobalWork->stockCount;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpSortStockList);

s8 func_002426F0(void)
{
    if (cmbGlobalWork->stockCount < 0x10) {
        return cmbGlobalWork->stockCount;
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00242710);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDebugDumpCount);

void cmpInitSeqInfo(cmpSeqInfo_t *info, s32 value)
{
    info->unk_00 = 3;
    info->unk_02 = -1;
    info->unk_05 = 0;
    info->unk_0C = 0;
    info->unk_10 = value;
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


INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateSeqInfo);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_002429C0);


INCLUDE_ASM(const s32, "code_001FAE80", cmpItemHasFlag);

void func_00242A70(s32 key) {
}

s32 campStatusIsActive(void)
{
    return campStatusActive != 0;
}

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540128);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540138);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540148);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540158);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540168);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540178);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540188);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540198);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401CC);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005401F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540208);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540218);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540228);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540238);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540248);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540258);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540268);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540278);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540288);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540298);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005402F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540308);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540318);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540328);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540338);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540348);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540358);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540368);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540378);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540388);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540398);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005403F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540408);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540418);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540428);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540438);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540448);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540458);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540468);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540478);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540488);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540498);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005404F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540508);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540518);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540528);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540538);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540548);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540558);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540568);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540578);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540588);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540598);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005405A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005405B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005405C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005405D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005405F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540610);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540628);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540640);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540650);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540668);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540690);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005406A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005406D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005406F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540708);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540718);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540738);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540758);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540770);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540798);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005407A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005407C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005407D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005407E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005407F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540828);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540850);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540888);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005408B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005408E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540908);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540930);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540958);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540978);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540998);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005409B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005409E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540A10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540A38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540A50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540A60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540A88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540AA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540AC8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540AE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540B08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540B20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540B38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540B60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540B78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540BA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540BC8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540BE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540C98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540CB8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540CD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540CF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540D18);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540D30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540D58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540D70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540D98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540DC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540DE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540E10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540E28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540E58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540E78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540E98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540EC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540EE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540F08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540F30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540F50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540F70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540F88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540FA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540FC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540FD8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540FE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541008);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541038);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541060);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541088);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005410A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005410B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005410D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005410E8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541110);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541130);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541148);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541170);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541188);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005411A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005411C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005411D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005411F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541218);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541230);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541248);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541268);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541280);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005412A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005412C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005412D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005412F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541318);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541330);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541358);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541380);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005413A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005413C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005413E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541408);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541430);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541448);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541460);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541478);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541498);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005414B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005414D8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005414F8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541518);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541540);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541558);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541568);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541580);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005415A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005415B8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005415D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005415F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541608);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541620);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541638);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541648);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541678);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541698);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005416B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005416D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005416F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541710);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541738);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541760);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541780);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005417A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005417C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005417E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541808);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541828);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541850);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541878);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005418A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005418C8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005418E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541908);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541928);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541950);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541970);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541990);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005419A8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005419D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005419F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541A08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541A30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541A58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541A78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541A98);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541AB8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541AE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541B08);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541B30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541B58);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541B78);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541B90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541BB8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541BE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541BF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541C90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541CF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541D90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541DF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541E90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541EA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541EB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541EC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541ED0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541EE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541EF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541F90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00541FF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542000);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542010);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542020);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542030);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542040);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542050);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542060);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542070);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542080);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542090);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005420F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542100);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542110);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542120);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542130);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542140);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542150);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542160);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542170);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542180);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542190);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005421F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542200);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542210);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542220);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542230);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542240);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542250);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542260);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542270);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542280);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542290);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005422F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542300);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542310);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542320);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542330);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542340);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542350);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542360);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542370);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542380);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542390);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005423F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542400);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542410);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542420);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542430);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542440);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542450);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542460);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542470);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542480);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542490);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005424F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542500);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542510);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542520);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542530);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542540);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542550);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542560);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542570);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542580);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542590);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005425F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542600);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542610);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542620);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542630);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542640);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542650);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542670);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542680);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542690);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005426F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542700);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542710);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542720);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542730);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542740);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542750);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542760);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542770);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542780);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542790);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005427F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542800);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542810);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542820);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542830);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542840);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542850);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542860);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542870);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542880);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542890);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005428F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542900);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542910);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542920);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542930);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542940);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542950);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542960);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542970);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542980);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542990);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429A0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429B0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429C0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429D0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429E0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005429F0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542A90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542AF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542B90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542BF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542C90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542CF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542D90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542DF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542E90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542EA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542EB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542EC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542ED0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542EE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542EF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F00);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F10);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F20);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F30);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F40);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F60);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F80);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542F90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FA0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FB0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FD0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FE0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00542FF0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543000);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543018);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543028);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543040);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543050);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543060);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543070);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543080);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00543098);

INCLUDE_RODATA(const s32, "code_001FAE80", D_005430A8);

INCLUDE_SDATA(const s32, "code_001FAE80", gProcessStat_0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAD5);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FAF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB00);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpSkillCursor);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB0C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB18);

INCLUDE_SDATA(const s32, "code_001FAE80", cmbActionMode);

INCLUDE_SDATA(const s32, "code_001FAE80", cmbActionStep);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB1D);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB20);

INCLUDE_SDATA(const s32, "code_001FAE80", shpProcessStat);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB25);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB68);

INCLUDE_SDATA(const s32, "code_001FAE80", GBWK_0);

INCLUDE_SDATA(const s32, "code_001FAE80", gProcessStat_1);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB75);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FB98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBB0);

INCLUDE_SDATA(const s32, "code_001FAE80", gProcessStat_2);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBB5);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FBF8);

INCLUDE_SDATA(const s32, "code_001FAE80", GBWK_3);

INCLUDE_SDATA(const s32, "code_001FAE80", gProcessStat_3);

INCLUDE_SDATA(const s32, "code_001FAE80", trmMode);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC48);

INCLUDE_SDATA(const s32, "code_001FAE80", gMsgID);

INCLUDE_SDATA(const s32, "code_001FAE80", gMsgFlag);

INCLUDE_SDATA(const s32, "code_001FAE80", gMsgInvFlag);

INCLUDE_SDATA(const s32, "code_001FAE80", gSelMsgNo);

INCLUDE_SDATA(const s32, "code_001FAE80", gSelMsgPos);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC5D);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FC98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCA0);

INCLUDE_SDATA(const s32, "code_001FAE80", fclDbgPID);

INCLUDE_SDATA(const s32, "code_001FAE80", g_pinst);

INCLUDE_SDATA(const s32, "code_001FAE80", g_pdebug);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCE0);

INCLUDE_SDATA(const s32, "code_001FAE80", GBWK_1);

INCLUDE_SDATA(const s32, "code_001FAE80", gProcessStat_4);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCED);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FCF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD24);

INCLUDE_SDATA(const s32, "code_001FAE80", titleWork);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD2C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD30);

INCLUDE_SDATA(const s32, "code_001FAE80", dds3TitleFadeLevel);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD40);

INCLUDE_SDATA(const s32, "code_001FAE80", gGlobal);

INCLUDE_SDATA(const s32, "code_001FAE80", fldWmapEnabled);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD4C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD88);

INCLUDE_SDATA(const s32, "code_001FAE80", oStaff_UI);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD90);

INCLUDE_SDATA(const s32, "code_001FAE80", PlayingNo);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FD98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDA8);

INCLUDE_SDATA(const s32, "code_001FAE80", movieViewerPhase);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDB4);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDC8);

INCLUDE_SDATA(const s32, "code_001FAE80", campWork);

INCLUDE_SDATA(const s32, "code_001FAE80", campExitState);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDD5);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FDF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE70);

INCLUDE_SDATA(const s32, "code_001FAE80", campStatusPanel);

INCLUDE_SDATA(const s32, "code_001FAE80", campInfoWork);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE7C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE80);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatTimer);

INCLUDE_SDATA(const s32, "code_001FAE80", rstHeartsMasterPhase);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE8E);

INCLUDE_SDATA(const s32, "code_001FAE80", cmbBirthPanel);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE94);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FE98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEBC);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatusPhase);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatusWindowHidden);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatusModelLoadState);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpInfoWindow);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FECC);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FED0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FED8);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpPanelEffList);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEE0);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpEncycWork);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEE8);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatusModelReady);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpModelInitFlag);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEEE);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpStatusModelId);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEF2);

INCLUDE_SDATA(const s32, "code_001FAE80", cmpModelHandle);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FEF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF08);

INCLUDE_SDATA(const s32, "code_001FAE80", campStatusActive);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF14);

INCLUDE_SDATA(const s32, "code_001FAE80", cmbRaceTable);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF1C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FF98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0054FFF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550000);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550004);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550008);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550010);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550018);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0055001C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550020);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550028);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550030);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550038);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0055003C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550040);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550048);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550050);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550054);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550058);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550060);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550064);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550068);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550070);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550078);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550080);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550088);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550090);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550098);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005500F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550100);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550108);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550110);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550118);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550120);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550128);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550130);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550134);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550138);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550140);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550148);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550150);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550158);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550160);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550168);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550170);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550178);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550180);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550188);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550190);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550198);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005501F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550200);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550208);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550210);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550218);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550220);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550228);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550230);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550238);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550240);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550248);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550250);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550258);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550260);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550268);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550270);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550278);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550280);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550288);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550290);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550298);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005502F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550300);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550308);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550310);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550318);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550320);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550328);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550330);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550338);

INCLUDE_SDATA(const s32, "code_001FAE80", D_0055033C);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550340);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550348);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550350);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550358);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550360);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550368);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550370);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550378);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550380);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550388);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550390);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550398);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005503F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550400);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550408);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550410);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550418);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550420);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550428);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550430);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550438);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550440);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550448);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550450);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550458);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550460);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550468);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550470);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550478);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550480);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550488);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550490);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550498);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005504F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550500);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550508);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550510);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550518);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550520);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550528);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550530);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550538);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550540);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550548);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550550);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550558);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550560);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550568);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550570);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550578);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550580);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550588);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550590);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550598);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005505F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550600);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550608);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550610);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550618);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550620);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550628);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550630);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550638);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550640);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550648);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550650);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550658);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550660);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550668);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550670);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550678);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550680);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550688);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550690);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550698);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005506F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550700);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550708);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550710);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550718);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550720);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550728);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550730);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550738);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550740);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550748);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550750);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550758);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550760);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550768);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550770);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550778);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550780);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550788);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550790);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550798);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005507F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550800);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550808);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550810);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550818);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550820);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550828);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550830);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550838);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550840);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550848);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550850);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550858);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550860);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550868);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550870);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550878);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550880);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550888);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550890);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550898);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005508F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550900);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550908);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550910);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550918);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550920);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550928);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550930);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550938);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550940);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550948);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550950);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550958);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550960);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550968);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550970);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550978);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550980);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550988);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550990);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550998);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509A0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509A8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509B0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509B8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509C0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509C8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509D0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509D8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509E0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509E8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509F0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_005509F8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550A98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550AF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550B98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550BF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550C98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550CF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550D98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DA8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DB0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DB8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DC0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DC8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DD0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DD8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DE0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DE8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DF0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550DF8);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E00);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E08);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E10);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E18);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E20);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E28);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E30);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E38);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E40);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E48);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E50);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E58);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E60);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E68);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E70);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E78);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E80);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E88);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E90);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550E98);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550EA0);

INCLUDE_SDATA(const s32, "code_001FAE80", D_00550EA8);

