#include "common.h"
#include "sdk_libgcc.h"
#include "newbattle.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"
#include "field.h"

extern f32 aViewEye[];

extern f32 D_002FA240[];

extern f32 dds3ScreenParam[];

extern void ReverseMatrix(void);

extern void MultMatrixRev(f32 *aMatrix);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);

extern void nbDrawTargetCursor(nbMainProcessData_t *data, s32 x, s32 y);


extern s32 evtGetAgeOfMoon16(void);

extern void evtSetAgeOfMoon(s32 age);

extern void datSetDevilWork(s32 id, datUnitWork_t *w);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba,
                        sdfTexHandle_t *th);

extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);

/* The selection block at +0x70 of an akuma record, which nbSetPlayerStartData
 * clears before the devil enters the stock. */
typedef struct nbAkumaSelect_t {
    u8 unk00[0xA];
    s16 unk0A;
    s16 unk0C;
} nbAkumaSelect_t;

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


/* 60-byte rows; this unit reads only the base hp and mp pairs. */
typedef struct nbDevilInfo_t {
    u32 flag;
    u8 race;
    u8 unk05;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    u8 unk0E[0x2A];
    s8 attackattr;
    u8 attackcnt;
    u8 attackinterval;
    u8 reserve1;
} nbDevilInfo_t;

extern nbDevilInfo_t datDevilTable[];



/* Rows of the same 164-byte stride, a separate object from D_003989E0. */
typedef struct nbDeadEvent_t {
    u16 script;
    u16 event;
    u8 unk04[0xA0];
} nbDeadEvent_t;

/* 38-byte encounter formation rows. */
typedef struct nbEncountForm_t {
    u8 unk00[4];
    u8 formationtype;
    u8 unk05;
    u16 id[11];
    u8 unk_1C[4];
    u16 flag;
    u8 unk_22;
    u8 nakamayobi;
    u16 mae;
} nbEncountForm_t;

void nbGetUnitMarkPos(nbMainProcessData_t *data, nbFormation_t *form, sdfDrawTag_t *tag);

void nbDrawRotatedModel(sdfDrawTag_t *tag, sdfVector4_t *pos, sdfVector4_t *rot,
                        sdfVector4_t *center, f32 r, f32 scl, f32 h);

extern void *memset(void *a, s32 b, u32 c);

s32 nbClipEffectNum(s32 screenPosition[4]);

void nbLoadKoukaPos(nbFormation_t *form);

extern void *memset(void *a, s32 b, u32 c);

extern u32 effBTLSystemGetRGBA(s32 id);

extern char D_00544120[]; /* "nb_action" */

extern void ToMatrix(void);

void nbMakeUnitMatrix(nbFormation_t *form);

extern void nbSetOrderSide(s32 side);

extern void nbSetTargetSkill(nbActionProcessData_t *a, datUnitWork_t *work, s32 *ctype,
                             s32 *crule, s32 *carea, s32 mode);

extern s32 datCheckStockFull(void);

extern s32 datSearchDevilStock(s32 id);

extern s32 fclEncycRegist(datUnitWork_t *work, s32 mode);

extern void nbSortOrder(s32 start);

extern s32 datGetParam(datUnitWork_t *work, s32 paratype);

extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

extern nbEncountForm_t nbEncountFormTable[];


s32 nbCheckDeadScript(nbFormation_t *form);

extern s32 datGetItemSkillNo(s32 itemid);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern s32 nbGetJyokyoMes(nbMainProcessData_t *data, s32 jyokyomes);

extern datUnitVisual_t datModelParamTable[];

extern datUnitVisual_t D_003494B0;

extern nbDeadEvent_t D_00398A80[];

extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);

void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 var_id, s32 var_ind);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);

datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);

s32 nbGetUnitID(datUnitWork_t *work);

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

void nbAddOrder(s32 findex)
{
    nbMainProcessData_t *data;
    s32 start;
    s32 i;
    s32 partyIndex;
    u32 stat;

    data = nbGetMainProcessData();
    stat = data->form[findex].stat;
    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 0x40) == 0) {
        return;
    }
    start = data->orderindex + 1;
    partyIndex = data->form[findex].partyindex;
    for (i = start; i < 12; i++) {
        if (data->order[i] < 0) {
            data->order[i] = partyIndex;
            nbSortOrder(start);
            break;
        }
    }
}

void nbSortOrder(s32 start)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 i;
    s32 j;

    for (i = start; i < 11; i++) {
        if (data->order[i] < 0) {
            continue;
        }
        for (j = i + 1; j < 12; j++) {
            datUnitWork_t *wi;
            datUnitWork_t *wj;
            s32 pi;
            s32 pj;

            if (data->order[j] < 0) {
                continue;
            }
            wi = nbGetUnitWorkPointer(data, &data->party[data->order[i]]);
            wj = nbGetUnitWorkPointer(data, &data->party[data->order[j]]);
            pi = datGetParam(wi, 4);
            pj = datGetParam(wj, 4);
            if (pi < pj) {
                s8 t = data->order[i];

                data->order[i] = data->order[j];
                data->order[j] = t;
            } else if (pi == pj) {
                if (data->party[data->order[i]].formindex >
                    data->party[data->order[j]].formindex) {
                    s8 t = data->order[i];

                    data->order[i] = data->order[j];
                    data->order[j] = t;
                }
            }
        }
    }
}

void nbSetOrderSide(s32 side)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 cnt;
    s32 i;

    for (i = 0; i < 12; i++) {
        data->order[i] = -1;
    }
    cnt = 0;
    data->orderindex = -1;
    if (side == 0) {
        for (i = 0; i < 4; i++) {
            if ((data->party[i].flag & 1) != 0) {
                if ((data->party[i].flag & 0x20) == 0) {
                    data->order[cnt] = i;
                    cnt++;
                }
            }
        }
    } else {
        for (i = 4; i < 14; i++) {
            if ((data->party[i].flag & 1) != 0) {
                if ((data->party[i].flag & 0x20) == 0) {
                    data->order[cnt] = i;
                    cnt++;
                }
            }
        }
    }
    if (cnt > 0) {
        data->orderindex = 0;
    } else {
        data->orderindex = -1;
        data->activeunit = -1;
        return;
    }
    nbSortOrder(0);
    data->activeunit = data->order[data->orderindex];
}

void nbSetOrder(void)
{
    if ((nbGetMainProcessData()->stat & 8) == 0) {
        nbSetOrderSide(0);
        return;
    }
    nbSetOrderSide(1);
}

#ifdef NON_MATCHING
void nbSetNextOrder(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 next;

    if (data->orderindex < 0) {
        nbSetOrder();
        return;
    }
    do {
        next = data->orderindex + 1;
        data->orderindex = next;
        if (data->orderindex >= 12) {
            nbSetOrder();
            break;
        }
    } while (data->order[data->orderindex] < 0);
    data->activeunit = data->order[data->orderindex];
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMisc", nbSetNextOrder);
#endif

void nbUpdateOrder(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s8 *order = data->order;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (*order >= 0) {
            u16 flag = data->party[*order].flag;

            if ((flag & 1) == 0) {
                *order = -1;
            } else if ((flag & 0x20) != 0) {
                *order = -1;
            }
        }
        order++;
    }
}

/* vu0 routine: project vf10 only when it lies in the forward view cone. */
s32 nbClipEffectNum(s32 screenPosition[4])
{
    f32 viewDistance, forwardDot;
    f32 *projectionData;

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(viewDistance);
    if (dpcmp(fptodp(viewDistance), 16.0) <= 0)
        return 0;

    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADMATRIX(D_002FA240);
    SDF_VU0_COPYMATRIX_A_TO_B();
    ReverseMatrix();
    SDF_VU0_GET_ROW2();
    SDF_VU0_NORMALIZE();
    SDF_VU0_INNERPRODUCT(forwardDot);
    if (forwardDot <= 0.5f)
        return 0;

    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_COPYMATRIX_B_TO_A();
    projectionData = dds3ScreenParam + 12;
    MultMatrixRev(projectionData);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projectionData += 16;
    SDF_VU0_LOADVECTOR_B(projectionData);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3ScreenParam + 32);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_FTOI4();
    SDF_VU0_STOREVECTOR(screenPosition);
    screenPosition[0] -= 0x7000;
    screenPosition[1] -= 0x7900;
    return 1;
}

/* vu0 routine: the kouka mark position, scaled and lowered by the form radius. */
void nbLoadKoukaPos(nbFormation_t *form)
{
    f32 y;

    SDF_VU0_LOADVECTOR(&form->center);
    SDF_VU0_SCALEVECTOR_V0(form->scl);
    SDF_VU0_LOADVECTOR_B(&form->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_Y(y);
    y -= form->r * form->scl;
    SDF_VU0_SETVECTOR_Y(y);
}

void nbGetKoukaMarkPos(nbFormation_t *form)
{
    if ((form->stat & 3) == 3) {
        if (sdfModelGetMarkPos(form->hunit->owner->inner, 2) != 0) {
            return;
        }
    }
    nbLoadKoukaPos(form);
}

void nbMakeUnitMatrix(nbFormation_t *form)
{
    SDF_VU0_LOADVECTOR(&form->orgrot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&form->center);
    SDF_VU0_SCALEVECTOR(form->scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(form);
    SDF_VU0_ADDVECTOR();
}

void nbMakeUnitDrawMatrix(nbFormation_t *form)
{
    sdfVector4_t v;

    memset(&v, 0, sizeof(v));
    v.z = 1.0f;
    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    v.z = form->r;
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    nbMakeUnitMatrix(form);
    SDF_VU0_LOADVECTOR_B_MEMOPS(v);
    SDF_VU0_ADDVECTOR();
}

#ifdef NON_MATCHING
extern const f32 D_003FA880[][4];
extern const u32 D_003FACA0[];
extern const f32 D_003FAF20[][4];
extern const u32 D_003FB220[];
extern const f32 D_003FADB0[][4];
extern const u32 D_003FAED0[];
extern sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors,
                                                s32 vertexCount, u32 primitiveFlags);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void MultMatrixNoArg(void);
extern void SetRotateMatrixX_B(f32 angle);

void nbDrawRotatedModelX(sdfDrawTag_t *tag, sdfVector4_t *pos, sdfVector4_t *rot,
                         sdfVector4_t *center, f32 r, f32 scl, f32 h)
{
    sdfVector4_t scale = {1.0f, 1.0f, 1.0f, 1.0f};

    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(center);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_TRANSLATE_XYZ1();
    MultMatrixNoArg();
    SDF_VU0_LOADVECTOR(&scale);
    SDF_VU0_SCALEVECTOR(r);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SCALE_XYZW();
    SDF_VU0_COPYMATRIX_A_TO_C();
    sdfDrawTagLink(tag, (u32)sdfDrawRotatedModel(D_003FA880, D_003FACA0, 0x42, 0x80));
    SDF_VU0_COPYMATRIX_C_TO_A();
    SetRotateMatrixX_B(3.1415925f);
    MultMatrixNoArg();
    sdfDrawTagLink(tag, (u32)sdfDrawRotatedModel(D_003FA880, D_003FACA0, 0x40, 0x80));
}

void nbDrawRotatedModel(sdfDrawTag_t *tag, sdfVector4_t *pos, sdfVector4_t *rot,
                        sdfVector4_t *center, f32 r, f32 scl, f32 h)
{
    sdfVector4_t scale = {1.0f, 1.0f, 1.0f, 1.0f};

    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(center);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_TRANSLATE_XYZ1();
    MultMatrixNoArg();
    SDF_VU0_LOADVECTOR(&scale);
    SDF_VU0_SCALEVECTOR(r);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SCALE_XYZW();
    sdfDrawTagLink(tag, (u32)sdfDrawRotatedModel(D_003FAF20, D_003FB220, 0x30, 0x80));
    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(center);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_TRANSLATE_XYZ1();
    MultMatrixNoArg();
    SDF_VU0_LOADVECTOR(&scale);
    SDF_VU0_SCALEVECTOR(r);
    SDF_VU0_SCALEVECTOR(scl);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SCALE_XYZW();
    sdfDrawTagLink(tag, (u32)sdfDrawRotatedModel(D_003FADB0, D_003FAED0, 0x12, 0x80));
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMisc", nbDrawRotatedModelX);
INCLUDE_ASM(const s32, "newbattle/nbMisc", nbDrawRotatedModel);
#endif

void nbGetUnitMarkPos(nbMainProcessData_t *data, nbFormation_t *form, sdfDrawTag_t *tag)
{
    s32 pos[4];

    SDF_VU0_LOADVECTOR(&form->pos);
    if ((form->stat & 1) != 0) {
        sdfModelGetMarkPos(form->hunit->owner->inner, 0);
    }
    if (nbClipEffectNum(pos) != 0) {
        nbDrawTargetCursor(data, pos[0] >> 4, pos[1] >> 3);
    }
}

void nbDrawPanelSprite(sdfTexHandle_t *th, s32 no, s32 x, s32 y, u32 rgba)
{
    nbPanelProcess_nbPanelTMXuv_t uv;

    uv.u0 = no * 13 + 1;
    uv.v0 = 1;
    nbPnlSprite(x >> 4, y >> 3, 0xC, 0x12, uv.u0, uv.v0, 0xC, 0x12, rgba, th);
}

/* NON_MATCHING: 24 of 66 words differ (first at +0x74). */

#ifdef NON_MATCHING

void nbDrawKoukaSprite(sdfTexHandle_t *th, u32 value, s32 x, s32 y, u32 col, s32 align)
{
    u8 digits[12];
    s32 n = 0;

    if (value != 0) {
        do {
            digits[n] = value % 10;
            n++;
            value = value / 10;
        } while (value != 0);
    }
    if (n == 0) {
        digits[0] = 0;
        n = 1;
    }
    if (align != 0) {
        if (align <= 0) {
            if (align == -1) {
                x -= n * 192;
            }
        }
    } else {
        x -= n * 96;
    }
    for (n = n - 1; n >= 0; n--) {
        nbDrawPanelSprite(th, digits[n], x, y, col);
        x += 0xC0;
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbMisc", nbDrawKoukaSprite);

#endif

#ifdef NON_MATCHING
extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill);
extern s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid);

void nbSetTargetSkill(nbActionProcessData_t *a, datUnitWork_t *work, s32 *ctype,
                      s32 *crule, s32 *carea, s32 mode)
{
    s32 type = 0;
    s32 rule = 0;
    s32 area = 0;
    s32 skill;
    u8 targetarea;
    u8 targetrule;

    switch (work->nowcommand) {
    case 0:
        if (mode != 0) {
            rule = 2;
            area = 1;
        } else {
            area = 2;
        }
        if (nbHatudoCheckSkill(work, 0x130) != 0 && mode == 0) {
            type = 1;
        }
        break;
    case 1:
    case 5:
        if (work->nowcommand == 1) {
            skill = work->nowindex;
        } else {
            skill = nbGetItemSkillNo(a, work->nowindex);
        }
        switch (datSkillTable.normal[skill].targettype) {
        case 0: type = 0; break;
        case 1: type = 1; break;
        case 2: type = 2; break;
        }
        targetarea = datSkillTable.normal[skill].targetarea;
        if (targetarea & 1) {
            area = mode != 0 ? 2 : 1;
        }
        area |= targetarea & 2;
        targetrule = datSkillTable.normal[skill].targetrule;
        rule = targetrule & 0xF;
        break;
    case 4:
        area = 2;
        break;
    case 6:
        if ((a->data->stat & 8) == 0) {
            rule = a->data->playerpcnt >= 4 ? 2 : 12;
            area = 1;
        }
        break;
    }
    if (ctype != NULL) *ctype = type;
    if (crule != NULL) *crule = rule;
    if (carea != NULL) *carea = area;
}
#else
INCLUDE_RODATA(const s32, "newbattle/nbMisc", D_00544130);

INCLUDE_RODATA(const s32, "newbattle/nbMisc", D_00544140);

INCLUDE_ASM(const s32, "newbattle/nbMisc", nbSetTargetSkill);
#endif

#ifdef NON_MATCHING
extern datSkill_t attr[];
extern s32 nbHatudoCheckSkill(datUnitWork_t *work, s32 skill);
extern char D_00544170[];

extern s32 printf(const char *fmt, ...);

void nbSetNormalSkill0(datUnitWork_t *work)
{
    datNormalSkill_t *skill;
    datNormalSkillVisual_t *visual;
    datSkill_t *skillattr;
    s32 attackattr;
    u32 attackcnt;
    s32 index;

    if (work->nowcommand == 0) {
        index = 0;
        skill = &datSkillTable.normal[index];
        skillattr = &attr[index];
        visual = &datSkillTable.visual[index];
        skill->targettype = nbHatudoCheckSkill(work, 0x130) != 0;
    } else if (work->nowcommand == 1 && work->nowindex >= 0xA4 && work->nowindex < 0xA8) {
        index = work->nowindex;
        skill = &datSkillTable.normal[index];
        skillattr = &attr[index];
        visual = &datSkillTable.visual[index];
    } else {
        return;
    }
    if (work->flag & 4) {
        attackattr = 0;
        attackcnt = 1;
    } else {
        attackattr = datDevilTable[work->id].attackattr;
        attackcnt = datDevilTable[work->id].attackcnt;
    }
    if (skill != NULL) {
        skillattr->skillattr = attackattr;
        skill->targetcntmax = attackcnt;
        skill->targetcntmin = attackcnt;
        visual->animetype = 0;
    }
    printf(D_00544170);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMisc", nbSetNormalSkill0);
#endif

s32 nbGetNakama(datUnitWork_t *s)
{
    datUnitWork_t *stock;
    s32 i;

    if (datCheckStockFull() != 0) {
        return 0;
    }
    stock = NULL;
    datSearchDevilStock(s->id);
    for (i = 4; i < 0x10; i++) {
        if ((DDS3_GBWK->akuma[i].flag & 1) == 0) {
            stock = (datUnitWork_t *)&DDS3_GBWK->akuma[i].flag;
            break;
        }
    }
    datSetDevilWork(s->id, stock);
    fclEncycRegist(stock, 0);
    *((u8 *)(*(s32 *)((u8 *)DDS3_GBWK + 0x1924)) + (u32)DDS3_GBWK + 0x1914) = i;
    *(s32 *)((u8 *)DDS3_GBWK + 0x1924) = *(s32 *)((u8 *)DDS3_GBWK + 0x1924) + 1;
    return 1;
}

void nbSetDevilNameMes(s32 hmsg, s32 no, datUnitWork_t *work)
{
    if (work->id == 0) {
        itfMesMngSetVar(hmsg, 0, 0, 10);
        return;
    }
    itfMesMngSetVar(hmsg, no, work->id, 1);
}

void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 var_id, s32 var_ind)
{
    a->jyokyomes = nbGetJyokyoMes(a->data, jyokyomes);
    a->jyo_varid = var_id;
    a->jyo_varind = var_ind;
}

s32 nbGetJyokyoMes(nbMainProcessData_t *data, s32 jyokyomes)
{
    if ((data->stat & 8) != 0) {
        return jyokyomes + 1;
    }
    return jyokyomes;
}

u32 nbGetModelColor(nbFormation_t *form)
{
    datUnitWork_t *work = nbGetUnitWorkFromFormindex(form->formindex);

    if (work == NULL) {
        return 0x80808080;
    }
    switch (work->badstatus & 0xFFF) {
    case 0x001:
        return effBTLSystemGetRGBA(1);
    case 0x002:
        return effBTLSystemGetRGBA(2);
    case 0x004:
        return effBTLSystemGetRGBA(3);
    case 0x008:
        return effBTLSystemGetRGBA(4);
    case 0x010:
        return effBTLSystemGetRGBA(5);
    case 0x020:
        return effBTLSystemGetRGBA(6);
    case 0x040:
        return effBTLSystemGetRGBA(7);
    case 0x080:
        return effBTLSystemGetRGBA(8);
    case 0x100:
        return effBTLSystemGetRGBA(9);
    case 0x200:
        return effBTLSystemGetRGBA(10);
    case 0x400:
        return effBTLSystemGetRGBA(11);
    }
    return 0x80808080;
}

s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 id, s32 formindex)
{
    datUnitWork_t *unit = data->enemyunit;
    nbDevilInfo_t *info;
    datUnitWork_t *work;
    s32 i;

    for (i = 0; i < 10; i++) {
        if ((unit[i].flag & 1) == 0) {
            break;
        }
    }
    if (i == 10) {
        return -1;
    }
    work = &unit[i];
    datSetDevilWork(id, work);
    info = &datDevilTable[id];
    work->flag |= 2;
    work->maxhp = info->maxhp;
    work->hp = info->hp;
    work->maxmp = info->maxmp;
    work->mp = info->mp;
    return i;
}

typedef struct
{
    u8 unk_00000[0xA54];
    datUnitWork_t unitwork[16];
} nbStockGlobalWork_t;

extern nbStockGlobalWork_t *nbStockGlobalWork asm("DDS3_GBWK");
extern u32 dds3GetRandIntA(void *state, u32 limit);

s32 nbGetRandamStock(void)
{
    s32 entries[16];
    u32 count = 0;
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((nbStockGlobalWork->unitwork[i].flag & 1) != 0 &&
            (nbStockGlobalWork->unitwork[i].flag & 2) == 0 &&
            (nbStockGlobalWork->unitwork[i].flag & 0x10) == 0 &&
            (nbStockGlobalWork->unitwork[i].badstatus & 0xFFF) != 0x800 &&
            nbStockGlobalWork->unitwork[i].hp != 0) {
            entries[count++] = i;
        }
    }
    {
        s32 selected = -1;
        if (count != 0) {
            selected = entries[dds3GetRandIntA(NULL, count)];
        }
        return selected;
    }
}

void nbSetMainPhase(s32 encno)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    data->encno = encno;
    data->formationtype = nbEncountFormTable[encno].formationtype;
    data->endtype = 0;
    data->activeunit = -1;
    data->orderindex = -1;
    data->previndex = -1;
    data->prevcommand = 0;
    data->prevcommind = 0;
    data->prevflag = 0;
    data->turn = 0;
    data->nextphase = 0;
    data->esccnt = 0;
}

void nbCountActiveUnits(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        datUnitWork_t *work = nbGetUnitWorkFromFormindex(i);

        if (work != NULL) {
            if (work->prevtarea == 2) {
                work->prevtarea = -1;
                work->prevtform = -1;
            }
        }
    }
}

#ifdef NON_MATCHING
extern void nbSetEnemyFormation(nbMainProcessData_t *data);

void nbSetupBattleUnits(nbMainProcessData_t *data, s32 encno)
{
    nbEncountForm_t *entry = &nbEncountFormTable[encno];
    datUnitWork_t *work;
    s32 count = 0;
    s32 i;
    s32 nakamayobi;

    for (i = 0; i < 10; i++) {
        data->form[i + 4].stat = 0x200;
        data->party[i + 4].flag = 0;
    }
    for (i = 0; i < 10; i++) {
        if (entry->id[i] != 0) {
            datSetDevilWork(entry->id[i], &data->enemyunit[count]);
            work = &data->enemyunit[count];
            work->flag |= 2;
            work->maxhp = datDevilTable[entry->id[i]].maxhp;
            work->hp = datDevilTable[entry->id[i]].hp;
            work->maxmp = datDevilTable[entry->id[i]].maxmp;
            work->mp = datDevilTable[entry->id[i]].mp;
            nbAddUnitStock(data, 1, count, i + 4);
            count++;
        }
    }
    nbSetEnemyFormation(data);
    nakamayobi = entry->nakamayobi;
    data->nakamayobi = nakamayobi != 0 ? nakamayobi : -1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMisc", nbSetupBattleUnits);
#endif

void nbSetPlayerStartData(nbMainProcessData_t *data)
{
    s32 i;
    s32 cnt = 0;

    for (i = 0; i < *(s32 *)DDS3_GBWK->stocklist && cnt < 4; i++) {
        s32 id = DDS3_GBWK->unk1914[i];
        s32 off = id * sizeof(fldAnahoriAkuma_t) + 0xAC0;
        nbAkumaSelect_t *sel = (nbAkumaSelect_t *)((u8 *)DDS3_GBWK + off);
        u32 flag = DDS3_GBWK->akuma[id].flag;

        if (flag & 1) {
            sel->unk0A = -1;
            sel->unk0C = -1;
            if (flag & 2) {
                nbAddUnitStock(data, 0, id, cnt);
                if (data->stat & 0x10) {
                    data->form[cnt].stat |= 0x2000;
                }
                cnt++;
            }
        }
    }
    nbSetPlayerFormation(data, 0);
}

s32 nbGetUnitID(datUnitWork_t *work)
{
    s32 id = 0;

    if ((work->flag & 4) == 0) {
        id = work->id;
    }
    return (work->badstatus & 0xFFF) != 0x200 ? id : 0xFF;
}

datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work)
{
    s32 id = nbGetUnitID(work);

    if (id == 0) {
        return &D_003494B0;
    }
    return &datModelParamTable[id];
}

s32 nbCheckEventDead(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->script != 0) {
        return 1;
    }
    return p->event != 0;
}

s32 nbCheckDeadScript(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->script != 0) {
        return p->script;
    }
    return 0;
}

s32 nbCheckDeadEvent(nbFormation_t *form)
{
    s32 id = nbGetUnitID(nbGetUnitWorkFromFormindex(form->formindex));
    nbDeadEvent_t *p;

    if (id == 0) {
        return 0;
    }
    p = &D_00398A80[id];
    if (p->event != 0) {
        return p->event;
    }
    return 0;
}

void nbAddKagututiMoon(nbMainProcessData_t *data)
{
    nbFormation_t *form = nbGetEventForm();
    datUnitWork_t *work;

    if (form == NULL) {
        return;
    }
    work = nbGetUnitWorkFromFormindex(form->formindex);
    if (work == NULL) {
        return;
    }
    if (work->id != 0x125) {
        return;
    }
    evtSetAgeOfMoon((evtGetAgeOfMoon16() + 2) % 16);
}

s32 nbGetItemSkillNo(nbActionProcessData_t *a, s32 itemid)
{
    s32 no = datGetItemSkillNo(itemid);

    if (itemid == 1) {
        no = a->massuruid;
    }
    return no;
}

INCLUDE_RODATA(const s32, "newbattle/nbMisc", D_00544170);

