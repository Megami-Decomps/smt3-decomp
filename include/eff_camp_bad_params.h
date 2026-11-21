#ifndef EFF_CAMP_BAD_PARAMS_H
#define EFF_CAMP_BAD_PARAMS_H

#include "sdf_types.h"

struct mdlObj_t;

typedef struct
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 scl;
    struct mdlObj_t *hModel;
} effCAMPBadParams_t;

extern effCAMPBadParams_t D_00349300;
void *effCAMPBadCreate(effCAMPBadParams_t *params, s32 id);

#endif
