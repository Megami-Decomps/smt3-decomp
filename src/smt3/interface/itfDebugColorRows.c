#include "pbDebugColorRows.h"

SIZE_CHECK(pbDebugColorRowSize, pbColorRow_t, 8);

extern char D_0054F580[4];
extern char D_0054F578[8];
extern char D_0054F570[8];
extern char D_0054F568[8];
extern char D_0054F560[8];

/* The filter panel stores a speed label after the four drawn channel labels. */
pbColorRow_t D_00326C20[5] = {
    { D_0054F580, 2 },
    { D_0054F578, 4 },
    { D_0054F570, 1 },
    { D_0054F568, 0 },
    { D_0054F560, 0 },
};
