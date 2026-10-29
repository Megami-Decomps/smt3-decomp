#include "common.h"
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
    u16 map[20][20];
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

typedef struct { u16 kind; s16 x, y; u8 unk06[0x8A]; } pbBlockEntry_t;
typedef struct
{
    s32 value;
    u16 groupID;
    u16 count;
    u8 unk08[0xA];
    s16 cellX, cellY;
    u8 unk16[0x9A];
    pbBlockEntry_t *blockEntries;
    u8 unkB4[4];
    f32 rotation;
    u8 unkBC[8];
    f32 x, y, z;
} pbGroupData_t;
typedef struct { s16 x, y; u16 status; } pbRecordCell_t;
typedef struct { s32 status; s16 x, y; } pbRecordCharacter_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;


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

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern pbProcHandle_t *blockDB;

extern s32 pbPopRecord(pbProcHandle_t *p, pbProcHandle_t **c, pbProcHandle_t *blkDB);

extern void *sdfChipAllocDirect(s32 aSize);

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
    return slot->characters != NULL;
}

INCLUDE_ASM(const s32, "pb/pbRecord", pbPopRecord);

#ifdef NON_MATCHING
s32 pbAddRecord_blk(pbProcHandle_t *panel, pbGroupData_t *group)
{
    pbRecord_t *rec = panel->data.record;
    pbRecordSlot_t *slots;
    pbRecordCell_t *cells;
    pbBlockEntry_t *entry;
    pbMapWork_t *map;
    s32 position;
    u32 i;

    rec->slot[rec->pos].flag[2] = group->count;
    if (rec->slot[rec->pos].blocks != NULL) {
        sdfChipFreeDelayedDirect(rec->slot[rec->pos].blocks);
    }
    cells = sdfChipAllocDirect(group->count * sizeof(*cells));
    position = rec->pos;
    slots = rec->slot;
    slots[position].blocks = cells;
    if (group->count != 0) {
        map = D_0054F5C4;
        entry = group->blockEntries;
        i = 0;
        do {
            cells[i].x = group->cellX + entry->x;
            cells[i].y = group->cellY + entry->y;
            cells[i].status = map->map[cells[i].y][cells[i].x] & 0xE7FF;
            entry++;
        } while (++i < group->count);
    }
    if (slots[position].floors != NULL) {
        sdfChipFreeDelayedDirect(slots[position].floors);
        position = rec->pos;
        slots = rec->slot;
        slots[position].flag[3] = 0;
        slots[position].floors = NULL;
        position = rec->pos;
    }
    return position;
}
#else
INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_blk);
#endif

s32 pbAddRecord_flor(pbProcHandle_t *panel, pbGroupData_t *group)
{
    pbRecord_t *rec = panel->data.record;
    pbRecordCell_t *cells;
    pbBlockEntry_t *entry;
    s32 position;
    u32 i;

    rec->slot[rec->pos].flag[3] = group->count;
    if (rec->slot[rec->pos].floors != NULL) {
        sdfChipFreeDelayedDirect(rec->slot[rec->pos].floors);
    }
    cells = sdfChipAllocDirect(group->count * sizeof(*cells));
    position = rec->pos;
    rec->slot[position].floors = cells;
    i = 0;
    if (group->count != 0) {
        entry = group->blockEntries;
        do {
            cells[i].x = group->cellX + entry->x;
            cells[i].y = group->cellY + entry->y;
            cells[i].status = D_0054F5C4->map[cells[i].y][cells[i].x] & 0x800;
            entry++;
        } while (++i < group->count);
    }
    return position;
}

#ifdef NON_MATCHING
/* 73 of 81 words match; coordinate/slot address setup differs. See docs/nonmatching/pb/pbRecord.md. */
extern void pbGetMyCharcterPos2(pbProcHandle_t *panel, s16 *x, s16 *y);
extern u32 pbMyCharacterDispStat(pbProcHandle_t *panel);

s32 pbAddRecord_char(pbProcHandle_t *panel, s32 currentChar,
                     pbProcHandle_t **characters)
{
    pbRecord_t *rec = panel->data.record;
    s16 x, y;
    u32 i;

    rec->slot[rec->pos].flag[1] = D_0054F5C4->pcharNum;
    if (rec->slot[rec->pos].characters != NULL) {
        sdfChipFreeDelayedDirect(rec->slot[rec->pos].characters);
    }
    rec->slot[rec->pos].characters = sdfChipAllocDirect(
        D_0054F5C4->pcharNum * sizeof(pbRecordCharacter_t));
    rec->slot[rec->pos].flag[0] = currentChar;

    for (i = 0; i < D_0054F5C4->pcharNum; i++) {
        pbGetMyCharcterPos2(*characters, &x, &y);
        ((pbRecordCharacter_t *)rec->slot[rec->pos].characters)[i].x = x;
        ((pbRecordCharacter_t *)rec->slot[rec->pos].characters)[i].y = y;
        ((pbRecordCharacter_t *)rec->slot[rec->pos].characters)[i].status =
            pbMyCharacterDispStat(*characters++);
    }
    return rec->pos;
}
#else
INCLUDE_ASM(const s32, "pb/pbRecord", pbAddRecord_char);
#endif

extern s32 printf(const char *format, ...);

void pbDumpRecordData(pbProcHandle_t *panel)
{
    pbRecord_t *rec = panel->data.record;
    char names[7][32] = {
        "FLOOR", "WALL", "NORMAL BLOCK CORE", "NORMAL BLOCK",
        "ROUND BLOCK CORE", "ROUND BLOCK", "EXIT"
    };
    u32 i;
    u32 j;

    printf("\n(( record data ))\n");
    printf("stack point = %d\n", rec->pos);
    for (i = 0; i < 30; i++) {
        pbRecordCell_t *blocks;
        pbRecordCell_t *floors;

        if (rec->pos == (s32)i) {
            printf(">>>stack %d\n", i);
        } else {
            printf("---stack %d\n", i);
        }
        printf("\t<character = %d>\n", rec->slot[i].flag[1]);
        printf("\tcurrent char = %d\n", rec->slot[i].flag[0]);
        for (j = 0; j < rec->slot[i].flag[1]; j++) {
            pbRecordCharacter_t *characters = (pbRecordCharacter_t *)rec->slot[i].characters;
            printf("\t  (x,y) = (%d, %d)\n", characters[j].x, characters[j].y);
        }
        blocks = (pbRecordCell_t *)rec->slot[i].blocks;
        printf("\t<block = %d>\n", rec->slot[i].flag[2]);
        for (j = 0; j < rec->slot[i].flag[2]; j++) {
            u16 status = blocks[j].status;
            printf("\t  (x,y) = (%d,%d) %s : grpid = %d\n",
                   blocks[j].x, blocks[j].y, names[status >> 13], status & 0x7FF);
        }
        floors = (pbRecordCell_t *)rec->slot[i].floors;
        printf("\t<floor = %d>\n", rec->slot[i].flag[3]);
        for (j = 0; j < rec->slot[i].flag[3]; j++) {
            u16 status = floors[j].status;
            printf("\t  (x,y) = (%d,%d) %s : grpid = %d\n",
                   floors[j].x, floors[j].y, names[status >> 13], status & 0x7FF);
        }
    }
}

extern s8 dds3PadButtonState[];
extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);
extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);
extern void rsetOtOfs(u32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail,
                     s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 width, s32 height,
                        s32 u, s32 v, s32 uw, s32 vh, u32 color,
                        sdfTexHandle_t *texture);

s32 pb_draw_help_Process(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;

    if ((s8)dds3PadButtonState[0x21] < 0 ||
        (s8)dds3PadButtonState[0x23] < 0 ||
        (s8)dds3PadButtonState[0x20] < 0) {
        p2sdPlaySE(0x340006, 0x7F, 0x3F);
        dds3ResetProcessFlag(panel->id, 0x10, 1);
        return -1;
    }

    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                0x80808080, (sdfTexHandle_t *)D_0054F5C4->tex34C);
    return 0;
}


void pb_draw_help_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->helpProc = NULL;
}
