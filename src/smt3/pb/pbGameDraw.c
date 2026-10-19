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

extern void sdfChipFreeDelayedDirect(void *ptr);

extern pbMapWork_t *D_0054F5C4;

extern pbPanel_t *record;

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatSetTex(sdfMat_t *mat, void *tex);

extern pbPanel_t *blockDB;

extern void *sdfChipAllocDirect(u32 size);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *dst, s32 c, u32 n);

extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_record_Process(dds3ProcessID_t *id);

extern void pb_proc_record_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern void pbFreeRecord(pbRecordSlot_t *slot);

extern void pbFreeGroupEntry(mdlResrc_t *entry);

extern void pbClearRecord(pbPanel_t *panel);

extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern char D_005361B0[], D_005361C8[], D_005365D8[], D_00536398[], D_00535EB8[];
extern char D_00536040[], D_00535CE8[], D_00535CD8[], D_00535FA8[];


pbPanel_t *pbStartBlockDataBase(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    sdfMat_t *mat;
    panel->process = dds3StartProcess(D_00536040, 0x2AFD, 1, 1, pb_proc_block_DB_Process,
                                      (dds3ProcessFunc_t)pb_proc_block_DB_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0x108);
    panel->data.blockDB = work;
    memset(work, 0, 0x108);
    dds3SetProcessData(panel->process, panel);
    mat = sdfMatNew();
    panel->data.blockDB->mat = mat;
    sdfMatSetTex(mat, D_0054F5C4->baseTex);
    return panel;
}

void *pbSearchGroupDB(pbPanel_t *panel, u32 group) {
    return pbFindGroupEntry(panel->data.groupDB, group);
}

void pbDeleteGroupEntry(pbPanel_t *panel, u32 group) {
    pbGroupDB_t *db = panel->data.groupDB;
    s32 removed = 0;
    u32 i;
    for (i = 0; i < db->count; i++) {
        if (removed != 0) {
            db->entry[i - 1] = db->entry[i];
        }
        if (db->entry[i]->group == group) {
            pbFreeGroupEntry((mdlResrc_t *)db->entry[i]);
            removed = 1;
        }
    }
    db->count = db->count - 1;
}

INCLUDE_RODATA(const s32, "pb/pbGameDraw", D_00536040);

INCLUDE_ASM(const s32, "pb/pbGameDraw", pbDumpBlockData);

INCLUDE_ASM(const s32, "pb/pbGameDraw", pbRebuildOutline);

pbPanel_t *pbStartDrawMapCursor(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->process = dds3StartProcess(D_005361B0, 0x2AFD, 1, 1, pb_draw_map_cursor_Process,
                                      (dds3ProcessFunc_t)pb_draw_map_cursor_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbSetMapCursorFlag(pbPanel_t *panel, s32 flag) {
    pbPanelValue_t *v = panel->data.value;
    if (panel != NULL) {
        v->value = flag;
    }
}

s32 pb_proc_record_Process(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    u32 state = panel->state;
    switch (state) {
    case 4:
    case 5:
    case 6:
        return -1;
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 0:
        break;
    }
    return 0;
}

void pb_proc_record_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRecord_t *rec = panel->data.record;
    pbClearRecord(panel);
    sdfChipFreeDelayedDirect(rec->slot);
    sdfChipFreeDelayedDirect(rec);
    sdfChipFreeDelayedDirect(panel);
}

void pbFreeRecord(pbRecordSlot_t *slot) {
    if (slot->block != NULL) {
        slot->flag[0] = 0;
        slot->flag[1] = 0;
        sdfChipFreeDelayedDirect(slot->block);
        slot->block = NULL;
    }
    if (slot->floor != NULL) {
        slot->flag[2] = 0;
        sdfChipFreeDelayedDirect(slot->floor);
        slot->floor = NULL;
    }
    if (slot->chara != NULL) {
        slot->flag[3] = 0;
        sdfChipFreeDelayedDirect(slot->chara);
        slot->chara = NULL;
    }
}

INCLUDE_RODATA(const s32, "pb/pbGameDraw", D_005361B0);

