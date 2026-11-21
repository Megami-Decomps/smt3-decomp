#include "interface.h"

extern const char itfMesDebugLabelLeftTop[16], itfMesDebugLabelRightBottom[16];
extern const char itfMesDebugLabelPosition[16], itfMesDebugLabelChange[16];
extern char itfMesDebugLabelAlpha[8];

/* Five debugger menu labels and their original null slot. */
const char *itfMesDebugMenuLabels[6] = {
    itfMesDebugLabelLeftTop, itfMesDebugLabelRightBottom,
    itfMesDebugLabelPosition, itfMesDebugLabelAlpha, itfMesDebugLabelChange, NULL,
};

/* The selection cursor updates these four native vertex Y coordinates. */
sdfIVector2_t itfMesSelectRectVerts[4] = {
    { 0x810, 0xBC0 }, { 0xC30, 0xBC0 },
    { 0xC30, 0xC70 }, { 0x810, 0xC70 },
};

itfColor_t itfMesSelectRectColors[2] = {
    {{ 0x57, 0, 0x10, 0 }}, {{ 0x57, 0, 0x10, 0x26 }},
};
