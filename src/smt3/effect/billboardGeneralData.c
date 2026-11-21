#include "billboardGeneralData.h"

extern const char billPathBil00[16];
extern const char billPathSmoke16[18];
extern const char billPathMblur8[17];
extern const char billPathMblur8_3[19];
extern const char billPathSmoke07[18];
extern const char billPathSmoke08[18];
extern const char billPathSmoke10[18];
extern const char billPathSmoke12[18];

billGeneralRow_t dds3BillboardGeneralRows[8] = {
    { billPathBil00,   0, 2 },
    { billPathSmoke16, 0, 1 },
    { billPathMblur8,  0, 1 },
    { billPathMblur8_3, 0, 1 },
    { billPathSmoke07, 0, 1 },
    { billPathSmoke08, 0, 1 },
    { billPathSmoke10, 0, 1 },
    { billPathSmoke12, 0, 1 },
};
