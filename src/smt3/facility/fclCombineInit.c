#include "common.h"
#include "basic.h"
#include "motion.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    u16 devilId;
    u16 unk_02;
    s32 flag;
} cmbUnlockEntry_t;

typedef struct
{
    u16 devilId;
    u16 selector[2];
    u8 percent;
} cmbEvoTableEntry_t;

typedef struct
{
    u16 skill[8];
    s8 count;
} cmpSkillList_t;

extern char D_0053C938[];

extern char D_0054FB00[];

extern cmbUnlockEntry_t D_0032DCC0[];

extern cmbEvoTableEntry_t D_0032DBB8[];

extern u16 D_0032DCA0[];

extern s8 cmbCheckEvoUnlocked(u16 devilId, datUnitWork_t *list, u8 count);

extern s8 gProcessStat_0;

extern cmpSkillList_t *cmpSkillCursor;

extern s32 cmbDrawSequence(void);

extern void *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                              void *drawExit, void *deleteExit, void *p);

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

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

extern u32 dds3GetRandA(void *state);

extern u32 datGetMaxHp(datUnitWork_t *work);

extern u32 datGetMaxMp(datUnitWork_t *work);

extern s32 cmbLookupCombine(u16 devilA, u16 devilB, s8 flag);

extern s32 cmbCalcResultLevel(cmbUnit_t *unitA, cmbUnit_t *unitB, cmbUnit_t *unitC, s8 flag);

extern s32 cmbGetRaceCombineResult(s32 race, u8 level);

extern s32 evtBitCheck(s32 no);

extern s32 evtStartEvent(s32 no);

extern s32 evtGetAgeOfMoon(void);

extern void *dds3SearchProcessName(const char *namestr);

extern s32 dds3GetProcessStat(void *id);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

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

extern rcvWork_t *shutdownfunc;

extern char D_0053CF10[];

extern void sortSkillList(cmbUnit_t *unit);

extern void p2sdStopBGM(s32 id);

extern s32 mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

extern void sdfMemFreeDirect(void *aHandle);

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

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

extern s8 cmbAddLevelUpParamEx(void *work, s8 mode);

extern void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst);
extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

extern s32 cmpDrawSpriteFlag(u8 id, s32 flag, s32 arg);

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
    /* 0x00 */ s32 tex;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ s32 X;
    /* 0x14 */ s32 Y;
    /* 0x18 */ s32 Z;
    /* 0x1C */ u8 color[4];
    /* 0x20 */ u8 Flag;
} fclSprite_t;

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

extern void fclFilterOFF(void);

extern void fclSetSprite(fclSprite_t *pSprite, s32 Xsize, s32 Ysize, s32 Z, s32 U1, s32 V1, s32 U2,
                         s32 V2, s32 Col, s32 TexHandle);

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
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
} cmpCursorInfo_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x15C];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} cmbListWork_t;

extern void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s8 Style,
                              cmbListWork_t *work);

extern void cmpModelInitCore2(void);

extern s8 dds3PadButtonState[];

extern void *dds3GetProcessData(s32 id);

typedef struct
{
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ u8 unk_04[0x10];
} cmbSlotParam_t;

extern cmbSlotParam_t D_0032DE28[];

extern s32 p2sdPlayBGM(s32 MidiNo);

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

extern s32 fclRunPanelAnim(void *work);

extern s32 printf(const char *fmt, ...);

extern u8 D_0032D1F0[];

extern void sdfTexFree(s32 aHandle);

typedef struct
{
    /* 0x00 */ s8 index;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 id;
} cmbStockSlot_t;

typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ u8 unk_0038[0x1004];
} cmbStockCursorGroup_t;

typedef struct
{
    /* 0x00 */ s32 handle;
    /* 0x04 */ void *lock;
    /* 0x08 */ s32 size;
    /* 0x0C */ s16 id;
    /* 0x0E */ u8 unk_0E[2];
    /* 0x10 */ struct mdlObj_t *model;
} cmbActionData_t;

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
    /* 0x0000C */ u8 unk_0000C[8];
    /* 0x00014 */ void *resourceHandle;
    /* 0x00018 */ u8 unk_00018[4];
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x1004];
    /* 0x01058 */ cmbStockCursorGroup_t stockCursorGroup[3];
    /* 0x0410C */ u8 unk_0410C[2];
    /* 0x0410E */ u8 evoCandidateCount;
    /* 0x0410F */ u8 reserved0410F;
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x2C9];
    /* 0x043E5 */ s8 resultIndex[2][12][12];
    /* 0x04505 */ u8 reserved04505;
    /* 0x04506 */ u16 resultId[2][12][12];
    /* 0x04746 */ u8 reserved04746[0x11DC4];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B;
    /* 0x1650C */ void *resourceHandle_1650C;
    /* 0x16510 */ s32 script;
    /* 0x16514 */ u8 *scriptArg;
    /* 0x16518 */ void *resourceHandle_16518;
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ cmbActionData_t actionModel;
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x11];
    /* 0x165ED */ u8 stockCursorCount;
    /* 0x165EE */ u8 unk_165EE[0x1052];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ void *panel;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern cmbGbWork_t *CMB_GBWK;

extern s32 datAddItem(s32 id, s32 num);
extern s8 cmbCountEvoCandidates(datUnitWork_t *list, u8 count);

extern s32 cmpStatSetTimer(s32 kind, s8 index);

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 dds3FadeCheck(void);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

extern void cmpClearCursorSlots(cmpCursorInfo_t *cursor);

extern void cmbCreateLocalStock(void);

extern s32 func_002043E0(cmbActionData_t *work);

extern void mdlSmoothLoopMotion(void *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                                f32 aSmoothLeng);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void fclScriptForcedEnd(u8 *PID);

extern void cmbCreateCombineResultTable(s32 mode);

extern u32 fclBlendColor(u32 color, u32 mask, u32 alpha);

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

extern void cmpInitCursorRange(cmpCursorInfo_t *cur, u8 count, u8 visible, s32 layout);

extern void cmpInitCursorLayout(cmpCursorInfo_t *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern void cmpModelFreeResource(void);

extern void cmpResetDrawEnv(void);

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern void cmpUpdateTerminate(cmpInfoWindow_t *window);

extern void fclEncycSetExplainData(void *pbmd);

extern void frDestroyFontRequestAfter(void *pFRQ);

extern u8 D_00551F18[4];
extern void cmpSetCursorFlag(cmpCursorInfo_t *cursor, s32 flags);

void fclCombineDisableCmdZensyo(void)
{
    s32 enabled = (evtBitCheck(0x28) ^ 1) != 0;
    s32 allow = evtBitCheck(0x8A0) ^ 1;

    enabled = (allow != 0) ? enabled : 0;
    if (enabled != 0) {
        cmpSetCursorFlag(&CMB_GBWK->combineCursor, (s32)D_00551F18);
        if ((D_00551F18[1] & 2) == 0) {
            D_00551F18[1] |= 2;
            CMB_GBWK->combineCursor.unk_21++;
        }
    }
}

extern u8 cmbMenuEntryTable[8];
extern void cmpSetCursorFlag(cmpCursorInfo_t *cursor, s32 flags);

void cmbEnableMenuEntry5(void)
{
    s32 enabled = (evtBitCheck(0x28) ^ 1) != 0;
    s32 allow = evtBitCheck(0x8A0) ^ 1;

    enabled = (allow != 0) ? enabled : 0;
    if (enabled != 0) {
        cmpSetCursorFlag(&CMB_GBWK->menuCursor, (s32)cmbMenuEntryTable);
        if ((cmbMenuEntryTable[5] & 2) == 0) {
            cmbMenuEntryTable[5] |= 2;
            CMB_GBWK->menuCursor.unk_21++;
        }
    }
}

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

void fclCombineEventStart(s8 Signal)
{
    fclSetWorkValue(1, Signal);
    fclClearFlag(1);
    evtStartEvent(0x320);
}

void fclCombineInitWork(void)
{
    fclSetWorkValue(1, 100);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCombineLoadTextures);

void fclCombineTexFree(void)
{
    s8 i;

    for (i = 0; i < 7U; i++) {
        sdfTexFree(CMB_GBWK->texHandle[i]);
    }
    printf("COMBINE : Tex free.\n");
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

s32 fclCombineCheckMoonPhase(void)
{
    s32 found = 0;
    s32 i;
    dds3PartyUnit_t *party;

    if (evtGetAgeOfMoon() != 0) {
        return 0;
    }
    i = 0;
    party = cmbGlobalWork->party;
    for (; i < 16; i++, party++) {
        if (party->unit.flag & 1) {
            switch (party->unit.id) {
            case 0x51:
                found |= 1;
                break;
            case 0x52:
                found |= 2;
                break;
            }
        }
    }
    return found == 3 ? 0 : found;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCombineBGM);

void fclCombineInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x1A724);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x30, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclUpdateInit);

extern s32 fclDestroyPanel(void *pPanel);
extern void cmpSetSkillCursor(cmpSkillList_t *list);
extern void fclCombineBGM(s8 Mode);
extern void evtDisableMoonDraw(void);

void fclCombineDestroy(void)
{
    if (CMB_GBWK == NULL) {
        return;
    }
    fclCombineTexFree();
    fclDestroyPanel(CMB_GBWK->panel);
    cmpSetSkillCursor(NULL);
    cmpModelFreeResource();
    cmpResetDrawEnv();
    fclScriptForcedEnd(CMB_GBWK->scriptArg);
    sdfMemFreeDirect(CMB_GBWK->resourceHandle_1650C);
    sdfMemFreeDirect(CMB_GBWK->resourceHandle_16518);
    fclCombineInitWork();
    fclCombineBGM(1);
    sdfMemFreeDirect(CMB_GBWK->resourceHandle);
    CMB_GBWK = NULL;
    gProcessStat_0 = 2;
    evtDisableMoonDraw();
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCombineCalc);

s32 fclCombineDraw(void)
{
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    return cmbDrawSequence();
}

extern s32 cmbUpdateSequence(void);

s32 fclCombineUpdate(void)
{
    s32 ret;
    s32 flag;

    if ((CMB_GBWK->unk_00000 & 0x40) != 0) {
        ret = -1;
        if (cmpLoadStatusModel() != 0) {
            ret = 0;
        }
        return ret;
    }
    flag = fclGetFlag(1);
    if (flag == 0) {
        return flag;
    }
    if (cmbUpdateSequence() == -1) {
        CMB_GBWK->unk_00000 |= 0x10;
    }
    return 0;
}

void fclCombineProcessStart(void)
{
    fclUpdateInit();
    dds3StartProcess(D_0054FB00, 0x400, 1, 1, fclCombineCalc, NULL, NULL);
    dds3StartProcess("combine_draw", 0x2B10, 1, 1, fclCombineDraw, NULL, NULL);
    dds3StartProcess(D_0053C938, 0x520C, 1, 1, fclCombineUpdate, fclCombineDestroy, NULL);
    gProcessStat_0 = 1;
}

void fclCombineProcessEnd(void)
{
    dds3KillProcessName(D_0054FB00, 1);
    dds3KillProcessName("combine_draw", 1);
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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetBirthPhase);

s32 cmbBirthDevilIsAvailable(void)
{
    u16 flag = cmbGlobalWork->party[0].unit.badstatus & 0x8000;

    return flag != 0;
}

void cmbBuildResultRow(s32 mode)
{
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++) {
            CMB_GBWK->resultIndex[mode][j][i] = -1;
            CMB_GBWK->resultId[mode][j][i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCreateCombineResultTable);

void cmbCreateLocalStock(void)
{
    s32 i;

    CMB_GBWK->stockCursorCount = 0;
    CMB_GBWK->evoCandidateCount =
        cmbCountEvoCandidates(&cmbGlobalWork->party[4].unit, 12);

    for (i = 4; i < 16; i++) {
        if ((cmbGlobalWork->party[i].unit.flag & 1) != 0 &&
            (cmbGlobalWork->party[i].unit.badstatus & 0x800) == 0 &&
            (cmbGlobalWork->party[i].unit.flag & 0x10) == 0) {
            CMB_GBWK->unk_165EE[(s8)CMB_GBWK->stockCursorCount++] = i;
        }
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCalcRoot);

void cmbCalcDevSel1st(void)
{
    if (CMB_GBWK->unk_00004 != 0) {
        cmbCreateLocalStock();
    }
    if (CMB_GBWK->unk_00004 != 0) {
        *(s8 *)((u8 *)CMB_GBWK + 0x43E4) = 0;
    }
    cmbCreateCombineResultTable(0);
    if (CMB_GBWK->unk_00004 != 1) {
        return;
    }
    cmpInitCursorRange(&CMB_GBWK->stockCursorGroup[0].cursor, CMB_GBWK->stockCursorCount,
                       CMB_GBWK->stockCursorCount, 0);
    cmpInitCursorLayout(&CMB_GBWK->stockCursorGroup[0].cursor, 0x2B, 0x80, 0x1C5, 0x15, 0x17);
    cmpClearCursorSlots(&CMB_GBWK->stockCursorGroup[0].cursor);
    CMB_GBWK->stockSlot[0].index = -1;
    CMB_GBWK->stockSlot[1].index = -1;
    CMB_GBWK->stockSlot[2].index = -1;
    evtDisableMoonDraw();
    fclScriptForcedEnd(CMB_GBWK->scriptArg);
}

void cmbInitStockCursor(void)
{
    if (CMB_GBWK->unk_00004 != 0) {
        cmbCreateLocalStock();
    }

    if (CMB_GBWK->unk_00004 == 1) {
        cmpInitCursorRange(&CMB_GBWK->stockCursorGroup[1].cursor,
                           CMB_GBWK->stockCursorCount, CMB_GBWK->stockCursorCount, 0);
        cmpInitCursorLayout(&CMB_GBWK->stockCursorGroup[1].cursor, 0x2B, 0x80, 0x1C5,
                            0x15, 0x17);
        cmpClearCursorSlots(&CMB_GBWK->stockCursorGroup[1].cursor);
    }
}

void cmbSeqBuildResultTable(void)
{
    if (CMB_GBWK->unk_00004 != 0) {
        cmbCreateLocalStock();
    }

    if (CMB_GBWK->unk_00004 == 1) {
        cmbCreateCombineResultTable(1);
        cmpInitCursorRange(&CMB_GBWK->stockCursorGroup[2].cursor,
                           CMB_GBWK->stockCursorCount, CMB_GBWK->stockCursorCount, 0);
        cmpInitCursorLayout(&CMB_GBWK->stockCursorGroup[2].cursor, 0x2B, 0x80, 0x1C5,
                            0x15, 0x17);
        cmpClearCursorSlots(&CMB_GBWK->stockCursorGroup[2].cursor);
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbSetupBirthPanel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCalcBirthDevil);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbAddBirthDevilToStock);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbStartResultScript);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCalcIntroduce);

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbRunBirthDevilStep);

extern datUnitWork_t *cmbGetBirthDevilPanel(void);
extern u16 cmpGetStatusModelId(void);
extern void cmpSetModelReloadFlag(void);
extern void campFreeStatusModel(void);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053C938);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053C950);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053C988);

void cmbSeqSetPhase(void)
{
    if (CMB_GBWK->unk_00004 == 1) {
        CMB_GBWK->unk_1650A = 3;
    }

    switch ((s8)CMB_GBWK->unk_00001) {
    case 7:
    case 8:
    case 10:
    case 11:
    case 15: {
        u16 modelId = cmbGetBirthDevilPanel()->id;
        if (modelId != cmpGetStatusModelId()) {
            cmpSetModelReloadFlag();
        }
        campFreeStatusModel();
        /* Qualified cc1probe: reset uid89 survives sibling-call selection and
         * is deleted in life analysis, keeping the native jal and epilogue. */
        modelId = 0;
        break;
    }
    case 9:
    case 12:
    case 13:
    case 14:
    default:
        break;
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCalcSequence);

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

s32 cmbGetEvoTable(u16 devilId, datUnitWork_t *unit)
{
    s8 index = cmbFindEvoIndex(devilId);
    cmbEvoTableEntry_t *entry;
    s32 i;
    u8 percent;

    if (index < 0) {
        return 0;
    }
    entry = &D_0032DBB8[index];
    percent = entry->percent;
    for (i = 0; i < 2; i++) {
        s32 selector = (s8)entry->selector[i];
        s8 param;

        if (selector <= 0) {
            continue;
        }
        if (selector >= 7) {
            continue;
        }
        param = selector - 1;
        unit->mitamaparam[param] =
            cmbApplyPercent(unit->param[param], unit->mitamaparam[param], 0x28,
                            percent);
    }
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetCombineRace);

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

typedef struct {
    u16 devilId;
    u8 raceA;
    u8 raceB;
    u16 result;
} cmbRacePairEntry_t;

extern cmbRacePairEntry_t D_0032DC90[];

u16 cmbFindCombineResult(u16 first, u16 second, u16 thirdId)
{
    datDevilInfo_t *devilTable = datDevilTable;
    datDevilInfo_t *third = &devilTable[thirdId];
    cmbRacePairEntry_t *table = D_0032DC90;
    s32 pass = 0;
    u8 race[2];
    u16 result;

    for (; pass < 2; pass++) {
        race[1] = third->race;
        race[0] = devilTable[second].race;
        {
            u32 i = 0;
            cmbRacePairEntry_t *entry = table;
            for (; i < 2; i++, entry++) {
                if (entry->devilId == first && entry->raceA == race[0] &&
                    entry->raceB == race[1]) {
                    result = entry->result;
                    if (cmbDevilIsUnlocked(result) == 0 &&
                        cmbIsSpecialPair(first, second, result) == 0) {
                        return result;
                    }
                }
            }
        }
        {
            u8 swap = first;
            first = second;
            second = swap;
        }
    }
    return 0;
}

typedef struct {
    u16 devilA;
    u16 devilB;
    u16 devilC;
    u16 result;
} cmbTripleEntry_t;

extern cmbTripleEntry_t D_0032DBE0[];
extern cmbPairEntry_t D_0032DC18[];

extern u16 cmbCalcCombineResult(cmbUnit_t *unitA, cmbUnit_t *unitB, s8 flag,
                                s8 levelFlag);

u16 cmbFindUnlockedResult(cmbUnit_t *unitA, cmbUnit_t *unitB, cmbUnit_t *unitC,
                          s8 flag, s8 levelFlag)
{
    cmbTripleEntry_t *table = D_0032DBE0;
    s32 pass = 0;
    u16 ids[3];
    u16 result;
    u32 i;
    cmbPairEntry_t *pair;

    ids[0] = unitA->unit.id;
    ids[1] = unitB->unit.id;
    ids[2] = unitC->unit.id;
    for (; pass < 2; pass++) {
        u32 j = 0;
        cmbTripleEntry_t *entry = table;
        for (; j < 7; j++, entry++) {
            if (entry->devilA == ids[0] && entry->devilB == ids[1] &&
                entry->devilC == ids[2]) {
                result = entry->result;
                if (cmbDevilIsUnlocked(result) == 0) {
                    return result;
                }
            }
        }
        {
            u16 swap = ids[0];
            ids[0] = ids[1];
            ids[1] = swap;
        }
    }
    ids[0] = cmbCalcCombineResult(unitA, unitB, flag, levelFlag);
    ids[1] = unitC->unit.id;
    pair = D_0032DC18;
    for (i = 0; i < 19; i++, pair++) {
        if (pair->devilA == ids[0] && pair->devilB == ids[1]) {
            result = pair->result;
            if (cmbDevilIsUnlocked(result) == 0) {
                return result;
            }
        }
    }
    return 0;
}

s32 cmbFindBirthEntry(u16 devilId)
{
    u32 i;

    for (i = 0; i < 7; i++) {
        if (devilId == D_0032DBE0[i].result) {
            return 1;
        }
    }
    for (i = 0; i < 19; i++) {
        if (devilId == D_0032DC18[i].result) {
            return 1;
        }
    }
    for (i = 0; i < 2; i++) {
        if (devilId == D_0032DC90[i].result) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbFindBirthResult);

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

s32 cmbFindPairEntry(u16 first, u16 second)
{
    u32 i;
    cmbPairEntry_t *entry = D_0032DB90;

    for (i = 0; i < 6; i++, entry++) {
        s32 side = 0;
        if (entry->devilA == first) {
            side = 1;
        }
        if (entry->devilB == first) {
            side = 2;
        }
        switch (side) {
        case 1:
            if (entry->devilB == second) {
                return entry->result;
            }
            break;
        case 2:
            if (entry->devilA == second) {
                return entry->result;
            }
            break;
        }
    }
    return 0;
}

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

/* Race IDs index both matrices from one, not zero. */
extern u8 D_0032D270[2][32][32];

s32 cmbLookupCombine(u16 devilA, u16 devilB, s8 flag)
{
    u8 race[2];
    u8 temp;
    datDevilInfo_t *first = &datDevilTable[devilA];
    datDevilInfo_t *second = &datDevilTable[devilB];

    race[0] = first->race;
    race[1] = second->race;
    if (race[0] == 0x26) {
        race[0] = 0x13;
    }
    if (race[1] == 0x26) {
        race[1] = 0x13;
    }
    if (race[0] < race[1]) {
        temp = race[1];
        race[1] = race[0];
        race[0] = temp;
    }
    if (flag != 0) {
        return D_0032D270[1][race[1] - 1][race[0] - 1];
    }
    return D_0032D270[0][race[1] - 1][race[0] - 1];
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbCalcResultLevel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetRaceCombineResult);

void cmbGetDevilIdAndRace(cmbUnit_t *unit, u16 *devilId, u8 *race)
{
    *devilId = unit->unit.id;
    *race = datDevilTable[unit->unit.id].race;
}

s32 cmbCheckUnlockedPair(u16 first, u16 second)
{
    cmbPairEntry_t *table = D_0032DB78;
    s32 pass;

    for (pass = 0; pass < 2; pass++) {
        u32 i = 0;
        cmbPairEntry_t *entry = table;

        for (; i < 3; i++, entry++) {
            s32 matched = 0;
            if (entry->devilA == first) {
                matched++;
            }
            if (entry->devilB == second) {
                matched++;
            }
            if (matched == 2) {
                u16 result = entry->result;
                if (cmbDevilIsUnlocked(result) == 0) {
                    return result;
                }
            }
        }
        {
            u16 swap = first;
            first = second;
            second = swap;
        }
    }
    return 0;
}

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

s32 cmbFindSpecialEntry(u16 devilId)
{
    s32 i;
    u16 candidate;

    for (i = 13; i >= 0; i--) {
        candidate = D_0032DCA0[i];
        if (cmbCheckEvoUnlocked(candidate, (datUnitWork_t *)&DDS3_GBWK->akuma[0].flag, 16) < 0 &&
            candidate != devilId) {
            return candidate;
        }
    }
    return candidate;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbBuildDevilEntry);

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetCombineResultEx);

extern s32 cmbGetLevelUpCount(datUnitWork_t *work, u32 exp);

void cmbCalcIkenieExp(datUnitWork_t *work, datUnitWork_t *otherA,
                      datUnitWork_t *otherB, datUnitWork_t *expSource)
{
    u32 exp = (u32)((f32)expSource->exp * 1.5f + 0.0f);
    s32 levelUpCount;
    s32 newLevel;

    work->exp += datCopyDevilWorkEntry(work, work->level);
    levelUpCount = cmbGetLevelUpCount(work, exp);
    work->exp += exp;
    newLevel = work->level + levelUpCount;
    if (newLevel >= 0x100) {
        levelUpCount = 0xFF - work->level;
    }
    work->level += levelUpCount;

    while (levelUpCount-- > 0) {
        s8 param = cmbAddLevelUpParamEx(work, 1);
        if (param >= 0) {
            work->param[(s32)param]++;
        }
    }

    work->flag |= 0x200;
    if (work->level == 0xFF) {
        work->exp = datCopyDevilWorkEntry(work, 0xFF);
    }
}

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

s32 cmbMakeBirthDevil(s32 id1, s32 id2, s32 id3, s8 race1, s8 race2, u16 *ids, s8 *races)
{
    datUnitWork_t unit;
    u8 kind;
    u16 id;

    id = cmbGetCombineResultEx(id1, id2, race1, race2, ids, races, &kind);
    if (kind == 2) {
        return 0;
    }
    if (cmbDevilCanBirth(id) != 0) {
        id = 0;
    }
    if (id == 0) {
        return 0;
    }
    cmbCopyDefaultDevilToStock(id, &unit);
    return id;
}

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbInheritSkills);

void rstClearSkillSlot(cmbUnit_t *unit, u8 slot)
{
    unit->unit.keisyoskill[slot] = 0;
    sortSkillList(unit);
}

s8 rstAddSkillToSlot(u16 skill, datUnitWork_t *unit);

s8 cmbReplaceSkill(u16 skill, cmbUnit_t *unit)
{
    s32 slot;
    u16 selected;

    for (slot = 0; slot < 24; slot++) {
        if (unit->unit.keisyoskill[slot] == skill) {
            break;
        }
    }
    selected = unit->unit.keisyoskill[slot];
    rstClearSkillSlot(unit, slot);
    return rstAddSkillToSlot(selected, &unit->unit);
}

s8 cmbChkSkillOwner(u16 skill, datUnitWork_t *unit);

s8 rstAddSkillToSlot(u16 skill, datUnitWork_t *unit)
{
    s32 slot;

    if (cmbChkSkillOwner(skill, unit) >= 0) {
        return -2;
    }
    for (slot = 0; slot < 8; slot++) {
        if (unit->skill[slot] == 0) {
            unit->skill[slot] = skill;
            unit->skillcnt++;
            return slot;
        }
    }
    return -1;
}

s8 cmbChkSkillOwner(u16 skill, datUnitWork_t *unit)
{
    s32 i = 0;
    u16 *p;

    if (unit->skillcnt != 0) {
        s32 count = unit->skillcnt;

        p = unit->skill;
        do {
            if (*p == skill) {
                return i;
            }
            p++;
            i++;
        } while (i < count);
    }
    return -1;
}

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

extern void cmbInitStockUnit(u16 id, datUnitWork_t *unit);

void cmbCopyDefaultDevilToStock(u16 devilId, datUnitWork_t *dst)
{
    datDevilInfo_t *entry;
    const u8 *sourceParam;
    s8 *destParam;
    s32 i;

    memset(dst, 0, sizeof(*dst));
    dst->flag = 1;
    dst->id = devilId;
    entry = &datDevilTable[devilId];
    sourceParam = &entry->unk_06[0xA];
    destParam = dst->param;
    dst->level = entry->unk_05;
    for (i = 0; i < 6; i++) {
        *destParam++ = *sourceParam++;
    }
    dst->hp = datGetMaxHp(dst);
    dst->maxhp = dst->hp;
    dst->mp = datGetMaxMp(dst);
    dst->maxmp = dst->mp;
    cmbInitStockUnit(devilId, dst);
}

void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src)
{
    memcpy(dst, src, sizeof(*dst));
    dst->maxhp = datGetMaxHp(dst);
    dst->maxmp = datGetMaxMp(dst);
    if (dst->hp > dst->maxhp) {
        dst->hp = dst->maxhp;
    }
    if (dst->mp > dst->maxmp) {
        dst->mp = dst->maxmp;
    }
    dst->flag |= 1;
}

void cmbClearStockEntry(cmbUnit_t *unit)
{
    memset(unit, 0, 0xEC);
}

void cmbClearUnitSkills(datUnitWork_t *unit)
{
    /* The native clear covers the first twelve skill entries. */
    memset(unit->skill, 0, 12 * sizeof(unit->skill[0]));
    unit->skillcnt = 0;
}

void cmbClearSkillSlots(cmbUnit_t *unit)
{
    memset(unit->unit.keisyoskill, 0, sizeof(unit->unit.keisyoskill));
}

void cmb_1FF0C8(cmbUnit_t *list, u8 count)
{
    u8 dst = 0;
    s32 src = 0;
    cmbUnit_t *entry;

    for (; src < count; src++) {
        entry = &list[src];
        if (entry->unit.id != 0) {
            if (dst < src) {
                cmbCopyCustomDevilToStock(&list[dst].unit, &entry->unit);
                cmbClearStockEntry(entry);
            }
            dst++;
        }
    }
}

void cmbApplyEvoUnlock(s8 id)
{
    s8 dst = 0;
    s8 src = 0;

    if (*(s32 *)DDS3_GBWK->stocklist > 0) {
        do {
            DDS3_GBWK->unk1914[dst] = DDS3_GBWK->unk1914[src];
            if (DDS3_GBWK->unk1914[src] != id) {
                dst++;
            }
            src++;
        } while (src < *(s32 *)DDS3_GBWK->stocklist);
    }
    *(s32 *)DDS3_GBWK->stocklist = *(s32 *)DDS3_GBWK->stocklist - 1;
}

void cmbFindStockEnd(s8 devilId)
{
    cmbGlobalWork->stock[cmbGlobalWork->stockCount++] = devilId;
}

extern s8 cmbCheckEvoUnlocked(u16 devilId, datUnitWork_t *list, u8 count);

extern void cmbApplyEvoUnlock(s8 index);

s32 cmbStoreBirthDevil(u16 devilId, cmbUnit_t *list, u8 count)
{
    s8 index = cmbCheckEvoUnlocked(devilId, (datUnitWork_t *)list, count);

    if (index < 0) {
        return 0;
    }
    cmbApplyEvoUnlock(index - count + 0x10);
    cmbClearStockEntry(&list[index]);
    return -1;
}


s32 cmbAddDevilToStock(u16 devilId, cmbUnit_t *list, u8 count)
{
    s8 slot = cmbFindStockSlot(list, count);

    if (slot < 0) {
        return -1;
    }
    cmbFindStockEnd((s8)(slot - count + 0x10));
    cmbCopyDefaultDevilToStock(devilId, &list[slot].unit);
    return slot;
}

s32 cmbAddCustomDevilToStock(datUnitWork_t *custom, cmbUnit_t *list, u8 count)
{
    s8 slot = cmbFindStockSlot(list, count);

    if (slot < 0) {
        return -1;
    }
    cmbFindStockEnd((s8)(slot - count + 0x10));
    cmbCopyCustomDevilToStock(&list[slot].unit, custom);
    return slot;
}

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

u16 cmbPickEvoResult(datUnitWork_t *unit)
{
    s32 cumulative = 0;
    s32 totalWeight = cmbApplyEvoResult(unit);
    s32 pick;
    s32 i;
    u16 *skill;

    if (totalWeight == 0) {
        return 0;
    }
    i = 0;
    pick = dds3GetRandA(0) % (u32)totalWeight;
    skill = unit->keisyoskill;
    for (; i < 24; i++) {
        cumulative += cmbCopyEvoDevil(unit, *skill);
        if (pick < cumulative) {
            return *skill;
        }
        skill++;
    }
    return 0;
}

void cmbCalcInheritCount(cmbUnit_t *unitA, cmbUnit_t *unitB, cmbUnit_t *unitC,
                         cmbUnit_t *result, cmpSkillList_t *out)
{
    s8 count = (s8)unitA->unit.skillcnt;
    f32 average;
    u16 skill;

    count += unitB->unit.skillcnt;
    if (unitC != NULL) {
        count = (s8)(count + (u8)unitC->unit.skillcnt);
        average = (count + 0x1.599998p+1f) / 0x1.d99998p+1f;
    } else {
        average = (count + 0x1.599998p+1f) / 0x1.d99998p+1f;
    }
    count = (s8)(s32)average;

    if (out != NULL) {
        out->count = 0;
    }
    if (cmbFindSkillSlot(result) == 0) {
        return;
    }
    if (result->unit.skillcnt + count >= 9) {
        count = 8 - (u8)result->unit.skillcnt;
    }
    while (count > 0) {
        skill = cmbPickEvoResult(&result->unit);
        if (skill == 0) {
            count--;
            continue;
        }
        if (cmbReplaceSkill(skill, result) < 0) {
            continue;
        }
        if (out != NULL) {
            out->skill[(s8)out->count++] = skill;
        }
        count--;
    }
}

s32 cmbGetSpecialItemId(void)
{
    return 0x8D;
}

extern u8 D_0032DDD0[5][2];
extern u8 D_0032DDE0[];
extern u16 D_0032DDE8[4][5];
extern s32 datCheckItem(s32 id, s32 count);

s32 cmbGetMoonBonus(u16 devilId)
{
    s32 moon = evtGetAgeOfMoon();
    s32 moonIndex = -1;
    s32 raceIndex = -1;
    u8 race = datDevilTable[devilId].race;
    u32 i;
    u16 itemId;

    for (i = 0; i < 5; i++) {
        if (moon >= D_0032DDD0[i][0] && moon <= D_0032DDD0[i][1]) {
            moonIndex = i;
        }
    }

    {
        u32 j;
        for (j = 0; j < 4; j++) {
            if (D_0032DDE0[j] == race) {
                raceIndex = j;
            }
        }
    }

    if (datCheckItem(0x8D, 1) != 0) {
        if (moonIndex < 0 || raceIndex < 0) {
            return 0;
        }
        itemId = D_0032DDE8[raceIndex][moonIndex];
        if (cmbDevilIsUnlocked(itemId) != 0) {
            return 0;
        }
        return itemId;
    }
    return 0;
}

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetBirthDevil);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawUnitLevel);

extern fclSpriteTbl_t cmbSpriteTable[];
extern void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 OtNo);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CB30);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CB48);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CB60);

void cmbDrawStockIcon(s32 x, s32 y, s32 z, s8 icon, u32 ot)
{
    s32 spriteId = icon < 0 ? 0 : 1;

    switch (icon) {
    case 0:
        spriteId = 2;
        break;
    case 5:
        spriteId = 3;
        break;
    case 6:
        spriteId = 4;
        break;
    case 2:
        spriteId = 5;
        break;
    case 12:
        spriteId = 6;
        break;
    default:
        break;
    }
    fclDrawSpriteRes(x, y, z * 16, 0, 1, spriteId, CMB_GBWK->texHandle, cmbSpriteTable, ot);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawStockEntry);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawStockBox);

extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern fclSpriteTbl_t cmbSpriteTable[];
extern void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 OtNo);

void cmbDrawStockFrame(s32 unusedX, s32 unusedY, s32 z, s32 ot)
{
    s32 i;
    s32 x;

    cmpDrawBoxF(0x1020, 0x388, z, 0xF1, 9, 0x33, ot);
    x = 0x1060;
    for (i = 0; i < 12; i++) {
        s32 offset = (i < 9) ? 0 : -0x30;
        fclDrawSpriteRes(x + offset, 0x370, z << 4, 0, 2, (u16)i,
                         CMB_GBWK->texHandle, cmbSpriteTable, ot);
        x += 0x140;
    }
}

void cmbDrawStockText(s32 unused0, s32 unused1, s32 z, s32 ot)
{
    cmpDrawBoxF(0x1100, 0x388, z, 0xE3, 9, 0x341F1F6C, ot);
    fclDrawSpriteRes(0, 0, z << 4, 0, 0, 4, CMB_GBWK->texHandle,
                     cmbSpriteTable, ot);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawStockBg);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawListPanel);

extern void cmbDrawListPanel(s32 x, s32 y, s32 z, s32 ot);
extern void cmbDrawStockBox(s32 x, s32 y, s32 z, s32 color, s8 count, s32 ot);
extern void cmbDrawStockBg(s32 x, s32 y, s32 z, u8 mode, s32 ot);

void cmbDrawStockList(s32 x, s32 y, s32 z, s32 color, s8 count, u8 mode, s32 ot)
{
    cmbDrawListPanel(0, 0x320, z, ot);
    cmbDrawStockBox(x, y, z, color, count, ot);
    if (mode == 0) {
        cmbDrawStockFrame(x + 0xEA0, y, z, ot);
    } else {
        cmbDrawStockText(x + 0xEA0, y, z, ot);
    }
    cmbDrawStockBg(x + 0x120, y + 0x70, z, mode, ot);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawDevilStats);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawDevilNameLevel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawResultIcon);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawResultHeader);

s32 cmbDrawPhaseDefault(s32 active)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (active != 0) {
        mode = 0;
        if (fclGetFlag(0) == 0) {
            mode = 2;
            if (*(s8 *)((u8 *)CMB_GBWK + 3) == 0x10) {
                mode = 1;
            }
        }
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0x64, 0, 0, (fclPanel_t *)&pos);
    cmpUpdateListSelection(&pos, (u8 *)CMB_GBWK + 0x1C, 0, mode, 0x100, 0x47);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawInfoSprite);

/* Placement arguments are retained for callers; the header uses fixed coordinates. */
void cmbDrawTopSprite(s32 unusedX, s32 unusedY, u32 unusedZ, s32 unusedOt)
{
    fclDrawSpriteRes(0, 0, 0xFFFFF0, 0, 0, 0x23, CMB_GBWK->texHandle, cmbSpriteTable, 0x47);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawTopText);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawStockRow);

/* Same native row callback contract as cmpDrawListRowBox. */
typedef void (*cmbDrawRow_t)(s32, s32, s32, u16, u16, u16,
    s32, s32, s32, s8, s32, s32);
extern void cmbDrawStockRow(s32, s32, s32, u16, u16, u16,
    s32, s32, s32, s8, s32, s32);
extern void cmpDrawScrollBar(s32 x, s32 y, s32 z, s32 width, s32 height,
    s32 color, s32 scale, s8 phase, cmpCursorInfo_t *cursor, cmbDrawRow_t draw, s32 ot);

void cmbDrawStockScrollBar(s8 mode, s32 z, s32 unused, cmpCursorInfo_t *cursor, s32 ot)
{
    s32 x = cursor->CursorSize << 4;
    s32 y = cursor->CursorPos << 3;
    u16 width = cursor->unk_10;
    u16 height = cursor->unk_14;

    if (mode == 0) {
        cmpDrawScrollBar(x, y, z,
            width, height, 0x80808080, 0x100, 0,
            cursor, cmbDrawStockRow, ot);
    } else {
        cmpDrawScrollBar(x, y, z,
            width, height, 0x80808080, 0x100, 0,
            cursor, NULL, ot);
    }
}

/* NON_MATCHING: 25 of 29 words match, exact size; the `color & ~0xFF` value
 * takes $3 where retail uses $2. */
#ifdef NON_MATCHING
void cmbDrawResultFooter(s32 color)
{
    s32 corner[4];

    fclDrawPanelSprite2(0x48, 0x47);
    corner[0] = color;
    corner[1] = color & ~0xFF;
    corner[2] = color;
    corner[3] = color & ~0xFF;
    cmpDrawBoxGDefault(0, 0x148, 0xFFFFF, 0xC8, 0x3A, corner, 0x47);
    fclDrawPanelSprite2(0x44, 0x47);
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawResultFooter);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawResultPanel);

void cmbDrawStockPanel1(void)
{
    s8 count = *((u8 *)CMB_GBWK + 0x107A) + *((u8 *)CMB_GBWK + 0x107C);

    cmbDrawStockList(0x180, 0x3F0, 0xFFFF, 0, count, 0, 0x47);
    cmbDrawStockEntry(0x300, 0x408, 0xFFFFF, count, 0x89FEFF80,
                      (u8 *)CMB_GBWK + 0x1058, 0, 0x47);
    if (*(s8 *)((u8 *)CMB_GBWK + 0x43E4) >= *(s8 *)((u8 *)CMB_GBWK + 0x165ED)) {
        cmbDrawStockScrollBar(0, 0xFFFFF, 0x50807080, &CMB_GBWK->stockCursorGroup[0].cursor,
                              0x47);
    }
}

extern void cmbDrawStockEntry(s32 x, s32 y, s32 color, s8 count, s32 color2,
                              u8 *rows, s32 mode, s32 flags);

void cmbDrawStockPanel2(void)
{
    s8 count = *((u8 *)CMB_GBWK + 0x20b6) + *((u8 *)CMB_GBWK + 0x20b8);

    cmbDrawStockList(0x180, 0x3F0, 0xFFFF, 0, count, 1, 0x47);
    cmbDrawStockEntry(0x300, 0x408, 0xFFFFF, count, 0x89FEFF80,
                      (u8 *)CMB_GBWK + 0x2094, 1, 0x47);
    cmbDrawStockScrollBar(1, 0xFFFFF, 0x50807080, &CMB_GBWK->stockCursorGroup[1].cursor,
                          0x47);
}

void cmbDrawStockPanel3(void)
{
    s8 count = *((u8 *)CMB_GBWK + 0x30f2) + *((u8 *)CMB_GBWK + 0x30f4);

    cmbDrawStockList(0x180, 0x3F0, 0xFFFF, 0, count, 1, 0x47);
    cmbDrawStockEntry(0x300, 0x408, 0xFFFFF, count, 0x89FEFF80,
                      (u8 *)CMB_GBWK + 0x30d0, 2, 0x47);
    cmbDrawStockScrollBar(1, 0xFFFFF, 0x50807080, &CMB_GBWK->stockCursorGroup[2].cursor,
                          0x47);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawStatus);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawBirthDevilMaskBG);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawBirthDevil);

INCLUDE_ASM(const s32, "facility/fclCombineInit", func_00201FB0);

s32 cmbDrawPhase3(s32 active)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (active != 0) {
        mode = 0;
        if (fclGetFlag(0) == 0) {
            mode = 2;
            if (*(s8 *)((u8 *)CMB_GBWK + 3) == 0x10) {
                mode = 1;
            }
        }
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0x64, 0, 0, (fclPanel_t *)&pos);
    cmpUpdateListSelection(&pos, &CMB_GBWK->menuCursor, 0, mode, 0x100, 0x47);
}

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawSequence);

void cmbExit(void)
{
    cmpMoveCursorListEnd(&CMB_GBWK->combineCursor);
    CMB_GBWK->unk_00005 = 1;
    CMB_GBWK->unk_00008 = fclScriptStart(CMB_GBWK->script, 0xB, &CMB_GBWK->scriptArg);
    CMB_GBWK->unk_00001 = 4;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateRoot);

s32 cmbStockIsLocked(s32 index)
{
    s32 flag = cmbGlobalWork->party[index].unit.flag & 0x800;

    return flag != 0;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateStockCursor);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbChangeSequence);

extern void fclRbnRelease(fclRibbon_t *phandle);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpLoadStatusModel(void);

extern void cmpPlayCursorSE(u32 button);

void cmbWaitCancelButton(void)
{
    s32 button = cmpGetPressedButton(2) & 0xFFFF;

    if (button == 2) {
        if (cmpLoadStatusModel() == 0) {
            fclRbnRelease(CMB_GBWK->ribbon);
            CMB_GBWK->ribbon = NULL;
            CMB_GBWK->unk_00001 = CMB_GBWK->unk_00003;
            CMB_GBWK->unk_00004 = button;
        } else {
            button = 0x8000;
        }
    }
    /* The native caller discards this sound helper's incidental return value. */
    cmpPlayCursorSE(button);
}

extern void fclRbnSwitchActive(fclRibbon_t *phandle, s32 index);

void cmbSwitchRibbon(void)
{
    s32 index = CMB_GBWK->unk_1650A;

    if (CMB_GBWK->stockSlot[2].index == -1) {
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
    if (CMB_GBWK->stockSlot[2].index != -1) {
        return 0;
    }
    return evtBitCheck(0x25) != 0;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateBirthDevil);

extern void cmbAddBirthDevilToStock(datUnitWork_t *unit, s32 count);

void cmbSetAction(void)
{
    CMB_GBWK->stockSlot[0].id =
        cmbGlobalWork->party[CMB_GBWK->stockSlot[0].index].unit.id;
    CMB_GBWK->stockSlot[1].id =
        cmbGlobalWork->party[CMB_GBWK->stockSlot[1].index].unit.id;
    CMB_GBWK->stockSlot[2].id =
        cmbGlobalWork->party[CMB_GBWK->stockSlot[2].index].unit.id;
    cmbAddBirthDevilToStock(&cmbGlobalWork->party[4].unit, 0xC);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateDecideDevil);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateSacConfirm);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateStockCursor2);

void cmbSeqAfterBirth(void)
{
    s32 info = cmpChkSeqInfo(CMB_GBWK);

    if (info == 1) {
        return;
    }
    if (info != 2) {
        return;
    }
    if (CMB_GBWK->stockSlot[2].index == -1) {
        CMB_GBWK->unk_00001 = 6;
    } else {
        CMB_GBWK->unk_00001 = 8;
    }
    CMB_GBWK->unk_00004 = 2;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateMenuCursor);

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

extern void cmbFreeActionModel(void *model);

void cmbFreeIntroduceModels(void)
{
    u32 i;

    for (i = 0; i < 5; i++) {
        cmbFreeActionModel((u8 *)CMB_GBWK + 0x16524 + i * 0x14);
    }
}

extern void effEVECombParSmokeDelete(s32 pCombParSmoke);

void cmbFreeSmokeEffect(void)
{
    effEVECombParSmokeDelete(CMB_GBWK->smoke[0]);
    effEVECombParSmokeDelete(CMB_GBWK->smoke[1]);
    effEVECombParSmokeDelete(CMB_GBWK->smoke[2]);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateIntroduce);

extern void fclEncycStartProcess(s32 mode);

extern void fclEncycSetTexHandle(s32 *texhandle);

void cmbStartEncycView(s32 mode)
{
    fclEncycStartProcess(mode);
    fclEncycSetTexHandle(CMB_GBWK->texHandle);
    fclEncycSetExplainData(CMB_GBWK->explainData);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateResultCursor);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbHandleDebugExit);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdatePanelAnim);

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateSequence);

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

extern void mdlGetFilebase(s32 aMajor, u16 aMinor, char *aName, s32 aNameLeng);

extern s32 sprintf(char *out, const char *fmt, ...);

extern char D_0053CD68[];

/* Called above its definition, where ee-gcc assumed int. */
void *sdfDevLoadFileEx(char *name, void **data, s32 *size);

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

extern void *sdfMemLock(sdfMemHandle_t *handle);

void cmbLoadActionModel(s32 id, cmbActionData_t *data)
{
    s32 handle;

    cmbInitActionData(id, data);
    handle = cmbMakeModelPath(id, &data->size);
    data->handle = handle;
    data->lock = sdfMemLock((sdfMemHandle_t *)handle);
    printf(D_0053CD80, (u16)data->id);
}

extern void mdlFree(struct mdlObj_t *model);
extern char D_0053CDA8[];
extern char D_0053CDC8[];

void cmbFreeActionModel(void *model)
{
    cmbActionData_t *data = model;

    if (data->handle != 0) {
        sdfMemFreeDirect((void *)data->handle);
        data->handle = 0;
        printf(D_0053CDA8, (u16)data->id);
    } else if (data->model != NULL) {
        mdlFree(data->model);
        data->model = NULL;
        while (sdfDelayedResrcCheck() != 0) {
        }
        printf(D_0053CDC8, (u16)data->id);
    }
}

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD28);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD38);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD48);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD58);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD68);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CD80);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CDA8);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CDC8);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CDE8);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CE10);

extern struct mdlObj_t *mdlLoad(u32 major, u32 minor);
extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
extern char D_0053CDE8[];
extern char D_0053CE10[];

struct mdlObj_t *cmbGetActionModelHandle(cmbActionData_t *data)
{
    struct mdlObj_t *model = data->model;

    if (data->handle == 0) {
        if (model == NULL) {
            data->model = mdlLoad(6, (u16)data->id);
            printf(D_0053CDE8, (u16)data->id);
            model = data->model;
        }
    } else {
        if (model != NULL) {
            sdfMemFreeDirect((void *)data->handle);
            data->handle = 0;
        } else {
            mdlPackDecode(6, (u16)data->id, 0x101, (const u8 *)data->lock,
                          data->size);
            data->model = mdlLoad(6, (u16)data->id);
            sdfMemFreeDirect((void *)data->handle);
            data->handle = 0;
            printf(D_0053CE10, (u16)data->id);
        }
        model = data->model;
    }
    return model;
}

extern void mdlLoopMotion(void *aHandle, s32 aGroup, s32 aNumber);

void *cmbUpdateIntroduceModel(cmbActionData_t *work)
{
    struct mdlObj_t *model;

    cmbGetActionModelHandle(work);
    model = work->model;
    if (model != NULL) {
        mdlLoopMotion(model, 0, 0xD);
        model = work->model;
        *(s32 *)model &= ~1;
    }
    return model;
}

s32 func_002043E0(cmbActionData_t *work)
{
    mdlObj_t *model = work->model;

    if (model == NULL) {
        return 0;
    }
    model->flags |= 1;
    return 0;
}

typedef struct
{
    f32 v[4];
} cmbPanelScale_t;

extern cmbPanelScale_t D_0053CE38;

f32 cmbGetPanelScale(s32 mode)
{
    cmbPanelScale_t scale;

    scale = D_0053CE38;
    return scale.v[mode & 3];
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbRotateModelPanel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawModelPoly);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawModelAt);

extern void *dds3GetControlScene(void);

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

extern void *dds3PointBasicGetResource(void *pPointBasic);

/* NON_MATCHING: 9 of 25 words differ (first at +0x20). */
#ifdef NON_MATCHING
s32 cmbFindCameraPoint(char *name, u128 *out)
{
    s32 found = (s32)dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 0x11, name);
    void *basic = (void *)found;

    if (basic != NULL) {
        found = 1;
        basic = dds3PointBasicGetResource(basic);
        SDF_VU0_COPYVECTOR(out, basic);
    }
    return found;
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbFindCameraPoint);
#endif

extern datUnitVisual_t datModelParamTable[];
extern void mdlSetPosition(struct mdlObj_t *model);
extern void mdlSetScale(struct mdlObj_t *model);
extern void mdlLink(struct mdlObj_t *model, void *ot);

void cmbDrawActionModel(struct mdlObj_t *model, u16 id, f32 *position, f32 *rotation, void *ot)
{
    /* VU0 consumes these stores through the bare-address vector load. */
    volatile f32 scale[4] __attribute__((aligned(16)));
    f32 value;
    datUnitVisual_t *params;

    if (model != NULL) {
        SDF_VU0_LOADVECTOR(position);
        mdlSetPosition(model);
        params = &datModelParamTable[id];
        scale[3] = 1.0f;
        value = params->formscale;
        scale[2] = value;
        scale[0] = value;
        scale[1] = value;
        SDF_VU0_LOADVECTOR(scale);
        mdlSetScale(model);
        mdlLink(model, ot);
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbGetModelMarkPos);

/* NON_MATCHING: 20 of 26 words match, exact size; the two `fclDrawPanelSprite`
 * calls put the `ori` of the sprite id in the delay slot where retail puts the
 * flags copy. */
#ifdef NON_MATCHING
void cmbDrawFullScreenPanel(s32 flags)
{
    cmpDisableRGB(flags);
    fclDrawPanelSprite(0x34000, flags);
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0, flags);
    cmpAllocRefGifTag(flags);
    fclDrawPanelSprite(0x34000, flags);
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawFullScreenPanel);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawModelPanel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbDrawTargetModel);

typedef struct
{
    u8 reserved00[0x80];
    u32 modelValue;
} cmbSceneModel_t;

typedef struct
{
    u8 reserved00[0x18];
    cmbSceneModel_t *model;
} cmbSceneModelOwner_t;

extern dds3Basic_t *dds3GetActiveScene(void);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type,
                                                    const char *name);
extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

s32 cmbGetSceneModel(cmbSceneModelOwner_t *work, const char *name)
{
    dds3Basic_t *scene = dds3GetActiveScene();
    dds3Basic_t *basic;
    evtUnit_t *unit;

    if (scene == NULL) {
        return 0;
    }
    basic = dds3SceneBasicSearchNameAndType(scene, 5, name);
    if (basic == NULL) {
        return 0;
    }
    unit = evtGetUnitHandle2(basic);
    if (unit == NULL) {
        return 0;
    }
    if (unit->value == 0) {
        return 0;
    }
    work->model->modelValue = unit->value;
    return 1;
}

extern void sdfModelOtInit(void *aModelOt, sdfGrOt_t *aOt0, sdfGrOt_t *aOt1, sdfGrOt_t *aOt2,
                           sdfGrOt_t *aOt3);

void cmbInitModelOt(s32 unused, void *table)
{
    sdfGrOt_t *ot = &sdfGrOtTable[23];
    s32 dummy;
    s32 *p = &dummy;

    sdfModelOtInit(table, ot, &ot[1], &ot[21], &ot[22]);
}

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CE38);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbLoadTargetModel);

extern void *dds3SearchSceneBasic(void *pBasic, u32 serialID, s32 type);

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

/* Called above its definition, where ee-gcc assumed int. */
dds3Basic_t *dds3GetActiveScene(void);

s32 cmbFindUnitScene(void *work)
{
    dds3Basic_t *scene = dds3GetActiveScene();
    dds3Basic_t *basic;
    evtUnit_t *unit;

    if (scene == NULL) {
        return 0;
    }
    basic = dds3SearchSceneBasic(scene, DDS3_GBWK->integers[15], 5);
    if (basic == NULL) {
        return 0;
    }
    unit = evtGetUnitHandle2(basic);
    if (unit == NULL) {
        return 0;
    }
    if (unit->value == 0) {
        return 0;
    }
    *(u32 *)(*(u8 **)((u8 *)work + 0x18) + 0x80) = unit->value;
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateCameraPos);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateCameraZoom);

datUnitWork_t *cmbGetBirthDevilPanel(void)
{
    s32 sel = CMB_GBWK->unk_1650A;
    datUnitWork_t *panel = (datUnitWork_t *)CMB_GBWK->birthPanel;

    if (sel != 3) {
        panel = &cmbGlobalWork->party[*(s8 *)((u8 *)CMB_GBWK + sel * 4 + 0x4110)];
    }
    return panel;
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateTimeTable);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbStartSmokeEffect);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbSetActionWorkValues);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbRunActionSe);

extern void fclCombineBGM(s8 Mode);

/* NON_MATCHING: 3 of 25 words differ (first at +0x54). */
#ifdef NON_MATCHING
s32 cmbActionBGM(s32 mode)
{
    if (mode == 0) {
        p2sdStopBGM(0x380000);
        fclCombineBGM(0);
        return printf("ACT BGM END ...\n");
    }
    fclCombineBGM(mode);
    p2sdPlayBGM(0x380000);
    printf("ACT BGM START!!!\n");
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbActionBGM);
#endif

void func_00205AB8(void) {
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbInitAction);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmb_205CE8);

void func_00205DB8(void)
{
    if (CMB_GBWK->stockSlot[2].index < 0) {
        return;
    }
    func_002043E0(&CMB_GBWK->actionModel);
}

extern void effEVECombParSmokeSetZtest(s32 pCombParSmoke, s32 test);

void cmb_205DF0(void)
{
    effEVECombParSmokeSetZtest(CMB_GBWK->smoke[0], 1);
    effEVECombParSmokeSetZtest(CMB_GBWK->smoke[1], 1);
    func_002043E0((cmbActionData_t *)((u8 *)CMB_GBWK + 0x16524));
    func_002043E0((cmbActionData_t *)((u8 *)CMB_GBWK + 0x16538));
    func_00205DB8();
}

void cmb_205E68(void)
{
    mdlSmoothLoopMotion(CMB_GBWK->actionModel.model, 0, 1, 0.0f, 1.0f);
}

extern f32 cmbTargetModelId;

void func_00205EA0(void)
{
    sdfMotionCtrlr_t *motion = CMB_GBWK->actionModel.model->first;

    if ((f32)datModelParamTable[(u16)CMB_GBWK->actionModel.id].badframe - 1.0f <= motion->frame) {
        motion->speed = 0.0f;
    }
    /* Native ramp: one float below 25, with the rounded 25/3 step. */
    if (cmbTargetModelId < 0x1.8ffffep+4f) {
        cmbTargetModelId += 0x1.0aaaaap+3f;
    }
}

extern void cmbFreeActionModel(void *model);

void cmbFreeActionModels(void)
{
    cmbFreeActionModel((u8 *)CMB_GBWK + 0x16524);
    cmbFreeActionModel((u8 *)CMB_GBWK + 0x16538);
    if ((s8)CMB_GBWK->stockSlot[2].index >= 0)
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

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateAction);

typedef struct
{
    s16 h[18];
} cmbCopy36_t;

typedef struct
{
    s16 h[16];
} cmbCopy32_t;

typedef struct
{
    s16 h[288];
} cmbCopy576_t;

/* NON_MATCHING: 35 of 176 words differ, two registers exchanged and two
 * instructions scheduled elsewhere. */
#ifdef NON_MATCHING
void func_00206308(u8 *dst, u8 *src)
{
    *(s32 *)(dst + 0x00) = *(s32 *)(src + 0x10);
    *(u16 *)(dst + 0x04) = *(u16 *)(src + 0x14);
    *(cmbCopy36_t *)(dst + 0x06) = *(cmbCopy36_t *)(src + 0x16);
    *(cmbCopy32_t *)(dst + 0x0C) = *(cmbCopy32_t *)(src + 0x1C);
    *(u16 *)(dst + 0x12) = *(u16 *)(src + 0x38);
    *(s32 *)(dst + 0x2C) = *(s32 *)(src + 0x3C);
    *(cmbCopy576_t *)(dst + 0x14) = *(cmbCopy576_t *)(src + 0x3A);
    *(cmbCopy36_t *)(dst + 0x44) = *(cmbCopy36_t *)(src + 0x7A);
    *(cmbCopy36_t *)(dst + 0x4A) = *(cmbCopy36_t *)(src + 0x80);
    *(cmbCopy576_t *)(dst + 0x50) = *(cmbCopy576_t *)(src + 0x86);
    *(u16 *)(dst + 0x80) = *(u16 *)(src + 0xCE);
    *(s32 *)(dst + 0x84) = *(s32 *)(src + 0xD8);
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", func_00206308);
#endif

/* NON_MATCHING: 35 of 176 words differ, the same residual as func_00206308. */
#ifdef NON_MATCHING
void func_002065C8(u8 *dst, u8 *src)
{
    *(s32 *)(dst + 0x10) = *(s32 *)(src + 0x00);
    *(u16 *)(dst + 0x14) = *(u16 *)(src + 0x04);
    *(cmbCopy36_t *)(dst + 0x16) = *(cmbCopy36_t *)(src + 0x06);
    *(cmbCopy32_t *)(dst + 0x1C) = *(cmbCopy32_t *)(src + 0x0C);
    *(u16 *)(dst + 0x38) = *(u16 *)(src + 0x12);
    *(s32 *)(dst + 0x3C) = *(s32 *)(src + 0x2C);
    *(cmbCopy576_t *)(dst + 0x3A) = *(cmbCopy576_t *)(src + 0x14);
    *(cmbCopy36_t *)(dst + 0x7A) = *(cmbCopy36_t *)(src + 0x44);
    *(cmbCopy36_t *)(dst + 0x80) = *(cmbCopy36_t *)(src + 0x4A);
    *(cmbCopy576_t *)(dst + 0x86) = *(cmbCopy576_t *)(src + 0x50);
    *(u16 *)(dst + 0xCE) = *(u16 *)(src + 0x80);
    *(s32 *)(dst + 0xD8) = *(s32 *)(src + 0x84);
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", func_002065C8);
#endif

void func_00206888(s32 a0, s32 a1, s32 a2, s8 cursorCount, s32 a4, cmbListWork_t *work)
{
    lstInitListWindow(0x100, 0x2E0, 0xFFFFF, 0x109, 0xCA, 0x30303060, 0, work);
    cmpInitCursorLayout(&work->listCursor, 0x1A, 0xC, 0xC4, 0x16, 0x16);
    cmpInitCursorRange(&work->listCursor, cursorCount, 8, 0);
}

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", shop_206948);

void func_00206A28(void) {
}

void func_00206A30(void) {
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", shop_206A38);

s32 shopStartEvent(s8 value)
{
    fclSetWorkValue(1, value);
    fclClearFlag(1);
    return evtStartEvent(0x322);
}

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CF10);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CF20);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CF30);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CF50);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", gProcessStat_0);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAD5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAD8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAE0);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAE8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAF0);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FAF8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FB00);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", cmpSkillCursor);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FB0C);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FB10);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FB18);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", cmbActionMode);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", cmbActionStep);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", D_0054FB1D);

