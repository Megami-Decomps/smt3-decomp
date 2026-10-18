#include "common.h"
#include "effBattle.h"
#include "ps2asm.h"

/* Formation extent view. The retail accessors read radius/height at +0x50/+0x54
 * and uniform scale at +0x5C; this is not a claim about the full formation size. */
typedef struct effBTLModel_t {
    u8 unk00[0x50];
    f32 radius;
    f32 height;
    u8 unk58[4];
    f32 scale;
} effBTLModel_t;

extern void effBTLModelGetSphereCenter(effBTLModel_t *model);
extern s32 effBTLModelGetMarkPosition(effBTLModel_t *model, s32 mark);
extern s32 effBTLModelGetMarkMatrix(effBTLModel_t *model, s32 mark);
extern void nbSetFormPos(effBTLModel_t *model, const f32 *position);
extern u32 D_00317740[]; /* 0, 5, 15, 30 frames */
extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);
extern u32 effBTLSkillGetInvokeParty(void);
extern u32 effBTLSkillGetTargetParty(void);
extern void effBTLPartGetPartyPositionPtr(u32 party, void *request);
extern f32 aViewTarget[4];
extern f32 aViewEye[4];
extern f32 aViewDown[4];
extern f32 effBTL_Ptr_D[];
extern void LookAtMatrix(const void *directionPoint, const void *basisOrigin, const void *up);
extern void ReverseMatrix(void);
extern void SetRotateMatrixX(f32 angle);
extern void SetRotateMatrixY_B(f32 angle);
extern void MultMatrixRevNoArg(void);


INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetSphereCenter);

/* Return the larger unscaled extent multiplied by the unit scale. */
f32 effBTLModelGetRadius(effBTLModel_t *unit) {
    f32 radius;
    f32 height;

    radius = unit->radius;
    height = unit->height;
    if (height < radius) {
        return radius * unit->scale;
    }
    return height * unit->scale;
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetDirection);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetDirection2);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetCircumference);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetCircumferencePtr);

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 effBTLModelGetHeight(effBTLModel_t *unit) {
    f32 position[4];
    effBTLModelGetSphereCenter(unit);
    SDF_VU0_STOREVECTOR(position);
    return unit->height * unit->scale * 0.5f - position[1];
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_0016C898);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetMarkPosition);

/* Fall back to the bounding-sphere center when the model has no such mark. */
void effBTLModelGetMarkPosition2(effBTLModel_t *model, s32 mark) {
    s32 found = effBTLModelGetMarkPosition(model, mark);

    if (found == 0) {
        effBTLModelGetSphereCenter(model);
        return;
    }
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetMarkPosition3);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetMarkPositionEvalTm);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetMarkMatrix);

void effBTLModelGetMarkMatrix2(effBTLModel_t *model, s32 mark) {
    if (effBTLModelGetMarkMatrix(model, mark) != 0) {
        return;
    }
    SDF_VU0_LOADIDENTITY();
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetCenter);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetSphereExp);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetLineMaxLength);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetMaxLength);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetSphere);

INCLUDE_ASM(const s32, "effect/effBattleMisc", eff_16D150);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetDirection);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelLook);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelLook2);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupLook);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetGroupNumber);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetGroupMaxHeight);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrModelGroupGetCenterAndHeight);

void effBTLModelSetPosition(effBTLModel_t *model, const f32 *position) {
    nbSetFormPos(model, position);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLGetGroupPos);

INCLUDE_ASM(const s32, "effect/effBattleMisc", nbGetUnitModelPos);

void effBTLModelSetRotate(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetRotate);

void effBTLModelAllPushPosition(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelAllPopPosition);

void effBTLModelAllPushRotate(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelAllPopRotate);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupPopPosition);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupRotate);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupDiameterDisposition);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelShowInit);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelShow);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelHide);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelShowRun);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelAllShow);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelAllHide);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelInvokeShow);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelTargetPartyShow);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelInvokePartyHide);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelTargetPartyHide);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetInvokeGroupNumber);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetTargetGroupNumber);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetID);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelCheckBossID);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGetBadStatus);

/* Native point-light/color callers use this four-entry table as frame durations. */
u32 effBTLPartGetScalePtrSub(s32 index) {
    return D_00317740[index];
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartCalcScale);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetScalePtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrGetAllModelCenterOffset);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrResetAllModelCenterOffset);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrGetModelCenter);

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(void *owner, effBTLAimParams_t *request, f32 *out) {
    GetPositionPtrTable[request->scaleKind](owner, request);
    SDF_VU0_STOREVECTOR(out);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetModelPositionPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetPartyPositionPtr);

void effBTLPartGetInvokePartyPositionPtr(u32 unused, void *request) {
    effBTLPartGetPartyPositionPtr(effBTLSkillGetInvokeParty(), request);
}

void effBTLPartGetTargetPartyPositionPtr(u32 unused, void *request) {
    effBTLPartGetPartyPositionPtr(effBTLSkillGetTargetParty(), request);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetWorldPositionPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetModelMarkPositionPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetRadiusPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetPointLightPtr);

/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *request, sdfMatrix4_t *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (request->aimMode == 4 || request->directionMode == 9) {
        LookAtMatrix(aViewTarget, aViewEye, aViewDown);
        ReverseMatrix();
        VU0_COPY_VF0_TO_VF31();
    } else {
        if (request->aimMode == 5 || request->aimMode == 3 || request->directionMode == 8 || request->directionMode == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (request->directionMode < 9) {
            angle = effBTL_Ptr_D[request->directionMode];
        }
        SetRotateMatrixX(tilt);
        SetRotateMatrixY_B(angle);
        MultMatrixRevNoArg();
    }
    SDF_VU0_STOREMATRIX(out);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartDebugPositionPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelCheckBoundsHide);
