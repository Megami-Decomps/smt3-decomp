#include "common.h"
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
    s32 dispStat;
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
    s32 unk00;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbMatWork_t;

typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern s32 pbMoveChar;

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern u32 pbMapIsValid(void);

extern s8 pbLoadState;

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern pbProcHandle_t *blockDB;

extern char pbMapPathBuf[];

extern pbProcHandle_t *pbWaitLoadMapData(pbProcHandle_t *parent);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 pb_draw_bg_Process(dds3ProcessID_t *id);

extern void pb_draw_bg_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_dummy_Process(dds3ProcessID_t *id);

extern void pb_proc_dummy_Shutdown(dds3ProcessID_t *id);

extern void dds3FadeOut(s32, s32, s32, s32);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void sdfFileCmdReadFile(char *aFileName, void *aBuffer, s32 aLength, void *aCallback, s32 aUserData);

extern s32 pbLoadSystemDataDone(s32 handle, s32 status);

extern s32 pbGetMoveChar(void);

extern char D_00535DE0[];

extern sdfMemHandle_t *a_key;

extern void p2sdStopAllSe(void);

extern void fclEncycFreeEntries(char *name);

extern void fldProcNextJump(void);

extern s8 pbCheckPreviousRecord(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void sdfTexFreeDelayed(void *aHandle);
extern u8 pbMapPathIdx, pbMenuIndex, pbMenuState;
extern char D_00535E10[], D_00535E20[], D_00535EC8[];


INCLUDE_ASM(const s32, "pb/pbMain", pbEditInitProc);

void pbEditMainExitProc(void) {
    p2sdStopAllSe();
    sdfTexFreeDelayed(D_0054F5C4->baseTex);
    sdfTexFreeDelayed(D_0054F5C4->tex344);
    sdfTexFreeDelayed(D_0054F5C4->tex348);
    sdfTexFreeDelayed(D_0054F5C4->tex34C);
    if (D_0054F5C4->giveupScript != NULL) {
        sdfMemFreeDirect(D_0054F5C4->mapData);
        D_0054F5C4->giveupScript = NULL;
    }
    sdfMemFreeDirect(a_key);
    fclEncycFreeEntries(D_00535DE0);
    fldProcNextJump();
}

s32 pb_proc_dummy_Process(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    if (panel != NULL) {
        panel->flags = 0x20000;
    }
    return -1;
}

void pb_proc_dummy_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "pb/pbMain", pb_draw_bg_Process);

void pb_draw_bg_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDelayedDirect(id->workdata);
}

INCLUDE_RODATA(const s32, "pb/pbMain", D_00535DE0);

INCLUDE_ASM(const s32, "pb/pbMain", pbStartMapEditor);

s32 pbGetMoveChar(void) {
    return pbMoveChar;
}

pbProcHandle_t *pbStartDummy(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->id = dds3StartProcess("pb-proc-dummy", 0x3EE, 1, 1, pb_proc_dummy_Process,
                                      (dds3ProcessFunc_t)pb_proc_dummy_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    work = sdfChipAllocDirect(4);
    work->value = 0;
    panel->data.value = work;
    panel->flags = 0;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

pbProcHandle_t *pbStartBackGround(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    panel->id = dds3StartProcess(D_00535E10, 0x2AFD, 1, 1, pb_draw_bg_Process,
                                      (dds3ProcessFunc_t)pb_draw_bg_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

s32 func_001D4B28(void) {
    return 0;
}

u32 pbMapIsValid(void) {
    u32 ret = (D_0054F5C4->pcharNum == 0);
    if ((D_0054F5C4->exitX < 0) || (D_0054F5C4->exitY < 0)) {
        ret |= 2;
    }
    return ret;
}

s32 pb_1D4B60(void) {
    return 0;
}

void func_001D4B68(void) {
}

s32 pb_1D4B70(void) {
    return 0;
}

void func_001D4B78(void) {
}

s32 pb_1D4B80(void) {
    return 0;
}

s32 pb_1D4B88(void) {
    return 0;
}

void func_001D4B90(void) {
}

s32 pbLoadSystemDataDone(s32 handle, s32 status) {
    if (status == 8) {
        pbLoadState = 1;
    } else if (status == 0) {
        pbLoadState = -1;
    }
    return 0;
}

s32 func_001D4BC0(void) {
    return 1;
}

s32 func_001D4BC8(void) {
    return pbMapIsValid() == 0;
}

s8 func_001D4BE8(void) {
    s8 ret = pbCheckPreviousRecord();
    return ret;
}


/* NON_MATCHING: 1 of 37 words; retail annuls the epilogue restore in a beql delay slot. */
#ifdef NON_MATCHING
void func_001D4C08(pbProcHandle_t *panel) {
    if (pbMapIsValid() == 0) {
        dds3FadeOut(0, 0, 0, 1);
        dds3ResetProcessFlag(panel->id, 0x10, 1);
        if (pbGetMoveChar() != 0) {
            if (pbGetMoveChar() != 0) {
                ((pbProcHandle_t *)pbGetMoveChar())->flags = 0x40000;
            }
            ((pbProcHandle_t *)pbGetMoveChar())->flags |= 1;
        }
    }
}
#else
INCLUDE_ASM(const s32, "pb/pbMain", func_001D4C08);
#endif


void pbMakeMapPathHdd(pbProcHandle_t *parent) {
    if (pbMapPathIdx == 1) {
        dds3FadeOut(0, 0, 0, 1);
        sprintf(pbMapPathBuf, D_00535E20, pbMenuIndex);
        sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
        pbLoadState = 0;
        pbWaitLoadMapData(parent);
    } else {
        pbMapPathIdx = 1;
    }
}

INCLUDE_RODATA(const s32, "pb/pbMain", D_00535E10);

INCLUDE_RODATA(const s32, "pb/pbMain", D_00535E20);

