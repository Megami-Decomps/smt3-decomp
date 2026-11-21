#include "newbattle.h"

SIZE_CHECK(nbPanelAutoUvEntrySize, nbPanelProcess_nbPanelTMXuv_t, 0x10);

/* Texture rectangles for the auto-battle prompt and its background. */
nbPanelProcess_nbPanelTMXuv_t nbPanelAutoUV[3] = {
    { 65, 93, 63, 16 },
    { 1, 93, 63, 16 },
    { 114, 72, 118, 18 },
};
