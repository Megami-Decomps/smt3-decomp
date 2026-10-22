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

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern void p2sdPlayBGM(s32 MidiNo);

extern void p2sdStopBGM(s32 no);

extern char D_00551560[];

/* 28-byte entries; only the fields this unit reads are named. */
typedef struct {
    s32 rid;
    s32 unk_04;
    s32 unk_08;
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

typedef struct {
    s32 unk_00;
    s32 maka;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    u8 unk_14[0x8];
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
} nbMakaWork_t;

extern void nbFreeEventEntry(nbMainProcessData_t *data);

extern s32 evtGetEventPackStat(s32 eventno);

extern s32 nbMakaProcess1(dds3ProcessID_t *id);

extern void nbMakaShutdown(dds3ProcessID_t *id);

extern nbMakaWork_t *nbAllocMakaWork(void);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern void p2sdLoadSMGFile(s32 MidiNo);

extern void nbMakaStep(nbMakaWork_t *w);

extern void nbDrawMakaNum(nbMakaWork_t *w, s32 x, s32 y, u32 col, s32 n);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void nbMakaDispOn(s32 type);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern dds3ProcessID_t *evtLoadEventPack(s32 eventno);

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

nbMakaWork_t *GetMakaProcessData()
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(D_00551560);
    if (id == NULL) {
        return NULL;
    }
    return dds3GetProcessData(id);
}

void nbMakaDispOn(s32 type)
{
    nbMakaWork_t *w;

    w = GetMakaProcessData();
    if (type == 0) {
        w->unk_0C = 0x112;
        w->unk_10 = 0x10D;
    }
    w->maka = DDS3_GBWK->maka;
    w->unk_1C = type;
    w->unk_00 = 1;
}

void nbMakaDispOff(void)
{
    nbMakaWork_t *w;

    w = GetMakaProcessData();
    if (w->unk_08 != 0) {
        if (w->unk_1C == 0) {
            w->unk_24 = 0x14;
        }
        w->unk_00 = 2;
    }
}

u32 nbPow10(u32 v, s32 n)
{
    s32 p;

    p = 10;
    if (n > 0) {
        do {
            p = p * 10;
            n--;
        } while (n != 0);
    }
    return (v % p) / (p / 10);
}

INCLUDE_ASM(const s32, "newbattle/nbMakaProcess", nbDrawMakaNum);

void nbMakaStep(nbMakaWork_t *w)
{
    switch (w->unk_00) {
    case 1:
        w->unk_08 = 1;
        if (w->unk_28 == 0x80) {
            w->unk_00 = 0;
            return;
        }
        w->unk_28 += 0x20;
        if (w->unk_28 > 0x80) {
            w->unk_28 = 0x80;
        }
        return;
    case 2:
        if (w->unk_24 > 0) {
            w->unk_24--;
            return;
        }
        if (w->unk_28 > 0) {
            w->unk_28 -= 0x14;
            if (w->unk_28 < 0) {
                w->unk_28 = 0;
            }
            return;
        }
        w->unk_08 = 0;
        w->unk_00 = 0;
        return;
    }
}

s32 nbMakaProcess1(dds3ProcessID_t *id)
{
    nbMakaWork_t *w;

    w = dds3GetProcessData(id);
    nbMakaStep(w);
    if (w->unk_08 != 0) {
        nbDrawMakaNum(w, w->unk_0C, w->unk_10, 0x80808080, w->unk_28);
    }
    return 0;
}

void nbMakaShutdown(dds3ProcessID_t *id)
{
    void *w;

    w = dds3GetProcessData(id);
    if (w != NULL) {
        sdfChipFreeDirect(w);
    }
}

nbMakaWork_t *nbAllocMakaWork(void)
{
    nbMakaWork_t *w;

    w = sdfChipAllocDirect(sizeof(nbMakaWork_t));
    memset(w, 0, sizeof(nbMakaWork_t));
    w->maka = DDS3_GBWK->maka;
    return w;
}

dds3ProcessID_t *nbInitMakaProcess(dds3ProcessID_t *parent)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00551560, 0x2B0E, 1, 1, nbMakaProcess1, nbMakaShutdown,
                          nbAllocMakaWork());
    if (parent != NULL) {
        dds3LinkProcess(parent, id);
    }
    return id;
}

void nbGetEventEntry(nbMainProcessData_t *data)
{
    s32 bgm;
    s32 v;

    bgm = nbEventEntryTable[data->eventno].unk_0C;
    v = nbEventEntryTable[data->eventno].unk_10;
    if (bgm != 0) {
        p2sdPlayBGM(bgm + v);
    }
}

void nbFreeEventEntry(nbMainProcessData_t *data)
{
    s32 v;
    s32 bgm;

    v = nbEventEntryTable[data->eventno].unk_10;
    bgm = nbEventEntryTable[data->eventno].unk_0C;
    if (v != 0 && bgm != 0) {
        p2sdStopBGM(bgm + v);
        bgm = 0;
    }
}

s32 nbCheckEventEntries(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (nbEventEntryTable[i].unk_08 != 0 && nbEventEntryTable[i].unk_08 == data->encno) {
            if (nbEventEntryTable[i].rid != 0 &&
                evtGetEventPackStat(nbEventEntryTable[i].rid) != 2) {
                return 0;
            }
            if (nbEventEntryTable[i].unk_0C != 0 &&
                p2sdLoadCheckMidi(nbEventEntryTable[i].unk_0C) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void nbStartEventPacks(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (nbEventEntryTable[i].unk_08 != 0 && nbEventEntryTable[i].unk_08 == data->encno) {
            if (nbEventEntryTable[i].rid != 0) {
                dds3LinkProcess(data->mainproc,
                                evtLoadEventPack(nbEventEntryTable[i].rid));
            }
            if (nbEventEntryTable[i].unk_0C != 0) {
                p2sdLoadSMGFile(nbEventEntryTable[i].unk_0C);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "newbattle/nbMakaProcess", nbDigitStringTable);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551560);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551568);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551570);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551578);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551580);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551588);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551590);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_00551598);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_005515A0);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_005515A8);

INCLUDE_SDATA(const s32, "newbattle/nbMakaProcess", D_005515B0);

