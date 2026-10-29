#include "common.h"
#include "newbattle.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"
#include "event.h"
#include "model.h"
#include "motion.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    mdlObj_t *owner;
    u8 unk_90[0x14];
    u32 flags;
} evtUnitHandle_t;

typedef struct effBTLSysHandle_t effBTLSysHandle_t;

typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;





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
    s32 formationtype;
    s32 unk_3C;
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

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern nbEncountForm_t nbEncountFormTable[];

extern f32 nbFormEnemyInterval;

extern s32 nbSetActionCommand(nbMainProcessData_t *data, s32 command);

extern void *memset(void *a, s32 b, u32 c);

extern void nbAllFormDispOn(void);

extern void nbGroupFormDispOn(s32 side);

extern void nbGroupFormDispOff(s32 side);

extern u32 f;

extern char D_005458A0[];

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void nbFormDispOff(s32 formindex);

extern void nbFormDispOn(s32 formindex);

extern void dds3SetPosPoseSet(dds3Basic_t *pBasic, sdfVector4_t *pos);

extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, f32 *rot);

extern void Mult(void);

extern f32 D_005458B0[4];

extern f32 D_00575440[4];

extern f32 D_005514F8;

extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);

extern void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot);

extern void nbResetFormPosRot(nbFormation_t *form);

extern void nbSetPartyFormation(nbMainProcessData_t *data, s32 group);

extern void nbCopyFormation(nbMainProcessData_t *data, s32 sindex, s32 dindex);
extern effBTLSysHandle_t *effBTLSystemCreate(nbFormation_t *pFormation, s32 id);
extern void effBTLSystemDelete2(effBTLSysHandle_t *hEffBTLSys);
extern s32 effBTLSystemGetID(effBTLSysHandle_t *hEffBTLSys);

extern void nbFormatFormation(nbMainProcessData_t *data, s32 index);

extern void nbSetEnemyFormation(nbMainProcessData_t *data);

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

extern u8 datModelParamTable[];

extern u128 D_00526420;

extern u128 D_00526430;

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void nbResetAllFormPosRot(void);

extern void nbSingleFormExceptedDispOff(nbFormation_t *form);

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

void nbFormDispOff(s32 formindex)
{
    nbFormation_t *f;

    f = &nbGetMainProcessData()->form[formindex];
    if ((f->stat & 1) == 0) {
        return;
    }
    if ((f->stat & 2) == 0) {
        return;
    }
    if (f->basic == NULL) {
        return;
    }
    f->hunit->owner->flags |= 1;
    dds3ObjectBaseOnFlag(f->basic, 0x1000);
}

void nbFormDispOff2(nbFormation_t *form)
{
    nbFormDispOff(form->formindex);
}

void nbFormDispOn(s32 formindex)
{
    nbFormation_t *f;

    f = &nbGetMainProcessData()->form[formindex];
    if ((f->stat & 1) == 0) {
        return;
    }
    if ((f->stat & 2) == 0) {
        return;
    }
    if (f->basic == NULL) {
        return;
    }
    f->hunit->owner->flags &= ~1;
    dds3ObjectBaseOffFlag(f->basic, 0x1000);
}

void nbFormDispOn2(nbFormation_t *form)
{
    nbFormDispOn(form->formindex);
}

void nbGroupFormDispOff(s32 side)
{
    s32 start;
    s32 count;
    s32 i;

    start = 0;
    count = 0;
    switch (side) {
    case 0:
        count = 4;
        break;
    case 1:
        start = 4;
        count = 10;
        break;
    case 2:
        start = 14;
        count = 2;
        break;
    }
    for (i = start; i < start + count; i++) {
        nbFormDispOff(i);
    }
}

void nbGroupFormDispOn(s32 side)
{
    s32 start;
    s32 count;
    s32 i;

    start = 0;
    count = 0;
    switch (side) {
    case 0:
        count = 4;
        break;
    case 1:
        start = 4;
        count = 10;
        break;
    case 2:
        start = 14;
        count = 2;
        break;
    }
    for (i = start; i < start + count; i++) {
        nbFormDispOn(i);
    }
}

void nbAllFormDispOff(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormDispOff(i);
    }
}

void nbAllFormDispOn(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormDispOn(i);
    }
}

void nbSingleFormExceptedDispOff(nbFormation_t *form)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (i == form->formindex) {
            nbFormDispOn(i);
        } else {
            nbFormDispOff(i);
        }
    }
}

void nbDoubleFormExceptedDispOff(nbFormation_t *form1, nbFormation_t *form2)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (i == form1->formindex || i == form2->formindex) {
            nbFormDispOn(i);
        } else {
            nbFormDispOff(i);
        }
    }
}

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);
extern void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate);

void nbRestartMotion(void)
{
    nbMainProcessData_t *data;
    /* Qualified cc1 allocation trace: both passes assign pseudo 85 (UIDs
     * 45/165), allocated to $16. Hooked and plain output agree; one form
     * iterator keeps the shared live range. See docs/cc1probe.md. */
    nbFormation_t *form;
    s32 i = 0;

    data = nbGetMainProcessData();

    for (; i < 16; i++) {
        form = &data->form[i];
        if ((form->stat & 1) == 0) continue;
        if ((form->stat & 2) == 0) continue;
        if ((form->stat & 0x8000) != 0) {
            datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(i));
            form->r = visual->formsize;
            form->cr = 0.0f;
            SDF_VU0_COPYVECTOR(&form->center, &visual->center);
            form->scl = visual->formscale;
            form->h = visual->formheight;
            form->oneshotr = visual->oneshotr;
            form->allshotr = visual->allshotr;
            form->stat &= ~0x8000;
        }
    }

    for (i = 0; i < 16; i++) {
        form = &data->form[i];
        if ((form->stat & 1) == 0) continue;
        if ((form->stat & 2) == 0) continue;
        if ((form->stat & 0x4000) != 0) {
            evtUnitHandle_t *hunit = form->hunit;
            hunit->flags &= ~0x400000;
            hunit->flags &= ~0x40;
            hunit->flags |= 0x20;
            form->nexthokanb = 0;
            form->nexthokan = 0;
            hunit->owner->first->speed = form->motspeed;
            form->stat &= ~0x4000;
            nbSetUnitMotion(form, 0, 1, 1.0f);
        }
    }
}

void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos)
{
    SDF_VU0_COPYVECTOR(&form->pos, pos);
    if ((form->stat & 1) == 0) {
        return;
    }
    if ((form->stat & 2) == 0) {
        return;
    }
    if (form->basic == NULL) {
        return;
    }
    dds3SetPosPoseSet(form->basic, pos);
    SDF_VU0_COPYVECTOR(&form->pos, pos);
}

void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot)
{
    sdfVector4_t *p = &form->rot;

    SDF_VU0_COPYVECTOR(p, rot);
    if ((form->stat & 1) == 0) {
        return;
    }
    if ((form->stat & 2) == 0) {
        return;
    }
    if (form->basic == NULL) {
        return;
    }
    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_LOADVECTOR_B(D_005458B0);
    Mult();
    SDF_VU0_STOREVECTOR_NOMEM(D_00575440);
    dds3SetRotPoseSet(form->basic, D_00575440);
    SDF_VU0_COPYVECTOR(p, rot);
}

void nbSetFormPosRot(nbFormation_t *form, sdfVector4_t *pos, sdfVector4_t *rot)
{
    nbSetFormPos(form, pos);
    nbSetFormRot(form, rot);
}

void nbResetFormPosRot(nbFormation_t *form)
{
    if (form->stat & 1) {
        nbSetFormPos(form, &form->orgpos);
        nbSetFormRot(form, &form->orgrot);
    }
}

void nbResetAllFormPosRot(void)
{
    nbMainProcessData_t *data;
    s32 i;

    data = nbGetMainProcessData();
    for (i = 0; i < 16; i++) {
        nbResetFormPosRot(&data->form[i]);
    }
}

void nbLoadUnitPos(nbFormation_t *form)
{
    SDF_VU0_LOADVECTOR(&form->pos);
}

void effBTLModelLoadRotate(nbFormation_t *form)
{
    SDF_VU0_LOADVECTOR(&form->rot);
}

f32 nbCameraGetBossDistance(void)
{
    return 800.0f;
}

f32 nbCameraGetSummonNear(void)
{
    return 50.0f;
}

f32 nbCameraGetSummonFar(void)
{
    return 100.0f;
}

f32 func_0028A330(void)
{
    return D_005514F8;
}

f32 nbGetEnemyInterval(void)
{
    return nbFormEnemyInterval;
}

INCLUDE_RODATA(const s32, "newbattle/nbFormation", D_005458B0);

#ifdef NON_MATCHING
extern f32 sdfAtan(f32 value);
extern f32 sdfSin(f32 value);
extern f32 sdfCos(f32 value);
extern s32 effBTLModelGetID(nbFormation_t *form);
extern s32 printf(const char *format, ...);
extern sdfVector4_t D_00526400;
extern sdfVector4_t D_00526410;

void nbSetPartyFormation(nbMainProcessData_t *data, s32 group)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    s32 list[32];
    s32 count = 0;
    s32 i;
    s32 formation;
    s32 id;
    f32 width = 0.0f;
    f32 interval = 0.0f;
    f32 halfWidth;
    f32 totalAngle;
    f32 gapAngle;
    f32 angle;
    f32 unitAngle;
    f32 distance;
    f32 sign;
    nbFormation_t *form;
    u32 stat;

    for (i = 0; i < 32; i++) {
        form = &data->form[i];
        stat = form->stat;
        if ((stat & group) && ((stat & 1) || (stat & 0x808) == 0x808)) {
            list[count++] = i;
            halfWidth = form->r * form->scl;
            width += halfWidth + halfWidth;
        }
    }
    if (count == 0) return;
    if (count > 1) {
        interval = 50.0f;
        if (!(1200.0f < width)) {
            interval = (1200.0f - width) / (f32)(u32)(count - 1);
            if (interval < 50.0f) interval = 50.0f;
            else if (interval > 100.0f) interval = 100.0f;
        }
    }
    if (group & 0x100) D_005514F8 = interval;
    else nbFormEnemyInterval = interval;
    gapAngle = sdfAtan(interval / 1200.0f);
    totalAngle = -gapAngle;
    for (i = 0; i < count; i++) {
        form = &data->form[list[i]];
        halfWidth = form->r * form->scl;
        totalAngle += sdfAtan((halfWidth + halfWidth) / 1200.0f);
        totalAngle += gapAngle;
    }
    distance = 1200.0f;
    if (width < 800.0f) width = 800.0f;
    else if (width > 800.0f) width = 800.0f;
    distance -= width * 0.5f;
    angle = -totalAngle * 0.5f;
    sign = 1.0f;
    if (!(group & 0x100)) sign = -1.0f;
    if (group & 0x100) SDF_VU0_COPYVECTOR(&rot, &D_00526400);
    else SDF_VU0_COPYVECTOR(&rot, &D_00526410);
    formation = data->formationtype >= 8 && data->formationtype < 11;
    for (i = 0; i < count; i++) {
        form = &data->form[list[i]];
        unitAngle = sdfAtan(form->r * form->scl / 1200.0f);
        angle += unitAngle;
        pos.x = sdfSin(angle) * 1200.0f;
        pos.y = 0.0f;
        pos.z = (distance - sdfCos(angle) * 1200.0f) * sign;
        id = effBTLModelGetID(form);
        if ((form->stat & 0x100) && id == 0x9B && !formation) {
            printf("nbSetPartyFormation : DV_ALBION\n");
            pos.z -= form->r * form->scl * 0.85f * sign;
        }
        nbSetFormPos(form, &pos);
        SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
        nbSetFormRot(form, &rot);
        SDF_VU0_COPYVECTOR(&form->orgrot, &rot);
        angle += unitAngle;
        angle += gapAngle;
        if (!(data->stat & 0x10) && (form->stat & 0x2000)) {
            form->stat &= ~0x2000;
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbFormation", nbSetPartyFormation);
#endif

#ifdef NON_MATCHING
void nbSetEnemyFormation(nbMainProcessData_t *data)
{
    sdfVector4_t pos, other;
    nbFormation_t *form, *second;
    datUnitWork_t *work;
    u32 i, id;
    f32 scale;

    switch (data->formationtype) {
    case 0:
        printf("nbSetEnemyFormation:ENC_FORMATION_NORMAL\n");
        nbSetPartyFormation(data, 0x200);
        break;
    case 1:
        printf("nbSetEnemyFormation:ENC_FORMATION_ARSIEL\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        for (i = 0; i < 10; i++, form++) {
            if (form->stat & 1) {
                nbLoadUnitPos(form);
                SDF_VU0_STOREVECTOR_NOMEM(&pos);
                pos.z += 300.0f;
                nbSetFormPos(form, &pos);
                SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
            }
        }
        break;
    case 2:
        printf("nbSetEnemyFormation:ENC_FORMATION_GIRIME\n");
        nbSetPartyFormation(data, 0x200);
        break;
    case 3:
        printf("nbSetEnemyFormation:ENC_FORMATION_MARA\n");
        nbSetPartyFormation(data, 0x200);
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        for (i = 0; i < 10; i++, form++) {
            if (form->stat & 1) {
                nbLoadUnitPos(form);
                SDF_VU0_STOREVECTOR_NOMEM(&pos);
                pos.z += 420.0f;
                nbSetFormPos(form, &pos);
                SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
            }
        }
        break;
    case 4:
        printf("nbSetEnemyFormation:ENC_FORMATION_3SISTER\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[5];
        if (form->stat & 1) {
            nbLoadUnitPos(form);
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            second = &data->form[4];
            nbLoadUnitPos(second);
            SDF_VU0_STOREVECTOR_NOMEM(&other);
            if (second->stat & 1) {
                other.z = pos.z;
                other.x = pos.x - 231.0f;
                nbSetFormPos(second, &other);
                SDF_VU0_COPYVECTOR(&second->orgpos, &other);
            }
            second = &data->form[6];
            nbLoadUnitPos(second);
            SDF_VU0_STOREVECTOR_NOMEM(&other);
            if (second->stat & 1) {
                other.z = pos.z;
                other.x = pos.x + 268.5f;
                nbSetFormPos(second, &other);
                SDF_VU0_COPYVECTOR(&second->orgpos, &other);
            }
        } else {
            form = &data->form[4];
            nbLoadUnitPos(form);
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            if (form->stat & 1) {
                second = &data->form[6];
                nbLoadUnitPos(second);
                SDF_VU0_STOREVECTOR_NOMEM(&other);
                if (second->stat & 1) {
                    other.z = pos.z;
                    other.x = pos.x + 345.0f;
                    nbSetFormPos(second, &other);
                    SDF_VU0_COPYVECTOR(&second->orgpos, &other);
                }
            }
        }
        break;
    case 5:
        printf("nbSetEnemyFormation:ENC_FORMATION_ALBION\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        for (i = 0; i < 10; i++, form++) {
            if (form->stat & 1) {
                work = nbGetUnitWorkFromFormindex(form->formindex);
                nbLoadUnitPos(form);
                SDF_VU0_STOREVECTOR_NOMEM(&pos);
                switch (work->id) {
                case 0x116: pos.z += 900.0f; break;
                case 0x117: pos.z += 600.0f; pos.x -= 100.0f; break;
                case 0x118: pos.z += 600.0f; pos.x -= 200.0f; break;
                case 0x119: pos.z += 600.0f; pos.x += 200.0f; break;
                case 0x11A: pos.z += 600.0f; pos.x += 100.0f; break;
                }
                nbSetFormPos(form, &pos);
                SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
            }
        }
        break;
    case 6:
        printf("nbSetEnemyFormation:ENC_FORMATION_TENSI\n");
        nbSetPartyFormation(data, 0x200);
        break;
    case 7:
        printf("nbSetEnemyFormation:ENC_FORMATION_BAAL\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        if (form->stat & 1) {
            nbLoadUnitPos(form);
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            pos.x -= 120.0f;
            nbSetFormPos(form, &pos);
            SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
        }
        break;
    case 8:
        printf("nbSetEnemyFormation:ENC_FORMATION_NET\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        if (form->stat & 1) {
            pos.x = 410.0f;
            pos.y = 0.0f;
            pos.z = 600.0f;
            nbSetFormPos(form, &pos);
            SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
            id = effBTLModelGetID(form);
            if (id == 0x103) {
                form->r = 400.0f;
                form->center.z = -150.0f;
                form->h = 780.0f;
                form->center.x = 410.0f;
                form->center.y = -450.0f;
            } else if (id == 0x124) {
                form->r = 400.0f;
                form->center.z = -220.0f;
                form->h = 780.0f;
                form->center.x = 410.0f;
                form->center.y = -450.0f;
            }
        }
        break;
    case 9:
    case 11:
        printf("nbSetEnemyFormation:ENC_FORMATION_ARI\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        if (form->stat & 1) {
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 600.0f;
            nbSetFormPos(form, &pos);
            SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
            id = effBTLModelGetID(form);
            switch (id) {
            case 0x102:
                form->r = 350.0f;
                form->center.z = -150.0f;
                form->h = 650.0f;
                form->center.y = -390.0f;
                form->center.x = 0.0f;
                break;
            case 0x123:
                form->r = 350.0f;
                form->center.z = -230.0f;
                form->h = 750.0f;
                form->center.y = -650.0f;
                form->center.x = 0.0f;
                break;
            case 0x156:
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = 1000.0f;
                nbSetFormPos(form, &pos);
                SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
                scale = form->scl;
                form->center.x = 0.0f;
                form->r = 200.0f / scale;
                form->center.z = 250.0f / scale;
                form->h = 700.0f / scale;
                form->center.y = -375.0f / scale;
                break;
            }
        }
        break;
    case 10:
        printf("nbSetEnemyFormation:ENC_FORMATION_KAGU\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        if (form->stat & 1) {
            id = effBTLModelGetID(form);
            if (id == 0x108) {
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z += 1450.0f;
                form->r = 500.0f;
                form->h = 1000.0f;
                form->center.y = -800.0f;
                form->center.z = 150.0f;
                form->center.x = 0.0f;
            } else if (id == 0x125) {
                pos.y = -659.99994f;
                pos.z += 960.0f;
                pos.x = 0.0f;
                form->r = 600.0f;
                form->h = 900.0f;
                form->center.y = 300.0f;
                form->center.z = 100.0f;
                form->center.x = 0.0f;
            }
            nbSetFormPos(form, &pos);
            SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
        }
        break;
    case 12:
        printf("nbSetEnemyFormation:ENC_FORMATION_BERU\n");
        nbSetPartyFormation(data, 0x200);
        break;
    case 13:
        printf("nbSetEnemyFormation:ENC_FORMATION_LUSI\n");
        nbSetPartyFormation(data, 0x200);
        form = &data->form[4];
        nbLoadUnitPos(form);
        SDF_VU0_STOREVECTOR_NOMEM(&pos);
        pos.z += 700.0f;
        pos.y -= 200.0f;
        nbSetFormPos(form, &pos);
        SDF_VU0_COPYVECTOR(&form->orgpos, &pos);
        scale = form->scl;
        form->r = 200.0f / scale;
        form->h = 1500.0f / scale;
        form->center.z = 300.0f / scale;
        form->center.y = -800.0f / scale;
        form->center.x = 0.0f / scale;
        break;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbFormation", nbSetEnemyFormation);
#endif

void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty)
{
    nbSetPartyFormation(data, 0x100);
}

void nbFormatFormation(nbMainProcessData_t *data, s32 index)
{
    nbFormation_t *form;

    form = &data->form[index];
    memset(form, 0, sizeof(nbFormation_t));
    form->r = 100.0f;
    form->cr = 100.0f;
    form->scl = 1.0f;
    form->formindex = index;
    if (index < 4) {
        form->stat |= 0x100;
    } else if (index < 0xE) {
        form->stat |= 0x200;
    } else {
        form->stat |= 0x400;
    }
    SDF_VU0_COPYVECTOR(&form->pos, &D_00526420);
    SDF_VU0_COPYVECTOR(&form->orgpos, &D_00526420);
    SDF_VU0_COPYVECTOR(&form->rot, &D_00526430);
    SDF_VU0_COPYVECTOR(&form->orgrot, &D_00526430);
}

void nbCopyFormation(nbMainProcessData_t *data, s32 sindex, s32 dindex)
{
    nbFormation_t *src;
    nbFormation_t *dst;
    s32 effectId = -1;

    if (sindex == dindex) {
        return;
    }

    src = &data->form[sindex];
    dst = &data->form[dindex];
    if (src->badeff != NULL) {
        effectId = effBTLSystemGetID(src->badeff);
        effBTLSystemDelete2(src->badeff);
        src->badeff = NULL;
    }
    if (src->stat & 0x40) {
        data->party[src->partyindex].formindex = dindex;
    }
    *dst = *src;
    dst->formindex = dindex;
    if (effectId >= 0) {
        dst->badeff = effBTLSystemCreate(dst, effectId);
    }
}

void nbMoveFormation(void *data, s32 sindex, s32 dindex)
{
    if (sindex != dindex) {
        nbCopyFormation(data, sindex, dindex);
        nbFormatFormation(data, sindex);
    }
}

void nbUpdateFormation(nbMainProcessData_t *data)
{
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        if ((data->form[i].stat & 1) == 0) {
            for (j = i + 1; j < 4; j++) {
                if ((data->form[j].stat & 1) != 0) {
                    nbMoveFormation(data, j, i);
                    break;
                }
            }
        }
    }
}

/* NON_MATCHING: 38 of 56 words differ (first at +0x4). */
#ifdef NON_MATCHING
void nbClearActionFlags(nbMainProcessData_t *data)
{
    nbFormation_t *form;
    nbFormation_t *p;
    s32 mask;
    s32 i;
    s32 src;

    form = data->form;
    mask = ~0x800;
    p = form;
    for (i = 3; i >= 0; i--) {
        p->stat &= mask;
        p++;
    }
    src = -1;
    p = form;
    for (i = 0; i < 4; i++) {
        if (!(p->stat & 1) && src >= 0) {
            SDF_VU0_COPYVECTOR(&p->orgpos, &form[src].orgpos);
            p->orgpos.x += form[src].r + 100.0f + 50.0f;
            SDF_VU0_COPYVECTOR(&p->pos, &p->orgpos);
            SDF_VU0_COPYVECTOR(&p->orgrot, &form[src].orgrot);
            SDF_VU0_COPYVECTOR(&p->rot, &p->orgrot);
            p->stat |= 0x800;
            return;
        }
        src = i;
        p++;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbFormation", nbClearActionFlags);
#endif

/* NON_MATCHING: 24 of 62 words differ (first at +0x5C). */
#ifdef NON_MATCHING
s32 nbSetActionCommand(nbMainProcessData_t *data, s32 command)
{
    nbFormation_t *form;
    s32 cnt;
    s32 i;
    s32 ret;

    form = data->form;
    ret = 0;
    for (i = 4; i < 14; i++) {
        form[i].stat &= ~0x800;
    }
    cnt = nbEncountFormTable[data->encno].unk_22;
    if (data->unk_84 == 0) {
        return 0;
    }
    if (cnt == 0) {
        return 0;
    }
    for (i = 4; i < cnt + 4; i++) {
        if ((form[i].stat & 1) == 0) {
            form[i].stat |= 0x800;
            if (command > 0) {
                form[i].r = *(f32 *)(datModelParamTable + command * 784 + 0x10) *
                            *(f32 *)(datModelParamTable + command * 784 + 0x18);
            }
            ret = 1;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbFormation", nbSetActionCommand);
#endif
INCLUDE_SDATA(const s32, "newbattle/nbFormation", D_005514F8);

INCLUDE_SDATA(const s32, "newbattle/nbFormation", nbFormEnemyInterval);

