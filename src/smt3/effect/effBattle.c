#include "common.h"
#include "effBattle.h"
#include "nb.h"
#include "basic.h"
#include "ps2asm.h"

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
extern s32 printf(const char *format, ...);
extern f32 D_0031B8B0[4];
extern f32 D_0031B8A0[4];
/* Non-GP BSS words: incomplete extern arrays preserve the native lui/lw
 * accesses (DDS idioms.md, "Declaring non-GP data"). */
extern u32 effBTLSkillTargetNumber[];
extern u32 effBTLSkillTargetBit[];
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
extern u16 D_0031B8C0[];
extern sdfVector4_t D_00319FA0[];
extern sdfVector4_t D_0031B8F0[];
extern s32 fldSkyGetAlpha(void);
extern void *memset(void *a, s32 b, u32 c);

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

typedef struct effBTLLightParams_t {
    f32 *sources[4];
    f32 ambient[4];
} effBTLLightParams_t;

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

extern f32 D_00569970[4];
extern f32 D_00569980[4];
extern f32 D_00569990[4];
extern f32 D_005699A0[4];
extern fldResourceLight_t D_005699B0;
extern u32 abgr;
extern u32 D_00551D84;
extern u32 D_0054F18C;
extern u32 D_0054F190;
extern u32 D_0031CA90[];
extern s32 effBTLFieldColorIsOn(void);
extern void effBTLFieldColorGetBaseColor(void);
extern void effBTLFieldColorStart(void);
extern f64 fptodp(f32 value);

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

extern effBTLColorRow_t D_00317750[];
extern f32 D_00532EA0[4];
extern f32 D_00532EB0[4];
extern f32 D_00532EC0[4];
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

extern effBTLLightParams_t dds3LightParam;

typedef f32 effBTLVector4_t[4];

PS2_SYMBOL_AT(effBTLVector4_t, effBTLSceneAmbient, dds3LightParam, 0x10);

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
    return hBTLCutEffect->currentFrame;
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
    if (frame_end < 6) {
        ratio = (f32)(frame_end - frame) / (f32)frame_end;
    } else {
        ratio = 1.0f;
        if (frame_end - 6 < frame) {
            ratio = (f32)(frame_end - frame) / 6.0f;
        }
    }
    SDF_VU0_LOADVECTOR(D_0031B8B0);
    SDF_VU0_LOADVECTOR_B(D_0031B8A0);
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
            nbCameraSetState(D_0031B8C0[hBTLCutEffect->kind]);
        }
    }
}

/* NON_MATCHING: 88 of 263 words differ (first at +0x34). */
#ifdef NON_MATCHING
/* Called above its definition, where ee-gcc assumed int. */
u32 effBTLSkillGetInvokeParty(void);

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
    u32 frame;
    u32 color;
    s32 count;
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
    if (list->shake != 0 && list->shakeFrame == frame) {
        if (list->shake < 0x1F) {
            dds3SetQuake(list->shakeFade, list->shake * 10);
        } else {
            printf("effBattle : shake over[%d]\n", list->shake);
            dds3SetQuake(list->shakeFade, 0x12C);
        }
    }
    SDF_VU0_LOADVECTOR(&formation->rot);
    SDF_VU0_LOADVECTOR_B(D_00532EA0);
    Mult();
    ToMatrix();
    SDF_VU0_STOREMATRIX(&base);
    if (effBTLSkillGetInvokeParty() == 0x100) {
        SDF_VU0_LOADVECTOR(D_00532EB0);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotA);
        SDF_VU0_LOADVECTOR(D_00532EC0);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotB);
    } else {
        SDF_VU0_LOADVECTOR(D_00532EC0);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotA);
        SDF_VU0_LOADVECTOR(D_00532EB0);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&rotB);
    }
    entry = list->entries;
    color = effBTLCutGetAllPartColor(frame, hBTLCutEffect->colorRampEndFrame);
    count = list->count;
    flags = hBTLCutEffect->stat;
    for (i = 0; i < count; i++, entry++) {
        if (entry->timing >= frame) {
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
            SDF_VU0_STOREMATRIX(&marked);
            dds3EffectPartSetRotateMatrix(hBTLCutEffect->parameterWorks[i], &marked);
            break;
        }
        effBTLPartGetRotateMatrixPtr(aim, &direction);
        dds3EffectPartSetDirection(hBTLCutEffect->parameterWorks[i], &direction);
        dds3EffectPartSetArea(hBTLCutEffect->parameterWorks[i], effBTLPartGetRadiusPtr(formation, aim));
        dds3EffectPartSetColor(hBTLCutEffect->parameterWorks[i], color);
        dds3EffectPartDraw(hBTLCutEffect->parameterWorks[i]);
    }
    if (list->callbackFrame == frame && hBTLCutEffect->sourceFrameCallback != NULL) {
        hBTLCutEffect->sourceFrameCallback(hBTLCutEffect);
    }
    if (hBTLCutEffect->triggerFrame == frame && hBTLCutEffect->triggerFrameCallback != NULL) {
        hBTLCutEffect->triggerFrameCallback(hBTLCutEffect);
    }
    effBTLCutSetMax(hBTLCutEffect->kind, hBTLCutEffect->currentFrame);
    hBTLCutEffect->currentFrame++;
}
#else
INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EA0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EB0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EC0);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutDraw);
#endif

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
    return effBTLSkillTargetNumber[0];
}

u32 effBTLSkillGetTargetBit(void) {
    return effBTLSkillTargetBit[0];
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

    D_00551D84 = id;
    if (D_00317750[id].color[0] != 0.0f &&
        (effBTLFieldColorIsOn() == 0 || D_00551D84 == 0x200)) {
        effBTLFieldColorGetBaseColor();
        work.r = D_00317750[id].color[1];
        work.g = D_00317750[id].color[2];
        work.b = D_00317750[id].color[3];
        SDF_VU0_LOADVECTOR_B_MEMOPS(work);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(work);
        format = "FieldColor:%f,%f,%f(%f,%f,%f)\n";
        ar = fptodp(work.r);
        ag = fptodp(work.g);
        ab = fptodp(work.b);
        br = fptodp(D_00317750[id].color[1]);
        bg = fptodp(D_00317750[id].color[2]);
        bb = fptodp(D_00317750[id].color[3]);
        printf(format, ar, ag, ab, br, bg, bb);
        D_00569970[1] = work.r;
        D_00569970[2] = work.g;
        D_00569970[3] = work.b;
        abgr = D_00317750[id].word;
        effBTLFieldColorStart();
    } else if (effBTLFieldColorIsOn() != 0 && id != 0) {
        color = D_0054F190;
        scale.r = (f32)(color & 0xFF) * 0.0078125f;
        scale.g = (f32)((color >> 8) & 0xFF) * 0.0078125f;
        scale.b = (f32)((color >> 16) & 0xFF) * 0.0078125f;
        color = D_0054F18C;
        work.r = (f32)(color & 0xFF) / 255.0f;
        work.g = (f32)((color >> 8) & 0xFF) / 255.0f;
        work.b = (f32)((color >> 16) & 0xFF) / 255.0f;
        SDF_VU0_LOADVECTOR(&scale);
        SDF_VU0_LOADVECTOR_B_MEMOPS(work);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(work);
        D_00569970[1] = work.r;
        D_00569970[2] = work.g;
        D_00569970[3] = work.b;
        abgr = D_0031CA90[0];
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
    SDF_VU0_COPYVECTOR(dds3LightParam.sources[0] + 4, pDir);
}

void effBTLFieldLightAxisSet(const f32 *pAxis);

void effBTLFieldLightAxisSet(const f32 *pAxis) {
    SDF_VU0_COPYVECTOR(effBTLFieldLightAnimAxis, pAxis);
}

void effBTLFieldLightAnimationSet(s32 frame, f32 angle) {
    effBTLFieldLightAnimFrame = 0;
    effBTLFieldLightAnimFrameEnd = frame;
    effBTLFieldLightAnimRotate = angle / frame;
}

void effBTLSkyFilterON(s32 alpha, u16 frame) {
    s32 frameCount = frame;

    effBTLSkyFilterAlphaBase = fldSkyGetAlpha();
    effBTLSkyFilterAlphaTarget = alpha;
    effBTLSkyFilterFrame = frameCount;
    effBTLSkyFilterFrameMax = frameCount;
}

void effBTLSkyFilterOFF(u16 frame) {
    s32 frameCount = frame;

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
    effBTLFieldColorFilter.frame = frame;
    effBTLFieldColorFilter.frameMax = frame;
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
    effBTLFieldColorFilter.frame = frame;
    effBTLFieldColorFilter.frameMax = frame;
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
        source = dds3LightParam.sources[0];
        SDF_VU0_COPYVECTOR(D_00569980, source + 4);
        SDF_VU0_COPYVECTOR(D_00569990, source);
        SDF_VU0_COPYVECTOR(D_005699A0, effBTLSceneAmbient);
        /* fallthrough */
    case 2:
        data = nbGetMainProcessData();
        D_005699B0.DiffuseR = D_00569970[1];
        D_005699B0.DiffuseG = D_00569970[2];
        D_005699B0.DiffuseB = D_00569970[3];
        D_005699B0.AmbientR = D_00569970[1];
        D_005699B0.AmbientG = D_00569970[2];
        D_005699B0.AmbientB = D_00569970[3];
        D_005699B0.InsideR = 0.0f;
        D_005699B0.OutsideR = 0.0f;
        D_005699B0.Soften = 0.0f;
        D_005699B0.Bias = 0.0f;
        D_005699B0.resv = 0;
        D_005699B0.Flag = 0;
        D_005699B0.AnimeType = 0;
        dds3SetLightObjectResoruce(data->light[1], &D_005699B0);
        dds3LinkSceneLight(data->light[1], 0xC);
        dds3LightParam.sources[0][4] = -0.73f;
        dds3LightParam.sources[0][5] = 1.55f;
        dds3LightParam.sources[0][6] = 0.24f;
        effBTLSkyFilterON(0x40, 0xC);
        effBTLFieldFilterON(abgr, 0xC);
        effBTLFieldColorProcess = 3;
        break;
    case 3:
        if (effBTLFieldLightAnimFrameEnd < effBTLFieldLightAnimFrame) {
            break;
        }
        SetRotateMatrixAxis(-effBTLFieldLightAnimRotate, effBTLFieldLightAnimAxis);
        SDF_VU0_LOADVECTOR(dds3LightParam.sources[0] + 4);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(dds3LightParam.sources[0] + 4);
        effBTLFieldLightAnimFrame++;
        break;
    case 4:
        data = nbGetMainProcessData();
        D_005699B0.DiffuseR = D_00569990[0];
        D_005699B0.DiffuseG = D_00569990[1];
        D_005699B0.DiffuseB = D_00569990[2];
        D_005699B0.AmbientR = D_005699A0[0];
        D_005699B0.AmbientG = D_005699A0[1];
        D_005699B0.AmbientB = D_005699A0[2];
        dds3SetLightObjectResoruce(data->light[1], &D_005699B0);
        dds3LinkSceneLight(data->light[1], 0xC);
        SDF_VU0_COPYVECTOR(dds3LightParam.sources[0] + 4, D_00569980);
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
        SDF_VU0_LOADVECTOR(D_0031B8F0);
        return;
    }
    table = D_00319FA0;
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
INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkillRun);

INCLUDE_SDATA(const s32, "effect/effBattle", D_0054F0CD);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLFieldColorProcess);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaTarget);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaBase);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrame);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrameMax);

