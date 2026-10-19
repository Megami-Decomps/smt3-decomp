#include "common.h"
#include "kernel.h"

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

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeMenu);

s32 evtIntChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0053BCC0));
    return 2;
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053BD10);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", D_0053BD20);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_0053BD30);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeMenu);

s32 evtFrameChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenuItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectHead);

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

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Item);

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

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", mfilter_Process);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerExit);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", func_001F79A8);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerDispatch);

s32 evtWriteRecord(s32 fd, const void *buffer, s32 size)
{
    return sceWrite(fd, buffer, size);
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType10);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType11);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType13);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType14);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType15);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType23);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType16);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCountObjType17);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveViewerHeader);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveViewerInfo);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveFrameList);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjectList);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType2);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType10);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType11);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType13);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType14);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType15);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType23);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType16);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSaveObjType17);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMakeViewerPath);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMakeMotionName);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerShutdown);
