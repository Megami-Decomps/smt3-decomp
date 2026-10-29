#include "common.h"
#include "basic.h"
#include "event.h"
#include "evtPolygonMovie.h"
#include "effect.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "kernel.h"
#include "model.h"
#include "motion.h"

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




extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frame);
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
typedef struct polyMovieObject_t polyMovieObject_t;
extern s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);
extern char D_0053B4A8[];
extern s8 D_0032C9E2[];
extern s32 PopMenu(void);
extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const char *src);
extern void evtRunFrame(s32 nowframe);
extern dds3ProcessID_t *evtInitFrameVar(void);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern void evtFreeStage(void);
extern s32 sdfDelayedResrcCheck(void);
extern void evtFreeViewerInfo(void);
extern void **dds3GetProcessData(void *id);
extern void sdfMemFreeDirect(void *aHandle);
extern s32 SearchNameTable(const char *name);
extern void evtStopFrame(s32 sw, u32 nowframe);
extern void *dds3SetActiveCameraSceneBasic(void *pSceneBasic, void *pCameraBasic);
extern f32 dds3CameraObjectBasicGetFovy(void *pBasic);
extern void dds3SetScreenFovy(f32 fovy);
extern void evtRunUnitMode(dds3Basic_t *unit, s32 mode);
extern void *sdfMemAllocDirect(s32 aSize);
extern void *sdfMemLock(void *aHandle);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);
extern dds3ProcessID_t *evtInitSky(void);
extern u32 dds3BackColor;
extern void PushMenu(s32 no, s32 x, s32 y);
extern void DelEffectTable_evtEventViewer(s32 index);
extern void DelBED(evtFrameInfo_t *frame);
extern s8 dds3PadButtonState[];
extern effScreenDrawParams_t *evtGetUnityCalcArea(void);
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
    evtFrameInfo_t *cursor = parent->children;

    if (cursor == 0) {
        parent->children = f;
        parent->lastChild = f;
        f->next = 0;
        f->prev = 0;
    } else {
        while (cursor != 0) {
            if (f->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    parent->children = f;
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
        parent->children = nextNode;
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
        evtFrameInfo_t *anchorNode = parent->children;
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

#ifdef NON_MATCHING
/* The viewer copies these records through pointers with no alignment guarantee. */
typedef struct __attribute__((packed)) { dds3Effect_Blur_t value; } evtBlurCopy_t;
typedef struct __attribute__((packed)) { dds3Effect_PartialMultiBlurParam_t value; } evtMultiBlurCopy_t;
typedef struct __attribute__((packed)) { dds3Effect_DistortionBlurParam_t value; } evtDistBlurCopy_t;
typedef struct __attribute__((packed)) { dds3Effect_RippleBlurParam_t value; } evtRippleBlurCopy_t;
typedef struct __attribute__((packed)) { dds3Effect_Filter_t value; } evtFilterCopy_t;
typedef struct __attribute__((packed)) { dds3Effect_PartialMultiFilterParam_t value; } evtMultiFilterCopy_t;

extern dds3LightState_t dds3LightParam;
extern f32 sdfFaceDivSoften, sdfFaceDivBias;
#endif

#ifdef NON_MATCHING
extern s32 evtAddNameTable(const char *name);
extern effBlurTemplate_t *Eve_PartialMultiBlurGetParams(void);
extern effBlurScatterWork_t *Eve_DistortionBlurGetParams(void);
extern effBlurScaleWork_t *Eve_RippleBlurGetParams(void);
extern dds3Effect_Filter_t *Eve_FilterGetParams(void);
extern dds3Effect_PartialMultiFilter_t *Eve_PartialMultiFilterGetParams(void);

evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frameNumber)
{
    evtFrameInfo_t *frame = sdfChipAllocDirect(0x20);
    pmdSceneLightTable_t *sceneLight;
    pmdSceneFogTable_t *sceneFog;
    sdf3DLight_t *light;
    void *work;
    s32 nameIndex;

    memset(frame, 0, 0x20);
    frame->frame = frameNumber;
    frame->nameIndex = -1;
    LinkFrameInfo(track, frame);
    switch (track->type) {
    case 2:
        frame->param.sf[0] = 0.5235987305641174f;
        frame->nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        break;
    case 1:
        nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        frame->param.sb[0] = 0;
        frame->nameIndex = nameIndex;
        break;
    case 3:
        frame->nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        frame->param.sb[5] = -1;
        frame->param.sh[3] = -1;
        frame->param.sb[0] = -1;
        frame->param.sh[1] = 0;
        frame->param.sb[4] = 0;
        break;
    case 18:
        nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        ((volatile evtFrameInfo_t *)frame)->param.sh[0] = 0;
        frame->nameIndex = nameIndex;
        frame->param.sh[1] = -1;
        ((volatile evtFrameInfo_t *)frame)->param.sb[0] = 0;
        break;
    case 20:
    case 21:
        frame->nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        frame->param.sb[7] = -1;
        frame->param.sb[0] = -1;
        frame->param.sb[1] = 0;
        frame->param.sh[1] = 0;
        frame->param.sb[4] = -1;
        frame->param.sb[5] = -1;
        frame->param.sb[6] = -1;
        break;
    case 9:
        frame->nameIndex = evtAddNameTable(viewerinfo->nametable[track->entryHeader]);
        frame->param.sh[0] = -1;
        frame->param.sh[1] = 1;
        break;
    case 12:
        frame->param.sh[0] = 1;
        frame->param.sh[1] = 0x80;
        frame->param.sw[1] = 0;
        break;
    case 22:
        frame->param.sh[0] = 1;
        break;
    case 10:
        sceneLight = sdfChipAllocDirect(0x30);
        light = dds3LightParam.parallel[0];
        SDF_VU0_COPYVECTOR(sceneLight->color, &light->color);
        SDF_VU0_COPYVECTOR(sceneLight->direction, &light->v);
        SDF_VU0_COPYVECTOR(sceneLight->ambient, &dds3LightParam.ambient);
        frame->work = sceneLight;
        frame->param.sf[0] = sdfFaceDivSoften;
        frame->param.sf[1] = sdfFaceDivBias;
        frame->nameIndex = 1;
        break;
    case 11:
        sceneFog = sdfChipAllocDirect(0x20);
        sceneFog->fog.vec[0] = dds3LightParam.fog.fogMin;
        sceneFog->fog.color = dds3LightParam.fog.color;
        frame->work = sceneFog;
        sceneFog->fog.vec[1] = dds3LightParam.fog.fogMax;
        sceneFog->fog.vec[2] = dds3LightParam.fog.fogNear;
        sceneFog->fog.vec[3] = dds3LightParam.fog.fogFar;
        frame->param.sh[0] = 1;
        break;
    case 13:
        work = sdfChipAllocDirect(0x28);
        frame->work = work;
        *(volatile evtBlurCopy_t *)work = *(volatile evtBlurCopy_t *)&evtGetUnityCalcArea()->source;
        frame->param.sh[0] = 1;
        break;
    case 14:
        work = sdfChipAllocDirect(0x2C);
        frame->work = work;
        *(volatile evtMultiBlurCopy_t *)work = *(volatile evtMultiBlurCopy_t *)&Eve_PartialMultiBlurGetParams()->body;
        frame->param.sh[0] = 1;
        break;
    case 15:
        work = sdfChipAllocDirect(0x2C);
        frame->work = work;
        *(volatile evtDistBlurCopy_t *)work = *(volatile evtDistBlurCopy_t *)&Eve_DistortionBlurGetParams()->params;
        frame->param.sh[0] = 1;
        break;
    case 23:
        work = sdfChipAllocDirect(0x2C);
        frame->work = work;
        *(volatile evtRippleBlurCopy_t *)work = *(volatile evtRippleBlurCopy_t *)&Eve_RippleBlurGetParams()->params;
        frame->param.sh[0] = 1;
        break;
    case 16:
        work = sdfChipAllocDirect(0x18);
        frame->work = work;
        *(volatile evtFilterCopy_t *)work = *(volatile evtFilterCopy_t *)Eve_FilterGetParams();
        frame->param.sh[0] = 1;
        break;
    case 17:
        work = sdfChipAllocDirect(0x24);
        frame->work = work;
        *(volatile evtMultiFilterCopy_t *)work = *(volatile evtMultiFilterCopy_t *)&Eve_PartialMultiFilterGetParams()->param;
        frame->param.sh[0] = 1;
        break;
    }
    return frame;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtAddFrameInfo);
#endif

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
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
    node = group->children;
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
    track->entryHeader = -1;
    track->fileIndex = -1;
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
    while (obj->children != NULL) {
        DelFrameInfo(obj, obj->children);
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
            model = viewerinfo->workdata->unitTable[i].MajorNum;
            index = viewerinfo->workdata->unitTable[i].MinorNum;
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
        effect = dds3AddD3PEffectObjectBasic2(object->effect.data, position, scale);
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
            effect = dds3AddMG12EffectObjectBasic2(1, object->effect.data);
            break;
        case 21:
            effect = dds3AddMG12EffectObjectBasic2(2, object->effect.data);
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

void RunFrame1(evtViewerObject_s *track, evtFrameInfo_t *frameInfo, s32 nowFrame, s32 length)
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
            frameInfo->work = AddBED(track->effect.data, frameInfo->param.sb[4],
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

void OutFrame1(evtViewerObject_s *track, evtFrameInfo_t *node, s32 nowFrame, s32 length)
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

#ifdef NON_MATCHING
extern evtViewerObject_s *evt_camera;
extern s8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern dds3LightState_t dds3LightParam;
extern f32 sdfFaceDivSoften, sdfFaceDivBias;
extern pmdSceneLightTable_t D_0056CE90;
extern evtBlendVecColor_t D_0056CEC0;
extern dds3Effect_Blur_t D_0056CEE0;
extern dds3Effect_PartialMultiBlurParam_t D_0056CF10;
extern dds3Effect_Filter_t D_0056CF40;
extern dds3Effect_PartialMultiFilterParam_t D_0056CF60;
extern dds3Effect_DistortionBlurParam_t D_0056CF90;
extern dds3Effect_RippleBlurParam_t D_0056CFC0;
extern void evtSetBokasi(s32 enabled);
extern void fldSkySetFilter(s32 index, s32 mode, s32 hokan);
extern void evtSetSkyAlpha(s32 frame, s32 alpha);
extern u16 evtCheckSkyAlphaAnime(void);
extern s32 fldSkyGetAlpha(void);
extern void dds3FadeClear(void);
extern void dds3FadeSet(s32 r, s32 g, s32 b, s32 a);
extern void evtHokanSceneLight(s32 hokan, f32 ratio, f32 *key1, f32 *key2, f32 *dst);
extern f32 evtHokanFloat(s32 hokan, f32 ratio, f32 first, f32 second);
extern void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *first, evtBlendVecColor_t *second, evtBlendVecColor_t *dst);
extern void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *first, dds3Effect_Blur_t *second, dds3Effect_Blur_t *dst);
extern void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *first, dds3Effect_PartialMultiBlurParam_t *second, dds3Effect_PartialMultiBlurParam_t *dst);
extern void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *first, dds3Effect_DistortionBlurParam_t *second, dds3Effect_DistortionBlurParam_t *dst);
extern void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *first, dds3Effect_RippleBlurParam_t *second, dds3Effect_RippleBlurParam_t *dst);
extern void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *first, dds3Effect_Filter_t *second, dds3Effect_Filter_t *dst);
extern void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *first, dds3Effect_PartialMultiFilterParam_t *second, dds3Effect_PartialMultiFilterParam_t *dst);
extern void evtSetUnityCalcArea(void *params);
extern void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *params);
extern void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *params);
extern void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *params);
extern void Eve_FilterSetParam(dds3Effect_Filter_t *params);
extern void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *params);
extern void dds3SetLightColor(s32 frame, s32 light, f32 *color);
extern void dds3SetLightDir(s32 frame, s32 light, f32 *direction);
extern void dds3SetAmbientColor(s32 frame, f32 *color);
extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);

void RunFrame2(evtViewerObject_s *track, evtFrameInfo_t *next, evtFrameInfo_t *previous,
               s32 nowFrame, s32 *nearestFrame)
{
    evtViewerInfo_t *viewer = viewerinfo;
    s32 start = previous != NULL ? previous->frame : viewer->firstframe;
    s32 end = next != NULL ? next->frame : viewer->lastframe;
    f32 ratio = end != start ? (f32)(s32)((u32)nowFrame - (u32)start) / (f32)(s32)((u32)end - (u32)start) : 0.0f;
    s32 hokan = 1, color = 0, alpha = 0, elapsed, duration, remaining, type;
    f32 fade = 1.0f, firstSoften, firstBias, nextSoften, nextBias;
    void *first, *second;
    s16 blurAlpha, blurScale;
    evtFrameInfo_t *after;

    type = track->type;
    switch (type) {
    case 2:
        if (previous != NULL) {
            elapsed = (s32)((u32)nowFrame - previous->frame);
            if (elapsed < *nearestFrame) {
                *nearestFrame = elapsed;
                evt_camera = track;
            }
        }
        break;
    case 22:
        evtSetBokasi(previous != NULL && (s16)previous->param.sh[0] != 0);
        break;
    case 12:
        if (previous != NULL) {
            if (viewer->fldfilter != previous->param.sw[1]) {
                fldSkySetFilter(previous->param.sw[1], (s16)previous->param.sh[0], previous->index);
                evtSetSkyAlpha(previous->index, (s16)previous->param.sh[1]);
                viewerinfo->fldfilter = previous->param.sw[1];
            } else if (evtCheckSkyAlphaAnime() == 0 && fldSkyGetAlpha() != (s16)previous->param.sh[1]) {
                evtSetSkyAlpha(previous->index, (s16)previous->param.sh[1]);
            }
        } else if (viewer->fldfilter != 0) {
            fldSkySetFilter(0, 1, 0);
            viewerinfo->fldfilter = 0;
        }
        break;
    case 6:
        if (previous == NULL) {
            dds3FadeClear();
            break;
        }
        if (previous->index != 0 && nowFrame < previous->frame + previous->index) {
            fade = (f32)(nowFrame - previous->frame) / (f32)previous->index;
        }
        switch ((s16)previous->param.sh[0]) {
        case 0:
            color = 0xFF;
            if (fade != 1.0f) {
                alpha = (s32)(128.0f * (1.0f - fade));
            }
            break;
        case 1:
            color = 0xFF;
            alpha = 0x80;
            if (fade != 1.0f) {
                alpha = (s32)(fade * 128.0f);
            }
            break;
        case 2:
            if (fade != 1.0f) {
                alpha = (s32)(128.0f * (1.0f - fade));
            }
            break;
        case 3:
            alpha = 0x80;
            if (fade != 1.0f) {
                alpha = (s32)(fade * 128.0f);
            }
            break;
        }
        dds3FadeSet(color, color, color, alpha);
        break;
    case 10:
        if (previous != NULL) {
            hokan = previous->nameIndex;
            first = previous->work;
            firstSoften = previous->param.sf[0];
            firstBias = previous->param.sf[1];
        } else {
            first = NULL;
            firstSoften = 7.0f;
            firstBias = 0.0f;
        }
        if (next != NULL) {
            second = next->work;
            nextSoften = next->param.sf[0];
            nextBias = next->param.sf[1];
        } else {
            second = NULL;
            nextSoften = 7.0f;
            nextBias = 0.0f;
        }
        if (first != NULL || second != NULL) {
            evtHokanSceneLight(hokan, ratio, first, second, D_0056CE90.color);
            dds3SetLightColor(0, 0, D_0056CE90.color);
            dds3SetLightDir(0, 0, D_0056CE90.direction);
            dds3SetAmbientColor(0, D_0056CE90.ambient);
            sdfFaceDivSoften = evtHokanFloat(hokan, ratio, firstSoften, nextSoften);
            sdfFaceDivBias = evtHokanFloat(hokan, ratio, firstBias, nextBias);
        }
        break;
    case 11:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 23:
        first = NULL;
        second = NULL;
        if (previous != NULL) {
            hokan = (s16)previous->param.sh[0];
            first = previous->work;
        }
        if (next != NULL) {
            second = next->work;
        }
        if (first == NULL && second == NULL) {
            break;
        }
        switch (type) {
        case 11:
            evtHokanSceneFog(hokan, ratio, first, second, &D_0056CEC0);
            dds3SetFogPara(0, D_0056CEC0.vec[1], D_0056CEC0.vec[0], D_0056CEC0.vec[2], D_0056CEC0.vec[3]);
            dds3LightParam.fog.color = D_0056CEC0.color;
            break;
        case 13:
            evtHokanBlur2(hokan, ratio, first, second, &D_0056CEE0);
            evtSetUnityCalcArea(&D_0056CEE0);
            viewerinfo->drawblur2 = 1;
            break;
        case 14:
            evtHokanMultBlur(hokan, ratio, first, second, &D_0056CF10);
            Eve_PartialMultiBlurSetParam(&D_0056CF10);
            viewerinfo->drawmblur = 1;
            break;
        case 15:
            evtHokanDistBlur(hokan, ratio, first, second, &D_0056CF90);
            Eve_DistortionBlurSetParam(&D_0056CF90);
            viewerinfo->drawdblur = 1;
            break;
        case 23:
            evtHokanRipBlur(hokan, ratio, first, second, &D_0056CFC0);
            Eve_RippleBlurSetParam(&D_0056CFC0);
            viewerinfo->drawrblur = 1;
            break;
        case 16:
            evtHokanFilter(hokan, ratio, first, second, &D_0056CF40);
            Eve_FilterSetParam(&D_0056CF40);
            viewerinfo->filter = 1;
            break;
        case 17:
            evtHokanMultFilter(hokan, ratio, first, second, &D_0056CF60);
            Eve_PartialMultiFilterSetParam(&D_0056CF60);
            viewerinfo->drawmfilter = 1;
            break;
        }
        break;
    case 8:
        blurAlpha = 0;
        blurScale = 0;
        if (previous != NULL) {
            if ((s16)previous->param.sh[2] != 0 && (after = previous->next) != NULL) {
                elapsed = (s32)((u32)nowFrame - previous->frame);
                duration = after->frame - previous->frame;
                remaining = (s32)((u32)duration - (u32)elapsed);
                blurAlpha = (s32)((u32)(s16)previous->param.sh[0] * (u32)remaining +
                                  (u32)(s16)after->param.sh[0] * (u32)elapsed) / duration;
                blurScale = (s32)((u32)(s16)previous->param.sh[1] * (u32)remaining +
                                  (u32)(s16)after->param.sh[1] * (u32)elapsed) / duration;
            } else {
                blurAlpha = previous->param.sh[0];
                blurScale = previous->param.sh[1];
            }
        }
        if (blurAlpha != 0 || blurScale != 0) {
            viewer->flags |= 4;
            dds3ScreenBlurFlag = 1;
            dds3ScreenBlurAlpha = blurAlpha;
            dds3ScreenBlurScale = blurScale;
        } else {
            viewer->flags &= ~4;
            dds3ScreenBlurFlag = 0;
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrame2);
#endif

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
            for (frameInfo = track->children; frameInfo != NULL;
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
                lightBasic = track->basic;
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

#ifdef NON_MATCHING
void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame)
{
    evtFrameInfo_t *frame;
    s32 mode = 0;
    s32 otherMode = 2;

    for (frame = track->children; frame != NULL && nowFrame >= frame->frame;
         frame = frame->next) {
        s32 value = frame->param.sb[0];

        if (value >= 0) {
            if (value < 2) {
                mode = value;
            } else if (value < 5) {
                otherMode = value;
            }
        }
    }
    evtRunUnitMode(track->basic, mode);
    evtRunUnitMode(track->basic, otherMode);
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", RunFrameUnit);
#endif

extern void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame);
extern void RunFrame2(evtViewerObject_s *track, evtFrameInfo_t *next,
                      evtFrameInfo_t *previous, s32 nowFrame, s32 *nearestFrame);
extern evtViewerObject_s *evt_camera;
extern s8 dds3ScreenBlurFlag;

#ifdef NON_MATCHING
void evtRunFrame(s32 nowFrame)
{
    evtViewerObject_s *track;
    evtFrameInfo_t *frame;
    evtFrameInfo_t *next;
    evtFrameInfo_t *previous;
    s32 length;
    s32 endFrame;
    s32 nearestFrame = 10000;

    viewerinfo->drawblur2 = 0;
    track = viewerinfo->firstobjinfo;
    viewerinfo->drawmblur = 0;
    viewerinfo->filter = 0;
    viewerinfo->flags &= ~6;
    viewerinfo->drawmfilter = 0;
    viewerinfo->drawdblur = 0;
    viewerinfo->drawrblur = 0;
    for (; track != NULL; track = track->next) {
        if (track->type == 1) {
            RunFrameUnit(track, nowFrame);
            continue;
        }
        for (frame = track->children; frame != NULL; frame = frame->next) {
            switch (track->type) {
            case 3:
            case 20:
            case 21:
                endFrame = (s16)frame->param.sh[1];
                if (endFrame == 0) {
                    length = viewerinfo->totalframe - frame->frame;
                } else {
                    length = endFrame - frame->frame;
                    if (length < 0) length = 0;
                }
                break;
            case 18:
                endFrame = (s16)frame->param.sh[0];
                if (endFrame == 0) {
                    length = viewerinfo->totalframe - frame->frame;
                } else {
                    length = endFrame - frame->frame;
                    if (length < 0) length = 0;
                }
                break;
            case 9:
                for (next = frame->next; next != NULL; next = next->next) {
                    if ((s16)next->param.sh[0] == (s16)frame->param.sh[0]) break;
                }
                if (next != NULL) {
                    length = next->frame - frame->frame;
                } else {
                    length = viewerinfo->totalframe - frame->frame;
                }
                break;
            default:
                length = frame->index;
                break;
            }
            if ((length == 0 && nowFrame == frame->frame) ||
                (length > 0 && nowFrame >= frame->frame && nowFrame < frame->frame + length)) {
                RunFrame1(track, frame, nowFrame, length);
            } else {
                OutFrame1(track, frame, nowFrame, length);
            }
        }
    }
    track = viewerinfo->firstobjinfo;
    evt_camera = NULL;
    for (; track != NULL; track = track->next) {
        for (frame = track->children; frame != NULL && nowFrame >= frame->frame; frame = frame->next) {
        }
        previous = frame != NULL ? frame->prev : track->lastChild;
        RunFrame2(track, frame, previous, nowFrame, &nearestFrame);
    }
    if (evt_camera != NULL) {
        viewerinfo->gamecamera = evt_camera->basic;
        SetActiveCamera();
    }
    RunFrame3(nowFrame);
    if (viewerinfo->flags & 1) {
        if (!(viewerinfo->flags & 2)) {
            dds3StopQuake();
        }
        if (!(viewerinfo->flags & 4)) {
            dds3ScreenBlurFlag = 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtRunFrame);
#endif

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


extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);
extern void sdfMotionPause(sdfMotionCtrlr_t *motion);
extern void sdfMotionPauseOff(sdfMotionCtrlr_t *motion);

#ifdef NON_MATCHING
void evtStopFrame(s32 sw, u32 nowFrame)
{
    dds3Basic_t *scene;
    dds3SceneBasic_t *sceneWork;
    dds3Basic_t *basic;
    dds3Basic_t *effect;
    dds3Basic_t *mover;
    dds3UnitObjectBasicWorkData_t *unitWork;
    dds3MatterObjectBasicWorkData_t *matterWork;
    dds3ObjectBaseControlWorkData_t *control;
    mdlObj_t *model;
    sdfMotionCtrlr_t *motion;
    evtViewerObject_s *track;
    evtFrameInfo_t *frame;
    s32 slot;

    if (dds3GetActiveScene() == NULL) {
        return;
    }
    scene = dds3GetActiveScene();
    sceneWork = scene->workdata;
    for (basic = sceneWork->basictable[BASIC_TYPE_UNITOBJECT].firstbasic;
         basic != NULL; basic = basic->next) {
        unitWork = basic->workdata;
        control = unitWork->pObjectBaseControlWorkData;
        model = control->pModel;
        if (sw == 0) {
            sdfMotionSeek(model->first, (f32)nowFrame);
            sdfMotionPause(model->first);
        } else {
            sdfMotionPauseOff(model->first);
        }
    }
    for (basic = sceneWork->basictable[BASIC_TYPE_MATTEROBJECT].firstbasic;
         basic != NULL; basic = basic->next) {
        matterWork = basic->workdata;
        motion = matterWork->pObjectBaseControlWorkData->pMot;
        if (motion != NULL) {
            if (sw == 0) {
                sdfMotionSeek(motion, (f32)nowFrame);
                sdfMotionPause(motion);
            } else {
                sdfMotionPauseOff(motion);
            }
        }
    }
    for (basic = sceneWork->basictable[BASIC_TYPE_MOVER].firstbasic;
         basic != NULL; basic = basic->next) {
        evtStopMover((polyMovieObject_t *)basic, sw, 0, nowFrame);
    }
    for (track = viewerinfo->firstobjinfo; track != NULL; track = track->next) {
        if (track->type != 3 && track->type != 18 && track->type != 20 && track->type != 21) {
            continue;
        }
        for (frame = track->children; frame != NULL; frame = frame->next) {
            if (track->type == 18) {
                effect = frame->work;
            } else if (track->type == 3 || track->type == 20 || track->type == 21) {
                slot = frame->param.sb[0];
                if (slot < 0) continue;
                effect = viewerinfo->effectBasic[slot];
            } else {
                continue;
            }
            if (effect != NULL) {
                mover = dds3GetBaseControlWorkData(effect)->pBasic[1];
                if (mover != NULL) {
                    evtStopMover((polyMovieObject_t *)mover, sw, frame->frame, nowFrame);
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtStopFrame);
#endif

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
                viewerinfo->fldfilter = 0;
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

#ifdef NON_MATCHING
extern char *D_0032C6E8[], *D_0032C6F8[], *D_0032C738[];
extern char *evtFrameListTable[], *evtFrameFadeNames[];
extern char D_0054F8B8[], D_0054F8C0[], D_0054F8C8[], D_0054F8D0[];
extern s32 mdlDebugPrintEffectScale(dds3ProcessID_t *id);
extern s32 mdlDebugPrintEffectScale2(dds3ProcessID_t *id);
extern s32 soften_bias_Process(dds3ProcessID_t *id);
extern s32 evtViewerExit(void);
extern s32 mfilter_Process(void);
extern void mblurSetParam(dds3Effect_PartialMultiBlurParam_t *params);
extern void dblurSetParam(dds3Effect_DistortionBlurParam_t *params);
extern void rblurSetParam(dds3Effect_RippleBlurParam_t *params);
extern void dblurSetParam2(dds3Effect_Filter_t *params);
extern void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *params);

s32 EnterFrameList(s32 action)
{
    evtViewerObject_s *track = viewerinfo->framelist_target;
    evtFrameInfo_t *frame;
    pmdSceneLightTable_t *light;
    pmdSceneFogTable_t *fog;
    dds3ProcessID_t *process;
    dds3ProcessFunc_t callback;
    char *processName;
    s32 type, field;

    if (viewerinfo->frameCursor + viewerinfo->frameFirst >= track->childCount) {
        if (action == 1) {
            AddFrameInfo2(track, viewerinfo->nowframe, viewerinfo);
        }
        return 0;
    }
    if (action == -2) {
        viewerinfo->itemNames = D_0032C6E8;
        viewerinfo->framelist_type = -1;
        viewerinfo->menuItemCount = 2;
        viewerinfo->select_header = "DELETE FRAME.OK? ";
        viewerinfo->select_y = 0;
        PushMenu(2, 0xD8, 0x78);
        return 0;
    }
    viewerinfo->framelist_type = D_0032C9E2[viewerinfo->framelist_x + track->type * 10];
    frame = evtGetSelectFrameInfo();
    field = viewerinfo->framelist_type;
    switch (field) {
    case 0:
        viewerinfo->value = frame->frame;
        PushMenu(6, 0xB4, 0x78);
        break;
    case 9:
        type = viewerinfo->framelist_target->type;
        if (type == 18) {
            viewerinfo->value = (s16)frame->param.sh[0];
            PushMenu(6, 0xB4, 0x78);
        } else if (type == 3 || type == 20 || type == 21) {
            viewerinfo->value = (s16)frame->param.sh[1];
            PushMenu(6, 0xB4, 0x78);
        }
        break;
    case 15:
        viewerinfo->value = frame->index;
        PushMenu(6, 0xB4, 0x78);
        break;
    case 1:
        viewerinfo->min = 0;
        viewerinfo->max = 10000;
        viewerinfo->value = frame->index;
        PushMenu(7, 0xB4, 0x78);
        break;
    case 2:
        if (viewerinfo->framelist_target->type == 7) {
            viewerinfo->min = 0;
            viewerinfo->max = 3000;
            viewerinfo->value = (s16)frame->param.sh[0];
            PushMenu(7, 0xB4, 0x78);
        }
        break;
    case 3:
        type = viewerinfo->framelist_target->type;
        if (type == 8) {
            viewerinfo->max = 255;
            viewerinfo->value = (s16)frame->param.sh[0];
            viewerinfo->min = 0;
        } else if (type == 12) {
            viewerinfo->min = -255;
            viewerinfo->max = 255;
            viewerinfo->value = (s16)frame->param.sh[1];
        }
        PushMenu(7, 0xB4, 0x78);
        break;
    case 4:
        viewerinfo->max = 255;
        viewerinfo->min = -255;
        viewerinfo->value = (s16)frame->param.sh[1];
        PushMenu(7, 0xB4, 0x78);
        break;
    case 5:
        type = viewerinfo->framelist_target->type;
        if (type == 8) {
            viewerinfo->select_y = (s16)frame->param.sh[2];
        }
        else if (type == 10) {
            viewerinfo->select_y = frame->nameIndex;
        }
        else if (type == 11 || type == 13 || type == 14 || type == 15 ||
                 type == 16 || type == 17 || type == 23) viewerinfo->select_y = (s16)frame->param.sh[0];
        viewerinfo->itemNames = evtFrameListTable;
        viewerinfo->menuItemCount = 2;
        viewerinfo->select_header = "HOKAN MODE   ";
        PushMenu(2, 0xD8, 0x78);
        break;
    case 11:
        type = viewerinfo->framelist_target->type;
        if (type == 9 || type == 22) {
            viewerinfo->select_y = (s16)frame->param.sh[type == 9 ? 1 : 0];
            viewerinfo->menuItemCount = 2;
            viewerinfo->itemNames = evtFrameListTable;
            viewerinfo->select_header = "ON/OFF MODE ";
            PushMenu(2, 0xD8, 0x78);
        }
        break;
    case 16:
        type = viewerinfo->framelist_target->type;
        if (type == 3) {
            viewerinfo->select_y = frame->param.sb[4];
        }
        else if (type == 20 || type == 21) {
            viewerinfo->select_y = frame->param.sb[1];
        }
        viewerinfo->itemNames = evtFrameListTable;
        viewerinfo->menuItemCount = 2;
        viewerinfo->select_header = "ON/OFF MODE ";
        PushMenu(2, 0xD8, 0x78);
        break;
    case 12:
        type = viewerinfo->framelist_target->type;
        if (type == 6) {
            viewerinfo->select_y = (s16)frame->param.sh[0];
            viewerinfo->menuItemCount = 4;
            viewerinfo->itemNames = evtFrameFadeNames;
            viewerinfo->select_header = "FADE TYPE  ";
        } else if (type == 1) {
            viewerinfo->select_y = frame->param.sb[0];
            viewerinfo->menuItemCount = 5;
            viewerinfo->itemNames = D_0032C738;
            viewerinfo->select_header = "UNIT CONTROL TYPE ";
        } else if (type == 12) {
            viewerinfo->select_y = (s16)frame->param.sh[0];
            viewerinfo->menuItemCount = 3;
            viewerinfo->itemNames = D_0032C6F8;
            viewerinfo->select_header = "TRANS MODE ";
        } else if (type == 19) {
            viewerinfo->select_y = (s16)frame->param.sh[0];
            viewerinfo->menuItemCount = 7;
            viewerinfo->itemNames = evtFrameFadeNames + 4;
            viewerinfo->select_header = "CONTROL TYPE ";
        } else {
            break;
        }
        PushMenu(2, 0xD8, 0x78);
        break;
    case 6:
        viewerinfo->floatMin = 1.0f;
        viewerinfo->floatMax = 180.0f;
        viewerinfo->fvalue = frame->param.sf[0] * 57.2957763671875f;
        PushMenu(8, 0xB4, 0x78);
        break;
    case 8:
        type = viewerinfo->framelist_target->type;
        if (type == 5) {
            viewerinfo->value = (s16)frame->param.sh[0];
            viewerinfo->min = 0;
            viewerinfo->max = 255;
        } else if (type == 19) {
            viewerinfo->value = (s16)frame->param.sh[1];
            *(volatile s32 *)&viewerinfo->min = 0;
            if ((s16)frame->param.sh[0] == 3) {
                *(volatile s32 *)&viewerinfo->min = -1;
            }
            viewerinfo->max = 255;
        } else if (type == 3) {
            viewerinfo->value = frame->param.sb[5];
            viewerinfo->min = -1;
            viewerinfo->max = 127;
        } else if (type == 12) {
            viewerinfo->value = frame->param.sw[1];
            viewerinfo->min = 0;
            viewerinfo->max = 255;
        } else if (type == 18) {
            viewerinfo->value = frame->param.sb[4];
            viewerinfo->min = 0;
            viewerinfo->max = 2;
        } else {
            break;
        }
        PushMenu(7, 0xB4, 0x78);
        break;
    case 7:
        viewerinfo->min = 0;
        viewerinfo->max = 127;
        viewerinfo->value = (s16)frame->param.sh[1];
        PushMenu(7, 0xB4, 0x78);
        break;
    case 10:
        viewerinfo->work_y = 0;
        viewerinfo->work_base = 0;
        PushMenu(10, 0x9C, 0x54);
        break;
    case 13:
        type = viewerinfo->framelist_target->type;
        processName = NULL;
        callback = NULL;
        switch (type) {
        case 20:
        case 21:
            viewerinfo->eventpos_y = 0;
            PushMenu(12, 0x78, 0x54);
            return 0;
        case 10:
            light = frame->work;
            dds3SetLightColor(0, 0, light->color);
            dds3SetLightDir(0, 0, light->direction);
            dds3SetAmbientColor(0, light->ambient);
            processName = D_0054F8B8;
            callback = mdlDebugPrintEffectScale;
            break;
        case 11:
            fog = frame->work;
            dds3SetFogPara(0, fog->fog.vec[1], fog->fog.vec[0], fog->fog.vec[2], fog->fog.vec[3]);
            dds3LightParam.fog.color = fog->fog.color;
            processName = D_0054F8C0;
            callback = mdlDebugPrintEffectScale2;
            break;
        case 13:
            evtSetUnityCalcArea(frame->work);
            viewerinfo->drawblur2 = 0;
            break;
        case 14:
            mblurSetParam(frame->work);
            viewerinfo->drawmblur = 0;
            break;
        case 15:
            dblurSetParam(frame->work);
            viewerinfo->drawdblur = 0;
            break;
        case 23:
            rblurSetParam(frame->work);
            viewerinfo->drawrblur = 0;
            break;
        case 16:
            dblurSetParam2(frame->work);
            viewerinfo->filter = 0;
            processName = D_0054F8C8;
            callback = (dds3ProcessFunc_t)evtViewerExit;
            break;
        case 17:
            mfilterSetParam(frame->work);
            viewerinfo->drawmfilter = 0;
            processName = D_0054F8D0;
            callback = (dds3ProcessFunc_t)mfilter_Process;
            break;
        default:
            return 0;
        }
        if (processName != NULL) {
            process = dds3StartProcess(processName, 0x2AF9, 1, 1, callback, NULL, (void *)1);
            viewerinfo->menuProcess = process;
        }
        PushMenu(11, 0, 0);
        break;
    case 14:
        sdfFaceDivSoften = frame->param.sf[0];
        sdfFaceDivBias = frame->param.sf[1];
        process = dds3StartProcess("soften_bias", 0x2AF9, 1, 1, soften_bias_Process, NULL, (void *)1);
        viewerinfo->menuProcess = process;
        PushMenu(11, 0, 0);
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", EnterFrameList);
#endif

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

#ifdef NON_MATCHING
s32 EnterBasicList(void)
{
    evtFrameInfo_t *frame = evtGetSelectFrameInfo();
    dds3Basic_t *selected = NULL;
    dds3Basic_t *basic;
    dds3Basic_t *scene;
    dds3SceneBasic_t *sceneWork;
    s32 count = 0;
    s32 type;
    s32 i;
    s32 nameIndex;
    s8 field;

    if (frame == NULL) {
        return 0;
    }
    if (viewerinfo->work_base + viewerinfo->work_y != 0) {
        for (i = 0; i < 18; i++) {
            if (i == 3) {
                continue;
            }
            scene = dds3GetActiveScene();
            sceneWork = scene->workdata;
            for (basic = sceneWork->basictable[i].firstbasic; basic != NULL; basic = basic->next) {
                if (basic->name != NULL && ++count == viewerinfo->work_base + viewerinfo->work_y) {
                    selected = basic;
                    break;
                }
            }
            if (selected != NULL) {
                break;
            }
        }
    }
    type = viewerinfo->framelist_target->type;
    field = D_0032C9E2[viewerinfo->framelist_x + type * 10];
    if (field == 10) {
        switch (type) {
        case 3:
            nameIndex = selected != NULL ? evtAddNameTable(selected->name) : -1;
            frame->param.sh[3] = nameIndex;
            break;
        case 9:
            scene = dds3GetActiveScene();
            basic = dds3SceneBasicSearchNameAndType(scene, BASIC_TYPE_UNITOBJECT,
                                                    viewerinfo->nametable[(s16)frame->param.sh[0]]);
            if (basic != NULL) {
                evtUnlinkLightObjectBasic(evtGetUnitHandle2(basic), 0);
            }
            nameIndex = selected != NULL ? evtAddNameTable(selected->name) : -1;
            frame->param.sh[0] = nameIndex;
            break;
        case 18:
            nameIndex = selected != NULL ? evtAddNameTable(selected->name) : -1;
            frame->param.sh[1] = nameIndex;
            break;
        }
    } else if (field == 13 && type >= 20 && type < 22) {
        nameIndex = selected != NULL ? evtAddNameTable(selected->name) : -1;
        frame->param.sb[4 + viewerinfo->eventpos_y] = nameIndex;
    }
    evtRunFrame(viewerinfo->nowframe);
    PopMenu();
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", EnterBasicList);
#endif

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

#ifdef NON_MATCHING
extern dds3Effect_PartialMultiBlurParam_t *mblurGetParam(void);
extern dds3Effect_DistortionBlurParam_t *dblurGetParam(void);
extern dds3Effect_RippleBlurParam_t *rblurGetParam(void);
extern dds3Effect_Filter_t *dblurGetParam2(void);
extern dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void);

s32 EnterData(void)
{
    evtFrameInfo_t *frame = evtGetSelectFrameInfo();
    evtViewerInfo_t *viewer = viewerinfo;
    evtViewerInfo_t *volatile *currentViewer = &viewerinfo;
    void *destination;
    pmdSceneFogTable_t *fog;

    switch (viewer->framelist_target->type) {
    case 10:
        if (viewer->framelist_type == 13) {
            sdf3DLight_t *light = dds3LightParam.parallel[0];
            pmdSceneLightTable_t *sceneLight = frame->work;
            SDF_VU0_COPYVECTOR(sceneLight->color, &light->color);
            SDF_VU0_COPYVECTOR(sceneLight->direction, &light->v);
            SDF_VU0_COPYVECTOR(sceneLight->ambient, &dds3LightParam.ambient);
        } else if (viewer->framelist_type == 14) {
            frame->param.sf[0] = sdfFaceDivSoften;
            frame->param.sf[1] = sdfFaceDivBias;
            viewer = *currentViewer;
        }
        break;
    case 11:
        fog = frame->work;
        viewer = *currentViewer;
        fog->fog.vec[0] = dds3LightParam.fog.fogMin;
        fog->fog.color = dds3LightParam.fog.color;
        fog->fog.vec[1] = dds3LightParam.fog.fogMax;
        fog->fog.vec[2] = dds3LightParam.fog.fogNear;
        fog->fog.vec[3] = dds3LightParam.fog.fogFar;
        break;
    case 13:
        destination = frame->work;
        *(volatile evtBlurCopy_t *)destination = *(volatile evtBlurCopy_t *)&evtGetUnityCalcArea()->source;
        viewer = *currentViewer;
        break;
    case 14:
        destination = frame->work;
        *(volatile evtMultiBlurCopy_t *)destination = *(volatile evtMultiBlurCopy_t *)mblurGetParam();
        viewer = *currentViewer;
        break;
    case 15:
        destination = frame->work;
        *(volatile evtDistBlurCopy_t *)destination = *(volatile evtDistBlurCopy_t *)dblurGetParam();
        viewer = *currentViewer;
        break;
    case 23:
        destination = frame->work;
        *(volatile evtRippleBlurCopy_t *)destination = *(volatile evtRippleBlurCopy_t *)rblurGetParam();
        viewer = *currentViewer;
        break;
    case 16:
        destination = frame->work;
        *(volatile evtFilterCopy_t *)destination = *(volatile evtFilterCopy_t *)dblurGetParam2();
        viewer = *currentViewer;
        break;
    case 17:
        destination = frame->work;
        *(volatile evtMultiFilterCopy_t *)destination = *(volatile evtMultiFilterCopy_t *)mfilterGetParam();
        viewer = *currentViewer;
        break;
    }
    evtRunFrame(viewer->nowframe);
    PopMenu();
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", EnterData);
#endif

extern s16 winoff_x;
extern s32 evtViewerDispatch(s32 actionIndex, s32 x, s32 y);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern s32 (*D_0032C750[])(s32 action, s32 previousMenu);

#ifdef NON_MATCHING
s32 evtEventViewerProcess4(void)
{
    s32 index;
    s32 previousMenu = 0;
    s32 done = 0;
    s32 action;
    s32 result;
    evtViewerMenuEntry_t *entry;
    evtViewerInfo_t *viewer;

    DrawEffect();
    for (index = 1; index <= viewerinfo->sp; index++) {
        entry = &viewerinfo->menuStack[index];
        action = evtViewerDispatch(entry->mode, entry->unk02 + winoff_x, entry->unk04);
        entry = &viewerinfo->menuStack[index];
        if (viewerinfo->nowmenu == entry->mode && viewerinfo->nowmenu == 4) {
            evtViewerDispatch(5, winoff_x + 0x90, 0x48);
        }
        if (action == 0) {
            previousMenu = viewerinfo->menuStack[index].mode;
            continue;
        }
        if (action == -1) {
            PopMenu();
        } else {
            result = D_0032C750[viewerinfo->menuStack[index].mode](action, previousMenu);
            if (result == 1) {
                return (s32)evtEventViewerProcess5_0;
            }
            if (result == -1) {
                done = 1;
            }
        }
        break;
    }

    if (winoff_x < 11 && dds3PadButtonState[0x29] != 0) {
        winoff_x += 10;
    } else if (winoff_x >= -99 && dds3PadButtonState[0x2B] != 0) {
        winoff_x -= 10;
    }
    viewer = viewerinfo;
    if (dds3PadButtonState[0x22] < 0) {
        if (viewer->menuProcess != NULL) {
            dds3KillProcess(viewer->menuProcess, 1);
            viewerinfo->menuProcess = NULL;
            viewer = viewerinfo;
        }
        if (viewer->menusw == 0) {
            viewer->nowmenu = 0;
            viewer->menusw = 1;
            viewer->sp = 0;
            PushMenu(4, 0x24, 0x18);
            viewer = viewerinfo;
        } else if (viewer->menusw == 1) {
            viewer->menusw = 0;
            done = 1;
            viewer = viewerinfo;
        }
    }
    if (viewer->nowmenu == 0 || done) {
        viewer->menusw = 0;
        viewer->nowmenu = 0;
        viewer->sp = 0;
        return (s32)evtEventViewerProcess3;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcess4);
#endif

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
    process = dds3StartProcess("EventViewer", 0x3E8, 1, 1, (dds3ProcessFunc_t)evtEventViewerProcess1,
                               (dds3ProcessFunc_t)evtEventViewerShutdown, viewer);
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

