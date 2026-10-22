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

extern void *memset(void *a, s32 b, u32 c);

extern char D_00545B78[];

extern void nbPlaySe(s32 no);

extern char D_00545B90[];

extern void p2sdPlaySE(s32 seno, s32 vol, s32 pan);

extern s32 nbLoadMotionByIndex(s32 unitid, s32 index);

extern s32 nbResultCount;

extern s32 bgmno;

extern void p2sdPlayBGM(s32 no);

extern s32 nbBgmSaved;

extern void p2sdSetBgmTrack(s32 track);

extern s32 nbBgmFadeState;

extern void p2sdStopBGM(s32 no);

extern void func_002C1E70(s32 no);

extern char D_00545CE0[];

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

extern u8 datModelParamTable[];

extern u8 D_003494B0[];

extern void nbFreeFormResources(nbMainProcessData_t *data, s32 index);

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern void p2sdLoadSMGFile(s32 MidiNo);

extern void nbSetMaeFlag(s32 flag);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbFindFormIndex(s32 id);

extern s32 nbMaeCheckDone(void);

extern void nbMaeWaitStep(void);

extern void sdfMemFreeDirect(void *aHandle);

extern void p2sdSetPcmBuffer(void *buf, u32 size);

extern void datDebugPrintf(const char *format);

extern char D_00545BC0[];

extern char D_00545BD0[];

extern char D_00545BE0[];

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 nbMaeState;

extern s32 nbMaeCounter;

extern s32 nbMaeFlag;

extern s32 D_00551540;

extern void nbBgmStop(void);

extern s32 nbFindFreeFormSlot(void);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 pacFileCheckEnd(void *pacFileHandle);

extern void *fileGetMemHandle(void *fileHandle);

extern void fileHandleFree(void *fileHandle);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern void nbLoadMidiFile(s32 no);

extern u32 sdfMemGetSize(void *aHandle);

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

s32 nbGetUnitWorkFromAction(nbFormation_t *form);

void nbPlaySe(s32 no)
{
    p2sdPlaySE(no, 0x7F, 0x3F);
}

s32 nbGetMotionTable(s32 unitid, s32 index)
{
    u8 *p;

    if (unitid == 0) {
        p = D_003494B0;
    } else {
        p = datModelParamTable + unitid * 784;
    }
    return *(s8 *)(p + index * 28 + 0x31);
}

INCLUDE_ASM(const s32, "newbattle/nbSound", nbLoadMotionByIndex);

s32 nbGetUnitFromAction(nbFormation_t *form, s32 index)
{
    return nbLoadMotionByIndex(nbGetUnitWorkFromAction(form), index);
}

s32 nbMakeSeId(s32 seno)
{
    return (seno + 0x500) << 16;
}

s32 nbPlayMotionSe(nbFormation_t *form)
{
    return nbMakeSeId(nbGetUnitWorkFromAction(form));
}

s32 nbMemoryLoadNegoSe(nbFormation_t *form)
{
    nbMainProcessData_t *data;
    void *buf;
    u32 size;
    s32 index;

    data = nbGetMainProcessData();
    if (p2sdLoadCheckMidi(nbPlayMotionSe(form)) != 0) {
        datDebugPrintf(D_00545BC0);
        return 1;
    }
    index = nbFindFormIndex(nbGetUnitWorkFromAction(form));
    if (index < 0) {
        return 0;
    }
    buf = data->statwork[index].motbuf;
    if (buf == NULL) {
        return 0;
    }
    size = sdfMemGetSize(buf);
    p2sdSetPcmBuffer(sdfMemLock(buf), size);
    return 1;
}

s32 nbMemoryLoadMotionSe(nbFormation_t *form, s32 mot)
{
    nbMainProcessData_t *data;
    s32 unitid;
    s32 formindex;
    void *buf;
    u32 size;

    data = nbGetMainProcessData();
    if (mot < 0) {
        return 0;
    }
    unitid = nbGetUnitFromAction(form, mot);
    if (unitid == 0) {
        return 0;
    }
    printf(D_00545BD0, unitid);
    if (p2sdLoadCheckMidi(unitid) != 0) {
        datDebugPrintf(D_00545BE0);
        return 1;
    }
    formindex = nbFindFormIndex(nbGetUnitWorkFromAction(form));
    if (formindex < 0) {
        return 0;
    }
    buf = data->statwork[formindex].buf[mot];
    if (buf == NULL) {
        return 0;
    }
    size = sdfMemGetSize(buf);
    p2sdSetPcmBuffer(sdfMemLock(buf), size);
    data->stat |= 0x200;
    return 1;
}

void nbGetFormEntry(s32 id)
{
    nbMainProcessData_t *data;
    s32 index;

    data = nbGetMainProcessData();
    index = nbFindFormIndex(id);
    if (index >= 0) {
        data->statwork[index].refcnt--;
        if (data->statwork[index].refcnt == 0) {
            nbFreeFormResources(data, index);
            data = NULL;
        }
    }
}

nbStatWork_t *nbSetFormEntry(s32 id)
{
    nbMainProcessData_t *data;
    nbStatWork_t *stat;
    s32 index;

    data = nbGetMainProcessData();
    index = nbFindFormIndex(id);
    if (index >= 0) {
        data->statwork[index].refcnt++;
        return NULL;
    }
    stat = &data->statwork[nbFindFreeFormSlot()];
    stat->id = 1;
    stat->unk_02 = id;
    stat->refcnt = 1;
    return stat;
}

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545BC0);

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545BD0);

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545BE0);

INCLUDE_ASM(const s32, "newbattle/nbSound", nbLoadMidiFile);

/* NON_MATCHING: 2 of 64 words differ (first at +0x74). */
#ifdef NON_MATCHING
void nbCheckPackLoad(nbMainProcessData_t *data)
{
    nbStatWork_t *stat;
    s32 i;
    s32 j;
    s32 ofs;

    ofs = 0;
    for (i = 9; i >= 0; i--) {
        stat = (nbStatWork_t *)((s32)data->statwork + ofs);
        if (stat->id != 0) {
            for (j = 0; j < 0x1A; j++) {
                if (stat->file[j] != NULL) {
                    if (pacFileCheckEnd(stat->file[j]) != 0) {
                        stat->buf[j] = fileGetMemHandle(stat->file[j]);
                        fileHandleFree(stat->file[j]);
                        stat->file[j] = NULL;
                    }
                }
            }
            if (stat->motfile != NULL) {
                if (pacFileCheckEnd(stat->motfile) != 0) {
                    stat->motbuf = fileGetMemHandle(stat->motfile);
                    fileHandleFree(stat->motfile);
                    stat->motfile = NULL;
                }
            }
        }
        ofs += sizeof(nbStatWork_t);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbSound", nbCheckPackLoad);
#endif

s32 nbFindFreeFormSlot(void)
{
    nbStatWork_t *stat;
    s32 i;

    stat = nbGetMainProcessData()->statwork;
    for (i = 0; i < 10; i++) {
        if (stat[i].id == 0) {
            return i;
        }
    }
    return -1;
}

s32 nbFindFormIndex(s32 id)
{
    nbStatWork_t *stat;
    s32 i;

    stat = nbGetMainProcessData()->statwork;
    for (i = 0; i < 10; i++) {
        if (stat->id != 0 && stat->unk_02 == id) {
            return i;
        }
        stat++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbSound", nbFreeFormResources);

void nbFreeAllFormResources(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        nbFreeFormResources(data, i);
    }
    sdfMemFreeDirect(data->statbuf);
    data->statwork = NULL;
    data->statbuf = NULL;
}

void nbAllocMainProcessData(nbMainProcessData_t *data)
{
    void *p;

    p = sdfMemAllocDirect(0x8C0);
    data->statbuf = p;
    data->statwork = sdfMemLock(p);
    memset(data->statwork, 0, 0x8C0);
    if (p2sdLoadCheckMidi(0x10000) == 0) {
        p2sdLoadSMGFile(0x10000);
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: exact on its own; its presence turns nbE905_End's bnel into bne. */
s32 nbMaeCheckDone()
{
    if (D_00551540 == 1) {
        return 1;
    }
    switch (nbMaeFlag) {
    case 0:
        break;
    case 1:
        if (p2sdLoadCheckMidi(nbMaeState) != 1) {
            return 0;
        }
        nbMaeFlag++;
        break;
    case 2:
        nbMaeFlag++;
        break;
    case 3:
        if (p2sdLoadCheckMidi(nbMaeCounter) != 0) {
            nbMaeFlag = 5;
        } else {
            p2sdLoadSMGFile(nbMaeCounter);
            nbMaeFlag++;
        }
        break;
    case 4:
        if (p2sdLoadCheckMidi(nbMaeCounter) != 1) {
            return 0;
        }
        nbMaeFlag++;
        break;
    case 5:
        nbMaeFlag = 0;
        D_00551540 = 1;
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbSound", nbMaeCheckDone);
#endif

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545C38);

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545C58);

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545C78);

INCLUDE_ASM(const s32, "newbattle/nbSound", nbSetMaeFlag);

void nbIncResultCounter(void)
{
    nbResultCount++;
}

void nbMaeWaitStep(void)
{
    p2sdPlayBGM(bgmno);
}

void nbRestoreBgm(void)
{
    if (nbBgmSaved > 0) {
        p2sdSetBgmTrack(nbBgmSaved);
        nbBgmSaved = 0;
    }
}

void nbBgmFadeOut(void)
{
    p2sdStopBGM(bgmno);
    p2sdStopBGM(nbBgmFadeState);
}

void nbBgmStop(void)
{
    printf(D_00545CE0, nbBgmSaved, nbBgmFadeState);
    p2sdStopBGM(bgmno);
    p2sdStopBGM(nbBgmFadeState);
    p2sdStopBGM(0x2B0000);
}

void nb_28DE10(void)
{
    func_002C1E70(0x7F);
}

void nb_28DE28(void)
{
    func_002C1E70(0);
}

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545CE0);

INCLUDE_RODATA(const s32, "newbattle/nbSound", D_00545CF8);

INCLUDE_SDATA(const s32, "newbattle/nbSound", bgmno);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbBgmSaved);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbBgmFadeState);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbResultCount);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbMaeState);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbMaeCounter);

INCLUDE_SDATA(const s32, "newbattle/nbSound", nbMaeFlag);

INCLUDE_SDATA(const s32, "newbattle/nbSound", D_00551540);

