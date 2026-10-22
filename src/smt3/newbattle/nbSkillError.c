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
    s32 select;
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

extern void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype);

extern void nbPlaySe(s32 no);

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, void *buff, s32 leng);

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 unk_04;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *unk_14;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *unk_20;
    void *mem;
} nbEventProcessData_t;

extern s32 nbFindStockSkillError(void);

extern s32 evtBitCheck(s32 no);

extern char D_00545DC8[];

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern char D_00545DB0[];

extern char D_005464A8[];

extern char D_005464C0[];

extern void pacFileHandleFree_(void *pacHandle);

extern s32 pacLoadUseTail(char *filename);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 sprintf(char *buf, const char *format, ...);

extern s32 nbMakeModelPackName(s32 no);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern u16 D_003E7028[];

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void *memcpy(void *a, const void *b, u32 c);

extern s32 pacCheckEnd(nbPacHandle_t *pacHandle);

extern void nbAddFormEntry(void *buf, s32 group, s32 id);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

extern void nbLoadMidiFile(s32 no);

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

s32 nbGetMotionTable(s32 unitid, s32 index);

INCLUDE_ASM(const s32, "newbattle/nbSkillError", CheckHojo);

/* s32 with no return keeps the one shared epilogue retail has. */
s32 nbDispSkillErrorMessage(s32 err)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    switch (err) {
    case 2:
        nbDispMsgHelp(data->unk_130, 0x42, 3);
        break;
    case 4:
        nbDispMsgHelp(data->unk_130, 0x46, 3);
        break;
    }
    if (err > 0 && (err < 3 || err == 4)) {
        nbPlaySe(0xD);
    }
}

/* NON_MATCHING: 1 of 18 words differ (first at +0x14). */
#ifdef NON_MATCHING
s32 nbCheckBattleSkillError1(s32 nskill)
{
    if (nskill < 0x33) {
        if (nskill >= 0x31) {
            if (nbFindStockSkillError() == 0) {
                return 2;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbSkillError", nbCheckBattleSkillError1);
#endif

INCLUDE_ASM(const s32, "newbattle/nbSkillError", nbCheckBattleSkillError2);

INCLUDE_ASM(const s32, "newbattle/nbSkillError", nbCheckBattleSkillError3);

INCLUDE_RODATA(const s32, "newbattle/nbSkillError", D_00546138);

INCLUDE_ASM(const s32, "newbattle/nbSkillError", cmpClearButtonGuide);

void nbLoadDefaultModelPack(nbMainProcessData_t *data)
{
    if (data->encno == 0x14D && data->modelpack == NULL) {
        data->modelpack = (void *)nbMakeModelPackName(0xFF);
    }
}

s32 nbCheckEventPackLoaded(nbMainProcessData_t *data)
{
    if (data->encno != 0x14D) {
        return 1;
    }
    if (pacCheckEnd(data->modelpack) == 0) {
        return 0;
    }
    data->modelpackbuf = data->modelpack->entry->buf;
    pacFileHandleFree_(data->modelpack);
    data->modelpack = NULL;
    return 1;
}

void nbFreeEventPack(nbMainProcessData_t *data)
{
    if (data->modelpack != NULL) {
        pacFileHandleFree_(data->modelpack);
        data->modelpack = NULL;
    }
    if (data->modelpackbuf != NULL) {
        sdfMemFreeDirect(data->modelpackbuf);
        data->modelpackbuf = NULL;
    }
}

void nbSetModelHandleOnce(nbFormation_t *form, NB_FILEHANDLE_T *packh)
{
    if (form->packh == NULL) {
        form->packh = packh;
    }
}

s32 nbMakeModelPackName(s32 no)
{
    char name[0x40];
    s32 h;

    sprintf(name, D_005464A8, no);
    h = pacLoadUseTail(name);
    printf(D_005464C0, name);
    return h;
}

/* s32 with no return blocks the sibling call retail does not take. */
s32 nbLoadModelPackIfEnabled(nbMainProcessData_t *data, void *buf, s32 no)
{
    if (data->formpack == NULL) {
        if (evtBitCheck(0xC0F)) {
            data->formpack = (nbPacHandle_t *)nbMakeModelPackName(no);
        } else {
            mdlPrepare(buf, no, 0);
            nbLoadMidiFile(no);
        }
    }
}

void nbDecodeModelPack(void *data, s32 size, s32 group, s32 id)
{
    mdlPackDecode(group, id, 0x101, data, size);
}

void nbLoadMotionData(nbStatWork_t *stat, void *src, u32 size, s32 motion, s32 unitid)
{
    void **dst;
    void *p;
    s32 id;
    s32 i;
    s32 kind;

    if (stat != NULL) {
        if (evtBitCheck(0xC0F)) {
            id = D_003E7028[unitid];
        } else {
            id = unitid;
        }
        dst = stat->buf;
        for (i = 0; i < 26; i++) {
            kind = nbGetMotionTable(id, i);
            if (kind >= 0 && kind == motion) {
                p = sdfMemAllocDirect(size);
                memcpy(sdfMemLock(p), src, size);
                *dst = p;
            }
            dst++;
        }
    }
}

void nbCopyPackData(nbMainProcessData_t *data, void *src, u32 size)
{
    void *p;

    if (data != NULL) {
        p = sdfMemAllocDirect(size);
        memcpy(sdfMemLock(p), src, size);
        data->packdata = p;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbSkillError", nbAddFormEntry);

s32 nbCheckModelPackLoaded(nbMainProcessData_t *data, s32 group, s32 id)
{
    void *buf;

    if (evtBitCheck(0xC0F) != 0) {
        if (pacCheckEnd(data->formpack) == 0) {
            goto zero;
        }
        buf = data->formpack->entry->buf;
        nbAddFormEntry(sdfMemLock(buf), group, id);
        sdfMemFreeDirect(buf);
        pacFileHandleFree_(data->formpack);
        data->formpack = NULL;
        return 1;
    }
    if (mdlPrepare(group, id, 0) == 0) {
        goto zero;
    }
    if (mdlPrepare(group, id, 0) != -1) {
        return 1;
    }
zero:
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbSkillError", D_005464A8);

INCLUDE_RODATA(const s32, "newbattle/nbSkillError", D_005464C0);

INCLUDE_RODATA(const s32, "newbattle/nbSkillError", D_005464E0);

