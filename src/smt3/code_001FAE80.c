#include "common.h"
#include "newdata.h"

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
    u8 unk_05[55];
} datDevilInfo_t;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
    /* 0x80 */ u8 unk_80[6];
    /* 0x86 */ u16 skill[24];
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
    /* 0x0000 */ u8 unk_00[0x360];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncycWork_t;

typedef struct
{
    u8 unk_00[0x30];
    s32 unk_30;
    s32 unk_34;
} dds3TitleWork_t;

typedef struct
{
    u8 unk_00;
    s8 unk_01;
} campInfoWork_t;

extern fclEncycWork_t *g_pinst;
extern dds3TitleWork_t *titleWork;
extern campInfoWork_t *campInfoWork;
extern s8 D_00551F80[8];
extern s8 cmpParamGaugeState[2];
extern u8 cmpStatusModelReady;

typedef struct
{
    void *allocation;
} fclRbnWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x13];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s32 unk_18;
} cmpInfoWindow_t;

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

typedef struct
{
    u8 flags;
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
    u8 unk_01[3];
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
extern s32 fclEndMessageEx();
extern void sdfChipFreeDirect(void *ptr);
extern void cmbAddLevelUpParamEx(void *work, s32 mode);
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
    /* 0x01 */ u8 unk_01[0x13];
    /* 0x14 */ void *allocation;
} campStatusPanel_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_01[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_18[0x1C];
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
extern s32 cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
extern s32 cmpDrawBoxG(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} fclSprite_t;

extern void qsort(void *base, s32 count, s32 size, s32 (*cmp)(const void *a, const void *b));
extern s32 func_00213148(const void *a, const void *b);
extern s32 func_00213190(const void *a, const void *b);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void fclFilterOFF(void);
extern void sdfFileCmdReadFile(const char *name, s32 offset, s32 size, void *done, void *buf);
extern void fclSetSprite(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void fclDrawSpriteEx(s32 a, s32 b, s32 w, s32 h, fclSprite_t *sprite, s32 flags);
typedef struct
{
    /* 0x00 */ u8 race;
    /* 0x01 */ u8 unk_01[0x18];
    /* 0x19 */ s8 unk_19;
    /* 0x1A */ u8 unk_1A[0x28];
} datRaceEntry_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x54];
    /* 0x54 */ s32 unk_54;
} motionWork_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x18];
    /* 0x0018 */ u8 unk_18;
    /* 0x0019 */ u8 unk_19;
    /* 0x001A */ u8 unk_1A[0x152];
    /* 0x016C */ s32 unk_16C;
    /* 0x0170 */ u8 unk_170[0x21A0];
    /* 0x2310 */ u32 unk_2310;
} rstWork_t;

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ u8 unk_03[0x5D];
} cmpLocalStock_t;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x6184];
    /* 0x6184 */ cmpLocalStock_t stock;
} campWork_t;

typedef struct
{
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 unk_04[4];
} datItemEntry_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[3];
    /* 0x03 */ u8 icon;
    /* 0x04 */ u8 unk_04[0x38];
} datSkillEntry_t;

extern campWork_t *campWork;
extern datItemEntry_t datItemTable[];
extern datSkillEntry_t datSkillTable[];
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
extern void func_00224230(s32 param);
typedef struct
{
    /* 0x0000 */ u8 unk_00[0x22E5];
    /* 0x22E5 */ s8 unk_22E5;
    /* 0x22E6 */ s8 unk_22E6;
    /* 0x22E7 */ u8 unk_22E7;
    /* 0x22E8 */ f32 unk_22E8;
    /* 0x22EC */ f32 unk_22EC;
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
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ s8 unk_03;
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
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0x1B];
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26[6];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorWork_t;

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
} cmpSeqInfo_t;

extern void cmpUpdateCursorScroll(cmpCursorWork_t *cursor, s32 mode);

extern u8 movieViewerDebugFlag[];
extern void cmpSetInfoWindowHidden(void);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineTexFree);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineSetTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkGeneralStat);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineCheckMoonPhase);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineBGM);

INCLUDE_ASM(const s32, "code_001FAE80", fclCombineInitRagTex);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbBirthDevilIsAvailable);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbSeqStartScript);

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
        if (unit->skill[i] == 0) {
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

INCLUDE_ASM(const s32, "code_001FAE80", cmbDevilCanBirth);

INCLUDE_ASM(const s32, "code_001FAE80", cmbMakeBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindStockSlot);

INCLUDE_ASM(const s32, "code_001FAE80", ragUseItemOnUnit);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInheritSkills);

void rstClearSkillSlot(cmbUnit_t *unit, u8 slot)
{
    unit->skill[slot] = 0;
    sortSkillList(unit);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbReplaceSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstAddSkillToSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkSkillOwner);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindSkillSlot);

s8 cmbFindSkillSlot(cmbUnit_t *unit)
{
    s32 count = 0;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (unit->skill[i] != 0) {
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

INCLUDE_ASM(const s32, "code_001FAE80", cmbClearUnitSkills);

void cmbClearSkillSlots(cmbUnit_t *unit)
{
    memset(unit->skill, 0, sizeof(unit->skill));
}

INCLUDE_ASM(const s32, "code_001FAE80", cmb_1FF0C8);

INCLUDE_ASM(const s32, "code_001FAE80", cmbApplyEvoUnlock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindStockEnd);

INCLUDE_ASM(const s32, "code_001FAE80", cmbStoreBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddCustomDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmbApplyEvoResult);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawTopSprite);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawPhasePanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawSequence);

INCLUDE_ASM(const s32, "code_001FAE80", cmbExit);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateRoot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbStockIsLocked);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChangeSequence);

INCLUDE_ASM(const s32, "code_001FAE80", cmbWaitCancelButton);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSwitchRibbon);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkIkenieEnable);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateBirthDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetAction);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateDecideDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateSacConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateStockCursor2);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSeqAfterBirth);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateMenuCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSeqReturnToPrev);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeIntroduceModels);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeSmokeEffect);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateIntroduce);

INCLUDE_ASM(const s32, "code_001FAE80", cmbStartEncycView);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateResultCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmbHandleDebugExit);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdatePanelAnim);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateModelLoad);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbActionExists);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD38);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD48);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053CD58);

INCLUDE_ASM(const s32, "code_001FAE80", cmbMakeModelPath);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitActionData);

INCLUDE_ASM(const s32, "code_001FAE80", cmbLoadActionModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeActionModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetActionModelHandle);

INCLUDE_ASM(const s32, "code_001FAE80", cmbUpdateIntroduceModel);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindCameraPoint);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawActionModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetModelMarkPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawFullScreenPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawModelPanel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbDrawTargetModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetSceneModel);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_00205DB8);

INCLUDE_ASM(const s32, "code_001FAE80", cmb_205DF0);

INCLUDE_ASM(const s32, "code_001FAE80", cmb_205E68);

INCLUDE_ASM(const s32, "code_001FAE80", func_00205EA0);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFreeActionModels);

s32 cmbChkActScript(void)
{
    return dds3SearchProcessName(D_0053CF10) != NULL;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkPolygonMovie);

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

INCLUDE_ASM(const s32, "code_001FAE80", shopStartEvent);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclShopTexFree);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopInitRagTex);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopGetTexPath);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocShopWork);

INCLUDE_ASM(const s32, "code_001FAE80", shop_update_Shutdown);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", fclShopProcessEnd);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_00207198);

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

INCLUDE_ASM(const s32, "code_001FAE80", shpCountUnlockedItems);

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

INCLUDE_ASM(const s32, "code_001FAE80", shpAddServiceTicket);

INCLUDE_ASM(const s32, "code_001FAE80", shpGetItemFromItemBox);

INCLUDE_ASM(const s32, "code_001FAE80", shpExit);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqBuy);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqSell);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqBuyNums);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqSellNums);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqPay);

INCLUDE_ASM(const s32, "code_001FAE80", shpServiceTicketItem);

INCLUDE_ASM(const s32, "code_001FAE80", shpServiceTicket);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateServiceTicket);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateServiceTicketItem);

INCLUDE_ASM(const s32, "code_001FAE80", shpUpdateSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", shopCheckSeqInfo);

INCLUDE_ASM(const s32, "code_001FAE80", shopStartScript);

INCLUDE_ASM(const s32, "code_001FAE80", shopUpdatePanelAnim);

INCLUDE_ASM(const s32, "code_001FAE80", shop_update_Process);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagEventStart);

void fclRagEventEnd(void)
{
    dds3KillProcessName(D_0053D140, 1);
    fclFreeStage();
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00209D88);

INCLUDE_ASM(const s32, "code_001FAE80", func_00209DD0);

INCLUDE_ASM(const s32, "code_001FAE80", func_00209E18);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclRagTexLoad);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagUpdateInit);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocRagWork);

INCLUDE_ASM(const s32, "code_001FAE80", fclDestroyRag);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagProcessEnd);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_0020ADE0);

INCLUDE_ASM(const s32, "code_001FAE80", ragGetConfirmEntry);

INCLUDE_ASM(const s32, "code_001FAE80", ragFindItemSlot);

INCLUDE_ASM(const s32, "code_001FAE80", ragSetSeqPhase2);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateScene);

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

INCLUDE_ASM(const s32, "code_001FAE80", ragExit);

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

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqMitama);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqRoot);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqTalk);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdateSeqErr);

INCLUDE_ASM(const s32, "code_001FAE80", ragUpdate);

INCLUDE_ASM(const s32, "code_001FAE80", fclStartRecoverEvent);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverFreeTextures);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverInitItemList);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverInitTargetList);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverInitRagTex);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverUpdateInit);

INCLUDE_ASM(const s32, "code_001FAE80", fclDestroyRecover);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", fclRecoverProcessEnd);

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

INCLUDE_ASM(const s32, "code_001FAE80", rcvGetPriceForUnit);

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

INCLUDE_ASM(const s32, "code_001FAE80", rcvReviveUnit);

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

INCLUDE_ASM(const s32, "code_001FAE80", rcvExit);

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

INCLUDE_ASM(const s32, "code_001FAE80", trmChkEventWait);

INCLUDE_ASM(const s32, "code_001FAE80", trmSetProcessTerm);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalScanCommands);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalInitList);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalInitMenu);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalDumpJump);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalResetCommands);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalLoadTextures);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalFreeTextures);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalSetMode);

INCLUDE_ASM(const s32, "code_001FAE80", fclAllocTerminalWork);

INCLUDE_ASM(const s32, "code_001FAE80", fclDestroyTerminal);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", fclTerminalProcessEnd);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_002109E0);

INCLUDE_ASM(const s32, "code_001FAE80", trm_210A00);

INCLUDE_ASM(const s32, "code_001FAE80", trm_210AF8);

s32 func_00210B38(void)
{
    u32 i;

    for (i = 0; i < 4; i++) {
        gCommandFlag[i] = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00210B68);

INCLUDE_ASM(const s32, "code_001FAE80", func_00210B90);

INCLUDE_ASM(const s32, "code_001FAE80", trmResetCursor);

INCLUDE_ASM(const s32, "code_001FAE80", trmCalc);

INCLUDE_ASM(const s32, "code_001FAE80", trmGetListFont);

INCLUDE_ASM(const s32, "code_001FAE80", trmFreeListFont);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawListRow);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawList);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateListSelection);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawSeq0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D640);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D650);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D660);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053D670);

INCLUDE_ASM(const s32, "code_001FAE80", trmDraw);

INCLUDE_ASM(const s32, "code_001FAE80", trmGetSaveResult);

INCLUDE_ASM(const s32, "code_001FAE80", trmStartSaveAct);

INCLUDE_ASM(const s32, "code_001FAE80", trmCheckSaveAct);

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

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSave);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSaveActIn);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdateSaveActOut);

INCLUDE_ASM(const s32, "code_001FAE80", trmUpdate);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2120B0);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawListSprite);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawBG);

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

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2129A0);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2129F8);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawStrCenterTarget);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawCaption);

INCLUDE_ASM(const s32, "code_001FAE80", trmDrawListText);

INCLUDE_ASM(const s32, "code_001FAE80", trm_212CB8);

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPriceText);

INCLUDE_ASM(const s32, "code_001FAE80", shopDrawItemCount);

INCLUDE_ASM(const s32, "code_001FAE80", func_00213148);

INCLUDE_ASM(const s32, "code_001FAE80", func_00213190);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSkillOrder);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSetEvoResult);

INCLUDE_ASM(const s32, "code_001FAE80", buildSkillSortArray);

INCLUDE_ASM(const s32, "code_001FAE80", applySkillSortArray);

void apply_213300(void *base, u8 count)
{
    qsort(base, count, 0xEC, func_00213148);
}

void sort_213328(u8 *base)
{
    qsort(&base[0x3A], 0x18, 2, func_00213190);
}

INCLUDE_ASM(const s32, "code_001FAE80", sortSkillList);

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

INCLUDE_ASM(const s32, "code_001FAE80", lstDrawUpDownCursor);

INCLUDE_ASM(const s32, "code_001FAE80", lstDrawListWindow);

INCLUDE_ASM(const s32, "code_001FAE80", lstDrawListWindowCursor);

INCLUDE_ASM(const s32, "code_001FAE80", lstPageUp);

INCLUDE_ASM(const s32, "code_001FAE80", lstPageDown);

INCLUDE_ASM(const s32, "code_001FAE80", datGetItemSlotIndex);

INCLUDE_ASM(const s32, "code_001FAE80", fclAddItem);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_213998);

INCLUDE_ASM(const s32, "code_001FAE80", fclLoadStage);

INCLUDE_ASM(const s32, "code_001FAE80", fclFilterOFF);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclScriptForcedEnd);

INCLUDE_ASM(const s32, "code_001FAE80", fclScriptStart);

INCLUDE_ASM(const s32, "code_001FAE80", func_00213BB8);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_00213CD8);

INCLUDE_ASM(const s32, "code_001FAE80", shuffleIndices);

INCLUDE_ASM(const s32, "code_001FAE80", func_00213DC0);

INCLUDE_ASM(const s32, "code_001FAE80", func_00213DF0);

INCLUDE_ASM(const s32, "code_001FAE80", fclBlendColor);

INCLUDE_ASM(const s32, "code_001FAE80", fclInitMessage);

INCLUDE_ASM(const s32, "code_001FAE80", fclStartMessage);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclEndMessageEx);

INCLUDE_ASM(const s32, "code_001FAE80", fclEndMessage);

INCLUDE_ASM(const s32, "code_001FAE80", fclDestroyMessage);

INCLUDE_ASM(const s32, "code_001FAE80", fclChkMessage);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclSetMessageInvalid);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_2142F0);

INCLUDE_ASM(const s32, "code_001FAE80", fclChkMessageDraw);

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

INCLUDE_ASM(const s32, "code_001FAE80", fcl_214428);

INCLUDE_ASM(const s32, "code_001FAE80", fcl_214458);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclLoadTexture);

void fclSetSpriteColor(u8 *sprite, u32 color)
{
    sprite[0x1C] = color >> 24;
    sprite[0x1D] = color >> 16;
    sprite[0x1E] = color >> 8;
    sprite[0x1F] = color;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclSetSpriteEx);

INCLUDE_ASM(const s32, "code_001FAE80", fclSetSprite);

void fclSetSpriteNormal(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
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

INCLUDE_ASM(const s32, "code_001FAE80", fclRagTexCalcSizes);

INCLUDE_ASM(const s32, "code_001FAE80", fclRagTexInit);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclDrawPanelTypeDef);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclUpdatePanelType3);

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

INCLUDE_ASM(const s32, "code_001FAE80", GetPriceForSummon);

s32 fclEncycWork(void)
{
    return g_pinst != NULL;
}

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycInitRecord);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycSearch);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycGetNum);

INCLUDE_ASM(const s32, "code_001FAE80", fclEncycGetRatio);

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

INCLUDE_ASM(const s32, "code_001FAE80", InitializeInstance);

INCLUDE_ASM(const s32, "code_001FAE80", allocPhaseBuffer);

void ReleaseWork(fclEncycWork_t *work)
{
    if (work->unk_36C != 0) {
        sdfMemFreeDirect(work->allocation);
        work->unk_36C = 0;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", CALC_ENCYC_Process);

INCLUDE_ASM(const s32, "code_001FAE80", DRAW_ENCYC_Process);

INCLUDE_ASM(const s32, "code_001FAE80", UPDT_ENCYC_Process);

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", DestPhaseRegistTop);

INCLUDE_ASM(const s32, "code_001FAE80", InitPhaseRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", CalcPhaseRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", DrawPhaseRegistConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", DestPhaseRegistConfirm);

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

INCLUDE_ASM(const s32, "code_001FAE80", PushListwinCursor);

INCLUDE_ASM(const s32, "code_001FAE80", PopListwinCursor);

INCLUDE_ASM(const s32, "code_001FAE80", AdjustStackCursor);

INCLUDE_ASM(const s32, "code_001FAE80", ResetReadMainRibbon);

INCLUDE_ASM(const s32, "code_001FAE80", AcceptKeyChangeSort);

INCLUDE_ASM(const s32, "code_001FAE80", ApplyChangeListIndex);

INCLUDE_ASM(const s32, "code_001FAE80", AcceptKeyListWindow);

INCLUDE_ASM(const s32, "code_001FAE80", CreateSortIndexByRace);

INCLUDE_ASM(const s32, "code_001FAE80", CreateSortIndexByLevel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRaceOrder);

INCLUDE_ASM(const s32, "code_001FAE80", cmpLevelOrder);

INCLUDE_ASM(const s32, "code_001FAE80", FillIndex);

INCLUDE_ASM(const s32, "code_001FAE80", SetElem);

INCLUDE_ASM(const s32, "code_001FAE80", SetElem2);

INCLUDE_ASM(const s32, "code_001FAE80", SetRecordToUnit);

INCLUDE_ASM(const s32, "code_001FAE80", SetRecordToUnitCore);

INCLUDE_ASM(const s32, "code_001FAE80", CompareUnitToRecord);

INCLUDE_ASM(const s32, "code_001FAE80", ExecSummon);

INCLUDE_ASM(const s32, "code_001FAE80", drawRegistList);

INCLUDE_ASM(const s32, "code_001FAE80", CreateFontListRead);

INCLUDE_ASM(const s32, "code_001FAE80", ReleaseFontList);

INCLUDE_ASM(const s32, "code_001FAE80", CreateFontExplain);

INCLUDE_ASM(const s32, "code_001FAE80", ReleaseFontExplain);

INCLUDE_ASM(const s32, "code_001FAE80", FadeOutRibbon);

INCLUDE_ASM(const s32, "code_001FAE80", FadeInRibbon);

INCLUDE_ASM(const s32, "code_001FAE80", GetDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", PrepSummon);

INCLUDE_ASM(const s32, "code_001FAE80", drawSummonCount);

INCLUDE_ASM(const s32, "code_001FAE80", drawEncycRatio);

INCLUDE_ASM(const s32, "code_001FAE80", CalcFadeAlpha);

INCLUDE_ASM(const s32, "code_001FAE80", CalcListElemColor);

INCLUDE_ASM(const s32, "code_001FAE80", RequestModelLoad);

INCLUDE_ASM(const s32, "code_001FAE80", SetScreenOffset);

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

INCLUDE_ASM(const s32, "code_001FAE80", ChangeDevilObjectReadMain);

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveRegistKeyHelp);

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveReadMainHelp);

INCLUDE_ASM(const s32, "code_001FAE80", CalcMoveViewModeRibbon);

INCLUDE_ASM(const s32, "code_001FAE80", DrawListWindowBack);

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

INCLUDE_ASM(const s32, "code_001FAE80", InitializeDebug_fclEncyc);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnCalc);

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnDraw);

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnSwitchActive);

INCLUDE_ASM(const s32, "code_001FAE80", fclRbnSetPara);

INCLUDE_ASM(const s32, "code_001FAE80", dumpCursorInfo);

INCLUDE_ASM(const s32, "code_001FAE80", MoveActive);

INCLUDE_ASM(const s32, "code_001FAE80", MoveDeactive);

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

INCLUDE_ASM(const s32, "code_001FAE80", rstChkEvoAct);

INCLUDE_ASM(const s32, "code_001FAE80", rstKillEvoAct);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetTestParam);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkLevelUpTarget);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkResultEntry);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcLevelFromExp);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcExpFromLevel);

void rstChkResult2Process(u8 *work)
{
    memset(&work[0x129F], 0, 6);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstCreateTargetList);

INCLUDE_ASM(const s32, "code_001FAE80", rstInit);

INCLUDE_ASM(const s32, "code_001FAE80", rstDestroyResult2);

INCLUDE_ASM(const s32, "code_001FAE80", rstResult2ProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", rstResult2ProcessEnd);

INCLUDE_ASM(const s32, "code_001FAE80", checkResultStat);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetLevelUpFlag);

void rstAddExp(s32 amount, datUnitWork_t *work)
{
    if (work->level < 0xFF) {
        work->level += amount;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", rstAdvanceLevelUpStep);

INCLUDE_ASM(const s32, "code_001FAE80", func_0021FE18);

void func_0021FE30(s8 a, s8 b, f32 x, f32 y)
{
    EventStart->unk_22E5 = a;
    EventStart->unk_22E8 = x;
    EventStart->unk_22EC = y;
    EventStart->unk_22E6 = b;
}

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawEvoModel);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetEvoState);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetMotionReqFromTbl);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkAddSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetEquipHeartsAisyo);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DA90);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DAA0);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetDestroySkillMaxHpMp);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetMaxHpMp);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkParamLimitAll);

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

INCLUDE_ASM(const s32, "code_001FAE80", rstChkSkillPowerUp1);

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

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSkillPowerUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkEvoEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcEvo);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSeqEvoAt);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcSeqDestroySkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstChkHeartsSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstRecalcHeartsSkill);

void rstLoadHeartsSkill(void) {
}

void rstStoreHeartsSkill(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", rstInitSkillAct);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetParam);

s8 func_00221DB0(s8 index)
{
    return D_00339E00[index].unk_19;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkGetHearts);

INCLUDE_ASM(const s32, "code_001FAE80", datFindCommentEntry);

INCLUDE_ASM(const s32, "code_001FAE80", datGetPlayerLNDLevel);

s32 datGetDevilRace(s8 index)
{
    return D_00339E00[index].race & 7;
}

INCLUDE_ASM(const s32, "code_001FAE80", datGetPlayerLNDList);

INCLUDE_ASM(const s32, "code_001FAE80", datGetDevilSlotIndex);

INCLUDE_ASM(const s32, "code_001FAE80", func_00222120);

INCLUDE_ASM(const s32, "code_001FAE80", datDevilIsRegistered);

INCLUDE_ASM(const s32, "code_001FAE80", datCountPlayerLND);

INCLUDE_ASM(const s32, "code_001FAE80", datGetDevilSlotState);

INCLUDE_ASM(const s32, "code_001FAE80", rstSetDevilRegistered);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetHeartsSkillAt);

INCLUDE_ASM(const s32, "code_001FAE80", func_00222250);

INCLUDE_ASM(const s32, "code_001FAE80", datGetDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetHeartsParamByIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetHeartsParamEx);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetHeartsParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddHeartsParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmbSubHeartsParam);

INCLUDE_ASM(const s32, "code_001FAE80", datAddHeartsSlot);

INCLUDE_ASM(const s32, "code_001FAE80", datAddHeartsEntry);

INCLUDE_ASM(const s32, "code_001FAE80", func_00222610);

INCLUDE_ASM(const s32, "code_001FAE80", rstCheckHeartsSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmbInitStockUnit);

INCLUDE_ASM(const s32, "code_001FAE80", cmb_222828);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCheckEvoUnlocked);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCountEvoCandidates);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkEvoNeedFlag);

INCLUDE_ASM(const s32, "code_001FAE80", func_002229E8);

INCLUDE_ASM(const s32, "code_001FAE80", datFindKugatatiEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoStats);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCheckEvoPair);

INCLUDE_ASM(const s32, "code_001FAE80", cmbFindEvoEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmbEvoEntryExists);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCountEvoReady);

INCLUDE_ASM(const s32, "code_001FAE80", func_00222DC0);

INCLUDE_ASM(const s32, "code_001FAE80", cmbAddLevelUpParamEx);

INCLUDE_ASM(const s32, "code_001FAE80", rst_223080);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindEvoEntryType7);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindEvoEntryType5);

INCLUDE_ASM(const s32, "code_001FAE80", cmbChkDevilEvo);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindKugatatiMsgEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCalcEvoEvent);

INCLUDE_ASM(const s32, "code_001FAE80", rstCalcEvoLevelDiff);

INCLUDE_ASM(const s32, "code_001FAE80", cmbCopyEvoDevilToStock);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindPowerUpSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstRollPowerUpSkill);

INCLUDE_ASM(const s32, "code_001FAE80", datCalcDevilExp);

INCLUDE_ASM(const s32, "code_001FAE80", datCopyDevilWorkEntry);

s32 datAdvanceDevilLevel(datUnitWork_t *work)
{
    return datCopyDevilWorkEntry(work, work->level + 1);
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00223748);

INCLUDE_ASM(const s32, "code_001FAE80", func_00223788);

INCLUDE_ASM(const s32, "code_001FAE80", rstRollComment);

INCLUDE_ASM(const s32, "code_001FAE80", func_00223890);

INCLUDE_ASM(const s32, "code_001FAE80", cmbGetLevelUpCount);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawGetCursorRow);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusRow);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusRow2);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawStatusPanel);

INCLUDE_ASM(const s32, "code_001FAE80", rstFindUnitScene);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetEncycPanel);

INCLUDE_ASM(const s32, "code_001FAE80", rstResetEncycPanel);

INCLUDE_ASM(const s32, "code_001FAE80", rstResetEncycPanel2);

INCLUDE_ASM(const s32, "code_001FAE80", rstDrawEvoBgFade);

INCLUDE_ASM(const s32, "code_001FAE80", rstDraw);

void rstOverWriteSkill(u16 *slot, u16 skill)
{
    *slot = skill;
    rstSetMaxHpMp(1, startPos->unk_16C);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstAddSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstBuildDestroyList);

INCLUDE_ASM(const s32, "code_001FAE80", rstSkillSlotIsFull);

INCLUDE_ASM(const s32, "code_001FAE80", rstWaitConfirmButton);

void rstResetAsignParam(void)
{
    startPos->unk_19 = startPos->unk_18;
    rstChkResult2Process((u8 *)startPos);
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00224230);

void rstAddAsignParam(void)
{
    func_00224230(startPos->unk_16C);
}

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqAsignPlayerParam);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqPlayerLevelUpOnly);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDevilParam);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqGift);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqDefaultSkill);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateSeqSkillPowerUp);

INCLUDE_ASM(const s32, "code_001FAE80", rstStartIntroduce);

INCLUDE_ASM(const s32, "code_001FAE80", rstUpdateEvoBgFade);

INCLUDE_ASM(const s32, "code_001FAE80", rstStartEvoActSE);

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

INCLUDE_ASM(const s32, "code_001FAE80", rstSetHeartsRecoverPlayer);

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

INCLUDE_ASM(const s32, "code_001FAE80", sdfSndMakeFilename);

INCLUDE_ASM(const s32, "code_001FAE80", sndInit);

s32 snd_226388(void)
{
    return 0x7B0;
}

s32 snd_226390(void)
{
    p2sdPlayBGM(scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", dds3_2263B8);

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

INCLUDE_ASM(const s32, "code_001FAE80", titleFreeWork);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DD98);

INCLUDE_ASM(const s32, "code_001FAE80", dds3Titlemain);

s32 titleStart(s32 arg)
{
    titleAllocWork(arg);
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", titleExit);

s32 msel_2286A0(void)
{
    titleAllocWork(0);
    return 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawMenuText);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawSaveNum);

INCLUDE_ASM(const s32, "code_001FAE80", mselUpdateCursor);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawTitleAnim);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawBackground);

INCLUDE_ASM(const s32, "code_001FAE80", mselDrawTitleAnim2);

INCLUDE_ASM(const s32, "code_001FAE80", mselToggleFlag);

INCLUDE_ASM(const s32, "code_001FAE80", mselMakeSaveText);

INCLUDE_ASM(const s32, "code_001FAE80", mselDevilChoice);

INCLUDE_ASM(const s32, "code_001FAE80", Msel_TOverData_Clear);

INCLUDE_ASM(const s32, "code_001FAE80", mselCountSaves);

INCLUDE_ASM(const s32, "code_001FAE80", mselFindSaveSlot);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE70);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DE88);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DEA8);

INCLUDE_ASM(const s32, "code_001FAE80", mselLoadBackground);

INCLUDE_ASM(const s32, "code_001FAE80", mselFreeTextures);

INCLUDE_ASM(const s32, "code_001FAE80", mselShutDown);

INCLUDE_ASM(const s32, "code_001FAE80", mselFreeResources);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053DF08);

INCLUDE_ASM(const s32, "code_001FAE80", mselMain);

s32 mselExit(void)
{
    mselShutDown();
    return -1;
}

INCLUDE_ASM(const s32, "code_001FAE80", mselInit);

s32 fldWmapIsEnabled(void)
{
    return fldWmapEnabled;
}

INCLUDE_ASM(const s32, "code_001FAE80", mselStartDataSet);

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

INCLUDE_ASM(const s32, "code_001FAE80", stfFreeTextures);

INCLUDE_ASM(const s32, "code_001FAE80", stfGetUIField);

INCLUDE_ASM(const s32, "code_001FAE80", stfResetUI);

INCLUDE_ASM(const s32, "code_001FAE80", stfMain2);

INCLUDE_ASM(const s32, "code_001FAE80", GetoStaffUI);

INCLUDE_ASM(const s32, "code_001FAE80", ObjDispOff);

INCLUDE_ASM(const s32, "code_001FAE80", stfDestroy);

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

INCLUDE_ASM(const s32, "code_001FAE80", movieDrawProc);

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

INCLUDE_ASM(const s32, "code_001FAE80", moviePlayFile_27041744);

INCLUDE_ASM(const s32, "code_001FAE80", dds3PlayMovieByIndex);

INCLUDE_ASM(const s32, "code_001FAE80", movieStop);

s32 dds3WaitMovieEnd(void)
{
    return sdfMovieIsFinished(D_00343E40);
}

u8 func_0022B7F8(void)
{
    return D_00343E74[0];
}

INCLUDE_ASM(const s32, "code_001FAE80", movieMan_Process);

INCLUDE_ASM(const s32, "code_001FAE80", dds3StartMovieManager);

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

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerFree);

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerScanDir);

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerStep);

INCLUDE_ASM(const s32, "code_001FAE80", movieDebugDrawInfo);

INCLUDE_ASM(const s32, "code_001FAE80", MovieViewerDrawFrameInfo);

INCLUDE_ASM(const s32, "code_001FAE80", MovieViewerProc);

INCLUDE_ASM(const s32, "code_001FAE80", InitMovieViewer);

INCLUDE_ASM(const s32, "code_001FAE80", FinishMovieViewer);

void movieViewerSetFlags(void)
{
    movieViewerDebugFlag[2] = 1;
    movieViewerDebugFlag[3] = 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", movieViewerInitRegs);

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

INCLUDE_ASM(const s32, "code_001FAE80", camp_22C610);

void func_0022C658(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", campDebugRegistAll);

INCLUDE_ASM(const s32, "code_001FAE80", dds3ClearStockTable);

void cmpInitLocalStock(void)
{
    campWork->stock.count = 3;
    cmpBuildStockNameList(&campWork->stock);
    cmpInitStockCursorPos();
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitDrawList);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitStockTable);

INCLUDE_ASM(const s32, "code_001FAE80", cmpinit);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_Shutdown);

INCLUDE_ASM(const s32, "code_001FAE80", cmpProcessStart);

INCLUDE_ASM(const s32, "code_001FAE80", campKillProcesses);

INCLUDE_ASM(const s32, "code_001FAE80", campCheckExitState);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSelectSrcStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSelectedStockIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSelectDstStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSelectItemID);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetItSkMessageVar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetItSkErrSelFlag);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindSkillMsgIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindItemMsgIndex);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitStockCursorPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpReCalcStockCursorPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitRootCursorPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitSelectCursor);

s8 camp_H_cmpLocalStock_t__ctor(void)
{
    cmpLocalStock_t stock;

    stock.count = 9;
    cmpBuildStockNameList(&stock);
    return stock.unk_02;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartStockMsg);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcRoot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetSkillTSL);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetItemTSL);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateList);

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

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawBox);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawSizedSprite);

INCLUDE_ASM(const s32, "code_001FAE80", nmeLerp);

INCLUDE_ASM(const s32, "code_001FAE80", nmeLerp2);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawFrame);

INCLUDE_ASM(const s32, "code_001FAE80", nmeDrawBlendText);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawHelp);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_2304C0);

void cmpDisableInfoWindow(void)
{
    cmpSetInfoWindowHidden();
}

INCLUDE_ASM(const s32, "code_001FAE80", campDrawText);

INCLUDE_ASM(const s32, "code_001FAE80", func_002306A0);

INCLUDE_ASM(const s32, "code_001FAE80", campSkillTargetIsSelf);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockRow);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawSetPhase);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawNumber);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawFade);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB28);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB50);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FB68);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDebugCheckPad);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateRoot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateItem);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateParty);

INCLUDE_ASM(const s32, "code_001FAE80", campUpdateStockSelect);

INCLUDE_ASM(const s32, "code_001FAE80", campUpdateStockCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateSkillSelect);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateProcessStatus);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRemoveStockDevil);

INCLUDE_ASM(const s32, "code_001FAE80", campWaitSelMessage);

INCLUDE_ASM(const s32, "code_001FAE80", campCloseInfoWindow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateDestroyItemConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", campShowStock);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateLoad);

INCLUDE_ASM(const s32, "code_001FAE80", campUpdateEffectStep);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStockVisible);

INCLUDE_ASM(const s32, "code_001FAE80", cmpClearStatusFlags);

INCLUDE_ASM(const s32, "code_001FAE80", camp_calc_stock_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockPanel);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_stock_Process);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_stock_Process);

void cmpSetDHEntry(void) {
}

INCLUDE_ASM(const s32, "code_001FAE80", datCountRegisteredHearts);

INCLUDE_ASM(const s32, "code_001FAE80", cmpHeartsIconIsVisible);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDhCheckReady);

INCLUDE_ASM(const s32, "code_001FAE80", campDhIconIsReady);

INCLUDE_ASM(const s32, "code_001FAE80", campDhCheckTextures);

INCLUDE_ASM(const s32, "code_001FAE80", cmpLoadDHTextures);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFreeTexture);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFreeResources);

s32 func_00232C08(void)
{
    return campInfoWork->unk_01 == 0;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetHeartsSequenceRemove);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitHeartsIcon);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitDHWindow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpinitDH);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_dh_Shutdown);

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

INCLUDE_ASM(const s32, "code_001FAE80", fclCalcArrowCursorOffset);

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

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHeartsIcon);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawIcon);

INCLUDE_ASM(const s32, "code_001FAE80", campDhDrawHearts);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDC0);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDD8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FDE8);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_dh_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDhApplyEntry);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFindEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmpReturnParamDH);

INCLUDE_ASM(const s32, "code_001FAE80", campDhEquipHearts);

INCLUDE_ASM(const s32, "code_001FAE80", campDhUpdateCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUnloadStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsAttachStatus);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStepHeartsAttachCursor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsAttachConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateHeartsRemoveConfirm);

INCLUDE_ASM(const s32, "code_001FAE80", campDhWaitMessageEnd);

INCLUDE_ASM(const s32, "code_001FAE80", campDhAdvanceAnim);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFadeAlpha);

INCLUDE_ASM(const s32, "code_001FAE80", campDhUpdateIcons);

INCLUDE_ASM(const s32, "code_001FAE80", campDhRollRatio);

INCLUDE_ASM(const s32, "code_001FAE80", campDhGetEntryRatio);

INCLUDE_ASM(const s32, "code_001FAE80", campDhScanEntries);

INCLUDE_ASM(const s32, "code_001FAE80", campDhFindVisibleIcon);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_dh_Process);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawSkillPanel);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpStatSetTimer);

void func_00235E40(s8 index)
{
    D_00574A48[index] = 1;
}

INCLUDE_ASM(const s32, "code_001FAE80", func_00235E60);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusCom);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusComEx);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcTargetDevil);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAdvanceStatusAnim);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusComEx2);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawItemListHeader);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawItemListScrollBar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawItemListRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCalcItemEntry);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawInfoPanel);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawSeq3);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillListHeader);

u8 cmpGetSkillIcon(u16 skill)
{
    return datSkillTable[skill].icon;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetSkillCost);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillListRow);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillListScrollBar);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawSkillList);

INCLUDE_ASM(const s32, "code_001FAE80", cmpExpandColor);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmp_239FA0);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawTriG);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawBoxSolid);

s32 cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    return cmpDrawBoxSolid(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawBoxG);

s32 cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
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

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23A8B0);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_0023A9A8);

INCLUDE_ASM(const s32, "code_001FAE80", func_0023A9F0);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23AA28);

void func_0023AA88(s32 value)
{
    D_0054FEBC = value;
}

INCLUDE_ASM(const s32, "code_001FAE80", func_0023AA90);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23AAC0);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_23AAF8);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_0023AD30);

INCLUDE_ASM(const s32, "code_001FAE80", cmpInitStatus);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_status_Shutdown);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocInfoProcess);

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

INCLUDE_ASM(const s32, "code_001FAE80", camp_calc_info_Process);

INCLUDE_ASM(const s32, "code_001FAE80", camp_draw_info_Process);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_info_Process);

INCLUDE_ASM(const s32, "code_001FAE80", camp_H_cmpGlobalWork_t__ctor);

void cmpSetCrossGeneralVal(u8 *work, s8 value)
{
    work[0x50] = value;
}

INCLUDE_ASM(const s32, "code_001FAE80", campInitPanelPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetAfterCrossFunc);

INCLUDE_ASM(const s32, "code_001FAE80", campRunPanelCallback);

INCLUDE_ASM(const s32, "code_001FAE80", fclInitCursorWindow);

void fclSetCursorList(void **work, void *list)
{
    work[1] = list;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorBlinkColor);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorFadeChannel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListItemBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListItemText);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawListFrame);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetBlinkOffset);

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

INCLUDE_ASM(const s32, "code_001FAE80", campGetCursorColorSel);

INCLUDE_ASM(const s32, "code_001FAE80", campGetCursorColorAlt);

INCLUDE_ASM(const s32, "code_001FAE80", campGetCursorColorAlt2);

INCLUDE_ASM(const s32, "code_001FAE80", campGetStockAlpha);

INCLUDE_ASM(const s32, "code_001FAE80", campCalcStockScale);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockCell);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockStatus);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockLabel);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockLabel2);

INCLUDE_ASM(const s32, "code_001FAE80", cmpStockHasRoom);

INCLUDE_ASM(const s32, "code_001FAE80", campDrawStockIcon);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdatePanelEffEntry);

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

INCLUDE_ASM(const s32, "code_001FAE80", func_0023EDB0);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetPanelEffState);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAddPanelEffEntry);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAddQuePanelEff);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffBox);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffRing);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawPanelEffQuad);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FF60);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindPanelEffSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdatePanelEffSlots);

INCLUDE_ASM(const s32, "code_001FAE80", camp_update_paneleff_Process);

INCLUDE_ASM(const s32, "code_001FAE80", cmpClearPanelEffSlots);

INCLUDE_ASM(const s32, "code_001FAE80", cmpAllocPanelEffWork);

void camp_update_paneleff_Shutdown(void)
{
    if (cmpPanelEffList != NULL) {
        sdfMemFreeDirect(cmpPanelEffList->allocation);
        cmpPanelEffList = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpStartProcessPanelEff);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetModelMotionId);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetModelScale);

INCLUDE_ASM(const s32, "code_001FAE80", rstGetEvoModelParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetModelPosRot);

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

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FFE8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0053FFF8);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540000);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540008);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540018);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540020);

INCLUDE_ASM(const s32, "code_001FAE80", cmpModelFreeResource);

void cmpPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    D_0054FEE0 = mdlPrepare(group, id, blocking);
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCreateBadEffect);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540030);

INCLUDE_ASM(const s32, "code_001FAE80", cmpBadEffSetup);

INCLUDE_RODATA(const s32, "code_001FAE80", D_00540054);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFreeBadEffect);

INCLUDE_ASM(const s32, "code_001FAE80", rstPrepareStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpModelSetup);

INCLUDE_RODATA(const s32, "code_001FAE80", D_0054007C);

INCLUDE_ASM(const s32, "code_001FAE80", cmpLoadStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStatusModelMotion);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStatusModelScale);

INCLUDE_ASM(const s32, "code_001FAE80", cmpResetStatusModelPos);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStatusModelPose);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetModelRotate);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSetStatusModelParam);

INCLUDE_ASM(const s32, "code_001FAE80", cmpDrawStatusModel);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRebuildStockList);

INCLUDE_ASM(const s32, "code_001FAE80", cmpRemoveStockSlot);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCompactStockList);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpChkSkillBad);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmp_241A10);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpResetCursorInfo);

INCLUDE_ASM(const s32, "code_001FAE80", rcvRootHasItems);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorAtFirstEnabled);

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorAtLastEnabled);

INCLUDE_ASM(const s32, "code_001FAE80", cmpFindNextCursorItem);

void cmpAdvanceCursorPhase(cmpCursorWork_t *cursor)
{
    cursor->unk_34 += 6;
    if (cursor->unk_34 > 0x200) {
        cursor->unk_34 -= 0x200;
    }
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpMoveCursor);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpMoveStockCursor);

void func_00242320(s8 value, u8 *work)
{
    work[0x2124] = value;
}

INCLUDE_ASM(const s32, "code_001FAE80", cmpCursorSlotIsSelectable);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStockInfoIndex);

INCLUDE_ASM(const s32, "code_001FAE80", cmpBuildStockNameList);

INCLUDE_ASM(const s32, "code_001FAE80", cmpGetStockCount);

INCLUDE_ASM(const s32, "code_001FAE80", cmpSortStockList);

INCLUDE_ASM(const s32, "code_001FAE80", func_002426F0);

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

INCLUDE_ASM(const s32, "code_001FAE80", cmpChkSeqInfo);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateSeqInfo);

INCLUDE_ASM(const s32, "code_001FAE80", cmpUpdateTerminate);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_242918);

INCLUDE_ASM(const s32, "code_001FAE80", cmp_242958);

u8 cmp_242998(s32 id)
{
    u8 *data = dds3GetProcessData(id);

    if (data == NULL) {
        return 0;
    }
    return data[0];
}

INCLUDE_ASM(const s32, "code_001FAE80", func_002429C0);

INCLUDE_ASM(const s32, "code_001FAE80", cmpItemHasFlag);

void func_00242A70(void) {
}

s32 campStatusIsActive(void)
{
    return campStatusActive != 0;
}

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

