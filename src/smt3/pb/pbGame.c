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

typedef struct { s16 number, label; } pbStageEntry_t;

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

typedef struct
{
    s16 clearBit;
    s16 unk02;
    pbStageEntry_t stage[1];
} pbLevel_t;

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

extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;

extern dds3ProcessID_t *scr_msg;

extern void sdfChipFreeDelayedDirect(void *ptr);

extern void sdfMemFreeDirect(sdfMemHandle_t *handle);

extern pbLevel_t *pbLevelTable[];

extern pbMapWork_t *D_0054F5C4;

extern pbGoal_t *goal;

extern pbPanel_t *record;

extern void sdfMatFreeDelayed(sdfMat_t *mat);

extern void dds3ResetProcessFlag(dds3ProcessID_t *id, u32 mask, s32 value);

extern void pbGotoNewStage(void);

typedef void (*pbCharacterFunc_t)(void);

extern pbCharacterFunc_t pbCharacterTable[];

extern pbPanel_t *blockDB;

extern u32 current_char;

extern f32 D_00327370[];

extern s32 pbPopRecord(pbPanel_t *record, pbPanel_t **chars, pbPanel_t *db);

extern pbPanel_t *pbStartErrorDraw(pbPanel_t *parent, s32 code, s32 timer);

extern void *sdfChipAllocDirect(u32 size);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 ClearProc(dds3ProcessID_t *id);

extern void DeleteClearProc(dds3ProcessID_t *id);

extern void rsetOtOfs(s32 ofs);

extern void rsetAlpha(s32 mode);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, void *);

extern void *memset(void *dst, s32 c, u32 n);

extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern char D_005361B0[], D_005361C8[], D_005365D8[], D_00536398[], D_00535EB8[];

extern char D_00536040[], D_00535CE8[], D_00535CD8[], D_00535FA8[];

extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern s32 viewModeCamera(dds3ProcessID_t *id);

extern void deleteViewModeCamera(dds3ProcessID_t *id);

extern char D_00535E00[], D_00535E10[], D_00535E20[], D_00535EC8[], D_00535FC8[];

extern void updateMotion(void);

extern void pbFreeGroupHandles(pbBlockDB_t *db);

extern void pbFreeGroupEntry(mdlResrc_t *entry);

extern void fclClearFlag(s32 flag);

extern dds3ProcessID_t *dds3SearchProcessName(char *name);

extern void dds3SetProcessFlag(dds3ProcessID_t *id, u32 mask, s32 value);

extern void setRoundBlockData(pbBlockObj_t *block, s16 index);

extern s32 pbStartAutoRoundCamera(s32, s32);

extern void func_001D2D00(pbPanel_t *panel, s32 arg);

extern pbPanel_t *mychar[];

extern s32 moveB, roundCamera;

extern char D_00535EF8[];

extern u32 pbPushRecord(pbPanel_t *panel);

extern u32 pbMyCharacterDispStat(pbPanel_t *panel);

extern dds3ProcessID_t *pbStartGiveupConfirm(void);

extern s8 pbCheckPopRecord(pbPanel_t *panel);

extern s8 pbCheckPreviousRecord(void);

extern u8 pbRotLKeyAct(void);

extern u8 pbRotRKeyAct(void);

extern s32 moveViewCamera;

extern sdfVector4_t aViewEye, aViewTarget, pbGameCameraParam, pbCameraVec;

extern sdfVector4_t pbInitCameraParam, pbCameraVec2;

extern void pbSetGameCamera1(void);

extern void pbSetGameCamera2(void);

extern void pbPreviousRecord(void);

extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);

extern dds3ProcessID_t *scrStartScript2(s32 handle, u8 *script, s32 arg);


s32 getCharacterNum(void) {
    s32 count = 0;
    u32 i;
    for (i = 0; i < D_0054F5C4->pcharNum; i++) {
        if (pbMyCharacterDispStat(mychar[i]) != 0) {
            count++;
        }
    }
    return count;
}

void pbNextCharacter(void) {
    do {
        current_char = (current_char + D_0054F5C4->pcharNum + 1) % D_0054F5C4->pcharNum;
    } while (pbMyCharacterDispStat(mychar[current_char]) == 0);
    updateMotion();
    if (pbCharacterTable[D_0054F5C4->charIndex] != NULL) {
        pbCharacterTable[D_0054F5C4->charIndex]();
    }
}

void updateMotion(void) {
    u32 i;
    for (i = 0; i < D_0054F5C4->pcharNum; i++) {
        func_001D2D00(mychar[i], 0);
    }
}

INCLUDE_ASM(const s32, "pb/pbGame", hitCheck);

INCLUDE_ASM(const s32, "pb/pbGame", pushCheck);

INCLUDE_ASM(const s32, "pb/pbGame", roundCheck);

INCLUDE_ASM(const s32, "pb/pbGame", roundDirCheck);

INCLUDE_ASM(const s32, "pb/pbGame", hallCheck);

INCLUDE_ASM(const s32, "pb/pbGame", pbFindProcHandle);

s8 goalHitCheck(s16 x, s16 y) {
    if ((x == goal->pos.x) && (y == goal->pos.y)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "pb/pbGame", gameKey0);

INCLUDE_ASM(const s32, "pb/pbGame", gameKey1);

u32 RotateBit(u32 val, s32 loop) {
    s32 i;
    for (i = 0; i < loop; i++) {
        u32 bit = (val & 1) << 3;
        val >>= 1;
        val |= bit;
    }
    return val;
}

INCLUDE_ASM(const s32, "pb/pbGame", gameKey2);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535EF8);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F08);

INCLUDE_ASM(const s32, "pb/pbGame", game);

INCLUDE_ASM(const s32, "pb/pbGame", deleteGame);

INCLUDE_ASM(const s32, "pb/pbGame", pbInitGame);

INCLUDE_ASM(const s32, "pb/pbGame", moveChar);

INCLUDE_ASM(const s32, "pb/pbGame", deleteMoveChar);

INCLUDE_ASM(const s32, "pb/pbGame", setMoveBlockData);


/* NON_MATCHING: 30 of 31 words, 124 bytes against retail's 140; retail holds the delta row in three pointer copies and materialises the -1 return late. */
#ifdef NON_MATCHING
s32 moveBlock(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    if (work->step >= 4) {
        return -1;
    }
    {
        f32 *delta = &D_00327370[work->index * 3];
        pbBlockObj_t *obj = work->block;
        obj->x = obj->x + delta[0];
        obj->y = obj->y + delta[1];
        obj->z = obj->z + delta[2];
    }
    work->step = work->step + 1;
    return 0;
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", moveBlock);
#endif


INCLUDE_ASM(const s32, "pb/pbGame", deleteMoveBlock);

void rotXY(s16 *x, s16 *y, s32 dir) {
    s16 ox = *x;
    if ((ox == 0) && (*y == 0)) {
        return;
    }
    *x = *y;
    *y = ox;
    if (dir == -1) {
        if (ox == 0) {
            s16 v = *x;
            *x = -v;
        }
    } else if (dir == 1) {
        if (*x == 0) {
            *y = -ox;
        }
    }
}

INCLUDE_ASM(const s32, "pb/pbGame", setRoundBlockData);

INCLUDE_ASM(const s32, "pb/pbGame", roundBlock);

void deleteRoundBlock(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    pbPushRecord(record);
    setRoundBlockData(work->block, work->index);
    work->block->value = 0;
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    moveB = 0;
}

INCLUDE_ASM(const s32, "pb/pbGame", roundAutoCamera);

void deleteRoundAutoCamera(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    if (cameraAngle > 6.28318501f) {
        cameraAngle = cameraAngle - 6.28318501f;
    } else if (cameraAngle < 0.0f) {
        cameraAngle = cameraAngle + 6.28318501f;
    }
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    roundCamera = 0;
}

INCLUDE_ASM(const s32, "pb/pbGame", viewModeCamera);

void deleteViewModeCamera(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    moveViewCamera = 0;
}

s32 ClearProc(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0, 0xC3, 0x200, 0x3A, 0, 0x1C4, 0x200, 0x3A, 0x80808080,
                D_0054F5C4->tex34C);
    work->value = work->value - 1;
    if (work->value < 0) {
        return -1;
    }
    return 0;
}

void DeleteClearProc(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->stage = (u16)D_0054F5C4->stage + 1;
    pbGotoNewStage();
}

INCLUDE_ASM(const s32, "pb/pbGame", CalcDegreeMode);

s16 GetStageNumber(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->number;
}

s16 GetStageScriptLabel(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->label;
}

s16 GetAllStageClearBit(void) {
    return pbLevelTable[D_0054F5C4->level]->clearBit;
}

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F40);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F48);

INCLUDE_ASM(const s32, "pb/pbGame", pbStartGame);

INCLUDE_ASM(const s32, "pb/pbGame", pbStartMoveChar);

INCLUDE_ASM(const s32, "pb/pbGame", pbStartMoveBlock);

INCLUDE_ASM(const s32, "pb/pbGame", pbStartRoundBlock);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F78);

INCLUDE_ASM(const s32, "pb/pbGame", pbStartAutoRoundCamera);

pbPanel_t *pbStartViewModeCamera(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbViewCamWork_t *work;
    panel->process = dds3StartProcess(D_00535FA8, 0x3EE, 0, 0, viewModeCamera,
                                      (dds3ProcessFunc_t)deleteViewModeCamera, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0x30);
    panel->data.viewCam = work;
    memset(work, 0, 0x30);
    work->scale = 1.0f;
    SDF_VU0_COPYVECTOR(&work->eye, &aViewEye);
    SDF_VU0_COPYVECTOR(&work->target, &aViewTarget);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbRoundCamera(s32 dir) {
    roundCamera = pbStartAutoRoundCamera(0, (dir != -1) ? -1 : 1);
    if (dir == -1) {
        pbRotLKeyAct();
    } else {
        pbRotRKeyAct();
    }
}

void pbSetGameCamera0(void) {
    SDF_VU0_COPYVECTOR(&aViewEye, &pbGameCameraParam);
    SDF_VU0_COPYVECTOR(&aViewTarget, &pbCameraVec);
}

INCLUDE_ASM(const s32, "pb/pbGame", pbSetGameCamera1);

INCLUDE_ASM(const s32, "pb/pbGame", pbSetGameCamera2);

INCLUDE_ASM(const s32, "pb/pbGame", pbSetGameCamera3);

INCLUDE_ASM(const s32, "pb/pbGame", pbSetGameCamera4);


/* NON_MATCHING: 26 of 38 words; gcc range-tests case 0 where retail compares it. */
#ifdef NON_MATCHING
void pbInitGameCamera(void) {
    switch (D_0054F5C4->charIndex) {
    case 0:
        pbSetGameCamera0();
        cameraAngle = 0.0f;
        break;
    case 1:
        SDF_VU0_COPYVECTOR(&aViewEye, &pbInitCameraParam);
        SDF_VU0_COPYVECTOR(&aViewTarget, &pbCameraVec2);
        pbSetGameCamera1();
        break;
    case 2:
        pbSetGameCamera2();
        break;
    }
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", pbInitGameCamera);
#endif


s8 pbCheckPreviousRecord(void) {
    s8 ret = pbCheckPopRecord(record);
    return ret;
}

void pbPreviousRecord(void) {
    s32 r = pbPopRecord(record, mychar, blockDB);
    if (r >= 0) {
        pbStartErrorDraw(0, 9, 30);
        if (blockDB != NULL) {
            blockDB->flags = 0x10000;
        }
        current_char = r;
        updateMotion();
        if (pbCharacterTable[D_0054F5C4->charIndex] != NULL) {
            pbCharacterTable[D_0054F5C4->charIndex]();
        }
    }
}

pbPanel_t *pbStartClear(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->process = dds3StartProcess(D_00535FC8, 0x3EE, 1, 1, ClearProc,
                                      (dds3ProcessFunc_t)DeleteClearProc, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(4);
    work->value = 30;
    panel->data.value = work;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

dds3ProcessID_t *pbStartGiveupConfirm(void) {
    fclClearFlag(0);
    scr_msg = scrStartScript2(0x3EE, D_0054F5C4->giveupScript, 1);
    dds3SetProcessFlag(dds3SearchProcessName(D_00535EF8), 0x10, 2);
    return scr_msg;
}

void pbGetCameraParam(f32 *param) {
    param[0] = cameraAngle;
    param[1] = cameraRadius;
    param[2] = cameraDegree;
}

void func_001D8EA0(f32 angle) {
    cameraAngle = angle;
}

void pbSetCameraRadius(f32 radius) {
    cameraRadius = radius;
}

void pbSetCameraDegree(f32 degree) {
    cameraDegree = degree;
}

void pbSetCameraDegreeInit(f32 degree) {
    cameraDegreeInit = degree;
}

dds3ProcessID_t *pbGetMessageScriptProc(void) {
    return scr_msg;
}

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FA8);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FC8);

INCLUDE_ASM(const s32, "pb/pbGame", pbGotoNewStage);

void pbRoundStageNext(s16 *level, s16 *stage) {
    pbLevel_t *lv = pbLevelTable[*level];
    *stage = (u16)*stage + 1;
    if ((lv->stage + *stage)->number < 0) {
        *stage = 0;
        *level = (u16)*level + 1;
        if (*level >= 3) {
            *level = 0;
        }
    }
}

void pbRoundStagePrev(s16 *level, s16 *stage) {
    pbLevel_t *lv;
    s32 s = (u16)*stage - 1;
    *stage = s;
    if ((s16)s < 0) {
        s32 l = (u16)*level - 1;
        *level = l;
        if ((s16)l < 0) {
            *level = 2;
        }
        lv = pbLevelTable[*level];
        *stage = 0;
        while ((lv->stage + *stage + 1)->number >= 0) {
            *stage = (u16)*stage + 1;
        }
    }
}

void *pbFindGroupEntry(pbGroupDB_t *db, u32 group) {
    pbGroupEntry_t *found = NULL;
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbGroupEntry_t *e = db->entry[i];
        if (group == e->group) {
            found = e;
            break;
        }
    }
    return found;
}

INCLUDE_ASM(const s32, "pb/pbGame", pbAddGroupEntry);

void pbFreeGroupEntry(mdlResrc_t *entry) {
    sdfMemFreeDirect(entry->resourceHandle);
    sdfChipFreeDelayedDirect(entry);
}

void pbFreeGroupHandles(pbBlockDB_t *db) {
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbFreeGroupEntry((mdlResrc_t *)db->entry[i]);
    }
    db->count = 0;
}

INCLUDE_ASM(const s32, "pb/pbGame", pbGetBlockAt);

INCLUDE_ASM(const s32, "pb/pbGame", pb_proc_block_DB_Process);

void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbBlockDB_t *db = panel->data.blockDB;
    sdfMatFreeDelayed(db->mat);
    pbFreeGroupHandles(db);
    sdfChipFreeDelayedDirect(db);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "pb/pbGame", pb_draw_map_cursor_Process);

void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_RODATA(const s32, "pb/pbGame", D_00536000);

