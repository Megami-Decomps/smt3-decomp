#include "effDebug.h"

SIZE_CHECK(effViewerMenuSize, effDebugMenu_t, 0x40);

extern const char D_00533EF0[16];

/* The empty startup menu keeps its native self address in the item slot.
 * Both editor paths test the zero item count before reading any item. */
effDebugMenu_t D_003200B8 = {
    .title = D_00533EF0,
    .x = 18,
    .y = 18,
    .items = (effDebugMenuItem_t *)&D_003200B8,
    .itemCount = 0,
    .mode = 1,
};
