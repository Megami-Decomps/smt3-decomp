#include "effect.h"

/* The battle field-color editor and solid-rectangle draw share this preset. */
dds3Effect_Filter_t effBTLFieldFilter = {
    .abgr = 0x184A4442,
    .blend = 0x48,
    .xy = { { 0, 0 }, { 512, 448 } },
};
