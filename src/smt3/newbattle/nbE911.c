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

extern s32 printf(const char *format, ...);

extern char D_00546880[];

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

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 (*D_00527520[])(nbEventProcessData_t *e);

extern void dds3DeleteSceneBasicData(void *basic);

extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

extern void nbCameraHelperModelSet(dds3Basic_t *hModel);

extern void nbCameraSetState(s32 state);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void evtSetUnitMotion(evtUnitHandle_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern void effBTLSkillEnd(void);

extern void effBTLSkillStart(s32 skillid, s32 arg1);

extern void effBTLPointLightSet(effBTL_SkillLight_t *pLight, nbFormation_t *pFormation);

extern void effBTLModelColorSet(effBTLModelColorParam_t *pModel, nbFormation_t *pFormation);

extern void dds3PadVibrationSet(s32 port, s32 motor, s32 level);

extern void effPadVibeFadeOUT(s32 mode, s32 power, s32 frame);

extern void nbUpdateEventProcessData(dds3ProcessID_t *id);

extern dds3Basic_t *D_00551620;

extern dds3Basic_t *D_00551624;

extern dds3Basic_t *D_00551628;

extern dds3Basic_t *D_0055162C;

extern void *memset(void *a, s32 b, u32 c);

extern void nbPanelAllHide(void);

extern char *text1[];

extern s8 D_00527530[];

extern sdfVector4_t D_00527640;

extern struct nbKoukaPacket_t *nbAddMotionKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                               s32 motion);

extern void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot);

extern void nbSetFormPosRot(nbFormation_t *form, sdfVector4_t *pos, sdfVector4_t *rot);

extern void nbDispText(char *text, char *text2, s32 type, s32 time, u32 color);

extern s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 id, s32 formindex);

extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);

extern void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect);

extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

/* The two light basics this fade reaches; neither is written anywhere in the
 * image, so only the chain down to the faded packet word is named. */
typedef struct { u8 unk_00[0x2C]; u32 color; } nbE911Prim_t;
typedef struct { u8 unk_00[0x28]; nbE911Prim_t *prim; } nbE911Work_t;
typedef struct { u8 unk_00[0x18]; nbE911Work_t *work; } nbE911Light_t;

extern nbE911Light_t *D_00551618;
extern nbE911Light_t *D_0055161C;

extern s32 nbCheckLoad(nbFormation_t *form);
extern void nbFormDispOn(s32 formindex);
extern void nbAddUnitAKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                  s32 a, s32 b);
extern void nbMakeUnitFadeInPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                   s32 a, s32 b, s32 c, s32 d);
extern void nbAddHitEffKoukaPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                   s32 a);
extern void dds3SetQuake(s32 power, s32 time);

#ifdef NON_MATCHING
/* NON_MATCHING: 94 of 114 words match at the exact 456-byte size. */
s32 Effect2(nbEventProcessData_t *e)
{
    nbFormation_t *forms = e->data->form;
    nbFormation_t *form = NULL;
    s32 t = e->seqtime;

    switch (t) {
    case 0:
        break;
    case 0x4B:
        form = &forms[4];
        if (nbCheckLoad(form) != 0) {
            e->seqtime--;
            form = NULL;
        }
        break;
    case 0x53:
        form = &forms[6];
        if (nbCheckLoad(form) != 0) {
            e->seqtime--;
            form = NULL;
        }
        break;
    case 0x8C:
        nbFreeEffectSlot(e);
        break;
    }
    if (form != NULL) {
        nbFormDispOn(form->formindex);
        nbAddUnitAKoukaPacket(0, 0, form, 0, 0);
        nbMakeUnitFadeInPacket(1, 0, form, 0, 0xF, 0xF, 0x14);
        nbAddHitEffKoukaPacket(1, 0, form, 4);
        dds3SetQuake(0xA, 0x258);
    }
    t = e->seqtime;
    if (t >= 0x3D && t <= 0x48) {
        s32 div = 12;
        s32 alpha = ((0x48 - t) << 7) / div;

        if (D_00551618 != NULL) {
            nbE911Prim_t *prim = D_00551618->work->prim;

            prim->color = (alpha << 24) | (prim->color & 0xFFFFFF);
            t = e->seqtime;
        }
    }
    if (t >= 0x45 && t <= 0x50) {
        s32 div = 12;
        s32 alpha = ((0x50 - t) << 7) / div;

        if (D_0055161C != NULL) {
            nbE911Prim_t *prim = D_0055161C->work->prim;

            prim->color = (alpha << 24) | (prim->color & 0xFFFFFF);
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbE911", Effect2);
#endif


s32 nbE911_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 2) {
        printf(D_00546880);
        return 1;
    }
    return 0;
}

s32 nbE911Process_1(dds3ProcessID_t *id)
{
    effBTL_SkillLight_t light;
    effBTLModelColorParam_t color;
    nbEventProcessData_t *e;
    evtUnitHandle_t *unit;
    nbFormation_t *form;
    s32 serial;

    e = dds3GetProcessData(id);
    if (D_00527520[e->seq](e) != 0) {
        D_00551620 = NULL;
        if (D_00551624 != NULL) {
            dds3DeleteSceneBasicData(D_00551624);
            D_00551624 = NULL;
        }
        if (D_00551628 != NULL) {
            dds3DeleteSceneBasicData(D_00551628);
            D_00551628 = NULL;
        }
        if (D_0055162C != NULL) {
            dds3DeleteSceneBasicData(D_0055162C);
            D_0055162C = NULL;
        }
        effBTLSkillEnd();
        return -1;
    }
    if (e->time > 0 && D_00551620 != NULL) {
        unit = dds3GetUnitHandleObjectBasic(D_00551620);
        if (unit->basic != NULL) {
            nbCameraHelperModelSet(unit->basic);
        }
    }
    switch (e->time) {
    case 0:
        nbCameraSetState(0x17);
        D_00551620 = NULL;
        D_00551624 = NULL;
        D_00551628 = NULL;
        D_0055162C = NULL;
        serial = evtInitMotionSeProcess(e->eventrid, 2);
        D_00551624 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_00551624), 0, 0, 0, 2);
        D_00551620 = D_00551624;
        break;
    case 0x28:
        serial = evtInitMotionSeProcess(e->eventrid, 3);
        D_00551628 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_00551628), 0, 0, 0, 2);
        D_00551620 = D_00551628;
        break;
    case 0x4B:
        serial = evtInitMotionSeProcess(e->eventrid, 4);
        D_0055162C = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_0055162C), 0, 0, 0, 2);
        D_00551620 = D_0055162C;
        break;
    }
    switch (e->time) {
    case 0:
        effBTLSkillStart(0xE6, 0);
        break;
    case 1:
        form = &e->data->form[5];
        light.type = 0;
        light.unknown01 = 2;
        light.unknown02 = 5;
        light.unknown04 = 0x1F4;
        light.diffuse = 0x80646464;
        light.ambient = 0x80462828;
        light.color_anim = 0;
        light.unknown14 = 2;
        light.unknown18 = 0x50;
        light.unknown1C = 0;
        color.unknown00 = 0x8094828A;
        color.unknown04 = 1;
        color.unknown08 = 0;
        color.unknown0C = 0x3C;
        color.unknown10 = 2;
        effBTLPointLightSet(&light, form);
        effBTLModelColorSet(&color, form);
        break;
    }
    switch (e->time) {
    case 0:
        dds3PadVibrationSet(0, 0, 0x28);
        dds3PadVibrationSet(1, 0x82, 0x28);
        break;
    case 0x28:
        effPadVibeFadeOUT(0, 1, 0xF);
        effPadVibeFadeOUT(1, 0xC8, 0xF);
        break;
    case 0x37:
        dds3PadVibrationSet(0, 0, 0x12);
        dds3PadVibrationSet(1, 0x82, 0x12);
        break;
    case 0x4C:
        dds3PadVibrationSet(0, 1, 5);
        dds3PadVibrationSet(1, 0xFF, 5);
        break;
    case 0x52:
        dds3PadVibrationSet(0, 1, 6);
        dds3PadVibrationSet(1, 0xFF, 6);
        break;
    case 0x58:
        effPadVibeFadeOUT(0, 1, 0x1E);
        effPadVibeFadeOUT(1, 0xFF, 0x1E);
        break;
    }
    nbUpdateEventProcessData(id);
    return 0;
}

s32 nbE912_Hatudo(nbEventProcessData_t *e)
{
    sdfVector4_t rot;
    nbFormation_t *form;
    s32 stockno;
    s32 i;

    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    nbPanelAllHide();
    for (i = 0; i < 4; i++) {
        if (e->data->form[D_00527530[i]].stat & 1) {
            nbAddMotionKoukaPacket(0, 0, &e->data->form[D_00527530[i]], 4);
            form = &e->data->form[D_00527530[i]];
            nbSetFormRot(form, &form->orgrot);
        }
    }
    nbDispText(text1[0xFB], NULL, 0, 0x5A, 0x89FEFF80);
    form = &e->data->form[6];
    nbSetFormPosRot(form, &D_00527640, &rot);
    stockno = nbAppendEnemyDevilWork(e->data, 0x116, form->formindex);
    nbAddUnitStock(e->data, 1, stockno, form->formindex);
    nbInitUnitEffect(form, 0, 0);
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE912_Wait1(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x3D) {
        nbFreeEffectSlot(e);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE911", D_00546880);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_00551618);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_0055161C);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_00551620);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_00551624);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_00551628);

INCLUDE_SDATA(const s32, "newbattle/nbE911", D_0055162C);

