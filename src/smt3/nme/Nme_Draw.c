#include "common.h"
#include "nme.h"

extern s32 nmeGetInputMode(nmeWork_t *work);
extern void cmpSetCursorRow(s32 row, cmpCursorInfo_t *cursor);


extern nmeWork_t *nmeUpdateWork;
extern s32 nmeGetCursorPos(nmeWork_t *work);
extern void nmeStepCursor(s8 delta);
extern void nmeDeleteChar(s8 delta);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputUBar);

typedef struct {
    void *res;
    void *entries;
} nmeSpriteTable_t;

extern nmeSpriteTable_t GBWK_2 __attribute__((section(".data")));
extern nmeWork_t *namelist_buttonAnim_end;
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page, s32 spriteId,
                             s32 *texList, nmeSpriteTable_t *table, s32 ot);

void nmeDrawFuncIcon(void)
{
    nmeSpriteTable_t *table = &GBWK_2;
    s32 ot = 0x47;

    fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 0,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 1,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 2,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 0x18,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);

    if (namelist_buttonAnim_end->primaryCursorActive == 0 ||
        (s8)namelist_buttonAnim_end->unk1B != 0) {
        return;
    }
    switch (namelist_buttonAnim_end->cursor.unk_22 +
            (u8)namelist_buttonAnim_end->cursor.unk_24) {
    case 0:
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 6,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 3,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        break;
    case 1:
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 7,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 4,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        break;
    case 2:
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 8,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 5,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        break;
    case 3:
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 0x1A,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        fclDrawSpriteRes(0, 0x1E0, 0, 0, 1, 0x19,
                         (s32 *)namelist_buttonAnim_end->textures, table, ot);
        break;
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDrawInputPanel);

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void cmpUpdateTerminate(void *work);
extern s32 cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void nmeDrawBlendText(s32 sprite, s32 page, sdfTexHandle_t **textures, nmeSpriteTable_t *table, u32 color, s32 animation, s32 ot);
extern void nmeDrawNamePanel(void);
extern void nmeDrawInputPanel(void);
extern void nmeDrawCursor(void);
extern void nmeDrawModeTabs(s8 mode);
extern void nmeDrawInputUBar(s8 mode);
extern void nmeDrawButtonGuide(void);

s32 nmeDraw(dds3ProcessID_t *id)
{
    nmeWork_t *work = dds3GetProcessData(id);
    nmeSpriteTable_t *table;
    namelist_buttonAnim_end = work;
    if (work->sequence.flag & 0x20) {
        cmpUpdateTerminate(work);
        return -1;
    }
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, 0x80, 0x47);
    table = &GBWK_2;
    nmeDrawBlendText(2, 1, namelist_buttonAnim_end->textures, table,
                     0x80808080, namelist_buttonAnim_end->animation, 0x47);
    fclDrawSpriteRes(0, 0, 0, 0, 2, 0,
                     (s32 *)namelist_buttonAnim_end->textures, table, 0x47);
    nmeDrawNamePanel();
    nmeDrawInputPanel();
    nmeDrawCursor();
    nmeDrawFuncIcon();
    if ((s8)namelist_buttonAnim_end->unk1B == 0) {
        nmeDrawModeTabs(namelist_buttonAnim_end->inputMode);
        nmeDrawInputUBar(namelist_buttonAnim_end->inputMode);
    } else {
        nmeDrawButtonGuide();
    }
    return 0;
}

s32 nmeGetCursorPos(nmeWork_t *work) {
    s32 mode = nmeGetInputMode(work);

    switch (mode) {
    case 0:
        return work->inputPos;
    case 1: {
        s32 position = (u8)work->inputPos;
        position -= 8;
        return (s8)position;
    }
    default:
        return work->inputPos;
    }
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", func_0029A108);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeStepCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeDeleteChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeConfirmChar);

void nmeBackspace(void)
{
    if (nmeGetCursorPos(nmeUpdateWork) > 0) {
        nmeStepCursor(-1);
    }
    nmeDeleteChar(1);
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursorBy);

void nmeSyncCursor(void) {
    s8 row = (s8)((u8)nmeUpdateWork->cursor.unk_22 +
                  (u8)nmeUpdateWork->cursor.unk_24);

    cmpSetCursorRow((s32)row + 2, &nmeUpdateWork->inputCursor);
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursor);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursor2);

extern s8 cmpGetCursorRow(cmpCursorInfo_t *cursor);
extern s32 nmeGetRowCount(s8 mode);
extern void nmeInitCursor(s8 mode, nmeWork_t *work);

s32 nmeResetCursor(cmpCursorInfo_t *cursor, s8 delta)
{
    s8 mode = (s8)((u8)nmeUpdateWork->modeCursor.unk_22 +
                   (u8)nmeUpdateWork->modeCursor.unk_24);

    if (cmpGetCursorRow(cursor) != 0) {
        return 0;
    }
    if (delta > 0) {
        s32 cursorRow = cursor->unk_22;
        cursorRow += (u8)cursor->unk_24;
        if (cursorRow == nmeGetRowCount(mode) - 1) {
            nmeUpdateWork->primaryCursorActive = 1;
            nmeInitCursor(-1, nmeUpdateWork);
            return 1;
        }
    }
    if (delta < 0 && cursor->unk_22 + (u8)cursor->unk_24 == 0) {
        if (mode == 3) {
            nmeUpdateWork->alternateCursorActive = 1;
        } else {
            nmeUpdateWork->primaryCursorActive = 1;
            nmeInitCursor(-1, nmeUpdateWork);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeFindNextChar);

INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursorWrap);

u32 nmeInputIsLocked(s32 row) {
    return 0;
}

s32 nmeCheckInputMode(u8 mode)
{
    if (mode < 2) {
        if (nmeUpdateWork->inputRowEnabled[0] == 0 ||
            nmeUpdateWork->inputRowEnabled[1] == 0) return 0;
        if (nmeInputIsLocked(0) == 0) return 1;
        if (nmeInputIsLocked(1) != 0) return 0;
    } else {
        if (nmeUpdateWork->inputRowEnabled[2] == 0) return 0;
        if (nmeInputIsLocked(2) != 0) return 0;
    }
    return 1;
}


INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeUpdateInput);

void nmeAdvanceAnim(void)
{
    nmeWork_t *work = nmeUpdateWork;
    work->animation += 2;
    if (work->animation >= 0x900) {
        work->animation = 0;
    }
}
