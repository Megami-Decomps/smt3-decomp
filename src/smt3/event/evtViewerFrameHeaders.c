#include "common.h"

extern char D_0054F990[8];
extern char D_0054F988[8];
extern char D_0054F980[8];
extern char D_0054F978[8];
extern char D_0054F970[8];
extern char D_0054F968[8];
extern char D_0054F960[8];
extern char D_0054F958[8];
extern char D_0054F950[8];
extern char D_0054F948[8];
extern char D_0054F940[8];
extern char D_0054F938[8];
extern char D_0054F930[8];
extern char D_0054F928[8];
extern const char evtFrameHeadSoftenBias[16];
extern char D_0054F920[8];
extern char D_0054F918[8];

/* The viewer stores these in writable pointer tables but only reads the
 * labels. Preserve const qualification on the original backing strings. */

char *frameheadstr[17] __attribute__((section(".data"), aligned(8))) = {
    D_0054F990,
    D_0054F988,
    D_0054F980,
    D_0054F978,
    D_0054F970,
    D_0054F968,
    D_0054F960,
    D_0054F958,
    D_0054F950,
    D_0054F948,
    D_0054F940,
    D_0054F938,
    D_0054F930,
    D_0054F928,
    (char *)evtFrameHeadSoftenBias,
    D_0054F920,
    D_0054F918,
};
