#include "itf_panel_meswin_data.h"

/* Sizes of the per-kind buffers allocated by itfPnlRegist2. */
s32 itfPanelPayloadSize[9] = {
    0x60, 0x1C0, 0x160, 0x60, 0x30, 0x50, 0x04, 0x04, 0x64,
};

/* Rows are reflected about the top and bottom edges of the message window. */
s32 itfPanelMesWinRowOffset[3] = { 0, 8, 0x30 };

/* Fixed-point RGB scales, with the native fourth component retained. */
itfColorVector_t itfPanelMesWinColorRatio[6] = {
    { 0xFFFFF, 0xFFFFF, 0xFFFFF, 0x1000 },
    { 0x20000, 0x20000, 0x20000, 0x1000 },
    { 0x28000, 0x28000, 0x28000, 0x1000 },
    {   0x100,   0x100,   0x100, 0x1000 },
    {  0x6000,  0x6000,  0x6000, 0x1000 },
    {  0xA000,  0xA000,  0xA000, 0x1000 },
};
