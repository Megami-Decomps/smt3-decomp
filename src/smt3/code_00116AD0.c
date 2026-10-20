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
    u16 sizeX;
    u16 sizeZ;
    u32 cellsOffset;
} fldRunMapGrid_t;

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

typedef struct {
    s32 gunFlag;                /* 0x00 */
    s32 word04;
    s32 word08;
    s32 word0C;
    s32 word10;
    s32 word14;
    s32 word18;
    s32 word1C;
    s32 word20;
    s32 word24;
    s32 word28;
    s32 skyTable;               /* 0x2C */
    s32 word30;
    s32 filter;                 /* 0x34 */
    s32 word38;
    s32 word3C;
    s32 word40;
    s32 enabled;                /* 0x44 */
    u32 unk48;
    s32 counter;                /* 0x4C */
    s32 word50;
    s32 word54;
    u32 unk58;
    s32 word5C;
} fldDanteGunWork_t;

extern fldDanteGunWork_t fldRunDanteGunFlag;
extern s32 fldPlayerStepDist;
extern u8 fldDoorExitFlag;
extern s8 fldDoorExitMode[];
extern char pointResName[];
extern char D_00531690[], D_00531950[];
extern s32 fldProcShutDownFlg;
extern s32 ProcSequence(dds3ProcessID_t *process);
extern s32 ProcDraw(dds3ProcessID_t *process);
extern void fclClearTerminalState(void);
extern void fldMakeFilePath(char *path, s32 area, s32 floor);
extern s32 sprintf(char *text, const char *format, ...);
extern sdfMemHandle_t *sdfDevLoadFileEx(char *path, void *destination, void *size);
extern void fldFreeSnowSeed(void);
extern void fldAutoMapSeqSetFlag(u32 flag);
extern void fldFreeInputPanel(void);
extern void evtDisableMoonDraw(void);
extern void fldFreeAddressProcess(void);
extern void dds3EffectDistortionBlurDelete(void *blur);
extern void campKillProcesses(void);
extern void scrAllReleaseScript(void);
extern s32 fldWmapIsEnabled(void);
extern s32 dds3ConfigChanged(void);
extern void dds3ConfigSaveBackup(void);
extern void dds3ConfigRestoreBackup(void);
extern void dds3SetPosPoseSet(dds3Basic_t *basic, void *position);
extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *basic);
extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, u32 flag);
extern void mdlSingleMotion(void *model, s32 motion, s32 flag);
extern void dds3GlobalWorkUserInit2(s32 mode);
extern s32 FroFtoI(f32 x, f32 z, u16 *ix, u16 *iz);
extern dds3Basic_t *fldAnaBasic;
extern void *hDistortionBlur;
extern u16 fldRunDanteStartX[2];
extern void fldRunDanteCollision(void);
extern void fldRunDanteAi(void);
extern void fldRunDanteSetModel(void);
extern void fldRunDanteSe(void);
extern void fldRunEffTask(void);
extern void fldRunTamaSe(void);
extern void fldRunDanteStanceSet(s32 stance);
extern char D_00531950[];
extern s32 p2sdLoadCheckMidi(u32 midino);
extern s32 p2sdLoadSMGFile(u32 midino);
extern void sdfDrawSync(void);
extern void dds3GlobalWorkClearFlags(void);
extern void evtInitGlobalWork(void);
extern void datInitPlayer(void);
extern void datInitBossPress(void);
extern void dds3ClearStockTable(void);
extern void frInitNameData(void);
extern void fclEncycInitRecord(s32 mode);
extern void NewGame2Push(void);
extern void NewGame2Pop(void);
extern s32 sdfFileOpen(const char *path);
extern void sdfFileRead(s32 fd, void *buffer, s32 size);
extern void sdfFileClose(s32 fd);
extern void datDelDevil(s32 devilId);
extern char D_005315D0[];
extern u8 D_0030A670[], D_00311450[], D_00311850[], D_00314100[], D_002FC1A0[];
extern u8 fldTakaraTable[], fldAllCnt[];
extern s32 nbGetEncountOn(void);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32 mode);
extern void fldSkyDrawFilter(void);
extern void fldDrawFrameSprite(void);
extern u32 fldDbg_BlurOn[];
extern u32 dds3AllocObjectId(void);
extern dds3Basic_t *dds3AddUnitObjectBasic(s32 id, void *position, void *rotation);
extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);
extern void dds3ObjectBaseMakeUnit(dds3Basic_t *basic, s32 kind, s32 arg2);
extern char D_00531820[];
extern f32 sdfSin(f32 radians);
extern f32 sdfCos(f32 radians);
extern void fldFindDoorEvent(void *event);
extern void fldFindTakaraSlot(s32 id);
extern char D_005316B0[];
extern char D_005316D0[], D_005316F0[], D_00531710[], D_00531728[], D_00531740[], D_00531758[];
extern s32 gfldPlayerSize;
extern void *gfldAnaPtr;
extern s32 gfldAnaSize;
extern void *gfldWavePtr;
extern s32 gfldWaveSize;
extern void mdlPackDecode(s32 group, s32 id, s32 mode, void *data, s32 size);
extern void *sdfMemAllocDirect(s32 size);
extern void *sdfMemGetPtr(sdfMemHandle_t *handle);
extern void *gfldPlayerPtr;
extern s32 gfldPlayerSizeBk;
extern s32 slGetExitType(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 mode);
extern void fldGbInit_System(s32 mode);
extern void fldGbFlagClear(void);
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
extern void dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, s32 (*func)(dds3PoseSet_t *, dds3Basic_t *));
extern void fldRunTamaFree(void);
extern void fldEncountClearFlag(u32 flag);
extern void p2sdFadeOutBgm(u32 arg);
extern void p2sdStopBGM(u32 arg);

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

/* One loadable field effect: the resource handle trio the loader fills, then
 * the live effect or part handle and the placement the task applies. */
typedef struct {
    sdfMemHandle_t *file;
    void *data;
    s32 size;
    void *part;
    void *eff;
    s32 life;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
} fldEffRes_t;

typedef struct {
    fldEffRes_t *res;
    s32 life;
    s32 mode;
    s32 rotate;
    f32 turnMin;
    f32 turnMax;
} fldEffEntry_t;

extern fldEffEntry_t *D_002FBEF0[];
extern char D_002FBD40[];
extern void FromRotation(f32 angle);
extern void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);
extern dds3Basic_t *D_0054EAB0;
extern dds3Basic_t *fldRunTama2Basic;
extern dds3Basic_t *fldRunTama3Basic;
extern s32 D_0054EABC;
extern sdfVector4_t D_00531148;
extern sdfVector4_t D_00531040;
extern char D_00531050[], D_00531060[], D_00531070[];
extern char D_00531080[], D_00531098[], D_005310B0[], D_005310C8[];
extern char D_005310E0[], D_005310F8[], D_00531118[], D_00531130[];
extern fldEffRes_t fldDspark01File;
extern fldEffRes_t fldDsmoke2File;
extern fldEffRes_t fldDspark02File;
extern fldEffRes_t fldGun02aFile;
extern fldEffRes_t fldGun01a0File;
extern fldEffRes_t fldMuzzleB010File;
extern fldEffRes_t fldDsmoke1File;
extern fldEffRes_t fldDflash1File;
extern void Mult(void);
typedef struct {
    f32 f[10];
} fldTamaSeFrame_t;

typedef struct {
    s16 v[10];
} fldTamaSeVol_t;

typedef struct {
    u8 unk00[0x1C];
    f32 frame;
} fldMotionCtrl_t;

typedef struct {
    u8 unk00[0x1C];
    fldMotionCtrl_t *motion;
} fldModel_t;

extern fldTamaSeFrame_t D_00531158;
extern fldTamaSeFrame_t D_00531180;
extern fldTamaSeFrame_t D_005311A8;
extern fldTamaSeVol_t D_005311D0;
extern s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z);
extern void frSetFontDrawOT(s32 ot);
extern void fldKanjiDraw(s32 x, s32 y, u32 color, const char *text);
extern void evtEnableMoonDraw(void);
extern void fldSkyRefreshTbl(void);
extern char D_0054EB40[];
extern s32 fldUnitPad(dds3PoseSet_t *pUpdatePoseSet, dds3Basic_t *pBasic);
extern void fldPlayerCalc_Hit(void);
extern void fldHitChkUnit(sdfVector4_t *pos);
extern sdfVector4_t D_00531830;
extern sdfVector4_t D_00531840;
extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 id, sdfVector4_t *campos, sdfVector4_t *tm, sdfVector4_t *lookat);
extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, sdfVector4_t *tm);
extern void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 radius);
extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);
extern void fldStatusSet(void);
extern dds3Basic_t *dds3GetActiveScene(void);
extern void dds3VisibleSceneBasic(dds3Basic_t *scene, s32 flag);
extern u8 cmpGetLoadState(void);
extern void dds3FadeOut2(s32 frame);
extern void evtKillFieldScript(void);
extern void p2sdStopAllSe(void);
extern void dds3FadeIn2(s32 frame);
extern s32 fldPlayerChkAna(void);
extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);
extern void fldPlayerPosRot(void);
extern void dds3FadeOut(s32 red, s32 green, s32 blue, s32 frame);
extern void fldPlayerMotReset(void);
extern void cmpProcessStart(void);
extern s32 dds3SceneBasicStartScript(dds3Basic_t *scene, const char *name);
extern s32 fldLightMaState __attribute__((section(".data")));
extern s32 fldChkVortex(void);
extern s32 fldProcCampChk(void);
extern s32 fldProcAmChk(void);
extern s32 fldEncountTestFlag(u32 flag);
extern s32 fldAutoMapSeqTestFlag(u32 flag);
extern void fldAutoMapSeqClearFlag(u32 flag);
extern void fldEncountSetFlag(u32 flag);
extern void fldReleaseUnitPad(void);
extern void fldAutoMapSeqStart(void);
extern sdfGrOt_t *D_002F6A18[4];
extern void dds3EffectDelete(void *node);
extern void dds3EffectDraw(void *node);
extern s32 dds3EffectGetState(void *node);
extern void *dds3EffectLoad(void *header);
extern void dds3EffectSetPosition(void *node, sdfVector4_t *pos);
extern void dds3EffectSetRotateMatrix(void *node, sdfMatrix4_t *matrix);
extern void *dds3EffectPartCreate(s32 kind, void *params);
extern void dds3EffectPartDelete(void *part);
extern void dds3EffectPartDraw(void *part);
extern void dds3EffectPartSetPosition(void *part, sdfVector4_t *pos);
extern void dds3EffectPartSetRotateMatrix(void *part, sdfMatrix4_t *matrix);
extern void SetRotateMatrixY(f32 angle);
extern void sdfModelOtInit(void *table, sdfGrOt_t *first, sdfGrOt_t *second,
                           sdfGrOt_t *third, sdfGrOt_t *fourth);

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
    f32 unk14;
    f32 unk18;
    s16 mapIx;
    s16 mapIz;
    u8 unk20[0x4];
    u16 unk24;
    u16 unk26;
    s32 unk28;
    void *unk2C;
    s32 unk30;
    s32 unk34;
    s32 waitCount;
    f32 angleToPlayer;
    f32 speed;
    s32 unk44;
    s32 unk48;
    u32 flag;
    s32 unk50;
    s32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
    char *doorName;
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

extern fldUnitControl_t fldUnitControl;
extern fldFollowCameraControl_t fldFollowCameraControl;
extern fldRunDante_t fldRun_Dante;
extern fldUnitControl_t *fldGetUnitControl(void);
extern fldFollowCameraControl_t *fldGetFollowCameraControl(void);

extern fldSequenceController_t fldAutoMapSeqFlags;
extern u32 fldEncountFlags;
extern s32 fldRunDanteHandle __attribute__((section(".bss")));
extern s32 fldRunDanteSe_asise;
extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;
extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;
extern s8 slNewGameBit2561[0x18];
extern u16 D_0054EA16;
extern s32 D_0054EA18;
extern sdfMemHandle_t *D_0054EA1C;
extern sdfMemHandle_t *D_00551C7C;
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

void NewGame2Push(void)
{
    s32 i;

    slNewGameBit2208 = 0;
    slNewGameBit2209 = 0;
    slNewGameBit2210 = 0;
    slNewGameBit2211 = 0;
    slNewGameBit2212 = 0;
    slNewGameBit2213 = 0;
    slNewGameBit2323 = 0;
    slNewGameBit162 = 0;
    D_0054EA16 = 0;
    D_0054EA18 = 0;
    if (evtBitCheck(0x8A0) != 0) {
        slNewGameBit2208 = 1;
    }
    if (evtBitCheck(0x8A1) != 0) {
        slNewGameBit2209 = 1;
    }
    if (evtBitCheck(0x8A2) != 0) {
        slNewGameBit2210 = 1;
    }
    if (evtBitCheck(0x8A3) != 0) {
        slNewGameBit2211 = 1;
    }
    if (evtBitCheck(0x8A4) != 0) {
        slNewGameBit2212 = 1;
    }
    if (evtBitCheck(0x8A5) != 0) {
        slNewGameBit2213 = 1;
    }
    if (evtBitCheck(0x913) != 0) {
        slNewGameBit2323 = 1;
    }
    if (evtBitCheck(0xA2) != 0) {
        slNewGameBit162 = 1;
    }
    D_0054EA16 = DDS3_GBWK->half0C;
    D_0054EA18 = DDS3_GBWK->unk3C;
    D_0054EA1C = sdfMemAllocDirect(0x7E90);
    memcpy(sdfMemGetPtr(D_0054EA1C), (u8 *)DDS3_GBWK + 0x11E80, 0x7E90);
    D_00551C7C = sdfMemAllocDirect(0xA0);
    memcpy(sdfMemGetPtr(D_00551C7C), DDS3_GBWK->boss_press, 0xA0);
    for (i = 0; i < 0x18; i++) {
        slNewGameBit2561[i] = 0;
    }
    if (evtBitCheck(0xA01) != 0) {
        slNewGameBit2561[0] = 1;
    }
    if (evtBitCheck(0xA03) != 0) {
        slNewGameBit2561[1] = 1;
    }
    if (evtBitCheck(0xA05) != 0) {
        slNewGameBit2561[2] = 1;
    }
    if (evtBitCheck(0xA07) != 0) {
        slNewGameBit2561[3] = 1;
    }
    if (evtBitCheck(0xA09) != 0) {
        slNewGameBit2561[4] = 1;
    }
    if (evtBitCheck(0xA0B) != 0) {
        slNewGameBit2561[5] = 1;
    }
    if (evtBitCheck(0xA0D) != 0) {
        slNewGameBit2561[6] = 1;
    }
    if (evtBitCheck(0xA0F) != 0) {
        slNewGameBit2561[7] = 1;
    }
    if (evtBitCheck(0xA11) != 0) {
        slNewGameBit2561[8] = 1;
    }
    if (evtBitCheck(0xA13) != 0) {
        slNewGameBit2561[9] = 1;
    }
    if (evtBitCheck(0xA15) != 0) {
        slNewGameBit2561[10] = 1;
    }
    if (evtBitCheck(0xA17) != 0) {
        slNewGameBit2561[11] = 1;
    }
    if (evtBitCheck(0xA19) != 0) {
        slNewGameBit2561[12] = 1;
    }
    if (evtBitCheck(0xA1B) != 0) {
        slNewGameBit2561[13] = 1;
    }
    if (evtBitCheck(0xA1D) != 0) {
        slNewGameBit2561[14] = 1;
    }
    if (evtBitCheck(0xA1F) != 0) {
        slNewGameBit2561[15] = 1;
    }
    if (evtBitCheck(0xA21) != 0) {
        slNewGameBit2561[16] = 1;
    }
    if (evtBitCheck(0xA23) != 0) {
        slNewGameBit2561[17] = 1;
    }
    if (evtBitCheck(0xA25) != 0) {
        slNewGameBit2561[18] = 1;
    }
    if (evtBitCheck(0xA27) != 0) {
        slNewGameBit2561[19] = 1;
    }
    if (evtBitCheck(0xA29) != 0) {
        slNewGameBit2561[20] = 1;
    }
    if (evtBitCheck(0xA2B) != 0) {
        slNewGameBit2561[21] = 1;
    }
    if (evtBitCheck(0xA2D) != 0) {
        slNewGameBit2561[22] = 1;
    }
    if (evtBitCheck(0xA2F) != 0) {
        slNewGameBit2561[23] = 1;
    }
}

void NewGame2Pop(void)
{
    s32 i;

    if (slNewGameBit2208 != 0) {
        evtBitOn(0x8A0);
    }
    if (slNewGameBit2209 != 0) {
        evtBitOn(0x8A1);
    }
    if (slNewGameBit2210 != 0) {
        evtBitOn(0x8A2);
    }
    if (slNewGameBit2211 != 0) {
        evtBitOn(0x8A3);
    }
    if (slNewGameBit2212 != 0) {
        evtBitOn(0x8A4);
    }
    if (slNewGameBit2213 != 0) {
        evtBitOn(0x8A5);
    }
    if (slNewGameBit2323 != 0) {
        evtBitOn(0x913);
    }
    if (slNewGameBit162 != 0) {
        evtBitOn(0xA2);
    }
    DDS3_GBWK->unk3C = D_0054EA18;
    DDS3_GBWK->word08 = D_0054EA18;
    DDS3_GBWK->half0C = D_0054EA16;
    memcpy((u8 *)DDS3_GBWK + 0x11E80, sdfMemGetPtr(D_0054EA1C), 0x7E90);
    sdfMemFreeDirect(D_0054EA1C);
    memcpy(DDS3_GBWK->boss_press, sdfMemGetPtr(D_00551C7C), 0xA0);
    sdfMemFreeDirect(D_00551C7C);
    if (slNewGameBit2561[0] != 0) {
        evtBitOn(0xA01);
    }
    if (slNewGameBit2561[1] != 0) {
        evtBitOn(0xA03);
    }
    if (slNewGameBit2561[2] != 0) {
        evtBitOn(0xA05);
    }
    if (slNewGameBit2561[3] != 0) {
        evtBitOn(0xA07);
    }
    if (slNewGameBit2561[4] != 0) {
        evtBitOn(0xA09);
    }
    if (slNewGameBit2561[5] != 0) {
        evtBitOn(0xA0B);
    }
    if (slNewGameBit2561[6] != 0) {
        evtBitOn(0xA0D);
    }
    if (slNewGameBit2561[7] != 0) {
        evtBitOn(0xA0F);
    }
    if (slNewGameBit2561[8] != 0) {
        evtBitOn(0xA11);
    }
    if (slNewGameBit2561[9] != 0) {
        evtBitOn(0xA13);
    }
    if (slNewGameBit2561[10] != 0) {
        evtBitOn(0xA15);
    }
    if (slNewGameBit2561[11] != 0) {
        evtBitOn(0xA17);
    }
    if (slNewGameBit2561[12] != 0) {
        evtBitOn(0xA19);
    }
    if (slNewGameBit2561[13] != 0) {
        evtBitOn(0xA1B);
    }
    if (slNewGameBit2561[14] != 0) {
        evtBitOn(0xA1D);
    }
    if (slNewGameBit2561[15] != 0) {
        evtBitOn(0xA1F);
    }
    if (slNewGameBit2561[16] != 0) {
        evtBitOn(0xA21);
    }
    if (slNewGameBit2561[17] != 0) {
        evtBitOn(0xA23);
    }
    if (slNewGameBit2561[18] != 0) {
        evtBitOn(0xA25);
    }
    if (slNewGameBit2561[19] != 0) {
        evtBitOn(0xA27);
    }
    if (slNewGameBit2561[20] != 0) {
        evtBitOn(0xA29);
    }
    if (slNewGameBit2561[21] != 0) {
        evtBitOn(0xA2B);
    }
    if (slNewGameBit2561[22] != 0) {
        evtBitOn(0xA2D);
    }
    if (slNewGameBit2561[23] != 0) {
        evtBitOn(0xA2F);
    }
}

void dds3GlobalWorkUserInit2(s32 mode)
{
    s32 saved;

    if (mode == 1) {
        NewGame2Push();
        saved = DDS3_GBWK->memHandle;
        memset(DDS3_GBWK, 0, 0x1A510);
        DDS3_GBWK->memHandle = saved;
    }
    DDS3_GBWK->unk38 = 0;
    DDS3_GBWK->unk3C = 0;
    dds3GlobalWorkClearFlags();
    evtInitGlobalWork();
    DDS3_GBWK->akuma[0].unk00 = 8;
    datInitPlayer();
    datInitBossPress();
    dds3ClearStockTable();
    DDS3_GBWK->unk1CFA = 0;
    DDS3_GBWK->heartscnt = 0;
    frInitNameData();
    fldGbInit_System(0);
    fclEncycInitRecord(0);
    if (mode == 1) {
        NewGame2Pop();
    }
}

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

s32 ooyTestDrawProc(dds3ProcessID_t *id)
{
    if (nbGetEncountOn() != 0) {
        return 0;
    }
    if (fldProcCampChk() == 1) {
        return 0;
    }
    rsetOtOfs(0x47);
    rsetTest(1, 0, 0x81, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    fldSkyDrawFilter();
    if (fldDbg_BlurOn[0] != 0) {
        if (fldAutoMapChkMode() == 0) {
            rsetOtOfs(0x47);
        } else {
            rsetOtOfs(0x51);
        }
        fldDrawFrameSprite();
    }
    rsetOtOfs(0x27);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    rsetOtOfs(0x2E);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
    return 0;
}

void InitOoyTest(void)
{
    dds3StartProcess(D_00530F30, DDS3_USERDRAW_PRIORITY, 0, 0, ooyTestDrawProc, NULL, NULL);
}

void KillOoyTest(void)
{
    dds3KillProcessName(D_00530F30, 1);
}

void fldElvGetFloorDigits(s32 x, s32 y, s32 floor)
{
    char text[32];
    s32 i;
    s32 digit;

    frSetFontDrawOT(0x48);
    if (floor > 1000) {
        char *roof = D_002FBD40;

        fldKanjiDraw(x, y / 2, -0x80, roof + 0x30);
        return;
    }
    i = 0;
    if (floor < 0) {
        text[0] = 'B';
        i = 1;
        floor = -floor;
    }
    digit = floor / 10;
    if (digit >= 1 && digit <= 9) {
        text[i] = digit + '0';
        i++;
    }
    digit = floor % 10;
    if (digit < 10) {
        text[i] = digit + '0';
        i++;
    }
    text[i] = 'F';
    text[i + 1] = '\0';
    fldKanjiDraw(x, y / 2, -0x80, text);
}

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

void fldRunTamaInit(void)
{
    fldEffRes_t *res;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t axis;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    axis = D_00531040;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(3.1415926f);
    SDF_VU0_LOADVECTOR_B(&rot);
    Mult();
    SDF_VU0_STOREVECTOR_NOMEM(&rot);
    D_0054EAB0 = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(D_0054EAB0, D_00531050);
    dds3ObjectBaseMakeUnit(D_0054EAB0, 2, 0x11);
    dds3ObjectBaseOnFlag(D_0054EAB0, 1);
    dds3ObjectBaseOnFlag(D_0054EAB0, 0x100);
    fldRunTama2Basic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(fldRunTama2Basic, D_00531060);
    dds3ObjectBaseMakeUnit(fldRunTama2Basic, 2, 0x12);
    dds3ObjectBaseOnFlag(fldRunTama2Basic, 1);
    dds3ObjectBaseOnFlag(fldRunTama2Basic, 0x100);
    fldRunTama3Basic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(fldRunTama3Basic, D_00531070);
    dds3ObjectBaseMakeUnit(fldRunTama3Basic, 2, 0x13);
    dds3ObjectBaseOnFlag(fldRunTama3Basic, 1);
    dds3ObjectBaseOnFlag(fldRunTama3Basic, 0x100);
    res = &fldDspark01File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->file = sdfDevLoadFileEx(D_00531080, &res->data, &res->size);
    res = &fldDsmoke2File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_00531098, &res->data, &res->size);
    res = &fldDspark02File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_005310B0, &res->data, &res->size);
    res = &fldGun02aFile;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_005310C8, &res->data, &res->size);
    res = &fldGun01a0File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_005310E0, &res->data, &res->size);
    res = &fldMuzzleB010File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_005310F8, &res->data, &res->size);
    res = &fldDsmoke1File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_00531118, &res->data, &res->size);
    res = &fldDflash1File;
    res->file = NULL;
    res->data = NULL;
    res->size = 0;
    res->part = NULL;
    res->eff = NULL;
    res->life = 0;
    res->x = 0.0f;
    res->y = 0.0f;
    res->z = 0.0f;
    res->turn = 0.0f;
    res->file = sdfDevLoadFileEx(D_00531130, &res->data, &res->size);
}

/* NON_MATCHING: 27 of 47 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void fldRunTamaFree(void)
{
    fldEffEntry_t *entry;
    fldEffRes_t *res;
    void *handle;
    s32 i;

    for (i = 0; i < 2; i++) {
        for (entry = D_002FBEF0[i]; (res = entry->res) != NULL; entry++) {
            if (entry->mode == 0) {
                handle = res->eff;
                if (handle != NULL) {
                    dds3EffectDelete(handle);
                    res->eff = NULL;
                }
            } else {
                handle = res->part;
                if (handle != NULL) {
                    dds3EffectPartDelete(handle);
                    res->part = NULL;
                }
            }
            handle = res->file;
            if (handle != NULL) {
                sdfMemFreeDelayedDirect(handle);
                res->file = NULL;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaFree);
#endif

/* NON_MATCHING: 67 of 73 words differ (first at +0x10). */
#ifdef NON_MATCHING
void fldRunEffSet(s32 id, f32 x, f32 y, f32 z, f32 turn)
{
    fldRunDante_t *dante = &fldRun_Dante;
    fldEffEntry_t *entry;
    fldEffRes_t *res;
    void *handle;

    for (entry = D_002FBEF0[id]; (res = entry->res) != NULL; entry++) {
        if (entry->turnMin < dante->unk60 && dante->unk60 < entry->turnMax) {
            if (entry->mode == 0) {
                handle = res->eff;
                if (handle != NULL) {
                    dds3EffectDelete(handle);
                    res->eff = NULL;
                }
                res->eff = dds3EffectLoad(res->data);
                res->life = entry->life;
                res->x = x;
                res->y = y;
                res->z = z;
                res->turn = turn;
            } else {
                handle = res->part;
                if (handle != NULL) {
                    dds3EffectPartDelete(handle);
                    res->part = NULL;
                }
                res->part = dds3EffectPartCreate(6, res->data);
                res->life = entry->life;
                res->x = x;
                res->y = y;
                res->z = z;
                res->turn = turn;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldRunEffSet);
#endif

/* NON_MATCHING: 115 of 136 words differ (first at +0x10). */
#ifdef NON_MATCHING
void fldRunEffTask(void)
{
    sdfVector4_t pos;
    sdfMatrix4_t matrix;
    fldEffEntry_t *entry;
    fldEffRes_t *res;
    s32 i;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;

    for (i = 0; i < 2; i++) {
        for (entry = D_002FBEF0[i]; (res = entry->res) != NULL; entry++) {
            if (entry->mode == 0) {
                if (res->eff == NULL) {
                    continue;
                }
                if (dds3EffectGetState(res->eff) == 0) {
                    dds3EffectDelete(res->eff);
                    res->eff = NULL;
                    continue;
                }
                pos.x = res->x;
                pos.y = res->y;
                pos.z = res->z;
                dds3EffectSetPosition(res->eff, &pos);
                if (entry->rotate == 1) {
                    SetRotateMatrixY((90.0f - res->turn) * 3.14f / 180.0f);
                    SDF_VU0_STOREMATRIX(&matrix);
                    dds3EffectSetRotateMatrix(res->eff, &matrix);
                }
                dds3EffectDraw(res->eff);
            } else {
                if (res->part == NULL) {
                    continue;
                }
                if (res->life < 0) {
                    dds3EffectPartDelete(res->part);
                    res->part = NULL;
                    continue;
                }
                res->life = res->life - 1;
                pos.x = res->x;
                pos.y = res->y;
                pos.z = res->z;
                dds3EffectPartSetPosition(res->part, &pos);
                if (entry->rotate == 1) {
                    SetRotateMatrixY((90.0f - res->turn) * 3.14f / 180.0f);
                    SDF_VU0_STOREMATRIX(&matrix);
                    dds3EffectPartSetRotateMatrix(res->part, &matrix);
                }
                dds3EffectPartDraw(res->part);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldRunEffTask);
#endif

void fldRunTamaSet(f32 x, f32 y, f32 z, f32 turn)
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t axis;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    axis = D_00531148;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    SDF_VU0_LOADVECTOR(&axis);
    turn = turn * 3.14f;
    FromRotation(turn / 180.0f);
    SDF_VU0_STOREVECTOR(&rot);
    switch (D_0054EABC) {
    case 0:
        dds3ObjectBaseOffFlag(D_0054EAB0, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(D_0054EAB0), 0, 0);
        dds3SetRotPoseSet(D_0054EAB0, &rot);
        dds3SetPosPoseSet(D_0054EAB0, &pos);
        break;
    case 1:
        dds3ObjectBaseOffFlag(fldRunTama2Basic, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldRunTama2Basic), 0, 0);
        dds3SetRotPoseSet(fldRunTama2Basic, &rot);
        dds3SetPosPoseSet(fldRunTama2Basic, &pos);
        break;
    case 2:
        dds3ObjectBaseOffFlag(fldRunTama3Basic, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldRunTama3Basic), 0, 0);
        dds3SetRotPoseSet(fldRunTama3Basic, &rot);
        dds3SetPosPoseSet(fldRunTama3Basic, &pos);
        break;
    }
    D_0054EABC = D_0054EABC + 1;
    D_0054EABC = D_0054EABC % 3;
}

/* NON_MATCHING: 165 of 230 words differ (first at +0x34). */
#ifdef NON_MATCHING
void fldRunTamaSe(void)
{
    sdfVector4_t pos;
    fldTamaSeFrame_t frame1;
    fldTamaSeFrame_t frame2;
    fldTamaSeFrame_t frame3;
    fldTamaSeVol_t vol;
    fldModel_t *model;
    f32 now;
    s32 i;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    frame1 = D_00531158;
    frame2 = D_00531180;
    frame3 = D_005311A8;
    vol = D_005311D0;
    model = dds3ObjectBaseGetModelHandle(D_0054EAB0);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame1.f[i] - 0.5f < now && now < frame1.f[i] + 0.5f) {
            dds3GetPosPoseSet(D_0054EAB0);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
    model = dds3ObjectBaseGetModelHandle(fldRunTama2Basic);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame2.f[i] - 0.5f < now && now < frame2.f[i] + 0.5f) {
            dds3GetPosPoseSet(fldRunTama2Basic);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
    model = dds3ObjectBaseGetModelHandle(fldRunTama3Basic);
    now = model->motion->frame;
    for (i = 0; i < 10; i++) {
        if (frame3.f[i] - 0.5f < now && now < frame3.f[i] + 0.5f) {
            dds3GetPosPoseSet(fldRunTama3Basic);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            p2sdPlaySE(0x6F0006,
                       fldRunDanteDistToTarget(pos.x, pos.y, pos.z) * vol.v[i] / 128 / 2, 0x3F);
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldRunTamaSe);
#endif

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531040);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531050);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531060);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531070);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531080);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531098);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005310B0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005310C8);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005310E0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005310F8);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531118);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531130);

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

s32 FroFtoI(f32 x, f32 z, u16 *ix, u16 *iz)
{
    f32 ox = fldRun_MAP_OX;
    f32 oz = fldRun_MAP_OZ;
    f32 best = 10000.0f;
    s16 bestX = 0;
    s16 bestZ = 0;
    s16 i;
    s16 j;
    s32 offset;
    f32 dx;
    f32 dz;
    f32 d;

    for (j = 0; j < ((fldRunMapGrid_t *)pFroMapHead)->sizeZ; j++) {
        for (i = 0; i < ((fldRunMapGrid_t *)pFroMapHead)->sizeX; i++) {
            offset = (((fldRunMapGrid_t *)pFroMapHead)->sizeX * j + i) * 2;
            if (((u8 *)fldRunMapScale)[offset] != 0) {
                dx = x - (ox + (f32)fldRun_MAP_BX * (f32)i);
                dz = z - (oz + (f32)-fldRun_MAP_BZ * (f32)j);
                EE_SQRT_F32_SCHED(d, dx * dx + dz * dz);
                if (d < best) {
                    best = d;
                    bestZ = j;
                    bestX = i;
                }
            }
        }
    }
    *ix = bestX;
    *iz = bestZ;
    if (bestX == 0 && bestZ == 0) {
        return -1;
    }
    return 0;
}

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

void fldRunDanteDoin(void)
{
    void **basic;

    if (fldRun_Dante.waitCount > 0) {
        fldRun_Dante.waitCount--;
    }
    if (fldRun_Dante.waitCount == 0x4E && fldRun_Dante.doorName != NULL) {
        basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 0x11, fldRun_Dante.doorName);
        fldFindDoorEvent(basic[1]);
    }
    if (fldRun_Dante.waitCount >= 0x15) {
        fldRun_Dante.x += sdfSin(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
        fldRun_Dante.z += sdfCos(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
    } else if (fldRun_Dante.waitCount == 0x14) {
        fldRun_Dante.unk50 = 3;
        fldRun_Dante.flag |= 2;
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, &fldRun_Dante.mapIx, &fldRun_Dante.mapIz);
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteMove);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDantePwak);

void fldRunDanteSoba(void)
{
    f32 dist;

    if (fldRun_Dante.waitCount > 0) {
        fldRun_Dante.waitCount--;
    }
    dist = fldRun_Dante.unk58;
    fldRun_Dante.angleToPlayer = fldRun_Dante.unk5C;
    fldRun_Dante.turn =
        fldTurnToward(fldRun_Dante.turn, 180.0f - fldRun_Dante.unk5C, 10.0f, 1.0f);
    if (dist > 100.0f) {
        fldRun_Dante.x += sdfSin(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
        fldRun_Dante.z += sdfCos(fldRun_Dante.angleToPlayer * 3.14f / 180.0f) * fldRun_Dante.speed;
    }
    FroFtoI(fldRun_Dante.x, fldRun_Dante.z, &fldRun_Dante.mapIx, &fldRun_Dante.mapIz);
    if (dist < 3600.0f) {
        if (fldRun_Dante.unk54 != 1) {
            fldRun_Dante.unk54 = 1;
            fldRun_Dante.flag |= 4;
        }
    } else if (fldRun_Dante.unk54 != 0) {
        fldRun_Dante.unk54 = 0;
        fldRun_Dante.flag |= 4;
    }
}

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDantePatr);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteGun1);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteGun2);

INCLUDE_ASM(const s32, "code_00116AD0", fldRunDanteCollision);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531318);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531328);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531338);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531350);

void fldRunDanteAi(void)
{
    switch (fldRun_Dante.unk00) {
    case 0:
        func_0011C790();
        return;
    case 1:
        func_0011C798();
        return;
    case 2:
        func_0011C7B8();
        return;
    case 9:
        fldRunDanteDoin();
        return;
    case 5:
        fldRunDantePwak();
        return;
    case 3:
        fldRunDanteSoba();
        return;
    case 4:
        fldRunDantePatr();
        return;
    case 7:
        fldRunDanteGun1();
        return;
    case 8:
        fldRunDanteGun2();
        return;
    default:
        do {
            fldRunDanteMove();
        } while (0);
        break;
    }
}

void fldRunDanteLoadBgm(void)
{
    if (p2sdLoadCheckMidi(0x820000) == 0) {
        p2sdLoadSMGFile(0x820000);
    }
    while (p2sdLoadCheckMidi(0x820000) != 1) {
        sdfDrawSync();
    }
    if (p2sdLoadCheckMidi(0x830000) == 0) {
        p2sdLoadSMGFile(0x830000);
    }
    while (p2sdLoadCheckMidi(0x830000) != 1) {
        sdfDrawSync();
    }
    if (p2sdLoadCheckMidi(0x6F0000) == 0) {
        p2sdLoadSMGFile(0x6F0000);
    }
    while (p2sdLoadCheckMidi(0x6F0000) != 1) {
        sdfDrawSync();
    }
    p2sdStopBGM(0x820000);
}

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

void fldRunDanteInit(void)
{
    fldMakeFroFileName();
    fldRunDanteLoadBgm();
    fldRunTamaInit();
    fldRun_MAP_BX = 600;
    fldRun_MAP_BZ = 600;
    switch (fldGlobalBase.floor) {
    case 3:
        fldRun_MAP_OX = 0xF6E;
        fldRun_MAP_OZ = 0xC1C;
        fldRun_Dante.mapIx = 3;
        fldRun_Dante.mapIz = 10;
        break;
    case 4:
        fldRun_MAP_OX = 0xC8;
        fldRun_MAP_OZ = 0x64;
        fldRun_Dante.mapIx = 7;
        fldRun_Dante.mapIz = 0;
        fldRunDanteStartX[0] = 3;
        fldRunDanteStartX[1] = 2;
        break;
    case 5:
        fldRun_MAP_OX = 0xCE4;
        fldRun_MAP_OZ = -0x834;
        fldRun_Dante.mapIx = 0;
        fldRun_Dante.mapIz = 0;
        break;
    case 7:
        fldRun_MAP_OX = -0x1482;
        fldRun_MAP_OZ = 0xFD2;
        fldRun_Dante.mapIx = 0;
        fldRun_Dante.mapIz = 0;
        break;
    case 8:
        fldRun_MAP_OX = -0xDAC;
        fldRun_MAP_OZ = -0xB54;
        fldRun_Dante.mapIx = 0;
        fldRun_Dante.mapIz = 0;
        break;
    case 9:
        fldRun_MAP_OX = 0x12C;
        fldRun_MAP_OZ = 0x834;
        fldRun_Dante.mapIx = 0;
        fldRun_Dante.mapIz = 0;
        break;
    case 10:
        fldRun_MAP_OX = -0xA8C;
        fldRun_MAP_OZ = -0xA8C;
        fldRun_Dante.mapIx = 0;
        fldRun_Dante.mapIz = 0;
        break;
    }
    fldRun_Dante.waitCount = -20;
    fldRun_Dante.unk26 = 0;
    fldRun_Dante.unk30 = 0;
    fldRun_Dante.unk34 = 0;
    fldRun_Dante.y = 0.0f;
    fldRun_Dante.angleToPlayer = 0.0f;
    fldRun_Dante.speed = 0.0f;
    fldRun_Dante.unk48 = 0;
    fldRun_Dante.flag = 0;
    fldRun_Dante.unk54 = 0;
    fldRun_Dante.unk64 = 0;
    fldRun_Dante.unk60 = 0.0f;
    fldRun_Dante.x = fldRun_MAP_OX + (f32)fldRun_MAP_BX * (f32)fldRun_Dante.mapIx;
    fldRun_Dante.z = fldRun_MAP_OZ + (f32)-fldRun_MAP_BZ * (f32)fldRun_Dante.mapIz;
    switch (fldGlobalBase.floor) {
    case 3:
        fldRunDanteStanceSet(6);
        evtBitOn(0xAF4);
        break;
    case 4:
        fldRunDanteStanceSet(2);
        evtBitOn(0xAF4);
        break;
    case 5:
        fldRunDanteStanceSet(3);
        evtBitOn(0xAF4);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        fldRunAiCng(0);
        evtBitOff(0xAF4);
        break;
    default:
        fldRunAiCng(2);
        evtBitOff(0xAF4);
        break;
    }
    fldRunDanteSetModel();
}

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

void fldRunDanteSe(void)
{
    s32 seFlag = 0;
    s32 walkFlag = 0;
    s32 vol;
    f32 prev;
    f32 now;

    now = fldRun_Dante.unk18;
    prev = fldRun_Dante.unk14;
    vol = fldRunDanteDistToTarget(fldRun_Dante.x, fldRun_Dante.y, fldRun_Dante.z);
    switch (fldRun_Dante.unk50) {
    case 0:
        if (prev < 5.0f && 5.0f <= now) {
            seFlag = 1;
        }
        if (prev < 15.0f && 15.0f <= now) {
            seFlag = 1;
        }
        break;
    case 2:
        if (prev < 5.0f && 5.0f <= now) {
            seFlag = 1;
        }
        if (prev < 24.0f && 24.0f <= now) {
            seFlag = 1;
        }
        break;
    case 1:
        if (prev < 3.0f && 3.0f <= now) {
            seFlag = 1;
        }
        if (prev < 18.0f && 18.0f <= now) {
            seFlag = 1;
        }
        if (fldRun_Dante.flag & 2) {
            walkFlag = 1;
        }
        break;
    case 4:
        if (prev < 17.0f && 17.0f <= now) {
            walkFlag = 1;
        }
        break;
    }
    if (seFlag != 0) {
        p2sdPlaySE(0x6F0001 + fldRunDanteSe_asise, vol, 0x3F);
        fldRunDanteSe_asise =
            (fldRunDanteSe_asise + (s32)(((u32)dds3GetRandA(0) >> 4) & 1) + 1) % 4;
    }
    if (walkFlag != 0) {
        p2sdPlaySE(0x6F000A, vol * 30 / 100, 0x3F);
    }
    if (fldRun_Dante.flag & 8) {
        p2sdPlaySE(0x6F0005, vol * 30 / 100, 0x3F);
    }
    if (fldRun_Dante.flag & 4) {
        fldRunDanteSwapBgm(fldRun_Dante.unk54);
    }
}

void fldRunView(void)
{
    if (evtBitCheck(0xAF3) == 0) {
        return;
    }
    if (evtBitCheck(0xAF4) == 0) {
        return;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return;
    }
    fldRunDanteCollision();
    fldRunDanteAi();
    fldRunDanteSetModel();
    fldRunDanteSe();
    fldRunEffTask();
    fldRunTamaSe();
    FroFtoI(fldGlobalBase.x, fldGlobalBase.z, &fldRunDanteStartX[0], &fldRunDanteStartX[1]);
    fldRun_Dante.mapIxOld = fldRun_Dante.mapIx;
    fldRun_Dante.mapIzOld = fldRun_Dante.mapIz;
}

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

void fldAllTblLoad(void)
{
    s32 fd = sdfFileOpen(D_005315D0);

    sdfFileRead(fd, D_0030A670, 0x4E0);
    sdfFileRead(fd, fldAreaNameTable, 0x2BC0);
    sdfFileRead(fd, fldPlaceNameTable, 0x24C0);
    sdfFileRead(fd, fldFloorNameTable, 0x1880);
    sdfFileRead(fd, D_00311450, 0x400);
    sdfFileRead(fd, D_00311850, 0x800);
    sdfFileRead(fd, D_00314100, 0x2A00);
    sdfFileRead(fd, fldTakaraTable, 0x1600);
    sdfFileRead(fd, D_002FC1A0, 0x80);
    sdfFileRead(fd, fldAllCnt, 0x100);
    sdfFileClose(fd);
}

/* NON_MATCHING: 23 of 73 words differ (first at +0xC). */
#ifdef NON_MATCHING
void fldGbFlagClear(void)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 0x20; i++) {
        DDS3_GBWK->takaraOpen[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        DDS3_GBWK->takaraOpenEx[i] = 0;
    }
    for (k = 0; k < 0x28; k++) {
        for (j = 0; j < 0x20; j++) {
            DDS3_GBWK->gb[k].flags[j].modelFlag = 0;
            DDS3_GBWK->gb[k].flags[j].animeFlag = 0;
            DDS3_GBWK->gb[k].flags[j].atariFlag = 0;
            DDS3_GBWK->gb[k].flags[j].modelFlag2 = 0;
            DDS3_GBWK->gb[k].flags[j].animeFlag2 = 0;
            for (i = 0; i < 0x10; i++) {
                DDS3_GBWK->gb[k].flags[j].animeId2[i] = -1;
            }
            DDS3_GBWK->gb[k].flags[j].npcFlag = 0;
            DDS3_GBWK->gb[k].flags[j].hojiFlag = 0;
        }
    }
    fldAutoMapClear();
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldGbFlagClear);
#endif

void fldGbInit_System(s32 mode)
{
    s32 *p;
    s32 i;
    f32 dist;
    f32 ang;

    fldGlobalBase.area = 0;
    dist = 100.0f;
    ang = 180.0f;
    fldGlobalBase.wordA4 = 0x10;
    fldGlobalBase.floor = 0;
    fldGlobalBase.blockID = 0;
    fldGlobalBase.word10 = 0;
    fldGlobalBase.consumedFlags = 0;
    fldGlobalBase.mode = 0;
    fldGlobalBase.word1C = 0;
    fldGlobalBase.word5C = 0;
    fldGlobalBase.menuOpenFlag = 0;
    fldGlobalBase.encountState = 0;
    fldGlobalBase.playerSeTable = 0;
    fldGlobalBase.word44 = 0;
    fldGlobalBase.hitMatterIndex = -1;
    fldGlobalBase.word4C = -1;
    fldGlobalBase.nameU = -1;
    fldGlobalBase.nameD = -1;
    fldGlobalBase.word98 = -1;
    fldGlobalBase.floatB4 = dist;
    fldGlobalBase.word90 = 1;
    fldGlobalBase.word94 = 0;
    fldGlobalBase.cameraParamIndex = 0;
    fldGlobalBase.sequenceCode = 1;
    fldGlobalBase.reuseAreaResources = 0;
    fldGlobalBase.wordAC = 0;
    fldGlobalBase.wordB0 = 0;
    fldGlobalBase.word78 = 0;
    fldGlobalBase.word80 = 0;
    fldGlobalBase.lightMaIdx = 0;
    fldGlobalBase.wordBC = 0;
    fldGlobalBase.wordC0 = 0;
    fldGlobalBase.wordC8 = 0;
    fldGlobalBase.wordCC = 0;
    fldGlobalBase.wordF8 = 0;
    fldGlobalBase.camAngle2 = ang;
    fldGlobalBase.x = 0.0f;
    fldGlobalBase.y = 0.0f;
    fldGlobalBase.z = 0.0f;
    fldGlobalBase.prevX = 0.0f;
    fldGlobalBase.prevY = 0.0f;
    fldGlobalBase.prevZ = 0.0f;
    fldGlobalBase.hitX = 0.0f;
    fldGlobalBase.hitY = 0.0f;
    fldGlobalBase.hitZ = 0.0f;
    fldGlobalBase.camAngle = 0.0f;
    fldGlobalBase.float12C = 0.0f;
    fldGlobalBase.float130 = 0.0f;
    fldGlobalBase.hitValid = 0;
    fldRunDanteGunFlag.gunFlag = 0;
    fldRunDanteGunFlag.word04 = 0;
    fldRunDanteGunFlag.word08 = 0;
    fldRunDanteGunFlag.word0C = 0;
    fldRunDanteGunFlag.word10 = 0;
    fldRunDanteGunFlag.word14 = 0;
    fldRunDanteGunFlag.word18 = 0;
    fldRunDanteGunFlag.word1C = 0;
    fldRunDanteGunFlag.word20 = 0;
    fldRunDanteGunFlag.word24 = 1;
    fldRunDanteGunFlag.word28 = 1;
    fldRunDanteGunFlag.skyTable = 0;
    fldRunDanteGunFlag.word30 = 0;
    fldRunDanteGunFlag.filter = 0;
    fldRunDanteGunFlag.word38 = 0;
    fldRunDanteGunFlag.word3C = 0;
    fldRunDanteGunFlag.word40 = 0;
    fldRunDanteGunFlag.counter = 1;
    fldRunDanteGunFlag.unk48 = 0;
    fldRunDanteGunFlag.word50 = 0;
    fldRunDanteGunFlag.word5C = 1;
    fldRunDanteGunFlag.unk58 = 0;
    fldRunDanteGunFlag.word54 = 0;
    if (mode == 0) {
        fldGbFlagClear();
        DDS3_GBWK->previewX = 0;
        DDS3_GBWK->previewY = 0;
        p = (s32 *)((u8 *)DDS3_GBWK + 0x11A8C);
        for (i = 14; i >= 0; i--) {
            *p = 0;
            p++;
        }
        DDS3_GBWK->ver[0] = 0x56;
        DDS3_GBWK->ver[1] = 0x45;
        DDS3_GBWK->ver[2] = 0x52;
        DDS3_GBWK->ver[3] = 1;
        DDS3_GBWK->ver[4] = 0;
        DDS3_GBWK->ver[5] = 0;
        DDS3_GBWK->ver[6] = 0;
        DDS3_GBWK->ver[7] = 0;
        DDS3_GBWK->word08 = 0;
        DDS3_GBWK->half0C = 0;
        DDS3_GBWK->transition = 0;
    }
    fldRunDanteClear();
}

void fldResetSequenceParams(void)
{
    f32 dist;

    fldGlobalBase.consumedFlags = 0;
    dist = 100.0f;
    fldGlobalBase.word1C = 0;
    fldGlobalBase.word44 = 0;
    fldGlobalBase.hitMatterIndex = -1;
    fldGlobalBase.word4C = -1;
    fldGlobalBase.word94 = 0;
    fldGlobalBase.wordB0 = 0;
    fldGlobalBase.word78 = 0;
    fldPlayerStepDist = 0;
    fldGlobalBase.word80 = 0;
    fldGlobalBase.wordF8 = 0;
    fldRunDanteGunFlag.gunFlag = 0;
    fldRunDanteGunFlag.word04 = 0;
    fldRunDanteGunFlag.word08 = 0;
    fldRunDanteGunFlag.word0C = 0;
    fldRunDanteGunFlag.word10 = 0;
    fldRunDanteGunFlag.word14 = 0;
    fldRunDanteGunFlag.skyTable = 0;
    fldRunDanteGunFlag.word30 = 0;
    fldRunDanteGunFlag.filter = 0;
    fldRunDanteGunFlag.word38 = 0;
    fldGlobalBase.word90 = 1;
    fldGlobalBase.word98 = -1;
    fldGlobalBase.wordA4 = 0x10;
    fldGlobalBase.floatB4 = dist;
    fldRunDanteGunFlag.word40 = 0;
    fldGlobalBase.wordC8 = 0;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_005315D0);

s32 dds3EffectFldCharToType(s32 charId)
{
    if (charId < 0x28) {
        return charId % 100;
    }
    switch (charId) {
    case 0x28:
        return 2;
    case 0x29:
        return 5;
    case 0x2A:
        return 7;
    case 0x2B:
        return 9;
    case 0x2C:
        return 10;
    case 0x2D:
        return 30;
    }
    return 0;
}

s32 dds3EffectFldTypeIsSpecial(s32 type)
{
    switch (type) {
    case 2:
    case 5:
    case 7:
    case 9:
    case 10:
    case 30:
        return 1;
    }
    return 0;
}

void fldSetModelFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].modelFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].modelFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].modelFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].modelFlag &= ~(1 << bit);
    }
}

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

void fldSetAnimeFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].animeFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].animeFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].animeFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].animeFlag &= ~(1 << bit);
    }
}

s32 fldGetAnimeFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag >> bit) & 1;
}

void fldSetAtariFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].atariFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].atariFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].atariFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].atariFlag &= ~(1 << bit);
    }
}

s32 fldGbChkHit(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].atariFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].atariFlag >> bit) & 1;
}

void fldSetModelFlag2(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].modelFlag2 |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].modelFlag2 &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].modelFlag2 |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].modelFlag2 &= ~(1 << bit);
    }
}

s32 fldGetModelFlag2(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag2 >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag2 >> bit) & 1;
}

void fldSetAnimeFlag2(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].animeFlag2 |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].animeFlag2 &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].animeFlag2 |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].animeFlag2 &= ~(1 << bit);
    }
}

s32 fldGetAnimeFlag2(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag2 >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag2 >> bit) & 1;
}

void fldSetAnimeId2(s32 charId, s32 idx, s32 slot, s32 id)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        DDS3_GBWK->gb[type].flags[idx].animeId2[slot] = id;
        return;
    }
    type = dds3EffectFldCharToType(charId);
    DDS3_GBWK->gb[type].flags[idx].animeId2[slot] = id;
}

s32 fldGetAnimeId2(s32 charId, s32 idx, s32 slot)
{
    s32 type;
    u8 id;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return -1;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        id = DDS3_GBWK->gb[type].flags[idx].animeId2[slot];
    } else {
        type = dds3EffectFldCharToType(charId);
        id = DDS3_GBWK->gb[type].flags[idx].animeId2[slot];
    }
    return id != 0xFF ? id : -1;
}

void fldSetNpcFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].npcFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].npcFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].npcFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].npcFlag &= ~(1 << bit);
    }
}

s32 fldGbChkNpc(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].npcFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].npcFlag >> bit) & 1;
}

void fldGbSetGimic(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].hojiFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].hojiFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].hojiFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].hojiFlag &= ~(1 << bit);
    }
}

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

void fldSetTakaraOpen(s32 id)
{
    s32 idx;
    s32 bit;

    if (id >= 0x100) {
        idx = (id - 0x100) >> 3;
        bit = (id - 0x100) & 7;
        DDS3_GBWK->takaraOpen[idx + 0x28] |= 1 << bit;
        fldFindTakaraSlot(id);
        return;
    }
    idx = id >> 3;
    bit = id & 7;
    DDS3_GBWK->takaraOpen[idx] |= 1 << bit;
    fldFindTakaraSlot(id);
    if (id >= 0xF0 && id < 0x100) {
        evtBitOn(id + 0x610);
    }
}

s32 fldGetTakaraOpen(s32 id)
{
    s32 idx;
    s32 bit;

    if (id >= 0x100) {
        idx = (id - 0x100) >> 3;
        bit = (id - 0x100) & 7;
        return ((u32)DDS3_GBWK->takaraOpenEx[idx] >> bit) & 1;
    }
    idx = id >> 3;
    bit = id & 7;
    return ((u32)DDS3_GBWK->takaraOpen[idx] >> bit) & 1;
}

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

void fldDbgWmapStart(void)
{
    s32 field;

    if (DDS3_GBWK->transition != 0) {
        dds3GlobalWorkUserInit2(1);
    } else {
        dds3GlobalWorkUserInit2(0);
        if (fldWmapIsEnabled() == 0) {
            evtBitOn(0xAF2);
        }
    }
    if (dds3ConfigChanged() == 0) {
        dds3ConfigSaveBackup();
    } else {
        dds3ConfigRestoreBackup();
    }
    evtBitOn(0xC0F);
    fldRunDanteGunFlag.enabled = 1;
    field = 0x259;
    fldRunDanteGunFlag.counter = 0;
    fldSetNextField(0xB, 0x259);
    dds3AdminiCallSequence(7, &field, 4, 0);
}

void fldMainLoadStart(void) {
    fldSceneParam_t record;
    u32 mode;
    u32 *modep = &mode;

    if (slGetExitType() == 1) {
        return;
    }
    if (DDS3_GBWK->transition != 0) {
        mode = 1;
        dds3AdminiCallSequence(0x29, modep, 4, 0);
        return;
    }
    fldGbInit_System(1);
    fldRunDanteGunFlag.counter = 0;
    fldRunDanteGunFlag.enabled = 1;
    evtBitOn(0xC0F);
    fldSetNextField(0, 0);
    fldSetSceneParam2(&record, 1, 1, pointResName);
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

void fldInitAllValue(void)
{
    u8 *p = (u8 *)&fldSceneParam;

    SDF_VU0_STOREVECTOR_ZERO(p);
    SDF_VU0_STOREVECTOR_ZERO(p + 0x10);
    memset(fldGetUnitControl(), 0, 0xC);
    memset(fldGetFollowCameraControl(), 0, 0x7C);
}

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

void fldPlayerALoad(void)
{
    if (fldPlayerAnimData == NULL) {
        fldPlayerAnimData = sdfDevLoadFileEx(D_005316B0, &gfldPlayerPtr, &gfldPlayerSizeBk);
    }
}

void fldPlayerAFree(void)
{
    if (fldPlayerAnimData != NULL) {
        sdfMemFreeDirect(fldPlayerAnimData);
        fldPlayerAnimData = NULL;
    }
}

void fldJDataLoad(void)
{
    void *src;
    void *dst;

    if (fldGlobalBase.area == 11 || fldGlobalBase.area == 14) {
        if (evtBitCheck(0x8B0) != 0) {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316D0, &gfldPlayerPtr, &gfldPlayerSize);
        } else {
            mhfldPlayerFile = sdfDevLoadFileEx(D_005316F0, &gfldPlayerPtr, &gfldPlayerSize);
        }
        mdlPackDecode(2, 0, 0x101, gfldPlayerPtr, gfldPlayerSize);
    } else if (fldGlobalBase.area < 10) {
        mhfldPlayerFile = sdfDevLoadFileEx(D_00531710, &gfldPlayerPtr, &gfldPlayerSize);
        mdlPackDecode(2, 4, 0x101, gfldPlayerPtr, gfldPlayerSize);
    } else {
        fldPlayerALoad();
        gfldPlayerSize = gfldPlayerSizeBk;
        mhfldPlayerFile = sdfMemAllocDirect(gfldPlayerSize);
        src = sdfMemGetPtr(fldPlayerAnimData);
        dst = sdfMemGetPtr(mhfldPlayerFile);
        memcpy(dst, src, gfldPlayerSize);
        gfldPlayerPtr = dst;
        mdlPackDecode(2, 0, 0x101, dst, gfldPlayerSize);
        sdfMemFreeDelayedDirect(mhfldPlayerFile);
        mhfldPlayerFile = NULL;
    }
    if (fldGlobalBase.area == 21 || fldGlobalBase.area == 26 || fldGlobalBase.area >= 36) {
        mhfldAnaFile = sdfDevLoadFileEx(D_00531728, &gfldAnaPtr, &gfldAnaSize);
        mdlPackDecode(2, 5, 0x101, gfldAnaPtr, gfldAnaSize);
    }
    if (fldGlobalBase.area == 22) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531740, &gfldWavePtr, &gfldWaveSize);
        mdlPackDecode(2, 0x1B, 0x101, gfldWavePtr, gfldWaveSize);
    } else if (fldGlobalBase.area == 45) {
        mhfldWaveFile = sdfDevLoadFileEx(D_00531758, &gfldWavePtr, &gfldWaveSize);
        mdlPackDecode(2, 0x1B, 0x101, gfldWavePtr, gfldWaveSize);
    }
}

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

void fldRegistUnitPad(void)
{
    sdfVector4_t pos;
    fldUnitControl_t *ctrl;

    ctrl = fldGetUnitControl();
    if (ctrl->handle != NULL) {
        fldEncountSetFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(ctrl->handle, fldUnitPad);
    }
    fldGlobalBase.word5C = 4;
    if (fldGlobalBase.menuOpenFlag < 4) {
        fldGlobalBase.menuOpenFlag = 4;
    }
    if (fldGlobalBase.encountState < 5) {
        fldGlobalBase.encountState = 5;
    }
    fldStatusSet();
    fldGlobalBase.hitMatterIndex = -1;
    fldPlayerCalc_Hit();
    dds3GetPosPoseSet(ctrl->handle);
    SDF_VU0_STOREVECTOR_NOMEM(&pos);
    if (fldGlobalBase.area < 0xA) {
        pos.y = pos.y + 20.0f;
    } else {
        pos.y = pos.y + 10.0f;
    }
    fldHitChkUnit(&pos);
}

void fldReleaseUnitPad(void)
{
    fldUnitControl_t *ctl = fldGetUnitControl();

    if (ctl->handle != NULL) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(ctl->handle, 0);
    }
    fldEncountCheckCount = 4;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531690);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005316A0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005316B0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005316D0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005316F0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531710);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531728);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531740);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531758);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531770);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531780);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531790);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005317A0);

INCLUDE_RODATA(const s32, "code_00116AD0", D_005317B0);

INCLUDE_ASM(const s32, "code_00116AD0", fldSetUnit);

void fldAnaStart(f32 x, f32 y, f32 z)
{
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (mhfldAnaFile != NULL) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3SetPosPoseSet(fldAnaBasic, pos);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldAnaBasic), 0, 0);
        dds3ObjectBaseOffFlag(fldAnaBasic, 1);
        fldSePlay(0x26);
    }
}

void fldWaveInit(void)
{
    f32 pos[4];
    f32 rot[4];
    s32 i;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    memset(rot, 0, sizeof(rot));
    rot[3] = 1.0f;
    if (mhfldWaveFile == NULL) {
        return;
    }
    for (i = 0; i < 0x28; i++) {
        fldWaveBuff[i].active = 0;
        fldWaveBuff[i].count = 0;
        fldWaveBuff[i].handle = NULL;
        fldWaveBuff[i].handle = dds3AddUnitObjectBasic(dds3AllocObjectId(), pos, rot);
        dds3SetUnitObjectId(fldWaveBuff[i].handle, D_00531820);
        dds3ObjectBaseMakeUnit(fldWaveBuff[i].handle, 2, 0x1B);
        dds3ObjectBaseOnFlag(fldWaveBuff[i].handle, 1);
    }
}

void fldWaveStart(f32 x, f32 y, f32 z)
{
    f32 pos[4];
    s32 i;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (mhfldWaveFile == NULL) {
        return;
    }
    for (i = 0; i < 0x28; i++) {
        if (fldWaveBuff[i].active != 0) {
            continue;
        }
        fldWaveBuff[i].active = 1;
        fldWaveBuff[i].count = 0;
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        dds3SetPosPoseSet(fldWaveBuff[i].handle, pos);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].handle), 0, 0);
        dds3ObjectBaseOffFlag(fldWaveBuff[i].handle, 1);
        mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldWaveBuff[i].handle), 0, 0);
        return;
    }
}

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

void fldSetUnitFollowCamera(void)
{
    sdfVector4_t lookat;
    sdfVector4_t campos;
    sdfVector4_t tm;
    fldFollowCameraControl_t *ctrl;

    lookat = D_00531830;
    memset(&campos, 0, sizeof(campos));
    campos.w = 1.0f;
    tm = D_00531840;
    ctrl = &fldFollowCameraControl;
    ctrl->handle =
        dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), &campos, &tm, &lookat);
    dds3SetUnitObjectId(ctrl->handle, D_0054EB40);
    dds3CameraObjectBasicGetCameraTM(ctrl->handle, &tm);
    dds3GetRadiusPoseSet(ctrl->handle, 2.0f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), ctrl->handle);
}

s32 fldUnitPad(dds3PoseSet_t *pUpdatePoseSet, dds3Basic_t *pBasic)
{
    return 0;
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531820);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531830);

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531840);

s32 fldChkVortex(void)
{
    s32 list[3] = { 1, 11, 14 };
    u32 i;

    for (i = 0; i < 3; i++) {
        if (fldGlobalBase.area == list[i]) {
            return 0;
        }
    }
    if ((u32)(fldGlobalBase.area - 0x259) < 0x10) {
        return 0;
    }
    return 1;
}

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

void fldLightMaTask(void)
{
    if (gFldNowDZone == 0) {
        if (fldGlobalBase.lightMaIdx != 0) {
            fldGlobalBase.lightMaIdx = 0;
            fldSkySetTblLightMa(0, 0);
        }
    } else if (DDS3_GBWK->maFlags & 8) {
        if (fldGlobalBase.lightMaIdx == 0) {
            fldGlobalBase.lightMaIdx = fldLightMaIdxCalc();
            fldSkySetTblLightMa(fldGlobalBase.lightMaIdx, 0x14);
        }
    } else if (fldGlobalBase.lightMaIdx != 0) {
        fldGlobalBase.lightMaIdx = 0;
        fldSkySetTblLightMa(0, 0x14);
        fldSePlay(0x2A);
    }
}

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

/* NON_MATCHING: 10 of 30 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void fldAnahoriSelectInit(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    gFldAnahoriAkumaIdx = 0;
    gFldAnahoriAkumaIdxMax = 0;
    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp != 0) {
            n++;
        }
    }
    gFldAnahoriAkumaIdxMax = n;
}
#else
INCLUDE_ASM(const s32, "code_00116AD0", fldAnahoriSelectInit);
#endif

s32 fldAnahoriSelectGet(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    if (gFldAnahoriAkumaIdxMax == 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp == 0) {
            continue;
        }
        if (gFldAnahoriAkumaIdx == n) {
            return DDS3_GBWK->akuma[i].devilId;
        }
        n++;
    }
    return 0;
}

void fldAnahoriSelectNext(void)
{
    s32 idx = gFldAnahoriAkumaIdx + 1;

    if (idx >= gFldAnahoriAkumaIdxMax) {
        idx = 0;
    }
    gFldAnahoriAkumaIdx = idx;
}

void fldAnahoriSelectDel(void)
{
    s32 n = 0;
    s32 i;
    u32 flag;

    for (i = 0; i < 16; i++) {
        flag = DDS3_GBWK->akuma[i].flag;
        if (!(flag & 1)) {
            continue;
        }
        if (flag & 4) {
            continue;
        }
        if (flag & 0x10) {
            continue;
        }
        if (flag & 0x800) {
            continue;
        }
        if (DDS3_GBWK->akuma[i].hp == 0) {
            continue;
        }
        if (gFldAnahoriAkumaIdx == n) {
            if (DDS3_GBWK->akuma[i].level >= 0x15) {
                evtBitOn(0x504);
            }
            datDelDevil(DDS3_GBWK->akuma[i].devilId);
            return;
        }
        n++;
    }
}

s32 fldOtInit(void)
{
    if ((fldGlobalBase.area == 0x2A && fldGlobalBase.floor == 0xF) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0x8) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0x9) ||
        (fldGlobalBase.area == 0x2B && fldGlobalBase.floor == 0xA)) {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
        D_002F6A18[1] = &sdfGrOtTable[46];
    } else if (fldGlobalBase.area == 0x20 || fldGlobalBase.area == 0x24 ||
               fldGlobalBase.area == 0x25) {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
        D_002F6A18[1] = &sdfGrOtTable[35];
    } else {
        sdfModelOtInit(D_002F6A18, &sdfGrOtTable[34], &sdfGrOtTable[40], &sdfGrOtTable[42],
                       &sdfGrOtTable[41]);
    }
}

INCLUDE_RODATA(const s32, "code_00116AD0", D_00531860);

INCLUDE_ASM(const s32, "code_00116AD0", ProcRandomEncountCheck);

s32 fldProcKiretaChk(void)
{
    return scrSearchScriptName(D_00531860) != NULL;
}

s32 fldProcHirakenaiChk(void)
{
    return scrSearchScriptName("hirakenai_label") != NULL;
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

s32 ProcCampCheck(void)
{
    if (fldChkVortex() == 0) {
        return 0;
    }
    if (fldProcCampWait > 0) {
        fldProcCampWait = fldProcCampWait - 1;
        if (fldProcCampWait == 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3FadeIn2(4);
            cmpProcessStart();
        }
        return 0;
    }
    if (fldEncountTestFlag(0x20) == 0) {
        if (fldProcCampChk() == 3) {
            fldAutoMapSeqClearFlag(1);
            fldRegistUnitPad();
            evtEnableMoonDraw();
            if (fldLightMaState != 0) {
                fldSkyRefreshTbl();
            } else {
                fldSkyRefreshTbl();
            }
            fldStatusSet();
            dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
            fldEncountSetFlag(0x20);
            fldEncountClearFlag(0x80);
            if (cmpGetLoadState() == 1) {
                dds3FadeOut2(0);
                fldEveHit_End();
                fldAutoMapSeqSetFlag(1);
                fldAutoMapSeqSetFlag(2);
                fldReleaseUnitPad();
                evtKillFieldScript();
                p2sdStopAllSe();
                dds3AdminiCallSequence(4, NULL, 0, 0);
                return 0;
            }
            dds3FadeOut2(0);
            dds3FadeIn2(8);
            return 0;
        }
    }
    if (fldEncountTestFlag(0x20) == 0) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (fldAutoMapSeqTestFlag(1) == 1) {
        return 0;
    }
    if (fldPlayerChkAna() != 0) {
        return 0;
    }
    if (fldAutoMapChkMode() != 0) {
        return 0;
    }
    if (fldGlobalBase.menuOpenFlag != 0) {
        goto ret0;
    }
    if (((s8 *)dds3PadButtonState)[0x20] >= 0) {
        goto ret0;
    }
    p2sdPlaySE(0xE, 0x7F, 0x3F);
    fldAutoMapSeqSetFlag(1);
    fldReleaseUnitPad();
    fldPlayerPosRot();
    evtDisableMoonDraw();
    fldGlobalBase.menuOpenFlag = 4;
    fldGlobalBase.encountState = 5;
    fldEncountClearFlag(0x20);
    fldEncountSetFlag(0x80);
    dds3FadeOut(0, 0, 0, 4);
    fldProcCampWait = 5;
    fldPlayerMotReset();
ret0:
    return 0;
}

s32 fldProcAmChk(void)
{
    if (fldProcAmWait > 0) {
        return 2;
    }
    return fldAutoMapChkMode() != 0;
}

s32 ProcAmCheck(void)
{
    s32 trigger = 0;

    if (fldGlobalBase.area < 0xA) {
        return 0;
    }
    if (fldProcCampChk() != 0) {
        return 0;
    }
    if (fldProcAmChk() == 0) {
        if (fldEncountTestFlag(0x40) == 0) {
            return 0;
        }
    }
    if ((fldRunDanteGunFlag.unk58 & fldEncountTestFlag(0x40)) != 0) {
        if (((s8 *)dds3PadButtonState)[0x22] != 0) {
            trigger = 1;
        }
    } else {
        if (((s8 *)dds3PadButtonState)[0x22] < 0) {
            trigger = 1;
        } else {
            trigger = 0;
        }
    }
    if (fldGlobalBase.area == 0x26 ||
        (fldGlobalBase.area == 0x2C && fldGlobalBase.floor + 1 >= 0xC &&
         fldGlobalBase.floor + 1 < 0x1C) ||
        (fldGlobalBase.area == 0x2C && fldGlobalBase.floor == 0x1E) ||
        (fldGlobalBase.area == 0x2D && fldGlobalBase.floor == 0x1D) ||
        (fldGlobalBase.area == 0x2D && fldGlobalBase.floor == 0x1E)) {
        if (fldGlobalBase.menuOpenFlag != 0) {
            goto ret0;
        }
        if (fldEncountTestFlag(0x40) == 0) {
            goto ret0;
        }
        if (trigger != 0) {
            fldGlobalBase.menuOpenFlag = 4;
            fldGlobalBase.encountState = 5;
            fldReleaseUnitPad();
            dds3SceneBasicStartScript(dds3GetActiveScene(), "hirakenai_label");
        }
        goto ret0;
    }
    if (fldProcAmWait > 0) {
        fldProcAmWait = fldProcAmWait - 1;
        if (fldProcAmWait == 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3FadeIn2(4);
            fldAutoMapSeqStart();
        }
        goto ret0;
    }
    if (fldProcAmChk() != 0) {
        goto ret0;
    }
    if (fldGlobalBase.menuOpenFlag != 0) {
        goto ret0;
    }
    if (fldPlayerChkAna() != 0) {
        goto ret0;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        goto ret0;
    }
    if (trigger != 0) {
        if (fldRunDanteGunFlag.unk48 != 0) {
            goto ret0;
        }
        fldGlobalBase.menuOpenFlag = 4;
        fldGlobalBase.encountState = 5;
        fldReleaseUnitPad();
        fldPlayerPosRot();
        dds3FadeOut(0, 0, 0, 4);
        fldProcAmWait = 5;
        fldPlayerMotReset();
    }
    ret0:
    return 0;
}

s32 RetryMapLoad(void)
{
    fldSceneParam_t *param = &fldSceneParam;
    s32 flag;

    if ((fldDoorExitFlag & 2) && (s8)fldDoorExitMode[0] != 0) {
        dds3SceneBasicStartScript(dds3GetControlScene(), (const char *)fldDoorExitMode);
        flag = fldDoorExitFlag;
        if ((flag & 1) == 0) {
            fldDoorExitMode[0] = 0;
            fldDoorExitFlag = flag & 0xFD;
        }
        return 1;
    }
    if (param->mode == 0 && param->note[0] != 0) {
        dds3SceneBasicStartScript(dds3GetControlScene(), param->note);
        return 1;
    }
    if (fldGlobalBase.retryScript == NULL) {
        goto ret0;
    }
    dds3SceneBasicStartScript(dds3GetControlScene(), fldGlobalBase.retryScript);
    return 1;
ret0:
    return 0;
}


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
    dds3StartProcess("fldProcSequence", 0x3F7, 0, 0, ProcSequence, 0, 0);
    dds3StartProcess(D_00531950, 0x2B0A, 0, 0, ProcDraw, 0, 0);
    fldProcShutDownFlg = 1;
    InitOoyTest();
}

void fldProcShutDown(void)
{
    if (fldProcShutDownFlg == 0) {
        return;
    }
    fldProcShutDownFlg = 0;
    fldFreeSnowSeed();
    fldEncountClearFlag(0x10);
    fldEncountClearFlag(0x20);
    fldEncountClearFlag(0x80);
    fldAutoMapSeqSetFlag(1);
    fldAutoMapSeqSetFlag(2);
    dds3KillProcessName("fldProcSequence", 1);
    dds3KillProcessName(D_00531950, 1);
    fldFreeInputPanel();
    evtDisableMoonDraw();
    fldFreeAddressProcess();
    if (hDistortionBlur != NULL) {
        dds3EffectDistortionBlurDelete(hDistortionBlur);
        hDistortionBlur = NULL;
    }
    campKillProcesses();
    scrAllReleaseScript();
}

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

INCLUDE_SDATA(const s32, "code_00116AD0", pointResName);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB38);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB40);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldAnahoriAkumaIdx);

INCLUDE_SDATA(const s32, "code_00116AD0", gFldAnahoriAkumaIdxMax);

INCLUDE_SDATA(const s32, "code_00116AD0", fldNextFieldId);

INCLUDE_SDATA(const s32, "code_00116AD0", D_0054EB54);

INCLUDE_SDATA(const s32, "code_00116AD0", fldEncountSagasiFlag);

INCLUDE_SDATA(const s32, "code_00116AD0", hDistortionBlur);

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

