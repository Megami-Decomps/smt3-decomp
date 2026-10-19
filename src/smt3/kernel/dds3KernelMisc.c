#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern s8 dds3PadButtonState[64];

extern u64 *dds3PrimFrameRect(s32 x, s32 y, s32 z, s32 width, s32 height, u32 fill, u32 border);

extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);

void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput) {
    s32 backwardState;
    s32 forwardState;

    if (cursorRow == NULL) {
        return;
    }
    if (itemCount < 2) {
        return;
    }
    backwardState = dds3PadButtonState[backwardInput + padSet * 16];
    if (backwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            } else if (itemCount < visibleRows) {
                *cursorRow = itemCount - 1;
            } else {
                *cursorRow = visibleRows - 1;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        } else if (visibleRows < itemCount) {
            *scrollOffset = itemCount - visibleRows;
            *cursorRow = visibleRows - 1;
        } else {
            *cursorRow = itemCount - 1;
        }
    } else if (dds3PadButtonState[backwardInput + padSet * 16] & 2) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        }
    }
    forwardState = dds3PadButtonState[forwardInput + padSet * 16];
    if (forwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            } else {
                *cursorRow = 0;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        } else {
            *scrollOffset = 0;
            *cursorRow = 0;
        }
    } else if (dds3PadButtonState[forwardInput + padSet * 16] & 2) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        }
    }
}

s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor) {
    SelectSub(primaryScroll, primaryCursor, primaryCount, primaryVisible, padSet, 7, 6);
    SelectSub(secondaryScroll, secondaryCursor, secondaryCount, secondaryVisible, padSet, 5, 4);
    if (padSet == 0) {
        if (dds3PadButtonState[1] < 0) {
            return 1;
        }
        if (dds3PadButtonState[3] < 0) {
            return -1;
        }
    } else {
        if (dds3PadButtonState[0x11] < 0) {
            return 1;
        }
        if (dds3PadButtonState[0x13] < 0) {
            return -1;
        }
    }
    return 0;
}

void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}
