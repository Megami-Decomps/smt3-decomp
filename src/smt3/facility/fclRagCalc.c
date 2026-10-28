#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct
{
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 Flag;
    /* 0x06 */ u16 res;
} fclSpriteEntry_t;

typedef struct
{
    /* 0x00 */ s8 tex;
    /* 0x02 */ fclSpriteRect_t rect;
} fclSpriteRes_t;

typedef struct
{
    /* 0x00 */ fclSpriteRes_t *res;
    /* 0x04 */ fclSpriteEntry_t *entries;
} fclSpriteTbl_t;

typedef struct
{
    /* 0x00 */ u8 StepY[4];
    /* 0x04 */ s8 StopFlag;
    /* 0x05 */ u8 unk_05[3];
    /* 0x08 */ s32 CursorSize;
    /* 0x0C */ s32 CursorPos;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s8 unk_24;
    /* 0x25 */ s8 unk_25;
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 unk_27[5];
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
} cmpCursorInfo_t;

extern char D_0053D1F0[];

extern char D_0053D200[];

typedef struct
{
    /* 0x0000 */ u8 unk_0000[9];
    /* 0x0009 */ u8 unk_0009[0x17];
    /* 0x0020 */ s32 flags;
    /* 0x0024 */ u8 unk_0024[0x138];
    /* 0x015C */ cmpCursorInfo_t listCursor;
} lstWork_t;

extern u8 ragHeartsTable[];

extern void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 OtNo);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void trmDrawListText(fclPanelPos_t *panel, s32 msg, s32 *tex, char *str,
                            s32 arg);

extern void fclDrawPhaseHeader(u8 *work, s32 color, s32 mode, s32 id);

typedef struct
{
    u8 flags;
    s8 sequenceFlag;
    u8 reserved02[2];
    s8 workStatus;
    u8 reserved05[0x13];
    s8 eventStarted;
    s8 ragType;
    u8 reserved1A[0x462A];
    void *messageFile;
    u32 script;
    s32 scriptArg;
    s32 backgroundTexture;
    s32 texture[2];
    u8 reserved465C[0x24];
    fclPanel_t panel[3];
} ragCalcWork_t;

extern ragCalcWork_t *ragCalcWork;
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 fclChkDbgProcess(void);
extern void fclRagEventStart(s8 signal);
extern s32 cmpLoadStatusModel(void);
extern s32 fclGetFlag(s8 index);
extern u32 dds3GetProcessStat(dds3ProcessID_t *id);
extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern void *dds3GetActiveScene(void);
extern void dds3VisibleSceneBasic(void *basic, s32 visible);
extern void cmpModelFreeResource(void);
extern void cmpResetDrawEnv(void);
extern void ragUpdateScene(void);
extern void cmpUpdateTerminate(void *work);

s32 ragCalc(dds3ProcessID_t *id)
{
    ragCalcWork = dds3GetProcessData(id);
    if (fclChkDbgProcess() != 0) {
        return 0;
    }
    if (ragCalcWork->eventStarted == 0) {
        fclRagEventStart(1);
        ragCalcWork->eventStarted = 1;
    }
    if (fclGetFlag(1) == 0) {
        return 0;
    }

    switch (ragCalcWork->sequenceFlag) {
    case 0:
        ragCalcWork->panel[2].Timer = 0;
        if (ragCalcWork->workStatus == 1) {
            dds3ProcessID_t *process = (dds3ProcessID_t *)(u32)ragCalcWork->scriptArg;

            if (dds3GetProcessStat(process) != 2) {
                ragCalcWork->scriptArg = fclScriptStart(ragCalcWork->script, 0, NULL);
            }
        }
        break;
    case 8:
        if (ragCalcWork->workStatus != 0) {
            fclClearFlag(2);
        }
        break;
    case 2:
    case 3: {
        void *scene = dds3GetActiveScene();

        if (scene != NULL) {
            dds3VisibleSceneBasic(scene, 1);
        }
        cmpModelFreeResource();
        cmpResetDrawEnv();
        break;
    }
    case 7:
        ragUpdateScene();
        break;
    }
    ragCalcWork->workStatus = 0;

    if (ragCalcWork->flags & 0x10) {
        cmpUpdateTerminate(ragCalcWork);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawFrameEdge);

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawListFrame);

typedef struct
{
    /* 0x0000 */ u8 unk_00[3];
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004[0x4650];
    /* 0x4654 */ s32 texHandle[1];
} ragDrawWork_t;

extern ragDrawWork_t *ragDrawWork;

extern fclSpriteTbl_t D_003306B8[];

void ragDrawHeartsFooter(fclPanel_t *panel, s32 id)
{
    cmpDrawBoxF(0x520, 0x200, panel->Z, 0x7b, 9, 0x19191e59, id);
    fclDrawSpriteRes(0, 0, panel->Z << 4, 0, 0, 3,
                     ragDrawWork->texHandle, D_003306B8, id);
    cmpDrawBoxF(0xf60, 0x200, panel->Z, 0xe5, 9, 0x161e276e, id);
    fclDrawSpriteRes(0, 0, panel->Z << 4, 0, 0, 4,
                     ragDrawWork->texHandle, D_003306B8, id);
}

void ragDrawListFooter(fclPanel_t *panel, s32 id)
{
    cmpDrawBoxF(0x490, 0x200, panel->Z, 0x6a, 9, 0x19191e59, id);
    fclDrawSpriteRes(0, 0, panel->Z << 4, 0, 0, 7,
                     ragDrawWork->texHandle, D_003306B8, id);
    cmpDrawBoxF(0xbb0, 0x200, panel->Z, 0x20, 9, 0x19191e59, id);
    fclDrawSpriteRes(0, 0, panel->Z << 4, 0, 0, 8,
                     ragDrawWork->texHandle, D_003306B8, id);
}

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawHeartsRows);

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawListRows);

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawHeartsList);

extern void ragDrawListRows(lstWork_t *list, s32 kind, u8 *rows, s32 mode, s32 ot);

extern void ragDrawListFrame(fclPanelPos_t *pos, s32 kind, u8 item, s32 ot);

extern s32 D_003305E8[];

/* NON_MATCHING: 10 of 84 words differ, the work pointer in the other
 * caller-saved register in each of the two blocks that load it. */
#ifdef NON_MATCHING
void ragDrawList2(s8 mode)
{
    fclPanelPos_t pos;
    u8 *work = (u8 *)ragDrawWork;
    s32 top = *(s16 *)(work + 0x2492);
    u8 *rows = work + top + 0x3B;
    s8 item = *(s8 *)(work + top + *(u8 *)(work + 0x2494) + 0x3B);

    fclDrawPanel((fclPanel_t *)(work + 0x4700), 0x47);
    fclUpdatePanel((fclPanel_t *)((u8 *)ragDrawWork + 0x4700));
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    trmDrawListText(&pos, *(s32 *)(ragHeartsTable + 0x8BC), ragDrawWork->texHandle, (char *)D_003306B8, 0x47);
    ragDrawListRows((lstWork_t *)((u8 *)ragDrawWork + 0x2314), 1, rows, mode, 0x47);
    fclDrawPanel((fclPanel_t *)((u8 *)ragDrawWork + 0x46C0), 0x47);
    fclInitPanelPos(*(s32 *)((u8 *)ragDrawWork + 0x46C0), *(s32 *)((u8 *)ragDrawWork + 0x46C4),
                    *(s32 *)((u8 *)ragDrawWork + 0x46C8), 0, 0, 0, (fclPanel_t *)&pos);
    ragDrawListFrame(&pos, 1, item, 0x47);
    fclDrawPhaseHeader((u8 *)ragDrawWork + 0x46C0, 0x1121D33, 1, 0x47);
    fclDrawPhaseHeader((u8 *)ragDrawWork + 0x2314, 0x80808080, 0, 0x47);
}
#else
INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawList2);
#endif

/* NON_MATCHING: 10 of 84 words differ, the same residual as ragDrawList2. */
#ifdef NON_MATCHING
void ragDrawList3(s8 mode)
{
    fclPanelPos_t pos;
    u8 *work = (u8 *)ragDrawWork;
    s32 top = *(s16 *)(work + 0x362A);
    u8 *rows = work + top + 0x44;
    s8 item = *(s8 *)(work + top + *(u8 *)(work + 0x362C) + 0x44);

    fclDrawPanel((fclPanel_t *)(work + 0x4700), 0x47);
    fclUpdatePanel((fclPanel_t *)((u8 *)ragDrawWork + 0x4700));
    fclInitPanelPos(0, 0xB0, 0xFFFFF, 0, 0, 0, (fclPanel_t *)&pos);
    trmDrawListText(&pos, D_003305E8[0], ragDrawWork->texHandle, (char *)D_003306B8, 0x47);
    ragDrawListRows((lstWork_t *)((u8 *)ragDrawWork + 0x34AC), 2, rows, mode, 0x47);
    fclDrawPanel((fclPanel_t *)((u8 *)ragDrawWork + 0x46C0), 0x47);
    fclInitPanelPos(*(s32 *)((u8 *)ragDrawWork + 0x46C0), *(s32 *)((u8 *)ragDrawWork + 0x46C4),
                    *(s32 *)((u8 *)ragDrawWork + 0x46C8), 0, 0, 0, (fclPanel_t *)&pos);
    ragDrawListFrame(&pos, 2, item, 0x47);
    fclDrawPhaseHeader((u8 *)ragDrawWork + 0x46C0, 0x14132D33, 1, 0x47);
    fclDrawPhaseHeader((u8 *)ragDrawWork + 0x34AC, 0x80808080, 0, 0x47);
}
#else
INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawList3);
#endif

INCLUDE_RODATA(const s32, "facility/fclRagCalc", D_0053D240);

INCLUDE_RODATA(const s32, "facility/fclRagCalc", D_0053D250);

INCLUDE_RODATA(const s32, "facility/fclRagCalc", D_0053D260);

INCLUDE_SDATA(const s32, "facility/fclRagCalc", D_0054FBA0);

INCLUDE_SDATA(const s32, "facility/fclRagCalc", D_0054FBA8);

