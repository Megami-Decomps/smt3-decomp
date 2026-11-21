#ifndef NB_CAMERA_MODEL_ALL_DATA_H
#define NB_CAMERA_MODEL_ALL_DATA_H

#include "common.h"

/* 12-byte model overrides indexed by unit work ID. */
typedef struct nbCamera_AllTable_t {
    f32 distance;
    f32 up;
    f32 radius;
} nbCamera_AllTable_t;

extern nbCamera_AllTable_t nbCameraModelAllDefault;

#endif
