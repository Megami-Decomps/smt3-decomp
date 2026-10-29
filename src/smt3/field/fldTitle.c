#include "common.h"
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
extern s32 sprintf(char *destination, const char *format, ...);
extern s32 fldTitleState, D_0054EF8C, D_0054EF90, D_0054EF94;
extern void *sdfDevLoadFileEx(const char *name, u32 *data, u32 *size);
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

INCLUDE_ASM(const s32, "field/fldTitle", fldTitle_Process);

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

INCLUDE_ASM(const s32, "field/fldTitle", fldTitleMini_Process);

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

extern s32 fldTitleMiniState;
extern s32 fldSnowActive;
extern s32 fldArea[];
extern s32 D_0054EFB0;
extern s32 evtBitCheck(s32 no);
extern s32 fldTitleMini_Process(dds3ProcessID_t *id);

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

