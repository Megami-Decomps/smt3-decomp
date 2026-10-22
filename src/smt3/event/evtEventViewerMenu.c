#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "ps2asm.h"

EQUATE_FPTODP();
extern char D_0054F9D8[];
extern char D_0054F9E0[];
extern char D_0054F9E8[];
extern char D_0054F9F0[];
extern char D_0054F9B0[]; /* "     %d" */
/* Viewer timeline key; the viewer links these on a group's child list. */
typedef struct evtViewerKey_t {
    u16 frame;
    u16 index;                      /* 0x02 */
    s32 nameIndex;
    union {
        struct {
            u8 unk08[2];
            u16 typeIndex;          /* 0x0A */
        } f;
        s32 words[2];
    } body;                         /* 0x08 */
    u8 unk10[4];
    void *payload;                  /* 0x14 */
    struct evtViewerKey_t *next;    /* 0x18 */
    struct evtViewerKey_t *prev;    /* 0x1C */
} evtViewerKey_t;
/* Group prefix: the viewer walks the group chain and each group's keys. */
typedef struct evtViewerGroup_t {
    s32 type;
    s32 entryHeader;                /* 0x04 */
    u8 unk08[0x10];
    s32 childCount;                 /* 0x18 */
    evtViewerKey_t *children;       /* 0x1C */
    evtViewerKey_t *lastChild;      /* 0x20 */
    struct evtViewerGroup_t *next;  /* 0x24 */
} evtViewerGroup_t;
typedef struct {
    u8 unk0000[4];
    s32 flags;                      /* 0x04 */
    void *movieWork;                /* 0x08 */
    s32 headerThird;                /* 0x0C */
    s32 headerFirst;
    s32 frameRange;
    s32 nowframe;               /* 0x18 */
    u8 unk001C[4];
    s32 entryTotal;                 /* 0x20 */
    char entryName[256][32];        /* 0x24 */
    u8 unk2024[0xC];
    s32 trackCount;                 /* 0x2030 */
    evtViewerGroup_t *groups;       /* 0x2034 */
    u8 unk2038[0x248];
    s32 actionMode;                 /* 0x2280 */
    u8 unk2284[8];
    s32 controlState;               /* 0x228C */
    u8 unk2290[0x18];
    s32 mainCursor;                 /* 0x22A8 */
    s32 groupFirst;                 /* 0x22AC */
    u8 unk22B0[4];
    s32 groupCursor;                /* 0x22B4 */
    s32 posCursor;                  /* 0x22B8 */
    s32 itemCursor;                 /* 0x22BC */
    s32 itemCount;                  /* 0x22C0 */
    char *title;                    /* 0x22C4 */
    char **itemNames;               /* 0x22C8 */
    s32 charCol;                    /* 0x22CC */
    s32 charRow;                    /* 0x22D0 */
    char nameStorage[0xD];          /* 0x22D4 */
    u8 unk22E1[7];
    char filename[0xC];             /* 0x22E8 */
    s32 objectCursor;               /* 0x22F4 */
    s32 objectScroll;               /* 0x22F8 */
    s32 frameColumn;                /* 0x22FC */
    s32 frameFirst;
    s32 frameCursor;
    evtViewerGroup_t *frameGroup;   /* 0x2308 */
    s32 commandCategory;
    s32 value;                      /* 0x2310 */
    s32 valueMin;
    s32 valueMax;
    f32 floatValue;                 /* 0x231C */
    f32 floatMin;
    f32 floatMax;
} evtViewerInfo_t;
extern evtViewerInfo_t *viewerinfo;
extern s32 (*D_0032CB48[])(s32, s32);
void evtFrameListItemSub(void *tag, s32 ox, s32 oy, s32 co, evtViewerKey_t *f);
extern s32 mfilterRun(void);
extern s32 evtViewerCheckDirty(void);


extern char D_0054F9B8[];

/* Shared by the float and integer menus after the preceding menu label. */

extern u16 evt_file_minor;

extern u16 D_0054F824;

extern s32 sprintf(char *buf, const char *fmt, ...);

extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern s32 sceWrite(s32 a, const void *b, s32 c);

extern void sdfDrawTagLink(void *aTag, u32 aPacket);

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

extern evtViewerKey_t *evtGetSelectFrameInfo(void);

extern s16 evtFrameList[];

extern s8 D_0032C9E2[];

extern char *frameheadstr[];

extern s8 framelist_xcnt[];

s32 evtFrameListHead(void *tag, s32 ox, s32 oy);
s32 evtFrameListItem(void *tag, s32 ox, s32 oy, s32 no);

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

extern void evtMakeMotionName(void *pWork);

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
void evtFileName2Item(void *tag, s32 ox, s32 oy, u32 no);
s32 evtFileNameHead(void *tag, s32 ox, s32 oy);
void evtFileNameItem(void *tag, s32 ox, s32 oy, s32 no);

extern void dds3ConsPanelTagLink(void *tag, s32 x, s32 y, s32 sx, s32 sy);

void evtObjectListSub(void *tag, evtViewerGroup_t *obj, s32 ox, s32 oy);
void evtObjectListItem(void *tag, s32 ox, s32 oy, s32 no);

s32 evtSelectHead(void *tag, s32 ox, s32 oy);
void evtSelectItem(void *tag, s32 ox, s32 oy, s32 no);
void evtMainMenuItem(void *tag, s32 ox, s32 oy, s32 no);

/* NON_MATCHING: 49 of 112 words differ (first at +0x34). */
#ifdef NON_MATCHING
void evtMenuDisp(void *tag, s32 x, s32 y, s32 sx, s32 sy,
                 s32 baseitem, s32 maxitem, void *head, void *item)
{
    s32 used = 0;
    s32 px;
    s32 py;
    s32 index;

    dds3ConsPanelTagLink(tag, x, y, sx, sy);
    px = x * 16 + 0x7000;
    py = y * 8 + 0x7900;
    if (head != NULL) {
        used = ((evtMenuHead_t)head)(tag, px, py);
        py += used * 0x60;
    }
    for (index = baseitem; used < sy; used++) {
        if (item != NULL) {
            ((evtMenuItem_t)item)(tag, px, py, index);
            py += 0x60;
            index++;
        }
    }
    if (evtMenuDisp_cnt & 0x10) {
        px = (x + sx * 5) * 16 + 0x7060;
        py = y * 8 + 0x78B0;
        if (baseitem > 0) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, D_0054F9A0));
        }
        py = (y + sy * 12) * 8 + 0x78F0;
        if (index < maxitem) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(px, py, 0xFEFFFF, 6, D_0054F9A8));
        }
    }
    evtMenuDisp_cnt++;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMenuDisp);
#endif

s32 evtFloatChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

void evtFloatChangeItem(void *tag, s32 ox, s32 oy, u32 no)
{
    switch (no) {
    case 0:
        /* The f64 cast is the fptodp libcall, and that libcall is what keeps
         * the format string in a call-saved register across it. */
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 4, "     %.1f",
                                               (f64)viewerinfo->floatValue));
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
    if (viewerinfo->actionMode != 8) {
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
    viewerinfo->floatValue += step;
    if (viewerinfo->floatValue < viewerinfo->floatMin) {
        viewerinfo->floatValue = viewerinfo->floatMin;
    }
    if (viewerinfo->floatValue >= viewerinfo->floatMax) {
        viewerinfo->floatValue = viewerinfo->floatMax;
    }
    return 0;
}

s32 evtIntChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

void evtIntChangeItem(void *tag, s32 ox, s32 oy, u32 no)
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
    if (viewerinfo->actionMode != 7) {
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
    if (viewerinfo->value < viewerinfo->valueMin) {
        viewerinfo->value = viewerinfo->valueMin;
    }
    if (viewerinfo->value >= viewerinfo->valueMax) {
        viewerinfo->value = viewerinfo->valueMax;
    }
    return 0;
}

s32 evtFrameChangeHead(void *tag, s32 ox, s32 oy)
{
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtFrameChangeItem(void *tag, s32 ox, s32 oy, u32 no)
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
    if (viewerinfo->actionMode != 6) {
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
    if (viewerinfo->value >= viewerinfo->headerThird) {
        viewerinfo->value = viewerinfo->headerThird - 1;
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
        if (viewerinfo->mainCursor == no) {
            color = viewerinfo->actionMode == 1 ? 4 : 5;
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
    if (viewerinfo->actionMode != 1) {
        return 0;
    }
    return dds3SelectMenu(0, 1, 7, 1, 7, NULL, NULL, NULL, &viewerinfo->mainCursor);
}

s32 evtSelectHead(void *tag, s32 ox, s32 oy)
{
    char *title = viewerinfo->title;

    if (title == NULL) {
        return 0;
    }
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0, D_0054F9B8, title));
    return 2;
}
void evtSelectItem(void *tag, s32 ox, s32 oy, s32 no)
{
    s32 color;

    if (no < viewerinfo->itemCount) {
        if (viewerinfo->itemCursor == no) {
            color = viewerinfo->actionMode == 2 ? 4 : 5;
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

    if (viewerinfo->title != NULL) {
        width = strlen(viewerinfo->title);
        if (width < 6) {
            width = 6;
        }
    }
    evtMenuDisp(list, x, y, width, viewerinfo->itemCount + 3, 0, viewerinfo->itemCount,
                evtSelectHead, evtSelectItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->actionMode != 2) {
        return 0;
    }
    return dds3SelectMenu(0, 1, viewerinfo->itemCount, 1, viewerinfo->itemCount,
                          NULL, NULL, NULL, &viewerinfo->itemCursor);
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
        if (viewerinfo->charRow == no && viewerinfo->charCol == i) {
            color = viewerinfo->actionMode == 3 ? 4 : 5;
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
    if (viewerinfo->actionMode != 3) {
        return 0;
    }
    input = dds3SelectMenu(0, 0xB, 4, 0xB, 4, NULL, NULL, &viewerinfo->charCol,
                           &viewerinfo->charRow);
    if (input < 0) {
        return -1;
    }
    if (input == 1) {
        key = D_0032CAD0[viewerinfo->charRow][viewerinfo->charCol];
        if (viewerinfo->charRow == 3 && viewerinfo->charCol >= 4) {
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

void evtFileName2Item(void *tag, s32 ox, s32 oy, u32 no)
{
    s32 color = 0;

    if (viewerinfo->charRow == no) {
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
    if (viewerinfo->actionMode != 9) {
        return 0;
    }
    if (dds3PadButtonState[0x26] < 0) {
        viewerinfo->charRow ^= 1;
    }
    if (dds3PadButtonState[0x27] < 0) {
        viewerinfo->charRow ^= 1;
    }
    if ((u8)dds3PadButtonState[0x24] & 2) {
        switch (viewerinfo->charRow) {
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
        switch (viewerinfo->charRow) {
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
        switch (viewerinfo->charRow) {
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
        switch (viewerinfo->charRow) {
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

void evtObjectListSub(void *tag, evtViewerGroup_t *obj, s32 ox, s32 oy)
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
                                           viewerinfo->entryName[index]));
}

void evtObjectListItem(void *tag, s32 ox, s32 oy, s32 no)
{
    char names[24][30] = {
        "STAGE", "UNIT", "CAMERA", "D3P", "MESSAGE", "SE",
        "FADE", "QUAKE", "BLUR", "LIGHT", "SLIGHT", "SFOG",
        "FF", "BLUR2", "MBLUR", "DBLUR", "FILTER", "MFILTER",
        "BED", "BGM", "MG1", "MG2", "FBOKASI", "RBLUR",
    };
    evtViewerGroup_t *group;
    s32 color;
    s32 i;

    if (viewerinfo->trackCount == 0) {
        return;
    }
    if (no < viewerinfo->trackCount) {
        group = viewerinfo->groups;
        for (i = 0; i < no; i++) {
            group = group->next;
        }
        if (viewerinfo->objectScroll + viewerinfo->objectCursor == no) {
            if (viewerinfo->actionMode == 4) {
                viewerinfo->frameColumn = 0;
                viewerinfo->frameFirst = 0;
                viewerinfo->frameCursor = 0;
                viewerinfo->frameGroup = group;
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

    evtMenuDisp(list, x, y, 8, 0x1D, viewerinfo->objectScroll, viewerinfo->trackCount,
                NULL, evtObjectListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->actionMode != 4) {
        return 0;
    }
    if (viewerinfo->trackCount == 0) {
        return 0;
    }
    if (viewerinfo->trackCount < 0x1D) {
        visible = viewerinfo->trackCount;
    } else {
        visible = 0x1D;
    }
    return dds3SelectMenu(0, 1, viewerinfo->trackCount, 1, visible, NULL,
                          &viewerinfo->objectScroll, NULL, &viewerinfo->objectCursor);
}

/* NON_MATCHING: 39 of 78 words differ (first at +0x20). */
#ifdef NON_MATCHING
s32 evtFrameListHead(void *tag, s32 ox, s32 oy)
{
    s32 type = viewerinfo->frameGroup->type;
    s32 i;

    for (i = 0; i < evtFrameList[type * 5]; i++) {
        s32 w = framelist_xcnt[D_0032C9E2[i + type * 10]] * 0xC0;

        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, 0xE, D_0054F9B8,
                                               frameheadstr[D_0032C9E2[i + type * 10]]));
        ox += w;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListHead);
#endif

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C230);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C240);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C250);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C260);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C270);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C280);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C290);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C2A0);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListItemSub);

s32 evtFrameListItem(void *tag, s32 ox, s32 oy, s32 no)
{
    evtViewerKey_t *key;
    s32 color;
    s32 i;

    if (no < viewerinfo->frameGroup->childCount + 1) {
        key = NULL;
        if (no != viewerinfo->frameGroup->childCount) {
            key = viewerinfo->frameGroup->children;
            for (i = 0; i < no; i++) {
                key = key->next;
            }
        }
        if (viewerinfo->frameCursor + viewerinfo->frameFirst == no) {
            if (viewerinfo->actionMode == 5) {
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
    }
}
s32 evtFrameListMenu(s32 x, s32 y)
{
    evtViewerGroup_t *group;
    void *list;
    s32 visible;
    s32 ret;

    if (viewerinfo->trackCount == 0) {
        return 0;
    }
    group = viewerinfo->frameGroup;
    if (evtFrameList[group->type * 5] == 0) {
        return -1;
    }
    list = dds3FontReqAllocDrawTag();
    evtMenuDisp(list, x, y, 0x1C, 0x19, viewerinfo->frameCursor, group->childCount + 1,
                evtFrameListHead, evtFrameListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->actionMode != 5) {
        return 0;
    }
    if (group->childCount + 1 < 0x18) {
        visible = group->childCount + 1;
    } else {
        visible = 0x18;
    }
    ret = dds3SelectMenu(0, evtFrameList[group->type * 5], group->childCount + 1,
                         evtFrameList[group->type * 5], visible, NULL,
                         &viewerinfo->frameCursor, &viewerinfo->frameColumn,
                         &viewerinfo->frameFirst);
    if (dds3PadButtonState[0x20] < 0) {
        ret = -2;
    }
    if (dds3PadButtonState[0x2C] != 0) {
        if (viewerinfo->frameCursor + viewerinfo->frameFirst < group->childCount) {
            evtViewerKey_t *key = evtGetSelectFrameInfo();

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
    if (viewerinfo->groupFirst + viewerinfo->groupCursor != no) {
        color = 0;
    }
    if (no == 0) {
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(ox, oy, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != 3) {
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].head;
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
            for (basic = ((dds3SceneBasic_t *)dds3GetActiveScene()->workdata)->basictable[i].head;
                 basic != NULL; basic = basic->next) {
                if (basic->name != NULL) {
                    count++;
                }
            }
        }
    }
    count++;
    evtMenuDisp(list, x, y, 0x1A, 0x17, viewerinfo->groupFirst, count, NULL, evtBasicListItem);
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, list);
    if (viewerinfo->actionMode != 0xA) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, NULL, &viewerinfo->groupFirst, NULL,
                          &viewerinfo->groupCursor);
}

void evtEventPosItem(void *tag, s32 ox, s32 oy, s32 no)
{
    dds3Basic_t *basic;
    s32 color;
    s32 slot;

    basic = NULL;
    color = 4;
    if (viewerinfo->posCursor != no) {
        color = 0;
    }
    slot = ((s8 *)((u8 *)no + (u32)evtGetSelectFrameInfo()))[0xC];
    if (slot >= 0) {
        basic = dds3SceneBasicSearchName(dds3GetActiveScene(), viewerinfo->entryName[slot]);
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

    switch (viewerinfo->frameGroup->type) {
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
    if (viewerinfo->actionMode != 0xC) {
        return 0;
    }
    return dds3SelectMenu(0, 1, rows, 1, rows, NULL, NULL, NULL, &viewerinfo->posCursor);
}

s32 mfilter_Process(void)
{
    if (mfilterRun() == 0) {
        viewerinfo->controlState = 0;
        return -1;
    }
    return 0;
}
s32 evtViewerExit(void)
{
    if (evtViewerCheckDirty() == 0) {
        viewerinfo->controlState = 0;
        return -1;
    }
    return 0;
}
/* NON_MATCHING: 20 of 56 words differ (first at +0x4). */
#ifdef NON_MATCHING
s32 func_001F79A8(void)
{
    s32 busy = 1;

    switch (viewerinfo->frameGroup->type) {
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
        if (viewerinfo->controlState == 0) {
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
    if (viewerinfo->controlState == 0) {
        return 1;
    }
    dds3KillProcess((void *)viewerinfo->controlState, 1);
    viewerinfo->controlState = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", func_001F79A8);
#endif

s32 evtViewerDispatch(s32 actionIndex, s32 x, s32 y)
{
    s32 mode = viewerinfo->actionMode;

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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtViewerKey_t *key;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtViewerKey_t *key;
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
    nameCount = viewerinfo->entryTotal;
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

    header[0] = viewerinfo->headerFirst;
    header[1] = viewerinfo->frameRange;
    header[2] = viewerinfo->headerThird;
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        evtViewerKey_t *key;

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
                        record.span = (u16)viewerinfo->frameRange - key->frame;
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

    for (i = 0; i < viewerinfo->entryTotal; i++) {
        evtWriteRecord(fd, viewerinfo->entryName[i], 0x20);
    }
}
void evtSaveObjType2(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
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
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x30);
            }
        }
    }
}
void evtSaveObjType11(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x20);
            }
        }
    }
}
void evtSaveObjType13(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x28);
            }
        }
    }
}
void evtSaveObjType14(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType15(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType23(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x2C);
            }
        }
    }
}
void evtSaveObjType16(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x18);
            }
        }
    }
}
void evtSaveObjType17(s32 fd)
{
    evtViewerGroup_t *group;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtViewerKey_t *key;
            for (key = group->children; key != NULL; key = key->next) {
                evtWriteRecord(fd, key->payload, 0x24);
            }
        }
    }
}
s32 evtMakeViewerPath(s32 kind)
{
    char path[0x40];
    evtViewerGroup_t *group;
    s32 fd;
    s32 i;

    for (group = viewerinfo->groups; group != NULL; group = group->next) {
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

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMakeMotionName);

s32 evtViewerShutdown(s32 kind)
{
    void *movie;

    evtFreeViewerInfo();
    evtFreeStage();
    evtUnRegistUnit();
    evtInitViewerInfo();
    viewerinfo->flags |= 1;
    if (viewerinfo->movieWork != NULL) {
        evtFreePolygonMovieWorkData(viewerinfo->movieWork);
        viewerinfo->movieWork = NULL;
    }
    movie = evtGetPolygonMovieWorkData2(D_0054F824, evt_file_minor, kind);
    if (movie == NULL) {
        return 0;
    }
    viewerinfo->movieWork = movie;
    evtMakeMotionName(movie);
    return 1;
}
