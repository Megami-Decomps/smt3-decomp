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

extern s32 printf(const char *format, ...);

extern void nbPanelAllHide(void);

extern u32 f;

extern u16 D_00575520[];

extern char D_005754E0[];

extern char D_005467C0[];

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, void *buff, s32 leng);

extern void func_0024D938(void *e);

extern u32 dds3DrawFlag;

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

extern s32 evtBitCheck(s32 no);

extern void *sdfMemLock(void *aHandle);

extern void evtSetUnitMotion(evtUnitHandle_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern char D_005467E0[];

extern s32 nbMaeCheckDone(void);

extern void nbMaeWaitStep(void);

extern s32 sprintf(char *buf, const char *format, ...);

extern s32 nbMakeModelPackName(s32 no);

extern char D_00546548[];

extern char D_00546570[];

extern void *evtGetResorceData(s32 eventno, s32 rid);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 pacCheckEnd(nbPacHandle_t *pacHandle);

extern void *fileGetMemHandle(void *fileHandle);

extern void fileHandleFree(void *fileHandle);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern u32 sdfMemGetSize(void *aHandle);

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

nbKoukaPacket_t *nbAddFadeOutKoukaPacket(s32 startframe, s32 uniqueid, s32 length, s32 coltype);

nbKoukaPacket_t *nbAddSkillEffectKoukaPacket3(s32 startframe, s32 uniqueid, u8 *bed, s32 cutno, nbFormation_t *form);

void nbBgmFadeOut(void);

nbFormation_t *nbGetEventForm();

extern s32 (*D_005274A0[])(nbEventProcessData_t *e);

extern void dds3DeleteSceneBasicData(void *basic);

extern evtUnitHandle_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

extern void nbCameraHelperModelSet(dds3Basic_t *hModel);

extern void nbCameraSetState(s32 state);

extern s32 evtInitMotionSeProcess(s32 eventId, s32 resourceId);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void effBTLSkillEnd(void);

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

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);

extern void nbUpdateEventProcessData(dds3ProcessID_t *id);

extern dds3Basic_t *D_005515D4;

extern dds3Basic_t *D_005515D8;

extern dds3Basic_t *D_005515DC;

s32 nbE904Process_1(dds3ProcessID_t *id)
{
    effBTL_SkillLight_t light;
    effBTLModelColorParam_t color;
    nbEventProcessData_t *e;
    evtUnitHandle_t *unit;
    nbFormation_t *form;
    s32 serial;

    e = dds3GetProcessData(id);
    if (D_005274A0[e->seq](e) != 0) {
        D_005515D4 = NULL;
        if (D_005515D8 != NULL) {
            dds3DeleteSceneBasicData(D_005515D8);
            D_005515D8 = NULL;
        }
        if (D_005515DC != NULL) {
            dds3DeleteSceneBasicData(D_005515DC);
            D_005515DC = NULL;
        }
        effBTLSkillEnd();
        return -1;
    }
    if (e->time > 0 && D_005515D4 != NULL) {
        unit = dds3GetUnitHandleObjectBasic(D_005515D4);
        if (unit->basic != NULL) {
            nbCameraHelperModelSet(unit->basic);
        }
    }
    switch (e->time) {
    case 0:
        nbCameraSetState(0x17);
        D_005515D4 = NULL;
        D_005515D8 = NULL;
        D_005515DC = NULL;
        serial = evtInitMotionSeProcess(e->eventrid, 3);
        D_005515D8 = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515D8), 0, 0, 0, 2);
        D_005515D4 = D_005515D8;
        break;
    case 0x14:
        serial = evtInitMotionSeProcess(e->eventrid, 4);
        D_005515DC = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
        evtSetUnitMotion(dds3GetUnitHandleObjectBasic(D_005515DC), 0, 0, 0, 2);
        D_005515D4 = D_005515DC;
        break;
    }
    switch (e->time) {
    case 0:
        effBTLSkillStart(0x201, 0);
        break;
    case 1:
        form = &e->data->form[5];
        light.type = 0;
        light.unknown01 = 2;
        light.unknown02 = 0;
        light.unknown04 = 0;
        light.diffuse = 0x80FFE6FF;
        light.ambient = 0x801E1E1E;
        light.color_anim = 0x19;
        light.unknown14 = 1;
        light.unknown18 = 0xB4;
        light.unknown1C = 1;
        color.unknown00 = 0x803C3C3C;
        color.unknown04 = 0x78;
        color.unknown08 = 3;
        color.unknown0C = 0xC8;
        color.unknown10 = 0;
        effBTLPointLightSet(&light, form);
        effBTLModelColorSet(&color, form);
        break;
    }
    switch (e->time) {
    case 50:
        dds3PadVibrationSet(0, 1, 4);
        dds3PadVibrationSet(1, 0xC8, 3);
        break;
    case 70:
        dds3PadVibrationSet(0, 1, 4);
        dds3PadVibrationSet(1, 0xC8, 3);
        break;
    case 100:
        dds3PadVibrationSet(0, 1, 4);
        dds3PadVibrationSet(1, 0xC8, 3);
        break;
    case 122:
        dds3PadVibrationSet(0, 1, 4);
        dds3PadVibrationSet(1, 0xC8, 3);
        break;
    case 0:
    case 23:
    case 43:
    case 53:
    case 63:
    case 73:
    case 93:
    case 103:
    case 113:
    case 125:
        dds3PadVibrationSet(0, 0, 0x14);
        dds3PadVibrationSet(1, 0x6E, 0x14);
        break;
    case 20:
    case 40:
    case 60:
    case 90:
    case 110:
    case 140:
        dds3PadVibrationSet(0, 1, 4);
        dds3PadVibrationSet(1, 0xFF, 3);
        break;
    case 143:
        dds3PadVibrationSet(0, 0, 0x14);
        effPadVibeFadeOUT(1, 0x6E, 0x32);
        break;
    }
    nbUpdateEventProcessData(id);
    return 0;
}

s32 Model1Start(nbEventProcessData_t *ed)
{
    nbFormation_t *form;
    s32 i;

    nbPanelAllHide();
    form = nbGetEventForm();
    evtSetUnitMotion(form->hunit, 0x10, 0xA, 0, 2);
    for (i = 4; i < 9; i++) {
        D_00575520[i] = ed->data->party[form->partyindex].count[i];
    }
    nbAddSkillEffectKoukaPacket3(0, 0, evtGetResorceData(0x389, 1), 2, form);
    if (evtBitCheck(0xC0F)) {
        ed->pfile = (nbPacHandle_t *)nbMakeModelPackName(0x102);
    } else {
        sprintf(D_005754E0, D_005467C0, 0x102);
        ed->file = fileLoad(D_005754E0);
    }
    nbFreeEffectSlot(ed);
    return 0;
}

s32 Model1Wait(nbEventProcessData_t *ed)
{
    switch (ed->seqtime) {
    case 1:
        nbBgmFadeOut();
        break;
    case 0xAC:
        dds3SetQuake(0x1E, 0x1F4);
        break;
    case 0xB9:
        dds3SetQuake(0x1E, 0x1F4);
        break;
    }
    if (ed->seqtime >= 0xF0 && pacCheckEnd(ed->pfile) != 0) {
        if (evtBitCheck(0xC0F) == 0) {
            ed->mem = fileGetMemHandle(ed->file);
            fileHandleFree(ed->file);
            ed->file = NULL;
        }
        nbAddFadeOutKoukaPacket(0, 0, 0xF, 1);
        nbFreeEffectSlot(ed);
    }
    return 0;
}

s32 Model1Wait_2(nbEventProcessData_t *ed)
{
    void *buf;
    s32 size;

    if (ed->seqtime == 0x1E) {
        func_0024D938(ed);
    }
    if (ed->seqtime >= 0x20) {
        if (evtBitCheck(0xC0F) == 0) {
            buf = sdfMemLock(ed->mem);
            size = sdfMemGetSize(ed->mem);
            mdlPackDecode(1, 0x102, 0x101, buf, size);
        }
        nbAddFadeInKoukaPacket(3, 0, 0x28, 1);
        nbFreeEffectSlot(ed);
    }
    return 0;
}

extern s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 id, s32 formindex);

extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);

extern void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect);

extern void nbSetModelHandleOnce(nbFormation_t *form, NB_FILEHANDLE_T *packh);

extern struct nbKoukaPacket_t *nbAddFormOnOffPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                    nbFormation_t *form, s32 sw);

extern struct nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, nbActionProcessData_t *uniqueid,
                                                     nbFormation_t *form, s32 a, s32 frame);

extern void nbSetEnemyFormation(nbMainProcessData_t *data);

extern void nbSetMaeFlag(s32 flag);

s32 nbE905_Model2Start(nbEventProcessData_t *ed)
{
    nbFormation_t *p;
    s32 i;

    p = ed->data->form + 4;
    nbAddUnitStock(ed->data, 1, nbAppendEnemyDevilWork(ed->data, 0x102, p->formindex),
                   p->formindex);
    nbInitUnitEffect(p, 0, 0);
    if (evtBitCheck(0xC0F) != 0) {
        nbSetModelHandleOnce(p, ed->pfile);
        ed->pfile = NULL;
    }
    for (i = 4; i < 9; i++) {
        ed->data->party[p->partyindex].count[i] = D_00575520[i];
    }
    nbAddFormOnOffPacket(0, 0, p, 1);
    nbAddUnitAKoukaPacket(0, 0, p, 0, 0);
    nbAddUnitAKoukaPacket(0, 0, p, 0x80000000, 0);
    dds3DrawFlag |= 0x2000000;
    nbSetEnemyFormation(ed->data);
    ed->data->encno = 0x3E0;
    nbSetMaeFlag(0);
    nbFreeEffectSlot(ed);
    return 0;
}

s32 func_002930E8(nbEventProcessData_t *e)
{
    dds3DrawFlag |= 0x2000000;
    nbFreeEffectSlot(e);
    return 0;
}

s32 nbE905_End(nbEventProcessData_t *e)
{
    if (e->seqtime >= 0x24) {
        if (nbMaeCheckDone() != 0) {
            printf(D_005467E0);
            nbMaeWaitStep();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbE904", D_005467C0);

INCLUDE_RODATA(const s32, "newbattle/nbE904", D_005467E0);

INCLUDE_SDATA(const s32, "newbattle/nbE904", D_005515D4);

INCLUDE_SDATA(const s32, "newbattle/nbE904", D_005515D8);

INCLUDE_SDATA(const s32, "newbattle/nbE904", D_005515DC);

