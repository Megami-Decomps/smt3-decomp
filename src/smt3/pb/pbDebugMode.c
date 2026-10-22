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

typedef struct { s16 x, y; } pbGoalPos_t;

typedef struct { u8 unk00[8]; pbGoalPos_t pos; } pbGoal_t;

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

typedef struct { sdfMemHandle_t *allocation; u16 unk04, unk06, unk08, level, stage; } pbDebugWork_t;

extern u8 D_0054F584;

extern pbDebugWork_t *g_pdebug_0;

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern pbMapWork_t *D_0054F5C4;

extern pbGoal_t *goal;

extern pbPanel_t *record;

extern pbPanel_t *blockDB;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern u8 degree_mode;

void ReleaseDebug_pbDebugMode(void);

extern char D_0054F530[];

extern char D_0054F538[];

extern char D_0054F540[];

extern char D_0054F548[];

extern char D_0054F550[];

extern char D_0054F558[];

extern char D_0054F588[];

extern char D_0054F590[];

extern char D_0054F598[];

extern char D_0054F5A0[];

extern char D_0054F5A8[];

extern s32 D_0054F510;

extern void *D_0054F514;


INCLUDE_ASM(const s32, "pb/pbDebugMode", pbDebugProc);

void pbDebugShutdown(void) {
    ReleaseDebug_pbDebugMode();
}

void InitializeDebug_pbDebugMode(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x10);
    pbDebugWork_t *work = sdfMemLock(handle);
    work->unk04 = 0;
    work->unk06 = 0;
    work->unk08 = 0;
    work->allocation = handle;
    work->level = D_0054F5C4->level;
    work->stage = D_0054F5C4->stage;
    g_pdebug_0 = work;
}

void ReleaseDebug_pbDebugMode(void) {
    if (g_pdebug_0 != NULL) {
        sdfMemFreeDirect(g_pdebug_0->allocation);
        g_pdebug_0 = NULL;
    }
}

INCLUDE_ASM(const s32, "pb/pbDebugMode", DebugProcMenu_pbDebugMode);

INCLUDE_ASM(const s32, "pb/pbDebugMode", DebugProcRadius);

INCLUDE_ASM(const s32, "pb/pbDebugMode", DebugProcDegree);

INCLUDE_ASM(const s32, "pb/pbDebugMode", DebugProcStage);

INCLUDE_ASM(const s32, "pb/pbDebugMode", DebugProcFree);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536690);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005366F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536708);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536720);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536738);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536750);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536768);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536780);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536798);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005367B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005367C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005367E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005367F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536810);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536828);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536840);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536858);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536870);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536888);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005368A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005368B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005368D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005368E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536900);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536918);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536930);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536948);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536960);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536978);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536990);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005369A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005369C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005369D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005369F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536A98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536AB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536AC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536AE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536AF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536B88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536BA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536BB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536BD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536BE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536C90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536CA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536CC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536CD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536CF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536D98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536DB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536DC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536DE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536DF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536E88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536EA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536EB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536ED0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536EE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536F90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536FA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536FC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536FD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00536FE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537000);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537018);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537030);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537048);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537060);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537078);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537090);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005370A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005370C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005370D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005370F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537108);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537120);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537138);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537150);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537168);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537180);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537198);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005371B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005371C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005371E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005371F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537210);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537228);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537240);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537258);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537270);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537288);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005372A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005372B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005372D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005372E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537300);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537318);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537330);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537348);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537360);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537378);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537390);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005373A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005373C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005373D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005373F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537408);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537420);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537438);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537450);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537468);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537480);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537498);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005374B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005374C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005374E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005374F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537510);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537528);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537540);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537558);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537570);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537588);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005375A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005375B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005375D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005375E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537600);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537618);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537630);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537648);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537660);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537678);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537690);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005376A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005376C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005376D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005376F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537708);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537720);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537738);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537750);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537768);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537780);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537798);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005377B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005377C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005377E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005377F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537810);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537828);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537840);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537858);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537870);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537888);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005378A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005378B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005378D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005378E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537900);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537918);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537930);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537948);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537960);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537978);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537990);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005379A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005379C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005379D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005379F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537A98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537AB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537AC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537AE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537AF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537B88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537BA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537BB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537BD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537BE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537C90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537CA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537CC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537CD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537CF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537D98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537DB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537DC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537DE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537DF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537E88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537EA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537EB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537ED0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537EE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537F90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537FA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537FC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537FD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00537FF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538008);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538020);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538038);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538050);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538068);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538080);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538098);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005380B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005380C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005380E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005380F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538110);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538128);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538140);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538158);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538170);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538188);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005381A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005381B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005381D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005381E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538200);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538218);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538230);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538248);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538260);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538278);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538290);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005382A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005382C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005382D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005382F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538308);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538320);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538338);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538350);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538368);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538380);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538398);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005383B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005383C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005383E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005383F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538410);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538428);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538440);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538458);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538470);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538488);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005384A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005384B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005384D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005384E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538500);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538518);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538530);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538548);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538560);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538578);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538590);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005385A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005385C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005385D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005385F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538608);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538620);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538638);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538650);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538668);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538680);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538698);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005386B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005386C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005386E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005386F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538710);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538728);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538740);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538758);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538770);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538788);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005387A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005387B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005387D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005387E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538800);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538818);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538830);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538848);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538860);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538878);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538890);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005388A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005388C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005388D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005388F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538908);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538920);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538938);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538950);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538968);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538980);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538998);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005389B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005389C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005389E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005389F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538A88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538AA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538AB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538AD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538AE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538B90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538BA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538BC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538BD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538BF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538C98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538CB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538CC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538CE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538CF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538D88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538DA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538DB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538DD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538DE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538E90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538EA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538EC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538ED8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538EF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538F98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538FB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538FC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538FE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00538FF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539010);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539028);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539040);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539058);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539070);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539088);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005390A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005390B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005390D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005390E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539100);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539118);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539130);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539148);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539160);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539178);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539190);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005391A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005391C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005391D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005391F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539208);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539220);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539238);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539250);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539268);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539280);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539298);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005392B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005392C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005392E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005392F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539310);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539328);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539340);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539358);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539370);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539388);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005393A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005393B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005393D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005393E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539400);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539418);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539430);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539448);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539460);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539478);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539490);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005394A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005394C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005394D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005394F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539508);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539520);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539538);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539550);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539560);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539570);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539580);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539590);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005395F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539600);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539610);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539620);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539630);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539648);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539660);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539678);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539688);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539698);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005396A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005396B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005396D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005396E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539700);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539718);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539730);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539748);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539760);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539778);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539790);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005397A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005397C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005397D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005397F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539808);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539820);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539838);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539850);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539868);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539880);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539898);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005398B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005398C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005398E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005398F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539910);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539928);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539940);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539958);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539970);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539988);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005399A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005399B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005399D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_005399E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539A90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539AA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539AC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539AD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539AF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B68);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B80);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539B98);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539BB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539BC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539BE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539BF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539C88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539CA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539CB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539CD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539CE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D00);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D18);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D30);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D48);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D60);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D78);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539D90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539DA8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539DC0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539DD8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539DF0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E08);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E20);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E38);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E50);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539E90);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539EB0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539EC8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539EE0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539EF8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F10);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F28);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F40);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F58);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F70);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539F88);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539FA0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539FB8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539FD0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_00539FE8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A000);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A018);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A030);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A048);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A060);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A078);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A090);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A0A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A0C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A0D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A0F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A108);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A120);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A138);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A150);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A168);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A180);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A198);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A1B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A1C8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A1E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A1F8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A210);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A228);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A240);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A258);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A270);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A288);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A298);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A2B8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A2D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A2F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A308);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A320);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A338);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A350);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A368);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A380);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A398);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A3B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A3D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A3E8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A400);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A418);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A430);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A448);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A460);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A478);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A490);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A4A8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A4C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A4D8);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A4F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A508);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A520);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A538);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A550);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A568);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A580);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A590);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5A0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5B0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5C0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5D0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5E0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A5F0);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A600);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A618);

INCLUDE_RODATA(const s32, "pb/pbDebugMode", D_0053A630);

