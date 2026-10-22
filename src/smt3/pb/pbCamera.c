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
    s32 dispStat;
    s16 x, y;
    s32 dir;
    u8 unk0C[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 unk00;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbMatWork_t;

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
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern pbProcHandle_t *record;

extern void mdlFree(mdlObj_t *aHandle);

extern pbProcHandle_t *blockDB;

INCLUDE_ASM(const s32, "pb/pbCamera", pbRotateXCamera);

INCLUDE_ASM(const s32, "pb/pbCamera", pbRotateYCamera);

INCLUDE_ASM(const s32, "pb/pbCamera", pb_1D2838);

INCLUDE_ASM(const s32, "pb/pbCamera", pbSetViewRotation);

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
