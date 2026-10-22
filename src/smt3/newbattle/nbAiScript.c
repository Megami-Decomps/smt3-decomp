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
    u32 unk_04;
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

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

extern void nbPanelAutoHide(void);

extern nbActionProcessData_t *scrGetData(void);

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

extern void nbSetNormalSkill0(datUnitWork_t *work);

extern s32 nbSetActionCommand(nbMainProcessData_t *data, s32 command);

extern s32 nbSetAiTarget(nbActionProcessData_t *a, s32 code, s32 n);

extern s32 nbGetSelectBit(nbMainProcessData_t *data);

extern s32 nbAiChkJoken(nbActionProcessData_t *a, u32 jo);

extern s32 nbGetHojoCounter(s32 formindex, s32 type);

extern s32 nbGetAisyoTable(s32 formindex);

extern s32 evtGetAgeOfMoon16(void);

extern s32 nbGetEscapeSwitch(nbMainProcessData_t *data);

extern void nbRunAi(nbActionProcessData_t *a);

extern void nbRunAiSub(nbActionProcessData_t *a, s32 tbl);

extern void *memset(void *a, s32 b, u32 c);

extern f32 scrGetFloatPara(s32 n);

extern void nbCameraScriptSet(sdfVector4_t *pPos, sdfVector4_t *pQ);

extern void nbCameraScriptSet1Shot(nbFormation_t *pTargetForm);

extern void nbCameraScriptSetBougai(nbFormation_t *pTargetForm);

extern void nbPanelAllHide(void);

extern void nbAllFormDispOn(void);

extern void nbGroupFormDispOn(s32 side);

extern void nbGroupFormDispOff(s32 side);

extern s32 btl_ai_rot_cnt;

extern dds3ProcessID_t *scrStartScript2(s32 priority, u8 *scrdata, s32 startproc);

extern void scrSetUserData(dds3ProcessID_t *id, void *p);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern u8 scrEmbedded_4F0800[];

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *unk_20;
    void *mem;
} nbEventProcessData_t;

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

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
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LIFEMIN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 1, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_BAARLV(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 9, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LVMIN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 10, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_BAD(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 2, scrGetIntPara(0)) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_ID(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 3, scrGetIntPara(0)) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_LIGHT(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 4, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_DARK(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 5, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_PLAYER(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 6, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_RND(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 8, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

s32 nbCommand_AI_TAR_JOKEN(void)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    if (!nbSetAiTarget(a, 7, 0) || !nbSetAiTarget(a, 0, 0)) {
        a->select = nbGetSelectBit(a->data);
    }
    return 1;
}

static void AiChkJoken(s32 c)
{
    nbActionProcessData_t *a;

    a = scrGetData();
    a->aiselect = 0;
    if (nbAiChkJoken(a, c)) {
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
INCLUDE_SDATA(const s32, "newbattle/nbAiScript", btl_ai_rot_cnt);

INCLUDE_SDATA(const s32, "newbattle/nbAiScript", D_005514EC);

