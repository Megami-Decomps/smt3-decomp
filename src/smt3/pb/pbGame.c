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

typedef struct { s16 number, label; } pbStageEntry_t;

typedef struct { u16 blk[20][20]; } pbMapBlock_t;

typedef struct { u8 unk00[0x10]; pbMapBlock_t map; } pbMapArea_t;

typedef struct
{
    s32 pcharNum;
    u8 unk004[8];
    pbMapArea_t area;
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

typedef struct { s16 dir; u16 step; f32 speed; s32 unk08; } pbMoveCharWork_t;

typedef struct { s32 dir; f32 angle; } pbRoundCamWork_t;

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
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; pbMoveCharWork_t *moveChar;
            pbRoundCamWork_t *roundCam; } data;
};

extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;

extern dds3ProcessID_t *scr_msg;

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern pbLevel_t *pbLevelTable[];

extern pbMapWork_t *D_0054F5C4;

extern pbGoal_t *goal;

extern pbProcHandle_t *record;

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern void pbGotoNewStage(void);

typedef void (*pbCharacterFunc_t)(void);

extern pbCharacterFunc_t pbCharacterTable[];

extern pbProcHandle_t *blockDB;

extern u32 current_char;

extern f32 D_00327370[];

extern s32 pbPopRecord(pbProcHandle_t *p, pbProcHandle_t **c, pbProcHandle_t *blkDB);

extern pbProcHandle_t *pbStartErrorDraw(pbProcHandle_t *parent, s32 eCode, s32 frame);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 ClearProc(dds3ProcessID_t *id);

extern void DeleteClearProc(dds3ProcessID_t *id);

extern void rsetOtOfs(s32 otofs);

extern void rsetAlpha(s32 mode);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, void *);

extern void *memset(void *a, s32 b, u32 c);

extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);

extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);

extern char D_005361B0[];

extern char D_00536040[], D_00535CE8[], D_00535FA8[];

extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);

extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);

extern s32 viewModeCamera(dds3ProcessID_t *id);

extern s32 moveChar(dds3ProcessID_t *id);

extern void deleteMoveChar(dds3ProcessID_t *id);

extern char D_00535F60[], D_00535F78[], D_00535F50[], D_00535F90[];

extern s32 roundBlock(dds3ProcessID_t *id);

extern u8 degree_mode;

extern s8 dds3PadButtonState[];

extern u8 pbGameKeyState;

extern u32 RotateBit(u32 val, s32 loop);

extern void pbRoundCamera(s32 dir);

typedef struct { s16 x, y; } pbCursorPos_t;

extern pbCursorPos_t D_00551EC4;

extern sdfVector4_t D_0056CDD0[2];

extern void pbSetMapCursorFlag(pbProcHandle_t *p, s32 flag);

extern void pbSetGameCamera4(s16 x, s16 y);

extern void pbRotateXCamera(f32 angle);

extern void pbRotateYCamera(f32 angle);

extern u8 pbGetKeyAction(s32 press);

extern void dds3SetQuake(s32 frame, s32 range);

extern void pbDeleteGroupEntry(pbProcHandle_t *db, u32 grpID);

extern void pbRebuildOutline(pbGroupData_t *p);

extern pbProcHandle_t *pbStartMoveBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir);

typedef struct { s16 x, y; } pbKeyEntry_t;

extern s16 pbKeyTable[][2];

extern void pbGetMyCharcterPos2(pbProcHandle_t *p, s16 *x, s16 *y);

extern void pbMoveMyCharcter(pbProcHandle_t *p, f32 x, f32 y);

extern s32 game(dds3ProcessID_t *id);

extern s32 roundAutoCamera(dds3ProcessID_t *id);

extern s32 pbGetKeyMode(void);

extern char D_00535F08[];

extern void deleteGame(dds3ProcessID_t *id);

extern void pbLoadSystemData(s32 num);

extern pbProcHandle_t *pbStartBlockDataBase(pbProcHandle_t *parent);

extern pbProcHandle_t *pbInitMapBaseDraw(pbProcHandle_t *parent, s32 sw);

extern pbProcHandle_t *pbInitExitDraw(pbProcHandle_t *parent);

extern pbProcHandle_t *pbStartRecord(pbProcHandle_t *parent);

extern pbProcHandle_t *pbStartDrawMapCursor(pbProcHandle_t *parent);

extern pbProcHandle_t *bmap, *mapCursor;

extern s32 D_0054F5F0;

extern pbMapBlock_t D_00327458;

extern s32 moveBlock(dds3ProcessID_t *id);

extern void deleteMoveBlock(dds3ProcessID_t *id);

extern s32 p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern u32 pbAddRecord_flor(pbProcHandle_t *p, pbGroupData_t *grp);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);

extern u32 pbAddRecord_blk(pbProcHandle_t *p, pbGroupData_t *grp);

extern void deleteViewModeCamera(dds3ProcessID_t *id);

extern char D_00535E10[], D_00535EC8[], D_00535FC8[];

extern void updateMotion(void);

extern void pbFreeGroupHandles(pbBlockDB_t *db);

extern void pbFreeGroupEntry(dds3ModelResrc_t *entry);

extern void fclClearFlag(s32 flag);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern void setRoundBlockData(pbGroupData_t *grp, s32 dir);

extern void func_001D2D00(pbProcHandle_t *panel, s32 arg);

extern pbProcHandle_t *mychar[];

extern pbProcHandle_t *moveB;

extern pbProcHandle_t *roundCamera;

extern char D_00535EF8[];

extern u32 pbPushRecord(pbProcHandle_t *p);

extern u32 pbMyCharacterDispStat(pbProcHandle_t *p);

extern dds3ProcessID_t *pbStartGiveupConfirm(void);

extern s8 pbCheckPopRecord(pbProcHandle_t *p);

extern s8 pbCheckPreviousRecord(void);

extern u8 pbRotLKeyAct(void);

extern u8 pbRotRKeyAct(void);

extern pbProcHandle_t *moveViewCamera;

extern sdfVector4_t aViewEye, aViewTarget, pbGameCameraParam, pbCameraVec;

extern sdfVector4_t pbInitCameraParam, pbCameraVec2;

extern void pbSetGameCamera1(void);

extern void pbSetGameCamera2(void);

extern void pbPreviousRecord(void);

extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern dds3ProcessID_t *scrStartScript2(s32 priority, u8 *scrdata, s32 startproc);


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

s8 hallCheck(pbGroupData_t *grp) {
    u32 i;
    s32 cx = grp->cellX;
    s32 cy = grp->cellY;
    for (i = 0; i < grp->num; i++) {
        if ((D_0054F5C4->area.map.blk[cy + grp->blk[i].y][cx + grp->blk[i].x] & 0x800) == 0) {
            break;
        }
    }
    if (i < grp->num) {
        return 0;
    }
    return 1;
}

s32 pbFindProcHandle(s16 px, s16 py) {
    s16 i;
    s32 found = 0;
    s16 x, y;
    for (i = 0; (u32)i < D_0054F5C4->pcharNum; i++) {
        if (i == current_char) {
            continue;
        }
        if (pbMyCharacterDispStat(mychar[i]) == 0) {
            continue;
        }
        pbGetMyCharcterPos2(mychar[i], &x, &y);
        if (px == x && py == y) {
            found = 1;
            break;
        }
    }
    return found;
}

s8 goalHitCheck(s16 x, s16 y) {
    if ((x == goal->pos.x) && (y == goal->pos.y)) {
        return 1;
    }
    return 0;
}

s32 gameKey0(void) {
    s32 rot;
    s32 ret = -1;
    if ((dds3PadButtonState[0x28] != 0) && (dds3PadButtonState[0x2A] != 0)) {
        rot = 1;
    } else {
        rot = 0;
    }
    if (dds3PadButtonState[0x26] & 2) {
        if (rot != 0) {
            pbRotateXCamera(0.2f);
        } else {
            ret = pbGetKeyAction(0);
        }
    } else if (dds3PadButtonState[0x27] & 2) {
        if (rot != 0) {
            pbRotateXCamera(-0.2f);
        } else {
            ret = pbGetKeyAction(2);
        }
    } else if (dds3PadButtonState[0x24] & 2) {
        ret = pbGetKeyAction(3);
    } else if (dds3PadButtonState[0x25] & 2) {
        ret = pbGetKeyAction(1);
    }
    if ((dds3PadButtonState[0x28] != 0) && (rot == 0)) {
        pbRoundCamera(-1);
    } else if ((dds3PadButtonState[0x2A] != 0) && (rot == 0)) {
        pbRoundCamera(1);
    }
    return ret;
}

/* NON_MATCHING: 150 of 168 words, 672 bytes against retail's 680; the cursor pair is one splat symbol where retail has two gp-addressed globals. */
#ifdef NON_MATCHING
s32 gameKey1(void) {
    s32 ret = -1;
    if (dds3PadButtonState[0x22] < 0) {
        p2sdPlaySE(0x340009, 0x7F, 0x3F);
        if (pbGameKeyState == 0) {
            pbGetMyCharcterPos2(mychar[current_char], &D_00551EC4.x, &D_00551EC4.y);
            SDF_VU0_COPYVECTOR(&D_0056CDD0[0], &aViewEye);
            SDF_VU0_COPYVECTOR(&D_0056CDD0[1], &aViewTarget);
            pbGameKeyState = 1;
        } else {
            pbSetMapCursorFlag(mapCursor, 0);
            SDF_VU0_COPYVECTOR(&aViewEye, &D_0056CDD0[0]);
            SDF_VU0_COPYVECTOR(&aViewTarget, &D_0056CDD0[1]);
            pbGameKeyState = 0;
            if (moveViewCamera != NULL) {
                moveViewCamera->flags = 0x50000;
            }
        }
    }
    if (dds3PadButtonState[0x23] < 0) {
        p2sdPlaySE(0x340009, 0x7F, 0x3F);
        degree_mode = (degree_mode ^ 2) | 1;
    }
    if (dds3PadButtonState[0x26] != 0) {
        ret = pbGetKeyAction(0);
    } else if (dds3PadButtonState[0x27] != 0) {
        ret = pbGetKeyAction(2);
    } else if (dds3PadButtonState[0x24] != 0) {
        ret = pbGetKeyAction(3);
    } else if (dds3PadButtonState[0x25] != 0) {
        ret = pbGetKeyAction(1);
    }
    if (pbGameKeyState != 0) {
        u32 flags;
        if ((u32)(D_00551EC4.x + pbKeyTable[ret][0]) < 20) {
            D_00551EC4.x += pbKeyTable[ret][0];
        }
        if ((u32)(D_00551EC4.y + pbKeyTable[ret][1]) < 20) {
            D_00551EC4.y += pbKeyTable[ret][1];
        }
        pbSetGameCamera4(D_00551EC4.x, D_00551EC4.y);
        flags = D_00551EC4.y < 0x13;
        if (D_00551EC4.x > 0) {
            flags |= 2;
        }
        if (D_00551EC4.y > 0) {
            flags |= 4;
        }
        if (D_00551EC4.x < 0x13) {
            flags |= 8;
        }
        ret = -1;
        pbSetMapCursorFlag(mapCursor, RotateBit(flags, pbGetKeyMode()));
    }
    if (dds3PadButtonState[0x28] < 0) {
        p2sdPlaySE(0x340009, 0x7F, 0x3F);
        pbRoundCamera(-1);
    } else if (dds3PadButtonState[0x2A] < 0) {
        p2sdPlaySE(0x340009, 0x7F, 0x3F);
        pbRoundCamera(1);
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", gameKey1);
#endif

u32 RotateBit(u32 val, s32 loop) {
    s32 i;
    for (i = 0; i < loop; i++) {
        u32 bit = (val & 1) << 3;
        val >>= 1;
        val |= bit;
    }
    return val;
}

s32 gameKey2(void) {
    s32 rot;
    s32 ret = -1;
    if ((dds3PadButtonState[0x28] != 0) && (dds3PadButtonState[0x2A] != 0)) {
        rot = 1;
    } else {
        rot = 0;
    }
    if (dds3PadButtonState[0x26] & 2) {
        if (rot != 0) {
            pbRotateXCamera(0.2f);
        } else {
            ret = pbGetKeyAction(0);
        }
    } else if (dds3PadButtonState[0x27] & 2) {
        if (rot != 0) {
            pbRotateXCamera(-0.2f);
        } else {
            ret = pbGetKeyAction(2);
        }
    } else if (dds3PadButtonState[0x24] & 2) {
        ret = pbGetKeyAction(3);
    } else if (dds3PadButtonState[0x25] & 2) {
        ret = pbGetKeyAction(1);
    }
    if ((dds3PadButtonState[0x28] != 0) && (rot == 0)) {
        pbRotateYCamera(0.2f);
    } else if ((dds3PadButtonState[0x2A] != 0) && (rot == 0)) {
        pbRotateYCamera(-0.2f);
    }
    return ret;
}

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535EF8);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F08);

INCLUDE_ASM(const s32, "pb/pbGame", game);

void deleteGame(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    D_0054F5C4->area.map = D_00327458;
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "pb/pbGame", pbInitGame);

s32 moveChar(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbMoveCharWork_t *work = panel->data.moveChar;
    u32 step;
    if (work->step < 4) {
        s32 dir = work->dir;
        pbMoveMyCharcter(mychar[current_char], D_00327370[dir * 3] * work->speed,
                         D_00327370[dir * 3 + 2] * work->speed);
    }
    if (pbCharacterTable[D_0054F5C4->charIndex] != NULL) {
        pbCharacterTable[D_0054F5C4->charIndex]();
    }
    step = work->step + 1;
    work->step = step;
    return ((u16)step < 4) ? 0 : -1;
}

INCLUDE_ASM(const s32, "pb/pbGame", deleteMoveChar);

void setMoveBlockData(pbGroupData_t *grp, s32 dir) {
    u32 i = 0;
    grp->cellX += pbKeyTable[dir][0];
    grp->cellY += pbKeyTable[dir][1];
    if (grp->num != 0) {
        pbMapArea_t *a = &D_0054F5C4->area;
        do {
            s16 y = grp->cellY + grp->blk[i].y;
            s16 x = grp->cellX + grp->blk[i].x;
            a->map.blk[y][x] &= 0xF800;
            a->map.blk[y][x] = (a->map.blk[y][x] | (grp->grpID & 0x7FF)) & 0x1FFF;
            a->map.blk[y][x] |= grp->blk[i].kind << 13;
            i++;
        } while (i < grp->num);
    }
}


/* NON_MATCHING: 30 of 31 words, 124 bytes against retail's 140; retail holds the delta row in three pointer copies and materialises the -1 return late. */
#ifdef NON_MATCHING
s32 moveBlock(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    if (work->step >= 4) {
        return -1;
    }
    {
        f32 *delta = &D_00327370[work->index * 3];
        pbGroupData_t *obj = work->block;
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


/* NON_MATCHING: 46 of 77 words, 308 bytes against retail's 324; the entry array is hoisted out of the loop retail reloads it in. */
#ifdef NON_MATCHING
void deleteMoveBlock(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    work->block->value = 0;
    if (work->index != 4) {
        setMoveBlockData(work->block, work->index);
        if (hallCheck(work->block) == 1) {
            moveB = pbStartMoveBlock(NULL, work->block, 4);
        } else {
            pbPushRecord(record);
            moveB = NULL;
        }
    } else {
        pbGroupData_t *block;
        u32 i;
        dds3SetQuake(0x1E, 0x50);
        pbPushRecord(record);
        block = work->block;
        i = 0;
        if (block->num != 0) {
            pbMapArea_t *a = &D_0054F5C4->area;
            do {
                s16 y = block->cellY + (block->blk + i)->y;
                s16 x = block->cellX + (block->blk + i)->x;
                a->map.blk[y][x] &= 0xF7FF;
                i++;
            } while (i < block->num);
        }
        pbDeleteGroupEntry(blockDB, block->grpID);
        moveB = NULL;
    }
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", deleteMoveBlock);
#endif

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

void setRoundBlockData(pbGroupData_t *grp, s32 dir) {
    u32 i = 0;
    grp->rot = 0.0f;
    if (grp->num != 0) {
        do {
            s16 y, x;
            rotXY(&grp->blk[i].x, &grp->blk[i].y, dir);
            y = grp->cellY + grp->blk[i].y;
            x = grp->cellX + grp->blk[i].x;
            D_0054F5C4->area.map.blk[y][x] &= 0xF800;
            D_0054F5C4->area.map.blk[y][x] =
                (D_0054F5C4->area.map.blk[y][x] | (grp->grpID & 0x7FF)) & 0x1FFF;
            D_0054F5C4->area.map.blk[y][x] |= grp->blk[i].kind << 13;
            i++;
        } while (i < grp->num);
    }
    pbRebuildOutline(grp);
}

/* NON_MATCHING: 12 of 34 words, 136 bytes against retail's 140; the two clamp stores cross-jump and the guard exit lands last. */
#ifdef NON_MATCHING
s32 roundBlock(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    u32 step = work->step;
    if (step < 4) {
        pbGroupData_t *block = work->block;
        block->rot += work->index * 0.39269909f;
        if (block->rot > 6.28318501f) {
            block->rot -= 6.28318501f;
        } else if (block->rot < -6.28318501f) {
            block->rot += 6.28318501f;
        }
        work->step = step + 1;
        return 0;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", roundBlock);
#endif

void deleteRoundBlock(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    pbPushRecord(record);
    setRoundBlockData(work->block, work->index);
    work->block->value = 0;
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    moveB = NULL;
}

s32 roundAutoCamera(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbRoundCamWork_t *work = panel->data.roundCam;
    s32 ret = 0;
    if (work->dir > 0) {
        cameraAngle += 0.2f;
        if (cameraAngle >= work->angle) {
            if (work->angle >= 6.28318501f) {
                cameraAngle = 0.0f;
            } else {
                cameraAngle = work->angle;
            }
            ret = -1;
        }
    } else {
        cameraAngle -= 0.2f;
        if (cameraAngle <= work->angle) {
            cameraAngle = work->angle;
            ret = -1;
        }
    }
    return ret;
}

void deleteRoundAutoCamera(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    if (cameraAngle > 6.28318501f) {
        cameraAngle = cameraAngle - 6.28318501f;
    } else if (cameraAngle < 0.0f) {
        cameraAngle = cameraAngle + 6.28318501f;
    }
    dds3ResetProcessFlag(panel->id, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    roundCamera = NULL;
}

INCLUDE_ASM(const s32, "pb/pbGame", viewModeCamera);

void deleteViewModeCamera(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    moveViewCamera = NULL;
}

s32 ClearProc(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
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
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->stage = (u16)D_0054F5C4->stage + 1;
    pbGotoNewStage();
}

void CalcDegreeMode(void) {
    f32 d;
    if (degree_mode & 2) {
        d = cameraDegree + 0.15f;
        if (d > 1.560796142f) {
            cameraDegree = 1.560796142f;
            degree_mode = degree_mode & 0xFE;
            return;
        }
    } else {
        d = cameraDegree - 0.15f;
        if (d < cameraDegreeInit) {
            cameraDegree = cameraDegreeInit;
            degree_mode = degree_mode & 0xFE;
            return;
        }
    }
    cameraDegree = d;
}

s16 GetStageNumber(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->number;
}

s16 GetStageScriptLabel(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->label;
}

s16 GetAllStageClearBit(void) {
    return pbLevelTable[D_0054F5C4->level]->clearBit;
}

pbProcHandle_t *pbStartGame(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    u32 i;
    pbLoadSystemData(GetStageNumber());
    panel->id = dds3StartProcess(D_00535F50, 0x3EE, 1, 1, game,
                                      (dds3ProcessFunc_t)deleteGame, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    dds3SetProcessData(panel->id, panel);
    blockDB = NULL;
    bmap = NULL;
    goal = NULL;
    record = NULL;
    mapCursor = NULL;
    roundCamera = NULL;
    moveViewCamera = NULL;
    D_0054F5F0 = 0;
    scr_msg = NULL;
    for (i = 0; i < 4; i++) {
        mychar[i] = NULL;
    }
    blockDB = pbStartBlockDataBase(panel);
    bmap = pbInitMapBaseDraw(panel, 1);
    goal = (pbGoal_t *)pbInitExitDraw(panel);
    record = pbStartRecord(panel);
    mapCursor = pbStartDrawMapCursor(panel);
    D_00327458 = D_0054F5C4->area.map;
    cameraDegree = 1.1f;
    cameraRadius = 700.0f;
    cameraDegreeInit = 1.1f;
    cameraAngle = 0.0f;
    pbInitGameCamera();
    return panel;
}

pbProcHandle_t *pbStartMoveChar(pbProcHandle_t *parent, s32 dir, u32 scl) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbMoveCharWork_t *work;
    panel->id = dds3StartProcess(D_00535F60, 0x3EE, 0, 0, moveChar,
                                      (dds3ProcessFunc_t)deleteMoveChar, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0xC);
    panel->data.moveChar = work;
    memset(work, 0, 0xC);
    work->dir = dir;
    work->step = 0;
    work->speed = scl;
    dds3SetProcessData(panel->id, panel);
    func_001D2D00(mychar[current_char], 1);
    return panel;
}

pbProcHandle_t *pbStartMoveBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbRoundBlock_t *work;
    u32 i;
    panel->id = dds3StartProcess(D_00535F78, 0x3EE, 0, 0, moveBlock,
                                      (dds3ProcessFunc_t)deleteMoveBlock, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0xC);
    panel->data.round = work;
    memset(work, 0, 0xC);
    work->step = 0;
    work->index = dir;
    work->block = grp;
    dds3SetProcessData(panel->id, panel);
    if (dir == 4) {
        p2sdPlaySE(0x340004, 0x7F, 0x3F);
        pbAddRecord_flor(record, work->block);
        dds3PadVibrationSet(1, 0x6E, 0xC);
    } else {
        pbAddRecord_blk(record, work->block);
    }
    grp->value = 1;
    i = 0;
    if (grp->num != 0) {
        do {
            s16 x = grp->cellX + grp->blk[i].x;
            s16 y = grp->cellY + grp->blk[i].y;
            i++;
            D_0054F5C4->area.map.blk[y][x] &= 0x1800;
        } while (i < grp->num);
    }
    return panel;
}

pbProcHandle_t *pbStartRoundBlock(pbProcHandle_t *parent, pbGroupData_t *grp, s32 dir) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbRoundBlock_t *work;
    u32 i;
    panel->id = dds3StartProcess(D_00535F78, 0x3EE, 0, 0, roundBlock,
                                      (dds3ProcessFunc_t)deleteRoundBlock, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0xC);
    panel->data.round = work;
    memset(work, 0, 0xC);
    work->step = 0;
    work->index = dir;
    work->block = grp;
    dds3SetProcessData(panel->id, panel);
    pbAddRecord_blk(record, grp);
    grp->value = 1;
    i = 0;
    if (grp->num != 0) {
        do {
            s16 x = grp->cellX + grp->blk[i].x;
            s16 y = grp->cellY + grp->blk[i].y;
            i++;
            D_0054F5C4->area.map.blk[y][x] &= 0x1800;
        } while (i < grp->num);
    }
    return panel;
}

pbProcHandle_t *pbStartAutoRoundCamera(pbProcHandle_t *parent, s32 dir) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbRoundCamWork_t *work;
    dds3ProcessID_t *id;
    panel->id = dds3StartProcess(D_00535F90, 0x3EE, 0, 0, roundAutoCamera,
                                      (dds3ProcessFunc_t)deleteRoundAutoCamera, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(8);
    panel->data.roundCam = work;
    memset(work, 0, 8);
    work->dir = dir;
    work->angle = 0.0f;
    switch (pbGetKeyMode()) {
    case 0:
        if (dir == 1) {
            work->angle = 1.5707963f;
        } else {
            cameraAngle = 6.28318501f;
            work->angle = 4.71238852f;
        }
        break;
    case 1:
        if (dir == 1) {
            work->angle = 6.28318501f;
        } else {
            work->angle = 3.1415927f;
        }
        break;
    case 2:
        if (dir == 1) {
            work->angle = 4.71238852f;
        } else {
            work->angle = 1.5707963f;
        }
        break;
    case 3:
        if (dir == 1) {
            work->angle = 3.1415927f;
        } else {
            work->angle = 0.0f;
        }
        break;
    }
    dds3SetProcessData(panel->id, panel);
    dds3SetProcessFlag(panel->id, 0x10, 1);
    id = dds3SearchProcessName(D_00535F08);
    if (id != NULL) {
        dds3ResetProcessFlag(id, 0x10, 2);
    }
    return panel;
}

pbProcHandle_t *pbStartViewModeCamera(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbViewCamWork_t *work;
    panel->id = dds3StartProcess(D_00535FA8, 0x3EE, 0, 0, viewModeCamera,
                                      (dds3ProcessFunc_t)deleteViewModeCamera, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0x30);
    panel->data.viewCam = work;
    memset(work, 0, 0x30);
    work->scale = 1.0f;
    SDF_VU0_COPYVECTOR(&work->eye, &aViewEye);
    SDF_VU0_COPYVECTOR(&work->target, &aViewTarget);
    dds3SetProcessData(panel->id, panel);
    return panel;
}

void pbRoundCamera(s32 dir) {
    roundCamera = pbStartAutoRoundCamera(NULL, (dir != -1) ? -1 : 1);
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

pbProcHandle_t *pbStartClear(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->id = dds3StartProcess(D_00535FC8, 0x3EE, 1, 1, ClearProc,
                                      (dds3ProcessFunc_t)DeleteClearProc, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(4);
    work->value = 30;
    panel->data.value = work;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

dds3ProcessID_t *pbStartGiveupConfirm(void) {
    fclClearFlag(0);
    scr_msg = scrStartScript2(0x3EE, D_0054F5C4->giveupScript, 1);
    dds3SetProcessFlag(dds3SearchProcessName(D_00535EF8), 0x10, 2);
    return scr_msg;
}

void pbGetCameraParam(f32 *out) {
    out[0] = cameraAngle;
    out[1] = cameraRadius;
    out[2] = cameraDegree;
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

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F40);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F48);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F50);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F60);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F78);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F90);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FA8);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FC8);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);
extern void evtBitOn(s32 no);
extern s32 printf(const char *format, ...);
extern pbProcHandle_t *pbGetMoveChar(void);

void pbGotoNewStage(void)
{
    s32 number = GetStageNumber();

    if (scr_msg != NULL) {
        dds3KillProcess(scr_msg, 0);
        dds3ResetProcessFlag(dds3SearchProcessName(D_00535EF8), 0x10, 2);
        scr_msg = NULL;
    }
    if (number < 0) {
        evtBitOn(GetAllStageClearBit());
        printf("****** CONGRATULATIONS !! ******\n");
        if (pbGetMoveChar() != NULL) {
            pbGetMoveChar()->flags = 0x60000;
        }
    } else {
        pbLoadSystemData(number);
    }
}

void pbRoundStageNext(s16 *plevel, s16 *pstage) {
    pbLevel_t *lv = pbLevelTable[*plevel];
    *pstage = (u16)*pstage + 1;
    if ((lv->stage + *pstage)->number < 0) {
        *pstage = 0;
        *plevel = (u16)*plevel + 1;
        if (*plevel >= 3) {
            *plevel = 0;
        }
    }
}

void pbRoundStagePrev(s16 *plevel, s16 *pstage) {
    pbLevel_t *lv;
    s32 s = (u16)*pstage - 1;
    *pstage = s;
    if ((s16)s < 0) {
        s32 l = (u16)*plevel - 1;
        *plevel = l;
        if ((s16)l < 0) {
            *plevel = 2;
        }
        lv = pbLevelTable[*plevel];
        *pstage = 0;
        while ((lv->stage + *pstage + 1)->number >= 0) {
            *pstage = (u16)*pstage + 1;
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

void pbFreeGroupEntry(dds3ModelResrc_t *entry) {
    sdfMemFreeDirect(entry->effectMemHandle);
    sdfChipFreeDelayedDirect(entry);
}

void pbFreeGroupHandles(pbBlockDB_t *db) {
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbFreeGroupEntry((dds3ModelResrc_t *)db->entry[i]);
    }
    db->count = 0;
}

/* NON_MATCHING: 60 of 68 words, 272 bytes against retail's 288; the first test is not if-converted and x and y land in the wrong registers. */
#ifdef NON_MATCHING
u8 pbGetBlockAt(s16 x, s16 y, s16 grp) {
    u8 ret = 0;
    if ((x + 1 >= 20) || ((D_0054F5C4->area.map.blk[y][x + 1] & 0x7FF) != grp)) {
        ret = 4;
    }
    if ((x - 1 < 0) || ((D_0054F5C4->area.map.blk[y][x - 1] & 0x7FF) != grp)) {
        ret |= 8;
    }
    if ((y + 1 >= 20) || ((D_0054F5C4->area.map.blk[y + 1][x] & 0x7FF) != grp)) {
        ret |= 1;
    }
    if ((y - 1 < 0) || ((D_0054F5C4->area.map.blk[y - 1][x] & 0x7FF) != grp)) {
        ret |= 2;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", pbGetBlockAt);
#endif

INCLUDE_ASM(const s32, "pb/pbGame", pb_proc_block_DB_Process);

void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbBlockDB_t *db = panel->data.blockDB;
    sdfMatFreeDelayed(db->mat);
    pbFreeGroupHandles(db);
    sdfChipFreeDelayedDirect(db);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "pb/pbGame", pb_draw_map_cursor_Process);

void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_RODATA(const s32, "pb/pbGame", D_00536000);

