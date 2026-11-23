#include "field.h"

SIZE_CHECK(fldAutoMapVectorSize, sdfVector4_t, 0x10);
SIZE_CHECK(fldAutoMapXYZSize, fldAmXYZ_t, 0xC);

/* The two homogeneous vectors precede the per-model XYZ positions. */
sdfVector4_t fldAutoMapFieldId __attribute__((section(".bss"), aligned(16)));
sdfVector4_t fldAutoMapPosX __attribute__((section(".bss"), aligned(16)));
fldAmXYZ_t fldAutoMapModelPos[32] __attribute__((section(".bss")));
