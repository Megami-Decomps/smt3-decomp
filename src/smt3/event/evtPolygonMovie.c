#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "effect.h"
#include "event.h"
#include "ps2asm.h"


void *dds3GetProcessData(dds3ProcessID_t *task);

/* SMT3 allocates and clears 0xD4 bytes; the process flag helpers use its head. */
typedef struct pmdCutInfo_t {
    s32 firstFrame;
    s32 lastFrame;
} pmdCutInfo_t;

typedef struct evtBlendVecColor_t {
    f32 vec[4];
    u32 color;
} evtBlendVecColor_t;

typedef struct pmdSceneFogTable_t {
    evtBlendVecColor_t fog;
    s32 reserve[3];
} pmdSceneFogTable_t;

typedef struct pmdFrameTable_t pmdFrameTable_t;

typedef struct pmdEffectTable_t {
    s32 nameIndex;
    s32 dataOffset;
    s32 kind;
    s32 reserve;
} pmdEffectTable_t;

typedef struct evtPolygonMovieWork_t {
    u32 movieFlags;
    void *file04;
    sdfMemHandle_t *mem08;
    void *pm1data;
    void *header1;
    u8 unk14[4];
    char (*nameTable1)[32];
    u8 unk1C[0x24];
    pmdEffectTable_t *effectTable;
    s32 effectCount;
    u8 unk48[4];
    void *file4C;
    sdfMemHandle_t *mem50;
    void *pm2data;
    u8 unk58[8];
    char (*nameTable)[32];
    u8 unk64[4];
    pmdCutInfo_t *cutInfo;
    pmdFrameTable_t *frameTable;
    s32 frameCount;
    u8 unk74[8];
    f32 (*slightTable)[12];
    u8 unk80[4];
    pmdSceneFogTable_t *sfogTable;
    u8 unk88[4];
    effBlurQuad_t *blur2Table;
    u8 unk90[4];
    effBlurTemplateBody_t *mblurTable;
    u8 unk98[4];
    effBlurScatterParams_t *dblurTable;
    u8 unkA0[4];
    effSolidRectParams_t *filterTable;
    u8 unkA8[4];
    effResourceRectParams_t *mfilterTable;
    u8 unkB0[4];
    effBlurScaleParams_t *rblurTable;
    u8 unkB8[4];
    s32 mesHandle;
    dds3ProcessID_t *processId;
    s32 eventNo;
    s32 cutNo;
    s32 nowFrame;
    dds3Basic_t **effectBasicTable;
} evtPolygonMovieWork_t;

typedef struct pmdFrameTable_t {
    u16 kind;
    u16 frame;
    u16 length;
    s16 nameIndex;
    union {
        s8 b[8];
        s16 h[4];
        s32 w[2];
        f32 f[2];
    } data;
} pmdFrameTable_t;

void dds3SetProcessData(dds3ProcessID_t *task, void *data);
extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, char *name);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void evtPlaySe(s32 group, s32 se);
extern void evtSetBokasi(s32 on);
extern void dds3SetQuake(s32 length, s32 amount);
extern void dds3StopQuake(void);
extern void fldSkySetFilter(s32 filter, s32 mode, s32 frames);
extern void evtSetSkyAlpha(s32 frames, s32 alpha);
void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *work);
extern dds3Basic_t *dds3GetActiveScene(void);
extern void sdfChipFreeDirect(void *block);
extern void itfMesMngRelease(s32 id);
extern void fileSyncAll(void);
extern void fileHandleFree(void *handle);
extern void sdfMemFreeDirect(void *handle);
extern void FrameRun(evtPolygonMovieWork_t *work);
extern s32 printf(const char *fmt, ...);
extern f32 D_0056CFF0[12];
extern s32 strcmp(const char *a, const char *b);
extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *resource, f32 *position, f32 *scale);
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(void *resource, f32 *position, f32 *scale);
extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 kind, void *resource);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 stat);
extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, s32 entry);
extern void evtEffectLoopControl(dds3Basic_t *basic, s32 mode);
extern void dds3EffectObjectSetMG12Pos(dds3Basic_t *basic, dds3Basic_t *p0, dds3Basic_t *p1, dds3Basic_t *p2, dds3Basic_t *p3);
dds3Basic_t *SearchBasicFromNameIndex_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 nameIndex);
s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *work, pmdFrameTable_t *f);
extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);
extern void dds3EffectObjectPresetMG12(dds3Basic_t *basic);
extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *owner);
extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *owner, s32 index);
extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *target);
extern dds3ObjectBaseControl_t *dds3GetBaseControlWorkData(dds3Basic_t *basic);
extern void evtPlayEventBgm(s32 eventNo, s32 bgm);
extern void evtFadeInEventBgm(s32 eventNo, s32 bgm);
extern void evtStopEventBgm(s32 eventNo, s32 bgm);
extern void evtTransEventBgm(s32 eventNo, s32 bgm);
extern void p2sdBgmVolDown(void);
extern void p2sdBgmVolUp(void);
extern void p2sdStopAllBgm(void);
extern s32 pacFileCheckEnd(void);
extern sdfMemHandle_t *fileGetMemHandle(void *request);
extern void *fileLoad(const char *name);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, void *data, s32 mode);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t processFunc,
                                         dds3ProcessFunc_t shutdownFunc, void *data);
void evtGetPolygonMovieWorkData1(evtPolygonMovieWork_t *work, void *pm1data, void *pm2data);
s32 evtPolygonMovieProcess1(dds3ProcessID_t *task);
void evtPolygonMovieShutdown(dds3ProcessID_t *task);
void SetPMFileName(s32 eventNo, s32 cutNo, char *pm1file, char *pm2file);
evtPolygonMovieWork_t *InitPolygonMovieWorkData(void);
extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern void dds3SetLightColor(s32 index, s32 kind, f32 *color);
extern void dds3SetLightDir(s32 index, s32 kind, f32 *dir);
extern void dds3SetAmbientColor(s32 kind, f32 *color);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, s32 mask);
extern void *evtGetUnitHandle2(dds3Basic_t *unit);
extern void evtLinkLightObjectBasic(void *handle, dds3Basic_t *light, s32 frames);
extern void evtUnlinkLightObjectBasic(void *handle, s32 frames);
extern s32 sprintf(char *buffer, const char *format, ...);
void evtHokanSceneLight(s32 enable, f32 t, f32 *a, f32 *b, f32 *out);
f32 evtHokanFloat(s32 enable, f32 t, f32 a, f32 b);
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern u8 dds3ScreenBlurFlag;
extern u32 dds3LightParam[];
extern pmdSceneFogTable_t D_0056D020;
extern effBlurQuad_t D_0056D040;
extern effBlurTemplateBody_t D_0056D070;
extern effBlurScatterParams_t D_0056D0A0;
extern effBlurScaleParams_t D_0056D0D0;
extern effResourceRectParams_t D_0056D100;
extern effSolidRectParams_t D_0056D130;
extern void dds3SetFogPara(s32 mode, f32 min, f32 max, f32 near, f32 far);
extern void Eve_BlurSetParam(effBlurQuad_t *param);
extern void Eve_BlurON(void);
extern void Eve_BlurOFF(void);
extern void Eve_PartialMultiBlurSetParam(effBlurTemplateBody_t *param);
extern void Eve_PartialMultiBlurON(void);
extern void Eve_PartialMultiBlurOFF(void);
extern void Eve_DistortionBlurSetParam(effBlurScatterParams_t *param);
extern void Eve_DistortionBlurON(void);
extern void Eve_DistortionBlurOFF(void);
extern void Eve_RippleBlurSetParam(effBlurScaleParams_t *param);
extern void Eve_RippleBlurON(void);
extern void Eve_RippleBlurOFF(void);
extern void Eve_PartialMultiFilterSetParam(effResourceRectParams_t *param);
extern void Eve_PartialMultiFilterON(void);
extern void Eve_PartialMultiFilterOFF(void);
extern void Eve_FilterSetParam(effSolidRectParams_t *param);
extern void Eve_FilterON(void);
extern void Eve_FilterOFF(void);
void evtHokanSceneFog(s32 enable, f32 t, evtBlendVecColor_t *a, evtBlendVecColor_t *b, evtBlendVecColor_t *out);
void evtHokanBlur2(s32 enable, f32 t, effBlurQuad_t *a, effBlurQuad_t *b, effBlurQuad_t *out);
void evtHokanMultBlur(s32 enable, f32 t, effBlurTemplateBody_t *a, effBlurTemplateBody_t *b, effBlurTemplateBody_t *out);
void evtHokanDistBlur(s32 enable, f32 t, effBlurScatterParams_t *a, effBlurScatterParams_t *b, effBlurScatterParams_t *out);
void evtHokanRipBlur(s32 enable, f32 t, effBlurScaleParams_t *a, effBlurScaleParams_t *b, effBlurScaleParams_t *out);
void evtHokanMultFilter(s32 enable, f32 t, effResourceRectParams_t *a, effResourceRectParams_t *b, effResourceRectParams_t *out);
void evtHokanFilter(s32 enable, f32 t, effSolidRectParams_t *a, effSolidRectParams_t *b, effSolidRectParams_t *out);
s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *work, pmdFrameTable_t *f, char *effName, char *targetName, s32 posIndex, s32 cutNo);
f32 GetKeyFrameRatio(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);
void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 no);
void evtRunUnitMode(dds3Basic_t *unit, s32 mode);
typedef void (*evtFrameFunc_t)(evtPolygonMovieWork_t *, pmdFrameTable_t *, pmdFrameTable_t *, s32);
extern evtFrameFunc_t nowcamera[];
extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);
extern void dds3SetScreenFovy(f32 fovy);
extern s32 evtCameraFrameState;
extern f32 nowfovy;
extern s32 D_0054F8E0;

extern void *sdfChipAllocDirect(u32 size);
extern void *memset(void *destination, s32 value, u32 size);

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 enable, f32 t, u32 a, u32 b);

extern f32 evtHokanFogParam[4];

extern u32 D_0032C830[4];


extern effBlurQuad_t D_0032C840;


extern effBlurTemplateBody_t D_0032C870;


extern effSolidRectParams_t D_0032C8A0;


extern effResourceRectParams_t D_0032C8C0;


extern effBlurScatterParams_t D_0032C8F0;


extern effBlurScaleParams_t D_0032C920;

typedef struct polyMovieClip_t {
    u8 opaque_00[8];
    f32 duration;
    f32 position;
} polyMovieClip_t;

typedef struct polyMovieState_t {
    u8 opaque_00[4];
    polyMovieClip_t *clip;
} polyMovieState_t;

typedef struct polyMovieObject_t {
    u8 opaque_00[0x18];
    polyMovieState_t *state;
} polyMovieObject_t;

s32 evtStopMover(polyMovieObject_t *movie, s32 undo, s32 start, s32 end);

typedef struct evtScaledValue_t {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} evtScaledValue_t;

void dds3PathBasicControlSetRatio(evtScaledValue_t *value, f32 multiplier);

void dds3PathBasicPauseOn(evtScaledValue_t *value);

void dds3PathBasicPauseOff(evtScaledValue_t *value);

extern f32 D_0032C7F0[4];

extern f32 D_0032C800[4];

extern f32 D_0032C810[4];

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

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

sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out);

extern void *memcpy(void *dst, const void *src, u32 size);

static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, unit);
}

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

/* One asm location for every arm's w set, so jump2 can cross-jump them. */
static inline void SetBlendVecW(f32 w)
{
    SDF_VU0_SETVECTOR_W(w);
}

void UnitFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;

    if (f->frame == now) {
        if (f->nameIndex >= 0) {
            basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 5, work->nameTable[f->nameIndex]);
            if (basic != NULL) {
                evtRunUnitMode(basic, f->data.b[0]);
            }
        }
    }
}

void CameraFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;

    if (f->nameIndex >= 0) {
        basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 4, work->nameTable[f->nameIndex]);
        if (basic != NULL) {
            if (evtCameraFrameState == 0) {
                D_0054F8E0 = -1;
            }
            if (D_0054F8E0 < f->frame) {
                D_0054F8E0 = f->frame;
                evtCameraFrameState = (s32)basic;
                nowfovy = f->data.f[0];
            }
        }
    }
}

s32 QuakeFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->frame == now) {
        if (f->data.h[0] == 0) {
            dds3StopQuake();
        } else {
            dds3SetQuake(f->length, f->data.h[0]);
        }
    }
}

s32 FadeFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->frame == now) {
        switch (f->data.h[0]) {
        case 0:
            dds3FadeIn(0xFF, 0xFF, 0xFF, f->length);
            break;
        case 1:
            dds3FadeOut(0xFF, 0xFF, 0xFF, f->length);
            break;
        case 2:
            dds3FadeIn(0, 0, 0, f->length);
            break;
        case 3:
            dds3FadeOut(0, 0, 0, f->length);
            break;
        }
    }
}

void SeFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->frame == now) {
        evtPlaySe(work->eventNo, f->data.h[0]);
    }
}

s32 BgmFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 eventNo;
    s32 bgm;

    if (f->frame == now) {
        eventNo = work->eventNo;
        bgm = f->data.h[1];
        printf("bgm_frame e%03d, bgm %d
", eventNo, bgm);
        switch (f->data.h[0]) {
        case 0:
            evtPlayEventBgm(eventNo, bgm);
            break;
        case 1:
            evtFadeInEventBgm(eventNo, bgm);
            break;
        case 2:
            evtStopEventBgm(eventNo, bgm);
            break;
        case 3:
            evtTransEventBgm(eventNo, bgm);
            break;
        case 4:
            p2sdBgmVolDown();
            break;
        case 5:
            p2sdBgmVolUp();
            break;
        case 6:
            p2sdStopAllBgm();
            break;
        }
    }
}

void BlurFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 alpha;
    s32 scale;
    s32 elapsed;
    s32 span;

    if (f->data.h[2] != 0) {
        if (nf != NULL) {
            elapsed = now - f->frame;
            span = nf->frame - f->frame;
            alpha = (s16)((f->data.h[0] * (span - elapsed) + nf->data.h[0] * elapsed) / span);
            scale = (s16)((f->data.h[1] * (span - elapsed) + nf->data.h[1] * elapsed) / span);
        } else {
            alpha = f->data.h[0];
            scale = f->data.h[1];
        }
    } else {
        alpha = f->data.h[0];
        scale = f->data.h[1];
    }
    if (alpha == 0 && scale == 0) {
        dds3ScreenBlurFlag = 0;
    } else {
        dds3ScreenBlurAlpha = alpha;
        dds3ScreenBlurScale = scale;
        dds3ScreenBlurFlag = 1;
    }
}

dds3Basic_t *AddEffect(evtPolygonMovieWork_t *work, pmdFrameTable_t *f)
{
    f32 position[4];
    f32 scale[4];
    s32 i;
    s32 kind;
    dds3Basic_t *basic;

    memset(position, 0, sizeof(position));
    memset(scale, 0, sizeof(scale));
    kind = 0;
    basic = NULL;
    scale[3] = 1.0f;
    for (i = 0; i < work->effectCount; i++) {
        if (strcmp(work->nameTable1[work->effectTable[i].nameIndex], work->nameTable[f->nameIndex]) == 0) {
            kind = work->effectTable[i].kind;
            switch (kind) {
            case 0:
                basic = dds3AddD3PEffectObjectBasic2((u8 *)work->header1 + work->effectTable[i].dataOffset, position, scale);
                break;
            case 2:
                basic = dds3AddMG12EffectObjectBasic2(1, (u8 *)work->header1 + work->effectTable[i].dataOffset);
                break;
            case 3:
                basic = dds3AddMG12EffectObjectBasic2(2, (u8 *)work->header1 + work->effectTable[i].dataOffset);
                break;
            case 1:
                basic = dds3AddBEDEffectObjectBasic2((u8 *)work->header1 + work->effectTable[i].dataOffset, position, scale);
                break;
            }
            break;
        }
    }
    if (basic != NULL) {
        dds3EffectObjectBasicStatOn(basic, 1);
        if (kind == 1) {
            dds3StartBEDEffectObjectBasic(basic, f->data.b[4]);
        }
    }
    return basic;
}

s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *work, pmdFrameTable_t *f, char *effName, char *targetName, s32 posIndex, s32 cutNo)
{
    s32 i;
    dds3Basic_t *basic;
    dds3Basic_t *target;
    dds3ObjectBaseControl_t *control;

    for (i = 0; i < 0x7F; i++) {
        if (work->effectBasicTable[i] == NULL) {
            break;
        }
    }
    if (i == 0x7F) {
        return -1;
    }
    basic = AddEffect(work, f);
    if (basic == NULL) {
        return -1;
    }
    basic->name = effName;
    work->effectBasicTable[i] = basic;
    if (f->data.b[4] != 0) {
        dds3EffectObjectPresetMG12(basic);
    }
    if (targetName == NULL) {
        return i;
    }
    target = dds3SceneBasicSearchName(dds3GetActiveScene(), targetName);
    if (target == NULL) {
        return i;
    }
    switch (target->type >> 24) {
    case BASIC_TYPE_UNITOBJECT:
        if (posIndex >= 0) {
            dds3EffectObjectBasicLinkObjectIndex(basic, target, posIndex);
            break;
        }
        /* fallthrough */
    case BASIC_TYPE_CAMERAOBJECT:
    case BASIC_TYPE_MATTEROBJECT:
    case BASIC_TYPE_EFFECTOBJECT:
    case BASIC_TYPE_EVENTOBJECT:
    case BASIC_TYPE_LIGHTOBJECT:
    case BASIC_TYPE_POINT:
        dds3EffectObjectBasicLinkObject(basic, target);
        break;
    case BASIC_TYPE_PATH:
        evtPathLinkAndMake(target, basic);
        control = dds3GetBaseControlWorkData(basic);
        evtStopMover((polyMovieObject_t *)control->control[1], 1, 0, 0);
        break;
    }
    return i;
}

s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *work, pmdFrameTable_t *f)
{
    dds3Basic_t *parts[4];
    s32 i;
    s32 j;
    dds3Basic_t *basic;
    s8 *name;
    dds3Basic_t **out;

    for (i = 0; i < 0x7F; i++) {
        if (work->effectBasicTable[i] == NULL) {
            break;
        }
    }
    if (i == 0x7F) {
        return -1;
    }
    basic = AddEffect(work, f);
    if (basic == NULL) {
        return -1;
    }
    basic->name = work->nameTable[f->nameIndex];
    work->effectBasicTable[i] = basic;
    name = &f->data.b[4];
    out = parts;
    for (j = 0; j < 4; j++) {
        *out = NULL;
        if (*name >= 0) {
            *out = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 7, work->nameTable[*name]);
        }
        name++;
        out++;
    }
    dds3EffectObjectSetMG12Pos(basic, parts[0], parts[1], parts[2], parts[3]);
    if (f->data.b[1] == 0) {
        return i;
    }
    dds3EffectObjectPresetMG12(basic);
    return i;
}

void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 no)
{
    dds3Basic_t *basic;

    if (no >= 0) {
        basic = work->effectBasicTable[no];
        if (basic != NULL) {
            dds3DeleteSceneBasicData(basic);
            work->effectBasicTable[no] = NULL;
        }
    }
}

void EffectFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    char *target = NULL;
    dds3Basic_t *basic;

    if (f->frame == now) {
        if (f->data.h[3] >= 0) {
            target = work->nameTable[f->data.h[3]];
        }
        f->data.b[0] = AddEffectTable(now, work, f, work->nameTable[f->nameIndex], target, f->data.b[5], 0);
    } else if (f->data.h[1] > 0) {
        if (now == f->data.h[1]) {
            DelEffectTable_evtPolygonMovie(work, f->data.b[0]);
            f->data.b[0] = -1;
        }
    }
    if (f->data.b[0] >= 0) {
        basic = work->effectBasicTable[f->data.b[0]];
        if (basic != NULL) {
            if (f->length == now) {
                if (f->length != 0) {
                    evtEffectLoopControl(basic, 0);
                }
            }
        }
    }
}

dds3Basic_t *SearchBasicFromNameIndex_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 nameIndex)
{
    if (nameIndex < 0) {
        return NULL;
    }
    return dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 7, work->nameTable[nameIndex]);
}

void MG12FrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;
    dds3Basic_t *p0;
    dds3Basic_t *p1;
    dds3Basic_t *p2;

    if (f->frame == now) {
        f->data.b[0] = AddEffectTableMG12(now, work, f);
    } else if (f->data.h[1] > 0) {
        if (now == f->data.h[1]) {
            DelEffectTable_evtPolygonMovie(work, f->data.b[0]);
            f->data.b[0] = -1;
        }
    }
    if (f->data.b[0] >= 0) {
        basic = work->effectBasicTable[f->data.b[0]];
        if (basic != NULL) {
            if (f->length == now) {
                if (f->length != 0) {
                    evtEffectLoopControl(basic, 0);
                }
            }
        }
        if (f->kind == 0x15) {
            if (basic != NULL) {
                p0 = SearchBasicFromNameIndex_evtPolygonMovie(work, f->data.b[4]);
                p1 = SearchBasicFromNameIndex_evtPolygonMovie(work, f->data.b[5]);
                p2 = SearchBasicFromNameIndex_evtPolygonMovie(work, f->data.b[6]);
                dds3EffectObjectSetMG12Pos(basic, p0, p1, p2,
                                           SearchBasicFromNameIndex_evtPolygonMovie(work, f->data.b[7]));
            }
        }
    }
}

void BEDFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    char *target = NULL;

    if (f->frame == now) {
        if (f->data.h[1] >= 0) {
            target = work->nameTable[f->data.h[1]];
        }
        f->data.h[3] = AddEffectTable(now, work, f, work->nameTable[f->nameIndex], target, -1, f->data.b[4]);
    } else if (f->data.h[0] > 0) {
        if (now == f->data.h[0]) {
            DelEffectTable_evtPolygonMovie(work, f->data.h[3]);
            f->data.h[3] = -1;
        }
    }
}

s32 LightFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *light;
    dds3Basic_t *unit;
    void *handle;

    if (f->frame == now) {
        if (f->data.h[0] >= 0) {
            light = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 9, work->nameTable[f->nameIndex]);
            if (light != NULL) {
                unit = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 5, work->nameTable[f->data.h[0]]);
                if (unit != NULL) {
                    handle = evtGetUnitHandle2(unit);
                    if (f->data.h[1] != 0) {
                        evtLinkLightObjectBasic(handle, light, f->length);
                    } else {
                        evtUnlinkLightObjectBasic(handle, f->length);
                    }
                }
            }
        }
    }
}

void SkyFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->frame == now) {
        fldSkySetFilter(f->data.w[1], f->data.h[0], f->length);
        evtSetSkyAlpha(f->length, f->data.h[1]);
    }
}

s32 BokasiFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->frame == now) {
        if (f->data.h[0] != 0) {
            evtSetBokasi(1);
        } else {
            evtSetBokasi(0);
        }
    }
}

f32 GetKeyFrameRatio(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 start;
    s32 end;

    if (now < f->frame) {
        start = work->cutInfo->firstFrame;
        end = f->frame;
    } else {
        start = f->frame;
        if (nf != NULL) {
            end = nf->frame;
        } else {
            end = work->cutInfo->lastFrame;
        }
    }
    if (end != start) {
        return (f32)(now - start) / (f32)(end - start);
    }
    return 0.0f;
}

void SLightFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    f32 *key1;
    f32 *key2;
    f32 soften1;
    f32 bias1;
    f32 soften2;
    f32 bias2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->nameIndex;
        soften1 = f->data.f[0];
        bias1 = f->data.f[1];
        key1 = work->slightTable[f->length];
        if (nf != NULL) {
            bias2 = nf->data.f[1];
            soften2 = nf->data.f[0];
            key2 = work->slightTable[nf->length];
        } else {
            soften2 = 7.0f;
            bias2 = 0.0f;
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        bias2 = f->data.f[1];
        soften1 = 7.0f;
        bias1 = 0.0f;
        soften2 = f->data.f[0];
        key2 = work->slightTable[f->length];
    }
    evtHokanSceneLight(hokan, ratio, key1, key2, D_0056CFF0);
    dds3SetLightColor(0, 0, D_0056CFF0);
    dds3SetLightDir(0, 0, &D_0056CFF0[4]);
    dds3SetAmbientColor(0, &D_0056CFF0[8]);
    sdfFaceDivSoften = evtHokanFloat(hokan, ratio, soften1, soften2);
    sdfFaceDivBias = evtHokanFloat(hokan, ratio, bias1, bias2);
}

void SFogFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    pmdSceneFogTable_t *key1;
    pmdSceneFogTable_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->sfogTable[f->length];
        if (nf != NULL) {
            key2 = &work->sfogTable[nf->length];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->sfogTable[f->length];
    }
    evtHokanSceneFog(hokan, ratio, &key1->fog, &key2->fog, &D_0056D020.fog);
    dds3SetFogPara(0, D_0056D020.fog.vec[1], D_0056D020.fog.vec[0], D_0056D020.fog.vec[2], D_0056D020.fog.vec[3]);
    dds3LightParam[12] = D_0056D020.fog.color;
}

void Blur2FrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effBlurQuad_t *key1;
    effBlurQuad_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->blur2Table[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->blur2Table[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->blur2Table[f->data.h[1]];
    }
    evtHokanBlur2(hokan, ratio, key1, key2, &D_0056D040);
    Eve_BlurSetParam(&D_0056D040);
    if (((u8 *)&D_0056D040.color)[3] < 3) {
        Eve_BlurOFF();
    } else {
        Eve_BlurON();
    }
}

void MBlurFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effBlurTemplateBody_t *key1;
    effBlurTemplateBody_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->mblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->mblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->mblurTable[f->data.h[1]];
    }
    evtHokanMultBlur(hokan, ratio, key1, key2, &D_0056D070);
    Eve_PartialMultiBlurSetParam(&D_0056D070);
    if (((u8 *)&D_0056D070.source.color)[3] < 3) {
        Eve_PartialMultiBlurOFF();
    } else {
        Eve_PartialMultiBlurON();
    }
}

void DBlurFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effBlurScatterParams_t *key1;
    effBlurScatterParams_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->dblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->dblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->dblurTable[f->data.h[1]];
    }
    evtHokanDistBlur(hokan, ratio, key1, key2, &D_0056D0A0);
    Eve_DistortionBlurSetParam(&D_0056D0A0);
    if (((u8 *)&D_0056D0A0.color)[3] < 3) {
        Eve_DistortionBlurOFF();
    } else {
        Eve_DistortionBlurON();
    }
}

void RBlurFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effBlurScaleParams_t *key1;
    effBlurScaleParams_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->rblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->rblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->rblurTable[f->data.h[1]];
    }
    evtHokanRipBlur(hokan, ratio, key1, key2, &D_0056D0D0);
    Eve_RippleBlurSetParam(&D_0056D0D0);
    if (((u8 *)&D_0056D0D0.color)[3] < 3) {
        Eve_RippleBlurOFF();
    } else {
        Eve_RippleBlurON();
    }
}

void MFilterFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effResourceRectParams_t *key1;
    effResourceRectParams_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->mfilterTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->mfilterTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->mfilterTable[f->data.h[1]];
    }
    evtHokanMultFilter(hokan, ratio, key1, key2, &D_0056D100);
    Eve_PartialMultiFilterSetParam(&D_0056D100);
    if (((u8 *)&D_0056D100.draw.rgba)[3] < 3) {
        Eve_PartialMultiFilterOFF();
    } else {
        Eve_PartialMultiFilterON();
    }
}

void FilterFrameFunc(evtPolygonMovieWork_t *work, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    effSolidRectParams_t *key1;
    effSolidRectParams_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(work, f, nf, now);
    hokan = 1;
    if (now >= f->frame) {
        hokan = f->data.h[0];
        key1 = &work->filterTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &work->filterTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &work->filterTable[f->data.h[1]];
    }
    evtHokanFilter(hokan, ratio, key1, key2, &D_0056D130);
    Eve_FilterSetParam(&D_0056D130);
    if (((u8 *)&D_0056D130.color)[3] < 2) {
        Eve_FilterOFF();
    } else {
        Eve_FilterON();
    }
}

void FrameRun(evtPolygonMovieWork_t *work)
{
    s32 i;
    pmdFrameTable_t *f;
    pmdFrameTable_t *nf;
    pmdFrameTable_t *prev;
    s32 length;
    s32 now;
    evtFrameFunc_t func;

    evtCameraFrameState = 0;
    prev = NULL;
    for (i = 0; i < work->frameCount; i++) {
        f = &work->frameTable[i];
        nf = NULL;
        if (i + 1 < work->frameCount) {
            if (work->frameTable[i + 1].kind == f->kind) {
                nf = &work->frameTable[i + 1];
            }
        }
        if (prev != NULL) {
            if (prev->kind != f->kind) {
                prev = NULL;
            }
        }
        switch (f->kind) {
        case 3:
        case 20:
        case 21:
            if (f->data.h[1] == 0) {
                length = 0;
            } else {
                length = f->data.h[1] - f->frame + 1;
            }
            break;
        case 18:
            if (f->data.h[0] == 0) {
                length = 0;
            } else {
                length = f->data.h[0] - f->frame + 1;
            }
            break;
        case 9:
        case 12:
        case 22:
            length = 0;
            break;
        case 10:
        case 11:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 23:
            length = -1;
            break;
        default:
            length = f->length;
            break;
        }
        if (length > 0) {
            now = work->nowFrame;
            if (now < f->frame) {
                goto next;
            }
            if (now < f->frame + length) {
                goto run;
            }
            goto next;
        } else if (length == 0) {
            now = work->nowFrame;
            if (now < f->frame) {
                goto next;
            }
        } else if ((now = work->nowFrame) < f->frame) {
            if (prev != NULL) {
                goto next;
            }
        } else if (nf != NULL) {
            if (now >= nf->frame) {
                goto next;
            }
        }
    run:
        func = nowcamera[f->kind];
        if (func != NULL) {
            func(work, f, nf, now);
        }
    next:
        prev = f;
    }
    if (evtCameraFrameState != 0) {
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), (dds3Basic_t *)evtCameraFrameState);
        dds3SetScreenFovy(nowfovy);
    }
}

s32 evtPolygonMovieProcess2(dds3ProcessID_t *task)
{
    evtPolygonMovieWork_t *work;
    s32 ret;

    work = (evtPolygonMovieWork_t *)dds3GetProcessData(task);
    FrameRun(work);
    work->nowFrame++;
    ret = 0;
    if (work->nowFrame >= work->cutInfo->lastFrame) {
        printf("end polygon movie! frame:%d
", work->nowFrame);
        return -1;
    }
    return ret;
}

/* NON_MATCHING: 32 of 68 words differ (first at +0x24). */
#ifdef NON_MATCHING
void CheckLoad_evtPolygonMovie(evtPolygonMovieWork_t *work)
{
    if ((work->movieFlags & 8) == 0) {
        if (work->movieFlags & 2) {
            if (work->file04 != NULL) {
                if (pacFileCheckEnd() != 0) {
                    work->mem08 = fileGetMemHandle(work->file04);
                    work->pm1data = (void *)sdfMemLock(work->mem08);
                    fileHandleFree(work->file04);
                    work->file04 = NULL;
                    work->movieFlags &= ~2;
                }
            }
        } else if (work->movieFlags & 4) {
            if (work->file4C != NULL) {
                if (pacFileCheckEnd() != 0) {
                    work->mem50 = fileGetMemHandle(work->file4C);
                    work->pm2data = (void *)sdfMemLock(work->mem50);
                    fileHandleFree(work->file4C);
                    work->file4C = NULL;
                    work->movieFlags &= ~4;
                }
            }
        } else if (work->pm1data != NULL) {
            if (work->pm2data != NULL) {
                evtGetPolygonMovieWorkData1(work, work->pm1data, work->pm2data);
                work->movieFlags |= 8;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", CheckLoad_evtPolygonMovie);
#endif

s32 evtCheckPolygonMovieRun(dds3ProcessID_t *task)
{
    evtPolygonMovieWork_t *state;
    s32 set;

    state = (evtPolygonMovieWork_t *)dds3GetProcessData(task);
    set = 0;
    if (state->movieFlags & 8) {
        set = 1;
    }
    return set;
}

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BAF8);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BB20);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtPolygonMovieProcess1);

void evtPolygonMovieShutdown(dds3ProcessID_t *task)
{
    evtFreePolygonMovieWorkData(dds3GetProcessData(task));
    dds3SetProcessData(task, NULL);
}

f32 evtHokanFloat(s32 enable, f32 t, f32 a, f32 b)
{
    if (enable) {
        return a * (1.0f - t) + b * t;
    }
    return a;
}

s32 evtHokanInt(s32 enable, f32 t, s32 a, s32 b)
{
    s32 result = a;

    if (enable) {
        result = (s32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return result;
}

u32 evtHokanUInt(s32 enable, f32 t, u32 a, u32 b)
{
    if (enable) {
        return (u32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return a;
}

/* vu0 routine: blend two RGBA8888 colours by t */
/* NON_MATCHING: 9 of 49 words differ (first at +0x4). */
#ifdef NON_MATCHING
u32 evtHokanColor(s32 enable, f32 t, u32 a, u32 b)
{
    u32 color = a;

    if (enable) {
        eeUnpackRGBAWordFloat(a, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - t);
        SDF_VU0_COPYVECTOR_A_TO_B();
        eeUnpackRGBAWordFloat(b, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(t);
        SDF_VU0_ADDVECTOR();
        color = eePackRGBAWord(128.0f);
    }
    return color;
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanColor);
#endif

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtHokanSceneLight(s32 enable, f32 t, f32 *a, f32 *b, f32 *out)
{
    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(out);
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a + 4);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b + 4);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(out + 4);
    if (a != NULL) {
        SDF_VU0_LOADVECTOR(a + 8);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - t);
    if (b != NULL) {
        SDF_VU0_LOADVECTOR_B(b + 8);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_B_V0(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(out + 8);
}

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 enable, f32 t, evtBlendVecColor_t *a, evtBlendVecColor_t *b, evtBlendVecColor_t *out)
{

    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        SDF_VU0_SETVECTOR_X(a->vec[0]);
        SDF_VU0_SETVECTOR_Y(a->vec[1]);
        SDF_VU0_SETVECTOR_Z(a->vec[2]);
        SetBlendVecW(a->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - t);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (b != NULL) {
        SDF_VU0_SETVECTOR_X(b->vec[0]);
        SDF_VU0_SETVECTOR_Y(b->vec[1]);
        SDF_VU0_SETVECTOR_Z(b->vec[2]);
        SetBlendVecW(b->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(out->vec[0]);
    SDF_VU0_GETVECTOR_Y(out->vec[1]);
    SDF_VU0_GETVECTOR_Z(out->vec[2]);
    SDF_VU0_GETVECTOR_W(out->vec[3]);
    if (a != NULL) {
        UnpackRGBAWordFloat(a->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - t);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (b != NULL) {
        UnpackRGBAWordFloat(b->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(t);
    SDF_VU0_ADDVECTOR();
    out->color = PackRGBAWord(128.0f);
}

void evtHokanBlur2(s32 enable, f32 t, effBlurQuad_t *a, effBlurQuad_t *b, effBlurQuad_t *out)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C840;
    }
    if (b == NULL) {
        b = &D_0032C840;
    }
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->angle = evtHokanFloat(enable, t, a->angle, b->angle);
    out->displacement = evtHokanFloat(enable, t, a->displacement, b->displacement);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        out->position[positionOrCornerIndex] = evtHokanInt(enable, t, a->position[positionOrCornerIndex], b->position[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->corners[positionOrCornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->corners[positionOrCornerIndex][coordinateIndex], b->corners[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanMultBlur(s32 enable, f32 t, effBlurTemplateBody_t *a, effBlurTemplateBody_t *b, effBlurTemplateBody_t *out)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C870;
    }
    if (b == NULL) {
        b = &D_0032C870;
    }
    out->extent = evtHokanUInt(enable, t, a->extent, b->extent);
    out->source.color = evtHokanColor(enable, t, a->source.color, b->source.color);
    out->source.blendControl = a->source.blendControl;
    out->source.angle = evtHokanFloat(enable, t, a->source.angle, b->source.angle);
    out->source.displacement = evtHokanFloat(enable, t, a->source.displacement, b->source.displacement);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        out->source.position[positionOrCornerIndex] = evtHokanInt(enable, t, a->source.position[positionOrCornerIndex], b->source.position[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->source.corners[positionOrCornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->source.corners[positionOrCornerIndex][coordinateIndex], b->source.corners[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanFilter(s32 enable, f32 t, effSolidRectParams_t *a, effSolidRectParams_t *b, effSolidRectParams_t *out);

/* NON_MATCHING: 1 of 72 words differ (first at +0xC0). */
#ifdef NON_MATCHING
void evtHokanFilter(s32 enable, f32 t, effSolidRectParams_t *a, effSolidRectParams_t *b, effSolidRectParams_t *out)
{
    s32 cornerIndex;
    s32 coordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C8A0;
    }
    if (b == NULL) {
        b = &D_0032C8A0;
    }
    for (cornerIndex = 0; cornerIndex < 2; cornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            out->corners[cornerIndex][coordinateIndex] = evtHokanInt(enable, t, a->corners[cornerIndex][coordinateIndex], b->corners[cornerIndex][coordinateIndex]);
        }
    }
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtHokanFilter);
#endif

void evtHokanMultFilter(s32 enable, f32 t, effResourceRectParams_t *a, effResourceRectParams_t *b, effResourceRectParams_t *out)
{
    s32 centerCoordinateIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C8C0;
    }
    if (b == NULL) {
        b = &D_0032C8C0;
    }
    out->extent = evtHokanUInt(enable, t, a->extent, b->extent);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        out->center[centerCoordinateIndex] = evtHokanInt(
            enable, t, a->center[centerCoordinateIndex], b->center[centerCoordinateIndex]);
    }
    out->draw.rgba = evtHokanColor(enable, t, a->draw.rgba, b->draw.rgba);
    out->draw.blendControl = a->draw.blendControl;
}

void evtHokanDistBlur(s32 enable, f32 t, effBlurScatterParams_t *a, effBlurScatterParams_t *b, effBlurScatterParams_t *out)
{
    s32 positionAxisIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C8F0;
    }
    if (b == NULL) {
        b = &D_0032C8F0;
    }
    out->count = evtHokanUInt(enable, t, a->count, b->count);
    out->delaySpread = evtHokanUInt(enable, t, a->delaySpread, b->delaySpread);
    out->angleStep = evtHokanFloat(enable, t, a->angleStep, b->angleStep);
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtHokanFloat(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtHokanFloat(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->positionSpread = evtHokanUInt(enable, t, a->positionSpread, b->positionSpread);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        out->position[positionAxisIndex] = evtHokanInt(enable, t, a->position[positionAxisIndex], b->position[positionAxisIndex]);
    }
    out->size = evtHokanUInt(enable, t, a->size, b->size);
}

void evtHokanRipBlur(s32 enable, f32 t, effBlurScaleParams_t *a, effBlurScaleParams_t *b, effBlurScaleParams_t *out)
{
    s32 positionAxisIndex;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_0032C920;
    }
    if (b == NULL) {
        b = &D_0032C920;
    }
    out->count = evtHokanUInt(enable, t, a->count, b->count);
    out->phaseStep = evtHokanFloat(enable, t, a->phaseStep, b->phaseStep);
    out->spacing = evtHokanFloat(enable, t, a->spacing, b->spacing);
    out->color = evtHokanColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtHokanFloat(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtHokanFloat(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->angleStep = evtHokanFloat(enable, t, a->angleStep, b->angleStep);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        out->position[positionAxisIndex] = evtHokanInt(enable, t, a->position[positionAxisIndex], b->position[positionAxisIndex]);
    }
    out->size = evtHokanUInt(enable, t, a->size, b->size);
}

void evtRunUnitMode(dds3Basic_t *unit, s32 mode)
{
    switch (mode) {
    case 0:
        *(u32 *)dds3GetUnitHandleObjectBasic(unit)->owner &= ~1;
        break;
    case 1:
        *(u32 *)dds3GetUnitHandleObjectBasic(unit)->owner |= 1;
        break;
    case 2:
        dds3ObjectBaseOffFlag(unit, 0x400);
        dds3ObjectBaseOffFlag(unit, 0x200);
        break;
    case 3:
        dds3ObjectBaseOnFlag(unit, 0x400);
        dds3ObjectBaseOffFlag(unit, 0x200);
        break;
    case 4:
        dds3ObjectBaseOffFlag(unit, 0x400);
        dds3ObjectBaseOnFlag(unit, 0x200);
        mode = 0;
        break;
    }
}

void evtSetPolygonMovieFlag(dds3ProcessID_t *process, s32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    work->movieFlags |= mask;
}

void evtResetPolygonMovieFlag(dds3ProcessID_t *process, s32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    work->movieFlags &= ~mask;
}

u8 evtCheckPolygonMovieFlag(dds3ProcessID_t *process, u32 mask)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(process);
    return (work->movieFlags & mask) != 0;
}

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtStopMover(polyMovieObject_t *movie, s32 undo, s32 startFrame, s32 endFrame)
{
    polyMovieClip_t *clip;
    s32 elapsedFrames;
    s32 duration;

    elapsedFrames = endFrame - startFrame;
    if (elapsedFrames < 0) {
        elapsedFrames = 0;
    }
    clip = movie->state->clip;
    if (clip != NULL) {
        duration = (s32)clip->duration;
        if (duration <= elapsedFrames) {
            elapsedFrames = duration;
        }
        dds3PathBasicControlSetRatio((evtScaledValue_t *)clip, (f32)elapsedFrames / (f32)duration);
        if (undo == 0) {
            dds3PathBasicPauseOn((evtScaledValue_t *)clip);
        } else {
            dds3PathBasicPauseOff((evtScaledValue_t *)clip);
        }
    }
}

evtPolygonMovieWork_t *InitPolygonMovieWorkData(void)
{
    evtPolygonMovieWork_t *work;

    work = sdfChipAllocDirect(sizeof(evtPolygonMovieWork_t));
    if (work == NULL) {
        return work;
    }
    memset(work, 0, sizeof(evtPolygonMovieWork_t));
    return work;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData1);

void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *work)
{
    if (work != NULL) {
        if (work->effectBasicTable != NULL) {
            sdfChipFreeDirect(work->effectBasicTable);
        }
        if (work->mesHandle >= 0) {
            itfMesMngRelease(work->mesHandle);
        }
        fileSyncAll();
        if (work->file04 != NULL) {
            fileHandleFree(work->file04);
        }
        if (work->file4C != NULL) {
            fileHandleFree(work->file4C);
        }
        if (work->mem08 != NULL) {
            sdfMemFreeDirect(work->mem08);
        }
        if (work->mem50 != NULL) {
            sdfMemFreeDirect(work->mem50);
        }
        sdfChipFreeDirect(work);
    }
}

/* Allocate a resource block holding a fresh "PMD2" header; returns the handle. */
sdfMemHandle_t *evtAllocMovieWork(pmdHeader_t **out)
{
    s32 header[16] = {0, 0, 0x32444D50, 0, 1, 4, 0, 0, 0, 0x10, 1, 0x30, 0, 999, 1000, 0};
    s32 headerBytes;
    sdfMemHandle_t *handle;
    void *block;

    headerBytes = 0x40;
    handle = sdfMemAllocDirect(headerBytes);
    block = (void *)sdfMemLock(handle);
    memcpy(block, header, headerBytes);
    *out = block;
    return handle;
}

void SetPMFileName(s32 eventNo, s32 cutNo, char *pm1file, char *pm2file)
{
    sprintf(pm1file, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM1", eventNo / 10 * 10, eventNo, eventNo, cutNo, eventNo, cutNo);
    sprintf(pm2file, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2", eventNo / 10 * 10, eventNo, eventNo, cutNo, eventNo, cutNo);
}

/* NON_MATCHING: 2 of 60 words differ (first at +0x5C). */
#ifdef NON_MATCHING
evtPolygonMovieWork_t *evtGetPolygonMovieWorkData2(s32 eventNo, s32 cutNo, s32 type)
{
    char pm1file[64];
    char pm2file[64];
    void *pm1data;
    void *pm2data;
    evtPolygonMovieWork_t *work;
    sdfMemHandle_t *handle1;
    sdfMemHandle_t *handle2;

    SetPMFileName(eventNo, cutNo, pm1file, pm2file);
    handle1 = sdfDevLoadFileEx(pm1file, &pm1data, 0);
    if (pm1data == NULL) {
        return NULL;
    }
    if (type == 0) {
        handle2 = sdfDevLoadFileEx(pm2file, &pm2data, 0);
    } else {
        handle2 = evtAllocMovieWork((pmdHeader_t **)&pm2data);
    }
    work = InitPolygonMovieWorkData();
    evtGetPolygonMovieWorkData1(work, pm1data, pm2data);
    if (work == NULL) {
        evtFreePolygonMovieWorkData(work);
        return NULL;
    }
    work->mem08 = handle1;
    work->mem50 = handle2;
    work->eventNo = eventNo;
    work->cutNo = cutNo;
    return work;
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData2);
#endif

void evtStartPolygonMovie2(s32 priority, s32 eventNo, s32 cutNo)
{
    char pm1file[64];
    char pm2file[64];
    evtPolygonMovieWork_t *work;

    SetPMFileName(eventNo, cutNo, pm1file, pm2file);
    work = InitPolygonMovieWorkData();
    work->eventNo = eventNo;
    work->cutNo = cutNo;
    work->file04 = fileLoad(pm1file);
    work->movieFlags |= 2;
    work->file4C = fileLoad(pm2file);
    work->movieFlags |= 4;
    work->processId = dds3StartProcess("PolygonMovie", priority, 1, 1,
                                       (dds3ProcessFunc_t)evtPolygonMovieProcess1,
                                       (dds3ProcessFunc_t)evtPolygonMovieShutdown, work);
}

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", evtCameraFrameState);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", nowfovy);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", D_0054F8E0);

