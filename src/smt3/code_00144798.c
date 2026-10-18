#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"

#include "ps2asm.h"
/* Field-only views, with offsets taken from the SMT3 retail accesses. */
typedef struct
{
    s32 flags;
    s32 soundId;
    f32 radius;
    f32 sourceX;
    f32 sourceY;
    f32 sourceZ;
} fldSe3D_t;

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


typedef struct
{
    s32 gunFlag;
    u8 unk04[0x28];
    s32 skyTable;              /* 0x2C */
    u8 unk30[4];
    s32 filter;                /* 0x34 */
} fldEnvironmentWork_t;

typedef struct
{
    void *resourceState;
    u32 roomNumber;
} fldMatterWork_t;

typedef struct
{
    s32 unk00;
    s32 value;
} fldDanteParam_t;

typedef struct
{
    u8 data[0x100];
} fldNpcPalette_t;


typedef struct fldFileRequest_t fldFileRequest_t;
typedef struct fldUnit_t fldUnit_t;
typedef struct fldInputPanel_t fldInputPanel_t;

enum sl_mc_result
{
    SL_MC_RESULT_PENDING = 0,
    SL_MC_RESULT_SUCCESS = 1,
    SL_MC_RESULT_ERROR = -1,
    SL_MC_RESULT_SPECIAL_ERROR = -2
};
typedef enum sl_mc_result slMcResult_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern fldSe3D_t fldSe3DTable[8];
extern s32 gFldSe3DNum;
extern s32 D_002FC020[];
extern s32 fldBgmTrackTable[];
void p2sdPlayBGM(s32 trackId);
extern u32 fldSeLastBgm;
extern void p2sdSendStopCmd(u32 controlWord);
extern void p2sdStopBGM(s32 trackHandle);
extern void p2sdStopAllBgm(void);
extern void p2sdStopSe(u32);
extern s32 D_0054EE3C;
extern s32 D_0030A2D8[];
extern s32 fldBgmReloadStep;
extern s32 p2sdLoadCheckMidi(s32);
extern s32 D_0054EE40;
extern void p2sdLoadSMGFile(s32);
extern s32 fldBgmLoadStep;
extern s32 fldBgmSmgAddr;
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
extern s32 fldGbChkNpc(s32 mapId, u32 slotIndex, s32 bit);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *object, u32 flags);
extern u8 D_003129D0[0x100];
extern char D_005327B0[];
dds3ProcessID_t *dds3SearchProcessName(const char *name);
extern char D_00532820[];
extern sdfTexHandle_t *fldTitleMiniTex;
extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);
extern sdfTexHandle_t *fldTitleMiniWork;
extern s32 fldSnowActive;
extern s32 slProcShutDownFlg;
extern s32 gSlWindowCnt;
extern s32 gslInfoCode;
extern s32 slChooseSaveIndex;
extern u32 D_005516BC;
void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);
s8 sdfMcGetSlotIndex(s32 request);
u32 sdfMcGetSlotFlags(s32 request, s32 slot);
extern s32 slChooseData_getCardInfoNext(void);
extern void mcMakeFileName(char *dst, s32 number);
extern u32 strlen(const char *s);
extern void *memcpy(void *, const void *, unsigned int);
void sdfMcFuncOpen(u32 context, const char *path, s32 openFlags);
s32 slHeadLoadProc_openFile(void);
extern s32 D_00551D50;
slMcResult_t sdfMcFuncOpen1(s32 *resultOut);
sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);
void *sdfMemLock(sdfMemHandle_t *allocation);
extern sdfMemHandle_t *slLoadFileFd;
extern void sdfMcFuncRead(s32, u32, s32);
extern void *slLoadFileHandle;
s32 slHeadLoadProc_readFile(void);
extern s32 slNoCard(void);
slMcResult_t sdfMcFuncRead1(void);
extern void sdfMcFuncClose(s32);
extern s32 slHeadLoadProc_closeFile(void);
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);
extern void *slExitWaitNextFunc;
s32 slExitWaitProc(void);
extern s32 slExitWaitCnt;
extern s32 gslMode;
extern s32 slTop(void);
s32 slEndingExit(void);
extern u32 gslErrorCode;
extern u32 slQueryYesNoMsg;
extern u32 gslDrawFlagFile;
s32 slQueryYesNo(void *start, void *finish, s32 mode);
s32 slSaveData(void);
extern s32 slFileSel(void);
s32 slExitToTitle(void);
s32 slBootProc_CheckCard(void);
void sdfMcRegetInfo(s32 request);
extern s32 gslCheckCardWait;
extern u32 gSlNowCardCheck;
s32 slBootProc_CheckCardR(void);
extern u32 gslDrawFlagBg;
s32 slExitToTitleSub(void);
extern char D_005328C0[];
s32 slExitToTitle2(void);
extern char D_005328D0[];
fldFileRequest_t *fileLoad(const char *requestName);
extern sdfMemHandle_t *D_00551D44;
extern u32 baseIconFileHandle;
s32 sdfMcGetFreeBytes(s32 request);
s32 sdfMcFuncGetInfo1(void);
u8 sdfMcGetFileInfoReady(s32 request);
extern s32 slChooseData_B(void);
extern u32 gslErrorKeyFlag;
extern s8 dds3PadButtonState[];
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);
extern u32 slHitAnyKeyNextFunc;
extern s32 slHitAnyKeyNoChkProc(void);
extern s32 slHitAnyKeyProc(void);
void sdfMcFuncChdir(u32 context, const char *path);
s32 slChooseData_getCardInfo2(void);
slMcResult_t sdfMcFuncChdir1(void);
void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);
extern char D_005328E0[];
extern u8 D_005696C0[];
void sdfMcFuncGetDir(u32 context, const char *path, void *directoryEntries, s32 entryLimit);
s32 slChooseData_getCardInfo3(void);
slMcResult_t sdfMcFuncGetDir1(s32 *resultOut);
s32 slChooseData_getCardInfo2_B(void);
s32 slChooseData_getCardInfo3_B(void);
s32 slChooseData_getCardInfoNext_B(void);
void sdfMcSetCardStat(s32 request);
s32 slSaveData_chdir(void);
void sdfMcFuncMkdir(u32 context, const char *path);
s32 slSaveProc_Write_mkdir(void);
s32 slSaveProc_Write_chdir(void);
slMcResult_t sdfMcFuncMkdir1(void);
extern s32 slFileSelError;
s32 slSaveData_deleteIconSys(void);
slMcResult_t sdfMcFuncDelete1(void);
extern s32 slSaveData_writeFile(void);
void slKillLoadProcess(void);
extern char D_005328F0[];
extern s32 slSaveWriteEnd_baseIcon(void);
extern u32 D_00551D48;
extern u32 D_00551D4C;
s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, void *continuation, u32 *pendingRequestStatus);
void sdfMcFuncDelete(u32 context, const char *path);
extern u32 *slSaveWriteFile_buff;
extern u32 *slSaveWriteFile_size;
extern void *slSaveWriteFile_endFunc;
extern u32 *slSaveWriteFile_dataWaitPtr;
s32 slSaveProc_openFile(void);
s32 slSaveProc_waitDataReady(void);
extern void sdfMcFuncWrite(s32 request, u32 first, u32 second);
s32 slSaveProc_writeFile(void);
slMcResult_t sdfMcFuncWrite1(void);
s32 slSaveProc_closeFile(void);
slMcResult_t sdfMcFuncClose1(void);
slMcResult_t sdfMcFuncFormat1(void);
extern void dds3SoftResetOn(void);
s32 slCardChk(void);
extern void dds3SoftResetOff(void);
void sdfMcFuncFormat(u32 port);
extern u32 gSlEndingMode;
void slVersionConvert_Step1(void);
s32 slLoadProc_openFile(void);
s32 slLoadProc_readFile(void);
s32 slLoadProc_closeFile(void);
void slVersionConvert_Step3(void);
extern s8 D_005516C1;
extern s32 dds3ConfigChanged(void);
extern void dds3ConfigSaveBackup(void);
extern void dds3ConfigRestoreBackup(void);
extern s32 gslAnimCnt;
extern s32 slDraw(s32);
s32 pacFileCheckEnd(const fldFileRequest_t *request);
sdfMemHandle_t *fileGetMemHandle(const fldFileRequest_t *request);
u8 *fileGetLoadPtr(const fldFileRequest_t *request);
u32 fileGetLoadSize(const fldFileRequest_t *request);
s32 fileHandleFree(fldFileRequest_t *request);
extern char D_00532948[];
extern u32 gslExitType;
extern void *slQueryYesFunc;
extern void *slQueryNoFunc;
extern s32 slQueryYesNoCursor;
extern s32 slQueryYesNoKeyFlag;
extern s32 slQueryYesNoProc(void);
extern u32 gSlExitMode;
extern s32 scrGetIntPara(s32);
fldInputPanel_t *fldInputPanelSetMessage(s32 choice);
fldInputPanel_t *fldInputPanelSetDraw(s32 isPending);
extern void fldReleaseUnitPad(void);
dds3Basic_t *dds3GetActiveScene(void);
dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *world);
extern void fldPlayerMotReset(void);
extern s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *object, void *update);
extern void fldRegistUnitPad(void);
extern s32 fldCamMode[];
dds3Basic_t *fldGetUnitFollowCamera(void);
dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *world, dds3Basic_t *camera);
extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                       f32 *targetPositionOut);
extern void fldCamGetPhokanParam(void);
extern f32 fldCamPhokanSelX[];
extern f32 D_002FC9F0[];
extern s32 fldCamPhokanState;
extern s32 fldCamPHokanMode;
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);
char *scrGetStrPara(s32 idx);
dds3Basic_t *dds3GetControlScene(void);
dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *world, const u8 *name);
s32 printf(const char *format, ...);
extern char D_0054F0A8[];
extern char *strstr(const char *, const char *);
extern s32 fldCamUnit[];
extern fldEnvironmentWork_t fldRunDanteGunFlag;
extern void fldSkySetFilter(s32 area, s32 value, s32 duration);
extern void fldSetFilterAnime(s32);
extern void fldSetModelFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);
dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);
u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object);
u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object);
u32 dds3GetBasicArray(const dds3BasicArrayHandle_t *object);
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);
u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *object);
void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *node);
extern void fldSetAnimeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);
void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *object, u32 requestedMode);
extern s32 fldArea[];
extern void fldSetAtariFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);
extern s32 fldSetModelFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);
extern s32 fldSetAnimeFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);
extern void fldGetAnimeId2(s32 area, s32 slotIndex, s32 valueIndex, s32 value);
extern void fldAutoMapOpenBlock2(s32 area, s32 floor, s32 flagNumber);
extern void fldAutoMapCloseBlock2(s32 area, s32 floor, s32 flagNumber);
u32 fldGetRestoreMode(void);
scrProcessWork_s *scrGetScriptHandle(void);
extern s32 fldFindEveRngIndex(u32);
extern s32 fldDoor_GetHasigoTbl(s32);
extern s32 fldDoor_GetElvTbl(s32);
extern s32 fldElvCheckEnd(void);
extern s32 fldWapGetDoor(s32 value);
extern void scrSetIntReturnValue(s32 value);
extern s32 fldDoor_ChkWarp(void);
extern s32 fldInfNext(s32 param0, s32 param1);
extern s32 fldInfGet(s32 actorId, s32 attribute);
extern s32 fldInfChkFastPanel(s32 index);
extern void fldInfToWAP(void);
void fldBgmLoad(void);
extern void fldSePlay(s32 soundId);
extern void fldSeStop(s32 soundId);
extern void fldMotionSePlay(s32 group, s32 sound);
extern s32 fldEncountRate[];
extern const char *fldEveHitResName_Find(u32 key);
extern void fldDoor_ExitCamera(const char *entry);
extern s32 fldDoor_GetWarpEff(void);
extern fldUnit_t *evtGetUnitHandle(s32 param);
extern void fldNpcLightSet(fldUnit_t *unit);
void fldAnahoriSelectInit(void);
extern u32 fldAnahoriSelectGet(void);
void fldAnahoriSelectNext(void);
void fldAnahoriSelectDel(void);
extern fldDanteParam_t *D_0054EAF0;
extern void fldRunDanteStanceSet(s32);
extern s32 (*proc_func_ans)(dds3ProcessID_t *process);
extern s32 fldInputPanelCheckKey(void);
extern void fldEncountSetFlag(s32 flags);
extern void fldEncountClearFlag(s32 flags);
extern char *strcpy(char *dst, const char *src);
extern void fldSkySetTbl(s32 table, s32 durationMode);
extern s32 fldGetTakaraOpen(s32 room);
extern void fldSetTakaraOpen(s32 room);
extern void fldDoor_ElvExit(s32 selection);
extern void fldElvSetFloor(s32 first, s32 second);
extern void fldElvReset(void);
extern s32 fldEveHitResID_Find(u32 process);
extern void fldDoorRun_FindStart(s32 scene);
extern void fldBgmContinue(void);
extern void fldMotionSeStop(s32 group, s32 sound);
extern void fldTitleStart(s32 field, s32 option);
extern void evtBitOn(s32 bit);
extern void evtBitOff(s32 bit);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s16 flg;
    u8 spdx;
    u8 spdy;
    s16 life;
    s16 alp;
} fldSeedWork_t;

extern fldSeedWork_t *gFldSeedBuff;
extern sdfMemHandle_t *fldSnowSeedBuf;
extern s32 gFldKyouseiTalk;
extern void fldFreeNextFile(void);
extern void sdfMemUnlock(sdfMemHandle_t *allocation);
extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *allocation);
extern void fldDoor_WarpEx(s32 index);
extern char D_00532AA0[];

typedef struct
{
    s32 smgId;
    s8 trans[16];
} fldBgmEntry_t;

extern fldBgmEntry_t gFldBgmTop[];
extern fldMapPoint_t fldAutoMapFieldId;
extern s32 fldAutoMapAreaNo;
extern s32 fldNpcModelFreeIdx;
extern dds3Basic_t *fldNpcFreeCount;
extern s32 fldNpcCamTarget;
extern void mdlUnRegist(s32 kind, s32 slot);
extern void dds3DeleteUnitObjectBasic(dds3Basic_t *object);
extern void *frwGetSmallFontFRQ(s32 x, s32 y, s32 mode, u32 rgba, const char *text, s32 flag);
extern void *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 mode, u32 rgba, const char *text, s32 flag);
extern void frDrawFont(void *request);
extern void frDrawFontEx(void *request, s32 mode);
extern void frDestroyFontRequestAfter(void *request);
extern void *slKanjiDraw_handle;
extern s32 slDDS3To31Flg;
extern s32 sprintf(char *dst, const char *format, ...);
extern char D_00532860[];
extern s8 D_00569743[];
extern s32 version_convert_ver;
extern u8 D_00316B00[];
extern u32 D_0054F06C;
extern u32 D_0054F070;
extern void dds3AdminiCallSequence(s32 kind, s32 *argument, s32 mode, s32 flag);

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
extern char D_00532A88[];
extern void fldDoor_Warp(const char *entry);
extern void sdfMcClearSlotFlags(s32 request, s32 slot);
extern s32 slChooseData_getCardInfo_B(void);
extern s32 slTopMenuSel[];
extern s32 fldGetBgmTrack(s32 code);
extern void p2sdSetBgmTrack(s32 track);
extern void dds3EffectReset(void *effect);
extern void dds3EffectSetPosition(void *effect, const sdfVector4_t *position);
extern void *D_0054EEDC;
extern sdfTexHandle_t *D_0054EF98;
extern sdfTexHandle_t *D_0054EF9C;
extern sdfTexHandle_t *D_0054EFA0;
extern sdfTexHandle_t *D_0054EFA4;

extern s32 gFldBgmBit;
extern s16 gFldAreaBgm[];
extern s32 evtBitCheck(s32 bit);
extern void fldBgmPlay(void);
extern void dds3EffectDelete(void *effect);
extern s32 fldTakaraSlotCount;
extern s32 fldNpcLoadIdx;
extern f32 slNextPageDraw_fc;
extern f32 sdfSin(f32 radians);
extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 color, sdfTexHandle_t *texture);
extern sdfTexHandle_t *slProcPtr;
extern s32 slErrorWaitMc(void);
extern void *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(void *tag);
extern void sdfDrawTagLink(void *tag, void *packet);
extern void sdfBillBoardMake1(void *packet, const sdfVector4_t *position, u32 color,
                              const void *uv, const void *vtx);
extern sdfGrOt_t sdfGrOtTable[85];
extern s32 evtGetAgeOfMoon(void);
extern s32 dds3GetRandIntA(s32 low, s32 high);
extern s32 D_00316EC8[];
extern char D_00532AC8[];
extern char D_00532980[];
extern void resetCamTgtPlayer(void);
extern void fldMakeFilePath(char *out, s32 field, s32 area);
extern char D_00532790[];
extern char D_005327A0[];
extern s32 sdfFileOpen(const char *path);
extern void sdfFileRead(s32 fd, void *buffer, s32 size);
extern void sdfFileClose(s32 fd);

extern s32 gSlSelCnt;
extern s32 slSaveProc_CheckCard(void);
extern u16 gFldSnowLife;
extern s32 dds3GetRandA(s32 kind);

extern s32 fldSeSmgId;
extern s32 DNG_MAX[];
extern s32 fldEveHit_ChkDir(dds3Basic_t *unit, dds3Basic_t *hit);
extern void frEnableMode(s32 mode);
extern void frDisableMode(s32 mode);
extern void *frSetFontString(const char *text, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void frSetFontSpace(void *request, s32 space);
extern void frSetDrawPosition(void *request, s32 x, s32 y);
extern void frSetFontColor(void *request, u32 rgba);
extern void func_001C7E38(void *request, s32 a1, s32 a2);
extern void *slKanjiDrawH_handle;
extern void evtDisableMoonDraw(void);
extern void evtDisableMoonDraw2(void);
extern void evtEnableMoonDraw2(void);

typedef struct
{
    s32 type;
    s16 id;
    s16 num;
    s32 unk08;
    s32 unk0C;
} fldTakaraEntry_t;

extern fldTakaraEntry_t fldTakaraTable[];
extern char D_00532A58[];

void fldSe_Init(void) {
    fldSe3D_t *entry = fldSe3DTable;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flags = 0;
        entry->radius = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    gFldSe3DNum = 0;
}

void fldAddSe3D(s32 soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ) {
    s32 i = gFldSe3DNum;

    fldSe3DTable[i].flags = 0;
    fldSe3DTable[i].soundId = soundId;
    fldSe3DTable[i].radius = radius;
    fldSe3DTable[i].sourceX = sourceX;
    fldSe3DTable[i].sourceY = sourceY;
    fldSe3DTable[i].sourceZ = sourceZ;
    gFldSe3DNum = i + 1;
}

void fldClearSe3D(void) {
    s32 i;

    for (i = 0; i < gFldSe3DNum; i++) {
        s32 flags = fldSe3DTable[i].flags;

        if (flags & 1) {
            fldSe3DTable[i].flags = flags & ~1;
            p2sdStopSe(gFldBgmTop[fldGlobalBase.area].smgId + fldSe3DTable[i].soundId);
        }
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldPlaySe3D);

INCLUDE_RODATA(const s32, "code_00144798", D_00532110);

INCLUDE_ASM(const s32, "code_00144798", fldGetBgmTrack);

INCLUDE_ASM(const s32, "code_00144798", fldBgmPlay);

void fldPlayFieldBgm(void) {
    if (D_002FC020[0] == 0x80) {
        p2sdPlayBGM(fldBgmTrackTable[0] + 1);
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldBgmContinue);

void fldPauseBgm(void) {
    p2sdSendStopCmd(fldSeLastBgm);
    fldSeLastBgm = 0;
}

void fldBgmFadeOut(void) {
    p2sdStopBGM(fldSeLastBgm);
    if (fldGlobalBase.sequenceCode >= 0x80) {
        fldGlobalBase.sequenceCode = 1;
    }
    fldSeLastBgm = 0;
}

s32 fldBgmTransChk(s32 id1, s32 id2) {
    s8 first = gFldBgmTop[fldGb[0]].trans[id1];
    s8 second = gFldBgmTop[fldGb[0]].trans[id2];

    if (first != 0 && second != 0) {
        return 1;
    }
    return 0;
}

void fldBgmTrans(void) {
    s32 track = fldGetBgmTrack(fldGlobalBase.sequenceCode);

    if (track == -1) {
        if (fldGlobalBase.area >= 50) {
            return;
        }
        track = gFldBgmTop[fldGlobalBase.area].smgId + fldGlobalBase.sequenceCode;
    }
    p2sdSetBgmTrack(track);
}

void fldBgmStopAll(void) {
    p2sdStopAllBgm();
}

void fldBgmStop(void) {
    p2sdStopSe(fldSeLastBgm);
    D_002FC020[0] = 0;
}

INCLUDE_ASM(const s32, "code_00144798", fldSePlay);

void fldSeStop(s32 soundId) {
    s32 id;

    if (fldGlobalBase.area >= 50) {
        return;
    }
    if (soundId == 0x100) {
        p2sdStopSe(0x660060);
        return;
    }
    if (soundId == 0x101) {
        p2sdStopSe(0x660061);
        return;
    }
    if (soundId == 0x102) {
        p2sdStopSe(0x660064);
        return;
    }
    id = fldSeSmgId + soundId;
    if (fldGlobalBase.area == 20) {
        s32 floor = fldGlobalBase.floor;

        if (floor == 5 || floor == 8 || floor == 13 || floor == 14 || floor == 15 ||
            floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
            id = DNG_MAX[0] + soundId;
        }
    }
    p2sdStopSe(id);
}

void fldSeVol(s32 soundId, s32 volume) {
    s32 id;

    if (fldGlobalBase.area >= 50) {
        return;
    }
    id = fldSeSmgId + soundId;
    if (fldGlobalBase.area == 20) {
        s32 floor = fldGlobalBase.floor;

        if (floor == 5 || floor == 8 || floor == 13 || floor == 14 || floor == 15 ||
            floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
            id = DNG_MAX[0] + soundId;
        }
    }
    p2sdPlaySE(id, volume, 0x3F);
}

void fld_1457F8(s32 mode) {
    switch (mode) {
    case -1:
        if (D_0054EE3C != mode) {
            p2sdStopSe(D_0054EE3C);
            D_0054EE3C = mode;
        }
        break;
    case 0:
        mode = -1;
        if (D_0054EE3C != mode) {
            p2sdStopBGM(D_0054EE3C);
            D_0054EE3C = mode;
        }
        break;
    case 1:
        mode = fldBgmTrackTable[0] + 1;
        p2sdPlayBGM(mode);
        D_0054EE3C = mode;
        break;
    case 2:
        mode = D_0030A2D8[0] + 1;
        p2sdPlayBGM(mode);
        D_0054EE3C = mode;
        break;
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldBgmLoad);

s32 fldWaitBgmReload(void) {
    switch (fldBgmReloadStep) {
    case 0:
        if (p2sdLoadCheckMidi(D_0054EE40) == 0) {
            p2sdLoadSMGFile(D_0054EE40);
            fldBgmReloadStep = fldBgmReloadStep + 1;
        } else {
            fldBgmReloadStep = fldBgmReloadStep + 2;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(D_0054EE40) == 1) {
            fldBgmReloadStep = fldBgmReloadStep + 1;
        }
        break;
    default:
        fldBgmReloadStep = -1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00144798", fldBgmBitChk);

void fldMotionSePlay(s32 seId, s32 subId) {
    s32 cmd = 0x9010000;

    if (seId != 0) {
        cmd = 0x9000000 + ((seId << 16) + subId);
    }
    p2sdPlaySE(cmd, 0x7F, 0x3F);
}

void fldMotionSeStop(s32 seId, s32 subId) {
    s32 cmd = 0x9010000;

    if (seId != 0) {
        cmd = 0x9000000 + ((seId << 16) + subId);
    }
    p2sdStopSe(cmd);
}

s32 fldWaitBgmLoad(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = fldBgmLoadStep + 2;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 1) {
            fldBgmLoadStep = fldBgmLoadStep + 1;
        }
        break;
    default:
        fldBgmLoadStep = -1;
        return 1;
    }
    return 0;
}

s32 fldBgmIsLoading(void) {
    return fldBgmLoadStep != -1;
}

void fldStartBgmLoad(s32 kind) {
    if (fldBgmLoadStep != -1) {
        return;
    }
    switch (kind) {
    case 0:
        fldBgmSmgAddr = 0x310000;
        break;
    case 1:
        fldBgmSmgAddr = 0x390000;
        break;
    case 2:
        return;
    case 3:
    case 4:
        fldBgmSmgAddr = 0x20000;
        break;
    case 5:
        fldBgmSmgAddr = 0x300000;
        break;
    }
    fldBgmLoadStep = 0;
    fldWaitBgmLoad();
}

INCLUDE_ASM(const s32, "code_00144798", fld_145DC0);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapFontInit);

INCLUDE_ASM(const s32, "code_00144798", fldAmModelLoadFile);

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

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapModelLoad);

void fldAutoMapFileFree(void) {
    if (fldAutoMapBuf != 0) {
        sdfMemFreeDirect(fldAutoMapBuf);
    }
    fldAutoMapBuf = 0;
    fldAutoMapHeader = 0;
    fldAutoMapData = 0;
}

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapLoad);

void fldAutoMapDrawMdl1(sdfModelRootNode_t *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    sdfModelNode_t *handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = emitter->nodeList->items[0];
    VU0_LOAD_VF10_BARRIER(pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&emitter->tm.m[12]);
    sdfModelMakeLocalTm(handle);
    sdfModelEvalTm(emitter);
    sdfModelLinkOt(&D_002F6A48, emitter);
}

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDrawIconOfsR);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDrawIconOfsRF);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDrawBgOfsRF);

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

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDrawOneArea);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDrawWindow);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapSeqStart);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapFree);

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

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapCalc);

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

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapOpenBlock);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapOpenBlock2);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapCloseBlock2);

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

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapProc);

INCLUDE_RODATA(const s32, "code_00144798", D_00532230);

INCLUDE_RODATA(const s32, "code_00144798", D_00532240);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapDraw);

INCLUDE_ASM(const s32, "code_00144798", fldAutoMapClear);

INCLUDE_ASM(const s32, "code_00144798", fldDrawAreaBlockFont);

INCLUDE_ASM(const s32, "code_00144798", fldInitPacLoader);

INCLUDE_ASM(const s32, "code_00144798", fldInitBlurEffect);

INCLUDE_ASM(const s32, "code_00144798", fldFreeBlurEffect);

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

INCLUDE_ASM(const s32, "code_00144798", fldNpcLightSet);

INCLUDE_RODATA(const s32, "code_00144798", D_00532338);

INCLUDE_RODATA(const s32, "code_00144798", D_00532348);

INCLUDE_ASM(const s32, "code_00144798", fldNpcLoad);

INCLUDE_RODATA(const s32, "code_00144798", D_005326F0);

INCLUDE_RODATA(const s32, "code_00144798", D_00532700);

INCLUDE_ASM(const s32, "code_00144798", fldItemBoxAdd);

void fldFindTakaraSlot(s32 id) {
    s32 i;

    for (i = 0; i < fldTakaraCount; i++) {
        if (fldTakaraSlotTable[i].id == id && fldTakaraSlotTable[i].activationState == 0) {
            fldTakaraSlotTable[i].activationRequested = 1;
        }
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldFreeModelTable);

INCLUDE_ASM(const s32, "code_00144798", fldItemBoxCalcTask);

void fldTDmgStart(f32 x, f32 y, f32 z) {
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    position.x = x;
    position.y = y;
    position.z = z;
    position.w = 1.0f;
    dds3EffectReset(D_0054EEDC);
    dds3EffectSetPosition(D_0054EEDC, &position);
}

INCLUDE_ASM(const s32, "code_00144798", fldDmgStart);

INCLUDE_ASM(const s32, "code_00144798", fldSibukiStart);

INCLUDE_ASM(const s32, "code_00144798", fldDrawEATexQuad);

INCLUDE_ASM(const s32, "code_00144798", fldDrawAddressPanel);

INCLUDE_ASM(const s32, "code_00144798", fldDrawInputPanel);

void fldDrawInputCursor(s32 x, s32 y) {
    if (fldGb[0] < 200) {
        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x, y, 0x14, 0x15, 0x2E, 0x47, 0x14, 0x15, 0x80808080, fldUiTexture);
        rsetAlpha(0);
    }
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532720);

INCLUDE_ASM(const s32, "code_00144798", fldEADraw);

INCLUDE_ASM(const s32, "code_00144798", fldVLDraw);

INCLUDE_RODATA(const s32, "code_00144798", D_00532770);

INCLUDE_ASM(const s32, "code_00144798", fldNpcModelStart);

void fldDeleteNpcModel(s32 index) {
    if (fldNpcModelFreeIdx == index && fldNpcFreeCount != NULL) {
        dds3DeleteUnitObjectBasic(fldNpcFreeCount);
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcCamTarget = 0;
        fldNpcModelFreeIdx = -1;
    }
}

void fldFreeNpcModel(void) {
    if (fldNpcModelFreeIdx != -1 && fldNpcFreeCount != NULL) {
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcModelFreeIdx = -1;
        fldNpcCamTarget = 0;
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldNpcCalc);

INCLUDE_ASM(const s32, "code_00144798", fldNpcDraw);

void fldNpcHojiRefresh(void) {
    s32 i;

    for (i = 0; i < fldNpcModelTable; i++) {
        s32 room = fldModelSlotTable[i].room;
        if (room != 0 && fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1, room) != 0) {
            if (fldModelSlotTable[i].object != 0) {
                dds3ObjectBaseOnFlag(fldModelSlotTable[i].object, 1);
            }
        }
    }
}

void fldNpcLightLoad(s32 index) {
    char path[0x40];
    char directory[0x20];
    s32 fd;

    if (index < 100) {
        fldMakeFilePath(directory, index, 1);
        if (index == 0x18 && evtBitCheck(0x4F) != 0) {
            sprintf(path, D_00532790, directory, 0x18);
        } else {
            sprintf(path, D_005327A0, directory, index);
        }
        fd = sdfFileOpen(path);
        sdfFileRead(fd, D_003129D0, 0x100);
        sdfFileClose(fd);
    }
}

void fldGetEAEntry(const fldNpcPalette_t *src) {
    memcpy(D_003129D0, src, sizeof(*src));
}

s32 fldTitleChk(void) {
    return dds3SearchProcessName(D_005327B0) != 0;
}

INCLUDE_ASM(const s32, "code_00144798", fldTitle_Process);

void fldTitle_Shutdown(void) {
    if (D_0054EF98 != NULL) {
        sdfTexFreeDelayed(D_0054EF98);
        D_0054EF98 = NULL;
    }
    if (D_0054EF9C != NULL) {
        sdfTexFreeDelayed(D_0054EF9C);
        D_0054EF9C = NULL;
    }
    if (D_0054EFA0 != NULL) {
        sdfTexFreeDelayed(D_0054EFA0);
        D_0054EFA0 = NULL;
    }
    if (D_0054EFA4 != NULL) {
        sdfTexFreeDelayed(D_0054EFA4);
        D_0054EFA4 = NULL;
    }
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532790);

INCLUDE_RODATA(const s32, "code_00144798", D_005327A0);

INCLUDE_RODATA(const s32, "code_00144798", D_005327B0);

INCLUDE_ASM(const s32, "code_00144798", fldTitleStart);

s32 fldTitleMiniChk(void) {
    return dds3SearchProcessName(D_00532820) != 0;
}

INCLUDE_ASM(const s32, "code_00144798", fldTitleMini_Process);

void fldTitleMini_Shutdown(void) {
    if (fldTitleMiniTex != 0) {
        sdfTexFreeDelayed(fldTitleMiniTex);
        fldTitleMiniTex = 0;
    }
    if (fldTitleMiniWork != 0) {
        sdfTexFreeDelayed(fldTitleMiniWork);
        fldTitleMiniWork = 0;
    }
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532820);

INCLUDE_ASM(const s32, "code_00144798", fldTitleMiniStart);

void fldSetSnowActive(void) {
    if (fldSnowActive == 0) {
        fldSnowActive = 1;
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldSnowInit);

void fldFreeSnowSeed(void) {
    if (gFldSeedBuff != NULL) {
        sdfMemUnlock(fldSnowSeedBuf);
        sdfMemFreeDelayedDirect(fldSnowSeedBuf);
        fldSnowSeedBuf = NULL;
        gFldSeedBuff = NULL;
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldSnowSpriteTex);

void fldSnowSprite3D(const sdfVector4_t *work) {
    void *tag = sdfGrAllocPacketBuff(0x20);
    void *packet;
    sdfGrOt_t *ot;

    sdfDrawTagInit(tag);
    packet = sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, work, *(u32 *)((u8 *)work + 0x40), (u8 *)work + 0x10,
                      (u8 *)work + 0x20);
    sdfDrawTagLink(tag, packet);
    ot = &sdfGrOtTable[44];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void fldSnowSeedInit(void) {
    s32 i;

    if (gFldSeedBuff != NULL) {
        for (i = 0; i < 700; i++) {
            gFldSeedBuff[i].flg = 0;
        }
    }
}

void fldInitSnowSeed(f32 x, f32 y, f32 z) {
    s32 i;

    if (gFldSeedBuff == NULL) {
        return;
    }
    for (i = 0; i < 700; i++) {
        if (gFldSeedBuff[i].flg == 0) {
            gFldSeedBuff[i].x = x;
            gFldSeedBuff[i].y = y;
            gFldSeedBuff[i].z = z;
            gFldSeedBuff[i].flg = 1;
            gFldSeedBuff[i].life = gFldSnowLife;
            gFldSeedBuff[i].alp = 0x80;
            gFldSeedBuff[i].spdx = dds3GetRandA(0) & 0x1F;
            gFldSeedBuff[i].spdy = dds3GetRandA(0) & 0xF;
            return;
        }
    }
}

void func_00151018(s32 index) {
    if (gFldSeedBuff != NULL) {
        gFldSeedBuff[index].flg = 0;
    }
}

INCLUDE_ASM(const s32, "code_00144798", fldSnowSeedReset);

INCLUDE_ASM(const s32, "code_00144798", fldSnowTask);

void mcMakeFileName(char *dst, s32 number) {
    if (slDDS3To31Flg != 0) {
        sprintf(dst, D_00532860, 0xFEDA, number);
    } else {
        sprintf(dst, D_00532860, 0x51AF, number);
    }
}

void slVersionConvert_Step1(void) {
    version_convert_ver = *(D_00569743 + sdfMcGetSlotIndex(D_005516BC) * 52);
}

/* Save revisions have different persistent block extents. Keeping this
 * earlier provider in C also exposes the native same-TU call visibility to
 * slLoadProc_openFile (DDS idioms: earlier definitions and branch-likely). */
u32 slVersionConvert_GetSize(void) {
    if (version_convert_ver == 1) {
        return 0xF990;
    }
    if (version_convert_ver == 2) {
        return 0xFA30;
    }
    return (u32)(version_convert_ver - 3) < 2 ? 0x16DC0 : 0x1A510;
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532860);

INCLUDE_ASM(const s32, "code_00144798", slVersionConvert_Step3);

u8 slGetPadTrig(s32 condition) {
    return condition != 0 && slProcShutDownFlg == 1;
}

u8 slGetPadRep(s32 loaded) {
    return loaded != 0 && slProcShutDownFlg == 1;
}

INCLUDE_ASM(const s32, "code_00144798", slCopyNonZero);

void slKanjiDraw(s32 x, s32 y, u32 rgba, const char *text) {
    slKanjiDraw_handle = frwGetSmallFontFRQ(x << 4, y << 4, 0, rgba, text, 0);
    frDrawFont(slKanjiDraw_handle);
    frDestroyFontRequestAfter(slKanjiDraw_handle);
}

void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *text) {
    frEnableMode(1);
    slKanjiDrawH_handle = frSetFontString(text, 0, 0, 0, 0, 0);
    frDisableMode(1);
    frSetFontSpace(slKanjiDrawH_handle, 3);
    frSetDrawPosition(slKanjiDrawH_handle, x << 4, y << 4);
    frSetFontColor(slKanjiDrawH_handle, rgba);
    func_001C7E38(slKanjiDrawH_handle, 0, 0x4A);
    frDestroyFontRequestAfter(slKanjiDrawH_handle);
    frSetFontDrawOT(0x48);
}

void slKanjiDrawN(s32 x, s32 y, u32 rgba, const char *text) {
    void *request = frwGetSmallNumFontFRQ(x << 4, y << 4, 0, rgba, text, 0);

    frDrawFontEx(request, 1);
    frDestroyFontRequestAfter(request);
}

void slNextPageDraw(void) {
    f32 phase;

    rsetOtOfs(0x4A);
    rsetAlpha(0);
    phase = slNextPageDraw_fc + 0.4f;
    slNextPageDraw_fc = phase;
    DrwSprite2DF(448.0f, sdfSin(phase) + 308.0f, 25.0f, 18.0f, 0xDB, 0x2D, 0x19, 0x12,
                 0x80808080, slProcPtr);
}

INCLUDE_ASM(const s32, "code_00144798", slMcAccessDraw);

s32 slChkWindowOpenDone(void) {
    if (gSlWindowCnt < 7) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", slDrawWindow);

INCLUDE_ASM(const s32, "code_00144798", slDrawError);

void slSetInfo(u32 state) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = state;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

INCLUDE_ASM(const s32, "code_00144798", slDrawInfo);

s32 slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    sdfMcSetSlotIndex(D_005516BC, slChooseSaveIndex);
    ctx = D_005516BC;
    slot = sdfMcGetSlotIndex(ctx);
    if ((sdfMcGetSlotFlags(ctx, slot) & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slChooseData_getCardInfoNext();
    }
    path[0] = 0x2F;
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slHeadLoadProc_openFile;
}

s32 slHeadLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slLoadFileFd = sdfMemAllocDirect(0x34);
        slLoadFileHandle = sdfMemLock(slLoadFileFd);
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, 0x34);
        return (s32)slHeadLoadProc_readFile;
    }
    return slNoCard();
}

s32 slHeadLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slHeadLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

INCLUDE_ASM(const s32, "code_00144798", slHeadLoadProc_closeFile);

s32 slExitWait(void *callback) {
    dds3FadeOut(0, 0, 0, 15);
    slExitWaitNextFunc = callback;
    slExitWaitCnt = 20;
    return (s32)slExitWaitProc;
}

s32 slEndingTop(void) {
    gslMode = 0;
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 4;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slTop, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 9;
    return slQueryYesNo(&slSaveData, &slFileSel, 1);
}

s32 slEndingExit(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 8;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slExitToTitle, &slEndingTop, 1);
}

s32 slBootTop(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return (s32)slBootProc_CheckCard;
}

s32 slBootRetry(void) {
    sdfMcRegetInfo(0);
    gslCheckCardWait = 30;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slBootProc_CheckCardR;
}

s32 slBootError(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 5;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slBootError2(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 6;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slNoCard(void) {
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slSetInfo(0);
    gslErrorCode = 1;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(D_005516BC);
    return (s32)slErrorWaitMc;
}

s32 slTop(void) {
    gSlSelCnt = 0;
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slTopMenuSel[0] = 0;
    slTopMenuSel[1] = 0;
    slTopMenuSel[2] = 0;
    slTopMenuSel[3] = 0;
    slTopMenuSel[4] = 0;
    sdfMcRegetInfo(0);
    gslCheckCardWait = 15;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slSaveProc_CheckCard;
}

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

s32 slExitToTitleSub(void) {
    s32 argument = 0;

    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slExitToTitle(void) {
    return slExitToTitleSub();
}

s32 slExitToTitle2(void) {
    s32 argument = 2;

    gslExitType = 1;
    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slCardChk(void) {
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slSetInfo(1);
    gslErrorCode = 0;
    gslDrawFlagFile = 0;
    return slTop();
}

INCLUDE_ASM(const s32, "code_00144798", slFileSel);

s32 slCancel(void) {
    if (gslMode == 1 && dds3SearchProcessName(D_005328C0) == NULL) {
        return slExitToTitle2();
    }
    slSetInfo(0);
    return slExitWait(&slExit);
}

s32 slSaveProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slTop();
}

s32 slLoadProc1st(void) {
    return slTop();
}

s32 slEndingProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slEndingTop();
}

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

INCLUDE_ASM(const s32, "code_00144798", slSaveProc_CheckCard);

s32 sl1Ken(void) {
    s32 i = 0;
    s32 count = 0;

    do {
        u32 flags = sdfMcGetSlotFlags(D_005516BC, i);

        i++;
        if (flags & SL_SLOT_FLAG_CAN_READ_SAVE) {
            if (flags & SL_SLOT_FLAG_DIRECTORY_READY) {
                if (flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                    count++;
                }
            }
        }
    } while (i < 5);
    return count;
}

s32 slSizeChk(void) {
    return sdfMcGetFreeBytes(D_005516BC) > 0x293FF;
}

s32 slBootProc_CheckCard(void) {
    s32 result = sdfMcFuncGetInfo1();
    if (result == 0) {
        return result;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(D_005516BC);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(D_005516BC)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return (s32)slBootRetry();
    default:
        return 0;
    }
}

s32 slBootProc_CheckCardR(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(D_005516BC);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(D_005516BC)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return slChooseData_B();
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "code_00144798", slErrorWaitMc);

s32 slErrorWaitToFileMc(void) {
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() != 0) {
        if (gslErrorKeyFlag == 1) {
            gslErrorCode = 0;
            return slFileSel();
        }
        if (sdfMcGetFileInfoReady(D_005516BC) != 1) {
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            return slTop();
        }
        sdfMcRegetInfo(D_005516BC);
    }
    return 0;
}

s32 slErrorWait(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        return slTop();
    }
    return 0;
}

s32 slExitWaitProc(void) {
    s32 remaining = slExitWaitCnt - 1;
    slExitWaitCnt = remaining;
    if (remaining <= 0) {
        if (slHitAnyKeyNextFunc == -1) {
            return -1;
        }
        return ((s32 (*)())slExitWaitNextFunc)();
    }
    return 0;
}

s32 slHitAnyKeyNoChk(u32 callback) {
    slHitAnyKeyNextFunc = callback;
    gslErrorKeyFlag = 0;
    return (s32)slHitAnyKeyNoChkProc;
}

s32 slHitAnyKeyNoChkProc(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        slSetInfo(0);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        if (slHitAnyKeyNextFunc == -1) {
            return slExitWait((void *)-1);
        }
        return ((s32 (*)())slHitAnyKeyNextFunc)();
    }
    return 0;
}

s32 slHitAnyKey(u32 callback) {
    slHitAnyKeyNextFunc = callback;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(D_005516BC);
    return (s32)slHitAnyKeyProc;
}

INCLUDE_ASM(const s32, "code_00144798", slHitAnyKeyProc);

s32 slChooseData_getCardInfo(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(D_005516BC, (const char *)buf);
    return (s32)slChooseData_getCardInfo2;
}

s32 slChooseData_getCardInfo2(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(D_005516BC, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3;
    }
    if (t == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

INCLUDE_ASM(const s32, "code_00144798", slChooseData_getCardInfoNext);

INCLUDE_ASM(const s32, "code_00144798", slChooseData);

s32 slChooseData_getCardInfo_B(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(D_005516BC, (const char *)buf);
    return (s32)slChooseData_getCardInfo2_B;
}

s32 slChooseData_getCardInfo2_B(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(D_005516BC, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3_B;
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfoNext_B(void) {
    s32 slot = slChooseSaveIndex;
    s32 buttons = sdfMcGetSlotFlags(D_005516BC, slot);

    slot++;
    if (buttons & SL_SLOT_FLAG_CAN_READ_SAVE) {
        if (buttons & SL_SLOT_FLAG_DIRECTORY_READY) {
            if (buttons & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                slSetInfo(0);
                gslDrawFlagFile = 0;
                gslErrorCode = 0;
                return slExitToTitle();
            }
        }
    }
    slChooseSaveIndex = slot;
    if (slot == 5) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError2();
    }
    return slChooseData_getCardInfo_B();
}

s32 slChooseData_B(void) {
    s32 i = 0;
    s32 *selection;

    slSetInfo(1);
    gslDrawFlagFile = 0;
    slChooseSaveIndex = 0;
    sdfMcSetCardStat(D_005516BC);
    selection = slTopMenuSel;
    do {
        *selection = 0;
        selection++;
        sdfMcClearSlotFlags(D_005516BC, i);
        i++;
    } while (i < 5);
    return slChooseData_getCardInfo_B();
}

s32 slSaveData(void) {
    u8 name[0x50];
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);
    sdfMcGetSlotFlags(entry, slot);
    slSetInfo(2);
    sdfMcSetCardStat(entry);
    if (sdfMcGetSlotFlags(entry, slot) & SL_SLOT_FLAG_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcMakeFileName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return (s32)slSaveProc_Write_mkdir;
}

s32 slSaveData_chdir(void) {
    u8 buf[0x50];
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slot);
    sdfMcFuncChdir(entry, (const char *)buf);
    return (s32)slSaveProc_Write_chdir;
}

s32 slSaveProc_Write_mkdir(void) {
    slMcResult_t status = sdfMcFuncMkdir1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return (s32)slErrorWait;
}

s32 slSaveProc_Write_chdir(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_Write_deleteIconSys(void) {
    slMcResult_t t = sdfMcFuncDelete1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_writeFile();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_doNothing(void) {
    sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_CAN_READ_SAVE);
    sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
    slSetInfo(4);
    return slHitAnyKey((u32)slFileSel);
}

s32 slSaveWriteEnd_iconSys(void) {
    slKillLoadProcess();
    return (s32)slSaveProc_doNothing;
}

s32 slSaveWriteEnd_baseIcon(void) {
    D_00316B00[0xE2] = -0x7E;
    D_00316B00[0xE3] = slFileSelError + 0x50;
    D_00316B00[0xE4] = 0;
    D_00316B00[0xE5] = 0;
    return slSaveWriteFile(D_005328E0, &D_0054F06C, &D_0054F070, slSaveWriteEnd_iconSys, 0);
}

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(D_005328F0, &D_00551D48, &D_00551D4C,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

INCLUDE_ASM(const s32, "code_00144798", slSaveData_writeFile);

s32 slSaveData_deleteIconSys(void) {
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);
    u32 flags = sdfMcGetSlotFlags(entry, slot);
    if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
        return slSaveData_writeFile();
    }
    sdfMcFuncDelete(entry, D_005328E0);
    return (s32)slSaveProc_Write_deleteIconSys;
}

s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, void *continuation, u32 *pendingRequestStatus) {
    slSaveWriteFile_buff = dataAddressSlot;
    slSaveWriteFile_size = dataSizeSlot;
    slSaveWriteFile_endFunc = continuation;
    slSaveWriteFile_dataWaitPtr = pendingRequestStatus;
    sdfMcFuncOpen(D_005516BC, fileName, 0x203);
    return (s32)slSaveProc_openFile;
}

s32 slSaveProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return (s32)slSaveProc_waitDataReady;
        }
        sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return (s32)slSaveProc_writeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slSaveProc_closeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_waitDataReady(void) {
    if (baseIconFileHandle != 0) {
        return 0;
    }
    sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
    return (s32)slSaveProc_writeFile;
}

s32 slProc_Format(void) {
    slMcResult_t status = sdfMcFuncFormat1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        gslErrorCode = 6;
        return (s32)slErrorWait;
    }
    slSetInfo(6);
    return slHitAnyKey((u32)slCardChk);
}

s32 slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(D_005516BC);
    return (s32)slProc_Format;
}

s32 slFormatCancel(void) {
    if (gSlEndingMode != 0) {
        slQueryYesNoMsg = 7;
        return slQueryYesNo(slEndingExit, slTop, 1);
    }
    slQueryYesNoMsg = 7;
    return slQueryYesNo(slExit, slTop, 1);
}

s32 slLoadData(void) {
    char path[0x50];
    u32 ctx = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(ctx);
    u32 attr = sdfMcGetSlotFlags(ctx, slot);
    u32 len;

    slSetInfo(3);
    if ((attr & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slFileSel();
    }
    slVersionConvert_Step1();
    path[0] = '/';
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slLoadProc_openFile;
}

s32 slLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);
    u32 size;

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    size = slVersionConvert_GetSize();
    slLoadFileFd = sdfMemAllocDirect(size);
    slLoadFileHandle = sdfMemLock(slLoadFileFd);
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, size);
        return (s32)slLoadProc_readFile;
    }
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slVersionConvert_Step3();
        sdfMemFreeDirect(slLoadFileFd);
        D_005516C1 = 1;
        slKillLoadProcess();
        if (dds3ConfigChanged() == 0) {
            dds3ConfigSaveBackup();
        } else {
            dds3ConfigRestoreBackup();
        }
        slSetInfo(13);
        return slHitAnyKeyNoChk(-1);
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

INCLUDE_ASM(const s32, "code_00144798", slFileSelCalc);

INCLUDE_RODATA(const s32, "code_00144798", D_005328C0);

INCLUDE_RODATA(const s32, "code_00144798", D_005328D0);

INCLUDE_RODATA(const s32, "code_00144798", D_005328E0);

INCLUDE_RODATA(const s32, "code_00144798", D_005328F0);

INCLUDE_ASM(const s32, "code_00144798", slFileSelDraw);

s32 slCalc(dds3ProcessID_t *task) {
    s32 next;
    u32 job;
    s32 (*cur)();
    gslAnimCnt++;
    slDraw((s32)task);
    next = proc_func_ans(task);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = (s32 (*)())next;
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd((fldFileRequest_t *)job) != 0) {
        baseIconFileHandle = 0;
        D_00551D44 = fileGetMemHandle((fldFileRequest_t *)(u32)job);
        D_00551D48 = (u32)fileGetLoadPtr((fldFileRequest_t *)(u32)job);
        D_00551D4C = fileGetLoadSize((fldFileRequest_t *)(u32)job);
        fileHandleFree((fldFileRequest_t *)(u32)job);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00144798", slDraw);

INCLUDE_ASM(const s32, "code_00144798", fmProcTrigSequence);

INCLUDE_ASM(const s32, "code_00144798", slShutDownProcess);

u32 fmShutDown2(void) {
    return 1;
}

s32 fmProcessChk(void) {
    return dds3SearchProcessName(D_00532948) != 0;
}

u32 slGetExitType(void) {
    return gslExitType;
}

INCLUDE_ASM(const s32, "code_00144798", slQueryYesNoProc);

s32 slQueryYesNo(void *start, void *finish, s32 mode) {
    slQueryYesFunc = start;
    slQueryNoFunc = finish;
    slQueryYesNoCursor = mode;
    slQueryYesNoKeyFlag = 0;
    if (slQueryYesNoMsg != 4 && slQueryYesNoMsg != 8) {
        sdfMcRegetInfo(D_005516BC);
    }
    return (s32)slQueryYesNoProc;
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532948);

INCLUDE_ASM(const s32, "code_00144798", slQueryYesNoDraw);

u32 slGetExitMode(void) {
    return gSlExitMode;
}

void slSetTmnlID(s16 x, s16 y) {
    dds3GlobalWork_t *state = DDS3_GBWK;

    state->previewX = x;
    state->previewY = y;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_ACT_WIN_DSP);

INCLUDE_ASM(const s32, "code_00144798", fldCommand_ACT_WIN_DSP2);

s32 fldCommand_ACT_WIN_DSP3(void) {
    s32 result;

    fldInputPanelSetMessage(scrGetIntPara(0));
    fldInputPanelSetDraw(1);
    result = fldInputPanelCheckKey();
    if (result == -1) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    if (result == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_DIR_HIT_CNTR);

s32 fldCommand_PAD_DISABLE(void) {
    dds3Basic_t *object;

    fldReleaseUnitPad();
    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    fldPlayerMotReset();
    return 1;
}

s32 fldCommand_PAD_ENABLE(void) {
    dds3Basic_t *object;

    object = dds3GetActiveUnitSceneBasic((dds3Basic_t *)dds3GetActiveScene());
    if (object == 0) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(object, 0);
    fldPlayerMotReset();
    fldRegistUnitPad();
    return 1;
}

u32 fldCommand_ENCOUNT_ON(void) {
    fldEncountSetFlag(0x10);
    return 1;
}

u32 fldCommand_ENCOUNT_OFF(void) {
    fldEncountClearFlag(0x10);
    return 1;
}

s32 fldCommand_SET_FIX_CAMERA(void) {
    dds3Basic_t *scene = dds3GetControlScene();
    dds3Basic_t *basic = dds3SearchSceneBasic(scene, scrGetIntPara(0), 4);

    if (basic == NULL) {
        printf(D_00532980);
        return 1;
    }
    fldGlobalBase.mode = 1;
    fldGlobalBase.campos.x = basic->poseset->pos.x;
    fldGlobalBase.campos.y = basic->poseset->pos.y;
    fldGlobalBase.campos.z = basic->poseset->pos.z;
    resetCamTgtPlayer();
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_SET_CHASE_CAMERA);

s32 func_00158548(void) {
    fldCamMode[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_CAM_DIR);

s32 fldCommand_CAM_SEL_PHOKAN(void) {
    f32 pos[4];
    f32 rot[4];
    dds3Basic_t *target;
    dds3Basic_t *object;
    dds3Basic_t *world;

    if (scrGetIntPara(0) == -1) {
        target = fldGetUnitFollowCamera();
        if (target == NULL) {
            return 1;
        }
        object = dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
        if (object == NULL) {
            return 1;
        }
        dds3CameraObjectBasicGetTgtCam(object, pos, rot);
        fldCamGetPhokanParam();
        fldCamPhokanSelX[0] = pos[0];
        fldCamPhokanSelX[1] = pos[1];
        fldCamPhokanSelX[2] = pos[2];
        D_002FC9F0[0] = rot[0];
        D_002FC9F0[1] = rot[1];
        D_002FC9F0[2] = rot[2];
        fldCamPhokanState = 0;
        fldCamPHokanMode = scrGetIntPara(1);
        fldCamMode[0] = 4;
    } else {
        world = dds3GetActiveScene();
        target = dds3SearchSceneBasic(world, scrGetIntPara(0), 4);
        if (target == NULL) {
            return 1;
        }
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), target);
    }
    return 1;
}

s32 fldCommand_SET_NPC_CAMERA(void) {
    char *name;

    name = scrGetStrPara(0);
    fldGlobalBase.mode = 5;
    strcpy(fldGlobalBase.requestedSceneName, name);
    return 1;
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532980);

INCLUDE_ASM(const s32, "code_00144798", fldCommand_SET_ALLPANEL);

void fldPanelSet(void) {
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_SET_CASH_F);

s32 fldCommand_FREE_CASH_F(void) {
    if (fldGb[0] < 10) {
        return 1;
    }
    fldFreeNextFile();
    return 1;
}

s32 fldCommand_SEARCH_ID(const char *name) {
    dds3Basic_t *entry = dds3SceneBasicSearchName(dds3GetControlScene(), name);
    if (entry != 0) {
        return entry->serialID;
    }
    printf("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

s32 fldCommand_GetHojiID(char *name) {
    s32 index;
    if (strstr(name, D_0054F0A8) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (name[index] == '\0') {
            if (index < 4) return 0;
            return (name[index - 2] - '0') * 10 + (name[index - 1] - '0');
        }
    }
    return 0;
}

s32 fldCommand_SET_CAMTBL(void) {
    s32 value;

    value = scrGetIntPara(0);
    fldCamUnit[0] = value;
    return 1;
}

s32 fldCommand_FLD_KANKYO(void) {
    s32 durationMode;

    fldRunDanteGunFlag.skyTable = scrGetIntPara(0);
    durationMode = scrGetIntPara(1);
    fldSkySetTbl(fldRunDanteGunFlag.skyTable, durationMode);
    return 1;
}

s32 fldCommand_FLD_FILTER(void) {
    s32 second;
    s32 third;
    fldRunDanteGunFlag.filter = scrGetIntPara(0) & 0xFF;
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    fldSkySetFilter(fldRunDanteGunFlag.filter, second, third);
    fldSetFilterAnime(scrGetIntPara(3));
    return 1;
}

s32 fldCommand_FLD_MODEL_ON(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    dds3BasicArrayHandle_t *list;
    dds3Basic_t *node;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    mode = scrGetIntPara(3);
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            fldSetModelFlag(world, stage, room, 0);
        }
    } else {
        room = fldCommand_GetHojiID(name);
        fldSetModelFlag(world, stage, room, 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        list = dds3SceneBasicGetList(dds3GetControlScene(), 6);
        if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)list) != 0) {
            dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)list);
            do {
                node = (dds3Basic_t *)(u32)dds3GetBasicArray((dds3BasicArrayHandle_t *)list);
                if (((fldMatterWork_t *)node->workdata)->roomNumber == (u32)room) {
                    switch (mode) {
                    case 0:
                        dds3MatterObjectBasicSetAlpMode(node, 1);
                        break;
                    case 1:
                        dds3MatterObjectBasicSetAlpMode(node, 5);
                        break;
                    case 2:
                        dds3MatterObjectBasicSetAlpMode(node, 7);
                        break;
                    }
                }
            } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)list) != 0);
            dds3FreeBasicArrayHandle(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_MODEL_OFF);

s32 fldCommand_FLD_ANIME_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAnimeFlag(world, stage, i, 0);
        }
    } else {
        fldSetAnimeFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != -1) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 0);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 1);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 2);
                break;
            case 3:
                dds3ObjectBaseMatterAnimeSpeed(object, 6);
                break;
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGlobalBase.area;
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldGlobalBase.floor + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAnimeFlag(world, stage, i, 1);
        }
    } else {
        fldSetAnimeFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    if (world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1) {
        id = fldCommand_SEARCH_ID(name);
        if (id != -1) {
            object = dds3SearchSceneBasic(dds3GetControlScene(), id, 6);
            switch (scrGetIntPara(3)) {
            case 0:
                dds3ObjectBaseMatterAnimeSpeed(object, 3);
                break;
            case 1:
                dds3ObjectBaseMatterAnimeSpeed(object, 4);
                break;
            case 2:
                dds3ObjectBaseMatterAnimeSpeed(object, 5);
                break;
            }
        }
    }
    return 1;
}

s32 fldCommand_FLD_ATARI_ON(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAtariFlag(world, stage, i, 0);
        }
    } else {
        fldSetAtariFlag(world, stage, fldCommand_GetHojiID(name), 0);
    }
    return 1;
}

s32 fldCommand_FLD_ATARI_OFF(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    name = scrGetStrPara(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            fldSetAtariFlag(world, stage, i, 1);
        }
    } else {
        fldSetAtariFlag(world, stage, fldCommand_GetHojiID(name), 1);
    }
    return 1;
}

s32 fldCommand_FLD_MODEL_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetModelFlag2(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_MODEL_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetModelFlag2(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ON2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAnimeFlag2(area, floor, target, 0);
    return 1;
}

s32 fldCommand_FLD_ANIME_OFF2(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, target;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    target = scrGetIntPara(2);
    if (target == 0) return 1;
    fldSetAnimeFlag2(area, floor, target, 1);
    return 1;
}

s32 fldCommand_FLD_ANIME_ID2(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrGetIntPara(0);
    if (world == 0) {
        world = fldGb[0];
    }
    stage = scrGetIntPara(1);
    if (stage == 0) {
        stage = fldArea[0] + 1;
    }
    slot = scrGetIntPara(2);
    if (slot == 0) {
        return 1;
    }
    fldGetAnimeId2(world, stage, slot, scrGetIntPara(3));
    return 1;
}

s32 fldCommand_FLD_MAP_ON(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldAutoMapOpenBlock2(area, floor, flagNumber);
    return 1;
}

s32 fldCommand_FLD_MAP_OFF(void) {
    s32 area = scrGetIntPara(0);
    s32 floor, flagNumber;
    if (area == 0) area = fldGb[0];
    floor = scrGetIntPara(1);
    if (floor == 0) floor = fldArea[0] + 1;
    flagNumber = scrGetIntPara(2);
    if (flagNumber == 0) return 1;
    fldAutoMapCloseBlock2(area, floor, flagNumber);
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_NPC_ON);

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_NPC_OFF);

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_GIMIC_HOJI);

s32 fldCommand_FLD_CHK_BTL(void) {
    if (fldGetRestoreMode() == 1) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_ANA(void) {
    if (fldGetRestoreMode() == 3) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_SISETU(void) {
    if (fldGetRestoreMode() == 4) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 fldCommand_FLD_CHK_EVE(void) {
    if (fldGetRestoreMode() == 2) {
        scrSetIntReturnValue(1);
        return 1;
    }
    scrSetIntReturnValue(0);
    return 1;
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532A58);

s32 fldCommand_FLD_GET_TAKARA_TBL(void) {
    s32 index = fldFindEveRngIndex((u32)scrGetScriptHandle()->ProcessID);

    switch (scrGetIntPara(0)) {
    case 0:
        printf(D_00532A58, fldTakaraTable[index].type, fldTakaraTable[index].id,
               fldTakaraTable[index].num);
        scrSetIntReturnValue(fldTakaraTable[index].type);
        break;
    case 1:
        scrSetIntReturnValue(fldTakaraTable[index].id);
        break;
    case 2:
        scrSetIntReturnValue(fldTakaraTable[index].num);
        break;
    case 3:
        scrSetIntReturnValue(fldTakaraTable[index].unk08);
        break;
    case 4:
        scrSetIntReturnValue(fldTakaraTable[index].unk0C);
        break;
    }
    return 1;
}

u32 fldCommand_FLD_SET_TAKARA_OPEN(void) {
    scrProcessWork_s *script;
    s32 room;

    script = scrGetScriptHandle();
    room = fldFindEveRngIndex((u32)script->ProcessID);
    fldSetTakaraOpen(room);
    return 1;
}

u32 fldCommand_FLD_CHK_TAKARA_OPEN(void) {
    if (fldGetTakaraOpen(fldFindEveRngIndex((u32)scrGetScriptHandle()->ProcessID)) != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_CALL_FIELD(void) {
    const char *entry;

    printf(D_00532A88);
    if (fldDoor_ChkWarp() != 0) {
        fldDoor_Warp(NULL);
        return 1;
    }
    entry = fldEveHitResName_Find((u32)scrGetScriptHandle()->ProcessID);
    if (entry != NULL) {
        fldDoor_Warp(entry);
    }
    return 1;
}

s32 fldCommand_FLD_CALL_WAPEX(void) {
    printf(D_00532AA0);
    fldDoor_WarpEx(scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_WARP_CASH_F);

u32 fldCommand_FLD_WARP_HASIGO_GETTBL(void) {
    s32 motionEntry;

    motionEntry = scrGetIntPara(0);
    motionEntry = fldDoor_GetHasigoTbl(motionEntry);
    scrSetIntReturnValue(motionEntry);
    return 1;
}

u32 fldCommand_FLD_WARP_ELV_GETTBL(void) {
    s32 rowValue;

    rowValue = scrGetIntPara(0);
    rowValue = fldDoor_GetElvTbl(rowValue);
    scrSetIntReturnValue(rowValue);
    return 1;
}

u32 fldCommand_FLD_WARP_ELV_EXIT(void) {
    s32 sceneRoomSelection;

    sceneRoomSelection = scrGetIntPara(0);
    fldDoor_ElvExit(sceneRoomSelection);
    return 1;
}

u32 fldCommand_FLD_ELV_FLOORSTART(void) {
    s32 first;
    s32 second;

    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    fldElvSetFloor(first, second);
    return 1;
}

u32 fldCommand_FLD_ELV_FLOOREND(void) {
    fldElvReset();
    return 1;
}

u32 fldCommand_FLD_ELV_ENDCHK(void) {
    s32 result;

    result = fldElvCheckEnd();
    scrSetIntReturnValue(result);
    return 1;
}

s32 fldCommand_FLD_WARP_GET_DOOR(void) {
    scrSetIntReturnValue(fldWapGetDoor(scrGetIntPara(0)));
    return 1;
}

u32 fldCommand_FLD_WARP_RUN_START(void) {
    s32 scene;
    if (fldDoor_ChkWarp()) {
        scene = 0;
    } else {
        scene = fldEveHitResID_Find((u32)scrGetScriptHandle()->ProcessID);
    }
    fldDoorRun_FindStart(scene);
    return 1;
}

s32 fldCommand_FLD_INF_NEXT(void) {
    s32 param0 = scrGetIntPara(0);
    s32 param1 = scrGetIntPara(1);

    scrSetIntReturnValue(fldInfNext(param0, param1));
    return 1;
}

s32 fldCommand_FLD_INF_GET(void) {
    s32 actorId = scrGetIntPara(0);
    s32 attribute = scrGetIntPara(1);

    scrSetIntReturnValue(fldInfGet(actorId, attribute));
    return 1;
}

void fldCommand_FLD_INF_GETPANEL(void) {
    scrSetIntReturnValue(fldInfChkFastPanel(1));
}

s32 fldCommand_FLD_INF_GOWAP(void) {
    fldInfToWAP();
    return 1;
}

u32 fldCommand_FLD_BGM_RELOAD(void) {
    fldBgmLoad();
    return 1;
}

s32 fldCommand_FLD_BGM_RELOAD_WAIT(void)
{
    return fldWaitBgmReload() != 0;
}

INCLUDE_RODATA(const s32, "code_00144798", D_00532A88);

INCLUDE_RODATA(const s32, "code_00144798", D_00532AA0);

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_BGM_PLAY);

s32 fldCommand_FLD_BGM_CONTINUE(void) {
    D_002FC020[0] = scrGetIntPara(0);
    fldBgmContinue();
    return 1;
}

s32 fldCommand_FLD_BGM_PAUSE(void) {
    fldPauseBgm();
    return 1;
}

s32 fldCommand_FLD_BGM_STOP(void) {
    fldBgmStop();
    return 1;
}

s32 fldCommand_FLD_BGM_FADE(void) {
    fldBgmFadeOut();
    return 1;
}

s32 fldCommand_FLD_SE_PLAY(void) {
    fldSePlay(scrGetIntPara(0));
    return 1;
}

s32 fldCommand_FLD_SE_STOP(void) {
    fldSeStop(scrGetIntPara(0));
    return 1;
}

s32 fldCommand_FLD_MSE_PLAY(void)
{
    s32 sequenceGroup;

    sequenceGroup = scrGetIntPara(0);
    fldMotionSePlay(sequenceGroup, 0);
    return 1;
}

s32 fldCommand_FLD_MSE_PLAY2(void) {
    s32 sequenceGroup = scrGetIntPara(0);
    s32 sequenceIndex = scrGetIntPara(1);

    fldMotionSePlay(sequenceGroup, sequenceIndex);
    return 1;
}

u32 fldCommand_FLD_MSE_STOP2(void) {
    s32 sequenceGroup;
    s32 sequenceIndex;

    sequenceGroup = scrGetIntPara(0);
    sequenceIndex = scrGetIntPara(1);
    fldMotionSeStop(sequenceGroup, sequenceIndex);
    return 1;
}

u32 fldCommand_FLD_TITLE_START(void) {
    return 1;
}

u32 fldCommand_FLD_TITLE_START2(void) {
    s32 fieldId;
    s32 titleArg;

    fieldId = scrGetIntPara(0);
    titleArg = scrGetIntPara(1);
    fldTitleStart(fieldId, titleArg);
    return 1;
}

s32 fldCommand_FLD_SET_ENCOUNT(void) {
    s32 value;

    value = scrGetIntPara(0);
    fldEncountRate[0] = value;
    return 1;
}

s32 fldCommand_FLD_EXIT_CAMERA(void) {
    scrProcessWork_s *command = scrGetScriptHandle();
    const char *record = fldEveHitResName_Find((u32)command->ProcessID);

    if (record != NULL) {
        fldDoor_ExitCamera(record);
    }
    return 1;
}

s32 fldCommand_FLD_WARP_GET_WARPEFF(void) {
    scrSetIntReturnValue(fldDoor_GetWarpEff());
    return 1;
}

s32 fldCommand_FLD_MODEL_LIGHT(void) {
    fldNpcLightSet(evtGetUnitHandle(scrGetIntPara(0)));
    return 1;
}

INCLUDE_ASM(const s32, "code_00144798", fldCommand_FLD_SAGASI_AKUMA);

u32 fldCommand_FLD_ANAHORI_INIT(void) {
    fldAnahoriSelectInit();
    return 1;
}

u32 fldCommand_FLD_ANAHORI_GET(void) {
    u32 busy;

    busy = fldAnahoriSelectGet();
    scrSetIntReturnValue(busy);
    return 1;
}

u32 fldCommand_FLD_ANAHORI_NEXT(void) {
    fldAnahoriSelectNext();
    return 1;
}

u32 fldCommand_FLD_ANAHORI_DEL(void) {
    fldAnahoriSelectDel();
    return 1;
}

s32 fldCommand_FLD_NPC_SINEN(void) {
    gFldKyouseiTalk = scrGetIntPara(0);
    return 1;
}

s32 fldCommand_FLD_GET_KIRETA(void) {
    s32 changed = 0;
    switch (scrGetIntPara(0)) {
    case 0:
        if (fldGlobalBase.consumedFlags & 1) {
            fldGlobalBase.consumedFlags &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (fldGlobalBase.consumedFlags & 2) {
            fldGlobalBase.consumedFlags &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (fldGlobalBase.consumedFlags & 4) {
            fldGlobalBase.consumedFlags &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (fldGlobalBase.consumedFlags & 8) {
            fldGlobalBase.consumedFlags &= ~8;
            changed = 1;
        }
        break;
    }
    scrSetIntReturnValue(changed);
    return 1;
}

s32 fldCommand_FLD_GET_ITEMBOX_RND(void) {
    s32 threshold = D_00316EC8[evtGetAgeOfMoon()];
    s32 value = dds3GetRandIntA(0, 100);

    printf(D_00532AC8, value, threshold);
    if (threshold >= value) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 fldCommand_FLD_MODEL_DANTE(void) {
    if (D_0054EAF0 == NULL) {
        scrSetIntReturnValue(-1);
        return 1;
    }
    scrSetIntReturnValue(D_0054EAF0->value);
    return 1;
}

u32 fldCommand_FLD_DANTE_AI_STOP(void) {
    evtBitOff(0xAF4);
    return 1;
}

u32 fldCommand_FLD_DANTE_AI_START(void) {
    evtBitOn(0xAF4);
    return 1;
}

s32 fldCommand_FLD_DANTE_STANCE(void) {
    fldRunDanteStanceSet(scrGetIntPara(0));
    return 1;
}
INCLUDE_RODATA(const s32, "code_00144798", D_00532AC8);

INCLUDE_SDATA(const s32, "code_00144798", gFldBgmBit);

INCLUDE_SDATA(const s32, "code_00144798", fldSeLastBgm);

INCLUDE_SDATA(const s32, "code_00144798", fldSeSmgId);

INCLUDE_SDATA(const s32, "code_00144798", gFldSndTransSrc);

INCLUDE_SDATA(const s32, "code_00144798", gFldSe3DNum);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE34);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE38);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE3C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE40);

INCLUDE_SDATA(const s32, "code_00144798", fldBgmReloadStep);

INCLUDE_SDATA(const s32, "code_00144798", fldBgmSmgAddr);

INCLUDE_SDATA(const s32, "code_00144798", fldBgmLoadStep);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapField);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapSeqStep);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE58);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapSeqState);

INCLUDE_SDATA(const s32, "code_00144798", AutoMapSeq);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapHeader);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapData);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapBuf);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE70);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapBBox);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE78);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE7C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE80);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapDrawX);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapDrawY);

INCLUDE_SDATA(const s32, "code_00144798", fldAutoMapDrawState);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE90);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE94);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE98);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EE9C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EEA0);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EEA4);

INCLUDE_SDATA(const s32, "code_00144798", fldAreaFontWork);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcCamTarget);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EEB0);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcModelTable);

INCLUDE_SDATA(const s32, "code_00144798", fldUiTexture);

INCLUDE_SDATA(const s32, "code_00144798", fldEATexture);

INCLUDE_SDATA(const s32, "code_00144798", fldVLTexture);

INCLUDE_SDATA(const s32, "code_00144798", fldVLBlurHandle);

INCLUDE_SDATA(const s32, "code_00144798", fldSnowBlurHandle);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcLoadIdx);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EED0);

INCLUDE_SDATA(const s32, "code_00144798", fldDmgBlurHandle);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcBlurHandle);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EEDC);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcFreeCount);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcModelFreeIdx);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcDrawCount);

INCLUDE_SDATA(const s32, "code_00144798", fldNpcModelCount);

INCLUDE_SDATA(const s32, "code_00144798", fldGenNpcData);

INCLUDE_SDATA(const s32, "code_00144798", fldGenNpcSize);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara1File);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara1Data);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara1Size);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara3File);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara3Data);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara3Size);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara2File);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara2Data);

INCLUDE_SDATA(const s32, "code_00144798", fldTakara2Size);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF1C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF20);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF24);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF28);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF2C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF30);

INCLUDE_SDATA(const s32, "code_00144798", fldItemBoxCount);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF38);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF3C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF40);

INCLUDE_SDATA(const s32, "code_00144798", fldTakaraSlotCount);

INCLUDE_SDATA(const s32, "code_00144798", fldTakaraCount);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF4C);

INCLUDE_SDATA(const s32, "code_00144798", fldEADrawState);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF54);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF58);

INCLUDE_SDATA(const s32, "code_00144798", fldEADrawWork);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF60);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF64);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF68);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF6C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF70);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF74);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF78);

INCLUDE_SDATA(const s32, "code_00144798", fldCamNpcTarget);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF80);

INCLUDE_SDATA(const s32, "code_00144798", fldCamNpcTarget2);

INCLUDE_SDATA(const s32, "code_00144798", fldTitleState);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF8C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF90);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF94);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF98);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EF9C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EFA0);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EFA4);

INCLUDE_SDATA(const s32, "code_00144798", fldTitleMiniState);

INCLUDE_SDATA(const s32, "code_00144798", fldSnowActive);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EFB0);

INCLUDE_SDATA(const s32, "code_00144798", fldTitleMiniTex);

INCLUDE_SDATA(const s32, "code_00144798", fldTitleMiniWork);

INCLUDE_SDATA(const s32, "code_00144798", gFldSeedBuff);

INCLUDE_SDATA(const s32, "code_00144798", fldSnowSeedBuf);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowMode);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowX);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowZ);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowY);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowYD);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowSpd);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowLife);

INCLUDE_SDATA(const s32, "code_00144798", gFldSnowW);

INCLUDE_SDATA(const s32, "code_00144798", D_0054EFE4);

INCLUDE_SDATA(const s32, "code_00144798", slDDS3To31Flg);

INCLUDE_SDATA(const s32, "code_00144798", slProcShutDownFlg);

INCLUDE_SDATA(const s32, "code_00144798", slProcPtr);

INCLUDE_SDATA(const s32, "code_00144798", _saveUI);

INCLUDE_SDATA(const s32, "code_00144798", slExitWaitCnt);

INCLUDE_SDATA(const s32, "code_00144798", baseIconFileHandle);

INCLUDE_SDATA(const s32, "code_00144798", gslCheckCardWait);

INCLUDE_SDATA(const s32, "code_00144798", gslDrawFlagFile);

INCLUDE_SDATA(const s32, "code_00144798", gslDrawFlagBg);

INCLUDE_SDATA(const s32, "code_00144798", gslAnimCnt);

INCLUDE_SDATA(const s32, "code_00144798", gSlSelCnt);

INCLUDE_SDATA(const s32, "code_00144798", gSlNowCardCheck);

INCLUDE_SDATA(const s32, "code_00144798", gSlExitMode);

INCLUDE_SDATA(const s32, "code_00144798", gSlFileErroeFlg);

INCLUDE_SDATA(const s32, "code_00144798", gSlSaveMode);

INCLUDE_SDATA(const s32, "code_00144798", gSlEndingMode);

INCLUDE_SDATA(const s32, "code_00144798", slChooseSaveIndex);

INCLUDE_SDATA(const s32, "code_00144798", slQueryYesNoCursor);

INCLUDE_SDATA(const s32, "code_00144798", slQueryYesNoMsg);

INCLUDE_SDATA(const s32, "code_00144798", slQueryYesNoKeyFlag);

INCLUDE_SDATA(const s32, "code_00144798", slFileSelError);

INCLUDE_SDATA(const s32, "code_00144798", gslFlushCnt);

INCLUDE_SDATA(const s32, "code_00144798", gslMode);

INCLUDE_SDATA(const s32, "code_00144798", gslInfoLoadFlg);

INCLUDE_SDATA(const s32, "code_00144798", gslExitType);

INCLUDE_SDATA(const s32, "code_00144798", gslInfoCode);

INCLUDE_SDATA(const s32, "code_00144798", gslErrorCode);

INCLUDE_SDATA(const s32, "code_00144798", gslErrorKeyFlag);

INCLUDE_SDATA(const s32, "code_00144798", version_convert_ver);

INCLUDE_SDATA(const s32, "code_00144798", slNextPageDraw_fc);

INCLUDE_SDATA(const s32, "code_00144798", gSlMcAccessCnt);

INCLUDE_SDATA(const s32, "code_00144798", slMcAccessDraw_fc);

INCLUDE_SDATA(const s32, "code_00144798", gSlWindowCnt);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F06C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F070);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F074);

INCLUDE_SDATA(const s32, "code_00144798", slFileSelWork);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F07C);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F080);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F088);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F090);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F098);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F0A0);

INCLUDE_SDATA(const s32, "code_00144798", D_0054F0A8);

