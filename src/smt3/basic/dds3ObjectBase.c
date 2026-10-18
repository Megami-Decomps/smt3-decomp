#include "common.h"
#include "basic.h"

s32 BasicTypeToItemType(u32 type);
void dds3AreaBasicRegistItem(dds3Basic_t *area, dds3Basic_t *basic);
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *basic);
s32 dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, void *func);
void dds3MoverBasicSetPath(dds3Basic_t *mover, dds3Basic_t *path);
void dds3MoverBasicMakePath(dds3Basic_t *basic);
void dds3MoverBasicFreePath(dds3Basic_t *mover);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *mover);

void *dds3ObjectBaseGetControlBasic(dds3Basic_t *basic, s32 type);
void *dds3ObjectBaseSetControlBasic(dds3Basic_t *basic, dds3Basic_t *control);
void *dds3ObjectBaseSetControlBasicType(dds3Basic_t *basic, dds3Basic_t *control, s32 type);


INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseInitControlWorkData);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseFreeControlWorkData);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseOnFlag);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseOffFlag);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseCheckFlag);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseSetUserValue);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseGetUserValue);

void *dds3ObjectBaseSetControlBasic(dds3Basic_t *basic, dds3Basic_t *control)
{
    if (control == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(basic, control,
                                             BasicTypeToItemType(control->type >> 24));
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseSetControlBasicType);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseGetControlBasic);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseGetEventArray);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseGetModelHandle);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseFreeModelHandle);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMakeUnit);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMakeMatter);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMatterAnimeSpeed);

s32 dds3ObjectBaseRegistArea(dds3Basic_t *basic)
{
    dds3Basic_t *area;

    area = dds3ObjectBaseGetControlBasic(basic, 5);
    if (area == NULL) {
        return 0;
    }
    dds3AreaBasicRegistItem(area, basic);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseModelLinkOtSvs);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseModelLinkOt);

void dds3ObjectBaseCreateMover(dds3Basic_t *basic)
{
    if (dds3ObjectBaseGetControlBasic(basic, 1) == NULL) {
        dds3ObjectBaseSetControlBasic(basic, dds3AddMoverBasic(basic));
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseSetPadControl);

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, void *func)
{
    dds3Basic_t *mover;

    mover = dds3ObjectBaseGetControlBasic(basic, 1);
    if (mover == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(mover, func);
    return 1;
}

void dds3ObjectBaseMakePath(dds3Basic_t *basic, dds3Basic_t *path)
{
    dds3MoverBasicSetPath(dds3ObjectBaseGetControlBasic(basic, 1), path);
    dds3MoverBasicMakePath(basic);
}

void dds3ObjectBaseFreePath(dds3Basic_t *basic)
{
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(basic, 1));
}

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *basic)
{
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(basic, 1));
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3GetBaseControlWorkData);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseSetControlBasicSvs);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9D0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9D8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9E0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9E8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9F0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9F8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", frMessageCode);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054EA02);

