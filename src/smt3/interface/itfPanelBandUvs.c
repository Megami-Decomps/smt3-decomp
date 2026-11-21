#include "itf_panel_draw_color.h"

/* Native sprite corners: u0, v0, u1, v1 in the four-word draw record. */
itfDrawColor_t itfPanelBandTopColor = {{ 0x10, 0x10, 0x7D0, 0x168 }};
itfDrawColor_t itfPanelBandStretchColor = {{ 0x10, 0x168, 0x7D0, 0x169 }};
itfDrawColor_t itfPanelBandBottomColor = {{ 0x10, 0x168, 0x7D0, 0x2D0 }};
itfDrawColor_t itfPanelBandShortColor = {{ 0x10, 0x10, 0x7D0, 0x2D0 }};
