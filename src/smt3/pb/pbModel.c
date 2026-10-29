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

typedef struct
{
    s32 pcharNum;
    u8 unk004[0x14];
    s16 exitX, exitY;
    u16 map[20][20];
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    s32 menuState;
    struct pbProcHandle_t *helpProc;
    u8 reserved368[4];
    s32 drawCount;
} pbMapWork_t;

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

typedef struct { sdfVector4_t v[4]; } pbMatrix_t;

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

extern s32 (*D_0054F5B0[])(s16 x, s16 y, s32 mode);

extern s32 sdfGrAllocPacketBuff(s32 size);

extern void sdfDrawTagInit(sdfDrawTag_t *tag);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *mat,
                             s32 (*allocator)(s32));

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 pbDrawMapBlock(s32 x, s32 y, u8 drawFlags, s32 special,
                          sdfDrawTag_t *tag);

void pbSetModelPos(mdlObj_t *aHandle, const f32 *pos) {
    SDF_VU0_LOADVECTOR(pos);
    mdlSetPosition(aHandle);
}

#ifdef NON_MATCHING
extern void mdlGetRotate(mdlObj_t *aHandle);
#endif

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

extern void *sdfChipAllocDirect(s32 size);
extern void *memset(void *dst, s32 value, u32 size);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
    s32 shutdownTimer, dds3ProcessFunc_t process, dds3ProcessFunc_t shutdown, void *data);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern s32 pb_draw_mycharacter_Process(dds3ProcessID_t *id);
extern void pb_draw_mycharacter_Shutdown(dds3ProcessID_t *id);
extern mdlObj_t *mdlLoad(u32 major, u32 minor);
extern void mdlLoopMotion(mdlObj_t *model, s32 group, s32 number);
extern void mdlSetScale(mdlObj_t *model);
extern const sdfVector4_t D_00535C58;
extern char D_00535C68[];

/* The USA constructor ignores the color and motion arguments supplied by its caller. */
pbProcHandle_t *pbInitMyCharacterDraw(pbProcHandle_t *parent, u32 color, u32 motion)
{
    /* Only the bare-address VU0 load observes this scale vector. */
    volatile sdfVector4_t scale __attribute__((aligned(16))) = D_00535C58;
    pbProcHandle_t *panel;
    pbMyChar_t *chara;

    panel = sdfChipAllocDirect(sizeof(*panel));
    panel->id = dds3StartProcess(D_00535C68, 0x2AFD, 1, 1,
        pb_draw_mycharacter_Process, (dds3ProcessFunc_t)pb_draw_mycharacter_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    panel->data.chara = sdfChipAllocDirect(sizeof(*chara));
    chara = panel->data.chara;
    memset(chara, 0, sizeof(*chara));
    dds3SetProcessData(panel->id, panel);
    chara->flg = 1;
    chara->y = 0;
    chara->x = 0;
    chara->dir = -1;
    chara->pos.x = -380.0f;
    chara->pos.z = -380.0f;
    chara->pos.w = 1.0f;
    chara->model = mdlLoad(6, 0x150);
    mdlLoopMotion(chara->model, 0, 0);
    SDF_VU0_LOADVECTOR(&scale);
    mdlSetScale(chara->model);
    return panel;
}

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
        ret = chara->flg;
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
        chara->x = x + chara->x;
        chara->y = y + chara->y;
    }
}

void pbGetCellY(pbProcHandle_t *panel, s16 cx, s16 cy) {
    pbMyChar_t *chara = panel->data.chara;
    if (panel != NULL) {
        chara->x = cx;
        chara->y = cy;
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
        *x = chara->x;
        *y = chara->y;
    }
}

s32 pbGetMyCharcterRot(pbProcHandle_t *p) {
    pbMyChar_t *chara = p->data.chara;
    s32 ret = -1;
    if (p != NULL) {
        ret = chara->dir;
    }
    return ret;
}


void pbSetPanelModel(pbProcHandle_t *panel, s32 index) {
    pbMyChar_t *chara = panel->data.chara;
    if (panel != NULL) {
        pbSetModelRotate((mdlObj_t *)pbGetPanelField(panel), 0.0f, D_00326C60[index], 0.0f);
        chara->dir = index;
    }
}


extern sdfVector4_t D_00327248;
extern const f64 D_00535C80[];
extern f32 D_0054E2AC, D_0054E2B0, D_0054E2B4, D_0054E2B8;
extern void pbSetPrimMatrix(sdfVector4_t *rotation, f32 *position, void *packet);
extern s32 pbDrawMapPolyTransformed(sdfVector4_t *base, u8 drawFlags,
                                    s32 special, sdfDrawTag_t *tag);

#ifdef NON_MATCHING
s32 pb_draw_exit_Process(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;
    pbExit_t *work = panel->data.mat;
    f32 position[4];
    sdfVector4_t vertices;
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    f32 angle;

    memset(position, 0, sizeof(position));
    memset(&vertices, 0, sizeof(vertices));
    if (panel->state == 1) {
        if (panel != NULL) {
            panel->flags = 0;
        }
    } else if (panel->state == 5) {
        return -1;
    }
    if (work->flg == 0) {
        return 0;
    }
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf3DPrimLinkMat(tag, work->mat, NULL);
    angle = (f64)D_00327248.y + D_00535C80[0];
    D_00327248.y = angle;
    if (D_0054E2AC < angle) {
        D_00327248.y = angle - D_0054E2B0;
    } else if (angle < D_0054E2B4) {
        D_00327248.y = angle + D_0054E2B8;
    }
    position[0] = panel->x * 40 - 380;
    position[1] = -20.0f;
    position[2] = panel->y * 40 - 380;
    pbSetPrimMatrix(&D_00327248, position, tag);
    pbDrawMapPolyTransformed(&vertices, 0x1F, 6, tag);
    ot = &sdfGrOtTable[40];
    ot->regFunc(ot, tag);
    return 0;
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", pb_draw_exit_Process);
#endif

void pb_draw_exit_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbExit_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

/* NON_MATCHING: 7 of 133 words differ; see docs/matching/pb/pbModel.md. */
#ifdef NON_MATCHING
s32 pb_draw_mapbase_Process(dds3ProcessID_t *id)
{
    s32 (*callbacks[2])(s16 x, s16 y, s32 mode);
    pbProcHandle_t *panel;
    pbExit_t *work;
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 callbackResult = 0x1F;
    s32 row;
    u16 state;

    __builtin_memcpy(callbacks, D_0054F5B0, sizeof(callbacks));
    panel = id->workdata;
    state = panel->state;
    work = panel->data.mat;

    switch (state) {
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    }

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf3DPrimLinkMat(tag, work->mat, NULL);

    for (row = 0; row < 20; row++) {
        s32 col;
        for (col = 0; col < 20; col++) {
            u16 tile = D_0054F5C4->map[row][col];

            if ((tile & 0x800) == 0) {
                if (work->flg == 0) {
                    callbackResult = callbacks[work->value]((s16)col, (s16)row, 0);
                }
                {
                    s32 drawCount = pbDrawMapBlock(col, row,
                                                   callbackResult, 0, tag);
                    pbMapWork_t *mapWork = D_0054F5C4;
                    mapWork->drawCount = mapWork->drawCount + drawCount;
                }
            }

            if ((D_0054F5C4->map[row][col] >> 13) == 1) {
                if (work->flg == 0) {
                    callbackResult = callbacks[work->value]((s16)col, (s16)row, 2);
                }
                {
                    s32 drawCount = pbDrawMapBlock(col, row,
                                                   callbackResult, 1, tag);
                    pbMapWork_t *mapWork = D_0054F5C4;
                    mapWork->drawCount = mapWork->drawCount + drawCount;
                }
            }
        }
    }

    ot = &sdfGrOtTable[0x27];
    ot->regFunc(ot, tag);
    return 0;
}

#else
INCLUDE_ASM(const s32, "pb/pbModel", pb_draw_mapbase_Process);
#endif

void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbExit_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

void pbInitMapBaseMat(pbProcHandle_t *panel) {
    pbExit_t *work = panel->data.mat;
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

#ifdef NON_MATCHING
extern s8 D_0054F5B8[4][2];
extern s32 D_00535C88[];

u8 pbFindBlockSlot(s16 x, s16 y, s32 mode)
{
    s8 offsets[4][2];
    s32 bits[4];
    pbMapWork_t *map = D_0054F5C4;
    u16 cell;
    u32 kind, neighborKind, i;
    s32 nx, ny, outside;
    s16 result = 0;
    u16 neighbor, blocked;

    __builtin_memcpy(offsets, D_0054F5B8, sizeof(offsets));
    cell = map->map[y][x];
    __builtin_memcpy(bits, D_00535C88, sizeof(bits));
    kind = cell >> 13;
    if (mode == 0) {
        if (cell & 0x800) {
            return 0;
        }
        result = kind == 0;
    } else if (mode == 1) {
        result = 1;
        if (kind == 0) {
            return 0;
        }
    }
    for (i = 0; i < 4; i++) {
        ny = y + offsets[i][1];
        nx = x + offsets[i][0];
        outside = (u32)ny >= 20 || (u32)nx >= 20;
        neighbor = map->map[ny][nx];
        blocked = neighbor & 0x800;
        neighborKind = neighbor >> 13;
        if (mode == 0) {
            if (!outside) {
                if (blocked) {
                    result |= 1 << bits[i];
                }
                if (kind == 0 && neighborKind == 1) {
                    result |= 1 << bits[i];
                }
                if (kind >= 2) {
                    result |= 1 << bits[i];
                }
            }
        } else if (mode == 1) {
            if (neighborKind < 2) {
                result |= 1 << bits[i];
            }
        } else if (!outside && neighborKind != 1) {
            result |= 1 << bits[i];
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", pbFindBlockSlot);
#endif

#ifdef NON_MATCHING
s32 func_001D35F0(s16 x, s16 y, s32 mode)
{
    pbMapWork_t *map = D_0054F5C4;
    u16 cell = map->map[y][x];
    u32 kind;
    s32 result;

    kind = cell >> 13;
    if (mode == 0) {
        result = kind == 0;
        if (cell & 0x800) {
            return 0;
        }
    } else {
        result = 1;
        if (kind == 0) {
            return 0;
        }
    }
    if (x + 1 >= 20) {
        result |= 8;
    } else {
        cell = map->map[y][x + 1];
        kind = cell >> 13;
        if (mode == 0) {
            if (cell & 0x800) {
                result |= 8;
            }
        } else if (mode == 1) {
            if (kind < 2) {
                result |= 8;
            }
        } else if (kind == 0) {
            result |= 8;
        }
    }
    if (x - 1 < 0) {
        result |= 0x10;
    } else {
        cell = map->map[y][x - 1];
        kind = cell >> 13;
        if (mode == 0) {
            if (cell & 0x800) {
                result |= 0x10;
            }
        } else if (mode == 1) {
            if (kind < 2) {
                result |= 0x10;
            }
        } else if (kind == 0) {
            result |= 0x10;
        }
    }
    if (y + 1 >= 20) {
        result |= 2;
    } else {
        cell = map->map[y + 1][x];
        kind = cell >> 13;
        if (mode == 0) {
            if (cell & 0x800) {
                result |= 2;
            }
        } else if (mode == 1) {
            if (kind < 2) {
                result |= 2;
            }
        } else if (kind == 0) {
            result |= 2;
        }
    }
    if (y - 1 < 0) {
        result |= 4;
    } else {
        cell = map->map[y - 1][x];
        kind = cell >> 13;
        if (mode == 0) {
            if (cell & 0x800) {
                result |= 4;
            }
        } else if (mode == 1) {
            if (kind < 2) {
                result |= 4;
            }
        } else if (kind == 0) {
            result |= 4;
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", func_001D35F0);
#endif

extern sdfVector2_t D_00326D78[];
extern u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);

#ifdef NON_MATCHING
void pbDrawMapPoly(sdfVector4_t *base, sdfDrawTag_t *tag)
{
    sdfVector4_t vertices[6];
    sdf3DPrimPolyParm_t request;
    f32 x = base->x;
    f32 y = base->y;
    f32 z = base->z;
    f32 w = base->w;
    u32 i;

    for (i = 0; i < 6; i++) {
        sdfVector4_t *vertex = &D_00326CD0[D_00326D50[i]];
        vertices[i].x = vertex->x + x;
        vertices[i].y = vertex->y + y;
        vertices[i].z = vertex->z + z;
        vertices[i].w = vertex->w + w;
    }
    memset(&request, 0, sizeof(request));
    request.faceCount = 2;
    request.pointCount = 6;
    request.color = 0x80808080;
    request.vertex = vertices;
    request.normal = D_0056CB30;
    request.uv = D_00326D78;
    sdfDrawTagLink(tag, (u32)sdf3DPrimMakePoly(&request));
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", pbDrawMapPoly);
#endif

extern void pbDrawMapPoly(sdfVector4_t *base, sdfDrawTag_t *tag);

void pb_1D3940(s32 x, s32 y, sdfDrawTag_t *tag)
{
    sdfVector4_t rotation __attribute__((aligned(16)));
    sdfVector4_t position __attribute__((aligned(16)));
    sdfVector4_t geometry __attribute__((aligned(16)));

    memset(&rotation, 0, 0x10);
    memset(&position, 0, 0x10);
    memset(&geometry, 0, 0x10);
    geometry.y = 20.0f;
    position.x = x * 40 - 380;
    position.z = y * 40 - 380;
    pbSetPrimMatrix(&rotation, &position.x, tag);
    pbDrawMapPoly(&geometry, tag);
}

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535C80);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535C88);

#ifdef NON_MATCHING
extern sdfVector2_t D_00326E68[], D_00326F58[], D_00327048[], D_00327138[];
extern u32 D_00327228[];
extern const f64 D_00535CB8, D_00535CC0, D_00535CC8, D_00535CD0;

s32 pbDrawMapPolyTransformed(sdfVector4_t *base, u8 drawFlags, s32 special, sdfDrawTag_t *tag)
{
    sdfVector4_t vertices[36] __attribute__((aligned(16)));
    sdfVector4_t normals[36] __attribute__((aligned(16)));
    sdf3DPrimPolyParm_t request;
    sdfVector4_t *source, *vertex;
    f32 x, y, z, w, sourceX, sourceY, sourceZ, sourceW;
    u32 side, corner, count = 0, index, color;

    if (drawFlags == 0) {
        return 0;
    }
    memset(&request, 0, sizeof(request));
    color = D_00327228[special];
    request.attr = 0x4000;
    request.color = color;
    request.vertex = vertices;
    request.normal = normals;
    switch (special) {
    case 0:
        request.uv = D_00326D78;
        break;
    case 1:
        request.uv = D_00327138;
        break;
    case 2:
    case 3:
        request.uv = D_00326E68;
        break;
    case 4:
        request.uv = D_00327048;
        break;
    case 5:
        request.uv = D_00326F58;
        break;
    }
    for (side = 0; side < 5; side++) {
        if ((drawFlags >> side) & 1) {
            x = base->x;
            y = base->y;
            z = base->z;
            w = base->w;
            for (corner = 0; corner < 6; corner++) {
                index = D_00326D50[side * 6 + corner];
                source = &D_00326CD0[index];
                vertex = &vertices[count + corner];
                sourceW = source->w;
                sourceX = source->x;
                sourceY = source->y;
                sourceZ = source->z;
                vertex->x = sourceX + x;
                vertex->y = sourceY + y;
                vertex->z = sourceZ + z;
                vertex->w = sourceW + w;
                if (special == 6) {
                    vertex->x = (f32)((f64)vertex->x * 0.5);
                    vertex->y = (f32)((f64)vertex->y * 0.5);
                    vertex->z = (f32)((f64)vertex->z * 0.5);
                } else if (special == 1) {
                    if (index >= 4) {
                        vertex->y = vertex->y - 20.0f;
                    }
                    if (drawFlags & 8) {
                        vertex->x = (f32)((f64)vertex->x - D_00535CB8);
                    }
                    if (drawFlags & 0x10) {
                        vertex->x = (f32)((f64)vertex->x + D_00535CC0);
                    }
                    if (drawFlags & 2) {
                        vertex->z = (f32)((f64)vertex->z - D_00535CC8);
                    }
                    if (drawFlags & 4) {
                        vertex->z = (f32)((f64)vertex->z + D_00535CD0);
                    }
                }
                SDF_VU0_COPYVECTOR(&normals[count + corner], &D_00326C70[side]);
            }
            request.faceCount += 2;
            count += 6;
            request.pointCount += 6;
        }
    }
    sdfDrawTagLink(tag, (u32)sdf3DPrimMakePoly(&request));
    return request.pointCount;
}
#else
INCLUDE_ASM(const s32, "pb/pbModel", pbDrawMapPolyTransformed);
#endif

extern s32 pbDrawMapPolyTransformed(sdfVector4_t *base, u8 drawFlags,
                                    s32 special, sdfDrawTag_t *tag);

s32 pbDrawMapBlock(s32 x, s32 y, u8 drawFlags, s32 special,
                   sdfDrawTag_t *tag)
{
    sdfVector4_t rotation __attribute__((aligned(16)));
    sdfVector4_t position __attribute__((aligned(16)));
    sdfVector4_t geometry __attribute__((aligned(16)));
    s32 px;
    s32 pz;
    s32 result;

    memset(&rotation, 0, 0x10);
    memset(&position, 0, 0x10);
    memset(&geometry, 0, 0x10);
    result = 0;
    if (drawFlags != 0) {
        px = x * 40 - 380;
        pz = y * 40 - 380;
        position.x = px;
        position.z = pz;
        if (special == 0) {
            position.y = 20.0f;
        } else {
            position.y = -20.0f;
        }
        pbSetPrimMatrix(&rotation, &position.x, tag);
        result = pbDrawMapPolyTransformed(&geometry, drawFlags, special, tag);
    }
    return result;
}

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CB8);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CC0);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CC8);

INCLUDE_RODATA(const s32, "pb/pbModel", D_00535CD0);

