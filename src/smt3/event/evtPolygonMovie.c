#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "effect.h"
#include "event.h"
#include "ps2asm.h"


void *dds3GetProcessData(dds3ProcessID_t *id);

/* SMT3 allocates and clears 0xD4 bytes; the process flag helpers use its head. */
typedef struct pmdCutInfo_t {
    s32 FirstFrame;
    s32 LastFrame;
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
    s32 NameIndex;
    s32 DataOffset;
    s32 Type;
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
    dds3Effect_Blur_t *blur2Table;
    u8 unk90[4];
    dds3Effect_PartialMultiBlurParam_t *mblurTable;
    u8 unk98[4];
    dds3Effect_DistortionBlurParam_t *dblurTable;
    u8 unkA0[4];
    dds3Effect_Filter_t *filterTable;
    u8 unkA8[4];
    dds3Effect_PartialMultiFilterParam_t *mfilterTable;
    u8 unkB0[4];
    dds3Effect_RippleBlurParam_t *rblurTable;
    u8 unkB8[4];
    s32 mesHandle;
    dds3ProcessID_t *processId;
    s32 eventNo;
    s32 cutNo;
    s32 nowFrame;
    dds3Basic_t **effectBasicTable;
} evtPolygonMovieWork_t;

typedef struct pmdFrameTable_t {
    u16 ObjectType;
    u16 Frame;
    u16 Length;
    s16 NameIndex;
    union {
        s8 b[8];
        s16 h[4];
        s32 w[2];
        f32 f[2];
    } data;
} pmdFrameTable_t;

void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern dds3Basic_t *dds3GetControlScene(void);
extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, char *name);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void evtPlaySe(s32 eventno, s32 seno);
extern void evtSetBokasi(s32 sw);
extern void dds3SetQuake(s32 frame, s32 range);
extern void dds3StopQuake(void);
extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);
extern void evtSetSkyAlpha(s32 frame, s32 a);
void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *wdata);
extern dds3Basic_t *dds3GetActiveScene(void);
extern void sdfChipFreeDirect(void *aPtr);
extern void itfMesMngRelease(s32 id);
extern void fileSyncAll(void);
extern void fileHandleFree(void *fileHandle);
extern void sdfMemFreeDirect(void *aHandle);
extern void FrameRun(evtPolygonMovieWork_t *wdata);
extern s32 printf(const char *fmt, ...);
extern f32 D_0056CFF0[12];
extern s32 strcmp(const char *a, const char *b);
extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, f32 *pos, f32 *rot);
extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(void *data, f32 *pos, f32 *rot);
extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s);
extern void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, s32 cutno);
extern void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);
extern void dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1, dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);
dds3Basic_t *SearchBasicFromNameIndex_evtPolygonMovie(evtPolygonMovieWork_t *work, s32 nameIndex);
s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f);
extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);
extern void dds3EffectObjectPresetMG12(dds3Basic_t *eff);
extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);
extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);
extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);
extern dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);
extern void evtPlayEventBgm(s32 eventNo, s32 bgm);
extern void evtFadeInEventBgm(s32 eventNo, s32 bgm);
extern void evtStopEventBgm(s32 eventNo, s32 bgm);
extern void evtTransEventBgm(s32 eventNo, s32 bgm);
extern void p2sdBgmVolDown(void);
extern void p2sdBgmVolUp(void);
extern void p2sdStopAllBgm(void);
extern s32 pacFileCheckEnd(void);
extern sdfMemHandle_t *fileGetMemHandle(void *fileHandle);
extern void *fileLoad(const char *filename);
extern sdfMemHandle_t *sdfDevLoadFileEx(const char *name, void *data, s32 mode);
extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
void evtGetPolygonMovieWorkData1(evtPolygonMovieWork_t *wdata, void *pm1data, void *pm2data);
s32 evtPolygonMovieProcess1(dds3ProcessID_t *id);
void evtPolygonMovieShutdown(dds3ProcessID_t *id);
void SetPMFileName(s32 eventno, s32 cutno, char *pm1file, char *pm2file);
evtPolygonMovieWork_t *InitPolygonMovieWorkData(void);
extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern void dds3SetLightColor(s32 frame, s32 no, f32 *col);
extern void dds3SetLightDir(s32 frame, s32 no, f32 *dir);
extern void dds3SetAmbientColor(s32 frame, f32 *col);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);
extern void *evtGetUnitHandle2(dds3Basic_t *unit);
extern void evtLinkLightObjectBasic(void *h, dds3Basic_t *light, s32 frame);
extern void evtUnlinkLightObjectBasic(void *h, s32 frame);
extern s32 sprintf(char *buffer, const char *format, ...);
void evtHokanSceneLight(s32 hokan, f32 ratio, f32 *key1, f32 *key2, f32 *d);
f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2);
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern u8 dds3ScreenBlurFlag;
extern u32 dds3LightParam[];
extern pmdSceneFogTable_t D_0056D020;
extern dds3Effect_Blur_t D_0056D040;
extern dds3Effect_PartialMultiBlurParam_t D_0056D070;
extern dds3Effect_DistortionBlurParam_t D_0056D0A0;
extern dds3Effect_RippleBlurParam_t D_0056D0D0;
extern dds3Effect_PartialMultiFilterParam_t D_0056D100;
extern dds3Effect_Filter_t D_0056D130;
extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far);
extern void Eve_BlurSetParam(dds3Effect_Blur_t *pParam);
extern void Eve_BlurON(void);
extern void Eve_BlurOFF(void);
extern void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *pParam);
extern void Eve_PartialMultiBlurON(void);
extern void Eve_PartialMultiBlurOFF(void);
extern void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *pParam);
extern void Eve_DistortionBlurON(void);
extern void Eve_DistortionBlurOFF(void);
extern void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *pParam);
extern void Eve_RippleBlurON(void);
extern void Eve_RippleBlurOFF(void);
extern void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam);
extern void Eve_PartialMultiFilterON(void);
extern void Eve_PartialMultiFilterOFF(void);
extern void Eve_FilterSetParam(dds3Effect_Filter_t *pParam);
extern void Eve_FilterON(void);
extern void Eve_FilterOFF(void);
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d);
void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d);
void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d);
void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d);
void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d);
void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d);
void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);
s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, char *effname, char *targetname, s32 posindex, s32 cutno);
f32 GetKeyFrameRatio(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now);
void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *wdata, s32 no);
void evtRunUnitMode(dds3Basic_t *unit, s32 mode);
typedef void (*evtFrameFunc_t)(evtPolygonMovieWork_t *, pmdFrameTable_t *, pmdFrameTable_t *, s32);
extern evtFrameFunc_t nowcamera[];
extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);
extern void dds3SetScreenFovy(f32 fovy);
extern s32 evtCameraFrameState;
extern f32 nowfovy;
extern s32 D_0054F8E0;

extern void *sdfChipAllocDirect(u32 aSize);
extern void *memset(void *a, s32 b, u32 c);

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2);

extern f32 evtHokanFogParam[4];

extern u32 D_0032C830[4];


extern dds3Effect_Blur_t D_0032C840;


extern dds3Effect_PartialMultiBlurParam_t D_0032C870;


extern dds3Effect_Filter_t D_0032C8A0;


extern dds3Effect_PartialMultiFilterParam_t D_0032C8C0;


extern dds3Effect_DistortionBlurParam_t D_0032C8F0;


extern dds3Effect_RippleBlurParam_t D_0032C920;

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

s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);

typedef struct evtScaledValue_t {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} evtScaledValue_t;

void dds3PathBasicControlSetRatio(evtScaledValue_t *pControl, f32 ratio);

void dds3PathBasicPauseOn(evtScaledValue_t *value);

void dds3PathBasicPauseOff(evtScaledValue_t *pControl);

extern f32 D_0032C7F0[4];

extern f32 D_0032C800[4];

extern f32 D_0032C810[4];

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

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

extern void *memcpy(void *a, const void *b, u32 c);

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

void UnitFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;

    if (f->Frame == now) {
        if (f->NameIndex >= 0) {
            basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 5, wdata->nameTable[f->NameIndex]);
            if (basic != NULL) {
                evtRunUnitMode(basic, f->data.b[0]);
            }
        }
    }
}

void CameraFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;

    if (f->NameIndex >= 0) {
        basic = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 4, wdata->nameTable[f->NameIndex]);
        if (basic != NULL) {
            if (evtCameraFrameState == 0) {
                D_0054F8E0 = -1;
            }
            if (D_0054F8E0 < f->Frame) {
                D_0054F8E0 = f->Frame;
                evtCameraFrameState = (s32)basic;
                nowfovy = f->data.f[0];
            }
        }
    }
}

s32 QuakeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->Frame == now) {
        if (f->data.h[0] == 0) {
            dds3StopQuake();
        } else {
            dds3SetQuake(f->Length, f->data.h[0]);
        }
    }
}

s32 FadeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->Frame == now) {
        switch (f->data.h[0]) {
        case 0:
            dds3FadeIn(0xFF, 0xFF, 0xFF, f->Length);
            break;
        case 1:
            dds3FadeOut(0xFF, 0xFF, 0xFF, f->Length);
            break;
        case 2:
            dds3FadeIn(0, 0, 0, f->Length);
            break;
        case 3:
            dds3FadeOut(0, 0, 0, f->Length);
            break;
        }
    }
}

void SeFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->Frame == now) {
        evtPlaySe(wdata->eventNo, f->data.h[0]);
    }
}

s32 BgmFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 eventNo;
    s32 bgm;

    if (f->Frame == now) {
        eventNo = wdata->eventNo;
        bgm = f->data.h[1];
        printf("bgm_frame e%03d, bgm %d\n", eventNo, bgm);
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

void BlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 alpha;
    s32 scale;
    s32 elapsed;
    s32 span;

    if (f->data.h[2] != 0) {
        if (nf != NULL) {
            elapsed = now - f->Frame;
            span = nf->Frame - f->Frame;
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

dds3Basic_t *AddEffect(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f)
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
    for (i = 0; i < wdata->effectCount; i++) {
        if (strcmp(wdata->nameTable1[wdata->effectTable[i].NameIndex], wdata->nameTable[f->NameIndex]) == 0) {
            kind = wdata->effectTable[i].Type;
            switch (kind) {
            case 0:
                basic = dds3AddD3PEffectObjectBasic2((u8 *)wdata->header1 + wdata->effectTable[i].DataOffset, position, scale);
                break;
            case 2:
                basic = dds3AddMG12EffectObjectBasic2(1, (u8 *)wdata->header1 + wdata->effectTable[i].DataOffset);
                break;
            case 3:
                basic = dds3AddMG12EffectObjectBasic2(2, (u8 *)wdata->header1 + wdata->effectTable[i].DataOffset);
                break;
            case 1:
                basic = dds3AddBEDEffectObjectBasic2((u8 *)wdata->header1 + wdata->effectTable[i].DataOffset, position, scale);
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

s32 AddEffectTable(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, char *effname, char *targetname, s32 posindex, s32 cutno)
{
    s32 i;
    dds3Basic_t *basic;
    dds3Basic_t *target;
    dds3ObjectBaseControlWorkData_t *control;

    for (i = 0; i < 0x7F; i++) {
        if (wdata->effectBasicTable[i] == NULL) {
            break;
        }
    }
    if (i == 0x7F) {
        return -1;
    }
    basic = AddEffect(wdata, f);
    if (basic == NULL) {
        return -1;
    }
    basic->name = effname;
    wdata->effectBasicTable[i] = basic;
    if (f->data.b[4] != 0) {
        dds3EffectObjectPresetMG12(basic);
    }
    if (targetname == NULL) {
        return i;
    }
    target = dds3SceneBasicSearchName(dds3GetActiveScene(), targetname);
    if (target == NULL) {
        return i;
    }
    switch (target->type >> 24) {
    case BASIC_TYPE_UNITOBJECT:
        if (posindex >= 0) {
            dds3EffectObjectBasicLinkObjectIndex(basic, target, posindex);
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
        evtStopMover((polyMovieObject_t *)control->pBasic[1], 1, 0, 0);
        break;
    }
    return i;
}

s32 AddEffectTableMG12(s32 now, evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f)
{
    dds3Basic_t *parts[4];
    s32 i;
    s32 j;
    dds3Basic_t *basic;
    s8 *name;
    dds3Basic_t **out;

    for (i = 0; i < 0x7F; i++) {
        if (wdata->effectBasicTable[i] == NULL) {
            break;
        }
    }
    if (i == 0x7F) {
        return -1;
    }
    basic = AddEffect(wdata, f);
    if (basic == NULL) {
        return -1;
    }
    basic->name = wdata->nameTable[f->NameIndex];
    wdata->effectBasicTable[i] = basic;
    name = &f->data.b[4];
    out = parts;
    for (j = 0; j < 4; j++) {
        *out = NULL;
        if (*name >= 0) {
            *out = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 7, wdata->nameTable[*name]);
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

void DelEffectTable_evtPolygonMovie(evtPolygonMovieWork_t *wdata, s32 no)
{
    dds3Basic_t *basic;

    if (no >= 0) {
        basic = wdata->effectBasicTable[no];
        if (basic != NULL) {
            dds3DeleteSceneBasicData(basic);
            wdata->effectBasicTable[no] = NULL;
        }
    }
}

void EffectFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    char *target = NULL;
    dds3Basic_t *basic;

    if (f->Frame == now) {
        if (f->data.h[3] >= 0) {
            target = wdata->nameTable[f->data.h[3]];
        }
        f->data.b[0] = AddEffectTable(now, wdata, f, wdata->nameTable[f->NameIndex], target, f->data.b[5], 0);
    } else if (f->data.h[1] > 0) {
        if (now == f->data.h[1]) {
            DelEffectTable_evtPolygonMovie(wdata, f->data.b[0]);
            f->data.b[0] = -1;
        }
    }
    if (f->data.b[0] >= 0) {
        basic = wdata->effectBasicTable[f->data.b[0]];
        if (basic != NULL) {
            if (f->Length == now) {
                if (f->Length != 0) {
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

void MG12FrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *basic;
    dds3Basic_t *p0;
    dds3Basic_t *p1;
    dds3Basic_t *p2;

    if (f->Frame == now) {
        f->data.b[0] = AddEffectTableMG12(now, wdata, f);
    } else if (f->data.h[1] > 0) {
        if (now == f->data.h[1]) {
            DelEffectTable_evtPolygonMovie(wdata, f->data.b[0]);
            f->data.b[0] = -1;
        }
    }
    if (f->data.b[0] >= 0) {
        basic = wdata->effectBasicTable[f->data.b[0]];
        if (basic != NULL) {
            if (f->Length == now) {
                if (f->Length != 0) {
                    evtEffectLoopControl(basic, 0);
                }
            }
        }
        if (f->ObjectType == 0x15) {
            if (basic != NULL) {
                p0 = SearchBasicFromNameIndex_evtPolygonMovie(wdata, f->data.b[4]);
                p1 = SearchBasicFromNameIndex_evtPolygonMovie(wdata, f->data.b[5]);
                p2 = SearchBasicFromNameIndex_evtPolygonMovie(wdata, f->data.b[6]);
                dds3EffectObjectSetMG12Pos(basic, p0, p1, p2,
                                           SearchBasicFromNameIndex_evtPolygonMovie(wdata, f->data.b[7]));
            }
        }
    }
}

void BEDFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    char *target = NULL;

    if (f->Frame == now) {
        if (f->data.h[1] >= 0) {
            target = wdata->nameTable[f->data.h[1]];
        }
        f->data.h[3] = AddEffectTable(now, wdata, f, wdata->nameTable[f->NameIndex], target, -1, f->data.b[4]);
    } else if (f->data.h[0] > 0) {
        if (now == f->data.h[0]) {
            DelEffectTable_evtPolygonMovie(wdata, f->data.h[3]);
            f->data.h[3] = -1;
        }
    }
}

s32 LightFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    dds3Basic_t *light;
    dds3Basic_t *unit;
    void *handle;

    if (f->Frame == now) {
        if (f->data.h[0] >= 0) {
            light = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 9, wdata->nameTable[f->NameIndex]);
            if (light != NULL) {
                unit = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 5, wdata->nameTable[f->data.h[0]]);
                if (unit != NULL) {
                    handle = evtGetUnitHandle2(unit);
                    if (f->data.h[1] != 0) {
                        evtLinkLightObjectBasic(handle, light, f->Length);
                    } else {
                        evtUnlinkLightObjectBasic(handle, f->Length);
                    }
                }
            }
        }
    }
}

void SkyFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->Frame == now) {
        fldSkySetFilter(f->data.w[1], f->data.h[0], f->Length);
        evtSetSkyAlpha(f->Length, f->data.h[1]);
    }
}

s32 BokasiFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    if (f->Frame == now) {
        if (f->data.h[0] != 0) {
            evtSetBokasi(1);
        } else {
            evtSetBokasi(0);
        }
    }
}

f32 GetKeyFrameRatio(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    s32 start;
    s32 end;

    if (now < f->Frame) {
        start = wdata->cutInfo->FirstFrame;
        end = f->Frame;
    } else {
        start = f->Frame;
        if (nf != NULL) {
            end = nf->Frame;
        } else {
            end = wdata->cutInfo->LastFrame;
        }
    }
    if (end != start) {
        return (f32)(now - start) / (f32)(end - start);
    }
    return 0.0f;
}

void SLightFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    f32 *key1;
    f32 *key2;
    f32 soften1;
    f32 bias1;
    f32 soften2;
    f32 bias2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->NameIndex;
        soften1 = f->data.f[0];
        bias1 = f->data.f[1];
        key1 = wdata->slightTable[f->Length];
        if (nf != NULL) {
            bias2 = nf->data.f[1];
            soften2 = nf->data.f[0];
            key2 = wdata->slightTable[nf->Length];
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
        key2 = wdata->slightTable[f->Length];
    }
    evtHokanSceneLight(hokan, ratio, key1, key2, D_0056CFF0);
    dds3SetLightColor(0, 0, D_0056CFF0);
    dds3SetLightDir(0, 0, &D_0056CFF0[4]);
    dds3SetAmbientColor(0, &D_0056CFF0[8]);
    sdfFaceDivSoften = evtHokanFloat(hokan, ratio, soften1, soften2);
    sdfFaceDivBias = evtHokanFloat(hokan, ratio, bias1, bias2);
}

void SFogFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    pmdSceneFogTable_t *key1;
    pmdSceneFogTable_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->sfogTable[f->Length];
        if (nf != NULL) {
            key2 = &wdata->sfogTable[nf->Length];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->sfogTable[f->Length];
    }
    evtHokanSceneFog(hokan, ratio, &key1->fog, &key2->fog, &D_0056D020.fog);
    dds3SetFogPara(0, D_0056D020.fog.vec[1], D_0056D020.fog.vec[0], D_0056D020.fog.vec[2], D_0056D020.fog.vec[3]);
    dds3LightParam[12] = D_0056D020.fog.color;
}

void Blur2FrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_Blur_t *key1;
    dds3Effect_Blur_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->blur2Table[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->blur2Table[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->blur2Table[f->data.h[1]];
    }
    evtHokanBlur2(hokan, ratio, key1, key2, &D_0056D040);
    Eve_BlurSetParam(&D_0056D040);
    if (((u8 *)&D_0056D040.abgr)[3] < 3) {
        Eve_BlurOFF();
    } else {
        Eve_BlurON();
    }
}

void MBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_PartialMultiBlurParam_t *key1;
    dds3Effect_PartialMultiBlurParam_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->mblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->mblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->mblurTable[f->data.h[1]];
    }
    evtHokanMultBlur(hokan, ratio, key1, key2, &D_0056D070);
    Eve_PartialMultiBlurSetParam(&D_0056D070);
    if (((u8 *)&D_0056D070.Blur.abgr)[3] < 3) {
        Eve_PartialMultiBlurOFF();
    } else {
        Eve_PartialMultiBlurON();
    }
}

void DBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_DistortionBlurParam_t *key1;
    dds3Effect_DistortionBlurParam_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->dblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->dblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->dblurTable[f->data.h[1]];
    }
    evtHokanDistBlur(hokan, ratio, key1, key2, &D_0056D0A0);
    Eve_DistortionBlurSetParam(&D_0056D0A0);
    if (((u8 *)&D_0056D0A0.abgr)[3] < 3) {
        Eve_DistortionBlurOFF();
    } else {
        Eve_DistortionBlurON();
    }
}

void RBlurFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_RippleBlurParam_t *key1;
    dds3Effect_RippleBlurParam_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->rblurTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->rblurTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->rblurTable[f->data.h[1]];
    }
    evtHokanRipBlur(hokan, ratio, key1, key2, &D_0056D0D0);
    Eve_RippleBlurSetParam(&D_0056D0D0);
    if (((u8 *)&D_0056D0D0.abgr)[3] < 3) {
        Eve_RippleBlurOFF();
    } else {
        Eve_RippleBlurON();
    }
}

void MFilterFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_PartialMultiFilterParam_t *key1;
    dds3Effect_PartialMultiFilterParam_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->mfilterTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->mfilterTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->mfilterTable[f->data.h[1]];
    }
    evtHokanMultFilter(hokan, ratio, key1, key2, &D_0056D100);
    Eve_PartialMultiFilterSetParam(&D_0056D100);
    if (((u8 *)&D_0056D100.Filter.rgba)[3] < 3) {
        Eve_PartialMultiFilterOFF();
    } else {
        Eve_PartialMultiFilterON();
    }
}

void FilterFrameFunc(evtPolygonMovieWork_t *wdata, pmdFrameTable_t *f, pmdFrameTable_t *nf, s32 now)
{
    f32 ratio;
    dds3Effect_Filter_t *key1;
    dds3Effect_Filter_t *key2;
    s32 hokan;

    ratio = GetKeyFrameRatio(wdata, f, nf, now);
    hokan = 1;
    if (now >= f->Frame) {
        hokan = f->data.h[0];
        key1 = &wdata->filterTable[f->data.h[1]];
        if (nf != NULL) {
            key2 = &wdata->filterTable[nf->data.h[1]];
        } else {
            key2 = NULL;
        }
    } else {
        key1 = NULL;
        key2 = &wdata->filterTable[f->data.h[1]];
    }
    evtHokanFilter(hokan, ratio, key1, key2, &D_0056D130);
    Eve_FilterSetParam(&D_0056D130);
    if (((u8 *)&D_0056D130.abgr)[3] < 2) {
        Eve_FilterOFF();
    } else {
        Eve_FilterON();
    }
}

void FrameRun(evtPolygonMovieWork_t *wdata)
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
    for (i = 0; i < wdata->frameCount; i++) {
        f = &wdata->frameTable[i];
        nf = NULL;
        if (i + 1 < wdata->frameCount) {
            if (wdata->frameTable[i + 1].ObjectType == f->ObjectType) {
                nf = &wdata->frameTable[i + 1];
            }
        }
        if (prev != NULL) {
            if (prev->ObjectType != f->ObjectType) {
                prev = NULL;
            }
        }
        switch (f->ObjectType) {
        case 3:
        case 20:
        case 21:
            if (f->data.h[1] == 0) {
                length = 0;
            } else {
                length = f->data.h[1] - f->Frame + 1;
            }
            break;
        case 18:
            if (f->data.h[0] == 0) {
                length = 0;
            } else {
                length = f->data.h[0] - f->Frame + 1;
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
            length = f->Length;
            break;
        }
        if (length > 0) {
            now = wdata->nowFrame;
            if (now < f->Frame) {
                goto next;
            }
            if (now < f->Frame + length) {
                goto run;
            }
            goto next;
        } else if (length == 0) {
            now = wdata->nowFrame;
            if (now < f->Frame) {
                goto next;
            }
        } else if ((now = wdata->nowFrame) < f->Frame) {
            if (prev != NULL) {
                goto next;
            }
        } else if (nf != NULL) {
            if (now >= nf->Frame) {
                goto next;
            }
        }
    run:
        func = nowcamera[f->ObjectType];
        if (func != NULL) {
            func(wdata, f, nf, now);
        }
    next:
        prev = f;
    }
    if (evtCameraFrameState != 0) {
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), (dds3Basic_t *)evtCameraFrameState);
        dds3SetScreenFovy(nowfovy);
    }
}

s32 evtPolygonMovieProcess2(dds3ProcessID_t *id)
{
    evtPolygonMovieWork_t *work;
    s32 ret;

    work = (evtPolygonMovieWork_t *)dds3GetProcessData(id);
    FrameRun(work);
    work->nowFrame++;
    ret = 0;
    if (work->nowFrame >= work->cutInfo->LastFrame) {
        printf("end polygon movie! frame:%d\n", work->nowFrame);
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

s32 evtCheckPolygonMovieRun(dds3ProcessID_t *id)
{
    evtPolygonMovieWork_t *state;
    s32 set;

    state = (evtPolygonMovieWork_t *)dds3GetProcessData(id);
    set = 0;
    if (state->movieFlags & 8) {
        set = 1;
    }
    return set;
}

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BAF8);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_0053BB20);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtPolygonMovieProcess1);

void evtPolygonMovieShutdown(dds3ProcessID_t *id)
{
    evtFreePolygonMovieWorkData(dds3GetProcessData(id));
    dds3SetProcessData(id, NULL);
}

f32 evtHokanFloat(s32 hokan, f32 ratio, f32 f1, f32 f2)
{
    if (hokan) {
        return f1 * (1.0f - ratio) + f2 * ratio;
    }
    return f1;
}

s32 evtHokanInt(s32 hokan, f32 ratio, s32 i1, s32 i2)
{
    s32 result = i1;

    if (hokan) {
        result = (s32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return result;
}

u32 evtHokanUInt(s32 hokan, f32 ratio, u32 i1, u32 i2)
{
    if (hokan) {
        return (u32)((f32)i1 * (1.0f - ratio) + (f32)i2 * ratio);
    }
    return i1;
}

/* vu0 routine: blend two RGBA8888 colours by t */
u32 evtHokanColor(s32 hokan, f32 ratio, u32 c1, u32 c2)
{
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (hokan) {
        color1[0] = c1;
        MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = c2;
        MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return c1;
}

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtHokanSceneLight(s32 hokan, f32 ratio, f32 *key1, f32 *key2, f32 *d)
{
    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 != NULL) {
        SDF_VU0_LOADVECTOR(key1);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
    if (key2 != NULL) {
        SDF_VU0_LOADVECTOR_B(key2);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C7F0);
    }
    SDF_VU0_SCALEVECTOR_B_V0(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(d);
    if (key1 != NULL) {
        SDF_VU0_LOADVECTOR(key1 + 4);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
    if (key2 != NULL) {
        SDF_VU0_LOADVECTOR_B(key2 + 4);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C800);
    }
    SDF_VU0_SCALEVECTOR_B_V0(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(d + 4);
    if (key1 != NULL) {
        SDF_VU0_LOADVECTOR(key1 + 8);
    } else {
        SDF_VU0_LOADVECTOR(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_V0(1.0f - ratio);
    if (key2 != NULL) {
        SDF_VU0_LOADVECTOR_B(key2 + 8);
    } else {
        SDF_VU0_LOADVECTOR_B(D_0032C810);
    }
    SDF_VU0_SCALEVECTOR_B_V0(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(d + 8);
}

/* vu0 routine: blend a vector and an RGBA8888 colour by t (missing input =
 * default vector/colour) */
void evtHokanSceneFog(s32 hokan, f32 ratio, evtBlendVecColor_t *key1, evtBlendVecColor_t *key2, evtBlendVecColor_t *d)
{

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 != NULL) {
        SDF_VU0_SETVECTOR_X(key1->vec[0]);
        SDF_VU0_SETVECTOR_Y(key1->vec[1]);
        SDF_VU0_SETVECTOR_Z(key1->vec[2]);
        SetBlendVecW(key1->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        SDF_VU0_SETVECTOR_X(key2->vec[0]);
        SDF_VU0_SETVECTOR_Y(key2->vec[1]);
        SDF_VU0_SETVECTOR_Z(key2->vec[2]);
        SetBlendVecW(key2->vec[3]);
    } else {
        SDF_VU0_SETVECTOR_X(evtHokanFogParam[1]);
        SDF_VU0_SETVECTOR_Y(evtHokanFogParam[0]);
        SDF_VU0_SETVECTOR_Z(evtHokanFogParam[2]);
        SetBlendVecW(evtHokanFogParam[3]);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_GETVECTOR_X(d->vec[0]);
    SDF_VU0_GETVECTOR_Y(d->vec[1]);
    SDF_VU0_GETVECTOR_Z(d->vec[2]);
    SDF_VU0_GETVECTOR_W(d->vec[3]);
    if (key1 != NULL) {
        UnpackRGBAWordFloat(key1->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
    SDF_VU0_COPYVECTOR_A_TO_B();
    if (key2 != NULL) {
        UnpackRGBAWordFloat(key2->color, 1.0f / 128.0f);
    } else {
        UnpackRGBAWordFloat(D_0032C830[0], 1.0f / 128.0f);
    }
    SDF_VU0_SCALEVECTOR_BITS(ratio);
    SDF_VU0_ADDVECTOR();
    d->color = PackRGBAWord(128.0f);
}

void evtHokanBlur2(s32 hokan, f32 ratio, dds3Effect_Blur_t *key1, dds3Effect_Blur_t *key2, dds3Effect_Blur_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C840;
    }
    if (key2 == NULL) {
        key2 = &D_0032C840;
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->scale = evtHokanFloat(hokan, ratio, key1->scale, key2->scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->center[positionOrCornerIndex], key2->center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[positionOrCornerIndex][coordinateIndex], key2->xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C870;
    }
    if (key2 == NULL) {
        key2 = &D_0032C870;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    d->Blur.abgr = evtHokanColor(hokan, ratio, key1->Blur.abgr, key2->Blur.abgr);
    d->Blur.blend = key1->Blur.blend;
    d->Blur.rotate = evtHokanFloat(hokan, ratio, key1->Blur.rotate, key2->Blur.rotate);
    d->Blur.scale = evtHokanFloat(hokan, ratio, key1->Blur.scale, key2->Blur.scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->Blur.center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->Blur.center[positionOrCornerIndex], key2->Blur.center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->Blur.xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->Blur.xy[positionOrCornerIndex][coordinateIndex], key2->Blur.xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d);

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d)
{
    s32 cornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C8A0;
    }
    if (key2 == NULL) {
        key2 = &D_0032C8A0;
    }
    for (cornerIndex = 0; cornerIndex < 2; cornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[cornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[cornerIndex][coordinateIndex], key2->xy[cornerIndex][coordinateIndex]);
        }
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
}

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d)
{
    s32 centerCoordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C8C0;
    }
    if (key2 == NULL) {
        key2 = &D_0032C8C0;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        d->center[centerCoordinateIndex] = evtHokanInt(
            hokan, ratio, key1->center[centerCoordinateIndex], key2->center[centerCoordinateIndex]);
    }
    d->Filter.rgba = evtHokanColor(hokan, ratio, key1->Filter.rgba, key2->Filter.rgba);
    d->Filter.blendControl = key1->Filter.blendControl;
}

void evtHokanDistBlur(s32 hokan, f32 ratio, dds3Effect_DistortionBlurParam_t *key1, dds3Effect_DistortionBlurParam_t *key2, dds3Effect_DistortionBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C8F0;
    }
    if (key2 == NULL) {
        key2 = &D_0032C8F0;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->interval = evtHokanUInt(hokan, ratio, key1->interval, key2->interval);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->distortion = evtHokanFloat(hokan, ratio, key1->distortion, key2->distortion);
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->width = evtHokanUInt(hokan, ratio, key1->width, key2->width);
}

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &D_0032C920;
    }
    if (key2 == NULL) {
        key2 = &D_0032C920;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->interval = evtHokanFloat(hokan, ratio, key1->interval, key2->interval);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->power = evtHokanFloat(hokan, ratio, key1->power, key2->power);
    d->frequency = evtHokanFloat(hokan, ratio, key1->frequency, key2->frequency);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
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

void evtSetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(id);
    work->movieFlags |= flag;
}

void evtResetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(id);
    work->movieFlags &= ~flag;
}

u8 evtCheckPolygonMovieFlag(dds3ProcessID_t *id, u32 flag)
{
    evtPolygonMovieWork_t *work = (evtPolygonMovieWork_t *)dds3GetProcessData(id);
    return (work->movieFlags & flag) != 0;
}

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe)
{
    polyMovieClip_t *clip;
    s32 elapsedFrames;
    s32 duration;

    elapsedFrames = nowframe - baseframe;
    if (elapsedFrames < 0) {
        elapsedFrames = 0;
    }
    clip = mover->state->clip;
    if (clip != NULL) {
        duration = (s32)clip->duration;
        if (duration <= elapsedFrames) {
            elapsedFrames = duration;
        }
        dds3PathBasicControlSetRatio((evtScaledValue_t *)clip, (f32)elapsedFrames / (f32)duration);
        if (sw == 0) {
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

void evtFreePolygonMovieWorkData(evtPolygonMovieWork_t *wdata)
{
    if (wdata != NULL) {
        if (wdata->effectBasicTable != NULL) {
            sdfChipFreeDirect(wdata->effectBasicTable);
        }
        if (wdata->mesHandle >= 0) {
            itfMesMngRelease(wdata->mesHandle);
        }
        fileSyncAll();
        if (wdata->file04 != NULL) {
            fileHandleFree(wdata->file04);
        }
        if (wdata->file4C != NULL) {
            fileHandleFree(wdata->file4C);
        }
        if (wdata->mem08 != NULL) {
            sdfMemFreeDirect(wdata->mem08);
        }
        if (wdata->mem50 != NULL) {
            sdfMemFreeDirect(wdata->mem50);
        }
        sdfChipFreeDirect(wdata);
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

void SetPMFileName(s32 eventno, s32 cutno, char *pm1file, char *pm2file)
{
    sprintf(pm1file, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM1", eventno / 10 * 10, eventno, eventno, cutno, eventno, cutno);
    sprintf(pm2file, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2", eventno / 10 * 10, eventno, eventno, cutno, eventno, cutno);
}

/* NON_MATCHING: 2 of 60 words differ (first at +0x5C). */
#ifdef NON_MATCHING
evtPolygonMovieWork_t *evtGetPolygonMovieWorkData2(s32 eventno, s32 cutno, s32 type)
{
    char pm1file[64];
    char pm2file[64];
    void *pm1data;
    void *pm2data;
    evtPolygonMovieWork_t *work;
    sdfMemHandle_t *handle1;
    sdfMemHandle_t *handle2;

    SetPMFileName(eventno, cutno, pm1file, pm2file);
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
    work->eventNo = eventno;
    work->cutNo = cutno;
    return work;
}
#else
INCLUDE_ASM(const s32, "event/evtPolygonMovie", evtGetPolygonMovieWorkData2);
#endif

void evtStartPolygonMovie2(s32 priority, s32 eventno, s32 cutno)
{
    char pm1file[64];
    char pm2file[64];
    evtPolygonMovieWork_t *work;

    SetPMFileName(eventno, cutno, pm1file, pm2file);
    work = InitPolygonMovieWorkData();
    work->eventNo = eventno;
    work->cutNo = cutno;
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

