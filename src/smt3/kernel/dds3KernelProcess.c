#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "kernel_draw.h"
#include "model.h"
#include "ps2asm.h"

extern dds3Basic_t *dds3TheWorld;

extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

typedef struct sdfPreDrawChain_t sdfPreDrawChain_t;
extern dds3DrawBank_t dds3DrawEnv[2];
extern sdfPreDrawChain_t D_002F6A70;
extern sdfGrOt_t sdfGrOtTable[85];
extern u32 dds3GetFrameID(void);
extern void sdfGrInitNewFrame(void);
extern void sdfGrOtInit(sdfGrOt_t *ot, s32 count);
extern void sdfPreDrawChainInit(sdfPreDrawChain_t *chain);
extern void dds3InitDrawEnvSub(s32 frame);
extern void sdfModelSetDrawEnv(sdfModelDrawEnv_t *env);

s32 dds3FirstProcess(dds3ProcessID_t *id)
{
    u32 frame = dds3GetFrameID();
    sdfModelDrawEnv_t *env;
    s32 count;

    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, 85);
    sdfPreDrawChainInit(&D_002F6A70);
    dds3InitDrawEnvSub(frame);
    env = dds3DrawEnv[frame].modelEnv;
    for (count = 10; count >= 0; --count, ++env) {
        sdfModelSetDrawEnv(env);
    }
    sdfGrOtTable[0].regFunc(&sdfGrOtTable[0], &dds3DrawEnv[frame].initTag);
    return 0;
}

/* Reference implementation: native packet/camera flow; assembly remains production. */
#ifdef NON_MATCHING
extern u32 dds3RenderTex, dds3RenderDrawEnv, dds3BackColor;
extern dds3LightState_t dds3LightParam, dds3LightParam2;
extern sdf3DLight_t *D_002F5D10[];
extern sdfVector4_t D_002F5D20, D_002F5C20, D_002F5C30, effPCPBossTexRect;
extern sdfVector4_t fldAutoMapRect, D_002F5E00, D_002F5E10, aViewTarget, aViewEye, aViewDown;
extern f32 dds3QuakeOffset[4];
extern sdf3DScreenParam_t D_002F5B80, dds3ScreenParam, dds3ScreenParam2;
extern sdfMatrix4_t D_002FA2A0, aProjMatrix, D_002FA240;
extern sdf3DFogParam_t D_002F5D30;
extern sdfModelOt_t D_002F69B8[];
PS2_SYMBOL_AT(sdfVector4_t, dds3AmbientColor, dds3LightParam, 0x10);
PS2_SYMBOL_AT(sdfVector4_t, dds3AmbientColor2, dds3LightParam2, 0x10);
PS2_SYMBOL_AT(sdf3DFogParam_t, dds3FogParamMain, dds3LightParam, 0x20);
PS2_SYMBOL_AT(sdf3DFogParam_t, dds3FogParam2, dds3LightParam2, 0x20);
extern void dds3RunQuake(void);
extern void dds3SceneLightUpdateFirst(void);
extern void sdf3DMakeScreenParam(sdf3DScreenParam_t *);
extern void LookAtMatrix(const void *, const void *, const void *);
extern void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *, sdf3DScreenParam_t *, void *);
extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
extern s32 sdfGrAllocPacketBuff(s32);
extern void sdf3DMakeFogPacket(sdf3DFogPacket_t *, sdf3DFogParam_t *);
extern void sdfDrawTagLink(sdfDrawTag_t *, u32);
extern void sdf3DSetCamera(sdf3DScreenParam_t *, void *);
extern void sdfPreDrawChainLink(sdfPreDrawChain_t *, sdfPreDrawTag_t *);
extern void sdfModelOtInit(sdfModelOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *, sdfGrOt_t *);
extern void fldOtInit(void);
extern void sdf3DSetDefaultLight(u8 *);
extern void sdf3DSetDefaultFog(u8 *);
extern sdfDrawTag_t *dds3LinkZClearDrawTag(s32);

s32 dds3DrawFirstProcess(dds3ProcessID_t *id)
{
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;
    u32 frame = dds3GetFrameID();
    sdfDrawTag_t *sceneTag;
    sdf3DFogPacket_t *sceneFog;

    dds3RunQuake();
    dds3SceneLightUpdateFirst();
    if (dds3RenderTex != 0) {
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].renderLight, D_002F5D10, &D_002F5D20);
        dds3DrawEnv[frame].renderEnv.clear.clearColor = dds3RenderDrawEnv;
        if (dds3RenderTex != 0) {
            sdfDrawTag_t *renderTag;
            sdf3DFogPacket_t *renderFog;
            sdf3DMakeScreenParam(&D_002F5B80);
            LookAtMatrix(&D_002F5C20, &D_002F5C30, &effPCPBossTexRect);
            SDF_VU0_STOREMATRIX(&D_002FA2A0);
            sdf3DMakeViewportPacket(&dds3DrawEnv[frame].renderViewport, &D_002F5B80, &D_002FA2A0);
            sdfGrOtTable[1].regFunc(&sdfGrOtTable[1], &dds3DrawEnv[frame].renderTag);
            renderTag = dds3FontReqAllocDrawTag();
            renderFog = (sdf3DFogPacket_t *)sdfGrAllocPacketBuff(0x50);
            sdf3DMakeFogPacket(renderFog, &D_002F5D30);
            sdfDrawTagLink(renderTag, (u32)renderFog);
            sdfGrOtTable[1].regFunc(&sdfGrOtTable[1], renderTag);
        }
    }
    {
        sdfDrawTag_t *subTag;
        sdf3DFogPacket_t *subFog;
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].subLight, dds3LightParam2.parallel, &dds3AmbientColor2);
        sdf3DMakeScreenParam(&dds3ScreenParam2);
        LookAtMatrix(&fldAutoMapRect, &D_002F5E00, &D_002F5E10);
        SDF_VU0_STOREMATRIX(&aProjMatrix);
        sdf3DMakeViewportPacket(&dds3DrawEnv[frame].subViewport, &dds3ScreenParam2, &aProjMatrix);
        sdfGrOtTable[73].regFunc(&sdfGrOtTable[73], &dds3DrawEnv[frame].subTag);
        subTag = dds3FontReqAllocDrawTag();
        subFog = (sdf3DFogPacket_t *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket(subFog, &dds3FogParam2);
        sdfDrawTagLink(subTag, (u32)subFog);
        sdfGrOtTable[73].regFunc(&sdfGrOtTable[73], subTag);
    }
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[frame].light, dds3LightParam.parallel, &dds3AmbientColor);
    dds3DrawEnv[frame].stdEnv.env.clear.clearColor = dds3BackColor;
    sdf3DMakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(dds3QuakeOffset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(eye);
    LookAtMatrix((const void *)target, (const void *)eye, &aViewDown);
    SDF_VU0_STOREMATRIX(&D_002FA240);
    sdf3DMakeViewportPacket(&dds3DrawEnv[frame].viewport, &dds3ScreenParam, &D_002FA240);
    sdf3DSetCamera(&dds3ScreenParam, &D_002FA240);
    sdfGrOtTable[7].regFunc(&sdfGrOtTable[7], &dds3DrawEnv[frame].drawTag);
    sceneTag = dds3FontReqAllocDrawTag();
    sceneFog = (sdf3DFogPacket_t *)sdfGrAllocPacketBuff(0x50);
    sdf3DMakeFogPacket(sceneFog, &dds3FogParamMain);
    sdfDrawTagLink(sceneTag, (u32)sceneFog);
    sdfGrOtTable[7].regFunc(&sdfGrOtTable[7], sceneTag);
    sdfPreDrawChainLink(&D_002F6A70, &dds3DrawEnv[frame].preDrawTag);
    sdfModelOtInit(&D_002F69B8[0], &sdfGrOtTable[39], &sdfGrOtTable[40], &sdfGrOtTable[44], &sdfGrOtTable[45]);
    fldOtInit();
    sdfModelOtInit(&D_002F69B8[7], &sdfGrOtTable[51], &sdfGrOtTable[52], &sdfGrOtTable[53], &sdfGrOtTable[54]);
    sdfModelOtInit(&D_002F69B8[8], &sdfGrOtTable[63], &sdfGrOtTable[64], &sdfGrOtTable[65], &sdfGrOtTable[66]);
    sdfModelOtInit(&D_002F69B8[1], &sdfGrOtTable[8], &sdfGrOtTable[9], &sdfGrOtTable[10], &sdfGrOtTable[11]);
    sdfModelOtInit(&D_002F69B8[2], &sdfGrOtTable[13], &sdfGrOtTable[14], &sdfGrOtTable[15], &sdfGrOtTable[16]);
    sdfModelOtInit(&D_002F69B8[3], &sdfGrOtTable[18], &sdfGrOtTable[19], &sdfGrOtTable[20], &sdfGrOtTable[21]);
    sdfModelOtInit(&D_002F69B8[4], &sdfGrOtTable[23], &sdfGrOtTable[24], &sdfGrOtTable[25], &sdfGrOtTable[26]);
    sdfModelOtInit(&D_002F69B8[5], &sdfGrOtTable[28], &sdfGrOtTable[29], &sdfGrOtTable[30], &sdfGrOtTable[31]);
    sdfModelOtInit(&D_002F69B8[9], &sdfGrOtTable[74], &sdfGrOtTable[75], &sdfGrOtTable[76], &sdfGrOtTable[77]);
    sdfModelOtInit(&D_002F69B8[10], &sdfGrOtTable[2], &sdfGrOtTable[3], &sdfGrOtTable[4], &sdfGrOtTable[5]);
    sdf3DSetDefaultLight((u8 *)&dds3DrawEnv[frame].light);
    sdf3DSetDefaultFog((u8 *)sceneFog);
    sceneTag = dds3LinkZClearDrawTag(0);
    sdfGrOtTable[69].regFunc(&sdfGrOtTable[69], sceneTag);
    sceneTag = dds3LinkZClearDrawTag(1);
    sdfGrOtTable[80].regFunc(&sdfGrOtTable[80], sceneTag);
    sceneTag = dds3LinkZClearDrawTag(2);
    sdfGrOtTable[82].regFunc(&sdfGrOtTable[82], sceneTag);
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);
#endif

typedef struct sdfGrRefPacketHeader_t sdfGrRefPacketHeader_t;

/* Sprite GIF payload: RGBAQ followed by UV/XYZ2 for each corner.
 * The GS ignores the unused lanes; native writes XYZ's 16-bit ADC lane. */
typedef struct {
    s32 u, v;
    u8 unusedUV[8];
    s32 x, y, z;
    s16 adc;
    u8 unusedXYZ[2];
} dds3BlurCorner_t;
typedef struct {
    s32 r, g, b, a;
    dds3BlurCorner_t corner[2];
} dds3BlurSprite_t;
extern sdfModelOt_t D_002F69B8[];
extern sdfGrOt_t D_002F6938;
extern u8 dds3ScreenBlurFlag, dds3DrawProcessFlag;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern s32 dds3ScreenBlurOfsX[2];
extern sdfDrawTag_t *dds3ClosedDrawTag;
extern u32 dds3DrawFlag;
extern s8 D_00551888;
extern void dds3DrawDistRipOverError(void);
extern void dds3FadeUpdate(void);
extern void dds3DrawFade(void);
extern void dds3BlurUpdate(void);
extern u32 dds3NullDrawProc(void);
extern void sdfModelDrawEnvLinkOt(sdfGrOt_t **, sdfModelDrawEnv_t *);
extern s32 sdfGrAllocPacketBuff(s32);
extern void sdfDrawTagInit(sdfDrawTag_t *);
extern void sdfDrawTagLink(sdfDrawTag_t *, u32);
extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *, u32, sdfGrRefPacketHeader_t *);
extern sdfGrPacketHeader_t *sdf2DAllocSprite(s32);
extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *);
extern sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *);
extern void sdfGrFlushOtChain(u32 *);
extern void sdfGrReqDraw(sdfDrawTag_t *, sdfPreDrawChain_t *);

/* DDS1 kwlnRenderFrame donor, with SMT3's eleven model groups and draw bank. */
s32 dds3DrawProcess(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    sdfModelOt_t *groupHeads;
    sdfModelDrawEnv_t *packetGroups;
    sdfDrawTag_t *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    dds3BlurSprite_t *vertex;
    void *spritePacket;
    sdfDrawTag_t *poolHead;
    s32 edgeDistances[4];
    s32 i;

    dds3DrawDistRipOverError();
    dds3FadeUpdate();
    dds3DrawFade();
    dds3BlurUpdate();
    groupHeads = D_002F69B8;
    packetGroups = dds3DrawEnv[bufferIndex].modelEnv;
    for (i = 10; i >= 0; i--) {
        sdfModelDrawEnvLinkOt(groupHeads->ot, packetGroups);
        groupHeads++;
        packetGroups++;
    }
    if (dds3ScreenBlurFlag != 0 && dds3NullDrawProc() == 0) {
        packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(packetList);
        sdfGrLinkDispFrameTex(packetList, (u32)&dds3DrawEnv[bufferIndex].stdEnv, (sdfGrRefPacketHeader_t *)sdfGrAllocPacketBuff(0x20));
        texturePacket = (u64 *)sdfGrAllocPacketBuff(0x40);
        texturePacket[0] = 3;
        /* VIF FLUSHA, then DIRECT for the three following quadwords. */
        texturePacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        texturePacket[2] = ((u64)0x10000000 << 32) | 0x8002;
        texturePacket[3] = 0xE;
        texturePacket[4] = ((u64)0x80 << 32) | 0x80;
        texturePacket[5] = 0x3B;
        texturePacket[6] = 0;
        texturePacket[7] = 0x3F;
        sdfDrawTagLink(packetList, (u32)texturePacket);
        blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
        blendPacket[0] = 3;
        blendPacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        blendPacket[2] = ((u64)0x10000000 << 32) | 0x8002;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = 0x44;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(packetList, (u32)blendPacket);
        edgeDistances[0] = dds3ScreenBlurOfsX[0] + 0x100;
        edgeDistances[1] = dds3ScreenBlurOfsX[1] + 0xE0;
        edgeDistances[2] = 0x100 - dds3ScreenBlurOfsX[0];
        edgeDistances[3] = 0xE0 - dds3ScreenBlurOfsX[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * dds3ScreenBlurScale >> 12;
        }
        spritePacket = sdf2DAllocSprite(1);
        vertex = (dds3BlurSprite_t *)sdf2DGetPrimPtr(spritePacket);
        vertex->r = 0x80;
        vertex->g = 0x80;
        vertex->b = 0x80;
        vertex->a = dds3ScreenBlurAlpha;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = 0x7000 - edgeDistances[0] * 16;
        vertex->corner[0].y = 0x7900 - edgeDistances[1] * 8;
        vertex->corner[0].z = 0;
        vertex->corner[0].adc = 0;
        vertex->corner[1].u = 0x2000;
        vertex->corner[1].v = 0xE00;
        vertex->corner[1].x = 0x9000 + edgeDistances[2] * 16;
        vertex->corner[1].y = 0x8700 + edgeDistances[3] * 8;
        vertex->corner[1].z = 0;
        vertex->corner[1].adc = 0;
        sdfDrawTagLink(packetList, (u32)spritePacket);
        D_002F6938.regFunc(&D_002F6938, packetList);
    }
    poolHead = sdfGrOtClose(sdfGrOtTable);
    dds3ClosedDrawTag = poolHead;
    if (dds3DrawProcessFlag != 0) {
        sdfGrFlushOtChain((u32 *)poolHead->head);
        dds3DrawProcessFlag = 0;
    }
    if (!(dds3DrawFlag & 0x2000000)) {
        sdfGrReqDraw(poolHead, &D_002F6A70);
    } else {
        dds3DrawFlag &= 0xFDFFFFFF;
    }
    D_00551888 = 0;
    return 0;
}

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

