#ifndef FLD_RESOURCE_LIGHT_H
#define FLD_RESOURCE_LIGHT_H

#include "sdf_types.h"

typedef struct fldResourceLight_t
{
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;

struct dds3Basic_t;

void dds3SetLightObjectResoruce(struct dds3Basic_t *basic, fldResourceLight_t *resource);

#endif
