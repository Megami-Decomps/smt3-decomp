#include "common.h"
#include "basic.h"

typedef struct dds3MoverBasicWork_t
{
    dds3Basic_t *object;
    dds3PathBasicControl_t *pathcontrol;
    s32 (*primarycontrol)(dds3PoseSet_t *pose, dds3Basic_t *object);
    dds3Basic_t *pathbasic;
} dds3MoverBasicWork_t;

extern s32 dds3MoverCount;
extern char D_002FA670[10][16]; /* "MOVER00".."MOVER09" */

extern void *memset(void *s, s32 c, u32 n);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern dds3Basic_t *dds3AddSceneBasicData(u32 type);
extern u32 dds3AllocObjectId(void);
extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *object, dds3Basic_t *item, u32 itemType);
extern dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *pathBasic);
extern void dds3DeletePathControl(dds3PathBasicControl_t *control);

void dds3MoverBasicFreePath(dds3Basic_t *basic);
s32 MoverBasicMainprocuder(dds3Basic_t *basic);

s32 MoverBasicConstructor(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work;

    work = sdfChipAllocDirect(sizeof(dds3MoverBasicWork_t));
    basic->workdata = work;
    memset(work, 0, sizeof(dds3MoverBasicWork_t));
    return 1;
}

void MoverBasicDestructor(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work;

    work = basic->workdata;
    dds3MoverBasicFreePath(basic);
    dds3ObjectBaseSetControlBasicType(work->object, NULL, 1);
    sdfChipFreeDirect(work);
}

INCLUDE_ASM(const s32, "basic/dds3MoverBasic", MoverBasicMainprocuder);

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *object)
{
    dds3Basic_t *basic;
    dds3MoverBasicWork_t *work;

    basic = dds3AddSceneBasicData(BASIC_TYPE_MOVER);
    work = basic->workdata;
    basic->serialID = dds3AllocObjectId();
    work->object = object;
    basic->name = D_002FA670[dds3MoverCount];
    dds3MoverCount++;
    dds3MoverCount %= 10;
    return basic;
}

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *basic, s32 (*func)(dds3PoseSet_t *pose, dds3Basic_t *object))
{
    dds3MoverBasicWork_t *work = basic->workdata;

    work->primarycontrol = func;
}

void dds3MoverBasicSetPath(dds3Basic_t *basic, dds3Basic_t *pathBasic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    work->pathbasic = pathBasic;
}

void dds3MoverBasicMakePath(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    if (work->pathcontrol != NULL) {
        dds3DeletePathControl(work->pathcontrol);
    }
    work->pathcontrol = dds3AllocPathBasicWorkData(work->pathbasic);
}

void dds3MoverBasicFreePath(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    if (work->pathcontrol != NULL) {
        dds3DeletePathControl(work->pathcontrol);
        work->pathcontrol = NULL;
    }
}

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *basic)
{
    dds3MoverBasicWork_t *work = basic->workdata;

    return work->pathcontrol;
}

s32 dds3MoverBasicUpdate(dds3Basic_t *basic)
{
    return MoverBasicMainprocuder(basic);
}

u32 BasicTypeToItemType(u32 type)
{
    u32 itemType = 0;

    switch (type) {
    case BASIC_TYPE_MOVER:
        itemType = 1;
        break;
    case BASIC_TYPE_STATICMODEL:
    case BASIC_TYPE_EFFECT:
    case BASIC_TYPE_EVENT:
        itemType = 3;
        break;
    case BASIC_TYPE_MODELRANGE:
        itemType = 4;
        break;
    case BASIC_TYPE_AREA:
        itemType = 5;
        break;
    }
    return itemType;
}

INCLUDE_SDATA(const s32, "basic/dds3MoverBasic", dds3MoverCount);

INCLUDE_SDATA(const s32, "basic/dds3MoverBasic", D_0054E9CC);

