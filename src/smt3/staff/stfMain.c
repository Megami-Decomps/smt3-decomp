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

extern const char stfProcName[];

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
    oStaff_UI->scrollY = (s32)((u32)oStaff_UI->scrollY + SDF_FRAME_DUE());
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
        oStaff_UI->unk_0104 = (s32)((u32)oStaff_UI->unk_0104 + 2u * SDF_FRAME_DUE());
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
            oStaff_UI->unk_0114 = oStaff_UI->unk_0114 + 2 / step * SDF_FRAME_DUE();
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
    oStaff_UI->unk_0100 = (s32)((u32)oStaff_UI->unk_0100 + SDF_FRAME_DUE());
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

const char stfProcName[16] = "staffProc";

const char stfMovieTitle2M[32] = "cdrom0:/movie/title2M.IPS";

const char stfMovieTitle1M[32] = "cdrom0:/movie/title1M.IPS";

const char stfMovieE616M[32] = "cdrom0:/movie/e616_m.IPS";

const char stfMoviePeephole[32] = "cdrom0:/movie/peephole.IPS";

const char stfMovieE731[24] = "cdrom0:/movie/e731.ipu";

const char stfMovieE727[24] = "cdrom0:/movie/e727.IPS";

const char stfMovieTitle3[32] = "cdrom0:/movie/title3.ipu";

const char stfMovieE686[24] = "cdrom0:/movie/e686.IPS";

const char stfMovieE684[24] = "cdrom0:/movie/e684.IPS";

const char stfMovieE688[24] = "cdrom0:/movie/e688.IPS";

const char stfMovieTitle2[32] = "cdrom0:/movie/title2.IPS";

const char stfMovieMenu[24] = "cdrom0:/movie/menu.IPS";

const char stfMovieE676[24] = "cdrom0:/movie/e676.IPS";

const char stfMovieE617[24] = "cdrom0:/movie/e617.IPS";

const char stfMovieTitle1[32] = "cdrom0:/movie/title1.IPS";

const char stfMovieFile_e674[24] = "cdrom0:/movie/e674.IPS";

const char stfMovieFile_e673_020[32] = "cdrom0:/movie/e673_020.IPS";

const char stfMovieFile_gOver[32] = "cdrom0:/movie/g_over.ipu";

const char stfMovieFile_e658[24] = "cdrom0:/movie/e658.IPS";

const char stfMovieFile_e804_i[32] = "cdrom0:/movie/e804_i.IPS";

const char stfMovieFile_e804_f[32] = "cdrom0:/movie/e804_f.IPS";

const char stfMovieFile_e804[24] = "cdrom0:/movie/e804.IPS";

const char stfMovieFile_e616[24] = "cdrom0:/movie/e616.IPS";

const char stfMovieFile_e500_002[32] = "cdrom0:/movie/e500_002.ipu";

const char stfMovieFile_dds[24] = "cdrom0:/movie/dds.IPS";

char stfTextFormat[4] = "%s";


/* The staff text remains in the native string providers. */
extern const char D_0053DF90[];
extern const char D_0053DFA8[];
extern const char D_0053DFB8[];
extern const char D_0053DFC8[];
extern const char D_0053DFD8[];
extern const char D_0053DFF0[];
extern const char D_0053E008[];
extern const char D_0053E020[];
extern const char D_0053E030[];
extern const char D_0053E040[];
extern const char D_0053E050[];
extern const char D_0053E060[];
extern const char D_0053E078[];
extern const char D_0053E088[];
extern const char D_0053E0A0[];
extern const char D_0053E0B0[];
extern const char D_0053E0C0[];
extern const char D_0053E0D0[];
extern const char D_0053E0E0[];
extern const char D_0053E0F8[];
extern const char D_0053E108[];
extern const char D_0053E118[];
extern const char D_0053E130[];
extern const char D_0053E148[];
extern const char D_0053E158[];
extern const char D_0053E168[];
extern const char D_0053E178[];
extern const char D_0053E190[];
extern const char D_0053E1A8[];
extern const char D_0053E1C0[];
extern const char D_0053E1D0[];
extern const char D_0053E1E0[];
extern const char D_0053E1F0[];
extern const char D_0053E200[];
extern const char D_0053E218[];
extern const char D_0053E228[];
extern const char D_0053E240[];
extern const char D_0053E250[];
extern const char D_0053E260[];
extern const char D_0053E270[];
extern const char D_0053E280[];
extern const char D_0053E290[];
extern const char D_0053E2A8[];
extern const char D_0053E2C0[];
extern const char D_0053E2D0[];
extern const char D_0053E2E0[];
extern const char D_0053E2F0[];
extern const char D_0053E308[];
extern const char D_0053E318[];
extern const char D_0053E328[];
extern const char D_0053E338[];
extern const char D_0053E348[];
extern const char D_0053E360[];
extern const char D_0053E370[];
extern const char D_0053E380[];
extern const char D_0053E390[];
extern const char D_0053E3A8[];
extern const char D_0053E3B8[];
extern const char D_0053E3C8[];
extern const char D_0053E3D8[];
extern const char D_0053E3F0[];
extern const char D_0053E400[];
extern const char D_0053E410[];
extern const char D_0053E420[];
extern const char D_0053E430[];
extern const char D_0053E440[];
extern const char D_0053E458[];
extern const char D_0053E468[];
extern const char D_0053E478[];
extern const char D_0053E488[];
extern const char D_0053E498[];
extern const char D_0053E4A8[];
extern const char D_0053E4B8[];
extern const char D_0053E4C8[];
extern const char D_0053E4D8[];
extern const char D_0053E4E8[];
extern const char D_0053E500[];
extern const char D_0053E510[];
extern const char D_0053E520[];
extern const char D_0053E530[];
extern const char D_0053E540[];
extern const char D_0053E550[];
extern const char D_0053E560[];
extern const char D_0053E570[];
extern const char D_0053E580[];
extern const char D_0053E590[];
extern const char D_0053E5A0[];
extern const char D_0053E5B0[];
extern const char D_0053E5C0[];
extern const char D_0053E5D0[];
extern const char D_0053E5E8[];
extern const char D_0053E5F8[];
extern const char D_0053E608[];
extern const char D_0053E618[];
extern const char D_0053E630[];
extern const char D_0053E648[];
extern const char D_0053E660[];
extern const char D_0053E678[];
extern const char D_0053E690[];
extern const char D_0053E6A0[];
extern const char D_0053E6B8[];
extern const char D_0053E6C8[];
extern const char D_0053E6E0[];
extern const char D_0053E6F0[];
extern const char D_0053E700[];
extern const char D_0053E710[];
extern const char D_0053E720[];
extern const char D_0053E730[];
extern const char D_0053E740[];
extern const char D_0053E750[];
extern const char D_0053E760[];
extern const char D_0053E770[];
extern const char D_0053E780[];
extern const char D_0053E798[];
extern const char D_0053E7B0[];
extern const char D_0053E7C0[];
extern const char D_0053E7D8[];
extern const char D_0053E7E8[];
extern const char D_0053E7F8[];
extern const char D_0053E808[];
extern const char D_0053E818[];
extern const char D_0053E828[];
extern const char D_0053E838[];
extern const char D_0053E848[];
extern const char D_0053E878[];
extern const char D_0053E890[];
extern const char D_0053E8A8[];
extern const char D_0053E8B8[];
extern const char D_0053E8D0[];
extern const char D_0053E8E0[];
extern const char D_0053E8F8[];
extern const char D_0053E910[];
extern const char D_0053E920[];
extern const char D_0053E930[];
extern const char D_0053E950[];
extern const char D_0053E968[];
extern const char D_0053E978[];
extern const char D_0053E988[];
extern const char D_0053E998[];
extern const char D_0053E9A8[];
extern const char D_0053E9B8[];
extern const char D_0053E9D0[];
extern const char D_0053E9E8[];
extern const char D_0053EA00[];
extern const char D_0053EA10[];
extern const char D_0053EA20[];
extern const char D_0053EA30[];
extern const char D_0053EA48[];
extern const char D_0053EA60[];
extern const char D_0053EA78[];
extern const char D_0053EA88[];
extern const char D_0053EA98[];
extern const char D_0053EAA8[];
extern const char D_0053EAB8[];
extern const char D_0053EAD0[];
extern const char D_0053EAE0[];
extern const char D_0053EAF0[];
extern const char D_0053EB08[];
extern const char D_0053EB18[];
extern const char D_0053EB30[];
extern const char D_0053EB40[];
extern const char D_0053EB58[];
extern const char D_0053EB68[];
extern const char D_0053EB80[];
extern const char D_0053EB90[];
extern const char D_0053EBA0[];
extern const char D_0053EBB0[];
extern const char D_0053EBC0[];
extern const char D_0053EBD0[];
extern const char D_0053EBE0[];
extern const char D_0053EBF0[];
extern const char D_0053EC00[];
extern const char D_0053EC10[];
extern const char D_0053EC28[];
extern const char D_0053EC38[];
extern const char D_0053EC48[];
extern const char D_0053EC58[];
extern const char D_0053EC70[];
extern const char D_0053EC80[];
extern const char D_0053EC90[];
extern const char D_0053ECA0[];
extern const char D_0053ECB0[];
extern const char D_0053ECC0[];
extern const char D_0053ECD0[];
extern const char D_0054FD60[];
extern const char D_0054FD68[];
extern const char D_0054FD70[];
extern const char D_0054FD78[];
extern const char D_0054FD80[];
extern const char D_0054FD88[];

/* Credit lines in scroll order, using the two native text columns. */
stfStaffRoll_t stfStaffRollTable[0xB5] = {
    { D_0053ECD0, D_0053ECC0, 0, 1, { 0, 0 }, 24 },
    { D_0053ECB0, D_0053ECA0, 0, 1, { 0, 0 }, 456 },
    { D_0053EC90, D_0053EC80, 0, 1, { 0, 0 }, 576 },
    { NULL, D_0053EC70, 0, 1, { 0, 0 }, 600 },
    { NULL, D_0053ECA0, 0, 1, { 0, 0 }, 648 },
    { D_0053EC58, D_0053EC48, 0, 1, { 0, 0 }, 744 },
    { D_0053EC38, D_0053EC28, 0, 1, { 0, 0 }, 840 },
    { D_0053EC10, D_0053EC00, 0, 1, { 0, 0 }, 912 },
    { D_0053EBF0, D_0053EBE0, 0, 1, { 0, 0 }, 1008 },
    { D_0053EBD0, D_0053EC70, 0, 1, { 0, 0 }, 1224 },
    { D_0053EBC0, D_0053EC80, 0, 1, { 0, 0 }, 1320 },
    { NULL, D_0053EBB0, 0, 1, { 0, 0 }, 1344 },
    { NULL, D_0053EC48, 0, 1, { 0, 0 }, 1392 },
    { D_0053EBA0, D_0053EB90, 0, 1, { 0, 0 }, 1464 },
    { NULL, D_0053EB80, 0, 1, { 0, 0 }, 1488 },
    { NULL, D_0053EB68, 0, 1, { 0, 0 }, 1512 },
    { D_0053EB58, D_0053EC48, 0, 1, { 0, 0 }, 1728 },
    { D_0053EB40, D_0053EB30, 0, 1, { 0, 0 }, 1824 },
    { D_0053EB18, D_0053EB08, 0, 1, { 0, 0 }, 1872 },
    { D_0053EAF0, D_0053EAE0, 0, 1, { 0, 0 }, 1920 },
    { NULL, D_0053EAD0, 0, 1, { 0, 0 }, 1944 },
    { D_0053EAB8, D_0053EC28, 0, 1, { 0, 0 }, 2160 },
    { D_0053EAA8, D_0053EA98, 0, 1, { 0, 0 }, 2232 },
    { NULL, D_0053EA88, 0, 1, { 0, 0 }, 2256 },
    { NULL, D_0053EA78, 0, 1, { 0, 0 }, 2280 },
    { NULL, D_0053EA60, 0, 1, { 0, 0 }, 2304 },
    { D_0053EA48, D_0053EC00, 0, 1, { 0, 0 }, 2376 },
    { D_0053EA30, D_0053EA20, 0, 1, { 0, 0 }, 2448 },
    { NULL, D_0053EA10, 0, 1, { 0, 0 }, 2472 },
    { NULL, D_0053EA00, 0, 1, { 0, 0 }, 2496 },
    { D_0053E9E8, D_0053E9D0, 0, 1, { 0, 0 }, 2568 },
    { D_0053E9B8, D_0053E9A8, 0, 1, { 0, 0 }, 2640 },
    { D_0053E998, D_0053E988, 0, 1, { 0, 0 }, 2712 },
    { NULL, D_0053E978, 0, 1, { 0, 0 }, 2736 },
    { NULL, D_0053E968, 0, 1, { 0, 0 }, 2760 },
    { NULL, D_0053E950, 0, 1, { 0, 0 }, 2784 },
    { D_0053E930, D_0053E920, 0, 1, { 0, 0 }, 2856 },
    { D_0053E910, D_0053E8F8, 0, 1, { 0, 0 }, 2928 },
    { D_0053E8E0, D_0053E8D0, 0, 1, { 0, 0 }, 3000 },
    { D_0053E8B8, D_0053E8A8, 0, 1, { 0, 0 }, 3216 },
    { NULL, D_0053E890, 0, 1, { 0, 0 }, 3240 },
    { D_0053E878, D_0054FD88, 0, 1, { 0, 0 }, 3672 },
    { D_0053E848, NULL, 0, 1, { 0, 0 }, 3744 },
    { NULL, D_0053E838, 0, 0, { 0, 0 }, 3792 },
    { NULL, D_0053E828, 0, 1, { 0, 0 }, 3816 },
    { NULL, D_0054FD80, 0, 0, { 0, 0 }, 3864 },
    { NULL, D_0053E818, 0, 1, { 0, 0 }, 3888 },
    { NULL, D_0053E808, 0, 1, { 0, 0 }, 3912 },
    { NULL, D_0053E7F8, 0, 1, { 0, 0 }, 3936 },
    { NULL, D_0053E7E8, 0, 1, { 0, 0 }, 3960 },
    { NULL, D_0053E7D8, 0, 1, { 0, 0 }, 3984 },
    { D_0053E7C0, D_0053E7B0, 0, 0, { 0, 0 }, 4200 },
    { NULL, D_0053E798, 0, 0, { 0, 0 }, 4224 },
    { D_0053E780, D_0053E770, 0, 1, { 0, 0 }, 4392 },
    { NULL, D_0053E760, 0, 1, { 0, 0 }, 4416 },
    { NULL, D_0053E750, 0, 1, { 0, 0 }, 4440 },
    { D_0053E740, D_0053EB90, 0, 1, { 0, 0 }, 4512 },
    { D_0053E730, D_0053E720, 0, 1, { 0, 0 }, 4560 },
    { D_0053E710, D_0053E700, 0, 1, { 0, 0 }, 4584 },
    { NULL, D_0053E6F0, 0, 1, { 0, 0 }, 4608 },
    { D_0053E6E0, D_0053E6C8, 0, 0, { 0, 0 }, 4656 },
    { NULL, D_0053E6B8, 0, 1, { 0, 0 }, 4680 },
    { NULL, D_0053E6A0, 0, 1, { 0, 0 }, 4704 },
    { NULL, D_0053E690, 0, 1, { 0, 0 }, 4728 },
    { NULL, D_0053E678, 0, 1, { 0, 0 }, 4752 },
    { NULL, D_0053E660, 0, 1, { 0, 0 }, 4776 },
    { D_0053E648, D_0053E630, 0, 0, { 0, 0 }, 4872 },
    { NULL, D_0053E618, 0, 1, { 0, 0 }, 4896 },
    { D_0054FD78, D_0053E608, 0, 1, { 0, 0 }, 4944 },
    { NULL, D_0053E5F8, 0, 1, { 0, 0 }, 4968 },
    { NULL, D_0053E5E8, 0, 1, { 0, 0 }, 4992 },
    { NULL, D_0053E9D0, 0, 1, { 0, 0 }, 5016 },
    { D_0053E5D0, D_0053E5C0, 0, 1, { 0, 0 }, 5112 },
    { D_0053E5B0, D_0053E5A0, 0, 1, { 0, 0 }, 5184 },
    { NULL, D_0053E590, 0, 1, { 0, 0 }, 5208 },
    { NULL, D_0053E580, 0, 1, { 0, 0 }, 5232 },
    { NULL, D_0053E570, 0, 1, { 0, 0 }, 5256 },
    { D_0053E560, D_0053E550, 0, 1, { 0, 0 }, 5328 },
    { NULL, D_0053E540, 0, 1, { 0, 0 }, 5352 },
    { NULL, D_0053E530, 0, 1, { 0, 0 }, 5376 },
    { NULL, D_0053E520, 0, 1, { 0, 0 }, 5400 },
    { D_0054FD70, D_0053E510, 0, 1, { 0, 0 }, 5472 },
    { NULL, D_0053E500, 0, 1, { 0, 0 }, 5496 },
    { NULL, D_0053E4E8, 0, 1, { 0, 0 }, 5520 },
    { NULL, D_0053E4D8, 0, 1, { 0, 0 }, 5544 },
    { NULL, D_0053E4C8, 0, 1, { 0, 0 }, 5568 },
    { NULL, D_0053E4B8, 0, 1, { 0, 0 }, 5592 },
    { NULL, D_0053E4A8, 0, 1, { 0, 0 }, 5616 },
    { NULL, D_0053E498, 0, 1, { 0, 0 }, 5640 },
    { NULL, D_0053E488, 0, 1, { 0, 0 }, 5664 },
    { D_0053E478, NULL, 0, 1, { 0, 0 }, 5760 },
    { D_0053ECB0, D_0053E468, 0, 1, { 0, 0 }, 5808 },
    { D_0053EC90, D_0053EC80, 0, 1, { 0, 0 }, 5832 },
    { NULL, D_0053E458, 0, 1, { 0, 0 }, 5856 },
    { NULL, D_0053E440, 0, 1, { 0, 0 }, 5880 },
    { D_0054FD68, D_0053E430, 0, 1, { 0, 0 }, 5904 },
    { NULL, D_0053ECA0, 0, 1, { 0, 0 }, 5928 },
    { NULL, D_0053EC70, 0, 1, { 0, 0 }, 5952 },
    { NULL, D_0053E420, 0, 1, { 0, 0 }, 5976 },
    { NULL, D_0053E410, 0, 1, { 0, 0 }, 6000 },
    { NULL, D_0053E400, 0, 1, { 0, 0 }, 6024 },
    { NULL, D_0053E3F0, 0, 1, { 0, 0 }, 6048 },
    { NULL, D_0053E3D8, 0, 1, { 0, 0 }, 6072 },
    { NULL, D_0053E3C8, 0, 1, { 0, 0 }, 6096 },
    { NULL, D_0053EB80, 0, 1, { 0, 0 }, 6120 },
    { NULL, D_0053EBB0, 0, 1, { 0, 0 }, 6144 },
    { D_0053EC58, D_0053EC48, 0, 1, { 0, 0 }, 6192 },
    { D_0053EB58, D_0053E3B8, 0, 1, { 0, 0 }, 6216 },
    { D_0053E3A8, D_0053E390, 0, 1, { 0, 0 }, 6240 },
    { NULL, D_0053E380, 0, 1, { 0, 0 }, 6264 },
    { NULL, D_0053E370, 0, 1, { 0, 0 }, 6288 },
    { NULL, D_0053EB08, 0, 1, { 0, 0 }, 6312 },
    { NULL, D_0053EB30, 0, 1, { 0, 0 }, 6336 },
    { D_0053EC38, D_0053EC00, 0, 1, { 0, 0 }, 6384 },
    { D_0053E360, D_0053E348, 0, 1, { 0, 0 }, 6408 },
    { NULL, D_0053EC28, 0, 1, { 0, 0 }, 6432 },
    { NULL, D_0053EA98, 0, 1, { 0, 0 }, 6456 },
    { NULL, D_0053E920, 0, 1, { 0, 0 }, 6480 },
    { NULL, D_0053E8D0, 0, 1, { 0, 0 }, 6504 },
    { NULL, D_0053E8F8, 0, 1, { 0, 0 }, 6528 },
    { NULL, D_0053E9A8, 0, 1, { 0, 0 }, 6552 },
    { NULL, D_0053EA20, 0, 1, { 0, 0 }, 6576 },
    { NULL, D_0053EA88, 0, 1, { 0, 0 }, 6600 },
    { NULL, D_0053EA78, 0, 1, { 0, 0 }, 6624 },
    { NULL, D_0053EA10, 0, 1, { 0, 0 }, 6648 },
    { NULL, D_0053E338, 0, 1, { 0, 0 }, 6672 },
    { NULL, D_0053E988, 0, 1, { 0, 0 }, 6696 },
    { NULL, D_0053E978, 0, 1, { 0, 0 }, 6720 },
    { NULL, D_0053E328, 0, 1, { 0, 0 }, 6744 },
    { NULL, D_0053E318, 0, 1, { 0, 0 }, 6768 },
    { NULL, D_0053E308, 0, 1, { 0, 0 }, 6792 },
    { NULL, D_0053E2F0, 0, 1, { 0, 0 }, 6816 },
    { NULL, D_0053E2E0, 0, 1, { 0, 0 }, 6840 },
    { NULL, D_0053E2D0, 0, 1, { 0, 0 }, 6864 },
    { NULL, D_0053E2C0, 0, 1, { 0, 0 }, 6888 },
    { NULL, D_0053E2A8, 0, 1, { 0, 0 }, 6912 },
    { NULL, D_0053E290, 0, 1, { 0, 0 }, 6936 },
    { NULL, D_0053E280, 0, 1, { 0, 0 }, 6960 },
    { NULL, D_0053E828, 0, 1, { 0, 0 }, 6984 },
    { NULL, D_0053E270, 0, 1, { 0, 0 }, 7008 },
    { D_0053E9E8, D_0053E9D0, 0, 1, { 0, 0 }, 7032 },
    { D_0053EBF0, D_0053EBE0, 0, 1, { 0, 0 }, 7080 },
    { D_0053E260, D_0053E8A8, 0, 1, { 0, 0 }, 7104 },
    { NULL, D_0053E890, 0, 1, { 0, 0 }, 7128 },
    { D_0053E250, D_0053E240, 0, 1, { 0, 0 }, 7176 },
    { NULL, D_0053E228, 0, 1, { 0, 0 }, 7200 },
    { NULL, D_0053E218, 0, 1, { 0, 0 }, 7224 },
    { NULL, D_0053E200, 0, 0, { 0, 0 }, 7248 },
    { NULL, D_0053E1F0, 0, 0, { 0, 0 }, 7272 },
    { NULL, D_0053E1E0, 0, 0, { 0, 0 }, 7296 },
    { NULL, D_0053E7B0, 0, 0, { 0, 0 }, 7320 },
    { NULL, D_0053E798, 0, 0, { 0, 0 }, 7344 },
    { D_0053E1D0, D_0053E390, 0, 1, { 0, 0 }, 7488 },
    { NULL, D_0053E1C0, 0, 1, { 0, 0 }, 7512 },
    { NULL, D_0053E3B8, 0, 1, { 0, 0 }, 7536 },
    { NULL, D_0053E380, 0, 1, { 0, 0 }, 7560 },
    { NULL, D_0053E370, 0, 1, { 0, 0 }, 7584 },
    { NULL, D_0053E1A8, 0, 1, { 0, 0 }, 7608 },
    { NULL, D_0053E3D8, 0, 1, { 0, 0 }, 7632 },
    { NULL, D_0053E190, 0, 1, { 0, 0 }, 7656 },
    { NULL, D_0053E178, 0, 0, { 0, 0 }, 7704 },
    { D_0053E168, D_0053E158, 0, 1, { 0, 0 }, 7968 },
    { D_0053E148, D_0053E468, 0, 1, { 0, 0 }, 8136 },
    { NULL, D_0053E130, 0, 1, { 0, 0 }, 8184 },
    { D_0053E118, D_0053E108, 0, 1, { 0, 0 }, 8424 },
    { NULL, D_0053E5C0, 0, 1, { 0, 0 }, 8448 },
    { D_0053E838, D_0053E0F8, 0, 1, { 0, 0 }, 8856 },
    { D_0053E0E0, NULL, 0, 1, { 0, 0 }, 9210 },
    { D_0053E0D0, D_0053E0C0, 0, 1, { 0, 0 }, 9276 },
    { D_0053E0B0, D_0053E0A0, 0, 1, { 0, 0 }, 9316 },
    { D_0053E088, D_0053E078, 0, 1, { 0, 0 }, 9356 },
    { D_0053E060, D_0053E050, 0, 1, { 0, 0 }, 9396 },
    { D_0053E040, D_0053E030, 0, 1, { 0, 0 }, 9436 },
    { NULL, D_0053E0A0, 0, 1, { 0, 0 }, 9460 },
    { D_0053E020, D_0053E008, 0, 1, { 0, 0 }, 9500 },
    { D_0053DFF0, D_0053DFD8, 0, 1, { 0, 0 }, 9540 },
    { D_0054FD60, D_0053DFC8, 0, 1, { 0, 0 }, 9580 },
    { NULL, D_0053DFB8, 0, 1, { 0, 0 }, 9604 },
    { NULL, D_0053DFA8, 0, 1, { 0, 0 }, 9628 },
    { NULL, D_0053E030, 0, 1, { 0, 0 }, 9652 },
    { D_0053DF90, NULL, 0, 1, { 0, 0 }, 9924 },
};
