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
    sdfTexHandle_t *pnltex[4];
    s32 unk_130;
    s32 unk_134;
    nbPacHandle_t *modelpack;
    void *modelpackbuf;
} nbMainProcessData_t;

typedef struct
{
    u32 flag;
    u8 race;
    u8 level;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    s16 aisyoid;
    s8 param[6];
    u16 skill[8];
    s16 keisyotype;
    u16 keisyoform;
    u16 dropmakka;
    u16 dropexp;
    u8 dropitem[2];
    u8 droppoint[2];
    u16 specialbit;
    u8 specialitem;
    u8 specialpoint;
    u8 hougyokupoint;
    u8 masekipoint;
    s8 attackattr;
    u8 attackcnt;
    u8 attackinterval;
    u8 reserve1;
} datDevilFormat_t;

typedef struct _FRQ _FRQ;

/* Only the fields this unit reads are named; x and y are in 1/16 pixels. */
struct _FRQ
{
    u8 unk_00[0x4];
    s32 x;
    s32 y;
};

/* The nb_help process work; only the fields this unit touches are named. */
typedef struct nbHelpProcessData_t
{
    s32 type;
    char *text1;
    char *text2;
    s32 text3;
    s32 time;
    _FRQ *textmax;
    s32 textwidth;
    s32 nextwidth;
    u32 textcol;
    s32 reqmsgid;
    s32 mesid;
    s32 nextmeshandle;
    s32 nextmesid;
    s32 panelAlpha;
} nbHelpProcessData_t;

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

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

extern datDevilFormat_t datDevilTable[];

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern char D_005514F0[];

extern void nbHelpOff(void);

extern s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);

extern _FRQ *frwGetSmallFontFRQ(s32 X, s32 Y, u32 Pri, u32 Color, char *pStr, s32 pFRQ);

extern _FRQ *frGetCNameFRQ(s8 no, s8 style, _FRQ *chain, s8 tp);

extern s32 frGetStrWidth2(_FRQ *pFRQ);

extern _FRQ *frDestroyFontRequestAfter(_FRQ *pFRQ);

extern s32 datGetPlayerLNDLevel(void *work);

extern char *datExplainTable[];

extern char *text1[];

extern char *text2[];

extern char *pStr[];

extern char *D_005263F8[];

extern void nbDispText(char *text, char *text2, s32 type, s32 time, u32 color);

extern void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern s32 nbHelpProcess1(dds3ProcessID_t *id);

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

extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

void nbDispText(char *text, char *text2, s32 type, s32 time, u32 color)
{
    nbHelpProcessData_t *w;
    _FRQ *frq;

    w = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    nbHelpOff();
    w->text1 = text;
    w->text2 = text2;
    if (type == 1) {
        w->reqmsgid = w->nextmeshandle;
        w->mesid = w->nextmesid;
    }
    w->textcol = color;
    switch (type) {
    case 1:
        w->textwidth = itfMesMngGetWidth(w->reqmsgid, w->mesid, 0) >> 4;
        break;
    case 2:
        frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, color, w->text1, 0);
        w->textwidth = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        frq = frGetCNameFRQ(0, 1, 0, 0);
        w->nextwidth = w->textwidth + 10;
        w->textwidth = w->textwidth + frGetStrWidth2(frq) + 10;
        frDestroyFontRequestAfter(frq);
        break;
    case 0:
    default:
        frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, w->textcol, w->text1, 0);
        w->textwidth = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        if (w->text2 != NULL) {
            frq = frwGetSmallFontFRQ(0, 0, 0xFF0051, w->textcol, w->text2, 0);
            w->nextwidth = w->textwidth + 10;
            w->textwidth = w->textwidth + frGetStrWidth2(frq) + 10;
            frDestroyFontRequestAfter(frq);
        }
        break;
    }
    w->time = time;
    w->type = type;
    w->text3 = 0;
}

void nbDispSkillName(s32 id)
{
    nbDispText(text1[id], NULL, 0, 30, 0x89FEFF80);
}

void nbDispTextById(s32 no)
{
    nbDispText(pStr[no], NULL, 0, 30, 0x89FEFF80);
}

void nb_289078(s32 no, s32 type)
{
    nbDispText(D_005263F8[no], NULL, type, 30, 0x89FEFF80);
}

void nbDispActionText(u32 command)
{
    switch (command) {
    case 0:
        nbDispText(text1[0x200], NULL, 0, 30, 0x89FEFF80);
        return;
    case 2:
        nbDispText(text1[0x20E], NULL, 0, 30, 0x89FEFF80);
        return;
    case 3:
        nbDispText(text1[0x202], NULL, 0, 30, 0x89FEFF80);
        return;
    case 6:
        nbDispText(text1[0x203], NULL, 0, 30, 0x89FEFF80);
        return;
    case 7:
        nbDispText(text1[0x204], NULL, 0, 30, 0x89FEFF80);
        break;
    }}

void nb_2891A8(s32 no)
{
    nbDispText(text1[0x200 + no], NULL, 0, 30, 0x89FEFF80);
}

void nbDispDevilName(s32 id)
{
    char **lndtext;

    nbGetMainProcessData();
    if (id == 0) {
        lndtext = &datExplainTable[0x40];
        nbDispText(lndtext[datGetPlayerLNDLevel((u8 *)DDS3_GBWK + 0xA54)], NULL, 2, 2,
                   0xA09DC380);
    } else {
        nbDispText(datExplainTable[datDevilTable[id].race], text2[id],
                   0, 2, 0xA09DC380);
    }}

void nbDispJyokyoHelp(s32 msgid, s32 no, s32 var_id, s32 var_ind)
{
    s32 digit;

    digit = var_id;
    if (var_id == 1) {
        if (var_ind == 0) {
            digit = 10;
        }
    }
    if (digit >= 0) {
        itfMesMngSetVar(msgid, 0, var_ind, digit);
    }
    nbDispMsgHelp(msgid, no, 1);
}

void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype)
{
    nbHelpProcessData_t *w;

    w = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    w->nextmeshandle = msgid;
    w->nextmesid = no;
    switch (msgtype) {
    case 0:
        nbDispText(NULL, NULL, 1, 45, 0x89FEFF80);
        return;
    case 1:
        nbDispText(NULL, NULL, 1, 45, 0x89FEFF80);
        return;
    case 2:
        nbDispText(NULL, NULL, 1, 20, 0xA09DC380);
        return;
    case 3:
        nbDispText(NULL, NULL, 1, 30, 0x89FEFF80);
        break;
    }}

void nbHelpOff(void)
{
    nbHelpProcessData_t *h;

    h = dds3GetProcessData(dds3SearchProcessName(D_005514F0));
    if (h->text1 == NULL) {
        if (h->type != 1) {
            return;
        }
        if (h->reqmsgid < 0) {
            return;
        }
    }
    h->text1 = NULL;
    h->text2 = NULL;
    if (h->type == 1) {
        itfMesMngRequestMessageRelease(h->reqmsgid, 0);
        h->reqmsgid = -1;
    }
}

extern void nbPnl_setOt(s32 ot);
extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail,
                         s32 date, s32 datm, s32 zte, s32 ztst);
extern void nbPnl_rsetAlpha(s32 mode);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
                        s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *texture);

void nbDrawHelpPanel(nbHelpProcessData_t *w)
{
    nbMainProcessData_t *data;
    s32 halfWidth;
    s32 sideWidth;
    s32 centerX = 0x100;
    u32 color;

    data = nbGetMainProcessData();
    nbPnl_setOt(0x47);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    halfWidth = w->textwidth / 2;
    color = ((u32)w->panelAlpha << 24) | 0x808080;
    halfWidth += 0x60;
    if (halfWidth < 0x88) {
        halfWidth = 0x88;
    }
    sideWidth = halfWidth - 0x88;
    if (halfWidth < 0x89) {
        sideWidth = 0;
    }
    nbPnlSprite(centerX - halfWidth, 0x4D, 0x88, 0x21, 1, 0x5F, 0x88, 0x21, color, data->pnltex[0]);
    nbPnlSprite(halfWidth + centerX, 0x6E, -0x88, -0x21, 1, 0x5F, 0x88, 0x21, color, data->pnltex[0]);
    nbPnlSprite(centerX - sideWidth, 0x4D, sideWidth * 2, 0x21, 0x87, 0x5F, 1, 0x21, color,
                data->pnltex[0]);
}

extern void cmpSetSkillListRows(s32 rows);
extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);
extern void itfMesMngRequestMessage(s32 id, s32 message, s32 page);
extern void frSetFontColor(_FRQ *font, u32 color);
extern s32 frDrawFontEx(_FRQ *font, s8 mode);
extern void frSetDrawPosition(_FRQ *font, u32 x, u32 y);
extern void nbDrawHelpPanel(nbHelpProcessData_t *w);

s32 nbHelpProcess1(dds3ProcessID_t *id)
{
    nbHelpProcessData_t *w;
    s32 x;

    w = dds3GetProcessData(id);
    if (w->text1 != NULL || (w->type == 1 && w->reqmsgid >= 0)) {
        cmpSetSkillListRows(20);
        x = 256 - w->textwidth / 2;
        switch (w->type) {
        case 1:
            itfMesMngSetMesPosition(w->reqmsgid, x << 4, 0x2A0);
            itfMesMngRequestMessage(w->reqmsgid, w->mesid, 0);
            break;
        case 2:
            w->textmax = frwGetSmallFontFRQ(x << 4, 0x2A0, 0xFF0051, w->textcol, w->text1, 0);
            frSetFontColor(w->textmax, w->panelAlpha | 0x80808000);
            frDrawFontEx(w->textmax, 1);
            frDestroyFontRequestAfter(w->textmax);
            w->textmax = NULL;
            x += w->nextwidth;
            w->textmax = frGetCNameFRQ(0, 1, 0, 0);
            frSetDrawPosition(w->textmax, x << 4, 0x2A0);
            frSetFontColor(w->textmax, w->panelAlpha | 0x80808000);
            frDrawFontEx(w->textmax, 1);
            frDestroyFontRequestAfter(w->textmax);
            w->textmax = NULL;
            break;
        case 0:
            w->textmax = frwGetSmallFontFRQ(x << 4, 0x2A0, 0xFF0051, w->textcol, w->text1, 0);
            frSetFontColor(w->textmax, w->panelAlpha | 0x80808000);
            frDrawFontEx(w->textmax, 1);
            frDestroyFontRequestAfter(w->textmax);
            w->textmax = NULL;
            if (w->text2 != NULL) {
                x += w->nextwidth;
                w->textmax = frwGetSmallFontFRQ(x << 4, 0x2A0, 0xFF0051, w->textcol, w->text2, 0);
                frSetFontColor(w->textmax, w->panelAlpha | 0x80808000);
                frDrawFontEx(w->textmax, 1);
                frDestroyFontRequestAfter(w->textmax);
                w->textmax = NULL;
            }
            break;
        }
        if (w->type != 1) {
            nbDrawHelpPanel(w);
        }
        cmpSetSkillListRows(-1);
        w->text3++;
        if (w->time > 0 && w->text3 > w->time) {
            nbHelpOff();
        } else if (w->type == 1) {
            nbDrawHelpPanel(w);
        }
    }
    return 0;
}

void nbHelpShutdown(dds3ProcessID_t *id)
{
    nbHelpProcessData_t *w;

    w = dds3GetProcessData(id);
    if (w->text1 != NULL || (w->type == 1 && w->reqmsgid >= 0)) {
        w->text1 = NULL;
        w->text2 = NULL;
        if (w->type == 1) {
            itfMesMngRequestMessageRelease(w->reqmsgid, 0);
            w->reqmsgid = -1;
        }
    }
    sdfChipFreeDirect(w);
}

nbHelpProcessData_t *nbAllocHelpWork(void)
{
    nbHelpProcessData_t *w;

    w = sdfChipAllocDirect(sizeof(nbHelpProcessData_t));
    memset(w, 0, sizeof(nbHelpProcessData_t));
    w->reqmsgid = -1;
    w->panelAlpha = 0x80;
    return w;
}

void nbInitHelpProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *proc;

    proc = dds3StartProcess(D_005514F0, 0x3F9, 1, 1, nbHelpProcess1, nbHelpShutdown,
                            nbAllocHelpWork());
    dds3LinkProcess(data->MainProcessID, proc);
    data->helpproc = proc;
}

INCLUDE_SDATA(const s32, "newbattle/nbHelpProcess", D_005514F0);

