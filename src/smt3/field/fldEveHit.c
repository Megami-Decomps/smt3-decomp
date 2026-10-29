#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldTriggerSphere_t fldTriggerSphere_t;

typedef struct fldTriggerPlane_t fldTriggerPlane_t;

typedef struct fldTriggerBox_t fldTriggerBox_t;

typedef union fldTriggerShape_t fldTriggerShape_t;

typedef struct fldTaskInfo_t fldTaskInfo_t;

typedef struct fldRoomState_t fldRoomState_t;

typedef struct fldInfHit_t fldInfHit_t;

typedef struct fldInfPack_t fldInfPack_t;

typedef struct fldInfView_t fldInfView_t;

typedef struct fldInfExtraAction_t fldInfExtraAction_t;

typedef struct fldInfFlagSelector_t fldInfFlagSelector_t;

typedef struct fldInfMessageRow_t fldInfMessageRow_t;

typedef struct fldInfSet_t fldInfSet_t;

typedef struct fldInfTable_t fldInfTable_t;

enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

struct fldTriggerSphere_t {
    u32 kind;
    f32 radius;
};

struct fldTriggerPlane_t {
    u32 kind;
    f32 width;
    f32 height;
};

struct fldTriggerBox_t {
    u32 kind;
    f32 size[3];
};

union fldTriggerShape_t {
    u32 kind;
    fldTriggerSphere_t sphere;
    fldTriggerPlane_t plane;
    fldTriggerBox_t box;
};

struct fldTaskInfo_t {
    s32 unk0;
    s32 slot;
    u32 unk08;
    fldTriggerShape_t *shape;
};

struct fldRoomState_t {
    f32 corner[FIELD_ROOM_CORNER_COUNT][4];
    f32 center[4];
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];
    f32 limit[FIELD_ROOM_PLANE_COUNT];
    s32 worldObjectKey;
    f32 unk10C;
    s32 unk110;
    f32 unk114;
    s32 unk118;
    s32 unk11C;
    s16 unk120;
    s16 roomId;
    s16 unk124;
    s16 mode;
    s16 entryIndex;
    s16 axisMode;
    s32 shapeKind;
};

/* The event-range box block at fldRoomState_t + 0x120, reached through its own
   pointer so the offset folds into the symbol address. */
typedef struct {
    s16 unk00;
    s16 takaraIndex;
    s16 unk04;
    s16 hitKind;
    s16 npcIndex;
    s16 unk0A;
    s32 unk0C;
} fldEveRngBox_t;

/* The plane block of fldRoomState_t, reached through its own pointer so the
   0x90 offset folds into the symbol address. */
typedef struct {
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];
    f32 limit[FIELD_ROOM_PLANE_COUNT];
} fldRoomPlanes_t;

/* The same block based at plane[1], the first of the four side planes; the
   room's own plane[0] and its distance sit before limit[]. */
typedef struct {
    f32 plane[FIELD_ROOM_PLANE_COUNT - 1][4];
    f32 firstLimit;
    f32 limit[FIELD_ROOM_PLANE_COUNT - 1];
} fldRoomSidePlanes_t;

struct fldInfHit_t {
    s8 area;
    char eventName[15];
};

struct fldInfPack_t {
    s32 setIndex;
    fldInfHit_t hits[5];
};

struct fldInfView_t {
    s32 player;
    s32 motion;
    char positionName[12];
    char cameraName[12];
};

struct fldInfExtraAction_t {
    s32 action;
    s32 parameter0;
    s32 parameter1;
    s32 parameter2;
    s32 parameter3;
};

struct fldInfFlagSelector_t {
    s16 flag;
    s8 offTarget;
    s8 onTarget;
};

struct fldInfMessageRow_t {
    s16 kind;
    s16 message;
    s8 targets[4];
    s16 flags[2];
    u8 view;
    u8 extraAction;
};

struct fldInfSet_t {
    union {
        s16 packed;
        struct {
            u8 kind;
            u8 area;
        } bytes;
    } kindArea;
    s8 action;
    s8 eventHit;
    char eventName[12];
    fldInfFlagSelector_t flags[4];
    fldInfMessageRow_t messages[20];
};

struct fldInfTable_t {
    fldInfPack_t packs[8];
    fldInfView_t views[40];
    fldInfExtraAction_t extraActions[40];
    fldInfSet_t sets[40];
};

extern void *memset(void *, int, unsigned int);

dds3Basic_t *dds3GetActiveScene(void);

u32 dds3AllocObjectId(void);

void *dds3EventBasicGetResource(dds3Basic_t *pBasic);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

dds3Basic_t *dds3GetControlScene(void);

void dds3SceneBasicEndScript(void *scene, const char *procname);

/* Common prefix of the event payloads registered by fldEveHit_AddEveData.
 * The scene lookup consumes the object ID, not the remaining payload. */
typedef struct {
    u32 objectId;
} fldEventDataHeader_t;

extern fldEventDataHeader_t *fldEveHitDataTable[];

extern u32 gEvnRngPid[];

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern fldFileResource_t *gEvnRng[];

extern s32 gFldEveRngIdx;

extern s32 gFldEveRngIdx_T;

extern s32 fldEveHitState;

extern u32 gEveNum;

s32 dds3ExistProcess(dds3ProcessID_t *id);

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

s32 dds3SceneBasicStartScript(dds3Basic_t *scene, const char *procname);

extern s8 fldDoorExitMode[];

extern u8 *fldDoor_Find3(void);

extern u8 fldDoorExitFlag;

extern fldRoomState_t fldEveHitTable[];

extern s32 strcmp(const char *left, const char *right);

extern fldInfTable_t fldInfTable;

extern s32 gFldInfIdx;

extern const char D_002FF3E0[];

s32 evtBitCheck(s32 no);

extern s32 gFldKyouseiTalk;

/* gFldEveHitOnOffTbl stride, 20 bytes. */
typedef struct {
    s16 area;
    s16 floor;
    s16 flag;
    char name[14];
} fldEveHitSwitch_t;

extern fldEveHitSwitch_t gFldEveHitOnOffTbl[32];

void fldDrawPos(const f32 *pos);

extern void fldStatusSet(void);

f32 fldVecDot(sdfVector4_t *a, sdfVector4_t *b);

extern s32 datAddHp(datUnitWork_t *work, s32 hp);

extern s32 datAddMp(datUnitWork_t *work, s32 mp);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

void fldKaifukuTest(void) {
    s32 i;
    s32 ofs = 0;

    for (i = 0; i < 16; i++) {
        datAddHp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 9999);
        datAddMp((datUnitWork_t *)((u8 *)DDS3_GBWK + ofs + 0xA54), 9999);
        *(u16 *)((u8 *)ofs + (u32)DDS3_GBWK + 0xA62) = 0;
        ofs += 0xEC;
    }
    p2sdPlaySE(0x10, 0x7F, 0x3F);
}

#ifdef NON_MATCHING
extern fldDanteGunState_t fldRunDanteGunFlag;
extern s32 datCheckDying(datUnitWork_t *work);

void fldStatusSet(void)
{
    s32 i;

    fldRunDanteGunFlag.word50 = 0;
    for (i = 0; i < 16; i++) {
        fldAnahoriAkuma_t *unit = &DDS3_GBWK->akuma[i];
        volatile u16 *badstatus = &unit->badstatus;

        if ((unit->flag & 2) && unit->hp != 0) {
            if (*badstatus & 0x40) {
                fldRunDanteGunFlag.word50 |= 1;
            }
            if (*badstatus & 0x100) {
                fldRunDanteGunFlag.word50 |= 4;
            }
            if (*badstatus & 0x20) {
                fldRunDanteGunFlag.word50 |= 8;
            }
            if (*badstatus & 0x8000) {
                fldRunDanteGunFlag.word50 |= 0x10;
            }
        }
    }
    if (datCheckDying((datUnitWork_t *)&DDS3_GBWK->akuma[0].flag)) {
        fldRunDanteGunFlag.word50 |= 2;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldEveHit", fldStatusSet);
#endif

extern s32 fldEveRngPrevIdx;

extern s32 fldEveRngHitIdx;

extern s32 fldEveHitCount;

void fldClearDoorEvents(void);

void fldClearEveRngTable(void) {
    s32 i;
    s32 j;

    fldEveRngCount = 0;
    gEveNum = 0;
    fldEveHitState = 0;
    for (i = 0; i < 0x40; i++) {
        for (j = 0; j < 8; j++) {
            fldEveHitTable[i].corner[j][0] = 0.0f;
            fldEveHitTable[i].corner[j][1] = 0.0f;
            fldEveHitTable[i].corner[j][2] = 0.0f;
            fldEveHitTable[i].corner[j][3] = 1.0f;
        }
        fldEveHitTable[i].center[0] = 0.0f;
        fldEveHitTable[i].center[1] = 0.0f;
        fldEveHitTable[i].center[2] = 0.0f;
        fldEveHitTable[i].center[3] = 1.0f;
        for (j = 0; j < 6; j++) {
            fldEveHitTable[i].plane[j][0] = 0.0f;
            fldEveHitTable[i].plane[j][1] = 0.0f;
            fldEveHitTable[i].plane[j][2] = 0.0f;
            fldEveHitTable[i].plane[j][3] = 1.0f;
            fldEveHitTable[i].limit[j] = 0.0f;
        }
        gEvnRngPid[i] = 0;
        fldEveHitTable[i].unk10C = 0.0f;
        fldEveHitTable[i].unk110 = 0;
        fldEveHitTable[i].worldObjectKey = 0;
        fldEveHitTable[i].unk114 = 0.0f;
        fldEveHitTable[i].unk118 = 0;
        fldEveHitTable[i].unk11C = 0;
        fldEveHitTable[i].unk120 = 0;
        fldEveHitTable[i].roomId = -1;
        fldEveHitTable[i].unk124 = -1;
        fldEveHitTable[i].mode = 0;
        fldEveHitTable[i].entryIndex = -1;
        fldEveHitTable[i].axisMode = 0;
        fldEveHitTable[i].shapeKind = 0;
    }
    fldEveHitCount = 0;
    fldEveRngPrevIdx = -1;
    fldEveRngHitIdx = -1;
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    fldClearDoorEvents();
}

void fldEveHit_End(void) {
    s32 slotIndex;
    dds3Basic_t *scene;
    char *scriptName;
    fldTaskInfo_t *taskInfo;

    fldEveHitState = 1;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        gEvnRngPid[slotIndex] = 0;
    }
    gFldEveRngIdx = -1;
    gFldEveRngIdx_N = -1;
    gFldEveRngIdx_T = -1;
    scene = dds3GetControlScene();
    if (scene != NULL) {
        for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
            taskInfo = (fldTaskInfo_t *)gEvnRng[slotIndex]->data;
            if (taskInfo->slot >= 0) {
                scriptName = dds3EventBasicGetResource((dds3Basic_t *)dds3SearchSceneBasic(
                    scene, fldEveHitDataTable[taskInfo->slot]->objectId, 0xD));
                if (scrSearchScriptName(scriptName) != NULL) {
                    dds3SceneBasicEndScript(dds3GetActiveScene(), scriptName);
                }
            }
        }
    }
}

/* The event-hit source records at D_002FCFA0: a sphere, the put record that
   names it, the transform dds3AddPointBasic consumes, and the two resource
   headers the hit and the event are registered under. */
typedef struct {
    u32 Type;
    f32 r;
} fldResPutEveBall_t;

typedef struct {
    u32 type;
    s32 eveIndex;
    s32 visible;
    fldResPutEveBall_t *ball;
} fldResPut_t;

typedef struct {
    u32 resourceID;
    u32 resourceType;
    char *resourceName;
    u32 flag;
    fldResourcePoint_t *sisei;
    void *areaName;
    void *resourceLink;
    char *blockName;
    fldResPut_t *put;
} fldResHead_t;

typedef struct {
    fldResPutEveBall_t ball;  /* 0x00 */
    fldResPut_t put;          /* 0x08 */
    fldResourcePoint_t sisei; /* 0x18 */
    fldResHead_t hit;         /* 0x48 */
    fldResHead_t event;       /* 0x6C */
} fldEveHitSource_t;

extern fldEveHitSource_t D_002FCFA0[];

extern char D_002FCD70[];

extern char D_002FCD80[];

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *pData, char *pName);

extern dds3Basic_t *dds3AddEventBasic(s32 serialID, void *pData, char *pName);

extern s32 fldEveHit_AddEveHit(fldResHead_t *pEvnRng, dds3Basic_t *pBasic);

u32 fldEveHit_AddEveData(fldEventDataHeader_t *pEve, dds3Basic_t *pBasic) {
    u32 index = gEveNum;

    fldEveHitDataTable[index] = pEve;
    gEveNum = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "field/fldEveHit", fldEveHit_AddEveHit);

extern void ToMatrix(void);

s32 fldEveHit_ChkDir(dds3Basic_t *unit, dds3Basic_t *put)
{
    f32 dir[4];
    f32 pos[4];
    s32 i;
    fldTaskInfo_t *info;
    f32 *target;
    f32 length;
    f32 dot;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    for (i = 0; i < fldEveRngCount; i++) {
        if (fldEveHitTable[i].worldObjectKey != put->serialID) {
            continue;
        }
        info = (fldTaskInfo_t *)gEvnRng[i]->data;
        switch (info->shape->kind) {
        case 0:
            target = (f32 *)put->workdata;
            pos[0] = target[0];
            pos[1] = target[1];
            pos[2] = target[2];
            SDF_VU0_LOADVECTOR(&unit->poseset->rot);
            ToMatrix();
            SDF_VU0_STORE_ROW2(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR(-1.0f);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(pos);
            SDF_VU0_LOADVECTOR_B(&unit->poseset->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_GETLENGTH(length);
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_INNERPRODUCT(dot);
            if (dot * 180.0f / 3.14f < 0.0f) {
                return 0;
            }
            if (200.0f < length) {
                return 0;
            }
            return 1;
        case 1:
            return fldEveHitTable[i].shapeKind == info->shape->kind;
        case 2:
            SDF_VU0_LOADVECTOR(&unit->poseset->rot);
            ToMatrix();
            SDF_VU0_STORE_ROW2(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(dir);
            if (fldVecDot((sdfVector4_t *)dir, (sdfVector4_t *)fldEveHitTable[i].center) < 0.01f) {
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 32 of 153 words differ (first at +0x8). */
#ifdef NON_MATCHING
s32 fldEveHit_ChkDir2(dds3Basic_t *unit, dds3Basic_t *put)
{
    f32 dir[4];
    f32 pos[4];
    s32 i;
    fldTaskInfo_t *info;
    f32 *probe;
    f32 length;
    f32 dot;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    for (i = 0; i < fldEveRngCount; i++) {
        if (fldEveHitTable[i].worldObjectKey != put->serialID) {
            continue;
        }
        info = (fldTaskInfo_t *)gEvnRng[i]->data;
        switch (info->shape->kind) {
        case 0:
            probe = pos;
            *(u128 *)pos = *(u128 *)put->workdata;
            SDF_VU0_LOADVECTOR(&unit->poseset->rot);
            ToMatrix();
            SDF_VU0_STORE_ROW2(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR(-1.0f);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR_RWADDR(probe);
            SDF_VU0_LOADVECTOR_B(&unit->poseset->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_GETLENGTH(length);
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_INNERPRODUCT(dot);
            if (dot * 180.0f / 3.14f < 0.0f) {
                return 0;
            }
            return 1;
        case 1:
            return fldEveHitTable[i].shapeKind == info->shape->kind;
        case 2:
            SDF_VU0_LOADVECTOR(&unit->poseset->rot);
            ToMatrix();
            SDF_VU0_STORE_ROW2(dir);
            dir[1] = 0.0f;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(dir);
            if (fldVecDot((sdfVector4_t *)dir, (sdfVector4_t *)fldEveHitTable[i].center) < 0.0f) {
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldEveHit", fldEveHit_ChkDir2);
#endif

s32 fldEveHitTestRect(f32 *pos, s32 idx) {
    f32 probe[3];
    f32 planar[2];
    fldRoomSidePlanes_t *sides;
    s32 i;
    f32 zero;

    if (fldEveHitTable[idx].axisMode == 0) {
        probe[0] = 0.0f;
        probe[1] = pos[1];
        probe[2] = pos[2];
        planar[0] = pos[1];
        planar[1] = pos[2];
    } else {
        probe[0] = pos[0];
        probe[1] = pos[1];
        probe[2] = 0.0f;
        planar[0] = pos[0];
        planar[1] = pos[1];
    }
    zero = 0.0f;
    sides = (fldRoomSidePlanes_t *)fldEveHitTable[idx].plane[1];
    for (i = 0; i < FIELD_ROOM_PLANE_COUNT - 2; i++) {
        if (fldVecDot((sdfVector4_t *)probe, (sdfVector4_t *)sides->plane[i]) -
                sides->limit[i] < zero) {
            return -1;
        }
    }
    return 0;
}

s32 fldEveHitTestPlanes(sdfVector4_t *pos, s32 idx) {
    fldRoomPlanes_t *planes = (fldRoomPlanes_t *)fldEveHitTable[idx].plane;
    s32 i;
    f32 zero = 0.0f;

    for (i = 0; i < FIELD_ROOM_PLANE_COUNT; i++) {
        if (zero < fldVecDot(pos, (sdfVector4_t *)planes->plane[i]) - planes->limit[i]) {
            return 0;
        }
    }
    return 1;
}

s32 fldEveHit_ScriptStart(char *pScriptName, s32 idx) {
    s32 pid;
    s32 mode;

    if (scrSearchScriptName(pScriptName) == NULL) {
        pid = dds3SceneBasicStartScript(dds3GetActiveScene(), pScriptName);
        gEvnRngPid[idx] = pid;
        gFldEveRngIdx = idx;
        mode = fldEveHitTable[idx].mode;
        gFldEveRngIdx_N = (mode == 1) ? idx : -1;
        gFldEveRngIdx_T = (mode == 2) ? idx : -1;
        return pid;
    }
    return -1;
}

s32 fldEveHit_ScriptEnd(char *pScriptName, u32 idx) {
    gEvnRngPid[idx] = 0;
    if (scrSearchScriptName(pScriptName) != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), pScriptName);
        return 0;
    }
    return -1;
}

u32 fldEveHit_ScriptEnd2(u32 idx) {
    u32 *task = &gEvnRngPid[idx];

    if (dds3ExistProcess((dds3ProcessID_t *)*task) != 0) {
        dds3KillProcess((dds3ProcessID_t *)*task, 0);
    }
    *task = 0;
    return 0;
}

s32 fldEveHit_ChkEveHitAfter(void) {
    u8 *object;
    u32 state = fldDoorExitFlag;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && fldDoorExitMode[0] != 0) {
        dds3SceneBasicEndScript(dds3GetActiveScene(), (const char *)fldDoorExitMode);
    }
    fldDoorExitMode[0] = 0;
    fldDoorExitFlag = 0;
    object = fldDoor_Find3();
    if (scrSearchScriptName((char *)object) == 0) {
        dds3SceneBasicStartScript(dds3GetActiveScene(), (const char *)object);
    }
    return 1;
}

s32 fldEveHit_ChkOn(const char *evehitname) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (gFldEveHitOnOffTbl[i].area == fldGlobalBase.area &&
            gFldEveHitOnOffTbl[i].floor == fldGlobalBase.floor + 1 &&
            strcmp(gFldEveHitOnOffTbl[i].name, evehitname) == 0) {
            return evtBitCheck(gFldEveHitOnOffTbl[i].flag) != 0;
        }
    }
    return 1;
}

#ifdef NON_MATCHING
extern fldDanteGunState_t fldRunDanteGunFlag;
extern s32 fldEncountTestFlag(u32 flag);
extern s32 fldGbChkNpc(s32 area, u32 floor, s32 id);
extern void rsetOtOfs(u32 offset);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32 mode);
extern void ooyTestLine3D(u32 color1, u32 color2, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2);
extern s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right);
extern void fldDbgPrint2D(s32 x, s32 y, const char *str);
extern s32 sprintf(char *buffer, const char *format, ...);
extern const char *fldCheckNpcTalk(const char *name);
extern const char *fldCheckNpcTalk2(const char *name);
extern const char *fldInf_FindTypeEveNpc(const char *name);
extern u8 *fldDoor_Find2(const char *name);
extern u8 *fldFindEventEntry(const char *name);
extern s32 gFldInf_HitIdx, gFldInf_WpExit;


extern void fldPurgeEveRngProcesses(void);

void fldEveHit_ChkEveHit(dds3Basic_t *player)
{
    sdfVector4_t scratch __attribute__((aligned(16)));
    sdfVector4_t current __attribute__((aligned(16)));
    sdfVector4_t previous __attribute__((aligned(16)));
    sdfVector4_t corners[8] __attribute__((aligned(16)));
    dds3PoseSet_t playerPose __attribute__((aligned(16)));
    dds3PoseSet_t eventPose __attribute__((aligned(16)));
    char message[64];
    dds3Basic_t *scene;
    s32 talkIndex = -1, doorIndex = -1;
    s32 i, j;

    memset(&scratch, 0, sizeof(scratch));
    scratch.w = 1.0f;
    memset(&current, 0, sizeof(current));
    current.w = 1.0f;
    memset(&previous, 0, sizeof(previous));
    previous.w = 1.0f;
    if (fldEveHitState != 0 || fldEncountTestFlag(0x40) == 0) {
        return;
    }
    fldPurgeEveRngProcesses();
    if (fldRunDanteGunFlag.word18 == 1) {
        rsetOtOfs(0x51);
        rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
        rsetAlpha(0);
    }
    scene = dds3GetControlScene();
    for (i = 0; i < fldEveRngCount; i++) {
        fldRoomState_t *room = &fldEveHitTable[i];
        fldFileResource_t *event;
        fldTaskInfo_t *info;
        const f32 *position;
        char *script;
        u32 kind;
        s32 crossing;
        f32 nowDistance, oldDistance;

        if (room->unk124 > 0 && fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1, room->unk124) != 0) {
            continue;
        }
        if (fldEveHit_ChkOn(gEvnRng[i]->name) == 0) {
            continue;
        }
        event = gEvnRng[i];
        info = event->data;
        kind = info->shape->kind;
        if (kind == 0) {
            position = event->transform;
            playerPose.r = 30.0f;
            playerPose.pos.x = player->poseset->pos.x;
            playerPose.pos.y = player->poseset->pos.y;
            playerPose.pos.z = player->poseset->pos.z;
            eventPose.r = info->shape->sphere.radius;
            eventPose.pos.x = position[0];
            eventPose.pos.y = position[1];
            eventPose.pos.z = position[2];
            if (dds3ComparePosPoseSet(&playerPose, &eventPose) == 1) {
                if (fldRunDanteGunFlag.word20 == 1) {
                    fldDbgPrint2D(0x40, 0x140, gEvnRng[i]->name);
                    if (fldRunDanteGunFlag.word20 == 1 && room->roomId != -1) {
                        fldTakaraTbl_t *treasure = &fldTakaraTable[room->roomId];
                        sprintf(message, "Takara:%d [%d,%d,%d,%d,%d]", room->roomId,
                                treasure->type, treasure->id, treasure->num, treasure->trap, treasure->param);
                        fldDbgPrint2D(0x40, 0x1A0, message);
                    }
                }
                if (fldCheckNpcTalk2(gEvnRng[i]->name) != NULL) {
                    talkIndex = i;
                } else if (((fldTaskInfo_t *)gEvnRng[i]->data)->slot >= 0) {
                    info = gEvnRng[i]->data;
                    script = dds3EventBasicGetResource(dds3SearchSceneBasic(scene, fldEveHitDataTable[info->slot]->objectId, 0xD));
                    fldEveHit_ScriptStart(script, i);
                }
            } else if (fldCheckNpcTalk(gEvnRng[i]->name) == NULL &&
                       ((fldTaskInfo_t *)gEvnRng[i]->data)->slot >= 0) {
                fldEveHit_ScriptEnd2(i);
            }
        } else if (kind == 1) {
            f32 x, y, z;
            position = event->transform;
            x = position[0];
            y = position[1];
            z = position[2];
            for (j = 0; j < 4; j++) {
                SDF_VU0_COPYVECTOR(&corners[j], room->corner[j]);
            }
            SDF_VU0_COPYVECTOR(&scratch, room->center);
            if (fldRunDanteGunFlag.word18 == 1) {
                ooyTestLine3D(0x8010FF10, 0x8010FF10, x, y, z,
                              x + scratch.x * 40.0f, y + scratch.y * 40.0f, z + scratch.z * 40.0f);
            }
            for (j = 1; j < 5; j++) {
                SDF_VU0_COPYVECTOR(&scratch, room->plane[j]);
            }
            current.x = fldGlobalBase.x;
            current.y = fldGlobalBase.y - 45.0f;
            current.z = fldGlobalBase.z;
            previous.x = fldGlobalBase.prevX;
            previous.y = fldGlobalBase.prevY - 45.0f;
            previous.z = fldGlobalBase.prevZ;
            nowDistance = fldVecDot(&current, (sdfVector4_t *)room->plane[0]) - room->limit[0];
            oldDistance = fldVecDot(&previous, (sdfVector4_t *)room->plane[0]) - room->limit[0];
            crossing = 0;
            if (nowDistance < 0.0f && oldDistance >= 0.0f) crossing = -1;
            else if (nowDistance >= 0.0f && oldDistance < 0.0f) crossing = 1;
            if (crossing != 0) {
                if (fldEveHitTestRect((f32 *)&current, i) != -1) {
                    room->shapeKind = crossing;
                    if (fldRunDanteGunFlag.word20 == 1) {
                        fldDbgPrint2D(0x40, 0x140, gEvnRng[i]->name);
                    }
                    if (fldCheckNpcTalk(gEvnRng[i]->name) != NULL) {
                        talkIndex = i;
                    } else if (((fldTaskInfo_t *)gEvnRng[i]->data)->slot >= 0) {
                        info = gEvnRng[i]->data;
                        script = dds3EventBasicGetResource(dds3SearchSceneBasic(scene, fldEveHitDataTable[info->slot]->objectId, 0xD));
                        if (scrSearchScriptName(script) == NULL) {
                            gEvnRngPid[i] = dds3SceneBasicStartScript(dds3GetActiveScene(), script);
                            gFldEveRngIdx = i;
                            gFldEveRngIdx_N = room->mode == 1 ? i : -1;
                            gFldEveRngIdx_T = room->mode == 2 ? i : -1;
                        }
                    }
                } else {
                    room->shapeKind = 0;
                }
            }
        } else if (kind == 2) {
            for (j = 0; j < 8; j++) {
                SDF_VU0_COPYVECTOR(&corners[j], room->corner[j]);
            }
            SDF_VU0_COPYVECTOR(&scratch, room->center);
            for (j = 0; j < 6; j++) {
                SDF_VU0_COPYVECTOR(&scratch, room->plane[j]);
            }
            scratch.x = player->poseset->pos.x;
            scratch.y = player->poseset->pos.y - 45.0f;
            scratch.z = player->poseset->pos.z;
            scratch.w = 1.0f;
            event = gEvnRng[i];
            if (fldEveHitTestPlanes(&scratch, i) != 0) {
                if (fldCheckNpcTalk(event->name) != NULL) {
                    talkIndex = i;
                } else if (fldFindEventEntry(gEvnRng[i]->name) != NULL) {
                    doorIndex = i;
                } else if (((fldTaskInfo_t *)gEvnRng[i]->data)->slot >= 0) {
                    info = gEvnRng[i]->data;
                    script = dds3EventBasicGetResource(dds3SearchSceneBasic(scene, fldEveHitDataTable[info->slot]->objectId, 0xD));
                    fldEveHit_ScriptStart(script, i);
                }
            } else if (fldCheckNpcTalk(event->name) == NULL && fldFindEventEntry(gEvnRng[i]->name) == NULL) {
                info = gEvnRng[i]->data;
                if (info->slot >= 0) {
                    script = dds3EventBasicGetResource(dds3SearchSceneBasic(scene, fldEveHitDataTable[info->slot]->objectId, 0xD));
                    fldEveHit_ScriptEnd(script, i);
                }
                gEvnRngPid[i] = 0;
            }
        }
    }
    if (talkIndex != -1) {
        if (fldEveHit_ScriptStart((char *)fldInf_FindTypeEveNpc(gEvnRng[talkIndex]->name), talkIndex) != -1) {
            fldEveRngHitIdx = talkIndex;
            gFldInf_HitIdx = talkIndex;
            gFldInf_WpExit = 0;
        }
        doorIndex = -1;
    } else if (fldEveRngHitIdx != talkIndex) {
        s32 oldIndex = fldEveRngHitIdx;
        fldEveHit_ScriptEnd((char *)fldInf_FindTypeEveNpc(gEvnRng[oldIndex]->name), oldIndex);
        fldEveRngHitIdx = talkIndex;
    }
    if (doorIndex != -1) {
        if (fldEveHit_ScriptStart((char *)fldDoor_Find2(gEvnRng[doorIndex]->name), doorIndex) != -1) {
            fldEveRngPrevIdx = doorIndex;
        }
    } else if (fldEveRngPrevIdx != doorIndex) {
        s32 oldIndex = fldEveRngPrevIdx;
        fldEveHit_ScriptEnd((char *)fldFindEventEntry(gEvnRng[oldIndex]->name), oldIndex);
        fldEveRngPrevIdx = doorIndex;
    }
}
#else
INCLUDE_RODATA(const s32, "field/fldEveHit", D_00531E08);

INCLUDE_ASM(const s32, "field/fldEveHit", fldEveHit_ChkEveHit);
#endif

void fld_13F6B8(void) {
    s32 count = fldEveRngCount;
    s32 i = 0;
    if (count > 0) {
        fldFileResource_t **entry = gEvnRng;
        do {
            fldDrawPos((*entry)->transform);
            i++;
            entry++;
        } while (i < fldEveRngCount);
    }
}

void fldPurgeEveRngProcesses(void) {
    s32 count = fldEveRngCount;
    s32 slotIndex = 0;
    if (count > 0) {
        u32 *taskSlot = gEvnRngPid;
        do {
            if (dds3ExistProcess((dds3ProcessID_t *)*taskSlot) == 0) {
                *taskSlot = 0;
            }
            slotIndex++;
            taskSlot++;
        } while (slotIndex < fldEveRngCount);
    }
}

s32 fldEveHitResID_Find(u32 pid) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == pid) {
            return gEvnRng[slotIndex]->id;
        }
    }
    return -1;
}

s32 fldFindEveRngIndex(u32 task) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == task) {
            return fldEveHitTable[slotIndex].roomId;
        }
    }
    return -1;
}

const char *fldEveHitResName_Find(u32 pid) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] == pid) {
            return gEvnRng[slotIndex]->name;
        }
    }
    return 0;
}

/* NON_MATCHING: 25 of 126 words differ (first at +0xC8). */
#ifdef NON_MATCHING
void fldEveHit_AddEveHitItemBox(s32 idx, f32 x, f32 y, f32 z, f32 r)
{
    u32 id;
    u32 pointId;
    s32 slot;
    s32 i = fldEveHitCount;
    fldResourcePoint_t *sisei;
    fldEveRngBox_t *box;
    dds3Basic_t *event;
    dds3Basic_t *point;

    id = dds3AllocObjectId();
    event = dds3AddEventBasic(id, "itembox_label", D_002FCD70);
    D_002FCFA0[i].event.resourceID = id;
    D_002FCFA0[i].put.eveIndex =
        fldEveHit_AddEveData((fldEventDataHeader_t *)&D_002FCFA0[i].event, event);
    D_002FCFA0[i].put.visible = 1;
    D_002FCFA0[i].hit.put = &D_002FCFA0[i].put;
    D_002FCFA0[i].ball.Type = 0;
    D_002FCFA0[i].ball.r = r;
    D_002FCFA0[i].put.type = 1;
    D_002FCFA0[i].put.ball = &D_002FCFA0[i].ball;
    sisei = &D_002FCFA0[i].sisei;
    sisei->pos.x = x;
    sisei->pos.y = y;
    sisei->pos.z = z;
    sisei->pos.w = 1.0f;
    sisei->rot.x = 0.0f;
    sisei->rot.y = 0.0f;
    sisei->rot.z = 0.0f;
    sisei->rot.w = 0.0f;
    sisei->scl.x = 1.0f;
    sisei->scl.y = 1.0f;
    sisei->scl.z = 1.0f;
    sisei->scl.w = 1.0f;
    D_002FCFA0[i].hit.sisei = sisei;
    pointId = dds3AllocObjectId();
    D_002FCFA0[i].hit.resourceID = pointId;
    D_002FCFA0[i].hit.resourceName = D_002FCD70;
    point = dds3AddPointBasic(pointId, D_002FCFA0[i].hit.sisei, D_002FCD80);
    slot = fldEveHit_AddEveHit(&D_002FCFA0[i].hit, point);
    fldEveHitCount++;
    box = (fldEveRngBox_t *)&fldEveHitTable[slot].unk120;
    box->takaraIndex = idx;
    box->hitKind = 2;
    box->npcIndex = -1;
}
#else
INCLUDE_ASM(const s32, "field/fldEveHit", fldEveHit_AddEveHitItemBox);
#endif

/* NON_MATCHING: 11 of 97 words differ (first at +0x2C). */
#ifdef NON_MATCHING
void fldEveHit_AddEveHitNpc(char *resname, s32 eveidx, f32 x, f32 y, f32 z, f32 r, s32 hojiID,
                           s32 npcidx)
{
    u32 id;
    s32 slot;
    s32 i = fldEveHitCount;
    fldEveRngBox_t *box;
    dds3Basic_t *point;
    fldResourcePoint_t *sisei;
    D_002FCFA0[i].put.ball = &D_002FCFA0[i].ball;
    D_002FCFA0[i].ball.Type = 0;
    D_002FCFA0[i].hit.put = &D_002FCFA0[i].put;
    D_002FCFA0[i].ball.r = r;
    D_002FCFA0[i].put.eveIndex = eveidx;
    D_002FCFA0[i].put.type = 1;
    D_002FCFA0[i].put.visible = 1;
    sisei = &D_002FCFA0[i].sisei;
    sisei->pos.x = x;
    sisei->pos.y = y;
    sisei->pos.z = z;
    sisei->pos.w = 1.0f;
    sisei->rot.x = 0.0f;
    sisei->rot.y = 0.0f;
    sisei->rot.z = 0.0f;
    sisei->rot.w = 0.0f;
    sisei->scl.x = 1.0f;
    sisei->scl.y = 1.0f;
    sisei->scl.z = 1.0f;
    sisei->scl.w = 1.0f;
    D_002FCFA0[i].hit.sisei = sisei;
    id = dds3AllocObjectId();
    D_002FCFA0[i].hit.resourceID = id;
    D_002FCFA0[i].hit.resourceName = resname;
    point = dds3AddPointBasic(id, D_002FCFA0[i].hit.sisei, D_002FCD80);
    slot = fldEveHit_AddEveHit(&D_002FCFA0[i].hit, point);
    fldEveHitCount++;
    box = (fldEveRngBox_t *)&fldEveHitTable[slot].unk120;
    box->unk04 = hojiID;
    box->hitKind = 1;
    box->npcIndex = npcidx;
}
#else
INCLUDE_ASM(const s32, "field/fldEveHit", fldEveHit_AddEveHitNpc);
#endif

s32 fldCheckEveRngN(s32 npcidx) {
    s32 idx;
    fldEveRngBox_t *box;

    idx = gFldEveRngIdx_N;
    if (gFldKyouseiTalk != 0) {
        return 1;
    }
    if (idx == -1) {
        return 0;
    }
    box = (fldEveRngBox_t *)&fldEveHitTable[idx].unk120;
    if (box->hitKind == 1) {
        if (box->npcIndex == npcidx) {
            return 1;
        }
    }
    return 0;
}

s32 fldCheckEveRngT(s32 takaraidx) {
    s32 idx;
    fldEveRngBox_t *box;

    idx = gFldEveRngIdx_T;
    if (idx == -1) {
        return 0;
    }
    box = (fldEveRngBox_t *)&fldEveHitTable[idx].unk120;
    if (box->hitKind == 2) {
        if (box->takaraIndex == takaraidx) {
            return 1;
        }
    }
    return 0;
}

s32 fldCheckEveRngEncount(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < fldEveRngCount; slotIndex++) {
        if (gEvnRngPid[slotIndex] != 0) {
            return 1;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = fldInfTable.packs[packOrSetIndex].setIndex;
                    if (fldInfTable.sets[gFldInfIdx].kindArea.packed == 1) {
                        gFldInfIdx = candidateSet;
                        return D_002FF3E0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (fldInfTable.sets[packOrSetIndex].kindArea.packed == 1 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}

const char *fldCheckNpcTalk2(const char *eventName) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (eventName == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(eventName, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    s32 candidateSet = fldInfTable.packs[packOrSetIndex].setIndex;
                    if (fldInfTable.sets[gFldInfIdx].kindArea.packed == 0) {
                        gFldInfIdx = candidateSet;
                        return D_002FF3E0;
                    }
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if (fldInfTable.sets[packOrSetIndex].kindArea.packed == 0 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(eventName, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}
INCLUDE_SDATA(const s32, "field/fldEveHit", fldEveRngPrevIdx);

INCLUDE_SDATA(const s32, "field/fldEveHit", fldEveRngHitIdx);

INCLUDE_SDATA(const s32, "field/fldEveHit", fldEveRngCount);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "field/fldEveHit", gEveNum);

INCLUDE_SDATA(const s32, "field/fldEveHit", fldEveHitState);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldKyouseiTalk);

INCLUDE_SDATA(const s32, "field/fldEveHit", fldEveHitCount);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldInfIdx);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "field/fldEveHit", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "field/fldEveHit", fldDoorExitFlag);

INCLUDE_SDATA(const s32, "field/fldEveHit", D_0054EDE5);

