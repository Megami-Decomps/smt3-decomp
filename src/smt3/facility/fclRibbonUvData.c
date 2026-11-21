#include "interface.h"

/* The sprite providers use this four-word interface record for UV
 * coordinates as well as colors. Here the lanes are u0, v0, u1, v1. */
itfColor_t fclRibbonBackFrameUvBody = {{ 0x930, 0x1E0, 0xFD0, 0x430 }};
itfColor_t fclRibbonBackFrameUvEdge = {{ 0xFC0, 0x1E0, 0xFD0, 0x430 }};
itfColor_t fclRibbonBackUvBody = {{ 0x10, 0x1B0, 0x6F0, 0x1D0 }};
itfColor_t fclRibbonBackLeftUv = {{ 0x10, 0x10, 0x350, 0x1A0 }};
itfColor_t fclRibbonBackRightUv = {{ 0x340, 0x10, 0x680, 0x1A0 }};
itfColor_t fclRibbonHexEndUv = {{ 0xA90, 0x10, 0xBC0, 0x150 }};
itfColor_t fclRibbonHexMidUv = {{ 0xBB0, 0x10, 0xBC0, 0x150 }};
