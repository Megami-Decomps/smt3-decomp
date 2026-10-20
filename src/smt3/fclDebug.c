#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern u32 fclDbgPID;

s32 FclDebug(void)
{
    return -1;
}

s32 fclChkDbgProcess(void)
{
    return fclDbgPID != 0;
}

INCLUDE_SDATA(const s32, "fclDebug", fclDbgPID);

