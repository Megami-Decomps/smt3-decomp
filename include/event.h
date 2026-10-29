#ifndef EVENT_H
#define EVENT_H

#include "basic.h"
#include "model.h"
#include "motion.h"

typedef struct evtFrameInfo_t {
    u16 frame;
    u16 index;
    s32 nameIndex;
    union {
        union { s8 sb[0xC]; u16 sh[6]; s32 sw[3]; f32 sf[3]; } param;
        union {
            struct { u8 unk08[2]; u16 typeIndex; } f;
            s32 words[2];
            s16 shorts[4];
            s8 bytes[8];
            u8 unsignedBytes[8];
            u16 unsignedShorts[4];
            f32 values[2];
        } body;
    };
    union { void *work; void *payload; };
    struct evtFrameInfo_t *next;
    struct evtFrameInfo_t *prev;
} evtFrameInfo_t;

typedef struct evtViewerObject_s {
    s32 type;
    s32 entryHeader;
    s32 fileIndex;
    dds3Basic_t *basic;
    union {
        u8 unk10[8];
        struct { s16 major, minor; s32 reserve; } resource;
        struct { void *data; s32 reserve; } effect;
    };
    s32 childCount;
    evtFrameInfo_t *children;
    evtFrameInfo_t *lastChild;
    struct evtViewerObject_s *next;
    struct evtViewerObject_s *prev;
} evtViewerObject_s;

typedef struct {
    u16 mode;
    u16 unk02;
    u16 unk04;
    u16 unk06;
} evtViewerMenuEntry_t;

struct evtViewerInfo_t {
    void *block;
    s32 flags;                          /* 0x0004 */
    struct evtPolygonMovieWork_t *workdata;    /* 0x0008 */
    s32 totalframe;                     /* 0x000C */
    s32 firstframe;                     /* 0x0010 */
    s32 lastframe;                      /* 0x0014 */
    s32 nowframe;                   /* 0x0018 */
    s32 oldframe;                   /* 0x001C */
    s32 nametablecnt;                     /* 0x0020 */
    char nametable[256][32];            /* 0x0024 */
    dds3Basic_t *activecamera;          /* 0x2024 */
    u8 freecamera[4];
    dds3Basic_t *gamecamera;         /* 0x202C */
    s32 objcnt;                     /* 0x2030 */
    evtViewerObject_s *firstobjinfo;            /* 0x2034 */
    evtViewerObject_s *lastTrack;         /* 0x2038 */
    void *effectBasic[0x7F];            /* 0x203C */
    s32 framedisp;                        /* 0x2238 */
    evtViewerMenuEntry_t menuStack[8];  /* 0x223C */
    s32 sp;                      /* 0x227C */
    s32 nowmenu;                     /* 0x2280 */
    s32 menusw;                   /* 0x2284 */
    s32 fldfilter;
    /* Cancelled and cleared by the menu-step handler. */
    struct dds3ProcessID_t *menuProcess;
    s32 drawblur2;                      /* 0x2290 */
    s32 drawmblur;                      /* 0x2294 */
    s32 drawdblur;                 /* 0x2298 */
    s32 filter;                         /* 0x229C */
    s32 drawmfilter;                    /* 0x22A0 */
    s32 drawrblur;                     /* 0x22A4 */
    s32 fileMode;                       /* 0x22A8 */
    s32 work_base;                        /* 0x22AC */
    u8 work_x[4];
    s32 work_y;                        /* 0x22B4 */
    s32 eventpos_y;
    s32 select_y;                        /* 0x22BC */
    s32 menuItemCount;                  /* 0x22C0 */
    char *select_header;                        /* 0x22C4 */
    union { s32 *menuTable; char **itemNames; };                     /* 0x22C8 */
    s32 filename_x;                        /* 0x22CC */
    s32 filename_y;                        /* 0x22D0 */
    char nameStorage[0xD];              /* 0x22D4 */
    u8 unk22E1[7];
    union { char filename[0x14]; struct { char filenameText[0xC]; s32 objlist_y; s32 objlist_base; }; };                /* 0x22E8 */
    s32 framelist_x;                    /* 0x22FC */
    s32 frameFirst;                     /* 0x2300 */
    s32 frameCursor;                    /* 0x2304 */
    evtViewerObject_s *framelist_target;        /* 0x2308 */
    s32 framelist_type;
    s32 value;                          /* 0x2310 */
    s32 min;                       /* 0x2314 */
    s32 max;                       /* 0x2318 */
    f32 fvalue;
    f32 floatMin;
    f32 floatMax;                     /* 0x231C */
};

typedef struct evtViewerInfo_t evtViewerInfo_t;

typedef struct pmdUnitTable_t {
    s32 NameIndex, FileIndex, MajorNum, MinorNum, DataOffset, DataSize, Reserve1, Reserve2;
} pmdUnitTable_t;

typedef struct pmdStageTable_t {
    s32 NameIndex, FileIndex, MajorNum, MinorNum;
} pmdStageTable_t;

typedef struct pmdCutInfo_t {
    s32 FirstFrame;
    s32 LastFrame;
    s32 TotalFrame;
    s32 reserve;
} pmdCutInfo_t;

typedef struct pmdEntry_t {
    u32 type;
    u32 elementSize;
    u32 value;
    u32 offset;
} pmdEntry_t;

typedef struct pmdEffectTable_t {
    s32 NameIndex;
    s32 DataOffset;
    s32 Type;
    s32 reserve;
} pmdEffectTable_t;

typedef struct pmdHeader_t {
    u8 pad[0x10];
    s32 count;
    s32 kind;
    u8 pad2[8];
    pmdEntry_t entries[1];
} pmdHeader_t;

typedef struct pmdFrameTable_t {
    u16 ObjectType;
    u16 Frame;
    u16 Length;
    s16 NameIndex;
    union {
        s8 b[8];
        u8 ub[8];
        s16 h[4];
        u16 uh[4];
        s32 w[2];
        f32 f[2];
    } data;
} pmdFrameTable_t;

/* Interpolated event vector and packed colour. */
typedef struct evtBlendVecColor_t {
    f32 vec[4];
    u32 color;
} evtBlendVecColor_t;

/* Fog record in a polygon movie's scene table. */
typedef struct pmdSceneFogTable_t {
    evtBlendVecColor_t fog;
    s32 reserve[3];
} pmdSceneFogTable_t;

/* Scene-light records are the three vectors consumed by SLightFrameFunc. */
typedef struct {
    f32 color[4], direction[4], ambient[4];
} pmdSceneLightTable_t;

/* Event-unit allocation: evtCreateUnitHandle allocates/clears 0x148 bytes.
 * SMT3 omits DDS's extra motion, shadow and vertical-movement members. */
typedef enum evtUnitMotionState_t {
    EVT_UNIT_MOTION_STATE_IDLE = 0,
    EVT_UNIT_MOTION_STATE_SOURCE = 1,
    EVT_UNIT_MOTION_STATE_MOTION = 2,
    EVT_UNIT_MOTION_STATE_VECTOR = 3,
    EVT_UNIT_MOTION_STATE_VALUE = 4
} evtUnitMotionState_t;

typedef enum evtUnitTransitionSourceKind_t {
    EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR = 0,
    EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE = 1,
    EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE = 2
} evtUnitTransitionSourceKind_t;

enum evtUnitFlags_t {
    EVT_UNIT_FLAG_PATH_REVERSE = 0x4,
    EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR = 0x100,
    EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR = 0x200,
    EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS = 0x300,
    EVT_UNIT_FLAG_RGB_TRANSITION = 0x8000,
    EVT_UNIT_FLAG_ALPHA_TRANSITION = 0x10000,
    EVT_UNIT_FLAG_VALUE_CHANGED = 0x20000,
    EVT_UNIT_FLAG_TARGET_TRANSITION = 0x40000,
    EVT_UNIT_FLAG_TARGET_BLEND_IN = 0x80000,
    EVT_UNIT_FLAG_TARGET_BLEND_OUT = 0x100000,
    EVT_UNIT_FLAG_TARGET_BLEND_PHASES = 0x180000
};

typedef struct {
    s16 motionId;
    s16 startFrame;
    s16 endFrame;
} evtUnitMotionSetup_t;

typedef struct evtUnit_t {
    u32 color;
    union {
        s32 objectId;
        u32 firstCurrent;
    };
    union {
        u8 pad08[8];
        struct {
            u32 color08;
            u32 color0C;
        };
    };
    f32 vec10[4];
    f32 vec20[4];
    f32 vec30[4];
    union {
        u8 pad40[0x10];
        f32 vec40[4];
    };
    u32 color50;
    union {
        u8 pad54[0xC];
        struct {
            u32 color54;
            u32 color58;
            u32 color5C;
        };
    };
    u32 color60;
    union {
        u8 pad64[4];
        u32 color64;
    };
    void *endpointWork;
    u32 value;
    f32 targetVector[4];           /* 0x70 */
    dds3Basic_t *effObj;           /* 0x80 */
    s32 currentTransitionValue;
    s32 previousTransitionValue;
    struct mdlObj_t *owner;
    void *linkedUnit;
    s32 (*motionCallback)(struct evtUnit_t *, s32);
    f32 wobblePhase;
    s32 pathHandle;                /* 0x9C */
    f32 pathSpeed;
    u32 flags;                     /* 0xA4 */
    s16 motionState;               /* 0xA8 */
    s16 transitionSourceKind;
    s16 motionSubmode;
    s16 motionTicks;
    s16 motionParameter;
    s16 directionOffset;
    f32 transitionMotionScale;
    s16 waitMotion;
    s16 waitBlendStart;
    s16 waitBlendEnd;
    evtUnitMotionSetup_t motionSetup; /* 0xBE */
    s16 motionEndFrame;
    s16 movingTicks;
    s16 idleTicks;
    s8 firstSlot;
    s8 secondSlot;
    u8 slotFlags[12];              /* 0xCC */
    s16 tableValues[12];           /* 0xD8 */
    s16 slotA[12];
    s16 slotB[12];
    s16 slotC[12];
    union {
        s16 colorDuration;
        u16 colorFramesRemaining;
    };
    union {
        s16 directionDuration;
        u16 directionFramesRemaining;
    };
    union {
        u8 pad154[8];
        struct {
            u16 rgbElapsed;
            u16 rgbDuration;
            u16 alphaElapsed;
            u16 alphaDuration;
        };
    };
    s16 transitionElapsed;
    union {
        s16 transitionDuration;
        u16 transitionFrameCount;
    };
} evtUnit_t;



/* Transform prefix in the scene's type-17 source records. */
typedef struct {
    sdfVector4_t position;
    f32 rotation[4];
} evtWorldSourceTransformPrefix_t;

#endif /* EVENT_H */
