#include "common.h"
#include "effBattle.h"

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
extern void *effBTLFieldColorProcess;
extern f32 effBTLFieldLightAnimAxis[4];
extern effSolidRectParams_t effBTLFieldColorFilter;
extern void effBTLFieldColorEnd(void);
extern void dds3EffectFilter(effSolidRectParams_t *params);
extern void effBTLFieldColorRun(void);
extern void effBTLPointLightRun(void);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCreate);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLDelete);

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

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutCreate);

/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
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

u32 effBTLCutGetAllPartColor(u32 elapsed, u32 duration);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutGetAllPartColor);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetCameraState);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EA0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EB0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532EC0);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutDraw);

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

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillInit);

INCLUDE_RODATA(const s32, "effect/effBattle", D_00532F20);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillStart);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillEnd);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillSetFormationInfo);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillGetInvokeFormation);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillGetInvokeParty);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillGetTargetFormation);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillGetTargetParty);

u32 effBTLSkillGetTargetNumber(void) {
    return effBTLSkillTargetNumber[0];
}

u32 effBTLSkillGetTargetBit(void) {
    return effBTLSkillTargetBit[0];
}

s32 effBTLSkillCheckRun(void) {
    return effBTLSkillRun;
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkillSetState);

s32 effBTLSkillGetState(void) {
    return effBTLSkillState;
}

s32 effBTLSkillGetID(void) {
    return effBTLSkillID;
}

void effBTLFieldColorInit(void) {
    effBTLFieldColorProcess = NULL;
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorON);

void effBTLFieldColorOFF(void) {
    effBTLFieldColorEnd();
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorStart);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorEnd);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldLightDirectionSet);

void effBTLFieldLightAxisSet(const f32 *axis);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldLightAxisSet);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldLightAnimationSet);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkyFilterON);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkyFilterOFF);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLSkyFilterRun);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldFilterON);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldFilterOFF);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldFilterRun);

void effBTLFieldFilterDraw(void) {
    if (effBTLFieldColorFilter.color & 0xFF000000) {
        dds3EffectFilter(&effBTLFieldColorFilter);
    }
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorRun);

void effBTLFieldColorDraw(void) {
    if (effBTLFieldColorProcess != NULL) {
        effBTLFieldFilterDraw();
    }
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorGetBaseColor);

s32 nbEffectProcess(void) {
    effBTLFieldColorRun();
    effBTLPointLightRun();
    return 0;
}

s32 nbEffectDraw(void) {
    effBTLFieldColorDraw();
    return 0;
}

INCLUDE_ASM(const s32, "effect/effBattle", nbEffectShutdown);

INCLUDE_ASM(const s32, "effect/effBattle", nbInitEffectProcess);
INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkillRun);

INCLUDE_SDATA(const s32, "effect/effBattle", D_0054F0CD);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLFieldColorProcess);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaTarget);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterAlphaBase);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrame);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkyFilterFrameMax);

