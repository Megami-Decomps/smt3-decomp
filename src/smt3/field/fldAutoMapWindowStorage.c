#include "sdf_resource.h"

SIZE_CHECK(fldAutoMapWindowHandleSize, sdfTexHandle_t *, 4);

sdfTexHandle_t *fldAutoMapWindowTex[3] __attribute__((section(".bss"), aligned(4)));
