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

INCLUDE_RODATA(const s32, "field/fldTitle", D_00532820);

INCLUDE_ASM(const s32, "field/fldTitle", fldTitleMiniStart);
