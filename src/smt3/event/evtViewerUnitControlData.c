#include "common.h"

extern char evtViewerUnitControlNameDispOn[8];
extern const char evtViewerUnitDispOff[16];
extern const char evtViewerUnitShadowOff[16];
extern const char evtViewerUnitMshadowOn[16];
extern const char evtViewerUnitVshadowOn[16];

/* The viewer stores these in writable pointer tables but only reads the
 * labels. Preserve const qualification on the original backing strings. */

char *evtViewerUnitControlNames[6] __attribute__((section(".data"), aligned(8))) = {
    evtViewerUnitControlNameDispOn,
    (char *)evtViewerUnitDispOff,
    (char *)evtViewerUnitShadowOff,
    (char *)evtViewerUnitMshadowOn,
    (char *)evtViewerUnitVshadowOn,
    NULL,
};
