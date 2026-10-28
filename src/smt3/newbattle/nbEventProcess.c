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

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

/* 28-byte entries; only the fields this unit reads are named. */
typedef struct {
    s32 rid;
    s32 unk_04;
    s32 encno;
    s32 unk_0C;
    s32 unk_10;
    u16 fadetime;
    u16 fadeoutframe;
    u16 fadestopframe;
    u16 fadeinframe;
} nbEventEntry_t;

extern nbEventEntry_t nbEventEntryTable[];

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

extern s32 nbFindStockSkillError(void);

extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, u32 s);

extern void nbFreeEventEntry(nbMainProcessData_t *data);

extern void fileSyncAll(void);

extern char D_00545DC8[];

extern void *nbEventProcess1(dds3ProcessID_t *id);

extern void nbEventShutdown(dds3ProcessID_t *id);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void *nbEventProcess3(dds3ProcessID_t *id);

extern char D_00545DB0[];

extern void pacFileHandleFree_(void *pacHandle);

extern void sdfMemFreeDirect(void *aHandle);

extern void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void fileHandleFree(void *fileHandle);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern void nbSetUnitMotion(nbFormation_t *form, s32 motion2, s32 mode, f32 ritu);

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

void nbUpdateEventProcessData(dds3ProcessID_t *id)
{
    nbEventProcessData_t *e;

    e = dds3GetProcessData(id);
    if (e != NULL) {
        e->time++;
        e->seqtime++;
    }
}

s32 func_0028FA30(nbEventProcessData_t *e)
{
    if (e == NULL) {
        return 0;
    }
    return e->seq;
}

/* NON_MATCHING: exact on its own; its presence turns the bnel of three
   nbFreeEffectSlot callers (Wait1_nbE901, PartsWait, nbE912_Wait1) and of
   EffDel into a bne. */
void nbFreeEffectSlot(nbEventProcessData_t *e)
{
    if (e != NULL) {
        e->seq++;
        e->seqtime = 0;
    }
}

void func_0028FA68(nbEventProcessData_t *e, s32 step)
{
    if (e != NULL) {
        e->seq = step;
        e->seqtime = 0;
    }
}

nbFormation_t *nbGetEventForm(e)
nbEventProcessData_t *e;
{
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form + 4;
    for (i = 4; i < 14; i++) {
        if (form->stat & 1) {
            return form;
        }
        form++;
    }
    return NULL;
}

dds3Basic_t *nbCreateBasicEffect(void *p, void *pos, void *rot)
{
    dds3Basic_t *basic;

    basic = dds3AddBEDEffectObjectBasic2(p, pos, rot);
    dds3EffectObjectBasicStatOn(basic, 1);
    return basic;
}

void ResetWaitMotion(nbEventProcessData_t *ed)
{
    s32 i;

    for (i = 0; i < 14; i++) {
        if (ed->data->form[i].stat & 1) {
            if (ed->data->form[i].stat & 2) {
                nbSetWaitMotion(&ed->data->form[i], 0, 1.0f);
            }
        }
    }
}

void SetWaitMotion(nbEventProcessData_t *ed)
{
    s32 i;

    for (i = 0; i < 14; i++) {
        if (ed->data->form[i].stat & 1) {
            if (ed->data->form[i].stat & 2) {
                nbSetWaitMotion(&ed->data->form[i], 0, 1.0f);
                nbSetUnitMotion(&ed->data->form[i], 0, 0, 1.0f);
            }
        }
    }
}

s32 nbGetDeadEventFadeTime(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadetime;
}

s32 nbGetDeadEventFadeOutFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadeoutframe;
}

s32 nbGetDeadEventFadeInFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadeinframe;
}

s32 nbGetDeadEventFadeStopFrame(s32 eventno)
{
    return nbEventEntryTable[eventno % 100].fadestopframe;
}

INCLUDE_ASM(const s32, "newbattle/nbEventProcess", nbEventProcess3);

void *nbEventProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbEventProcessData_t *e;
    s32 rid;

    data = nbGetMainProcessData();
    e = dds3GetProcessData(id);
    printf(D_00545DB0, data->eventno);
    rid = nbEventEntryTable[data->eventno].rid;
    data->eventrid = rid;
    if (rid > 0) {
        e->eventrid = rid;
    }
    return nbEventProcess3;
}

void nbEventShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    nbEventProcessData_t *e;

    data = nbGetMainProcessData();
    nbFreeEventEntry(data);
    data->stat &= ~0x1000;
    data->eventno = 0;
    data->eventproc = NULL;
    e = dds3GetProcessData(id);
    SetWaitMotion(e);
    if (e->mem != NULL) {
        sdfMemFreeDirect(e->mem);
        e->mem = NULL;
    }
    if (e->file != NULL) {
        fileSyncAll();
        fileHandleFree(e->file);
        e->file = NULL;
    }
    if (e->pfile != NULL) {
        fileSyncAll();
        pacFileHandleFree_(e->pfile);
        e->pfile = NULL;
    }
    if (e->unk_18 != NULL) {
        e->unk_18(id);
    }
    if (e->userdata != NULL) {
        sdfChipFreeDirect(e->userdata);
    }
    sdfChipFreeDirect(e);
}

void nbInitEventProcess(nbMainProcessData_t *data)
{
    nbEventProcessData_t *e;
    dds3ProcessID_t *id;

    e = sdfChipAllocDirect(sizeof(nbEventProcessData_t));
    memset(e, 0, sizeof(nbEventProcessData_t));
    e->data = data;
    data->stat |= 0x1000;
    data->endtype = 0;
    id = dds3StartProcess(D_00545DC8, 0x3F9, 1, 1, nbEventProcess1, nbEventShutdown, e);
    dds3LinkProcess(data->mainproc, id);
    data->eventproc = id;
}

s32 nbFindStockSkillError(void)
{
    datUnitWork_t *w;
    fldAnahoriAkuma_t *a;
    s32 i;
    s32 ofs;

    /* akuma[i].flag starts a datUnitWork_t, so the table has two views. */
    w = (datUnitWork_t *)((u8 *)DDS3_GBWK + 4);
    a = (fldAnahoriAkuma_t *)DDS3_GBWK;
    for (i = 0; i < 16; i++) {
        ofs = 0xA50 + i * 0xEC;
        if ((((datUnitWork_t *)((u8 *)w + ofs))->flag & 1) != 0) {
            if ((((fldAnahoriAkuma_t *)((u8 *)a + ofs))->badstatus & 0xFFF) == 0x800) {
                return 1;
            }
        }
    }
    return 0;
}

s32 cmpAnyButtonGuideActive(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if ((list[i]->badstatus & 0xFFF) == 0x800) {
            return 1;
        }
    }
    return 0;
}

s32 cmpButtonGuideShowDone(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if (list[i]->hp < list[i]->maxhp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpButtonGuideHideDone(datUnitWork_t **list, s32 cnt)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if (list[i]->mp < list[i]->maxmp) {
            return 0;
        }
    }
    return 1;
}

s32 cmpFindButtonGuideByFlag(datUnitWork_t **list, s32 cnt, s32 mask)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        if ((list[i]->badstatus & 0xFFF) & mask) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbEventProcess", D_00545DB0);

INCLUDE_RODATA(const s32, "newbattle/nbEventProcess", D_00545DC8);

