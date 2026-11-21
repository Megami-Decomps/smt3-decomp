#include "nbPanelGeometry.h"

SIZE_CHECK(nbPanelXYZEntrySize, nbPanelProcess_nbPanelXYZ_t, 0x14);

/* Frame and arrow rectangles shared by the stock and party selectors. */
nbPanelProcess_nbPanelXYZ_t nbStockSelectTable[8] = {
    { 0, 0, -1, 224, 15 },
    { 0, 15, -1, 224, 77 },
    { 28, -3, -1, 253, 3 },
    { 28, 92, -1, 253, 3 },
    { 7, 20, -1, 7, 9 },
    { 14, 83, -1, -7, -9 },
    { 28, -3, -1, 253, 3 },
    { 28, 92, -1, 253, 3 },
};
