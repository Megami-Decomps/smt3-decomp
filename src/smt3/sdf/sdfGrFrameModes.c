#include "draw.h"

/* Screen geometry and GS frame mode, indexed by sdfGrSetFrameSetMode. */
sdfGrScreenDef_t sdfGrFrameModes[4] = {
    { 640, 448, 224, 0, 48, 2 },
    { 512, 448, 224, 0, 48, 2 },
    { 640, 480, 240, 0, 48, 2 },
    { 512, 480, 240, 0, 48, 2 },
};
