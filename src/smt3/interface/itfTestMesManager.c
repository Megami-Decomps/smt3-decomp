#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"


typedef struct { u8 unk00[8]; f32 degrees; u8 unk0C[4]; } pbFovWork_t;

/* The native FOV workspace is an unnamed view at pinst + 0xA34. Bind its
 * assembler address without redefining pinst's message-instance array type
 * or inventing an overlapping allocation in the generated BSS split. */
PS2_SYMBOL_AT(pbFovWork_t, pbDebugFovWork, pinst, 0xA34);

typedef struct { s32 x, y, z; u32 style; } pbPrintState_t;

typedef struct
{
    s16 editing, selection;
    u32 blink;
    u8 unk08[8];
} pbPanelEditor_t;

typedef struct { const char *title; s32 highlight; } pbColorRow_t;

typedef struct { u8 value; u8 unk01[3]; } pbColorByte_t;

typedef union
{
    struct { s32 red, green, blue, alpha; };
    s32 channels[4];
    pbColorByte_t channelBytes[4];
} pbColor_t;

extern pbPanelEditor_t pbDebugPanel2Pos, pbDebugPanel3X;

extern pbColorRow_t D_00326C20[6];

extern pbColor_t D_00326C50;

extern sdf3DScreenParam_t dds3ScreenParam;

extern const char *D_00326C08[6];

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;

extern s32 dds3ScreenBlurOfsX[2]; /* Native adjacent X/Y offset pair. */

extern s32 D_00551EA8, pbDebugPanel3State;

extern u8 D_0054F584;

extern void *dds3FontReqAllocDrawTag(void);

extern void *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void sdfDrawTagLink(sdfDrawTag_t *, u32);

extern void sdfDevSetPrintCB(pbPrintState_t *, s32, s32, s32, s32);

extern void *sdfDevPrintfEx(void *, const char *, ...);

#include "sdk_libc.h"

extern f32 sdfSin(f32 aRadius);

s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);

void pbDrawRotatedPrim(s32 otIndex);

extern char D_0054F530[];

extern char D_0054F538[];

extern char D_0054F540[];

extern char D_0054F548[];

extern char D_0054F550[];

extern char D_0054F558[];

extern char D_0054F588[];

extern char D_0054F590[];

extern char D_0054F598[];

extern char D_0054F5A0[];

extern char D_0054F5A8[];

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;



extern char *D_00326BC8[];

extern s32 D_00326BD8[4];

extern s32 D_00326BE8[4];

extern s32 D_00326BF8[4];

extern u8 scrEmbedded_3267A8[];

extern sdfGrOt_t sdfGrOtTable[];

extern u8 dds3PadButtonState[];

extern u32 dds3BackColor;

extern s32 D_0054F510;

extern void *D_0054F514;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);


extern void itfMesMngSetKutiCallback(s32 id, void *func);

extern void itfDebugPrintMark(void);

extern void sdfTexFreeDelayed(void *aHandle);

extern void *itfMiscLoadTexture(char *filename);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void *sdf2DMakeStdPacket(void *aPacket, u32 aLeng);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

extern void itfMiscDrawSpritePrim(s32 *pos, s32 *uv, s32 *color, s32 z, void *texhandle, s32 context,
                                  sdfDrawTag_t *pdrawtag);
extern sdfVector4_t aViewEye, aViewTarget;


void TestMsgReloadTexture(void);

s32 TestMsgMngC_Process(dds3ProcessID_t *id);

s32 TestMsgMngD_Process(dds3ProcessID_t *id);

void itfDebugMesMngInitialize(void)
{
    dds3BackColor = 0x80FFFFFF;
    TestMsgReloadTexture();
    itfMesMngSetKutiCallback(
        ((scrProcessWork_s *)dds3GetProcessData(scrStartScript2(0x3E8, scrEmbedded_3267A8, 0)))
            ->MesHandle,
        itfDebugPrintMark);
    dds3StartProcess("TestMsgMngC", 0x3EF, 0, 0, TestMsgMngC_Process, NULL, NULL);
    dds3StartProcess("TestMsgMngD", 0x2AFE, 0, 0, TestMsgMngD_Process, NULL, NULL);
}

void TestMsgReloadTexture(void)
{
    if (D_0054F514 != NULL) {
        sdfTexFreeDelayed(D_0054F514);
        D_0054F514 = NULL;
    }
    D_0054F510 = (D_0054F510 + 1) & 3;
    if (D_0054F510 != 3) {
        D_0054F514 = itfMiscLoadTexture(D_00326BC8[D_0054F510]);
    }
}

s32 TestMsgMngC_Process(dds3ProcessID_t *id)
{
    if ((s8)dds3PadButtonState[0x20] < 0) {
        TestMsgReloadTexture();
    }
    return 0;
}

s32 TestMsgMngD_Process(dds3ProcessID_t *id)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    if ((D_0054F514 != NULL) && (D_0054F510 != 3)) {
        tag = sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        itfMiscDrawAlphaPrim(tag, 0, 0);
        itfMiscDrawSpritePrim(D_00326BD8, D_00326BE8, D_00326BF8, 0xFFF, D_0054F514, 0, tag);
        ot = &sdfGrOtTable[71];
        ((sdfGrOtRegFunc_t)ot->regFunc)(ot, tag);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_00535BD0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_00535BE8);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_00535C00);

void itfDebugPrintMark(void) {
    printf("********* AAAA ********\n");
}

s32 pbDebugPrintPanel(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    f32 radiansToDegrees = 57.2957795f;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
                    (u32)dds3PrimFrameRect(0x8500, 0x79C0, 0xFEFFFF,
                                  0xA80, 0x120, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfDrawTagLink(list,
                    (u32)sdfDevPrintfEx(&packet, "FOVY: %6.2f",
                        dds3ScreenParam.fovy * radiansToDegrees));
    { sdfGrOt_t *ot = &sdfGrOtTable[81]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }
    if (((s8)dds3PadButtonState[0x33]) < 0) {
        return -1;
    }
    pbDebugFovWork.degrees = dds3ScreenParam.fovy * radiansToDegrees;
    if (pbCalcFovDegrees(&pbDebugFovWork.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        dds3ScreenParam.fovy = pbDebugFovWork.degrees * 0.0174532925f;
    }
    return 0;
}

s32 pbCalcFovDegrees(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep)
{
    f32 current = *value;
    s32 changed = 0;

    if ((s8)dds3PadButtonState[0x35] < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x35] & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if ((s8)dds3PadButtonState[0x34] < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x34] & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if ((s8)dds3PadButtonState[0x3A] < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x3A] & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if ((s8)dds3PadButtonState[0x38] < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x38] & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *value = current;
        return 1;
    }
    return 0;
}

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 pbDebugPrintPanel2(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet,
        "BLUR:        %s", dds3ScreenBlurFlag ? D_0054F530 : D_0054F538));

    y = 0x7A20;
    if (pbDebugPanel2Pos.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x82C0, pbDebugPanel2Pos.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel2Pos.blink++;
        if (pbDebugPanel2Pos.blink < 32 || (pbDebugPanel2Pos.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F540));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfDevSetPrintCB(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_0054F548, D_00326C08[row]));
        highlight = 0;
        if (pbDebugPanel2Pos.editing != 0 && row == pbDebugPanel2Pos.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F550, dds3ScreenBlurAlpha));
            break;
        case 1:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F550, dds3ScreenBlurScale));
            break;
        case 2:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F558, dds3ScreenBlurOfsX[0], dds3ScreenBlurOfsX[1]));
            break;
        }
        y += 0x60;
    }
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect((dds3ScreenBlurOfsX[0] * 16) + 0x7FC0, (dds3ScreenBlurOfsX[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    { sdfGrOt_t *ot = &sdfGrOtTable[83]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }

    if ((s8)((s8)dds3PadButtonState[0x32]) < 0) {
        dds3ScreenBlurFlag ^= 1;
    }
    if (pbDebugPanel2Pos.editing == 0) {
        previous = pbDebugPanel2Pos.selection;
        if ((u8)((s8)dds3PadButtonState[0x36]) & 2) {
            pbDebugPanel2Pos.selection--;
            if (pbDebugPanel2Pos.selection < 0) {
                pbDebugPanel2Pos.selection = 2;
            }
        }
        if ((u8)((s8)dds3PadButtonState[0x37]) & 2) {
            pbDebugPanel2Pos.selection++;
            if (pbDebugPanel2Pos.selection >= 3) {
                pbDebugPanel2Pos.selection = 0;
            }
        }
        if (previous != pbDebugPanel2Pos.selection) {
            pbDebugPanel2Pos.blink = 0;
        }
        if (((s8)dds3PadButtonState[0x31]) < 0) {
            pbDebugPanel2Pos.editing ^= 1;
            pbDebugPanel2Pos.blink = 0;
        }
        if ((u8)((s8)dds3PadButtonState[0x33]) & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel2Pos.selection == 2) {
            if (((s8)dds3PadButtonState[0x36]) != 0) {
                dds3ScreenBlurOfsX[1] -= 16;
                if (dds3ScreenBlurOfsX[1] < -224) {
                    dds3ScreenBlurOfsX[1] = -224;
                }
            }
            if (((s8)dds3PadButtonState[0x37]) != 0) {
                dds3ScreenBlurOfsX[1] += 16;
                if (dds3ScreenBlurOfsX[1] > 224) {
                    dds3ScreenBlurOfsX[1] = 224;
                }
            }
            if (((s8)dds3PadButtonState[0x34]) != 0) {
                dds3ScreenBlurOfsX[0] -= 16;
                if (dds3ScreenBlurOfsX[0] < -256) {
                    dds3ScreenBlurOfsX[0] = -256;
                }
            }
            if (((s8)dds3PadButtonState[0x35]) != 0) {
                dds3ScreenBlurOfsX[0] += 16;
                if (dds3ScreenBlurOfsX[0] > 256) {
                    dds3ScreenBlurOfsX[0] = 256;
                }
            }
        } else if (pbDebugPanel2Pos.selection == 0) {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                dds3ScreenBlurAlpha -= 10;
                if (dds3ScreenBlurAlpha < 0) {
                    dds3ScreenBlurAlpha = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                dds3ScreenBlurAlpha += 10;
                if (dds3ScreenBlurAlpha > 255) {
                    dds3ScreenBlurAlpha = 255;
                }
            }
        } else {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                dds3ScreenBlurScale -= 10;
                if (dds3ScreenBlurScale < -255) {
                    dds3ScreenBlurScale = -255;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                dds3ScreenBlurScale += 10;
                if (dds3ScreenBlurScale > 255) {
                    dds3ScreenBlurScale = 255;
                }
            }
        }
        if (((s8)dds3PadButtonState[0x33]) < 0) {
            pbDebugPanel2Pos.editing ^= 1;
            pbDebugPanel2Pos.blink = 0;
        }
    }
    return 0;
}

/* Draw the four color channels and handle selection and value editing. */
s32 pbDebugPrintPanel3(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F588));
    if (pbDebugPanel3X.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x8980, pbDebugPanel3X.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel3X.blink++;
        if (pbDebugPanel3X.blink < 32 || (pbDebugPanel3X.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F590));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfDevSetPrintCB(&packet, 0x8980, y, 0xFF0000, D_00326C20[row].highlight);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_0054F598, D_00326C20[row].title));
        highlight = 0;
        if (pbDebugPanel3X.editing != 0 && row == pbDebugPanel3X.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F5A0, D_00326C50.channels[row]));
        } else {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F5A8, D_00551EA8));
        }
        y += 0x60;
    }
    { sdfGrOt_t *ot = &sdfGrOtTable[83]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }
    pbDebugPanel3State += D_00551EA8;
    pbDrawRotatedPrim(0x46);
    if ((s8)((s8)dds3PadButtonState[0x32]) < 0) {
        D_0054F584 ^= 1;
    }
    if (pbDebugPanel3X.editing == 0) {
        previousSelection = pbDebugPanel3X.selection;
        if ((u8)((s8)dds3PadButtonState[0x36]) & 2) {
            pbDebugPanel3X.selection--;
            if (pbDebugPanel3X.selection < 0) {
                pbDebugPanel3X.selection = 3;
            }
        }
        if ((u8)((s8)dds3PadButtonState[0x37]) & 2) {
            pbDebugPanel3X.selection++;
            if (pbDebugPanel3X.selection >= 4) {
                pbDebugPanel3X.selection = 0;
            }
        }
        if (previousSelection != pbDebugPanel3X.selection) {
            pbDebugPanel3X.blink = 0;
        }
        if (((s8)dds3PadButtonState[0x31]) < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
        if ((u8)((s8)dds3PadButtonState[0x33]) & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel3X.selection < 4) {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                D_00326C50.channels[pbDebugPanel3X.selection] -= 10;
                if (D_00326C50.channels[pbDebugPanel3X.selection] < 0) {
                    D_00326C50.channels[pbDebugPanel3X.selection] = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                D_00326C50.channels[pbDebugPanel3X.selection] += 10;
                if (D_00326C50.channels[pbDebugPanel3X.selection] > 255) {
                    D_00326C50.channels[pbDebugPanel3X.selection] = 255;
                }
            }
        } else {
            previousRate = D_00551EA8;
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                D_00551EA8 -= 0x100;
                if (D_00551EA8 < 0) {
                    D_00551EA8 = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                D_00551EA8 += 0x100;
                if (D_00551EA8 > 0x800) {
                    D_00551EA8 = 0x800;
                }
            }
            if (previousRate != D_00551EA8) {
                pbDebugPanel3State = 0;
            }
        }
        if (((s8)dds3PadButtonState[0x33]) < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
    }
    return 0;
}

u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

void *pbMakeSpritePacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x48 : 0x47;
    return packet;
}

void *pbMakePrimPacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x43 : 0x42;
    return packet;
}

void pbDrawRotatedPrim(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    pbColorByte_t *component;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    f32 phase;

    phase = (f32)(pbDebugPanel3State % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSin(phase) + 1.0f) * 0.5f * D_00326C50.alpha);
    component = D_00326C50.channelBytes;
    for (i = 0; i != 3; i++, component++) {
        color |= component->value << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list, (u32)pbMakeSpritePacket(0x33001, 0));
    sdfDrawTagLink(list, (u32)pbMakePrimPacket(6, 0));
    sdfDrawTagLink(list, (u32)pbAllocPrimPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &sdfGrOtTable[surfaceIndex];
    ((sdfGrOtRegFunc_t)surface->regFunc)(surface, list);
}

void func_001D2638(f32 *delta) {
    f32 *eye = (f32 *)&aViewEye;
    u32 i;
    for (i = 0; i < 3; i++) {
        eye[i] += delta[i];
    }
}

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_00535C58);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_00535C68);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F510);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F514);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F518);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F520);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F528);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F530);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F538);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F540);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F548);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F550);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F558);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F560);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F568);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F570);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F578);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F580);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F584);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F588);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F590);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F598);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5A0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5A8);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5B0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5B8);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5C0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5C4);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5D0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5D2);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5D4);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbMoveCharList);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", blockDB);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", bmap);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", goal);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", record);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", mapCursor);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5F0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", degree_mode);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbGameKeyState);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", pbMapCursorWork);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F5FC);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F600);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F608);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", g_pdebug_0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F614);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F618);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_0054F620);

