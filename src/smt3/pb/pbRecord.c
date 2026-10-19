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

extern pbPanel_t *blockDB;

extern s32 pbPopRecord(pbPanel_t *record, pbPanel_t **chars, pbPanel_t *db);

extern void *sdfChipAllocDirect(u32 size);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *dst, s32 c, u32 n);

extern s32 pb_proc_record_Process(dds3ProcessID_t *id);

extern void pb_proc_record_Shutdown(dds3ProcessID_t *id);

extern s32 pb_draw_help_Process(dds3ProcessID_t *id);

extern void pb_draw_help_Shutdown(dds3ProcessID_t *id);

extern void pbFreeRecord(pbRecordSlot_t *slot);

extern u32 pbPushRecord(pbPanel_t *panel);

extern s8 pbCheckPopRecord(pbPanel_t *panel);

extern void pbClearRecord(pbPanel_t *panel);

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern char D_005361B0[], D_005361C8[], D_005365D8[], D_00536398[], D_00535EB8[];


pbPanel_t *pbStartRecord(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbRecord_t *work;
    panel->process = dds3StartProcess(D_005361C8, 0x3EE, 1, 1, pb_proc_record_Process,
                                      (dds3ProcessFunc_t)pb_proc_record_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(8);
    panel->data.record = work;
    work->pos = 0;
    work->slot = sdfChipAllocDirect(0x1E0);
    memset(work->slot, 0, 0x1E0);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbClearRecord(pbPanel_t *panel) {
    pbRecord_t *rec = panel->data.record;
    u32 i;
    for (i = 0; i < 30; i++) {
        pbFreeRecord(&rec->slot[i]);
    }
}

s32 pb_1DA400(pbPanel_t *panel) {
    return panel->data.value->value;
}

u32 pbPushRecord(pbPanel_t *panel) {
    pbPanelValue_t *rec = panel->data.value;
    return rec->value = (rec->value + 31) % 30;
}

s8 pbCheckPopRecord(pbPanel_t *panel) {
    pbRecord_t *rec = panel->data.record;
    pbRecordSlot_t *slot = &rec->slot[(rec->pos + 29) % 30];
    return slot->block != NULL;
}

INCLUDE_ASM(const s32, "pb/pbRecord", pbPopRecord);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_blk);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_flor);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_char);

INCLUDE_RODATA(const s32, "pb/pbRecord", D_005361C8);

INCLUDE_ASM(const s32, "pb/pbRecord", pbDumpRecordData);

INCLUDE_ASM(const s32, "pb/pbRecord", pb_draw_help_Process);

void pb_draw_help_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->helpProc = NULL;
}
