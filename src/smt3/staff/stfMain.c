#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"
#include "sdfFrame.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

extern void sdfMemFreeDirect(void *aHandle);

extern saddr_t stfShutDown(void);

extern char stfProcName[];

extern u16 dds3VblanksPerFrame;
extern u32 dds3GetKernelCount(void);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ s32 unk_0004;
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ s32 unk_000C;
    /* 0x0010 */ sdfTexHandle_t *textureHandle;
    /* 0x0014 */ u8 reserved0014[0xD0];
    /* 0x00E4 */ s32 reserved00E4;
    /* 0x00E8 */ s32 reserved00E8;
    /* 0x00EC */ s32 scrollY;
    /* 0x00F0 */ s32 reserved00F0;
    /* 0x00F4 */ s32 reserved00F4;
    /* 0x00F8 */ f32 initFloat;
    /* 0x00FC */ s32 unk_00FC;
    /* 0x0100 */ s32 unk_0100;
    /* 0x0104 */ s32 unk_0104;
    /* 0x0108 */ s32 unk_0108;
    /* 0x010C */ s32 unk_010C;
    /* 0x0110 */ s32 unk_0110;
    /* 0x0114 */ s32 unk_0114;
    /* 0x0118 */ s32 unk_0118;
} stfUiWork_t;

extern stfUiWork_t *oStaff_UI;

extern void stfFreeTextures(void);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

typedef struct
{
    /* 0x0000 */ const char *text0;
    /* 0x0004 */ const char *text1;
    /* 0x0008 */ s8 style0;
    /* 0x0009 */ s8 style1;
    /* 0x000A */ u8 unk_0A[2];
    /* 0x000C */ s32 y;
} stfStaffRoll_t;

extern stfStaffRoll_t stfStaffRollTable[0xB5];
extern char stfTextFormat[];
#include "sdk_libc.h"

extern u32 stfDrawText(s32 style, u32 color, const char *text);
extern s32 stfDrawStaffText(f32 x, f32 y, s32 style, u32 color, const char *text);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern void GetoStaffUI(void);
extern void dds3AdminiCallSequence(s32 kind, void *arg, u32 size, s32 mode);

s32 stfMain2(dds3ProcessID_t *id)
{
    char buf[128];
    s32 arg;
    s32 i;
    s32 y;
    s32 w;
    s32 state;

    for (i = 0; i < 0xB5; i++) {
        y = (s32)((u32)stfStaffRollTable[i].y
                  - (u32)(oStaff_UI->scrollY / 2) + 0x1C0u);
        if (i == 0xB4) {
            if (y < 0xE0) {
                y = 0xE0;
            }
        }
        if (y >= -0x1E && y <= 0x1C0) {
            if (stfStaffRollTable[i].text0 != NULL) {
                if (stfStaffRollTable[i].text1 == NULL) {
                    sprintf(buf, stfTextFormat, stfStaffRollTable[i].text0);
                    w = stfDrawText(1, 0x80808080, buf);
                    {
                        /* Keep the half-width through the centered coordinate
                         * calculation; the final float argument is its consumer. */
                        FAKEMATCH_ASM_REG(s32, centeredX, "$3");
                        centeredX = w / 2;
                        centeredX = 0x100 - centeredX;
                        stfDrawStaffText(centeredX, y, stfStaffRollTable[i].style0, 0x80808080, buf);
                    }
                    continue;
                }
                sprintf(buf, stfTextFormat, stfStaffRollTable[i].text0);
                w = stfDrawText(1, 0x80808080, buf);
                stfDrawStaffText((s32)(0xF1u - (u32)w), y, stfStaffRollTable[i].style0, 0x80808080, buf);
            }
            if (stfStaffRollTable[i].text1 != NULL) {
                sprintf(buf, stfTextFormat, stfStaffRollTable[i].text1);
                stfDrawStaffText(271.0f, y, stfStaffRollTable[i].style1, 0x80808080, buf);
            }
        }
    }

    /* Use the one observed state for both bounds, with no extra RAM read. */
    state = oStaff_UI->reserved00E4;
    if (state != 0) {
        if (state < 0) {
            goto done;
        }
        if (state < 3) {
            goto scroll;
        }
        goto done;
    } else {
        dds3FadeIn(0, 0, 0, 0x1E);
        oStaff_UI->reserved00E4 = 2;
        oStaff_UI->unk_0118 = 0;
        oStaff_UI->scrollY = 0;
        oStaff_UI->reserved00F4 = 0;
        {
            /* A live pointer operand keeps this final reset store after
             * the preceding resets, without a general memory barrier. */
            stfUiWork_t *resetWork = oStaff_UI;
            FAKEMATCH_ASM_LIVE(resetWork);
            resetWork->unk_0110 = 0;
        }
        goto done;
    }

scroll:
    oStaff_UI->scrollY = (s32)((u32)oStaff_UI->scrollY + SDF_FRAME_DUE(dds3GetKernelCount()));
    GetoStaffUI();
    if (oStaff_UI->scrollY ==
        (s32)((u32)((s32)((u32)stfStaffRollTable[0xB4].y + 0xE0u) / 0x1E)
              * 0x3Cu + 0x12Cu)) {
        dds3FadeOut(0, 0, 0, 0x28);
    }
    {
        /* Preserve the actual wrapped numerator/result lifetime. Compute the
         * independent scroll quotient first to retain the native divide order.
         * These local constraints reconstruct codegen, not original spelling. */
        FAKEMATCH_ASM_REG(s32, endRow, "$2");
        s32 scrollBlock;
        endRow = (s32)((u32)stfStaffRollTable[0xB4].y + 0xE0u);
        scrollBlock = oStaff_UI->scrollY / 0x3C;
        endRow = endRow / 0x1E + 6;
        if (endRow < scrollBlock) {
            arg = 0x2B4;
            dds3AdminiCallSequence(7, &arg, 4, 0);
        }
    }
done:
    /* Qualified ee-gcc 2.96 sched2 dumps show this one-pass return
     * retains the loop-end/return-use dependency ahead of the restores,
     * giving the native zero-return delay fills. This is a compiler-shape
     * reconstruction, not evidence of the original source spelling. */
    do {
        return 0;
    } while (0);
}

extern void stfGetUIField(s32, s32, s32);
extern void ObjDispOff(void);
extern void rsetOtOfs(s32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32);
extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);

void GetoStaffUI(void) {
    s32 elapsed;
    s32 field;
    s32 fieldCount = oStaff_UI->unk_0110;
    s32 drawY;

    if (fieldCount < 15) {
        elapsed = oStaff_UI->scrollY - 0x780;
        if (elapsed >= 0) {
            field = elapsed / 0x450;
            if (field >= fieldCount) {
                field = 1;
                if ((fieldCount & 1) == 0) {
                    field = -1;
                }
                stfGetUIField(fieldCount, 0, field);
                oStaff_UI->unk_0110++;
            }
        }
    }

    ObjDispOff();
    drawY = oStaff_UI->scrollY;
    if (drawY < 0x1CB9) {
        return;
    }
    drawY -= 0x1CB8;
    drawY = 0x1C0 - (drawY / 2);
    if (drawY < -0x3F) {
        return;
    }

    rsetOtOfs(0x47);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    DrwSprite2D(0x108, drawY, 0x100, 0x80, 0, 0, 0x100, 0x80, 0x80808080,
                oStaff_UI->textureHandle);
}

extern void stfResetUI(void);
extern void DrwSprite2DC(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                         u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0,
                      u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2,
                      u32 r3, u32 g3, u32 b3, u32 a3);

void ObjDispOff(void)
{
    s32 step;
    /* Keep the live clamp input/result together through regmove. This value
     * dies at the first cover test, before any call; no empty operand is needed. */
    FAKEMATCH_ASM_REG(s32, cover, "$2");
    s32 width;
    s32 x;
    s32 fade;
    s32 alpha;

    if (oStaff_UI->unk_000C == 0) {
        return;
    }
    {
        s32 normalStep = 1;
        step = 2;
        if (oStaff_UI->unk_0110 != 0xF) {
            step = normalStep;
        }
    }
    if (oStaff_UI->unk_0100 >= 0x450) {
        stfResetUI();
        return;
    }

    rsetOtOfs(0x47);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    rsetAlpha(0);
    cover = (s32)((u32)oStaff_UI->unk_0100 * 3u);
    if (cover > 0x200) {
        cover = 0x200;
    }
    if (cover >= 0x200) {
        oStaff_UI->unk_0104 = (s32)((u32)oStaff_UI->unk_0104 + 2u * SDF_FRAME_DUE(dds3GetKernelCount()));
        if (oStaff_UI->unk_0104 > 0x80) {
            oStaff_UI->unk_0104 = 0x80;
        }
        alpha = oStaff_UI->unk_0104;
        rsetAlpha(0);
        DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                    ((u32)alpha << 24) | 0xA0A0A0, EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_000C));
        DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                    0x80A0A0A0, EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0008));
    }
    rsetAlpha(1);
    if (oStaff_UI->unk_010C < 0) {
        /* Keep the actual frame input/result together through the EE multiply
         * expansion. Its last use is x, before any drawing call. */
        FAKEMATCH_ASM_REG(s32, slide, "$2");
        width = 0x100;
        fade = 0;
        slide = oStaff_UI->unk_0100;
        slide = (s32)((u32)slide * 3u);
        x = (s32)(0x200u - (u32)slide);
        if (x >= -0xFF) {
            if (x < 0) {
                fade = -x;
                x = 0;
                width = 0x100 - fade;
                fade = fade / 2;
            }
            DrwSprite2DC(x, 0, width, 0x1C0, x, 0, width, 0x1C0,
                         ((u32)fade << 24) | 0xFFFFFF, 0x80FFFFFF, 0x80FFFFFF,
                         ((u32)fade << 24) | 0xFFFFFF,
                         EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
            if ((s32)((u32)x + (u32)width) < 0x200) {
                DrwSprite2D((s32)((u32)x + (u32)width), 0, (s32)(0x200u - ((u32)x + (u32)width)), 0x1C0,
                            (s32)((u32)x + (u32)width), 0, (s32)(0x200u - ((u32)x + (u32)width)), 0x1C0,
                            0x80FFFFFF, EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
            }
        } else {
            DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80FFFFFF,
                        EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
        }
    } else {
        /* Local reconstruction aids for the input and adjacent product/test
         * lifetimes. Both values die before the drawing calls. */
        FAKEMATCH_ASM_REG(s32, slice, "$2");
        FAKEMATCH_ASM_REG(s32, frame, "$4");
        fade = 0;
        width = 0x300;
        frame = oStaff_UI->unk_0100;
        slice = (s32)((u32)frame * 3u);
        if (slice <= 0x300) {
            width = slice;
        }
        slice = width < 0x100;
        if (slice) {
            fade = width / 2;
            DrwSprite2DC(0, 0, width, 0x1C0, 0, 0, width, 0x1C0,
                         ((u32)fade << 24) | 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
                         ((u32)fade << 24) | 0xFFFFFF,
                         EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
        } else if (width < 0x300) {
            x = width - 0x100;
            if (width >= 0x201) {
                /* The real numerator becomes the draw opacity. */
                fade = width - 0x200;
                /* Hold the real numerator at this point for EE scheduling;
                 * this empty operand is not a memory barrier. */
                FAKEMATCH_ASM_LIVE(fade);
                fade /= 2;
                width = 0x200 - x;
            } else {
                width = 0x100;
            }
            DrwSprite2DC(x, 0, width, 0x1C0, x, 0, width, 0x1C0,
                         0x80FFFFFF, ((u32)fade << 24) | 0xFFFFFF,
                         ((u32)fade << 24) | 0xFFFFFF, 0x80FFFFFF,
                         EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
            DrwSprite2D(0, 0, x, 0x1C0, 0, 0, x, 0x1C0, 0x80FFFFFF,
                        EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
        } else {
            DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80FFFFFF,
                        EE_ADDR_AS(sdfTexHandle_t *, oStaff_UI->unk_0004));
        }
    }
    rsetAlpha(0);
    if (oStaff_UI->unk_0100 >= 0x3BB) {
        if (oStaff_UI->unk_0114 < 0x80) {
            oStaff_UI->unk_0114 = oStaff_UI->unk_0114 + 2 / step * SDF_FRAME_DUE(dds3GetKernelCount());
        } else {
            oStaff_UI->unk_0114 = 0x80;
        }
        alpha = oStaff_UI->unk_0114;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 0x80) {
            alpha = 0x80;
        }
        aDrawBoxG(0, 0, 0x200, 0xE0, 0, 0, 0, alpha, 0, 0, 0, alpha,
                  0, 0, 0, alpha, 0, 0, 0, alpha);
    }
    oStaff_UI->unk_0100 = (s32)((u32)oStaff_UI->unk_0100 + SDF_FRAME_DUE(dds3GetKernelCount()));
}

extern s32 p2sdGetPcmState(void);

extern void p2sdSyncSound(void);

extern void stfFreeFont(void);

void stfDestroy(void)
{
    dds3VblanksPerFrame = 2;
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    stfFreeTextures();
    stfFreeFont();
    sdfMemFreeDirect(oStaff_UI->allocation);
    oStaff_UI = NULL;
}

extern void stfFreeFiles(void);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern s32 p2sdLoadPcmFile(const char *name, u8 channel);
extern u32 dds3BackColor;
extern s32 stfMain2(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 drawTimer,
                              dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit, void *work);

saddr_t stfShutDown(void) {
    sdfMemHandle_t *handle;
    stfUiWork_t *work;

    dds3BackColor = 0x80000000;
    handle = sdfMemAllocDirect(0x11C);
    work = sdfMemLock(handle);
    oStaff_UI = work;
    work->allocation = handle;
    work->initFloat = 0.09999999404f;
    work->unk_000C = work->unk_0008 = work->unk_0004 = work->reserved00F4 =
        work->reserved00F0 = work->scrollY = work->reserved00E8 = work->reserved00E4 = 0;
    stfFreeFiles();
    p2sdLoadPcmFile("STAFF.ADB", 0x7F);
    dds3VblanksPerFrame = 1;
    return EE_ADDR_S(dds3StartProcess(stfProcName, 0x2B19, 0, 0,
                                 stfMain2, (dds3ProcessFunc_t)stfDestroy, NULL));
}

s32 stfExit(void)
{
    stfShutDown();
    return -1;
}

s32 stfInit(void)
{
    dds3KillProcessName(stfProcName, 1);
    return 0;
}

INCLUDE_RODATA(const s32, "staff/stfMain", stfProcName);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F110);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F130);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F150);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F170);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F190);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F1A8);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F1C0);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F1E0);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F1F8);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F210);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F228);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F248);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F260);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F278);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F290);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F2B0);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F2C8);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F2E8);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F308);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F320);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F340);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F360);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F378);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F390);

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F3B0);

INCLUDE_SDATA(const s32, "staff/stfMain", stfTextFormat);

