#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
} fclEncyc_instance_tag;

extern void evtFreeStage(void);

extern void func_00116BB8(void);

extern s32 dds3GlobalWorkInit(void);

extern void sdfTexFree(sdfTexHandle_t *handle);
extern void sdfTexFreeDelayed(sdfTexHandle_t *handle);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern sdfTexHandle_t *sdfTexSet(void *texture);
extern void frwFreeStaffFont(void);

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
    /* 0x00E0 */ u8 padE0[0x1C];
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

s32 dds3CreateGameData(void)
{
    evtFreeStage();
    func_00116BB8();
    return dds3GlobalWorkInit();
}

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfFreeFiles);

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

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfDrawStaffText);

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfDrawText);

#ifdef NON_MATCHING
/* NON_MATCHING: 12 of 35 words differ (first at +0x4). */
void stfLoadUITexture(s32 index)
{
    s32 slot = index % 4;
    oStaff_UI->textureHandleA = sdfTexSet(oStaff_UI->uiTextureInputsA[slot]);
    oStaff_UI->textureHandleB = sdfTexSet(oStaff_UI->uiTextureInputsB[slot]);
    oStaff_UI->textureHandleC = sdfTexSet(oStaff_UI->uiTextureInputsC[index]);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfLoadUITexture);
#endif

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

INCLUDE_SDATA(const s32, "kernel/dds3CreateData", oStaff_UI);

