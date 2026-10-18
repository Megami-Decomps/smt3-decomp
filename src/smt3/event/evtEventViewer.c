#include "common.h"

/* evtAddFrameInfo allocates/clears a 0x20-byte key. Its parameter bytes
 * depend on the owning track kind; only the list and payload are needed here. */
typedef struct evtFrameInfo_t {
    u16 frame;
    u8 unk02[2];
    s32 nameIndex;
    u8 parameters[0xC];
    void *payload;                 /* 0x14 */
    struct evtFrameInfo_t *next;   /* 0x18 */
    struct evtFrameInfo_t *prev;   /* 0x1C */
} evtFrameInfo_t;

/* Track prefix consumed by the intrusive-list helpers. */
typedef struct {
    s32 type;
    u8 unk04[0x14];
    s32 childCount;                /* 0x18 */
    evtFrameInfo_t *children;      /* 0x1C */
    evtFrameInfo_t *lastChild;     /* 0x20 */
} evtFrameTrack_t;

typedef struct evtViewer_t evtViewer_t;
extern evtFrameInfo_t *evtAddFrameInfo(evtFrameTrack_t *track, s32 frame, evtViewer_t *viewer);
extern void sdfChipFreeDirect(void *memory);
extern void dds3DeleteSceneBasicData(void *basic);
extern void evtInitViewerInfo(void);
extern s32 evtEventViewerProcess2(void);
extern s32 dds3KillProcessName(const char *name, s32 hierarchy);
extern const char D_0053B9E8[];


/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void LinkFrameInfo(evtFrameTrack_t *list, evtFrameInfo_t *insertedNode) {
    evtFrameInfo_t *cursor = list->children;

    if (cursor == 0) {
        list->children = insertedNode;
        list->lastChild = insertedNode;
        insertedNode->next = 0;
        insertedNode->prev = 0;
    } else {
        while (cursor != 0) {
            if (insertedNode->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    list->children = insertedNode;
                    cursor->prev = insertedNode;
                    insertedNode->next = cursor;
                    insertedNode->prev = 0;
                } else {
                    cursor->prev->next = insertedNode;
                    insertedNode->prev = cursor->prev;
                    insertedNode->next = cursor;
                    cursor->prev = insertedNode;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            evtFrameInfo_t *tailNode = list->lastChild;
            tailNode->next = insertedNode;
            insertedNode->prev = list->lastChild;
            insertedNode->next = 0;
            list->lastChild = insertedNode;
        }
    }
    list->childCount++;
}

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void UnlinkFrameInfo(evtFrameTrack_t *list, evtFrameInfo_t *removedNode) {
    evtFrameInfo_t *nextNode = removedNode->next;
    evtFrameInfo_t *previousNode = removedNode->prev;
    if (previousNode == 0) {
        list->children = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        evtFrameInfo_t *previousNeighbor = removedNode->prev;
        evtFrameInfo_t *nextNeighbor = removedNode->next;
        if (nextNeighbor == 0) {
            list->lastChild = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = list->childCount;
        removedNode->prev = 0;
        removedNode->next = 0;
        list->childCount = nodeCount - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void SortFrameInfo(evtFrameTrack_t *list) {
    if (list != 0) {
        evtFrameInfo_t *anchorNode = list->children;
        while (anchorNode != 0) {
            evtFrameInfo_t *resumeNode = anchorNode->next;
            evtFrameInfo_t *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->frame < anchorNode->frame) {
                    UnlinkFrameInfo(list, candidateNode);
                    LinkFrameInfo(list, candidateNode);
                    /* Resume from the relocated node's new successor, not its old one. */
                    resumeNode = candidateNode->next;
                    break;
                }
                candidateNode = candidateNode->next;
            }
            anchorNode = resumeNode;
        }
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddFrameInfo);

evtFrameInfo_t *AddFrameInfo2(evtFrameTrack_t *group, s32 frame, evtViewer_t *viewer)
{
    return evtAddFrameInfo(group, frame, viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetSelectFrameInfo);

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void DelFrameInfo(evtFrameTrack_t *entry, evtFrameInfo_t *node)
{
    UnlinkFrameInfo(entry, node);
    if (node->payload != NULL) {
        if (entry->type != 0x12) {
            sdfChipFreeDirect(node->payload);
        }
        node->payload = NULL;
    }
    sdfChipFreeDirect(node);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", DelFrameInfo2);

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetViewerObjectTypeCount);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtGetAllFrameInfoCount);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelViewerObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSetFirstFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtSetLastFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtInitViewerInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtFreeViewerInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtUnRegistUnit);

INCLUDE_ASM(const s32, "event/evtEventViewer", SearchNameTable);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddNameTable);

INCLUDE_ASM(const s32, "event/evtEventViewer", SearchBasicFromNameIndex_evtEventViewer);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B4A8);

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkEffectBasic);

INCLUDE_ASM(const s32, "event/evtEventViewer", AddEffectTable2);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelEffectTable_evtEventViewer);

INCLUDE_ASM(const s32, "event/evtEventViewer", AddBED);

void DelBED(evtFrameInfo_t *node)
{
    if (node->payload != NULL) {
        dds3DeleteSceneBasicData(node->payload);
    }
    node->payload = NULL;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawEffect);

INCLUDE_ASM(const s32, "event/evtEventViewer", SetActiveCamera);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame1);

INCLUDE_ASM(const s32, "event/evtEventViewer", OutFrame1);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame2);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame3);

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrameUnit);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtRunFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", DrawActiveCameraName);

INCLUDE_ASM(const s32, "event/evtEventViewer", SelectActiveCamera);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtStopFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerStopFrame);

INCLUDE_ASM(const s32, "event/evtEventViewer", FrameSelect);

INCLUDE_ASM(const s32, "event/evtEventViewer", PushMenu);

INCLUDE_ASM(const s32, "event/evtEventViewer", PopMenu);

u32 evtEventViewerProcess5_0(void)
{
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B620);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterMain);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterSelect);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterObjList);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B700);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B710);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B720);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B730);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B740);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B750);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B760);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B770);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFrameList);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFrameChange);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterIntFloatChange);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFilename2);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterBasicList);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterEventPos);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterData);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess4);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess3);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess2);

s32 evtEventViewerProcess1(void)
{
    evtInitViewerInfo();
    return (s32)evtEventViewerProcess2;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerShutdown);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtInitEventViewer);

s32 evtKillEventViewer(void)
{
    return dds3KillProcessName(D_0053B9E8, 1);
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B9E8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", viewerinfo);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F824);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evt_file_minor);

INCLUDE_SDATA(const s32, "event/evtEventViewer", winoff_x);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F82A);

INCLUDE_SDATA(const s32, "event/evtEventViewer", evt_camera);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F830);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F834);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F838);

INCLUDE_SDATA(const s32, "event/evtEventViewer", capwait);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F840);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F848);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F850);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F858);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F860);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F868);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F870);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F878);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F880);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F888);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F890);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F898);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8A0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8B0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0054F8D0);

