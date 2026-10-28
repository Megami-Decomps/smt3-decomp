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
    u8 baseLevel;
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

extern u8 D_0032DBD8[][4];

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

extern s32 p2sdStopBGM(s32 id);

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

extern void p2sdPlayBGM(s32 MidiNo);

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
    /* 0x04746 */ u8 reserved04746[0x11DC2];
    /* 0x16508 */ u8 displayFlags[2];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B;
    /* 0x1650C */ void *resourceHandle_1650C;
    /* 0x16510 */ s32 script;
    /* 0x16514 */ u8 *scriptArg;
    /* 0x16518 */ void *resourceHandle_16518;
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[4];
    /* 0x16524 */ cmbActionData_t actionData[5];
    /* 0x16588 */ u8 reserved16588[8];
    /* 0x16590 */ f32 modelMark[3][4];
    /* 0x165C0 */ f32 cameraPosition[4];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ s32 actionTargetIndex;
    /* 0x165E0 */ s8 actionPhase;
    u8 reserved165E1[3];
    /* 0x165E4 */ s32 actionFrame;
    /* 0x165E8 */ f32 actionZoom;
    /* 0x165EC */ s8 introductionFlag;
    /* 0x165ED */ u8 stockCursorCount;
    /* 0x165EE */ u8 unk_165EE[0x12];
    /* 0x16600 */ cmpCursorInfo_t resultCursor;
    /* 0x16638 */ u8 reserved16638[0x1008];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 reserved17678[0x1004];
    /* 0x1867C */ cmbListWork_t eligibleList;
    /* 0x18810 */ u8 reserved18810[0x1004];
    /* 0x19814 */ datUnitWork_t eligibleStock[16];
    /* 0x1A6D4 */ s32 eligibleCount;
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ fclSprite_t backgroundSprite;
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ fclPanel_t *panel;
    /* 0x1A720 */ fclRibbon_t *ribbon;
    /* 0x1A724 */ u8 resultPanelAnim[0x24];
    /* 0x1A748 */ u8 sacrificePanelAnim[0x24];
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

extern char *D_0032CB80[];
extern s32 fclLoadTexture(char *filename);
extern void fclSetPanelTex(fclPanel_t *panel, s32 u0, s32 v0, s32 u1, s32 v1, s32 texture);

void fclCombineLoadTextures(void)
{
    s8 i;

    for (i = 0; (u8)i < 7; i++) {
        CMB_GBWK->texHandle[i] = fclLoadTexture(D_0032CB80[i]);
    }
    fclSetPanelTex(CMB_GBWK->panel, 0, 0, 0x100, 0x80, CMB_GBWK->texHandle[5]);
    fclSetSprite(&CMB_GBWK->backgroundSprite, 0x200, 0x1C0, 0, 5, 5,
                 0x1FB, 0xFB, 0x80808080, CMB_GBWK->texHandle[6]);
    printf("COMBINE : Tex load.\n");
}

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

extern void p2sdPlayBGMFade(s32 id);

void fclCombineBGM(s8 mode)
{
    s32 generalStat = cmbChkGeneralStat();
    s8 previous = (s8)CMB_GBWK->unk_0410C[1];
    s32 bgm;

    switch (previous) {
    case 1:
        bgm = 0x360000;
        break;
    case 2:
        bgm = 0x370000;
        break;
    default:
        if (generalStat == 1) {
            bgm = 0x330000;
        } else {
            bgm = generalStat == 0 ? 0x350000 : 0x320000;
        }
        break;
    }

    switch (mode) {
    case 0:
        p2sdPlayBGM(bgm);
        break;
    case 1:
        p2sdStopBGM(bgm);
        break;
    case 2:
        p2sdPlayBGMFade(bgm);
        /* Qualified cc1probe: selector reset uid146 survives sibling-call
         * selection and is deleted in life analysis, preserving retail jal. */
        mode = 0;
        break;
    }
}

void fclCombineInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x1A724);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x30, fclRagTexCalcSizes);
        tex++;
    }
}

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void *sdfDevLoadFileEx(char *name, void **data, s32 *size);
extern void cmpInitSeqInfo(void *work, void *parent);
extern void fclInitCursorWindow(void *work, void *data, u8 count);
extern void fclSetCursorList(void *work, s32 mode);
extern void evtDisableMoonCalc(void);
extern void cmpModelInit(void);
extern fclPanel_t *fclCreatePanel(s32 x, s32 y, s32 z, s32 w, s32 h, s32 color, s8 type, s8 speed);
extern char D_0053C890[];
extern char D_0053C8B8[];
extern u8 D_0032D1A8[];
extern u8 D_0032D1B8[];
extern u8 D_0032D1C8[];

void fclUpdateInit(void)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(0x1A770);
    void *locked = sdfMemLock(allocation);

    CMB_GBWK = locked;
    memset(CMB_GBWK, 0, 0x1A770);
    CMB_GBWK->resourceHandle = allocation;
    cmpInitSeqInfo(CMB_GBWK, NULL);

    CMB_GBWK->resourceHandle_1650C =
        sdfDevLoadFileEx(D_0053C890, (void **)&CMB_GBWK->script, NULL);
    CMB_GBWK->resourceHandle_16518 =
        sdfDevLoadFileEx(D_0053C8B8, &CMB_GBWK->explainData, NULL);
    CMB_GBWK->unk_00001 = 0;
    CMB_GBWK->unk_00003 = 0;
    CMB_GBWK->unk_00004 = 1;

    fclInitCursorWindow((u8 *)CMB_GBWK + 0x18, D_0032D1A8, 4);
    fclSetCursorList((u8 *)CMB_GBWK + 0x18, 0x19);
    fclInitCursorWindow((u8 *)CMB_GBWK + 0x165FC, D_0032D1B8, 3);
    fclSetCursorList((u8 *)CMB_GBWK + 0x165FC, 0x19);
    fclInitCursorWindow((u8 *)CMB_GBWK + 0x1763C, D_0032D1C8, 8);
    fclSetCursorList((u8 *)CMB_GBWK + 0x1763C, 0x19);

    memset(D_00551F18, 0, 4);
    memset(cmbMenuEntryTable, 0, 8);
    CMB_GBWK->displayFlags[0] = 0;
    CMB_GBWK->displayFlags[1] = 0;
    CMB_GBWK->unk_1650B = 0;
    evtDisableMoonCalc();
    cmpModelInit();
    cmpResetDrawEnv();
    fclCombineSetTable();

    CMB_GBWK->panel = fclCreatePanel(0, 0x320, 0, 0, 0x13A, 0, 0, 0x1D);
    fclCombineLoadTextures();
    fclCombineInitRagTex((u8 *)CMB_GBWK);
    CMB_GBWK->unk_0410C[1] = fclCombineCheckMoonPhase();
    fclCombineBGM(0);
    fclSetWorkValue(2, 0);
    if (evtBitCheck(0x915) == 0) {
        CMB_GBWK->unk_0410C[0] = 1;
    }
    fclCombineEventStart(1);
}

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

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053C890);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053C8B8);

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

extern s32 cmbAddCustomDevilToStock(datUnitWork_t *custom, cmbUnit_t *list, u8 count);
extern s32 cmbGetSpecialItemId(void);
extern s32 fclEncycRegist(datUnitWork_t *unit, s32 mode);

void cmbAddBirthDevilToStock(datUnitWork_t *units, u8 count)
{
    datUnitWork_t *first = &cmbGlobalWork->party[CMB_GBWK->stockSlot[0].index].unit;
    datUnitWork_t *second = &cmbGlobalWork->party[CMB_GBWK->stockSlot[1].index].unit;
    datUnitWork_t *third = NULL;

    if (CMB_GBWK->stockSlot[2].index >= 0) {
        third = &cmbGlobalWork->party[CMB_GBWK->stockSlot[2].index].unit;
    }
    cmbStoreBirthDevil(first->id, (cmbUnit_t *)units, count);
    cmbStoreBirthDevil(second->id, (cmbUnit_t *)units, count);
    if (third != NULL) {
        cmbStoreBirthDevil(third->id, (cmbUnit_t *)units, count);
    }
    cmbCopyCustomDevilToStock((datUnitWork_t *)CMB_GBWK->birthPanel + 1,
                            (datUnitWork_t *)CMB_GBWK->birthPanel);
    if ((s8)CMB_GBWK->birthPanel[0x2C5] == 0) {
        ((datUnitWork_t *)CMB_GBWK->birthPanel + 1)->id = 0;
    } else {
        cmbCopyCustomDevilToStock((datUnitWork_t *)CMB_GBWK->birthPanel,
                                (datUnitWork_t *)CMB_GBWK->birthPanel + 2);
    }
    cmbAddCustomDevilToStock((datUnitWork_t *)CMB_GBWK->birthPanel, (cmbUnit_t *)units, count);
    fclEncycRegist((datUnitWork_t *)CMB_GBWK->birthPanel, 0);
    if (datDevilTable[((datUnitWork_t *)CMB_GBWK->birthPanel)->id].race == 0x26 &&
        (s8)CMB_GBWK->birthPanel[0x2C4] != 2) {
        datAddItem(cmbGetSpecialItemId(), -1);
    }
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbStartResultScript);

typedef struct
{
    s32 messageId;
    u8 reserved04[0x2C];
} cmbMasterText_t;

extern cmbMasterText_t master_text[];

extern void cmbRunActionSe(s32 enabled, s32 mode);
extern s32 cmbChkPolygonMovie(void);
extern void *cmbUpdateIntroduceModel(cmbActionData_t *work);

s32 cmbCalcIntroduce(void)
{
    if (cmbChkPolygonMovie() != 0) {
        return 0;
    }
    if (CMB_GBWK->actionFrame != 0 && (s8)CMB_GBWK->birthPanel[0x2C5] != 0) {
        cmbUpdateIntroduceModel(&CMB_GBWK->actionData[3]);
        cmbRunActionSe(0, 1);
        CMB_GBWK->birthPanel[0x2C5] = 0;
    }
    if (CMB_GBWK->actionFrame < 150) {
        return 0;
    }
    if (CMB_GBWK->unk_00004 != 0) {
        fclSetWorkValue(8, master_text[((datUnitWork_t *)CMB_GBWK->birthPanel)->id].messageId);
        fclSetWorkValue(9, ((datUnitWork_t *)CMB_GBWK->birthPanel)->id);
        fclSetWorkValue(10, datDevilTable[((datUnitWork_t *)CMB_GBWK->birthPanel)->id].race);
        fclScriptStart(CMB_GBWK->script, 0xC, &CMB_GBWK->scriptArg);
    }
    return 1;
}

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

extern s32 cmbCalcRoot(void);
extern void cmbCalcDevSel1st(void);
extern void cmbInitStockCursor(void);
extern s32 cmbRunBirthDevilStep(void);
extern s32 cmbCalcBirthDevil(void);
extern void cmbSeqBuildResultTable(void);
extern s32 cmbCalcIntroduce(void);
extern void cmbStartResultScript(void);
extern void fclRbnCalc(fclRibbon_t *ribbon);

s32 cmbCalcSequence(void)
{
    switch ((s8)CMB_GBWK->unk_00001) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
    case 13:
    case 14:
    case 16:
        cmpResetDrawEnv();
        break;
    case 2:
        break;
    default:
        cmpModelInitCore2();
        break;
    }
    cmbSeqSetPhase();
    switch ((s8)CMB_GBWK->unk_00001) {
    case 0:
        if (cmbCalcRoot() != 0) {
            return 0;
        }
        break;
    case 1:
        CMB_GBWK->unk_00001 = 5;
    case 5:
        cmbCalcDevSel1st();
        break;
    case 3:
        if (CMB_GBWK->unk_00004 != 0) {
            s32 script;
            cmbGbWork_t *work;
            cmbEnableMenuEntry2();
            cmbEnableMenuEntry6();
            cmbEnableMenuEntry5();
            CMB_GBWK->unk_00005 = 1;
            script = fclScriptStart(CMB_GBWK->script, 10, &CMB_GBWK->scriptArg);
            work = CMB_GBWK;
            work->unk_00001 = 3;
            work->unk_00008 = script;
            CMB_GBWK->unk_00002 = 0;
        }
        break;
    case 2:
        if (cmbSeqStartScript() != 0) {
            return 0;
        }
        break;
    case 6:
        cmbInitStockCursor();
        break;
    case 9:
        cmbRunBirthDevilStep();
        break;
    case 10:
        if (cmbCalcBirthDevil() != 0) {
            return 0;
        }
        break;
    case 8:
        cmbSeqBuildResultTable();
        break;
    case 7:
    case 11:
        if (CMB_GBWK->unk_00004 != 0) {
            fclClearFlag(2);
        }
        break;
    case 14:
        if (cmbCalcIntroduce() == 0) {
            return 0;
        }
        break;
    case 15:
        cmbStartResultScript();
        break;
    case 4:
    case 12:
    case 13:
    case 16:
        break;
    }
    if (CMB_GBWK->ribbon != NULL) {
        fclRbnCalc(CMB_GBWK->ribbon);
    }
    CMB_GBWK->unk_00004 = 0;
    if (CMB_GBWK->unk_00000 & 0x10) {
        cmpUpdateTerminate((cmpInfoWindow_t *)CMB_GBWK);
        return -1;
    }
    return 0;
}

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

s32 cmbGetCombineRace(u16 devilA, u16 devilB, u16 devilC)
{
    datDevilInfo_t *table = datDevilTable;
    u8 race[3];
    s8 swap;

    race[0] = table[devilA].race;
    race[1] = table[devilB].race;
    race[2] = table[devilC].race;
    for (swap = 0; swap < 2; swap++) {
        u32 j = 0;
        do {
            if ((D_0032DBD8[j][0] == race[0]) &&
                (D_0032DBD8[j][1] == race[1]) &&
                (D_0032DBD8[j][2] == race[2])) {
                return (s8)D_0032DBD8[j][3];
            }
            j++;
        } while (j < 1);
        {
            u8 temp = race[0];
            race[0] = race[1];
            race[1] = temp;
        }
    }
    return 0;
}

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

s32 cmbCalcResultLevel(cmbUnit_t *unitA, cmbUnit_t *unitB, cmbUnit_t *unitC, s8 flag)
{
    s32 a, b;
    s32 c = 0;
    if (flag == 0) {
        datDevilInfo_t *table = datDevilTable;
        a = table[unitA->unit.id].baseLevel;
        b = table[unitB->unit.id].baseLevel;
        if (unitC != NULL) {
            c = table[unitC->unit.id].baseLevel;
        }
    } else {
        a = unitA->unit.level;
        b = unitB->unit.level;
        if (unitC != NULL) {
            c = unitC->unit.level;
        }
    }
    if (c == 0) {
        return ((a + b + 1) >> 1) + 1;
    }
    return (a + b + c + 2) / 3;
}

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

s32 cmbBuildDevilEntry(u16 devilId)
{
    datUnitWork_t unit;
    datUnitWork_t *leader = &cmbGlobalWork->party[0].unit;
    datDevilInfo_t *table = datDevilTable;
    s32 found = 0;
    s8 attempt = 1;
    u16 candidate;

    do {
        candidate = dds3GetRandA(0) % 0xA8 + 1;
        cmbCopyDefaultDevilToStock(candidate, &unit);
        if (table[candidate].race == 0x21) {
            continue;
        }
        if (cmbDevilIsUnlocked(candidate) != 0) {
            continue;
        }
        {
            s32 bound = leader->level + 3;
            if (unit.level > bound) {
                continue;
            }
            bound = leader->level - 15;
            if (unit.level < bound) {
                continue;
            }
        }
        if (cmbCheckEvoUnlocked(candidate, &cmbGlobalWork->party[0].unit, 16) >= 0) {
            continue;
        }
        if (cmbDevilIsSpecial(candidate) != 0 || candidate == devilId) {
            continue;
        }
        found = 1;
        break;
    } while (attempt++ < 16);
    if (found == 0) {
        return cmbFindSpecialEntry(devilId);
    }
    return candidate;
}

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
    dst->level = entry->baseLevel;
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

extern void fclDrawSpriteAlpha(s32 x, s32 y, s32 z, u8 mode, s32 *colors,
                               s32 page, s32 sprite, s32 *textures,
                               fclSpriteTbl_t *table, s32 ot);

void cmbDrawStockBox(s32 unusedX, s32 unusedY, s32 z, s32 unused3, s8 selected, s32 ot)
{
    s32 i;
    s32 rowY;
    s32 selectedY;

    cmpDrawBoxF(0x180, 0x388, z, 0x4F, 9, 0x33, ot);
    cmpDrawBoxF(0x6A0, 0x388, z, 0x6F, 9, 0x33, ot);
    cmpDrawBoxF(0xDC0, 0x388, z, 0x1E, 9, 0x33, ot);
    cmpDrawBoxF(0x180, 0x3D0, z, 0xA, 0x11B, 0x33, ot);

    rowY = 0x420;
    selectedY = 0;
    for (i = 0; i < 12; i++) {
        s32 sprite = i;
        s32 xOffset = (i < 9) ? 0 : -0x30;

        if (i == selected) {
            /* The sprite stays live across alpha rendering. Qualified
             * cc1probe --alloc --no-dumps maps pseudo 97 to $16;
             * see docs/cc1probe.md. */
            sprite = i + 0xC;
            fclDrawSpriteAlpha(0, selectedY, z << 4, 1, NULL, 2, 0x18,
                               CMB_GBWK->texHandle, cmbSpriteTable, ot);
        }
        fclDrawSpriteRes(0x180 + xOffset, rowY, z << 4, NULL, 2, (u16)sprite,
                         CMB_GBWK->texHandle, cmbSpriteTable, ot);
        rowY += 0xB8;
        selectedY += 0xB8;
    }

    fclDrawSpriteRes(0, 0, z << 4, NULL, 0, 1, CMB_GBWK->texHandle,
                     cmbSpriteTable, ot);
    fclDrawSpriteRes(0, 0, z << 4, NULL, 0, 2, CMB_GBWK->texHandle,
                     cmbSpriteTable, ot);
    fclDrawSpriteRes(0, 0, z << 4, NULL, 0, 3, CMB_GBWK->texHandle,
                     cmbSpriteTable, ot);
}

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

void cmbDrawStockBg(s32 x, s32 y, s32 z, u8 mode, s32 ot)
{
    s32 i;
    s32 j;

    cmpDrawBoxF(0x2A0, 0x400, z, 0x1C7, 0x113, 0x2C303880, ot);
    if (mode == 0) {
        for (i = 0; i < 12; i++) {
            cmpDrawBoxF(0x2B0, 0x408 + i * 0xB8, z, 0xD5, 0x14, 0x1B1A1773, ot);
        }
        for (i = 0; i < 12; i++) {
            s32 column = 0x1030 + i * 0x140;
            for (j = 0; j < 12; j++) {
                cmpDrawBoxF(column, 0x408 + j * 0xB8, z, 0x11, 0x14, 0x1B1A1773, ot);
            }
        }
    } else {
        for (i = 0; i < 12; i++) {
            cmpDrawBoxF(0x2B0, 0x408 + i * 0xB8, z, 0x1C5, 0x14, 0x1B1A1773, ot);
        }
    }
    if (mode == 0) {
        for (i = 0; i < 12; i++) {
            cmpDrawBoxF(0x1000 + i * 0x140, 0x408, z, 3, 0x111, 0x29282880, ot);
        }
    }
    for (i = 0; i < 11; i++) {
        fclDrawSpriteRes(0x2B0, 0x4A8 + i * 0xB8, z << 4, 0, 0, 0,
                         CMB_GBWK->texHandle, cmbSpriteTable, ot);
    }
}

extern s32 D_00345D88[];
extern s32 fclDrawPanel(fclPanel_t *panel, s32 ot);
extern void fclUpdatePanel(fclPanel_t *panel);

void cmbDrawListPanel(s32 unusedX, s32 unusedY, s32 z, s32 ot)
{
    cmbGbWork_t *work = CMB_GBWK;
    fclDrawPanel(work->panel, 0x47);
    work = CMB_GBWK;
    fclUpdatePanel(work->panel);
    fclDrawSpriteRes(0, 0, z << 4, 0, 0, 0x24, D_00345D88, cmbSpriteTable, ot);
    fclDrawSpriteRes(0, 0, z << 4, 0, 0, 0x25, D_00345D88, cmbSpriteTable, ot);
    fclDrawSpriteRes(0, 0, z << 4, 0, 0, 0x21, CMB_GBWK->texHandle, cmbSpriteTable, ot);
    fclDrawSpriteRes(0, 0, z << 4, 0, 0, 0x22, CMB_GBWK->texHandle, cmbSpriteTable, ot);
}

extern void cmbDrawListPanel(s32 x, s32 y, s32 z, s32 ot);
extern void cmbDrawStockBox(s32 x, s32 y, s32 z, s32 color, s8 count, s32 ot);

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

extern char D_0054FB10[];
extern s32 sprintf(char *out, const char *fmt, ...);

extern void *frGetCNameFRQ(s8 targetNo, s8 style, void *previous, s32 firstOption);
extern void frSetDrawPosition(void *request, u32 x, u32 y);
extern void frSetPriority(void *request, u32 priority);
extern void frSetFontColor(void *request, u32 color);
extern s32 func_001C7E38(void *request, s8 mode, u32 ot);
extern void *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                              const char *text, void *previous);
extern u16 datGetLevelForDraw(datUnitWork_t *work);

void cmbDrawDevilNameLevel(s32 x, s32 y, s32 priority, datUnitWork_t *work, s32 ot)
{
    void *request;
    char text[64];

    request = frGetCNameFRQ(0, 1, NULL, 0);
    frSetDrawPosition(request, 0x1340, 0x1D0);
    frSetPriority(request, priority << 4);
    frSetFontColor(request, 0xA09DC380);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);

    sprintf(text, D_0054FB10, datGetLevelForDraw(work));
    request = frwGetNumFontFRQ(0x1D30, 0x1C0, priority, 0xA09DC380, text, NULL);
    func_001C7E38(request, 1, ot);
    frDestroyFontRequestAfter(request);
}

extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *color,
                                s32 id, s32 spriteId, s32 *texs,
                                fclSpriteTbl_t *table, s32 ot);

void cmbDrawResultIcon(s8 mode, s32 z, s32 unused2, s32 unused3, s32 alpha, s32 ot)
{
    s32 color[4];
    u32 blend = fclBlendColor(0x80808080, 0, alpha);

    color[0] = blend;
    color[1] = blend;
    color[2] = blend;
    color[3] = blend;
    if (mode == 0) {
        fclDrawSpriteFilter(0, 0, z << 4, 0, color, 0, 9,
                            CMB_GBWK->texHandle, cmbSpriteTable, ot);
        fclDrawSpriteFilter(0, 0, z << 4, 0, color, 0, 10,
                            CMB_GBWK->texHandle, cmbSpriteTable, ot);
        fclDrawSpriteRes(0, 0, z << 4, color, 0, 0xE, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
        fclDrawSpriteRes(0xB4, 0, z << 4, color, 0, 0x10, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
    } else {
        fclDrawSpriteRes(0, 0, z << 4, color, 0, 0xB, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
        fclDrawSpriteRes(0, 0, z << 4, color, 0, 0xC, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
        fclDrawSpriteRes(0, 0, z << 4, color, 0, 0xF, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
        fclDrawSpriteRes(0xB4, 0, z << 4, color, 0, 0x11, CMB_GBWK->texHandle,
                         cmbSpriteTable, ot);
    }
}

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

extern s32 fclDrawPanelSprite(s32 id, s32 arg);

void cmbDrawInfoSprite(s32 unusedX, s32 unusedY, s32 z, s32 unusedOt)
{
    s32 i;

    fclDrawPanelSprite(0x30000, 0x47);
    for (i = 0; i < 2; i++) {
        fclDrawSpriteRes(0, 0, z, NULL, 0, 0x1F, CMB_GBWK->texHandle,
                         cmbSpriteTable, 0x47);
        fclDrawSpriteRes(0, 0, z, NULL, 0, 0x20, CMB_GBWK->texHandle,
                         cmbSpriteTable, 0x47);
    }
    for (i = 0x15; i < 0x1F; i++) {
        fclDrawSpriteRes(0, 0, z, NULL, 0, (u16)i, CMB_GBWK->texHandle,
                         cmbSpriteTable, 0x47);
    }
}

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

extern void cmpDrawStatusCom(s32 unit, s8 mode, s8 second, s8 third);
extern void fclRbnDraw(fclRibbon_t *ribbon, s32 x, s32 y, u32 z,
                      s32 alpha, s32 flag, s32 width, s32 ot);

void cmbDrawStatus(void)
{
    cmbGbWork_t *work = CMB_GBWK;
    s32 slot = 0;
    switch ((s8)work->unk_00003) {
    case 5:
        slot = (s8)work->unk_165EE[work->stockCursorGroup[0].cursor.unk_22 +
                              (u8)work->stockCursorGroup[0].cursor.unk_24];
        break;
    case 6:
        slot = (s8)work->unk_165EE[work->stockCursorGroup[1].cursor.unk_22 +
                              (u8)work->stockCursorGroup[1].cursor.unk_24];
        break;
    case 8:
        slot = (s8)work->unk_165EE[work->stockCursorGroup[2].cursor.unk_22 +
                              (u8)work->stockCursorGroup[2].cursor.unk_24];
        break;
    }
    cmpDrawStatusCom((s32)&cmbGlobalWork->party[slot].unit, 1, 0, 0);
    fclRbnDraw(CMB_GBWK->ribbon, 0, 0x98, 0, 0x80, 2, -0x80, 0x47);
}

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

typedef struct cmpPadStopWork_t cmpPadStopWork_t;
extern s32 cmpGetPressedButton(s32 mask);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

void cmbUpdateStockCursor(void)
{
    s32 button = cmpGetPressedButton(0x237) & 0xFFFF;
    switch (button) {
    case 1: {
        s8 selected;
        if ((s8)CMB_GBWK->birthPanel[0x2C8] < (s8)CMB_GBWK->stockCursorCount) {
            button = 0;
            break;
        }
        selected = (s8)((u8)CMB_GBWK->stockCursorGroup[0].cursor.unk_22 +
                                                (u8)CMB_GBWK->stockCursorGroup[0].cursor.unk_24);
        CMB_GBWK->stockSlot[0].unk_01 = selected;
        CMB_GBWK->stockSlot[0].index = CMB_GBWK->unk_165EE[selected];
        if (cmbStockIsLocked(CMB_GBWK->stockSlot[0].index)) {
            button = 0x8000;
            break;
        }
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 6;
        CMB_GBWK->unk_00004 = button;
        break;
    }
    case 2:
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 0;
        CMB_GBWK->unk_00004 = button;
        break;
    case 0x10:
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[0].cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[0].cursor, 1);
        break;
    case 0x200:
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 9;
        CMB_GBWK->unk_00004 = 1;
        break;
    }
    if (button == 4) {
        if ((s8)CMB_GBWK->displayFlags[0] == 0) {
            CMB_GBWK->displayFlags[0] = 1;
        } else {
            CMB_GBWK->displayFlags[0] = 0;
        }
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[0].cursor, 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&CMB_GBWK->stockCursorGroup[0].cursor,
                                            button, 0x30));
}

extern char D_0053CC60[];
INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CC60);

void cmbChangeSequence(void)
{
    s32 button = cmpGetPressedButton(0x237) & 0xFFFF;

    switch (button) {
    case 1: {
        s32 count = (s8)((u8)CMB_GBWK->stockCursorGroup[1].cursor.unk_22 +
                         (u8)CMB_GBWK->stockCursorGroup[1].cursor.unk_24);
        s32 previous = (s8)CMB_GBWK->stockSlot[0].unk_01;
        u8 devilIndex;

        CMB_GBWK->stockSlot[1].unk_01 = count;
        devilIndex = CMB_GBWK->unk_165EE[count];
        CMB_GBWK->stockSlot[1].index = devilIndex;
        if (cmbStockIsLocked(CMB_GBWK->stockSlot[1].index)) {
            button = 0x8000;
            break;
        }
        if (CMB_GBWK->resultId[0][count][previous] == 0) {
            return;
        }
        CMB_GBWK->stockSlot[2].index = -1;
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 0xA;
        CMB_GBWK->unk_00004 = button;
        printf(D_0053CC60);
        break;
    }
    case 2:
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 5;
        CMB_GBWK->unk_00004 = button;
        break;
    case 0x10:
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[1].cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[1].cursor, 1);
        break;
    case 0x200:
        CMB_GBWK->displayFlags[0] = 0;
        CMB_GBWK->unk_00001 = 9;
        CMB_GBWK->unk_00004 = 1;
        break;
    }

    if (button == 4) {
        if ((s8)CMB_GBWK->displayFlags[0] == 0) {
            CMB_GBWK->displayFlags[0] = 1;
        } else {
            CMB_GBWK->displayFlags[0] = 0;
        }
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&CMB_GBWK->stockCursorGroup[1].cursor, 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&CMB_GBWK->stockCursorGroup[1].cursor, button, 0x30));
}

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

extern void cmbAddBirthDevilToStock(datUnitWork_t *unit, u8 count);

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

s32 cmbUpdateDecideDevil(void)
{
    s32 flag;

    if (dds3GetProcessStat(CMB_GBWK->scriptArg) == 2) {
        return 0;
    }
    flag = fclGetFlag(2);
    switch (flag) {
    case 0:
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
        CMB_GBWK->scriptArg = NULL;
        fclCombineInitWork();
        CMB_GBWK->unk_00001 = 13;
        CMB_GBWK->unk_00004 = 1;
        cmbSetAction();
        CMB_GBWK->unk_1650A = 3;
        break;
    case 1:
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
        if (CMB_GBWK->stockSlot[2].index == -1) {
            CMB_GBWK->unk_00001 = 6;
        } else {
            CMB_GBWK->unk_00001 = 8;
        }
        CMB_GBWK->unk_00004 = 2;
        break;
    }
    return 1;
}

s32 cmbUpdateSacConfirm(void)
{
    s32 flag;

    if (dds3GetProcessStat(CMB_GBWK->scriptArg) == 2) {
        return 0;
    }
    flag = fclGetFlag(2);
    switch (flag) {
    case 0:
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
        CMB_GBWK->unk_00001 = 8;
        CMB_GBWK->unk_00004 = 1;
        break;
    case 1:
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
        fclScriptStart(CMB_GBWK->script, 8, &CMB_GBWK->scriptArg);
        CMB_GBWK->unk_00001 = 11;
        CMB_GBWK->unk_00004 = flag;
        break;
    }
    return 1;
}

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

void cmbUpdateIntroduce(void)
{
    s32 frame;
    s32 processStat;

    if (cmbChkPolygonMovie() != 0) {
        return;
    }
    frame = CMB_GBWK->actionFrame;
    if (frame < 150) {
        CMB_GBWK->actionFrame = frame + 1;
        return;
    }
    if (frame < 1000) {
        CMB_GBWK->actionFrame = frame + 1;
    }
    processStat = dds3GetProcessStat(CMB_GBWK->scriptArg);
    if (processStat != 0) {
        return;
    }
    if (dds3FadeCheck() != 0) {
        return;
    }
    fclSetWorkValue(4, 0);
    if (cmbChkActScript() != 0) {
        return;
    }
    if (CMB_GBWK->introductionFlag == 0) {
        cmbFreeIntroduceModels();
        cmbFreeSmokeEffect();
        fclFreeStage();
        CMB_GBWK->introductionFlag = 1;
        return;
    }
    fclCombineLoadTextures();
    CMB_GBWK->unk_0410C[0] = 0;
    CMB_GBWK->unk_0410C[1] = fclCombineCheckMoonPhase();
    evtBitOn(0x915);
    cmbActionBGM(0);
    {
        cmbGbWork_t *work = CMB_GBWK;
        work->unk_00001 = 0;
        work->scriptArg = NULL;
    }
    CMB_GBWK->unk_00004 = 1;
}

extern void fclEncycStartProcess(s32 mode);

extern void fclEncycSetTexHandle(s32 *texhandle);

void cmbStartEncycView(s32 mode)
{
    fclEncycStartProcess(mode);
    fclEncycSetTexHandle(CMB_GBWK->texHandle);
    fclEncycSetExplainData(CMB_GBWK->explainData);
}

extern s32 fclEncycWork(void);
extern s32 fclEncycCheckRegistable(void);
extern s32 fclEncycGetNum(void);
extern void cmbStartEncycView(s32 mode);

void cmbUpdateResultCursor(void)
{
    s32 nextPhase;
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    if (fclEncycWork() != 0) return;
    if (fclGetFlag(0) == 0) return;
    /* Qualified cc1probe --alloc --no-dumps assigns this live phase
     * value (pseudo 84) to $17; see docs/cc1probe.md. */
    nextPhase = 2;
    switch (button) {
    case 1: {
        s32 selected = CMB_GBWK->resultCursor.unk_22 + (u8)CMB_GBWK->resultCursor.unk_24;
        s32 mode;
        s32 scriptIndex;
        s32 scriptResult;
        cmbGbWork_t *work;
        cmbGbWork_t *errorWork;
        switch (selected) {
        case 0:
            if (fclEncycCheckRegistable() != 0) {
                mode = 0;
                goto openEncyc;
            }
            errorWork = CMB_GBWK;
            scriptIndex = 0x14;
            goto scriptError;
        case 1:
            if (fclEncycGetNum() == 0) goto error21;
            mode = 1;
            break;
        default:
            CMB_GBWK->unk_00001 = 0;
            goto remember;
        }
openEncyc:
        cmbStartEncycView(mode);
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
remember:
        CMB_GBWK->unk_00004 = button;
        break;
error21:
        errorWork = CMB_GBWK;
        scriptIndex = 0x15;
scriptError:
        errorWork->unk_00005 = button;
        scriptResult = fclScriptStart(CMB_GBWK->script, scriptIndex, &CMB_GBWK->scriptArg);
        work = CMB_GBWK;
        work->unk_00001 = 0x10;
        work->unk_00008 = scriptResult;
        CMB_GBWK->unk_00003 = 0x10;
        CMB_GBWK->unk_00004 = -1;
        CMB_GBWK->unk_00002 = nextPhase;
        break;
    }
    case 2:
        CMB_GBWK->unk_00001 = 0;
        CMB_GBWK->unk_00004 = 1;
        break;
    case 0x10:
        cmpMoveCursor(&CMB_GBWK->resultCursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&CMB_GBWK->resultCursor, 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&CMB_GBWK->resultCursor, 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&CMB_GBWK->resultCursor, button, 0x30));
}

extern s8 FclDebug(s8 mode, void *work);

s32 cmbHandleDebugExit(void)
{
    s32 result = 0;
    s8 debugResult = FclDebug(0, CMB_GBWK);

    if (debugResult == -1) {
        return result;
    }
    if (debugResult < 0) {
        result = 1;
    } else if (debugResult != 0) {
        result = 1;
    } else {
        evtDisableMoonDraw();
        fclScriptForcedEnd(CMB_GBWK->scriptArg);
        CMB_GBWK->scriptArg = NULL;
        fclCombineInitWork();
        CMB_GBWK->unk_00001 = 13;
        CMB_GBWK->unk_00004 = 1;
        result = 1;
    }
    return result;
}

extern void fclSetPanelAnimState(void *anim, u8 state);

s32 cmbUpdatePanelAnim(void)
{
    s8 phase;

    phase = CMB_GBWK->unk_00001;
    switch (phase) {
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
    case 15:
        fclSetPanelAnimState(CMB_GBWK->resultPanelAnim, 2);
        break;
    default:
        fclSetPanelAnimState(CMB_GBWK->resultPanelAnim, 1);
        break;
    }

    switch ((s8)CMB_GBWK->unk_00001) {
    case 10:
    case 11:
    case 15:
        if (CMB_GBWK->stockSlot[2].index < 0) {
            break;
        }
    case 8:
        fclSetPanelAnimState(CMB_GBWK->sacrificePanelAnim, 2);
        break;
    default:
        fclSetPanelAnimState(CMB_GBWK->sacrificePanelAnim, 1);
        break;
    }

    fclRunPanelAnim(CMB_GBWK->resultPanelAnim);
    return fclRunPanelAnim(CMB_GBWK->sacrificePanelAnim);
}


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

extern void cmbUpdateRoot(void);
extern void cmbUpdateMenuCursor(void);
extern void cmbUpdateResultCursor(void);
extern void cmbUpdateStockCursor(void);
extern void cmbChangeSequence(void);
extern void cmbUpdateStockCursor2(void);
extern void cmbUpdateBirthDevil(void);
extern void cmbUpdateAction(void);
extern s32 cmpUpdateSeqInfo(cmbGbWork_t *work, s8 previous);

s32 cmbUpdateSequence(void)
{
    s32 updateInfo = 1;
    s8 previous = CMB_GBWK->unk_00001;

    if (CMB_GBWK->unk_00000 & 0x40) {
        return -1;
    }
    cmbUpdatePanelAnim();
    cmbUpdateModelLoad();
    switch ((s8)CMB_GBWK->unk_00001) {
    case 0:
        if (cmbHandleDebugExit() == 0) {
            cmbUpdateRoot();
        }
        break;
    case 3:
        cmbUpdateMenuCursor();
        break;
    case 4:
        if (cmpChkSeqInfo(CMB_GBWK) == 2) {
            fclClearFlag(1);
            fclSetWorkValue(1, 0);
            CMB_GBWK->unk_00000 |= 0x10;
        }
        break;
    case 2:
        cmbUpdateResultCursor();
        break;
    case 5:
        cmbUpdateStockCursor();
        break;
    case 6:
        cmbChangeSequence();
        break;
    case 9:
        cmbWaitCancelButton();
        break;
    case 10:
        cmbUpdateBirthDevil();
        break;
    case 7:
        updateInfo = cmbUpdateSacConfirm();
        break;
    case 8:
        cmbUpdateStockCursor2();
        break;
    case 11:
        updateInfo = cmbUpdateDecideDevil();
        break;
    case 13:
        if (fclCombineChkEventStat() == 0) {
            cmbUpdateAction();
        }
        break;
    case 15:
        cmbSeqAfterBirth();
        break;
    case 14:
        cmbUpdateIntroduce();
        break;
    case 16:
        cmbSeqReturnToPrev();
        break;
    }
    if (updateInfo == 0) {
        return 0;
    }
    cmpUpdateSeqInfo(CMB_GBWK, previous);
    return 0;
}

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

extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
typedef struct fldResourcePoint_t fldResourcePoint_t;
extern fldResourcePoint_t *dds3PointBasicGetResource(dds3Basic_t *pPointBasic);

s32 cmbFindCameraPoint(const char *name, f32 *out)
{
    dds3Basic_t *scene = dds3GetControlScene();
    dds3Basic_t *basic = dds3SceneBasicSearchNameAndType(scene, 0x11, name);

    if (basic == NULL) {
        return 0;
    }
    SDF_VU0_COPYVECTOR(out, dds3PointBasicGetResource(basic));
    return 1;
}

extern datUnitVisual_t datModelParamTable[];
extern void mdlSetPosition(struct mdlObj_t *model);
extern void mdlSetScale(struct mdlObj_t *model);
extern void mdlLink(struct mdlObj_t *model, void *ot);

void cmbDrawActionModel(struct mdlObj_t *model, u16 id, f32 *position, u32 color, void *ot)
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

extern void cmbDrawModelAt(u8 index, f32 *position, f32 firstOffset, f32 secondOffset);

void cmbDrawTargetModel(f32 (*positions)[4], f32 *sizes, s32 *enabled,
                        s32 draw, f32 scale)
{
    s32 i = 0;

    for (; i < 2; i++) {
        if (enabled[i] != 0 && CMB_GBWK->actionZoom > 0.5f && draw != 0) {
            f32 value = sizes[i] * scale;
            f32 offset = cmbGetModelScaleById(CMB_GBWK->actionData[i].id);
            cmbDrawModelAt(i, *positions, sizes[i], value + offset);
        }
        positions++;
    }
}

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

extern void cmbGetModelMarkPos(f32 (*positions)[4], f32 *sizes, s32 *found);
extern void cmbDrawModelPanel(f32 (*positions)[4], f32 *sizes, u32 color,
                              f32 scale, s32 *found, s32 ot);
extern void cmbDrawFullScreenPanel(s32 flags);
extern void mdlSetColorWithEffect(struct mdlObj_t *model, u32 color);
extern f32 cmbActionTargetModel[3];
extern f32 cmbTargetModelId;
extern s8 cmbActionMode;
extern char D_0053CE48[];

void cmbLoadTargetModel(s32 drawTarget, s32 drawModels, f32 scale)
{
    f32 positions[3][4] __attribute__((aligned(16)));
    f32 sizes[3];
    s32 found[3];
    void *modelOt[4];
    s32 i;

    cmbGetModelMarkPos(positions, sizes, found);
    if (drawModels != 0) {
        if (scale != 0.0f) {
            cmpDrawSpriteFlag(0, 0, 0x11);
            cmpDrawSpriteFlag(0, 1, 0x11);
            cmbDrawModelPanel(positions, sizes, 0, scale, found, 0x12);
            cmpDrawSpriteFlag(1, 0, 0x16);
            cmpDrawSpriteFlag(1, 1, 0x16);
        }
        for (i = 0; i < 3; i++) {
            if (found[i] != 0) {
                u32 color;
                if (cmbActionMode != 0) {
                    CMB_GBWK->modelMark[2][1] -= cmbTargetModelId;
                }
                color = fclBlendColor(0x80000000, 0x80808080, (s32)cmbActionTargetModel[i]);
                mdlSetColorWithEffect(CMB_GBWK->actionData[i].model, color);
                cmbGetSceneModel((cmbSceneModelOwner_t *)CMB_GBWK->actionData[i].model, D_0053CE48);
                cmbInitModelOt((u16)CMB_GBWK->actionData[i].id, modelOt);
                cmbDrawActionModel(CMB_GBWK->actionData[i].model,
                    (u16)CMB_GBWK->actionData[i].id, CMB_GBWK->modelMark[i], 0x80808080, modelOt);
            }
        }
    }
    if (drawModels != 0 && scale != 0.0f) {
        cmbDrawFullScreenPanel(0x1B);
    }
    cmbDrawTargetModel(positions, sizes, found, drawTarget, scale);
    cmpDrawSpriteFlag(0, 0, 0x21);
    cmpDrawSpriteFlag(0, 1, 0x21);
}

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

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CE38);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CE48);

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbUpdateTimeTable);

typedef struct effEVECombParSmokeParam_t {
    f32 position[4];
    u32 number;
    s32 duration;
    s32 wait;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode;
    u8 pad2A[2];
    s32 fade_in;
    s32 fade_out;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent;
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge;
    s32 colorTransitionFrames;
} effEVECombParSmokeParam_t;
typedef struct effEVECombParSmoke_t effEVECombParSmoke_t;
extern effEVECombParSmoke_t *effEVECombParSmokeCreate(effEVECombParSmokeParam_t *params);

s32 cmbStartSmokeEffect(u16 devilId)
{
    effEVECombParSmokeParam_t params;
    f32 modelSize = datModelParamTable[devilId].formsize / 50.0f;

    params = *(effEVECombParSmokeParam_t *)CMB_GBWK->table;
    params.spawnRadius = cmbGetSmokeScale(devilId);
    params.number = cmbGetSmokeColor(devilId);
    params.number = (u32)((f32)params.number * modelSize);
    params.spawnRadius *= modelSize * 0x1.333332p+0f;
    return (s32)effEVECombParSmokeCreate(&params);
}

void cmbSetActionWorkValues(void)
{
    if (CMB_GBWK->stockSlot[2].index >= 0) {
        fclSetWorkValue(2, 1);
    } else {
        fclSetWorkValue(2, 0);
    }
    fclSetWorkValue(3, (s8)CMB_GBWK->birthPanel[0x2C5]);
    if (datDevilTable[((datUnitWork_t *)CMB_GBWK->birthPanel)->id].race != 0x26) {
        fclSetWorkValue(7, 0);
    } else if ((s8)CMB_GBWK->birthPanel[0x2C4] != 2) {
        fclSetWorkValue(7, cmbGetSpecialItemId());
    } else {
        fclSetWorkValue(7, -1);
    }
    fclClearFlag(3);
    fclClearFlag(4);
    fclSetWorkValue(4, 1);
    fclSetWorkValue(5,
                    datModelParamTable[((datUnitWork_t *)CMB_GBWK->birthPanel)->id].modelsize);
    fclSetWorkValue(6, ((datUnitWork_t *)CMB_GBWK->birthPanel)->id);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", cmbRunActionSe);

extern void fclCombineBGM(s8 Mode);

extern const char D_0053CEB0[];
extern const char D_0053CEC8[];

s32 cmbActionBGM(s32 mode)
{
    s32 result;

    if (mode == 0) {
        p2sdStopBGM(0x380000);
        fclCombineBGM(0);
        return printf(D_0053CEB0);
    }
    fclCombineBGM(mode);
    p2sdPlayBGM(0x380000);
    result = printf(D_0053CEC8);
    /* Use fclCombineBGM's qualified consumed-selector reset pattern to keep
     * the native final diagnostic call and epilogue. */
    mode = 0;
    return result;
}

void func_00205AB8(void) {
}

typedef struct {
    datUnitWork_t unit;
    s32 reservedEC;
    u16 secondModelId;
} cmbBirthModelInfo_t;

extern f32 cmbActionTargetModel[3];
extern f32 cmbTargetModelId;
extern s8 cmbActionMode;
extern s8 cmbActionStep;
extern char D_0053CEE0[];
extern char D_0053CEF8[];

void cmbInitAction(void)
{
    cmpModelFreeResource();
    cmpResetDrawEnv();
    fclCombineTexFree();
    fclFreeStage();

    cmbLoadActionModel(CMB_GBWK->stockSlot[0].id,
                       &CMB_GBWK->actionData[0]);
    cmbUpdateIntroduceModel(&CMB_GBWK->actionData[0]);
    cmbLoadActionModel(CMB_GBWK->stockSlot[1].id,
                       &CMB_GBWK->actionData[1]);
    cmbUpdateIntroduceModel(&CMB_GBWK->actionData[1]);
    if (CMB_GBWK->stockSlot[2].index >= 0) {
        cmbLoadActionModel(CMB_GBWK->stockSlot[2].id,
                           &CMB_GBWK->actionData[2]);
        cmbUpdateIntroduceModel(&CMB_GBWK->actionData[2]);
    } else {
        cmbInitActionData(0, &CMB_GBWK->actionData[2]);
    }

    if (((cmbBirthModelInfo_t *)CMB_GBWK->birthPanel)->secondModelId != 0) {
        cmbInitActionData(((datUnitWork_t *)CMB_GBWK->birthPanel)->id,
                          &CMB_GBWK->actionData[3]);
        cmbLoadActionModel(((cmbBirthModelInfo_t *)CMB_GBWK->birthPanel)->secondModelId,
                           &CMB_GBWK->actionData[4]);
    } else {
        cmbLoadActionModel(((datUnitWork_t *)CMB_GBWK->birthPanel)->id,
                           &CMB_GBWK->actionData[3]);
        cmbInitActionData(0, &CMB_GBWK->actionData[4]);
    }

    CMB_GBWK->smoke[0] = cmbStartSmokeEffect(CMB_GBWK->stockSlot[0].id);
    CMB_GBWK->smoke[1] = cmbStartSmokeEffect(CMB_GBWK->stockSlot[1].id);
    CMB_GBWK->smoke[2] = cmbStartSmokeEffect(CMB_GBWK->stockSlot[2].id);
    cmbSetActionWorkValues();
    evtStartEvent(0x327);
    dds3FadeOut(0, 0, 0, 0);

    {
        cmbGbWork_t *work = CMB_GBWK;
        work->actionZoom = 0.0f;
        work->actionTargetIndex = 0;
        if (work->stockSlot[2].index >= 0) {
            cmbActionMode = 1;
        } else {
            cmbActionMode = 0;
        }
    }
    cmbActionStep = 0;
    CMB_GBWK->introductionFlag = 0;
    cmbActionTargetModel[0] = 0.0f;
    cmbActionTargetModel[1] = 0.0f;
    cmbActionTargetModel[2] = 0.0f;
    cmbTargetModelId = 0.0f;
    printf(D_0053CEE0);
    printf(D_0053CEF8, cmbActionMode);
}

void cmb_205CE8(void)
{
    s32 i;
    if (CMB_GBWK->actionZoom < 2.0f) {
        CMB_GBWK->actionZoom += 0.007f;
    }
    for (i = 0; i < 2; i++) {
        cmbActionTargetModel[i] += func_00204018((u16)CMB_GBWK->actionData[i].id);
        if (cmbActionTargetModel[i] > 256.0f) {
            cmbActionTargetModel[i] = 256.0f;
        }
    }
}

void func_00205DB8(void)
{
    if (CMB_GBWK->stockSlot[2].index < 0) {
        return;
    }
    func_002043E0(&CMB_GBWK->actionData[2]);
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
    mdlSmoothLoopMotion(CMB_GBWK->actionData[2].model, 0, 1, 0.0f, 1.0f);
}

extern f32 cmbTargetModelId;

void func_00205EA0(void)
{
    sdfMotionCtrlr_t *motion = CMB_GBWK->actionData[2].model->first;

    if ((f32)datModelParamTable[(u16)CMB_GBWK->actionData[2].id].badframe - 1.0f <= motion->frame) {
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

extern s8 cmbActionStep;
extern s32 cmpLoadStatusModel(void);
extern s32 sdfDelayedResrcCheck(void);
extern s8 fclGetWorkValue(s8 index);
extern void cmbUpdateTimeTable(void);

void cmbUpdateAction(void)
{
    void *event;
    void *movie;
    s32 allowSkip = 0;

    if (CMB_GBWK->actionPhase == 0) {
        if (cmpLoadStatusModel() != 0) {
            return;
        }
        cmbInitAction();
        {
            cmbGbWork_t *work = CMB_GBWK;
            work->actionPhase = 1;
            work->actionFrame = 0;
        }
        return;
    }
    if (cmbActionStep != 0) {
        if (cmbActionStep == 2) {
            dds3KillProcessName(D_0053CF20, 0);
            while (sdfDelayedResrcCheck() != 0) {
            }
            fclFilterOFF();
        }
        cmbFreeActionModels();
        cmbRunActionSe(0, 0);
        if (cmbActionStep == 2) {
            p2sdPlaySE(0x380004, 0x7F, 0x3F);
        }
        if ((s8)CMB_GBWK->birthPanel[0x2C5] != 0) {
            cmbUpdateIntroduceModel(&CMB_GBWK->actionData[4]);
            cmbActionExists(&CMB_GBWK->actionData[3]);
            cmbRunActionSe(1, 1);
        } else {
            cmbUpdateIntroduceModel(&CMB_GBWK->actionData[3]);
        }
        {
            cmbGbWork_t *work = CMB_GBWK;
            work->actionPhase = 0;
            work->actionFrame = 0;
        }
        CMB_GBWK->unk_00001 = 14;
        CMB_GBWK->unk_00004 = 1;
        return;
    }
    event = dds3SearchProcessName(D_0053CF10);
    movie = dds3SearchProcessName(D_0053CF20);
    if (CMB_GBWK->actionFrame == 0) {
        if (fclCombineChkEventStat() != 0 || event == NULL || movie == NULL) {
            return;
        }
        if ((*(u32 *)dds3GetProcessData((s32)movie) & 8) == 0) {
            return;
        }
        if (fclGetFlag(4) == 0) {
            return;
        }
        cmbActionBGM(1);
        cmbRunActionSe(1, 0);
        dds3FadeIn(0, 0, 0, 0);
    }
    cmbUpdateTimeTable();
    if (fclGetFlag(3) != 0) {
        cmbActionStep = 1;
    } else {
        if (fclGetWorkValue(2) != 0) {
            if (CMB_GBWK->actionFrame < 780) {
                allowSkip = 1;
            }
        } else {
            allowSkip = CMB_GBWK->actionFrame < 580;
        }
        if (allowSkip != 0) {
            if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x23] < 0) {
                if (CMB_GBWK->unk_0410C[0] == 0) {
                    if (dds3GetActiveScene() != NULL) {
                        cmbActionStep = 2;
                    }
                }
            }
        }
    }
}

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

void shop_206948(void)
{
    s32 i;

    CMB_GBWK->eligibleCount = 0;
    for (i = 0; i < 16; i++) {
        datUnitWork_t *unit = &cmbGlobalWork->party[i].unit;

        if (func_00206910(unit) != 0) {
            cmbGbWork_t *work = CMB_GBWK;
            s32 *count = &work->eligibleCount;
            s32 countValue = *count;
            datUnitWork_t *destination = &work->eligibleStock[countValue];
            /* Qualified cc1probe: sched1 uids 64/86/89 share count pseudo
             * 100 for the row offset and published successor (native $3).
             * Splitting the successor into another value restores nine
             * differences; see docs/CONTRIBUTING.md, shared temporaries. */
            countValue++;
            *count = countValue;
            cmbCopyCustomDevilToStock(destination, unit);
        }
    }
    func_00206888(0, 0, 0, (s8)CMB_GBWK->eligibleCount, 8, &CMB_GBWK->eligibleList);
}

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

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CEB0);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CEC8);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CEE0);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", D_0053CEF8);

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

