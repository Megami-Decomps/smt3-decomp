#include "sdf_tm_packet.h"

SIZE_CHECK(sdf3DPrimIdentityTmSize, sdf3DTmPacketData_t, 0x80);

/* Identity transform and normal rows for the eight-quadword DMA reset. */
sdf3DTmPacketData_t sdf3DPrimIdentityTm __attribute__((aligned(16))) = {
    .tm = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
    },
    .cmd = { 0x04000010, 0x14000000 }, /* ITOP 16, MSCAL 0, then NOPs. */
};
