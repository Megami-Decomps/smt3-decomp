#include "effDebug.h"
#include "effBattle.h"

extern char D_00533A80[];
extern char D_0054F188[];
extern u32 effBTLUnitColor;
extern char D_0054F180[];
extern char D_0054F178[];
extern char D_00533A70[];
extern char D_0054F170[];
extern u8 effBTLUnitLightOn;
extern u32 effBTLUnitLightColor;
extern u32 D_0054F168[2];
extern u32 effBTLUnitAmbientColor;
extern char D_0054F160[];
extern effBTLAimParams_t effBTLAimParams;
extern char D_0054F158[];
extern u8 D_00551DB9;
extern char D_0054F150[];
extern u8 D_00551DBA;
extern char D_0054F148[];
extern sdfVector4_t effBTLLastFormationPos;
extern char D_0054F140[];
extern char D_0054F138[];
extern char D_00533A60[];
extern u8 effBTLDebugSphereOn;

/* MODEL EDIT menu entries. */
effDebugMenuItem_t D_0031C810[24] = {
    { .label = D_00533A80, .type = 19 },
    { .label = D_0054F188, .type = 9, .value = &effBTLUnitColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &effBTLUnitColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &effBTLUnitColor, .maxValue = 255, .step = 1 },
    { .type = 18 },
    { .label = D_00533A70, .type = 19 },
    { .label = D_0054F170, .type = 8, .value = &effBTLUnitLightOn, .step = 1 },
    { .label = D_0054F188, .type = 9, .value = &effBTLUnitLightColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &effBTLUnitLightColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &effBTLUnitLightColor, .maxValue = 255, .step = 1 },
    { .label = (const char *)D_0054F168, .type = 19 },
    { .label = D_0054F188, .type = 9, .value = &effBTLUnitAmbientColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F180, .type = 10, .value = &effBTLUnitAmbientColor, .maxValue = 255, .step = 1 },
    { .label = D_0054F178, .type = 11, .value = &effBTLUnitAmbientColor, .maxValue = 255, .step = 1 },
    { .label = (const char *)D_0054F168, .type = 19 },
    { .label = D_0054F160, .type = 1, .value = &effBTLAimParams, .maxValue = 5, .step = 1 },
    { .label = D_0054F158, .type = 1, .value = &D_00551DB9, .maxValue = 5, .step = 1 },
    { .label = D_0054F150, .type = 1, .value = &D_00551DBA, .maxValue = 10, .step = 1 },
    { .type = 18 },
    { .label = D_0054F148, .type = 7, .value = &effBTLLastFormationPos.x },
    { .label = D_0054F140, .type = 7, .value = &effBTLLastFormationPos.y },
    { .label = D_0054F138, .type = 7, .value = &effBTLLastFormationPos.z },
    { .type = 18 },
    { .label = D_00533A60, .type = 8, .value = &effBTLDebugSphereOn, .maxValue = 10, .step = 1 },
};
