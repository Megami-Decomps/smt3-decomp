#include "common.h"
#include "effBattle.h"
#include "nb.h"
#include "event.h"
#include "model.h"
#include "draw3d.h"
#include "datCalc.h"
#include "ps2asm.h"

extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);
extern void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot);
extern u32 D_00317740[]; /* 0, 5, 15, 30 frames */
extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);
extern u32 effBTLSkillGetInvokeParty(void);
extern u32 effBTLSkillGetTargetParty(void);
extern void effBTLPartGetPartyPositionPtr(u32 group, void *pPosPtr);
extern f32 aViewTarget[4];
extern f32 aViewEye[4];
extern f32 aViewDown[4];
extern f32 effBTL_Ptr_D[];
extern u8 effBTLModelShowFlag[];
extern f32 D_0031B900[];
extern f32 D_0031B910[];
extern f32 D_0031B920[];
extern f32 D_0031B930[];
extern f32 D_0031B940[];
extern f32 D_0031B960[];
extern f32 D_0031B970[];
extern f32 D_0031B980[];
extern sdfVector4_t effBTLPartCenterOffset[32];
extern nbMainProcessData_t *nbGetMainProcessData(void);
extern s32 effBTLSkillCheckRun(void);
extern s32 effBTLSkillGetID(void);
extern void ToMatrix(void);
extern void InversMatrix(void);
extern f32 effBTLModelGroupGetSphereExp(sdfVector4_t *group, u32 model);
extern void nbLoadUnitPos(nbFormation_t *form);
extern void effBTLModelLoadRotate(nbFormation_t *form);
extern void mdlGetPosition(mdlObj_t *aHandle);
extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern void effDebugPointDraw(sdfVector4_t *pPos);
extern void *memset(void *a, s32 b, u32 c);
extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);
extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
extern void ReverseMatrix(void);
extern void MultMatrixRev(sdfMatrix4_t *aMatrix);
extern sdf3DScreenParam_t dds3ScreenParam;
extern s32 sdfModelCheckBounds(f32 *aMinMax);
extern void SetRotateMatrixX(f32 aAngle);
extern void SetRotateMatrixY_B(f32 aAngle);
extern void MultMatrixRevNoArg(void);
extern f32 effBTLModelGroupGetSphere(u32 group);
extern f32 effLinePointLength(sdfVector4_t *pDir, sdfVector4_t *pLinePos, sdfVector4_t *pPos);
extern void SetRotateMatrixY(f32 aAngle);
extern f32 sdfSin(f32 aRadius);
extern f32 sdfCos(f32 aRadius);
extern f32 fabsf(f32 value);
extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);
extern f32 sdfAtan2(f32 aY, f32 aX);
extern void nbFormDispOn2(nbFormation_t *form);
extern void nbFormDispOff2(nbFormation_t *form);
extern void effBTLModelGetCircumference(nbFormation_t *pFormation, sdfVector4_t *pDirection);


void effBTLModelGetSphereCenter(nbFormation_t *pFormation) {
    SDF_VU0_LOADVECTOR(&pFormation->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&pFormation->center);
    SDF_VU0_SCALEVECTOR(pFormation->scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(&pFormation->pos);
    SDF_VU0_ADDVECTOR();
}

/* Return the larger unscaled extent multiplied by the unit scale. */
f32 effBTLModelGetRadius(nbFormation_t *pFormation) {
    f32 radius;
    f32 height;

    radius = pFormation->r;
    height = pFormation->h;
    if (height < radius) {
        return radius * pFormation->scl;
    }
    return height * pFormation->scl;
}

void effBTLModelGetDirection(nbFormation_t *pFormation, sdfVector4_t *pDirection) {
    u32 stat = pFormation->stat;

    if (stat & 0x100) {
        SDF_VU0_COPYVECTOR(pDirection, D_0031B900);
    } else if (stat & 0x200) {
        SDF_VU0_COPYVECTOR(pDirection, D_0031B910);
    }
}

void effBTLModelGetDirection2(nbFormation_t *pFormation) {
    SDF_VU0_LOADVECTOR(&pFormation->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(D_0031B920);
    SDF_VU0_APPLYMATRIX_XYZ();
}

/* vu0 routine: the point on the formation sphere in the given direction. */
void effBTLModelGetCircumference(nbFormation_t *pFormation, sdfVector4_t *pDirection) {
    sdfVector4_t radius;

    SDF_VU0_LOADVECTOR(&pFormation->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(pDirection);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&pFormation->center);
    SDF_VU0_SCALEVECTOR(pFormation->scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(&pFormation->pos);
    SDF_VU0_ADDVECTOR();
    radius.x = pFormation->r;
    radius.y = pFormation->r;
    radius.z = pFormation->r;
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B_MEMOPS(radius);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
}

void effBTLModelGetCircumferencePtr(nbFormation_t *pFormation, u16 ptr_y, u16 ptr_x) {
    sdfVector4_t dir;

    SetRotateMatrixX(D_0031B940[ptr_x]);
    SetRotateMatrixY_B(D_0031B940[ptr_y]);
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR(D_0031B930);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_MEMOPS(dir);
    effBTLModelGetCircumference(pFormation, &dir);
}

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 effBTLModelGetHeight(nbFormation_t *pFormation) {
    f32 position[4];
    effBTLModelGetSphereCenter(pFormation);
    SDF_VU0_STOREVECTOR(position);
    return pFormation->h * pFormation->scl * 0.5f - position[1];
}

f32 func_0016C898(nbFormation_t *unit) {
    return unit->r * unit->scl;
}

s32 effBTLModelGetMarkPosition(nbFormation_t *pFormation, s32 mark) {
    if ((pFormation->stat & 3) == 3 && pFormation->hunit != NULL) {
        return sdfModelGetMarkPos(pFormation->hunit->owner->inner, mark);
    }
    return 0;
}

/* Fall back to the bounding-sphere center when the model has no such mark. */
void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark) {
    s32 found = effBTLModelGetMarkPosition(pFormation, mark);

    if (found == 0) {
        effBTLModelGetSphereCenter(pFormation);
        return;
    }
}

void effBTLModelGetMarkPosition3(mdlObj_t *hModel, s32 mark) {
    if (hModel == NULL) {
        SDF_VU0_LOADVECTOR(D_0031B960);
        return;
    }
    if (sdfModelGetMarkPos(hModel->inner, mark) == 0) {
        mdlGetPosition(hModel);
    }
}

s32 effBTLModelGetMarkPositionEvalTm(nbFormation_t *pFormation, s32 mark) {
    sdfModelRootNode_t *root;

    if ((pFormation->stat & 3) == 3 && pFormation->hunit != NULL) {
        root = pFormation->hunit->owner->inner;
        sdfModelEvalTm(root);
        return sdfModelGetMarkPos(root, mark);
    }
    return 0;
}

s32 effBTLModelGetMarkMatrix(nbFormation_t *pFormation, s32 mark) {
    if ((pFormation->stat & 3) == 3 && pFormation->hunit != NULL) {
        return sdfModelGetMarkPosMatrix(pFormation->hunit->owner->inner, mark);
    }
    return 0;
}

void effBTLModelGetMarkMatrix2(nbFormation_t *pFormation, s32 mark) {
    if (effBTLModelGetMarkMatrix(pFormation, mark) != 0) {
        return;
    }
    SDF_VU0_LOADIDENTITY();
}

void effBTLModelGroupGetCenter(u32 group) {
    sdfVector4_t sum;
    sdfVector4_t scale;
    nbFormation_t *form;
    u32 count;
    u32 stat;
    s32 i;
    f32 zero;

    count = 0;
    form = nbGetMainProcessData()->form;
    zero = 0.0f;
    sum.z = zero;
    sum.y = zero;
    sum.x = zero;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLModelGetSphereCenter(form);
            SDF_VU0_LOADVECTOR_B_MEMOPS(sum);
            SDF_VU0_ADDVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_MEMOPS(sum);
            count++;
        }
        form++;
    }
    if (count != 0) {
        scale.x = 1.0f / count;
    } else {
        scale.x = 1.0f;
    }
    scale.y = scale.x;
    scale.z = scale.x;
    SDF_VU0_LOADVECTOR_MEMOPS(sum);
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
}

f32 effBTLModelGroupGetSphereExp(sdfVector4_t *group, u32 model) {
    nbFormation_t *form;
    f32 max;
    f32 length;
    s32 count;
    u32 stat;
    s32 i;

    count = 0;
    max = 0.0f;
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & model) != 0 && (stat & 3) == 3) {
            effBTLModelGetSphereCenter(form);
            SDF_VU0_LOADVECTOR_B(group);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(length);
            count++;
            if (form->h * 0.5f < form->r) {
                length += form->r * form->scl;
            } else {
                length += form->h * form->scl * 0.5f;
            }
            if (max < length) {
                max = length;
            }
        }
        form++;
    }
    if (count == 1) {
        max = max * 1.33f;
    }
    return max;
}

f32 effBTLModelGroupGetLineMaxLength(sdfVector4_t *pVec, sdfVector4_t *pPos, u32 group) {
    sdfVector4_t center;
    nbFormation_t *form;
    f32 max;
    f32 length;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    max = 0.0f;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLModelGetSphereCenter(form);
            SDF_VU0_STOREVECTOR_MEMOPS(center);
            length = effLinePointLength(pVec, pPos, &center) + form->r * form->scl;
            if (max < length) {
                max = length;
            }
        }
        form++;
    }
    return max;
}

f32 effBTLModelGroupGetMaxLength(u32 group) {
    sdfVector4_t pos;
    nbFormation_t *form;
    f32 max;
    f32 min;
    f32 far;
    f32 near;
    f32 radius;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    max = 0.0f;
    min = max;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLGetGroupPos(form, &pos);
            radius = form->r * form->scl;
            far = pos.x + radius;
            if (max < far) {
                max = far;
            }
            if (far < min) {
                min = far;
            }
            near = pos.x - radius;
            if (max < near) {
                max = near;
            }
            if (near < min) {
                min = near;
            }
        }
        form++;
    }
    return max - min;
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelGroupGetSphere);

INCLUDE_ASM(const s32, "effect/effBattleMisc", eff_16D150);

void effBTLModelGroupGetDirection(u32 group, sdfVector4_t *pDirection) {
    if (group & 0x100) {
        SDF_VU0_COPYVECTOR(pDirection, D_0031B970);
    } else if (group & 0x200) {
        SDF_VU0_COPYVECTOR(pDirection, D_0031B980);
    }
}

void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos) {
    sdfVector4_t direction;
    sdfVector4_t center;
    sdfVector4_t rotate;

    effBTLModelGetSphereCenter(pFormation);
    SDF_VU0_STOREVECTOR(&center);
    direction.x = pTargetPos->x - center.x;
    direction.z = pTargetPos->z - center.z;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        return;
    }
    FromEuler(0.0f, sdfAtan2(direction.x, direction.z), 0.0f);
    SDF_VU0_STOREVECTOR_NOMEM(&rotate);
    nbSetFormRot(pFormation, &rotate);
}

/* NON_MATCHING: 2 of 68 words differ (first at +0x98). */
#ifdef NON_MATCHING
void effBTLModelLook2(nbFormation_t *pFormation, sdfVector4_t *pTargetPos, f32 angle) {
    sdfVector4_t direction;
    sdfVector4_t center;
    sdfVector4_t rotate;
    f32 x;

    effBTLModelGetSphereCenter(pFormation);
    SDF_VU0_STOREVECTOR(&center);
    direction.x = pTargetPos->x - center.x;
    direction.z = pTargetPos->z - center.z;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        return;
    }
    x = (sdfSin(angle) / sdfCos(angle)) * direction.z;
    if (fabsf(direction.x) < fabsf(x)) {
        x = direction.x;
    } else if (0.0f <= direction.x) {
        x = fabsf(x);
    } else {
        x = -fabsf(x);
    }
    FromEuler(0.0f, sdfAtan2(x, direction.z), 0.0f);
    SDF_VU0_STOREVECTOR_NOMEM(&rotate);
    nbSetFormRot(pFormation, &rotate);
}
#else
INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelLook2);
#endif

void effBTLModelGroupLook(u32 group, sdfVector4_t *pPos) {
    nbFormation_t *form;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLModelLook(form, pPos);
        }
        form++;
    }
}

u32 effBTLModelGetGroupNumber(u32 group) {
    nbFormation_t *form;
    s32 count;
    s32 i;

    count = 0;
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & group) != 0 && (form->stat & 3) == 3) {
            count++;
        }
        form++;
    }
    return count;
}

f32 effBTLModelGetGroupMaxHeight(u32 group) {
    nbFormation_t *form;
    f32 max;
    f32 height;
    s32 first;
    u32 stat;
    s32 i;

    first = 1;
    form = nbGetMainProcessData()->form;
    max = 0.0f;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            height = effBTLModelGetHeight(form);
            if (first) {
                max = height;
                first = 0;
            } else if (max < height) {
                max = height;
            }
        }
        form++;
    }
    return max;
}

/* NON_MATCHING: 4 of 146 words differ (first at +0x70). */
#ifdef NON_MATCHING
void effBTLPartPtrModelGroupGetCenterAndHeight(u32 group, sdfVector4_t *pCenter, f32 *pHeightAve,
                                               f32 *pHeightMax, f32 *pFrontMax) {
    sdfVector4_t pos;
    sdfVector4_t sum;
    sdfVector4_t scale;
    nbFormation_t *form;
    u32 back;
    f32 front;
    f32 total;
    f32 top;
    f32 depth;
    f32 radius;
    f32 average;
    f32 top2;
    f32 zero;
    u32 count;
    u32 stat;
    u32 i;

    count = 0;
    back = group & 0x100;
    front = 0.0f;
    total = front;
    form = nbGetMainProcessData()->form;
    top = -100000.0f;
    if (back == 0) {
        top = 100000.0f;
    }
    zero = 0.0f;
    sum.x = zero;
    sum.y = zero;
    sum.z = zero;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLPartPtrGetModelCenter(form);
            SDF_VU0_LOADVECTOR_B(&sum);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            SDF_VU0_ADDVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B(&sum);
            radius = form->r * form->scl;
            depth = pos.y - radius;
            if (back != 0) {
                top2 = pos.z + radius;
                if (top < top2) {
                    top = top2;
                }
            } else {
                top2 = pos.z - radius;
                if (top2 < top) {
                    top = top2;
                }
            }
            total += depth;
            if (depth < front) {
                front = depth;
            }
            count++;
        }
        form++;
    }
    if (count != 0) {
        average = 1.0f / count;
        scale.z = average;
        scale.y = average;
        scale.x = average;
        SDF_VU0_LOADVECTOR(&sum);
        SDF_VU0_LOADVECTOR_B(&scale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(pCenter);
        *pFrontMax = top;
        *pHeightAve = total / count;
        *pHeightMax = front;
    } else {
        zero = 0.0f;
        pCenter->z = zero;
        pCenter->y = zero;
        pCenter->x = zero;
        *pFrontMax = zero;
        *pHeightAve = zero;
        *pHeightMax = zero;
    }
}
#else
INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrModelGroupGetCenterAndHeight);
#endif

void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos) {
    nbSetFormPos(pFormation, pPos);
}

void effBTLGetGroupPos(nbFormation_t *form, f32 *out) {
    if ((form->stat & 3) == 3) {
        nbLoadUnitPos(form);
        SDF_VU0_STOREVECTOR(out);
    }
}

void nbGetUnitModelPos(nbFormation_t *form, f32 *out) {
    if ((form->stat & 3) == 3) {
        mdlGetPosition(form->hunit->owner);
        SDF_VU0_STOREVECTOR(out);
    }
}

void effBTLModelSetRotate(void) {
}

void effBTLModelGetRotate(nbFormation_t *pFormation, f32 *pRot) {
    if ((pFormation->stat & 3) == 3) {
        effBTLModelLoadRotate(pFormation);
        SDF_VU0_STOREVECTOR(pRot);
    }
}

void effBTLModelAllPushPosition(void) {
}

void effBTLModelAllPopPosition(void) {
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & 3) == 3) {
            effBTLModelSetPosition(form, &form->orgpos);
        }
        form++;
    }
}

void effBTLModelAllPushRotate(void) {
}

void effBTLModelAllPopRotate(void) {
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & 3) == 3) {
            nbSetFormRot(form, &form->orgrot);
        }
        form++;
    }
}

void effBTLModelGroupPopPosition(u32 group) {
    nbFormation_t *form;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            effBTLModelSetPosition(form, &form->orgpos);
        }
        form++;
    }
}

void effBTLModelGroupRotate(u32 group, f32 angle) {
    sdfMatrix4_t rotate;
    sdfVector4_t center;
    sdfVector4_t pos;
    nbFormation_t *form;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    SetRotateMatrixY(angle);
    SDF_VU0_STOREMATRIX(&rotate);
    effBTLModelGroupGetCenter(group);
    SDF_VU0_STOREVECTOR(&center);
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLGetGroupPos(form, &pos);
            center.y = pos.y;
            SDF_VU0_LOADMATRIX(&rotate);
            SDF_VU0_LOADVECTOR(&pos);
            SDF_VU0_LOADVECTOR_B(&center);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            effBTLModelSetPosition(form, &pos);
        }
        form++;
    }
}

void effBTLModelGroupDiameterDisposition(u32 group) {
    sdfVector4_t sphere;
    sdfVector4_t pos;
    nbFormation_t *form;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    effBTLModelGroupGetSphere(group);
    SDF_VU0_STOREVECTOR_MEMOPS(sphere);
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLGetGroupPos(form, &pos);
            pos.z = sphere.z;
            effBTLModelSetPosition(form, &pos);
        }
        form++;
    }
}

void effBTLModelShowInit(void) {
    memset(effBTLModelShowFlag, 0xFF, 32);
}

void effBTLModelShow(nbFormation_t *pFormation) {
    effBTLModelShowFlag[pFormation->formindex] = 1;
}

void effBTLModelHide(nbFormation_t *pFormation) {
    effBTLModelShowFlag[pFormation->formindex] = 0;
}

void effBTLModelShowRun(void) {
    nbFormation_t *form;
    u32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if (effBTLModelShowFlag[form->formindex] == 1) {
            nbFormDispOn2(form);
        } else if (effBTLModelShowFlag[form->formindex] == 0) {
            nbFormDispOff2(form);
        }
        effBTLModelShowFlag[form->formindex] = -1;
        form++;
    }
}

void effBTLModelAllShow(void) {
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & 3) == 3) {
            effBTLModelShow(form);
        }
        form++;
    }
}

void effBTLModelAllHide(void) {
    nbFormation_t *form;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & 3) == 3) {
            effBTLModelHide(form);
        }
        form++;
    }
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelInvokeShow);

void effBTLModelTargetPartyShow(void) {
    nbFormation_t *form;
    u32 party;
    u32 stat;
    s32 i;

    party = effBTLSkillGetTargetParty();
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & party) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            effBTLModelShow(form);
        }
        form++;
    }
}

void effBTLModelInvokePartyHide(void) {
    nbFormation_t *form;
    u32 party;
    u32 stat;
    s32 i;

    party = effBTLSkillGetInvokeParty();
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & party) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            effBTLModelHide(form);
        }
        form++;
    }
}

void effBTLModelTargetPartyHide(void) {
    nbFormation_t *form;
    u32 party;
    u32 stat;
    s32 i;

    party = effBTLSkillGetTargetParty();
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & party) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            effBTLModelHide(form);
        }
        form++;
    }
}

u32 effBTLModelGetInvokeGroupNumber(void) {
    nbFormation_t *form;
    u32 party;
    u32 stat;
    s32 count;
    s32 i;

    party = effBTLSkillGetInvokeParty();
    count = 0;
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & party) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            count++;
        }
        form++;
    }
    return count;
}

u32 effBTLModelGetTargetGroupNumber(void) {
    nbFormation_t *form;
    u32 party;
    u32 stat;
    s32 count;
    s32 i;

    party = effBTLSkillGetTargetParty();
    count = 0;
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & party) != 0 && (stat & 1) != 0 && (stat & 2) != 0) {
            count++;
        }
        form++;
    }
    return count;
}

s32 effBTLModelGetID(nbFormation_t *pFormation) {
    datUnitWork_t *work;

    if (pFormation == NULL) {
        return 0x200;
    }
    if ((pFormation->stat & 1) == 0) {
        return 0x200;
    }
    work = nbGetUnitWorkFromFormindex(pFormation->formindex);
    if (work->flag & 4) {
        return 0;
    }
    return work->id;
}

s32 effBTLModelCheckBossID(nbFormation_t *pFormation) {
    datUnitWork_t *work;

    if (pFormation == NULL) {
        return 0;
    }
    if ((pFormation->stat & 1) == 0) {
        return 0;
    }
    work = nbGetUnitWorkFromFormindex(pFormation->formindex);
    if (work == NULL) {
        return 0;
    }
    return work->id >= 0x100;
}

u16 effBTLModelGetBadStatus(nbFormation_t *pFormation) {
    return nbGetUnitWorkFromFormindex(pFormation->formindex)->badstatus;
}

/* Native point-light/color callers use this four-entry table as frame durations. */
u32 effBTLPartGetScalePtrSub(s32 pFormation) {
    return D_00317740[pFormation];
}

f32 effBTLPartCalcScale(nbFormation_t *form) {
    f32 scale = (form->r + form->h * 0.5f) * 0.5f * form->scl * 0.0142857143f;

    if (2.0f < scale) {
        scale = 2.0f;
    } else if (scale < 0.8f) {
        scale = 0.8f;
    }
    return scale;
}

f32 effBTLPartGetScalePtr(nbFormation_t *form, effBTLAimParams_t *request) {
    switch (effBTLModelGetID(form)) {
    case 0x112:
        return 2.7f;
    case 0x102:
    case 0x103:
    case 0x108:
    case 0x123:
    case 0x124:
    case 0x125:
    case 0x141:
    case 0x158:
        return 3.0f;
    case 0x104:
    default:
        return effBTLPartCalcScale(form);
    }
}

void effBTLPartPtrGetAllModelCenterOffset(void) {
    sdfVector4_t center;
    sdfVector4_t offset;
    sdfVector4_t zero;
    nbFormation_t *form;
    sdfVector4_t *out;
    datUnitWork_t *work;
    s32 i;

    memset(&zero, 0, 16);
    form = nbGetMainProcessData()->form;
    out = effBTLPartCenterOffset;
    for (i = 0; i < 32; i++) {
        work = nbGetUnitWorkFromFormindex(form->formindex);
        if (work != NULL) {
            if (work->badstatus & 0x413) {
                effBTLModelGetSphereCenter(form);
                SDF_VU0_STOREVECTOR_MEMOPS(center);
                effBTLModelGetMarkPosition2(form, 0);
                SDF_VU0_LOADVECTOR_B_MEMOPS(center);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&offset);
                SDF_VU0_LOADVECTOR(&form->rot);
                ToMatrix();
                InversMatrix();
                SDF_VU0_LOADVECTOR(&offset);
                SDF_VU0_APPLYMATRIX();
            } else {
                SDF_VU0_LOADVECTOR(&zero);
            }
            SDF_VU0_STOREVECTOR(out);
        }
        out++;
        form++;
    }
}

void effBTLPartPtrResetAllModelCenterOffset(void) {
    sdfVector4_t zero;
    sdfVector4_t *p;
    s32 i;

    memset(&zero, 0, 16);
    nbGetMainProcessData();
    SDF_VU0_LOADVECTOR(&zero);
    p = effBTLPartCenterOffset;
    for (i = 0; i < 32; i++) {
        SDF_VU0_STOREVECTOR(p);
        p++;
    }
}

void effBTLPartPtrGetModelCenter(nbFormation_t *pFormation) {
    sdfVector4_t center;

    effBTLModelGetSphereCenter(pFormation);
    SDF_VU0_STOREVECTOR_MEMOPS(center);
    SDF_VU0_LOADVECTOR(&pFormation->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR_B(&effBTLPartCenterOffset[pFormation->formindex]);
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR_MEMOPS(center);
    SDF_VU0_ADDVECTOR();
}

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos) {
    GetPositionPtrTable[pPosPtr->scaleKind](pFormation, pPosPtr);
    SDF_VU0_STOREVECTOR(pPos);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetModelPositionPtr);

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetPartyPositionPtr);

void effBTLPartGetInvokePartyPositionPtr(u32 pFormation, void *pPosPtr) {
    effBTLPartGetPartyPositionPtr(effBTLSkillGetInvokeParty(), pPosPtr);
}

void effBTLPartGetTargetPartyPositionPtr(u32 pFormation, void *pPosPtr) {
    effBTLPartGetPartyPositionPtr(effBTLSkillGetTargetParty(), pPosPtr);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetWorldPositionPtr);

void effBTLPartGetModelMarkPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr) {
    nbMainProcessData_t *data = nbGetMainProcessData();
    s32 modelId;
    s32 skillId;
    s32 modelIndex;
    s32 skillRuns;

    if ((data->stat & 0x1000) == 0 && pPosPtr->aimMode < 2) {
        modelId = effBTLModelGetID(pFormation);
        skillId = effBTLSkillGetID();
        modelIndex = modelId - 0x102;
        if ((u32)modelIndex < 0x40) {
            switch (modelIndex) {
            case 1:
            case 34:
                effBTLModelGetSphereCenter(pFormation);
                return;
            case 0:
            case 33:
                effBTLModelGetSphereCenter(pFormation);
                return;
            case 6:
            case 35:
                skillRuns = effBTLSkillCheckRun();
                if (skillRuns != 0 &&
                    (skillId == 0xFE || skillId == 0xF1 || skillId == 0xDD))
                    break;
                effBTLModelGetSphereCenter(pFormation);
                return;
            case 63:
                effBTLModelGetSphereCenter(pFormation);
                return;
            }
        }
    }
    if (effBTLModelGetMarkPosition(pFormation, pPosPtr->aimMode) == 0) {
        effBTLModelGetSphereCenter(pFormation);
    }
}

f32 effBTLPartGetRadiusPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr) {
    sdfVector4_t center;
    u32 party;
    f32 radius;

    radius = 0.0f;
    switch (pPosPtr->scaleKind) {
    case 0:
        radius = pFormation->r * pFormation->scl;
        break;
    case 1:
        party = effBTLSkillGetInvokeParty();
        effBTLModelGroupGetCenter(party);
        SDF_VU0_STOREVECTOR_MEMOPS(center);
        radius = effBTLModelGroupGetSphereExp(&center, party);
        break;
    case 2:
        party = effBTLSkillGetTargetParty();
        effBTLModelGroupGetCenter(party);
        SDF_VU0_STOREVECTOR_MEMOPS(center);
        radius = effBTLModelGroupGetSphereExp(&center, party);
        break;
    case 3:
        effBTLModelGroupGetCenter(0x700);
        SDF_VU0_STOREVECTOR_MEMOPS(center);
        radius = effBTLModelGroupGetSphereExp(&center, 0x700);
        break;
    }
    return radius;
}

void effBTLPartGetPointLightPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pDir) {
    sdfVector4_t position;

    effBTLPartGetPositionPtr(pFormation, pPosPtr, &position);
    effBTLModelGetSphereCenter(pFormation);
    SDF_VU0_LOADVECTOR_B_MEMOPS(position);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(pDir);
}

/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix) {
    f32 angle = 0;
    f32 tilt = 0;

    if (pPosPtr->aimMode == 4 || pPosPtr->directionMode == 9) {
        LookAtMatrix(aViewTarget, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
    } else {
        if (pPosPtr->aimMode == 5 || pPosPtr->aimMode == 3 || pPosPtr->directionMode == 8 || pPosPtr->directionMode == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (pPosPtr->directionMode < 9) {
            angle = effBTL_Ptr_D[pPosPtr->directionMode];
        }
        SetRotateMatrixX(tilt);
        SetRotateMatrixY_B(angle);
        MultMatrixRevNoArg();
    }
    SDF_VU0_STOREMATRIX(pMatrix);
}

void effBTLPartDebugPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr) {
    sdfVector4_t position;

    effBTLPartGetPositionPtr(pFormation, pPosPtr, &position);
    effDebugPointDraw(&position);
}

void effBTLModelCheckBoundsHide(nbFormation_t *pFormation, sdfVector4_t *w, sdfVector4_t *h,
                                sdfVector4_t *pTarget, f32 pEye, f32 pUp) {
    sdfMatrix4_t matrix;
    sdfVector4_t row;
    f32 bounds[6];
    u32 stat;

    stat = pFormation->stat;
    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 2) == 0) {
        return;
    }
    if (pFormation->hunit == NULL) {
        return;
    }
    if (pFormation->hunit->owner == NULL) {
        return;
    }
    LookAtMatrix(w, h, pTarget);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_STOREMATRIX(&matrix);
    SDF_VU0_LOADVECTOR(&pFormation->pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&row);
    SDF_VU0_LOADVECTOR(&pFormation->rot);
    ToMatrix();
    SDF_VU0_LOAD_ROW3(&row);
    MultMatrixRev(&matrix);
    bounds[0] = -pEye;
    bounds[1] = -pUp;
    bounds[2] = -pEye;
    bounds[3] = pEye;
    bounds[4] = 0;
    bounds[5] = pEye;
    if (sdfModelCheckBounds(bounds) != 0) {
        effBTLModelHide(pFormation);
    }
}
