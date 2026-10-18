#include "common.h"

extern u32 dds3DrawFlag;
extern u8 fadergba[4];
extern u16 fadenow;
extern u16 fademax;

typedef struct dds3Filter2Param {
    /* 0x00 */ u8 rgba[4];
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 rect[4];
} dds3Filter2Param_t;

extern void *dds3LightParam[8];
extern u8 dds3Filter2Buf[0x18];
extern dds3Filter2Param_t filter2target;
extern u16 filter2now;
extern u16 filter2max;
extern u16 filter2abase;
extern u16 filter2atarget;
extern u16 filter2swapnow;
extern u16 filter2swapmax;

typedef struct dds3MFilterParam {
    /* 0x00 */ s32 area[3];
    /* 0x0C */ u8 rgba[4];
} dds3MFilterParam_t;

typedef struct dds3Blur2Param {
    /* 0x00 */ u8 rgba[4];
    /* 0x04 */ s32 unk04[3];
    /* 0x10 */ s32 area[2];
    /* 0x18 */ s32 rect[4];
} dds3Blur2Param_t;

extern s32 dds3MFilterBuf[9];
extern dds3MFilterParam_t mfiltertarget;
extern u16 mfilternow;
extern u16 mfiltermax;
extern u16 mfilterabase;
extern u16 mfilteratarget;
extern u16 mfilterswapnow;
extern u16 mfilterswapmax;

typedef struct dds3RipBlurParam {
    /* 0x00 */ s32 count;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ u8 rgba[4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ s32 x;
    /* 0x24 */ s32 y;
    /* 0x28 */ s32 area;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 blur2buf[10];
} dds3RipBlurParam_t;

extern dds3RipBlurParam_t ripblurtarget;

typedef struct dds3MultiBlurParam {
    /* 0x00 */ s32 extent;
    /* 0x04 */ u8 rgba[4];
    /* 0x08 */ s32 blend;
    /* 0x0C */ f32 rotation;
    /* 0x10 */ f32 scale;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 unk1C[4];
} dds3MultiBlurParam_t;

typedef struct dds3DistBlurParam {
    /* 0x00 */ s32 unk00[3];
    /* 0x0C */ u8 rgba[4];
    /* 0x10 */ s32 unk10[3];
    /* 0x1C */ s32 x;
    /* 0x20 */ s32 y;
    /* 0x24 */ s32 spread;
    /* 0x28 */ s32 unk28;
} dds3DistBlurParam_t;

extern s32 dds3MultiBlurBuf[11];
extern dds3MultiBlurParam_t mulblurtarget;
extern u16 mulblurnow; extern u16 mulblurmax; extern u16 mulblurabase;
extern u16 mulbluratarget; extern u16 mulblurswapnow; extern u16 mulblurswapmax;

extern s32 dds3DistBlurBuf[11];
extern dds3DistBlurParam_t distblurtarget;
extern u16 distblurnow; extern u16 distblurmax; extern u16 distblurabase;
extern u16 distbluratarget; extern u16 distblurswapnow; extern u16 distblurswapmax;
extern u32 distblurovercnt;

extern s32 dds3RipBlurBuf[11];
extern u16 ripblurnow; extern u16 ripblurmax; extern u16 ripblurabase;
extern u16 ripbluratarget; extern u16 ripblurswapnow; extern u16 ripblurswapmax;
extern u32 ripblurovercnt;

s32 *Eve_PartialMultiBlurGetParams(void);
void Eve_PartialMultiBlurSetParam(void *param);
void Eve_PartialMultiBlurON(void);
void Eve_PartialMultiBlurOFF(void);
s32 *Eve_DistortionBlurGetParams(void);
void Eve_DistortionBlurSetParam(void *param);
void Eve_DistortionBlurON(void);
void Eve_DistortionBlurOFF(void);
s32 *Eve_RippleBlurGetParams(void);
void Eve_RippleBlurSetParam(void *param);
void Eve_RippleBlurON(void);
void Eve_RippleBlurOFF(void);
extern dds3Blur2Param_t blur2target;
extern u16 blur2now;
extern u16 blur2max;
extern u16 blur2abase;
extern u16 blur2atarget;
extern u16 blur2swapnow;
extern u16 blur2swapmax;

extern u16 dds3ScreenBlurAlpha;
extern u16 dds3ScreenBlurScale;
extern u8 dds3ScreenBlurFlag;
extern u16 blurnow;
extern u16 blurmax;
extern u16 bluralphabase;
extern u16 blurscalebase;
extern u16 bluralphatarget;
extern u16 blurscaletarget;

s32 *Eve_PartialMultiFilterGetParams(void);
void Eve_PartialMultiFilterSetParam(void *param);
void Eve_PartialMultiFilterON(void);
void Eve_PartialMultiFilterOFF(void);
s32 *Eve_BlurGetParam(void);
void Eve_BlurSetParam(void *param);
void Eve_BlurON(void);
void Eve_BlurOFF(void);

void setdraw_xy(s32 *rect);

extern f32 dds3FogParam[4];
extern f32 dds3FogColor[4];
extern u16 fogparanow; extern u16 fogparamax;
extern f32 sdfFaceDivSoften; extern f32 sdfFaceDivBias;
extern u16 sbnow; extern u16 sbmax;
extern f32 softenbase; extern f32 softentarget;
extern f32 biasbase; extern f32 biastarget;

extern u32 dds3RenderTex;
extern u32 dds3RenderTexHandle;
extern void *dds3RenderTexture;
extern void *D_00551BA8;

void sdfTexFree(void *tex);
void *sdfMatNew(void);
void *frwGetSmallFontFRQ(s32 x, s32 y, s32 arg2, s32 color, const char *text, void *arg5);
void frDrawFont(void *req);
void frDestroyFontRequestAfter(void *req);
void *Eve_FilterGetParams(void);
void Eve_FilterSetParam(void *param);
void Eve_FilterON(void);
void Eve_FilterOFF(void);
void *memcpy(void *dst, const void *src, u32 len);

void dds3FadeClear(void);
s32 dds3FadeCheck(void);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitDrawEnvSub);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3AllocRenderTexture);

void dds3FreeRenderTexture(void)
{
    if (dds3RenderTexture != NULL) {
        sdfTexFree(dds3RenderTexture);
    }
    dds3RenderTexture = NULL;
    dds3RenderTexHandle = 0;
    dds3RenderTex = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RenderTextureON);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RenderTextureOFF);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3CheckRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3GetRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RunQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3StopQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitZClearDrawTag);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3LinkZClearDrawTag);

void dds3FadeClear(void)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = 0;
    fadergba[1] = 0;
    fadergba[2] = 0;
    fadergba[3] = 0;
}

void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha)
{
    dds3DrawFlag &= ~3;
    fadergba[0] = red;
    fadergba[1] = green;
    fadergba[2] = blue;
    fadergba[3] = alpha;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeColorSet);

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 frame)
{
    fadergba[0] = red;
    fadergba[1] = green;
    fadergba[2] = blue;
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = frame;
    fadenow = frame;
    dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
}

void dds3FadeIn2(s32 frame)
{
    fadergba[3] = -128;
    if (frame == 0) {
        fadergba[3] = 0;
        dds3DrawFlag &= ~3;
        fadenow = 0;
        fademax = 0;
    } else {
        fademax = frame;
        fadenow = frame;
        dds3DrawFlag = (dds3DrawFlag | 1) & ~2;
    }
}

void dds3FadeOut(s32 red, s32 green, s32 blue, s32 frame)
{
    fadergba[0] = red;
    fadergba[1] = green;
    fadergba[2] = blue;
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fademax = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        return;
    }
    fademax = frame;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
}

void dds3FadeOut2(s32 frame)
{
    fadergba[3] = 0;
    if (frame == 0) {
        fadenow = 0;
        fadergba[3] = -128;
        dds3DrawFlag &= ~3;
        fademax = 0;
    } else {
        fademax = frame;
        fadenow = 0;
        dds3DrawFlag = (dds3DrawFlag & ~1) | 2;
    }
}

s32 dds3FadeCheck(void)
{
    return (dds3DrawFlag & 3) != 0;
}

void dds3FadeUpdate(void)
{
    if (dds3FadeCheck() != 0) {
        if (dds3DrawFlag & 1) {
            fadenow = fadenow - 1;
        } else {
            fadenow = fadenow + 1;
        }
        fadergba[3] = (fadenow << 7) / fademax;
        if (((dds3DrawFlag & 1) && fadenow == 0) || ((dds3DrawFlag & 2) && fadenow == fademax)) {
            fadenow = 0;
            fademax = 0;
            dds3DrawFlag &= ~3;
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawDistRipOverError);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawFade);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetScreenFovy);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetAmbient);

void dds3SetFog(s32 *param)
{
    memcpy(&dds3LightParam[8], param, 0x14);
}

void dds3SetLight(void *light, s32 index)
{
    dds3LightParam[index] = light;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", setdraw_xy);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFilter2Para);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFilter2RGBA);

void dds3Filter2Swap(s32 frame)
{
    memcpy(dds3Filter2Buf, Eve_FilterGetParams(), 0x18);
    filter2swapnow = 0;
    filter2swapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x100000;
        setdraw_xy(filter2target.rect);
        Eve_FilterSetParam(&filter2target);
    } else {
        dds3DrawFlag |= 0x100000;
    }
}

void dds3Filter2On(s32 frame)
{
    filter2abase = 0;
    filter2now = 0;
    filter2atarget = filter2target.rgba[3];
    filter2max = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x80000;
        setdraw_xy(filter2target.rect);
        Eve_FilterSetParam(&filter2target);
        Eve_FilterON();
    } else {
        dds3DrawFlag |= 0x80000;
    }
}

void dds3Filter2Off(s32 frame)
{
    filter2atarget = 0;
    filter2abase = filter2target.rgba[3];
    filter2now = 0;
    filter2max = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x80000;
        dds3DrawFlag &= ~0x100000;
        Eve_FilterOFF();
    } else {
        dds3DrawFlag |= 0x80000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMFilterPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMFilterRGBA);

void dds3SetMFilterArea(s32 x, s32 y, s32 z)
{
    mfiltertarget.area[0] = x;
    mfiltertarget.area[1] = y;
    mfiltertarget.area[2] = z;
}

void dds3MFilterSwap(s32 frame)
{
    memcpy(dds3MFilterBuf, Eve_PartialMultiFilterGetParams(), 0x24);
    mfilterswapnow = 0;
    mfilterswapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x400000;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
    } else {
        dds3DrawFlag |= 0x400000;
    }
}

void dds3MFilterOn(s32 frame)
{
    mfilterabase = 0;
    mfilternow = 0;
    mfilteratarget = mfiltertarget.rgba[3];
    mfiltermax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x200000;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
        Eve_PartialMultiFilterON();
    } else {
        dds3DrawFlag |= 0x200000;
    }
}

void dds3MFilterOff(s32 frame)
{
    mfilteratarget = 0;
    mfilterabase = mfiltertarget.rgba[3];
    mfilternow = 0;
    mfiltermax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x200000;
        dds3DrawFlag &= ~0x400000;
        Eve_PartialMultiFilterOFF();
    } else {
        dds3DrawFlag |= 0x200000;
    }
}

void dds3SetBlur(s32 frame, s32 alpha, s32 scale)
{
    if (frame == 0) {
        dds3ScreenBlurAlpha = alpha;
        dds3ScreenBlurScale = scale;
        if (alpha == 0 && scale == 0) {
            dds3ScreenBlurFlag = 0;
        } else {
            dds3ScreenBlurFlag = 1;
        }
        dds3DrawFlag &= ~0x800;
    } else {
        bluralphabase = dds3ScreenBlurAlpha;
        blurscalebase = dds3ScreenBlurScale;
        bluralphatarget = alpha;
        blurscaletarget = scale;
        blurmax = frame;
        blurnow = 0;
        dds3DrawFlag |= 0x800;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur2Para);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur2RGBA);

void dds3SetBlur2Area(s32 x, s32 y)
{
    blur2target.area[0] = x;
    blur2target.area[1] = y;
}

void dds3Blur2Swap(s32 frame)
{
    memcpy(ripblurtarget.blur2buf, Eve_BlurGetParam(), 0x28);
    blur2swapnow = 0;
    blur2swapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x40000;
        setdraw_xy(blur2target.rect);
        Eve_BlurSetParam(&blur2target);
    } else {
        dds3DrawFlag |= 0x40000;
    }
}

void dds3Blur2On(s32 frame)
{
    blur2abase = 0;
    blur2now = 0;
    blur2atarget = blur2target.rgba[3];
    blur2max = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        setdraw_xy(blur2target.rect);
        Eve_BlurSetParam(&blur2target);
        Eve_BlurON();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}

void dds3Blur2Off(s32 frame)
{
    blur2atarget = 0;
    blur2abase = blur2target.rgba[3];
    blur2now = 0;
    blur2max = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x20000;
        dds3DrawFlag &= ~0x40000;
        Eve_BlurOFF();
    } else {
        dds3DrawFlag |= 0x20000;
    }
}

void dds3SetMultiBlurPara(f32 rotation, f32 scale, s32 blend)
{
    mulblurtarget.rotation = rotation;
    mulblurtarget.scale = scale;
    mulblurtarget.blend = blend;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMultiBlurRGBA);

void dds3SetMultiBlurArea(s32 extent, s32 x, s32 y)
{
    mulblurtarget.extent = extent;
    mulblurtarget.x = x;
    mulblurtarget.y = y;
}

void dds3MultiBlurSwap(s32 frame)
{
    memcpy(dds3MultiBlurBuf, Eve_PartialMultiBlurGetParams(), 0x2C);
    mulblurswapnow = 0;
    mulblurswapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x8000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
    } else {
        dds3DrawFlag |= 0x8000;
    }
}

void dds3MultiBlurOn(s32 frame)
{
    mulblurabase = 0;
    mulblurnow = 0;
    mulbluratarget = mulblurtarget.rgba[3];
    mulblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
        Eve_PartialMultiBlurON();
    } else {
        dds3DrawFlag |= 0x1000;
    }
}

void dds3MultiBlurOff(s32 frame)
{
    mulbluratarget = 0;
    mulblurnow = 0;
    mulblurabase = mulblurtarget.rgba[3];
    mulblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000;
        dds3DrawFlag &= ~0x8000;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
        Eve_PartialMultiBlurOFF();
    } else {
        dds3DrawFlag |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetDistBlurPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetDistBlurRGBA);

void dds3SetDistBlurArea(s32 spread, s32 x, s32 y)
{
    distblurtarget.spread = spread;
    distblurtarget.x = x;
    distblurtarget.y = y;
}

void dds3DistBlurSwap(s32 frame)
{
    memcpy(dds3DistBlurBuf, Eve_DistortionBlurGetParams(), 0x2C);
    distblurswapnow = 0;
    distblurswapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x10000;
        Eve_DistortionBlurSetParam(&distblurtarget);
    } else {
        dds3DrawFlag |= 0x10000;
    }
}

void dds3DistBlurOn(s32 frame)
{
    distblurabase = 0;
    distblurnow = 0;
    distbluratarget = distblurtarget.rgba[3];
    distblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurON();
    } else {
        dds3DrawFlag |= 0x2000;
        Eve_DistortionBlurSetParam(&distblurtarget);
        Eve_DistortionBlurOFF();
    }
}

void dds3DistBlurOff(s32 frame)
{
    distbluratarget = 0;
    distblurabase = distblurtarget.rgba[3];
    distblurnow = 0;
    distblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x2000;
        dds3DrawFlag &= ~0x10000;
        Eve_DistortionBlurOFF();
    } else {
        dds3DrawFlag |= 0x2000;
    }
}

void dds3SetRipBlurPara(s32 count, s32 blend, f32 phase, f32 spacing, f32 angle, f32 amp, f32 anglestep)
{
    if (count >= 0x29) {
        ripblurovercnt++;
        count = 0x28;
    }
    ripblurtarget.count = count;
    ripblurtarget.unk04 = phase;
    ripblurtarget.unk08 = spacing;
    ripblurtarget.unk14 = angle;
    ripblurtarget.unk18 = amp;
    ripblurtarget.unk1C = anglestep;
    ripblurtarget.unk10 = blend;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetRipBlurRGBA);

void dds3SetRipBlurArea(s32 size, s32 x, s32 y)
{
    ripblurtarget.area = size;
    ripblurtarget.x = x;
    ripblurtarget.y = y;
}

void dds3RipBlurSwap(s32 frame)
{
    memcpy(dds3RipBlurBuf, Eve_RippleBlurGetParams(), 0x2C);
    ripblurswapnow = 0;
    ripblurswapmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x1000000;
        Eve_RippleBlurSetParam(&ripblurtarget);
    } else {
        dds3DrawFlag |= 0x1000000;
    }
}

void dds3RipBlurOn(s32 frame)
{
    ripblurabase = 0;
    ripblurnow = 0;
    ripbluratarget = ripblurtarget.rgba[3];
    ripblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x800000;
        Eve_RippleBlurSetParam(&ripblurtarget);
        Eve_RippleBlurON();
    } else {
        dds3DrawFlag |= 0x800000;
        Eve_RippleBlurSetParam(&ripblurtarget);
        Eve_RippleBlurOFF();
    }
}

void dds3RipBlurOff(s32 frame)
{
    ripbluratarget = 0;
    ripblurabase = ripblurtarget.rgba[3];
    ripblurnow = 0;
    ripblurmax = frame;
    if (frame == 0) {
        dds3DrawFlag &= ~0x800000;
        dds3DrawFlag &= ~0x1000000;
        Eve_RippleBlurOFF();
    } else {
        dds3DrawFlag |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BlurUpdate);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RotationArc);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightDir);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetAmbientColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFogColor);

void dds3SetFogPara(s32 frame, f32 x, f32 y, f32 z, f32 w)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x400;
        ((f32 *)&dds3LightParam[8])[0] = x;
        ((f32 *)&dds3LightParam[8])[1] = y;
        ((f32 *)&dds3LightParam[8])[2] = z;
        ((f32 *)&dds3LightParam[8])[3] = w;
    } else {
        f32 px = ((f32 *)&dds3LightParam[8])[0];
        f32 py = ((f32 *)&dds3LightParam[8])[1];
        f32 pz = ((f32 *)&dds3LightParam[8])[2];
        f32 pw = ((f32 *)&dds3LightParam[8])[3];
        dds3DrawFlag |= 0x400;
        fogparamax = frame;
        dds3FogParam[0] = px;
        dds3FogParam[1] = py;
        dds3FogParam[2] = pz;
        dds3FogParam[3] = pw;
        dds3FogColor[0] = x;
        dds3FogColor[1] = y;
        dds3FogColor[2] = z;
        dds3FogColor[3] = w;
        fogparanow = 0;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SceneLightUpdateFirst);

void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x4000;
        sdfFaceDivSoften = soften;
        sdfFaceDivBias = bias;
    } else {
        f32 psoften = sdfFaceDivSoften;
        f32 pbias = sdfFaceDivBias;
        dds3DrawFlag |= 0x4000;
        sbmax = frame;
        softenbase = psoften;
        softentarget = soften;
        biasbase = pbias;
        biastarget = bias;
        sbnow = 0;
    }
}

void eglInit(void)
{
    if (D_00551BA8 == NULL) {
        D_00551BA8 = sdfMatNew();
        *(f32 *)((u8 *)D_00551BA8 + 0x1C) = 1.0f;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglSetOt);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkTest);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkAlpha);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkSprite);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkBox);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkTri);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkPoly4);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkTexPoly4);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkLine);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkPrint);

void eglLinkKanji(s32 x, s32 y, s32 color, const char *text)
{
    void *req = frwGetSmallFontFRQ(x << 4, y << 3, 0, color, text, NULL);

    frDrawFont(req);
    frDestroyFontRequestAfter(req);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", egl3Dto2D);
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", egl_OT_Offset);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0054E942);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackColor);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTex);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTexHandle);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTexture);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTexWidth);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTexHeight);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderDrawEnv);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3DrawFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0054E961);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurAlpha);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurScale);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0054E966);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurOfsX);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurOfsY);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3QuakeFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3QuakeFrame);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3QuakeXRange);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3QuakeYRange);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", fadergba);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3FadeColorG);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3FadeColorB);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3FadeState);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", distblurovercnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", ripblurovercnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0054E984);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0054E988);

