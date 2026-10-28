#include "common.h"
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
    u8 unk00[0x30];
    u32 unk30;
    s32 unk34;
    effPartHandle_t *hPart;
} nbCameraHelper_tag;

extern void *sdfChipAllocDirect(s32 aSize);

extern effPartHandle_t *nbCameraHelperFileLoadPB(const char *pFilename);

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

extern f32 D_0054F3A0[];

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, u32 aId);

extern void *dds3EffectFileListCreate(const char *dir, s32 type);

extern void *dds3EffectFileListWindowCreate(void *a);

extern void dds3EffectFileListWindowSetPos(void *a, s32 b, s32 c);

extern void dds3EffectFileListWindowSetOt(void *hEffFLWin, s32 ot);

extern void dds3EffectFileListWindowSetPadWait(void *hEffFLWin, s32 wait);

extern void dds3EffectFileListWindowSetWindowColor(void *hEffFLWin, u32 frame_c, u32 bg_c);

extern const char D_005358A8[];

extern u32 D_0054F37C;

extern char D_00535850[];

extern char D_00535878[];

extern s32 printf(const char *format, ...);

extern s32 nbCameraBossCutFlag;

extern f32 D_0054F3A0[];

extern char *nbCameraHelperViewerMode;

extern nbCameraHelper_tag *D_0054F3DC;

extern sdfMemHandle_t *D_0054F3E0;

extern sdfMemHandle_t *D_0054F3E4;

extern void *nbCameraHelperViewerState;

extern void *nbCameraHelperViewerHandle;

extern u8 D_0054F370;

extern u32 D_0054F374;

extern u8 D_0054F378;

extern u32 D_0054F37C;

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

INCLUDE_ASM(const s32, "code_001C55F8", nbCameraHelperFileLoadPB);

/* Called above its definition, where ee-gcc assumed int. */
u16 nbCameraHelperLength(nbCameraHelper_tag *hCameraHelper);

nbCameraHelper_tag *nbCameraHelperCreate(const char *pFilename)
{
    nbCameraHelper_tag *pHelper = sdfChipAllocDirect(0x3C);

    pHelper->hPart = nbCameraHelperFileLoadPB(pFilename);
    pHelper->unk30 = 0;
    pHelper->unk34 = nbCameraHelperLength(pHelper);
    return pHelper;
}

void nbCameraHelperDelete(nbCameraHelper_tag *hCameraHelper)
{
    dds3EffectPartDelete(hCameraHelper->hPart);
    sdfChipFreeDirect(hCameraHelper);
}

u16 nbCameraHelperLength(nbCameraHelper_tag *hCameraHelper)
{
    return dds3EffectPartGetHandle(hCameraHelper->hPart)->first->frameLength;
}

INCLUDE_ASM(const s32, "code_001C55F8", nbCameraHelperRun);

void nbCameraHelperModelSetSub(mdlObj_t *hModel, u32 target, u32 eye)
{
    nbCamera_t *pCamera = &pNbCamera;
    f32 length;

    if (sdfModelGetMarkPos(hModel->inner, target) == 0) {
        printf(D_00535850);
    }
    SDF_VU0_STOREVECTOR(pCamera);
    if (sdfModelGetMarkPos(hModel->inner, eye) == 0) {
        printf(D_00535878);
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
    void *hFileList = dds3EffectFileListCreate(D_005358A8, 0x10);
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

INCLUDE_ASM(const s32, "code_001C55F8", nbCameraHelperViewer_PB_Draw);

void nbCameraHelperViewer_PB_End(void *pData)
{
    dds3EffectFileListWindowDelete(nbCameraHelperViewerPBWork);
    dds3EffectFileListDelete(nbCameraHelperViewerPBHandle);
}

s32 nbCameraHelperViewer_Init(s32 pData)
{
    s32 size;

    if (nbCameraHelperViewerMode != NULL) {
        if (D_0054F3DC != NULL) {
            nbCameraHelperDelete(D_0054F3DC);
            if (D_0054F3E0 != NULL) {
                sdfMemFreeDirect(D_0054F3E0);
            }
            if (D_0054F3E4 != NULL) {
                sdfMemFreeDirect(D_0054F3E4);
            }
        }
        D_0054F3DC = nbCameraHelperCreate(nbCameraHelperViewerMode);
        nbCameraHelperViewerMode = NULL;
        size = nbCameraHelperLength(D_0054F3DC) * 16;
        D_0054F3E0 = sdfMemAllocDirect(size);
        nbCameraHelperViewerState = sdfMemLock(D_0054F3E0);
        D_0054F3E4 = sdfMemAllocDirect(size);
        nbCameraHelperViewerHandle = sdfMemLock(D_0054F3E4);
    }
    return pData;
}

INCLUDE_RODATA(const s32, "code_001C55F8", D_00535850);

INCLUDE_RODATA(const s32, "code_001C55F8", D_00535878);

INCLUDE_RODATA(const s32, "code_001C55F8", D_00535898);

INCLUDE_RODATA(const s32, "code_001C55F8", D_005358A8);

INCLUDE_RODATA(const s32, "code_001C55F8", D_005358C0);

INCLUDE_RODATA(const s32, "code_001C55F8", D_005358D0);

INCLUDE_ASM(const s32, "code_001C55F8", nbCameraHelperViewer_Draw);

void nbCameraHelperViewer_End(void) {
}

