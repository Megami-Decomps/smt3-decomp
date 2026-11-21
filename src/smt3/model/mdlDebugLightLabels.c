#include "mdlDebugData.h"

extern char mdlDebugLabelLight0[8], mdlDebugLabelLight1[8], mdlDebugLabelLight2[8];
extern const char mdlDebugLabelAmbient[16];

SIZE_CHECK(mdlDebugLabelSize, mdlDebugLabel_t, 12);

mdlDebugLabel_t mdlDebugLightLabels[4] = {
    { 0x8680, 0x79C0, 0, mdlDebugLabelLight0 },
    { 0x8680, 0x7CC0, 0, mdlDebugLabelLight1 },
    { 0x8680, 0x7FC0, 0, mdlDebugLabelLight2 },
    { 0x8680, 0x82C0, 0, mdlDebugLabelAmbient },
};
