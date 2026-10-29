#include "fileManager.h"
#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "fldNpc_tag.h"
#include "fldTakaraWork_tag.h"
#include "ps2asm.h"

typedef struct
{
    u8 opaque00[4];
    fldAmAreaData_t *areaTable; /* 0x04: first relocated area row */
    s32 areaCount;              /* 0x08: number of area rows */
} fldAmInfoHeader_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern sdfMemHandle_t *fldAutoMapBuf;

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern fldAmAreaData_t *fldAutoMapHeader;

extern s32 fldAutoMapData;

extern void *memset(void *a, s32 b, u32 c);

extern void sdfModelMakeLocalTm(sdfModelNode_t *aNode);

extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

extern sdfModelOt_t D_002F6A48;

extern void sdfModelLinkOt(sdfModelOt_t *aOt, sdfModelRootNode_t *aRoot);

extern u32 AutoMapSeq;

extern s32 fldAreaNameUpper[];

extern s32 fldAreaNameLower[];

extern s32 fldAutoMapField;

extern s32 fldAutoMapChkOpen(s32 fld, s32 area, u32 block);

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

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern u32 strlen(const char *s);

dds3Basic_t *dds3GetActiveScene(void);

extern void fldRegistUnitPad(void);

extern void fldAutoMapOpenBlock2(s32 fld, s32 area, s32 SBlock);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b,
                      u32 a);

struct sdf3DScreenParam_t;
extern void sdf3DSetCamera(struct sdf3DScreenParam_t *aParam, void *aCamera);

extern struct sdf3DScreenParam_t dds3ScreenParam;

extern u8 aProjMatrix[];

extern u8 D_002FA240[];

extern void InversMatrix(void);

extern void fldAutoMapDrawBgOfsRF(f32 x, f32 y, f32 r);

extern f32 D_0054EEA0;

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern sdfVector4_t fldAutoMapFieldId;

extern sdfVector4_t fldAutoMapPosX;

extern s32 fldAutoMapAreaNo;

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

#include "sdk_libc.h"

extern void fldAddressResolver(u8 *pFileTop, void *pData);

extern u8 *fldGetInfoHeader(void *pData);

extern void fldAutoMapFontInit(s32 bank);

extern void fldAmModelLoadFile(void);

extern void sdfDrawSync(void);

extern void fldAutoMapFree(void);

extern void fldAutoMapSeqClearFlag(s32 flag);

extern void evtEnableMoonDraw(void);

extern s32 fldMenuOpenFlag[];

extern void frSetFontDrawOT(u32 OtType);

extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 visible);

extern void *fldBlurEffectHandle;

extern void dds3EffectDistortionBlurDelete(void *hDistortionBlur);

extern sdfTexHandle_t *fldEATexture;

extern sdfTexHandle_t *fldVLTexture;

extern sdfTexHandle_t *fldVLBlurHandle;

extern sdfTexHandle_t *fldSnowBlurHandle;

extern sdfMemHandle_t *fldBlurEffectTable[];

extern void *fldBlurEffectId[];

extern void *D_0054EED0;

extern u16 D_00567EA0[];

extern s16 D_00567EF8[];

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

extern void *dds3EffectLoad(void *pD3P);

extern void *D_00551D08;

extern void *D_00551D10;

extern void *D_00551D18;

extern void *fldDmgBlurHandle;

extern void *fldNpcBlurHandle;

extern sdfTexHandle_t *sdfTexSet(void *aTex);

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

extern void *dds3EffectDistortionBlurCreate(fldBlurParam_t *pParam);

extern void dds3EffectDelete(void *pData);

extern struct effNode_t *fldTakaraSlotCount;

extern struct effNode_t *fldNpcLoadIdx;

extern void fldMakeFilePath(char *pathstr, s32 field, s32 area);


extern sdfModelRootNode_t *D_00312150[];

extern fldAmXYZ_t fldAutoMapModelPos[32];

extern sdfTexHandle_t *D_00551CC0[2];

extern sdfTexHandle_t *fldAutoMapWindowTex[];

extern void sdfModelRootFree(void *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);

extern s32 sdfDelayedResrcCheck(void);

extern s32 fldDrawFlag[];

extern s8 dds3PadButtonState[64];

extern void fldDbgPrint2D(s32 x, s32 y, const char *str);

extern void fldAutoMapDrawOneArea(void);

extern s32 fldEncountTestFlag(s32 flag);

void fldAllIconDel(s32 index, f32 *x, f32 *z);

extern s32 fldAutoMapSeqTestFlag(s32 flag);

extern void fldAutoMapSeqSetFlag(s32 flag);

extern void fldReleaseUnitPad(void);

extern void evtDisableMoonDraw(void);

extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);

extern void *sdfModelMakeEarly(s32 aTexList, void *aModelData);

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


extern fileRequest_t *pacLoad(const char *filename);

extern void pacSync(fileRequest_t *pacFileHandle);

extern void pacFileHandleFree_(fileRequest_t *pacHandle);

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);

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

extern struct effResourceHeader_t *fldNpcPacData;

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

extern sdfDrawTag_t *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

extern sdfGrOt_t *fldDrawGetOt(void);

extern void sdfPrimLinkFt4(sdfDrawTag_t *aTag, u32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                           s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2,
                           s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ, void *aAllocator);

void fldAutoMapFontInit(s32 bank) {
    s32 i;

    for (i = 0; i < 0x29; i++) {
        D_00567EA0[i] = strlen(D_0030AB54[fldAutoMapFindArea(bank, i)]);
    }
    for (i = 0; i < 0x29; i++) {
        D_00567EF8[i] = strlen(D_0030FBD4[fldAutoMapFindFloor(bank, i)]);
    }
    for (i = 0; i < 0x18; i++) {
        D_00567F50[i] = strlen(D_0030D714[fldAutoMapFindPlace(bank, i)]);
    }
    i = 0;
    if (fldGb[0] < 50) {
        i = fldGb[0];
    }
    D_00551CC8 = strlen(D_0030A670[i]);
}


void fldAmModelLoadFile(void) {
    char path[0x80];
    char dir[0x40];
    fldAreaWork_t *work = &fldGlobalBase;
    u8 *data;
    u8 *transferStart;
    s32 floor;
    fldAmInfoHeader_t *info;
    fldAmAreaData_t *rows;
    s32 count;

    fldMakeFilePath(dir, work->area, 1);
    if (work->area == 0x19 && work->floor + 1 >= 0x11) {
        sprintf(path, "%sf%03db.amb", dir, 0x19);
    } else if (fldGlobalBase.area == 0x22) {
        floor = fldGlobalBase.floor + 1;
        if (floor < 4) {
            sprintf(path, "%sf%03d.amb", dir, 0x22);
        } else if (floor < 0xB) {
            sprintf(path, "%sf%03db.amb", dir, 0x22);
        } else if (floor < 0x13) {
            sprintf(path, "%sf%03dc.amb", dir, 0x22);
        } else if (floor < 0x1A) {
            sprintf(path, "%sf%03dd.amb", dir, 0x22);
        } else {
            goto plain;
        }
    } else {
    plain:
        sprintf(path, "%sf%03d.amb", dir, fldGlobalBase.area);
    }
    fldAutoMapBuf = sdfDevLoadFileEx(path, (u32 *)&data, 0);
    transferStart = data + 8;
    fldAddressResolver(data, transferStart);
    info = (fldAmInfoHeader_t *)fldGetInfoHeader(transferStart);
    rows = info->areaTable;
    count = info->areaCount;
    fldAutoMapData = count;
    fldAutoMapHeader = rows;
}


void fldAmbLoadFromMem(u8 *data, sdfMemHandle_t *mh) {
    s32 field = fldGb[0];
    u32 *info;

    if (field < 200) {
        if (field >= 10) {
            fldAutoMapBuf = mh;
            fldAddressResolver(data, data + 8);
            info = (u32 *)fldGetInfoHeader(data + 8);
            {
                u32 mapHeader = info[1];
                u32 mapData = info[2];

                fldAutoMapData = mapData;
                fldAutoMapHeader = (fldAmAreaData_t *)mapHeader;
            }
        }
    }
}

void fldAutoMapModelLoad(void) {
    fldAreaWork_t *work = &fldGlobalBase;

    if (work->area < 200) {
        fldAutoMapFontInit(work->area % 100);
        /* Keep the one-pass model phase visible as one block; see
         * docs/levers/callee-visibility.md for the matched call delay slot. */
        do {
            s32 area = fldGlobalBase.area;
            if (area >= 10) {
                fldAmModelLoadFile();
            } else {
                break;
            }
        } while (0);
    }
}

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

extern f32 sdfSin(f32 aRadius);
extern void SetRotateMatrixY(f32 aAngle);
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

void fldAutoMapDrawBgOfsRF(f32 x, f32 y, f32 r) {
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
    SetRotateMatrixY((r + 180.0f) * 3.14f / 180.0f);
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

s32 fldAutoMap_GetFloor(s32 area, u32 block) {
    s32 result = 1;
    fldAmAreaData_t *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    fldAmSBlock_t *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->nSB; j++, item++) {
            if (i == area && j == block) {
                result = item->floor + 1;
            }
        }
    }
    return result;
}

void fldAutoMapNodeSet(sdfModelRootNode_t *root_node, s32 nodeidx, s32 mode) {
    sdfModelNode_t *parent = root_node->nodeList->items[nodeidx + 1];
    sdfModelNode_t *first;
    sdfModelNode_t *node;
    sdfModelNode_t *next;
    sdfModelNode_t *headNext;
    u16 flags;

    if (parent->children == NULL) {
        return;
    }
    parent->children->flags &= ~SDF_MODEL_NODE_FLAG_HIDE;
    if (mode == 0) {
        parent->children->flags |= SDF_MODEL_NODE_FLAG_HIDE;
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
        flags = node->flags & ~SDF_MODEL_NODE_FLAG_HIDE;
        node->flags = flags;
        if (mode == 0) {
            node->flags = flags | SDF_MODEL_NODE_FLAG_HIDE;
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
    fldAmAreaData_t *rec = fldAutoMapHeader;
    s32 i;
    u32 j;
    fldAmSBlock_t *item;

    for (i = 0; i < (s32)fldAutoMapData; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->nSB; j++, item++) {
            if (max < item->floor) {
                max = item->floor;
            }
        }
    }
    return max + 1;
}

#ifdef NON_MATCHING
struct frFontHandle_t;
struct frFontRequest_t;
extern struct frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 priority, s32 color,
                                                  const char *text, struct frFontRequest_t *previous);
extern u32 frGetStrWidth2(struct frFontHandle_t *font);
extern s32 frDestroyFontRequestAfter(struct frFontHandle_t *font);
extern void frSetPriority(struct frFontHandle_t *font, u32 priority);
extern s32 frDrawFont(struct frFontHandle_t *font);
extern s32 frDrawFontEx(struct frFontHandle_t *font, s8 mode);
extern struct frFontHandle_t *D_00551CA0;
extern const char D_005321C0[];
extern s32 fldWmapMode, gFldNowDZone;
extern s32 fldAutoMapDrawState, fldAutoMapDrawY;
extern f32 fldAutoMapDrawX, fldCamPitch;
extern s32 D_0054EE78, D_0054EE7C, D_0054EE90;
extern f32 D_0054EE80, D_0054EE94, D_0054EE98, D_0054EE9C;
extern f32 D_0054C4A4, D_0054C4A8, D_0054C4AC, D_0054C4B0;
extern f32 D_0054C4B4, D_0054C4B8, D_0054C4BC, D_0054C4C0;
extern s32 evtBitCheck(s32 flag);
extern s32 fldAutoMapGetFloorPrev(s32 floor);
extern s32 fldAutoMapGetFloorNext(s32 floor);
extern void fldAutoMapGetBBox(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ);
extern void fldPoint3To2H(f32 *x, f32 *y, f32 px, f32 py, f32 pz);
extern void fldAutoMapDrawIconOfsR(s32 index, s32 mode, f32 x, f32 y);
extern void fldAutoMapDrawIconOfsRF(s32 index, f32 x, f32 y, f32 rotation);
extern void rsetFrameMaskRgbOn(void);
extern void rsetFrameMaskRgbOff(void);
extern f32 sdfSin(f32 angle);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h,
                      u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1,
                      u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);
extern void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h,
                       u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1,
                       u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);

static inline s32 fldAutoMapNodeVisible(fldAmSBlock_t *item, s32 area, u32 block,
                                        s32 floor, s32 checkOpen, s32 specialRoom)
{
    s32 visible = 0;

    if (floor == item->floor) {
        if (!checkOpen || fldAutoMapBBox != 0 || fldAutoMapChkOpen(fldAutoMapField, area, block) != 0) {
            visible = 1;
        }
    }
    if (specialRoom && fldGlobalBase.area == 20 && area == 7) {
        if (evtBitCheck(0x4EB) != 0) {
            if (block == 0) {
                visible = 0;
            }
        } else if (block == 2 || block == 4) {
            visible = 0;
        }
    }
    return visible;
}

static inline s32 fldAutoMapIconNumber(u32 type, s32 adjacent)
{
    switch (type) {
    case 1: return 10;
    case 2: return adjacent ? -1 : 11;
    case 3: return adjacent ? -1 : 12;
    case 4: return 5;
    case 5: return adjacent ? -1 : 2;
    case 6: return 4;
    case 7: return 6;
    case 8: return 7;
    case 9: return 3;
    case 10: return adjacent ? -1 : 14;
    case 11: return adjacent ? -1 : 13;
    case 12: return adjacent ? -1 : 15;
    default: return -1;
    }
}

void fldAutoMapDrawOneArea(void)
{
    sdfVector4_t rect __attribute__((aligned(16)));
    s32 floor, maxFloor, room, state;
    u32 block, icon;
    f32 x, y, offsetX, offsetZ, minX, maxZ, maxX, minZ;
    s32 blink;
    volatile s32 firstColor;
    struct frFontRequest_t *request;
    struct frFontHandle_t *font;
    s32 textX;
    const char *text;

    if (fldWmapMode != 0) {
        s32 showAll = fldAutoMapBBox;
        if ((dds3PadButtonState[0x29] != 0 && dds3PadButtonState[0x2B] < 0) ||
            (dds3PadButtonState[0x2B] != 0 && dds3PadButtonState[0x29] < 0)) {
            showAll++;
        }
        fldAutoMapBBox = showAll & 1;
    }
    if (fldAutoMapBBox != 0) {
        fldDbgPrint2D(0x640, -0x280, D_005321C0);
    }
    maxFloor = fldAutoMapGetFloorMax();
    frSetFontDrawOT(0x51);
    SDF_VU0_LOADMATRIX(aProjMatrix);
    InversMatrix();
    floor = fldAutoMapDrawArea;
    D_0054EE78 = floor;
    if (dds3PadButtonState[0x28] < 0 && floor < maxFloor) {
        s32 next = floor + 1;
        if (fldAutoMapBBox == 0) {
            next = fldAutoMapGetFloorNext(floor);
        }
        floor = D_0054EE78;
        if (floor != next) {
            p2sdPlaySE(2, 0x7F, 0x3F);
            D_0054EE7C = D_0054EE78;
            floor = next;
            D_0054EE80 = D_0054C4A4;
            fldAutoMapDrawState = -24;
            D_0054EE78 = floor;
            fldAutoMapDrawX = 0.0f;
            fldAutoMapDrawY = 0;
            D_0054EE90 = 0;
        }
    }
    if (dds3PadButtonState[0x2A] < 0 && floor >= 2) {
        s32 previous = floor - 1;
        if (fldAutoMapBBox == 0) {
            previous = fldAutoMapGetFloorPrev(floor);
        }
        floor = D_0054EE78;
        if (floor != previous) {
            p2sdPlaySE(2, 0x7F, 0x3F);
            D_0054EE7C = D_0054EE78;
            floor = previous;
            D_0054EE80 = D_0054C4A8;
            fldAutoMapDrawState = 24;
            D_0054EE78 = floor;
            fldAutoMapDrawX = 0.0f;
            fldAutoMapDrawY = 0;
            D_0054EE90 = 0;
        }
    }
    state = fldAutoMapDrawState;
    if (state < 0) {
        state += 4;
        fldAutoMapDrawState = state;
        D_0054EE90 += 4;
    }
    if (state > 0) {
        fldAutoMapDrawState = state - 4;
        D_0054EE90 -= 4;
    }
    if (!(D_0054EE80 < 1000.0f) || !(D_0054EE80 > -1000.0f)) {
        if (D_0054EE80 > 0.0f) {
            fldAutoMapDrawX -= 500.0f;
        } else if (D_0054EE80 < 0.0f) {
            fldAutoMapDrawX += 500.0f;
        }
    }
    fldAutoMapDrawY = 0x80;
    fldAutoMapDrawArea = floor;
    D_0054EE80 = 0.0f;
    sdf3DSetCamera(&dds3ScreenParam, aProjMatrix);
    rsetOtOfs(0x4A);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0);
    rsetOtOfs(0x4B);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetOtOfs(0x4A);
    rsetTest(1, 1, 0x80, 1, 0, 0, 1, 1);
    if (dds3PadButtonState[0x29] != 0) {
        s32 adjacentFloor;
        rsetAlpha(0);
        rsetFrameMaskRgbOn();
        rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
        adjacentFloor = D_0054EE78 - 1;
        if (fldAutoMapDrawY >= 0x80) {
            for (room = 0; room < fldAutoMapData; room++) {
                fldAmAreaData_t *area = &fldAutoMapHeader[room];
                offsetX = offsetZ = 0.0f;
                for (block = 0; block < area->nSB; block++) {
                    fldAmSBlock_t *item = &area->items[block];
                    if ((item->floor == adjacentFloor - 1 || item->floor == adjacentFloor + 1) &&
                        (fldAutoMapBBox != 0 || fldAutoMapChkOpen(fldAutoMapField, room, block) != 0) &&
                        item->icons != NULL) {
                        for (icon = 0; icon < item->iconCount; icon++) {
                            fldAmIcon_t *entry = &item->icons[icon];
                            s32 number = fldAutoMapIconNumber(entry->type, 1);
                            if (number >= 0) {
                                fldPoint3To2H(&x, &y, entry->position->x + offsetX + 0.0f,
                                              D_0054EE80, entry->position->z + offsetZ + 0.0f);
                                fldAutoMapDrawIconOfsR(number, 0, x, y);
                            }
                        }
                    }
                }
            }
        }
        for (room = 0; room < fldAutoMapData; room++) {
            fldAmAreaData_t *area = &fldAutoMapHeader[room];
            for (block = 0; block < area->nSB; block++) {
                fldAmSBlock_t *item = &area->items[block];
                s32 visible = 0;
                if (item->floor == adjacentFloor - 1 || item->floor == adjacentFloor + 1) {
                    visible = fldAutoMapBBox != 0 || fldAutoMapChkOpen(fldAutoMapField, room, block) != 0;
                }
                fldAutoMapNodeSet(D_00312150[room], item->nodeIndex, visible);
            }
            D_00312150[room]->color = 0x80808080;
            fldAutoMapDrawMdl1(D_00312150[room], 0.0f, D_0054EE80, 0.0f);
        }
        rsetFrameMaskRgbOff();
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 1, 1, 1, 1, 1);
        aDrawBoxF(0, 0, 0x200, 0xE0, 0x40, 0x40, 0x60, 0x30);
        rsetTest(1, 1, 0x80, 1, 0, 0, 1, 1);
    }
    floor = D_0054EE78 - 1;
    for (room = 0; room < fldAutoMapData; room++) {
        fldAmAreaData_t *area = &fldAutoMapHeader[room];
        for (block = 0; block < area->nSB; block++) {
            s32 visible = fldAutoMapNodeVisible(&area->items[block], room, block, floor, 1, 1);
            fldAutoMapNodeSet(D_00312150[room], area->items[block].nodeIndex, visible);
        }
    }
    for (room = 0; room < fldAutoMapData; room++) {
        D_00312150[room]->color = 0x80808080;
        fldAutoMapDrawMdl1(D_00312150[room], 0.0f, D_0054EE80, 0.0f);
    }
    rsetOtOfs(0x4A);
    rsetAlpha(0);
    rsetFrameMaskRgbOn();
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0);
    floor = D_0054EE78 - 1;
    for (room = 0; room < fldAutoMapData; room++) {
        fldAmAreaData_t *area = &fldAutoMapHeader[room];
        sdfModelRootNode_t *model;
        for (block = 0; block < area->nSB; block++) {
            s32 visible = fldAutoMapNodeVisible(&area->items[block], room, block, floor, 1, 1);
            fldAutoMapNodeSet(D_00312150[room], area->items[block].nodeIndex, visible);
        }
        model = D_00312150[room];
        model->color = 0x80808080;
        fldAutoMapDrawMdl1(model, -50.0f, D_0054EE80, 50.0f);
        fldAutoMapDrawMdl1(D_00312150[room], 50.0f, D_0054EE80, 50.0f);
        fldAutoMapDrawMdl1(D_00312150[room], -50.0f, D_0054EE80, -50.0f);
        fldAutoMapDrawMdl1(D_00312150[room], 50.0f, D_0054EE80, -50.0f);
        for (block = 0; block < area->nSB; block++) {
            s32 visible = fldAutoMapNodeVisible(&area->items[block], room, block, floor, 0, 1);
            fldAutoMapNodeSet(D_00312150[room], area->items[block].nodeIndex, visible);
        }
        model = D_00312150[room];
        model->color = 0x10808080;
        fldAutoMapDrawMdl1(model, 0.0f, D_0054EE80, 0.0f);
        D_00312150[room]->color = 0x80808080;
    }
    rsetOtOfs(0x4E);
    rsetFrameMaskRgbOff();
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 1, 1, 1, 1, 1);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0x80, 0x80, 0xA2, 0x80);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    sdf3DSetCamera(&dds3ScreenParam, D_002FA240);
    D_0054EE98 += D_0054C4AC;
    D_0054EE9C += D_0054C4B0;
    SDF_VU0_COPYVECTOR(&rect, &fldAutoMapRect);
    if (fldAutoMapDrawY >= 0x80) {
        s32 currentFloor = D_0054EE78;
        floor = currentFloor - 1;
        for (room = 0; room < fldAutoMapData; room++) {
            fldAmAreaData_t *area = &fldAutoMapHeader[room];
            offsetX = offsetZ = 0.0f;
            for (block = 0; block < area->nSB; block++) {
                fldAmSBlock_t *item = &area->items[block];
                s32 visible = 0;
                if (item->floor == floor) {
                    visible = fldAutoMapBBox != 0 || fldAutoMapChkOpen(fldAutoMapField, room, block) != 0;
                }
                if (visible && item->icons != NULL) {
                    for (icon = 0; icon < item->iconCount; icon++) {
                        fldAmIcon_t *entry = &item->icons[icon];
                        s32 number = fldAutoMapIconNumber(entry->type, 0);
                        f32 bob = 0.0f;
                        if (entry->type == 11) {
                            bob = sdfSin(D_0054EE9C + (f32)(room + (s32)icon) * D_0054C4B4) * 30.0f;
                        }
                        if (number >= 0) {
                            f32 px = entry->position->x + offsetX + 0.0f;
                            f32 pz = entry->position->z + offsetZ + bob;
                            f32 dx = sdfFabs(rect.x - px);
                            f32 dz = sdfFabs(rect.z - pz);
                            if (dx < D_0054C4B8 && dz < D_0054C4B8) {
                                fldPoint3To2H(&x, &y, px, D_0054EE80, pz);
                                fldAutoMapDrawIconOfsR(number, 0, x, y);
                            }
                        }
                    }
                }
            }
        }
        if (currentFloor == fldGlobalBase.amFloor) {
            f32 px, pz, dx, dz;
            s32 darkZone;
            fldAllIconDel(fldAutoMapAreaNo, &offsetX, &offsetZ);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(1);
            px = fldGlobalBase.x + offsetX;
            pz = fldGlobalBase.z + offsetZ;
            dx = sdfFabs(rect.x - px);
            dz = sdfFabs(rect.z - pz);
            fldPoint3To2H(&x, &y, px, D_0054EE80, pz);
            darkZone = gFldNowDZone;
            if (dx < D_0054C4BC && dz < D_0054C4BC) {
                rsetAlpha(1);
                fldAutoMapDrawIconOfsRF(1, x, y, -fldCamPitch);
                rsetAlpha(0);
                fldAutoMapDrawIconOfsRF(0, x, y, fldGlobalBase.camAngle);
            }
            if (darkZone != 0 && fldGlobalBase.lightMaIdx == 0) {
                s32 spriteX, spriteY;
                rsetOtOfs(0x51);
                rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
                rsetAlpha(1);
                aDrawBoxF(0x18, 0x29, 0x1D0, 0xE0, 0, 0, 0, 0);
                aDrawBoxF((s32)(x - 2048.0f) + 0xDC, (s32)(y - 2048.0f) + 0x67, 0x48, 0x25, 0, 0, 0, 0x80);
                rsetTest(1, 0, 0x80, 1, 1, 0, 1, 1);
                rsetAlpha(0);
                aDrawBoxF(0x18, 0x29, 0x1D0, 0xE0, 8, 0x11, 0x1A, 0x80);
                rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
                spriteX = (s32)(x - 2048.0f) + 0xDC;
                spriteY = (s32)(y - 2048.0f) * 2;
                DrwSprite2D(spriteX, spriteY + 0xCD, 0x48, 0x26, 0x30, 0x51, 0x48, 0x26, 0x80808080, D_00551CC0[0]);
                DrwSprite2D(spriteX, spriteY + 0xF3, 0x48, 0x26, 0x30, 0x76, 0x48, -0x26, 0x80808080, D_00551CC0[0]);
                aDrawBoxF(0, 0x29, 0x18, 0xAA, 0, 0, 0, 0x80);
                aDrawBoxF(0x1E8, 0x29, 0x18, 0xAA, 0, 0, 0, 0x80);
                aDrawBoxG(0x18, 0x29, 0x7B, 0xAA, 0, 0, 0, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80);
                aDrawBoxG(0x16D, 0x29, 0x7B, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0);
            }
        }
    }
    rsetOtOfs(0x51);
    rsetAlpha(0);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    DrwSprite2D(0, 0, 0x100, 0x80, 0, 0, 0x100, 0x80, 0x80808080, fldAutoMapWindowTex[1]);
    DrwSprite2D(0x200, 0, -0x100, 0x80, 0, 0, 0xFF, 0x80, 0x80808080, fldAutoMapWindowTex[1]);
    DrwSprite2D(0, 0x80, 0x26, 0xF8, 0, 0x7E, 0x26, 1, 0x80808080, fldAutoMapWindowTex[1]);
    DrwSprite2D(0x200, 0x80, -0x26, 0xF8, 0, 0x7E, 0x25, 1, 0x80808080, fldAutoMapWindowTex[1]);
    DrwSprite2D(0, 0x1F8, 0x100, -0x80, 0, 0, 0x100, 0x7F, 0x80808080, fldAutoMapWindowTex[1]);
    DrwSprite2D(0x200, 0x1F8, -0x100, -0x80, 0, 0, 0xFF, 0x7F, 0x80808080, fldAutoMapWindowTex[1]);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetFrameMaskRgbOn();
    aDrawBoxG(0, 0, 0x200, 0xE0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    aDrawBoxGF(24.0f, 79.0f, 464.0f, 346.0f, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80);
    if (gFldNowDZone != 0 && fldGlobalBase.lightMaIdx == 0) {
        aDrawBoxF(0, 0x29, 0x18, 0xAA, 0, 0, 0, 0x80);
        aDrawBoxF(0x1E8, 0x29, 0x18, 0xAA, 0, 0, 0, 0x80);
    }
    rsetTest(1, 0, 0x80, 1, 1, 0, 1, 1);
    rsetFrameMaskRgbOff();
    rsetAlpha(1);
    aDrawBoxG(-0x58, 0, 0x258, 0x7D, 0x16, 0x13, 0x49, 0, 0x16, 0x13, 0x49, 0x66, 0x16, 0x13, 0x49, 0, 0x16, 0x13, 0x49, 0);
    aDrawBoxG(0, 0x4F, 0x118, 0x91, 0x1E, 0x24, 0x38, 0, 0x1E, 0x24, 0x38, 0, 0x1E, 0x24, 0x38, 0, 0x1E, 0x24, 0x38, 0x4C);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    rsetAlpha(0);
    DrwSprite2D(0xE, 0x34, 0xDA, 0x17, 1, 1, 0xDA, 0x17, 0x80808080, D_00551CC0[1]);
    floor = D_0054EE78;
    if (floor != fldAutoMapGetFloorNext(D_0054EE78)) {
        DrwSprite2D(0x17, 0x36, 0x3D, 0x13, 1, 0x1A, 0x3D, 0x13, 0x80808080, D_00551CC0[1]);
    }
    if (D_0054EE78 != fldAutoMapGetFloorPrev(floor)) {
        DrwSprite2D(0x84, 0x36, 0x3D, 0x13, 0x40, 0x1A, 0x3D, 0x13, 0x80808080, D_00551CC0[1]);
    }
    blink = (s32)(((u32)(s32)(sdfSin(D_0054EE94) * 32.0f) << 24) + 0x60909090u);
    D_0054EE94 += D_0054C4C0;
    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    minX = (f32)((s32)((minX - 1000.0f) / 1000.0f) * 1000);
    minZ = (f32)((s32)((minZ + 1000.0f) / 1000.0f) * 1000);
    maxX = (f32)((s32)((maxX + 1000.0f) / 1000.0f) * 1000);
    maxZ = (f32)((s32)((maxZ - 1000.0f) / 1000.0f) * 1000);
    DrwSprite2D(0xF5, 0x44, 0x16, 0x19, 0x40, 1, 0x16, 0x19, 0x80808080, D_00551CC0[0]);
    DrwSprite2D(0xF5, 0x19B, 0x16, 0x19, 0x58, 1, 0x16, 0x19, 0x80808080, D_00551CC0[0]);
    DrwSprite2D(0xB, 0xEE, 0x1F, 0x1F, 1, 1, 0x1F, 0x1F, 0x80808080, D_00551CC0[0]);
    DrwSprite2D(0x1D6, 0xEE, 0x1F, 0x1F, 0x21, 1, 0x1F, 0x1F, 0x80808080, D_00551CC0[0]);
    if ((f32)D_00551CB0 < maxZ) {
        DrwSprite2D(0xFB, 0x45, 0xB, 0xC, 0x74, 0x38, 0xB, 0xC, blink, D_00551CC0[0]);
    }
    if (minZ < (f32)D_00551CB0) {
        DrwSprite2D(0xFB, 0x1B4, 0xB, -0xC, 0x74, 0x38, 0xB, 0xC, blink, D_00551CC0[0]);
    }
    if (minX < (f32)D_00551CAC) {
        DrwSprite2D(0x1A, 0xF7, -0xC, 0xD, 0x5E, 0x1B, 0xC, 0xD, blink, D_00551CC0[0]);
    }
    if ((f32)D_00551CAC < maxX) {
        DrwSprite2D(0x1E6, 0xF7, 0xC, 0xD, 0x5E, 0x1B, 0xC, 0xD, blink, D_00551CC0[0]);
    }
    DrwSprite2D(0xE3, 9, 0xFC, 0x51, 2, 0x2F, 0xFC, 0x51, 0x80808080, D_00551CC0[1]);
    DrwSprite2D(0x1DF, 9, 0x22, 0x51, 0xFD, 0x2F, 1, 0x51, 0x80808080, D_00551CC0[1]);
    text = D_0030A670[fldGlobalBase.area % 100];
    request = frwGetSmallFontFRQ(0, 0, 0, firstColor, text, NULL);
    font = (struct frFontHandle_t *)request;
    textX = (s32)((0x1ECu - frGetStrWidth2(font)) << 4);
    frDestroyFontRequestAfter(font);
    request = frwGetSmallFontFRQ(textX, 0x98, 0, 0xA09DC380, text, NULL);
    D_00551CA0 = (struct frFontHandle_t *)request;
    frSetPriority(D_00551CA0, 0xFFFFFF);
    frDrawFont(D_00551CA0);
    frDestroyFontRequestAfter(D_00551CA0);
    rsetOtOfs(0x51);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 2, 0, 0, 1, 2);
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    if (fldAutoMapDrawState != 0 && (u32)(D_0054EE7C - 1) < 40 && D_00567EF8[D_0054EE7C] >= 2) {
        s32 alpha = 0x80 - (D_0054EE90 < 0 ? -D_0054EE90 : D_0054EE90) * 8;
        s32 color = (alpha < 0 ? 0 : alpha) | 0x78759B00;
        text = D_0030FBD4[fldAutoMapFindFloor(fldGlobalBase.area % 100, D_0054EE7C)];
        request = frwGetSmallFontFRQ(0, 0, 0, color, text, NULL);
        font = (struct frFontHandle_t *)request;
        textX = (s32)((0x1ECu - frGetStrWidth2(font)) << 4);
        frDestroyFontRequestAfter(font);
        request = frwGetSmallFontFRQ(textX, (D_0054EE90 + 0x3C) * 8, 0, color, text, NULL);
        D_00551CA0 = (struct frFontHandle_t *)request;
        frSetPriority(D_00551CA0, 0xFFFFFF);
        frDrawFontEx(D_00551CA0, 1);
        frDestroyFontRequestAfter(D_00551CA0);
    }
    if ((u32)(D_0054EE78 - 1) < 40 && D_00567EF8[D_0054EE78] >= 2) {
        s32 alpha = 0x80 - (fldAutoMapDrawState < 0 ? -fldAutoMapDrawState : fldAutoMapDrawState) * 8;
        s32 color = (alpha < 0 ? 0 : alpha) | 0x78759B00;
        text = D_0030FBD4[fldAutoMapFindFloor(fldGlobalBase.area % 100, D_0054EE78)];
        request = frwGetSmallFontFRQ(0, 0, 0, color, text, NULL);
        font = (struct frFontHandle_t *)request;
        textX = (s32)((0x1ECu - frGetStrWidth2(font)) << 4);
        frDestroyFontRequestAfter(font);
        request = frwGetSmallFontFRQ(textX, fldAutoMapDrawState * 8 + 0x1DC, 0, color, text, NULL);
        D_00551CA0 = (struct frFontHandle_t *)request;
        frSetPriority(D_00551CA0, 0xFFFFFF);
        frDrawFontEx(D_00551CA0, 1);
        frDestroyFontRequestAfter(D_00551CA0);
    }
    SDF_VU0_LOADMATRIX(D_002FA240);
}
#else
INCLUDE_ASM(const s32, "field/fldAutoMap", fldAutoMapDrawOneArea);
#endif

void fldAutoMapDrawWindow(void) {
    SDF_VU0_LOADMATRIX(aProjMatrix);
    InversMatrix();
    sdf3DSetCamera(&dds3ScreenParam, aProjMatrix);
    rsetOtOfs(0x4A);
    rsetTest(1, 0, 0x80, 1, 0, 0, 1, 1);
    rsetAlpha(0);
    aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 0x80);
    DrwSprite2D(0x1A, 0x52, 0x1CC, 0x154, 0, 0, 0x100, 0x80, 0x80808080,
                fldAutoMapWindowTex[0]);
    D_0054EEA0 = D_0054EEA0 - 1.0f;
    fldAutoMapDrawBgOfsRF(255.0f, 250.0f, D_0054EEA0);
    sdf3DSetCamera(&dds3ScreenParam, D_002FA240);
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
    fldAmAreaData_t *rec;
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
        fldAmXYZ_t *pos;

        D_00312150[i] = sdfModelMakeEarly(0, rec->pMdl);
        pos = rec->pPos;
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

/* These are the existing native .data flag and camera/vector records. */
extern s32 fldAutoMapEnable __attribute__((section(".data")));
extern u8 dds3PadStickStatusState[2][2];
extern sdfVector4_t D_00532240;
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewTarget;
extern sdfVector4_t aViewDown;
void dds3FadeOut2(s32 frame);
void dds3FadeIn2(s32 frame);
void fldAutoMapGetBBox(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ);

void fldAutoMapCalc(void) {
    sdfVector4_t focus;
    sdfVector4_t eye;
    sdfVector4_t up = D_00532240;
    fldAmLightParam_t *light;
    f32 minX, maxZ, maxX, minZ;
    s32 closeRequested = 0;

    if (fldAutoMapChkMode() == 0) {
        return;
    }
    if (fldAutoMapEnable != 0) {
        if ((s8)dds3PadButtonState[0x22] != 0) {
            closeRequested = 1;
        }
    } else if ((s8)dds3PadButtonState[0x22] < 0) {
        closeRequested = 1;
    }

    /* A close request does not advance the input-delay counter. */
    if (closeRequested == 0) {
        fldAutoMapSeqStep++;
    }
    if (closeRequested != 0 ||
        ((s8)dds3PadButtonState[0x23] < 0 && fldAutoMapSeqState == 0)) {
        fldAutoMapSeqEnd();
        dds3FadeOut2(0);
        dds3FadeIn2(8);
        return;
    }
    if ((s8)dds3PadButtonState[0x23] < 0) {
        p2sdPlaySE(10, 127, 63);
        D_00551CAC = D_00551CB4;
        D_00551CB0 = D_00551CB8;
        fldAutoMapDrawArea = D_0054EE58;
        fldAutoMapSeqState = 0;
    }

    if (fldAutoMapSeqStep >= 11) {
        if ((s8)dds3PadStickStatusState[0][1] != 0) {
            if (dds3PadStickStatusState[0][1] & 2) {
                if ((s8)dds3PadButtonState[0x25] != 0 &&
                    (s8)dds3PadButtonState[0x27] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC += 1000;
                    D_00551CB0 -= 1000;
                } else if ((s8)dds3PadButtonState[0x25] != 0 &&
                           (s8)dds3PadButtonState[0x26] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC += 1000;
                    D_00551CB0 += 1000;
                } else if ((s8)dds3PadButtonState[0x24] != 0 &&
                           (s8)dds3PadButtonState[0x27] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC -= 1000;
                    D_00551CB0 -= 1000;
                } else if ((s8)dds3PadButtonState[0x24] != 0 &&
                           (s8)dds3PadButtonState[0x26] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC -= 1000;
                    D_00551CB0 += 1000;
                } else if ((s8)dds3PadButtonState[0x25] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC += 1000;
                } else if ((s8)dds3PadButtonState[0x24] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC -= 1000;
                } else if ((s8)dds3PadButtonState[0x27] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CB0 -= 1000;
                } else if ((s8)dds3PadButtonState[0x26] != 0) {
                    fldAutoMapSeqState = 1;
                    D_00551CB0 += 1000;
                }
            }
        } else {
            /* A held diagonal consumes the direction choice even when its
             * Z-repeat bit is absent; do not fall back to a cardinal step. */
            if ((s8)dds3PadButtonState[0x25] != 0 &&
                (s8)dds3PadButtonState[0x27] != 0) {
                if (dds3PadButtonState[0x27] & 2) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC += 1000;
                    D_00551CB0 -= 1000;
                }
            } else if ((s8)dds3PadButtonState[0x25] != 0 &&
                       (s8)dds3PadButtonState[0x26] != 0) {
                if (dds3PadButtonState[0x26] & 2) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC += 1000;
                    D_00551CB0 += 1000;
                }
            } else if ((s8)dds3PadButtonState[0x24] != 0 &&
                       (s8)dds3PadButtonState[0x27] != 0) {
                if (dds3PadButtonState[0x27] & 2) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC -= 1000;
                    D_00551CB0 -= 1000;
                }
            } else if ((s8)dds3PadButtonState[0x24] != 0 &&
                       (s8)dds3PadButtonState[0x26] != 0) {
                if (dds3PadButtonState[0x26] & 2) {
                    fldAutoMapSeqState = 1;
                    D_00551CAC -= 1000;
                    D_00551CB0 += 1000;
                }
            } else if (dds3PadButtonState[0x25] & 2) {
                fldAutoMapSeqState = 1;
                D_00551CAC += 1000;
            } else if (dds3PadButtonState[0x24] & 2) {
                fldAutoMapSeqState = 1;
                D_00551CAC -= 1000;
            } else if (dds3PadButtonState[0x27] & 2) {
                fldAutoMapSeqState = 1;
                D_00551CB0 -= 1000;
            } else if (dds3PadButtonState[0x26] & 2) {
                fldAutoMapSeqState = 1;
                D_00551CB0 += 1000;
            }
        }
    }

    fldAutoMapGetBBox(&minX, &maxZ, &maxX, &minZ);
    /* Quantize the two X endpoints, then the two Z endpoints. */
    minX = (s32)((minX - 1000.0f) / 1000.0f) * 1000;
    maxX = (s32)((maxX + 1000.0f) / 1000.0f) * 1000;
    maxZ = (s32)((maxZ - 1000.0f) / 1000.0f) * 1000;
    minZ = (s32)((minZ + 1000.0f) / 1000.0f) * 1000;

    if ((f32)D_00551CAC < minX) {
        D_00551CAC = (s32)minX;
    }
    if (maxX < (f32)D_00551CAC) {
        D_00551CAC = (s32)maxX;
    }
    if ((f32)D_00551CB0 < minZ) {
        D_00551CB0 = (s32)minZ;
    }
    if (maxZ < (f32)D_00551CB0) {
        D_00551CB0 = (s32)maxZ;
    }

    focus.x = D_00551CAC;
    focus.y = 0.0f;
    focus.z = D_00551CB0;
    eye.x = D_00551CAC;
    eye.y = -24000.0f;
    eye.z = D_00551CB0;
    if (dds3PadButtonState[0x2B] != 0) {
        eye.y = -60000.0f;
    }
    SDF_VU0_COPYVECTOR(&D_002F5E00, &eye);
    SDF_VU0_COPYVECTOR(&fldAutoMapRect, &focus);
    SDF_VU0_COPYVECTOR(&D_002F5E10, &up);
    light = (fldAmLightParam_t *)&dds3LightParam2[0x20];
    light->near = 255.0f;
    light->scale = 1000.0f;
    light->far = 255.0f;
    light->bias = 20000.0f;
    light->mode = 0x808080;
    SDF_VU0_COPYVECTOR(&aViewEye, &eye);
    SDF_VU0_COPYVECTOR(&aViewTarget, &focus);
    SDF_VU0_COPYVECTOR(&aViewDown, &up);
}


void fldAutoMapSetField(s32 fld, s32 area, s32 block) {
    f32 x = fldGlobalBase.x;
    f32 z = fldGlobalBase.z;

    fldGlobalBase.blockID = block;
    fldAutoMapFieldId.z = z;
    fldAutoMapFieldId.x = x;
    fldGlobalBase.area = fld;
    fldGlobalBase.floor = area;
    fldAutoMapAreaNo = area;
    fldAutoMapFieldId.y = 0.0f;
    fldAutoMapField = fld % 100;
}

s32 fldAutoMapChkOpen(s32 fld, s32 area, u32 block) {
    s32 floor = fld % 100;

    if (floor < 10) {
        return 0;
    }
    floor -= 10;
    if (floor >= 30) {
        return (*(u64 *)((u8 *)DDS3_GBWK + 0x17F10 + (floor * 32 + area) * 8) >> block) & 1;
    }
    return (*(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (floor * 32 + area) * 8) >> block) & 1;
}

void fldAutoMapOpenBlock(s32 blockID) {
    s32 floor;

    if (blockID <= 0) {
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
    fldGlobalBase.blockID = blockID - 1;
    if (floor >= 30) {
        u32 ofs = 0x17F10 + (floor * 32 + fldGlobalBase.floor) * 8;

        *(u64 *)((u8 *)DDS3_GBWK + ofs) |= (u64)1 << (blockID - 1);
    } else {
        *(u64 *)((u8 *)DDS3_GBWK + 0xFC58 + (floor * 32 + fldGlobalBase.floor) * 8) |=
            (u64)1 << (blockID - 1);
    }
    fldGlobalBase.amSBlock = blockID;
    fldGlobalBase.amFloor = fldAutoMap_GetFloor(fldGlobalBase.floor, fldGlobalBase.blockID);
    if (fldAutoMapField == 25) {
        if (fldGlobalBase.floor == 0) {
            fldAutoMapOpenBlock2(fldAutoMapField, 17, blockID);
        }
        if (fldGlobalBase.floor == 16) {
            fldAutoMapOpenBlock2(fldAutoMapField, 1, blockID);
        }
        if (fldGlobalBase.floor == 1) {
            fldAutoMapOpenBlock2(fldAutoMapField, 18, blockID);
        }
        if (fldGlobalBase.floor == 17) {
            fldAutoMapOpenBlock2(fldAutoMapField, 2, blockID);
        }
        if (fldGlobalBase.floor == 2) {
            fldAutoMapOpenBlock2(fldAutoMapField, 19, blockID);
        }
        if (fldGlobalBase.floor == 18) {
            fldAutoMapOpenBlock2(fldAutoMapField, 3, blockID);
        }
        if (fldGlobalBase.floor == 3) {
            fldAutoMapOpenBlock2(fldAutoMapField, 20, blockID);
        }
        if (fldGlobalBase.floor == 19) {
            fldAutoMapOpenBlock2(fldAutoMapField, 4, blockID);
        }
        if (fldGlobalBase.floor == 4) {
            fldAutoMapOpenBlock2(fldAutoMapField, 21, blockID);
        }
        if (fldGlobalBase.floor == 20) {
            fldAutoMapOpenBlock2(fldAutoMapField, 5, blockID);
        }
    }
    if (fldAutoMapField == 34) {
        if (fldGlobalBase.floor == 1) {
            fldAutoMapOpenBlock2(fldAutoMapField, 3, blockID);
        }
        if (fldGlobalBase.floor == 2) {
            fldAutoMapOpenBlock2(fldAutoMapField, 2, blockID);
        }
    }
}

void fldAutoMapOpenBlock2(s32 fld, s32 area, s32 SBlock) {
    s32 room = fld % 100;
    s32 index = area - 1;
    s32 bit = SBlock - 1;

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
void fldAutoMapCloseBlock2(s32 fld, s32 area, s32 SBlock) {
    s32 bit = SBlock - 1;
    s32 room = fld % 100;
    s32 index = area - 1;

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

void fldAutoMapNameU(s32 nameID) {
    if (nameID >= 0x40) {
        fldAreaNameUpper[0] = -1;
    } else {
        fldAreaNameUpper[0] = nameID;
    }
}

void fldAutoMapNameD(s32 nameID) {
    if (nameID >= 0x40) {
        fldAreaNameLower[0] = -1;
    } else {
        fldAreaNameLower[0] = nameID;
    }
}

s32 fldAutoMapGetFloorPrev(s32 floor) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    fldAmAreaData_t *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->nSB; j++) {
            if (fldAutoMapChkOpen(fldAutoMapField, i, j) != 0) {
                item = fldAutoMap_GetFloor(i, j);
                if (item < floor && best < item) {
                    best = item;
                }
            }
        }
    }
    if (best == -1) {
        return floor;
    }
    return best;
}

s32 fldAutoMapGetFloorNext(s32 floor) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = 999;
    fldAmAreaData_t *entry = fldAutoMapHeader;

    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        for (j = 0; j < entry->nSB; j++) {
            if (fldAutoMapChkOpen(fldAutoMapField, i, j) != 0) {
                item = fldAutoMap_GetFloor(i, j);
                if (floor < item && item < best) {
                    best = item;
                }
            }
        }
    }
    if (best == 999) {
        return floor;
    }
    return best;
}

void fldAllIconDel(s32 index, f32 *x, f32 *z) {
    s32 i;
    fldAmAreaData_t *entry = fldAutoMapHeader;
    for (i = 0; i < (s32)fldAutoMapData; i++, entry++) {
        if (i == index) {
            fldAmXYZ_t *position = entry->pPos;
            *x = position->x;
            *z = position->z;
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

void fldAutoMapGetBBox(f32 *getX0, f32 *getZ0, f32 *getX1, f32 *getZ1) {
    s32 room;
    u32 i;
    fldAmAreaData_t *rec;
    fldAmSBlock_t *item;
    fldAmXYZ_t *a;
    s32 visible;
    fldAmXYZ_t *b;

    room = 0;
    *getX0 = 0.0f;
    *getZ0 = 0.0f;
    *getX1 = 0.0f;
    *getZ1 = 0.0f;
    rec = fldAutoMapHeader;
    for (; room < fldAutoMapData; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->nSB; i++, item++) {
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
                    if (*getX0 > a->x) {
                        *getX0 = a->x;
                    }
                    b = item->pointB;
                    if (*getX0 > b->x) {
                        *getX0 = b->x;
                    }
                    if (*getX1 < a->x) {
                        *getX1 = a->x;
                    }
                    if (*getX1 < b->x) {
                        *getX1 = b->x;
                    }
                    if (*getZ0 < a->z) {
                        *getZ0 = a->z;
                    }
                    if (*getZ0 < b->z) {
                        *getZ0 = b->z;
                    }
                    if (a->z < *getZ1) {
                        *getZ1 = a->z;
                    }
                    if (b->z < *getZ1) {
                        *getZ1 = b->z;
                    }
                }
            }
        }
    }
}

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
extern void frPreCalcStrWidth(const char *pStr, s32 Style, s32 Mode);
struct frFontHandle_t;
struct frFontRequest_t;
extern struct frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 priority, s32 color,
                                                  const char *text, struct frFontRequest_t *previous);
extern u32 frGetStrWidth2(struct frFontHandle_t *font);
extern s32 frDestroyFontRequestAfter(struct frFontHandle_t *font);
extern void fldDrawAddressPanel(s32 width, s32 x, s32 y);
extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji);

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
    fileRequest_t *request;
    sdfPacDecodeResrc_t *work;
    sdfMemHandle_t *handle;
    u32 i;

    D_0054EF1C = NULL;
    D_0054EF20 = NULL;
    D_0054EF24 = NULL;
    fldTakara1File = sdfDevLoadFileEx("/fld/f/bin/takara1.PB", (u32 *)&fldTakara1Data, &fldTakara1Size);
    fldTakara3File = sdfDevLoadFileEx("/fld/f/bin/takara3.PB", (u32 *)&fldTakara3Data, &fldTakara3Size);
    fldTakara2File = sdfDevLoadFileEx("/fld/f/bin/takara2.PB", (u32 *)&fldTakara2Data, &fldTakara2Size);
    request = pacLoad("/fld/f/bin/fldmix.LB");
    pacSync(request);
    work = request->pac.resrcHead;
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

        fldModelSlotTable[i].type = 0;
        fldModelSlotTable[i].x = 0;
        fldModelSlotTable[i].y = 0;
        fldModelSlotTable[i].z = 0;
        fldModelSlotTable[i].object = NULL;
        fldModelSlotTable[i].cntf = 0;
        fldModelSlotTable[i].cnti = 0;
        fldModelSlotTable[i].cntAnmf = 0;
        fldModelSlotTable[i].cntAnmi = 0;
        fldModelSlotTable[i].unk44 = 0;
        fldModelSlotTable[i].room = 0;
        fldModelSlotTable[i].itemBoxIdx = 0;
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
        fldTakaraSlotTable[i].object = 0;
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

