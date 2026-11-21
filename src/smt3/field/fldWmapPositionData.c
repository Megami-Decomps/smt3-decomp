#include "sdf_types.h"

/* World-map interpolation updates the XYZ lanes and retains homogeneous W. */
sdfVector4_t fldWmapPlayerPos = { 0.0f, -1.0f, 0.0f, 1.0f };
sdfVector4_t fldWmapPosFrom = { 0.0f, -1.0f, 0.0f, 1.0f };
sdfVector4_t fldWmapPosTo = { 0.0f, -1.0f, 0.0f, 1.0f };
