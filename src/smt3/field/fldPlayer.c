#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
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

void dds3SetPosPoseSet(dds3Basic_t *pBasic, u128 *pos);

dds3Basic_t *dds3GetActiveScene(void);

extern f32 R360_LR(f32 a, f32 b);

extern f32 fabsf(f32);

extern u32 fldRunDanteGunFlag[];

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern s32 fldAutoMapChkMode(void);

extern s32 fldGb __attribute__((section(".data")));

s32 evtBitCheck(s32 no);

void fldPlayerOff(void);

extern f32 fldWmFastMove;

extern s32 gfldPlayerHasiCnt;

extern s32 gfldPlayerDamegeCnt;

extern s32 fldPlayerStepDist;

extern s32 fldWmWarpMode;

/* fldUnitControl_t as fldGetUnitControl returns it. */
typedef struct {
    dds3Basic_t *handle;
    dds3Basic_t *handle2;
    s8 moveFlag;
    s8 rotFlag;
} fldUnitControl_t;

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

/* The world-map player scale triple, copied as one word-aligned 16-byte block. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} fldScaleVec_t;

extern fldScaleVec_t D_00531D08;

extern fldScaleVec_t D_00531D18;

extern s32 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, s32 flag);

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
    f32 pos[4];

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
        SDF_VU0_STOREVECTOR(pos);
        if (pos[1] > -2.5f && pos[1] < 2.5f) {
            dds3ObjectBaseOffFlag(control->handle2, 1);
        } else {
            dds3ObjectBaseOnFlag(control->handle2, 1);
        }
    }
    return 0;
}

extern s32 fldPlayerStatus[];

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
        if (fldPlayerStatus[0] & 2) {
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
extern f32 D_002FC9C0[];
extern void fldSePlay(s32 id);
extern void fldSeVol(s32 id, s32 volume);
extern u32 dds3GetRandA(s32 stream);

/* NON_MATCHING: 363 of 364 words match at the native 1456-byte size. */
#ifdef NON_MATCHING
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
            dx = fldCamTarget[0] - D_002FC9C0[0];
            dy = fldCamTarget[1] - D_002FC9C0[1];
            dz = fldCamTarget[2] - D_002FC9C0[2];
            distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 500.0f;
            if (distance > 7000.0f) distance = 7000.0f;
            if (distance < 0.0f) distance = 0.0f;
            volume = ((7000 - (s32)distance) * 127) / 7000;
            if (volume > 127) volume = 127;
            if (volume < 0) volume = 0;
        } else {
            volume = 127;
            volume -= (dds3GetRandA(0) >> 2) & 0x3F;
            if (volume > 127) volume = 127;
            if (volume < 0) volume = 0;
        }
        if (hisMotion != 0 && hisMotion != 8 && hisMotion != 9 && upMotion == 8) {
            fldSeVol(left + 3, volume);
        } else {
            left += (dds3GetRandA(0) >> 4) % 3;
            right += (dds3GetRandA(0) >> 4) % 3;
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
                volume -= dds3GetRandA(0) % 31;
                if (volume > 127) volume = 127;
                if (volume < 0) volume = 0;
                if (before < 1.0f && now >= 1.0f) fldSeVol(0x24, volume);
                if (before < 18.0f && now >= 18.0f) fldSeVol(0x37, volume);
            } else if (upMotion == 15) {
                volume = 127;
                volume -= dds3GetRandA(0) % 31;
                if (volume > 127) volume = 127;
                if (volume < 0) volume = 0;
                if (before < 1.0f && now >= 1.0f) fldSeVol(0x24, volume);
                if (before < 18.0f && now >= 18.0f) fldSeVol(0x37, volume);
            }
        }
        /* Retail keeps jal + epilogue here. Qualified compiler trace: this
         * reset survives sibling selection and is deleted by life analysis.
         * See docs/nonmatching/field/fldPlayer.md. */
        volume = 0;
    }
}
#else
INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerMotionSe);
#endif

void fldPlayerCalc_Hit(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    f32 pos[4];
    f32 probe[4];

    if (ctl == NULL) {
        return;
    }
    if (fldPlayerFastMode == 1) {
        return;
    }
    if (l_UseCollision == 0) {
        return;
    }
    probe[0] = fldGlobalBase.x;
    probe[1] = fldGlobalBase.y;
    probe[2] = fldGlobalBase.z;
    fldHitChkUnitElv(probe);
    if (fldGlobalBase.hitValid == 0) {
        return;
    }
    fldGlobalBase.x = fldGlobalBase.hitX;
    fldGlobalBase.y = fldGlobalBase.hitY;
    fldGlobalBase.z = fldGlobalBase.hitZ;
    pos[0] = fldGlobalBase.hitX;
    pos[1] = fldGlobalBase.hitY;
    pos[2] = fldGlobalBase.hitZ;
    dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    fldGlobalBase.hitValid = 0;
    dds3GetPosPoseSet(ctl->handle);
    SDF_VU0_STOREVECTOR(pos);
    SDF_VU0_COPYVECTOR((u8 *)ctl->handle->poseset + 0x70, pos);
}

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_WarpRun);

/* NON_MATCHING: 14 of 58 words differ (first at +0x44). */
#ifdef NON_MATCHING
s32 fldPlayerCalcHasigo(void) {
    fldUnitControl_t *ctl = fldGetUnitControl();
    f32 pos[4];

    if (gfldPlayerDamegeCnt > 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pos[1] -= 2.0f;
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1];
        fldGlobalBase.z = pos[2];
        pos[1] = fldGlobalBase.y;
        pos[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    } else if (gfldPlayerDamegeCnt < 0) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pos[1] += 2.0f;
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1];
        fldGlobalBase.z = pos[2];
        pos[1] = fldGlobalBase.y;
        pos[2] = fldGlobalBase.z;
        dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
    } else {
        return 0;
    }
    fldPlayerSdwOff();
    fldPlayerSvsOff();
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalcHasigo);
#endif

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531C50);

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531C60);

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Nml);

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
    fldScaleVec_t scale = D_00531D08;
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
    fldScaleVec_t scale = D_00531D18;
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

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Wm2);

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc_Nml_syukan);

extern s32 printf(const char *format, ...);

extern char D_00531D58[];

extern char D_00531D70[];

extern s32 fldEncountTestFlag(s32 mask);

extern void fldEncountSetFlag(s32 mask);

extern void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void fldDmgStart(f32 x, f32 y, f32 z);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);

extern void fldStatusSet(void);

extern void fldPlayerCalc_WarpRun(void);

extern void fldPlayerMotionSe(s32 hisMotion, s32 upMotion);

extern s32 fldPlayerCalc_Wm2(void);

extern void fldPlayerCalc_Nml(void);

extern void fldPlayerCalc_Nml_syukan(void);

/* NON_MATCHING: 17 of 169 words differ (first at +0x64). */
#ifdef NON_MATCHING
s32 fldPlayerCalc(void)
{
    fldUnitControl_t *ctl;
    mdlObj_t *mdl;
    s32 mode;

    if (fldRunDanteGunFlag[0] != 0) {
        return 0;
    }
    if (fldAutoMapChkMode() != 0) {
        return 0;
    }
    if (fldEncountTestFlag(0x40) == 0 && fldGlobalBase.word80 > 0) {
        ctl = fldGetUnitControl();
        mdl = (mdlObj_t *)dds3ObjectBaseGetModelHandle(ctl->handle);
        if (mdl->current.h.motionIndex != 0xA) {
            printf(D_00531D58);
            mdl->first->speed = 1.0f;
            mdlSingleMotion(mdl, 0, 0xA);
            fldDmgStart(fldGlobalBase.x, fldGlobalBase.y, fldGlobalBase.z);
            fldSePlay(0x22);
            dds3PadVibrationSet(0, 1, 0x1E);
            dds3PadVibrationSet(1, 0x96, 0x1E);
            fldStatusSet();
        }
        if (mdl->first->stat == 5) {
            printf(D_00531D70);
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
    if (mode == 1) {
        fldPlayerSvsOff();
        goto nml;
    }
    if (mode == 3) {
    nml:
        fldPlayerCalc_Nml();
        fldPlayerMirChk();
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
#else
INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531D08);

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531D18);

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531D28);

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531D38);

INCLUDE_RODATA(const s32, "field/fldPlayer", D_00531D48);

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerCalc);
#endif

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

/* NON_MATCHING: 121 of 144 words differ (first at +0x28). */
#ifdef NON_MATCHING
s32 fldPlayerFastSet(void)
{
    fldUnitControl_t *ctl = fldGetUnitControl();
    f32 pos[4];
    f32 probe[3];
    dds3PoseSet_t *pose;

    fldPlayerFastMode = 0;
    if (l_UseCollision == 0) {
        return 0;
    }
    if (fldGlobalBase.area < 0xA) {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        probe[0] = pos[0];
        probe[1] = pos[1] + 20.0f;
        probe[2] = pos[2];
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1] + 20.0f;
        fldGlobalBase.z = pos[2];
        fldHitChkUnit(probe);
        if (fldGlobalBase.hitValid != 0) {
            fldGlobalBase.x = fldGlobalBase.hitX;
            if (fldGlobalBase.area == 1) {
                fldGlobalBase.y = fldGlobalBase.hitY + 44.0f - 11.0f;
            } else {
                fldGlobalBase.y = fldGlobalBase.hitY + 44.0f;
            }
            pos[0] = fldGlobalBase.x;
            pos[1] = fldGlobalBase.y;
            pos[2] = fldGlobalBase.hitZ;
            fldGlobalBase.z = fldGlobalBase.hitZ;
            dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
            fldGlobalBase.hitValid = 0;
        }
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pose = ctl->handle->poseset;
        SDF_VU0_COPYVECTOR((u8 *)pose + 0x70, pos);
        dds3OffPoseSetFlag(pose, POSESET_FLAG_POS2);
    } else {
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        fldGlobalBase.prevX = fldGlobalBase.x;
        fldGlobalBase.prevY = fldGlobalBase.y;
        fldGlobalBase.prevZ = fldGlobalBase.z;
        probe[0] = pos[0];
        probe[1] = pos[1] + 10.0f;
        probe[2] = pos[2];
        fldGlobalBase.x = pos[0];
        fldGlobalBase.y = pos[1] + 10.0f;
        fldGlobalBase.z = pos[2];
        fldHitChkUnit(probe);
        if (fldGlobalBase.hitValid != 0) {
            fldGlobalBase.x = fldGlobalBase.hitX;
            fldGlobalBase.y = fldGlobalBase.hitY + 44.0f;
            fldGlobalBase.z = fldGlobalBase.hitZ;
            pos[0] = fldGlobalBase.x;
            pos[1] = fldGlobalBase.y;
            pos[2] = fldGlobalBase.z;
            dds3SetPosPoseSet(ctl->handle, (u128 *)pos);
            fldGlobalBase.hitValid = 0;
        }
        dds3GetPosPoseSet(ctl->handle);
        SDF_VU0_STOREVECTOR(pos);
        pose = ctl->handle->poseset;
        SDF_VU0_COPYVECTOR((u8 *)pose + 0x70, pos);
        dds3OffPoseSetFlag(pose, POSESET_FLAG_POS2);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerFastSet);
#endif

void fldPlayerMotReset(void) {
    dds3Basic_t *unit;

    if (fldGb < 10) {
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

    if (fldGb < 10) {
        return;
    }
    unit = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (unit == NULL) {
        return;
    }
    fldPlayerMotion(((mdlObj_t *)dds3UnitObjectBasicGetControlWorkData(unit)->pModel)->current.h.motionIndex,
                    9, 0.0f);
}

INCLUDE_ASM(const s32, "field/fldPlayer", fldPlayerPosRot);
INCLUDE_SDATA(const s32, "field/fldPlayer", fldPlayerWarpState);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldPlayerFastMode);

INCLUDE_SDATA(const s32, "field/fldPlayer", gfldPlayerHasiCnt);

INCLUDE_SDATA(const s32, "field/fldPlayer", gfldPlayerDamegeCnt);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldPlayerStepDist);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldPlayerMoveState);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldPlayerSyukanState);

INCLUDE_SDATA(const s32, "field/fldPlayer", D_0054ECFC);

INCLUDE_SDATA(const s32, "field/fldPlayer", D_0054ED00);

INCLUDE_SDATA(const s32, "field/fldPlayer", D_0054ED04);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldWmWarpMode);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldWmapPlayerState);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldWmFastMove);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldSyukanState);

INCLUDE_SDATA(const s32, "field/fldPlayer", fldSkyEnable);

