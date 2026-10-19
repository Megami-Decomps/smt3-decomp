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

extern void dds3EffectPartDelete(dds3EffectPart_t *part);
extern dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *part);
extern dds3EffectPart_t *dds3EffectPartCreate(u16 kind, void *params);
extern void dds3EffectPartSetScale(dds3EffectPart_t *part, f32 scale);
extern void sdfChipFreeDirect(void *ptr);
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
extern void dds3EffectFilter(effSolidRectParams_t *params);
extern void effBTLFieldColorRun(void);
extern void effBTLPointLightRun(void);
extern void effBTLPointLightOFF(void);
extern void effBTLSkillClearActive(void);
extern void effBTLPartPtrResetAllModelCenterOffset(void);
extern void effBTLSystemPartHandleDelete(void);
extern void nbCameraSetState(s32 state);
extern void sdfMemFreeDirect(void *handle);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfChipAllocDirect(s32 size);
extern f32 effBTLPartGetScalePtr(nbFormation_t *formation);
extern char D_00532E50[];
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern char D_00532E20[];

typedef struct effBTLImage_t {
    s32 words[0xC38 / 4];
} effBTLImage_t;
extern u32 effColorBlend(u32 firstColor, u32 secondColor, f32 blend);
extern void nbStageFilterAlp(s32 alpha);
extern void effBTLPointLightON(void);
extern void effBTLPointLightInit(void);
extern void effBTLSkillSetActive(void);
extern void effBTLCutReset(void);
extern void effBTLPartPtrGetAllModelCenterOffset(void);
extern void effBTLModelShowInit(void);
extern void effBTLSystemPartHandleCreate(void);
extern void effBTLFieldColorON(u32 id);
extern dds3ProcessID_t *dds3StartProcess(const char *name, s32 priority, s32 startDelay,
                                         s32 endDelay, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *workData);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);
extern u16 D_0031B8C0[];
extern sdfVector4_t D_00319FA0[];
extern sdfVector4_t D_0031B8F0[];
extern char D_00532E38[];
extern char D_00532F38[];
extern char D_00532F68[];
extern char D_00533028[];
extern char D_00533058[];
extern char D_00533068[];
extern s32 fldSkyGetAlpha(void);
extern void *memset(void *dst, s32 value, u32 count);
extern char D_00532F08[];
extern char D_00532F20[];
extern char D_00532F50[];
extern char D_00532F90[];
extern char D_00532FB8[];

typedef struct effBTLSkillInfo_t {
    nbFormation_t *invokeForm;
    nbFormation_t *targetForm[32];
    u32 targetNumber;
    u32 targetBit;
} effBTLSkillInfo_t;

extern effBTLSkillInfo_t D_005698E0;

typedef struct effBTLFieldFilter_t {
    effSolidRectParams_t rect;
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

typedef struct effBTLLightResource_t {
    u32 resv;
    u32 flag;
    u32 animeType;
    f32 insideR;
    f32 outsideR;
    f32 soften;
    f32 bias;
    f32 diffuseR;
    f32 diffuseG;
    f32 diffuseB;
    f32 ambientR;
    f32 ambientG;
    f32 ambientB;
} effBTLLightResource_t;

typedef struct effBTLLightWork_t {
    u8 unk00[0x70];
    u16 switchFrame;
} effBTLLightWork_t;

extern f32 D_00569970[4];
extern f32 D_00569980[4];
extern f32 D_00569990[4];
extern f32 D_005699A0[4];
extern effBTLLightResource_t D_005699B0;
extern u32 D_00551D88;
extern u32 D_00551D84;
extern u32 D_0054F18C;
extern u32 D_0054F190;
extern u32 D_0031CA90[];
extern s32 effBTLFieldColorIsOn(void);
extern void effBTLFieldColorGetBaseColor(void);
extern void effBTLFieldColorStart(void);
extern s32 sdfFloatToULong(f32 value);

typedef struct effBTLColor_t {
    f32 a;
    f32 r;
    f32 g;
    f32 b;
} effBTLColor_t;

typedef struct effBTLColorRow_t {
    effBTLColor_t color;
    u32 word;
} effBTLColorRow_t;

extern effBTLColorRow_t D_00317750[];
extern char D_00532FE8[];
extern char D_00532ED0[];
extern f32 D_00532EA0[4];
extern f32 D_00532EB0[4];
extern f32 D_00532EC0[4];
extern void effBTLPointLightSet(void *light, void *formation);
extern void effBTLModelColorSet(void *keys, void *formation);
extern void effBTLPartGetPositionPtr(void *formation, effBTLAimParams_t *aim, sdfVector4_t *out);
extern void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *aim, sdfMatrix4_t *out);
extern f32 effBTLPartGetRadiusPtr(void *formation, effBTLAimParams_t *aim);
extern void effBTLModelGetMarkMatrix2(void *formation, s32 mark);
extern void effBTLCutSetMax(u8 kind, u32 frame);
extern void dds3SetQuake(s32 frame, s32 range);
extern void dds3EffectPartSetPosition(dds3EffectPart_t *part, sdfVector4_t *position);
extern void dds3EffectPartSetRotateMatrix(dds3EffectPart_t *part, sdfMatrix4_t *matrix);
extern void dds3EffectPartSetDirection(dds3EffectPart_t *part, sdfMatrix4_t *matrix);
extern void dds3EffectPartSetArea(dds3EffectPart_t *part, f32 area);
extern void dds3EffectPartSetColor(dds3EffectPart_t *part, u32 color);
extern void dds3EffectPartDraw(dds3EffectPart_t *part);
extern void Mult(void);
extern void ToMatrix(void);
extern void MultMatrixNoArg(void);
extern void SetRotateMatrixAxis(f32 angle, const f32 *axis);
extern void dds3SetLightObjectResoruce(dds3Basic_t *basic, effBTLLightResource_t *resource);
extern void dds3LinkSceneLight(dds3Basic_t *basic, s32 frames);
extern void dds3UnlinkSceneLight(dds3Basic_t *basic);

extern effBTLLightParams_t dds3LightParam;

typedef f32 effBTLVector4_t[4];

PS2_SYMBOL_AT(effBTLVector4_t, effBTLSceneAmbient, dds3LightParam, 0x10);

effBTL_t *effBTLCreate(void *data) {
    sdfMemHandle_t *handle;
    effBTL_t *work;

    handle = sdfMemAllocDirect(0xC44);
    work = sdfMemLock(handle);
    work->resource = handle;
    *(effBTLImage_t *)work = *(effBTLImage_t *)data;
    effBTLPartCreate(work, data);
    work->active = 0;
    work->cutList = NULL;
    printf(D_00532E20);
    return work;
}

void effBTLDelete(effBTL_t *work) {
    effBTLCut_t *cut;
    effBTLCut_t *next;

    cut = work->cutList;
    while (cut != NULL) {
        next = cut->next;
        effBTLCutDelete(cut);
        cut = next;
    }
    effBTLPartDelete(work);
    sdfMemFreeDirect(work->resource);
    printf(D_00532E38);
}

s32 effBTLGetMotionChangeFrame(effBTL_t *handle, u16 kind) {
    s32 bank = kind >= 2;
    return handle->banks[bank].list.callbackFrame;
}

s32 effBTLGetCutEndFrame(effBTL_t *handle, u16 kind) {
    s32 bank = kind >= 2;
    return handle->banks[bank].list.triggerFrame;
}

s32 effBTLGetDrawEndFrame(effBTL_t *handle, u16 kind) {
    s32 bank = kind >= 2;
    if (kind == 2) {
        return handle->banks[bank].list.triggerFrame;
    }
    return handle->banks[bank].list.colorRampEndFrame;
}

void effBTLCutListRelease(effBTLCut_t *cut) {
    effBTLCut_t **link = &cut->state->cutList;
    effBTLCut_t *cur;

    while ((cur = *link) != NULL) {
        if (cur == cut) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

effBTLCut_t *effBTLCutCreate(effBTL_t *owner, u16 kind, void *formation, s32 flags) {
    effBTLCut_t *cut;
    effBTLCutData_t *list;
    effBTLPart_t *entry;
    dds3EffectPart_t **work;
    u8 actorScaleEnabled;
    s32 remaining;
    u16 count;

    cut = sdfChipAllocDirect(0x120);
    memset(cut, 0, 0x120);
    cut->state = owner;
    cut->next = owner->cutList;
    owner->cutList = cut;
    list = (kind < 2) ? &owner->banks[0].list : &owner->banks[1].list;
    cut->triggerFrame = list->triggerFrame;
    cut->kind = kind;
    cut->list = list;
    if (kind == 2) {
        cut->colorRampEndFrame = list->triggerFrame;
    } else {
        cut->colorRampEndFrame = list->colorRampEndFrame;
    }
    work = cut->parameterWorks;
    cut->unk18 = flags;
    cut->formation = formation;
    count = cut->list->count;
    entry = cut->list->entries;
    actorScaleEnabled = cut->list->actorScaleEnabled;
    if (count != 0) {
        remaining = count;
        do {
            if (entry->paramKind != 0xFFFE) {
                *work = dds3EffectPartCopy(entry->value.work);
                if (actorScaleEnabled != 0 && formation != NULL) {
                    u8 scaleKind = entry->aimParams.scaleKind;

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
    printf(D_00532E50, cut->kind, cut->state, cut->state->active);
    cut->state->active++;
    return cut;
}

/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
INCLUDE_RODATA(const s32, "effect/effBattle", D_00532E20);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532E38);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532E50);

void effBTLCutDelete(effBTLCut_t *owner) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_WORK_CAPACITY; workIndex++) {
        if (owner->parameterWorks[workIndex] != 0) {
            dds3EffectPartDelete(owner->parameterWorks[workIndex]);
        }
    }
    effBTLCutListRelease(owner);
    owner->state->active--;
    printf("effBTL:cut %d handle delete(%p)[%d]\n", owner->kind, owner->state,
           owner->state->active);
    sdfChipFreeDirect(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBTLCutGetCutNumber(effBTLCut_t *work) {
    return work->kind;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBTLCutGetFrame(effBTLCut_t *work) {
    return work->currentFrame;
}

void *effBTLCutGetFormation(effBTLCut_t *cut) {
    return cut->formation;
}

void effBTLCutSetMotionChangeCallbackFunc(effBTLCut_t *cut, effBTLCallback_t callback) {
    cut->sourceFrameCallback = callback;
}

/* Store the callback used when the current frame reaches triggerFrame. */
void effBTLCutSetCutEndCallbackFunc(effBTLCut_t *work, effBTLCallback_t callback) {
    work->triggerFrameCallback = callback;
}

/* Read the callback frame from the linked source. */
s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *work) {
    return work->list->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBTLCutSetCutEndFrame(effBTLCut_t *work, s32 triggerFrame) {
    work->triggerFrame = triggerFrame;
    if (work->kind == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        work->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBTLCutGetCutEndFrame(effBTLCut_t *work) {
    return work->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBTLCutSetDrawEndFrame(effBTLCut_t *work, u32 endFrame) {
    if (work->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (endFrame < work->colorRampEndFrame) {
            work->colorRampEndFrame = endFrame;
        }
        return;
    }
    work->colorRampEndFrame = endFrame;
}

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBTLCutGetDrawEndFrame(effBTLCut_t *work) {
    return work->colorRampEndFrame;
}

void effBTLCutSetUserData(effBTLCut_t *cut, void *userData) {
    cut->userData = userData;
}

void *effBTLCutGetUserData(effBTLCut_t *cut) {
    return cut->userData;
}

typedef struct effBTLColorWork_t {
    u32 word;
    u8 pad04[12];
} effBTLColorWork_t;

u32 effBTLCutGetAllPartColor(u32 elapsed, u32 duration) {
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

    if (duration == 0 || elapsed >= duration) {
        return 0x808080;
    }
    if (duration < 6) {
        ratio = (f32)(duration - elapsed) / (f32)duration;
    } else {
        ratio = 1.0f;
        if (duration - 6 < elapsed) {
            ratio = (f32)(duration - elapsed) / 6.0f;
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

void effBTLCutSetCameraState(effBTLCut_t *cut) {
    s32 dummy;
    s32 *block = &dummy;

    if ((nbGetMainProcessData()->stat & 0x1000) == 0) {
        if (cut->kind < 3) {
            nbCameraSetState(D_0031B8C0[cut->kind]);
        }
    }
}

/* NON_MATCHING: 88 of 263 words differ (first at +0x34). */
#ifdef NON_MATCHING
void effBTLCutDraw(effBTLCut_t *cut) {
    sdfVector4_t position;
    sdfMatrix4_t base;
    sdfMatrix4_t rotA;
    sdfMatrix4_t rotB;
    sdfMatrix4_t direction;
    sdfMatrix4_t marked;
    effBTLCutData_t *list;
    effBTLPart_t *entry;
    effBTLAimParams_t *aim;
    nbFormation_t *formation;
    u32 frame;
    u32 color;
    s32 count;
    s32 flags;
    s32 i;
    u8 kind;

    frame = cut->currentFrame;
    list = cut->list;
    formation = cut->formation;
    if (frame == 0) {
        effBTLCutSetCameraState(cut);
        effBTLPointLightSet(list->colorKeys, formation);
        effBTLModelColorSet(list->rgbKeys, formation);
    }
    if (list->shake != 0 && list->shakeFrame == frame) {
        if (list->shake < 0x1F) {
            dds3SetQuake(list->shakeFade, list->shake * 10);
        } else {
            printf(D_00532ED0, list->shake);
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
    color = effBTLCutGetAllPartColor(frame, cut->colorRampEndFrame);
    count = list->count;
    flags = cut->unk18;
    for (i = 0; i < count; i++, entry++) {
        if (entry->zeroKindCount >= frame) {
            continue;
        }
        kind = entry->kind;
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
        if (cut->parameterWorks[i] == NULL) {
            continue;
        }
        aim = &entry->aimParams;
        effBTLPartGetPositionPtr(formation, aim, &position);
        dds3EffectPartSetPosition(cut->parameterWorks[i], &position);
        switch (aim->scaleKind) {
        case 0:
        case 4:
            dds3EffectPartSetRotateMatrix(cut->parameterWorks[i], &base);
            break;
        case 1:
            dds3EffectPartSetRotateMatrix(cut->parameterWorks[i], &rotA);
            break;
        case 2:
            dds3EffectPartSetRotateMatrix(cut->parameterWorks[i], &rotB);
            break;
        case 3:
            break;
        case 5:
            effBTLModelGetMarkMatrix2(formation, entry->aimParams.aimMode);
            SDF_VU0_LOADMATRIX_B(&base);
            MultMatrixNoArg();
            SDF_VU0_STOREMATRIX(&marked);
            dds3EffectPartSetRotateMatrix(cut->parameterWorks[i], &marked);
            break;
        }
        effBTLPartGetRotateMatrixPtr(aim, &direction);
        dds3EffectPartSetDirection(cut->parameterWorks[i], &direction);
        dds3EffectPartSetArea(cut->parameterWorks[i], effBTLPartGetRadiusPtr(formation, aim));
        dds3EffectPartSetColor(cut->parameterWorks[i], color);
        dds3EffectPartDraw(cut->parameterWorks[i]);
    }
    if (list->callbackFrame == frame && cut->sourceFrameCallback != NULL) {
        cut->sourceFrameCallback(cut);
    }
    if (cut->triggerFrame == frame && cut->triggerFrameCallback != NULL) {
        cut->triggerFrameCallback(cut);
    }
    effBTLCutSetMax(cut->kind, cut->currentFrame);
    cut->currentFrame++;
}
#else
INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EA0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EB0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EC0);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutDraw);
#endif

void effBTLCutSetScale(effBTLCut_t *effect, f32 scale) {
    s32 i;
    s32 count;

    if (scale != 1.0f) {
        count = effect->list->count;
        for (i = 0; i < count; i++) {
            dds3EffectPartSetScale(effect->parameterWorks[i], scale);
        }
    }
}

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(effBTL_t *destination, effBTL_t *source) {
    u8 *sourceBytes = (u8 *)source;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        effBTLPart_t *entry = destination->banks[bankIndex].list.entries;
        u16 entryCountForBank = destination->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->paramKind != 0xFFFE) {
                    if (entry->paramKind == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 sourceBank = index / 0x3C;
                        u32 sourceEntryIndex = index % 0x3C;
                        effBTLPart_t *sourceEntry = &destination->banks[sourceBank].list.entries[sourceEntryIndex];
                        dds3EffectPart_t *duplicatedWork = dds3EffectPartCopy(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->paramKind;

                        entry->value.work = duplicatedWork;
                        entry->paramKind = sourceKind;
                    } else {
                        entry->value.work = dds3EffectPartCreate(entry->paramKind, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->paramKind == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
                        entry->zeroKindCount++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
    }
}

/* Release each initialized parameter-work entry in the handle's two banks. */
void effBTLPartDelete(effBTL_t *handle) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = handle->banks[bankIndex].list.count;
        effBTLPart_t *entry = handle->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->paramKind != 0xFFFE) {
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
    printf(D_00532F08);
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
    printf(D_00532F20, stat);
    printf(D_00532F38, id);
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
    printf(D_00532F50);
}

void effBTLSkillSetFormationInfo(s32 formIndex, u32 targetBit) {
    nbFormation_t *form;
    nbFormation_t **target;
    u32 bit;
    s32 count;
    s32 i;

    form = nbGetMainProcessData()->form;
    D_005698E0.targetBit = targetBit;
    D_005698E0.invokeForm = &form[formIndex];
    target = D_005698E0.targetForm;
    bit = 1;
    count = 0;
    for (i = 0; i < 32; i++) {
        if (targetBit & bit) {
            *target++ = form;
            count++;
        }
        bit <<= 1;
        form++;
    }
    D_005698E0.targetNumber = count;
    printf(D_00532F68);
}

nbFormation_t *effBTLSkillGetInvokeFormation(void) {
    nbMainProcessData_t *data;

    if (D_005698E0.invokeForm == NULL) {
        data = nbGetMainProcessData();
        printf(D_00532F90);
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
        printf(D_00532FB8);
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
    printf(D_00532F20, state);
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

/* NON_MATCHING: 13 of 241 words differ (first at +0x80). */
#ifdef NON_MATCHING
void effBTLFieldColorON(u32 id) {
    effBTLColor_t work;
    effBTLColor_t scale;
    u32 color;
    char *format;
    s32 ar;
    s32 ag;
    s32 ab;
    s32 br;
    s32 bg;
    s32 bb;

    D_00551D84 = id;
    if (D_00317750[id].color.a != 0.0f &&
        (effBTLFieldColorIsOn() == 0 || D_00551D84 == 0x200)) {
        effBTLFieldColorGetBaseColor();
        work.r = D_00317750[id].color.r;
        work.g = D_00317750[id].color.g;
        work.b = D_00317750[id].color.b;
        SDF_VU0_LOADVECTOR_B_MEMOPS(work);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(work);
        format = D_00532FE8;
        ar = sdfFloatToULong(work.r);
        ag = sdfFloatToULong(work.g);
        ab = sdfFloatToULong(work.b);
        br = sdfFloatToULong(D_00317750[id].color.r);
        bg = sdfFloatToULong(D_00317750[id].color.g);
        bb = sdfFloatToULong(D_00317750[id].color.b);
        printf(format, ar, ag, ab, br, bg, bb);
        D_00569970[1] = work.r;
        D_00569970[2] = work.g;
        D_00569970[3] = work.b;
        D_00551D88 = D_00317750[id].word;
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
        D_00551D88 = D_0031CA90[0];
        effBTLFieldColorStart();
    }
    effBTLFieldLightAnimFrame = 0;
    effBTLFieldLightAnimFrameEnd = 0;
    effBTLFieldLightAnimRotate = 0.0f;
}
#else
INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F08);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F20);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F38);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F50);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F68);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F90);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532FB8);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorON);
#endif

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

void effBTLFieldLightDirectionSet(const f32 *direction) {
    SDF_VU0_COPYVECTOR(dds3LightParam.sources[0] + 4, direction);
}

void effBTLFieldLightAxisSet(const f32 *axis);

void effBTLFieldLightAxisSet(const f32 *axis) {
    SDF_VU0_COPYVECTOR(effBTLFieldLightAnimAxis, axis);
}

void effBTLFieldLightAnimationSet(s32 frame, f32 angle) {
    effBTLFieldLightAnimFrame = 0;
    effBTLFieldLightAnimFrameEnd = frame;
    effBTLFieldLightAnimRotate = angle / frame;
}

void effBTLSkyFilterON(s32 alphaTarget, u16 frame) {
    s32 frameCount = frame;

    effBTLSkyFilterAlphaBase = fldSkyGetAlpha();
    effBTLSkyFilterAlphaTarget = alphaTarget;
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

void effBTLFieldFilterON(u32 color, u16 frame) {
    if (frame == 0) {
        effBTLFieldColorFilter.rect.color = color;
        effBTLFieldColorFilter.frame = 0;
        return;
    }
    effBTLFieldColorFilter.frame = frame;
    effBTLFieldColorFilter.frameMax = frame;
    effBTLFieldColorFilter.baseColor = effBTLFieldColorFilter.rect.color;
    effBTLFieldColorFilter.targetColor = color;
}

void effBTLFieldFilterOFF(u16 frame) {
    if (frame == 0) {
        effBTLFieldColorFilter.frame = 0;
        effBTLFieldColorFilter.rect.color = 0;
        return;
    }
    effBTLFieldColorFilter.baseColor = effBTLFieldColorFilter.rect.color;
    effBTLFieldColorFilter.targetColor = 0;
    effBTLFieldColorFilter.frame = frame;
    effBTLFieldColorFilter.frameMax = frame;
}

void effBTLFieldFilterRun(void) {
    if (effBTLFieldColorFilter.frame != 0) {
        effBTLFieldColorFilter.rect.color =
            effColorBlend(effBTLFieldColorFilter.targetColor, effBTLFieldColorFilter.baseColor,
                          (f32)effBTLFieldColorFilter.frame / (f32)effBTLFieldColorFilter.frameMax);
        effBTLFieldColorFilter.frame--;
    }
}

void effBTLFieldFilterDraw(void) {
    if (effBTLFieldColorFilter.rect.color & 0xFF000000) {
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
        D_005699B0.diffuseR = D_00569970[1];
        D_005699B0.diffuseG = D_00569970[2];
        D_005699B0.diffuseB = D_00569970[3];
        D_005699B0.ambientR = D_00569970[1];
        D_005699B0.ambientG = D_00569970[2];
        D_005699B0.ambientB = D_00569970[3];
        D_005699B0.insideR = 0.0f;
        D_005699B0.outsideR = 0.0f;
        D_005699B0.soften = 0.0f;
        D_005699B0.bias = 0.0f;
        D_005699B0.resv = 0;
        D_005699B0.flag = 0;
        D_005699B0.animeType = 0;
        dds3SetLightObjectResoruce(data->light[1], &D_005699B0);
        dds3LinkSceneLight(data->light[1], 0xC);
        dds3LightParam.sources[0][4] = -0.73f;
        dds3LightParam.sources[0][5] = 1.55f;
        dds3LightParam.sources[0][6] = 0.24f;
        effBTLSkyFilterON(0x40, 0xC);
        effBTLFieldFilterON(D_00551D88, 0xC);
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
        D_005699B0.diffuseR = D_00569990[0];
        D_005699B0.diffuseG = D_00569990[1];
        D_005699B0.diffuseB = D_00569990[2];
        D_005699B0.ambientR = D_005699A0[0];
        D_005699B0.ambientG = D_005699A0[1];
        D_005699B0.ambientB = D_005699A0[2];
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
    major = data->unk7C - 0xC8;
    minor = data->unk7E;
    if ((u16)major >= 0x28 || minor <= 0 || minor >= 0xB) {
        printf(D_00533028);
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
    nbGetMainProcessData()->effectProcess = NULL;
}

void nbInitEffectProcess(nbMainProcessData_t *data) {
    dds3ProcessID_t *process;
    dds3ProcessID_t *draw;

    process = dds3StartProcess(D_00533058, 0x3F9, 2, 1, nbEffectProcess, nbEffectShutdown, NULL);
    dds3LinkProcess(data->unkBC, process);
    data->effectProcess = process;
    draw = dds3StartProcess(D_00533068, 0x2B0E, 1, 1, nbEffectDraw, NULL, NULL);
    dds3LinkProcess(process, draw);
    effBTLSkillInit();
    effBTLFieldColorInit();
    effBTLPointLightInit();
    effBTLModelShowInit();
    effBTLSystemPartHandleCreate();
}
INCLUDE_RODATA(const s32, "effect/effBattle", D_00533028);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00533058);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00533068);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkillRun);

INCLUDE_SDATA(const s32, "effect/effBattle", D_0054F0CD);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLFieldColorProcess);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaTarget);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaBase);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrame);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrameMax);

