#include "common.h"

extern const char ragTmxPath01[32];
extern const char ragTmxPath02[32];

/* fclLoadTexture takes a char pointer but only reads the path. Keep the
 * path strings const and the native pointer table writable. */
char *ragTmxPathTable[2] __attribute__((section(".data"), aligned(8))) = {
    (char *)ragTmxPath01,
    (char *)ragTmxPath02,
};
