#include "effDebug.h"

extern const char D_00533C40[16];
extern effDebugMenuItem_t effBTLDebugMenuItems[5];
void effBTLDebugPrintDistance(void *unused);

effDebugMenu_t effBTLHelperViewerInit = {
    .title = D_00533C40,
    .x = 16,
    .y = 16,
    .items = effBTLDebugMenuItems,
    .itemCount = 5,
    .mode = 1,
    .update = effBTLDebugPrintDistance,
};
