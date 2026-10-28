#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern char D_0053D640[];

extern char D_0053D650[];

extern char D_0053D660[];

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0x38];
    /* 0x003D */ s8 unk_003D;
    /* 0x003E */ u8 unk_003E[0x1198];
    /* 0x11D6 */ s16 unk_11D6;
    /* 0x11D8 */ u8 unk_11D8;
    /* 0x11D9 */ u8 unk_11D9[0x1018];
    /* 0x21F1 */ s8 TerminalType;
    /* 0x21F2 */ u16 TerminalNo;
    /* 0x21F4 */ s8 TerminalList[0x21];
    /* 0x2215 */ s8 unk_2215;
    /* 0x2216 */ u8 unk_2216[0xA];
    /* 0x2220 */ s32 ScriptPID;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ u8 unk_2234[4];
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

extern trmUpdateWork_t *trmDrawWork;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 fclChkDbgProcess(void);

extern s32 trmChkEventWait(void);

extern void cmpUpdateTerminate();

extern s32 fclGetFlag(s8 index);

extern void trmDrawSeq0(s8 mode);

extern void trmDrawList(void);

extern void trmUpdateListSelection(void);

s32 trmDraw(dds3ProcessID_t *id)
{
    trmDrawWork = dds3GetProcessData(id);
    if (fclChkDbgProcess() != 0) {
        return 0;
    }
    if (trmChkEventWait() != 0) {
        return 0;
    }
    if ((trmDrawWork->unk_0000 & 0x20) != 0) {
        cmpUpdateTerminate(trmDrawWork);
        return -1;
    }
    if ((trmDrawWork->unk_0000 & 0x2) != 0) {
        switch (trmDrawWork->unk_0001) {
        case 0:
            if (fclGetFlag(0) != 0) {
                trmDrawSeq0(0);
            } else {
                trmDrawSeq0(2);
            }
            break;
        case 1:
            trmDrawList();
            break;
        case 5:
            trmUpdateListSelection();
            break;
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
            break;
        }
    }
    return 0;
}

extern s16 fldAllCnt[][2];

s32 trmGetSaveResult(void)
{
    if (trmUpdateWork->TerminalType == 0) {
        s32 phase = trmUpdateWork->TerminalNo;

        if (phase < 0x14) {
            if (phase >= 0x12) {
                return 0x1CE006D;
            }
        }
        return 0x1CC006D;
    }
    return 0x1CD006D;
}
