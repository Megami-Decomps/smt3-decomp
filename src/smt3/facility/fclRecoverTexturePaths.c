#include "common.h"

extern const char rcvTmxPath01[35];
extern const char rcvTmxPath02[35];
extern const char rcvTmxPath03[35];

/* fclLoadTexture reads the path without modifying it. */
char *rcvTmxPathTable[3] = {
    (char *)rcvTmxPath01, (char *)rcvTmxPath02, (char *)rcvTmxPath03,
};
