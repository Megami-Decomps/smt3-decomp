#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"
#include "field.h"

/* The quadword copy of a formation position: 16-byte aligned, so ee-gcc moves
 * it with one lq/sq pair. */
typedef struct nbQuadVector_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) nbQuadVector_t;

/* One object: StageData is its byte at +1 and nbStageParam its word at +8. */
typedef struct nbStageState_t {
    u8 stat;
    u8 data;
    u8 pad02[2];
    s16 major;
    u16 minor;
    u32 param;
} nbStageState_t;

extern char D_00543618[];

extern char D_00543688[];

extern char D_00543698[];

extern u8 D_003FA760[];

extern void *memset(void *a, s32 b, u32 c);

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

extern void fldSkySetAlpha(s32 alp);

extern void evtFreeAllUnit(void);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void *memset(void *a, s32 b, u32 c);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern sdfVector4_t D_00543628;

extern sdfVector4_t D_00543648;

extern char *D_00551110;

extern char *D_00551114;

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, sdfVector4_t *pos, f32 *rot);

extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);

extern char D_00543668[];

extern char D_00543678[];

extern void *memcpy(void *a, const void *b, u32 c);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

extern dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

#include "basic.h"
#include "dds3obj.h"

extern f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern s32 evtLoadArea(s32 fld, s32 area);



extern nbStageState_t nbStageState;

extern void fldSkyCalcTable(void);

extern void fldSkyCalcFilter(void);

extern void fldSkyDrawFilter(void);

extern void nbStageSp(void);

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

void nbAddStageCamera(nbMainProcessData_t *data)
{
    sdfVector4_t pos;
    f32 rot[4];
    dds3Basic_t *camera;

    pos = D_00543628;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    camera = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), &pos, rot);
    camera->name = D_00551110;
    dds3GetRadiusPoseSet(camera, 10.0f);
    dds3ObjectBaseCreateMover(camera);
    dds3ObjectBaseSetPadControl(camera, 0);
    dds3CameraObjectBasicSetFovy(camera, 0.8726646f);
    data->niidacamera = camera;
}

void nbAddStageCamera2(nbMainProcessData_t *data)
{
    sdfVector4_t pos;
    f32 rot[4];
    dds3Basic_t *camera;

    pos = D_00543648;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    camera = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), &pos, rot);
    camera->name = D_00551114;
    dds3GetRadiusPoseSet(camera, 10.0f);
    dds3ObjectBaseCreateMover(camera);
    dds3ObjectBaseSetPadControl(camera, 0);
    dds3CameraObjectBasicSetFovy(camera, 0.6981317f);
    *(dds3Basic_t **)data->eventcamera = camera;
}

void nbSetStageParam(s16 a, s16 b)
{
    nbStageState.major = a;
    nbStageState.minor = b;
    nbStageState.data = 2;
    nbStageState.stat = 0;
    nbStageState.param = 0x80;
}

void func_0024C6F0(void)
{
    nbStageState.param = 0;
    nbStageState.major = 0;
    nbStageState.minor = 0;
    nbStageState.stat = 0;
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

/* NON_MATCHING: 12 of 109 words differ (first at +0x8). */

#ifdef NON_MATCHING

extern void nbLoadStageSky(void);

void StageLoad(void)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t q;
    nbMainProcessData_t *data;
    dds3Basic_t *camera;
    s32 state;

    memcpy(&pos, D_00543668, 0x10);
    memcpy(&rot, D_00543678, 0x10);
    memset(&q, 0, 0x10);
    q.w = 1.0f;
    data = nbGetMainProcessData();
    state = nbStageState.stat;
    switch (state) {
    case 0:
        nbStageState.stat = 1;
        nbFreeStage();
        evtLoadArea(nbStageState.major, nbStageState.minor);
        camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        dds3CameraObjectBasicSetFovy(camera, 0.8726646f);
        dds3SetPosPoseSet(camera, &pos);
        SDF_VU0_LOADVECTOR(&rot);
        FromEuler(-0.26179937f, -0.34906585f, 0.0f);
        SDF_VU0_STOREVECTOR(&q);
        dds3SetRotPoseSet(camera, &q);
        dds3CameraObjectBasicSetFovy(camera, 0.69813165f);
        dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
        nbAddStageCamera(data);
        nbAddStageCamera2(data);
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), data->niidacamera);
        SetBattleLight(data);
        nbLoadStageSky();
        break;
    case 1:
    case 2:
        nbStageState.stat = 3;
        data->StageMajor = nbStageState.major;
        data->StageMinor = nbStageState.minor;
        break;
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbStageProcess", StageLoad);

#endif

void StageFree(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    evtFreeStage();
    nbStageState.major = 0;
    nbStageState.minor = 0;
    data->StageMajor = 0;
    data->StageMinor = 0;
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
        if (nbStageState.stat == 3) {
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
    nbGetMainProcessData()->StageProcessID = 0;
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

    nbStageState.stat = 0;
    nbStageState.data = 0;
    nbStageState.pad02[0] = 0;
    nbStageState.major = 0;
    nbStageState.minor = 0;
    id = dds3StartProcess(D_00543688, 0x3F9, 1, 1, nbStageProcess1, nbStageShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, id);
    data->StageProcessID = id;
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

typedef struct nbBlurParam_t {
    u32 color;
    s32 unk04;
    s32 unk08;
    f32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 width;
    s32 height;
    s32 unk28;
    s32 unk2C;
} nbBlurParam_t;

extern void dds3EffectBlur(nbBlurParam_t *param);

void nbStageSp(void)
{
    nbBlurParam_t blur;
    s32 n;

    if (fldGlobalBase.area == 0xD7 && fldGlobalBase.floor == 0) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x648C96;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if (fldGlobalBase.area == 0xDA) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0xC896C8;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if (fldGlobalBase.area == 0xD8) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x96B4C8;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if ((fldGlobalBase.area == 0xCA || fldGlobalBase.area == 0xF0) && fldGlobalBase.floor == 0) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x8296B4;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if ((fldGlobalBase.area == 0xE0 && (fldGlobalBase.floor == 2 || fldGlobalBase.floor == 5)) ||
        (fldGlobalBase.area == 0xE8 && fldGlobalBase.floor == 1)) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x8296B4;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if (fldGlobalBase.area == 0xEA && fldGlobalBase.floor == 0) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x96B4C8;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    } else if (fldGlobalBase.area == 0xDF && fldGlobalBase.floor == 2) {
        n = nbStageState.param * 30 / 128;
        if (n > 0) {
            blur.color = (n << 24) | 0x3296B4;
            blur.unk04 = 0x44;
            blur.unk0C = 1.0f;
            blur.width = 0x200;
            blur.height = 0x1C0;
            blur.unk08 = 0;
            blur.unk10 = 0;
            blur.unk14 = 0;
            blur.unk18 = 0;
            blur.unk1C = 0;
            dds3EffectBlur(&blur);
        }
    }
}
INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551110);

INCLUDE_SDATA(const s32, "newbattle/nbStageProcess", D_00551114);

