#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void dds3KillProcessName(char *namestr, s32 flag);

void fclFreeStage(void);

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

extern rcvWork_t *shutdownfunc;

extern void p2sdStopBGM(s32 id);

extern void sdfMemFreeDirect(void *ptr);

extern s8 gProcessStat_2;

extern char D_0053D390[];

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 handle);

extern char D_0054FBD8[];

extern char D_0053D3F8[];

extern char D_0053D408[];

extern void fclScriptForcedEnd(u8 *script);

typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ void (*calcSizes)();
} fclRagTex_t;

extern void fclRagTexInit(fclRagTex_t *tex, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                          void (*calc)());

extern void fclRagTexCalcSizes();

extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 e);

extern void evtDisableMoonDraw(void);

void fclRecoverEventEnd(void)
{
    dds3KillProcessName(D_0053D390, 1);
    fclFreeStage();
}

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D390);

INCLUDE_ASM(const s32, "facility/fclRecoverInit", fclRecoverLoadTextures);

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x44D4];
    /* 0x44D4 */ s32 texHandle[3];
} fclRecoverWork_t;

void fclRecoverFreeTextures(fclRecoverWork_t *work)
{
    s32 *tex = work->texHandle;
    s8 i;

    for (i = 0; i < 3; i++) {
        sdfTexFree(*tex++);
    }
    printf("RECOVER : Tex free.\n");
}

extern void lstInitListWindow(s32 a, s32 b, s32 color, s32 d, s32 e, s32 f, s32 g,
                              void *list);

void fclRecoverInitItemList(void *list)
{
    lstInitListWindow(0x1E0, 0x388, 0xFFFFF, 0x1C4, 0xB8, 0x19191E59, 0, list);
    cmpInitCursorLayout((u8 *)list + 0x15C, 0x21, 8, 0x193, 0x1A, 0x23);
}

void fclRecoverInitTargetList(void *list)
{
    lstInitListWindow(0x270, 0x410, 0xFFFFF, 0xEC, 0xA8, 0x19191E59, 0, list);
    cmpInitCursorLayout((u8 *)list + 0x15C, 0x1E, 0xC, 0xC4, 0x1A, 0x1A);
}

void fclRecoverInitRagTex(u8 *work)
{
    fclRagTex_t *tex = (fclRagTex_t *)(work + 0x4520);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclRagTexInit(tex, 0, 0, 0x32, 0, 0x200, 0x20, fclRagTexCalcSizes);
        tex++;
    }
}

INCLUDE_ASM(const s32, "facility/fclRecoverInit", fclRecoverUpdateInit);

void fclDestroyRecover(void)
{
    if (shutdownfunc == NULL) {
        return;
    }
    fclRecoverFreeTextures((fclRecoverWork_t *)shutdownfunc);
    fclScriptForcedEnd(shutdownfunc->unk_44D0);
    sdfMemFreeDirect(shutdownfunc->unk_44C8);
    fclRecoverEventEnd();
    sdfMemFreeDirect(shutdownfunc->allocation);
    shutdownfunc = NULL;
    gProcessStat_2 = 2;
    evtDisableMoonDraw();
    p2sdStopBGM(0x300000);
}

INCLUDE_ASM(const s32, "facility/fclRecoverInit", fclRecoverProcessStart);

void fclRecoverProcessEnd(void)
{
    dds3KillProcessName(D_0054FBD8, 1);
    dds3KillProcessName(D_0053D3F8, 1);
    dds3KillProcessName(D_0053D408, 1);
}

s32 rcvCheckProcessStat(void)
{
    s8 stat = gProcessStat_2;

    if (stat == 1) {
        return 1;
    }
    if (stat >= 2) {
        if (stat == 2) {
            gProcessStat_2 = 0;
        }
    }
    return 0;
}

void func_0020DBC8(void)
{
    if (shutdownfunc != NULL) {
        shutdownfunc->flags |= 0x10;
    }
}

s32 rcv_20DBE8(void)
{
    return 0;
}

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D3F8);

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D408);

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D420);

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D430);

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D440);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", shutdownfunc);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", gProcessStat_2);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBB5);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBB8);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBC0);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBC8);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBD0);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBD8);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBE0);

INCLUDE_SDATA(const s32, "facility/fclRecoverInit", D_0054FBE8);

