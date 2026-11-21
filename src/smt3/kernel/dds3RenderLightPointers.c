#include "draw3d.h"

extern sdf3DLight_t D_002F5C50;
extern sdf3DLight_t D_002F5C90;
extern sdf3DLight_t D_002F5CD0;

/* Overlay light packet setup consumes precisely these three entries. */
sdf3DLight_t *dds3RenderLightParallel[3] = {
    &D_002F5C50, &D_002F5C90, &D_002F5CD0,
};
