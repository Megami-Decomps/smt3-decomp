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

extern pbProcHandle_t *pbMoveChar;

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern u32 pbMapIsValid(void);

extern s8 pbLoadState;

extern pbMapWork_t *pbMapWork;

extern pbProcHandle_t *record;

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern pbProcHandle_t *blockDB;

extern char pbMapPathBuf[];

extern pbProcHandle_t *pbWaitLoadMapData(pbProcHandle_t *parent);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 pb_draw_bg_Process(dds3ProcessID_t *id);

extern void pb_draw_bg_Shutdown(dds3ProcessID_t *id);

extern s32 pb_proc_dummy_Process(dds3ProcessID_t *id);

extern void pb_proc_dummy_Shutdown(dds3ProcessID_t *id);

extern void dds3FadeOut(s32, s32, s32, s32);

#include "sdk_libc.h"

extern void sdfFileCmdReadFile(char *aFileName, void *aBuffer, s32 aLength, void *aCallback, s32 aUserData);

extern s32 pbLoadSystemDataDone(s32 handle, s32 status);

extern pbProcHandle_t *pbGetMoveChar(void);
/* Opaque ABI view preserves retail annulment while the provider stays C.
 * Same real symbol and prototype; a reconstruction aid, not original spelling. */
extern pbProcHandle_t *pbGetMoveCharOpaque(void) FAKEMATCH_ASM_NAME("pbGetMoveChar");

extern const char pbEncycName[];

extern sdfMemHandle_t *a_key;

extern void p2sdStopAllSe(void);

extern void fclEncycFreeEntries(char *name);

extern void fldProcNextJump(void);

extern s8 pbCheckPreviousRecord(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void sdfTexFreeDelayed(void *aHandle);
extern u8 pbMapPathIdx, pbMenuIndex, pbMenuState;
extern const char pbDrawBgProcName[], pbMapPathFormat[];
extern char pbLoadMapDataProcName[];

typedef struct { dds3ProcessID_t *id; } pbMapEditorState_t;
extern s32 pbEditorState;
extern s32 pbEditWork;
/* Constructors consume the editor root's process-link prefix. The unsized
 * view preserves its non-small-data address form. */
extern pbMapEditorState_t pbMapEditorState[];
extern pbProcHandle_t *(*pbEditorCreateTable[])(pbProcHandle_t *parent);

extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 restore);
extern s32 p2sdStopBGM(s32 midi);
extern char pbProcNameFormat[]; /* Shared "%s\n" process-name format. */

/* Exact native dispatcher; see docs/matching/pb/pbMain.md. */
s32 pbEditInitProc(dds3ProcessID_t *id)
{
    if (pbEditWork == 0) {
        pbProcHandle_t *(*create)(pbProcHandle_t *) = pbEditorCreateTable[pbEditorState];

        if (create == NULL) {
            printf("********* Puzzle boy process halted (0).\n");
            dds3AdminiCallSequence(0, NULL, 0, 0);
            return -1;
        }
        pbMoveChar = create((pbProcHandle_t *)pbMapEditorState);
#ifdef FIX_BUGS
        /* The editor's constructor, func_001D4B28, returns NULL in retail, and
         * the switch below reads low memory through it. Halt as for no constructor. */
        if (pbMoveChar == NULL) {
            printf("********* Puzzle boy process halted (0).\n");
            dds3AdminiCallSequence(0, NULL, 0, 0);
            return -1;
        }
#endif
        pbEditWork = 1;
    }
    switch (pbMoveChar->state) {
    case 3:
        pbEditWork = 0;
        pbEditorState--;
        break;
    case 2:
    case 5:
        pbEditWork = 0;
        pbEditorState++;
        break;
    case 4:
        pbEditWork = 0;
        pbEditorState = pbMoveChar->unk04;
        break;
    case 6: {
        FAKEMATCH_ASM_REG(pbMapWork_t *, map, "$2");
        printf("********* Puzzle boy process halted (1).\n");
        p2sdStopBGM(0x340000);
        /* Local reconstruction aid: the live map address uses native $2.
         * Its lifetime starts after the sound call and ends at the byte read;
         * it does not carry a value across either call. */
        map = pbMapWork;
        if (map->unk35C[0] != 0) {
            dds3AdminiCallSequence(0, NULL, 0, 0);
        }
        return -1;
    }
    case 0:
    case 1:
        break;
    default:
        printf(pbProcNameFormat, pbMoveChar->id->name);
        printf("default %d\n", pbMoveChar->state);
        break;
    }
    return 0;
}

void pbEditMainExitProc(void) {
    p2sdStopAllSe();
    sdfTexFreeDelayed(pbMapWork->baseTex);
    sdfTexFreeDelayed(pbMapWork->tex344);
    sdfTexFreeDelayed(pbMapWork->tex348);
    sdfTexFreeDelayed(pbMapWork->tex34C);
    if (pbMapWork->giveupScript != NULL) {
        sdfMemFreeDirect(pbMapWork->mapData);
        pbMapWork->giveupScript = NULL;
    }
    sdfMemFreeDirect(a_key);
    fclEncycFreeEntries(pbEncycName);
    fldProcNextJump();
}

s32 pb_proc_dummy_Process(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    if (panel != NULL) {
        panel->flags = 0x20000;
    }
    return -1;
}

void pb_proc_dummy_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

extern void rsetOtOfs(s32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm,
                     s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);
extern dds3ProcessID_t *pbGetMessageScriptProc(void);
extern s32 fclGetFlag(s8 index);

s32 pb_draw_bg_Process(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    s32 state = panel->state;
    s32 tens;

    switch (state) {
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    default:
        break;
    }

    rsetOtOfs(0x27);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                0x80808080, pbMapWork->tex344);

    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (pbGetMessageScriptProc() == NULL && fclGetFlag(0) == 0) {
        DrwSprite2D(0, 0x15E, 0x200, 0x62, 0, 0x15E, 0x200, 0x62,
                    0x80808080, pbMapWork->tex348);
    }

    DrwSprite2D(0x100, 0, 0x100, 0xC8, 0x100, 0, 0x100, 0xC8,
                0x80808080, pbMapWork->tex348);

    DrwSprite2D(0x1C4, 0x1B, 0xD, 0x14,
                ((pbMapWork->stage + 1) % 10) * 0x14 + 0x28,
                0x1CC, 0xD, 0x14, 0x80808080, pbMapWork->tex348);
    tens = (pbMapWork->stage + 1) / 10;
    if (tens > 0) {
        DrwSprite2D(0x1B7, 0x1B, 0xD, 0x14, tens * 0x14 + 0x28, 0x1CC, 0xD, 0x14,
                    0x80808080, pbMapWork->tex348);
    }
    return 0;
}

void pb_draw_bg_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDelayedDirect(id->workdata);
}

extern const char pbEncycName[];
extern void pbLoadTextures(s32 mode, s32 stage);
extern s32 pbEditInitProc(dds3ProcessID_t *id);
extern pbProcHandle_t *pbStartBackGround(pbProcHandle_t *parent);
extern void pbEditMainExitProc(void);
extern void pbDebugShutdown(void);
extern dds3ProcessFunc_t *pbDebugProc(dds3ProcessID_t *proc_id);
extern s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update,
                               dds3ProcessFunc_t destroy, void *userValue);
extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 priority, s32 v2, s32 v3,
                                         dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

const char pbEncycName[16] = "PUZZLE BOY";

dds3ProcessID_t *pbStartMapEditor(s32 mode, s32 stage) {
    s32 state;
    dds3ProcessID_t *rootProcess;
    pbMapEditorState_t *mapState;

    pbEditWork = 0;
    if (mode == 0) {
        pbEditorState = 0;
    } else if (mode == 1) {
        pbEditorState = mode;
    } else {
        state = pbEditorState;
        if (mode == 2) {
            state = mode;
        }
        pbEditorState = state;
    }

    pbMoveChar = 0;
    rootProcess = dds3StartProcess("puzzleBoy-root", 0x3EE, 1, 0, pbEditInitProc,
                                   (dds3ProcessFunc_t)pbEditMainExitProc, NULL);
    mapState = pbMapEditorState;
    mapState->id = rootProcess;
    pbLoadTextures(mode, stage);
    pbStartBackGround((pbProcHandle_t *)mapState);
    if (mode == 1) {
        fclEncycAllocEntry(pbEncycName, (dds3ProcessFunc_t)pbDebugProc,
                           (dds3ProcessFunc_t)pbDebugShutdown, NULL);
    }
    return mapState->id;
}

pbProcHandle_t *pbGetMoveChar(void) {
    return pbMoveChar;
}

pbProcHandle_t *pbStartDummy(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->id = dds3StartProcess("pb-proc-dummy", 0x3EE, 1, 1, pb_proc_dummy_Process,
                                      (dds3ProcessFunc_t)pb_proc_dummy_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    work = sdfChipAllocDirect(4);
    work->value = 0;
    panel->data.value = work;
    panel->flags = 0;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

pbProcHandle_t *pbStartBackGround(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    panel->id = dds3StartProcess(pbDrawBgProcName, 0x2AFD, 1, 1, pb_draw_bg_Process,
                                      (dds3ProcessFunc_t)pb_draw_bg_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

/* No-op constructor in the same parent/handle table as pbStartGame. */
pbProcHandle_t *func_001D4B28(pbProcHandle_t *parent) {
    return NULL;
}

u32 pbMapIsValid(void) {
    u32 ret = (pbMapWork->pcharNum == 0);
    if ((pbMapWork->exitX < 0) || (pbMapWork->exitY < 0)) {
        ret |= 2;
    }
    return ret;
}

s32 pb_1D4B60(void) {
    return 0;
}

void func_001D4B68(void) {
}

s32 pb_1D4B70(void) {
    return 0;
}

void func_001D4B78(void) {
}

s32 pb_1D4B80(void) {
    return 0;
}

s32 pb_1D4B88(void) {
    return 0;
}

void func_001D4B90(void) {
}

s32 pbLoadSystemDataDone(s32 handle, s32 status) {
    if (status == 8) {
        pbLoadState = 1;
    } else if (status == 0) {
        pbLoadState = -1;
    }
    return 0;
}

s32 func_001D4BC0(void) {
    return 1;
}

s32 func_001D4BC8(void) {
    return pbMapIsValid() == 0;
}

s8 func_001D4BE8(void) {
    s8 ret = pbCheckPreviousRecord();
    return ret;
}

void func_001D4C08(pbProcHandle_t *panel) {
    if (pbMapIsValid() == 0) {
        dds3FadeOut(0, 0, 0, 1);
        dds3ResetProcessFlag(panel->id, 0x10, 1);
        if (pbGetMoveCharOpaque() != 0) {
            if (pbGetMoveCharOpaque() != 0) {
                ((pbProcHandle_t *)pbGetMoveCharOpaque())->flags = 0x40000;
            }
            ((pbProcHandle_t *)pbGetMoveCharOpaque())->flags |= 1;
        }
    }
}

void pbMakeMapPathHdd(pbProcHandle_t *parent) {
    if (pbMapPathIdx == 1) {
        dds3FadeOut(0, 0, 0, 1);
        sprintf(pbMapPathBuf, pbMapPathFormat, pbMenuIndex);
        sdfFileCmdReadFile(pbMapPathBuf, pbMapWork, -1, pbLoadSystemDataDone, 0);
        pbLoadState = 0;
        pbWaitLoadMapData(parent);
    } else {
        pbMapPathIdx = 1;
    }
}

const char pbDrawBgProcName[16] = "pb-draw-bg";

const char pbMapPathFormat[32] = "pfs0:/pb/mapdata/pbMap%02d.map";

