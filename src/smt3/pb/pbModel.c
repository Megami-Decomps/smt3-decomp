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
    s16 cellX, cellY;
    s32 rot;
    u8 unk0C[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct
{
    s32 pcharNum;
    u8 unk004[0x14];
    s16 exitX, exitY;
    u8 unk01C[0x320];
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    s32 menuState;
    struct pbProcHandle_t *helpProc;
} pbMapWork_t;

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

typedef struct { sdfVector4_t v[4]; } pbMatrix_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *process;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void mdlSetPosition(mdlObj_t *aHandle);

extern void mdlSetRotate(mdlObj_t *aHandle);

extern void FromEuler(void);

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatSetTex(sdfMat_t *aMat, void *aTex);

extern void pbBuildMapBaseVerts(pbProcHandle_t *panel);

extern pbProcHandle_t *blockDB;

extern sdfVector4_t D_0056CB30[], D_0056C8F0[], D_00326C70[], D_00326CD0[];

extern u8 D_00326D50[];

extern s32 pb_draw_mapbase_Process(dds3ProcessID_t *id);

extern void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id);

extern s32 pb_draw_exit_Process(dds3ProcessID_t *id);

extern void pb_draw_exit_Shutdown(dds3ProcessID_t *id);

extern void pbInitMapBaseMat(pbProcHandle_t *panel);

extern void pbSetModelRotate(mdlObj_t *model, f32 x, f32 y, f32 z);

extern void SetRotateMatrixXYZ(sdfVector4_t *aAngles);

extern void StoreMatrix(pbMatrix_t *aMatrix);

extern void LoadMatrix(pbMatrix_t *aMatrix);

extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);

extern void func_001D2D00(pbProcHandle_t *panel, s32 arg);

extern f32 D_00326C60[];

extern u32 pbMyCharacterDispStat(pbProcHandle_t *p);

void pbSetModelPos(mdlObj_t *aHandle, const f32 *pos) {
    SDF_VU0_LOADVECTOR(pos);
    mdlSetPosition(aHandle);
}

void pb_1D2B18(mdlObj_t *model, sdfVector4_t *rot) {
    mdlGetRotate(model);
    SDF_VU0_STOREVECTOR(rot);
}

void pbSetModelRotate(mdlObj_t *model, f32 x, f32 y, f32 z) {
    FromEuler();
    mdlSetRotate(model);
}

void func_001D2B68(void) {
}

void func_001D2B70(void) {
}

INCLUDE_ASM(const s32, "pb/pbModel", pbInitMyCharacterDraw);

s32 pbGetPanelField(pbProcHandle_t *panel) {
    pbMyChar_t *chara = panel->data.chara;
    s32 ret = 0;
    if (panel != NULL) {
        ret = chara->field;
    }
    return ret;
}

u32 pbMyCharacterDispStat(pbProcHandle_t *p) {
    pbMyChar_t *chara = p->data.chara;
    u32 ret = 0;
    if (p != NULL) {
        ret = chara->dispStat;
    }
    return ret;
}

void pbSetPanelValue(pbProcHandle_t *panel, s8 value) {
    pbPanelValue_t *v = panel->data.value;
    if (panel != NULL) {
        v->value = value;
    }
}

void func_001D2D00(pbProcHandle_t *panel, s32 arg) {
}

void pbChangeMotion(void) {
}

s32 pb_1D2D10(void) {
    return 0;
}

void pbMoveMyCharcter(pbProcHandle_t *p, f32 x, f32 y) {
    pbMyChar_t *chara = p->data.chara;
    if (p != NULL) {
        chara->pos.x += x;
        chara->pos.z += y;
    }
}

void pbMoveMyCharcter2(pbProcHandle_t *p, s16 x, s16 y) {
    pbMyChar_t *chara = p->data.chara;
    if (p != NULL) {
        chara->cellX = x + chara->cellX;
        chara->cellY = y + chara->cellY;
    }
}

void pbGetCellY(pbProcHandle_t *panel, s16 cx, s16 cy) {
    pbMyChar_t *chara = panel->data.chara;
    if (panel != NULL) {
        chara->cellX = cx;
        chara->cellY = cy;
        chara->pos.x = cx * 40 - 380;
        chara->pos.z = cy * 40 - 380;
    }
}

void pbGetMyCharcterPos(pbProcHandle_t *p, sdfVector4_t *pos) {
    pbMyChar_t *chara = p->data.chara;
    if (p != NULL) {
        SDF_VU0_COPYVECTOR(pos, &chara->pos);
    }
}

void pbGetMyCharcterPos2(pbProcHandle_t *p, s16 *x, s16 *y) {
    pbMyChar_t *chara = p->data.chara;
    if (p != NULL) {
        *x = chara->cellX;
        *y = chara->cellY;
    }
}

s32 pbGetMyCharcterRot(pbProcHandle_t *p) {
    pbMyChar_t *chara = p->data.chara;
    s32 ret = -1;
    if (p != NULL) {
        ret = chara->rot;
    }
    return ret;
}


/* NON_MATCHING: 2 of 23 words; the index and work pseudos swap saved registers. */
#ifdef NON_MATCHING
void pbSetPanelModel(pbProcHandle_t *panel, s32 index) {
    pbPanelModel_t *work;
    if (panel != NULL) {
        work = panel->data.model;
        pbSetModelRotate((mdlObj_t *)pbGetPanelField(panel), 0.0f, D_00326C60[index], 0.0f);
        work->modelIndex = index;
    }
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", pbSetPanelModel);
#endif


INCLUDE_ASM(const s32, "pb/pbModel", pb_draw_exit_Process);

void pb_draw_exit_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbMatWork_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "pb/pbModel", pb_draw_mapbase_Process);

void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbMatWork_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

void pbInitMapBaseMat(pbProcHandle_t *panel) {
    pbMatWork_t *work = panel->data.mat;
    pbBuildMapBaseVerts(panel);
    work->mat = sdfMatNew();
    sdfMatSetTex(work->mat, D_0054F5C4->baseTex);
}

void pbBuildMapBaseVerts(pbProcHandle_t *panel) {
    u32 row, col;
    for (row = 0; row < 6; row++) {
        for (col = 0; col < 6; col++) {
            u32 i = col + row * 6;
            SDF_VU0_COPYVECTOR(&D_0056CB30[i], &D_00326C70[row]);
            SDF_VU0_COPYVECTOR(&D_0056C8F0[i], &D_00326CD0[D_00326D50[i]]);
        }
    }
}

void pbSetPrimMatrix(sdfVector4_t *rot, f32 *pos, void *prim) {
    pbMatrix_t m;
    SetRotateMatrixXYZ(rot);
    StoreMatrix(&m);
    m.v[3].x = pos[0];
    m.v[3].y = pos[1];
    m.v[3].z = pos[2];
    LoadMatrix(&m);
    sdf3DPrimLinkTm(prim, 0);
}

INCLUDE_ASM(const s32, "pb/pbModel", pbFindBlockSlot);

INCLUDE_ASM(const s32, "pb/pbModel", func_001D35F0);

INCLUDE_ASM(const s32, "pb/pbModel", pbDrawMapPoly);

INCLUDE_ASM(const s32, "pb/pbModel", pb_1D3940);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535C80);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535C88);

INCLUDE_ASM(const s32, "pb/pbModel", pbDrawMapPolyTransformed);

INCLUDE_ASM(const s32, "pb/pbModel", pbDrawMapBlock);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CB8);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CC0);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CC8);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CD0);

