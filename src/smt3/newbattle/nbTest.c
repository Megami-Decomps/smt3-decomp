#include "common.h"
#include "newbattle.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"

extern void evtBitOff(s32 no);
extern s32 evtBitCheck(s32 no);
extern void evtSetAgeOfMoon(u8 age);

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

/* 164-byte entries indexed by devil id; only the field this unit reads is named. */
typedef struct {
    u8 unk_00[0x2];
    u16 ailevel;
    struct { u32 joken[2]; s32 no; } entry[13];
    u8 unk_A4[0x4];
} datDevilAI_t;

extern datDevilAI_t D_003989E0[];

extern s32 D_00551500;

extern u32 f;

extern datUnitWork_t D_005269D0;

extern u8 D_00526AC0[];

extern u8 D_00526AD0[];

extern u8 D_00526B48[];

extern u8 D_00526530[];

extern u8 D_00526540[];

extern u8 D_005267D0[];

extern u8 D_005267E0[];

extern datUnitWork_t test2_pdummy;

extern u16 test2_dvllist[];

extern s32 test_phearts;

extern char D_00545B78[];

extern void datAddHearts(s32 id);

extern void datEquipHearts(s32 HeartsID);

extern void datRemoveHearts(void);

extern s32 rstCalcExpFromLevel(s32 Index, s32 Level);

extern void effBTLSetFieldColor(u32 color);

extern datUnitWork_t test_skilllist;

extern u16 test_pskill[];

extern void SetBossDummy(void);

extern void *TestProcess1(void);

extern void *TestProcess2(void);

extern s32 dds3SearchProcessPriority(s32 priority);

extern void nbExit(void);

extern u32 nbEncountCalc(s32 id, f32 length);

extern void nbCallNewBattle(s32 encpack, s32 code);

extern s32 pack;

extern char D_00545B90[];

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

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

extern void evtBitOn(s32 no);
extern void evtBitOff(s32 no);
extern s32 evtBitCheck(s32 no);
extern void evtSetAgeOfMoon(u8 age);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 shutdownTimer,
                                         dds3ProcessFunc_t process, dds3ProcessFunc_t shutdown,
                                         void *work);

extern void nbSetBattleDummy(s32 count, void *devilList, void *skillList, void *passiveSkills,
                             datUnitWork_t *skillEntries);

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

extern datDevilAI_t D_00526B50;
extern datDevilAI_t D_00526BF4;
extern datDevilAI_t D_00526C98;
extern datDevilAI_t D_00526D3C;
extern datDevilAI_t D_00526DE0;
extern datDevilAI_t D_00526E84;
extern datDevilAI_t D_00526F28;
extern datDevilAI_t D_00526FCC;
extern datDevilAI_t D_00527070;
extern datDevilAI_t D_00527114;
extern datDevilAI_t D_005271B8;

extern void *memcpy(void *dest, const void *src, u32 size);

void SetBossDummy(void)
{
    memcpy(&D_003989E0[257], &D_00526B50, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[269], &D_00526BF4, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[278], &D_00526C98, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[279], &D_00526D3C, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[280], &D_00526D3C, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[281], &D_00526D3C, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[282], &D_00526D3C, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[288], &D_00526DE0, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[292], &D_00526E84, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[259], &D_00526F28, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[291], &D_00526FCC, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[258], &D_00527070, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[293], &D_00527114, sizeof(datDevilAI_t));
    memcpy(&D_003989E0[264], &D_005271B8, sizeof(datDevilAI_t));
    evtBitOn(0xC0B);
    evtBitOff(0xC01);
    evtBitOff(0xC07);
    {
        /* Preserve the native signed mode dispatch after the bit callbacks. */
        s32 mode = (s32)f;
        if (mode == 3) {
            return;
        }
        if (mode < 4) {
            if (mode == 2) {
                evtSetAgeOfMoon(8);
            }
        }
    }
}


typedef union {
    s32 value;
    struct { u16 low; u16 high; };
} nbTestSkillId_t;

extern void datSetDevilWork(s32 id, datUnitWork_t *work);
extern void *memset(void *dest, s32 value, u32 size);

#ifdef NON_MATCHING
void nbSetBattleDummy(s32 count, void *devilList, void *skillList, void *passiveSkills,
                      datUnitWork_t *dummy)
{
    volatile s32 *devil = devilList;
    volatile nbTestSkillId_t *skills = skillList;
    volatile nbTestSkillId_t *passive = passiveSkills;
    dds3GlobalWork_t *volatile *global = &DDS3_GBWK;
    volatile dds3GlobalWork_t *save;
    s32 i, j, next, nextDevil;
    u16 skill, start;

    memset(DDS3_GBWK->unitwork, 0, sizeof(DDS3_GBWK->unitwork));
    DDS3_GBWK->unitwork[0] = *dummy;
    i = 0;
    nextDevil = devil[0];
    while (nextDevil >= 0) {
        datSetDevilWork(devil[i], &DDS3_GBWK->unitwork[i + 4]);
        save = *global;
        next = skills[i * 30].value;
        start = save->unitwork[i + 4].skillcnt;
        j = 0;
        while (next >= 0) {
            skill = skills[i * 30 + j].low;
            next = skills[i * 30 + j + 1].value;
            save->unitwork[i + 4].skill[start + j] = skill;
            j++;
            save = *global;
        }
        nextDevil = devil[i + 1];
        save->unitwork[i + 4].skillcnt = start + j;
        i++;
    }
    save = *global;
    save->stockCount = i + 1;
    save->stockOrder[0] = 0;
    for (j = 0; j < i; j++) {
        (*global)->stockOrder[j + 1] = j + 4;
    }
    j = 0;
    next = passive[0].value;
    if (next >= 0) {
        save = *global;
        do {
            skill = passive[j].low;
            next = passive[j + 1].value;
            save->unitwork[0].skill[j] = skill;
            j++;
        } while (next >= 0);
    }
    save = *global;
    save->maka = 0;
    save->unitwork[0].skillcnt = j;
    for (i = 0; i < 160; i++) {
        (*global)->stockRemainder[i] = 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbTest", nbSetBattleDummy);
#endif

void nbInitPlayer(void)
{
    nbSetBattleDummy(D_00551500, D_00526530, D_00526540, test_pskill, &test_skilllist);
    evtBitOn(0xC04);
}

void DrawTest(void) {
}

typedef struct {
    u8 reserved0000[0x1928];
    u8 quantity[0xA0];
} testInventory_t;

void *TestProcess5(void)
{
    s32 i;

    evtBitOn(0xC00);
    evtBitOn(0xC01);
    evtBitOff(0xC02);
    evtBitOff(0xC03);
    evtBitOff(0xC04);
    evtBitOff(0xC05);
    evtBitOn(0xC06);
    evtBitOn(0xC07);
    evtBitOn(0xC0A);
    evtBitOn(0xC0C);
    effBTLSetFieldColor(1);
    evtBitOff(0xC0D);
    evtBitOff(0xC10);
    evtBitOff(0xC12);
    nbSetBattleDummy(D_00551500, D_005267D0, D_005267E0, test2_dvllist, &test2_pdummy);

    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))[7].hp = 0;
    DDS3_GBWK->maka = 0x15F90;
    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))[7].badstatus = 0x800;
    for (i = 0; i < 0xA0; i++) {
        ((testInventory_t *)DDS3_GBWK)->quantity[i] = 9;
    }

    for (i = 0; i < 16; i++) {
        datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + i;
        if ((unit->flag & 1) != 0 && unit->badstatus != 0x800) {
            u16 maxHp = unit->maxhp;
            u16 maxMp = unit->maxmp;
            unit->badstatus = 0;
            unit->hp = maxHp;
            unit->mp = maxMp;
        }
    }

    *(s32 *)((u8 *)DDS3_GBWK + 0x11AB8) |= 0xC;
    return TestProcess1;
}


void *TestProcess4(void)
{
    s32 i;

    evtBitOn(0xC00);
    evtBitOn(0xC01);
    evtBitOff(0xC02);
    evtBitOff(0xC03);
    evtBitOff(0xC04);
    evtBitOff(0xC05);
    evtBitOn(0xC06);
    evtBitOn(0xC07);
    evtBitOn(0xC0A);
    effBTLSetFieldColor(0);
    evtBitOff(0xC0D);
    evtBitOff(0xC10);
    evtBitOff(0xC12);
    nbSetBattleDummy(D_00551500, D_005267D0, D_005267E0, test2_dvllist, &test2_pdummy);
    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->exp = rstCalcExpFromLevel(0, 0x32);
    DDS3_GBWK->maka = 0x15F90;
    DDS3_GBWK->akuma[7].badstatus = 0x800;
    DDS3_GBWK->akuma[7].hp = 0;
    for (i = 0; i < 0xA0; i++) {
        DDS3_GBWK->stocklist[i + 4] = 9;
    }
    return TestProcess1;
}

void *TestProcess8(void)
{
    s32 i;
    u8 *ai;

    evtBitOn(0xC00);
    evtBitOn(0xC01);
    evtBitOff(0xC02);
    evtBitOff(0xC03);
    evtBitOff(0xC04);
    evtBitOff(0xC05);
    evtBitOn(0xC06);
    evtBitOff(0xC07);
    evtBitOn(0xC0A);
    evtBitOn(0xC0C);
    effBTLSetFieldColor(0);
    evtBitOff(0xC0D);
    evtBitOff(0xC10);
    evtBitOff(0xC12);
    evtBitOn(0xC0F);
    evtBitOn(0xC11);
    nbSetBattleDummy(D_00551500, D_00526AC0, D_00526AD0, D_00526B48, &D_005269D0);
    ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))->exp = rstCalcExpFromLevel(0, 0x32);
    ai = (u8 *)D_003989E0;
    f = 0xC;
    *(s16 *)(ai + 0x3C38) = 0;
    *(s16 *)(ai + 0x3C62) = 0x62;
    *(s16 *)(ai + 0x3C6A) = 0x62;
    for (i = 0; i < 0xA0; i++) {
        DDS3_GBWK->stocklist[i + 4] = 9;
    }
    return TestProcess1;
}

void *TestProcess3(void)
{
    evtBitOff(0xC00);
    evtBitOff(0xC01);
    evtBitOff(0xC02);
    evtBitOff(0xC03);
    evtBitOn(0xC04);
    evtBitOff(0xC05);
    evtBitOff(0xC06);
    evtBitOff(0xC07);
    evtBitOff(0xC0A);
    evtBitOn(0xC0C);
    effBTLSetFieldColor(0);
    evtBitOff(0xC0D);
    evtBitOff(0xC10);
    evtBitOff(0xC12);
    nbSetBattleDummy(D_00551500, D_00526530, D_00526540, test_pskill, &test_skilllist);
    if (test_phearts > 0) {
        printf(D_00545B78);
        datAddHearts(test_phearts);
        datEquipHearts(test_phearts);
    } else {
        datRemoveHearts();
    }
    return TestProcess1;
}

void *TestProcess7(void)
{
    SetBossDummy();
    return TestProcess1;
}

void *TestProcess2(void)
{
    if (dds3SearchProcessPriority(0x3FA) == 0) {
        nbExit();
        return (void *)-1;
    }
    return NULL;
}

void *TestProcess6(void)
{
    u32 encounter;

    DrawTest();
    encounter = nbEncountCalc(pack, 100.0f);
    if (encounter == 0) {
        return NULL;
    }
    nbCallNewBattle(pack, encounter);
    return TestProcess2;
}

void *TestProcess1(void)
{
    return NULL;
}

void TestShutdown(void) {
}

void nbExitBattleTest(void)
{
    nbExit();
    dds3KillProcessName(D_00545B90, 1);
}

void nbInitBattleTest(void)
{
    dds3GlobalWork_t *work;

    if (evtBitCheck(0xC08) == 0) {
        nbSetBattleDummy(4, D_00526530, D_00526540, test_pskill, &test_skilllist);
        evtBitOn(0xC08);
    }

    work = DDS3_GBWK;
    work->encountCount = 0;
    work->encountkyori = 0.0f;
    *(s32 *)((u8 *)work + 0x11AB8) |= 0xC;
    dds3StartProcess(D_00545B90, 0x2B0C, 1, 1, TestProcess1, TestShutdown, NULL);
}

INCLUDE_RODATA(const s32, "newbattle/nbTest", D_00545B78);

INCLUDE_RODATA(const s32, "newbattle/nbTest", D_00545B90);

INCLUDE_SDATA(const s32, "newbattle/nbTest", D_00551500);

INCLUDE_SDATA(const s32, "newbattle/nbTest", pack);

INCLUDE_SDATA(const s32, "newbattle/nbTest", f);

INCLUDE_SDATA(const s32, "newbattle/nbTest", D_00551514);

INCLUDE_SDATA(const s32, "newbattle/nbTest", test_phearts);

