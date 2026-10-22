#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbPanel_t pbPanel_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 dispStat;
    s16 cellX, cellY;
    s32 rot;
    u8 unk0C[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyCharacter_t;

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
    struct pbPanel_t *helpProc;
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

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbBlockObj_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbBlockObj_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbPanel_t
{
    dds3ProcessID_t *process;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyCharacter_t *chara;
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfChipFreeDirect(void *aPtr);

extern s8 pbLoadState;

extern pbMapWork_t *D_0054F5C4;

extern pbPanel_t *record;

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern pbPanel_t *blockDB;

extern char pbMapPathBuf[];

extern char D_00535E40[], D_00535E68[];

extern pbPanel_t *pbWaitLoadMapData(pbPanel_t *parent);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 pb_load_mapdata_Process(dds3ProcessID_t *id);

extern void pb_load_mapdata_Shutdown(dds3ProcessID_t *id);

extern u8 pbMapPathIdx, pbMenuIndex, pbMenuState;

extern void *memset(void *a, s32 b, u32 c);

extern s32 pb_proc_menu_Process(dds3ProcessID_t *id);

extern void pb_proc_menu_Shutdown(dds3ProcessID_t *id);

extern char D_005361B0[];

extern void dds3FadeOut(s32, s32, s32, s32);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void sdfFileCmdReadFile(char *aFileName, void *aBuffer, s32 aLength, void *aCallback, s32 aUserData);

extern s32 pbLoadSystemDataDone(s32 handle, s32 status);

extern s32 pbGetMoveChar(void);

extern void dds3AdminiCallSequence(s32, s32, s32, s32);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern char D_00535E90[];

extern dds3ProcessID_t *pbStartGiveupConfirm(void);

extern void pbPreviousRecord(void);

extern pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern char D_00535E10[], D_00535EC8[];


INCLUDE_ASM(const s32, "pb/pbMenu", pbMakeMapPath);

void func_001D4E08(void) {
}

void func_001D4E10(void) {
}

void exitPB(pbPanel_t *p) {
    dds3ResetProcessFlag(p->process, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        ((pbPanel_t *)pbGetMoveChar())->flags = 0x60000;
    }
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void func_001D4E70(pbPanel_t *panel) {
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    pbStartGiveupConfirm();
    panel->flags = 0x50000;
}


/* NON_MATCHING: 21 of 22 words; retail annuls the trailing lui in a beql delay slot. */
#ifdef NON_MATCHING
void func_001D4EB0(pbPanel_t *panel) {
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        ((pbPanel_t *)pbGetMoveChar())->flags = 0x10000;
    }
    panel->flags = 0x50000;
}
#else
INCLUDE_ASM(const s32, "pb/pbMenu", func_001D4EB0);
#endif


void func_001D4F08(void) {
    pbPreviousRecord();
}

void pbSetDebugOwner(pbPanel_t *panel) {
    pbPanelValue_t *work = panel->data.value;
    pbPanel_t *help = pbStartHelp((pbPanel_t *)pbGetMoveChar(), work->value);
    dds3ProcessID_t *id;
    D_0054F5C4->helpProc = help;
    dds3SetProcessFlag(help->process, 0x10, 1);
    id = dds3SearchProcessName(D_00535E90);
    if (id != NULL) {
        dds3ResetProcessFlag(id, 0x10, 2);
    }
    if (panel != NULL) {
        panel->flags = 0x50000;
    }
}

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E40);

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E68);

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E90);

INCLUDE_ASM(const s32, "pb/pbMenu", pb_proc_menu_Process);

void pb_proc_menu_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    dds3ResetProcessFlag(id, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->menuState = 0;
}

INCLUDE_ASM(const s32, "pb/pbMenu", pb_load_mapdata_Process);

void pb_load_mapdata_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDirect(id->workdata);
}

pbPanel_t *pbStartMenu(pbPanel_t *parent, s32 mode) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->process = dds3StartProcess("pb-proc-menu", 0x3F8, 1, 1, pb_proc_menu_Process,
                                      (dds3ProcessFunc_t)pb_proc_menu_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    panel->data.value->value = mode;
    dds3SetProcessData(panel->process, panel);
    pbMenuState = 0;
    pbMapPathIdx = 0;
    return panel;
}

pbPanel_t *pbWaitLoadMapData(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    panel->process = dds3StartProcess(D_00535EC8, 0x3EE, 1, 1, pb_load_mapdata_Process,
                                      (dds3ProcessFunc_t)pb_load_mapdata_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbLoadSystemData(s32 num) {
    dds3FadeOut(0, 0, 0, 1);
    if (num < 30) {
        sprintf(pbMapPathBuf, D_00535E40, num);
    } else {
        sprintf(pbMapPathBuf, D_00535E68, num);
    }
    sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
    pbLoadState = 0;
    pbWaitLoadMapData(NULL);
}

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535EC8);

