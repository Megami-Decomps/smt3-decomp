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

struct frFontGlyph_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 depth,
    u32 colors, const u8 *text, struct frFontGlyph_t *parent);

f32 sdfAtan2(f32 y, f32 x);

dds3Basic_t *dds3GetActiveScene(void);

extern s32 gencStageInnAlp;

extern s32 encDrawState;

extern void *hPartialMultiBlur;

extern char D_0054EC80[];

extern void dds3EffectPartialMultiBlurDelete(void *p);

extern s32 encDrawProc(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 flag, dds3ProcessFunc_t func, void *arg,
                                         void *parent);

extern s32 gencOn;

extern s32 gencMode;

extern void rsetOtOfs(s32 ofs);

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void fldDrawBokasi(void);

void fldDrawFrameSpriteAlpha(s32 alpha);

s32 nbGetEncountOn(void);

void rsetAlpha(s32 mode);

void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17);

extern effBlurTemplateBody_t D_0055DAD8;

extern void dds3EffectPartialMultiBlurSetParam(void *blur, effBlurTemplateBody_t *body);

extern void dds3EffectPartialMultiBlurDraw(void *blur);

extern void encFreeBlur(void);

s32 encDrawProc(dds3ProcessID_t *id) {
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
            D_0055DAD8.source.displacement = D_0055DAD8.source.displacement + 0.001f;
            D_0055DAD8.extent = D_0055DAD8.extent - 2;
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

extern effBlurTemplateBody_t D_0055DAD8;

extern effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const effBlurTemplateBody_t *body);

extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 flag);

void encStart(void) {
    if (nbGetEncountOn() != 0) {
        return;
    }
    gencStageInnAlp = 0x80;
    D_0055DAD8.source.color = 0x09808080;
    D_0055DAD8.source.blendControl = 0x44;
    D_0055DAD8.source.angle = 0.01f;
    D_0055DAD8.source.displacement = 1.02f;
    D_0055DAD8.extent = 0x12C;
    gencMode = 0;
    encDrawState = 0;
    D_0055DAD8.source.position[0] = 0;
    D_0055DAD8.source.position[1] = 0;
    D_0055DAD8.source.corners[0][0] = 0;
    D_0055DAD8.source.corners[0][1] = 0;
    D_0055DAD8.source.corners[1][0] = 0;
    D_0055DAD8.source.corners[1][1] = 0;
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

