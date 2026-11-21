#include "itfTestMsgSpriteData.h"

SIZE_CHECK(itfTestMsgSpriteVertexSize, sdfIVector2_t, 8);
SIZE_CHECK(itfTestMsgSpriteComponentsSize, itfColor_t, 16);

/* Two sprite corners, two UV pairs, and one RGBA modulation value. */
sdfIVector2_t D_00326BD8[2] = {
    { 0, 0 },
    { 0x2000, 0xE00 },
};
itfColor_t D_00326BE8 = { { 0, 0, 0x2000, 0x1C00 } };
itfColor_t D_00326BF8 = { { 0x80, 0x80, 0x80, 0x7F } };
