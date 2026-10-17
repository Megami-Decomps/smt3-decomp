#include "common.h"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitDrawEnvSub);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3AllocRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FreeRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RenderTextureON);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RenderTextureOFF);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3CheckRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3GetRenderTexture);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RunQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3StopQuake);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitZClearDrawTag);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3LinkZClearDrawTag);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeClear);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeSet);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeColorSet);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeIn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeIn2);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeOut);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeOut2);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeCheck);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FadeUpdate);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawDistRipOverError);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawFade);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetScreenFovy);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetAmbient);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFog);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLight);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", setdraw_xy);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFilter2Para);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFilter2RGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Filter2Swap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Filter2On);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Filter2Off);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMFilterPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMFilterRGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMFilterArea);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MFilterSwap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MFilterOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MFilterOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur2Para);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur2RGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetBlur2Area);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Blur2Swap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Blur2On);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3Blur2Off);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMultiBlurPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMultiBlurRGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetMultiBlurArea);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MultiBlurSwap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MultiBlurOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3MultiBlurOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetDistBlurPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetDistBlurRGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetDistBlurArea);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DistBlurSwap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DistBlurOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DistBlurOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetRipBlurPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetRipBlurRGBA);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetRipBlurArea);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RipBlurSwap);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RipBlurOn);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RipBlurOff);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3BlurUpdate);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RotationArc);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightDir);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetAmbientColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFogColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetFogPara);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SceneLightUpdateFirst);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetSoftenBias);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglInit);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", eglLinkKanji);

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

