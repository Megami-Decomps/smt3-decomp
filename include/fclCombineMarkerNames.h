#ifndef FCL_COMBINE_MARKER_NAMES_H
#define FCL_COMBINE_MARKER_NAMES_H

#include "common.h"

typedef struct {
    const char *model[3];
    const char *initialCamera[1];
} cmbMarkerNames_t;
SIZE_CHECK(cmbMarkerNames_size_must_be_0x10, cmbMarkerNames_t, 0x10);

extern cmbMarkerNames_t cmbModelMarkNames;
/* Native link alias to the one in-bounds initial-camera pointer. */
extern const char *cmbCameraPointNames[];

#endif
