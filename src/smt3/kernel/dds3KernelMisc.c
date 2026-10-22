#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern s8 dds3PadButtonState[64];

extern u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

void SelectSub(s32 *pbase, s32 *pm, s32 list, s32 win, s32 pad, s32 pluskey, s32 minuskey) {
    s32 backwardState;
    s32 forwardState;

    if (pm == NULL) {
        return;
    }
    if (list < 2) {
        return;
    }
    backwardState = dds3PadButtonState[minuskey + pad * 16];
    if (backwardState < 0) {
        if (pbase == NULL) {
            if (*pm > 0) {
                (*pm)--;
            } else if (list < win) {
                *pm = list - 1;
            } else {
                *pm = win - 1;
            }
        } else if (*pm > 0) {
            (*pm)--;
        } else if (*pbase > 0) {
            (*pbase)--;
        } else if (win < list) {
            *pbase = list - win;
            *pm = win - 1;
        } else {
            *pm = list - 1;
        }
    } else if (dds3PadButtonState[minuskey + pad * 16] & 2) {
        if (pbase == NULL) {
            if (*pm > 0) {
                (*pm)--;
            }
        } else if (*pm > 0) {
            (*pm)--;
        } else if (*pbase > 0) {
            (*pbase)--;
        }
    }
    forwardState = dds3PadButtonState[pluskey + pad * 16];
    if (forwardState < 0) {
        if (pbase == NULL) {
            if (*pm < win - 1) {
                (*pm)++;
            } else {
                *pm = 0;
            }
        } else if (*pm < win - 1) {
            (*pm)++;
        } else if (*pbase + win < list) {
            (*pbase)++;
        } else {
            *pbase = 0;
            *pm = 0;
        }
    } else if (dds3PadButtonState[pluskey + pad * 16] & 2) {
        if (pbase == NULL) {
            if (*pm < win - 1) {
                (*pm)++;
            }
        } else if (*pm < win - 1) {
            (*pm)++;
        } else if (*pbase + win < list) {
            (*pbase)++;
        }
    }
}

s32 dds3SelectMenu(s32 pad, s32 listsx, s32 listsy, s32 winsx, s32 winsy, s32 *basex, s32 *basey, s32 *x, s32 *y) {
    SelectSub(basey, y, listsy, winsy, pad, 7, 6);
    SelectSub(basex, x, listsx, winsx, pad, 5, 4);
    if (pad == 0) {
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

void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(tag, (u32)dds3PrimFrameRect(x * 0x10 + 0x6FD0, y * 8 + 0x78E8, 0xFEFFFF,
                                           sx * columnSpan + rowSpan, sy * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z) {
    s32 columnSpan = 0xC0, rowSpan = 0x60;
    sdfDrawTagLink(tag, (u32)dds3PrimFrameRect(x * 0x10 + 0x6FD0, y * 8 + 0x78E8, z,
                                           sx * columnSpan + rowSpan, sy * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}
