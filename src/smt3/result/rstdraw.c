#include "common.h"
#include "result2.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"



extern rstData_t *rstDrawWork;

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void nmeDrawFrame(s32 type, s32 color);

extern void rstDrawGetCursorRow(void);

extern void rstDrawStatusRow(s8 row, s8 mode, s8 flag);

extern void rstDrawStatusRow2(s8 row, s8 mode, s8 flag);

extern void rstDrawStatusPanel(s8 row, s8 mode);

extern s32 rstResetEncycPanel(void);

extern s32 rstResetEncycPanel2(void);

#ifdef NON_MATCHING
extern s32 cmpDrawBoxF(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
#endif

void rstDrawEvoBgFade(void)
{
    cmpDrawBoxF(0, 0, 0, 0x200, 0x1C0, rstDrawWork->fadeColor, 7);
}

/* NON_MATCHING: 103 of 116 words match at the exact size; retail keeps a register
 * copy of the work pointer that the C coalesces away. */
#ifdef NON_MATCHING
extern void cmpFindPanelEffSlot(s32 arg);
extern void cmpUpdateTerminate(void *work);

s32 rstDraw(dds3ProcessID_t *id)
{
    rstDrawWork = dds3GetProcessData(id);
    if (rstDrawWork->flags & 0x20) {
        cmpUpdateTerminate(rstDrawWork);
        return -1;
    }
    nmeDrawFrame(0x10, rstDrawWork->unk_2310);
    rstDrawEvoBgFade();
    if (rstDrawWork->flags & 2) {
        switch (rstDrawWork->phase) {
        case 0:
        case 24:
            rstDrawGetCursorRow();
            break;
        case 1:
            rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 1);
            break;
        case 3:
            if (rstDrawWork->unk_29 == 1) {
                rstDrawStatusRow2(-rstDrawWork->unk_1B, 0, rstDrawWork->unk_74);
            } else {
                rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 0);
            }
            break;
        case 5:
            rstDrawStatusRow(-rstDrawWork->unk_1B, 5, 0);
            break;
        case 6:
            rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 0);
            break;
        case 7:
        case 8:
            rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 1);
            break;
        case 10:
            rstDrawStatusRow2(-rstDrawWork->unk_1B, 2, rstDrawWork->unk_74);
            break;
        case 11:
        case 12:
        case 13:
            rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 0);
            break;
        case 17:
        case 18:
            rstResetEncycPanel();
            rstDrawStatusRow(-rstDrawWork->unk_1B, 2, 0);
            break;
        case 20:
            rstResetEncycPanel2();
            rstDrawStatusRow(-rstDrawWork->unk_1B, 0, 1);
            break;
        case 21:
            rstDrawStatusPanel(-rstDrawWork->unk_1B, 1);
            break;
        case 22:
            rstDrawStatusPanel(-rstDrawWork->unk_1B, 0);
            break;
        }
    }
    cmpFindPanelEffSlot(0x48);
    return 0;
}
#else
INCLUDE_ASM(const s32, "result/rstdraw", rstDraw);
#endif
