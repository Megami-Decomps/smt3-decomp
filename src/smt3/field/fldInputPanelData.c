#include "common.h"

/* One label and cursor X coordinate for each input-panel message type. */
char fldInputPanelLabel[12][16] = {
    "A", "Talk", "Open Door", "Jump Off", "Enter", "Ride",
    "Climb Ladder", "Descend Ladder", "Open", "Examine", "Exit", "Enter",
};

s32 fldInputPanelX[12] = {
    250, 236, 208, 217, 230, 240, 195, 180, 227, 224, 236, 230,
};
