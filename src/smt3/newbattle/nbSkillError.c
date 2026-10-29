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
    u8 unk_A8[0x14];
    dds3ProcessID_t *MainProcessID;
    u8 unk_C0[0x34];
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

#include "sdk_libc.h"

extern nbMainProcessData_t *nbGetMainProcessData(void);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern nbParty_t *nbGetPartyFromFormindex(s32 formindex);

extern void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype);

extern void nbPlaySe(s32 no);

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, void *buff, s32 leng);



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

extern s32 nbLoadMidiFile(s32 no);

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

extern s32 nbGetHojoCounterMax(s32 type);
extern s32 nbGetHojoCounterMin(s32 type);

s32 CheckHojo(nbParty_t **list, s32 cnt, u32 hojo)
{
    s32 i;

    for (i = 0; i < cnt; i++) {
        nbParty_t *party = list[i];

        if ((hojo & 1) != 0 && party->count[4] != nbGetHojoCounterMax(4)) {
            return 1;
        }
        if ((hojo & 2) != 0 && party->count[4] != nbGetHojoCounterMin(4)) {
            return 1;
        }
        if ((hojo & 4) != 0 && party->count[5] != nbGetHojoCounterMax(5)) {
            return 1;
        }
        if ((hojo & 8) != 0 && party->count[5] != nbGetHojoCounterMin(5)) {
            return 1;
        }
        if ((hojo & 0x10) != 0 && party->count[6] != nbGetHojoCounterMax(6)) {
            return 1;
        }
        if ((hojo & 0x20) != 0 && party->count[6] != nbGetHojoCounterMin(6)) {
            return 1;
        }
        if ((hojo & 0x40) != 0 && party->count[7] != nbGetHojoCounterMax(7)) {
            return 1;
        }
        if ((hojo & 0x80) != 0 && party->count[7] != nbGetHojoCounterMin(7)) {
            return 1;
        }
        if ((hojo & 0x100) != 0 && party->count[8] != nbGetHojoCounterMax(8)) {
            return 1;
        }
        if ((hojo & 0x200) != 0 && party->count[8] != nbGetHojoCounterMin(8)) {
            return 1;
        }
        if ((hojo & 0x1000) != 0 && party->count[9] != nbGetHojoCounterMax(9)) {
            return 1;
        }
        if ((hojo & 0x2000) != 0 && party->count[10] != nbGetHojoCounterMax(10)) {
            return 1;
        }
        if ((hojo & 0x4000) != 0 && party->count[11] != nbGetHojoCounterMax(11)) {
            return 1;
        }
        if ((hojo & 0x8000) != 0 && party->count[12] != nbGetHojoCounterMax(12)) {
            return 1;
        }
        if ((hojo & 0x10000) != 0 && party->count[13] != nbGetHojoCounterMax(13)) {
            return 1;
        }
        if ((hojo & 0x20000) != 0 && party->count[14] != nbGetHojoCounterMax(14)) {
            return 1;
        }
        if ((hojo & 0x1000000) != 0 && party->count[15] != nbGetHojoCounterMax(15)) {
            return 1;
        }
    }
    return 0;
}

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

extern s32 cmpAnyButtonGuideActive(datUnitWork_t **list, s32 count);
extern s32 cmpButtonGuideShowDone(datUnitWork_t **list, s32 count);
extern s32 cmpButtonGuideHideDone(datUnitWork_t **list, s32 count);
extern s32 cmpFindButtonGuideByFlag(datUnitWork_t **list, s32 count, s32 mask);

#ifdef NON_MATCHING
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern nbParty_t *nbGetPartyFromFormindex(s32 formindex);
#endif

s32 nbCheckBattleSkillError2(s32 nskill, u32 select)
{
    datUnitWork_t *unitList[32];
    nbParty_t *partyList[32];
    nbMainProcessData_t *data;
    datUnitWork_t *unit;
    nbParty_t *party;
    s32 unitCount;
    s32 partyCount;
    s32 i;
    s32 result = 0;

    data = nbGetMainProcessData();
    if ((data->stat & 0x08000000) != 0) {
        return 0;
    }
    unitCount = 0;
    partyCount = 0;
    for (i = 0; i < 16; i++) {
        if ((select & (1 << i)) != 0) {
            unit = nbGetUnitWorkFromFormindex(i);
            if (unit != NULL) {
                unitList[unitCount++] = unit;
            }
            party = nbGetPartyFromFormindex(i);
            if (party != NULL) {
                partyList[partyCount++] = party;
            }
        }
    }
    if (unitCount == 0) {
        return 0;
    }

    switch (nskill) {
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 81:
    case 94:
        if (cmpButtonGuideShowDone(unitList, unitCount) != 0) {
            result = 1;
        }
        break;
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 88:
        if (cmpFindButtonGuideByFlag(unitList, unitCount,
                                     datSkillTable.normal[nskill].basstatus) == 0) {
            result = 1;
        }
        break;
    case 51:
    case 85:
    case 86:
    case 92:
    case 237:
    case 238:
    case 239:
    case 240:
    case 246:
    case 248:
        if (cmpButtonGuideShowDone(unitList, unitCount) == 0) {
            break;
        }
        /* fall through */
    case 42:
    case 82:
    case 83:
    case 84:
    case 93:
        if (cmpButtonGuideHideDone(unitList, unitCount) != 0) {
            result = 1;
        }
        break;
    case 68:
    case 69:
    case 70:
    case 224:
        if (CheckHojo(partyList, partyCount,
                      datSkillTable.normal[nskill].hojotype) == 0) {
            result = 4;
        }
        break;
    case 218:
        if (cmpButtonGuideShowDone(unitList, unitCount) != 0) {
            if (cmpFindButtonGuideByFlag(unitList, unitCount,
                                         datSkillTable.normal[nskill].basstatus) == 0) {
                result = 1;
            }
        }
        break;
    default:
        break;
    }
    return result;
}

#ifdef NON_MATCHING
s32 nbCheckBattleSkillError3(s32 nskill, u32 select)
{
    datUnitWork_t *unitList[32];
    nbParty_t *partyList[32];
    datUnitWork_t *unit;
    nbParty_t *party;
    s32 unitCount = 0;
    s32 partyCount = 0;
    s32 i;

    nbGetMainProcessData();
    for (i = 0; i < 16; i++) {
        if (select & (1u << i)) {
            unit = nbGetUnitWorkFromFormindex(i);
            if (unit != NULL) {
                unitList[unitCount++] = unit;
            }
            party = nbGetPartyFromFormindex(i);
            if (party != NULL) {
                partyList[partyCount++] = party;
            }
        }
    }
    if (unitCount != 0) {
        switch (nskill) {
        case 0x34: case 0x35: case 0x36:
        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0xCB: case 0xCC: case 0xCD: case 0xCE:
        case 0x109: case 0x114:
            if (CheckHojo(partyList, partyCount, datSkillTable.normal[nskill].hojotype) == 0) {
                return 3;
            }
            break;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbSkillError", nbCheckBattleSkillError3);
#endif

s32 cmpClearButtonGuide(s32 skill, u32 select)
{
    /* Same unit-list capacity as nbCheckBattleSkillError2. */
    datUnitWork_t *unitList[32];
    datUnitWork_t *unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54);
    s32 unitCount = 0;
    s32 i;
    s32 result = 0;

    for (i = 0; i < 16; i++, unit++) {
        if ((select & (1u << i)) != 0 && (unit->flag & 1) != 0) {
            unitList[unitCount++] = unit;
        }
    }

    printf("\xA4\xB9\xA4\xAD\xA4\xEB\xA4\xB9\xA4\xAD\xA4\xEB\xA1\xE1%d\n", skill);

    /* These skills perform their specific validation below. */
    switch (skill) {
    case 0x31:
    case 0x32:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
        break;
    default:
        if (unitCount == 1 && cmpAnyButtonGuideActive(unitList, 1) != 0) {
            return 1;
        }
        break;
    }

    switch (skill) {
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x51:
    case 0x5E:
        if (cmpButtonGuideShowDone(unitList, unitCount) != 0) {
            result = 1;
        }
        break;
    case 0x2A:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x5D:
        if (cmpButtonGuideHideDone(unitList, unitCount) != 0) {
            result = 1;
        }
        break;
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x30:
    case 0x58:
        if (cmpFindButtonGuideByFlag(unitList, unitCount,
                                    datSkillTable.normal[skill].basstatus) == 0) {
            result = 1;
        }
        break;
    case 0x31:
    case 0x32:
        if (cmpAnyButtonGuideActive(unitList, unitCount) == 0) {
            return 1;
        }
        break;
    case 0x49:
        if ((DDS3_GBWK->maFlags & 1) != 0) {
            return 4;
        }
        break;
    case 0x4A:
        if ((DDS3_GBWK->maFlags & 2) != 0) {
            return 4;
        }
        break;
    case 0x4B:
        if ((DDS3_GBWK->maFlags & 4) != 0) {
            return 4;
        }
        break;
    case 0x4C:
        if ((DDS3_GBWK->maFlags & 8) != 0) {
            return 4;
        }
        break;
    case 0x50:
    case 0x55:
    case 0x56:
    case 0x5C:
    case 0xED:
    case 0xEE:
    case 0xEF:
    case 0xF0:
    case 0xF6:
    case 0xF8:
        if (cmpButtonGuideShowDone(unitList, unitCount) != 0) {
            result = cmpButtonGuideHideDone(unitList, unitCount) != 0;
        }
        break;
    default:
        break;
    }
    return result;
}


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

/* The local parameter reset retains retail's jal and shared epilogue.
 * Qualified cc1 trace: reset insn92 survives sibling-call selection and is
 * removed in life analysis; hooked, dump and plain assembly agree.
 * See the post-call-reset rule in docs/CONTRIBUTING.md. */
void nbLoadModelPackIfEnabled(nbFormation_t *form, s32 group, s32 no)
{
    if (form->packh == NULL) {
        if (evtBitCheck(0xC0F)) {
            form->packh = (nbPacHandle_t *)nbMakeModelPackName(no);
        } else {
            mdlPrepare(group, no, 0);
            nbLoadMidiFile(no);
            form = NULL;
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

void nbCopyPackData(nbStatWork_t *stat, void *src, u32 size, s32 unitid)
{
    void *p;

    if (stat != NULL) {
        p = sdfMemAllocDirect(size);
        memcpy(sdfMemLock(p), src, size);
        stat->motbuf = p;
    }
}

/* Pack descriptors follow the header at 0x20; offsets are buffer-relative. */
typedef struct {
    s32 type;
    u32 size;
    u32 reserved08;
    u32 offset;
} nbPackEntry_t;

extern nbStatWork_t *nbSetFormEntry(s32 id);
extern char D_005464E0[];

typedef struct {
    u8 reserved00[0x10];
    s32 entryCount;
    u8 reserved14[0xC];
    nbPackEntry_t entries[0];
} nbPackHeader_t;

void nbAddFormEntry(void *buffer, s32 group, s32 id)
{
    nbPackHeader_t *header = buffer;
    s32 i = 0;
    s32 motion = 0;
    nbStatWork_t *stat = nbSetFormEntry(id);
    nbPackEntry_t *entries = header->entries;

    for (; i < header->entryCount; i++) {
        nbPackEntry_t *entry = &entries[i];
        void *data = (u8 *)buffer + entry->offset;
        u32 size = entry->size;

        switch (entry->type) {
        case 0:
            nbDecodeModelPack(data, size, group, id);
            break;
        case 1:
            if (stat != NULL) {
                nbLoadMotionData(stat, data, size, motion, id);
            }
            motion++;
            break;
        case 2:
            if (stat != NULL) {
                nbCopyPackData(stat, data, size, id);
            }
            break;
        }
    }
    printf(D_005464E0, group, id);
}


s32 nbCheckModelPackLoaded(nbFormation_t *form, s32 group, s32 id)
{
    void *buf;

    if (evtBitCheck(0xC0F) != 0) {
        if (pacCheckEnd(form->packh) == 0) {
            goto zero;
        }
        buf = form->packh->entry->buf;
        nbAddFormEntry(sdfMemLock(buf), group, id);
        sdfMemFreeDirect(buf);
        pacFileHandleFree_(form->packh);
        form->packh = NULL;
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

