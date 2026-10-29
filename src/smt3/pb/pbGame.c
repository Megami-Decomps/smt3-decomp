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
    struct pbProcHandle_t *menuProc;
    struct pbProcHandle_t *helpProc;
    u8 reserved368[4];
    s32 drawCount;
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

typedef struct
{
    u16 kind;
    s16 x, y;
    u16 outline_num;
    u8 reserved08[8];
    f32 outlineVertices[8][4];
} pbBlockEntry_t;

typedef struct
{
    s32 value;
    u16 grpID;
    u16 num;
    u8 unk08[8];
    u16 kind;
    s16 cellX, cellY;
    u8 unk16[0x8A];
    sdfMemHandle_t *blockMem;
    /* Unrecovered bytes between the allocation handle and locked block pointer. */
    u8 blockStorageGap[0xC];
    pbBlockEntry_t *blk;
    union {
        sdfVector4_t rotation;
        struct { f32 rotationX, rot, rotationZ; u8 reservedC0[4]; };
    };
    f32 x, y, z;
    u8 reservedD0[4];
    u32 colors[8];
    u8 reservedF4[0xC];
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

extern s16 D_00551EC4, D_00551EC6;

extern sdfVector4_t D_0056CDD0[2];

extern void pbSetMapCursorFlag(pbProcHandle_t *p, s32 flag);

extern void pbSetGameCamera4(s16 x, s16 y);

extern void pbRotateXCamera(f32 angle);

extern void pbRotateYCamera(f32 angle);

extern u8 pbGetKeyAction(u8 press);

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

extern u8 pbGetKeyMode(void);

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

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern u32 pbAddRecord_flor(pbProcHandle_t *p, pbGroupData_t *grp);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);

extern u32 pbAddRecord_blk(pbProcHandle_t *p, pbGroupData_t *grp);

extern void deleteViewModeCamera(dds3ProcessID_t *id);

extern char D_00535E10[], D_00535EC8[], D_00535FC8[];

extern void updateMotion(void);

extern void pbFreeGroupHandles(pbBlockDB_t *db);

extern void pbFreeGroupEntry(pbGroupData_t *entry);

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

extern f32 aViewEye[4], aViewTarget[4];
/* Three rows initialized by pbLoadTextures; pbInitCameraParam is row one. */
extern f32 pbGameCameraParam[3][4];
/* Three target-offset rows; pbCameraVec2 names the second row. */
extern f32 pbCameraVec[3][4];

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

extern s32 pbFindProcHandle(s16 px, s16 py);
extern s8 goalHitCheck(s16 x, s16 y);
extern u32 pushCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup);
s32 roundCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup);

s32 hitCheck(s32 direction, pbGroupData_t **pGroup)
{
    s16 x, y;
    s16 targetX, targetY;
    u16 block;
    u32 kind;
    s32 status = 1;

    *pGroup = NULL;
    pbGetMyCharcterPos2(mychar[current_char], &x, &y);
    targetX = (s16)(x + pbKeyTable[direction][0]);
    targetY = (s16)(y + pbKeyTable[direction][1]);
    if (goalHitCheck(x, y) != 0) {
        return 6;
    }
    if ((u16)targetX >= 20 || targetY < 0 || targetY >= 20) {
        return 1;
    }
    if (pbFindProcHandle(targetX, targetY) == 1) {
        return 1;
    }
    block = D_0054F5C4->area.map.blk[targetY][targetX];
    if ((block & 0x800) != 0) {
        return 1;
    }

    kind = block >> 13;
    switch (kind) {
    case 0:
        status = 0;
        break;
    case 1:
        status = 1;
        break;
    case 2:
    case 3:
        status = pushCheck(direction, D_0054F5C4->area.map.blk[targetY][targetX] & 0x7FF, pGroup);
        break;
    case 4:
        status = 2;
        break;
    case 5:
        status = roundCheck(direction, D_0054F5C4->area.map.blk[targetY][targetX] & 0x7FF, pGroup);
        break;
    default:
        break;
    }
    return status;
}

extern void *pbSearchGroupDB(pbProcHandle_t *p, u32 grpID);
extern s32 pbFindProcHandle(s16 px, s16 py);

u32 pushCheck(s32 direction, u32 grpID, pbGroupData_t **pGroup)
{
    pbGroupData_t *group;
    s16 baseX, baseY;
    s32 x, y;
    u32 status = 3;
    u32 i;
    u16 block;

    group = pbSearchGroupDB(blockDB, grpID);
    *pGroup = group;
    baseX = (s16)(group->cellX + pbKeyTable[direction][0]);
    baseY = (s16)(group->cellY + pbKeyTable[direction][1]);
    i = 0;
    if (group->num != 0) {
        do {
            x = (s16)((*pGroup)->blk[i].x + baseX);
            y = (s16)((*pGroup)->blk[i].y + baseY);
            if ((u16)x >= 20 || y < 0 || y >= 20) {
                status = 2;
                break;
            }
            if (pbFindProcHandle(x, y) == 1) {
                status = 2;
                break;
            }
            block = D_0054F5C4->area.map.blk[y][x];
            if ((block >> 13) == 1) {
                status = 2;
            } else if ((block >> 13) >= 2) {
                if ((block & 0x7FF) != (*pGroup)->grpID) {
                    status = 2;
                }
            }
            i++;
        } while (i < (*pGroup)->num);
    }
    return status;
}

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

s32 gameKey1(void) {
    s32 ret = -1;
    if (dds3PadButtonState[0x22] < 0) {
        p2sdPlaySE(0x340009, 0x7F, 0x3F);
        if (pbGameKeyState == 0) {
            pbGetMyCharcterPos2(mychar[current_char], &D_00551EC4, &D_00551EC6);
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
        if ((u32)(D_00551EC4 + pbKeyTable[ret][0]) < 20) {
            D_00551EC4 += pbKeyTable[ret][0];
        }
        if ((u32)(D_00551EC6 + pbKeyTable[ret][1]) < 20) {
            D_00551EC6 += pbKeyTable[ret][1];
        }
        pbSetGameCamera4(D_00551EC4, D_00551EC6);
        flags = 0;
        if (D_00551EC6 < 0x13) {
            flags |= 1;
        }
        if (D_00551EC4 > 0) {
            flags |= 2;
        }
        if (D_00551EC6 > 0) {
            flags |= 4;
        }
        if (D_00551EC4 < 0x13) {
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

/* NON_MATCHING: 12 of 164 words differ at the exact 656-byte size.
 * The slot index and compact write cursor exchange saved registers. */
#ifdef NON_MATCHING
/* Initializer fields in the work allocation; the map starts at +0x1C. */
typedef struct
{
    s32 pcharNum;
    u8 pcharReg[4];
    s16 pcharPos[4][2];
    s16 exitX, exitY;
    pbMapBlock_t map;
} pbMapWorkInitView_t;

extern s32 D_00551EF4;
extern u32 pbMoveCharList;
extern u32 pbMapIsValid(void);
extern void dds3FadeIn2(s32 frame);
extern void pbSetViewRotation(f32 angle, f32 radius, f32 degree);
extern void pbSetExitPos(pbProcHandle_t *panel, s32 x, s32 y);
extern void pbSetExitDisp(pbProcHandle_t *panel, s8 sw);
extern pbProcHandle_t *pbInitMyCharacterDraw(pbProcHandle_t *parent, u32 color, u32 motion);
extern void pbSetPanelValue(pbProcHandle_t *panel, s8 value);
extern void pbGetCellY(pbProcHandle_t *panel, s16 cx, s16 cy);
extern void pbClearRecord(pbProcHandle_t *panel);
extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern s16 GetStageScriptLabel(void);
extern void pbInitGameCamera(void);

void pbInitGame(pbProcHandle_t *panel, dds3ProcessID_t *id)
{
    u32 i;
    pbProcHandle_t **write;
    s32 scriptLabel;
    pbCharacterFunc_t characterFunc;

    D_00551EF4 = 2;
    pbMoveCharList = 1;
    current_char = 0;
    pbSetViewRotation(cameraAngle, cameraRadius, cameraDegree);
    pbGameKeyState = 0;
    ((pbMapWorkInitView_t *)D_0054F5C4)->map = D_00327458;
    if (pbMapIsValid()) {
        return;
    }

    dds3FadeIn2(0x1E);
    pbSetExitPos((pbProcHandle_t *)goal, ((pbMapWorkInitView_t *)D_0054F5C4)->exitX, ((pbMapWorkInitView_t *)D_0054F5C4)->exitY);
    pbSetExitDisp((pbProcHandle_t *)goal, 1);
    write = mychar;
    for (i = 0; i < 4; i++) {
        if (mychar[i] != NULL) {
            mychar[i]->flags = 0x50000;
            mychar[i] = NULL;
        }
        if (((pbMapWorkInitView_t *)D_0054F5C4)->pcharReg[i] != 0) {
            *write = pbInitMyCharacterDraw(panel, (u32)i, 0);
            pbSetPanelValue(*write, 1);
            pbGetCellY(*write, ((pbMapWorkInitView_t *)D_0054F5C4)->pcharPos[i][0],
                       ((pbMapWorkInitView_t *)D_0054F5C4)->pcharPos[i][1]);
            write++;
        }
    }
    updateMotion();
    characterFunc = pbCharacterTable[D_0054F5C4->charIndex];
    if (characterFunc != NULL) {
        characterFunc();
    }
    pbClearRecord(record);
    scriptLabel = GetStageScriptLabel();
    if (scriptLabel >= 0) {
        fclClearFlag(0);
        fclSetWorkValue(0, scriptLabel);
        scr_msg = scrStartScript2(0x3EE, D_0054F5C4->giveupScript, 0);
        dds3SetProcessFlag(id, 0x10, 2);
    } else {
        pbMoveCharList = 0;
    }
}
#else
INCLUDE_ASM(const s32, "pb/pbGame", pbInitGame);
#endif

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

extern void pbMoveMyCharcter2(pbProcHandle_t *p, s16 x, s16 y);
extern void pbSetPanelValue(pbProcHandle_t *panel, s8 value);
extern pbProcHandle_t *pbStartClear(pbProcHandle_t *parent);
extern void pbClearRecord(pbProcHandle_t *panel);
extern void pbNextCharacter(void);
extern void func_001D2D00(pbProcHandle_t *panel, s32 arg);
extern s32 getCharacterNum(void);
extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);
extern pbProcHandle_t *pbGetMoveChar(void);
extern pbProcHandle_t *moveC;
extern u32 pbMoveCharList;
extern pbMapWork_t *D_0054F5C4;
extern pbProcHandle_t *record;

void deleteMoveChar(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;
    pbMoveCharWork_t *work = panel->data.moveChar;
    s16 position[2];
    s16 deltaX = (s16)((f32)pbKeyTable[work->dir][0] * work->speed);
    s16 deltaY = (s16)((f32)pbKeyTable[work->dir][1] * work->speed);

    pbMoveMyCharcter2(mychar[current_char], deltaX, deltaY);
    pbGetMyCharcterPos2(mychar[current_char], &position[0], &position[1]);
    if (goalHitCheck(position[0], position[1])) {
        p2sdPlaySE(0x34000A, 0x7F, 0x3F);
        if ((u32)getCharacterNum() < 2) {
            if (D_0054F5C4->menuProc != NULL) {
                dds3KillProcess(D_0054F5C4->menuProc->id, 0);
            }
            pbMoveCharList = 1;
            func_001D2D00(mychar[current_char], 0);
            pbStartClear(pbGetMoveChar());
            pbClearRecord(record);
        } else {
            pbSetPanelValue(mychar[current_char], 0);
            pbNextCharacter();
        }
    }
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    moveC = NULL;
}

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
    SDF_VU0_COPYVECTOR(&aViewEye, &pbGameCameraParam[0]);
    SDF_VU0_COPYVECTOR(&aViewTarget, &pbCameraVec[0]);
}

extern void pbGetMyCharcterPos(pbProcHandle_t *character, sdfVector4_t *position);

void pbSetGameCamera1(void)
{
    f32 eye[4];
    f32 target[4];
    u32 i;

    if (mychar[current_char] != NULL) {
        memset(&eye, 0, sizeof(eye));
        memset(&target, 0, sizeof(target));
        pbGetMyCharcterPos(mychar[current_char], (sdfVector4_t *)target);
        for (i = 0; i < 4; i++) {
            target[i] += pbCameraVec[1][i];
        }
        for (i = 0; i < 4; i++) {
            eye[i] = aViewEye[i] - aViewTarget[i] + target[i];
        }
        SDF_VU0_COPYVECTOR(&aViewEye, &eye);
        SDF_VU0_COPYVECTOR(&aViewTarget, &target);
    }
}

void pbSetGameCamera2(void)
{
    f32 eye[4];
    f32 target[4];
    u32 i;

    if (mychar[current_char] != NULL) {
        memset(&eye, 0, sizeof(eye));
        memset(&target, 0, sizeof(target));
        pbGetMyCharcterPos(mychar[current_char], (sdfVector4_t *)target);
        for (i = 0; i < 4; i++) {
            target[i] += pbCameraVec[1][i];
        }
        for (i = 0; i < 4; i++) {
            eye[i] = aViewEye[i] - aViewTarget[i] + target[i];
        }
        SDF_VU0_COPYVECTOR(&aViewEye, &eye);
        SDF_VU0_COPYVECTOR(&aViewTarget, &target);
    }
}

extern s32 pbGetMyCharcterRot(pbProcHandle_t *character);

void pbSetGameCamera3(void)
{
    f32 eye[4];
    f32 target[4];
    s32 direction;

    if (mychar[current_char] != NULL) {
        memset(&eye, 0, sizeof(eye));
        memset(&target, 0, sizeof(target));
        pbGetMyCharcterPos(mychar[current_char], (sdfVector4_t *)target);
        SDF_VU0_COPYVECTOR(&eye, &target);
        direction = pbGetMyCharcterRot(mychar[current_char]);
        eye[0] += (f32)pbKeyTable[direction][0] * 160.0f;
        eye[2] += (f32)pbKeyTable[direction][1] * 160.0f;
        eye[1] -= 60.0f;
        target[1] -= 40.0f;
        SDF_VU0_COPYVECTOR(&aViewEye, &eye);
        SDF_VU0_COPYVECTOR(&aViewTarget, &target);
    }
}

void pbSetGameCamera4(s16 x, s16 y)
{
    f32 eye[4];
    f32 target[4];
    u32 i;

    memset(&eye, 0, sizeof(eye));
    memset(&target, 0, sizeof(target));
    target[0] = x * 40 - 380;
    target[2] = y * 40 - 380;
    for (i = 0; i < 4; i++) {
        target[i] += pbCameraVec[1][i];
    }
    for (i = 0; i < 4; i++) {
        eye[i] = aViewEye[i] - aViewTarget[i] + target[i];
    }
    SDF_VU0_COPYVECTOR(&aViewEye, &eye);
    SDF_VU0_COPYVECTOR(&aViewTarget, &target);
}

void pbInitGameCamera(void) {
    u32 type = D_0054F5C4->charIndex;
    switch (type) {
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
        /* Reset the local selector; qualified late deletion:
         * docs/levers/callee-visibility.md#camera-selector-reset-after-the-final-call. */
        type = 0;
        break;
    }
}


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

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);
extern void evtBitOn(s32 no);
extern s32 printf(const char *format, ...);
extern pbProcHandle_t *pbGetMoveChar(void);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F40);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F48);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F50);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F60);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F78);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535F90);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FA8);

INCLUDE_RODATA(const s32, "pb/pbGame", D_00535FC8);

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

extern void pbRebuildOutline(pbGroupData_t *group);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);

void pbAddGroupEntry(pbBlockDB_t *db)
{
    pbGroupData_t *group;
    /* Qualified allocation traces: the scan and outline loops share one index.
     * See docs/levers/cross-block-locals.md. */
    u32 index, column;
    u32 kind;
    u32 id;
    u32 i;
    u16 cell;

    for (index = 0; index < 20; index++) {
        for (column = 0; column < 20; column++) {
            cell = D_0054F5C4->area.map.blk[index][column];
            kind = cell >> 13;
            if (kind < 5) {
                if (kind >= 3) {
                    id = cell & 0x7FF;
                    if (pbFindGroupEntry((pbGroupDB_t *)db, id) == NULL) {
                        db->entry[db->count] = sdfChipAllocDirect(sizeof(pbGroupData_t));
                        memset(db->entry[db->count], 0, sizeof(pbGroupData_t));
                        group = (pbGroupData_t *)db->entry[db->count];
                        if (kind == 3) {
                            kind = 2;
                            D_0054F5C4->area.map.blk[index][column] =
                                (D_0054F5C4->area.map.blk[index][column] & 0x1FFF) | 0x4000;
                        }
                        group->grpID = id;
                        group->z = (s16)index * 40 - 380;
                        group->y = -20.0f;
                        group->kind = kind;
                        group->cellX = column;
                        group->cellY = index;
                        group->x = (s16)column * 40 - 380;
                        if (kind == 2) {
                            for (i = 0; i < 8; i++) group->colors[i] = 0x80D0D0D0;
                        } else {
                            for (i = 0; i < 8; i++) group->colors[i] = 0x80D0D0D0;
                        }
                        db->count++;
                    }
                }
            }
        }
    }
    for (index = 0; index < 20; index++) {
        for (column = 0; column < 20; column++) {
            s32 cellKind;
            cell = D_0054F5C4->area.map.blk[index][column];
            cellKind = cell >> 13;
            if (cellKind < 6) {
                if (cellKind >= 2) {
                    group = pbFindGroupEntry((pbGroupDB_t *)db, cell & 0x7FF);
                    group->num++;
                }
            }
        }
    }
    for (index = 0; index < 20; index++) {
        for (column = 0; column < 20; column++) {
            u32 blockKind;
            u16 blockCell = D_0054F5C4->area.map.blk[index][column];
            blockKind = blockCell >> 13;
            if (blockKind < 6) {
                if (blockKind >= 2) {
                    group = pbFindGroupEntry((pbGroupDB_t *)db, blockCell & 0x7FF);
                    if (group->blk == NULL) {
                        group->blockMem = sdfMemAllocDirect(group->num * sizeof(pbBlockEntry_t));
                        group->blk = sdfMemLock(group->blockMem);
                        memset(group->blk, 0, group->num * sizeof(pbBlockEntry_t));
                    }
                    for (i = 0; group->blk[i].kind != 0; i++) {}
                    group->blk[i].kind = blockKind;
                    group->blk[i].x = column - group->cellX;
                    group->blk[i].y = index - group->cellY;
                }
            }
        }
    }
    for (index = 0; index < db->count; index++) {
        pbRebuildOutline((pbGroupData_t *)db->entry[index]);
    }
}


void pbFreeGroupEntry(pbGroupData_t *entry) {
    sdfMemFreeDirect(entry->blockMem);
    sdfChipFreeDelayedDirect(entry);
}

void pbFreeGroupHandles(pbBlockDB_t *db) {
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbFreeGroupEntry((pbGroupData_t *)db->entry[i]);
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

typedef struct { sdfVector4_t v[4]; } pbMatrix_t;
extern void pbAddGroupEntry(pbBlockDB_t *db);
extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *material, s32 (*allocator)(s32));
extern void pbSetPrimMatrix(sdfVector4_t *rotation, f32 *position, void *tag);
extern u8 pbFindBlockSlot(s16 x, s16 y, s32 mode);
extern s32 pbDrawMapPolyTransformed(sdfVector4_t *position, u8 slot, u32 kind,
                                   sdfDrawTag_t *tag);
extern void SetRotateMatrixXYZ(const sdfVector3_t *rotation);
extern void StoreMatrix(void *matrix);
extern void LoadMatrix(const void *matrix);
extern sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors,
                                               s32 count, u32 flags);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern sdfGrOt_t sdfGrOtTable[85];
extern f64 fptodp(f32 value);
extern f32 dptofp(f64 value);

s32 pb_proc_block_DB_Process(dds3ProcessID_t *id)
{
    sdfVector4_t position;
    pbMatrix_t matrix;
    pbProcHandle_t *panel = id->workdata;
    pbBlockDB_t *db = panel->data.blockDB;
    sdfDrawTag_t *mapTag;
    sdfDrawTag_t *outlineTag;
    sdfGrOt_t *ot;
    void (*registerTag)(sdfGrOt_t *, sdfDrawTag_t *);
    pbGroupData_t *group;
    u32 i;
    u32 j;
    u8 slot;
    s32 drawn;

    memset(&position, 0, sizeof(position));
    switch (panel->state) {
    case 1:
        pbFreeGroupHandles(db);
        pbAddGroupEntry(db);
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    }

    mapTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(mapTag);
    outlineTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(outlineTag);
    sdf3DPrimLinkMat(mapTag, db->mat, NULL);
    for (i = 0; i < db->count; i++) {
        group = (pbGroupData_t *)db->entry[i];
        pbSetPrimMatrix(&group->rotation, &group->x, mapTag);
        memset(&position, 0, sizeof(position));
        for (j = 0; j < group->num; j++) {
            slot = 31;
            if (group->value == 0) {
                slot = pbFindBlockSlot(group->cellX + group->blk[j].x,
                                       group->cellY + group->blk[j].y, 1);
            }
            position.x = group->blk[j].x * 40;
            position.z = group->blk[j].y * 40;
            drawn = pbDrawMapPolyTransformed(&position, slot, group->blk[j].kind,
                                             mapTag);
            D_0054F5C4->drawCount += drawn;
            SetRotateMatrixXYZ((const sdfVector3_t *)&group->rotation);
            StoreMatrix(&matrix);
            matrix.v[3].x = group->x;
            matrix.v[3].y = group->y * 2.0 - 2.0;
            matrix.v[3].z = group->z;
            LoadMatrix(&matrix);
            if (group->blk[j].outline_num != 0) {
                sdfDrawTagLink(outlineTag, (u32)sdfDrawRotatedModel(group->blk[j].outlineVertices,
                    group->colors, group->blk[j].outline_num, 0x80));
            }
        }
    }
    /* Reuse the callback local: qualified allocation traces keep its four
     * references in one global pseudo at $2. Direct field calls create two
     * overlapping local quantities and place the second target in $3. */
    ot = &sdfGrOtTable[39];
    registerTag = ot->regFunc;
    registerTag(ot, mapTag);
    ot = &sdfGrOtTable[40];
    registerTag = ot->regFunc;
    registerTag(ot, outlineTag);
    return 0;
}


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

