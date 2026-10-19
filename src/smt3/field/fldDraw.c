#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldSpriteCorner_t fldSpriteCorner_t;
typedef struct fldSpriteVertex_t fldSpriteVertex_t;
typedef struct fldLightSet_t fldLightSet_t;
typedef struct fldCollisionFace_t fldCollisionFace_t;
typedef struct fldValueRecord_t fldValueRecord_t;
typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;
typedef struct fldWallMeshRecord_t fldWallMeshRecord_t;
typedef struct fldTriggerSphere_t fldTriggerSphere_t;
typedef struct fldTriggerPlane_t fldTriggerPlane_t;
typedef struct fldTriggerBox_t fldTriggerBox_t;
typedef union fldTriggerShape_t fldTriggerShape_t;
typedef struct fldTaskInfo_t fldTaskInfo_t;
typedef struct fldRoomState_t fldRoomState_t;
typedef struct fldInfHit_t fldInfHit_t;
typedef struct fldInfPack_t fldInfPack_t;
typedef struct fldInfView_t fldInfView_t;
typedef struct fldInfExtraAction_t fldInfExtraAction_t;
typedef struct fldInfFlagSelector_t fldInfFlagSelector_t;
typedef struct fldInfMessageRow_t fldInfMessageRow_t;
typedef struct fldInfSet_t fldInfSet_t;
typedef struct fldInfTable_t fldInfTable_t;
typedef struct fldActorEntry_t fldActorEntry_t;
typedef union fldWaypointHeaderValues_t fldWaypointHeaderValues_t;
typedef struct fldWaypointHeader_t fldWaypointHeader_t;
typedef struct fldWaypointBlock_t fldWaypointBlock_t;
typedef struct fldFollowCameraControl_t fldFollowCameraControl_t;

typedef struct TMX_HEADER_t TMX_HEADER_t;

struct fldSpriteCorner_t {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
};

struct fldSpriteVertex_t {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    fldSpriteCorner_t corner[2];
};

typedef struct sdfTexBuf_t sdfTexBuf_t;

typedef struct dmaPacketHeader_t dmaPacketHeader_t;

typedef struct frFontGlyph_t frFontGlyph_t;

typedef struct {
    s32 px;
    s32 py;
    s32 sx;
    s32 sy;
    u32 pz;
    u32 panelCol;
    u32 frameCol;
    s32 cx;
    s32 cy;
    s32 cz;
    sdfDrawTag_t *tag;
} dbgConsCtl_t;


struct fldLightSet_t {
    u8 fadeArea;
    u8 pad1[3];
    s32 displayStateSlot14;
    s32 fadeMode;
    s32 swayMode;
    u8 pad10[0xC];
    s32 fogNearValue;
    s32 depthNearValue;
    s32 fogFarValue;
    s32 depthFarValue;
    s32 drawColorFixedR;
    s32 drawColorFixedG;
    s32 drawColorFixedB;
    f32 lightColorAX;
    f32 lightColorAY;
    f32 lightColorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightColorBX;
    f32 lightColorBY;
    f32 lightColorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightColorCX;
    f32 lightColorCY;
    f32 lightColorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 backgroundColorX;
    f32 backgroundColorY;
    f32 backgroundColorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
};

enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

struct fldCollisionFace_t {
    u32 attributes;
    u8 moveFloor;
    u8 sound;
    u16 stop;
    u16 place;
    u8 automap[2];
    u32 vertexIndices[4];
    s16 encounterType;
    s16 encounter;
    s16 special[2];
};

struct fldValueRecord_t {
    s16 mode;
    s16 count;
    f32 normal[4];
    f32 planeConstant;
    f32 plane[4][4];
    f32 limit[4];
    f32 bound[4];
    f32 vertices[4][4];
    f32 previousPosition[3];
    u32 unkC4;
    u16 unkC8;
    u16 unkCA;
    s32 id;
    dds3Basic_t *worldNode;
    fldCollisionFace_t *face;
    void *stopData;
    s32 sceneFlag;
    s32 ownerIndex;
};

struct fldCollisionDescriptor_t {
    s32 vertexCount;
    s32 faceCount;
    s32 triangleCount;
    f32 (*vertices)[4];
    fldCollisionFace_t *faces;
    void *stopData;
};

struct fldWallMeshRecord_t {
    f32 vertices[8][4];
    fldCollisionFace_t faces[4];
    fldCollisionDescriptor_t geometry;
};


struct fldTriggerSphere_t {
    u32 kind;
    f32 radius;
};

struct fldTriggerPlane_t {
    u32 kind;
    f32 width;
    f32 height;
};

struct fldTriggerBox_t {
    u32 kind;
    f32 size[3];
};

union fldTriggerShape_t {
    u32 kind;
    fldTriggerSphere_t sphere;
    fldTriggerPlane_t plane;
    fldTriggerBox_t box;
};

struct fldTaskInfo_t {
    s32 unk0;
    s32 slot;
    u32 unk08;
    fldTriggerShape_t *shape;
};

struct fldRoomState_t {
    f32 corner[FIELD_ROOM_CORNER_COUNT][4];
    f32 center[4];
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];
    f32 limit[FIELD_ROOM_PLANE_COUNT];
    s32 worldObjectKey;
    f32 unk10C;
    s32 unk110;
    f32 unk114;
    s32 unk118;
    s32 unk11C;
    s16 unk120;
    s16 roomId;
    s16 unk124;
    s16 mode;
    s16 entryIndex;
    s16 axisMode;
    s32 shapeKind;
};

/* The event-range box block at fldRoomState_t + 0x120, reached through its own
   pointer so the offset folds into the symbol address. */
typedef struct {
    s16 unk00;
    s16 takaraIndex;
    s16 unk04;
    s16 hitKind;
    s16 npcIndex;
    s16 unk0A;
    s32 unk0C;
} fldEveRngBox_t;

/* The plane block of fldRoomState_t, reached through its own pointer so the
   0x90 offset folds into the symbol address. */
typedef struct {
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];
    f32 limit[FIELD_ROOM_PLANE_COUNT];
} fldRoomPlanes_t;

/* The same block based at plane[1], the first of the four side planes; the
   room's own plane[0] and its distance sit before limit[]. */
typedef struct {
    f32 plane[FIELD_ROOM_PLANE_COUNT - 1][4];
    f32 firstLimit;
    f32 limit[FIELD_ROOM_PLANE_COUNT - 1];
} fldRoomSidePlanes_t;

struct fldInfHit_t {
    s8 area;
    char eventName[15];
};

struct fldInfPack_t {
    s32 setIndex;
    fldInfHit_t hits[5];
};

struct fldInfView_t {
    s32 player;
    s32 motion;
    char positionName[12];
    char cameraName[12];
};

struct fldInfExtraAction_t {
    s32 action;
    s32 parameter0;
    s32 parameter1;
    s32 parameter2;
    s32 parameter3;
};

struct fldInfFlagSelector_t {
    s16 flag;
    u8 offTarget;
    u8 onTarget;
};

struct fldInfMessageRow_t {
    s16 kind;
    s16 message;
    u8 targets[4];
    s16 flags[2];
    u8 view;
    u8 extraAction;
};

struct fldInfSet_t {
    union {
        s16 packed;
        struct {
            u8 kind;
            u8 area;
        } bytes;
    } kindArea;
    s8 action;
    u8 eventHit;
    char eventName[12];
    fldInfFlagSelector_t flags[4];
    fldInfMessageRow_t messages[20];
};

struct fldInfTable_t {
    fldInfPack_t packs[8];
    fldInfView_t views[40];
    fldInfExtraAction_t extraActions[40];
    fldInfSet_t sets[40];
};

typedef struct devState_t devState_t;

struct fldActorEntry_t {
    s8 kind;
    u8 pad01;
    s16 requiredFlag;
    s16 floor;
    char name[0xC];
    s16 motion;
    s16 secondaryMotion;
    s16 sound;
    char motionName[0xC];
    char otherName[0xC];
    s8 variantMode;
    s8 flags31;
    s16 variant;
    s16 sequenceKind;
    s16 sequenceValue;
    char sequenceName[0xC];
    s8 linkKind;
    s8 rowIndex;
    char linkName[0xC];
    s8 sequenceCode;
    s8 selectedRoom;
    s8 flags54;
    char taskName[0xF];
};

enum { FLD_WAYPOINT_HEADER_COUNT = 5, FLD_WAYPOINT_BLOCK_BYTES = 0x64A0 };

union fldWaypointHeaderValues_t {
    s16 data[12];
    struct {
        s16 floors[6];
        s16 blocks[6];
    } elevator;
};

struct fldWaypointHeader_t {
    s32 area;
    s16 sound;
    s16 floorCount;
    fldWaypointHeaderValues_t body;
};

struct fldWaypointBlock_t {
    fldWaypointHeader_t headers[FLD_WAYPOINT_HEADER_COUNT];
    fldActorEntry_t actors[256];
};

/* fldGetFollowCameraControl owns the follow-camera basic at +0x78. */
struct fldFollowCameraControl_t {
    u8 unk00[0x78];
    dds3Basic_t *camera;
};


sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *header);

extern u32 D_0054EC44;

extern u32 fldDrawEnv;

extern sdfTexHandle_t *D_0054EC60;

extern u32 D_00551C9C;

extern u8 sdfTexEmbedded_2FC4A0[];

extern sdfMat_t *sdfMatNew(void);


s32 sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 contextOffset);

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 loopCount);

void sdfDrawTagInit(sdfDrawTag_t *list);

void sdfDrawTagLink(sdfDrawTag_t *list, u32 packetAddress);

s32 sdfGrAllocPacketBuff(s32 size);

extern s32 sdf2DGetPrimPtr(s32);

void sdfPrimLinkGt4(
    sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
    s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocatePacket)(s32));

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

void MultMatrixRev(const void *matrix);

extern u8 D_002FA240[];

extern sdf3DScreenParam_t dds3ScreenParam;
extern sdf3DScreenParam_t dds3ScreenParam2;

extern u8 aProjMatrix[];

extern u32 sdf2DMakeStdPacket(u32, s32);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

struct sdfTexBuf_t *sdfTexGetPacket(sdfTexHandle_t *texture);

s32 sdfTexGetPacketLength(const sdfTexHandle_t *texture);

void SetRotateMatrixZ(f32 angle);

void sdfDrawTagLinkRef(sdfDrawTag_t *list, u32 packetAddress);

extern void sdf3DMakeDirectRefPacket(dmaPacketHeader_t *, u32, s32);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const sdfVector4_t *, u32, const UV_t *, const VTX_t *);

sdfGrPacketHeader_t *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

extern f32 D_002FC720[][4];

extern u32 D_002FC740[];

void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

extern void *memset(void *, int, unsigned int);


void sdfFreeAutoDelayed(void *data);

void sdfLinkStoreImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

extern sdfGrFrameSet_t D_00529388;

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

struct frFontGlyph_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 depth,
    u32 colors, const u8 *text, struct frFontGlyph_t *parent);

s32 frDestroyFontRequestAfter(struct frFontGlyph_t *glyph);

s32 frDrawFont(frFontGlyph_t *glyph);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

void sdfDevSetPrintCB(sifCommand_t *packet, s32 source, s32 end, s32 argument, s32 index);

void *sdfDevPrintfEx(void *packet, const char *format, ...);

extern u8 D_0054EC50[];

extern void InversMatrix(void);

extern u8 D_0054EC58[];

f32 sdfSin(f32 angle);

f32 sdfCos(f32 angle);


f32 sdfAtan2(f32 y, f32 x);


fldFollowCameraControl_t *fldGetFollowCameraControl(void);

void dds3SetPosPoseSet(dds3Basic_t *object, u128 *vector);


extern f32 fldCamPhokanSelX[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

extern f32 fldCamPhokanEyeX[];

extern f32 fldCamPhokanTgtX[];

extern f32 D_002FC9C0[];

extern f32 fldCamTarget[];

dds3Basic_t *fldGetUnitFollowCamera(void);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

dds3Basic_t *dds3GetActiveScene(void);


extern f32 fldCamElvState;

extern f32 D_0055DB20[];

extern f32 D_0055DB10[];

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *, u32);


extern f32 R360_LR(f32 fromAngle, f32 toAngle);

extern f32 fabsf(f32);


extern u32 fldRunDanteGunFlag[];
extern s32 datCheckDying(datUnitWork_t *unit);


extern void dds3ObjectBaseOffFlag(void *object, s32 mask);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern u32 D_0054EAF0;
extern u32 fldPlayerUnit;
extern f32 D_002FCA60[];
extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);
extern void evtUnitLightOn(evtUnit_t *unit);
extern void evtSetUnitLightAndAmbient(evtUnit_t *unit, s32 duration, u32 firstColor, u32 secondColor);
extern void evtSetUnitLightDir(evtUnit_t *unit, s32 duration);
extern s32 gencStageInnAlp;
extern s32 encDrawState;
extern dds3Basic_t *D_0054EAE8;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *spawnedObject);

extern u32 fldSkyTable;

extern u32 fldSkyTableIdx;

extern u32 fldSkyLightMaTbl;

extern u32 D_002FCAC0[];

extern u32 D_002FCAA0[];

extern f32 fldSkyPlayerIdx[];

extern void *fldSkyDefaultData;

/* fldSkyDataTable stride, 292 bytes; only the filter mode at offset 0 is used here. */
typedef struct {
    s32 mode;
    s32 colors[8][9];
} fldSkyData_t;

extern fldSkyData_t *fldSkyDataTable;
extern s32 fldAutoMapChkMode(void);
extern void *fldSkyDefault;
extern s32 fldSkyTblUpperIdx;
extern s32 D_0054ED5C;
extern f32 D_0054ED60;
extern s32 D_0054ED64;
extern s32 D_0054ED68;
extern s32 D_0054ED6C;
extern f32 D_0054ED70;
extern f32 D_0054ED74;
extern s32 D_0054ED7C;
extern f32 D_0054ED80;
extern s32 D_0054ED84;
extern s32 D_0054ED8C;

extern void fldSkySetPlayer(s32 duration, f32 colorAX, f32 colorAY, f32 colorAZ,
                            f32 colorBX, f32 colorBY, f32 colorBZ,
                            f32 dirX, f32 dirY, f32 dirZ);
extern s32 fldSkyEnable;
extern s32 fldSkyFilter;
extern s32 fldSkyFilterPrev;
extern f32 fldSkyFilterRate;
extern f32 fldSkyFilterTime;
extern s32 fldLightMaState __attribute__((section(".data")));
extern s32 fldGb __attribute__((section(".data")));

extern void dds3SetFogColor(s32, f32 *);

extern s32 dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern s32 dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

u32 sdfMemLock(sdfMemHandle_t *allocation);

extern u32 fldItemBoxObjBuf;

extern u32 fldMatterTable;

extern u32 D_0054EDA8;

extern u32 D_0054EDAC;

void sdfMemUnlock(sdfMemHandle_t *allocation);

void sdfMemFreeDelayedDirect(sdfMemHandle_t *allocation);

extern s32 D_0054EDA0;

extern s32 fldPlaneCount;

u32 dds3AllocObjectId(void);

extern s32 fldItemBoxObjCount;

void *dds3EventBasicGetResource(dds3Basic_t *object);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

dds3Basic_t *dds3GetControlScene(void);

void dds3SceneBasicEndScript(void *unusedContext, const char *taskName);

/* Common prefix of the event payloads registered by fldEveHit_AddEveData.
 * The scene lookup consumes the object ID, not the remaining payload. */
typedef struct {
    u32 objectId;
} fldEventDataHeader_t;

extern fldEventDataHeader_t *fldEveHitDataTable[];

extern u32 gEvnRngPid[];

extern scrProcessWork_s *scrSearchScriptName(char *name);

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern fldFileResource_t *gEvnRng[];

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_T;

extern s32 fldEveHitState;

extern u32 gEveNum;

s32 dds3ExistProcess(dds3ProcessID_t *task);

s32 dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

s32 dds3SceneBasicStartScript(dds3Basic_t *worldNode, const char *taskName);

extern s8 fldDoorExitMode[];

extern u8 *fldDoor_Find3(void);

extern u8 fldDoorExitFlag;

extern fldRoomState_t fldEveHitTable[];

extern s32 strcmp(const char *left, const char *right);

extern fldInfTable_t fldInfTable;

extern s32 gFldInfIdx;

extern const char D_002FF3E0[];

void fldMakeFilePath(char *buffer, s32 area, s32 unused);

devState_t *sdfFileOpen(const char *path);

void sdfFileRead(devState_t *state, void *buffer, s32 byteCount);

void sdfFileClose(devState_t *state);

extern s32 sprintf(char *, const char *, ...);

extern const char D_00531E98[];

extern void *memcpy(void *, const void *, unsigned int);

extern s32 gFldInf_HitIdx;

extern s32 gFldInf_WpExit;

extern u8 *fldDoor_Find2(const char *);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *world, s32 slotIndex, const u8 *name);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *world, dds3Basic_t *camera);

s32 evtBitCheck(s32 flag);

extern fldWaypointBlock_t fldDoorParamTable;

void fldPlayerOff(void);


s32 fldLoadNextFile(s32 area, s32 room);

extern u32 gFld_ElvTakasa;

extern s32 fldDoorWarpTarget;


/* fldDoorTable is fldDoorParamTable.actors: the same 100-byte waypoint record,
   reached through its own symbol at fldDoorParamTable + 0xA0. */
extern fldActorEntry_t fldDoorTable[256];

/* The 0xA0 scene request block dds3AdminiCallSequence(6, ...) takes. */
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

extern void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                              s32 code, s32 link, const char *subname);
extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 size, s32 mode);
extern s32 D_0054EDF4;
extern s32 fldDoorSiseState;
extern s32 D_0054EDFC;
extern s32 D_0054EE00;
extern s32 gFldNowDZone;
void fldDoorLoad(s32 field);
extern s32 scriptlabel;
extern void *fldDrwZBuf;
extern f32 fldWmFastMove;
extern void *hPartialMultiBlur;
extern s32 gfldPlayerHasiCnt;
extern s32 gfldPlayerDamegeCnt;
extern s32 fldPlayerStepDist;
extern s32 gSkyTblAnime;
extern f32 gSkyTblAnimeCntI;
extern s32 gSkyTblBaseIdx;
extern s32 D_002FC0EC __attribute__((section(".data")));
extern char D_0054EC80[];

extern void *sdfAllocAuto(u32 size);
extern void sdfFreeAutoDelayed(void *p);
extern void dds3EffectPartialMultiBlurDelete(void *p);
extern void fldSkySetTbl(s32 idx, s32 flag);
extern s32 encDrawProc(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 flag, dds3ProcessFunc_t func, void *arg,
                                         void *parent);


extern sdfGrOt_t sdfGrOtTable[85];
extern s32 gencOn;
extern s32 gencMode;
extern s32 rsetFrameMaskOtOffset;
extern s32 gFilterAlpha;
extern s32 gFilterAnime;
extern f32 gFilterAnimeCntF;
extern s32 gFilterAnimeOfsA;
extern s32 fldCamResetOn;
extern s32 fldCamMiwatasi;
extern s32 fldElvCameraScene;
extern s32 fldWmWarpMode;
extern s32 gFldWapPanel;
extern s32 fldSkyAlphaOfs __attribute__((section(".data")));

extern void fldCamSetChaseCore(f32 height);

/* fldUnitControl_t as fldGetUnitControl returns it. */
typedef struct {
    dds3Basic_t *handle;
    dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

extern fldUnitControl_t *fldGetUnitControl(void);
extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *basic);
extern void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *basic, u32 value);
extern dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *scene);
extern dds3ObjectBaseControl_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *basic);
extern void fldPlayerMotion(s32 motion, s32 mode, f32 speed);
extern void fldPlayerMotReset2(void);
extern f32 fldCamLength;
extern s32 gFldKyouseiTalk;
extern s32 fldArea[];
extern u8 D_00303020[];
extern s32 gFldWap_after_nextidx;
extern s32 fldPlayerFastMode;
extern s32 l_UseCollision;
extern void fldHitChkUnitElv(f32 *pos);
extern void dds3SetSclPoseSet(dds3Basic_t *basic, f32 *scl);
extern f32 fldWmapPlayerState;
/* The world-map player scale triple, copied as one word-aligned 16-byte block. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} fldScaleVec_t;

extern fldScaleVec_t D_00531D08;
extern fldScaleVec_t D_00531D18;
extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *object, s32 mask);
extern void dds3GetPosPoseSet(dds3Basic_t *object);

/* gFldEveHitOnOffTbl stride, 20 bytes. */
typedef struct {
    s16 area;
    s16 floor;
    s16 flag;
    char name[14];
} fldEveHitSwitch_t;

extern fldEveHitSwitch_t gFldEveHitOnOffTbl[32];

/* fldDoorEventTable stride, 84 bytes; the clear leaves +0x20 alone. */
typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
} fldDoorEvent_t;

extern fldDoorEvent_t fldDoorEventTable[256];
extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct {
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} fldDrawBank_t;

extern fldDrawBank_t dds3DrawEnv[];
extern u32 dds3GetFrameID(void);
extern void sdfGrSetFrameMaskRGB(const u8 *source, void *packet, s32 gsContext);
extern void sdfGrSetFrameMaskReset(const u8 *source, void *packet, s32 gsContext);
extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress);
extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, void *node);

void fldDrawInit(void) {
    if (D_0054EC44 == 0) {
        sdfMat_t *object;
        D_0054EC44 = 1;
        object = sdfMatNew();
        fldDrawEnv = (u32)object;
        object->constant = 1.0f;
        D_00551C9C = (u32)sdfMatNew();
        D_0054EC60 = sdfTexSet((TMX_HEADER_t *)(sdfTexEmbedded_2FC4A0));
    }
}

sdfGrOt_t *fldDrawGetOt(void)
{
    return &sdfGrOtTable[rsetFrameMaskOtOffset];
}


void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = color & 0xFF;
    vtx->g = (color >> 8) & 0xFF;
    vtx->b = (color >> 16) & 0xFF;
    vtx->a = (color >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, 0);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void DrwSprite2DC(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture) {
    void *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)handle);
    s32 r = color & 0xFF;
    s32 g = (color >> 8) & 0xFF;
    s32 b = (color >> 16) & 0xFF;
    s32 a = (color >> 24) & 0xFF;
    s32 ubase = u * 16;
    s32 vbase = v * 16;
    s32 xl = (s32)(x * 16.0f) + 0x7000;
    s32 yt = (s32)(y * 8.0f) + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = r;
    vtx->g = g;
    vtx->b = b;
    vtx->a = a;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + (s32)(w * 16.0f);
    vtx->corner[1].y = yt + (s32)(h * 8.0f);
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, texture, 0);
    sdfDrawTagLink(command, (u32)handle);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void fldPoint3To2(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADMATRIX(aProjMatrix);
    MultMatrixRev(&dds3ScreenParam2.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam2.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam2.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2_A(void) {
    SDF_VU0_LOADMATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
}

void fldPoint3To2_B(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(result);
    *dstX = result[0];
    *dstY = result[1];
}

extern void rsetOtOfs(s32 ofs);

/* NON_MATCHING: exact on its own; its presence turns two bnel of encDrawProc
   into bne. */
#ifdef NON_MATCHING
void rsetOtOfs(s32 ofs)
{
    rsetFrameMaskOtOffset = ofs;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", rsetOtOfs);
#endif


void rsetTexa(s32 lower, s32 bits, u64 upper) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *entry;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    entry = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    data[4] = (bit17 << 17) | 0x10000 | (bit15 << 15) | (bit14 << 14) | (bit12 << 12) | (bit4 << 4) | (bit1 << 1) | bit0;
    data[5] = 0x47;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetAlpha(s32 mode) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = (u64 *)sdf2DMakeStdPacket(packet, 0x30);
    switch (mode) {
    case 1:
        data[4] = 0x48;
        break;
    case 2:
        data[4] = 0x42;
        break;
    case 3:
        data[4] = 0x84;
        break;
    case 4:
        data[4] = 0x06;
        break;
    case 5:
        data[4] = 0x89;
        break;
    case 6:
        data[4] = 0x42;
        break;
    case 10:
        data[4] = 0x2A;
        break;
    case 20:
        data[4] = 0x4A;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    s32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2) {
    u32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    sdfGrOt_t *descriptor;
    u32 *pos;
    u64 lo;
    u64 hi;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y1;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    tag = vertexTag;
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF_Direct(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3) {
    u32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y0;
    coords[4] = x1;
    coords[5] = y1;
    coords[6] = x0;
    coords[7] = y1;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFFFFULL;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    sdfGrOt_t *descriptor;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    coords[4] = x2 * 16;
    coords[5] = y2 * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(coords[i * 2] + 0x7000) |
            ((u64)(coords[i * 2 + 1] + 0x7900) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxGF(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

    coords[0] = (s32)(x * 16.0f);
    coords[1] = (s32)(y * 8.0f);
    coords[2] = (s32)((x + w) * 16.0f);
    coords[3] = (s32)(y * 8.0f);
    coords[4] = (s32)((x + w) * 16.0f);
    coords[5] = (s32)((y + h) * 8.0f);
    coords[6] = (s32)(x * 16.0f);
    coords[7] = (s32)((y + h) * 8.0f);
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void func_0012A140(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr((s32)packet);
    lo = (u64)gsWord0 | ((u64)gsWord1 << 32);
    hi = (u64)gsWord2 | ((u64)gsWord3 << 32);
    tag = vertexTag;
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void drawSprite3D(sdfTexHandle_t *texture, Sprite3D_t *sprite) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    dmaPacketHeader_t *header;
    u8 *packet;
    f32 mat[16];
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    header = (dmaPacketHeader_t *)sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(header, (u32)sdfTexGetPacket(texture), sdfTexGetPacketLength(texture));
    sdfDrawTagLinkRef(command, (u32)header);
    SetRotateMatrixZ(sprite->rot);
    SDF_VU0_STOREMATRIX(mat);
    packet = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(command, (u32)packet);
    packet = (u8 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, &sprite->pos, sprite->col, sprite->uv, sprite->vt);
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetFrameMaskRgbOn(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *texture;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    texture = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskRGB(dds3DrawEnv[dds3GetFrameID()].frameTex, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetFrameMaskRgbOff(void) {
    sdfDrawTag_t *command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    void *texture;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    texture = (void *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskReset(dds3DrawEnv[dds3GetFrameID()].frameTex, texture, 0);
    sdfDrawTagLinkRefGif(command, (u32)texture);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    sdfDrawTag_t *list;
    s32 record;
    sdfGrOt_t *descriptor;
    D_002FC720[0][0] = x;
    D_002FC720[0][1] = y;
    D_002FC720[0][2] = z;
    D_002FC720[1][0] = u;
    D_002FC720[1][1] = v;
    D_002FC720[1][2] = w;
    D_002FC740[1] = second;
    D_002FC740[0] = first;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    record = (s32)effDebugDrawWireModel((const f32 (*)[4])D_002FC720, D_002FC740, 2, 0x80);
    sdfDrawTagLink(list, record);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, list);
}

void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
                         f32 x0, f32 y0, f32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[3];
    u32 colors[3];
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, (sdfMat_t *)fldDrawEnv, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 1;
    desc.pointCount = 3;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = z0;
    vertices[1].x = x1;
    vertices[1].y = y1;
    vertices[1].z = z1;
    vertices[2].x = x2;
    vertices[2].y = y2;
    vertices[2].z = z2;
    colors[0] = color0;
    colors[1] = color1;
    colors[2] = color2;
    sdfDrawTagLink(command, (u32)sdf3DPrimMakePoly(&desc));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawPoly4);
INCLUDE_ASM(const s32, "field/fldDraw", fldDrawTexPoly4);

void fldDrawFrameSprite(void) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x44;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = 0x30;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF7;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0x3FFF;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = 0x9009;
    vertex->corner[1].y = 0x8705;
    vertex->corner[1].mask = 0x3FFF;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawBokasi(void) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x8000000064ULL;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x81;
    vertex->g = 0x81;
    vertex->b = 0x81;
    vertex->a = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawFrameSpriteAlpha(s32 alpha) {
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    void *reference = (void *)sdfGrAllocPacketBuff(0x20);
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, (u32)dds3DrawEnv[dds3GetFrameID()].frameTex, reference);
    texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)texturePacket);
    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = ((u64)(u32)alpha << 32) | 0x64;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);
    sprite = sdf2DAllocSprite(1);
    vertex = (fldSpriteVertex_t *)sdf2DGetPrimPtr((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, (u32)sprite);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void fldDrawPos(const f32 *pos) {
    Sprite3D_t packet;
    f32 half = 36.0f;

    packet.uv[0].u = 0;
    packet.uv[0].v = 0;
    packet.uv[1].u = 0x200;
    packet.uv[1].v = 0;
    packet.uv[2].u = 0x200;
    packet.uv[2].v = 0x200;
    packet.uv[3].u = 0;
    packet.uv[3].v = 0x200;
    packet.vt[0].x = -half;
    packet.vt[0].y = -half;
    packet.vt[1].x = half;
    packet.vt[1].y = -half;
    packet.vt[2].x = half;
    packet.vt[2].y = half;
    packet.vt[3].x = -half;
    packet.vt[3].y = half;
    packet.col = 0x8080FF80;
    packet.pos.x = pos[0];
    packet.pos.y = pos[1];
    packet.pos.z = pos[2];
    packet.rot = 0.0f;
    rsetOtOfs(0x2E);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(D_0054EC60, &packet);
}

void fldDrawPosCol(const f32 *pos, s32 color) {
    Sprite3D_t packet;
    f32 half = 36.0f;

    packet.uv[0].u = 0;
    packet.uv[0].v = 0;
    packet.uv[1].u = 0x200;
    packet.uv[1].v = 0;
    packet.uv[2].u = 0x200;
    packet.uv[2].v = 0x200;
    packet.uv[3].u = 0;
    packet.uv[3].v = 0x200;
    packet.vt[0].x = -half;
    packet.vt[0].y = -half;
    packet.vt[1].x = half;
    packet.vt[1].y = -half;
    packet.vt[2].x = half;
    packet.vt[2].y = half;
    packet.vt[3].x = -half;
    packet.vt[3].y = half;
    packet.col = color;
    packet.pos.x = pos[0];
    packet.pos.y = pos[1];
    packet.pos.z = pos[2];
    packet.rot = 0.0f;
    rsetOtOfs(0x2E);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    drawSprite3D(D_0054EC60, &packet);
}

void fldDraw2DBar(s32 x, s32 y, s32 width, s32 height) {
    DrwSprite2D(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, D_0054EC60);
    DrwSprite2D(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, D_0054EC60);
    DrwSprite2D(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, D_0054EC60);
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    rsetAlpha(0);
}

void fldDrwZAlloc(void)
{
    if (fldDrwZBuf == NULL) {
        fldDrwZBuf = sdfAllocAuto(0x70000);
    }
}


void fldDrwZFree(void)
{
    if (fldDrwZBuf != NULL) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = NULL;
    }
}


void fldDrwZSave(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        sdfGrOt_t *descriptor;
        sdfLinkStoreImage(packet, D_00529388.zbuffHandle,
                                0, 0, 0x200, 0xE0, (s32)fldDrwZBuf, 0, 0, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, packet);
    }
}

void fldDrwZLoad(void) {
    if (fldDrwZBuf != 0) {
        sdfDrawTag_t *packet = sdfPrimAllocDrawTag(0);
        sdfGrOt_t *descriptor;
        sdfLinkLoadImage(packet, D_00529388.zbuffHandle,
                                  0, 0, 0x200, 0xE0, (s32)fldDrwZBuf, 0);
        descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, packet);
    }
}

void fldKanjiDraw(s32 x, s32 y, u32 colors, const u8 *text)
{
    frFontGlyph_t *object;

    object = frwGetSmallFontFRQ(x << 4, y << 4, 0, colors, text, 0);
    frDrawFont(object);
    frDestroyFontRequestAfter(object);
}

void LF(dbgConsCtl_t *cons)
{
    cons->cy += 0x60;
}


void initDrawTag(dbgConsCtl_t *quad) {
    s32 packet;
    sdfDrawTag_t *packetList;

    packetList = dds3FontReqAllocDrawTag();
    quad->tag = packetList;
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket((sdfPrimTransPacket_t *)packet);
    sdfDrawTagLink(quad->tag, packet);
}

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;
    f32 screenX;
    f32 screenY;

    SDF_VU0_LOADMATRIX(D_002FA240);
    InversMatrix();
    fldPoint3To2(&screenX, &screenY, x, y, z);
    if (screenX < -4000.0f || screenX > 4000.0f ||
        screenY < -4000.0f || screenY > 4000.0f) {
        return;
    }

    quad.cx = 0x73C0;
    quad.cy = 0x8440;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x8440;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, (s32)screenX * 16, (s32)screenY * 16, quad.cz, 1);
    sdfDrawTagLink(quad.tag,
                    (u32)sdfDevPrintfEx(&packet, D_0054EC50, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrint2D(s32 x, s32 y, const char *format) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x73C0;
    quad.cy = 0x7CC0;
    quad.cz = 0x0FFFFF7E;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x7CC0;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7D;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x, quad.cy + y, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, format));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[83];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrint2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x73C0;
    quad.cy = 0x7CC0;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x73C0;
    quad.py = 0x7CC0;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x, quad.cy + y, quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintNum2D(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, D_0054EC50, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintNum2DH(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, D_0054EC58, drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2D(s32 x, s32 y, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, 0);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + x * 16, quad.cy + y * 8, quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDbgPrintStr2DCF(s32 packetField, s32 drawValue, f32 x, f32 y) {
    dbgConsCtl_t quad;
    sifCommand_t packet;

    quad.cx = 0x7000;
    quad.cy = 0x7900;
    quad.cz = 0x0FFFFF80;
    quad.sy = 0;
    quad.panelCol = 0x10000000;
    quad.frameCol = 0x80806020;
    quad.px = 0x7000;
    quad.py = 0x7900;
    quad.sx = 0x1A40;
    quad.pz = 0x0FFFFF7F;
    initDrawTag(&quad);
    sdfDevSetPrintCB(&packet, quad.cx + (s32)(x * 16.0f), quad.cy + (s32)(y * 8.0f), quad.cz, packetField);
    sdfDrawTagLink(quad.tag, (u32)sdfDevPrintfEx(&packet, (const char *)drawValue));
    LF(&quad);
    {
        sdfGrOt_t *textOt = &sdfGrOtTable[81];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))textOt->regFunc)(textOt, quad.tag);
    }
}

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius) {
    s32 color;
    s32 angle;
    s32 prev;
    s32 i;
    f32 a;
    f32 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;

    if (fade != 0) {
        color = fade & 0xFF;
        rsetOtOfs(0x2E);
        rsetAlpha(5);
        if (color > 0x80) {
            color = 0x80;
        }
        color = 0x80 - color;
        if (color >= 0x80) {
            color = 0x7F;
        }
        if (color < 0x10) {
            color = 0x10;
        }
        color = (color << 24) | 0x10101;
        for (i = 0; i < 360; i += 10) {
            angle = (i + 360) % 360;
            prev = (i + 350) % 360;
            a = (f32)angle * 3.14f / 180.0f;
            sinA = sdfSin(a) * radius;
            cosA = sdfCos(a) * radius;
            b = (f32)prev * 3.14f / 180.0f;
            sinB = sdfSin(b) * radius;
            cosB = sdfCos(b) * radius;
            fldDrawPoly3(color, 0x80101010, 0x80101010,
                                x + 0.0f, y - 2.0f, z + 0.0f,
                                x + sinA, y - 2.0f, z + cosA,
                                x + sinB, y - 2.0f, z + cosB);
        }
        rsetAlpha(0);
    }
}

s32 nbGetEncountOn(void)
{
    return gencOn;
}


void nbSetEncountMode(s32 mode)
{
    gencMode = mode;
}


extern effBlurTemplateBody_t D_0055DAD8;
extern void dds3EffectPartialMultiBlurSetParam(void *blur, effBlurTemplateBody_t *body);
extern void dds3EffectPartialMultiBlurDraw(void *blur);
extern void encFreeBlur(void);

s32 encDrawProc(dds3ProcessID_t *id) {
    if (gencOn == 0) {
        return 0;
    }
    if (gencMode == 0) {
        rsetOtOfs(0x43);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        rsetAlpha(0);
        fldDrawBokasi();
        aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
        rsetAlpha(0);
        if (encDrawState >= 13) {
            D_0055DAD8.source.displacement = D_0055DAD8.source.displacement + 0.001f;
            D_0055DAD8.extent = D_0055DAD8.extent - 2;
            dds3EffectPartialMultiBlurSetParam(hPartialMultiBlur, &D_0055DAD8);
            dds3EffectPartialMultiBlurDraw(hPartialMultiBlur);
        }
        if (encDrawState >= 0x29) {
            rsetOtOfs(0x47);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 2);
        } else if (encDrawState >= 0x15) {
            rsetOtOfs(0x47);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 2);
        }
        encDrawState = encDrawState + 1;
    }
    if (gencMode == 1) {
        if (gencStageInnAlp > 0) {
            rsetOtOfs(0x51);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            fldDrawFrameSpriteAlpha(gencStageInnAlp);
            gencStageInnAlp = gencStageInnAlp - 2;
        } else {
            encFreeBlur();
        }
    }
    return 0;
}

void encFreeBlur(void)
{
    gencOn = 0;
    if (hPartialMultiBlur != NULL) {
        dds3EffectPartialMultiBlurDelete(hPartialMultiBlur);
        hPartialMultiBlur = NULL;
    }
}


extern effBlurTemplateBody_t D_0055DAD8;
extern effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const effBlurTemplateBody_t *body);
extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 flag);

void encStart(void) {
    if (nbGetEncountOn() != 0) {
        return;
    }
    gencStageInnAlp = 0x80;
    D_0055DAD8.source.color = 0x09808080;
    D_0055DAD8.source.blendControl = 0x44;
    D_0055DAD8.source.angle = 0.01f;
    D_0055DAD8.source.displacement = 1.02f;
    D_0055DAD8.extent = 0x12C;
    gencMode = 0;
    encDrawState = 0;
    D_0055DAD8.source.position[0] = 0;
    D_0055DAD8.source.position[1] = 0;
    D_0055DAD8.source.corners[0][0] = 0;
    D_0055DAD8.source.corners[0][1] = 0;
    D_0055DAD8.source.corners[1][0] = 0;
    D_0055DAD8.source.corners[1][1] = 0;
    hPartialMultiBlur = dds3EffectPartialMultiBlurCreate(&D_0055DAD8);
    gencOn = 1;
    if (dds3GetActiveScene() == NULL) {
        return;
    }
    dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
}

void encInitProcess(void)
{
    dds3StartProcess(D_0054EC80, 0x2B0F, 0, 1, encDrawProc, NULL, NULL);
}


f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    f32 dx;
    f32 dz;
    f32 result;
    f32 absZ;
    f32 angle;
    f32 dist;

    if (fromX == toX && fromZ == toZ) return 0.0f;
    result = fromX - toX;
    dx = result;
    dz = fromZ - toZ;
    absZ = dz;
    if (result < 0.0f) result = -result;
    if (absZ < 0.0f) absZ = -absZ;
    angle = sdfAtan2(absZ, result) * (180.0f / 3.14f);
    if (dx < 0.0f) {
        if (!(dz < 0.0f)) angle = -angle;
    } else {
        if (dz < 0.0f) angle = 180.0f - angle;
        else angle += 180.0f;
    }
    result = angle;
    return result;
}

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    s32 angle = (s32)(360.0f - fldCalcAngleBetween(fromX, fromZ, toX, toZ) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldCamCalcSlopeOffset);

void fldCamSetLeng(void) {
    f32 dx = D_002FC9C0[0] - fldCamTarget[0];
    f32 dy = D_002FC9C0[1] - fldCamTarget[1];
    f32 dz = D_002FC9C0[2] - fldCamTarget[2];

    fldCamLength = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

void fldCamResetNpc(void)
{
    fldCamResetOn = 0;
}


INCLUDE_ASM(const s32, "field/fldDraw", fldCamCalcSaka);

INCLUDE_ASM(const s32, "field/fldDraw", resetCamNormal);

INCLUDE_ASM(const s32, "field/fldDraw", resetCamNormal2);

extern f32 fldCamPitch;
extern f32 fldCamParamTable[][4];

/* NON_MATCHING: 3 of 95 words differ (first at +0x90). */
#ifdef NON_MATCHING
void fldCamGetPhokanParam(void) {
    f32 angle;

    fldCamPhokanEyeX[0] =
        fldGlobalBase.x - sdfSin(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex][1];
    fldCamPhokanEyeX[2] =
        fldGlobalBase.z - sdfCos(fldCamPitch * 3.14f / 180.0f) * 80.0f;
    fldCamPhokanEyeX[3] = 1.0f;
    angle = fldCamPitch * 3.14f / 180.0f;
    fldCamPhokanTgtX[0] = fldGlobalBase.x +
        sdfSin(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex][0];
    fldCamPhokanTgtX[1] = fldGlobalBase.y +
        fldCamParamTable[fldGlobalBase.cameraParamIndex][2];
    fldCamPhokanTgtX[2] = fldGlobalBase.z +
        sdfCos(angle) * fldCamParamTable[fldGlobalBase.cameraParamIndex][0];
    fldCamPhokanTgtX[3] = 1.0f;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", fldCamGetPhokanParam);
#endif

void fldCamUpdateFollow(void) {
    union {
        u128 q;
        f32 f[4];
    } nearPoint, farPoint;
    fldFollowCameraControl_t *control = fldGetFollowCameraControl();
    f32 ratio = (f32)fldCamPhokanState / (f32)fldCamPHokanMode;
    f32 remaining = 1.0f - ratio;
    dds3Basic_t *node;

    nearPoint.f[0] = fldCamPhokanEyeX[0] * ratio + fldCamPhokanSelX[0] * remaining;
    nearPoint.f[1] = fldCamPhokanEyeX[1] * ratio + fldCamPhokanSelX[1] * remaining;
    nearPoint.f[2] = fldCamPhokanEyeX[2] * ratio + fldCamPhokanSelX[2] * remaining;
    farPoint.f[0] = fldCamPhokanTgtX[0] * ratio + fldCamPhokanTgtX[0] * remaining;
    farPoint.f[1] = fldCamPhokanTgtX[1] * ratio + fldCamPhokanTgtX[1] * remaining;
    farPoint.f[2] = fldCamPhokanTgtX[2] * ratio + fldCamPhokanTgtX[2] * remaining;
    SDF_VU0_COPYVECTOR(D_002FC9C0, &nearPoint);
    SDF_VU0_COPYVECTOR(fldCamTarget, &farPoint);
    dds3SetPosPoseSet(control->camera, &farPoint.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamPhokanState++;
    if (fldCamPHokanMode < fldCamPhokanState) {
        fldGlobalBase.mode = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldDraw", calcCamNormal);

INCLUDE_ASM(const s32, "field/fldDraw", calcCamDirLock);

extern s32 fldNpcCamTarget;
extern f32 fldCamNpcTarget;
extern f32 fldCamNpcTarget2;
extern f32 D_0054EF6C;
extern f32 D_0054EF70;
extern f32 D_0054EF74;
extern f32 D_0054EF78;
extern f32 D_0054EF80;
extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, void *out);
extern void dds3GetTMPoseSet(dds3Basic_t *object);

void calcCamNormalNPC(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 dist;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetPosPoseSet(unit->handle);
    eye.f[3] = 1.0f;
    fldGlobalBase.menuOpenFlag = 4;
    fldGlobalBase.encountState = 5;
    if (fldNpcCamTarget == 2) {
        fldCamResetOn = 2;
        eye.f[0] = D_0054EF6C;
        eye.f[2] = D_0054EF74;
        eye.f[1] = D_0054EF70 + fldCamNpcTarget + 60.0f;
    } else {
        fldCamResetOn = 1;
        eye.f[0] = D_0054EF6C;
        eye.f[2] = D_0054EF74;
        eye.f[1] = D_0054EF70 + fldCamNpcTarget;
    }
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    dds3GetTMPoseSet(unit->handle);
    target.f[3] = 1.0f;
    if (fldNpcCamTarget == 2) {
        angle = 3.14f;
        angle = -D_0054EF78 * angle;
        angle = angle / 180.0f;
        dist = 200.0f;
        target.f[0] = eye.f[0] + sdfSin(angle) * dist;
        target.f[1] = D_0054EF70 + fldCamNpcTarget;
        target.f[2] = eye.f[2] + sdfCos(angle) * dist;
    } else {
        angle = 3.14f;
        angle = -D_0054EF78 * angle;
        angle = angle / 180.0f;
        dist = 200.0f;
        target.f[0] = eye.f[0] + sdfSin(angle) * (fldCamNpcTarget2 + dist);
        target.f[1] = D_0054EF70 + D_0054EF80;
        target.f[2] = eye.f[2] + sdfCos(angle) * (fldCamNpcTarget2 + dist);
    }
    dds3SetPosPoseSet(control->camera, &target.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
}

extern void dds3GetRotPoseSet(dds3Basic_t *object);
extern f32 EulerAngleAxisY(void);
extern f32 fldCamPitch;
extern f32 fldCamParamTable[][4];

/* NON_MATCHING: 36 of 126 words differ (first at +0x5C). */
#ifdef NON_MATCHING
void resetCamTgtPlayer(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    fldUnitControl_t *unit;
    dds3Basic_t *node;
    f32 angle;
    f32 dx;
    f32 dy;
    f32 dz;

    control = fldGetFollowCameraControl();
    unit = fldGetUnitControl();
    dds3GetRotPoseSet(unit->handle);
    angle = EulerAngleAxisY() * 180.0f / 3.14f;
    fldGlobalBase.camAngle = angle;
    fldGlobalBase.camAngle2 = angle;
    eye.f[0] = D_002FC9C0[0] + (fldGlobalBase.x - D_002FC9C0[0]);
    eye.f[1] = D_002FC9C0[1] +
               (fldGlobalBase.y + fldCamParamTable[fldGlobalBase.cameraParamIndex][1] - D_002FC9C0[1]);
    eye.f[2] = D_002FC9C0[2] + (fldGlobalBase.z - D_002FC9C0[2]);
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    dds3GetTMPoseSet(unit->handle);
    target.f[3] = 1.0f;
    target.f[0] = fldGlobalBase.campos.x;
    target.f[1] = fldGlobalBase.campos.y;
    target.f[2] = fldGlobalBase.campos.z;
    dds3SetPosPoseSet(control->camera, &target.q);
    target.f[0] = fldCamTarget[0] + (target.f[0] - fldCamTarget[0]);
    target.f[1] = fldCamTarget[1] + (target.f[1] - fldCamTarget[1]);
    target.f[2] = fldCamTarget[2] + (target.f[2] - fldCamTarget[2]);
    dds3SetPosPoseSet(control->camera, &target.q);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    dx = eye.f[0] - target.f[0];
    dy = eye.f[1] - target.f[1];
    dz = eye.f[2] - target.f[2];
    fldCamLength = fsqrtf(dx * dx + dy * dy + dz * dz);
    fldCamPitch = GetSphericalPosMove(target.f[0], target.f[2], eye.f[0], eye.f[2]) + 180.0f;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", resetCamTgtPlayer);
#endif

INCLUDE_ASM(const s32, "field/fldDraw", fldEventUpdateEx_Unity_2);

INCLUDE_ASM(const s32, "field/fldDraw", fldCamSetChaseCore);

void fldCamSetChase(void)
{
    fldCamSetChaseCore(0.0f);
}


void fldCamUpdateDefault(void)
{
    fldCamSetChaseCore(8.0f);
}


extern u8 dds3PadStickState[2][2][2];
extern f32 fldCamPitch;
extern f32 D_0054ECB4;
extern f32 D_0054ECB8;

/* NON_MATCHING: 28 of 148 words differ (first at +0x16C). */
#ifdef NON_MATCHING
void fldCamUpdateChase(void) {
    union {
        u128 q;
        f32 f[4];
    } eye, target;
    fldFollowCameraControl_t *control;
    dds3Basic_t *node;

    control = fldGetFollowCameraControl();
    memset(&eye, 0, sizeof(eye));
    eye.f[3] = 1.0f;
    memset(&target, 0, sizeof(target));
    target.f[3] = 1.0f;
    D_0054ECB4 = (f32)(dds3PadStickState[0][0][0] - 0x80) * 0.7f + fldCamPitch;
    D_0054ECB8 = -((f32)(dds3PadStickState[0][0][1] - 0x80) * 2.5f);
    eye.f[0] = fldGlobalBase.x - sdfSin(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.f[1] = fldGlobalBase.y - 200.0f + D_0054ECB8;
    eye.f[2] = fldGlobalBase.z - sdfCos(D_0054ECB4 * 3.14f / 180.0f) * 256.0f;
    eye.f[0] = D_002FC9C0[0] + (eye.f[0] - D_002FC9C0[0]) * 0.0625f;
    eye.f[1] = D_002FC9C0[1] + (eye.f[1] - D_002FC9C0[1]) * 0.0625f;
    eye.f[2] = D_002FC9C0[2] + (eye.f[2] - D_002FC9C0[2]) * 0.0625f;
    dds3CameraObjectBasicGetCameraTM(control->camera, &eye);
    target.f[1] = fldCamTarget[1] + (fldGlobalBase.y - 200.0f - fldCamTarget[1]) * 0.0625f;
    target.f[0] = fldCamTarget[0] + (fldGlobalBase.x - fldCamTarget[0]) * 0.0625f;
    target.f[2] = fldCamTarget[2] + (fldGlobalBase.z - fldCamTarget[2]) * 0.0625f;
    dds3SetPosPoseSet(control->camera, &target.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    SDF_VU0_COPYVECTOR(D_002FC9C0, &eye);
    SDF_VU0_COPYVECTOR(fldCamTarget, &target);
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", fldCamUpdateChase);
#endif

dds3Basic_t *fldCamNowEveCamera(void) {
    dds3Basic_t *worldObject = dds3GetActiveScene();
    dds3Basic_t *currentEntry = dds3GetActiveCameraSceneBasic(worldObject);

    if (fldGetUnitFollowCamera() == currentEntry) {
        currentEntry = NULL;
    }
    return currentEntry;
}

INCLUDE_ASM(const s32, "field/fldDraw", calcCamSyukan);

void fldSetElvCameraScene(u32 mode) {
    fldElvCameraScene = mode;
    dds3CameraObjectBasicGetTgtCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), D_0055DB20,
                                              D_0055DB10);
    fldCamElvState = 0;
}

void fldCamElvTask(void) {
    f32 direction = 0.0f;
    f32 phase = fldCamElvState;
    dds3Basic_t *camera;

    if (fldElvCameraScene != 0) {
        if (fldElvCameraScene == 1) {
            direction = 1.0f;
        }
        if (fldElvCameraScene == 2) {
            direction = 1.0f;
        }
        if (fldElvCameraScene == -1) {
            direction = -1.0f;
        }
        if (fldElvCameraScene == -2) {
            direction = -1.0f;
        }
        camera = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
        if (fldElvCameraScene == 1 || fldElvCameraScene == -1) {
            if (phase < 3.14f) {
                phase += 0.2f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.5f;
            } else {
                phase += 0.02f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.0f;
            }
        }
        if (fldElvCameraScene == 2 || fldElvCameraScene == -2) {
            phase += 11.0f;
            if (phase > 0.0f) {
                phase -= 1.0f;
            } else if (phase > -3.14f) {
                phase -= 0.2f;
                camera->poseset->pos.y = D_0055DB10[1] + sdfSin(phase * direction) * 2.0f;
            } else {
                phase -= 0.01f;
            }
            phase -= 11.0f;
        }
        fldCamElvState = phase;
        dds3OnPoseSetFlag(camera->poseset, POSESET_FLAG_UPDATEPOSE);
    }
}

void fldClearElvCameraScene(void)
{
    fldElvCameraScene = 0;
}

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531BF0);

INCLUDE_ASM(const s32, "field/fldDraw", fldCamMain);

extern void *dds3SearchProcessName(const char *name);
extern void dds3SetScreenFovy(f32 fovy);
extern f32 fldCamParamTable[][4];
extern char D_00531BF0[];

void fldCamSyukanSuriawase(void) {
    fldAreaWork_t *state;

    if (dds3SearchProcessName("fldProcSequence") == NULL) {
        return;
    }
    state = &fldGlobalBase;
    if (state->mode != 0) {
        return;
    }
    if (evtBitCheck(0xAF0) == 0) {
        return;
    }
    if (scrSearchScriptName(D_00531BF0) != NULL) {
        return;
    }
    if (dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) != fldGetUnitFollowCamera()) {
        return;
    }
    dds3SetScreenFovy(fldCamParamTable[state->cameraParamIndex][3] * 3.14f / 180.0f);
}

s32 fldCamMiwatasiChk(void)
{
    return fldCamMiwatasi;
}


s32 fldCamNpcCamChk(char *name) {
    if (name == NULL) {
        return 0;
    }
    if (fldGlobalBase.mode == 5 && strcmp(fldGlobalBase.requestedSceneName, name) == 0) {
        return 1;
    }
    return 0;
}

f32 R360_LR(f32 fromDegrees, f32 toDegrees) {
    f32 diff;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    diff = fromDegrees - toDegrees;
    if (diff > 180.0f || diff < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
}

f32 fldPlayerCalc360_8(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(R360_LR(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(R360_LR(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(R360_LR(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(R360_LR(315.0f, angle));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 1:
        angle = 45.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 3:
        angle = 135.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 5:
        angle = 225.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    case 7:
        angle = 315.0f;
        break;
    }
    return angle;
}

f32 fldPlayerCalc360_4(f32 angle) {
    f32 best;
    s32 index;
    f32 diff;
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(270.0f, angle));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        angle = 0.0f;
        break;
    case 2:
        angle = 90.0f;
        break;
    case 4:
        angle = 180.0f;
        break;
    case 6:
        angle = 270.0f;
        break;
    }
    return angle;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerMirChk);

extern s32 fldPlayerStatus[];
extern s32 fldChkVortex(void);
extern void mdlLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex);
extern void mdlSmoothLoopMotion(mdlObj_t *ctx, s32 searchId, s32 motionIndex,
                                f32 blendLeadFrames, f32 blendFrames);

void fldPlayerMotion(s32 motion, s32 mode, f32 speed) {
    fldUnitControl_t *ctl;
    mdlObj_t *model;
    mdlObj_t *model2 = NULL;

    ctl = fldGetUnitControl();
    model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    if (ctl->handle2 != NULL) {
        model2 = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2);
    }
    if (motion == mode) {
        return;
    }
    model->first->speed = 1.0f;
    if (fldChkVortex() != 0) {
        if (fldPlayerStatus[0] & 2) {
            mdlLoopMotion(model, 3, 1);
        } else {
            mdlLoopMotion(model, 3, 0);
        }
    }
    mdlSmoothLoopMotion(model, 0, mode, speed, speed);
    if (ctl->handle2 == NULL) {
        return;
    }
    model2->first->speed = 1.0f;
    mdlSmoothLoopMotion(model2, 0, mode, speed, speed);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerMotionSe);

void fldPlayerCalc_Hit(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    f32 pos[4];
    f32 probe[4];

    if (ctl == NULL) {
        return;
    }
    if (fldPlayerFastMode == 1) {
        return;
    }
    if (l_UseCollision == 0) {
        return;
    }
    probe[0] = fldGlobalBase.x;
    probe[1] = fldGlobalBase.y;
    probe[2] = fldGlobalBase.z;
    fldHitChkUnitElv(probe);
    if (fldGlobalBase.hitValid == 0) {
        return;
    }
    fldGlobalBase.x = fldGlobalBase.hitX;
    fldGlobalBase.y = fldGlobalBase.hitY;
    fldGlobalBase.z = fldGlobalBase.hitZ;
    pos[0] = fldGlobalBase.hitX;
    pos[1] = fldGlobalBase.hitY;
    pos[2] = fldGlobalBase.hitZ;
    dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    fldGlobalBase.hitValid = 0;
    dds3GetPosPoseSet(ctl->handle);
    SDF_VU0_STOREVECTOR(pos);
    SDF_VU0_COPYVECTOR((u8 *)ctl->handle->poseset + 0x70, pos);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalc_WarpRun);

/* NON_MATCHING: 14 of 58 words differ (first at +0x44). */
#ifdef NON_MATCHING
s32 fldPlayerCalcHasigo(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    f32 pos[4];

    if (gfldPlayerDamegeCnt > 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pos[1] -= 2.0f;
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1];
        fldGlobalBase.z = pos[2];
        pos[1] = fldGlobalBase.y;
        pos[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    } else if (gfldPlayerDamegeCnt < 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pos[1] += 2.0f;
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1];
        fldGlobalBase.z = pos[2];
        pos[1] = fldGlobalBase.y;
        pos[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    } else {
        return 0;
    }
    fldPlayerSdwOff();
    fldPlayerSvsOff();
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalcHasigo);
#endif

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531C50);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531C60);

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalc_Nml);

void fldPlayerCalc_WarpStart(void)
{
    fldWmWarpMode = 1;
}


void fldPlayerCalc_FastStart(void)
{
    if (fldWmWarpMode == 0) {
        fldWmWarpMode = 2;
        fldWmFastMove = 60.0f;
    }
}


void fldPlayerCalc_Wm3(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    mdlObj_t *model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    fldScaleVec_t scale = D_00531D08;
    f32 next;

    if (fldWmWarpMode != 1) {
        model->first->speed = 0.0f;
    } else {
        scale.x *= fldWmapPlayerState;
        scale.y *= fldWmapPlayerState;
        scale.z *= fldWmapPlayerState;
        dds3SetSclPoseSet(ctl->handle, &scale.x);
        next = fldWmapPlayerState - 0.08f;
        model->first->speed = 5.0f;
        if (next <= 0.1f) {
            fldWmapPlayerState = 0.1f;
        } else {
            fldWmapPlayerState = next;
        }
    }
}

void fldPlayerCalc_Wm4(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    mdlObj_t *model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    fldScaleVec_t scale = D_00531D18;
    f32 next;

    if (fldWmWarpMode == 2) {
        fldWmWarpMode = 3;
        fldWmapPlayerState = 0.1f;
    } else {
        scale.x *= fldWmapPlayerState;
        scale.y *= fldWmapPlayerState;
        scale.z *= fldWmapPlayerState;
        dds3SetSclPoseSet(ctl->handle, &scale.x);
        next = fldWmapPlayerState + 0.04f;
        model->first->speed = 5.0f;
        if (1.0f < next) {
            fldWmapPlayerState = 1.0f;
            fldWmWarpMode = 0;
        } else {
            fldWmapPlayerState = next;
        }
    }
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalc_Wm2);

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalc_Nml_syukan);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D08);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D18);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D28);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D38);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D48);

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerCalc);

s32 fldPlayerChkAna(void)
{
    if (gfldPlayerHasiCnt == 0 && fldPlayerStepDist == 0 && gfldPlayerDamegeCnt == 0) {
        return 0;
    }
    return 1;
}


void fldPlayerOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOnFlag(ctl->handle, 1);
    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOnFlag(ctl->handle2, 1);
    }
    fldPlayerMotReset2();
}

void fldPlayerOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOffFlag(ctl->handle, 1);
    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOffFlag(ctl->handle2, 1);
    }
}

void fldPlayerSvsOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseSetControlBasicSvs(ctl->handle, 0);
    dds3ObjectBaseOffFlag(ctl->handle, 0x800);
}

void fldPlayerSvsOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseSetControlBasicSvs(ctl->handle, 0x80);
    dds3ObjectBaseOnFlag(ctl->handle, 0x800);
}

void fldPlayerAlpOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle))->inner->color = 0;
    if (ctl->handle2 != NULL) {
        ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2))->inner->color = 0;
    }
}

void fldPlayerAlpOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle))->inner->color = 0x80808080;
    if (ctl->handle2 != NULL) {
        ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2))->inner->color = 0x80808080;
    }
}

void fldPlayerSdwOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOnFlag(ctl->handle, 0x400);
        return;
    }
    dds3ObjectBaseOnFlag(ctl->handle, 0x200);
}

void fldPlayerSdwOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOffFlag(ctl->handle, 0x400);
    dds3ObjectBaseOffFlag(ctl->handle, 0x200);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerFastSet);

void fldPlayerMotReset(void) {
    dds3Basic_t *unit;

    if (fldGb < 10) {
        return;
    }
    unit = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (unit == NULL) {
        return;
    }
    fldPlayerMotion(((mdlObj_t *)dds3UnitObjectBasicGetControlWorkData(unit)->modelHandle)->current.h.motionIndex,
                    9, 10.0f);
}

void fldPlayerMotReset2(void) {
    dds3Basic_t *unit;

    if (fldGb < 10) {
        return;
    }
    unit = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (unit == NULL) {
        return;
    }
    fldPlayerMotion(((mdlObj_t *)dds3UnitObjectBasicGetControlWorkData(unit)->modelHandle)->current.h.motionIndex,
                    9, 0.0f);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPlayerPosRot);

void fldSkyDefaultLoad(void) {
    devState_t *fd;

    if (fldSkyDefault == NULL) {
        fldSkyDefault = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (fldSkyDefaultData == NULL) {
        fldSkyDefaultData = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (fldSkyDataTable == NULL) {
        fldSkyDataTable = (fldSkyData_t *)sdfMemLock(sdfMemAllocDirect(0x12400));
    }
    fd = sdfFileOpen("/fld/f/bin/FILTER.FLD");
    sdfFileRead(fd, fldSkyDataTable, 0xE000);
    sdfFileClose(fd);
    fd = sdfFileOpen("/fld/f/bin/BATTLEBG.SKY");
    sdfFileRead(fd, fldSkyDefaultData, 0xE000);
    sdfFileClose(fd);
}

void fldSkyFieldLoad(s32 area) {
    char path[64];
    char directory[32];
    devState_t *fd;

    gFilterAlpha = 0x80;
    if (area < 200) {
        fldMakeFilePath(directory, area, 1);
        if (fldGlobalBase.area == 0x19 && evtBitCheck(0x599) != 0 && evtBitCheck(0x743) == 0) {
            sprintf(path, "%sF%03db.SKY", directory, area);
        } else if (fldGlobalBase.area == 0x18 && evtBitCheck(0x4F) != 0) {
            sprintf(path, "%sF%03db.SKY", directory, area);
        } else if (fldGlobalBase.area == 0x18 && evtBitCheck(0x16) != 0 && evtBitCheck(0x3E) == 0) {
            sprintf(path, "%sF%03dc.SKY", directory, area);
        } else {
            sprintf(path, "%sF%03d.SKY", directory, area);
        }
        fd = sdfFileOpen(path);
        sdfFileRead(fd, fldSkyDefault, 0xE000);
        sdfFileClose(fd);
    }
}

void fldGetSkyEntry(const void *src) {
    gFilterAlpha = 0x80;
    memcpy(fldSkyDefault, src, 0xE000);
}

void fldSkyCalcFilter(void) {
    gFilterAnimeOfsA = 0;
    switch (gFilterAnime) {
    case 1:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 2:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 3:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 4:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 5:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 6:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    }
}

INCLUDE_ASM(const s32, "field/fldDraw", fldSkyDrawFilter);

void fldSkySetFilter(s32 area, s32 value, s32 duration) {
    if (fldSkyEnable == 0 && fldGb < 0x32) {
        duration = 0;
    }
    if (duration == 0) {
        fldSkyFilter = area;
        fldSkyFilterRate = 1.0f;
        fldSkyFilterTime = 1.0f;
    } else {
        fldSkyFilterRate = 0.0f;
        fldSkyFilter = fldSkyFilterPrev;
        fldSkyFilterTime = (f32)duration;
    }
    fldSkyFilterPrev = area;
    fldSkyDataTable[area].mode = value;
}

void fldSetFilterAnime(s32 anime)
{
    gFilterAnime = anime;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
}


void fldSkySetAlpha(s32 alpha)
{
    gFilterAlpha = alpha;
}


s32 fldSkyGetAlpha(void)
{
    return gFilterAlpha;
}


void fldSkySetAlphaOfs(s32 ofs)
{
    fldSkyAlphaOfs = ofs;
}


s32 fldSkyGetAlphaOfs(void)
{
    return fldSkyAlphaOfs;
}


void fldSkySetTableAnime(s32 anime)
{
    s32 idx = D_002FC0EC;

    gSkyTblAnime = anime;
    gSkyTblAnimeCntI = 0;
    gSkyTblBaseIdx = idx;
    fldSkySetTbl(idx, 1);
}


/* The dispatch runs ahead of all three bodies, which a switch or an if chain
 * over the same three values does not reproduce. */
void fldSkyCalcTable(void) {
    s32 anime = gSkyTblAnime;

    if (anime == 1) {
        goto anime1;
    }
    if (anime < 2) {
        return;
    }
    if (anime == 2) {
        goto anime2;
    }
    if (anime != 3) {
        return;
    }
    goto anime3;

anime1:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 30.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 30);
        return;
    }
    if (gSkyTblAnimeCntI == 60.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 30);
        gSkyTblAnimeCntI = 0.0f;
    }
    return;

anime2:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 15.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 1);
        return;
    }
    if (gSkyTblAnimeCntI == 30.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 1);
        gSkyTblAnimeCntI = 0.0f;
    }
    return;

anime3:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 60.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 60);
        return;
    }
    if (gSkyTblAnimeCntI == 120.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 60);
        gSkyTblAnimeCntI = 0.0f;
    }
}

void fld_136800(s32 duration, f32 lightR, f32 lightG, f32 lightB,
                f32 ambientR, f32 ambientG, f32 ambientB) {
    evtUnit_t *unit;
    s32 r, g, b;
    u32 lightColor, ambientColor;

    if (D_0054EAF0 != 0) {
        unit = evtGetUnitHandle2((dds3Basic_t *)D_0054EAF0);
        r = (s32)(lightR * 128.0f);
        g = (s32)(lightG * 128.0f);
        b = (s32)(lightB * 128.0f);
        lightColor = r | (b << 16) | (g << 8) | 0x80000000;
        r = (s32)(ambientR * 128.0f);
        g = (s32)(ambientG * 128.0f);
        b = (s32)(ambientB * 128.0f);
        ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, duration, lightColor, ambientColor);
    }
}

void fldSkySetPlayer(s32 duration, f32 colorAX, f32 colorAY, f32 colorAZ,
                     f32 colorBX, f32 colorBY, f32 colorBZ,
                     f32 dirX, f32 dirY, f32 dirZ) {
    f32 dir[4];
    evtUnit_t *playerUnit = NULL;
    evtUnit_t *secondUnit = NULL;
    evtUnit_t *mainUnit;
    u32 lightColor, ambientColor;
    s32 r, g, b;

    if (fldPlayerUnit != 0) {
        playerUnit = evtGetUnitHandle2((dds3Basic_t *)fldPlayerUnit);
        evtUnitLightOn(playerUnit);
    }
    if (D_0054EAF0 != 0) {
        secondUnit = evtGetUnitHandle2((dds3Basic_t *)D_0054EAF0);
        evtUnitLightOn(secondUnit);
    }
    mainUnit = evtGetUnitHandle2(D_0054EAE8);
    evtUnitLightOn(mainUnit);

    r = (s32)(colorAX * 128.0f);
    g = (s32)(colorAY * 128.0f);
    b = (s32)(colorAZ * 128.0f);
    lightColor = r | (b << 16) | (g << 8) | 0x80000000;
    r = (s32)(colorBX * 128.0f);
    g = (s32)(colorBY * 128.0f);
    b = (s32)(colorBZ * 128.0f);
    ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
    evtSetUnitLightAndAmbient(mainUnit, duration, lightColor, ambientColor);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightAndAmbient(playerUnit, duration, lightColor, ambientColor);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightAndAmbient(secondUnit, duration, lightColor, ambientColor);
    }

    dir[0] = dirX;
    dir[1] = dirY;
    dir[2] = dirZ;
    dir[3] = 0.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)dir);
    evtSetUnitLightDir(mainUnit, duration);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightDir(playerUnit, duration);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightDir(secondUnit, duration);
    }
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)dir);
    evtSetUnitLightDir(mainUnit, 0);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightDir(playerUnit, 0);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightDir(secondUnit, 0);
    }

    D_002FCA60[0] = colorAX;
    D_002FCA60[1] = colorAY;
    D_002FCA60[2] = colorAZ;
    D_002FCA60[4] = dirX;
    D_002FCA60[5] = dirY;
    D_002FCA60[6] = dirZ;
    fldSkyPlayerIdx[0] = colorBX;
    fldSkyPlayerIdx[1] = colorBY;
    fldSkyPlayerIdx[2] = colorBZ;
}

void fldSkySetTbl(s32 idx, s32 durationMode) {
    fldAreaWork_t *state = &fldGlobalBase;
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (state->area < 200) {
        if (fldSkyEnable == 0 && state->area < 0x32) {
            durationMode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (durationMode != 0) {
            duration = defaultDuration;
        }
        if (durationMode >= 2) {
            duration = durationMode;
        }
        fldSkyTable = idx;
        fldSkyTableIdx = idx;
        fldSkyTblUpperIdx = 0;
        if (state->lightMaIdx == 0) {
            light = &((fldLightSet_t *)fldSkyDefault)[idx];
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
        }
    }
}

void fldSkySetTblModosu(s32 speed) {
    if (fldSkyEnable == 0 && fldGb < 0x32) {
        speed = 0;
    }
    if (fldSkyTableIdx != 0 && fldSkyTable != fldSkyTableIdx) {
        if (fldLightMaState == 0) {
            D_002FC0EC = fldSkyTableIdx;
        }
        fldSkySetTbl(fldSkyTableIdx, speed);
    }
}

void fldSkySetTblUpper(s32 idx, s32 mode) {
    fldAreaWork_t *state = &fldGlobalBase;
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;
    u32 previousIndex;

    if (state->area < 200) {
        if (fldSkyEnable == 0 && state->area < 0x32) {
            mode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (mode != 0) {
            duration = defaultDuration;
        }
        if (mode >= 2) {
            duration = mode;
        }
        if (fldSkyTable != idx) {
            previousIndex = fldSkyTableIdx;
            if (fldSkyTblUpperIdx == 0) {
                previousIndex = fldSkyTable;
            }
            D_0054ED5C = gFilterAnime;
            D_0054ED60 = gFilterAnimeCntF;
            D_0054ED64 = gFilterAnimeOfsA;
            D_0054ED68 = fldSkyFilterPrev;
            D_0054ED6C = fldSkyFilter;
            D_0054ED70 = fldSkyFilterRate;
            D_0054ED74 = fldSkyFilterTime;
            fldSkyTableIdx = previousIndex;
            D_0054ED7C = gSkyTblAnime;
            D_0054ED80 = gSkyTblAnimeCntI;
            D_0054ED84 = gSkyTblBaseIdx;
            fldSkyTblUpperIdx = 1;
            fldSkyTable = idx;
            if (state->lightMaIdx == 0) {
                light = &((fldLightSet_t *)fldSkyDefault)[idx];
                fldRunDanteGunFlag[11] = idx;
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
            }
        }
    }
}

void fldSkySetTblLightMaDZ(s32 enabled) {
    if (enabled == 0) {
        dds3ObjectBaseOffFlag(D_0054EAE8, 0x100);
        return;
    }
    dds3ObjectBaseOnFlag(D_0054EAE8, 0x100);
    dds3UnitObjectBasicEL_LinkFree(D_0054EAE8);
}

void fldSkySetTblLightMa(s32 idx, s32 durationMode) {
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (fldGb < 200) {
        if (fldSkyEnable == 0 && fldGb < 0x32) {
            durationMode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (durationMode != 0) {
            duration = defaultDuration;
        }
        if (durationMode >= 2) {
            duration = durationMode;
        }
        if (fldSkyLightMaTbl != idx) {
            if (idx == 0) {
                idx = fldSkyTable;
            }
            light = &((fldLightSet_t *)fldSkyDefault)[idx];
            D_0054ED8C = fldRunDanteGunFlag[11];
            fldRunDanteGunFlag[11] = idx;
            fldSkyLightMaTbl = idx;
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
        }
    }
}

void fldSetFileResolverIdx(u32 state) {
    fldSkyTable = state;
    fldSkyTableIdx = 0;
    fldRunDanteGunFlag[12] = 0;
    fldRunDanteGunFlag[11] = state;
    fldSkyLightMaTbl = 0;
}

void fld_1374F0(void) {
    u32 *displayPointers = D_002FCAC0;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = (u32)D_002FCAA0;
    displayPointers[1] = (u32)fldSkyPlayerIdx;
}

void fldSkyRefreshTbl(void) {
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    f32 *dirp;
    evtUnit_t *unit;
    s32 r, g, b;
    u32 lightColor, ambientColor;

    if (fldGb < 200) {
        if (fldSkyLightMaTbl != 0) {
            light = &((fldLightSet_t *)fldSkyDefault)[fldSkyLightMaTbl];
        } else {
            light = &((fldLightSet_t *)fldSkyDefault)[fldSkyTable];
        }
        fldSkySetFilter(fldRunDanteGunFlag[13], fldRunDanteGunFlag[15], 0);
        fldSetFilterAnime(fldRunDanteGunFlag[16]);
        vec[0] = light->drawColorFixedR * 0.00390625f;
        vec[1] = light->drawColorFixedG * 0.00390625f;
        vec[2] = light->drawColorFixedB * 0.00390625f;
        vec[3] = 0;
        dds3SetFogColor(0, vec);
        dds3SetFogPara(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
        dir[0] = light->lightDirectionAX;
        dir[1] = light->lightDirectionAY;
        dir[2] = light->lightDirectionAZ;
        dir[3] = 0;
        dirp = dir;
        dds3SetLightDir(0, 0, dirp);
        vec[0] = light->lightColorAX;
        vec[1] = light->lightColorAY;
        vec[2] = light->lightColorAZ;
        vec[3] = 0;
        dds3SetLightColor(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0;
        dds3SetLightDir(0, 1, dirp);
        vec[0] = light->lightColorBX;
        vec[1] = light->lightColorBY;
        vec[2] = light->lightColorBZ;
        vec[3] = 0;
        dds3SetLightColor(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0;
        dds3SetLightDir(0, 2, dirp);
        vec[0] = light->lightColorCX;
        vec[1] = light->lightColorCY;
        vec[2] = light->lightColorCZ;
        vec[3] = 0;
        dds3SetLightColor(0, 2, vec);
        vec[0] = light->backgroundColorX;
        vec[1] = light->backgroundColorY;
        vec[2] = light->backgroundColorZ;
        vec[3] = 1.0f;
        dds3SetAmbientColor(0, vec);
        unit = evtGetUnitHandle2(D_0054EAE8);
        evtUnitLightOn(unit);
        r = (s32)(light->unitColorA[0] * 128.0f);
        g = (s32)(light->unitColorA[1] * 128.0f);
        b = (s32)(light->unitColorA[2] * 128.0f);
        lightColor = r | (b << 16) | (g << 8) | 0x80000000;
        r = (s32)(light->unitColorB[0] * 128.0f);
        g = (s32)(light->unitColorB[1] * 128.0f);
        b = (s32)(light->unitColorB[2] * 128.0f);
        ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, 0, lightColor, ambientColor);
        dir[0] = light->unitLightDirection[0];
        dir[1] = light->unitLightDirection[1];
        dir[2] = light->unitLightDirection[2];
        dir[3] = 0;
        SDF_VU0_LOADVECTOR(dirp);
        evtSetUnitLightDir(unit, 0);
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir(unit, 0);
    }
}

void fldSkyBattle(void) {
    fldLightSet_t *light = &((fldLightSet_t *)fldSkyDefaultData)[fldSkyTable];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->fadeArea;
    fldRunDanteGunFlag[13] = area;
    fldRunDanteGunFlag[14] = light->displayStateSlot14;
    value = light->fadeMode;
    fldRunDanteGunFlag[15] = value;
    fldRunDanteGunFlag[16] = light->swayMode;
    fldSkySetFilter(area, value, 0);
    fldSetFilterAnime(fldRunDanteGunFlag[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    dds3SetFogColor(0, vec);
    dds3SetFogPara(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    dds3SetLightDir(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    dds3SetLightColor(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    dds3SetLightDir(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    dds3SetLightColor(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    dds3SetLightDir(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    dds3SetLightColor(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
    vec[3] = 1.0f;
    dds3SetAmbientColor(0, vec);
}

void fldAllocPlaneTable(void) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(FIELD_VALUE_RECORD_STORAGE_SIZE);
    void *storage;

    D_0054EDAC = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldMatterTable = (u32)storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    allocation = sdfMemAllocDirect(FIELD_AUX_RECORD_STORAGE_SIZE);
    D_0054EDA8 = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldItemBoxObjBuf = (u32)storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

void fldFreeMatterTable(void) {
    sdfMemUnlock((sdfMemHandle_t *)D_0054EDAC);
    sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EDAC);
    fldMatterTable = 0;
    sdfMemUnlock((sdfMemHandle_t *)D_0054EDA8);
    sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EDA8);
    fldItemBoxObjBuf = 0;
}

f32 fldVecDot(sdfVector4_t *a, sdfVector4_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}


f32 MATH_VectorLength(sdfVector4_t *v)
{
    f32 len;

    EE_SQRT_F32(len, v->x * v->x + v->y * v->y + v->z * v->z);
    return len;
}


void fldMakeEveHitPlanes(f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant) {
    f32 firstPoint[4];
    f32 secondPoint[4];
    f32 thirdPoint[4];
    f32 inverseLength;

    firstPoint[0] = vertices[0];
    firstPoint[1] = vertices[1];
    firstPoint[2] = vertices[2];
    secondPoint[0] = vertices[4];
    secondPoint[1] = vertices[5];
    secondPoint[2] = vertices[6];
    thirdPoint[0] = vertices[8];
    thirdPoint[1] = vertices[9];
    thirdPoint[2] = vertices[10];
    *normalX = (secondPoint[1] - firstPoint[1]) * (thirdPoint[2] - secondPoint[2]) - (secondPoint[2] - firstPoint[2]) * (thirdPoint[1] - secondPoint[1]);
    *normalY = (secondPoint[2] - firstPoint[2]) * (thirdPoint[0] - secondPoint[0]) - (secondPoint[0] - firstPoint[0]) * (thirdPoint[2] - secondPoint[2]);
    *normalZ = (secondPoint[0] - firstPoint[0]) * (thirdPoint[1] - secondPoint[1]) - (secondPoint[1] - firstPoint[1]) * (thirdPoint[0] - secondPoint[0]);
    inverseLength = 1.0f / fsqrtf(*normalX * *normalX + *normalY * *normalY + *normalZ * *normalZ);
    *normalX = *normalX * inverseLength;
    *normalY = *normalY * inverseLength;
    *normalZ = *normalZ * inverseLength;
    if (*normalX > -0.0001f && *normalX < 0.0001f) {
        *normalX = 0.0f;
    }
    if (*normalY > -0.0001f && *normalY < 0.0001f) {
        *normalY = 0.0f;
    }
    if (*normalZ > -0.0001f && *normalZ < 0.0001f) {
        *normalZ = 0.0f;
    }
    *planeConstant = -(*normalX * thirdPoint[0] + *normalY * thirdPoint[1] + *normalZ * thirdPoint[2]);
}

dds3Basic_t *fldFindMatterIndex(s32 id) {
    s32 index = 0;

    if (D_0054EDA0 > 0) {
        fldValueRecord_t *record = (fldValueRecord_t *)fldMatterTable;
        do {
            if (record->id == id) {
                return record->worldNode;
            }
            index++;
            record++;
        } while (index < D_0054EDA0);
    }
    return 0;
}

void fldFindMatterIndexFrom(s32 id, dds3Basic_t *worldNode) {
    s32 index;

    for (index = 0; index < D_0054EDA0; index++) {
        if (((fldValueRecord_t *)fldMatterTable)[index].id == id) {
            ((fldValueRecord_t *)fldMatterTable)[index].worldNode = worldNode;
        }
    }
}

extern s32 D_0054ED90;
extern s32 fldHitChkMatter;

void fldClearMatterFlags(void) {
    s32 count = D_0054EDA0;

    if (count > 0) {
        dds3Basic_t **node = &((fldValueRecord_t *)fldMatterTable)->worldNode;
        do {
            count--;
            *node = NULL;
            node = (dds3Basic_t **)((u8 *)node + sizeof(fldValueRecord_t));
        } while (count != 0);
    }
    D_0054EDA0 = 0;
    fldHitChkMatter = -1;
    D_0054ED90 = -1;
    fldPlaneCount = 0;
    fldItemBoxObjCount = 0;
    if (fldMatterTable != 0) {
        fldFreeMatterTable();
    }
}

void fldMakePlane(fldCollisionDescriptor_t *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex);

INCLUDE_ASM(const s32, "field/fldDraw", fldMakePlane);

s32 HitPositionInside2(f32 *out, s32 mode, s32 count, f32 *pos, fldValueRecord_t *zone, f32 margin) {
    f32 probe[3];
    f32 planar[2];
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 planeIndex;
    f32 best = margin;

    if (mode == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else if (mode == 1) {
        probe[0] = pos[0];
        probe[1] = 0.0f;
        probe[2] = pos[2];
        planar[0] = pos[0];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    for (planeIndex = 0; planeIndex < count; planeIndex++) {
        dist = fldVecDot((sdfVector4_t *)probe, (sdfVector4_t *)zone->plane[planeIndex]) - zone->limit[planeIndex];
        slack = dist + margin;
        if (slack < 0.0f) {
            *out = -1.0f;
            return -1;
        }
        if (planar[0] < zone->bound[0] - margin || planar[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planar[0] || zone->bound[3] + margin < planar[1]) {
            *out = -1.0f;
            return -1;
        }
        if (dist < 0.0f) {
            if (mode == 1) {
                best = dist + 45.0f;
            } else {
                best = slack;
            }
            inside = 1;
        }
    }
    if (best < 0.001f) {
        best = -1.0f;
    }
    *out = best;
    return inside;
}

extern f32 fldHitChkWork[];
extern s32 fldGbChkHit(s32 area, s32 floor, s32 bit);
extern s32 fldGbChkNpc(s32 area, s32 floor, s32 bit);

void fldHitChkUnitElvOnly(f32 *position) {
    f32 current[4];
    f32 previous[4];
    f32 currentSave[4];
    f32 previousSave[4];
    f32 travel[4];
    f32 clearance;
    f32 radius;
    f32 distance;
    s32 index;
    s32 moved;
    s32 inside;

    current[0] = position[0];
    current[1] = position[1];
    current[2] = position[2];
    previous[0] = fldHitChkWork[0];
    previous[1] = fldHitChkWork[1];
    previous[2] = fldHitChkWork[2];
    if (fldGlobalBase.area == 1) {
        radius = 25.0f;
        current[1] = position[1] - 35.0f;
    } else if (fldGlobalBase.area < 10) {
        radius = 30.0f;
        current[1] = position[1] - 35.0f;
    } else {
        radius = 45.0f;
        current[1] = position[1] - 35.0f;
    }
    EE_ABS_F32(travel[0], current[0] - previous[0]);
    EE_ABS_F32(travel[2], current[2] - previous[2]);
    for (index = 0; index < D_0054EDA0; index++) {
        if (!(((fldValueRecord_t *)fldMatterTable)[index].face->attributes & 0x80)) {
            continue;
        }
        if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((fldValueRecord_t *)fldMatterTable)[index].sceneFlag) != 0) {
            continue;
        }
        if (((fldValueRecord_t *)fldMatterTable)[index].ownerIndex > 0 &&
            fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1,
                        ((fldValueRecord_t *)fldMatterTable)[index].ownerIndex) != 0) {
            continue;
        }
        currentSave[0] = current[0];
        currentSave[1] = current[1];
        currentSave[2] = current[2];
        previousSave[0] = previous[0];
        previousSave[1] = previous[1];
        previousSave[2] = previous[2];
        if (((fldValueRecord_t *)fldMatterTable)[index].worldNode != NULL) {
            moved = 1;
            current[0] = currentSave[0] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.x;
            current[1] = currentSave[1] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.y;
            current[2] = currentSave[2] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.z;
            previous[0] = previousSave[0] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.x;
            previous[1] = previousSave[1] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.y;
            previous[2] = previousSave[2] - ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.z;
        } else {
            moved = 0;
        }
        EE_ABS_F32(distance, fldVecDot((sdfVector4_t *)current,
                                       (sdfVector4_t *)((fldValueRecord_t *)fldMatterTable)[index].normal) -
                                 ((fldValueRecord_t *)fldMatterTable)[index].planeConstant);
        if (distance < radius) {
            HitPositionInside2(&clearance, ((fldValueRecord_t *)fldMatterTable)[index].mode,
                               ((fldValueRecord_t *)fldMatterTable)[index].count, current,
                               &((fldValueRecord_t *)fldMatterTable)[index], radius);
            if (clearance < 0.0f) {
                inside = 0;
            } else {
                inside = 1;
            }
            if (inside == 1 && ((fldValueRecord_t *)fldMatterTable)[index].worldNode != NULL) {
                fldGlobalBase.hitMatterIndex = index;
                ((fldValueRecord_t *)fldMatterTable)[index].previousPosition[0] =
                    ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.x;
                ((fldValueRecord_t *)fldMatterTable)[index].previousPosition[1] =
                    ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.y;
                ((fldValueRecord_t *)fldMatterTable)[index].previousPosition[2] =
                    ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.z;
            }
        }
        if (moved != 0) {
            current[0] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.x;
            current[1] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.y;
            current[2] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.z;
            previous[0] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.x;
            previous[1] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.y;
            previous[2] += ((fldValueRecord_t *)fldMatterTable)[index].worldNode->poseset->pos.z;
        }
    }
}

/* NON_MATCHING: 34 of 97 words differ (first at +0x48). */
#ifdef NON_MATCHING
void fldHitChkUnitElv(f32 *position) {
    f32 result[4];
    f32 previous[4];
    f32 delta[4];
    fldValueRecord_t *record;
    s32 index;
    f32 x;
    f32 y;
    f32 z;

    memset(delta, 0, sizeof(delta));
    delta[3] = 1.0f;
    fldGlobalBase.hitValid = 0;
    index = fldGlobalBase.hitMatterIndex;
    if (index == -1) {
        fldHitChkUnitElvOnly(position);
        index = fldGlobalBase.hitMatterIndex;
    }
    if (index == -1) {
        return;
    }
    record = &((fldValueRecord_t *)fldMatterTable)[index];
    if (record->worldNode != NULL) {
        delta[0] = record->worldNode->poseset->pos.x - record->previousPosition[0];
        delta[1] = record->worldNode->poseset->pos.y - record->previousPosition[1];
        delta[2] = record->worldNode->poseset->pos.z - record->previousPosition[2];
        record->previousPosition[0] = record->worldNode->poseset->pos.x;
        record->previousPosition[1] = record->worldNode->poseset->pos.y;
        record->previousPosition[2] = record->worldNode->poseset->pos.z;
    }
    if (fldGbChkHit(fldGlobalBase.area, fldGlobalBase.floor + 1,
                    record->sceneFlag) != 0) {
        return;
    }
    x = position[0] + delta[0];
    y = position[1] + delta[1];
    z = position[2] + delta[2];
    previous[0] = fldHitChkWork[0];
    previous[1] = fldHitChkWork[1];
    previous[2] = fldHitChkWork[2];
    fldGlobalBase.hitValid = 1;
    fldGlobalBase.hitX = x;
    fldGlobalBase.hitY = y;
    fldGlobalBase.hitZ = z;
    position[0] = x;
    position[1] = y;
    position[2] = z;
    fldHitChkWork[0] = x;
    fldHitChkWork[1] = y;
    fldHitChkWork[2] = z;
    result[0] = x;
    result[1] = y;
    result[2] = z;
}
#else
INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkUnitElv);
#endif

INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkUnitDamage);

INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkUnit);

INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkUnitVWM);

void func_0013B658(void) {
}

void func_0013B660(void) {
}

void func_0013B668(void) {
}

INCLUDE_ASM(const s32, "field/fldDraw", fldHitWM_Mask);

f32 HitPositionInside3(s32 axisMode, s32 planeCount, f32 margin, f32 *position, fldValueRecord_t *zone) {
    f32 projectedPosition[3];
    f32 planarPosition[2];
    f32 clearance = margin;
    f32 planeDistance;
    f32 marginDistance;
    s32 planeIndex;

    if (axisMode == 0) {
        projectedPosition[0] = 0.0f;
        projectedPosition[1] = position[1];
        projectedPosition[2] = position[2];
        planarPosition[0] = position[1];
        planarPosition[1] = position[2];
    } else if (axisMode == 1) {
        projectedPosition[0] = position[0];
        projectedPosition[1] = 0.0f;
        projectedPosition[2] = position[2];
        planarPosition[0] = position[0];
        planarPosition[1] = position[2];
    } else {
        projectedPosition[0] = position[0];
        projectedPosition[1] = position[1];
        projectedPosition[2] = 0.0f;
        planarPosition[0] = position[0];
        planarPosition[1] = position[1];
    }
    for (planeIndex = 0; planeIndex < planeCount; planeIndex++) {
        planeDistance = fldVecDot((sdfVector4_t *)projectedPosition, (sdfVector4_t *)zone->plane[planeIndex]) - zone->limit[planeIndex];
        marginDistance = planeDistance + margin;
        if (marginDistance < 0.0f || planarPosition[0] < zone->bound[0] - margin || planarPosition[1] < zone->bound[1] - margin ||
            zone->bound[2] + margin < planarPosition[0] || zone->bound[3] + margin < planarPosition[1]) {
            return -1.0f;
        }
        if (planeDistance < 0.0f) {
            clearance = marginDistance;
        }
    }
    if (clearance < 0.001f) {
        clearance = -1.0f;
    }
    return clearance;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkCamera);

INCLUDE_ASM(const s32, "field/fldDraw", fldHitChkEnemy);

void fldAddItemBoxObject(s32 id, f32 x, f32 y, f32 z, f32 extent, f32 height) {
    f32 origin[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 index;
    f32 minX, minY, minZ, maxX, maxZ;

    index = fldItemBoxObjCount;
    minX = x - extent;
    maxX = x + extent;
    minY = y - height;
    /* Build the front boundary before the back boundary, matching the
     * native corner ordering. The traced sched1 death notes then place
     * the final Z/extent use on minZ; local allocation keeps Z with maxZ. */
    maxZ = z + extent;
    minZ = z - extent;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.vertexCount = 8;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.faceCount = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.triangleCount = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.vertices = ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.faces = ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry.stopData = NULL;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[0][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[1][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[2][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][1] = minY;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[3][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[4][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][2] = maxZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[5][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][0] = minX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[6][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][0] = maxX;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][1] = y;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][2] = minZ;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].vertices[7][3] = 1.0f;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[1] = 1;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[2] = 5;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].vertexIndices[3] = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[0].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[0] = 1;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[1] = 3;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[2] = 7;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].vertexIndices[3] = 5;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[1].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[0] = 3;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[1] = 2;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[2] = 6;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].vertexIndices[3] = 7;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[2].special[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].attributes = 128;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].moveFloor = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].sound = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].stop = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].place = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].automap[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].automap[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[0] = 2;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[1] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[2] = 4;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].vertexIndices[3] = 6;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].encounterType = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].encounter = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].special[0] = 0;
    ((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].faces[3].special[1] = 0;
    fldMakePlane(&((fldWallMeshRecord_t *)fldItemBoxObjBuf)[index].geometry,
                  origin, dds3AllocObjectId(), 0, id);
    fldItemBoxObjCount++;
}

extern s32 datAddHp(datUnitWork_t *work, s32 hp);
extern s32 datAddMp(datUnitWork_t *work, s32 mp);
extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

void fldKaifukuTest(void) {
    s32 i;
    s32 ofs = 0;

    for (i = 0; i < 16; i++) {
        datAddHp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 9999);
        datAddMp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 9999);
        *(u16 *)((u8 *)ofs + (u32)DDS3_GBWK + 0xA62) = 0;
        ofs += 0xEC;
    }
    p2sdPlaySE(0x10, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldStatusSet);

extern s32 fldEveRngPrevIdx;
extern s32 fldEveRngHitIdx;
extern s32 fldEveHitCount;
void fldClearDoorEvents(void);

void fldClearEveRngTable(void) {
    s32 i;
    s32 j;

    fldEveRngCount = 0;
    gEveNum = 0;
    fldEveHitState = 0;
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 8; j++) {
            fldEveHitTable[i].corner[j][0] = 0.0f;
            fldEveHitTable[i].corner[j][1] = 0.0f;
            fldEveHitTable[i].corner[j][2] = 0.0f;
            fldEveHitTable[i].corner[j][3] = 1.0f;
        }
        fldEveHitTable[i].center[0] = 0.0f;
        fldEveHitTable[i].center[1] = 0.0f;
        fldEveHitTable[i].center[2] = 0.0f;
        fldEveHitTable[i].center[3] = 1.0f;
        for (j = 0; j < 6; j++) {
            fldEveHitTable[i].plane[j][0] = 0.0f;
            fldEveHitTable[i].plane[j][1] = 0.0f;
            fldEveHitTable[i].plane[j][2] = 0.0f;
            fldEveHitTable[i].plane[j][3] = 1.0f;
            fldEveHitTable[i].limit[j] = 0.0f;
        }
        gEvnRngPid[i] = 0;
        fldEveHitTable[i].unk10C = 0.0f;
        fldEveHitTable[i].unk110 = 0;
        fldEveHitTable[i].worldObjectKey = 0;
        fldEveHitTable[i].unk114 = 0.0f;
        fldEveHitTable[i].unk118 = 0;
        fldEveHitTable[i].unk11C = 0;
        fldEveHitTable[i].unk120 = 0;
        fldEveHitTable[i].roomId = -1;
        fldEveHitTable[i].unk124 = -1;
        fldEveHitTable[i].mode = 0;
        fldEveHitTable[i].entryIndex = -1;
        fldEveHitTable[i].axisMode = 0;
        fldEveHitTable[i].shapeKind = 0;
    }
    fldEveHitCount = 0;
    fldEveRngPrevIdx = -1;
    fldEveRngHitIdx = -1;
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    fldClearDoorEvents();
}

void fldEveHit_End(void) {
    s32 slotIndex;
    dds3Basic_t *scene;
    char *scriptName;
    fldTaskInfo_t *taskInfo;

    fldEveHitState = 1;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        gEvnRngPid[slotIndex] = 0;
    }
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    scene = dds3GetControlScene();
    if (scene != NULL) {
        for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
            taskInfo = (fldTaskInfo_t *)gEvnRng[slotIndex]->data;
            if (taskInfo->slot >= 0) {
                scriptName = dds3EventBasicGetResource((dds3Basic_t *)dds3SearchSceneBasic(
                    scene, fldEveHitDataTable[taskInfo->slot]->objectId, 0xD));
                if (scrSearchScriptName(scriptName) != NULL) {
                    dds3SceneBasicEndScript(dds3GetActiveScene(), scriptName);
                }
            }
        }
    }
}

u32 fldEveHit_AddEveData(fldEventDataHeader_t *data, dds3Basic_t *object) {
    u32 index = gEveNum;

    fldEveHitDataTable[index] = data;
    gEveNum = index + 1;
    return index;
}


INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_AddEveHit);

INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_ChkDir);

INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_ChkDir2);

s32 fldEveHitTestRect(f32 *pos, s32 idx) {
    f32 probe[3];
    f32 planar[2];
    fldRoomSidePlanes_t *sides;
    s32 i;
    f32 zero;

    if (fldEveHitTable[idx].axisMode == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    zero = 0.0f;
    sides = (fldRoomSidePlanes_t *)fldEveHitTable[idx].plane[1];
    for (i = 0; i < FIELD_ROOM_PLANE_COUNT - 2; i++) {
        if (fldVecDot((sdfVector4_t *)probe, (sdfVector4_t *)sides->plane[i]) -
                sides->limit[i] < zero) {
            return -1;
        }
    }
    return 0;
}

s32 fldEveHitTestPlanes(sdfVector4_t *pos, s32 idx) {
    fldRoomPlanes_t *planes = (fldRoomPlanes_t *)fldEveHitTable[idx].plane;
    s32 i;
    f32 zero = 0.0f;

    for (i = 0; i < FIELD_ROOM_PLANE_COUNT; i++) {
        if (zero < fldVecDot(pos, (sdfVector4_t *)planes->plane[i]) - planes->limit[i]) {
            return 0;
        }
    }
    return 1;
}

s32 fldEveHit_ScriptStart(char *name, s32 idx) {
    s32 pid;
    s32 mode;

    if (scrSearchScriptName(name) == NULL) {
        pid = dds3SceneBasicStartScript(dds3GetActiveScene(), name);
        gEvnRngPid[idx] = pid;
        gFldEveRngIdx = idx;
        mode = fldEveHitTable[idx].mode;
        gFldEveRngIdx_N = (mode == 1) ? idx : -1;
        gFldEveRngIdx_T = (mode == 2) ? idx : -1;
        return pid;
    }
    return -1;
}

s32 fldEveHit_ScriptEnd(char *flag, u32 slot) {
    gEvnRngPid[slot] = 0;
    if (scrSearchScriptName(flag) != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), flag);
        return 0;
    }
    return -1;
}

u32 fldEveHit_ScriptEnd2(u32 slot) {
    u32 *task = &gEvnRngPid[slot];

    if (dds3ExistProcess((dds3ProcessID_t *)*task) != 0) {
        dds3KillProcess((dds3ProcessID_t *)*task, 0);
    }
    *task = 0;
    return 0;
}

s32 fldEveHit_ChkEveHitAfter(void) {
    u8 *object;
    u32 state = fldDoorExitFlag;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && fldDoorExitMode[0] != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), (const char *)fldDoorExitMode);
    }
    fldDoorExitMode[0] = 0;
    fldDoorExitFlag = 0;
    object = fldDoor_Find3();
    if (scrSearchScriptName((char *)object) == 0) {
        dds3SceneBasicStartScript(dds3GetActiveScene(), (const char *)object);
    }
    return 1;
}

s32 fldEveHit_ChkOn(const char *name) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (gFldEveHitOnOffTbl[i].area == fldGlobalBase.area &&
            gFldEveHitOnOffTbl[i].floor == fldGlobalBase.floor + 1 &&
            strcmp(gFldEveHitOnOffTbl[i].name, name) == 0) {
            return evtBitCheck(gFldEveHitOnOffTbl[i].flag) != 0;
        }
    }
    return 1;
}

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531E08);

INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_ChkEveHit);

void fld_13F6B8(void) {
    s32 count = fldEveRngCount;
    s32 i = 0;
    if (count > 0) {
        fldFileResource_t **entry = gEvnRng;
        do {
            fldDrawPos((*entry)->transform);
            i++;
            entry++;
        } while (i < fldEveRngCount);
    }
}

void fldPurgeEveRngProcesses(void) {
    s32 count = fldEveRngCount;
    s32 slotIndex = 0;
    if (count > 0) {
        u32 *taskSlot = gEvnRngPid;
        do {
            if (dds3ExistProcess((dds3ProcessID_t *)*taskSlot) == 0) {
                *taskSlot = 0;
            }
            slotIndex++;
            taskSlot++;
        } while (slotIndex < fldEveRngCount);
    }
}

s32 fldEveHitResID_Find(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == task) {
            return gEvnRng[slotIndex]->id;
        }
    }
    return -1;
}

s32 fldFindEveRngIndex(u32 task) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == task) {
            return fldEveHitTable[slotIndex].roomId;
        }
    }
    return -1;
}

const char *fldEveHitResName_Find(u32 task) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == task) {
            return gEvnRng[slotIndex]->name;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_AddEveHitItemBox);

INCLUDE_ASM(const s32, "field/fldDraw", fldEveHit_AddEveHitNpc);

s32 fldCheckEveRngN(s32 npcidx) {
    s32 idx;
    fldEveRngBox_t *box;

    idx = gFldEveRngIdx_N;
    if (gFldKyouseiTalk != 0) {
        return 1;
    }
    if (idx == -1) {
        return 0;
    }
    box = (fldEveRngBox_t *)&fldEveHitTable[idx].unk120;
    if (box->hitKind == 1) {
        if (box->npcIndex == npcidx) {
            return 1;
        }
    }
    return 0;
}

s32 fldCheckEveRngT(s32 takaraidx) {
    s32 idx;
    fldEveRngBox_t *box;

    idx = gFldEveRngIdx_T;
    if (idx == -1) {
        return 0;
    }
    box = (fldEveRngBox_t *)&fldEveHitTable[idx].unk120;
    if (box->hitKind == 2) {
        if (box->takaraIndex == takaraidx) {
            return 1;
        }
    }
    return 0;
}

s32 fldCheckEveRngEncount(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] != 0) {
            return 1;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = fldInfTable.packs[packOrSetIndex].setIndex;
                    if (fldInfTable.sets[gFldInfIdx].kindArea.packed == 1) {
                        gFldInfIdx = candidateSet;
                        return D_002FF3E0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (fldInfTable.sets[packOrSetIndex].kindArea.packed == 1 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk2(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = fldInfTable.packs[packOrSetIndex].setIndex;
                    if (fldInfTable.sets[gFldInfIdx].kindArea.packed == 0) {
                        gFldInfIdx = candidateSet;
                        return D_002FF3E0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (fldInfTable.sets[packOrSetIndex].kindArea.packed == 0 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}

const char *fldInf_FindTypeEveNpc(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    gFldInfIdx = fldInfTable.packs[packOrSetIndex].setIndex;
                    return D_002FF3E0;
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if ((u16)fldInfTable.sets[packOrSetIndex].kindArea.packed < 2 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldInfChkFastPanel);

INCLUDE_ASM(const s32, "field/fldDraw", fldInfNext);

s32 fldInfGet(s32 row, s32 which) {
    s32 message = row - 10;
    s32 ret = 0;

    switch (which) {
    case 0:
        ret = fldInfTable.sets[gFldInfIdx].messages[message].kind;
        break;
    case 1:
        ret = fldInfTable.sets[gFldInfIdx].messages[message].message;
        break;
    }
    return ret;
}

void fldInfLoad(s32 field) {
    char path[64];
    char directory[32];
    devState_t *command;
    if (field < 200) {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, D_00531E98, directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldInfTable, sizeof(fldInfTable));
        sdfFileClose(command);
    }
}

void fldInfLoadFromMem(const void *source) {
    memcpy(&fldInfTable, source, sizeof(fldInfTable));
}

void fldInfToWAP(void) {
    s32 slot;
    char *actor;

    gFldInf_WpExit = 1;
    slot = gFldInf_HitIdx;
    actor = (char *)fldDoor_Find2(gEvnRng[slot]->name);
    if (scrSearchScriptName(actor) != NULL) {
        return;
    }
    gEvnRngPid[slot] = dds3SceneBasicStartScript(dds3GetActiveScene(), actor);
}

/* The native format slot includes twelve trailing zero bytes. */
const char D_00531E98[24] = "%sF%03d.INF";

extern u8 D_00302FA0[];
extern u8 D_00302FB0[];
extern u8 D_00302FC0[];
extern u8 D_00302FD0[];
extern u8 D_00302FF0[];
extern u8 D_00303000[];
extern u8 D_00303010[];
extern u8 D_00303030[];

u8 *fldFindEventEntry(const char *name) {
    s32 i;
    fldActorEntry_t *entry;

    if (name == NULL) return NULL;
    for (i = 0; i < 256; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->floor != fldGlobalBase.floor + 1) continue;
        if (strcmp(name, entry->name) != 0) continue;
        switch (entry->kind) {
        case 1:
            if (fldGlobalBase.area < 10) {
                if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 11) {
                    return D_00303010;
                }
                return D_00303010;
            }
            if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 10) {
                return D_00303000;
            }
            return D_00302FA0;
        case 2:
            return D_00302FB0;
        case 3:
            return D_00302FC0;
        case 4:
            if (fldGlobalBase.area >= 40) {
                return D_00303030;
            }
            return D_00302FD0;
        case 5:
            return D_00302FF0;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            return D_00303020;
        }
    }
    return NULL;
}

u8 *fldDoor_Find2(const char *name) {
    s32 i;
    fldActorEntry_t *entry;

    if (name == NULL) return NULL;
    for (i = 0; i < 256; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->floor != fldGlobalBase.floor + 1) continue;
        if (strcmp(name, entry->name) != 0) continue;
        scriptlabel = i;
        switch (entry->kind) {
        case 1:
            if (fldGlobalBase.area < 10) {
                if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 11) {
                    gFldWapPanel = 7;
                    return D_00303010;
                }
                gFldWapPanel = 7;
                return D_00303010;
            }
            if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 10) {
                gFldWapPanel = 6;
                return D_00303000;
            }
            gFldWapPanel = 0;
            return D_00302FA0;
        case 2:
            gFldWapPanel = 1;
            return D_00302FB0;
        case 3:
            gFldWapPanel = 2;
            return D_00302FC0;
        case 4:
            if (fldGlobalBase.area >= 40) {
                gFldWapPanel = 9;
                return D_00303030;
            }
            gFldWapPanel = 3;
            return D_00302FD0;
        case 5:
            gFldWapPanel = 5;
            return D_00302FF0;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            gFldWapPanel = 0;
            return D_00303020;
        }
    }
    return NULL;
}

u8 *fldDoor_Find3(void) {
    s32 idx = gFldWap_after_nextidx;
    fldActorEntry_t *entry = &fldDoorTable[idx];

    if (entry->floor == fldArea[0] + 1) {
        if (entry->kind == 10) {
            scriptlabel = idx;
            gFldWapPanel = 8;
            return D_00303020;
        }
    }
    return NULL;
}

s32 fldDoor_GetWarpEff(void)
{
    return fldDoorTable[scriptlabel].motion;
}


s32 fldWapGetDoor(void) {
    fldActorEntry_t *entry = &fldDoorTable[scriptlabel];
    s32 motion;

    if (entry->kind == 1) {
        motion = entry->motion;
        if (motion == 5 || entry->secondaryMotion == 5 || motion == 6 ||
        entry->secondaryMotion == 6 || motion == 7 ||
        entry->secondaryMotion == 7 || motion == 8 ||
        entry->secondaryMotion == 8) {
            return 0x28;
        }
        return 0x14;
    }
    return 0;
}

extern scrProcessWork_s *scrGetScriptHandle(void);
extern void fldSePlay(s32 soundId);
extern void fldFindDoorEvent(s32 handle);
extern u32 fldLoadNextGetStatus(void);
extern void fldStartBgmLoad(s32 kind);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeColorSet(s32 r, s32 g, s32 b);
extern void fldAnaStart(f32 x, f32 y, f32 z);
extern f32 fldPlayerWarpState;

void fldDoorRun_FindStart(s32 useScript) {
    s32 idx;
    fldActorEntry_t *entry;
    const char *name;

    if (fldGlobalBase.area < 10) {
        fldPlayerCalc_WarpStart();
        return;
    }
    if (useScript != 0) {
        name = fldEveHitResName_Find((u32)scrGetScriptHandle()->ProcessID);
        if (name == 0) return;
        if (fldDoor_Find2(name) == 0) return;
    }
    idx = scriptlabel;
    entry = &fldDoorTable[idx];
    if (entry->kind == 1) {
        if (entry->floor != fldGlobalBase.floor + 1) return;
        fldSePlay(entry->sound);
        fldFindDoorEvent(fldDoorEventTable[idx].unk04);
        if (entry->variant >= 6) return;
        if (entry->variant < 0) return;
        if (fldLoadNextGetStatus() != 0) return;
        fldStartBgmLoad(entry->variant);
        return;
    }
    if (entry->kind == 2) {
        if (entry->floor != fldGlobalBase.floor + 1) return;
        fldPlayerFastMode = 1;
        fldPlayerWarpState = *(f32 *)&fldDoorEventTable[idx].unk24;
        if (entry->motion == 1) {
            fldSePlay(0x38);
            dds3FadeOut(0xC0, 0xC0, 0xC0, 0xF);
            return;
        }
        dds3FadeOut(0, 0, 0, 0xF);
        return;
    }
    if (entry->kind == 3) {
        dds3FadeColorSet(0, 0, 0);
        fldAnaStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
        return;
    }
    if (entry->kind == 5) {
        if (entry->motion == 0) {
            gfldPlayerDamegeCnt = 100;
        } else {
            gfldPlayerDamegeCnt = -100;
        }
    } else if (entry->kind == 10) {
        fldSePlay(0x38);
    }
}

void fldDoor_ExitCamera(const char *name) {
    fldActorEntry_t *entry;
    char *entryName;
    dds3Basic_t *camera;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorParamTable.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 0) {
            continue;
        }
        entryName = entry->name;
        if (entry->floor == fldGlobalBase.floor + 1) {
            if (strcmp(name, entryName) == 0) {
                if (entry->variantMode == 2) {
                    return;
                }
                if (entry->variantMode == 0 && entry->linkKind == 3) {
                    camera = (dds3Basic_t *)dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 4,
                                                                                  entry->linkName);
                    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
                    fldPlayerOff();
                    return;
                }
            }
        }
    }
}

s32 fldDoor_ChkWarp(void)
{
    return gFldWapPanel == 8;
}


extern s32 gFldSndTransSrc;
extern s32 fldBgmTransChk(s32 id1, s32 id2);
extern void fldBgmFadeOut(void);
extern void fldBgmTrans(void);

s32 fldWap_SetBgm(fldActorEntry_t *entry) {
    s32 result = 0;

    if (fldGlobalBase.area != 0x22 && fldGlobalBase.area != 0x20 && fldGlobalBase.area != 0x25 &&
        fldGlobalBase.area != 0x24 && fldGlobalBase.area != 0x29 && fldGlobalBase.area != 0x2A &&
        fldGlobalBase.area != 0x2B && fldGlobalBase.area != 0x2C && fldGlobalBase.area != 0x2D) {
        if (fldGlobalBase.sequenceCode != entry->sequenceCode) {
            result = fldBgmTransChk(fldGlobalBase.sequenceCode, entry->sequenceCode);
            if (entry->sequenceCode != 0) {
                if (result == 0) {
                    fldBgmFadeOut();
                }
                if (result == 1) {
                    gFldSndTransSrc = fldGlobalBase.sequenceCode;
                }
                fldGlobalBase.sequenceCode = entry->sequenceCode;
                if (result == 1) {
                    fldBgmTrans();
                }
            } else if (result == 1) {
                gFldSndTransSrc = fldGlobalBase.sequenceCode;
                fldGlobalBase.sequenceCode = result;
                fldBgmTrans();
            }
        }
    }
    if (entry->selectedRoom != 0) {
        fldGlobalBase.playerSeTable = entry->selectedRoom - 1;
    }
    fldGlobalBase.cameraParamIndex = entry->rowIndex;
    return result;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDoor_Warp);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531F18);

INCLUDE_RODATA(const s32, "field/fldDraw", D_00531F28);

INCLUDE_RODATA(const s32, "field/fldDraw", jtbl_00531F40);

INCLUDE_ASM(const s32, "field/fldDraw", fldDoor_WarpEx);

INCLUDE_ASM(const s32, "field/fldDraw", fldSetTerminalState);

INCLUDE_ASM(const s32, "field/fldDraw", fldSetTerminalIndex);

void fldDoor_SiseExit(void) {
    fldSceneParam_t record;
    fldActorEntry_t *entry;
    s32 index;
    s32 state;
    s32 zone;
    s32 sub;
    s32 nextArea;
    s32 nextKind;
    s32 shortcut;

    if (D_0054EDF4 == 0) {
        return;
    }
    state = fldDoorSiseState;
    zone = D_0054EDFC;
    sub = D_0054EE00;
    fldDoorLoad(D_0054EDF4);
    for (index = 0; index < 0x100; index++) {
        entry = &fldDoorTable[index];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind != 7) {
            continue;
        }
        shortcut = (zone == 3 || zone == 4);
        if (shortcut == 0) {
            if (state != entry->floor) {
                continue;
            }
        }
        if (zone != entry->motion) {
            continue;
        }
        if (sub != entry->secondaryMotion) {
            continue;
        }
        if (entry->variantMode != 0) {
            continue;
        }
        nextArea = entry->variant;
        if (nextArea == 0) {
            nextArea = D_0054EDF4;
        }
        nextKind = entry->sequenceKind;
        if (nextKind == 0) {
            nextKind = fldDoorSiseState;
        }
        if (fldGlobalBase.area != nextArea) {
            fldBgmFadeOut();
        }
        fldWap_SetBgm(entry);
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = index + 1;
        }
        gFldNowDZone = entry->flags31;
        fldSetSceneParam4(&record, nextArea, nextKind, entry->sequenceName, entry->sequenceCode,
                          entry->linkKind, entry->linkName);
        if (shortcut != 0) {
            record.restore = 5;
        } else {
            record.restore = 4;
        }
        D_0054EDF4 = 0;
        fldDoorSiseState = 0;
        D_0054EDFC = 0;
        D_0054EE00 = 0;
        dds3AdminiCallSequence(6, &record, 0xA0, 0);
        return;
    }
}

extern s16 D_002FC1A0[][2];
extern s16 fldAllCnt[][2];
extern void fldSetSceneParamLoad(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                                 s32 code, s32 link, const char *subname);

void fldWap_Load(s32 all, s32 sub, fldSceneParam_t *record) {
    fldActorEntry_t *entry;
    s32 i;
    s32 field;
    s32 zone;
    s32 nextKind;

    if (all == 0) {
        field = D_002FC1A0[sub][0];
        zone = 3;
    } else {
        zone = 4;
        field = fldAllCnt[sub][0];
    }
    if (field == 0) return;
    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->kind != 7) continue;
        if (zone != entry->motion) continue;
        if (sub != entry->secondaryMotion) continue;
        if (entry->variantMode != 0) continue;
        fldGlobalBase.cameraParamIndex = entry->rowIndex;
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = i + 1;
        }
        if (entry->variant != 0) {
            field = entry->variant;
        }
        if (entry->sequenceKind != 0) {
            nextKind = entry->sequenceKind;
        } else {
            nextKind = 1;
        }
        gFldNowDZone = entry->flags31;
        if (field == 0) return;
        if (nextKind == 0) return;
        fldSetSceneParamLoad(record, field, nextKind, entry->sequenceName, entry->sequenceCode,
                             entry->linkKind, entry->linkName);
        record->restore = 5;
        if (fldGlobalBase.sequenceCode != entry->sequenceCode) {
            if (entry->sequenceCode != 0) {
                fldGlobalBase.sequenceCode = entry->sequenceCode;
            }
        }
        if (entry->selectedRoom != 0) {
            fldGlobalBase.playerSeTable = entry->selectedRoom - 1;
        }
        return;
    }
}

extern s16 D_002FC1A2[][2];
extern void fldSetNextField(s32 field, s32 parameter);
extern void fldAutoMapOpenBlock2(s32 area, s32 floor, s32 flagNumber);
extern void evtBitOn(s32 bit);

void fldDoor_EveBtlExit(s32 field, s32 zone) {
    fldSceneParam_t record;
    s32 block[4];
    s32 param[4];
    fldActorEntry_t *entry;
    s32 i;
    s32 nextArea;
    s32 nextKind;

    fldDoorLoad(field);
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->kind != 8) continue;
        if (entry->motion != zone) continue;
        if (entry->variantMode == 2) {
            fldSetNextField(0, 0);
            fldBgmFadeOut();
            fldAutoMapOpenBlock2(fldGlobalBase.area, fldGlobalBase.floor + 1, entry->sequenceValue);
            D_0054EDF4 = fldGlobalBase.area;
            fldDoorSiseState = fldGlobalBase.floor + 1;
            D_0054EDFC = entry->variant;
            D_0054EE00 = entry->sequenceKind;
            switch ((u32)entry->variant) {
            case 0:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1C, param, 4, 0);
                return;
            case 1:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1D, param, 4, 0);
                return;
            case 2:
                param[0] = entry->sequenceKind;
                dds3AdminiCallSequence(0x1B, param, 4, 0);
                return;
            case 3:
                block[0] = 0;
                block[1] = entry->sequenceKind;
                if (D_002FC1A2[block[1]][0] != 0) {
                    evtBitOn(D_002FC1A2[block[1]][0]);
                }
                DDS3_GBWK->previewX = block[0];
                DDS3_GBWK->previewY = block[1];
                dds3AdminiCallSequence(0x1F, block, 8, 0);
                return;
            case 4:
                block[0] = 1;
                block[1] = entry->sequenceKind;
                DDS3_GBWK->previewX = block[0];
                DDS3_GBWK->previewY = block[1];
                dds3AdminiCallSequence(0x1F, block, 8, 0);
                return;
            case 5:
                dds3AdminiCallSequence(0x1E, 0, 0, 0);
                return;
            }
            return;
        }
        if (entry->variantMode != 0) continue;
        nextArea = field;
        if (entry->variant != 0) {
            nextArea = entry->variant;
        }
        nextKind = entry->sequenceKind;
        if (fldGb != nextArea) {
            fldBgmFadeOut();
        }
        fldWap_SetBgm(entry);
        if (entry->flags54 != 0) {
            fldDoorExitFlag = entry->flags54;
            strcpy((char *)fldDoorExitMode, entry->taskName);
            gFldWap_after_nextidx = i + 1;
        }
        gFldNowDZone = entry->flags31;
        fldSetSceneParam4(&record, nextArea, nextKind, entry->sequenceName, entry->sequenceCode,
                          entry->linkKind, entry->linkName);
        record.restore = 6;
        dds3AdminiCallSequence(6, &record, 0xA0, 0);
        return;
    }
}

void fldDoor_WarpCash(const char *name) {
    fldActorEntry_t *entry;
    s32 area;
    s32 room;
    s32 i;

    if (name == NULL) {
        return;
    }
    for (i = 0; i < 0x100; i++) {
        entry = &fldDoorParamTable.actors[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) {
            continue;
        }
        if (entry->kind == 1 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 2 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 3 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 4 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        } else if (entry->kind == 5 && entry->variantMode == 0) {
            if (entry->floor == fldGlobalBase.floor + 1 && strcmp(name, entry->name) == 0) {
                area = entry->variant ? entry->variant : fldGlobalBase.area;
                room = entry->sequenceKind;
                if (room == 0) {
                    room = fldGlobalBase.floor + 1;
                }
                fldLoadNextFile(area, room);
                return;
            }
        }
    }
}

s32 fldDoor_GetHasigoTbl(s32 kind) {
    fldActorEntry_t *entry = &fldDoorTable[scriptlabel];
    dds3Basic_t *node;

    switch (kind) {
    case 0:
        return entry->motion;
    case 1:
        node = dds3SceneBasicSearchName(dds3GetActiveScene(), entry->motionName);
        if (node != NULL) {
            return node->serialID;
        }
        /* fallthrough */
    case 2:
        node = dds3SceneBasicSearchName(dds3GetActiveScene(), entry->otherName);
        if (node != NULL) {
            return node->serialID;
        }
        break;
    }
    return 0;
}

s32 fldDoor_GetElvTbl(u32 kind) {
    s32 slot = gFld_ElvTakasa;

    switch (kind) {
    case 0:
        return fldDoorParamTable.headers[slot].floorCount;
    case 1:
        return fldDoorParamTable.headers[slot].sound;
    case 2:
        return fldDoorParamTable.headers[slot].body.data[0];
    case 3:
        return fldDoorParamTable.headers[slot].body.data[1];
    case 4:
        return fldDoorParamTable.headers[slot].body.data[2];
    case 5:
        return fldDoorParamTable.headers[slot].body.data[3];
    case 6:
        return fldDoorParamTable.headers[slot].body.data[4];
    case 7:
        return fldDoorParamTable.headers[slot].body.data[5];
    case 8:
        return fldDoorParamTable.headers[slot].body.data[6];
    case 9:
        return fldDoorParamTable.headers[slot].body.data[7];
    case 10:
        return fldDoorParamTable.headers[slot].body.data[8];
    case 11:
        return fldDoorParamTable.headers[slot].body.data[9];
    case 12:
        return fldDoorParamTable.headers[slot].body.data[10];
    case 13:
        return fldDoorParamTable.headers[slot].body.data[11];
    case 14:
        return fldDoorParamTable.headers[slot].floorCount - fldDoorWarpTarget - 1;
    }
    return 0;
}

s32 fldDoor_ElvTakasaToFloor(s32 index) {
    s32 slot = gFld_ElvTakasa;
    s32 count = fldDoorParamTable.headers[slot].floorCount;

    if (count - 1 < index) {
        return count - fldDoorWarpTarget - 1;
    }
    index = count - index - 1;
    return fldDoorParamTable.headers[slot].body.data[index];
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDoor_ElvExit);

void fldClearDoorEvents(void) {
    fldDoorEvent_t *entry = fldDoorEventTable;
    s32 i;

    for (i = 0; i < 256; i++, entry++) {
        entry->unk00 = 0;
        entry->unk04 = 0;
        entry->unk08 = 0;
        entry->unk0C = -1;
        entry->unk10 = 0;
        entry->unk14 = -1;
        entry->unk18 = 0;
        entry->unk1C = 0;
        entry->unk24 = 0;
        entry->unk28 = 0;
        entry->unk2C = 0;
        entry->unk30 = 0;
        entry->unk34 = 0;
        entry->unk38 = 0;
        entry->unk3C = 0;
        entry->unk40 = 0;
        entry->unk44 = 0;
        entry->unk48 = 0;
        entry->unk4C = 0;
        entry->unk50 = 0;
    }
    scriptlabel = -1;
}

void fldDoorLoad(s32 field) {
    char path[64];
    char directory[32];
    devState_t *command;
    if (field >= 100) {
        memset(&fldDoorParamTable, 0, sizeof(fldDoorParamTable));
    } else {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, "%sF%03d.WAP", directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldDoorParamTable, sizeof(fldDoorParamTable));
        sdfFileClose(command);
    }
}

void fldGetDoorEntry(const void *source) {
    memcpy(&fldDoorParamTable, source, sizeof(fldDoorParamTable));
}

INCLUDE_ASM(const s32, "field/fldDraw", fldSetupDoorScenes);

void fldFindDoorEvent(s32 id) {
    fldDoorEvent_t *e;
    fldActorEntry_t *entry;
    s32 i;

    if (id == 0) {
        return;
    }
    if (id == -1) {
        return;
    }
    e = fldDoorEventTable;
    entry = fldDoorTable;
    for (i = 0; i < 256; i++) {
        if (e->unk00 == 1 && e->unk04 == id) {
            s32 motion = entry->motion;

            e->unk00 = 2;
            e->unk18 = 0;
            e->unk1C = 0;
            if (motion == 5 || entry->secondaryMotion == 5 || motion == 6 ||
                entry->secondaryMotion == 6 || motion == 7 ||
                entry->secondaryMotion == 7 || motion == 8 ||
                entry->secondaryMotion == 8) {
                e->unk20 = 0x28;
            } else {
                e->unk20 = 0x14;
            }
            e->unk24 = 0.0f;
            e->unk28 = 0.0f;
            e->unk2C = 0.0f;
            e->unk3C = 0.0f;
            e->unk40 = 0.0f;
            e->unk44 = 0.0f;
        }
        e++;
        entry++;
    }
}

s32 fldDoor_Step(s32 id, f32 *px, f32 *py, f32 *pz) {
    f32 steps[50] = {
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
        0.5f, 1.4f, 1.0f, 0.5f, 1.0f, 0.6f, 1.2f, 0.8f, 1.2f, 0.8f,
    };
    fldDoorEvent_t *e;
    s32 i;

    if (fldGb >= 200) {
        return 0;
    }
    for (i = 0; i < 256; i++) {
        e = (fldDoorEvent_t *)((u8 *)(i * 0x54) + (u32)fldDoorEventTable);
        if (e->unk0C == id) {
            if (e->unk00 == 2) {
                s32 count = e->unk18;
                s32 limit = e->unk20;
                if (count < limit) {
                    if (limit == 0x28) {
                        f32 step = steps[count];
                        e->unk24 += e->unk30 * step;
                        e->unk28 += e->unk34 * step;
                        e->unk2C += e->unk38 * step;
                    } else {
                        e->unk24 += e->unk30;
                        e->unk28 += e->unk34;
                        e->unk2C += e->unk38;
                        if (evtBitCheck(0xAF3) != 0) {
                            e->unk24 += e->unk30;
                            e->unk28 += e->unk34;
                            e->unk2C += e->unk38;
                        }
                    }
                    e->unk18 = e->unk18 + 1;
                } else if (evtBitCheck(0xAF3) != 0) {
                    e->unk00 = 3;
                    e->unk18 = 0;
                }
            } else if (e->unk00 == 3) {
                if (e->unk18 < 0x14) {
                    e->unk24 -= e->unk30;
                    e->unk28 -= e->unk34;
                    e->unk2C -= e->unk38;
                    if (evtBitCheck(0xAF3) != 0) {
                        e->unk24 -= e->unk30;
                        e->unk28 -= e->unk34;
                        e->unk2C -= e->unk38;
                    }
                    e->unk18 = e->unk18 + 1;
                } else {
                    e->unk18 = 0;
                    e->unk00 = 1;
                }
            } else {
                continue;
            }
            *px = e->unk24;
            *py = e->unk28;
            *pz = e->unk2C;
            return 1;
        }
        if (e->unk14 == id) {
            if (e->unk00 == 2) {
                s32 count = e->unk1C;
                s32 limit = e->unk20;
                if (count < limit) {
                    if (limit == 0x28) {
                        f32 step = steps[count];
                        e->unk3C += e->unk48 * step;
                        e->unk40 += e->unk4C * step;
                        e->unk44 += e->unk50 * step;
                    } else {
                        e->unk3C += e->unk48;
                        e->unk40 += e->unk4C;
                        e->unk44 += e->unk50;
                    }
                    e->unk1C = count + 1;
                } else if (evtBitCheck(0xAF3) != 0) {
                    e->unk1C = 0;
                }
            } else if (e->unk00 == 3) {
                if (e->unk1C < 0x14) {
                    e->unk3C -= e->unk48;
                    e->unk40 -= e->unk4C;
                    e->unk44 -= e->unk50;
                    e->unk1C = e->unk1C + 1;
                } else {
                    e->unk1C = 0;
                }
            } else {
                continue;
            }
            *px = e->unk3C;
            *py = e->unk40;
            *pz = e->unk44;
            return 1;
        }
    }
    return 0;
}

void func_00144288(void) {
}

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC44);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrwZBuf);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC4C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC50);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC58);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrawEnv);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC60);

INCLUDE_SDATA(const s32, "field/fldDraw", hPartialMultiBlur);

INCLUDE_SDATA(const s32, "field/fldDraw", gencMode);

INCLUDE_SDATA(const s32, "field/fldDraw", gencStageInnAlp);

INCLUDE_SDATA(const s32, "field/fldDraw", encDrawState);

INCLUDE_SDATA(const s32, "field/fldDraw", gencOn);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC7C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC80);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC88);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamDist);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamMiwatasi);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC94);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC98);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EC9C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECA0);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamHeight);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamParamTable2);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamResetOn);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECB0);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECB4);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECB8);

INCLUDE_SDATA(const s32, "field/fldDraw", fldElvCameraScene);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamElvState);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECC4);

INCLUDE_SDATA(const s32, "field/fldDraw", fldHitChkMatter);

INCLUDE_SDATA(const s32, "field/fldDraw", fldHitChkSlope);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamPhokanState);

INCLUDE_SDATA(const s32, "field/fldDraw", fldCamPHokanMode);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECD8);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECDC);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlayerWarpState);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlayerFastMode);

INCLUDE_SDATA(const s32, "field/fldDraw", gfldPlayerHasiCnt);

INCLUDE_SDATA(const s32, "field/fldDraw", gfldPlayerDamegeCnt);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlayerStepDist);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlayerMoveState);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlayerSyukanState);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ECFC);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED00);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED04);

INCLUDE_SDATA(const s32, "field/fldDraw", fldWmWarpMode);

INCLUDE_SDATA(const s32, "field/fldDraw", fldWmapPlayerState);

INCLUDE_SDATA(const s32, "field/fldDraw", fldWmFastMove);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSyukanState);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyEnable);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyDefault);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyDefaultData);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyDataTable);

INCLUDE_SDATA(const s32, "field/fldDraw", gFilterAlpha);

INCLUDE_SDATA(const s32, "field/fldDraw", gFilterAnime);

INCLUDE_SDATA(const s32, "field/fldDraw", gFilterAnimeCntF);

INCLUDE_SDATA(const s32, "field/fldDraw", gFilterAnimeOfsA);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyFilterPrev);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyFilter);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyFilterRate);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyFilterTime);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyTable);

INCLUDE_SDATA(const s32, "field/fldDraw", gSkyTblAnime);

INCLUDE_SDATA(const s32, "field/fldDraw", gSkyTblAnimeCntI);

INCLUDE_SDATA(const s32, "field/fldDraw", gSkyTblBaseIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyTblUpperIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED5C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED60);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED64);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED68);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED6C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED70);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED74);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyTableIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED7C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED80);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED84);

INCLUDE_SDATA(const s32, "field/fldDraw", fldSkyLightMaTbl);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED8C);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054ED90);

INCLUDE_SDATA(const s32, "field/fldDraw", fldItemBoxObjBuf);

INCLUDE_SDATA(const s32, "field/fldDraw", fldItemBoxObjCount);

INCLUDE_SDATA(const s32, "field/fldDraw", fldMatterTable);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDA0);

INCLUDE_SDATA(const s32, "field/fldDraw", fldPlaneCount);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDA8);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDAC);

INCLUDE_SDATA(const s32, "field/fldDraw", fldEveRngPrevIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", fldEveRngHitIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", fldEveRngCount);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "field/fldDraw", gEveNum);

INCLUDE_SDATA(const s32, "field/fldDraw", fldEveHitState);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldKyouseiTalk);

INCLUDE_SDATA(const s32, "field/fldDraw", fldEveHitCount);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldInfIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDoorExitFlag);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDE5);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldWap_after_nextidx);

INCLUDE_SDATA(const s32, "field/fldDraw", gFld_ElvTakasa);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDoorWarpTarget);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDF4);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDoorSiseState);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EDFC);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EE00);

INCLUDE_SDATA(const s32, "field/fldDraw", scriptlabel);

INCLUDE_SDATA(const s32, "field/fldDraw", gFldWapPanel);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0054EE0C);

