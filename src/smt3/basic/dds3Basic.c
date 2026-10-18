#include "common.h"
#include "basic.h"
#include "ps2asm.h"

extern void *sdfChipAllocDirect(u32 size);
extern void Normalize(void);
extern void Mult(void);
extern void ToMatrix(void);
extern f32 fabsf(f32 value);

void dds3BackupPoseSet(dds3PoseSet_t *pose);


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

s32 dds3InitPoseSet(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;

    if (basic == NULL) {
        return 0;
    }
    if (basic->poseset != NULL) {
        return 0;
    }
    pose = sdfChipAllocDirect(sizeof(dds3PoseSet_t));
    if (pose == NULL) {
        return 0;
    }
    pose->flag = POSESET_FLAG_UPDATEPOSE;
    dds3BackupPoseSet(pose);
    VU0_STORE_VF0(&pose->plane);
    basic->poseset = pose;
    pose->frameFall = 0;
    pose->r = 0.0f;
    return 1;
}

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

/* vu0 routine: reset translation/rotation and set unit scale. */
void dds3BackupPoseSet(dds3PoseSet_t *pose)
{
    sdfVector4_t *position = &pose->pos;
    sdfVector4_t *rotation;
    sdfVector4_t *scale;

    VU0_STORE_VF0(position);
    rotation = &pose->rot;
    VU0_STORE_VF0(rotation);
    VU0_UNIT_SCALE();
    scale = &pose->scl;
    SDF_VU0_STOREVECTOR(scale);
}

void dds3SetRadiusPoseSet(dds3PoseSet_t *pose)
{
    SDF_VU0_COPYVECTOR(&pose->workscl, &pose->scl);
    SDF_VU0_COPYVECTOR(&pose->workrot, &pose->rot);
    SDF_VU0_COPYVECTOR(&pose->workpos, &pose->pos);
}

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r)
{
    basic->poseset->r = r;
}

f32 func_0010DE98(dds3Basic_t *basic)
{
    return basic->poseset->r;
}

void dds3SetPosPoseSet(dds3Basic_t *basic, const sdfVector4_t *position)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *dst = &pose->pos;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, position);
}

void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *dst = &pose->rot;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, rotation);
}

void dds3SetSclPoseSet(dds3Basic_t *basic, const sdfVector4_t *scale)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *dst = &pose->scl;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, scale);
}

/* vu0 routine: return the position in vf10 with homogeneous w = 1. */
void dds3GetPosPoseSet(dds3Basic_t *basic)
{
    sdfVector4_t *position = &basic->poseset->pos;

    SDF_VU0_LOADVECTOR_BARRIER(position);
    SDF_VU0_VECTOR_W_SET_ONE();
}

void dds3GetRotPoseSet(dds3Basic_t *basic)
{
    sdfVector4_t *rotation = &basic->poseset->rot;

    SDF_VU0_LOADVECTOR_BARRIER(rotation);
    Normalize();
}

/* vu0 routine: return scale in vf10. */
void func_0010DF70(dds3Basic_t *basic)
{
    sdfVector4_t *scale = &basic->poseset->scl;

    SDF_VU0_LOADVECTOR_BARRIER(scale);
}

void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *position = &pose->pos;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_LOADVECTOR_B_BARRIER(offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&pose->pos);
}

void dds3AddRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation)
{
    dds3PoseSet_t *pose = basic->poseset;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(&pose->rot);
    SDF_VU0_LOADVECTOR_B(rotation);
    Mult();
    SDF_VU0_STOREVECTOR(&pose->rot);
}

void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *src = &pose->scl;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&pose->scl);
}

/* vu0 routine: compose scale, quaternion rotation and translation in vf28-31. */
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *rotation,
                    const sdfVector4_t *translation)
{
    SDF_VU0_LOADVECTOR(rotation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(scale);
    VU0_SCALE_MATRIX_ROWS_VF10();
    SDF_VU0_LOADVECTOR(translation);
    SDF_VU0_VECTOR_W_SET_ONE();
    VU0_COPY_VF10_TO_VF31();
}

/* Load the cached matrix, rebuilding it only when a pose setter invalidated it. */
void dds3GetTMPoseSet(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose = basic->poseset;
    u32 flags = pose->flag;

    if (flags & POSESET_FLAG_UPDATETM) {
        SDF_VU0_LOADMATRIX(&pose->workTM);
    } else {
        pose->flag = flags | POSESET_FLAG_UPDATETM;
        dds3MakeMatrix(&pose->scl, &pose->rot, &pose->pos);
        SDF_VU0_STOREMATRIX(&pose->workTM);
    }
}

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right)
{
    f32 distance;
    f32 leftRadius;
    f32 rightRadius;

    SDF_VU0_LOADVECTOR(&left->pos);
    SDF_VU0_LOADVECTOR_B(&right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    leftRadius = left->r;
    rightRadius = right->r;
    if (fabsf(distance) - (leftRadius + rightRadius) < 0.0f) {
        return 1;
    }
    return 0;
}

s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right);

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

