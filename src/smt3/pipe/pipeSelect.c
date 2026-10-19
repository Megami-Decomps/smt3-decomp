#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

s32 pipeSelectSection(void)
{
    return 0;
}

void DataRelease(void) {
}
