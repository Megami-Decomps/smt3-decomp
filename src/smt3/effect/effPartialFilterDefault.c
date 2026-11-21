#include "effect.h"

SIZE_CHECK(partialFilterDefaultSize, dds3Effect_PartialMultiFilterParam_t, 0x24);

dds3Effect_PartialMultiFilterParam_t mfilterDefaultParam = {
    .area = 150,
    .center = { 0, 0 },
    .Filter = { { { 0x80, 0x80, 0x80, 0x40 } }, 0x48, { 0, 0, 0, 0 } },
};
