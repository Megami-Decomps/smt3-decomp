#include "effect.h"

dds3Effect_PartialMultiFilterParam_t evtMultFilterDefaultParam = {
    .area = 150,
    .center = { 0, 0 },
    .Filter = { { { 0x80, 0x80, 0x80, 0 } }, 0x48, { 0, 0, 0, 0 } },
};
