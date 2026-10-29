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

/* Only the fields this unit touches are named; the entry is 38 bytes. */
typedef struct
{
    s8 backAttack;
    s8 esc;
    u8 unk_02[0x4];
    u16 id[11];
    u8 unk_1C[0x6];
    u8 unk_22;
    u8 unk_23;
    u16 mae;
} nbEncountForm_t;

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern nbEncountForm_t nbEncountFormTable[];

extern datDevilFormat_t datDevilTable[];

extern u32 nbEncountCalc(s32 id, f32 length);

extern s32 nbEncountMax;

extern s32 func_0028CFC8(s32 tableno, s32 encno);

extern s32 nbGetUnitID(datUnitWork_t *work);

extern s32 datGetPlayerLevel(void);

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

typedef struct datEncountRitu_t {
    u16 encountid;
    u16 ritu;
    u8 sensei;
    u8 renzokuritu;
    u16 renzokuindex;
} datEncountRitu_t;

typedef struct datEncountTbl_t {
    s32 normalritu;
    datEncountRitu_t ritu[12];
} datEncountTbl_t;

/* Native encounter packs embed their four table selectors and four rows. */
typedef struct nbEncountEntry_t {
    u16 stageid;
    u16 areaid;
    u16 btlsound;
    u16 moonchk;
    u16 table2;
    u16 eventbit1;
    u16 eventbit2;
    u8 table1[4];
    u8 reserved12[2];
    datEncountTbl_t enctable[4];
} nbEncountEntry_t;

extern nbEncountEntry_t nbEncountTable[];

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

extern s32 evtGetAgeOfMoon(void);
extern s32 evtBitCheck(s32 no);

#ifdef NON_MATCHING
s32 nbGetEncountJoken(s32 id)
{
    nbEncountEntry_t *entry = &nbEncountTable[id];
    s32 first = 0;
    s32 second = 0;

    if (entry->moonchk != 0 &&
        ((entry->moonchk >> evtGetAgeOfMoon()) & 1) != 0) {
        return entry->table2;
    }
    if (entry->eventbit1 != 0) {
        first = evtBitCheck(entry->eventbit1);
    }
    if (entry->eventbit2 != 0) {
        second = evtBitCheck(entry->eventbit2);
    }
    if (first == 0) {
        return entry->table1[second != 0 ? 2 : 0];
    }
    return entry->table1[second != 0 ? 3 : 1];
}
#else
INCLUDE_ASM(const s32, "newbattle/nbEncount", nbGetEncountJoken);
#endif

extern s32 nbGetEncountJoken(s32 id);

extern s32 D_0054FF1C;
extern s32 dds3GetRandIntA(s32 minimum, s32 range);
extern s32 printf(const char *format, ...);
extern char D_00545BA0[];

s32 nbGetEncountMax(s32 id)
{
    s32 condition;
    s32 total;
    s32 i;
    s32 maximum;

    if (id == 0) {
        return 0;
    }
    condition = nbGetEncountJoken(id);
    total = 0;
    for (i = 0; i < 12; i++) {
        total += nbEncountTable[id].enctable[condition].ritu[i].ritu;
    }
    if (total == 0) {
        return 0;
    }
    if (nbEncountMax > 0) {
        return nbEncountMax;
    }
    maximum = D_0054FF1C - nbEncountTable[id].enctable[condition].normalritu * 48 / 100;
    nbEncountMax = dds3GetRandIntA(0, maximum * 2 - 32) + 16;
    printf(D_00545BA0, nbEncountMax, maximum);
    return nbEncountMax;
}

/* The count is stored as u16, but the public getter returns s32 (HD/IDB). */
s32 nbGetEncountCnt(void)
{
    return DDS3_GBWK->encountCount;
}

extern s32 nbCheckEncountLevel(s32 table);
extern void fclClearTerminalState(void);

u32 nbEncountCalc(s32 id, f32 length)
{
    s32 condition;
    s32 maximum;
    /* Reuse the integer count for movement chunks and encounter-weight totals.
     * cc1probe --func nbEncountCalc --alloc on this unit passed hooked,
     * unhooked and plain-output parity. Expansion uids 110/281/488 share
     * pseudo 88; global allocation gives it priority 5744 versus 3879 for
     * the loop index. Splitting the counts reverses their register homes. */
    s32 count;
    s32 i;
    s32 random;
    s32 encounter;
    s32 cumulative;

    if (id == 0 || length == 0.0f) {
        return 0;
    }
    condition = nbGetEncountJoken(id);
    maximum = nbGetEncountMax(id);
    if (maximum == 0) {
        return 0;
    }
    DDS3_GBWK->encountkyori += length;
    if (DDS3_GBWK->encountkyori < 100.0f) {
        return 0;
    }
    count = DDS3_GBWK->encountkyori / 100.0f;
    DDS3_GBWK->encountkyori -= count * 100.0f;
    for (i = 0; i < count; i++) {
        if (DDS3_GBWK->maFlags & 2) {
            random = dds3GetRandIntA(0, 7);
            DDS3_GBWK->encountCount = DDS3_GBWK->encountCount + random + 6;
        } else {
            random = dds3GetRandIntA(0, 3);
            DDS3_GBWK->encountCount = DDS3_GBWK->encountCount + random + 2;
        }
    }
    if (DDS3_GBWK->encountCount > 65000) {
        DDS3_GBWK->encountCount = 65000;
    }
    if (DDS3_GBWK->encountCount < maximum) {
        return 0;
    }
    random = dds3GetRandIntA(0, 1000);
    count = 0;
    for (i = 0; i < 12; i++) {
        if (nbEncountTable[id].enctable[condition].ritu[i].encountid != 0) {
            cumulative = count + nbEncountTable[id].enctable[condition].ritu[i].ritu;
            if (random < cumulative) {
                encounter = nbEncountTable[id].enctable[condition].ritu[i].encountid;
                if ((DDS3_GBWK->maFlags & 1) && nbCheckEncountLevel(encounter) == 0) {
                    fclClearTerminalState();
                    return 0;
                }
                nbEncountMax = 0;
                return ((u32)condition << 24) | (i << 16) | encounter;
            }
            count = cumulative;
        }
    }
    return 0;
}

s32 nbCheckEncountLevel(s32 tbl)
{
    u16 *p;
    s32 sum;
    s32 cnt;
    s32 i;
    s32 id;
    s32 lvl;

    if (tbl == 0) {
        return 0;
    }
    sum = 0;
    cnt = 0;
    p = nbEncountFormTable[tbl].id;
    for (i = 10; i >= 0; i--) {
        id = *p++;
        if (id != 0) {
            cnt++;
            sum += datDevilTable[id].level;
        }
    }
    if (cnt == 0) {
        return 0;
    }
    sum = sum / cnt;
    lvl = datGetPlayerLevel();
    if (sum + lvl / 4 < lvl) {
        return 0;
    }
    return 1;
}

void fclClearTerminalState(void)
{
    /* The u8 * local is load-bearing: it caches the base so ee-gcc emits one
     * $gp load for both stores. Reaching through DDS3_GBWK-> per statement
     * reloads $gp and reorders them. +0x1CF4 is encountkyori (an f32 retail
     * stores an integer zero into) and +0x1CF8 is encountCount. */
    u8 *gb;

    gb = (u8 *)DDS3_GBWK;
    *(s32 *)(gb + 0x1CF4) = 0;
    *(u16 *)(gb + 0x1CF8) = 0;
    nbEncountMax = 0;
}

s32 func_0028CFC8(s32 tableno, s32 encno)
{
    s32 v;

    v = 0;
    if (tableno > 0) {
        v = nbEncountTable[tableno].btlsound;
    }
    if (nbEncountFormTable[encno].mae != 0) {
        v = nbEncountFormTable[encno].mae;
    }
    if (v != 0) {
        return v;
    }
    return 1;
}

s32 nbInitMaeTableDefault(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    return func_0028CFC8(data->unk_28, data->encno);
}

s32 nbInitMaeTable(u32 code)
{
    return func_0028CFC8(code >> 24, code & 0xFFFF);
}

s32 nbGetUnitWorkFromAction(nbFormation_t *form)
{
    return nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
}

INCLUDE_RODATA(const s32, "newbattle/nbEncount", D_00545BA0);

INCLUDE_SDATA(const s32, "newbattle/nbEncount", nbEncountMax);

