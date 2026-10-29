#ifndef FIELD_H
#define FIELD_H

#include "common.h"
#include "sdf_types.h"

typedef struct {
    struct dds3Basic_t *handle;
    struct dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

typedef struct {
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
    struct dds3Basic_t *handle;
} fldFollowCameraControl_t;

/* Field collision record shared by the hit and camera code. */
typedef struct collision_s collision_s;
struct collision_s {
    s16 mode2D;
    s16 numedges;
    f32 normal[4];
    f32 num;
    f32 e_normal[4][4];
    f32 e_num[4];
    f32 min[4];
    f32 max[4][4];
    f32 point[3];
    u32 elv;
    u16 blockID;
    u16 hitflag;
    s32 resID;
    struct dds3Basic_t *matter;
    struct fldCollisionFace_t *pFace;
    void *pTome;
    s32 hojiID;
    s32 npcID;
};

typedef struct fldAmXYZ_t
{
    f32 x;
    f32 y;
    f32 z;
} fldAmXYZ_t;

typedef struct fldAmIcon_t
{
    u32 type;
    fldAmXYZ_t *position;
} fldAmIcon_t;

typedef struct fldAmSBlock_t
{
    char *name;
    u32 nodeIndex;
    struct fldAmIcon_t *icons;
    u32 iconCount;
    s32 floor;
    fldAmXYZ_t *pointA;
    fldAmXYZ_t *pointB;
} fldAmSBlock_t;

typedef struct fldAmAreaData_t
{
    char *name;
    fldAmSBlock_t *items;
    u32 nSB;
    void *pMdl;
    fldAmXYZ_t *pPos;
} fldAmAreaData_t;

union fldTriggerShape_t;

typedef struct fldResPut_t
{
    u32 type;
    s32 eventIndex;
    u32 visible;
    union {
        void *parameters;
        union fldTriggerShape_t *shape;
    } data;
} fldResPut_t;

typedef struct fldResourcePoint_t
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t scl;
} fldResourcePoint_t;

extern fldResourcePoint_t fldPlayerPosRotWork;

/* Counted filename tables use a variable-length trailing array. The native
 * filename entry is 0x0C bytes and the file-resource row is 0x24 bytes. */
typedef struct fldFileNameEntry_t
{
    u32 word00;
    const char *name;
    u32 word08;
} fldFileNameEntry_t;

typedef struct fldFileNameTable_t
{
    u32 count;
    fldFileNameEntry_t entries[1];
} fldFileNameTable_t;

typedef struct fldResHead_t
{
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    fldResourcePoint_t *transform;
    u32 word14;
    fldFileNameTable_t *names;
    u32 word1C;
    void *data;
} fldResHead_t;

/* Shared view of fldGlobalBase through the last recovered position component.
 * Unknown gaps remain opaque; this is not the complete global allocation. */
typedef struct fldAreaWork_t
{
    u32 flags;                  /* 0x000 */
    s32 area;                   /* 0x004 */
    s32 floor;                  /* 0x008: zero-based */
    s32 blockID;                /* 0x00C */
    s32 word10;                 /* 0x010 */
    u32 consumedFlags;          /* 0x014 */
    s32 mode;                   /* 0x018 */
    s32 word1C;                 /* 0x01C */
    char requestedSceneName[0x10]; /* 0x020 */
    sdfVector4_t campos;        /* 0x030 */
    s32 playerSeTable;          /* 0x040 */
    s32 word44;                 /* 0x044 */
    s32 hitMatterIndex;         /* 0x048 */
    s32 word4C;                 /* 0x04C */
    char *retryScript;          /* 0x050 */
    struct dds3Basic_t *fastArea;      /* 0x054: the first area object built from the FL2 block table */
    u8 unk58[4];
    s32 word5C;                 /* 0x05C */
    s32 amSBlock;               /* 0x060 */
    s32 amFloor;                /* 0x064 */
    s32 nameU;                  /* 0x068 */
    s32 nameD;                  /* 0x06C */
    s32 menuOpenFlag;           /* 0x070 */
    s32 encountState;           /* 0x074 */
    s32 word78;                 /* 0x078 */
    s32 previousFloor;          /* 0x07C: last floor fldFreeFile recorded */
    s32 word80;                 /* 0x080 */
    s32 resourceFlag;           /* 0x084 */
    s32 resourceArea;           /* 0x088 */
    s32 resourceFloor;          /* 0x08C */
    s32 word90;                 /* 0x090 */
    s32 word94;                 /* 0x094 */
    s32 word98;                 /* 0x098 */
    s32 cameraParamIndex;       /* 0x09C */
    s32 sequenceCode;           /* 0x0A0 */
    s32 wordA4;                 /* 0x0A4 */
    s32 reuseAreaResources;     /* 0x0A8 */
    s32 wordAC;                 /* 0x0AC */
    s32 wordB0;                 /* 0x0B0 */
    f32 floatB4;                /* 0x0B4 */
    s32 lightMaIdx;             /* 0x0B8 */
    s32 wordBC;                 /* 0x0BC */
    s32 wordC0;                 /* 0x0C0 */
    u8 unkC4[4];
    union { s32 wordC8; f32 playerMoveDistance; }; /* 0x0C8 */
    s32 wordCC;                 /* 0x0CC */
    void *pacRes[10];           /* 0x0D0: handle and mapped pointer per pac entry */
    s32 wordF8;                 /* 0x0F8 */
    u8 unkFC[4];
    f32 x;                      /* 0x100 */
    f32 y;                      /* 0x104 */
    f32 z;                      /* 0x108 */
    f32 prevX;                  /* 0x10C: the position before this step */
    f32 prevY;                  /* 0x110 */
    f32 prevZ;                  /* 0x114 */
    f32 hitX;                   /* 0x118 */
    f32 hitY;
    f32 hitZ;
    f32 camAngle;               /* 0x124 */
    f32 camAngle2;              /* 0x128 */
    f32 float12C;               /* 0x12C */
    f32 float130;               /* 0x130 */
    s32 hitValid;               /* 0x134 */
} fldAreaWork_t;

extern fldAreaWork_t fldGlobalBase;

typedef struct {
    s32 type;
    s16 id;
    s16 num;
    s32 trap;
    s32 param;
} fldTakaraTbl_t;

extern fldTakaraTbl_t fldTakaraTable[];

typedef struct {
    u8 x;
    u8 y;
    u8 z;
    u8 w;
    u8 unk04[4];
    u32 routeOffset; /* byte offset from the map header */
    u32 routeCount;
} fldMapCell_t;

typedef struct {
    u32 firstOffset;
    u32 cellCount;
    u32 secondOffset;
} fldRunMapHeader_t;

typedef struct {
    u8 x;
    u8 z;
    u16 angle;
} fldRoutePoint_t;

typedef struct {
    s32 gunFlag;
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
    s32 skyTable;
    s32 word30;
    s32 filter;
    s32 word38;
    s32 word3C;
    s32 word40;
    s32 enabled;
    u32 unk48;
    s32 counter;
    s32 word50;
    s32 word54;
    u32 unk58;
    s32 word5C;
} fldDanteGunState_t;

typedef struct {
    union { u8 safeUnitPoint[0x30]; fldResourcePoint_t safeUnit; };
    u32 flag;
    s32 mode;
    u32 sequenceWord38;
    u32 pointResrcID;
    char pointResName[16];
    s32 loadField;
    s32 loadArea;
    s32 loadScript;
    s32 callmode;
    u16 bgm;
    s16 bgmMode;
    s32 camMode;
    u8 unk68[8];
    char camName[16];
    char FoceScript[16];
    u32 loadMode;
    u8 unk94[12];
} fldSceneParam_t;

typedef struct slFileWork_t {
    s32 w[5];
} slFileWork_t;

typedef struct GameFileInfo_t {
    u8 verstr[3];
    u8 ver;
    u8 TmnlMode;
    u8 TmnlId;
    u8 Lv;
    u8 mode;
    s32 PlayTime;
    union { struct { u16 endcnt; u16 newfile; }; s32 endCounterWord; };
    slFileWork_t work;
    u8 autoMapOpen;
    u8 autoMapOpenEX[15];
} GameFileInfo_t;

/* Native 0x24-byte resource collision face; attributes occupy the first word. */
typedef struct fldCollisionFace_t fldCollisionFace_t;
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

/* Field encounter control and the native placement table. */
#include "fldNpc_tag.h"
extern u32 fldEncountFlags;
extern fldNpcSlot_t fldModelSlotTable[32];
extern s32 fldNpcModelTable;

#endif /* FIELD_H */
