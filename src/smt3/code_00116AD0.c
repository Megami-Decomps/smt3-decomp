#include "common.h"

#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"

typedef struct {
    u16 modelFlag;
    u8 unk02[0x1A];
    u16 hojiFlag;
} fldGbFlags_t;

typedef struct {
    fldGbFlags_t flags[32];
} fldGbTypeBlock_t;

extern sdfVector4_t fldCamTarget;
extern s32 fldGb __attribute__((section(".data")));
extern s32 dds3EffectFldTypeIsSpecial(s32 charId);
extern s32 dds3EffectFldCharToType(s32 charId);


/* ps2asm.h's EE_SQRT_F32 is volatile, which pins the surrounding schedule.
   This form lets cc1 hoist the compare constants above the sqrt. */
#define EE_SQRT_F32_SCHED(out, in) __asm__ ( \
    ".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder" : "=f" (out) : "f" (in))

extern f32 sdfAtan2(f32 x, f32 z);
extern dds3Basic_t *dds3GetControlScene(void);
extern void dds3GetPosPoseSet(dds3Basic_t *basic);
extern void dds3GetRotPoseSet(dds3Basic_t *basic);
extern void fldRunDanteShutdown(void);


typedef struct {
    s32 active;
    s32 count;
    u8 unk08[0x8];
    dds3Basic_t *handle;
} fldWave_t;

typedef struct {
    u8 x;
    u8 y;
    u8 z;
    u8 w;
    u8 unk04[0xC];
} fldMapCell_t;

typedef struct {
    u8 unk00[0x4];
    u32 cellCount;
} fldRunMapHeader_t;

extern fldWave_t fldWaveBuff[];
extern void *mhfldWaveFile;
extern void *mhfldPlayerFile;
extern void *mhfldAnaFile;
extern s32 gFldRndEncBtl;

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);
extern s32 fldLoadNextGetStatus2(void);
extern s32 fldLoadNextGetStatus(void);
extern void fldLoadNextTask(void);
extern void fldFreeNextFile(void);
extern s32 fldBgmIsLoading(void);
extern void fldWaitBgmLoad(void);
extern s32 sdfDelayedResrcCheck(void);
extern void fldPlayerAFree(void);
extern void fldLoadNextFile_SetId_BtlRet(s32 id, s32 arg1);


typedef struct {
    s16 fld;
    s16 area;
    u8 unk04[0x18];
} fldNameEntry_t;

extern fldNameEntry_t fldAreaNameTable[];
extern fldNameEntry_t fldPlaceNameTable[];
extern fldNameEntry_t fldFloorNameTable[];

extern s32 fldNext_fld;
extern s32 D_0054EB7C;
extern s32 gFldAnahoriAkumaIdx;
extern s32 gFldAnahoriAkumaIdxMax;
extern s16 fldRun_MAP_OX;
extern s16 fldRun_MAP_OZ;
extern s16 fldRun_MAP_BX;
extern s16 fldRun_MAP_BZ;
extern s32 fldRunAiTimer __attribute__((section(".bss")));

extern s32 dds3AdminiGetNextSequence(void);
extern s32 dds3AdminiGetPrevHistoryFlag(void);
extern void fldDoor_EveBtlExit(s32 fld, s32 arg1);
extern s32 evtBitCheck(s32 bit);
extern void fldSetElvCameraScene(s32 dir);


typedef struct {
    u8 unk00[0x4];
    u32 flag;
    u8 unk08[0x2];
    u16 hp;
    u8 unk0C[0xE0];
} fldAnahoriAkuma_t;

/* Only the fields reached from this unit are known. */
typedef struct {
    u8 unk00000[0xA4C];
    u32 maFlags;
    fldAnahoriAkuma_t akuma[16];
    u8 unk01910[0x4D48];
    fldGbTypeBlock_t gb[48];
    u8 takaraOpen[0x20];
    u8 unk11A78[0x8];
    u8 takaraOpenEx[0x20];
} dds3GlobalWork_t;

typedef struct {
    u8 unk00[0x4];
    s32 fld;
    s32 area;
    u8 unk0C[0x8];
    u32 flag;
    u8 unk18[0xA0];
    s32 lightMaIdx;
    u8 unkBC[0x44];
    f32 posX;
    u8 unk104[0x4];
    f32 posZ;
} fldGlobalWork_t;

typedef struct {
    u8 unk00[0x34];
    s32 mode;
    u8 unk38[0x24];
    s32 restore;
} fldSceneParam_t;

extern dds3GlobalWork_t *DDS3_GBWK;
extern fldGlobalWork_t fldGlobalBase;
extern fldSceneParam_t fldSceneParam;
extern u8 D_002FC480[];
extern s32 gFldNowDZone;
extern s32 fldProcCampWait;
extern s32 fldEncountCheckCount __attribute__((section(".data")));
extern s32 fldJDataHandle __attribute__((section(".data")));

extern void fldEsutoMaClear(void);
extern void fldRiberaMaClear(void);
extern void fldRifutoMaClear(void);
extern void fldLightMaClear(void);
extern s32 fldLightMaIdxCalc(void);
extern void fldSePlay(s32 no);
extern void fldSkySetTblLightMa(s32 idx, s32 flag);
extern void fldDrawInit(void);
extern void fldInitAllValue(void);
extern void fldSe_Init(void);
extern void sdfRandomize(void *state, u32 seed);
extern void fldInitPacLoader(void);
extern void fldSkyDefaultLoad(void);
extern void fldAutoMapLoad(void);
extern void fldAllTblLoad(void);
extern s32 campCheckExitState(void);
extern void dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, s32 flag);
extern void fldRunTamaFree(void);
extern void fldEncountClearFlag(u32 flag);
extern void p2sdFadeOutBgm(u32 arg);
extern void p2sdStopBGM(u32 arg);

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct fldResourcePoint_t fldResourcePoint_t;
typedef struct scrProcessWork_s scrProcessWork_s;

/* The font node frFont hands back. It accumulates the drawn string's width at
 * +0xC, which the centring path in dds3FontReqAddString reads. */
typedef struct
{
    u8 unk00[0xC];
    s32 width;
} _FRQ;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

typedef struct
{
    dds3FontReq_t req;
    _FRQ *frq;
} dds3FontReqString_t;

typedef struct
{
    dds3FontReq_t req;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 z;
    u32 color;
} dds3FontReqBox_t;

typedef struct
{
    dds3FontReq_t *first;
    dds3FontReq_t *last;
} dds3FontReqList_t;

typedef struct
{
    dds3Basic_t *handle;
    dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

typedef struct
{
    sdfVector4_t wideCameraOffset;
    sdfVector4_t wideTargetOffset;
    sdfVector4_t narrowCameraOffset;
    sdfVector4_t narrowTargetOffset;
    sdfVector4_t q0;
    sdfVector4_t q1;
    f32 rotFrame;
    f32 rotFrameMax;
    f32 t0;
    f32 t1;
    f32 lenFrame;
    f32 lenFrameMax;
    dds3Basic_t *handle;
} fldFollowCameraControl_t;

/* Dante's chase work block. Only the fields the chase code reaches are known. */
typedef struct
{
    s32 unk00;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
    u8 unk14[0x8];
    u16 mapIx;
    u16 mapIz;
    u8 unk20[0x18];
    s32 waitCount;
    f32 angleToPlayer;
    u8 unk40[0xC];
    u32 flag;
    u8 unk50[0x4];
    s32 unk54;
    u8 unk58[0x14];
    u16 mapIxOld;
    u16 mapIzOld;
    s32 unk70;
} fldRunDante_t;

extern dds3FontReqList_t D_00551C80;
extern dds3FontReqFunc_t dds3FontReqTable;
extern dds3FontReqFunc_t D_0054EA30;

extern sdfGrOt_t sdfGrOtTable[];
extern u8 dds3PadButtonState[];

extern char D_00530F30[];
extern char D_00531860[];
extern char D_005318E0[];

extern fldUnitControl_t fldUnitControl;
extern fldFollowCameraControl_t fldFollowCameraControl;
extern fldRunDante_t fldRun_Dante;
extern fldUnitControl_t *fldGetUnitControl(void);
extern fldFollowCameraControl_t *fldGetFollowCameraControl(void);

extern u32 fldAutoMapSeqFlags[1];
extern u32 fldEncountFlags;
extern s32 fldRunDanteHandle __attribute__((section(".bss")));
extern s32 gFldAnahoriAkumaIdx;
extern s32 gFldAnahoriAkumaIdxMax;
extern s32 fldProcAmWait;
extern s32 fldNext_fld;
extern s32 D_0054EB7C;
extern s32 fldNextFieldId;
extern s32 fldElvState;
extern s32 fldElvFloorTarget;
extern s32 gFldElvFloorNow;
extern s32 fldElvFloorFrom;
extern s32 fldElvFloorNum;
extern s32 fldElvFloorTo;
extern s32 fldElvFloorDisp;
extern sdfMemHandle_t *fldPlayerAnimData;
extern sdfMemHandle_t *fldRunMapData;
extern fldRunMapHeader_t *fldRunMapHeader;
extern fldMapCell_t *fldRunMapCellData;
extern void *pFroMapHead;
extern f32 fldRunMapScale;
extern s32 D_002FC3D8 __attribute__((section(".data")));

extern dds3Basic_t *dds3AddSceneBasicData(u32 type);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);
extern void *sdfChipAllocZeroDirect(s32 size);
extern void sdfChipFreeDirect(void *ptr);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *handle);
extern void *sdfGrAllocPacketBuff(s32 size);
extern s32 sdfDrawTagInit(sdfDrawTag_t *tag);
extern sdfDrawTag_t *sdfPrimAllocDrawTag(void *allocator);
extern void sdfPrimLinkBoxLine(sdfDrawTag_t *tag, u32 color, u32 prim, s32 x0, s32 y0, s32 x1,
                               s32 y1, u32 z, void *allocator);
extern s32 frDrawFont(_FRQ *frq);
extern void frSetFontColorNo(_FRQ *frq, u8 colorno);
extern _FRQ *frSetFontString(char *str, s32 style, s32 colorno, s32 tp, _FRQ *frq, s32 mode);
extern void frSetDrawPosition(_FRQ *frq, s32 x, s32 y);
extern _FRQ *frDestroyFontRequestAfter(_FRQ *frq);
extern scrProcessWork_s *scrSearchScriptName(char *scriptname);
extern s32 fldAutoMapChkMode(void);
extern void fldClearElvCameraScene(void);
extern void fldSetTerminalState(s32 type, s32 idx);
extern void fldPauseBgm(s32 fld, s32 area, void *name);
extern void fldProcNextJump(void);
extern s32 fldDoor_SiseExit(void);
extern void fldEveHit_End(void);
extern void fldShutoff(void);
extern void evtFreeStage(void);
extern void fldProcShutDown(void);
extern void fldRunDanteGetDistAngle(f32 *dist, f32 *angle);
extern void dds3DebugFloatEditEx(f32 *value, u8 *pad, f32 step, f32 min, f32 max, f32 scale);
extern void dds3DebugIntEditEx(u32 *value, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 *pad);
extern void dds3DebugColorEditEx(u32 *value, s32 digits, u8 *pad);
extern s32 ooyTestDrawProc(dds3ProcessID_t *id);
extern void fldEsutoMaClear(void);

dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *pData, char *pName)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_POINT);

    basic->workdata = pData;
    basic->serialID = serialID;
    basic->name = pName;
    return basic;
}

fldResourcePoint_t *dds3PointBasicGetResource(dds3Basic_t *basic)
{
    return basic->workdata;
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3GlobalWorkInit);

void dds3GlobalWorkUserInit(void) {
}

INCLUDE_ASM(const s32, "code_00116AD0", NewGame2Push);

INCLUDE_ASM(const s32, "code_00116AD0", NewGame2Pop);

INCLUDE_ASM(const s32, "code_00116AD0", dds3GlobalWorkUserInit2);

void dds3FontReqListInsert(dds3FontReqList_t *list, void *node, s32 linkofs)
{
    dds3FontReq_t *link = (dds3FontReq_t *)((u8 *)node + linkofs);
    dds3FontReq_t *last = list->last;

    if (last == NULL) {
        list->first = node;
    } else {
        ((dds3FontReq_t *)((u8 *)last + linkofs))->next = node;
    }
    link->prev = last;
    link->next = NULL;
    list->last = node;
}

void dds3FontReqListRemove(dds3FontReqList_t *list, void *node, s32 linkofs)
{
    dds3FontReq_t *link = (dds3FontReq_t *)((u8 *)node + linkofs);
    dds3FontReq_t *prev = link->prev;
    dds3FontReq_t *next = link->next;

    if (prev == NULL) {
        list->first = next;
    } else {
        ((dds3FontReq_t *)((u8 *)prev + linkofs))->next = next;
    }
    if (next == NULL) {
        list->last = prev;
    } else {
        ((dds3FontReq_t *)((u8 *)next + linkofs))->prev = prev;
    }
}

void dds3FontReqListAdd(dds3FontReq_t *req, dds3FontReqFunc_t *func)
{
    dds3FontReqListInsert(&D_00551C80, req, 0);
    req->func = func;
}

void dds3FontReqListRunOne(dds3FontReq_t *req)
{
    dds3FontReqListRemove(&D_00551C80, req, 0);
    req->func->destroy(req);
}

void dds3FontReqListRunAll(void)
{
    dds3FontReq_t *req;

    while ((req = D_00551C80.first) != NULL) {
        dds3FontReqListRunOne(req);
    }
}

void func_00117830(dds3FontReq_t *req, s32 group)
{
    req->group = group;
}

void dds3FontReqListRunTo(s32 group)
{
    dds3FontReq_t *req;
    dds3FontReq_t *next;

    for (req = D_00551C80.first; req != NULL; req = next) {
        next = req->next;
        if (req->group == group) {
            dds3FontReqListRunOne(req);
        }
    }
}

void dds3FontReqListCount(void)
{
    dds3FontReq_t *req;

    for (req = D_00551C80.first; req != NULL; req = req->next) {
        req->func->draw(req);
    }
}

void dds3FontReqDestroy(dds3FontReqString_t *req)
{
    frDestroyFontRequestAfter(req->frq);
    sdfChipFreeDirect(req);
}

s32 dds3FontReqDrawFont(dds3FontReqString_t *req)
{
    return frDrawFont(req->frq);
}

dds3FontReq_t *dds3FontReqAddString(s32 x, s32 y, char *str)
{
    dds3FontReqString_t *req;
    _FRQ *frq;

    frq = frSetFontString(str, 0, 0, 0, NULL, 0);
    if (x == 0x800000) {
        x = (0x200 - frq->width) * 8;
    }
    frSetDrawPosition(frq, x, y);
    req = sdfChipAllocZeroDirect(sizeof(dds3FontReqString_t));
    req->frq = frq;
    dds3FontReqListAdd(&req->req, &dds3FontReqTable);
    return &req->req;
}

void dds3FontReqSetColorNo(dds3FontReqString_t *req, u8 colorno)
{
    frSetFontColorNo(req->frq, colorno);
}

void dds3FontReqFree(dds3FontReqBox_t *req)
{
    sdfChipFreeDirect(req);
}

void dds3FontReqDrawBoxLine(dds3FontReqBox_t *req)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    tag = sdfPrimAllocDrawTag(NULL);
    sdfPrimLinkBoxLine(tag, req->color, 0, req->x, req->y, req->x + req->w, req->y + req->h,
                       req->z, NULL);
    ot = &sdfGrOtTable[71];
    ((sdfGrOtRegFunc_t)ot->regFunc)(ot, tag);
}

dds3FontReq_t *dds3FontReqAddBox(s32 x, s32 y, u32 z, s32 w, s32 h, u32 color)
{
    dds3FontReqBox_t *req = sdfChipAllocZeroDirect(sizeof(dds3FontReqBox_t));

    req->x = x;
    req->y = y;
    req->w = w;
    req->h = h;
    req->z = z;
    req->color = color;
    dds3FontReqListAdd(&req->req, &D_0054EA30);
    return &req->req;
}

void func_00117AF8(dds3FontReqBox_t *req, s32 x, s32 y)
{
    req->x = x;
    req->y = y;
}

sdfDrawTag_t *dds3FontReqAllocDrawTag(void)
{
    sdfDrawTag_t *tag = sdfGrAllocPacketBuff(0x20);

    sdfDrawTagInit(tag);
    return tag;
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimFrameRect);

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimLine);

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimLine3D);

INCLUDE_ASM(const s32, "code_00116AD0", dds3DebugFloatEditEx);

void dds3DebugFloatEdit(f32 *value, f32 step, f32 min, f32 max, f32 scale)
{
    dds3DebugFloatEditEx(value, &dds3PadButtonState[0x20], step, min, max, scale);
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3DebugIntEditEx);

void dds3DebugIntEdit(u32 *value, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    dds3DebugIntEditEx(value, a1, a2, a3, a4, a5, &dds3PadButtonState[0x20]);
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3DebugColorEditEx);

void dds3DebugColorEdit(u32 *value, s32 digits)
{
    dds3DebugColorEditEx(value, digits, &dds3PadButtonState[0x20]);
}

void dds3MenuStepValue(char *out, f32 value)
{
    s32 i;
    s32 f1;
    s32 f2;

    value += 0.005f;
    i = (s32)value;
    if (i >= 10) {
        f1 = 9;
        f2 = 9;
    } else {
        value -= (f32)i;
        value *= 10.0f;
        f1 = (s32)value;
        value -= (f32)f1;
        f2 = (s32)(value * 10.0f);
    }
    out[0] = i + '0';
    out[1] = '.';
    out[2] = f1 + '0';
    out[3] = f2 + '0';
    out[4] = '\0';
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimDebugTextBox);

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimDebugTextBoxRGB);

s32 dds3NullDrawProc(dds3ProcessID_t *id)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3DrawUnitObjectShadow);

INCLUDE_ASM(const s32, "code_00116AD0", ooyTestDrawProc);

void InitOoyTest(void)
{
    dds3StartProcess(D_00530F30, DDS3_USERDRAW_PRIORITY, 0, 0, ooyTestDrawProc, NULL, NULL);
}

void KillOoyTest(void)
{
    dds3KillProcessName(D_00530F30, 1);
}

INCLUDE_ASM(const s32, "code_00116AD0", fldElvGetFloorDigits);

INCLUDE_ASM(const s32, "code_00116AD0", fldElvSetFloor);

void fldElvReset(void)
{
    fldElvState = 0;
    gFldElvFloorNow = -999;
    fldElvFloorFrom = -999;
    fldElvFloorNum = -999;
    fldElvFloorTo = -999;
    fldElvFloorDisp = -999;
    fldElvFloorTarget = -999;
    fldClearElvCameraScene();
}

s32 fldElvCheckEnd(void)
{
    if (fldElvState == 1) {
        return 2;
    }
    if (fldElvFloorTarget != -999 && gFldElvFloorNow == fldElvFloorTo) {
        if (fldElvFloorFrom < gFldElvFloorNow) {
            fldSetElvCameraScene(2);
        } else {
            fldSetElvCameraScene(-2);
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00530F30);

INCLUDE_ASM(const s32, "code_00116AD0", fldElvDrawFloorNum);

INCLUDE_ASM(const s32, "code_00116AD0", drawSceneBasicList);

INCLUDE_ASM(const s32, "code_00116AD0", fldSdwDraw);

void fldRunDanteClear(void)
{
    fldRunDanteHandle = -1;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531040);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaInit);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaFree);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunEffSet);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunEffTask);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaSet);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaSe);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531148);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531158);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531180);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005311A8);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005311D0);

INCLUDE_ASM(const s32, "code_00116AD0", fldMakeFroFileName);

void fldFreeRunMapData(void)
{
    if (fldRunMapData != NULL) {
        sdfMemFreeDelayedDirect(fldRunMapData);
        fldRunMapData = NULL;
        fldRunMapHeader = NULL;
        fldRunMapCellData = NULL;
        pFroMapHead = NULL;
        fldRunMapScale = 0.0f;
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", FroFtoI);

void fldMapIndexToPos(s16 ix, s16 iz, f32 *px, f32 *pz)
{
    f32 ox = fldRun_MAP_OX;
    f32 oz = fldRun_MAP_OZ;

    *px = ox + (f32)fldRun_MAP_BX * (f32)ix;
    *pz = oz + (f32)-fldRun_MAP_BZ * (f32)iz;
}

fldMapCell_t *fldFindMapCell(s16 x, s16 y, s16 z, s16 w)
{
    fldMapCell_t *p = fldRunMapCellData;
    u32 i;

    for (i = 0; i < fldRunMapHeader->cellCount; i++, p++) {
        if (p->x == (u8)x && p->y == (u8)y && p->z == (u8)z && p->w == (u8)w) {
            return p;
        }
    }
    return NULL;
}

void fldRunDanteGetDistAngle(f32 *pDist, f32 *pAngle)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 angle;

    dx = fldGlobalBase.posX - fldRun_Dante.x;
    dz = fldGlobalBase.posZ - fldRun_Dante.z;
    EE_SQRT_F32_SCHED(dist, dx * dx + dz * dz);
    if (dist < 1.0f) {
        angle = 0.0f;
    } else {
        angle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *pDist = dist;
    *pAngle = angle;
}

void fldGetDistAngleTo(f32 *pDist, f32 *pAngle, f32 x, f32 z)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 angle;

    dx = x - fldRun_Dante.x;
    dz = z - fldRun_Dante.z;
    EE_SQRT_F32_SCHED(dist, dx * dx + dz * dz);
    if (dist < 1.0f) {
        angle = 0.0f;
    } else {
        angle = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *pDist = dist;
    *pAngle = angle;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldTurnToward);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunAiCng);

void func_0011C790(void) {
}

void func_0011C798(void)
{
    if (fldRun_Dante.waitCount > 0) {
        fldRun_Dante.waitCount--;
    }
}

void func_0011C7B8(void)
{
    if (fldRun_Dante.waitCount > 0) {
        fldRun_Dante.waitCount--;
    }
    if (fldRun_Dante.unk54 != 0) {
        fldRun_Dante.unk54 = 0;
        fldRun_Dante.flag |= 0x4;
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteDoin);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteMove);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDantePwak);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteSoba);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDantePatr);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteGun1);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteGun2);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteCollision);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531318);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531328);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531338);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531350);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteAi);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteLoadBgm);

void fldRunDanteSwapBgm(s32 toDante)
{
    if (toDante == 0) {
        p2sdFadeOutBgm(0x6B0001);
        p2sdStopBGM(0x820000);
    } else {
        p2sdFadeOutBgm(0x820000);
        p2sdStopBGM(0x6B0001);
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteInit);

void fldRunDanteShutdown(void)
{
    fldFreeRunMapData();
    fldRunTamaFree();
    if (fldJDataHandle != 0) {
        fldRun_Dante.unk70 = fldRun_Dante.unk00;
    } else {
        fldRunDanteHandle = -1;
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteDrawShadow);

void fldRunDanteFacePlayer(void)
{
    f32 dist;
    f32 angle;

    fldRunDanteGetDistAngle(&dist, &angle);
    fldRun_Dante.angleToPlayer = angle;
    fldRun_Dante.turn = 180.0f - angle;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteSetModel);

s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 v;

    dx = fldCamTarget.x - x;
    dy = fldCamTarget.y - y;
    dz = fldCamTarget.z - z;
    EE_SQRT_F32_SCHED(dist, dx * dx + dy * dy + dz * dz);
    dist -= 500.0f;
    if (dist > 5400.0f) {
        dist = 5400.0f;
    }
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    v = ((5400 - (s32)dist) * 127) / 5400;
    if (v > 127) {
        v = 127;
    }
    if (v < 0) {
        v = 0;
    }
    return v;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteSe);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunView);

s32 fldRunEncountChk(void)
{
    f32 dist;
    f32 angle;

    if (evtBitCheck(0xAF3) == 0) {
        return 0;
    }
    if (fldRunAiTimer & 1) {
        return 0;
    }
    fldRunDanteGetDistAngle(&dist, &angle);
    if (dist < 250.0f) {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_005313E0);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteStanceSet);

INCLUDE_ASM(const s32, "code_00116AD0", fldAllTblLoad);

INCLUDE_ASM(const s32, "code_00116AD0", fldGbFlagClear);

INCLUDE_ASM(const s32, "code_00116AD0", fldGbInit_System);

INCLUDE_ASM(const s32, "code_00116AD0", fldResetSequenceParams);

INCLUDE_ASM(const s32, "code_00116AD0", dds3EffectFldCharToType);

INCLUDE_ASM(const s32, "code_00116AD0", dds3EffectFldTypeIsSpecial);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetModelFlag);

s32 fldGetModelFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag >> bit) & 1;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldSetAnimeFlag);

INCLUDE_ASM(const s32, "code_00116AD0", dds3ObjectBaseAddModelOt);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetAtariFlag);

INCLUDE_ASM(const s32, "code_00116AD0", fldGbChkHit);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetModelFlag2);

INCLUDE_ASM(const s32, "code_00116AD0", dds3EffectObjectAddFld);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetAnimeFlag2);

INCLUDE_ASM(const s32, "code_00116AD0", fld_120C30);

INCLUDE_ASM(const s32, "code_00116AD0", fldGetAnimeId2);

INCLUDE_ASM(const s32, "code_00116AD0", dds3EffectObjectFindFld);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetNpcFlag);

INCLUDE_ASM(const s32, "code_00116AD0", fldGbChkNpc);

INCLUDE_ASM(const s32, "code_00116AD0", fldGbSetGimic);

s32 fldGetHojiFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].hojiFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].hojiFlag >> bit) & 1;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldSetTakaraOpen);

INCLUDE_ASM(const s32, "code_00116AD0", fldGetTakaraOpen);

s32 fldAutoMapFindArea(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0x190; i++) {
        if (fld == fldAreaNameTable[i].fld && area == fldAreaNameTable[i].area) {
            return i;
        }
    }
    return 0;
}

s32 fldAutoMapFindPlace(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0x150; i++) {
        if (fld == fldPlaceNameTable[i].fld && area == fldPlaceNameTable[i].area) {
            return i;
        }
    }
    return 0;
}

s32 fldAutoMapFindFloor(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0xE0; i++) {
        if (fld == fldFloorNameTable[i].fld && area == fldFloorNameTable[i].area) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldDbgWmapStart);

INCLUDE_ASM(const s32, "code_00116AD0", fldMainLoadStart);

void fldFirstInit(void)
{
    fldDrawInit();
    fldInitAllValue();
    fldSe_Init();
    sdfRandomize(D_002FC480, 0x1E240);
    fldInitPacLoader();
    fldSkyDefaultLoad();
    fldAutoMapLoad();
    fldAllTblLoad();
}

INCLUDE_ASM(const s32, "code_00116AD0", fldInitAllValue);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetSceneParam2);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetSceneParam3);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetSceneParam4);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetSceneParam5);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetSceneParamLoad);

s32 fldGetRestoreMode(void)
{
    if (fldSceneParam.mode == 1) {
        return 1;
    }
    if (fldSceneParam.restore == 3) {
        return 3;
    }
    if (fldSceneParam.restore == 4) {
        return 4;
    }
    if (fldSceneParam.restore == 5) {
        return 5;
    }
    if (fldSceneParam.restore == 6) {
        return 2;
    }
    return fldSceneParam.mode != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldReadyScene);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531690);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005316A0);

INCLUDE_ASM(const s32, "code_00116AD0", fldPlayerALoad);

void fldPlayerAFree(void)
{
    if (fldPlayerAnimData != NULL) {
        sdfMemFreeDirect(fldPlayerAnimData);
        fldPlayerAnimData = NULL;
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldJDataLoad);

void fldJDataClear(void)
{
    if (mhfldPlayerFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldPlayerFile);
        mhfldPlayerFile = NULL;
    }
    if (mhfldAnaFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldAnaFile);
        mhfldAnaFile = NULL;
    }
    if (mhfldWaveFile != NULL) {
        sdfMemFreeDelayedDirect((sdfMemHandle_t *)mhfldWaveFile);
        mhfldWaveFile = NULL;
    }
    if (fldJDataHandle == 0 && gFldRndEncBtl == 0) {
        fldPlayerAFree();
        fldLoadNextFile_SetId_BtlRet(0, 0);
    }
}

void fldShutoff(void)
{
    dds3Basic_t *handle;
    u8 *p;

    if (fldGetUnitControl()->handle != NULL) {
        handle = fldGetUnitControl()->handle;
        if (dds3GetControlScene() != NULL) {
            dds3GetPosPoseSet(handle);
            p = (u8 *)&fldSceneParam;
            VU0_STORE_VF10(p);
            dds3GetRotPoseSet(handle);
            VU0_STORE_VF10(p + 0x10);
        }
        fldGetUnitControl()->handle = NULL;
        fldGetUnitControl()->handle2 = NULL;
        fldGetFollowCameraControl()->handle = NULL;
        fldJDataClear();
        fldRunDanteShutdown();
    }
}

void fldAllShutDown(void)
{
    fldEveHit_End();
    KillOoyTest();
    fldShutoff();
    evtFreeStage();
    fldProcShutDown();
    D_002FC3D8 = 0;
}

fldUnitControl_t *fldGetUnitControl(void)
{
    return &fldUnitControl;
}

fldFollowCameraControl_t *fldGetFollowCameraControl(void)
{
    return &fldFollowCameraControl;
}

dds3Basic_t *fldGetUnitFollowCamera(void)
{
    return fldGetFollowCameraControl()->handle;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRegistUnitPad);

void fldReleaseUnitPad(void)
{
    fldUnitControl_t *ctl = fldGetUnitControl();

    if (ctl->handle != NULL) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(ctl->handle, 0);
    }
    fldEncountCheckCount = 4;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531770);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531780);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531790);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005317A0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005317B0);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetUnit);

INCLUDE_ASM(const s32, "code_00116AD0", fldAnaStart);

INCLUDE_ASM(const s32, "code_00116AD0", fldWaveInit);

INCLUDE_ASM(const s32, "code_00116AD0", fldWaveStart);

void fldWaveTask(void)
{
    fldWave_t *p;
    s32 i;

    if (mhfldWaveFile == NULL) {
        return;
    }
    p = fldWaveBuff;
    for (i = 0x27; i >= 0; i--, p++) {
        if (p->active == 0) {
            continue;
        }
        if (++p->count >= 0x47) {
            dds3ObjectBaseOnFlag(p->handle, 1);
            p->active = 0;
        }
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldSetUnitFollowCamera);

s32 fldUnitPad(dds3PoseSet_t *pUpdatePoseSet, dds3Basic_t *pBasic)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldChkVortex);

s32 fldLightMaIdxCalc(void)
{
    s32 idx = 0x64;

    if (fldGlobalBase.fld == 0x15) {
        idx = 0x66;
        if (fldGlobalBase.area == 0x02) {
            idx = 0x64;
        }
        if (fldGlobalBase.area == 0x17) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.fld == 0x1A) {
        idx = 0x66;
        if (fldGlobalBase.area == 0x12) {
            idx = 0x65;
        }
        if (fldGlobalBase.area == 0x13) {
            idx = 0x65;
        }
        if (fldGlobalBase.area == 0x15) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.fld == 0x1D) {
        idx = 0x66;
        if (fldGlobalBase.area == 0x01) {
            idx = 0x64;
        }
        if (fldGlobalBase.area == 0x09) {
            idx = 0x65;
        }
        if (fldGlobalBase.area == 0x0A) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.fld == 0x25) {
        if (fldGlobalBase.area == 0x0F) {
            idx = 0x64;
        }
    }
    return idx;
}

void fldLightMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x8) != 0) {
        fldLightMaClear();
        fldGlobalBase.flag |= 0x1;
    }
}

void fldLightMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x8;
    fldGlobalBase.flag &= ~0x1;
}

void fldLightMaStart(void)
{
    if (gFldNowDZone != 0) {
        if ((DDS3_GBWK->maFlags & 0x8) == 0) {
            fldSePlay(0x29);
        }
        fldGlobalBase.lightMaIdx = fldLightMaIdxCalc();
        DDS3_GBWK->maFlags |= 0x8;
        fldGlobalBase.flag &= ~0x1;
        fldSkySetTblLightMa(fldGlobalBase.lightMaIdx, 0);
    }
}

s32 fldLightMaChk(void)
{
    if ((DDS3_GBWK->maFlags & 0x8) != 0) {
        return 1;
    }
    return gFldNowDZone != 0 ? 0 : -1;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldLightMaTask);

void fldRifutoMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x4) != 0) {
        fldRifutoMaClear();
        fldGlobalBase.flag |= 0x2;
    }
}

void fldRifutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x4;
    fldGlobalBase.flag &= ~0x2;
}

void fldRifutoMaStart(void)
{
    DDS3_GBWK->maFlags |= 0x4;
    fldGlobalBase.flag &= ~0x2;
}

s32 fldRifutoMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x4) {
        return 1;
    }
    return 0;
}

void fldRiberaMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x2) != 0) {
        fldRiberaMaClear();
        fldGlobalBase.flag |= 0x4;
    }
}

void fldRiberaMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x2;
    fldGlobalBase.flag &= ~0x4;
}

void fldRiberaMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x2) & ~0x1;
    fldGlobalBase.flag &= ~0x4;
}

s32 fldRiberaMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x2) {
        return 1;
    }
    return 0;
}

void fldEsutoMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x1) != 0) {
        fldEsutoMaClear();
        fldGlobalBase.flag |= 0x8;
    }
}

void fldEsutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x1;
    fldGlobalBase.flag &= ~0x8;
}

void fldEsutoMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x1) & ~0x2;
    fldGlobalBase.flag &= ~0x8;
}

s32 fldEsutoMaChk(void)
{
    if (DDS3_GBWK->maFlags & 0x1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldAnahoriSelectInit);

INCLUDE_ASM(const s32, "code_00116AD0", fldAnahoriSelectGet);

void fldAnahoriSelectNext(void)
{
    s32 idx = gFldAnahoriAkumaIdx + 1;

    if (idx >= gFldAnahoriAkumaIdxMax) {
        idx = 0;
    }
    gFldAnahoriAkumaIdx = idx;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldAnahoriSelectDel);

INCLUDE_ASM(const s32, "code_00116AD0", fldOtInit);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531830);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531840);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531850);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531860);

INCLUDE_ASM(const s32, "code_00116AD0", ProcRandomEncountCheck);

s32 fldProcKiretaChk(void)
{
    return scrSearchScriptName(D_00531860) != NULL;
}

s32 fldProcHirakenaiChk(void)
{
    return scrSearchScriptName(D_005318E0) != NULL;
}

s32 fldProcCampChk(void)
{
    if (fldProcCampWait > 0) {
        return 2;
    }
    if (campCheckExitState() != 0) {
        return 1;
    }
    return fldEncountTestFlag(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "code_00116AD0", ProcCampCheck);

s32 fldProcAmChk(void)
{
    if (fldProcAmWait > 0) {
        return 2;
    }
    return fldAutoMapChkMode() != 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", ProcAmCheck);

INCLUDE_ASM(const s32, "code_00116AD0", RetryMapLoad);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005318E0);

INCLUDE_ASM(const s32, "code_00116AD0", ProcSequence);

INCLUDE_ASM(const s32, "code_00116AD0", ProcDraw);

void fldAutoMapSeqSetFlag(u32 flag)
{
    fldAutoMapSeqFlags[0] |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags[0] &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags[0] & flag) != 0;
}

void fldEncountSetFlag(u32 flag)
{
    fldEncountFlags |= flag;
}

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}

INCLUDE_ASM(const s32, "code_00116AD0", fldProcTrigSequence);

INCLUDE_ASM(const s32, "code_00116AD0", fldProcShutDown);

s32 fldProcShutDown2(void)
{
    if (fldJDataHandle == 0) {
        if (fldLoadNextGetStatus2() != 0) {
            fldLoadNextTask();
            if (fldLoadNextGetStatus() == 1) {
                return 0;
            }
            fldFreeNextFile();
            return 0;
        }
    }
    if (fldBgmIsLoading() != 0) {
        fldWaitBgmLoad();
        return 0;
    }
    return sdfDelayedResrcCheck() == 0;
}

void fldProcSetTerminal(s32 type, s32 idx)
{
    fldSetTerminalState(type, idx);
}

void fldProc_SisetuReturnPoint(s32 fld, s32 area, void *pName)
{
    fldPauseBgm(fld, area, pName);
}

void fldProc_CallSisetuReturn(void)
{
    if (fldNext_fld != 0) {
        fldProcNextJump();
    } else {
        fldDoor_SiseExit();
    }
}

void fldSetNextField(s32 fld, s32 area)
{
    fldNext_fld = fld;
    D_0054EB7C = area;
    if (fld == 14 && area == 0x25B) {
        fldNextFieldId = 1;
    }
}

void fldProcNextJump(void)
{
    if (fldNext_fld != 0) {
        if (dds3AdminiGetNextSequence() > 0) {
            return;
        }
        if (dds3AdminiGetNextSequence() < 0) {
            if (dds3AdminiGetPrevHistoryFlag() & 1) {
                return;
            }
        }
        fldDoor_EveBtlExit(fldNext_fld, D_0054EB7C);
        fldNext_fld = 0;
    }
}

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA08);

INCLUDE_SDATA(const s32, "code_00116AD0", slNewGameBit2211);

INCLUDE_SDATA(const s32, "code_00116AD0", slNewGameBit2212);

INCLUDE_SDATA(const s32, "code_00116AD0", slNewGameBit2213);

INCLUDE_SDATA(const s32, "code_00116AD0", slNewGameBit2323);

INCLUDE_SDATA(const s32, "code_00116AD0", slNewGameBit162);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA15);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA16);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA18);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA1C);

INCLUDE_SDATA(const s32, "code_00116AD0", DDS3_GBWK);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA24);

INCLUDE_SDATA(const s32, "code_00116AD0", dds3FontReqTable);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA2C);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA30);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA38);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA40);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA48);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA50);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA58);

INCLUDE_SDATA(const s32, "code_00116AD0", fldCamDirLock);

INCLUDE_SDATA(const s32, "code_00116AD0", fldCamPitch);

INCLUDE_SDATA(const s32, "code_00116AD0", fldCamLength);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldElvFloorNow);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvFloorFrom);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvFloorNum);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvFloorTo);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvFloorDisp);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvFloorTarget);

INCLUDE_SDATA(const s32, "code_00116AD0", fldElvState);

INCLUDE_SDATA(const s32, "code_00116AD0", fldShadowWork);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA8C);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EA90);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRun_MAP_OX);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRun_MAP_OZ);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRun_MAP_BX);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRun_MAP_BZ);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunMapData);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunMapHeader);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunMapCellData);

INCLUDE_SDATA(const s32, "code_00116AD0", pFroMapHead);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunMapScale);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAB0);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunTama2Basic);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunTama3Basic);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EABC);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAC0);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAC8);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAD0);

INCLUDE_SDATA(const s32, "code_00116AD0", fldRunDanteSe_asise);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EADC);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAE0);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAE8);

INCLUDE_SDATA(const s32, "code_00116AD0", fldPlayerUnit);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EAF0);

INCLUDE_SDATA(const s32, "code_00116AD0", l_UseCollision);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldNowDZone);

INCLUDE_SDATA(const s32, "code_00116AD0", fldPlayerAnimData);

INCLUDE_SDATA(const s32, "code_00116AD0", mhfldPlayerFile);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldPlayerPtr);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldPlayerSize);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldPlayerSizeBk);

INCLUDE_SDATA(const s32, "code_00116AD0", mhfldAnaFile);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldAnaPtr);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldAnaSize);

INCLUDE_SDATA(const s32, "code_00116AD0", fldAnaBasic);

INCLUDE_SDATA(const s32, "code_00116AD0", mhfldWaveFile);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldWavePtr);

INCLUDE_SDATA(const s32, "code_00116AD0", gfldWaveSize);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB30);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB38);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB40);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldAnahoriAkumaIdx);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldAnahoriAkumaIdxMax);

INCLUDE_SDATA(const s32, "code_00116AD0", fldNextFieldId);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB54);

INCLUDE_SDATA(const s32, "code_00116AD0", fldEncountSagasiFlag);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB5C);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldRndEncBtl);

INCLUDE_SDATA(const s32, "code_00116AD0", fldEncountCheckWork);

INCLUDE_SDATA(const s32, "code_00116AD0", fldProcCampWait);

INCLUDE_SDATA(const s32, "code_00116AD0", fldProcAmWait);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldStart);

INCLUDE_SDATA(const s32, "code_00116AD0", fldProcShutDownFlg);

INCLUDE_SDATA(const s32, "code_00116AD0", fldNext_fld);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB7C);

INCLUDE_SDATA(const s32, "code_00116AD0", fldAutoMapSeqFlags);

INCLUDE_SDATA(const s32, "code_00116AD0", fldSequenceState);

INCLUDE_SDATA(const s32, "code_00116AD0", fldEncountFlags);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB8C);

INCLUDE_SDATA(const s32, "code_00116AD0", fldEncountTimer);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB94);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531940);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531950);

