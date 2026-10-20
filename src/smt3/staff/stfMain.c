#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void dds3KillProcessName(char *namestr, s32 flag);

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_t;

extern void sdfMemFreeDirect(void *ptr);

extern s32 stfShutDown(void);

extern char D_0053F100[];

extern u16 dds3VblanksPerFrame;

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

extern void (*DrawPhaseTable[])(fclEncyc_instance_t *work);

INCLUDE_ASM(const s32, "staff/stfMain", stfMain2);

INCLUDE_ASM(const s32, "staff/stfMain", GetoStaffUI);

INCLUDE_ASM(const s32, "staff/stfMain", ObjDispOff);

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

INCLUDE_ASM(const s32, "staff/stfMain", stfShutDown);

s32 stfExit(void)
{
    stfShutDown();
    return -1;
}

s32 stfInit(void)
{
    dds3KillProcessName(D_0053F100, 1);
    return 0;
}

INCLUDE_RODATA(const s32, "staff/stfMain", D_0053F100);

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

INCLUDE_SDATA(const s32, "staff/stfMain", D_0054FD90);

