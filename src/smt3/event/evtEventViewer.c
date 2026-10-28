#include "common.h"
#include "basic.h"
#include "event.h"

/* The viewer's current menu, which EnterMain switches on to decide what Enter does
 * from each one. Names from the IDB. */
typedef enum EVT_VIEWER_MENU {
    EVT_VIEWER_MENU_NON = 0,
    EVT_VIEWER_MENU_MAIN = 1,
    EVT_VIEWER_MENU_SELECT = 2,
    EVT_VIEWER_MENU_FILENAME = 3,
    EVT_VIEWER_MENU_OBJLIST = 4,
    EVT_VIEWER_MENU_FRAMELIST = 5,
    EVT_VIEWER_MENU_FRAMECHANGE = 6,
    EVT_VIEWER_MENU_INTCHANGE = 7,
    EVT_VIEWER_MENU_FLOATCHANGE = 8,
    EVT_VIEWER_MENU_FILENAME2 = 9,
    EVT_VIEWER_MENU_BASICLIST = 10,
    EVT_VIEWER_MENU_DATA = 11,
    EVT_VIEWER_MENU_EVENTPOS = 12,
    EVT_VIEWER_MENU_MAX = 13
} EVT_VIEWER_MENU;

/* evtAddFrameInfo allocates/clears a 0x20-byte key. Its parameter bytes
 * depend on the owning track kind; only the list and payload are needed here. */
typedef struct evtFrameInfo_t {
    u16 frame;
    u16 index;                     /* 0x02 */
    s32 nameIndex;
    /* Parameter bytes: the owning track kind decides which view is written. */
    union {
        s8 sb[0xC];
        u16 sh[6];
        s32 sw[3];
        f32 sf[3];
    } param;                       /* 0x08 */
    void *work;                 /* 0x14 */
    struct evtFrameInfo_t *next;   /* 0x18 */
    struct evtFrameInfo_t *prev;   /* 0x1C */
} evtFrameInfo_t;

/* Track prefix consumed by the intrusive-list helpers. */
typedef struct evtViewerObject_s {
    s32 type;
    s32 nameindex;
    s32 fileindex;
    s32 unitHandle;                /* 0x0C */
    union {
        u8 unk10[8];
        /* Effect tracks carry their loaded resource at 0x10. */
        struct { void *effectData; u8 unk14[4]; } effect;
    };
    s32 childCount;                /* 0x18 */
    evtFrameInfo_t *p;      /* 0x1C */
    evtFrameInfo_t *lastChild;     /* 0x20 */
    struct evtViewerObject_s *next;  /* 0x24 */
    struct evtViewerObject_s *prev;  /* 0x28 */
} evtViewerObject_s;

/* Registered model record in the viewer's movie work table. */
typedef struct {
    u8 unk00[8];
    s32 model;                     /* 0x08 */
    s32 index;                     /* 0x0C */
    u8 unk10[0x10];
} evtViewerUnit_t;

typedef struct {
    u8 unk00[0x34];
    evtViewerUnit_t *units;        /* 0x34 */
    s32 unitCount;                 /* 0x38 */
} evtViewerMovieWork_t;

/* One pushed viewer menu mode; menuDepth indexes these. */
typedef struct {
    u16 mode;
    u16 unk02;
    u16 unk04;
    u16 unk06;
} evtViewerMenuEntry_t;

struct evtViewerInfo_t {
    void *block;
    s32 flags;                          /* 0x0004 */
    evtViewerMovieWork_t *workdata;    /* 0x0008 */
    s32 totalframe;                     /* 0x000C */
    s32 firstframe;                     /* 0x0010 */
    s32 lastframe;                      /* 0x0014 */
    s32 nowframe;                   /* 0x0018 */
    s32 oldframe;                   /* 0x001C */
    s32 nametablecnt;                     /* 0x0020 */
    char nametable[256][32];            /* 0x0024 */
    dds3Basic_t *activecamera;          /* 0x2024 */
    u8 freecamera[4];
    dds3Basic_t *gamecamera;         /* 0x202C */
    s32 objcnt;                     /* 0x2030 */
    evtViewerObject_s *firstobjinfo;            /* 0x2034 */
    evtViewerObject_s *lastTrack;         /* 0x2038 */
    void *effectBasic[0x7F];            /* 0x203C */
    s32 framedisp;                        /* 0x2238 */
    evtViewerMenuEntry_t menuStack[8];  /* 0x223C */
    s32 sp;                      /* 0x227C */
    s32 nowmenu;                     /* 0x2280 */
    s32 menusw;                   /* 0x2284 */
    u32 word2288;
    u8 reserved228C[4];
    s32 drawblur2;                      /* 0x2290 */
    s32 drawmblur;                      /* 0x2294 */
    s32 drawdblur;                 /* 0x2298 */
    s32 filter;                         /* 0x229C */
    s32 drawmfilter;                    /* 0x22A0 */
    s32 drawrblur;                     /* 0x22A4 */
    s32 fileMode;                       /* 0x22A8 */
    s32 work_base;                        /* 0x22AC */
    u8 work_x[4];
    s32 work_y;                        /* 0x22B4 */
    u8 eventpos_y[4];
    s32 select_y;                        /* 0x22BC */
    s32 menuItemCount;                  /* 0x22C0 */
    char *select_header;                        /* 0x22C4 */
    s32 *menuTable;                     /* 0x22C8 */
    s32 filename_x;                        /* 0x22CC */
    s32 filename_y;                        /* 0x22D0 */
    char nameStorage[0xD];              /* 0x22D4 */
    u8 unk22E1[7];
    char filename[0x14];                /* 0x22E8 */
    s32 framelist_x;                    /* 0x22FC */
    s32 frameFirst;                     /* 0x2300 */
    s32 frameCursor;                    /* 0x2304 */
    evtViewerObject_s *framelist_target;        /* 0x2308 */
    s32 framelist_type;
    s32 value;                          /* 0x2310 */
    s32 min;                       /* 0x2314 */
    s32 max;                       /* 0x2318 */
    f32 fvalue;                     /* 0x231C */
};

typedef struct evtViewerInfo_t evtViewerInfo_t;
extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frame, evtViewerInfo_t *viewer);
extern void sdfChipFreeDirect(void *aPtr);
extern void dds3DeleteSceneBasicData(void *basic);
extern void evtInitViewerInfo(void);
extern s32 dds3KillProcessName(const char *name, s32 shutdowntimer);
extern evtViewerInfo_t *viewerinfo;
extern void *memset(void *a, s32 b, u32 c);
extern void *sdfChipAllocDirect(u32 aSize);
extern void evtFreePolygonMovieWorkData(void *wdata);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
extern void *dds3AddBEDEffectObjectBasic2(void *data, f32 *pos, f32 *rot);
extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);
extern void dds3StartBEDEffectObjectBasic(void *basic, s32 cutno);
extern void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe);
extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);
extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);
extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);
extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);
extern dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);
extern s32 evtStopMover(void *mover, s32 sw, s32 baseframe, s32 nowframe);
extern char D_0053B4A8[];
extern s8 D_0032C9E2[];
extern s32 PopMenu(void);
extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const char *src);
extern void evtRunFrame(s32 nowframe);
extern s32 evtInitFrameVar(void);
extern void dds3LinkProcess(void *parent, s32 target);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void evtFreeViewerInfo(void);
extern void **dds3GetProcessData(void *id);
extern void sdfMemFreeDirect(void *aHandle);
extern s32 SearchNameTable(const char *name);
extern void evtStopFrame(s32 sw, s32 nowframe);
extern void *dds3SetActiveCameraSceneBasic(void *pSceneBasic, void *pCameraBasic);
extern f32 dds3CameraObjectBasicGetFovy(void *pBasic);
extern void dds3SetScreenFovy(f32 fovy);
extern void evtRunUnitMode(s32 unit, s32 mode);
extern void *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(void *aHandle);
extern void *dds3StartProcess(const char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                              s32 v3, void *drawExit, void *deleteExit,
                              void *p);
extern s32 evtInitSky(void);
extern u32 dds3BackColor;
extern void PushMenu(s32 no, s32 x, s32 y);
extern void DelEffectTable_evtEventViewer(s32 index);
extern void DelBED(evtFrameInfo_t *frame);
extern s8 dds3PadButtonState[];
extern void *evtGetUnityCalcArea(void);
extern void dds3EffectBlur(void *area);
extern void Eve_PartialMultiBlurON(void);
extern void Eve_PartialMultiBlurOFF(void);
extern void Eve_PartialMultiFilterON(void);
extern void Eve_PartialMultiFilterOFF(void);
extern void Eve_DistortionBlurON(void);
extern void Eve_DistortionBlurOFF(void);
extern void Eve_RippleBlurON(void);
extern void Eve_RippleBlurOFF(void);
extern void Eve_FilterON(void);
extern void Eve_FilterOFF(void);
extern void evtMakeViewerPath(s32 kind);
extern void evtViewerShutdown(s32 kind);
extern u16 evt_file_minor;
extern u16 D_0054F824;
extern void evtViewerStopFrame(void);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern s32 printf(const char *fmt, ...);
extern void mdlUnRegist(s32 aMajor, s32 aMinor);
extern s32 D_0032C6E0[];
extern s32 capwait;
extern void DrawEffect(void);
extern void FrameSelect(void);
extern dds3Basic_t *dds3GetActiveScene(void);
extern char *D_0054F830;
extern char *D_0054F834;
extern char D_0054F838[];
extern void *D_002F6978[];
extern void *dds3FontReqAllocDrawTag(void);
extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);
extern void sdfDrawTagLink(void *aTag, u32 aPacket);
extern void SetActiveCamera(void);
extern s32 evtEventViewerProcess3(void);
extern s32 evtEventViewerProcess4(void);
void LinkViewerObject(evtViewerObject_s *obj);
void UnlinkViewerObject(evtViewerObject_s *obj);
void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);
void DelViewerObject(evtViewerObject_s *obj);


/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void LinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f) {
    evtFrameInfo_t *cursor = parent->p;

    if (cursor == 0) {
        parent->p = f;
        parent->lastChild = f;
        f->next = 0;
        f->prev = 0;
    } else {
        while (cursor != 0) {
            if (f->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    parent->p = f;
                    cursor->prev = f;
                    f->next = cursor;
                    f->prev = 0;
                } else {
                    cursor->prev->next = f;
                    f->prev = cursor->prev;
                    f->next = cursor;
                    cursor->prev = f;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            evtFrameInfo_t *tailNode = parent->lastChild;
            tailNode->next = f;
            f->prev = parent->lastChild;
            f->next = 0;
            parent->lastChild = f;
        }
    }
    parent->childCount++;
}

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void UnlinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f) {
    evtFrameInfo_t *nextNode = f->next;
    evtFrameInfo_t *previousNode = f->prev;
    if (previousNode == 0) {
        parent->p = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        evtFrameInfo_t *previousNeighbor = f->prev;
        evtFrameInfo_t *nextNeighbor = f->next;
        if (nextNeighbor == 0) {
            parent->lastChild = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = parent->childCount;
        f->prev = 0;
        f->next = 0;
        parent->childCount = nodeCount - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void SortFrameInfo(evtViewerObject_s *parent) {
    if (parent != 0) {
        evtFrameInfo_t *anchorNode = parent->p;
        while (anchorNode != 0) {
            evtFrameInfo_t *resumeNode = anchorNode->next;
            evtFrameInfo_t *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->frame < anchorNode->frame) {
                    UnlinkFrameInfo(parent, candidateNode);
                    LinkFrameInfo(parent, candidateNode);
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

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame, viewer);
}

evtFrameInfo_t *evtGetSelectFrameInfo(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    evtViewerObject_s *group = viewer->framelist_target;
    evtFrameInfo_t *node;
    s32 i;

    if (group == NULL) {
        return NULL;
    }
    node = group->p;
    for (i = 0; i < viewer->frameCursor + viewer->frameFirst; i++) {
        if (node == NULL) {
            break;
        }
        node = node->next;
    }
    return node;
}

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f)
{
    UnlinkFrameInfo(parent, f);
    if (f->work != NULL) {
        if (parent->type != 0x12) {
            sdfChipFreeDirect(f->work);
        }
        f->work = NULL;
    }
    sdfChipFreeDirect(f);
}

void DelFrameInfo2(void)
{
    evtViewerObject_s *group = viewerinfo->framelist_target;
    evtFrameInfo_t *node = evtGetSelectFrameInfo();
    evtViewerInfo_t *viewer;
    s32 slot;

    switch (group->type) {
    case 3:
    case 20:
    case 21:
        slot = node->param.sb[0];
        if (slot >= 0) {
            DelEffectTable_evtEventViewer(slot);
        }
        break;
    case 18:
        DelBED(node);
        break;
    }
    DelFrameInfo(group, node);
    viewer = viewerinfo;
    if (viewer->frameCursor == 0) {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        }
    } else if (viewer->frameFirst > 0) {
        viewer->frameFirst--;
    } else {
        viewer->frameCursor--;
    }
}

/* Insert the track in ascending type order and bump the viewer's track count. */
void LinkViewerObject(evtViewerObject_s *obj) {
    evtViewerObject_s *cursor = viewerinfo->firstobjinfo;

    if (cursor == 0) {
        viewerinfo->firstobjinfo = obj;
        viewerinfo->lastTrack = obj;
        obj->next = 0;
        obj->prev = 0;
    } else {
        while (cursor != 0) {
            if (obj->type < cursor->type) {
                if (cursor->prev == 0) {
                    viewerinfo->firstobjinfo = obj;
                    cursor->prev = obj;
                    obj->next = cursor;
                    obj->prev = 0;
                } else {
                    cursor->prev->next = obj;
                    obj->prev = cursor->prev;
                    obj->next = cursor;
                    cursor->prev = obj;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            viewerinfo->lastTrack->next = obj;
            obj->prev = viewerinfo->lastTrack;
            obj->next = 0;
            viewerinfo->lastTrack = obj;
        }
    }
    viewerinfo->objcnt++;
}

void UnlinkViewerObject(evtViewerObject_s *obj) {
    if (obj->prev == 0) {
        viewerinfo->firstobjinfo = obj->next;
    } else {
        obj->prev->next = obj->next;
    }
    if (obj->next == 0) {
        viewerinfo->lastTrack = obj->prev;
    } else {
        obj->next->prev = obj->prev;
    }
    obj->prev = 0;
    obj->next = 0;
    viewerinfo->objcnt--;
}

evtViewerObject_s *evtAddViewerObject(s32 type)
{
    evtViewerObject_s *track = sdfChipAllocDirect(0x2C);

    if (track == NULL) {
        return NULL;
    }
    memset(track, 0, 0x2C);
    track->type = type;
    track->nameindex = -1;
    track->fileindex = -1;
    LinkViewerObject(track);
    return track;
}

s32 evtGetViewerObjectTypeCount(s32 type)
{
    evtViewerObject_s *track = viewerinfo->firstobjinfo;
    s32 count = 0;

    while (track != NULL) {
        if (track->type == type) {
            count++;
        }
        track = track->next;
    }
    return count;
}

s32 evtGetAllFrameInfoCount(void)
{
    evtViewerObject_s *track = viewerinfo->firstobjinfo;
    s32 count = 0;

    while (track != NULL) {
        count += track->childCount;
        track = track->next;
    }
    return count;
}

void DelViewerObject(evtViewerObject_s *obj)
{
    while (obj->p != NULL) {
        DelFrameInfo(obj, obj->p);
    }
    UnlinkViewerObject(obj);
    sdfChipFreeDirect(obj);
}

void evtSetFirstFrame(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 currentFrame = viewer->nowframe;

    viewer->firstframe = currentFrame;
    if (viewer->lastframe < currentFrame) {
        viewer->lastframe = currentFrame;
    }
}

void evtSetLastFrame(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 currentFrame = viewer->nowframe;

    viewer->lastframe = currentFrame;
    if (currentFrame < viewer->firstframe) {
        viewer->firstframe = currentFrame;
    }
}

void evtInitViewerInfo(void)
{
    memset(viewerinfo, 0, 0x2328);
    viewerinfo->flags |= 1;
    viewerinfo->totalframe = 0x21C;
    viewerinfo->lastframe = 0x21B;
    viewerinfo->framedisp = 1;
}

void evtFreeViewerInfo(void)
{
    while (viewerinfo->firstobjinfo != NULL) {
        DelViewerObject(viewerinfo->firstobjinfo);
    }
    if (viewerinfo->workdata != NULL) {
        evtFreePolygonMovieWorkData(viewerinfo->workdata);
    }
}

void evtUnRegistUnit(void)
{
    s32 i;
    s32 model;
    s32 index;

    if (viewerinfo->workdata != NULL) {
        for (i = 0; i < viewerinfo->workdata->unitCount; i++) {
            model = viewerinfo->workdata->units[i].model;
            index = viewerinfo->workdata->units[i].index;
            printf("unregist %d, %d\n", model, index);
            mdlUnRegist(model, index);
        }
    }
}

s32 SearchNameTable(const char *name)
{
    s32 index = 0;

    if (viewerinfo->nametablecnt > 0) {
        do {
            if (strcmp(name, viewerinfo->nametable[index]) == 0) {
                return index;
            }
            index++;
        } while (index < viewerinfo->nametablecnt);
    }
    return -1;
}

s32 evtAddNameTable(const char *name)
{
    s32 existingIndex = SearchNameTable(name);
    s32 newIndex;

    if (existingIndex >= 0) {
        return existingIndex;
    }
    newIndex = viewerinfo->nametablecnt;
    strcpy(viewerinfo->nametable[newIndex], name);
    viewerinfo->nametablecnt++;
    return newIndex;
}

void *SearchBasicFromNameIndex_evtEventViewer(s32 index)
{
    if (index < 0) {
        return NULL;
    }
    return dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 7, viewerinfo->nametable[index]);
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B4A8);

void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe)
{
    dds3Basic_t *owner;
    s32 currentFrame;
    void *mover;

    if (eff == NULL) {
        return;
    }
    currentFrame = viewerinfo->nowframe;
    if (nameindex < 0) {
        return;
    }
    owner = dds3SceneBasicSearchName(dds3GetActiveScene(), viewerinfo->nametable[nameindex]);
    if (owner == NULL) {
        return;
    }
    switch (owner->type >> 24) {
    case 5:
        if (posindex >= 0) {
            dds3EffectObjectBasicLinkObjectIndex(eff, owner, posindex);
            break;
        }
        /* fallthrough */
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 17:
        dds3EffectObjectBasicLinkObject(eff, owner);
        break;
    case 16:
        evtPathLinkAndMake(owner, eff);
        mover = dds3GetBaseControlWorkData(eff)->pBasic[1];
        printf(D_0053B4A8, baseframe, currentFrame);
        if ((viewerinfo->flags & 1) != 0) {
            evtStopMover(mover, 0, baseframe, currentFrame);
        } else {
            evtStopMover(mover, 1, baseframe, currentFrame);
        }
        break;
    }
}

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, void *pos, void *rot);
extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);
extern s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);
extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                                    dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

s32 AddEffectTable2(s32 nowframe, evtViewerObject_s *object, evtFrameInfo_t *frame)
{
    f32 position[4];
    f32 scale[4];
    dds3Basic_t *effect;
    s32 index;
    s32 type;

    effect = NULL;
    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    scale[3] = 1.0f;

    index = 0;
    while (index < 0x7F && viewerinfo->effectBasic[index] != NULL) {
        index++;
    }
    if (index == 0x7F) {
        return -1;
    }

    type = object->type;
    switch (type) {
    case 3:
        effect = dds3AddD3PEffectObjectBasic2(object->effect.effectData, position, scale);
        if (frame->param.sb[4] != 0) {
            dds3EffectObjectPresetMG12(effect);
        }
        LinkEffectBasic(effect, (s16)frame->param.sh[3], frame->param.sb[5], frame->frame);
        break;
    case 20:
    case 21: {
        dds3Basic_t *part0;
        dds3Basic_t *part1;
        dds3Basic_t *part2;
        dds3Basic_t *part3;

        switch (type) {
        case 20:
            effect = dds3AddMG12EffectObjectBasic2(1, object->effect.effectData);
            break;
        case 21:
            effect = dds3AddMG12EffectObjectBasic2(2, object->effect.effectData);
            break;
        }
        part0 = SearchBasicFromNameIndex_evtEventViewer(frame->param.sb[4]);
        part1 = SearchBasicFromNameIndex_evtEventViewer(frame->param.sb[5]);
        part2 = SearchBasicFromNameIndex_evtEventViewer(frame->param.sb[6]);
        part3 = SearchBasicFromNameIndex_evtEventViewer(frame->param.sb[7]);
        dds3EffectObjectSetMG12Pos(effect, part0, part1, part2, part3);
        if (frame->param.sb[1] != 0) {
            dds3EffectObjectPresetMG12(effect);
        }
        break;
    }
    }

    viewerinfo->effectBasic[index] = effect;
    if (effect != NULL) {
        dds3EffectObjectBasicStatOn(effect, 1);
    }
    return index;
}

void DelEffectTable_evtEventViewer(s32 index)
{
    if (viewerinfo->effectBasic[index] != NULL) {
        dds3DeleteSceneBasicData(viewerinfo->effectBasic[index]);
        viewerinfo->effectBasic[index] = NULL;
    }
}

void *AddBED(void *data, s32 cutno, s32 nameindex, s32 posindex, s32 baseframe)
{
    f32 position[4];
    f32 scale[4];
    void *basic;

    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    scale[3] = 1.0f;
    basic = dds3AddBEDEffectObjectBasic2(data, position, scale);
    dds3EffectObjectBasicStatOn(basic, 1);
    dds3StartBEDEffectObjectBasic(basic, cutno);
    LinkEffectBasic(basic, nameindex, posindex, baseframe);
    return basic;
}

void DelBED(evtFrameInfo_t *frame)
{
    if (frame->work != NULL) {
        dds3DeleteSceneBasicData(frame->work);
    }
    frame->work = NULL;
}

void DrawEffect(void)
{
    if (viewerinfo->drawblur2 != 0) {
        dds3EffectBlur(evtGetUnityCalcArea());
    }
    if (viewerinfo->drawmblur != 0) {
        Eve_PartialMultiBlurON();
    } else {
        Eve_PartialMultiBlurOFF();
    }
    if (viewerinfo->drawmfilter != 0) {
        Eve_PartialMultiFilterON();
    } else {
        Eve_PartialMultiFilterOFF();
    }
    if (viewerinfo->drawdblur != 0) {
        Eve_DistortionBlurON();
    } else {
        Eve_DistortionBlurOFF();
    }
    if (viewerinfo->drawrblur != 0) {
        Eve_RippleBlurON();
    } else {
        Eve_RippleBlurOFF();
    }
    if (viewerinfo->filter != 0) {
        Eve_FilterON();
    } else {
        Eve_FilterOFF();
    }
}

void SetActiveCamera(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    void *camera;

    if (viewer->activecamera != NULL) {
        camera = viewer->activecamera;
    } else {
        camera = viewer->gamecamera;
    }
    if (camera != NULL) {
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
        dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
    }
}

extern void evtEffectLoopControl(dds3Basic_t *effect, s32 sw);
extern s32 evtPlaySe(s32 eventNo, s32 soundNo);
extern void dds3SetQuake(s32 frame, s32 range);
extern void dds3StopQuake(void);

void RunFrame1(evtViewerObject_s *track, evtFrameInfo_t *frameInfo, s32 nowFrame)
{
    switch (track->type) {
    /* Other viewer track kinds do no work in this frame pass. */
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 22:
    case 23:
        break;
    case 3:
    case 20:
    case 21: {
        dds3Basic_t *effect;

        if (nowFrame >= frameInfo->frame) {
            if (frameInfo->param.sb[0] < 0) {
                frameInfo->param.sb[0] = AddEffectTable2(nowFrame, track, frameInfo);
            }
        }
        if (frameInfo->param.sb[0] < 0) {
            break;
        }
        effect = viewerinfo->effectBasic[frameInfo->param.sb[0]];
        if (effect == NULL) {
            return;
        }

        if (nowFrame >= frameInfo->index && frameInfo->index != 0) {
            evtEffectLoopControl(effect, 0);
            frameInfo->param.sw[2] = 1;
        } else if (frameInfo->param.sw[2] == 1) {
            DelEffectTable_evtEventViewer(frameInfo->param.sb[0]);
            frameInfo->param.sb[0] = -1;
            frameInfo->param.sb[0] = AddEffectTable2(nowFrame, track, frameInfo);
            frameInfo->param.sw[2] = 0;
        }

        if (track->type == 21) {
            dds3Basic_t *part0;
            dds3Basic_t *part1;
            dds3Basic_t *part2;
            dds3Basic_t *part3;

            effect = viewerinfo->effectBasic[frameInfo->param.sb[0]];
            if (effect == NULL) {
                return;
            }
            part0 = SearchBasicFromNameIndex_evtEventViewer(frameInfo->param.sb[4]);
            part1 = SearchBasicFromNameIndex_evtEventViewer(frameInfo->param.sb[5]);
            part2 = SearchBasicFromNameIndex_evtEventViewer(frameInfo->param.sb[6]);
            part3 = SearchBasicFromNameIndex_evtEventViewer(frameInfo->param.sb[7]);
            dds3EffectObjectSetMG12Pos(effect, part0, part1, part2, part3);
            return;
        }
        break;
    }
    case 18:
        if (nowFrame == frameInfo->frame) {
            DelBED(frameInfo);
            frameInfo->work = AddBED(track->effect.effectData, frameInfo->param.sb[4],
                                     (s16)frameInfo->param.sh[1], -1,
                                     frameInfo->frame);
        }
        break;
    case 5:
        evtPlaySe(D_0054F824, (s16)frameInfo->param.sh[0]);
        return;
    case 7:
        if (frameInfo->index == 0 && (s16)frameInfo->param.sh[0] == 0) {
            viewerinfo->flags &= ~2;
            dds3StopQuake();
            return;
        }
        if (nowFrame == frameInfo->frame) {
            viewerinfo->flags |= 2;
            dds3SetQuake(frameInfo->index, (s16)frameInfo->param.sh[0]);
        }
        break;
    }
}

void OutFrame1(evtViewerObject_s *track, evtFrameInfo_t *node)
{
    s32 slot;

    switch (track->type) {
    case 3:
    case 20:
    case 21:
        slot = node->param.sb[0];
        if (slot >= 0) {
            DelEffectTable_evtEventViewer(slot);
            node->param.sb[0] = -1;
        }
        break;
    case 18:
        DelBED(node);
        node = NULL;
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame2);

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *basic);
extern void evtLinkLightObjectBasic(evtUnit_t *unit, dds3Basic_t *light,
                                    s32 frame);
extern void evtUnlinkLightObjectBasic(evtUnit_t *unit, s32 frame);
extern dds3Basic_t *dds3GetActiveScene(void);

void RunFrame3(s32 nowframe) {
    dds3Basic_t *scene;
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *outerBasic;
    evtViewerObject_s *track;
    evtFrameInfo_t *frameInfo;
    evtFrameInfo_t *bestFrame;
    dds3Basic_t *lightBasic;
    evtUnit_t *unit;
    s32 bestFrameNumber;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    scene = dds3GetActiveScene();
    sceneWork = scene->workdata;
    for (outerBasic = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic;
         outerBasic != NULL; outerBasic = outerBasic->next) {
        if (outerBasic->name == NULL) {
            continue;
        }
        bestFrame = NULL;
        lightBasic = NULL;
        bestFrameNumber = -1;
        for (track = viewerinfo->firstobjinfo; track != NULL;
             track = track->next) {
            if (track->type != 9) {
                continue;
            }
            for (frameInfo = track->p; frameInfo != NULL;
                 frameInfo = frameInfo->next) {
                dds3Basic_t *namedBasic;

                if (nowframe < (s32)frameInfo->frame ||
                    (s16)frameInfo->param.sh[0] < 0) {
                    continue;
                }
                scene = dds3GetActiveScene();
                namedBasic = dds3SceneBasicSearchNameAndType(
                    scene, BASIC_TYPE_UNITOBJECT,
                    viewerinfo->nametable[
                        (s16)frameInfo->param.sh[0]]);
                if (outerBasic != namedBasic ||
                    (s32)frameInfo->frame <= bestFrameNumber) {
                    continue;
                }
                bestFrameNumber = frameInfo->frame;
                bestFrame = frameInfo;
                lightBasic = (dds3Basic_t *)track->unitHandle;
            }
        }

        unit = evtGetUnitHandle2(outerBasic);
        if (bestFrame != NULL) {
            if ((s16)bestFrame->param.sh[1] != 0) {
                if (unit->currentTransitionValue != (s32)lightBasic ||
                    (unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION) == 0) {
                    evtLinkLightObjectBasic(unit, lightBasic, bestFrame->index);
                }
            } else {
                if ((unit->flags & EVT_UNIT_FLAG_TARGET_TRANSITION) == 0) {
                    continue;
                }
                if ((unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) != 0) {
                    continue;
                }
                evtUnlinkLightObjectBasic(unit, bestFrame->index);
            }
        } else if (unit->currentTransitionValue != 0) {
            evtUnlinkLightObjectBasic(unit, 0);
        }
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrameUnit);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtRunFrame);

void DrawActiveCameraName(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    char *name;
    char *cameraName;
    void *tag;

    if (viewer->activecamera != NULL) {
        cameraName = viewer->activecamera->name;
        if (cameraName != NULL) {
            name = cameraName;
        } else {
            name = D_0054F830;
        }
    } else {
        if (viewer->gamecamera == NULL) {
            return;
        }
        name = D_0054F834;
    }
    tag = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8680, 0x79C0, 0xFEFFFF, 5, D_0054F838, name));
    ((void (*)(void *, void *))D_002F6978[4])(D_002F6978, tag);
}

void SelectActiveCamera(void)
{
    dds3SceneBasic_t *work;
    evtViewerInfo_t *viewer;
    dds3Basic_t *camera;

    if (dds3GetActiveScene() == NULL) {
        viewerinfo->activecamera = NULL;
        return;
    }
    work = dds3GetActiveScene()->workdata;
    if (dds3PadButtonState[0x23] < 0) {
        viewer = viewerinfo;
        camera = viewer->activecamera;
        if (camera == NULL) {
            viewer->activecamera = work->basictable[4].lastbasic;
        } else {
            camera = camera->prev;
            if (camera != NULL) {
                viewer->activecamera = camera;
            } else if (viewer->gamecamera != NULL) {
                viewer->activecamera = NULL;
            } else {
                viewer->activecamera = work->basictable[4].lastbasic;
            }
        }
    } else if (dds3PadButtonState[0x20] < 0) {
        viewer = viewerinfo;
        camera = viewer->activecamera;
        if (camera == NULL) {
            viewer->activecamera = work->basictable[4].firstbasic;
        } else {
            camera = camera->next;
            if (camera != NULL) {
                viewer->activecamera = camera;
            } else if (viewer->gamecamera != NULL) {
                viewer->activecamera = NULL;
            } else {
                viewer->activecamera = work->basictable[4].firstbasic;
            }
        }
    } else {
        return;
    }
    SetActiveCamera();
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtStopFrame);

void evtViewerStopFrame(void)
{
    evtViewerInfo_t *viewer = viewerinfo;

    if (viewer->flags & 1) {
        evtStopFrame(0, viewer->nowframe);
    } else {
        evtStopFrame(1, viewer->nowframe);
    }
}

void FrameSelect(void)
{
    s32 delta = 0;
    s32 frame;
    s32 last;

    if (((u8)dds3PadButtonState[4] & 2) != 0) {
        delta = viewerinfo->nowframe > 0 ? -1 : 0;
    }
    if (((u8)dds3PadButtonState[5] & 2) != 0) {
        evtViewerInfo_t *viewer = viewerinfo;
        if (viewer->nowframe < viewer->totalframe - 1) {
            delta = 1;
        }
    }
    if (((u8)dds3PadButtonState[6] & 2) != 0) {
        delta = -10;
    }
    if (((u8)dds3PadButtonState[7] & 2) != 0) {
        delta = 10;
    }
    if (dds3PadButtonState[0x2D] < 0) {
        evtViewerInfo_t *viewer = viewerinfo;
        if (viewer->nowframe != viewer->firstframe) {
            delta = viewer->firstframe - viewer->nowframe;
        }
    }
    if (delta != 0) {
        evtViewerInfo_t *viewer = viewerinfo;
        frame = viewer->nowframe + delta;
        if (frame < 0) {
            frame = 0;
        }
        last = viewer->totalframe - 1;
        viewer->nowframe = frame;
        if (last < frame) {
            viewer->nowframe = last;
        }
        evtViewerStopFrame();
    }
}

void PushMenu(s32 no, s32 x, s32 y)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 depth = viewer->sp + 1;

    viewer->nowmenu = no;
    viewer->sp = depth;
    viewer->menuStack[depth].mode = no;
    viewer->menuStack[depth].unk02 = x;
    viewer->menuStack[depth].unk04 = y;
    if (no > 0) {
        if (no >= 3) {
            if (no == 3) {
                viewer->filename_x = 0;
                viewer->filename_y = 0;
                viewer->nameStorage[0] = 0;
                viewerinfo->nameStorage[0xC] = 0;
            }
        }
    }
}

s32 PopMenu(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 depth = viewer->sp;
    s32 top = depth - 1;
    u16 mode;

    if (depth == 0) {
        viewer->nowmenu = 0;
        return 0;
    }
    viewer->sp = top;
    mode = viewer->menuStack[top].mode;
    viewer->nowmenu = mode;
    return mode;
}

u32 evtEventViewerProcess5_0(void)
{
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B620);

s32 EnterMain(void)
{
    evtViewerInfo_t *viewer = viewerinfo;

    switch (viewer->fileMode) {
    case EVT_VIEWER_MENU_NON:
    case EVT_VIEWER_MENU_MAIN:
    case EVT_VIEWER_MENU_SELECT:
        viewer->filename_x = 0;
        viewer->filename_y = 0;
        PushMenu(9, 0xE4, 0x3C);
        break;
    case EVT_VIEWER_MENU_FRAMELIST:
        viewer->value = viewer->totalframe - 1;
        viewer->min = viewer->lastframe;
        viewer->max = 10000;
        PushMenu(7, 0xB4, 0x78);
        break;
    case EVT_VIEWER_MENU_FILENAME:
    case EVT_VIEWER_MENU_OBJLIST:
        viewer->select_y = 0;
        viewer->menuItemCount = 2;
        viewer->select_header = "FRAME SET.OK? ";
        viewer->menuTable = D_0032C6E0;
        PushMenu(2, 0xE4, 0x3C);
        break;
    case EVT_VIEWER_MENU_FRAMECHANGE:
        viewer->flags |= 1;
        viewer->nowframe = viewer->firstframe;
        evtViewerStopFrame();
        viewerinfo->nowmenu = 0;
        viewerinfo->sp = 0;
        viewerinfo->menusw = 0;
        capwait = 0;
        return 1;
    }
    return 0;
}

s32 EnterSelect(s32 unused, s32 menuMode)
{
    s32 completed = 0;
    evtFrameInfo_t *node;
    switch (menuMode) {
    case 1:
        switch (viewerinfo->fileMode) {
        case 3:
            if (viewerinfo->select_y == 0) {
                completed = 1;
                evtSetFirstFrame();
            } else {
                PopMenu();
            }
            break;
        case 4:
            if (viewerinfo->select_y == 0) {
                completed = 1;
                evtSetLastFrame();
            } else {
                PopMenu();
            }
            break;
        }
        break;
    case 5:
        node = evtGetSelectFrameInfo();
        switch (viewerinfo->framelist_type) {
        case -1:
            if (viewerinfo->select_y == 0) DelFrameInfo2();
            break;
        case 5:
            switch (viewerinfo->framelist_target->type) {
            case 8:
                node->param.sh[2] = viewerinfo->select_y;
                break;
            case 10:
                node->nameIndex = viewerinfo->select_y;
                break;
            case 11: case 13: case 14: case 15: case 16: case 17: case 23:
                node->param.sh[0] = viewerinfo->select_y;
                break;
            }
            break;
        case 11:
            switch (viewerinfo->framelist_target->type) {
            case 9:
                node->param.sh[1] = viewerinfo->select_y;
                break;
            case 22:
                node->param.sh[0] = viewerinfo->select_y;
                break;
            }
            break;
        case 16:
            switch (viewerinfo->framelist_target->type) {
            case 3:
                node->param.sb[4] = viewerinfo->select_y;
                break;
            case 20: case 21:
                node->param.sb[1] = viewerinfo->select_y;
                break;
            }
            break;
        case 12:
            switch (viewerinfo->framelist_target->type) {
            case 6:
                node->param.sh[0] = viewerinfo->select_y;
                break;
            case 12:
                node->param.sh[0] = viewerinfo->select_y;
                viewerinfo->word2288 = 0;
                break;
            case 19:
                node->param.sh[0] = viewerinfo->select_y;
                break;
            case 1:
                node->param.sb[0] = viewerinfo->select_y;
                break;
            }
            break;
        }
        evtRunFrame(viewerinfo->nowframe);
        PopMenu();
        break;
    }
    return completed ? -1 : 0;
}

s32 EnterObjList(void)
{
    PushMenu(5, 0x90, 0x48);
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B700);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B710);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B720);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B730);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B740);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B750);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B760);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0053B770);

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFrameList);

s32 EnterFrameChange(void)
{
    evtFrameInfo_t *node = evtGetSelectFrameInfo();
    evtViewerInfo_t *viewer;
    evtViewerObject_s *group;
    s32 type;

    if (node != NULL) {
        viewer = viewerinfo;
        group = viewer->framelist_target;
        type = group->type;
        switch (D_0032C9E2[viewer->framelist_x + type * 10]) {
        case 0:
            node->frame = viewer->value;
            SortFrameInfo(group);
            evtRunFrame(viewerinfo->nowframe);
            break;
        case 9:
            switch (type) {
            case 3:
            case 20:
            case 21:
                node->param.sh[1] = viewer->value;
                break;
            case 18:
                node->param.sh[0] = viewer->value;
                break;
            }
            break;
        case 0xF:
            node->index = viewer->value;
            break;
        }
        PopMenu();
    }
    return 0;
}

s32 EnterIntFloatChange(void)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 value = viewer->value;
    f32 ratio = viewer->fvalue;
    evtFrameInfo_t *node;

    if (viewer->menusw == 0) {
        if (viewer->fileMode == 5) {
            viewer->totalframe = value + 1;
            PopMenu();
            return 0;
        }
    }
    node = evtGetSelectFrameInfo();
    if (node == NULL) {
        return 0;
    }
    switch (D_0032C9E2[viewerinfo->framelist_x + viewerinfo->framelist_target->type * 10]) {
    case 1:
        node->index = value;
        break;
    case 2:
        if (viewerinfo->framelist_target->type == 7) {
            node->param.sh[0] = value;
        }
        break;
    case 3:
        switch (viewerinfo->framelist_target->type) {
        case 8:
            node->param.sh[0] = value;
            break;
        case 12:
            node->param.sh[1] = value;
            break;
        }
        break;
    case 6:
        node->param.sf[0] = ratio * 0.0174532924f;
        break;
    case 8:
        switch (viewerinfo->framelist_target->type) {
        case 5:
            node->param.sh[0] = value;
            break;
        case 3:
            node->param.sb[5] = value;
            break;
        case 12:
            node->param.sw[1] = value;
            break;
        case 18:
            node->param.sb[4] = value;
            break;
        case 19:
            node->param.sh[1] = value;
            break;
        }
        break;
    case 4:
    case 7:
        node->param.sh[1] = value;
        break;
    }
    evtRunFrame(viewerinfo->nowframe);
    PopMenu();
    return 0;
}

s32 EnterFilename2(void)
{
    s32 mode = viewerinfo->fileMode;
    s32 done = 0;

    if (mode < 3) {
        if (mode >= 0) {
            sprintf(viewerinfo->filename, "E%3d_%03d", D_0054F824, evt_file_minor);
            mode = viewerinfo->fileMode;
            if (mode == 0) {
                evtMakeViewerPath(0);
                evtMakeViewerPath(1);
            } else if (mode == 1) {
                evtViewerShutdown(0);
            } else if (mode == 2) {
                evtViewerShutdown(1);
            }
            done = 1;
        }
    }
    return done != 0 ? -1 : 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterBasicList);

s32 EnterEventPos(void)
{
    if (evtGetSelectFrameInfo() == NULL) {
        return 0;
    }
    viewerinfo->work_base = 0;
    viewerinfo->work_y = 0;
    PushMenu(0xA, 0x9C, 0x54);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", EnterData);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess4);

/* NON_MATCHING: 3 of 91 words differ (first at +0x110). */
#ifdef NON_MATCHING
s32 evtEventViewerProcess3(void)
{
    if (viewerinfo->nowframe != viewerinfo->oldframe) {
        evtRunFrame(viewerinfo->nowframe);
    }
    viewerinfo->oldframe = viewerinfo->nowframe;
    DrawActiveCameraName();
    DrawEffect();
    if (dds3PadButtonState[0x22] < 0) {
        viewerinfo->flags |= 1;
        evtViewerStopFrame();
        viewerinfo->nowmenu = 0;
        viewerinfo->sp = 0;
        viewerinfo->menusw = 0;
        PushMenu(1, 0x24, 0x18);
        return (s32)evtEventViewerProcess4;
    }
    SelectActiveCamera();
    if (viewerinfo->menusw == 0) {
        if (dds3PadButtonState[0x21] < 0) {
            if (viewerinfo->framedisp != 0) {
                viewerinfo->framedisp = 0;
            } else {
                viewerinfo->framedisp = 1;
            }
        }
    }
    if (dds3PadButtonState[0x2C] < 0) {
        viewerinfo->flags ^= 1;
        evtViewerStopFrame();
    }
    if ((viewerinfo->flags & 1) != 0) {
        FrameSelect();
        if ((viewerinfo->flags & 1) != 0) {
            return 0;
        }
    }
    if (viewerinfo->nowframe >= viewerinfo->lastframe) {
        viewerinfo->flags ^= 1;
        evtViewerStopFrame();
    } else {
        viewerinfo->nowframe = viewerinfo->nowframe + 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess3);
#endif

s32 evtEventViewerProcess2(void *id)
{
    evtRunFrame(viewerinfo->nowframe);
    dds3LinkProcess(id, evtInitFrameVar());
    return (s32)evtEventViewerProcess3;
}

s32 evtEventViewerProcess1(void)
{
    evtInitViewerInfo();
    return (s32)evtEventViewerProcess2;
}

void evtEventViewerShutdown(void *id)
{
    evtFreeStage();
    while (sdfDelayedResrcCheck() != 0) {
    }
    evtUnRegistUnit();
    evtFreeViewerInfo();
    sdfMemFreeDirect(*dds3GetProcessData(id));
    viewerinfo = NULL;
}

void evtInitEventViewer(void)
{
    void *block;
    evtViewerInfo_t *viewer;
    void *process;

    dds3BackColor = 0x80000000;
    block = sdfMemAllocDirect(0x2328);
    viewer = sdfMemLock(block);
    memset(viewer, 0, 0x2328);
    viewer->block = block;
    viewerinfo = viewer;
    process = dds3StartProcess("EventViewer", 0x3E8, 1, 1, evtEventViewerProcess1,
                               evtEventViewerShutdown, viewer);
    dds3LinkProcess(process, evtInitSky());
}

s32 evtKillEventViewer(void)
{
    return dds3KillProcessName("EventViewer", 1);
}

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

