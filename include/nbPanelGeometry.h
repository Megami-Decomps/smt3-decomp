#ifndef NB_PANEL_GEOMETRY_H
#define NB_PANEL_GEOMETRY_H

#include "common.h"

/* A panel rectangle: position, then width and height. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 h;
} nbPanelProcess_nbPanelXYZ_t;

extern nbPanelProcess_nbPanelXYZ_t nbPanelPolyXYZ[46];
extern nbPanelProcess_nbPanelXYZ_t nbPanelAutoGlowXYZ[2];

#endif
