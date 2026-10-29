#include "common.h"
#include "newbattle.h"
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





/* A loaded pac file; only the chain down to the decoded buffer is named. */




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

extern s32 printf(const char *format, ...);

extern void nbPanelAllHide(void);

extern u32 f;



extern void nbFreeEffectSlot(nbEventProcessData_t *e);

extern void evtSetUnitMotion(evtUnitHandle_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbCheckLoadGroup(s32 side);

extern void nbDelAllKoukaPacketType(s32 type);

extern void MoveSub(nbEventProcessData_t *ed);

extern char D_00546898[];

extern char D_005468B0[];

extern void *evtGetResorceData(s32 eventno, s32 rid);

extern void nbSingleFormExceptedDispOff(nbFormation_t *form);

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

nbKoukaPacket_t *nbAddFadeOutKoukaPacket(s32 startframe, s32 uniqueid, s32 length, s32 coltype);

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket3(s32 startframe, s32 uniqueid, u8 *bed, s32 cutno, nbFormation_t *form);

extern s8 D_00527530[];

extern sdfVector4_t D_00527540[4][4];

extern sdfVector4_t D_00527640;

extern sdfVector4_t D_00527650;

extern s32 evtUnitRotationTarget(evtUnitHandle_t *h, f32 *pos, f32 radio);

extern void evtSetUnitMove1(evtUnitHandle_t *work, s32 motionSubmode, s128 *vector, s32 unused,
                            s32 frames, s32 directionOffset, void *callback, s32 unusedLast);

extern nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);

extern nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                             s32 sw);

extern nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                              u32 a, s32 frame);

extern u32 effBTLCutGetFrame(effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLCutSetDrawEndFrame(effBTLCutHandle_t *hBTLCutEffect, u32 frame);

extern nbKoukaPacket_t *D_00527660[];

extern nbKoukaPacket_t *D_00527664[];

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 (*D_00527670[])(nbEventProcessData_t *e);

extern void dds3DeleteSceneBasicData(void *basic);

extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

extern void nbCameraHelperModelSet(dds3Basic_t *hModel);

extern void nbCameraSetState(s32 state);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void nbAllFormDispOn(void);

extern void nbGroupFormDispOff(s32 group);

extern void nbCameraBoss_BSALBI_E912_Set(void);

extern void nbCameraBoss_BSALBI_E912_Run(void);

extern void nbUpdateEventProcessData(dds3ProcessID_t *id);

extern dds3Basic_t *D_00551630;

extern dds3Basic_t *D_00551634;

extern dds3Basic_t *D_00551638;


void MoveSub(nbEventProcessData_t *ed)
{
    sdfVector4_t target;
    s32 list[4];
    nbFormation_t *form;
    evtUnitHandle_t *h;
    s32 cnt;
    s32 i;
    s32 no;
    s32 frames;
    f32 len;
    f32 len2;

    cnt = 0;
    for (i = 0; i < 4; i++) {
        no = D_00527530[i];
        if (ed->data->form[no].stat & 1) {
            list[cnt] = no;
            cnt++;
        }
    }
    for (i = 0; i < cnt; i++) {
        form = &ed->data->form[list[i]];
        h = form->hunit;
        SDF_VU0_LOADVECTOR(&D_00527540[cnt - 1][i]);
        SDF_VU0_LOADVECTOR_B(&D_00527640);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_LOADVECTOR_B(&D_00527650);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&target);
        SDF_VU0_LOADVECTOR_B(&form->orgpos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len);
        frames = len / 6.0f;
        SDF_VU0_LOADVECTOR(&target);
        SDF_VU0_LOADVECTOR_B(&form->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len2);
        if (len2 <= 20.0f) {
            evtUnitRotationTarget(h, (f32 *)&D_00527640, 0.1f);
        } else {
            if (len2 <= 50.0f) {
                frames = frames * len2 / 50.0f;
            }
            evtSetUnitMove1(h, 0, (s128 *)&target, -1, frames, 9, NULL, 0);
        }
    }
}

s32 Move1(nbEventProcessData_t *ed)
{
    MoveSub(ed);
    if (ed->seqtime == 0x55 && nbCheckLoadGroup(1) == 0) {
        nbFreeEffectSlot(ed);
    }
    return 0;
}

s32 nbE912_Effect1(nbEventProcessData_t *e)
{
    nbFormation_t *form;
    s32 i;
    s32 no;

    form = &e->data->form[6];
    switch (e->seqtime) {
    case 1:
        for (i = 0; i < 4; i++) {
            no = D_00527530[i];
            if (e->data->form[no].stat & 1) {
                nbAddMotionKoukaPacket(0, 0, &e->data->form[no], 0x10);
            }
        }
        nbAddFormOnOffPacket(1, 0, form, 1);
        nbAddUnitAKoukaPacket(0, 0, form, 0, 0);
        break;
    case 2:
        D_00527660[0] = nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x390, 1), 2, form);
        break;
    case 0x27:
        D_00527664[0] = nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x390, 2), 2, form);
        break;
    case 0x36:
        nbAddUnitAKoukaPacket(1, 0, form, 0x80000000, 0);
        evtSetUnitMotion(form->hunit, 0x11, 0, 0, 0);
        break;
    case 0x6D:
        effBTLCutSetDrawEndFrame(D_00527660[0]->cuteff,
                                 effBTLCutGetFrame(D_00527660[0]->cuteff) + 0x20);
        effBTLCutSetDrawEndFrame(D_00527660[1]->cuteff,
                                 effBTLCutGetFrame(D_00527660[1]->cuteff) + 0x20);
        nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x390, 3), 2, form);
        nbFreeEffectSlot(e);
        break;
    }
    return 0;
}

s32 nbE912_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x10) {
        printf(D_00546898);
        e->data->stat &= ~8;
        return 1;
    }
    return 0;
}

s32 nb_295D98(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;
    evtUnitHandle_t *unit;
    s32 serial;

    e = dds3GetProcessData(id);
    if (D_00527670[e->seq](e) != 0) {
        D_00551630 = NULL;
        if (D_00551634 != NULL) {
            dds3DeleteSceneBasicData(D_00551634);
            D_00551634 = NULL;
        }
        if (D_00551638 != NULL) {
            dds3DeleteSceneBasicData(D_00551638);
            D_00551638 = NULL;
        }
        return -1;
    }
    if (e->time > 0 && D_00551630 != NULL) {
        unit = dds3GetUnitHandleObjectBasic(D_00551630);
        if (unit->basic != NULL) {
            nbCameraHelperModelSet(unit->basic);
        }
    }
    switch (e->time) {
    case 0:
        nbCameraSetState(0x17);
        nbAllFormDispOn();
        nbGroupFormDispOff(0);
        D_00551630 = NULL;
        D_00551634 = NULL;
        D_00551638 = NULL;
        nbCameraBoss_BSALBI_E912_Set();
        break;
    case 0x3C:
        nbCameraSetState(0x17);
        serial = evtInitMotionSeProcess(e->eventrid, 4);
        D_00551634 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_00551634), 0, 0, 0, 2);
        D_00551630 = D_00551634;
        break;
    case 0xD2:
        serial = evtInitMotionSeProcess(e->eventrid, 5);
        D_00551638 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_00551638), 0, 0, 0, 2);
        D_00551630 = D_00551638;
        break;
    default:
        if (e->time < 0x3C) {
            nbCameraBoss_BSALBI_E912_Run();
        }
        break;
    }
    nbUpdateEventProcessData(id);
    return 0;
}

s32 nbE913_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    form = nbGetEventForm();
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
    if (e->seqtime >= 0xD3) {
        printf(D_005468B0);
        nbDelAllKoukaPacketType(0x2B);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE912", D_00546898);

INCLUDE_RODATA(const s32, "newbattle/nbE912", D_005468B0);

INCLUDE_SDATA(const s32, "newbattle/nbE912", D_00551630);

INCLUDE_SDATA(const s32, "newbattle/nbE912", D_00551634);

INCLUDE_SDATA(const s32, "newbattle/nbE912", D_00551638);

