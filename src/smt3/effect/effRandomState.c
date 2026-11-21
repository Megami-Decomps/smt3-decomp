#include "sdf_math_state.h"

SIZE_CHECK(dds3EffectRandStateSize, sdfRandCB_t, 0x10);

/* The effect manager seeds each generator word at startup. Its initial
 * zero state is file-backed, independent of the default/particle states. */
sdfRandCB_t dds3EffectBase[1] = {
    { { 0, 0, 0, 0 } },
};
