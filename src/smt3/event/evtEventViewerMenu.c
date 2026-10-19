#include "common.h"
#include "kernel.h"

/* EE GCC promotes %f arguments through SMT3's software conversion helper. */
__asm__("fptodp = sdfFloatToULong");
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
    u8 unk08[2];
    u16 typeIndex;                  /* 0x0A */
    u8 unk0C[8];
    void *payload;                  /* 0x14 */
    struct evtViewerKey_t *next;    /* 0x18 */
    struct evtViewerKey_t *prev;    /* 0x1C */
} evtViewerKey_t;
/* Group prefix: the viewer walks the group chain and each group's keys. */
typedef struct evtViewerGroup_t {
    s32 type;
    u8 unk04[0x14];
    s32 childCount;                 /* 0x18 */
    evtViewerKey_t *children;       /* 0x1C */
    evtViewerKey_t *lastChild;      /* 0x20 */
    struct evtViewerGroup_t *next;  /* 0x24 */
} evtViewerGroup_t;
typedef struct {
    u8 unk0000[0xC];
    s32 headerThird;                /* 0x0C */
    s32 headerFirst;
    s32 frameRange;
    u8 unk0018[8];
    s32 entryTotal;                 /* 0x20 */
    char entryName[256][32];        /* 0x24 */
    u8 unk2024[0x10];
    evtViewerGroup_t *groups;       /* 0x2034 */
    u8 unk2038[0x248];
    s32 actionMode;                 /* 0x2280 */
    u8 unk2284[8];
    s32 controlState;               /* 0x228C */
    u8 unk2290[0x34];
    char *title;                    /* 0x22C4 */
    u8 unk22C8[8];
    s32 charRow;                    /* 0x22D0 */
    u8 unk22D4[0x28];
    s32 frameColumn;                /* 0x22FC */
    s32 frameFirst;
    s32 frameCursor;
    evtViewerGroup_t *frameGroup;   /* 0x2308 */
    s32 commandCategory;
    s32 value;                      /* 0x2310 */
    s32 valueMin;
    s32 valueMax;
    f32 floatValue;                 /* 0x231C */
} evtViewer_t;
extern evtViewer_t *viewerinfo;
extern s32 (*D_0032CB48[])(s32, s32);
extern char D_0053C3A8[]; /* "----- NEW FRAME -----" */
void evtFrameListItemSub(void *list, s32 x, s32 y, s32 color, evtViewerKey_t *key);
extern s32 mfilterRun(void);
extern s32 evtViewerCheckDirty(void);

extern char D_0053BEF0[];

extern char D_0054F9B8[];

/* Shared by the float and integer menus after the preceding menu label. */
extern char D_0053BCC0[]; /* "VALUE CHANGE." */

extern u16 evt_file_minor;

extern u16 D_0054F824;

extern s32 sprintf(char *buf, const char *fmt, ...);

extern s32 sdfDevPrintf(s32 x, s32 y, u32 color, s32 flag, const char *format, ...);

extern s32 sceWrite(s32 fd, const void *buffer, s32 size);

extern void sdfDrawTagLink(void *list, u32 tag);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMenuDisp);

s32 evtFloatChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0053BCC0));
    return 2;
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053BCC0);

void evtFloatChangeItem(void *list, s32 x, s32 y, u32 index)
{
    switch (index) {
    case 0:
        /* The f64 cast is the fptodp libcall, and that libcall is what keeps
         * the format string in a call-saved register across it. */
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, "     %.1f",
                                               (f64)viewerinfo->floatValue));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+0.1"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = VALUE-+1.0"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeMenu);

s32 evtIntChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0053BCC0));
    return 2;
}

void evtIntChangeItem(void *list, s32 x, s32 y, u32 index)
{
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, D_0054F9B0, viewerinfo->value));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = VALUE-+"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        break;
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeMenu);

s32 evtFrameChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtFrameChangeItem(void *list, s32 x, s32 y, u32 index)
{
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 4, D_0054F9B0, viewerinfo->value));
        return;
    case 2:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RR  = ENTER"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RD  = CANCEL"));
        return;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 7:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 8:
        break;
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenuItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenu);

s32 evtSelectHead(void *list, s32 x, s32 y)
{
    char *title = viewerinfo->title;

    if (title == NULL) {
        return 0;
    }
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0054F9B8, title));
    return 2;
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameMenu);

s32 evtFileName2Head(void *list, s32 x, s32 y)
{
    char name[32];

    sprintf(name, D_0053BEF0, D_0054F824, evt_file_minor);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0054F9B8, name));
    return 2;
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053BEF0);

void evtFileName2Item(void *list, s32 x, s32 y, u32 index)
{
    s32 color = 0;

    if (viewerinfo->charRow == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0xE, D_0054F9D8));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, D_0054F9E0, D_0054F824));
        return;
    case 1:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0xE, D_0054F9E8));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x + 0x600, y, 0xFEFFFF, color, D_0054F9F0, evt_file_minor));
        return;
    case 3:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RR  = ENTER"));
        return;
    case 6:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RD  = CANCEL"));
        return;
    case 7:
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Menu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListSub);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtOjbectListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C230);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C240);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C250);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C260);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C270);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C280);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C290);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C2A0);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListItemSub);

s32 evtFrameListItem(void *list, s32 x, s32 y, s32 index)
{
    evtViewerKey_t *key;
    s32 color;
    s32 i;

    if (index < viewerinfo->frameGroup->childCount + 1) {
        key = NULL;
        if (index != viewerinfo->frameGroup->childCount) {
            key = viewerinfo->frameGroup->children;
            for (i = 0; i < index; i++) {
                key = key->next;
            }
        }
        if (viewerinfo->frameCursor + viewerinfo->frameFirst == index) {
            if (viewerinfo->actionMode == 5) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        if (key != NULL) {
            evtFrameListItemSub(list, x, y, color, key);
        } else {
            sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color ? color : 8, D_0053C3A8));
        }
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListMenu);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053C3A8);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosMenu);

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
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", func_001F79A8);

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
                key->typeIndex = childIndex++;
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
                key->typeIndex = childIndex++;
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
                key->typeIndex = childIndex++;
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
                key->typeIndex = childIndex++;
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
                key->typeIndex = childIndex++;
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
                key->typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveViewerHeader);

void evtSaveViewerInfo(s32 fd)
{
    s32 header[4];

    header[0] = viewerinfo->headerFirst;
    header[1] = viewerinfo->frameRange;
    header[2] = viewerinfo->headerThird;
    header[3] = 0;
    evtWriteRecord(fd, header, 0x10);
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveFrameList);

void evtSaveObjectList(s32 fd)
{
    s32 i;

    for (i = 0; i < viewerinfo->entryTotal; i++) {
        evtWriteRecord(fd, viewerinfo->entryName[i], 0x20);
    }
}
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType2);

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
INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMakeViewerPath);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMakeMotionName);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerShutdown);
