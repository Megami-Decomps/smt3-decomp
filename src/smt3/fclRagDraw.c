#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

s32 fclGetFlag(s8 index);

typedef struct fclPanel
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

void fclInitPanelPos(s32 a, s32 b, s32 c, s16 d, s16 e, s32 f, fclPanel_t *panel);

extern s32 cmpUpdateListSelection(fclPanelPos_t *pos, u8 *list, s32 a,
                                  s32 mode, s32 b, s32 id);

typedef struct
{
    /* 0x0000 */ u8 unk_00[3];
    /* 0x0003 */ s8 unk_03;
    /* 0x0004 */ u8 unk_0004[0x4650];
    /* 0x4654 */ s32 texHandle[1];
} ragDrawWork_t;

extern ragDrawWork_t *ragDrawWork;

INCLUDE_ASM(const s32, "fclRagDraw", ragDrawSeqStatus);

extern void ragDrawSeqStatus(void);

extern void ragDrawHeartsList(s32 mode);

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

s32 ragDrawSeqRoot(s32 active)
{
    fclPanelPos_t pos;
    s32 mode = 1;

    if (active != 0) {
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

INCLUDE_ASM(const s32, "fclRagDraw", ragDrawSeqErr);

INCLUDE_ASM(const s32, "fclRagDraw", ragDraw);
