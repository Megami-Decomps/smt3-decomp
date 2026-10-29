#include "common.h"
#include "newbattle.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"
#include "event.h"
#include "font.h"

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
    sdfMemHandle_t *form_hndl;
    datUnitWork_t *enemyunit;
    sdfMemHandle_t *enemyunit_hndl;
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

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern char D_00545728[];

extern void *nbDebugProcess1(dds3ProcessID_t *id);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern u8 dds3PadButtonState[];

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

extern void nbSetTargetProcess(s32 act, s32 partyindex, s32 ctype, s32 crule, s32 carea);

extern s32 menucur;

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

/* 0x30 bytes; only the fields this unit touches are named. */
typedef struct {
    nbMainProcessData_t *data;
    s32 seq;
    s32 nextseq;
    s32 side;
    s32 stockno;
    s32 formindex;
    nbFormation_t *form;
    s32 partyindex;
    nbParty_t *party;
    s32 hp;
    s32 mp;
    u32 bad;
} nbDebugProcessData_t;

extern void SetDvlChange(nbDebugProcessData_t *d);

extern void DispHpMpBad2(nbDebugProcessData_t *d, nbFormation_t *form);

extern s32 mdlGetNumOfCut(mdlObj_t *model, s32 group);
extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate);
extern s32 nbMemoryLoadNegoSe(nbFormation_t *form);
extern s32 nbPlayMotionSe(nbFormation_t *form);
extern s32 nbMemoryLoadMotionSe(nbFormation_t *form, s32 motion);
extern s32 nbGetUnitFromAction(nbFormation_t *form, s32 motion);
extern void nbDelAllUniqueIDKoukaPacket(s32 uniqueID);
struct nbKoukaPacket_t;
extern struct nbKoukaPacket_t *nbAddSePacketLock(s32 frame, s32 uniqueID, s32 sound);

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

void SetMotion(nbMainProcessData_t *data, nbFormation_t *form, s32 motion, s32 mode)
{
    s32 sound = 0;

    if (motion < mdlGetNumOfCut(form->hunit->owner, 0)) {
        nbSetUnitMotion(form, motion, mode, 1.0f);
        switch (motion) {
        case 14:
            if (nbMemoryLoadNegoSe(form) != 0) {
                sound = nbPlayMotionSe(form);
            }
            break;
        case 15:
            if (nbMemoryLoadNegoSe(form) != 0) {
                sound = nbPlayMotionSe(form) + 1;
            }
            break;
        default:
            if (nbMemoryLoadMotionSe(form, motion) != 0) {
                sound = nbGetUnitFromAction(form, motion);
            }
            break;
        }
        if (sound > 0) {
            data->stat &= ~0x200;
            nbDelAllUniqueIDKoukaPacket(0x3039);
            nbAddSePacketLock(1, 0x3039, sound);
            /* Retail keeps a normal call and shared epilogue. This dead local
             * reset survives cc1's sibling pass and is removed in the life pass
             * (verified with compiler output parity). */
            sound = 0;
        }
    }
}

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 color, s32 attr, const char *format, ...);
extern const char *nbBadStatusNames[12];
extern const char D_00551358[];
extern const char D_00551360[];
extern sdfGrOt_t D_002F67F8;


extern const char *D_004F06D8[];
typedef struct frFontRequest_t frFontRequest_t;
extern frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 priority, s32 color,
                                        const char *text, frFontRequest_t *request);
extern s32 frDrawFont(frFontHandle_t *request);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);
extern void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z);
extern s32 dds3SelectMenu(s32 pad, s32 listsx, s32 listsy, s32 winsx, s32 winsy,
                         s32 *basex, s32 *basey, s32 *x, s32 *y);

extern void DispBadStatus(s32 x, s32 y, u32 status, s32 color);

extern s32 D_005512EC;
extern s32 D_005512F0;
extern const char D_005512F8[];
extern char *text2[];

#ifdef NON_MATCHING
s32 nbDebugStockSelect(void)
{
    sdfDrawTag_t *tag;
    frFontRequest_t *request;
    fldAnahoriAkuma_t *unit;
    s32 i, y, color, attr, base, result, index;
    u32 flags;
    u16 name;

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    dds3ConsPanelTagLinkZ(tag, 0xA2, 0x3E, 16, 21, 0xFEFFFB);
    D_002F67F8.regFunc(&D_002F67F8, tag);
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    base = *(volatile s32 *)&D_005512F0;
    y = 70;
    for (i = 0; i < 12; i++) {
        unit = &DDS3_GBWK->akuma[base + i];
        if (D_005512EC == i) {
            attr = 4;
            color = 0xFF0080;
        } else {
            attr = 0;
            color = -0x80;
        }
        DispBadStatus(0xAA, y + 4, unit->badstatus, attr);
        flags = unit->flag;
        if (flags & 1) {
            name = 0xFF;
            if (!(flags & 4)) {
                name = unit->devilId;
            }
            request = frwGetSmallFontFRQ(0xFA * 16, y * 8, 0xFFFF80, color, text2[name], NULL);
            frDrawFont((frFontHandle_t *)request);
            frDestroyFontRequestAfter((frFontHandle_t *)request);
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7FA0, (y + 4) * 8 + 0x7900,
                                                  0xFFFF80, attr, D_005512F8, 0, 0xFF));
        }
        y += 20;
        base = *(volatile s32 *)&D_005512F0;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
    result = dds3SelectMenu(0, 1, 16, 1, 12, NULL, &D_005512F0, NULL, &D_005512EC);
    if (result == -1) {
        return -2;
    }
    if (result != 1) {
        return -1;
    }
    index = D_005512F0 + D_005512EC;
    return (DDS3_GBWK->akuma[index].flag & 1) ? index : -1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", nbDebugStockSelect);
#endif

void DispBadStatus(s32 x, s32 y, u32 status, s32 color)
{
    s32 i;
    u32 bit;
    sdfDrawTag_t *tag;
    const char **name;

    i = 0;
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    bit = 1;
    name = nbBadStatusNames;
    while (i < 12) {
        if ((status & (bit << i)) != 0) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(x * 16 + 0x7000, y * 8 + 0x7900, 0xFEFFFF, color,
                                                  D_00551358, *name));
            break;
        }
        name++;
        i++;
    }
    if (i == 12) {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(x * 16 + 0x7000, y * 8 + 0x7900, 0xFEFFFF, color, D_00551360));
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
}


typedef struct {
    const char *count[20];
    const char *aisyo[16];
} nbDebugUnitLabels_t;
extern nbDebugUnitLabels_t nbDebugUnitLabelTable;
extern const char D_00545658[], D_00545668[], D_00545678[], D_00545688[];
extern const char D_00551488[], D_00551490[];
extern nbParty_t *nbGetPartyFromFormindex(s32 formindex);
extern u32 nbGetAisyo(s32 nskill, s32 formindex, s32 attr);

#ifdef NON_MATCHING
void DispHpMpBad2(nbDebugProcessData_t *d, nbFormation_t *form)
{
    nbDebugUnitLabels_t labels;
    datUnitWork_t *work;
    nbParty_t *party;
    sdfDrawTag_t *tag;
    frFontRequest_t *request;
    char flags[6];
    s32 i, y, n;
    u32 aisyo;

    labels = nbDebugUnitLabelTable;
    work = nbGetUnitWorkFromFormindex(form->formindex);
    party = nbGetPartyFromFormindex(form->formindex);
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    request = frwGetSmallFontFRQ(0x80, 0x270, 0xFFFF80, -0x80, text2[work->id], NULL);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, 0x7C30, 0xFEFFFF, 0, D_00545658, work->id));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, 0x7C90, 0xFEFFFF, 0, D_00545668, work->level));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, 0x7CF0, 0xFEFFFF, d->seq == 5 ? 4 : 0,
                                         D_00545678, work->hp, work->maxhp));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, 0x7D50, 0xFEFFFF, d->seq == 6 ? 4 : 0,
                                         D_00545688, work->mp, work->maxmp));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, 0x7DB0, 0xFEFFFF, 0, D_00551488));
    DispBadStatus(0x50, 0x96, work->badstatus, 0);
    y = 0x7E70;
    for (i = 0; i < 20; i++) {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7080, y, 0xFEFFFF, 0,
                                             D_00551490, labels.count[i], party->count[i]));
        y += 0x60;
    }
    y = 0x80D0;
    for (i = 0; i < 12; i++) {
        aisyo = nbGetAisyo(-1, form->formindex, i);
        n = 0;
        if (aisyo & 0x10000) flags[n++] = 'M';
        if (aisyo & 0x20000) flags[n++] = 'H';
        if (aisyo & 0x40000) flags[n++] = 'K';
        if (aisyo & 0x100000) flags[n++] = 'B';
        if ((s32)aisyo < 0) flags[n++] = 'W';
        flags[n] = '\0';
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7820, y, 0xFEFFFF, 0,
                                             D_00551490, labels.aisyo[i], aisyo & 0xFFFF));
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7E20, y, 0xFEFFFF, 0, D_00551358, flags));
        y += 0x60;
    }
    frDrawFont((frFontHandle_t *)request);
    frDestroyFontRequestAfter((frFontHandle_t *)request);
    D_002F67F8.regFunc(&D_002F67F8, tag);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", DispHpMpBad2);
#endif

void DispHpMpBad(nbDebugProcessData_t *d)
{
    nbFormation_t *form;
    s32 i;

    form = NULL;
    for (i = 0; i < 0x20; i++) {
        if (d->data->form[i].anime & 8) {
            form = &d->data->form[i];
            break;
        }
    }
    if (form != NULL) {
        DispHpMpBad2(d, form);
    }
}

s32 SelectMenu(nbDebugProcessData_t *d)
{
    const char **label = D_004F06D8;
    s32 i;
    s32 x = 200;
    s32 y = 50 * 16;
    s32 result;
    sdfDrawTag_t *tag;

    for (i = 0; i < 5; i++, label++, y += 12 * 16) {
        u32 color = i == menucur ? 0xFF0080 : 0xFFFFFF80;
        frFontRequest_t *request = frwGetSmallFontFRQ(x * 16, y, 0xFF0001, color, *label, NULL);
        frDrawFont((frFontHandle_t *)request);
        frDestroyFontRequestAfter((frFontHandle_t *)request);
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    dds3ConsPanelTagLinkZ(tag, x - 8, 0x5C, 10, 12, 0xFF0000);
    D_002F67F8.regFunc(&D_002F67F8, tag);
    if ((s8)dds3PadButtonState[3] < 0) {
        return -1;
    }
    result = dds3SelectMenu(0, 1, 5, 1, 5, NULL, NULL, NULL, &menucur);
    if (result == -1 || result == 1) {
        return result;
    }
    return 0;
}

s32 SelectHpMp(nbDebugProcessData_t *d)
{
    datUnitWork_t *work;
    s32 delta;
    s32 v;

    work = nbGetUnitWorkFromFormindex(d->form->formindex);
    delta = 0;
    if (dds3PadButtonState[4] & 2) {
        delta = -100;
    }
    if (dds3PadButtonState[5] & 2) {
        delta = 100;
    }
    if (dds3PadButtonState[6] & 2) {
        delta = -10;
    }
    if (dds3PadButtonState[7] & 2) {
        delta = 10;
    }
    if ((s8)dds3PadButtonState[1] < 0) {
        return 1;
    }
    if ((s8)dds3PadButtonState[3] < 0) {
        return -1;
    }
    switch (d->seq) {
    case 5:
        v = work->hp;
        v += delta;
        if (v <= 0) {
            v = 1;
        }
        if (v > work->maxhp) {
            v = work->maxhp;
        }
        work->hp = v;
        break;
    case 6:
        v = work->mp;
        v += delta;
        if (v < 0) {
            v = 0;
        }
        if (v > work->maxmp) {
            v = work->maxmp;
        }
        work->mp = v;
        break;
    }
    return 0;
}

extern s32 badcur;
extern s32 nbMakeBadKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form);
extern s32 nbMakeBadDamagePacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 bad);

s32 SelectBad(nbDebugProcessData_t *d)
{
    s32 i;
    s32 y;
    sdfDrawTag_t *tag;
    datUnitWork_t *work;
    s32 color;
    s32 result;

    i = 0;
    y = 100;
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    work = nbGetUnitWorkFromFormindex(d->form->formindex);
    dds3ConsPanelTagLinkZ(tag, 0x160, 0x5C, 10, 15, 0xFEFFFE);
    for (; i < 13; i++) {
        color = i == badcur ? 4 : 0;
        if (i == 0) {
            DispBadStatus(0x168, y, 0, color);
        } else {
            DispBadStatus(0x168, y, 1u << (i - 1), color);
        }
        y += 12;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
    result = dds3SelectMenu(0, 1, 13, 1, 13, NULL, NULL, NULL, &badcur);
    if (result == -1) {
        return -1;
    }
    if (result != 1) {
        return 0;
    }
    if (badcur == 0) {
        nbMakeBadKaifukuPacket(0, 0, d->form);
    } else {
        work->badstatus &= 0xF000;
        nbMakeBadDamagePacket(0, 0, d->form, 1u << (badcur - 1));
    }
    return 1;
}

extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);
typedef struct { const char *name[4]; } nbDebugMotionNames_t;
extern nbDebugMotionNames_t D_00545698;
extern const char D_005514B8[];
extern const char D_005514C0[];
extern const char D_005514C8[];
extern const char D_005514D0[];
extern s32 D_005512E8;
extern s32 D_005512E0;
extern s32 slSelectMotIndex;

#ifdef NON_MATCHING
s32 SelectMotData(nbDebugProcessData_t *d, s32 active)
{
    nbDebugMotionNames_t typeNames;
    sdfDrawTag_t *tag;
    datUnitWork_t *work;
    nbUnitVisualMotions_t *visual;
    nbUnitMotion_t *motion;
    s32 i, y, attr, result, selected;
    s32 byteValue;
    s32 halfValue;
    u8 byteBits;
    u16 halfBits;

    typeNames = D_00545698;
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    work = nbGetUnitWorkFromFormindex(d->form->formindex);
    dds3ConsPanelTagLinkZ(tag, 0x2E, 0x16E, 12, 5, 0xFF0000);
    visual = (nbUnitVisualMotions_t *)nbGetUnitVisual(work);
    motion = &visual->motions[slSelectMotIndex + D_005512E0];
    y = 0x8490;
    for (i = 0; i < 4; i++) {
        selected = *(volatile s32 *)&D_005512E8;
        attr = active && i == selected ? 4 : 0;
        switch (i) {
        case 0:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7320, y, 0xFFFF80, attr,
                                                 D_005514B8, typeNames.name[motion->type]));
            break;
        case 1:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7320, y, 0xFFFF80, attr,
                                                 D_005514C0, motion->data));
            break;
        case 2:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7320, y, 0xFFFF80, attr,
                                                 D_005514C8, motion->bframe));
            break;
        case 3:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7320, y, 0xFFFF80, attr,
                                                 D_005514D0, motion->hframe));
            break;
        }
        y += 0x60;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
    if (!active) {
        return 0;
    }
    result = dds3SelectMenu(0, 1, 4, 1, 4, NULL, NULL, NULL, &D_005512E8);
    if (result == -1) {
        return -1;
    }
    selected = *(volatile s32 *)&D_005512E8;
    if (result == 1) {
        SetMotion(d->data, d->form, slSelectMotIndex + D_005512E0, 0);
        return 1;
    }
    switch (selected) {
    case 0:
        if ((dds3PadButtonState[5] & 2) && motion->type < 3) {
            motion->type++;
        }
        if ((dds3PadButtonState[4] & 2) && motion->type != 0) {
            motion->type--;
        }
        break;
    case 1:
        if (dds3PadButtonState[5] & 2) {
            motion->data = (u8)motion->data + 1;
        }
        if (dds3PadButtonState[4] & 2) {
            byteValue = ((volatile nbUnitMotion_t *)motion)->data;
            byteBits = *(volatile u8 *)&motion->data;
            if (byteValue >= 0) {
                motion->data = byteBits - 1;
            }
        }
        break;
    case 2:
        if (dds3PadButtonState[5] & 2) {
            motion->bframe = (u16)motion->bframe + 1;
        }
        if (dds3PadButtonState[4] & 2) {
            halfValue = ((volatile nbUnitMotion_t *)motion)->bframe;
            halfBits = *(volatile u16 *)&motion->bframe;
            if (halfValue > 0) {
                motion->bframe = halfBits - 1;
            }
        }
        break;
    case 3:
        if (dds3PadButtonState[5] & 2) {
            motion->hframe = (u16)motion->hframe + 1;
        }
        if (dds3PadButtonState[4] & 2) {
            halfValue = ((volatile nbUnitMotion_t *)motion)->hframe;
            halfBits = *(volatile u16 *)&motion->hframe;
            if (halfValue > 0) {
                motion->hframe = halfBits - 1;
            }
        }
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", SelectMotData);
#endif

extern const char *D_004F06F0[];
extern const char D_005514D8[];

#ifdef NON_MATCHING
s32 SelectMot(nbDebugProcessData_t *d, s32 active)
{
    sdfDrawTag_t *tag;
    frFontRequest_t *request;
    s32 i, y, yText, attr, color, result, base, selected;
    u8 button;

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    nbGetUnitWorkFromFormindex(d->form->formindex);
    dds3ConsPanelTagLinkZ(tag, 0x18C, 0x4C, 8, 20, 0xFF0000);
    y = 0x7BA0;
    yText = 80;
    selected = *(volatile s32 *)&D_005512E0;
    for (i = 0; i < 10; i++) {
        attr = 0;
        color = -0x80;
        if (selected == i) {
            if (active) {
                attr = 4;
                color = 0xFF0080;
            } else {
                attr = 5;
                color = 0xFFFF00;
            }
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8900, y, 0xFFFF80, attr,
                                             D_005514D8, slSelectMotIndex + i));
        y += 0xB0;
        request = frwGetSmallFontFRQ(0x1AE * 16, yText * 8, 0xFF0005, color,
                                     D_004F06F0[slSelectMotIndex + i], NULL);
        yText += 22;
        frDrawFont((frFontHandle_t *)request);
        frDestroyFontRequestAfter((frFontHandle_t *)request);
        selected = *(volatile s32 *)&D_005512E0;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
    if (!active) {
        return 0;
    }
    result = dds3SelectMenu(0, 1, 26, 1, 10, NULL, &slSelectMotIndex, NULL, &D_005512E0);
    if (result == -1) {
        return -1;
    }
    if (result == 1) {
        SetMotion(d->data, d->form, slSelectMotIndex + D_005512E0, 0);
        return 1;
    }
    button = ((volatile u8 *)dds3PadButtonState)[5];
    base = *(volatile s32 *)&slSelectMotIndex;
    if ((button & 2) && base + 10 < 26) {
        slSelectMotIndex = base + 20 < 26 ? base + 10 : 16;
    }
    button = ((volatile u8 *)dds3PadButtonState)[4];
    base = *(volatile s32 *)&slSelectMotIndex;
    if ((button & 2) && base > 0) {
        base -= 10;
        slSelectMotIndex = base < 0 ? 0 : base;
    }
    if ((s8)dds3PadButtonState[0x20] < 0) {
        return 2;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", SelectMot);
#endif

extern const char D_005514E0[];
extern s32 devilbase;
extern s32 devilcur;

#ifdef NON_MATCHING
s32 SelectDevil(void)
{
    sdfDrawTag_t *tag;
    frFontRequest_t *request;
    s32 i, y, yText, attr, color, result, base, selected;
    u8 button;

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    dds3ConsPanelTagLinkZ(tag, 0xA2, 0x48, 15, 20, 0xFF0000);
    y = 0x7BA0;
    yText = 80;
    selected = *(volatile s32 *)&devilcur;
    for (i = 0; i < 10; i++) {
        if (selected == i) {
            attr = 4;
            color = 0xFF0080;
        } else {
            attr = 0;
            color = -0x80;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7AA0, y, 0xFFFF80, attr,
                                             D_005514E0, devilbase + i));
        y += 0xB0;
        request = frwGetSmallFontFRQ(0xE6 * 16, yText * 8, 0xFF0005, color,
                                     text2[devilbase + i], NULL);
        yText += 22;
        frDrawFont((frFontHandle_t *)request);
        frDestroyFontRequestAfter((frFontHandle_t *)request);
        selected = *(volatile s32 *)&devilcur;
    }
    D_002F67F8.regFunc(&D_002F67F8, tag);
    result = dds3SelectMenu(0, 1, 0x180, 1, 10, NULL, &devilbase, NULL, &devilcur);
    if (result == -1 || result == 1) {
        return result;
    }
    button = ((volatile u8 *)dds3PadButtonState)[5];
    base = *(volatile s32 *)&devilbase;
    if ((button & 2) && base + 10 < 0x180) {
        devilbase = base + 20 < 0x180 ? base + 10 : 0x176;
    }
    button = ((volatile u8 *)dds3PadButtonState)[4];
    base = *(volatile s32 *)&devilbase;
    if ((button & 2) && base > 0) {
        base -= 10;
        devilbase = base < 0 ? 0 : base;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", SelectDevil);
#endif

void SetSelect(void)
{
    nbSetTargetProcess(0, 0, 0, 0, 3);
}

typedef struct
{
    /* 0x00 */ u32 flag;
    /* 0x04 */ u8 race;
    /* 0x05 */ u8 level;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ s16 aisyoid;
    /* 0x10 */ s8 param[6];
    /* 0x16 */ u16 skill[8];
    /* 0x26 */ s16 keisyotype;
    /* 0x28 */ u16 keisyoform;
    /* 0x2A */ u16 dropmakka;
    /* 0x2C */ u16 dropexp;
    /* 0x2E */ u8 dropitem[2];
    /* 0x30 */ u8 droppoint[2];
    /* 0x32 */ u16 specialbit;
    /* 0x34 */ u8 specialitem;
    /* 0x35 */ u8 specialpoint;
    /* 0x36 */ u8 hougyokupoint;
    /* 0x37 */ u8 masekipoint;
    /* 0x38 */ s8 attackattr;
    /* 0x39 */ u8 attackcnt;
    /* 0x3A */ u8 attackinterval;
    /* 0x3B */ u8 reserve1;
} datDevilFormat_t;

extern datDevilFormat_t datDevilTable[];
extern void nbReturnUnit(s32 partyindex, s32 lost, s32 dead);
extern u64 sdfDelayedResrcCheck(void);
extern void datSetDevilWork(s32 id, datUnitWork_t *work);
extern void nbSummonUnit(s32 side, s32 stockno, s32 formindex);
extern void nbSetEnemyFormation(nbMainProcessData_t *data);
extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

void SetDvlChange(nbDebugProcessData_t *d)
{
    datUnitWork_t *work;

    nbReturnUnit(d->partyindex, 0, 0);
    while (sdfDelayedResrcCheck() != 0) {
    }
    if (d->side == 1) {
        work = &d->data->enemyunit[d->stockno];
    } else {
        work = (datUnitWork_t *)((u8 *)DDS3_GBWK + d->stockno * sizeof(datUnitWork_t) + 0xA54);
    }
    datSetDevilWork(devilbase + devilcur, work);
    work->hp = work->maxhp = datDevilTable[devilbase + devilcur].maxhp;
    work->mp = work->maxmp = datDevilTable[devilbase + devilcur].maxmp;
    nbSummonUnit(d->side, d->stockno, d->formindex);
    switch (d->side) {
    case 1:
        nbSetEnemyFormation(d->data);
        break;
    case 0:
        nbSetPlayerFormation(d->data, 0);
        /* Retail keeps jal and a shared epilogue. This dead local reset
         * survives cc1 sibling selection and is deleted in the life pass
         * (qualified trace with compiler output parity). */
        work = NULL;
        break;
    }
}

void SetTarget_nbDebugProcess(nbDebugProcessData_t *a)
{
    nbFormation_t *form;
    nbParty_t *party;
    s32 i;

    a->form = NULL;
    a->formindex = 0;
    for (i = 0; i < 0x10; i++) {
        form = (nbFormation_t *)(i * sizeof(nbFormation_t) + (s32)a->data->form);
        if (form->stat & 8) {
            a->formindex = i;
            a->form = form;
            a->partyindex = form->partyindex;
            party = &a->data->party[form->partyindex];
            a->party = party;
            if (party->flag & 2) {
                a->side = 1;
                a->stockno = party->statindex;
                return;
            }
            a->side = 0;
            a->stockno = party->statindex;
            return;
        }
    }
}

void BackUpHpMpBad(nbDebugProcessData_t *d)
{
    datUnitWork_t *unit;

    unit = nbGetUnitWorkFromFormindex(d->form->formindex);
    d->hp = unit->hp;
    d->mp = unit->mp;
    d->bad = unit->badstatus;
}

void SetNextSeq(nbDebugProcessData_t *d)
{
    s32 seq;

    seq = d->nextseq;
    d->nextseq = -1;
    d->seq = seq;
    switch (seq) {
    case 1:
        SetSelect();
        break;
    case 3:
        SetDvlChange(d);
        break;
    case 2:
    case 4:
    case 8:
    case 9:
        break;
    case 5:
    case 6:
    case 7:
        BackUpHpMpBad(d);
        seq = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbDebugProcess", nbDebugProcess1);

void nbDebugShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    sdfChipFreeDirect(dds3GetProcessData(id));
    data->debugproc = NULL;
}

nbDebugProcessData_t *InitDebugProcessData(nbMainProcessData_t *data)
{
    nbDebugProcessData_t *w;

    w = sdfChipAllocDirect(sizeof(nbDebugProcessData_t));
    memset(w, 0, sizeof(nbDebugProcessData_t));
    w->data = data;
    w->seq = 0;
    w->formindex = 0;
    w->form = NULL;
    menucur = 0;
    return w;
}

void nbInitDebugProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_00545728, 0x3F9, 1, 1, nbDebugProcess1, nbDebugShutdown,
                          InitDebugProcessData(data));
    dds3LinkProcess(data->MainProcessID, id);
    data->debugproc = id;
}

INCLUDE_RODATA(const s32, "newbattle/nbDebugProcess", D_00545728);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", menucur);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", badcur);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005512E0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", slSelectMotIndex);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005512E8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005512EC);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005512F0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005512F8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551300);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551308);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551310);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551318);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551320);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551328);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551330);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551338);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551340);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551348);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551350);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551358);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551360);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551368);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551370);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551378);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551380);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551388);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551390);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551398);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513A0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513A8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513B0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513B8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513C0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513C8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513D0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513D8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513E0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513E8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513F0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005513F8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551400);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551408);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551410);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551418);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551420);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551428);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551430);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551438);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551440);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551448);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551450);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551458);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551460);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551468);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551470);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551478);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551480);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551488);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551490);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_00551498);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514A0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514A8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514B0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514B8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514C0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514C8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514D0);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514D8);

INCLUDE_SDATA(const s32, "newbattle/nbDebugProcess", D_005514E0);

