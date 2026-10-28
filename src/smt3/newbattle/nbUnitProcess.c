#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"
#include "field.h"

extern void Mult(void);

extern f32 D_00543720[];
extern char D_00543730[];

extern f32 D_003FA7A0[];

extern u8 D_003493A0[];

extern u8 D_0034F1C0[];

void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu);

extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);

void nbSetUnitMotion(nbFormation_t *form, s32 motion2, s32 mode, f32 ritu);

void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect);

/* The quadword copy of a formation position: 16-byte aligned, so ee-gcc moves
 * it with one lq/sq pair. */
typedef struct nbQuadVector_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) nbQuadVector_t;

/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 lockframe;
    u16 end;
    u16 hp;
    u16 pad06;
} nbTimeList_t;

typedef struct nbActionProcessData_t {
    nbMainProcessData_t *data;
    u32 stat;
    s8 seq;
    s8 nextseq;
    s16 seqtime;
    s32 seqcnt;
    s32 skilltime;
    f32 press;
    s32 newpresstype;
    s16 newpress_p;
    s16 newpress_ten;
    s32 newaddpresstype;
    struct nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    u32 select;
    u32 timer;
    s32 runerr;
    s32 jyokyomes;
    s32 jyo_varid;
    s32 jyo_varind;
    s32 adata;
    s32 autoskill;
    s32 allhpdamage;
    s32 allmpdamage;
    s32 massuruid;
    u32 negotype;
    s32 motion;
    s32 motse;
    void *hEffBTL;
    u8 unk78[0x10];
    nbTimeList_t timelist[32];
    u32 packettime;
    s32 hitstopframe;
    u8 unk190[0x10];
    u32 aisummonid;
} nbActionProcessData_t;

/* 128-byte party records. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

typedef struct pmdProcessWork_t {
    nbFormation_t *form;
} pmdProcessWork_t;

extern sdfGrOt_t sdfGrOtTable[];

void nbGetUnitMarkPos(nbMainProcessData_t *data, nbFormation_t *form, sdfDrawTag_t *tag);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void effBTLSystemDraw(struct effBTLSysHandle_t *hEffBTLSys);

void nbDrawRotatedModel(sdfDrawTag_t *tag, sdfVector4_t *pos, sdfVector4_t *rot,
                        sdfVector4_t *center, f32 r, f32 scl, f32 h);

extern s32 datCheckDying(datUnitWork_t *work);

extern s32 mdlGetNumOfCut(struct mdlObj_t *aHandle, s32 aGroup);

extern void evtSetWaitMotion(struct evtUnit_t *h, s32 motion);

extern void evtSetWaitMotionSpeed(struct evtUnit_t *h, f32 sp);

extern s32 GetDyingBadWaitMotion(nbFormation_t *form, s32 motion, datUnitWork_t *work);

extern char D_00551118[];

extern char D_00543768[]; /* "nb_unitdraw" */

void *nbUnitProcess2(dds3ProcessID_t *id);

void *nbUnitShutdown(dds3ProcessID_t *id);

void *nbUnitDrawProcess1(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern s32 mdlGetMinorNum(mdlObj_t *aHandle);

/* The 0x1C-byte motion records at datUnitVisual_t +0x2C; the shared view splits
 * the same bytes as part[], six bytes later. */
typedef struct nbUnitMotion_t {
    s16 movetype;
    u16 actframe;
    s32 se_no;
    f32 motionsp;
    f32 atarisize;
    f32 movey;
    f32 movez;
    s16 bframe;
    s16 hframe;
} nbUnitMotion_t;

extern f32 nbGetMotionSpeed(nbFormation_t *form, s32 motion, f32 ritu);

extern f32 dds3GetRandFloatA(s32 name);

extern void dds3SetPosPoseSet(struct dds3Basic_t *pBasic, sdfVector4_t *pos);

extern void nbReturnUnit(s32 partyno, s32 lost, s32 dead);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern s32 nbMakeUnitFadeInPacket(s32 startframe, void *uniqueid, nbFormation_t *form, u32 col,
                                  s32 delay, s32 aframe, s32 colframe);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);

extern void dds3DeleteSceneBasicData(struct dds3Basic_t *basic);

extern void effBTLSystemDelete2(struct effBTLSysHandle_t *hEffBTLSys);

extern void nbSetFormationSideFlag(nbFormation_t *form, s32 index);

extern void nbGetFormEntry(s32 id);

extern u32 datGetMaxHp(datUnitWork_t *work);

extern u32 datGetMaxMp(datUnitWork_t *work);

extern void datClearCommMemory(datUnitWork_t *work);

extern s32 evtSetRegistUnit(s32 major, s32 minor);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, u32 serial, s32 type);

extern void dds3LinkLightObjectToUnitObject(dds3Basic_t *light, dds3Basic_t *unit, s32 flag);

extern void dds3SetRotPoseSet(dds3Basic_t *basic, u128 *rot);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, s32 flag);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern void evtSetUnitColorRGB(struct evtUnit_t *unit, s32 frame, u32 rgb);

extern void evtSetUnitColorA(struct evtUnit_t *unit, s32 frame, u32 alpha);

extern void nbFormDispOff(s32 formindex);

extern u32 nbGetModelColor(nbFormation_t *form);

extern void mdlSetEffectScale(mdlObj_t *model, f32 scale);

extern void nbLoadModelPackIfEnabled(nbFormation_t *form, s32 group, s32 no);
extern s32 nbCheckModelPackLoaded(nbFormation_t *form, s32 group, s32 id);
extern s32 nbLoadMidiFile(s32 no);
extern void sdfMotionSeek(sdfMotionCtrlr_t *ctrlr, f32 frame);
extern struct nbKoukaPacket_t *nbAddBadEffKoukaPacket(s32 startframe, s32 uniqueid,
                                                   nbFormation_t *form, u32 bad);
extern struct nbKoukaPacket_t *nbAddBadMotKoukaPacket(s32 startframe, s32 uniqueid,
                                                   nbFormation_t *form, s32 badstat);
extern s32 dds3GetRandIntA(s32 minimum, s32 range);
extern s32 printf(const char *format, ...);

datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);

s32 nbGetUnitID(datUnitWork_t *work);

#ifdef NON_MATCHING

#else

#endif

s32 GetDyingBadWaitMotion(nbFormation_t *form, s32 motion, datUnitWork_t *work)
{
    if ((nbGetMainProcessData()->stat & 0x1000) != 0) {
        return motion;
    }
    if (datCheckDying(work) != 0) {
        if (motion != 0) {
            return motion;
        }
        if (mdlGetNumOfCut(form->hunit->owner, 0) >= 0x17) {
            motion = 0x16;
        }
    }
    if (motion != 0) {
        return motion;
    }
    switch (work->badstatus & 0xFFF) {
    case 4:
    case 8:
    case 0x20:
    case 0x40:
    case 0x80:
    case 0x100:
        motion = 2;
        break;
    }
    return motion;
}

s32 nbSetNegoWaitMotion(nbFormation_t *form)
{
    datUnitWork_t *work = nbGetUnitWorkFromFormindex(form->formindex);

    if (work != NULL) {
        switch (work->badstatus & 0xFFF) {
        case 4:
        case 8:
        case 0x200:
            break;
        default:
            nbSetWaitMotion(form, 13, 1.0f);
            nbSetUnitMotion(form, 13, 0, 1.0f);
            break;
        }
    }
}

/* Complete motion-bank view of a native 0x310-byte model-visual row. */
typedef struct {
    sdfVector4_t center;
    f32 formsize;
    f32 formheight;
    f32 formscale;
    f32 oneshotr;
    f32 allshotr;
    u16 modelsize;
    u16 shadowsize;
    u16 badframe;
    u16 autowait;
    nbUnitMotion_t motions[26];
    u32 reserved[3];
} nbUnitVisualMotions_t;

typedef char nbUnitVisualMotions_size_must_be_0x310[
    sizeof(nbUnitVisualMotions_t) == 0x310 ? 1 : -1];

extern s32 nbGetMotionNo(nbFormation_t *form, s32 motion);
extern void evtSetUnitMotion(evtUnit_t *unit, s32 motion, s32 before, s32 smooth, s32 mode);
extern void evtSetWaitHokanFrame(evtUnit_t *unit, s32 before, s32 smooth);

void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 rate)
{
    nbMainProcessData_t *data;
    datUnitWork_t *work;
    nbUnitVisualMotions_t *visual;
    evtUnit_t *unit;
    s32 motionNo;
    s32 before = 0;
    s32 smooth = 0;
    f32 speed;

    data = nbGetMainProcessData();
    unit = form->hunit;
    work = nbGetUnitWorkPointer(data, &data->party[form->partyindex]);
    if ((form->anime & 0x80) == 0) {
        visual = (nbUnitVisualMotions_t *)nbGetUnitVisual(work);
        /* Requested and adjusted categories share one scalar; qualified cc1
         * allocation keeps both in $16. See docs/levers/addressing-and-types.md. */
        motion = GetDyingBadWaitMotion(form, motion, work);
        speed = nbGetMotionSpeed(form, motion, rate);
        motionNo = nbGetMotionNo(form, motion);

        switch (motion) {
        case 0:
        case 2:
        case 13:
        case 22:
        case 23:
            before = form->nexthokanb;
            smooth = form->nexthokan;
            break;
        case 1:
            before = 0;
            smooth = 1;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 24:
        case 25:
            before = 0;
            smooth = 0;
            break;
        case 14:
        case 15:
            before = 0;
            smooth = 0xF;
            break;
        }

        switch (motion) {
        case 0:
        case 2:
        case 13:
        case 22:
            if (unit->owner->current.h.motionIndex != motionNo) {
                evtSetUnitMotion(unit, motionNo, before, smooth, mode);
            }
            break;
        default:
            evtSetUnitMotion(unit, motionNo, before, smooth, mode);
            break;
        }

        unit->owner->first->speed = speed;
        form->motspeed = speed;
        form->nexthokan = visual->motions[motion].hframe;
        form->nexthokanb = visual->motions[motion].bframe;
        if (mode == 0 || mode == 3) {
            evtSetWaitHokanFrame(unit, form->nexthokanb, form->nexthokan);
            form->nexthokan = visual->motions[form->waitmot].hframe;
            form->nexthokanb = visual->motions[form->waitmot].bframe;
        }
    }
}


void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu)
{
    datUnitWork_t *work;
    datUnitVisual_t *visual;
    nbUnitMotion_t *motionRecord;
    s32 selectedMotion;
    s32 unitid;
    s32 motionNo;
    f32 speed;

    work = nbGetUnitWorkFromFormindex(form->formindex);
    selectedMotion = GetDyingBadWaitMotion(form, motion, work);
    unitid = nbGetUnitID(work);
    if (unitid == 0) {
        motionNo = D_003493A0[selectedMotion];
    } else {
        motionNo = D_0034F1C0[selectedMotion];
    }
    /* The loop-end note keeps the multiply before both callbacks in sched2.
     * Qualified cc1 trace: multiply133 precedes loop-end232 and call161;
     * plain, dump and hooked output agree. See docs/CONTRIBUTING.md. */
    do {
        visual = nbGetUnitVisual(work);
        motionRecord = (nbUnitMotion_t *)(selectedMotion * sizeof(nbUnitMotion_t) +
                                         (u32)visual + 0x2C);
        speed = motionRecord->motionsp * ritu;
    } while (0);
    evtSetWaitMotion(form->hunit, motionNo);
    evtSetWaitMotionSpeed(form->hunit, speed);
    form->waitmot = selectedMotion;
}

s32 nbCalcUnitVisual(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));

    return visual->part[part].unk00;
}

s32 nbCalcUnitVisual2(nbFormation_t *form, s32 part)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));

    return visual->part[part].unk01;
}

s32 nbGetMotionActframe(nbFormation_t *form, s32 motion, f32 ritu)
{
    nbUnitMotion_t *mot = (nbUnitMotion_t *)((u8 *)(motion * sizeof(nbUnitMotion_t)) +
        (u32)nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex)) + 0x2C);
    s32 frame = mot->actframe;

    return frame / nbGetMotionSpeed(form, motion, ritu);
}

f32 nbGetMotionSpeed(nbFormation_t *form, s32 motion, f32 ritu)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));
    f32 motspeed = *(f32 *)visual->part[motion].unk02;

    if (ritu != 1.0f) {
        motspeed *= ritu;
    }
    return motspeed;
}

/* NON_MATCHING: 23 of 46 words differ (first at +0x5C). */

#ifdef NON_MATCHING

s32 nbGetMotionNo(nbFormation_t *form, s32 motion)
{
    datUnitWork_t *work = nbGetUnitWorkFromFormindex(form->formindex);
    s32 id = nbGetUnitID(work);
    datUnitVisual_t *visual = nbGetUnitVisual(work);
    u32 ofs = motion * sizeof(nbUnitMotion_t) + 0x30;

    form->cr = *(f32 *)((u8 *)(motion * sizeof(nbUnitMotion_t)) + (u32)visual + 0x38) * form->scl;
    form->movey = *(f32 *)((u8 *)visual + ofs + 0xC);
    form->movez = *(f32 *)((u8 *)visual + ofs + 0x10);
    if (id == 0) {
        return D_003493A0[motion];
    }
    return D_0034F1C0[motion];
}

#else

INCLUDE_ASM(const s32, "newbattle/nbUnitProcess", nbGetMotionNo);

#endif

INCLUDE_ASM(const s32, "newbattle/nbUnitProcess", nbAddUnitStock);

void nbSummonUnit(s32 side, s32 stockno, s32 formindex)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    nbAddUnitStock(data, side, stockno, formindex);
    nbInitUnitEffect(&data->form[formindex], 0, 1);
}

void func_0024D938(void)
{
    s32 i;

    for (i = 4; i < 14; i++) {
        nbReturnUnit(i, 0, 0);
    }
}

void nbReturnUnit(s32 partyno, s32 lost, s32 dead)
{
    nbMainProcessData_t *data;
    struct nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    u16 id;
    u16 partyFlags;
    u16 badstatus;

    data = nbGetMainProcessData();
    party = &data->party[partyno];
    partyFlags = party->flag;
    if ((partyFlags & 1) != 0) {
        if ((party->flag & 8) == 0) {
            form = &data->form[party->formindex];
            if ((form->stat & 2) != 0) {
                dds3DeleteSceneBasicData(form->basic);
                form->basic = NULL;
                form->serial = 0;
                form->hunit = NULL;
            }
            if ((form->anime & 0x100) != 0 && form->badeff != NULL) {
                effBTLSystemDelete2(form->badeff);
                form->badeff = NULL;
            }
            form->partyindex = 0;
            form->stat = 0;
            form->flag = 0;
            form->anime = 0;
            form->type = 0;
            nbSetFormationSideFlag(form, party->formindex);
            party->flag |= 8;
        }

        work = nbGetUnitWorkPointer(data, party);
        id = work->id;
        if ((party->flag & 2) != 0) {
            work->flag = 0;
        } else if ((party->flag & 4) == 0) {
            if (lost != 0) {
                work->flag = 0;
            } else {
                work->flag &= ~2u;
            }

            if (dead != 0) {
                badstatus = work->badstatus;
                if ((badstatus & 0xFFF) == 0x200) {
                    work->badstatus = badstatus & 0xFDFF;
                    work->maxhp = datGetMaxHp(work);
                    work->maxmp = datGetMaxMp(work);
                }
                work->badstatus = 0x800;
                work->hp = 0;
            } else {
                badstatus = work->badstatus;
                switch (badstatus & 0xFFF) {
                case 1:
                case 2:
                case 4:
                case 8:
                case 16:
                case 128:
                    work->badstatus = badstatus & 0xF000;
                    break;
                default:
                    break;
                }
            }
            datClearCommMemory(work);
        }

        nbGetFormEntry(id);
        if ((party->flag & 2) != 0) {
            data->enemypcnt--;
        } else if ((party->flag & 4) == 0) {
            data->playerpcnt--;
        }
        party->flag = 0;
        party->formindex = -1;
        party->statindex = -1;
    }
}

void nb_24DBF0(s32 partyno, s32 noeffect)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    if ((data->party[partyno].flag & 1) != 0) {
        if ((data->party[partyno].flag & 8) == 0) {
            nbFormation_t *form = &data->form[data->party[partyno].formindex];

            if (noeffect == 0) {
                form->anime = (form->anime | 1) & ~2;
            }
        }
    }
}

void nb_24DC80(s32 partyno, s32 noeffect)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    if ((data->party[partyno].flag & 1) != 0) {
        if ((data->party[partyno].flag & 8) == 0) {
            nbFormation_t *form = &data->form[data->party[partyno].formindex];

            if (noeffect == 0) {
                form->anime = (form->anime | 2) & ~1;
            }
        }
    }
}

void nbRunKoukaType1(s32 partyindex, sdfVector4_t *pos, f32 time)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    struct nbParty_t *party =
        (struct nbParty_t *)((u8 *)(partyindex * 0x80) + (u32)data->party);
    nbFormation_t *form;

    if ((party->flag & 1) == 0) {
        return;
    }
    if ((party->flag & 8) != 0) {
        return;
    }
    form = &data->form[party->formindex];
    if (time == 1.0f) {
        form->anime &= ~0x20;
        form->stat |= 0x10;
        form->anime &= ~0x40;
        SDF_VU0_COPYVECTOR(&form->pos, pos);
        if (form->basic != NULL) {
            dds3SetPosPoseSet(form->basic, pos);
        }
        return;
    }
    form->anime |= 0x20;
    form->stat |= 0x10;
    form->anime &= ~0x40;
    SDF_VU0_COPYVECTOR(&form->startpos, &form->pos);
    SDF_VU0_COPYVECTOR(&form->tarpos, pos);
    form->shakePower = time;
    form->shakePhase = 0;
}

void nbRunKoukaType2(s32 partyindex, f32 time)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    struct nbParty_t *party =
        (struct nbParty_t *)((u8 *)(partyindex * 0x80) + (u32)data->party);
    nbFormation_t *form;

    if ((party->flag & 1) == 0) {
        return;
    }
    if ((party->flag & 8) != 0) {
        return;
    }
    form = &data->form[party->formindex];
    if (time == 1.0f) {
        form->anime &= ~0x20;
        form->stat &= ~0x10;
        form->anime &= ~0x40;
        SDF_VU0_COPYVECTOR(&form->pos, &form->orgpos);
        if (form->basic != NULL) {
            dds3SetPosPoseSet(form->basic, &form->orgpos);
        }
        return;
    }
    form->anime |= 0x40;
    form->anime &= ~0x20;
    SDF_VU0_COPYVECTOR(&form->startpos, &form->pos);
    SDF_VU0_COPYVECTOR(&form->tarpos, &form->orgpos);
    form->shakePower = time;
    form->shakePhase = 0;
}

void nbSetSakeKoukaParam(nbFormation_t *form, f32 range, f32 power)
{
    form->type = 2;
    form->damageRange = range;
    form->shakePhase = 0;
    form->shakePower = power;
    form->addmovet = power;
}

void nbSetDamage(nbFormation_t *form, f32 range, s32 week)
{
    datUnitWork_t *work;

    form->damageRange = range;
    form->type = 1;
    form->damageFrames = 20;
    form->time = 0;
    form->badframe = 0;
    work = nbGetUnitWorkFromFormindex(form->formindex);
    if ((form->stat & 0x40) != 0) {
        nbSetUnitMotion(form, 1, 0, 1.0f);
        if (week == 0) {
            s32 badframe = nbGetUnitVisual(work)->badframe;

            form->badframe = badframe;
            form->damageFrames = badframe + 10;
        }
    }
}

void nbSetDamage2(nbFormation_t *form, f32 range)
{
    form->type = 1;
    form->time = 0;
    form->damageFrames = 0x14;
    form->damageRange = range;
    form->addmovet = 0.0f;
}

void nbSetBadstatMotion(nbFormation_t *form, s32 type, f32 range)
{
    datUnitWork_t *work;
    nbMainProcessData_t *data;
    struct nbParty_t *party;

    data = nbGetMainProcessData();
    party = data->party;
    form->type = 3;
    form->time = 0;
    form->damageRange = range;
    form->damageFrames = 0;
    work = nbGetUnitWorkPointer(data, &party[form->partyindex]);
    if ((form->stat & 0x40) != 0) {
        s32 badframe;

        nbSetUnitMotion(form, 1, 0, 1.0f);
        badframe = nbGetUnitVisual(work)->badframe;
        form->damageFrames = badframe + 10;
        form->badframe = badframe;
    }
}

void nbSetBadstatMotion2(nbFormation_t *form)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    struct nbParty_t *party = &data->party[form->partyindex];

    form->type = 0;
    form->time = 0;
    form->damageFrames = 0;
    nbGetUnitWorkPointer(data, party);
    if ((form->stat & 0x40) != 0) {
        nbSetUnitMotion(form, 1, 0, 1.0f);
        form->anime |= 0x80;
        form->hunit->owner->first->speed = 0.0f;
    }
}

void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect)
{
    s32 stat = form->stat;
    u32 flag;

    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 0x40) == 0) {
        return;
    }
    if ((stat & 2) != 0) {
        return;
    }
    flag = form->flag & ~6;
    form->type = 4;
    form->time = 0;
    form->flag = flag | 1;
    if (kage != 0) {
        form->flag = flag | 9;
    }
    if (noeffect == 0) {
        form->anime |= 0x200;
    }
}

void nbLoadUnitGroup(s32 side, s32 stop, s32 anime)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 start = 0;
    s32 cnt = 0;
    s32 i;

    switch (side) {
    case 0:
        cnt = 4;
        break;
    case 1:
        start = 4;
        cnt = 0xA;
        break;
    }
    for (i = start; i < start + cnt; i++) {
        nbInitUnitEffect(&data->form[i], stop, anime);
    }
}

s32 nbCheckLoad(nbFormation_t *form)
{
    s32 flag = form->stat;

    if ((flag & 1) == 0) {
        return 0;
    }
    if ((flag & 0x40) == 0) {
        return 0;
    }
    if ((flag & 2) != 0) {
        return 0;
    }
    if ((form->flag & 4) != 0) {
        return 0;
    }
    return 1;
}

s32 nbCheckLoadGroup(s32 side)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 start;
    s32 cnt;
    s32 i;

    start = 0;
    cnt = 0;
    switch (side) {
    case 0:
        cnt = 4;
        break;
    case 1:
        start = 4;
        cnt = 10;
        break;
    }
    for (i = start; i < start + cnt; i++) {
        nbFormation_t *form = &data->form[i];

        if ((form->stat & 1) != 0) {
            if ((form->stat & 0x40) != 0) {
                if ((form->stat & 2) == 0) {
                    if ((form->flag & 4) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* NON_MATCHING: 14 of 152 words differ; see docs/nonmatching/newbattle/nbUnitProcess.md. */
#ifdef NON_MATCHING
void UpdateFormAnime(nbMainProcessData_t *data, nbFormation_t *form, datUnitWork_t *work)
{
    u32 anime;
    u32 phaseBits;
    f32 phase;
    f32 inversePhase;
    f32 blend;
    f32 power;
    f32 step;
    s32 motionIndex;

    if ((form->stat & 2) != 0 && (data->stat & 0x1000) == 0 &&
        mdlGetNumOfCut(form->hunit->owner, 0) >= 0x17) {
        if (datCheckDying(work) != 0) {
            motionIndex = form->hunit->owner->current.h.motionIndex;
            if (motionIndex != 0x16 && (motionIndex == 0 || motionIndex == 2)) {
                nbSetWaitMotion(form, 0, 1.0f);
                nbSetUnitMotion(form, 0, 1, 1.0f);
            }
        } else if (form->hunit->owner->current.h.motionIndex == 0x16) {
            nbSetWaitMotion(form, 0, 1.0f);
            nbSetUnitMotion(form, 0, 1, 1.0f);
        }
    }

    anime = form->anime;
    if ((anime & 0x60) != 0) {
        phase = form->range;
        power = form->shakePower;
        step = (1.0f - phase) * power;
        form->shakePower = power + 0.02f;
        if (step < 0.01f) {
            step = 0.01f;
        }
        form->range = phase + step;
        if (form->range > 1.0f) {
            form->range = 1.0f;
        }
        SDF_VU0_LOADVECTOR(&form->startpos);
        blend = form->range;
        inversePhase = 1.0f - blend;
        PS2_FLOAT_BITS(phaseBits, inversePhase);
        SDF_VU0_SCALEVECTOR_BITS(phaseBits);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if ((anime & 0x20) != 0) {
            SDF_VU0_LOADVECTOR(&form->tarpos);
        } else {
            SDF_VU0_LOADVECTOR(&form->orgpos);
        }
        PS2_FLOAT_BITS(phaseBits, blend);
        SDF_VU0_SCALEVECTOR_BITS(phaseBits);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(&form->pos);
        if (form->basic != NULL) {
            dds3SetPosPoseSet(form->basic, &form->pos);
            anime = form->anime;
            blend = form->range;
        }
        if (blend >= 1.0f) {
            if ((anime & 0x40) != 0) {
                form->stat &= ~0x10;
            }
            form->anime = anime & ~0x20;
            form->anime &= ~0x40;
            anime = form->anime;
        }
    }
    if ((anime & 0x400) != 0) {
        nbSetHpScale(form);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbUnitProcess", UpdateFormAnime);
#endif

void UpdateFormPosRot(nbFormation_t *form)
{
    dds3PoseSet_t *pose;

    if (form->basic != NULL) {
        pose = form->basic->poseset;
        SDF_VU0_COPYVECTOR(&form->pos, &pose->pos);
        SDF_VU0_LOADVECTOR(&pose->rot);
        SDF_VU0_LOADVECTOR_B(D_00543720);
        Mult();
        SDF_VU0_STOREVECTOR(&form->rot);
    }
}

void *nbUnitDrawProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    sdfDrawTag_t *tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTag_t *tag2;
    sdfGrOt_t *ot;
    void (*reg)(sdfGrOt_t *, sdfDrawTag_t *);
    s32 i;

    sdfDrawTagInit(tag);
    tag2 = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag2);
    for (i = 0; i < 0x10; i++) {
        nbFormation_t *form = &data->form[i];

        if ((form->anime & 0x1C) != 0) {
            nbGetUnitMarkPos(data, form, tag);
        }
        if ((form->anime & 0x100) != 0) {
            if (form->badeff != NULL) {
                effBTLSystemDraw(form->badeff);
            }
        }
        if ((data->stat & 0x100000) != 0) {
            nbDrawRotatedModel(tag2, &form->pos, &form->rot, &form->center, form->r, form->scl,
                               form->h);
        }
    }
    ot = &sdfGrOtTable[71];
    reg = ot->regFunc;
    reg(ot, tag);
    ot = &sdfGrOtTable[44];
    reg = ot->regFunc;
    reg(ot, tag2);
    return NULL;
}

/* NON_MATCHING: 20 of 65 words differ (first at +0x2C). */

#ifdef NON_MATCHING

void RunSake(nbMainProcessData_t *data, nbFormation_t *form)
{
    sdfVector4_t pos;
    struct dds3Basic_t *basic = form->basic;
    f32 power;
    f32 x;

    if (basic == NULL) {
        form->type = 0;
        form->time = 0;
        return;
    }
    power = form->shakePower;
    if (power >= 0.0f) {
        x = form->damageRange * form->range;
        form->shakePower = power + 0.02f;
        form->range += (1.0f - form->range) * power;
        if (form->range >= 0.99f) {
            form->shakePower = -form->addmovet * 0.6f;
        }
    } else {
        x = form->damageRange * form->range;
        form->shakePower = power - 0.01f;
        form->range -= form->range * -power;
        if (form->range <= 0.01f) {
            form->type = 0;
            SDF_VU0_COPYVECTOR(&pos, &form->pos);
            dds3SetPosPoseSet(basic, &pos);
            return;
        }
    }
    SDF_VU0_COPYVECTOR(&pos, &form->pos);
    pos.x += x;
    dds3SetPosPoseSet(basic, &pos);
}

#else

INCLUDE_ASM(const s32, "newbattle/nbUnitProcess", RunSake);

#endif

void RunBadAction(nbMainProcessData_t *data, nbFormation_t *form)
{
    sdfVector4_t pos;
    f32 d;

    if (form->basic == NULL) {
        return;
    }
    d = dds3GetRandFloatA(0) * 0.0f + 1.0f;
    if ((form->time & 1) != 0) {
        d = -d;
    }
    SDF_VU0_COPYVECTOR(&pos, &form->pos);
    pos.x += d;
    dds3SetPosPoseSet(form->basic, &pos);
}

void RunDamage(nbMainProcessData_t *data, nbFormation_t *form)
{
    sdfVector4_t pos;
    dds3Basic_t *basic = form->basic;
    f32 offset;

    if (basic == NULL) {
        form->type = 0;
        form->time = 0;
        return;
    }

    switch (form->type) {
    case 1:
        if (form->badframe > 0 && form->badframe == form->time) {
            form->hunit->owner->first->speed = 0.0f;
        }
        if (form->time >= (u32)form->damageFrames) {
            form->type = 0;
            form->time = 0;
            SDF_VU0_COPYVECTOR(&pos, &form->pos);
            dds3SetPosPoseSet(basic, &pos);
            if ((form->anime & 0x80) == 0 &&
                form->hunit->owner->first->speed == 0.0f) {
                nbSetUnitMotion(form, 0, 1, 1.0f);
            }
            return;
        }
        break;
    case 3:
        form->anime |= 0x80;
        if (form->badframe > 0 && form->badframe == form->time) {
            form->hunit->owner->first->speed = 0.0f;
        }
        if (form->time >= (u32)form->damageFrames) {
            form->type = 0;
            form->time = 0;
            SDF_VU0_COPYVECTOR(&pos, &form->pos);
            dds3SetPosPoseSet(basic, &pos);
            return;
        }
        break;
    }

    offset = dds3GetRandFloatA(0) * form->damageRange;
    if ((form->time & 1) != 0) {
        offset = -offset;
    }
    SDF_VU0_COPYVECTOR(&pos, &form->pos);
    pos.x += offset;
    dds3SetPosPoseSet(form->basic, &pos);

    if (form->damageRange > 0.0f) {
        form->damageRange -= 0.7f;
        if (form->damageRange < 0.0f) {
            form->damageRange = 0.0f;
        }
    }
}

s32 nbCheckModelLoad(nbMainProcessData_t *data, s32 major, s32 minor)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormation_t *form = &data->form[i];

        if ((form->stat & 1) != 0) {
            if ((form->stat & 2) != 0) {
                if (mdlGetMinorNum(form->hunit->owner) == minor) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void RegistUnit(nbMainProcessData_t *data, s32 major, s32 minor, nbFormation_t *form,
                datUnitWork_t *work)
{
    dds3Basic_t *basic;
    datUnitVisual_t *visual;
    f32 radius = 50.0f;
    s32 serial;

    serial = evtSetRegistUnit(major, minor);
    form->stat |= 2;
    basic = dds3SearchSceneBasic(dds3GetActiveScene(), serial, 5);
    form->serial = serial;
    form->basic = basic;
    form->hunit = ((dds3UnitObjectBasicWorkData_t *)basic->workdata)->hUnit;
    form->hunit->flags |= 0x200000;
    if (data->light[0] != NULL) {
        dds3LinkLightObjectToUnitObject(data->light[0], basic, 1);
    }
    nbSetWaitMotion(form, 0, 1.0f);
    nbSetUnitMotion(form, 0, 1, 1.0f);
    form->modelcol = nbGetModelColor(form);
    if (minor == 0x129) {
        form->anime |= 0x400;
    }
    dds3SetPosPoseSet(basic, &form->pos);
    if (form->partyindex < 4) {
        dds3SetRotPoseSet(basic, (u128 *)D_003FA7A0);
    }
    if ((form->anime & 0x200) == 0) {
        evtSetUnitColorRGB(form->hunit, 0, 0);
        evtSetUnitColorA(form->hunit, 0, 0);
    } else {
        nbFormDispOff(form->formindex);
    }
    visual = nbGetUnitVisual(work);
    if (visual->shadowsize == 0) {
        dds3ObjectBaseOffFlag(basic, 0x400);
    } else {
        dds3ObjectBaseOnFlag(basic, 0x400);
        switch (visual->shadowsize) {
        case 1:
            radius = 50.0f;
            break;
        case 2:
            radius = 100.0f;
            break;
        case 3:
            radius = 200.0f;
            break;
        case 4:
            radius = 300.0f;
            break;
        }
        basic->poseset->r = radius;
    }
    if (form->scl != 1.0f) {
        mdlSetEffectScale(form->hunit->owner, form->scl);
    }
    form->flag = (form->flag & ~2u) | 4;
}

s32 CheckLoad_nbUnitProcess(nbMainProcessData_t *data)
{
    nbFormation_t *form = data->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((form->stat & 1) != 0 && (form->flag & 2) != 0) {
            return 1;
        }
        form++;
    }
    return 0;
}

void RunLoad(nbMainProcessData_t *data, nbFormation_t *form)
{
    datUnitWork_t *work;
    s32 unitid;
    u32 flag;
    u32 random;
    f32 frame;
    u32 badstatus;

    work = nbGetUnitWorkFromFormindex(form->formindex);
    unitid = nbGetUnitID(work);
    flag = form->flag;
    if ((flag & 1) != 0) {
        if (CheckLoad_nbUnitProcess(data) != 0) {
            return;
        }
        if (nbCheckModelLoad(data, 1, unitid) != 0) {
            printf(D_00543730);
            form->flag &= ~1;
            RegistUnit(data, 1, unitid, form, work);
            nbLoadMidiFile(unitid);
            flag = form->flag;
        } else {
            form->flag = (form->flag & ~1) | 0x12;
            flag = form->flag;
        }
    }

    if ((flag & 2) != 0) {
        if ((flag & 0x10) != 0) {
            nbLoadModelPackIfEnabled(form, 1, unitid);
            form->flag &= ~0x10;
        }
        if (nbCheckModelPackLoaded(form, 1, unitid) != 0) {
            RegistUnit(data, 1, unitid, form, work);
        }
    }

    if ((form->flag & 0xC) == 4) {
        random = dds3GetRandIntA(0, form->hunit->owner->first->frameLength);
        frame = (f32)random;
        sdfMotionSeek(form->hunit->owner->first, frame);
        if ((form->anime & 0x200) != 0) {
            nbFormDispOff(form->formindex);
            form->anime &= ~0x200;
        } else {
            nbMakeUnitFadeInPacket(0, 0, form, 0, 0xF, 0xF, 0x14);
        }
        badstatus = work->badstatus & 0xFFF;
        if (badstatus != 0) {
            nbAddBadEffKoukaPacket(1, 0, form, badstatus);
            nbAddBadMotKoukaPacket(1, 0, form, work->badstatus & 0xFFF);
        }
        form->type = 0;
        form->flag &= ~4;
    }
}

extern sdfVector4_t D_003FA7B0;
extern void RunSake(nbMainProcessData_t *data, nbFormation_t *form);
extern void UpdateFormAnime(nbMainProcessData_t *data, nbFormation_t *form, datUnitWork_t *work);

INCLUDE_RODATA(const s32, "newbattle/nbUnitProcess", D_00543720);

INCLUDE_RODATA(const s32, "newbattle/nbUnitProcess", D_00543730);

void *nbUnitProcess2(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 i;

    for (i = 0; i < 16; i++) {
        nbFormation_t *form = &data->form[i];
        datUnitWork_t *work = nbGetUnitWorkFromFormindex(i);
        u32 stat = form->stat;
        if ((stat & 1) != 0) {
            switch (form->type) {
            case 0:
                if (work != NULL && ((work->badstatus & 0xFFF) == 1 ||
                                     (work->badstatus & 0xFFF) == 0x10)) {
                    RunBadAction(data, form);
                    stat = form->stat;
                }
                break;
            case 1:
            case 3:
                RunDamage(data, form);
                stat = form->stat;
                break;
            case 2:
                RunSake(data, form);
                stat = form->stat;
                break;
            case 4:
                RunLoad(data, form);
                stat = form->stat;
                break;
            }
            if ((stat & 0x80) != 0 && (form->stat & 2) != 0 && form->type != 3) {
                if (form->hunit->owner->first->stat == 5) {
                    if (form->hunit->motionSetup.motionId != 0) {
                        nbSetUnitMotion(form, 0, 3, 1.0f);
                    } else if (form->autotime != 0) {
                        form->autotime--;
                    } else {
                        nbSetUnitMotion(form, 0, 3, 1.0f);
                    }
                }
            }
            UpdateFormAnime(data, form, work);
            /* Qualified cc1probe: the one-pass eligibility block retains
             * separate signed type checks (rtl.00 through sched2), matching
             * retail. The equivalent combined condition folds to an unsigned
             * range test and is eight bytes shorter. Hooked/unhooked and
             * dump/plain outputs agree; see docs/CONTRIBUTING.md. */
            do {
                if (form->type < 5) {
                    if (form->type > 0) {
                        break;
                    }
                }
                if (work == NULL || ((work->badstatus & 0xFFF) != 1 &&
                                     (work->badstatus & 0xFFF) != 0x10)) {
                    UpdateFormPosRot(form);
                }
            } while (0);
            if (form->basic != NULL) {
                SDF_VU0_LOADVECTOR(&D_003FA7B0);
                SDF_VU0_SCALEVECTOR(form->scl);
                form->basic->poseset->flag = (form->basic->poseset->flag | 1) & ~2;
                SDF_VU0_STOREVECTOR(&form->basic->poseset->scl);
                if (form->basic != NULL) {
                    if ((form->stat & 0x2000) != 0) {
                        ((dds3UnitObjectBasicWorkData_t *)form->basic->workdata)->rotationFlags |= 1;
                    } else {
                        ((dds3UnitObjectBasicWorkData_t *)form->basic->workdata)->rotationFlags &= ~1;
                    }
                    form->basic->poseset->flag = (form->basic->poseset->flag | 1) & ~2;
                }
            }
            form->time++;
        }
    }
    return NULL;
}

void *nbUnitShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->UnitProcessID = 0;
}

void nbInitUnitProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id = dds3StartProcess(D_00551118, 0x3F9, 1, 1, nbUnitProcess2,
                                           nbUnitShutdown, NULL);

    dds3LinkProcess(data->MainProcessID, id);
    data->UnitProcessID = id;
    dds3LinkProcess(id, dds3StartProcess(D_00543768, 0x2B0E, 1, 1, nbUnitDrawProcess1,
                                         NULL, NULL));
}

void nbCheckPacketState(nbActionProcessData_t *a, s32 startframe)
{
    if (a->form->motendtype == 2) {
        nbMakeUnitFadeInPacket(startframe, a, a->form, 0, 2, 4, 4);
        a->form->motendtype = 0;
    }
}

INCLUDE_RODATA(const s32, "newbattle/nbUnitProcess", D_00543768);

INCLUDE_SDATA(const s32, "newbattle/nbUnitProcess", D_00551118);

