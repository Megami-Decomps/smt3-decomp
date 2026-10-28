#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "draw.h"

/* One frame buffer's draw bank, stride 0x1BE0; only the eleven model draw
 * environments at 0x40 and the init tag at 0x0 are reached here. */
typedef struct
{
    u8 unk0000[0x40];
    sdfModelDrawEnv_t modelEnv[11];
    u8 unk12D0[0x910];
} dds3KernelProcess_dds3DrawBank_t;

typedef struct
{
    sdfDrawTag_t *tagHead;
    sdfPreDrawTag_t *tail;
} dds3KernelProcess_sdfPreDrawChain_t;

extern dds3KernelProcess_dds3DrawBank_t dds3DrawEnv[];

extern dds3KernelProcess_sdfPreDrawChain_t D_002F6A70[];

extern sdfGrOt_t sdfGrOtTable[];

extern s32 dds3GetFrameID(void);

extern void sdfGrInitNewFrame(void);

extern void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);

extern void sdfPreDrawChainInit(dds3KernelProcess_sdfPreDrawChain_t *aChain);

extern void dds3InitDrawEnvSub(s32 i);

extern void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv);

extern void sdfModelDrawEnvLinkOt(sdfGrOt_t **aOt, sdfModelDrawEnv_t *aEnv);

extern sdfGrOt_t *D_002F69B8[];

extern sdfGrOt_t D_002F6938;

extern dds3KernelProcess_sdfPreDrawChain_t D_002F6A70[];

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern s32 dds3ScreenBlurOfsX;

extern s32 dds3ScreenBlurOfsY;

extern u32 dds3DrawFlag;

extern u8 dds3DrawProcessFlag;

extern sdfDrawTag_t *dds3ClosedDrawTag;

extern u8 D_00551888;

extern void dds3DrawDistRipOverError(void);

extern void dds3FadeUpdate(void);

extern void dds3DrawFade(void);

extern void dds3BlurUpdate(void);

extern s32 dds3NullDrawProc(void);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, u32 aEnv, void *aPkt);

extern void *sdf2DAllocSprite(s32 aNumOfSpr);

extern void *sdf2DGetPrimPtr(void *aPacketPtr);

extern sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt);

extern void sdfGrFlushOtChain(u32 *source);

extern void sdfGrReqDraw(sdfDrawTag_t *aTag, dds3KernelProcess_sdfPreDrawChain_t *aPreDraw);

extern dds3Basic_t *dds3TheWorld;

extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

s32 dds3FirstProcess(dds3ProcessID_t *id)
{
    sdfModelDrawEnv_t *env;
    s32 frame;
    s32 i;

    frame = dds3GetFrameID();
    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, 0x55);
    sdfPreDrawChainInit(D_002F6A70);
    dds3InitDrawEnvSub(frame);
    env = dds3DrawEnv[frame].modelEnv;
    for (i = 0; i < 11; i++) {
        sdfModelSetDrawEnv(env);
        env++;
    }
    sdfGrOtTable[0].regFunc(&sdfGrOtTable[0], (sdfDrawTag_t *)&dds3DrawEnv[frame]);
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);

#ifdef NON_MATCHING
/* NON_MATCHING: 157 of 218 words match at the exact 872-byte size; the two
   0x40 packets are stored through a saved register where retail keeps the
   allocator return in $2. */
s32 dds3DrawProcess(dds3ProcessID_t *id)
{
    s32 blur[4];
    sdfModelDrawEnv_t *env;
    sdfGrOt_t **ot;
    sdfGrOt_t *otp;
    sdfDrawTag_t *tag;
    sdfDrawTag_t *closed;
    void *sprite;
    s32 *prim;
    s32 *ofs;
    u64 *p;
    s32 frame;
    s32 i;

    frame = dds3GetFrameID();
    dds3DrawDistRipOverError();
    dds3FadeUpdate();
    dds3DrawFade();
    dds3BlurUpdate();
    ot = D_002F69B8;
    env = dds3DrawEnv[frame].modelEnv;
    for (i = 0; i < 11; i++) {
        sdfModelDrawEnvLinkOt(ot, env);
        ot += 4;
        env++;
    }
    if (dds3ScreenBlurFlag != 0 && dds3NullDrawProc() == 0) {
        tag = sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        sdfGrLinkDispFrameTex(tag, (u32)&dds3DrawEnv[frame].unk12D0[0x30],
                              sdfGrAllocPacketBuff(0x20));
        p = sdfGrAllocPacketBuff(0x40);
        p[0] = 3;
        p[1] = 0x5000000310000000;
        p[2] = 0x1000000000008002;
        p[3] = 0xE;
        p[4] = 0x8000000080;
        p[5] = 0x3B;
        p[6] = 0;
        p[7] = 0x3F;
        sdfDrawTagLink(tag, (u32)p);
        p = sdfGrAllocPacketBuff(0x40);
        p[0] = 3;
        p[1] = 0x5000000310000000;
        p[2] = 0x1000000000008002;
        p[3] = 0xE;
        p[4] = 0x31001;
        p[5] = 0x47;
        p[6] = 0x44;
        p[7] = 0x42;
        sdfDrawTagLink(tag, (u32)p);
        ofs = &dds3ScreenBlurOfsX;
        blur[0] = ofs[0] + 0x100;
        blur[1] = ofs[1] + 0xE0;
        blur[2] = 0x100 - ofs[0];
        blur[3] = 0xE0 - ofs[1];
        for (i = 0; i < 4; i++) {
            blur[i] = blur[i] * dds3ScreenBlurScale >> 12;
        }
        sprite = sdf2DAllocSprite(1);
        prim = sdf2DGetPrimPtr(sprite);
        prim[0] = 0x80;
        prim[1] = 0x80;
        prim[2] = 0x80;
        prim[3] = dds3ScreenBlurAlpha;
        prim[4] = 0;
        prim[5] = 0;
        *(u16 *)((s32)prim + 0x4C) = 0;
        prim[8] = 0x7000 - (blur[0] << 4);
        prim[9] = 0x7900 - (blur[1] << 3);
        prim[10] = 0;
        prim[12] = 0x2000;
        prim[13] = 0xE00;
        *(u16 *)((s32)prim + 0x2C) = 0;
        prim[16] = 0x9000 + (blur[2] << 4);
        prim[17] = 0x8700 + (blur[3] << 3);
        prim[18] = 0;
        sdfDrawTagLink(tag, (u32)sprite);
        otp = &D_002F6938;
        otp->regFunc(otp, tag);
    }
    closed = sdfGrOtClose(sdfGrOtTable);
    dds3ClosedDrawTag = closed;
    if (dds3DrawProcessFlag != 0) {
        sdfGrFlushOtChain((u32 *)*(void **)((s32)closed + 4));
        dds3DrawProcessFlag = 0;
    }
    if ((dds3DrawFlag & 0x2000000) == 0) {
        sdfGrReqDraw(closed, D_002F6A70);
    } else {
        dds3DrawFlag &= ~0x2000000;
    }
    D_00551888 = 0;
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);
#endif

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

