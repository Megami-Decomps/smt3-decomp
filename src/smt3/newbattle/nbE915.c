#include "common.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    dds3Basic_t *basic;
    u8 unk_90[0x14];
    u32 flags;
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
    u8 unk_7E[0x6];
    s32 unk_84;
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
    u8 unk_E0[0x14];
    dds3ProcessID_t *koukaproc;
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

/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 stat;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 select;
    u8 unk_40[0x28];
    s32 unk_68;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 aichktbl[0x8];
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

extern s32 btl_ai_rot_cnt;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern char D_005514F0[];

extern s32 D_00551500;

extern u32 f;

extern s32 test_phearts;

extern char D_005467C0[];

extern char D_00546800[];

extern char D_00546840[];

extern dds3Basic_t *D_005515B4;

extern void effBTLSkillStart(s32 id, s32 stat);

extern f32 D_005514F8;

extern s32 pack;

extern s32 nbEncountMax;

extern s32 nbResultCount;

extern s32 bgmno;

extern s32 nbBgmSaved;

extern s32 nbBgmFadeState;

extern void nbSetEnemyFormation(nbMainProcessData_t *data);

extern char D_00546820[];

extern char D_00546880[];

extern char D_00551560[];

extern s32 nbBadCurseTimer;

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 eventrid;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *unk_20;
    void *mem;
} nbEventProcessData_t;

extern char D_00551160[];

extern u32 menucur;

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 readbasetime;

extern s32 readtime;

extern char D_00551550[];

extern char D_005467E0[];

extern char D_00546860[];

extern char D_00546898[];

extern char D_005468B0[];

extern char D_005468C8[];

extern char D_005468D8[];

extern char D_005467F0[];

extern char D_00546830[];

extern char D_00546870[];

extern void *bedbasic;

extern s32 D_005515BC;

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 nbMaeState;

extern s32 nbMaeCounter;

extern s32 nbMaeFlag;

extern s32 D_00551540;

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void nbCameraSetState(s32 state);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

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

void nbFormDispOff2(nbFormation_t *form);

void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

nbFormation_t *nbGetEventForm();

void nbUpdateEventProcessData(dds3ProcessID_t *id);

/* NON_MATCHING: 13 of 69 words differ (first at +0x38). */
#ifdef NON_MATCHING
s32 nbE915Process_1(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (D_00527698[e->seq](e) != 0) {
        return -1;
    }
    if (e->time == 0) {
        nbCameraSetState(0x17);
        nbSetEnemyFormation(e->data);
        nbSetPlayerFormation(e->data, 0);
    }
    if (e->time < 0x6E) {
        if (e->time == 0) {
            nbCameraBoss_BSBERU_E915_Init();
        }
        nbCameraBoss_BSBERU_E915_0_Set();
    } else if (e->time == 0x6E) {
        nbCameraBoss_BSBERU_E915_1_Set();
    } else if (e->time >= 0xFA) {
        if (e->time == 0xFA) {
            nbFormDispOff2(nbGetEventForm());
            nbCameraBoss_BSBERU_E915_Init();
        }
        nbCameraBoss_BSBERU_E915_2_Set();
    }
    if (e->time == 0) {
        effBTLSkillStart(0x203, 0);
    }
    nbUpdateEventProcessData(id);
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbE915", nbE915Process_1);
#endif

