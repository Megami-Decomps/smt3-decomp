#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

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

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ s8 phase;
    /* 0x0002 */ u8 unk_0002[0x19];
    /* 0x001B */ u8 unk_1B;
    /* 0x001C */ u8 unk_001C[0xD];
    /* 0x0029 */ s8 unk_29;
    /* 0x002A */ u8 unk_002A[0x4A];
    /* 0x0074 */ s8 unk_74;
    /* 0x0075 */ u8 unk_0075[0x7];
    /* 0x007C */ s8 unk_7C;
    /* 0x007D */ s8 unk_7D;
    /* 0x007E */ u8 unk_007E[0xEE];
    /* 0x016C */ void *unk_16C;
    /* 0x0170 */ u8 unk_170[0x112E];
    /* 0x129E */ s8 unk_129E;
    /* 0x129F */ u8 unk_129F[6];
    /* 0x12A5 */ u8 unk_12A5[0x103F];
    /* 0x22E4 */ u8 unk_22E4;
    /* 0x22E5 */ u8 unk_22E5[0x2B];
    /* 0x2310 */ s32 unk_2310;
    /* 0x2314 */ s32 fadeColor;
} rstDrawWork_t;

extern rstDrawWork_t *rstDrawWork;

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
