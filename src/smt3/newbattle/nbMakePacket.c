#include "common.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    dds3Basic_t *basic;
} evtUnitHandle_t;
typedef struct effBTLSysHandle_t effBTLSysHandle_t;
typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;

typedef struct
{
    sdfVector4_t orgpos;
    sdfVector4_t orgrot;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 cr;
    f32 scl;
    f32 oneshotr;
    f32 allshotr;
    f32 movey;
    f32 movez;
    s32 type;
    u32 time;
    u32 stat;
    u32 flag;
    u32 anime;
    dds3Basic_t *basic;
    s32 serial;
    evtUnitHandle_t *hunit;
    s16 partyindex;
    s16 formindex;
    u32 autotime;
    effBTLSysHandle_t *badeff;
    u32 modelcol;
    f32 motspeed;
    s16 motendtype;
    s16 motenddata;
    s16 nexthokan;
    s16 nexthokanb;
    s16 waitmot;
    s16 motcate;
    s32 reserve4;
    NB_FILEHANDLE_T *packh;
    char *akey;
    s32 read_minor;
    u32 tartime;
    f32 range;
    f32 addmovet;
    f32 movet;
    sdfVector4_t tarpos;
    sdfVector4_t startpos;
} nbFormation_t;
typedef struct datUnitWork_t
{
    u32 flag;
    u16 id;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    u16 badstatus;
    u32 exp;
    u16 level;
    s8 param[6];
    s8 skillparam[6];
    u16 uniqueid;
    u16 reserve1;
    u16 reserve2;
    u16 namecode[8];
    u16 skillcnt;
    u16 skill[24];
    s16 nowcommand;
    u16 nowindex;
    s16 nowtarea;
    s16 nowtform;
    s16 prevcommand;
    u16 previndex;
    s16 prevtarea;
    s16 prevtform;
    s8 mitamaparam[6];
    s8 levelupparam[6];
    u16 keisyoskill[24];
    u8 keiattr[24];
    u16 hensinmae;
    u16 getdevilhearts[4];
    u32 hensinmaeexp;
    u16 fullnamecode[8];
} datUnitWork_t;
typedef struct nbParty_t
{
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;
typedef struct effBTL_t effBTL_t;
/* A loaded pac file; only the chain down to the decoded buffer is named. */
typedef struct { u8 unk_00[0x8]; void *buf; } nbPacEntry_t;
typedef struct { u8 unk_00[0x60]; nbPacEntry_t *entry; } nbPacHandle_t;
/* 0xE0 bytes; only the field this unit reads is named. */
typedef struct
{
    s16 id;
    u16 unk_02;
    s32 refcnt;
    void *file[26];
    void *buf[26];
    void *motfile;
    void *motbuf;
} nbStatWork_t;
typedef struct effBTLCutHandle_t effBTLCutHandle_t;
/* Only the fields this unit touches are named; the rest of the layout is unconfirmed. */
typedef struct nbMainProcessData_t
{
    nbFormation_t *form;
    u8 unk_04[0xC];
    nbParty_t *party;
    u8 unk_14[0x4];
    s16 playerpcnt;
    s16 enemypcnt;
    u8 unk_1C[0x4];
    nbStatWork_t *statwork;
    void *statbuf;
    s32 unk_28;
    u8 unk_2C[0x8];
    s32 encno;
    u8 unk_38[0x8];
    s32 actionstackp;
    u32 endtype;
    u8 unk_48[0x18];
    u32 stat;
    u8 unk_64[0x4];
    u8 turn;
    u8 unk_69[0xF];
    s16 press4_p;
    s16 pressturn;
    u16 stagemajor;
    u8 unk_7E[0xA];
    s32 eventno;
    s32 eventrid;
    s16 negopformindex;
    s16 negoeformindex;
    s32 negoresult;
    u8 item[4];
    u8 itemcnt[4];
    u32 maka;
    u32 exp;
    u8 unk_A8[0xC];
    nbPacHandle_t *formpack;
    u8 unk_B8[0x4];
    dds3ProcessID_t *mainproc;
    u8 unk_C0[0x1C];
    void *packdata;
    u8 unk_E0[0x18];
    dds3ProcessID_t *negoproc;
    u8 unk_FC[0x4];
    dds3ProcessID_t *debugproc;
    dds3ProcessID_t *aiproc;
    dds3ProcessID_t *pnlproc;
    dds3ProcessID_t *helpproc;
    dds3ProcessID_t *autocommproc;
    dds3ProcessID_t *encproc;
    dds3ProcessID_t *eventproc;
    u8 unk_11C[0x4];
    void *pnltex[4];
    s32 unk_130;
    s32 unk_134;
    nbPacHandle_t *modelpack;
    void *modelpackbuf;
} nbMainProcessData_t;
typedef struct
{
    u32 flag;
    u8 race;
    u8 level;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    s16 aisyoid;
    s8 param[6];
    u16 skill[8];
    s16 keisyotype;
    u16 keisyoform;
    u16 dropmakka;
    u16 dropexp;
    u8 dropitem[2];
    u8 droppoint[2];
    u16 specialbit;
    u8 specialitem;
    u8 specialpoint;
    u8 hougyokupoint;
    u8 masekipoint;
    s8 attackattr;
    u8 attackcnt;
    u8 attackinterval;
    u8 reserve1;
} datDevilFormat_t;
typedef struct
{
    sdfVector4_t center;
    f32 formsize;
    f32 formheight;
    f32 formscale;
    f32 oneshotr;
    f32 allshotr;
    u16 modelsize;
    u16 shadowsize;
    u16 badframe;
    u16 autowait;
    void *motion;
    u32 reserve1;
    u32 reserve2;
    u32 reserve3;
} datUnitVisual_t;
typedef struct _FRQ _FRQ;
/* Only the fields this unit reads are named; x and y are in 1/16 pixels. */
struct _FRQ
{
    u8 unk_00[0x4];
    s32 x;
    s32 y;
};
/* The box nbCenterText centres a string inside; only the width is named. */
typedef struct { u8 unk_00[0xC]; s32 width; } nbTextBox_t;
/* 0x38 bytes of panel flash state; retail never names the fields. */
typedef struct
{
    s32 unk_00;
    s16 unk_04;
    u8 pad_06[0x2];
    s32 unk_08;
    s16 unk_0C;
    u8 pad_0E[0x2];
    f32 unk_10;
    s32 unk_14;
    s16 unk_18;
    u8 pad_1A[0x2];
    f32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    s32 unk_30;
    s32 unk_34;
} nbPanelFlash_t;
/* The nb_help process work; only the fields this unit touches are named. */
typedef struct nbHelpProcessData_t
{
    s32 type;
    char *text;
    char *text2;
    s32 unk_C;
    s32 time;
    u8 unk_14[0x4];
    s32 width;
    s32 width2;
    u32 color;
    s32 meshandle;
    s32 mesid;
    s32 nextmeshandle;
    s32 nextmesid;
    s32 unk_34;
} nbHelpProcessData_t;
/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 unk_04;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 selectbit;
    u8 unk_40[0x28];
    s32 unk_68;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 unk_198[0x8];
    u32 aisummonid;
} nbActionProcessData_t;
typedef struct nbNegoProcessData_t
{
    nbMainProcessData_t *data;
    nbActionProcessData_t *actdata;
    u32 stat;
    s32 negotype;
    s32 waitframe;
    u32 nownegoid;
    s32 nownegophase;
    u32 nextnegoid;
    s32 nextkouhocnt;
    u32 nextkouho[8];
    s32 nowmsgid;
    s32 nowmsgno;
    s32 nowmes;
    s32 msgwndid;
    u32 msgwndcnt;
    s32 nowselect;
    u32 workid[8];
    s32 requesttype;
    s32 requestdata;
    s32 requesthpmp;
    s32 prevreqtype;
    s32 cnt6_36;
    s32 cnt6_37;
    s32 cnt6_58;
    s32 cnt6_66;
    s32 mes6_2;
    s32 mes6_25;
    s32 mes6_67;
    s32 flag1_6;
    s32 flag1_39;
    s32 historyindex;
    u32 history[14];
    s32 historymes[14];
    s32 endid;
    s32 endtype;
    s32 negoreqtype;
    u32 negoskillid;
    s32 aisyo;
    s32 aisyomes;
    s32 hojopartyindex;
    s32 hojoskillid;
    s32 hojoseikou;
    s32 hojoaisyo;
    s32 hojoaisyomes;
    s32 qtype;
    s32 qindex;
    datUnitWork_t *pwork;
    datUnitWork_t *ework;
    s32 pformindex;
    s32 eformindex;
    nbFormation_t *pform;
    nbFormation_t *eform;
    s32 ppartyindex;
    s32 epartyindex;
    nbParty_t *pparty;
    nbParty_t *eparty;
    s32 pkucyo;
    s32 ekucyo;
    s32 innen_id;
    s32 temp_param[4];
    s32 boupartyindex;
    s32 getmakka;
    s32 getitem;
    s32 getinfotype;
    s32 getinfomsg;
    s32 flowmsg[31];
    s32 questionmsg;
    s32 aisyomsg[31];
    s32 infomsg[31];
    s32 helpmsg;
    s32 helpmsg2;
    s32 innenmsg;
    s32 bossmsg;
    s32 selectcur;
} nbNegoProcessData_t;

typedef union
{
    s8 c[4];
    s16 s[2];
    s32 i;
    f32 f;
    void *p;
} nbKoukaData_u;

typedef struct nbKoukaPacket_t
{
    s16 startframe;
    s16 framelength;
    s8 type;
    u8 flag;
    u16 time;
    s32 uniqueid;
    struct nbKoukaPacket_t *next;
    struct nbKoukaPacket_t *prev;
    nbFormation_t *form;
    datUnitWork_t *unit;
    nbParty_t *party;
    s32 partyno;
    effBTLCutHandle_t *cuteff;
    nbKoukaData_u data1;
    nbKoukaData_u data2;
    sdfVector4_t v;
} nbKoukaPacket_t;

typedef struct nbKoukaProcessData_t
{
    nbMainProcessData_t *data;
    s32 packetcnt;
    nbKoukaPacket_t *first;
    nbKoukaPacket_t *last;
} nbKoukaProcessData_t;

typedef struct effPCPBossBUNSINParam_t
{
    nbFormation_t *pStartForm;
    nbFormation_t *pEndForm;
} effPCPBossBUNSINParam_t;

typedef struct effPCPBossBUNSIN2Param_t
{
    nbFormation_t *pStartForm;
    nbFormation_t *pForm1;
    nbFormation_t *pForm2L;
    nbFormation_t *pForm2R;
} effPCPBossBUNSIN2Param_t;

typedef struct effPCPBossONGYParam_t
{
    nbFormation_t *pFormation;
} effPCPBossONGYParam_t;

typedef struct effPCPBossONGY2Param_t
{
    nbFormation_t *pFormation;
} effPCPBossONGY2Param_t;

typedef struct
{
    u8 flag;
    u8 use;
    u8 koukatype;
    u8 costtype;
    u16 cost;
    u16 costbase;
    u8 targettype;
    u8 targetarea;
    u8 targetrule;
    u8 targetrandom;
    u16 untargetbadstat;
    u16 targetprog;
    u8 hittype;
    u8 hitlevel;
    u16 hitprog;
    u8 targetcntmin;
    u8 targetcntmax;
    u16 hptype;
    s16 hpn;
    u16 mptype;
    s16 mpn;
    s16 hpbase;
    s16 mpbase;
    u16 minus;
    u8 badtype;
    u8 badlevel;
    u16 basstatus;
    u32 hojotype;
    s8 hojopoint;
    u8 deadtype;
    u32 program;
    s16 criticalpoint;
    s16 failpoint;
    s16 magicbase;
    s16 magiclimit;
} datNormalSkill_t;

typedef struct
{
    s8 skillattr;
    s8 type;
    s16 index;
    u16 keisyoform;
    s8 capacity;
    u8 flag;
} datSkill_t;

/* Only the fields this unit touches are named; the entry is 38 bytes. */
typedef struct
{
    s8 unk_00;
    s8 esc;
    u8 unk_02[0x4];
    u16 id[11];
    u8 unk_1C[0x8];
    u16 unk_24;
} nbEncountForm_t;

typedef struct
{
    u8 motion;
    u8 animetype;
    u8 hatudo;
    u8 bedno;
} datNormalSkillVisual_t;

typedef struct
{
    datNormalSkill_t normal[288];
    u8 reserve[8];
    datNormalSkillVisual_t visual[288];
} datSkillTable_t;

extern char D_00544890[]; /* "カット長 = %d\n" */
extern char D_00544900[]; /* "length = %d\n" */

extern nbKoukaProcessData_t *nbGetKoukaProcessData(void);
extern nbKoukaPacket_t *nbAddKoukaPacket(nbKoukaProcessData_t *k, s32 startframe, s32 framelength,
                                         s32 uniqueid);
extern effBTL_t *effBTLCreate(u8 *bed);
extern effBTLCutHandle_t *effBTLCutCreate(effBTL_t *hbtl, u16 cutno, nbFormation_t *form, s32 flag);
extern s32 effBTLGetCutEndFrame(effBTL_t *hbtl, u16 cutno);
extern s32 printf(const char *format, ...);
extern nbKoukaPacket_t *nbAddSePacket(s32 startframe, s32 uniqueid, s32 seid);
extern s32 nbCheckSePacket(s32 startframe, s32 seid, s32 range);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern void *effBTLSystemCreate(nbFormation_t *form, u16 no);
extern s32 effBTLSystemGetLength(void *hsys);
extern s32 effPCPBossBUNSINGetLength(void);
extern void *effPCPBossBUNSINCreate(effPCPBossBUNSINParam_t *param);
extern s32 effPCPBossBUNSIN2GetLength(void);
extern void *effPCPBossBUNSIN2Create(effPCPBossBUNSIN2Param_t *param);
extern s32 effPCPBossONGYGetLength(void);
extern void *effPCPBossONGYCreate(effPCPBossONGYParam_t *param);
extern nbMainProcessData_t *nbGetMainProcessData(void);
extern u32 nbGetBadStat(s32 formindex);
extern s32 nbCheckEventDead(nbFormation_t *form);
extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);
extern s32 effPCPBossONGY2GetLength(void);
extern void *effPCPBossONGY2Create(effPCPBossONGY2Param_t *param);
extern void datAddHp(datUnitWork_t *unit, s32 n);
extern void datAddMp(datUnitWork_t *unit, s32 n);
extern void datSetBadStat(datUnitWork_t *unit, s32 badstat);
extern void datResetBadStat(datUnitWork_t *unit, s32 badstat);
extern s32 datGet3Zokusei(datUnitWork_t *unit);
extern datSkillTable_t datSkillTable;
extern datSkill_t attr[];
extern nbEncountForm_t nbEncountFormTable[];
extern nbParty_t *nbGetPartyFromFormindex(s32 formindex);
extern s32 nbGetButuriAttack(s32 nskill, s32 sformindex, s32 dformindex, s32 waza);
extern s32 datCheckSyojiSkill(datUnitWork_t *unit, s32 skill);
extern s32 evtGetAgeOfMoon(void);
extern s32 dds3GetRandIntA(s32 min, s32 max);
extern s32 nbCheckSkillCost(s32 formindex, s32 nskill);
extern void itfMesMngSetVar(s32 msgid, s32 type, s32 value, s32 digit);
extern void itfMesMngMsgWndCls(s32 msgwndid);
extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *unit);
extern datDevilFormat_t datDevilTable[];
/* 164-byte entries indexed by devil id; only the field this unit reads is named. */
typedef struct { u8 unk_00[0x2]; u16 ailevel; u8 unk_04[0xA0]; } datDevilAI_t;
extern datDevilAI_t D_003989E0[];
extern u32 nbGetAisyo(s32 nskill, s32 dformindex, s32 skillattr);
extern void *sdfTexSet(void *header);
extern void sdfTexFreeDelayed(void *texture);
extern void *pnbPnlFile1;
extern void *pnbPnlFile2;
extern void *pnbPnlFile3;
extern void *pnbPnlFile4;
extern s32 gnbPnlOtOfs;
extern s32 nbPnlDrawFlag;
extern f32 nbPnlCnt;
extern u32 nbPanelBaseABGR;
extern u32 nbPanelBaseRGBA;
extern s32 nbPanelPGPress;
extern s32 nbPanelPGNumber;
extern s32 nbPanelPGTurn;
extern s32 nbPanelPGTurnOld;
extern s32 nbPanelPGFrame;
extern s32 nbPanelPGBirthWait;
extern u32 nbPanelPGBaseColor;
extern s8 nbPanelPGShow;
extern s8 nbPanelPGStart;
extern s8 nbPanelPGEncount;
extern s32 nbPanelPT_1_Process;
extern s32 nbPanelPT_2_Process;
extern s32 nbPanelPT_3_Process;
extern s32 nbPanelPTP_1_Process;
extern s32 nbPanelPTAProcess;
extern s32 nbPanelPTAFrame;
extern f32 nbPanelPTARate_S;
extern f32 nbPanelPTARate_E;
extern s8 nbPanelPTAOnly;
extern s32 nbPanelPT_1_Frame;
extern s32 nbPanelPT_2_Frame;
extern f32 nbPanelPT_2_Rate;
extern s32 nbPanelPT_3_Frame;
extern f32 nbPanelPT_3_Rate;
extern s32 nbPanelPTP_0_Frame;
extern s32 nbPanelPTP_0_Process;
extern s32 nbPanelPTP_1_Frame;
extern char D_005453B8[];
extern s8 nbPanelPTOmit;
extern s32 nbPanelStockShiftX;
extern s32 nbPanelStockFrame;
extern u32 nbPanelStockABGR;
extern s8 nbPanelStockRun;
extern s8 nbPanelStockPad;
extern s8 nbPanelStockFadeFlag;
extern s32 nbPanelAnalyzeProcess;
extern s32 nbPanelAnalyzeFrame;
extern datUnitWork_t *pNbPanelAnalyzeUnitWork;
extern u32 PanelAutoABGR;
extern u32 PanelAutoBaseColor;
extern s8 PanelAutoShow;
extern s8 PanelPartyShow;
extern f32 nbFormEnemyInterval;
extern void nbPanelPressGaugeShow(void);
extern void nbPanelPressGaugeHide(void);
extern void nbPanelPartyShow(void);
extern void nbPanelPartyHide(void);
extern void nbPanelAutoShow(void);
extern void nbPanelAutoHide(void);
extern nbActionProcessData_t *scrGetData(void);
extern s32 scrGetIntPara(s32 no);
extern void scrSetIntReturnValue(s32 value);
extern void nbSetNormalSkill0(datUnitWork_t *work);
extern void nbSetActionCommand(nbMainProcessData_t *data, s32 command);
extern s32 nbSetAiTarget(nbActionProcessData_t *a, s32 type, s32 para);
extern s32 nbGetSelectBit(nbMainProcessData_t *data);
extern s32 nbAiChkJoken(nbActionProcessData_t *a, u32 joken);
extern s32 nbGetHojoCounter(s32 formindex, s32 hojotype);
extern s32 nbGetAisyoTable(s32 formindex);
extern s32 evtGetAgeOfMoon16(void);
extern s32 nbGetEscapeSwitch(nbMainProcessData_t *data);
extern void nbRunAi(nbActionProcessData_t *a);
extern void nbRunAiSub(nbActionProcessData_t *a, s32 no);
extern void *memset(void *dst, s32 c, u32 n);
extern f32 scrGetFloatPara(s32 no);
extern void nbCameraScriptSet(sdfVector4_t *pos, sdfVector4_t *rot);
extern void nbCameraScriptSet1Shot(nbFormation_t *form);
extern void nbCameraScriptSetBougai(nbFormation_t *form);
extern void nbPanelAllHide(void);
extern void nbAllFormDispOn(void);
extern void nbGroupFormDispOn(s32 group);
extern void nbGroupFormDispOff(s32 group);
extern s32 btl_ai_rot_cnt;
extern dds3ProcessID_t *scrStartScript2(s32 handle, u8 *script, s32 arg);
extern void scrSetUserData(dds3ProcessID_t *id, void *data);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);
extern dds3ProcessID_t *dds3SearchProcessName(const char *name);
extern u8 scrEmbedded_4F0800[];
extern char D_005514F0[];
extern void nbHelpOff(void);
extern s32 itfMesMngGetWidth(s32 msgid, s32 index, s32 mode);
extern _FRQ *frwGetSmallFontFRQ(s32 x, s32 y, u32 col1, u32 col2, char *str, s32 mode);
extern _FRQ *frGetCNameFRQ(s8 no, s32 style, s32 colorno, s32 tp);
extern s32 frGetStrWidth2(_FRQ *frq);
extern _FRQ *frDestroyFontRequestAfter(_FRQ *frq);
extern _FRQ *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 mode, u32 col, char **str, s32 flag);
extern void func_001C7E38(_FRQ *frq, s32 mode, s32 kind);
extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);
/* A panel rectangle: position, then width and height. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 h;
} nbPanelXYZ_t;
typedef struct {
    s32 u;
    s32 v;
    s32 uw;
    s32 vh;
} nbPanelUV_t;
extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba,
                        sdfTexHandle_t *th);
extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                           s32 ztst);
extern void nbPnl_rsetAlpha(s32 mode);
extern char D_00545728[];
extern void *nbDebugProcess1(dds3ProcessID_t *id);
extern s32 datGetPlayerLNDLevel(void *work);
extern u8 *DDS3_GBWK;
extern char *datExplainTable[];
extern char *text1[];
extern char *text2[];
extern char *pStr[];
extern char *D_005263F8[];
extern void nbDispText(char *text, char *text2, s32 type, s32 time, u32 color);
extern void nbDispMsgHelp(s32 meshandle, s32 mesid, s32 type);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void itfMesMngRequestMessageRelease(s32 handle, s32 mode);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, void *processfunc,
                                         void *shutdownfunc, void *data);
extern s32 nbHelpProcess1(dds3ProcessID_t *id);
extern char D_005458A0[];
extern s32 nbPanelAutoCheckShow(void);
extern void nbPlaySe(s32 no);
extern u32 dds3DebugSign;
extern u8 dds3PadButtonState[];
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);
extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, u32 flag);
extern void nbFormDispOff(s32 formindex);
extern void nbFormDispOn(s32 formindex);
extern void dds3SetPosPoseSet(dds3Basic_t *basic, sdfVector4_t *pos);
extern void dds3SetRotPoseSet(dds3Basic_t *basic, f32 *rot);
extern void Mult(void);
extern f32 D_005458B0[4];
extern f32 D_00575440[4];
extern f32 D_005514F8;
extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);
extern void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot);
extern void nbResetFormPosRot(nbFormation_t *form);
extern void nbSetPartyFormation(s32 party, s32 flag);
extern void nbCopyFormation(void *data, s32 from, s32 to);
extern void nbFormatFormation(nbMainProcessData_t *data, s32 index);
extern void SetBossDummy(void);
extern void *TestProcess1(void);
extern void *TestProcess2(void);
extern s32 dds3SearchProcessPriority(s32 priority);
extern void nbExit(void);
extern void *nbEncountCalc(s32 packno, f32 dist);
extern void nbCallNewBattle(s32 packno, void *enc);
extern s32 pack;
extern char D_00545B90[];
extern void dds3KillProcessName(char *namestr, s32 mode);
extern s32 nbEncountMax;
extern s32 func_0028CFC8(s32 tableno, s32 encno);
extern s32 nbGetUnitID(datUnitWork_t *unit);
extern void p2sdPlaySE(s32 seno, s32 vol, s32 pan);
extern s32 nbLoadMotionByIndex(s32 unitid, s32 index);
extern s32 nbResultCount;
extern s32 bgmno;
extern void p2sdPlayBGM(s32 no);
extern s32 nbBgmSaved;
extern void p2sdSetBgmTrack(s32 track);
extern s32 nbBgmFadeState;
extern void p2sdStopBGM(s32 no);
extern void func_002C1E70(s32 no);
extern s32 nbGetInfoMsg(s32 type);
extern void nbSetMesItem(s32 msgid, s32 it);
typedef struct { u16 mes; u8 pad[0x3A]; } nbInnenEntry_t;
extern nbInnenEntry_t D_003E315E[];
typedef struct { s32 unk_00; u8 pad[0x64]; } nbNegoSkillEntry_t;
extern nbNegoSkillEntry_t D_003E1338[];
typedef struct { u8 rate; u8 item; } nbMasterTextKouho_t;
typedef struct {
    u8 unk_00[0x9];
    u8 unk_09;
    u8 unk_0A[0xC];
    nbMasterTextKouho_t itema[4];
    nbMasterTextKouho_t itemb[4];
    nbMasterTextKouho_t stone[4];
    u8 unk_2E[0x2];
} nbMasterTextEntry_t;
extern nbMasterTextEntry_t master_text[];
extern void nbCameraSetTarget(nbFormation_t *form);
extern s32 nbGetKureItemA(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework);
extern s32 datGetPlayerLevel(void);
extern s32 nbGetLvAvg(nbMainProcessData_t *data, s32 type);
extern s32 datGetParam(datUnitWork_t *unit, s32 param);
extern void mdlPackDecode(s32 group, s32 id, s32 mode, void *data, s32 size);
extern void nbSetEnemyFormation(nbMainProcessData_t *data);
extern void func_0024D938(void *e);
extern u32 dds3DrawFlag;
extern void nbPanelPressGaugeStart(void);
extern void nbPanelPressTurnGaugeAnimStart2(void);
extern void nbPanelPressTurnOmit(void);
extern char D_00546820[];
extern char D_00546880[];
extern char D_00545380[];
extern char D_00545CE0[];
extern char D_00551560[];
/* 28-byte entries; only the fields this unit reads are named. */
typedef struct {
    s32 rid;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    u16 fadetime;
    u16 fadeoutframe;
    u16 fadestopframe;
    u16 fadeinframe;
} nbEventEntry_t;
extern nbEventEntry_t nbEventEntryTable[];
/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} nbColorWork_t;
typedef struct { s32 step; s32 weight; } nbBadCurseEntry_t;
extern nbBadCurseEntry_t nbBadCurseTable[];
extern s32 nbBadCurseTimer;
/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 unk_08;
    s32 step;
    s32 frame;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *unk_1C;
    nbPacHandle_t *unk_20;
    void *unk_24;
} nbEventProcessData_t;
extern void nbFreeEffectSlot(nbEventProcessData_t *e);
typedef struct {
    s32 unk_00;
    s32 maka;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    u8 unk_14[0x8];
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
} nbMakaWork_t;
extern nbMakaWork_t *GetMakaProcessData(void);
extern void nbSetJyokyoMes(nbActionProcessData_t *a, s32 mes, s32 type, s32 id);
extern s32 ChkAiHojo(s32 formindex, s32 hojotype);
extern s32 nbFindStockSkillError(void);
extern nbFormation_t *nbGetEventForm(nbEventProcessData_t *e);
extern nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);
extern void nbSetTargetProcess(s32 a, s32 b, s32 c, s32 d, s32 e);
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(s32 a, s32 b, s32 c, s32 d);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, u32 stat);
extern u8 datModelParamTable[];
extern u8 D_003494B0[];
/* 60-byte entries; only the fields this unit reads are named. */
typedef struct {
    s16 id;
    u16 pid;
    u16 eid;
    u16 unk_06;
    struct { u16 item; u16 pad; } phase[13];
} nbInnenRec_t;
extern nbInnenRec_t D_003E3158[];
extern s32 datGetRace(datUnitWork_t *unit);
extern s32 datSearchDevilStock(s32 id);
extern void nbCameraSetBougai(nbFormation_t *form);
extern s32 nbSearchHojoSkillWork(nbNegoProcessData_t *n, s32 type);
extern char D_005450E0[];
/* 420-byte entries; only the field this unit reads is named. */
typedef struct { u8 unk_00[0x4]; u16 unk_04; u8 pad[0x1A2]; } nbEncountEntry_t;
extern nbEncountEntry_t nbEncountTable[];
extern nbKoukaPacket_t *nbAddSePacket2(s32 startframe, s32 uniqueid, s32 seid);
extern void effBTLSkillEnd(void);
extern s32 evtBitCheck(s32 bit);
extern void evtBitOn(s32 bit);
extern u8 datNegoReqTable[];
extern u8 datNegoTuikaReqtype[];
extern void nbLoadUnitGroup(s32 a, s32 b, s32 c);
extern void nbFreeEventEntry(nbMainProcessData_t *data);
extern s32 nbFindBadCurseTarget(nbActionProcessData_t *a, s32 step);
extern void nbSetTargetSkill(nbActionProcessData_t *a, datUnitWork_t *work, s32 *ctype,
                             s32 *crule, s32 *carea, s32 mode);
extern s32 nbSetSelectOkMark(s32 partyindex, s32 ctype, s32 crule, s32 carea);
extern s32 nbBadCurseApplyMove(nbActionProcessData_t *a, s32 mode);
extern void fileSyncAll(void);
extern s32 evtGetEventPackStat(s32 rid);
extern u128 D_00526420;
extern u128 D_00526430;
extern nbNegoProcessData_t *InitNegoProcessData(nbActionProcessData_t *a, s32 pformindex,
                                                s32 eformindex, s32 negotype, s32 skillid);
extern void nbSetNegoWaitMotion(nbFormation_t *form);
extern void nbSetAllHojoSkillWork(nbNegoProcessData_t *n);
extern void p2sdBgmVolDown(void);
extern void *nbNegoProcess1(dds3ProcessID_t *id);
extern void nbNegoDebugPrint(nbNegoProcessData_t *n);
extern s32 nbCheckNegoMessage(nbNegoProcessData_t *n);
extern char D_00544F10[];
extern u32 hnbPnlFile1;
extern u32 hnbPnlFile2;
extern u32 hnbPnlFile3;
extern u32 hnbPnlFile4;
extern char D_00545230[];
extern char D_00545250[];
extern char D_00545270[];
extern char D_00545290[];
extern u32 sdfDevLoadFileEx(char *path, void *dest, void *size);
extern char D_00545DC8[];
extern void *nbEventProcess1(dds3ProcessID_t *id);
extern void nbEventShutdown(dds3ProcessID_t *id);
extern void nbNegoShutdown(dds3ProcessID_t *id);
extern char D_00551160[];
extern void *nbEncProcess2_5(dds3ProcessID_t *id);
extern s32 nbMakaProcess1(dds3ProcessID_t *id);
extern void nbMakaShutdown(dds3ProcessID_t *id);
extern nbMakaWork_t *nbAllocMakaWork(void);
extern s32 nbGetNegoKureMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);
extern void nbFreeFormResources(nbMainProcessData_t *data, s32 index);
extern void *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(void *p);
extern s32 p2sdLoadCheckMidi(s32 no);
extern void p2sdLoadSMGFile(s32 no);
extern void evtSetUnitMotion(evtUnitHandle_t *hunit, s32 motion, s32 a, s32 b, s32 c);
extern void nbSetMaeFlag(s32 flag);
extern void nbMakaStep(nbMakaWork_t *w);
extern void nbDrawMakaNum(nbMakaWork_t *w, s32 x, s32 y, u32 col, s32 n);
extern u32 menucur;
extern s32 (*D_00527518[])(nbEventProcessData_t *e);
extern void *nbEncProcess1(dds3ProcessID_t *id);
extern void *nbEventProcess3(dds3ProcessID_t *id);
extern void effBTLEncountStart(void);
extern s32 dds3GetKernelCount(void);
extern s32 readbasetime;
extern s32 readtime;
extern char D_00545DB0[];
extern void nbEncShutdown(dds3ProcessID_t *id);
extern char D_00551550[];
extern char D_00546500[];
extern char D_005467E0[];
extern s32 nbFindFormIndex(s32 id);
/* 0x30 bytes; only the fields this unit touches are named. */
typedef struct {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    nbFormation_t *form;
    s32 partyindex;
    nbParty_t *party;
    s32 hp;
    s32 mp;
    u32 bad;
} nbDebugWork_t;
extern void SetDvlChange(nbDebugWork_t *d);
extern char D_005464A8[];
extern char D_005464C0[];
extern s32 nbCheckLoadGroup(s32 group);
extern void nbDelAllKoukaPacketType(s32 type);
extern s32 nbMaeCheckDone(void);
extern void nbMaeWaitStep(void);
extern void MoveSub(nbEventProcessData_t *e);
extern void pacFileHandleFree_(void *h);
extern s32 pacLoadUseTail(char *name);
extern void sdfMemFreeDirect(void *p);
extern s32 sprintf(char *buf, const char *format, ...);
extern s32 nbMakeModelPackName(s32 no);
extern char D_00546548[];
extern char D_00546860[];
extern char D_00546898[];
extern char D_005468B0[];
extern char D_005468C8[];
extern char D_005468D8[];
extern char D_00546570[];
extern char D_005467F0[];
extern char D_00546830[];
extern char D_00546870[];
extern nbInnenEntry_t D_003E317C[];
extern nbInnenEntry_t D_003E317E[];
extern s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n);
extern s32 nbGetQuestionType(datUnitWork_t *unit);
extern s32 datCheckTalk(datUnitWork_t *unit);
extern s32 datCheckStockFull(void);
extern s32 datCheckItem(s32 item, s32 cnt);
extern void nbPanelDraw(s32 show);
extern void nbMakaDispOn(s32 no);
extern s32 datCheckDying(datUnitWork_t *unit);
extern nbKoukaPacket_t *nbMakeKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                            s32 type, s32 hp, s32 mp, s32 bad);
extern s32 nbCheckFlowUnit(nbNegoProcessData_t *n);
extern s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework);
extern char D_00545208[]; /* "pwork hp = %d\n" */
extern void nbSetHojoCounter(s32 formindex, s32 hojotype, s32 n);
extern s32 ChkAiHojoMax(s32 formindex, s32 hojotype);
extern u32 datGetAisyo(datUnitWork_t *unit, s32 type);
extern s32 nbGetHojoCounterMax(s32 hojotype);
extern s32 nbGetHojoCounterMin(s32 hojotype);
extern void nbResetAllFormPosRot(void);
extern void dds3DeleteSceneBasicData(void *basic);
extern void *bedbasic;
extern u8 pNbCamera[];
extern void nbCameraBoss_Sp_DOUTYO_Run(u8 *camera);
extern void nbCameraBoss_Ongy_BUNSIN_Run(u8 *camera);
extern void *evtGetResorceData(s32 id, s32 type);
extern void DispHpMpBad2(nbDebugWork_t *d, nbFormation_t *form);
extern u8 datNegoReqItemListA[];
extern u8 datNegoReqItemListB[];
extern u8 D_003E2318[];
extern u8 D_003E23F8[];
extern s32 nbSelectKureItemA(s32 group);
extern s32 nbSelectKureItemB(s32 group);
extern void nbSetWaitMotion(nbFormation_t *form, s32 mode, f32 speed);
extern void nbSingleFormExceptedDispOff(nbFormation_t *form);
extern void nbBgmStop(void);
extern s32 nbFindFreeFormSlot(void);
extern u16 D_003E7028[];
extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);
extern s32 (*D_00527688[])(nbEventProcessData_t *e);
extern void nbCameraSetState(s32 state);
extern void nbCameraBoss_Foru_E913_Set(void);
extern void nbCameraBoss_Foru_E913_Run(void);
extern void *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern u64 *sdf2DMakeStdPacket(void *buff, s32 size);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, void *packet);
extern sdfGrOt_t sdfGrOtTable[];
extern void frSetDrawPosition(_FRQ *frq, s32 x, s32 y);
extern void *memcpy(void *dst, const void *src, u32 n);
extern s32 pacCheckEnd(nbPacHandle_t *h);
extern void nbAddFormEntry(void *buf, s32 group, s32 id);
extern dds3ProcessID_t *evtLoadEventPack(s32 rid);
extern s32 nbFlowGetUnitEntry(datUnitWork_t *a, datUnitWork_t *b, s32 c, s32 d);
extern void *fileGetMemHandle(void *h);
extern void fileHandleFree(void *h);
typedef struct { s16 jouken[3]; u8 pad[0xC6]; } nbInfoSelectEntry_t;
extern nbInfoSelectEntry_t InfoSelectTable[];
extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);
extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 mes, s32 type, s32 wait);
extern s32 (*nbE902Table[])(nbEventProcessData_t *e);
extern void dds3PadVibrationSet(s32 port, s32 power, s32 time);
extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 speed);
extern s32 mdlPrepare(s32 group, s32 id, s32 blocking);
extern void nbLoadMidiFile(s32 no);
extern u32 sdfMemGetSize(void *p);
extern nbKoukaPacket_t *nbAddFadeInKoukaPacket(s32 startframe, s32 uniqueid, s32 length,
                                               s32 coltype);
extern s32 datCheckItemFull(s32 item);
// @@DECLS@@
extern void datAddMaka(s32 maka);
extern void datAddItem(s32 item, s32 cnt);
extern s32 nbSetNegoYesMotion(nbNegoProcessData_t *n, nbFormation_t *form);
extern s32 nbSetNegoNoMotion(nbNegoProcessData_t *n, nbFormation_t *form);
extern s32 (*D_004EF4B0[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF550[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF698[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF6F8[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF780[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF7C8[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF7D8[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF810[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF868[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF8D8[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF928[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF950[])(nbNegoProcessData_t *n);
extern s32 (*D_004EF960[])(nbNegoProcessData_t *n);
extern s32 (*D_004EFA28[])(nbNegoProcessData_t *n);
extern s32 (*D_004EFA60[])(nbNegoProcessData_t *n);
extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket(s32 startframe, s32 framelength, s32 uniqueid,
                                             effBTLCutHandle_t *cuteff)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, framelength, uniqueid);
    p->type = 0;
    p->cuteff = cuteff;
    return p;
}

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket3(s32 startframe, s32 uniqueid, u8 *bed, s32 cutno,
                                              nbFormation_t *form)
{
    effBTL_t *hbtl;
    effBTLCutHandle_t *cut;
    s32 endframe;
    nbKoukaPacket_t *p;

    hbtl = effBTLCreate(bed);
    cut = effBTLCutCreate(hbtl, cutno, form, 1);
    endframe = effBTLGetCutEndFrame(hbtl, cutno);
    printf(D_00544890, endframe);
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, endframe, uniqueid);
    p->type = 43;
    p->data1.p = hbtl;
    p->form = form;
    p->cuteff = cut;
    return p;
}

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket2(s32 startframe, s32 framelength, s32 uniqueid, effBTL_t *hbtl,
                                  s32 cutno, nbFormation_t *form, s32 j)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, framelength, uniqueid);
    if (framelength == 0) {
        p->flag |= 1;
    }
    p->type = 21;
    p->cuteff = NULL;
    p->data1.p = hbtl;
    p->data2.s[0] = cutno;
    p->data2.s[1] = j;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket1(s32 startframe, s32 framelength, s32 uniqueid,
                                  effBTLCutHandle_t *cuteff)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, framelength, uniqueid);
    p->type = 26;
    p->cuteff = cuteff;
    return p;
}

void nbAddHatudoEffectEndPacket(s32 startframe, s32 uniqueid)
{
    nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid)->type = 58;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544890);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAddHatudoEffectKoukaPacket);

nbKoukaPacket_t *nbAddTargetMoveKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno,
                                            sdfVector4_t *pos, f32 speed)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 1;
    p->partyno = partyno;
    SDF_VU0_COPYVECTOR(&p->v, pos);
    p->data1.f = speed;
    return p;
}

nbKoukaPacket_t *nbAddHomeMoveKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno, f32 speed)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 2;
    p->partyno = partyno;
    p->data1.f = speed;
    return p;
}

nbKoukaPacket_t *nbAddRotKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno, sdfVector4_t *rot,
                                     s32 speed)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 3;
    p->partyno = partyno;
    SDF_VU0_COPYVECTOR(&p->v, rot);
    p->data1.i = speed;
    return p;
}

nbKoukaPacket_t *nbAddHomeRotKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno, s32 speed)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 4;
    p->partyno = partyno;
    p->data1.i = speed;
    return p;
}

nbKoukaPacket_t *nbAddUnitColKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 col,
                                         s32 frame)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 5;
    p->form = form;
    p->data1.i = frame;
    p->data2.i = col;
    return p;
}

nbKoukaPacket_t *nbAddUnitHomeColKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                             s32 frame)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 6;
    p->form = form;
    p->data1.i = frame;
    return p;
}

nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 a,
                                       s32 frame)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 7;
    p->form = form;
    p->data1.i = frame;
    p->data2.i = a;
    return p;
}

nbKoukaPacket_t *nbAddUnitHomeAKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                           s32 frame)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 8;
    p->form = form;
    p->data1.i = frame;
    return p;
}

nbKoukaPacket_t *nbAddSakeKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;
    f32 r;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    r = form->r * 0.9f;
    if (r > 100.0f) {
        r = 100.0f;
    }
    p->form = form;
    p->data1.f = r;
    p->data2.f = 0.3f;
    p->type = 13;
    nbAddSePacket(startframe, uniqueid, 0x10010);
    return p;
}

nbKoukaPacket_t *nbAddMotionStopPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 32;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddMotionStopClearPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 33;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                        s32 motion)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 27;
    p->form = form;
    p->data1.i = motion;
    return p;
}

nbKoukaPacket_t *nbAddSkillMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                             s32 motion, s32 endtype, f32 ritu)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 55;
    p->form = form;
    p->data1.s[0] = motion;
    p->data1.s[1] = endtype;
    p->data2.f = ritu;
    return p;
}

nbKoukaPacket_t *nbAddDmgMotKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                        f32 range, s32 week)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 9;
    p->form = form;
    p->data1.f = range;
    p->data2.i = week;
    return p;
}

nbKoukaPacket_t *nbAddEasyDmgMotKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                            f32 range)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 53;
    p->form = form;
    p->data1.f = range;
    return p;
}

nbKoukaPacket_t *nbAddBadDmgMotKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                           f32 range, s32 badstat)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 10;
    p->form = form;
    p->data1.f = range;
    p->data2.i = badstat;
    return p;
}

nbKoukaPacket_t *nbAddBadMotKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                        s32 badstat)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 37;
    p->form = form;
    p->data2.i = badstat;
    return p;
}

nbKoukaPacket_t *nbAddReturnKoukaPacket(s32 startframe, s32 uniqueid, nbParty_t *party, s32 dead)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 11;
    p->party = party;
    p->data1.i = dead;
    return p;
}

nbKoukaPacket_t *nbAddSummonKoukaPacket(s32 startframe, s32 uniqueid, s32 side, s32 stockno,
                                        s32 formindex)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 12;
    p->data1.s[0] = side;
    p->data1.s[1] = stockno;
    p->data2.s[0] = formindex;
    return p;
}

nbKoukaPacket_t *nbAddSkillColorOnPacket(s32 startframe, s32 uniqueid, s32 skillid, s32 para)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 54;
    p->data1.i = skillid;
    p->data2.i = para;
    return p;
}

nbKoukaPacket_t *nbAddSkillColorOffPacket(s32 startframe, s32 uniqueid)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 14;
    return p;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544900);

nbKoukaPacket_t *nbAddHitEffKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 type)
{
    nbKoukaPacket_t *p;
    s32 sysid;
    s32 seid;

    seid = 0;
    if ((form->stat & 1) == 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 15;
    p->form = form;
    p->data1.i = type;
    switch (type) {
    case 0:
        sysid = 24;
        break;
    case 7:
        sysid = 25;
        break;
    case 1:
        seid = 0x10008;
        sysid = 17;
        break;
    case 2:
        seid = 0x10009;
        sysid = 18;
        break;
    case 4:
        seid = 0x10001;
        sysid = 26;
        break;
    case 5:
        sysid = 20;
        seid = 0x10000;
        break;
    case 6:
        sysid = 14;
        break;
    case 3:
        p->framelength = 0;
        return p;
    case 8:
        seid = 0x10005;
        sysid = 27;
        break;
    case 9:
        seid = 0x10007;
        sysid = 28;
        break;
    case 10:
        seid = 0x10006;
        sysid = 29;
        break;
    default:
        return p;
    }
    p->data2.p = effBTLSystemCreate(form, sysid);
    p->framelength = effBTLSystemGetLength(p->data2.p);
    printf(D_00544900, p->framelength);
    if (seid != 0) {
        nbAddSePacket(startframe, uniqueid, seid);
    }
    return p;
}

nbKoukaPacket_t *nbAddBadEffKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 24;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddBadEffKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 bad)
{
    s32 index;
    nbKoukaPacket_t *p;

    index = 0;
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 23;
    p->form = form;
    switch (bad) {
    case 0x1:
        index = 1;
        break;
    case 0x2:
        index = 2;
        break;
    case 0x4:
        index = 3;
        break;
    case 0x8:
        index = 4;
        break;
    case 0x10:
        index = 5;
        break;
    case 0x20:
        index = 6;
        break;
    case 0x40:
        index = 7;
        break;
    case 0x80:
        index = 8;
        break;
    case 0x100:
        index = 9;
        break;
    case 0x200:
        index = 10;
        break;
    case 0x400:
        index = 11;
        break;
    }
    p->data1.i = index;
    return p;
}

nbKoukaPacket_t *nbAddNumEffKoukaPacket(s32 startframe, s32 frame, s32 uniqueid, nbFormation_t *form,
                                        s32 n, s32 n2, s32 type)
{
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    k = nbGetKoukaProcessData();
    p = k->first;
    while (p != NULL) {
        next = p->next;
        if (p->type == 16 && p->uniqueid == uniqueid && p->form == form &&
            startframe < p->startframe + p->framelength) {
            p->framelength = startframe - p->startframe;
            if (p->framelength < 0) {
                p->framelength = 0;
            }
        }
        p = next;
    }
    p = nbAddKoukaPacket(k, startframe, frame, uniqueid);
    p->type = 16;
    p->form = form;
    p->data1.i = type;
    p->data2.i = n;
    p->partyno = n2;
    return p;
}

nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 startframe, s32 frame, s32 uniqueid,
                                         nbFormation_t *form, s32 type)
{
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    nbKoukaPacket_t *next;

    k = nbGetKoukaProcessData();
    p = k->first;
    while (p != NULL) {
        next = p->next;
        if (p->type == 22 && p->uniqueid == uniqueid && p->form == form &&
            startframe < p->startframe + p->framelength) {
            p->framelength = startframe - p->startframe;
            if (p->framelength < 0) {
                p->framelength = 0;
            }
        }
        p = next;
    }
    if (type == 4) {
        nbAddSePacket(startframe, uniqueid, 0x1000A);
    }
    p = nbAddKoukaPacket(k, startframe, frame, uniqueid);
    p->type = 22;
    p->form = form;
    p->data1.i = type;
    return p;
}

nbKoukaPacket_t *nbAddHpMpKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 n,
                                      s32 type)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 17;
    p->form = form;
    p->data1.i = type;
    p->data2.i = n;
    return p;
}

nbKoukaPacket_t *nbAddPressPacket(s32 startframe, s32 uniqueid, f32 press)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 18;
    p->data1.f = press;
    return p;
}

nbKoukaPacket_t *nbAddNewPressPacket(s32 startframe, s32 uniqueid, s32 press_p, s32 press_ten)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 31;
    p->data1.s[0] = press_p;
    p->data1.s[1] = press_ten;
    return p;
}

nbKoukaPacket_t *nbAddBadStatKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                         u32 badstat, s32 sw)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 19;
    p->form = form;
    p->data1.i = sw;
    p->data2.i = badstat;
    return p;
}

nbKoukaPacket_t *nbAddBadMotKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 20;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddDeadPcmPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    datUnitWork_t *unit;
    nbKoukaPacket_t *p;

    unit = nbGetUnitWorkFromFormindex(form->formindex);
    if (unit == NULL) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 36;
    p->form = form;
    if ((unit->badstatus & 0xFFF) == 0x200) {
        p->data1.i = 255;
    } else {
        p->data1.i = unit->id;
    }
    return p;
}

nbKoukaPacket_t *nbAddSePacket(s32 startframe, s32 uniqueid, s32 seid)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, seid, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 25;
    p->data1.i = seid;
    return p;
}

nbKoukaPacket_t *nbAddSePacket2(s32 startframe, s32 uniqueid, s32 seid)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, seid, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 28;
    p->data1.i = seid;
    return p;
}

nbKoukaPacket_t *nbAddSePacketLock(s32 startframe, s32 uniqueid, s32 seid)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, seid, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 29;
    p->data1.i = seid;
    return p;
}

void nbAddHitStopPacket(s32 startframe, s32 uniqueid)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 2, uniqueid);
    p->type = 30;
}

nbKoukaPacket_t *nbAddAnalyzePacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 2, uniqueid);
    p->type = 34;
    p->form = form;
    return p;
}

nbKoukaPacket_t *nbAddJyokyoHelpPacket(s32 startframe, s32 uniqueid, s32 no, s32 var_id, s32 var_ind)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 35;
    p->data1.s[0] = no;
    p->data1.s[1] = var_id;
    p->data2.i = var_ind;
    return p;
}

nbKoukaPacket_t *nbAddSkillNamePacket(s32 startframe, s32 uniqueid, s32 id)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 49;
    p->data1.i = id;
    return p;
}

nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 sw)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 38;
    p->form = form;
    p->data1.i = sw;
    return p;
}

nbKoukaPacket_t *nbAddFormFlagOnOffPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 sw,
                                          u32 flag)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 44;
    p->form = form;
    p->data1.i = sw;
    p->data2.i = flag;
    return p;
}

nbKoukaPacket_t *nbAddFlyKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 45;
    p->form = form;
    p->data1.i = 0;
    return p;
}

nbKoukaPacket_t *nbAddOngBunshinPacket(s32 startframe, s32 uniqueid, nbFormation_t *sform,
                                       nbFormation_t *dform)
{
    effPCPBossBUNSINParam_t param;
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    void *h;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, effPCPBossBUNSINGetLength(), uniqueid);
    param.pStartForm = sform;
    param.pEndForm = dform;
    h = effPCPBossBUNSINCreate(&param);
    p->type = 39;
    p->data1.p = h;
    return p;
}

nbKoukaPacket_t *nbAddOngBunshin2Packet(s32 startframe, s32 uniqueid, nbFormation_t *sform,
                                        nbFormation_t *form1, nbFormation_t *form2l,
                                        nbFormation_t *form2r)
{
    effPCPBossBUNSIN2Param_t param;
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    void *h;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, effPCPBossBUNSIN2GetLength(), uniqueid);
    param.pStartForm = sform;
    param.pForm1 = form1;
    param.pForm2L = form2l;
    param.pForm2R = form2r;
    h = effPCPBossBUNSIN2Create(&param);
    p->type = 40;
    p->data1.p = h;
    return p;
}

nbKoukaPacket_t *nbAddOngOutPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    effPCPBossONGYParam_t param;
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    void *h;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, effPCPBossONGYGetLength(), uniqueid);
    param.pFormation = form;
    h = effPCPBossONGYCreate(&param);
    p->type = 41;
    p->data1.p = h;
    return p;
}

nbKoukaPacket_t *nbAddOngOut2Packet(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    effPCPBossONGY2Param_t param;
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    void *h;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, effPCPBossONGY2GetLength(), uniqueid);
    param.pFormation = form;
    h = effPCPBossONGY2Create(&param);
    p->type = 42;
    p->data1.p = h;
    return p;
}

nbKoukaPacket_t *nbAddDeadEventKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                           s32 eventno)
{
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, 0, uniqueid);
    k->data->actionstackp = 0;
    k->data->eventno = p->data1.i;
    k->data->endtype = 7;
    p->type = 46;
    p->form = form;
    p->data1.i = eventno;
    return p;
}

nbKoukaPacket_t *nbAddFadeOutKoukaPacket(s32 startframe, s32 uniqueid, s32 length, s32 coltype)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 47;
    p->data1.i = length;
    p->data2.i = coltype;
    return p;
}

nbKoukaPacket_t *nbAddFadeInKoukaPacket(s32 startframe, s32 uniqueid, s32 length, s32 coltype)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 48;
    p->data1.i = length;
    p->data2.i = coltype;
    return p;
}

nbKoukaPacket_t *nbAddHojoKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                      u32 hojotype, s32 hojopoint)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 50;
    p->form = form;
    p->data1.i = hojotype;
    p->data2.i = hojopoint;
    return p;
}

nbKoukaPacket_t *nbAddFlareChargePacket(s32 startframe, s32 uniqueid, s32 point)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 51;
    p->data1.i = point;
    return p;
}

nbKoukaPacket_t *nbAddPadShockPacket(s32 startframe, s32 uniqueid, s32 mode, s32 pow, s32 time)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 52;
    p->data1.s[0] = mode;
    p->data1.s[1] = pow;
    p->data2.s[0] = time;
    return p;
}

void nbAddPanelStockFadeOutPacket(s32 startframe, s32 uniqueid)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 56;
}

void nbAddPanelStockEndPacket(s32 startframe, s32 uniqueid)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 57;
}

s32 nbMakeSummonPacket(s32 startframe, s32 uniqueid, s32 side, s32 stockno, s32 formindex)
{
    nbFormation_t *form;

    form = &nbGetMainProcessData()->form[formindex];
    nbAddSummonKoukaPacket(startframe, uniqueid, side, stockno, formindex);
    nbMakeUnitFadeInPacket(startframe + 1, uniqueid, form, 0, 15, 15, 20);
    return 1;
}

s32 nbMakeReturnPacket(s32 startframe, s32 uniqueid, nbParty_t *party)
{
    nbFormation_t *form;

    form = &nbGetMainProcessData()->form[party->formindex];
    party->flag |= 0x20;
    nbMakeUnitFadeOutPacket(startframe, uniqueid, form, 0, 6, 6, 6);
    nbAddReturnKoukaPacket(startframe + 13, uniqueid, party, 0);
    return 1;
}

s32 nbMakeUnitFadeInPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 col, s32 delay,
                           s32 aframe, s32 colframe)
{
    nbAddUnitColKoukaPacket(startframe, uniqueid, form, col, 0);
    nbAddUnitHomeAKoukaPacket(startframe, uniqueid, form, aframe);
    nbAddUnitHomeColKoukaPacket(startframe + delay, uniqueid, form, colframe);
    return 1;
}

s32 nbMakeUnitFadeOutPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 col, s32 delay,
                            s32 colframe, s32 aframe)
{
    nbAddUnitColKoukaPacket(startframe, uniqueid, form, col, colframe);
    nbAddUnitAKoukaPacket(startframe + delay, uniqueid, form, 0, aframe);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeDeadVisualPacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeDamagePacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeKaifukuPacket);

s32 nbMakeDeadPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 deadchk)
{
    nbMainProcessData_t *data;
    nbParty_t *party;
    datUnitWork_t *unit;
    u32 stat;

    data = nbGetMainProcessData();
    stat = form->stat;
    if ((stat & 1) == 0) {
        return 1;
    }
    if (stat & 0x40) {
        party = &data->party[form->partyindex];
        unit = nbGetUnitWorkPointer(data, party);
        party->flag |= 0x30;
        if (nbCheckEventDead(form) != 0 || (unit->flag & 4) != 0) {
            if (deadchk != 1) {
                return 1;
            }
        }
        nbAddReturnKoukaPacket(startframe, uniqueid, party, 1);
    }
    return 1;
}

s32 nbMakePressPacket(s32 startframe, s32 uniqueid, f32 press)
{
    nbMainProcessData_t *data;
    s32 pcnt;

    data = nbGetMainProcessData();
    if (data->stat & 8) {
        pcnt = data->enemypcnt;
    } else {
        pcnt = data->playerpcnt;
    }
    nbAddPressPacket(startframe, uniqueid, press / pcnt);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeNewPressPacket);

s32 nbMakeFlyBadPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    nbAddFlyKoukaPacket(startframe, uniqueid, form);
    return 1;
}

s32 nbMakeBadDamagePacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 bad)
{
    nbAddBadDmgMotKoukaPacket(startframe, uniqueid, form, 17.0f, bad);
    nbAddBadEffKoukaPacket(startframe, uniqueid, form, bad);
    nbAddBadStatKoukaPacket(startframe, uniqueid, form, bad, 1);
    return 1;
}

s32 nbMakeBadKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form)
{
    u32 bad;
    s32 formindex;
    s32 vis;
    s32 hide;

    formindex = form->formindex;
    vis = formindex < 16;
    hide = !vis;
    bad = nbGetBadStat(formindex);
    if (bad != 0) {
        nbAddBadStatKoukaPacket(startframe, uniqueid, form, 0xFFF, 0);
    }
    if (!hide) {
        switch (bad) {
        case 0x1:
        case 0x2:
        case 0x4:
        case 0x8:
        case 0x10:
        case 0x20:
        case 0x40:
        case 0x80:
        case 0x100:
        case 0x200:
        case 0x400:
        case 0x800:
            nbAddBadMotKaifukuPacket(startframe, uniqueid, form);
            nbAddBadEffKaifukuPacket(startframe, uniqueid, form);
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeUnitMoveGetCyDist);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeUnitMovePacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeNegoWorkPosPacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeNegoBackPacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeNegoForwardPacket);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetHitType);

s32 nbGet3Zokusei(s32 formindex)
{
    datUnitWork_t *unit;
    s32 z;

    unit = nbGetUnitWorkFromFormindex(formindex);
    z = datGet3Zokusei(unit);
    switch (z) {
    case 0:
        return 1;
    case 1:
        return 2;
    case -1:
        return 0;
    }
    return 1;
}

s32 nbCheckKoukaJyoken(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 hittype;
    s32 zoku;

    hittype = datSkillTable.normal[nskill].hittype;
    zoku = nbGet3Zokusei(dformindex);
    if ((hittype & 1) != 0) {
        return 1;
    }
    if ((hittype & 2) != 0 && zoku == 0) {
        return 1;
    }
    if ((hittype & 4) != 0 && zoku == 1) {
        return 1;
    }
    if ((hittype & 8) != 0 && zoku == 2) {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005449E0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005449F8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKoukaType);

void nbAddHpMp(s32 formindex, s32 type, s32 n)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return;
    }
    switch (type) {
    case 0:
        datAddHp(unit, n);
        break;
    case 1:
        datAddMp(unit, n);
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetAisyoRitu);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKoukaHp);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKoukaMp);

f32 nbGetHpDrainRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 hptype;

    hptype = datSkillTable.normal[nskill].hptype;
    if (hptype < 14) {
        if (hptype >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 nbGetMpDrainRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    s32 mptype;

    mptype = datSkillTable.normal[nskill].mptype;
    if (mptype < 14) {
        if (mptype >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 nbGetGensyoRitu(s32 nskill, s32 sformindex, s32 dformindex)
{
    return datSkillTable.normal[nskill].minus / 100.0f;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKoukaBadDamage);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544BA8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKoukaBadKaifuku);

u32 nbCheckHitEffect(s32 nskill, s32 sformindex, s32 dformindex)
{
    return datSkillTable.visual[nskill].bedno == 0;
}

s32 nbGetDeadType(s32 nskill, s32 sformindex, s32 dformindex)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckStoneDead);

void nbSetBadStat(s32 formindex, s32 sw, s32 badstat)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return;
    }
    if (sw != 0) {
        datSetBadStat(unit, badstat);
    } else {
        datResetBadStat(unit, badstat);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBadKaifuku);

u32 nbGetBadStat(s32 formindex)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    return unit->badstatus & 0xFFF;
}

s32 nbCheckBadCurse(s32 formindex)
{
    datUnitWork_t *unit;
    s32 bad;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    bad = unit->badstatus & 0x8000;
    return bad != 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetAisyoTable);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544BF8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544C10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetAisyo);

s32 nbGetNormalSkillAttr(s32 nskill)
{
    return attr[nskill].skillattr;
}

void nbSetHojoCounter(s32 formindex, s32 type, s32 n)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party != NULL) {
        party->count[type] = n;
    }
}

s32 nbGetHojoCounter(s32 formindex, s32 type)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return 0;
    }
    return party->count[type];
}

s32 nbGetHojoCounterMax(s32 hojotype)
{
    s32 max;

    max = 0;
    switch (hojotype) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 16:
    case 17:
    case 18:
        max = 127;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        max = 4;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        max = 1;
        break;
    case 19:
        max = 32767;
        break;
    }
    return max;
}

s32 nbGetHojoCounterMin(s32 hojotype)
{
    s32 min;

    min = 0;
    switch (hojotype) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        min = -4;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        min = 0;
        break;
    }
    return min;
}

s32 nbSetHojoAddCounter(s32 formindex, s32 type, s32 point, s32 nvirtual)
{
    nbParty_t *party;
    s32 old;
    s32 n;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return 0;
    }
    old = party->count[type];
    n = old + point;
    if (type < 20) {
        if (type >= 0) {
            if (nbGetHojoCounterMax(type) < n) {
                n = nbGetHojoCounterMax(type);
            }
            if (n < nbGetHojoCounterMin(type)) {
                n = nbGetHojoCounterMin(type);
            }
        }
    }
    if (nvirtual == 0) {
        party->count[type] = n;
    }
    return n - old;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetHojoKouka);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetHojoRitu);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetButuriAttack);

s32 nbGetButuriKaifuku(s32 nskill, s32 sformindex, s32 dformindex, s32 waza)
{
    return nbGetButuriAttack(nskill, sformindex, dformindex, waza);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetMagicAttack);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetMagicKaifuku);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetMaxHpWazaPoint);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetVirtualAisyo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetVirtualKoukaPoint);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckSkillCost);

s32 nbCheckSkillUse(s32 formindex, s32 nskill)
{
    datUnitWork_t *unit;
    s32 cost;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if ((unit->badstatus & 0xFFF) == 0x20) {
        if (datSkillTable.normal[nskill].koukatype == 1) {
            return 1;
        }
    }
    cost = nbCheckSkillCost(formindex, nskill);
    switch (cost) {
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountTableItem);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetExpMakaItem);

s32 nbGetLvAvg(nbMainProcessData_t *data, s32 type)
{
    s32 i;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    for (i = 0; i < 14; i++) {
        if ((data->party[i].flag & 1) != 0 && (data->party[i].flag & 4) == 0) {
            if (type == 0) {
                if ((data->party[i].flag & 2) != 0) {
                    continue;
                }
            } else {
                if ((data->party[i].flag & 2) == 0) {
                    continue;
                }
            }
            cnt++;
            sum += nbGetUnitWorkPointer(data, &data->party[i])->level;
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetLvAvg2(nbMainProcessData_t *data, s32 mode)
{
    datUnitWork_t *w;
    s32 i;
    s32 id;
    s32 cnt;
    s32 sum;

    sum = 0;
    cnt = 0;
    if (mode == 0) {
        w = (datUnitWork_t *)(DDS3_GBWK + 0xA54);
        for (i = 0; i < 16; i++) {
            if ((w->flag & 1) != 0) {
                if ((w->flag & 2) != 0) {
                    cnt++;
                    sum += w->level;
                }
            }
            w++;
        }
    } else {
        for (i = 0; i < 11; i++) {
            id = nbEncountFormTable[data->encno].id[i];
            if (id != 0) {
                cnt++;
                sum += datDevilTable[id].level;
            }
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetParamAvg(nbMainProcessData_t *data, s32 type, s32 param)
{
    s32 i;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    for (i = 0; i < 14; i++) {
        if ((data->party[i].flag & 1) != 0 && (data->party[i].flag & 4) == 0) {
            if (type == 0) {
                if ((data->party[i].flag & 2) != 0) {
                    continue;
                }
            } else {
                if ((data->party[i].flag & 2) == 0) {
                    continue;
                }
            }
            cnt++;
            sum += datGetParam(nbGetUnitWorkPointer(data, &data->party[i]), param);
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

s32 nbGetParamAvg2(nbMainProcessData_t *data, s32 mode, s32 param)
{
    datUnitWork_t *w;
    s32 i;
    s32 ofs;
    s32 id;
    s32 cnt;
    s32 sum;

    cnt = 0;
    sum = 0;
    if (mode == 0) {
        for (i = 0, ofs = 0; i < 16; i++) {
            w = (datUnitWork_t *)(DDS3_GBWK + ofs + 0xA54);
            ofs += 0xEC;
            if ((w->flag & 1) != 0) {
                if ((w->flag & 2) != 0) {
                    cnt++;
                    sum += datGetParam(w, param);
                }
            }
        }
    } else {
        for (i = 0; i < 11; i++) {
            id = nbEncountFormTable[data->encno].id[i];
            if (id != 0) {
                cnt++;
                sum += datDevilTable[id].param[param];
            }
        }
    }
    if (cnt == 0) {
        return 0;
    }
    return sum / cnt;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckSensei);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBackAttack);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckEscape);

s32 nbGetEscapeSwitch(nbMainProcessData_t *data)
{
    if ((data->stat & 1) != 0) {
        return 0;
    }
    return nbEncountFormTable[data->encno].esc != 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D00);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D10);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D20);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D28);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D30);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D58);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544D70);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544DA8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544DE0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckRenzokuEncount);

s32 nbCheckKuisibariSkill(s32 formindex)
{
    datUnitWork_t *unit;
    nbParty_t *party;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (datCheckSyojiSkill(unit, 0x159) == 0) {
        return 0;
    }
    party = nbGetPartyFromFormindex(formindex);
    if ((party->flag & 0x400) != 0) {
        return 0;
    }
    party->flag |= 0x200;
    return 1;
}

s32 nbCheckKuisibari2Skill(s32 formindex)
{
    datUnitWork_t *unit;
    nbParty_t *party;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (datCheckSyojiSkill(unit, 0x168) == 0) {
        return 0;
    }
    party = nbGetPartyFromFormindex(formindex);
    if ((party->flag & 0x400) != 0) {
        return 0;
    }
    party->flag |= 0x200;
    return 1;
}

void nbSetKuisibariStop(s32 formindex)
{
    nbParty_t *party;

    party = nbGetPartyFromFormindex(formindex);
    if (party == NULL) {
        return;
    }
    if ((party->flag & 0x200) != 0) {
        party->flag |= 0x400;
    }
}

s32 nbHatudoCheckSkill(datUnitWork_t *unit, s32 skill)
{
    if (datCheckSyojiSkill(unit, skill) == 0) {
        return 0;
    }
    switch (skill) {
    case 300:
        return evtGetAgeOfMoon() == 8;
    case 301:
        return evtGetAgeOfMoon() == 0;
    }
    return 1;
}

s32 nbCheckSingleTargetSkill(s32 nskill)
{
    return datSkillTable.normal[nskill].targettype == 0;
}

s32 nbCheckFormationPlayerSide(s32 formindex)
{
    return formindex < 4;
}

s32 nbCheckFormationFriend(s32 formindex1, s32 formindex2)
{
    return nbCheckFormationPlayerSide(formindex1) == nbCheckFormationPlayerSide(formindex2);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetMakaBaramaki);

s32 nbCheckBadPanicAction(datUnitWork_t *unit)
{
    u32 flag;
    s32 ritu;
    s32 hit;

    flag = unit->flag;
    if ((flag & 4) != 0) {
        ritu = 30;
    } else if ((flag & 0x20) != 0) {
        ritu = 10;
    } else {
        ritu = 30;
    }
    hit = dds3GetRandIntA(0, 100) < ritu;
    if (hit) {
        return 0;
    }
    return 1;
}

s32 nbCheckBadStatusGameOver(nbMainProcessData_t *data)
{
    datUnitWork_t *unit;
    s32 i;

    for (i = 0; i < 4; i++) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (unit != NULL && (unit->badstatus & 0xFFF) != 0x400) {
            break;
        }
    }
    return i == 4;
}

void nbSetHpScale(nbFormation_t *form)
{
    datUnitWork_t *unit;
    f32 ritu;

    unit = nbGetUnitWorkFromFormindex(form->formindex);
    if (unit == NULL) {
        return;
    }
    if (unit->hp == 0) {
        return;
    }
    ritu = (f32)unit->hp / (f32)unit->maxhp;
    form->scl = (nbGetUnitVisual(unit)->formscale - 0.5f) * ritu + 0.5f;
}

u32 nbGetDevilFormatFlag(s32 formindex)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(formindex);
    if (unit == NULL) {
        return 0;
    }
    if ((unit->flag & 4) != 0) {
        return 0;
    }
    return datDevilTable[unit->id].flag;
}

f32 nbGetCriticalPow(s32 sformindex, f32 pow)
{
    /* Player and enemy sides use the same multiplier. */
    if (sformindex < 4) {
        pow = pow * 1.5f;
    } else {
        pow = pow * 1.5f;
    }
    return pow;
}

s32 nbGetKakutokuExp(nbMainProcessData_t *data)
{
    s32 exp;
    s32 encno;

    encno = data->encno;
    exp = data->exp;
    if (encno == 267 || encno == 284) {
        if (exp > 20000) {
            exp = 20000;
        }
    }
    return exp;
}

s32 nbGetKakutokuMaka(nbMainProcessData_t *data)
{
    s32 maka;
    s32 encno;

    encno = data->encno;
    maka = data->maka;
    if (encno == 267 || encno == 284) {
        if (maka > 10000) {
            maka = 10000;
        }
    }
    return maka;
}

void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id)
{
    s32 cnt;

    cnt = n->nextkouhocnt;
    if (cnt < 8) {
        n->nextkouho[cnt] = id;
        n->nextkouhocnt++;
    }
}

u32 nbGetPrevNegoId(nbNegoProcessData_t *n)
{
    return n->history[n->historyindex];
}

u32 nbGetBackNoNegoId(nbNegoProcessData_t *n, s32 a)
{
    return n->history[(n->historyindex - a + 15) % 14];
}

void nbSetMesMakka(s32 msgid, s32 i)
{
    itfMesMngSetVar(msgid, 3, i, 0);
}

void nbSetMesItem(s32 msgid, s32 it)
{
    itfMesMngSetVar(msgid, 2, it, 3);
}

void nbClsNegoMessage(nbNegoProcessData_t *n)
{
    s32 msgwndid;

    msgwndid = n->msgwndid;
    if (msgwndid >= 0) {
        itfMesMngMsgWndCls(msgwndid);
        n->msgwndid = -1;
    }
}

void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel)
{
    n->nowmsgid = msgid;
    n->nowmsgno = msgno;
    n->nowselect = 0;
    if (sel != 0) {
        n->stat |= 4;
    } else {
        n->stat &= ~4;
        n->nowmes = msgno;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckNegoMessage);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetNegoMesVar);

s32 nbGetNegoYaruMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = (datDevilTable[b->id].dropmakka * (dds3GetRandIntA(0, 4) + 5) + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoYaruMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = nbGetNegoKureMakkaA(n, a, b);
    maka = (maka * (dds3GetRandIntA(0, 30) + 40) / 100 + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = (s32)((f32)(b->level * (dds3GetRandIntA(0, 10) + 21)) * 0.8f);
    maka = (maka + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    s32 maka;

    maka = nbGetNegoKureMakkaA(n, a, b);
    maka = (maka * (dds3GetRandIntA(0, 20) + 50) / 100 + 5) / 10 * 10;
    return maka != 0 ? maka : 1;
}

s32 nbGetNegoKureHpA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b)
{
    if (evtBitCheck(8) != 0) {
        return b->maxhp * (dds3GetRandIntA(0, 10) + 20) / 100;
    }
    return b->maxhp * (dds3GetRandIntA(0, 8) + 14) / 100;
}

s32 nbGetNegoKureHpB(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework)
{
    return ework->maxhp * (dds3GetRandIntA(0, 10) + 10) / 100;
}

s32 nbGetNegoKureMpA(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework)
{
    return ework->maxmp * (dds3GetRandIntA(0, 30) + 10) / 100;
}

s32 nbGetNegoKureMpB(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework)
{
    return ework->maxmp * (dds3GetRandIntA(0, 10) + 10) / 100;
}

s32 nbGetKureItemA(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework)
{
    s32 group;

    if (ework->level == 0) {
        return 0;
    }
    if (ework->level >= 0x64) {
        return 0;
    }
    group = (ework->level - 1) / 5;
    return D_003E2318[(group * 4 + nbSelectKureItemA(group)) * 2];
}

s32 nbSelectKureItemA(s32 group)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    r = dds3GetRandIntA(0, 0x64);
    p = &datNegoReqItemListA[group * 8];
    sum = 0;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
    }
    return 0;
}

s32 nbGetKureItemB(nbNegoProcessData_t *n, datUnitWork_t *pwork, datUnitWork_t *ework)
{
    s32 group;

    if (ework->level == 0) {
        return 0;
    }
    if (ework->level >= 0x64) {
        return 0;
    }
    group = (ework->level - 1) / 5;
    return D_003E23F8[(group * 3 + nbSelectKureItemB(group)) * 2];
}

s32 nbSelectKureItemB(s32 group)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    r = dds3GetRandIntA(0, 0x64);
    p = &datNegoReqItemListB[group * 6];
    sum = 0;
    for (i = 0; i < 3; i++, p += 2) {
        if (r < sum + *p) {
            return i;
        }
        sum += *p;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoRequestType);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoTuikaRequestType);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetAllHojoSkillWork);

s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n)
{
    nbMainProcessData_t *data;
    s32 cnt;
    s32 i;
    s32 j;

    data = n->data;
    cnt = 0;
    for (i = 0; i < 4; i++) {
        if (data->party[i].flag & 1) {
            if (!(data->party[i].flag & 0x80)) {
                for (j = 0; j < 8; j++) {
                    if (n->data->party[i].hojoskill[j] != 0) {
                        if (!(n->data->party[i].hojoskill[j] & 0x8000)) {
                            cnt++;
                        }
                    }
                }
            }
        }
    }
    return cnt;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSearchHojoSkillWork);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetQuestionType);

s32 nbGetNegoYaruItemA(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].itema[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

s32 nbGetNegoYaruItemB(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].itemb[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

s32 nbGetNegoYaruStone(datUnitWork_t *w)
{
    u8 *p;
    s32 r;
    s32 sum;
    s32 i;

    sum = 0;
    r = dds3GetRandIntA(0, 0x64);
    /* Walks the item byte of each entry; its weight is the byte before it. */
    p = &master_text[w->id].stone[0].item;
    for (i = 0; i < 4; i++, p += 2) {
        if (r < sum + p[-1]) {
            return *p;
        }
        sum += p[-1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetInfoMsg);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckSelectMesJoukenAri);

s32 nbSetNegoNoMotion(nbNegoProcessData_t *n, nbFormation_t *form)
{
    nbClsNegoMessage(n);
    nbAddMotionKoukaPacket(0, (s32)n->actdata, form, 0xF);
    nbAddSePacket2(0, (s32)n->actdata, nbPlayMotionSe(form) + 1);
    return 0xF;
}

s32 nbSetNegoYesMotion(nbNegoProcessData_t *n, nbFormation_t *form)
{
    nbClsNegoMessage(n);
    nbAddMotionKoukaPacket(0, (s32)n->actdata, form, 0xE);
    nbAddSePacket2(0, (s32)n->actdata, nbPlayMotionSe(form));
    return 0xF;
}

s32 nbFlowFindUnitEntry(nbNegoProcessData_t *n)
{
    nbInnenRec_t *p;
    s32 i;

    p = D_003E3158;
    for (i = 0; i < 64; i++) {
        if (p->id != 0 && n->pwork->id == p->pid && n->ework->id == p->eid) {
            return i;
        }
        p++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoDebugPrint);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoDebugPrint2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", NegoDebug);

void *nbNegoProcessMessageChk(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *n;

    n = dds3GetProcessData(id);
    if (n->data->stat & 0x400000) {
        nbNegoDebugPrint(n);
    }
    if (nbCheckNegoMessage(n) != 0) {
        printf(D_00544F10);
        return nbNegoProcess1;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoProcessMessage);

s32 nbNegoSetProcessData(dds3ProcessID_t *id)
{
    nbNegoProcessData_t *n;

    n = dds3GetProcessData(id);
    n->data->negoresult = n->endtype;
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoShutdown);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00544F10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", InitNegoProcessData);

void nbInitNegoProcess(nbActionProcessData_t *a, s32 pformindex, s32 eformindex, s32 negotype,
                      s32 skillid)
{
    nbNegoProcessData_t *n;
    dds3ProcessID_t *id;

    n = InitNegoProcessData(a, pformindex, eformindex, negotype, skillid);
    nbSetNegoWaitMotion(n->eform);
    nbSetAllHojoSkillWork(n);
    p2sdBgmVolDown();
    id = dds3StartProcess(D_00551160, 0x3F9, 1, 1, nbNegoProcess1, nbNegoShutdown, n);
    dds3LinkProcess(a->data->mainproc, id);
    a->data->negoproc = id;
}

s32 Flow1_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10005);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x10004;
    if (datCheckTalk(n->pwork) != 0 || (n->actdata->unk_04 & 0x20) == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_5);

s32 Flow1_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000A);
    n->flag1_6 = 1;
    return 0;
}

s32 Flow1_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10008);
    nbAddNegoIdKouho(n, 0x10003);
    if (datCheckTalk(n->ework) != 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x10009;
    return 0;
}

s32 Flow1_9(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 8;
    switch (n->ework->badstatus & 0xFFF) {
    case 1:
        mes = 0xA;
        break;
    case 0x200:
        mes = 9;
        break;
    case 4:
        mes = 0xB;
        break;
    case 8:
        mes = 0xD;
        break;
    case 0x10:
        mes = 0xC;
        break;
    case 2:
        mes = 0xE;
        break;
    case 0x400:
        break;
    }
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    return 1;
}

s32 Flow1_10(nbNegoProcessData_t *n)
{
    nbMainProcessData_t *data;
    s32 major;

    nbAddNegoIdKouho(n, 0x1001F);
    nbAddNegoIdKouho(n, 0x1001A);
    n->workid[3] = 0x1000B;
    if (n->ework->id >= 0x100) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    data = n->data;
    if ((data->stat & 1) != 0) {
        n->nextnegoid = n->nextkouho[0];
    }
    major = data->stagemajor;
    if (major >= 0xF0 && major < 0xF7) {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_33);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_32);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_12);

s32 Flow1_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000E);
    nbAddNegoIdKouho(n, 0x1000F);
    return 0;
}

s32 Flow1_14(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1000F);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 6, 0);
    return 0;
}

s32 Flow1_15(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x10010);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 7, 0);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_16);

s32 Flow1_17(nbNegoProcessData_t *n)
{
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 4, 0);
    datAddMaka(-n->getmakka);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_18);

s32 Flow1_19(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10014);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x40001;
    n->nextnegoid = n->nextkouho[0];
    if ((n->pwork->flag & 4) == 0 && (n->ework->flag & 4) == 0 &&
        n->pwork->id == n->ework->id) {
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_20(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10015);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x30001;
    n->nextnegoid = n->nextkouho[0];
    if ((n->ework->flag & 4) == 0 && datSearchDevilStock(n->ework->id) >= 0 &&
        n->negoreqtype == 0) {
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_21(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10016);
    nbAddNegoIdKouho(n, 0x1001B);
    if (n->data->enemypcnt == 1) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    if ((n->data->stat & 0x800000) == 0) {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_25);

s32 Flow1_26(nbNegoProcessData_t *n)
{
    s32 entry;

    nbAddNegoIdKouho(n, 0x10012);
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0x50001;
    if (evtGetAgeOfMoon() == 8) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    entry = nbFlowFindUnitEntry(n);
    if (entry != -1) {
        n->innen_id = entry;
        n->flag1_6 = 0;
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_27(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    nbAddNegoIdKouho(n, 0x1001C);
    n->workid[3] = 0x1001E;
    if (master_text[n->ework->id].unk_09 == 1) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow1_28(nbNegoProcessData_t *n)
{
    if (n->negotype != 3) {
        nbAddNegoIdKouho(n, 0x1001D);
        nbAddNegoIdKouho(n, 0x1001F);
    } else {
        nbAddNegoIdKouho(n, 0x1001D);
        nbAddNegoIdKouho(n, 0x1001E);
    }
    n->nextnegoid = n->nextkouho[1];
    n->workid[3] = 0x1001E;
    if (datCheckDying(n->ework) == 0 && dds3GetRandIntA(0, 100) < 0x32) {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_29);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_30);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_31);

s32 Flow1_34(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10023);
    nbAddNegoIdKouho(n, 0x10026);
    if (D_003E1338[n->negoskillid - 0x180].unk_00 == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x10024);
    nbAddNegoIdKouho(n, 0x10025);
    if (datGetPlayerLevel() < datDevilTable[n->ework->id].level) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow1_36(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x36, 0);
    return 1;
}

s32 Flow1_37(nbNegoProcessData_t *n)
{
    s32 mes;
    s32 found;

    found = 0;
    mes = 0;
    switch (datDevilTable[n->ework->id].race) {
    case 0x21:
        found = 1;
        mes = 0x37;
        break;
    case 7:
        found = 1;
        mes = 0x65;
        break;
    case 8:
        found = 1;
        mes = 0x66;
        break;
    }
    if (found == 1) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        return 1;
    }
    nbAddNegoIdKouho(n, 0x60001);
    return 0;
}

s32 Flow1_38(nbNegoProcessData_t *n)
{
    if (datDevilTable[n->ework->id].race == 0x21 && n->negoskillid == 0x193) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x64, 0);
        return 1;
    }
    nbAddNegoIdKouho(n, 0x60001);
    return 0;
}

s32 Flow1_39(nbNegoProcessData_t *n)
{
    if (datDevilTable[n->ework->id].race == 0x21 && n->negoreqtype == 0) {
        n->flag1_39 = 1;
        nbAddNegoIdKouho(n, 0x1001F);
        if (dds3GetRandIntA(0, 2) != 0) {
            n->temp_param[0] = 0x2B;
        } else {
            n->temp_param[0] = 0x2C;
        }
    } else {
        nbAddNegoIdKouho(n, 0x10013);
    }
    return 0;
}

s32 nbNegoFlowOne(nbNegoProcessData_t *n)
{
    return D_004EF4B0[(u16)n->nownegoid](n);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFlowGetUnitEntry);

s32 nbFlowCheckUnit(nbNegoProcessData_t *n, s32 ptype, s32 etype, s32 data)
{
    s32 p;
    s32 e;

    p = nbFlowGetUnitEntry(n->pwork, n->ework, ptype, data);
    e = nbFlowGetUnitEntry(n->ework, n->pwork, etype, data);
    if (p != 0 && e != 0) {
        return 1;
    }
    return 0;
}

s32 nbFlowCheckUnit2(datUnitWork_t *pwork, datUnitWork_t *ework, s32 ptype, s32 etype,
                     s32 data)
{
    s32 p;
    s32 e;

    p = nbFlowGetUnitEntry(pwork, ework, ptype, data);
    e = nbFlowGetUnitEntry(ework, pwork, etype, data);
    if (p != 0 && e != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_2);

s32 Flow6_80(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, 0x60051);
    nbAddNegoIdKouho(n, 0x60009);
    if ((u32)dds3GetRandIntA(0, 0xA) < 4) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        do {
            mes = dds3GetRandIntA(0, 4) + 0x1C;
        } while (mes == n->mes6_2);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        n->mes6_2 = mes;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_81(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60004);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x126, 0);
    return 0;
}

s32 Flow6_3(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60004);
        nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->aisyomes, 0);
        break;
    }
    return 0;
}

s32 Flow6_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60005);
    nbAddNegoIdKouho(n, 0x90001);
    if (dds3GetRandIntA(0, 100) < 65) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], 0x26, 0);
    return 0;
}

s32 Flow6_6(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60007);
        nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->aisyomes, 0);
        break;
    }
    return 0;
}

s32 Flow6_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60008);
    nbAddNegoIdKouho(n, 0xA0001);
    if (dds3GetRandIntA(0, 100) < 70) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], 0x27, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_9);

s32 Flow6_10(nbNegoProcessData_t *n)
{
    if (n->requesttype != 4) {
        n->requesttype = 4;
        n->prevreqtype = 4;
    }
    nbAddNegoIdKouho(n, 0x60046);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x42, 0);
    return 0;
}

s32 Flow6_11(nbNegoProcessData_t *n)
{
    if (n->requesttype != 3) {
        n->requesttype = 3;
        n->prevreqtype = 3;
    }
    nbAddNegoIdKouho(n, 0x60046);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x41, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_12);

s32 Flow6_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60010);
    nbAddNegoIdKouho(n, 0x6000E);
    n->nextnegoid = n->nextkouho[0];
    if (datCheckDying(n->ework) != 0) {
        if (dds3GetRandIntA(0, 100) < 70) {
            n->nextnegoid = n->nextkouho[1];
        }
    }
    return 0;
}

s32 Flow6_14(nbNegoProcessData_t *n)
{
    s32 mes;
    u32 prev;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x6000F);
        prev = nbGetPrevNegoId(n);
        switch (prev) {
        case 0x6000C:
            mes = 0x44;
            break;
        case 0x6000D:
            mes = 0x45;
            break;
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        break;
    }
    return 0;
}

s32 Flow6_15(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 2;
}

s32 Flow6_16(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x43, 0);
    nbMakaDispOn(0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_17);

s32 Flow6_18(nbNegoProcessData_t *n)
{
    if (n->requesttype == 1) {
        n->requestdata /= 2;
    }
    nbAddNegoIdKouho(n, 0x60014);
    return 0;
}

s32 Flow6_19(nbNegoProcessData_t *n)
{
    if (n->requesttype == 1) {
        n->requestdata *= 2;
    }
    nbAddNegoIdKouho(n, 0x60014);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_20);

s32 Flow6_21(nbNegoProcessData_t *n)
{
    if (n->requesttype != 2) {
        n->requesttype = 2;
        n->prevreqtype = 2;
    }
    nbAddNegoIdKouho(n, 0x70001);
    n->workid[4] = 0x60018;
    n->workid[5] = 0x60041;
    if (n->cnt6_37 != 0) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x38, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3E, 0);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_70);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_22);

s32 Flow6_23(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60041);
        n->workid[2] = 0x60045;
        switch (n->requesttype) {
        case 0:
            mes = 0x51;
            break;
        case 1:
            mes = 0x50;
            break;
        case 5:
            mes = 0x50;
            break;
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_24);

s32 Flow6_25(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, 0x6001A);
    do {
        mes = dds3GetRandIntA(0, 4) + 0x25;
    } while (mes == n->mes6_25);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    n->mes6_25 = mes;
    return 0;
}

s32 Flow6_26(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001B);
    nbAddNegoIdKouho(n, 0x6001C);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_27(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0x6001C);
    n->workid[0] = 0x6001D;
    n->workid[1] = 0x60021;
    r = nbSearchHojoSkillWork(n, 3);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_28);

s32 Flow6_29(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001E);
    nbAddNegoIdKouho(n, 0x60020);
    if (n->hojoaisyo == 1) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_30(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001F);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->hojoaisyomes, 0);
    return 0;
}

s32 Flow6_31(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60023);
    if (dds3GetRandIntA(0, 10) != 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_32);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_33);

s32 Flow6_34(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60026);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x47, 0);
    return 0;
}

s32 Flow6_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60027;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x114, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_36);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_37);

s32 Flow6_38(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60027;
    if (n->cnt6_36 == 1) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5D, 0);
        return 0;
    }
    if (n->cnt6_36 == 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5E, 0);
        return 0;
    }
    if (n->cnt6_36 == 3) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5F, 0);
        return 0;
    }
    if (n->cnt6_36 >= 4) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x60, 0);
        return 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_39);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_40);

s32 Flow6_41(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6002A);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_42);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_43);

s32 Flow6_44(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbAddNegoIdKouho(n, 0x6002D);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_45(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0xA0017);
    nbAddNegoIdKouho(n, 0x60034);
    n->workid[0] = 0x6002E;
    n->workid[1] = 0x6002F;
    r = nbSearchHojoSkillWork(n, 2);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_46(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60009;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x4F, 0);
    n->cnt6_36 = 0;
    n->cnt6_37 = 0;
    return 0;
}

s32 Flow6_47(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg2, 9, 0);
    n->waitframe = 60;
    return 0;
}

s32 Flow6_48(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60031);
    nbAddNegoIdKouho(n, 0xA0001);
    if (n->cnt6_58 >= 3) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_49(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60032);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    if (n->requesttype == 1) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_50(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60033);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_51(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    n->workid[0] = 0x6003A;
    n->workid[1] = 0x6003F;
    r = nbSearchHojoSkillWork(n, 0);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_52(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60035);
    nbClsNegoMessage(n);
    n->hojoseikou = 0;
    n->hojoaisyo = 0;
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545068);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_53);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_74);

s32 Flow6_54(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;

    nbAddNegoIdKouho(n, 0x60038);
    nbAddNegoIdKouho(n, 0x60037);
    n->nextnegoid = n->nextkouho[0];
    work = nbGetUnitWorkPointer(n->data, &n->data->party[n->hojopartyindex]);
    if (n->hojoskillid == 0x1A2 && work->id == 0x14) {
        n->temp_param[0] = 2;
        n->nextnegoid = n->nextkouho[1];
        n->temp_param[1] = 0x58;
    }
    if (n->hojoskillid == 0x1A3 && work->id == 0xF) {
        n->nextnegoid = n->nextkouho[1];
        n->temp_param[0] = 6;
        n->temp_param[1] = 0x59;
    }
    return 0;
}

s32 Flow6_55(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60038);
    nbDispNegoMessage(n, n->flowmsg[n->temp_param[0]], n->temp_param[1], 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_56);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_57);

s32 Flow6_58(nbNegoProcessData_t *n)
{
    n->requestdata /= 2;
    n->cnt6_58++;
    nbAddNegoIdKouho(n, 0x6003B);
    return 0;
}

s32 Flow6_59(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60047);
    nbAddNegoIdKouho(n, 0x60048);
    nbAddNegoIdKouho(n, 0x60049);
    if (n->cnt6_58 < 4) {
        if (n->cnt6_58 > 0) {
            n->nextnegoid = n->nextkouho[n->cnt6_58 - 1];
        }
    }
    return 0;
}

s32 Flow6_71(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003C;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5A, 0);
    return 0;
}

s32 Flow6_60(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x55, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_72(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003D;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5B, 0);
    return 0;
}

s32 Flow6_61(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x56, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_73(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003E;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5C, 0);
    return 0;
}

s32 Flow6_62(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x57, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_63(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x6004A);
        n->workid[6] = 0x60040;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x53, 0);
        break;
    }
    return 0;
}

s32 Flow6_64(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x54, 0);
    nbMakaDispOn(0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_65);

s32 Flow6_66(nbNegoProcessData_t *n)
{
    n->cnt6_66++;
    nbAddNegoIdKouho(n, 0x60043);
    nbAddNegoIdKouho(n, 0xA0001);
    if (n->cnt6_66 < 4) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_67(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, n->workid[2]);
    do {
        mes = dds3GetRandIntA(0, 4) + 0x21;
    } while (mes == n->mes6_67);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    n->mes6_67 = mes;
    return 0;
}

s32 Flow6_69(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    return 0;
}

s32 nbNegoFlowTwo(nbNegoProcessData_t *n)
{
    return D_004EF550[(u16)n->nownegoid](n);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_1);

s32 Flow9_2(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x90003);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xA7, 0);
        break;
    }
    return 0;
}

s32 Flow9_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90004);
    nbAddNegoIdKouho(n, 0x90006);
    if (datCheckStockFull() != 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow9_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90005);
    nbAddNegoIdKouho(n, 0x90009);
    printf(D_005450E0, datGetPlayerLevel(), n->ework->level);
    if (datGetPlayerLevel() < n->ework->level) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow9_5(nbNegoProcessData_t *n)
{
    n->actdata->unk_04 |= 8;
    nbAddNegoIdKouho(n, 0x90007);
    nbDispNegoMessage(n, n->helpmsg, 2, 0);
    return 0;
}

s32 Flow9_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x9000A);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xAB, 0);
    return 0;
}

s32 Flow9_7(nbNegoProcessData_t *n)
{
    return 9;
}

s32 Flow9_9(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x9000A);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xAC, 0);
    return 0;
}

s32 Flow9_10(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xAD, 0);
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005450E0);

s32 Flow9_11(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x9000D);
    nbAddNegoIdKouho(n, 0x90010);
    switch (n->negoreqtype) {
    case 1:
    case 5:
        n->nextnegoid = n->nextkouho[0];
        break;
    case 2:
        n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
        break;
    case 3:
    case 4:
        n->nextnegoid = n->nextkouho[1];
        break;
    default:
        n->negoreqtype = 1;
        break;
    }
    return 0;
}

s32 Flow9_13(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x9000E);
        n->getmakka = nbGetNegoYaruMakkaA(n, n->pwork, n->ework);
        if (n->negoskillid == 0x191) {
            n->getmakka = n->getmakka * 3 / 2;
        }
        nbSetMesMakka(n->flowmsg[n->ekucyo], n->getmakka);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xA8, 0);
        break;
    }
    return 0;
}

s32 Flow9_14(nbNegoProcessData_t *n)
{
    n->actdata->unk_04 |= 8;
    nbAddNegoIdKouho(n, 0x9000F);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow9_15(nbNegoProcessData_t *n)
{
    u8 *p;

    nbAddNegoIdKouho(n, 0xA001C);
    datAddMaka(n->getmakka);
    if (n->negoskillid == 0x191) {
        p = (u8 *)(datDevilTable[n->ework->id].race + (s32)DDS3_GBWK + 0x1CA8);
        if (*p < 0xFF) {
            *p = *p + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_16);

s32 Flow9_17(nbNegoProcessData_t *n)
{
    n->actdata->unk_04 |= 8;
    nbAddNegoIdKouho(n, 0x90012);
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow9_18(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow9_19(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x90014);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xAA, 0);
        break;
    }
    return 0;
}

/* NON_MATCHING: 8 of 43 words differ (first at +0x24). */
#ifdef NON_MATCHING
s32 Flow9_20(nbNegoProcessData_t *n)
{
    s32 phase;

    phase = n->nownegophase;
    switch (phase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x112, 0);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x113, 1);
        break;
    case 2:
        n->getinfotype = n->nowselect != 0 ? phase : 0;
        nbAddNegoIdKouho(n, 0x90015);
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_20);
#endif

s32 Flow9_21(nbNegoProcessData_t *n)
{
    n->actdata->unk_04 |= 8;
    nbAddNegoIdKouho(n, 0x90016);
    n->getinfomsg = nbGetInfoMsg(n->getinfotype);
    return 0;
}

s32 Flow9_22(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    nbDispNegoMessage(n, n->infomsg[n->ekucyo], n->getinfomsg, 0);
    return 0;
}

s32 nbNegoFlowOk(nbNegoProcessData_t *n)
{
    return D_004EF698[(u16)n->nownegoid](n);
}

s32 nbCheckFlowUnit(nbNegoProcessData_t *n)
{
    if (n->negoskillid == 0x18B) {
        return n->pwork->hp != 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_1);

s32 Flow10_2(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xA0003);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], dds3GetRandIntA(0, 2) + 0xD7, 0);
        break;
    }
    return 0;
}

s32 Flow10_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0006);
    nbAddNegoIdKouho(n, 0xA0004);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow10_4(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0xA0006);
    n->workid[0] = 0xA000A;
    n->workid[1] = 0xA0005;
    r = nbSearchHojoSkillWork(n, 1);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow10_5(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xA0006);
        nbCameraSetTarget(n->eform);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDA, 0);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_7);

s32 Flow10_8(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDB, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 4;
        return 0;
    }
    return 4;
}

s32 Flow10_9(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xDF, 0);
    if (nbCheckFlowUnit(n) == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = 4;
        return 0;
    }
    return 4;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_33);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_10);

s32 Flow10_11(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA000C);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE0, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_12);

s32 Flow10_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0010);
    nbAddNegoIdKouho(n, 0xA0010);
    nbAddNegoIdKouho(n, 0xA0011);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 3)];
    return 0;
}

s32 Flow10_15(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0012);
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    nbCameraSetTarget(n->eform);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->getmakka);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE2, 0);
    return 0;
}

s32 Flow10_16(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0013);
    nbAddNegoIdKouho(n, 0xA0011);
    n->getitem = nbGetNegoYaruItemB(n->ework);
    if (n->getitem == 0) {
        n->getitem = 0x24;
    }
    if (datCheckItemFull(n->getitem) != 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    n->nextnegoid = n->nextkouho[0];
    nbCameraSetTarget(n->eform);
    nbSetMesItem(n->flowmsg[n->ekucyo], n->getitem);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE1, 0);
    return 0;
}

s32 Flow10_17(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0014);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE3, 0);
    return 0;
}

s32 Flow10_18(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001E);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow10_19(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001F);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow10_20(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0020);
    nbClsNegoMessage(n);
    nbPanelDraw(1);
    nbMakeKaifukuPacket(0, 0, n->pform, 0, n->pwork->maxhp, -1, 0);
    nbAddHitEffKoukaPacket(0, 0, n->pform, 8);
    n->waitframe = 0;
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_21);

s32 Flow10_22(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEA, 0);
    return 0;
}

s32 Flow10_23(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_25);

s32 Flow10_26(nbNegoProcessData_t *n)
{
    s32 r;

    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE5, 0);
    r = nbCheckFlowUnit(n);
    if (r == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = r;
        return 0;
    }
    return 1;
}

s32 Flow10_27(nbNegoProcessData_t *n)
{
    s32 r;

    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE9, 0);
    r = nbCheckFlowUnit(n);
    if (r == 1) {
        nbAddNegoIdKouho(n, 0xA0021);
        n->temp_param[0] = r;
        return 0;
    }
    return 1;
}

s32 Flow10_28(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xE4, 0);
    return 0;
}

s32 Flow10_29(nbNegoProcessData_t *n)
{
    return 2;
}

s32 Flow10_30(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow10_31(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow10_32(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0016);
    nbDispNegoMessage(n, n->helpmsg2, 7, 0);
    nbPanelDraw(0);
    n->waitframe = 60;
    return 0;
}

s32 nbNegoFlowNg(nbNegoProcessData_t *n)
{
    return D_004EF6F8[(u16)n->nownegoid](n);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_2);

s32 Flow2_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x20004);
    return 0;
}

s32 Flow2_4(nbNegoProcessData_t *n)
{
    u8 *p;
    s32 q;

    nbAddNegoIdKouho(n, 0x20005);
    q = nbGetQuestionType(n->ework);
    n->qtype = q;
    p = (u8 *)(q + (s32)DDS3_GBWK + 0x1CE8);
    if (*p < 0xFF) {
        *p = *p + 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_6);

s32 Flow2_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x20008);
    n->waitframe = nbSetNegoYesMotion(n, n->eform);
    return 0;
}

s32 Flow2_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x20009);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_10);

s32 Flow2_11(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow2_12(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90007);
    nbDispNegoMessage(n, n->helpmsg, 9, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_13);

s32 Flow2_14(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x2000F);
    nbAddNegoIdKouho(n, 0x20010);
    nbCameraSetTarget(n->eform);
    if (dds3GetRandIntA(0, 2) == 0) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x2F, 0);
        n->nextnegoid = n->nextkouho[1];
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x2E, 0);
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow2_15(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow2_16(nbNegoProcessData_t *n)
{
    if (n->negoskillid == 0x18B) {
        if (n->pwork->hp != 0) {
            nbAddNegoIdKouho(n, 0xA0021);
            n->temp_param[0] = 4;
            return 0;
        }
        return 4;
    }
    return 4;
}

s32 nbNegoFlowFullMoon(nbNegoProcessData_t *n)
{
    return D_004EF780[(u16)n->nownegoid](n);
}

s32 Flow3_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x30002);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x30, 0);
    return 0;
}

s32 Flow3_2(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 2;
}

s32 nbNegoFlowStock(nbNegoProcessData_t *n)
{
    return D_004EF7C8[(u16)n->nownegoid](n);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_1);

s32 Flow4_2(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x40003);
    nbAddNegoIdKouho(n, 0x4000C);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    return 0;
}

s32 Flow4_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x40004);
    return 0;
}

s32 Flow4_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x40005);
    nbAddNegoIdKouho(n, 0x40006);
    nbAddNegoIdKouho(n, 0x40007);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 3)];
    return 0;
}

s32 Flow4_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x40008);
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->getmakka);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x33, 0);
    return 0;
}

s32 Flow4_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x40009);
    nbAddNegoIdKouho(n, 0x40005);
    n->getitem = nbGetNegoYaruItemB(n->ework);
    if (n->getitem == 0) {
        n->getitem = 0x12;
    }
    if (datCheckItemFull(n->getitem)) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    nbSetMesItem(n->flowmsg[n->ekucyo], n->getitem);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x34, 0);
    return 0;
}

s32 Flow4_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x4000A);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x35, 0);
    return 0;
}

s32 Flow4_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x4000B);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow4_9(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x4000B);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow4_10(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x4000B);
    nbClsNegoMessage(n);
    nbPanelDraw(1);
    nbMakeKaifukuPacket(0, 0, n->pform, 0, n->pwork->maxhp, -1, 0);
    nbAddHitEffKoukaPacket(0, 0, n->pform, 8);
    n->waitframe = 0;
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_11);

s32 Flow4_12(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x4000D);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x32, 0);
    return 0;
}

s32 Flow4_13(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 2;
}

s32 nbNegoFlowDouchara(nbNegoProcessData_t *n)
{
    return D_004EF7D8[(u16)n->nownegoid](n);
}

s32 Flow5_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x50002);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_3);

s32 Flow5_4(nbNegoProcessData_t *n)
{
    switch (D_003E315E[n->innen_id].mes) {
    case 0:
        nbAddNegoIdKouho(n, 0x50006);
        n->nextnegoid = n->nextkouho[0];
        break;
    case 1:
        return 4;
    }
    return 0;
}

s32 Flow5_5(nbNegoProcessData_t *n)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_6);

s32 Flow5_7(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_8(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_9(nbNegoProcessData_t *n)
{
    if (D_003E3158[n->innen_id].phase[n->nownegophase].item == 0) {
        return 2;
    }
    n->getitem = D_003E3158[n->innen_id].phase[n->nownegophase].item;
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 8, 0);
    return 0;
}

s32 Flow5_10(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_11(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_12(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_13(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_14(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->innenmsg, D_003E317E[n->innen_id].mes, 0);
    return 6;
}

s32 Flow5_15(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x50012);
    nbDispNegoMessage(n, n->innenmsg, D_003E317C[n->innen_id].mes, 0);
    return 0;
}

s32 Flow5_16(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_17(nbNegoProcessData_t *n)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_18);

s32 Flow5_19(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_20(nbNegoProcessData_t *n)
{
    return 0;
}

s32 Flow5_21(nbNegoProcessData_t *n)
{
    if (D_003E3158[n->innen_id].phase[n->nownegophase].item == 0) {
        return 6;
    }
    n->getitem = D_003E3158[n->innen_id].phase[n->nownegophase].item;
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 nbNegoFlowInnen(nbNegoProcessData_t *n)
{
    return D_004EF810[(u16)n->nownegoid](n);
}

s32 Flow7_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x70003);
    return 0;
}

s32 Flow7_3(nbNegoProcessData_t *n)
{
    u8 *p;

    nbAddNegoIdKouho(n, 0x7000F);
    nbAddNegoIdKouho(n, 0x7000B);
    n->qtype = nbGetQuestionType(n->ework);
    p = (u8 *)(n->qtype + (s32)DDS3_GBWK + 0x1CE8);
    if (*p < 0xFF) {
        *p = *p + 1;
    }
    if (n->qtype == 0) {
        n->qindex = dds3GetRandIntA(0, 5);
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->qindex = dds3GetRandIntA(0, 2);
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_24);

s32 Flow7_25(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[4]);
        nbDispNegoMessage(n, n->questionmsg, 0x27, 0);
        break;
    }
    return 0;
}

s32 Flow7_26(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, n->workid[5]);
        n->workid[2] = 0x60045;
        nbDispNegoMessage(n, n->questionmsg, 0x28, 0);
        break;
    }
    return 0;
}

s32 nbNegoFlowQuestion(nbNegoProcessData_t *n)
{
    return D_004EF868[(u16)n->nownegoid](n);
}

s32 Flow8_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80002);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x9F, 0);
    return 0;
}

s32 Flow8_2(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80003);
    return 0;
}

s32 Flow8_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80004);
    nbAddNegoIdKouho(n, 0x80008);
    nbAddNegoIdKouho(n, 0x8000B);
    return 0;
}

s32 Flow8_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80005);
    return 0;
}

s32 Flow8_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80006);
    nbAddNegoIdKouho(n, 0x80007);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    return 0;
}

s32 Flow8_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90001);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xA0, 0);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_7);

s32 Flow8_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80009);
    return 0;
}

s32 Flow8_9(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x8000A);
    nbAddNegoIdKouho(n, 0x80013);
    return 0;
}

s32 Flow8_10(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x8000E);
    nbAddNegoIdKouho(n, 0x8000D);
    return 0;
}

s32 Flow8_11(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x8000C);
    nbAddNegoIdKouho(n, 0x8000D);
    return 0;
}

s32 Flow8_12(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], dds3GetRandIntA(0, 2) + 0xA4, 0);
        return 1;
    }
    return 0;
}

s32 Flow8_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80011);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xA6, 0);
    return 0;
}

s32 Flow8_14(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x8000F);
    nbAddNegoIdKouho(n, 0x80010);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_15);

s32 Flow8_16(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xA2, 0);
    return 1;
}

s32 Flow8_17(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80012);
    return 0;
}

s32 Flow8_18(nbNegoProcessData_t *n)
{
    nbSetMesMakka(n->helpmsg, 6600);
    nbDispNegoMessage(n, n->helpmsg, 4, 0);
    return 1;
}

s32 Flow8_19(nbNegoProcessData_t *n)
{
    return 4;
}

s32 nbNegoFlowFight(nbNegoProcessData_t *n)
{
    return D_004EF8D8[(u16)n->nownegoid](n);
}

s32 Flow11_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xB0002);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_3);

s32 Flow11_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xB0006);
    nbAddNegoIdKouho(n, 0xB0005);
    nbPanelPartyHide();
    printf(D_00545208, n->pwork->hp);
    if (n->pwork->hp == 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow11_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x80013);
    nbAddNegoIdKouho(n, 0xB0009);
    if (n->pwork->flag & 4) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow11_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xB0007);
    nbAddNegoIdKouho(n, 0xB0008);
    if (n->requesthpmp == 1) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow11_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xB0009);
    nbAddNegoIdKouho(n, 0x60018);
    if (n->pwork->mp != 0) {
        n->nextnegoid = n->nextkouho[1];
    } else if (n->requestdata < 0x32) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow11_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60018);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], dds3GetRandIntA(0, 2) == 0 ? 0x99 : 0x9C, 0);
    return 0;
}

s32 Flow11_9(nbNegoProcessData_t *n)
{
    u32 prev;
    s32 mes;

    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xA0001);
        nbAddNegoIdKouho(n, 0xA0018);
        mes = n->requesthpmp == 0 ? 0x9D : 0x9E;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        prev = nbGetPrevNegoId(n);
        switch (prev) {
        case 0xB0005:
            n->nextnegoid = n->nextkouho[1];
            break;
        case 0xB0007:
            n->nextnegoid = n->nextkouho[0];
            break;
        }
        break;
    }
    return 0;
}

s32 nbNegoFlowDrain(nbNegoProcessData_t *n)
{
    return D_004EF928[(u16)n->nownegoid](n);
}

s32 Flow14_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0xE0002;
    return 0;
}

s32 Flow14_2(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x1001E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x110, 0);
        break;
    }
    return 0;
}

s32 nbNegoFlowBinjo(nbNegoProcessData_t *n)
{
    return D_004EF950[(u16)n->nownegoid](n);
}

s32 Flow12_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0xC0002;
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_4);

s32 Flow12_24(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC0019);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xED, 0);
        break;
    }
    return 0;
}

s32 Flow12_25(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow12_16(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEE, 0);
        break;
    }
    return 0;
}

s32 Flow12_17(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xEF, 0);
        break;
    }
    return 0;
}

s32 Flow12_18(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF0, 0);
        break;
    }
    return 0;
}

s32 Flow12_19(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF1, 0);
        break;
    }
    return 0;
}

s32 Flow12_20(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF2, 0);
        break;
    }
    return 0;
}

s32 Flow12_21(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF3, 0);
        break;
    }
    return 0;
}

s32 Flow12_22(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF4, 0);
        break;
    }
    return 0;
}

s32 Flow12_23(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC002E);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF5, 0);
        break;
    }
    return 0;
}

s32 Flow12_26(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC001B);
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545208);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_27);

s32 Flow12_28(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC001D);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF9, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x105, 0);
    }
    return 0;
}

s32 Flow12_29(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD4, 1);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001E);
        nbAddNegoIdKouho(n, 0xC0026);
        n->nextnegoid = n->nextkouho[n->nowselect];
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_30);

s32 Flow12_31(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 2, 0);
    return 9;
}

s32 Flow12_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0024);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF6, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x102, 0);
    }
    return 0;
}

s32 Flow12_36(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0025);
    if (dds3GetRandIntA(0, 2) == 0) {
        n->getinfotype = 0;
    } else {
        n->getinfotype = 2;
    }
    n->getinfomsg = nbGetInfoMsg(n->getinfotype);
    return 0;
}

s32 Flow12_37(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    nbDispNegoMessage(n, n->infomsg[n->ekucyo], n->getinfomsg, 0);
    return 0;
}

s32 Flow12_38(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0027);
    if (dds3GetRandIntA(0, 2) == 0) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x108, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x109, 0);
    }
    return 0;
}

s32 Flow12_39(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 0;
}

s32 Flow12_40(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0029);
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF7, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x103, 0);
    }
    n->getmakka = nbGetNegoYaruMakkaB(n, n->pwork, n->ework);
    return 0;
}

s32 Flow12_41(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002A);
    nbSetMesMakka(n->helpmsg, n->getmakka);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow12_42(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    datAddMaka(n->getmakka);
    return 0;
}

s32 Flow12_43(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002C);
    nbAddNegoIdKouho(n, 0xC0028);
    n->getitem = nbGetNegoYaruItemB(n->ework);
    if (n->getitem == 0) {
        n->getitem = 1;
    }
    if (datCheckItemFull(n->getitem) != 0) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    if (n->negotype != 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xF8, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x104, 0);
    }
    return 0;
}

s32 Flow12_44(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002D);
    nbSetMesItem(n->helpmsg, n->getitem);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow12_45(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    datAddItem(n->getitem, 1);
    return 0;
}

s32 Flow12_46(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC002F);
    return 0;
}

s32 Flow12_47(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xC0030);
    nbAddNegoIdKouho(n, 0xC0031);
    if (dds3GetRandIntA(0, 2) == 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow12_48(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    return 0;
}

s32 Flow12_49(nbNegoProcessData_t *n)
{
    return 4;
}

s32 nbNegoFlowMukan(nbNegoProcessData_t *n)
{
    return D_004EF960[(u16)n->nownegoid](n);
}

s32 Flow13_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0xD0002;
    return 0;
}

s32 Flow13_2(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xD0003);
    nbAddNegoIdKouho(n, 0xD0004);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    if (datGetRace(n->ework) == 0x15 || datGetRace(n->ework) == 0x16 ||
        datGetRace(n->ework) == 0x17) {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow13_3(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x70001);
    n->workid[4] = 0xC001A;
    n->workid[5] = 0xD000B;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xFE, 0);
    return 0;
}

s32 Flow13_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xD0005);
    n->requesttype = 0;
    n->requestdata = nbGetKureItemA(n, n->pwork, n->ework);
    nbSetMesItem(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xFF, 0);
    return 0;
}

s32 Flow13_5(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD3, 1);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xD0006);
        nbAddNegoIdKouho(n, 0xD0007);
        if (n->nowselect == 0) {
            n->nextnegoid = n->nextkouho[0];
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        break;
    }
    return 0;
}

s32 Flow13_6(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xD0009);
    nbAddNegoIdKouho(n, 0xD0008);
    if (datCheckItem(n->requestdata, 1) != 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow13_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xD000A);
    nbAddNegoIdKouho(n, 0xD000C);
    n->nextnegoid = n->nextkouho[dds3GetRandIntA(0, 2)];
    return 0;
}

s32 Flow13_12(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x116, 0);
    return 0;
}

/* NON_MATCHING: 4 of 46 words differ (first at +0x18). */
#ifdef NON_MATCHING
s32 Flow13_8(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 2:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x12B, 0);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x51, 0);
        break;
    case 3:
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_8);
#endif

s32 Flow13_9(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xC001A);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x101, 0);
        datAddItem(n->requestdata, -1);
        break;
    }
    return 0;
}

s32 Flow13_10(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x100, 0);
        break;
    case 2:
        return 1;
    }
    return 0;
}

s32 Flow13_11(nbNegoProcessData_t *n)
{
    return 1;
}

s32 nbNegoFlowSensei(nbNegoProcessData_t *n)
{
    return D_004EFA28[(u16)n->nownegoid](n);
}

s32 Flow15_1(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x1001F);
    n->workid[3] = 0xF0005;
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_6);

s32 Flow15_7(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0xD2, 1);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xF0008);
        nbAddNegoIdKouho(n, 0xF0017);
        if (n->nowselect == 0) {
            n->nextnegoid = n->nextkouho[0];
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_10);

s32 Flow15_11(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 2, 0);
    return 3;
}

s32 Flow15_12(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xF0011);
    nbDispNegoMessage(n, n->helpmsg, 3, 0);
    return 0;
}

s32 Flow15_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xF0012);
    nbDispNegoMessage(n, n->helpmsg, 5, 0);
    return 0;
}

s32 Flow15_14(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xF0013);
    if (dds3GetRandIntA(0, 2) == 0) {
        n->getinfotype = 0;
    } else {
        n->getinfotype = 2;
    }
    n->getinfomsg = nbGetInfoMsg(n->getinfotype);
    return 0;
}

s32 Flow15_17(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    datAddMaka(n->requestdata);
    return 0;
}

s32 Flow15_18(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    if ((u32)(n->requestdata - 1) < 0x9F) {
        datAddItem(n->requestdata, 1);
    }
    return 0;
}

s32 Flow15_19(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001C);
    nbDispNegoMessage(n, n->infomsg[n->ekucyo], n->getinfomsg, 0);
    return 0;
}

s32 Flow15_20(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        break;
    case 1:
        nbAddNegoIdKouho(n, 0xF0016);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x10E, 0);
        break;
    }
    return 0;
}

s32 Flow15_22(nbNegoProcessData_t *n)
{
    return 4;
}

s32 Flow15_23(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x115, 0);
    return 7;
}

s32 nbNegoFlowInoti(nbNegoProcessData_t *n)
{
    return D_004EFA60[(u16)n->nownegoid](n);
}

s32 Flow16_1(nbNegoProcessData_t *n)
{
    s32 race;

    nbAddNegoIdKouho(n, 0x100002);
    nbAddNegoIdKouho(n, 0x100003);
    nbAddMojiEffKoukaPacket(0, 0x1E, (s32)n->actdata, n->eform, 0);
    nbCameraSetBougai(n->eform);
    race = datGetRace(n->ework);
    if (race < 0x18) {
        if (race >= 0x15) {
            n->nextnegoid = n->nextkouho[1];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow16_2);

s32 Flow16_3(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x12C, 0);
    return 4;
}

s32 nbNegoFlowKoketa(nbNegoProcessData_t *n)
{
    return D_004EFAC0[(u16)n->nownegoid](n);
}

void nbPnlLoad(void)
{
    hnbPnlFile1 = sdfDevLoadFileEx(D_00545230, &pnbPnlFile1, 0);
    hnbPnlFile2 = sdfDevLoadFileEx(D_00545250, &pnbPnlFile2, 0);
    hnbPnlFile3 = sdfDevLoadFileEx(D_00545270, &pnbPnlFile3, 0);
    hnbPnlFile4 = sdfDevLoadFileEx(D_00545290, &pnbPnlFile4, 0);
}

void nbPnlTexSet(nbMainProcessData_t *data)
{
    data->pnltex[0] = sdfTexSet(pnbPnlFile1);
    data->pnltex[1] = sdfTexSet(pnbPnlFile2);
    data->pnltex[2] = sdfTexSet(pnbPnlFile3);
    data->pnltex[3] = sdfTexSet(pnbPnlFile4);
}

void nbPnlTexDel(nbMainProcessData_t *data)
{
    sdfTexFreeDelayed(data->pnltex[0]);
    sdfTexFreeDelayed(data->pnltex[1]);
    sdfTexFreeDelayed(data->pnltex[2]);
    sdfTexFreeDelayed(data->pnltex[3]);
    data->pnltex[0] = NULL;
    data->pnltex[1] = NULL;
    data->pnltex[2] = NULL;
    data->pnltex[3] = NULL;
}

void nbPnl_setOt(s32 ofs)
{
    gnbPnlOtOfs = ofs;
}

void nbPnl_rsetTex1(s32 tex, s32 ctx)
{
    sdfDrawTag_t *list;
    u64 *buff;
    u64 *packet;
    sdfGrOt_t *ot;

    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    buff = sdfGrAllocPacketBuff(0x30);
    packet = sdf2DMakeStdPacket(buff, 0x30);
    packet[4] = (ctx << 6) | (tex << 5);
    packet[5] = 0x14;
    sdfDrawTagLink(list, buff);
    ot = &sdfGrOtTable[gnbPnlOtOfs];
    ot->regFunc(ot, list);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnl_rsetTest);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545230);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545250);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545270);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545290);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnl_rsetAlpha);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnlSprite);

void nbPanelBaseColorSet(u32 col)
{
    nbPanelBaseABGR = col;
    nbPanelBaseRGBA = (col >> 24) | ((col >> 8) & 0xFF00) | ((col & 0xFF00) << 8) | (col << 24);
}

void nbPanelAllShow(void)
{
    nbPanelPressGaugeShow();
    nbPanelPartyShow();
    nbPanelAutoShow();
}

void nbPanelAllHide(void)
{
    nbPanelAutoHide();
    nbPanelPartyHide();
    nbPanelPressGaugeHide();
}

void nbPanelSpriteCenteringDraw(s32 x, s32 y, nbPanelXYZ_t *p, nbPanelXYZ_t *area,
                                nbPanelUV_t *uv, u32 abgr, sdfTexHandle_t *th)
{
    s32 cx;
    u32 col;

    cx = p->x + ((area->w - p->w) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(cx + x, p->y + y, p->w, p->h, uv->u, uv->v, uv->uw, uv->vh, col, th);
}

void nbPanelSpriteScaleDraw(s32 x, s32 y, nbPanelXYZ_t *p, nbPanelUV_t *uv, u32 abgr, f32 scale,
                            sdfTexHandle_t *th)
{
    s32 sw;
    s32 sh;
    s32 px;
    s32 py;
    u32 col;

    sw = p->w * scale;
    sh = p->h * scale;
    px = p->x - ((sw - p->w) >> 1);
    py = p->y - ((sh - p->h) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(px + x, py + y, sw, sh, uv->u, uv->v, uv->uw, uv->vh, col, th);
}

void nbPanelSpriteScaleWHDraw(s32 x, s32 y, nbPanelXYZ_t *p, nbPanelUV_t *uv, u32 abgr, f32 sclw,
                              f32 sclh, sdfTexHandle_t *th)
{
    s32 sw;
    s32 sh;
    s32 px;
    s32 py;
    u32 col;

    sw = p->w * sclw;
    sh = p->h * sclh;
    px = p->x - ((sw - p->w) >> 1);
    py = p->y - ((sh - p->h) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(px + x, py + y, sw, sh, uv->u, uv->v, uv->uw, uv->vh, col, th);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSpriteBlendDraw);

void nbPanelSmallStringDraw(s32 x, s32 y, nbPanelXYZ_t *p, u32 abgr, char **str)
{
    _FRQ *frq;
    u32 col;

    col = effMagaCylinderCalcColor(abgr, nbPanelBaseRGBA);
    frq = frwGetSmallNumFontFRQ((p->x + x) << 4, (p->y + y) << 3, 0, col, str, 0);
    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSmallNameStringDraw);

void nbCenterText(_FRQ *frq, nbTextBox_t *box)
{
    frSetDrawPosition(frq, frq->x + (((box->width - frGetStrWidth2(frq)) >> 1) << 4), frq->y);
}

u32 nbPanelHPColor(datUnitWork_t *w)
{
    s32 rate;

    if (datCheckDying(w)) {
        return 0x754B3D80;
    }
    rate = w->hp * 100 / w->maxhp;
    if (rate < 25) {
        return 0x754B3D80;
    }
    return rate < 50 ? 0xB4A06480 : 0xA09DC380;
}

u32 nbPanelMPColor(datUnitWork_t *unit)
{
    s32 pct;

    pct = unit->mp * 100 / unit->maxmp;
    if (pct < 0x19) {
        return 0x754B3D80;
    }
    return pct < 0x32 ? 0xB4A06480 : 0xA09DC380;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelCursorDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonScaleDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonFlashDraw);

void nbPanelDrawPP0(s32 x, s32 y, nbMainProcessData_t *data, s32 index)
{
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17, nbPanelBaseABGR, data->pnltex[0]);
    nbPnlSprite(x + 0x36, y, 0xE4, 0x17, 0x69, 1, 1, 0x17, nbPanelBaseABGR, data->pnltex[0]);
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545300);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawPP1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawPP);

void nbPanelPressGaugeInit(void)
{
    nbPanelPGShow = 1;
    nbPanelPGNumber = 0;
    nbPanelPGFrame = 0;
    nbPanelPGBirthWait = 0;
    nbPanelPGStart = 0;
    nbPanelPGEncount = 0;
    nbPanelPGTurn = -1;
    nbPanelPGTurnOld = -1;
    nbPanelPGBaseColor = 0x80808080;
}

void nbPanelPressGaugeStart(void)
{
    nbPanelPGStart = 1;
    nbPanelPGNumber = 0;
    nbPanelPGFrame = 0;
    nbPanelPGBirthWait = 0;
    nbPanelPGTurn = -1;
    nbPanelPGTurnOld = -1;
}

void nbPanelPressGaugeReset(void)
{
    printf(D_00545380);
    nbPanelPressGaugeStart();
    nbPanelPressTurnGaugeAnimStart2();
    nbPanelPressTurnOmit();
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545380);

void nbPanelPressGaugeShow(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (!nbPanelPGShow && !nbPanelPGEncount) {
        printf("nbPanelPressGaugeShow : Reset\n");
        nbPanelPGPress = data->stat & 8;
        nbPanelPGFrame = 0;
        nbPanelPGNumber = data->pressturn;
    }
    nbPanelPGEncount = 0;
    nbPanelPGShow = 1;
    nbPanelPGBaseColor = 0x80808080;
}

void nbPanelPressGaugeHide(void)
{
    nbPanelPGShow = 0;
}

void nbPanelPressGaugeEncountHide(void)
{
    nbPanelPGShow = 0;
    nbPanelPGEncount = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGauge);

void nbPanelPressTurnInit(void)
{
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTAProcess = 0;
    nbPanelPTP_1_Process = 0;
    nbPanelPTAOnly = 0;
    nbPanelPTOmit = 0;
}

s32 nbPanelPressTurnRun(void)
{
    s32 run;

    run = 0;
    if (nbPanelPT_1_Process != 0 || nbPanelPT_2_Process != 0 || nbPanelPT_3_Process != 0 ||
        nbPanelPTAProcess != 0 || nbPanelPTP_1_Process != 0) {
        run = 1;
    }
    return run;
}

s32 nbPanelPressTurnGaugeRun(void)
{
    return nbPanelPTAProcess != 0;
}

void nbPanelPressTurnOmit(void)
{
    nbPanelPTOmit = 1;
}

void nbPanelPressTurnStart(void)
{
    printf(D_005453B8);
    nbGetMainProcessData();
    if (nbPanelPTOmit == 0) {
        nbPanelPT_1_Process = 1;
        nbPanelPT_1_Frame = 0;
        nbPanelPT_2_Process = 1;
        nbPanelPT_2_Frame = 0;
        nbPanelPT_2_Rate = 0.1f;
        nbPanelPT_3_Process = 1;
        nbPanelPT_3_Frame = 0;
        nbPanelPT_3_Rate = 0.1f;
        nbPanelPTP_0_Frame = 0;
        nbPanelPTP_0_Process = 0;
        nbPanelPTP_1_Frame = 0;
        nbPanelPTP_1_Process = 0;
        if (nbPanelPGTurn != nbPanelPGTurnOld) {
            nbPanelPTAProcess = 1;
            nbPanelPTAFrame = 0;
            nbPanelPTAOnly = 0;
        }
        nbPanelPGTurnOld = nbPanelPGTurn;
    } else {
        nbPanelPTOmit = 0;
    }
}

void nbPanelPressTurnGaugeAnimStart(void)
{
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTAProcess = 1;
    nbPanelPTAOnly = 1;
    nbPanelPTAFrame = 0;
}

void nbPanelPressTurnGaugeAnimStart2(void)
{
    nbPanelPTAFrame = 4;
    nbPanelPTAProcess = 3;
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTARate_S = 0.1f;
    nbPanelPTARate_E = 0.1f;
    nbPanelPTAOnly = 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005453B8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPartySelect);

void nbPanelStockInit(void)
{
    nbPanelStockShiftX = 300;
    nbPanelStockABGR = 0x80808080;
    nbPanelStockRun = 0;
    nbPanelStockPad = 0;
    nbPanelStockFadeFlag = 0;
    nbPanelStockFrame = 0;
}

s32 nbPanelStockCheckRun(void)
{
    return nbPanelStockRun;
}

void nbPanelStockEnd(void)
{
    nbPanelStockRun = 0;
    nbPanelStockPad = 0;
}

void nbPanelStockFadeOUT(void)
{
    nbPanelStockFadeFlag = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelStockSelectInit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelStockSelectCtrl);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelStockSelect);

void nbPanelAnalyzeInit(void)
{
    nbPanelAnalyzeProcess = 0;
    nbPanelAnalyzeFrame = 0;
    pNbPanelAnalyzeUnitWork = NULL;
}

void nbPanelAnalyzeStart(void)
{
    nbPanelAnalyzeFrame = 0;
    nbPanelAnalyzeProcess = 1;
}

void nbPanelAnalyzeEnd(void)
{
    nbPanelAnalyzeFrame = 0;
    nbPanelAnalyzeProcess = 4;
}

void nbPanelAnalyzeDraw(datUnitWork_t *unit)
{
    pNbPanelAnalyzeUnitWork = unit;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelAnalyzeRun);

void nbPanelAutoShow(void)
{
    PanelAutoShow = 1;
    PanelAutoBaseColor = 0x80808080;
}

void nbPanelAutoHide(void)
{
    PanelAutoShow = 0;
}

s32 nbPanelAutoCheckShow(void)
{
    return PanelAutoShow;
}

void nbPanelAutoInit(void)
{
    PanelAutoABGR = 0x40808080;
    PanelAutoBaseColor = 0x80808080;
    PanelAutoShow = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelAuto);

void nbPanelDraw(s32 show)
{
    if (show) {
        nbPanelAllShow();
    } else {
        nbPanelAllHide();
    }
    nbPnlDrawFlag = 1;
}

s32 nbPanelRunProcess(dds3ProcessID_t *id)
{
    nbPnlCnt += 0.2f;
    return 0;
}

void nbPanelPartyShow(void)
{
    PanelPartyShow = 1;
}

void nbPanelPartyHide(void)
{
    PanelPartyShow = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPartyDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawProcess);

void nbPanelShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    nbPnlTexDel(data);
    data->pnlproc = NULL;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitPanelProcess);

void nbPanelFlashInit(nbPanelFlash_t *p, s32 a, s32 b, s32 c, f32 x, f32 y, s32 d)
{
    p->unk_04 = 1;
    p->unk_00 = 0;
    p->unk_0C = 1;
    p->unk_08 = 0;
    p->unk_10 = 0.1f;
    p->unk_14 = 0;
    p->unk_18 = 1;
    p->unk_1C = 0.1f;
    p->unk_28 = x;
    p->unk_2C = y;
    p->unk_20 = a;
    p->unk_24 = b;
    p->unk_30 = c;
    p->unk_34 = d;
}

/* vu0 routine: unpack two colours, multiply and pack */
u32 nbPanelFlashRGBAMulti(u32 a, u32 b)
{
    nbColorWork_t work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = a;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = b;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelFlashDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDebugStockSelect);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", DispBadStatus);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005454F0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545508);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545518);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545528);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545538);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545548);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545558);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545568);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545578);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545588);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545598);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005455A8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005455B8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", nbDebugUnitLabelTable);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", DispHpMpBad2);

void DispHpMpBad(nbDebugWork_t *d)
{
    nbFormation_t *form;
    s32 i;

    form = NULL;
    for (i = 0; i < 0x20; i++) {
        if (d->data->form[i].anime & 8) {
            form = &d->data->form[i];
            break;
        }
    }
    if (form != NULL) {
        DispHpMpBad2(d, form);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMenu);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectHpMp);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectBad);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMotData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectDevil);

void SetSelect(void)
{
    nbSetTargetProcess(0, 0, 0, 0, 3);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetDvlChange);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetTarget_nbDebugProcess);

void BackUpHpMpBad(nbDebugWork_t *d)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(d->form->formindex);
    d->hp = unit->hp;
    d->mp = unit->mp;
    d->bad = unit->badstatus;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545698);

void SetNextSeq(nbDebugWork_t *d)
{
    s32 seq;

    seq = d->unk_08;
    d->unk_08 = -1;
    d->unk_04 = seq;
    switch (seq) {
    case 1:
        SetSelect();
        break;
    case 3:
        SetDvlChange(d);
        break;
    case 2:
    case 4:
    case 8:
    case 9:
        break;
    case 5:
    case 6:
    case 7:
        BackUpHpMpBad(d);
        seq = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDebugProcess1);

void nbDebugShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    sdfChipFreeDirect(dds3GetProcessData(id));
    data->debugproc = NULL;
}

nbDebugWork_t *InitDebugProcessData(nbMainProcessData_t *data)
{
    nbDebugWork_t *w;

    w = sdfChipAllocDirect(sizeof(nbDebugWork_t));
    memset(w, 0, sizeof(nbDebugWork_t));
    w->data = data;
    w->unk_04 = 0;
    w->unk_14 = 0;
    w->form = NULL;
    menucur = 0;
    return w;
}

void nbInitDebugProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00545728, 0x3F9, 1, 1, nbDebugProcess1, nbDebugShutdown,
                          InitDebugProcessData(data));
    dds3LinkProcess(data->mainproc, id);
    data->debugproc = id;
}

s32 Chk_MYHP(nbActionProcessData_t *a, s32 hp)
{
    if (a->work->hp * 100 / a->work->maxhp <= hp) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_MYMP(nbActionProcessData_t *a, s32 mp)
{
    if (a->work->mp <= mp) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_FRHP(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (n < work->hp * 100 / work->maxhp) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_DMGCNT_285578(nbActionProcessData_t *a, s32 n)
{
    if (nbGetHojoCounter(a->form->formindex, 0) >= n) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_CHKCNT_2855D8(nbActionProcessData_t *a, s32 n)
{
    s32 cnt;

    cnt = nbGetHojoCounter(a->form->formindex, a->aijokentbl + 1);
    if (cnt < 127) {
        cnt++;
    }
    nbSetHojoCounter(a->form->formindex, a->aijokentbl + 1, cnt);
    if (cnt < n) {
        return 0;
    }
    a->aiselect = 1 << a->form->formindex;
    return 1;
}

s32 Chk_FRCNT(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (n >= a->data->enemypcnt) {
        party = &a->data->party[4];
        for (i = 9; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_ENCNT(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (n >= a->data->playerpcnt) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_MYBAD(nbActionProcessData_t *a, s32 badstat)
{
    if ((a->work->badstatus & 0xFFF) == badstat) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_FRBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENALLBAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 cnt;
    u32 bit;
    s32 one;

    one = 1;
    cnt = 0;
    bit = 0;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if ((nbGetUnitWorkPointer(a->data, &a->data->party[i])->badstatus & 0xFFF) != n) {
            continue;
        }
        cnt++;
        bit |= one << a->data->party[i].formindex;
    }
    if (cnt == a->data->playerpcnt) {
        a->aiselect = bit;
        return 1;
    }
    return 0;
}

s32 Chk_ENID(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (work->id != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_FRID(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (i == a->partyindex) {
            continue;
        }
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (nbGetUnitWorkPointer(a->data, &a->data->party[i])->id != n) {
            continue;
        }
        a->aiselect |= one << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545728);

s32 ChkAiHojo(s32 formindex, s32 hojotype)
{
    switch (hojotype) {
    case 0:
        if (nbGetHojoCounter(formindex, 4) > 0) {
            return 1;
        }
        break;
    case 1:
        if (nbGetHojoCounter(formindex, 4) < 0) {
            return 1;
        }
        break;
    case 2:
        if (nbGetHojoCounter(formindex, 5) > 0) {
            return 1;
        }
        break;
    case 3:
        if (nbGetHojoCounter(formindex, 5) < 0) {
            return 1;
        }
        break;
    case 4:
        if (nbGetHojoCounter(formindex, 6) > 0) {
            return 1;
        }
        break;
    case 5:
        if (nbGetHojoCounter(formindex, 6) < 0) {
            return 1;
        }
        break;
    case 6:
        if (nbGetHojoCounter(formindex, 7) > 0) {
            return 1;
        }
        break;
    case 7:
        if (nbGetHojoCounter(formindex, 7) < 0) {
            return 1;
        }
        break;
    case 8:
        if (nbGetHojoCounter(formindex, 8) > 0) {
            return 1;
        }
        break;
    case 9:
        if (nbGetHojoCounter(formindex, 8) < 0) {
            return 1;
        }
        break;
    case 11:
        if (nbGetHojoCounter(formindex, 9) > 0) {
            return 1;
        }
        break;
    case 12:
        if (nbGetHojoCounter(formindex, 10) > 0) {
            return 1;
        }
        break;
    case 13:
        if (nbGetHojoCounter(formindex, 11) > 0) {
            return 1;
        }
        break;
    case 14:
        if (nbGetHojoCounter(formindex, 12) > 0) {
            return 1;
        }
        break;
    case 15:
        if (nbGetHojoCounter(formindex, 13) > 0) {
            return 1;
        }
        break;
    case 16:
        if (nbGetHojoCounter(formindex, 14) > 0) {
            return 1;
        }
        break;
    case 17:
        if (nbGetHojoCounter(formindex, 15) > 0) {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkAiHojoMax);

s32 Chk_ENHOJO(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojo(a->data->party[i].formindex, n)) {
            a->aiselect |= one << a->data->party[i].formindex;
        }
    }
    return a->aiselect != 0;
}

s32 Chk_FRHOJO(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    s32 one;

    one = 1;
    for (i = 4; i < 14; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (ChkAiHojo(a->data->party[i].formindex, n)) {
            a->aiselect |= one << a->data->party[i].formindex;
        }
    }
    return a->aiselect != 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENHOJOMAX);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRHOJOMAX);

s32 Chk_BA_286248(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (a->data->stat & 0x10) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_POW_2862D0(nbActionProcessData_t *a, s32 n)
{
    nbParty_t *party;
    s32 i;
    s32 one;

    one = 1;
    if (a->data->stat & 0x80) {
        party = a->data->party;
        for (i = 3; i >= 0; i--, party++) {
            if (!(party->flag & 1)) {
                continue;
            }
            if (party->flag & 8) {
                continue;
            }
            if (party->flag & 0x10) {
                continue;
            }
            if (party->flag & 0x20) {
                continue;
            }
            a->aiselect |= one << party->formindex;
        }
        return 1;
    }
    return 0;
}

s32 Chk_ENLIGHT(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    datUnitWork_t *work;    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != 1) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_ENDARK(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    datUnitWork_t *work;    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        work = nbGetUnitWorkPointer(a->data, &a->data->party[i]);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != -1) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_SENSEI_286530(nbActionProcessData_t *a)
{
    if (ChkAiHojo(a->form->formindex, 0x11) != 0) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 Chk_PLV(nbActionProcessData_t *a, s32 lv)
{
    if (datGetPlayerLevel() < lv) {
        return 0;
    }
    return 1;
}

s32 Chk_PPARTYLV(nbActionProcessData_t *a, s32 lv)
{
    if (nbGetLvAvg(a->data, 0) < lv) {
        return 0;
    }
    return 1;
}

s32 Chk_TURN(nbActionProcessData_t *a, s32 turn)
{
    if (a->data->turn < turn) {
        return 0;
    }
    return 1;
}

s32 Chk_ENHAMAMHK(nbActionProcessData_t *a, s32 n)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (!(a->data->party[i].flag & 1)) {
            continue;
        }
        if (a->data->party[i].flag & 8) {
            continue;
        }
        if (a->data->party[i].flag & 0x10) {
            continue;
        }
        if (a->data->party[i].flag & 0x20) {
            continue;
        }
        if (datGetAisyo(nbGetUnitWorkPointer(a->data, &a->data->party[i]), 6) & 0x70000) {
            continue;
        }
        a->aiselect |= 1 << a->data->party[i].formindex;
    }
    return a->aiselect != 0;
}

s32 Chk_SKILLNOMP(nbActionProcessData_t *a, s32 nskill)
{
    if (nbCheckSkillCost(a->form->formindex, nskill) != 0) {
        a->aiselect = 1 << a->form->formindex;
        return 1;
    }
    return 0;
}

s32 nbAiChkJoken(nbActionProcessData_t *a, u32 joken)
{
    u32 prev;
    s32 type;
    s32 para;

    type = joken >> 16;
    para = joken & 0xFFFF;
    if (type == 0) {
        return 1;
    }
    prev = a->aiselect;
    a->aiselect = 0;
    if (D_004F0788[type](a, para) == 0) {
        return 0;
    }
    if (prev != 0) {
        a->aiselect = prev & a->aiselect;
    }
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetAiEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPickAiAction);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetKoudou);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_AI);

void Sel_LIFEMIN(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->hp < min) {
            min = work->hp;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_BAARLV(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if ((work->badstatus & 0xFFF) == 0x200) {
            continue;
        }
        if (work->level < min) {
            min = work->level;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_LVMIN(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;
    s32 index;
    s32 min;

    index = -1;
    min = 10000;
    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->level < min) {
            min = work->level;
            index = i;
        }
    }
    if (index >= 0) {
        a->data->form[index].stat |= 8;
    }
}

void Sel_BAD(nbActionProcessData_t *a, s32 n)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        if ((nbGetUnitWorkFromFormindex(i)->badstatus & 0xFFF) != n) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_ID(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (work->id != n) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_LIGHT(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != 1) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_DARK(nbActionProcessData_t *a, s32 n)
{
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        work = nbGetUnitWorkFromFormindex(i);
        if (work->flag & 4) {
            continue;
        }
        if (datGet3Zokusei(work) != -1) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_PLAYER(nbActionProcessData_t *a, s32 n)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(a->data->form[i].stat & 4)) {
            continue;
        }
        if (!(nbGetUnitWorkFromFormindex(i)->flag & 4)) {
            continue;
        }
        a->data->form[i].stat |= 8;
    }
}

void Sel_JOKEN(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;
    s32 one;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        one = 1;
        if ((form->stat & 4) && (a->aiselect & (one << i))) {
            form->stat |= 8;
        }
        form++;
    }
}

void Sel_RND(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        if (form->stat & 4) {
            form->stat |= 8;
        }
        form++;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetSingleTarget2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetGroupTarget2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkCost);

s32 ChkClose(nbActionProcessData_t *a)
{
    s32 nowindex;
    s32 ret;

    ret = 1;
    switch (a->work->nowcommand) {
    case 0:
        nowindex = 0;
        break;
    case 1:
    case 5:
        nowindex = a->work->nowindex;
        break;
    default:
        return ret;
    }
    if (nbCheckSkillUse(a->form->formindex, nowindex) >= 1) {
        switch (D_003989E0[a->work->id].ailevel) {
        case 0:
            break;
        case 1:
        case 2:
            ret = 0;
            break;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetAiTarget);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRunAiSub);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRunAi);

s32 nbCommand_AI_ACT_ATTACK(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->work->nowcommand = 0;
    nbSetNormalSkill0(a->work);
    return 1;
}

s32 nbCommand_AI_ACT_ESCAPE(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->work->nowcommand = 7;
    a->work->nowindex = 0;
    return 1;
}

s32 nbCommand_AI_ACT_WAIT(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->work->nowcommand = 3;
    a->work->nowindex = 0;
    return 1;
}

s32 nbCommand_AI_ACT_SKILL(void)
{
    nbActionProcessData_t *a;
    s32 n;

    a = scrGetData();
    n = scrGetIntPara(0);
    a->work->nowcommand = 1;
    a->work->nowindex = n;
    return 1;
}

s32 nbCommand_AI_ACT_SUMMON(void)
{
    nbActionProcessData_t *a;
    s32 n;
    s32 no;

    a = scrGetData();
    n = scrGetIntPara(0);
    a->work->nowcommand = 1;
    a->work->nowindex = n;
    no = scrGetIntPara(1);
    a->aisummonid = no;
    nbSetActionCommand(a->data, no);
    return 1;
}

s32 nbCommand_AI_SET_CRITICAL(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->party->flag |= 0x100;
    return 1;
}

s32 nbCommand_AI_TAR_AI(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LIFEMIN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 1, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_BAARLV(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 9, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LVMIN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 10, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_BAD(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 2, scrGetIntPara(0)) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_ID(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 3, scrGetIntPara(0)) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LIGHT(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 4, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_DARK(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 5, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_PLAYER(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 6, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_RND(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 8, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_JOKEN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 7, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->selectbit = nbGetSelectBit(a->data);
    }
    return 1;
}

static void AiChkJoken(s32 joken)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->aiselect = 0;
    if (nbAiChkJoken(a, joken)) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
}

s32 nbCommand_AI_CHK_MYHP(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x10000);
    return 1;
}

s32 nbCommand_AI_CHK_MYMP(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x180000);
    return 1;
}

s32 nbCommand_AI_CHK_FRHP(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x20000);
    return 1;
}

s32 nbCommand_AI_CHK_DMGCNT(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x30000);
    return 1;
}

s32 nbCommand_AI_CHK_FRCNT(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x50000);
    return 1;
}

s32 nbCommand_AI_CHK_ENCNT(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x60000);
    return 1;
}

s32 nbCommand_AI_CHK_MYBAD(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x70000);
    return 1;
}

s32 nbCommand_AI_CHK_FRBAD(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x80000);
    return 1;
}

s32 nbCommand_AI_CHK_ENBAD(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x90000);
    return 1;
}

s32 nbCommand_AI_CHK_ENALLBAD(void)
{
    AiChkJoken(scrGetIntPara(0) | 0xA0000);
    return 1;
}

s32 nbCommand_AI_CHK_ENID(void)
{
    AiChkJoken(scrGetIntPara(0) | 0xB0000);
    return 1;
}

s32 nbCommand_AI_CHK_FRID(void)
{
    AiChkJoken(scrGetIntPara(0) | 0xC0000);
    return 1;
}

s32 nbCommand_AI_CHK_ENHOJO(void)
{
    AiChkJoken(scrGetIntPara(0) | 0xD0000);
    return 1;
}

s32 nbCommand_AI_CHK_FRHOJO(void)
{
    AiChkJoken(scrGetIntPara(0) | 0xE0000);
    return 1;
}

s32 nbCommand_AI_CHK_ENHOJOMAX(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x190000);
    return 1;
}

s32 nbCommand_AI_CHK_FRHOJOMAX(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x1A0000);
    return 1;
}

s32 nbCommand_AI_CHK_BA(void)
{
    AiChkJoken(0xF0000);
    return 1;
}

s32 nbCommand_AI_CHK_ENLIGHT(void)
{
    AiChkJoken(0x100000);
    return 1;
}

s32 nbCommand_AI_CHK_ENDARK(void)
{
    AiChkJoken(0x110000);
    return 1;
}

s32 nbCommand_AI_CHK_POW(void)
{
    AiChkJoken(0x120000);
    return 1;
}

s32 nbCommand_AI_CHK_PLV(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x130000);
    return 1;
}

s32 nbCommand_AI_CHK_PPARTYLV(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x140000);
    return 1;
}

s32 nbCommand_AI_GET_TURN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    scrSetIntReturnValue(a->data->turn);
    return 1;
}

s32 nbCommand_AI_GET_ALLKOUDO(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    scrSetIntReturnValue(nbGetHojoCounter(a->form->formindex, 0x11));
    return 1;
}

s32 nbCommand_AI_GET_PRESSKOUDO(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    scrSetIntReturnValue(nbGetHojoCounter(a->form->formindex, 0x12));
    return 1;
}

s32 nbCommand_AI_GET_MYLV(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    scrSetIntReturnValue(a->work->level);
    return 1;
}

s32 nbCommand_AI_GET_MYAISYO(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    scrSetIntReturnValue(nbGetAisyoTable(a->form->formindex));
    return 1;
}

s32 nbCommand_AI_GET_FUREA(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16() / 2);
    return 1;
}

s32 nbCommand_AI_CHK_ESCAPE(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (nbGetEscapeSwitch(a->data)) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 nbCommand_AI_CHK_ENHAMAMHK(void)
{
    AiChkJoken(0x1B0000);
    return 1;
}

s32 nbCommand_AI_CHK_SKILLNOMP(void)
{
    AiChkJoken(scrGetIntPara(0) | 0x1C0000);
    return 1;
}

s32 nbCommand_AI(void)
{
    nbRunAi(scrGetData());
    return 1;
}

s32 nbCommand_AI_TBL(void)
{
    s32 no;

    no = scrGetIntPara(0);
    nbRunAiSub(scrGetData(), no);
    return 1;
}

s32 nbCommand_BTL_CAM_SET(void)
{
    sdfVector4_t pos;
    sdfVector4_t rot;

    memset(&pos, 0, sizeof(pos));
    scrGetData();
    pos.x = scrGetFloatPara(0);
    pos.y = scrGetFloatPara(1);
    pos.z = scrGetFloatPara(2);
    rot.x = scrGetFloatPara(3);
    rot.y = scrGetFloatPara(4);
    rot.z = scrGetFloatPara(5);
    rot.w = scrGetFloatPara(6);
    nbCameraScriptSet(&pos, &rot);
    return 1;
}

s32 nbCommand_BTL_CAM_ORG(void)
{
    return 1;
}

s32 nbCommand_BTL_CAM_1SHOT(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    nbCameraScriptSet1Shot(a->form);
    return 1;
}

s32 nbCommand_BTL_CAM_2SHOT(void)
{
    return 1;
}

s32 nbCommand_BTL_CAM_BOUGAI(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    nbCameraScriptSetBougai(a->form);
    return 1;
}

s32 nbCommand_BTL_PNL_OFF(void)
{
    nbPanelAllHide();
    nbPanelAutoHide();
    return 1;
}

s32 nbCommand_BTL_PNL_ON(void)
{
    return 1;
}

s32 nbCommand_BTL_EVENT(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->data->eventno = scrGetIntPara(0);
    return 1;
}

s32 nbCommand_BTL_TAIKYO(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->data->negoresult = 2;
    a->data->negoeformindex = a->form->formindex;
    a->unk_68 = 0;
    return 1;
}

s32 nbCommand_BTL_ALLTAIKYO(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->data->negoresult = 6;
    a->data->negoeformindex = a->form->formindex;
    a->unk_68 = 0;
    return 1;
}

s32 nbCommand_BTL_PRESS(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->data->negoresult = 4;
    a->data->negoeformindex = a->form->formindex;
    a->unk_68 = 0;
    return 1;
}

s32 nbCommand_BTL_NAKAMA(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->data->negoresult = 3;
    a->data->negoeformindex = a->form->formindex;
    a->unk_68 = 0;
    return 1;
}

s32 nbCommand_BTL_FORM_ALL_ON(void)
{
    nbAllFormDispOn();
    return 1;
}

s32 nbCommand_BTL_FORM_PLY_ON(void)
{
    nbGroupFormDispOn(0);
    nbGroupFormDispOff(1);
    return 1;
}

s32 nbCommand_BTL_FORM_ENE_ON(void)
{
    nbGroupFormDispOff(0);
    nbGroupFormDispOn(1);
    return 1;
}

s32 nbCommand_BTL_AI_ROT_CLR(void)
{
    btl_ai_rot_cnt = 0;
    return 1;
}

s32 nbCommand_BTL_AI_ROT_NEXT(void)
{
    s32 max;

    max = scrGetIntPara(0);
    btl_ai_rot_cnt++;
    btl_ai_rot_cnt %= max;
    return 1;
}

s32 nbCommand_BTL_AI_ROT_GET(void)
{
    scrSetIntReturnValue(btl_ai_rot_cnt);
    return 1;
}

s32 nbCommand_BTL_AI_CHANGE_OFF(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    data->stat |= 0x40000;
    return 1;
}

s32 nbCommand_BTL_AI_CHANGE_ON(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    data->stat &= ~0x40000;
    return 1;
}

void nbInitAiScript(nbActionProcessData_t *a, s32 no)
{
    dds3ProcessID_t *proc;
    scrProcessWork_s *w;

    proc = scrStartScript2(a->data->mainproc->priority - 1, scrEmbedded_4F0800, no);
    scrSetUserData(proc, a);
    w = dds3GetProcessData(proc);
    if (w->MesHandle >= 0) {
        itfMesMngSetVar(w->MesHandle, 0, a->work->id, 1);
    }
    dds3LinkProcess(a->data->mainproc, proc);
    a->data->aiproc = proc;
}

void nbDispText(char *text, char *text2, s32 type, s32 time, u32 color)
{
    nbHelpProcessData_t *w;
    _FRQ *frq;

    w = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    nbHelpOff();
    w->text = text;
    w->text2 = text2;
    if (type == 1) {
        w->meshandle = w->nextmeshandle;
        w->mesid = w->nextmesid;
    }
    w->color = color;
    switch (type) {
    case 1:
        w->width = itfMesMngGetWidth(w->meshandle, w->mesid, 0) >> 4;
        break;
    case 2:
        frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, color, w->text, 0);
        w->width = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        frq = frGetCNameFRQ(0, 1, 0, 0);
        w->width2 = w->width + 10;
        w->width = w->width + frGetStrWidth2(frq) + 10;
        frDestroyFontRequestAfter(frq);
        break;
    case 0:
    default:
        frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, w->color, w->text, 0);
        w->width = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        if (w->text2 != NULL) {
            frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, w->color, w->text2, 0);
            w->width2 = w->width + 10;
            w->width = w->width + frGetStrWidth2(frq) + 10;
            frDestroyFontRequestAfter(frq);
        }
        break;
    }
    w->time = time;
    w->type = type;
    w->unk_C = 0;
}

void nbDispSkillName(s32 no)
{
    nbDispText(text1[no], NULL, 0, 30, 0x89FEFF80);
}

void nbDispTextById(s32 no)
{
    nbDispText(pStr[no], NULL, 0, 30, 0x89FEFF80);
}

void nb_289078(s32 no, s32 type)
{
    nbDispText(D_005263F8[no], NULL, type, 30, 0x89FEFF80);
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545850);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545868);

void nbDispActionText(u32 command)
{
    switch (command) {
    case 0:
        nbDispText(text1[0x200], NULL, 0, 30, 0x89FEFF80);
        return;
    case 2:
        nbDispText(text1[0x20E], NULL, 0, 30, 0x89FEFF80);
        return;
    case 3:
        nbDispText(text1[0x202], NULL, 0, 30, 0x89FEFF80);
        return;
    case 6:
        nbDispText(text1[0x203], NULL, 0, 30, 0x89FEFF80);
        return;
    case 7:
        nbDispText(text1[0x204], NULL, 0, 30, 0x89FEFF80);
        break;
    }}

void nb_2891A8(s32 no)
{
    nbDispText(text1[0x200 + no], NULL, 0, 30, 0x89FEFF80);
}

void nbDispDevilName(s32 formindex)
{
    char **lndtext;

    nbGetMainProcessData();
    if (formindex == 0) {
        lndtext = &datExplainTable[0x40];
        nbDispText(lndtext[datGetPlayerLNDLevel(DDS3_GBWK + 0xA54)], NULL, 2, 2,
                   0xA09DC380);
    } else {
        nbDispText(datExplainTable[datDevilTable[formindex].race], text2[formindex],
                   0, 2, 0xA09DC380);
    }}

void nbDispJyokyoHelp(s32 meshandle, s32 mesid, s32 type, s32 value)
{
    s32 digit;

    digit = type;
    if (type == 1) {
        if (value == 0) {
            digit = 10;
        }
    }
    if (digit >= 0) {
        itfMesMngSetVar(meshandle, 0, value, digit);
    }
    nbDispMsgHelp(meshandle, mesid, 1);
}

void nbDispMsgHelp(s32 meshandle, s32 mesid, s32 type)
{
    nbHelpProcessData_t *w;

    w = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    w->nextmeshandle = meshandle;
    w->nextmesid = mesid;
    switch (type) {
    case 0:
        nbDispText(NULL, NULL, 1, 45, 0x89FEFF80);
        return;
    case 1:
        nbDispText(NULL, NULL, 1, 45, 0x89FEFF80);
        return;
    case 2:
        nbDispText(NULL, NULL, 1, 20, 0xA09DC380);
        return;
    case 3:
        nbDispText(NULL, NULL, 1, 30, 0x89FEFF80);
        break;
    }}

void nbHelpOff(void)
{
    nbHelpProcessData_t *h;

    h = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    if (h->text == NULL) {
        if (h->type != 1) {
            return;
        }
        if (h->meshandle < 0) {
            return;
        }
    }
    h->text = NULL;
    h->text2 = NULL;
    if (h->type == 1) {
        itfMesMngRequestMessageRelease(h->meshandle, 0);
        h->meshandle = -1;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDrawHelpPanel);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbHelpProcess1);

void nbHelpShutdown(dds3ProcessID_t *id)
{
    nbHelpProcessData_t *w;

    w = dds3GetProcessData(id);
    if (w->text != NULL || (w->type == 1 && w->meshandle >= 0)) {
        w->text = NULL;
        w->text2 = NULL;
        if (w->type == 1) {
            itfMesMngRequestMessageRelease(w->meshandle, 0);
            w->meshandle = -1;
        }
    }
    sdfChipFreeDirect(w);
}

nbHelpProcessData_t *nbAllocHelpWork(void)
{
    nbHelpProcessData_t *w;

    w = sdfChipAllocDirect(sizeof(nbHelpProcessData_t));
    memset(w, 0, sizeof(nbHelpProcessData_t));
    w->meshandle = -1;
    w->unk_34 = 0x80;
    return w;
}

void nbInitHelpProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *proc;

    proc = dds3StartProcess(D_005514F0, 0x3F9, 1, 1, nbHelpProcess1, nbHelpShutdown,
                            nbAllocHelpWork());
    dds3LinkProcess(data->mainproc, proc);
    data->helpproc = proc;
}

s32 nbAutoCommProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = dds3GetProcessData(id);
    if (nbPanelAutoCheckShow() == 0) {
        return 0;
    }
    if (dds3DebugSign != 1 || (data->stat & 0x20000000) == 0) {
        if (data->stat & 0x40) {
            if ((s8)dds3PadButtonState[0x23] < 0 || (s8)dds3PadButtonState[0x22] < 0) {
                data->stat &= ~0x40;
                nbPlaySe(11);
            }
        } else if ((s8)dds3PadButtonState[0x22] < 0) {
            data->stat |= 0x40;
            nbPlaySe(14);
        }
    }
    return 0;}

void nbInitAutoCommProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *proc;

    proc = dds3StartProcess(D_005458A0, 0x2B0E, 1, 1, nbAutoCommProcess1, NULL, data);
    dds3LinkProcess(data->mainproc, proc);
    data->autocommproc = proc;
}

void nbFormDispOff(s32 formindex)
{
    nbFormation_t *f;

    f = &nbGetMainProcessData()->form[formindex];
    if ((f->stat & 1) == 0) {
        return;
    }
    if ((f->stat & 2) == 0) {
        return;
    }
    if (f->basic == NULL) {
        return;
    }
    f->hunit->basic->flag |= 1;
    dds3ObjectBaseOnFlag(f->basic, 0x1000);
}

void nbFormDispOff2(nbFormation_t *form)
{
    nbFormDispOff(form->formindex);
}

void nbFormDispOn(s32 formindex)
{
    nbFormation_t *f;

    f = &nbGetMainProcessData()->form[formindex];
    if ((f->stat & 1) == 0) {
        return;
    }
    if ((f->stat & 2) == 0) {
        return;
    }
    if (f->basic == NULL) {
        return;
    }
    f->hunit->basic->flag &= ~1;
    dds3ObjectBaseOffFlag(f->basic, 0x1000);
}

void nbFormDispOn2(nbFormation_t *form)
{
    nbFormDispOn(form->formindex);
}

void nbGroupFormDispOff(s32 group)
{
    s32 start;
    s32 count;
    s32 i;

    start = 0;
    count = 0;
    switch (group) {
    case 0:
        count = 4;
        break;
    case 1:
        start = 4;
        count = 10;
        break;
    case 2:
        start = 14;
        count = 2;
        break;
    }
    for (i = start; i < start + count; i++) {
        nbFormDispOff(i);
    }
}

void nbGroupFormDispOn(s32 group)
{
    s32 start;
    s32 count;
    s32 i;

    start = 0;
    count = 0;
    switch (group) {
    case 0:
        count = 4;
        break;
    case 1:
        start = 4;
        count = 10;
        break;
    case 2:
        start = 14;
        count = 2;
        break;
    }
    for (i = start; i < start + count; i++) {
        nbFormDispOn(i);
    }
}

void nbAllFormDispOff(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormDispOff(i);
    }
}

void nbAllFormDispOn(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormDispOn(i);
    }
}

void nbSingleFormExceptedDispOff(nbFormation_t *form)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (i == form->formindex) {
            nbFormDispOn(i);
        } else {
            nbFormDispOff(i);
        }
    }
}

void nbDoubleFormExceptedDispOff(nbFormation_t *form1, nbFormation_t *form2)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (i == form1->formindex || i == form2->formindex) {
            nbFormDispOn(i);
        } else {
            nbFormDispOff(i);
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRestartMotion);

void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos)
{
    SDF_VU0_COPYVECTOR(&form->pos, pos);
    if ((form->stat & 1) == 0) {
        return;
    }
    if ((form->stat & 2) == 0) {
        return;
    }
    if (form->basic == NULL) {
        return;
    }
    dds3SetPosPoseSet(form->basic, pos);
    SDF_VU0_COPYVECTOR(&form->pos, pos);
}

void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot)
{
    sdfVector4_t *p = &form->rot;

    SDF_VU0_COPYVECTOR(p, rot);
    if ((form->stat & 1) == 0) {
        return;
    }
    if ((form->stat & 2) == 0) {
        return;
    }
    if (form->basic == NULL) {
        return;
    }
    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_LOADVECTOR_B(D_005458B0);
    Mult();
    SDF_VU0_STOREVECTOR_NOMEM(D_00575440);
    dds3SetRotPoseSet(form->basic, D_00575440);
    SDF_VU0_COPYVECTOR(p, rot);
}

void nbSetFormPosRot(nbFormation_t *form, sdfVector4_t *pos, sdfVector4_t *rot)
{
    nbSetFormPos(form, pos);
    nbSetFormRot(form, rot);
}

void nbResetFormPosRot(nbFormation_t *form)
{
    if (form->stat & 1) {
        nbSetFormPos(form, &form->orgpos);
        nbSetFormRot(form, &form->orgrot);
    }
}

void nbResetAllFormPosRot(void)
{
    nbMainProcessData_t *data;
    s32 i;

    data = nbGetMainProcessData();
    for (i = 0; i < 16; i++) {
        nbResetFormPosRot(&data->form[i]);
    }
}

void nbLoadUnitPos(nbFormation_t *form)
{
    SDF_VU0_LOADVECTOR(&form->pos);
}

void effBTLModelLoadRotate(nbFormation_t *form)
{
    SDF_VU0_LOADVECTOR(&form->rot);
}

f32 nbCameraGetBossDistance(void)
{
    return 800.0f;
}

f32 nbCameraGetSummonNear(void)
{
    return 50.0f;
}

f32 nbCameraGetSummonFar(void)
{
    return 100.0f;
}

f32 func_0028A330(void)
{
    return D_005514F8;
}

f32 nbGetEnemyInterval(void)
{
    return nbFormEnemyInterval;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005458A0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005458B0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetPartyFormation);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetEnemyFormation);

void nbSetPlayerFormation(s32 party)
{
    nbSetPartyFormation(party, 0x100);
}

void nbFormatFormation(nbMainProcessData_t *data, s32 index)
{
    nbFormation_t *form;

    form = &data->form[index];
    memset(form, 0, sizeof(nbFormation_t));
    form->r = 100.0f;
    form->cr = 100.0f;
    form->scl = 1.0f;
    form->formindex = index;
    if (index < 4) {
        form->stat |= 0x100;
    } else if (index < 0xE) {
        form->stat |= 0x200;
    } else {
        form->stat |= 0x400;
    }
    SDF_VU0_COPYVECTOR(&form->pos, &D_00526420);
    SDF_VU0_COPYVECTOR(&form->orgpos, &D_00526420);
    SDF_VU0_COPYVECTOR(&form->rot, &D_00526430);
    SDF_VU0_COPYVECTOR(&form->orgrot, &D_00526430);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCopyFormation);

void nbMoveFormation(void *data, s32 from, s32 to)
{
    if (from != to) {
        nbCopyFormation(data, from, to);
        nbFormatFormation(data, from);
    }
}

void nbUpdateFormation(nbMainProcessData_t *data)
{
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        if ((data->form[i].stat & 1) == 0) {
            for (j = i + 1; j < 4; j++) {
                if ((data->form[j].stat & 1) != 0) {
                    nbMoveFormation(data, j, i);
                    break;
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbClearActionFlags);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetActionCommand);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetBossDummy);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetBattleDummy);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitPlayer);

void DrawTest(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess3);

void *TestProcess7(void)
{
    SetBossDummy();
    return TestProcess1;
}

void *TestProcess2(void)
{
    if (dds3SearchProcessPriority(0x3FA) == 0) {
        nbExit();
        return (void *)-1;
    }
    return NULL;
}

void *TestProcess6(void)
{
    void *next;

    DrawTest();
    next = nbEncountCalc(pack, 100.0f);
    if (next == NULL) {
        return next;
    }
    nbCallNewBattle(pack, next);
    return TestProcess2;
}

void *TestProcess1(void)
{
    return NULL;
}

void TestShutdown(void) {
}

void nbExitBattleTest(void)
{
    nbExit();
    dds3KillProcessName(D_00545B90, 1);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitBattleTest);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountJoken);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountMax);

u16 nbGetEncountCnt(void)
{
    return *(u16 *)(DDS3_GBWK + 0x1CF8);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncountCalc);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckEncountLevel);

void fclClearTerminalState(void)
{
    u8 *gb;

    gb = DDS3_GBWK;
    *(s32 *)(gb + 0x1CF4) = 0;
    *(u16 *)(gb + 0x1CF8) = 0;
    nbEncountMax = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", func_0028CFC8);

s32 nbInitMaeTableDefault(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    return func_0028CFC8(data->unk_28, data->encno);
}

s32 nbInitMaeTable(u32 code)
{
    return func_0028CFC8(code >> 24, code & 0xFFFF);
}

s32 nbGetUnitWorkFromAction(nbFormation_t *form)
{
    return nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
}

void nbPlaySe(s32 no)
{
    p2sdPlaySE(no, 0x7F, 0x3F);
}

s32 nbGetMotionTable(s32 unitid, s32 index)
{
    u8 *p;

    if (unitid == 0) {
        p = D_003494B0;
    } else {
        p = datModelParamTable + unitid * 784;
    }
    return *(s8 *)(p + index * 28 + 0x31);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMotionByIndex);

s32 nbGetUnitFromAction(nbFormation_t *form, s32 index)
{
    return nbLoadMotionByIndex(nbGetUnitWorkFromAction(form), index);
}

s32 nbMakeSeId(s32 seno)
{
    return (seno + 0x500) << 16;
}

s32 nbPlayMotionSe(nbFormation_t *form)
{
    return nbMakeSeId(nbGetUnitWorkFromAction(form));
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545B90);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545BA0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMemoryLoadNegoSe);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMemoryLoadMotionSe);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetFormEntry);

nbStatWork_t *nbSetFormEntry(s32 id)
{
    nbMainProcessData_t *data;
    nbStatWork_t *stat;
    s32 index;

    data = nbGetMainProcessData();
    index = nbFindFormIndex(id);
    if (index >= 0) {
        data->statwork[index].refcnt++;
        return NULL;
    }
    stat = &data->statwork[nbFindFreeFormSlot()];
    stat->id = 1;
    stat->unk_02 = id;
    stat->refcnt = 1;
    return stat;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMidiFile);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckPackLoad);

s32 nbFindFreeFormSlot(void)
{
    nbStatWork_t *stat;
    s32 i;

    stat = nbGetMainProcessData()->statwork;
    for (i = 0; i < 10; i++) {
        if (stat[i].id == 0) {
            return i;
        }
    }
    return -1;
}

s32 nbFindFormIndex(s32 id)
{
    nbStatWork_t *stat;
    s32 i;

    stat = nbGetMainProcessData()->statwork;
    for (i = 0; i < 10; i++) {
        if (stat->id != 0 && stat->unk_02 == id) {
            return i;
        }
        stat++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeFormResources);

void nbFreeAllFormResources(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        nbFreeFormResources(data, i);
    }
    sdfMemFreeDirect(data->statbuf);
    data->statwork = NULL;
    data->statbuf = NULL;
}

void nbAllocMainProcessData(nbMainProcessData_t *data)
{
    void *p;

    p = sdfMemAllocDirect(0x8C0);
    data->statbuf = p;
    data->statwork = sdfMemLock(p);
    memset(data->statwork, 0, 0x8C0);
    if (p2sdLoadCheckMidi(0x10000) == 0) {
        p2sdLoadSMGFile(0x10000);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMaeCheckDone);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C38);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C58);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C78);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetMaeFlag);

void nbIncResultCounter(void)
{
    nbResultCount++;
}

void nbMaeWaitStep(void)
{
    p2sdPlayBGM(bgmno);
}

void nbRestoreBgm(void)
{
    if (nbBgmSaved > 0) {
        p2sdSetBgmTrack(nbBgmSaved);
        nbBgmSaved = 0;
    }
}

void nbBgmFadeOut(void)
{
    p2sdStopBGM(bgmno);
    p2sdStopBGM(nbBgmFadeState);
}

void nbBgmStop(void)
{
    printf(D_00545CE0, nbBgmSaved, nbBgmFadeState);
    p2sdStopBGM(bgmno);
    p2sdStopBGM(nbBgmFadeState);
    p2sdStopBGM(0x2B0000);
}

void nb_28DE10(void)
{
    func_002C1E70(0x7F);
}

void nb_28DE28(void)
{
    func_002C1E70(0);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess3);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545CE0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_55);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_5);

void *nbEncProcess2(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (nbCheckLoadGroup(1) != 0) {
        return NULL;
    }
    if ((data->stat & 0x10) != 0) {
        nbLoadUnitGroup(0, 0, 1);
    } else {
        nbLoadUnitGroup(0, 1, 0);
    }
    return nbEncProcess2_5;
}

void *nbEncProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    effBTLEncountStart();
    readbasetime = dds3GetKernelCount();
    readtime = 0;
    if ((data->stat & 0x800) != 0) {
        nbLoadUnitGroup(1, 1, 0);
    } else {
        nbLoadUnitGroup(1, 1, 1);
    }
    return nbEncProcess2;
}

void nbEncShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->encproc = NULL;
}

void nbInitEncProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00551550, 0x3F9, 1, 1, nbEncProcess1, nbEncShutdown, NULL);
    dds3LinkProcess(data->mainproc, id);
    data->encproc = id;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCollectBadCurseUnits);

void nbMarkBadCurseUnits(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        if (form->stat & 4) {
            form->stat |= 8;
        }
        form++;
    }
}

s32 nbBadCurseApplyMove(nbActionProcessData_t *a, s32 mode)
{
    s32 ctype;
    s32 crule;
    s32 carea;

    ctype = 0;
    crule = 0;
    carea = 0;
    nbSetTargetSkill(a, a->work, &ctype, &crule, &carea, mode);
    if (nbSetSelectOkMark(a->partyindex, ctype, crule, carea) == 0) {
        return 0;
    }
    switch (ctype) {
    case 0:
        nbCollectBadCurseUnits(a);
        break;
    case 1:
    case 2:
        nbMarkBadCurseUnits(a);
        break;
    }
    a->selectbit = nbGetSelectBit(a->data);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSkillIsInRange);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindBadCurseTarget);

s32 nbBadCurseStep3(nbActionProcessData_t *a)
{
    s32 target;

    target = nbFindBadCurseTarget(a, 3);
    if (target < 0) {
        return 1;
    }
    a->work->nowcommand = 1;
    a->work->nowindex = target;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep2(nbActionProcessData_t *a)
{
    s32 target;

    target = nbFindBadCurseTarget(a, 2);
    if (target < 0) {
        return 1;
    }
    a->work->nowcommand = 1;
    a->work->nowindex = target;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep1(nbActionProcessData_t *a)
{
    a->work->nowcommand = 0;
    a->work->nowindex = 0;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    if (nbCheckBadCurse(a->form->formindex) != 0) {
        nbSetJyokyoMes(a, 0x4A, 1, a->work->id);
    } else {
        switch (a->work->badstatus & 0xFFF) {
        case 8:
            nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
            break;
        case 0x80:
            nbSetJyokyoMes(a, 0x36, 1, a->work->id);
            break;
        }
    }
    a->unk_04 |= 0x200;
    return 0;
}

s32 nbBadCurseStep4(nbActionProcessData_t *a)
{
    if (nbCheckFormationPlayerSide(a->form->formindex) != 0 &&
        *(s32 *)(DDS3_GBWK + 0xA48) == 0) {
        return 1;
    }
    if ((a->work->flag & 0x20) != 0 && (a->party->flag & 0x800) != 0) {
        return 1;
    }
    a->party->flag |= 0x800;
    a->work->nowcommand = 1;
    a->work->nowindex = 0xF7;
    if (nbBadCurseApplyMove(a, 0) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep5(nbActionProcessData_t *a)
{
    if (nbCheckFormationPlayerSide(a->form->formindex) == 0) {
        return 1;
    }
    a->work->nowcommand = 4;
    a->work->nowindex = 0x180;
    a->unk_68 = 6;
    if (nbBadCurseApplyMove(a, 0) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep6(nbActionProcessData_t *a)
{
    datUnitWork_t *w;

    w = a->work;
    if (w->flag & 4) {
        return 1;
    }
    if (w->id >= 0x100) {
        return 1;
    }
    w->nowcommand = 7;
    w->nowindex = 0;
    switch (w->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, w->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, w->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep0(nbActionProcessData_t *a)
{
    datUnitWork_t *w;
    s32 bad;

    w = a->work;
    bad = w->badstatus & 0xFFF;
    w->nowcommand = 8;
    w->nowindex = 0;
    switch (bad) {
    case 8:
        nbSetJyokyoMes(a, 0x30, 1, w->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x38, 1, w->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep7(nbActionProcessData_t *a)
{
    datUnitWork_t *w;

    w = a->work;
    w->nowcommand = 10;
    w->nowindex = 0;
    nbSetJyokyoMes(a, 0x4C, 1, w->id);
    return 0;
}

s32 nbGetBadCurseStep(void)
{
    s32 i;
    s32 total;
    s32 sum;
    s32 r;

    total = 0;
    for (i = 0; i < nbBadCurseTimer; i++) {
        total += nbBadCurseTable[i].weight;
    }
    if (total == 0) {
        return -1;
    }
    r = dds3GetRandIntA(0, total);
    sum = 0;
    for (i = 0; i < nbBadCurseTimer; i++) {
        if (nbBadCurseTable[i].weight != 0) {
            sum += nbBadCurseTable[i].weight;
            if (r < sum) {
                return nbBadCurseTable[i].step;
            }
        }
    }
    return 9;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetBadCurseTimer);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitBadCurseState);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545D40);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbApplyBadCurse);

nbMakaWork_t *GetMakaProcessData(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(D_00551560);
    if (id == NULL) {
        return NULL;
    }
    return dds3GetProcessData(id);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakaDispOn);

void nbMakaDispOff(void)
{
    nbMakaWork_t *w;

    w = GetMakaProcessData();
    if (w->unk_08 != 0) {
        if (w->unk_1C == 0) {
            w->unk_24 = 0x14;
        }
        w->unk_00 = 2;
    }
}

u32 nbPow10(u32 v, s32 n)
{
    s32 p;

    p = 10;
    if (n > 0) {
        do {
            p = p * 10;
            n--;
        } while (n != 0);
    }
    return (v % p) / (p / 10);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDrawMakaNum);

void nbMakaStep(nbMakaWork_t *w)
{
    switch (w->unk_00) {
    case 1:
        w->unk_08 = 1;
        if (w->unk_28 == 0x80) {
            w->unk_00 = 0;
            return;
        }
        w->unk_28 += 0x20;
        if (w->unk_28 > 0x80) {
            w->unk_28 = 0x80;
        }
        return;
    case 2:
        if (w->unk_24 > 0) {
            w->unk_24--;
            return;
        }
        if (w->unk_28 > 0) {
            w->unk_28 -= 0x14;
            if (w->unk_28 < 0) {
                w->unk_28 = 0;
            }
            return;
        }
        w->unk_08 = 0;
        w->unk_00 = 0;
        return;
    }
}

s32 nbMakaProcess1(dds3ProcessID_t *id)
{
    nbMakaWork_t *w;

    w = dds3GetProcessData(id);
    nbMakaStep(w);
    if (w->unk_08 != 0) {
        nbDrawMakaNum(w, w->unk_0C, w->unk_10, 0x80808080, w->unk_28);
    }
    return 0;
}

void nbMakaShutdown(dds3ProcessID_t *id)
{
    void *w;

    w = dds3GetProcessData(id);
    if (w != NULL) {
        sdfChipFreeDirect(w);
    }
}

nbMakaWork_t *nbAllocMakaWork(void)
{
    nbMakaWork_t *w;

    w = sdfChipAllocDirect(sizeof(nbMakaWork_t));
    memset(w, 0, sizeof(nbMakaWork_t));
    w->maka = *(s32 *)(DDS3_GBWK + 0xA48);
    return w;
}

dds3ProcessID_t *nbInitMakaProcess(dds3ProcessID_t *parent)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00551560, 0x2B0E, 1, 1, nbMakaProcess1, nbMakaShutdown,
                          nbAllocMakaWork());
    if (parent != NULL) {
        dds3LinkProcess(parent, id);
    }
    return id;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEventEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeEventEntry);

s32 nbCheckEventEntries(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (nbEventEntryTable[i].unk_08 != 0 && nbEventEntryTable[i].unk_08 == data->encno) {
            if (nbEventEntryTable[i].rid != 0 &&
                evtGetEventPackStat(nbEventEntryTable[i].rid) != 2) {
                return 0;
            }
            if (nbEventEntryTable[i].unk_0C != 0 &&
                p2sdLoadCheckMidi(nbEventEntryTable[i].unk_0C) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void nbStartEventPacks(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (nbEventEntryTable[i].unk_08 != 0 && nbEventEntryTable[i].unk_08 == data->encno) {
            if (nbEventEntryTable[i].rid != 0) {
                dds3LinkProcess(data->mainproc,
                                evtLoadEventPack(nbEventEntryTable[i].rid));
            }
            if (nbEventEntryTable[i].unk_0C != 0) {
                p2sdLoadSMGFile(nbEventEntryTable[i].unk_0C);
            }
        }
    }
}

void nbUpdateEventProcessData(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (e != NULL) {
        e->unk_08++;
        e->frame++;
    }
}

s32 func_0028FA30(nbEventProcessData_t *e)
{
    if (e == NULL) {
        return 0;
    }
    return e->step;
}

void nbFreeEffectSlot(nbEventProcessData_t *e)
{
    if (e != NULL) {
        e->step++;
        e->frame = 0;
    }
}

void func_0028FA68(nbEventProcessData_t *e, s32 step)
{
    if (e != NULL) {
        e->step = step;
        e->frame = 0;
    }
}

nbFormation_t *nbGetEventForm(nbEventProcessData_t *e)
{
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form + 4;
    for (i = 4; i < 14; i++) {
        if (form->stat & 1) {
            return form;
        }
        form++;
    }
    return NULL;
}

dds3Basic_t *nbCreateBasicEffect(s32 a, s32 b, s32 c, s32 d)
{
    dds3Basic_t *basic;

    basic = dds3AddBEDEffectObjectBasic2(a, b, c, d);
    dds3EffectObjectBasicStatOn(basic, 1);
    return basic;
}

void ResetWaitMotion(nbEventProcessData_t *e)
{
    s32 i;

    for (i = 0; i < 14; i++) {
        if (e->data->form[i].stat & 1) {
            if (e->data->form[i].stat & 2) {
                nbSetWaitMotion(&e->data->form[i], 0, 1.0f);
            }
        }
    }
}

void SetWaitMotion(nbEventProcessData_t *e)
{
    s32 i;

    for (i = 0; i < 14; i++) {
        if (e->data->form[i].stat & 1) {
            if (e->data->form[i].stat & 2) {
                nbSetWaitMotion(&e->data->form[i], 0, 1.0f);
                nbSetUnitMotion(&e->data->form[i], 0, 0, 1.0f);
            }
        }
    }
}

s32 nbGetDeadEventFadeTime(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadetime;
}

s32 nbGetDeadEventFadeOutFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadeoutframe;
}

s32 nbGetDeadEventFadeInFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadeinframe;
}

s32 nbGetDeadEventFadeStopFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadestopframe;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEventProcess3);

void *nbEventProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbEventProcessData_t *e;
    s32 rid;

    data = nbGetMainProcessData();
    e = dds3GetProcessData(id);
    printf(D_00545DB0, data->eventno);
    rid = nbEventEntryTable[data->eventno].rid;
    data->eventrid = rid;
    if (rid > 0) {
        e->unk_04 = rid;
    }
    return nbEventProcess3;
}

void nbEventShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbEventProcessData_t *e;

    data = nbGetMainProcessData();
    nbFreeEventEntry(data);
    data->stat &= ~0x1000;
    data->eventno = 0;
    data->eventproc = NULL;
    e = dds3GetProcessData(id);
    SetWaitMotion(e);
    if (e->unk_24 != NULL) {
        sdfMemFreeDirect(e->unk_24);
        e->unk_24 = NULL;
    }
    if (e->unk_1C != NULL) {
        fileSyncAll();
        fileHandleFree(e->unk_1C);
        e->unk_1C = NULL;
    }
    if (e->unk_20 != NULL) {
        fileSyncAll();
        pacFileHandleFree_(e->unk_20);
        e->unk_20 = NULL;
    }
    if (e->unk_18 != NULL) {
        e->unk_18(id);
    }
    if (e->unk_14 != NULL) {
        sdfChipFreeDirect(e->unk_14);
    }
    sdfChipFreeDirect(e);
}

void nbInitEventProcess(nbMainProcessData_t *data)
{
    nbEventProcessData_t *e;
    dds3ProcessID_t *id;

    e = sdfChipAllocDirect(sizeof(nbEventProcessData_t));
    memset(e, 0, sizeof(nbEventProcessData_t));
    e->data = data;
    data->stat |= 0x1000;
    data->endtype = 0;
    id = dds3StartProcess(D_00545DC8, 0x3F9, 1, 1, nbEventProcess1, nbEventShutdown, e);
    dds3LinkProcess(data->mainproc, id);
    data->eventproc = id;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindStockSkillError);

s32 cmpAnyButtonGuideActive(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if ((list[i]->badstatus & 0xFFF) == 0x800) {
            return 1;
        }
    }
    return 0;
}

s32 cmpButtonGuideShowDone(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if (list[i]->hp < list[i]->maxhp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpButtonGuideHideDone(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if (list[i]->mp < list[i]->maxmp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpFindButtonGuideByFlag(datUnitWork_t **list, s32 cnt, s32 mask)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if ((list[i]->badstatus & 0xFFF) & mask) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", CheckHojo);

/* s32 with no return keeps the one shared epilogue retail has. */
s32 nbDispSkillErrorMessage(s32 type)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    switch (type) {
    case 2:
        nbDispMsgHelp(data->unk_130, 0x42, 3);
        break;
    case 4:
        nbDispMsgHelp(data->unk_130, 0x46, 3);
        break;
    }
    if (type > 0 && (type < 3 || type == 4)) {
        nbPlaySe(0xD);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", nbDigitStringTable);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545DB0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545DC8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError3);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546138);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", cmpClearButtonGuide);

void nbLoadDefaultModelPack(nbMainProcessData_t *data)
{
    if (data->encno == 0x14D && data->modelpack == NULL) {
        data->modelpack = (void *)nbMakeModelPackName(0xFF);
    }
}

s32 nbCheckEventPackLoaded(nbMainProcessData_t *data)
{
    if (data->encno != 0x14D) {
        return 1;
    }
    if (pacCheckEnd(data->modelpack) == 0) {
        return 0;
    }
    data->modelpackbuf = data->modelpack->entry->buf;
    pacFileHandleFree_(data->modelpack);
    data->modelpack = NULL;
    return 1;
}

void nbFreeEventPack(nbMainProcessData_t *data)
{
    if (data->modelpack != NULL) {
        pacFileHandleFree_(data->modelpack);
        data->modelpack = NULL;
    }
    if (data->modelpackbuf != NULL) {
        sdfMemFreeDirect(data->modelpackbuf);
        data->modelpackbuf = NULL;
    }
}

void nbSetModelHandleOnce(nbFormation_t *form, NB_FILEHANDLE_T *packh)
{
    if (form->packh == NULL) {
        form->packh = packh;
    }
}

s32 nbMakeModelPackName(s32 no)
{
    char name[0x40];
    s32 h;

    sprintf(name, D_005464A8, no);
    h = pacLoadUseTail(name);
    printf(D_005464C0, name);
    return h;
}

/* s32 with no return blocks the sibling call retail does not take. */
s32 nbLoadModelPackIfEnabled(nbMainProcessData_t *data, void *buf, s32 no)
{
    if (data->formpack == NULL) {
        if (evtBitCheck(0xC0F)) {
            data->formpack = (nbPacHandle_t *)nbMakeModelPackName(no);
        } else {
            mdlPrepare(buf, no, 0);
            nbLoadMidiFile(no);
        }
    }
}

void nbDecodeModelPack(void *data, s32 size, s32 group, s32 id)
{
    mdlPackDecode(group, id, 0x101, data, size);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMotionData);

void nbCopyPackData(nbMainProcessData_t *data, void *src, u32 size)
{
    void *p;

    if (data != NULL) {
        p = sdfMemAllocDirect(size);
        memcpy(sdfMemLock(p), src, size);
        data->packdata = p;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAddFormEntry);

s32 nbCheckModelPackLoaded(nbMainProcessData_t *data, s32 group, s32 id)
{
    void *buf;

    if (evtBitCheck(0xC0F) != 0) {
        if (pacCheckEnd(data->formpack) == 0) {
            goto zero;
        }
        buf = data->formpack->entry->buf;
        nbAddFormEntry(sdfMemLock(buf), group, id);
        sdfMemFreeDirect(buf);
        pacFileHandleFree_(data->formpack);
        data->formpack = NULL;
        return 1;
    }
    if (mdlPrepare(group, id, 0) == 0) {
        goto zero;
    }
    if (mdlPrepare(group, id, 0) != -1) {
        return 1;
    }
zero:
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", RunUgou);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Wait1_nbE901);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetUp1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SyuGo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SyuGo2);

s32 Cls1(nbEventProcessData_t *e)
{
    func_0024D938(e);
    nbFreeEffectSlot(e);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Daisupe);

s32 ReadWait2(nbEventProcessData_t *e)
{
    if (e->frame >= 0x1F && nbCheckLoadGroup(1) == 0) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

s32 Wait2_nbE901(nbEventProcessData_t *e)
{
    nbFreeEffectSlot(e);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Syutugen2);

s32 End(nbEventProcessData_t *e)
{
    nbCameraBoss_Sp_DOUTYO_Run(pNbCamera);
    if (e->frame < 0x6F) {
        return 0;
    }
    effBTLSkillEnd();
    nbResetAllFormPosRot();
    dds3DeleteSceneBasicData(bedbasic);
    e->data->stat |= 8;
    e->data->press4_p = 0;
    e->data->pressturn = 0;
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE901Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Pos1);

s32 SetForm(nbEventProcessData_t *e)
{
    nbSetEnemyFormation(e->data);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE902_Wait1(nbEventProcessData_t *e)
{
    nbGetEventForm(e);
    if (*(u16 *)(datModelParamTable + 0x3386E) < e->frame) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectCopy);

s32 Bunshin1(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = e->data->form;
    nbAddOngBunshin2Packet(0, (s32)e, &form[5], &form[6], &form[4], &form[7]);
    nbCameraBoss_Ongy_BUNSIN_Run(pNbCamera);
    nbFreeEffectSlot(e);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Wait2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Hontai);

s32 nbE902_End(nbEventProcessData_t *e)
{
    if (e->frame > 0) {
        printf(D_00546500);
        effBTLSkillEnd();
        if (evtBitCheck(0xBF0) != 0) {
            e->data->stat &= ~8;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE902Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Hatudo);

s32 nbE903_Wait1(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm(e);
    /* The animation block the unit basic points at; its end frame sits at +0x2E. */
    if (*(u16 *)((u8 *)form->hunit->basic->poseset + 0x2E) < e->frame) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

s32 Parts1(nbEventProcessData_t *e)
{
    nbAddMotionKoukaPacket(0, 0, nbGetEventForm(e), 0x10);
    nbFreeEffectSlot(e);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsEffect1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsWait);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsEffect2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", EffDel);

s32 nbE903_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x1F) {
        printf(D_00546548);
        e->data->stat |= 0x2000;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE903Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Test);

s32 nbE904_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0xBF) {
        printf(D_00546570);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005464A8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005464C0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005464E0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546500);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546518);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546528);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546538);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546548);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546570);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE904Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model1Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model1Wait);

s32 Model1Wait_2(nbEventProcessData_t *e)
{
    void *buf;
    s32 size;

    if (e->frame == 0x1E) {
        func_0024D938(e);
    }
    if (e->frame >= 0x20) {
        if (evtBitCheck(0xC0F) == 0) {
            buf = sdfMemLock(e->unk_24);
            size = sdfMemGetSize(e->unk_24);
            mdlPackDecode(1, 0x102, 0x101, buf, size);
        }
        nbAddFadeInKoukaPacket(3, 0, 0x28, 1);
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE905_Model2Start);

s32 func_002930E8(nbEventProcessData_t *e)
{
    dds3DrawFlag |= 0x2000000;
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE905_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x24) {
        if (nbMaeCheckDone() != 0) {
            printf(D_005467E0);
            nbMaeWaitStep();
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE905Process_1);

s32 nbE906_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm(e);
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x38A, 1), 2, form);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x38A, 2), 2, form);
    nbAddFadeOutKoukaPacket(0x140, 0, 0x46, 1);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE906_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x1A5) {
        printf(D_005467F0);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE906Process_1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005467E0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005467F0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model1Start);

s32 nbE907_Model1Wait(nbEventProcessData_t *e)
{
    if (e->frame >= 0x8C) {
        if (pacCheckEnd(e->unk_20) != 0) {
            if (evtBitCheck(0xC0F) == 0) {
                e->unk_24 = fileGetMemHandle(e->unk_1C);
                fileHandleFree(e->unk_1C);
                e->unk_1C = NULL;
            }
            nbAddFadeOutKoukaPacket(0, 0, 0xA, 1);
            nbFreeEffectSlot(e);
        }
    }
    return 0;
}

s32 nbE907_Model1Wait_2(nbEventProcessData_t *e)
{
    void *p;

    if (e->frame == 0xA) {
        func_0024D938(e);
    }
    if (e->frame >= 0xD) {
        if (evtBitCheck(0xC0F) == 0) {
            p = sdfMemLock(e->unk_24);
            mdlPackDecode(1, 0x103, 0x101, p, sdfMemGetSize(e->unk_24));
        }
        nbAddFadeInKoukaPacket(3, 0, 0xA, 1);
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model2Start);

s32 Model2Wait_1(nbEventProcessData_t *e)
{
    nbFormation_t *form;
    void *res;

    form = nbGetEventForm(e);
    evtSetUnitMotion(form->hunit, 0x11, 0xA, 0, 0);
    dds3DrawFlag |= 0x2000000;
    res = evtGetResorceData(0x38B, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, res, 2, form);
    nbFreeEffectSlot(e);
    return 0;
}

s32 Model2Wait_15(nbEventProcessData_t *e)
{
    if (nbMaeCheckDone() != 0) {
        nbMaeWaitStep();
    }
    if (e->frame >= 0x76) {
        if (nbMaeCheckDone() != 0) {
            nbFreeEffectSlot(e);
        }
    }
    return 0;
}

s32 nbE907_End(nbEventProcessData_t *e)
{
    if (e->frame > 0) {
        printf(D_00546820);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907Process_1);

s32 nbE908_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm(e);
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x38C, 1), 2, form);
    nbAddFadeOutKoukaPacket(0xBE, 0, 0x46, 1);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE908_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x10F) {
        printf(D_00546830);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE908Process_1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546820);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546830);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model1Start);

s32 nbE909_Model1Wait(nbEventProcessData_t *e)
{
    if (e->frame >= 0x64) {
        if (pacCheckEnd(e->unk_20) != 0) {
            if (evtBitCheck(0xC0F) == 0) {
                e->unk_24 = fileGetMemHandle(e->unk_1C);
                fileHandleFree(e->unk_1C);
                e->unk_1C = NULL;
            }
            nbAddFadeOutKoukaPacket(0, 0, 0xA, 1);
            nbFreeEffectSlot(e);
        }
    }
    return 0;
}

s32 nbE909_Model1Wait_2(nbEventProcessData_t *e)
{
    void *p;

    if (e->frame == 0xA) {
        func_0024D938(e);
    }
    if (e->frame >= 0x1E) {
        if (evtBitCheck(0xC0F) == 0) {
            p = sdfMemLock(e->unk_24);
            mdlPackDecode(1, 0x108, 0x101, p, sdfMemGetSize(e->unk_24));
        }
        nbAddFadeInKoukaPacket(3, 0, 0xA, 1);
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model2Start);

s32 nbE909_Model2Wait_1(nbEventProcessData_t *e)
{
    evtSetUnitMotion(nbGetEventForm(e)->hunit, 0x11, 0, 0, 0);
    dds3DrawFlag |= 0x2000000;
    e->data->encno = 0x30E;
    nbSetMaeFlag(0);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE909_Model2Wait_15(nbEventProcessData_t *e)
{
    if (e->frame >= 0x51) {
        if (nbMaeCheckDone() != 0) {
            nbMaeWaitStep();
            nbFreeEffectSlot(e);
        }
    }
    return 0;
}

s32 nbE909_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x1A) {
        printf(D_00546860);
        e->data->stat &= ~8;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909Process_1);

s32 nbE910_Test(nbEventProcessData_t *e)
{
    nbPanelAllHide();
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE910_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x15) {
        printf(D_00546870);
        e->data->endtype = 8;
        return 1;
    }
    return 0;
}

s32 nbE910Process_1(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (D_00527518[e->step](e) != 0) {
        return -1;
    }
    nbUpdateEventProcessData(id);
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Effect1);

s32 Wait1(nbEventProcessData_t *e)
{
    if (e->frame == 1) {
        nbSetEnemyFormation(e->data);
        nbAllFormDispOn();
        nbGroupFormDispOff(0);
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Effect2);

s32 nbE911_End(nbEventProcessData_t *e)
{
    if (e->frame >= 2) {
        printf(D_00546880);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE911Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Hatudo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Wait1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", MoveSub);

s32 Move1(nbEventProcessData_t *e)
{
    MoveSub(e);
    if (e->frame == 0x55 && nbCheckLoadGroup(1) == 0) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Effect1);

s32 nbE912_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x10) {
        printf(D_00546898);
        e->data->stat &= ~8;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nb_295D98);

s32 nbE913_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm(e);
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x391, 1), 2, form);
    nbAddFadeOutKoukaPacket(0x91, 0, 0x28, 1);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE913_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0xD3) {
        printf(D_005468B0);
        nbDelAllKoukaPacketType(0x2B);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

s32 nbE913Process_1(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (D_00527688[e->step](e) != 0) {
        return -1;
    }
    if (e->unk_08 == 0) {
        nbCameraSetState(0x17);
        nbCameraBoss_Foru_E913_Set();
    }
    nbCameraBoss_Foru_E913_Run();
    nbUpdateEventProcessData(id);
    return 0;
}

s32 nbE914_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm(e);
    nbBgmStop();
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x392, 1), 2, form);
    nbAddFadeOutKoukaPacket(0xE6, 0, 0x28, 1);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE914_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x10F) {
        printf(D_005468C8);
        nbDelAllKoukaPacketType(0x2B);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE914Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE915_Test);

s32 nbE915_End(nbEventProcessData_t *e)
{
    if (e->frame >= 0x173) {
        printf(D_005468D8);
        nbDelAllKoukaPacketType(0x2B);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE915Process_1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546860);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546870);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546880);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546898);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005468B0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005468C8);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005468D8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551140);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551148);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551150);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551158);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551160);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", gnbPnlOtOfs);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPnlDrawFlag);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPnlCnt);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551174);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551178);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelBaseABGR);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelBaseRGBA);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551188);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGPress);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGNumber);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGTurn);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGTurnOld);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGFrame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGBirthWait);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGStart);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGShow);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGEncount);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPGBaseColor);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTOmit);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005511AD);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_1_Process);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_1_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_2_Process);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_2_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_2_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_3_Process);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_3_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPT_3_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTP_0_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTP_0_Process);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005511D8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTP_1_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTP_1_Process);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTP_1_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTAProcess);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTAFrame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTARate_S);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTARate_E);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelPTAOnly);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005511F9);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005511FC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockRun);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockPad);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockABGR);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockFadeFlag);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551209);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockFrame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelStockShiftX);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", StockIDListCount);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", StockIDListCursor);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", StockIDListTop);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelAnalyzeProcess);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelAnalyzeFrame);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbPanelAnalyzeRate);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", pNbPanelAnalyzeUnitWork);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551230);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", PanelAutoABGR);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", PanelAutoBaseColor);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", PanelAutoShow);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", PanelPartyShow);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551240);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551248);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551250);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551258);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551260);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551268);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551270);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551278);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551280);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551288);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551290);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551298);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512A0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512A8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512B0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512B8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512C0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512C8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512D0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", menucur);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", badcur);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512E0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", slSelectMotIndex);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512E8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512EC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512F0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005512F8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551300);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551308);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551310);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551318);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551320);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551328);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551330);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551338);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551340);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551348);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551350);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551358);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551360);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551368);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551370);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551378);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551380);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551388);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551390);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551398);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513A0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513A8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513B0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513B8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513C0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513C8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513D0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513D8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513E0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513E8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513F0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005513F8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551400);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551408);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551410);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551418);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551420);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551428);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551430);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551438);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551440);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551448);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551450);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551458);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551460);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551468);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551470);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551478);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551480);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551488);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551490);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551498);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514A0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514A8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514B0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514B8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514C0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514C8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514D0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514D8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514E0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", btl_ai_rot_cnt);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514EC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514F0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005514F8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbFormEnemyInterval);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551500);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", pack);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", f);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551514);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", test_phearts);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbEncountMax);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", bgmno);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbBgmSaved);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbBgmFadeState);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbResultCount);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbMaeState);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbMaeCounter);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbMaeFlag);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551540);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", readbasetime);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", readtime);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551550);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", nbBadCurseTimer);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_0055155C);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551560);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551568);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551570);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551578);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551580);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551588);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551590);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551598);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515A0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515A8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515B0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515B4);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", bedbasic);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515BC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515C0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515C4);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515C8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515CC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515D0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515D4);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515D8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515DC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515E0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515E4);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515E8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515EC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515F0);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515F4);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515F8);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_005515FC);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551600);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551604);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551608);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_0055160C);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551610);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551614);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551618);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_0055161C);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551620);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551624);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551628);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_0055162C);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551630);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551634);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551638);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_0055163C);

INCLUDE_SDATA(const s32, "newbattle/nbMakePacket", D_00551640);

