#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
} fldMapPoint_t;

typedef struct
{
    char *name;
    u32 nodeIndex;
    void *icons;
    u32 iconCount;
    s32 floor;
    fldMapPoint_t *pointA;
    fldMapPoint_t *pointB;
} fldMapItem_t;

typedef struct
{
    char *name;
    fldMapItem_t *items;
    u32 count;
    void *model;
    fldMapPoint_t *pos;
} fldMapRoom_t;

typedef struct
{
    s32 unk00;
    s32 id;
    s32 activationState;
    s32 activationRequested;
    u8 unk10[0x14];
    void *effect;              /* 0x24 */
    u8 unk28[8];
} fldTakaraSlot_t;

typedef struct
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    u8 unk10[0x10];
    dds3Basic_t *object;       /* 0x20 */
    u8 unk24[0xC];
    void *effect;              /* 0x30 */
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 room;                  /* 0x48 */
    s32 unk4C;
    u8 unk50[0x20];
} fldNpcSlot_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern sdfMemHandle_t *fldAutoMapBuf;

void sdfMemFreeDirect(sdfMemHandle_t *allocation);

extern fldMapRoom_t *fldAutoMapHeader;

extern s32 fldAutoMapData;

extern void *memset(void *dst, s32 value, u32 size);

extern void sdfModelMakeLocalTm(sdfModelNode_t *node);

extern void sdfModelEvalTm(sdfModelRootNode_t *model);

extern sdfModelOt_t D_002F6A48;

extern void sdfModelLinkOt(sdfModelOt_t *ot, sdfModelRootNode_t *model);

extern u32 AutoMapSeq;

extern s32 fldAreaNameUpper[];

extern s32 fldAreaNameLower[];

extern s32 fldAutoMapField;

extern s32 fldAutoMapChkOpen(s32 area, s32 room, u32 item);

extern s32 fldAutoMapBBox;

extern s32 fldTakaraCount;

extern fldTakaraSlot_t fldTakaraSlotTable[32];

extern s32 fldGb[];

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern sdfTexHandle_t *fldUiTexture;

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);

extern s32 fldNpcModelTable;

extern fldNpcSlot_t fldModelSlotTable[];

extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);

extern u32 strlen(const char *s);

dds3Basic_t *dds3GetActiveScene(void);

extern void fldRegistUnitPad(void);

extern void fldAutoMapOpenBlock2(s32 area, s32 floor, s32 flagNumber);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2,
                      u32 gsWord3);

extern void sdf3DSetCamera(void *node, void *transformMatrix);

extern u8 dds3ScreenParam[];

extern u8 aProjMatrix[];

extern u8 D_002FA240[];

extern void InversMatrix(void);

extern void fldAutoMapDrawBgOfsRF(f32 x, f32 y, f32 z);

extern f32 D_0054EEA0;

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *allocation);

extern sdfVector4_t fldAutoMapFieldId;

extern sdfVector4_t fldAutoMapPosX;

extern s32 fldAutoMapAreaNo;

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

extern s32 sprintf(char *dst, const char *format, ...);

extern void fldAddressResolver(u8 *buffer, void *chunk);

extern u8 *fldGetInfoHeader(void *chunk);

extern void fldAutoMapFontInit(s32 field);

extern void fldAmModelLoadFile(void);

extern void sdfDrawSync(void);

extern void fldAutoMapFree(void);

extern void fldAutoMapSeqClearFlag(s32 flag);

extern void evtEnableMoonDraw(void);

extern s32 fldMenuOpenFlag[];

extern void frSetFontDrawOT(s32 ot);

extern void dds3VisibleSceneBasic(dds3Basic_t *scene, s32 visible);

extern void *fldBlurEffectHandle;

extern void dds3EffectDistortionBlurDelete(void *handle);

extern sdfTexHandle_t *fldEATexture;

extern sdfTexHandle_t *fldVLTexture;

extern sdfTexHandle_t *fldVLBlurHandle;

extern sdfTexHandle_t *fldSnowBlurHandle;

extern sdfMemHandle_t *fldBlurEffectTable[];

extern void *fldBlurEffectId[];

extern void *D_0054EED0;

extern u16 D_00567EA0[];

extern u16 D_00567EF8[];

extern u16 D_00567F50[];

extern char D_0030AB54[][28];

extern char D_0030FBD4[][28];

extern char D_0030D714[][28];

extern char D_0030A670[][26];

extern u16 D_00551CC8;

extern s32 fldAutoMapFindArea(s32 fld, s32 area);

extern s32 fldAutoMapFindFloor(s32 fld, s32 area);

extern s32 fldAutoMapFindPlace(s32 fld, s32 area);

extern void *D_0054EEDC;

extern void *dds3EffectLoad(void *resource);

extern void *D_00551D08;

extern void *D_00551D10;

extern void *D_00551D18;

extern void *fldDmgBlurHandle;

extern void *fldNpcBlurHandle;

extern sdfTexHandle_t *sdfTexSet(void *resource);

extern void *D_00551CD0;

extern void *D_00551CD8;

extern void *D_00551CE0;

extern void *D_00551CE8;

extern void *D_00551CF0;

extern void *D_00551CF8;

extern void *D_00568E90[];

extern void *D_00568F90[];

typedef struct
{
    s32 unk00;
    s32 unk04;
    f32 unk08;
    u32 unk0C;
    s32 unk10;
    s32 unk14;
    f32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} fldBlurParam_t;

extern void *dds3EffectDistortionBlurCreate(fldBlurParam_t *param);

extern void dds3EffectDelete(void *effect);

extern s32 fldTakaraSlotCount;

extern s32 fldNpcLoadIdx;

extern void fldMakeFilePath(char *out, s32 field, s32 area);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *path, u8 **loadedAt, u32 *size);

extern void *D_00312150[];

extern fldMapPoint_t fldAutoMapModelPos[32];

extern sdfTexHandle_t *D_00551CC0[2];

extern sdfTexHandle_t *fldAutoMapWindowTex[];

extern void sdfModelRootFree(void *model, s32 a, s32 b);

extern s32 sdfDelayedResrcCheck(void);

extern s32 fldDrawFlag[];

extern s8 dds3PadButtonState[64];

extern void fldDbgPrint2D(s32 x, s32 y, const char *format);

extern void fldAutoMapDrawOneArea(void);

extern s32 fldEncountTestFlag(s32 flag);

void fldAllIconDel(s32 index, f32 *x, f32 *z);

extern s32 fldAutoMapSeqTestFlag(s32 flag);

extern void fldAutoMapSeqSetFlag(s32 flag);

extern void fldReleaseUnitPad(void);

extern void evtDisableMoonDraw(void);

extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);

extern void *sdfModelMakeEarly(s32 mode, void *resource);

extern s32 fldAutoMapSeqState;

extern s32 fldAutoMapSeqStep;

extern s32 fldAutoMapDrawArea;

extern s32 D_0054EE58;

extern s32 D_00551CAC;

extern s32 D_00551CB0;

extern s32 D_00551CB4;

extern s32 D_00551CB8;

extern sdfVector4_t D_00532230;

extern sdfVector4_t D_002F5E00;

extern sdfVector4_t D_002F5E10;

extern sdfVector4_t fldAutoMapRect;

extern u8 dds3LightParam2[];

extern f32 dds3ScreenParam2[];

typedef struct
{
    f32 near;
    f32 far;
    f32 scale;
    f32 bias;
    u32 mode;
} fldAmLightParam_t;


typedef struct
{
    u8 pad0[0x60];
    sdfPacWork_t *queueHead;
} fldPacRequest_t;

extern fldPacRequest_t *pacLoad(const char *path);

extern void pacSync(fldPacRequest_t *request);

extern void pacFileHandleFree_(fldPacRequest_t *request);

extern void *sdfMemGetPtr(sdfMemHandle_t *handle);

extern sdfMemHandle_t *fldTakara1File;

extern u8 *fldTakara1Data;

extern u32 fldTakara1Size;

extern sdfMemHandle_t *fldTakara2File;

extern u8 *fldTakara2Data;

extern u32 fldTakara2Size;

extern sdfMemHandle_t *fldTakara3File;

extern u8 *fldTakara3Data;

extern u32 fldTakara3Size;

extern void *fldPacFileHandle;

extern void *fldNpcPacData;

extern void *fldTakaraPacData;

extern void *D_00551CCC;

extern void *D_00551CD4;

extern void *D_00551CDC;

extern void *D_00551CE4;

extern void *D_00551CEC;

extern void *D_00551CF4;

extern void *D_00551D04;

extern void *D_00551D0C;

extern void *D_00551D14;

extern void *D_00551D1C;

extern void *D_0054EF1C;

extern void *D_0054EF20;

extern void *D_0054EF24;

extern void *D_0054EF28;

extern void *D_0054EF2C;

extern s32 D_0054EF30;

extern void *D_0054EF3C;

extern void *D_0054EF40;


extern s16 fldAutoMapIconOfsTable[][8];

extern sdfDrawTag_t *sdfGrAllocPacketBuff(s32 size);

extern void sdfDrawTagInit(sdfDrawTag_t *tag);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *tag, sdfTexHandle_t *texture, s32 contextOffset);

extern sdfGrOt_t *fldDrawGetOt(void);

extern void sdfPrimLinkFt4(sdfDrawTag_t *tag, u32 color, s32 mode, s32 x0, s32 y0, s32 u0,
                           s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2,
                           s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 mask, void *next);

void fldAutoMapFontInit(s32 fld) {
    s32 i;

    for (i = 0; i < 0x29; i++) {
        D_00567EA0[i] = strlen(D_0030AB54[fldAutoMapFindArea(fld, i)]);
    }
    for (i = 0; i < 0x29; i++) {
        D_00567EF8[i] = strlen(D_0030FBD4[fldAutoMapFindFloor(fld, i)]);
    }
    for (i = 0; i < 0x18; i++) {
        D_00567F50[i] = strlen(D_0030D714[fldAutoMapFindPlace(fld, i)]);
    }
    i = 0;
    if (fldGb[0] < 50) {
        i = fldGb[0];
    }
    D_00551CC8 = strlen(D_0030A670[i]);
}


/* NON_MATCHING: 24 of 91 words, two alignment nops short; saved registers are swapped. */
#ifdef NON_MATCHING
void fldAmModelLoadFile(void) {
    fldAreaWork_t *work = &fldGlobalBase;
    char path[0x80];
    char dir[0x40];
    u8 *data;
    const char *fmt;
    s32 floor;
    u32 *info;

    fldMakeFilePath(dir, work->area, 1);
    if (work->area == 0x19 && work->floor + 1 >= 0x11) {
        sprintf(path, "%sf%03db.amb", dir, 0x19);
    } else if (fldGlobalBase.area == 0x22) {
        floor = fldGlobalBase.floor + 1;
        if (floor < 4) {
            fmt = "%sf%03d.amb";
        } else if (floor < 0xB) {
            fmt = "%sf%03db.amb";
        } else if (floor < 0x13) {
            fmt = "%sf%03dc.amb";
        } else if (floor < 0x1A) {
            fmt = "%sf%03dd.amb";
        } else {
            goto plain;
        }
        sprintf(path, fmt, dir, 0x22);
    } else {
    plain:
        sprintf(path, "%sf%03d.amb", dir, fldGlobalBase.area);
    }
    fldAutoMapBuf = sdfDevLoadFileEx(path, &data, 0);
    fldAddressResolver(data, data + 8);
    info = (u32 *)fldGetInfoHeader(data + 8);
    fldAutoMapData = info[2];
    fldAutoMapHeader = (fldMapRoom_t *)info[1];
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAmModelLoadFile);
#endif


void fldAmbLoadFromMem(u8 *data, sdfMemHandle_t *handle) {
    s32 field = fldGb[0];
    u32 *info;

    if (field < 200) {
        if (field >= 10) {
            fldAutoMapBuf = handle;
            fldAddressResolver(data, data + 8);
            info = (u32 *)fldGetInfoHeader(data + 8);
            {
                u32 mapHeader = info[1];
                u32 mapData = info[2];

                fldAutoMapData = mapData;
                fldAutoMapHeader = (fldMapRoom_t *)mapHeader;
            }
        }
    }
}

INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapModelLoad);

void fldAutoMapFileFree(void) {
    if (fldAutoMapBuf != 0) {
        sdfMemFreeDirect(fldAutoMapBuf);
    }
    fldAutoMapBuf = 0;
    fldAutoMapHeader = 0;
    fldAutoMapData = 0;
}

void fldAutoMapLoad(void) {
    s32 i;

    fldGlobalBase.nameU = -1;
    fldGlobalBase.nameD = -1;
    fldAutoMapFieldId.x = 0.0f;
    fldAutoMapFieldId.y = 0.0f;
    fldAutoMapFieldId.z = 0.0f;
    fldAutoMapFieldId.w = 1.0f;
    fldAutoMapPosX.x = 0.0f;
    fldAutoMapPosX.y = 0.0f;
    fldAutoMapPosX.z = 0.0f;
    fldAutoMapPosX.w = 1.0f;
    fldAutoMapBuf = NULL;
    fldAutoMapHeader = NULL;
    fldAutoMapData = 0;
    D_00551CC0[0] = NULL;
    D_00551CC0[1] = NULL;
    fldAutoMapWindowTex[0] = NULL;
    fldAutoMapWindowTex[1] = NULL;
    fldAutoMapWindowTex[2] = NULL;
    for (i = 0; i < 32; i++) {
        D_00312150[i] = NULL;
        fldAutoMapModelPos[i].x = 0.0f;
        fldAutoMapModelPos[i].y = 0.0f;
        fldAutoMapModelPos[i].z = 0.0f;
    }
}

void fldAutoMapDrawMdl1(sdfModelRootNode_t *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    sdfModelNode_t *handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = emitter->nodeList->items[0];
    SDF_VU0_LOADVECTOR_BARRIER(pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&emitter->tm.m[12]);
    sdfModelMakeLocalTm(handle);
    sdfModelEvalTm(emitter);
    sdfModelLinkOt(&D_002F6A48, emitter);
}

/* NON_MATCHING: 10 of 146 words, exact size; the two saved fp registers hold x
 * and y the other way round, which rotates all three arms. */
#ifdef NON_MATCHING
void fldAutoMapDrawIconOfsR(s32 index, s32 mode, f32 x, f32 y) {
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s16 *t = fldAutoMapIconOfsTable[index];
    s32 tu;
    s32 tv;
    s32 tw;
    s32 th;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 cx;
    s32 cy;
    s32 dx;
    s32 dy;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, D_00551CC0[0], 0);
    tu = t[0];
    tv = t[1];
    tw = t[2];
    th = t[3];
    u0 = tu * 16;
    v0 = tv * 16;
    u1 = u0 + tw * 16;
    v1 = v0 + th * 16;
    switch (mode) {
    case 4:
        y0 = (s32)(y * 16.0f) - t[4] * 8;
        x0 = (s32)(x * 16.0f) - t[5] * 16;
        y1 = y0 + tw * 8;
        x1 = x0 + th * 16;
        ax = x1;
        ay = y0;
        bx = x1;
        by = y1;
        cx = x0;
        cy = y0;
        dx = x0;
        dy = y1;
        break;
    case 8:
        y0 = (s32)(y * 16.0f) - t[5] * 8;
        x0 = (s32)(x * 16.0f) - t[4] * 16;
        y1 = y0 + th * 8;
        x1 = x0 + tw * 16;
        ax = x0;
        ay = y1;
        bx = x1;
        by = y1;
        cx = x0;
        cy = y0;
        dx = x1;
        dy = y0;
        break;
    default:
        y0 = (s32)(y * 16.0f) - t[5] * 8;
        x0 = (s32)(x * 16.0f) - t[4] * 16;
        y1 = y0 + th * 8;
        x1 = x0 + tw * 16;
        ax = x0;
        ay = y0;
        bx = x1;
        by = y0;
        cx = x0;
        cy = y1;
        dx = x1;
        dy = y1;
        break;
    }
    sdfPrimLinkFt4(tag, 0x80808080, 0x44, ax, ay, u0, v0, bx, by, u1, v0, cx, cy, u0, v1,
                   dx, dy, u1, v1, 0, 0);
    ot = fldDrawGetOt();
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))fldDrawGetOt()->regFunc)(ot, tag);
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapDrawIconOfsR);
#endif

extern f32 sdfSin(f32 radians);
extern void SetRotateMatrixY(f32 angle);
extern f32 D_0054EE70;
extern sdfTexHandle_t *fldAutoMapBgTex[];

/* NON_MATCHING: 187 of 207 words, exact size; the four texture-coordinate
 * locals hold the other register of each pair ($16 with $17, $18 with $19). */
#ifdef NON_MATCHING
void fldAutoMapDrawIconOfsRF(s32 index, f32 x, f32 y, f32 z) {
    sdfVector4_t v0;
    sdfVector4_t v1;
    sdfVector4_t v2;
    sdfVector4_t v3;
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s16 *t;
    u32 color;
    s32 u0;
    s32 vv0;
    s32 u1;
    s32 vv1;
    f32 x0;
    f32 z0;
    f32 x1;
    f32 z1;

    if (index == 1) {
        color = (s32)(sdfSin(D_0054EE70) * 64.0f) << 24;
        color += 0x80000000;
        D_0054EE70 = D_0054EE70 + 0.2f;
    } else {
        color = 0x80000000;
    }
    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, fldUiTexture, 0);
    t = fldAutoMapIconOfsTable[index];
    u0 = t[0] * 16;
    vv0 = t[1] * 16;
    u1 = t[2] * 16;
    vv1 = t[3] * 16;
    x0 = (f32)-(t[4] * 16);
    z0 = (f32)-(t[5] * 16);
    x1 = x0 + (f32)u1;
    z1 = z0 + (f32)vv1;
    u1 = u0 + u1;
    vv1 = vv0 + vv1;
    v0.x = x0;
    v0.y = 0.0f;
    v0.z = z0;
    v3.x = x1;
    v3.y = 0.0f;
    v3.z = z1;
    v1.x = x1;
    v1.y = 0.0f;
    v1.z = z0;
    v2.x = x0;
    v2.y = 0.0f;
    v2.z = z1;
    SetRotateMatrixY((z + 180.0f) * 3.14f / 180.0f);
    SDF_VU0_LOADVECTOR(&v0);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v0);
    SDF_VU0_LOADVECTOR(&v1);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v1);
    SDF_VU0_LOADVECTOR(&v2);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v2);
    SDF_VU0_LOADVECTOR(&v3);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v3);
    v0.x = v0.x + x * 16.0f;
    v0.z = v0.z * 0.5f + y * 16.0f;
    v1.z = v1.z * 0.5f + y * 16.0f;
    v1.x = v1.x + x * 16.0f;
    v2.x = v2.x + x * 16.0f;
    v2.z = v2.z * 0.5f + y * 16.0f;
    v3.x = v3.x + x * 16.0f;
    v3.z = v3.z * 0.5f + y * 16.0f;
    sdfPrimLinkFt4(tag, color | 0x808080, 0x44,
                   (s32)v0.x, (s32)v0.z, u0, vv0,
                   (s32)v1.x, (s32)v1.z, u1, vv0,
                   (s32)v2.x, (s32)v2.z, u0, vv1,
                   (s32)v3.x, (s32)v3.z, u1, vv1, 0, 0);
    ot = fldDrawGetOt();
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))fldDrawGetOt()->regFunc)(ot, tag);
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapDrawIconOfsRF);
#endif

void fldAutoMapDrawBgOfsRF(f32 x, f32 y, f32 z) {
    sdfVector4_t v0;
    sdfVector4_t v1;
    sdfVector4_t v2;
    sdfVector4_t v3;
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 au = 0;
    s32 av = 0;
    s32 du = 0x1000;
    s32 dv = 0x800;
    s32 bu = 0x1000;
    s32 bv = 0;
    s32 cu = 0;
    s32 cv = 0x800;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, fldAutoMapBgTex[0], 0);
    v0.x = -4800.0f;
    v0.y = 0.0f;
    v0.z = -4800.0f;
    v3.x = 4800.0f;
    v3.y = 0.0f;
    v3.z = 4800.0f;
    v1.x = 4800.0f;
    v1.y = 0.0f;
    v1.z = -4800.0f;
    v2.x = -4800.0f;
    v2.y = 0.0f;
    v2.z = 4800.0f;
    SetRotateMatrixY((z + 180.0f) * 3.14f / 180.0f);
    SDF_VU0_LOADVECTOR(&v0);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v0);
    SDF_VU0_LOADVECTOR(&v1);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v1);
    SDF_VU0_LOADVECTOR(&v2);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v2);
    SDF_VU0_LOADVECTOR(&v3);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&v3);
    v0.x = v0.x + x * 16.0f;
    v0.z = v0.z * 0.5f + y * 8.0f;
    v1.z = v1.z * 0.5f + y * 8.0f;
    v1.x = v1.x + x * 16.0f;
    v2.x = v2.x + x * 16.0f;
    v2.z = v2.z * 0.5f + y * 8.0f;
    v3.x = v3.x + x * 16.0f;
    v3.z = v3.z * 0.5f + y * 8.0f;
    sdfPrimLinkFt4(tag, 0x80808080, 0x44,
                   (s32)(v0.x + 28672.0f), (s32)(v0.z + 30976.0f), au, av,
                   (s32)(v1.x + 28672.0f), (s32)(v1.z + 30976.0f), bu, bv,
                   (s32)(v2.x + 28672.0f), (s32)(v2.z + 30976.0f), cu, cv,
                   (s32)(v3.x + 28672.0f), (s32)(v3.z + 30976.0f), du, dv, 0, 0);
    ot = fldDrawGetOt();
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))fldDrawGetOt()->regFunc)(ot, tag);
}

s32 fldAutoMap_GetFloor(s32 scene, u32 index) {
    s32 result = 1;
    fldMapRoom_t *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    fldMapItem_t *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->floor + 1;
            }
        }
    }
    return result;
}

void fldAutoMapNodeSet(sdfModelRootNode_t *model, s32 index, s32 clearFlag) {
    sdfModelNode_t *parent = model->nodeList->items[index + 1];
    sdfModelNode_t *first;
    sdfModelNode_t *node;
    sdfModelNode_t *next;
    sdfModelNode_t *headNext;
    u16 flags;

    if (parent->children == NULL) {
        return;
    }
    parent->children->flags &= ~SDF_MODEL_NODE_FLAG_HIDDEN;
    if (clearFlag == 0) {
        parent->children->flags |= SDF_MODEL_NODE_FLAG_HIDDEN;
    }

    first = parent->children;
    headNext = first->next;
    if (headNext == NULL) {
        return;
    }
    node = headNext;
    if (parent != node->parent || first == node) {
        return;
    }
    for (;;) {
        flags = node->flags & ~SDF_MODEL_NODE_FLAG_HIDDEN;
        node->flags = flags;
        if (clearFlag == 0) {
            node->flags = flags | SDF_MODEL_NODE_FLAG_HIDDEN;
        }
        next = node->next;
        if (next == NULL) {
            return;
        }
        node = next;
        if (parent != node->parent) {
            return;
        }
        if (first != node) {
            continue;
        }
        return;
    }
}

s32 fldAutoMapGetFloorMax(void) {
    s32 max = 0;
    fldMapRoom_t *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    fldMapItem_t *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->floor) {
                max = item->floor;
            }
        }
    }
    return max + 1;
}

INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapDrawOneArea);

void fldAutoMapDrawWindow(void) {
    SDF_VU0_LOADMATRIX(aProjMatrix);
    InversMatrix();
    sdf3DSetCamera(dds3ScreenParam, aProjMatrix);
    rsetOtOfs(0x4A);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    DrwSprite2D(0x1A, 0x52, 0x1CC, 0x154, 0, 0, 0x100, 0x80, 0x80808080,
                fldAutoMapWindowTex[0]);
    D_0054EEA0 = D_0054EEA0 - 1.0f;
    fldAutoMapDrawBgOfsRF(255.0f, 250.0f, D_0054EEA0);
    sdf3DSetCamera(dds3ScreenParam, D_002FA240);
    SDF_VU0_LOADMATRIX(D_002FA240);
    rsetOtOfs(0x51);
    rsetTest(1, 5, 0x81, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

/* NON_MATCHING: 148 of 195 words, exact size; the closing vector block is
 * scheduled differently and holds the two coordinates in other fp registers. */
#ifdef NON_MATCHING
void fldAutoMapSeqStart(void) {
    sdfVector4_t target;
    sdfVector4_t camera;
    sdfVector4_t up;
    f32 px;
    f32 pz;
    fldAmLightParam_t *light;
    fldMapRoom_t *rec;
    s32 i;
    f32 wx;
    f32 wz;
    f32 zero;

    if (fldAutoMapSeqTestFlag(1) == 1) {
        return;
    }
    if (fldGlobalBase.area < 10) {
        return;
    }
    dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
    p2sdPlaySE(0xE, 0x7F, 0x3F);
    rec = fldAutoMapHeader;
    for (i = 0; i < fldAutoMapData; i++, rec++) {
        fldMapPoint_t *pos;

        D_00312150[i] = sdfModelMakeEarly(0, rec->model);
        pos = rec->pos;
        fldAutoMapModelPos[i].x = pos->x;
        fldAutoMapModelPos[i].y = pos->y;
        fldAutoMapModelPos[i].z = pos->z;
    }
    D_00551CC0[0] = sdfTexSet(fldGlobalBase.pacRes[7]);
    D_00551CC0[1] = sdfTexSet(fldGlobalBase.pacRes[9]);
    fldAutoMapWindowTex[0] = sdfTexSet(fldGlobalBase.pacRes[1]);
    fldAutoMapWindowTex[1] = sdfTexSet(fldGlobalBase.pacRes[3]);
    fldAutoMapWindowTex[2] = sdfTexSet(fldGlobalBase.pacRes[5]);
    fldAutoMapSeqState = 0;
    fldAutoMapDrawArea = fldGlobalBase.amFloor;
    D_0054EE58 = fldGlobalBase.amFloor;
    fldAutoMapAreaNo = fldGlobalBase.floor;
    AutoMapSeq = 1;
    fldAllIconDel(fldGlobalBase.floor, &px, &pz);
    fldAutoMapSeqStep = 0;
    D_00551CB4 = (s32)((f32)(s32)fldGlobalBase.x + px);
    D_00551CB8 = (s32)((f32)(s32)fldGlobalBase.z + pz);
    D_00551CB0 = D_00551CB8;
    D_00551CAC = D_00551CB4;
    fldAutoMapSeqSetFlag(1);
    fldReleaseUnitPad();
    evtDisableMoonDraw();
    zero = 0.0f;
    wz = fldGlobalBase.z + pz + 200.0f;
    wx = fldGlobalBase.x + px;
    fldAutoMapFieldId.x = wx;
    fldAutoMapFieldId.y = zero;
    fldAutoMapFieldId.z = wz;
    fldAutoMapPosX.x = wx;
    fldAutoMapPosX.y = zero;
    fldAutoMapPosX.z = wz;
    up = D_00532230;
    target.x = (f32)D_00551CAC;
    target.y = zero;
    target.z = (f32)D_00551CB0;
    camera.x = (f32)D_00551CAC;
    camera.y = -24000.0f;
    camera.z = (f32)D_00551CB0;
    SDF_VU0_COPYVECTOR(&D_002F5E00, &camera);
    SDF_VU0_COPYVECTOR(&fldAutoMapRect, &target);
    SDF_VU0_COPYVECTOR(&D_002F5E10, &up);
    light = (fldAmLightParam_t *)&dds3LightParam2[0x20];
    light->near = 255.0f;
    light->far = 255.0f;
    light->scale = 1000.0f;
    light->bias = 20000.0f;
    light->mode = 0x108010;
    dds3ScreenParam2[4] = 2048.0f;
    dds3ScreenParam2[5] = 2048.0f;
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapSeqStart);
#endif

void fldAutoMapFree(void) {
    s32 i;

    for (i = 0; i < fldAutoMapData; i++) {
        if (D_00312150[i] != NULL) {
            sdfModelRootFree(D_00312150[i], 1, 1);
        }
    }
    for (i = 0; i < 32; i++) {
        D_00312150[i] = NULL;
        fldAutoMapModelPos[i].x = 0.0f;
        fldAutoMapModelPos[i].y = 0.0f;
        fldAutoMapModelPos[i].z = 0.0f;
    }
    if (D_00551CC0[0] != NULL) {
        sdfTexFreeDelayed(D_00551CC0[0]);
    }
    if (D_00551CC0[1] != NULL) {
        sdfTexFreeDelayed(D_00551CC0[1]);
    }
    if (fldAutoMapWindowTex[0] != NULL) {
        sdfTexFreeDelayed(fldAutoMapWindowTex[0]);
    }
    if (fldAutoMapWindowTex[1] != NULL) {
        sdfTexFreeDelayed(fldAutoMapWindowTex[1]);
    }
    if (fldAutoMapWindowTex[2] != NULL) {
        sdfTexFreeDelayed(fldAutoMapWindowTex[2]);
    }
    D_00551CC0[0] = NULL;
    D_00551CC0[1] = NULL;
    fldAutoMapWindowTex[0] = NULL;
    fldAutoMapWindowTex[1] = NULL;
    fldAutoMapWindowTex[2] = NULL;
    while (sdfDelayedResrcCheck()) {
        sdfDrawSync();
    }
    AutoMapSeq = 0;
    frSetFontDrawOT(0x48);
}

void fldAutoMapSeqEnd(void) {
    sdfDrawSync();
    sdfDrawSync();
    fldAutoMapFree();
    AutoMapSeq = 0;
    fldAutoMapSeqClearFlag(1);
    fldRegistUnitPad();
    evtEnableMoonDraw();
    fldMenuOpenFlag[0] = 4;
    frSetFontDrawOT(0x48);
    dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
}

u32 fldAutoMapChkMode(void) {
    return AutoMapSeq;
}

INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapCalc);

void fldAutoMapSetField(s32 field, s32 area, s32 block) {
    f32 x = fldGlobalBase.x;
    f32 z = fldGlobalBase.z;

    fldGlobalBase.blockID = block;
    fldAutoMapFieldId.z = z;
    fldAutoMapFieldId.x = x;
    fldGlobalBase.area = field;
    fldGlobalBase.floor = area;
    fldAutoMapAreaNo = area;
    fldAutoMapFieldId.y = 0.0f;
    fldAutoMapField = field % 100;
}

s32 fldAutoMapChkOpen(s32 area, s32 room, u32 item) {
    s32 floor = area % 100;

    if (floor < 10) {
        return 0;
    }
    floor -= 10;
    if (floor >= 30) {
        return (*(u64 *)((u8 *)DDS3_GBWK + 0x17F10 + (floor * 32 + room) * 8) >> item) & 1;
    }
    return (*(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (floor * 32 + room) * 8) >> item) & 1;
}

void fldAutoMapOpenBlock(s32 flagNumber) {
    s32 floor;

    if (flagNumber <= 0) {
        return;
    }
    if (fldEncountTestFlag(0x80) != 0) {
        return;
    }
    floor = fldAutoMapField % 100;
    if (floor < 10) {
        return;
    }
    floor -= 10;
    fldGlobalBase.blockID = flagNumber - 1;
    if (floor >= 30) {
        u32 ofs = 0x17F10 + (floor * 32 + fldGlobalBase.floor) * 8;

        *(u64 *)((u8 *)DDS3_GBWK + ofs) |= (u64)1 << (flagNumber - 1);
    } else {
        *(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (floor * 32 + fldGlobalBase.floor) * 8) |=
            (u64)1 << (flagNumber - 1);
    }
    fldGlobalBase.amSBlock = flagNumber;
    fldGlobalBase.amFloor = fldAutoMap_GetFloor(fldGlobalBase.floor, fldGlobalBase.blockID);
    if (fldAutoMapField == 25) {
        if (fldGlobalBase.floor == 0) {
            fldAutoMapOpenBlock2(fldAutoMapField, 17, flagNumber);
        }
        if (fldGlobalBase.floor == 16) {
            fldAutoMapOpenBlock2(fldAutoMapField, 1, flagNumber);
        }
        if (fldGlobalBase.floor == 1) {
            fldAutoMapOpenBlock2(fldAutoMapField, 18, flagNumber);
        }
        if (fldGlobalBase.floor == 17) {
            fldAutoMapOpenBlock2(fldAutoMapField, 2, flagNumber);
        }
        if (fldGlobalBase.floor == 2) {
            fldAutoMapOpenBlock2(fldAutoMapField, 19, flagNumber);
        }
        if (fldGlobalBase.floor == 18) {
            fldAutoMapOpenBlock2(fldAutoMapField, 3, flagNumber);
        }
        if (fldGlobalBase.floor == 3) {
            fldAutoMapOpenBlock2(fldAutoMapField, 20, flagNumber);
        }
        if (fldGlobalBase.floor == 19) {
            fldAutoMapOpenBlock2(fldAutoMapField, 4, flagNumber);
        }
        if (fldGlobalBase.floor == 4) {
            fldAutoMapOpenBlock2(fldAutoMapField, 21, flagNumber);
        }
        if (fldGlobalBase.floor == 20) {
            fldAutoMapOpenBlock2(fldAutoMapField, 5, flagNumber);
        }
    }
    if (fldAutoMapField == 34) {
        if (fldGlobalBase.floor == 1) {
            fldAutoMapOpenBlock2(fldAutoMapField, 3, flagNumber);
        }
        if (fldGlobalBase.floor == 2) {
            fldAutoMapOpenBlock2(fldAutoMapField, 2, flagNumber);
        }
    }
}

void fldAutoMapOpenBlock2(s32 area, s32 floor, s32 flagNumber) {
    s32 room = area % 100;
    s32 index = floor - 1;
    s32 bit = flagNumber - 1;

    if (room < 10) {
        return;
    }
    room -= 10;
    if (room >= 30) {
        u32 ofs = 0x17F10 + (room * 32 + index) * 8;

        *(u64 *)((u8 *)DDS3_GBWK + ofs) |= (u64)1 << bit;
    } else {
        *(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (room * 32 + index) * 8) |= (u64)1 << bit;
    }
}

/* NON_MATCHING: 40 of 42 words, exact size; the two `addiu -1` setups are swapped
 * between the slot before `div` and the `beql` delay slot. */
#ifdef NON_MATCHING
void fldAutoMapCloseBlock2(s32 area, s32 floor, s32 flagNumber) {
    s32 bit = flagNumber - 1;
    s32 room = area % 100;
    s32 index = floor - 1;

    if (room < 10) {
        return;
    }
    room -= 10;
    if (room >= 30) {
        u32 ofs = 0x17F10 + (room * 32 + index) * 8;

        *(u64 *)((u8 *)DDS3_GBWK + ofs) &= ~((u64)1 << bit);
    } else {
        *(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (room * 32 + index) * 8) &= ~((u64)1 << bit);
    }
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapCloseBlock2);
#endif

void fldAutoMapNameU(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        fldAreaNameUpper[0] = -1;
    } else {
        fldAreaNameUpper[0] = labelIndex;
    }
}

void fldAutoMapNameD(s32 labelIndex) {
    if (labelIndex >= 0x40) {
        fldAreaNameLower[0] = -1;
    } else {
        fldAreaNameLower[0] = labelIndex;
    }
}

s32 fldAutoMapGetFloorPrev(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    fldMapRoom_t *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldAutoMapChkOpen(fldAutoMapField, i, j) != 0) {
                item = fldAutoMap_GetFloor(i, j);
                if (item < limit && best < item) {
                    best = item;
                }
            }
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldAutoMapGetFloorNext(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = 999;
    fldMapRoom_t *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldAutoMapChkOpen(fldAutoMapField, i, j) != 0) {
                item = fldAutoMap_GetFloor(i, j);
                if (limit < item && item < best) {
                    best = item;
                }
            }
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}

void fldAllIconDel(s32 index, f32 *x, f32 *z) {
    s32 i;
    fldMapRoom_t *entry = fldAutoMapHeader;
    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        if (i == index) {
            fldMapPoint_t *position = entry->pos;
            *x = position->x;
            *z = position->z;
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

void fldAutoMapGetBBox(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ) {
    s32 room;
    u32 i;
    fldMapRoom_t *rec;
    fldMapItem_t *item;
    fldMapPoint_t *a;
    s32 visible;
    fldMapPoint_t *b;

    room = 0;
    *minX = 0.0f;
    *maxZ = 0.0f;
    *maxX = 0.0f;
    *minZ = 0.0f;
    rec = fldAutoMapHeader;
    for (; room < fldAutoMapData; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->count; i++, item++) {
            visible = 0;
            if (fldAutoMapChkOpen(fldAutoMapField, room, i) != 0) {
                visible = 1;
            }
            if (fldAutoMapBBox != 0) {
                visible = 1;
            }
            if (visible != 0) {
                a = item->pointA;
                if (a != NULL) {
                    if (*minX > a->x) {
                        *minX = a->x;
                    }
                    b = item->pointB;
                    if (*minX > b->x) {
                        *minX = b->x;
                    }
                    if (*maxX < a->x) {
                        *maxX = a->x;
                    }
                    if (*maxX < b->x) {
                        *maxX = b->x;
                    }
                    if (*maxZ < a->z) {
                        *maxZ = a->z;
                    }
                    if (*maxZ < b->z) {
                        *maxZ = b->z;
                    }
                    if (a->z < *minZ) {
                        *minZ = a->z;
                    }
                    if (b->z < *minZ) {
                        *minZ = b->z;
                    }
                }
            }
        }
    }
}

/* NON_MATCHING: 15 of 18 words, exact size; two `beql`/`bnel` where retail has
 * the plain branch, on the tests whose arm calls the still-assembly callee. */
#ifdef NON_MATCHING
void fldAutoMapProc(void) {
    if (fldGb[0] < 10) {
        return;
    }
    if (fldGb[0] >= 200) {
        return;
    }
    if (AutoMapSeq == 1) {
        fldAutoMapCalc();
    }
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapProc);
#endif

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532230);

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532240);

void fldAutoMapDraw(void) {
    char buf[0x20];
    s32 i;

    if (AutoMapSeq != 1) {
        return;
    }
    fldAutoMapDrawWindow();
    fldAutoMapDrawOneArea();
    if (fldDrawFlag[0] == 0) {
        return;
    }
    if (dds3PadButtonState[0x2B] == 0) {
        return;
    }
    sprintf(buf, "BIT DEBUG %d", fldAutoMapField);
    fldDbgPrint2D(0xC80, -0x2E0, buf);
    if (fldAutoMapField - 10 > 0 && fldAutoMapField - 10 < 30) {
        for (i = 0; i < 32; i++) {
            sprintf(buf, "%2d %016lx", i,
                    *(u64 *)((u8 *)DDS3_GBWK + 0xFC58 +
                             ((fldAutoMapField - 10) * 32 + i) * 8));
            fldDbgPrint2D(0xC80, i * 96 - 640, buf);
        }
    } else {
        for (i = 0; i < 32; i++) {
            sprintf(buf, "%2d %016lx", i,
                    *(u64 *)((u8 *)DDS3_GBWK + 0x17F10 +
                             ((fldAutoMapField - 10) * 32 + i) * 8));
            fldDbgPrint2D(0xC80, i * 96 - 640, buf);
        }
    }
}

void fldAutoMapClear(void) {
    u8 *gb;
    s32 i;
    s32 j;

    gb = (u8 *)DDS3_GBWK;
    for (i = 0; i < 30; i++) {
        u64 *row = (u64 *)((u8 *)(i * 256) + (u32)gb);

        row = (u64 *)((u8 *)row + 0xFC58);
        for (j = 0; j < 32; j++) {
            *row++ = 0;
        }
    }
    gb = (u8 *)DDS3_GBWK;
    for (i = 0; i < 8; i++) {
        u64 *row = (u64 *)(gb + 0x19D10 + i * 256);

        for (j = 0; j < 32; j++) {
            *row++ = 0;
        }
    }
}

extern f32 D_0054EEA4;
extern s32 fldAreaFontWork;
extern void frPreCalcStrWidth(const char *text, s32 small, s32 mode);
extern void *frwGetSmallFontFRQ(s32 x, s32 y, s32 mode, u32 rgba, const char *text, s32 flag);
extern u32 frGetStrWidth2(void *glyphChain);
extern void frDestroyFontRequestAfter(void *frq);
extern void fldDrawAddressPanel(s32 width, s32 x, s32 y);
extern void fldKanjiDraw(s32 x, s32 y, u32 colors, const char *text);

/* NON_MATCHING: 57 of 226 words, 908 bytes against 904; the frame holds one
 * saved register too many and the fade arms differ in two words. */
#ifdef NON_MATCHING
void fldDrawAreaBlockFont(void) {
    char *str;
    void *frq;
    s32 w;
    s32 x;
    s32 y;
    f32 v;

    if (fldGlobalBase.area >= 200) {
        return;
    }
    v = D_0054EEA4;
    switch (*(s32 *)((u8 *)&fldGlobalBase + 0x90)) {
    case 0:
        v = 0.0f;
        fldAreaFontWork = 0;
        D_0054EEA4 = v;
        break;
    case 1:
        v = 1.0f;
        fldAreaFontWork = 0;
        D_0054EEA4 = v;
        break;
    case 2:
        if (v > 0.0f) {
            v -= 0.1f;
            if (v < 0.0f) {
                v = 0.0f;
                fldAreaFontWork = 0;
            } else {
                fldAreaFontWork = -1;
            }
            D_0054EEA4 = v;
        }
        break;
    case 3:
        if (v < 1.0f) {
            v += 0.1f;
            if (!(v > 1.0f)) {
                fldAreaFontWork = 1;
            } else {
                v = 1.0f;
                fldAreaFontWork = 0;
            }
            D_0054EEA4 = v;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (v < 0.99f) {
            if (v > 0.01f) {
                if (fldAreaFontWork == -1) {
                    v -= 0.1f;
                    if (v < 0.0f) {
                        v = 0.0f;
                        fldAreaFontWork = 0;
                    }
                    D_0054EEA4 = v;
                } else if (fldAreaFontWork == 1) {
                    v += 0.1f;
                    if (v > 1.0f) {
                        v = 1.0f;
                        fldAreaFontWork = 0;
                    }
                    D_0054EEA4 = v;
                }
            }
        }
        break;
    }
    x = (s32)(v * 0.0f + (1.0f - v) * 250.0f);
    y = (s32)(v * 0.0f + (1.0f - v) * 0.0f);
    if (fldGlobalBase.nameD != -1) {
        if ((s16)D_00567F50[fldGlobalBase.nameD] < 2) {
            return;
        }
        str = D_0030D714[fldAutoMapFindPlace(fldGlobalBase.area % 100, fldGlobalBase.nameD)];
        frPreCalcStrWidth(str, 1, 0);
        frq = frwGetSmallFontFRQ(0, 0, 0, 0, str, 0);
        w = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        w += 8;
        fldDrawAddressPanel(w, x, y);
        fldKanjiDraw(x + 0x1EC - w, (y + 0x18) / 2, 0xF0DC6480, str);
    } else {
        if (fldGlobalBase.nameU == -1) {
            return;
        }
        if ((s16)D_00567EA0[fldGlobalBase.nameU] < 2) {
            return;
        }
        str = D_0030AB54[fldAutoMapFindArea(fldGlobalBase.area % 100, fldGlobalBase.nameU)];
        frPreCalcStrWidth(str, 1, 0);
        frq = frwGetSmallFontFRQ(0, 0, 0, 0, str, 0);
        w = frGetStrWidth2(frq);
        frDestroyFontRequestAfter(frq);
        w += 8;
        fldDrawAddressPanel(w, x, y);
        fldKanjiDraw(x + 0x1EC - w, (y + 0x18) / 2, 0xA09DC380, str);
    }
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldDrawAreaBlockFont);
#endif

void fldInitPacLoader(void) {
    fldPacRequest_t *request;
    sdfPacWork_t *work;
    sdfMemHandle_t *handle;
    u32 i;

    D_0054EF1C = NULL;
    D_0054EF20 = NULL;
    D_0054EF24 = NULL;
    fldTakara1File = sdfDevLoadFileEx("/fld/f/bin/takara1.PB", &fldTakara1Data, &fldTakara1Size);
    fldTakara3File = sdfDevLoadFileEx("/fld/f/bin/takara3.PB", &fldTakara3Data, &fldTakara3Size);
    fldTakara2File = sdfDevLoadFileEx("/fld/f/bin/takara2.PB", &fldTakara2Data, &fldTakara2Size);
    request = pacLoad("/fld/f/bin/fldmix.LB");
    pacSync(request);
    work = request->queueHead;
    for (i = 0; work != NULL; i++) {
        switch (i) {
    case 0:
        handle = work->resrc;
        D_00551D04 = handle;
        D_00551D08 = sdfMemGetPtr(handle);
        break;
    case 1:
        handle = work->resrc;
        fldPacFileHandle = handle;
        fldNpcPacData = sdfMemGetPtr(handle);
        break;
    case 2:
        handle = work->resrc;
        D_00551D1C = handle;
        fldTakaraPacData = sdfMemGetPtr(handle);
        break;
    case 3:
        handle = work->resrc;
        D_00551D0C = handle;
        D_00551D10 = sdfMemGetPtr(handle);
        break;
    case 4:
        handle = work->resrc;
        D_00551CCC = handle;
        D_00551CD0 = sdfMemGetPtr(handle);
        break;
    case 5:
        handle = work->resrc;
        D_00551CD4 = handle;
        D_00551CD8 = sdfMemGetPtr(handle);
        break;
    case 6:
        handle = work->resrc;
        D_00551CDC = handle;
        D_00551CE0 = sdfMemGetPtr(handle);
        break;
    case 7:
        handle = work->resrc;
        D_00551CE4 = handle;
        D_00551CE8 = sdfMemGetPtr(handle);
        break;
    case 8:
        handle = work->resrc;
        D_00551CEC = handle;
        D_00551CF0 = sdfMemGetPtr(handle);
        break;
    case 9:
        handle = work->resrc;
        D_00551CF4 = handle;
        D_00551CF8 = sdfMemGetPtr(handle);
        break;
    case 10:
        handle = work->resrc;
        D_00551D14 = handle;
        D_00551D18 = sdfMemGetPtr(handle);
        break;
    case 11:
        handle = work->resrc;
        D_0054EF28 = handle;
        D_0054EF2C = sdfMemGetPtr(handle);
        D_0054EF30 = 4;
        break;
    case 12:
        handle = work->resrc;
        D_0054EF3C = handle;
        D_0054EF40 = sdfMemGetPtr(handle);
        break;
    case 13:
        handle = work->resrc;
        fldGlobalBase.pacRes[6] = handle;
        fldGlobalBase.pacRes[7] = sdfMemGetPtr(handle);
        break;
    case 14:
        handle = work->resrc;
        fldGlobalBase.pacRes[8] = handle;
        fldGlobalBase.pacRes[9] = sdfMemGetPtr(handle);
        break;
    case 15:
        handle = work->resrc;
        fldGlobalBase.pacRes[0] = handle;
        fldGlobalBase.pacRes[1] = sdfMemGetPtr(handle);
        break;
    case 16:
        handle = work->resrc;
        fldGlobalBase.pacRes[2] = handle;
        fldGlobalBase.pacRes[3] = sdfMemGetPtr(handle);
        break;
    case 17:
        handle = work->resrc;
        fldGlobalBase.pacRes[4] = handle;
        fldGlobalBase.pacRes[5] = sdfMemGetPtr(handle);
        break;
        }
        work = work->next;
    }
    pacFileHandleFree_(request);
}

void fldInitBlurEffect(void) {
    fldBlurParam_t param;
    s32 i;

    if (fldGlobalBase.area >= 0xC8) {
        return;
    }
    D_0054EEDC = dds3EffectLoad(D_00551D18);
    if (fldGlobalBase.area >= 0xB) {
        D_0054EED0 = dds3EffectLoad(D_00551D08);
        fldDmgBlurHandle = NULL;
        fldBlurEffectId[0] = dds3EffectLoad(D_00551D10);
        fldNpcBlurHandle = NULL;
    }
    if (fldGlobalBase.area < 0xA) {
        fldUiTexture = sdfTexSet(D_00551CD0);
        fldVLTexture = sdfTexSet(D_00551CD8);
        fldVLBlurHandle = sdfTexSet(D_00551CE0);
    } else {
        fldUiTexture = sdfTexSet(D_00551CE8);
        fldVLTexture = NULL;
        fldVLBlurHandle = NULL;
    }
    fldEATexture = sdfTexSet(D_00551CF0);
    fldSnowBlurHandle = sdfTexSet(D_00551CF8);
    /* The casts pin the store order; assigning the array elements directly
     * swaps the first two stores. */
    for (i = 0; i < 0x40; i++) {
        *(void **)&fldBlurEffectTable[i] = NULL;
        *(void **)&D_00568E90[i] = NULL;
        *(void **)&D_00568F90[i] = NULL;
    }
    fldNpcModelFreeIdx = -1;
    param.unk00 = 0x3C;
    param.unk04 = 0xF;
    param.unk08 = 0.22f;
    param.unk0C = 0x1E848484;
    param.unk10 = 0x44;
    fldNpcFreeCount = NULL;
    param.unk14 = 0;
    param.unk18 = 0.13f;
    param.unk1C = 0;
    param.unk20 = -0x30;
    param.unk24 = 0x60;
    param.unk28 = 0x60;
    fldBlurEffectHandle = dds3EffectDistortionBlurCreate(&param);
}

void fldFreeBlurEffect(void) {
    s32 i;

    dds3EffectDistortionBlurDelete(fldBlurEffectHandle);
    if (fldUiTexture != NULL) {
        sdfTexFreeDelayed(fldUiTexture);
        fldUiTexture = NULL;
    }
    if (fldEATexture != NULL) {
        sdfTexFreeDelayed(fldEATexture);
        fldEATexture = NULL;
    }
    if (fldVLTexture != NULL) {
        sdfTexFreeDelayed(fldVLTexture);
        fldVLTexture = NULL;
    }
    if (fldVLBlurHandle != NULL) {
        sdfTexFreeDelayed(fldVLBlurHandle);
        fldVLBlurHandle = NULL;
    }
    if (fldSnowBlurHandle != NULL) {
        sdfTexFreeDelayed(fldSnowBlurHandle);
        fldSnowBlurHandle = NULL;
    }
    for (i = 0; i < 64; i++) {
        if (fldBlurEffectTable[i] != NULL) {
            sdfMemFreeDelayedDirect(fldBlurEffectTable[i]);
            fldBlurEffectTable[i] = NULL;
        }
    }
    for (i = 0; i < 32; i++) {
        if (fldModelSlotTable[i].effect != NULL) {
            dds3EffectDelete(fldModelSlotTable[i].effect);
            fldModelSlotTable[i].effect = NULL;
        }
    }
    if (D_0054EEDC != NULL) {
        dds3EffectDelete(D_0054EEDC);
        D_0054EEDC = NULL;
    }
    if (D_0054EED0 != NULL) {
        dds3EffectDelete(D_0054EED0);
        D_0054EED0 = NULL;
    }
    if (fldBlurEffectId[0] != NULL) {
        dds3EffectDelete(fldBlurEffectId[0]);
        fldBlurEffectId[0] = NULL;
    }
}

void fldClearModelSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        void *effect = fldModelSlotTable[i].effect;

        fldModelSlotTable[i].unk00 = 0;
        fldModelSlotTable[i].unk04 = 0;
        fldModelSlotTable[i].unk08 = 0;
        fldModelSlotTable[i].unk0C = 0;
        fldModelSlotTable[i].object = NULL;
        fldModelSlotTable[i].unk34 = 0;
        fldModelSlotTable[i].unk38 = 0;
        fldModelSlotTable[i].unk3C = 0;
        fldModelSlotTable[i].unk40 = 0;
        fldModelSlotTable[i].unk44 = 0;
        fldModelSlotTable[i].room = 0;
        fldModelSlotTable[i].unk4C = 0;
        if (effect != NULL) {
            dds3EffectDelete(effect);
        }
        fldModelSlotTable[i].effect = NULL;
    }
    fldNpcModelTable = 0;
    fldNpcLoadIdx = 0;
    fldNpcCamTarget = 0;
}

void fldClearTakaraSlots(void) {
    s32 i;

    fldTakaraCount = 0;
    for (i = 0; i < 32; i++) {
        void *effect = fldTakaraSlotTable[i].effect;

        fldTakaraSlotTable[i].id = -1;
        fldTakaraSlotTable[i].unk00 = 0;
        fldTakaraSlotTable[i].activationState = 0;
        fldTakaraSlotTable[i].activationRequested = 0;
        if (effect != NULL) {
            dds3EffectDelete(effect);
        }
        fldTakaraSlotTable[i].effect = NULL;
    }
    fldTakaraSlotCount = 0;
}

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532338);

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532348);

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532358);

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_00532478);

INCLUDE_RODATA(const s32, "field/fldAutoMap", D_005326B8);

