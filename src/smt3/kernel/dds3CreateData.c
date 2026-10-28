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

extern void sdfTexFree(s32 aHandle);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ s32 unk_0004;
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ s32 unk_000C;
    /* 0x0010 */ u8 unk_0010[0xEC];
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

s32 dds3CreateGameData(void)
{
    evtFreeStage();
    func_00116BB8();
    return dds3GlobalWorkInit();
}

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfFreeFiles);

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfFreeFont);

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfDrawStaffText);

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfDrawText);

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfLoadUITexture);

void stfFreeTextures(void)
{
    if (oStaff_UI->unk_0004 != 0) {
        sdfTexFree(oStaff_UI->unk_0004);
    }
    if (oStaff_UI->unk_0008 != 0) {
        sdfTexFree(oStaff_UI->unk_0008);
    }
    if (oStaff_UI->unk_000C != 0) {
        sdfTexFree(oStaff_UI->unk_000C);
    }
    oStaff_UI->unk_0004 = 0;
    oStaff_UI->unk_0008 = 0;
    oStaff_UI->unk_000C = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3CreateData", stfGetUIField);

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

