#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

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

extern void sdfMemFreeDirect(void *aHandle);

extern s8 gProcessStat_2;

extern char D_0053D390[];

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 aHandle);

extern char D_0054FBD8[];

extern char D_0053D3F8[];

extern char D_0053D408[];

extern void fclScriptForcedEnd(u8 *PID);

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

extern void cmpInitCursorLayout(void *cur, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern void evtDisableMoonDraw(void);

extern char *D_003306C0[];

extern u8 rcv_data[];

extern u8 D_00551F51[2];

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void *memset(void *a, s32 b, u32 c);

extern void cmpInitSeqInfo(void *work, s32 seq);

extern void fclInitCursorWindow(void *cur, u8 *data, s32 count);

extern void fclSetCursorList(void *cur, s32 count);

extern void cmpSetCursorFlag(void *cur, u8 *flags);

extern void fclRecoverInitItemList(void *list);

extern void fclRecoverInitTargetList(void *list);

extern void fclRecoverInitRagTex(u8 *work);

extern void fclRecoverLoadTextures(void *work);

extern void fclInitPanel(void *panel, s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 color, s8 type,
                         s8 speed);

extern void fclStartRecoverEvent(s32 mode);

extern void *fclLoadTexture(char *pFileName);

extern void fclSetPanelTex(void *panel, s32 a2, s32 a3, s32 a4, s32 a5, s32 tex);

extern s32 rcvCalc(dds3ProcessID_t *id);

extern s32 rcvDraw(dds3ProcessID_t *id);

extern s32 rcvUpdate(dds3ProcessID_t *id);

extern void fclDestroyRecover(void);

extern void *dds3StartProcess(char *name, u32 priority, s32 v2, s32 v3,
                              s32 (*processfunc)(dds3ProcessID_t *id), void *shutdownfunc,
                              void *data);

void fclRecoverEventEnd(void)
{
    dds3KillProcessName(D_0053D390, 1);
    fclFreeStage();
}

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x44D4];
    /* 0x44D4 */ s32 texHandle[3];
    /* 0x44E0 */ u8 panel[0x40];
} fclRecoverWork_t;

INCLUDE_RODATA(const s32, "facility/fclRecoverInit", D_0053D390);

void fclRecoverLoadTextures(void *workp)
{
    fclRecoverWork_t *work = workp;
    s8 i;

    for (i = 0; i < 3; i++) {
        work->texHandle[i] = (s32)fclLoadTexture(D_003306C0[i]);
    }
    fclSetPanelTex(work->panel, 0, 0, 0x100, 0x80, work->texHandle[2]);
    printf("RECOVER : Tex load.\n");
}

void fclRecoverFreeTextures(fclRecoverWork_t *work)
{
    s32 *tex = work->texHandle;
    s8 i;

    for (i = 0; i < 3; i++) {
        sdfTexFree(*tex++);
    }
    printf("RECOVER : Tex free.\n");
}

extern void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s32 Style,
                              void *pListWindow);

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

rcvWork_t *fclRecoverUpdateInit(s8 mode, s32 seq)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x4568);
    u8 *work = sdfMemLock(handle);

    memset(work, 0, 0x4568);
    *(sdfMemHandle_t **)(work + 0x14) = handle;
    cmpInitSeqInfo(work, seq);
    work[1] = 0;
    work[0x18] = mode;
    work[4] = 1;
    work[3] = 0;
    *(s32 *)(work + 0x44D0) = 0;
    fclInitCursorWindow(work + 0x1C, rcv_data, 7);
    fclSetCursorList(work + 0x1C, 0x19);
    D_00551F51[0] |= 2;
    D_00551F51[1] |= 2;
    work[0x41] = 2;
    cmpSetCursorFlag(work + 0x20, D_00551F51 - 1);
    fclRecoverInitItemList(work + 0x1130);
    fclRecoverInitTargetList(work + 0x22C8);
    fclInitPanel(work + 0x44E0, 0, 0x298, 0xFFFFF, 0x200, 0xE4, 0, 0, 0x1D);
    *(void **)(work + 0x44C8) =
        sdfDevLoadFileEx("/facility/scr/recover/message.bf", (u32 *)(work + 0x44CC), 0);
    fclStartRecoverEvent(1);
    evtDisableMoonCalc();
    fclRecoverLoadTextures(work);
    fclRecoverInitRagTex(work);
    p2sdPlayBGM(0x300000);
    fclSetWorkValue(2, 0);
    return (rcvWork_t *)work;
}

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

void fclRecoverProcessStart(s8 mode)
{
    if (*(s32 *)((u8 *)DDS3_GBWK + 0x1924) == 0) {
        return;
    }
    if (shutdownfunc != NULL) {
        return;
    }
    shutdownfunc = fclRecoverUpdateInit(mode, 0);
    dds3StartProcess(D_0054FBD8, 0x401, 1, 1, rcvCalc, NULL, shutdownfunc);
    dds3StartProcess(D_0053D3F8, 0x2B11, 1, 1, rcvDraw, NULL, shutdownfunc);
    dds3StartProcess(D_0053D408, 0x520D, 1, 1, rcvUpdate, fclDestroyRecover, shutdownfunc);
    gProcessStat_2 = 1;
}

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

