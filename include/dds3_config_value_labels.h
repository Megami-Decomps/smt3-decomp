#ifndef DDS3_CONFIG_VALUE_LABELS_H
#define DDS3_CONFIG_VALUE_LABELS_H

#include "common.h"

typedef struct {
    const char *text;
    s32 optionCount;
} configValueLabel_t;
extern configValueLabel_t dds3ConfigValueLabels[];

#endif
