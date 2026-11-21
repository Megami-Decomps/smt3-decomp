#include "sdf_types.h"

/* LookAtMatrix reads all three vectors with quadword loads. The first
 * aligned object and the 16-byte records keep every input aligned. */
sdfVector4_t dds3RenderTargetVector __attribute__((aligned(16))) = { 0.0f, -100.0f, 0.0f, 0.0f };
sdfVector4_t dds3RenderEyeVector = { 0.0f, -100.0f, -400.0f, 0.0f };
sdfVector4_t effPCPBossTexRect = { 0.0f, 1.0f, 0.0f, 0.0f };
