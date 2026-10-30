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

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party);

extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern void nbSetMesItem(s32 msgid, s32 it);

extern s32 nbGetKureItemA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

extern s32 datGetParam(datUnitWork_t *work, s32 paratype);
struct nbKoukaPacket_t;
extern s32 nbMakeNegoBackPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                nbFormation_t *target);
extern s32 nbMakeNegoWorkPosPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                   nbFormation_t *target);
extern s32 nbMakeNegoForwardPacket(s32 startframe, s32 uniqueid, nbFormation_t *form,
                                  nbFormation_t *target);
extern s32 nbMakeUnitFadeInPacket(s32 startframe, s32 uniqueid, nbFormation_t *form, u32 col,
                                 s32 delay, s32 aframe, s32 colframe);
extern struct nbKoukaPacket_t *nbAddHomeMoveKoukaPacket(s32 startframe, s32 uniqueid, s32 partyno,
                                                f32 speed);
extern void nbCameraSetTaizi(nbFormation_t *invoke, nbFormation_t *target);
extern void nbFormDispOff2(nbFormation_t *form);
extern void nbFormDispOn2(nbFormation_t *form);




extern s32 datGetRace(datUnitWork_t *work);

extern s32 nbSearchHojoSkillWork(nbNegoProcessData_t *n, s32 type);

extern s32 evtBitCheck(s32 no);

extern s32 nbGetNegoKureMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbGetHojoSkillWorkCnt(nbNegoProcessData_t *n);

extern s32 datCheckItem(s32 id, s32 num);

extern void nbMakaDispOn(s32 type);

extern s32 datCheckDying(datUnitWork_t *work);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void nbAddNegoIdKouho(nbNegoProcessData_t *n, u32 id);

extern void nbDispNegoMessage(nbNegoProcessData_t *n, s32 msgid, s32 msgno, s32 sel);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

// @@DECLS@@
extern void datAddMaka(s32 addmaka);

extern void datAddItem(s32 id, s32 num);

extern s32 nbSetNegoYesMotion(nbNegoProcessData_t *n, nbFormation_t *form);

extern s32 nbSetNegoNoMotion(nbNegoProcessData_t *n, nbFormation_t *form);

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

void nbClsNegoMessage(nbNegoProcessData_t *n);

s32 nbGetNegoYaruMakkaA(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

u32 nbGetPrevNegoId(nbNegoProcessData_t *n);

void nbSetMesMakka(s32 msgid, s32 i);

extern nbNegoSkillEntry_t D_003E1338[];
extern s32 nbFlowCheckUnit2(datUnitWork_t *pwork, datUnitWork_t *ework,
                            s32 ptype, s32 etype, s32 data);

extern s32 datCheckStockFull(void);
extern s32 nbFlowCheckUnit(nbNegoProcessData_t *n, s32 ptype, s32 etype, s32 data);

#ifdef NON_MATCHING
s32 Flow6_1(nbNegoProcessData_t *n)
{
    nbNegoSkillCondition_t *condition;
    s32 skill;
    s32 ptype;
    s32 etype;
    s32 i;

    nbAddNegoIdKouho(n, 0x60003);
    nbAddNegoIdKouho(n, 0x60050);
    nbAddNegoIdKouho(n, 0x60006);
    nbAddNegoIdKouho(n, 0x90006);
    nbAddNegoIdKouho(n, 0x60002);
    if (datCheckStockFull() != 0 && n->negoreqtype == 0) {
        n->nextnegoid = n->nextkouho[3];
        return 0;
    }
    skill = n->negoskillid - 0x180;
    n->aisyo = 0;
    condition = D_003E1338[skill].condition;
    for (i = 0; i < 4; i++, condition++) {
        ptype = condition->ptype;
        etype = condition->etype;
        if ((ptype != 0 || etype != 0) && nbFlowCheckUnit(n, ptype, etype, 1)) {
            n->aisyo = 1;
            n->aisyomes = condition->message;
            break;
        }
    }
    if (n->aisyo == 0) {
        condition = &D_003E1338[skill].condition[4];
        for (i = 0; i < 4; i++, condition++) {
            ptype = condition->ptype;
            etype = condition->etype;
            if ((ptype != 0 || etype != 0) && nbFlowCheckUnit(n, ptype, etype, 0)) {
                n->aisyo = 2;
                n->aisyomes = condition->message;
                break;
            }
        }
    }
    switch (n->aisyo) {
    case 0:
        n->nextnegoid = 0x60002;
        break;
    case 1:
        n->nextnegoid = 0x60003;
        break;
    case 2:
        n->nextnegoid = 0x60006;
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowTwo", Flow6_1);
#endif

s32 Flow6_2(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, 0x60009);
    nbAddNegoIdKouho(n, 0x60050);
    if (datCheckSyojiSkill(n->pwork, 0x163) == 1) {
        n->nextnegoid = n->nextkouho[1];
        return 0;
    }
    do {
        mes = dds3GetRandIntA(0, 4) + 0x1C;
    } while (mes == n->mes6_2);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    n->mes6_2 = mes;
    n->nextnegoid = n->nextkouho[0];
    return 0;
}

s32 Flow6_80(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, 0x60051);
    nbAddNegoIdKouho(n, 0x60009);
    if ((u32)dds3GetRandIntA(0, 0xA) < 4) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        do {
            mes = dds3GetRandIntA(0, 4) + 0x1C;
        } while (mes == n->mes6_2);
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        n->mes6_2 = mes;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_81(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60004);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x126, 0);
    return 0;
}

s32 Flow6_3(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoYesMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60004);
        nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->aisyomes, 0);
        break;
    }
    return 0;
}

s32 Flow6_4(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60005);
    nbAddNegoIdKouho(n, 0x90001);
    if ((s32)dds3GetRandIntA(0, 100) < 65) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_5(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], 0x26, 0);
    return 0;
}

s32 Flow6_6(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60007);
        nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->aisyomes, 0);
        break;
    }
    return 0;
}

s32 Flow6_7(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60008);
    nbAddNegoIdKouho(n, 0xA0001);
    if ((s32)dds3GetRandIntA(0, 100) < 70) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_8(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], 0x27, 0);
    return 0;
}

extern s32 nbGetNegoRequestType(s32 reqtype);
extern char D_00544FF0[];
#include "sdk_libc.h"

INCLUDE_RODATA(const s32, "newbattle/NbNegoFlowTwo", D_00544FF0);

s32 Flow6_9(nbNegoProcessData_t *n)
{
    s32 requestType;
    s32 overrideType;

    nbAddNegoIdKouho(n, 0x60011);
    nbAddNegoIdKouho(n, 0x60015);
    nbAddNegoIdKouho(n, 0x6000B);
    nbAddNegoIdKouho(n, 0x6000A);
    nbAddNegoIdKouho(n, 0x6000C);

    do {
        requestType = nbGetNegoRequestType(master_text[n->ework->id].reqtype);
    } while ((u32)n->prevreqtype >= 2 &&
             (requestType == 4 || requestType == n->prevreqtype));

    overrideType = n->negoreqtype;
    n->requestdata = 0;
    if (overrideType == 4) {
        requestType = 0;
    }
    if (overrideType == 5) {
        requestType = 5;
    }
    n->requesttype = requestType;
    printf(D_00544FF0, requestType);

    if ((u32)requestType < 6) {
        switch (requestType) {
        case 0:
            n->nextnegoid = n->nextkouho[0];
            break;
        case 1:
            n->nextnegoid = n->nextkouho[0];
            break;
        case 2:
            n->nextnegoid = n->nextkouho[1];
            break;
        case 3:
            n->nextnegoid = n->nextkouho[2];
            break;
        case 4:
            n->nextnegoid = n->nextkouho[3];
            break;
        case 5:
            n->nextnegoid = n->nextkouho[4];
            break;
        }
    }
    n->prevreqtype = requestType;
    return 0;
}

s32 Flow6_10(nbNegoProcessData_t *n)
{
    if (n->requesttype != 4) {
        n->requesttype = 4;
        n->prevreqtype = 4;
    }
    nbAddNegoIdKouho(n, 0x60046);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x42, 0);
    return 0;
}

s32 Flow6_11(nbNegoProcessData_t *n)
{
    if (n->requesttype != 3) {
        n->requesttype = 3;
        n->prevreqtype = 3;
    }
    nbAddNegoIdKouho(n, 0x60046);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x41, 0);
    return 0;
}

s32 Flow6_12(nbNegoProcessData_t *n)
{
    s32 plv;
    s32 elv;
    s32 ritu;

    if (n->requesttype != 5) {
        n->requesttype = 5;
        n->prevreqtype = 5;
    }
    n->requestdata = nbGetNegoYaruMakkaA(n, n->pwork, n->ework);
    nbAddNegoIdKouho(n, 0x6000D);
    nbAddNegoIdKouho(n, 0x6000E);
    plv = n->pwork->level;
    elv = n->ework->level;
    if (plv - 5 >= elv && plv - 9 < elv) {
        ritu = 0x14;
    } else if (plv - 0xA >= elv && plv - 0xE < elv) {
        ritu = 0x28;
    } else if (plv - 0xF >= elv && plv - 0x13 < elv) {
        ritu = 0x3C;
    } else if (plv - 0x14 >= elv && plv - 0x18 < elv) {
        ritu = 0x50;
    } else {
        ritu = 0x61;
        if (plv - 0x19 < elv) {
            ritu = 0;
        }
    }
    if ((s32)dds3GetRandIntA(0, 0x64) < ritu) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_13(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60010);
    nbAddNegoIdKouho(n, 0x6000E);
    n->nextnegoid = n->nextkouho[0];
    if (datCheckDying(n->ework) != 0) {
        if ((s32)dds3GetRandIntA(0, 100) < 70) {
            n->nextnegoid = n->nextkouho[1];
        }
    }
    return 0;
}

s32 Flow6_14(nbNegoProcessData_t *n)
{
    s32 mes;
    u32 prev;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x6000F);
        prev = nbGetPrevNegoId(n);
        switch (prev) {
        case 0x6000C:
            mes = 0x44;
            break;
        case 0x6000D:
            mes = 0x45;
            break;
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        break;
    }
    return 0;
}

s32 Flow6_15(nbNegoProcessData_t *n)
{
    nbDispNegoMessage(n, n->helpmsg, 1, 0);
    return 2;
}

s32 Flow6_16(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x43, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_17(nbNegoProcessData_t *n)
{
    if ((u32)n->requesttype >= 2) {
        if (dds3GetRandIntA(0, 2) == 0) {
            n->requesttype = 1;
        } else {
            n->requesttype = 0;
        }
        n->prevreqtype = n->requesttype;
    }
    if (n->requesttype == 1) {
        n->requestdata = nbGetNegoKureMakkaA(n, n->pwork, n->ework);
    }
    if (n->requesttype == 0) {
        n->requestdata = nbGetKureItemA(n, n->pwork, n->ework);
        if (n->requestdata == 0) {
            n->requestdata = 8;
        }
    }
    nbAddNegoIdKouho(n, 0x60014);
    nbAddNegoIdKouho(n, 0x60012);
    nbAddNegoIdKouho(n, 0x60013);
    switch (n->aisyo) {
    case 0:
        n->nextnegoid = n->nextkouho[0];
        break;
    case 1:
        n->nextnegoid = n->nextkouho[1];
        break;
    case 2:
        n->nextnegoid = n->nextkouho[2];
        break;
    }
    return 0;
}

s32 Flow6_18(nbNegoProcessData_t *n)
{
    if (n->requesttype == 1) {
        n->requestdata /= 2;
    }
    nbAddNegoIdKouho(n, 0x60014);
    return 0;
}

s32 Flow6_19(nbNegoProcessData_t *n)
{
    if (n->requesttype == 1) {
        n->requestdata *= 2;
    }
    nbAddNegoIdKouho(n, 0x60014);
    return 0;
}

s32 Flow6_20(nbNegoProcessData_t *n)
{
    s32 rounded;

    nbAddNegoIdKouho(n, 0x60046);
    if (n->requesttype == 1) {
        if (evtBitCheck(8) == 0) {
            n->requestdata /= 2;
            rounded = (s32)((u32)n->requestdata + 5) / 10 * 10;
            n->requestdata = rounded != 0 ? rounded : 1;
        }
        if (n->requesttype == 1) {
            if (evtBitCheck(0x1A) != 0 && evtBitCheck(0x16) == 0) {
                n->requestdata = (u32)n->requestdata * 8;
                n->requestdata /= 10;
                rounded = (s32)((u32)n->requestdata + 5) / 10 * 10;
                n->requestdata = rounded != 0 ? rounded : 1;
            }
            if (n->requesttype == 1) {
                nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3F, 0);
                nbMakaDispOn(0);
                goto complete;
            }
        }
    }
    nbSetMesItem(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x40, 0);
complete:
    return 0;
}

s32 Flow6_21(nbNegoProcessData_t *n)
{
    if (n->requesttype != 2) {
        n->requesttype = 2;
        n->prevreqtype = 2;
    }
    nbAddNegoIdKouho(n, 0x70001);
    n->workid[4] = 0x60018;
    n->workid[5] = 0x60041;
    if (n->cnt6_37 != 0) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x38, 0);
    } else {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3E, 0);
    }
    return 0;
}

extern char D_00545018[];
extern void nbMakaDispOff(void);

INCLUDE_RODATA(const s32, "newbattle/NbNegoFlowTwo", D_00545018);

s32 Flow6_70(nbNegoProcessData_t *n)
{
    u32 previous;

    switch (n->nownegophase) {
    case 0:
        switch (n->requesttype) {
        case 4:
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3C, 1);
            break;
        case 3:
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3B, 1);
            break;
        case 5:
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3D, 1);
            break;
        case 1:
            previous = nbGetPrevNegoId(n);
            switch (previous) {
            case 0x6003C:
            case 0x6003D:
            case 0x6003E:
            case 0x60040:
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x52, 1);
                break;
            default:
                nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x39, 1);
                break;
            }
            break;
        case 2:
            break;
        case 0:
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x3A, 1);
            break;
        }
        break;
    case 1:
        printf(D_00545018, n->nowselect);
        nbAddNegoIdKouho(n, 0x60030);
        switch (n->requesttype) {
        case 4:
            nbAddNegoIdKouho(n, 0x80001);
            break;
        case 3:
            nbAddNegoIdKouho(n, 0xB0001);
            break;
        case 1:
        case 5:
            nbAddNegoIdKouho(n, 0x60016);
            break;
        case 2:
            break;
        case 0:
            nbAddNegoIdKouho(n, 0x60016);
            break;
        }
        if (n->nowselect == 0) {
            n->nextnegoid = n->nextkouho[1];
        } else {
            n->nextnegoid = n->nextkouho[0];
        }
        nbMakaDispOff();
        break;
    }
    return 0;
}

#ifdef NON_MATCHING
extern s32 datCheckMaka(s32 maka);
#endif

s32 Flow6_22(nbNegoProcessData_t *n)
{
    switch (n->requesttype) {
    case 5:
        nbAddNegoIdKouho(n, 0x80001);
        nbAddNegoIdKouho(n, 0x60017);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60018);
        nbAddNegoIdKouho(n, 0x60017);
        break;
    case 0:
        nbAddNegoIdKouho(n, 0x60018);
        nbAddNegoIdKouho(n, 0x60017);
        break;
    }
    switch (n->requesttype) {
    case 1:
        if (datCheckMaka(n->requestdata) != 0) {
            n->nextnegoid = n->nextkouho[0];
            datAddMaka(-n->requestdata);
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        break;
    case 5:
        if (datCheckMaka(n->requestdata) != 0) {
            n->nextnegoid = n->nextkouho[0];
            datAddMaka(-n->requestdata);
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        break;
    case 0:
        if (datCheckItem(n->requestdata, 1) != 0) {
            n->nextnegoid = n->nextkouho[0];
            datAddItem(n->requestdata, -1);
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        break;
    }
    return 0;
}

s32 Flow6_23(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x60041);
        n->workid[2] = 0x60045;
        switch (n->requesttype) {
        case 0:
            mes = 0x51;
            break;
        case 1:
            mes = 0x50;
            break;
        case 5:
            mes = 0x50;
            break;
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
        break;
    }
    return 0;
}

s32 Flow6_24(nbNegoProcessData_t *n)
{
    s32 rate[4];
    s32 race, enemyParam;
    s32 playerParam;
    s32 playerLevel, enemyLevel;
    s32 choice, sum, roll, i, count;
    u16 enemyId;

    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60019);
    nbAddNegoIdKouho(n, 0x60025);
    nbAddNegoIdKouho(n, 0x6002B);
    nbAddNegoIdKouho(n, 0xA000D);
    nbAddNegoIdKouho(n, 0x6004A);
    n->cnt6_36 = (u32)n->cnt6_36 + 1;
    for (i = 0; i < 4; i++) {
        rate[i] = D_003E1338[n->negoskillid - 0x180].rate[i];
    }
    race = datGetRace(n->ework);
    if (race != 0x13) {
        switch (n->aisyo) {
        case 1:
            rate[3] -= 10;
            rate[2] -= 10;
            rate[0] += 20;
            break;
        case 2:
            rate[3] += 10;
            rate[2] += 10;
            rate[0] -= 20;
            break;
        }
    } else {
        switch (n->aisyo) {
        case 1:
            rate[3] -= 5;
            rate[2] -= 5;
            rate[0] += 10;
            break;
        case 2:
            rate[3] += 10;
            rate[2] += 10;
            rate[0] -= 20;
            break;
        }
    }
    if (datGetRace(n->ework) != 0x13) {
        playerParam = datGetParam(n->pwork, 1);
        enemyParam = datGetParam(n->ework, 1);
        if (enemyParam + 5 < playerParam) {
            rate[3] -= 5;
            rate[2] -= 5;
            rate[0] += 10;
        } else if (playerParam < enemyParam - 5) {
            rate[3] += 5;
            rate[2] += 5;
            rate[0] -= 10;
        }
    }
    if (n->negotype == 3) {
        rate[0] = 70;
        rate[1] = 20;
        rate[2] = 5;
        rate[3] = 5;
    }
    if (n->cnt6_37 == 1) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    if (n->requesttype == 2) {
        rate[2] += rate[3];
        rate[3] = 0;
    }
    count = n->cnt6_36;
    if (count < 2) {
        if (evtBitCheck(8) != 0) {
            n->nextnegoid = n->nextkouho[2];
            return 0;
        }
        count = n->cnt6_36;
        if (count < 2 && n->negoreqtype == 0) {
            n->nextnegoid = n->nextkouho[2];
            return 0;
        }
    }
    playerLevel = n->pwork->level;
    enemyLevel = n->ework->level;
    if (count < 3) {
        if (evtBitCheck(8) != 0) {
            if (enemyLevel >= playerLevel - 10) {
                s32 adjustedRate = rate[2] + rate[0];
                rate[0] = 0;
                count = n->cnt6_36;
                rate[2] = adjustedRate;
            } else {
                count = n->cnt6_36;
            }
        } else {
            count = n->cnt6_36;
        }
    }
    if (count >= 4) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    if (n->negoreqtype != 0 && count >= 2) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    enemyId = n->ework->id;
    if ((enemyId == 0x5C || enemyId == 0x3D || enemyId == 0x61 || enemyId == 0x5B) &&
        evtBitCheck(8) == 0 && n->negoreqtype == 0 && n->cnt6_36 >= 2) {
        rate[0] = rate[0] + rate[3] + rate[2];
        rate[3] = 0;
        rate[2] = 0;
    }
    /* ee-gcc otherwise allocates these reloads and their comparison differently.
     * These local bindings and the instruction-free input constraint preserve
     * native allocation; they are reconstruction aids, not original spelling. */
    {
        register datUnitWork_t *playerWork asm("$3") = n->pwork;
        register s32 enemyReload asm("$18");
        register s32 playerReload asm("$16");
        enemyReload = n->ework->level;
        playerReload = playerWork->level;
        asm("" : : "r" (playerReload), "r" (enemyReload));
        enemyLevel = enemyReload;
        playerLevel = playerReload;
    }
    {
        register s32 belowThreshold asm("$2") = playerLevel - 17 < enemyLevel;
        if (!belowThreshold && dds3GetRandIntA(0, 100) < 4) {
            rate[2] += rate[3];
            rate[3] = 0;
        }
    }
    roll = dds3GetRandIntA(0, 100);
    sum = 0;
    choice = 0;
    while (choice < 4 && sum + rate[choice] < roll) {
        sum += rate[choice];
        choice++;
    }
    if (choice == 4) {
        choice = 2;
    }
    n->nextnegoid = n->nextkouho[choice];
    if (choice == 3 && n->cnt6_36 >= 2 && n->negoreqtype == 0 && dds3GetRandIntA(0, 10) < 6) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x63, 0);
        n->nextnegoid = n->nextkouho[4];
        return 0;
    }
    return 0;
}

s32 Flow6_25(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, 0x6001A);
    do {
        mes = dds3GetRandIntA(0, 4) + 0x25;
    } while (mes == n->mes6_25);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    n->mes6_25 = mes;
    return 0;
}

s32 Flow6_26(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001B);
    nbAddNegoIdKouho(n, 0x6001C);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_27(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0x6001C);
    n->workid[0] = 0x6001D;
    n->workid[1] = 0x60021;
    r = nbSearchHojoSkillWork(n, 3);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_28(nbNegoProcessData_t *n)
{
    s32 count;
    s32 playerLevel;
    s32 enemyLevel;

    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60024);

    if (n->cnt6_36 < 2) {
        if (evtBitCheck(8) != 0) {
            n->nextnegoid = n->nextkouho[1];
            return 0;
        }
    }

    count = n->cnt6_36;
    playerLevel = n->pwork->level;
    enemyLevel = n->ework->level;
    if (count == 3) {
        if (evtBitCheck(8) != 0 && !(playerLevel - 17 < enemyLevel)) {
            n->nextnegoid = n->nextkouho[0];
            return 0;
        }
    }

    count = n->cnt6_36;
    playerLevel = n->pwork->level;
    enemyLevel = n->ework->level;
    if (count < 3) {
        if (evtBitCheck(8) != 0 && !(enemyLevel < playerLevel - 10)) {
            n->nextnegoid = n->nextkouho[1];
            return 0;
        }
    }

    if ((u32)dds3GetRandIntA(NULL, 10) >= 2) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_29(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001E);
    nbAddNegoIdKouho(n, 0x60020);
    if (n->hojoaisyo == 1) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_30(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6001F);
    nbDispNegoMessage(n, n->aisyomsg[n->ekucyo], n->hojoaisyomes, 0);
    return 0;
}

s32 Flow6_31(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60023);
    if (dds3GetRandIntA(0, 10) != 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_32(nbNegoProcessData_t *n)
{
    /* The qualified --jump trace shows one nextId store shared by the random and
     * unmatched-skill routes, while message success stores next[0] separately.
     * This forward branch supplies that native completion shape (SMT3 matching
     * policy in docs/CONTRIBUTING.md: one forward branch with trace evidence). */
    u32 nextId;

    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60022);
    if (!((u32)dds3GetRandIntA(NULL, 0xA) < 2)) {
        if (n->hojoskillid == 0x1A2) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x6E, 0);
        } else if (n->hojoskillid == 0x1A4) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x6F, 0);
        } else if (n->hojoskillid == 0x19D) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x70, 0);
        } else if (n->hojoskillid == 0x19E) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x71, 0);
        } else if (n->hojoskillid == 0x19F) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x72, 0);
        } else if (n->hojoskillid == 0x1A3) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x73, 0);
        } else {
            nextId = n->nextkouho[0];
            goto storeNextId;
        }
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    nextId = n->nextkouho[1];
storeNextId:
    n->nextnegoid = nextId;
    return 0;
}

s32 Flow6_33(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x90001);
    nbAddNegoIdKouho(n, 0x60022);
    nbAddNegoIdKouho(n, 0x60026);
    if (n->hojoskillid == 0x19D &&
        (n->ework->id == 0x3A || n->ework->id == 0x3C || n->ework->id == 0x3D ||
         n->ework->id == 0x5B || n->ework->id == 0x5A || n->ework->id == 0x5C)) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x8E, 0);
        n->nextnegoid = n->nextkouho[2];
        return 0;
    }
    /* Retail retains the dead sltiu at 0x00270D68 after these identical
     * random-selection arms fold. Flow6_80 shows the live selection form. */
    if ((u32)dds3GetRandIntA(0, 0xA) < 5) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_34(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60026);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x47, 0);
    return 0;
}

s32 Flow6_35(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60027;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x114, 0);
    return 0;
}

s32 Flow6_36(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60026);
    nbAddNegoIdKouho(n, 0x90001);
    if (n->ework->id == 0x5C || n->ework->id == 0x3D ||
        n->ework->id == 0x61 || n->ework->id == 0x5B) {
        if (evtBitCheck(8) == 0 && n->negoreqtype == 0 && n->cnt6_36 >= 3) {
            n->nextnegoid = n->nextkouho[1];
            return 0;
        }
    }
    n->nextnegoid = n->nextkouho[0];
    return 0;
}

/* NON_MATCHING: 9 of 67 words differ (first at +0x9C). */
#ifdef NON_MATCHING
s32 Flow6_37(nbNegoProcessData_t *n)
{
    s32 plevel;
    s32 elevel;

    nbAddNegoIdKouho(n, 0x60026);
    nbAddNegoIdKouho(n, 0x90001);
    if (n->ework->id == 0x5C || n->ework->id == 0x3D ||
        n->ework->id == 0x61 || n->ework->id == 0x5B) {
        if (evtBitCheck(8) == 0 && n->negoreqtype == 0 && n->cnt6_36 >= 3) {
            n->nextnegoid = n->nextkouho[1];
            return 0;
        }
    }
    n->nextnegoid = n->nextkouho[0];
    plevel = n->pwork->level;
    elevel = n->ework->level;
    if (n->cnt6_36 == 3) {
        if (evtBitCheck(8) != 0) {
            if (!(plevel - 0x11 < elevel)) {
                n->nextnegoid = n->nextkouho[1];
                return 0;
            }
        }
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowTwo", Flow6_37);
#endif

s32 Flow6_38(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60027;
    if (n->cnt6_36 == 1) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5D, 0);
        return 0;
    }
    if (n->cnt6_36 == 2) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5E, 0);
        return 0;
    }
    if (n->cnt6_36 == 3) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5F, 0);
        return 0;
    }
    if (n->cnt6_36 >= 4) {
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x60, 0);
        return 0;
    }
    return 0;
}

extern s32 nbGetNegoTuikaRequestType(s32 tuikareqtype);
extern s32 nbGetKureItemB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);
extern s32 nbGetNegoKureMakkaB(nbNegoProcessData_t *n, datUnitWork_t *a, datUnitWork_t *b);

s32 Flow6_39(nbNegoProcessData_t *n)
{
    s32 requestType;

    nbAddNegoIdKouho(n, 0x60028);
    if (n->ework->id == 0x5C && evtBitCheck(8) == 0 && n->negoreqtype == 0) {
        if (dds3GetRandIntA(NULL, 1) == 0) n->requesttype = 3;
    } else {
        requestType = nbGetNegoTuikaRequestType(master_text[n->ework->id].tuikareqtype);
        switch (requestType) {
        case 0:
            n->requesttype = 1;
            break;
        case 1:
            n->requesttype = 0;
            break;
        case 2:
            n->requesttype = 3;
            break;
        }
    }

    n->requestdata = 0;
    if (n->negoreqtype == 4) {
        n->requesttype = 0;
    }

    if (n->prevreqtype != 3 && n->requesttype == 3) {
        if (dds3GetRandIntA(NULL, 2) == 0) n->requesttype = 1;
        else n->requesttype = 0;
    }
    switch (n->requesttype) {
    case 0:
        n->requestdata = nbGetKureItemB(n, n->pwork, n->ework);
        if (n->requestdata == 0) n->requestdata = 0x10;
        break;
    case 1:
        n->requestdata = nbGetNegoKureMakkaB(n, n->pwork, n->ework);
        break;
    case 3:
        break;
    }

    n->prevreqtype = n->requesttype;
    return 0;
}

s32 Flow6_40(nbNegoProcessData_t *n)
{
    s32 mes;

    mes = 0;
    nbAddNegoIdKouho(n, 0x60029);
    switch (n->requesttype) {
    case 1:
        mes = 0x4A;
        nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
        nbMakaDispOn(0);
        break;
    case 3:
        mes = 0x4C;
        break;
    case 0:
        mes = 0x4B;
        nbSetMesItem(n->flowmsg[n->ekucyo], n->requestdata);
        break;
    }
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    return 0;
}

s32 Flow6_41(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6002A);
    return 0;
}

s32 Flow6_42(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x48, 1);
        break;
    case 1:
        if (n->requesttype != 3) {
            nbAddNegoIdKouho(n, 0x60016);
            nbAddNegoIdKouho(n, 0x60041);
        } else {
            nbAddNegoIdKouho(n, 0xB0001);
            nbAddNegoIdKouho(n, 0x60041);
        }
        n->workid[2] = 0x60027;
        if (n->nowselect == 0) {
            n->nextnegoid = n->nextkouho[0];
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
        nbMakaDispOff();
        break;
    }
    return 0;
}

/* NON_MATCHING: 23 of 68 words differ (first at +0x28). */
#ifdef NON_MATCHING
s32 Flow6_43(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x4E, 0);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x6002C);
        nbAddNegoIdKouho(n, 0x90001);
        if (n->data->enemypcnt == 1) {
            if (n->cnt6_36 >= 4) {
                if ((u32)dds3GetRandIntA(0, 0xA) < 6) {
                    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x61, 0);
                    n->nextnegoid = n->nextkouho[1];
                    break;
                }
            }
        }
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x62, 0);
        n->nextnegoid = n->nextkouho[0];
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowTwo", Flow6_43);
#endif

s32 Flow6_44(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA0017);
    nbAddNegoIdKouho(n, 0x6002D);
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_45(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0xA0017);
    nbAddNegoIdKouho(n, 0x60034);
    n->workid[0] = 0x6002E;
    n->workid[1] = 0x6002F;
    r = nbSearchHojoSkillWork(n, 2);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_46(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x60009;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x4F, 0);
    n->cnt6_36 = 0;
    n->cnt6_37 = 0;
    return 0;
}

s32 Flow6_47(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0xA001D);
    nbDispNegoMessage(n, n->helpmsg2, 9, 0);
    n->waitframe = 60;
    return 0;
}

s32 Flow6_48(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60031);
    nbAddNegoIdKouho(n, 0xA0001);
    if (n->cnt6_58 >= 3) {
        n->nextnegoid = n->nextkouho[1];
    } else {
        n->nextnegoid = n->nextkouho[0];
    }
    return 0;
}

s32 Flow6_49(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60032);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    if (n->requesttype == 1) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_50(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60033);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    if (nbGetHojoSkillWorkCnt(n) > 0) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_51(nbNegoProcessData_t *n)
{
    s32 r;

    nbAddNegoIdKouho(n, 0x60034);
    nbAddNegoIdKouho(n, 0x60041);
    n->workid[2] = 0x60045;
    n->workid[0] = 0x6003A;
    n->workid[1] = 0x6003F;
    r = nbSearchHojoSkillWork(n, 0);
    if (r != 0) {
        n->hojopartyindex = (u32)r >> 16;
        n->hojoskillid = r & 0xFFFF;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_52(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60035);
    nbClsNegoMessage(n);
    n->hojoseikou = 0;
    n->hojoaisyo = 0;
    return 0;
}

extern void nbCameraSetHojo(nbFormation_t *hojo, nbFormation_t *target1,
                            nbFormation_t *target2);
extern void nbDispSkillName(s32 id);
extern char D_00545068[];

INCLUDE_RODATA(const s32, "newbattle/NbNegoFlowTwo", D_00545068);

s32 Flow6_53(nbNegoProcessData_t *n)
{
    s32 message;
    s32 skill;
    nbFormation_t *form;

    message = 0;
    skill = n->hojoskillid;
    form = &n->data->form[n->data->party[n->hojopartyindex].formindex];

    switch (skill) {
    case 0x199:
        message = 0xC;
        break;
    case 0x19A:
        message = 0xD;
        break;
    case 0x1A2:
        message = 0xE;
        break;
    case 0x19B:
        message = 0xF;
        break;
    case 0x19C:
        message = 0x10;
        break;
    case 0x1A4:
        message = 0x11;
        break;
    case 0x19D:
        message = 0x12;
        break;
    case 0x19E:
        message = 0x13;
        break;
    case 0x19F:
        message = 0x14;
        break;
    case 0x1A3:
        message = 0x15;
        break;
    }

    switch (n->nownegophase) {
    case 0:
        nbCameraSetHojo(form, n->pform, n->eform);
        nbDispNegoMessage(n, n->helpmsg, message, 0);
        break;
    case 1:
        nbClsNegoMessage(n);
        nbCameraSetTaizi(n->pform, n->eform);
        nbMakeNegoBackPacket(0, (s32)n->actdata, n->pform, n->eform);
        nbFormDispOff2(form);
        nbMakeNegoWorkPosPacket(0, (s32)n->actdata, form, n->eform);
        n->waitframe = 10;
        break;
    case 2:
        nbFormDispOff2(n->pform);
        nbMakeUnitFadeInPacket(1, (s32)n->actdata, n->pform, 0, 0, 0, 0);
        nbMakeNegoForwardPacket(0, (s32)n->actdata, form, n->eform);
        n->stat |= 8;
        nbFormDispOn2(form);
        nbCameraSetTaizi(form, n->eform);
        n->waitframe = 5;
        break;
    case 3:
        nbDispSkillName(skill);
        printf(D_00545068, n->hojopartyindex, n->hojoskillid);
        n->waitframe = 0x3C;
        break;
    case 4:
        nbAddNegoIdKouho(n, 0x60036);
        break;
    }
    return 0;
}

s32 Flow6_74(nbNegoProcessData_t *n)
{
    nbFormation_t *form;

    if ((n->stat & 8) == 0) {
        nbAddNegoIdKouho(n, n->workid[6]);
        return 0;
    }

    form = &n->data->form[n->data->party[n->hojopartyindex].formindex];
    switch (n->nownegophase) {
    case 0:
        nbMakeNegoBackPacket(0, (s32)n->actdata, form, n->eform);
        nbFormDispOff2(n->pform);
        nbMakeNegoWorkPosPacket(0, (s32)n->actdata, n->pform, n->eform);
        nbCameraSetTaizi(n->pform, n->eform);
        n->waitframe = 10;
        break;
    case 1:
        nbFormDispOff2(form);
        nbMakeUnitFadeInPacket(1, (s32)n->actdata, form, 0, 0, 0, 0);
        nbAddHomeMoveKoukaPacket(0, 0, n->hojopartyindex, 1.0f);
        nbFormDispOn2(n->pform);
        nbMakeNegoForwardPacket(0, (s32)n->actdata, n->pform, n->eform);
        n->waitframe = 10;
        break;
    case 2:
        nbAddNegoIdKouho(n, n->workid[6]);
        n->stat &= ~8;
        break;
    }
    return 0;
}



s32 Flow6_54(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;

    nbAddNegoIdKouho(n, 0x60038);
    nbAddNegoIdKouho(n, 0x60037);
    n->nextnegoid = n->nextkouho[0];
    work = nbGetUnitWorkPointer(n->data, &n->data->party[n->hojopartyindex]);
    if (n->hojoskillid == 0x1A2 && work->id == 0x14) {
        n->temp_param[0] = 2;
        n->nextnegoid = n->nextkouho[1];
        n->temp_param[1] = 0x58;
    }
    if (n->hojoskillid == 0x1A3 && work->id == 0xF) {
        n->nextnegoid = n->nextkouho[1];
        n->temp_param[0] = 6;
        n->temp_param[1] = 0x59;
    }
    return 0;
}

s32 Flow6_55(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60038);
    nbDispNegoMessage(n, n->flowmsg[n->temp_param[0]], n->temp_param[1], 0);
    return 0;
}

#ifdef NON_MATCHING
s32 Flow6_56(nbNegoProcessData_t *n)
{
    datUnitWork_t *work;
    nbNegoSkillCondition_t *condition;
    s32 skill;
    s32 ptype;
    s32 etype;
    s32 i;

    nbAddNegoIdKouho(n, 0x60039);
    skill = n->hojoskillid;
    if ((skill < 0x19C || skill >= 0x1A2) &&
        (skill < 0x1A3 || skill >= 0x1A5)) {
        n->hojoaisyo = 0;
    } else {
        work = nbGetUnitWorkPointer(n->data, &n->data->party[n->hojopartyindex]);
        condition = D_003E1338[n->hojoskillid - 0x180].condition;
        for (i = 0; i < 4; i++, condition++) {
            ptype = condition->ptype;
            etype = condition->etype;
            if ((ptype != 0 || etype != 0) &&
                nbFlowCheckUnit2(work, n->ework, ptype, etype, 1)) {
                n->hojoaisyo = 1;
                n->hojoaisyomes = condition->message;
                break;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowTwo", Flow6_56);
#endif

s32 Flow6_57(nbNegoProcessData_t *n)
{
    s32 ritu;
    s32 elv;
    s32 plv;

    ritu = 0x46;
    nbAddNegoIdKouho(n, n->workid[0]);
    nbAddNegoIdKouho(n, n->workid[1]);
    if (n->hojoskillid == 0x1A2) {
        ritu = 0x55;
    }
    elv = datGetParam(n->ework, 1);
    plv = datGetParam(n->pwork, 1);
    if (elv + 5 < plv) {
        ritu += 0xA;
    } else if (plv < elv - 5) {
        ritu -= 0xA;
    }
    if (n->hojoskillid == 0x19D &&
        (n->ework->id == 0x3A || n->ework->id == 0x3C || n->ework->id == 0x3D ||
         n->ework->id == 0x5A || n->ework->id == 0x5B || n->ework->id == 0x5C)) {
        ritu = 0;
    }
    if ((s32)dds3GetRandIntA(0, 0x64) < ritu) {
        n->hojoseikou = 1;
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->hojoseikou = 0;
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_58(nbNegoProcessData_t *n)
{
    n->requestdata /= 2;
    n->cnt6_58++;
    nbAddNegoIdKouho(n, 0x6003B);
    return 0;
}

s32 Flow6_59(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60047);
    nbAddNegoIdKouho(n, 0x60048);
    nbAddNegoIdKouho(n, 0x60049);
    if (n->cnt6_58 < 4) {
        if (n->cnt6_58 > 0) {
            n->nextnegoid = n->nextkouho[n->cnt6_58 - 1];
        }
    }
    return 0;
}

s32 Flow6_71(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003C;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5A, 0);
    return 0;
}

s32 Flow6_60(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x55, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_72(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003D;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5B, 0);
    return 0;
}

s32 Flow6_61(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x56, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_73(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x6004A);
    n->workid[6] = 0x6003E;
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x5C, 0);
    return 0;
}

s32 Flow6_62(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x57, 0);
    nbMakaDispOn(0);
    return 0;
}

s32 Flow6_63(nbNegoProcessData_t *n)
{
    switch (n->nownegophase) {
    case 0:
        n->waitframe = nbSetNegoNoMotion(n, n->eform);
        break;
    case 1:
        nbAddNegoIdKouho(n, 0x6004A);
        n->workid[6] = 0x60040;
        nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x53, 0);
        break;
    }
    return 0;
}

s32 Flow6_64(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60046);
    nbSetMesMakka(n->flowmsg[n->ekucyo], n->requestdata);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x54, 0);
    nbMakaDispOn(0);
    return 0;
}

/* NON_MATCHING: 79 of 100 words differ (first at +0x10); four words long. */
#ifdef NON_MATCHING
s32 Flow6_65(nbNegoProcessData_t *n)
{
    s32 ritu;
    s32 ppar;
    s32 epar;

    ritu = 0x32;
    nbAddNegoIdKouho(n, 0xA0001);
    nbAddNegoIdKouho(n, 0x60042);
    nbAddNegoIdKouho(n, 0x90001);
    if (datGetRace(n->ework) != 0x13) {
        if (n->aisyo == 1) {
            ritu = 0x1E;
        } else if (n->aisyo == 2) {
            ritu = 0x46;
        }
    } else if (n->aisyo == 1) {
        ritu = 0x28;
    } else if (n->aisyo == 2) {
        ritu = 0x46;
    }
    if (datGetRace(n->ework) != 0x13) {
        ppar = datGetParam(n->pwork, 1);
        epar = datGetParam(n->ework, 1);
        if (epar + 5 < ppar) {
            ritu -= 5;
        } else if (ppar < epar - 5) {
            ritu += 5;
        }
    }
    if (n->cnt6_37 != 0) {
        n->nextnegoid = n->nextkouho[0];
        return 0;
    }
    if (n->cnt6_36 >= 4) {
        if ((u32)dds3GetRandIntA(0, 0xA) >= 3) {
            nbDispNegoMessage(n, n->flowmsg[n->ekucyo], 0x129, 0);
            n->nextnegoid = n->nextkouho[2];
        } else {
            n->nextnegoid = n->nextkouho[1];
        }
    } else {
        n->nextnegoid = (s32)dds3GetRandIntA(0, 0x64) < ritu ? n->nextkouho[0] : n->nextkouho[1];
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/NbNegoFlowTwo", Flow6_65);
#endif

s32 Flow6_66(nbNegoProcessData_t *n)
{
    n->cnt6_66++;
    nbAddNegoIdKouho(n, 0x60043);
    nbAddNegoIdKouho(n, 0xA0001);
    if (n->cnt6_66 < 4) {
        n->nextnegoid = n->nextkouho[0];
    } else {
        n->nextnegoid = n->nextkouho[1];
    }
    return 0;
}

s32 Flow6_67(nbNegoProcessData_t *n)
{
    s32 mes;

    nbAddNegoIdKouho(n, n->workid[2]);
    do {
        mes = dds3GetRandIntA(0, 4) + 0x21;
    } while (mes == n->mes6_67);
    nbDispNegoMessage(n, n->flowmsg[n->ekucyo], mes, 0);
    n->mes6_67 = mes;
    return 0;
}

s32 Flow6_69(nbNegoProcessData_t *n)
{
    nbAddNegoIdKouho(n, 0x60009);
    return 0;
}

s32 nbNegoFlowTwo(nbNegoProcessData_t *n)
{
    return D_004EF550[(u16)n->nownegoid](n);
}
