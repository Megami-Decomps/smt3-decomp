#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern char D_005327B0[];
#include "sdk_libc.h"

extern s32 fldTitleState, D_0054EF8C, D_0054EF90, D_0054EF94;
extern sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *texture);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 a, s32 b,
    dds3ProcessFunc_t process, dds3ProcessFunc_t shutdown, void *work);
extern s32 fldTitle_Process(dds3ProcessID_t *id);

dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern char D_00532820[];

extern sdfTexHandle_t *fldTitleMiniTex;

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern sdfTexHandle_t *fldTitleMiniWork;

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void fldTitleStart(s32 fld, s32 waittime);

extern sdfTexHandle_t *D_0054EF98;

extern sdfTexHandle_t *D_0054EF9C;

extern sdfTexHandle_t *D_0054EFA0;

extern sdfTexHandle_t *D_0054EFA4;


extern char D_005327A0[];

s32 fldTitleChk(void) {
    return dds3SearchProcessName(D_005327B0) != 0;
}

extern s32 fldProcCampChk(void);
extern void rsetOtOfs(s32);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32);
extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32,
                        sdfTexHandle_t *);
extern void aDrawBoxG(s32, s32, s32, s32,
                      u32, u32, u32, u32, u32, u32, u32, u32,
                      u32, u32, u32, u32, u32, u32, u32, u32);
extern s32 fldSnowActive;
extern s32 fldTitleMiniState;

/* NON_MATCHING: 755 of 757 instruction words match at the native 3028-byte size. */
#ifdef NON_MATCHING
s32 fldTitle_Process(dds3ProcessID_t *id)
{
    s32 height;

    rsetOtOfs(0x47);
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(0);
    switch (D_0054EF8C) {
    case 0: {
        D_0054EF94--;
        if (D_0054EF94 <= 0) {
            D_0054EF8C = 1;
            fldTitleState = 0;
        }
        break;
    }
    case 1: {
        s32 alpha;
        s32 x;
        rsetAlpha(2);
        height = 448 - fldTitleState * 12;
        if (height < 106) height = 106;
        DrwSprite2D(0, 180 - (height >> 1), 512, height, 0, 0, 256, 128,
                    0x10808080, D_0054EFA0);
        rsetAlpha(0);

        if (fldTitleState >= 19) {
            alpha = (fldTitleState * 128 - 0x900) / 10;
            if (alpha > 128) alpha = 128;
            alpha <<= 24;
            alpha |= 0x00808080;
            DrwSprite2D(0x1B, 0x5F, 0x1CA, 0xAC, 0, 0, 0x80, 0x40,
                        alpha, D_0054EF9C);
            DrwSprite2D(0x4B, 0x96, 0x64, 0x3A, 2, 5, 0x64, 0x3A,
                        alpha, D_0054EFA4);
            DrwSprite2D(0xAF, 0x96, 0xA2, 0x3A, 0x65, 5, 1, 0x3A,
                        alpha, D_0054EFA4);
            DrwSprite2D(0x151, 0x96, 0x64, 0x3A, 0x65, 5, -0x64, 0x3A,
                        alpha, D_0054EFA4);
        }

        if (fldTitleState >= 19) {
            alpha = (fldTitleState * 128 - 0x900) / 6;
            if (alpha > 128) alpha = 128;
            alpha <<= 24;
            alpha |= 0x00808080;
            x = fldTitleState * 56 - 0x530;
            if (x > 0) x = 0;
            DrwSprite2D(x + 0x11, 0x81, 0x7E, 2, 1, 2, 0x7E, 2,
                        alpha, D_0054EFA4);
            DrwSprite2D(x + 0x8F, 0x81, 0xE2, 2, 0x7E, 2, 1, 2,
                        alpha, D_0054EFA4);
            DrwSprite2D(x + 0x171, 0x81, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                        alpha, D_0054EFA4);

            x = 0x530 - fldTitleState * 56;
            if (x < 0) x = 0;
            DrwSprite2D(x + 0x11, 0xE5, 0x7E, 2, 1, 2, 0x7E, 2,
                        alpha, D_0054EFA4);
            DrwSprite2D(x + 0x8F, 0xE5, 0xE2, 2, 0x7E, 2, 1, 2,
                        alpha, D_0054EFA4);
            DrwSprite2D(x + 0x171, 0xE5, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                        alpha, D_0054EFA4);
        }

        if (fldTitleState >= 20) {
            alpha = (fldTitleState * 128 - 2432) / 15;
            if (alpha > 128) alpha = 128;
            DrwSprite2D(129, 115, 254, 69, 1, 1, 254, 69,
                        0x00808080 | (alpha << 24), D_0054EF98);
        }

        if ((u32)(fldTitleState - 20) < 10) {
            rsetAlpha(1);

            alpha = (fldTitleState * 128 - 0xA00) / 10;
            if (alpha > 128) alpha = 128;
            if (alpha < 0) alpha = 0;
            DrwSprite2D(0x81, 0x73, 0xFE, 0x45, 1, 1, 0xFE, 0x45,
                        0x00807060 | (alpha << 24), D_0054EF98);
            rsetAlpha(0);
        }

        if (fldTitleState >= 30) {
            rsetAlpha(1);

            alpha = (0x30 - fldTitleState) * 128 / 18;
            if (alpha > 128) alpha = 128;
            if (alpha < 0) alpha = 0;
            DrwSprite2D(0x81, 0x73, 0xFE, 0x45, 1, 1, 0xFE, 0x45,
                        0x00807060 | (alpha << 24), D_0054EF98);
            rsetAlpha(0);
        }

        if (++fldTitleState >= 61) {
            fldTitleState = 0;
            D_0054EF8C++;
        }
        break;
    }
    case 2: {
        s32 color;
        rsetAlpha(2);
        DrwSprite2D(0, 0x7F, 0x200, 0x6A, 0, 0, 0x100, 0x80,
                    0x10808080, D_0054EFA0);
        rsetAlpha(0);
        color = 0x80808080;
        DrwSprite2D(0x1B, 0x5F, 0x1CA, 0xAC, 0, 0, 0x80, 0x40,
                    color, D_0054EF9C);
        DrwSprite2D(0x4B, 0x96, 0x64, 0x3A, 2, 5, 0x64, 0x3A,
                    color, D_0054EFA4);
        DrwSprite2D(0xAF, 0x96, 0xA2, 0x3A, 0x65, 5, 1, 0x3A,
                    color, D_0054EFA4);
        DrwSprite2D(0x151, 0x96, 0x64, 0x3A, 0x65, 5, -0x64, 0x3A,
                    color, D_0054EFA4);
        DrwSprite2D(0x11, 0x81, 0x7E, 2, 1, 2, 0x7E, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x8F, 0x81, 0xE2, 2, 0x7E, 2, 1, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x11, 0xE5, 0x7E, 2, 1, 2, 0x7E, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x8F, 0xE5, 0xE2, 2, 0x7E, 2, 1, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x171, 0x81, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x171, 0xE5, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                    color, D_0054EFA4);
        DrwSprite2D(0x81, 0x73, 0xFE, 0x45, 1, 1, 0xFE, 0x45,
                    color, D_0054EF98);

        if (++fldTitleState >= 61) {
            fldTitleState = 0;
            D_0054EF8C++;
        }
        break;
    }
    case 3: {
        s32 alpha;
        s32 x;
        rsetAlpha(2);
        alpha = (20 - fldTitleState) * 16 / 20;
        if (alpha < 0) alpha = 0;
        if (alpha > 128) alpha = 128;
        DrwSprite2D(0, 0x7F, 0x200, 0x6A, 0, 0, 0x100, 0x80,
                    0x00808080 | (alpha << 24), D_0054EFA0);
        rsetAlpha(0);

        {
            s32 color;
            alpha = (10 - fldTitleState) * 128 / 10;
            if (alpha < 0) alpha = 0;
            if (alpha > 128) alpha = 128;
            color = 0x00808080 | (alpha << 24);
            DrwSprite2D(0x1B, 0x5F, 0x1CA, 0xAC, 0, 0, 0x80, 0x40,
                        color, D_0054EF9C);
            DrwSprite2D(0x4B, 0x96, 0x64, 0x3A, 2, 5, 0x64, 0x3A,
                        color, D_0054EFA4);
            DrwSprite2D(0xAF, 0x96, 0xA2, 0x3A, 0x65, 5, 1, 0x3A,
                        color, D_0054EFA4);
            DrwSprite2D(0x151, 0x96, 0x64, 0x3A, 0x65, 5, -0x64, 0x3A,
                        color, D_0054EFA4);

        }
        x = fldTitleState < 5 ? 0 : fldTitleState * 72 - 0x120;
        if (x < 0x190) {
            s32 color = 0x80808080;
            DrwSprite2D(x + 0x11, 0x81, 0x7E, 2, 1, 2, 0x7E, 2,
                        color, D_0054EFA4);
            DrwSprite2D(x + 0x8F, 0x81, 0xE2, 2, 0x7E, 2, 1, 2,
                        color, D_0054EFA4);
            DrwSprite2D(x + 0x171, 0x81, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                        color, D_0054EFA4);
        }

        if (fldTitleState >= 5) {
            s32 displacement = fldTitleState * 72 - 0x120;
            x = -displacement;
        } else {
            x = 0;
        }
        if (x >= -0x18F) {
            s32 color = 0x80808080;
            DrwSprite2D(x + 0x11, 0xE5, 0x7E, 2, 1, 2, 0x7E, 2,
                        color, D_0054EFA4);
            DrwSprite2D(x + 0x8F, 0xE5, 0xE2, 2, 0x7E, 2, 1, 2,
                        color, D_0054EFA4);
            DrwSprite2D(x + 0x171, 0xE5, 0x7E, 2, 0x7E, 2, -0x7E, 2,
                        color, D_0054EFA4);
        }

        alpha = (20 - fldTitleState) * 128 / 20;
        if (alpha < 0) alpha = 0;
        if (alpha > 128) alpha = 128;
        DrwSprite2D(0x81, 0x73, 0xFE, 0x45, 1, 1, 0xFE, 0x45,
                    0x00808080 | (alpha << 24), D_0054EF98);

        if (++fldTitleState >= 61) {
            fldTitleState = 0;
            D_0054EF8C++;
        }
        break;
    }
    default:
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldTitle", fldTitle_Process);
#endif

void fldTitle_Shutdown(void) {
    if (D_0054EF98 != NULL) {
        sdfTexFreeDelayed(D_0054EF98);
        D_0054EF98 = NULL;
    }
    if (D_0054EF9C != NULL) {
        sdfTexFreeDelayed(D_0054EF9C);
        D_0054EF9C = NULL;
    }
    if (D_0054EFA0 != NULL) {
        sdfTexFreeDelayed(D_0054EFA0);
        D_0054EFA0 = NULL;
    }
    if (D_0054EFA4 != NULL) {
        sdfTexFreeDelayed(D_0054EFA4);
        D_0054EFA4 = NULL;
    }
}

INCLUDE_RODATA(const s32, "field/fldTitle", D_005327B0);

void fldTitleStart(s32 fld, s32 waittime)
{
    char path[32];
    u32 data;
    sdfMemHandle_t *allocation;

    D_0054EF90 = fld;
    D_0054EF94 = waittime;
    fldTitleState = 0;
    D_0054EF8C = 0;
    sprintf(path, "/fld/f/pnl/df%03d.tmx", fld);
    allocation = (sdfMemHandle_t *)sdfDevLoadFileEx(path, &data, NULL);
    D_0054EF98 = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect(allocation);

    allocation = (sdfMemHandle_t *)sdfDevLoadFileEx("/fld/f/pnl/dun_01.tmx", &data, NULL);
    D_0054EF9C = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect(allocation);

    allocation = (sdfMemHandle_t *)sdfDevLoadFileEx("/fld/f/pnl/dun_02.tmx", &data, NULL);
    D_0054EFA0 = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect(allocation);

    allocation = (sdfMemHandle_t *)sdfDevLoadFileEx("/fld/f/pnl/dun_03.tmx", &data, NULL);
    D_0054EFA4 = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect(allocation);

    if (!fldTitleChk()) {
        dds3StartProcess(D_005327B0, 0x2B0A, 0, 1, fldTitle_Process,
                         (dds3ProcessFunc_t)fldTitle_Shutdown, NULL);
    }
}

s32 fldTitleMiniChk(void) {
    return dds3SearchProcessName(D_00532820) != 0;
}


s32 fldTitleMini_Process(dds3ProcessID_t *id)
{
    s32 phase;
    s32 maxAlpha;
    s32 alpha;
    s32 x;

    if (fldProcCampChk() != 1) {
        maxAlpha = 128;
        rsetOtOfs(0x47);
        rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
        rsetAlpha(0);

        phase = fldSnowActive;
        switch (phase) {
        case 0:
            break;
        case 1: {
            if (fldTitleMiniState >= 3) {
                s32 stripColor;
                alpha = (fldTitleMiniState * 128 - 384) / 7;
                x = fldTitleMiniState * 80 - 752;
                if (x > 0) x = 0;
                if (alpha > 128) alpha = maxAlpha;
                stripColor = 0x00808080 | (alpha << 24);

                DrwSprite2D(x, 340, 9, 8, 2, 1, 9, 8, stripColor, fldTitleMiniWork);
                DrwSprite2D(x + 9, 340, 503, 8, 10, 1, 1, 8, stripColor,
                            fldTitleMiniWork);

                x = 752 - fldTitleMiniState * 80;
                if (x < 0) x = 0;
                DrwSprite2D(x, 378, 9, 8, 2, 8, 9, -8, stripColor, fldTitleMiniWork);
                DrwSprite2D(x + 9, 378, 503, 8, 10, 8, 1, -8, stripColor,
                            fldTitleMiniWork);
            }

            if (fldTitleMiniState >= 5) {
                s32 shade;
                alpha = (fldTitleMiniState * 128 - 512) / 10;
                if (alpha > 128) alpha = maxAlpha;
                shade = 89;
                shade *= alpha;
                shade /= 128;
                aDrawBoxG(0, 174, 256, 15,
                          0, 0, 0, 0, 0, 0, 0, shade,
                          0, 0, 0, shade, 0, 0, 0, 0);
                aDrawBoxG(256, 174, 256, 15,
                          0, 0, 0, shade, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, shade);
            }

            if (fldTitleMiniState >= 9) {
                alpha = fldTitleMiniState * 16 - 128;
                if (alpha > 128) alpha = maxAlpha;
                DrwSprite2D(128, 313, 256, 128, 0, 0, 256, 128, 0x00808080 | (alpha << 24),
                            fldTitleMiniTex);
            }

            if (++fldTitleMiniState >= 21) {
                fldTitleMiniState = 0;
                fldSnowActive++;
            }
            break;
        }

        case 2: {
            s32 flatColor;
            flatColor = 0x80808080;
            DrwSprite2D(0, 340, 9, 8, 2, 1, 9, 8, flatColor, fldTitleMiniWork);
            DrwSprite2D(9, 340, 503, 8, 10, 1, 1, 8, flatColor, fldTitleMiniWork);
            DrwSprite2D(0, 378, 9, 8, 2, 8, 9, -8, flatColor, fldTitleMiniWork);
            DrwSprite2D(9, 378, 503, 8, 10, 8, 1, -8, flatColor, fldTitleMiniWork);
            aDrawBoxG(0, 174, 256, 15,
                      0, 0, 0, 0, 0, 0, 0, 89,
                      0, 0, 0, 89, 0, 0, 0, 0);
            aDrawBoxG(256, 174, 256, 15,
                      0, 0, 0, 89, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 89);
            DrwSprite2D(128, 313, 256, 128, 0, 0, 256, 128, flatColor,
                        fldTitleMiniTex);

            if (++fldTitleMiniState >= 31) {
                fldTitleMiniState = 0;
                fldSnowActive++;
            }
            break;
        }

        case 3: {
            s32 displacement;

            alpha = (7 - fldTitleMiniState) * 128 / 7;
            if (alpha < 0) alpha = 0;
            if (alpha > 128) alpha = maxAlpha;

            if (fldTitleMiniState >= 3) {
                x = fldTitleMiniState * 128 - 256;
            } else {
                x = 0;
            }
            if (x < 512) {
                s32 stripColor;
                stripColor = 0x00808080 | (alpha << 24);
                DrwSprite2D(x, 340, 9, 8, 2, 1, 9, 8, stripColor, fldTitleMiniWork);
                DrwSprite2D(x + 9, 340, 503, 8, 10, 1, 1, 8, stripColor,
                            fldTitleMiniWork);
            }

            if (fldTitleState >= 3) {
                displacement = fldTitleMiniState * 128 - 256;
                x = -displacement;
            } else {
                x = 0;
            }
            if (x >= -511) {
                s32 stripColor;
                stripColor = 0x00808080 | (alpha << 24);
                DrwSprite2D(x, 378, 9, 8, 2, 8, 9, -8, stripColor, fldTitleMiniWork);
                DrwSprite2D(x + 9, 378, 503, 8, 10, 8, 1, -8, stripColor,
                            fldTitleMiniWork);
            }

            alpha = (15 - fldTitleMiniState) * 128 / 15;
            if (alpha < 0) alpha = 0;
            if (alpha > 128) alpha = 128;
            if (alpha > 0) {
                s32 shade;
                shade = 89;
                shade *= alpha;
                shade /= 128;
                aDrawBoxG(0, 174, 256, 15,
                          0, 0, 0, 0, 0, 0, 0, shade,
                          0, 0, 0, shade, 0, 0, 0, 0);
                aDrawBoxG(256, 174, 256, 15,
                          0, 0, 0, shade, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, shade);
            }

            alpha = (25 - fldTitleMiniState) * 128 / 25;
            if (alpha < 0) alpha = 0;
            if (alpha > 128) alpha = 128;
            DrwSprite2D(128, 313, 256, 128, 0, 0, 256, 128, 0x00808080 | (alpha << 24),
                        fldTitleMiniTex);

            if (++fldTitleMiniState >= 31) {
                fldTitleMiniState = 0;
                fldSnowActive++;
            }
            break;
        }
        default:
            return -1;
        }
    }
    return 0;
}

void fldTitleMini_Shutdown(void) {
    if (fldTitleMiniTex != 0) {
        sdfTexFreeDelayed(fldTitleMiniTex);
        fldTitleMiniTex = 0;
    }
    if (fldTitleMiniWork != 0) {
        sdfTexFreeDelayed(fldTitleMiniWork);
        fldTitleMiniWork = 0;
    }
}

extern s32 fldArea[];
extern s32 D_0054EFB0;
extern s32 evtBitCheck(s32 no);

INCLUDE_RODATA(const s32, "field/fldTitle", D_00532820);

void fldTitleMiniStart(s32 fld)
{
    char path[32];
    u32 data;
    void *allocation;

    if (fld < 0xA) {
        return;
    }
    if (fld == 0xB && evtBitCheck(0x43F) == 0) return;
    if (fld == 0xD && evtBitCheck(0x4C3) == 0) return;
    if (fld == 0xE && evtBitCheck(0x420) == 0) return;
    if (fld == 0xF && evtBitCheck(0x440) == 0) return;
    if (fld == 0x10 && evtBitCheck(0x460) == 0) return;
    if (fld == 0x11 && evtBitCheck(0x480) == 0) return;
    if (fld == 0x12 && evtBitCheck(0x4A0) == 0) return;
    if (fld == 0x13 && evtBitCheck(0x4C0) == 0) return;
    if (fld == 0x14 && evtBitCheck(0x4E0) == 0) return;
    if (fld == 0x15 && evtBitCheck(0x500) == 0) return;
    if (fld == 0x16 && evtBitCheck(0x520) == 0) return;
    if (fld == 0x17 && evtBitCheck(0x540) == 0) return;
    if (fld == 0x18 && evtBitCheck(0x560) == 0) return;
    if (fld == 0x19 && evtBitCheck(0x580) == 0) return;
    if (fld == 0x1A && evtBitCheck(0x5A0) == 0) return;
    if (fld == 0x1B && evtBitCheck(0x5C0) == 0) return;
    if (fld == 0x1C && evtBitCheck(0x5E0) == 0) return;
    if (fld == 0x1D && evtBitCheck(0x600) == 0) return;
    if (fld == 0x1E && evtBitCheck(0x620) == 0) return;
    if (fld == 0x1F && evtBitCheck(0x640) == 0) return;
    if (fld == 0x20 && evtBitCheck(0x660) == 0) return;
    if (fld == 0x21 && evtBitCheck(0x680) == 0) return;
    if (fld == 0x22 && evtBitCheck(0x6A0) == 0) return;
    if (fld == 0x23 && evtBitCheck(0x6E0) == 0) return;
    if (fld == 0x26 && evtBitCheck(0x710) == 0) return;
    if (fld == 0x28 && evtBitCheck(0x750) == 0) return;
    if (fld == 0x29 && evtBitCheck(0x760) == 0) return;
    if (fld == 0x2A && evtBitCheck(0x780) == 0) return;
    if (fld == 0x2B && evtBitCheck(0x7A0) == 0) return;
    if (fld == 0x2C && evtBitCheck(0x7C0) == 0) return;
    if (fld == 0x2D && evtBitCheck(0x7E0) == 0) return;

    if (fld == 0xC && fldArea[0] == 1) return;
    if (fld == 0xF) {
        if (fldArea[0] == 0x14) return;
        if (fldArea[0] == 0x13) return;
    }
    if (fld == 0x22 && fldArea[0] == 2) return;
    if (fld == 0x24) {
        fld = 0x20;
    }
    if (fld == 0x25) {
        fld = 0x20;
    }

    D_0054EFB0 = fld;
    fldTitleMiniState = 0;
    fldSnowActive = 0;
    sprintf(path, "/fld/f/pnl/ds%03d.tmx", fld);
    allocation = sdfDevLoadFileEx(path, &data, NULL);
    fldTitleMiniTex = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect((sdfMemHandle_t *)allocation);

    allocation = sdfDevLoadFileEx("/fld/f/pnl/dun_04.tmx", &data, NULL);
    fldTitleMiniWork = sdfTexSet((const TMX_HEADER_t *)data);
    sdfMemFreeDirect((sdfMemHandle_t *)allocation);

    if (fldTitleChk() == 0) {
        dds3StartProcess(D_00532820, 0x2B0A, 0, 1, fldTitleMini_Process,
                         (dds3ProcessFunc_t)fldTitleMini_Shutdown, NULL);
    }
}

