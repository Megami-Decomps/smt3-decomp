#include "pbBoardData.h"

/* Four cell edges, each with native four-component endpoints. */
pbOutlineEdge_t pbOutlineEdgeTable[4] = {
    { { 20.0f, 0.0f, 20.0f, 0.0f }, { -20.0f, 0.0f, 20.0f, 0.0f } },
    { { -20.0f, 0.0f, -20.0f, 0.0f }, { 20.0f, 0.0f, -20.0f, 0.0f } },
    { { 20.0f, 0.0f, -20.0f, 0.0f }, { 20.0f, 0.0f, 20.0f, 0.0f } },
    { { -20.0f, 0.0f, 20.0f, 0.0f }, { -20.0f, 0.0f, -20.0f, 0.0f } },
};

s32 pbMapCursorBaseTable[4][4] = {
    { 0x0D20, 0x00D0, 0x12D0, 0x0228 },
    { 0x0120, 0x0578, 0x03D0, 0x0770 },
    { 0x0D20, 0x0AC0, 0x12D0, 0x0C18 },
    { 0x1C30, 0x0578, 0x1EE0, 0x0770 },
};

s32 pbMapCursorUvTable[4][4] = {
    { 0x1490, 0x1C10, 0x1A40, 0x1EC0 },
    { 0x0EE0, 0x1C10, 0x1190, 0x2000 },
    { 0x1A50, 0x1C10, 0x2000, 0x1EC0 },
    { 0x11C0, 0x1C10, 0x1470, 0x2000 },
};

s32 pbMapCursorColorTable[4][4] = {
    { 0x0080, 0x0080, 0x0080, 0x0080 },
    { 0x0080, 0x0080, 0x0080, 0x0080 },
    { 0x0080, 0x0080, 0x0080, 0x0080 },
    { 0x0080, 0x0080, 0x0080, 0x0080 },
};
