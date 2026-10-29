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

typedef struct {
    u16 kind;
    s16 x, y;
    u16 outline_num;
    u8 reserved08[8];
    f32 outline[8][4];
} pbBlockEntry_t;

typedef struct
{
    s32 value;
    u16 grpID;
    u16 num;
    u8 unk08[0xA];
    s16 cellX, cellY;
    u8 unk16[0x9A];
    pbBlockEntry_t *blk;
    u8 unkB4[4];
    f32 rot;
    u8 unkBC[8];
    f32 x, y, z;
} pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupData_t *group[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

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

extern void sdfChipFreeDelayedDirect(void *aPtr);
extern s32 printf(const char *format, ...);

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatSetTex(sdfMat_t *aMat, void *aTex);

extern pbProcHandle_t *blockDB;

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *a, s32 b, u32 c);

extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_record_Process(dds3ProcessID_t *id);

extern void pb_proc_record_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern void pbFreeRecord(pbRecordSlot_t *slot);

extern void pbFreeGroupEntry(dds3ModelResrc_t *entry);

extern void pbClearRecord(pbProcHandle_t *p);

extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);


pbProcHandle_t *pbStartBlockDataBase(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    sdfMat_t *mat;
    panel->id = dds3StartProcess("pb-proc-block DB", 0x2AFD, 1, 1, pb_proc_block_DB_Process,
                                      (dds3ProcessFunc_t)pb_proc_block_DB_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0x108);
    panel->data.blockDB = work;
    memset(work, 0, 0x108);
    dds3SetProcessData(panel->id, panel);
    mat = sdfMatNew();
    panel->data.blockDB->mat = mat;
    sdfMatSetTex(mat, D_0054F5C4->baseTex);
    return panel;
}

void *pbSearchGroupDB(pbProcHandle_t *p, u32 grpID) {
    return pbFindGroupEntry(p->data.groupDB, grpID);
}

void pbDeleteGroupEntry(pbProcHandle_t *panel, u32 group) {
    pbGroupDB_t *db = panel->data.groupDB;
    s32 removed = 0;
    u32 i;
    for (i = 0; i < db->count; i++) {
        if (removed != 0) {
            db->entry[i - 1] = db->entry[i];
        }
        if (db->entry[i]->group == group) {
            pbFreeGroupEntry((dds3ModelResrc_t *)db->entry[i]);
            removed = 1;
        }
    }
    db->count = db->count - 1;
}


void pbDumpBlockData(pbProcHandle_t *p) {
    pbBlockDB_t *db = p->data.blockDB;
    char names[7][32] = {
        "FLOOR", "WALL", "NORMAL BLOCK CORE", "NORMAL BLOCK",
        "ROUND BLOCK CORE", "ROUND BLOCK", "EXIT"
    };
    u32 i = 0, j;

    printf("\n(( block data ))\n");
    for (; i < db->count; i++) {
        printf("ID = %d, (core x,y) = (%d, %d) %dblocks\n",
               db->group[i]->grpID, db->group[i]->cellX,
               db->group[i]->cellY, db->group[i]->num);
        for (j = 0; j < db->group[i]->num; j++) {
            printf("\t(x,y) = (%d, %d) type = %s : outline = %d\n",
                   db->group[i]->blk[j].x, db->group[i]->blk[j].y,
                   names[db->group[i]->blk[j].kind],
                   db->group[i]->blk[j].outline_num >> 1);
        }
    }
}

typedef struct {
    f32 from[4];
    f32 to[4];
} pbOutlineEdge_t;

extern pbOutlineEdge_t D_00327780[];
extern u8 pbGetBlockAt(s16 x, s16 y, s16 group);

/* NON_MATCHING: 96 of 98 words match at 392 native/emitted bytes. Only the
 * two block-address additions commute their operands (+0x4C and +0xDC). */
#ifdef NON_MATCHING
void pbRebuildOutline(pbGroupData_t *group)
{
    s32 i;

    for (i = 0; i < group->num; i++) {
        pbBlockEntry_t *block = &group->blk[i];
        f32 x = (f32)(block->x * 40);
        f32 z = (f32)(block->y * 40);
        s32 bits;
        s32 side;

        block->outline_num = 0;
        bits = pbGetBlockAt(group->cellX + block->x, group->cellY + block->y, group->grpID);
        for (side = 0; side < 4; side++) {
            if (((bits >> side) & 1) != 0) {
                u16 n;

                pbBlockEntry_t *destination = &group->blk[i];
                n = destination->outline_num;
                destination->outline_num += 2;
                destination->outline[n][0] = D_00327780[side].from[0] + x;
                destination->outline[n][1] = D_00327780[side].from[1];
                destination->outline[n][2] = D_00327780[side].from[2] + z;
                destination->outline[n + 1][0] = D_00327780[side].to[0] + x;
                destination->outline[n + 1][1] = D_00327780[side].to[1];
                destination->outline[n + 1][2] = D_00327780[side].to[2] + z;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "pb/pbGameDraw", pbRebuildOutline);
#endif

pbProcHandle_t *pbStartDrawMapCursor(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->id = dds3StartProcess("pb-draw-map cursor", 0x2AFD, 1, 1, pb_draw_map_cursor_Process,
                                      (dds3ProcessFunc_t)pb_draw_map_cursor_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    dds3SetProcessData(panel->id, panel);
    return panel;
}

void pbSetMapCursorFlag(pbProcHandle_t *p, s32 flag) {
    pbPanelValue_t *v = p->data.value;
    if (p != NULL) {
        v->value = flag;
    }
}

s32 pb_proc_record_Process(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
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
    pbProcHandle_t *panel = id->workdata;
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


