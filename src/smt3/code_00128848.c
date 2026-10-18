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
    u8 unk114[0xC];
    s16 unk120;
    s16 roomId;
    s16 unk124;
    s16 mode;
    s16 entryIndex;
    s16 axisMode;
    s32 shapeKind;
};

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







extern void dds3ObjectBaseOffFlag(void *object, s32 mask);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern dds3Basic_t *D_0054EAE8;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *spawnedObject);

extern u32 fldSkyTable;

extern u32 fldSkyTableIdx;

extern u32 fldSkyLightMaTbl;

extern u32 D_002FCAC0[];

extern u32 D_002FCAA0[];

extern f32 fldSkyPlayerIdx[];

extern void *fldSkyDefaultData;

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



/* fldDoorTable stride, 100 bytes; only the warp effect id is reached here. */
typedef struct {
    u8 unk00[0x12];
    s16 warpEff;
    u8 unk14[0x50];
} fldDoorData_t;

extern fldDoorData_t fldDoorTable[256];
extern s32 scriptlabel;
extern void *fldDrwZBuf;
extern f32 fldWmFastMove;
extern void *D_0054EC68;
extern s32 gfldPlayerHasiCnt;
extern s32 gfldPlayerDamegeCnt;
extern s32 fldPlayerStepDist;
extern s32 gSkyTblAnime;
extern s32 gSkyTblAnimeCntI;
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

    VU0_LOAD_MATRIX(D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    VU0_COPY_MATRIX_TO_SECONDARY();
    VU0_LOAD_VF10_FROM(*(const Quad *)vec);
    VU0_TRANSFORM_POINT_VF10();
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF11(&dds3ScreenParam.scale);
    VU0_MUL_VF10_VF11();
    VU0_LOAD_VF11(&dds3ScreenParam.trans);
    VU0_ADD_VF10_VF11();
    VU0_STORE_VF10(result);
    *dstX = result[0];
    *dstY = result[1];
}

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    VU0_LOAD_MATRIX(aProjMatrix);
    MultMatrixRev(&dds3ScreenParam2.TM);
    VU0_COPY_MATRIX_TO_SECONDARY();
    VU0_LOAD_VF10_FROM(*(const Quad *)vec);
    VU0_TRANSFORM_POINT_VF10();
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF11(&dds3ScreenParam2.scale);
    VU0_MUL_VF10_VF11();
    VU0_LOAD_VF11(&dds3ScreenParam2.trans);
    VU0_ADD_VF10_VF11();
    VU0_STORE_VF10(result);
    *dstX = result[0];
    *dstY = result[1];
}

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2_A);

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2_B);

void rsetOtOfs(s32 ofs)
{
    rsetFrameMaskOtOffset = ofs;
}


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

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxG);

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
    VU0_STORE_MATRIX(mat);
    packet = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(command, (u32)packet);
    packet = (u8 *)sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, &sprite->pos, sprite->col, sprite->uv, sprite->vt);
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "code_00128848", rsetFrameMaskRgbOn);

INCLUDE_ASM(const s32, "code_00128848", rsetFrameMaskRgbOff);

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

INCLUDE_ASM(const s32, "code_00128848", fldDrawPoly4);

INCLUDE_ASM(const s32, "code_00128848", fldDrawTexPoly4);

INCLUDE_ASM(const s32, "code_00128848", fldDrawFrameSprite);

INCLUDE_ASM(const s32, "code_00128848", fldDrawBokasi);

INCLUDE_ASM(const s32, "code_00128848", fldDrawFrameSpriteAlpha);

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

INCLUDE_ASM(const s32, "code_00128848", fldDrawPosCol);

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

    VU0_LOAD_MATRIX(D_002FA240);
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


INCLUDE_ASM(const s32, "code_00128848", encDrawProc);

void encFreeBlur(void)
{
    gencOn = 0;
    if (D_0054EC68 != NULL) {
        dds3EffectPartialMultiBlurDelete(D_0054EC68);
        D_0054EC68 = NULL;
    }
}


INCLUDE_ASM(const s32, "code_00128848", encStart);

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

INCLUDE_ASM(const s32, "code_00128848", fldCamCalcSlopeOffset);

INCLUDE_ASM(const s32, "code_00128848", fldCamSetLeng);

void fldCamResetNpc(void)
{
    fldCamResetOn = 0;
}


INCLUDE_ASM(const s32, "code_00128848", fldCamCalcSaka);

INCLUDE_ASM(const s32, "code_00128848", resetCamNormal);

INCLUDE_ASM(const s32, "code_00128848", resetCamNormal2);

INCLUDE_ASM(const s32, "code_00128848", fldCamGetPhokanParam);

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
    PS2_COPY_QUAD(D_002FC9C0, &nearPoint);
    PS2_COPY_QUAD(fldCamTarget, &farPoint);
    dds3SetPosPoseSet(control->camera, &farPoint.q);
    node = control->camera;
    ((s32 (*)(dds3Basic_t *))node->basicfunction->mainprocuder)(node);
    fldCamPhokanState++;
    if (fldCamPHokanMode < fldCamPhokanState) {
        fldGlobalBase.mode = 0;
    }
}

INCLUDE_ASM(const s32, "code_00128848", calcCamNormal);

INCLUDE_ASM(const s32, "code_00128848", calcCamDirLock);

INCLUDE_ASM(const s32, "code_00128848", calcCamNormalNPC);

INCLUDE_ASM(const s32, "code_00128848", resetCamTgtPlayer);

INCLUDE_ASM(const s32, "code_00128848", fldEventUpdateEx_Unity_2);

INCLUDE_ASM(const s32, "code_00128848", fldCamSetChaseCore);

void fldCamSetChase(void)
{
    fldCamSetChaseCore(0.0f);
}


void fldCamUpdateDefault(void)
{
    fldCamSetChaseCore(8.0f);
}


INCLUDE_ASM(const s32, "code_00128848", fldCamUpdateChase);

dds3Basic_t *fldCamNowEveCamera(void) {
    dds3Basic_t *worldObject = dds3GetActiveScene();
    dds3Basic_t *currentEntry = dds3GetActiveCameraSceneBasic(worldObject);

    if (fldGetUnitFollowCamera() == currentEntry) {
        currentEntry = NULL;
    }
    return currentEntry;
}

INCLUDE_ASM(const s32, "code_00128848", calcCamSyukan);

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

INCLUDE_RODATA(const s32, "code_00128848", D_00531BF0);

INCLUDE_ASM(const s32, "code_00128848", fldCamMain);

INCLUDE_ASM(const s32, "code_00128848", fldCamSyukanSuriawase);

s32 fldCamMiwatasiChk(void)
{
    return fldCamMiwatasi;
}


INCLUDE_ASM(const s32, "code_00128848", fldCamNpcCamChk);

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

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMirChk);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotion);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotionSe);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Hit);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_WarpRun);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalcHasigo);

INCLUDE_RODATA(const s32, "code_00128848", D_00531C50);

INCLUDE_RODATA(const s32, "code_00128848", D_00531C60);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Nml);

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


INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm3);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm4);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm2);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Nml_syukan);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D08);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D18);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D28);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D38);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D48);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc);

s32 fldPlayerChkAna(void)
{
    if (gfldPlayerHasiCnt == 0 && fldPlayerStepDist == 0 && gfldPlayerDamegeCnt == 0) {
        return 0;
    }
    return 1;
}


INCLUDE_ASM(const s32, "code_00128848", fldPlayerOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSvsOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSvsOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerAlpOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerAlpOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSdwOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSdwOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerFastSet);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotReset);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotReset2);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerPosRot);

INCLUDE_ASM(const s32, "code_00128848", fldSkyDefaultLoad);

INCLUDE_ASM(const s32, "code_00128848", fldSkyFieldLoad);

INCLUDE_ASM(const s32, "code_00128848", fldGetSkyEntry);

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

INCLUDE_ASM(const s32, "code_00128848", fldSkyDrawFilter);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetFilter);

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


INCLUDE_ASM(const s32, "code_00128848", fldSkyCalcTable);

INCLUDE_ASM(const s32, "code_00128848", fld_136800);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetPlayer);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTbl);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblModosu);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblUpper);

void fldSkySetTblLightMaDZ(s32 enabled) {
    if (enabled == 0) {
        dds3ObjectBaseOffFlag(D_0054EAE8, 0x100);
        return;
    }
    dds3ObjectBaseOnFlag(D_0054EAE8, 0x100);
    dds3UnitObjectBasicEL_LinkFree(D_0054EAE8);
}

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblLightMa);

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

INCLUDE_ASM(const s32, "code_00128848", fldSkyRefreshTbl);

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

INCLUDE_ASM(const s32, "code_00128848", fldClearMatterFlags);

void fldMakePlane(fldCollisionDescriptor_t *geometry, f32 *translation,
                   u32 id, s32 sceneFlag, s32 ownerIndex);

INCLUDE_ASM(const s32, "code_00128848", fldMakePlane);

s32 HitPositionInside2(f32 *out, s32 mode, s32 count, f32 margin, f32 *pos, fldValueRecord_t *zone) {
    f32 probe[3];
    f32 planar[2];
    f32 best = margin;
    f32 dist;
    f32 slack;
    s32 inside = 0;
    s32 planeIndex;

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

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitElvOnly);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitElv);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitDamage);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnit);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitVWM);

void func_0013B658(void) {
}

void func_0013B660(void) {
}

void func_0013B668(void) {
}

INCLUDE_ASM(const s32, "code_00128848", fldHitWM_Mask);

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

INCLUDE_ASM(const s32, "code_00128848", fldHitChkCamera);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkEnemy);

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

INCLUDE_ASM(const s32, "code_00128848", fldKaifukuTest);

INCLUDE_ASM(const s32, "code_00128848", fldStatusSet);

INCLUDE_ASM(const s32, "code_00128848", fldClearEveRngTable);

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


INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHit);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkDir);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkDir2);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitTestRect);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitTestPlanes);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ScriptStart);

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

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkOn);

INCLUDE_RODATA(const s32, "code_00128848", D_00531E08);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkEveHit);

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

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHitItemBox);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHitNpc);

INCLUDE_ASM(const s32, "code_00128848", fldCheckEveRngN);

INCLUDE_ASM(const s32, "code_00128848", fldCheckEveRngT);

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

INCLUDE_ASM(const s32, "code_00128848", fldInfChkFastPanel);

INCLUDE_ASM(const s32, "code_00128848", fldInfNext);

INCLUDE_ASM(const s32, "code_00128848", fldInfGet);

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

INCLUDE_ASM(const s32, "code_00128848", fldFindEventEntry);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Find2);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Find3);

s32 fldDoor_GetWarpEff(void)
{
    return fldDoorTable[scriptlabel].warpEff;
}


INCLUDE_ASM(const s32, "code_00128848", fldWapGetDoor);

INCLUDE_ASM(const s32, "code_00128848", fldDoorRun_FindStart);

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


INCLUDE_ASM(const s32, "code_00128848", fldWap_SetBgm);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Warp);

INCLUDE_RODATA(const s32, "code_00128848", D_00531F18);

INCLUDE_RODATA(const s32, "code_00128848", D_00531F28);

INCLUDE_RODATA(const s32, "code_00128848", jtbl_00531F40);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_WarpEx);

INCLUDE_ASM(const s32, "code_00128848", fldSetTerminalState);

INCLUDE_ASM(const s32, "code_00128848", fldSetTerminalIndex);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_SiseExit);

INCLUDE_ASM(const s32, "code_00128848", fldWap_Load);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_EveBtlExit);

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

INCLUDE_ASM(const s32, "code_00128848", fldDoor_GetHasigoTbl);

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

INCLUDE_ASM(const s32, "code_00128848", fldDoor_ElvExit);

INCLUDE_ASM(const s32, "code_00128848", fldClearDoorEvents);

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

INCLUDE_ASM(const s32, "code_00128848", fldSetupDoorScenes);

INCLUDE_ASM(const s32, "code_00128848", fldFindDoorEvent);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Step);

void func_00144288(void) {
}

INCLUDE_RODATA(const s32, "code_00128848", D_00532030);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC44);

INCLUDE_SDATA(const s32, "code_00128848", fldDrwZBuf);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC4C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC50);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC58);

INCLUDE_SDATA(const s32, "code_00128848", fldDrawEnv);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC60);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC68);

INCLUDE_SDATA(const s32, "code_00128848", gencMode);

INCLUDE_SDATA(const s32, "code_00128848", gencStageInnAlp);

INCLUDE_SDATA(const s32, "code_00128848", encDrawState);

INCLUDE_SDATA(const s32, "code_00128848", gencOn);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC7C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC80);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC88);

INCLUDE_SDATA(const s32, "code_00128848", fldCamDist);

INCLUDE_SDATA(const s32, "code_00128848", fldCamMiwatasi);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC94);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC98);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC9C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECA0);

INCLUDE_SDATA(const s32, "code_00128848", fldCamHeight);

INCLUDE_SDATA(const s32, "code_00128848", fldCamParamTable2);

INCLUDE_SDATA(const s32, "code_00128848", fldCamResetOn);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB0);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB4);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB8);

INCLUDE_SDATA(const s32, "code_00128848", fldElvCameraScene);

INCLUDE_SDATA(const s32, "code_00128848", fldCamElvState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECC4);

INCLUDE_SDATA(const s32, "code_00128848", fldHitChkMatter);

INCLUDE_SDATA(const s32, "code_00128848", fldHitChkSlope);

INCLUDE_SDATA(const s32, "code_00128848", fldCamPhokanState);

INCLUDE_SDATA(const s32, "code_00128848", fldCamPHokanMode);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECD8);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECDC);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerWarpState);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerFastMode);

INCLUDE_SDATA(const s32, "code_00128848", gfldPlayerHasiCnt);

INCLUDE_SDATA(const s32, "code_00128848", gfldPlayerDamegeCnt);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerStepDist);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerMoveState);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerSyukanState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECFC);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED00);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED04);

INCLUDE_SDATA(const s32, "code_00128848", fldWmWarpMode);

INCLUDE_SDATA(const s32, "code_00128848", fldWmapPlayerState);

INCLUDE_SDATA(const s32, "code_00128848", fldWmFastMove);

INCLUDE_SDATA(const s32, "code_00128848", fldSyukanState);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyEnable);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDefault);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDefaultData);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDataTable);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAlpha);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnime);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnimeCntF);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnimeOfsA);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterPrev);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilter);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterRate);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterTime);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTable);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblAnime);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblAnimeCntI);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblBaseIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTblUpperIdx);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED5C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED60);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED64);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED68);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED6C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED70);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED74);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTableIdx);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED7C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED80);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED84);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyLightMaTbl);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED8C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED90);

INCLUDE_SDATA(const s32, "code_00128848", fldItemBoxObjBuf);

INCLUDE_SDATA(const s32, "code_00128848", fldItemBoxObjCount);

INCLUDE_SDATA(const s32, "code_00128848", fldMatterTable);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDA0);

INCLUDE_SDATA(const s32, "code_00128848", fldPlaneCount);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDA8);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDAC);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngPrevIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngHitIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngCount);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "code_00128848", gEveNum);

INCLUDE_SDATA(const s32, "code_00128848", fldEveHitState);

INCLUDE_SDATA(const s32, "code_00128848", gFldKyouseiTalk);

INCLUDE_SDATA(const s32, "code_00128848", fldEveHitCount);

INCLUDE_SDATA(const s32, "code_00128848", gFldInfIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorExitFlag);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDE5);

INCLUDE_SDATA(const s32, "code_00128848", gFldWap_after_nextidx);

INCLUDE_SDATA(const s32, "code_00128848", gFld_ElvTakasa);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorWarpTarget);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDF4);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorSiseState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDFC);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EE00);

INCLUDE_SDATA(const s32, "code_00128848", scriptlabel);

INCLUDE_SDATA(const s32, "code_00128848", gFldWapPanel);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EE0C);

