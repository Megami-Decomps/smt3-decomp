#include "common.h"
#include "basic.h"

extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, s32 entry);
extern void dds3EffectObjectPresetMG12(dds3Basic_t *basic);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *basic, dds3Basic_t *p0,
                                      dds3Basic_t *p1, dds3Basic_t *p2, dds3Basic_t *p3);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 stat);
extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *owner);
extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *owner,
                                                 s32 index);
extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);
extern void *dds3GetControlScene(void);
extern s32 dds3GetMapNo(void *scene);
extern void dds3AdminiCallSequence(s32 seq, s32 a, s32 b, s32 c);
extern void dds3SetActiveCameraSceneBasic(void *scene, dds3Basic_t *camera);
extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 id, f32 *pos, f32 *rot);
extern s32 dds3AllocObjectId(void);
extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);
extern u32 dds3DrawFlag;

extern void evtEffectLoopControl(dds3Basic_t *basic, s32 on);
extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *target);
extern s32 evtCheckPathStat(dds3Basic_t *basic);
extern void evtRegistBEField(s32 event, s32 resource);
extern void evtLoadArea(s32 area, s32 room);
extern void evtStageAnimeControl(s32 index, s32 value);
extern void evtFreeAllUnit(void);
extern void evtFreeStage(void);

extern void fldSetNextField(s32 command, s32 para);
extern dds3Basic_t *fldGetUnitFollowCamera(void);

extern void datDebugPrintf(const char *fmt, ...);
extern void sdfPrintf(const char *fmt, ...);
extern s32 sdfDelayedResrcCheck(void);

extern char D_0053A9D8[];
extern char D_0053AA10[];
extern char D_0053AA38[];
extern char D_0053AA88[];

extern void *dds3GetActiveScene(void);
extern void *dds3SearchSceneBasic(void *scene, s32 key, s32 type);
extern void dds3ObjectBaseOnFlag(void *basic, s32 flag);
extern void dds3ObjectBaseOffFlag(void *basic, s32 flag);
extern void dds3SetPosPoseSet(void *basic, u128 *pos);
extern void dds3SetSclPoseSet(void *basic, u128 *scl);

extern void *evtGetUnitHandle(s32 key);
extern s32 evtCheckUnitWait(void *unit);
extern void evtSetUnitMotion(void *unit, s32 motion, s32 start, s32 end, s32 mode);
extern void evtUnitLightOff(void *unit, s32 time);
extern s32 evtUnitLightAnimeStat(void *unit);

extern s32 scrGetIntPara(s32 no);
extern f32 scrGetFloatPara(s32 no);
extern void scrSetIntReturnValue(s32 value);

extern char D_0053A8C8[];
extern char D_0053A908[];

extern s32 printf(const char *fmt, ...);
extern void func_001E3C30(void *unit, s32 p1, s32 p2, s32 p3, s32 p4, s32 p5);
extern void evtLinkLightObjectBasic(void *unit, void *target, s32 no);
extern void evtUnlinkLightObjectBasic(void *unit, s32 time);
extern void evtPathStop(void *basic);
extern void evtPathStart(void *basic);
extern void evtPathLoop(void *basic, s32 on);
extern void *fldGetMatter_From_ModelRangeID(s32 id);
extern void evtLoadEventBgm(s32 event);
extern s32 evtCheckEventBgmLoaded(s32 event);
extern s32 scrGetCommandTimer(void);
extern void evtSetSkyAlpha(s32 no, s32 alpha);
extern s32 dds3ExistProcess(void *id);
extern void evtPictureDrawOn(void *id);
extern void evtPictureDrawOff(void *id);
extern void evtKillPictureProcess(void *id);

extern char D_0053AF00[];
extern char D_0053AF18[];
extern char D_0053AFE8[];
extern char D_0053B010[];

extern s32 *dds3GetActiveUnitSceneBasic(void *scene);
extern void fldSetFilterAnime(s32 on);
extern void fldSkySetAlpha(s32 alpha);
extern void fldSkySetFilter(s32 r, s32 g, s32 b);
extern s32 evtSearchEventPack(s32 no);
extern s32 evtGetEventPackStat(s32 no);
extern void *scrGetScriptHandle(void);

extern char D_0053ADE0[];

extern dds3Basic_t *dds3SceneBasicSearchName(void *scene, const char *name);
extern dds3Basic_t *dds3AddMG12EffectObjectBasic(s32 kind, const char *name);
extern void *evtStartPictureProcess2(s32 id, const char *name);
extern void dds3LinkProcess(void *parent, void *child);

extern char *scrGetStrPara(s32 no);

extern char D_0053AFA0[];
extern char D_0053AC00[];
extern char D_0053AC20[];

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} evtVec_t;

extern dds3Basic_t *dds3AddD3PEffectObjectBasic(const char *name, evtVec_t *pos, evtVec_t *scale);
extern dds3Basic_t *dds3AddBEDEffectObjectBasic(const char *name, evtVec_t *pos, evtVec_t *scale);

extern char D_0053A9A8[];

extern void evtKillEventPack(s32 no);

extern char D_0053AD70[];
extern char D_0053AD80[];
extern char D_0053ADB0[];
extern char D_0053AE00[];

void *evtSearchBasic(s32 type, s32 key);

void *evtSearchBasic(s32 type, s32 key)
{
    void *scene;

    scene = dds3GetActiveScene();
    return dds3SearchSceneBasic(scene, key, type);
}

u32 evtCommand_MODEL_PLAYER(void)
{
    void *scene;
    s32 *basic;
    s32 key;

    scene = dds3GetActiveScene();
    basic = dds3GetActiveUnitSceneBasic(scene);
    key = -1;
    if (basic != NULL) {
        key = basic[1];
    }
    scrSetIntReturnValue(key);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_DSP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_BE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_POS);

u32 evtCommand_MODEL_SHADOW_ON(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOnFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_ON2(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOnFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_SHADOW_OFF(void)
{
    void *scene;
    s32 key;
    void *unit;
    u32 ret = 1;

    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ObjectBaseOffFlag(unit, 0x400);
    dds3ObjectBaseOffFlag(unit, 0x200);
    return 1;
}

u32 evtCommand_MODEL_POS_XYZ(void)
{
    f32 pos[4];
    void *scene;
    s32 key;
    void *unit;

    memset(pos, 0, 0x10);
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    pos[0] = scrGetFloatPara(1);
    pos[1] = scrGetFloatPara(2);
    pos[2] = scrGetFloatPara(3);
    dds3SetPosPoseSet(unit, (u128 *)pos);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_ROT_XYZ);

u32 evtCommand_MODEL_SCL_XYZ(void)
{
    f32 scl[4];
    void *scene;
    s32 key;
    void *unit;

    memset(scl, 0, 0x10);
    scl[3] = 1.0f;
    scene = dds3GetActiveScene();
    key = scrGetIntPara(0);
    unit = dds3SearchSceneBasic(scene, key, 5);
    if (unit == NULL) {
        return 1;
    }
    scl[0] = scrGetFloatPara(1);
    scl[1] = scrGetFloatPara(2);
    scl[2] = scrGetFloatPara(3);
    dds3SetSclPoseSet(unit, (u128 *)scl);
    return 1;
}

u32 evt_1E50C0(void)
{
    void *unit;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), scrGetIntPara(2), scrGetIntPara(3), 0);
    printf(D_0053A8C8);
    return 1;
}

u32 evt_1E5158(void)
{
    void *unit;
    s32 motion;
    s32 start;
    s32 end;
    s32 mode;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    motion = scrGetIntPara(1);
    start = scrGetIntPara(2);
    end = scrGetIntPara(3);
    mode = scrGetIntPara(4);
    evtSetUnitMotion(unit, motion, start, end, mode);
    return 1;
}

u32 evt_1E51F8(void)
{
    void *unit;
    s32 p1;
    s32 p2;
    s32 p3;
    s32 p4;
    s32 p5;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    if (unit == NULL) {
        return 1;
    }
    p1 = scrGetIntPara(1);
    p2 = scrGetIntPara(2);
    p3 = scrGetIntPara(3);
    p4 = scrGetIntPara(4);
    p5 = scrGetIntPara(5);
    func_001E3C30(unit, p1, p2, p3, p4, p5);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtRunKutiMotion);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_KUTISET);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_KUTI);

u32 evtCommand_MODEL_WAIT(void)
{
    s32 key;
    void *unit;
    u32 ret = 1;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return ret;
    }
    return evtCheckUnitWait(unit) != 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_EXWAIT);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E54B8);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_MOVE2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_EXMOT);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_MOT);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_WAIT_HOKAN);

u32 evtCommand_MODEL_MOVE_MOT(void)
{
    s32 key;
    void *unit;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    if (unit == NULL) {
        return 1;
    }
    evtSetUnitMotion(unit, scrGetIntPara(1), 5, 7, 1);
    printf(D_0053A908);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_ROT);

u32 evtCommand_MODEL_ON(void)
{
    u32 **unit;

    unit = (u32 **)evtGetUnitHandle(scrGetIntPara(0));
    if (unit != NULL) {
        unit[35][0] &= ~1;
    }
    return 1;
}

u32 evtCommand_MODEL_OFF(void)
{
    u32 **unit;

    unit = (u32 **)evtGetUnitHandle(scrGetIntPara(0));
    if (unit != NULL) {
        unit[35][0] |= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_LIGHT_ON);

u32 evtCommand_MODEL_LIGHT_OFF(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnitLightOff(unit, time);
    return 1;
}

u32 evtCommand_MODEL_LIGHT_WAIT(void)
{
    s32 key;
    void *unit;
    s32 stat;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    stat = evtUnitLightAnimeStat(unit);
    return stat == 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_LIGHT_RGB);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A8C8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A908);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_LIGHT_DIR);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_RGB);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MODEL_A);

u32 evtCommand_MODEL_LIGHT_LINK(void)
{
    s32 key;
    void *unit;
    void *target;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    target = evtSearchBasic(9, scrGetIntPara(2));
    if (target == NULL) {
        return 1;
    }
    evtLinkLightObjectBasic(unit, target, scrGetIntPara(1));
    return 1;
}

u32 evtCommand_MODEL_LIGHT_UNLINK(void)
{
    s32 key;
    void *unit;
    s32 time;

    key = scrGetIntPara(0);
    unit = evtGetUnitHandle(key);
    time = scrGetIntPara(1);
    evtUnlinkLightObjectBasic(unit, time);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_BE);

u32 evtCommand_EFFECT_LOAD(void)
{
    evtVec_t pos;
    evtVec_t scale;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    basic = dds3AddD3PEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(D_0053A9A8);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

u32 evtCommand_EFFBED_LOAD(void)
{
    evtVec_t pos;
    evtVec_t scale;
    dds3Basic_t *basic;

    memset(&pos, 0, sizeof(pos));
    memset(&scale, 0, sizeof(scale));
    scale.w = 1.0f;
    basic = dds3AddBEDEffectObjectBasic(scrGetStrPara(0), &pos, &scale);
    if (basic == NULL) {
        printf(D_0053A9A8);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A990);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A9A8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053A9D8);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFBED_BE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFBED_BE_EVO);

u32 evtCommand_EFFBED_START(void)
{
    s32 key;
    dds3Basic_t *basic;
    u32 entry;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        entry = scrGetIntPara(1);
        if (entry >= 3U) {
            datDebugPrintf(D_0053AA10, entry);
            return 1;
        } else {
            dds3StartBEDEffectObjectBasic(basic, entry);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCalcEffBedScale);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFBED_START_EVO);

u32 evtCommand_EFFMG1_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(1, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AA38);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AA10);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AA38);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG1_BE);

u32 evtCommand_EFFMG2_LOAD(void)
{
    dds3Basic_t *basic;

    basic = dds3AddMG12EffectObjectBasic(2, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AA88);
        printf(D_0053A9D8, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        dds3EffectObjectBasicStatOn(basic, 1);
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AA88);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFMG2_BE);

u32 evtCommand_EFFMG2_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_PRESET(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3EffectObjectPresetMG12(basic);
    }
    return 1;
}

u32 evtCommand_EFFECT_CLS(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        dds3DeleteSceneBasicData(basic);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_POS_XYZ);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_ROT_XYZ);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EFFECT_POS);

s32 evtCommand_EFFMG1_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG1_POS mg1 ID error!
");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    if (point0 == NULL || point1 == NULL) {
        datDebugPrintf("EFFMG1_POS point ID not found!
");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, NULL, NULL) == 0) {
        datDebugPrintf("EFFMG1_POS set error!
");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFMG2_POS(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *point0;
    dds3Basic_t *point1;
    dds3Basic_t *point2;
    dds3Basic_t *point3;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFMG2_POS mg2 ID error!
");
        return 1;
    }
    point0 = evtSearchBasic(7, scrGetIntPara(1));
    point1 = evtSearchBasic(7, scrGetIntPara(2));
    point2 = evtSearchBasic(7, scrGetIntPara(3));
    point3 = evtSearchBasic(7, scrGetIntPara(4));
    if (point0 == NULL || point1 == NULL || point2 == NULL || point3 == NULL) {
        datDebugPrintf("EFFMG2_POS point ID not found!
");
        return 1;
    }
    if (dds3EffectObjectSetMG12Pos(effect, point0, point1, point2, point3) == 0) {
        datDebugPrintf("EFFMG2_POS set error!
");
        return 1;
    }
    return 1;
}

s32 evtCommand_EFFECT_ON(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 1);
        dds3EffectObjectBasicStatOn(basic, 1);
    }
    return 1;
}

s32 evtCommand_EFFECT_LINK(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObject(effect, owner);
    return 1;
}

s32 evtCommand_EFFECT_LINK2(void)
{
    dds3Basic_t *effect = evtSearchBasic(7, scrGetIntPara(0));
    dds3Basic_t *owner;
    s32 type;

    if (effect == NULL) {
        return 1;
    }
    type = 4;
    do {
        owner = evtSearchBasic(type, scrGetIntPara(1));
        type++;
    } while (type < 10 && owner == NULL);
    if (owner == NULL) {
        return 1;
    }
    dds3EffectObjectBasicLinkObjectIndex(effect, owner, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_EFFECT_UNLINK(void)
{
    dds3Basic_t *basic;

    basic = evtSearchBasic(7, scrGetIntPara(0));
    if (basic == NULL) {
        return 1;
    }
    dds3EffectObjectBasicUnlinkObject(basic);
    return 1;
}

s32 evtCommand_EFFECT_OFF(void)
{
    s32 key;
    dds3Basic_t *basic;

    key = scrGetIntPara(0);
    basic = evtSearchBasic(7, key);
    if (basic != NULL) {
        evtEffectLoopControl(basic, 0);
    }
    return 1;
}

s32 evtCommand_EFFECT_PATH_MOVE(void)
{
    dds3Basic_t *effect;
    dds3Basic_t *path;

    effect = evtSearchBasic(7, scrGetIntPara(0));
    if (effect == NULL) {
        datDebugPrintf("EFFECT_PATH_MOVE not found eff!
");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        datDebugPrintf("EFFECT_PATH_MOVE not found path!
");
        return 1;
    }
    evtPathLinkAndMake(path, effect);
    return 1;
}

s32 evtCommand_EFFECT_PATH_WAIT(void)
{
    s32 key = scrGetIntPara(0);
    dds3Basic_t *basic = evtSearchBasic(7, key);
    s32 stat;

    if (basic == NULL) {
        datDebugPrintf("EFFECT_PATH_WAIT not found eff!
");
        return 1;
    }
    stat = evtCheckPathStat(basic);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

u32 evtCommand_SEARCH_ID(void)
{
    void *scene;
    dds3Basic_t *basic;

    scene = dds3GetControlScene();
    basic = dds3SceneBasicSearchName(scene, scrGetStrPara(0));
    if (basic == NULL) {
        printf(D_0053AC00, scrGetStrPara(0));
        datDebugPrintf(D_0053AC20, scrGetStrPara(0));
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

s32 evtCommand_RTRN_TITLE(void)
{
    dds3AdminiCallSequence(2, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_STAFF);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_ENDSAVE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_COMBINE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_RECOVER);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_SHOP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_RAG);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_SAVE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_PUZZLEBOY);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AC00);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AC20);

s32 evtCommand_CALL_NEXT(void)
{
    s32 para0 = scrGetIntPara(0);
    s32 para1 = scrGetIntPara(1);
    s32 para2 = scrGetIntPara(2);
    s32 command;

    printf("CALL_NEXT(%d,%d,%d)
", para0, para1, para2);
    command = scrGetIntPara(1);
    fldSetNextField(command, scrGetIntPara(2));
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_EVENT);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_EXIT_EVENT);

INCLUDE_ASM(const s32, "event/evtCommand", CallBattleSub);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_BATTLE);

INCLUDE_ASM(const s32, "event/evtCommand", CallBattleSub2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_BATTLE2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_RESTORE_BATTLE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CALL_FIELD3);

s32 evtCommand_FIELD_BE(void)
{
    s32 event;
    s32 resource;

    sdfPrintf("FIELD_BE start
");
    event = scrGetIntPara(0);
    resource = scrGetIntPara(1);
    evtRegistBEField(event, resource);
    sdfPrintf("FIELD_BE end
");
    return 1;
}

s32 evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    s32 mapno;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    mapno = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != mapno) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}

s32 evtCommand_FIELD_ANIME_START(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(0, value);
    return 1;
}

s32 evtCommand_FIELD_ANIME_STOP(void)
{
    s32 value;

    value = scrGetIntPara(0);
    evtStageAnimeControl(1, value);
    return 1;
}

s32 evtCommand_FIELD_CLS(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown !!
");
    datDebugPrintf("field shutdown.
");
    return 1;
}

s32 evtCommand_FIELD_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    evtFreeStage();
    while (sdfDelayedResrcCheck()) {
    }
    dds3DrawFlag |= 0x2000000;
    printf("event stage shutdown2 !!
");
    datDebugPrintf("field shutdown2.
");
    return 1;
}

s32 evtCommand_UNIT_CLS(void)
{
    evtFreeAllUnit();
    printf("unit all clear !!
");
    datDebugPrintf("unit all clear.
");
    return 1;
}

s32 evtCommand_UNIT_CLS_STOP(void)
{
    evtFreeAllUnit();
    while (sdfDelayedResrcCheck()) {
    }
    printf("unit all clear2 !!
");
    datDebugPrintf("unit all clear2.
");
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_SEL);

u32 evtCommand_CAM_SEL_FOVY(void)
{
    dds3Basic_t *camera;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
        *(u32 *)((u8 *)camera->workdata + 0x88) |= 1;
    }
    if (camera == NULL) {
        return 1;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
    return 1;
}

s32 evtCommand_CAM_PATH_MOVE(void)
{
    dds3Basic_t *camera;
    dds3Basic_t *path;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        printf("CAM_PATH_MOVE error!
");
        return 1;
    }
    path = evtSearchBasic(0x10, scrGetIntPara(1));
    if (path == NULL) {
        printf("CAM_PATH_MOVE error!
");
        return 1;
    }
    if (evtPathLinkAndMake(path, camera) == 0) {
        printf("CAM_PATH_MOVE error!
");
        return 1;
    }
    return 1;
}

s32 evtCommand_CAM_PATH_WAIT(void)
{
    dds3Basic_t *camera;
    s32 stat;

    if (scrGetIntPara(0) < 0) {
        camera = fldGetUnitFollowCamera();
    } else {
        camera = evtSearchBasic(4, scrGetIntPara(0));
    }
    if (camera == NULL) {
        return 1;
    }
    stat = evtCheckPathStat(camera);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

s32 evtCommand_CAM_CREATE(void)
{
    f32 pos[4];
    f32 rot[4];
    s32 id;

    memset(pos, 0, 0x10);
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    id = dds3AllocObjectId();
    dds3GetRadiusPoseSet(dds3AddCameraObjectBasicTypeFree(id, pos, rot), 1.0f);
    scrSetIntReturnValue(id);
    return 1;
}

s32 evtCommand_CAM_CLS(void)
{
    s32 key;
    dds3Basic_t *camera;

    key = scrGetIntPara(0);
    camera = evtSearchBasic(4, key);
    if (camera == NULL) {
        return 1;
    }
    dds3DeleteSceneBasicData(camera);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_POS_XYZ);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_ROT_XYZ);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CAM_ROT_Q);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AD70);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AD80);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_OBJ_PATH_MOVE);

s32 evtCommand_OBJ_PATH_WAIT(void)
{
    void *obj;
    s32 stat;

    obj = evtSearchBasic(6, scrGetIntPara(0));
    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            printf(D_0053AD70, scrGetIntPara(0));
            printf(D_0053AD80, scrGetIntPara(1));
            printf(D_0053ADB0);
            return 1;
        }
    }
    stat = evtCheckPathStat(obj);
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MOON_START);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_MOON_STOP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_GET_MOON);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_GET_MOON16);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_CHG_MOON);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADB0);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_LOAD_BE);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053ADE0);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_READ_BE);

u32 evtCommand_CHK_BE(void)
{
    s32 no;
    s32 ret;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() != NULL && evtSearchEventPack(no) != 0
            && evtGetEventPackStat(no) == 2) {
        datDebugPrintf(D_0053ADE0, no);
        ret = 1;
    } else {
        ret = 0;
    }
    scrSetIntReturnValue(ret);
    return 1;
}



u32 evtCommand_DEL_BE(void)
{
    s32 no;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() == NULL) {
        return 1;
    }
    if (evtSearchEventPack(no) == 0) {
        return 1;
    }
    datDebugPrintf(D_0053AE00, no);
    evtKillEventPack(no);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE00);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_LOAD);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE28);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE70);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AE88);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_RUN2);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_LOAD_END_WAIT);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PMV_RUN);

s32 evtCommand_PATH_STOP(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathStop(obj);
    return 1;
}

s32 evtCommand_PATH_START(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathStart(obj);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_RATE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_PATH_DIR);

s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 1);
    return 1;
}

s32 evtCommand_PATH_LOOP_OFF(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 0);
    return 1;
}

s32 evtCommand_LIGHT_PATH_MOVE(void)
{
    void *light;

    light = evtSearchBasic(9, scrGetIntPara(0));
    if (evtPathLinkAndMake(evtSearchBasic(0x10, scrGetIntPara(1)), light) != 0) {
        return 1;
    }
    printf(D_0053AF00);
    datDebugPrintf(D_0053AF18);
    return 1;
}

s32 evtCommand_LIGHT_PATH_WAIT(void)
{
    s32 stat;

    stat = evtCheckPathStat(evtSearchBasic(9, scrGetIntPara(0)));
    switch (stat) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AEB8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF00);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF18);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AF38);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_BE);

u32 evtCommand_TEX_LOAD(void)
{
    void *pw;
    void *process;

    pw = scrGetScriptHandle();
    if (pw == NULL) {
        return 1;
    }
    if (*(s32 *)((u8 *)pw + 0xE4) == 0) {
        printf(D_0053AFA0);
        return 1;
    }
    process = evtStartPictureProcess2(0x2AFE, scrGetStrPara(0));
    dds3LinkProcess(*(void **)((u8 *)pw + 0xE4), process);
    scrSetIntReturnValue((s32)process);
    return 1;
}

u32 evtCommand_TEX_ON(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOn(id);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtPictureDrawOff(id);
    }
    return 1;
}

u32 evtCommand_TEX_CLS(void)
{
    void *id;

    id = (void *)scrGetIntPara(0);
    if (dds3ExistProcess(id) != 0) {
        evtKillPictureProcess(id);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_SWAP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_OT);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_PRI);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_SIZE);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_POS);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_ALP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_ZOOM);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_TEX_STAT);

u32 evtCommand_SET_SKY(void)
{
    s32 r;
    s32 g;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    r = scrGetIntPara(0);
    g = scrGetIntPara(1);
    fldSkySetFilter(r, g, scrGetIntPara(2));
    return 1;
}

u32 evtCommand_SET_SKY_A(void)
{
    s32 alpha;

    alpha = scrGetIntPara(1);
    if (alpha < -255) {
        datDebugPrintf(D_0053AFE8);
        alpha = -255;
    }
    if (alpha > 255) {
        datDebugPrintf(D_0053B010);
        alpha = 255;
    }
    evtSetSkyAlpha(scrGetIntPara(0), alpha);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BOKASI_ON);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BOKASI_OFF);

u32 evtCommand_LOAD_SOUND_E(void)
{
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else if (evtCheckEventBgmLoaded(scrGetIntPara(0)) != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BGM_PLAY_E);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BGM_TRANS_E);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_FADEOUT_E);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BGM_VOL_DOWN);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_BGM_VOL_UP);

INCLUDE_ASM(const s32, "event/evtCommand", evtCommand_SE_PLAY_E);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E9168);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E91A8);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E91E8);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E9220);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E9248);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E9270);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E9298);

INCLUDE_ASM(const s32, "event/evtCommand", evt_1E92D8);

u32 evt_1E92F8(void)
{
    if (scrGetIntPara(0) <= 0) {
        dds3DrawFlag |= 0x2000000;
        return 1;
    }
    if (scrGetCommandTimer() < scrGetIntPara(0)) {
        dds3DrawFlag |= 0x2000000;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", evtDrawDeadMoonSprite);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AFA0);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053AFE8);

INCLUDE_RODATA(const s32, "event/evtCommand", D_0053B010);

INCLUDE_ASM(const s32, "event/evtCommand", nbLoadDeadMoonTex);

INCLUDE_ASM(const s32, "event/evtCommand", nbFreeDeadMoonTex);
INCLUDE_SDATA(const s32, "event/evtCommand", D_0054F754);

INCLUDE_SDATA(const s32, "event/evtCommand", DeadMoonTmx);

