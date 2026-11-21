#include "nbPanelGeometry.h"

SIZE_CHECK(nbPanelXYZEntrySize, nbPanelProcess_nbPanelXYZ_t, 0x14);

/* Main and alternate press-gauge silhouettes and blend shapes. */
nbPanelProcess_nbPanelXYZ_t nbPressGaugeTable[4] = {
    { 61, 19, -1, 34, 56 },
    { 61, 19, -1, 40, 56 },
    { 46, 22, -1, 40, 33 },
    { 54, 0, -1, 42, 47 },
};
