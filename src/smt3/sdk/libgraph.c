#include "common.h"

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

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsPutIMR);

INCLUDE_ASM(const s32, "sdk/libgraph", sceGsGetIMR);
