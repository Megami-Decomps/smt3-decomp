#include "mdlDebugData.h"

extern char mdlDebugLabelFog[8];

/* Coordinates and format for the original fog editor title. */
mdlDebugLabel_t mdlDebugScaleFogLabel __attribute__((section(".data"))) = {
    0x85C0, 0x79C0, 0, mdlDebugLabelFog
};
