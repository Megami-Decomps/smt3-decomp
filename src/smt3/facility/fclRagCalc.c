#include "common.h"
#include "interface.h"
#include "fclRag.h"
#include "facility.h"
#include "camp.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"
#include "font.h"









extern char D_0053D1F0[];

extern char D_0053D200[];

extern u8 ragHeartsTable[];

extern void fclDrawSpriteRes(s32 X, s32 Y, s32 Z, s32 *pClip, s32 page, s32 spriteId, s32 *texList,
                             fclSpriteTbl_t *table, s32 OtNo);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern s32 trmDrawListText(fclPanelPos_t *panel, char *text, s32 *tex, fclSpriteTbl_t *table,
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

#ifdef NON_MATCHING
extern void fclClearFlag(s8 index);
#endif

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


typedef struct
{
    /* 0x0000 */ u8 unk_00[3];
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004[0x4650];
    /* 0x4654 */ s32 texHandle[1];
} ragDrawWork_t;

extern ragDrawWork_t *ragDrawWork;

extern fclSpriteTbl_t D_003306B8[];


extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);

extern s32 sprintf(char *buf, const char *fmt, ...);
extern ragItemReq_t *ragGetConfirmEntry(s8 kind, u8 index);
extern void cmpSetSkillListRows(s32 rows);
extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                                const char *text, frFontHandle_t *previous);
extern frFontHandle_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                           const char *text, frFontHandle_t *previous);
extern s32 func_001C7E38(frFontHandle_t *request, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *request);
extern void fclDrawSpriteFilter(s32 x, s32 y, s32 z, u8 filter, s32 *color,
                                s32 page, s32 spriteId, s32 *texList,
                                fclSpriteTbl_t *table, s32 ot);
extern s32 fclDrawListSprite(s32 x, s32 y, s32 color, s32 width, s32 height,
                             s32 arg6, s32 blend, s32 ot);
extern char *pStr[];
extern char D_0054FBA0[];
extern char D_0054FBA8[] __attribute__((section(".sdata")));

void ragDrawFrameEdge(fclPanelPos_t *pos, u8 kind, u8 item, s32 ot)
{
    ragItemReq_t *entries = ragGetConfirmEntry((s8)kind, item);
    s32 x = pos->unk_00;
    s32 rowY = pos->unk_04;
    s32 i;

    cmpSetSkillListRows(0x13);
    for (i = 0; i < 3; i++) {
        u16 id = entries[i].item;

        if (id != 0) {
            char text[0x20];
            frFontHandle_t *font;
            s32 numberY = rowY + 0x18;

            sprintf(text, D_0054FBA0, pStr[id]);
            font = frwGetSmallFontFRQ2(x, rowY + 0x38, pos->unk_08,
                                       0x75A8EB80, text, NULL);
            func_001C7E38(font, 1, ot);
            frDestroyFontRequestAfter(font);
            fclDrawSpriteRes(x + 0x650, rowY + 0x60,
                             pos->unk_08 << 4, NULL, 0, 1,
                             ragDrawWork->texHandle, D_003306B8, ot);

            sprintf(text, D_0054FBA8, entries[i].count);
            font = frwGetNumFontFRQ(x + 0x740, numberY,
                                    pos->unk_08, 0x75A8EB80, text, NULL);
            func_001C7E38(font, 1, ot);
            frDestroyFontRequestAfter(font);

            sprintf(text, D_0054FBA8, DDS3_GBWK->stockRemainder[id]);
            font = frwGetNumFontFRQ(x + 0xB10, numberY,
                                    pos->unk_08, 0x6C81BB80, text, NULL);
            func_001C7E38(font, 1, ot);
            frDestroyFontRequestAfter(font);

            fclDrawSpriteFilter(x + 0xCC0, rowY + 0x48,
                                pos->unk_08 << 4, 1, NULL, 0, 2,
                                ragDrawWork->texHandle, D_003306B8, ot);
            fclDrawListSprite(x + 0xC0, rowY + 0xE0, pos->unk_08,
                              0xD8, 2, 0x152480, 0, ot);
            rowY += 0xC8;
        }
    }
    cmpSetSkillListRows(-1);
}

void ragDrawListFrame(fclPanelPos_t *pos, s8 kind, u8 item, s32 ot)
{
    cmpDrawBoxF(0xEA0, 0x660, pos->unk_08, 0x9A, 9, 0x0B192C6E, ot);
    fclDrawSpriteRes(0, 0, pos->unk_08 << 4, NULL, 0, 5,
                     ragDrawWork->texHandle, D_003306B8, ot);
    cmpDrawBoxF(0x18C0, 0x660, pos->unk_08, 0x4F, 9, 0x0B192C6E, ot);
    fclDrawSpriteRes(0, 0, pos->unk_08 << 4, NULL, 0, 6,
                     ragDrawWork->texHandle, D_003306B8, ot);
    ragDrawFrameEdge(pos, kind, item, ot);
}

void ragDrawHeartsFooter(fclPanelPos_t *panel, s32 id)
{
    cmpDrawBoxF(0x520, 0x200, panel->unk_08, 0x7b, 9, 0x19191e59, id);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, 0, 0, 3,
                     ragDrawWork->texHandle, D_003306B8, id);
    cmpDrawBoxF(0xf60, 0x200, panel->unk_08, 0xe5, 9, 0x161e276e, id);
    fclDrawSpriteRes(0, 0, panel->unk_08 << 4, 0, 0, 4,
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

extern ragItemReq_t *ragGetConfirmEntry(s8 kind, u8 index);
extern s32 ragFindItemSlot(ragItemReq_t *requirements);
extern s32 datGetItemSlotIndex(u16 item);
extern s32 cmbChkGetHearts(s32 index);
extern void lstDrawListWindow(lstWork_t *work, s8 noCursor, s32 ot);
extern void lstDrawListWindowCursor(lstWindowWork_t *work, s32 ot);
extern void cmpSetSkillListRows(s32 rows);
extern u32 cmpDrawListItemBox(cmpCursorInfo_t *cursor, s32 item, s32 row,
                              s32 mode, s32 alpha);
extern frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                          const char *text, frFontHandle_t *previous);
extern s32 fclDrawListSprite(s32 x, s32 y, s32 z, s32 width, s32 height,
                             s32 color, s32 blend, s32 ot);
extern char *pStr[];
extern s32 func_001C7E38(frFontHandle_t *font, s8 mode, u32 ot);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *font);

#ifdef NON_MATCHING
void ragDrawHeartsRows(lstWork_t *list, u8 kind, s8 *rows, s8 mode, s32 ot)
{
    cmpCursorInfo_t *cursor = &list->listCursor;
    s32 x = list->pos.unk_00;
    s32 y = list->pos.unk_04;
    s32 z = list->pos.unk_08;
    s32 i;

    lstDrawListWindow(list, mode, ot);
    cmpSetSkillListRows(0x13);
    for (i = 0; i < cursor->unk_26; i++) {
        s8 index = rows[i];
        u8 flag = (u8)rows[i];
        u16 item = ((ragHeartsEntry_t *)ragHeartsTable)[index].item;
        ragItemReq_t *requirements = ragGetConfirmEntry((s8)kind, flag);
        s32 disabled = ragFindItemSlot(requirements) == 0;
        s32 slot = datGetItemSlotIndex(item);
        u32 color;
        frFontHandle_t *font;

        if (slot != 0 && cmbChkGetHearts(slot) != 0) {
            disabled = 1;
        }
        color = cmpDrawListItemBox(cursor, i, i, disabled | (mode != 0 ? 2 : 0), 0x100);
        font = frwGetSmallFontFRQ2(x + 0x310, y + 0xC0, z, color, pStr[item], NULL);
        func_001C7E38(font, 1, ot);
        frDestroyFontRequestAfter(font);
        fclDrawListSprite(x + 0x210, y + 0x160, z, 0x93, 2, 0x22242F80, 0, ot);
        y += cursor->StepY << 3;
    }
    cmpSetSkillListRows(-1);
    ragDrawHeartsFooter(&list->pos, ot);
    if (mode == 0) {
        lstDrawListWindowCursor((lstWindowWork_t *)list, ot);
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawHeartsRows);
#endif

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawListRows);

INCLUDE_ASM(const s32, "facility/fclRagCalc", ragDrawHeartsList);

extern void ragDrawListRows(lstWork_t *list, s32 kind, u8 *rows, s32 mode, s32 ot);

extern char *D_003305E8[];

/* NON_MATCHING: 10 of 84 words differ, the work pointer in the other
 * caller-saved register in each of the two blocks that load it. */
#ifdef NON_MATCHING
extern s32 fclDrawPanel(fclPanel_t *pPanel, s32 OtNo);
extern void fclUpdatePanel(fclPanel_t *pPanel);

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
    trmDrawListText(&pos, *(char **)(ragHeartsTable + 0x8BC), ragDrawWork->texHandle, D_003306B8, 0x47);
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
    trmDrawListText(&pos, D_003305E8[0], ragDrawWork->texHandle, D_003306B8, 0x47);
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

