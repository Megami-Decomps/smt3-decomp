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

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern void sdfChipFreeDirect(void *aPtr);

extern s8 pbLoadState;

extern pbMapWork_t *D_0054F5C4;

extern pbProcHandle_t *record;

extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern pbProcHandle_t *blockDB;

extern char pbMapPathBuf[];

extern char D_00535E40[], D_00535E68[];

extern pbProcHandle_t *pbWaitLoadMapData(pbProcHandle_t *parent);

extern void *sdfChipAllocDirect(u32 aSize);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

extern s32 pb_load_mapdata_Process(dds3ProcessID_t *id);

extern void pb_load_mapdata_Shutdown(dds3ProcessID_t *id);

extern u8 pbMapPathIdx, pbMenuIndex, pbMenuState;

extern void *memset(void *a, s32 b, u32 c);

extern s32 pb_proc_menu_Process(dds3ProcessID_t *id);

extern void pb_proc_menu_Shutdown(dds3ProcessID_t *id);

extern char D_005361B0[];

extern void dds3FadeOut(s32, s32, s32, s32);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void sdfFileCmdReadFile(char *aFileName, void *aBuffer, s32 aLength, void *aCallback, s32 aUserData);

extern s32 pbLoadSystemDataDone(s32 handle, s32 status);

extern pbProcHandle_t *pbGetMoveChar(void);

extern void dds3AdminiCallSequence(s32, s32, s32, s32);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);

extern char D_00535E90[];

extern dds3ProcessID_t *pbStartGiveupConfirm(void);

extern void pbPreviousRecord(void);

extern pbProcHandle_t *pbStartHelp(pbProcHandle_t *parent, s32 page);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern char D_00535E10[], D_00535EC8[];


extern u8 pbMapNo;

void pbMakeMapPath(pbProcHandle_t *parent)
{
    if (pbMapPathIdx == 3) {
        dds3FadeOut(0, 0, 0, 1);
        if (pbMapNo < 30) {
            sprintf(pbMapPathBuf, D_00535E40, pbMapNo);
        } else {
            sprintf(pbMapPathBuf, D_00535E68, pbMapNo);
        }
        sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
        pbLoadState = 0;
        pbWaitLoadMapData(parent);
    } else {
        pbMapPathIdx = 3;
    }
}

void func_001D4E08(void) {
}

void func_001D4E10(void) {
}

void exitPB(pbProcHandle_t *p) {
    dds3ResetProcessFlag(p->id, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        pbGetMoveChar()->flags = 0x60000;
    }
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void func_001D4E70(pbProcHandle_t *panel) {
    dds3ResetProcessFlag(panel->id, 0x10, 1);
    pbStartGiveupConfirm();
    panel->flags = 0x50000;
}


void func_001D4EB0(pbProcHandle_t *panel) {
    dds3ResetProcessFlag(panel->id, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        pbGetMoveChar()->flags = 0x10000;
    }
    panel->flags = 0x50000;
}


void func_001D4F08(void) {
    pbPreviousRecord();
}

void pbSetDebugOwner(pbProcHandle_t *panel) {
    pbPanelValue_t *work = panel->data.value;
    pbProcHandle_t *help = pbStartHelp(pbGetMoveChar(), work->value);
    dds3ProcessID_t *id;
    D_0054F5C4->helpProc = help;
    dds3SetProcessFlag(help->id, 0x10, 1);
    id = dds3SearchProcessName(D_00535E90);
    if (id != NULL) {
        dds3ResetProcessFlag(id, 0x10, 2);
    }
    if (panel != NULL) {
        panel->flags = 0x50000;
    }
}

extern u8 dds3PadButtonState[];
extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);
extern void rsetOtOfs(u32 offset);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail,
                     s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 width, s32 height,
                        s32 u, s32 v, s32 uw, s32 vh, u32 color,
                        sdfTexHandle_t *texture);
extern u8 D_0054F5D0[2];
extern s32 D_0054F5D4;
extern u8 D_00551EC1;
extern s32 (*D_003272F0[][8])(pbProcHandle_t *panel);
extern void (*D_00327330[][8])(pbProcHandle_t *panel);

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E40);

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E68);

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535E90);

s32 pb_proc_menu_Process(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;

    switch (panel->state) {
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    }

    if ((s8)dds3PadButtonState[0x23] < 0 || (s8)dds3PadButtonState[0x20] < 0) {
        p2sdPlaySE(0x340006, 0x7F, 0x3F);
        return -1;
    }

    switch (pbMapPathIdx) {
    case 0:
        if (dds3PadButtonState[0x26] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            if (pbMenuState == 0) {
                pbMenuState = D_0054F5D0[work->value] - 1;
            } else {
                pbMenuState = pbMenuState - 1;
            }
        } else if (dds3PadButtonState[0x27] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            if (pbMenuState < D_0054F5D0[work->value] - 1) {
                pbMenuState = pbMenuState + 1;
            } else {
                pbMenuState = 0;
            }
        }
        break;
    case 1:
        if (dds3PadButtonState[0x24] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMenuIndex = (pbMenuIndex + 0x1D) % 0x1E;
        } else if (dds3PadButtonState[0x25] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMenuIndex = (pbMenuIndex + 0x1F) % 0x1E;
        } else if (dds3PadButtonState[0x26] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMenuIndex = (pbMenuIndex + 0x28) % 0x1E;
        } else if (dds3PadButtonState[0x27] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMenuIndex = (pbMenuIndex + 0x14) % 0x1E;
        }
        break;
    case 3:
        if (dds3PadButtonState[0x24] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMapNo = (pbMapNo + 0x31) % 0x32;
        } else if (dds3PadButtonState[0x25] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMapNo = (pbMapNo + 0x33) % 0x32;
        } else if (dds3PadButtonState[0x26] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMapNo = (pbMapNo + 0x3C) % 0x32;
        } else if (dds3PadButtonState[0x27] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            pbMapNo = (pbMapNo + 0x28) % 0x32;
        }
        break;
    case 4:
        if ((dds3PadButtonState[0x24] & 2) || (dds3PadButtonState[0x25] & 2)) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            if (D_0054F5D4 != 0) {
                D_0054F5D4 = 0;
            } else {
                D_0054F5D4 = 1;
            }
        }
        break;
    case 5:
        if (dds3PadButtonState[0x24] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            D_00551EC1 = (D_00551EC1 + 0xC) % 0xD;
        } else if (dds3PadButtonState[0x25] & 2) {
            p2sdPlaySE(0x340007, 0x7F, 0x3F);
            D_00551EC1 = (D_00551EC1 + 0xE) % 0xD;
        }
        break;
    }

    if ((s8)dds3PadButtonState[0x21] < 0) {
        if (D_003272F0[work->value][pbMenuState](panel) == 1) {
            p2sdPlaySE(0x340005, 0x7F, 0x3F);
            D_00327330[work->value][pbMenuState](panel);
        } else {
            p2sdPlaySE(0x340008, 0x7F, 0x3F);
        }
    } else if ((s8)dds3PadButtonState[0x23] < 0) {
        p2sdPlaySE(0x340006, 0x7F, 0x3F);
        if (pbMapPathIdx != 0) {
            pbMapPathIdx = 0;
        }
    }

    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0, 0, 0x100, 0x100, 0, 0, 0x100, 0x100, 0x80808080,
                (sdfTexHandle_t *)D_0054F5C4->tex348);
    DrwSprite2D(0x27, pbMenuState * 24 + 0x43, 0x1A, 0x1A, 1, 0x1CC, 0x1A, 0x1A,
                0x80808080, (sdfTexHandle_t *)D_0054F5C4->tex348);
    return 0;
}

void pb_proc_menu_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    dds3ResetProcessFlag(id, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->menuState = 0;
}

extern void *memcpy(void *dst, const void *src, u32 size);
typedef struct { u16 cells[20][20]; } pbMapBlock_t;
extern pbMapBlock_t D_00327458;

s32 pb_load_mapdata_Process(dds3ProcessID_t *id) {
    s32 result = 0;
    pbProcHandle_t *parent;
    pbPanelValue_t *panelValue;

    if (id->parent != NULL) {
        s8 loadState;
        parent = id->parent->workdata;
        loadState = pbLoadState;
        panelValue = parent->data.value;
        if (loadState == 1) {
            dds3ResetProcessFlag(parent->id, 0x10, 1);
            parent->flags = 0x50000;
            if (pbGetMoveChar() != 0) {
                pbGetMoveChar()->flags = 0x10000;
            }
            if (panelValue->value == loadState) {
                memcpy(&D_00327458, D_0054F5C4->unk01C, 0x320);
            }
            result = -1;
        }
    } else if (pbLoadState == 1) {
        if (pbGetMoveChar() != 0) {
            pbGetMoveChar()->flags = 0x10000;
        }
        memcpy(&D_00327458, D_0054F5C4->unk01C, 0x320);
        result = -1;
    }

    return result;
}

void pb_load_mapdata_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDirect(id->workdata);
}

pbProcHandle_t *pbStartMenu(pbProcHandle_t *parent, s32 mode) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->id = dds3StartProcess("pb-proc-menu", 0x3F8, 1, 1, pb_proc_menu_Process,
                                      (dds3ProcessFunc_t)pb_proc_menu_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    panel->data.value->value = mode;
    dds3SetProcessData(panel->id, panel);
    pbMenuState = 0;
    pbMapPathIdx = 0;
    return panel;
}

pbProcHandle_t *pbWaitLoadMapData(pbProcHandle_t *parent) {
    pbProcHandle_t *panel = sdfChipAllocDirect(0x10);
    panel->id = dds3StartProcess(D_00535EC8, 0x3EE, 1, 1, pb_load_mapdata_Process,
                                      (dds3ProcessFunc_t)pb_load_mapdata_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->id, panel->id);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->id, panel);
    return panel;
}

void pbLoadSystemData(s32 num) {
    dds3FadeOut(0, 0, 0, 1);
    if (num < 30) {
        sprintf(pbMapPathBuf, D_00535E40, num);
    } else {
        sprintf(pbMapPathBuf, D_00535E68, num);
    }
    sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
    pbLoadState = 0;
    pbWaitLoadMapData(NULL);
}

INCLUDE_RODATA(const s32, "pb/pbMenu", D_00535EC8);

