#include "common.h"
#include "kernel.h"

extern char D_00551690[];



extern s8 gProcessStat;

/* Only the flag byte this unit tests is named. */
typedef struct { u8 flags; } nmeWork_t;

extern nmeWork_t *nmeUpdateWork;

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 dds3FadeCheck(void);

extern void nmeUpdateInput(void);

extern void nmeAdvanceAnim(void);

s32 nmeUpdate(dds3ProcessID_t *id) {
    nmeUpdateWork = dds3GetProcessData(id);
    if (dds3FadeCheck() != 0) {
        return 0;
    }
    if ((nmeUpdateWork->flags & 0x40) != 0) {
        return -1;
    }
    nmeUpdateInput();
    nmeAdvanceAnim();
    return 0;
}

