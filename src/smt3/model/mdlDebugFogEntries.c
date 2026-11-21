#include "kernel_draw.h"

typedef struct mdlDebugEntry_t {
    s32 y;
    f32 *target;
    s32 min;
    s32 max;
    const char *label;
    const char *format;
} mdlDebugEntry_t;

extern dds3LightState_t dds3LightParam;
extern char mdlDebugLabelMax[8], mdlDebugLabelNear[8];
extern char mdlDebugLabelMin[8], mdlDebugLabelFar[8], mdlDebugIntFormat[8];

/* The editor uses all four rows and directly updates the live fog state. */
mdlDebugEntry_t mdlDebugEntryTable[4] = {
    { 0x7A20, &dds3LightParam.fog.fogMax,  0, 255,    mdlDebugLabelMax,  mdlDebugIntFormat },
    { 0x7A80, &dds3LightParam.fog.fogNear, 0, 999999, mdlDebugLabelNear, mdlDebugIntFormat },
    { 0x7B40, &dds3LightParam.fog.fogMin,  0, 255,    mdlDebugLabelMin,  mdlDebugIntFormat },
    { 0x7BA0, &dds3LightParam.fog.fogFar,  0, 999999, mdlDebugLabelFar,  mdlDebugIntFormat },
};
