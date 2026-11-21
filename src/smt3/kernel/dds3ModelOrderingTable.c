#include "model.h"

SIZE_CHECK(dds3ModelOtRowSize, sdfModelOt_t, 0x10);

/* Each model packet group has four ordering-table heads. The native file
 * starts these writable heads null; the frame and field setup fill them. */
sdfModelOt_t sdfModelOtTable[11] = { 0 };
