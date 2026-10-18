#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"

#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

/* EE GCC promotes %f arguments through SMT3's software conversion helper. */
__asm__("fptodp = sdfFloatToULong");

typedef struct { s32 x, y, z; u32 style; } pbPrintState_t;
typedef struct { u8 unk00[8]; f32 degrees; u8 unk0C[4]; } pbFovWork_t;
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
typedef struct { s32 value; } pbPanelValue_t;
typedef struct pbGroupDB_t pbGroupDB_t;
/* Common panel work header: its constructor installs process and data links. */
typedef struct
{
    dds3ProcessID_t *process;
    u32 flags;
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; } data;
} pbPanel_t;
typedef struct { sdfMemHandle_t *allocation; } pbDebugWork_t;

/* The native FOV workspace is an unnamed view at pinst + 0xA34. Bind its
 * assembler address without redefining pinst's message-instance array type
 * or inventing an overlapping allocation in the generated BSS split. */
extern pbFovWork_t pbDebugFovWork;
__asm__("pbDebugFovWork = pinst + 0xA34");
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
extern s32 pbMoveChar;
extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;
extern u8 keyActMode;
extern dds3ProcessID_t *scr_msg;
extern pbDebugWork_t *g_pdebug_0;
extern void *dds3FontReqAllocDrawTag(void);
extern void *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
extern void sdfDrawTagLink(sdfDrawTag_t *, u32);
extern void sdfDevSetPrintCB(pbPrintState_t *, s32, s32, s32, s32);
extern void *sdfDevPrintfEx(void *, const char *, ...);
extern s32 printf(const char *, ...);
extern f32 sdfSin(f32 angle);
extern void mdlSetPosition(mdlObj_t *model);
extern void mdlSetRotate(mdlObj_t *model);
extern void FromEuler(void);
extern void sdfChipFreeDelayedDirect(void *ptr);
extern void sdfChipFreeDirect(void *ptr);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern s32 pbMapIsValid(void);
extern void pbPreviousRecord(void);
extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);
extern pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page);
void ReleaseDebug_pbDebugMode(void);
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

extern char D_00535BB0[];
extern char D_00535BC0[];
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
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern dds3ProcessID_t *scrStartScript2(s32 handle, u8 *script, s32 arg);
extern void itfMesMngSetKutiCallback(s32 meshandle, void *func);
extern void itfDebugPrintMark(void);
extern void sdfTexFreeDelayed(void *tex);
extern void *itfMiscLoadTexture(char *name);
extern void *sdfGrAllocPacketBuff(u32 size);
extern void *sdf2DMakeStdPacket(void *buff, u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *tag, s32 mode, s32 value);
extern void itfMiscDrawSpritePrim(s32 *rect, s32 *uv, s32 *color, s32 z, void *tex, s32 flag,
                                  sdfDrawTag_t *tag);

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
    dds3StartProcess(D_00535BB0, 0x3EF, 0, 0, TestMsgMngC_Process, NULL, NULL);
    dds3StartProcess(D_00535BC0, 0x2AFE, 0, 0, TestMsgMngD_Process, NULL, NULL);
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

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C00);

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

INCLUDE_ASM(const s32, "code_001D1470", func_001D2638);

INCLUDE_ASM(const s32, "code_001D1470", pbRotateXCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbRotateYCamera);

INCLUDE_ASM(const s32, "code_001D1470", pb_1D2838);

INCLUDE_ASM(const s32, "code_001D1470", pbSetViewRotation);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mycharacter_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mycharacter_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pb_1D2AD8);

void pbSetModelPos(mdlObj_t *model, const f32 *position) {
    SDF_VU0__LOADVECTOR(position);
    mdlSetPosition(model);
}

INCLUDE_ASM(const s32, "code_001D1470", pb_1D2B18);

void pbSetModelRotate(mdlObj_t *model) {
    FromEuler();
    mdlSetRotate(model);
}

void func_001D2B68(void) {
}

void func_001D2B70(void) {
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C58);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMyCharacterDraw);

INCLUDE_ASM(const s32, "code_001D1470", pbGetPanelField);

INCLUDE_ASM(const s32, "code_001D1470", pbMyCharacterDispStat);

INCLUDE_ASM(const s32, "code_001D1470", pbSetPanelValue);

void func_001D2D00(void) {
}

void pbChangeMotion(void) {
}

s32 pb_1D2D10(void) {
    return 0;
}

INCLUDE_ASM(const s32, "code_001D1470", pbMoveMyCharcter);

INCLUDE_ASM(const s32, "code_001D1470", pbMoveMyCharcter2);

INCLUDE_ASM(const s32, "code_001D1470", pbGetCellY);

INCLUDE_ASM(const s32, "code_001D1470", pbGetMyCharcterPos);

INCLUDE_ASM(const s32, "code_001D1470", pbGetMyCharcterPos2);

INCLUDE_ASM(const s32, "code_001D1470", pbGetMyCharcterRot);

INCLUDE_ASM(const s32, "code_001D1470", pbSetPanelModel);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_exit_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_exit_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mapbase_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mapbase_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMapBaseMat);

INCLUDE_ASM(const s32, "code_001D1470", pbBuildMapBaseVerts);

INCLUDE_ASM(const s32, "code_001D1470", pbSetPrimMatrix);

INCLUDE_ASM(const s32, "code_001D1470", pbFindBlockSlot);

INCLUDE_ASM(const s32, "code_001D1470", func_001D35F0);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapPoly);

INCLUDE_ASM(const s32, "code_001D1470", pb_1D3940);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C88);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapPolyTransformed);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapBlock);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CD0);

INCLUDE_ASM(const s32, "code_001D1470", pbInitExitDraw);

INCLUDE_ASM(const s32, "code_001D1470", pbSetExitDisp);

s32 pbGetExitDisp(pbPanel_t *panel) {
    return panel->data.value->value;
}

void pbSetExitPos(pbPanel_t *panel, s32 x, s32 y) {
    panel->x = x;
    panel->y = y;
}

INCLUDE_ASM(const s32, "code_001D1470", pbInitMapBaseDraw);

void func_001D4140(pbPanel_t *panel, s32 value) {
    panel->data.value->value = value;
}

INCLUDE_ASM(const s32, "code_001D1470", pbLoadTextures);

INCLUDE_ASM(const s32, "code_001D1470", pbEditInitProc);

INCLUDE_ASM(const s32, "code_001D1470", pbEditMainExitProc);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_dummy_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_dummy_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_bg_Process);

void pb_draw_bg_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDelayedDirect(id->workdata);
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535DE0);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMapEditor);

s32 pbGetMoveChar(void) {
    return pbMoveChar;
}

INCLUDE_ASM(const s32, "code_001D1470", pbStartDummy);

INCLUDE_ASM(const s32, "code_001D1470", pbStartBackGround);

s32 func_001D4B28(void) {
    return 0;
}

INCLUDE_ASM(const s32, "code_001D1470", pbMapIsValid);

s32 pb_1D4B60(void) {
    return 0;
}

void func_001D4B68(void) {
}

s32 pb_1D4B70(void) {
    return 0;
}

void func_001D4B78(void) {
}

s32 pb_1D4B80(void) {
    return 0;
}

s32 pb_1D4B88(void) {
    return 0;
}

void func_001D4B90(void) {
}

INCLUDE_ASM(const s32, "code_001D1470", pbLoadSystemDataDone);

s32 func_001D4BC0(void) {
    return 1;
}

s32 func_001D4BC8(void) {
    return pbMapIsValid() == 0;
}

INCLUDE_ASM(const s32, "code_001D1470", func_001D4BE8);

INCLUDE_ASM(const s32, "code_001D1470", func_001D4C08);

INCLUDE_ASM(const s32, "code_001D1470", pbMakeMapPathHdd);

INCLUDE_ASM(const s32, "code_001D1470", pbMakeMapPath);

void func_001D4E08(void) {
}

void func_001D4E10(void) {
}

INCLUDE_ASM(const s32, "code_001D1470", exitPB);

INCLUDE_ASM(const s32, "code_001D1470", func_001D4E70);

INCLUDE_ASM(const s32, "code_001D1470", func_001D4EB0);

void func_001D4F08(void) {
    pbPreviousRecord();
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E68);

INCLUDE_ASM(const s32, "code_001D1470", pbSetDebugOwner);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_menu_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_menu_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pb_load_mapdata_Process);

void pb_load_mapdata_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDirect(id->workdata);
}

INCLUDE_ASM(const s32, "code_001D1470", pbStartMenu);

INCLUDE_ASM(const s32, "code_001D1470", pbWaitLoadMapData);

INCLUDE_ASM(const s32, "code_001D1470", pbLoadSystemData);

INCLUDE_ASM(const s32, "code_001D1470", getCharacterNum);

INCLUDE_ASM(const s32, "code_001D1470", pbNextCharacter);

INCLUDE_ASM(const s32, "code_001D1470", updateMotion);

INCLUDE_ASM(const s32, "code_001D1470", hitCheck);

INCLUDE_ASM(const s32, "code_001D1470", pushCheck);

INCLUDE_ASM(const s32, "code_001D1470", roundCheck);

INCLUDE_ASM(const s32, "code_001D1470", roundDirCheck);

INCLUDE_ASM(const s32, "code_001D1470", hallCheck);

INCLUDE_ASM(const s32, "code_001D1470", pbFindProcHandle);

INCLUDE_ASM(const s32, "code_001D1470", goalHitCheck);

INCLUDE_ASM(const s32, "code_001D1470", gameKey0);

INCLUDE_ASM(const s32, "code_001D1470", gameKey1);

INCLUDE_ASM(const s32, "code_001D1470", RotateBit);

INCLUDE_ASM(const s32, "code_001D1470", gameKey2);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535EF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F08);

INCLUDE_ASM(const s32, "code_001D1470", game);

INCLUDE_ASM(const s32, "code_001D1470", deleteGame);

INCLUDE_ASM(const s32, "code_001D1470", pbInitGame);

INCLUDE_ASM(const s32, "code_001D1470", moveChar);

INCLUDE_ASM(const s32, "code_001D1470", deleteMoveChar);

INCLUDE_ASM(const s32, "code_001D1470", setMoveBlockData);

INCLUDE_ASM(const s32, "code_001D1470", moveBlock);

INCLUDE_ASM(const s32, "code_001D1470", deleteMoveBlock);

INCLUDE_ASM(const s32, "code_001D1470", rotXY);

INCLUDE_ASM(const s32, "code_001D1470", setRoundBlockData);

INCLUDE_ASM(const s32, "code_001D1470", roundBlock);

INCLUDE_ASM(const s32, "code_001D1470", deleteRoundBlock);

INCLUDE_ASM(const s32, "code_001D1470", roundAutoCamera);

INCLUDE_ASM(const s32, "code_001D1470", deleteRoundAutoCamera);

INCLUDE_ASM(const s32, "code_001D1470", viewModeCamera);

INCLUDE_ASM(const s32, "code_001D1470", deleteViewModeCamera);

INCLUDE_ASM(const s32, "code_001D1470", ClearProc);

INCLUDE_ASM(const s32, "code_001D1470", DeleteClearProc);

INCLUDE_ASM(const s32, "code_001D1470", CalcDegreeMode);

INCLUDE_ASM(const s32, "code_001D1470", GetStageNumber);

INCLUDE_ASM(const s32, "code_001D1470", GetStageScriptLabel);

INCLUDE_ASM(const s32, "code_001D1470", GetAllStageClearBit);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F48);

INCLUDE_ASM(const s32, "code_001D1470", pbStartGame);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMoveChar);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMoveBlock);

INCLUDE_ASM(const s32, "code_001D1470", pbStartRoundBlock);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F78);

INCLUDE_ASM(const s32, "code_001D1470", pbStartAutoRoundCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbStartViewModeCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbRoundCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera0);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera1);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera2);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera3);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera4);

INCLUDE_ASM(const s32, "code_001D1470", pbInitGameCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbCheckPreviousRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbPreviousRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbStartClear);

INCLUDE_ASM(const s32, "code_001D1470", pbStartGiveupConfirm);

INCLUDE_ASM(const s32, "code_001D1470", pbGetCameraParam);

void func_001D8EA0(f32 angle) {
    cameraAngle = angle;
}

void pbSetCameraRadius(f32 radius) {
    cameraRadius = radius;
}

void pbSetCameraDegree(f32 degree) {
    cameraDegree = degree;
}

void pbSetCameraDegreeInit(f32 degree) {
    cameraDegreeInit = degree;
}

dds3ProcessID_t *pbGetMessageScriptProc(void) {
    return scr_msg;
}

INCLUDE_ASM(const s32, "code_001D1470", pbGotoNewStage);

INCLUDE_ASM(const s32, "code_001D1470", pbRoundStageNext);

INCLUDE_ASM(const s32, "code_001D1470", pbRoundStagePrev);

INCLUDE_ASM(const s32, "code_001D1470", pbFindGroupEntry);

INCLUDE_ASM(const s32, "code_001D1470", pbAddGroupEntry);

void pbFreeGroupEntry(mdlResrc_t *entry) {
    sdfMemFreeDirect(entry->resourceHandle);
    sdfChipFreeDelayedDirect(entry);
}

INCLUDE_ASM(const s32, "code_001D1470", pbFreeGroupHandles);

INCLUDE_ASM(const s32, "code_001D1470", pbGetBlockAt);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_block_DB_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_block_DB_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_map_cursor_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_map_cursor_Shutdown);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536000);

INCLUDE_ASM(const s32, "code_001D1470", pbStartBlockDataBase);

void *pbSearchGroupDB(pbPanel_t *panel, u32 group) {
    return pbFindGroupEntry(panel->data.groupDB, group);
}

INCLUDE_ASM(const s32, "code_001D1470", pbDeleteGroupEntry);

INCLUDE_ASM(const s32, "code_001D1470", pbDumpBlockData);

INCLUDE_ASM(const s32, "code_001D1470", pbRebuildOutline);

INCLUDE_ASM(const s32, "code_001D1470", pbStartDrawMapCursor);

INCLUDE_ASM(const s32, "code_001D1470", pbSetMapCursorFlag);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_record_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_record_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pbFreeRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbStartRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbClearRecord);

s32 pb_1DA400(pbPanel_t *panel) {
    return panel->data.value->value;
}

INCLUDE_ASM(const s32, "code_001D1470", pbPushRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbCheckPopRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbPopRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_blk);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_flor);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_char);

INCLUDE_ASM(const s32, "code_001D1470", pbDumpRecordData);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_help_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_help_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pbStartHelp);

pbPanel_t *func_001DB020(pbPanel_t *parent) {
    return pbStartHelp(parent, 0);
}

INCLUDE_ASM(const s32, "code_001D1470", pbResetKeyState);

INCLUDE_ASM(const s32, "code_001D1470", pbGetKeyAction);

INCLUDE_ASM(const s32, "code_001D1470", pbGetKeyMode);

void func_001DB0D8(u8 mode) {
    keyActMode = mode;
}

INCLUDE_ASM(const s32, "code_001D1470", pbRotRKeyAct);

INCLUDE_ASM(const s32, "code_001D1470", pbRotLKeyAct);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetFormatStatusMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetTypeStatusMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetFreeStatusMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetStatusMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetDirMCard);

INCLUDE_ASM(const s32, "code_001D1470", pbGetDirMCard2);

INCLUDE_ASM(const s32, "code_001D1470", pbChangeDirectory);

INCLUDE_ASM(const s32, "code_001D1470", pbMkdirMCard);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536498);

INCLUDE_RODATA(const s32, "code_001D1470", D_005364C0);

INCLUDE_ASM(const s32, "code_001D1470", pbReadMCard);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536538);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536560);

INCLUDE_ASM(const s32, "code_001D1470", pbWriteMCard);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_error_Process);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_error_Shutdown);

INCLUDE_ASM(const s32, "code_001D1470", pbStartErrorDraw);

INCLUDE_ASM(const s32, "code_001D1470", pbDebugProc);

void pbDebugShutdown(void) {
    ReleaseDebug_pbDebugMode();
}

INCLUDE_ASM(const s32, "code_001D1470", InitializeDebug_pbDebugMode);

void ReleaseDebug_pbDebugMode(void) {
    if (g_pdebug_0 != NULL) {
        sdfMemFreeDirect(g_pdebug_0->allocation);
        g_pdebug_0 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001D1470", DebugProcMenu_pbDebugMode);

INCLUDE_RODATA(const s32, "code_001D1470", D_005365E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005365F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536608);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536620);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcRadius);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcDegree);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcStage);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcFree);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536690);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536708);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536720);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536738);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536750);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536768);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536780);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536798);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536810);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536828);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536840);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536858);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536870);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536888);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536900);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536918);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536930);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536948);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536960);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536978);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536990);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536ED0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537000);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537018);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537030);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537048);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537060);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537078);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537090);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537108);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537120);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537138);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537150);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537168);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537180);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537198);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537210);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537228);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537240);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537258);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537270);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537288);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537300);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537318);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537330);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537348);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537360);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537378);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537390);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537408);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537420);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537438);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537450);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537468);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537480);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537498);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537510);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537528);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537540);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537558);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537570);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537588);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537600);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537618);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537630);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537648);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537660);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537678);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537690);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537708);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537720);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537738);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537750);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537768);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537780);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537798);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537810);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537828);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537840);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537858);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537870);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537888);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537900);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537918);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537930);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537948);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537960);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537978);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537990);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537ED0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538008);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538020);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538038);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538050);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538068);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538080);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538098);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538110);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538128);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538140);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538158);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538170);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538188);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538200);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538218);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538230);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538248);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538260);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538278);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538290);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538308);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538320);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538338);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538350);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538368);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538380);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538398);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538410);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538428);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538440);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538458);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538470);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538488);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538500);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538518);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538530);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538548);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538560);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538578);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538590);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538608);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538620);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538638);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538650);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538668);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538680);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538698);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538710);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538728);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538740);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538758);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538770);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538788);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538800);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538818);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538830);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538848);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538860);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538878);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538890);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538908);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538920);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538938);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538950);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538968);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538980);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538998);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538ED8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539010);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539028);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539040);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539058);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539070);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539088);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539100);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539118);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539130);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539148);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539160);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539178);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539190);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539208);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539220);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539238);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539250);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539268);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539280);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539298);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539310);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539328);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539340);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539358);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539370);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539388);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539400);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539418);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539430);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539448);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539460);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539478);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539490);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539508);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539520);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539538);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539550);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539560);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539570);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539580);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539590);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539600);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539610);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539620);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539630);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539648);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539660);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539678);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539688);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539698);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539700);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539718);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539730);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539748);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539760);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539778);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539790);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539808);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539820);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539838);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539850);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539868);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539880);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539898);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539910);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539928);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539940);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539958);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539970);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539988);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A000);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A018);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A030);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A048);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A060);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A078);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A090);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A108);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A120);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A138);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A150);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A168);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A180);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A198);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A210);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A228);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A240);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A258);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A270);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A288);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A298);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A308);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A320);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A338);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A350);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A368);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A380);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A398);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A400);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A418);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A430);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A448);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A460);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A478);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A490);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A508);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A520);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A538);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A550);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A568);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A580);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A590);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A600);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A618);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A630);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F510);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F514);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F518);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F520);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F528);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F530);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F538);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F540);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F548);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F550);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F558);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F560);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F568);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F570);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F578);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F580);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F584);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F588);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F590);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F598);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5A0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5A8);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5B0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5B8);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5C0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5C4);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5D4);

INCLUDE_SDATA(const s32, "code_001D1470", pbMoveCharList);

INCLUDE_SDATA(const s32, "code_001D1470", blockDB);

INCLUDE_SDATA(const s32, "code_001D1470", bmap);

INCLUDE_SDATA(const s32, "code_001D1470", goal);

INCLUDE_SDATA(const s32, "code_001D1470", record);

INCLUDE_SDATA(const s32, "code_001D1470", mapCursor);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5F0);

INCLUDE_SDATA(const s32, "code_001D1470", degree_mode);

INCLUDE_SDATA(const s32, "code_001D1470", pbGameKeyState);

INCLUDE_SDATA(const s32, "code_001D1470", pbMapCursorWork);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5FC);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F600);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F608);

INCLUDE_SDATA(const s32, "code_001D1470", g_pdebug_0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F614);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F618);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F620);

