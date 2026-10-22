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
    s32 selectbit;
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

extern s32 printf(const char *format, ...);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 nbGetSelectBit(nbMainProcessData_t *data);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern char D_00545CE0[];

typedef struct { s32 step; s32 weight; } nbBadCurseEntry_t;

extern nbBadCurseEntry_t nbBadCurseTable[];

extern s32 nbBadCurseTimer;

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 unk_08;
    s32 step;
    s32 frame;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *unk_1C;
    nbPacHandle_t *unk_20;
    void *unk_24;
} nbEventProcessData_t;

extern void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 var_id, s32 var_ind);

extern void nbLoadUnitGroup(s32 side, s32 stop, s32 anime);

extern s32 nbFindBadCurseTarget(nbActionProcessData_t *a, s32 step);

extern void nbSetTargetSkill(nbActionProcessData_t *a, datUnitWork_t *work, s32 *ctype,
                             s32 *crule, s32 *carea, s32 mode);

extern s32 nbSetSelectOkMark(s32 partyindex, s32 ctype, s32 crule, s32 carea);

extern s32 nbBadCurseApplyMove(nbActionProcessData_t *a, s32 mode);

extern void *nbEncProcess2_5(dds3ProcessID_t *id);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void *nbEncProcess1(dds3ProcessID_t *id);

extern void effBTLEncountStart(void);

extern s32 dds3GetKernelCount(void);

extern s32 readbasetime;

extern s32 readtime;

extern void nbEncShutdown(dds3ProcessID_t *id);

extern char D_00551550[];

extern s32 nbCheckLoadGroup(s32 side);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern void *nbEncProcess3(dds3ProcessID_t *id);

extern char D_00545CF8[];

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

s32 nbCheckBadCurse(s32 formindex);

s32 nbCheckFormationPlayerSide(s32 formindex);

void *nbEncProcess3(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;
    u32 i;

    data = nbGetMainProcessData();
    form = data->form;
    for (i = 0; i < 0x20; i++) {
        if (form->stat & 1) {
            if (!(form->stat & 2)) {
                return NULL;
            }
            if (form->hunit == NULL) {
                return NULL;
            }
            if (form->hunit->flags & 0x10000) {
                return NULL;
            }
        }
        form++;
    }
    if (nbCameraStart() == 0) {
        return NULL;
    }
    effBTLEncountEnd();
    nbGetMainProcessData()->stat &= ~0x800;
    return (void *)-1;
}

void *nbEncProcess2_6(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbParty_t *party;
    nbFormation_t *form;
    s32 mask;
    s32 i;

    data = nbGetMainProcessData();
    if (nbCheckLoadGroup(0) != 0) {
        return NULL;
    }
    if (readtime == 0) {
        readtime = dds3GetKernelCount() - readbasetime;
    }
    party = data->party;
    mask = ~8;
    for (i = 3; i >= 0; i--) {
        if (party->flag & 9) {
            form = &data->form[party->formindex];
            if (form->flag & 8) {
                form->flag &= mask;
                form->anime |= 0x200;
            }
        }
        party++;
    }
    printf(D_00545CF8, readtime);
    return nbEncProcess3;
}

INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbEncProcess2_55);

INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbEncProcess2_5);

void *nbEncProcess2(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (nbCheckLoadGroup(1) != 0) {
        return NULL;
    }
    if ((data->stat & 0x10) != 0) {
        nbLoadUnitGroup(0, 0, 1);
    } else {
        nbLoadUnitGroup(0, 1, 0);
    }
    return nbEncProcess2_5;
}

void *nbEncProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    effBTLEncountStart();
    readbasetime = dds3GetKernelCount();
    readtime = 0;
    if ((data->stat & 0x800) != 0) {
        nbLoadUnitGroup(1, 1, 0);
    } else {
        nbLoadUnitGroup(1, 1, 1);
    }
    return nbEncProcess2;
}

void nbEncShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->encproc = NULL;
}

void nbInitEncProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00551550, 0x3F9, 1, 1, nbEncProcess1, nbEncShutdown, NULL);
    dds3LinkProcess(data->mainproc, id);
    data->encproc = id;
}

/* NON_MATCHING: 11 of 41 words differ (first at +0x28). */
#ifdef NON_MATCHING
void nbCollectBadCurseUnits(nbActionProcessData_t *a)
{
    s8 list[32];
    nbFormation_t *form;
    s32 i;
    s32 cnt;
    s32 ofs;

    cnt = 0;
    for (i = 0, ofs = 0; i < 0x10; i++) {
        form = (nbFormation_t *)(ofs + (s32)a->data->form);
        ofs += sizeof(nbFormation_t);
        if (form->stat & 4) {
            list[cnt++] = i;
        }
    }
    if (cnt > 0) {
        a->data->form[list[dds3GetRandIntA(0, cnt)]].stat |= 8;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbCollectBadCurseUnits);
#endif

void nbMarkBadCurseUnits(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        if (form->stat & 4) {
            form->stat |= 8;
        }
        form++;
    }
}

s32 nbBadCurseApplyMove(nbActionProcessData_t *a, s32 mode)
{
    s32 ctype;
    s32 crule;
    s32 carea;

    ctype = 0;
    crule = 0;
    carea = 0;
    nbSetTargetSkill(a, a->work, &ctype, &crule, &carea, mode);
    if (nbSetSelectOkMark(a->partyindex, ctype, crule, carea) == 0) {
        return 0;
    }
    switch (ctype) {
    case 0:
        nbCollectBadCurseUnits(a);
        break;
    case 1:
    case 2:
        nbMarkBadCurseUnits(a);
        break;
    }
    a->selectbit = nbGetSelectBit(a->data);
    return 1;
}

s32 nbSkillIsInRange(s32 skill, s32 type)
{
    switch (type) {
    case 2:
        if (skill >= 0x2A) {
            goto zero;
        }
        if (skill < 0x24) {
            goto zero;
        }
        return 1;
    case 3:
        if (skill >= 0x45) {
            goto zero;
        }
        if (skill >= 0x40) {
            return 1;
        }
        goto zero;
    }
zero:
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbFindBadCurseTarget);

s32 nbBadCurseStep3(nbActionProcessData_t *a)
{
    s32 target;

    target = nbFindBadCurseTarget(a, 3);
    if (target < 0) {
        return 1;
    }
    a->work->nowcommand = 1;
    a->work->nowindex = target;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep2(nbActionProcessData_t *a)
{
    s32 target;

    target = nbFindBadCurseTarget(a, 2);
    if (target < 0) {
        return 1;
    }
    a->work->nowcommand = 1;
    a->work->nowindex = target;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep1(nbActionProcessData_t *a)
{
    a->work->nowcommand = 0;
    a->work->nowindex = 0;
    if (nbBadCurseApplyMove(a, 1) == 0) {
        return 1;
    }
    if (nbCheckBadCurse(a->form->formindex) != 0) {
        nbSetJyokyoMes(a, 0x4A, 1, a->work->id);
    } else {
        switch (a->work->badstatus & 0xFFF) {
        case 8:
            nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
            break;
        case 0x80:
            nbSetJyokyoMes(a, 0x36, 1, a->work->id);
            break;
        }
    }
    a->unk_04 |= 0x200;
    return 0;
}

s32 nbBadCurseStep4(nbActionProcessData_t *a)
{
    if (nbCheckFormationPlayerSide(a->form->formindex) != 0 &&
        DDS3_GBWK->maka == 0) {
        return 1;
    }
    if ((a->work->flag & 0x20) != 0 && (a->party->flag & 0x800) != 0) {
        return 1;
    }
    a->party->flag |= 0x800;
    a->work->nowcommand = 1;
    a->work->nowindex = 0xF7;
    if (nbBadCurseApplyMove(a, 0) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep5(nbActionProcessData_t *a)
{
    if (nbCheckFormationPlayerSide(a->form->formindex) == 0) {
        return 1;
    }
    a->work->nowcommand = 4;
    a->work->nowindex = 0x180;
    a->unk_68 = 6;
    if (nbBadCurseApplyMove(a, 0) == 0) {
        return 1;
    }
    switch (a->work->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, a->work->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, a->work->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep6(nbActionProcessData_t *a)
{
    datUnitWork_t *w;

    w = a->work;
    if (w->flag & 4) {
        return 1;
    }
    if (w->id >= 0x100) {
        return 1;
    }
    w->nowcommand = 7;
    w->nowindex = 0;
    switch (w->badstatus & 0xFFF) {
    case 8:
        nbSetJyokyoMes(a, 0x2E, 1, w->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x36, 1, w->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep0(nbActionProcessData_t *a)
{
    datUnitWork_t *w;
    s32 bad;

    w = a->work;
    bad = w->badstatus & 0xFFF;
    w->nowcommand = 8;
    w->nowindex = 0;
    switch (bad) {
    case 8:
        nbSetJyokyoMes(a, 0x30, 1, w->id);
        break;
    case 0x80:
        nbSetJyokyoMes(a, 0x38, 1, w->id);
        break;
    }
    return 0;
}

s32 nbBadCurseStep7(nbActionProcessData_t *a)
{
    datUnitWork_t *w;

    w = a->work;
    w->nowcommand = 10;
    w->nowindex = 0;
    nbSetJyokyoMes(a, 0x4C, 1, w->id);
    return 0;
}

s32 nbGetBadCurseStep(void)
{
    s32 i;
    s32 total;
    s32 sum;
    s32 r;

    total = 0;
    for (i = 0; i < nbBadCurseTimer; i++) {
        total += nbBadCurseTable[i].weight;
    }
    if (total == 0) {
        return -1;
    }
    r = dds3GetRandIntA(0, total);
    sum = 0;
    for (i = 0; i < nbBadCurseTimer; i++) {
        if (nbBadCurseTable[i].weight != 0) {
            sum += nbBadCurseTable[i].weight;
            if (r < sum) {
                return nbBadCurseTable[i].step;
            }
        }
    }
    return 9;
}

/* NON_MATCHING: 2 of 11 words differ (first at +0x14). */
#ifdef NON_MATCHING
void nbSetBadCurseTimer(s32 step, s32 weight)
{
    nbBadCurseTable[nbBadCurseTimer].step = step;
    nbBadCurseTable[nbBadCurseTimer].weight = weight;
    nbBadCurseTimer++;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbSetBadCurseTimer);
#endif

INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbInitBadCurseState);

INCLUDE_RODATA(const s32, "newbattle/nbEncProcess", D_00545D40);

INCLUDE_ASM(const s32, "newbattle/nbEncProcess", nbApplyBadCurse);
INCLUDE_SDATA(const s32, "newbattle/nbEncProcess", readbasetime);

INCLUDE_SDATA(const s32, "newbattle/nbEncProcess", readtime);

INCLUDE_SDATA(const s32, "newbattle/nbEncProcess", D_00551550);

INCLUDE_SDATA(const s32, "newbattle/nbEncProcess", nbBadCurseTimer);

INCLUDE_SDATA(const s32, "newbattle/nbEncProcess", D_0055155C);

