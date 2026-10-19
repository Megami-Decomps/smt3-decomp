#include "common.h"
#include "draw.h"
#include "draw3d.h"
#include "ps2asm.h"

/* One A+D register command with its standard DMA/GIF header. */
typedef struct
{
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t write;
} dds3GsCommand_t;

/* Each GIF reglist loop contains an RGBA register followed by an XYZ register. */
typedef struct
{
    u64 low;
    u64 high;
} dds3GouraudRegister_t;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} dds3PrintControl_t;

/* sdfGrSetStdDrawEnv and sdfGrLinkStdDrawEnv are handed the DMA tag that
 * prefixes the draw env, so the env's own packet header sits 0x10 in. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGrDrawEnv_t env;
    u8 unk180[0xA0];
} dds3StdDrawEnv_t;

/* One frame buffer's draw bank, stride 0x1BE0. Three draw tags: the VU1 init
 * chain at 0x0, the scene chain at 0x12D0, and the second-screen chain at
 * 0x1A10. */
typedef struct
{
    sdfDrawTag_t initTag;
    sdf3DInitPacket_t initPacket;
    u8 unk0040[0x1290];
    sdfDrawTag_t drawTag;
    sdfPreDrawTag_t preDrawTag;
    dds3StdDrawEnv_t stdEnv;
    sdf3DViewportPacket_t viewport;
    sdf3DLightParallelPacket_t light;
    sdfDrawTag_t renderTag;
    sdfGrDrawEnv_t renderEnv;
    sdf3DViewportPacket_t renderViewport;
    sdf3DLightParallelPacket_t renderLight;
    sdfDrawTag_t subTag;
    sdf3DViewportPacket_t subViewport;
    sdf3DLightParallelPacket_t subLight;
} dds3DrawBank_t;

/* Two-context Z-clear packets are 0xB0 bytes, two buffers per state. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t testGif;
    sdfGrTagStdReg_t test;
    sdfGifTag_t clearGif;
    sdfGrTagStdReg_t colorA;
    sdfGrTagStdReg_t positionA;
    sdfGrTagStdReg_t colorB;
    sdfGrTagStdReg_t positionB;
    sdfGifTag_t blendGif;
    sdfGrTagStdReg_t blend[2];
} dds3ZClearPacket_t;


extern u32 dds3DrawFlag;
extern u8 fadergba[4];
/* The fade sprite packet: a four-register state block then a textureless
 * sprite, 0xB0 bytes, one per frame buffer. */
typedef struct
{
    sdfDmaTag_t dma;
    sdfGifTag_t stateGif;
    sdfGrTagStdReg_t state[4];
    sdfGifTag_t spriteGif;
    sdfGrTagStdReg_t sprite[4];
} dds3FadePacket_t;

extern dds3FadePacket_t dds3FadePacket[2];
extern u16 fadenow;
extern u16 fademax;

typedef struct dds3Filter2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ s32 rect[4];
} dds3Filter2Param_t;

/* Native offsets: parallel lights 0x00, ambient 0x10, fog 0x20. */
typedef struct
{
    sdf3DLight_t *parallel[4];
    sdfVector4_t ambient;
    sdf3DFogParam_t fog;
} dds3LightState_t;

typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
} dds3FogRange_t;

extern dds3LightState_t dds3LightParam;
/* Native fog setters address the record at +0x20 as a separate base.
 * This typed view allocates no storage; the enclosing light state is unchanged. */
PS2_SYMBOL_AT(sdf3DFogParam_t, dds3FogState, dds3LightParam, 0x20);
extern dds3Filter2Param_t dds3Filter2Buf;
extern dds3Filter2Param_t filter2target;
extern u16 filter2now;
extern u16 filter2max;
extern u16 filter2abase;
extern u16 filter2atarget;
extern u16 filter2swapnow;
extern u16 filter2swapmax;

typedef struct dds3MFilterParam {
    /* 0x00 */ s32 area[3];
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ s32 rect[4];
} dds3MFilterParam_t;

typedef struct dds3Blur2Param {
    /* 0x00 */ union { u8 rgba[4]; u32 color; };
    /* 0x04 */ s32 blend;
    /* 0x08 */ f32 rotation;
    /* 0x0C */ f32 scale;
    /* 0x10 */ s32 area[2];
    /* 0x18 */ s32 rect[4];
} dds3Blur2Param_t;

extern dds3MFilterParam_t dds3MFilterBuf;
extern dds3MFilterParam_t mfiltertarget;
extern u16 mfilternow;
extern u16 mfiltermax;
extern u16 mfilterabase;
extern u16 mfilteratarget;
extern u16 mfilterswapnow;
extern u16 mfilterswapmax;

typedef struct dds3RipBlurBody {
    /* 0x00 */ s32 count;
    /* 0x04 */ f32 phaseStep;
    /* 0x08 */ f32 spacing;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ f32 angleStep;
    /* 0x20 */ s32 x;
    /* 0x24 */ s32 y;
    /* 0x28 */ s32 area;
} dds3RipBlurBody_t __attribute__((aligned(8)));

typedef struct dds3RipBlurParam {
    /* 0x00 */ dds3RipBlurBody_t ripple;
    /* 0x2C */ u32 unk2C;
    /* 0x30 */ dds3Blur2Param_t blur2buf;
} dds3RipBlurParam_t;

extern dds3RipBlurParam_t ripblurtarget;

typedef struct dds3MultiBlurParam {
    /* 0x00 */ s32 extent;
    /* 0x04 */ union { u8 rgba[4]; u32 color; };
    /* 0x08 */ s32 blend;
    /* 0x0C */ f32 rotation;
    /* 0x10 */ f32 scale;
    /* 0x14 */ s32 x;
    /* 0x18 */ s32 y;
    /* 0x1C */ s32 rect[4];
} dds3MultiBlurParam_t;

typedef struct dds3DistBlurParam {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 delaySpread;
    /* 0x08 */ f32 angleStep;
    /* 0x0C */ union { u8 rgba[4]; u32 color; };
    /* 0x10 */ s32 blend;
    /* 0x14 */ f32 uvAngle;
    /* 0x18 */ f32 uvAmplitude;
    /* 0x1C */ s32 x;
    /* 0x20 */ s32 y;
    /* 0x24 */ s32 spread;
    /* 0x28 */ s32 size;
} dds3DistBlurParam_t;

extern dds3MultiBlurParam_t dds3MultiBlurBuf;
extern dds3MultiBlurParam_t mulblurtarget;
extern u16 mulblurnow; extern u16 mulblurmax; extern u16 mulblurabase;
extern u16 mulbluratarget; extern u16 mulblurswapnow; extern u16 mulblurswapmax;

extern dds3DistBlurParam_t dds3DistBlurBuf;
extern dds3DistBlurParam_t distblurtarget;
extern u16 distblurnow; extern u16 distblurmax; extern u16 distblurabase;
extern u16 distbluratarget; extern u16 distblurswapnow; extern u16 distblurswapmax;
extern u32 distblurovercnt;

extern dds3RipBlurBody_t dds3RipBlurBuf;
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

extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern u8 dds3ScreenBlurFlag;
extern u16 blurnow;
extern u16 blurmax;
extern s16 bluralphabase;
extern s16 blurscalebase;
extern s16 bluralphatarget;
extern s16 blurscaletarget;

s32 *Eve_PartialMultiFilterGetParams(void);
void Eve_PartialMultiFilterSetParam(void *param);
void Eve_PartialMultiFilterON(void);
void Eve_PartialMultiFilterOFF(void);
s32 *Eve_BlurGetParam(void);
void Eve_BlurSetParam(void *param);
void Eve_BlurON(void);
void Eve_BlurOFF(void);

void setdraw_xy(s32 *rect);

extern dds3FogRange_t dds3FogParam;
extern dds3FogRange_t dds3FogColor;
extern u16 fogparanow; extern u16 fogparamax;
extern f32 sdfFaceDivSoften; extern f32 sdfFaceDivBias;
extern u16 sbnow; extern u16 sbmax;
extern f32 softenbase; extern f32 softentarget;
extern f32 biasbase; extern f32 biastarget;

extern u32 dds3RenderTex;
extern sdfGsMemSlice_t *dds3RenderTexHandle;
extern sdfTexHandle_t *dds3RenderTexture;
extern sdfMat_t *D_00551BA8;

void sdfTexFree(sdfTexHandle_t *tex);
sdfMat_t *sdfMatNew(void);
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

extern sdf3DScreenParam_t dds3ScreenParam;
extern sdf3DScreenParam_t dds3ScreenParam2;
extern u8 dds3DrawProcessFlag;
extern u16 dds3QuakeFlag;
extern s16 dds3QuakeFrame;
extern u16 dds3QuakeXRange;
extern u16 dds3QuakeYRange;
extern u32 egl_OT_Offset;
extern f32 dds3QuakeOffset[4];

/* The quake's two axis gains are copied in by value, which is the unaligned
 * eight-byte block move at the entry. */
typedef struct
{
    f32 axis[2];
} dds3QuakeGain_t;

extern dds3QuakeGain_t D_0054E988[];
f32 dds3GetRandFloatA(s32 kind);
void sdfGrSetScreen(s32 screen);
void sdf3DMakeScreenParam(sdf3DScreenParam_t *param);
void dds3InitDrawEnvSub(s32 buffer);
void dds3InitZClearDrawTag(void);
void sdf3DLinkInitPacket(sdfDrawTag_t *tag, sdf3DInitPacket_t *packet);
void sdfPreDrawTagInit(sdfPreDrawTag_t *tag);
void sdfGrSetStdDrawEnv(dds3StdDrawEnv_t *env, sdfGrFrameSet_t *frameSet);
void sdfGrLinkStdDrawEnv(sdfDrawTag_t *tag, sdfPreDrawTag_t *preTag, dds3StdDrawEnv_t *env);
extern sdfGrFrameSet_t D_00529388;
extern sdfMatrix4_t aProjMatrix;
extern dds3LightState_t dds3LightParam2;
/* Retail addresses each light record's ambient term as its own symbol, so the
 * parallel array and the ambient reach sdf3DMakeLightParallelPacket apart. */
PS2_SYMBOL_AT(sdfVector4_t, dds3AmbientColor, dds3LightParam, 0x10);
PS2_SYMBOL_AT(sdfVector4_t, dds3AmbientColor2, dds3LightParam2, 0x10);
extern u32 dds3BackColor;
void eglInit(void);

extern sdfGrOt_t sdfGrOtTable[];
extern u64 D_002F5970[];
extern f32 D_002F5980[][4];
extern u32 D_002F59A0[];
extern sdf3DScreenParam_t D_002F5B80;
extern sdf3DLight_t *D_002F5D10[];
extern sdfVector4_t D_002F5D20;
extern sdfMatrix4_t D_002FA240;
extern sdfMatrix4_t D_002FA2A0;
extern sdfGsMemSlice_t *D_00529398[];
extern sdfGrFrameSet_t dds3RenderTexId;
extern dds3DrawBank_t dds3DrawEnv[];
extern dds3ZClearPacket_t dds3ZClearDrawTag[3][2];
extern u16 dds3RenderTexWidth;
extern u16 dds3RenderTexHeight;
extern u32 dds3RenderDrawEnv;
extern u32 dds3DebugSign;
extern s16 lightcolornow[4];
extern s16 lightcolormax[4];
extern s16 lightdirnow[4];
extern s16 lightdirmax[3];
extern u32 dds3LightColor[];
extern u32 D_00557230[];
extern f32 D_00557240[][4];
extern f32 D_00557270[][4];
extern u16 ambientnow, ambientmax;
extern u32 ambientbase, ambienttarget;
extern u16 fogrgbnow, fogrgbmax;
extern u32 fogrgbbase, fogrgbtarget;
extern dds3MultiBlurParam_t key2;
extern dds3DistBlurParam_t D_00556BC0;
extern dds3RipBlurParam_t dds3BlurState;
/* The rectangular frame body follows the ripple body in the same storage.
 * Bind its typed view without defining or extending the imported BSS symbol. */
PS2_SYMBOL_AT(dds3Blur2Param_t, dds3Blur2State, dds3BlurState, 0x30);
extern dds3Filter2Param_t dds3BlurParam;
extern dds3MFilterParam_t D_00556C70;

void *memset(void *dest, s32 value, u32 size);
f32 fabsf(f32 value);
u32 dds3GetFrameID(void);
void dds3FreeRenderTexture(void);
sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
void *sdfGrAllocPacketBuff(u32 size);
void sdfDrawTagInit(sdfDrawTag_t *tag);
void sdfDrawTagLink(sdfDrawTag_t *tag, void *packet);
void *sdf2DMakeStdPacket(void *packet, s32 size);
s32 sdf2DGetPrimPacketSize(s32 registers, s32 loops);
void sdf2DMakePrimPacketHeader(void *packet, s32 primitive, s32 registers, s32 registerList, s32 loops);
void *sdf2DGetPrimPtr(void *packet);
void sdf2DLinkTexPacket(sdfDrawTag_t *tag, sdfTexHandle_t *texture, s32 context);
void sdfPrimLinkGt4(sdfDrawTag_t *tag, s32 primitive,
    s32 x0, s32 y0, s32 u0, s32 v0, u32 color0,
    s32 x1, s32 y1, s32 u1, s32 v1, u32 color1,
    s32 x2, s32 y2, s32 u2, s32 v2, u32 color2,
    s32 x3, s32 y3, s32 u3, s32 v3, u32 color3, s32 depth, void *extra);
void sdf3DPrimLinkResetTm(sdfDrawTag_t *tag, s32 mode);
void sdf3DPrimLinkMat(sdfDrawTag_t *tag, sdfMat_t *material, s32 mode);
void *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *param);
void sdfMatSetTex(sdfMat_t *material, sdfTexHandle_t *texture);
void sdfMatFreeDelayed(sdfMat_t *material);
void *effDebugDrawWireModel(f32 vertices[][4], u32 *colors, s32 count, s32 alpha);
void sdfPrimSetOpaquePacket(void *packet);
void sdfDevSetPrintCB(dds3PrintControl_t *control, s32 x, s32 y, s32 z, u32 color);
void *sdfDevPrintfEx(dds3PrintControl_t *control, const char *format, ...);
void *sdfDevPrintf(s32 x, s32 y, s32 z, u32 color, const char *format, ...);
void sdfGrSetDrawEnv(sdfGrDrawEnv_t *env, sdfGrFrameSet_t *frameSet, s32 buffer);
void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *packet, sdf3DScreenParam_t *screen, sdfMatrix4_t *matrix);
void sdf3DMakeLightParallelPacket(sdf3DLightParallelPacket_t *packet, sdf3DLight_t **lights, sdfVector4_t *ambient);
sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
sdfTexHandle_t *sdfTexNew(s32 width, s32 height, s32 format, s32 mipmapCount,
    sdfGsMemSlice_t *pixels, s32 clutFormat, s32 clutCount, sdfGsMemSlice_t *clut);
void sdfTexSetWrapMode(sdfTexHandle_t *texture, s32 mode);
void sdfTexMakeGIFPacket(sdfTexHandle_t *texture);
void MultMatrixRev(sdfMatrix4_t *matrix);
void Normalize(void);
void dds3RotationArc(void);
void evtHokanBlur2(s32 enabled, f32 ratio, dds3Blur2Param_t *from, dds3Blur2Param_t *to, dds3Blur2Param_t *out);
void evtHokanMultBlur(s32 enabled, f32 ratio, dds3MultiBlurParam_t *from, dds3MultiBlurParam_t *to, dds3MultiBlurParam_t *out);
void evtHokanDistBlur(s32 enabled, f32 ratio, dds3DistBlurParam_t *from, dds3DistBlurParam_t *to, dds3DistBlurParam_t *out);
void evtHokanRipBlur(s32 enabled, f32 ratio, dds3RipBlurBody_t *from, dds3RipBlurBody_t *to, dds3RipBlurBody_t *out);
void evtHokanFilter(s32 enabled, f32 ratio, dds3Filter2Param_t *from, dds3Filter2Param_t *to, dds3Filter2Param_t *out);
void evtHokanMultFilter(s32 enabled, f32 ratio, dds3MFilterParam_t *from, dds3MFilterParam_t *to, dds3MFilterParam_t *out);

void dds3InitDrawEnvSub(s32 buffer)
{
    sdfDrawTagInit(&dds3DrawEnv[buffer].initTag);
    sdf3DLinkInitPacket(&dds3DrawEnv[buffer].initTag, &dds3DrawEnv[buffer].initPacket);
    sdfDrawTagInit(&dds3DrawEnv[buffer].drawTag);
    sdfPreDrawTagInit(&dds3DrawEnv[buffer].preDrawTag);
    sdfGrSetStdDrawEnv(&dds3DrawEnv[buffer].stdEnv, &D_00529388);
    /* The byte offset is what associates the sum as base plus index; the
     * member path reaches stdEnv.env.clear.clearColor and associates it the
     * other way round. */
    *(u32 *)((u8 *)&dds3DrawEnv[buffer] + 0x1410) = dds3BackColor;
    sdfGrLinkStdDrawEnv(&dds3DrawEnv[buffer].drawTag, &dds3DrawEnv[buffer].preDrawTag,
        &dds3DrawEnv[buffer].stdEnv);
    sdf3DMakeViewportPacket(&dds3DrawEnv[buffer].viewport, &dds3ScreenParam, &D_002FA240);
    sdfDrawTagLink(&dds3DrawEnv[buffer].drawTag, &dds3DrawEnv[buffer].viewport);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[buffer].light, dds3LightParam.parallel,
        &dds3AmbientColor);
    sdfDrawTagLink(&dds3DrawEnv[buffer].drawTag, &dds3DrawEnv[buffer].light);
    sdfDrawTagInit(&dds3DrawEnv[buffer].subTag);
    sdf3DMakeViewportPacket(&dds3DrawEnv[buffer].subViewport, &dds3ScreenParam2, &aProjMatrix);
    sdfDrawTagLink(&dds3DrawEnv[buffer].subTag, &dds3DrawEnv[buffer].subViewport);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[buffer].subLight, dds3LightParam2.parallel,
        &dds3AmbientColor2);
    sdfDrawTagLink(&dds3DrawEnv[buffer].subTag, &dds3DrawEnv[buffer].subLight);
}

void dds3InitDrawEnv(void)
{
    sdfGrSetScreen(1);
    sdf3DMakeScreenParam(&dds3ScreenParam);
    sdf3DMakeScreenParam(&dds3ScreenParam2);
    dds3InitDrawEnvSub(0);
    dds3InitDrawEnvSub(1);
    dds3InitZClearDrawTag();
    dds3DrawProcessFlag = 0;
    eglInit();
}

void dds3InitRenderDrawEnv(void)
{
    s32 i;

    dds3RenderTexId.w = dds3RenderTexWidth;
    dds3RenderTexId.h = dds3RenderTexHeight;
    dds3RenderTexId.buffH = dds3RenderTexHeight;
    dds3RenderTexId.pixelFormat = 0;
    dds3RenderTexId.zbuffFormat = 0x30;
    dds3RenderTexId.frameHandle[0] = dds3RenderTexHandle;
    dds3RenderTexId.frameHandle[1] = NULL;
    dds3RenderTexId.zbuffHandle = D_00529398[0];
    sdf3DMakeScreenParam(&D_002F5B80);
    for (i = 0; i != 2; i++) {
        sdfDrawTagInit(&dds3DrawEnv[i].renderTag);
        sdfGrSetDrawEnv(&dds3DrawEnv[i].renderEnv, &dds3RenderTexId, 0);
        dds3DrawEnv[i].renderEnv.clear.clearColor = dds3RenderDrawEnv;
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, &dds3DrawEnv[i].renderEnv);
        sdf3DMakeViewportPacket(&dds3DrawEnv[i].renderViewport, &D_002F5B80, &D_002FA2A0);
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, &dds3DrawEnv[i].renderViewport);
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[i].renderLight, D_002F5D10, &D_002F5D20);
        sdfDrawTagLink(&dds3DrawEnv[i].renderTag, &dds3DrawEnv[i].renderLight);
    }
}

/* Replacing a render texture releases the old texture first. A texture-creation
 * failure leaves the allocated framebuffer handle tracked, as in retail. */
s32 dds3AllocRenderTexture(u16 width, u16 height, f32 aspect)
{
    sdfTexHandle_t *texture;

    if (dds3RenderTexture != NULL) {
        dds3FreeRenderTexture();
    }
    dds3RenderTex = 0;
    dds3RenderTexWidth = width;
    dds3RenderTexHeight = height;
    dds3RenderTexHandle = sdfGsAllocFrameBuff(width, height, 0);
    if (dds3RenderTexHandle == NULL) {
        return 0;
    }
    texture = sdfTexNew(width, height, 0, 0, dds3RenderTexHandle, 0, 0, NULL);
    if (texture == NULL) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, 5);
    sdfTexMakeGIFPacket(texture);
    D_002F5B80.aspect = aspect;
    D_002F5B80.width = width;
    D_002F5B80.height = height;
    dds3InitRenderDrawEnv();
    return 1;
}

void dds3FreeRenderTexture(void)
{
    if (dds3RenderTexture != NULL) {
        sdfTexFree(dds3RenderTexture);
    }
    dds3RenderTexture = NULL;
    dds3RenderTexHandle = 0;
    dds3RenderTex = 0;
}

s32 dds3RenderTextureON(void)
{
    s32 result = 0;

    if (dds3RenderTexture == 0) {
        return result;
    }
    dds3RenderTex = 1;
    return 1;
}

void dds3RenderTextureOFF(void)
{
    dds3RenderTex = 0;
}

u32 dds3CheckRenderTexture(void)
{
    return dds3RenderTex;
}

void *dds3GetRenderTexture(void)
{
    return dds3RenderTexture;
}

void dds3RunQuake(void)
{
    f32 range[4];
    dds3QuakeGain_t gain = D_0054E988[0];
    f32 amplitude;
    f32 kick;
    s32 i;

    if (dds3QuakeFlag == 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(dds3QuakeOffset);
        return;
    }
    if (dds3QuakeFrame == 0) {
        dds3QuakeFlag = 0;
        return;
    }
    range[0] = dds3QuakeXRange * 0.01f;
    range[1] = dds3QuakeYRange * 0.01f;
    for (i = 0; i < 2; i++) {
        if (range[i] != 0.0f) {
            amplitude = dds3GetRandFloatA(0) * range[i] * 0.5f + range[i] * 0.5f;
            if (range[i] * gain.axis[i] <= fabsf(dds3QuakeOffset[i])) {
                if (dds3QuakeOffset[i] <= 0.0f) {
                    dds3QuakeOffset[i] = dds3QuakeOffset[i] + amplitude;
                } else {
                    dds3QuakeOffset[i] = dds3QuakeOffset[i] - amplitude;
                }
            } else {
                kick = amplitude * (dds3GetRandFloatA(0) - 0.5f);
                dds3QuakeOffset[i] = dds3QuakeOffset[i] + (kick + kick);
            }
            if (dds3QuakeOffset[i] < -range[i]) {
                dds3QuakeOffset[i] = -range[i];
            }
            if (range[i] < dds3QuakeOffset[i]) {
                dds3QuakeOffset[i] = range[i];
            }
        }
    }
    if (dds3QuakeFrame > 0) {
        s32 divisor = dds3QuakeFrame + 5;
        dds3QuakeXRange -= dds3QuakeXRange / divisor;
        dds3QuakeYRange -= dds3QuakeYRange / divisor;
        dds3QuakeFrame--;
    }
}

/* Zero amplitude stops the quake; zero duration leaves it running indefinitely. */
void dds3SetQuake(s32 duration, s32 amplitude)
{
    if (amplitude == 0) {
        dds3QuakeFlag = 0;
        return;
    }
    dds3QuakeFlag = 1;
    if (duration == 0) {
        dds3QuakeFrame = -1;
    } else {
        dds3QuakeFrame = duration;
    }
    dds3QuakeYRange = amplitude * 10;
    dds3QuakeXRange = dds3QuakeYRange * 4 / 10;
}

void dds3StopQuake(void)
{
    dds3QuakeFlag = 0;
}

/* NON_MATCHING: 21 of 87 words differ (first at +0x14). */
#ifdef NON_MATCHING
void dds3InitZClearDrawTag(void)
{
    s32 state;
    s32 buffer;

    for (state = 0; state < 3; state++) {
        for (buffer = 0; buffer < 2; buffer++) {
            dds3ZClearDrawTag[state][buffer].dma.qwc = 0xA;
            dds3ZClearDrawTag[state][buffer].dma.pad = 0;
            dds3ZClearDrawTag[state][buffer].dma.id = 0x70;
            dds3ZClearDrawTag[state][buffer].dma.addr = NULL;
            dds3ZClearDrawTag[state][buffer].dma.dat0 = 0;
            dds3ZClearDrawTag[state][buffer].dma.dat1 = 0x5000000A;
            dds3ZClearDrawTag[state][buffer].testGif.parm = 0x1000000000000001;
            dds3ZClearDrawTag[state][buffer].testGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].test.reg = 0x47;
            dds3ZClearDrawTag[state][buffer].test.data = 0x32001;
            dds3ZClearDrawTag[state][buffer].clearGif.parm = 0x1003400000000004;
            dds3ZClearDrawTag[state][buffer].clearGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].colorA.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorA.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionA.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionA.data = 0x79007000;
            dds3ZClearDrawTag[state][buffer].colorB.reg = 1;
            dds3ZClearDrawTag[state][buffer].colorB.data = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].positionB.reg = 5;
            dds3ZClearDrawTag[state][buffer].positionB.data = 0x87009000;
            dds3ZClearDrawTag[state][buffer].blendGif.parm = 0x1000000000008002;
            dds3ZClearDrawTag[state][buffer].blendGif.regs = 0xE;
            dds3ZClearDrawTag[state][buffer].blend[0].reg = 0x47;
            dds3ZClearDrawTag[state][buffer].blend[0].data = 0x5101B;
            dds3ZClearDrawTag[state][buffer].blend[1].reg = 0x42;
            dds3ZClearDrawTag[state][buffer].blend[1].data = 0x44;
        }
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitZClearDrawTag);
#endif

sdfDrawTag_t *dds3LinkZClearDrawTag(s32 state)
{
    sdfDrawTag_t *tag = dds3FontReqAllocDrawTag();

    sdfDrawTagLink(tag, &dds3ZClearDrawTag[state][dds3GetFrameID()]);
    return tag;
}

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

void dds3FadeColorSet(s8 red, s8 green, s8 blue)
{
    fadergba[0] = red;
    fadergba[1] = green;
    fadergba[2] = blue;
}

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

extern char D_0052E190[]; /* "DISTBLUR_NUMERR:%d" */
extern char D_0052E1A8[]; /* "RIPBLUR_NUMERR :%d" */

void dds3DrawDistRipOverError(void)
{
    sdfDrawTag_t *tag;

    if (dds3DebugSign != 0) {
        if (distblurovercnt != 0 || ripblurovercnt != 0) {
            tag = sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(tag);
            if ((s32)distblurovercnt > 0) {
                sdfDrawTagLink(tag, sdfDevPrintf(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, D_0052E190, distblurovercnt));
            }
            if ((s32)ripblurovercnt > 0) {
                sdfDrawTagLink(tag, sdfDevPrintf(0x73C0, 0x7B40, 0xFEFFFF, 4, D_0052E1A8, ripblurovercnt));
            }
            {
                sdfGrOt_t *ot = &sdfGrOtTable[81];
                ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))ot->regFunc)(ot, tag);
            }
        }
    }
}

/* NON_MATCHING: 52 of 170 words differ (first at +0x38). */
#ifdef NON_MATCHING
void dds3DrawFade(void)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *descriptor;
    u32 frameId;
    s32 alpha;
    u64 color;

    frameId = dds3GetFrameID();
    alpha = 0x80 - fadergba[3];
    if (alpha < 0x80) {
        color = 0x3F80000000000000 | ((u64)fadergba[1] << 8) | fadergba[0] | ((u64)fadergba[2] << 16) | (u64)(alpha << 24);
        dds3FadePacket[frameId].dma.qwc = 0xA;
        dds3FadePacket[frameId].dma.pad = 0;
        dds3FadePacket[frameId].dma.id = 0x70;
        dds3FadePacket[frameId].dma.addr = NULL;
        dds3FadePacket[frameId].dma.dat0 = 0;
        dds3FadePacket[frameId].dma.dat1 = 0x5000000A;
        dds3FadePacket[frameId].stateGif.parm = 0x1000000000000004;
        dds3FadePacket[frameId].stateGif.regs = 0xE;
        dds3FadePacket[frameId].state[0].reg = 0x42;
        dds3FadePacket[frameId].state[0].data = 1;
        dds3FadePacket[frameId].state[1].reg = 0x49;
        dds3FadePacket[frameId].state[1].data = 0;
        dds3FadePacket[frameId].state[2].reg = 0x4A;
        dds3FadePacket[frameId].state[2].data = 0;
        dds3FadePacket[frameId].state[3].reg = 0x47;
        dds3FadePacket[frameId].state[3].data = 0x31003;
        dds3FadePacket[frameId].spriteGif.parm = 0x1023400000008004;
        dds3FadePacket[frameId].spriteGif.regs = 0xE;
        dds3FadePacket[frameId].sprite[0].reg = 1;
        dds3FadePacket[frameId].sprite[0].data = color;
        dds3FadePacket[frameId].sprite[1].reg = 5;
        dds3FadePacket[frameId].sprite[1].data = 0xFEFFFF79007000;
        dds3FadePacket[frameId].sprite[2].reg = 1;
        dds3FadePacket[frameId].sprite[2].data = color;
        dds3FadePacket[frameId].sprite[3].reg = 5;
        dds3FadePacket[frameId].sprite[3].data = 0xFEFFFF87009000;
        list = dds3FontReqAllocDrawTag();
        sdfDrawTagLink(list, &dds3FadePacket[frameId]);
        descriptor = &sdfGrOtTable[79];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, list);
    }
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawFade);
#endif

void dds3SetScreenFovy(f32 fieldOfView)
{
    dds3ScreenParam.fovy = fieldOfView;
}

void dds3SetAmbient(sdfVector4_t *ambient)
{
    SDF_VU0_COPYVECTOR(&dds3LightParam.ambient, ambient);
}

void dds3SetFog(s32 *param)
{
    memcpy(&dds3LightParam.fog, param, 0x14);
}

void dds3SetLight(void *light, s32 index)
{
    dds3LightParam.parallel[index] = light;
}

void setdraw_xy(s32 *rect)
{
    rect[2] = 512;
    rect[3] = 448;
    rect[1] = 0;
    rect[0] = 0;
}

void dds3SetFilter2Para(s32 blend)
{
    filter2target.blend = blend;
}

void dds3SetFilter2RGBA(u32 rgba)
{
    filter2target.color = rgba;
}

void dds3Filter2Swap(s32 frame)
{
    memcpy(&dds3Filter2Buf, Eve_FilterGetParams(), 0x18);
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

void dds3SetMFilterPara(s32 blend)
{
    mfiltertarget.blend = blend;
}

void dds3SetMFilterRGBA(u32 rgba)
{
    mfiltertarget.color = rgba;
}

void dds3SetMFilterArea(s32 x, s32 y, s32 z)
{
    mfiltertarget.area[0] = x;
    mfiltertarget.area[1] = y;
    mfiltertarget.area[2] = z;
}

void dds3MFilterSwap(s32 frame)
{
    memcpy(&dds3MFilterBuf, Eve_PartialMultiFilterGetParams(), 0x24);
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

void dds3SetBlur2Para(f32 rotation, f32 scale, s32 blend)
{
    blur2target.rotation = rotation;
    blur2target.scale = scale;
    blur2target.blend = blend;
}

void dds3SetBlur2RGBA(u32 rgba)
{
    blur2target.color = rgba;
}

void dds3SetBlur2Area(s32 x, s32 y)
{
    blur2target.area[0] = x;
    blur2target.area[1] = y;
}

void dds3Blur2Swap(s32 frame)
{
    memcpy(&ripblurtarget.blur2buf, Eve_BlurGetParam(), 0x28);
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

void dds3SetMultiBlurRGBA(u32 rgba)
{
    mulblurtarget.color = rgba;
}

void dds3SetMultiBlurArea(s32 extent, s32 x, s32 y)
{
    mulblurtarget.extent = extent;
    mulblurtarget.x = x;
    mulblurtarget.y = y;
}

void dds3MultiBlurSwap(s32 frame)
{
    memcpy(&dds3MultiBlurBuf, Eve_PartialMultiBlurGetParams(), 0x2C);
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

void dds3SetDistBlurPara(s32 count, s32 size, s32 delaySpread, s32 blend,
                         f32 angleStep, f32 uvAngle, f32 uvAmplitude)
{
    if (count >= 101)
{
        distblurovercnt++;
        count = 100;
    }
    distblurtarget.count = count;
    distblurtarget.size = size;
    distblurtarget.delaySpread = delaySpread;
    distblurtarget.angleStep = angleStep;
    distblurtarget.uvAngle = uvAngle;
    distblurtarget.uvAmplitude = uvAmplitude;
    distblurtarget.blend = blend;
}

void dds3SetDistBlurRGBA(u32 rgba)
{
    distblurtarget.color = rgba;
}

void dds3SetDistBlurArea(s32 spread, s32 x, s32 y)
{
    distblurtarget.spread = spread;
    distblurtarget.x = x;
    distblurtarget.y = y;
}

void dds3DistBlurSwap(s32 frame)
{
    memcpy(&dds3DistBlurBuf, Eve_DistortionBlurGetParams(), 0x2C);
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
    ripblurtarget.ripple.count = count;
    ripblurtarget.ripple.phaseStep = phase;
    ripblurtarget.ripple.spacing = spacing;
    ripblurtarget.ripple.uvAngle = angle;
    ripblurtarget.ripple.uvAmplitude = amp;
    ripblurtarget.ripple.angleStep = anglestep;
    ripblurtarget.ripple.blend = blend;
}

void dds3SetRipBlurRGBA(u32 rgba)
{
    ripblurtarget.ripple.color = rgba;
}

void dds3SetRipBlurArea(s32 size, s32 x, s32 y)
{
    ripblurtarget.ripple.area = size;
    ripblurtarget.ripple.x = x;
    ripblurtarget.ripple.y = y;
}

void dds3RipBlurSwap(s32 frame)
{
    memcpy(&dds3RipBlurBuf, Eve_RippleBlurGetParams(), 0x2C);
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
    ripbluratarget = ripblurtarget.ripple.rgba[3];
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
    ripblurabase = ripblurtarget.ripple.rgba[3];
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

void dds3BlurUpdate(void)
{
    s32 alpha;

    if (dds3DrawFlag & 0x800) {
        blurnow++;
        dds3ScreenBlurAlpha = bluralphabase + blurnow * (bluralphatarget - bluralphabase) / blurmax;
        dds3ScreenBlurScale = blurscalebase + blurnow * (blurscaletarget - blurscalebase) / blurmax;
        if (dds3ScreenBlurAlpha == 0 && dds3ScreenBlurScale == 0) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        if (blurnow >= blurmax) {
            dds3DrawFlag &= 0xfffff7ff;
        }
    }
    if (dds3DrawFlag & 0x40000) {
        if (blur2swapnow < blur2swapmax) {
            blur2swapnow++;
        }
        evtHokanBlur2(1, (f32)blur2swapnow / (f32)blur2swapmax, &ripblurtarget.blur2buf,
                        &blur2target, &dds3Blur2State);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (dds3Blur2State.rgba[3] != 0) {
            Eve_BlurON();
        }
        else {
            Eve_BlurOFF();
        }
        if (blur2swapnow >= blur2swapmax) {
            dds3DrawFlag &= ~0x40000;
        }
    }
    if (dds3DrawFlag & 0x20000) {
        if (blur2now < blur2max) {
            blur2now++;
        }
        dds3Blur2State = blur2target;
        alpha = blur2abase + blur2now *
                (blur2atarget - blur2abase) /
                blur2max;
        dds3Blur2State.color = (dds3Blur2State.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (alpha == 0) {
            Eve_BlurOFF();
        }
        else {
            Eve_BlurON();
        }
        if (blur2now >= blur2max) {
            dds3DrawFlag &= ~0x20000;
        }
    }
    if (dds3DrawFlag & 0x8000) {
        if (mulblurswapnow < mulblurswapmax) {
            mulblurswapnow++;
        }
        evtHokanMultBlur(1, (f32)mulblurswapnow / (f32)mulblurswapmax, &dds3MultiBlurBuf,
                        &mulblurtarget, &key2);
        Eve_PartialMultiBlurSetParam(&key2);
        if (key2.rgba[3] != 0) {
            Eve_PartialMultiBlurON();
        }
        else {
            Eve_PartialMultiBlurOFF();
        }
        if (mulblurswapnow >= mulblurswapmax) {
            dds3DrawFlag &= ~0x8000;
        }
    }
    if (dds3DrawFlag & 0x1000) {
        if (mulblurnow < mulblurmax) {
            mulblurnow++;
        }
        key2 = mulblurtarget;
        alpha = mulblurabase + mulblurnow *
                (mulbluratarget - mulblurabase) /
                mulblurmax;
        key2.color = (key2.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiBlurSetParam(&key2);
        if (alpha == 0) {
            Eve_PartialMultiBlurOFF();
        }
        else {
            Eve_PartialMultiBlurON();
        }
        if (mulblurnow >= mulblurmax) {
            dds3DrawFlag &= ~0x1000;
        }
    }
    if (dds3DrawFlag & 0x10000) {
        if (distblurswapnow < distblurswapmax) {
            distblurswapnow++;
        }
        evtHokanDistBlur(1, (f32)distblurswapnow / (f32)distblurswapmax, &dds3DistBlurBuf,
                        &distblurtarget, &D_00556BC0);
        Eve_DistortionBlurSetParam(&D_00556BC0);
        if (D_00556BC0.rgba[3] != 0) {
            Eve_DistortionBlurON();
        }
        else {
            Eve_DistortionBlurOFF();
        }
        if (distblurswapnow >= distblurswapmax) {
            dds3DrawFlag &= ~0x10000;
        }
    }
    if (dds3DrawFlag & 0x2000) {
        if (distblurnow < distblurmax) {
            distblurnow++;
        }
        D_00556BC0 = distblurtarget;
        alpha = distblurabase + distblurnow *
                (distbluratarget - distblurabase) /
                distblurmax;
        D_00556BC0.color = (D_00556BC0.color & 0xFFFFFF) | (alpha << 24);
        Eve_DistortionBlurSetParam(&D_00556BC0);
        if (alpha == 0) {
            Eve_DistortionBlurOFF();
        }
        else {
            Eve_DistortionBlurON();
        }
        if (distblurnow >= distblurmax) {
            dds3DrawFlag &= ~0x2000;
        }
    }
    if (dds3DrawFlag & 0x1000000) {
        if (ripblurswapnow < ripblurswapmax) {
            ripblurswapnow++;
        }
        evtHokanRipBlur(1, (f32)ripblurswapnow / (f32)ripblurswapmax, &dds3RipBlurBuf,
                        &ripblurtarget.ripple, &dds3BlurState.ripple);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (dds3BlurState.ripple.rgba[3] != 0) {
            Eve_RippleBlurON();
        }
        else {
            Eve_RippleBlurOFF();
        }
        if (ripblurswapnow >= ripblurswapmax) {
            dds3DrawFlag &= ~0x1000000;
        }
    }
    if (dds3DrawFlag & 0x800000) {
        if (ripblurnow < ripblurmax) {
            ripblurnow++;
        }
        dds3BlurState.ripple = ripblurtarget.ripple;
        alpha = ripblurabase + ripblurnow *
                (ripbluratarget - ripblurabase) /
                ripblurmax;
        dds3BlurState.ripple.color = (dds3BlurState.ripple.color & 0xFFFFFF) | (alpha << 24);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (alpha == 0) {
            Eve_RippleBlurOFF();
        }
        else {
            Eve_RippleBlurON();
        }
        if (ripblurnow >= ripblurmax) {
            dds3DrawFlag &= ~0x800000;
        }
    }
    if (dds3DrawFlag & 0x100000) {
        if (filter2swapnow < filter2swapmax) {
            filter2swapnow++;
        }
        evtHokanFilter(1, (f32)filter2swapnow / (f32)filter2swapmax, &dds3Filter2Buf,
                                        &filter2target, &dds3BlurParam);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (dds3BlurParam.rgba[3] != 0) {
            Eve_FilterON();
        }
        else {
            Eve_FilterOFF();
        }
        if (filter2swapnow >= filter2swapmax) {
            dds3DrawFlag &= ~0x100000;
        }
    }
    if (dds3DrawFlag & 0x80000) {
        if (filter2now < filter2max) {
            filter2now++;
        }
        dds3BlurParam = filter2target;
        alpha = filter2abase + filter2now *
                (filter2atarget - filter2abase) /
                filter2max;
        dds3BlurParam.color = (dds3BlurParam.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (alpha == 0) {
            Eve_FilterOFF();
        }
        else {
            Eve_FilterON();
        }
        if (filter2now >= filter2max) {
            dds3DrawFlag &= ~0x80000;
        }
    }
    if (dds3DrawFlag & 0x400000) {
        if (mfilterswapnow < mfilterswapmax) {
            mfilterswapnow++;
        }
        evtHokanMultFilter(1, (f32)mfilterswapnow / (f32)mfilterswapmax, &dds3MFilterBuf,
                        &mfiltertarget, &D_00556C70);
        Eve_PartialMultiFilterSetParam(&D_00556C70);
        if (D_00556C70.rgba[3] != 0) {
            Eve_PartialMultiFilterON();
        }
        else {
            Eve_PartialMultiFilterOFF();
        }
        if (mfilterswapnow >= mfilterswapmax) {
            dds3DrawFlag &= ~0x400000;
        }
    }
    if (dds3DrawFlag & 0x200000) {
        if (mfilternow < mfiltermax) {
            mfilternow++;
        }
        D_00556C70 = mfiltertarget;
        alpha = mfilterabase + mfilternow *
                (mfilteratarget - mfilterabase) /
                mfiltermax;
        D_00556C70.color = (D_00556C70.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiFilterSetParam(&D_00556C70);
        if (alpha == 0) {
            Eve_PartialMultiFilterOFF();
        }
        else {
            Eve_PartialMultiFilterON();
        }
        if (mfilternow >= mfiltermax) {
            dds3DrawFlag &= ~0x200000;
        }
    }
}

/* vu0 routine: shortest-arc quaternion rotating VF10 to VF11. */
void dds3RotationArc(void)
{
    f32 identity[4];
    f32 axis[4];
    f32 dot;
    f32 component;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;

    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();

    SDF_VU0_INNERPRODUCT(dot);
    if (dot >= 0.9999f) {
        SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)identity);
        return;
    }

    if (dot <= -0.9999f) {
        SDF_VU0_GETVECTOR_Y(component);
        if (0.9999f <= fabsf(component)) {
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_Z(-1.0f);
        } else {
            SDF_VU0_GETVECTOR_Z(component);
            axis[0] = -component;
            axis[1] = 0.0f;
            SDF_VU0_GETVECTOR_X(axis[2]);
            axis[3] = 0.0f;
            SDF_VU0_LOADVECTOR(axis);
            SDF_VU0_NORMALIZE();
        }

        SDF_VU0_INNERPRODUCT(dot);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_SETVECTOR_W(dot);
        return;
    }

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(dot);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(axis);
    SDF_VU0_SETVECTOR_W(dot);
    Normalize();
}

/* vu0 routine: stage an immediate or interpolated parallel light color. */
void dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color)
{
    sdf3DLight_t *light = dds3LightParam.parallel[lightIndex];
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedBaseColor;
    u32 packedTargetColor;
    u32 blendFlag;

    if (light != NULL) {
        blendFlag = 4 << (lightIndex * 2);
        if (blendFrames == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(color);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[lightIndex]->color);
        } else {
            lightcolormax[lightIndex] = blendFrames;
            lightcolornow[lightIndex] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(light);
            MMI_RGBA_PACK_128(packedBaseColor);
            baseColorWords[0] = packedBaseColor;
            dds3LightColor[lightIndex] = packedBaseColor;
            SDF_VU0_LOADVECTOR(color);
            SDF_VU0_VECTOR_W_SET_ZERO();
            MMI_RGBA_PACK_128(packedTargetColor);
            D_00557230[lightIndex] = packedTargetColor;
            targetColorWords[0] = packedTargetColor;
        }
    }
}

void dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction)
{
    u32 blendFlag;

    if (dds3LightParam.parallel[lightIndex] != NULL) {
        blendFlag = 8 << (lightIndex * 2);
        if (blendFrames == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[lightIndex]->v);
        } else {
            lightdirmax[lightIndex] = blendFrames;
            lightdirnow[lightIndex] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[lightIndex]->v);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            /* No memory clobber here: retail recomputes the slot offset after the call. */
            SDF_VU0_STOREVECTOR_NOMEM(D_00557240[lightIndex]);
            dds3RotationArc();
            SDF_VU0_STOREVECTOR(D_00557270[lightIndex]);
        }
    }
}

/* vu0 routine: stage an immediate or interpolated ambient color. */
void dds3SetAmbientColor(s32 blendFrames, f32 *color)
{
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedColor;

    if (blendFrames == 0) {
        dds3DrawFlag &= ~0x100;
        SDF_VU0_LOADVECTOR(color);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&dds3LightParam.ambient);
    } else {
        ambientmax = blendFrames;
        ambientnow = 0;
        dds3DrawFlag |= 0x100;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_128(packedColor);
        baseColorWords[0] = packedColor;
        ambientbase = packedColor;
        SDF_VU0_LOADVECTOR(color);
        SDF_VU0_VECTOR_W_SET_ONE();
        MMI_RGBA_PACK_128(packedColor);
        targetColorWords[0] = packedColor;
        ambienttarget = packedColor;
    }
}

/* vu0 routine: stage an immediate or interpolated packed color. */
void dds3SetFogColor(s32 blendFrames, f32 *color)
{
    u32 targetColorWords[4];
    u32 packedTargetColor;

    SDF_VU0_LOADVECTOR(color);
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (blendFrames == 0) {
        dds3DrawFlag &= ~0x200;
        dds3LightParam.fog.color = targetColorWords[0];
    } else {
        dds3DrawFlag |= 0x200;
        fogrgbmax = blendFrames;
        fogrgbtarget = targetColorWords[0];
        fogrgbbase = dds3LightParam.fog.color;
        fogrgbnow = 0;
    }
}

void dds3SetFogPara(s32 blendFrames, f32 fogMax, f32 fogMin, f32 fogNear, f32 fogFar)
{
    if (blendFrames == 0) {
        dds3DrawFlag &= ~0x400;
        dds3FogState.fogMax = fogMax;
        dds3FogState.fogMin = fogMin;
        dds3FogState.fogNear = fogNear;
        dds3FogState.fogFar = fogFar;
    } else {
        f32 oldMax = dds3FogState.fogMax;
        f32 oldMin = dds3FogState.fogMin;
        f32 oldNear = dds3FogState.fogNear;
        f32 oldFar = dds3FogState.fogFar;
        dds3DrawFlag |= 0x400;
        fogparamax = blendFrames;
        dds3FogParam.fogMax = oldMax;
        dds3FogParam.fogMin = oldMin;
        dds3FogParam.fogNear = oldNear;
        dds3FogParam.fogFar = oldFar;
        dds3FogColor.fogMax = fogMax;
        dds3FogColor.fogMin = fogMin;
        dds3FogColor.fogNear = fogNear;
        dds3FogColor.fogFar = fogFar;
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
        D_00551BA8->constant = 1.0f;
    }
}

void eglSetOt(u32 surfaceIndex)
{
    egl_OT_Offset = surfaceIndex;
}

void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst)
{
    void *list = (void *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    dds3GsCommand_t *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    command = (dds3GsCommand_t *)sdf2DMakeStdPacket(packet, 0x30);
    command->write.data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->write.reg = 0x47;
    sdfDrawTagLink(list, packet);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

INCLUDE_RODATA(const s32, "kernel/dds3KernelDraw", D_0052E190);

INCLUDE_RODATA(const s32, "kernel/dds3KernelDraw", D_0052E1A8);

void eglLinkAlpha(s32 blendMode)
{
    void *list = (void *)sdfGrAllocPacketBuff(0x20);
    void *packet;
    dds3GsCommand_t *command;

    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x30);
    command = (dds3GsCommand_t *)sdf2DMakeStdPacket(packet, 0x30);
    switch (blendMode) {
    case 1:
        command->write.data = 0x48;
        break;
    case 2:
        command->write.data = 0x42;
        break;
    case 3:
        command->write.data = 0x84;
        break;
    case 4:
        command->write.data = 0x06;
        break;
    case 5:
        command->write.data = 0x89;
        break;
    case 6:
        command->write.data = 0x42;
        break;
    case 10:
        command->write.data = 0x2A;
        break;
    case 20:
        command->write.data = 0x4A;
        break;
    default:
        command->write.data = 0x44;
        break;
    }
    command->write.reg = 0x42;
    sdfDrawTagLink(list, packet);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

void eglLinkSprite(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void eglLinkBox(s32 x, s32 y, s32 width, s32 height,
                s32 color0, s32 color1, s32 color2, s32 color3)
{
    s32 coords[4][2];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    dds3GouraudRegister_t *registers;
    s32 i;
    sdfGrOt_t *descriptor;

    u32 r0 = color0 & 0xFF;
    u32 g0 = (color0 >> 8) & 0xFF;
    u32 b0 = (color0 >> 16) & 0xFF;
    u32 a0 = (u32)color0 >> 24;
    u32 r1 = color1 & 0xFF;
    u32 g1 = (color1 >> 8) & 0xFF;
    u32 b1 = (color1 >> 16) & 0xFF;
    u32 a1 = (u32)color1 >> 24;
    u32 r2 = color2 & 0xFF;
    u32 g2 = (color2 >> 8) & 0xFF;
    u32 b2 = (color2 >> 16) & 0xFF;
    u32 a2 = (u32)color2 >> 24;
    u32 r3 = color3 & 0xFF;
    u32 g3 = (color3 >> 8) & 0xFF;
    u32 b3 = (color3 >> 16) & 0xFF;
    u32 a3 = (u32)color3 >> 24;
    coords[0][0] = x * 16;
    coords[0][1] = y * 8;
    coords[1][0] = (x + width) * 16;
    coords[1][1] = y * 8;
    coords[2][0] = (x + width) * 16;
    coords[2][1] = (y + height) * 8;
    coords[3][0] = x * 16;
    coords[3][1] = (y + height) * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    registers = (dds3GouraudRegister_t *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            registers->low = (u64)r0 | ((u64)g0 << 32);
            registers->high = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            registers->low = (u64)r1 | ((u64)g1 << 32);
            registers->high = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            registers->low = (u64)r2 | ((u64)g2 << 32);
            registers->high = (u64)b2 | ((u64)a2 << 32);
        } else {
            registers->low = (u64)r3 | ((u64)g3 << 32);
            registers->high = (u64)b3 | ((u64)a3 << 32);
        }
        registers++;
        registers->high = 0xFFFFFF;
        registers->low = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        registers++;
    }
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 color0, s32 color1, s32 color2)
{
    s32 coords[3][2];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    dds3GouraudRegister_t *registers;
    s32 i;
    sdfGrOt_t *descriptor;

    u32 r0 = color0 & 0xFF;
    u32 g0 = (color0 >> 8) & 0xFF;
    u32 b0 = (color0 >> 16) & 0xFF;
    u32 a0 = (u32)color0 >> 24;
    u32 r1 = color1 & 0xFF;
    u32 g1 = (color1 >> 8) & 0xFF;
    u32 b1 = (color1 >> 16) & 0xFF;
    u32 a1 = (u32)color1 >> 24;
    u32 r2 = color2 & 0xFF;
    u32 g2 = (color2 >> 8) & 0xFF;
    u32 b2 = (color2 >> 16) & 0xFF;
    u32 a2 = (u32)color2 >> 24;
    coords[0][0] = x0 * 16;
    coords[0][1] = y0 * 8;
    coords[1][0] = x1 * 16;
    coords[1][1] = y1 * 8;
    coords[2][0] = x2 * 16;
    coords[2][1] = y2 * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    registers = (dds3GouraudRegister_t *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            registers->low = (u64)r0 | ((u64)g0 << 32);
            registers->high = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            registers->low = (u64)r1 | ((u64)g1 << 32);
            registers->high = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            registers->low = (u64)r2 | ((u64)g2 << 32);
            registers->high = (u64)b2 | ((u64)a2 << 32);
        }
        registers++;
        registers->high = 0xFFFFFF;
        registers->low = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        registers++;
    }
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 color0, u32 color1, u32 color2, u32 color3)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, D_00551BA8, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = D_002F5970[0];
    sdfDrawTagLink(list, sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

/* Preserve native vertex order 0,2,3,1; W components are not initialized. */
void eglLinkTexPoly4(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, f32 y1, f32 z1, s32 color2, f32 x2, f32 y2, f32 z2, s32 color3, f32 x3, f32 y3, f32 z3, sdfTexHandle_t *texture, f32 u0, f32 v0, f32 u1, f32 v1)
{
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t verts[4];
    u32 colors[4];
    u64 strip[2];
    sdfVector2_t uvs[4];
    sdfMat_t *asset;
    void *list;
    sdfGrOt_t *surface;

    asset = sdfMatNew();
    sdfMatSetTex(asset, texture);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, asset, 0);
    memset(&desc, 0, sizeof(desc));
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = 4;
    desc.vertex = verts;
    desc.vertexColor = colors;
    desc.faceIndex = (u32 *)strip;
    desc.uv = uvs;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    colors[0] = color0;
    colors[1] = color2;
    colors[2] = color3;
    colors[3] = color1;
    strip[0] = D_002F5970[0];
    uvs[0].x = u0;
    uvs[0].y = v0;
    uvs[1].x = u0;
    uvs[1].y = v1;
    uvs[2].x = u1;
    uvs[2].y = v1;
    uvs[3].x = u1;
    uvs[3].y = v0;
    sdfDrawTagLink(list, sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    sdfMatFreeDelayed(asset);
}

void eglLinkLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 color0, u32 color1)
{
    void *list;
    D_002F5980[0][0] = x0;
    D_002F5980[0][1] = y0;
    D_002F5980[0][2] = z0;
    D_002F5980[1][0] = x1;
    D_002F5980[1][1] = y1;
    D_002F5980[1][2] = z1;
    D_002F59A0[1] = color1;
    D_002F59A0[0] = color0;
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, effDebugDrawWireModel(D_002F5980, D_002F59A0, 2, 0x80));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    }
}

void eglLinkPrint(s32 x, s32 y, u32 color, const char *format)
{
    dds3PrintControl_t sifParameters;
    void *list;
    void *packet;
    sdfGrOt_t *surface;
    list = dds3FontReqAllocDrawTag();
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, packet);
    sdfDevSetPrintCB(&sifParameters, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, color);
    sdfDrawTagLink(list, sdfDevPrintfEx(&sifParameters, format));
    surface = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void eglLinkKanji(s32 x, s32 y, s32 color, const char *text)
{
    void *req = frwGetSmallFontFRQ(x << 4, y << 3, 0, color, text, NULL);

    frDrawFont(req);
    frDestroyFontRequestAfter(req);
}

/* vu0 routine: project a world point into viewport pixel coordinates. */
void egl3Dto2D(s32 *gridX, s32 *gridY, f32 x, f32 y, f32 z)
{
    f32 point[4];
    f32 screen[4];

    point[0] = x;
    point[1] = y;
    point[2] = z;
    point[3] = 1.0f;
    SDF_VU0_LOADMATRIX(&D_002FA240);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR(point);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(screen);
    *gridX = ((s32)(screen[0] * 16.0f) - 0x7000) >> 4;
    *gridY = ((s32)(screen[1] * 16.0f) - 0x7900) >> 3;
}
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

