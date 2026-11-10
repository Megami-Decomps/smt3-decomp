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

extern pbProcHandle_t *record;

extern sdfMat_t *sdfMatNew(void);

extern pbProcHandle_t *blockDB;

extern void *sdfChipAllocDirect(s32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern void *memset(void *a, s32 b, size_t c);

extern s32 pb_draw_mapbase_Process(dds3ProcessID_t *id);

extern void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id);

extern s32 pb_draw_exit_Process(dds3ProcessID_t *id);

extern void pb_draw_exit_Shutdown(dds3ProcessID_t *id);

extern void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);

extern void pbInitMapBaseMat(pbProcHandle_t *panel);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);


pbProcHandle_t *pbInitExitDraw(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbExit_t *work;
    panel->id = dds3StartProcess("pb-draw-exit", 0x2AFD, 1, 1, pb_draw_exit_Process,
                                      (dds3ProcessFunc_t)pb_draw_exit_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->x = 10;
    panel->y = 10;
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0xC);
    panel->data.mat = work;
    memset(work, 0, 0xC);
    dds3SetProcessData(panel->id, panel);
    work->flg = 0;
    work->unk06 = 0;
    work->unk04 = 0;
    work->mat = sdfMatNew();
    sdfMatSetDiffuse(work->mat, 0x70FFFFFF);
    return panel;
}

void pbSetExitDisp(pbProcHandle_t *p, s8 sw) {
    p->data.value->value = sw;
}

s32 pbGetExitDisp(pbProcHandle_t *panel) {
    return panel->data.value->value;
}

void pbSetExitPos(pbProcHandle_t *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}


extern char pbDrawMapbaseProcName[]; /* "pb-draw-mapbase", followed by two aligned empty strings. */

pbProcHandle_t *pbInitMapBaseDraw(pbProcHandle_t *parent, s32 sw) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    pbExit_t *mapWork;
    dds3ProcessID_t *processId;
    panel->id = dds3StartProcess(pbDrawMapbaseProcName, 0x2AFD, 1, 1, pb_draw_mapbase_Process,
                                      (dds3ProcessFunc_t)pb_draw_mapbase_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0xC);
    panel->data.mat = work;
    memset(work, 0, 0xC);
    mapWork = panel->data.mat;
    processId = panel->id;
    mapWork->value = sw;
    dds3SetProcessData(processId, panel);
    pbInitMapBaseMat(panel);
    return panel;
}


void func_001D4140(pbProcHandle_t *panel, s32 value) {
    panel->data.value->value = value;
}


/* Allocation and consumers in pbModel/pbGame establish this 0x370-byte owner. */
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
    pbProcHandle_t *menuProc;
    pbProcHandle_t *helpProc;
    u8 reserved368[4];
    s32 drawCount;
} pbMapWork_t;

extern sdfMemHandle_t *a_key;
extern pbMapWork_t *pbMapWork;
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern sdfTexHandle_t *sdfDevLoadTex(const char *filename);
/* Native initialization writes all three four-float rows. The second row of each is
 * consumed by pbInitGameCamera. */
extern f32 pbGameCameraParam[3][4];
extern f32 pbCameraVec[3][4];
extern sdfVector4_t aViewDown, sdf3DUpVector;
extern void dds3SetLightColor(s32 frame, s32 no, f32 *color);
extern void dds3SetLightDir(s32 frame, s32 no, f32 *direction);
extern void dds3SetAmbientColor(s32 frame, f32 *color);
extern sdf3DScreenParam_t dds3ScreenParam;
extern s32 pbFogParam[];
extern void dds3SetFog(s32 *fog);
extern u32 dds3DrawFlag;
extern void pbResetKeyState(void);
extern void p2sdPlayBGM(s32 track);

INCLUDE_RODATA(const s32, "pb/pbDraw", pbDrawMapbaseProcName);

void pbLoadTextures(s32 mode, s32 stage)
{
    u32 row, column;
    sdfVector4_t light;

    a_key = sdfMemAllocDirect(sizeof(pbMapWork_t));
    pbMapWork = sdfMemLock(a_key);
    memset(pbMapWork, 0, sizeof(pbMapWork_t));
    for (row = 0; row < 20; row++) {
        for (column = 0; column < 20; column++) {
            pbMapWork->map[row][column] =
                (pbMapWork->map[row][column] & 0x1FFF) | 0x2000;
        }
    }
    pbMapWork->charIndex = 1;
    pbMapWork->exitX = pbMapWork->exitY = -1;
    stage--;
    if (stage < 0) {
        pbMapWork->unk35C[0] = 1;
        stage = 0;
    }
    pbMapWork->level = stage;
    pbMapWork->stage = 0;
    pbMapWork->baseTex = sdfDevLoadTex("/pb/sample1.tmx");
    pbMapWork->tex344 = sdfDevLoadTex("/pb/p_boy03.tmx");
    pbMapWork->tex348 = sdfDevLoadTex("/pb/p_boy01.tmx");
    pbMapWork->tex34C = sdfDevLoadTex("/pb/p_boy02.tmx");
    pbMapWork->giveupScript = NULL;
    pbMapWork->mapData = sdfDevLoadFileEx("/pb/pbmsg.bf",
        (u32 *)&pbMapWork->giveupScript, NULL);
    pbMapWork->helpProc = pbMapWork->menuProc = NULL;

    pbGameCameraParam[0][0] = 0.0f;
    pbGameCameraParam[0][1] = -1300.0f;
    pbGameCameraParam[0][2] = -200.0f;
    pbGameCameraParam[0][3] = 0.0f;
    pbCameraVec[0][0] = 0.0f;
    pbCameraVec[0][1] = 0.0f;
    pbCameraVec[0][2] = 0.0f;
    pbCameraVec[0][3] = 0.0f;
    pbGameCameraParam[1][0] = 0.0f;
    pbGameCameraParam[1][1] = -600.0f;
    pbGameCameraParam[1][2] = -500.0f;
    pbGameCameraParam[1][3] = 0.0f;
    pbCameraVec[1][0] = 0.0f;
    pbCameraVec[1][1] = 0.0f;
    pbCameraVec[1][2] = 0.0f;
    pbCameraVec[1][3] = 0.0f;
    pbGameCameraParam[2][0] = 0.0f;
    pbGameCameraParam[2][1] = -700.0f;
    pbGameCameraParam[2][2] = -1000.0f;
    pbGameCameraParam[2][3] = 0.0f;
    pbCameraVec[2][0] = 0.0f;
    pbCameraVec[2][1] = 0.0f;
    pbCameraVec[2][2] = 0.0f;
    pbCameraVec[2][3] = 0.0f;
    SDF_VU0_COPYVECTOR(&aViewDown, &sdf3DUpVector);

    light.x = 1.0f;
    light.y = 1.0f;
    light.z = 1.0f;
    light.w = 0.0f;
    dds3SetLightColor(0, 0, &light.x);
    light.x = 0.0f;
    light.y = 1.0f;
    light.z = 0.5f;
    light.w = 0.0f;
    dds3SetLightDir(0, 0, &light.x);
    light.x = 0.0f;
    light.y = 0.0f;
    light.z = 0.0f;
    light.w = 0.0f;
    dds3SetLightColor(0, 1, &light.x);
    light.x = 1.0f;
    light.y = 0.0f;
    light.z = 0.0f;
    light.w = 0.0f;
    dds3SetLightDir(0, 1, &light.x);
    light.x = 0.0f;
    light.y = 0.0f;
    light.z = 0.0f;
    light.w = 0.0f;
    dds3SetLightColor(0, 2, &light.x);
    light.x = 0.0f;
    light.y = 0.0f;
    light.w = 0.0f;
    light.z = 1.0f;
    dds3SetLightDir(0, 2, &light.x);
    light.x = 0.65f;
    light.y = 0.65f;
    light.z = 0.65f;
    light.w = 1.0f;
    dds3SetAmbientColor(0, &light.x);
    dds3ScreenParam.fovy = 0.6283185f;
    dds3SetFog(pbFogParam);
    dds3DrawFlag &= ~0x400;
    pbResetKeyState();
    p2sdPlayBGM(0x340000);
}
