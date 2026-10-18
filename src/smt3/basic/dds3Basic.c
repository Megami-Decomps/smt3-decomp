#include "common.h"
#include "basic.h"

extern u32 dds3ObjectIdCounter;

void sdfChipFreeDirect(void *ptr);


void dds3DeleteBasic(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;

    if (basic == NULL) {
        return;
    }
    function = basic->basicfunction;
    if (function != NULL && function->destructor != NULL) {
        ((void (*)(dds3Basic_t *))function->destructor)(basic);
    }
    if (basic->prev != NULL) {
        basic->prev->next = basic->next;
    }
    if (basic->next != NULL) {
        basic->next->prev = basic->prev;
    }
    sdfChipFreeDirect(basic);
}

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3InitPoseSet);

void dds3FreePoseSet(dds3Basic_t *basic)
{
    if (basic != NULL && basic->poseset != NULL) {
        sdfChipFreeDirect(basic->poseset);
        basic->poseset = NULL;
    }
}

void dds3OnPoseSetFlag(dds3PoseSet_t *poseset, u32 flag)
{
    poseset->flag |= flag;
}

void dds3OffPoseSetFlag(dds3PoseSet_t *poseset, u32 flag)
{
    poseset->flag &= ~flag;
}

s32 dds3CheckPoseSetFlag(dds3PoseSet_t *poseset, u32 flag)
{
    return (poseset->flag & flag) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3BackupPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3SetRadiusPoseSet);

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r)
{
    basic->poseset->r = r;
}

f32 func_0010DE98(dds3Basic_t *basic)
{
    return basic->poseset->r;
}

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3SetPosPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3SetRotPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3SetSclPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3GetPosPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3GetRotPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", func_0010DF70);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3GetPosPoseSetMCR);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3AddRotPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3GetSclPoseSetMCR);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3MakeMatrix);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3GetTMPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3ComparePosPoseSet);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3ComparePosPoseSet2);

s32 dds3CallBasicMainProc(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (basic != NULL) {
        function = basic->basicfunction;
        if (function != NULL && function->mainprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->mainprocuder)(basic);
        }
    }
    return result;
}

s32 dds3CallBasicDrawProc(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (basic != NULL) {
        function = basic->basicfunction;
        if (function != NULL && function->drawprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->drawprocuder)(basic);
        }
    }
    return result;
}

u32 dds3AllocObjectId(void)
{
    dds3ObjectIdCounter = (dds3ObjectIdCounter + 1) & 0xFFFF;
    return dds3ObjectIdCounter;
}

void dds3SetUnitObjectId(dds3Basic_t *basic, char *name)
{
    if (basic != NULL) {
        basic->name = name;
    }
}

char *func_0010E270(dds3Basic_t *basic)
{
    if (basic == NULL) {
        return NULL;
    }
    return basic->name;
}
INCLUDE_SDATA(const s32, "basic/dds3Basic", dds3ObjectIdCounter);

INCLUDE_SDATA(const s32, "basic/dds3Basic", dds3TheWorld);

