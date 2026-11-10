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

const char pbFileHumanPcxxxAB[15] = "human/pcxxx.AB";

const char pbFileHumanPcxxxMB[15] = "human/pcxxx.MB";

const char pbFileHumanPcxxxTB[15] = "human/pcxxx.TB";

const char pbFileHumanPcAB[12] = "human/pc.AB";

const char pbFileHumanPcMB[12] = "human/pc.MB";

const char pbFileHumanPcTB[12] = "human/pc.TB";

const char pbFileDevilBoss0x169PB[20] = "devil/boss/0x169.PB";

const char pbFileDevilBoss0x168PB[20] = "devil/boss/0x168.PB";

const char pbFileDevilBoss0x167PB[20] = "devil/boss/0x167.PB";

const char pbFileDevilBoss0x166PB[20] = "devil/boss/0x166.PB";

const char pbFileDevilBoss0x165PB[20] = "devil/boss/0x165.PB";

const char pbFileDevilBoss0x164PB[20] = "devil/boss/0x164.PB";

const char pbFileDevilBoss0x163PB[20] = "devil/boss/0x163.PB";

const char pbFileDevilBoss0x162PB[20] = "devil/boss/0x162.PB";

const char pbFileDevilBoss0x161PB[20] = "devil/boss/0x161.PB";

const char pbFileDevilBoss0x160PB[20] = "devil/boss/0x160.PB";

const char pbFileDevilBoss0x15FPB[20] = "devil/boss/0x15f.PB";

const char pbFileDevilBoss0x15EPB[20] = "devil/boss/0x15e.PB";

const char pbFileDevilBoss0x15DPB[20] = "devil/boss/0x15d.PB";

const char pbFileDevilBoss0x15CPB[20] = "devil/boss/0x15c.PB";

const char pbFileDevilBoss0x15BPB[20] = "devil/boss/0x15b.PB";

const char pbFileDevilBoss0x15APB[20] = "devil/boss/0x15a.PB";

const char pbFileDevilBoss0x159PB[20] = "devil/boss/0x159.PB";

const char pbFileDevilBoss0x158PB[20] = "devil/boss/0x158.PB";

const char pbFileDevilBoss0x157PB[20] = "devil/boss/0x157.PB";

const char pbFileDevilBoss0x156PB[20] = "devil/boss/0x156.PB";

const char pbFileDevilBoss0x155PB[20] = "devil/boss/0x155.PB";

const char pbFileDevilBoss0x154PB[20] = "devil/boss/0x154.PB";

const char pbFileDevilBoss0x153PB[20] = "devil/boss/0x153.PB";

const char pbFileDevilBoss0x152PB[20] = "devil/boss/0x152.PB";

const char pbFileDevilBoss0x150PB[20] = "devil/boss/0x150.PB";

const char pbFileDevilBoss0x14CPB[20] = "devil/boss/0x14c.PB";

const char pbFileDevilBoss0x14BPB[20] = "devil/boss/0x14b.PB";

const char pbFileDevilBoss0x14APB[20] = "devil/boss/0x14a.PB";

const char pbFileDevilBoss0x148PB[20] = "devil/boss/0x148.PB";

const char pbFileDevilBoss0x147PB[20] = "devil/boss/0x147.PB";

const char pbFileDevilBoss0x146PB[20] = "devil/boss/0x146.PB";

const char pbFileDevilBoss0x145PB[20] = "devil/boss/0x145.PB";

const char pbFileDevilBoss0x144PB[20] = "devil/boss/0x144.PB";

const char pbFileDevilBoss0x143PB[20] = "devil/boss/0x143.PB";

const char pbFileDevilBoss0x142PB[20] = "devil/boss/0x142.PB";

const char pbFileDevilBoss0x141PB[20] = "devil/boss/0x141.PB";

const char pbFileDevilBoss0x12FPB[20] = "devil/boss/0x12f.PB";

const char pbFileDevilBoss0x12BPB[20] = "devil/boss/0x12b.PB";

const char pbFileDevilBoss0x129PB[20] = "devil/boss/0x129.PB";

const char pbFileDevilBoss0x126PB[20] = "devil/boss/0x126.PB";

const char pbFileDevilBoss0x125PB[20] = "devil/boss/0x125.PB";

const char pbFileDevilBoss0x124PB[20] = "devil/boss/0x124.PB";

const char pbFileDevilBoss0x123PB[20] = "devil/boss/0x123.PB";

const char pbFileDevilBoss0x122PB[20] = "devil/boss/0x122.PB";

const char pbFileDevilBoss0x121PB[20] = "devil/boss/0x121.PB";

const char pbFileDevilBoss0x120PB[20] = "devil/boss/0x120.PB";

const char pbFileDevilBoss0x11BPB[20] = "devil/boss/0x11b.PB";

const char pbFileDevilBoss0x11APB[20] = "devil/boss/0x11a.PB";

const char pbFileDevilBoss0x119PB[20] = "devil/boss/0x119.PB";

const char pbFileDevilBoss0x118PB[20] = "devil/boss/0x118.PB";

const char pbFileDevilBoss0x117PB[20] = "devil/boss/0x117.PB";

const char pbFileDevilBoss0x116PB[20] = "devil/boss/0x116.PB";

const char pbFileDevilBoss0x114PB[20] = "devil/boss/0x114.PB";

const char pbFileDevilBoss0x110PB[20] = "devil/boss/0x110.PB";

const char pbFileDevilBoss0x10FPB[20] = "devil/boss/0x10f.PB";

const char pbFileDevilBoss0x10EPB[20] = "devil/boss/0x10e.PB";

const char pbFileDevilBoss0x10DPB[20] = "devil/boss/0x10d.PB";

const char pbFileDevilBoss0x10CPB[20] = "devil/boss/0x10c.PB";

const char pbFileDevilBoss0x10BPB[20] = "devil/boss/0x10b.PB";

const char pbFileDevilBoss0x10APB[20] = "devil/boss/0x10a.PB";

const char pbFileDevilBoss0x108PB[20] = "devil/boss/0x108.PB";

const char pbFileDevilBoss0x103PB[20] = "devil/boss/0x103.PB";

const char pbFileDevilBoss0x102PB[20] = "devil/boss/0x102.PB";

const char pbFileDevilBoss0x101PB[20] = "devil/boss/0x101.PB";

const char pbFileDevilBoss0x100PB[20] = "devil/boss/0x100.PB";

const char pbFileDevil0xF00xFF0xFFPB[24] = "devil/0xf0/0xff/0xff.PB";

const char pbFileDevil0xC00xCF0xCFPB[24] = "devil/0xc0/0xcf/0xcf.PB";

const char pbFileDevil0xC00xCE0xCEPB[24] = "devil/0xc0/0xce/0xce.PB";

const char pbFileDevil0xC00xCD0xCDPB[24] = "devil/0xc0/0xcd/0xcd.PB";

const char pbFileDevil0xC00xCC0xCCPB[24] = "devil/0xc0/0xcc/0xcc.PB";

const char pbFileDevil0xC00xCB0xCBPB[24] = "devil/0xc0/0xcb/0xcb.PB";

const char pbFileDevil0xC00xCA0xCAPB[24] = "devil/0xc0/0xca/0xca.PB";

const char pbFileDevil0xC00xC90xC9PB[24] = "devil/0xc0/0xc9/0xc9.PB";

const char pbFileDevil0xC00xC80xC8PB[24] = "devil/0xc0/0xc8/0xc8.PB";

const char pbFileDevil0xC00xC70xC7PB[24] = "devil/0xc0/0xc7/0xc7.PB";

const char pbFileDevil0xC00xC60xC6PB[24] = "devil/0xc0/0xc6/0xc6.PB";

const char pbFileDevil0xC00xC50xC5PB[24] = "devil/0xc0/0xc5/0xc5.PB";

const char pbFileDevil0xC00xC40xC4PB[24] = "devil/0xc0/0xc4/0xc4.PB";

const char pbFileDevil0xC00xC30xC3PB[24] = "devil/0xc0/0xc3/0xc3.PB";

const char pbFileDevil0xC00xC20xC2PB[24] = "devil/0xc0/0xc2/0xc2.PB";

const char pbFileDevil0xC00xC10xC1PB[24] = "devil/0xc0/0xc1/0xc1.PB";

const char pbFileDevil0xC00xC00xC0PB[24] = "devil/0xc0/0xc0/0xc0.PB";

const char pbFileDevil0xB00xB20xB2PB[24] = "devil/0xb0/0xb2/0xb2.PB";

const char pbFileDevil0xB00xB10xB1PB[24] = "devil/0xb0/0xb1/0xb1.PB";

const char pbFileDevil0xB00xB00xB0PB[24] = "devil/0xb0/0xb0/0xb0.PB";

const char pbFileDevil0xA00xAF0xAFPB[24] = "devil/0xa0/0xaf/0xaf.PB";

const char pbFileDevil0xA00xAE0xAEPB[24] = "devil/0xa0/0xae/0xae.PB";

const char pbFileDevil0xA00xAD0xADPB[24] = "devil/0xa0/0xad/0xad.PB";

const char pbFileDevil0xA00xAC0xACPB[24] = "devil/0xa0/0xac/0xac.PB";

const char pbFileDevil0xA00xAB0xABPB[24] = "devil/0xa0/0xab/0xab.PB";

const char pbFileDevil0xA00xAA0xAAPB[24] = "devil/0xa0/0xaa/0xaa.PB";

const char pbFileDevil0xA00xA90xA9PB[24] = "devil/0xa0/0xa9/0xa9.PB";

const char pbFileDevil0xA00xA80xA8PB[24] = "devil/0xa0/0xa8/0xa8.PB";

const char pbFileDevil0xA00xA70xA7PB[24] = "devil/0xa0/0xa7/0xa7.PB";

const char pbFileDevilDammyPB[15] = "devil/dammy.PB";

const char pbFileDevil0xA00xA10xA1PB[24] = "devil/0xa0/0xa1/0xa1.PB";

const char pbFileDevil0xA00xA00xA0PB[24] = "devil/0xa0/0xa0/0xa0.PB";

const char pbFileDevil0x900x9D0x9DPB[24] = "devil/0x90/0x9d/0x9d.PB";

const char pbFileDevil0x900x9C0x9CPB[24] = "devil/0x90/0x9c/0x9c.PB";

const char pbFileDevil0x900x9B0x9BPB[24] = "devil/0x90/0x9b/0x9b.PB";

const char pbFileDevil0x900x9A0x9APB[24] = "devil/0x90/0x9a/0x9a.PB";

const char pbFileDevil0x900x990x99PB[24] = "devil/0x90/0x99/0x99.PB";

const char pbFileDevil0x900x980x98PB[24] = "devil/0x90/0x98/0x98.PB";

const char pbFileDevil0x900x970x97PB[24] = "devil/0x90/0x97/0x97.PB";

const char pbFileDevil0x900x960x96PB[24] = "devil/0x90/0x96/0x96.PB";

const char pbFileDevil0x900x950x95PB[24] = "devil/0x90/0x95/0x95.PB";

const char pbFileDevil0x900x940x94PB[24] = "devil/0x90/0x94/0x94.PB";

const char pbFileDevil0x900x930x93PB[24] = "devil/0x90/0x93/0x93.PB";

const char pbFileDevil0x900x920x92PB[24] = "devil/0x90/0x92/0x92.PB";

const char pbFileDevil0x900x910x91PB[24] = "devil/0x90/0x91/0x91.PB";

const char pbFileDevil0x900x900x90PB[24] = "devil/0x90/0x90/0x90.PB";

const char pbFileDevil0x800x8F0x8FPB[24] = "devil/0x80/0x8f/0x8f.PB";

const char pbFileDevil0x800x8E0x8EPB[24] = "devil/0x80/0x8e/0x8e.PB";

const char pbFileDevil0x800x8D0x8DPB[24] = "devil/0x80/0x8d/0x8d.PB";

const char pbFileDevil0x800x8C0x8CPB[24] = "devil/0x80/0x8c/0x8c.PB";

const char pbFileDevil0x800x8B0x8BPB[24] = "devil/0x80/0x8b/0x8b.PB";

const char pbFileDevil0x800x8A0x8APB[24] = "devil/0x80/0x8a/0x8a.PB";

const char pbFileDevil0x800x890x89PB[24] = "devil/0x80/0x89/0x89.PB";

const char pbFileDevil0x800x880x88PB[24] = "devil/0x80/0x88/0x88.PB";

const char pbFileDevil0x800x870x87PB[24] = "devil/0x80/0x87/0x87.PB";

const char pbFileDevil0x800x860x86PB[24] = "devil/0x80/0x86/0x86.PB";

const char pbFileDevil0x800x850x85PB[24] = "devil/0x80/0x85/0x85.PB";

const char pbFileDevil0x800x840x84PB[24] = "devil/0x80/0x84/0x84.PB";

const char pbFileDevil0x800x830x83PB[24] = "devil/0x80/0x83/0x83.PB";

const char pbFileDevil0x800x820x82PB[24] = "devil/0x80/0x82/0x82.PB";

const char pbFileDevil0x800x810x81PB[24] = "devil/0x80/0x81/0x81.PB";

const char pbFileDevil0x800x800x80PB[24] = "devil/0x80/0x80/0x80.PB";

const char pbFileDevil0x700x7F0x7FPB[24] = "devil/0x70/0x7f/0x7f.PB";

const char pbFileDevil0x700x7E0x7EPB[24] = "devil/0x70/0x7e/0x7e.PB";

const char pbFileDevil0x700x7D0x7DPB[24] = "devil/0x70/0x7d/0x7d.PB";

const char pbFileDevil0x700x7C0x7CPB[24] = "devil/0x70/0x7c/0x7c.PB";

const char pbFileDevil0x700x7B0x7BPB[24] = "devil/0x70/0x7b/0x7b.PB";

const char pbFileDevil0x700x7A0x7APB[24] = "devil/0x70/0x7a/0x7a.PB";

const char pbFileDevil0x700x790x79PB[24] = "devil/0x70/0x79/0x79.PB";

const char pbFileDevil0x700x780x78PB[24] = "devil/0x70/0x78/0x78.PB";

const char pbFileDevil0x700x770x77PB[24] = "devil/0x70/0x77/0x77.PB";

const char pbFileDevil0x700x760x76PB[24] = "devil/0x70/0x76/0x76.PB";

const char pbFileDevil0x700x750x75PB[24] = "devil/0x70/0x75/0x75.PB";

const char pbFileDevil0x700x740x74PB[24] = "devil/0x70/0x74/0x74.PB";

const char pbFileDevil0x700x730x73PB[24] = "devil/0x70/0x73/0x73.PB";

const char pbFileDevil0x700x720x72PB[24] = "devil/0x70/0x72/0x72.PB";

const char pbFileDevil0x700x710x71PB[24] = "devil/0x70/0x71/0x71.PB";

const char pbFileDevil0x700x700x70PB[24] = "devil/0x70/0x70/0x70.PB";

const char pbFileDevil0x600x6F0x6FPB[24] = "devil/0x60/0x6f/0x6f.PB";

const char pbFileDevil0x600x6E0x6EPB[24] = "devil/0x60/0x6e/0x6e.PB";

const char pbFileDevil0x600x6D0x6DPB[24] = "devil/0x60/0x6d/0x6d.PB";

const char pbFileDevil0x600x6C0x6CPB[24] = "devil/0x60/0x6c/0x6c.PB";

const char pbFileDevil0x600x6B0x6BPB[24] = "devil/0x60/0x6b/0x6b.PB";

const char pbFileDevil0x600x6A0x6APB[24] = "devil/0x60/0x6a/0x6a.PB";

const char pbFileDevil0x600x690x69PB[24] = "devil/0x60/0x69/0x69.PB";

const char pbFileDevil0x600x680x68PB[24] = "devil/0x60/0x68/0x68.PB";

const char pbFileDevil0x600x670x67PB[24] = "devil/0x60/0x67/0x67.PB";

const char pbFileDevil0x600x660x66PB[24] = "devil/0x60/0x66/0x66.PB";

const char pbFileDevil0x600x650x65PB[24] = "devil/0x60/0x65/0x65.PB";

const char pbFileDevil0x600x640x64PB[24] = "devil/0x60/0x64/0x64.PB";

const char pbFileDevil0x600x630x63PB[24] = "devil/0x60/0x63/0x63.PB";

const char pbFileDevil0x600x620x62PB[24] = "devil/0x60/0x62/0x62.PB";

const char pbFileDevil0x600x610x61PB[24] = "devil/0x60/0x61/0x61.PB";

const char pbFileDevil0x600x600x60PB[24] = "devil/0x60/0x60/0x60.PB";

const char pbFileDevil0x500x5F0x5FPB[24] = "devil/0x50/0x5f/0x5f.PB";

const char pbFileDevil0x500x5E0x5EPB[24] = "devil/0x50/0x5e/0x5e.PB";

const char pbFileDevil0x500x5D0x5DPB[24] = "devil/0x50/0x5d/0x5d.PB";

const char pbFileDevil0x500x5C0x5CPB[24] = "devil/0x50/0x5c/0x5c.PB";

const char pbFileDevil0x500x5B0x5BPB[24] = "devil/0x50/0x5b/0x5b.PB";

const char pbFileDevil0x500x5A0x5APB[24] = "devil/0x50/0x5a/0x5a.PB";

const char pbFileDevil0x500x590x59PB[24] = "devil/0x50/0x59/0x59.PB";

const char pbFileDevil0x500x580x58PB[24] = "devil/0x50/0x58/0x58.PB";

const char pbFileDevil0x500x570x57PB[24] = "devil/0x50/0x57/0x57.PB";

const char pbFileDevil0x500x560x56PB[24] = "devil/0x50/0x56/0x56.PB";

const char pbFileDevil0x500x550x55PB[24] = "devil/0x50/0x55/0x55.PB";

const char pbFileDevil0x500x540x54PB[24] = "devil/0x50/0x54/0x54.PB";

const char pbFileDevil0x500x530x53PB[24] = "devil/0x50/0x53/0x53.PB";

const char pbFileDevil0x500x520x52PB[24] = "devil/0x50/0x52/0x52.PB";

const char pbFileDevil0x500x510x51PB[24] = "devil/0x50/0x51/0x51.PB";

const char pbFileDevil0x500x500x50PB[24] = "devil/0x50/0x50/0x50.PB";

const char pbFileDevil0x400x4F0x4FPB[24] = "devil/0x40/0x4f/0x4f.PB";

const char pbFileDevil0x400x4E0x4EPB[24] = "devil/0x40/0x4e/0x4e.PB";

const char pbFileDevil0x400x4D0x4DPB[24] = "devil/0x40/0x4d/0x4d.PB";

const char pbFileDevil0x400x4C0x4CPB[24] = "devil/0x40/0x4c/0x4c.PB";

const char pbFileDevil0x400x4B0x4BPB[24] = "devil/0x40/0x4b/0x4b.PB";

const char pbFileDevil0x400x4A0x4APB[24] = "devil/0x40/0x4a/0x4a.PB";

const char pbFileDevil0x400x490x49PB[24] = "devil/0x40/0x49/0x49.PB";

const char pbFileDevil0x400x480x48PB[24] = "devil/0x40/0x48/0x48.PB";

const char pbFileDevil0x400x470x47PB[24] = "devil/0x40/0x47/0x47.PB";

const char pbFileDevil0x400x460x46PB[24] = "devil/0x40/0x46/0x46.PB";

const char pbFileDevil0x400x450x45PB[24] = "devil/0x40/0x45/0x45.PB";

const char pbFileDevil0x400x440x44PB[24] = "devil/0x40/0x44/0x44.PB";

const char pbFileDevil0x400x430x43PB[24] = "devil/0x40/0x43/0x43.PB";

const char pbFileDevil0x400x420x42PB[24] = "devil/0x40/0x42/0x42.PB";

const char pbFileDevil0x400x410x41PB[24] = "devil/0x40/0x41/0x41.PB";

const char pbFileDevil0x400x400x40PB[24] = "devil/0x40/0x40/0x40.PB";

const char pbFileDevil0x300x3F0x3FPB[24] = "devil/0x30/0x3f/0x3f.PB";

const char pbFileDevil0x300x3E0x3EPB[24] = "devil/0x30/0x3e/0x3e.PB";

const char pbFileDevil0x300x3D0x3DPB[24] = "devil/0x30/0x3d/0x3d.PB";

const char pbFileDevil0x300x3C0x3CPB[24] = "devil/0x30/0x3c/0x3c.PB";

const char pbFileDevil0x300x3B0x3BPB[24] = "devil/0x30/0x3b/0x3b.PB";

const char pbFileDevil0x300x3A0x3APB[24] = "devil/0x30/0x3a/0x3a.PB";

const char pbFileDevil0x300x390x39PB[24] = "devil/0x30/0x39/0x39.PB";

const char pbFileDevil0x300x380x38PB[24] = "devil/0x30/0x38/0x38.PB";

const char pbFileDevil0x300x370x37PB[24] = "devil/0x30/0x37/0x37.PB";

const char pbFileDevil0x300x360x36PB[24] = "devil/0x30/0x36/0x36.PB";

const char pbFileDevil0x300x350x35PB[24] = "devil/0x30/0x35/0x35.PB";

const char pbFileDevil0x300x340x34PB[24] = "devil/0x30/0x34/0x34.PB";

const char pbFileDevil0x300x330x33PB[24] = "devil/0x30/0x33/0x33.PB";

const char pbFileDevil0x300x320x32PB[24] = "devil/0x30/0x32/0x32.PB";

const char pbFileDevil0x300x310x31PB[24] = "devil/0x30/0x31/0x31.PB";

const char pbFileDevil0x300x300x30PB[24] = "devil/0x30/0x30/0x30.PB";

const char pbFileDevil0x200x2F0x2FPB[24] = "devil/0x20/0x2f/0x2f.PB";

const char pbFileDevil0x200x2E0x2EPB[24] = "devil/0x20/0x2e/0x2e.PB";

const char pbFileDevil0x200x2D0x2DPB[24] = "devil/0x20/0x2d/0x2d.PB";

const char pbFileDevil0x200x2C0x2CPB[24] = "devil/0x20/0x2c/0x2c.PB";

const char pbFileDevil0x200x2B0x2BPB[24] = "devil/0x20/0x2b/0x2b.PB";

const char pbFileDevil0x200x2A0x2APB[24] = "devil/0x20/0x2a/0x2a.PB";

const char pbFileDevil0x200x290x29PB[24] = "devil/0x20/0x29/0x29.PB";

const char pbFileDevil0x200x280x28PB[24] = "devil/0x20/0x28/0x28.PB";

const char pbFileDevil0x200x270x27PB[24] = "devil/0x20/0x27/0x27.PB";

const char pbFileDevil0x200x260x26PB[24] = "devil/0x20/0x26/0x26.PB";

const char pbFileDevil0x200x250x25PB[24] = "devil/0x20/0x25/0x25.PB";

const char pbFileDevil0x200x240x24PB[24] = "devil/0x20/0x24/0x24.PB";

const char pbFileDevil0x200x230x23PB[24] = "devil/0x20/0x23/0x23.PB";

const char pbFileDevil0x200x220x22PB[24] = "devil/0x20/0x22/0x22.PB";

const char pbFileDevil0x200x210x21PB[24] = "devil/0x20/0x21/0x21.PB";

const char pbFileDevil0x200x200x20PB[24] = "devil/0x20/0x20/0x20.PB";

const char pbFileDevil0x100x1F0x1FPB[24] = "devil/0x10/0x1f/0x1f.PB";

const char pbFileDevil0x100x1E0x1EPB[24] = "devil/0x10/0x1e/0x1e.PB";

const char pbFileDevil0x100x1D0x1DPB[24] = "devil/0x10/0x1d/0x1d.PB";

const char pbFileDevil0x100x1C0x1CPB[24] = "devil/0x10/0x1c/0x1c.PB";

const char pbFileDevil0x100x1B0x1BPB[24] = "devil/0x10/0x1b/0x1b.PB";

const char pbFileDevil0x100x1A0x1APB[24] = "devil/0x10/0x1a/0x1a.PB";

const char pbFileDevil0x100x190x19PB[24] = "devil/0x10/0x19/0x19.PB";

const char pbFileDevil0x100x180x18PB[24] = "devil/0x10/0x18/0x18.PB";

const char pbFileDevil0x100x170x17PB[24] = "devil/0x10/0x17/0x17.PB";

const char pbFileDevil0x100x160x16PB[24] = "devil/0x10/0x16/0x16.PB";

const char pbFileDevil0x100x150x15PB[24] = "devil/0x10/0x15/0x15.PB";

const char pbFileDevil0x100x140x14PB[24] = "devil/0x10/0x14/0x14.PB";

const char pbFileDevil0x100x130x13PB[24] = "devil/0x10/0x13/0x13.PB";

const char pbFileDevil0x100x120x12PB[24] = "devil/0x10/0x12/0x12.PB";

const char pbFileDevil0x100x110x11PB[24] = "devil/0x10/0x11/0x11.PB";

const char pbFileDevil0x100x100x10PB[24] = "devil/0x10/0x10/0x10.PB";

const char pbFileDevil0x000x0F0x0FPB[24] = "devil/0x00/0x0f/0x0f.PB";

const char pbFileDevil0x000x0E0x0EPB[24] = "devil/0x00/0x0e/0x0e.PB";

const char pbFileDevil0x000x0D0x0DPB[24] = "devil/0x00/0x0d/0x0d.PB";

const char pbFileDevil0x000x0C0x0CPB[24] = "devil/0x00/0x0c/0x0c.PB";

const char pbFileDevil0x000x0B0x0BPB[24] = "devil/0x00/0x0b/0x0b.PB";

const char pbFileDevil0x000x0A0x0APB[24] = "devil/0x00/0x0a/0x0a.PB";

const char pbFileDevil0x000x090x09PB[24] = "devil/0x00/0x09/0x09.PB";

const char pbFileDevil0x000x080x08PB[24] = "devil/0x00/0x08/0x08.PB";

const char pbFileDevil0x000x070x07PB[24] = "devil/0x00/0x07/0x07.PB";

const char pbFileDevil0x000x060x06PB[24] = "devil/0x00/0x06/0x06.PB";

const char pbFileDevil0x000x050x05PB[24] = "devil/0x00/0x05/0x05.PB";

const char pbFileDevil0x000x040x04PB[24] = "devil/0x00/0x04/0x04.PB";

const char pbFileDevil0x000x030x03PB[24] = "devil/0x00/0x03/0x03.PB";

const char pbFileDevil0x000x020x02PB[24] = "devil/0x00/0x02/0x02.PB";

const char pbFileDevil0x000x010x01PB[24] = "devil/0x00/0x01/0x01.PB";

const char pbFileDevil0x000x000x00PB[24] = "devil/0x00/0x00/0x00.PB";

const char pbFileDevilOn0x152OnPB[21] = "devil/on/0x152_on.PB";

const char pbFileDevilOn0x150OnPB[21] = "devil/on/0x150_on.PB";

const char pbFileDevilOn0x14COnPB[21] = "devil/on/0x14c_on.PB";

const char pbFileDevilOn0x14BOnPB[21] = "devil/on/0x14b_on.PB";

const char pbFileDevilOn0x14AOnPB[21] = "devil/on/0x14a_on.PB";

const char pbFileDevilOn0x148OnPB[21] = "devil/on/0x148_on.PB";

const char pbFileDevilOn0x147OnPB[21] = "devil/on/0x147_on.PB";

const char pbFileDevilOn0x146OnPB[21] = "devil/on/0x146_on.PB";

const char pbFileDevilOn0x145OnPB[21] = "devil/on/0x145_on.PB";

const char pbFileDevilOn0x144OnPB[21] = "devil/on/0x144_on.PB";

const char pbFileDevilOn0x143OnPB[21] = "devil/on/0x143_on.PB";

const char pbFileDevilOn0x142OnPB[21] = "devil/on/0x142_on.PB";

const char pbFileDevilOn0x141OnPB[21] = "devil/on/0x141_on.PB";

const char pbFileDevilOn0x12FOnPB[21] = "devil/on/0x12f_on.PB";

const char pbFileDevilOn0x12BOnPB[21] = "devil/on/0x12b_on.PB";

const char pbFileDevilOn0x129OnPB[21] = "devil/on/0x129_on.PB";

const char pbFileDevilOn0x126OnPB[21] = "devil/on/0x126_on.PB";

const char pbFileDevilOn0x125OnPB[21] = "devil/on/0x125_on.PB";

const char pbFileDevilOn0x124OnPB[21] = "devil/on/0x124_on.PB";

const char pbFileDevilOn0x123OnPB[21] = "devil/on/0x123_on.PB";

const char pbFileDevilOn0x122OnPB[21] = "devil/on/0x122_on.PB";

const char pbFileDevilOn0x121OnPB[21] = "devil/on/0x121_on.PB";

const char pbFileDevilOn0x120OnPB[21] = "devil/on/0x120_on.PB";

const char pbFileDevilOn0x11BOnPB[21] = "devil/on/0x11b_on.PB";

const char pbFileDevilOn0x11AOnPB[21] = "devil/on/0x11a_on.PB";

const char pbFileDevilOn0x119OnPB[21] = "devil/on/0x119_on.PB";

const char pbFileDevilOn0x118OnPB[21] = "devil/on/0x118_on.PB";

const char pbFileDevilOn0x117OnPB[21] = "devil/on/0x117_on.PB";

const char pbFileDevilOn0x116OnPB[21] = "devil/on/0x116_on.PB";

const char pbFileDevilOn0x114OnPB[21] = "devil/on/0x114_on.PB";

const char pbFileDevilOn0x110OnPB[21] = "devil/on/0x110_on.PB";

const char pbFileDevilOn0x10FOnPB[21] = "devil/on/0x10f_on.PB";

const char pbFileDevilOn0x10EOnPB[21] = "devil/on/0x10e_on.PB";

const char pbFileDevilOn0x10DOnPB[21] = "devil/on/0x10d_on.PB";

const char pbFileDevilOn0x10COnPB[21] = "devil/on/0x10c_on.PB";

const char pbFileDevilOn0x10BOnPB[21] = "devil/on/0x10b_on.PB";

const char pbFileDevilOn0x10AOnPB[21] = "devil/on/0x10a_on.PB";

const char pbFileDevilOn0x108OnPB[21] = "devil/on/0x108_on.PB";

const char pbFileDevilOn0x103OnPB[21] = "devil/on/0x103_on.PB";

const char pbFileDevilOn0x102OnPB[21] = "devil/on/0x102_on.PB";

const char pbFileDevilOn0x101OnPB[21] = "devil/on/0x101_on.PB";

const char pbFileDevilOn0x100OnPB[21] = "devil/on/0x100_on.PB";

const char pbFileDevilOn0xFFOnPB[20] = "devil/on/0xff_on.PB";

const char pbFileDevilOn0xCFOnPB[20] = "devil/on/0xcf_on.PB";

const char pbFileDevilOn0xCEOnPB[20] = "devil/on/0xce_on.PB";

const char pbFileDevilOn0xCDOnPB[20] = "devil/on/0xcd_on.PB";

const char pbFileDevilOn0xCCOnPB[20] = "devil/on/0xcc_on.PB";

const char pbFileDevilOn0xCBOnPB[20] = "devil/on/0xcb_on.PB";

const char pbFileDevilOn0xCAOnPB[20] = "devil/on/0xca_on.PB";

const char pbFileDevilOn0xC9OnPB[20] = "devil/on/0xc9_on.PB";

const char pbFileDevilOn0xC8OnPB[20] = "devil/on/0xc8_on.PB";

const char pbFileDevilOn0xC7OnPB[20] = "devil/on/0xc7_on.PB";

const char pbFileDevilOn0xC6OnPB[20] = "devil/on/0xc6_on.PB";

const char pbFileDevilOn0xC5OnPB[20] = "devil/on/0xc5_on.PB";

const char pbFileDevilOn0xC4OnPB[20] = "devil/on/0xc4_on.PB";

const char pbFileDevilOn0xC3OnPB[20] = "devil/on/0xc3_on.PB";

const char pbFileDevilOn0xC2OnPB[20] = "devil/on/0xc2_on.PB";

const char pbFileDevilOn0xC1OnPB[20] = "devil/on/0xc1_on.PB";

const char pbFileDevilOn0xC0OnPB[20] = "devil/on/0xc0_on.PB";

const char pbFileDevilOn0xB2OnPB[20] = "devil/on/0xb2_on.PB";

const char pbFileDevilOn0xB1OnPB[20] = "devil/on/0xb1_on.PB";

const char pbFileDevilOn0xB0OnPB[20] = "devil/on/0xb0_on.PB";

const char pbFileDevilOn0xAFOnPB[20] = "devil/on/0xaf_on.PB";

const char pbFileDevilOn0xAEOnPB[20] = "devil/on/0xae_on.PB";

const char pbFileDevilOn0xADOnPB[20] = "devil/on/0xad_on.PB";

const char pbFileDevilOn0xACOnPB[20] = "devil/on/0xac_on.PB";

const char pbFileDevilOn0xABOnPB[20] = "devil/on/0xab_on.PB";

const char pbFileDevilOn0xAAOnPB[20] = "devil/on/0xaa_on.PB";

const char pbFileDevilOn0xA9OnPB[20] = "devil/on/0xa9_on.PB";

const char pbFileDevilOn0xA8OnPB[20] = "devil/on/0xa8_on.PB";

const char pbFileDevilOn0xA7OnPB[20] = "devil/on/0xa7_on.PB";

const char pbFileDevilOnDammyPB[18] = "devil/on/dammy.PB";

const char pbFileDevilOn0xA1OnPB[20] = "devil/on/0xa1_on.PB";

const char pbFileDevilOn0xA0OnPB[20] = "devil/on/0xa0_on.PB";

const char pbFileDevilOn0x9DOnPB[20] = "devil/on/0x9d_on.PB";

const char pbFileDevilOn0x9COnPB[20] = "devil/on/0x9c_on.PB";

const char pbFileDevilOn0x9BOnPB[20] = "devil/on/0x9b_on.PB";

const char pbFileDevilOn0x9AOnPB[20] = "devil/on/0x9a_on.PB";

const char pbFileDevilOn0x99OnPB[20] = "devil/on/0x99_on.PB";

const char pbFileDevilOn0x98OnPB[20] = "devil/on/0x98_on.PB";

const char pbFileDevilOn0x97OnPB[20] = "devil/on/0x97_on.PB";

const char pbFileDevilOn0x96OnPB[20] = "devil/on/0x96_on.PB";

const char pbFileDevilOn0x95OnPB[20] = "devil/on/0x95_on.PB";

const char pbFileDevilOn0x94OnPB[20] = "devil/on/0x94_on.PB";

const char pbFileDevilOn0x93OnPB[20] = "devil/on/0x93_on.PB";

const char pbFileDevilOn0x92OnPB[20] = "devil/on/0x92_on.PB";

const char pbFileDevilOn0x91OnPB[20] = "devil/on/0x91_on.PB";

const char pbFileDevilOn0x90OnPB[20] = "devil/on/0x90_on.PB";

const char pbFileDevilOn0x8FOnPB[20] = "devil/on/0x8f_on.PB";

const char pbFileDevilOn0x8EOnPB[20] = "devil/on/0x8e_on.PB";

const char pbFileDevilOn0x8DOnPB[20] = "devil/on/0x8d_on.PB";

const char pbFileDevilOn0x8COnPB[20] = "devil/on/0x8c_on.PB";

const char pbFileDevilOn0x8BOnPB[20] = "devil/on/0x8b_on.PB";

const char pbFileDevilOn0x8AOnPB[20] = "devil/on/0x8a_on.PB";

const char pbFileDevilOn0x89OnPB[20] = "devil/on/0x89_on.PB";

const char pbFileDevilOn0x88OnPB[20] = "devil/on/0x88_on.PB";

const char pbFileDevilOn0x87OnPB[20] = "devil/on/0x87_on.PB";

const char pbFileDevilOn0x86OnPB[20] = "devil/on/0x86_on.PB";

const char pbFileDevilOn0x85OnPB[20] = "devil/on/0x85_on.PB";

const char pbFileDevilOn0x84OnPB[20] = "devil/on/0x84_on.PB";

const char pbFileDevilOn0x83OnPB[20] = "devil/on/0x83_on.PB";

const char pbFileDevilOn0x82OnPB[20] = "devil/on/0x82_on.PB";

const char pbFileDevilOn0x81OnPB[20] = "devil/on/0x81_on.PB";

const char pbFileDevilOn0x80OnPB[20] = "devil/on/0x80_on.PB";

const char pbFileDevilOn0x7FOnPB[20] = "devil/on/0x7f_on.PB";

const char pbFileDevilOn0x7EOnPB[20] = "devil/on/0x7e_on.PB";

const char pbFileDevilOn0x7DOnPB[20] = "devil/on/0x7d_on.PB";

const char pbFileDevilOn0x7COnPB[20] = "devil/on/0x7c_on.PB";

const char pbFileDevilOn0x7BOnPB[20] = "devil/on/0x7b_on.PB";

const char pbFileDevilOn0x7AOnPB[20] = "devil/on/0x7a_on.PB";

const char pbFileDevilOn0x79OnPB[20] = "devil/on/0x79_on.PB";

const char pbFileDevilOn0x78OnPB[20] = "devil/on/0x78_on.PB";

const char pbFileDevilOn0x77OnPB[20] = "devil/on/0x77_on.PB";

const char pbFileDevilOn0x76OnPB[20] = "devil/on/0x76_on.PB";

const char pbFileDevilOn0x75OnPB[20] = "devil/on/0x75_on.PB";

const char pbFileDevilOn0x74OnPB[20] = "devil/on/0x74_on.PB";

const char pbFileDevilOn0x73OnPB[20] = "devil/on/0x73_on.PB";

const char pbFileDevilOn0x72OnPB[20] = "devil/on/0x72_on.PB";

const char pbFileDevilOn0x71OnPB[20] = "devil/on/0x71_on.PB";

const char pbFileDevilOn0x70OnPB[20] = "devil/on/0x70_on.PB";

const char pbFileDevilOn0x6FOnPB[20] = "devil/on/0x6f_on.PB";

const char pbFileDevilOn0x6EOnPB[20] = "devil/on/0x6e_on.PB";

const char pbFileDevilOn0x6DOnPB[20] = "devil/on/0x6d_on.PB";

const char pbFileDevilOn0x6COnPB[20] = "devil/on/0x6c_on.PB";

const char pbFileDevilOn0x6BOnPB[20] = "devil/on/0x6b_on.PB";

const char pbFileDevilOn0x6AOnPB[20] = "devil/on/0x6a_on.PB";

const char pbFileDevilOn0x69OnPB[20] = "devil/on/0x69_on.PB";

const char pbFileDevilOn0x68OnPB[20] = "devil/on/0x68_on.PB";

const char pbFileDevilOn0x67OnPB[20] = "devil/on/0x67_on.PB";

const char pbFileDevilOn0x66OnPB[20] = "devil/on/0x66_on.PB";

const char pbFileDevilOn0x65OnPB[20] = "devil/on/0x65_on.PB";

const char pbFileDevilOn0x64OnPB[20] = "devil/on/0x64_on.PB";

const char pbFileDevilOn0x63OnPB[20] = "devil/on/0x63_on.PB";

const char pbFileDevilOn0x62OnPB[20] = "devil/on/0x62_on.PB";

const char pbFileDevilOn0x61OnPB[20] = "devil/on/0x61_on.PB";

const char pbFileDevilOn0x60OnPB[20] = "devil/on/0x60_on.PB";

const char pbFileDevilOn0x5FOnPB[20] = "devil/on/0x5f_on.PB";

const char pbFileDevilOn0x5EOnPB[20] = "devil/on/0x5e_on.PB";

const char pbFileDevilOn0x5DOnPB[20] = "devil/on/0x5d_on.PB";

const char pbFileDevilOn0x5COnPB[20] = "devil/on/0x5c_on.PB";

const char pbFileDevilOn0x5BOnPB[20] = "devil/on/0x5b_on.PB";

const char pbFileDevilOn0x5AOnPB[20] = "devil/on/0x5a_on.PB";

const char pbFileDevilOn0x59OnPB[20] = "devil/on/0x59_on.PB";

const char pbFileDevilOn0x58OnPB[20] = "devil/on/0x58_on.PB";

const char pbFileDevilOn0x57OnPB[20] = "devil/on/0x57_on.PB";

const char pbFileDevilOn0x56OnPB[20] = "devil/on/0x56_on.PB";

const char pbFileDevilOn0x55OnPB[20] = "devil/on/0x55_on.PB";

const char pbFileDevilOn0x54OnPB[20] = "devil/on/0x54_on.PB";

const char pbFileDevilOn0x53OnPB[20] = "devil/on/0x53_on.PB";

const char pbFileDevilOn0x52OnPB[20] = "devil/on/0x52_on.PB";

const char pbFileDevilOn0x51OnPB[20] = "devil/on/0x51_on.PB";

const char pbFileDevilOn0x50OnPB[20] = "devil/on/0x50_on.PB";

const char pbFileDevilOn0x4FOnPB[20] = "devil/on/0x4f_on.PB";

const char pbFileDevilOn0x4EOnPB[20] = "devil/on/0x4e_on.PB";

const char pbFileDevilOn0x4DOnPB[20] = "devil/on/0x4d_on.PB";

const char pbFileDevilOn0x4COnPB[20] = "devil/on/0x4c_on.PB";

const char pbFileDevilOn0x4BOnPB[20] = "devil/on/0x4b_on.PB";

const char pbFileDevilOn0x4AOnPB[20] = "devil/on/0x4a_on.PB";

const char pbFileDevilOn0x49OnPB[20] = "devil/on/0x49_on.PB";

const char pbFileDevilOn0x48OnPB[20] = "devil/on/0x48_on.PB";

const char pbFileDevilOn0x47OnPB[20] = "devil/on/0x47_on.PB";

const char pbFileDevilOn0x46OnPB[20] = "devil/on/0x46_on.PB";

const char pbFileDevilOn0x45OnPB[20] = "devil/on/0x45_on.PB";

const char pbFileDevilOn0x44OnPB[20] = "devil/on/0x44_on.PB";

const char pbFileDevilOn0x43OnPB[20] = "devil/on/0x43_on.PB";

const char pbFileDevilOn0x42OnPB[20] = "devil/on/0x42_on.PB";

const char pbFileDevilOn0x41OnPB[20] = "devil/on/0x41_on.PB";

const char pbFileDevilOn0x40OnPB[20] = "devil/on/0x40_on.PB";

const char pbFileDevilOn0x3FOnPB[20] = "devil/on/0x3f_on.PB";

const char pbFileDevilOn0x3EOnPB[20] = "devil/on/0x3e_on.PB";

const char pbFileDevilOn0x3DOnPB[20] = "devil/on/0x3d_on.PB";

const char pbFileDevilOn0x3COnPB[20] = "devil/on/0x3c_on.PB";

const char pbFileDevilOn0x3BOnPB[20] = "devil/on/0x3b_on.PB";

const char pbFileDevilOn0x3AOnPB[20] = "devil/on/0x3a_on.PB";

const char pbFileDevilOn0x39OnPB[20] = "devil/on/0x39_on.PB";

const char pbFileDevilOn0x38OnPB[20] = "devil/on/0x38_on.PB";

const char pbFileDevilOn0x37OnPB[20] = "devil/on/0x37_on.PB";

const char pbFileDevilOn0x36OnPB[20] = "devil/on/0x36_on.PB";

const char pbFileDevilOn0x35OnPB[20] = "devil/on/0x35_on.PB";

const char pbFileDevilOn0x34OnPB[20] = "devil/on/0x34_on.PB";

const char pbFileDevilOn0x33OnPB[20] = "devil/on/0x33_on.PB";

const char pbFileDevilOn0x32OnPB[20] = "devil/on/0x32_on.PB";

const char pbFileDevilOn0x31OnPB[20] = "devil/on/0x31_on.PB";

const char pbFileDevilOn0x30OnPB[20] = "devil/on/0x30_on.PB";

const char pbFileDevilOn0x2FOnPB[20] = "devil/on/0x2f_on.PB";

const char pbFileDevilOn0x2EOnPB[20] = "devil/on/0x2e_on.PB";

const char pbFileDevilOn0x2DOnPB[20] = "devil/on/0x2d_on.PB";

const char pbFileDevilOn0x2COnPB[20] = "devil/on/0x2c_on.PB";

const char pbFileDevilOn0x2BOnPB[20] = "devil/on/0x2b_on.PB";

const char pbFileDevilOn0x2AOnPB[20] = "devil/on/0x2a_on.PB";

const char pbFileDevilOn0x29OnPB[20] = "devil/on/0x29_on.PB";

const char pbFileDevilOn0x28OnPB[20] = "devil/on/0x28_on.PB";

const char pbFileDevilOn0x27OnPB[20] = "devil/on/0x27_on.PB";

const char pbFileDevilOn0x26OnPB[20] = "devil/on/0x26_on.PB";

const char pbFileDevilOn0x25OnPB[20] = "devil/on/0x25_on.PB";

const char pbFileDevilOn0x24OnPB[20] = "devil/on/0x24_on.PB";

const char pbFileDevilOn0x23OnPB[20] = "devil/on/0x23_on.PB";

const char pbFileDevilOn0x22OnPB[20] = "devil/on/0x22_on.PB";

const char pbFileDevilOn0x21OnPB[20] = "devil/on/0x21_on.PB";

const char pbFileDevilOn0x20OnPB[20] = "devil/on/0x20_on.PB";

const char pbFileDevilOn0x1FOnPB[20] = "devil/on/0x1f_on.PB";

const char pbFileDevilOn0x1EOnPB[20] = "devil/on/0x1e_on.PB";

const char pbFileDevilOn0x1DOnPB[20] = "devil/on/0x1d_on.PB";

const char pbFileDevilOn0x1COnPB[20] = "devil/on/0x1c_on.PB";

const char pbFileDevilOn0x1BOnPB[20] = "devil/on/0x1b_on.PB";

const char pbFileDevilOn0x1AOnPB[20] = "devil/on/0x1a_on.PB";

const char pbFileDevilOn0x19OnPB[20] = "devil/on/0x19_on.PB";

const char pbFileDevilOn0x18OnPB[20] = "devil/on/0x18_on.PB";

const char pbFileDevilOn0x17OnPB[20] = "devil/on/0x17_on.PB";

const char pbFileDevilOn0x16OnPB[20] = "devil/on/0x16_on.PB";

const char pbFileDevilOn0x15OnPB[20] = "devil/on/0x15_on.PB";

const char pbFileDevilOn0x14OnPB[20] = "devil/on/0x14_on.PB";

const char pbFileDevilOn0x13OnPB[20] = "devil/on/0x13_on.PB";

const char pbFileDevilOn0x12OnPB[20] = "devil/on/0x12_on.PB";

const char pbFileDevilOn0x11OnPB[20] = "devil/on/0x11_on.PB";

const char pbFileDevilOn0x10OnPB[20] = "devil/on/0x10_on.PB";

const char pbFileDevilOn0x0FOnPB[20] = "devil/on/0x0f_on.PB";

const char pbFileDevilOn0x0EOnPB[20] = "devil/on/0x0e_on.PB";

const char pbFileDevilOn0x0DOnPB[20] = "devil/on/0x0d_on.PB";

const char pbFileDevilOn0x0COnPB[20] = "devil/on/0x0c_on.PB";

const char pbFileDevilOn0x0BOnPB[20] = "devil/on/0x0b_on.PB";

const char pbFileDevilOn0x0AOnPB[20] = "devil/on/0x0a_on.PB";

const char pbFileDevilOn0x09OnPB[20] = "devil/on/0x09_on.PB";

const char pbFileDevilOn0x08OnPB[20] = "devil/on/0x08_on.PB";

const char pbFileDevilOn0x07OnPB[20] = "devil/on/0x07_on.PB";

const char pbFileDevilOn0x06OnPB[20] = "devil/on/0x06_on.PB";

const char pbFileDevilOn0x05OnPB[20] = "devil/on/0x05_on.PB";

const char pbFileDevilOn0x04OnPB[20] = "devil/on/0x04_on.PB";

const char pbFileDevilOn0x03OnPB[20] = "devil/on/0x03_on.PB";

const char pbFileDevilOn0x02OnPB[20] = "devil/on/0x02_on.PB";

const char pbFileDevilOn0x01OnPB[20] = "devil/on/0x01_on.PB";

const char pbFileDevilOn0x00OnPB[20] = "devil/on/0x00_on.PB";

const char pbFileFieldJewelLostPB[20] = "field/jewel_lost.PB";

const char pbFileFieldSpeedLinePB[20] = "field/speed_line.PB";

const char pbFileFieldMakkaLostPB[20] = "field/makka_lost.PB";

const char pbFileFieldMajinHolePB[20] = "field/majin_hole.PB";

const char pbFileFieldTamaLow03PB[20] = "field/tama_low03.PB";

const char pbFileFieldTamaLow02PB[20] = "field/tama_low02.PB";

const char pbFileFieldTamaLow01PB[20] = "field/tama_low01.PB";

const char pbFileFieldSPlayerPB[18] = "field/s_player.PB";

const char pbFileFieldJewel1PB[16] = "field/jewel1.PB";

const char pbFileFieldMaka2PB[15] = "field/maka2.PB";

const char pbFileFieldMaka1PB[15] = "field/maka1.PB";

const char pbFileFieldBlood2PB[16] = "field/blood2.PB";

const char pbFileFieldBlood1PB[16] = "field/blood1.PB";

const char pbFileFieldPipe3PB[15] = "field/pipe3.PB";

const char pbFileFieldPipe2PB[15] = "field/pipe2.PB";

const char pbFileFieldPipe1PB[15] = "field/pipe1.PB";

const char pbFileFieldDantePB[15] = "field/dante.PB";

const char pbFileFieldWpcAB[13] = "field/wpc.AB";

const char pbFileFieldWpcMB[13] = "field/wpc.MB";

const char pbFileFieldWpcTB[13] = "field/wpc.TB";

const char pbFileFieldTakaraMB[16] = "field/takara.MB";

const char pbFileFieldTakaraTB[16] = "field/takara.TB";

const char pbFileFieldMatTestAB[18] = "field/mat_test.AB";

const char pbFileFieldMatTestMB[18] = "field/mat_test.MB";

const char pbFileFieldMatTestTB[18] = "field/mat_test.TB";

const char pbFileFieldHandPB[14] = "field/hand.PB";

const char pbFileFieldPcAB[12] = "field/pc.AB";

const char pbFileFieldPcMB[12] = "field/pc.MB";

const char pbFileFieldPcTB[12] = "field/pc.TB";

const char pbFileEventE632ManiPB[19] = "event/e632/mani.PB";

const char pbFileEventE634TobiraPB[21] = "event/e634/tobira.PB";

const char pbFileEventE634PlayerAPB[23] = "event/e634/player_a.PB";

const char pbFileEventE634ManinejiPB[23] = "event/e634/manineji.PB";

const char pbFileEventE634MagaPB[19] = "event/e634/maga.PB";

const char pbFileEventE634HikawaPB[21] = "event/e634/hikawa.PB";

const char pbFileEventE634AporoPB[20] = "event/e634/aporo.PB";

const char pbFileEventE632PlayerAPB[23] = "event/e632/player_a.PB";

const char pbFileEventE632HijiriPB[21] = "event/e632/hijiri.PB";

const char pbFileEventE629GozuPB[19] = "event/e629/gozu.PB";

const char pbFileEventE629ManePB[19] = "event/e629/mane.PB";

const char pbFileEventE629PlayerAPB[23] = "event/e629/player_a.PB";

const char pbFileEventE635TukuePB[20] = "event/e635/tukue.PB";

const char pbFileEventE635TiakiPB[20] = "event/e635/tiaki.PB";

const char pbFileEventE635SinenDPB[22] = "event/e635/sinen_d.PB";

const char pbFileEventE635SinenCPB[22] = "event/e635/sinen_c.PB";

const char pbFileEventE635SinenBPB[22] = "event/e635/sinen_b.PB";

const char pbFileEventE635SinenAPB[22] = "event/e635/sinen_a.PB";

const char pbFileEventE635PlayerAPB[23] = "event/e635/player_a.PB";

const char pbFileEventE635FieldBPB[22] = "event/e635/field_b.PB";

const char pbFileEventE635FieldAPB[22] = "event/e635/field_a.PB";

const char pbFileEventE605TiakiPB[20] = "event/e605/tiaki.PB";

const char pbFileEventE603TiakiPB[20] = "event/e603/tiaki.PB";

const char pbFileEventE603PlayerBPB[23] = "event/e603/player_b.PB";

const char pbFileEventE621TiakiPB[20] = "event/e621/tiaki.PB";

const char pbFileEventE621PlayerAPB[23] = "event/e621/player_a.PB";

const char pbFileEventE621BeamPB[19] = "event/e621/beam.PB";

const char pbFileEventE550TiakiPB[20] = "event/e550/tiaki.PB";

const char pbFileEventE601YuukoPB[20] = "event/e601/yuuko.PB";

const char pbFileEventE662HijiriPB[21] = "event/e662/hijiri.PB";

const char pbFileEventE662ManiPB[19] = "event/e662/mani.PB";

const char pbFileEventE659KagePB[19] = "event/e659/kage.PB";

const char pbFileEventE659HikariPB[21] = "event/e659/hikari.PB";

const char pbFileEventE659YuukoPB[20] = "event/e659/yuuko.PB";

const char pbFileEventE651YuukoPB[20] = "event/e651/yuuko.PB";

const char pbFileEventE651PlayerAPB[23] = "event/e651/player_a.PB";

const char pbFileEventE657YuukoPB[20] = "event/e657/yuuko.PB";

const char pbFileEventE646HijiriPB[21] = "event/e646/hijiri.PB";

const char pbFileEventE646ManiPB[19] = "event/e646/mani.PB";

const char pbFileEventE604TiakiPB[20] = "event/e604/tiaki.PB";

const char pbFileEventE604IsamuPB[20] = "event/e604/isamu.PB";

const char pbFileEventE604PlayerBPB[23] = "event/e604/player_b.PB";

const char pbFileEventE652ManiPB[19] = "event/e652/mani.PB";

const char pbFileEventE623ManiPB[19] = "event/e623/mani.PB";

const char pbFileEventE652PlayerAPB[23] = "event/e652/player_a.PB";

const char pbFileEventE623PlayerAPB[23] = "event/e623/player_a.PB";

const char pbFileEventE618PlayerAPB[23] = "event/e618/player_a.PB";

const char pbFileEventE618PlanesPB[21] = "event/e618/planes.PB";

const char pbFileEventE611PlayerBPB[23] = "event/e611/player_b.PB";

const char pbFileEventE652HijiriPB[21] = "event/e652/hijiri.PB";

const char pbFileEventE611DoorPB[19] = "event/e611/door.PB";

const char pbFileEventE611IsamuPB[20] = "event/e611/isamu.PB";

const char pbFileEventE615ManiPB[19] = "event/e615/mani.PB";

const char pbFileEventE615HikawaPB[21] = "event/e615/hikawa.PB";

const char pbFileEventE615ChairPB[20] = "event/e615/chair.PB";

const char pbFileEventE619ManiPB[19] = "event/e619/mani.PB";

const char pbFileEventE623HijiriPB[21] = "event/e623/hijiri.PB";

const char pbFileEventE614ReikoPB[20] = "event/e614/reiko.PB";

const char pbFileEventE614PlayerPB[21] = "event/e614/player.PB";

const char pbFileEventE607TiakiPB[20] = "event/e607/tiaki.PB";

const char pbFileEventE619HijiriPB[21] = "event/e619/hijiri.PB";

const char pbFileEventE619PlayerAPB[23] = "event/e619/player_a.PB";

const char pbFileEventE550PlayerPB[21] = "event/e550/player.PB";

const char pbFileEventExtraIsamuPB[21] = "event/extra/isamu.PB";

const char pbFileEventE602PlayerBPB[23] = "event/e602/player_b.PB";

const char pbFileEventE512IsamuPB[20] = "event/e512/isamu.PB";

const char pbFileEventE512DoorPB[19] = "event/e512/door.PB";

const char pbFileEventE615HijiriPB[21] = "event/e615/hijiri.PB";

const char pbFileEventE634TiakiPB[20] = "event/e634/tiaki.PB";

const char pbFileEventE616ForuneAB[21] = "event/e616/forune.AB";

const char pbFileEventE616ForuneMB[21] = "event/e616/forune.MB";

const char pbFileEventE616ForuneTB[21] = "event/e616/forune.TB";

const char pbFileEventE602HijiriPB[21] = "event/e602/hijiri.PB";

const char pbFileEventE608IsamuPB[20] = "event/e608/isamu.PB";

const char pbFileEventE608DoorPB[19] = "event/e608/door.PB";

const char pbFileEventE505Player01AB[23] = "event/e505/player01.AB";

const char pbFileEventE505Player01MB[23] = "event/e505/player01.MB";

const char pbFileEventE505Player01TB[23] = "event/e505/player01.TB";

const char pbFileEventE505Maga01AB[21] = "event/e505/maga01.AB";

const char pbFileEventE505Maga01MB[21] = "event/e505/maga01.MB";

const char pbFileEventE505Maga01TB[21] = "event/e505/maga01.TB";

const char pbFileEventE505Kurikara01AB[25] = "event/e505/kurikara01.AB";

const char pbFileEventE505Kurikara01MB[25] = "event/e505/kurikara01.MB";

const char pbFileEventE505Kurikara01TB[25] = "event/e505/kurikara01.TB";

const char pbFileEventE505HikawaAB[21] = "event/e505/hikawa.AB";

const char pbFileEventE505HikawaMB[21] = "event/e505/hikawa.MB";

const char pbFileEventE505HikawaTB[21] = "event/e505/hikawa.TB";

const char pbFileEventE505Arliman01AB[24] = "event/e505/arliman01.AB";

const char pbFileEventE505Arliman01MB[24] = "event/e505/arliman01.MB";

const char pbFileEventE505Arliman01TB[24] = "event/e505/arliman01.TB";

const char pbFileEventE506TengAB[19] = "event/e506/teng.AB";

const char pbFileEventE506TengMB[19] = "event/e506/teng.MB";

const char pbFileEventE506TengTB[19] = "event/e506/teng.TB";

const char pbFileEventE503U503001AB[23] = "event/e503/u503_001.AB";

const char pbFileEventE503U503001MB[23] = "event/e503/u503_001.MB";

const char pbFileEventE503U503001TB[23] = "event/e503/u503_001.TB";

const char pbFileEventE503U503000AB[23] = "event/e503/u503_000.AB";

const char pbFileEventE503U503000MB[23] = "event/e503/u503_000.MB";

const char pbFileEventE503U503000TB[23] = "event/e503/u503_000.TB";

const char pbFileEventE502U502002AB[23] = "event/e502/u502_002.AB";

const char pbFileEventE502U502002MB[23] = "event/e502/u502_002.MB";

const char pbFileEventE502U502002TB[23] = "event/e502/u502_002.TB";

const char pbFileEventE502U502001AB[23] = "event/e502/u502_001.AB";

const char pbFileEventE502U502001MB[23] = "event/e502/u502_001.MB";

const char pbFileEventE502U502001TB[23] = "event/e502/u502_001.TB";

const char pbFileEventE502U502000AB[23] = "event/e502/u502_000.AB";

const char pbFileEventE502U502000MB[23] = "event/e502/u502_000.MB";

const char pbFileEventE502U502000TB[23] = "event/e502/u502_000.TB";

const char pbFileEventE501EPlayerAB[23] = "event/e501/e_player.AB";

const char pbFileEventE501EPlayerMB[23] = "event/e501/e_player.MB";

const char pbFileEventE501EPlayerTB[23] = "event/e501/e_player.TB";

const char pbFileEventE501ETengAB[21] = "event/e501/e_teng.AB";

const char pbFileEventE501ETengMB[21] = "event/e501/e_teng.MB";

const char pbFileEventE501ETengTB[21] = "event/e501/e_teng.TB";

const char pbFileEventE501EInkyuAB[22] = "event/e501/e_inkyu.AB";

const char pbFileEventE501EInkyuMB[22] = "event/e501/e_inkyu.MB";

const char pbFileEventE501EInkyuTB[22] = "event/e501/e_inkyu.TB";

const char pbFileEventE501EHokutoAB[23] = "event/e501/e_hokuto.AB";

const char pbFileEventE501EHokutoMB[23] = "event/e501/e_hokuto.MB";

const char pbFileEventE501EHokutoTB[23] = "event/e501/e_hokuto.TB";

const char pbFileEventE501EDoorAB[21] = "event/e501/e_door.AB";

const char pbFileEventE501EDoorMB[21] = "event/e501/e_door.MB";

const char pbFileEventE501EDoorTB[21] = "event/e501/e_door.TB";

const char pbFileEventExtraP02PB[19] = "event/extra/p02.PB";

const char pbFileEventExtraP01PB[19] = "event/extra/p01.PB";

const char pbFileEventHogePB[14] = "event/hoge.PB";

const char pbFileEventExtraDanteD01PB[25] = "event/extra/dante_d01.PB";

const char pbFileEventExtraAmaraEvePPB[27] = "event/extra/amara_eve_p.PB";

const char pbFileEventExtraSakahagiPB[24] = "event/extra/sakahagi.PB";

const char pbFileEventExtraFutomimiPB[24] = "event/extra/futomimi.PB";

const char pbFileEventExtraNpc52PB[21] = "event/extra/npc52.PB";

const char pbFileEventExtraNpc51PB[21] = "event/extra/npc51.PB";

const char pbFileEventExtraF015LuiPB[24] = "event/extra/f015_lui.PB";

const char pbFileEventExtraTest04PB[22] = "event/extra/test04.PB";

const char pbFileEventExtraTest03PB[22] = "event/extra/test03.PB";

const char pbFileEventExtraTest02PB[22] = "event/extra/test02.PB";

const char pbFileEventExtraTest01PB[22] = "event/extra/test01.PB";

const char pbFileEventExtraTestChiakiPB[27] = "event/extra/test_chiaki.PB";

const char pbFileEventExtraManeHPB[22] = "event/extra/mane_H.PB";

const char pbFileEventExtraHutomimiPB[24] = "event/extra/hutomimi.PB";

const char pbFileEventExtraLuGoPB[21] = "event/extra/lu_go.PB";

const char pbFileEventExtraManeGPB[22] = "event/extra/mane_G.PB";

const char pbFileEventExtraManeFPB[22] = "event/extra/mane_F.PB";

const char pbFileEventExtraManeEPB[22] = "event/extra/mane_E.PB";

const char pbFileEventExtraThorFldPB[24] = "event/extra/thor_fld.PB";

const char pbFileEventExtraManeDPB[22] = "event/extra/mane_D.PB";

const char pbFileEventExtraLuiPB[19] = "event/extra/lui.PB";

const char pbFileEventExtraTiaki2PB[22] = "event/extra/tiaki2.PB";

const char pbFileEventExtraManeCPB[22] = "event/extra/mane_C.PB";

const char pbFileEventExtraManeBPB[22] = "event/extra/mane_B.PB";

const char pbFileEventExtraManeAPB[22] = "event/extra/mane_A.PB";

const char pbFileEventExtraChiakiPB[22] = "event/extra/chiaki.PB";

const char pbFileEventTestPlayerPB[21] = "event/test/player.PB";

const char pbFileEventE550YuukoPB[20] = "event/e550/yuuko.PB";

const char pbFileEventE550HijiriPB[21] = "event/e550/hijiri.PB";

const char pbFileEventTestToolPB[19] = "event/test/tool.PB";

const char pbFileMiscBukabuPB[15] = "misc/bukabu.PB";

const char pbFileMiscKariahaPB[16] = "misc/kariaha.PB";

const char pbFileMiscDoriPB[13] = "misc/dori.PB";

const char pbFileMiscElfPB[12] = "misc/elf.PB";

const char pbFileMiscPapiruPB[15] = "misc/papiru.PB";

const char pbFileMiscKoronPB[14] = "misc/koron.PB";

const char pbFileMiscInkyuPB[14] = "misc/inkyu.PB";

const char pbFileMiscGyukiPB[14] = "misc/gyuki.PB";

const char pbFileMiscColorbarAB[17] = "misc/colorbar.AB";

const char pbFileMiscColorbarMB[17] = "misc/colorbar.MB";

const char pbFileMiscColorbarTB[17] = "misc/colorbar.TB";

