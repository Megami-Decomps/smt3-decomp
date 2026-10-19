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
extern void effBTLPartGetPartyPositionPtr(u32 party, void *request);
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
extern void ToMatrix(void);
extern void InversMatrix(void);
extern f32 effBTLModelGroupGetSphereExp(sdfVector4_t *center, u32 group);
extern void nbLoadUnitPos(nbFormation_t *form);
extern void effBTLModelLoadRotate(nbFormation_t *form);
extern void mdlGetPosition(mdlObj_t *obj);
extern void sdfModelEvalTm(sdfModelRootNode_t *root);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *root, s32 mark);
extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);
extern void effDebugPointDraw(sdfVector4_t *position);
extern void *memset(void *dst, s32 value, u32 size);
extern void effBTLModelLook(nbFormation_t *form, sdfVector4_t *target);
extern void LookAtMatrix(const void *directionPoint, const void *basisOrigin, const void *up);
extern void ReverseMatrix(void);
extern void MultMatrixRev(sdfMatrix4_t *matrix);
extern sdf3DScreenParam_t dds3ScreenParam;
extern s32 sdfModelCheckBounds(f32 *bounds);
extern void SetRotateMatrixX(f32 angle);
extern void SetRotateMatrixY_B(f32 angle);
extern void MultMatrixRevNoArg(void);
extern f32 effBTLModelGroupGetSphere(u32 group);
extern f32 effLinePointLength(sdfVector4_t *dir, sdfVector4_t *linePos, sdfVector4_t *pos);
extern void SetRotateMatrixY(f32 angle);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);
extern f32 fabsf(f32 value);
extern void FromEuler(f32 x, f32 y, f32 z);
extern f32 sdfAtan2(f32 y, f32 x);
extern void nbFormDispOn2(nbFormation_t *form);
extern void nbFormDispOff2(nbFormation_t *form);
extern void effBTLModelGetCircumference(nbFormation_t *form, sdfVector4_t *dir);


void effBTLModelGetSphereCenter(nbFormation_t *form) {
    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&form->center);
    SDF_VU0_SCALEVECTOR(form->scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(&form->pos);
    SDF_VU0_ADDVECTOR();
}

/* Return the larger unscaled extent multiplied by the unit scale. */
f32 effBTLModelGetRadius(nbFormation_t *unit) {
    f32 radius;
    f32 height;

    radius = unit->r;
    height = unit->h;
    if (height < radius) {
        return radius * unit->scl;
    }
    return height * unit->scl;
}

void effBTLModelGetDirection(nbFormation_t *form, sdfVector4_t *out) {
    u32 stat = form->stat;

    if (stat & 0x100) {
        SDF_VU0_COPYVECTOR(out, D_0031B900);
    } else if (stat & 0x200) {
        SDF_VU0_COPYVECTOR(out, D_0031B910);
    }
}

void effBTLModelGetDirection2(nbFormation_t *form) {
    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(D_0031B920);
    SDF_VU0_APPLYMATRIX_XYZ();
}

/* vu0 routine: the point on the formation sphere in the given direction. */
void effBTLModelGetCircumference(nbFormation_t *form, sdfVector4_t *dir) {
    sdfVector4_t radius;

    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(dir);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&form->center);
    SDF_VU0_SCALEVECTOR(form->scl);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_LOADVECTOR_B(&form->pos);
    SDF_VU0_ADDVECTOR();
    radius.x = form->r;
    radius.y = form->r;
    radius.z = form->r;
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B_MEMOPS(radius);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
}

void effBTLModelGetCircumferencePtr(nbFormation_t *form, u16 direction, u16 tilt) {
    sdfVector4_t dir;

    SetRotateMatrixX(D_0031B940[tilt]);
    SetRotateMatrixY_B(D_0031B940[direction]);
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR(D_0031B930);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR_MEMOPS(dir);
    effBTLModelGetCircumference(form, &dir);
}

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 effBTLModelGetHeight(nbFormation_t *unit) {
    f32 position[4];
    effBTLModelGetSphereCenter(unit);
    SDF_VU0_STOREVECTOR(position);
    return unit->h * unit->scl * 0.5f - position[1];
}

f32 func_0016C898(nbFormation_t *unit) {
    return unit->r * unit->scl;
}

s32 effBTLModelGetMarkPosition(nbFormation_t *form, s32 mark) {
    if ((form->stat & 3) == 3 && form->hunit != NULL) {
        return sdfModelGetMarkPos(form->hunit->owner->inner, mark);
    }
    return 0;
}

/* Fall back to the bounding-sphere center when the model has no such mark. */
void effBTLModelGetMarkPosition2(nbFormation_t *model, s32 mark) {
    s32 found = effBTLModelGetMarkPosition(model, mark);

    if (found == 0) {
        effBTLModelGetSphereCenter(model);
        return;
    }
}

void effBTLModelGetMarkPosition3(mdlObj_t *obj, s32 mark) {
    if (obj == NULL) {
        SDF_VU0_LOADVECTOR(D_0031B960);
        return;
    }
    if (sdfModelGetMarkPos(obj->inner, mark) == 0) {
        mdlGetPosition(obj);
    }
}

s32 effBTLModelGetMarkPositionEvalTm(nbFormation_t *form, s32 mark) {
    sdfModelRootNode_t *root;

    if ((form->stat & 3) == 3 && form->hunit != NULL) {
        root = form->hunit->owner->inner;
        sdfModelEvalTm(root);
        return sdfModelGetMarkPos(root, mark);
    }
    return 0;
}

s32 effBTLModelGetMarkMatrix(nbFormation_t *form, s32 mark) {
    if ((form->stat & 3) == 3 && form->hunit != NULL) {
        return sdfModelGetMarkPosMatrix(form->hunit->owner->inner, mark);
    }
    return 0;
}

void effBTLModelGetMarkMatrix2(nbFormation_t *model, s32 mark) {
    if (effBTLModelGetMarkMatrix(model, mark) != 0) {
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

f32 effBTLModelGroupGetSphereExp(sdfVector4_t *center, u32 group) {
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
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLModelGetSphereCenter(form);
            SDF_VU0_LOADVECTOR_B(center);
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

f32 effBTLModelGroupGetLineMaxLength(sdfVector4_t *dir, sdfVector4_t *linePos, u32 group) {
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
            length = effLinePointLength(dir, linePos, &center) + form->r * form->scl;
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

void effBTLModelGroupGetDirection(u32 group, sdfVector4_t *out) {
    if (group & 0x100) {
        SDF_VU0_COPYVECTOR(out, D_0031B970);
    } else if (group & 0x200) {
        SDF_VU0_COPYVECTOR(out, D_0031B980);
    }
}

void effBTLModelLook(nbFormation_t *form, sdfVector4_t *target) {
    sdfVector4_t direction;
    sdfVector4_t center;
    sdfVector4_t rotate;

    effBTLModelGetSphereCenter(form);
    SDF_VU0_STOREVECTOR(&center);
    direction.x = target->x - center.x;
    direction.z = target->z - center.z;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        return;
    }
    FromEuler(0.0f, sdfAtan2(direction.x, direction.z), 0.0f);
    SDF_VU0_STOREVECTOR_NOMEM(&rotate);
    nbSetFormRot(form, &rotate);
}

/* NON_MATCHING: 2 of 68 words differ (first at +0x98). */
#ifdef NON_MATCHING
void effBTLModelLook2(nbFormation_t *form, sdfVector4_t *target, f32 angle) {
    sdfVector4_t direction;
    sdfVector4_t center;
    sdfVector4_t rotate;
    f32 x;

    effBTLModelGetSphereCenter(form);
    SDF_VU0_STOREVECTOR(&center);
    direction.x = target->x - center.x;
    direction.z = target->z - center.z;
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
    nbSetFormRot(form, &rotate);
}
#else
INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLModelLook2);
#endif

void effBTLModelGroupLook(u32 group, sdfVector4_t *target) {
    nbFormation_t *form;
    u32 stat;
    s32 i;

    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        stat = form->stat;
        if ((stat & group) != 0 && (stat & 3) == 3) {
            effBTLModelLook(form, target);
        }
        form++;
    }
}

u32 effBTLModelGetGroupNumber(u32 mask) {
    nbFormation_t *form;
    s32 count;
    s32 i;

    count = 0;
    form = nbGetMainProcessData()->form;
    for (i = 0; i < 32; i++) {
        if ((form->stat & mask) != 0 && (form->stat & 3) == 3) {
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
void effBTLPartPtrModelGroupGetCenterAndHeight(u32 group, sdfVector4_t *center, f32 *heightAve,
                                               f32 *heightMax, f32 *frontMax) {
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
        SDF_VU0_STOREVECTOR(center);
        *frontMax = top;
        *heightAve = total / count;
        *heightMax = front;
    } else {
        zero = 0.0f;
        center->z = zero;
        center->y = zero;
        center->x = zero;
        *frontMax = zero;
        *heightAve = zero;
        *heightMax = zero;
    }
}
#else
INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartPtrModelGroupGetCenterAndHeight);
#endif

void effBTLModelSetPosition(nbFormation_t *form, sdfVector4_t *pos) {
    nbSetFormPos(form, pos);
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

void effBTLModelGetRotate(nbFormation_t *form, f32 *out) {
    if ((form->stat & 3) == 3) {
        effBTLModelLoadRotate(form);
        SDF_VU0_STOREVECTOR(out);
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

void effBTLModelShow(nbFormation_t *unit) {
    effBTLModelShowFlag[unit->formindex] = 1;
}

void effBTLModelHide(nbFormation_t *unit) {
    effBTLModelShowFlag[unit->formindex] = 0;
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

s32 effBTLModelGetID(nbFormation_t *form) {
    datUnitWork_t *work;

    if (form == NULL) {
        return 0x200;
    }
    if ((form->stat & 1) == 0) {
        return 0x200;
    }
    work = nbGetUnitWorkFromFormindex(form->formindex);
    if (work->flag & 4) {
        return 0;
    }
    return work->id;
}

s32 effBTLModelCheckBossID(nbFormation_t *form) {
    datUnitWork_t *work;

    if (form == NULL) {
        return 0;
    }
    if ((form->stat & 1) == 0) {
        return 0;
    }
    work = nbGetUnitWorkFromFormindex(form->formindex);
    if (work == NULL) {
        return 0;
    }
    return work->id >= 0x100;
}

u16 effBTLModelGetBadStatus(nbFormation_t *unit) {
    return nbGetUnitWorkFromFormindex(unit->formindex)->badstatus;
}

/* Native point-light/color callers use this four-entry table as frame durations. */
u32 effBTLPartGetScalePtrSub(s32 index) {
    return D_00317740[index];
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

void effBTLPartPtrGetModelCenter(nbFormation_t *form) {
    sdfVector4_t center;

    effBTLModelGetSphereCenter(form);
    SDF_VU0_STOREVECTOR_MEMOPS(center);
    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR_B(&effBTLPartCenterOffset[form->formindex]);
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR_MEMOPS(center);
    SDF_VU0_ADDVECTOR();
}

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *form, effBTLAimParams_t *request, sdfVector4_t *out) {
    GetPositionPtrTable[request->scaleKind](form, request);
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

f32 effBTLPartGetRadiusPtr(nbFormation_t *form, effBTLAimParams_t *request) {
    sdfVector4_t center;
    u32 party;
    f32 radius;

    radius = 0.0f;
    switch (request->scaleKind) {
    case 0:
        radius = form->r * form->scl;
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

void effBTLPartGetPointLightPtr(nbFormation_t *form, effBTLAimParams_t *request, sdfVector4_t *out) {
    sdfVector4_t position;

    effBTLPartGetPositionPtr(form, request, &position);
    effBTLModelGetSphereCenter(form);
    SDF_VU0_LOADVECTOR_B_MEMOPS(position);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(out);
}

/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *request, sdfMatrix4_t *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (request->aimMode == 4 || request->directionMode == 9) {
        LookAtMatrix(aViewTarget, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_RESET_ROW3();
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

void effBTLPartDebugPositionPtr(nbFormation_t *form, effBTLAimParams_t *request) {
    sdfVector4_t position;

    effBTLPartGetPositionPtr(form, request, &position);
    effDebugPointDraw(&position);
}

void effBTLModelCheckBoundsHide(nbFormation_t *form, sdfVector4_t *target, sdfVector4_t *eye,
                                sdfVector4_t *up, f32 width, f32 height) {
    sdfMatrix4_t matrix;
    sdfVector4_t row;
    f32 bounds[6];
    u32 stat;

    stat = form->stat;
    if ((stat & 1) == 0) {
        return;
    }
    if ((stat & 2) == 0) {
        return;
    }
    if (form->hunit == NULL) {
        return;
    }
    if (form->hunit->owner == NULL) {
        return;
    }
    LookAtMatrix(target, eye, up);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_STOREMATRIX(&matrix);
    SDF_VU0_LOADVECTOR(&form->pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&row);
    SDF_VU0_LOADVECTOR(&form->rot);
    ToMatrix();
    SDF_VU0_LOAD_ROW3(&row);
    MultMatrixRev(&matrix);
    bounds[0] = -width;
    bounds[1] = -height;
    bounds[2] = -width;
    bounds[3] = width;
    bounds[4] = 0;
    bounds[5] = width;
    if (sdfModelCheckBounds(bounds) != 0) {
        effBTLModelHide(form);
    }
}
