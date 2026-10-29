#include "common.h"
#include "basic.h"
#include "event.h"
#include "effect.h"
#include "kernel.h"
#include "ps2asm.h"

extern char D_0054F9D8[];
extern char D_0054F9E0[];
extern char D_0054F9E8[];
extern char D_0054F9F0[];
extern char D_0054F9B0[]; /* "     %d" */
/* Viewer timeline key; the viewer links these on a group's child list. */

/* Group prefix: the viewer walks the group chain and each group's keys. */


extern evtViewerInfo_t *viewerinfo;
extern s32 (*D_0032CB48[])(s32, s32);
void evtFrameListItemSub(void *tag, s32 ox, s32 oy, s32 co, evtFrameInfo_t *f);
extern s32 mfilterRun(void);
extern s32 evtViewerCheckDirty(void);


extern char D_0054F9B8[];

/* Shared by the float and integer menus after the preceding menu label. */

extern u16 evt_file_minor;

extern u16 D_0054F824;

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern s32 sceWrite(s32 a, const void *b, s32 c);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

extern void *D_002F6978[];

extern void *dds3FontReqAllocDrawTag(void);

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);

extern s32 strlen(const char *s);

extern s32 evtMenuDisp_cnt;

extern char D_0054F9A0[];

extern char D_0054F9A8[];

typedef s32 (*evtMenuHead_t)(void *list, s32 x, s32 y);
typedef void (*evtMenuItem_t)(void *list, s32 x, s32 y, s32 index);

void evtMenuDisp(void *tag, s32 x, s32 y, s32 sx, s32 sy,
                 s32 baseitem, s32 maxitem, void *head, void *item);

extern char D_0054F9C0[];

extern char D_0054F9C8[];

extern char D_0054F9F8[];

extern s8 dds3PadButtonState[];

extern void evtViewerStopFrame(void);

extern void evtRunFrame(s32 nowframe);

extern char D_0054F9D0[];

extern s8 D_0032CAD0[][12];



extern char D_0054FA90[];

extern char D_0054FA98[];

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

extern evtFrameInfo_t *evtGetSelectFrameInfo(void);

/* A signed column count followed by eight signed column kinds, ten bytes per track. */
typedef struct evtFrameColumns_t {
    s16 count;
    s8 columns[8];
} evtFrameColumns_t;
extern evtFrameColumns_t evtFrameList[];

extern char *frameheadstr[];

extern s8 framelist_xcnt[];

s32 evtFrameListHead(void *tag, s32 ox, s32 oy);
void evtFrameListItem(void *tag, s32 ox, s32 oy, s32 no);

extern s32 func_001AD6E8(void);

extern s32 mblurProcess(void);

extern s32 dblurProcess(void);

extern s32 rblurProcess(void);

extern s32 dds3KillProcess(void *id, s32 shutdowntimer);

extern void evtFreeViewerInfo(void);

extern void evtFreeStage(void);

extern void evtUnRegistUnit(void);

extern void evtInitViewerInfo(void);

extern void evtFreePolygonMovieWorkData(void *wdata);

extern void *evtGetPolygonMovieWorkData2(s32 eventno, s32 cutno, s32 type);

extern s32 evtGetViewerObjectTypeCount(s32 type);

extern s32 evtGetAllFrameInfoCount(void);

extern u8 sdfHddValid;

extern char *sdfFileGetHostRoot(void);

extern s32 printf(const char *fmt, ...);

extern s32 sceOpen();

extern s32 sceClose(s32 a);

extern s32 sceSync(const char *a, s32 b);







extern char D_0054FAA0[];

extern char D_0054FAA8[];

void evtBasicListItem(void *tag, s32 ox, s32 oy, s32 no);

void evtEventPosItem(void *tag, s32 ox, s32 oy, s32 no);
s32 evtFileName2Head(void *tag, s32 ox, s32 oy);
void evtFileName2Item(void *tag, s32 ox, s32 oy, s32 no);
s32 evtFileNameHead(void *tag, s32 ox, s32 oy);
void evtFileNameItem(void *tag, s32 ox, s32 oy, s32 no);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);

void evtObjectListSub(void *tag, evtViewerObject_s *obj, s32 ox, s32 oy);
void evtObjectListItem(void *tag, s32 ox, s32 oy, s32 no);

s32 evtSelectHead(void *tag, s32 ox, s32 oy);
void evtSelectItem(void *tag, s32 ox, s32 oy, s32 no);
void evtMainMenuItem(void *tag, s32 ox, s32 oy, s32 no);

/* DDS1 evtDrawMenuFrame: marker offsets are grid units before GS conversion. */
void evtMenuDisp(void *tag, s32 x, s32 y, s32 sx, s32 sy,
                 s32 baseitem, s32 maxitem, void *head, void *item)
{
    s32 used = 0;
    s32 px;
    s32 py;
    s32 index;

    dds3ConsPanelTagLink(tag, x, y, sx, sy);
    /* GS fixed-point encoding; unsigned shifts also handle negative offsets. */
    px = ((u32)x << 4) + 0x7000;
    py = ((u32)y << 3) + 0x7900;
    if (head != NULL) {
        s32 headerRows = ((evtMenuHead_t)head)(tag, px, py);

        py += headerRows * 0x60;
        used = headerRows;
    }
    for (index = baseitem; used < sy; used++) {
        if (item != NULL) {
            ((evtMenuItem_t)item)(tag, px, py, index);
            py += 0x60;
            index++;
        }
    }
    if (evtMenuDisp_cnt & 0x10) {
        py = ((u32)(y - 10) << 3) + 0x7900;
        px = ((u32)(x + sx * 5 + 6) << 4) + 0x7000;
        if (baseitem > 0) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, D_0054F9A0));
        }
        py = ((u32)(y + sy * 12 - 2) << 3) + 0x7900;
        if (index < maxitem) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, D_0054F9A8));
        }
    }
    evtMenuDisp_cnt++;
}

s32 evtFloatChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

void evtFloatChangeItem(void *tag, s32 ox, s32 oy, s32 no)
{
    switch (no) {
    case 0:
        /* The f64 cast is the fptodp libcall, and that libcall is what keeps
         * the format string in a call-saved register across it. */
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 4, "     %.1f",
                                               (f64)viewerinfo->fvalue));
        return;
    case 2:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " L,R = VALUE-+0.1"));
        return;
    case 3:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " U,D = VALUE-+1.0"));
        return;
    case 4:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}
s32 evtFloatChangeMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    f32 step;

    evtMenuDisp(list, x, y, 0x16, 9, 0, 1, evtFloatChangeHead, evtFloatChangeItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 8) {
        return 0;
    }
    if (dds3PadButtonState[0x21] < 0) {
        return 1;
    }
    if (dds3PadButtonState[0x23] < 0) {
        return -1;
    }
    step = 0.0f;
    if ((u8)dds3PadButtonState[0x24] & 2) {
        step = -0.1f;
    } else if ((u8)dds3PadButtonState[0x25] & 2) {
        step = 0.1f;
    }
    if ((u8)dds3PadButtonState[0x26] & 2) {
        step = -1.0f;
    } else if ((u8)dds3PadButtonState[0x27] & 2) {
        step = 1.0f;
    }
    viewerinfo->fvalue += step;
    if (viewerinfo->fvalue < viewerinfo->floatMin) {
        viewerinfo->fvalue = viewerinfo->floatMin;
    }
    if (viewerinfo->fvalue >= viewerinfo->floatMax) {
        viewerinfo->fvalue = viewerinfo->floatMax;
    }
    return 0;
}

s32 evtIntChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

void evtIntChangeItem(void *tag, s32 ox, s32 oy, s32 no)
{
    switch (no) {
    case 0:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 4, D_0054F9B0, viewerinfo->value));
        return;
    case 2:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " L,R = VALUE-+"));
        return;
    case 3:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}
s32 evtIntChangeMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 step;

    evtMenuDisp(list, x, y, 0x16, 9, 0, 1, evtIntChangeHead, evtIntChangeItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 7) {
        return 0;
    }
    if (dds3PadButtonState[0x21] < 0) {
        return 1;
    }
    if (dds3PadButtonState[0x23] < 0) {
        return -1;
    }
    if ((u8)dds3PadButtonState[0x24] & 2) {
        step = -1;
    } else if ((u8)dds3PadButtonState[0x25] & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if ((u8)dds3PadButtonState[0x26] & 2) {
        step = -10;
    } else if ((u8)dds3PadButtonState[0x27] & 2) {
        step = 10;
    }
    viewerinfo->value += step;
    if (viewerinfo->value < viewerinfo->min) {
        viewerinfo->value = viewerinfo->min;
    }
    if (viewerinfo->value >= viewerinfo->max) {
        viewerinfo->value = viewerinfo->max;
    }
    return 0;
}

s32 evtFrameChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtFrameChangeItem(void *tag, s32 ox, s32 oy, s32 no)
{
    switch (no) {
    case 0:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 4, D_0054F9B0, viewerinfo->value));
        return;
    case 2:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RD  = CANCEL"));
        return;
    case 6:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 7:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 8:
        break;
    }
}
s32 evtFrameChangeMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 step;

    evtMenuDisp(list, x, y, 0x16, 0xB, 0, 1, evtFrameChangeHead, evtFrameChangeItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 6) {
        return 0;
    }
    if (dds3PadButtonState[0x21] < 0) {
        return 1;
    }
    if (dds3PadButtonState[0x23] < 0) {
        return -1;
    }
    if ((u8)dds3PadButtonState[0x24] & 2) {
        step = -1;
    } else if ((u8)dds3PadButtonState[0x25] & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if ((u8)dds3PadButtonState[0x26] & 2) {
        step = -10;
    } else if ((u8)dds3PadButtonState[0x27] & 2) {
        step = 10;
    }
    if (dds3PadButtonState[0x20] < 0) {
        step = viewerinfo->nowframe - viewerinfo->value;
    }
    viewerinfo->value += step;
    if (viewerinfo->value < 0) {
        viewerinfo->value = 0;
    }
    if (viewerinfo->value >= viewerinfo->totalframe) {
        viewerinfo->value = viewerinfo->totalframe - 1;
    }
    if (dds3PadButtonState[0x2C] != 0) {
        if (viewerinfo->nowframe != viewerinfo->value) {
            viewerinfo->nowframe = viewerinfo->value;
            evtViewerStopFrame();
            evtRunFrame(viewerinfo->nowframe);
        }
    }
    return 0;
}

void evtMainMenuItem(void *tag, s32 ox, s32 oy, s32 no)
{
    char names[7][30] = {
        "SAVE PROJ    >>",
        "LOAD PROJ    >>",
        "LOAD NEW PROJ>>",
        "SET START FRAME",
        "SET END   FRAME",
        "SET TOTAL FRAME",
        "CAPTURE        ",
    };
    s32 color;

    if (no < 7) {
        if (viewerinfo->fileMode == no) {
            color = viewerinfo->nowmenu == 1 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9B8, names[no]));
    }
}

s32 evtMainMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();

    evtMenuDisp(list, x, y, 0xF, 7, 0, 7, NULL, evtMainMenuItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 1) {
        return 0;
    }
    return dds3SelectMenu(0, 1, 7, 1, 7, NULL, NULL, NULL, &viewerinfo->fileMode);
}

s32 evtSelectHead(void *tag, s32 ox, s32 oy)
{
    char *title = viewerinfo->select_header;

    if (title == NULL) {
        return 0;
    }
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, D_0054F9B8, title));
    return 2;
}
void evtSelectItem(void *tag, s32 ox, s32 oy, s32 no)
{
    s32 color;

    if (no < viewerinfo->menuItemCount) {
        if (viewerinfo->select_y == no) {
            color = viewerinfo->nowmenu == 2 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9C0,
                                               viewerinfo->itemNames[no]));
    }
}

s32 evtSelectMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 width = 10;

    if (viewerinfo->select_header != NULL) {
        width = strlen(viewerinfo->select_header);
        if (width < 6) {
            width = 6;
        }
    }
    evtMenuDisp(list, x, y, width, viewerinfo->menuItemCount + 3, 0, viewerinfo->menuItemCount,
                evtSelectHead, evtSelectItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 2) {
        return 0;
    }
    return dds3SelectMenu(0, 1, viewerinfo->menuItemCount, 1, viewerinfo->menuItemCount,
                          NULL, NULL, NULL, &viewerinfo->select_y);
}

s32 evtFileNameHead(void *tag, s32 ox, s32 oy)
{
    char text[16];

    sprintf(text, D_0054F9C8, (s32)viewerinfo + 0x22D4, (s32)viewerinfo + 0x22E0);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, D_0054F9B8, text));
    return 2;
}

void evtFileNameItem(void *tag, s32 ox, s32 oy, s32 no)
{
    s32 drawX = ox + 0xC0;
    s8 *table = D_0032CAD0[no];
    s32 i = 0;
    s32 color;
    s8 ch;
    s32 cellX;

    do {
        color = 0;
        if (viewerinfo->filename_y == no && viewerinfo->filename_x == i) {
            color = viewerinfo->nowmenu == 3 ? 4 : 5;
        }
        ch = *table++;
        cellX = drawX;
        drawX += 0xC0;
        i++;
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(cellX, oy, 0xFEFFFF, color, D_0054F9D0, ch));
    } while (i < 0xB);
}

s32 evtFileNameMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 input;
    s32 length;
    s32 key;

    evtMenuDisp(list, x, y, 0xE, 6, 0, 4, evtFileNameHead, evtFileNameItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 3) {
        return 0;
    }
    input = dds3SelectMenu(0, 0xB, 4, 0xB, 4, NULL, NULL, &viewerinfo->filename_x,
                           &viewerinfo->filename_y);
    if (input < 0) {
        return -1;
    }
    if (input == 1) {
        key = D_0032CAD0[viewerinfo->filename_y][viewerinfo->filename_x];
        if (viewerinfo->filename_y == 3 && viewerinfo->filename_x >= 4) {
            switch (key) {
            case 'B':
            case 'S':
                for (length = 0; viewerinfo->nameStorage[length] != 0; length++) {
                }
                if (length > 0) {
                    viewerinfo->nameStorage[length - 1] = 0;
                }
                break;
            case 'K':
            case 'O':
                if (viewerinfo->nameStorage[0] != 0) {
                    return 1;
                }
                break;
            }
        } else {
            for (length = 0; viewerinfo->nameStorage[length] != 0; length++) {
            }
            if (length < 8) {
                viewerinfo->nameStorage[length] = key;
                viewerinfo->nameStorage[length + 1] = 0;
            }
        }
    }
    return 0;
}

s32 evtFileName2Head(void *tag, s32 ox, s32 oy)
{
    char name[32];

    sprintf(name, "[E%3d_%03d.PM1+2]", D_0054F824, evt_file_minor);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, D_0054F9B8, name));
    return 2;
}

void evtFileName2Item(void *tag, s32 ox, s32 oy, s32 no)
{
    s32 color = 0;

    if (viewerinfo->filename_y == no) {
        color = 4;
    }
    switch (no) {
    case 0:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0xE, D_0054F9D8));
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox + 0x600, oy, 0xFEFFFF, color, D_0054F9E0, D_0054F824));
        return;
    case 1:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0xE, D_0054F9E8));
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox + 0x600, oy, 0xFEFFFF, color, D_0054F9F0, evt_file_minor));
        return;
    case 3:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " RR  = ENTER"));
        return;
    case 6:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " RD  = CANCEL"));
        return;
    case 7:
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}
s32 evtFileName2Menu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    u32 num;

    evtMenuDisp(list, x, y, 0x11, 0xB, 0, 8, evtFileName2Head, evtFileName2Item);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 9) {
        return 0;
    }
    if (dds3PadButtonState[0x26] < 0) {
        viewerinfo->filename_y ^= 1;
    }
    if (dds3PadButtonState[0x27] < 0) {
        viewerinfo->filename_y ^= 1;
    }
    if ((u8)dds3PadButtonState[0x24] & 2) {
        switch (viewerinfo->filename_y) {
        case 0:
            if (D_0054F824 >= 0x1F5) {
                D_0054F824--;
            }
            break;
        case 1:
            if (evt_file_minor >= 2) {
                evt_file_minor--;
            }
            break;
        }
    }
    if ((u8)dds3PadButtonState[0x25] & 2) {
        switch (viewerinfo->filename_y) {
        case 0:
            if (D_0054F824 < 0x3E7) {
                D_0054F824++;
            }
            break;
        case 1:
            if (evt_file_minor < 0x3E7) {
                evt_file_minor++;
            }
            break;
        }
    }
    if ((u8)dds3PadButtonState[0x28] & 2) {
        switch (viewerinfo->filename_y) {
        case 0:
            if (D_0054F824 >= 0x1F5) {
                num = (u16)(D_0054F824 - 500);
                if (num < 0x1F4) {
                    D_0054F824 = 0x1F4;
                } else {
                    D_0054F824 -= 500;
                }
            }
            break;
        case 1:
            if (evt_file_minor >= 2) {
                num = (u16)(evt_file_minor - 10);
                if (num == 0) {
                    evt_file_minor = 1;
                } else {
                    evt_file_minor -= 10;
                }
            }
            break;
        }
    }
    if ((u8)dds3PadButtonState[0x2A] & 2) {
        switch (viewerinfo->filename_y) {
        case 0:
            D_0054F824 += 100;
            break;
        case 1:
            evt_file_minor += 10;
            break;
        }
    }
    if (dds3PadButtonState[0x21] < 0) {
        return 1;
    }
    if (dds3PadButtonState[0x23] < 0) {
        return -1;
    }
    if (dds3PadButtonState[0x20] < 0) {
        D_0054F824 = 0x258;
    }
    return 0;
}

void evtObjectListSub(void *tag, evtViewerObject_s *obj, s32 ox, s32 oy)
{
    s32 index;

    ox += 0x6C0;
    dds3ConsPanelTagLink(tag, (ox - 0x7000) >> 4, 0x18, 0x1C, 3);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, 0x79C0, 0xFEFFFF, 0xE, D_0054F9F8));
    index = obj->entryHeader;
    if (index < 0) {
        return;
    }
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox + 0x3C0, 0x79C0, 0xFEFFFF, 0, D_0054F9B8,
                                           viewerinfo->nametable[index]));
}

void evtObjectListItem(void *tag, s32 ox, s32 oy, s32 no)
{
    char names[24][30] = {
        "STAGE", "UNIT", "CAMERA", "D3P", "MESSAGE", "SE",
        "FADE", "QUAKE", "BLUR", "LIGHT", "SLIGHT", "SFOG",
        "FF", "BLUR2", "MBLUR", "DBLUR", "FILTER", "MFILTER",
        "BED", "BGM", "MG1", "MG2", "FBOKASI", "RBLUR",
    };
    evtViewerObject_s *group;
    s32 color;
    s32 i;

    if (viewerinfo->objcnt == 0) {
        return;
    }
    if (no < viewerinfo->objcnt) {
        group = viewerinfo->firstobjinfo;
        for (i = 0; i < no; i++) {
            group = group->next;
        }
        if (viewerinfo->objlist_base + viewerinfo->objlist_y == no) {
            if (viewerinfo->nowmenu == 4) {
                viewerinfo->framelist_x = 0;
                viewerinfo->frameFirst = 0;
                viewerinfo->frameCursor = 0;
                viewerinfo->framelist_target = group;
                color = 4;
            } else {
                color = 5;
            }
            evtObjectListSub(tag, group, ox, oy);
        } else {
            color = 0;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9B8,
                                               names[group->type]));
    }
}

s32 evtOjbectListMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 visible;

    evtMenuDisp(list, x, y, 8, 0x1D, viewerinfo->objlist_base, viewerinfo->objcnt,
                NULL, evtObjectListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 4) {
        return 0;
    }
    if (viewerinfo->objcnt == 0) {
        return 0;
    }
    if (viewerinfo->objcnt < 0x1D) {
        visible = viewerinfo->objcnt;
    } else {
        visible = 0x1D;
    }
    return dds3SelectMenu(0, 1, viewerinfo->objcnt, 1, visible, NULL,
                          &viewerinfo->objlist_base, NULL, &viewerinfo->objlist_y);
}

/* NON_MATCHING: 39 of 78 words differ (first at +0x20). */
#ifdef NON_MATCHING
s32 evtFrameListHead(void *tag, s32 ox, s32 oy)
{
    s32 type = viewerinfo->framelist_target->type;
    s32 i;

    for (i = 0; i < evtFrameList[type].count; i++) {
        s32 w = framelist_xcnt[evtFrameList[type].columns[i]] * 0xC0;

        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0xE, D_0054F9B8,
                                               frameheadstr[evtFrameList[type].columns[i]]));
        ox += w;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListHead);
#endif

extern char D_0054FA40[], D_0054FA48[], D_0054FA50[], D_0054FA58[];
extern char D_0054FA60[], D_0054FA68[], D_0054FA70[], D_0054FA78[];
extern char D_0054FA80[], D_0054FA88[];
extern char *evtItemFadeNames[];
extern char *D_0032CB30[];
extern void *memcpy(void *, const void *, u32);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C230);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C240);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C250);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C260);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C270);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C280);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C290);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C2A0);

void evtFrameListItemSub(void *tag, s32 ox, s32 oy, s32 co, evtFrameInfo_t *f)
{
    char name[32];
    s32 type = viewerinfo->framelist_target->type;
    s32 column;
    s32 color;
    /* Unknown track/kind combinations retain the preceding column value. */
    s32 value = 0;

    for (column = 0; column < evtFrameList[type].count; column++) {
        color = viewerinfo->framelist_x == column ? co : 0;
        switch (evtFrameList[type].columns[column]) {
        case 0:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, f->frame));
            break;
        case 9:
            switch (type) {
            case 18: value = f->body.shorts[0]; break;
            case 3: value = f->body.shorts[1]; break;
            case 20: case 21: value = f->body.shorts[1]; break;
            }
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, value));
            break;
        case 15:
            switch (type) {
            case 3: case 20: case 21: value = f->index; break;
            }
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, value));
            break;
        case 1:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, f->index));
            break;
        case 13:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA48));
            break;
        case 14:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, "%.1f %.2f", (f64)f->body.values[0], (f64)f->body.values[1]));
            break;
        case 2:
            if (type == 7) value = f->body.shorts[0];
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, value));
            break;
        case 3:
            switch (type) {
            case 8: value = f->body.shorts[0]; break;
            case 12: value = f->body.shorts[1]; break;
            }
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, value));
            break;
        case 4:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, f->body.shorts[1]));
            break;
        case 5:
            switch (type) {
            case 8: value = f->body.shorts[2]; break;
            case 10: value = f->nameIndex; break;
            case 11: case 13: case 14: case 15: case 16: case 17: case 23: value = f->body.shorts[0]; break;
            }
            if (value != 0) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA50));
            } else {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA58));
            }
            break;
        case 12:
            switch (type) {
            case 12:
                switch (f->body.shorts[0]) {
                case 0: sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA60)); break;
                case 1: sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA68)); break;
                case 2: sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA70)); break;
                }
                break;
            case 6:
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9B8, evtItemFadeNames[f->body.shorts[0]]));
                break;
            case 19:
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9B8, evtItemFadeNames[4 + f->body.shorts[0]]));
                break;
            case 1:
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054F9B8, D_0032CB30[f->body.bytes[0]]));
                break;
            }
            break;
        case 11:
            switch (type) {
            case 22: value = f->body.shorts[0]; break;
            case 9: value = f->body.shorts[1]; break;
            }
            if (value != 0) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA50));
            } else {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA58));
            }
            break;
        case 16:
            switch (type) {
            case 20: case 21: value = f->body.bytes[1]; break;
            case 3: value = f->body.bytes[4]; break;
            }
            if (value != 0) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA50));
            } else {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA58));
            }
            break;
        case 6:
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA78, (f64)(f->body.values[0] * 57.2957763671875f)));
            break;
        case 8:
            switch (type) {
            case 5: value = f->body.shorts[0]; break;
            case 19: value = f->body.shorts[1]; break;
            case 3: value = f->body.bytes[5]; break;
            case 12: value = f->body.words[1]; break;
            case 18: value = f->body.bytes[4]; break;
            }
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA80, value));
            break;
        case 7:
            if (type == 5) value = f->body.shorts[1];
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA40, value));
            break;
        case 10:
            switch (type) {
            case 3: value = f->body.shorts[3]; break;
            case 18: value = f->body.shorts[1]; break;
            case 9: value = f->body.shorts[0]; break;
            }
            if (value < 0) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, "-----------"));
            } else {
                memcpy(name, viewerinfo->nametable[value], sizeof(name));
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA88, name));
            }
            break;
        }
        ox += framelist_xcnt[evtFrameList[type].columns[column]] * 0xC0;
    }
}


void evtFrameListItem(void *tag, s32 ox, s32 oy, s32 no)
{
    evtFrameInfo_t *key;
    s32 color;
    s32 i;

    if (no < viewerinfo->framelist_target->childCount + 1) {
        key = NULL;
        if (no != viewerinfo->framelist_target->childCount) {
            key = viewerinfo->framelist_target->children;
            for (i = 0; i < no; i++) {
                key = key->next;
            }
        }
        if (viewerinfo->frameCursor + viewerinfo->frameFirst == no) {
            if (viewerinfo->nowmenu == 5) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        if (key != NULL) {
            evtFrameListItemSub(tag, ox, oy, color, key);
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
        /* Post-call own-state reset (docs/CONTRIBUTING.md): close the real
         * traversal cursor after either render tail. Stock RTL keeps UID228
         * through sibling/CFG and deletes it in life; native retains JALs
         * at 0x1F71F8/0x1F7230. No native reset store is claimed. */
        key = NULL;
    }
}
s32 evtFrameListMenu(s32 x, s32 y)
{
    evtViewerObject_s *group;
    void *list;
    s32 visible;
    s32 ret;

    if (viewerinfo->objcnt == 0) {
        return 0;
    }
    group = viewerinfo->framelist_target;
    if (evtFrameList[group->type].count == 0) {
        return -1;
    }
    list = dds3FontReqAllocDrawTag();
    evtMenuDisp(list, x, y, 0x1C, 0x19, viewerinfo->frameCursor, group->childCount + 1,
                evtFrameListHead, evtFrameListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 5) {
        return 0;
    }
    if (group->childCount + 1 < 0x18) {
        visible = group->childCount + 1;
    } else {
        visible = 0x18;
    }
    ret = dds3SelectMenu(0, evtFrameList[group->type].count, group->childCount + 1,
                         evtFrameList[group->type].count, visible, NULL,
                         &viewerinfo->frameCursor, &viewerinfo->framelist_x,
                         &viewerinfo->frameFirst);
    if (dds3PadButtonState[0x20] < 0) {
        ret = -2;
    }
    if (dds3PadButtonState[0x2C] != 0) {
        if (viewerinfo->frameCursor + viewerinfo->frameFirst < group->childCount) {
            evtFrameInfo_t *key = evtGetSelectFrameInfo();

            if (viewerinfo->nowframe != key->frame) {
                viewerinfo->nowframe = key->frame;
                evtViewerStopFrame();
                evtRunFrame(viewerinfo->nowframe);
            }
        }
    }
    return ret;
}

void evtBasicListItem(void *tag, s32 ox, s32 oy, s32 no)
{
    s32 color;
    s32 count;
    s32 i;
    dds3Basic_t *basic;

    color = 4;
    if (viewerinfo->work_base + viewerinfo->work_y != no) {
        color = 0;
    }
    if (no == 0) {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != 3) {
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].firstbasic;
                 basic != NULL; basic = basic->next) {
                if (basic->name != NULL) {
                    count++;
                    if (count == no) {
                        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color,
                                                               D_0054F9C0, basic->name));
                        return;
                    }
                }
            }
        }
    }
}

s32 evtBasicListMenu(s32 x, s32 y)
{
    s32 i;
    s32 count = 0;
    void *list;
    s32 shown = 0x17;
    dds3Basic_t *basic;

    list = dds3FontReqAllocDrawTag();
    for (i = 0; i < 0x12; i++) {
        if (i != 3) {
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].firstbasic;
                 basic != NULL; basic = basic->next) {
                if (basic->name != NULL) {
                    count++;
                }
            }
        }
    }
    count++;
    evtMenuDisp(list, x, y, 0x1A, 0x17, viewerinfo->work_base, count, NULL, evtBasicListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 0xA) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, NULL, &viewerinfo->work_base, NULL,
                          &viewerinfo->work_y);
}

void evtEventPosItem(void *tag, s32 ox, s32 oy, s32 no)
{
    dds3Basic_t *basic;
    s32 color;
    s32 slot;

    basic = NULL;
    color = 4;
    if (viewerinfo->eventpos_y != no) {
        color = 0;
    }
    slot = ((s8 *)((u8 *)no + (u32)evtGetSelectFrameInfo()))[0xC];
    if (slot >= 0) {
        basic = dds3SceneBasicSearchName(dds3GetActiveScene(), viewerinfo->nametable[slot]);
    }
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0xE, D_0054FA90, no));
    if (basic != NULL) {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, D_0054FA98, basic->name));
    } else {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, "   -----------------------"));
    }
}

s32 evtEventPosMenu(s32 x, s32 y)
{
    void *list = dds3FontReqAllocDrawTag();
    s32 rows;

    switch (viewerinfo->framelist_target->type) {
    case 0x14:
        rows = 2;
        break;
    case 0x15:
        rows = 4;
        break;
    default:
        return -1;
    }
    evtMenuDisp(list, x, y, 0x1C, rows, 0, rows, NULL, evtEventPosItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->nowmenu != 0xC) {
        return 0;
    }
    return dds3SelectMenu(0, 1, rows, 1, rows, NULL, NULL, NULL, &viewerinfo->eventpos_y);
}

s32 mfilter_Process(void)
{
    if (mfilterRun() == 0) {
        viewerinfo->menuProcess = 0;
        return -1;
    }
    return 0;
}
s32 evtViewerExit(void)
{
    if (evtViewerCheckDirty() == 0) {
        viewerinfo->menuProcess = 0;
        return -1;
    }
    return 0;
}
/* NON_MATCHING: 20 of 56 words differ (first at +0x4). */
#ifdef NON_MATCHING
s32 func_001F79A8(void)
{
    s32 busy = 1;

    switch (viewerinfo->framelist_target->type) {
    case 0xD:
        busy = func_001AD6E8();
        break;
    case 0xE:
        busy = mblurProcess();
        break;
    case 0xF:
        busy = dblurProcess();
        break;
    case 0x17:
        busy = rblurProcess();
        break;
    case 0x10:
    case 0x11:
        if (viewerinfo->menuProcess == 0) {
            return 1;
        }
        break;
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    default:
        busy = dds3PadButtonState[0x2C] >= 0;
        break;
    }
    if (busy != 0) {
        return 0;
    }
    if (viewerinfo->menuProcess == 0) {
        return 1;
    }
    dds3KillProcess((void *)viewerinfo->menuProcess, 1);
    viewerinfo->menuProcess = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", func_001F79A8);
#endif

s32 evtViewerDispatch(s32 actionIndex, s32 x, s32 y)
{
    s32 mode = viewerinfo->nowmenu;

    if (mode == 11 && actionIndex != mode) {
        return 0;
    }
    return D_0032CB48[actionIndex](x, y);
}
s32 evtWriteRecord(s32 fd, const void *buffer, s32 size)
{
    return sceWrite(fd, buffer, size);
}

s32 evtCountObjType10(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->index = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType11(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->index = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType13(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType14(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType15(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType23(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType16(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
s32 evtCountObjType17(void)
{
    s32 childIndex = 0;
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                key->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
typedef struct {
    u8 reserved00;
    u8 reserved01;
    u16 reserved02;
    s32 byteLength;                 /* 0x04 */
    char tag[4];                    /* 0x08 */
    s32 reserved0C;
    s32 count;                      /* 0x10 */
    s32 kind;                       /* 0x14 */
    s32 reserved18;
    s32 reserved1C;
} evtViewerFileHeader_t;

typedef struct {
    s32 type;
    s32 elementSize;                /* 0x04 */
    s32 value;                      /* 0x08 */
    s32 offset;                     /* 0x0C */
} evtViewerDirEntry_t;

/* NON_MATCHING: 2 of 256 words differ (first at +0x1B4). */
#ifdef NON_MATCHING
void evtSaveViewerHeader(s32 fd, s32 mode)
{
    evtViewerFileHeader_t header;
    evtViewerDirEntry_t entry;
    s32 i;
    s32 payloadOffset;
    s32 count0;
    s32 count1;
    s32 frameCount;
    s32 frameBytes;
    s32 nameCount;
    s32 nameBytes;
    s32 count2;
    s32 count2Bytes;
    s32 n10;
    s32 n10Bytes;
    s32 n11;
    s32 n11Bytes;
    s32 n13;
    s32 n13Bytes;
    s32 n14;
    s32 n14Bytes;
    s32 n15;
    s32 n15Bytes;
    s32 n16;
    s32 n16Bytes;
    s32 n17;
    s32 n17Bytes;
    s32 n23;
    s32 n23Bytes;
    s32 byteLength;

    if (mode != 2) {
        return;
    }
    payloadOffset = 0xE0;
    count0 = evtGetViewerObjectTypeCount(0);
    count1 = evtGetViewerObjectTypeCount(1);
    frameCount = evtGetAllFrameInfoCount();
    frameBytes = frameCount * 0x10;
    nameCount = viewerinfo->nametablecnt;
    nameBytes = nameCount * 0x20;
    count2 = evtGetViewerObjectTypeCount(2);
    count2Bytes = count2 * 0x10;
    n10 = evtCountObjType10();
    n10Bytes = n10 * 0x30;
    n11 = evtCountObjType11();
    n11Bytes = n11 * 0x20;
    n13 = evtCountObjType13();
    n13Bytes = n13 * 0x28;
    n14 = evtCountObjType14();
    n14Bytes = n14 * 0x2C;
    n15 = evtCountObjType15();
    n15Bytes = n15 * 0x2C;
    n16 = evtCountObjType16();
    n16Bytes = n16 * 0x18;
    n17 = evtCountObjType17();
    n17Bytes = n17 * 0x24;
    n23 = evtCountObjType23();
    n23Bytes = n23 * 0x2C;

    byteLength = nameBytes + 0xF0;
    byteLength += frameBytes;
    byteLength += count2Bytes;
    byteLength += n10Bytes;
    byteLength += n11Bytes;
    byteLength += n13Bytes;
    byteLength += n14Bytes;
    byteLength += n15Bytes;
    byteLength += n16Bytes;
    byteLength += n17Bytes;
    byteLength += n23Bytes;

    header.reserved00 = 0;
    header.reserved01 = 0;
    header.reserved02 = 0;
    header.count = 12;
    header.tag[0] = 'P';
    header.tag[1] = 'M';
    header.tag[2] = 'D';
    header.tag[3] = '2';
    header.byteLength = byteLength;
    header.kind = 4;
    header.reserved0C = 0;
    header.reserved18 = 0;
    header.reserved1C = 0;
    evtWriteRecord(fd, &header, 0x20);

    for (i = 0; i < 0x15; i++) {
        entry.type = i;
        entry.elementSize = 0;
        entry.value = 0;
        entry.offset = 0;
        switch (i) {
        case 0:
            if (mode == 2) {
                entry.elementSize = 0x10;
                entry.value = 1;
            }
            break;
        case 2:
            if (mode == 1) {
                entry.elementSize = 0x10;
                entry.value = count0;
            }
            break;
        case 3:
            if (mode == 1) {
                entry.elementSize = 0x20;
                entry.value = count1;
            }
            break;
        case 4:
            if (mode == 2) {
                entry.elementSize = 0x10;
                entry.value = frameCount;
            }
            break;
        case 1:
            entry.elementSize = 0x20;
            entry.value = nameCount;
            break;
        case 5:
            if (mode == 2) {
                entry.elementSize = 0x10;
                entry.value = count2;
            }
            break;
        case 13:
            if (mode == 2) {
                entry.elementSize = 0x30;
                entry.value = n10;
            }
            break;
        case 14:
            if (mode == 2) {
                entry.elementSize = 0x20;
                entry.value = n11;
            }
            break;
        case 15:
            if (mode == 2) {
                entry.elementSize = 0x28;
                entry.value = n13;
            }
            break;
        case 16:
            if (mode == 2) {
                entry.elementSize = 0x2C;
                entry.value = n14;
            }
            break;
        case 17:
            if (mode == 2) {
                entry.elementSize = 0x2C;
                entry.value = n15;
            }
            break;
        case 20:
            if (mode == 2) {
                entry.elementSize = 0x2C;
                entry.value = n23;
            }
            break;
        case 18:
            if (mode == 2) {
                entry.elementSize = 0x18;
                entry.value = n16;
            }
            break;
        case 19:
            if (mode == 2) {
                entry.elementSize = 0x24;
                entry.value = n17;
            }
            break;
        }
        if (entry.elementSize != 0 || entry.value != 0) {
            entry.offset = payloadOffset;
            payloadOffset += entry.elementSize * entry.value;
            evtWriteRecord(fd, &entry, 0x10);
        }
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveViewerHeader);
#endif

void evtSaveViewerInfo(s32 fd)
{
    s32 header[4];

    header[0] = viewerinfo->firstframe;
    header[1] = viewerinfo->lastframe;
    header[2] = viewerinfo->totalframe;
    header[3] = 0;
    evtWriteRecord(fd, header, 0x10);
}
typedef struct {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    s32 body[2];
} evtSaveRecord_t;

void evtSaveFrameList(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        evtFrameInfo_t *key;

        for (key = group->children; key != NULL; key = key->next) {
            evtSaveRecord_t record;
            s32 i;
            u16 value;

            value = (u16)key->nameIndex;
            record.groupType = group->type;
            record.start = key->frame;
            record.span = key->index;
            record.value = value;
            for (i = 0; i < 2; i++) {
                record.body[i] = key->body.words[i];
            }
            if (group->type == 8) {
                if (key->body.words[0] == 0) {
                    record.span = 0;
                } else {
                    if (key->next != NULL) {
                        record.span = key->next->frame - key->frame;
                    } else {
                        record.span = (u16)viewerinfo->lastframe - key->frame;
                    }
                }
            }
            evtWriteRecord(fd, &record, 0x10);
        }
    }
}

void evtSaveObjectList(s32 fd)
{
    s32 i;

    for (i = 0; i < viewerinfo->nametablecnt; i++) {
        evtWriteRecord(fd, viewerinfo->nametable[i], 0x20);
    }
}
void evtSaveObjType2(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 2) {
            s32 header[4];
            header[0] = group->entryHeader;
            header[1] = 0;
            header[2] = 0;
            header[3] = 0;
            evtWriteRecord(fd, header, 0x10);
        }
    }
}

void evtSaveObjType10(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x30);
            }
        }
    }
}
void evtSaveObjType11(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x20);
            }
        }
    }
}
void evtSaveObjType13(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x28);
            }
        }
    }
}
void evtSaveObjType14(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType15(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType23(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType16(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x18);
            }
        }
    }
}
void evtSaveObjType17(s32 fd)
{
    evtViewerObject_s *group;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtFrameInfo_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x24);
            }
        }
    }
}
s32 evtMakeViewerPath(s32 kind)
{
    char path[0x40];
    evtViewerObject_s *group;
    s32 fd;
    s32 i;

    for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
    }
    if (kind == 0) {
        if (sdfHddValid) {
            sprintf(path, "pfs0:/event/pmvtool/%s.PM2", viewerinfo->filename);
        } else {
            sprintf(path, "%sevent/pmvtool/%s.PM2", sdfFileGetHostRoot(), viewerinfo->filename);
        }
    } else {
        if (sdfHddValid) {
            sprintf(path, "pfs0:/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2", D_0054F824 / 10 * 10, D_0054F824, D_0054F824,
                    evt_file_minor, D_0054F824, evt_file_minor);
        } else {
            sprintf(path, "%sevent/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2", sdfFileGetHostRoot(), D_0054F824 / 10 * 10,
                    D_0054F824, D_0054F824, evt_file_minor, D_0054F824, evt_file_minor);
        }
    }
    if (sdfHddValid) {
        printf("hdd -> %s\n", path);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        printf("pc -> %s\n", path);
        fd = sceOpen(path, 0x602);
    }
    if (fd < 0) {
        printf(D_0054FAA0);
        return 0;
    }
    evtSaveViewerHeader(fd, 2);
    for (i = 0; i < 0x15; i++) {
        switch (i) {
        case 0:
            evtSaveViewerInfo(fd);
            break;
        case 1:
            evtSaveObjectList(fd);
            break;
        case 4:
            evtSaveFrameList(fd);
            break;
        case 5:
            evtSaveObjType2(fd);
            break;
        case 13:
            evtSaveObjType10(fd);
            break;
        case 14:
            evtSaveObjType11(fd);
            break;
        case 15:
            evtSaveObjType13(fd);
            break;
        case 16:
            evtSaveObjType14(fd);
            break;
        case 17:
            evtSaveObjType15(fd);
            break;
        case 18:
            evtSaveObjType16(fd);
            break;
        case 19:
            evtSaveObjType17(fd);
            break;
        case 20:
            evtSaveObjType23(fd);
            break;
        }
    }
    sceClose(fd);
    sceSync(D_0054FAA8, 0);
    return 1;
}

/* PM1/PM2 layouts and pointers follow evtGetPolygonMovieWorkData1. */
typedef struct pmdCutInfo_t {
    s32 FirstFrame;
    s32 LastFrame;
    s32 TotalFrame;
    s32 reserve;
} pmdCutInfo_t;



typedef struct pmdFrameTable_t pmdFrameTable_t;



typedef struct pmdEffectTable_t {
    s32 NameIndex;
    s32 DataOffset;
    s32 Type;
    s32 reserve;
} pmdEffectTable_t;

typedef struct pmdEntry_t {
    u32 type;
    u32 elementSize;
    u32 value;
    u32 offset;
} pmdEntry_t;

typedef struct pmdHeader_t {
    u8 pad[0x10];
    s32 count;
    s32 kind;
    u8 pad2[8];
    pmdEntry_t entries[1];
} pmdHeader_t;

typedef struct evtPolygonMovieWork_t {
    u32 movieFlags;
    void *file04;
    sdfMemHandle_t *mem08;
    void *pm1data;
    void *header1;
    pmdEntry_t *entryTable1;
    char (*nameTable1)[32];
    s32 nameCount1;
    pmdStageTable_t *stageTable;
    s32 stageCount;
    void *stageF1;
    void *stageF2;
    void *stageTB;
    pmdUnitTable_t *unitTable;
    s32 unitCount;
    void *type9Data;
    pmdEffectTable_t *effectTable;
    s32 effectCount;
    void *type8Data;
    void *file4C;
    sdfMemHandle_t *mem50;
    void *pm2data;
    pmdHeader_t *header2;
    pmdEntry_t *entryTable2;
    char (*nameTable)[32];
    s32 nameCount2;
    pmdCutInfo_t *cutInfo;
    pmdFrameTable_t *frameTable;
    s32 frameCount;
    void *cameraTable;
    s32 cameraCount;
    pmdSceneLightTable_t *slightTable;
    s32 slightCount;
    pmdSceneFogTable_t *sfogTable;
    s32 sfogCount;
    dds3Effect_Blur_t *blur2Table;
    s32 blur2Count;
    dds3Effect_PartialMultiBlurParam_t *mblurTable;
    s32 mblurCount;
    dds3Effect_DistortionBlurParam_t *dblurTable;
    s32 dblurCount;
    dds3Effect_Filter_t *filterTable;
    s32 filterCount;
    dds3Effect_PartialMultiFilterParam_t *mfilterTable;
    s32 mfilterCount;
    dds3Effect_RippleBlurParam_t *rblurTable;
    s32 rblurCount;
    s32 mesHandle;
    dds3ProcessID_t *processId;
    s32 eventNo;
    s32 cutNo;
    s32 nowFrame;
    dds3Basic_t **effectBasicTable;
} evtPolygonMovieWork_t;

typedef struct pmdFrameTable_t {
    u16 ObjectType;
    u16 Frame;
    u16 Length;
    s16 NameIndex;
    union {
        s8 b[8];
        u8 ub[8];
        s16 h[4];
        u16 uh[4];
        s32 w[2];
        f32 f[2];
    } data;
} pmdFrameTable_t;


extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *scene, s32 serial, s32 type);
extern s32 evtLoadArea_FromMemory(s32 fld, s32 area, s32 pF1, s32 pF2, const void *pTB, s32 pBF);
extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
extern s32 evtSetRegistUnit(s32 major, s32 minor);
extern evtUnit_t *evtGetUnitHandle(s32 serial);
extern void mdlSmoothSingleMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);
extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);
extern evtViewerObject_s *evtAddViewerObject(s32 type);
extern s32 evtAddNameTable(const char *name);
/* Native provider only consumes the track and frame, not a third viewer argument. */
extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *group, s32 frame);
extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *camera, f32 fovy);
extern s32 strcmp(const char *a, const char *b);
extern void *memcpy(void *dest, const void *src, u32 size);

s32 evtMakeMotionName(evtPolygonMovieWork_t *work)
{
    char pathName[64];
    s32 i;
    s32 j;
    s32 major;
    s32 minor;
    s32 serial;
    dds3SceneBasic_t *scene;
    dds3Basic_t *linkedBasic;
    dds3Basic_t *basic;
    evtViewerObject_s *group;
    evtFrameInfo_t *key;
    pmdFrameTable_t *frame;

    if (work->stageTable != NULL) {
        major = work->stageTable->MajorNum;
        minor = work->stageTable->MinorNum;
        evtLoadArea_FromMemory(major, minor, (s32)work->stageF1, (s32)work->stageF2, work->stageTB, 0);
        group = evtAddViewerObject(0);
        group->entryHeader = evtAddNameTable(work->nameTable1[work->stageTable->NameIndex]);
        group->resource.major = major;
        group->resource.minor = minor;
    }
    scene = dds3GetControlScene()->workdata;
    for (basic = scene->basictable[4].firstbasic; basic != NULL; basic = basic->next) {
        if (basic->name != NULL) {
            group = evtAddViewerObject(2);
            group->basic = basic;
            group->entryHeader = evtAddNameTable(basic->name);
        }
    }
    for (i = 0; i < work->unitCount; i++) {
        major = work->unitTable[i].MajorNum;
        minor = work->unitTable[i].MinorNum;
        mdlPackDecode(major, minor, 0x101, (u8 *)work->header1 + work->unitTable[i].DataOffset,
                      work->unitTable[i].DataSize);
        serial = evtSetRegistUnit(major, minor);
        group = evtAddViewerObject(1);
        group->basic = dds3SearchSceneBasic(dds3GetControlScene(), serial, 5);
        group->entryHeader = evtAddNameTable(work->nameTable1[work->unitTable[i].NameIndex]);
        group->basic->name = viewerinfo->nametable[group->entryHeader];
        group->resource.major = major;
        group->resource.minor = minor;
        mdlSmoothSingleMotion(evtGetUnitHandle(serial)->owner, 0, 0, 0.0f, 0.0f);
    }
    for (i = 0; i < work->effectCount; i++) {
        switch (work->effectTable[i].Type) {
        case 0: group = evtAddViewerObject(3); break;
        case 1: group = evtAddViewerObject(18); break;
        case 2: group = evtAddViewerObject(20); break;
        case 3: group = evtAddViewerObject(21); break;
        default: continue;
        }
        group->entryHeader = evtAddNameTable(work->nameTable1[work->effectTable[i].NameIndex]);
        group->effect.data = (u8 *)work->header1 + work->effectTable[i].DataOffset;
    }
    for (basic = scene->basictable[9].firstbasic; basic != NULL; basic = basic->next) {
        if (basic->name != NULL) {
            group = evtAddViewerObject(9);
            group->basic = basic;
            group->entryHeader = evtAddNameTable(basic->name);
        }
    }
    viewerinfo->activecamera = NULL;
    for (basic = scene->basictable[16].firstbasic; basic != NULL; basic = basic->next) {
        if (basic->name != NULL) {
            for (linkedBasic = scene->basictable[4].firstbasic; linkedBasic != NULL; linkedBasic = linkedBasic->next) {
                if (linkedBasic->name != NULL) {
                    sprintf(pathName, "%s_MOTION", linkedBasic->name);
                    if (strcmp(basic->name, pathName) == 0) {
                        evtPathLinkAndMake(basic, linkedBasic);
                    }
                }
            }
            for (linkedBasic = scene->basictable[9].firstbasic; linkedBasic != NULL; linkedBasic = linkedBasic->next) {
                if (linkedBasic->name != NULL) {
                    sprintf(pathName, "%s_MOTION", linkedBasic->name);
                    if (strcmp(basic->name, pathName) == 0) {
                        evtPathLinkAndMake(basic, linkedBasic);
                    }
                }
            }
            for (linkedBasic = scene->basictable[7].firstbasic; linkedBasic != NULL; linkedBasic = linkedBasic->next) {
                if (linkedBasic->name != NULL) {
                    sprintf(pathName, "%s_MOTION", linkedBasic->name);
                    if (strcmp(basic->name, pathName) == 0) {
                        evtPathLinkAndMake(basic, linkedBasic);
                    }
                }
            }
        }
    }
    if (work->cutInfo != NULL) {
        viewerinfo->totalframe = work->cutInfo->TotalFrame;
        viewerinfo->firstframe = work->cutInfo->FirstFrame;
        viewerinfo->lastframe = work->cutInfo->LastFrame;
    }
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 1) {
            for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
                if (strcmp(viewerinfo->nametable[group->entryHeader], work->nameTable[frame->NameIndex]) == 0) {
                    key = evtAddFrameInfo(group, frame->Frame);
                    key->body.unsignedBytes[0] = frame->data.ub[0];
                    key->nameIndex = evtAddNameTable(work->nameTable[frame->NameIndex]);
                }
            }
        }
    }
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 2) {
            for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
                if (strcmp(viewerinfo->nametable[group->entryHeader], work->nameTable[frame->NameIndex]) == 0) {
                    key = evtAddFrameInfo(group, frame->Frame);
                    key->body.values[0] = frame->data.f[0];
                    key->nameIndex = evtAddNameTable(work->nameTable[frame->NameIndex]);
                    dds3CameraObjectBasicSetFovy(group->basic, key->body.values[0]);
                }
            }
        }
    }
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        switch (frame->ObjectType) {
        case 3:
        case 18:
        case 20:
        case 21:
            for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
                if (strcmp(viewerinfo->nametable[group->entryHeader], work->nameTable[frame->NameIndex]) == 0) {
                    key = evtAddFrameInfo(group, frame->Frame);
                    switch (frame->ObjectType) {
                    case 3:
                        key->body.unsignedShorts[1] = frame->data.uh[1];
                        key->body.unsignedBytes[5] = frame->data.ub[5];
                        if (frame->data.h[3] < 0) {
                            key->body.shorts[3] = -1;
                        } else {
                            key->body.shorts[3] = evtAddNameTable(work->nameTable[frame->data.h[3]]);
                        }
                        key->body.unsignedBytes[4] = frame->data.ub[4];
                        if (work->header2->kind < 4) {
                            key->index = 0;
                        } else {
                            key->index = frame->Length;
                        }
                        break;
                    case 20:
                    case 21:
                        key->body.unsignedShorts[1] = frame->data.uh[1];
                        key->index = frame->Length;
                        for (j = 0; j < 4; j++) {
                            if (frame->data.b[j + 4] < 0) {
                                key->body.bytes[j + 4] = -1;
                            } else {
                                key->body.bytes[j + 4] = evtAddNameTable(work->nameTable[frame->data.b[j + 4]]);
                            }
                        }
                        key->body.unsignedBytes[1] = frame->data.ub[1];
                        break;
                    case 18:
                        key->body.unsignedShorts[0] = frame->data.uh[0];
                        key->body.unsignedBytes[4] = frame->data.ub[4];
                        if (frame->data.h[1] < 0) {
                            key->body.shorts[1] = -1;
                        } else {
                            key->body.shorts[1] = evtAddNameTable(work->nameTable[frame->data.h[1]]);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 9) {
            for (group = viewerinfo->firstobjinfo; group != NULL; group = group->next) {
                if (strcmp(viewerinfo->nametable[group->entryHeader], work->nameTable[frame->NameIndex]) == 0) {
                    key = evtAddFrameInfo(group, frame->Frame);
                    key->index = frame->Length;
                    key->body.unsignedShorts[1] = frame->data.uh[1];
                    if (frame->data.h[0] < 0) {
                        key->body.shorts[0] = -1;
                    } else {
                        key->body.shorts[0] = evtAddNameTable(work->nameTable[frame->data.h[0]]);
                    }
                }
            }
        }
    }
    group = evtAddViewerObject(10);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 10) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.values[0] = frame->data.f[0];
            key->body.values[1] = frame->data.f[1];
            key->nameIndex = frame->NameIndex;
            memcpy(key->payload, &work->slightTable[frame->Length], sizeof(pmdSceneLightTable_t));
        }
    }
    group = evtAddViewerObject(11);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 11) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->sfogTable[frame->Length], sizeof(pmdSceneFogTable_t));
        }
    }
    group = evtAddViewerObject(12);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 12) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->index = frame->Length;
            key->body.unsignedShorts[0] = frame->data.uh[0];
            key->body.unsignedShorts[1] = frame->data.uh[1];
            key->body.words[1] = frame->data.w[1];
            if (work->header2->kind == 1) {
                key->body.shorts[1] = 0;
            }
            if (work->header2->kind == 2) {
                key->body.shorts[1] -= 128;
                if (key->body.shorts[1] < -255) {
                    key->body.shorts[1] = -255;
                }
                if (key->body.shorts[1] > 255) {
                    key->body.shorts[1] = 255;
                }
            }
        }
    }
    group = evtAddViewerObject(22);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 22) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
        }
    }
    group = evtAddViewerObject(13);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 13) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->blur2Table[frame->data.h[1]], sizeof(dds3Effect_Blur_t));
        }
    }
    group = evtAddViewerObject(14);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 14) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->mblurTable[frame->data.h[1]], sizeof(dds3Effect_PartialMultiBlurParam_t));
        }
    }
    group = evtAddViewerObject(15);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 15) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->dblurTable[frame->data.h[1]], sizeof(dds3Effect_DistortionBlurParam_t));
        }
    }
    group = evtAddViewerObject(23);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 23) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->rblurTable[frame->data.h[1]], sizeof(dds3Effect_RippleBlurParam_t));
        }
    }
    group = evtAddViewerObject(16);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 16) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->filterTable[frame->data.h[1]], sizeof(dds3Effect_Filter_t));
        }
    }
    group = evtAddViewerObject(17);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 17) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            memcpy(key->payload, &work->mfilterTable[frame->data.h[1]], sizeof(dds3Effect_PartialMultiFilterParam_t));
        }
    }
    group = evtAddViewerObject(5);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 5) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
        }
    }
    group = evtAddViewerObject(19);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 19) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            key->body.unsignedShorts[1] = frame->data.uh[1];
        }
    }
    group = evtAddViewerObject(6);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 6) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->index = frame->Length;
            key->body.unsignedShorts[0] = frame->data.uh[0];
        }
    }
    group = evtAddViewerObject(7);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 7) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->index = frame->Length;
            key->body.unsignedShorts[0] = frame->data.uh[0];
        }
    }
    group = evtAddViewerObject(8);
    for (i = 0; i < work->frameCount; i++) {
        frame = &work->frameTable[i];
        if (frame->ObjectType == 8) {
            key = evtAddFrameInfo(group, frame->Frame);
            key->body.unsignedShorts[0] = frame->data.uh[0];
            key->body.unsignedShorts[1] = frame->data.uh[1];
            key->body.unsignedShorts[2] = frame->data.uh[2];
        }
    }
    evtViewerStopFrame();
    evtRunFrame(viewerinfo->nowframe);
    evtViewerStopFrame();
    return 1;
}

s32 evtViewerShutdown(s32 kind)
{
    void *movie;

    evtFreeViewerInfo();
    evtFreeStage();
    evtUnRegistUnit();
    evtInitViewerInfo();
    viewerinfo->flags |= 1;
    if (viewerinfo->workdata != NULL) {
        evtFreePolygonMovieWorkData(viewerinfo->workdata);
        viewerinfo->workdata = NULL;
    }
    movie = evtGetPolygonMovieWorkData2(D_0054F824, evt_file_minor, kind);
    if (movie == NULL) {
        return 0;
    }
    viewerinfo->workdata = movie;
    evtMakeMotionName(movie);
    return 1;
}
