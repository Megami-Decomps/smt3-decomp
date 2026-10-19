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

extern pbPanel_t *record;

extern pbPanel_t *blockDB;

extern void *sdfChipAllocDirect(u32 size);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *dst, s32 c, u32 n);

extern s32 pb_draw_help_Process(dds3ProcessID_t *id);

extern void pb_draw_help_Shutdown(dds3ProcessID_t *id);

extern pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page);

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);


pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->process = dds3StartProcess("pb-draw-help", 0x2AFD, 1, 1, pb_draw_help_Process,
                                      (dds3ProcessFunc_t)pb_draw_help_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->x = 0;
    panel->y = 0;
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(8);
    panel->data.value = work;
    memset(work, 0, 8);
    work->value = page;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

pbPanel_t *func_001DB020(pbPanel_t *parent) {
    return pbStartHelp(parent, 0);
}

INCLUDE_ASM(const s32, "pb/pbHelp", pbResetKeyState);


