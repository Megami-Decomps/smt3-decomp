#include "sdf_screen_state.h"

SIZE_CHECK(sdfScreenParamSize, sdf3DScreenParam_t, 0xA0);
SIZE_CHECK(sdfScreenMatrixSize, sdfMatrix4_t, 0x40);
SIZE_CHECK(sdfScreenVectorSize, sdfVector4_t, 0x10);

/* These writable globals initially occupy file-backed zero bytes in retail.
 * The VU reads the projection rows at +0x70 and +0x80, stores both matrices
 * as four quadwords, and reads and writes the eye offset as one quadword. */
sdf3DScreenParam_t sdf3DScreenParam __attribute__((section(".data"), aligned(16))) = { 0 };
sdfMatrix4_t sdf3DScreenTm __attribute__((section(".data"), aligned(16))) = { 0 };
sdfMatrix4_t sdfCameraTm __attribute__((section(".data"), aligned(16))) = { 0 };
f32 sdf3DEyeOffset[4] __attribute__((section(".data"), aligned(16))) = { 0 };
