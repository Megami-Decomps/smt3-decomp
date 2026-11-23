#include "sdf_resource.h"
#include "draw3d.h"
#include "sdf_lighting.h"

SIZE_CHECK(cmpViewDrawTagSize, sdfDrawTag_t, 0x20);
SIZE_CHECK(cmpViewInitSize, sdf3DInitPacket_t, 0x20);
SIZE_CHECK(cmpViewLightSize, sdf3DLightParallelPacket_t, 0xE0);
SIZE_CHECK(cmpViewPortSize, sdf3DViewportPacket_t, 0xD0);

/* The camp view builder writes one packet set per frame buffer. */
sdfDrawTag_t cmpViewTag[2] __attribute__((section(".bss"), aligned(16)));
sdf3DInitPacket_t cmpViewInitPacket[2] __attribute__((section(".bss"), aligned(16)));
sdfDrawTag_t cmpLightTag[2] __attribute__((section(".bss"), aligned(16)));
sdf3DLightParallelPacket_t cmpLightPacket[2] __attribute__((section(".bss"), aligned(16)));
sdf3DViewportPacket_t cmpViewportPacket[2] __attribute__((section(".bss"), aligned(16)));
