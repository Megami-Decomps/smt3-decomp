#include "common.h"
#include "newbattle.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"
#include "model.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    mdlObj_t *owner;
    u8 unk_90[0x14];
    u32 flags;
} evtUnitHandle_t;

typedef struct effBTLSysHandle_t effBTLSysHandle_t;

typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;





/* A loaded pac file; only the chain down to the decoded buffer is named. */


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
    dds3ProcessID_t *MainProcessID;
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
    s32 massuruid;
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

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern u32 f;

extern dds3Basic_t *D_005515B4;

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, sdfVector4_t *pos);

extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);

extern void func_0024D938(void *e);

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

extern void nbFreeEffectSlot(nbEventProcessData_t *e);

extern nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);

extern void effBTLSkillEnd(void);

extern s32 evtBitCheck(s32 no);

extern void nbLoadUnitGroup(s32 side, s32 stop, s32 anime);

extern void evtSetUnitMotion(evtUnitHandle_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbCheckLoadGroup(s32 side);

extern void nbResetAllFormPosRot(void);

extern void dds3DeleteSceneBasicData(void *basic);

extern void *bedbasic;

extern u8 pNbCamera[];

extern void nbCameraBoss_Sp_DOUTYO_Run(u8 *pNbCamera);

extern void nbCameraBoss_Sp_DOUTYO_Set(u8 *pNbCamera);

extern void nbCameraBoss_Sp_DOUTYO_TPFix_Set(u8 *pNbCamera);

extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic);

extern s32 sdfModelGetMarkPos(struct sdfModelRootNode_t *aRoot, s32 aId);

extern s32 evtUnitRotationTarget(evtUnitHandle_t *h, f32 *pos, f32 radio);

extern nbKoukaPacket_t *nbAddHitEffKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 type);

extern void dds3SetQuake(s32 frame, s32 range);

extern void *evtGetResorceData(s32 eventno, s32 rid);

extern s32 D_005515BC;

extern void nbSetMainPhase(s32 encno);

extern void nbSetupBattleUnits(nbMainProcessData_t *data, s32 encno);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern void effPadVibeFadeOUT(s32 mode, s32 power, s32 frame);

extern void dds3StartBEDEffectObjectBasic(void *basic, u32 cutno);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 time);

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

dds3Basic_t *nbCreateBasicEffect(void *p, void *pos, void *rot);

nbFormation_t *nbGetEventForm();

void nbUpdateEventProcessData(dds3ProcessID_t *id);

void nbSetModelHandleOnce(nbFormation_t *form, NB_FILEHANDLE_T *packh);

extern s32 nbCameraBoss_Sp_DOUTYO_TP_Set(u8 *pNbCamera);
extern s32 D_005515C0;
extern char *text1[];
extern void nbPanelAllShow(void);
extern s32 dds3GetRandIntA(s32 minimum, s32 range);
extern s32 mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

s32 RunUgou(nbEventProcessData_t *e)
{
    s32 list[16];
    s32 count = 0;
    nbFormation_t *lead;
    s32 *p;
    s32 tmp;
    s32 i;

    nbPanelAllShow();
    nbResetAllFormPosRot();
    p = list;
    for (i = 4; i < 14; i++) {
        if (e->data->form[i].stat & 1) {
            *p++ = i;
            count++;
        }
    }
    lead = &e->data->form[list[count / 2]];
    if (count < 4) {
        D_005515BC = 0x112;
        D_005515C0 = 0x128;
    } else if (count < 6) {
        D_005515BC = 0x111;
        D_005515C0 = 0x127;
    } else {
        D_005515BC = 0x110;
        D_005515C0 = 0x126;
    }
    tmp = list[0];
    list[0] = list[count / 2];
    list[count / 2] = tmp;
    nbAddMotionKoukaPacket(0, 0, lead, 0x10);
    for (i = 1; i < count; i++) {
        s32 wait = dds3GetRandIntA(0, 0xF);

        nbAddMotionKoukaPacket(wait + 0xF, 0, &e->data->form[list[i]], 0x10);
    }
    effBTLSkillStart(0xE8, 0);
    nbCameraBoss_Sp_DOUTYO_TP_Set(pNbCamera);
    nbDispText(text1[0xE8], NULL, 0, 0x5A, 0x89FEFF80);
    if (evtBitCheck(0xC0F) != 0) {
        e->pfile = (nbPacHandle_t *)nbMakeModelPackName(D_005515C0);
    } else {
        mdlPrepare(1, D_005515C0, 0);
    }
    nbFreeEffectSlot(e);
    return 0;
}


s32 Wait1_nbE901(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x42) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

s32 SetUp1(nbEventProcessData_t *ed)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    void *h;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    pos.y = -600.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    h = evtInitMotionSeProcess(ed->eventrid, 1);
    basic = dds3SearchSceneBasic(dds3GetActiveScene(), h, 5);
    D_005515B4 = basic;
    pos.x = 0.0f;
    pos.y = 85.0f;
    pos.z = 0.0f;
    dds3SetPosPoseSet(basic, &pos);
    evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515B4), 0, 0, 0, 2);
    bedbasic = nbCreateBasicEffect(evtGetResorceData(0x385, 2), &pos, &rot);
    nbFreeEffectSlot(ed);
    return 0;
}

s32 SyuGo(nbEventProcessData_t *e)
{
    sdfVector4_t v;
    sdfVector4_t vec1;
    sdfVector4_t vec2;
    evtUnitHandle_t *h;
    nbFormation_t *form;
    s32 i;
    s32 ofs;
    s32 mark;

    memset(&vec1, 0, sizeof(vec1));
    vec1.z = -100.0f;
    memset(&vec2, 0, sizeof(vec2));
    vec2.z = -5000.0f;
    if (e->seqtime == 2) {
        nbCameraBoss_Sp_DOUTYO_TPFix_Set(pNbCamera);
    }
    if (e->seqtime == 5) {
        e->data->press4_p = 0;
        e->data->pressturn = 0;
    }
    if (e->seqtime >= 0x38) {
        nbFreeEffectSlot(e);
    }
    h = dds3GetUnitHandleObjectBasic(D_005515B4);
    i = 0;
    mark = 5;
    ofs = 0;
    for (; i < 6; i++) {
        form = (nbFormation_t *)((u8 *)e->data->form + ofs + 0x3C0);
        if (form->stat & 1) {
            sdfModelGetMarkPos(h->owner->inner, mark - i);
            SDF_VU0_STOREVECTOR(&v);
            nbSetFormPos(form, &v);
            if (e->seqtime < 0x33) {
                evtUnitRotationTarget(form->hunit, (f32 *)&vec1, 0.08f);
            } else {
                evtUnitRotationTarget(form->hunit, (f32 *)&vec2, 0.05f);
            }
        }
        ofs += 0xF0;
    }
    return 0;
}

s32 SyuGo2(nbEventProcessData_t *ed)
{
    s32 i;
    s32 ofs;
    s32 mark;
    nbFormation_t *form;

    if (ed->seqtime >= 0xB) {
        if (ed->seqtime == 0xB) {
            ofs = 0x3C0;
            for (i = 9; i >= 0; i--) {
                form = (nbFormation_t *)((u8 *)ofs + (u32)ed->data->form);
                ofs += 0xF0;
                if (form->stat & 1) {
                    nbAddMotionKoukaPacket(0, 0, form, 4);
                }
            }
        }
        if (ed->seqtime == 0x19) {
            dds3StartBEDEffectObjectBasic(bedbasic, 2);
        }
        if (ed->seqtime >= 0x31) {
            nbFreeEffectSlot(ed);
        }
    }
    return 0;
}

s32 Cls1(nbEventProcessData_t *ed)
{
    func_0024D938(ed);
    nbFreeEffectSlot(ed);
    return 0;
}

s32 Daisupe(nbEventProcessData_t *ed)
{
    nbFormation_t *form;
    sdfVector4_t pos;

    nbSetMainPhase(D_005515BC);
    nbSetupBattleUnits(ed->data, D_005515BC);
    nbLoadUnitGroup(1, 0, 1);
    if (evtBitCheck(0xC0F) != 0) {
        nbSetModelHandleOnce(nbGetEventForm(), ed->pfile);
        ed->pfile = NULL;
    }
    form = nbGetEventForm();
    SDF_VU0_COPYVECTOR(&pos, &form->pos);
    pos.y -= 1000.0f;
    nbSetFormPos(form, &pos);
    nbFreeEffectSlot(ed);
    return 0;
}

s32 ReadWait2(nbEventProcessData_t *ed)
{
    if (ed->seqtime >= 0x1F && nbCheckLoadGroup(1) == 0) {
        nbFreeEffectSlot(ed);
    }
    return 0;
}

s32 Wait2_nbE901(nbEventProcessData_t *e)
{
    nbFreeEffectSlot(e);
    return 0;
}

s32 Syutugen2(nbEventProcessData_t *ed)
{
    sdfVector4_t pos;
    sdfVector4_t pos2;
    nbFormation_t *form;
    sdfVector4_t *op;

    if (ed->seqtime == 3) {
        nbAddHitEffKoukaPacket(0, 0, nbGetEventForm(), 4);
    }
    if (ed->seqtime == 5) {
        dds3SetQuake(0x1E, 0x258);
    }
    form = nbGetEventForm();
    SDF_VU0_COPYVECTOR(&pos, &form->pos);
    pos.y = pos.y + 150.0f;
    if (pos.y > 0.0f) {
        pos.y = 0.0f;
        nbFreeEffectSlot(ed);
    }
    nbSetFormPos(form, &pos);
    if (ed->seqtime >= 4) {
        if (ed->seqtime == 4) {
            SDF_VU0_LOADVECTOR(&form->pos);
            SDF_VU0_STOREVECTOR(&pos);
            op = &pos2;
            SDF_VU0_STOREVECTOR_RWADDR(op);
            pos.y = 0.0f;
            nbSetFormPos(form, &pos);
            nbCameraBoss_Sp_DOUTYO_Set(pNbCamera);
            nbSetFormPos(form, &pos2);
        } else {
            nbCameraBoss_Sp_DOUTYO_Run(pNbCamera);
        }
    }
    return 0;
}

s32 End(nbEventProcessData_t *ed)
{
    nbCameraBoss_Sp_DOUTYO_Run(pNbCamera);
    if (ed->seqtime < 0x6F) {
        return 0;
    }
    effBTLSkillEnd();
    nbResetAllFormPosRot();
    dds3DeleteSceneBasicData(bedbasic);
    ed->data->stat |= 8;
    ed->data->press4_p = 0;
    ed->data->pressturn = 0;
    return 1;
}

s32 nbE901Process_1(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (D_00527420[e->seq](e) != 0) {
        return -1;
    }
    switch (e->time) {
    case 0xD4:
        dds3PadVibrationSet(0, 1, 0xA);
        dds3PadVibrationSet(1, 0xE1, 0xA);
        break;
    case 0xDE:
        effPadVibeFadeOUT(0, 1, 0x2D);
        effPadVibeFadeOUT(1, 0x8C, 0x2D);
        break;
    }
    nbUpdateEventProcessData(id);
    return 0;
}
INCLUDE_SDATA(const s32, "newbattle/nbE901", D_005515B4);

INCLUDE_SDATA(const s32, "newbattle/nbE901", bedbasic);

INCLUDE_SDATA(const s32, "newbattle/nbE901", D_005515BC);

INCLUDE_SDATA(const s32, "newbattle/nbE901", D_005515C0);

