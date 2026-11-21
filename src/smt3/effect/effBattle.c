#include "common.h"
#include "kernel_draw.h"
#include "sdk_libgcc.h"
#include "effBattle.h"
#include "nb.h"
#include "basic.h"
#include "ps2asm.h"
#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 60,
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0
};

extern void dds3EffectPartDelete(dds3EffectPart_t *hEffPart);
extern dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *hEffPartSrc);
extern dds3EffectPart_t *dds3EffectPartCreate(u16 type, void *pData);
extern void dds3EffectPartSetScale(dds3EffectPart_t *hEffPart, f32 scale);
extern void sdfChipFreeDirect(void *aPtr);
#include "sdk_libc.h"

extern f32 effBTLFadeColorFrom[4];
extern f32 effBTLFadeColorTo[4];
/* Non-GP BSS words: incomplete extern arrays preserve the native lui/lw
 * accesses (DDS idioms.md, "Declaring non-GP data"). */
extern s8 effBTLSkillRun;
extern s32 effBTLSkillState;
extern s32 effBTLSkillID;
extern s32 effBTLFieldColorProcess;
extern f32 effBTLFieldLightAnimAxis[4];
extern s32 effBTLFieldLightAnimFrame;
extern s32 effBTLFieldLightAnimFrameEnd;
extern f32 effBTLFieldLightAnimRotate;
extern s32 effBTLSkyFilterAlphaBase;
extern s32 effBTLSkyFilterAlphaTarget;
extern u16 effBTLSkyFilterFrame;
extern u16 effBTLSkyFilterFrameMax;
extern void effBTLFieldColorEnd(void);
extern void effBTLFieldColorOFF(void);
extern void dds3EffectFilter(dds3Effect_Filter_t *params);
extern void effBTLFieldColorRun(void);
extern void effBTLPointLightRun(void);
extern void effBTLPointLightOFF(void);
extern void effBTLSkillClearActive(void);
extern void effBTLPartPtrResetAllModelCenterOffset(void);
extern void effBTLSystemPartHandleDelete(void);
extern void nbCameraSetState(s32 state);
extern void sdfMemFreeDirect(void *aHandle);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void *sdfChipAllocDirect(s32 aSize);
extern f32 effBTLPartGetScalePtr(nbFormation_t *pFormation);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);

typedef struct effBTLImage_t {
    s32 words[0xC38 / 4];
} effBTLImage_t;
extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
extern void nbStageFilterAlp(s32 alp);
extern void effBTLPointLightON(void);
extern void effBTLPointLightInit(void);
extern void effBTLSkillSetActive(void);
extern void effBTLCutReset(void);
extern void effBTLPartPtrGetAllModelCenterOffset(void);
extern void effBTLModelShowInit(void);
extern void effBTLSystemPartHandleCreate(void);
extern void effBTLFieldColorON(u32 id);
extern dds3ProcessID_t *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern u16 effBTLCutCameraState[];
extern sdfVector4_t effBTLStageColorTable[];
extern sdfVector4_t effBTLDefaultColor[];
extern s32 fldSkyGetAlpha(void);
extern void *memset(void *a, s32 b, size_t c);

typedef struct effBTLSkillInfo_t {
    nbFormation_t *invokeForm;
    nbFormation_t *targetForm[32];
    u32 targetNumber;
    u32 targetBit;
} effBTLSkillInfo_t;

extern effBTLSkillInfo_t D_005698E0;

typedef struct effBTLFieldFilter_t {
    dds3Effect_Filter_t rect;
    u32 baseColor;
    u32 targetColor;
    u16 frame;
    u16 frameMax;
} effBTLFieldFilter_t;

extern effBTLFieldFilter_t effBTLFieldColorFilter;

typedef struct fldResourceLight_t {
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;

typedef struct effBTLLightWork_t {
    u8 unk00[0x70];
    u16 switchFrame;
} effBTLLightWork_t;

extern f32 effBTLFieldColor[4];
extern f32 effBTLSavedLight1[4];
extern f32 effBTLSavedLight0[4];
extern f32 effBTLSavedAmbient[4];
extern fldResourceLight_t effBTLFieldLight;
extern u32 abgr;
extern u32 effBTLFieldColorId;
extern u32 effBTLLightColor;
extern u32 D_0054F190;
extern u32 effBTLFieldFilter[];
extern s32 effBTLFieldColorIsOn(void);
extern void effBTLFieldColorGetBaseColor(void);
extern void effBTLFieldColorStart(void);

typedef struct effBTLColor_t {
    f32 a;
    f32 r;
    f32 g;
    f32 b;
} effBTLColor_t;

/* Native 20-byte table row: alpha, red, green, blue, then packed color.
 * Array components preserve the indexed address shape; see
 * docs/levers/addressing-and-types.md. */
typedef struct effBTLColorRow_t {
    f32 color[4];
    u32 word;
} effBTLColorRow_t;

extern effBTLColorRow_t effBTLFieldColorTable[];
extern const f32 effBTLCutBaseRotate[4];
extern const f32 effBTLCutRotateA[4];
extern const f32 effBTLCutRotateB[4];
extern void effBTLPointLightSet(void *pLight, void *pFormation);
extern void effBTLModelColorSet(void *pModel, void *pFormation);
extern void effBTLPartGetPositionPtr(void *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos);
extern void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix);
extern f32 effBTLPartGetRadiusPtr(void *pFormation, effBTLAimParams_t *pPosPtr);
extern void effBTLModelGetMarkMatrix2(void *pFormation, s32 mark);
extern void effBTLCutSetMax(u8 kind, u32 frame);
extern void dds3SetQuake(s32 frame, s32 range);
extern void dds3EffectPartSetPosition(dds3EffectPart_t *hEffPart, sdfVector4_t *pPos);
extern void dds3EffectPartSetRotateMatrix(dds3EffectPart_t *hEffPart, sdfMatrix4_t *pMatrix);
extern void dds3EffectPartSetDirection(dds3EffectPart_t *hEffPart, sdfMatrix4_t *pMatrix);
extern void dds3EffectPartSetArea(dds3EffectPart_t *hEffPart, f32 area);
extern void dds3EffectPartSetColor(dds3EffectPart_t *hEffPart, u32 abgr);
extern void dds3EffectPartDraw(dds3EffectPart_t *hEffPart);
extern void Mult(void);
extern void ToMatrix(void);
extern void MultMatrixNoArg(void);
extern void SetRotateMatrixAxis(f32 aAngle, const f32 *aAxis);
extern void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource);
extern void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame);
extern void dds3UnlinkSceneLight(dds3Basic_t *basic);

extern dds3LightState_t dds3LightParam;

typedef f32 effBTLVector4_t[4];

SYMBOL_AT(effBTLVector4_t, effBTLSceneAmbient, dds3LightParam, 0x10);

#ifdef NON_MATCHING
extern void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData);
#endif

effBTL_t *effBTLCreate(void *pData) {
    sdfMemHandle_t *handle;
    effBTL_t *work;

    handle = sdfMemAllocDirect(0xC44);
    work = sdfMemLock(handle);
    work->resource = handle;
    *(effBTLImage_t *)work = *(effBTLImage_t *)pData;
    effBTLPartCreate(work, pData);
    work->CutCount = 0;
    work->hCutListTop = NULL;
    printf("effBTL:handle create\n");
    return work;
}

#ifdef NON_MATCHING
extern void effBTLPartDelete(effBTL_t *pSkill);
#endif

void effBTLDelete(effBTL_t *hBTLEffect) {
    effBTLCut_t *cut;
    effBTLCut_t *next;

    cut = hBTLEffect->hCutListTop;
    while (cut != NULL) {
        next = cut->next;
        effBTLCutDelete(cut);
        cut = next;
    }
    effBTLPartDelete(hBTLEffect);
    sdfMemFreeDirect(hBTLEffect->resource);
    printf("effBTL:handle delete\n");
}

s32 effBTLGetMotionChangeFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    return hBTLEffect->banks[bank].list.callbackFrame;
}

s32 effBTLGetCutEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    return hBTLEffect->banks[bank].list.frame;
}

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    if (CutNum == 2) {
        return hBTLEffect->banks[bank].list.frame;
    }
    return hBTLEffect->banks[bank].list.colorRampEndFrame;
}

void effBTLCutListRelease(effBTLCut_t *hBTLCutEffect) {
    effBTLCut_t **link = &hBTLCutEffect->state->hCutListTop;
    effBTLCut_t *cur;

    while ((cur = *link) != NULL) {
        if (cur == hBTLCutEffect) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

effBTLCut_t *effBTLCutCreate(effBTL_t *hBTLEffect, u16 CutNum, void *pFormation, s32 stat) {
    effBTLCut_t *cut;
    effBTL_SkillCutTable_t *list;
    effBTL_SkillPartsList_t *entry;
    dds3EffectPart_t **work;
    u8 actorScaleEnabled;
    s32 remaining;
    u16 count;

    cut = sdfChipAllocDirect(0x120);
    memset(cut, 0, 0x120);
    cut->state = hBTLEffect;
    cut->next = hBTLEffect->hCutListTop;
    hBTLEffect->hCutListTop = cut;
    list = (CutNum < 2) ? &hBTLEffect->banks[0].list : &hBTLEffect->banks[1].list;
    cut->triggerFrame = list->frame;
    cut->kind = CutNum;
    cut->list = list;
    if (CutNum == 2) {
        cut->colorRampEndFrame = list->frame;
    } else {
        cut->colorRampEndFrame = list->colorRampEndFrame;
    }
    work = cut->parameterWorks;
    cut->stat = stat;
    cut->formation = pFormation;
    count = cut->list->count;
    entry = cut->list->entries;
    actorScaleEnabled = cut->list->actorScaleEnabled;
    if (count != 0) {
        remaining = count;
        do {
            if (entry->type != 0xFFFE) {
                *work = dds3EffectPartCopy(entry->value.work);
                if (actorScaleEnabled != 0 && pFormation != NULL) {
                    u8 scaleKind = entry->pos_ptr.scaleKind;

                    if (scaleKind == 0 || scaleKind == 4) {
                        dds3EffectPartSetScale(*work, effBTLPartGetScalePtr(cut->formation));
                    }
                }
            } else {
                *work = NULL;
            }
            work++;
            entry++;
            remaining--;
        } while (remaining != 0);
    }
    cut->currentFrame = 0;
    printf("effBTL:cut %d handle create(%p)[%d]\n", cut->kind, cut->state, cut->state->CutCount);
    cut->state->CutCount++;
    return cut;
}

/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
void effBTLCutDelete(effBTLCut_t *hBTLCutEffect) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_WORK_CAPACITY; workIndex++) {
        if (hBTLCutEffect->parameterWorks[workIndex] != 0) {
            dds3EffectPartDelete(hBTLCutEffect->parameterWorks[workIndex]);
        }
    }
    effBTLCutListRelease(hBTLCutEffect);
    hBTLCutEffect->state->CutCount--;
    printf("effBTL:cut %d handle delete(%p)[%d]\n", hBTLCutEffect->kind, hBTLCutEffect->state,
           hBTLCutEffect->state->CutCount);
    sdfChipFreeDirect(hBTLCutEffect);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBTLCutGetCutNumber(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->kind;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBTLCutGetFrame(effBTLCut_t *hBTLCutEffect) {
    return SDF_FRAME_TICK(hBTLCutEffect->currentFrame);
}

void *effBTLCutGetFormation(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->formation;
}

void effBTLCutSetMotionChangeCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallbackFunc) {
    hBTLCutEffect->sourceFrameCallback = pCallbackFunc;
}

/* Store the callback used when the current frame reaches triggerFrame. */
void effBTLCutSetCutEndCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallbackFunc) {
    hBTLCutEffect->triggerFrameCallback = pCallbackFunc;
}

/* Read the callback frame from the linked source. */
s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->list->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBTLCutSetCutEndFrame(effBTLCut_t *hBTLCutEffect, s32 frame) {
    hBTLCutEffect->triggerFrame = frame;
    if (hBTLCutEffect->kind == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        hBTLCutEffect->colorRampEndFrame = frame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBTLCutGetCutEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBTLCutSetDrawEndFrame(effBTLCut_t *hBTLCutEffect, u32 frame) {
    if (hBTLCutEffect->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (frame < hBTLCutEffect->colorRampEndFrame) {
            hBTLCutEffect->colorRampEndFrame = frame;
        }
        return;
    }
    hBTLCutEffect->colorRampEndFrame = frame;
}

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBTLCutGetDrawEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->colorRampEndFrame;
}

void effBTLCutSetUserData(effBTLCut_t *hBTLCutEffect, void *pData) {
    hBTLCutEffect->userData = pData;
}

void *effBTLCutGetUserData(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->userData;
}

typedef struct effBTLColorWork_t {
    u32 word;
    u8 pad04[12];
} effBTLColorWork_t;

u32 effBTLCutGetAllPartColor(u32 frame, u32 frame_end) {
    effBTLColorWork_t work[1];
    u32 color;
    char *format;
    s32 ar;
    s32 ag;
    s32 ab;
    s32 br;
    s32 bg;
    s32 bb;
    f32 ratio;

    if (frame_end == 0 || frame >= frame_end) {
        return 0x808080;
    }
    if (frame_end < SDF_FRAMES(6)) {
        ratio = (f32)(frame_end - frame) / (f32)frame_end;
    } else {
        ratio = 1.0f;
        if (frame_end - SDF_FRAMES(6) < frame) {
            ratio = (f32)(frame_end - frame) / SDF_FRAMES_F(6.0f);
        }
    }
    SDF_VU0_LOADVECTOR(effBTLFadeColorFrom);
    SDF_VU0_LOADVECTOR_B(effBTLFadeColorTo);
    SDF_VU0_SCALEVECTOR(1.0f - ratio);
    SDF_VU0_SCALEVECTOR_B(ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_dds3ColorFtoI(color, 128.0f);
    work[0].word = color;
    return color;
}

void effBTLCutSetCameraState(effBTLCut_t *hBTLCutEffect) {
    s32 dummy;
    s32 *block = &dummy;

    if ((nbGetMainProcessData()->stat & 0x1000) == 0) {
        if (hBTLCutEffect->kind < 3) {
            nbCameraSetState(effBTLCutCameraState[hBTLCutEffect->kind]);
        }
    }
}

/* Canonical declaration of the provider defined below. */
extern const char effBTLShakeFormat[];
u32 effBTLSkillGetInvokeParty(void);

const f32 effBTLCutBaseRotate[4] = {0.0f, 1.0f, 0.0f, 0.0f};

const f32 effBTLCutRotateA[4] = {0.0f, 1.0f, 0.0f, 0.0f};

const f32 effBTLCutRotateB[4] = {0.0f, 0.0f, 0.0f, 1.0f};

const char effBTLShakeFormat[28] = "effBattle : shake over[%d]\n";

void effBTLCutDraw(effBTLCut_t *hBTLCutEffect) {
    sdfVector4_t position;
    sdfMatrix4_t base;
    sdfMatrix4_t rotA;
    sdfMatrix4_t rotB;
    sdfMatrix4_t direction;
    sdfMatrix4_t marked;
    effBTL_SkillCutTable_t *list;
    effBTL_SkillPartsList_t *entry;
    effBTLAimParams_t *aim;
    nbFormation_t *formation;
    s32 count;
    u32 frame;
    u32 color;
    s32 flags;
    s32 i;
    u8 kind;

    frame = hBTLCutEffect->currentFrame;
    list = hBTLCutEffect->list;
    formation = hBTLCutEffect->formation;
    if (frame == 0) {
        effBTLCutSetCameraState(hBTLCutEffect);
        effBTLPointLightSet(list->colorKeys, formation);
        effBTLModelColorSet(list->rgbKeys, formation);
    }
    /* The quake draws from the shared random stream, so it starts on a retail
     * tick's first host tick. */
    if (list->shake != 0 && list->shakeFrame == SDF_FRAME_TICK(frame) &&
        SDF_FRAME_DUE()) {
        if (list->shake < 0x1F) {
            dds3SetQuake(list->shakeFade, list->shake * 10);
        } else {
            printf(effBTLShakeFormat, list->shake);
            dds3SetQuake(list->shakeFade, 0x12C);
        }
    }
    SDF_VU0_LOADVECTOR(&formation->rot);
    SDF_VU0_LOADVECTOR_B(effBTLCutBaseRotate);
    Mult();
    ToMatrix();
    SDF_VU0_STOREMATRIX(&base);
    if (effBTLSkillGetInvokeParty() == 0x100) {
        SDF_VU0_LOADVECTOR(effBTLCutRotateA);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotA);
        SDF_VU0_LOADVECTOR(effBTLCutRotateB);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotB);
    } else {
        SDF_VU0_LOADVECTOR(effBTLCutRotateB);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotA);
        SDF_VU0_LOADVECTOR(effBTLCutRotateA);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotB);
    }
    entry = list->entries;
    color = effBTLCutGetAllPartColor(frame, SDF_FRAMES(hBTLCutEffect->colorRampEndFrame));
    count = list->count;
    flags = hBTLCutEffect->stat;
    for (i = 0; i < count; i++, entry++) {
        if (SDF_FRAMES(entry->timing + 1) - 1 >= frame) {
            continue;
        }
        kind = entry->reverberations;
        if (kind != 0) {
            if ((flags & 1) == 0) {
                continue;
            }
            if (kind == 1 && (flags & 2) != 0) {
                continue;
            }
            if (kind == 2 && (flags & 2) == 0) {
                continue;
            }
        }
        if (hBTLCutEffect->parameterWorks[i] == NULL) {
            continue;
        }
        aim = &entry->pos_ptr;
        effBTLPartGetPositionPtr(formation, aim, &position);
        dds3EffectPartSetPosition(hBTLCutEffect->parameterWorks[i], &position);
        switch (aim->scaleKind) {
        case 0:
        case 4:
            dds3EffectPartSetRotateMatrix(hBTLCutEffect->parameterWorks[i], &base);
            break;
        case 1:
            dds3EffectPartSetRotateMatrix(hBTLCutEffect->parameterWorks[i], &rotA);
            break;
        case 2:
            dds3EffectPartSetRotateMatrix(hBTLCutEffect->parameterWorks[i], &rotB);
            break;
        case 3:
            break;
        case 5:
            effBTLModelGetMarkMatrix2(formation, entry->pos_ptr.aimMode);
            SDF_VU0_LOADMATRIX_B(&base);
            MultMatrixNoArg();
            /* VU writes marked for the next external call, with no intervening
             * C access to marked. This target's verified bare-address handoff
             * preserves those stores without a general memory barrier. */
            SDF_VU0_STOREMATRIX_NOMEM(&marked);
            dds3EffectPartSetRotateMatrix(hBTLCutEffect->parameterWorks[i], &marked);
            break;
        }
        effBTLPartGetRotateMatrixPtr(aim, &direction);
        dds3EffectPartSetDirection(hBTLCutEffect->parameterWorks[i], &direction);
        dds3EffectPartSetArea(hBTLCutEffect->parameterWorks[i], effBTLPartGetRadiusPtr(formation, aim));
        dds3EffectPartSetColor(hBTLCutEffect->parameterWorks[i], color);
        dds3EffectPartDraw(hBTLCutEffect->parameterWorks[i]);
    }
    if (SDF_FRAMES(list->callbackFrame) == frame && hBTLCutEffect->sourceFrameCallback != NULL) {
        hBTLCutEffect->sourceFrameCallback(hBTLCutEffect);
    }
    if (SDF_FRAMES(hBTLCutEffect->triggerFrame) == frame && hBTLCutEffect->triggerFrameCallback != NULL) {
        hBTLCutEffect->triggerFrameCallback(hBTLCutEffect);
    }
    effBTLCutSetMax(hBTLCutEffect->kind, SDF_FRAME_TICK(hBTLCutEffect->currentFrame));
    hBTLCutEffect->currentFrame++;
}

void effBTLCutSetScale(effBTLCut_t *hBTLCutEffect, f32 scale) {
    s32 i;
    s32 count;

    if (scale != 1.0f) {
        count = hBTLCutEffect->list->count;
        for (i = 0; i < count; i++) {
            dds3EffectPartSetScale(hBTLCutEffect->parameterWorks[i], scale);
        }
    }
}

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData) {
    u8 *sourceBytes = (u8 *)pData;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;
        u16 entryCountForBank = pSkill->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->type != 0xFFFE) {
                    if (entry->type == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 sourceBank = index / 0x3C;
                        u32 sourceEntryIndex = index % 0x3C;
                        effBTL_SkillPartsList_t *sourceEntry = &pSkill->banks[sourceBank].list.entries[sourceEntryIndex];
                        dds3EffectPart_t *duplicatedWork = dds3EffectPartCopy(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->type;

                        entry->value.work = duplicatedWork;
                        entry->type = sourceKind;
                    } else {
                        entry->value.work = dds3EffectPartCreate(entry->type, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->type == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
                        entry->timing++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
    }
}

/* Release each initialized parameter-work entry in the handle's two banks. */
void effBTLPartDelete(effBTL_t *pSkill) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = pSkill->banks[bankIndex].list.count;
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->type != 0xFFFE) {
                    dds3EffectPartDelete(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}

void effBTLSkillInit(void) {
    memset(&D_005698E0, 0, sizeof(D_005698E0));
    effBTLSkillRun = 0;
    effBTLSkillState = 0;
    printf("effBTLSkillInit \n");
}

void effBTLSkillStart(u32 id, u32 stat) {
    effBTLSkillID = id;
    effBTLFieldColorON(id);
    if ((u32)effBTLSkillID < 0x200) {
        if (effBTLSkillID != 0) {
            nbCameraSetState(6);
        } else {
            nbCameraSetState(5);
        }
    }
    effBTLPointLightON();
    effBTLPartPtrGetAllModelCenterOffset();
    effBTLSkillSetActive();
    effBTLCutReset();
    effBTLSkillRun = 1;
    printf("effBTL:Set Stat[%X]\n", stat);
    printf("effBTL:Skill Start[%X]\n", id);
}

void effBTLSkillEnd(void) {
    effBTLFieldColorOFF();
    if ((u32)effBTLSkillID < 0x200) {
        nbCameraSetState(0xA);
    }
    effBTLPointLightOFF();
    effBTLSkillClearActive();
    effBTLPartPtrResetAllModelCenterOffset();
    effBTLSkillRun = 0;
    printf("effBTL:Skill End\n");
}

void effBTLSkillSetFormationInfo(s32 InvokeID, u32 TargetBit) {
    nbFormation_t *form;
    nbFormation_t **target;
    u32 bit;
    s32 count;
    s32 i;

    form = nbGetMainProcessData()->form;
    D_005698E0.targetBit = TargetBit;
    D_005698E0.invokeForm = &form[InvokeID];
    target = D_005698E0.targetForm;
    bit = 1;
    count = 0;
    for (i = 0; i < 32; i++) {
        if (TargetBit & bit) {
            *target++ = form;
            count++;
        }
        bit <<= 1;
        form++;
    }
    D_005698E0.targetNumber = count;
    printf("effBTL:Skill Set Formation Info\n");
}

nbFormation_t *effBTLSkillGetInvokeFormation(void) {
    nbMainProcessData_t *data;

    if (D_005698E0.invokeForm == NULL) {
        data = nbGetMainProcessData();
        printf("effBTL:InvokeForm == NULL ****** !!\n");
        return data->form;
    }
    return D_005698E0.invokeForm;
}

u32 effBTLSkillGetInvokeParty(void) {
    nbFormation_t *form = D_005698E0.invokeForm;

    if (form == NULL) {
        return 0x100;
    }
    return form->stat & 0x700;
}

nbFormation_t *effBTLSkillGetTargetFormation(s32 number) {
    if (D_005698E0.targetNumber == 0) {
        printf("error: effBTLSkillInfo.target_number == 0\n");
        return D_005698E0.invokeForm;
    }
    return D_005698E0.targetForm[number];
}

u32 effBTLSkillGetTargetParty(void) {
    nbFormation_t *form = D_005698E0.targetForm[0];

    if (form == NULL) {
        return 0x100;
    }
    return form->stat & 0x700;
}

u32 effBTLSkillGetTargetNumber(void) {
    return D_005698E0.targetNumber;
}

u32 effBTLSkillGetTargetBit(void) {
    return D_005698E0.targetBit;
}

s32 effBTLSkillCheckRun(void) {
    return effBTLSkillRun;
}

void effBTLSkillSetState(s32 state) {
    effBTLSkillState = state;
    printf("effBTL:Set Stat[%X]\n", state);
}

s32 effBTLSkillGetState(void) {
    return effBTLSkillState;
}

s32 effBTLSkillGetID(void) {
    return effBTLSkillID;
}

void effBTLFieldColorInit(void) {
    effBTLFieldColorProcess = 0;
}

void effBTLFieldColorON(u32 id) {
    effBTLColor_t work;
    effBTLColor_t scale;
    u32 color;
    char *format;
    f64 ar;
    f64 ag;
    f64 ab;
    f64 br;
    f64 bg;
    f64 bb;

    effBTLFieldColorId = id;
    if (effBTLFieldColorTable[id].color[0] != 0.0f &&
        (effBTLFieldColorIsOn() == 0 || effBTLFieldColorId == 0x200)) {
        effBTLFieldColorGetBaseColor();
        work.r = effBTLFieldColorTable[id].color[1];
        work.g = effBTLFieldColorTable[id].color[2];
        work.b = effBTLFieldColorTable[id].color[3];
        SDF_VU0_LOADVECTOR_B_MEMOPS(work);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(work);
        format = "FieldColor:%f,%f,%f(%f,%f,%f)\n";
        ar = fptodp(work.r);
        ag = fptodp(work.g);
        ab = fptodp(work.b);
        br = fptodp(effBTLFieldColorTable[id].color[1]);
        bg = fptodp(effBTLFieldColorTable[id].color[2]);
        bb = fptodp(effBTLFieldColorTable[id].color[3]);
        printf(format, ar, ag, ab, br, bg, bb);
        effBTLFieldColor[1] = work.r;
        effBTLFieldColor[2] = work.g;
        effBTLFieldColor[3] = work.b;
        abgr = effBTLFieldColorTable[id].word;
        effBTLFieldColorStart();
    } else if (effBTLFieldColorIsOn() != 0 && id != 0) {
        color = D_0054F190;
        scale.r = (f32)(color & 0xFF) * 0.0078125f;
        scale.g = (f32)((color >> 8) & 0xFF) * 0.0078125f;
        scale.b = (f32)((color >> 16) & 0xFF) * 0.0078125f;
        color = effBTLLightColor;
        work.r = (f32)(color & 0xFF) / 255.0f;
        work.g = (f32)((color >> 8) & 0xFF) / 255.0f;
        work.b = (f32)((color >> 16) & 0xFF) / 255.0f;
        SDF_VU0_LOADVECTOR(&scale);
        SDF_VU0_LOADVECTOR_B_MEMOPS(work);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(work);
        effBTLFieldColor[1] = work.r;
        effBTLFieldColor[2] = work.g;
        effBTLFieldColor[3] = work.b;
        abgr = effBTLFieldFilter[0];
        effBTLFieldColorStart();
    }
    effBTLFieldLightAnimFrame = 0;
    effBTLFieldLightAnimFrameEnd = 0;
    effBTLFieldLightAnimRotate = 0.0f;
}

void effBTLFieldColorOFF(void) {
    effBTLFieldColorEnd();
}

void effBTLFieldColorStart(void) {
    if (effBTLFieldColorProcess == 0) {
        effBTLFieldColorProcess = 1;
    } else {
        effBTLFieldColorProcess = 2;
    }
}

void effBTLFieldColorEnd(void) {
    if (effBTLFieldColorProcess != 0) {
        effBTLFieldColorProcess = 4;
    }
}

void effBTLFieldLightDirectionSet(const f32 *pDir) {
    SDF_VU0_COPYVECTOR(&dds3LightParam.parallel[0]->v, pDir);
}

void effBTLFieldLightAxisSet(const f32 *pAxis);

void effBTLFieldLightAxisSet(const f32 *pAxis) {
    SDF_VU0_COPYVECTOR(effBTLFieldLightAnimAxis, pAxis);
}

void effBTLFieldLightAnimationSet(s32 frame, f32 angle) {
    effBTLFieldLightAnimFrame = 0;
    effBTLFieldLightAnimFrameEnd = SDF_FRAMES(frame);
    effBTLFieldLightAnimRotate = angle / SDF_FRAMES(frame);
}

void effBTLSkyFilterON(s32 alpha, u16 frame) {
    s32 frameCount = SDF_FRAMES(frame);

    effBTLSkyFilterAlphaBase = fldSkyGetAlpha();
    effBTLSkyFilterAlphaTarget = alpha;
    effBTLSkyFilterFrame = frameCount;
    effBTLSkyFilterFrameMax = frameCount;
}

void effBTLSkyFilterOFF(u16 frame) {
    s32 frameCount = SDF_FRAMES(frame);

    effBTLSkyFilterAlphaBase = fldSkyGetAlpha();
    effBTLSkyFilterAlphaTarget = 0x80;
    effBTLSkyFilterFrame = frameCount;
    effBTLSkyFilterFrameMax = frameCount;
}

void effBTLSkyFilterRun(void) {
    s32 target;
    f32 ratio;

    if (effBTLSkyFilterFrame != 0) {
        target = effBTLSkyFilterAlphaTarget;
        ratio = (f32)effBTLSkyFilterFrame / (f32)effBTLSkyFilterFrameMax;
        nbStageFilterAlp((u32)((f32)(effBTLSkyFilterAlphaBase - target) * ratio) + target);
        effBTLSkyFilterFrame--;
    }
}

void effBTLFieldFilterON(u32 abgr, u16 frame) {
    if (frame == 0) {
        effBTLFieldColorFilter.rect.abgr = abgr;
        effBTLFieldColorFilter.frame = 0;
        return;
    }
    effBTLFieldColorFilter.frame = SDF_FRAMES(frame);
    effBTLFieldColorFilter.frameMax = SDF_FRAMES(frame);
    effBTLFieldColorFilter.baseColor = effBTLFieldColorFilter.rect.abgr;
    effBTLFieldColorFilter.targetColor = abgr;
}

void effBTLFieldFilterOFF(u16 frame) {
    if (frame == 0) {
        effBTLFieldColorFilter.frame = 0;
        effBTLFieldColorFilter.rect.abgr = 0;
        return;
    }
    effBTLFieldColorFilter.baseColor = effBTLFieldColorFilter.rect.abgr;
    effBTLFieldColorFilter.targetColor = 0;
    effBTLFieldColorFilter.frame = SDF_FRAMES(frame);
    effBTLFieldColorFilter.frameMax = SDF_FRAMES(frame);
}

void effBTLFieldFilterRun(void) {
    if (effBTLFieldColorFilter.frame != 0) {
        effBTLFieldColorFilter.rect.abgr =
            effColorBlend(effBTLFieldColorFilter.targetColor, effBTLFieldColorFilter.baseColor,
                          (f32)effBTLFieldColorFilter.frame / (f32)effBTLFieldColorFilter.frameMax);
        effBTLFieldColorFilter.frame--;
    }
}

void effBTLFieldFilterDraw(void) {
    if (effBTLFieldColorFilter.rect.abgr & 0xFF000000) {
        dds3EffectFilter(&effBTLFieldColorFilter.rect);
    }
}

void effBTLFieldColorRun(void) {
    nbMainProcessData_t *data;
    f32 *source;

    switch (effBTLFieldColorProcess) {
    case 1:
        source = (f32 *)dds3LightParam.parallel[0];
        SDF_VU0_COPYVECTOR(effBTLSavedLight1, source + 4);
        SDF_VU0_COPYVECTOR(effBTLSavedLight0, source);
        SDF_VU0_COPYVECTOR(effBTLSavedAmbient, effBTLSceneAmbient);
        /* fallthrough */
    case 2:
        data = nbGetMainProcessData();
        effBTLFieldLight.DiffuseR = effBTLFieldColor[1];
        effBTLFieldLight.DiffuseG = effBTLFieldColor[2];
        effBTLFieldLight.DiffuseB = effBTLFieldColor[3];
        effBTLFieldLight.AmbientR = effBTLFieldColor[1];
        effBTLFieldLight.AmbientG = effBTLFieldColor[2];
        effBTLFieldLight.AmbientB = effBTLFieldColor[3];
        effBTLFieldLight.InsideR = 0.0f;
        effBTLFieldLight.OutsideR = 0.0f;
        effBTLFieldLight.Soften = 0.0f;
        effBTLFieldLight.Bias = 0.0f;
        effBTLFieldLight.resv = 0;
        effBTLFieldLight.Flag = 0;
        effBTLFieldLight.AnimeType = 0;
        dds3SetLightObjectResoruce(data->light[1], &effBTLFieldLight);
        dds3LinkSceneLight(data->light[1], 0xC);
        dds3LightParam.parallel[0]->v.x = -0.73f;
        dds3LightParam.parallel[0]->v.y = 1.55f;
        dds3LightParam.parallel[0]->v.z = 0.24f;
        effBTLSkyFilterON(0x40, 0xC);
        effBTLFieldFilterON(abgr, 0xC);
        effBTLFieldColorProcess = 3;
        break;
    case 3:
        if (effBTLFieldLightAnimFrameEnd < effBTLFieldLightAnimFrame) {
            break;
        }
        SetRotateMatrixAxis(-effBTLFieldLightAnimRotate, effBTLFieldLightAnimAxis);
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->v);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[0]->v);
        effBTLFieldLightAnimFrame++;
        break;
    case 4:
        data = nbGetMainProcessData();
        effBTLFieldLight.DiffuseR = effBTLSavedLight0[0];
        effBTLFieldLight.DiffuseG = effBTLSavedLight0[1];
        effBTLFieldLight.DiffuseB = effBTLSavedLight0[2];
        effBTLFieldLight.AmbientR = effBTLSavedAmbient[0];
        effBTLFieldLight.AmbientG = effBTLSavedAmbient[1];
        effBTLFieldLight.AmbientB = effBTLSavedAmbient[2];
        dds3SetLightObjectResoruce(data->light[1], &effBTLFieldLight);
        dds3LinkSceneLight(data->light[1], 0xC);
        SDF_VU0_COPYVECTOR(&dds3LightParam.parallel[0]->v, effBTLSavedLight1);
        effBTLSkyFilterOFF(0xC);
        effBTLFieldFilterOFF(0xC);
        effBTLFieldColorProcess = 5;
        break;
    case 5:
        data = nbGetMainProcessData();
        if (((effBTLLightWork_t *)data->light[1]->workdata)->switchFrame != 0) {
            break;
        }
        effBTLFieldFilterOFF(0);
        dds3UnlinkSceneLight(data->light[1]);
        effBTLFieldColorProcess = 0;
        break;
    case 0:
        return;
    }
    effBTLSkyFilterRun();
    effBTLFieldFilterRun();
}

void effBTLFieldColorDraw(void) {
    if (effBTLFieldColorProcess != 0) {
        effBTLFieldFilterDraw();
    }
}

void effBTLFieldColorGetBaseColor(void) {
    nbMainProcessData_t *data;
    sdfVector4_t *table;
    s16 major;
    s32 minor;

    data = nbGetMainProcessData();
    major = data->StageMajor - 0xC8;
    minor = data->StageMinor;
    if ((u16)major >= 0x28 || minor <= 0 || minor >= 0xB) {
        printf("effBTLFieldColorGetBaseColor:DummyColorLoad\n");
        SDF_VU0_LOADVECTOR(effBTLDefaultColor);
        return;
    }
    table = effBTLStageColorTable;
    table--;
    SDF_VU0_LOADVECTOR(&table[major * 10 + minor]);
}

s32 nbEffectProcess(void) {
    effBTLFieldColorRun();
    effBTLPointLightRun();
    return 0;
}

s32 nbEffectDraw(void) {
    effBTLFieldColorDraw();
    return 0;
}

void nbEffectShutdown(void) {
    effBTLSystemPartHandleDelete();
    nbGetMainProcessData()->EffectProcessID = NULL;
}

void nbInitEffectProcess(nbMainProcessData_t *data) {
    dds3ProcessID_t *process;
    dds3ProcessID_t *draw;

    process = dds3StartProcess("nb_effect", 0x3F9, 2, 1, nbEffectProcess, nbEffectShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, process);
    data->EffectProcessID = process;
    draw = dds3StartProcess("nb_effect_draw", 0x2B0E, 1, 1, nbEffectDraw, NULL, NULL);
    dds3LinkProcess(process, draw);
    effBTLSkillInit();
    effBTLFieldColorInit();
    effBTLPointLightInit();
    effBTLModelShowInit();
    effBTLSystemPartHandleCreate();
}
s8 effBTLSkillRun = 0;

SDATA_PAD(3);

s32 effBTLFieldColorProcess = 0;

s32 effBTLSkyFilterAlphaTarget = 0x80;

s32 effBTLSkyFilterAlphaBase = 0x80;

u16 effBTLSkyFilterFrame = 0;

u16 effBTLSkyFilterFrameMax = 0;


#define DEFAULT_FIELD_COLOR { { 0.0f, 0.50196076f, 0.50196076f, 0.50196076f }, 0x80808080 }

/* Skill IDs 0x000–0x1FF, then four scripted battle color presets. */
effBTLColorRow_t effBTLFieldColorTable[516] = {
    /* 0x000–0x00F */
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.30588234f, 0.18823529f, 0.15686274f }, 0x0C565282 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.35294116f, 0.32156861f, 0.45490194f }, 0x18403A38 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },

    /* 0x010–0x01F */
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.31372548f }, 0x1E463232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.29019607f, 0.18823529f, 0.25882352f }, 0x1630202C },
    { { 1.0f, 0.29019607f, 0.18823529f, 0.25882352f }, 0x1630202C },
    { { 1.0f, 0.23529411f, 0.13333333f, 0.20392156f }, 0x12261622 },
    { { 1.0f, 0.20784313f, 0.15686274f, 0.19607842f }, 0x1E3C283C },
    { { 1.0f, 0.20784313f, 0.15686274f, 0.19607842f }, 0x1E3C283C },
    { { 1.0f, 0.20784313f, 0.15686274f, 0.19607842f }, 0x1E3C283C },
    { { 1.0f, 0.20784313f, 0.15686274f, 0.19607842f }, 0x1E3C283C },

    /* 0x020–0x02F */
    { { 1.0f, 0.20392156f, 0.1490196f, 0.34509802f }, 0x182A262E },
    { { 1.0f, 0.20392156f, 0.1490196f, 0.34509802f }, 0x182A262E },
    { { 1.0f, 0.20392156f, 0.1490196f, 0.34509802f }, 0x182A262E },
    { { 1.0f, 0.20392156f, 0.1490196f, 0.34509802f }, 0x182A262E },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A323232 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A323232 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A323232 },

    /* 0x030–0x03F */
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A323232 },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    { { 1.0f, 0.2117647f, 0.141176463f, 0.18039215f }, 0x1222162A },
    { { 1.0f, 0.17254901f, 0.20392156f, 0.18039215f }, 0x16222A20 },
    { { 1.0f, 0.215686262f, 0.12549019f, 0.243137241f }, 0x16222022 },
    { { 1.0f, 0.17647058f, 0.17647058f, 0.039215684f }, 0x0A1E2828 },
    { { 1.0f, 0.17647058f, 0.17647058f, 0.039215684f }, 0x0A1E2828 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.2745098f }, 0x19322846 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.35294116f }, 0x00000000 },
    { { 1.0f, 0.20392156f, 0.15686274f, 0.141176463f }, 0x141C1622 },
    { { 1.0f, 0.18823529f, 0.094117642f, 0.12549019f }, 0x18161C30 },
    { { 1.0f, 0.20392156f, 0.15686274f, 0.141176463f }, 0x141C1622 },

    /* 0x040–0x04F */
    { { 1.0f, 0.31372548f, 0.25490195f, 0.29411763f }, 0x1E321428 },
    { { 1.0f, 0.23529411f, 0.2745098f, 0.31372548f }, 0x1432320A },
    { { 1.0f, 0.2745098f, 0.23529411f, 0.31372548f }, 0x1E32231E },
    { { 1.0f, 0.23529411f, 0.2745098f, 0.31372548f }, 0x1E32231E },
    { { 1.0f, 0.31372548f, 0.25490195f, 0.29411763f }, 0x1E321428 },
    { { 1.0f, 0.19607842f, 0.23529411f, 0.2745098f }, 0x1E321E14 },
    { { 1.0f, 0.2745098f, 0.23529411f, 0.19607842f }, 0x1E321428 },
    { { 1.0f, 0.1490196f, 0.1490196f, 0.18039215f }, 0x122A2A2A },
    { { 1.0f, 0.19607842f, 0.1490196f, 0.243137241f }, 0x122E1E20 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.22745097f, 0.18823529f, 0.17254901f }, 0x141C1622 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },

    /* 0x050–0x05F */
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A32282E },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A32282E },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A32282E },
    { { 1.0f, 0.1372549f, 0.117647052f, 0.117647052f }, 0x18404E2E },
    { { 1.0f, 0.141176463f, 0.141176463f, 0.2745098f }, 0x12341C2C },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A323232 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.23529411f, 0.23529411f, 0.2745098f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.13333333f, 0.13333333f, 0.26666666f }, 0x14301828 },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A32282E },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x28372319 },
    DEFAULT_FIELD_COLOR,

    /* 0x060–0x06F */
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A3C3C3C },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A3C3C3C },
    { { 1.0f, 0.2745098f, 0.2745098f, 0.2745098f }, 0x0A3C3C3C },
    { { 1.0f, 0.19607842f, 0.23529411f, 0.23529411f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.19607842f, 0.35294116f }, 0x0A323232 },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.117647052f }, 0x0A323246 },
    { { 1.0f, 0.23529411f, 0.215686262f, 0.19607842f }, 0x0A4B4646 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x0A3C2832 },
    { { 1.0f, 0.35294116f, 0.31372548f, 0.2745098f }, 0x1428323C },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.15686274f }, 0x0A0A0A0A },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A0A3232 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x0A3C2828 },
    { { 1.0f, 0.43137253f, 0.2745098f, 0.35294116f }, 0x46141414 },
    { { 1.0f, 0.15686274f, 0.117647052f, 0.117647052f }, 0x0A0A3232 },
    { { 1.0f, 0.23529411f, 0.19607842f, 0.15686274f }, 0x0A28323C },
    { { 1.0f, 0.15686274f, 0.17647058f, 0.15686274f }, 0x0A323232 },

    /* 0x070–0x07F */
    { { 1.0f, 0.19607842f, 0.19607842f, 0.117647052f }, 0x0A323232 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.23529411f }, 0x1E464646 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x143C3C3C },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A3C3C3C },
    { { 1.0f, 0.23529411f, 0.15686274f, 0.23529411f }, 0x0A323232 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.35294116f, 0.35294116f, 0.31372548f }, 0x32464646 },
    { { 1.0f, 0.35294116f, 0.23529411f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.35294116f, 0.35294116f, 0.31372548f }, 0x32464646 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.36862743f, 0.28235293f, 0.49411762f }, 0x122C1C24 },

    /* 0x080–0x08F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.25490195f, 0.19607842f, 0.25490195f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.23529411f, 0.164705873f, 0.12549019f }, 0x122C2040 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.19607842f, 0.19607842f, 0.15686274f }, 0x0A282828 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.23529411f }, 0x0A282828 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.23529411f }, 0x0A282828 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.23529411f }, 0x0A282828 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.23529411f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.18823529f, 0.1490196f, 0.26666666f }, 0x12301220 },

    /* 0x090–0x09F */
    { { 1.0f, 0.19607842f, 0.1490196f, 0.219607831f }, 0x12241222 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.25490195f, 0.19607842f, 0.25490195f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.29019607f, 0.22745097f, 0.2117647f }, 0x16182024 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.15686274f }, 0x0A1E1E1E },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },

    /* 0x0A0–0x0AF */
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.243137241f, 0.12549019f, 0.086274505f }, 0x122C3440 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.15686274f }, 0x0A1E1E1E },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x0B0–0x0BF */
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A282828 },
    { { 1.0f, 0.243137241f, 0.12549019f, 0.086274505f }, 0x122C3440 },
    { { 1.0f, 0.12549019f, 0.219607831f, 0.34509802f }, 0x18564840 },
    { { 1.0f, 0.243137241f, 0.12549019f, 0.086274505f }, 0x122C3440 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.23529411f }, 0x0A3C2828 },
    { { 1.0f, 0.31372548f, 0.28235293f, 0.37647057f }, 0x18403A38 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x1E463232 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x1E463232 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.23529411f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.19607842f, 0.19607842f, 0.1372549f }, 0x0A323232 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.31372548f }, 0x0A463232 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.31372548f }, 0x0A463232 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x193C4646 },
    { { 1.0f, 0.35294116f, 0.35294116f, 0.35294116f }, 0x23643C46 },

    /* 0x0C0–0x0CF */
    { { 1.0f, 0.35294116f, 0.35294116f, 0.35294116f }, 0x235A3C5A },
    { { 1.0f, 0.2117647f, 0.18823529f, 0.28235293f }, 0x162C2624 },
    { { 1.0f, 0.29019607f, 0.22745097f, 0.2117647f }, 0x16182024 },
    { { 1.0f, 0.2117647f, 0.18823529f, 0.28235293f }, 0x162C2624 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.19607842f, 0.19607842f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.17647058f, 0.15686274f, 0.19607842f }, 0x1E282832 },
    { { 1.0f, 0.2745098f, 0.23529411f, 0.23529411f }, 0x14282832 },
    { { 1.0f, 0.17647058f, 0.17647058f, 0.1372549f }, 0x0A282828 },
    { { 1.0f, 0.117647052f, 0.117647052f, 0.15686274f }, 0x0A322828 },
    { { 1.0f, 0.243137241f, 0.18039215f, 0.32156861f }, 0x14402A30 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.117647052f }, 0x0A142828 },

    /* 0x0D0–0x0DF */
    { { 1.0f, 0.19607842f, 0.15686274f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.35294116f }, 0x00000000 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.117647052f }, 0x0A283232 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.35294116f }, 0x00000000 },
    { { 1.0f, 0.17647058f, 0.1372549f, 0.17647058f }, 0x0A321E32 },
    { { 1.0f, 0.18823529f, 0.1490196f, 0.18823529f }, 0x183E2642 },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.117647052f }, 0x0A2D1E2D },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.117647052f }, 0x0A1E2832 },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.117647052f }, 0x0A1E2832 },
    { { 1.0f, 0.243137241f, 0.18039215f, 0.32156861f }, 0x14402A30 },
    { { 1.0f, 0.23529411f, 0.13333333f, 0.20392156f }, 0x12302240 },
    { { 1.0f, 0.19607842f, 0.12549019f, 0.18823529f }, 0x122A1634 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x0E0–0x0EF */
    { { 1.0f, 0.25098038f, 0.22745097f, 0.37647057f }, 0x122C2622 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.15686274f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.243137241f, 0.164705873f, 0.219607831f }, 0x122C262E },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x0F0–0x0FF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.117647052f }, 0x0A141E1E },
    { { 1.0f, 0.19607842f, 0.117647052f, 0.15686274f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x0A282828 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.15686274f, 0.15686274f, 0.19607842f }, 0x0A282828 },
    { { 1.0f, 0.20392156f, 0.141176463f, 0.25098038f }, 0x14362036 },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    { { 1.0f, 0.23529411f, 0.23529411f, 0.2745098f }, 0x00808080 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.15686274f }, 0x0A1E1E1E },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.17254901f, 0.17254901f, 0.17254901f }, 0x3C281E1E },

    /* 0x100–0x10F */
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.243137241f, 0.12549019f, 0.086274505f }, 0x122C3440 },
    { { 1.0f, 0.31372548f, 0.31372548f, 0.31372548f }, 0x0A3C3C3C },
    { { 1.0f, 0.29019607f, 0.22745097f, 0.219607831f }, 0x0E222030 },
    { { 1.0f, 0.29019607f, 0.22745097f, 0.219607831f }, 0x0E222030 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.29411763f, 0.2745098f, 0.2745098f }, 0x0A32323C },
    { { 1.0f, 0.18039215f, 0.1490196f, 0.22745097f }, 0x1430242A },
    { { 1.0f, 0.17647058f, 0.15686274f, 0.15686274f }, 0x0A323237 },
    { { 1.0f, 0.23529411f, 0.19607842f, 0.19607842f }, 0x0A3C3232 },
    { { 1.0f, 0.15686274f, 0.12549019f, 0.15686274f }, 0x182A2C2E },
    { { 1.0f, 0.25098038f, 0.243137241f, 0.33725489f }, 0x1436302E },
    { { 1.0f, 0.17254901f, 0.164705873f, 0.25882352f }, 0x122C2624 },
    { { 1.0f, 0.25098038f, 0.22745097f, 0.25098038f }, 0x12141816 },
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.17254901f, 0.164705873f, 0.18039215f }, 0x0E181C1A },

    /* 0x110–0x11F */
    { { 1.0f, 0.15686274f, 0.07843137f, 0.07843137f }, 0x0A4A4442 },
    { { 1.0f, 0.17254901f, 0.164705873f, 0.25098038f }, 0x142A2020 },
    { { 1.0f, 0.22745097f, 0.18823529f, 0.17254901f }, 0x141C1622 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.23529411f, 0.2745098f, 0.31372548f }, 0x1432320A },
    { { 1.0f, 0.19607842f, 0.12549019f, 0.18823529f }, 0x122A1634 },
    { { 1.0f, 0.15686274f, 0.15686274f, 0.117647052f }, 0x0A141E1E },
    { { 1.0f, 0.35294116f, 0.35294116f, 0.35294116f }, 0x235A3C5A },
    { { 1.0f, 0.23529411f, 0.19607842f, 0.15686274f }, 0x0A28323C },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.15686274f, 0.19607842f, 0.19607842f }, 0x0A323232 },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.117647052f }, 0x0A323246 },
    { { 1.0f, 0.19607842f, 0.12549019f, 0.18823529f }, 0x122A1634 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },
    { { 1.0f, 0.26666666f, 0.243137241f, 0.25098038f }, 0x18222624 },

    /* 0x120–0x12F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x130–0x13F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    { { 1.0f, 0.215686262f, 0.19607842f, 0.19607842f }, 0x1E321E28 },
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x140–0x14F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x150–0x15F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x160–0x16F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x170–0x17F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x180–0x18F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x190–0x19F */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1A0–0x1AF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1B0–0x1BF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1C0–0x1CF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1D0–0x1DF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1E0–0x1EF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x1F0–0x1FF */
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,
    DEFAULT_FIELD_COLOR,

    /* 0x200–0x203 */
    { { 1.0f, 0.23529411f, 0.23529411f, 0.2745098f }, 0x00808080 },
    { { 1.0f, 0.19607842f, 0.15686274f, 0.15686274f }, 0x0A282828 },
    { { 1.0f, 0.43921567f, 0.43921567f, 0.43921567f }, 0x00000000 },
    { { 1.0f, 0.2117647f, 0.2862745f, 0.2862745f }, 0x00000000 },
};

#undef DEFAULT_FIELD_COLOR

#define DEFAULT_STAGE_COLOR { 0.0f, 1.0f, 1.0f, 1.0f }
#define DEFAULT_STAGE_COLOR_GROUP \
    DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, \
    DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR, DEFAULT_STAGE_COLOR

/* Major stages 0xC8–0xEF, with minor stages 1–10 in each group. */
sdfVector4_t effBTLStageColorTable[400] = {
    /* 0xC8 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xC9 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xCA */
    { 0.0f, 0.390625f, 0.390625f, 0.390625f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xCB */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xCC */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xCD */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xCE */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xCF */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD0 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD1 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD2 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD3 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD4 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD5 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD6 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xD7 */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 1.0f, 0.921875f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xD8 */
    { 0.0f, 0.6640625f, 0.6640625f, 0.6640625f },
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    { 0.0f, 1.2109375f, 1.2109375f, 1.2109375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xD9 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xDA */
    { 0.0f, 0.6875f, 0.6875f, 0.6875f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xDB */
    { 0.0f, 1.078125f, 1.078125f, 1.078125f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xDC */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 1.0f, 0.703125f, 1.0f },
    { 0.0f, 0.78125f, 1.0f, 1.0f },
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xDD */
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xDE */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xDF */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE0 */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 1.0f, 1.0f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xE1 */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 1.0f, 0.859375f, 0.78125f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xE2 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE3 */
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xE4 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE5 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE6 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE7 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xE8 */
    DEFAULT_STAGE_COLOR,
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xE9 */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xEA */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 0.84375f, 1.0f, 1.0f },
    { 0.0f, 0.84375f, 0.84375f, 0.84375f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xEB */
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    { 0.0f, 1.171875f, 1.171875f, 1.171875f },
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    DEFAULT_STAGE_COLOR,
    /* 0xEC */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xED */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xEE */ DEFAULT_STAGE_COLOR_GROUP,
    /* 0xEF */ DEFAULT_STAGE_COLOR_GROUP,
};

#undef DEFAULT_STAGE_COLOR_GROUP
#undef DEFAULT_STAGE_COLOR
