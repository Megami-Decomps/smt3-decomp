#include "effDebug.h"
#include "effect.h"

extern char D_0054F1B8[];
extern char D_0054F188[];
extern u32 effBTLLightColor;
extern char D_0054F180[];
extern char D_0054F178[];
extern char D_0054F1B0[];
extern dds3Effect_Filter_t effBTLFieldFilter;
extern char D_0054F1A8[];
extern char D_0054F1A0[];
extern u32 D_0054F190;
extern char D_0054F198[];
extern u8 D_0054F18A;

/* FIELD EDIT menu entries. */
effDebugMenuItem_t D_0031CAA8[17] = {
    { .label = D_0054F1B8, .type = 19 },
    { .label = D_0054F188, .type = 9, .value = &effBTLLightColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &effBTLLightColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &effBTLLightColor, .maxValue = 255, .step = 1 },
    { .type = 18 },
    { .label = D_0054F1B0, .type = 19 },
    { .label = D_0054F188, .type = 9, .value = &effBTLFieldFilter, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &effBTLFieldFilter, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &effBTLFieldFilter, .maxValue = 255, .step = 1 },
    { .label = D_0054F1A8, .type = 12, .value = &effBTLFieldFilter, .maxValue = 255, .step = 1 },
    { .type = 18 },
    { .label = D_0054F1A0, .type = 19 },
    { .label = D_0054F188, .type = 9, .value = &D_0054F190, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &D_0054F190, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &D_0054F190, .maxValue = 255, .step = 1 },
    { .type = 18 },
    { .label = D_0054F198, .type = 8, .value = &D_0054F18A, .step = 1 },
};
