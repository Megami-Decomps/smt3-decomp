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

extern char D_00544890[]; /* "カット長 = %d\n" */

extern nbKoukaProcessData_t *nbGetKoukaProcessData(void);

extern nbKoukaPacket_t *nbAddKoukaPacket(nbKoukaProcessData_t *k, s32 startframe, s32 framelength,
                                         s32 uniqueid);

extern effBTL_t *effBTLCreate(u8 *pData);

extern effBTLCutHandle_t *effBTLCutCreate(effBTL_t *hBTLEffect, u16 CutNum, nbFormation_t *pFormation, s32 stat);

extern s32 effBTLGetCutEndFrame(effBTL_t *hBTLEffect, u16 CutNum);

extern s32 printf(const char *format, ...);

extern nbKoukaPacket_t *nbAddSePacket(s32 startframe, s32 uniqueid, s32 se);

extern s32 nbCheckSePacket(s32 start, s32 se, s32 frame);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern void *effBTLSystemCreate(nbFormation_t *pFormation, u16 id);

extern s32 effBTLSystemGetLength(void *hEffBTLSys);

extern s32 effPCPBossBUNSINGetLength(void);

extern void *effPCPBossBUNSINCreate(effPCPBossBUNSINParam_t *pParam);

extern s32 effPCPBossBUNSIN2GetLength(void);

extern void *effPCPBossBUNSIN2Create(effPCPBossBUNSIN2Param_t *pParam);

extern s32 effPCPBossONGYGetLength(void);

extern void *effPCPBossONGYCreate(effPCPBossONGYParam_t *pParam);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern u32 nbGetBadStat(s32 formindex);

extern s32 nbCheckEventDead(nbFormation_t *form);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);

extern s32 effPCPBossONGY2GetLength(void);

extern void *effPCPBossONGY2Create(effPCPBossONGY2Param_t *pParam);

extern void *memset(void *a, s32 b, u32 c);

extern u32 f;

extern void ToMatrix(void);

extern f32 nbMakeUnitMoveGetCyDist(nbFormation_t *i_form, nbFormation_t *t_form);

extern s32 sdfFloatToULong(f32 value);

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 eventrid;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *userdata;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *pfile;
    void *mem;
} nbEventProcessData_t;

extern nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);

extern nbKoukaPacket_t *nbAddSePacket2(s32 startframe, s32 uniqueid, s32 se);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern nbKoukaPacket_t *nbMakeKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                            s32 type, s32 hp, s32 mp, s32 badstat);

extern s32 nbMakeDamagePacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 type, s32 hp,
                              s32 mp, s32 badstat, s32 hiteff, s32 dead, s32 deadtype, s32 moji,
                              s32 animeoff);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern nbKoukaPacket_t *nbAddFadeInKoukaPacket(s32 startframe, s32 uniqueid, s32 length,
                                               s32 coltype);

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

nbKoukaPacket_t *nbAddRotKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno, sdfVector4_t *v,
                                     s32 speed)
{
    nbKoukaPacket_t *p;

    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 3;
    p->partyno = partyno;
    SDF_VU0_COPYVECTOR(&p->v, v);
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
    printf("length = %d\n", p->framelength);
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

nbKoukaPacket_t *nbAddSePacket(s32 startframe, s32 uniqueid, s32 se)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, se, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 25;
    p->data1.i = se;
    return p;
}

nbKoukaPacket_t *nbAddSePacket2(s32 startframe, s32 uniqueid, s32 se)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, se, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 28;
    p->data1.i = se;
    return p;
}

nbKoukaPacket_t *nbAddSePacketLock(s32 startframe, s32 uniqueid, s32 se)
{
    nbKoukaPacket_t *p;

    if (nbCheckSePacket(startframe, se, 5) != 0) {
        return NULL;
    }
    p = nbAddKoukaPacket(nbGetKoukaProcessData(), startframe, 0, uniqueid);
    p->type = 29;
    p->data1.i = se;
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
                                        nbFormation_t *form1, nbFormation_t *form2,
                                        nbFormation_t *form3)
{
    effPCPBossBUNSIN2Param_t param;
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;
    void *h;

    k = nbGetKoukaProcessData();
    p = nbAddKoukaPacket(k, startframe, effPCPBossBUNSIN2GetLength(), uniqueid);
    param.pStartForm = sform;
    param.pForm1 = form1;
    param.pForm2L = form2;
    param.pForm2R = form3;
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

/* NON_MATCHING: 2 of 65 words differ (first at +0x8C). */
#ifdef NON_MATCHING
s32 nbMakeNegoBackPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                         nbFormation_t *target)
{
    sdfVector4_t v;
    sdfVector4_t pos;
    sdfVector4_t dir;

    memset(&dir, 0, sizeof(dir));
    dir.z = 1.0f;
    SDF_VU0_LOADVECTOR(&target->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&v);
    nbMakeUnitMoveGetCyDist(form, target);
    SDF_VU0_LOADVECTOR(&v);
    SDF_VU0_SCALEVECTOR(form->r * form->scl);
    SDF_VU0_LOADVECTOR_B(&form->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    nbAddTargetMoveKoukaPacket(startframe, uniqueid, form->partyindex, &pos, 0.05f);
    nbMakeUnitFadeOutPacket(startframe, uniqueid, form, 0, 4, 5, 5);
    return 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeNegoBackPacket);
#endif

s32 nbMakeNegoForwardPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                            nbFormation_t *target)
{
    sdfVector4_t v;
    sdfVector4_t pos;
    sdfVector4_t dir;
    f32 dist;
    f32 scale;

    memset(&dir, 0, sizeof(dir));
    dir.z = 1.0f;
    SDF_VU0_LOADVECTOR(&target->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&v);
    dist = nbMakeUnitMoveGetCyDist(form, target);
    SDF_VU0_LOADVECTOR(&v);
    scale = target->r * target->scl + 100.0f + dist;
    SDF_VU0_SCALEVECTOR(scale);
    SDF_VU0_LOADVECTOR_B(&target->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    nbAddTargetMoveKoukaPacket(startframe, uniqueid, form->partyindex, &pos, 0.05f);
    nbMakeUnitFadeInPacket(startframe, uniqueid, form, 0, 4, 5, 5);
    return 1;
}
