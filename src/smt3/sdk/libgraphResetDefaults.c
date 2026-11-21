#include "sdk_gs_reset_defaults.h"

SIZE_CHECK(sceGsDefaultGParamSize, GParam, 16);
SIZE_CHECK(sceGsVifQuadwordSize, sceGsVifCommands_t, 16);

GParam sceGsGParam = {
    .interlace = 1,
    .mode = 2,
    .fieldMode = 1,
    .version = 3,
};

/* Two quadwords of VIF initialization in the original initialized data. */
const sceGsVifCommands_t sceGsResetPathPackets[2]
    __attribute__((section(".data"))) = {
    { .vif = { 0x01000404, 0x20000000, 0, 0x05000000 } }, /* STCYCL, STMASK + value, STMOD */
    { .vif = { 0x06000000, 0x03000000, 0x02000000, 0x04000000 } }, /* MSKPATH3, BASE, OFFSET, ITOP */
};
