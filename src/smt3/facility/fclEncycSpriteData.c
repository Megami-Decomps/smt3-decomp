#include "interface.h"

/* Native UV rectangles use two packed GS (U, V) corners in four u32 lanes. */
itfColor_t fclEncycListRowUv = { { 0xA70, 0x220, 0xAA0, 0x250 } };
itfColor_t fclEncycListIconLeftUv = { { 0xE30, 0x5D0, 0xF50, 0x660 } };
itfColor_t fclEncycListIconRightUv = { { 0xE30, 0x670, 0xF50, 0x700 } };
itfColor_t fclEncycReadMarkerOffUv = { { 0xB40, 0x470, 0xBD0, 0x4E0 } };
itfColor_t fclEncycReadMarkerOnUv = { { 0xBF0, 0x470, 0xC80, 0x4E0 } };
itfColor_t fclEncycReadSelectOffUv = { { 0xA30, 0x4F0, 0xAE0, 0x5D0 } };
itfColor_t fclEncycReadSelectOnUv = { { 0x980, 0x1F0, 0xA30, 0x2D0 } };
itfColor_t fclEncycReadGroupSeparatorUv = { { 0xCE0, 0x420, 0xDE0, 0x4F0 } };
itfColor_t fclEncycHelpPanelUv[1] = { { { 0x10, 0x610, 0xCA0, 0x7F0 } } };
itfColor_t fclEncycMainHelpOverlayUv = { { 0x6C0, 0x10, 0xA70, 0x150 } };
itfColor_t fclEncycHelpOverlayUv[1] = { { { 0x1A0, 0x4B0, 0x550, 0x5F0 } } };
itfColor_t fclEncycMoneyBoxUv = { { 0x10, 0x660, 0x3D0, 0x7A0 } };
itfColor_t fclEncycMoneyOverlayUv = { { 0x3F0, 0x660, 0x4C0, 0x7A0 } };

/* Shared neutral RGBA modulation for encyclopedia sprite draws. */
itfColor_t g_uv_summon_back[1] = { { { 0x80, 0x80, 0x80, 0x80 } } };
