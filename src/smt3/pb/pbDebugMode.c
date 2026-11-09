#include "common.h"
#include "sdk_libgcc.h"
#include "pbRecord.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbProcHandle_t pbProcHandle_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 flg;
    s16 x, y;
    s32 dir;
    u8 dmy[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct
{
    s32 pcharNum;
    u8 unk004[0x14];
    s16 exitX, exitY;
    u8 unk01C[0x320];
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    s32 menuState;
    struct pbProcHandle_t *helpProc;
} pbMapWork_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 flg;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbExit_t;

typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { s16 x, y; } pbGoalPos_t;

typedef struct { u8 unk00[8]; pbGoalPos_t pos; } pbGoal_t;


/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

typedef struct
{
    sdfMemHandle_t *allocation;
    s16 phase;
    s16 menuCursor;
    s16 systemFileNum;
    s16 level;
    s16 stage;
} pbDebugWork_t;

extern u8 D_0054F584;

extern pbDebugWork_t *g_pdebug_0;

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern pbMapWork_t *pbMapWork;

extern pbGoal_t *goal;

extern pbProcHandle_t *record;

extern pbProcHandle_t *blockDB;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern u8 degree_mode;

void ReleaseDebug_pbDebugMode(void);

extern char pbDebugBlurOnText[];

extern char pbDebugBlurOffText[];

extern char pbDebugPanel2CursorMark[];

extern char pbDebugPanel2RowFormat[];

extern char pbDebugBlurValueFormat[];

extern char pbDebugBlurOffsetFormat[];

extern char pbDebugPanel3Title[];

extern char pbDebugPanel3CursorMark[];

extern char pbDebugPanel3RowFormat[];

extern char pbDebugChannelFormat[];

extern char D_0054F5A8[];

extern s32 testMsgTexIndex;

extern void *testMsgTexture;


typedef s32 (*pbDebugProcStep_t)(pbDebugWork_t *work, sdfDrawTag_t *tag);

extern pbDebugProcStep_t DebugProcTable[];
extern sdfGrOt_t sdfGrOtTable[];
extern sdfDrawTag_t *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void InitializeDebug_pbDebugMode(void);

/* D_002F6938 is sdfGrOtTable[81]; its regFunc accepts this draw tag. */
SYMBOL_AT(sdfGrOt_t, pbDebugModeOt81, sdfGrOtTable, 0xA20);

dds3ProcessFunc_t *pbDebugProc(dds3ProcessID_t *proc_id)
{
    sdfDrawTag_t *tag;
    s32 result;

    if (g_pdebug_0 == NULL) {
        InitializeDebug_pbDebugMode();
    }
    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    result = DebugProcTable[g_pdebug_0->phase](g_pdebug_0, tag);
    pbDebugModeOt81.regFunc(&pbDebugModeOt81, tag);
    return EE_ADDR_AS(dds3ProcessFunc_t *, (result != 0 ? -1 : 0));
}

void pbDebugShutdown(void) {
    ReleaseDebug_pbDebugMode();
}

void InitializeDebug_pbDebugMode(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x10);
    pbDebugWork_t *work = sdfMemLock(handle);
    work->phase = 0;
    work->menuCursor = 0;
    work->systemFileNum = 0;
    work->allocation = handle;
    work->level = pbMapWork->level;
    work->stage = pbMapWork->stage;
    g_pdebug_0 = work;
}

void ReleaseDebug_pbDebugMode(void) {
    if (g_pdebug_0 != NULL) {
        sdfMemFreeDirect(g_pdebug_0->allocation);
        g_pdebug_0 = NULL;
    }
}

extern s8 dds3PadButtonState[];
extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
extern void *sdfDevPrintf(s32 x, s32 y, s32 color, s32 attr, const char *format, ...);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, void *drawTag);

extern char *pbDebugMenuTextTable[];
extern char pbDebugMenuCursorMark[];

s32 DebugProcMenu_pbDebugMode(pbDebugWork_t *work, sdfDrawTag_t *tag)
{
    s32 i;
    s32 y;

    if ((dds3PadButtonState[0x36] & 2) != 0) {
        work->menuCursor--;
        if (work->menuCursor < 0) {
            work->menuCursor = 3;
        }
    } else if ((dds3PadButtonState[0x37] & 2) != 0) {
        work->menuCursor++;
        if (work->menuCursor >= 4) {
            work->menuCursor = 0;
        }
    }

    if (dds3PadButtonState[0x33] < 0) {
        return 1;
    }
    if (dds3PadButtonState[0x31] < 0) {
        switch (work->menuCursor) {
        case 0:
            work->phase = 1;
            break;
        case 1:
            work->phase = 2;
            break;
        case 2:
            work->phase = 3;
            break;
        case 3:
            work->phase = 4;
            break;
        }
    }

    dds3ConsPanelTagLink(tag, 16, 16, 24, 6);
    y = 0x7A00;
    for (i = 0; i < 4; i++) {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7380, y, 0xFFFFF0, 0,
                                         pbDebugMenuTextTable[i]));
        y += 0x60;
    }
    y = 0x7A00 + work->menuCursor * 0x60;
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, y, 0xFFFFF0, 0, pbDebugMenuCursorMark));
    return 0;
}

extern void pbGetCameraParam(f32 *out);
extern void pbSetCameraRadius(f32 radius);
extern void pbSetCameraDegree(f32 degree);
extern void pbSetCameraDegreeInit(f32 degree);
extern char pbDebugFloatFormat[];

s32 DebugProcRadius(pbDebugWork_t *work, sdfDrawTag_t *tag)
{
    f32 camera[3];
    f64 radiusValue;
    char *format;

    pbGetCameraParam(camera);
    if (dds3PadButtonState[0x33] < 0) {
        work->phase = 0;
    }
    if (dds3PadButtonState[0x36] || dds3PadButtonState[0x34]) {
        camera[1] -= 5.0f;
        if (camera[1] < 50.0f) {
            camera[1] = 50.0f;
        }
        pbSetCameraRadius(camera[1]);
    } else if (dds3PadButtonState[0x37] || dds3PadButtonState[0x35]) {
        camera[1] += 5.0f;
        if (1500.0f < camera[1]) {
            camera[1] = 1500.0f;
        }
        pbSetCameraRadius(camera[1]);
    }
    dds3ConsPanelTagLink(tag, 16, 16, 24, 4);
    format = pbDebugFloatFormat;
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, 0x7A00, 0xFFFFF0, 0,
                                     "CAMERA RADIUS (LENGTH)"));
    radiusValue = fptodp(camera[1]);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7980, 0x7A60, 0xFFFFF0, 0,
                                     format, radiusValue));
    return 0;
}

s32 DebugProcDegree(pbDebugWork_t *work, sdfDrawTag_t *tag)
{
    f32 camera[3];
    f64 degreeValue;
    char *format;

    pbGetCameraParam(camera);
    if (dds3PadButtonState[0x33] < 0) {
        work->phase = 0;
    }
    if (dds3PadButtonState[0x36] || dds3PadButtonState[0x34]) {
        camera[2] += 0.01f;
        if (1.560796142f < camera[2]) {
            camera[2] = 1.560796142f;
        }
        pbSetCameraDegree(camera[2]);
        pbSetCameraDegreeInit(camera[2]);
    } else if (dds3PadButtonState[0x37] || dds3PadButtonState[0x35]) {
        camera[2] -= 0.01f;
        if (camera[2] < 0.0f) {
            camera[2] = 0.0f;
        }
        pbSetCameraDegree(camera[2]);
        pbSetCameraDegreeInit(camera[2]);
    }
    dds3ConsPanelTagLink(tag, 16, 16, 24, 4);
    format = pbDebugFloatFormat;
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, 0x7A00, 0xFFFFF0, 0,
                                     "CAMERA DEGREE (RADIAN)"));
    degreeValue = fptodp(camera[2]);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7980, 0x7A60, 0xFFFFF0, 0,
                                     format, degreeValue));
    return 0;
}

extern void pbLoadSystemData(s32 num);
extern void pbRoundStagePrev(s16 *level, s16 *stage);
extern void pbRoundStageNext(s16 *level, s16 *stage);
extern void pbGotoNewStage(void);

s32 DebugProcStage(pbDebugWork_t *work, sdfDrawTag_t *tag)
{
    if (dds3PadButtonState[0x36] & 2) {
        pbRoundStagePrev(&work->level, &work->stage);
    }
    if (dds3PadButtonState[0x37] & 2) {
        pbRoundStageNext(&work->level, &work->stage);
    }
    if (dds3PadButtonState[0x33] < 0) {
        work->phase = 0;
    } else if (dds3PadButtonState[0x31] < 0) {
        pbMapWork->level = work->level;
        pbMapWork->stage = work->stage;
        pbGotoNewStage();
    }
    dds3ConsPanelTagLink(tag, 16, 16, 24, 3);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, 0x7A00, 0xFFFFF0, 0,
                                     "STAGE : %d -%2d", work->level + 1,
                                     work->stage + 1));
    return 0;
}

s32 DebugProcFree(pbDebugWork_t *work, sdfDrawTag_t *tag)
{
    if (dds3PadButtonState[0x36] & 2) {
        work->systemFileNum--;
        if (work->systemFileNum < 0) {
            work->systemFileNum = 49;
        }
    }
    if (dds3PadButtonState[0x37] & 2) {
        work->systemFileNum++;
        if (work->systemFileNum >= 50) {
            work->systemFileNum = 0;
        }
    }
    if (dds3PadButtonState[0x33] < 0) {
        work->phase = 0;
    } else if (dds3PadButtonState[0x31] < 0) {
        pbLoadSystemData(work->systemFileNum);
    }
    dds3ConsPanelTagLink(tag, 16, 16, 24, 3);
    sdfDrawTagLink(tag, sdfDevPrintf(0x7180, 0x7A00, 0xFFFFF0, 0,
                                     "DATA SELECT : %2d", work->systemFileNum));
    return 0;
}

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcxxxAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcxxxMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcxxxTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileHumanPcTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x169PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x168PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x167PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x166PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x165PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x164PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x163PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x162PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x161PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x160PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x15APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x159PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x158PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x157PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x156PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x155PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x154PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x153PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x152PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x150PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x14CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x14BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x14APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x148PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x147PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x146PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x145PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x144PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x143PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x142PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x141PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x12FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x12BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x129PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x126PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x125PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x124PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x123PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x122PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x121PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x120PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x11BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x11APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x119PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x118PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x117PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x116PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x114PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x110PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x10APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x108PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x103PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x102PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x101PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilBoss0x100PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xF00xFF0xFFPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCF0xCFPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCE0xCEPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCD0xCDPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCC0xCCPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCB0xCBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xCA0xCAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC90xC9PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC80xC8PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC70xC7PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC60xC6PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC50xC5PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC40xC4PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC30xC3PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC20xC2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC10xC1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xC00xC00xC0PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xB00xB20xB2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xB00xB10xB1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xB00xB00xB0PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAF0xAFPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAE0xAEPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAD0xADPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAC0xACPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAB0xABPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xAA0xAAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xA90xA9PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xA80xA8PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xA70xA7PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilDammyPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xA10xA1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0xA00xA00xA0PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x9D0x9DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x9C0x9CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x9B0x9BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x9A0x9APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x990x99PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x980x98PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x970x97PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x960x96PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x950x95PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x940x94PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x930x93PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x920x92PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x910x91PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x900x900x90PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8F0x8FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8E0x8EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8D0x8DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8C0x8CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8B0x8BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x8A0x8APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x890x89PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x880x88PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x870x87PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x860x86PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x850x85PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x840x84PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x830x83PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x820x82PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x810x81PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x800x800x80PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7F0x7FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7E0x7EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7D0x7DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7C0x7CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7B0x7BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x7A0x7APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x790x79PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x780x78PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x770x77PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x760x76PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x750x75PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x740x74PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x730x73PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x720x72PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x710x71PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x700x700x70PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6F0x6FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6E0x6EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6D0x6DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6C0x6CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6B0x6BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x6A0x6APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x690x69PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x680x68PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x670x67PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x660x66PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x650x65PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x640x64PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x630x63PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x620x62PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x610x61PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x600x600x60PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5F0x5FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5E0x5EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5D0x5DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5C0x5CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5B0x5BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x5A0x5APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x590x59PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x580x58PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x570x57PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x560x56PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x550x55PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x540x54PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x530x53PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x520x52PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x510x51PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x500x500x50PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4F0x4FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4E0x4EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4D0x4DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4C0x4CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4B0x4BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x4A0x4APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x490x49PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x480x48PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x470x47PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x460x46PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x450x45PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x440x44PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x430x43PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x420x42PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x410x41PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x400x400x40PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3F0x3FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3E0x3EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3D0x3DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3C0x3CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3B0x3BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x3A0x3APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x390x39PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x380x38PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x370x37PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x360x36PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x350x35PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x340x34PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x330x33PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x320x32PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x310x31PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x300x300x30PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2F0x2FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2E0x2EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2D0x2DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2C0x2CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2B0x2BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x2A0x2APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x290x29PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x280x28PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x270x27PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x260x26PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x250x25PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x240x24PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x230x23PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x220x22PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x210x21PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x200x200x20PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1F0x1FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1E0x1EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1D0x1DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1C0x1CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1B0x1BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x1A0x1APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x190x19PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x180x18PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x170x17PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x160x16PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x150x15PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x140x14PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x130x13PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x120x12PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x110x11PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x100x100x10PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0F0x0FPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0E0x0EPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0D0x0DPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0C0x0CPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0B0x0BPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x0A0x0APB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x090x09PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x080x08PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x070x07PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x060x06PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x050x05PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x040x04PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x030x03PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x020x02PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x010x01PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevil0x000x000x00PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x152OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x150OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x14COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x14BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x14AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x148OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x147OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x146OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x145OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x144OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x143OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x142OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x141OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x12FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x12BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x129OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x126OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x125OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x124OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x123OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x122OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x121OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x120OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x11BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x11AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x119OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x118OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x117OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x116OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x114OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x110OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x108OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x103OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x102OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x101OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x100OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xFFOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCFOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCEOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCDOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCCOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCBOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xCAOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC9OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC8OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC7OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC6OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC5OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC4OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC3OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC2OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC1OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xC0OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xB2OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xB1OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xB0OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xAFOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xAEOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xADOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xACOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xABOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xAAOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xA9OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xA8OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xA7OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOnDammyPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xA1OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0xA0OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x9DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x9COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x9BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x9AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x99OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x98OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x97OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x96OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x95OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x94OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x93OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x92OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x91OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x90OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x8AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x89OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x88OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x87OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x86OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x85OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x84OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x83OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x82OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x81OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x80OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x7AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x79OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x78OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x77OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x76OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x75OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x74OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x73OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x72OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x71OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x70OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x6AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x69OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x68OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x67OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x66OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x65OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x64OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x63OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x62OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x61OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x60OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x5AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x59OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x58OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x57OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x56OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x55OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x54OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x53OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x52OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x51OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x50OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x4AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x49OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x48OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x47OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x46OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x45OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x44OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x43OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x42OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x41OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x40OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x3AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x39OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x38OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x37OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x36OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x35OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x34OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x33OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x32OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x31OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x30OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x2AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x29OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x28OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x27OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x26OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x25OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x24OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x23OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x22OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x21OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x20OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x1AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x19OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x18OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x17OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x16OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x15OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x14OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x13OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x12OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x11OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x10OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0FOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0EOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0DOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0COnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0BOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x0AOnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x09OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x08OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x07OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x06OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x05OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x04OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x03OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x02OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x01OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileDevilOn0x00OnPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldJewelLostPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldSpeedLinePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMakkaLostPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMajinHolePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldTamaLow03PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldTamaLow02PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldTamaLow01PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldSPlayerPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldJewel1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMaka2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMaka1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldBlood2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldBlood1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPipe3PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPipe2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPipe1PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldDantePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldWpcAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldWpcMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldWpcTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldTakaraMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldTakaraTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMatTestAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMatTestMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldMatTestTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldHandPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPcAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPcMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileFieldPcTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE632ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634TobiraPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634ManinejiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634MagaPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634HikawaPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634AporoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE632PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE632HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE629GozuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE629ManePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE629PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635TukuePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635SinenDPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635SinenCPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635SinenBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635SinenAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635FieldBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE635FieldAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE605TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE603TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE603PlayerBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE621TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE621PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE621BeamPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE550TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE601YuukoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE662HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE662ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE659KagePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE659HikariPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE659YuukoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE651YuukoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE651PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE657YuukoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE646HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE646ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE604TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE604IsamuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE604PlayerBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE652ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE623ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE652PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE623PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE618PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE618PlanesPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE611PlayerBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE652HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE611DoorPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE611IsamuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE615ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE615HikawaPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE615ChairPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE619ManiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE623HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE614ReikoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE614PlayerPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE607TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE619HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE619PlayerAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE550PlayerPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraIsamuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE602PlayerBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE512IsamuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE512DoorPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE615HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE634TiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE616ForuneAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE616ForuneMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE616ForuneTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE602HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE608IsamuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE608DoorPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Player01AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Player01MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Player01TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Maga01AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Maga01MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Maga01TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Kurikara01AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Kurikara01MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Kurikara01TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505HikawaAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505HikawaMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505HikawaTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Arliman01AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Arliman01MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE505Arliman01TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE506TengAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE506TengMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE506TengTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503001AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503001MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503001TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503000AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503000MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE503U503000TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502002AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502002MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502002TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502001AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502001MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502001TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502000AB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502000MB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE502U502000TB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EPlayerAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EPlayerMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EPlayerTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501ETengAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501ETengMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501ETengTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EInkyuAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EInkyuMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EInkyuTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EHokutoAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EHokutoMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EHokutoTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EDoorAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EDoorMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE501EDoorTB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraP02PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraP01PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventHogePB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraDanteD01PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraAmaraEvePPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraSakahagiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraFutomimiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraNpc52PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraNpc51PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraF015LuiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTest04PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTest03PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTest02PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTest01PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTestChiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeHPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraHutomimiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraLuGoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeGPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeFPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeEPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraThorFldPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeDPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraLuiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraTiaki2PB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeCPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeBPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraManeAPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventExtraChiakiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventTestPlayerPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE550YuukoPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventE550HijiriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileEventTestToolPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscBukabuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscKariahaPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscDoriPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscElfPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscPapiruPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscKoronPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscInkyuPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscGyukiPB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscColorbarAB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscColorbarMB);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", pbFileMiscColorbarTB);

