#include "field.h"
#include "sdf_resource.h"

SIZE_CHECK(fldNpcSlotSize, fldNpcSlot_t, 0x70);

/* Native field model records, followed by the independent blur handles. */
fldNpcSlot_t fldModelSlotTable[32] __attribute__((section(".bss"), aligned(16)));
sdfMemHandle_t *fldBlurEffectTable[64] __attribute__((section(".bss")));
