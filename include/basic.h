#ifndef BASIC_H
#define BASIC_H

/* dds3Basic: the scene graph node every world, area, camera, unit, effect and
 * light hangs off, plus its transform block.
 * See include/README.md for how the layouts were verified. */

#include "common.h"
#include "libsdf.h"

typedef struct dds3Basic_t dds3Basic_t;
typedef struct dds3BasicFunction_t dds3BasicFunction_t;
typedef struct dds3PoseSet_t dds3PoseSet_t;
typedef struct dds3BasicArrayHandle_t dds3BasicArrayHandle_t;
typedef struct dds3BasicArray_t dds3BasicArray_t;
typedef struct dds3WorldBasic_t dds3WorldBasic_t;
typedef struct dds3PathBasicControl_t dds3PathBasicControl_t;

/* Bodies not recovered yet: reached only through pointers. */
struct effSpline_t;
struct fldResourcePathData_t_Vector3;
struct fldResourcePathData_t_Vector4;
struct fldResourcePathData_t_Float;
struct fldResourcePathData_t_Light;

/* One vtable per basic type, indexed by dds3BasicFuncTablePtr[type].
 * constructor takes (basic, color) and returns 1 on success; the rest take the
 * basic. reserve1 and reserve2 are from the HD metadata, so the 24-byte size
 * is the one thing here not pinned by the binary. */
struct dds3BasicFunction_t
{
    void *constructor;
    void *destructor;
    void *mainprocuder;
    void *drawprocuder;
    void *reserve1;
    void *reserve2;
};

/* Transform state. workTM is the composed matrix, kept inline so the VU0 paths
 * can lq/sq it straight from offset 0. */
struct dds3PoseSet_t
{
    sdfMatrix4_t workTM;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t scl;
    sdfVector4_t pos2;    /* unconfirmed */
    sdfVector4_t workpos; /* unconfirmed */
    sdfVector4_t workrot; /* unconfirmed */
    sdfVector4_t workscl; /* unconfirmed */
    sdfVector4_t plane;   /* unconfirmed */
    u32 flag;
    f32 r;
    f32 frameFall;
    u32 pad;
};

struct dds3Basic_t
{
    u32 flag;
    u32 serialID;
    char *name;
    u32 type; /* the basic type in the top byte: type << 24 */
    dds3BasicFunction_t *basicfunction;
    void *staticdata; /* unconfirmed */
    void *workdata;
    dds3PoseSet_t *poseset;
    dds3Basic_t *next;
    dds3Basic_t *prev;
    dds3Basic_t *worknext;   /* unconfirmed */
    dds3Basic_t *workprev;   /* unconfirmed */
    dds3Basic_t *workpointer; /* unconfirmed */
    u32 color;
    u32 pad[3];
};

/* A span of the world's shared basic array. The three indices start at -1. */
struct dds3BasicArrayHandle_t
{
    s16 firstindex;
    s16 lastindex;
    s16 nowindex;
    u16 count;
    dds3BasicArrayHandle_t *next;
    dds3BasicArrayHandle_t *prev;
};

/* One slot of the world's shared basic array. dds3AddWorld builds the free
 * chain over it with an 8-byte stride; next and prev are -1 terminated. */
struct dds3BasicArray_t
{
    dds3Basic_t *target;
    s16 next;
    s16 prev;
};

/* The world basic's workdata: the scene list, the two selected scenes, and the
 * shared basic array with its free chain and the handles carved out of it.
 * basicarray is a pointer: worldconstructor stores a word there and every
 * reader indexes it, so the IDB's inline dds3BasicArray_t[10000] (which also
 * moves basicarraycnt to +0x13894) is not this build's layout. */
struct dds3WorldBasic_t
{
    dds3Basic_t *firstscene;
    dds3Basic_t *lastscene;
    dds3Basic_t *activescene;
    dds3Basic_t *controlscene;
    sdfMemHandle_t *basicarray_hndl;
    dds3BasicArray_t *basicarray;
    u16 basicarraycnt;
    s16 firstempty;
    s16 lastempty;
    u16 emptycount;
    dds3BasicArrayHandle_t *firstheader;
    dds3BasicArrayHandle_t *lastheader;
};

/* Path playback for a camera or object following a spline.
 * Only the 36-byte size is confirmed; the fields are from the HD metadata. */
struct dds3PathBasicControl_t
{
    u32 mode;
    u32 flag;
    f32 frameMax;
    f32 frame;
    struct effSpline_t *pSplineHandle;
    struct fldResourcePathData_t_Vector3 *pTransrate;
    struct fldResourcePathData_t_Vector4 *pRotation;
    struct fldResourcePathData_t_Float *pFovy;
    struct fldResourcePathData_t_Light *pLight;
};

/* dds3AllocBasic rejects a type outside this range. */
enum basic_type
{
    BASIC_TYPE_WORLD = 0,
    BASIC_TYPE_SCENE = 1,
    BASIC_TYPE_AREA = 2,
    BASIC_TYPE_MOVER = 3,
    BASIC_TYPE_CAMERAOBJECT = 4,
    BASIC_TYPE_UNITOBJECT = 5,
    BASIC_TYPE_MATTEROBJECT = 6,
    BASIC_TYPE_EFFECTOBJECT = 7,
    BASIC_TYPE_EVENTOBJECT = 8,
    BASIC_TYPE_LIGHTOBJECT = 9,
    BASIC_TYPE_STATICMODEL = 10,
    BASIC_TYPE_MODELRANGE = 11,
    BASIC_TYPE_EFFECT = 12,
    BASIC_TYPE_EVENT = 13,
    BASIC_TYPE_FOG = 14,
    BASIC_TYPE_LIGHT = 15,
    BASIC_TYPE_PATH = 16,
    BASIC_TYPE_POINT = 17,
    BASIC_TYPE_MAX = 18
};

/* dds3PoseSet_t.flag. UPDATETM is set once workTM is composed. */
enum poseset_flag
{
    POSESET_FLAG_UPDATEPOSE = 1,
    POSESET_FLAG_UPDATETM = 2,
    POSESET_FLAG_ONGROUND = 4,
    POSESET_FLAG_POS2 = 8
};

enum dds3_pathbasic_flag
{
    DDS3_PATHBASIC_FLAG_POS = 1,
    DDS3_PATHBASIC_FLAG_ROT = 2,
    DDS3_PATHBASIC_FLAG_FOVY = 4,
    DDS3_PATHBASIC_FLAG_STOP = 8,
    DDS3_PATHBASIC_FLAG_LIGHT = 16,
    DDS3_PATHBASIC_FLAG_LOOP = 32
};

enum dds3_pathbasic_control
{
    DDS3_PATHBASIC_CONTROL_FORWARD = 0,
    DDS3_PATHBASIC_CONTROL_BACK = 1,
    DDS3_PATHBASIC_CONTROL_MAX = 2
};

enum dds3_basicarray
{
    DDS3_BASICARRAY_SERACHDELETE_MODE_FIRST = 0,
    DDS3_BASICARRAY_SERACHDELETE_MODE_ALL = 1,
    DDS3_BASICARRAY_SERACHDELETE_MODE_MAX = 2
};

#endif /* BASIC_H */
