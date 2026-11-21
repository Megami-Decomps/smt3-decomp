#include "common.h"

extern const char mselTitleCursor0Line0[32];
extern const char mselTitleCursor1Line0[22];
extern const char mselTitleCursor1Line1[27];
extern const char mselTitleCursor1Line2[21];
extern u32 D_0054FD40;

/* The first description has one sentence and two encoded blank lines. */
const char *mselTitleCursor0Lines[4] = {
    mselTitleCursor0Line0, (const char *)&D_0054FD40, (const char *)&D_0054FD40, NULL,
};
const char *mselTitleCursor1Lines[4] = {
    mselTitleCursor1Line0, mselTitleCursor1Line1, mselTitleCursor1Line2, NULL,
};
