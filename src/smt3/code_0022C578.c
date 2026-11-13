#include "common.h"
#include "facility.h"
#include "result2.h"
#include "camp.h"
#include "kernel.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "draw3d.h"
#include "kernel_draw.h"
#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

typedef struct frFontHandle_t frFontHandle_t;
typedef struct frFontRequest_t frFontRequest_t;

extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *packet, s32 prim, s32 count, s64 registers, s32 loops);


extern char fclCombineCalcName[];

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

extern datUnitWork_t *cmbBirthPanel;

extern u16 rstHeartsMasterPhase;

extern u8 rstSkillActFlag;

extern s32 D_0054FEBC;

extern s8 cmpStatusModelFlag;

extern s8 cmpStatusModelLoadState;

extern mdlObj_t *cmpEncycWork;

extern u16 cmpStatusModelId;

extern u32 fclDbgPID;


extern s32 campStatusActive;



typedef struct
{
    /* 0x0000 */ u8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ u8 unk_0019[3];
    /* 0x001C */ s32 reserved001C;
    /* 0x0020 */ s32 entryStep;
    /* 0x0024 */ s32 targetPhase;
    /* 0x0028 */ u8 reserved0028[4];
    /* 0x002C */ s32 fadeAlpha;
    /* 0x0030 */ s8 unk_30;
    /* 0x0031 */ s8 unk_31;
    /* 0x0032 */ u8 unk_0032[0x112E];
    /* 0x1160 */ s32 unk_1160;
    /* 0x1164 */ s32 unk_1164;
    /* 0x1168 */ s32 unk_1168;
    /* 0x116C */ s32 unk_116C;
    /* 0x1170 */ s32 unk_1170;
    /* 0x1174 */ s32 unk_1174;
    /* 0x1178 */ u8 reserved1178[0x100];
    /* 0x1278 */ cmpHeartsIcon_t heartsIcons[25];
} campInfoWork_t;

extern fclEncyc_instance_tag *g_pinst;


extern campInfoWork_t *campInfoWork;

extern s8 cmpParamGaugeState[2];

extern s8 cmpStatusModelReady;

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

extern cmpInfoWindow_t *cmpInfoWindow;

extern cmbWork_t *shpWork;

extern saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 gMsgID;

extern s32 gSelMsgNo;

typedef struct
{
    /* 0x00 */ s8 id;
    /* 0x01 */ s8 type;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ s32 color;
} cmpHeartsIconTableEntry_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0xF];
    /* 0x14 */ void *allocation;
    union {
        /* 0x18 */ u16 slot[16];
        struct {
            u16 reserved0018[10];
            /* 0x2C */ s32 fadeAlpha;
            /* 0x30 */ s8 unk_30;
            /* 0x31 */ s8 unk_31;
            /* 0x32 */ u8 unk_32[6];
        };
    };
    /* 0x38 */ s8 slotCount;
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

extern cmpHeartsIconTableEntry_t cmpHeartsIconTable[];

extern campStatusPanel_t *campStatusPanel;

extern s32 cmpGaugeTimers[6];

extern void campUpdateEffectStep(void);

extern s32 func_00232198(void);

extern cmpPanelEff_t *cmpPanelEffList;

extern void cmpDrawSpriteFlag(u8 id, s32 flag, s32 arg);

extern void cmpDrawSprite(s32 id, s32 flag, s32 arg);

extern void cmpDrawSprite2(s32 id, s32 flag, s32 arg);

extern void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);

extern void cmpDrawBoxG(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *colors, s32 g, s32 h);









extern s32 sdfDelayedResrcCheck(void);

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 PartyCnt;
    /* 0x03 */ s8 StockCnt;
    /* 0x04 */ s8 InvalidCnt;
    /* 0x05 */ s8 StockIdx[0x10];
    /* 0x15 */ s8 ListIdx[0x10];
    /* 0x25 */ u8 unk_25[0x31];
} cmpLocalStock_t;

/* The stock work the cursor helpers take: two 0x56-byte rows at the top and a
 * pair of 0x103C-byte records at 0xC0, both selected by the same index. */
typedef struct
{
    /* 0x0000 */ cmpLocalStock_t stock[2];
    /* 0x00AC */ u8 unk_00AC[0x14];
    /* 0x00C0 */ cmpStockInfo_t info[2];
} cmpStockWork_t;


typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ u8 unk_0038[0x1004];
} cmpListPage_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002;
    /* 0x0003 */ s8 previousPhase;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 stockMessagePhase;
    /* 0x0019 */ u8 unk_0019[0x103F];
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
    /* 0x8304 */ u16 itemId[0xA0];
    /* 0x8444 */ u8 itemMode;
    /* 0x8445 */ u8 statusPhase;
    /* 0x8446 */ u8 reserved8446[2];
    /* 0x8448 */ u8 crossPanel[0x20];
    /* 0x8468 */ s32 unk_8468;
    /* 0x846C */ u8 unk_846C[0x34];
    /* 0x84A0 */ s32 effectStep;
    /* 0x84A4 */ void *dhProcess;
    /* 0x84A8 */ s32 RootTimer;
    /* 0x84AC */ s32 fadeStep;
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

extern datSkillBadEntry_t datSkillBadEntries[] FAKEMATCH_ASM_NAME("datSkillTable");

extern campWork_t *campWork;

extern datItemEntry_t datItemTable[];

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

extern void cmpBuildStockNameList(cmpLocalStock_t *stock);

extern void cmpClearCursorSlots(cmpCursorInfo_t *cur);

extern void cmpInitStockCursorPos(void);

extern s32 cmpStartSkillTokusyuMsg(u16 skill);

extern void cmpModelInitCore2(void);

extern campStatusPanel_t *cmpStatusPhase;

extern s8 cmpStatusWindowHidden;

extern s8 cmpModelInitFlag;

extern u16 cmpStatusGroup;

extern s32 cmpModelHandle;

extern fclRibbon_t *g_pdebug;

extern void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                           const u32 *colors, s32 surfaceIndex);

extern s32 cmpGetCursorSlotValue(s8 index, cmpCursorSlot_t *slot);

extern f32 dds3ScreenParam[];

extern s32 cmpStatusModelResult;

extern s8 cmpParamGaugeColors[];

extern char *cmpParamNames[];

extern s8 dds3PadButtonState[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);

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






extern u8 *GBWK_0;

extern u8 *GBWK_1;

extern u8 *GBWK_3;

#include "sdk_libc.h"

extern void sdfTexFree(s32 aHandle);

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

extern void sdfDrawTagLinkRefGif(void *aTag, addr_t aPacket);

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



extern cmpModelParam_t cmpModelParamTable[];

extern s32 dds3FadeCheck(void);

extern s8 gProcessStat_4;

extern s8 campExitState;

extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);

extern void campDrawText(s32 id, s32 *colors);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void mdlSetRotate(void *aHandle);




extern saddr_t PlayingNo;

extern s32 fclChkMessage(void);

extern void cmpReturnParamDH(void);

extern s32 cmpUnloadStatusModel(void);

typedef s32 (*cmpSeqFunc_t)(void);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern char shpProcName[];

extern char ragCalcProcName[];

extern char rcvCalcProcName[];

extern char rstProcName[];

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

extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

extern u32 fclBlendColor(u32 color, u32 mask, u32 alpha);

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

extern cmpMsgEntry_t cmpItemMsgTable[];

extern cmpMsgEntry_t cmpSkillMsgTable[];

extern campStatusPanel_t *campDhUpdateWork;

extern s8 cmpGetHeartsIconId(s8 index);

extern void cmpInitCursorRange(void *cur, u8 count, u8 visible, saddr_t layout);

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern void cmpModelFreeResource(void);

extern void cmpResetDrawEnv(void);

extern void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor);

extern void cmpUpdateTerminate(cmpInfoWindow_t *window);

extern s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

extern void func_0023B500(s32 value);

extern s32 cmpGetPressedButton(s32 mask);

extern s32 cmpLoadStatusModel(void);

extern void cmpPlayCursorSE(u32 button);

extern s32 cmpChkSeqInfo(cmbGbWork_t *work);

extern void cmpUpdateStatusModelLoad(u16 id);

extern s32 frDrawFontOt(frFontHandle_t *frq, s8 mode, u32 arg);

extern s32 D_00345D88[];
#define D_00345D9C ((void **)(D_00345D88 + 5))

extern fclSpriteTbl_t fclSpriteTable[];

extern char shpFmtCount7[];

extern void cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 unused,
                                   s8 mode, s32 alpha, s32 ot);

extern s32 cmpModelSetup(u16 a, s32 b);

extern s32 cmpChkPadPressUpDown(void);

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char rcvNumFormat[];

extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);

extern char trmProcessNameTrmFmt[];

extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                               const char *text, frFontHandle_t *previous);

extern s32 frDrawFontOt(frFontHandle_t *frq, s8 mode, u32 arg);

extern void fmProcTrigSequence(s32 mode);


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

extern void *sdfMemLock(void *aHandle);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

/* Called above its definition, where ee-gcc assumed int. */
u16 cmpGetStatusModelId(void);

extern s32 cmpModelSetup(u16 a, s32 b);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *dst, datUnitWork_t *src);

extern char sndPathPrefix[]; /* "/sound/" */

extern s8 sndInitDone;

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

extern const char cmpCampTmxPath[];

void cmpTexLoad(void)
{
    D_00345D88[0] = fclLoadTexture(cmpCampTmxPath);
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

/* The final add shares the preceding slot publication. Each arm captures the
 * current work root after its callback; derive the list only for its consumer. */
extern void cmbClearStockEntry(cmbUnit_t *unit);
extern s32 cmbAddDevilToStock(u16 devilId, cmbUnit_t *list, u8 count);
extern void fclEncycDebugRegist(void);
extern void evtBitOn(s32 bit);

void campDebugRegistAll(void)
{
    s32 slot;
    s32 i;
    u16 lastDevil;
    u32 registered;
    dds3GlobalWork_t *nextWork;

    if (dds3PadButtonState[0x28] != 0) {
        return;
    }

    DDS3_GBWK->maxstock = 12;
    for (i = 1; i < 16; i++) {
        cmbClearStockEntry(&DDS3_GBWK->party[i]);
        DDS3_GBWK->stock[i] = 0;
    }
    registered = 1;
    DDS3_GBWK->stockCount = 1;

    if (DDS3_GBWK->party[0].unit.skillcnt == 0) {
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 1;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 2;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 3;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 4;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 5;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 6;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 7;
        DDS3_GBWK->party[0].unit.skill[DDS3_GBWK->party[0].unit.skillcnt++] = 8;
    }

    if (dds3PadButtonState[0x2A] != 0) {
        slot = cmbAddDevilToStock(0x4B, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x1F, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x01, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x8A, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x8B, &DDS3_GBWK->party[4], 12);
        nextWork = DDS3_GBWK;
        lastDevil = 0x6D;
    } else {
        slot = cmbAddDevilToStock(0x35, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x4E, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x35, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x01, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x50, &DDS3_GBWK->party[4], 12);
        DDS3_GBWK->party[4 + slot].unit.flag = registered;
        slot = cmbAddDevilToStock(0x0B, &DDS3_GBWK->party[4], 12);
        nextWork = DDS3_GBWK;
        lastDevil = 0x9A;
    }

    DDS3_GBWK->party[4 + slot].unit.flag = registered;
    slot = cmbAddDevilToStock(lastDevil, &nextWork->party[4], 12);
    DDS3_GBWK->party[4 + slot].unit.flag = registered;
    fclEncycDebugRegist();

    if (dds3PadButtonState[0x29] != 0) {
        return;
    }
    evtBitOn(0xA);
    evtBitOn(0x25);
    evtBitOn(0x28);
    evtBitOn(0x915);
}

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

extern u8 itfMesEmbedded_345160[];
extern char *cmpCursorCells[];
extern char *cmpPage0Cells[];
extern char *cmpPage1Cells[];
extern char *cmpPage2Cells[];
/* Four-byte cursor-flag storage in small BSS. */
extern u8 cmpPage2CursorFlags[4];
extern void cmpInitSeqInfo(cmpSeqInfo_t *work, cmpSeqInfo_t *parent);
extern s8 fclInitMessage(void *data);
extern void cmpSetCursorFlag(cmpCursorInfo_t *cursor, saddr_t flags);
extern void cmpDebugDumpCount(void);
extern void camp_H_cmpGlobalWork_t__ctor(void);

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, size_t c);
#endif

void cmpinit(void)
{
    void *allocation = sdfMemAllocDirect(0x8540);
    campWork = sdfMemLock(allocation);
    memset(campWork, 0, 0x8540);
    campWork->allocation = allocation;
    cmpInitSeqInfo((cmpSeqInfo_t *)campWork, NULL);
    fclInitMessage(itfMesEmbedded_345160);
    fclSetSelMessagePos(1);
    campWork->unk_0001 = 0;
    campWork->unk_0004 = 1;
    cmpInitCursorRange((u8 *)campWork + 0x1C, 7, 7, EE_ADDR_S(cmpCursorCells));
    cmpInitCursorLayout((u8 *)campWork + 0x1C, 0, 0, 0, 0, 0x17);
    cmpInitCursorRange(&campWork->page[0].cursor, 4, 4, EE_ADDR_S(cmpPage0Cells));
    cmpInitCursorLayout(&campWork->page[0].cursor, 0, 0, 0, 0, 0x17);
    cmpInitCursorRange(&campWork->page[1].cursor, 3, 3, EE_ADDR_S(cmpPage1Cells));
    cmpInitCursorLayout(&campWork->page[1].cursor, 0, 0, 0, 0, 0x17);
    cmpInitCursorRange(&campWork->page[2].cursor, 2, 2, EE_ADDR_S(cmpPage2Cells));
    cmpInitCursorLayout(&campWork->page[2].cursor, 0, 0, 0, 0, 0x17);
    cmpSetCursorFlag(&campWork->page[2].cursor, EE_ADDR_S(cmpPage2CursorFlags));
    cmpDebugDumpCount();
    camp_H_cmpGlobalWork_t__ctor();
    campWork->effectStep = SDF_FRAMES(0x2B3);
    cmpInitStockTable();
    cmpLoadState = 0;
}

extern void rstSetPanelEffFlag(void);

#ifdef NON_MATCHING
extern void campHideStatusWindow(void);
extern void cmpSetStatusWindowHidden(void);
#endif

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

extern char cmpCalcProcName[];

extern const char campDrawProcName[];

extern const char campUpdateProcName[];

extern void cmpinit();

extern s32 cmpCalcSequence(void);

extern s32 camp_draw_Process(void);

extern s32 camp_update_Process(void);

extern void *cmpStartProcessPanelEff(s32 parent);

#ifdef NON_MATCHING
extern void *cmpStartProcessStock(cmpLocalStock_t *stock, s32 value);
#endif

void cmpProcessStart(void)
{
    cmpinit();
    dds3StartProcess(cmpCalcProcName, 0x3F2, 1, 0, cmpCalcSequence, NULL, NULL);
    dds3StartProcess(campDrawProcName, 0x2B07, 1, 0, camp_draw_Process, NULL, NULL);
    dds3StartProcess(campUpdateProcName, 0x520B, 1, 0, camp_update_Process, camp_update_Shutdown, NULL);
    cmpStartProcessStock(&campWork->stock[0], 0);
    cmpInitLocalStock();
    cmpInitDrawList();
    cmpStartProcessPanelEff(0);
    campExitState = 1;
}

extern char cmpCalcProcName[];

extern const char campDrawProcName[];

extern const char campUpdateProcName[];

void campKillProcesses(void)
{
    dds3KillProcessName(cmpCalcProcName, 0);
    dds3KillProcessName(campDrawProcName, 0);
    dds3KillProcessName(campUpdateProcName, 0);
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
extern s32 cmpFindFreeStockSlot(s8 index, s8 room);
extern s32 cmpStockHasRoom(cmpStockWork_t *work, s8 index);

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

extern s32 cmpFindFreeStockSlot(s8 index, s8 room);

/* NON_MATCHING: 12 of 32 words differ (first at +0x50). */
#ifdef NON_MATCHING
/* Called above its definition, where ee-gcc assumed int. */
s32 cmpStockHasRoom(cmpStockWork_t *work, s8 index);

extern void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot);

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

#ifdef NON_MATCHING
extern s32 cmpStockEffIsActive(u8 *work);
extern u16 cmpChkSkillBad(u16 skill);
extern s32 cmpAddQuePanelEff(s32 x, s32 y, s32 z, u16 width, u16 height, u8 step,
                             u8 delay, s32 color0, s32 color1, u8 kind);
extern s32 cmpSetPanelEffParam(s32 index, s32 value);

void cmpSetStockRecoverEff(s8 effect, u16 skill, cmpLocalStock_t *stock)
{
    s32 count = cmpStockEffIsActive((u8 *)stock);
    s32 index = cmpGetStockInfoIndex(stock, 1, 1);
    s32 room = cmpStockHasRoom((cmpStockWork_t *)stock, 1);
    s8 slot = 0;
    s32 position;
    s32 i;

    if (count == 1) {
        slot = cmpFindFreeStockSlot(index, room);
    } else if (count == 4) {
        room = 1;
    }
    position = slot * 0x18;
    for (i = 0; i < count; i++) {
        s32 y = position + (room != 0 ? 0x24 : 0x8F);
        s32 panel;

        if (cmpChkSkillBad(skill) & 0x800) {
            cmpStockWork_t *work = (cmpStockWork_t *)stock;
            u8 *base = (u8 *)work;
            s32 offset = (u8 *)&work->info[room != 0 ? 0 : 1].rows[0].slot - base;
            cmpCursorSlot_t *cursor = (cmpCursorSlot_t *)(base + offset +
                slot * sizeof(work->info[0].rows[0]));

            cmpSetCursorSlot(0, 3, 0x10, cursor);
            panel = cmpAddQuePanelEff(0xE20, y * 8, 0xFFFFFF, 0x118, 0x15,
                                      0x20, i, 0x75A8FF80, 0x75A8FF00, 2);
        } else if (cmpChkSkillBad(skill) & 0xFFF) {
            panel = cmpAddQuePanelEff(0xE20, y * 8, 0xFFFFFF, 0x118, 0x15,
                                      0x20, i, 0xCD67FF80, 0xCD67FF00, 1);
        } else {
            s32 xOffset, width, color0, color1;
            u8 step;

            switch (effect) {
            case 3:
                xOffset = 0; width = 0x1E; step = 0x20;
                color0 = 0xAD610580; color1 = 0xAD610500;
                break;
            case 4:
                xOffset = 0; width = 0; step = 8;
                color0 = 0x00436580; color1 = 0x00436580;
                break;
            case 5:
                xOffset = 0; width = 0; step = 0x20;
                color0 = 0xFF692F80; color1 = 0xFF7D2F00;
                break;
            default:
                xOffset = 0x1E; width = 0x3C; step = 0x20;
                color0 = 0x7CFF6780; color1 = 0x7CFF6700;
                break;
            }
            panel = cmpAddQuePanelEff((0xE2 - xOffset) * 16, y * 8, 0xFFFFFF,
                                      0x118 + width, 0x15, step, i,
                                      color0, color1, effect);
        }
        cmpSetPanelEffParam(panel, slot);
        if (count != 1) {
            position += 0x18;
            slot++;
            if (slot >= 4) {
                room = 0;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpSetStockRecoverEff);
#endif

extern void datAddItem(s32 id, s32 num);

extern s32 datCheckItem(s32 id, s32 num);

s32 cmpDestroyItem(s8 all)
{
    u16 id = campWork->itemId[campWork->itemCursor + campWork->itemScroll];

    if (all == 0)
    {
        if ((datItemTable[id].flags2 & 4) != 0)
        {
            datAddItem(id, -1);
        }
    }
    else
    {
        datAddItem(id, -DDS3_GBWK->stocklist[id + 4]);
    }
    return datCheckItem(id, 1) != 0;
}

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

    return &DDS3_GBWK->party[index].unit;
}

u16 cmpGetSelectItemID(void)
{
    return campWork->itemId[campWork->itemCursor + campWork->itemScroll];
}

/* Encoded player name and glyph count written by frInitNameData. */
typedef struct {
    u8 reserved00[0x6604];
    u8 nameBytes[0x50];
    u8 nameLength;
} cmpSavedNameView_t;

extern char *text2[];
extern void fclSetMessageVar_20743088(s8 index, saddr_t value);

#ifdef NON_MATCHING
extern void *memcpy(void *a, const void *b, size_t c);
#ifdef FIX_BUGS
extern void *memmove(void *a, const void *b, size_t c);
#endif
#endif

void cmpSetItSkMessageVar(datUnitWork_t *unit)
{
    char name[((cmpSavedNameView_t *)DDS3_GBWK)->nameLength * 2 + 1];

    memcpy(name, ((cmpSavedNameView_t *)DDS3_GBWK)->nameBytes,
           ((cmpSavedNameView_t *)DDS3_GBWK)->nameLength * 2);
    name[((cmpSavedNameView_t *)DDS3_GBWK)->nameLength * 2] = 0;
    if (unit->id != 0) {
        fclSetMessageVar_20743088(0, EE_ADDR_S(text2[unit->id]));
    } else {
        fclSetMessageVar_20743088(0, EE_ADDR_S(name));
    }
}

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
    cmpMsgEntry_t *entry = cmpSkillMsgTable;
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
    cmpMsgEntry_t *entry = cmpItemMsgTable;
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

#ifdef NON_MATCHING
extern s32 evtBitCheck(s32 no);
#endif

void cmpCreateLocalItem(u8 mode)
{
    s32 i;

    memset(campWork->itemId, 0, sizeof(campWork->itemId));
    campWork->unk_8302 = 0;
    campWork->itemMode = mode;
    switch (mode) {
    case 0:
        for (i = 1; i < 0x60; i++) {
            if (DDS3_GBWK->itemQuantity[i] != 0) {
                campWork->itemId[campWork->unk_8302++] = i;
            }
        }
        break;
    case 1:
        for (i = 0x60; i < 0x70; i++) {
            if (DDS3_GBWK->itemQuantity[i] != 0) {
                campWork->itemId[campWork->unk_8302++] = i;
            }
        }
        break;
    default:
        for (i = 0; i < 0x30; i++) {
            if (evtBitCheck(i + 0x3D0) != 0) {
                campWork->itemId[campWork->unk_8302++] = i + 0x70;
            }
        }
        break;
    }
}

extern s32 fclStartMessage(s32 message);
extern s32 cmpClearButtonGuide(s32 skill, u32 select);
extern s32 cmpStartSkillMsg(u16 skill, s32 errorFlags);
extern s32 cmpGetKugatatiMsg(void);
extern s32 fldEsutoMaChk(void);
extern s32 fldRiberaMaChk(void);
extern s32 fldRifutoMaChk(void);
extern s32 fldLightMaChk(void);

#ifdef NON_MATCHING
extern s32 fclStartMessage(s32 message);
#endif

s32 cmpStartSkillTokusyuMsg(u16 skill)
{
    s32 msg = 0x20;

    switch (skill) {
    case 0x49:
        if (fldEsutoMaChk() != 0) {
            msg = 0x21;
        }
        break;
    case 0x4A:
        if (fldRiberaMaChk() != 0) {
            msg = 0x21;
        }
        break;
    case 0x4B:
        if (fldRifutoMaChk() != 0) {
            msg = 0x21;
        }
        break;
    case 0x4C:
        switch (fldLightMaChk()) {
        case -1:
            msg = 0x22;
            break;
        case 1:
            msg = 0x21;
            break;
        }
        break;
    }
    if (msg != 0x20) {
        fclStartMessage(msg);
        return 1;
    }
    return 0;
}


s32 cmpStartItemTokusyuMsg(u16 item)
{
    return cmpStartSkillTokusyuMsg(datItemTable[item].skill);
}

extern s32 cmpClearButtonGuide(s32 skill, u32 select);

s32 cmpStartSkillMsg(u16 skill, s32 errorFlags)
{
    s32 guide = cmpClearButtonGuide(skill, errorFlags);
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
    /* Keep the ordinary message path first and share its mode/result return. */
    if (cmpStartSkillTokusyuMsg(skill) == 0)
    {
        index = cmpFindSkillMsgIndex(mode, skill);
        if (index > 0)
        {
            if (mode == 0) {
                fclStartMessage(index + 0xC);
            } else {
                fclStartMessage(index + 0x20);
            }
        }
    }
    else
    {
        mode = 2;
    }
    return mode;
}


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
    msg = 26;
    if (diff >= 2)
    {
        msg = 0;
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

#ifdef NON_MATCHING
s32 cmpStartItemMsg(u16 item, s32 errorFlags)
{
    u16 skill = datItemTable[cmpGetSelectItemID()].skill;
    s32 guide;
    s32 mode;
    s32 index;

    if (item == 0x3A) {
        fclStartMessage(cmpGetKugatatiMsg());
        return 0;
    }
    guide = cmpClearButtonGuide(skill, errorFlags);
    mode = guide > 0 && guide < 5;
    if (cmpStartItemTokusyuMsg(item) != 0) {
        return 2;
    }
    index = cmpFindItemMsgIndex(mode, item);
    if (index > 0) {
        if (mode == 0) {
            fclStartMessage(index + 0x13);
        } else {
            fclStartMessage(index + 0x20);
        }
        return mode;
    }
    if (skill != 0) {
        return cmpStartSkillMsg(skill, errorFlags);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpStartItemMsg);
#endif

extern s32 datGetItemSkillNo(s32 itemid);
extern s32 cmpStartItemMsg(u16 item, s32 errorFlags);
extern s32 cmpUseItemTokusyu(u16 item, saddr_t target, saddr_t user);
extern void cmpRecover(u16 skill, datUnitWork_t *stock, datUnitWork_t *target);
#ifdef NON_MATCHING
extern void cmpSetStockRecoverEff(s8 effect, u16 skill, cmpLocalStock_t *stock);
#else
extern void cmpSetStockRecoverEff(s32 effect, s32 skill, cmpLocalStock_t *stock);
#endif
extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);

s32 cmpUseItem(void)
{
    u16 itemId = cmpGetSelectItemID();
    datUnitWork_t *stock = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    datUnitWork_t *target = cmpGetSelectDstStock();
    u16 skill = datGetItemSkillNo(itemId);
    u16 currentItem;
    s32 errorFlags;
    s32 message;
    s32 effect;

    cmpSetItSkMessageVar(target);
    errorFlags = cmpGetItSkErrSelFlag(skill);
    currentItem = cmpGetSelectItemID();
    message = cmpStartItemMsg(currentItem, errorFlags);
    switch (message) {
    case 1:
        p2sdPlaySE(0xD, 0x7F, 0x3F);
        return 2;
    case 2:
        return message;
    default:
        break;
    }
    effect = cmpUseItemTokusyu(itemId, EE_ADDR_S(stock), EE_ADDR_S(target));
    if (effect == 0 && skill != 0) {
        cmpRecover(skill, stock, target);
    }
    cmpSetStockRecoverEff(effect, skill, campWork->stock);
    campWork->unk_8303 = 1;
    return cmpDestroyItem(0) != 0;
}

u16 cmpGetSelectSkillID(void)
{
    datUnitWork_t *unit =
        &DDS3_GBWK->party[cmpGetStockInfoIndex(campWork->stock, 0, 0)].unit;

    return unit->skill[campWork->itemCursor + campWork->itemScroll];
}

extern s32 cmpAddCost(u16 skill, datUnitWork_t *unit);
extern s32 cmpGetSkillEntry(u16 skill, datUnitWork_t *unit);
extern s32 cmpStartSkillMsg(u16 skill, s32 errorFlags);
extern s32 cmpUseSkillTokusyu(u16 skill, datUnitWork_t *user, saddr_t target);

s32 cmpUseSkill(void)
{
    u16 skill = cmpGetSelectSkillID();
    datUnitWork_t *user = cmpGetSelectSrcStock();
    datUnitWork_t *target = cmpGetSelectDstStock();
    s32 errorFlags;
    u16 messageSkill;
    s32 messageMode;
    s32 effect;

    cmpSetItSkMessageVar(target);
    errorFlags = cmpGetItSkErrSelFlag(skill);
    messageSkill = cmpGetSelectSkillID();
    messageMode = cmpStartSkillMsg(messageSkill, errorFlags);
    switch (messageMode) {
    case 1:
        p2sdPlaySE(0xD, 0x7F, 0x3F);
        return 2;
    case 2:
        return messageMode;
    default:
        break;
    }
    if (cmpAddCost(skill, user) == 0) {
        return 0;
    }
    effect = cmpUseSkillTokusyu(skill, user, EE_ADDR_S(target));
    if (effect == 0) {
        cmpRecover(skill, user, target);
    }
    cmpSetStockRecoverEff(effect, skill, campWork->stock);
    return cmpGetSkillEntry(skill, user) != 0;
}

s32 cmpChkDoNotUseSkill(u16 skill)
{
    s32 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);
    u8 use;
    u32 normalSkill;

    if (cmpGetSkillEntry(skill, (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index) == 0) {
        return -1;
    }
    use = datSkillTable.normal[skill].use & 1;
    if (use == 0) {
        return 1;
    }
    normalSkill = skill < 0x120;
    /* Target-specific constraint: keep the live comparison result opaque to
     * boolean-range folding, preserving retail's sltiu logical negation.
     * This emits no instruction and does not assert original source spelling. */
    FAKEMATCH_ASM_LIVE(normalSkill);
    return normalSkill == 0;
}

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

#ifdef NON_MATCHING
extern void cmpCompactStockList(s8 devilId);
extern void cmpRemoveStockSlot(s8 index);
extern void cmpSetStockFlag(datUnitWork_t *work);
extern void datAllClearCommMemory(void);
extern void datClearCommMemory(datUnitWork_t *work);
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
    cmpClearStockFlag(&DDS3_GBWK->party[index].unit);
    datClearCommMemory(&DDS3_GBWK->party[index].unit);
    datAllClearCommMemory();
}

extern s32 func_0022E168(u16 devilId);

void cmpRemoveStockDevilEntry(void)
{
    s8 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);

    if (index < 0) {
        return;
    }
    func_0022E168(DDS3_GBWK->party[index].unit.id);
}

#ifdef NON_MATCHING
extern void cmpRebuildStockList(void);
#endif

s32 func_0022E168(u16 devilId)
{
    s32 i;
    s32 result;
    s32 stockIndex = 0;
    s32 found = 0;
    s32 removeIndex = 0;

    for (i = 0; i < DDS3_GBWK->stockCount; ++i) {
        removeIndex = i;
        stockIndex = (u8)DDS3_GBWK->stock[i];
        if (DDS3_GBWK->party[stockIndex].unit.id == devilId) {
            found = 1;
            break;
        }
    }
    result = 0;
    if (found != 0) {
        memset(&DDS3_GBWK->party[stockIndex].unit, 0, sizeof(datUnitWork_t));
        cmpRemoveStockSlot((s8)removeIndex);
        cmpRebuildStockList();
        result = 1;
    }
    return result;
}

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

    rows = campWork->stock[0].PartyCnt + (u8)campWork->stock[0].StockCnt;
    cmpInitCursorRange(campWork->unk_6230, (u8)rows, (u8)rows, 0);
    rows = campWork->stock[1].PartyCnt + (u8)campWork->stock[1].StockCnt;
    cmpInitCursorRange((u8 *)campWork + 0x726C, (u8)rows, (u8)rows, 0);
}

/* Target-specific allocation aid for the real stock-count accumulator; this
 * does not imply a hardware register requirement or original source spelling.
 * The binding ends before each call. See docs/matching/code_0022C578.md. */
extern void cmpChangeCursorListNums(s32 count, u8 *cursor);

void cmpReCalcStockCursorPos(s8 index)
{
    if (index == 0 || index == 2) {
        s32 count;
        {
            s32 partyCount = campWork->stock[0].PartyCnt;
            FAKEMATCH_ASM_REG(s32, stockCount, "$4") = (u8)campWork->stock[0].StockCnt;
            stockCount = partyCount + stockCount;
            count = stockCount & 0xFF;
        }
        cmpChangeCursorListNums(count, &campWork->unk_6230[0]);
    }
    if (index == 1 || index == 2) {
        s32 count;
        {
            s32 partyCount = campWork->stock[1].PartyCnt;
            FAKEMATCH_ASM_REG(s32, stockCount, "$4") = (u8)campWork->stock[1].StockCnt;
            stockCount = partyCount + stockCount;
            count = stockCount & 0xFF;
        }
        cmpChangeCursorListNums(count, &campWork->unk_6230[0x103C]);
    }
}

void cmpInitRootCursorPos(void)
{
    cmpResetCursorInfo(&campWork->page[0].cursor);
    cmpResetCursorInfo(&campWork->page[1].cursor);
    cmpResetCursorInfo(&campWork->page[2].cursor);
}

void cmpInitSelectCursor(s32 index)
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
        return (u8)stock.PartyCnt + (u8)stock.StockCnt;
    }
    return stock.StockCnt;
}

#ifdef NON_MATCHING
extern void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func, void *data, s32 value, s32 alpha);
extern void cmpSetCrossGeneralVal(u8 *work, s8 value);
#endif

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

extern void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func,
                                 void *data, s32 value, s32 alpha);
extern void cmpSetCrossGeneralVal(u8 *work, s8 value);
extern void cmpDrawSkillList(s32 *position, cmpCursorInfo_t *cursor,
                             datUnitWork_t *unit, s8 mode, s32 alpha);

const char campDrawProcName[16] = "camp_draw";
const char campUpdateProcName[16] = "camp_update";
void cmpCalcSkill(void)
{
    u32 index = (u8)cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);
    datUnitWork_t *unit;
    s32 cursor;
    s32 oldCount;
    s32 newCount;
    s8 selected;
    s32 i;

    if (campWork->unk_0004 != 0) {
        switch (campWork->previousPhase) {
        case 0:
        case 2:
            cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 1);
            campWork->unk_0001 = 8;
            cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0,
                                cmpUpdateListSelection,
                                (u8 *)campWork + 0x1C, 0, 0);
            cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 1);
            break;
        case 8:
            if (campWork->unk_0004 == 1) {
                unit = &DDS3_GBWK->party[index].unit;
                cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0, NULL, NULL, 0, 0);
                campWork->unk_0001 = 10;
                cmpInitSelectCursor((s8)unit->skillcnt);
                cmpInitSkillCursor(unit);
                cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0x120, 0x288,
                                    cmpDrawSkillList, campWork->unk_410C,
                                    &DDS3_GBWK->party[index].unit, 0);
            } else {
                cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                campWork->unk_0001 = 0;
                cmpInitStockCursorPos();
            }
            break;
        case 9:
            if (campWork->unk_0004 == 2) {
                cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                campWork->unk_0001 = 10;
            } else {
                if (cmpUseSkill() != 0) {
                    campWork->unk_0001 = 9;
                } else {
                    campWork->unk_0001 = 10;
                    cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0x120, 0x288,
                                        cmpDrawSkillList, campWork->unk_410C,
                                        &DDS3_GBWK->party[index].unit, 0);
                    cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                }
            }
            break;
        case 10:
            if (campWork->unk_0004 == 1) {
                if (cmpGetSkillTargetArea(cmpGetSelectSkillID()) == 0) {
                    cmpUseSkill();
                    campWork->unk_0001 = 10;
                } else {
                    cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 1);
                    campWork->unk_0001 = 9;
                }
            } else {
                cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0,
                                    cmpUpdateListSelection,
                                    (u8 *)campWork + 0x1C, 0, 0);
                cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 1);
                campWork->unk_0001 = 8;
            }
            break;
        }
        campWork->previousPhase = 2;
    }
    if (campWork->unk_0001 == 9) {
        cmpSetSkillTSL(cmpGetSelectSkillID());
        return;
    }
    /* Preserve the current list selection across the rebuild. */
    cursor = ((cmpStockWork_t *)&campWork->stock[0])->info[0].unk_0E;
    oldCount = campWork->stock[0].PartyCnt + (s8)campWork->stock[0].StockCnt;
    selected = campWork->stock[0].ListIdx[
        cursor + ((cmpStockWork_t *)&campWork->stock[0])->info[0].unk_10];
    campWork->stock[0].count = 0xF;
    cmpBuildStockNameList(&campWork->stock[0]);
    cmpReCalcStockCursorPos(0);
    newCount = campWork->stock[0].PartyCnt;
    newCount += (s8)campWork->stock[0].StockCnt;
    if (newCount != oldCount) {
        for (i = 0; i < newCount; i++) {
            if (campWork->stock[0].ListIdx[i] == selected) {
                ((cmpStockWork_t *)&campWork->stock[0])->info[0].unk_10 = i;
                break;
            }
        }
    }
}

extern char *D_00330914[];

extern char cmpFmtNameCount[];

extern char *pStr[];

extern s32 cmpItemHasFlag(u16 item, s8 which);

#ifdef NON_MATCHING
extern s32 fclStartSelMessage(s32 message);

extern void fclSetMessageInvalid(s8 mode);
#endif

s32 cmpCalcDestroyItem(void)
{
    char buf[16];
    u16 id = campWork->itemId[campWork->itemCursor + campWork->itemScroll];

    if (cmpItemHasFlag(id, 1) == 0)
    {
        return 0;
    }
    campWork->stockMessagePhase = 0;
    fclSetMessageVar_20743088(2, EE_ADDR_S(pStr[id]));
    sprintf(buf, cmpFmtNameCount, D_00330914[0], DDS3_GBWK->stocklist[id + 4]);
    fclSetMessageVar_20743088(0xB, EE_ADDR_S(buf));
    fclSetMessageInvalid(0);
    fclStartMessage(9);
    fclStartSelMessage(0xB);
    return 1;
}

#ifdef NON_MATCHING
extern void cmpCalcItemEntry(fclPanelPos_t *pos, cmpCursorInfo_t *cursor, s32 context,
                             s8 flag, s32 alpha, s32 ot);

s32 cmpCalcItem(void)
{
    s8 status = campWork->unk_0004;
    s8 phase;
    s32 result = 0;

    if (status != 0) {
        phase = campWork->previousPhase;
        switch (phase) {
        case 0:
            cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0,
                                cmpUpdateListSelection,
                                &campWork->page[0].cursor, 0, 0);
            break;
        case 1:
            cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0, NULL, NULL, 0, 0);
            if (campWork->unk_0004 == phase) {
                switch (campWork->page[0].cursor.unk_22 +
                        (u8)campWork->page[0].cursor.unk_24) {
                case 0:
                case 1:
                    cmpCreateLocalItem(0);
                    break;
                case 2:
                    cmpCreateLocalItem(1);
                    break;
                case 3:
                    cmpCreateLocalItem(2);
                    break;
                }
                campWork->unk_0001 = 0xB;
                cmpInitSelectCursor((s8)campWork->unk_8302);
                cmpSetAfterCrossFunc(campWork->crossPanel, 0x120, 0x288,
                                    cmpCalcItemEntry,
                                    campWork->unk_410C, 0, 0);
            } else {
                campWork->unk_0001 = 0;
                cmpInitStockCursorPos();
            }
            break;
        case 11:
            if (status == 1) {
                switch (campWork->page[0].cursor.unk_22 +
                        (u8)campWork->page[0].cursor.unk_24) {
                case 0:
                    if (cmpGetSkillTargetArea(datItemTable[cmpGetSelectItemID()].skill) == 0) {
                        cmpUseItem();
                        campWork->unk_0001 = phase;
                        return 1;
                    }
                    campWork->unk_0001 = 9;
                    break;
                case 1:
                    if (cmpCalcDestroyItem() == 0) {
                        return 1;
                    }
                    campWork->unk_0001 = 0x10;
                    campWork->unk_0004 = status;
                    result = 1;
                    break;
                case 2:
                case 3:
                    campWork->unk_0001 = phase;
                    return 0;
                }
                cmpSetCrossGeneralVal(campWork->crossPanel, 1);
                if (result != 0) {
                    return 1;
                }
            } else {
                cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0,
                                    cmpUpdateListSelection,
                                    &campWork->page[0].cursor, 0, 0);
                campWork->unk_0001 = 1;
            }
            break;
        case 9:
            if (status == 2) {
                campWork->unk_0001 = 0xB;
                cmpSetCrossGeneralVal(campWork->crossPanel, 0);
            } else {
                if (cmpUseItem() != 0) {
                    campWork->unk_0001 = phase;
                } else {
                    campWork->unk_0001 = 0xB;
                    cmpSetAfterCrossFunc(campWork->crossPanel, 0x120, 0x288,
                                        cmpCalcItemEntry,
                                        campWork->unk_410C, 0, 0);
                    cmpSetCrossGeneralVal(campWork->crossPanel, 0);
                }
            }
            break;
        }
        campWork->previousPhase = 1;
    }
    if (campWork->unk_0001 == 9) {
        cmpSetItemTSL(cmpGetSelectItemID());
    } else {
        campWork->stock[0].count = 0xF;
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(0);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpCalcItem);
#endif

s32 cmpSetPartyDSL(void)
{
    switch (campWork->page[1].cursor.unk_22 + (u8)campWork->page[1].cursor.unk_24)
    {
    case 0:
        if (cmpStartStockMsg(0) == 0) {
            return 0;
        }
        campWork->stock[0].count = 0xA;
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(2);
        break;
    case 1:
        if (camp_H_cmpLocalStock_t__ctor() == 0) {
            return 0;
        }
        campWork->stock[0].count = 0x19;
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(2);
        break;
    case 2:
        if (cmpStartStockMsg(1) == 0) {
            return 0;
        }
        campWork->stock[0].count = 0x8B;
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(2);
        break;
    }
    return 1;
}

#ifdef NON_MATCHING
extern void cmpMoveStockCursor(s8 index, s8 dir, u8 *base);
#endif

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

extern void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func,
                                 void *data, s32 value, s32 alpha);
extern void cmpSetCrossGeneralVal(u8 *work, s8 value);
extern s32 fclStartMessage(s32 message);
extern s32 fclStartSelMessage(s32 message);
extern void fclSetMessageInvalid(s8 mode);

s32 cmpCalcParty(void)
{
    s32 selectedIndex = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);
    s8 status;
    s8 phase;

    status = campWork->unk_0004;
    if (status != 0) {
        phase = campWork->previousPhase;
        switch (phase) {
        case 0:
            cmpSetAfterCrossFunc((u8 *)campWork + 0x8448, 0, 0,
                                cmpUpdateListSelection,
                                (u8 *)campWork + 0x2094, 0, 0);
            break;
        case 3:
            break;
        case 8: {
            s32 selectedCount;

            if (status == 1) {
                selectedCount = campWork->page[1].cursor.unk_22 +
                                (u8)campWork->page[1].cursor.unk_24;
                switch (selectedCount) {
                case 0:
                    if (camp_H_cmpLocalStock_t__ctor() == 3) {
                        campWork->unk_0001 = 8;
                        goto finish;
                    }
                    /* Fall through. */
                case 1:
                    if (cmpCalcStockControl() != 0) {
                        campWork->unk_0001 = 8;
                    } else {
                        cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                        campWork->unk_0001 = 3;
                    }
                    goto finish;
                default:
                    goto finish;
                case 2:
                    break;
                }

                if ((DDS3_GBWK->party[selectedIndex].unit.flag & 1) == 0) {
                    campWork->unk_0001 = 8;
                    goto finish;
                }
                if ((DDS3_GBWK->party[selectedIndex].unit.flag & 0x800) != 0) {
                    datUnitWork_t *unit = cmpGetSelectSrcStock();

                    fclSetMessageVar_20743088(0, EE_ADDR_S(text2[unit->id]));
                    fclStartMessage(0xA);
                    campWork->unk_0001 = 8;
                    goto finish;
                }

                campWork->stockMessagePhase = 0;
                fclSetMessageInvalid(0);
                fclStartMessage(6);
                fclStartSelMessage(0xB);
                campWork->unk_0001 = 0xE;
                campWork->unk_0004 = status;
                return 1;
            } else {
                cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                campWork->unk_0001 = 3;
            }
            break;
        }
        case 9:
            if (status == 1) {
                if (cmpCalcStockControl() != 0) {
                    campWork->unk_0001 = 8;
                } else {
                    cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 0);
                    campWork->unk_0001 = 3;
                }
            } else {
                campWork->unk_0001 = 8;
            }
            cmpSetPartyDSL();
            break;
        }

finish:
        if (campWork->unk_0001 == 3) {
            cmpInitLocalStock();
            cmpInitDrawList();
        }
        campWork->previousPhase = 3;
    }
    return 0;
}

extern void dds3ConfigProcessStart(s32 Mode);

#ifdef NON_MATCHING
extern void cmpSetStockVisible(s8 visible);
#endif

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

extern void cmpCreateLocalItem(u8 mode);

extern void cmpChangeCursorListNums(s32 count, u8 *cursor);

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

/* The cursor initializer only mutates slots; its return register is unused. */
extern void cmpSetIniStockRecoverEff(u8 *work);

#ifdef NON_MATCHING
extern s32 cmpCalcItem(void);
#endif

s32 cmpCalcSequence(void)
{
    s8 phase = campWork->unk_0001;
    s32 result = 0;

    switch (phase) {
    case 0:
        cmpCalcRoot();
        break;
    case 1:
        result = cmpCalcItem();
        break;
    case 2:
        cmpCalcSkill();
        break;
    case 3:
        result = cmpCalcParty();
        break;
    case 5:
        if (campWork->unk_0004 == 1) {
            if ((s8)campWork->statusPhase == 0) {
                cmpSetCrossGeneralVal(campWork->crossPanel, 1);
            } else {
                cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0, NULL, NULL, 0, 0);
                campWork->unk_8468 = 0x200;
            }
        }
        if (campWork->unk_0004 == 2 && (s8)campWork->statusPhase == 0) {
            cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0, cmpUpdateListSelection,
                                 (u8 *)campWork + 0x1C, 0, 0);
            cmpSetCrossGeneralVal(campWork->crossPanel, 1);
            break;
        }
        break;
    case 4:
        if (campWork->unk_0004 != 0) {
            cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0, NULL, NULL, 0, 0);
        }
        break;
    case 6:
        cmpCalcConfig();
        break;
    case 7:
        cmpCalcLoad();
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        break;
    case 12:
        if (campWork->unk_0004 == 1) {
            cmpSetAfterCrossFunc(campWork->crossPanel, 0, 0, NULL, NULL, 0, 0);
        }
        break;
    case 13:
        break;
    case 14:
        if (campWork->unk_0004 == 1 && (s8)campWork->stockMessagePhase == 1) {
            s8 index = cmpGetStockInfoIndex(campWork->stock, 0, 0);
            datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + index;

            cmpSetIniStockRecoverEff((u8 *)campWork->stock);
            fclSetMessageVar_20743088(0, EE_ADDR_S(text2[unit->id]));
            fclSetMessageInvalid(1);
            if ((unit->badstatus & 0x800) != 0) {
                fclStartMessage(8);
            } else {
                fclStartMessage(7);
            }
        }
        break;
    case 15:
    case 16:
        break;
    default:
        break;
    }
    cmpUpdateList();
    if (result != 0) {
        return 0;
    }
    campWork->unk_0004 = 0;
    if ((campWork->unk_0000 & 0x10) != 0) {
        cmpUpdateTerminate((cmpInfoWindow_t *)campWork);
        return -1;
    }
    return 0;
}

/* The gradient fan passes three fixed-point X and Y coordinates here.
 * Each vertex receives a separate distortion scale; X uses twice the Y scale. */
void nmeMakeGradColors(s32 *xs, s32 *ys, s32 delta0, s32 delta1, s32 delta2)
{
    s32 scale[2][3] = {
        { (s32)((f32)delta0 * 2.0f), (s32)((f32)delta1 * 2.0f), (s32)((f32)delta2 * 2.0f) },
        { (s32)(f32)delta0, (s32)(f32)delta1, (s32)(f32)delta2 }
    };
    s32 i;
    for (i = 0; i < 3; i++) {
        xs[i] += xs[i] * scale[0][i] / 0xA00;
        ys[i] += ys[i] * scale[1][i] / 0xA00;
    }
}

extern const s32 nmeQuadPanelCoords[];
extern void fclDrawPanelSprite2(s32 id, s32 ot);

void nmeDrawQuadPanel(s32 color0, s32 color1, s32 alpha, s32 ot)
{
    u32 coords[12];
    u32 colors[6];
    s32 xOffset = alpha << 4;
    s32 yOffset = alpha << 3;

    memcpy(coords, nmeQuadPanelCoords, sizeof(coords));
    colors[0] = color0;
    colors[1] = 0;
    colors[2] = 0;
    colors[3] = color1;
    colors[4] = 0;
    colors[5] = 0;

    fclDrawPanelSprite2(0x48, ot);
    coords[1] += xOffset;
    coords[5] += yOffset;
    cmpDrawTriG(coords, coords + 3, 0, colors, ot);
    coords[7] -= xOffset;
    coords[11] -= yOffset;
    cmpDrawTriG(coords + 6, coords + 9, 0, colors + 3, ot);
    fclDrawPanelSprite2(0x44, ot);
}

const char cmpTextWhatWillYouDiscard[24] __attribute__((aligned(16))) = "What will you discard?";
const char cmpTextWhoWillYouPartWith[24] = "Who will you part with?";
const char cmpTextWhoWillYouReturn[24] = "Who will you return?";
const char cmpTextWhoWillYouSummon[24] = "Who will you summon?";
const char cmpTextFromWhomWillYouExtract[32] = "From whom will you extract?";
const char cmpTextWhichWillYouUnequip[24] = "Which will you unequip?";
const char cmpTextWhichWillYouIngest[24] = "Which will you ingest?";
const char cmpTextSPITEMS[16] = "SP ITEMS";
const char cmpTextUseWhatItem[16] = "Use what item?";
const char cmpTextWhoWillUseIt[24] = "Who will use it?";
const char cmpTextWhoWillUseASkill[24] = "Who will use a skill?";
const char cmpTextWhatWillYouUse[24] = "What will you use?";
const char cmpTextMAINMENU[16] = "MAIN MENU";
const char cmpTextNoDemonsToExtractFrom[32] = "No demons to extract from";
const char cmpTextSummoned[16] = "Summoned";
const s32 nmeQuadPanelCoords[12] = {0, 8320, 0, 0, 0, 1600, 8192, 0, 8192, 3584, 3584, 1696};

#ifdef NON_MATCHING
extern const s32 nmeGradFaces[26][10];
extern const u32 nmeGradBaseColors[];
extern const f32 nmeGradWeights[];
extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

void nmeDrawGradQuad(s32 amount, s32 alpha, s32 ot)
{
    s32 faces[26][10];
    u32 baseColors[9];
    u32 colors[3];
    f32 weights[5];
    s32 coords[2][3];
    u32 i;
    s32 layer;
    s32 offset;
    s32 strength;
    f32 scale;

    memcpy(faces, nmeGradFaces, sizeof(faces));
    memcpy(baseColors, nmeGradBaseColors, sizeof(baseColors));
    memcpy(weights, nmeGradWeights, sizeof(weights));
    for (i = 0; i < 26; i++) {
        strength = amount;
        offset = 0;
        for (layer = 0; layer < 2; layer++) {
            if (layer != 0) {
                strength = strength * 180 / 256;
            }
            scale = strength;
            coords[0][0] = (faces[i][0] - offset) << 4;
            coords[0][1] = (faces[i][2] - offset) << 4;
            coords[0][2] = (faces[i][8] - offset) << 4;
            coords[1][0] = faces[i][1] << 3;
            coords[1][1] = faces[i][3] << 3;
            coords[1][2] = faces[i][9] << 3;
            colors[0] = fclBlendColor(baseColors[0], 0x14002210, alpha);
            colors[1] = fclBlendColor(baseColors[1], 0x14002210, alpha);
            colors[2] = fclBlendColor(baseColors[2], 0x14002210, alpha);
            nmeMakeGradColors(coords[0], coords[1], (s32)(scale * weights[0]),
                              (s32)(scale * weights[1]), (s32)(scale * weights[4]));
            cmpDrawTriG((u32 *)coords[0], (u32 *)coords[1], 0, colors, ot);

            coords[0][0] = (faces[i][2] - offset) << 4;
            coords[0][1] = (faces[i][4] - offset) << 4;
            coords[0][2] = (faces[i][8] - offset) << 4;
            coords[1][0] = faces[i][3] << 3;
            coords[1][1] = faces[i][5] << 3;
            coords[1][2] = faces[i][9] << 3;
            colors[0] = fclBlendColor(baseColors[3], 0x14002210, alpha);
            colors[1] = baseColors[4];
            colors[2] = fclBlendColor(baseColors[5], 0x14002210, alpha);
            nmeMakeGradColors(coords[0], coords[1], (s32)(scale * weights[1]),
                              (s32)(scale * weights[2]), (s32)(scale * weights[4]));
            cmpDrawTriG((u32 *)coords[0], (u32 *)coords[1], 0, colors, ot);

            coords[0][0] = (faces[i][4] - offset) << 4;
            coords[0][1] = (faces[i][6] - offset) << 4;
            coords[0][2] = (faces[i][8] - offset) << 4;
            coords[1][0] = faces[i][5] << 3;
            coords[1][1] = faces[i][7] << 3;
            coords[1][2] = faces[i][9] << 3;
            offset += 10;
            colors[0] = baseColors[6];
            colors[1] = baseColors[7];
            colors[2] = fclBlendColor(baseColors[8], 0x14002210, alpha);
            nmeMakeGradColors(coords[0], coords[1], (s32)(scale * weights[2]),
                              (s32)(scale * weights[3]), (s32)(scale * weights[4]));
            cmpDrawTriG((u32 *)coords[0], (u32 *)coords[1], 0, colors, ot);
        }
    }
}

#else
INCLUDE_ASM(const s32, "code_0022C578", nmeDrawGradQuad);
#endif

const u32 nmeGradBaseColors[10] = {0x10, 0x10, 0x10, 0x10, 0x0, 0x10, 0x0, 0x0, 0x10, 0x0};
const f32 nmeGradWeights[6] = {2.0f, 3.0f, 1.5f, 1.5f, 2.0f, 0.0f};
void nmeDrawBlendQuad(s32 alpha, s32 ot)
{
    u32 coords[2][3] = {{0x2000, 0xC80, 0x2000}, {0xE00, 0xE00, 0x9C0}};
    u32 colors[3];

    memset(colors, 0, sizeof(colors));
    colors[0] = fclBlendColor(0x59, 0x26, 0x100 - alpha);
    cmpDrawTriG(coords[0], coords[1], 0, colors, ot);
}

#ifdef NON_MATCHING
extern void cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
#endif

void nmeDrawBox(s32 color, s32 unused, s32 flag)
{
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, color, flag);
}

/* Native callers pass full argument words. Narrow only at the rendering
 * consumer, retaining this provider's byte and halfword conversions. */
void nmeDrawSizedSprite(s32 id, s32 idx, s32 *texs, fclSpriteTbl_t *tbl, s32 color, s32 size,
                        s32 ot)
{
    s32 colors[4];
    s32 half = size / 2;

    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteSize(-(half * 16), -(half * 8), 0, size + 0x200, size + 0x1C0, colors, (u8)id, (u16)idx,
                      texs, tbl, ot);
}

s32 nmeLerp(s32 amplitude, s32 frame, s32 period)
{
    s32 offset = (frame * 2) % period;
    s32 halfPeriod = period / 2;
    s32 halfAmplitude = amplitude / 2;
    s32 quarter = period / 4;
    s32 square = quarter * quarter;
    s32 descending;
    s32 value;

    if (offset >= halfPeriod) {
        offset = period - offset;
    }
    if (offset >= quarter) {
        offset = halfPeriod - offset;
        descending = 0;
    } else {
        descending = 1;
    }
    value = halfAmplitude * (offset * offset) / square;
    return descending ? amplitude - value : value;
}

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

extern void nmeDrawGradQuad(s32 x, s32 alpha, s32 ot);
extern void nmeDrawQuadPanel(s32 color0, s32 color1, s32 alpha, s32 ot);

void nmeDrawFrame(s32 type, s32 frame)
{
    s32 move = nmeLerp(0x12C, frame % 0x300, 0x300);
    s32 pulse = nmeLerp(0x32, frame % 0x100, 0x100);
    s32 alpha = nmeLerp2(0x100, frame % 0x300, 0x300);

    nmeDrawSizedSprite(0, 0, D_00345D88, fclSpriteTable, 0x80808040, 0, 7);
    nmeDrawBox(0x40, pulse, 7);
    nmeDrawBlendQuad(pulse, 7);
    nmeDrawGradQuad(move, 0x100 - alpha, 7);
    nmeDrawQuadPanel(0x9192FF4C, 0x88D8FC33, pulse, 7);
}

void nmeDrawBlendText(s32 sprite, s32 page, sdfTexHandle_t **textures,
                      fclSpriteTbl_t *table, u32 color, s32 frame, s32 ot)
{
    s32 pulse = nmeLerp(100, frame % 0x100, 0x100);
    s32 phase = frame % 0x300;
    /* This real percent divisor is established before the size phase. */
    s32 percent = 100;
    s32 size = nmeLerp(0x8C, phase, 0x300);
    s32 shade;

    /* Repeat the actual remainder, as nmeDrawFrame does: native retains
     * its second division check even when the remainder itself is shared. */
    nmeLerp2(0x100, frame % 0x300, 0x300);
    nmeDrawSizedSprite(sprite, page, (s32 *)textures, table, 0x80808080, 0, ot);
    nmeDrawSizedSprite(sprite, page, (s32 *)textures, table, 0x80808080, size, ot);
    shade = fclBlendColor(0x35, 0x1C, pulse * 0x100 / percent);
    nmeDrawBox(shade, pulse, ot);
    nmeDrawBlendQuad(pulse, ot);
    nmeDrawQuadPanel(0x756E6866, 0x14140A66, pulse * 2, ot);
}

extern frFontRequest_t *frwGetHelpFRQ(s32 x, s32 y, s32 priority, s8 bank, u16 index, s8 mode);
extern void frSetFontColor(frFontHandle_t *request, u32 color);

#ifdef NON_MATCHING
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page, s32 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 ot);
#endif

void cmpDrawHelp(fclPanelPos_t *pos, u16 index, s8 bank, s8 skipBackground, s32 alpha, u32 ot)
{
    s32 colors[4];
    s32 halfAlpha = (alpha / 2) | 0x80808000;
    u32 fontColor;
    void *font;

    colors[0] = halfAlpha;
    colors[1] = halfAlpha;
    colors[2] = halfAlpha;
    colors[3] = halfAlpha;
    if (skipBackground == 0) {
        fclDrawSpriteRes(0, 0, 0, colors, 7, 0xB, D_00345D88, fclSpriteTable, 0x47);
    }
    fontColor = fclBlendColor(0xA09DC380, 0xA09DC300, alpha);
    font = frwGetHelpFRQ(pos->unk_00, pos->unk_04, pos->unk_08, bank, index, 0);
    frSetFontColor(font, fontColor);
    frDrawFontOt(font, 1, ot);
    frDestroyFontRequestAfter(font);
}

extern const char *cmpMenuTextTable[];

extern s32 cmpStartProcessInfo(s32 text, s32 flag);

void cmp_2304C0(s8 index)
{
    const char **entry = &cmpMenuTextTable[index];

    if (cmpStartProcessInfo(EE_ADDR_S(*entry), 0) == 0) {
        func_0023B500(EE_ADDR_S(*entry));
    }
}

void cmpDisableInfoWindow(void)
{
    cmpSetInfoWindowHidden();
}

extern const char *cmpMenuStrings[] FAKEMATCH_ASM_NAME("cmpMenuTextTable");
extern void cmpSetSkillListRows(s32 rows);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page,
                             s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 ot);
extern u32 frGetStrWidth2(frFontHandle_t *font);
extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);

void campDrawText(s32 id, s32 *colors)
{
    s8 index = id;
    u8 alpha = colors != NULL ? *(u8 *)colors : 0x80;
    /* Compute the actual font color before the row-height callback; this
     * preserves the native live color across that call. */
    u32 fontColor = 0xA09DC300 | alpha;
    frFontHandle_t *font;
    s32 x;

    cmpSetSkillListRows(0x12);
    font = frwGetSmallFontFRQ2(0, 0, 0xFFFFF, fontColor,
                               cmpMenuStrings[index], NULL);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 0x21, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 0x22, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 0x23, D_00345D88, fclSpriteTable, 0x47);
    x = (0xB9 - (s32)frGetStrWidth2(font)) / 2 + 0x10;
    frSetDrawPosition(font, x * 16, 0xC8);
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
    cmpSetSkillListRows(-1);
}

#ifdef NON_MATCHING
extern const volatile f32 D_0054E45C;
extern void cmpDrawPanelEffCell(s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1, s32 color, s32 ot);

void func_002306A0(s32 x, s32 y, s32 z, s32 size, u32 color, s32 edgeColor, s8 direction, s32 ot)
{
    u32 xs[3], ys[3], colors[3];
    s32 half = size / 2;
    s32 height = (s32)((f32)size / D_0054E45C);

    switch (direction) {
    case 0:
        xs[0] = x + half * 16;
        xs[1] = x - half * 16;
        xs[2] = x;
        ys[0] = y;
        ys[1] = y;
        ys[2] = y - height * 8;
        break;
    case 1:
        xs[0] = x - half * 16;
        xs[1] = x + half * 16;
        xs[2] = x;
        ys[0] = y;
        ys[1] = y;
        ys[2] = y + height * 8;
        break;
    case 2:
        xs[0] = x;
        xs[1] = x;
        xs[2] = x - height * 16;
        ys[0] = y - half * 8;
        ys[1] = y + half * 8;
        ys[2] = y;
        break;
    case 3:
        xs[0] = x;
        xs[1] = x;
        xs[2] = x + height * 16;
        ys[0] = y + half * 8;
        ys[1] = y - half * 8;
        ys[2] = y;
        break;
    default:
        /* Retail leaves the coordinates unset; no retail caller takes this path. */
        xs[0] = xs[1] = xs[2] = 0;
        ys[0] = ys[1] = ys[2] = 0;
        break;
    }
    colors[0] = colors[1] = colors[2] = color;
    cmpDrawTriG(xs, ys, z, colors, ot);
    cmpDrawPanelEffCell(xs[0], ys[0], z, xs[1], ys[1], z, edgeColor, ot);
    cmpDrawPanelEffCell(xs[1], ys[1], z, xs[2], ys[2], z, edgeColor, ot);
    cmpDrawPanelEffCell(xs[2], ys[2], z, xs[0], ys[0], z, edgeColor, ot);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", func_002306A0);
#endif

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

void campDrawStockRow(s8 mode)
{
    u8 stockIndex = (u8)cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);

    if (mode == 0) {
        campWork->drawPhase = 1;
        switch (campWork->previousPhase) {
        case 3:
            switch (campWork->page[1].cursor.unk_22 + (u8)campWork->page[1].cursor.unk_24) {
            case 0:
                campDrawText(0xD, 0);
                break;
            case 1:
                campDrawText(0xE, 0);
                break;
            case 2:
                campDrawText(0xF, 0);
                break;
            }
            break;
        case 2:
            campDrawText(5, 0);
            break;
        }
    } else {
        switch (campWork->previousPhase) {
        case 2:
            campSkillTargetIsSelf(DDS3_GBWK->party[stockIndex].unit.skill[
                campWork->itemCursor + campWork->itemScroll], 3);
            campDrawText(6, 0);
            break;
        case 1:
            campSkillTargetIsSelf(datItemTable[campWork->itemId[
                campWork->itemCursor + campWork->itemScroll]].skill, 5);
            campDrawText(6, 0);
            break;
        case 3:
            campWork->drawPhase = 3;
            break;
        }
    }
}

void campDrawSetPhase(void)
{
    campDrawText(1, 0);
    campWork->drawPhase = 2;
}

void campDrawStockCursor(void)
{
    switch (campWork->page[0].cursor.unk_22 + (u8)campWork->page[0].cursor.unk_24) {
    case 1:
        campDrawText(0x10, 0);
        break;
    case 0:
        campDrawText(7, 0);
        break;
    case 2:
        campDrawText(8, 0);
        break;
    case 3:
        campDrawText(9, 0);
        break;
    }
    campWork->drawPhase = 0;
}


extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page,
                             s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 ot);
extern frFontRequest_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                         const char *text, frFontRequest_t *previous);
extern char cmpFmtMaka[];

void campDrawNumber(void)
{
    s32 timer = campWork->RootTimer;
    char text[0x20];
    s32 colors[4];
    void *font;
    s32 fade;
    u32 fontColor;
    s32 phase;
    s32 stepMode;

    if (timer != 0 && timer != 0x200) {
        if (timer < 0x100) {
            fade = timer / 2;
        } else {
            timer %= 0x100;
            fade = (0x100 - timer) / 2;
        }
        fade = SDF_FRAME_INTERPOLATE_DUE(&campWork->RootTimer, fade);
        colors[0] = 0x80808000 | fade;
        colors[1] = 0x80808000 | fade;
        colors[2] = 0x80808000 | fade;
        colors[3] = 0x80808000 | fade;
        fclDrawSpriteRes(0, 0, 0, colors, 0, 0x1A,
                         D_00345D88, fclSpriteTable, 0x47);
        fontColor = 0xA09DC300 | fade;
        sprintf(text, cmpFmtMaka, DDS3_GBWK->maka);
        font = frwGetNumFontFRQ(0x3C0, 0xC90, 0xFFFFF,
                                fontColor, text, NULL);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
    }

    phase = campWork->unk_0001;
    switch (phase) {
    case 4:
    case 6:
    case 7:
        stepMode = 0;
        break;
    default:
        stepMode = 1;
        break;
    }
    if ((s8)campWork->statusPhase != 0) {
        stepMode = 0;
        campWork->RootTimer = 0x200;
    }

    if (stepMode != 0) {
        if (campWork->RootTimer > 0x100) {
            campWork->RootTimer = 0x200 - campWork->RootTimer;
        }
        if (campWork->RootTimer < 0x100) {
            campWork->RootTimer += 0x2A * SDF_FRAME_DUE();
        }
        if (campWork->RootTimer > 0x100) {
            campWork->RootTimer = 0x100;
        }
    } else {
        if (campWork->RootTimer < 0x100) {
            campWork->RootTimer = 0x200 - campWork->RootTimer;
        }
        if ((u32)(campWork->RootTimer - 0x100) < 0x100) {
            campWork->RootTimer += 0x2A * SDF_FRAME_DUE();
        }
        if (campWork->RootTimer > 0x200) {
            campWork->RootTimer = 0x200;
        }
    }
}

extern void fclDrawSpriteFull(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode, s32 *color, s32 id,
                              u16 spriteId, s32 *texList, fclSpriteTbl_t *table, s32 OtNo);

void campDrawFade(void)
{
    s32 colors[4];
    s32 color = 0x80808000;
    s32 alpha = 0x100 - campWork->fadeStep;
    s32 step;

    if (alpha != 0) {
        color = alpha / 8 | color;
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        fclDrawSpriteFull(0, 0, 0xFFFFF, 0x200, 0x1C0, 2, colors, 0, 0, D_00345D88,
                          fclSpriteTable, 0x47);
    }
    step = campWork->fadeStep + 0x20;
    campWork->fadeStep = step;
    if (step >= 0x101) {
        campWork->fadeStep = 0x100;
    }
}







/* The first sprite ID uses a localized live-value constraint; see the
 * matching note for the paired ordinary and nonvolatile controls. */
struct campCrossPanel_t;
extern void campRunPanelCallback(struct campCrossPanel_t *panel);
extern void campDrawSkillPanel(void);
extern void campDrawInfoPanel(void);
extern void campDrawSeq3(s8 phase);
extern void campDrawStockRow(s8 mode);
extern void campDrawNumber(void);
extern void fclDrawPanelSprite(s32 id, s32 ot);
extern void cmpFindPanelEffSlot(s32 ot);

s32 camp_draw_Process(void)
{
    if (campWork->unk_0001 != 7) {
        nmeDrawFrame(0x10, (s32)SDF_FRAME_STEP(campWork->effectStep));
    }
    if ((campWork->unk_0000 & 0x20) != 0) {
        cmpUpdateTerminate((cmpInfoWindow_t *)campWork);
        return -1;
    }
    campRunPanelCallback((struct campCrossPanel_t *)campWork->crossPanel);
    switch (campWork->unk_0001) {
    case 0: campDrawSkillPanel(); break;
    case 1: campDrawInfoPanel(); break;
    case 3: campDrawSeq3(0); break;
    case 5: campDrawText(2, 0); break;
    case 4: campDrawText(0xA, 0); break;
    case 8: campDrawStockRow(0); break;
    case 9: campDrawStockRow(1); break;
    case 11: campDrawStockCursor(); break;
    case 10: campDrawSetPhase(); break;
    case 14:
        if (campWork->stockMessagePhase == 0) {
            campDrawSeq3(2);
        } else {
            campDrawSeq3(3);
        }
        break;
    case 16: campDrawStockCursor(); break;
    }
    campDrawNumber();
    {
        s32 spriteId = 0x33001;
        /* Keep this actual draw argument complete before the call, leaving
         * the OT argument in its native delay slot. This empty constraint
         * emits no instructions and is not a memory barrier. */
        FAKEMATCH_ASM_LIVE_VOLATILE(spriteId);
        fclDrawPanelSprite(spriteId, 0x48);
    }
    cmpFindPanelEffSlot(0x48);
    fclDrawPanelSprite(0x5101B, 0x48);
    campDrawFade();
    return 0;
}


extern u32 dds3DebugSign;

extern void func_00242A70(s32 key);

#ifdef NON_MATCHING
extern s32 campStatusIsActive(void);
#endif

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

extern void *cmpStartProcessDH(cmpSeqInfo_t *parent);

void cmpUpdateRoot(void)
{
    u16 button = cmpGetPressedButton(0x3B);
    switch (button) {
    case 1:
        campWork->unk_0001 = ((u8 *)campWork)[0x3E] + ((u8 *)campWork)[0x40] + 1;
        campWork->unk_0004 = 1;
        switch (campWork->unk_0001) {
        case 5:
            campWork->drawPhase = 1;
            campWork->stock[0].count = 0xCF;
            break;
        case 4:
            cmpInitSelectCursor((s8)DDS3_GBWK->heartscnt);
            campWork->dhProcess = cmpStartProcessDH(NULL);
            /* fall through */
        default:
            campWork->stock[0].count = 0xF;
            break;
        }
        cmpBuildStockNameList(&campWork->stock[0]);
        cmpReCalcStockCursorPos(2);
        break;
    case 2:
    case 8:
        button = 0;
        campWork->unk_0000 |= 0x10;
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)campWork + 0x1C), -1);
        break;
    case 0x20:
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)campWork + 0x1C), 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)campWork + 0x1C), 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)((u8 *)campWork + 0x1C), button, 0x30));
}


void cmpUpdateItem(void)
{
    u16 button = cmpGetPressedButton(0x33);

    switch (button) {
    case 1:
        campWork->unk_0001 = button;
        campWork->unk_0004 = button;
        break;
    case 2:
        campWork->unk_0001 = 0;
        campWork->unk_0004 = button;
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveCursor(&campWork->page[0].cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&campWork->page[0].cursor, 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&campWork->page[0].cursor, 0);
    }
    cmpPlayCursorSE(
        (u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&campWork->page[0], button, 0x30));
}


extern void cmpSetCrossGeneralVal(u8 *work, s8 value);

void cmpUpdateParty(void)
{
    u16 button = cmpGetPressedButton(0x33);

    switch (button) {
    case 1:
        cmpInitStockCursorPos();
        if (cmpSetPartyDSL() != 0) {
            cmpSetCrossGeneralVal((u8 *)campWork + 0x8448, 1);
            campWork->unk_0001 = 8;
        } else {
            button = 0x8000;
        }
        break;
    case 2:
        campWork->unk_0001 = 0;
        campWork->unk_0004 = button;
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveCursor(&campWork->page[1].cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&campWork->page[1].cursor, 1);
        break;
    }

    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&campWork->page[1].cursor, 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&campWork->page[1], button, 0x30));
}

#ifdef NON_MATCHING
void campUpdateStockSelect(s8 mode)
{
    u16 button = cmpGetPressedButton(0x33);
    s32 invalid = 0;
    cmpCursorInfo_t *cursor;

    if (fclChkMessage() != 0) {
        return;
    }
    switch (button) {
    case 1: {
        s8 index = cmpGetStockInfoIndex(campWork->stock, mode, 0);

        if (campWork->previousPhase == 3) {
            s32 selection = campWork->page[1].cursor.unk_22 +
                            (u8)campWork->page[1].cursor.unk_24;
            if (selection == 0) {
                datUnitWork_t *unit = &DDS3_GBWK->unitwork[index];
                u32 flags = unit->flag;

                invalid = (flags & 2) != 0;
                if ((unit->badstatus & 0x800) != 0 || (flags & 0x10) != 0) {
                    invalid = 1;
                }
                if (camp_H_cmpLocalStock_t__ctor() == 3) {
                    invalid = 1;
                }
                if (invalid == 0) {
                    button = 0;
                    p2sdPlaySE(0x11, 0x7F, 0x3F);
                }
            } else if (selection == 1) {
                button = 0;
                p2sdPlaySE(0x12, 0x7F, 0x3F);
            }
        }
        if (campWork->previousPhase == 2) {
            datUnitWork_t *unit = &DDS3_GBWK->unitwork[
                cmpGetStockInfoIndex(campWork->stock, 0, 0)];

            if ((unit->badstatus & 0x20) != 0 || unit->skillcnt == 0) {
                invalid = 1;
            }
        }
        if (campWork->previousPhase == 1 ||
            (campWork->previousPhase == 2 && mode != 0)) {
            button &= 0xFFFE;
        }
        if (invalid != 0) {
            button = 0x8000;
        } else {
            campWork->unk_0001 = campWork->previousPhase;
            campWork->unk_0004 = 1;
        }
        break;
    }
    case 2:
        campWork->unk_0001 = campWork->previousPhase;
        campWork->unk_0004 = 2;
        cmpResetCursorInfo((cmpCursorInfo_t *)&campWork->unk_6230[mode * 0x103C]);
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveStockCursor(mode, -1, (u8 *)campWork->stock);
        break;
    case 0x20:
        cmpMoveStockCursor(mode, 1, (u8 *)campWork->stock);
        break;
    }
    cursor = (cmpCursorInfo_t *)&campWork->unk_6230[mode * 0x103C];
    cmpMoveCursor2(cursor, 0);
    cursor = (cmpCursorInfo_t *)&campWork->unk_6230[mode * 0x103C];
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)cursor, button, 0x30));
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campUpdateStockSelect);
#endif

void campUpdateStockCursor(void)
{
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    u16 item = campWork->itemId[campWork->itemCursor + campWork->itemScroll];
    s32 category;
    s32 currentCategory;

    if (fclChkMessage() != 0) {
        return;
    }

    switch (button) {
    case 1:
        if (campWork->unk_8302 != 0) {
            category = campWork->page[0].cursor.unk_22;
            category += (u8)campWork->page[0].cursor.unk_24;
            switch (category) {
            case 0:
                if (cmpItemIsUsable(item) != 0) {
                    button = 0x8000;
                }
                break;
            case 1:
                if (cmpItemHasFlag(item, 1) == 0) {
                    button = 0x8000;
                }
                break;
            }
            if (button != 0x8000) {
                campWork->unk_0001 = 1;
                campWork->unk_0004 = 1;
            }
        }
        currentCategory = campWork->page[0].cursor.unk_22 +
                          (u8)campWork->page[0].cursor.unk_24;
        if (currentCategory < 4) {
            if (currentCategory >= 2) {
                button = 0;
            }
        }
        break;
    case 2:
        campWork->unk_0001 = 1;
        campWork->unk_0004 = button;
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, -1);
        break;
    case 0x20:
        cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, 1);
        break;
    }

    if (campWork->unk_410C[0x20] == 0 && button != 2) {
        button = 0;
    }
    cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, 0);
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)campWork->unk_410C, button, 0x30));
}

extern s32 cmpChkDoNotUseSkill(u16 skill);

/* Exact-type view of the existing provider, deliberately hiding its earlier
 * definition. In paired complete-owner builds, the direct call moves the
 * common cursor-work load into the button-one branch delay slot; this view
 * preserves retail's no-op there and load in the default exit's delay slot.
 * The provider and ABI are unchanged. This is a target-specific visibility
 * aid, not evidence of original source spelling. */
extern s32 cmpChkDoNotUseSkillView(u16 skill) FAKEMATCH_ASM_NAME("cmpChkDoNotUseSkill");
void cmpUpdateSkillSelect(void)
{
    u16 button = cmpGetPressedButton(0x33);
    u8 index = cmpGetStockInfoIndex(campWork->stock, 0, 0);
    u16 skill = DDS3_GBWK->party[index].unit.skill[campWork->itemCursor + campWork->itemScroll];

    if (fclChkMessage() != 0) {
        return;
    }
    switch (button) {
    case 1: {
        s32 blocked = cmpChkDoNotUseSkillView(skill);

        if (blocked != 0) {
            button = 0x8000;
            break;
        }
        campWork->unk_0001 = 2;
        campWork->unk_0004 = button;
        break;
    }
    case 2:
        campWork->unk_0001 = 2;
        campWork->unk_0004 = 2;
        cmpDisableInfoWindow();
        break;
    case 0x10:
        cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, -1);
        break;
    case 0x20:
        cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, 1);
        break;
    }
    cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_410C, 0);
    button = (u16)cmpDisablePadBitStop((cmpPadStopWork_t *)campWork->unk_410C,
                                       button, 0x30);
    cmpPlayCursorSE(button);
}

extern u8 cmpClearStatusFlags(void);
extern void *cmpStartProcessStatus(s8 arg, cmpSeqInfo_t *parent);
extern void cmpSetStatusWindowHidden(void);
extern void cmpMoveStockCursor(s8 index, s8 direction, u8 *stock);

void cmpUpdateProcessStatus(void)
{
    u16 button = cmpGetPressedButton(0x33);
    u8 index = cmpGetStockInfoIndex(&campWork->stock[0], 0, 0);

    if ((s8)campWork->statusPhase != 0 && (s8)campWork->statusPhase < 3) {
        campWork->statusPhase += SDF_FRAME_DUE();
    }
    switch (button) {
    case 1:
        if (cmpClearStatusFlags() != 0) {
            cmpStartProcessStatus((s8)index, NULL);
            campWork->unk_0004 = button;
            campWork->statusPhase = button;
        } else if ((s8)campWork->statusPhase != 0) {
            button = 0;
        }
        break;
    case 2:
        if ((s8)campWork->statusPhase == 3) {
            if (cmpLoadStatusModel() == 0) {
                campWork->drawPhase = 1;
                cmpSetStatusWindowHidden();
                campWork->unk_0004 = button;
                campWork->statusPhase = 0;
            } else {
                button = 0x8000;
            }
        } else if ((s8)campWork->statusPhase == 0) {
            cmpDisableInfoWindow();
            campWork->unk_0001 = 0;
            campWork->unk_0004 = button;
            campWork->statusPhase = 0;
        }
        break;
    case 0x10:
        if ((s8)campWork->statusPhase == 0) {
            cmpMoveStockCursor(0, -1, (u8 *)&campWork->stock[0]);
        }
        break;
    case 0x20:
        if ((s8)campWork->statusPhase == 0) {
            cmpMoveStockCursor(0, 1, (u8 *)&campWork->stock[0]);
        }
        break;
    }
    cmpMoveCursor2((cmpCursorInfo_t *)campWork->unk_6230, 0);
    cmpPlayCursorSE((u16)cmpDisablePadBitStop((cmpPadStopWork_t *)campWork->unk_6230, button, 0x30));
}

void cmpRemoveStockDevil(void)
{
    cmpRemoveStockDevilEntry();
    campWork->stock[0].count = 0x8B;
    cmpBuildStockNameList(&campWork->stock[0]);
    cmpInitDrawList();
    cmpReCalcStockCursorPos(2);
}

extern s32 cmpSetStockRemoveEff(u8 *work);
extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern s8 fclEndMessage(void);

void campWaitSelMessage(void)
{
    if (campWork->stockMessagePhase > 0) {
        if (campWork->stockMessagePhase == 1 &&
            cmpSetStockRemoveEff((u8 *)campWork->stock) == 0) {
            cmpRemoveStockDevil();
            campWork->stockMessagePhase = 3;
        }
        if (fclChkMessage() == 0 && campWork->stockMessagePhase != 1) {
            if (cmpStartStockMsg(1) == 0) {
                cmpSetCrossGeneralVal(campWork->crossPanel, 0);
                campWork->unk_0001 = 3;
                campWork->unk_0004 = 1;
            } else {
                campWork->unk_0001 = 8;
                campWork->unk_0004 = -1;
                campWork->previousPhase = 3;
            }
        }
    } else if (fclChkSelMessage() == 0) {
        switch (fclGetSelMessagePos()) {
        case 0:
            campWork->stockMessagePhase = 1;
            p2sdPlaySE(0x13, 0x7F, 0x3F);
            /* fall through */
        case 1:
            if (campWork->stockMessagePhase == 0) {
                fclEndMessage();
                campWork->stockMessagePhase = 2;
            } else {
                /* Keep this initialized publication value in native $2.
                 * The ordinary and generic-constraint forms reuse a saved
                 * constant, changing dispatch and the shared exit. */
                FAKEMATCH_ASM_REG(s32, confirm, "$2") = 1;
                FAKEMATCH_ASM_LIVE(confirm);
                campWork->unk_0004 = confirm;
            }
            break;
        case 2:
        default:
            goto done;
        }
    }
done:
    return;
}

#ifdef NON_MATCHING
extern s32 campInfoWorkExists(void);
#endif

void campCloseInfoWindow(void)
{
    if (campInfoWorkExists() == 0) {
        campWork->unk_0001 = 0;
        campWork->unk_0004 = 2;
        cmpDisableInfoWindow();
    }
}

extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern s8 fclEndMessage(void);

void cmpUpdateDestroyItemConfirm(void)
{
    s32 closed = 0;
    s32 confirm;

    if (fclChkSelMessage() != 0) {
        goto done;
    }
    confirm = 1;
    switch (fclGetSelMessagePos()) {
    case 0:
        cmpDestroyItem(1);
        cmpCreateLocalItem(0);
        cmpChangeCursorListNums(campWork->unk_8302, campWork->unk_410C);
        if (campWork->unk_8302 == 0) {
            campWork->unk_0001 = 0;
            campWork->unk_0004 = confirm;
            closed = 1;
        }
        break;
    case 1:
        break;
    case 2:
    default:
        goto done;
    }
    fclEndMessage();
    if (!closed) {
        campWork->unk_0001 = 11;
        campWork->previousPhase = 1;
    }
    cmpSetCrossGeneralVal(campWork->crossPanel, 0);
done:
    return;
}

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

#ifdef NON_MATCHING
extern s32 fmProcessChk(void);
extern u32 slGetExitMode(void);
#endif

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
    if (work->effectStep >= SDF_FRAMES(0x900)) {
        work->effectStep = 0;
    }
}

s32 func_00232198(void)
{
    s32 i;

    if (!SDF_FRAME_DUE()) return 0;
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

extern void fclUpdateMessage(void);
extern s32 cmpUpdateSeqInfo(cmbGbWork_t *work, s8 value);

#ifdef NON_MATCHING
extern void campUpdateStockSelect(s8 mode);
#endif

s32 camp_update_Process(void)
{
    s8 state = campWork->unk_0001;

    if ((campWork->unk_0000 & 0x40) != 0) {
        return -1;
    }
    switch (state) {
    case 0:
        if (campDebugCheckPad(0) == 0) {
            cmpUpdateRoot();
        }
        break;
    case 1:
        cmpUpdateItem();
        break;
    case 2:
        break;
    case 3:
        cmpUpdateParty();
        break;
    case 5:
        if (campDebugCheckPad(1) == 0) {
            cmpUpdateProcessStatus();
        }
        break;
    case 4:
        campCloseInfoWindow();
        break;
    case 6:
        campShowStock();
        break;
    case 7:
        cmpUpdateLoad();
        break;
    case 8:
        campUpdateStockSelect(0);
        break;
    case 9:
        campUpdateStockSelect(1);
        break;
    case 11:
        campUpdateStockCursor();
        break;
    case 10:
        cmpUpdateSkillSelect();
        break;
    case 12:
    case 13:
        break;
    case 14:
        campWaitSelMessage();
        break;
    case 15:
        break;
    case 16:
        cmpUpdateDestroyItemConfirm();
        break;
    default:
        break;
    }
    fclUpdateMessage();
    campUpdateEffects();
    cmpUpdateSeqInfo((cmbGbWork_t *)campWork, state);
    return 0;
}


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
extern s32 camp_update_stock_Process(dds3ProcessID_t *id);

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

/* Normalize the returned flag byte explicitly in unsigned arithmetic.
 * The split conversion retains the native zero-extension around the store. */
u8 cmpClearStatusFlags(void)
{
    campStatusPanel_t *panel = campStatusPanel;
    if (panel != NULL) {
        u32 value = panel->flags & 2;
        u32 high = value << 24;
        panel->flags = value;
        return high >> 24;
    }
    return 0;
}

extern cmpInfoWindow_t *campCalcStockWindow;

s32 camp_calc_stock_Process(s32 id)
{
    cmpInfoWindow_t *window = dds3GetProcessData(id);

    campCalcStockWindow = window;
    if ((window->flags & 0x10) != 0) {
        cmpUpdateTerminate(window);
        return -1;
    }
    return 0;
}

#ifdef NON_MATCHING
extern void campDrawStockSlots(s8 *indices, s8 active, s8 *flags, cmpLocalStock_t *stock);
#else
extern void campDrawStockSlots(s8 *indices, s32 active, s8 *flags, cmpLocalStock_t *stock);
#endif

void campDrawStockPanel(cmpLocalStock_t *stock, s32 mode)
{
    s8 indices[3];
    s8 flags[3];
    s8 state = *((s8 *)stock + 0x2124);

    switch (state - 1) {
    case 0:
        indices[0] = cmpGetStockInfoIndex(stock, 0, 1);
        indices[1] = cmpGetStockInfoIndex(stock, 0, 1);
        indices[2] = -1;
        flags[0] = 0;
        flags[1] = 0;
        flags[2] = 0;
        break;
    case 1:
        indices[0] = -1;
        indices[1] = cmpGetStockInfoIndex(stock, 0, 1);
        indices[2] = -1;
        flags[0] = 0;
        flags[1] = 0;
        flags[2] = 0;
        break;
    case 2:
        indices[0] = cmpGetStockInfoIndex(stock, 1, 1);
        indices[1] = cmpGetStockInfoIndex(stock, 1, 1);
        indices[2] = cmpGetStockInfoIndex(stock, 0, 1);
        flags[0] = 1;
        flags[1] = 1;
        flags[2] = 0;
        break;
    case 4:
        indices[0] = cmpGetStockInfoIndex(stock, 1, 1);
        indices[1] = cmpGetStockInfoIndex(stock, 1, 1);
        indices[2] = -1;
        flags[0] = 1;
        flags[1] = 1;
        flags[2] = 0;
        break;
    case 3:
    default:
        indices[0] = -1;
        indices[1] = -1;
        indices[2] = -1;
        flags[0] = 0;
        flags[1] = 0;
        flags[2] = 0;
        break;
    }
    campDrawStockSlots(indices, cmpStockEffIsActive((u8 *)stock), flags, stock);
}

extern cmpInfoWindow_t *campDrawStockWindow;

s32 camp_draw_stock_Process(s32 id)
{
    cmpInfoWindow_t *window = dds3GetProcessData(id);

    campDrawStockWindow = window;
    if ((window->flags & 0x20) != 0) {
        cmpUpdateTerminate(window);
        return -1;
    }
    if ((window->flags & 2) != 0) {
        campDrawStockPanel(EE_ADDR_AS(cmpLocalStock_t *, window->unk_1C), 0);
    }
    return 0;
}

extern cmpStockSeqInfo_t *cmpStockUpdateState;
extern void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor);

s32 camp_update_stock_Process(dds3ProcessID_t *id)
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

void cmpSetDHEntry(s8 id) {
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

    for (i = 0; i < ((u8 *)DDS3_GBWK)[0x1D7B]; i++) {
        if (DDS3_GBWK->unk_01CFB[i] == id) {
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
    return cmpHeartsIconTable[index].id;
}

s8 cmpGetHeartsIconType(s8 index)
{
    return cmpHeartsIconTable[index].type;
}

s32 cmpGetHeartsIconColor(s8 index)
{
    return cmpHeartsIconTable[index].color;
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

extern sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *aTex);

void campDhCheckTextures(void)
{
    s32 ofs;
    s32 i;

    for (i = 0, ofs = 0x1160; i < 3; i++, ofs += 4) {
        /* Keep the freshly observed owner in its native temporary home. It
         * dies at the row calculation, before any callback can clobber v0.
         * This is a target compiler constraint, not original-source evidence. */
        FAKEMATCH_ASM_REG(campInfoWork_t *, work, "v0") = campInfoWork;
        u8 *row;
        u8 *textureRow;
        TMX_HEADER_t *texture;

        row = (u8 *)work + ofs;
        textureRow = row;

        /* The two real reads use separate row-address copies in native code.
         * This input-only operand retains the texture-load cursor; it is not
         * a memory barrier and adds no instruction or dummy consumer. */
        FAKEMATCH_ASM_READ(textureRow);
        texture = EE_ADDR_AS(TMX_HEADER_t *, *(s32 *)(textureRow + 0xC));
        if (texture != NULL && *(s32 *)row == 0) {
            sdfTexHandle_t *handle = sdfTexSet(texture);

            *(s32 *)((u8 *)campInfoWork + ofs) = EE_ADDR(handle);
            printf("Set hearts tex. : %d\n", i);
        }
    }
}


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

extern const char campDhFreeTexMsg[];

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
    printf(campDhFreeTexMsg);
}

extern void campDhFreeTexture(campInfoWork_t *work);

extern void sdfMemFreePtrDirect(void *aPtr);

extern const char campDhFreeDataMsg[];

void campDhFreeResources(campInfoWork_t *work)
{
    s32 *tex;
    s32 i;

    campDhFreeTexture(work);
    tex = &work->unk_116C;
    for (i = 0; i < 3; i++)
    {
        sdfMemFreePtrDirect(EE_ADDR_AS(void *, tex[i]));
        tex[i] = 0;
    }
    printf(campDhFreeDataMsg);
}

s32 func_00232C08(void)
{
    return campInfoWork->unk_01 == 0;
}

extern char *pstr[];

extern char *pstr[];

s32 cmpSetHeartsSequenceRemove(void)
{
    if (DDS3_GBWK->equipHearts == 0)
    {
        return 0;
    }
    if ((DDS3_GBWK->party[0].unit.badstatus & 0x8000) == 0)
    {
        campInfoWork->unk_18 = 0;
        fclSetMessageVar_20743088(0xC, EE_ADDR_S(pstr[DDS3_GBWK->equipHearts]));
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

extern u32 dds3GetRandA(void *state);
extern void rstCreateBeforeSkillList(s8 heartsId, datUnitWork_t *stock, s8 levelOffset,
                                     rstSkillInfo_t *info);

void cmpInitHeartsIcon(campInfoWork_t *work)
{
    s32 i;
    datUnitWork_t *unit = &DDS3_GBWK->unitwork[0];

    for (i = 0; i < 25; i++) {
        s8 id = cmpGetHeartsIconId((s8)i);
        s8 type;
        s32 color;
        s8 moveType;

        if (cmpHeartsIconIsVisible(id) == 0) {
            type = -1;
            color = 0;
            moveType = 0;
        } else {
            rstSkillInfo_t skills;
            s8 hasSkills;
            u8 skillCount;

            type = cmpGetHeartsIconType((s8)i);
            color = cmpGetHeartsIconColor((s8)i);
            rstCreateBeforeSkillList(id, unit, 0, &skills);
            hasSkills = skills.SkillCnt;
            skillCount = (u8)skills.SkillCnt;
            moveType = 0;
            if (hasSkills != 0) {
                u16 level = unit->level;
                u8 targetLevel = skills.TargetLevel[0];

                moveType = 2;
                if (level + 1 < targetLevel) {
                    moveType = 0;
                    if (level + 4 >= targetLevel) {
                        moveType = 1;
                    }
                }
                if ((s8)skillCount == 1 && id == 1 && evtBitCheck(0x8C1) == 0) {
                    moveType = 0;
                }
            }
        }
        work->heartsIcons[i].IconType = type;
        work->heartsIcons[i].Stat = 0;
        work->heartsIcons[i].IconColor = color;
        work->heartsIcons[i].MoveType = moveType;
        work->heartsIcons[i].Axis = 0;
        work->heartsIcons[i].AxisChg = -1;
        work->heartsIcons[i].Wait1st = SDF_FRAMES(dds3GetRandA(NULL) & 0x1F);
        {
            u32 wait = dds3GetRandA(NULL) & 0x1F;

            work->heartsIcons[i].Angle = 0.0f;
            work->heartsIcons[i].Wait = SDF_FRAMES(wait);
        }
        work->heartsIcons[i].DefCnt = 0;
        work->heartsIcons[i].DefAng = 0.0f;
        work->heartsIcons[i].Cnt = 0;
        work->heartsIcons[i].Timer = 0;
        work->heartsIcons[i].DistAng = 0.0f;
        work->heartsIcons[i].TimerSpd = 6;
    }
}

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

extern void cmpInitSeqInfo(cmpSeqInfo_t *work, cmpSeqInfo_t *parent);
extern void cmpInitHeartsIcon(campInfoWork_t *work);
extern s8 fclGetCurrentSelMessagePos(void);

campInfoWork_t *cmpinitDH(cmpSeqInfo_t *parent)
{
    campInfoWork_t *work;
    if (campInfoWork == NULL) {
        void *allocation = sdfMemAllocDirect(0x1F8C);
        work = sdfMemLock(allocation);
        memset(work, 0, 0x1F8C);
        work->allocation = allocation;
        work->fadeAlpha = 0x100;
        cmpLoadDHTextures((cmpDhWork_t *)work);
    } else {
        work = campInfoWork;
    }
    work->reserved001C = 0;
    work->entryStep = 0;
    work->targetPhase = 0;
    cmpInitSeqInfo((cmpSeqInfo_t *)work, parent);
    work->unk_00 &= ~1;
    cmpInitHeartsIconTable(work);
    cmpInitHeartsIcon(work);
    cmpInitDHWindow((u8 *)work);
    cmpSetDHEntry(*(s8 *)((u8 *)work + work->unk_31 * 0x34 + 0x1280));
    work->unk_30 = fclGetCurrentSelMessagePos();
    fclSetSelMessagePos(0);
    work->unk_04 = 0;
    return work;
}

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

extern campInfoWork_t *cmpinitDH(cmpSeqInfo_t *parent);

extern s32 camp_calc_dh_Process(dds3ProcessID_t *id);

extern s32 camp_draw_dh_Process();

extern s32 camp_update_dh_Process();

const char campDhFreeTexMsg[24] = "Free hearts tex.\n";
const char campDhFreeDataMsg[24] = "Free hearts tex data.\n";
void *cmpStartProcessDH(cmpSeqInfo_t *parent)
{
    void *proc;

    campInfoWork = cmpinitDH(parent);
    cmpSetStockVisible(0);
    proc = dds3StartProcess("camp_calc_dh", 0x3F0, 1, 1, camp_calc_dh_Process, NULL, campInfoWork);
    dds3StartProcess("camp_draw_dh", 0x2B05, 1, 1, camp_draw_dh_Process, NULL, campInfoWork);
    dds3StartProcess("camp_update_dh", 0x5209, 1, 1, camp_update_dh_Process, camp_update_dh_Shutdown,
                     campInfoWork);
    return proc;
}

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

extern s32 cmpAddQuePanelEff(s32 x, s32 y, s32 z, u16 width, u16 height, u8 step,
                             u8 delay, s32 color0, s32 color1, u8 kind);
extern void rstCreateBeforeSkillList(s8 heartsId, datUnitWork_t *stock, s8 levelOffset,
                                     rstSkillInfo_t *info);

void cmpSetStatusStartEff(void)
{
    rstSkillInfo_t skills;
    f32 alpha;
    s32 i = 0;
    s32 color1 = 0x4686E100;

    cmpAddQuePanelEff(-0xFA0, 0x8A8, 0xFFFFFF, 0x1EA, 0x19, 0x20, 0,
                      0x4686E180, color1, 6);
    rstCreateBeforeSkillList((s8)DDS3_GBWK->heartsequip, &DDS3_GBWK->unitwork[0], 0, &skills);
    if (skills.SkillCnt > 0) {
        alpha = 48.0f;
        for (; i < skills.SkillCnt; i++) {
            cmpAddQuePanelEff((i / 4) * 0x740 + 0x1010, (i % 4) * 0xB0 + 0x9E0,
                              0xFFFFFF, 0x72, 0x13, (u8)(u32)alpha, 5,
                              0x4686E180, color1, 6);
            /* The same alpha feeds the next iteration. This empty target
             * constraint at its carried lifetime keeps the native conversion
             * and schedule; at loop entry it delays the FPU comparison.
             * It is a compiler reconstruction aid, not original spelling. */
            FAKEMATCH_ASM_LIVE_F32(alpha);
        }
    }
}

extern campInfoWork_t *volatile campDhCalcStateLive FAKEMATCH_ASM_NAME("campDhCalcState");
extern campWork_t *volatile campWorkLive FAKEMATCH_ASM_NAME("campWork");
extern void cmpSetStatusStartEff(void);
extern void cmpModelInit(void);

#ifdef NON_MATCHING
s32 camp_calc_dh_Process(dds3ProcessID_t *id)
{
    campInfoWork_t *work = dds3GetProcessData(id);
    u8 flags = ((volatile campInfoWork_t *)work)->unk_00;

    campDhCalcStateLive = work;
    if (flags & 2) {
        s8 state = work->unk_01;

        if (state == 1) {
            if (work->unk_04 == state) {
                campDhFreeTexture(work);
                while (sdfDelayedResrcCheck() != 0) {
                }
                cmpSetStatusStartEff();
                cmpStatSetTimer(0, 0);
                cmpModelInit();
                cmpModelSetup(0, 0);
            }
        } else if (state == 0) {
            s32 ready = campDhCheckReady();
            campWork_t *camp = campWorkLive;

            if (ready != 0) {
                cmpSetCrossGeneralVal(camp->crossPanel, -1);
            } else {
                campDhCheckTextures();
                camp = campWorkLive;
                cmpSetCrossGeneralVal(camp->crossPanel, 0);
            }
        }
    }
    work = campDhCalcStateLive;
    work->unk_04 = 0;
    work = campDhCalcStateLive;
    if (work->unk_00 & 0x10) {
        cmpUpdateTerminate((cmpInfoWindow_t *)work);
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", camp_calc_dh_Process);
#endif

#ifdef NON_MATCHING
extern void cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *f, s32 g);
#endif

void campDhDrawBox(s32 alpha)
{
    s32 corner[4];
    s32 scale = 0x66;
    s32 color = ((alpha * scale / 0x100) & 0xFF) | 0x11D2500;
    s32 dim;

    corner[0] = color;
    dim = corner[0] & 0xFFFFFF00;
    corner[1] = color;
    corner[2] = color;
    corner[3] = color;
    cmpDrawBoxGDefault(0, 0, 0, 0x200, 0x82, corner, 0x47);
    corner[1] = color;
    corner[2] = dim;
    corner[3] = dim;
    corner[0] = color;
    cmpDrawBoxGDefault(0, 0x410, 0, 0x200, 0x8C, corner, 0x47);
}

extern void cmpDrawBoxGDefault(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 ot);

#ifdef NON_MATCHING
void campDhDrawBoxSprite(s32 timer)
{
    s32 phase = (s32)((u32)timer * 6) % 0x200;
    s32 rising = 1;
    s32 offset;
    s32 colors[4];

    if (phase >= 0x100) {
        phase = 0x200 - phase;
    }
    if (phase >= 0x80) {
        phase = 0x100 - phase;
        rising = 0;
    }
    offset = phase * phase * 0x50 / 0x4000;
    if (rising) {
        offset = 0xA0 - offset;
    }
    colors[0] = colors[1] = 0x006E9813;
    colors[2] = colors[3] = 0x006E9800;
    fclDrawPanelSprite2(0x48, 0x47);
    cmpDrawBoxGDefault(0, -offset * 8, 0, 0x200, offset + 0xE6, colors, 0x47);
    colors[0] = colors[1] = 0x00121800;
    colors[2] = colors[3] = 0x00121880;
    fclDrawPanelSprite2(0x44, 0x47);
    cmpDrawBoxGDefault(0, 0x3B0, 0, 0x200, offset + 0x14A, colors, 0x47);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDhDrawBoxSprite);
#endif

void campDhDrawGaugeBars(s32 unusedAlpha)
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

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x1B];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 entryStep;
    /* 0x24 */ s32 targetPhase;
    /* 0x28 */ u8 reserved28[4];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 iconIndex;
    /* 0x32 */ u8 reserved32[0x1146];
    /* 0x1178 */ s32 cursorAlpha[64];
} campDhWork_t;

extern campDhWork_t *campDhDrawWork;

void fclGetArrowCursorTarget(s32 x, s32 y, s32 z, u32 color, s32 extra)
{
    s32 alpha = campDhDrawWork->targetPhase;
    s32 phase;
    s32 colors[4];
    fclSpriteTbl_t *table;
    s32 *texList;

    if (alpha < 512) {
        if (alpha >= 257) {
            alpha = 512 - alpha;
        }
        phase = 0;
    } else {
        alpha -= 512;
        if (alpha >= 257) {
            alpha = 512 - alpha;
        }
        phase = 1;
    }

    if (phase == 0) {
        colors[0] = fclBlendColor(0x80808080, 0x80808040, 256 - alpha);
        texList = (s32 *)((u8 *)campDhDrawWork + 0x1160);
        table = fclSpriteTable;
        colors[1] = colors[0];
        colors[2] = colors[0];
        colors[3] = colors[0];
        fclDrawSpriteRes(x, y, 0, colors, 8, 0x2A,
                         texList, table, 0x47);
        colors[0] = fclBlendColor(0x80808080, 0x80808026, alpha);
        texList = (s32 *)((u8 *)campDhDrawWork + 0x1160);
        colors[1] = colors[0];
        colors[2] = colors[0];
        colors[3] = colors[0];
        fclDrawSpriteRes(x, y, 0, colors, 0xB, 0xD,
                         texList, table, 0x47);
    } else {
        colors[0] = fclBlendColor(0xA0A0A080, 0x80808080, alpha);
        texList = (s32 *)((u8 *)campDhDrawWork + 0x1160);
        table = fclSpriteTable;
        colors[1] = colors[0];
        colors[2] = colors[0];
        colors[3] = colors[0];
        fclDrawSpriteRes(x, y, 0, colors, 8, 0x2A,
                         texList, table, 0x47);
    }
}

extern s32 datDevilIsRegistered(u8 devilId);


void fclDrawArrowCursorIcon(s32 x, s32 y, s32 z, s8 index, s32 extra)
{
    s8 id;

    id = cmpGetHeartsIconId(index);
    if (cmpHeartsIconIsVisible(id) != 0) {
        if (datDevilIsRegistered(id) != 0) {
            fclDrawSpriteRes(x, y, 0, NULL, 8, 2,
                (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
        } else {
            fclDrawSpriteRes(x, y, 0, NULL, 8, 1,
                (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
        }
    } else {
        fclDrawSpriteRes(x, y, 0, NULL, 8, 0,
            (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    }
}

extern void fclDrawSpriteSizeAlphaFilter(s32 x, s32 y, s32 d, s32 w, s32 h, u8 mode,
    s32 *color, s32 id, f32 angle, u16 idx, s32 *texs, fclSpriteTbl_t *tbl, s32 ot);

void fclDrawArrowCursorBox(s32 x, s32 y, s32 z, s32 w, s32 h, u32 color,
                          s32 id, s32 mode, s32 alpha, f32 angle)
{
    s32 colors[4];
    u32 blended;

    id += 43;
    mode = mode != 0;
    if (alpha > 256) {
        alpha = 512 - alpha;
    }
    blended = fclBlendColor(color, color & 0xFF, alpha);
    colors[0] = blended;
    colors[1] = blended;
    colors[2] = blended;
    colors[3] = blended;
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    fclDrawSpriteSizeAlphaFilter(x, y, 0, w, h, mode, colors, 8, angle,
        (u16)id, (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
}

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ u32 color;
    /* 0x08 */ s8 mode;
    /* 0x09 */ s8 direction;
    /* 0x0A */ u8 unk_0A[2];
    /* 0x0C */ s32 directionTimer;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
} campDhEntry_t;

void fclGetArrowCursorEntry(s32 x, s32 y, s32 z, s8 index, s32 extra)
{
    campDhEntry_t *entry;
    s32 inset;

    entry = (campDhEntry_t *)((u8 *)campDhDrawWork + 0x1278) + index;
    inset = 0;
    if (entry->unk_01 < 0) {
        return;
    }
    if (entry->mode < 3) {
        if (entry->mode > 0) {
            inset = (s32)((entry->unk_18 - entry->unk_1C) / 10.0f);
            inset *= 8;
        }
    }
    fclDrawArrowCursorBox(x - inset, y - (inset >> 1), 0,
        (inset >> 3) + 27, (inset >> 3) + 28, entry->color, entry->unk_01,
        0, 256, entry->unk_18);
    x -= 64;
    y -= 32;
    if (entry->mode == 2) {
        f32 angle = entry->unk_18;
        fclDrawArrowCursorBox(x - inset, y - (inset >> 1), 0,
            (inset >> 3) + 35, (inset >> 3) + 36, entry->color, entry->unk_01,
            1, SDF_FRAME_INTERPOLATE_DUE(&entry->unk_2C, entry->unk_2C), angle);
    }
}

extern campDhWork_t *volatile campDhBackgroundWork FAKEMATCH_ASM_NAME("campDhDrawWork");

#ifdef NON_MATCHING
void campDhDrawBackground(s32 x, s32 y, s32 unused, s32 alpha)
{
    campDhWork_t *work;
    s32 colors[4];
    s32 color = 0x8080803C;
    s32 phase;
    s32 distance;
    s32 height = 150;
    s32 yOffset = 0;
    s32 drawY;
    s32 i;

    fclDrawPanelSprite(0x30000, 0x47);
    cmpDrawBoxF(0, 0, 0xFFFFFF, 0x200, 0x1C0, 0, 0x47);
    fclDrawPanelSprite(0x3000F, 0x47);
    work = campDhBackgroundWork;
    for (i = 3; i < 0x21; i++) {
        fclDrawSpriteRes(x, y, 0, NULL, 8, i,
                         (s32 *)((u8 *)work + 0x1160), fclSpriteTable, 0x47);
        work = campDhBackgroundWork;
    }
    fclDrawPanelSprite(0x50000, 0x47);
    work = campDhBackgroundWork;
    phase = ((volatile campDhWork_t *)work)->unk_1C % 128;
    if (phase >= 80) {
        color = 0x80808000 | ((47 - (phase - 80)) * 60 / 47);
    }
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    phase = ((volatile campDhWork_t *)work)->unk_1C % 128;
    distance = (phase * 331 / 127) * 16;
    if (phase >= 80) {
        phase -= 80;
        height = 150 + phase * 8;
        yOffset = (phase * phase * 40 / 2209) * 8;
    }
    drawY = y + yOffset + (141 - (height >> 1)) * 8;
    fclDrawPanelSprite2(0x48, 0x47);
    fclDrawSpriteSize(x + distance - 0x960, drawY, 0, 150, height, colors, 8, 0x29,
                      (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    fclDrawPanelSprite2(0x48, 0x47);
    fclDrawSpriteSize(x - distance + 0x2000, drawY, 0, 150, height, colors, 8, 0x29,
                      (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    fclDrawPanelSprite2(0x44, 0x47);
    fclDrawPanelSprite(0x30000, 0x47);
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0, 0x47);
    fclDrawPanelSprite(0x5000F, 0x47);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDhDrawBackground);
#endif

void fclDrawArrowCursorLabel(s32 x, s32 y, s32 z, s32 index, s32 *color, s32 alpha)
{
    s32 colors[4];
    s32 offsetX;
    s32 offsetY;
    s32 phase;
    s32 width;
    s32 height;

    fclCalcArrowCursorOffset(index, &offsetX, &offsetY);
    if (alpha >= 0) {
        if (alpha <= 256) {
            colors[0] = 0x80808000 | ((256 - alpha) / 2);
            phase = alpha * 240 / 256;
        } else {
            colors[0] = 0x80808000 | (alpha / 2 - 128);
            phase = 0;
        }
        width = (s32)((f32)phase * 1.5f);
        height = (s32)((f32)phase * 0.8f);
        colors[1] = colors[0];
        colors[2] = colors[0];
        colors[3] = colors[0];
        fclDrawSpriteSize(x + offsetX * 16 - width,
            y + offsetY * 8 - (height >> 1), 0, (width >> 3) + 63,
            (height >> 3) + 63, colors, 8, 36,
            (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    }
    fclDrawSpriteRes(x + offsetX * 16, y + offsetY * 8, 0, color, 8, 36,
        (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
}

void fclDrawArrowCursor(s32 x, s32 y, s32 z, s32 extra)
{
    s32 visible;
    s32 count;
    s32 i;
    s32 xOffset;
    s32 yOffset;

    visible = cmpGetHeartsIconId(0x18);
    visible = cmpHeartsIconIsVisible((s8)visible) != 0;
    count = visible + 0x18;
    for (i = 0; i < count; i++) {
        s32 colors[4];
        s8 index = (s8)i;
        s32 alpha;
        s32 color;

        fclCalcArrowCursorOffset((s8)index, &xOffset, &yOffset);
        fclDrawArrowCursorIcon(x + (xOffset << 4), y + (yOffset << 3), 0, (s8)index, extra);
        fclGetArrowCursorEntry(x + (xOffset << 4), y + (yOffset << 3), 0, (s8)index, extra);
        if (cmpGetHeartsIconId((s8)index) == DDS3_GBWK->heartsequip) {
            fclGetArrowCursorTarget(x + (xOffset << 4), y + (yOffset << 3), 0, 0x80808080, 0x100);
        }
        alpha = campDhDrawWork->cursorAlpha[i];
        color = (alpha / 2) & 0xFF;
        color |= 0x80808000;
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        fclDrawArrowCursorLabel(x, y, 0, i, colors, -1);
    }
}

#ifdef NON_MATCHING
extern void campDhDrawBackground(s32 x, s32 y, s32 unused, s32 alpha);
#endif

void campDhDrawPanel(s32 x, s32 y, s32 unused, s32 alpha)
{
    s32 phase;
    s32 colors[4];

    phase = campDhDrawWork->unk_1C % 128;
    if (phase < 16) {
        colors[0] = 0x80808000 | (128 - phase * 4);
    } else {
        colors[0] = 0x80808000 | ((phase - 16) * 64 / 112 + 64);
    }
    colors[1] = colors[0];
    colors[2] = colors[0];
    colors[3] = colors[0];
    fclDrawSpriteRes(x, y, 0, colors, 8, 0x25,
        (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    fclDrawSpriteRes(x, y, 0, colors, 8, 0x26,
        (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    fclDrawSpriteRes(x, y, 0, colors, 8, 0x27,
        (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    fclDrawSpriteRes(x, y, 0, colors, 8, 0x28,
        (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    campDhDrawBackground(x, y, 0, alpha);
    fclDrawArrowCursor(x, y, 0, alpha);
    if ((u8)(campDhDrawWork->flags & 1)) {
        fclDrawArrowCursorLabel(x, y, 0, campDhDrawWork->iconIndex,
            0, campDhDrawWork->entryStep);
    }
}

extern const u32 campDhQuadGaugeCoords[];
extern const u32 campDhQuadGaugeColors[];

void campDhDrawQuadGauge(s32 ratio)
{
    u32 coordinates[6];
    u32 colors[3];

    memcpy(coordinates, campDhQuadGaugeCoords, sizeof(coordinates));
    memcpy(colors, campDhQuadGaugeColors, sizeof(colors));
    coordinates[1] -= (0x100 - ratio) << 5;
    colors[0] = fclBlendColor(colors[0], 0, 0x100);
    cmpDrawTriG(coordinates, coordinates + 3, 0, colors, 0x47);
}

void campDhDrawHelpTextAlt(s32 x, s32 y, s32 z, s32 alpha)
{
    s32 colors[4];
    s32 color;

    color = 0x4D618900 | ((256 - alpha) * 51 / 256);
    colors[0] = 0;
    colors[1] = color;
    colors[2] = 0;
    colors[3] = color;
    cmpDrawBoxGDefault(x + 0xDD0, y + 0xAF8, 0, 52, 5, colors, 0x47);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    cmpDrawBoxGDefault(x + 0x1110, y + 0xAF8, 0, 96, 5, colors, 0x47);
    colors[0] = color;
    colors[1] = 0;
    colors[2] = color;
    colors[3] = 0;
    cmpDrawBoxGDefault(x + 0x1710, y + 0xAF8, 0, 52, 5, colors, 0x47);
}

/* The captured font X coordinate uses its native saved register. At the
 * draw-position consumer, the short-lived font argument binding preserves
 * native argument ordering; $4 is not live across any preceding call.
 * These SMT3-only allocation/scheduling aids emit no instructions and do
 * not establish historical source spelling. The ordinary control swaps
 * four coordinate words; coordinate-only control rotates three arguments. */
extern u8 frEnableMode(s32 flag);
extern u8 frDisableMode(u8 flag);
extern frFontHandle_t *frLinkFontString(u8 *text, s8 style, s8 color, s8 type,
    frFontHandle_t *previous);
extern void frSetFontSpace(frFontHandle_t *font, s32 space);
extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);
extern void frSetPriority(frFontHandle_t *font, u32 priority);
extern void frSetMessageStepY(u32 step);
extern void frSetMessageDefStepY(void);
/* EE prefix-initializer view: the canonical provider takes fclPanel_t*,
 * but its complete native body only stores X/Y/Z/W/H/Col at offsets
 * 0x00/04/08/0C/0E/10. It performs no reads, calls, pointer escape or
 * full-object operation. Explicit casts at this owner's calls retain the
 * existing 0x20-byte prefix storage; they do not assert ISO C compatibility
 * between fclPanelPos_t and the full panel type. */
extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work,
    fclPanel_t *pPanelPos);

void campDhDrawHelpText(s32 x, s32 y, s32 z, s32 alpha, s8 index)
{
    fclPanelPos_t panel;
    frFontHandle_t *font;
    FAKEMATCH_ASM_REG(u32, fontX, "$19") = x + 0x2A0;
    u32 fontY = y + 0x7F0;

    if (index != 0) {
        frEnableMode(1);
        font = frLinkFontString((u8 *)pstr[index], 0, 0, 0, NULL);
        frDisableMode(1);
        frSetFontSpace(font, 3);
        {
            FAKEMATCH_ASM_REG(frFontHandle_t *, drawFont, "$4") = font;
            FAKEMATCH_ASM_READ(fontX);
            frSetDrawPosition(drawFont, fontX, fontY);
        }
        frSetPriority(font, 0);
        frSetFontColor(font, fclBlendColor(0x7475FF80, 0x7475FF00, alpha));
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
        fclInitPanelPos(x + 0xA00, y + 0x990, 0, 0, 0, 0, (fclPanel_t *)&panel);
        frSetMessageStepY(22);
        cmpDrawHelp(&panel, (u16)index, 2, 1, alpha, 0x47);
        frSetMessageDefStepY();
    }
}


extern const u32 campDhGaugePalette[];
extern char campDhFmtAmount[];
extern s8 cmpGetParamGaugeColor(s8 index);
extern s32 cmbGetHeartsParam(s8 devilId, s8 index);
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority,
                                               u32 color, const char *text,
                                               frFontHandle_t *previous);

typedef struct
{
    /* 0x00 */ s32 visible;
    /* 0x04 */ s32 alpha;
    /* 0x08 */ s8 step;
    /* 0x09 */ s8 id;
    /* 0x0A */ u8 pad[2];
    /* 0x0C */ s32 fade;
    /* 0x10 */ s32 fadeStep;
} campDhIconGauge_t;

extern void campDhDrawHelpText(s32 x, s32 y, s32 z, s32 alpha, s8 index);

extern void campDhDrawHeartsGauge(campDhIconGauge_t *gauge, s8 arg);

void campDhDrawHeartsGauge(campDhIconGauge_t *gauge, s8 arg)
{
    char text[0x10];
    s32 colors[4];
    s32 palette[3];
    frFontHandle_t *font;
    s32 baseOt = 0x47;
    /* SMT3-only allocation aid for the live first-phase table. Its consumers
     * preserve $16; no table value is carried into the later drawing phases. */
    FAKEMATCH_ASM_REG(fclSpriteTbl_t *, table, "$16") = fclSpriteTable;
    s32 amount;
    s32 sprite;
    s32 color;
    s32 y;
    s32 i;

    memcpy(palette, campDhGaugePalette, sizeof(palette));

    fclDrawSpriteRes(0, 0x28, 0, NULL, 0xB, 0, (s32 *)((u8 *)campDhDrawWork + 0x1160),
                     table, baseOt);
    fclDrawSpriteRes(0, 0x28, 0, NULL, 0xB, 1, (s32 *)((u8 *)campDhDrawWork + 0x1160),
                     table, baseOt);

    colors[0] = fclBlendColor(0x80808080, 0x40404080, gauge->alpha);
    colors[1] = colors[0];
    colors[2] = colors[0];
    colors[3] = colors[0];

    y = 0x28;
    for (i = 0; i < 5; i++) {
        fclDrawSpriteRes(0, y, 0, NULL, 0xB, 2, (s32 *)((u8 *)campDhDrawWork + 0x1160),
                         table, baseOt);
        fclDrawSpriteRes(0, y, 0, NULL, 0xB, 3, (s32 *)((u8 *)campDhDrawWork + 0x1160),
                         table, baseOt);
        fclDrawSpriteRes(0, y, 0, colors, 0xB, (u16)(i + 4),
                         (s32 *)((u8 *)campDhDrawWork + 0x1160), table, baseOt);
        y += 0xD0;
    }

    y = 0x28;
    if (gauge->visible != 0) {
        for (i = 0; i < 5; i++) {
            s8 parameter = cmpGetParamGaugeColor((s8)i);
            amount = cmbGetHeartsParam(arg, parameter);
            if (amount == 0) {
                sprite = 0xB;
                color = palette[2];
            } else if (amount > 0) {
                sprite = 9;
                color = palette[1];
            } else {
                sprite = 0xA;
                color = palette[0];
            }
            fclDrawSpriteRes(0, y, 0, NULL, 0xB,
                             sprite,
                             (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
            sprintf(text, campDhFmtAmount, amount);
            font = (frFontHandle_t *)frwGetSmallNumFontFRQ(0x5E0, y + 0x910, 0, (u32)color, text, NULL);
            frDrawFontOt(font, 1, 0x47);
            frDestroyFontRequestAfter(font);
            y += 0xD0;
        }
    }

    campDhDrawGaugeBars(0x100);
    campDhDrawHelpText(0, 0x28, 0, gauge->alpha, gauge->id);
    if (gauge->visible == 0) {
        s32 half;
        campDhDrawHelpTextAlt(0, 0x28, 0, gauge->alpha);
        half = (0x100 - gauge->alpha) / 2;
        colors[0] = half | 0x80808000;
        colors[1] = colors[0];
        colors[2] = colors[0];
        colors[3] = colors[0];
        fclDrawSpriteRes(0, 0x28, 0, colors, 0xB, 0xC,
                         (s32 *)((u8 *)campDhDrawWork + 0x1160), fclSpriteTable, 0x47);
    }
    campDhDrawQuadGauge(gauge->alpha);
}



void campDhDrawHeartsIcon(s32 a, s32 b, s32 c, s8 d, s32 e)
{
    campDhWork_t *work = campDhDrawWork;

    if ((work->flags & 1) == 0) {
        if (*((u8 *)DDS3_GBWK + 0x1CFA) == 0) {
            return;
        }
    }
    campDhDrawHeartsGauge((campDhIconGauge_t *)((u8 *)work + 0x1F78), d);
}

extern void campDhDrawBoxSprite(s32 alpha);
extern void campDhDrawPanel(s32 x, s32 y, s32 unused, s32 alpha);

void campDhDrawIcon(void)
{
    campDhDrawBoxSprite(campDhDrawWork->unk_1C);
    if (campDhIconIsReady() != 0) {
        campDhDrawPanel(0, 0, 0, 0x100);
        campDhDrawHeartsIcon(0, 0, 0, cmpGetHeartsIconId(campDhDrawWork->iconIndex), 0x100);
    }
    campDhDrawBox(campDhDrawWork->fadeAlpha);
}

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

#ifdef NON_MATCHING
#endif

void campDhDrawHearts(void)
{
    s8 hearts[6];
    s32 i;

    for (i = 0; i < 6; i++) {
        hearts[i] = cmbGetHeartsParam(DDS3_GBWK->heartsequip, (s8)i);
    }
    cmpDrawStatusComEx(&DDS3_GBWK->unitwork[0], 0x80, -1, 0, hearts, 0, 0xA09DC380,
                       4, 4, 0);
}

extern void campDhDrawIcon(void);

const u32 campDhQuadGaugeCoords[6] = {0x2000, 0x0, 0x2000, 0xE00, 0xE00, 0x6A0};
const u32 campDhQuadGaugeColors[4] = {0xD0D0D80, 0x0, 0x0, 0x0};
const u32 campDhGaugePalette[6] = {0x905BFF80, 0x75CBBD80, 0x76768680, 0x0, 0x0, 0x0};
s32 camp_draw_dh_Process(s32 id)
{
    campInfoWork_t *work = dds3GetProcessData(id);

    campDhDrawWork = (campDhWork_t *)work;
    if ((work->unk_00 & 0x20) != 0) {
        cmpUpdateTerminate((cmpInfoWindow_t *)work);
        return -1;
    }
    if ((work->unk_00 & 2) != 0) {
        switch (work->unk_01) {
        case 0:
        case 3:
            campDhDrawIcon();
            break;
        case 1:
        case 2:
        case 4:
            campDhDrawHearts();
            break;
        }
    }
    return 0;
}


void campDhApplyEntry(s32 button)
{
    s8 index = ((campDhWork_t *)campDhUpdateWork)->iconIndex;
    s32 visible;

    ((campDhWork_t *)campDhUpdateWork)->cursorAlpha[index] = 0x80;
    visible = cmpHeartsIconIsVisible(cmpGetHeartsIconId(0x18)) != 0;
    if (index != 0x18) {
        if (button & 0x10) {
            if (index / 8 == 0) {
                if (visible != 0 && (u32)(index - 3) < 2) {
                    index = 0x18;
                } else {
                    index += 0x10;
                }
            } else {
                index -= 8;
            }
        }
        if (button & 0x20) {
            if ((s8)(index / 8) == 2) {
                if (visible != 0 && (u8)(index - 0x13) < 2) {
                    index = 0x18;
                } else {
                    index -= 0x10;
                }
            } else {
                index += 8;
            }
        }
        if (button & 0x40) {
            if ((index & 7) == 0) {
                index += 7;
            } else {
                index--;
            }
        }
        if (button & 0x80) {
            if ((s8)(index % 8) == 7) {
                index -= 7;
            } else {
                index++;
            }
        }
    } else {
        if (button & 0x10) index = 0x13;
        if (button & 0x20) index = 3;
        if (button & 0x40) index = 0x13;
        if (button & 0x80) index = 0x14;
    }
    {
        campDhWork_t *work = (campDhWork_t *)campDhUpdateWork;
        work->cursorAlpha[index] = 0;
        work->iconIndex = index;
        work->entryStep = 0;
    }
    cmpSetDHEntry(*(s8 *)((u8 *)campDhUpdateWork + ((campDhWork_t *)campDhUpdateWork)->iconIndex * 0x34 + 0x1280));
}

s32 campDhFindEntry(void)
{
    s8 id = cmpGetHeartsIconId(campDhUpdateWork->unk_31);
    s8 i;

    if (cmpHeartsIconIsVisible(id) == 0)
    {
        return 1;
    }
    if (DDS3_GBWK->equipHearts != 0)
    {
        if (id == DDS3_GBWK->equipHearts)
        {
            return 2;
        }
        for (i = 0; i < ((u8 *)DDS3_GBWK)[0x1D7B]; i++)
        {
            if (id == DDS3_GBWK->unk_01CFB[i])
            {
                if ((DDS3_GBWK->party[0].unit.badstatus & 0x8000) != 0)
                {
                    return 4;
                }
            }
        }
    }
    else if ((DDS3_GBWK->party[0].unit.badstatus & 0x8000) == 0)
    {
        return 3;
    }
    return 0;
}

/* The Hearts panel keeps the pre-selection equipment byte and a complete
 * player unit snapshot, restored after the confirmation message. */
typedef struct
{
    u8 unk_00[0x32];
    s8 equipHearts;
    u8 unk_33;
    datUnitWork_t unit;
} campDhSavedUnit_t;

extern void datEquipHearts(s32 HeartsID);

void cmpReturnParamDH(void)
{
    datEquipHearts(((campDhSavedUnit_t *)campDhUpdateWork)->equipHearts);
    DDS3_GBWK->party[0].unit = ((campDhSavedUnit_t *)campDhUpdateWork)->unit;
}


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

/* Exact-type, same-symbol view withholds the earlier provider definition.
 * The direct call leaves three default-tail words different; this conservative
 * visibility boundary restores the native delay slots without changing the
 * provider or call target. This is a reconstruction aid, not historical source. */
extern void campDhHideInfoWindowView(void) FAKEMATCH_ASM_NAME("campHideInfoWindow");
void campDhUpdateCursor(void)
{
    u16 button = (u16)cmpGetPressedButton(0xF3);
    s32 entry;

    if (campDhIconIsReady() == 0) {
        return;
    }
    ((campDhWork_t *)campDhUpdateWork)->flags |= 1;

    switch (button) {
    case 1:
        entry = campDhFindEntry();
        switch (entry) {
        case 0:
        case 2:
        case 3:
        case 4:
            ((u8 *)campDhUpdateWork)[0x18] = 0;
            ((campDhSavedUnit_t *)campDhUpdateWork)->equipHearts = DDS3_GBWK->equipHearts;
            ((campDhSavedUnit_t *)campDhUpdateWork)->unit = DDS3_GBWK->party[0].unit;
            campDhEquipHearts();
            ((u8 *)campDhUpdateWork)[1] = 1;
            ((u8 *)campDhUpdateWork)[4] = 1;
            break;
        case 1:
            button = 0x8000;
            break;
        default:
            break;
        }
        break;
    case 2:
        campDhHideInfoWindowView();
        break;
    case 0x10:
        campDhApplyEntry(0x10);
        break;
    case 0x20:
        campDhApplyEntry(0x20);
        break;
    case 0x40:
        campDhApplyEntry(0x40);
        break;
    case 0x80:
        campDhApplyEntry(0x80);
        break;
    }
    cmpPlayCursorSE(button);
}

s32 cmpUnloadStatusModel(void)
{
    cmpModelFreeResource();
    while (sdfDelayedResrcCheck() != 0) {
    }
    cmpResetDrawEnv();
    campDhUpdateWork->unk_01[0] = 0;
    campDhUpdateWork->unk_04 = 1;
}

extern s8 cmpGetStatusModelFlag(void);
extern s32 fclStartSelMessage(s32 message);
extern void fclSetMessageInvalid(s8 mode);
extern s32 fclStartMessage(s32 message);

void cmpUpdateHeartsAttachStatus(void)
{
    u16 button = cmpGetPressedButton(0x33);

    if (cmpGetStatusModelFlag() != 1) {
        cmpUpdateStatusModelLoad(0);
    }
    if (cmpLoadStatusModel() != 0) {
        return;
    }
    switch (button) {
    case 1:
        if (((campDhSavedUnit_t *)campDhUpdateWork)->equipHearts != 0) {
            if (DDS3_GBWK->heartsequip == ((campDhSavedUnit_t *)campDhUpdateWork)->equipHearts) {
                fclSetMessageVar_20743088(0xC, EE_ADDR_S(pstr[DDS3_GBWK->heartsequip]));
                fclSetMessageInvalid(1);
                fclStartMessage(0);
                campDhUpdateWork->unk_01[0] = 4;
                campDhUpdateWork->unk_04 = button;
                break;
            } else if ((*(u16 *)((u8 *)DDS3_GBWK + 0xA62) & 0x8000) != 0) {
                fclSetMessageInvalid(1);
                fclStartMessage(1);
                campDhUpdateWork->unk_01[0] = 4;
                campDhUpdateWork->unk_04 = button;
                break;
            }
        }
        fclSetMessageVar_20743088(0xC, EE_ADDR_S(pstr[DDS3_GBWK->heartsequip]));
        fclSetMessageInvalid(0);
        fclStartMessage(2);
        fclStartSelMessage(0xB);
        campDhUpdateWork->unk_01[0] = 2;
        campDhUpdateWork->unk_04 = 1;
        break;
    case 2:
        cmpReturnParamDH();
        cmpUnloadStatusModel();
        break;
    case 0x10:
    case 0x20:
        return;
    }
    cmpPlayCursorSE(button);
    /* Like cmpUpdateHeartsAttachConfirm, resetting the handled input retains
     * the native jal plus epilogue; the reset is deleted later. */
    button = 0;
}

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

extern campDhTimerWork_t *campDhTimerWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

#ifdef NON_MATCHING
extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);
#endif

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
        campDhTimerWork->unk_28 += SDF_FRAME_DUE();
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

extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern s8 fclEndMessage(void);
extern void fclSetMessageInvalid(s8 mode);
extern s32 fclStartMessage(s32 message);

extern void cmpStepHeartsAttachCursor(s32 step);

void cmpUpdateHeartsAttachConfirm(void)
{
    s32 cancelled = 1;

    if (((s8 *)campDhUpdateWork)[0x18] > 0) {
        if (((s8 *)campDhUpdateWork)[0x18] == 1) {
            cmpStepHeartsAttachCursor(0);
            p2sdPlaySE(0x18, 0x7F, 0x3F);
            fclSetMessageVar_20743088(0xC, EE_ADDR_S(pstr[DDS3_GBWK->heartsequip]));
            fclSetMessageInvalid(1);
            fclStartMessage(3);
            ((s8 *)campDhUpdateWork)[0x18] = 2;
        }
        cmpStepHeartsAttachCursor(1);
        if (fclChkMessage() == 0) {
            cmpUnloadStatusModel();
        }
    } else if (fclChkSelMessage() == 0) {
        switch (fclGetSelMessagePos()) {
        case 0:
            cancelled = 0;
            /* fall through */
        case 1:
            fclEndMessage();
            if (cancelled != 0) {
                cmpUnloadStatusModel();
                cmpReturnParamDH();
                /* The qualified cc1 trace retains this choice reset through
                 * sibling-call selection and deletes it in the life pass. */
                cancelled = 0;
            } else {
                ((s8 *)campDhUpdateWork)[0x18] = 1;
            }
            break;
        }
    }
}

extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern s8 fclEndMessage(void);
extern void fclSetMessageInvalid(s8 mode);
extern s32 fclStartMessage(s32 message);
extern void datRemoveHearts(void);

void cmpUpdateHeartsRemoveConfirm(void)
{
    s8 phase = ((s8 *)campDhUpdateWork)[0x18];

    if (phase > 0) {
        if (phase == 1) {
            fclSetMessageVar_20743088(0xC, EE_ADDR_S(pstr[*((u8 *)DDS3_GBWK + 0x1CFA)]));
            fclSetMessageInvalid(1);
            fclStartMessage(5);
            datRemoveHearts();
            ((s8 *)campDhUpdateWork)[0x18] = 2;
        }
        if (fclChkMessage() != 0) {
            return;
        }
        campDhUpdateWork->unk_01[0] = 4;
        campDhUpdateWork->unk_04 = 1;
        return;
    }
    if (fclChkSelMessage() != 0) {
        return;
    }
    switch (fclGetSelMessagePos()) {
    case 0:
        ((s8 *)campDhUpdateWork)[0x18] = 1;
        break;
    case 1:
        if (((s8 *)campDhUpdateWork)[0x18] == 0) {
            ((s8 *)campDhUpdateWork)[0x18] = 2;
        }
        fclEndMessage();
        break;
    }
}

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
    campDhTimerWork->unk_1C += SDF_FRAME_DUE();
    if (campDhTimerWork->unk_1C >= 0x400) {
        campDhTimerWork->unk_1C = 0;
    }
    campDhTimerWork->unk_20 += SDF_FRAME_STEP(8);
    if (campDhTimerWork->unk_20 >= 0x200) {
        campDhTimerWork->unk_20 = 0;
    }
    if (campDhTimerWork->unk_24 < 0x200) {
        campDhTimerWork->unk_24 += SDF_FRAME_STEP(0x20);
    } else {
        campDhTimerWork->unk_24 += SDF_FRAME_STEP(0x10);
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
        alpha = work->fadeAlpha + SDF_FRAME_STEP(0x20);
        work->fadeAlpha = alpha;
        if (alpha >= 0x101) {
            work->fadeAlpha = 0x100;
        }
    } else {
        alpha = work->fadeAlpha - SDF_FRAME_STEP(0x20);
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
            campDhTimerWork->iconTimer[i] -= SDF_FRAME_STEP(0x10);
        }
        if (campDhTimerWork->iconTimer[i] < 0) {
            campDhTimerWork->iconTimer[i] = 0;
        }
    }
}

extern u32 dds3GetRandA(void *state);

void campDhRollRatio(campDhEntry_t *entry)
{
    s32 frames;
    s32 delay;

    if (entry->unk_00 == 1) {
        if (entry->directionTimer > 0) {
            entry->directionTimer--;
        }
        if (entry->directionTimer == 0) {
            switch (entry->direction) {
            case 1: entry->unk_1C = -10.0f; break;
            case 2: entry->unk_1C = 10.0f; break;
            default: entry->unk_1C = 0.0f; break;
            }
        }
        entry->unk_00 = 0;
        entry->unk_20 = entry->unk_1C - entry->unk_18;
        frames = SDF_FRAMES(dds3GetRandA(NULL) % 5 + 1);
        entry->unk_24 = frames;
        entry->unk_28 = frames;
        entry->unk_20 /= (f32)frames;
    } else {
        switch (entry->mode) {
        case 1: delay = (dds3GetRandA(NULL) & 31) + 20; break;
        case 2: delay = dds3GetRandA(NULL) & 7; break;
        default: return;
        }
        entry->unk_14 = SDF_FRAMES(delay);
        entry->unk_00 = 1;
        entry->unk_1C = entry->unk_18;
        entry->unk_20 = (f32)(dds3GetRandA(NULL) % 40) - 20.0f;
        frames = SDF_FRAMES(dds3GetRandA(NULL) % 5 + 1);
        entry->unk_24 = frames;
        entry->unk_28 = frames;
        entry->unk_20 /= (f32)frames;
        if (entry->mode == 2) {
            if (entry->directionTimer <= 0) {
                entry->direction = dds3GetRandA(NULL) % 3;
                entry->directionTimer = SDF_FRAMES(dds3GetRandA(NULL) % 10);
            }
        } else {
            entry->directionTimer = -1;
        }
    }
}

void campDhGetEntryRatio(campDhEntry_t *entry)
{
    if (entry->unk_14 > 0) {
        entry->unk_14--;
    } else if (entry->unk_28 == 0) {
        campDhRollRatio(entry);
        /* Retail uses jal plus the shared epilogue. The matched
         * cmpUpdateStatusModelLoad sibling uses the same post-call reset
         * to keep the final call from becoming a sibling call. */
        entry = NULL;
    } else {
        entry->unk_28--;
        entry->unk_18 += entry->unk_20;
    }
}

typedef struct
{
    u8 unk_0000[0x1278];
    campDhEntry_t entries[25];
} campDhEntryWork_t;

extern campDhEntryWork_t *campDhEntryWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

void campDhScanEntries(void)
{
    s32 ofs;
    s32 i;

    for (ofs = 0, i = 0x18; i >= 0; i--, ofs += sizeof(campDhEntry_t)) {
        campDhEntry_t *entry = (campDhEntry_t *)((u8 *)campDhEntryWork->entries + ofs);

        if (entry->unk_01 < 0) {
            continue;
        }
        campDhGetEntryRatio(entry);
        if (entry->unk_10 > 0) {
            entry->unk_10 -= 1;
            continue;
        }
        if ((entry->unk_2C = entry->unk_2C + entry->unk_30 * SDF_FRAME_DUE()) >= 0x201) {
            entry->unk_2C = 0;
            SDF_FRAME_INTERPOLATE_RESET(&entry->unk_2C);
        }
    }
}


void campDhFindVisibleIcon(void)
{
    campDhIconGauge_t *gauge = (campDhIconGauge_t *)((u8 *)campDhUpdateWork + 0x1F78);
    s8 id;

    if (campDhIconIsReady() == 0) {
        return;
    }
    id = cmpGetHeartsIconId(campDhUpdateWork->unk_31);
    if (cmpHeartsIconIsVisible(id) != 0) {
        gauge->id = id;
        gauge->visible = 1;
    } else {
        gauge->visible = 0;
    }
    if (gauge->visible != 0) {
        if (gauge->alpha < 0x100) {
            gauge->alpha = gauge->alpha + SDF_FRAME_STEP_INT(gauge->step);
        }
        if (gauge->alpha >= 0x101) {
            gauge->alpha = 0x100;
        }
    } else {
        if (gauge->alpha > 0) {
            gauge->alpha = gauge->alpha - SDF_FRAME_STEP_INT(gauge->step);
        }
        if (gauge->alpha < 0) {
            gauge->alpha = 0;
        }
    }
    if (gauge->fade < 0x100) {
        gauge->fade = gauge->fade + SDF_FRAME_STEP_INT(gauge->fadeStep);
    }
    if (gauge->fade >= 0x101) {
        gauge->fade = 0x100;
    }
}


s32 camp_update_dh_Process(s32 id)
{
    campInfoWork_t *work = dds3GetProcessData(id);
    s8 state = work->unk_01;

    campDhUpdateWork = (campStatusPanel_t *)work;
    if ((work->unk_00 & 0x40) != 0) {
        return campDhCheckReady() != 0 ? 0 : -1;
    }
    switch (state) {
    case 0:
        campDhUpdateCursor();
        break;
    case 1:
        cmpUpdateHeartsAttachStatus();
        break;
    case 2:
        cmpUpdateHeartsAttachConfirm();
        break;
    case 3:
        cmpUpdateHeartsRemoveConfirm();
        break;
    case 4:
        campDhWaitMessageEnd();
        break;
    }
    campDhScanEntries();
    campDhFindVisibleIcon();
    campDhUpdateIcons();
    campDhAdvanceAnim();
    campDhFadeAlpha();
    cmpUpdateSeqInfo(campDhUpdateWork, state);
    return 0;
}


extern void cmpSetSkillListRows(s32 rows);

void campDrawSkillPanel(void)
{
    cmpSetSkillListRows(0x14);
    campDrawText(0, 0);
    cmpSetSkillListRows(-1);
    campWork->drawPhase = 0;
}

void cmbSetBirthPanel(datUnitWork_t *panel)
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
    memset(cmpGaugeTimers, 0, 0x14);
    return cmpStatusAnimCount[index];
}

void func_00235E40(s8 index)
{
    cmpGaugeTimers[index] = 1;
}

s32 func_00235E60(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        if (cmpGaugeTimers[i] != 0) {
            return 1;
        }
    }
    return 0;
}

void cmpGetGaugeBlinkAlpha(s32 frame, s8 state, u32 *colors)
{
    s32 ratio = 0;
    u32 color;
    if (frame % 0x30 < 0x18) {
        switch (state) {
        case 0:
        case 3:
            break;
        case 1:
        case 4:
        case 5:
            ratio = 0x100 - (frame % 0x18) * 10;
            break;
        case 2:
            ratio = (frame % 0x18) * 10;
            break;
        }
    } else {
        switch (state) {
        case 0:
        case 3:
            break;
        case 1:
        case 4:
        case 5:
            ratio = 0x100 - (0x18 - cmpStatusAnimCount[0] % 0x18) * 10;
            break;
        case 2:
            ratio = (0x18 - frame % 0x18) * 10;
            break;
        }
    }
    color = fclBlendColor(0x80808080, 0x80808000, ratio);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
}

s32 cmpGetPanelAlphaOrDefault(u8 *alpha, s32 color)
{
    s32 value = 0x80;

    if (alpha != NULL) {
        value = *alpha;
    }
    return (color & ~0xFF) | value;
}

extern s32 cmpStatTimer;
extern void cmpGetGaugeBlinkAlpha(s32 frame, s8 state, u32 *colors);
extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *colors,
                                s32 page, s32 spriteId, s32 *texList,
                                fclSpriteTbl_t *table, s32 ot);
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority,
                                               u32 color, const char *text,
                                               frFontHandle_t *previous);
extern char cmpFmtDec2[];

void rstDrawNumber(s32 x, s32 y, s8 number)
{
    u32 colors[4];
    char text[0x10];
    frFontHandle_t *font;
    s32 yBase = y - 0x14;
    s32 spriteY = yBase << 3;

    fclDrawSpriteFilter(x << 4, spriteY, 0, 0, NULL, 6, 0,
                        D_00345D88, fclSpriteTable, 0x47);
    if (number > 0) {
        if (cmpStatTimer < 0x18) {
            cmpStatTimer += SDF_FRAME_DUE();
        }
    } else if (cmpStatTimer > 0) {
        cmpStatTimer -= SDF_FRAME_DUE();
    }
    cmpGetGaugeBlinkAlpha(cmpStatTimer, 2, colors);
    fclDrawSpriteRes((x + 0x7E) << 4, (yBase + 0x92) << 3, 0, (s32 *)colors,
                     6, 1, D_00345D88, fclSpriteTable, 0x47);
    if (number >= 10) {
        fclDrawSpriteRes(x << 4, spriteY, 0, (s32 *)colors,
                         6, 1, D_00345D88, fclSpriteTable, 0x47);
    }
    sprintf(text, cmpFmtDec2, number);
    font = frwGetSmallNumFontFRQ(x + 0x7B0, yBase + 0x420, 0xFFFF,
                                 0xA09DC380, text, NULL);
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
}

extern char cmpFmtValMax[];
extern void fclDrawSpriteRect(s32 x, s32 y, s32 z, s32 width, s32 height,
                              s32 *colors, s32 page, s32 spriteId, s32 *texList,
                              fclSpriteTbl_t *table, s32 ot);

#ifdef NON_MATCHING
void cmpDrawStatusIconRow(s32 x, s32 y, s32 z, s32 color,
                          datUnitWork_t *unit, s32 flags, s32 ot)
{
    volatile datUnitWork_t *live = unit;
    u8 mask = flags;
    u16 badstatus = live->badstatus;
    s32 kind = (badstatus & 0x800) != 0;
    s32 value = 1;
    s32 maximum = 1;
    s32 colors[4];
    s32 textColor;
    s32 ratio;
    s32 sprite;
    char text[16];
    frFontHandle_t *font;

    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    if (mask & 5) {
        value = live->hp;
        maximum = live->maxhp;
    }
    if (mask & 0xA) {
        maximum = live->maxmp;
        value = live->mp;
        kind |= (badstatus & 0x800) != 0 ? 4 : 2;
    }
    if (value >= 1000) {
        value = 999;
    }
    if (maximum >= 1000) {
        maximum = 999;
    }
    if (kind & 1) {
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x754B3D80);
    } else {
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
    }
    if (mask & 0xC) {
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xFFBD8380);
    }
    fclDrawSpriteRes(x, y, z * 16, colors, 2, 6, D_00345D88, fclSpriteTable, ot);
    fclDrawSpriteRes(x, y, z * 16, colors, 2, 5, D_00345D88, fclSpriteTable, ot);
    ratio = value * 1000 / maximum;
    if (ratio != 0) {
        switch ((kind & 0xE) >> 1) {
        case 1:
            sprite = 8;
            break;
        case 2:
            sprite = 0xB;
            break;
        default:
            sprite = 4;
            break;
        }
        fclDrawSpriteRect(x, y, z * 16, ratio, 1000, colors, 2, sprite,
                          D_00345D88, fclSpriteTable, ot);
    }
    sprintf(text, cmpFmtValMax, value);
    font = frwGetSmallNumFontFRQ(x + 0x50, y, z, textColor, text, NULL);
    sprintf(text, cmpFmtValMax, maximum);
    font = frwGetSmallNumFontFRQ(x + 0x2C0, y, z, textColor, text, font);
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusIconRow);
#endif

extern void cmpDrawStatusIconRow(s32 x, s32 y, s32 z, s32 color,
                                 datUnitWork_t *unit, s32 flags, s32 ot);

void cmpDrawStatusIcons(s32 x, s32 y, s32 *colors, s32 mode, datUnitWork_t *unit)
{
    s32 hpIconFlags;
    s32 mpIconFlags;

    if (mode == 3) {
        hpIconFlags = cmbBirthPanel->maxhp != unit->maxhp ? 4 : 1;
        mpIconFlags = cmbBirthPanel->maxmp != unit->maxmp ? 8 : 2;
    } else {
        hpIconFlags = 1;
        mpIconFlags = 2;
    }

    fclDrawSpriteRes(x, y, 0xFFFF, colors, 2, 3, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(x, y, 0xFFFF, colors, 2, 7, D_00345D88, fclSpriteTable, 0x47);
    cmpDrawStatusIconRow(x + 0x2B0, y + 0x3C8, 0xFFFF, colors[0], unit, hpIconFlags, 0x47);
    cmpDrawStatusIconRow(x + 0x960, y + 0x3C8, 0xFFFF, colors[0], unit, mpIconFlags, 0x47);
}

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawStatusName(s32 x, s32 y, u8 *alpha, datUnitWork_t *unit)
{
    volatile s32 colors[4];
    s32 *texList;
    fclSpriteTbl_t *spriteTable;
    s32 mode = 0;
    u16 status = unit->badstatus;
    u32 flags = unit->flag;

    if (status & 0x800) mode = 1;
    if (flags & 0x10) mode = 2;
    if (status & 0x8000) mode = 3;
    if (mode == 0) return;

    if (mode == 3) {
        colors[0] = 0x09051C00;
        colors[1] = 0x09051C40;
    } else {
        colors[0] = 0;
        colors[1] = 0x40;
    }
    {
        s32 first = colors[0];
        s32 second = colors[1];

        colors[2] = first;
        colors[3] = second;
    }
    cmpDrawBoxGDefault(0x11B0, 0, 0, 0xE5, 0x1C0, (s32 *)colors, 0x47);
    colors[2] = 0x1D053400;
    colors[3] = 0x01010120;
    colors[0] = 0x1D053400;
    colors[1] = 0x01010120;
    cmpDrawBoxGDefault(0x4A0, 0, 0, 0x1B6, 0x1C0, (s32 *)colors, 0x47);
    texList = D_00345D88;
    spriteTable = fclSpriteTable;
    fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x12, texList, spriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x15, texList, spriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x16, texList, spriteTable, 0x47);
    switch (mode) {
    case 1:
        fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x13, texList, spriteTable, 0x47);
        break;
    case 2:
        fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x14, texList, spriteTable, 0x47);
        break;
    case 3:
        fclDrawSpriteRes(0, 0, 0, NULL, 2, 0x17, texList, spriteTable, 0x47);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusName);
#endif

extern char *datExplainTable[];
extern u32 strlen(const char *text);

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawStatusGauge(s32 x, s32 y, s8 index, s32 intensity)
{
    s8 length = strlen(datExplainTable[index + 0x40]) >> 1;
    s32 width = length * 20;
    s32 height = 20;
    s32 delta = (s32)(0x100U - (u32)intensity);
    s32 colors[4];
    s32 color;

    x = (x + 0x10) * 16;
    y = (y + 0x43) * 8;
    if (intensity < 0x100) {
        s32 half = delta / 2;

        width += delta;
        height += delta;
        x -= half * 16;
        y -= half * 8;
    }
    color = fclBlendColor(0x80808080, 0x80808000, intensity);
    colors[0] = colors[1] = colors[2] = colors[3] = color;
    if (intensity > 0) {
        fclDrawSpriteSize(x, y, 0, width, height, colors, 6, 1,
                          D_00345D88, fclSpriteTable, 0x47);
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusGauge);
#endif

#include "font.h"

extern s32 datGetPlayerLNDLevel(datUnitWork_t *unit);
extern u16 datGetLevelForDraw(datUnitWork_t *unit);
extern s32 datAdvanceDevilLevel(datUnitWork_t *unit);
extern frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);
extern datDevilTableEntry_t datDevilTable[];
extern char *datExplainTable[];
extern char cmpFmtExpNext[];
extern s16 cmpHeartsMasterPhase FAKEMATCH_ASM_NAME("rstHeartsMasterPhase");
extern datUnitWork_t *volatile cmpStatusBirthLive FAKEMATCH_ASM_NAME("cmbBirthPanel");
extern void cmpDrawStatusBadge(s32 x, s32 y, s32 *colors, u16 statusFlags, s32 unused);

/* NON_MATCHING. Behaviour checked against retail: 2 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawStatusRow(s32 x, s32 y, s32 *colors, s32 mode, datUnitWork_t *unit)
{
    volatile datUnitWork_t *live = unit;
    s32 textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
    s32 levelColor;
    s32 spriteX = x * 16;
    s32 spriteY = y * 8;
    s32 nameX = (x + 0x58) * 16;
    s32 badgeX;
    s32 length;
    frFontHandle_t *font;
    frFontHandle_t *number;
    char text[16];

    if (mode == 3) {
        datUnitWork_t *birth = cmpStatusBirthLive;
        u16 level = live->level;
        u16 birthLevel = ((volatile datUnitWork_t *)birth)->level;
        if (level != birthLevel) {
            levelColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xFFBD8380);
        } else {
            if (live->badstatus & 0x800) {
                textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x754B3D80);
            }
            levelColor = textColor;
        }
    } else {
        if (live->badstatus & 0x800) {
            textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x754B3D80);
        }
        levelColor = textColor;
    }
    cmpDrawStatusName(x, y, (u8 *)colors, unit);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 0, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 1, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 2, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 3, D_00345D88, fclSpriteTable, 0x47);
    if (unit->flag & 4) {
        s32 level;
        s32 raceColor = textColor;

        cmpSetSkillListRows(0x14);
        level = datGetPlayerLNDLevel(unit);
        if (mode == 2) {
            s16 phase;
            raceColor = fclBlendColor(0x80C08080, textColor, cmpHeartsMasterPhase);
            phase = cmpHeartsMasterPhase;
            if (phase > 0) {
                phase -= SDF_FRAME_STEP(8);
                cmpHeartsMasterPhase = phase;
            }
            if (phase < 0) {
                phase = 0;
                cmpHeartsMasterPhase = 0;
            }
            cmpDrawStatusGauge(x, y, (s8)level, phase);
        }
        font = frwGetSmallFontFRQ2((x + 0xA) * 16, (y + 0x42) * 8, 0xFFFF,
                                   raceColor, datExplainTable[level + 0x40], NULL);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
        cmpSetSkillListRows(-1);
        cmpSetSkillListRows(0x14);
        frEnableMode(1);
        font = frGetNameFRQ(0, NULL, 0);
        frSetDrawPosition(font, nameX, (y + 0x43) * 8);
        frSetFontColor(font, textColor);
        frSetPriority(font, 0xFFFF0);
        frSetFontSpace(font, 2);
        frDisableMode(1);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
        cmpSetSkillListRows(-1);
    } else {
        u16 id;
        frFontHandle_t *race;

        cmpSetSkillListRows(0x14);
        id = live->id;
        race = frwGetSmallFontFRQ2((x + 0xA) * 16, (y + 0x42) * 8, 0xFFFF,
                                   textColor, datExplainTable[datDevilTable[id].race], NULL);
        cmpSetSkillListRows(0x14);
        id = live->id;
        font = frwGetSmallFontFRQ2(nameX, (y + 0x43) * 8, 0xFFFF,
                                   textColor, text2[id], race);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
        cmpSetSkillListRows(-1);
    }
    length = font->advance;
    if (x + (nameX >> 4) + length < 0xB7) {
        badgeX = (x + 0xB7) * 16;
    } else {
        badgeX = nameX + (x + length + 2) * 16;
    }
    cmpDrawStatusBadge(badgeX, (y + 0x41) * 8, colors, live->badstatus, 0x100);
    fclDrawSpriteRes(spriteX, spriteY, 0, colors, 2, 0, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY, 0, colors, 2, 1, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY, 0, colors, 2, 2, D_00345D88, fclSpriteTable, 0x47);
    sprintf(text, cmpFmtDec2, datGetLevelForDraw(unit));
    number = frwGetSmallNumFontFRQ((x + 0x2C) * 16, (y + 0x62) * 8,
                                   0xFFFF, levelColor, text, NULL);
    frDrawFontOt(number, 1, 0x47);
    frDestroyFontRequestAfter(number);
    sprintf(text, cmpFmtExpNext, datAdvanceDevilLevel(unit) - live->exp);
    number = frwGetSmallNumFontFRQ((x + 0x7C) * 16, (y + 0x62) * 8,
                                   0xFFFF, textColor, text, NULL);
    frDrawFontOt(number, 1, 0x47);
    frDestroyFontRequestAfter(number);
    cmpDrawStatusIcons(spriteX, spriteY, colors, mode, unit);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawStatusRow);
#endif

extern s32 cmbGetHeartsParamEx(s8 devilId, s8 index, s32 registered);
extern s32 cmbGetParam(datUnitWork_t *unit, s32 index);
extern volatile s32 cmpGaugeAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawParamGauge(s32 x, s32 y, s32 *colors, s32 rowHeight,
                       s8 index, s8 bonus, s8 mode, datUnitWork_t *unit)
{
    s32 hearts = 0;
    s8 value;
    s8 i;
    s32 marked = 0;
    s32 rowY;
    u32 blinkColors[4];

    if ((unit->flag & 4) != 0) {
        s8 parameter = cmpGetParamGaugeColor(index);
        s8 equipped = (s8)DDS3_GBWK->heartsequip;
        if (mode == 5) {
            hearts = cmbGetHeartsParamEx(equipped, parameter, 0);
        } else {
            hearts = cmbGetHeartsParam(equipped, parameter);
        }
    }
    value = bonus + cmbGetParam(unit, cmpGetParamGaugeColor(index));
    if (mode == 4) {
        value -= hearts;
    } else if (mode == 5) {
        hearts = (s8)(hearts + bonus);
    }
    if (hearts < 0) {
        value -= hearts;
    }
    if (value > 0) {
        s32 low = value - hearts;
        s32 high = value + hearts;
        s32 drawX = x * 16;

        rowY = y + index * rowHeight;
        for (i = 0; i < value && i < 40; i++) {
            s32 sprite = 2;
            if (hearts > 0) {
                if (i >= low) {
                    sprite = marked ? 3 : 4;
                    marked = 1;
                }
            } else if (i >= high) {
                sprite = marked ? 5 : 6;
                marked = 1;
            }
            fclDrawSpriteRes(drawX, rowY * 8, 0, colors, 4, sprite,
                             D_00345D88, fclSpriteTable, 0x47);
            drawX += 0x50;
        }
    }
    if (bonus == 0) {
        return;
    }
    rowY = y + index * rowHeight;
    switch (mode) {
    case 0:
        if (cmpGaugeAnimLive[0] % 48 != 0) {
            mode = 2;
        } else {
            cmpParamGaugeState[0] = 0;
        }
        if (cmpParamGaugeState[0] == 0) {
            mode = 0;
        }
        /* fall through */
    case 1:
    case 2: {
        s32 left;
        s32 right;
        s32 leftX;
        s32 rightX;

        cmpGetGaugeBlinkAlpha(cmpGaugeAnimLive[0], mode, blinkColors);
        i = value - bonus;
        left = i - hearts;
        right = i + hearts;
        leftX = left * 5;
        rightX = right * 5;
        while (i < value) {
            if ((hearts < 0 ? right : left) >= 40) {
                return;
            }
            fclDrawSpriteRes((x + (hearts < 0 ? rightX : leftX)) * 16,
                             rowY * 8, 0, (s32 *)blinkColors, 6, 3,
                             D_00345D88, fclSpriteTable, 0x47);
            leftX += 5;
            rightX += 5;
            left++;
            right++;
            i++;
        }
        break;
    }
    case 3: {
        s32 position = hearts >= 0 ? value - hearts : value + hearts;
        s32 *timer = &cmpGaugeTimers[index];
        s32 frame = *timer;

        if (frame != 0) {
            frame++;
            *timer = frame;
            if (frame >= SDF_FRAMES(24)) {
                *timer = 0;
                return;
            }
            cmpGetGaugeBlinkAlpha(SDF_FRAME_TICK(frame), 0, blinkColors);
            fclDrawSpriteRes((x + position * 5 - 5) * 16, rowY * 8, 0,
                             (s32 *)blinkColors, 6, 3, D_00345D88, fclSpriteTable, 0x47);
        }
        break;
    }
    case 4:
    case 5:
        cmpGetGaugeBlinkAlpha(cmpGaugeAnimLive[0], mode, blinkColors);
        for (i = value - bonus; i < value && i < 40; i++) {
            fclDrawSpriteRes((x + i * 5) * 16, rowY * 8, 0, (s32 *)blinkColors,
                             6, 0xB, D_00345D88, fclSpriteTable, 0x47);
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawParamGauge);
#endif

extern s32 cmbGetParam(datUnitWork_t *unit, s32 index);
extern void cmpDrawParamGauge(s32 x, s32 y, s32 *colors, s32 rowHeight,
                              s8 index, s8 bonus, s8 mode, datUnitWork_t *unit);
extern void fclDrawSpriteAlpha(s32 x, s32 y, s32 z, u8 mode, s32 *colors,
                                s32 page, s32 spriteId, s32 *texList,
                                fclSpriteTbl_t *table, s32 ot);

#ifdef NON_MATCHING
void cmpDrawParamPanel(s32 x, s32 y, s32 *colors, s8 *bonuses, datUnitWork_t *unit,
                       s8 selected, s8 selectMode, s8 mode)
{
    s32 spriteX = x * 16;
    s32 spriteY = y * 8;
    s32 numberX = (u16)((x + 0x2E) * 16);
    s32 numberY = (u16)((y + 0xA5) * 8);
    s8 gaugeMode = mode;
    s32 i;
    char text[16];

    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 4, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 5, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 6, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(spriteX, spriteY + 4, 0, colors, 1, 7, D_00345D88, fclSpriteTable, 0x47);
    for (i = 0; i < 5; i++) {
        s8 parameter = cmpGetParamGaugeColor((s8)i);
        s32 textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
        s8 bonus = 0;
        s32 value;
        frFontHandle_t *font;

        if (selected >= 0 && selectMode == 2 && i == selected) {
            textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
        }
        if (bonuses != NULL) {
            bonus = bonuses[parameter];
        }
        value = cmbGetParam(unit, parameter);
        if (gaugeMode != 4) {
            value += bonus;
        }
        if (value >= 0x29) {
            value = 0x28;
        }
        fclDrawSpriteFilter(spriteX, spriteY + i * 0xA0, 0, 0, colors, 4, 0,
                            D_00345D88, fclSpriteTable, 0x47);
        sprintf(text, cmpFmtDec2, value);
        font = frwGetSmallNumFontFRQ(numberX, numberY, 0xFFFF, textColor, text, NULL);
        frDrawFontOt(font, 1, 0x47);
        numberY += 0xA0;
        frDestroyFontRequestAfter(font);
        if (selected >= 0) {
            gaugeMode = 2;
        }
        cmpDrawParamGauge(x, y, colors, 0x14, (s8)i, bonus, gaugeMode, unit);
        fclDrawSpriteAlpha(spriteX, spriteY + i * 0xA0, 0, 2, colors, 4, 1,
                           D_00345D88, fclSpriteTable, 0x47);
    }
    if (selected >= 0 && selectMode == 2) {
        fclDrawSpriteRes(spriteX, (y + selected * 0x14) * 8, 0, colors, 6, 2,
                         D_00345D88, fclSpriteTable, 0x47);
    }
    for (i = 0; i < 5; i++) {
        if (selected == i && selectMode == 2) {
            fclDrawSpriteRes(spriteX, spriteY, 0, colors, 6, i + 5,
                             D_00345D88, fclSpriteTable, 0x47);
        } else {
            fclDrawSpriteRes(spriteX, spriteY, 0, colors, 2, i + 0xC,
                             D_00345D88, fclSpriteTable, 0x47);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawParamPanel);
#endif

extern datDevilTableEntry_t datDevilTable[];
extern char *datExplainTable[];
extern s16 datGetDevilParam(u8 devilId);

void cmpDrawStatusValue(s32 x, s32 y, s32 *colors, s32 mode, datUnitWork_t *unit)
{
    s32 color;
    s32 affinity;
    frFontHandle_t *font;

    if (mode == 4) {
        color = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x64B2B580);
        goto color_ready;
    } else {
        color = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
        goto color_ready;
    }
color_ready:
    if (unit->flag & 4) {
        affinity = datGetDevilParam(DDS3_GBWK->heartsequip);
    } else {
        affinity = datDevilTable[unit->id].aisyoid;
    }
    font = frwGetSmallFontFRQ2((s32)(((u32)x + 0x18u) << 4),
                               (s32)(((u32)y + 0x115u) << 3),
                               0xFFFFF, color, datExplainTable[affinity + 0x50], 0);
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
}

void cmpDrawStatusBars(s32 x, s32 y, u8 *alpha)
{
    s32 colors[4];
    u32 a = 0x80;
    u32 color;
    u32 scaled;

    if (alpha != NULL) {
        a = *alpha;
    }
    color = (a >> 1) | 0x80808000;
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteRes(x << 4, y << 3, 0, colors, 1, 8, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(x << 4, y << 3, 0, colors, 1, 9, D_00345D88, fclSpriteTable, 0x47);
    scaled = a * 3;
    color = ((scaled + (scaled << 4)) >> 7) | 0x80808000;
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    fclDrawSpriteRes(x << 4, y << 3, 0, colors, 1, 10, D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(x << 4, y << 3, 0, colors, 1, 11, D_00345D88, fclSpriteTable, 0x47);
}

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawSkillRowText(s32 x, s32 y, s32 *colors, datUnitWork_t *unit,
                         s32 mode, s8 *count)
{
    s32 row;
    s32 col;
    s32 i;

    if ((unit->flag & 4) != 0 && DDS3_GBWK->equipHearts == 0) {
        fclDrawSpriteRes(x * 16, y * 8 + 4, 0, colors, 1, 0xC,
                         D_00345D88, fclSpriteTable, 0x47);
        fclDrawSpriteRes(x * 16, y * 8 + 4, 0, colors, 1, 0xD,
                         D_00345D88, fclSpriteTable, 0x47);
        fclDrawSpriteRes(x * 16, y * 8 + 4, 0, colors, 1, 0xE,
                         D_00345D88, fclSpriteTable, 0x47);
        fclDrawSpriteRes(x * 16, y * 8 + 4, 0, colors, 1, 0xF,
                         D_00345D88, fclSpriteTable, 0x47);
    } else {
        s32 alpha = colors != NULL ? *(u8 *)colors : 0x80;
        cmpDrawBoxF(x * 16, (y + 0x136) * 8, 0, 0x200, 0x61,
                    alpha * 13 / 64, 0x47);
    }
    for (row = 0; row < 4; row++) {
        s32 sprite = row < 3 ? 1 : 2;
        for (col = 0; col < 2; col++) {
            fclDrawSpriteRes(x * 16 + col * 0x740, (y + row * 0x16) * 8,
                             0, colors, 5, sprite, D_00345D88, fclSpriteTable, 0x47);
        }
    }
    for (i = 0; i < *count; i++) {
        s32 column = i / 4;
        s32 line = i % 4;
        s32 xOffset = column * 116;
        s32 yOffset = line * 22;

        fclDrawSpriteRes((x + xOffset) * 16, (y + yOffset) * 8, 0, colors,
                         5, 3, D_00345D88, fclSpriteTable, 0x47);
        if (mode == 4) {
            fclDrawPanelSprite2(0x48, 0x47);
            cmpDrawBoxF((xOffset + 0x101) * 16, (yOffset + 0x13C) * 8,
                        0, 0x71, 0x12, 0x171580, 0x47);
            fclDrawPanelSprite2(0x44, 0x47);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillRowText);
#endif

extern volatile s32 cmpStatusAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

#ifdef NON_MATCHING
void cmpDrawSkillIcons(s32 x, s32 y, s8 index)
{
    s32 count;
    s32 row;
    s32 column;
    u8 i;

    if (cmpParamGaugeState[1] == 0) {
        return;
    }
    row = index / 4;
    column = index % 4;
    x += row * 116;
    y += column * 22;
    if (index >= 8) {
        x++;
        y--;
    }
    count = cmpStatusAnimLive[1];
    if (count / 2 <= 0) {
        return;
    }
    i = 0;
    do {
        s32 intensity;
        s32 colors[4];

        count = cmpStatusAnimLive[1];
        intensity = (s32)(0x80U - (u32)count * 4 - (u32)i * 16);
        if (intensity < 0) {
            intensity = 0;
        }
        if (intensity != 0) {
            s32 color = intensity | 0x80808000;

            colors[0] = colors[1] = colors[2] = colors[3] = color;
            fclDrawSpriteSize((x - i) * 16, (y - i) * 8, 0,
                              i * 2 + 0x89, i * 2 + 0x2B, colors,
                              6, 10, D_00345D88, fclSpriteTable, 0x47);
            count = cmpStatusAnimLive[1];
        }
        i++;
    } while (i < count / 2);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillIcons);
#endif

extern s32 cmpFindSkillEntry(u16 skillId);
extern char *text1[];
extern volatile s32 cmpSkillAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq32, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawSkill(s32 x, s32 y, s32 *colors, datUnitWork_t *unit,
                  rstSkillInfo_t *skills, s8 selected, s8 selectMode,
                  s32 specialColor, s8 highlightMode, s32 rowMode)
{
    volatile datUnitWork_t *liveUnit = unit;
    s32 textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
    s32 i;

    cmpDrawSkillRowText(x, y, colors, unit, rowMode, &skills->SkillCnt);
    cmpSetSkillListRows(0x12);
    for (i = 0; i < liveUnit->skillcnt; i++) {
        s32 column = i / 4;
        s32 row = i % 4;
        u16 skill;
        frFontHandle_t *font;

        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0xA09DC380);
        if (selected >= 0 && selectMode == 1 && i == selected) {
            textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
        }
        if (highlightMode == 2 && cmpFindSkillEntry(liveUnit->skill[i]) != 0) {
            textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
        }
        if (selected >= 0 && (selectMode == 1 || selectMode == 3) && i == selected) {
            textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
        }
        skill = liveUnit->skill[i];
        font = frwGetSmallFontFRQ2(0, 0, 0xFFFFF, textColor, text1[skill], NULL);
        frSetDrawPosition(font, (x + 0x18 + column * 0x74 + (0x74 - font->advance) / 2) * 16,
                          (y + 0x13B + row * 0x16) * 8);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);
    }
    cmpSetSkillListRows(-1);
    if ((selected >= liveUnit->skillcnt && selectMode == 1) || highlightMode == 1) {
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
    } else if (highlightMode == 0 || highlightMode == 2) {
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, specialColor);
    }
    cmpSetSkillListRows(0x12);
    for (i = 0; i < skills->SkillCnt; i++) {
        s32 column = i / 4;
        s32 row = i % 4;
        s32 dx = column * 0x74;
        s32 dy = row * 0x16;

        if (i == 0 && (skills->skill[0] != 0x165 || evtBitCheck(0x8C1) != 0)) {
            frFontHandle_t *font;
            if (selected >= 0 && selectMode == 1 && selected == 8) {
                textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x80C08080);
            }
            font = frwGetSmallFontFRQ2(0, 0, 0xFFFFF, textColor, text1[skills->skill[i]], NULL);
            frSetDrawPosition(font, (x + 0x101 + dx + (0x72 - font->advance) / 2) * 16,
                              (y + 0x13C + dy) * 8);
            frDrawFontOt(font, 1, 0x47);
            frDestroyFontRequestAfter(font);
        } else {
            fclDrawSpriteRes((x + dx) * 16, (y + dy) * 8, 0, colors, 5, 0,
                             D_00345D88, fclSpriteTable, 0x47);
        }
        textColor = cmpGetPanelAlphaOrDefault((u8 *)colors, 0x4D4B4B4D);
    }
    cmpSetSkillListRows(-1);
    if (highlightMode == 1 || selectMode == 3) {
        cmpDrawSkillIcons(x, y, (s8)rstSkillActFlag);
    }
    if (selected >= 0 && selectMode == 1) {
        s32 dx = (selected / 4) * 0x74;
        s32 dy = (selected % 4) * 0x16;
        s32 frame;
        s32 expansionX;
        s32 expansionY;
        s32 color;
        s32 pulseColors[4];

        if (selected >= 8) {
            dx++;
        }
        fclDrawSpriteRes((x + dx) * 16, (y + dy) * 8, 0, NULL, 6, 4,
                         D_00345D88, fclSpriteTable, 0x47);
        frame = cmpSkillAnimLive[2];
        if (frame < 0x100) {
            frame += SDF_FRAME_STEP(8);
            cmpSkillAnimLive[2] = frame;
            if (frame >= 0x100) {
                cmpSkillAnimLive[2] = 0;
            }
        } else {
            cmpSkillAnimLive[2] = 0;
        }
        frame = cmpSkillAnimLive[2];
        expansionX = frame * 24 / 256;
        expansionY = frame / 64;
        dy -= expansionY / 2;
        dx -= expansionX / 2;
        color = ((0x100 - frame) / 2) | 0x80808000;
        pulseColors[0] = color;
        pulseColors[1] = color;
        pulseColors[2] = color;
        pulseColors[3] = color;
        fclDrawSpriteSize((x + dx) * 16, (y + dy) * 8, 0,
                          expansionX + 0x76, expansionY + 0x13, pulseColors,
                          6, 4, D_00345D88, fclSpriteTable, 0x47);
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkill);
#endif


void cmpDrawStatusCom(datUnitWork_t *a, s8 b, s8 c, s8 d)
{
    cmpDrawStatusComEx(a, 0x80, -1, 0, 0, 0, 0xA09DC380, b, c, d);
}


void cmpDrawStatusComEx(datUnitWork_t *a, u8 alpha, s8 b, s8 c, s8 *d, s8 e, s32 color, s8 f, s8 g, s8 h)
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
    if (!SDF_FRAME_DUE()) return;
    if (cmpStatusAnimCount[0] < 0x30) {
        cmpStatusAnimCount[0]++;
    } else {
        cmpStatusAnimCount[0] = 0;
    }
    if (cmpStatusAnimCount[1] < 0x3C) {
        cmpStatusAnimCount[1]++;
    }
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x10];
} cmpModelLink_t;

extern cmpModelLink_t sdfModelOtTable[];

extern void cmpDrawStatusRow(s32, s32, s32 *, s32, datUnitWork_t *);
extern void cmpDrawParamPanel(s32, s32, s32 *, s8 *, datUnitWork_t *, s8, s8, s8);
extern void cmpDrawStatusValue(s32, s32, s32 *, s32, datUnitWork_t *);
extern u16 itfMesMngGetStatManager(void);
extern void rstCreateBeforeSkillList(s8, datUnitWork_t *, s8, rstSkillInfo_t *);
extern void cmpDrawSkill(s32, s32, s32 *, datUnitWork_t *, rstSkillInfo_t *, s8, s8,
                         s32, s8, s32);
extern s32 cmpDrawStatusModel(void *);

void cmpDrawStatusComEx2(datUnitWork_t *unit, s32 x, s32 y, u8 alpha,
                         s8 kind, s8 detail, s8 *delta, s8 mode, u32 color,
                         s8 show, s8 option, s8 extra, s8 full)
{
    s32 xOffset[3];
    s32 yOffset[3];
    rstSkillInfo_t skillData;
    s32 colors[4];
    datUnitWork_t target;
    memset(xOffset, 0, sizeof(xOffset));
    memset(yOffset, 0, sizeof(yOffset));
    colors[0] = (alpha & 0xFF) | 0x80808000;
    colors[1] = (alpha & 0xFF) | 0x80808000;
    colors[2] = (alpha & 0xFF) | 0x80808000;
    colors[3] = (alpha & 0xFF) | 0x80808000;

    switch (show) {
    case 0:
    case 1:
    case 4:
        break;
    case 2:
        yOffset[0] = -30;
        yOffset[1] = 0;
        yOffset[2] = 20;
        break;
    case 3:
        if (cmbBirthPanel == NULL) {
            show = 1;
        }
        break;
    }

    cmpCalcTargetDevil(unit, &target);
    cmpDrawStatusRow(x + xOffset[0], y + yOffset[0], colors, show, unit);
    cmpDrawParamPanel(x + xOffset[1], y + yOffset[1], colors, delta, unit,
                      kind, detail, option);
    cmpDrawStatusBars(x + xOffset[1], y + yOffset[1] + 4, (u8 *)colors);
    cmpDrawStatusValue(x + xOffset[1], y + yOffset[1] + 4, colors, show, unit);

    if ((itfMesMngGetStatManager() & 2) == 0 || show == 2) {
        rstCreateBeforeSkillList(DDS3_GBWK->heartsequip, &target, mode, &skillData);
        cmpDrawSkill(x + xOffset[2], y + yOffset[2], colors, &target,
                     &skillData, kind, detail, 0x4D4B4B4D, extra, show);
    }

    if (show == 0) {
        fclDrawSpriteRes(x + xOffset[2], y + yOffset[2], 0, colors, 0, 0x20, D_00345D88,
                         fclSpriteTable, 0x47);
        fclDrawSpriteRes(x + xOffset[2], y + yOffset[2], 0, colors, 2, 0x11, D_00345D88,
                         fclSpriteTable, 0x47);
    }
    if (full != 0) {
        cmpDrawStatusModel(sdfModelOtTable);
    }
    cmpAdvanceStatusAnim();
}

extern char *pStr[];

extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                               const char *text, frFontHandle_t *previous);

void cmpDrawItemListHeader(s32 x, s32 y, s32 c, u16 index)
{
    void *frq = frwGetSmallFontFRQ2(x, y, 0xFFFFFF, c, pStr[index], 0);

    frDrawFontOt(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

/* Called before its definition without a parameter prototype. */
extern void cmpDrawScrollThumb();

void cmpDrawItemListScrollBar(s32 alpha, cmpCursorInfo_t *cursor)
{
    if (cursor->unk_20 != 0) {
        if (cursor->unk_22 != 0) {
            cmpDrawScrollThumb(0, 0, 0x10, 0x80808080, 0, alpha, 0x47);
        } else {
            cmpDrawScrollThumb(0, 0, 0x10, 0x80808080, 2, alpha, 0x47);
        }
        if (cursor->unk_22 + cursor->unk_26 != cursor->unk_20) {
            cmpDrawScrollThumb(0, 0, 0x10, 0x80808080, 1, alpha, 0x47);
        } else {
            cmpDrawScrollThumb(0, 0, 0x10, 0x80808080, 3, alpha, 0x47);
        }
        cmpDrawScrollBar(0, 0x280, 0x10, 0xA4, 0x16, 0x80808080, alpha,
                         cursor->unk_24, cursor, 0, 0x47);
    }
}

/* NON_MATCHING: 2 of 46 words differ (first at +0x90), exact size; the last
   call's two argument setups are scheduled the other way round. */
#ifdef NON_MATCHING
void cmpDrawItemListRow(s32 x, s32 y, s32 z, s32 row, s32 ot)
{
    s32 colors[4];
    s32 color = row * 38 / 0x100;

    colors[2] = 0;
    colors[3] = 0;
    colors[0] = color;
    colors[1] = color;
    fclDrawPanelSprite(0x1003, ot);
    cmpDrawBoxGDefault(x, y, z, 0x95, 0xB6, colors, ot);
    fclDrawPanelSprite(0x5101B, ot);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawItemListRow);
#endif


/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
extern s32 cmpCursorBlinkColor(s32 timer);
extern s32 cmpCursorFadeChannel(s32 color, s32 timer, s32 rate);
extern void cmpDrawListFrame(s32 x, s32 y, s32 z, s32 rowHeight,
                             s8 rowCount, s32 alpha, s32 ot);
extern void cmpSetCursorSlot(s8 index, s8 state, u8 param,
                             cmpCursorSlot_t *slot);
extern s32 cmpItemIsUsable(u16 item);
extern s32 cmpItemHasFlag(u16 item, s8 which);
extern s32 cmpAllocEffectSlot(s32 color, s8 selected, s8 special,
                              cmpCursorSlot_t *slot);
extern void cmpSetSkillListRows(s32 rows);
extern void cmpDrawItemListHeader(s32 x, s32 y, s32 color, u16 item);
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *colors, s32 page,
                             s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 ot);
extern frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 width,
                                               u32 color, const char *text,
                                               frFontHandle_t *previous);
extern s32 frDrawFontOt(frFontHandle_t *font, s8 mode, u32 ot);
extern void cmpDrawItemListRow(s32 x, s32 y, s32 z, s32 row, s32 ot);
extern void fclInitPanelPos(s32 x, s32 y, s32 z, s16 w, s16 h,
                            s32 work, fclPanel_t *panel);
extern void cmpDrawHelp(fclPanelPos_t *panel, u16 index, s8 bank,
                        s8 skipBackground, s32 alpha, u32 ot);
extern void cmpDrawItemListScrollBar(s32 alpha, cmpCursorInfo_t *cursor);
extern void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor);
extern s32 fclChkMessage(void);
extern s32 D_00345D88[5];
extern fclSpriteTbl_t fclSpriteTable[];
extern const char cmpFmtItemQty[];

void cmpCalcItemEntry(fclPanelPos_t *pos, cmpCursorInfo_t *cursor,
                      s32 callbackContext, s8 callbackFlag, s32 alpha,
                      s32 ot)
{
    s32 x;
    s32 rowY = 0;
    s32 blinkLow;
    s32 selectedRow;
    s32 start;
    cmpCursorSlot_t (*slots)[2];
    s32 foldedAlpha;
    s32 blinkColor;
    s32 fontColor;
    s32 itemY;
    s32 row;
    u8 itemRows;
    s32 colorParts[4];
    char numberText[16];
    fclPanelPos_t helpPanel;

    (void)callbackContext;
    (void)ot;

    x = pos->unk_00;
    itemY = pos->unk_04;
    itemRows = (u8)cursor->unk_26;
    selectedRow = (u8)cursor->unk_25;
    start = (u8)cursor->unk_22;
    slots = (cmpCursorSlot_t (*)[2])((u8 *)cursor + 0x3C);
    blinkColor = cmpCursorBlinkColor(alpha);
    blinkLow = (u8)blinkColor;
    cmpDrawListFrame(0, 0, 0x10, 0x17, 8, alpha, 0x47);

    foldedAlpha = alpha;
    if (alpha >= 0x100) foldedAlpha = 0x200 - alpha;

    row = start;
    if (campWork->unk_8302 != 0) {
        while (row < start + itemRows) {
            u16 item = campWork->itemId[row];
            s32 rowColor;
            s32 selected = 0;
            s32 special = 0;

            if (item != 0) {
                rowColor = 0xA09DC380;
                if (callbackFlag >= 0 && row - start == selectedRow) {
                    cmpSetCursorSlot(0, 1, 0x10,
                                     &slots[selectedRow + start][0]);
                    selected = 1;
                    rowColor = 0x89FEFF80;
                }

                if ((s8)campWork->itemMode == 0) {
                    if (campWork->page[0].cursor.unk_22 +
                            (u8)campWork->page[0].cursor.unk_24 != 1) {
                        if (cmpItemIsUsable(item) != 0) {
                            rowColor = 0x4D4B4B4D;
                            special = 1;
                            if (selected != 0) rowColor = 0x8EBEF52B;
                        }
                    } else if (cmpItemHasFlag(item, 1) == 0) {
                        rowColor = 0x4D4B4B4D;
                        if (selected != 0) rowColor = 0x8EBEF52B;
                        special = 1;
                    }
                }

                {
                    s32 allocated = cmpAllocEffectSlot(
                        rowColor, (s8)selected, (s8)special,
                        &slots[row][0]);
                    s32 color = (allocated & 0xFFFFFF00) |
                                ((((u32)rowColor & 0xFF) * (u32)blinkLow) >> 7);
                    frFontHandle_t *font;
                    s32 quantity;

                    fontColor = cmpCursorFadeChannel(color, alpha, 0);
                    cmpSetSkillListRows(0x12);

                    item = campWork->itemId[row];
                    cmpDrawItemListHeader(x - 0x5A, itemY,
                                          fontColor, item);

                    colorParts[0] = blinkColor;
                    colorParts[1] = blinkColor;
                    colorParts[2] = blinkColor;
                    colorParts[3] = blinkColor;
                    fclDrawSpriteRes(0x40, rowY, 0x10, colorParts, 7, 2,
                                     D_00345D88, fclSpriteTable, 0x47);

                    item = campWork->itemId[row];
                    quantity = DDS3_GBWK->itemQuantity[item];
                    if ((s8)campWork->itemMode != 2) {
                        sprintf(numberText, cmpFmtItemQty, quantity);
                    } else if (quantity < 2) {
                        sprintf(numberText, cmpFmtItemQty, 1);
                    } else {
                        sprintf(numberText, cmpFmtItemQty, quantity);
                    }

                    font = frwGetSmallNumFontFRQ(x + 0x810,
                                                 itemY + 0x10, 0x10,
                                                 (u32)fontColor, numberText, NULL);
                    frDrawFontOt(font, 1, 0x47);
                    frDestroyFontRequestAfter(font);
                }

                rowY += 0xB8;
                itemY += 0xB8;
            }
            row++;
        }
        fclInitPanelPos(0x120, 0x988, 0x10, 0, 0, 0, (fclPanel_t *)&helpPanel);
        cmpDrawHelp(&helpPanel, campWork->itemId[cursor->unk_22 + (u8)cursor->unk_24],
                    1, 0, foldedAlpha, 0x47);
        cmpSetSkillListRows(-1);
    } else {
        s32 halfColor = 0x80808000 | (foldedAlpha / 2);
        cmpDrawItemListRow(0xF0, 0x278, 0x10, foldedAlpha, 0x47);
        colorParts[0] = halfColor;
        colorParts[1] = halfColor;
        colorParts[2] = halfColor;
        colorParts[3] = halfColor;
        fclDrawSpriteRes(0, 0, 0x10, colorParts, 0, 0x1F,
                         D_00345D88, fclSpriteTable, 0x47);
    }

    if (callbackFlag == 0 && alpha < 0x101) {
        if (fclChkMessage() != 0) {
            cursor->unk_34 = 0;
        } else {
            cmpDrawItemListScrollBar(alpha, cursor);
        }
    }
    cmpUpdateCursorSlots(cursor);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpCalcItemEntry);
#endif

void campDrawInfoPanel(void)
{
    campDrawText(4, 0);
    campWork->drawPhase = 0;
}

/* This exact-type view withholds the new same-unit body from the dispatcher.
 * The paired direct call changes its branch/epilogue layout; the linker target
 * and truthful provider remain unchanged. This is not an original-source claim. */
extern void campDrawTextView(s32 id, s32 *colors) FAKEMATCH_ASM_NAME("campDrawText");

void campDrawSeq3(s8 phase)
{
    if (phase != 0) {
        if (phase != 2) {
            if (phase != 3) {
                return;
            }
        }
    }

    switch (phase) {
    case 0:
        campDrawTextView(3, 0);
        /* fall through */
    case 3:
        campWork->drawPhase = 0;
        break;
    case 2:
        campWork->drawPhase = 2;
        break;
    }
}

extern char *text1[];

void cmpDrawSkillListHeader(s32 x, s32 y, s32 c, u16 index)
{
    void *frq;

    cmpSetSkillListRows(0x12);
    frq = frwGetSmallFontFRQ2(x, y, 0xFFFFFF, c, text1[index], 0);
    frDrawFontOt(frq, 1, 0x47);
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

extern const u8 D_0053FEB8[];
extern char cmpFmtSkillCost[];

#ifdef NON_MATCHING
void cmpDrawSkillListRow(s32 x, s32 y, s32 textColor, u16 unusedSkill,
                         s8 rowFlag, s8 selected, u16 *work, u16 skill, s32 alpha)
{
    char buffer[0x20];
    s32 colors[4];
    s32 adjustedAlpha;
    s32 scaled;
    s32 sprite;
    s32 icon;
    frFontHandle_t *font;

    buffer[0] = D_0053FEB8[0];
    memset(buffer + 1, 0, 0x1F);
    adjustedAlpha = alpha;
    if (alpha >= 0x101) {
        adjustedAlpha = 0x200 - alpha;
    }
    scaled = (s32)(adjustedAlpha * 96);
    if (scaled < 0) {
        scaled += 0xFF;
    }
    colors[0] = (scaled >> 8) | 0x80808000;
    colors[1] = colors[0];
    colors[2] = colors[0];
    colors[3] = colors[0];

    if (skill >= 0x120) {
        if (selected != 0 || rowFlag == 0) {
            sprite = 0x14;
        } else {
            sprite = 0x15;
        }
        fclDrawSpriteRes(x + 0x120, y + 0x38, 0x10, colors, 7, sprite,
                         D_00345D88, fclSpriteTable, 0x47);
    } else {
        sprintf(buffer, cmpFmtSkillCost, cmpGetSkillCost(skill, work));
        font = (frFontHandle_t *)frwGetSmallNumFontFRQ(x, y + 0x10, 0x10, (u32)textColor, buffer, NULL);
        frDrawFontOt(font, 1, 0x47);
        frDestroyFontRequestAfter(font);

        sprite = 2;
        if (selected == 0) {
            sprite = rowFlag != 0;
        }
        icon = cmpGetSkillIcon(skill);
        if (icon == 1) {
            sprite += 0xE;
        } else if (icon == 2) {
            sprite += 0x11;
        } else {
            return;
        }
        fclDrawSpriteRes(x + 0x230, y + 0x20, 0x10, colors, 7, sprite,
                         D_00345D88, fclSpriteTable, 0x47);
    }
}

#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillListRow);
#endif

void cmpDrawSkillListScrollBar(s32 a, cmpCursorInfo_t *cursor)
{
    if (cursor->unk_20 != 0) {
        cmpDrawScrollBar(0, 0x280, 0x10, 0xA4, 0x16, 0x80808080, a, cursor->unk_24, cursor,
                         0, 0x47);
    }
}

extern s32 cmpCursorBlinkColor(s32 timer);
extern s32 cmpCursorFadeChannel(s32 color, s32 timer, s32 rate);
extern void cmpDrawListFrame(s32 x, s32 y, s32 unusedZ, s32 rowHeight,
                             s8 rowCount, s32 alpha, s32 ot);
extern void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot);
extern s32 cmpAllocEffectSlot(s32 color, s8 selected, s8 disabled,
                              cmpCursorSlot_t *slot);

#ifdef NON_MATCHING
void cmpDrawSkillList(s32 *position, cmpCursorInfo_t *cursor, datUnitWork_t *unit,
                      s8 mode, s32 alpha)
{
    volatile cmpCursorInfo_t *liveCursor = cursor;
    volatile datUnitWork_t *liveUnit = unit;
    s32 start = *(volatile u8 *)&cursor->unk_22;
    s32 rows = liveCursor->unk_26;
    volatile s32 *livePosition = position;
    s32 y = livePosition[1];
    s32 x = livePosition[0];
    s32 selectedRow = (u8)liveCursor->unk_25;
    s32 blink = cmpCursorBlinkColor(alpha) & 0xFF;
    s32 i;
    fclPanelPos_t helpPosition;

    cmpDrawListFrame(0, 0, 0x10, 0x17, 8, alpha, 0x47);
    for (i = start; i < start + rows && i < 24; i++) {
        s32 selected = 0;
        s32 disabled = 0;
        s32 color = 0xA09DC380;
        cmpCursorSlot_t *slot = (cmpCursorSlot_t *)((u8 *)cursor + 0x3C + i * 0x10);

        if (liveUnit->skill[i] == 0) {
            continue;
        }
        if (mode >= 0 && i - start == selectedRow) {
            color = 0x89FEFF80;
            selected = 1;
            cmpSetCursorSlot(0, 1, 0x10, slot);
        }
        if (cmpChkDoNotUseSkill(liveUnit->skill[i]) != 0) {
            color = selected ? 0x8EBEF52B : 0x4D4B4B4D;
            disabled = 1;
        }
        color = (cmpAllocEffectSlot(color, selected, disabled, slot) & ~0xFF) |
                (((color & 0xFF) * blink) >> 7);
        color = cmpCursorFadeChannel(color, alpha, 0);
        cmpDrawSkillListHeader(x, y, color, liveUnit->skill[i]);
        cmpDrawSkillListRow(x + 0x740, y, color, blink, selected, disabled,
                            (u16 *)unit, liveUnit->skill[i], alpha);
        y += 0xB8;
    }
    fclInitPanelPos(0x120, 0x988, 0x10, 0, 0, 0, (fclPanel_t *)&helpPosition);
    {
        u8 row = liveCursor->unk_24;
        s16 offset = cursor->unk_22;
        cmpDrawHelp(&helpPosition, liveUnit->skill[offset + row], 0, 0,
                    alpha >= 0x100 ? 0x200 - alpha : alpha, 0x47);
    }
    if (mode == 0 && alpha <= 0x100) {
        if (fclChkMessage() != 0) {
            cursor->unk_34 = 0;
        } else {
            cmpDrawSkillListScrollBar(alpha, cursor);
        }
    }
    cmpUpdateCursorSlots(cursor);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawSkillList);
#endif

void cmpExpandColor(u64 *dst, u32 color)
{
    u64 wide = color;

    dst[0] = ((color >> 24) & 0xFF) | ((u64)((color >> 16) & 0xFF) << 32);
    dst[1] = ((wide & 0xFF00) >> 8) | ((u64)(color & 0xFF) << 32);
}


extern sdfDrawTag_t cmpViewTag[2];
extern sdf3DInitPacket_t cmpViewInitPacket[2];
extern sdfDrawTag_t cmpLightTag[2];
extern sdf3DLightParallelPacket_t cmpLightPacket[2];
extern sdf3DViewportPacket_t cmpViewportPacket[2];
extern sdfMatrix4_t sdfViewMatrix;
extern dds3LightState_t dds3LightParam;
SYMBOL_AT(sdfVector4_t, dds3AmbientColor, dds3LightParam, 0x10);
extern f32 aViewDown[4];
extern void LookAtMatrix(f32 *, f32 *, f32 *);
extern void sdf3DMakeScreenParam(sdf3DScreenParam_t *);
extern void sdfDrawTagInit(sdfDrawTag_t *);
extern void sdf3DLinkInitPacket(sdfDrawTag_t *, sdf3DInitPacket_t *);
extern void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *, sdf3DScreenParam_t *, sdfMatrix4_t *);
extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

void cmpDrawSetup3DView(void)
{
    s32 i;

    sdf3DMakeScreenParam((sdf3DScreenParam_t *)dds3ScreenParam);
    LookAtMatrix(aViewTarget, aViewEye, aViewDown);
    SDF_VU0_STOREMATRIX(&sdfViewMatrix);
    for (i = 0; i < 2; i++) {
        sdfDrawTagInit(&cmpViewTag[i]);
        sdf3DLinkInitPacket(&cmpViewTag[i], &cmpViewInitPacket[i]);
        sdf3DMakeViewportPacket(&cmpViewportPacket[i], (sdf3DScreenParam_t *)dds3ScreenParam, &sdfViewMatrix);
        sdfDrawTagLink(&cmpViewTag[i], EE_ADDR(&cmpViewportPacket[i]));
        sdfDrawTagInit(&cmpLightTag[i]);
        sdf3DMakeLightParallelPacket(&cmpLightPacket[i], dds3LightParam.parallel, &dds3AmbientColor);
        sdfDrawTagLink(&cmpLightTag[i], EE_ADDR(&cmpLightPacket[i]));
    }
}


#ifdef NON_MATCHING
extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);
extern s32 sdfGrAllocPacketBuff(s32 aSize);
extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr);
#endif

void cmpDrawSpriteFlag(u8 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id != 0;
    if (flag == 0) {
        prim[1] = 0x4A;
    } else {
        prim[1] = 0x4B;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}


void func_00239C08(u8 id, s32 arg)
{
    cmpDrawSpriteFlag(id, 0, arg);
}

void cmpDrawSprite(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x47;
    } else {
        prim[1] = 0x48;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void fclDrawPanelSprite(s32 id, s32 arg)
{
    cmpDrawSprite(id, 0, arg);
}

void cmpDrawSprite2(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x42;
    } else {
        prim[1] = 0x43;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void fclDrawPanelSprite2(s32 id, s32 arg)
{
    cmpDrawSprite2(id, 0, arg);
}

void cmpDrawSpriteIndexed(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x49;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawSpriteIndexed2(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x14;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
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

void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot)
{
    u32 colors[3];

    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    cmpDrawTriG(xs, ys, z, colors, ot);
}

void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot)
{
    void *packet;
    struct { u64 colorChannels[2]; u64 packedXY; u64 depth; } *prim;
    sdfDrawTag_t *tag;
    sdfGrOt_t *otp;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(6, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x14B, 6, 0x515151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim[0].colorChannels, colors[0]);
    prim[0].packedXY = (xs[0] + 0x7000) | ((u64)(ys[0] + 0x7900) << 32);
    prim[0].depth = z;
    cmpExpandColor(prim[1].colorChannels, colors[1]);
    prim[1].packedXY = (xs[1] + 0x7000) | ((u64)(ys[1] + 0x7900) << 32);
    prim[1].depth = z;
    cmpExpandColor(prim[2].colorChannels, colors[2]);
    prim[2].packedXY = (xs[2] + 0x7000) | ((u64)(ys[2] + 0x7900) << 32);
    prim[2].depth = z;
    tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h)
{
    s32 colors[4];

    colors[3] = color;
    colors[1] = color;
    colors[2] = color;
    colors[0] = color;
    cmpDrawBoxG(a, b, c, d, e, colors, g, h);
}

void cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    cmpDrawBoxSolid(a, b, c, d, e, f, 0, g);
}

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawBoxG(s32 x, s32 y, s32 z, s32 width, s32 height,
                 s32 *colors, s32 mode, s32 ot)
{
    sdfGrPacketHeader_t *packet;
    sdfDrawTag_t *tag;
    sdfGrOt_t *surface = &sdfGrOtTable[ot];
    u64 *prim;
    volatile u64 *livePrim;
    volatile s32 *liveColors = colors;
    u32 left = x + 0x7000;
    u32 top = y + 0x7900;
    u32 right = left + width * 16;
    u32 bottom = top + height * 8;
    u32 color;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(8, 1)));
    sdf2DMakePrimPacketHeader(packet, (mode << 9) | 0x14C, 8, 0x51515151, 1);
    prim = sdf2DGetPrimPtr(packet);
    livePrim = prim;
    cmpExpandColor(prim, liveColors[0]);
    livePrim[2] = left | ((u64)top << 32);
    livePrim[3] = (u32)z;
    cmpExpandColor(prim + 4, liveColors[1]);
    color = liveColors[2];
    livePrim[7] = (u32)z;
    livePrim[6] = right | ((u64)top << 32);
    cmpExpandColor(prim + 8, color);
    color = liveColors[3];
    livePrim[11] = (u32)z;
    livePrim[10] = left | ((u64)bottom << 32);
    cmpExpandColor(prim + 12, color);
    livePrim[15] = (u32)z;
    livePrim[14] = right | ((u64)bottom << 32);
    tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    surface->regFunc(surface, tag);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawBoxG);
#endif

void cmpDrawBoxGDefault(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *f, s32 g)
{
    cmpDrawBoxG(a, b, c, d, e, f, 0, g);
}

#ifdef NON_MATCHING
extern void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
#endif

void cmp_23A360(s32 x, s32 y, s32 z, s32 w, s32 h, s32 g, s32 col)
{
    s32 right = x + w * 16;

    cmpDrawPanelEffCell(x, y, z, right, y, z, g, col);
    {
        s32 bottom = y + h * 8;

        cmpDrawPanelEffCell(x, y, z, x, bottom, z, g, col);
        cmpDrawPanelEffCell(right, y, z, right, bottom, z, g, col);
        cmpDrawPanelEffCell(x, bottom, z, x + ((w + 1) << 4), bottom, z, g, col);
    }
}


void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    u32 pair[2];

    pair[1] = g;
    pair[0] = g;
    cmpDrawBoxLine(a, b, c, d, e, f, pair, h);
}

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                    const u32 *colors, s32 surfaceIndex)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    void *packet;
    u64 *prim;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x149, 4, 0x5151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim, colors[0]);
    prim[2] = (x0 + 0x7000) | ((u64)(y0 + 0x7900) << 32);
    prim[3] = z0;
    cmpExpandColor(prim + 4, colors[1]);
    prim[6] = (x1 + 0x7000) | ((u64)(y1 + 0x7900) << 32);
    prim[7] = z1;
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, EE_ADDR(packet));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

#ifdef NON_MATCHING
extern u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 (*aAllocator)(s32));
extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 (*aAllocator)(s32));
#endif

void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex)
{
    sdf3DPrimPolyParm_t request;
    u32 vertexColors[3];
    f32 textureCoordinates[3][2];
    sdfVector4_t positions[3] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    u32 firstColor, secondColor, thirdColor;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    if (asset != NULL) {
        sdf3DPrimLinkMat(list, asset, 0);
    }
    memset(&request, 0, sizeof(request));
    firstColor = colors[0];
    secondColor = colors[1];
    thirdColor = colors[2];
    request.faceCount = 1;
    request.pointCount = 3;
    request.color = firstColor;
    request.vertex = positions;
    request.normal = NULL;
    request.uv = NULL;
    vertexColors[0] = firstColor;
    vertexColors[1] = secondColor;
    vertexColors[2] = thirdColor;
    request.vertexColor = vertexColors;
    SDF_VU0_COPYVECTOR(&positions[0], first);
    SDF_VU0_COPYVECTOR(&positions[1], second);
    SDF_VU0_COPYVECTOR(&positions[2], third);
    textureCoordinates[0][0] = 0.0f;
    textureCoordinates[0][1] = 0.0f;
    textureCoordinates[1][0] = 0.0f;
    textureCoordinates[1][1] = 0.0f;
    textureCoordinates[2][0] = 0.0f;
    textureCoordinates[2][1] = 0.0f;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&request)));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

void cmpDisableRGB(s32 ot)
{
    sdfGrOt_t *otp;
    void *tag;
    void *mask;

    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    mask = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x40));
    sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(tag, EE_ADDR(mask));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpAllocRefGifTag(s32 ot)
{
    sdfGrOt_t *otp;
    void *tag;
    void *mask;

    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    mask = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x40));
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, mask, 0);
    sdfDrawTagLinkRefGif(tag, EE_ADDR(mask));
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

/* This target-specific constraint completes the real sprite ID before the
 * tail-call argument copy/restores. Ordinary and nonvolatile forms retain a
 * four-word scheduling difference. It emits no instructions, is not a memory
 * barrier, and does not establish the original source spelling. */
void cmp_23A918(s32 arg)
{
    func_00239C08(0, arg);
    cmpAllocRefGifTag(arg);
    {
        s32 spriteId = 0x5C000;
        FAKEMATCH_ASM_LIVE_VOLATILE(spriteId);
        fclDrawPanelSprite(spriteId, arg);
    }
}

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

extern void mdlLink(void *aHandle, cmpModelLink_t *aModelOt);

void cmp_23AA28(f32 *a, f32 *rot, void *model, s32 index)
{
    func_0023A9A8(a, model);
    func_0023A9F0(rot, model);
    mdlLink(model, &sdfModelOtTable[index]);
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

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} cmpPrintControl_t;

extern void sdfDevSetPrintCB(cmpPrintControl_t *cb, s32 x, s32 y, s32 z, u32 attr);

extern void *sdfDevPrintfEx(cmpPrintControl_t *cb, const char *str, ...);

void cmpDrawDebugPrint(s32 x, s32 y, s32 z, u32 attr, const char *str, s32 ot)
{
    cmpPrintControl_t cb;
    sdfGrOt_t *otp;
    void *tag;

    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDevSetPrintCB(&cb, x * 16 + 0x7000, y * 8 + 0x7900, z, attr);
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintfEx(&cb, str)));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

extern void *dds3PrimFrameRect(s32 x, s32 y, s32 c, s32 w, s32 h, s32 f, s32 g);

void cmpDrawFrameRect(s32 x, s32 y, s32 c, s32 w, s32 h, s32 f, s32 g, s32 ot)
{
    sdfGrOt_t *otp;
    void *tag;
    void *packet;

    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    packet = dds3PrimFrameRect(x * 16 + 0x7000, y * 8 + 0x7900, c, w * 16, h * 8, f, g);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}


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
    return DDS3_GBWK->party[panel->slot[panel->slotCursor]].unit.id;
}

extern void cmpInitSeqInfo(cmpSeqInfo_t *pSeq, cmpSeqInfo_t *pParent);
extern void cmpModelInit(void);

campStatusPanel_t *cmpInitStatus(s8 arg, cmpSeqInfo_t *pParent)
{
    campStatusPanel_t *work;
    void *allocation;
    s32 i;

    if (cmpStatusWindowHidden == 0) {
        allocation = sdfMemAllocDirect(sizeof(*work));
        work = sdfMemLock(allocation);
        work->allocation = allocation;
    } else {
        work = cmpStatusPhase;
    }

    cmpInitSeqInfo((cmpSeqInfo_t *)work, pParent);
    work->slotCount = 0;
    work->slotCursor = 0;
    for (i = 0; i < 16; i++) {
        if (DDS3_GBWK->akuma[i].flag & 1) {
            if (i == arg) {
                work->slotCursor = work->slotCount;
            }
            work->slot[work->slotCount++] = i;
        }
    }

    if (cmpStatusWindowHidden == 0) {
        cmpSetStatusModelFlag(1);
        cmpModelInit();
    }
    cmpModelInitCore2();
    cmpClearStatusModelLoadState();
    return work;
}


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

extern campStatusPanel_t *cmpInitStatus(s8 arg, cmpSeqInfo_t *pParent);

extern s32 camp_calc_status_Process(s32 id);
extern s32 camp_draw_status_Process(s32 id);
extern s32 camp_update_status_Process(s32 id);

const u8 D_0053FEB8[8] __attribute__((section(".rodata"))) = {0};
void *cmpStartProcessStatus(s8 arg, cmpSeqInfo_t *pParent)
{
    void *proc = NULL;

    cmpStatusPhase = cmpInitStatus(arg, pParent);
    cmpSetStockVisible(0);
    if (cmpStatusWindowHidden == 0) {
        proc = dds3StartProcess("camp_calc_status", 0x3F1, 1, 1, camp_calc_status_Process, NULL,
                                cmpStatusPhase);
        dds3StartProcess("camp_draw_status", 0x2B06, 1, 1, camp_draw_status_Process, NULL,
                         cmpStatusPhase);
        dds3StartProcess("camp_update_status", 0x520A, 1, 1, camp_update_status_Process,
                         camp_update_status_Shutdown, cmpStatusPhase);
    }
    if (cmpStatusWindowHidden != 0) {
        cmpStatusWindowHidden = 2;
    }
    return proc;
}


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
    cmpDrawStatusCom(unit, 0, 0, 0);
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

void cmpUpdateStatusModelLoad(u16 id)
{
    s32 flag = cmpGetStatusModelFlag();

    switch (flag)
    {
    case 0:
        break;
    case 1:
        if (cmpLoadStatusModel() == 0)
        {
            if (cmpGetStatusModelLoadState() != 0)
            {
                cmpSetStatusModelFlag(0);
            }
            else
            {
                cmpModelSetup(id, 0);
                cmpSetStatusModelFlag(2);
            }
        }
        break;
    case 2:
        if (cmpLoadStatusModel() == 0)
        {
            cmpSetStatusModelFlag(3);
        }
        break;
    case 3:
        cmpSetStatusModelFlag(0);
        /* Dead store: blocks the sibcall this arm alone does not take. */
        flag = 0;
        break;
    }
}

extern s32 campStatusIsActive(void);

s32 camp_update_status_Process(s32 id)
{
    s32 button = cmpGetPressedButton(0x33) & 0xFFFF;
    u16 slot;

    cmpStatusPhase = dds3GetProcessData(id);
    if (campStatusIsActive() != 0) {
        return 0;
    }
    if (cmpStatusWindowHidden != 0) {
        return 0;
    }
    if ((cmpStatusPhase->flags & 0x40) != 0) {
        return -1;
    }

    switch (button) {
    case 0:
        break;
    case 0x10:
        if (cmpStatusPhase->slotCount != 1) {
            cmpStatusPhase->slotCursor--;
            if (cmpStatusPhase->slotCursor < 0) {
                cmpStatusPhase->slotCursor = cmpStatusPhase->slotCount - 1;
            }
            cmpSetModelReloadFlag();
        }
        break;
    case 0x20:
        if (cmpStatusPhase->slotCount != 1) {
            cmpStatusPhase->slotCursor++;
            if (cmpStatusPhase->slotCursor > cmpStatusPhase->slotCount - 1) {
                cmpStatusPhase->slotCursor = 0;
            }
            cmpSetModelReloadFlag();
        }
        break;
    }

    if (cmpGetStatusModelFlag() == 1) {
        if (cmpChkPadPressUpDown() != 0) {
            return 0;
        }
    }

    slot = cmpStatusPhase->slot[cmpStatusPhase->slotCursor];
    cmpUpdateStatusModelLoad(DDS3_GBWK->party[slot].unit.id);
    return 0;
}

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
    saddr_t proc;

    if (cmpInfoWindow != NULL) {
        return 0;
    }
    cmpInfoWindow = cmpAllocInfoProcess(EE_ADDR_AS(void *, text), flag);
    proc = EE_ADDR_S(dds3StartProcess("camp_calc_info", 0x3F2, 1, 1, camp_calc_info_Process, NULL,
                                 cmpInfoWindow));
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

extern void campDrawInfoText(s32 a, s32 b, s32 color, s32 d, s32 e, s32 f, s32 g);

s32 camp_draw_info_Process(s32 id)
{
    void *frq;

    cmpInfoWindow = dds3GetProcessData(id);
    if ((cmpInfoWindow->flags & 0x20) != 0)
    {
        cmpUpdateTerminate(cmpInfoWindow);
        return -1;
    }
    frq = frwGetSmallFontFRQ2(0x100, 0xA0, 0xFFFFF, 0xA09DC380, EE_ADDR_AS(char *, cmpInfoWindow->unk_18), 0);
    campDrawInfoText(0, 0x80, 0xFFFFF, *(s32 *)((u8 *)frq + 0xC) + 0x10, 0x18, 0x30303080, 0x47);
    frDrawFontOt(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
    return 0;
}

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

extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

void campInitPanelPos(void *panel, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    u8 *work = panel;

    fclInitPanelPos(b, c, 0x10, 0, 0, 0, (fclPanel_t *)(work + 4));
    *(s32 *)work = d;
    *(s32 *)(work + 0x20) = g;
    *(s32 *)(work + 0x18) = e;
    *(s32 *)(work + 0x1C) = f;
    work[0x24] = 0;
}

void cmpSetAfterCrossFunc(void *panel, s32 x, s32 y, void *func, void *data, s32 value, s32 alpha)
{
    u8 *work = panel;
    if (*(void **)(work + 0x44) != data) {
#ifdef FIX_BUGS
        /* BUG: the ranges overlap, so memcpy is undefined; retail's copies forward */
        memmove(work, work + 0x2C, 0x58);
#else
        memcpy(work, work + 0x2C, 0x58);
#endif
        work[0x24] = -1;
        fclInitPanelPos(x, y, 0x10, 0, 0, 0, (fclPanel_t *)(work + 0x30));
        *(void **)(work + 0x2C) = func;
        *(s32 *)(work + 0x4C) = alpha;
        *(void **)(work + 0x44) = data;
        *(s32 *)(work + 0x48) = value;
        work[0x50] = 0;
    }
}

typedef struct campCrossPanel_t
{
    /* 0x00 */ void (*func)(void *, void *, s32, s32, s32, s32);
    /* 0x04 */ u8 pos[0x14];
    /* 0x18 */ cmpPadStopWork_t *cursor;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 alpha;
    /* 0x24 */ s8 flag;
    /* 0x25 */ u8 unk_25[7];
} campCrossPanel_t;

#ifdef NON_MATCHING
extern void cmpClearCursorSlots(cmpCursorInfo_t *cur);
#endif

void campRunPanelCallback(campCrossPanel_t *panel)
{
    if (panel[1].func != NULL) {
        panel[1].func(panel[1].pos, panel[1].cursor, panel[1].unk_1C, panel[1].flag,
                      panel[1].alpha, 0x47);
        if (panel[1].alpha < 0x100) {
            panel[1].alpha = panel[1].alpha + 0x40;
        }
        if (panel[1].alpha >= 0x101) {
            panel[1].alpha = 0x100;
        }
    }
    if (panel[0].func != NULL) {
        panel[0].func(panel[0].pos, panel[0].cursor, panel[0].unk_1C, panel[0].flag,
                      panel[0].alpha, 0x47);
        if (panel[0].alpha < 0x200) {
            panel[0].alpha = panel[0].alpha + 0x40;
        } else {
            panel[0].func = NULL;
        }
        if (panel[0].alpha >= 0x201) {
            panel[0].alpha = 0x200;
        }
        if (panel[0].func == NULL) {
            panel[0].cursor->unk_38 = 0;
            panel[0].cursor->cursor.unk_30 = 0;
            cmpClearCursorSlots(&panel[0].cursor->cursor);
        }
    }
}


void fclInitCursorWindow(void *work, void *data, u8 count)
{
    cmpCursorInfo_t *cursor = (cmpCursorInfo_t *)((u8 *)work + 4);

    cmpInitCursorRange(cursor, count, count, EE_ADDR_S(data));
    cmpInitCursorLayout(cursor, 0, 0, 0, 0, 0x17);
}

void fclSetCursorList(void *work, s32 mode)
{
    *(s32 *)((u8 *)work + 4) = mode;
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

typedef struct {
    cmpCursorInfo_t cursor;
    u8 reserved38[4];
    cmpCursorSlot_t slot[256][2];
} cmpCursorExtended_t;

extern void cmpSetCursorSlot(s8 index, s8 state, u8 param, cmpCursorSlot_t *slot);
extern s32 cmpAllocEffectSlot(s32 color, s8 selected, s8 disabled,
                              cmpCursorSlot_t *slot);

u32 cmpDrawListItemBox(cmpCursorInfo_t *cursor, s32 item, s32 row,
                       s32 mode, s32 alpha)
{
    s32 selected = 0;
    s32 disabled = 0;
    u32 color;
    u32 blink;
    u32 effect;
    u32 product;

    if (cursor->flagsData.bytes != NULL) {
        u8 flags = cursor->flagsData.bytes[item];

        if (flags & 1) {
            mode = 1;
        }
        if (flags & 2) {
            return 0;
        }
    }
    if (row == (u8)cursor->unk_25) {
        u32 selectionMask = (u32)mode & 2U;

        selected = selectionMask == 0;
        /* Two eight-byte slots per row; retain native EE32 offset wrap. */
        cmpSetCursorSlot(0, 1, 0x10,
                         (cmpCursorSlot_t *)((u8 *)cursor + (s32)(0x3CU +
                             (((u32)(s32)cursor->unk_22 + (u32)(u8)cursor->unk_24) << 4))));
    }
    if (!(mode & 1)) {
        color = 0xA09DC380;
        if (selected) {
            color = 0x89FEFF80;
        }
    } else {
        disabled = 1;
        color = 0x4D4B4B4D;
        if (selected) {
            color = 0x8EBEF52B;
        }
    }
    blink = cmpCursorBlinkColor(alpha) & 0xFF;
    /* This channel product is retained across the effect-slot callback. */
    product = (color & 0xFFU) * blink;
    effect = cmpAllocEffectSlot(color, selected, disabled,
                                (cmpCursorSlot_t *)((u8 *)cursor + (s32)(0x3CU +
                                    (((u32)(s32)cursor->unk_22 + (u32)item) << 4))));
    effect = (effect & ~0xFF) | ((product >> 7) & 0xFF);
    return cmpCursorFadeChannel(effect, alpha, 0);
}




extern u32 cmpDrawListItemBox(cmpCursorInfo_t *cursor, s32 item, s32 row,
                              s32 mode, s32 alpha);

/* Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
void cmpDrawListItemText(s32 x, s32 y, s32 priority, u8 rowHeight,
                         cmpCursorInfo_t *cursor, s32 mode, s32 alpha, s32 ot)
{
    frFontHandle_t *font = NULL;
    s32 boxMode = 0;
    s32 row = 0;
    s32 i;

    if (mode == 2) boxMode = 2;
    cmpSetSkillListRows(0x12);
    for (i = 0; i < cursor->unk_20; i++) {
        u32 color = cmpDrawListItemBox(cursor, i, row, boxMode, alpha);

        if (color != 0) {
            font = frwGetSmallFontFRQ2(x, y, priority, color,
                                       cursor->listData.text[i], font);
            row++;
            y += rowHeight * 8;
        }
    }
    frDrawFontOt(font, 1, ot);
    frDestroyFontRequestAfter(font);
    cmpSetSkillListRows(-1);
}

#ifdef NON_MATCHING
void cmpDrawListFrame(s32 x, s32 y, s32 unusedZ, s32 rowHeight, s8 rowCount,
                      s32 phase, s32 unusedOt)
{
    s32 colors[4];
    s32 blinkColor;
    s32 phaseIsEarly;
    s32 xOffset;
    s32 drawX;
    u32 offset = 0;
    s32 i;
    s32 rowColor;
    f32 rowScale;

    blinkColor = cmpCursorBlinkColor(phase);
    phaseIsEarly = phase < 0x101;
    colors[0] = cmpCursorFadeChannel(blinkColor, phase, 0);
    colors[1] = colors[0];
    colors[2] = colors[0];
    colors[3] = colors[0];
    if (phaseIsEarly != 0) {
        xOffset = ((100 * (0x100 - phase)) >> 8) << 4;
    } else {
        xOffset = ((100 * (phase - 0x100)) >> 8) << 4;
    }
    drawX = x - xOffset;
    fclDrawSpriteRes(drawX, y, 0x10, colors, 7, 7,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(drawX, y, 0x10, colors, 7, 8,
                     D_00345D88, fclSpriteTable, 0x47);
    y += rowCount * rowHeight * 8;
    fclDrawSpriteRes(drawX, y, 0x10, colors, 7, 9,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(drawX, y, 0x10, colors, 7, 10,
                     D_00345D88, fclSpriteTable, 0x47);

    if (rowCount <= 0) {
        return;
    }
    rowScale = 1.5f;
    for (i = 0; i < rowCount; i++) {
        u32 rowPosition;
        if (phaseIsEarly != 0) {
            u32 scaled = offset * (u32)phase;
            rowPosition = (u32)((s32)((f32)scaled * rowScale) >> 8);
        } else {
            rowPosition = (u32)(i * rowHeight);
        }
        if (rowPosition > offset) {
            rowPosition = offset;
        }
        rowColor = cmpCursorFadeChannel(blinkColor, phase, i);
        colors[0] = rowColor;
        colors[1] = rowColor;
        colors[2] = rowColor;
        colors[3] = rowColor;
        fclDrawSpriteSize(x, rowPosition << 3, 0x10, 0xA1, rowHeight,
                          colors, 7, 5, D_00345D88, fclSpriteTable, 0x47);
        fclDrawSpriteSize(x, rowPosition << 3, 0x10, 0x32, rowHeight,
                          colors, 7, 6, D_00345D88, fclSpriteTable, 0x47);
        offset += rowHeight;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListFrame);
#endif

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

extern void cmpScaleColorByPhase(s8 phase, u32 *colors, s32 scale);

void cmpDrawListRowBox(s32 x, s32 y, s32 z, u16 w, u16 h2, u16 h, s32 c0, s32 c1, s32 c2,
                       s8 phase, s32 scale, s32 ot)
{
    u32 colors[4];

    colors[0] = c0;
    colors[1] = c1;
    colors[3] = c1;
    colors[2] = c0;
    cmpScaleColorByPhase(phase, colors, scale);
    cmpDrawBoxGDefault(x, y, z, w, h, (s32 *)colors, ot);
    colors[0] = c1;
    colors[1] = c2;
    colors[2] = c1;
    colors[3] = c2;
    cmpScaleColorByPhase(phase, colors, scale);
    cmpDrawBoxGDefault(x + w * 16, y, z, h2, h, (s32 *)colors, ot);
}


/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawListRows(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color,
                    s8 row, s32 scale, s8 phase, s8 layout,
                    cmpCursorInfo_t *cursor, cmpListRowDraw_t draw, s32 ot)
{
    s32 firstWidth = width / 3;
    s32 secondWidth = (s32)((u32)width << 1) / 3;
    s32 effect = 0;
    s32 step;
    s32 alpha;
    s32 baseColor;
    s32 otherColor;
    s32 blink;
    s32 firstColor;
    s32 secondColor;

    if (phase == -2) {
        effect = 2;
    } else if (phase == -1) {
        effect = 1;
    }
    memcpy(&step, &cursor->StepY, sizeof(step));
    y += step * row * 8;
    fclDrawPanelSprite2(layout == 0 ? 0x48 : 0x44, ot);
    if (effect == 0) {
        alpha = cmpCursorBlinkColor(scale) & 0xFF;
    } else {
        alpha = 0x80;
    }
    alpha = (alpha * 70 / 128) & 0xFF;
    baseColor = 0x00449B00 | alpha;
    otherColor = 0x1BBE8C00 | alpha;
    blink = cmpGetBlinkOffset(scale, (cmpPanelEff_t *)cursor);
    firstColor = fclBlendColor(baseColor, otherColor, blink);
    secondColor = fclBlendColor(baseColor, baseColor, blink);
    draw(x, y, z, secondWidth, firstWidth, height, firstColor, secondColor,
         0x1300BF00, effect, scale, ot);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListRows);
#endif

extern void cmpDrawListRows(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color,
                           s8 mode, s32 scale, s8 phase, s8 layout,
                           cmpCursorInfo_t *cursor, cmpListRowDraw_t draw, s32 ot);

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void cmpDrawListColumn(s32 x, s32 y, s32 z, s32 width, s32 height, s8 mode,
                       s32 intensity, cmpCursorInfo_t *cursor, cmpListRowDraw_t draw)
{
    volatile cmpCursorInfo_t *state = cursor;
    s32 timer = state->unk_34;
    s32 rowHeight;
    s32 alpha;
    s32 remaining;

    if (intensity != 0x100) {
        state->unk_34 = 0;
        return;
    }
    if (timer >= 300) {
        return;
    }
    alpha = (s32)((u32)timer << 7) / 300;
    rowHeight = timer / 16;
    remaining = 300 - timer;
    fclDrawPanelSprite(0x30000, 0x47);
    cmpDrawListRows(x, y - rowHeight * 8, z * 2, width, rowHeight, alpha,
                    mode, remaining, 0, 1, cursor, draw, 0x47);
    cmpDrawListRows(x, y + height * 8, z * 2, width, rowHeight, alpha,
                    mode, remaining, 0, 1, cursor, draw, 0x47);
    fclDrawPanelSprite(0x5101B, 0x47);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawListColumn);
#endif

#ifdef NON_MATCHING
void cmpDrawScrollBar(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color,
                      s32 alpha, s8 row, cmpCursorInfo_t *cursor,
                      cmpListRowDraw_t draw, s32 ot)
{
    s8 phase = 0;

    if (cursor != NULL) {
        phase = ((cmpPadStopWork_t *)cursor)->unk_38;
        if (phase < 0) {
            alpha = 0x100 - cursor->unk_30;
        }
    }
    if (draw == NULL) {
        draw = cmpDrawListRowBox;
    }
    fclDrawPanelSprite(0x7101B, ot);
    cmpDrawListRows(x, y, z, width, height, color, row, alpha,
                    phase, 0, cursor, draw, ot);
    fclDrawPanelSprite(0x5101B, ot);
    if (cursor != NULL) {
        cmpDrawListColumn(x, y, z, width, height, row, alpha, cursor, draw);
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawScrollBar);
#endif

void cmpDrawScrollThumb(s32 x, s32 y, s32 z, s32 color, s32 kind)
{
    s32 clip[4];

    clip[0] = color;
    clip[1] = color;
    clip[2] = color;
    clip[3] = color;
    switch ((s8)kind) {
    case 0:
        fclDrawSpriteRes(x, y, 0x10, clip, 7, 0, D_00345D88, fclSpriteTable, 0x47);
        break;
    case 1:
        fclDrawSpriteRes(x, y, 0x10, clip, 7, 1, D_00345D88, fclSpriteTable, 0x47);
        break;
    case 2:
        fclDrawSpriteRes(x, y, 0x10, clip, 7, 0xC, D_00345D88, fclSpriteTable, 0x47);
        break;
    case 3:
        fclDrawSpriteRes(x, y, 0x10, clip, 7, 0xD, D_00345D88, fclSpriteTable, 0x47);
        break;
    }
}


/* Exact size; 76 of 87 words match. Remaining differences schedule independent
 * argument setup and entry instructions in a different order. */
/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
extern void cmpDrawListFrame(s32 x, s32 y, s32 unusedZ, s32 rowHeight, s8 rows,
                             s32 alpha, s32 ot);
extern void cmpDrawListItemText(s32 x, s32 y, s32 color, u8 width,
                                cmpCursorInfo_t *cursor, s32 mode, s32 alpha, s32 ot);

void cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 unused,
                           s8 mode, s32 alpha, s32 ot)
{
    cmpCursorInfo_t *cursor = (cmpCursorInfo_t *)list;
    s32 x = pos->unk_00;
    s32 y = pos->unk_04;
    s8 difference = (s8)cursor->unk_20;
    difference -= cursor->unk_21;

    fclDrawPanelSprite(0x5101B, 0x47);
    cmpDrawListFrame(x - 0x280, y, 0x10, *(s32 *)cursor,
                     difference, alpha, 0x47);
    if (mode == 3) {
        return;
    }
    cmpDrawListItemText(x + 0x120, y + 0x288, 0xFFFFFF,
                        list[0], cursor, mode, alpha, 0x47);
    if (mode == 0) {
        s32 blink = cmpCursorBlinkColor(alpha);
        cmpDrawScrollBar(x, y + 0x280, 0x10, 0xA4, 0x16,
                         blink, alpha, cursor->unk_25, cursor,
                         cmpDrawListRowBox, 0x47);
    } else {
        cursor->unk_34 = 0;
    }
    cmpUpdateCursorSlots(cursor);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpUpdateListSelection);
#endif

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

void camp_23C9C0(fclPanelPos_t *pos, s32 width, s32 gap,
                  s32 first, s32 second, s32 third, s32 total,
                  s32 color1, s32 color2, s32 color3, s32 color4, s32 flag)
{
    s32 firstEnd = (((u16)pos->unk_0C * first / total) << 4) + pos->unk_00;
    s32 secondEnd = (((u16)pos->unk_0C * second / total) << 4) + firstEnd;
    s32 thirdEnd = (((u16)pos->unk_0C * third / total) << 4) + secondEnd;
    s32 x;
    s32 color;

    for (x = pos->unk_00; x < pos->unk_00 + ((u16)pos->unk_0C << 4);
         x += (width + gap) << 4) {
        if (x < firstEnd) {
            color = color1;
        } else if (x < secondEnd) {
            color = color2;
        } else if (x < thirdEnd) {
            color = color3;
        } else {
            color = color4;
        }
        cmpDrawBoxF(x, pos->unk_04, pos->unk_08, width,
                    (u16)pos->unk_0E, color, flag);
    }
}

void camp_23CAF8(s32 x, s32 y, s32 a, s32 b, s32 color, s32 g)
{
    u32 pair[2];
    s32 right = x + 0x200;

    pair[0] = color;
    pair[1] = color;
    cmpDrawBoxLine(right, a, b, y, a, b, pair, g);
    pair[0] = pair[0] & 0xFFFFFF00;
    cmpDrawBoxLine(x, a, b, right, a, b, pair, g);
}


void campDrawInfoText(s32 x, s32 y, s32 z, s32 h, s32 e, s32 color, s32 g)
{
    /* Keep the real height and draw-order arguments in call-preserved slots.
     * These local bindings and the mask binding below are compiler aids. */
    FAKEMATCH_ASM_REG(s32, savedE, "$20") = e;
    FAKEMATCH_ASM_REG(s32, savedG, "$21") = g;
    s32 corner[4];
    FAKEMATCH_ASM_REG(u32, mask, "$6");
    s32 inner = (s32)((u32)h - 0x10U);

    corner[3] = color;
    corner[1] = color;
    corner[2] = color;
    corner[0] = color;
    cmpDrawBoxGDefault(x, y, z, inner, e, corner, g);
    FAKEMATCH_ASM_LIVE_PAIR(savedE, savedG);
    /* The mask is initialized after the first call and consumed before the next. */
    mask = 0xFFFFFF00U;
    corner[1] = corner[1] & mask;
    corner[3] = corner[3] & mask;
    cmpDrawBoxGDefault((s32)((u32)x + ((u32)inner << 4)), y, z,
                       0x20, savedE, corner, savedG);
}

void camp_23CC70(s32 x, s32 y, s32 z, s32 h, s32 n, s32 f, s32 g, s32 col)
{
    cmpDrawBoxF(x, y, z, h, n, f, col);
    if (g != 0) {
        s32 right = x + h * 16;
        s32 bottom;

        cmpDrawPanelEffCell(x, y, z, right, y, z, g, col);
        bottom = y + n * 8;
        cmpDrawPanelEffCell(x, y, z, x, bottom, z, g, col);
        cmpDrawPanelEffCell(right, y, z, right, bottom, z, g, col);
        cmpDrawPanelEffCell(x, bottom, z, right, bottom, z, g, col);
    }
}


void camp_23CDB8(s32 x, s32 y, s32 z, s32 h, s32 n, s32 f, s32 g, s32 col)
{
    cmpDrawBoxF(x, y, z, h, n, f, col);
    if (g != 0) {
        s32 right = x + h * 16;

        cmpDrawPanelEffCell(x, y, z, right, y, z, g, col);
        cmpDrawPanelEffCell(x, y + n * 8, z, right, y + n * 8, z, g, col);
    }
}


extern frFontHandle_t *frGetCNameFRQ(s8 target, s8 style, frFontHandle_t *chain,
                                     s32 firstOption);
extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);
extern void frSetPriority(frFontHandle_t *font, u32 priority);
extern void frSetFontColor(frFontHandle_t *font, u32 color);

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void campDrawStatusRow1(s32 x, s32 y, u32 color, datUnitWork_t *unit)
{
    u32 flags = unit->flag;
    frFontHandle_t *font;

    if (!(flags & 1)) {
        return;
    }
    x += 0x180;
    y += 0x18;
    if (flags & 4) {
        font = frGetCNameFRQ(0, 1, NULL, 0);
        frSetDrawPosition(font, x, y);
        frSetPriority(font, 0xFFFFF0);
        frSetFontColor(font, color);
    } else {
        font = frwGetSmallFontFRQ2(x, y, 0xFFFFF, color, text2[unit->id], NULL);
    }
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStatusRow1);
#endif

void cmpDrawStatusBadge(s32 x, s32 y, s32 *colors, u16 statusFlags, s32 unused)
{
    u32 flags = statusFlags;
    s32 xOffset = 0;

    if ((flags & 0x100) != 0) {
        fclDrawSpriteRes(x, y, 0, colors, 3, 0, D_00345D88, fclSpriteTable, 0x47);
        xOffset = 0x180;
    }
    if ((flags & 0x800) != 0) {
        fclDrawSpriteRes(x + xOffset, y, 0, colors, 3, 1, D_00345D88, fclSpriteTable, 0x47);
        xOffset += 0x180;
    }
    if ((flags & 0x40) != 0) {
        fclDrawSpriteRes(x + xOffset, y, 0, colors, 3, 2, D_00345D88, fclSpriteTable, 0x47);
        xOffset += 0x180;
    }
    if ((flags & 0x20) != 0) {
        fclDrawSpriteRes(x + xOffset, y, 0, colors, 3, 3, D_00345D88, fclSpriteTable, 0x47);
    }
}

extern char cmpFmtHpMp[]; /* padded eight-byte "%3d" object */
extern char cmpFmtLevel[];
extern u16 datGetLevelForDraw(datUnitWork_t *work);

void campDrawStatusRow2(s32 inputX, s32 y, u32 color, datUnitWork_t *unit)
{
    /* Localized target constraint keeps the live horizontal origin in its
     * native saved register. It emits no instruction and is not a claim
     * about original source spelling. */
    FAKEMATCH_ASM_REG(s32, x, "$19") = inputX;
    char text[0x20];
    frFontHandle_t *font;
    s32 drawX;
    s32 drawY;
    u16 level;

    drawX = x + 0xA40;
    sprintf(text, cmpFmtHpMp, (u16)unit->hp);
    font = frwGetSmallNumFontFRQ(drawX, y, 0xFFFFF, color, text, NULL);
    drawX = x + 0xDD0;
    sprintf(text, cmpFmtHpMp, (u16)unit->mp);
    font = frwGetSmallNumFontFRQ(drawX, y, 0xFFFFF, color, text, font);
    level = datGetLevelForDraw(unit);
    drawX = x + 0x10A0;
    drawY = y + 0x28;
    sprintf(text, cmpFmtLevel, level);
    font = frwGetSmallNumFontFRQ(drawX, drawY, 0xFFFFF, color, text, font);
    frDrawFontOt(font, 1, 0x47);
    frDestroyFontRequestAfter(font);
}

extern void fclDrawSpriteRect(s32 x, s32 y, s32 d, s32 w, s32 h, s32 *color,
                              s32 id, s32 idx, s32 *texList,
                              fclSpriteTbl_t *table, s32 ot);

void campDrawStatusRow3(s32 x, s32 y, s32 color, datUnitWork_t *unit)
{
    s32 colors[4] = { color, color, color, color };
    s32 ratio;

    fclDrawSpriteRes(x, y, 0x10, colors, 0, 0x15,
                     D_00345D88, fclSpriteTable, 0x47);

    ratio = 1000 * unit->hp / unit->maxhp;
    if (ratio != 0) {
        fclDrawSpriteRect(x, y, 0x10, ratio, 1000, colors, 0, 0x14,
                          D_00345D88, fclSpriteTable, 0x47);
    }

    fclDrawSpriteRes(x, y, 0x10, colors, 0, 0x17,
                     D_00345D88, fclSpriteTable, 0x47);

    ratio = 1000 * unit->mp / unit->maxmp;
    if (ratio != 0) {
        if ((unit->badstatus & 0x800) != 0) {
            fclDrawSpriteRect(x, y, 0x10, ratio, 1000, colors, 0, 0x18,
                              D_00345D88, fclSpriteTable, 0x47);
        } else {
            fclDrawSpriteRect(x, y, 0x10, ratio, 1000, colors, 0, 0x16,
                              D_00345D88, fclSpriteTable, 0x47);
        }
    }
}

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
    return SDF_FRAME_INTERPOLATE_DUE(&campWork->stockAlpha[index + row * 4],
                                     campWork->stockAlpha[index + row * 4]);
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

extern s8 cmpGetCursorSlotState(s8 index, cmpCursorSlot_t *slot);

/* Historical retained-draft check: three PCSX2 call vectors covered only the
 * paths taken (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
void campDrawStockStatus(s32 x, s32 y, s32 z, s32 color,
                         s8 cursorMode, s8 detailMode, datUnitWork_t *unit,
                         cmpCursorSlot_t *cursor, s32 alpha)
{
    s32 drawX;
    s32 firstColor;
    s32 secondColor = color;
    s32 thirdColor = 0x80808080;
    s32 offset = 0;
    s32 colors[4];

    if (cursorMode != 0) {
        cmpSetCursorSlot(0, 1, 0x10, cursor);
    }
    firstColor = cmpAllocEffectSlot(color, 1, 0, cursor);
    switch (cmpGetCursorSlotState(0, cursor)) {
    case 3:
        firstColor = campGetCursorColorSel(firstColor, cursor);
        secondColor = campGetCursorColorSel(secondColor, cursor);
        thirdColor = campGetCursorColorSel(thirdColor, cursor);
        break;
    case 4:
        offset = campGetCursorColorAlt(cursor);
        firstColor = campGetCursorColorAlt2(firstColor, cursor);
        secondColor = campGetCursorColorAlt2(secondColor, cursor);
        thirdColor = campGetCursorColorAlt2(thirdColor, cursor);
        break;
    }
    firstColor = fclBlendColor(firstColor, 0, alpha);
    secondColor = fclBlendColor(secondColor, 0, alpha);
    thirdColor = fclBlendColor(thirdColor, 0, alpha);
    drawX = (s32)((u32)x + (u32)offset);
    campDrawStatusRow1(drawX, y, firstColor, unit);
    {
    /* Keep the native post-call mode reload in its comparison register.
     * This local constraint carries no value across a call. */
    FAKEMATCH_ASM_REG(s32, detail, "$3") = detailMode;
    FAKEMATCH_ASM_LIVE(detail);
    if (detail != 1) {
        colors[0] = thirdColor;
        colors[1] = thirdColor;
        colors[3] = thirdColor;
        cmpDrawStatusBadge(drawX, y, colors, unit->badstatus, 0x100);
        campDrawStatusRow2(drawX, y, secondColor, unit);
        campDrawStatusRow3(drawX, y, thirdColor, unit);
    }
}
}

extern void campDrawStockStatus(s32 x, s32 y, s32 z, s32 color,
                                s8 cursorMode, s8 detailMode,
                                datUnitWork_t *unit, cmpCursorSlot_t *cursor,
                                s32 alpha);

s32 campDrawStockLabel(s32 x, s32 y, s32 z, s8 slot, s8 row,
                       cmpCursorSlot_t *cursor, s8 cursorMode, s32 alpha)
{
    datUnitWork_t *unit = &DDS3_GBWK->party[DDS3_GBWK->stock[slot]].unit;
    s32 rowY = y + row * 0xC0;
    s32 color = 0xA09DC380;

    if (unit->flag & 6) {
        if (unit->badstatus & 0x800) {
            color = 0x754B3D80;
        }
        campDrawStockStatus(x + 0xD20, y + rowY + 0x120, 0, color,
                            cursorMode, 0, unit, cursor, alpha);
        return 1;
    }
    return 0;
}

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq26, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
s32 campDrawStockLabel2(s32 x, s32 y, s32 z, s8 slot, s8 localSlot,
                        cmpCursorSlot_t *cursor, s32 mode, s32 alpha)
{
    datUnitWork_t *unit = &DDS3_GBWK->party[slot].unit;
    s32 rowY = y + localSlot * 0xC0;
    s32 colors[4];
    s32 color = 0xA09DC380;
    s32 detail = 0;
    u32 flags = unit->flag;

    colors[0] = colors[1] = colors[2] = colors[3] = (alpha / 2) | 0x80808000;
    if (flags & 4) {
        return 0;
    }
    if (!(flags & 1)) {
        return 0;
    }
    if (flags & 2) {
        fclDrawSpriteRes(x, rowY, 0, colors, 0, 9, D_00345D88, fclSpriteTable, 0x47);
        flags = unit->flag;
        color = 0x4D4B4B4D;
        detail = 1;
    }
    if (flags & 0x10) {
        fclDrawSpriteRes(x, rowY, 0, colors, 0, 10, D_00345D88, fclSpriteTable, 0x47);
        color = 0x4D4B4B4D;
        detail = 1;
    }
    if (unit->badstatus & 0x800) {
        color = 0x754B3D80;
    }
    campDrawStockStatus(x + 0xD20, y + rowY + 0x478, 0, color,
                        mode, detail, unit, cursor, alpha);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStockLabel2);
#endif

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

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void campDrawStockRow1(s32 x, s32 y, s32 z, s8 *rows, s8 *columns,
                       s8 force, cmpStockWork_t *stock, s32 unused_ot)
{
    s32 colors[4];
    s32 row;
    s32 rowY = 0;
    s32 separatorY = 0x1C8;
    s32 xScale;
    s32 extraX;
    s32 color;

    for (row = 0; row < 4; row++) {
        xScale = campCalcStockScale((s8)row, 0);
        color = campDrawStockCell(0x80808080, (s8)row, 0);
        extraX = xScale + 0xD40;
        colors[0] = color;
        colors[1] = color;
        colors[2] = color;
        colors[3] = color;
        fclDrawSpriteRes(xScale, rowY, 0, colors, 0, 5, D_00345D88,
                         fclSpriteTable, 0x47);
        fclDrawSpriteRes(xScale, rowY, 0, colors, 0, 6, D_00345D88,
                         fclSpriteTable, 0x47);
        if (row < 3) {
            fclDrawSpriteRes(extraX, separatorY, 0, colors, 0, 0x12,
                             D_00345D88, fclSpriteTable, 0x47);
            fclDrawSpriteRes(extraX, separatorY, 0, colors, 0, 0x13,
                             D_00345D88, fclSpriteTable, 0x47);
        }
        rowY += 0xC0;
        separatorY += 0xC0;
    }

    {
        campWork_t *work = campWorkLive;

        for (row = 0; row < work->drawStock.PartyCnt; row++) {
            s32 cursorMode = campDrawStockIcon(1, (s8)row, rows, columns, force);
            s8 slot;
            s32 alpha;

            work = campWorkLive;
            slot = work->drawStock.ListIdx[row];
            alpha = campGetStockAlpha((s8)row, 0);
            campDrawStockLabel(x, y, z, slot, (s8)row,
                               &stock->info[0].rows[row].slot, cursorMode, alpha);
            work = campWorkLive;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStockRow1);
#endif

/* NON_MATCHING: 27 of 152 words differ at the exact 608-byte size. */
/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
extern s32 campDrawStockLabel2(s32 x, s32 y, s32 z, s8 slot, s8 localSlot,
                               cmpCursorSlot_t *cursor, s32 mode, s32 alpha);

void campDrawStockRow2(s32 x, s32 y, s32 z, s8 *rows, s8 *cols, s8 force, void *work)
{
    s32 i = 0;
    s32 rowY;
    s32 visibleRow;
    u8 *cursorBase = (u8 *)work + 0x10E8;

    if (DDS3_GBWK->maxstock > 0) {
        rowY = 0;
        do {
            s8 index = (s8)i++;
            s32 scale = campCalcStockScale(index, 1);
            s32 color = campDrawStockCell(0x80808080, index, 1);
            s32 colors[4];

            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            fclDrawSpriteRes(scale, rowY, 0, colors, 0, 7,
                             D_00345D88, fclSpriteTable, 0x47);
            fclDrawSpriteRes(scale, rowY, 0, colors, 0, 8,
                             D_00345D88, fclSpriteTable, 0x47);
            rowY += 0xC0;
        } while (i < DDS3_GBWK->maxstock);
    }

    visibleRow = 0;
    i = campWork->drawStock.PartyCnt;
    while (i < (s32)campWork->drawStock.PartyCnt + campWork->drawStock.StockCnt) {
        u8 partyCount = (u8)campWork->drawStock.PartyCnt;
        s32 iconResult = campDrawStockIcon(0, (s8)visibleRow, rows, cols, force);
        s8 localSlot = (s8)(i - partyCount);
        s8 slot = campWork->drawStock.StockIdx[i];
        u32 flag;
        s32 mode;
        s32 alpha;
        cmpCursorSlot_t *cursor;

        i++;
        flag = DDS3_GBWK->party[slot].unit.flag;
        mode = 0;
        if ((flag & 0x10) == 0) {
            mode = iconResult;
        }
        if ((flag & 0x2) != 0) {
            mode = 0;
        }
        alpha = campGetStockAlpha(localSlot, 1);
        cursor = (cmpCursorSlot_t *)(cursorBase + 0x3C + 0x10 * visibleRow);
        visibleRow = (s8)(visibleRow + campDrawStockLabel2(x, y, z, slot, localSlot,
                                                           cursor, mode, alpha));
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStockRow2);
#endif

s32 cmpFindFreeStockSlot(s8 index, s8 room)
{
    s8 slot = 0;

    if (index < 0) {
        return -1;
    }
    if (room != 0) {
        while (campWork->drawStock.ListIdx[slot] != index) {
            slot++;
        }
    } else {
        while (campWork->drawStock.ListIdx[slot] != index) {
            slot++;
        }
        slot -= campWork->drawStock.PartyCnt;
        if (slot < 0) {
            slot = 0;
        }
    }
    return slot;
}

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

#ifdef NON_MATCHING
void campDrawStockHighlight(s32 x, s32 y, s32 unused0, s32 unused1, s32 unused2,
                            s32 alpha, s8 status, s32 unused3, s32 ot)
{
    u32 color0 = 0x00647846;
    u32 color1 = 0x00449B46;
    u32 color2 = 0x1300BF00;
    u32 color3 = 0x1BBE8C46;
    u32 colors[4];
    s8 phase = 0;

    switch (status) {
    case -1: phase = 1; break;
    case -2: phase = 2; break;
    case 0: phase = 3; break;
    case -3:
        phase = 4;
        color0 = fclBlendColor(color0, color1, alpha);
        color3 = fclBlendColor(color3, color1, alpha);
        break;
    }
    fclDrawPanelSprite2(0x48, 0x47);
    colors[0] = color2;
    colors[1] = color1;
    colors[2] = color2;
    colors[3] = color1;
    cmpScaleColorByPhase(phase, colors, alpha);
    cmpDrawBoxGDefault(x, y, 0, 0x6D, 0x15, (s32 *)colors, ot);
    colors[0] = color1;
    colors[1] = color0;
    colors[2] = color1;
    colors[3] = color0;
    cmpScaleColorByPhase(phase, colors, alpha);
    cmpDrawBoxGDefault(x + 0x6D0, y, 0, 0x69, 0x15, (s32 *)colors, ot);
    colors[0] = color0;
    colors[1] = color3;
    colors[2] = color0;
    colors[3] = color3;
    cmpScaleColorByPhase(phase, colors, alpha);
    cmpDrawBoxGDefault(x + 0xD60, y, 0, 0xBD, 0x15, (s32 *)colors, ot);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStockHighlight);
#endif

extern void campDrawStockHighlight(s32 x, s32 y, s32 c, s32 d, s32 e, s32 f, s8 g, s32 h,
                                   s32 ot);

s32 campCheckStockFree(s32 x, s32 y, s8 index, s8 room, s8 known, s32 f, s8 g)
{
    s32 slot;
    s32 off;

    if (index < 0) {
        return 0;
    }
    if (known == 0) {
        slot = cmpFindFreeStockSlot(index, room);
    } else {
        slot = index;
    }
    if (room != 0) {
        off = slot * 24 + 0x24;
    } else {
        off = slot * 24 + 0x8F;
    }
    campDrawStockHighlight((x + 0xCE) << 4, (y + off) * 8, 0, 0, 0, f, g, 0, 0x47);
    return off * 8;
}


/* NON_MATCHING: 14 of 39 words differ at the exact size: retail keeps a second
 * register holding work + ofs that the C coalesces away. */
/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
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

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void campDrawStockSlots(s8 *indices, s8 active, s8 *flags,
                        cmpLocalStock_t *stock)
{
    s8 activeMode = active;
    s8 rows[2];
    s8 cols[2];
    s32 colors[4];
    s32 color;
    s8 i = 0;

    color = cmpCursorBlinkColor(0x100);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;

    do {
        s8 index = indices[i + 1];

        if (index >= 0) {
            s8 unitIndex = DDS3_GBWK->stock[index];
            datUnitWork_t *unit = &DDS3_GBWK->unitwork[unitIndex];

            if (unit->badstatus & 0x800) {
                indices[i + 1] = -1;
            }
            if ((unit->flag & 1) == 0) {
                indices[i + 1] = -1;
            }
        }

        index = indices[i + 1];
        if (index < 0) {
            rows[i] = 0;
            cols[i] = -1;
        } else if (activeMode == 1) {
            s32 room = cmpStockHasRoom((cmpStockWork_t *)stock, flags[i + 1]);

            rows[i] = room;
            cols[i] = cmpFindFreeStockSlot(indices[i + 1], (s8)room);
        } else {
            rows[i] = 0;
            cols[i] = -1;
        }
        i = (s8)(i + 1);
    } while (i < 2);

    campDrawStockRow1(0, 0, 0, rows, cols,
                      (activeMode == 0 || activeMode == 4), (cmpStockWork_t *)stock, 0x47);
    campDrawStockRow2(0, 0, 0, rows, cols, activeMode == 0, stock);

    fclDrawSpriteRes(0, 0, 0, colors, 0, 1,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 2,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 3,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0, 0, 0, colors, 0, 4,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0xD40, 0xF0, 0, colors, 0, 0xE,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0xD40, 0xF0, 0, colors, 0, 0xF,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0xD40, 0x408, 0, colors, 0, 0x10,
                     D_00345D88, fclSpriteTable, 0x47);
    fclDrawSpriteRes(0xD40, 0x408, 0, colors, 0, 0x11,
                     D_00345D88, fclSpriteTable, 0x47);

    if (activeMode == 1) {
        s8 room;
        u8 flag;
        s32 result = campCheckStockRoom(flags[0], indices[0], &room, &flag,
                                         (cmpStockWork_t *)stock, 0);

        campCheckStockFree(0, 0, indices[0], room, 0, 0x100 - result, (s8)flag);
    } else {
        s8 slot = 0;

        do {
            s8 room;
            u8 flag;
            s32 result = campCheckStockRoom(flags[0], slot, &room, &flag,
                                             (cmpStockWork_t *)stock, 1);

            campCheckStockFree(0, 0, slot, 1, 1, 0x100 - result, (s8)flag);
            slot = (s8)(slot + 1);
        } while (slot < 4);
    }

    if (activeMode == 0) {
        s8 partyCount = campWork->drawStock.PartyCnt;
        s8 stockCount = campWork->drawStock.StockCnt;
        s8 index = partyCount;

        if ((s32)index < (s32)partyCount + stockCount) {
            do {
                s8 currentPartyCount = (u8)campWork->drawStock.PartyCnt;
                s8 stockIndex = campWork->drawStock.StockIdx[index];
                datUnitWork_t *unit = &DDS3_GBWK->unitwork[stockIndex];
                s8 localIndex = (s8)(index - currentPartyCount);
                u32 unitFlags = unit->flag;

                if ((unitFlags & 0x10) == 0 && (unitFlags & 2) == 0) {
                    s8 room;
                    u8 flag;
                    s32 result = campCheckStockRoom(flags[0], localIndex,
                                                     &room, &flag,
                                                     (cmpStockWork_t *)stock, 1);

                    campCheckStockFree(0, 0, localIndex, 0, 1,
                                       0x100 - result, (s8)flag);
                }
                index = (s8)(index + 1);
                partyCount = campWork->drawStock.PartyCnt;
                stockCount = campWork->drawStock.StockCnt;
            } while ((s32)index < (s32)partyCount + stockCount);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", campDrawStockSlots);
#endif

typedef struct
{
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 start;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 width;
    /* 0x1C */ u8 reflected;
    /* 0x20 */ s32 total;
    /* 0x24 */ s32 left;
    /* 0x28 */ s32 unk_28;
} cmpPanelEffAnim_t;

void cmpRandomizePanelEff(s32 width, s32 height, s32 kind, cmpPanelEffAnim_t *anim)
{
    u32 color;
    s32 i;
    cmpPanelEffAnim_t *entry;

    switch (kind) {
    case 1: color = 0xFF51D980; break;
    case 2: color = 0x7AE7FF80; break;
    case 5: color = 0xFF7F5180; break;
    case 6: color = 0x5551FF80; break;
    default: color = 0xC2FF5180; break;
    }
    entry = anim;
    for (i = 0; i < 16; i++) {
        entry->width = ((dds3GetRandA(NULL) & 7) + 8) << 4;
        entry->reflected = dds3GetRandA(NULL) & 1;
        entry->pos = (dds3GetRandA(NULL) % (width / 6)) << 4;
        entry->unk_04 = (dds3GetRandA(NULL) % height) << 3;
        entry->start = entry->pos;
        entry->unk_0C = entry->unk_04;
        entry->target = entry->start + ((dds3GetRandA(NULL) % (width / 3 - (entry->width >> 4))) << 4);
        entry->unk_14 = entry->unk_0C;
        entry->total = (dds3GetRandA(NULL) & 7) + 1;
        entry->left = entry->total;
        entry->unk_28 = color;
        if (entry->reflected != 0) {
            entry->pos = -entry->pos;
            entry->start = -entry->start;
            entry->target = -entry->target;
            entry->width = -entry->width;
        }
        entry++;
    }
}



typedef struct
{
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 step;
    /* 0x02 */ u8 delay;
    /* 0x03 */ u8 kind;
    /* 0x04 */ u32 color4;
    /* 0x08 */ u32 color8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u16 width;
    /* 0x1A */ u16 height;
    /* 0x1C */ s32 frame;
    /* 0x20 */ s32 value;
    /* 0x24 */ cmpPanelEffAnim_t anim[16];
} cmpPanelEffState_t;

void cmpDrawPanelEffBar(cmpPanelEffState_t *state, s32 ot)
{
    cmpPanelEffAnim_t *anim = state->anim;
    s32 x = state->x + (state->width >> 1) * 16;
    s32 z = state->z;
    s32 y = state->y;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        s32 level = (anim->left << 7) / anim->total;

        cmpDrawPanelEffCell(x + anim->pos, y + anim->unk_04, z, x + anim->pos + anim->width,
                            y + anim->unk_04, z, (anim->unk_28 & 0xFFFFFF00) | (level & 0xFF),
                            ot);
        anim++;
    }
}

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
    /* 0x0009 */ u8 step;
    /* 0x000A */ u8 delay;
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

extern cmpPanelEff_t *volatile cmpPanelEffLive FAKEMATCH_ASM_NAME("cmpPanelEffList");
extern void cmpRandomizePanelEff(s32 width, s32 height, s32 kind,
                                cmpPanelEffAnim_t *anim);

#ifdef NON_MATCHING
void cmpAddPanelEffEntry(s32 index, s32 x, s32 y, s32 z, u16 width, u16 height,
                         u8 step, u8 delay, s32 color4, s32 color8, u8 kind)
{
    volatile cmpPanelEffState_t *entry = (cmpPanelEffState_t *)&cmpPanelEffList[index].state;
    volatile cmpPanelEffState_t *live;

    entry->frame = 0;
    entry->height = height;
    entry->x = x;
    entry->width = width;
    entry->y = y;
    entry->z = z;
    entry->color4 = color4;
    entry->color8 = color8;
    entry->step = step;
    live = (cmpPanelEffState_t *)&cmpPanelEffLive[index].state;
    live->delay = delay;
    live = (cmpPanelEffState_t *)&cmpPanelEffLive[index].state;
    live->kind = kind;
    live = (cmpPanelEffState_t *)&cmpPanelEffLive[index].state;
    live->state = 1;
    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 5:
    case 6:
        live = (cmpPanelEffState_t *)&cmpPanelEffLive[index].state;
        cmpRandomizePanelEff(width, height, kind,
                             (cmpPanelEffAnim_t *)live->anim);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpAddPanelEffEntry);
#endif

extern void cmpAddPanelEffEntry(s32 index, s32 x, s32 y, s32 z, u16 width, u16 height,
                                u8 step, u8 delay, s32 color4, s32 color8, u8 kind);

s32 cmpAddQuePanelEff(s32 a1, s32 a2, s32 a3, u16 a4, u16 a5, u8 a6, u8 a7, s32 a8, s32 a9, u8 a10)
{
    cmpPanelEff_t *eff;
    s32 slot = -1;
    s32 i;

    if (cmpPanelEffList == NULL)
        return -1;
    eff = (cmpPanelEff_t *)((u8 *)cmpPanelEffList + 0x18);
    for (i = 0; i < 32; i++)
    {
        if (eff->flags == 0)
        {
            slot = i;
        }
        eff++;
    }
    if (slot >= 0)
    {
        cmpAddPanelEffEntry(slot, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        return slot;
    }
    return -1;
}

#ifdef NON_MATCHING
void cmpDrawPanelEffBox(void *entry, s32 ot)
{
    cmpPanelEffState_t *state = entry;
    volatile cmpPanelEffState_t *live = state;
    s32 frame = live->frame;
    s32 halfWidth = live->width >> 1;
    u32 colorA = live->color8;
    u32 colorB = live->color4;
    s32 centerOffset = (halfWidth - 8) * 16;
    f32 radius;
    s32 colors[4];
    s32 i;

    if (frame > 0x80) {
        colorA = fclBlendColor(0x80, colorA, frame * 2 - 0x100);
        colorB = fclBlendColor(0x80, colorB, frame * 2 - 0x100);
    }
    radius = fsqrtf((f32)halfWidth * 1.5f);
    for (i = 0; i >= 0; i--) {
        s32 extent = (s32)(radius * (f32)frame * 0.00390625f);
        s32 width;
        s32 x;
        s32 y;
        s32 z;
        u16 height;

        fclDrawPanelSprite2(0x48, ot);
        colorA = fclBlendColor(colorA, colorB, frame);
        width = extent * extent + 8;
        y = live->y;
        x = live->x;
        z = live->z;
        height = live->height;
        colors[0] = 0x80;
        colors[1] = colorA;
        colors[2] = 0x80;
        colors[3] = colorA;
        cmpDrawBoxGDefault(x + (halfWidth - width - 8) * 16, y, z,
                           width, height, colors, ot);
        x = live->x;
        y = live->y;
        z = live->z;
        height = live->height;
        colors[0] = colorA;
        colors[1] = 0x80;
        colors[2] = colorA;
        colors[3] = 0x80;
        cmpDrawBoxGDefault(x + (halfWidth + 8) * 16, y, z, width, height, colors, ot);
        x = live->x;
        z = live->z;
        height = live->height;
        y = live->y;
        cmpDrawBoxF(x + centerOffset, y, z, 16, height, colorA, ot);
        cmpDrawPanelEffBar(state, ot);
        fclDrawPanelSprite2(0x44, ot);
        frame /= 2;
    }
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffBox);
#endif

void cmpDrawPanelEffRing(cmpPanelEffState_t *state, s32 ot)
{
    s32 frame = state->frame;
    s32 frameOffset = frame * 2 - 0x100;
    s32 neutralColor = 0x80;
    u32 colorA = state->color8;
    u32 colorB = state->color4;
    s32 spriteIndex;
    s32 square;
    s32 yOffset;
    s32 colors[4];

    if (frame >= 0x81) {
        colorA = fclBlendColor(neutralColor, colorA, frameOffset);
        colorB = fclBlendColor(neutralColor, colorB, frameOffset);
    }

    spriteIndex = (s32)(fsqrtf(50.0f) * (f32)frame * (1.0f / 256.0f));
    fclDrawPanelSprite2(0x48, ot);
    /* Keep the evolving color live across both blends; see
       docs/levers/cross-block-locals.md. */
    colorA = fclBlendColor(colorA, colorB, frame);

    square = spriteIndex * spriteIndex;
    yOffset = square * 8;

    colors[0] = neutralColor;
    colors[1] = neutralColor;
    colors[2] = neutralColor;
    colors[3] = (s32)colorA;
    cmpDrawBoxGDefault(state->x, state->y - yOffset, state->z, 0x14,
                       state->height + square, colors, ot);

    colors[0] = neutralColor;
    colors[1] = neutralColor;
    colors[2] = colorA;
    colors[3] = (s32)colorA;
    cmpDrawBoxGDefault(state->x + 0x140, state->y - yOffset, state->z,
                       state->width - 0x14, state->height + square, colors, ot);
    fclDrawPanelSprite2(0x44, ot);
}

#ifdef NON_MATCHING
/* NON_MATCHING: 128 of 131 words match at the native 524-byte size. */
extern const s32 cmpPanelEffCoords[];

void cmpDrawPanelEffQuad(cmpPanelEffState_t *state, s32 ot)
{
    s32 frame = state->frame;
    s32 offset = (frame << 8) - 0x2000;
    s32 neutralColor = 0x80;
    u32 colorA;
    u32 colorB;
    u32 coords[12];
    u32 corners[3];
    s32 radius;

    memcpy(coords, cmpPanelEffCoords, sizeof(coords));
    colorA = state->color8;
    colorB = state->color4;
    if (frame >= 0x21) {
        colorA = fclBlendColor(neutralColor, colorA,
                               offset / 0xE0);
        colorB = fclBlendColor(neutralColor, colorB,
                               offset / 0xE0);
    }

    /* One geometry scalar retains the native conversion/square allocation.
     * Qualified control and remaining stores: docs/matching/code_0022C578.md. */
    radius = (s32)(fsqrtf(3200.0f) * (f32)(0x100 - frame) * 0.00390625f);
    radius = 0xC80 - radius * radius;
    fclDrawPanelSprite2(0x48, ot);
    colorA = fclBlendColor(colorA, colorB, frame);

    coords[1] -= radius * 2;
    coords[5] -= radius;
    corners[0] = colorA;
    corners[1] = neutralColor;
    corners[2] = neutralColor;
    cmpDrawTriG(coords, coords + 3, state->z, corners, ot);

    coords[7] += radius * 2;
    coords[11] += radius;
    cmpDrawTriG(coords + 6, coords + 9, state->z, corners, ot);
    fclDrawPanelSprite2(0x44, ot);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpDrawPanelEffQuad);
#endif

extern void cmpDrawPanelEffBox(void *entry, s32 arg);
extern void cmpDrawPanelEffRing(cmpPanelEffState_t *entry, s32 arg);
extern void cmpDrawPanelEffQuad(cmpPanelEffState_t *entry, s32 arg);

const s32 cmpPanelEffCoords[12] = {8192, 3200, 8192, 3584, 3584, 2496, 0, 4992, 0, 0, 0, 1088};
void cmpFindPanelEffSlot(s32 arg)
{
    s32 i;

    if (cmpPanelEffList == NULL) {
        return;
    }
    for (i = 0; i < 0x20; i++) {
        s32 off = i * 0x2E4 + 0x10;
        cmpPanelEffView_t *eff = (cmpPanelEffView_t *)((u8 *)cmpPanelEffList + off);

        if (eff->state == 0) {
            continue;
        }
        if (eff->delay != 0) {
            continue;
        }
        switch (eff->kind) {
        case 0:
        case 1:
        case 2:
        case 5:
        case 6:
            cmpDrawPanelEffBox(EE_ADDR_AS(void *, (i * 0x2E4 + EE_ADDR_S(cmpPanelEffList) + 0x18)), arg);
            break;
        case 3:
            cmpDrawPanelEffRing(EE_ADDR_AS(void *, (i * 0x2E4 + EE_ADDR_S(cmpPanelEffList) + 0x18)), arg);
            break;
        case 4:
            cmpDrawPanelEffQuad(EE_ADDR_AS(void *, (i * 0x2E4 + EE_ADDR_S(cmpPanelEffList) + 0x18)), arg);
            break;
        }
    }
}


void cmpUpdatePanelEffSlots(void)
{
    s32 i;

    if (cmpPanelEffList == NULL) {
        return;
    }
    for (i = 0; i < 0x20; i++) {
        s32 off = i * 0x2E4 + 0x10;
        cmpPanelEffView_t *eff = (cmpPanelEffView_t *)((u8 *)cmpPanelEffList + off);
        s32 *alpha;
        s32 value;

        if (eff->state == 0) {
            continue;
        }
        if (eff->delay != 0) {
            eff->delay = eff->delay - 1;
            continue;
        }
        alpha = EE_ADDR_AS(s32 *, (i * 0x2E4 + EE_ADDR_S(cmpPanelEffList) + 0x34));
        value = *alpha + eff->step;
        if (value >= 0x101) {
            eff->state = 0;
        } else {
            *alpha = value;
            switch (eff->kind) {
            case 3:
                break;
            case 4:
                break;
            case 0:
            case 1:
            case 2:
            case 5:
            case 6:
                cmpUpdatePanelEffEntry(
                    (cmpPanelEffState_t *)((u8 *)&cmpPanelEffList[i] + 0x18));
                break;
            }
        }
    }
}


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

cmpPanelEff_t *cmpAllocPanelEffWork(s32 value)
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

void camp_update_paneleff_Shutdown(void)
{
    if (cmpPanelEffList != NULL) {
        sdfMemFreeDirect(cmpPanelEffList->allocation);
        cmpPanelEffList = NULL;
    }
}

extern cmpPanelEff_t *cmpAllocPanelEffWork(s32 value);

void *cmpStartProcessPanelEff(s32 parent)
{
    cmpPanelEff_t *work = cmpAllocPanelEffWork(parent);

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

mdlObj_t *cmpGetEncycWork(void)
{
    return cmpEncycWork;
}

u16 cmpGetStatusModelId(void)
{
    return cmpStatusModelId;
}

extern s32 mdlGetNumOfCut(mdlObj_t *work, s32 index);

#ifdef NON_MATCHING
extern void cmpGetStatusModelMotion(mdlObj_t *model);
#endif

void cmp_23F8F0(u16 id, s32 delta)
{
    if (delta < 0) {
        if (cmpModelParamTable[id].motionId == 0) {
            return;
        }
    } else {
        if (cmpModelParamTable[id].motionId >= mdlGetNumOfCut(cmpEncycWork, 0) - 1) {
            return;
        }
    }
    cmpModelParamTable[id].motionId = cmpModelParamTable[id].motionId + delta;
    cmpGetStatusModelMotion(cmpEncycWork);
}


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

extern void func_0023FCC0(void);

void cmpModelInitCore2(void)
{
    dds3ScreenParam[4] = 2188.0f;
    dds3ScreenParam[5] = 2048.0f;
    func_0023FCC0();
}

extern const f32 cmpDefaultLightColor[];
extern const u32 cmpDefaultLightDir[];
extern const u32 cmpDefaultAmbientColor[];
extern const u32 cmpDefaultFogColor[];
extern f32 D_00345F60[];
extern void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias);
extern void dds3SetLightColor(s32 frame, s32 number, f32 *color);
extern void dds3SetLightDir(s32 frame, s32 number, f32 *direction);
extern void dds3SetAmbientColor(s32 frame, f32 *color);
extern void dds3SetFogColor(s32 frame, f32 *color);
extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 nearPlane, f32 farPlane);
extern void sdf3DSetCamera(sdf3DScreenParam_t *screenParam, void *camera);

void func_0023FCC0(void)
{
    sdfVector4_t lightColor;
    sdfVector4_t lightDirection;
    sdfVector4_t ambientColor;
    sdfVector4_t fogColor;

    memcpy(&lightColor, cmpDefaultLightColor, sizeof(lightColor));
    memcpy(&lightDirection, cmpDefaultLightDir, sizeof(lightDirection));
    memcpy(&ambientColor, cmpDefaultAmbientColor, sizeof(ambientColor));
    memcpy(&fogColor, cmpDefaultFogColor, sizeof(fogColor));
    dds3SetSoftenBias(0, 5.0f, 0.0f);
    dds3SetLightColor(0, 0, &lightColor.x);
    dds3SetLightDir(0, 0, &lightDirection.x);
    dds3SetAmbientColor(0, &ambientColor.x);
    dds3SetFogColor(0, &fogColor.x);
    dds3SetFogPara(0, 255.0f, 255.0f, 2000.0f, 30000.0f);
    dds3ScreenParam[3] = 0.7551904917f;
    SDF_VU0_COPYVECTOR(aViewEye, &cmpStatusModelBasePos);
    SDF_VU0_COPYVECTOR(aViewTarget, &cmpStatusModelPos);
    SDF_VU0_COPYVECTOR(aViewDown, D_00345F60);
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    sdf3DSetCamera((sdf3DScreenParam_t *)dds3ScreenParam, &sdfViewMatrix);
}

void cmpModelInit(void)
{
    cmpModelInitFlag = 0;
    cmpStatusGroup = 6;
    cmpStatusModelId = 0;
    cmpModelHandle = 0;
    cmpModelInitCore2();
}

void cmpResetDrawEnv(void)
{
    dds3ScreenParam[4] = 2048.0f;
    dds3ScreenParam[5] = 2048.0f;
}

extern void mdlFree(mdlObj_t *aHandle);

extern void cmpFreeBadEffect(void);

extern const char cmpModelFreeMsg[];

void cmpModelFreeResource(void)
{
    if (cmpEncycWork != 0)
    {
        printf(cmpModelFreeMsg, cmpStatusGroup, cmpStatusModelId);
        mdlFree(cmpEncycWork);
        cmpEncycWork = 0;
        cmpFreeBadEffect();
    }
}

s32 cmpPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    /* mdlPrepare's result, which retail returns in v0 (cmpLoadStatusModel
     * reads it); the store after the call keeps it no sibling call. */
    return cmpStatusModelResult = mdlPrepare(group, id, blocking);
}

typedef struct
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 scl;
    void *hModel;
} cmpBadEffectParams_t;

extern datUnitVisual_t datModelParamTable[];

void cmpCreateBadEffect(u16 id, void *model, u8 *buffer)
{
    sdfVector4_t pos __attribute__((aligned(16)));
    sdfVector4_t rot __attribute__((aligned(16)));
    sdfVector4_t center __attribute__((aligned(16)));
    cmpBadEffectParams_t *params = (cmpBadEffectParams_t *)buffer;

    memset(&pos, 0, sizeof(pos));
    memset(&rot, 0, sizeof(rot));
    memset(&center, 0, sizeof(center));
    SDF_VU0_COPYVECTOR(&params->pos, &pos);
    SDF_VU0_COPYVECTOR(&params->rot, &rot);
    SDF_VU0_COPYVECTOR(&params->center, &center);
    params->hModel = model;
    params->r = datModelParamTable[id].formsize;
    params->h = datModelParamTable[id].formheight;
    params->scl = 1.0f;
}

extern void *hCAMPBadEffect;

extern u8 D_00349300[];

extern const char cmpBadEffReadyMsg[];

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
    printf(cmpBadEffReadyMsg);
    return 1;
}

extern void *hCAMPBadEffect;

extern void effCAMPBadDelete(void *hCAMPBadEffect);

extern const char cmpBadEffFreeMsg[];

void cmpFreeBadEffect(void)
{
    if (hCAMPBadEffect != NULL)
    {
        effCAMPBadDelete(hCAMPBadEffect);
        hCAMPBadEffect = NULL;
        printf(cmpBadEffFreeMsg);
    }
}

extern const char cmpModelPrepareMsg[];

void rstPrepareStatusModel(s32 group, s32 id, s32 blocking)
{
    cmpPrepareStatusModel(group, id, blocking);
    printf(cmpModelPrepareMsg, group, id);
}

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
    if ((group == cmpStatusGroup) && (model == cmpStatusModelId) && (cmpEncycWork != 0))
    {
        return 2;
    }
    cmpModelFreeResource();
    cmpStatusGroup = group;
    cmpStatusModelId = model;
    cmpModelHandle = blocking;
    cmpPrepareStatusModel(cmpStatusGroup, cmpStatusModelId, cmpModelHandle);
    cmpModelInitFlag = 1;
    if (blocking == 1)
    {
        cmpLoadStatusModel();
    }
    return 1;
}

extern const char cmpModelReadyMsg[];

extern mdlObj_t *mdlLoad(u32 group, u32 id);

extern void cmpGetStatusModelMotion();

s32 cmpLoadStatusModel(void)
{
    if (cmpModelInitFlag != 0)
    {
        if (cmpPrepareStatusModel(cmpStatusGroup, cmpStatusModelId, cmpModelHandle) != 0)
        {
            if (cmpPrepareStatusModel(cmpStatusGroup, cmpStatusModelId, cmpModelHandle) != -1)
            {
                cmpModelFreeResource();
                cmpEncycWork = mdlLoad(cmpStatusGroup, cmpStatusModelId);
                cmpGetStatusModelMotion(cmpEncycWork);
                printf(cmpModelReadyMsg, cmpStatusGroup, cmpStatusModelId);
                cmpModelInitFlag = 0;
                return 2;
            }
        }
        return 1;
    }
    return 0;
}

#include "motion.h"

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

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

void cmpSetStatusModelPose(mdlObj_t *model)
{
    sdfVector4_t pos;
    f32 scale;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    pos.x = cmpModelParamTable[cmpStatusModelId].evo[0];
    pos.y = cmpModelParamTable[cmpStatusModelId].evo[1];
    if (cmpStatusModelReady == 0) {
        cmpGetStatusModelScale(model, 0);
        pos.z = cmpModelParamTable[cmpStatusModelId].evo[2];
        cmpResetStatusModelPos();
    } else {
        scale = cmpGetStatusModelScale(model, 1);
        pos.z = 0.0f;
        pos.x -= cmpModelParamTable[cmpStatusModelId].evo[0] - cmpModelParamTable[cmpStatusModelId].evo[0] * scale;
        pos.y -= cmpModelParamTable[cmpStatusModelId].evo[1] - cmpModelParamTable[cmpStatusModelId].evo[1] * scale;
        cmpStatusModelBasePos.z = (-400.0f - cmpModelParamTable[cmpStatusModelId].evo[2]) * scale;
    }
    SDF_VU0_LOADVECTOR_MEMOPS(pos);
    mdlSetPosition(model);
}

void cmpSetModelRotate(void *model)
{
    FromEuler(cmpModelParamTable[cmpStatusModelId].pos[0] * 3.1415926f / 180.0f,
              cmpModelParamTable[cmpStatusModelId].pos[1] * 3.1415926f / 180.0f,
              cmpModelParamTable[cmpStatusModelId].pos[2] * 3.1415926f / 180.0f);
    mdlSetRotate(model);
}

extern f32 dds3QuakeOffset[];

void cmpSetStatusModelParam(void)
{
    sdfVector4_t target;
    sdfVector4_t eye;
    u32 frame = dds3GetFrameID();

    SDF_VU0_COPYVECTOR(aViewEye, &cmpStatusModelBasePos);
    SDF_VU0_COPYVECTOR(aViewTarget, &cmpStatusModelPos);
    SDF_VU0_COPYVECTOR(aViewDown, D_00345F60);
    sdf3DMakeScreenParam((sdf3DScreenParam_t *)dds3ScreenParam);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    /* The quake moves once per retail tick. */
    SDF_VU0_LOADVECTOR_B(SDF_FRAME_INTERPOLATE_PTR(dds3QuakeOffset, 3));
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&target);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&eye);
    LookAtMatrix((f32 *)&target, (f32 *)&eye, aViewDown);
    SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
    sdf3DMakeViewportPacket((sdf3DViewportPacket_t *)((u8 *)&dds3DrawEnv[frame] + 0x1520),
                          (sdf3DScreenParam_t *)dds3ScreenParam, &sdfViewMatrix);
    sdf3DSetCamera((sdf3DScreenParam_t *)dds3ScreenParam, &sdfViewMatrix);
}

extern void cmpSetStatusModelPose(mdlObj_t *model);

extern void cmpSetModelRotate(void *model);

extern void cmpSetStatusModelParam(void);

extern void effCAMPBadDraw(void *hCAMPBadEffect);

s32 cmpDrawStatusModel(void *scene)
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

extern dds3GlobalWork_t *volatile cmpRebuildGlobalWork FAKEMATCH_ASM_NAME("DDS3_GBWK");

#ifdef NON_MATCHING
void cmpRebuildStockList(void)
{
    dds3GlobalWork_t *work = cmpRebuildGlobalWork;
    s8 removed = 0;
    s32 slot = 4;
    s32 i;

    if (removed == work->stockCount) {
        return;
    }
    do {
        if ((work->party[slot].unit.flag & 1) == 0) {
            for (i = slot; i < 15; i++) {
                work->party[i] = work->party[i + 1];
                work = cmpRebuildGlobalWork;
            }
            memset(&work->party[15], 0, sizeof(work->party[15]));
            work = cmpRebuildGlobalWork;
            for (i = 0; i < ((volatile dds3GlobalWork_t *)work)->stockCount; i++) {
                if ((u8)work->stock[i] >= slot) {
                    work->stock[i]--;
                    work = cmpRebuildGlobalWork;
                }
            }
            removed++;
        }
        slot++;
    } while (slot < 16 && removed != ((volatile dds3GlobalWork_t *)work)->stockCount);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpRebuildStockList);
#endif

void cmpRemoveStockSlot(s8 index)
{
    s32 i;

    DDS3_GBWK->stock[index] = 0;
    for (i = index + 1; i < DDS3_GBWK->stockCount; i++)
    {
        DDS3_GBWK->stock[i - 1] = DDS3_GBWK->stock[i];
    }
    DDS3_GBWK->stockCount--;
}

void cmpCompactStockList(s8 devilId)
{
    DDS3_GBWK->stock[DDS3_GBWK->stockCount++] = devilId;
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
    return cmpParamGaugeColors[index];
}

extern const char cmpDebugIdFmt[];

extern s32 datExecSkill(s32 skill, datUnitWork_t *user, datUnitWork_t *target);

void cmpExecRecover(u16 skill, datUnitWork_t *user, datUnitWork_t *target)
{
    datUnitWork_t saveUser;
    datUnitWork_t saveTarget;

    cmbCopyCustomDevilToStock(&saveUser, user);
    cmbCopyCustomDevilToStock(&saveTarget, target);
    datExecSkill(skill, user, target);
    printf(cmpDebugIdFmt, skill, target->flag);
    if ((target->badstatus & 0x800) != 0)
    {
        cmbCopyCustomDevilToStock(user, &saveUser);
        cmbCopyCustomDevilToStock(target, &saveTarget);
    }
}

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

#ifdef NON_MATCHING
extern s32 datAddHp(datUnitWork_t *work, s32 hp);
extern s32 datAddMp(datUnitWork_t *work, s32 amount);
#endif

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

extern s32 cmbGetParamBase(datUnitWork_t *unit, s32 index);

const f32 cmpDefaultLightColor[4] = {1.0f, 1.0f, 1.0f, 0.0f};
const u32 cmpDefaultLightDir[4] = {0x3F75C28F, 0x3EB851EB, 0x3EE147AE, 0x3F800000};
const u32 cmpDefaultAmbientColor[4] = {0x3EF5C28F, 0x3EE66666, 0x3F0A3D70, 0x0};
const u32 cmpDefaultFogColor[4] = {0x3DCCCCCC, 0x3DCCCCCC, 0x3DCCCCCC, 0x3F800000};
const char cmpModelFreeMsg[24] = "Model %d:0x%03x free!!\n";
const char cmpBadEffReadyMsg[20] = "Bad effect ready!!\n";
const u32 D_00540054 __attribute__((section(".rodata"))) = 0;
const char cmpBadEffFreeMsg[24] = "Bad effect free!!\n";
const char cmpModelPrepareMsg[32] = "Model %d:0x%03x prepare...\n";
const char cmpModelReadyMsg[32] = "Model %d:0x%03x ready!!\n";
const char cmpTextVitality[16] = "Vitality";
const char cmpTextIntelligence[16] = "Intelligence";
const char cmpTextStrength[16] = "Strength";
const char cmpDebugIdFmt[16] = "id:%x %x\n";
s32 cmpUseItemKou(u16 item, datUnitWork_t *unit)
{
    s32 index = -1;

    switch (item) {
    case 0x26: index = 0; break;
    case 0x28: index = 2; break;
    case 0x29: index = 3; break;
    case 0x2A: index = 4; break;
    case 0x2B: index = 5; break;
    }
    if (index >= 0) {
        if (cmbGetParamBase(unit, index) < 40) {
            unit->param[index]++;
        }
        unit->maxhp = datGetMaxHp(unit);
        unit->maxmp = datGetMaxMp(unit);
        if ((unit->badstatus & 0x800) == 0) {
            unit->hp = unit->maxhp;
        }
        return index;
    }
    return -1;
}

s32 cmpUseItemTokusyu(u16 item, saddr_t target, saddr_t user)
{
    if (cmpUseItemKou(item, EE_ADDR_AS(datUnitWork_t *, user)) >= 0)
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

extern void fldEsutoMaStart(void);
extern void fldRiberaMaStart(void);
extern void fldRifutoMaStart(void);
extern void fldLightMaStart(void);

s32 cmpUseSkillTokusyu(u16 skill, datUnitWork_t *user, saddr_t unused)
{
    s32 se = 0x10;

    if (cmpGetSkillTargetArea(skill) == 0) {
        datExecSkill(skill, user, NULL);
        switch (skill) {
        case 0x49:
            fldEsutoMaStart();
            se = 0x1A;
            break;
        case 0x4A:
            fldRiberaMaStart();
            se = 0x1B;
            break;
        case 0x4B:
            fldRifutoMaStart();
            break;
        case 0x4C:
            fldLightMaStart();
            break;
        }
        p2sdPlaySE(se, 0x7F, 0x3F);
        return 4;
    }
    return 0;
}

void cmpRecover(u16 skill, datUnitWork_t *user, datUnitWork_t *target)
{
    s32 recovered = 0;
    s32 area = cmpGetSkillTargetArea(skill);
    /* This root feeds the count/stock/party phase. Its native register lifetime
     * ends at a recovery call; reload it before any subsequent loop use. */
    FAKEMATCH_ASM_REG(dds3GlobalWork_t *, root, "$7") = DDS3_GBWK;
    dds3GlobalWork_t *work;
    s32 i;

    if (area == 1) {
        cmpExecRecover(skill, user, target);
    } else {
        i = 0;
        work = root;
        while (i < work->stockCount) {
            target = &work->party[work->stock[i++]].unit;
            if (target->flag & 2) {
                cmpExecRecover(skill, user, target);
                root = DDS3_GBWK;
                recovered = 1;
            }
            work = root;
        }
    }
    if (cmpChkSkillBad(skill) & 0x800) {
        p2sdPlaySE(0x17, 0x7F, 0x3F);
    } else if (recovered == 0) {
        p2sdPlaySE(0x10, 0x7F, 0x3F);
    } else {
        p2sdPlaySE(0x16, 0x7F, 0x3F);
    }
}

char *cmpGetParamName(s8 index)
{
    return cmpParamNames[index];
}

s32 cmpGetPressedButton(s32 mask)
{
    if ((mask & 0x1) && dds3PadButtonState[0x21] < 0) {
        return 0x1;
    }
    if ((mask & 0x2) && dds3PadButtonState[0x23] < 0) {
        return 0x2;
    }
    if ((mask & 0x10) && (dds3PadButtonState[0x26] & 2)) {
        return 0x10;
    }
    if ((mask & 0x20) && (dds3PadButtonState[0x27] & 2)) {
        return 0x20;
    }
    if ((mask & 0x40) && (dds3PadButtonState[0x24] & 2)) {
        return 0x40;
    }
    if ((mask & 0x80) && (dds3PadButtonState[0x25] & 2)) {
        return 0x80;
    }
    if ((mask & 0x4) && dds3PadButtonState[0x22] < 0) {
        return 0x4;
    }
    if ((mask & 0x8) && dds3PadButtonState[0x20] < 0) {
        return 0x8;
    }
    if ((mask & 0x200) && (dds3PadButtonState[0x2A] & 2)) {
        return 0x200;
    }
    if ((mask & 0x100) && (dds3PadButtonState[0x28] & 2)) {
        return 0x100;
    }
    if ((mask & 0x800) && (dds3PadButtonState[0x2B] & 2)) {
        return 0x800;
    }
    if ((mask & 0x400) && (dds3PadButtonState[0x29] & 2)) {
        return 0x400;
    }
    if ((mask & 0x1000) && dds3PadButtonState[0x2C] < 0) {
        return 0x1000;
    }
    if ((mask & 0x2000) && dds3PadButtonState[0x2D] < 0) {
        return 0x2000;
    }
    if ((mask & 0x20000) && dds3PadButtonState[0x2A] < 0) {
        return 0x20000;
    }
    if ((mask & 0x10000) && dds3PadButtonState[0x28] < 0) {
        return 0x10000;
    }
    if ((mask & 0x80000) && dds3PadButtonState[0x2B] < 0) {
        return 0x80000;
    }
    if ((mask & 0x40000) && dds3PadButtonState[0x29] < 0) {
        return 0x40000;
    }
    return 0;
}

void func_002413B8(u32 button, s8 mode);

/* This UI entry fixes the shared cursor-sound helper's mode at zero. */
void cmpPlayCursorSE(u32 button)
{
    func_002413B8(button, 0);
}

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
void func_002413B8(u32 button, s8 mode)
{
    s32 sound;

    if (button & 0x8000) {
        p2sdPlaySE(0xD, 0x7F, 0x3F);
        return;
    }
    switch (button) {
    case 1:
    case 0x1000:
    case 0x2000:
        sound = 8;
        break;
    case 2:
        sound = 0xA;
        break;
    case 0x4000:
        sound = 0xC;
        break;
    case 0x10:
    case 0x20:
    case 0x40:
    case 0x80:
    case 0x100:
    case 0x200:
    case 0x400:
    case 0x800:
    case 0x10000:
    case 0x20000:
    case 0x40000:
    case 0x80000:
        switch (mode) {
        case 1:
            sound = 6;
            break;
        case 2:
            sound = 1;
            break;
        case 3:
            sound = 0x15;
            break;
        default:
            sound = 0;
            break;
        }
        break;
    default:
        return;
    }
    p2sdPlaySE(sound, 0x7F, 0x3F);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", func_002413B8);
#endif

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

#ifdef NON_MATCHING
extern s8 cmpGetCursorSlotState(s8 index, cmpCursorSlot_t *slot);
#endif

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

void cmpClearCursorSlots(cmpCursorInfo_t *cur)
{
    cmpCursorSlot_t (*slot)[2] = (cmpCursorSlot_t (*)[2])((u8 *)cur + 0x3C);
    s32 i;
    s32 j;

    for (j = 0; j < 0x100; j++, slot++) {
        u8 *valueBase;
        i = 0;
        /* The value word begins at +4 in each eight-byte slot. */
        valueBase = (u8 *)slot + 4;
        for (; i < 2; i++) {
            cmpCursorSlot_t *entry;
            s32 byteOffset;
            byteOffset = i * (s32)sizeof(cmpCursorSlot_t);
            entry = (cmpCursorSlot_t *)(byteOffset + (u8 *)slot);
            entry->state = 0;
            entry->param = 0;
            *(s32 *)(byteOffset + valueBase) = 0x100;
        }
    }
}


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

/* The offset and update pointer have real, separate native address lifetimes.
 * Keep their two short-lived registers; neither crosses a call. The empty
 * value-base operand preserves the conditional address phase, not memory order.
 * These are local matching aids, not evidence of historical source spelling. */
void cmpUpdateCursorSlots(cmpCursorInfo_t *cursor)
{
    cmpCursorSlot_t (*slot)[2] = (cmpCursorSlot_t (*)[2])((u8 *)cursor + 0x3C);
    s32 i;
    s32 j;
    s32 more;
    for (j = 0; j < 0x100; j++, slot++) {
        i = 0;
        do {
            FAKEMATCH_ASM_REG(s32, byteOffset, "$3");
            cmpCursorSlot_t *entry;
            byteOffset = i * (s32)sizeof(cmpCursorSlot_t);
            entry = (cmpCursorSlot_t *)(byteOffset + (u8 *)slot);
            i++;
            more = i < 2;
            if (entry->state != 0) {
                u8 *valueBase;
                s32 *value;
                FAKEMATCH_ASM_REG(s32 *, update, "$5");
                valueBase = (u8 *)slot + 4;
                FAKEMATCH_ASM_LIVE_VOLATILE(valueBase);
                value = (s32 *)(valueBase + byteOffset);
                update = value;
                if (*update < 0x100) {
                    *update += SDF_FRAME_STEP_INT(entry->param);
                } else {
                    entry->state = 0;
                }
                if (*value >= 0x100) {
                    *value = 0x100;
                }
            }
        } while (more);
    }
}

void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode)
{
    u8 *work = cur;

    memset(work + 8, 0, 0x18);
    *(s32 *)(work + 0x18) = a;
    *(s32 *)(work + 0x1C) = b;
    *(s32 *)(work + 8) = a;
    *(s32 *)(work + 0xC) = b;
    *(s32 *)(work + 0x10) = c;
    *(s32 *)(work + 0x14) = d;
    *(s32 *)work = mode;
}

void cmpInitCursorRange(void *cursor, u8 index, u8 rows, saddr_t flag)
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
    work->cursor.listData.flag = flag;
    work->cursor.flagsData.value = 0;
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

    flags = cursor->flagsData.bytes;
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

void cmpSetCursorFlag(cmpCursorInfo_t *cur, saddr_t flag)
{
    cur->flagsData.value = flag;
    cmpUpdateCursorScroll(cur, 1);
}

void cmp_241A10(cmpCursorInfo_t *cursor)
{
    u8 *root = cursor->flagsData.bytes;
    u8 *flags;
    s32 count;
    s32 i = cursor->unk_20;

    if (root == NULL) {
        return;
    }
    count = i--;
    cursor->unk_21 = 0;
    if (count > 0) {
        flags = root;
        /* Keep the actual scan pointer distinct from the null-tested root.
         * This empty operand is a local compiler aid, not a memory barrier. */
        FAKEMATCH_ASM_LIVE(flags);
        do {
            if ((flags[i] & 2) != 0) {
                cursor->unk_21++;
            }
        } while (i-- > 0);
    }
    cmpUpdateCursorScroll(cursor, 1);
}



s32 cmp_241A88(cmpCursorInfo_t *cursor, s32 index)
{
    u8 *flags = cursor->flagsData.bytes;
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

void cmpResetCursorPos(s32 value, cmpCursorInfo_t *cursor)
{
    cursor->StopFlag = 0;
    cursor->unk_24 = 0;
    cursor->unk_25 = 0;
    cursor->unk_22 = value;
}

void cmpSetCursorRow(s32 row, cmpCursorInfo_t *cursor)
{
    cursor->unk_24 = row;
}

void cmpResetCursorInfo(cmpCursorInfo_t *cursor)
{
    cursor->CursorSize = cursor->unk_18;
    cursor->CursorPos = cursor->unk_1C;
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
    u8 *flags = cursor->flagsData.bytes;
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

    flags = cursor->flagsData.bytes;
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

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * menu main savestate, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
s32 cmpFindNextCursorItem(cmpCursorInfo_t *cursor, s8 direction, s8 mode)
{
    s32 origin;
    s32 delta;
    s32 row;
    s32 index;
    s32 valid;
    s32 retry;
    u8 shift;
    u8 *flags;

    if (cursor->unk_20 < 2) {
        return 0;
    }
    if (direction == 0) {
        cursor->StopFlag = 0;
        return 0;
    }
    if (cursor->StopFlag != 0) {
        cursor->StopFlag = 2;
        return 0;
    }

    origin = cursor->unk_22 + (u8)cursor->unk_24;
    do {
        valid = 1;
        retry = 0;
        if (direction < 0) {
            if (cmpCursorAtFirstEnabled(cursor)) {
                shift = cursor->unk_21;
                delta = (cursor->unk_26 - (s8)shift - 1) * cursor->StepY;
                cursor->unk_22 = cursor->unk_20 - cursor->unk_26;
                cursor->unk_24 = cursor->unk_26 - 1;
                cursor->unk_25 = cursor->unk_26 - shift - 1;
                cursor->CursorPos += delta;
                cmpUpdateCursorScroll(cursor, -1);
            } else if (mode < (u8)cursor->unk_24 || cursor->unk_22 == 0) {
                flags = cursor->flagsData.bytes;
                cursor->unk_24 = (u8)cursor->unk_24 - 1;
                index = cursor->unk_22 + (u8)cursor->unk_24;
                if (flags != NULL && (flags[index] & 2) != 0) {
                    valid = 0;
                    retry = 1;
                }
                if (valid) {
                    cursor->unk_25--;
                    cursor->CursorPos -= cursor->StepY;
                }
            } else {
                cursor->unk_22--;
            }
            if (cmpCursorAtFirstEnabled(cursor)) {
                cursor->StopFlag = 1;
            }
        } else {
            if (cmpCursorAtLastEnabled(cursor)) {
                shift = cursor->unk_21;
                delta = (cursor->unk_26 - (s8)shift - 1) * cursor->StepY;
                cursor->unk_22 = 0;
                cursor->unk_24 = 0;
                cursor->unk_25 = 0;
                cursor->CursorPos -= delta;
                cmpUpdateCursorScroll(cursor, 1);
            } else if ((u8)cursor->unk_24 < (s32)(u8)cursor->unk_26 - mode - 1 ||
                       cursor->unk_22 + (u8)cursor->unk_26 == cursor->unk_20) {
                flags = cursor->flagsData.bytes;
                cursor->unk_24 = (u8)cursor->unk_24 + 1;
                index = cursor->unk_22 + (u8)cursor->unk_24;
                if (flags != NULL && (flags[index] & 2) != 0) {
                    valid = 0;
                    retry = 1;
                }
                if (valid) {
                    cursor->unk_25++;
                    cursor->CursorPos += cursor->StepY;
                }
            } else {
                cursor->unk_22++;
            }
            if (cmpCursorAtLastEnabled(cursor)) {
                cursor->StopFlag = 1;
            }
        }
    } while (retry);

    if (direction != 0) {
        if (((volatile cmpCursorInfo_t *)cursor)->unk_30 < 0x100) {
            ((volatile cmpCursorInfo_t *)cursor)->unk_30 = 0x100;
        }
        ((volatile cmpCursorInfo_t *)cursor)->unk_30 = 0x100;
        if (origin != cursor->unk_22 + (u8)cursor->unk_24) {
            ((cmpPadStopWork_t *)cursor)->unk_38 = direction >= 0 ? 2 : 1;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpFindNextCursorItem);
#endif

void cmpAdvanceCursorPhase(cmpCursorInfo_t *cursor)
{
    cursor->unk_34 += 6 * SDF_FRAME_DUE();
    if (cursor->unk_34 > 0x200) {
        cursor->unk_34 -= 0x200;
    }
}

#ifdef NON_MATCHING
extern s32 cmpFindNextCursorItem(cmpCursorInfo_t *cursor, s8 direction, s8 mode);
#endif

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
    /* Whether the cursor moved, which retail returns in v0 (lstMoveCursor's
     * loop reads it). The nesting keeps ee-gcc from turning this into a
     * sibling call, as in cmpMoveCursor2. */
    do
    {
        return cmpFindNextCursorItem(cur, dir, 1);
    } while (0);
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

#ifdef NON_MATCHING
void cmpChangeCursorListNums(s32 countValue, u8 *cursorData)
{
    cmpCursorInfo_t *cursor = (cmpCursorInfo_t *)cursorData;
    volatile cmpCursorInfo_t *live = cursor;
    u8 count = countValue;
    u8 oldCount = live->unk_20;
    u8 visible;
    u8 range;
    s32 offset;

    if (count == oldCount) {
        return;
    }
    if (oldCount == 0) {
        cmpInitCursorRange(cursor, count, count, 0);
        return;
    }
    visible = live->unk_26;
    if (count < visible) {
        cursor->unk_26 = count;
        visible = count;
        range = count;
    } else {
        range = live->unk_26;
        if (count != 0 && count < oldCount) {
            s32 remaining = count - cursor->unk_22;
            u16 position = live->unk_22;
            if (remaining < range) {
                u8 total = live->unk_20;
                cursor->unk_22 = position - (total - count);
            }
        }
    }
    if ((u8)cursor->unk_24 >= visible) {
        cursor->unk_24 = visible - 1;
    }
    offset = range - (s8)cursor->unk_21 - 1;
    if (offset < (u8)cursor->unk_25) {
        cursor->unk_25 = offset;
    }
    if (cursor->CursorPos != 0) {
        offset = (range - 1) * cursor->StepY;
        if (offset < cursor->CursorPos - cursor->unk_1C) {
            cursor->CursorPos = offset + cursor->unk_1C;
        }
    }
    cursor->unk_20 = count;
    cmpClearCursorSlots(cursor);
}
#else
INCLUDE_ASM(const s32, "code_0022C578", cmpChangeCursorListNums);
#endif

void cmpMoveCursorListEnd(cmpCursorInfo_t *cursor)
{
    s32 end = cursor->unk_20 + 0xFF;

    cursor->unk_25 = end - cursor->unk_21;
    cursor->unk_24 = end;
}

s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop)
{
    s8 delay = work->unk_38;

    if (delay < 0) {
        s32 next = work->cursor.unk_30;

        if (next >= 0) {
            f32 scaled = next;

            /* Native .lit4 coefficient 0x3f999999, exact on EE and host compilers. */
            next = (s32)(scaled / 0x1.333332p0f);
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

/* The alternate mask value feeds the final availability test. This empty
 * operand preserves the native branch-shaped selection in ee-gcc. It emits
 * no instructions and is a local reconstruction
 * aid, not evidence of the original source spelling. */
s32 cmpCursorSlotIsSelectable(datUnitWork_t *unit, u16 inputMask, s8 flag)
{
    u32 mask = inputMask;
    s32 flags = unit->flag;
    s32 test;
    s32 allowed;

    if ((flags & 1) != 0) {
        if (flag != 0 && (flags & 2) != 0) {
            allowed = mask & 0x20;
            if (allowed == 0) {
                goto no;
            }
        }
        if ((flags & 0x10) != 0) {
            allowed = mask & 0x40;
            if (allowed == 0) {
                goto no;
            }
        }
        if ((unit->badstatus & 0x800) != 0) {
            allowed = mask & 0x80;
            if (allowed == 0) {
                goto no;
            }
        }
        test = flags & 4;
        if (test != 0) {
            test = mask & 4;
        } else {
            test = mask & 8;
            FAKEMATCH_ASM_LIVE(test);
        }
        if (test == 0) {
            goto no;
        }
        return 1;
    }
    allowed = mask & 0x10;
    if (allowed != 0) {
        return 1;
    }
no:
    return 0;
}


s8 cmpGetStockInfoIndex(cmpLocalStock_t *stock, s8 index, s8 flag)
{
    s32 ofs = index * 0x103C + 0xC0;
    cmpStockInfo_t *info = (cmpStockInfo_t *)((u8 *)stock + ofs);
    s8 slot = *(u8 *)&info->unk_0E + info->unk_10;

    s8 result;
    if (flag == 0) {
        s32 offset = slot;
        offset += index * 0x56;
        result = ((s8 *)stock)[offset + 5];
    } else {
        s32 offset = slot;
        offset += index * 0x56;
        result = *(s8 *)((u32)offset + (u32)stock + 0x15);
    }
    return result;
}

void cmpBuildStockNameList(cmpLocalStock_t *stock)
{
    s8 outputIndex = 0;
    s32 i;

    stock->PartyCnt = 0;
    stock->StockCnt = 0;

    if ((stock->count & 1) != 0) {
        for (i = 0; i < DDS3_GBWK->stockCount; i++) {
            s8 stockId = DDS3_GBWK->stock[i];
            if ((DDS3_GBWK->party[stockId].unit.flag & 2) != 0 &&
                cmpCursorSlotIsSelectable(&DDS3_GBWK->party[stockId].unit, stock->count, 0) != 0) {
                stock->StockIdx[outputIndex] = stockId;
                stock->ListIdx[outputIndex] = i;
                outputIndex++;
                stock->PartyCnt++;
            }
        }
    }

    if ((stock->count & 2) != 0) {
        for (i = 4; i < 16; i++) {
            if ((DDS3_GBWK->party[i].unit.flag & 4) != 0) {
                continue;
            }
            if ((DDS3_GBWK->party[i].unit.flag & 1) == 0) {
                continue;
            }
            if (cmpCursorSlotIsSelectable(&DDS3_GBWK->party[i].unit, stock->count, 1) == 0) {
                continue;
            }
            {
                s32 j;
                s8 selectedIndex = outputIndex;

                stock->StockIdx[selectedIndex] = i;
                for (j = 0; j < DDS3_GBWK->stockCount; j++) {
                    if ((u8)DDS3_GBWK->stock[j] == i) {
                        stock->ListIdx[selectedIndex] = j;
                    }
                }
                stock->StockCnt++;
                outputIndex++;
            }
        }
    }
}


s8 cmpGetStockCount(void)
{
    return DDS3_GBWK->stockCount;
}

void cmpSortStockList(void)
{
    s32 i;
    s32 k;
    s8 j;
    s8 cur;

    for (i = 0; i < DDS3_GBWK->stockCount; i++)
    {
        j = 0;
        cur = DDS3_GBWK->stock[i];
        for (k = 0; k < DDS3_GBWK->stockCount; k++)
        {
            if ((i != k) && (cur == (u8)DDS3_GBWK->stock[k]))
            {
                DDS3_GBWK->stock[k] = 0;
                DDS3_GBWK->stockCount--;
            }
            else
            {
                DDS3_GBWK->stock[j++] = DDS3_GBWK->stock[k];
            }
        }
    }
}

s8 func_002426F0(void)
{
    if (DDS3_GBWK->stockCount < 0x10) {
        return DDS3_GBWK->stockCount;
    }
    return -1;
}

s8 func_00242710(u8 id)
{
    s32 count = DDS3_GBWK->stockCount;
    s8 n = 0;

    if (count > 0) {
        u8 *p = (u8 *)DDS3_GBWK->stock;
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

void cmpInitSeqInfo(cmpSeqInfo_t *pSeq, cmpSeqInfo_t *pParent)
{
    pSeq->unk_00 = 3;
    pSeq->unk_02 = -1;
    pSeq->unk_05 = 0;
    pSeq->unk_0C = 0;
    pSeq->unk_10 = EE_ADDR_S(pParent);
}

s32 cmpChkSeqInfo(cmbGbWork_t *work)
{
    if (work->unk_00005 != 0) {
        if (dds3GetProcessStat(EE_ADDR_AS(void *, work->unk_00008)) == 3) {
            work->unk_00005 = 0;
            return 2;
        }
        if (dds3GetProcessStat(EE_ADDR_AS(void *, work->unk_00008)) == 0) {
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



extern datSkill_t attr[];

s32 func_002429C0(void)
{
    s32 total = 0;
    s32 i;

    for (i = 0; i < DDS3_GBWK->akuma[0].skillcnt; i++) {
        total += attr[DDS3_GBWK->akuma[0].skill[i]].capacity;
    }
    return total;
}


s32 cmpItemHasFlag(u16 item, s8 which)
{
    if (which == 0) {
        u8 flag = datItemTable[item].flags & 1;
        if (flag != 0) {
            return 1;
        }
    } else {
        u32 flag = datItemTable[item].flags2 & 2;
        return flag == 0;
    }
    return 0;
}

void func_00242A70(s32 key) {
}

s32 campStatusIsActive(void)
{
    return campStatusActive != 0;
}

const char D_00540128[16] = "BeelzebubM";
const char D_00540138[16] = "Seven-Eye";
const char D_00540148[16] = "Station Hand";
const char D_00540158[16] = "Masakado";
const char D_00540168[16] = "Flauros Hallel";
const char D_00540178[16] = "Ose Hallel";
const char D_00540188[16] = "Baal Avatar";
const char D_00540198[16] = "Kagutsuchi";
const char cmpTextProtegonist[16] = "Protegonist";
const char D_005401B8[16] = "Black Frost";
const char cmpTextSakahagi[16] = "Sakahagi";
const char D_005401D8[16] = "Futomimi";
const char D_005401E8[16] = "Trumpeter";
const char D_005401F8[16] = "The Harlot";
const char D_00540208[16] = "Daisoujou";
const char D_00540218[16] = "Hell Biker";
const char D_00540228[16] = "Black Rider";
const char D_00540238[16] = "Red Rider";
const char D_00540248[16] = "White Rider";
const char D_00540258[16] = "Pale Rider";
const char D_00540268[16] = "Beelzebub";
const char D_00540278[16] = "Metatron";
const char D_00540288[16] = "Chatterskull";
const char D_00540298[16] = "Lachesis";
const char D_005402A8[16] = "Ongyo-Ki";
const char D_005402B8[16] = "Yatagarasu";
const char D_005402C8[16] = "Gui Xian";
const char D_005402D8[16] = "Cu Chulainn";
const char D_005402E8[16] = "Arahabaki";
const char D_005402F8[16] = "Valkyrie";
const char D_00540308[16] = "Will o' Wisp";
const char D_00540318[16] = "Black Ooze";
const char D_00540328[16] = "Choronzon";
const char D_00540338[16] = "Rakshasa";
const char D_00540348[16] = "Hresvelgr";
const char D_00540358[16] = "Succubus";
const char D_00540368[16] = "Queen Mab";
const char D_00540378[16] = "Baphomet";
const char D_00540388[16] = "Girimehkala";
const char D_00540398[16] = "Datsue-Ba";
const char D_005403A8[16] = "Shikigami";
const char D_005403B8[16] = "Momunofu";
const char D_005403C8[16] = "Shiki-Ouji";
const char D_005403D8[16] = "Sarutahiko";
const char D_005403E8[16] = "Gogmagog";
const char D_005403F8[16] = "Nekomata";
const char D_00540408[16] = "Badb Catha";
const char D_00540418[16] = "Cerberus";
const char D_00540428[16] = "Raja Naga";
const char D_00540438[16] = "Quetzalcoatl";
const char D_00540448[16] = "Yurlungur";
const char D_00540458[16] = "Decarabia";
const char D_00540468[16] = "Archangel";
const char D_00540478[16] = "Principality";
const char D_00540488[16] = "Dominion";
const char D_00540498[16] = "Jack Frost";
const char D_005404A8[16] = "High Pixie";
const char D_005404B8[16] = "Pyro Jack";
const char D_005404C8[16] = "Ara Mitama";
const char D_005404D8[16] = "Nigi Mitama";
const char D_005404E8[16] = "Kusi Mitama";
const char D_005404F8[16] = "Saki Mitama";
const char D_00540508[16] = "Feng Huang";
const char D_00540518[16] = "Minakata";
const char D_00540528[16] = "Okuninushi";
const char D_00540538[16] = "Mikazuchi";
const char D_00540548[16] = "Bishamon";
const char D_00540558[16] = "Kikuri-Hime";
const char D_00540568[16] = "Kushinada";
const char D_00540578[16] = "Dionysus";
const char D_00540588[16] = "Beiji-Weng";
const char D_00540598[16] = "Sarasvati";
const char D_005405A8[16] = "Scathach";
const char D_005405B8[16] = "Amaterasu";
const char D_005405C8[16] = "Null:curse";
const char D_005405D8[32] = "Resist:phys Weak:fire/ice";
const char D_005405F8[24] = "Null:ice Weak:fire";
const char D_00540610[24] = "Null:force Weak:fire";
const char D_00540628[24] = "Null:fire Weak:force";
const char D_00540640[16] = "Null:mind";
const char D_00540650[24] = "Null:expel Weak:death";
const char D_00540668[40] = "Repel:expel/death Drain:curse/mind";
const char D_00540690[24] = "Null:fire Drain:mind";
const char D_005406A8[40] = "Repel:elec Null:expel Weak:curse";
const char D_005406D0[32] = "Repel:magic Null:expel/death";
const char D_005406F0[24] = "Null:magic/expel/death";
const char D_00540708[16] = "Null:magic";
const char D_00540718[32] = "Repel:expel/death Resist:all";
const char D_00540738[32] = "Null:all except almighty";
const char D_00540758[24] = "Normal resistance";
const char D_00540770[40] = "Resist:phys Weak:force/expel/death";
const char D_00540798[16] = "Null:nerve";
const char D_005407A8[24] = "Repel:elec Weak:force";
const char D_005407C0[24] = "Resist:phys Weak:magic";
const char D_005407D8[16] = "Resist:ailments";
const char D_005407E8[16] = "Resist:all";
const char D_005407F8[48] = "Repel:fire/death Drn:ice Null:expel Res:phys";
const char D_00540828[40] = "Null:expel/death/ailments Resist:phys";
const char D_00540850[56] = "Repel:phys Drn:elec Null:expel/death/curse/nerve/mind";
const char D_00540888[48] = "Repel:expel/death Drain:curse/mind Null:nerve";
const char D_005408B8[48] = "Drain:force Null:fire/expel/death/ailments";
const char D_005408E8[32] = "Null:force/expel/death/ailments";
const char D_00540908[40] = "Drain:ice Null:expel/death/ailments";
const char D_00540930[40] = "Null:elec/force/expel/death/ailments";
const char D_00540958[32] = "Null:fire/expel/death/ailments";
const char D_00540978[32] = "Null:ice/expel/death/ailments";
const char D_00540998[32] = "Repel:expel/death Null:ailments";
const char D_005409B8[48] = "Null:expel/death/ailments Resist:phys/magic";
const char D_005409E8[40] = "Drn:fire Null:death Weak:expel/curse";
const char D_00540A10[40] = "Null:death/curse/nerve Weak:expel";
const char D_00540A38[24] = "Null:expel/death/mind";
const char D_00540A50[16] = "Null:ailments";
const char D_00540A60[40] = "Null:phys/death/curse/mind Weak:expel";
const char D_00540A88[24] = "Null:force/death";
const char D_00540AA0[40] = "Null:death Resist:magic Weak:expel";
const char D_00540AC8[32] = "Null:death/curse/mind Weak:fire";
const char D_00540AE8[32] = "Repel:expel/death Weak:elec";
const char D_00540B08[24] = "Null:expel/death";
const char D_00540B20[24] = "Null:phys/ice/expel";
const char D_00540B38[40] = "Repel:death Null:nerve/mind Weak:expel";
const char D_00540B60[24] = "Repel:force/expel";
const char D_00540B78[40] = "Repel:expel Null:curse/nerve Weak:death";
const char D_00540BA0[40] = "Repel:expel Drain:elec Weak:death";
const char D_00540BC8[32] = "Repel:expel Drain:ice Weak:elec";
const char D_00540BE8[32] = "Drain:ice Null:expel Weak:fire";
const char D_00540C08[24] = "Repel:force Null:expel";
const char D_00540C20[24] = "Null:expel Resist:phys";
const char D_00540C38[24] = "Drain:force Null:expel";
const char D_00540C50[48] = "Repel:ice Null:phys/expel/death Weak:all other";
const char D_00540C80[24] = "Drain:fire Null:expel";
const char D_00540C98[32] = "Null:ice/force/expel Weak:elec";
const char D_00540CB8[24] = "Repel:expel Null:fire";
const char D_00540CD0[32] = "Repel:expel Null:fire/force";
const char D_00540CF0[40] = "Repel:expel Null:fire/elec/force";
const char D_00540D18[24] = "Repel:expel Null:magic";
const char D_00540D30[40] = "Null:death Res:phys Weak:magic/expel";
const char D_00540D58[24] = "Null:death Weak:expel";
const char D_00540D70[40] = "Null:death Res:phys Weak:fire/expel";
const char D_00540D98[40] = "Null:death Res:phys Weak:ice/expel";
const char D_00540DC0[40] = "Null:death Res:phys Weak:elec/expel";
const char D_00540DE8[40] = "Null:death Res:phys Weak:force/expel";
const char D_00540E10[24] = "Null:death Weak:magic";
const char D_00540E28[48] = "Drn:fire Null:death Res:phys Weak:force/expel";
const char D_00540E58[32] = "Null:death Weak:ice/expel";
const char D_00540E78[32] = "Repel:death Weak:elec/expel";
const char D_00540E98[40] = "Null:death Resist:ailments Weak:fire";
const char D_00540EC0[40] = "Null:death Resist:ailments Weak:elec";
const char D_00540EE8[32] = "Null:ice/death Weak:fire";
const char D_00540F08[40] = "Drain:elec Null:death Weak:force";
const char D_00540F30[32] = "Null:fire/death Weak:elec";
const char D_00540F50[32] = "Repel:ice Null:death Weak:fire";
const char D_00540F70[24] = "Null:elec Weak:ice";
const char D_00540F88[24] = "Null:ice Weak:elec";
const char D_00540FA0[32] = "Null:curse/mind Weak:force";
const char D_00540FC0[24] = "Null:mind Weak:expel";
const char D_00540FD8[16] = "Resist:magic";
const char D_00540FE8[32] = "Resist:phys/magic/ailments";
const char D_00541008[48] = "Drain:fire Null:death/nerve/mind Weak:ice";
const char D_00541038[40] = "Repel:death Null:expel Resist:magic";
const char D_00541060[40] = "Null:expel/death/ailments Weak:elec";
const char D_00541088[24] = "Repel:death Weak:expel";
const char D_005410A0[24] = "Null:death Weak:ice";
const char D_005410B8[24] = "Null:death Weak:curse";
const char D_005410D0[24] = "Repel:phys Null:death";
const char D_005410E8[40] = "Drain:phys Null:death Weak:curse";
const char D_00541110[32] = "Null:nerve/mind Weak:elec";
const char D_00541130[24] = "Null:nerve Weak:ice";
const char D_00541148[40] = "Null:curse/nerve Res:mind Weak:fire";
const char D_00541170[24] = "Repel:phys Weak:elec";
const char D_00541188[24] = "Repel:elec Weak:fire";
const char D_005411A0[32] = "Resist:phys Weak:ailments";
const char D_005411C0[24] = "Null:nerve Weak:force";
const char D_005411D8[24] = "Resist:phys Weak:nerve";
const char D_005411F0[40] = "Repel:expel/death Null:phys Weak:fire";
const char D_00541218[24] = "Resist:fire Weak:ice";
const char D_00541230[24] = "Resist:force Weak:fire";
const char D_00541248[32] = "Null:expel/death Weak:nerve";
const char D_00541268[24] = "Null:force Weak:elec";
const char D_00541280[32] = "Null:phys/ice/death Weak:fire";
const char D_005412A0[32] = "Null:fire/death Weak:force";
const char D_005412C0[24] = "Resist:phys Weak:elec";
const char D_005412D8[32] = "Repel:force Null:ailments";
const char D_005412F8[32] = "Repel:fire Resist:phys Weak:ice";
const char D_00541318[24] = "Drain:force Weak:elec";
const char D_00541330[40] = "Drain:elec Null:ice/death Weak:fire";
const char D_00541358[40] = "Drain:elec Resist:phys Weak:fire";
const char D_00541380[32] = "Null:ice Resist:phys Weak:elec";
const char D_005413A0[32] = "Null:magic Weak:ailments";
const char D_005413C0[32] = "Drain:ice Null:death Weak:elec";
const char D_005413E0[40] = "Null:death Resist:phys Weak:elec";
const char D_00541408[40] = "Drn:fire Null:death Res:phys Weak:ice";
const char D_00541430[24] = "Null:death Weak:nerve";
const char D_00541448[24] = "Null:death Resist:magic";
const char D_00541460[24] = "Repel:death Weak:nerve";
const char D_00541478[32] = "Repel:expel Weak:ice/death";
const char D_00541498[32] = "Repel:expel Weak:death/mind";
const char D_005414B8[32] = "Repel:expel Weak:death/nerve";
const char D_005414D8[32] = "Repel:expel Weak:force/death";
const char D_005414F8[32] = "Repel:expel Weak:death/curse";
const char D_00541518[40] = "Repel:expel Drn:fire Weak:ice/death";
const char D_00541540[24] = "Drain:ice Weak:fire";
const char D_00541558[16] = "Resist:elec";
const char D_00541568[24] = "Resist:ice Weak:force";
const char D_00541580[32] = "Repel:force Weak:curse/nerve";
const char D_005415A0[24] = "Drain:ice Weak:mind";
const char D_005415B8[24] = "Resist:phys Weak:curse";
const char D_005415D0[32] = "Repel:expel/death Resist:magic";
const char D_005415F0[24] = "Null:force Weak:curse";
const char D_00541608[24] = "Drain:elec Weak:fire";
const char D_00541620[24] = "Resist:ice Weak:fire";
const char D_00541638[16] = "Drain:fire";
const char D_00541648[24] = "Repel:force Weak:curse";
const char D_00541660[24] = "Drain:force Weak:nerve";
const char D_00541678[32] = "Resist:phys Weak:curse/nerve";
const char D_00541698[24] = "Drain:fire Weak:ice";
const char D_005416B0[32] = "Null:expel/death/ailments";
const char D_005416D0[32] = "Null:expel/death Weak:force";
const char D_005416F0[32] = "Null:expel/death Resist:magic";
const char D_00541710[40] = "Repel:ice Null:expel/death Weak:fire";
const char D_00541738[40] = "Drain:fire Null:expel/death Weak:ice";
const char D_00541760[32] = "Null:expel/curse/mind Weak:elec";
const char D_00541780[32] = "Null:ice/expel Weak:ailments";
const char D_005417A0[32] = "Drain:elec Null:expel Weak:fire";
const char D_005417C0[32] = "Drain:elec Null:expel Weak:ice";
const char D_005417E0[40] = "Drain:force Null:expel Weak:elec";
const char D_00541808[32] = "Null:ice/expel Weak:fire";
const char D_00541828[40] = "Drn:fire Null:expel Weak:nerve/mind";
const char D_00541850[40] = "Repel:elec Null:expel Weak:fire/nerve";
const char D_00541878[40] = "Null:expel/curse/nerve Weak:force";
const char D_005418A0[40] = "Repel:force Null:expel Weak:elec";
const char D_005418C8[24] = "Repel:expel Null:death";
const char D_005418E0[40] = "Repel:elec Null:expel Weak:force";
const char D_00541908[32] = "Repel:ice Null:expel Weak:fire";
const char D_00541928[40] = "Drn:elec Null:expel Weak:curse/nerve";
const char D_00541950[32] = "Repel:fire Null:expel Weak:ice";
const char D_00541970[32] = "Null:expel/nerve/mind Weak:fire";
const char D_00541990[24] = "Repel:expel Weak:death";
const char D_005419A8[40] = "Repel:death Null:fire/expel Weak:ice";
const char D_005419D0[32] = "Null:fire/expel/death Weak:ice";
const char D_005419F0[24] = "Null:phys/expel/death";
const char D_00541A08[40] = "Null:expel/death Res:phys Weak:fire";
const char D_00541A30[40] = "Null:expel/death Resist:all other";
const char D_00541A58[32] = "Null:force/expel Weak:elec";
const char D_00541A78[32] = "Drain:fire Repel:expel Weak:ice";
const char D_00541A98[32] = "Null:expel/ailments Weak:fire";
const char D_00541AB8[40] = "Null:force/expel/curse Weak:elec";
const char D_00541AE0[40] = "Repel:expel Null:ailments Weak:force";
const char D_00541B08[40] = "Repel:expel Resist:phys Weak:death";
const char D_00541B30[40] = "Repel:expel/death Weak:nerve/mind";
const char D_00541B58[32] = "Null:ice/expel Weak:force";
const char D_00541B78[24] = "Null:fire/expel/death";
const char D_00541B90[40] = "Repel:phys Null:expel/death Weak:ice";
const char D_00541BB8[40] = "Repel:expel Null:death Res:all other";
const char D_00541BE0[16] = "Null:expel";
const char cmpTextStarKey[16] = "Star Key";
const char cmpTextAfterlifeBell[16] = "Afterlife Bell";
const char cmpTextBlackVisor[16] = "Black Visor";
const char cmpTextDeathstone[16] = "Deathstone";
const char cmpTextMoonKey[16] = "Moon Key";
const char cmpTextKnowledge[16] = "Knowledge";
const char cmpTextSovereignty[16] = "Sovereignty";
const char cmpTextFoundation[16] = "Foundation";
const char cmpTextEternity[16] = "Eternity";
const char cmpTextGodliness[16] = "Godliness";
const char cmpTextCompassion[16] = "Compassion";
const char cmpTextNetherstone[16] = "Netherstone";
const char cmpTextEarthstone[16] = "Earthstone";
const char cmpTextHeavenstone[16] = "Heavenstone";
const char cmpTextParkKey[16] = "Park Key";
const char cmpTextLordSSword[16] = "Lord's Sword";
const char cmpTextKimonStone[16] = "Kimon Stone";
const char cmpTextHimorogi[16] = "Himorogi";
const char cmpTextUmugiStone[16] = "Umugi Stone";
const char cmpText1000YenBill[16] = "1000 Yen Bill";
const char cmpTextRedKila[16] = "Red Kila";
const char cmpTextGreenKila[16] = "Green Kila";
const char cmpTextYellowKila[16] = "Yellow Kila";
const char cmpTextWhiteKila[16] = "White Kila";
const char cmpTextAnnexPass[16] = "Annex Pass";
const char cmpTextAquamarine[16] = "Aquamarine";
const char cmpTextTurquoise[16] = "Turquoise";
const char cmpTextAmethyst[16] = "Amethyst";
const char cmpTextSapphire[16] = "Sapphire";
const char cmpTextMasakados[16] = "Masakados";
const char cmpTextKamurogi[16] = "Kamurogi";
const char cmpTextMurakumo[16] = "Murakumo";
const char cmpTextAnathema[16] = "Anathema";
const char cmpTextNarukami[16] = "Narukami";
const char cmpTextShiranui[16] = "Shiranui";
const char cmpTextIyomante[16] = "Iyomante";
const char cmpTextWadatsumi[16] = "Wadatsumi";
const char cmpTextMarogareh[16] = "Marogareh";
const char cmpTextLuckyTicket[16] = "Lucky Ticket";
const char cmpTextDiviningWater[16] = "Divining Water";
const char cmpTextSmokeBall[16] = "Smoke Ball";
const char cmpTextLightBall[16] = "Light Ball";
const char cmpTextFloatBall[16] = "Float Ball";
const char cmpTextAttractPipe[16] = "Attract Pipe";
const char cmpTextRepulseBell[16] = "Repulse Bell";
const char cmpTextSpyglass[16] = "Spyglass";
const char cmpTextSoulReturn[16] = "Soul-Return";
const char cmpTextBlessedFan[16] = "Blessed Fan";
const char cmpTextLuIncense[16] = "Lu Incense";
const char cmpTextAgIncense[16] = "Ag Incense";
const char cmpTextViIncense[16] = "Vi Incense";
const char cmpTextMaIncense[16] = "Ma Incense";
const char cmpTextStIncense[16] = "St Incense";
const char cmpTextTetrajaRock[16] = "Tetraja Rock";
const char cmpTextDekajaRock[16] = "Dekaja Rock";
const char cmpTextAttackMirror[16] = "Attack Mirror";
const char cmpTextMagicMirror[16] = "Magic Mirror";
const char cmpTextPotOfDeath[16] = "Pot of Death";
const char cmpTextMakajamRock[16] = "Makajam Rock";
const char cmpTextWagtailPlume[16] = "Wagtail Plume";
const char cmpTextPoisonArrow[16] = "Poison Arrow";
const char cmpTextMegidolaRock[16] = "Megidola Rock";
const char cmpTextMegidoRock[16] = "Megido Rock";
const char cmpTextMamudoRock[16] = "Mamudo Rock";
const char cmpTextMahamaRock[16] = "Mahama Rock";
const char cmpTextMazanRock[16] = "Mazan Rock";
const char cmpTextMazioRock[16] = "Mazio Rock";
const char cmpTextMabufuRock[16] = "Mabufu Rock";
const char cmpTextMaragiRock[16] = "Maragi Rock";
const char cmpTextDisStone[16] = "Dis-Stone";
const char cmpTextDisMute[16] = "Dis-Mute";
const char cmpTextDisCharm[16] = "Dis-Charm";
const char cmpTextDisStun[16] = "Dis-Stun";
const char cmpTextDisPoison[16] = "Dis-Poison";
const char cmpTextSacredWater[16] = "Sacred Water";
const char cmpTextBalmOfRising[16] = "Balm of Rising";
const char cmpTextRevivalBead[16] = "Revival Bead";
const char cmpTextChakraElixir[16] = "Chakra Elixir";
const char cmpTextBeadOfLife[16] = "Bead of Life";
const char cmpTextSomaDroplet[16] = "Soma Droplet";
const char cmpTextGreatChakra[16] = "Great Chakra";
const char cmpTextChakraPot[16] = "Chakra Pot";
const char cmpTextChakraDrop[16] = "Chakra Drop";
const char cmpTextBeadChain[16] = "Bead Chain";
const char cmpTextLifeStone[16] = "Life Stone";
const char cmpTextMedicine[16] = "Medicine";
const char cmpTextMuscleDrink[16] = "Muscle Drink";
const char D_00542160[16] = "Reserve\314\314\314\314\314\314\314\314";
const char cmpTextCallEvil[16] = "Call Evil";
const char cmpTextCallSouls[16] = "Call Souls";
const char cmpTextCallSoldier[16] = "Call Soldier";
const char cmpTextCallAngel[16] = "Call Angel";
const char cmpTextJiveTalk[16] = "Jive Talk";
const char cmpTextWineParty[16] = "Wine Party";
const char cmpTextMaidenPlea[16] = "Maiden Plea";
const char cmpTextIntimidate[16] = "Intimidate";
const char cmpTextPersuade[16] = "Persuade";
const char cmpTextGonnection[16] = "Gonnection";
const char cmpTextArbitration[16] = "Arbitration";
const char cmpTextStoneHunt[16] = "Stone Hunt";
const char D_00542230[16] = "Threaten";
const char D_00542240[16] = "Death Pact";
const char D_00542250[16] = "Mischief";
const char D_00542260[16] = "Soul Recruit";
const char D_00542270[16] = "Dark Pledge";
const char D_00542280[16] = "Brainwash";
const char D_00542290[16] = "Son's Oath";
const char D_005422A0[16] = "Never Yield";
const char D_005422B0[16] = "Charisma";
const char D_005422C0[16] = "Watchful";
const char D_005422D0[16] = "Lucky Find";
const char D_005422E0[16] = "Thief's Lore";
const char D_005422F0[16] = "Mana Refill";
const char D_00542300[16] = "Life Refill";
const char D_00542310[16] = "Victory Cry";
const char D_00542320[16] = "Mana Aid";
const char D_00542330[16] = "Life Aid";
const char D_00542340[16] = "Force Repel";
const char D_00542350[16] = "Elec Repel";
const char D_00542360[16] = "Ice Repel";
const char D_00542370[16] = "Fire Repel";
const char D_00542380[16] = "Phys Repel";
const char D_00542390[16] = "Force Drain";
const char D_005423A0[16] = "Elec Drain";
const char D_005423B0[16] = "Ice Drain";
const char D_005423C0[16] = "Fire Drain";
const char D_005423D0[16] = "Phys Drain";
const char D_005423E0[16] = "Void Mind";
const char D_005423F0[16] = "Void Nerve";
const char D_00542400[16] = "Void Curse";
const char D_00542410[16] = "Void Death";
const char D_00542420[16] = "Void Expel";
const char D_00542430[16] = "Void Force";
const char D_00542440[16] = "Void Elec";
const char D_00542450[16] = "Void Ice";
const char D_00542460[16] = "Void Fire";
const char D_00542470[16] = "Void Phys";
const char D_00542480[16] = "Anti-Mind";
const char D_00542490[16] = "Anti-Nerve";
const char D_005424A0[16] = "Anti-Curse";
const char D_005424B0[16] = "Anti-Death";
const char D_005424C0[16] = "Anti-Expel";
const char D_005424D0[16] = "Anti-Force";
const char D_005424E0[16] = "Anti-Elec";
const char D_005424F0[16] = "Anti-Ice";
const char D_00542500[16] = "Anti-Fire";
const char D_00542510[16] = "Anti-Phys";
const char D_00542520[16] = "Force Boost";
const char D_00542530[16] = "Elec Boost";
const char D_00542540[16] = "Ice Boost";
const char D_00542550[16] = "Fire Boost";
const char D_00542560[16] = "Attack All";
const char D_00542570[16] = "Drain Attack";
const char D_00542580[16] = "Dark Might";
const char D_00542590[16] = "Bright Might";
const char D_005425A0[16] = "Mind's Eye";
const char D_005425B0[16] = "Fast Retreat";
const char D_005425C0[16] = "Mana Surge";
const char D_005425D0[16] = "Mana Gain";
const char D_005425E0[16] = "Mana Bonus";
const char D_005425F0[16] = "Life Surge";
const char D_00542600[16] = "Life Gain";
const char D_00542610[16] = "Life Bonus";
const char D_00542620[16] = "God's Bow";
const char D_00542630[16] = "Beast Roar";
const char D_00542640[16] = "Death Lust";
const char D_00542650[16] = "Hell Throttle";
const char D_00542660[16] = "Hell Burner";
const char D_00542670[16] = "Hell Exhaust";
const char D_00542680[16] = "Hell Spin";
const char D_00542690[16] = "Terrorblade";
const char D_005426A0[16] = "Meditation";
const char D_005426B0[16] = "Red Capote";
const char D_005426C0[16] = "Andalucia";
const char D_005426D0[16] = "Holy Star";
const char D_005426E0[16] = "High King";
const char D_005426F0[16] = "Root of Evil";
const char D_00542700[16] = "Evil Gleam";
const char D_00542710[16] = "Showtime";
const char D_00542720[16] = "Whirlwind";
const char D_00542730[16] = "Roundtrip";
const char D_00542740[16] = "Bullet-Time";
const char D_00542750[16] = "Rebellion";
const char D_00542760[16] = "Soul Divide";
const char D_00542770[16] = "Death Flies";
const char D_00542780[16] = "(Summon)";
const char D_00542790[16] = "Phase Shift";
const char D_005427A0[16] = "Apocalypse";
const char D_005427B0[16] = "Domination";
const char D_005427C0[16] = "Wild Dance";
const char D_005427D0[16] = "Lost Macca!";
const char D_005427E0[16] = "Icy Death";
const char D_005427F0[16] = "Hell's Call";
const char D_00542800[16] = "God's Curse";
const char D_00542810[16] = "Vast Light";
const char D_00542820[16] = "Conjuration";
const char D_00542830[16] = "Fire of Sinai";
const char D_00542840[16] = "Replicate";
const char D_00542850[16] = "Foul Union";
const char D_00542860[16] = "Divine Will";
const char cmpTextLaughter[16] = "Laughter";
const char cmpTextDarkHowl[16] = "Dark Howl";
const char cmpTextGathering[16] = "Gathering";
const char cmpTextBeckonCall[16] = "Beckon Call";
const char cmpTextInfiniteLight[16] = "Infinite Light";
const char cmpTextDragonEye[16] = "Dragon Eye";
const char cmpTextBeastEye[16] = "Beast Eye";
const char cmpTextIntoxicate[16] = "Intoxicate";
const char cmpTextPanicVoice[16] = "Panic Voice";
const char cmpTextSexyGaze[16] = "Sexy Gaze";
const char cmpTextSonicWave[16] = "Sonic Wave";
const char cmpTextEternalRest[16] = "Eternal Rest";
const char cmpTextBindingCry[16] = "Binding Cry";
const char cmpTextStunGaze[16] = "Stun Gaze";
const char cmpTextSolNiger[16] = "Sol Niger";
const char cmpTextDismalTune[16] = "Dismal Tune";
const char cmpTextDebilitate[16] = "Debilitate";
const char cmpTextFogBreath[16] = "Fog Breath";
const char cmpTextToxicCloud[16] = "Toxic Cloud";
const char cmpTextBaelSBane[16] = "Bael's Bane";
const char cmpTextEvilGaze[16] = "Evil Gaze";
const char cmpTextMuteGaze[16] = "Mute Gaze";
const char cmpTextStoneGaze[16] = "Stone Gaze";
const char cmpTextHellGaze[16] = "Hell Gaze";
const char cmpTextRadiance[16] = "Radiance";
const char cmpTextStarlight[16] = "Starlight";
const char cmpTextVioletFlash[16] = "Violet Flash";
const char cmpTextLifeDrain[16] = "Life Drain";
const char cmpTextManaDrain[16] = "Mana Drain";
const char cmpTextDeathtouch[16] = "Deathtouch";
const char cmpTextHolyWrath[16] = "Holy Wrath";
const char cmpTextThunderclap[16] = "Thunderclap";
const char cmpTextWetWind[16] = "Wet Wind";
const char cmpTextWindCutter[16] = "Wind Cutter";
const char cmpTextWingBuffet[16] = "Wing Buffet";
const char cmpTextBoltStorm[16] = "Bolt Storm";
const char cmpTextGlacialBlast[16] = "Glacial Blast";
const char cmpTextIceBreath[16] = "Ice Breath";
const char cmpTextRagnarok[16] = "Ragnarok";
const char cmpTextProminence[16] = "Prominence";
const char cmpTextHellfire[16] = "Hellfire";
const char cmpTextFireBreath[16] = "Fire Breath";
const char cmpTextTentacle[16] = "Tentacle";
const char cmpTextExtraHit[16] = "Extra Hit";
const char cmpTextRetaliate[16] = "Retaliate";
const char cmpTextGaeaRage[16] = "Gaea Rage";
const char cmpTextMagmaAxis[16] = "Magma Axis";
const char cmpTextSpiralViper[16] = "Spiral Viper";
const char cmpTextEvilMelody[16] = "Evil Melody";
const char cmpTextHolyMelody[16] = "Holy Melody";
const char D_00542B90[16] = "Earthquake";
const char D_00542BA0[16] = "Foul Havoc";
const char D_00542BB0[16] = "Last Resort";
const char D_00542BC0[16] = "Freikugel";
const char D_00542BD0[16] = "Oni-Kagura";
const char D_00542BE0[16] = "Javelin Rain";
const char D_00542BF0[16] = "Divine Shot";
const char D_00542C00[16] = "Xeros-Beat";
const char D_00542C10[16] = "Deadly Fury";
const char D_00542C20[16] = "Godly Light";
const char D_00542C30[16] = "Iron Claw";
const char D_00542C40[16] = "Stun Claw";
const char D_00542C50[16] = "Venom Claw";
const char D_00542C60[16] = "Feral Claw";
const char D_00542C70[16] = "Hell Fang";
const char D_00542C80[16] = "Stun Bite";
const char D_00542C90[16] = "Stone Bite";
const char D_00542CA0[16] = "Charm Bite";
const char D_00542CB0[16] = "Venom Bite";
const char D_00542CC0[16] = "Feral Bite";
const char D_00542CD0[16] = "Kamikaze";
const char D_00542CE0[16] = "Sacrifice";
const char D_00542CF0[16] = "Arid Needle";
const char D_00542D00[16] = "Toxic Sting";
const char D_00542D10[16] = "Stun Needle";
const char D_00542D20[16] = "Needle Rush";
const char D_00542D30[16] = "Chaos Blade";
const char D_00542D40[16] = "Guillotine";
const char D_00542D50[16] = "Deathbound";
const char D_00542D60[16] = "Mighty Gust";
const char D_00542D70[16] = "Stasis Blade";
const char D_00542D80[16] = "Dark Sword";
const char D_00542D90[16] = "Hassohappa";
const char D_00542DA0[16] = "Brutal Slash";
const char D_00542DB0[16] = "Heat Wave";
const char D_00542DC0[16] = "Hades Blast";
const char D_00542DD0[16] = "Hell Thrust";
const char D_00542DE0[16] = "Pestilence";
const char D_00542DF0[16] = "Lightoma";
const char D_00542E00[16] = "Riberama";
const char D_00542E10[16] = "Tetrakarn";
const char D_00542E20[16] = "Makarakarn";
const char D_00542E30[16] = "Makakaja";
const char D_00542E40[16] = "Rakukaja";
const char D_00542E50[16] = "Sukukaja";
const char D_00542E60[16] = "Tarukaja";
const char D_00542E70[16] = "Tentarafoo";
const char D_00542E80[16] = "Marin Karin";
const char D_00542E90[16] = "Shibaboo";
const char D_00542EA0[16] = "Makajamon";
const char D_00542EB0[16] = "Recarmdra";
const char D_00542EC0[16] = "Samrecarm";
const char D_00542ED0[16] = "Paraladi";
const char D_00542EE0[16] = "Posumudi";
const char D_00542EF0[16] = "Me Patra";
const char D_00542F00[16] = "Makatora";
const char D_00542F10[16] = "Mediarahan";
const char D_00542F20[16] = "Mediarama";
const char D_00542F30[16] = "Diarahan";
const char D_00542F40[16] = "Mamudoon";
const char D_00542F50[16] = "Mahamaon";
const char D_00542F60[16] = "Megidolaon";
const char D_00542F70[16] = "Megidola";
const char D_00542F80[16] = "Mazandyne";
const char D_00542F90[16] = "Maziodyne";
const char D_00542FA0[16] = "Mazionga";
const char D_00542FB0[16] = "Mabufudyne";
const char D_00542FC0[16] = "Mabufula";
const char D_00542FD0[16] = "Bufudyne";
const char D_00542FE0[16] = "Maragidyne";
const char D_00542FF0[16] = "Maragion";
const char D_00543000[24] = "\314\314\314\314\314\314\314\314\314\314\314\314\314\314\314\314\314\314";
const char D_00543018[16] = "Retreated";
const char D_00543028[24] = "Retreating.... Success!";
const char D_00543040[16] = "Retreating....";
const char D_00543050[16] = "Retreating...";
const char D_00543060[16] = "Retreating..";
const char D_00543070[16] = "Retreating.";
const char D_00543080[24] = "New enemy appeared!";
const char D_00543098[16] = "Fell in panic!";
const char D_005430A8[24] = "Reserved";
campWork_t *campWork = NULL;
s8 campExitState = 0;
SDATA_PAD(3);

char cmpTextLoad[8] = "Load";
char cmpTextConfig[8] = "Config";
char cmpTextStatus[8] = "Status";
char cmpTextParty[8] = "Party";
char cmpTextSkill[8] = "Skill";
char cmpTextItem[8] = "Item";
char cmpTextGems[8] = "Gems";
char cmpTextDiscard[8] = "Discard";
char cmpTextUse[8] = "Use";
char cmpTextSummon[8] = "Summon";
char cmpTextUnequip[8] = "Unequip";
char cmpTextEquip[8] = "Equip";
char cmpCalcProcName[8] = "camp";
char cmpFmtNameCount[8] = "%s%d";
char cmpTextGEMS[8] = "GEMS";
char cmpTextITEM[8] = "ITEM";
char D_0054FE58[8] = "PARTY";
char D_0054FE60[8] = "STATUS";
char D_0054FE68[8] = "Expend";
char cmpFmtMaka[4] = "%7d";
campStatusPanel_t *campStatusPanel = NULL;
campInfoWork_t *campInfoWork = NULL;
SDATA_PAD(4);

char campDhFmtAmount[8] = "%2d";
s32 cmpStatTimer = 0;
u16 rstHeartsMasterPhase = 0;
SDATA_PAD(2);

datUnitWork_t *cmbBirthPanel = NULL;
SDATA_PAD(4);

char cmpFmtDec2[8] = "%2d";
char cmpFmtValMax[8] = "%3d";
char cmpFmtExpNext[8] = "%6d";
const char cmpFmtItemQty[8] = "%2d";
char cmpFmtSkillCost[4] = "%3d";
s32 D_0054FEBC = 0;
campStatusPanel_t *cmpStatusPhase = NULL;
s8 cmpStatusWindowHidden = 0;
s8 cmpStatusModelLoadState = 0;
cmpInfoWindow_t *cmpInfoWindow = NULL;
SDATA_PAD(4);

char cmpFmtHpMp[8] = "%3d";
char cmpFmtLevel[4] = "%2d";
cmpPanelEff_t *cmpPanelEffList = NULL;
s32 cmpStatusModelResult = 0;
mdlObj_t *cmpEncycWork = NULL;
void *hCAMPBadEffect = NULL;
s8 cmpStatusModelReady = 0;
s8 cmpModelInitFlag = 0;
u16 cmpStatusGroup = 0;
u16 cmpStatusModelId = 0;
SDATA_PAD(2);

s32 cmpModelHandle = 0;
char cmpTextLuck[8] = "Luck";
char cmpTextAgility[8] = "Agility";
char cmpTextMagic[8] = "Magic";
s32 campStatusActive = 0;
SDATA_PAD(4);

s32 cmbRaceTable = 0x180;
s32 nbEncountMaxBase = 0x172;
char D_0054FF20[8] = "Lucifer";
char D_0054FF28[8] = "Shade";
char D_0054FF30[8] = "Mara";
char D_0054FF38[8] = "Reserve";
char D_0054FF40[8] = "Tharmus";
char D_0054FF48[8] = "Luvah";
char D_0054FF50[8] = "Urizen";
char D_0054FF58[8] = "Urthona";
char D_0054FF60[8] = "Noah";
char D_0054FF68[8] = "Ahriman";
char D_0054FF70[8] = "Specter";
char D_0054FF78[8] = "0xFE";
char D_0054FF80[8] = "0xFD";
char D_0054FF88[8] = "0xFC";
char D_0054FF90[8] = "0xFB";
char D_0054FF98[8] = "0xFA";
char D_0054FFA0[8] = "0xF9";
char D_0054FFA8[8] = "0xF8";
char D_0054FFB0[8] = "0xF7";
char D_0054FFB8[8] = "0xF6";
char D_0054FFC0[8] = "0xF5";
char D_0054FFC8[8] = "0xF4";
char D_0054FFD0[8] = "0xF3";
char D_0054FFD8[8] = "0xF2";
char D_0054FFE0[8] = "0xF1";
char D_0054FFE8[8] = "0xF0";
char D_0054FFF0[8] = "0xEF";
char D_0054FFF8[8] = "0xEE";
char D_00550000[8] = "0xED";
char D_00550008[8] = "0xEC";
char D_00550010[8] = "0xEB";
char D_00550018[8] = "0xEA";
char D_00550020[8] = "0xE9";
char D_00550028[8] = "0xE8";
char D_00550030[8] = "0xE7";
char D_00550038[8] = "0xE6";
char D_00550040[8] = "0xE5";
char D_00550048[8] = "0xE4";
char D_00550050[8] = "0xE3";
char D_00550058[8] = "0xE2";
char D_00550060[8] = "0xE1";
char D_00550068[8] = "0xE0";
char D_00550070[8] = "0xDF";
char D_00550078[8] = "0xDE";
char D_00550080[8] = "0xDD";
char D_00550088[8] = "0xDC";
char D_00550090[8] = "0xDB";
char D_00550098[8] = "0xDA";
char D_005500A0[8] = "0xD9";
char D_005500A8[8] = "0xD8";
char D_005500B0[8] = "0xD7";
char D_005500B8[8] = "0xD6";
char D_005500C0[8] = "0xD5";
char D_005500C8[8] = "0xD4";
char D_005500D0[8] = "0xD3";
char D_005500D8[8] = "0xD2";
char D_005500E0[8] = "0xD1";
char D_005500E8[8] = "0xD0";
char D_005500F0[8] = "Matador";
char D_005500F8[8] = "Dante";
char D_00550100[8] = "0xBF";
char D_00550108[8] = "0xBE";
char D_00550110[8] = "0xBD";
char D_00550118[8] = "0xBC";
char D_00550120[8] = "0xBB";
char D_00550128[8] = "0xBA";
char D_00550130[8] = "0xB9";
char D_00550138[8] = "0xB8";
char D_00550140[8] = "0xB7";
char D_00550148[8] = "0xB6";
char D_00550150[8] = "0xB5";
char D_00550158[8] = "0xB4";
char D_00550160[8] = "0xB3";
char D_00550168[8] = "Phantom";
char D_00550170[8] = "Loa";
char D_00550178[8] = "Atropos";
char D_00550180[8] = "Clotho";
char D_00550188[8] = "Fuu-Ki";
char D_00550190[8] = "Sui-Ki";
char D_00550198[8] = "Kin-Ki";
char D_005501A0[8] = "Kaiwan";
char D_005501A8[8] = "Pisaca";
char D_005501B0[8] = "Samael";
char D_005501B8[8] = "Manikin";
char D_005501C0[8] = "Albion";
char D_005501C8[8] = "Gurr";
char D_005501D0[8] = "Garuda";
char D_005501D8[8] = "Makami";
char D_005501E0[8] = "Barong";
char D_005501E8[8] = "Long";
char D_005501F0[8] = "Hanuman";
char D_005501F8[8] = "Kurama";
char D_00550200[8] = "Ganesha";
char D_00550208[8] = "Uriel";
char D_00550210[8] = "Raphael";
char D_00550218[8] = "Gabriel";
char D_00550220[8] = "Michael";
char D_00550228[8] = "Mou-Ryo";
char D_00550230[8] = "Slime";
char D_00550238[8] = "Blob";
char D_00550240[8] = "Shadow";
char D_00550248[8] = "Preta";
char D_00550250[8] = "Yaka";
char D_00550258[8] = "Legion";
char D_00550260[8] = "Zhen";
char D_00550268[8] = "Bicorn";
char D_00550270[8] = "Nue";
char D_00550278[8] = "Raiju";
char D_00550280[8] = "Mothman";
char D_00550288[8] = "Lilim";
char D_00550290[8] = "Fomor";
char D_00550298[8] = "Incubus";
char D_005502A0[8] = "Nyx";
char D_005502A8[8] = "Lilith";
char D_005502B0[8] = "Loki";
char D_005502B8[8] = "Abaddon";
char D_005502C0[8] = "Surt";
char D_005502C8[8] = "Aciel";
char D_005502D0[8] = "Mot";
char D_005502D8[8] = "Pazuzu";
char D_005502E0[8] = "Tao Tie";
char D_005502E8[8] = "Mada";
char D_005502F0[8] = "Taraka";
char D_005502F8[8] = "Shikome";
char D_00550300[8] = "Yaksini";
char D_00550308[8] = "Dakini";
char D_00550310[8] = "Rangda";
char D_00550318[8] = "Ikusa";
char D_00550320[8] = "Oni";
char D_00550328[8] = "Kodama";
char D_00550330[8] = "Hua Po";
char D_00550338[8] = "Sudama";
char D_00550340[8] = "Titan";
char D_00550348[8] = "Inugami";
char D_00550350[8] = "Sparna";
char D_00550358[8] = "Orthrus";
char D_00550360[8] = "Nozuchi";
char D_00550368[8] = "Naga";
char D_00550370[8] = "Mizuchi";
char D_00550378[8] = "Forneus";
char D_00550380[8] = "Eligor";
char D_00550388[8] = "Berith";
char D_00550390[8] = "Ose";
char D_00550398[8] = "Flauros";
char D_005503A0[8] = "Angel";
char D_005503A8[8] = "Power";
char D_005503B0[8] = "Virtue";
char D_005503B8[8] = "Throne";
char D_005503C0[8] = "Pixie";
char D_005503C8[8] = "Kelpie";
char D_005503D0[8] = "Setanta";
char D_005503D8[8] = "Troll";
char D_005503E0[8] = "Oberon";
char D_005503E8[8] = "Titania";
char D_005503F0[8] = "Koppa";
char D_005503F8[8] = "Apsaras";
char D_00550400[8] = "Isora";
char D_00550408[8] = "Dis";
char D_00550410[8] = "Karasu";
char D_00550418[8] = "Jinn";
char D_00550420[8] = "Onkot";
char D_00550428[8] = "Purski";
char D_00550430[8] = "Efreet";
char D_00550438[8] = "Erthys";
char D_00550440[8] = "Aeros";
char D_00550448[8] = "Aquans";
char D_00550450[8] = "Flaemis";
char D_00550458[8] = "Unicorn";
char D_00550460[8] = "Cai-Zhi";
char D_00550468[8] = "Shiisaa";
char D_00550470[8] = "Senri";
char D_00550478[8] = "Baihu";
char D_00550480[8] = "Chimera";
char D_00550488[8] = "Zouchou";
char D_00550490[8] = "Koumoku";
char D_00550498[8] = "Jikoku";
char D_005504A0[8] = "Thor";
char D_005504A8[8] = "Parvati";
char D_005504B0[8] = "Skadi";
char D_005504B8[8] = "Kali";
char D_005504C0[8] = "Wu Kong";
char D_005504C8[8] = "Shiva";
char D_005504D0[8] = "Uzume";
char D_005504D8[8] = "Sati";
char D_005504E0[8] = "Laksmi";
char D_005504E8[8] = "Horus";
char D_005504F0[8] = "Atavaka";
char D_005504F8[8] = "Odin";
char D_00550500[8] = "Mithra";
char D_00550508[8] = "Vishnu";
char D_00550510[8] = "00";
char D_00550518[8] = "Devil";
char D_00550520[8] = "Fiend";
char D_00550528[8] = "Emperor";
char D_00550530[8] = "Light";
char D_00550538[8] = "Hallel";
char D_00550540[8] = "Zoa";
char D_00550548[8] = "Corpus";
char D_00550550[8] = "Entity";
char D_00550558[8] = "Reaper";
char D_00550560[8] = "Raptor";
char D_00550568[8] = "Avian";
char D_00550570[8] = "Avatar";
char D_00550578[8] = "Dragon";
char D_00550580[8] = "Genma";
char D_00550588[8] = "Wargod";
char D_00550590[8] = "Seraph";
char D_00550598[8] = "Foul";
char D_005505A0[8] = "Haunt";
char D_005505A8[8] = "Wilder";
char D_005505B0[8] = "Night";
char D_005505B8[8] = "Tyrant";
char D_005505C0[8] = "Vile";
char D_005505C8[8] = "Femme";
char D_005505D0[8] = "Brute";
char D_005505D8[8] = "Jirae";
char D_005505E0[8] = "Beast";
char D_005505E8[8] = "Snake";
char D_005505F0[8] = "Fallen";
char D_005505F8[8] = "Divine";
char D_00550600[8] = "Fairy";
char D_00550608[8] = "Yoma";
char D_00550610[8] = "Mitama";
char D_00550618[8] = "Element";
char D_00550620[8] = "Holy";
char D_00550628[8] = "Kishin";
char D_00550630[8] = "Lady";
char D_00550638[8] = "Fury";
char D_00550640[8] = "Megami";
char D_00550648[8] = "Deity";
char D_00550650[8] = "Race";
char D_00550658[8] = "King";
char D_00550660[8] = "Lord";
char D_00550668[8] = "Phenom";
char D_00550670[8] = "Spirit";
char D_00550678[8] = "Slayer";
char D_00550680[8] = "Master";
char D_00550688[8] = "Saint";
char D_00550690[8] = "Battler";
char D_00550698[8] = "Expert";
char D_005506A0[8] = "Zealot";
char D_005506A8[8] = "Soldier";
char D_005506B0[8] = "Adept";
char D_005506B8[8] = "Votary";
char D_005506C0[8] = "Dignity";
char D_005506C8[8] = "Beauty";
char D_005506D0[8] = "Insight";
char D_005506D8[8] = "Wisdom";
char D_005506E0[8] = "Majesty";
char D_005506E8[8] = "Spoon";
char D_005506F0[8] = "Letter";
char D_005506F8[8] = "ID Card";
char D_00550700[8] = "Coral";
char D_00550708[8] = "Onyx";
char D_00550710[8] = "Garnet";
char D_00550718[8] = "Agate";
char D_00550720[8] = "Opal";
char D_00550728[8] = "Jade";
char D_00550730[8] = "Ruby";
char D_00550738[8] = "Emerald";
char D_00550740[8] = "Pearl";
char D_00550748[8] = "Diamond";
char D_00550750[8] = "Kailash";
char D_00550758[8] = "Gaea";
char D_00550760[8] = "Sophia";
char D_00550768[8] = "Gundari";
char D_00550770[8] = "Vimana";
char D_00550778[8] = "Adama";
char D_00550780[8] = "Satan";
char D_00550788[8] = "Gehenna";
char D_00550790[8] = "Muspell";
char D_00550798[8] = "Djed";
char D_005507A0[8] = "Geis";
char D_005507A8[8] = "Nirvana";
char D_005507B0[8] = "Miasma";
char D_005507B8[8] = "Kamudo";
char D_005507C0[8] = "Hifumi";
char D_005507C8[8] = "Ankh";
char D_005507D0[8] = "Soma";
char D_005507D8[8] = "Bead";
char D_005507E0[8] = "0x1FF";
char D_005507E8[8] = "0x1FE";
char D_005507F0[8] = "0x1FD";
char D_005507F8[8] = "0x1FC";
char D_00550800[8] = "0x1FB";
char D_00550808[8] = "0x1FA";
char D_00550810[8] = "0x1F9";
char D_00550818[8] = "0x1F8";
char D_00550820[8] = "0x1F7";
char D_00550828[8] = "0x1F6";
char D_00550830[8] = "0x1F5";
char D_00550838[8] = "0x1F4";
char D_00550840[8] = "0x1EF";
char D_00550848[8] = "0x1EE";
char D_00550850[8] = "0x1ED";
char D_00550858[8] = "0x1EC";
char D_00550860[8] = "0x1EB";
char D_00550868[8] = "0x1EA";
char D_00550870[8] = "0x1E9";
char D_00550878[8] = "0x1E8";
char D_00550880[8] = "0x1E7";
char D_00550888[8] = "0x1E6";
char D_00550890[8] = "0x1E5";
char D_00550898[8] = "0x1E4";
char D_005508A0[8] = "0x1E3";
char D_005508A8[8] = "0x1E2";
char D_005508B0[8] = "0x1E1";
char D_005508B8[8] = "0x1E0";
char D_005508C0[8] = "0x1DF";
char D_005508C8[8] = "0x1DE";
char D_005508D0[8] = "0x1DD";
char D_005508D8[8] = "0x1DC";
char D_005508E0[8] = "0x1DB";
char D_005508E8[8] = "0x1DA";
char D_005508F0[8] = "0x1D9";
char D_005508F8[8] = "0x1D8";
char D_00550900[8] = "0x1D7";
char D_00550908[8] = "0x1D6";
char D_00550910[8] = "0x1D5";
char D_00550918[8] = "0x1D4";
char D_00550920[8] = "0x1D3";
char D_00550928[8] = "0x1D2";
char D_00550930[8] = "0x1D1";
char D_00550938[8] = "0x1D0";
char D_00550940[8] = "0x1CF";
char D_00550948[8] = "0x1CE";
char D_00550950[8] = "0x1CD";
char D_00550958[8] = "0x1CC";
char D_00550960[8] = "0x1CB";
char D_00550968[8] = "0x1CA";
char D_00550970[8] = "0x1C9";
char D_00550978[8] = "0x1C8";
char D_00550980[8] = "0x1C7";
char D_00550988[8] = "0x1C6";
char D_00550990[8] = "0x1C5";
char D_00550998[8] = "0x1C4";
char D_005509A0[8] = "0x1C3";
char D_005509A8[8] = "0x1C2";
char D_005509B0[8] = "0x1C1";
char D_005509B8[8] = "0x1C0";
char D_005509C0[8] = "0x1BF";
char D_005509C8[8] = "0x1BE";
char D_005509D0[8] = "0x1BD";
char D_005509D8[8] = "0x1BC";
char D_005509E0[8] = "0x1BB";
char D_005509E8[8] = "0x1BA";
char D_005509F0[8] = "0x1B9";
char D_005509F8[8] = "0x1B8";
char D_00550A00[8] = "0x1B7";
char D_00550A08[8] = "0x1B6";
char D_00550A10[8] = "0x1B5";
char D_00550A18[8] = "0x1B4";
char D_00550A20[8] = "0x1B3";
char D_00550A28[8] = "0x1B2";
char D_00550A30[8] = "0x1B1";
char D_00550A38[8] = "0x1B0";
char D_00550A40[8] = "0x1AF";
char D_00550A48[8] = "0x1AE";
char D_00550A50[8] = "0x1AD";
char D_00550A58[8] = "0x1AC";
char D_00550A60[8] = "0x1AB";
char D_00550A68[8] = "0x1AA";
char D_00550A70[8] = "0x1A9";
char D_00550A78[8] = "0x1A8";
char D_00550A80[8] = "0x1A7";
char D_00550A88[8] = "0x1A6";
char D_00550A90[8] = "Flatter";
char D_00550A98[8] = "Nag";
char D_00550AA0[8] = "Detain";
char D_00550AA8[8] = "Haggle";
char D_00550AB0[8] = "Loan";
char D_00550AB8[8] = "Trade";
char D_00550AC0[8] = "Begging";
char D_00550AC8[8] = "Pester";
char D_00550AD0[8] = "Beseech";
char D_00550AD8[8] = "Wooing";
char D_00550AE0[8] = "Seduce";
char D_00550AE8[8] = "Kidnap";
char D_00550AF0[8] = "Scout";
char D_00550AF8[8] = "Talk";
char D_00550B00[8] = "0x17F";
char D_00550B08[8] = "0x17E";
char D_00550B10[8] = "0x17D";
char D_00550B18[8] = "0x17C";
char D_00550B20[8] = "0x17B";
char D_00550B28[8] = "0x17A";
char D_00550B30[8] = "0x179";
char D_00550B38[8] = "0x178";
char D_00550B40[8] = "0x177";
char D_00550B48[8] = "0x176";
char D_00550B50[8] = "0x175";
char D_00550B58[8] = "0x174";
char D_00550B60[8] = "0x173";
char D_00550B68[8] = "0x172";
char D_00550B70[8] = "0x171";
char D_00550B78[8] = "0x170";
char D_00550B80[8] = "0x16F";
char D_00550B88[8] = "0x16E";
char D_00550B90[8] = "0x16D";
char D_00550B98[8] = "0x16C";
char D_00550BA0[8] = "0x16B";
char D_00550BA8[8] = "0x16A";
char D_00550BB0[8] = "0x167";
char D_00550BB8[8] = "0x166";
char D_00550BC0[8] = "Pierce";
char D_00550BC8[8] = "Endure";
char D_00550BD0[8] = "Might";
char D_00550BD8[8] = "Preach";
char D_00550BE0[8] = "Startle";
char D_00550BE8[8] = "Stinger";
char D_00550BF0[8] = "Provoke";
char D_00550BF8[8] = "E & I";
char D_00550C00[8] = "Mirage";
char D_00550C08[8] = "Aurora";
char D_00550C10[8] = "Retreat";
char D_00550C18[8] = "Focus";
char D_00550C20[8] = "Summon";
char D_00550C28[8] = "Prayer";
char D_00550C30[8] = "Allure";
char D_00550C38[8] = "Lullaby";
char D_00550C40[8] = "Taunt";
char D_00550C48[8] = "War Cry";
char D_00550C50[8] = "Tornado";
char D_00550C58[8] = "Shock";
char D_00550C60[8] = "Avenge";
char D_00550C68[8] = "Counter";
char D_00550C70[8] = "0x0A2";
char D_00550C78[8] = "0x09D";
char D_00550C80[8] = "0x09C";
char D_00550C88[8] = "Attack";
char D_00550C90[8] = "Blight";
char D_00550C98[8] = "Tempest";
char D_00550CA0[8] = "Berserk";
char D_00550CA8[8] = "Lunge";
char D_00550CB0[8] = "0x05F";
char D_00550CB8[8] = "0x05E";
char D_00550CC0[8] = "0x05D";
char D_00550CC8[8] = "0x05C";
char D_00550CD0[8] = "0x05B";
char D_00550CD8[8] = "0x05A";
char D_00550CE0[8] = "0x059";
char D_00550CE8[8] = "0x058";
char D_00550CF0[8] = "0x057";
char D_00550CF8[8] = "0x056";
char D_00550D00[8] = "0x055";
char D_00550D08[8] = "0x054";
char D_00550D10[8] = "0x053";
char D_00550D18[8] = "0x052";
char D_00550D20[8] = "0x051";
char D_00550D28[8] = "0x050";
char D_00550D30[8] = "0x04E";
char D_00550D38[8] = "Dekunda";
char D_00550D40[8] = "Liftoma";
char D_00550D48[8] = "Estoma";
char D_00550D50[8] = "Trafuri";
char D_00550D58[8] = "Analyze";
char D_00550D60[8] = "Tetraja";
char D_00550D68[8] = "Pulinpa";
char D_00550D70[8] = "Dormina";
char D_00550D78[8] = "Dekaja";
char D_00550D80[8] = "Makajam";
char D_00550D88[8] = "Rakunda";
char D_00550D90[8] = "Sukunda";
char D_00550D98[8] = "Tarunda";
char D_00550DA0[8] = "Recarm";
char D_00550DA8[8] = "Petradi";
char D_00550DB0[8] = "Mutudi";
char D_00550DB8[8] = "Patra";
char D_00550DC0[8] = "Media";
char D_00550DC8[8] = "Diarama";
char D_00550DD0[8] = "Dia";
char D_00550DD8[8] = "Mamudo";
char D_00550DE0[8] = "Mudoon";
char D_00550DE8[8] = "Mudo";
char D_00550DF0[8] = "Mahama";
char D_00550DF8[8] = "Hamaon";
char D_00550E00[8] = "Hama";
char D_00550E08[8] = "Megido";
char D_00550E10[8] = "Mazanma";
char D_00550E18[8] = "Mazan";
char D_00550E20[8] = "Zandyne";
char D_00550E28[8] = "Zanma";
char D_00550E30[8] = "Zan";
char D_00550E38[8] = "Mazio";
char D_00550E40[8] = "Ziodyne";
char D_00550E48[8] = "Zionga";
char D_00550E50[8] = "Zio";
char D_00550E58[8] = "Mabufu";
char D_00550E60[8] = "Bufula";
char D_00550E68[8] = "Bufu";
char D_00550E70[8] = "Maragi";
char D_00550E78[8] = "Agidyne";
char D_00550E80[8] = "Agilao";
char D_00550E88[8] = "Agi";
char D_00550E90[8] = "Failed";
char D_00550E98[8] = "Dismiss";
char D_00550EA0[8] = "Pass";
char D_00550EA8[8] = "Dummy";
