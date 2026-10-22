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

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern pbProcHandle_t *blockDB;

extern s32 pbPopRecord(pbProcHandle_t *p, pbProcHandle_t **c, pbProcHandle_t *blkDB);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *a, s32 b, u32 c);

extern s32 pb_proc_record_Process(dds3ProcessID_t *id);

extern void pb_proc_record_Shutdown(dds3ProcessID_t *id);

extern s32 pb_draw_help_Process(dds3ProcessID_t *id);

extern void pb_draw_help_Shutdown(dds3ProcessID_t *id);

extern void pbFreeRecord(pbRecordSlot_t *slot);

extern u32 pbPushRecord(pbProcHandle_t *p);

extern s8 pbCheckPopRecord(pbProcHandle_t *p);

extern void pbClearRecord(pbProcHandle_t *p);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);


pbProcHandle_t *pbStartRecord(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbRecord_t *work;
    panel->id = dds3StartProcess("pb-proc-record", 0x3EE, 1, 1, pb_proc_record_Process,
                                      (dds3ProcessFunc_t)pb_proc_record_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(8);
    panel->data.record = work;
    work->pos = 0;
    work->slot = sdfChipAllocDirect(0x1E0);
    memset(work->slot, 0, 0x1E0);
    dds3SetProcessData(panel->id, panel);
    return panel;
}

void pbClearRecord(pbProcHandle_t *p) {
    pbRecord_t *rec = p->data.record;
    u32 i;
    for (i = 0; i < 30; i++) {
        pbFreeRecord(&rec->slot[i]);
    }
}

s32 pb_1DA400(pbProcHandle_t *panel) {
    return panel->data.value->value;
}

u32 pbPushRecord(pbProcHandle_t *p) {
    pbPanelValue_t *rec = p->data.value;
    return rec->value = (rec->value + 31) % 30;
}

s8 pbCheckPopRecord(pbProcHandle_t *p) {
    pbRecord_t *rec = p->data.record;
    pbRecordSlot_t *slot = &rec->slot[(rec->pos + 29) % 30];
    return slot->block != NULL;
}

INCLUDE_ASM(const s32, "pb/pbRecord", pbPopRecord);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_blk);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_flor);

INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_char);


INCLUDE_ASM(const s32, "pb/pbRecord", pbDumpRecordData);

INCLUDE_ASM(const s32, "pb/pbRecord", pb_draw_help_Process);

void pb_draw_help_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->helpProc = NULL;
}
