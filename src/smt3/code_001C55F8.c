#include "common.h"
#include "sdfFrame.h"
#include "newbattle.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

typedef struct nbCameraAnim_t {
    s32 rotate_f;
    f32 rotate_s;
    s32 distance_f;
    f32 distance_s;
} nbCameraAnim_t;

typedef struct nbCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 state_change;
    u32 state;
    u32 state_old;
    u32 state_old2;
    u32 phase;
    s32 frame;
    u32 type;
    f32 angle;
    nbCameraAnim_t anim;
} nbCamera_t;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct nbCameraHelper_tag {
    sdfVector4_t target;
    sdfVector4_t eye;
    sdfVector4_t up;
    s32 frame;
    s32 frameMax;
    effPartHandle_t *hPB;
} nbCameraHelper_tag;

extern void *sdfChipAllocDirect(s32 aSize);
extern void dds3EffectPartDraw(effPartHandle_t *hEffPart);

extern effPartHandle_t *nbCameraHelperFileLoadPB(const char *pFilename);

/* 128-byte party records; this unit only reads the formation index. */


extern f32 nbCameraBossVecTable[];

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, u32 aId);

extern void *dds3EffectFileListCreate(const char *dir, s32 type);

extern void *dds3EffectFileListWindowCreate(void *a);

extern void dds3EffectFileListWindowSetPos(void *a, s32 b, s32 c);

extern void dds3EffectFileListWindowSetOt(void *hEffFLWin, s32 ot);

extern void dds3EffectFileListWindowSetPadWait(void *hEffFLWin, s32 wait);

extern void dds3EffectFileListWindowSetWindowColor(void *hEffFLWin, u32 frame_c, u32 bg_c);

extern const char nbCameraHelperViewerPBDir[];

extern u32 nbCameraStartAnimWaitFrames;

extern const char nbCameraHelperMsgNotTarget[];

extern const char nbCameraHelperMsgNotEye[];

#include "sdk_libc.h"

extern s32 nbCameraBossCutFlag;

extern f32 nbCameraBossVecTable[];

extern char *nbCameraHelperViewerMode;

extern nbCameraHelper_tag *nbCameraHelperViewerCurrent;

extern sdfMemHandle_t *nbCameraHelperViewerStateHandle;

extern sdfMemHandle_t *nbCameraHelperViewerTargetHandle;

extern sdfVector4_t *nbCameraHelperViewerState;

extern sdfVector4_t *nbCameraHelperViewerHandle;

extern u8 nbCameraStartReadWaitActive;

extern u32 nbCameraStartReadWaitFrames;

extern u8 nbCameraStartAnimWaitActive;

extern u32 nbCameraStartAnimWaitFrames;

extern nbCamera_t pNbCamera;

extern s8 nbCameraPressRunFlag;

extern u32 nbCameraPressThrough;

extern u8 nbCameraCRSetPlayerFormRequestFlag;

extern u32 nbCameraBoss_BSBERU_E915_Frame;

extern f32 nbCameraBoss_BSBERU_E915_Rate;

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void nbCameraHelperModelSetSub(mdlObj_t *hModel, u32 target, u32 eye);

extern u32 nbCameraBoss_BSARI_MASEHA_Frame;

extern s32 nbCameraBoss_BSARI_MASEHA_Type;

extern void *nbCameraHelperViewerPBWork;

extern void *nbCameraHelperViewerPBHandle;

extern mdlObj_t *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

extern void dds3EffectPartDelete(effPartHandle_t *hEffPart);

extern void sdfChipFreeDirect(void *aPtr);

extern void dds3EffectFileListWindowDelete(void *a);

extern void dds3EffectFileListDelete(void *a);

extern s32 nbCameraBoss_BSARI_SIMANO_Type;

extern f32 nbCameraBoss_BSARI_SIMANO_Rate;

extern u8 nbCameraBoss_Pish_MUHENK_Type;

extern u32 nbCameraBoss_Pish_MUHENK_Frame;

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void *sdfFileOpen(const char *path);
extern u32 sdfFileLength(void *file);
extern s32 sdfFileRead(void *file, void *buffer, s32 size);
extern void sdfFileClose(void *file);
extern void *memcpy(void *dst, const void *src, size_t size);
extern effPartHandle_t *dds3EffectPartCreate(u16 type, void *data);
extern void mdlSingleMotion(mdlObj_t *model, s32 group, s32 number);

effPartHandle_t *nbCameraHelperFileLoadPB(const char *filename)
{
    struct {
        u32 size;
        u8 reserved[12];
    } header;
    void *file = sdfFileOpen(filename);
    sdfMemHandle_t *allocation;
    void *data;
    effPartHandle_t *part;

    header.size = sdfFileLength(file);
    allocation = sdfMemAllocDirect(header.size + sizeof(header));
    data = sdfMemLock(allocation);
    /* As in effBTLSystem_Part_DataLoad, only the header size is initialized. */
    memcpy(data, &header, sizeof(header));
    sdfFileRead(file, (u8 *)data + sizeof(header), header.size);
    sdfFileClose(file);
    part = dds3EffectPartCreate(3, data);
    mdlSingleMotion(dds3EffectPartGetHandle(part), 0, 0);
    sdfMemFreeDirect(allocation);
    printf("nbCameraHelper:file[%s]\n", filename);
    return part;
}


/* Called above its definition, where ee-gcc assumed int. */
u16 nbCameraHelperLength(nbCameraHelper_tag *hCameraHelper);

nbCameraHelper_tag *nbCameraHelperCreate(const char *pFilename)
{
    nbCameraHelper_tag *pHelper = sdfChipAllocDirect(0x3C);

    pHelper->hPB = nbCameraHelperFileLoadPB(pFilename);
    pHelper->frame = 0;
    pHelper->frameMax = nbCameraHelperLength(pHelper);
    return pHelper;
}

void nbCameraHelperDelete(nbCameraHelper_tag *hCameraHelper)
{
    dds3EffectPartDelete(hCameraHelper->hPB);
    sdfChipFreeDirect(hCameraHelper);
}

u16 nbCameraHelperLength(nbCameraHelper_tag *hCameraHelper)
{
    return dds3EffectPartGetHandle(hCameraHelper->hPB)->first->frameLength;
}

void nbCameraHelperRun(nbCameraHelper_tag *hCameraHelper)
{
    mdlObj_t *model;
    nbCamera_t *camera;
    s32 frameMax = hCameraHelper->frameMax;
    f32 length;

    if (hCameraHelper->frame < SDF_FRAMES(frameMax)) {
        camera = &pNbCamera;
        dds3EffectPartDraw(hCameraHelper->hPB);
        model = dds3EffectPartGetHandle(hCameraHelper->hPB);
        if (sdfModelGetMarkPos(model->inner, 0) == 0) {
            printf(nbCameraHelperMsgNotTarget);
        }
        SDF_VU0_STOREVECTOR(camera);
        SDF_VU0_STOREVECTOR(&hCameraHelper->target);
        if (sdfModelGetMarkPos(model->inner, 1) == 0) {
            printf(nbCameraHelperMsgNotEye);
        }
        SDF_VU0_STOREVECTOR(&hCameraHelper->eye);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(camera);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(length);
        camera->distance = length;
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&camera->direction);
        camera->fovy = 0.7853981f;
        hCameraHelper->frame++;
    }
}

void nbCameraHelperModelSetSub(mdlObj_t *hModel, u32 target, u32 eye)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 length;

    if (sdfModelGetMarkPos(hModel->inner, target) == 0) {
        printf(nbCameraHelperMsgNotTarget);
    }
    SDF_VU0_STOREVECTOR(pCamera);
    if (sdfModelGetMarkPos(hModel->inner, eye) == 0) {
        printf(nbCameraHelperMsgNotEye);
    }
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(pCamera);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    pCamera->distance = length;
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&pCamera->direction);
    pCamera->fovy = 0.7853981f;
}

void nbCameraHelperModelSet(mdlObj_t *hModel)
{
    nbCameraHelperModelSetSub(hModel, 0, 1);
}

s32 nbCameraHelperViewer_PB_Init(s32 pData)
{
    void *hFileList = dds3EffectFileListCreate(nbCameraHelperViewerPBDir, 0x10);
    void *pWork;

    nbCameraHelperViewerPBHandle = hFileList;
    pWork = dds3EffectFileListWindowCreate(hFileList);
    nbCameraHelperViewerPBWork = pWork;
    dds3EffectFileListWindowSetPos(pWork, 0xF0, 0xD0);
    dds3EffectFileListWindowSetOt(nbCameraHelperViewerPBWork, 1);
    dds3EffectFileListWindowSetPadWait(nbCameraHelperViewerPBWork, 0x53);
    dds3EffectFileListWindowSetWindowColor(nbCameraHelperViewerPBWork, 0x80806020, 0x80000000);
    nbCameraHelperViewerMode = NULL;
    return pData;
}

/* Callback slots follow the viewer record used by func_001AA310. */
typedef struct effDebugViewer_t {
    u8 reserved00[0x18];
    struct effDebugViewer_t *replacement;
    void *createParam;
    u8 reserved20[0xC];
    s32 (*create)(void *);
    u8 reserved30[4];
    void (*destroy)(void *);
    s32 *handle;
} effDebugViewer_t;

extern effDebugViewer_t nbCameraHelperViewerPBParam;
extern char nbCameraHelperViewerModeBuf[];
extern void dds3EffectFileListWindowDraw(void *window);
extern u32 dds3EffectFileListWindowGetState(void *window);
extern u32 dds3EffectFileListWindowGetFilename(void *window, char *filename);
extern void nbSetHelperViewer(void *viewer);

void nbCameraHelperViewer_PB_Draw(void *pData)
{
    effDebugViewer_t *viewer = NULL;

    dds3EffectFileListWindowDraw(nbCameraHelperViewerPBWork);
    if (dds3EffectFileListWindowGetState(nbCameraHelperViewerPBWork) == 1) {
        void (*destroy)(void *);
        s32 (*create)(void *);

        dds3EffectFileListWindowGetFilename(nbCameraHelperViewerPBWork, nbCameraHelperViewerModeBuf);
        nbCameraHelperViewerMode = nbCameraHelperViewerModeBuf;
        destroy = nbCameraHelperViewerPBParam.destroy;
        if (destroy != NULL) {
            if (nbCameraHelperViewerPBParam.handle != NULL) {
                destroy(EE_ADDR_AS(void *, *nbCameraHelperViewerPBParam.handle));
            } else {
                destroy(NULL);
            }
        }
        if (nbCameraHelperViewerPBParam.replacement != NULL) {
            viewer = nbCameraHelperViewerPBParam.replacement;
        }
        /* BUG: with no replacement viewer is still NULL here and retail reads through it. */
#ifdef FIX_BUGS
        if (viewer == NULL) {
            return;
        }
#endif
        create = viewer->create;
        if (create != NULL) {
            if (viewer->createParam != NULL) {
                *viewer->handle = create(viewer->createParam);
            } else {
                create(NULL);
            }
        }
        nbSetHelperViewer(viewer);
    }
}


void nbCameraHelperViewer_PB_End(void *pData)
{
    dds3EffectFileListWindowDelete(nbCameraHelperViewerPBWork);
    dds3EffectFileListDelete(nbCameraHelperViewerPBHandle);
}

s32 nbCameraHelperViewer_Init(s32 pData)
{
    s32 size;

    if (nbCameraHelperViewerMode != NULL) {
        if (nbCameraHelperViewerCurrent != NULL) {
            nbCameraHelperDelete(nbCameraHelperViewerCurrent);
            if (nbCameraHelperViewerStateHandle != NULL) {
                sdfMemFreeDirect(nbCameraHelperViewerStateHandle);
            }
            if (nbCameraHelperViewerTargetHandle != NULL) {
                sdfMemFreeDirect(nbCameraHelperViewerTargetHandle);
            }
        }
        nbCameraHelperViewerCurrent = nbCameraHelperCreate(nbCameraHelperViewerMode);
        nbCameraHelperViewerMode = NULL;
        size = nbCameraHelperLength(nbCameraHelperViewerCurrent) * 16;
        nbCameraHelperViewerStateHandle = sdfMemAllocDirect(size);
        nbCameraHelperViewerState = sdfMemLock(nbCameraHelperViewerStateHandle);
        nbCameraHelperViewerTargetHandle = sdfMemAllocDirect(size);
        nbCameraHelperViewerHandle = sdfMemLock(nbCameraHelperViewerTargetHandle);
    }
    return pData;
}

extern u8 nbCameraHelperViewerShowFrame;
extern u8 nbCameraHelperViewerShowLines;
extern const char nbCameraHelperViewerFrameFormat[];

extern void effDebugStringDraw(s32 x, s32 y, s32 commandIndex, const char *text);
extern void effDebugLineDraw2(sdfVector4_t *from, sdfVector4_t *to, u32 mode);
extern void effDebugPointDraw2(sdfVector4_t *point, u32 color);
extern s32 nbCameraCheckNiidaCamera(void);

const char nbCameraHelperMsgNotTarget[40] = "nbCameraHelper:Not Target Helper\n";
const char nbCameraHelperMsgNotEye[32] = "nbCameraHelper:Not Eye Helper\n";
const char nbCameraHelperBedSelectText[16] = "BED SELECT";
const char nbCameraHelperViewerPBDir[24] = "/efftool/camera/pb/";
const char nbCameraHelperPbSelectText[16] = "PB SELECT";
const char nbCameraHelperCameraHelperText[16] = "CAMERA HELPER";

#ifdef NON_MATCHING
/* NON_MATCHING: 48 of 83 words match; 332 bytes emitted against retail 336. */
void nbCameraHelperViewer_Draw(void *pData)
{
    char buf[0x70];
    u16 frame;
    s32 last;
    u16 i;
    sdfVector4_t *eye;
    sdfVector4_t *target;
    if (nbCameraHelperViewerCurrent == NULL) {
        return;
    }
    if (nbCameraHelperViewerShowFrame != 0) {
        sprintf(buf, nbCameraHelperViewerFrameFormat, nbCameraHelperViewerCurrent->frame,
                nbCameraHelperLength(nbCameraHelperViewerCurrent));
        effDebugStringDraw(0x12C, 0x3C, 0, buf);
    }
    frame = nbCameraHelperViewerCurrent->frame;
    eye = &nbCameraHelperViewerCurrent->eye;
    target = &nbCameraHelperViewerCurrent->target;
    SDF_VU0_COPYVECTOR(&nbCameraHelperViewerState[frame - 1], eye);
    SDF_VU0_COPYVECTOR(&nbCameraHelperViewerHandle[frame - 1], target);
    if (nbCameraHelperViewerShowLines != 0 && nbCameraCheckNiidaCamera() == 0) {
        last = frame - 1;
        for (i = 0; i < last; i++) {
            effDebugLineDraw2(&nbCameraHelperViewerState[i],
                              &nbCameraHelperViewerState[i + 1], 0x80804000);
            effDebugLineDraw2(&nbCameraHelperViewerHandle[i],
                              &nbCameraHelperViewerHandle[i + 1], 0x80004080);
        }
        effDebugPointDraw2(&nbCameraHelperViewerCurrent->eye, 0x80800000);
        effDebugPointDraw2(&nbCameraHelperViewerCurrent->target, 0x80000080);
    }
    nbCameraHelperRun(nbCameraHelperViewerCurrent);
}
#else
INCLUDE_ASM(const s32, "code_001C55F8", nbCameraHelperViewer_Draw);
#endif


void nbCameraHelperViewer_End(void) {
}

