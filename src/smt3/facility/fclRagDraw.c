#include "common.h"
#include "facility.h"
#include "camp.h"
#include "fclRag.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

s32 fclGetFlag(s8 index);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern s32 cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 a,
                                  s32 mode, s32 b, s32 id);


extern void ragDrawList2(s32 mode);

extern void ragDrawList3(s32 mode);

extern void ragDrawSeqErr(void);

extern s32 ragDrawSeqRoot(s32 Mode);

extern void cmpModelInitCore2(void);

extern void fclDrawBG(s32 *texHandle, s32 ot);

extern void cmpDrawBoxF(s32 x, s32 y, s32 color, s32 w, s32 h, s32 arg6, s32 ot);

extern void cmbCopyDefaultDevilToStock(s32 id, datUnitWork_t *work);

extern void cmbCopyCustomDevilToStock(datUnitWork_t *work, u8 *source);

extern s32 datGetParam(datUnitWork_t *work, s32 index);

extern void cmbSetBirthPanel(datUnitWork_t *work);


extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 fclChkDbgProcess(void);

extern void cmpUpdateTerminate(void *work);

void ragDrawSeqStatus(void)
{
    s8 delta[6];
    datUnitWork_t custom;
    datUnitWork_t base;
    s32 i;

    cmpModelInitCore2();
    fclDrawBG(ragDrawWork->bgTexHandle, 7);
    cmpDrawBoxF(0, 0x198, ragDrawWork->boxColor, 0x200, 0x168, 0x33, 7);
    cmbCopyDefaultDevilToStock(ragDrawWork->devilId, &base);
    cmbCopyCustomDevilToStock(&custom, ragDrawWork->customSource);
    custom.hp = custom.maxhp;
    custom.mp = custom.maxmp;
    for (i = 0; i < 6; i++) {
        delta[i] = datGetParam(&custom, i) - base.param[i];
        custom.param[i] -= delta[i];
    }
    cmbSetBirthPanel(&base);
    cmpDrawStatusComEx(&custom, 0x80, -1, 2, delta, 0, 0xA09DC380, 3, 2, 2);
    cmbSetBirthPanel(NULL);
}


extern void ragDrawHeartsList(s8 mode);

void ragDrawSeqConfirm(void)
{
    switch (ragDrawWork->unk_03) {
    case 1:
        ragDrawHeartsList(1);
        return;
    case 2:
        ragDrawSeqStatus();
        return;
    case 3:
        ragDrawSeqStatus();
        break;
    }
}

s32 ragDrawSeqRoot(s32 Mode)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (Mode != 0) {
        mode = 0;
        if (fclGetFlag(0) == 0) {
            mode = 2;
            if (*(s8 *)((u8 *)ragDrawWork + 3) == 4) {
                mode = 1;
            }
        }
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0x80, 0, 0, (fclPanel_t *)&pos);
    cmpUpdateListSelection(&pos, (u8 *)ragDrawWork + 0x140, 0, mode, 0x100, 0x47);
}

void ragDrawSeqErr(void)
{
    switch (ragDrawWork->unk_03) {
    case 1:
        ragDrawHeartsList(1);
        return;
    case 2:
        ragDrawList2(1);
        return;
    case 3:
        ragDrawList3(1);
        return;
    case 7:
        ragDrawSeqStatus();
        break;
    }
}

s32 ragDraw(dds3ProcessID_t *id)
{
    ragDrawWork_t *work;

    ragDrawWork = dds3GetProcessData(id);
    if (fclChkDbgProcess() != 0) {
        return 0;
    }
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    work = ragDrawWork;
    if (work->flags & 0x20) {
        cmpUpdateTerminate(work);
        return -1;
    }
    if (work->flags & 2) {
        switch (work->seq) {
        case 0:
            ragDrawSeqRoot(1);
            break;
        case 1:
            ragDrawHeartsList(fclGetFlag(0) == 0);
            break;
        case 2:
            ragDrawList2(fclGetFlag(0) == 0);
            break;
        case 3:
            ragDrawList3(fclGetFlag(0) == 0);
            break;
        case 4:
        case 5:
            ragDrawSeqRoot(0);
            break;
        case 6:
            ragDrawSeqErr();
            break;
        case 7:
            ragDrawSeqStatus();
            break;
        case 8:
            ragDrawSeqConfirm();
            break;
        }
    }
    return 0;
}
