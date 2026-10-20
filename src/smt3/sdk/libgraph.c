#include "common.h"

/* The GS interrupt mask register, through the kernel's syscall stubs. */
extern u64 GsGetIMR(void);
extern u64 GsPutIMR(u64 imr);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsResetGraph);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsGetGParam);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsResetPath);

INCLUDE_ASM(const s32, "sdk/libgraph", checkModelVersion);

INCLUDE_ASM(const s32, "sdk/libgraph", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSetDefDispEnv);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsPutDispEnv);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSyncPath);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSetDefLoadImage);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsSetDefStoreImage);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsExecLoadImage);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsExecStoreImage);

u64 sceGsPutIMR(u64 imr)
{
    u64 old = GsGetIMR();

    GsPutIMR(imr);
    return old;
}

u64 sceGsGetIMR(void)
{
    return GsGetIMR();
}
