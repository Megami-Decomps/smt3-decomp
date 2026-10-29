#include "common.h"
#include "facility.h"
#include "nme.h"

extern s32 nmeGetInputMode(nmeWork_t *work);
extern s8 nmeGetModeFlag(nmeWork_t *work);
extern void cmpSetCursorRow(s32 row, cmpCursorInfo_t *cursor);


extern nmeWork_t *nmeUpdateWork;
extern s32 nmeGetCursorPos(nmeWork_t *work);
extern void nmeStepCursor(s8 delta);
extern void nmeDeleteChar(s8 delta);

extern void nmeDrawNameText(s32 x, s32 y, s32 width, s32 height, u32 color, s32 mode);
extern void nmeDrawNameString(s32 x, s32 y, u32 color, s32 mode);

void nmeDrawInputUBar(s8 mode)
{
    switch (mode) {
    case 0:
    case 1:
        return nmeDrawNameText(0x2A, 0x96, 0xF4, 0x96, 0xA09DC380, 0);
    case 2:
        nmeDrawNameText(0x2A, 0x54, 0xF4, 0x54, 0x5A503980, 1);
        nmeDrawNameString(0x83, 0x9A, 0xA09DC380, 0);
        break;
    }
}



extern fclSpriteTbl_t GBWK_2 __attribute__((section(".data")));
extern nmeWork_t *namelist_buttonAnim_end;
extern void fclDrawSpriteRes(s32 x, s32 y, s32 z, s32 *clip, s32 page, s32 spriteId,
                             s32 *texList, fclSpriteTbl_t *table, s32 ot);

void nmeDrawFuncIcon(void)
{
    fclSpriteTbl_t *table = &GBWK_2;
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

extern void nmeDrawPanelSprites(void);

void nmeDrawInputPanel(void)
{
    fclSpriteTbl_t *table;
    s32 ot = 0x47;

    nmeDrawPanelSprites();
    table = &GBWK_2;
    fclDrawSpriteRes(0xE40, 0xCE0, 0, 0, 3, 3,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0xE40, 0xCE0, 0, 0, 3, 4,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x1350, 0xCE0, 0, 0, 3, 3,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x1350, 0xCE0, 0, 0, 3, 4,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x1860, 0xCE0, 0, 0, 3, 3,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x1860, 0xCE0, 0, 0, 3, 4,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x2D0, 0, 0, 0, 0, 0x25,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x2D0, 0, 0, 0, 0, 0x26,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
    fclDrawSpriteRes(0x320, 0, 0, 0, 0, 0x27,
                     (s32 *)namelist_buttonAnim_end->textures, table, ot);
}

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void cmpUpdateTerminate(void *work);
extern void cmpDrawBoxF(s32 x, s32 y, s32 z, s32 width, s32 height, s32 color, s32 ot);
extern void nmeDrawBlendText(s32 sprite, s32 page, sdfTexHandle_t **textures, fclSpriteTbl_t *table, u32 color, s32 animation, s32 ot);
extern void nmeDrawNamePanel(void);
extern void nmeDrawInputPanel(void);
extern void nmeDrawCursor(void);
extern void nmeDrawModeTabs(s8 mode);
extern void nmeDrawInputUBar(s8 mode);
extern void nmeDrawButtonGuide(void);

s32 nmeDraw(dds3ProcessID_t *id)
{
    nmeWork_t *work = dds3GetProcessData(id);
    fclSpriteTbl_t *table;
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

void func_0029A108(s32 mode)
{
    s8 row = mode;
    s8 index = nmeGetModeFlag(nmeUpdateWork) - 1;

    while (index >= 0) {
        u8 first = nmeUpdateWork->inputChars[row][index][0];
        u8 second = nmeUpdateWork->inputChars[row][index][1];

        if (first > 0x80 || second > 0x80) {
            nmeUpdateWork->inputRowEnabled[row] = index + 1;
            return;
        }
        index--;
    }
    nmeUpdateWork->inputRowEnabled[row] = 0;
}


void nmeStepCursor(s8 delta)
{
    s32 cursorPos = nmeGetCursorPos(nmeUpdateWork);
    s32 mode = nmeGetInputMode(nmeUpdateWork);

    switch (mode) {
    case 0:
        if (delta > 0) {
            nmeUpdateWork->inputPos++;
        }
        if (delta < 0 && cursorPos > 0) {
            nmeUpdateWork->inputPos--;
        }
        break;
    case 1:
        if (delta > 0) {
            if (cursorPos < nmeGetModeFlag(nmeUpdateWork) - 1) {
                nmeUpdateWork->inputPos++;
            }
        }
        if (delta < 0) {
            nmeUpdateWork->inputPos--;
        }
        break;
    case 2:
        if (delta > 0) {
            if (cursorPos < nmeGetModeFlag(nmeUpdateWork) - 1) {
                nmeUpdateWork->inputPos++;
            }
        }
        if (delta < 0 && cursorPos > 0) {
            nmeUpdateWork->inputPos--;
        }
        break;
    }
}

void nmeDeleteChar(s8 delta)
{
    s32 direction = delta;
    s32 position = nmeGetCursorPos(nmeUpdateWork);
    s32 mode = nmeGetInputMode(nmeUpdateWork);
    s32 row = nmeUpdateWork->inputRowEnabled[mode];
    s32 index;

    if (row <= 0) {
        return;
    }
    nmeUpdateWork->inputChars[mode][position][0] = 0x80;
    nmeUpdateWork->inputChars[mode][position][1] = 0x80;
    if (direction != 0) {
        for (index = position + 1; index < nmeUpdateWork->inputRowEnabled[mode]; index++) {
            nmeUpdateWork->inputChars[mode][index - 1][0] =
                nmeUpdateWork->inputChars[mode][index][0];
            nmeUpdateWork->inputChars[mode][index - 1][1] =
                nmeUpdateWork->inputChars[mode][index][1];
            nmeUpdateWork->inputChars[mode][index][0] = 0x80;
            nmeUpdateWork->inputChars[mode][index][1] = 0x80;
        }
    }
    if (nmeUpdateWork->inputRowEnabled[mode] - 1 >= position) {
        nmeUpdateWork->inputRowEnabled[mode]--;
    }
    func_0029A108(mode);
}

extern const u8 *D_005276B8[];
extern void func_0029A108(s32 mode);

void nmeConfirmChar(void)
{
    nmeWork_t *work = nmeUpdateWork;
    s32 modeSelect = (s8)((u8)work->modeCursor.unk_22 +
                          (u8)work->modeCursor.unk_24);
    s32 inputRow = work->inputCursor.unk_22 +
                   (u8)work->inputCursor.unk_24;
    s32 nameColumn = work->nameCursor.unk_22 +
                     (u8)work->nameCursor.unk_24;
    const u8 *charTable;
    u8 char0;
    u8 char1;
    /* One position workspace covers the table offset and then the edit cursor.
     * Qualified cc1 allocation traces keep both phases in one pseudo; splitting
     * them restores eight differing words (plain/dump/hooked output parity). */
    s32 position;
    s32 mode;

    if (modeSelect != 2) {
        position = ((inputRow * 16) + nameColumn) * 2 + 2;
    } else {
        position = ((inputRow * 16) + nameColumn) * 2;
    }
    charTable = D_005276B8[modeSelect];
    char0 = charTable[position];
    char1 = charTable[position + 1];

    position = nmeGetCursorPos(nmeUpdateWork);
    mode = nmeGetInputMode(nmeUpdateWork);
    nmeUpdateWork->inputChars[mode][position][0] = char0;
    nmeUpdateWork->inputChars[mode][position][1] = char1;
    if (nmeUpdateWork->inputRowEnabled[mode] == position) {
        nmeUpdateWork->inputRowEnabled[mode] =
            (u8)nmeUpdateWork->inputRowEnabled[mode] + 1;
    }
    nmeStepCursor(1);
    func_0029A108(mode);
}

void nmeBackspace(void)
{
    if (nmeGetCursorPos(nmeUpdateWork) > 0) {
        nmeStepCursor(-1);
    }
    nmeDeleteChar(1);
}

#ifdef NON_MATCHING
extern const s8 D_005516B8[3];
extern const u8 *volatile D_005276C4;

void nmeMoveCursorBy(s8 delta)
{
    volatile s32 first = D_005516B8[0];
    volatile s32 second = D_005516B8[1];
    volatile s32 terminator = D_005516B8[2];
    volatile u8 text[3];
    cmpCursorInfo_t *cursor = &nmeUpdateWork->inputCursor;
    s32 limit;
    s16 position;

    text[0] = first;
    text[1] = second;
    text[2] = terminator;
    position = (u16)cursor->unk_22 + delta;
    cursor->unk_22 = position;
    limit = cursor->unk_20 - cursor->unk_26;
    while (position != limit) {
        const u8 *character;
        u8 firstByte, secondByte;
        if (position > limit) {
            cursor->unk_22 = cursor->unk_20 - cursor->unk_26;
            break;
        }
        position = cursor->unk_22;
        if (position < 0) {
            cursor->unk_22 = 0;
            break;
        }
        character = D_005276C4 + position * 0x20;
        firstByte = character[0];
        text[0] = firstByte;
        secondByte = character[1];
        text[1] = secondByte;
        if (firstByte != 0x80 || secondByte != firstByte) {
            break;
        }
        position = (u16)cursor->unk_22 + delta;
        cursor->unk_22 = position;
        limit = cursor->unk_20 - cursor->unk_26;
    }
}
#else
INCLUDE_ASM(const s32, "nme/Nme_Draw", nmeMoveCursorBy);
#endif

/* Native dispatchers pass the mode word; this helper derives the row globally. */
void nmeSyncCursor(s32 mode) {
    s8 row = (s8)((u8)nmeUpdateWork->cursor.unk_22 +
                  (u8)nmeUpdateWork->cursor.unk_24);

    cmpSetCursorRow((s32)row + 2, &nmeUpdateWork->inputCursor);
}

extern s32 cmpGetPressedButton(s32 mask);
extern void nmeMoveCursorBy(s8 delta);
extern s32 nmeGetRowCount(s8 mode);
extern void cmpResetCursorPos(s32 value, cmpCursorInfo_t *cursor);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

s32 nmeMoveCursor(void)
{
    s32 result = 0;
    u16 button = cmpGetPressedButton(0xF3);
    s8 mode = (s8)((u8)nmeUpdateWork->modeCursor.unk_22 +
                   (u8)nmeUpdateWork->modeCursor.unk_24);

    switch (button) {
    case 1:
        switch (nmeUpdateWork->cursor.unk_22 + (u8)nmeUpdateWork->cursor.unk_24) {
        case 0:
            nmeStepCursor(1);
            break;
        case 1:
            nmeStepCursor(-1);
            break;
        case 2:
            nmeDeleteChar(1);
            break;
        case 3:
            result = 1;
            break;
        }
        break;
    case 2:
        nmeBackspace();
        break;
    case 0x10:
        cmpMoveCursor(&nmeUpdateWork->cursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&nmeUpdateWork->cursor, 1);
        break;
    case 0x40:
        nmeUpdateWork->primaryCursorActive = 0;
        cmpResetCursorPos(nmeGetRowCount(mode) - 1, &nmeUpdateWork->nameCursor);
        break;
    case 0x80:
        nmeUpdateWork->primaryCursorActive = 0;
        if (mode == 3) {
            nmeUpdateWork->alternateCursorActive = 1;
        } else {
            cmpResetCursorPos(0, &nmeUpdateWork->nameCursor);
        }
        break;
    }
    nmeSyncCursor(mode);
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&nmeUpdateWork->cursor, 0);
    }
    button = (u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&nmeUpdateWork->cursor,
                                       button, 0x30);
    cmpPlayCursorSE(button);
    return result;
}

void nmeMoveCursor2(void)
{
    u16 button = cmpGetPressedButton(0xF3);
    s8 mode = (s8)((u8)nmeUpdateWork->modeCursor.unk_22 +
                   (u8)nmeUpdateWork->modeCursor.unk_24);

    switch (button) {
    case 1:
        return;
    case 2:
        nmeBackspace();
        break;
    case 0x10:
        nmeMoveCursorBy(-1);
        break;
    case 0x20:
        nmeMoveCursorBy(1);
        break;
    case 0x40:
        nmeUpdateWork->alternateCursorActive = 0;
        nmeUpdateWork->primaryCursorActive = 1;
        cmpResetCursorPos(nmeGetRowCount(mode) - 1, &nmeUpdateWork->nameCursor);
        break;
    case 0x80:
        nmeUpdateWork->alternateCursorActive = 0;
        cmpResetCursorPos(0, &nmeUpdateWork->nameCursor);
        break;
    }
    nmeSyncCursor(mode);
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&nmeUpdateWork->cursor, 0);
    }
    button = (u16)cmpDisablePadBitStop((cmpPadStopWork_t *)&nmeUpdateWork->cursor,
                                       button, 0x30);
    cmpPlayCursorSE(button);
}

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


extern s32 cmpGetPressedButton(s32 mask);
extern s32 cmpFindNextCursorItem(cmpCursorInfo_t *cursor, s8 direction, s32 mode);
extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpGetPadUpDown(void);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void nmeConfirmChar(void);
extern void func_002413B8(u32 button, s8 mode);

void nmeFindNextChar(void)
{
    u16 button = (u16)cmpGetPressedButton(0xF3);

    switch (button) {
    case 1:
        nmeConfirmChar();
        break;
    case 2:
        nmeBackspace();
        break;
    case 0x10:
        cmpFindNextCursorItem(&nmeUpdateWork->inputCursor, -1, 0);
        break;
    case 0x20:
        cmpFindNextCursorItem(&nmeUpdateWork->inputCursor, 1, 0);
        break;
    case 0x40:
        if (nmeResetCursor(&nmeUpdateWork->nameCursor, -1) == 0) {
            cmpMoveCursor(&nmeUpdateWork->nameCursor, -1);
        }
        break;
    case 0x80:
        if (nmeResetCursor(&nmeUpdateWork->nameCursor, 1) == 0) {
            cmpMoveCursor(&nmeUpdateWork->nameCursor, 1);
        }
        break;
    }

    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&nmeUpdateWork->inputCursor, 0);
    }
    button = (u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&nmeUpdateWork->inputCursor, button, 0x30);
    if (cmpGetPadUpDown() == 0) {
        cmpMoveCursor(&nmeUpdateWork->nameCursor, 0);
    }
    button = (u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&nmeUpdateWork->nameCursor, button, 0xC0);
    func_002413B8(button, 1);
}

extern void nmeStartFadeOut(void);
extern void nmeInitNameBuffer(s8 mode, nmeWork_t *work);

void nmeMoveCursorWrap(void)
{
    u16 button = (u16)cmpGetPressedButton(0x33);

    switch (button) {
    case 1:
        if (nmeUpdateWork->wrapCursor.unk_22 +
            (u8)nmeUpdateWork->wrapCursor.unk_24 == 0) {
            nmeUpdateWork->unk1B = 2;
            nmeUpdateWork->sequence.flag |= 0x10;
            nmeStartFadeOut();
        } else {
            nmeInitNameBuffer(0, nmeUpdateWork);
        }
        break;
    case 2:
        nmeInitNameBuffer(0, nmeUpdateWork);
        break;
    case 0x10:
        cmpMoveCursor(&nmeUpdateWork->wrapCursor, -1);
        break;
    case 0x20:
        cmpMoveCursor(&nmeUpdateWork->wrapCursor, 1);
        break;
    }

    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor(&nmeUpdateWork->wrapCursor, 0);
    }
    button = (u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)&nmeUpdateWork->wrapCursor, button, 0x30);
    func_002413B8(button, 2);
}

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


extern s32 cmpGetPressedButton(s32 mask);
extern void cmpPlayCursorSE(u32 button);
extern void nmeMoveCursorWrap(void);
extern void nmeMoveCursor2(void);
extern s32 nmeMoveCursor(void);
extern void nmeFindNextChar(void);
extern void nmeInitNameRows(s8 mode, nmeWork_t *work);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);

void nmeUpdateInput(void)
{
    u32 button = (u16)cmpGetPressedButton(0x1000);
    s32 moved = 0;
    s32 cursorSource = 0;

    if ((s8)nmeUpdateWork->unk1B == 1) {
        nmeMoveCursorWrap();
        cursorSource = 2;
    }
    if ((s8)nmeUpdateWork->unk1B != 0) return;
    if (cursorSource == 0) {
        if (nmeUpdateWork->alternateCursorActive != 0) {
            nmeMoveCursor2();
            cursorSource = 1;
        }
    }
    if (cursorSource == 0) {
        if (nmeUpdateWork->primaryCursorActive != 0) {
            moved = nmeMoveCursor() != 0;
            cursorSource = 1;
        }
    }
    if (cursorSource == 0) nmeFindNextChar();
    if (button == 0x1000) moved = 1;
    if (moved != 0) {
        if (nmeUpdateWork->inputMode == 0) {
            if (nmeCheckInputMode(0) != 0) {
                if (nmeUpdateWork->entryMode == 0) nmeInitNameBuffer(2, nmeUpdateWork);
                else nmeUpdateWork->unk1B = 1;
            } else if (nmeUpdateWork->targetNo == 0) {
                button = 0x8000;
            } else {
                nmeInitNameRows(nmeUpdateWork->targetNo, nmeUpdateWork);
                nmeUpdateWork->unk1B = 1;
            }
        } else if (nmeCheckInputMode(2) != 0) {
            nmeUpdateWork->unk1B = 1;
        } else {
            button = 0x8000;
        }
        if ((s8)nmeUpdateWork->unk1B != 0) {
            cmpInitCursorRange(&nmeUpdateWork->wrapCursor, 2, 2, 0);
            cmpInitCursorLayout(&nmeUpdateWork->wrapCursor, 0, 0, 0, 0, 0);
        }
    }
    cmpPlayCursorSE(button);
}

void nmeAdvanceAnim(void)
{
    nmeWork_t *work = nmeUpdateWork;
    work->animation += 2;
    if (work->animation >= 0x900) {
        work->animation = 0;
    }
}
