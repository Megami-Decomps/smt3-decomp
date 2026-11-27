#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "fldCamHook.h"
#include "basic.h"
#include "dds3obj.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"



extern s32 printf(const char *format, ...);
extern s32 fldPlayerCalcHasigo(void);

dds3Basic_t *dds3GetActiveScene(void);

extern f32 R360_LR(f32 a, f32 b);

extern f32 fabsf(f32);

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern s32 fldAutoMapChkMode(void);


s32 evtBitCheck(s32 no);

void fldPlayerOff(void);
void fldPlayerSvsOff(void);
void fldPlayerSdwOff(void);

extern f32 fldWmFastMove;

extern s32 gfldPlayerHasiCnt;

extern s32 gfldPlayerDamegeCnt;

extern s32 fldPlayerStepDist;

extern s32 fldWmWarpMode;


extern fldUnitControl_t *fldGetUnitControl(void);

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *pObjectBasic, u32 svs);

extern dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);

extern dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

extern void fldPlayerMotion(s32 hisMotion, s32 upMotion, f32 motHokan);

extern void fldPlayerMotReset2(void);

extern s32 fldPlayerFastMode;

extern s32 l_UseCollision;

extern void fldHitChkUnitElv(f32 *phys_pos);

extern void fldHitChkUnit(f32 *phys_pos);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3SetSclPoseSet(dds3Basic_t *pBasic, f32 *scl);

extern f32 fldWmapPlayerState;

extern const sdfVector4Unaligned_t fldWm3Scale;

extern const sdfVector4Unaligned_t fldWm4Scale;

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, s32 flag);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

f32 R360_LR(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}

f32 fldPlayerCalc360_8(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(45.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(135.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(225.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(R360_LR(315.0f, aKakudo));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 1:
        aKakudo = 45.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 3:
        aKakudo = 135.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 5:
        aKakudo = 225.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    case 7:
        aKakudo = 315.0f;
        break;
    }
    return aKakudo;
}

f32 fldPlayerCalc360_4(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    }
    return aKakudo;
}

s32 fldPlayerMirChk(void) {
    fldUnitControl_t *control = fldGetUnitControl();
    sdfVector4_t pos;

    if (control == NULL) {
        return 0;
    }
    if (control->handle2 == NULL) {
        return 0;
    }
    if (dds3ObjectBaseCheckFlag(control->handle, 1)) {
        dds3ObjectBaseOnFlag(control->handle2, 1);
        return 0;
    }
    if (fldGlobalBase.area == 0x12 || fldGlobalBase.area == 0x1C ||
        fldGlobalBase.area == 0x1E ||
        (fldGlobalBase.area == 0x21 && fldGlobalBase.floor == 0) ||
        (fldGlobalBase.area == 0x11 && fldGlobalBase.floor == 1) ||
        (fldGlobalBase.area == 0x11 && fldGlobalBase.floor == 2) ||
        (fldGlobalBase.area == 0x11 && fldGlobalBase.floor == 3) ||
        (fldGlobalBase.area == 0xE && fldGlobalBase.floor == 0x10) ||
        (fldGlobalBase.area == 0xF &&
            (fldGlobalBase.floor == 0x10 || fldGlobalBase.floor == 9))) {
        dds3GetPosPoseSet(control->handle);
        SDF_VU0_STOREVECTOR(&pos);
        if (pos.y > -2.5f && pos.y < 2.5f) {
            dds3ObjectBaseOffFlag(control->handle2, 1);
        } else {
            dds3ObjectBaseOnFlag(control->handle2, 1);
        }
    }
    return 0;
}


extern s32 fldChkVortex(void);

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber,
                                f32 aBeforeLeng, f32 aSmoothLeng);

void fldPlayerMotion(s32 hisMotion, s32 upMotion, f32 motHokan) {
    fldUnitControl_t *ctl;
    mdlObj_t *model;
    mdlObj_t *model2 = NULL;

    ctl = fldGetUnitControl();
    model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    if (ctl->handle2 != NULL) {
        model2 = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2);
    }
    if (hisMotion == upMotion) {
        return;
    }
    model->first->speed = 1.0f;
    if (fldChkVortex() != 0) {
        if (fldRunDanteGunFlag.word50 & 2) {
            mdlLoopMotion(model, 3, 1);
        } else {
            mdlLoopMotion(model, 3, 0);
        }
    }
    mdlSmoothLoopMotion(model, 0, upMotion, motHokan, motHokan);
    if (ctl->handle2 == NULL) {
        return;
    }
    model2->first->speed = 1.0f;
    mdlSmoothLoopMotion(model2, 0, upMotion, motHokan, motHokan);
}

extern f32 fldCamTarget[];
extern f32 fldCamEye[];
extern void fldSePlay(s32 id);
extern void fldSeVol(s32 id, s32 volume);
extern u32 dds3GetRandA(void *name);

/* Native surface-category branch is retained by the localized operand below. */
/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
void fldPlayerMotionSe(s32 hisMotion, s32 upMotion)
{
    fldUnitControl_t *ctl = fldGetUnitControl();
    mdlObj_t *model = dds3ObjectBaseGetModelHandle(ctl->handle);
    f32 now = model->first->frame;
    f32 before = fldGlobalBase.float12C;
    s32 surface;
    s32 left;
    s32 right;
    s32 volume;
    f32 dx, dy, dz, distance;

    fldGlobalBase.float12C = now;
    if (hisMotion == 13) {
        if (before < 15.0f && now >= 15.0f) {
            fldSePlay(0x27);
        }
    } else if (hisMotion == 11) {
        if (before < 20.0f && now >= 20.0f) {
            fldSePlay(0x25);
        }
    } else {
        if (fldGlobalBase.word44 == 0) {
            /* Target-specific compiler aid on the actual default-category storage.
             * The empty read/write operand stays in the default branch,
             * preventing speculative override-decrement hoisting and recovering
             * its native annulled slot. It emits
             * no memory write and is not a general memory barrier. Removing
             * it changes only bnel to bne; READ_MEM does not reproduce it. */
            FAKEMATCH_ASM_LIVE_MEM(fldGlobalBase.playerSeTable);
            surface = fldGlobalBase.playerSeTable;
        } else {
            surface = fldGlobalBase.word44 - 1;
        }
        switch (surface) {
        case 1: left = 20; right = 20; break;
        case 2: left = 24; right = 24; break;
        case 3: left = 28; right = 28; break;
        default: left = 16; right = 16; break;
        }
        if (fldGlobalBase.mode == 1) {
            dx = fldCamTarget[0] - fldCamEye[0];
            dy = fldCamTarget[1] - fldCamEye[1];
            dz = fldCamTarget[2] - fldCamEye[2];
            distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 500.0f;
            if (distance > 7000.0f) distance = 7000.0f;
            if (distance < 0.0f) distance = 0.0f;
            volume = ((7000 - (s32)distance) * 127) / 7000;
            if (volume > 127) volume = 127;
            if (volume < 0) volume = 0;
        } else {
            volume = 127;
            volume -= (dds3GetRandA(SDF_FRAME_RAND()) >> 2) & 0x3F;
            if (volume > 127) volume = 127;
            if (volume < 0) volume = 0;
        }
        if (hisMotion != 0 && hisMotion != 8 && hisMotion != 9 && upMotion == 8) {
            fldSeVol(left + 3, volume);
        } else {
            left += (dds3GetRandA(SDF_FRAME_RAND()) >> 4) % 3;
            right += (dds3GetRandA(SDF_FRAME_RAND()) >> 4) % 3;
            if (upMotion == 1 || upMotion == 17 || upMotion == 2 || upMotion == 19 || upMotion == 3 || upMotion == 20) {
                if (before < 4.0f && now >= 4.0f) fldSeVol(left, volume);
                if (before < 13.0f && now >= 13.0f) fldSeVol(right, volume);
            } else if (upMotion == 4) {
                if (before < 4.0f && now >= 4.0f) fldSeVol(left, volume);
                if (before < 20.0f && now >= 20.0f) fldSeVol(right, volume);
            } else if (upMotion == 5) {
                if (before < 2.0f && now >= 2.0f) fldSeVol(left, volume);
                if (before < 16.0f && now >= 16.0f) fldSeVol(right, volume);
            } else if (upMotion == 6) {
                if (before < 2.0f && now >= 2.0f) fldSeVol(left, volume);
                if (before < 16.0f && now >= 16.0f) fldSeVol(right, volume);
            } else if (upMotion == 7) {
                if (before < 9.0f && now >= 9.0f) fldSeVol(left, volume);
                if (before < 21.0f && now >= 21.0f) fldSeVol(right, volume);
            } else if (upMotion == 14) {
                volume = 127;
                volume -= dds3GetRandA(SDF_FRAME_RAND()) % 31;
                if (volume > 127) volume = 127;
                if (volume < 0) volume = 0;
                if (before < 1.0f && now >= 1.0f) fldSeVol(0x24, volume);
                if (before < 18.0f && now >= 18.0f) fldSeVol(0x37, volume);
            } else if (upMotion == 15) {
                volume = 127;
                volume -= dds3GetRandA(SDF_FRAME_RAND()) % 31;
                if (volume > 127) volume = 127;
                if (volume < 0) volume = 0;
                if (before < 1.0f && now >= 1.0f) fldSeVol(0x24, volume);
                if (before < 18.0f && now >= 18.0f) fldSeVol(0x37, volume);
            }
        }
        /* Retail keeps jal + epilogue here. Qualified compiler trace: this
         * reset survives sibling selection and is deleted by life analysis.
         * See docs/matching/field/fldPlayer.md. */
        volume = 0;
    }
}

void fldPlayerCalc_Hit(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    sdfVector4_t pos;
    sdfVector4_t probe;

    if (ctl == NULL) {
        return;
    }
    if (fldPlayerFastMode == 1) {
        return;
    }
    if (l_UseCollision == 0) {
        return;
    }
    probe.x = fldGlobalBase.x;
    probe.y = fldGlobalBase.y;
    probe.z = fldGlobalBase.z;
    fldHitChkUnitElv(&probe.x);
    if (fldGlobalBase.hitValid == 0) {
        return;
    }
    fldGlobalBase.x = fldGlobalBase.hitX;
    fldGlobalBase.y = fldGlobalBase.hitY;
    fldGlobalBase.z = fldGlobalBase.hitZ;
    pos.x = fldGlobalBase.hitX;
    pos.y = fldGlobalBase.hitY;
    pos.z = fldGlobalBase.hitZ;
    dds3SetPosPoseSet(ctl->handle, (const sdfVector4_t *)&pos);
    fldGlobalBase.hitValid = 0;
    dds3GetPosPoseSet(ctl->handle);
    SDF_VU0_STOREVECTOR(&pos);
    SDF_VU0_COPYVECTOR((u8 *)ctl->handle->poseset + 0x70, &pos);
}

extern f32 fldCamPitch;
extern f32 fldPlayerWarpState;
extern const sdfVector4_t fldWarpRunAxis;
/* Canonical kernel/dds3ViewMatrix storage is in .data. The truthful section
 * declaration keeps the consumer's native size-less symbol reference. */
extern sdfMatrix4_t sdfViewMatrix __attribute__((section(".data")));
extern f32 EulerAngleAxisY(void);
extern void InversMatrix(void);
extern void FromRotation(f32 angle);
extern void dds3GetRotPoseSet(dds3Basic_t *basic);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flag);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);

void fldPlayerCalc_WarpRun(void)
{
    fldUnitControl_t *control = fldGetUnitControl();
    FAKEMATCH_ASM_REG(mdlObj_t *, model, "$21") = dds3ObjectBaseGetModelHandle(control->handle);
    mdlObj_t *model2 = NULL;
    FAKEMATCH_ASM_REG(s32, fastMode, "$4") = fldPlayerFastMode;
    sdfVector4_t position __attribute__((aligned(16)));
    sdfVector4_t alignedPrior __attribute__((aligned(16)));
    sdfVector4_t rotation __attribute__((aligned(16)));
    sdfVector4_t axis __attribute__((aligned(16)));
    f32 collisionPosition[4] __attribute__((aligned(16)));
    f32 angle, step, distance, pi;
    FAKEMATCH_ASM_REG(dds3Basic_t *, basic, "$5");
    dds3PoseSet_t *pose;
    FAKEMATCH_ASM_REG(s32, motion, "$19");
    FAKEMATCH_ASM_REG(s32, useCollision, "$2");
    sdfVector4_t *axisDest = &axis;
    FAKEMATCH_ASM_REG(s32, warpMode, "$3") = 1;
    /* Retain the mode through the direct aggregate copy so cc1 can use
     * the native little-endian block move address scratch. */
    FAKEMATCH_ASM_LIVE_WITH_INPUT(axisDest, warpMode);
    *axisDest = fldWarpRunAxis;
    if (fastMode != warpMode) {
        return;
    }
    {
        dds3Basic_t *secondary = control->handle2;
        if (secondary != NULL) {
            model2 = dds3ObjectBaseGetModelHandle(secondary);
        }
    }
    dds3GetPosPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&position);
    {
        FAKEMATCH_ASM_REG(fldAreaWork_t *, earlyWorld, "$16") = &fldGlobalBase;
        FAKEMATCH_ASM_REG(f32, posX, "$f5") = position.x;
        FAKEMATCH_ASM_REG(f32, posY, "$f4") = position.y;
        FAKEMATCH_ASM_REG(f32, posZ, "$f3") = position.z;
        useCollision = l_UseCollision;
        earlyWorld->prevX = earlyWorld->x;
        earlyWorld->prevY = earlyWorld->y;
        earlyWorld->prevZ = earlyWorld->z;
        earlyWorld->x = posX;
        earlyWorld->y = posY;
        earlyWorld->z = posZ;
        SDF_FRAME_MOVE_FROM(position);
        if (useCollision != 0) {
            dds3OnPoseSetFlag(control->handle->poseset, 8);
            if (control->handle2 != NULL) {
                dds3OnPoseSetFlag(control->handle2->poseset, 8);
            }
            if (l_UseCollision != 0) {
                FAKEMATCH_ASM_REG(f32, by, "$f0");
                FAKEMATCH_ASM_REG(f32, scaleInput, "$f1") = earlyWorld->float130;
                FAKEMATCH_ASM_REG(f32, height, "$f1");
                if (scaleInput > 0.0f) {
                    height = 5.0f;
                } else {
                    height = 10.0f;
                }
                by = position.y;
                by = by + height;
                earlyWorld->y = by;
            }
        }
    }
    {
        FAKEMATCH_ASM_REG(f32, pitch, "$f0") = fldCamPitch;
        FAKEMATCH_ASM_REG(f32, priorPitch, "$f1");
        {
            FAKEMATCH_ASM_REG(f32, bound, "$f2") = 360.0f;
            FAKEMATCH_ASM_LIVE_F32(bound);
            priorPitch = pitch;
            if (pitch >= bound) {
                do {
                    pitch = priorPitch - bound;
                    priorPitch = pitch;
                } while (pitch >= bound);
                FAKEMATCH_ASM_LIVE_F32(pitch);
                fldCamPitch = pitch;
            }
        }
        {
            FAKEMATCH_ASM_REG(f32, zeroPitch, "$f2") = 0.0f;
            priorPitch = pitch;
            if (priorPitch < zeroPitch) {
                FAKEMATCH_ASM_REG(f32, fullPitch, "$f3") = 360.0f;
                FAKEMATCH_ASM_LIVE_F32(fullPitch);
                do {
                    pitch = priorPitch + fullPitch;
                    priorPitch = pitch;
                } while (pitch < zeroPitch);
                fldCamPitch = pitch;
            }
        }
    }
    {
        fldAreaWork_t *camWorld = &fldGlobalBase;
        FAKEMATCH_ASM_REG(f32, turn, "$f1") = 360.0f;
        FAKEMATCH_ASM_REG(f32, a, "$f0") = camWorld->camAngle;
        if (a >= turn) {
            a = a - turn;
            while (a >= turn) { a = a - turn; }
            camWorld->camAngle = a;
        }
    }
    {
        FAKEMATCH_ASM_REG(fldAreaWork_t *, camWorld, "$2") = &fldGlobalBase;
        FAKEMATCH_ASM_REG(f32, zero, "$f1") = 0.0f;
        FAKEMATCH_ASM_REG(f32, a, "$f0") = camWorld->camAngle;
        if (a < zero) {
            FAKEMATCH_ASM_REG(f32, turn, "$f2") = 360.0f;
            a = a + turn;
            while (a < zero) { a = a + turn; }
            camWorld->camAngle = a;
        }
    }
    {
        /* The wrapping step uses separate previous and next values in
         * the native FP loop, with the true 360-degree bound alongside. */
        FAKEMATCH_ASM_REG(f32, warp, "$f0") = fldPlayerWarpState;
        FAKEMATCH_ASM_REG(f32, prior, "$f1");
        FAKEMATCH_ASM_REG(f32, full, "$f2") = 360.0f;
        FAKEMATCH_ASM_LIVE_F32(full);
        prior = warp;
        if (warp >= full) {
            do {
                warp = prior - full;
                prior = warp;
            } while (warp >= full);
            FAKEMATCH_ASM_LIVE_F32(warp);
            fldPlayerWarpState = warp;
        }
        {
            FAKEMATCH_ASM_REG(f32, zeroWarp, "$f2") = 0.0f;
            prior = warp;
            if (prior < zeroWarp) {
                FAKEMATCH_ASM_REG(f32, turnWarp, "$f3") = 360.0f;
                do {
                    warp = prior + turnWarp;
                    prior = warp;
                } while (warp < zeroWarp);
                fldPlayerWarpState = warp;
            }
        }
    }
    dds3GetRotPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&rotation);
    EulerAngleAxisY();
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    InversMatrix();
    control->moveFlag = 0;
    motion = 4;
    angle = R360_LR(fldGlobalBase.camAngle, fldPlayerWarpState);
    if (angle < 0.0f) {
        angle = -angle;
    }
    pi = 3.1399999f;
    distance = (180.0f - angle) * 0.077777774f;
    {
        f32 s = sdfSin((-fldPlayerWarpState * pi) / 180.0f);
        f32 oldX = fldGlobalBase.x;
        f32 oldY = fldGlobalBase.y;
        f32 cosVal;
        position.y = oldY;
        position.x = SDF_FRAME_MOVE(0, oldX - s * distance);
        cosVal = sdfCos((-fldPlayerWarpState * pi) / 180.0f);
        position.z = SDF_FRAME_MOVE(2, fldGlobalBase.z - cosVal * distance);
    }
    /* GetPos set the original W lane; the motion update replaced XYZ only. */
    dds3SetPosPoseSet(control->handle, &position);
    if (l_UseCollision != 0) {
        FAKEMATCH_ASM_REG(f32, cx, "$f2") = position.x;
        FAKEMATCH_ASM_REG(f32, cy, "$f0") = position.y;
        FAKEMATCH_ASM_REG(f32, cz, "$f1") = position.z;
        FAKEMATCH_ASM_REG(f32 *, cp, "$4") = collisionPosition;
        /* Collision reads XYZ only; the fourth scratch lane is inherited. */
        collisionPosition[0] = cx;
        cp[1] = cy;
        cp[2] = cz;
        FAKEMATCH_ASM_LIVE_F32_WITH_MEM(cx, cp[0]);
        fldGlobalBase.x = cx;
        fldGlobalBase.y = cy;
        fldGlobalBase.z = cz;
        fldHitChkUnit(cp);
        if (fldGlobalBase.hitValid != 0) {
            dds3Basic_t *setHandle;
            position.x = SDF_FRAME_MOVE_HIT(0, fldGlobalBase.hitX);
            position.y = SDF_FRAME_MOVE_HIT(1, fldGlobalBase.hitY + 44.0f);
            position.z = SDF_FRAME_MOVE_HIT(2, fldGlobalBase.hitZ);
            setHandle = control->handle;
            /* The pose-set call consumes this initialized coordinate packet
             * and actor handle. Keep both live before publishing XYZ. */
            FAKEMATCH_ASM_LIVE_WITH_MEM(setHandle, position);
            fldGlobalBase.x = position.x;
            fldGlobalBase.y = position.y;
            fldGlobalBase.z = position.z;
            dds3SetPosPoseSet(setHandle, &position);
            fldGlobalBase.hitValid = 0;
        }
        SDF_FRAME_MOVE_END(control->handle, position);
        dds3GetPosPoseSet(control->handle);
        SDF_VU0_STOREVECTOR(&position);
        basic = control->handle;
        FAKEMATCH_ASM_READ(basic);
        {
            FAKEMATCH_ASM_REG(sdfVector4_t *, poseX, "$3");
            sdfVector4_t *snapDst = &alignedPrior;
            /* Materialize the real local destination while its source
             * is still the actor's native pose pointer. */
            FAKEMATCH_ASM_READ(snapDst);

            /* Pose-set storage and this full previous-position snapshot
             * are quadword aligned, including the fourth lane. */
            pose = basic->poseset;
            poseX = &pose->pos2;
            SDF_VU0_COPYVECTOR(snapDst, poseX);
            if (sdfFabs(alignedPrior.x - position.x) < 1.0f) {
                poseX->x = position.x;
            } else {
                poseX->x = alignedPrior.x + (position.x - alignedPrior.x) / 3.0f;
            }
        }
        /* The original evaluates the primary pose pointer again per
         * selected y/z arm, then again for the w store and secondary copy. */
        if (sdfFabs(alignedPrior.y - position.y) < 1.0f) {
            ((dds3Basic_t volatile *)basic)->poseset->pos2.y = position.y;
        } else {
            ((dds3Basic_t volatile *)basic)->poseset->pos2.y = alignedPrior.y + (position.y - alignedPrior.y) / 3.0f;
        }
        if (sdfFabs(alignedPrior.z - position.z) < 1.0f) {
            ((dds3Basic_t volatile *)basic)->poseset->pos2.z = position.z;
        } else {
            ((dds3Basic_t volatile *)basic)->poseset->pos2.z = alignedPrior.z + (position.z - alignedPrior.z) / 3.0f;
        }
        pose = ((dds3Basic_t volatile *)basic)->poseset;
        pose->pos2.w = 1.0f;
        if (control->handle2 != NULL) {
            dds3PoseSet_t *other = control->handle2->poseset;
            other->pos2.x = pose->pos2.x;
            other->pos2.y = pose->pos2.y;
            other->pos2.z = pose->pos2.z;
            other->pos2.w = pose->pos2.w;
        }
    }
    angle = R360_LR(fldGlobalBase.camAngle, fldPlayerWarpState);
    step = angle / 6.0f;
    if (step < 0.0f) {
        step = -step;
    }
    if (step < 2.0f) {
        step = 2.0f;
    }
    step = SDF_FRAME_CHASE(step, angle, 2.0f);
    {
        FAKEMATCH_ASM_REG(f32, nextAngle, "$f1");
        if ((angle >= 0.0f && angle <= step) || (angle <= 0.0f && -step <= angle)) {
            nextAngle = fldPlayerWarpState;
        } else if (angle < 0.0f) {
            /* Consume the current heading in the native arithmetic input
             * register, separate from the result in nextAngle. */
            FAKEMATCH_ASM_REG(f32, currentAngle, "$f0") = fldGlobalBase.camAngle;
            FAKEMATCH_ASM_LIVE_F32(currentAngle);
            nextAngle = currentAngle - step;
        } else {
            FAKEMATCH_ASM_REG(f32, currentAngle, "$f0") = fldGlobalBase.camAngle;
            FAKEMATCH_ASM_LIVE_F32(currentAngle);
            nextAngle = currentAngle + step;
        }
        FAKEMATCH_ASM_LIVE_F32(nextAngle);
        fldGlobalBase.camAngle = nextAngle;
    }
    SDF_VU0_LOADVECTOR(axisDest);
    FromRotation(fldGlobalBase.camAngle * 3.1399999f / 180.0f);
    {
        FAKEMATCH_ASM_REG(fldAreaWork_t *, areaBase, "$3");
        sdfVector4_t *rotStore = &rotation;
        FAKEMATCH_ASM_LIVE(rotStore);
        SDF_VU0_STOREVECTOR(rotStore);
        {
            FAKEMATCH_ASM_REG(sdfVector4_t *, rotArg, "$16") = &rotation;
            FAKEMATCH_ASM_READ(rotArg);
            dds3SetRotPoseSet(control->handle, rotArg);
            {
                dds3Basic_t *secondaryRotation = control->handle2;
                if (secondaryRotation != NULL) {
                    dds3SetRotPoseSet(secondaryRotation, rotArg);
                }
            }
        }
        areaBase = &fldGlobalBase;
        {
            s32 area = areaBase->area;
            /* The alternate motion is a real conditional-move input. */
            FAKEMATCH_ASM_REG(s32, specialMotion, "$5") = 8;
            if (area == 23) {
                motion = specialMotion;
            }
        }
    }
    {
        /* The actual signed index is promoted for the comparison;
         * its incoming value is dead before either motion call. */
        FAKEMATCH_ASM_REG(s32, currentMotion, "$4") = model->current.h.motionIndex;
        if (currentMotion != motion) {
            FAKEMATCH_ASM_REG(f32, speed, "$f20") = 1.0f;
            FAKEMATCH_ASM_REG(f32, blend, "$f12") = 5.0f;
            FAKEMATCH_ASM_REG(s32, chosen, "$6");
            /* Keep the true blend and motion live until the speed input
             * is ready, then form A2 before publishing the old node's speed.
             * These constraints only select the EE issue order. */
            FAKEMATCH_ASM_LIVE_F32_GPR_WITH_F32(blend, motion, speed);
            chosen = motion;
            FAKEMATCH_ASM_LIVE_F32_WITH_GPR(speed, chosen);
            model->first->speed = speed;
            mdlSmoothLoopMotion(model, 0, chosen, blend, blend);
            if (control->handle2 != NULL) {
                /* The secondary call has a distinct controller load and
                 * reuses the same live speed after the primary call. */
                struct sdfMotionCtrlr_t *first = model2->first;
                FAKEMATCH_ASM_REG(f32, secondBlend, "$f12") = 5.0f;
                FAKEMATCH_ASM_REG(s32, secondChosen, "$6");
                FAKEMATCH_ASM_LIVE_F32_GPR_WITH_F32(secondBlend, motion, speed);
                secondChosen = motion;
                FAKEMATCH_ASM_LIVE_F32_WITH_GPR(speed, secondChosen);
                first->speed = speed;
                mdlSmoothLoopMotion(model2, 0, secondChosen, secondBlend, secondBlend);
            }
        }
    }
    {
        FAKEMATCH_ASM_REG(f32, finalX, "$f1") = position.x;
        fldAreaWork_t *finalBase = &fldGlobalBase;
        FAKEMATCH_ASM_REG(f32, finalY, "$f2") = position.y;
        FAKEMATCH_ASM_REG(f32, finalZ, "$f0") = position.z;
        FAKEMATCH_ASM_LIVE_F32(finalX);
        FAKEMATCH_ASM_READ(finalBase);
        FAKEMATCH_ASM_LIVE_F32_PAIR(finalY, finalZ);
        finalBase->x = finalX;
        finalBase->y = finalY;
        finalBase->z = finalZ;
    }
}

s32 fldPlayerCalcHasigo(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    sdfVector4_t pos;
    /* The signed arms share the updated Y/Z pair in their native FPRs. */
    FAKEMATCH_ASM_REG(f32, y, "$f0");
    FAKEMATCH_ASM_REG(f32, x, "$f5");
    FAKEMATCH_ASM_REG(f32, z, "$f4");
    f32 prevX;
    f32 prevY, prevZ;
    fldAreaWork_t *global;
    const sdfVector4_t *next;
    dds3Basic_t *target;
    if (gfldPlayerDamegeCnt > 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        global = &fldGlobalBase;
        next = &pos;
        FAKEMATCH_ASM_LIVE_PAIR(global, next);
        y = pos.y - 2.0f;
        z = pos.z;
        FAKEMATCH_ASM_LIVE_F32(z);
    } else if (gfldPlayerDamegeCnt < 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        global = &fldGlobalBase;
        next = &pos;
        FAKEMATCH_ASM_LIVE_PAIR(global, next);
        y = pos.y + 2.0f;
        z = pos.z;
        FAKEMATCH_ASM_LIVE_F32(z);
    } else {
        return 0;
    }
    /* Keep old and outgoing X together at the head of the commit. */
    prevX = global->x;
    x = pos.x;
    FAKEMATCH_ASM_LIVE_F32_PAIR(prevX, x);
    prevY = global->y;
    prevZ = global->z;
    target = ctl->handle;
    /* The local Y write precedes Z; the setter consumes the completed pose. */
    FAKEMATCH_ASM_LIVE_F32(y);
    pos.y = y;
    pos.z = z;
    global->prevX = prevX;
    /* Publish the previous X before the other previous coordinates. */
    FAKEMATCH_ASM_READ_MEM(global->prevX);
    global->prevY = prevY;
    global->prevZ = prevZ;
    global->z = z;
    global->x = x;
    /* Keep the real setter argument ready for the native Y delay slot. */
    FAKEMATCH_ASM_READ(target);
    global->y = y;
    dds3SetPosPoseSet(target, next);
    fldPlayerSdwOff();
    fldPlayerSvsOff();
    return 0;
}

const sdfVector4_t fldWarpRunAxis = {0.0f, 1.0f, 0.0f, 1.0f};

const sdfVector4_t fldNmlAxis = {0.0f, 1.0f, 0.0f, 1.0f};

#ifdef NON_MATCHING
extern const char fldDamageOnMsg[], fldDamageOffMsg[];
#define D_0054E90A ((u8 *)dds3PadStickState + 2)
extern u8 dds3PadStickState[2][2][2];
extern s8 dds3PadButtonState[];
extern s32 fldPlayerMoveState, fldPlayerSyukanState, fldCamSvsState;
extern u32 fldPlayerMoveStateSaved;
extern f32 fldCamPitch, fldCamLength, fldCamPitchTarget;
extern f32 fldNmlGunSpeedScale0, fldNmlPi0, fldNmlGunSpeedScale1, fldNmlPi1;
extern f32 fldNmlPi2, fldNmlPitchStep, fldNmlPi3, fldNmlPi4;
extern sdfMatrix4_t sdfViewMatrix;
#include "sdk_libc.h"

extern s32 fldEncountTestFlag(u32 flag);
extern s32 fldCamMiwatasiChk(void);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeIn2(s32 frames);
extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);
extern void fldEncountSetFlag(s32 mask);
extern void fldStatusSet(void);
extern void fldDmgStart(f32 x, f32 y, f32 z);
extern void mdlSingleMotion(mdlObj_t *model, s32 group, s32 motion);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flags);
extern void dds3GetRotPoseSet(dds3Basic_t *basic);

extern void InversMatrix(void);
extern f32 EulerAngleAxisY(void);
extern void FromRotation(f32 angle);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);
extern void fldHitChkUnitDamage(f32 *position);
extern void fldPlayerOn(void);
extern void fldPlayerSvsOn(void);
extern void fldPlayerSdwOn(void);
extern void fldPlayerMotionSe(s32 previous, s32 next);

static inline u64 fldPlayerPackedState(void)
{
    volatile u64 *state = (volatile u64 *)&DDS3_GBWK->akuma[0].packedState;
    return *state;
}

s32 fldPlayerCalc_Nml(void)
{
    fldUnitControl_t *control = fldGetUnitControl();
    dds3Basic_t *second = control->handle2;
    mdlObj_t *model = dds3ObjectBaseGetModelHandle(control->handle);
    sdfVector4_t axis __attribute__((aligned(16))) = *(volatile sdfVector4_t *)&fldNmlAxis;
    sdfVector4_t position __attribute__((aligned(16)));
    sdfVector4_t rotation __attribute__((aligned(16)));
    sdfVector4_t previous __attribute__((aligned(16)));
    f32 collisionPosition[4] __attribute__((aligned(16)));
    s32 left, right, forward, back, modifier, aiming = 0, action = 0;
    s32 moving = 0, moveDirection = -1, motion = 8, oldMotion;
    f32 angle, moveAngle = 0.0f, speed, delta, amount;

    if (second != NULL) {
        dds3ObjectBaseGetModelHandle(second);
    }
    if (gfldPlayerHasiCnt > 0) {
        f32 frame;
        oldMotion = model->current.h.motionIndex;
        if (oldMotion != 13) {
            model->first->speed = 1.0f;
            mdlSingleMotion(model, 0, 13);
            dds3FadeIn2(8);
        }
        frame = model->first->frame;
        if (fldGlobalBase.float12C < 13.0f && frame >= 13.0f) {
            dds3PadVibrationSet(0, 1, 10);
            dds3PadVibrationSet(1, 230, 10);
        }
        if (model->first->stat == 5) {
            gfldPlayerHasiCnt = 0;
            fldEncountSetFlag(0x40);
        } else {
            fldPlayerMotionSe(oldMotion, 13);
            return 0;
        }
    }
    if (fldGlobalBase.word78 == 0) {
        if (fldPlayerStepDist > 0) {
            if (fldGlobalBase.wordB0 == 0 && (fldRunDanteGunFlag.word50 & 1)) {
                fldSePlay(0x22);
                dds3FadeIn(0x80, 0x20, 0x20, 6);
                dds3PadVibrationSet(0, 1, 6);
                dds3PadVibrationSet(1, 150, 6);
                fldStatusSet();
                fldPlayerStepDist = 0;
                fldEncountSetFlag(0x40);
                return 0;
            }
            if (model->current.h.motionIndex != 10) {
                printf(fldDamageOnMsg);
                model->first->speed = 1.0f;
                mdlSingleMotion(model, 0, 10);
                fldDmgStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
                fldSePlay(0x22);
                dds3PadVibrationSet(0, 1, 30);
                dds3PadVibrationSet(1, 150, 30);
                fldStatusSet();
            }
            if (model->first->stat != 5) {
                return 0;
            }
            printf(fldDamageOffMsg);
            fldPlayerStepDist = 0;
            fldEncountSetFlag(0x40);
        }
    } else if (fldPlayerStepDist > 0) {
        s32 step = fldPlayerStepDist;
        if (model->current.h.motionIndex != 10) {
            model->first->speed = 1.0f;
            mdlSingleMotion(model, 0, 10);
            fldSePlay(0x22);
            dds3PadVibrationSet(0, 1, 30);
            dds3PadVibrationSet(1, 100, 30);
            fldStatusSet();
            step = fldPlayerStepDist;
        }
        step--;
        if (step != 0) {
            fldPlayerStepDist = step;
            return 0;
        }
        fldGlobalBase.word78 = 0;
        fldPlayerStepDist = 0;
        fldEncountSetFlag(0x40);
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (fldCamMiwatasiChk() != 0) {
        fldCamSvsState = 0x80;
        dds3ObjectBaseSetControlBasicSvs(control->handle, 0x80);
        dds3ObjectBaseOnFlag(control->handle, 0x800);
        fldPlayerOff();
        return 0;
    }
    if (l_UseCollision != 0) {
        dds3OnPoseSetFlag(control->handle->poseset, 8);
        if (control->handle2 != NULL) {
            dds3OnPoseSetFlag(control->handle2->poseset, 8);
        }
    }
    left = D_0054E90A[0] < 0x40;
    right = D_0054E90A[0] >= 0xC1;
    forward = D_0054E90A[1] < 0x40;
    back = D_0054E90A[1] >= 0xC1;
    if (!left && !right && !forward && !back) {
        left = dds3PadButtonState[0x24] != 0;
        right = dds3PadButtonState[0x25] != 0;
        forward = dds3PadButtonState[0x26] != 0;
        back = dds3PadButtonState[0x27] != 0;
    }
    if (FLD_CAM_PEEK(((dds3PadStickState[0][0][0] + 0xC0) & 0xFF) >= 0x81 ||
                     dds3PadStickState[0][0][1] < 0x40 || dds3PadStickState[0][0][1] >= 0xC1)) {
        left = right = forward = back = 0;
    }
    modifier = dds3PadButtonState[0x23] != 0 ||
               (dds3PadButtonState[0x28] != 0 && dds3PadButtonState[0x2A] != 0);
    if (fldGlobalBase.encountState > 0) {
        modifier = left = right = forward = back = 0;
    }
    dds3GetPosPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&position);
    fldGlobalBase.prevX = fldGlobalBase.x;
    fldGlobalBase.prevY = fldGlobalBase.y;
    fldGlobalBase.prevZ = fldGlobalBase.z;
    fldGlobalBase.x = position.x;
    fldGlobalBase.y = position.y;
    fldGlobalBase.z = position.z;
    SDF_FRAME_MOVE_FROM(position);
    if ((left || right || forward || back) && l_UseCollision != 0) {
        fldGlobalBase.y = position.y + (fldGlobalBase.float130 > 0.0f ? 5.0f : 10.0f);
    }
    {
        f32 pitch = fldCamPitch;
        f32 facing = fldGlobalBase.camAngle;
        if (pitch >= 360.0f) {
            do {
                pitch -= 360.0f;
            } while (pitch >= 360.0f);
            fldCamPitch = pitch;
        }
        if (pitch < 0.0f) {
            do {
                pitch += 360.0f;
            } while (pitch < 0.0f);
            fldCamPitch = pitch;
        }
        if (facing >= 360.0f) {
            do {
                facing -= 360.0f;
            } while (facing >= 360.0f);
            fldGlobalBase.camAngle = facing;
        }
        if (facing < 0.0f) {
            do {
                facing += 360.0f;
            } while (facing < 0.0f);
            fldGlobalBase.camAngle = facing;
        }
    }
    dds3GetRotPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&rotation);
    EulerAngleAxisY();
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    InversMatrix();
    control->moveFlag = 0;
    oldMotion = model->current.h.motionIndex;
    dds3GetPosPoseSet(control->handle);
    if (modifier) {
        s32 gunMode = fldRunDanteGunFlag.word54;
        if (gunMode == 0) {
            if (fldGlobalBase.mode != 1 && fldGlobalBase.mode != 3) {
                aiming = 1;
            }
        } else if (gunMode >= 1 && gunMode <= 10) {
            action = gunMode;
            if (gunMode == 2 && dds3PadButtonState[0x23] < 0) {
                fldDmgStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
                fldSePlay(0x22);
            }
        }
    }
    angle = fldGlobalBase.camAngle2;
    if (aiming && fldGlobalBase.mode != 1 && fldGlobalBase.mode != 3) {
        fldCamPitch = -fldGlobalBase.camAngle;
    }
    if (forward && left) {
        moving = 1;
        moveDirection = 7;
        moveAngle = 45.0f - fldCamPitch;
        if (aiming) {
            motion = 6;
        }
    } else if (forward && right) {
        moving = 1;
        moveDirection = 1;
        moveAngle = -45.0f - fldCamPitch;
        if (aiming) {
            motion = 5;
        }
    } else if (back && left) {
        moving = 1;
        moveDirection = 5;
        moveAngle = 135.0f - fldCamPitch;
        if (aiming) {
            motion = 7;
        }
    } else if (back && right) {
        moving = 1;
        moveDirection = 3;
        moveAngle = -135.0f - fldCamPitch;
        if (aiming) {
            motion = 7;
        }
    } else if (forward) {
        moving = 1;
        moveDirection = 0;
        moveAngle = -fldCamPitch;
        if (aiming) {
            motion = 1;
        }
    } else if (back) {
        moving = 1;
        moveDirection = 4;
        moveAngle = 180.0f - fldCamPitch;
        if (aiming) {
            motion = 7;
        }
    } else if (left) {
        moving = 1;
        moveDirection = 6;
        moveAngle = 90.0f - fldCamPitch;
        if (aiming) {
            motion = 6;
        }
    } else if (right) {
        moving = 1;
        moveDirection = 2;
        moveAngle = -90.0f - fldCamPitch;
        if (aiming) {
            motion = 5;
        }
    }
    if (moving && fldPlayerMoveState == moveDirection) {
        fldPlayerSyukanState++;
    } else if (moving && (moveDirection == 0 || (aiming && moveDirection == 6))) {
        fldPlayerMoveState = moveDirection;
        fldPlayerSyukanState = 0;
    } else {
        fldPlayerSyukanState = 0;
        fldPlayerMoveState = moveDirection;
    }
    if (moving) {
        if (!aiming) {
            angle = moveAngle;
            speed = fldGlobalBase.word4C != -1 ? 20.0f : 29.0f;
            motion = 1;
            if (fldGlobalBase.float130 > 0.0f) {
                speed += (sdfFabs(fldGlobalBase.float130) / 40.0f) + 2.0f;
            }
            if ((fldRunDanteGunFlag.word50 & 2) ||
                (fldPlayerPackedState() & 0x01400000ULL)) {
                speed *= fldNmlGunSpeedScale0;
            }
            delta = R360_LR(fldGlobalBase.camAngle, angle);
            if (delta < 0.0f) {
                delta = -delta;
            }
            moveAngle = (-angle * fldNmlPi0) / 180.0f;
            speed = (speed / 180.0f) * (180.0f - delta);
        } else {
            speed = fldPlayerMoveState == 0 ? 29.0f : 20.0f;
            if (fldRunDanteGunFlag.word50 & 2) {
                speed *= fldNmlGunSpeedScale1;
            }
            if (fldGlobalBase.float130 > 0.0f) {
                speed += (sdfFabs(fldGlobalBase.float130) / 40.0f) + 2.0f;
            }
            moveAngle = (-moveAngle * fldNmlPi1) / 180.0f;
        }
        position.x = SDF_FRAME_MOVE(0, fldGlobalBase.x - sdfSin(moveAngle) * speed);
        position.y = fldGlobalBase.y;
        position.z = SDF_FRAME_MOVE(2, fldGlobalBase.z - sdfCos(moveAngle) * speed);
    } else {
        position.x = fldGlobalBase.x;
        position.y = fldGlobalBase.y;
        position.z = fldGlobalBase.z;
    }
    dds3SetPosPoseSet(control->handle, &position);
    if (l_UseCollision != 0) {
        dds3Basic_t *basic;
        dds3PoseSet_t *pose;
        if (forward || back || left || right) {
            collisionPosition[0] = position.x;
            collisionPosition[1] = position.y;
            collisionPosition[2] = position.z;
            fldGlobalBase.x = position.x;
            fldGlobalBase.y = position.y;
            fldGlobalBase.z = position.z;
            fldHitChkUnit(collisionPosition);
            if (fldGlobalBase.hitValid != 0) {
                position.x = SDF_FRAME_MOVE_HIT(0, fldGlobalBase.hitX);
                position.y = SDF_FRAME_MOVE_HIT(1, fldGlobalBase.hitY + 44.0f);
                position.z = SDF_FRAME_MOVE_HIT(2, fldGlobalBase.hitZ);
                fldGlobalBase.x = position.x;
                fldGlobalBase.y = position.y;
                fldGlobalBase.z = position.z;
                dds3SetPosPoseSet(control->handle, &position);
                fldGlobalBase.hitValid = 0;
            }
            SDF_FRAME_MOVE_END(control->handle, position);
        }
        dds3GetPosPoseSet(control->handle);
        SDF_VU0_STOREVECTOR(&position);
        basic = control->handle;
        pose = basic->poseset;
        SDF_VU0_COPYVECTOR(&previous, &pose->pos2);
        if (sdfFabs(previous.x - position.x) < 1.0f) {
            pose->pos2.x = position.x;
        } else {
            pose->pos2.x = previous.x + (position.x - previous.x) / 3.0f;
        }
        if (sdfFabs(previous.y - position.y) < 1.0f) {
            basic->poseset->pos2.y = position.y;
        } else {
            basic->poseset->pos2.y = previous.y + (position.y - previous.y) / 3.0f;
        }
        if (sdfFabs(previous.z - position.z) < 1.0f) {
            basic->poseset->pos2.z = position.z;
        } else {
            basic->poseset->pos2.z = previous.z + (position.z - previous.z) / 3.0f;
        }
        pose = basic->poseset;
        pose->pos2.w = 1.0f;
        if (control->handle2 != NULL) {
            dds3PoseSet_t *other = control->handle2->poseset;
            other->pos2.x = pose->pos2.x;
            other->pos2.y = pose->pos2.y;
            other->pos2.z = pose->pos2.z;
            other->pos2.w = pose->pos2.w;
        }
    }
    fldHitChkUnitDamage(&position.x);
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    fldGlobalBase.camAngle2 = fldPlayerCalc360_8(angle);
    delta = R360_LR(fldGlobalBase.camAngle, angle);
    if (fldPlayerMoveState == 2 || fldPlayerMoveState == 6 ||
        fldPlayerMoveState == 3 || fldPlayerMoveState == 5) {
        amount = delta / 3.0f;
    } else if (fldPlayerMoveState == 1 || fldPlayerMoveState == 7) {
        amount = delta / 5.0f;
    } else {
        amount = delta / 6.0f;
    }
    if (aiming) {
        amount = delta * 0.5f;
    }
    if (amount < 0.0f) {
        amount = -amount;
    }
    if (amount < 2.0f) {
        amount = 2.0f;
    }
    amount = SDF_FRAME_CHASE(amount, delta, 2.0f);
    if ((delta >= 0.0f && delta <= amount) || (delta <= 0.0f && -amount <= delta)) {
        fldGlobalBase.camAngle = angle;
    } else if (delta < 0.0f) {
        fldGlobalBase.camAngle -= amount;
    } else {
        fldGlobalBase.camAngle += amount;
    }
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(fldGlobalBase.camAngle * fldNmlPi2 / 180.0f);
    SDF_VU0_STOREVECTOR(&rotation);
    dds3SetRotPoseSet(control->handle, &rotation);
    if (control->handle2 != NULL) {
        dds3SetRotPoseSet(control->handle2, &rotation);
    }
    if (fldGlobalBase.mode != 3 && fldGlobalBase.mode != 1) {
        if (aiming) {
            fldCamPitch = -fldGlobalBase.camAngle2;
            fldCamPitchTarget = fldCamPitch;
        } else if (FLD_CAM_TURN()) {
        } else if (fldPlayerSyukanState >= 4 &&
                   (fldPlayerMoveState == 1 || fldPlayerMoveState == 3 ||
                    fldPlayerMoveState == 5 || fldPlayerMoveState == 7)) {
            if (fldPlayerMoveState == 3) {
                fldCamPitch += 3.0f;
            } else if (fldPlayerMoveState == 5) {
                fldCamPitch -= 3.0f;
            } else {
                f32 target = -fldGlobalBase.camAngle;
                delta = R360_LR(fldCamPitch, target);
                amount = delta / 12.0f;
                if (amount < 0.0f) {
                    amount = -amount;
                }
                if (amount < 1.0f) {
                    amount = 1.0f;
                }
                amount = SDF_FRAME_CHASE(amount, delta, 1.0f);
                if ((delta >= 0.0f && delta <= amount) || (delta <= 0.0f && -amount <= delta)) {
                    fldCamPitch = target;
                } else if (delta < 0.0f) {
                    fldCamPitch -= amount;
                } else {
                    fldCamPitch += amount;
                }
            }
            fldCamPitchTarget = fldPlayerCalc360_8(fldCamPitch);
        } else if (fldPlayerSyukanState >= 3 && (fldPlayerMoveState == 2 || fldPlayerMoveState == 6)) {
            f32 target = -fldGlobalBase.camAngle;
            delta = R360_LR(fldCamPitch, target);
            amount = delta * 0.03125f;
            if (amount < 0.0f) {
                amount = -amount;
            }
            if (amount < 1.0f) {
                amount = 1.0f;
            }
            amount = SDF_FRAME_CHASE(amount, delta, 1.0f);
            if ((delta >= 0.0f && delta <= amount) || (delta <= 0.0f && -amount <= delta)) {
                fldCamPitch = target;
            } else if (delta < 0.0f) {
                fldCamPitch -= amount;
            } else {
                fldCamPitch += amount;
            }
            fldCamPitchTarget = fldPlayerCalc360_8(fldCamPitch);
        }
        if ((left || right || forward || back) && (s32)fldCamPitch != (s32)fldCamPitchTarget) {
            amount = fldNmlPitchStep;
            delta = R360_LR(fldCamPitch, fldCamPitchTarget);
            if (fldPlayerMoveState == 0) {
                amount = 0.5f;
            }
            if ((delta >= 0.0f && delta <= amount) || (delta <= 0.0f && -amount <= delta)) {
                fldCamPitch = fldCamPitchTarget;
            } else if (delta < 0.0f) {
                fldCamPitch -= amount;
            } else {
                fldCamPitch += amount;
            }
        }
        if (aiming) {
            s32 turn = 0;
            if (dds3PadButtonState[0x28] < 0 && dds3PadButtonState[0x2A] == 0) {
                fldCamPitchTarget -= 88.0f;
                turn = 1;
            } else if (dds3PadButtonState[0x2A] < 0 && dds3PadButtonState[0x28] == 0) {
                fldCamPitchTarget += 88.0f;
                turn = 1;
            }
            if (turn) {
                f32 target = fldPlayerCalc360_4(fldCamPitchTarget);
                fldCamPitchTarget = target;
                fldGlobalBase.camAngle2 = -target;
            }
        } else if (dds3PadButtonState[0x28] != 0 && dds3PadButtonState[0x2A] == 0) {
            fldCamPitch -= 4.0f;
            if (modifier) {
                fldGlobalBase.camAngle = -fldCamPitch;
                SDF_VU0_LOADVECTOR(&axis);
                FromRotation(fldGlobalBase.camAngle * fldNmlPi3 / 180.0f);
                SDF_VU0_STOREVECTOR(&rotation);
                dds3SetRotPoseSet(control->handle, &rotation);
            }
            fldCamPitchTarget = fldPlayerCalc360_8(fldCamPitch);
        } else if (dds3PadButtonState[0x2A] != 0 && dds3PadButtonState[0x28] == 0) {
            fldCamPitch += 4.0f;
            if (modifier) {
                fldGlobalBase.camAngle = -fldCamPitch;
                SDF_VU0_LOADVECTOR(&axis);
                FromRotation(fldGlobalBase.camAngle * fldNmlPi4 / 180.0f);
                SDF_VU0_STOREVECTOR(&rotation);
                dds3SetRotPoseSet(control->handle, &rotation);
            }
            fldCamPitchTarget = fldPlayerCalc360_8(fldCamPitch);
        }
        fldCamPitchTarget = fldPlayerCalc360_8(fldCamPitchTarget);
        fldPlayerMoveStateSaved = fldPlayerMoveState;
    }
    fldCamSvsState = 0;
    dds3ObjectBaseSetControlBasicSvs(control->handle, 0);
    fldPlayerOn();
    fldPlayerSdwOn();
    if (fldGlobalBase.mode == 1 || fldGlobalBase.mode == 3) {
        fldPlayerSvsOff();
    } else if (fldCamLength < 100.0f) {
        fldPlayerSvsOn();
    } else {
        fldPlayerSvsOff();
    }
    switch (action) {
    case 1:
        motion = 9;
        break;
    case 2:
        motion = 10;
        break;
    case 3:
        motion = 16;
        break;
    case 4:
        motion = 11;
        break;
    case 5:
        motion = 12;
        break;
    case 6:
        motion = 13;
        break;
    case 7:
        motion = 14;
        break;
    case 8:
        motion = 15;
        break;
    case 9:
        motion = 2;
        break;
    case 10:
        motion = 3;
        break;
    }
    if ((fldRunDanteGunFlag.word50 & 2) ||
        (fldPlayerPackedState() & 0x01400000ULL)) {
        if (motion == 8) {
            motion = 18;
        }
        if (motion == 6) {
            motion = 22;
        }
        if (motion == 5) {
            motion = 21;
        }
        if (motion == 7) {
            motion = 23;
        }
        if (motion == 1 || motion == 4) {
            motion = 17;
        }
    }
    fldPlayerMotion(oldMotion, motion, 6.0f);
    fldGlobalBase.x = position.x;
    fldGlobalBase.z = position.z;
    fldGlobalBase.y = position.y;
    {
        dds3PoseSet_t *pose = control->handle->poseset;
        fldPlayerPosRotWork.pos.x = pose->pos2.x;
        fldPlayerPosRotWork.pos.y = pose->pos2.y;
        fldPlayerPosRotWork.pos.z = pose->pos2.z;
        fldPlayerPosRotWork.pos.w = pose->pos2.w;
        fldPlayerPosRotWork.rot.x = pose->rot.x;
        fldPlayerPosRotWork.rot.y = pose->rot.y;
        fldPlayerPosRotWork.rot.z = pose->rot.z;
        fldPlayerPosRotWork.rot.w = pose->rot.w;
    }
    fldPlayerMotionSe(oldMotion, motion);
    return 0;
}
#else
/* Native diagnostic text used by the retained assembly. */
const char fldDamageOnMsg[] = "damage on\n";
const char fldDamageOffMsg[] = "damage off\n";

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Nml);
#endif

void fldPlayerCalc_WarpStart(void)
{
    fldWmWarpMode = 1;
}

void fldPlayerCalc_FastStart(void)
{
    if (fldWmWarpMode == 0) {
        fldWmWarpMode = 2;
        fldWmFastMove = 60.0f;
    }
}

void fldPlayerCalc_Wm3(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    mdlObj_t *model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    sdfVector4_t scale = fldWm3Scale;
    f32 next;

    if (fldWmWarpMode != 1) {
        model->first->speed = 0.0f;
    } else {
        scale.x *= fldWmapPlayerState;
        scale.y *= fldWmapPlayerState;
        scale.z *= fldWmapPlayerState;
        dds3SetSclPoseSet(ctl->handle, &scale.x);
        next = fldWmapPlayerState - 0.08f;
        model->first->speed = 5.0f;
        if (next <= 0.1f) {
            fldWmapPlayerState = 0.1f;
        } else {
            fldWmapPlayerState = next;
        }
    }
}

void fldPlayerCalc_Wm4(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    mdlObj_t *model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
    sdfVector4_t scale = fldWm4Scale;
    f32 next;

    if (fldWmWarpMode == 2) {
        fldWmWarpMode = 3;
        fldWmapPlayerState = 0.1f;
    } else {
        scale.x *= fldWmapPlayerState;
        scale.y *= fldWmapPlayerState;
        scale.z *= fldWmapPlayerState;
        dds3SetSclPoseSet(ctl->handle, &scale.x);
        next = fldWmapPlayerState + 0.04f;
        model->first->speed = 5.0f;
        if (1.0f < next) {
            fldWmapPlayerState = 1.0f;
            fldWmWarpMode = 0;
        } else {
            fldWmapPlayerState = next;
        }
    }
}

/* NON_MATCHING: 60 of 742 words match (first difference at +0x0: frame 0xF0
 * against retail's 0xE0, 2968 of 3068 bytes emitted), while the block shapes
 * agree to 606 of retail's 768 instructions. gcse PRE deletes the second
 * (high dds3PadButtonState) computation, which cc1probe --gcse shows as expr#8
 * avail uid205 with uid748 deleted, so one allocno carries the pad base across
 * eight calls. Global alloc then ranks it freq 3 / live 292 = priority 102
 * against the model handle's freq 2 / live 601 = 33, and with all nine
 * callee-saved registers taken at or above 102 the model handle spills to the
 * frame instead of taking $30, renaming every register after it; retail
 * rematerialises the pad base at both sites. -fno-gcse gives retail's frame and
 * 87 matching words but loses the shared %hi(fldGlobalBase) retail keeps in
 * $22, so neither setting is the real compile. The two 3.1399996f loads are
 * merged into a third callee-saved float register the same way, where retail
 * emits the two .lit4 entries D_0054C40C and D_0054C410. */
#ifdef NON_MATCHING
extern f32 fldCamPitch;
extern s8 dds3PadButtonState[];
extern s32 fldHitChkMatter;
extern s32 fldWmapMatterLast, fldWmapBlendStep;
extern collision_s *fldMatterTable;
extern sdfVector4_t fldWmapPlayerPos, fldWmapPosFrom, fldWmapPosTo;
extern const sdfVector4Unaligned_t fldWm2Scale, fldWm2WorkInit;
extern sdfMatrix4_t sdfViewMatrix;
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);
extern void fldStatusSet(void);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flags);

extern void FromEuler(f32 x, f32 y, f32 z);
extern void InversMatrix(void);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);
extern f32 sdfAtan2(f32 y, f32 x);
extern void fldHitChkUnitVWM(f32 *position);
extern void fldHitChkUnitDamage(f32 *position);

s32 fldPlayerCalc_Wm2(void)
{
    fldUnitControl_t *control = fldGetUnitControl();
    mdlObj_t *model = (mdlObj_t *)dds3ObjectBaseGetModelHandle(control->handle);
    s32 step = fldPlayerStepDist;
    sdfVector4_t position;
    sdfVector4_t previous;
    sdfVector4_t initial;
    sdfVector4_t direction;
    sdfVector4_t normal;
    sdfVector4_t rotation;
    sdfVector4_t scale = fldWm2Scale;
    sdfVector4_t work;
    s32 up, down, left, right;
    s32 moving = 0;
    f32 angle;
    f32 pitch;

    if (step > 0) {
        if (step == 60) {
            fldSePlay(0x22);
            dds3FadeIn(0x80, 0x20, 0x20, 6);
            dds3PadVibrationSet(0, 1, 8);
            dds3PadVibrationSet(1, 100, 8);
            fldStatusSet();
            step = fldPlayerStepDist;
        }
        step -= 10;
        if (step <= 0) {
            fldPlayerStepDist = 0;
        } else {
            fldPlayerStepDist = step;
            return 0;
        }
    }
    if (l_UseCollision != 0) {
        dds3OnPoseSetFlag(control->handle->poseset, 8);
    }
    up = dds3PadButtonState[0x27] != 0;
    left = dds3PadButtonState[0x24] != 0;
    right = dds3PadButtonState[0x25] != 0;
    down = dds3PadButtonState[0x26] != 0;
    if (fldGlobalBase.encountState > 0) {
        right = left = up = down = 0;
    }
    dds3GetPosPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&position);
    SDF_VU0_STOREVECTOR(&initial);
    fldGlobalBase.prevX = fldGlobalBase.x;
    fldGlobalBase.prevY = fldGlobalBase.y;
    fldGlobalBase.prevZ = fldGlobalBase.z;
    fldGlobalBase.x = position.x;
    fldGlobalBase.y = position.y;
    fldGlobalBase.z = position.z;
    SDF_FRAME_MOVE_FROM(position);
    if ((left || right || down || up) && l_UseCollision != 0) {
        fldGlobalBase.y = position.y + 3.0f;
    }
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    InversMatrix();
    control->moveFlag = 0;
    dds3GetPosPoseSet(control->handle);
    fldWmWarpMode = fldWmWarpMode == 1 ? 0 : fldWmWarpMode;
    if (fldWmWarpMode == 2) {
        fldPlayerCalc_Wm4();
    }
    if (fldWmWarpMode == 3) {
        fldPlayerCalc_Wm4();
    }
    if (fldWmWarpMode != 0) {
        right = left = up = down = 0;
    } else {
        pitch = 0.0f;
        fldCamPitch = pitch;
        angle = fldCamPitch;
        if (up && right) {
            angle = 45.0f - angle;
            moving = 1;
        } else if (up && left) {
            angle = -45.0f - angle;
            moving = 1;
        } else if (down && right) {
            angle = 135.0f - angle;
            moving = 1;
        } else if (down && left) {
            angle = -135.0f - angle;
            moving = 1;
        } else if (up) {
            angle = -angle;
            moving = 1;
        } else if (down) {
            angle = 180.0f - angle;
            moving = 1;
        } else if (right) {
            angle = 90.0f - angle;
            moving = 1;
        } else if (left) {
            angle = -90.0f - angle;
            moving = 1;
        }
        if (moving) {
            angle = -angle * 3.1399999f / 180.0f;
            position.x = SDF_FRAME_MOVE(0, fldGlobalBase.x - sdfSin(angle) * 16.0f);
            position.y = fldGlobalBase.y;
            position.z = SDF_FRAME_MOVE(2, fldGlobalBase.z - sdfCos(angle) * 16.0f);
            dds3SetPosPoseSet(control->handle, &position);
        } else {
            position.x = fldGlobalBase.x;
            position.y = fldGlobalBase.y;
            position.z = fldGlobalBase.z;
            dds3SetPosPoseSet(control->handle, &position);
        }
        if (l_UseCollision == 0 && fldRunDanteGunFlag.counter != 0 &&
            (dds3PadButtonState[0x29] != 0 || dds3PadButtonState[0x2B] != 0)) {
            position.x = fldGlobalBase.x;
            position.y = fldGlobalBase.y;
            position.z = fldGlobalBase.z;
            dds3SetPosPoseSet(control->handle, &position);
        }
    }
    if (l_UseCollision != 0) {
        dds3Basic_t *basic;
        dds3PoseSet_t *pose;

        if (down || up || left || right) {
            work.x = position.x;
            work.y = position.y;
            work.z = position.z;
            fldGlobalBase.x = position.x;
            fldGlobalBase.y = position.y;
            fldGlobalBase.z = position.z;
            fldHitChkUnitVWM(&work.x);
            if (fldGlobalBase.hitValid != 0) {
                fldGlobalBase.x = SDF_FRAME_MOVE_HIT(0, fldGlobalBase.hitX);
                if (fldGlobalBase.area == 1) {
                    fldGlobalBase.y = SDF_FRAME_MOVE_HIT(1, (fldGlobalBase.hitY + 44.0f) - 11.0f);
                } else {
                    fldGlobalBase.y = SDF_FRAME_MOVE_HIT(1, fldGlobalBase.hitY + 44.0f);
                }
                position.x = fldGlobalBase.x;
                position.y = fldGlobalBase.y;
                position.z = SDF_FRAME_MOVE_HIT(2, fldGlobalBase.hitZ);
                fldGlobalBase.z = position.z;
                dds3SetPosPoseSet(control->handle, &position);
                fldGlobalBase.hitValid = 0;
            }
            SDF_FRAME_MOVE_END(control->handle, position);
        }
        dds3GetPosPoseSet(control->handle);
        SDF_VU0_STOREVECTOR(&position);
        basic = control->handle;
        pose = basic->poseset;
        SDF_VU0_COPYVECTOR(&previous, &pose->pos2);
        if (sdfFabs(previous.x - position.x) < 1.0f) {
            pose->pos2.x = position.x;
        } else {
            pose->pos2.x = previous.x + (position.x - previous.x) / 3.0f;
        }
        if (sdfFabs(previous.y - position.y) < 1.0f) {
            basic->poseset->pos2.y = position.y;
        } else {
            basic->poseset->pos2.y = previous.y + (position.y - previous.y) / 3.0f;
        }
        if (sdfFabs(previous.z - position.z) < 1.0f) {
            basic->poseset->pos2.z = position.z;
        } else {
            basic->poseset->pos2.z = previous.z + (position.z - previous.z) / 3.0f;
        }
        basic->poseset->pos2.w = 1.0f;
        if (l_UseCollision != 0) {
            if (down || up || left || right) {
                direction.x = position.x - initial.x;
                direction.y = position.y - initial.y;
                direction.z = position.z - initial.z;
                direction.w = 1.0f;
                SDF_VU0_LOADVECTOR(&direction);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(&normal);
                position.x = initial.x + normal.x * 16.0f;
                position.y = (initial.y + normal.y * 16.0f) + 3.0f;
                SDF_FRAME_MOVE_FROM(initial);
                position.z = initial.z + normal.z * 16.0f;
                dds3SetPosPoseSet(basic, &position);
                work.x = position.x;
                work.y = position.y;
                work.z = position.z;
                fldGlobalBase.x = position.x;
                fldGlobalBase.y = position.y;
                fldGlobalBase.z = position.z;
                fldHitChkUnitVWM(&work.x);
                if (fldGlobalBase.hitValid != 0) {
                    fldGlobalBase.x = SDF_FRAME_MOVE_HIT(0, fldGlobalBase.hitX);
                    if (fldGlobalBase.area == 1) {
                        fldGlobalBase.y = SDF_FRAME_MOVE_HIT(1, (fldGlobalBase.hitY + 44.0f) - 11.0f);
                    } else {
                        fldGlobalBase.y = SDF_FRAME_MOVE_HIT(1, fldGlobalBase.hitY + 44.0f);
                    }
                    position.x = fldGlobalBase.x;
                    position.y = fldGlobalBase.y;
                    position.z = SDF_FRAME_MOVE_HIT(2, fldGlobalBase.hitZ);
                    fldGlobalBase.z = position.z;
                    dds3SetPosPoseSet(control->handle, &position);
                    fldGlobalBase.hitValid = 0;
                }
                SDF_FRAME_MOVE_END(control->handle, position);
            }
            dds3GetPosPoseSet(control->handle);
            SDF_VU0_STOREVECTOR(&position);
            basic = control->handle;
            pose = basic->poseset;
            SDF_VU0_COPYVECTOR(&previous, &pose->pos2);
            if (sdfFabs(previous.x - position.x) < 1.0f) {
                pose->pos2.x = position.x;
            } else {
                pose->pos2.x = previous.x + (position.x - previous.x) / 3.0f;
            }
            if (sdfFabs(previous.y - position.y) < 1.0f) {
                basic->poseset->pos2.y = position.y;
            } else {
                basic->poseset->pos2.y = previous.y + (position.y - previous.y) / 3.0f;
            }
            if (sdfFabs(previous.z - position.z) < 1.0f) {
                basic->poseset->pos2.z = position.z;
            } else {
                basic->poseset->pos2.z = previous.z + (position.z - previous.z) / 3.0f;
            }
            basic->poseset->pos2.w = 1.0f;
        }
    }
    fldHitChkUnitDamage(&position.x);
    fldGlobalBase.z = position.z;
    fldGlobalBase.x = position.x;
    fldGlobalBase.y = position.y;
    if (fldHitChkMatter >= 0 && fldWmapMatterLast != fldHitChkMatter) {
        fldWmapPosFrom.z = fldWmapPlayerPos.z;
        fldWmapPosFrom.x = fldWmapPlayerPos.x;
        fldWmapPosFrom.y = fldWmapPlayerPos.y;
        fldWmapPosTo.x = fldMatterTable[fldHitChkMatter].normal[0];
        fldWmapPosTo.y = fldMatterTable[fldHitChkMatter].normal[1];
        fldWmapMatterLast = fldHitChkMatter;
        fldWmapPosTo.z = fldMatterTable[fldHitChkMatter].normal[2];
        fldWmapBlendStep = 0;
    }
    if (fldWmapBlendStep < 8) {
        f32 fraction = (f32)fldWmapBlendStep;
        fldWmapBlendStep++;
        fldWmapPlayerPos.z = fldWmapPosFrom.z + (fldWmapPosTo.z - fldWmapPosFrom.z) * fraction * 0.125f;
        fldWmapPlayerPos.x = fldWmapPosFrom.x + (fldWmapPosTo.x - fldWmapPosFrom.x) * fraction * 0.125f;
        fldWmapPlayerPos.y = fldWmapPosFrom.y + (fldWmapPosTo.y - fldWmapPosFrom.y) * fraction * 0.125f;
    } else {
        fldWmapPlayerPos.z = fldWmapPosTo.z;
        fldWmapPlayerPos.x = fldWmapPosTo.x;
        fldWmapPlayerPos.y = fldWmapPosTo.y;
    }
    if (fldGlobalBase.area == 1) {
        FromEuler(-1.046666503f, 0.0f, 0.0f);
        SDF_VU0_STOREVECTOR(&rotation);
        dds3SetRotPoseSet(control->handle, &rotation);
    } else if (fldHitChkMatter >= 0) {
        f32 a, b, xrot, zrot, zAbs, xAbs;

        work = fldWm2WorkInit;
        work.x = fldWmapPlayerPos.x;
        work.y = fldWmapPlayerPos.y;
        work.z = fldWmapPlayerPos.z;
        a = sdfAtan2(work.x, work.y);
        zAbs = a < 0.0f ? -a : a;
        zrot = 3.1399997f - zAbs;
        if (a < 0.0f) {
            zrot = -zrot;
        }
        b = sdfAtan2(work.z, work.y);
        xAbs = b < 0.0f ? -b : b;
        xrot = 3.1399997f - xAbs;
        if (b >= 0.0f) {
            xrot = -xrot;
        }
        FromEuler(xrot, 0.0f, zrot);
        SDF_VU0_STOREVECTOR(&rotation);
        dds3SetRotPoseSet(control->handle, &rotation);
    }
    if (fldWmWarpMode == 0) {
        fldWmapPlayerState = 1.0f;
        dds3SetSclPoseSet(control->handle, &scale.x);
        model->first->speed = 1.0f;
    }
    if (evtBitCheck(0x75B) != 0) {
        dds3ObjectBaseOnFlag(control->handle, 1);
    } else {
        dds3ObjectBaseOffFlag(control->handle, 1);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Wm2);
#endif

/* NON_MATCHING: 776 of 1082 words differ (first at +0x4). Thirteen input flags
 * compete for four callee-saved registers. cc1probe --alloc measures allocno
 * priorities modifier 647, moving 453, forward 437, back 415, turnRight 380,
 * turnLeft 375, so turnLeft spills where retail keeps it, renaming every
 * register and flag slot downstream. moving is 12 refs over 794 insns, one per
 * source assignment; cutting it to a single assignment hands the freed register
 * to turnRight (387) and not turnLeft (381), so retail's set needs turnLeft to
 * outrank turnRight too and is not reachable from this source. */
#ifdef NON_MATCHING
extern const sdfVector4Unaligned_t fldNmlSyukanAxis;
extern u8 dds3PadStickState[2][2][2];
extern s8 dds3PadButtonState[];
extern s32 fldPlayerMoveState, fldPlayerSyukanState, fldSyukanState;
extern s32 fldCamSvsState;
extern f32 fldCamPitchTarget, fldSyukanGunSpeedScale, fldSyukanPi0, fldSyukanPi1, fldSyukanPi2;
extern f32 fldCamPitch;
extern sdfMatrix4_t sdfViewMatrix;
extern s32 fldCamMiwatasiChk(void);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeIn2(s32 frames);
extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);
extern void fldEncountSetFlag(s32 mask);
extern void fldStatusSet(void);
extern void fldDmgStart(f32 x, f32 y, f32 z);
extern void mdlSingleMotion(mdlObj_t *model, s32 group, s32 motion);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flags);
extern void dds3GetRotPoseSet(dds3Basic_t *basic);

extern void InversMatrix(void);
extern f32 EulerAngleAxisY(void);
extern void FromRotation(f32 angle);
extern void fldPlayerMotionSe(s32 previous, s32 next);
extern f32 sdfSin(f32 angle);
extern f32 sdfCos(f32 angle);
extern void fldHitChkUnitDamage(f32 *position);
extern void fldPlayerOn(void);

s32 fldPlayerCalc_Nml_syukan(void)
{
    fldUnitControl_t *control = fldGetUnitControl();
    mdlObj_t *model = dds3ObjectBaseGetModelHandle(control->handle);
    sdfVector4_t position __attribute__((aligned(16)));
    sdfVector4_t previous __attribute__((aligned(16)));
    sdfVector4_t rotation __attribute__((aligned(16)));
    sdfVector4_t axis __attribute__((aligned(16))) = fldNmlSyukanAxis;
    f32 collisionPosition[4] __attribute__((aligned(16)));
    s32 moving = 0;
    s32 left = 0, right = 0, forward = 0, back = 0;
    s32 turnLeft = 0, turnRight = 0, modifier = 0;
    s32 turnAround = 0;
    s32 lookLeft = 0, lookRight = 0, quickLeft = 0, quickRight = 0;
    s32 motion, oldMotion, step;
    f32 angle, speed, rad, delta, amount, target;

    if (gfldPlayerHasiCnt > 0) {
        oldMotion = model->current.h.motionIndex;
        motion = 13;
        if (oldMotion != motion) {
            model->first->speed = 1.0f;
            mdlSingleMotion(model, 0, motion);
            dds3FadeIn2(8);
        }
        if (fldGlobalBase.float12C < 13.0f && model->first->frame >= 13.0f) {
            dds3PadVibrationSet(0, 1, 10);
            dds3PadVibrationSet(1, 230, 10);
        }
        if (model->first->stat == 5) {
            gfldPlayerHasiCnt = 0;
            fldEncountSetFlag(0x40);
        } else {
            fldPlayerMotionSe(oldMotion, motion);
            return 0;
        }
    }
    step = fldPlayerStepDist;
    if (fldGlobalBase.word78 == 0) {
        if (step > 0) {
            if (fldGlobalBase.wordB0 == 0 && (fldRunDanteGunFlag.word50 & 1)) {
                fldSePlay(0x22);
                dds3FadeIn(0x80, 0x20, 0x20, 6);
                dds3PadVibrationSet(0, 1, 6);
                dds3PadVibrationSet(1, 150, 6);
                fldStatusSet();
                fldPlayerStepDist = 0;
                fldEncountSetFlag(0x40);
                return 0;
            }
            oldMotion = model->current.h.motionIndex;
            motion = 10;
            if (oldMotion != motion) {
                model->first->speed = 1.0f;
                mdlSingleMotion(model, 0, motion);
                fldDmgStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
                fldSePlay(0x22);
                dds3PadVibrationSet(0, 1, 30);
                dds3PadVibrationSet(1, 150, 30);
                fldStatusSet();
            }
            if (model->first->stat != 5) {
                return 0;
            }
            fldPlayerStepDist = 0;
            fldEncountSetFlag(0x40);
        }
    } else if (step > 0) {
        oldMotion = model->current.h.motionIndex;
        motion = 10;
        if (oldMotion != motion) {
            model->first->speed = 1.0f;
            mdlSingleMotion(model, 0, motion);
            fldSePlay(0x22);
            dds3PadVibrationSet(0, 1, 30);
            dds3PadVibrationSet(1, 100, 30);
            fldStatusSet();
            step = fldPlayerStepDist;
        }
        step--;
        if (step != 0) {
            fldPlayerStepDist = step;
            return 0;
        }
        fldGlobalBase.word78 = 0;
        fldPlayerStepDist = 0;
        fldEncountSetFlag(0x40);
    }
    if (fldCamMiwatasiChk() != 0) {
        fldCamSvsState = 0x80;
        dds3ObjectBaseSetControlBasicSvs(control->handle, 0x80);
        dds3ObjectBaseOnFlag(control->handle, 0x800);
        fldPlayerOff();
        return 0;
    }
    if (l_UseCollision != 0) {
        dds3OnPoseSetFlag(control->handle->poseset, 8);
    }
    if (fldSyukanState == 0) {
        left = dds3PadButtonState[0x24] != 0;
        turnRight = dds3PadButtonState[0x2A] != 0;
        forward = dds3PadButtonState[0x26] != 0;
        right = dds3PadButtonState[0x25] != 0;
        back = dds3PadButtonState[0x27] != 0;
        modifier = dds3PadButtonState[0x23] != 0;
        turnLeft = dds3PadButtonState[0x28] != 0;
        if (fldGlobalBase.encountState > 0) {
            modifier = 0;
            fldGlobalBase.encountState--;
        }
        if (!forward && !back && !modifier && !turnLeft && !turnRight) {
            lookRight = dds3PadButtonState[0x25] != 0;
            lookLeft = dds3PadButtonState[0x24] != 0;
        }
        if (!modifier) {
            if (back) {
                if (!left) {
                    turnAround = (right == 0);
                }
            }
        }
        if (modifier) {
            if (turnLeft) {
                quickLeft = 1;
            } else if (turnRight) {
                quickRight = 1;
            } else if (!forward && !back) {
                turnRight = right != 0;
                turnLeft = left != 0;
            }
        }
    }
    if (((dds3PadStickState[0][0][0] + 0xC0) & 0xFF) >= 0x81 ||
        dds3PadStickState[0][0][1] < 0x40 || dds3PadStickState[0][0][1] >= 0xC1) {
        left = 0;
        forward = 0;
        right = 0;
        back = 0;
        turnAround = 0;
        turnLeft = 0;
        turnRight = 0;
        modifier = 0;
        lookLeft = 0;
        lookRight = 0;
        quickLeft = 0;
        quickRight = 0;
    }
    if (fldGlobalBase.encountState > 0) {
        left = 0;
        forward = 0;
        right = 0;
        back = 0;
        turnLeft = 0;
        turnRight = 0;
        modifier = 0;
        lookLeft = 0;
        lookRight = 0;
        quickLeft = 0;
        quickRight = 0;
    }
    dds3GetPosPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&position);
    fldGlobalBase.prevX = fldGlobalBase.x;
    fldGlobalBase.prevY = fldGlobalBase.y;
    fldGlobalBase.prevZ = fldGlobalBase.z;
    fldGlobalBase.x = position.x;
    fldGlobalBase.y = position.y;
    fldGlobalBase.z = position.z;
    SDF_FRAME_MOVE_FROM(position);
    if ((left || right || forward || back || turnLeft || turnRight) && l_UseCollision != 0) {
        if (fldGlobalBase.float130 > 0.0f) {
            fldGlobalBase.y = position.y + 5.0f;
        } else {
            fldGlobalBase.y = position.y + 10.0f;
        }
    }
    while (fldCamPitch >= 360.0f) {
        fldCamPitch -= 360.0f;
    }
    while (fldCamPitch < 0.0f) {
        fldCamPitch += 360.0f;
    }
    while (fldGlobalBase.camAngle >= 360.0f) {
        fldGlobalBase.camAngle -= 360.0f;
    }
    while (fldGlobalBase.camAngle < 0.0f) {
        fldGlobalBase.camAngle += 360.0f;
    }
    dds3GetRotPoseSet(control->handle);
    SDF_VU0_STOREVECTOR(&rotation);
    EulerAngleAxisY();
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    InversMatrix();
    motion = 8;
    control->moveFlag = 0;
    oldMotion = model->current.h.motionIndex;
    dds3GetPosPoseSet(control->handle);
    angle = fldGlobalBase.camAngle2;
    if (modifier && forward && left) {
        moving = 1;
        angle = 45.0f - fldCamPitch;
    } else if (!modifier && forward && turnLeft) {
        moving = 1;
        angle = 45.0f - fldCamPitch;
    } else if (modifier && forward && right) {
        moving = 1;
        angle = -45.0f - fldCamPitch;
    } else if (!modifier && forward && turnRight) {
        moving = 1;
        angle = -45.0f - fldCamPitch;
    } else if (modifier && back && left) {
        moving = 1;
        angle = 135.0f - fldCamPitch;
    } else if (modifier && back && right) {
        moving = 1;
        angle = -135.0f - fldCamPitch;
    } else if (turnLeft) {
        moving = 1;
        angle = 90.0f - fldCamPitch;
    } else if (turnRight) {
        moving = 1;
        angle = -90.0f - fldCamPitch;
    } else if (forward) {
        moving = 1;
        angle = -fldCamPitch;
    } else if (back && !turnAround) {
        moving = 1;
        angle = 180.0f - fldCamPitch;
    } else {
        fldPlayerSyukanState = 0;
        fldPlayerMoveState = -1;
    }
    if (moving) {
        speed = 29.0f;
        motion = 1;
        if (fldGlobalBase.float130 > 0.0f) {
            speed = sdfFabs(fldGlobalBase.float130) / 40.0f + 2.0f + speed;
        }
        if (fldRunDanteGunFlag.word50 & 2) {
            speed *= fldSyukanGunSpeedScale;
        }
        rad = -angle * fldSyukanPi0 / 180.0f;
        position.x = SDF_FRAME_MOVE(0, fldGlobalBase.x - sdfSin(rad) * speed);
        position.y = fldGlobalBase.y;
        position.z = SDF_FRAME_MOVE(2, fldGlobalBase.z - sdfCos(rad) * speed);
        dds3SetPosPoseSet(control->handle, &position);
    } else {
        position.x = fldGlobalBase.x;
        position.y = fldGlobalBase.y;
        position.z = fldGlobalBase.z;
        dds3SetPosPoseSet(control->handle, &position);
    }
    if (l_UseCollision != 0) {
        dds3Basic_t *basic;
        dds3PoseSet_t *pose;
        if (forward || back || left || right || turnLeft || turnRight) {
            collisionPosition[0] = position.x;
            collisionPosition[1] = position.y;
            collisionPosition[2] = position.z;
            fldGlobalBase.x = position.x;
            fldGlobalBase.y = position.y;
            fldGlobalBase.z = position.z;
            fldHitChkUnit(collisionPosition);
            if (fldGlobalBase.hitValid != 0) {
                position.x = SDF_FRAME_MOVE_HIT(0, fldGlobalBase.hitX);
                position.z = SDF_FRAME_MOVE_HIT(2, fldGlobalBase.hitZ);
                position.y = SDF_FRAME_MOVE_HIT(1, fldGlobalBase.hitY + 44.0f);
                fldGlobalBase.x = position.x;
                fldGlobalBase.y = position.y;
                fldGlobalBase.z = position.z;
                dds3SetPosPoseSet(control->handle, &position);
                fldGlobalBase.hitValid = 0;
            }
            SDF_FRAME_MOVE_END(control->handle, position);
        }
        dds3GetPosPoseSet(control->handle);
        SDF_VU0_STOREVECTOR(&position);
        basic = control->handle;
        pose = basic->poseset;
        SDF_VU0_COPYVECTOR(&previous, &pose->pos2);
        if (sdfFabs(previous.x - position.x) < 1.0f) {
            pose->pos2.x = position.x;
        } else {
            pose->pos2.x = previous.x + (position.x - previous.x) / 3.0f;
        }
        if (sdfFabs(previous.y - position.y) < 1.0f) {
            basic->poseset->pos2.y = position.y;
        } else {
            basic->poseset->pos2.y = previous.y + (position.y - previous.y) / 3.0f;
        }
        if (sdfFabs(previous.z - position.z) < 1.0f) {
            basic->poseset->pos2.z = position.z;
        } else {
            basic->poseset->pos2.z = previous.z + (position.z - previous.z) / 3.0f;
        }
        basic->poseset->pos2.w = 1.0f;
    }
    fldHitChkUnitDamage(&position.x);
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    fldGlobalBase.camAngle2 = fldPlayerCalc360_8(angle);
    delta = R360_LR(fldGlobalBase.camAngle, angle);
    amount = delta / 6.0f;
    if (amount < 0.0f) {
        amount = -amount;
    }
    if (amount < 2.0f) {
        amount = 2.0f;
    }
    amount = SDF_FRAME_CHASE(amount, delta, 2.0f);
    {
        f32 result;
        if (delta >= 0.0f && delta <= amount) {
            result = angle;
        } else if (delta <= 0.0f && -amount <= delta) {
            result = angle;
        } else if (delta < 0.0f) {
            result = fldGlobalBase.camAngle - amount;
        } else {
            result = fldGlobalBase.camAngle + amount;
        }
        fldGlobalBase.camAngle = result;
    }
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(fldGlobalBase.camAngle * fldSyukanPi1 / 180.0f);
    SDF_VU0_STOREVECTOR(&rotation);
    dds3SetRotPoseSet(control->handle, &rotation);
    target = fldCamPitchTarget;
    delta = target - fldCamPitch;
    if (delta < 0.0f) {
        delta = -delta;
    }
    if (delta < 1.0f) {
        fldCamPitch = target;
        if (lookLeft || quickLeft) {
            target = fldPlayerCalc360_4(target - 50.0f);
            fldSyukanState = 2;
            fldCamPitchTarget = target;
        } else if (lookRight || quickRight) {
            target = fldPlayerCalc360_4(target + 50.0f);
            fldSyukanState = 2;
            fldCamPitchTarget = target;
        } else if (turnAround) {
            target = fldPlayerCalc360_4(target + 140.0f);
            fldSyukanState = 2;
            fldCamPitchTarget = target;
            delta = R360_LR(fldCamPitch, target);
            if ((s32)delta == -180) {
                delta = 180.0f;
            }
            amount = 10.0f;
            {
                f32 result;
                if (delta >= 0.0f && delta <= amount) {
                    result = target;
                } else if (delta <= 0.0f && -amount <= delta) {
                    result = target;
                } else if (delta < 0.0f) {
                    result = fldCamPitch - amount;
                } else {
                    result = fldCamPitch + amount;
                }
                fldCamPitch = result;
            }
            target = fldCamPitchTarget;
        }
    }
    delta = target - fldCamPitch;
    if (delta < 0.0f) {
        delta = -delta;
    }
    if (delta < 1.0f) {
        if (fldSyukanState > 0) {
            fldSyukanState--;
        }
    } else {
        f32 result;
        delta = R360_LR(fldCamPitch, target);
        amount = 10.0f;
        if (delta >= 0.0f && delta <= amount) {
            result = target;
        } else if (delta <= 0.0f && -amount <= delta) {
            result = target;
        } else if (delta < 0.0f) {
            result = fldCamPitch - amount;
        } else {
            result = fldCamPitch + amount;
        }
        fldCamPitch = result;
        target = fldCamPitchTarget;
    }
    fldCamPitchTarget = fldPlayerCalc360_8(target);
    fldGlobalBase.camAngle2 = -fldCamPitchTarget;
    fldGlobalBase.camAngle = -fldCamPitch;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(fldGlobalBase.camAngle * fldSyukanPi2 / 180.0f);
    SDF_VU0_STOREVECTOR(&rotation);
    dds3SetRotPoseSet(control->handle, &rotation);
    fldPlayerOn();
    if (fldRunDanteGunFlag.word50 & 2) {
        switch (motion) {
        case 1:
        case 4:
            motion = 17;
            break;
        case 8:
            motion = 18;
            break;
        }
    }
    fldPlayerMotion(oldMotion, motion, 4.0f);
    fldGlobalBase.x = position.x;
    fldGlobalBase.z = position.z;
    fldGlobalBase.y = position.y;
    fldPlayerMotionSe(oldMotion, motion);
    {
        dds3PoseSet_t *pose = control->handle->poseset;
        fldPlayerPosRotWork.pos.x = pose->pos2.x;
        fldPlayerPosRotWork.pos.y = pose->pos2.y;
        fldPlayerPosRotWork.pos.z = pose->pos2.z;
        fldPlayerPosRotWork.pos.w = pose->pos2.w;
        fldPlayerPosRotWork.rot.x = pose->rot.x;
        fldPlayerPosRotWork.rot.y = pose->rot.y;
        fldPlayerPosRotWork.rot.z = pose->rot.z;
        fldPlayerPosRotWork.rot.w = pose->rot.w;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Nml_syukan);
#endif

extern const char fldNoroiDamageOnMsg[];

extern const char fldNoroiDamageOffMsg[];

extern s32 fldEncountTestFlag(u32 mask);

extern void fldEncountSetFlag(s32 mask);

extern void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void fldDmgStart(f32 x, f32 y, f32 z);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);

extern void fldStatusSet(void);

extern void fldPlayerCalc_WarpRun(void);

extern void fldPlayerMotionSe(s32 hisMotion, s32 upMotion);

extern s32 fldPlayerCalc_Wm2(void);

extern s32 fldPlayerCalc_Nml(void);

extern s32 fldPlayerCalc_Nml_syukan(void);

/* Keep the mode-dispatch mirror check opaque to same-unit optimization.
 * The direct view merges its tail with the default arm; this canonical
 * same-symbol view retains the separate native call without changing ABI. */
extern s32 fldPlayerMirChkDispatch(void) FAKEMATCH_ASM_NAME("fldPlayerMirChk");

s32 fldPlayerCalc(void)
{
    fldUnitControl_t *ctl;
    mdlObj_t *mdl;
    s32 mode;
    /* The live motion-index comparison uses the native argument register;
     * its value is dead before any call and has no artificial consumer. */
    FAKEMATCH_ASM_REG(s32, motionIndex, "$4");

    if (fldRunDanteGunFlag.gunFlag != 0) {
        return 0;
    }
    if (fldAutoMapChkMode() != 0) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0 && fldGlobalBase.word80 > 0) {
        ctl = fldGetUnitControl();
        mdl = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
        motionIndex = mdl->current.h.motionIndex;
        if (motionIndex != 0xA) {
            printf(fldNoroiDamageOnMsg);
            mdl->first->speed = 1.0f;
            mdlSingleMotion(mdl, 0, 0xA);
            fldDmgStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
            fldSePlay(0x22);
            dds3PadVibrationSet(0, 1, 0x1E);
            dds3PadVibrationSet(1, 0x96, 0x1E);
            fldStatusSet();
        }
        if (mdl->first->stat == 5) {
            printf(fldNoroiDamageOffMsg);
            fldGlobalBase.word80 = 0;
            fldEncountSetFlag(0x40);
        } else {
            return 0;
        }
    }
    if (fldEncountTestFlag(0x40) == 0) {
        if (fldGlobalBase.area < 0xA) {
            if (fldWmWarpMode != 1) {
                ctl = fldGetUnitControl();
                mdl = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
                mdl->first->speed = 0.0f;
                if (evtBitCheck(0x75B) != 0) {
                    dds3ObjectBaseOnFlag(ctl->handle, 1);
                    return 0;
                }
                dds3ObjectBaseOffFlag(ctl->handle, 1);
                return 0;
            }
            fldPlayerCalc_Wm3();
            return 0;
        }
        mode = fldGlobalBase.mode;
        if (mode == 1 || mode == 3) {
            fldPlayerSvsOff();
        }
        fldPlayerCalcHasigo();
        fldPlayerCalc_Hit();
        fldPlayerCalc_WarpRun();
        ctl = fldGetUnitControl();
        mdl = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
        fldPlayerMotionSe(mdl->current.h.motionIndex, mdl->current.h.motionIndex);
        fldPlayerMirChk();
        return 0;
    }
    if (fldGlobalBase.area < 0xA) {
        return fldPlayerCalc_Wm2();
    }
    mode = fldGlobalBase.mode;
    fldWmWarpMode = 0;
    switch (mode) {
    case 1:
        fldPlayerSvsOff();
    case 3:
        fldPlayerCalc_Nml();
        fldPlayerMirChkDispatch();
        return 0;
    }
    if (evtBitCheck(0xAF0) != 0) {
        fldPlayerCalc_Nml_syukan();
    } else {
        fldPlayerCalc_Nml();
    }
    fldPlayerMirChk();
    return 0;
}

s32 fldPlayerChkAna(void)
{
    if (gfldPlayerHasiCnt == 0 && fldPlayerStepDist == 0 && gfldPlayerDamegeCnt == 0) {
        return 0;
    }
    return 1;
}

void fldPlayerOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOnFlag(ctl->handle, 1);
    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOnFlag(ctl->handle2, 1);
    }
    fldPlayerMotReset2();
}

void fldPlayerOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOffFlag(ctl->handle, 1);
    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOffFlag(ctl->handle2, 1);
    }
}

void fldPlayerSvsOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseSetControlBasicSvs(ctl->handle, 0);
    dds3ObjectBaseOffFlag(ctl->handle, 0x800);
}

void fldPlayerSvsOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseSetControlBasicSvs(ctl->handle, 0x80);
    dds3ObjectBaseOnFlag(ctl->handle, 0x800);
}

void fldPlayerAlpOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle))->inner->color = 0;
    if (ctl->handle2 != NULL) {
        ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2))->inner->color = 0;
    }
}

void fldPlayerAlpOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle))->inner->color = 0x80808080;
    if (ctl->handle2 != NULL) {
        ((mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle2))->inner->color = 0x80808080;
    }
}

void fldPlayerSdwOn(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    if (ctl->handle2 != NULL) {
        dds3ObjectBaseOnFlag(ctl->handle, 0x400);
        return;
    }
    dds3ObjectBaseOnFlag(ctl->handle, 0x200);
}

void fldPlayerSdwOff(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();

    dds3ObjectBaseOffFlag(ctl->handle, 0x400);
    dds3ObjectBaseOffFlag(ctl->handle, 0x200);
}

s32 fldPlayerFastSet(void)
{
    fldUnitControl_t *ctl = fldGetUnitControl();
    sdfVector4_t pos;
    f32 probe[3];
    dds3PoseSet_t *pose;
    /* Keep the entry area observation separate from the later publication
     * addresses. This reconstruction constraint ends before the pose getter;
     * the pointer is initialized below and never lives across a call. */
    FAKEMATCH_ASM_REG(fldAreaWork_t *, areaState, "$3");

    fldPlayerFastMode = 0;
    if (l_UseCollision == 0) {
        return 0;
    }
    areaState = &fldGlobalBase;
    if (areaState->area < 0xA) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        probe[0] = fldGlobalBase.x = pos.x;
        probe[1] = fldGlobalBase.y = pos.y + 20.0f;
        probe[2] = fldGlobalBase.z = pos.z;
        fldHitChkUnit(probe);
        if (fldGlobalBase.hitValid != 0) {
            fldGlobalBase.x = fldGlobalBase.hitX;
            if (fldGlobalBase.area == 1) {
                fldGlobalBase.y = fldGlobalBase.hitY + 44.0f - 11.0f;
            } else {
                fldGlobalBase.y = fldGlobalBase.hitY + 44.0f;
            }
            pos.x = fldGlobalBase.x;
            pos.y = fldGlobalBase.y;
            pos.z = fldGlobalBase.z = fldGlobalBase.hitZ;
            dds3SetPosPoseSet(ctl->handle, (const sdfVector4_t *)&pos);
            fldGlobalBase.hitValid = 0;
        }
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        pose = ctl->handle->poseset;
        SDF_VU0_COPYVECTOR((u8 *)pose + 0x70, &pos);
        dds3OffPoseSetFlag(pose, POSESET_FLAG_POS2);
    } else {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        probe[0] = fldGlobalBase.x = pos.x;
        probe[1] = fldGlobalBase.y = pos.y + 10.0f;
        probe[2] = fldGlobalBase.z = pos.z;
        fldHitChkUnit(probe);
        if (fldGlobalBase.hitValid != 0) {
            fldGlobalBase.x = fldGlobalBase.hitX;
            fldGlobalBase.y = fldGlobalBase.hitY + 44.0f;
            fldGlobalBase.z = fldGlobalBase.hitZ;
            pos.x = fldGlobalBase.x;
            pos.y = fldGlobalBase.y;
            pos.z = fldGlobalBase.z;
            dds3SetPosPoseSet(ctl->handle, (const sdfVector4_t *)&pos);
            fldGlobalBase.hitValid = 0;
        }
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(&pos);
        pose = ctl->handle->poseset;
        SDF_VU0_COPYVECTOR((u8 *)pose + 0x70, &pos);
        dds3OffPoseSetFlag(pose, POSESET_FLAG_POS2);
    }
    return 0;
}

void fldPlayerMotReset(void) {
    dds3Basic_t *unit;

    if (fldGlobalBase.area < 10) {
        return;
    }
    unit = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (unit == NULL) {
        return;
    }
    fldPlayerMotion(((mdlObj_t *)dds3UnitObjectBasicGetControlWorkData(unit)->pModel)->current.h.motionIndex,
                    9, 10.0f);
}

void fldPlayerMotReset2(void) {
    dds3Basic_t *unit;

    if (fldGlobalBase.area < 10) {
        return;
    }
    unit = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (unit == NULL) {
        return;
    }
    fldPlayerMotion(((mdlObj_t *)dds3UnitObjectBasicGetControlWorkData(unit)->pModel)->current.h.motionIndex,
                    9, 0.0f);
}

/* The native coordinate phases keep real scalar snapshots and pointer/call
 * arguments in distinct lifetimes. Local bindings and empty constraints retain
 * their allocation and access order; these are target reconstruction aids,
 * not evidence of the original source spelling. */
extern void dds3GetRotPoseSet(dds3Basic_t *basic);
extern f32 EulerAngleAxisY(void);
extern void dds3OnPoseSetFlag(dds3PoseSet_t *pose, u32 flag);


void fldPlayerPosRot(void)
{
    fldUnitControl_t *control = fldGetUnitControl();
    sdfVector4_t pos __attribute__((aligned(16)));
    sdfVector4_t rot __attribute__((aligned(16)));
    sdfVector4_t oldPos __attribute__((aligned(16)));
    dds3Basic_t *basic;
    dds3PoseSet_t *pose;
    f32 angle;

    if (control == NULL || control->handle == NULL) {
        return;
    }
    dds3ObjectBaseGetModelHandle(control->handle);
    dds3GetRotPoseSet(control->handle);
    angle = EulerAngleAxisY() * 180.0f;
    {
        FAKEMATCH_ASM_REG(dds3Basic_t *, positionHandle, "$4");
        FAKEMATCH_ASM_REG(fldAreaWork_t *, area, "$2");
        positionHandle = control->handle;
        angle = angle / 3.1399999f;
        area = &fldGlobalBase;
        FAKEMATCH_ASM_READ(area);
        area->camAngle2 = angle;
        FAKEMATCH_ASM_LIVE_F32(angle);
        area->camAngle = angle;
        dds3GetPosPoseSet(positionHandle);
    }
    SDF_VU0_STOREVECTOR(&pos);
    {
        FAKEMATCH_ASM_REG(f32, newX, "$f3") = pos.x;
        FAKEMATCH_ASM_REG(f32, oldX, "$f5") = fldGlobalBase.x;
        FAKEMATCH_ASM_REG(f32, oldY, "$f1") = fldGlobalBase.y;
        FAKEMATCH_ASM_REG(f32, oldZ, "$f2") = fldGlobalBase.z;
        fldGlobalBase.prevX = oldX;
        fldGlobalBase.prevY = oldY;
        fldGlobalBase.prevZ = oldZ;
        fldGlobalBase.x = newX;
        fldGlobalBase.y = pos.y;
        fldGlobalBase.z = pos.z;
    }
    dds3OnPoseSetFlag(control->handle->poseset, 1);
    dds3OnPoseSetFlag(control->handle->poseset, 8);
    if (control->handle2 != NULL) {
        dds3OnPoseSetFlag(control->handle2->poseset, 1);
        dds3OnPoseSetFlag(control->handle2->poseset, 8);
    }
    if (gfldPlayerDamegeCnt != 0) {
        basic = control->handle;
        pose = basic->poseset;
        SDF_VU0_COPYVECTOR(&pose->pos2, &pose->pos);
    } else {
        FAKEMATCH_ASM_REG(dds3Basic_t *, interpolationBasic, "$6");
        FAKEMATCH_ASM_REG(sdfVector4_t *, interpolationPos, "$3");
        FAKEMATCH_ASM_REG(sdfVector4_t *, oldPosition, "$4");
        fldPlayerSdwOn();
        basic = control->handle;
        oldPosition = &oldPos;
        pose = basic->poseset;
        interpolationBasic = basic;
        FAKEMATCH_ASM_LIVE(interpolationBasic);
        interpolationPos = &pose->pos2;
        FAKEMATCH_ASM_READ(interpolationPos);
        SDF_VU0_COPYVECTOR(oldPosition, interpolationPos);
        {
            f32 previousX = oldPos.x;
            f32 targetX = pos.x;
            FAKEMATCH_ASM_LIVE_F32_VOLATILE(targetX);
            if (sdfFabs(previousX - targetX) < 1.0f) {
                interpolationPos->x = targetX;
            } else {
                interpolationPos->x = previousX + (targetX - previousX) / 3.0f;
            }
        }
        if (sdfFabs(oldPos.y - pos.y) < 1.0f) {
            interpolationBasic->poseset->pos2.y = pos.y;
        } else {
            interpolationBasic->poseset->pos2.y = oldPos.y + (pos.y - oldPos.y) / 3.0f;
        }
        FAKEMATCH_ASM_LIVE(interpolationBasic);
        if (sdfFabs(oldPos.z - pos.z) < 1.0f) {
            interpolationBasic->poseset->pos2.z = pos.z;
        } else {
            interpolationBasic->poseset->pos2.z = oldPos.z + (pos.z - oldPos.z) / 3.0f;
        }
        FAKEMATCH_ASM_LIVE(interpolationBasic);
        {
            FAKEMATCH_ASM_REG(dds3PoseSet_t *, currentPose, "$2");
            currentPose = interpolationBasic->poseset;
            FAKEMATCH_ASM_READ(currentPose);
            currentPose->pos2.w = 1.0f;
        }
    }
    {
        FAKEMATCH_ASM_REG(dds3Basic_t *, otherHandle, "$3") = control->handle2;
        if (otherHandle != NULL) {
            FAKEMATCH_ASM_REG(dds3Basic_t *, callHandle, "$4") = otherHandle;
            FAKEMATCH_ASM_REG(dds3PoseSet_t *, other, "$3");
            FAKEMATCH_ASM_REG(dds3PoseSet_t *, sourcePose, "$2");
            FAKEMATCH_ASM_REG(sdfVector4_t *, callPos, "$5");
            sourcePose = basic->poseset;
            callPos = &pos;
            FAKEMATCH_ASM_READ_PAIR(sourcePose, callPos);
            other = otherHandle->poseset;
            FAKEMATCH_ASM_READ_PAIR(other, sourcePose);
            {
                FAKEMATCH_ASM_REG(f32, copyX, "$f1") = sourcePose->pos2.x;
                other->pos2.x = copyX;
            }
            other->pos2.y = sourcePose->pos2.y;
            other->pos2.z = sourcePose->pos2.z;
            other->pos2.w = sourcePose->pos2.w;
            other->pos.x = sourcePose->pos.x;
            other->pos.y = sourcePose->pos.y;
            other->pos.z = sourcePose->pos.z;
            other->pos.w = sourcePose->pos.w;
            other->rot.x = sourcePose->rot.x;
            other->rot.y = sourcePose->rot.y;
            other->rot.z = sourcePose->rot.z;
            other->rot.w = sourcePose->rot.w;
            rot.x = sourcePose->rot.x;
            rot.y = sourcePose->rot.y;
            rot.z = sourcePose->rot.z;
            rot.w = sourcePose->rot.w;
            dds3SetPosPoseSet(callHandle, callPos);
            dds3SetRotPoseSet(control->handle2, &rot);
            basic = control->handle;
        }
    }
    pose = basic->poseset;
    fldPlayerPosRotWork.pos.x = pose->pos2.x;
    fldPlayerPosRotWork.pos.y = pose->pos2.y;
    fldPlayerPosRotWork.pos.z = pose->pos2.z;
    fldPlayerPosRotWork.pos.w = pose->pos2.w;
    fldPlayerPosRotWork.rot.x = pose->rot.x;
    fldPlayerPosRotWork.rot.y = pose->rot.y;
    fldPlayerPosRotWork.rot.z = pose->rot.z;
    fldPlayerPosRotWork.rot.w = pose->rot.w;
}
const sdfVector4Unaligned_t fldWm3Scale = {1.0f, 1.0f, 1.0f, 1.0f};

const sdfVector4Unaligned_t fldWm4Scale = {1.0f, 1.0f, 1.0f, 1.0f};

const sdfVector4Unaligned_t fldWm2Scale = {1.0f, 1.0f, 1.0f, 1.0f};

const sdfVector4Unaligned_t fldWm2WorkInit = {0.0f, 1.0f, 0.0f, 1.0f};

const sdfVector4Unaligned_t fldNmlSyukanAxis = {0.0f, 1.0f, 0.0f, 1.0f};

const char fldNoroiDamageOnMsg[24] = "noroi damage on\n";

const char fldNoroiDamageOffMsg[32] = "noroi damage off\n";

f32 fldPlayerWarpState = 0.0f;

s32 fldPlayerFastMode = 0;

s32 gfldPlayerHasiCnt = 0;

s32 gfldPlayerDamegeCnt = 0;

s32 fldPlayerStepDist = 0;

s32 fldPlayerMoveState = 0;

s32 fldPlayerSyukanState = 0;

u32 fldPlayerMoveStateSaved = 0;

s32 fldWmapMatterLast = -1;

s32 fldWmapBlendStep = 0;

s32 fldWmWarpMode = 0;

f32 fldWmapPlayerState = 1.0f;

f32 fldWmFastMove = 0.0f;

s32 fldSyukanState = 0;

s32 fldSkyEnable = 0;

