#include "common.h"
#include "pbRecord.h"
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
extern f32 sdfCos(f32 angle);
extern f32 sdfSin(f32 angle);

void pbRotateXCamera(f32 angle)
{
    sdfVector4_t offset __attribute__((aligned(16)));
    f32 rotatedA;
    f32 rotatedB;

    SDF_VU0_COPYVECTOR(&offset, &aViewEye);
    offset.y -= aViewTarget.y;
    offset.z -= aViewTarget.z;
    rotatedA = sdfCos(angle) * offset.y + sdfSin(angle) * offset.z;
    rotatedB = -sdfSin(angle) * offset.y + sdfCos(angle) * offset.z;
    aViewEye.y = rotatedA + aViewTarget.y;
    aViewEye.z = rotatedB + aViewTarget.z;
}

void pbRotateYCamera(f32 angle)
{
    sdfVector4_t offset __attribute__((aligned(16)));
    f32 rotatedA;
    f32 rotatedB;

    SDF_VU0_COPYVECTOR(&offset, &aViewEye);
    offset.x -= aViewTarget.x;
    offset.z -= aViewTarget.z;
    rotatedA = sdfCos(angle) * offset.x + sdfSin(angle) * offset.z;
    rotatedB = -sdfSin(angle) * offset.x + sdfCos(angle) * offset.z;
    aViewEye.x = rotatedA + aViewTarget.x;
    aViewEye.z = rotatedB + aViewTarget.z;
}

void pb_1D2838(f32 angle)
{
    sdfVector4_t offset __attribute__((aligned(16)));
    f32 rotatedA;
    f32 rotatedB;

    SDF_VU0_COPYVECTOR(&offset, &aViewEye);
    offset.x -= aViewTarget.x;
    offset.y -= aViewTarget.y;
    rotatedA = sdfCos(angle) * offset.x + sdfSin(angle) * offset.y;
    rotatedB = -sdfSin(angle) * offset.x + sdfCos(angle) * offset.y;
    aViewEye.x = rotatedA + aViewTarget.x;
    aViewEye.y = rotatedB + aViewTarget.y;
}

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

extern void *memset(void *dst, s32 value, u32 size);
extern void pbSetModelPos(mdlObj_t *model, const f32 *pos);
extern void mdlLink(mdlObj_t *model, void *ot);
extern void mdlGetPosition(mdlObj_t *model);
extern void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius);
extern u8 D_002F69B8[];

s32 pb_draw_mycharacter_Process(dds3ProcessID_t *id)
{
    sdfVector4_t pos;
    pbProcHandle_t *panel = id->workdata;
    pbMyChar_t *chara = panel->data.chara;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    switch (panel->state) {
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    }
    if (chara->flg != 0) {
        pbSetModelPos(chara->model, &chara->pos.x);
        mdlLink(chara->model, D_002F69B8);
        mdlGetPosition(chara->model);
        SDF_VU0_STOREVECTOR(&pos);
        fldDrawMaruKage(0x80, pos.x, pos.y - 1.0f, pos.z, 20.0f);
    }
    return 0;
}

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
