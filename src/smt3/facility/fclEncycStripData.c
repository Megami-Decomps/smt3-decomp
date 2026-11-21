#include "interface.h"

typedef struct { s32 left, right; } fclEncycStrip_t;

itfColor_t fclEncycRegistStripColor = {{ 0, 0, 0, 0x40 }};
fclEncycStrip_t fclEncycRegistStrips[5] = {
    { 0x610, 0x9D0 },
    { 0xA40, 0x1120 },
    { 0x1190, 0x15B0 },
    { 0x15F0, 0x1A00 },
    { 0x1A40, 0x1C20 },
};

itfColor_t fclEncycCaptionStripColor = {{ 0, 0, 0, 0x40 }};
itfColor_t fclEncycCaptionLastStripColor = {{ 0x2F, 0x1F, 0x1F, 0x80 }};
fclEncycStrip_t fclEncycCaptionStrips[4] = {
    { 0x610, 0x9D0 },
    { 0xA40, 0x11C0 },
    { 0x1230, 0x1410 },
    { 0x1640, 0x1B50 },
};
