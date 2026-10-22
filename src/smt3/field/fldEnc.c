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

typedef struct frFontGlyph_t frFontGlyph_t;

struct frFontGlyph_t *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri,
    u32 Color, const u8 *pStr, struct frFontGlyph_t *pFRQ);

f32 sdfAtan2(f32 aY, f32 aX);

dds3Basic_t *dds3GetActiveScene(void);

extern s32 gencStageInnAlp;

extern s32 encDrawState;

extern void *hPartialMultiBlur;

extern char D_0054EC80[];

extern void dds3EffectPartialMultiBlurDelete(void *hPartialMultiBlur);

extern s32 encDrawProc(dds3ProcessID_t *aProcId);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit, void *deleteExit,
                                         void *p);

extern s32 gencOn;

extern s32 gencMode;

extern void rsetOtOfs(s32 otofs);

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);

void fldDrawBokasi(void);

void fldDrawFrameSpriteAlpha(s32 alpha);

s32 nbGetEncountOn(void);

void rsetAlpha(s32 mode);

void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern dds3Effect_PartialMultiBlurParam_t D_0055DAD8;

extern void dds3EffectPartialMultiBlurSetParam(void *hPartialMultiBlur, dds3Effect_PartialMultiBlurParam_t *pParam);

extern void dds3EffectPartialMultiBlurDraw(void *blur);

extern void encFreeBlur(void);

s32 encDrawProc(dds3ProcessID_t *aProcId) {
    if (gencOn == 0) {
        return 0;
    }
    if (gencMode == 0) {
        rsetOtOfs(0x43);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        rsetAlpha(0);
        fldDrawBokasi();
        aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
        rsetAlpha(0);
        if (encDrawState >= 13) {
            D_0055DAD8.Blur.scale = D_0055DAD8.Blur.scale + 0.001f;
            D_0055DAD8.area = D_0055DAD8.area - 2;
            dds3EffectPartialMultiBlurSetParam(hPartialMultiBlur, &D_0055DAD8);
            dds3EffectPartialMultiBlurDraw(hPartialMultiBlur);
        }
        if (encDrawState >= 0x29) {
            rsetOtOfs(0x47);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 2);
        } else if (encDrawState >= 0x15) {
            rsetOtOfs(0x47);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            aDrawBoxF(0, 0, 0x200, 0xE0, 0, 0, 0, 2);
        }
        encDrawState = encDrawState + 1;
    }
    if (gencMode == 1) {
        if (gencStageInnAlp > 0) {
            rsetOtOfs(0x51);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            fldDrawFrameSpriteAlpha(gencStageInnAlp);
            gencStageInnAlp = gencStageInnAlp - 2;
        } else {
            encFreeBlur();
        }
    }
    return 0;
}

void encFreeBlur(void)
{
    gencOn = 0;
    if (hPartialMultiBlur != NULL) {
        dds3EffectPartialMultiBlurDelete(hPartialMultiBlur);
        hPartialMultiBlur = NULL;
    }
}

extern dds3Effect_PartialMultiBlurParam_t D_0055DAD8;

extern effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);

extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 visible);

void encStart(void) {
    if (nbGetEncountOn() != 0) {
        return;
    }
    gencStageInnAlp = 0x80;
    D_0055DAD8.Blur.abgr = 0x09808080;
    D_0055DAD8.Blur.blend = 0x44;
    D_0055DAD8.Blur.rotate = 0.01f;
    D_0055DAD8.Blur.scale = 1.02f;
    D_0055DAD8.area = 0x12C;
    gencMode = 0;
    encDrawState = 0;
    D_0055DAD8.Blur.center[0] = 0;
    D_0055DAD8.Blur.center[1] = 0;
    D_0055DAD8.Blur.xy[0][0] = 0;
    D_0055DAD8.Blur.xy[0][1] = 0;
    D_0055DAD8.Blur.xy[1][0] = 0;
    D_0055DAD8.Blur.xy[1][1] = 0;
    hPartialMultiBlur = dds3EffectPartialMultiBlurCreate(&D_0055DAD8);
    gencOn = 1;
    if (dds3GetActiveScene() == NULL) {
        return;
    }
    dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
}

void encInitProcess(void)
{
    dds3StartProcess(D_0054EC80, 0x2B0F, 0, 1, encDrawProc, NULL, NULL);
}

f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    f32 dx;
    f32 dz;
    f32 result;
    f32 absZ;
    f32 angle;
    f32 dist;

    if (fromX == toX && fromZ == toZ) return 0.0f;
    result = fromX - toX;
    dx = result;
    dz = fromZ - toZ;
    absZ = dz;
    if (result < 0.0f) result = -result;
    if (absZ < 0.0f) absZ = -absZ;
    angle = sdfAtan2(absZ, result) * (180.0f / 3.14f);
    if (dx < 0.0f) {
        if (!(dz < 0.0f)) angle = -angle;
    } else {
        if (dz < 0.0f) angle = 180.0f - angle;
        else angle += 180.0f;
    }
    result = angle;
    return result;
}
INCLUDE_SDATA(const s32, "field/fldEnc", gencStageInnAlp);

INCLUDE_SDATA(const s32, "field/fldEnc", encDrawState);

INCLUDE_SDATA(const s32, "field/fldEnc", gencOn);

INCLUDE_SDATA(const s32, "field/fldEnc", D_0054EC7C);

INCLUDE_SDATA(const s32, "field/fldEnc", D_0054EC80);

