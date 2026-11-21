#include "nbPanelGeometry.h"

SIZE_CHECK(nbPanelXYZEntrySize, nbPanelProcess_nbPanelXYZ_t, 0x14);

/* Press-turn counter background and face. */
nbPanelProcess_nbPanelXYZ_t nbPressGaugeTable2[2] = {
    { 79, 17, -1, 75, 52 },
    { 86, 20, -1, 66, 15 },
};
