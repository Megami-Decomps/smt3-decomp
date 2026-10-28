#include "common.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"
#include "event.h"
#include "model.h"

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
    evtUnit_t *hunit;
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

typedef struct nbKoukaProcessData_t
{
    nbMainProcessData_t *data;
    s32 packetcnt;
    nbKoukaPacket_t *first;
    nbKoukaPacket_t *last;
} nbKoukaProcessData_t;

extern s32 printf(const char *format, ...);

extern void nbPanelAllHide(void);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern u32 f;

extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);

extern void nbResetFormPosRot(nbFormation_t *form);

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

extern void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern char D_00546500[];

extern char D_005464A8[];

extern char D_005464C0[];

extern char D_00546548[];

extern char D_00546570[];

extern void *evtGetResorceData(s32 eventno, s32 rid);

extern void nbSingleFormExceptedDispOff(nbFormation_t *form);

extern u32 effBTLCutGetFrame(effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLCutSetDrawEndFrame(effBTLCutHandle_t *hBTLCutEffect, u32 frame);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern sdfVector4_t D_00527490;

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

nbFormation_t *nbGetEventForm();

s32 Parts1(nbEventProcessData_t *ed)
{
    nbAddMotionKoukaPacket(0, 0, nbGetEventForm(ed), 0x10);
    nbFreeEffectSlot(ed);
    return 0;
}

extern const s32 D_00546518[4];
extern const s32 D_00546528[4];
extern void *memcpy(void *dst, const void *src, u32 size);
extern s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 id, s32 formindex);
extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);
extern void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect);
extern s32 nbCheckLoadGroup(s32 side);
extern nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, s32 sw);
extern nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                              u32 a, s32 frame);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);

s32 PartsEffect1(nbEventProcessData_t *ed)
{
    sdfVector4_t markPosition;
    s32 stockIds[4];
    s32 formIndices[4];
    nbFormation_t *form;
    nbFormation_t *groupForm;
    s32 seqtime;
    s32 mark;
    s32 i;
    s32 stockno;

    memcpy(stockIds, D_00546518, sizeof(stockIds));
    memcpy(formIndices, D_00546528, sizeof(formIndices));
    seqtime = ed->seqtime;
    form = NULL;
    mark = 0;
    if (seqtime == 1) {
        for (i = 0; i < 4; i++) {
            form = &ed->data->form[4 + formIndices[i]];
            stockno = nbAppendEnemyDevilWork(ed->data, stockIds[i], form->formindex);
            nbAddUnitStock(ed->data, 1, stockno, form->formindex);
            nbInitUnitEffect(form, 0, 0);
        }
        return 0;
    }
    switch (seqtime) {
    case 0x22:
        form = &ed->data->form[4];
        break;
    case 0x3D:
        form = &ed->data->form[5];
        mark = 1;
        break;
    case 0x58:
        form = &ed->data->form[7];
        mark = 2;
        break;
    case 0x6F:
        form = &ed->data->form[8];
        mark = 3;
        break;
    }
    if (seqtime >= 0x88 && nbCheckLoadGroup(1) == 0) {
        for (i = 0; i < 4; i++) {
            groupForm = &ed->data->form[4 + formIndices[i]];
            nbAddFormOnOffPacket(1, 0, groupForm, 1);
            nbAddUnitAKoukaPacket(0, 0, groupForm, 0, 0);
            nbAddUnitAKoukaPacket(1, 0, groupForm, 0x80000000, 0);
            evtSetUnitMotion(groupForm->hunit, 0x11, 0, 0, 0);
        }
        nbFreeEffectSlot(ed);
    }
    if (form != NULL) {
        sdfModelGetMarkPos(ed->data->form[6].hunit->owner->inner, mark + 0x1E);
        SDF_VU0_STOREVECTOR(&markPosition);
        markPosition.y = 0.0f;
        nbSetFormPos(form, &markPosition);
        nbAddSkillEffectKoukaPacket3(0, 1,
                                     evtGetResorceData(0x387, mark + 1), 2, form);
    }
    return 0;
}

s32 PartsWait(nbEventProcessData_t *ed)
{
    if (ed->seqtime >= 9) {
        nbFreeEffectSlot(ed);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE903", D_00546518);

INCLUDE_RODATA(const s32, "newbattle/nbE903", D_00546528);

s32 PartsEffect2(nbEventProcessData_t *ed)
{
    s32 list[4] = {0, 1, 3, 4};
    nbFormation_t *form;
    s32 i;

    for (i = 0; i < 4; i++) {
        form = &ed->data->form[list[i] + 4];
        nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x387, i + 5), 2, form);
    }
    nbFreeEffectSlot(ed);
    return 0;
}

s32 EffDel(nbEventProcessData_t *ed)
{
    nbKoukaProcessData_t *k;
    nbKoukaPacket_t *p;

    if (ed->seqtime == 0x4B) {
        k = dds3GetProcessData(ed->data->koukaproc);
        p = k->first;
        while (p != NULL) {
            if (p->uniqueid == 1 && p->type == 0x2B && p->cuteff != NULL) {
                effBTLCutSetDrawEndFrame(p->cuteff, effBTLCutGetFrame(p->cuteff) + 0xC);
            }
            p = p->next;
        }
        nbFreeEffectSlot(ed);
    }
    return 0;
}

s32 nbE903_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x1F) {
        printf(D_00546548);
        e->data->stat |= 0x2000;
        return 1;
    }
    return 0;
}

extern s32 (*D_00527470[])(nbEventProcessData_t *);
extern dds3Basic_t *D_005515C4;
extern dds3Basic_t *D_005515C8;
extern dds3Basic_t *D_005515CC;
extern dds3Basic_t *D_005515D0;

extern void nbCameraSetState(u16 state);
extern void nbAllFormDispOn(void);
extern void nbGroupFormDispOff(s32 side);
extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);
extern dds3Basic_t *dds3GetActiveScene(void);
extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 serialID, s32 type);
extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic);
extern void nbCameraHelperModelSet(mdlObj_t *model);
extern void nbUpdateEventProcessData(dds3ProcessID_t *id);

s32 nbE903Process_1(dds3ProcessID_t *id)
{
    nbEventProcessData_t *event = dds3GetProcessData(id);
    s32 time;
    s32 serial;
    dds3Basic_t *scene;

    if (D_00527470[event->seq](event) != 0) {
        D_005515C4 = NULL;
        if (D_005515C8 != NULL) {
            dds3DeleteSceneBasicData(D_005515C8);
            D_005515C8 = NULL;
        }
        if (D_005515CC != NULL) {
            dds3DeleteSceneBasicData(D_005515CC);
            D_005515CC = NULL;
        }
        if (D_005515D0 != NULL) {
            dds3DeleteSceneBasicData(D_005515D0);
            D_005515D0 = NULL;
        }
        return -1;
    }

    time = event->time;
    if (time > 0 && D_005515C4 != NULL) {
        evtUnit_t *unit = dds3GetUnitHandleObjectBasic(D_005515C4);
        if (unit->owner != NULL) {
            nbCameraHelperModelSet(unit->owner);
        }
        time = event->time;
    }
    switch (time) {
    case 0:
        nbCameraSetState(0x17);
        nbAllFormDispOn();
        nbGroupFormDispOff(0);
        D_005515C4 = NULL;
        D_005515C8 = NULL;
        D_005515CC = NULL;
        D_005515D0 = NULL;
        serial = evtInitMotionSeProcess(event->eventrid, 9);
        scene = dds3GetActiveScene();
        D_005515C8 = dds3SearchSceneBasic(scene, serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515C8), 0, 0, 0, 2);
        D_005515C4 = D_005515C8;
        break;
    case 0x32:
        serial = evtInitMotionSeProcess(event->eventrid, 10);
        scene = dds3GetActiveScene();
        D_005515CC = dds3SearchSceneBasic(scene, serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515CC), 0, 0, 0, 2);
        D_005515C4 = D_005515CC;
        break;
    case 0xBE:
        serial = evtInitMotionSeProcess(event->eventrid, 11);
        scene = dds3GetActiveScene();
        D_005515D0 = dds3SearchSceneBasic(scene, serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515D0), 0, 0, 0, 2);
        D_005515C4 = D_005515D0;
        break;
    }

    nbUpdateEventProcessData(id);
    return 0;
}

s32 Test(nbEventProcessData_t *ed)
{
    nbFormation_t *form;

    form = &ed->data->form[5];
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x388, 1), 2, form);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x388, 2), 2, form);
    nbAddFadeOutKoukaPacket(0x5A, 0, 0x46, 1);
    nbResetFormPosRot(form);
    nbSetFormPos(form, &D_00527490);
    nbFreeEffectSlot(ed);
    return 0;
}

s32 nbE904_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0xBF) {
        printf(D_00546570);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE903", D_00546548);

INCLUDE_RODATA(const s32, "newbattle/nbE903", D_00546570);

INCLUDE_SDATA(const s32, "newbattle/nbE903", D_005515C4);

INCLUDE_SDATA(const s32, "newbattle/nbE903", D_005515C8);

INCLUDE_SDATA(const s32, "newbattle/nbE903", D_005515CC);

INCLUDE_SDATA(const s32, "newbattle/nbE903", D_005515D0);

