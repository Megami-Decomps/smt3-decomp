#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

#include "kernel.h"

/* Only the flag byte, the draw phase, the row table and the two list indices
 * this unit reads are named. */
typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ s8 phase;
    /* 0x0002 */ u8 unk_0002[0x105A];
    /* 0x105C */ s32 rowNum[1];
    /* 0x1060 */ u8 unk_1060[0x24E];
    /* 0x12AE */ s16 listTop;
    /* 0x12B0 */ u8 listIndex;
} rcvDrawWork_t;

extern rcvDrawWork_t *rcvDrawWork;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 fclGetFlag(s8 index);

extern void cmpUpdateTerminate();

extern void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work,
                            fclPanelPos_t *pPanelPos);

extern void rcvDrawPanelRow(fclPanelPos_t *pos, s32 color, s32 num, s32 ot);

extern void rcvDrawSeq1(void);

extern void rcvDrawSeq2(void);

extern void rcvDrawSeq3(void);

extern void rcvDrawSeq4(void);

extern void rcvUpdateItemList(s32 mode);

extern void rcvDrawItemList(void);

extern s32 rcvIsHealPhase(void);

s32 rcvDraw(dds3ProcessID_t *id)
{
    fclPanelPos_t panel;

    rcvDrawWork = dds3GetProcessData(id);
    if (fclGetFlag(1) == 0) {
        return 0;
    }
    if ((rcvDrawWork->flags & 0x20) != 0) {
        cmpUpdateTerminate(rcvDrawWork);
        return -1;
    }
    fclInitPanelPos(0, 0, 0xFFFFF, 0, 0, 0, &panel);
    rcvDrawPanelRow(&panel, 0x80808080,
                    rcvDrawWork->rowNum[rcvDrawWork->listTop + rcvDrawWork->listIndex], 0x47);
    if ((rcvDrawWork->flags & 0x2) != 0) {
        switch (rcvDrawWork->phase) {
        case 0:
            rcvUpdateItemList(1);
            break;
        case 1:
            rcvDrawSeq1();
            break;
        case 2:
            rcvDrawSeq2();
            break;
        case 3:
            rcvDrawSeq3();
            break;
        case 4:
            rcvDrawSeq4();
            break;
        case 6:
            if (rcvIsHealPhase() != 0) {
                rcvDrawItemList();
            } else {
                rcvUpdateItemList(0);
            }
            break;
        case 5:
            rcvDrawItemList();
            break;
        case 7:
            rcvUpdateItemList(0);
            break;
        }
    }
    return 0;
}
