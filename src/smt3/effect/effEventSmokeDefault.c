#include "effEventSmoke.h"

SIZE_CHECK(effEVECombParSmokeParamSize, effEVECombParSmokeParam_t, 0x7C);

/* Default emitter parameters copied by effEVECombParSmokeCreate2. */
effEVECombParSmokeParam_t EVE_ParSmokeParam = {
    .position = { 0.0f, -150.0f, 0.0f, 0.0f },
    .number = 250,
    .duration = 180,
    .wait = 10,
    .motionDelaySpread = 15,
    .spawnRadius = 50.0f,
    .color = 0x60808080,
    .drawMode = 1,
    .fade_in = 15,
    .fade_out = 15,
    .baseScale = 15.0f,
    .scaleRandomness = 0.5f,
    .scaleAmplitudeX = 5.0f,
    .scaleAmplitudeY = 5.0f,
    .scalePhaseStep = 0.087266453f,
    .angularSpeed = 0.0087266453f,
    .verticalSpeed = -0.099999995f,
    .verticalSpeedRandomness = 0.19999999f,
    .lateralSpeed = 0.089999997f,
    .lateralSpeedRandomness = 0.799999953f,
    .accelerationPercent = 2.75f,
    .swayPhaseStep = 0.0523598716f,
    .swayAmplitude = 0.599999965f,
    .swayAmplitudeStep = 0.099999995f,
    .swayRandomness = 0.299999983f,
};
