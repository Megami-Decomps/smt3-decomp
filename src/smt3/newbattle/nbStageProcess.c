#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"

extern char *text1[];
extern char *text2[];
extern char *pStr[];
extern char D_00551130[];
extern void cmpSetSkillListRows(s32 rows);
extern void *frwGetSmallFontFRQ(s32 x, s32 y, u32 col1, u32 col2, char *str, void *parent);
extern void *frwGetSmallNumFontFRQ(s32 x, s32 y, u32 col1, u32 col2, char *str, void *parent);
extern s32 frDrawFontEx(void *frq, s8 mode);
extern s32 frDestroyFontRequestAfter(void *frq);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern s32 datGetSkillCost(datUnitWork_t *work, s32 no);


extern f32 aViewEye[];
extern f32 D_002FA240[];
extern f32 dds3ScreenParam[];
extern void ReverseMatrix(void);
extern void MultMatrixRev(f32 *matrix);
extern void Mult(void);
extern f32 D_00543720[];
extern char D_00543FB8[]; /* "nego result = %d\n" */
extern char D_00544020[];
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);
extern void nbDrawTargetCursor(s32 type, s32 x, s32 y);
extern s32 printf(const char *format, ...);
extern nbFormation_t *nbGetEventForm();
extern s32 evtGetAgeOfMoon16(void);
extern void evtSetAgeOfMoon(s32 age);
extern void nbPanelDraw(s32 flag);
extern void *sdfChipAllocDirect(u32 size);
extern void datSetDevilWork(s32 devilid, datUnitWork_t *work);
extern u8 D_003493A0[];
extern u8 D_0034F1C0[];
extern void DispCommCursor(s32 ox, s32 oy, s32 a);
extern void DispSummonCursor(s32 ox, s32 oy, s32 a);
extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba,
                        sdfTexHandle_t *th);
extern s32 datCheckPlayerDead(void);
void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu);
extern s32 dds3ExistProcess(dds3ProcessID_t *id);
extern void nbAddUnitStock(nbMainProcessData_t *data, s32 side, s32 stockno, s32 formindex);
void nbSetUnitMotion(nbFormation_t *form, s32 motion, s32 mode, f32 ritu);
void nbInitUnitEffect(nbFormation_t *form, s32 kage, s32 noeffect);
/* These inherited symbol names identify the soft-double conversion and
 * comparison helpers in this executable, not integer conversion/division. */
extern f64 sdfFloatToULong(f32 value);
extern s32 dpdiv(f64 left, f64 right);


/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 unlock;
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
} nbActionProcessData_t;

/* 128-byte party records. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

/* 60-byte rows; this unit reads only the base hp and mp pairs. */
typedef struct nbDevilInfo_t {
    u32 flag;
    u8 race;
    u8 unk05;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    u8 unk0E[0x2E];
} nbDevilInfo_t;

extern nbDevilInfo_t datDevilTable[];

typedef struct nbPanelUV_t {
    s32 u;
    s32 v;
    s32 uw;
    s32 vh;
} nbPanelUV_t;

typedef struct pmdProcessWork_t {
    nbFormation_t *form;
} pmdProcessWork_t;

/* One object: StageData is its byte at +1 and nbStageParam its word at +8. */
typedef struct nbStageState_t {
    u8 unk00;
    u8 data;
    u8 pad02[2];
    s16 unk04;
    s16 unk06;
    u32 param;
} nbStageState_t;

typedef struct nbActionFunc_t {
    void (*set)(nbActionProcessData_t *a);
    s32 (*check)(nbActionProcessData_t *a);
} nbActionFunc_t;

/* 164-byte rows indexed by datUnitWork_t::id. */
typedef struct nbDevilData_t {
    u16 aiscript;
    u8 unk02[0xA2];
} nbDevilData_t;

/* Rows of the same 164-byte stride, a separate object from D_003989E0. */
typedef struct nbDeadEvent_t {
    u16 script;
    u16 event;
    u8 unk04[0xA0];
} nbDeadEvent_t;

typedef struct nbCommSelProcessData_t {
    void *handle;
    nbMainProcessData_t *data;
    nbActionProcessData_t *act;
    s8 stat;
    u8 nowtype;
    s16 pad0E;
    s32 drawtype;
    s32 drawcursor;
    s32 drawlisty;
    s32 nowcursor[6];
    s32 nowlisty[6];
    struct nbParty_t *my;
    struct nbParty_t *partytarget;
    u16 commlist[6][288];
    s32 commcnt[6];
    s8 commdisable[6][288];
    s32 cnt;
    f32 smritu;
    u32 unk14B4;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 alpha;
    s32 helpa;
    f32 ritu;
    s32 openwait;
} nbCommSelProcessData_t;

/* 38-byte rows; this unit reads only the formation type at +4. */
typedef struct nbEncountForm_t {
    u8 unk00[4];
    u8 formationtype;
    u8 unk05[0x21];
} nbEncountForm_t;

extern char D_005441A8[];

typedef struct nbMasterTextEntry_t {
    u8 unk00[0xF];
    u8 talkflag;
    u8 unk10[0x20];
} nbMasterTextEntry_t;
extern nbMasterTextEntry_t master_text[];
extern char D_00543880[];
extern s32 evtBitCheck(s32 bit);
extern sdfGrOt_t sdfGrOtTable[];
extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
extern void sdfPrimLinkG4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, u32 color0, s32 x1,
                          s32 y1, u32 color1, s32 x2, s32 y2, u32 color2, s32 x3, s32 y3,
                          u32 color3, s32 depth, void *alloc);
void nbGetUnitMarkPos(s32 type, nbFormation_t *form, sdfDrawTag_t *tag);
extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void effBTLSystemDraw(struct effBTLSysHandle_t *handle);
extern s32 datCheckTalk(datUnitWork_t *work);
extern s32 evtGetAgeOfMoon(void);
void nbDrawRotatedModel(sdfDrawTag_t *tag, sdfVector4_t *pos, sdfVector4_t *rot,
                        sdfVector4_t *center, f32 r, f32 scl, f32 h);

extern char D_00543FA8[]; /* "NEGO p:%d e:%d\n" */
extern void nbInitNegoProcess(nbActionProcessData_t *a, s32 pformindex, s32 eformindex,
                              u32 negotype, s32 nowindex);
extern struct nbKoukaPacket_t *nbAddUnitAKoukaPacket(s32 startframe, nbActionProcessData_t *a,
                                                     nbFormation_t *form, s32 type, s32 mode);
extern s32 effPCPBossONGY2GetLength(void);
extern struct nbKoukaPacket_t *nbAddOngOut2Packet(s32 startframe, nbActionProcessData_t *a,
                                                  nbFormation_t *form);

extern s32 datCheckDying(datUnitWork_t *work);
extern s32 mdlGetNumOfCut(struct mdlObj_t *ctx, s32 searchId);
extern s32 nbCheckSingleTargetSkill(s32 skillid);
extern struct nbKoukaPacket_t *nbAddSakeKoukaPacket(s32 startframe, nbActionProcessData_t *a,
                                                    nbFormation_t *form);

extern char D_00544070[];
extern char D_00544088[];
extern char D_005440A0[];
extern char D_005440B0[];
extern char D_005440C0[];
extern char D_005440D8[];
extern void evtSetWaitMotion(struct evtUnit_t *hunit, s32 motion);
extern void evtSetWaitMotionSpeed(struct evtUnit_t *hunit, f32 speed);
extern void nbPlaySe(s32 no);
extern struct nbKoukaPacket_t *nbAddSePacket(s32 startframe, void *uniqueid, s32 seid);
extern void nbPanelStockEnd(void);
extern s32 GetDyingBadWaitMotion(nbFormation_t *form, s32 motion, datUnitWork_t *work);

extern char D_00543618[];
extern char D_00543688[];
extern char D_00543698[];
extern u8 D_003FA760[];
extern void *memset(void *dst, s32 c, u32 n);
extern struct dds3Basic_t *dds3GetControlScene(void);
extern struct dds3Basic_t *dds3SceneBasicSearchNameAndType(struct dds3Basic_t *scene, u32 type,
                                                           char *name);
extern s32 dds3AllocObjectId(void);
extern struct dds3Basic_t *dds3AddLightObjectBasic(s32 serial);
extern void dds3SetLightObjectResoruce(struct dds3Basic_t *basic, u8 *resource);
extern void dds3SetUnitObjectId(struct dds3Basic_t *basic, char *name);
extern void dds3FadeOut(u32 r, u32 g, u32 b, u32 frame);
void *nbStageProcess1(dds3ProcessID_t *id);
void nbStageShutdown(dds3ProcessID_t *id);
void *nbStageDrawProcess1(dds3ProcessID_t *id);

extern void dds3FadeColorSet(u32 r, u32 g, u32 b);
extern void dds3FadeIn2(u32 frame);
extern s32 nbGetEncountOn(void);
extern void nbSetEncountMode(s32 mode);
void StageLoad(void);

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3SearchProcessName(const char *name);
extern void SetTarget_nbActionProcess(nbActionProcessData_t *a);
extern s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add);
extern void fldSkySetAlpha(s32 alp);
extern void nbPanelAllShow(void);
extern void nbPanelPressGaugeShow(void);
extern s32 nbRunAi(nbActionProcessData_t *a);
extern void ClearTimeList(nbActionProcessData_t *a);
extern void SetHpForTimeList(nbActionProcessData_t *a);
extern void nbInitAiScript(nbActionProcessData_t *a, s32 no);
extern s32 nbGetDevilFormatFlag(s32 formindex);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern void nbPanelStockSelectInit(void);
extern u32 nbPanelStockSelect(nbMainProcessData_t *data, u32 pad);
extern void sdfMemFreeDirect(void *handle);
extern void evtFreeAllUnit(void);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void SetCommCursor2(nbCommSelProcessData_t *s);
extern void nbMakeUnitFadeOutPacket(s32 start, nbActionProcessData_t *a, nbFormation_t *form,
                                    u32 col, s32 delay, s32 aframe, s32 colframe);
extern s32 nbAddReturnKoukaPacket(s32 start, nbActionProcessData_t *a, struct nbParty_t *party,
                                  s32 kage);
extern s32 nbCheckEscape(nbMainProcessData_t *data);
extern s32 nbMakePressPacket(s32 start, nbActionProcessData_t *a, f32 press);
extern s32 nbMakeNewPressPacket(s32 start, nbActionProcessData_t *a, s32 presstype,
                                nbFormation_t *form);
extern struct nbKoukaPacket_t *nbAddMojiEffKoukaPacket(s32 start, s32 frame,
                                                       nbActionProcessData_t *a,
                                                       nbFormation_t *form, s32 type);
extern struct nbKoukaPacket_t *nbAddFormOnOffPacket(s32 start, nbActionProcessData_t *a,
                                                    nbFormation_t *form, s32 sw);
s32 CheckKoketaNego(nbActionProcessData_t *a);
s32 nbClipEffectNum(s32 screenPosition[4]);
void nbLoadKoukaPos(nbFormation_t *form);
extern void *memset(void *dst, s32 c, u32 n);
extern u32 effBTLSystemGetRGBA(s32 type);
extern char D_00551118[];
extern char D_00543768[]; /* "nb_unitdraw" */
extern char D_00544120[]; /* "nb_action" */
void *nbUnitProcess2(dds3ProcessID_t *id);
void *nbUnitShutdown(dds3ProcessID_t *id);
void *nbUnitDrawProcess1(dds3ProcessID_t *id);
void *nbActionProcess1(dds3ProcessID_t *id);
void nbActionShutdown(dds3ProcessID_t *id);
nbActionProcessData_t *InitActionProcessData(nbMainProcessData_t *data, s32 type,
                                            s32 partyindex, s32 target, s32 adata);
extern void ToMatrix(void);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, void *processfunc,
                                         void *shutdownfunc, void *data);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);
nbCommSelProcessData_t *InitCommSelProcessData(nbMainProcessData_t *data, struct nbParty_t *my,
                                               nbActionProcessData_t *a);
void *nbCommSelProcess1(dds3ProcessID_t *id);
void nbCommSelShutdown(dds3ProcessID_t *id);
void nbMakeUnitMatrix(nbFormation_t *form);
extern void nbMakeDeadVisualPacket(s32 start, nbActionProcessData_t *a, nbFormation_t *form,
                                   s32 kage, s32 type);
void SetDeadPacket(nbActionProcessData_t *a, s32 formindex);
extern void nbSetOrderSide(s32 side);
extern s32 mdlGetMinorNum(mdlObj_t *obj);
extern void nbPushAction(s32 type, s32 partyindex, s32 target, s32 script);
extern void nbSortOrder(void);
extern f32 dds3GetRandFloatA(s32 stream);
extern void dds3SetPosPoseSet(struct dds3Basic_t *basic, sdfVector4_t *pos);
extern s32 GetEndTime(nbActionProcessData_t *a, s32 formindex);
extern s32 nbGetExpMakaItem(datUnitWork_t *work);
extern s32 nbMakeDeadPacket(s32 startframe, nbActionProcessData_t *a, nbFormation_t *form,
                            s32 deadchk);
extern void sdfChipFreeDirect(void *block);
extern s32 dds3GetRandIntA(s32 low, s32 high);
extern nbEncountForm_t nbEncountFormTable[];
void nbSetOrder(s32 index);
s32 CheckTimeListDead(nbActionProcessData_t *a, s32 formindex);
s32 nbCheckDeadScript(nbFormation_t *form);

extern s32 StkSelId;
extern nbStageState_t nbStageState;
extern nbActionFunc_t ActionFuncTable[];

extern s32 nbGetUniqueIdKoukaPacketCnt(nbActionProcessData_t *a);
extern void effBTLDelete(void *hBTLEffect);
extern void nbCameraSetState(u16 state);
extern void nbReturnUnit(s32 partyno, s32 lost, s32 dead);
extern s32 datGetItemSkillNo(s32 itemid);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern s32 nbGetJyokyoMes(nbMainProcessData_t *data, s32 jyokyomes);
extern s32 nbMakeUnitFadeInPacket(s32 startframe, void *id, nbFormation_t *form, u32 col,
                                  s32 delay, s32 aframe, s32 colframe);
extern void fldSkyCalcTable(void);
extern void fldSkyCalcFilter(void);
extern void fldSkyDrawFilter(void);
extern void nbStageSp(void);
extern datUnitVisual_t datModelParamTable[];
extern datUnitVisual_t D_003494B0;
extern nbDevilData_t D_003989E0[];
extern nbDeadEvent_t D_00398A80[];
extern void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);
extern void nbAddFlareChargePacket(s32 start, nbActionProcessData_t *a, s32 type);

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq);
void SetTimeList(nbActionProcessData_t *a, s32 formindex, s32 time, s32 unlockadd, s32 endadd);
extern void nbAddAnalyzePacket(s32 start, nbActionProcessData_t *a, nbFormation_t *form);
extern void nbPanelAnalyzeStart(void);
extern struct nbParty_t *nbGetPartyFromFormindex(s32 formindex);
void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 varid, s32 varind);
extern s32 nbGetMakaBaramaki(datUnitWork_t *work);
extern s32 nbCheckFormationPlayerSide(s32 formindex);
extern void datAddMaka(s32 maka);
extern s32 nbGetHojoCounter(s32 formindex, s32 hojotype);
extern void nbSetHojoCounter(s32 formindex, s32 hojotype, s32 n);
extern s32 nbMakeReturnPacket(s32 start, nbActionProcessData_t *a, struct nbParty_t *party);
extern void nbDelAllKoukaPacketType(s32 type);
extern void nbInitCommSelProcess(nbMainProcessData_t *data, struct nbParty_t *my,
                                 nbActionProcessData_t *a);
void MemoryCommand(nbCommSelProcessData_t *s);
s32 GetCommListCnt(nbCommSelProcessData_t *s, s32 type);
extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);
nbCommSelProcessData_t *GetCommSelProcessData(void);
datUnitVisual_t *nbGetUnitVisual(datUnitWork_t *work);
s32 nbGetUnitID(datUnitWork_t *work);

void SetBattleLight(nbMainProcessData_t *data)
{
    sdfVector4_t pos;

    memset(&pos, 0, sizeof(pos));
    pos.x = -1000.0f;
    if (dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, D_00543618) == NULL) {
        struct dds3Basic_t *light = dds3AddLightObjectBasic(dds3AllocObjectId());

        data->light[0] = light;
        dds3SetLightObjectResoruce(light, D_003FA760);
        dds3SetPosPoseSet(data->light[0], &pos);
        dds3SetUnitObjectId(data->light[0], D_00543618);
    }
    {
        struct dds3Basic_t *light = dds3AddLightObjectBasic(dds3AllocObjectId());

        data->light[1] = light;
        dds3SetLightObjectResoruce(light, D_003FA760);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddStageCamera);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddStageCamera2);

void nbSetStageParam(s16 a, s16 b)
{
    nbStageState.unk04 = a;
    nbStageState.unk06 = b;
    nbStageState.data = 2;
    nbStageState.unk00 = 0;
    nbStageState.param = 0x80;
}

void func_0024C6F0(void)
{
    nbStageState.param = 0;
    nbStageState.unk04 = 0;
    nbStageState.unk06 = 0;
    nbStageState.unk00 = 0;
    nbStageState.data = 3;
}

s32 nbGetStageData(void)
{
    return nbStageState.data;
}

void nbFreeStage(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck() != 0) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck() != 0) {
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", StageLoad);

void StageFree(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    evtFreeStage();
    nbStageState.unk04 = 0;
    nbStageState.unk06 = 0;
    data->unk7C = 0;
    data->unk7E = 0;
    nbStageState.data = 0;
}

void *nbStageProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    switch (nbStageState.data) {
    case 1:
        break;
    case 2:
        StageLoad();
        dds3FadeColorSet(0, 0, 0);
        if (nbStageState.unk00 == 3) {
            nbStageState.data = 1;
            if ((data->stat & 0x800) != 0) {
                break;
            }
            dds3FadeIn2(0xF);
        }
        if ((data->stat & 0x800) == 0) {
            if (nbGetEncountOn() != 0) {
                nbSetEncountMode(1);
            }
        }
        break;
    case 3:
        StageFree();
        break;
    }
    return NULL;
}

void nbStageShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->stageProcess = 0;
    StageFree();
    nbStageState.data = 0;
}

void *nbStageDrawProcess1(dds3ProcessID_t *id)
{
    if (nbStageState.data == 1) {
        fldSkyCalcTable();
        fldSkyCalcFilter();
        fldSkyDrawFilter();
        nbStageSp();
    }
    return NULL;
}

void nbInitStageProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;
    dds3ProcessID_t *draw;

    nbStageState.unk00 = 0;
    nbStageState.data = 0;
    nbStageState.pad02[0] = 0;
    nbStageState.unk04 = 0;
    nbStageState.unk06 = 0;
    id = dds3StartProcess(D_00543688, 0x3F9, 1, 1, nbStageProcess1, nbStageShutdown, NULL);
    dds3LinkProcess(data->unkBC, id);
    data->stageProcess = id;
    draw = dds3StartProcess(D_00543698, 0x2B0E, 1, 1, nbStageDrawProcess1, NULL, NULL);
    dds3LinkProcess(id, draw);
    if (nbGetEncountOn() == 0) {
        dds3FadeOut(0, 0, 0, 0);
    }
}

void nbStageFilterAlp(s32 alp)
{
    fldSkySetAlpha(alp);
}

void func_0024CBD0(u32 param)
{
    nbStageState.param = param;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbStageSp);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetUnitMotion);

/* NON_MATCHING: 8 of 54 words differ (first at +0x8C). */
#ifdef NON_MATCHING
void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu)
{
    datUnitWork_t *work = nbGetUnitWorkFromFormindex(form->formindex);
    s32 mot = GetDyingBadWaitMotion(form, motion, work);
    s32 no;
    f32 speed;

    if (nbGetUnitID(work) == 0) {
        no = D_003493A0[mot];
    } else {
        no = D_0034F1C0[mot];
    }
    speed = *(f32 *)nbGetUnitVisual(work)->part[mot].unk02 * ritu;
    evtSetWaitMotion(form->hunit, no);
    evtSetWaitMotionSpeed(form->hunit, speed);
    form->waitmot = mot;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetWaitMotion);
#endif

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetMotionActframe);

f32 nbGetMotionSpeed(nbFormation_t *form, s32 part, f32 speed)
{
    datUnitVisual_t *visual = nbGetUnitVisual(nbGetUnitWorkFromFormindex(form->formindex));
    f32 motspeed = *(f32 *)visual->part[part].unk02;

    if (speed != 1.0f) {
        motspeed *= speed;
    }
    return motspeed;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetMotionNo);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddUnitStock);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbReturnUnit);

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
    *(s32 *)&form->addmovet = 0;
    work = nbGetUnitWorkFromFormindex(form->formindex);
    if ((form->stat & 0x40) != 0) {
        nbSetUnitMotion(form, 1, 0, 1.0f);
        if (week == 0) {
            s32 badframe = nbGetUnitVisual(work)->badframe;

            *(s32 *)&form->addmovet = badframe;
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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetBadstatMotion);

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

void nbLoadUnitGroup(s32 side, s32 kage, s32 noeffect)
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
        nbInitUnitEffect(&data->form[i], kage, noeffect);
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

s32 nbCheckLoadGroup(s32 group)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 start;
    s32 cnt;
    s32 i;

    start = 0;
    cnt = 0;
    switch (group) {
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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", UpdateFormAnime);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunSake);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunDamage);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RegistUnit);

s32 CheckLoad_nbUnitProcess(pmdProcessWork_t *wdata)
{
    nbFormation_t *form = wdata->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((form->stat & 1) != 0 && (form->flag & 2) != 0) {
            return 1;
        }
        form++;
    }
    return 0;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", RunLoad);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543720);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543730);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbUnitProcess2);

void *nbUnitShutdown(dds3ProcessID_t *id)
{
    nbGetMainProcessData()->unitProcess = 0;
}

void nbInitUnitProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id = dds3StartProcess(D_00551118, 0x3F9, 1, 1, nbUnitProcess2,
                                           nbUnitShutdown, NULL);

    dds3LinkProcess(data->unkBC, id);
    data->unitProcess = id;
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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetDeadPacket);

void SetPress(nbActionProcessData_t *a, f32 press)
{
    if (press < -1.0f) {
        if (press < a->press) {
            a->press = press;
        }
    } else if (press > -1.0f) {
        if (a->press >= -1.0f) {
            if (a->press < press || a->press == 0.0f) {
                a->press = press;
            }
        }
    } else if (a->press == 0.0f) {
        a->press = press;
    }
}

void SetNewPress(nbActionProcessData_t *a, s32 presstype)
{
    if (a->newpresstype < presstype) {
        a->newpresstype = presstype;
    }
}

void SetHitStopFrame(nbActionProcessData_t *a, s32 frame)
{
    frame += 7;
    if (a->hitstopframe == 0 || frame < a->hitstopframe) {
        a->hitstopframe = frame;
    }
}

void SetEnemyEscapePacket(nbActionProcessData_t *a, s32 formindex, s32 start)
{
    nbMakeReturnPacket(start, a, nbGetPartyFromFormindex(formindex));
    SetTimeList(a, formindex, start, 0x1E, 0x1E);
}

void SetAuroraPacket(nbActionProcessData_t *a, s32 formindex)
{
    s32 counter;

    nbGetPartyFromFormindex(formindex);
    counter = nbGetHojoCounter(formindex, 0x13);
    switch (counter) {
    case 0:
        counter = 0xF8;
        break;
    case 0xF8:
        counter = 0xF9;
        break;
    case 0xF9:
        counter = 0xFA;
        break;
    case 0xFA:
        counter = 0xFB;
        break;
    case 0xFB:
        counter = 0xF8;
        break;
    }
    nbSetHojoCounter(formindex, 0x13, counter);
    nbSetJyokyoMes(a, 0x5A, 1, nbGetUnitWorkFromFormindex(formindex)->id);
}

void SetAddPressPacket(nbActionProcessData_t *a, s32 nskill)
{
    if (datSkillTable.normal[nskill].program != 13) {
        return;
    }
    switch (datSkillTable.normal[nskill].hojopoint) {
    case 2:
        a->newaddpresstype = 0xF;
        break;
    case 3:
        a->newaddpresstype = 0x11;
        break;
    case 4:
        a->newaddpresstype = 0x10;
        break;
    }
}

void SetAddFlareChargePacket(nbActionProcessData_t *a, s32 start)
{
    nbAddFlareChargePacket(start, a, 2);
}

void SetMakaPacket(nbActionProcessData_t *a, s32 unused, s32 time)
{
    s32 maka = nbGetMakaBaramaki(a->work);

    if (nbCheckFormationPlayerSide(a->form->formindex) != 0) {
        s32 limit = DDS3_GBWK->maka;

        if (limit < maka) {
            maka = limit;
        }
    }
    SetTimeList(a, a->form->formindex, time, 0x1E, 0x1E);
    nbSetJyokyoMes(a, 0x2C, 0, maka);
    if (nbCheckFormationPlayerSide(a->form->formindex) != 0) {
        datAddMaka(-maka);
    } else {
        datAddMaka(maka);
    }
}

void SetTorafuPacket(nbActionProcessData_t *a, s32 formindex, s32 start)
{
    nbFormation_t *form = &a->data->form[formindex];

    nbMakeUnitFadeOutPacket(start, a, form, 0, 6, 6, 6);
    nbAddFormOnOffPacket(start + 12, a, form, 0);
    a->data->endtype = 3;
}

void SetAnalyzePacket(nbActionProcessData_t *a, s32 formindex, s32 start)
{
    nbAddAnalyzePacket(start + 2, a, &a->data->form[formindex]);
    nbPanelAnalyzeStart();
    a->stat |= 0x10;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetPlayerSummonPacket);

void SetSakePacket(nbActionProcessData_t *a, s32 nskill, s32 sformindex, s32 dformindex,
                   s32 cnt, s32 frame)
{
    nbAddSakeKoukaPacket(frame, a, &a->data->form[dformindex]);
    SetHitStopFrame(a, frame);
    SetTimeList(a, dformindex, frame, 0xA, 0x14);
    if (nbCheckSingleTargetSkill(nskill) != 0) {
        if (nbCheckFormationPlayerSide(dformindex) != 0) {
            nbSetJyokyoMes(a, 2, -1, 0);
        } else {
            nbSetJyokyoMes(a, 4, -1, 0);
        }
    }
    SetPress(a, -2.0f);
    SetNewPress(a, 4);
}

void SetMissPacket(nbActionProcessData_t *a, s32 sformindex, s32 nskill, s32 formindex,
                   s32 week, s32 start)
{
    nbAddMojiEffKoukaPacket(start, 0x2D, a, &a->data->form[formindex], 3);
    SetTimeList(a, formindex, start, 10, 20);
    SetPress(a, -2.0f);
    SetNewPress(a, 4);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetOngShadowPacket);

void SetOngShadowPacket2(nbActionProcessData_t *a, s32 skipindex, s32 frame)
{
    s32 cnt = 0;
    s32 i;

    for (i = 4; i < 0xE; i++) {
        nbFormation_t *form;
        u32 stat;

        if (i == skipindex) {
            continue;
        }
        form = &a->data->form[i];
        stat = form->stat;
        if ((stat & 1) != 0) {
            if ((stat & 2) != 0) {
                nbAddUnitAKoukaPacket(frame + 1, a, form, 0, 0);
                cnt++;
                nbMakeDeadPacket(frame + effPCPBossONGY2GetLength() + 1, a, form, 1);
                nbAddOngOut2Packet(frame, a, form);
                SetTimeList(a, i, frame, 0x1E, 0x1E);
            }
        }
    }
    if (cnt > 0) {
        a->data->stat |= 0x4000;
    }
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543768);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAllPacket);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetTarget_nbActionProcess);

void ClearTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        a->timelist[i].unlock = 0;
        a->timelist[i].end = 0;
        a->timelist[i].hp = 0;
    }
}

void SetHpForTimeList(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        a->timelist[i].hp = 0;
        if ((a->data->form[i].stat & 1) != 0) {
            if ((a->data->form[i].stat & 0x40) != 0) {
                a->timelist[i].hp =
                    nbGetUnitWorkPointer(a->data,
                                         &a->data->party[a->data->form[i].partyindex])
                        ->hp;
            }
        }
    }
}

s32 GetTimeListHp(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].hp;
}

s32 CheckTimeListDead(nbActionProcessData_t *a, s32 formindex)
{
    if (a->timelist[formindex].hp != 0) {
        return 0;
    }
    if ((nbGetDevilFormatFlag(formindex) & 0x100) != 0) {
        return 0;
    }
    return 1;
}

s32 CheckTimeListDeadEnemyParty(nbActionProcessData_t *a, s32 formindex)
{
    s32 start;
    s32 cnt;
    s32 i;

    if (formindex < 4) {
        start = 4;
        cnt = 10;
    } else {
        start = 0;
        cnt = 4;
    }
    for (i = start; i < start + cnt; i++) {
        if ((a->data->form[i].stat & 1) != 0) {
            if (CheckTimeListDead(a, i) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

s32 AddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add)
{
    s32 hp;

    if ((a->data->stat & 0x20000) != 0) {
        return 0;
    }
    hp = a->timelist[formindex].hp + add;
    if (hp < 0) {
        a->timelist[formindex].hp = 0;
        return 1;
    }
    a->timelist[formindex].hp = hp;
    return 0;
}

s32 nbAddTimeListHp(nbActionProcessData_t *a, s32 formindex, s32 add)
{
    return AddTimeListHp(a, formindex, add);
}

s32 GetUnlockTime(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].unlock;
}

s32 GetEndTime(nbActionProcessData_t *a, s32 formindex)
{
    return a->timelist[formindex].end;
}

void OverwriteTimeList(nbActionProcessData_t *a, s32 formindex, s32 time, s32 unlockadd, s32 endadd)
{
    if (unlockadd >= 0) {
        if (a->timelist[formindex].unlock < time + unlockadd) {
            a->timelist[formindex].unlock = time + unlockadd;
        }
    }
    if (endadd >= 0) {
        if (a->timelist[formindex].end < time + endadd) {
            a->timelist[formindex].end = time + endadd;
        }
        if (a->timelist[formindex].end < a->timelist[formindex].unlock) {
            a->timelist[formindex].end = a->timelist[formindex].unlock;
        }
    }
}

void SetTimeList(nbActionProcessData_t *a, s32 formindex, s32 time, s32 unlockadd, s32 endadd)
{
    if (time < a->timelist[formindex].unlock) {
        time = a->timelist[formindex].unlock;
    }
    if (unlockadd >= 0) {
        if (a->timelist[formindex].unlock < time + unlockadd) {
            a->timelist[formindex].unlock = time + unlockadd;
        }
    }
    if (endadd >= 0) {
        if (a->timelist[formindex].end < time + endadd) {
            a->timelist[formindex].end = time + endadd;
        }
        if (a->timelist[formindex].end < a->timelist[formindex].unlock) {
            a->timelist[formindex].end = a->timelist[formindex].unlock;
        }
    }
}

s32 GetAllEndTime(nbActionProcessData_t *a)
{
    s32 max = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
        if (max < a->timelist[i].end) {
            max = a->timelist[i].end;
        }
    }
    return max;
}

s32 GetAllUnlockTime(nbActionProcessData_t *a)
{
    s32 max = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
        if (max < a->timelist[i].unlock) {
            max = a->timelist[i].unlock;
        }
    }
    return max;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005437F8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", GetKoukaList);

void SetTargetPlayer(nbActionProcessData_t *a)
{
    nbFormation_t *form = a->data->form;
    s32 i;

    for (i = 0; i < 16; i++) {
        form[i].stat &= ~4;
        form[i].stat &= ~8;
    }
    for (i = 0; i < 16; i++) {
        if ((a->data->form[i].stat & 1) != 0) {
            if ((a->data->form[i].stat & 0x40) != 0) {
                if ((a->data->form[i].stat & 0x100) != 0) {
                    if ((nbGetUnitWorkFromFormindex(i)->flag & 4) != 0) {
                        a->data->form[i].stat |= 8;
                        return;
                    }
                }
            }
        }
    }
}

void SetTargetParty(nbActionProcessData_t *a, s32 partyindex)
{
    nbFormation_t *form;
    nbFormation_t *p;
    s32 i;

    form = a->data->form;
    for (i = 0; i < 16; i++) {
        form->stat &= ~4;
        form->stat &= ~8;
        form++;
    }
    p = a->data->form;
    for (i = 0; i < 16; i++) {
        if ((p->stat & 1) != 0) {
            if ((p->stat & 0x40) != 0) {
                if (p->partyindex == partyindex) {
                    p->stat |= 8;
                    return;
                }
            }
        }
        p++;
    }
}

s32 CheckArrCounter(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 hit;

    if (a->partyindex >= 4) {
        return 0;
    }
    form = nbGetEventForm();
    if (CheckTimeListDead(a, form->formindex) != 0) {
        return 0;
    }
    if (nbGetUnitWorkFromFormindex(form->formindex)->id != 0x123) {
        return 0;
    }
    hit = 0;
    if (evtBitCheck(0x851) != 0) {
        if ((a->stat & 0x10000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x852) != 0) {
        if ((a->stat & 0x20000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x853) != 0) {
        if ((a->stat & 0x40000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x854) != 0) {
        if ((a->stat & 0x80000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x855) != 0) {
        if ((a->stat & 0x100000) != 0) {
            hit = 1;
        }
    }
    if (evtBitCheck(0x856) != 0) {
        if ((a->stat & 0x200000) != 0) {
            hit = 1;
        }
    }
    if (hit == 0) {
        return 0;
    }
    nbPushAction(4, form->partyindex, a->partyindex, 0xF3);
    printf(D_00543880);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckCounter);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckMukanNego);

/* NON_MATCHING: 57 of 74 words differ (first at +0x4). */
#ifdef NON_MATCHING
s32 CheckSenseiNego(nbActionProcessData_t *a)
{
    u32 stat;
    datUnitWork_t *work;
    s32 rate;

    if ((u16)a->data->unk7C >= 0xF0 && (u16)a->data->unk7C <= 0xF6) {
        return 0;
    }
    stat = a->data->stat;
    if ((stat & 1) != 0) {
        return 0;
    }
    if ((stat & 0x80) == 0) {
        return 0;
    }
    if ((stat & 0x10) != 0) {
        return 0;
    }
    if ((stat & 0x100) != 0) {
        return 0;
    }
    if (evtGetAgeOfMoon() == 8) {
        return 0;
    }
    work = a->work;
    if (master_text[work->id].talkflag == 0) {
        return 0;
    }
    if (datCheckTalk(work) == 0) {
        return 0;
    }
    if (datCheckTalk((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54)) == 0) {
        return 0;
    }
    if ((a->data->stat & 0x2000000) != 0) {
        rate = 0x64;
    } else {
        rate = 21 / a->data->enemypcnt;
    }
    if (dds3GetRandIntA(0, 0x64) < rate) {
        a->data->stat |= 0x100;
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckSenseiNego);
#endif

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543880);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438A0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438B0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005438C0);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckInotiNego);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckBinjoNego);

void CheckDeadScript(nbActionProcessData_t *a)
{
    s32 i;

    if (a->type == 5) {
        return;
    }
    for (i = 4; i < 14; i++) {
        nbFormation_t *form = &a->data->form[i];

        if ((form->stat & 1) != 0) {
            if (CheckTimeListDead(a, i) != 0) {
                s32 script = nbCheckDeadScript(form);

                if (script != 0) {
                    nbPushAction(5, form->partyindex, 0, script);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckKoketaNego);

void KyouseiPress(nbActionProcessData_t *a)
{
    nbMainProcessData_t *data = a->data;

    data->unk78 = 0;
    data->unk7A = 0;
    if (data->unkE8 != NULL) {
        dds3KillProcess(data->unkE8, 0);
    }
}

void DokodemoPoa(nbActionProcessData_t *a)
{
    s32 i;

    for (i = 4; i < 14; i++) {
        nbFormation_t *form = &a->data->form[i];

        if ((form->stat & 1) != 0) {
            if ((form->stat & 0x40) != 0) {
                nbMakeDeadVisualPacket(0, a, form, 0, 1);
                SetTimeList(a, i, 0, 30, 30);
                SetDeadPacket(a, i);
            }
        }
    }
    if (a->data->unkE8 != NULL) {
        dds3KillProcess(a->data->unkE8, 0);
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetNowActionStat);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetDamagePacket);

void SetAction_NON(void) {
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_NON);

void SetAction_COMM(nbActionProcessData_t *a)
{
    if ((a->data->stat & 0x40) != 0) {
        a->stat |= 0x80;
    } else {
        nbDelAllKoukaPacketType(0x26);
    }
    nbPanelAllShow();
    nbPanelPressGaugeShow();
    nbInitCommSelProcess(a->data, a->party, a);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_COMM);

void SetAction_AI(nbActionProcessData_t *a)
{
    a->work->nowcommand = -1;
    a->work->nowindex = 0;
    nbRunAi(a);
}

s32 CheckAction_AI(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 7);
    return 1;
}

void SetAction_AISCR(nbActionProcessData_t *a)
{
    datUnitWork_t *work = a->work;
    u16 id = work->id;

    work->nowcommand = -1;
    work->nowindex = 0;
    nbInitAiScript(a, D_003989E0[id].aiscript);
}

s32 CheckAction_AISCR(nbActionProcessData_t *a)
{
    if (dds3ExistProcess(a->data->aiScriptProcess) != 0) {
        return 0;
    }
    a->data->aiScriptProcess = NULL;
    if (a->data->negoresult == 0) {
        SetNextActionSeq(a, 7);
    } else {
        ClearTimeList(a);
        SetHpForTimeList(a);
        SetNextActionSeq(a, 10);
    }
    return 1;
}

void SetAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    ClearTimeList(a);
    SetHpForTimeList(a);
    nbInitAiScript(a, a->adata);
}

s32 CheckAction_DEADSCRIPT(nbActionProcessData_t *a)
{
    if (dds3ExistProcess(a->data->aiScriptProcess) == 0) {
        a->data->aiScriptProcess = NULL;
        nbMakeUnitFadeOutPacket(0, a, a->form, 0, 10, 10, 10);
        nbAddReturnKoukaPacket(0x19, a, a->party, 1);
        SetTimeList(a, a->form->formindex, 0, 30, 30);
        SetNextActionSeq(a, 0x12);
        return 1;
    }
    return 0;
}

void SetAction_BAD(nbActionProcessData_t *a)
{
    nbPanelAllShow();
    nbPanelPressGaugeShow();
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_BAD);

void SetAction_BADWAIT(nbActionProcessData_t *a)
{
    a->stat |= 0x20;
    if ((a->data->stat & 8) != 0) {
        nbCameraSetState(0x19);
        return;
    }
    nbCameraSetState(3);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_BADWAIT);

void SetAction_TARSEL(nbActionProcessData_t *a)
{
    SetTarget_nbActionProcess(a);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_TARSEL);

void SetAction_STKSEL(nbActionProcessData_t *a)
{
    a->select = 0;
    nbPanelStockSelectInit();
    nbPanelStockSelect(a->data, 0);
    StkSelId = -1;
}

/* NON_MATCHING: 9 of 58 words differ (first at +0x38). */
#ifdef NON_MATCHING
s32 CheckAction_STKSEL(nbActionProcessData_t *a)
{
    nbFormation_t *form;
    s32 i;

    switch (StkSelId) {
    default:
        nbPanelStockSelect(a->data, 0);
        nbCloseCommSel();
        a->select = 1 << (StkSelId + 0x10);
        SetNextActionSeq(a, 7);
        a->stat |= 0x400;
        nbPlaySe(8);
        return 1;
    case -1:
        StkSelId = nbPanelStockSelect(a->data, 1);
        break;
    case -2:
        form = a->data->form;
        for (i = 0xF; i >= 0; i--) {
            form->stat &= ~8;
            form++;
        }
        nbSetCommSelStat(2);
        a->seq = 1;
        nbPlaySe(0xA);
        nbPanelStockEnd();
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_STKSEL);
#endif

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_READY);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_SKILL);

s32 CheckAction_SKILL(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_NEGO(nbActionProcessData_t *a)
{
    s32 p = -1;
    s32 e = -1;
    s32 i = 0;
    nbFormation_t *form = a->data->form;

    do {
        u32 stat = form[i].stat;

        if ((stat & 1) != 0) {
            if ((stat & 8) != 0) {
                if ((stat & 0x100) != 0) {
                    p = i;
                    break;
                }
                if ((stat & 0x200) != 0) {
                    e = i;
                    break;
                }
            }
        }
        i++;
    } while (i < 0x10);
    if ((a->form->stat & 0x100) != 0) {
        p = a->form->formindex;
    } else if ((a->form->stat & 0x200) != 0) {
        e = a->form->formindex;
    }
    printf(D_00543FA8, p, e);
    nbPanelDraw(0);
    a->data->negopformindex = p;
    a->data->negoeformindex = e;
    nbInitNegoProcess(a, p, e, a->negotype, a->work->nowindex);
}

s32 CheckAction_NEGO(nbActionProcessData_t *a)
{
    if (a->data->negoProcess == NULL) {
        printf(D_00543FB8, a->data->negoresult);
        SetNextActionSeq(a, 10);
        nbPanelDraw(1);
        if (datCheckPlayerDead() != 0) {
            a->data->negoresult = 8;
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E20);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E38);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E58);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E78);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543E98);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543EB8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543EE8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F00);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F40);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", jtbl_00543F80);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543FA8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00543FB8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_NEGORESULT);

s32 CheckAction_NEGORESULT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_WAIT);

s32 CheckAction_WAIT(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_ESCAPE(nbActionProcessData_t *a)
{
    if (a->data->esccnt < 255) {
        a->data->esccnt++;
    }
    nbMakePressPacket(0, a, -10.0f);
    nbMakeNewPressPacket(0, a, 8, a->form);
    if (nbCheckEscape(a->data) != 0) {
        a->data->endtype = 3;
        return;
    }
    CheckKoketaNego(a);
}

/* NON_MATCHING: 9 of 57 words differ (first at +0x1C). */
#ifdef NON_MATCHING
s32 CheckAction_ESCAPE(nbActionProcessData_t *a)
{
    switch ((u16)((u16)a->seqtime / 15)) {
    case 1:
        if (a->data->endtype == 3) {
            goto out;
        }
        a->packettime = 0;
        break;
    case 2:
        if (a->data->endtype != 3) {
            goto out;
        }
        a->packettime = 0x1E;
        dds3FadeOut(0xFF, 0xFF, 0xFF, 0x14);
        nbAddSePacket(0, a, 0x10000);
        SetTimeList(a, a->form->formindex, 0, 0x11, 0x11);
        break;
    default:
        goto out;
    }
    SetNextActionSeq(a, 0x12);
    return 1;
out:
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_ESCAPE);
#endif

void SetAction_RETURN(nbActionProcessData_t *a)
{
    printf(D_00544020);
    if ((a->data->stat & 8) != 0) {
        nbCameraSetState(0x15);
    }
    nbMakeReturnPacket(0, a, a->party);
    SetTimeList(a, a->form->formindex, 0, 40, 40);
    nbMakePressPacket(10, a, -0.5f);
    nbMakeNewPressPacket(10, a, 9, a->form);
}

s32 CheckAction_RETURN(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

void SetAction_INVISIBLE(void) {
}

s32 CheckAction_INVISIBLE(nbActionProcessData_t *a)
{
    SetNextActionSeq(a, 0x12);
    return 1;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544020);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetAction_CHECK1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", CheckAction_CHECK1);

void SetAction_CHECK3(void) {
}

s32 CheckAction_CHECK3(nbActionProcessData_t *a)
{
    if (nbGetUniqueIdKoukaPacketCnt(a) > 0) {
        return 0;
    }
    SetNextActionSeq(a, 0x15);
    return 1;
}

void SetAction_ATO(void) {
}

s32 CheckAction_ATO(nbActionProcessData_t *a)
{
    if (a->hEffBTL != NULL) {
        effBTLDelete(a->hEffBTL);
        a->hEffBTL = NULL;
    }
    SetNextActionSeq(a, 0x16);
    return 1;
}

void SetAction_END(nbActionProcessData_t *a)
{
    if ((a->stat & 0x10000) != 0) {
        printf(D_00544070);
    }
    if ((a->stat & 0x20000) != 0) {
        printf(D_00544088);
    }
    if ((a->stat & 0x40000) != 0) {
        printf(D_005440A0);
    }
    if ((a->stat & 0x80000) != 0) {
        printf(D_005440B0);
    }
    if ((a->stat & 0x100000) != 0) {
        printf(D_005440C0);
    }
    if ((a->stat & 0x200000) != 0) {
        printf(D_005440D8);
    }
}

s32 CheckAction_END(nbActionProcessData_t *a)
{
    return 1;
}

s32 nbGetActionSeq(dds3ProcessID_t *id)
{
    nbActionProcessData_t *a;

    if (id == NULL) {
        return 0;
    }
    a = dds3GetProcessData(id);
    return a->seq;
}

s32 nbGetPrevActionSeq(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 i = data->unkE4 - 1;
    dds3ProcessID_t *id;

    if (i < 0) {
        i = 3;
    }
    id = data->unkD4[i];
    if (id == NULL) {
        return -1;
    }
    return nbGetActionSeq(id);
}

void SetActionSeq(nbActionProcessData_t *a, s32 seq)
{
    a->seq = seq;
    a->nextseq = 0;
    a->seqtime = 0;
    a->seqcnt = 0;
    ActionFuncTable[(s8)seq].set(a);
}

void SetNextActionSeq(nbActionProcessData_t *a, s32 nextseq)
{
    a->nextseq = nextseq;
}

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544070);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544088);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440A0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440B0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440C0);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440D8);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005440E8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbActionProcess1);

void nbActionShutdown(dds3ProcessID_t *id)
{
    nbActionProcessData_t *a = dds3GetProcessData(id);
    dds3ProcessID_t **p = a->data->unkD4;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*p == id) {
            *p = NULL;
        }
        p++;
    }
    if (a->hEffBTL != NULL) {
        effBTLDelete(a->hEffBTL);
    }
    sdfChipFreeDirect(dds3GetProcessData(id));
}

nbActionProcessData_t *InitActionProcessData(nbMainProcessData_t *data, s32 type, s32 partyindex,
                                            s32 target, s32 adata)
{
    nbActionProcessData_t *a = sdfChipAllocDirect(0x1A4);
    struct nbParty_t *party;

    memset(a, 0, 0x1A4);
    a->seq = 0;
    a->data = data;
    party = &data->party[partyindex];
    a->party = party;
    if ((party->flag & 8) != 0) {
        a->form = NULL;
    } else {
        a->form = &data->form[party->formindex];
    }
    a->work = nbGetUnitWorkPointer(data, a->party);
    a->partyindex = partyindex;
    a->type = type;
    a->target = target;
    a->adata = adata;
    a->press = -1.0f;
    a->newpresstype = 0;
    ClearTimeList(a);
    return a;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbInitActionProcess);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbAddOrder);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSortOrder);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetOrderSide);

void nbSetOrder(s32 index)
{
    if ((nbGetMainProcessData()->stat & 8) == 0) {
        nbSetOrderSide(0);
        return;
    }
    nbSetOrderSide(1);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetNextOrder);

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
    if (dpdiv(sdfFloatToULong(viewDistance), 16.0) <= 0)
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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbLoadKoukaPos);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawRotatedModelX);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawRotatedModel);

void nbGetUnitMarkPos(s32 type, nbFormation_t *form, sdfDrawTag_t *tag)
{
    s32 pos[4];

    SDF_VU0_LOADVECTOR(&form->pos);
    if ((form->stat & 1) != 0) {
        sdfModelGetMarkPos(form->hunit->owner->inner, 0);
    }
    if (nbClipEffectNum(pos) != 0) {
        nbDrawTargetCursor(type, pos[0] >> 4, pos[1] >> 3);
    }
}

void nbDrawPanelSprite(sdfTexHandle_t *th, s32 no, s32 x, s32 y, u32 col)
{
    nbPanelUV_t uv;

    uv.u = no * 13 + 1;
    uv.v = 1;
    nbPnlSprite(x >> 4, y >> 3, 0xC, 0x12, uv.u, uv.v, 0xC, 0x12, col, th);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbDrawKoukaSprite);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544130);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544140);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetTargetSkill);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetNormalSkill0);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetNakama);

void nbSetDevilNameMes(s32 hmsg, s32 no, datUnitWork_t *work)
{
    if (work->id == 0) {
        itfMesMngSetVar(hmsg, 0, 0, 10);
        return;
    }
    itfMesMngSetVar(hmsg, no, work->id, 1);
}

void nbSetJyokyoMes(nbActionProcessData_t *a, s32 jyokyomes, s32 varid, s32 varind)
{
    a->jyokyomes = nbGetJyokyoMes(a->data, jyokyomes);
    a->jyo_varid = varid;
    a->jyo_varind = varind;
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

s32 nbAppendEnemyDevilWork(nbMainProcessData_t *data, s32 devilid, s32 formindex)
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
    datSetDevilWork(devilid, work);
    info = &datDevilTable[devilid];
    work->flag |= 2;
    work->maxhp = info->maxhp;
    work->hp = info->hp;
    work->maxmp = info->maxmp;
    work->mp = info->mp;
    return i;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbGetRandamStock);

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

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetupBattleUnits);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbSetPlayerStartData);

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

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_00544170);

s32 GetCommListCnt(nbCommSelProcessData_t *s, s32 type)
{
    s32 cnt[6] = {4, 4, 4, 3, 4, 4};

    if (type == 3) {
        return s->cnt;
    }
    return cnt[type];
}

nbCommSelProcessData_t *GetCommSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005441A8));
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelCursor);

INCLUDE_RODATA(const s32, "newbattle/nbStageProcess", D_005441A8);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelLabel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispPanelSitaji);

/* NON_MATCHING: 3 of 56 words differ (first at +0x28). */
#ifdef NON_MATCHING
void DispComm1PanelComm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 no = d;

    if (no <= 0x8003) {
        if (no >= 0x8000) {
            return;
        }
    }
    switch (datSkillTable.normal[no].costtype) {
    case 1:
        nbPnlSprite(ox + 0x9B, oy + 5, 0x14, 0xB, 0xC, 0x4E, 0x14, 0xB, col,
                    s->data->pnltex[0]);
        break;
    case 2:
        nbPnlSprite(ox + 0x9B, oy + 5, 0x14, 0xB, 0x21, 0x4E, 0x14, 0xB, col,
                    s->data->pnltex[0]);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelComm);
#endif

void DispComm1PanelTalk(void) {
}

void DispComm1PanelItem(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    nbPnlSprite(ox + 0x8C, oy + 6, 9, 9, 1, 0x4E, 9, 9, col, s->data->pnltex[0]);
}

void DispHpPanel(sdfTexHandle_t *th, datUnitWork_t *w, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 bar;

    nbPnlSprite(ox, oy, 0x14, 0xB, 0xC, 0x4E, 0x14, 0xB, col, th);
    oy += 0xC;
    nbPnlSprite(ox, oy, 0x32, 6, 1, 0x14, 0x31, 5, col, th);
    if (w->maxhp == 0) {
        return;
    }
    bar = w->hp * 49 / w->maxhp;
    if (bar == 0) {
        return;
    }
    nbPnlSprite(ox, oy, bar, 6, 1, 0xD, bar, 5, col, th);
}

void DispMpPanel(sdfTexHandle_t *th, datUnitWork_t *w, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 bar;

    nbPnlSprite(ox, oy, 0x14, 0xB, 0x21, 0x4E, 0x14, 0xB, col, th);
    oy += 0xC;
    nbPnlSprite(ox, oy, 0x32, 6, 1, 0x14, 0x31, 5, col, th);
    if (w->maxmp == 0) {
        return;
    }
    bar = w->mp * 49 / w->maxmp;
    if (bar == 0) {
        return;
    }
    nbPnlSprite(ox, oy, bar, 6, 1, 0x1B, bar, 5, col, th);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispComm1PanelSummon);

/* NON_MATCHING: 29 of 97 words differ (first at +0x8). */
#ifdef NON_MATCHING
void DispCommCursor(s32 ox, s32 oy, s32 a)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *ot;
    s32 xb;
    s32 x1;
    s32 alpha = 0x46;
    s32 y0, y1;
    u32 c0, c1;

    if (a != 0x80) {
        alpha = a * 0.0078125f * 70.0f;
    }
    xb = ox << 4;
    x1 = xb + 0x7630;
    ox = xb + 0x7000;
    list = dds3FontReqAllocDrawTag();
    y0 = (oy << 3) + 0x7900;
    y1 = (oy << 3) + 0x7998;
    c0 = (alpha << 24) | 0x8CBE1B;
    c1 = (alpha << 24) | 0x9B4400;
    sdfPrimLinkG4(list, 0x14C, ox, y0, c0, x1, y0, c1, ox, y1, c0, x1, y1, c1, -1, NULL);
    ox = x1;
    sdfPrimLinkG4(list, 0x14C, ox, y0, c1, xb + 0x7C60, y0, 0xBF0013, ox, y1, c1, xb + 0x7C60, y1,
                  0xBF0013, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, list);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommCursor);
#endif

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispSummonCursor);

void DispComm1PanelCursor(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 a = col >> 24;

    switch (type) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
        DispCommCursor(ox - 5, oy, a);
        break;
    case 3:
        DispSummonCursor(ox - 0xB, oy - 1, a);
        a = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispHelpPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommandPanel);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", AnimeCommandList);

void DispComm1Summon(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                     s32 listidx, s32 col2)
{
    char buf[16];
    datUnitWork_t *w = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + d;
    void *frq;
    void *frq2;
    u32 c;
    s32 per;
    u32 a;
    s32 id = w->id;

    cmpSetSkillListRows(0x13);
    frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text2[id], NULL);
    if (w->level < 0x64) {
        sprintf(buf, D_00551130, w->level);
    } else {
        sprintf(buf, D_00551130, 0x63);
    }
    frq = frwGetSmallNumFontFRQ((ox + 0x8C) << 4, (oy + 4) << 3, 0xFF0051, col, buf, frq);
    c = 0x754B3D80;
    per = w->hp * 100 / w->maxhp;
    if (per >= 0x19) {
        if (per < 0x32) {
            c = 0xB4A06480;
        } else {
            c = 0xA09DC380;
        }
    }
    a = col & 0xFF;
    c = (c & 0xFFFFFF00) | a;
    sprintf(buf, D_00551130, w->hp);
    frq2 = frwGetSmallNumFontFRQ((ox + 0x17) << 4, (oy + 0x13) << 3, 0xFF0051, c, buf, NULL);
    frDrawFontEx(frq2, 1);
    frDestroyFontRequestAfter(frq2);
    c = a | (0xA09DC380 & 0xFFFFFF00);
    sprintf(buf, D_00551130, w->mp);
    frq2 = frwGetSmallNumFontFRQ((ox + 0x52) << 4, (oy + 0x13) << 3, 0xFF0051, c, buf, NULL);
    frDrawFontEx(frq2, 1);
    frDestroyFontRequestAfter(frq2);
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Item(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
{
    char buf[16];
    void *frq;

    cmpSetSkillListRows(0x13);
    frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, pStr[d], NULL);
    sprintf(buf, D_00551130, ((u8 *)(u32)d + (u32)DDS3_GBWK)[0x1928]);
    frq = frwGetSmallNumFontFRQ((ox + 0x8E) << 4, (oy + 4) << 3, 0xFF0051, col, buf, frq);
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Talk(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
{
    void *frq;

    cmpSetSkillListRows(0x13);
    if (d == 0x8004) {
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x205], NULL);
    } else {
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[d], NULL);
    }
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Comm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
{
    char buf[16];
    datUnitWork_t *work = nbGetUnitWorkPointer(s->data, s->my);
    void *frq;
    s32 costtype;

    cmpSetSkillListRows(0x13);
    switch (d) {
    case 0x8000:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x200], NULL);
        break;
    case 0x8001:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x201], NULL);
        break;
    case 0x8002:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x202], NULL);
        break;
    case 0x8003:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x204], NULL);
        break;
    default:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[d], NULL);
        costtype = datSkillTable.normal[d].costtype;
        if (costtype < 3) {
            if (costtype != 0) {
                sprintf(buf, D_00551130, datGetSkillCost(work, d));
                frq = frwGetSmallNumFontFRQ((ox + 0x78) << 4, (oy + 4) << 3, 0xFF0051, col, buf,
                                            frq);
            }
        }
        break;
    }
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispHelp1);

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", DispCommandList2);

s32 CheckSelectError(nbCommSelProcessData_t *s)
{
    s32 type = s->nowtype;

    if (s->commcnt[type] == 0) {
        return 1;
    }
    return s->commdisable[type][s->nowcursor[type]] > 0;
}

void MemoryCommand(nbCommSelProcessData_t *s)
{
    datUnitWork_t *work = nbGetUnitWorkPointer(s->data, s->my);
    s32 type = s->nowtype;
    u16 d = s->commlist[type][s->nowcursor[type]];

    switch (type) {
    case 0:
    case 4:
    case 5:
        switch (d) {
        case 0x8000:
            work->nowcommand = 0;
            work->nowindex = 0;
            break;
        case 0x8001:
            work->nowindex = 0;
            work->nowcommand = 2;
            break;
        case 0x8002:
            work->nowindex = 0;
            work->nowcommand = 3;
            break;
        case 0x8003:
            work->nowindex = 0;
            work->nowcommand = 7;
            break;
        default:
            work->nowindex = d;
            work->nowcommand = 1;
            break;
        }
        break;
    case 1:
        if (d == 0x8004) {
            work->nowcommand = 4;
            work->nowindex = 0x180;
            break;
        }
        work->nowindex = d;
        work->nowcommand = 4;
        break;
    case 2:
        work->nowindex = d;
        work->nowcommand = 5;
        break;
    case 3:
        work->nowindex = d;
        work->nowcommand = 6;
        break;
    }
}

s32 GetNextCommType(nbCommSelProcessData_t *s, s32 dir)
{
    s32 type = s->nowtype;
    s32 i;

    i = 0;
    do {
        type = (dir != 0 ? type + 1 : type + 5) % 6;
        if (s->commcnt[type] > 0) {
            return type;
        }
        i++;
    } while (i < 6);
    return type;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SelectCommandList);

void SelectCommandListAuto(nbCommSelProcessData_t *s)
{
    if (CheckSelectError(s) == 0) {
        s->stat = 3;
        MemoryCommand(s);
    }
}

s32 nbGetCommSelStat(void)
{
    return GetCommSelProcessData()->stat;
}

void nbSetCommSelStat(s32 stat)
{
    GetCommSelProcessData()->stat = stat;
}

void nbCloseCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    s->stat = 4;
    s->unk14B4 = (s->unk14B4 | 2) & ~1;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", nbCommSelProcess1);

void nbCommSelShutdown(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s = dds3GetProcessData(id);
    nbMainProcessData_t *data = s->data;

    if (data->unkE8 == id) {
        data->unkE8 = NULL;
    }
    sdfMemFreeDirect(s->handle);
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetCommList);

void SetPrevCursor(nbCommSelProcessData_t *s, s32 type, s32 ind)
{
    s32 listcnt = GetCommListCnt(s, type);
    s32 y = ind;

    s->nowtype = type;
    s->nowcursor[type] = ind;
    s->nowlisty[type] = ind;
    if (y + listcnt >= s->commcnt[type]) {
        y -= y + listcnt - s->commcnt[type];
        s->nowlisty[type] = (y < 0) ? 0 : y;
    }
    if (s->commdisable[type][ind] != 0) {
        s->nowtype = 0;
        s->nowcursor[type] = 0;
        s->nowlisty[type] = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", SetCommCursor2);

void nbSetAutoCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    SetCommCursor2(s);
    s->unk14B4 |= 0x10;
}

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", InitCommSelProcessData);

void nbInitCommSelProcess(nbMainProcessData_t *data, struct nbParty_t *my,
                          nbActionProcessData_t *a)
{
    nbCommSelProcessData_t *s = InitCommSelProcessData(data, my, a);
    dds3ProcessID_t *id = dds3StartProcess(D_005441A8, 0x2B0E, 1, 1, nbCommSelProcess1,
                                           nbCommSelShutdown, s);

    dds3LinkProcess(data->unkBC, id);
    data->unkE8 = id;
}
INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551110);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551114);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551118);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", StkSelId);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551124);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551128);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551130);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551134);

