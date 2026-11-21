#include "dds3_config_value_labels.h"

extern const char nmeVibrationLabel[16];
extern const char nmeMenuMemoryLabel[16];
extern const char nmeAutoMemoryLabel[16];
extern char D_005516F0[8];

configValueLabel_t dds3ConfigValueLabels[4] = {
    { nmeVibrationLabel, 2 },
    { nmeMenuMemoryLabel, 3 },
    { nmeAutoMemoryLabel, 3 },
    { D_005516F0, 0 },
};
