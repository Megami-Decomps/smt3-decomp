#include "common.h"
#include "kernel.h"
#include "libsdf.h"

void dds3InitDebugSystem(void) {
}
u8 dds3PadStickPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

s8 dds3PadStickStatusPrev[4] = {0, 0, 0, 0};

u8 D_0054E904 = 0;

u8 D_0054E905 = 0;

u8 D_0054E906 = 0;

u8 D_0054E907 = 0;

u8 dds3PadStickState[8] = {0, 0, 0, 0, 0, 0, 0, 0};

u8 dds3PadStickStatusState[4] = {0, 0, 0, 0};

s32 D_0054E918[2] = {1, 0};

u32 D_0054E920[2] = {0, 0};

char D_0054E928[8] = "TGS";

char D_0054E930[8] = "PIPE";

char D_0054E938[8] = "FIELD";

