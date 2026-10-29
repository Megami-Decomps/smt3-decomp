#include "common.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"

/* evtAddFrameInfo allocates a 0x20-byte key; only the list and the frame
 * number are needed here. */


/* Shared track prefix recovered by evtEventViewer. */


typedef struct {
    u16 mode;
    u16 unk02;
    u16 unk04;
    u16 unk06;
} evtViewerMenuEntry_t;

typedef struct {
    void *block;
    s32 flags;
    void *workdata;
    s32 totalframe;
    s32 firstframe;
    s32 lastframe;
    s32 nowframe;
    s32 oldframe;
    s32 nametablecnt;
    char nametable[256][32];
    dds3Basic_t *activecamera;
    u8 freecamera[4];
    dds3Basic_t *gamecamera;
    s32 objcnt;
    evtViewerObject_s *firstobjinfo;
    evtViewerObject_s *lastTrack;
    void *effectBasic[0x7F];
    s32 framedisp;                  /* 0x2238 */
    evtViewerMenuEntry_t menuStack[8];
    s32 sp;
    s32 nowmenu;
    s32 menusw;                     /* 0x2284 */
    u8 unk2288[0x78];
    s32 frameFirst;                 /* 0x2300 */
    s32 frameCursor;                /* 0x2304 */
    evtViewerObject_s *framelist_target; /* 0x2308 */
} evtViewerInfo_t;

extern evtViewerInfo_t *viewerinfo;

extern char D_0053BC98[];

extern char D_0054F910[];

extern void *D_002F6978[];

extern void *dds3FontReqAllocDrawTag(void);

extern void dds3ConsPanelTagLink(void *aTag, s32 aX, s32 aY, s32 aSx, s32 aSy);

extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(void *aTag, u32 aPacket);

extern void sdfPrimLinkRect(void *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                            s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

s32 FrameVarProcess(dds3ProcessID_t *id);

/* NON_MATCHING: 956 of retail's 980 bytes; the body matches instruction for
 * instruction but three loop-invariant constants stay immediates here instead
 * of taking three more saved registers. */
#ifdef NON_MATCHING
s32 FrameVarProcess(dds3ProcessID_t *id)
{
    void *tag;
    void *cursorTag;
    void *drawTag;
    evtFrameInfo_t *frame;
    s32 color;
    s32 i;
    s32 span;
    s32 x;

    if (viewerinfo->framedisp == 0 && viewerinfo->menusw == 0) {
        return 0;
    }
    tag = dds3FontReqAllocDrawTag();
    dds3ConsPanelTagLink(tag, 0x1E, 0x190, 0x26, 2);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x71E0, 0x8580, 0xFEFFFF, 0, D_0054F910,
                                          viewerinfo->firstframe));
    sdfPrimLinkRect(tag, 0x80009000, 0, 0x74E0, 0x85A0, 0x8B60, 0x85B8, 0xFEFFFF, NULL);
    if (viewerinfo->firstframe > 0) {
        sdfPrimLinkRect(tag, 0x80909090, 0, 0x74E0, 0x85A0,
                        viewerinfo->firstframe * 360 / viewerinfo->totalframe * 16 + 0x74E0,
                        0x85B8, 0xFEFFFF, NULL);
    }
    if (viewerinfo->lastframe < viewerinfo->totalframe - 1) {
        sdfPrimLinkRect(tag, 0x80909090, 0,
                        viewerinfo->lastframe * 360 / viewerinfo->totalframe * 16 + 0x74E0,
                        0x85A0, 0x8B60, 0x85B8, 0xFEFFFF, NULL);
    }
    cursorTag = NULL;
    span = viewerinfo->nowframe * 360 / viewerinfo->totalframe;
    x = span * 16 + 0x74E0;
    sdfPrimLinkRect(tag, (viewerinfo->flags & 1) ? 0x80FFFF00 : 0x8000FFFF, 0, x, 0x8588,
                    ((span + 2) << 4) + 0x74E0, 0x85D8, 0xFEFFFF, NULL);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(x, 0x85E0, 0xFEFFFF,
                                          (viewerinfo->flags & 1) ? 5 : 6, D_0054F910,
                                          viewerinfo->nowframe));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8B60, 0x8580, 0xFEFFFF, 0, D_0054F910,
                                          viewerinfo->lastframe));
    if (viewerinfo->framelist_target != NULL && viewerinfo->menusw == 1) {
        frame = viewerinfo->framelist_target->children;
        i = 0;
        while (frame != NULL) {
            if (i == viewerinfo->frameCursor + viewerinfo->frameFirst) {
                cursorTag = dds3FontReqAllocDrawTag();
                drawTag = cursorTag;
                color = 0x800000FF;
            } else {
                drawTag = tag;
                color = 0x8000FFFF;
            }
            span = frame->frame * 360 / viewerinfo->totalframe;
            i++;
            sdfPrimLinkRect(drawTag, color, 0, 0x74E0 + span * 16, 0x85A0,
                            0x74E0 + ((span + 2) << 4), 0x85F8, 0xFEFFFF, NULL);
            frame = frame->next;
        }
    }
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, tag);
    if (cursorTag != NULL) {
        ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, cursorTag);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewerFrameVar", FrameVarProcess);
#endif

void evtInitFrameVar(void)
{
    dds3StartProcess(D_0053BC98, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BC98);

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_0053BCB0);

