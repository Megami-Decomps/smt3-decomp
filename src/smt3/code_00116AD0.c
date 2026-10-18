#include "common.h"

#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
extern u8 D_002FA930[];
extern char *D_002FA938[];
extern char D_0054EA38[], D_0054EA40[], D_0054EA48[], D_0054EA50[], D_0054EA58[];
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 style, const char *format, ...);
extern f64 sdfFloatToULong(f32 value);
extern s32 fldDoor_ElvTakasaToFloor(s32 floor);
extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *scene, s32 type);
extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *list);
extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *list);
extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *list);
extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *list);
extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);
extern void rsetOtOfs(u32 buffer);
extern void fldDrwZSave(void);
extern void fldDrwZLoad(void);
extern f32 R360_LR(f32 current, f32 target);
extern void dds3MenuStepValue(char *text, f32 value);

extern sdfVector4_t fldCamTarget;
extern s32 fldGb __attribute__((section(".data")));
extern s32 dds3EffectFldTypeIsSpecial(s32 charId);
extern s32 dds3EffectFldCharToType(s32 charId);


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
    u32 firstOffset;
    u32 cellCount;
    u32 secondOffset;
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
    u8 unk00[0x30];
    u32 unk30;
    s32 mode;
    u32 sequenceWord38;
    u32 sequenceWord3C;
    char name[16];
    s32 stage;
    s32 kind;
    s32 enabled;
    s32 restore;
    u16 code;
    u16 sequenceMode;
    s32 link;
    u8 unk68[8];
    char detail[16];
    char note[16];
    u32 options;
    u8 unk94[12];
} fldSceneParam_t;
typedef struct {
    u32 unk00;
    u32 cellsOffset;
} fldRunMapGrid_t;

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

typedef struct {
    u8 unk00[0x44];
    s32 enabled;
    u32 unk48;
    s32 counter;
} fldDanteGunWork_t;

extern fldDanteGunWork_t fldRunDanteGunFlag;
extern u8 fldDoorExitFlag;
extern u8 fldDoorExitMode[];
extern char D_0054EB30[];
extern char D_00531690[], D_00531940[], D_00531950[];
extern s32 fldProcShutDownFlg;
extern s32 ProcSequence(dds3ProcessID_t *process);
extern s32 ProcDraw(dds3ProcessID_t *process);
extern void fclClearTerminalState(void);
extern void fldMakeFilePath(char *path, s32 area, s32 floor);
extern s32 sprintf(char *text, const char *format, ...);
extern sdfMemHandle_t *sdfDevLoadFileEx(char *path, void *destination, s32 flags);
extern s32 slGetExitType(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 mode);
extern void fldGbInit_System(s32 mode);
extern void evtBitOn(s32 flag);
extern void fldSetNextField(s32 field, s32 parameter);
void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);

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

extern fldSequenceController_t fldAutoMapSeqFlags;
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
extern void *fldRunMapScale;
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
extern void dds3DebugIntEditEx(void *value, s32 type, s64 min, s64 max, s64 step, s64 bigStep, s8 *pad);
extern s32 dds3DebugColorEditEx(u32 *value, s32 digits, s8 *pad);
extern s32 ooyTestDrawProc(dds3ProcessID_t *id);

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

u64 *dds3PrimFrameRect(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    packet[0] = 7;
    right = x + width;
    bottom = y + height;
    depthHigh = (u64)(u32)depth << 32;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0x55555105510ULL;
    packet[4] = 0x146;
    packet[5] = colorA | 0x3F80000000000000ULL;
    packet[6] = (u32)(((u32)x & 0xFFFF) | ((u32)y << 16)) | depthHigh;
    packet[7] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[8] = 0x142;
    packet[9] = colorB | 0x3F80000000000000ULL;
    packet[14] = (u32)(((u32)(x - 0x10) & 0xFFFF) | ((u32)(y - 8) << 16)) | depthHigh;
    packet[10] = packet[14];
    packet[11] = (u32)(((u32)right & 0xFFFF) | ((u32)(y - 8) << 16)) | depthHigh;
    packet[12] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[13] = (u32)(((u32)(x - 0x10) & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[15] = 0;
    return packet;
}

u64 *dds3PrimLine(s32 x0, s32 y0, s32 z0, s32 color0, s32 x1, s32 y1, s32 z1, s32 color1, s32 flags) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x50);

    packet[0] = 4;
    packet[1] = 0x5000000400000000ULL;
    packet[2] = 0x5400000000008001ULL;
    packet[3] = 0x51510;
    packet[4] = flags | 0x109;
    packet[5] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | ((u64)((u32)z0 >> 4) << 32);
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | ((u64)((u32)z1 >> 4) << 32);
    packet[9] = 0;
    return packet;
}

INCLUDE_ASM(const s32, "code_00116AD0", dds3PrimLine3D);

void dds3DebugFloatEditEx(f32 *controlledValue, u8 *padState, f32 minimum, f32 maximum, f32 step, f32 bigStep) {
    f32 adjustedValue = *controlledValue;

    if ((s8)padState[5] & 0x80) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[5] & 2) {
        adjustedValue += bigStep;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[4] & 0x80) {
        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[4] & 2) {

        adjustedValue -= bigStep;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else if ((s8)padState[7] & 0x80) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = minimum;
        }
    } else if (padState[7] & 2) {
        adjustedValue += step;
        if (maximum < adjustedValue) {
            adjustedValue = maximum;
        }
    } else if ((s8)padState[6] & 0x80) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = maximum;
        }
    } else if (padState[6] & 2) {
        adjustedValue -= step;
        if (adjustedValue < minimum) {
            adjustedValue = minimum;
        }
    } else {
        return;
    }
    *controlledValue = adjustedValue;
}

void dds3DebugFloatEdit(f32 *value, f32 step, f32 min, f32 max, f32 scale)
{
    dds3DebugFloatEditEx(value, &dds3PadButtonState[0x20], step, min, max, scale);
}

void dds3DebugIntEditEx(void *valueAddress, s32 storageType, s64 min, s64 max, s64 step, s64 bigStep, s8 *padState) {
    s64 adjustedValue;
    switch (storageType) {
    case 1:
        adjustedValue = *(u8 *)valueAddress;
        break;
    case 2:
        adjustedValue = *(u16 *)valueAddress;
        break;
    case 4:
        adjustedValue = *(u32 *)valueAddress;
        break;
    case -1:
        adjustedValue = *(s8 *)valueAddress;
        break;
    case -2:
        adjustedValue = *(s16 *)valueAddress;
        break;
    case -4:
        adjustedValue = *(s32 *)valueAddress;
        break;
    default:
        return;
    }
    if (padState[5] & 0x80) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[5] & 2) {
        adjustedValue += bigStep;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if (padState[4] & 0x80) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = max;
        }
    } else if (padState[4] & 2) {
        adjustedValue -= bigStep;
        if (adjustedValue < min) {
            adjustedValue = min;
        }
    } else if (padState[7] & 0x80) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = min;
        }
    } else if (padState[7] & 2) {
        adjustedValue += step;
        if (max < adjustedValue) {
            adjustedValue = max;
        }
    } else if (padState[6] & 0x80) {
        adjustedValue -= step;
        if (adjustedValue < min) {
            adjustedValue = max;

        }
    } else if (padState[6] & 2) {
        adjustedValue -= step;
        if (adjustedValue < min) {
            adjustedValue = min;
        }
    } else {
        return;
    }
    switch (storageType) {
    case -1:
    case 1:
        *(u8 *)valueAddress = adjustedValue;
        break;
    case -2:
    case 2:
        *(u16 *)valueAddress = adjustedValue;
        break;
    case -4:
    case 4:
        *(u32 *)valueAddress = adjustedValue;
        break;
    }
}

void dds3DebugIntEdit(void *value, s32 a1, s64 a2, s64 a3, s64 a4, s64 a5)
{
    dds3DebugIntEditEx(value, a1, a2, a3, a4, a5, (s8 *)&dds3PadButtonState[0x20]);
}

s32 dds3DebugColorEditEx(u32 *color, s32 channel, s8 *padState) {
    s32 originalColor = *color;
    s32 channelValue = originalColor;
    s32 updatedColor;
    switch (channel) {
    case 0:
        break;
    case 1:
        channelValue = originalColor >> 8;
        break;
    case 2:
        channelValue = originalColor >> 16;
        break;
    case 3:
        channelValue = originalColor >> 24;
        break;
    }
    channelValue &= 0xFF;
    if (((padState[5] & 0x80) || (padState[7] & 0x80)) && channelValue == 0xFF) {
        channelValue = 0;
    } else if (padState[5] & 2) {
        channelValue += 10;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (((padState[4] & 0x80) || (padState[6] & 0x80)) && channelValue == 0) {
        channelValue = 0xFF;
    } else if (padState[4] & 2) {
        channelValue -= 10;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else if (padState[7] & 2) {
        channelValue += 1;
        if (channelValue >= 0x100) {
            channelValue = 0xFF;
        }
    } else if (padState[6] & 2) {
        channelValue -= 1;
        if (channelValue < 0) {
            channelValue = 0;
        }
    } else {
        return 0;
    }
    switch (channel) {
    case 0:
        updatedColor = (originalColor & 0xFFFFFF00) | channelValue;
        break;
    case 1:
        updatedColor = (originalColor & 0xFFFF00FF) | (channelValue << 8);
        break;
    case 2:
        updatedColor = (originalColor & 0xFF00FFFF) | (channelValue << 16);
        break;
    default:
        updatedColor = (originalColor & 0x00FFFFFF) | (channelValue << 24);
        break;
    }
    *color = updatedColor;
    return updatedColor != originalColor;
}

void dds3DebugColorEdit(u32 *value, s32 digits)
{
    dds3DebugColorEditEx(value, digits, (s8 *)&dds3PadButtonState[0x20]);
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

void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values) {
    char text[16];
    u32 color = 0x80000000;
    s32 i;
    s32 style;

    if (values != NULL) {
        color = 0;
        for (i = 0; i != 3; i++) {
            s32 channel = values[i] * 128.0f;
            if (channel > 0) {
                if (channel >= 256) {
                    channel = 255;
                }
                color |= channel << (i * 8);
            }
        }
        color |= 0x80000000;
    }
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x510, y + 0x18,
                     0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y,
                        0xFF0080, D_002FA930[i], D_002FA938[i]));
        style = selected == i ? 6 : 0;
        if (values == NULL) {
            text[0] = '-';
            text[1] = '-';
            text[2] = '-';
            text[3] = '-';
            text[4] = 0;
        } else {
            dds3MenuStepValue(text, values[i]);
        }
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x + 0x180,
                        y, 0xFF0080, style, text));
        y += 0x60;
    }
}

void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized) {
    s32 i;
    s32 style;
    s32 channel;
    const char *label;

    color = (color & 0xFFFFFF) | 0x80000000;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x510, y + 0x18,
                    0xFF0080, 0x240, 0xF0, color, 0x40806020));
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            style = 2;
            label = D_0054EA48;
            channel = color & 0xFF;
            break;
        case 1:
            style = 4;
            label = D_0054EA40;
            channel = (color >> 8) & 0xFF;
            break;
        default:
            style = 5;
            label = D_0054EA38;
            channel = (color >> 16) & 0xFF;
            break;
        }
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                        x, y, 0xFF0080, style, label));
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                        x + 0x180, y, 0xFF0080, i == selected ? 6 : 0,
                        D_0054EA50, channel));
        if (showNormalized != 0) {
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                            x, y + 0x180, 0xFF0080, style, label));
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
                            x + 0x240, y + 0x180, 0xFF0080, 0,
                            D_0054EA58, sdfFloatToULong(channel * (1.0f / 255.0f))));
        }
        y += 0x60;
    }
}

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

void fldElvSetFloor(s32 start, s32 end) {
    s32 first;
    s32 endEntry;

    first = fldDoor_ElvTakasaToFloor(start);
    fldElvState = 0;
    fldElvFloorFrom = first;
    if (first > 1000) {
        gFldElvFloorNow = first % 1000;
    } else {
        gFldElvFloorNow = first;
    }
    if (first > 1000) {
        fldElvFloorNum = first;
        fldElvFloorFrom = first % 1000;
    } else {
        fldElvFloorNum = -999;
    }

    endEntry = fldDoor_ElvTakasaToFloor(end);
    fldElvFloorTo = endEntry;
    if (endEntry > 1000) {
        fldElvFloorDisp = endEntry;
        fldElvFloorTo = endEntry % 1000;
    } else {
        fldElvFloorDisp = -999;
    }
    if (start == end) {
        fldElvFloorTarget = -999;
    } else {
        fldElvFloorTarget = 0;
    }
    if (gFldElvFloorNow != -999 && fldElvFloorTarget != -999) {
        fldElvFloorTarget++;
        if (fldElvFloorFrom < fldElvFloorTo) {
            fldSetElvCameraScene(1);
        } else {
            fldSetElvCameraScene(-1);
        }
    }
}

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

void drawSceneBasicList(void) {
    s32 hasEligibleBackground = 0;
    dds3BasicArrayHandle_t *objectChain;
    dds3Basic_t *worldObject;

    objectChain = dds3SceneBasicGetList(dds3GetControlScene(), 5);
    if (objectChain != 0) {
        if (dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0) {
            do {
                worldObject = (dds3Basic_t *)dds3GetBasicArray((dds3BasicArrayHandle_t *)objectChain);
                if (dds3ObjectBaseCheckFlag(worldObject, 0x200) != 0) {
                    if (dds3ObjectBaseCheckFlag(worldObject, 1) == 0) {
                        hasEligibleBackground = 1;

                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)objectChain) != 0);
        }
        dds3FreeBasicArrayHandle(objectChain);
    }
    if (fldGb == 1) {
        hasEligibleBackground = 0;
    }
    if (hasEligibleBackground != 0) {
        rsetOtOfs(0x24);
        fldDrwZSave();
        rsetOtOfs(0x26);
        fldDrwZLoad();
    }
}

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

void fldMakeFroFileName(void) {
    char directory[0x20];
    char path[0x40];
    fldRunMapHeader_t *resource;
    u32 base;
    u32 first;
    u32 second;
    u32 third;

    fldMakeFilePath(directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    sprintf(path, "%sF%03d_%03d.FRO", directory, fldGlobalBase.area, fldGlobalBase.floor + 1);
    fldRunMapData = sdfDevLoadFileEx(path, &resource, 0);
    base = (u32)resource;
    first = base + resource->firstOffset;
    second = base + resource->secondOffset;
    third = base + ((fldRunMapGrid_t *)second)->cellsOffset;
    fldRunMapHeader = (fldRunMapHeader_t *)base;
    fldRunMapCellData = (fldMapCell_t *)first;
    pFroMapHead = (fldRunMapGrid_t *)second;
    fldRunMapScale = (void *)third;
}

void fldFreeRunMapData(void)
{
    if (fldRunMapData != NULL) {
        sdfMemFreeDelayedDirect(fldRunMapData);
        fldRunMapData = NULL;
        fldRunMapHeader = NULL;
        fldRunMapCellData = NULL;
        pFroMapHead = NULL;
        fldRunMapScale = NULL;
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

    dx = fldGlobalBase.x - fldRun_Dante.x;
    dz = fldGlobalBase.z - fldRun_Dante.z;
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

f32 fldTurnToward(f32 cur, f32 target, f32 speed, f32 minStep) {
    f32 delta = R360_LR(cur, target);
    f32 step = delta / speed;

    if (step < 0.0f) {
        step = -step;
    }
    if (step < minStep) {
        step = minStep;
    }
    if ((delta >= 0.0f && delta <= step) || (delta <= 0.0f && -step <= delta)) {
        delta = target;
    } else if (delta < 0.0f) {
        delta = cur - step;
    } else {
        delta = cur + step;
    }
    return delta;
}

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

void fldMainLoadStart(void) {
    fldSceneParam_t record;
    u32 mode;

    if (slGetExitType() == 1) {
        return;
    }
    if (DDS3_GBWK->transition != 0) {
        mode = 1;
        dds3AdminiCallSequence(0x29, &mode, 4, 0);
        return;
    }
    fldGbInit_System(1);
    fldRunDanteGunFlag.counter = 0;
    fldRunDanteGunFlag.enabled = 1;
    evtBitOn(0xC0F);
    fldSetNextField(0, 0);
    fldSetSceneParam2(&record, 1, 1, D_0054EB30);
    record.options = 1;
    dds3AdminiCallSequence(6, &record, 0xA0, 0);
}

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

void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    record->unk30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->restore = 2;
    record->kind = kind;
    record->code = 0;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
    fldDoorExitFlag = 0;
    fldDoorExitMode[0] = 0;
}

void fldSetSceneParam3(fldSceneParam_t *record, s32 stage, s32 kind, const char *name) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    record->unk30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->restore = 3;
    record->kind = kind;
    record->code = 0;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    record->unk30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->restore = 2;
    record->sequenceMode = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

void fldSetSceneParam5(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (dds3SearchProcessName(D_00531690) != NULL) {
        if (fldGlobalBase.area == stage) {
            fldGlobalBase.reuseAreaResources = 1;
        } else {
            fclClearTerminalState();
        }
    }
    record->unk30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->restore = 2;
    record->kind = kind;
    record->code = code;
    record->sequenceMode = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->options = 0;
}

void fldSetSceneParamLoad(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk30 = 0;
    record->sequenceWord3C = 0;
    record->sequenceWord38 = 0;
    strcpy(record->name, name);
    record->enabled = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->restore = 2;
    record->sequenceMode = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->options = 0;
}

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
            SDF_VU0_STOREVECTOR(p);
            dds3GetRotPoseSet(handle);
            SDF_VU0_STOREVECTOR(p + 0x10);
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

    if (fldGlobalBase.area == 0x15) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x02) {
            idx = 0x64;
        }
        if (fldGlobalBase.floor == 0x17) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x1A) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x12) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x13) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x15) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x1D) {
        idx = 0x66;
        if (fldGlobalBase.floor == 0x01) {
            idx = 0x64;
        }
        if (fldGlobalBase.floor == 0x09) {
            idx = 0x65;
        }
        if (fldGlobalBase.floor == 0x0A) {
            idx = 0x65;
        }
    }
    if (fldGlobalBase.area == 0x25) {
        if (fldGlobalBase.floor == 0x0F) {
            idx = 0x64;
        }
    }
    return idx;
}

void fldLightMaClearMsg(void)
{
    if ((DDS3_GBWK->maFlags & 0x8) != 0) {
        fldLightMaClear();
        fldGlobalBase.consumedFlags |= 0x1;
    }
}

void fldLightMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x8;
    fldGlobalBase.consumedFlags &= ~0x1;
}

void fldLightMaStart(void)
{
    if (gFldNowDZone != 0) {
        if ((DDS3_GBWK->maFlags & 0x8) == 0) {
            fldSePlay(0x29);
        }
        fldGlobalBase.lightMaIdx = fldLightMaIdxCalc();
        DDS3_GBWK->maFlags |= 0x8;
        fldGlobalBase.consumedFlags &= ~0x1;
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
        fldGlobalBase.consumedFlags |= 0x2;
    }
}

void fldRifutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x4;
    fldGlobalBase.consumedFlags &= ~0x2;
}

void fldRifutoMaStart(void)
{
    DDS3_GBWK->maFlags |= 0x4;
    fldGlobalBase.consumedFlags &= ~0x2;
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
        fldGlobalBase.consumedFlags |= 0x4;
    }
}

void fldRiberaMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x2;
    fldGlobalBase.consumedFlags &= ~0x4;
}

void fldRiberaMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x2) & ~0x1;
    fldGlobalBase.consumedFlags &= ~0x4;
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
        fldGlobalBase.consumedFlags |= 0x8;
    }
}

void fldEsutoMaClear(void)
{
    DDS3_GBWK->maFlags &= ~0x1;
    fldGlobalBase.consumedFlags &= ~0x8;
}

void fldEsutoMaStart(void)
{
    DDS3_GBWK->maFlags = (DDS3_GBWK->maFlags | 0x1) & ~0x2;
    fldGlobalBase.consumedFlags &= ~0x8;
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
    fldAutoMapSeqFlags.flags |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
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

void fldProcTrigSequence(fldSceneParam_t *record, u32 mode) {
    fldSceneParam_t *scene = &fldSceneParam;
    fldSequenceController_t *controller = &fldAutoMapSeqFlags;

    controller->stage = 0;
    controller->flags = 0;
    scene->mode = mode;
    if (mode == 0) {
        scene->sequenceWord3C = record->sequenceWord3C;
        scene->sequenceWord38 = record->sequenceWord38;
        strcpy(scene->name, record->name);
        scene->stage = record->stage;
        scene->code = record->code;
        scene->kind = record->kind;
        scene->sequenceMode = record->sequenceMode;
        scene->enabled = record->enabled;
        scene->restore = record->restore;
        scene->link = record->link;
        strcpy(scene->detail, record->detail);
        strcpy(scene->note, record->note);
        scene->options = record->options;
    }
    dds3StartProcess(D_00531940, 0x3F7, 0, 0, ProcSequence, 0, 0);
    dds3StartProcess(D_00531950, 0x2B0A, 0, 0, ProcDraw, 0, 0);
    fldProcShutDownFlg = 1;
    InitOoyTest();
}

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

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531940);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531950);

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

