#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern u32 fclDbgPID;

/* The retail debug hook keeps its mode/work API and returns disabled status. */
s8 FclDebug(s8 mode, void *work)
{
    return -1;
}

s32 fclChkDbgProcess(void)
{
    return fclDbgPID != 0;
}

INCLUDE_SDATA(const s32, "facility/fclDebug", fclDbgPID);

