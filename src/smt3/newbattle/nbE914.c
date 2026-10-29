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

extern s32 printf(const char *format, ...);

extern void nbPanelAllHide(void);

extern u32 f;

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

extern void evtSetUnitMotion(evtUnitHandle_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void nbDelAllKoukaPacketType(s32 type);

extern char D_005468D8[];

extern void *evtGetResorceData(s32 eventno, s32 rid);

extern void nbSingleFormExceptedDispOff(nbFormation_t *form);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern void nbBgmStop(void);

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

extern s32 (*D_00527690[])(nbEventProcessData_t *e);

extern void dds3DeleteSceneBasicData(void *basic);

extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

extern void nbCameraHelperModelSet(dds3Basic_t *hModel);

extern nbFormation_t *nbGetEventForm(void);

extern void nbCameraSetState(s32 state);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void nbSetEnemyFormation(nbMainProcessData_t *data);

extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

extern void dds3CrossfadeStart(s32 frames);

extern void nbCameraBoss_BSMETA_E914_1_Set(void);

extern void effBTLSkillStart(s32 skillid, s32 arg1);

typedef struct
{
    u8 type;
    u8 unknown01;
    u8 unknown02;
    u8 unknown03;
    u32 unknown04;
    u32 diffuse;
    u32 ambient;
    u32 color_anim;
    u32 unknown14;
    u32 unknown18;
    u32 unknown1C;
    u32 unknown20;
    u8 reserve[0xC];
} effBTL_SkillLight_t;

typedef struct
{
    u32 unknown00;
    u32 unknown04;
    u32 unknown08;
    u32 unknown0C;
    u32 unknown10;
    u32 unknown14;
    u32 unknown18;
} effBTLModelColorParam_t;

extern void effBTLPointLightSet(effBTL_SkillLight_t *pLight, nbFormation_t *pFormation);

extern void effBTLModelColorSet(effBTLModelColorParam_t *pModel, nbFormation_t *pFormation);

extern void nbUpdateEventProcessData(dds3ProcessID_t *id);

extern dds3Basic_t *D_0055163C;

extern dds3Basic_t *D_00551640;

s32 nbE914Process_1(dds3ProcessID_t *id)
{
    u8 reserve[0x10];
    effBTL_SkillLight_t light;
    effBTLModelColorParam_t color;
    nbEventProcessData_t *e;
    evtUnitHandle_t *unit;
    nbFormation_t *form;
    s32 serial;

    e = dds3GetProcessData(id);
    if (D_00527690[e->seq](e) != 0) {
        D_0055163C = NULL;
        if (D_00551640 != NULL) {
            dds3DeleteSceneBasicData(D_00551640);
            D_00551640 = NULL;
        }
        return -1;
    }
    if (e->time > 0 && D_0055163C != NULL) {
        unit = dds3GetUnitHandleObjectBasic(D_0055163C);
        if (unit->basic != NULL) {
            nbCameraHelperModelSet(unit->basic);
        }
    }
    switch (e->time) {
    case 0:
        nbSingleFormExceptedDispOff(nbGetEventForm());
        nbCameraSetState(0x17);
        serial = evtInitMotionSeProcess(e->eventrid, 2);
        D_00551640 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_00551640), 0, 0, 0, 2);
        D_0055163C = D_00551640;
        nbSetEnemyFormation(e->data);
        nbSetPlayerFormation(e->data, 0);
        break;
    case 0x6E:
        dds3CrossfadeStart(0xC);
        nbCameraBoss_BSMETA_E914_1_Set();
        D_0055163C = NULL;
        break;
    }
    switch (e->time) {
    case 0:
        effBTLSkillStart(0x202, 0);
        break;
    case 1:
        form = nbGetEventForm();
        light.type = 2;
        light.unknown01 = 1;
        light.unknown02 = 0;
        light.unknown04 = 0;
        light.diffuse = 0x80E6FFFF;
        light.ambient = 0x80321E28;
        light.color_anim = 0;
        light.unknown14 = 0;
        light.unknown18 = 0x2710;
        light.unknown1C = 0;
        color.unknown00 = 0x80829696;
        color.unknown04 = 0;
        color.unknown08 = 0;
        color.unknown0C = 0x2710;
        color.unknown10 = 0;
        effBTLPointLightSet(&light, form);
        effBTLModelColorSet(&color, form);
        break;
    }
    nbUpdateEventProcessData(id);
    return 0;
}

s32 nbE915_Test(nbEventProcessData_t *e)
{
    nbFormation_t *form;

    nbBgmStop();
    form = nbGetEventForm();
    nbPanelAllHide();
    nbSingleFormExceptedDispOff(form);
    evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x393, 1), 2, form);
    nbAddFadeOutKoukaPacket(0x14A, 0, 0x28, 1);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE915_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x173) {
        printf(D_005468D8);
        nbDelAllKoukaPacketType(0x2B);
        e->data->endtype = 1;
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE914", D_005468D8);

INCLUDE_SDATA(const s32, "newbattle/nbE914", D_0055163C);

INCLUDE_SDATA(const s32, "newbattle/nbE914", D_00551640);

