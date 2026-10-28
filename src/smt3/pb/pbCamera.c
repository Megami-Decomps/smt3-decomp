#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbProcHandle_t pbProcHandle_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 flg;
    s16 x, y;
    s32 dir;
    u8 dmy[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 flg;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbExit_t;

typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern pbProcHandle_t *record;

extern void mdlFree(mdlObj_t *aHandle);

extern pbProcHandle_t *blockDB;

extern sdfVector4_t aViewEye, aViewTarget;

INCLUDE_ASM(const s32, "pb/pbCamera", pbRotateXCamera);

INCLUDE_ASM(const s32, "pb/pbCamera", pbRotateYCamera);

INCLUDE_ASM(const s32, "pb/pbCamera", pb_1D2838);

extern void SetRotateMatrixY(f32 angle);
extern void SetRotateMatrixX_B(f32 angle);
extern void MultMatrixNoArg(void);

void pbSetViewRotation(f32 angle, f32 radius, f32 degree)
{
    sdfVector4_t offset;

    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = -radius;
    offset.w = 1.0f;
    SetRotateMatrixY(angle);
    SetRotateMatrixX_B(-degree);
    MultMatrixNoArg();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&offset);
    aViewEye.x = offset.x + aViewTarget.x;
    aViewEye.y = offset.y + aViewTarget.y;
    aViewEye.z = offset.z + aViewTarget.z;
}

INCLUDE_ASM(const s32, "pb/pbCamera", pb_draw_mycharacter_Process);

void pb_draw_mycharacter_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbMyChar_t *chara = panel->data.chara;
    mdlFree(chara->model);
    sdfChipFreeDelayedDirect(chara);
    sdfChipFreeDelayedDirect(panel);
}

void pb_1D2AD8(mdlObj_t *model, sdfVector4_t *pos) {
    mdlGetPosition(model);
    SDF_VU0_STOREVECTOR(pos);
}
