#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "nb.h"
#include "basic.h"

/* Action operands share the prefix used by code_0024C470. */
struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 unk04;
    s8 seq;
    s8 nextseq;
    s16 unk0A;
    s32 unk0C;
    u8 unk10[8];
    s32 unk18;
    u8 unk1C[0xC];
    nbFormation_t *form;
    datUnitWork_t *work;
};

/* Sprite payload returned by sdf2DGetPrimPtr: one colour and two UV/XYZ
 * corners. Each corner occupies two quadwords; the full payload is 0x50. */
typedef struct
{
    s32 u, v;
    u8 unk08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 unk1E[2];
} sdf2DSpriteCorner_t;

typedef struct
{
    s32 r, g, b, a;
    sdf2DSpriteCorner_t corner[2];
} sdf2DSpritePrim_t;



typedef struct
{
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t write;
} nbGsRegisterPacket_t;

extern s32 gnbPnlOtOfs;
extern s8 nbPanelPGShow;
extern s8 nbPanelPTOmit;
extern dds3ProcessID_t *nbPanelPTAProcess;
extern s8 nbPanelStockRun;
extern s8 nbPanelStockFadeFlag;
extern s32 nbPanelAnalyzeProcess;
extern s32 nbPanelAnalyzeFrame;
extern datUnitWork_t *pNbPanelAnalyzeUnitWork;
extern s8 PanelAutoShow;
extern s8 PanelPartyShow;
extern s32 nbEncountMax;
extern s32 bgmno;
extern s32 nbBgmFadeState;
extern sdfGrOt_t sdfGrOtTable[];

extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void *sdf2DMakeStdPacket(s32 packet, s32 size);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdf2DAllocSprite(s32 count);
extern void *sdf2DGetPrimPtr(s32 packet);
extern void sdf2DLinkTexPacket(sdfDrawTag_t *tag, sdfTexHandle_t *texture, s32 index);
extern void nbPanelPressGaugeShow(void);
extern void nbPanelAutoShow(void);
void nbPanelPartyShow(void);
void nbPanelAutoHide(void);
void nbPanelPartyHide(void);
void nbPanelPressGaugeHide(void);
extern void *scrGetData(void);
extern void scrSetIntReturnValue(s32 value);
extern void nbRunAi(void *work);
extern void nbCameraScriptSet1Shot(void *camera);
extern void nbCameraScriptSetBougai(void *camera);
extern void nbAllFormDispOn(void);
extern void nbSetPartyFormation(s32 formation, s32 mask);
extern void SetBossDummy(void);
s32 TestProcess1(void);
extern void p2sdPlaySE(u32 soundId, s32 volume, s32 pan);
extern u32 nbGetUnitWorkFromAction(struct nbActionProcessData_t *action);
extern void nbLoadMotionByIndex(u32 unitId, s32 motion);
extern void nbMakeSeId(u32 unitId);
extern void p2sdPlayBGM(s32 soundId);
extern void p2sdStopBGM(s32 soundId);
extern void func_002C1E70(s32 volume);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void sdfChipFreeDirect(void *work);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void pacFileHandleFree_(void *pack);
extern void nbFreeEffectSlot(void);

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
typedef struct effBTLCutHandle_t effBTLCutHandle_t;
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
typedef struct nbActionProcessData_t nbActionProcessData_t;
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
    u8 unk_02[0x24];
} nbEncountForm_t;


extern char D_00544890[]; /* "カット長 = %d\n" */
extern char D_00544900[]; /* "length = %d\n" */

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
extern u32 nbGetAisyo(s32 nskill, s32 dformindex, s32 skillattr);

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
    PS2_COPY_QUAD(&p->v, pos);
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
    PS2_COPY_QUAD(&p->v, rot);
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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetLvAvg);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetLvAvg2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetParamAvg);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetParamAvg2);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoYaruMakkaA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoYaruMakkaB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureMakkaA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureMakkaB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureHpA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureHpB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureMpA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoKureMpB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKureItemA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSelectKureItemA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetKureItemB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSelectKureItemB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoRequestType);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoTuikaRequestType);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetAllHojoSkillWork);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetHojoSkillWorkCnt);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSearchHojoSkillWork);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetQuestionType);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoYaruItemA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoYaruItemB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetNegoYaruStone);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetInfoMsg);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckSelectMesJoukenAri);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetNegoNoMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetNegoYesMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFlowFindUnitEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoDebugPrint);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoDebugPrint2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", NegoDebug);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoProcessMessageChk);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoProcessMessage);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoSetProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoShutdown);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", InitNegoProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitNegoProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_33);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_32);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_25);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_26);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_27);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_28);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_29);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_30);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_31);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_34);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_35);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_36);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_37);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_38);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow1_39);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowOne);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFlowGetUnitEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFlowCheckUnit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFlowCheckUnit2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_80);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_81);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_70);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_25);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_26);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_27);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_28);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_29);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_30);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_31);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_32);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_33);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_34);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_35);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_36);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_37);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_38);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_39);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_40);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_41);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_42);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_43);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_44);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_45);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_46);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_47);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_48);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_49);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_50);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_51);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_52);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545068);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_53);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_74);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_54);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_55);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_56);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_57);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_58);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_59);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_71);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_60);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_72);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_61);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_73);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_62);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_63);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_64);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_65);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_66);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_67);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow6_69);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowTwo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_6);

s32 Flow9_7(void)
{
    return 9;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow9_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowOk);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckFlowUnit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_33);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_25);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_26);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_27);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_28);

s32 Flow10_29(void)
{
    return 2;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_30);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_31);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow10_32);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowNg);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow2_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowFullMoon);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow3_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow3_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowStock);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow4_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowDouchara);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_4);

s32 Flow5_5(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_6);

s32 Flow5_7(void)
{
    return 0;
}

s32 Flow5_8(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_9);

s32 Flow5_10(void)
{
    return 0;
}

s32 Flow5_11(void)
{
    return 0;
}

s32 Flow5_12(void)
{
    return 0;
}

s32 Flow5_13(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_15);

s32 Flow5_16(void)
{
    return 0;
}

s32 Flow5_17(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_18);

s32 Flow5_19(void)
{
    return 0;
}

s32 Flow5_20(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow5_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowInnen);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_25);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow7_26);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowQuestion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow8_18);

s32 Flow8_19(void)
{
    return 4;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowFight);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow11_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowDrain);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow14_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow14_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowBinjo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_24);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_25);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_16);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_20);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_21);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_22);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_26);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_27);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_28);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_29);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_30);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_31);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_35);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_36);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_37);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_38);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_39);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_40);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_41);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_42);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_43);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_44);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_45);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_46);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_47);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow12_48);

s32 Flow12_49(void)
{
    return 4;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowMukan);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_9);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow13_10);

s32 Flow13_11(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowSensei);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_11);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_12);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_13);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_14);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_17);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_18);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_19);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_20);

s32 Flow15_22(void)
{
    return 4;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow15_23);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowInoti);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow16_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow16_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Flow16_3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbNegoFlowKoketa);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnlLoad);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnlTexSet);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnlTexDel);

void nbPnl_setOt(s32 ot)
{
    gnbPnlOtOfs = ot;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPnl_rsetTex1);

void nbPnl_rsetTest(s32 alphaTestEnable, s32 alphaTestMethod, s32 alphaReference,
                   s32 alphaFail, s32 destinationAlphaEnable, s32 destinationAlphaMethod,
                   s32 unused, s32 depthTestMethod) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    nbGsRegisterPacket_t *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = (s32)sdfGrAllocPacketBuff(0x30);
    data = (nbGsRegisterPacket_t *)sdf2DMakeStdPacket(packet, 0x30);
    data->write.data = (depthTestMethod << 17) | 0x10000 | (destinationAlphaMethod << 15) |
                       (destinationAlphaEnable << 14) | (alphaFail << 12) |
                       (alphaReference << 4) | (alphaTestMethod << 1) | alphaTestEnable;
    data->write.reg = 0x47;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[gnbPnlOtOfs];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void nbPnl_rsetAlpha(s32 mode) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    nbGsRegisterPacket_t *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = (s32)sdfGrAllocPacketBuff(0x30);
    data = (nbGsRegisterPacket_t *)sdf2DMakeStdPacket(packet, 0x30);
    switch (mode) {
    case 1:
        data->write.data = 0x48;
        break;
    case 2:
        data->write.data = 0x42;
        break;
    case 3:
        data->write.data = 0x84;
        break;
    case 4:
        data->write.data = 0x06;
        break;
    case 5:
        data->write.data = 0x89;
        break;
    case 6:
        data->write.data = 0x42;
        break;
    case 10:
        data->write.data = 0x2A;
        break;
    case 20:
        data->write.data = 0x4A;
        break;
    default:
        data->write.data = 0x44;
        break;
    }
    data->write.reg = 0x42;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[gnbPnlOtOfs];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    sdf2DSpritePrim_t *vtx = (sdf2DSpritePrim_t *)sdf2DGetPrimPtr((s32)handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = color & 0xFF;
    vtx->g = (color >> 8) & 0xFF;
    vtx->b = (color >> 16) & 0xFF;
    vtx->a = (color >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, 0);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[gnbPnlOtOfs];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelBaseColorSet);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSpriteCenteringDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSpriteScaleDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSpriteScaleWHDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSpriteBlendDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSmallStringDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelSmallNameStringDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCenterText);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelHPColor);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelMPColor);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelCursorDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonScaleDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPolygonFlashDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawPP0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545300);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawPP1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDrawPP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGaugeInit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGaugeStart);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGaugeReset);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGaugeShow);

void nbPanelPressGaugeHide(void)
{
    nbPanelPGShow = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGaugeEncountHide);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressGauge);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurnInit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurnRun);

s32 nbPanelPressTurnGaugeRun(void)
{
    return nbPanelPTAProcess != NULL;
}

void nbPanelPressTurnOmit(void)
{
    nbPanelPTOmit = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurnStart);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurnGaugeAnimStart);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurnGaugeAnimStart2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPressTurn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelPartySelect);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelStockInit);

s8 nbPanelStockCheckRun(void)
{
    return nbPanelStockRun;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelStockEnd);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelAutoShow);

void nbPanelAutoHide(void)
{
    PanelAutoShow = 0;
}

s8 nbPanelAutoCheckShow(void)
{
    return PanelAutoShow;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelAutoInit);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelAuto);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelDraw);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelRunProcess);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelShutdown);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitPanelProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelFlashInit);

u32 nbPanelFlashRGBAMulti(u32 colorA, u32 colorB);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPanelFlashRGBAMulti);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", DispHpMpBad);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMenu);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectHpMp);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectBad);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMotData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectMot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectDevil);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetSelect);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetDvlChange);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetTarget_nbDebugProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", BackUpHpMpBad);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545698);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetNextSeq);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDebugProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDebugShutdown);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", InitDebugProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitDebugProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_MYHP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_MYMP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRHP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_DMGCNT_285578);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_CHKCNT_2855D8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRCNT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENCNT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_MYBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENALLBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkAiHojo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkAiHojoMax);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENHOJO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRHOJO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENHOJOMAX);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_FRHOJOMAX);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_BA_286248);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_POW_2862D0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENLIGHT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENDARK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_SENSEI_286530);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_PLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_PPARTYLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_TURN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_ENHAMAMHK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Chk_SKILLNOMP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAiChkJoken);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetAiEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPickAiAction);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetKoudou);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_AI);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_LIFEMIN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_BAARLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_LVMIN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_BAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_ID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_LIGHT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_DARK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_PLAYER);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_JOKEN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Sel_RND);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetSingleTarget2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetGroupTarget2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkCost);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ChkClose);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetAiTarget);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRunAiSub);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRunAi);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_ACT_ATTACK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_ACT_ESCAPE);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_ACT_WAIT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_ACT_SKILL);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_ACT_SUMMON);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_SET_CRITICAL);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_AI);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_LIFEMIN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_BAARLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_LVMIN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_BAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_ID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_LIGHT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_DARK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_PLAYER);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_RND);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TAR_JOKEN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", AiChkJoken);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_MYHP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_MYMP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRHP);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_DMGCNT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRCNT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENCNT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_MYBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENALLBAD);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRID);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENHOJO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRHOJO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENHOJOMAX);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_FRHOJOMAX);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_BA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENLIGHT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENDARK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_POW);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_PLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_PPARTYLV);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_GET_TURN);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_GET_ALLKOUDO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_GET_PRESSKOUDO);

s32 nbCommand_AI_GET_MYLV(void)
{
    nbActionProcessData_t *action = scrGetData();

    scrSetIntReturnValue(action->work->level);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_GET_MYAISYO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_GET_FUREA);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ESCAPE);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_ENHAMAMHK);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_CHK_SKILLNOMP);

s32 nbCommand_AI(void)
{
    nbRunAi(scrGetData());
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_AI_TBL);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_CAM_SET);

s32 nbCommand_BTL_CAM_ORG(void)
{
    return 1;
}

s32 nbCommand_BTL_CAM_1SHOT(void)
{
    nbActionProcessData_t *action = scrGetData();

    nbCameraScriptSet1Shot(action->form);
    return 1;
}

s32 nbCommand_BTL_CAM_2SHOT(void)
{
    return 1;
}

s32 nbCommand_BTL_CAM_BOUGAI(void)
{
    nbActionProcessData_t *action = scrGetData();

    nbCameraScriptSetBougai(action->form);
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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_EVENT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_TAIKYO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_ALLTAIKYO);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_PRESS);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_NAKAMA);

s32 nbCommand_BTL_FORM_ALL_ON(void)
{
    nbAllFormDispOn();
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_FORM_PLY_ON);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_FORM_ENE_ON);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_AI_ROT_CLR);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_AI_ROT_NEXT);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_AI_ROT_GET);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_AI_CHANGE_OFF);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCommand_BTL_AI_CHANGE_ON);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitAiScript);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispText);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispSkillName);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispTextById);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nb_289078);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545850);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545868);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispActionText);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nb_2891A8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispDevilName);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispJyokyoHelp);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispMsgHelp);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbHelpOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDrawHelpPanel);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbHelpProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbHelpShutdown);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAllocHelpWork);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitHelpProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAutoCommProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitAutoCommProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFormDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFormDispOff2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFormDispOn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFormDispOn2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGroupFormDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGroupFormDispOn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAllFormDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAllFormDispOn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSingleFormExceptedDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDoubleFormExceptedDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRestartMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetFormPos);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetFormRot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetFormPosRot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbResetFormPosRot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbResetAllFormPosRot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadUnitPos);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", effBTLModelLoadRotate);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", func_0028A330);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEnemyInterval);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005458B0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetPartyFormation);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetEnemyFormation);

void nbSetPlayerFormation(s32 formation)
{
    nbSetPartyFormation(formation, 0x100);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFormatFormation);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCopyFormation);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMoveFormation);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbUpdateFormation);

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

s32 TestProcess7(void)
{
    SetBossDummy();
    return (s32)TestProcess1;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", TestProcess6);

s32 TestProcess1(void)
{
    return 0;
}

void TestShutdown(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbExitBattleTest);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitBattleTest);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountJoken);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountMax);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEncountCnt);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncountCalc);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckEncountLevel);

void fclClearTerminalState(void)
{
    DDS3_GBWK->encounterRemainder = 0;
    DDS3_GBWK->encounterGauge = 0;
    nbEncountMax = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", func_0028CFC8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitMaeTableDefault);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitMaeTable);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetUnitWorkFromAction);

void nbPlaySe(u32 soundId)
{
    p2sdPlaySE(soundId, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetMotionTable);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMotionByIndex);

void nbGetUnitFromAction(nbActionProcessData_t *action, s32 motion)
{
    nbLoadMotionByIndex(nbGetUnitWorkFromAction(action), motion);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeSeId);

void nbPlayMotionSe(nbActionProcessData_t *action)
{
    nbMakeSeId(nbGetUnitWorkFromAction(action));
}

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545B90);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545BA0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMemoryLoadNegoSe);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMemoryLoadMotionSe);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetFormEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetFormEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMidiFile);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckPackLoad);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindFreeFormSlot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindFormIndex);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeFormResources);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeAllFormResources);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAllocMainProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMaeCheckDone);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C38);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C58);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545C78);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetMaeFlag);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbIncResultCounter);

void nbMaeWaitStep(void)
{
    p2sdPlayBGM(bgmno);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbRestoreBgm);

void nbBgmFadeOut(void)
{
    p2sdStopBGM(bgmno);
    p2sdStopBGM(nbBgmFadeState);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBgmStop);

void nb_28DE10(void)
{
    func_002C1E70(0x7F);
}

void nb_28DE28(void)
{
    func_002C1E70(0);
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_55);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2_5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEncProcess1);

void nbEncShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *battle = nbGetMainProcessData();

    battle->encounterProcess = NULL;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitEncProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCollectBadCurseUnits);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMarkBadCurseUnits);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseApplyMove);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSkillIsInRange);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindBadCurseTarget);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep4);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep5);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep6);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbBadCurseStep7);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetBadCurseStep);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetBadCurseTimer);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitBadCurseState);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545D40);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbApplyBadCurse);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", GetMakaProcessData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakaDispOn);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakaDispOff);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbPow10);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDrawMakaNum);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakaStep);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakaProcess1);

void nbMakaShutdown(dds3ProcessID_t *id)
{
    void *work = dds3GetProcessData(id);

    if (work != NULL) {
        sdfChipFreeDirect(work);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAllocMakaWork);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitMakaProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEventEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeEventEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckEventEntries);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbStartEventPacks);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbUpdateEventProcessData);

u32 func_0028FA30(dds3Basic_t *basic)
{
    u32 type = 0;

    if (basic != NULL) {
        type = basic->type;
    }
    return type;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFreeEffectSlot);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", func_0028FA68);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetEventForm);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCreateBasicEffect);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ResetWaitMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetWaitMotion);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetDeadEventFadeTime);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetDeadEventFadeOutFrame);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetDeadEventFadeInFrame);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbGetDeadEventFadeStopFrame);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEventProcess3);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEventProcess1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbEventShutdown);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", nbDigitStringTable);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00545DB0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbInitEventProcess);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbFindStockSkillError);

s32 cmpAnyButtonGuideActive(datUnitWork_t **units, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if ((units[i]->badstatus & 0xFFF) == 0x800) {
            return 1;
        }
    }
    return 0;
}

s32 cmpButtonGuideShowDone(datUnitWork_t **units, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (units[i]->hp < units[i]->maxhp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpButtonGuideHideDone(datUnitWork_t **units, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (units[i]->mp < units[i]->maxmp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpFindButtonGuideByFlag(datUnitWork_t **units, s32 count, u32 flags)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if ((units[i]->badstatus & 0xFFF) & flags) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", CheckHojo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDispSkillErrorMessage);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckBattleSkillError3);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546138);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", cmpClearButtonGuide);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadDefaultModelPack);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckEventPackLoaded);

void nbFreeEventPack(nbMainProcessData_t *battle)
{
    void *pack = battle->eventPack;

    if (pack != NULL) {
        pacFileHandleFree_(pack);
        battle->eventPack = NULL;
    }
    if (battle->eventMemory != NULL) {
        sdfMemFreeDirect(battle->eventMemory);
        battle->eventMemory = NULL;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbSetModelHandleOnce);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbMakeModelPackName);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadModelPackIfEnabled);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbDecodeModelPack);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbLoadMotionData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCopyPackData);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbAddFormEntry);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbCheckModelPackLoaded);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", RunUgou);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Wait1_nbE901);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetUp1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SyuGo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SyuGo2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Cls1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Daisupe);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", ReadWait2);

s32 Wait2_nbE901(void)
{
    nbFreeEffectSlot();
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Syutugen2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE901Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Pos1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SetForm);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE902_Wait1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", SelectCopy);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Bunshin1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Wait2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Hontai);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE902_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE902Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Hatudo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE903_Wait1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Parts1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsEffect1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsWait);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", PartsEffect2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", EffDel);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE903_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE903Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE904_End);

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

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model1Wait_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE905_Model2Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", func_002930E8);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE905_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE905Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE906_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE906_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE906Process_1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005467E0);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_005467F0);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model1Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model1Wait);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model1Wait_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_Model2Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model2Wait_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model2Wait_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE907Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE908_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE908_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE908Process_1);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546820);

INCLUDE_RODATA(const s32, "newbattle/nbMakePacket", D_00546830);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model1Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model1Wait);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model1Wait_2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Model2Start);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model2Wait_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_Model2Wait_15);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE909Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE910_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE910_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE910Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Effect1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Wait1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Effect2);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE911_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE911Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Hatudo);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Wait1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", MoveSub);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", Move1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_Effect1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE912_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nb_295D98);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE913_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE913_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE913Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE914_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE914_End);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE914Process_1);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE915_Test);

INCLUDE_ASM(const s32, "newbattle/nbMakePacket", nbE915_End);

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

