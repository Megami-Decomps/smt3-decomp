#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void evtFreeStage(void);

extern void dds3GlobalWorkDestroy(void);

extern void dds3GlobalWorkInit(void);

extern void sdfTexFree(sdfTexHandle_t *handle);
extern void sdfTexFreeDelayed(sdfTexHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *texture);
extern void frwFreeStaffFont(void);
extern void *frwGetStaffFontFRQ(u32 x, u32 y, s32 priority, s32 alternate,
                               s32 spacing, s32 width, u32 color,
                               const char *text, void *previous);
extern u32 frGetStrWidth2(void *request);
extern s32 frDrawFont(void *request);
extern s32 frDestroyFontRequestAfter(void *request);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ sdfTexHandle_t *textureHandleA;
    /* 0x0008 */ sdfTexHandle_t *textureHandleB;
    /* 0x000C */ sdfTexHandle_t *textureHandleC;
    /* 0x0010 */ sdfTexHandle_t *fontTexture;
    /* 0x0014 */ sdfMemHandle_t *fontMemoryA[4];
    /* 0x0024 */ void *uiTextureInputsA[4];
    /* 0x0034 */ sdfMemHandle_t *fontMemoryB[4];
    /* 0x0044 */ void *uiTextureInputsB[4];
    /* 0x0054 */ sdfMemHandle_t *fontMemoryC[15];
    /* 0x0090 */ void *uiTextureInputsC[15];
    /* 0x00CC */ u8 padCC[0x10];
    /* 0x00DC */ sdfMemHandle_t *fontMemoryD;
    /* 0x00E0 */ void *fontTextureInput;
    /* 0x00E4 */ u8 padE4[0x18];
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
extern void stfResetUI(void);
extern void stfLoadUITexture(s32 index);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

void dds3CreateGameData(s32 no)
{
    evtFreeStage();
    dds3GlobalWorkDestroy();
    dds3GlobalWorkInit();
}

extern s32 evtBitCheck(s32 eventNo);
extern char *stfMpicFileTable[];
extern char *stfPicFileTable[];
/* Padded shared asset-path pool remains in its native data owner. */
extern const char stfMlinePath01[], stfMlinePath02[], stfMlinePath03[], stfMlinePath04[];
extern const char stfLinePath01[], stfLinePath02[], stfLinePath03[], stfLinePath04[];
extern const char stfMPath01[], stfMPath02[], stfMPath03[], stfMPath04[];
extern void frwLoadStaffFont(void);

/* Load the selected staff-roll texture set and shared font resources. */
const char stfMlinePath01[23] = "/staff/stf_mline01.tmx";

const char stfMlinePath02[23] = "/staff/stf_mline02.tmx";

const char stfMlinePath03[23] = "/staff/stf_mline03.tmx";

const char stfMlinePath04[23] = "/staff/stf_mline04.tmx";

const char stfMPath01[19] = "/staff/stf_m01.tmx";

const char stfMPath02[19] = "/staff/stf_m02.tmx";

const char stfMPath03[19] = "/staff/stf_m03.tmx";

const char stfMPath04[19] = "/staff/stf_m04.tmx";

const char stfLinePath01[22] = "/staff/stf_line01.tmx";

const char stfLinePath02[22] = "/staff/stf_line02.tmx";

const char stfLinePath03[22] = "/staff/stf_line03.tmx";

const char stfLinePath04[22] = "/staff/stf_line04.tmx";

void stfFreeFiles(void)
{
    s32 i;

    if (evtBitCheck(0x12C)) {
        oStaff_UI->fontMemoryA[0] = sdfDevLoadFileEx(stfMlinePath01, (u32 *)&oStaff_UI->uiTextureInputsA[0], NULL);
        oStaff_UI->fontMemoryA[1] = sdfDevLoadFileEx(stfMlinePath02, (u32 *)&oStaff_UI->uiTextureInputsA[1], NULL);
        oStaff_UI->fontMemoryA[2] = sdfDevLoadFileEx(stfMlinePath03, (u32 *)&oStaff_UI->uiTextureInputsA[2], NULL);
        oStaff_UI->fontMemoryA[3] = sdfDevLoadFileEx(stfMlinePath04, (u32 *)&oStaff_UI->uiTextureInputsA[3], NULL);
        oStaff_UI->fontMemoryB[0] = sdfDevLoadFileEx(stfMPath01, (u32 *)&oStaff_UI->uiTextureInputsB[0], NULL);
        oStaff_UI->fontMemoryB[1] = sdfDevLoadFileEx(stfMPath02, (u32 *)&oStaff_UI->uiTextureInputsB[1], NULL);
        oStaff_UI->fontMemoryB[2] = sdfDevLoadFileEx(stfMPath03, (u32 *)&oStaff_UI->uiTextureInputsB[2], NULL);
        oStaff_UI->fontMemoryB[3] = sdfDevLoadFileEx(stfMPath04, (u32 *)&oStaff_UI->uiTextureInputsB[3], NULL);
        for (i = 0; i < 15; i++) {
            oStaff_UI->fontMemoryC[i] = sdfDevLoadFileEx(stfMpicFileTable[i], (u32 *)&oStaff_UI->uiTextureInputsC[i], NULL);
        }
    } else {
        oStaff_UI->fontMemoryA[0] = sdfDevLoadFileEx(stfLinePath01, (u32 *)&oStaff_UI->uiTextureInputsA[0], NULL);
        oStaff_UI->fontMemoryA[1] = sdfDevLoadFileEx(stfLinePath02, (u32 *)&oStaff_UI->uiTextureInputsA[1], NULL);
        oStaff_UI->fontMemoryA[2] = sdfDevLoadFileEx(stfLinePath03, (u32 *)&oStaff_UI->uiTextureInputsA[2], NULL);
        oStaff_UI->fontMemoryA[3] = sdfDevLoadFileEx(stfLinePath04, (u32 *)&oStaff_UI->uiTextureInputsA[3], NULL);
        oStaff_UI->fontMemoryB[0] = sdfDevLoadFileEx(stfMPath01, (u32 *)&oStaff_UI->uiTextureInputsB[0], NULL);
        oStaff_UI->fontMemoryB[1] = sdfDevLoadFileEx(stfMPath02, (u32 *)&oStaff_UI->uiTextureInputsB[1], NULL);
        oStaff_UI->fontMemoryB[2] = sdfDevLoadFileEx(stfMPath03, (u32 *)&oStaff_UI->uiTextureInputsB[2], NULL);
        oStaff_UI->fontMemoryB[3] = sdfDevLoadFileEx(stfMPath04, (u32 *)&oStaff_UI->uiTextureInputsB[3], NULL);
        for (i = 0; i < 15; i++) {
            oStaff_UI->fontMemoryC[i] = sdfDevLoadFileEx(stfPicFileTable[i], (u32 *)&oStaff_UI->uiTextureInputsC[i], NULL);
        }
    }
    oStaff_UI->fontMemoryD = sdfDevLoadFileEx("/staff/capcom_logo.tmx", (u32 *)&oStaff_UI->fontTextureInput, NULL);
    oStaff_UI->fontTexture = NULL;
    oStaff_UI->fontTexture = sdfTexSet(oStaff_UI->fontTextureInput);
    frwLoadStaffFont();
}

void stfFreeFont(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (oStaff_UI->fontMemoryA[i] != NULL) {
            sdfMemFreeDirect(oStaff_UI->fontMemoryA[i]);
            oStaff_UI->fontMemoryA[i] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        if (oStaff_UI->fontMemoryB[i] != NULL) {
            sdfMemFreeDirect(oStaff_UI->fontMemoryB[i]);
            oStaff_UI->fontMemoryB[i] = NULL;
        }
    }
    for (i = 0; i < 15; i++) {
        if (oStaff_UI->fontMemoryC[i] != NULL) {
            sdfMemFreeDirect(oStaff_UI->fontMemoryC[i]);
            oStaff_UI->fontMemoryC[i] = NULL;
        }
    }
    if (oStaff_UI->fontTexture != NULL) {
        sdfTexFreeDelayed(oStaff_UI->fontTexture);
    }
    oStaff_UI->fontTexture = NULL;
    if (oStaff_UI->fontMemoryD != NULL) {
        sdfMemFreeDirect(oStaff_UI->fontMemoryD);
        oStaff_UI->fontMemoryD = NULL;
    }
    frwFreeStaffFont();
}

s32 stfDrawStaffText(f32 x, f32 y, s32 style, u32 color, const char *text)
{
    void *request;

    if (style == 0) {
        request = frwGetStaffFontFRQ((s32)(x * 16.0f), (s32)(y * 8.0f),
                                   0, 1, 1, 10, color, text, NULL);
    } else {
        request = frwGetStaffFontFRQ((s32)(x * 16.0f), (s32)(y * 8.0f),
                                   0, 0, 1, 8, color, text, NULL);
    }
    frGetStrWidth2(request);
    frDrawFont(request);
    return frDestroyFontRequestAfter(request);
}

u32 stfDrawText(s32 style, u32 color, const char *text)
{
    void *request;
    u32 width;

    if (style == 0) {
        request = frwGetStaffFontFRQ(0, 0, 0, 0, 1, 10, color, text, NULL);
    } else {
        request = frwGetStaffFontFRQ(0, 0, 0, style, 1, 8, color, text, NULL);
    }
    width = frGetStrWidth2(request);
    frDestroyFontRequestAfter(request);
    return width;
}

void stfLoadUITexture(s32 index)
{
    s32 fullIndex = index;
    index %= 4;
    oStaff_UI->textureHandleA = sdfTexSet(oStaff_UI->uiTextureInputsA[index]);
    oStaff_UI->textureHandleB = sdfTexSet(oStaff_UI->uiTextureInputsB[index]);
    oStaff_UI->textureHandleC = sdfTexSet(oStaff_UI->uiTextureInputsC[fullIndex]);
}

void stfFreeTextures(void)
{
    if (oStaff_UI->textureHandleA != 0) {
        sdfTexFree(oStaff_UI->textureHandleA);
    }
    if (oStaff_UI->textureHandleB != 0) {
        sdfTexFree(oStaff_UI->textureHandleB);
    }
    if (oStaff_UI->textureHandleC != 0) {
        sdfTexFree(oStaff_UI->textureHandleC);
    }
    oStaff_UI->textureHandleA = 0;
    oStaff_UI->textureHandleB = 0;
    oStaff_UI->textureHandleC = 0;
}

void stfGetUIField(s32 arg0, s32 arg1, s32 arg2)
{
    if (oStaff_UI->unk_0118 == 1) {
        stfResetUI();
    }
    oStaff_UI->unk_010C = arg2;
    oStaff_UI->unk_0108 = arg1;
    oStaff_UI->unk_00FC = arg0;
    oStaff_UI->unk_0104 = 0;
    oStaff_UI->unk_0114 = 0;
    oStaff_UI->unk_0100 = 0;
    stfLoadUITexture(arg0);
    oStaff_UI->unk_0118 = 1;
}

void stfResetUI(void)
{
    stfFreeTextures();
    oStaff_UI->unk_00FC = 0;
    oStaff_UI->unk_0100 = 0;
    oStaff_UI->unk_0104 = 0;
    oStaff_UI->unk_0114 = 0;
    oStaff_UI->unk_0108 = 0;
    oStaff_UI->unk_010C = 0;
    oStaff_UI->unk_0118 = 0;
}

stfUiWork_t *oStaff_UI = 0;

